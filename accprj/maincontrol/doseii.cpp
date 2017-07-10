#include "doseii.h"
#include "ui/accui.h"

extern AccUi *accui;
extern float g_treat_time;

DoseTwo::DoseTwo()
{
    if (!ReadOwnFile())
    {
        ;//interlock
    }
    check_request = CHECK1;
    ready_status = NOTREADY;
    preset_treat_time = 0;
    preset_dose2 = 0;

    dose2_rate_offset = 0;
    //UINT32 dose2_rate2_offset;
    dose2_act_data = 0;  //dose 2 accumlated dose rate
    dose2_aver_rate = 0; // dose 2 average dose rate
    treatment_time = 0;//
    curry = 0;    //In-plane dynamic steering current

    segment_0 = 0;
    segment_2 = 0;
    segment_4 = 0;
    //UINT16 dyn_steer_current;
    prom_version = 0;

    //CHECKREPORT_TYPE_T check_report;
    check_report = 0;

    error_now.xx = 0;
    error_now.yy = 0;
    pressure = 0;
    temperature = 0;
    float_temp = 0.0;
    float_press = 0.0;

    jawA1_pos_counts = 0;
    jawA2_pos_counts = 0;
    jawB1_pos_counts = 0;
    jawB2_pos_counts = 0;

    send_frame_num = 0;
    rev_frame_num = 0;

    mode_now = RESET_MODE;

    for(int i=0; i<24; i++)
        item_has[i] = false;
}

DoseTwo::~DoseTwo()
{
    ;
}



bool DoseTwo::ReadOwnFile()
{
    QFile file(".\\AccConfig\\Doseii.xml");

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
                if (reader.name() == "Doseii")
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

bool DoseTwo::ReadOwnElement()
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
                if (reader.name() == "dose2_by_pass_mask")
                    dose2_by_pass_mask = reader.readElementText().toInt();
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

                reader.readNext();
            }
        }
//        reader.readNext();
    }

    return true;
}

bool DoseTwo::WriteOwnFile()
{
    QFile file(".\\AccConfig\\doseii.xml");

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
            if (root.tagName() == "Doseii")
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

void DoseTwo::WriteElement()
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
                if (node_l2.nodeName() == "gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[RESEARCH].gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[RESEARCH].drcv));
                else if (node_l2.nodeName() == "dsGainy")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[RESEARCH].dsGainY));
                else if (node_l2.nodeName() == "baly")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[RESEARCH].baly));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[RESEARCH].bitArray));
                else if (node_l2.nodeName() == "segment0Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[RESEARCH].segment0Gain));
                else if (node_l2.nodeName() == "segment2Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[RESEARCH].segment2Gain));
                else if (node_l2.nodeName() == "segment4Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[RESEARCH].segment4Gain));
                else if (node_l2.nodeName() == "dyn_steer_window")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer_window[RESEARCH]));
            }
        }
        else if (node_l1.nodeName() == "default")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[DEFAULT].gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[DEFAULT].drcv));
                else if (node_l2.nodeName() == "dsGainy")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[DEFAULT].dsGainY));
                else if (node_l2.nodeName() == "baly")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[DEFAULT].baly));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[DEFAULT].bitArray));
                else if (node_l2.nodeName() == "segment0Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[DEFAULT].segment0Gain));
                else if (node_l2.nodeName() == "segment2Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[DEFAULT].segment2Gain));
                else if (node_l2.nodeName() == "segment4Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[DEFAULT].segment4Gain));
                else if (node_l2.nodeName() == "dyn_steer_window")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer_window[DEFAULT]));
            }

        }
        else if (node_l1.nodeName() == "treatment")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[TREATMENT].gain));
                else if (node_l2.nodeName() == "drcv")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[TREATMENT].drcv));
                else if (node_l2.nodeName() == "dsGainy")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[TREATMENT].dsGainY));
                else if (node_l2.nodeName() == "baly")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[TREATMENT].baly));
                else if (node_l2.nodeName() == "bitarray")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_gain[TREATMENT].bitArray));
                else if (node_l2.nodeName() == "segment0Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[TREATMENT].segment0Gain));
                else if (node_l2.nodeName() == "segment2Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[TREATMENT].segment2Gain));
                else if (node_l2.nodeName() == "segment4Gain")
                    node_l2.firstChild().setNodeValue(QString::number(dose2_segment_gain[TREATMENT].segment4Gain));
                else if (node_l2.nodeName() == "dyn_steer_window")
                    node_l2.firstChild().setNodeValue(QString::number(dyn_steer_window[TREATMENT]));
            }

        }
    }
}

