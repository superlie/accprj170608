#include "dosei.h"
#include "ui/accui.h"

extern AccUi *accui;
extern bool g_compensate;
extern float g_treat_time;

DoseOne::DoseOne()
{
    if (!ReadOwnFile())
    {
        ;//interlock
    }

    work_mode = RESET_MODE;
    machine_type = NON_PRIMART;
    check_request = CHECK1;
    ready_status = NOTREADY;
    jaw_select = JAW_Y1;
    preset_treat_time = 0;
    preset_dose1 = 0;

    temperature = 0;
    pressure = 0;
    float_temp = 0.0;
    float_press = 0.0;
    dose1_act_data = 0;                //dose 1 accumulated dose rate
    dose1_aver_rate = 0;               //dose 1 average dose rate
    treatment_time = 0;
    currx = 0;     //Dynamic jaw posision for virtual wedge treatment
    //Gantry position for rotation treatment
    backup_dose_data = 0;
    backup_conversion_factor = 0;
    backup_time = 0;     //可能可以与treatment_time 合在一起
    backup_gantry_position = 0;      //可能可以与cross_plane_steering 合在一起
    check_report = 0;
    prom_version = 0; //版本号等信息
    dose1_rate_offset = 0;  //Transmitted once after READY state transition
    error_now.xx  = 0;
    error_now.yy  = 0;
    segment_0 = 0;    //Segment 0  read back
    segment_1 = 0;
    segment_3 = 0;
    MUGAP_time = 0;  //MUGAP treatment time
    virtual_wedge_jaw_position = 0;
    gantry_position_counts = 0;

    send_frame_num = 0;
    rev_frame_num = 0;

    mode_now = RESET_MODE;

    last_pc_radon_time = 0;
    dose_aver_rate_count = 0;

    for (int i=0;i<27;i++)
        item_has[i]=false;
}

DoseOne::~DoseOne()
{
    ;
}

//读取自己模块的文件
bool DoseOne::ReadOwnFile()
{
    QFile file(".\\AccConfig\\Dosei.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))  //以只读方式打开
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
                if (reader.name() == "Dosei")   //判断根节点是否是自己模块
                {
                    ReadOwnElement();   //读取子元素
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

//读取Dosei文件的参数
bool DoseOne::ReadOwnElement()
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
                if (reader.name() == "dose1_by_pass_mask")
                    dose1_by_pass_mask = reader.readElementText().toInt();
                else if (reader.name() == "preset_dose_per_degree")
                    rot_dose_per_degree = reader.readElementText().toInt();
                else if (reader.name() == "gantry_speed")
                    gantry_speed = reader.readElementText().toInt();
                else if (reader.name() == "compensate_gain")
                    compensate_gain = reader.readElementText().toInt();
                else if (reader.name() == "jaw_x1_max_speed")
                    max_position_speed[3] = reader.readElementText().toInt();
                else if (reader.name() == "jaw_x2_max_speed")
                    max_position_speed[2] = reader.readElementText().toInt();
                else if (reader.name() == "jaw_y1_max_speed")
                    max_position_speed[0] = reader.readElementText().toInt();
                else if (reader.name() == "jaw_y2_max_speed")
                    max_position_speed[1] = reader.readElementText().toInt();
                else if (reader.name() == "collimator_type")
                    max_position_speed[4] = reader.readElementText().toInt();
                else if (reader.name() == "ref_temp")
                    ref_temp = reader.readElementText().toFloat();
                else if (reader.name() == "ref_press")
                    ref_press = reader.readElementText().toFloat();
                else if (reader.name() == "compen_factor")
                    compen_factor = reader.readElementText().toFloat();
                reader.readNext();
            }
        }
    }
    return true;
}

