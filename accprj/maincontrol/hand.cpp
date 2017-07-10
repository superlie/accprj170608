#include "hand.h"
#include "ui/accui.h"

extern AccUi *accui;
extern QTimer *g_light_timer;

HandCon::HandCon()
{
    if (!ReadOwnFile())
    {
        ;//interlock
    }
    gantry_rotat_mode = false;
    collim_rotat_mode = false;
    jawa_mode = false;
    jawb_mode = false;



    req_act_or_pre_data = false;
    key_release = 0;
    motor_speed_request = NO_SPEED;
    gan_coli_jaw_mode = NULL_MODE;
    gantry_enable_count = 0;
    preset_position = 0;


    beam_shield_mode = false;
    field_light_tog = false;
    room_light_tog = false;
    range_finder_tog = false;
    back_pointer_tog = false;

    hand_gantry_control = false;
    hand_colli_control = false;
    hand_jawx1_control = false;
    hand_jawx2_control = false;
    hand_jawy1_control = false;
    hand_jawy2_control = false;

    have_send_movenow = false;//11-26
    //field_light_open = false;

    mlc_bank_1 = 0;
    mlc_bank_2 = 0;
    mlc_speed_mode_sel = 0;
    console_mlc_disp = false;
    clear_mlc_interlock = false;
    enable_active = false;
    mlc_block_mode = false;
    table_position_request = 0;
    enable_active_and_motion = false;
    jaws_mode = OPEN_JAWS;
    gantry_mode = CW_GANTRY;
    error_report = 0;
    prom_version = 0;
    deselect = false;

    send_frame_num = 0;
    rev_frame_num = 0;

    mode_now = RESET_MODE;
}

HandCon::~HandCon()
{

}

bool HandCon::ReadOwnFile()
{
    QFile file(".\\AccConfig\\Hand.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            return false;
        }
        reader.setDevice(&file);
        reader.readNext();

        while (!reader.atEnd())
        {
            if (reader.isStartElement())
            {
                if (reader.name() == "Hand")
                {
                    ReadOwnElement();
                }
                else
                {
                    //interlock
                    return false;
                }
            }
            else
            {
                reader.readNext();
            }
        }
        file.close();
    }
    catch (QString exception)
    {
        //interlock
        return false;
    }
    return true;
}

bool HandCon::ReadOwnElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();
        //        if (reader.isEndElement())
        //            break;

        if (reader.isStartElement())
        {
            while (!reader.isEndElement())
            {
                if (reader.name() == "zxt")
                    zxt = (ZXT_T)reader.readElementText().toInt();
                else if (reader.name() == "jaw_labeling")
                    jaw_labeling = (JAW_LABELING_T)reader.readElementText().toInt();
                else if (reader.name() == "mlc")
                    mlc = (MLC_T)reader.readElementText().toInt();

                reader.readNext();
            }
        }
    }

    return true;
}

bool HandCon::WriteOwnFile()
{
    QFile file(".\\AccConfig\\Hand.xml");

    try
    {
        if (!file.open(QFile::ReadWrite | QFile::Text))
        {
            //interlock
            return false;
        }
        if (!doc.setContent(&file))
        {
            //interlock
            file.close();
            return false;
        }
        file.close();
        root = doc.documentElement();
        while (!root.isNull())
        {
            if (root.tagName() == "Hand")
            {
                WriteOwnElement();
                file.open(QFile::WriteOnly | QFile::Truncate);
                QTextStream ts(&file);
                ts.reset();
                ts.setCodec("utf-8");
                doc.save(ts, 4, QDomNode::EncodingFromTextStream);

            }
            root = root.nextSibling().toElement();
        }
        file.close();
        return true;

    }
    catch (QString exception)
    {
        //interlock
        return false;
    }
    return true;
}

bool HandCon::WriteOwnElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "zxt")
            node.firstChild().setNodeValue(QString::number(zxt));

        else if (node.nodeName() == "jaw_labeling")
            node.firstChild().setNodeValue(QString::number(jaw_labeling));

        else if (node.nodeName() == "mlc")
            node.firstChild().setNodeValue(QString::number(mlc));

        node = node.nextSibling();
    }

    return true;
}

bool HandCon::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
{
    UINT8 low = 0, tmplow = 0;
    if (len > 18)
        return false;

    fc = send_frame_num << 4;
    send_frame_num = (send_frame_num + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return true;
}

UINT8 HandCon::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    fc = rev_frame_num << 4;
    rev_frame_num = (rev_frame_num + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

bool HandCon::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x24;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe4;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool HandCon::SendInvalidPosition()
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x20;
    tmpSend.data[2] = 0x29;
    tmpSend.data[3] = 0x28;
    QMutexLocker locker(&hand_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;
    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0],5);
    tmpSend.len = 6;
    return InsertOutQueue(tmpSend);
}