void DoseTwo::WriteOwnElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "dose2_by_pass_mask")
            node.firstChild().setNodeValue(QString::number(dose2_by_pass_mask));
        else if (node.nodeName() == "compensate_gain")
            node.firstChild().setNodeValue(QString::number(compensate_gain));
        else if (node.nodeName() == "jaw_x1_max_speed")
            node.firstChild().setNodeValue(QString::number(max_position_speed[3]));
        else if (node.nodeName() == "jaw_x2_max_speed")
            node.firstChild().setNodeValue(QString::number(max_position_speed[2]));
        else if (node.nodeName() == "jaw_y1_max_speed")
            node.firstChild().setNodeValue(QString::number(max_position_speed[0]));
        else if (node.nodeName() == "jaw_y2_max_speed")
            node.firstChild().setNodeValue(QString::number(max_position_speed[1]));
        else if (node.nodeName() == "collimator_type")
            node.firstChild().setNodeValue(QString::number(max_position_speed[4]));

        node = node.nextSibling();
    }
}

bool DoseTwo::CheckSoftpotDataLimit()
{
    bool suc = true;
    int i;
    for(i=0;i<3;i++)        //RESEARCH=0; DEFAULT=1; TREATMENT=2
    {
        suc &= CheckXMLData(dose2_gain[i].gain, DOSE2_GAIN_CK);
        suc &= CheckXMLData(dose2_gain[i].drcv, DRCV_CK);
        suc &= CheckXMLData(dose2_gain[i].dsGainY, DSGY_RSCH_CK);
        suc &= CheckXMLData(dose2_gain[i].baly, BALY_RSCH_CK);
        suc &= CheckXMLData(dose2_segment_gain[i].segment0Gain, S0_GAIN_CK);
        suc &= CheckXMLData(dose2_segment_gain[i].segment2Gain, S2_GAIN_CK);
        suc &= CheckXMLData(dose2_segment_gain[i].segment4Gain, S4_GAIN_CK);
        suc &= CheckXMLData(dyn_steer_window[i], DSWX_RSCH_CK);
    }
    suc &= CheckReschTreatToDefault(TREATMENT);

    return suc;
}

bool DoseTwo::CheckReschTreatToDefault(RE_TE_MODE mode)
{
    if( (dose2_gain[mode].gain<(dose2_gain[DEFAULT].gain*0.9) )
            || ( dose2_gain[mode].gain>(dose2_gain[DEFAULT].gain*1.1) ) )
        return false;
    if( (dose2_gain[mode].drcv<(dose2_gain[DEFAULT].drcv*0.9) )
            || ( dose2_gain[mode].drcv>(dose2_gain[DEFAULT].drcv*1.1) ) )
        return false;
    if( (dose2_gain[mode].dsGainY<(dose2_gain[DEFAULT].dsGainY*0.9) )
            || ( dose2_gain[mode].dsGainY>(dose2_gain[DEFAULT].dsGainY*1.1) ) )
        return false;
    if( (dose2_gain[mode].baly<(dose2_gain[DEFAULT].baly*0.9) )
            || ( dose2_gain[mode].baly>(dose2_gain[DEFAULT].baly*1.1) ) )
        return false;
    if( (dose2_segment_gain[mode].segment0Gain<(dose2_segment_gain[DEFAULT].segment0Gain*0.9) )
            || ( dose2_segment_gain[mode].segment0Gain>(dose2_segment_gain[DEFAULT].segment0Gain*1.1) ) )
        return false;
    if( (dose2_segment_gain[mode].segment2Gain<(dose2_segment_gain[DEFAULT].segment2Gain*0.9) )
            || ( dose2_segment_gain[mode].segment2Gain>(dose2_segment_gain[DEFAULT].segment2Gain*1.1) ) )
        return false;
    if( (dose2_segment_gain[mode].segment4Gain<(dose2_segment_gain[DEFAULT].segment4Gain*0.9) )
            || ( dose2_segment_gain[mode].segment4Gain>(dose2_segment_gain[DEFAULT].segment4Gain*1.1) ) )
        return false;
    if( (dyn_steer_window[mode]<(dyn_steer_window[DEFAULT]*0.9) )
            || ( dyn_steer_window[mode]>(dyn_steer_window[DEFAULT]*1.1) ) )
        return false;
    return true;
}