bool DoseOne::WriteOwnFile()
{
    QFile file(".\\AccConfig\\dosei.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
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
            if (root.tagName() == "Dosei")
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

bool DoseOne::ReadElement()
{
    //qDebug()<<"ReadElement";
    for (int i=0; i<27; i++)
        item_has[i] = false;

    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isEndElement())
            break;

        if (reader.isStartElement())
        {
            if (reader.name() == "research")
            {
                while ( ! (reader.isEndElement() && reader.name() == "research") )
                {
                    //qDebug()<<"Dose1: research reader name ="<<reader.name();
                    if (reader.name() == "doseigain")
                    {
                        item_has[0] = true;
                        dose1_gain[RESEARCH].dose1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment1_gain")
                    {
                        item_has[1] = true;
                        dose1_gain[RESEARCH].segment1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment3_gain")
                    {
                        item_has[2] = true;
                        dose1_gain[RESEARCH].segment3_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[3] = true;
                        dose1_gain[RESEARCH].drcv =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose1_gain[RESEARCH].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "ds_wind")
                    {
                        item_has[4] = true;
                        dyn_steer[RESEARCH].ds_wind =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ds_gainx")
                    {
                        item_has[5] = true;
                        dyn_steer[RESEARCH].ds_gainx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "balx")
                    {
                        item_has[6] = true;
                        dyn_steer[RESEARCH].balx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "swi")
                    {
                        item_has[7] = true;
                        swi[RESEARCH] = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "fwi")
                    {
                        item_has[8] = true;
                        fwi[RESEARCH] = reader.readElementText().toFloat();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "default")
            {
                while ( ! (reader.isEndElement() && reader.name() == "default") )
                {
                    //qDebug()<<"Dose1: Default reader name ="<<reader.name();
                    if (reader.name() == "doseigain")
                    {
                        item_has[9] = true;
                        dose1_gain[DEFAULT].dose1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment1_gain")
                    {
                        item_has[10] = true;
                        dose1_gain[DEFAULT].segment1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment3_gain")
                    {
                        item_has[11] = true;
                        dose1_gain[DEFAULT].segment3_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[12] = true;
                        dose1_gain[DEFAULT].drcv =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose1_gain[DEFAULT].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "ds_wind")
                    {
                        item_has[13] = true;
                        dyn_steer[DEFAULT].ds_wind =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ds_gainx")
                    {
                        item_has[14] = true;
                        dyn_steer[DEFAULT].ds_gainx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "balx")
                    {
                        item_has[15] = true;
                        dyn_steer[DEFAULT].balx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "swi")
                    {
                        item_has[16] = true;
                        swi[DEFAULT] = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "fwi")
                    {
                        item_has[17] = true;
                        fwi[DEFAULT] = reader.readElementText().toFloat();
                    }
                    reader.readNext();
                }

            }
            else if (reader.name() == "treatment")
            {
                while ( ! (reader.isEndElement() && reader.name() =="treatment") )
                {
                    //qDebug()<<"Dose1: Treatment: reader name ="<<reader.name();
                    if (reader.name() == "doseigain")
                    {
                        item_has[18] = true;
                        dose1_gain[TREATMENT].dose1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment1_gain")
                    {
                        item_has[19] = true;
                        dose1_gain[TREATMENT].segment1_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment3_gain")
                    {
                        item_has[20] = true;
                        dose1_gain[TREATMENT].segment3_gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[21] = true;
                        dose1_gain[TREATMENT].drcv =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose1_gain[TREATMENT].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "ds_wind")
                    {
                        item_has[22] = true;
                        dyn_steer[TREATMENT].ds_wind =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "ds_gainx")
                    {
                        item_has[23] = true;
                        dyn_steer[TREATMENT].ds_gainx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "balx")
                    {
                        item_has[24] = true;
                        dyn_steer[TREATMENT].balx =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "swi")
                    {
                        item_has[25] = true;
                        swi[TREATMENT] = reader.readElementText().toFloat();
                    }
                    else if (reader.name() == "fwi")
                    {
                        item_has[26] = true;
                        fwi[TREATMENT] = reader.readElementText().toFloat();
                    }
                    reader.readNext();
                }
            }
        }
    }
    return true;
}

void DoseOne::WriteElement()
{
    QDomNodeList list_l1, list_l2;
    QDomNode node_l1, node_l2;

    list_l1 = node.childNodes();
    for (int i = 0; i < list_l1.count(); i++)
    {
        node_l1 = list_l1.at(i);
        if (node_l1.nodeName() == "research")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "doseigain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[RESEARCH].dose1_gain));
                else if (node_l2.nodeName() == "segment1_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[RESEARCH].segment1_gain));
                else if (node_l2.nodeName() == "segment3_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[RESEARCH].segment3_gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[RESEARCH].drcv));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[RESEARCH].bitArray));
                else if (node_l2.nodeName() == "ds_wind")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[RESEARCH].ds_wind));
                else if (node_l2.nodeName() == "ds_gainx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[RESEARCH].ds_gainx));
                else if (node_l2.nodeName() == "balx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[RESEARCH].balx));
                else if (node_l2.nodeName() == "swi")
                    node_l2.firstChild().setNodeValue(QString::number(swi[RESEARCH]));
                else if (node_l2.nodeName() == "fwi")
                    node_l2.firstChild().setNodeValue(QString::number(fwi[RESEARCH]));
            }
        }
        else if (node_l1.nodeName() == "default")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "doseigain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[DEFAULT].dose1_gain));
                else if (node_l2.nodeName() == "segment1_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[DEFAULT].segment1_gain));
                else if (node_l2.nodeName() == "segment3_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[DEFAULT].segment3_gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[DEFAULT].drcv));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[DEFAULT].bitArray));
                else if (node_l2.nodeName() == "ds_wind")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[DEFAULT].ds_wind));
                else if (node_l2.nodeName() == "ds_gainx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[DEFAULT].ds_gainx));
                else if (node_l2.nodeName() == "balx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[DEFAULT].balx  ));
                else if (node_l2.nodeName() == "swi")
                    node_l2.firstChild().setNodeValue(QString::number(swi[DEFAULT]));
                else if (node_l2.nodeName() == "fwi")
                    node_l2.firstChild().setNodeValue(QString::number(fwi[DEFAULT]));
            }
        }
        else if (node_l1.nodeName() == "treatment")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "doseigain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[TREATMENT].dose1_gain));
                else if (node_l2.nodeName() == "segment1_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[TREATMENT].segment1_gain));
                else if (node_l2.nodeName() == "segment3_gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[TREATMENT].segment3_gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[TREATMENT].drcv));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose1_gain[TREATMENT].bitArray));
                else if (node_l2.nodeName() == "ds_wind")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[TREATMENT].ds_wind));
                else if (node_l2.nodeName() == "ds_gainx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[TREATMENT].ds_gainx));
                else if (node_l2.nodeName() == "balx")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer[TREATMENT].balx  ));
                else if (node_l2.nodeName() == "swi")
                    node_l2.firstChild().setNodeValue(QString::number(swi[TREATMENT]));
                else if (node_l2.nodeName() == "fwi")
                    node_l2.firstChild().setNodeValue(QString::number(fwi[TREATMENT]));
            }
        }
    }
}

void DoseOne::WriteOwnElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "dose1_by_pass_mask")
            node.firstChild().setNodeValue(QString::number(dose1_by_pass_mask));
        else if (node.nodeName() == "preset_dose_per_degree")
            node.firstChild().setNodeValue(QString::number(rot_dose_per_degree));
//        else if (node.nodeName() == "gantry_speed")
//            node.firstChild().setNodeValue(QString::number(gantry_speed));
        else if (node.nodeName() == "compensate_gain")
            node.firstChild().setNodeValue(QString::number(compensate_gain));
        else if (node.nodeName() == "jaw_x1_max_speed")
            node.firstChild().setNodeValue(QString::number( max_position_speed[3]));
        else if (node.nodeName() == "jaw_x2_max_speed")
            node.firstChild().setNodeValue(QString::number( max_position_speed[2]));
        else if (node.nodeName() == "jaw_y1_max_speed")
            node.firstChild().setNodeValue(QString::number( max_position_speed[0]));
        else if (node.nodeName() == "jaw_y2_max_speed")
            node.firstChild().setNodeValue(QString::number( max_position_speed[1]));
        else if (node.nodeName() == "collimator_type")
            node.firstChild().setNodeValue(QString::number( max_position_speed[4]));
        else if (node.nodeName() == "ref_temp")
            node.firstChild().setNodeValue(QString::number(ref_temp));
        else if (node.nodeName() == "ref_press")
            node.firstChild().setNodeValue(QString::number(ref_press));
        else if (node.nodeName() == "compen_factor")
            node.firstChild().setNodeValue(QString::number(compen_factor));
        node = node.nextSibling();
    }
}

bool DoseOne::CheckSoftpotDataLimit()
{
    bool suc = true;
    int i;
    for(i=0;i<3;i++)        //RESEARCH=0; DEFAULT=1; TREATMENT=2
    {
        suc &= CheckXMLData(dose1_gain[i].dose1_gain, DOSE1_GAIN_CK);
        suc &= CheckXMLData(dose1_gain[i].segment1_gain, S1_GAIN_CK);
        suc &= CheckXMLData(dose1_gain[i].segment3_gain, S3_GAIN_CK);
        suc &= CheckXMLData(dose1_gain[i].drcv, DRCV_CK);
        suc &= CheckXMLData(dyn_steer[i].ds_wind, DSWX_RSCH_CK);
        suc &= CheckXMLData(dyn_steer[i].ds_gainx, DSGX_RSCH_CK);
        suc &= CheckXMLData(dyn_steer[i].balx, BALX_RSCH_CK);
        suc &= CheckXMLData(swi[i], SWI_CK);
        suc &= CheckXMLData(fwi[i], FWI_CK);
    }
    suc &= CheckReschTreatToDefault(TREATMENT);
    return suc;
}

bool DoseOne::CheckReschTreatToDefault(RE_TE_MODE mode)
{
    if( ( dose1_gain[mode].dose1_gain<(dose1_gain[DEFAULT].dose1_gain*0.9) )
            || ( dose1_gain[mode].dose1_gain>(dose1_gain[DEFAULT].dose1_gain*1.1) ) )
        return false;
    if( ( dose1_gain[mode].segment1_gain<(dose1_gain[DEFAULT].segment1_gain*0.9) )
            || ( dose1_gain[mode].segment1_gain>(dose1_gain[DEFAULT].segment1_gain*1.1) ) )
        return false;
    if( ( dose1_gain[mode].segment3_gain<(dose1_gain[DEFAULT].segment3_gain*0.9) )
            || ( dose1_gain[mode].segment3_gain>(dose1_gain[DEFAULT].segment3_gain*1.1) ) )
        return false;
    if( ( dose1_gain[mode].drcv<(dose1_gain[DEFAULT].drcv*0.9) )
            || ( dose1_gain[mode].drcv>(dose1_gain[DEFAULT].drcv*1.1) ) )
        return false;
    if( ( dyn_steer[mode].ds_wind<(dyn_steer[DEFAULT].ds_wind*0.9) )
            || ( dyn_steer[mode].ds_wind>(dyn_steer[DEFAULT].ds_wind*1.1) ) )
        return false;
    if( ( dyn_steer[mode].ds_gainx<(dyn_steer[DEFAULT].ds_gainx*0.9) )
            || ( dyn_steer[mode].ds_gainx>(dyn_steer[DEFAULT].ds_gainx*1.1) ) )
        return false;
    if( ( dyn_steer[mode].balx<(dyn_steer[DEFAULT].balx*0.9) )
            || ( dyn_steer[mode].balx>(dyn_steer[DEFAULT].balx*1.1) ) )
        return false;
    if( ( swi[mode]<(swi[DEFAULT]*0.9) ) || ( swi[mode]>(swi[DEFAULT]*1.1) ) )
        return false;
    if( ( fwi[mode]<(fwi[DEFAULT]*0.9) ) || ( fwi[mode]>(fwi[DEFAULT]*1.1) ) )
        return false;
    return true;
}

bool DoseOne::CheckSoftpotDataIntegrity()
{
    for(int i=0; i<27; i++)
    {
        if(!item_has[i])
        {
            qDebug()<<"dose1 softpot integrity error:  i ="<<i;
            return false;
        }
    }
    return true;
}

//bool DoseOne::CheckCompenFactorIntegrity()
//{

//}

//bool DoseOne::CheckPressTempIntegrity()
//{

//}

bool DoseOne::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
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

UINT8 DoseOne::ComputeRevCHK(UINT8* data, UINT8 len)
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

UINT32 DoseOne::ComputePresetDose(float rads)
{
    float f_drcv = ConvertMemberToFloat(dose1_gain[g_mode].drcv, DRCV_CK);
    return (UINT32)(f_drcv * 409.5 * rads / dose_rate);
}

UINT16 DoseOne::ComputeConvertFactor()
{
     f_drcv = ConvertMemberToFloat(dose1_gain[g_mode].drcv, DRCV_CK);
     return (UINT16)(1250.3 * dose_rate / f_drcv);
}

float DoseOne::ConvertDose(UINT32 mcounts)
{
    return (float)((float)mcounts * (10.0 / 4095.0) * (dose_rate / f_drcv));
}

float DoseOne::ConvertDoserate(UINT32 mcounts)
{
    return (float)(10000 * dose_rate * mcounts / (4095 * f_drcv));
}

void DoseOne::ClearPcRadonTime()
{
    dose_aver_rate_count = 0;
     last_treat_time = 0;
     last_pc_radon_time = pc_radon_time = 0;
}

void DoseOne::SyncPcRadonTime()
{
     dose_aver_rate_count = 0;
     last_treat_time = treatment_time;
     last_pc_radon_time = pc_radon_time-2;
     qDebug()<<"Sync pc rad on time";
}

bool DoseOne::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x21;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe1;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendInformation()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    preset_dose1 = ComputePresetDose(f_preset_dose1);
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x49;     //I
    tmpSend.data[2] = preset_treat_time & 0x00FF;
    tmpSend.data[3] = (preset_treat_time & 0xFF00) >> 8;
    tmpSend.data[4] = dose1_by_pass_mask & 0x00FF;
    tmpSend.data[5] = (dose1_by_pass_mask & 0xFF00) >> 8;
    tmpSend.data[6] = (preset_dose1 & 0x000000FF);
    tmpSend.data[7] = (preset_dose1 & 0x0000FF00) >> 8;
    tmpSend.data[8] = (preset_dose1 & 0x00FF0000) >> 16;
    tmpSend.data[9] = (preset_dose1 & 0xFF000000) >> 24;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11] = ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;       //fixme

    return InsertOutQueue(tmpSend);
}

//LJY-9-23
bool DoseOne::SendRotData()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    preset_dose1 = ComputePresetDose(f_preset_dose1);
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x4a;     //I
    tmpSend.data[2] = rot_dose_per_degree & 0x00FF;
    tmpSend.data[3] = (rot_dose_per_degree & 0xFF00) >> 8;
    tmpSend.data[4] = rot_speed_per_sec;
    tmpSend.data[5] = 0x2b;     //CW不变

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;
    tmpSend.data[7] = ComputeLstCHK(&tmpSend.data[0], 7);
    tmpSend.len = 8;       //fixme

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendDynamicSteer()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x44;     //D
    tmpSend.data[2] = dyn_steer[g_mode].ds_wind & 0x00FF;
    tmpSend.data[3] = (dyn_steer[g_mode].ds_wind & 0xFF00) >> 8;
    tmpSend.data[4] = dyn_steer[g_mode].ds_gainx & 0x00FF;
    tmpSend.data[5] = (dyn_steer[g_mode].ds_gainx & 0xFF00) >> 8;
    tmpSend.data[6] = dyn_steer[g_mode].balx & 0x00FF;
    tmpSend.data[7] = (dyn_steer[g_mode].balx & 0xFF00) >> 8;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 8, tmpSend.data[8]))
        return false;
    tmpSend.data[9] = ComputeLstCHK(&tmpSend.data[0], 9);
    tmpSend.len = 10;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendParamBackupconverFact()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    param_bkup_conver_factor = ComputeConvertFactor();
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x4D;     //M
    tmpSend.data[2] = param_bkup_conver_factor & 0x00FF;
    tmpSend.data[3] = (param_bkup_conver_factor & 0xFF00) >> 8;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;
    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendUserMode(const  USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&dosei_mutex);
    if (mode_now == RESET_MODE)    //fixme
    {
        send_frame_num = 0;         //发送reset命令，将序列号清零
        //rev_frame_num = 0;
    }

    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;       //fixme

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendGainAndWorkMode()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x47;     //G

    tmpSend.data[2] = dose1_gain[g_mode].dose1_gain & 0x00FF;
    tmpSend.data[3] = (dose1_gain[g_mode].dose1_gain & 0xFF00) >> 8;
    tmpSend.data[4] = dose1_gain[g_mode].segment1_gain & 0x00FF;
    tmpSend.data[5] = (dose1_gain[g_mode].segment1_gain & 0xFF00) >> 8;
    tmpSend.data[6] = dose1_gain[g_mode].segment3_gain & 0x00FF;
    tmpSend.data[7] = (dose1_gain[g_mode].segment3_gain & 0xFF00) >> 8;
    tmpSend.data[8] = dose1_gain[g_mode].drcv & 0x00FF;
    tmpSend.data[9] = (dose1_gain[g_mode].drcv & 0xFF00) >> 8;
    //tmpSend.data[10] = dose1_gain[g_mode].bitArray;

    switch(g_ray_type)
    {
    case E1:
    case E2:
    case E3:
    case E4:
    case E5:
    case E6:
        tmpSend.data[10] = 0x11;
        break;
    case X_LO:
        tmpSend.data[10] = 0x10;
        break;
    case X_HI:
        tmpSend.data[10] = 0x18;
        break;
    case ROT_X_LO:
        tmpSend.data[10] = 0x12;
        break;
    case ROT_X_HI:
        tmpSend.data[10] = 0x1A;
        break;
    default:
        tmpSend.data[10] = 0x10;
        break;
    }

    if (!ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]))
        return false;

    QMutexLocker locker(&dosei_mutex);
    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendMaxJawSpeedAndColType()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x56;     //V

    for (int i = 0; i < 4; i++)
    {
        tmpSend.data[i * 2 + 2] = (UINT8)(max_position_speed[i] & 0x00FF);
        tmpSend.data[i * 2 + 3] = (UINT8)((max_position_speed[i] & 0xFF00) >> 8);
    }

    tmpSend.data[10] = (UINT8)(max_position_speed[4] & 0x00FF);

    if (!ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]))
        return false;

    QMutexLocker locker(&dosei_mutex);
    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendReadyStatus(const READYSTATE_TYPE_T status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendClear()
{
        TransRecvT tmpSend;
        memset(&tmpSend, 0, sizeof(TransRecvT));
        tmpSend.data[0] = TODOSEI;
        tmpSend.data[1] = 0x4c;
        QMutexLocker locker(&dosei_mutex);
        if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
            return false;
        tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
        tmpSend.len = 4;
        return InsertOutQueue(tmpSend);
}

bool DoseOne::SendRadOn()
{
    TransRecvT tmpSend;

    //compensate_gain = dose1_gain[g_mode].dose1_gain;
    float dose1gain=(float)dose1_gain[g_mode].dose1_gain;

    compensate_gain=(int)(dose1gain*compen_factor);

    dose1_act_data = 0;     //LJY-12-22
//    qDebug()<<"dose1_gain="<<dose1_gain[g_mode].dose1_gain
//           <<", to float="<<dose1gain<<", compensate_gain="<<compensate_gain;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x52;     //R
    tmpSend.data[2] = compensate_gain & 0x00FF;
    tmpSend.data[3] = (compensate_gain & 0xFF00) >> 8;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendInterrupt()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x4F;

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::SendTerminateOrComplete()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TODOSEI;
    tmpSend.data[1] = 0x54;     //T

    QMutexLocker locker(&dosei_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool DoseOne::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C' && revdata.data[2]==0)
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case '1':
        return RevPressurenTemperature(revdata);
    case '2':
        return RevDose1Data(revdata);
    case 'K':
        return RevLastTransmissionForCompletion(revdata);
    case '3':
        return RevParameterBackupData(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'C':
        return RevResetEcho(revdata);
    case 'N':
        return RevDose1Offset(revdata);
    case 'E':
        return RevErrReport(revdata);
    case 'P':
        return RevPeaks(revdata);
    case '5':
        return RevMUGAPTreatmentTime(revdata);
    case '4':
        return RevVirtualWedgeJawPosition(revdata);
    case 'B':
        return RevLastTransmissionForTermination(revdata);
    case 'H':
        return RevGantryPosition(revdata);
    default:   //Invalid command type received from controller 1
        InsertInterlock(79, 951, ERROR_IL);
    }
    return false;
}

bool DoseOne::RevPressurenTemperature(TransRecvT &revdata)
{
    if (revdata.len != 8)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    temperature = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    pressure = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);

    float_temp = ref_temp - 15.0 + (float)temperature * 0.00733;
    float_press = ref_press - 30 + (float)pressure * 0.0146;

//    accui->SetActTemp1(float_temp);
//    accui->SetActPress1(float_press);
    emit base->ShowActSvcValueSig(TEMP_PRESS1);

    ProcessCompensationFactorIL();

    if ((g_ray_type & (0xF)) < 7)
    {
        press_temp_new = true;
        ProcessPressurenTemperatureIL();
    }
    return true;
}

void DoseOne::ProcessCompensationFactorIL()
{
    float factor;
    //计算补偿因子
    //Compensation factor outside the interval,not latched

    if ((g_ray_type & (0xF)) < 7)
    {
        factor = ((273.16 + float_temp) / (273.16 + ref_temp)) * ( ref_press / float_press);
        // need add check compen or overide
        if(g_compensate)
        {
            compen_factor=factor;

        }
        else
        {
            compen_factor=1.0f;
        }
        emit base->ShowActSvcValueSig(CPS_FACTOR);  //11-24

        if (factor >= 1.14 || factor <= 0.86)
            InsertInterlockNoErr(6, SAFETY_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT,
                            NOT_LATCHED);
        else
            DeleteInterlock(6);
    }
    else
    {
        compen_factor=1.0;
    }
}


bool DoseOne::RevDose1Data(TransRecvT &revdata)
{
//    float tmp;
    UINT32 tmp_dose1;

    if (revdata.len != 14)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }


//    dose1_act_data =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
//            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);

    //LJY-12-21
    tmp_dose1 =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);
    if(dose1_act_data>tmp_dose1)
        InsertInterlock(62, 872, ERROR_IL, IL_ALL, LATCHED);
    dose1_act_data = tmp_dose1;

    dose1_aver_rate = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    treatment_time = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    currx = (revdata.data[10] & 0x00FF) |
            ((revdata.data[11] << 8) & 0xFF00);

    g_treat_time=dosei->ConvertMemberToFloat(dosei->treatment_time, TIMER_PRE_CK)*60;   //sec