bool HandCon::SendValidPosition()
{

    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x20;
    tmpSend.data[2] = 0x28;
    tmpSend.data[3] = 0x29;
    QMutexLocker locker(&hand_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;
    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0],5);
    tmpSend.len = 6;
    return InsertOutQueue(tmpSend);
}


bool HandCon::SendPosition(float position, POSITION_TYPE_T type,CTRL_MODE_T ctrlmode)
{
    int tmp = qAbs(position*10);
    UINT8 digit[4];

    for(int i=3;i>=0;i--)
    {
        digit[i] = tmp%10+48;//转换为ascii码
        tmp = tmp/10;
    }

    switch (ctrlmode)
    {
    case SYN_JAWA_MODE:
    case SYN_JAWB_MODE:
        digit[0]=0x20;      //空格
        break;
    case JAWA_1_MODE:       //Y1
    case JAWB_2_MODE:       //X1
        if(position>0)  digit[0]=0x2d;      //+ -> -
        else digit[0]=0x2b;                 //- -> +
        break;
    case JAWA_2_MODE:
    case JAWB_1_MODE:
        if(position>0)  digit[0]=0x2b;      //+
        else digit[0]=0x2d;                 //-
        break;
    default:
        break;
    }

    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x20;

    tmpSend.data[2] = digit[0];   //
    tmpSend.data[3] = digit[1];
    tmpSend.data[4] = digit[2];
    tmpSend.data[5] = 0x2e;
    tmpSend.data[6] = digit[3];
    tmpSend.data[7] = type;
    tmpSend.data[8] = ctrlmode;

    QMutexLocker locker(&hand_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 9, tmpSend.data[9]))
        return false;
    tmpSend.data[10] = ComputeLstCHK(&tmpSend.data[0],10);
    tmpSend.len = 11;

    return InsertOutQueue(tmpSend);
}

bool HandCon::SendUserMode(const USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&hand_mutex);
    if (mode_now == RESET_MODE)
    {
        send_frame_num = 0;         //发送reset命令，将序列号清零
        //rev_frame_num = 0;
    }

    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool HandCon::SendConfiguration()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x41;     //A
    //    tmpSend.data[2] = zxt;
    //    tmpSend.data[3] = jaw_labeling;
    //    tmpSend.data[4] = mlc;

    tmpSend.data[2] = 0x01;
    tmpSend.data[3] = 0x00;
    tmpSend.data[4] = 0x00;

    QMutexLocker locker(&hand_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 5, tmpSend.data[5]))
        return false;
    tmpSend.data[6] = ComputeLstCHK(&tmpSend.data[0], 6);
    tmpSend.len = 7;

    return InsertOutQueue(tmpSend);
}

bool HandCon::SendDeselectCurTableFunction()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOHAND;
    tmpSend.data[1] = 0x44;
    QMutexLocker locker(&hand_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

//分析收到的命令，因为命令长度短，直接在函数中实现了。
bool HandCon::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C')
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch (revdata.data[1])
    {
    case 0x0B:
        return RevGantryRotationMode(revdata);
    case 0x0C:
        return RevColimatorRotationMode(revdata);
    case 0x0D:
        return RevJawAMode(revdata);
    case 0x0E:
        return RevJawBMode(revdata);
    case 0x0F:
        return RevBeamShield(revdata);
    case 0x10:
        return RevFieldLightToggle(revdata);
    case 0x11:
        return RevRoomLightToggle(revdata);
    case 0x12:
        return RevRangeFinderToggle(revdata);
    case 0x13:
        return RevBackPointerToggle(revdata);
    case 0x15:      //in ASCII
        return RevMLCBankSelect(revdata);
    case 0x16:
        return RevMLCSpeedSelect(revdata);
    case 0x17:
        return RevDisplayMLCToggle(revdata);
    case 0x1A:
        return RevClearMLCInterlocks(revdata);
    case 0x1D:
        return RevEnableDeadmanNoSelect(revdata);
    case 0x20:
        return RevMLCModeToggle(revdata);
    case 0x28:
        return RevRequestActOrPreData(revdata);
    case 0x2C:
        return RevKeyReleaseNotifi(revdata);
    case 0x53:  //'S'
        return RevMotorSpeedRequest(revdata);
    case 0x54:
        return RevTablePositionRequest(revdata);
    case 0x1C:
        return RevEnableDeadmanSelected(revdata);
    case 0x14:
        return RevOpenJaw(revdata);
    case 0x18:
        return RevCloseJaw(revdata);
    case 0x34:
        return RevCW(revdata);
    case 0x38:
        return RevCCW(revdata);
    case 0x19:  //non-ascii

        return RevPrePositData(revdata);
    case 'E':
        return RevErrorReport(revdata);
    case 'C':
        return RevResetEcho(revdata);
    case 'D':
        return RevDeSelctCurHandCtr(revdata);
    default:   //Invalid command type received from controller 4
        InsertInterlock(79, 954, ERROR_IL);
    }
    return false;
}