bool DoseTwo::CheckSoftpotDataIntegrity()
{
    for(int i=0; i<24; i++)
    {
        if(!item_has[i])
        {
            qDebug()<<"dose2 softpot integrity error:  i ="<<i;
            return false;
        }
    }
    return true;
}

bool DoseTwo::ReadElement()
{
    //qDebug()<<"read dose2 element";
    for(int i=0; i<24; i++)
        item_has[i] = false;

    reader.readNext();
    while (!reader.atEnd())
    {
        //qDebug()<<"reader name ="<<reader.name();
        reader.readNext();
        if (reader.isEndElement())
            break;

        if (reader.isStartElement())
        {
            if (reader.name() == "research")
            {
                while (! (reader.isEndElement() && reader.name() == "research") )
                {
                    //qDebug()<<" Dose2: research reader name ="<<reader.name();
                    if (reader.name() == "gain")
                    {
                        item_has[0] = true;
                        dose2_gain[RESEARCH].gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[1] = true;
                        dose2_gain[RESEARCH].drcv = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dsGainy")
                    {
                        item_has[2] = true;
                        dose2_gain[RESEARCH].dsGainY = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "baly")
                    {
                        item_has[3] = true;
                        dose2_gain[RESEARCH].baly =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose2_gain[RESEARCH].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "segment0Gain")
                    {
                        item_has[4] = true;
                        dose2_segment_gain[RESEARCH].segment0Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment2Gain")
                    {
                        item_has[5] = true;
                        dose2_segment_gain[RESEARCH].segment2Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment4Gain")
                    {
                        item_has[6] = true;
                        dose2_segment_gain[RESEARCH].segment4Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dyn_steer_window")
                    {
                        item_has[7] = true;
                        dyn_steer_window[RESEARCH] = reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "default")
            {
                while (! (reader.isEndElement() && reader.name() == "default") )
                {
                    //qDebug()<<"Dose2: default reader name ="<<reader.name();
                    if (reader.name() == "gain")
                    {
                        item_has[8] = true;
                        dose2_gain[DEFAULT].gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[9] = true;
                        dose2_gain[DEFAULT].drcv = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dsGainy")
                    {
                        item_has[10] = true;
                        dose2_gain[DEFAULT].dsGainY = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "baly")
                    {
                        item_has[11] = true;
                        dose2_gain[DEFAULT].baly =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose2_gain[DEFAULT].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "segment0Gain")
                    {
                        item_has[12] = true;
                        dose2_segment_gain[DEFAULT].segment0Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment2Gain")
                    {
                        item_has[13] = true;
                        dose2_segment_gain[DEFAULT].segment2Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment4Gain")
                    {
                        item_has[14] = true;
                        dose2_segment_gain[DEFAULT].segment4Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dyn_steer_window")
                    {
                        item_has[15] = true;
                        dyn_steer_window[DEFAULT] = reader.readElementText().toInt();
                    }
                    reader.readNext();
                }

            }
            else if (reader.name() == "treatment")
            {
                while (! (reader.isEndElement() && reader.name() == "treatment") )
                {
                    //qDebug()<<"Dose2: treatment reader name ="<<reader.name();
                    if (reader.name() == "gain")
                    {
                        item_has[16] = true;
                        dose2_gain[TREATMENT].gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "drcv")
                    {
                        item_has[17] = true;
                        dose2_gain[TREATMENT].drcv = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dsGainy")
                    {
                        item_has[18] = true;
                        dose2_gain[TREATMENT].dsGainY = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "baly")
                    {
                        item_has[19] = true;
                        dose2_gain[TREATMENT].baly =  reader.readElementText().toInt();
                    }
//                    else if (reader.name() == "bitarray")
//                        dose2_gain[TREATMENT].bitArray =  reader.readElementText().toInt();
                    else if (reader.name() == "segment0Gain")
                    {
                        item_has[20] = true;
                        dose2_segment_gain[TREATMENT].segment0Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment2Gain")
                    {
                        item_has[21] = true;
                        dose2_segment_gain[TREATMENT].segment2Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "segment4Gain")
                    {
                        item_has[22] = true;
                        dose2_segment_gain[TREATMENT].segment4Gain = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "dyn_steer_window")
                    {
                        item_has[23] = true;
                        dyn_steer_window[TREATMENT] = reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
        }

    }
    return true;
}

bool DoseTwo::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
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

UINT8 DoseTwo::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    // rev_frame_num_mutex.lock();
    fc = rev_frame_num << 4;
    rev_frame_num = (rev_frame_num + 1) % 16;
    // rev_frame_num_mutex.unlock();

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

UINT32 DoseTwo::ComputePresetDose(float rads)
{
    f_drcv = ConvertMemberToFloat(dose2_gain[g_mode].drcv, DRCV_CK);
    return (UINT32)(f_drcv * 409.5 * rads / dose_rate);
}

float DoseTwo::ConvertDose(UINT32 mcounts)
{
    return (float)((float)mcounts * (10.0 / 4095.0) * (dose_rate / f_drcv));
}

float DoseTwo::ConvertDoserate(UINT32 mcounts)
{
    return (float)(1000 * dose_rate * mcounts / (4095 * f_drcv));
}


bool DoseTwo::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x22;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe2;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendInformation()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    preset_dose2 = ComputePresetDose(f_preset_dose2);
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x4C;     //L
    tmpSend.data[2] = preset_treat_time & 0x00FF;
    tmpSend.data[3] = (preset_treat_time & 0xFF00) >> 8;
    tmpSend.data[4] = dose2_by_pass_mask & 0x00FF;
    tmpSend.data[5] = (dose2_by_pass_mask & 0xFF00) >> 8;
    tmpSend.data[6] = (preset_dose2 & 0x000000FF);
    tmpSend.data[7] = (preset_dose2 & 0x0000FF00) >> 8;
    tmpSend.data[8] = (preset_dose2 & 0x00FF0000) >> 16;
    tmpSend.data[9] = (preset_dose2 & 0xFF000000) >> 24;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11] = ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;       //fixme

    return InsertOutQueue(tmpSend);

}

bool DoseTwo::SendUserMode(const USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&doseii_mutex);
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

bool DoseTwo::SendGainAndWorkMode()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x48;     //H

    tmpSend.data[2] = dose2_gain[g_mode].gain & 0x00FF;
    tmpSend.data[3] = (dose2_gain[g_mode].gain & 0xFF00) >> 8;
    tmpSend.data[4] = dose2_gain[g_mode].drcv & 0x00FF;
    tmpSend.data[5] = (dose2_gain[g_mode].drcv & 0xFF00) >> 8;
    tmpSend.data[6] = dose2_gain[g_mode].dsGainY & 0x00FF;
    tmpSend.data[7] = (dose2_gain[g_mode].dsGainY & 0xFF00) >> 8;
    tmpSend.data[8] = dose2_gain[g_mode].baly & 0x00FF;
    tmpSend.data[9] = (dose2_gain[g_mode].baly & 0xFF00) >> 8;
    //tmpSend.data[10] = dose2_gain[g_mode].bitArray;
    switch(g_ray_type)
    {
    case E1:
    case E2:
    case E3:
    case E4:
    case E5:
    case E6:
        tmpSend.data[10] = 0x31;
        break;
    case X_LO:
        tmpSend.data[10] = 0x30;
        break;
    case X_HI:
        tmpSend.data[10] = 0x38;
        break;
    case ROT_X_LO:
        tmpSend.data[10] = 0x32;
        break;
    case ROT_X_HI:
        tmpSend.data[10] = 0x3A;
        break;
    default:
        tmpSend.data[10] = 0x30;
        break;
    }

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]))
        return false;

    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendSegmentGain()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x53;     //S

    tmpSend.data[2] = dose2_segment_gain[g_mode].segment0Gain & 0x00FF;
    tmpSend.data[3] = (dose2_segment_gain[g_mode].segment0Gain & 0xFF00) >> 8;
    tmpSend.data[4] = dose2_segment_gain[g_mode].segment2Gain & 0x00FF;
    tmpSend.data[5] = (dose2_segment_gain[g_mode].segment2Gain & 0xFF00) >> 8;
    tmpSend.data[6] = dose2_segment_gain[g_mode].segment4Gain & 0x00FF;
    tmpSend.data[7] = (dose2_segment_gain[g_mode].segment4Gain & 0xFF00) >> 8;
    tmpSend.data[8] = dyn_steer_window[g_mode] & 0x00FF;
    tmpSend.data[9] = (dyn_steer_window[g_mode] & 0xFF00) >> 8;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;

    tmpSend.data[11] = ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;

    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendMaxJawSpeedAndColType()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x56;     //V

    for (int i = 0; i < 4; i++)
    {
        tmpSend.data[i * 2 + 2] = (UINT8)(max_position_speed[i] & 0x00FF);
        tmpSend.data[i * 2 + 3] = (UINT8)((max_position_speed[i] & 0xFF00) >> 8);
    }

    tmpSend.data[10] = (UINT8)(max_position_speed[4] & 0x00FF);

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 11, tmpSend.data[11]))
        return false;

    tmpSend.data[12] = ComputeLstCHK(&tmpSend.data[0], 12);
    tmpSend.len = 13;

    return InsertOutQueue(tmpSend);
}



bool DoseTwo::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendReadyStatus(const READYSTATE_TYPE_T status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendRadOn()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    float dose2gain=(float)dose2_gain[g_mode].gain;
    compensate_gain=(int)(dose2gain*compen_factor);

    dose2_act_data = 0;     //LJY-12-22
//    qDebug()<<"dose2_gain="<<dose2_gain[g_mode].gain
//           <<", to float="<<dose2gain<<", compensate_gain="<<compensate_gain;

    //compensate_gain = dose2_gain[g_mode].gain;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x52;     //R
    tmpSend.data[2] = compensate_gain & 0x00FF;
    tmpSend.data[3] = (compensate_gain & 0xFF00) >> 8;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendInterrupt()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x4F;

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool DoseTwo::SendTerminateOrComplete()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TODOSEII;
    tmpSend.data[1] = 0x54;     //T

    QMutexLocker locker(&doseii_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

//TODO:add fuction
bool DoseTwo::AnalysRevFram(TransRecvT &revdata)
{
    if (revdata.data[1]=='C' && revdata.data[2]==0)
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case 'N':
        return RevDose2Offset(revdata);
    case 'D':
        return RevAccumlatedDose2(revdata);
    case 'P':
        return RevPeaks(revdata);
    case 'C':
        return RevResetEcho(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'E':
        return RevErrReport(revdata);
    case '1':
        return RevPressurenTemperature(revdata);
    case 'G':
        return RevJawPositionReport(revdata);
    case 'B':
        return RevLastTransmissionForCompletion(revdata);
    default:   //Invalid command type received from controller 2
        InsertInterlock(79, 952, ERROR_IL);
    }
    return false;
}

bool DoseTwo::RevDose2Offset(TransRecvT &revdata)
{
//    float tmp;
    if (revdata.len != 6)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    dose2_rate_offset = (revdata.data[2] & 0x00FF) |
            ((revdata.data[3] << 8) & 0xFF00);

//    tmp = ConvertMemberToFloat(dose2_rate_offset, DOSE_RATE2_OFF);
//    accui->SetActOff2((UINT32)tmp);
    emit base->ShowActSvcValueSig(OFF2);

    return true;

}

bool DoseTwo::RevAccumlatedDose2(TransRecvT &revdata)
{
//    float tmp;
    UINT32 tmp_dose2;

    if (revdata.len != 14)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

//    dose2_act_data =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
//            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);
    //LJY-12-21
    tmp_dose2 =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);
    if(dose2_act_data>tmp_dose2)
        InsertInterlock(62, 873, ERROR_IL, IL_ALL, LATCHED);
    dose2_act_data = tmp_dose2;


    dose2_aver_rate = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    treatment_time = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    curry = (revdata.data[10] & 0x00FF) |
            ((revdata.data[11] << 8) & 0xFF00);

    emit base->ShowActDoseTimeMUSig();
    emit base->ShowActSvcValueSig(DR_2);

    if (g_status == RAD_ON)
    {
        dose_rate_new = true;
        //g_doseii_acc_timer->start(7200);
       emit base->g_doseii_start_signal();
    }
    ProcessDoseRatesIL();

    return true;
}

bool DoseTwo::RevPeaks(TransRecvT &revdata)
{
//    float tmp;
    if (revdata.len != 10)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    segment_0 = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    segment_2 = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);
    segment_4 = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);

//    tmp = ConvertMemberToFloat(segment_2, S2_GAIN_CK);
//    accui->SetActSeg2((UINT32)tmp);

//    tmp = ConvertMemberToFloat(segment_4, S4_GAIN_CK);
//    accui->SetActSeg4((UINT32)tmp);

    emit base->ShowActSvcValueSig(SEG24);

    //DS_CY

    ProcessFlatnessIL();
    return true;
}

void DoseTwo::ProcessFlatnessIL()
{
    float s0, s2, s4, max, min;
    s0 = ConvertMemberToFloat(segment_0, S0_GAIN_CK);
    s2 = ConvertMemberToFloat(segment_2, S2_GAIN_CK);
    s4 = ConvertMemberToFloat(segment_4, S4_GAIN_CK);
    max = qMax(s0, qMax(s2, s4));
    min = qMin(s0, qMin(s2, s4));
    flatness = (max - min) / (max + min) * 100;
    symmetry = (s2 - s4) / (s2 + s4) * 100;

//    accui->SetActSye(symmetry);
//    accui->SetActFye(flatness);
    emit base->ShowActSvcValueSig(SYE_FYE);

    //interlock#7:Crossplane 超过FWI引起， 安全联锁
    if ((g_treat_time > 2.0f) && (flatness > fwi[g_mode]))
        InsertInterlockNoErr(7, SAFETY_IL, IL_RADON, LATCHED);

    //interlock#8:Crossplane 超过SWI（？的2倍），安全联锁
    if ((g_treat_time > 2.0f) && (symmetry > swi[g_mode]))
        InsertInterlockNoErr(8, SAFETY_IL, IL_RADON, LATCHED);
}

bool DoseTwo::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 932, ERROR_IL);       //Invalid frame number detected from controller 2 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 942, ERROR_IL);       //Invalid checksum detected from controller 2 安全联锁

        return false;
    }
    return true;

}



bool DoseTwo::RevResetEcho(TransRecvT &revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(2, true);
    g_c0_echo_bits_lock.unlock();

    return true;
}

bool DoseTwo::RevCheckReport(TransRecvT &revdata)
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    check_report =  revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
//        accui->SetActChkDispReport2(check_report);
        emit base->ShowActSvcValueSig(CHK_2);
    }

    if (check_report == 0x0)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(2, true);
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

bool DoseTwo::RevErrReport(TransRecvT &revdata)
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
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    ProcessErrorIL();
    return true;
}

bool DoseTwo::RevPressurenTemperature(TransRecvT &revdata)
{
    if (revdata.len != 8)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    temperature = (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00);
    pressure = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);

    float_temp = ref_temp - 15.0 + (float)temperature * 0.00733;
    float_press = ref_press - 30.0 + (float)pressure * 0.0146;

//    accui->SetActTemp2(float_temp);
//    accui->SetActPress2(float_press);

    emit base->ShowActSvcValueSig(TEMP_PRESS2);

    if ((g_ray_type & (0xF)) < 7)
    {
        press_temp_new = true;
        ProcessPressurenTemperatureIL();
    }
    return true;
}