//    qDebug()<<"treat_time_f="<<treat_time_f;

    emit base->ShowActSvcValueSig(DR_1);
    emit base->ShowActDoseTimeMUSig();

    if (g_status == RAD_ON)
    {
        pc_radon_time = g_radon_time.elapsed() / 100 + last_pc_radon_time;
        dose_rate_new = true;
        emit base->g_dosei_start_signal(0);
        //qDebug()<<"Treat time="<<dosei->treatment_time<<" PC Time="<<pc_radon_time;
        if (qAbs( (int)(dosei->treatment_time) - (int)pc_radon_time) > 120)
            InsertInterlock(79, 991, ERROR_IL, IL_RADON, LATCHED);
    }

    ProcessDoseRatesIL();

    return true;
}

bool DoseOne::RevLastTransmissionForCompletion(TransRecvT &revdata)     //K
{
    //float tmp;
    if (revdata.len != 14)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    dose1_act_data =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);

    dose1_aver_rate = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    treatment_time = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    currx = (revdata.data[10] & 0x00FF) |
            ((revdata.data[11] << 8) & 0xFF00);

//    tmp = ConvertDose(dose1_act_data);
//    accui->SetActDos1((UINT32)tmp);

//    tmp = ConvertDoserate(dose1_aver_rate);
//    accui->SetActDosRate((UINT32)tmp);