void HandCon::ProcessErrorIL()
{
    switch (error_report)
    {
    case 0x0:   //可能是0x5    LED RAM test failed
        InsertInterlock(67, 405, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x15:  //RAM test failed
        InsertInterlock(67, 421, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x16:  //ROM test failed
        InsertInterlock(67, 422, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x17:  //PDF没有
        InsertInterlock(67, 423, ERROR_IL, IL_ALL, LATCHED);
        break;
        /*
        497   Frame-# and checksum received by controller 4 are wrong .
        498   Checksum received by controller 4 is wrong .
        499   Frame-# received by controller 4 is wrong.
         */
    case 0x61:  //PDF没有
        InsertInterlock(79, 497, ERROR_IL);
        break;
    case 0x62:  //PDF没有
        InsertInterlock(79, 498, ERROR_IL);
        break;
    case 0x63:  //PDF没有
        InsertInterlock(79, 499, ERROR_IL);
        break;
    default:        //Invalid data received from controller 4 安全联锁
        InsertInterlock(79, 974, ERROR_IL);
        break;
    }

}

bool HandCon::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 934, ERROR_IL);       // Invalid frame number detected from controller 4
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 944, ERROR_IL);       //Invalid checksum detected from controller 4

        return false;
    }
    return true;
}

//在motor接收到31命令返回position时调用
void HandCon::UpdatePositionToHand()
{
    float tmp,tmp1,tmp2;
    switch(collimatorOrjaw_start){
    case COLLIMATOR_START:

        tmp = ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);//从motor中获取act position
        if (tmp < 0.0)
            tmp += 360.0;
        hand->SendPosition(tmp,UPDATE_POSITION,COLLIMATOR_MODE);//主机发送20h到手柄带有update position data
        break;

    case SYN_JAWA_START:

        tmp1 = ConvertMemberToFloat(motor->position[JAW_A1], JAWY1_CK);//从motor中获取act position
        tmp2 = ConvertMemberToFloat(motor->position[JAW_A2], JAWY2_CK);
        hand->SendPosition(tmp1+tmp2,UPDATE_POSITION,SYN_JAWA_MODE);//主机发送20h到手柄带有update position data
        break;


    case JAWA_1_START:
        tmp1 = ConvertMemberToFloat(motor->position[JAW_A1], JAWY1_CK);
        if(tmp1>-0.1 && tmp1<0.1) tmp1=0;
        hand->SendPosition(tmp1,UPDATE_POSITION,JAWA_1_MODE);
        break;
    case JAWA_2_START:
        tmp2 = ConvertMemberToFloat(motor->position[JAW_A2], JAWY2_CK);
        if(tmp2>-0.1 && tmp2<0.1) tmp2=0;
        hand->SendPosition(tmp2,UPDATE_POSITION,JAWA_2_MODE);
        break;
    case SYN_JAWB_START:

        tmp1 = ConvertMemberToFloat(motor->position[JAW_B1], JAWX2_CK);//从motor中获取act position
        tmp2 = ConvertMemberToFloat(motor->position[JAW_B2], JAWX1_CK);
        hand->SendPosition(tmp1+tmp2,UPDATE_POSITION,SYN_JAWB_MODE);//主机发送20h到手柄带有update position data
        break;
    case JAWB_1_START:
        tmp1 = ConvertMemberToFloat(motor->position[JAW_B1], JAWX2_CK);
        if(tmp1>-0.1 && tmp1<0.1) tmp1=0;
        hand->SendPosition(tmp1,UPDATE_POSITION,JAWB_1_MODE);
        break;
    case JAWB_2_START:
        tmp2 = ConvertMemberToFloat(motor->position[JAW_B2], JAWX1_CK);
        if(tmp2>-0.1 && tmp2<0.1) tmp2=0;
        hand->SendPosition(tmp2,UPDATE_POSITION,JAWB_2_MODE);
        break;
    default:
        break;
    }

}

bool HandCon::RevGantryRotationMode(TransRecvT &revdata)//手柄控制Gantry处理
{
    bool ret;
    float tmp;

    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }

    gantry_rotat_mode = true;
    collim_rotat_mode = false;
    jawa_mode = false;
    jawb_mode = false;

    tmp = ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);//从motor中获取act position
    if (tmp < 0.0)
        tmp += 360.0;
    ret = SendPosition(tmp,ACTUAL_POSITION,GANTRY_MODE);//主机发送20h到手柄带有act position

    gan_coli_jaw_mode = GANTRY_MODE;
    return true;
}