bool DoseTwo::RevJawPositionReport(TransRecvT &revdata)
{
    if (revdata.len != 7)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    switch (revdata.data[2])
    {
    case 1:
        jawA1_pos_counts = (revdata.data[3] & 0x00FF)
                | ((revdata.data[4] << 8) & 0xFF00);
        break;
    case 2:
        jawA2_pos_counts = (revdata.data[3] & 0x00FF)
                | ((revdata.data[4] << 8) & 0xFF00);
        break;
    case 3:
        jawB1_pos_counts = (revdata.data[3] & 0x00FF)
                | ((revdata.data[4] << 8) & 0xFF00);
        break;
    case 4:
        jawB2_pos_counts = (revdata.data[3] & 0x00FF)
                | ((revdata.data[4] << 8) & 0xFF00);
        break;
    }

    return true;
}

bool DoseTwo::RevLastTransmissionForCompletion(TransRecvT &revdata)
{
//    float tmp;
    if (revdata.len != 14)
    {
        InsertInterlock(79, 962, ERROR_IL);   //Invalid length for command type from controller 2 .
        return false;
    }

    dose2_act_data =  (revdata.data[2] & 0x00FF) | ((revdata.data[3] << 8) & 0xFF00)
            | ((revdata.data[4] << 16) & 0xFF0000) | ((revdata.data[5] << 24) & 0xFF000000);

    dose2_aver_rate = (revdata.data[6] & 0x00FF) | ((revdata.data[7] << 8) & 0xFF00);
    treatment_time = (revdata.data[8] & 0x00FF) | ((revdata.data[9] << 8) & 0xFF00);
    curry = (revdata.data[10] & 0x00FF) |
            ((revdata.data[11] << 8) & 0xFF00);

    g_is_terminate = true;

//    tmp = ConvertDose(dose2_act_data);
//    accui->SetActDos2((UINT32)tmp);
    emit base->ShowActDoseTimeMUSig();

    //猜的begin
//    tmp = ConvertMemberToFloat(dose2_aver_rate, DOSE_RATE2_CK);
//    accui->SetActDscy(tmp);
    //emit base->ShowActSvcValueSig(DSCY);
    //end

    g_doseii_acc_timer->stop();
    return true;
}