//    tmp = ConvertMemberToFloat(treatment_time, TIMER_PRE_CK);
//    accui->SetActDosTime(tmp);
    emit base->ShowActDoseTimeMUSig();

    g_dosei_timer->stop();

    if (g_status == RAD_ON)
        g_is_complete = true;

    return true;
}

bool DoseOne::RevParameterBackupData(TransRecvT &revdata)
{

    if (revdata.len != 13)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    backup_dose_data =  ((revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00) |
            ((revdata.data[4] << 16) & 0xFF0000)) & 0x00FFFFFF;

    backup_conversion_factor = (revdata.data[5] & 0x00FF) | ((revdata.data[6] << 8) & 0xFF00);
    backup_time = (revdata.data[7] & 0x00FF) | ((revdata.data[8] << 8) & 0xFF00);
    backup_gantry_position = (revdata.data[9] & 0x00FF) |
            ((revdata.data[10] << 8) & 0xFF00);

    return true;
}

bool DoseOne::RevCheckReport(TransRecvT &revdata)
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    check_report =  revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
//        accui->SetActChkDispReport1(check_report);
        emit base->ShowActSvcValueSig(CHK_1);
    }

    if (check_report == 0x0)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(1, true);
        g_check_bits_mutex.unlock();
    }

    else if (check_report == 0x14)
    {
        //interlock
        ;
    }
    else if (check_report == 0x40)
    {
        //interlock
        ;
    }
    else if (check_report == 0x54)
    {
        //interlock
        ;
    }

    return true;
}