bool HandCon::RevColimatorRotationMode(TransRecvT &revdata)//手柄控制Colimator处理
{
    float tmp;
    //float tmp1,tmp2;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }

    gan_coli_jaw_mode = COLLIMATOR_MODE;

    //    hand_colli_control = true;
    //    accui->UpdateFuncKey();

    motor->SendStop(GANTRY);

    collim_rotat_mode = true;
    gantry_rotat_mode = false;
    jawa_mode = false;
    jawb_mode = false;

    tmp = ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);//从motor中获取act position
    if (tmp < 0.0)
        tmp += 360.0;

    SendPosition(tmp,ACTUAL_POSITION,COLLIMATOR_MODE);//主机发送20h到手柄带有act position data

    //qDebug()<<"Colimator SendPosition(ACT):"<<tmp;
    return true;
}

bool HandCon::RevJawAMode(TransRecvT &revdata)
{
    int mode;
    float tmp1,tmp2;
    if (revdata.len != 5 && revdata.len != 6)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    mode = revdata.data[2];


    gantry_rotat_mode = false;
    collim_rotat_mode = false;
    jawa_mode = true;
    jawb_mode = false;

    tmp1 = ConvertMemberToFloat(motor->position[JAW_A1],JAWY1_CK);
    tmp2 = ConvertMemberToFloat(motor->position[JAW_A2],JAWY2_CK);
    switch (mode) {
    case 0x16:
        gan_coli_jaw_mode = JAWA_1_MODE;
        SendPosition(tmp1,ACTUAL_POSITION,JAWA_1_MODE);
        break;
    case 0x17:
        gan_coli_jaw_mode = JAWA_2_MODE;
        SendPosition(tmp2,ACTUAL_POSITION,JAWA_2_MODE);
        break;
    default:
        gan_coli_jaw_mode = SYN_JAWA_MODE;
        SendPosition(tmp1+tmp2,ACTUAL_POSITION,SYN_JAWA_MODE);
        break;
    }



    return true;
}

bool HandCon::RevJawBMode(TransRecvT &revdata)
{
    int mode;
    float tmp1,tmp2;
    if (revdata.len != 5 && revdata.len != 6)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    mode = revdata.data[2];

    gantry_rotat_mode = false;
    collim_rotat_mode = false;
    jawa_mode = false;
    jawb_mode = true;

    tmp1 = ConvertMemberToFloat(motor->position[JAW_B1],JAWX2_CK);
    tmp2 = ConvertMemberToFloat(motor->position[JAW_B2],JAWX1_CK);

    switch(mode){
    case 0x16:
        gan_coli_jaw_mode = JAWB_1_MODE;
        SendPosition(tmp1,ACTUAL_POSITION,JAWB_1_MODE);
        break;
    case 0x17:
        gan_coli_jaw_mode = JAWB_2_MODE;
        SendPosition(tmp2,ACTUAL_POSITION,JAWB_2_MODE);
        break;
    default:
        gan_coli_jaw_mode = SYN_JAWB_MODE;
        SendPosition(tmp1+tmp2,ACTUAL_POSITION,SYN_JAWB_MODE);
        break;
    }


    return true;
}

bool HandCon::RevBeamShield(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    beam_shield_mode = true;
    return true;

}

//0x10
bool HandCon::RevFieldLightToggle(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    field_light_tog = true;

    //LJY-12-24
    if(!light->light_on)
        base->g_lighton_start_signal();

    if(io->mirror_default_pos)
    {
        //每次发送开关灯命令，都要启动等待接受开关灯状态报告的定时器
        light->SendFieldIlluminate();
        base->g_lightrep_start_signal();
    }
    else
        io->SendSetupFieldLight();

    return true;
}

//0x11
bool HandCon::RevRoomLightToggle(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    room_light_tog = true;
    light->SendRoomLight(); //11-25     0x11
    return true;
}

//0x12
bool HandCon::RevRangeFinderToggle(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    range_finder_tog = true;
    light->SendOpticalRangeFinder();    //11-25     0x12
    return true;
}

bool HandCon::RevBackPointerToggle(TransRecvT &revdata)
{
    return true;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    back_pointer_tog = true;
    light->SendOpticalBackPointer();    //11-25     0x13
    return true;

}

bool HandCon::RevMLCBankSelect(TransRecvT &revdata)
{
    if (revdata.len != 7)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    if (revdata.data[2] == 0x01)
    {
        mlc_bank_1 = (revdata.data[3] - '0') * 10 +
                (revdata.data[4] - '0');
    }
    else if (revdata.data[3] == 0x02)
    {
        mlc_bank_2 = (revdata.data[3] - '0') * 10 +
                (revdata.data[4] - '0');
    }
    return true;
}

bool HandCon::RevMLCSpeedSelect(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    mlc_speed_mode_sel =  revdata.data[2];
    return true;
}

bool HandCon::RevDisplayMLCToggle(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    console_mlc_disp = true;
    return true;
}

bool HandCon::RevClearMLCInterlocks(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    clear_mlc_interlock = true;
    return true;
}