void DoseTwo::ProcessErrorIL()
{
    switch (error_now.xx)
    {
    case 0xD:    //interlock#1:monitor 2/error#213安全联锁
        InsertInterlock(1, 213, SAFETY_IL, IL_RADON|IL_PROGRAM|IL_COMPLELE|IL_TERMINATE
                        , LATCHED);
        break;
    case 0x7:       //Treatment time actual >= treatment time preset
        InsertInterlock(2, 207, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x9:        //greater than 20% between dose rate2 and voltage.安全联锁
        InsertInterlock(13, 209, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x17:    //#13：virtual wedge下,安全联锁
        InsertInterlock(13, 223, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x0B:      //#59 Pressure/temperature 非
        InsertInterlock(59, 211, SAFETY_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0x08:      //#60 Dose2 Offset  非
        InsertInterlock(60, 208, SAFETY_IL, IL_READY|IL_INTERRUPT_NR,
                        LATCHED);
        break;
    case 0x0A:      //Dose2 saturation 安全联锁
        InsertInterlock(61, 210, SAFETY_IL, IL_RADON, LATCHED);
        break;
    case 0x3:       //Dose rate 2 offset greater than dose rate 2
        InsertInterlock(65, 203, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x4:       //‘O’ or ‘T’ command was received but HV is ‘on’
        InsertInterlock(65, 204, ERROR_IL, IL_INTERRUPT|IL_COMPLELE|IL_TERMINATE,
                        LATCHED);
        break;
    case 0x5:       //Dynamic steering inplane error
        InsertInterlock(65, 205, ERROR_IL, IL_ALL, LATCHED);
        //InsertInterlock(11);
        break;
    case 0x6:       //Treatment time programmed to 0.0 seconds
        InsertInterlock(65, 206, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0xE:       //RAdiation is on but ‘R’ , ‘H’ , or ‘L’ command not received
        InsertInterlock(65, 214, ERROR_IL, IL_PROGRAM|IL_READY|IL_COMPLELE|IL_TERMINATE,
                        LATCHED);
        break;
    case 0xF:       //‘R’ command was received but HV is OFF
        InsertInterlock(65, 215, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x15:      //RAM test failed .
        InsertInterlock(65, 221, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x16:      //RAM test failed .
        InsertInterlock(65, 222, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x01:      //Timeout on SIP polling detected by controller 2
        InsertInterlock(79, 201, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x02:      //Wrong command sequence detected by controller 2
        InsertInterlock(79, 202, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x61:      //Frame-# and checksum received by controller 2 are wrong
        InsertInterlock(79, 297, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x62:      //Checksum received by controller 2 is wrong
        InsertInterlock(79, 298, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x13:      //Dose calculation error.
        InsertInterlock(97, 219);
        break;
    case 0x63:      //Frame-# received by controller 2 is wrong
        InsertInterlock(79, 299, ERROR_IL, IL_ALL, LATCHED);
        break;
    default:        //Invalid data received from controller 2 安全联锁
        InsertInterlock(79, 972, ERROR_IL, IL_ALL, LATCHED);
        break;
    }
}