bool DoseOne::RevResetEcho(TransRecvT &revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(1, true);
    g_c0_echo_bits_lock.unlock();
    return true;
}

bool DoseOne::RevDose1Offset(TransRecvT& revdata)
{
    //float tmp;
    if (revdata.len != 6)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    dose1_rate_offset = (revdata.data[2] & 0x00FF) |
            ((revdata.data[3] << 8) & 0xFF00);

//    tmp = ConvertMemberToFloat(dose1_rate_offset, DOSE_RATE1_OFF);
//    accui->SetActOff1((UINT32)tmp);
    emit base->ShowActSvcValueSig(OFF1);

    return true;
}

bool DoseOne::RevErrReport(TransRecvT &revdata)
{
    if (revdata.len == 5)
      error_now.xx = revdata.data[2];

    else if (revdata.len == 6)
    {
        error_now.xx = revdata.data[2];
        error_now.yy = revdata.data[3];
    }
    else
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    ProcessErrorIL();
    return true;
}

bool DoseOne::RevPeaks(TransRecvT &revdata)
{
//    float tmp;
    if (revdata.len != 10)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }
    segment_0 = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    segment_1 = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);
    segment_3 = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);

//    tmp = ConvertMemberToFloat(segment_0, S0_GAIN_CK);
//    accui->SetActSeg0((UINT32)tmp);