bool HandCon::RevEnableDeadmanNoSelect(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    enable_active = true;
    return true;
}
void HandCon::ClearGantryFlags(void)
{
    //清理标志量

    //gantry_rotat_mode = false;


    key_release = 0;
    gantry_enable_count = 0;
    motor_speed_request = NO_SPEED;
    req_act_or_pre_data = false;
}

/*
bool HandCon::RevEnableDeadmanSelected(TransRecvT &revdata)
{
    UINT32 gantry_position;
    float tmp;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);     //Invalid length for command type from controller 4.
        ClearGantryFlags();
        return false;
    }

    enable_active_and_motion = true;

    if(gantry_rotat_mode)
    {
        if(++gantry_enable_count == 1)
        {
            key_release = 0;

            //12-01
            if(motor->CheckConvertGantry(preset_position,gantry_position))
            {
                motor->SendMoveNow(GANTRY,(UINT16)gantry_position);//主机向Motor发送p（50）命令move now
                hand_gantry_control = true;
                accui->UpdateFuncKey();
            }
            else
                SendInvalidPosition();
        }
        else
        {
            tmp = ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);//从motor中获取act position
            if (tmp < 0.0)
                tmp += 360.0;
            SendPosition(tmp,UPDATE_POSITION,GANTRY_MODE);//主机发送20h到手柄带有update position data
        }
    }
    return true;
}
*/

//12-03 new
bool HandCon::RevEnableDeadmanSelected(TransRecvT &revdata)
{
    UINT32 gantry_position;
    float tmp;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);     //Invalid length for command type from controller 4.
        ClearGantryFlags();
        return false;
    }

    if(gantry_rotat_mode)
    {
        if(!have_send_movenow)
        {
            key_release = 0;
            if(motor->CheckConvertGantry(preset_position,gantry_position))
            {
                motor->SendMoveNow(GANTRY,(UINT16)gantry_position,motor_speed_request);//主机向Motor发送p（50）命令move now
                hand_gantry_control = true;
                have_send_movenow = true;
                motor_speed_request = NO_SPEED;
                accui->UpdateFuncKey();
            }
            else
                SendInvalidPosition();
        }
        else
        {
            tmp = ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);//从motor中获取act position
            if (tmp < 0.0)
                tmp += 360.0;
            SendPosition(tmp,UPDATE_POSITION,GANTRY_MODE);//主机发送20h到手柄带有update position data
        }
    }

    return true;
}

bool HandCon::RevMLCModeToggle(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    mlc_block_mode = true;
    return true;
}

bool HandCon::RevRequestActOrPreData(TransRecvT &revdata)
{

    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    req_act_or_pre_data = true;

    return true;

}

bool HandCon::RevKeyReleaseNotifi(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    key_release = revdata.data[2];

    if(key_release == 0x02)//Enable Released
    {
        motor->SendStop(GANTRY);
        have_send_movenow = false;
        ClearGantryFlags();
    }
    if(key_release == 0x03)//CCW Released
    {
        switch(gan_coli_jaw_mode){
        case GANTRY_MODE:
            motor->SendStop(GANTRY);
            have_send_movenow = false;
            ClearGantryFlags();
            break;
        case COLLIMATOR_MODE:
            motor->SendStop(COLLIMATOR);
            have_send_movenow = false;
            break;
        case SYN_JAWA_MODE:
            motor->SendStop(JAW_A1);
            motor->SendStop(JAW_A2);
            have_send_movenow = false;
            break;
        case JAWA_1_MODE:
            motor->SendStop(JAW_A1);
            have_send_movenow = false;
            break;
        case JAWA_2_MODE:
            motor->SendStop(JAW_A2);
            have_send_movenow = false;
            break;
        case SYN_JAWB_MODE:
            motor->SendStop(JAW_B1);
            motor->SendStop(JAW_B2);
            have_send_movenow = false;
            break;
        case JAWB_1_MODE:
            motor->SendStop(JAW_B1);
            have_send_movenow = false;
            break;
        case JAWB_2_MODE:
            motor->SendStop(JAW_B2);
            have_send_movenow = false;
            break;
        default:
            break;
        }

    }

    if(key_release == 0x09)
    {
        //qDebug()<<"CCW释放后在释放两边按键SendStop";
        motor->SendStop(GANTRY);
        have_send_movenow = false;
        ClearGantryFlags();
    }


    return true;

}

bool HandCon::RevMotorSpeedRequest(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    motor_speed_request = (MOTOR_REQUEST_SPEED_T)revdata.data[2];
    have_send_movenow = false;      //11-26 需从新发一次MoveNow

    return true;
}

bool HandCon::RevTablePositionRequest(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    table_position_request = revdata.data[2];
    return true;
}

bool HandCon::RevOpenJaw(TransRecvT &revdata)
{
    UINT32 target_pos;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    jaws_mode = OPEN_JAWS;

    //11-26
    if(!have_send_movenow)
    {
        switch(gan_coli_jaw_mode)
        {
        case SYN_JAWA_MODE:
            SlaveBase::CheckValue(20.0f, JAWY1_CK, target_pos);
            motor->SendMoveNow(JAW_A1, target_pos, motor_speed_request);
            hand_jawy1_control = true;
            SlaveBase::CheckValue(20.0f, JAWY2_CK, target_pos);
            motor->SendMoveNow(JAW_A2, target_pos, motor_speed_request);
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWA_START;
            have_send_movenow = true;
            break;
        case SYN_JAWB_MODE:
            SlaveBase::CheckValue(20.0f, JAWX1_CK, target_pos);
            motor->SendMoveNow(JAW_B2, target_pos, motor_speed_request);
            hand_jawx1_control = true;
            SlaveBase::CheckValue(20.0f, JAWX2_CK, target_pos);
            motor->SendMoveNow(JAW_B1, target_pos, motor_speed_request);
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWB_START;
            have_send_movenow = true;
            break;
        case JAWA_1_MODE:
            SlaveBase::CheckValue(20.0f, JAWY1_CK, target_pos);
            motor->SendMoveNow(JAW_A1, target_pos, motor_speed_request);
            hand_jawy1_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWA_1_START;
            have_send_movenow = true;
            break;
        case JAWA_2_MODE:
            SlaveBase::CheckValue(20.0f, JAWY2_CK, target_pos);
            motor->SendMoveNow(JAW_A2, target_pos, motor_speed_request);
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWA_2_START;
            have_send_movenow = true;
            break;
        case JAWB_1_MODE:
            SlaveBase::CheckValue(20.0f, JAWX2_CK, target_pos);
            motor->SendMoveNow(JAW_B1, target_pos, motor_speed_request);
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWB_1_START;
            have_send_movenow = true;
            break;
        case JAWB_2_MODE:
            SlaveBase::CheckValue(20.0f, JAWX1_CK, target_pos);
            motor->SendMoveNow(JAW_B2, target_pos, motor_speed_request);
            hand_jawx1_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWB_2_START;
            have_send_movenow = true;
            break;
        case COLLIMATOR_MODE:       //CW
            SlaveBase::CheckValue(95, COLLI_CK, target_pos);
            motor->SendMoveNow(COLLIMATOR, target_pos, motor_speed_request);
            hand_colli_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = COLLIMATOR_START;
            have_send_movenow = true;
            break;
        default:
            break;
        }
    }
    return true;
}

bool HandCon::RevCloseJaw(TransRecvT &revdata)
{
    UINT32 target_pos;
    float tmp_jaw;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    jaws_mode = CLOSE_JAWS;

    //11-26
    if(!have_send_movenow)
    {
        switch(gan_coli_jaw_mode)
        {
        case SYN_JAWA_MODE:
            SlaveBase::CheckValue(0, JAWY1_CK, target_pos);
            motor->SendMoveNow(JAW_A1, target_pos, motor_speed_request);
            hand_jawy1_control = true;
            SlaveBase::CheckValue(0, JAWY2_CK, target_pos);
            motor->SendMoveNow(JAW_A2, target_pos, motor_speed_request);
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWA_START;
            have_send_movenow = true;
            break;
        case SYN_JAWB_MODE:
            SlaveBase::CheckValue(0, JAWX1_CK, target_pos);
            motor->SendMoveNow(JAW_B2, target_pos, motor_speed_request);
            hand_jawx1_control = true;
            SlaveBase::CheckValue(0, JAWX2_CK, target_pos);
            motor->SendMoveNow(JAW_B1, target_pos, motor_speed_request);
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWB_START;
            have_send_movenow = true;
            break;
        case JAWA_1_MODE:
            tmp_jaw = ConvertMemberToFloat(motor->position[JAW_A2], JAWY2_CK);//Y2
            tmp_jaw = qMax(-10.0f,-tmp_jaw);
            SlaveBase::CheckValue(tmp_jaw, JAWY1_CK, target_pos);   //Y1 min
            motor->SendMoveNow(JAW_A1, target_pos, motor_speed_request);
            hand_jawy1_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWA_1_START;
            have_send_movenow = true;
            break;
        case JAWA_2_MODE:
            tmp_jaw = ConvertMemberToFloat(motor->position[JAW_A1], JAWY1_CK);//Y1
            tmp_jaw = qMax(-10.0f,-tmp_jaw);
            SlaveBase::CheckValue(tmp_jaw, JAWY2_CK, target_pos);   //Y2 min
            motor->SendMoveNow(JAW_A2, target_pos, motor_speed_request);
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWA_2_START;
            have_send_movenow = true;
            break;
        case JAWB_1_MODE:
            tmp_jaw = ConvertMemberToFloat(motor->position[JAW_B2], JAWX1_CK);//X1
            tmp_jaw = qMax(-2.0f,-tmp_jaw);
            SlaveBase::CheckValue(tmp_jaw, JAWX2_CK, target_pos);   //x2 min
            motor->SendMoveNow(JAW_B1, target_pos, motor_speed_request);
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWB_1_START;
            have_send_movenow = true;
            break;
        case JAWB_2_MODE:
            tmp_jaw = ConvertMemberToFloat(motor->position[JAW_B1], JAWX2_CK);//X2
            tmp_jaw = qMax(-2.0f,-tmp_jaw);
            SlaveBase::CheckValue(tmp_jaw, JAWX1_CK, target_pos);   //x1 min
            motor->SendMoveNow(JAW_B2, target_pos, motor_speed_request);
            hand_jawx1_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = JAWB_2_START;
            have_send_movenow = true;
            break;
        case COLLIMATOR_MODE:       //CCW
            SlaveBase::CheckValue(-95, COLLI_CK, target_pos);
            motor->SendMoveNow(COLLIMATOR, target_pos, motor_speed_request);
            hand_colli_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = COLLIMATOR_START;
            have_send_movenow = true;
            break;
        default:
            break;
        }
    }
    return true;
}