//    tmp = ConvertMemberToFloat(segment_1, S1_GAIN_CK);
//    accui->SetActSeg1((UINT32)tmp);

//    tmp = ConvertMemberToFloat(segment_3, S3_GAIN_CK);
//    accui->SetActSeg3((UINT32)tmp);
    emit base->ShowActSvcValueSig(SEG013);
    //DS_CX

    ProcessFlatnessIL();
    return true;
}

void DoseOne::ProcessFlatnessIL()
{
    float s0, s1, s3, max, min;
    s0 = ConvertMemberToFloat(segment_0, S0_GAIN_CK);
    s1 = ConvertMemberToFloat(segment_1, S1_GAIN_CK);
    s3 = ConvertMemberToFloat(segment_3, S3_GAIN_CK);
    max = qMax(s0, qMax(s1, s3));
    min = qMin(s0, qMin(s1, s3));
    flatness = (max - min) / (max + min) * 100.0;
    symmetry = (s1 - s3) / (s1 + s3) * 100.0;

    //SX_E, FX_E
//    accui->SetActSxe(symmetry);
//    accui->SetActFxe(flatness);
    emit base->ShowActSvcValueSig(SXE_FXE);

    //interlock#7:Inplane 超过FWI引起， 安全联锁
    if ((g_treat_time > 2.0f) && (flatness > fwi[g_mode]))
        InsertInterlockNoErr(7, SAFETY_IL, IL_RADON, LATCHED);
    //interlock#8:Inplane 超过SWI（？的2倍），安全联锁
    if ((g_treat_time > 2.0f) && (symmetry > swi[g_mode]))//*2)
        InsertInterlockNoErr(8, SAFETY_IL, IL_RADON, LATCHED);
}

bool DoseOne::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 931, ERROR_IL);       // Invalid frame number detected from controller 1 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 941, ERROR_IL);       //Invalid checksum detected from controller 1 安全联锁

        return false;
    }
    return true;
}

bool DoseOne::RevMUGAPTreatmentTime(TransRecvT &revdata)
{

    if (revdata.len != 6)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    MUGAP_time = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    return true;
}

bool DoseOne::RevVirtualWedgeJawPosition(TransRecvT &revdata)
{
    if (revdata.len != 6)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }

    virtual_wedge_jaw_position = (revdata.data[2] & 0x00FF) &
            ((revdata.data[3] << 8) & 0xFF00);
    return true;
}

bool DoseOne::RevLastTransmissionForTermination(TransRecvT &revdata)    //B
{
    //float tmp;
    if (revdata.len != 14)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }


    dose1_act_data =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);

    dose1_aver_rate = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    treatment_time = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    currx = (revdata.data[10] & 0x00FF) |
            ((revdata.data[11] << 8) & 0xFF00);

    g_is_terminate = true;

//    treat_time_f=dosei->ConvertMemberToFloat(dosei->treatment_time, TIMER_PRE_CK)*60;   //sec
//    qDebug()<<"treat_time_f="<<treat_time_f;
    //    tmp = ConvertDose(dose1_act_data);