bool HandCon::RevCW(TransRecvT &revdata)
{
    UINT32 gpos_cw_max;
    float tmp;
    bool suc;

    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }

    gantry_mode = CW_GANTRY;

    if(gantry_rotat_mode)
    {
        if(motor_speed_request!=NO_SPEED)     //Fixme 12-02
        {
            suc = CheckValue(180+GANTRY_LIMIT, GANTRY_CK, gpos_cw_max);
            if(!suc)
            {
                return false;
            }
            motor->SendMoveNow(GANTRY,gpos_cw_max,motor_speed_request);
            //motor->SendMoveNow(GANTRY,0x0001,motor_speed_request);
            have_send_movenow = true;
            motor_speed_request = NO_SPEED;//只在hand返回53,motor request speed后才调用movenow
            hand_gantry_control = true;
            accui->UpdateFuncKey();
            //           qDebug()<<"Recv CCW Key,SendMoveNow:"<<maxpos;
        }
        else
        {
            tmp = ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);
            if (tmp < 0.0)
                tmp += 360.0;
            SendPosition(tmp,UPDATE_POSITION,GANTRY_MODE);//主机发送20h到手柄带有update position data
        }
    }
    return true;
}

bool HandCon::RevCCW(TransRecvT &revdata)
{
    UINT32 gpos_ccw_max;
    float tmp;
    bool suc;

    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }

    gantry_mode = CCW_GANTRY;

    if(gantry_rotat_mode)
    {
        if(motor_speed_request!=NO_SPEED)     //Fixme 12-02
        {
            suc = CheckValue(-(180+GANTRY_LIMIT), GANTRY_CK, gpos_ccw_max);
            if(!suc)
            {
                return false;
            }
            motor->SendMoveNow(GANTRY,gpos_ccw_max,motor_speed_request);
            //motor->SendMoveNow(GANTRY,0x1fff,motor_speed_request);
            have_send_movenow = true;
            motor_speed_request = NO_SPEED;//只在hand返回53,motor request speed后才调用movenow
            hand_gantry_control = true;
            accui->UpdateFuncKey();
            //           qDebug()<<"Recv CCW Key,SendMoveNow:"<<maxpos;
        }
        else
        {
            tmp = ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);
            if (tmp < 0.0)
                tmp += 360.0;
            SendPosition(tmp,UPDATE_POSITION,GANTRY_MODE);//主机发送20h到手柄带有update position data
        }
    }

    return true;
}