//    accui->SetActDos1((UINT32)tmp);

//    tmp = ConvertDoserate(dose1_aver_rate);
//    accui->SetActDosRate((UINT32)tmp);

//    tmp = ConvertMemberToFloat(treatment_time, TIMER_PRE_CK);
//    accui->SetActDosTime(tmp);

    emit base->ShowActDoseTimeMUSig();

    g_dosei_timer->stop();
    return true;
}

bool DoseOne::RevGantryPosition(TransRecvT &revdata)
{
    if (revdata.len != 6)
    {
        InsertInterlock(79, 961, ERROR_IL);   //Invalid length for command type from controller 1 .
        return false;
    }


    gantry_position_counts = (revdata.data[2] & 0x00FF) &
            ((revdata.data[3] << 8) & 0xFF00);
    return true;
}

void DoseOne::ProcessErrorIL()
{
    switch (error_now.xx)
    {
    case 0x7:    //interlock#2:treatment time escape/error#213安全联锁
        InsertInterlock(2, 107, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x5:   //Dynamic steering crossplane current exceeds window .
        InsertInterlock(64, 105, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x9:     //interlock#12:deviation greater than 20% between dose rate1 and voltage 安全联锁
        InsertInterlock(12, 109, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x17:    //interlock#12:virtual wedge下，the preset profile intensity by more than 25% of the max.安全联锁
        InsertInterlock(12, 123, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x0F:      //the calculated monitor 1 actual equals or exceeds the monitor1 presets plus 10%安全联锁
        InsertInterlock(56, 115, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x8:       //dose 1 offset value falls outside the range   非
        InsertInterlock(57, 108, SAFETY_IL, IL_READY|IL_INTERRUPT_NR, LATCHED);
        break;
    case 0xA:       //s0、s1、s3 greater than 3584 安全联锁
        InsertInterlock(58, 110, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0xE:       //#59 P/T out of range
        InsertInterlock(59, 114, SAFETY_IL, IL_READY|IL_PROGRAM|IL_INTERRUPT, LATCHED);
        break;
    case 0x3:       //Dose rate 1 offset greater than dose rate  1 非
        InsertInterlock(64, 103, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x4:       //'O' or ‘T’ command was received but HV is still on
        InsertInterlock(64, 104, ERROR_IL, IL_INTERRUPT|IL_TERMINATE, LATCHED);
        break;
    case 0x6:       //Preset treatment time equals 0
        InsertInterlock(64, 106, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0xB:       //HV ON signal erroneously active 非
        InsertInterlock(64, 111, ERROR_IL, IL_COMPLELE|IL_PROGRAM|IL_TERMINATE, LATCHED);
        break;
    case 0xC:       //RadON  is ‘on’ but ‘R’ ,’G’ or ‘I’ cmd not received .
        InsertInterlock(64, 112, ERROR_IL, IL_PROGRAM|IL_READY|IL_COMPLELE|IL_TERMINATE,
                        LATCHED);
        break;
    case 0xD:       //‘R’ command was received but HV is ‘off’
        InsertInterlock(64, 113, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x13:      //Dose calculation time-out （word没有）
        InsertInterlock(97, 119, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x14:      //Gantry has moved between READY and RAD ON .(安全联锁)
        InsertInterlock(64, 120, ERROR_IL, IL_READY|IL_RADON, LATCHED);
        break;
    case 0x15:      //RAM test failed
        InsertInterlock(64, 121, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x16:      //ROM test failed
        InsertInterlock(64, 122, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x18:      //Jaw position error for VW jaw motion (安全联锁)
        InsertInterlock(64, 124, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x1:       //Timeout on SIP polling detected by controller 1
        InsertInterlock(79, 101, ERROR_IL);
        break;
    case 0x2:       //Wrong command sequence detected by controller 1
        InsertInterlock(79, 102, ERROR_IL);
        break;
    case 0x61:      //Frame-# and checksum received by controller 1 are wrong
        InsertInterlock(79, 197, ERROR_IL);
        break;
    case 0x62:      //Checksum received by controller 1 is wrong
        InsertInterlock(79, 198, ERROR_IL);
        break;
    case 0x63:      //Frame-# received by controller 1 is wrong
        InsertInterlock(79, 199, ERROR_IL);
        break;
    case 0x10:      //Dose per degree out of tolerance
        InsertInterlock(82, 116, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x11:      //Gantry not moving or moving in the wrong direction
        InsertInterlock(82, 117, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x12:      //Malfunction in the parameter back-up system.安全
        InsertInterlock(93, 118, SAFETY_IL, IL_ALL & (~IL_RADON), LATCHED);
        break;
    default:        //Invalid data received from controller 1 安全联锁
        InsertInterlock(79, 971, ERROR_IL);
        break;

    }
}