bool HandCon::RevPrePositData(TransRecvT &revdata)
{
    UINT32 collimator_pos;
    UINT32 jaw1_pos,jaw2_pos,jaw_ret;
    float jaw;
    float temp;
    bool ret;
    if (revdata.len != 11)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    //no-ascii
    if(!collim_rotat_mode&&!gantry_rotat_mode&&revdata.data[2]==0x15)
    {
        //钨门，负值
        preset_position = revdata.data[3] * 10 + revdata.data[4] +revdata.data[6] * 0.1;
        preset_position = 0-preset_position;

    }
    else
    {
        preset_position = revdata.data[2] * 100 +
                revdata.data[3] * 10 + revdata.data[4] +
                revdata.data[6] * 0.1;
    }


    //向Collimator/JawA/JawB发送p(50)move now
    if(collim_rotat_mode)
    {
        if((preset_position >= 0 && preset_position <= 95) || (preset_position >= 265 && preset_position < 360))
        {
            if(preset_position>180.0)
                preset_position=preset_position-360.0;
            SlaveBase::CheckValue(preset_position,COLLI_CK,collimator_pos);
            motor->SendMoveNow(COLLIMATOR,(UINT16)collimator_pos);
            hand_colli_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = COLLIMATOR_START;

        }
        else
        {
            SendInvalidPosition();
            return false;
        }
    }
    else if(gantry_rotat_mode)
    {
        if(preset_position<0 || preset_position >=360)
        {
            SendInvalidPosition();
            return false;
        }
    }
    else if(jawa_mode||jawb_mode)
    {
        jaw = preset_position;
        switch(gan_coli_jaw_mode){
        case SYN_JAWA_MODE:
            ret = SlaveBase::CheckValue(jaw, JAWY_CK, jaw_ret);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            SlaveBase::CheckValue(jaw/2.0,JAWY1_CK,jaw1_pos);
            SlaveBase::CheckValue(jaw/2.0,JAWY2_CK,jaw2_pos);
            qDebug()<<"JAWY target pos ="<<jaw1_pos<<", "<<jaw2_pos;
            motor->SendMoveNow(JAW_A1,(UINT16)jaw1_pos);
            motor->SendMoveNow(JAW_A2,(UINT16)jaw2_pos);
            hand_jawy1_control = true;
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWA_START;
            break;
        case JAWA_1_MODE:       //Y1
            ret = SlaveBase::CheckValue(-jaw, JAWY1_CK, jaw1_pos);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            temp = ConvertMemberToFloat(motor->position[JAW_A2], JAWY2_CK);//从motor中获取act position
            if(temp-jaw<0)          //Y2-Y1
            {
                SendInvalidPosition();
                return false;
            }
            ret = motor->SendMoveNow(JAW_A1, jaw1_pos);
            hand_jawy1_control = true;
            accui->UpdateFuncKey();
            if(!ret)
                return false;
            collimatorOrjaw_start = JAWA_1_START;
            break;
        case JAWA_2_MODE:
            ret = SlaveBase::CheckValue(jaw, JAWY2_CK, jaw2_pos);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            temp = ConvertMemberToFloat(motor->position[JAW_A1], JAWY1_CK);//从motor中获取act position
            if(jaw-(-temp)<0)          //Y2-Y1  Y1=-temp
            {
                SendInvalidPosition();
                return false;
            }
            ret = motor->SendMoveNow(JAW_A2, jaw2_pos);
            hand_jawy2_control = true;
            accui->UpdateFuncKey();
            if(!ret)
                return false;
            collimatorOrjaw_start = JAWA_2_START;
            break;
        case SYN_JAWB_MODE:
            ret = SlaveBase::CheckValue(jaw, JAWX_CK, jaw_ret);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            SlaveBase::CheckValue(jaw/2.0,JAWX1_CK,jaw1_pos);
            SlaveBase::CheckValue(jaw/2.0,JAWX2_CK,jaw2_pos);
            motor->SendMoveNow(JAW_B2,(UINT16)jaw1_pos);
            motor->SendMoveNow(JAW_B1,(UINT16)jaw2_pos);
            hand_jawx1_control = true;
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            collimatorOrjaw_start = SYN_JAWB_START;
            break;
        case JAWB_1_MODE:
            ret = SlaveBase::CheckValue(jaw, JAWX2_CK, jaw1_pos);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            temp = ConvertMemberToFloat(motor->position[JAW_B2], JAWX1_CK);//从motor中获取act position
            qDebug()<<"X2-X1 ="<<jaw<<temp;
            if(jaw-(-temp)<0)          //X2-X1   X1=-temp
            {
                SendInvalidPosition();
                return false;
            }
            ret = motor->SendMoveNow(JAW_B1, jaw1_pos);
            hand_jawx2_control = true;
            accui->UpdateFuncKey();
            if(!ret)
                return false;
            collimatorOrjaw_start = JAWB_1_START;

            break;
        case JAWB_2_MODE:       //X1
            ret = SlaveBase::CheckValue(-jaw, JAWX1_CK, jaw2_pos);
            if(!ret)
            {
                SendInvalidPosition();
                return false;
            }
            temp = ConvertMemberToFloat(motor->position[JAW_B1], JAWX2_CK);//从motor中获取act position
            qDebug()<<"X2-X1 ="<<temp<<jaw;
            if(temp-jaw<0)          //X2-X1   jaw is from hand setting
            {
                SendInvalidPosition();
                return false;
            }
            ret = motor->SendMoveNow(JAW_B2, jaw2_pos);
            hand_jawx1_control = true;
            accui->UpdateFuncKey();
            if(!ret)
                return false;
            collimatorOrjaw_start = JAWB_2_START;

            break;
        default:
            break;
        }
    }

    return true;
}

bool HandCon::RevErrorReport(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    error_report = revdata.data[2];
    ProcessErrorIL();
    return true;
}

bool HandCon::RevResetEcho(TransRecvT &revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(4, true);
    g_c0_echo_bits_lock.unlock();
    return true;

}

bool HandCon::RevDeSelctCurHandCtr(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 964, ERROR_IL);   //Invalid length for command type from controller 4.
        return false;
    }
    deselect = true;
    return true;
}


