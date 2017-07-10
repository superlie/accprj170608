#include "io.h"
#include "ui/accui.h"

extern AccUi *accui;
extern bool g_acc3_change;
//extern ACCESSORY_B_TYPE accessory_3_type;
//extern RemoteController *remote;

InputOutput::InputOutput()
{

    if (!ReadOwnFile())
    {
        ;//interlock
    }
    mode = RESET_MODE;
    ready_status = NOTREADY;
    checkeq = CHECK1;

    memset(&accA, 0, sizeof(accA));
    memset(&accB, 0, sizeof(accB));

    error_type = 0;
    ack.rayType = E1;
    ack.headBit = OKEY;
    ack.bit = 0;
    ack.head = 0;
    check = CHECK1_PASSED;
    rst.data = 0;
    rst.version = 0;
    line_status = T1_ACTIVE;

    //duanzhibin 9/24
    acc1_exist = false;
    acc2_exist = false;
    acc3_exist = false;
    acc1_invalid = false;
    acc2_invalid = false;
    acc3_invalid = false;
    has_inl19=false;
    has_inl20=false;
    has_inl52=false;

    io_jawxy_control=false;     //插入附件后，屏蔽主机对JAW的操作
    mirror_default_pos = true;

    send_frame_num = 0;
    rev_frame_num = 0;

    mode_now = RESET_MODE;

    for(int i=0; i<21; i++)
        item_has[i] = false;
}

InputOutput::~InputOutput()
{

}

bool InputOutput::ReadOwnFile()
{
    QFile file(".\\AccConfig\\io.xml");

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
                if (reader.name() == "IO")
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

bool InputOutput::ReadElement()
{
    //qDebug()<<"Read IO element";
    for(int i=0; i<21; i++)
        item_has[i] = false;

    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();

        if (reader.isStartElement())
        {
            if (reader.name() == "research")
            {
                while (!reader.isEndElement())
                {
                    //qDebug()<<" IO: research reader name ="<<reader.name();
                    if (reader.name() == "modulatorTrig")
                    {
                        item_has[0] = true;
                        trig[RESEARCH].modulatorTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "injectorTrig")
                    {
                        item_has[1] = true;
                        trig[RESEARCH].injectorTrig= reader.readElementText().toInt();
                    }
                    else if (reader.name() == "afcTrig")
                    {
                        item_has[2] = true;
                        trig[RESEARCH].afcTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "rfTrig")
                    {
                        item_has[3] = true;
                        trig[RESEARCH].rfTrig =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst0")
                    {
                        item_has[4] = true;
                        trig[RESEARCH].tst0 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst1")
                    {
                        item_has[5] = true;
                        trig[RESEARCH].tst1 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "doseTrig")
                    {
                        item_has[6] = true;
                        trig[RESEARCH].doseTrig =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "default")
            {
                while (!reader.isEndElement())
                {
                    //qDebug()<<" IO: default reader name ="<<reader.name();
                    if (reader.name() == "modulatorTrig")
                    {
                        item_has[7] = true;
                        trig[DEFAULT].modulatorTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "injectorTrig")
                    {
                        item_has[8] = true;
                        trig[DEFAULT].injectorTrig= reader.readElementText().toInt();
                    }
                    else if (reader.name() == "afcTrig")
                    {
                        item_has[9] = true;
                        trig[DEFAULT].afcTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "rfTrig")
                    {
                        item_has[10] = true;
                        trig[DEFAULT].rfTrig =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst0")
                    {
                        item_has[11] = true;
                        trig[DEFAULT].tst0 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst1")
                    {
                        item_has[12] = true;
                        trig[DEFAULT].tst1 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "doseTrig")
                    {
                        item_has[13] = true;
                        trig[DEFAULT].doseTrig =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
            else if (reader.name() == "treatment")
            {
                while (!reader.isEndElement())
                {
                    //qDebug()<<" IO: treatment reader name ="<<reader.name();
                    if (reader.name() == "modulatorTrig")
                    {
                        item_has[14] = true;
                        trig[TREATMENT].modulatorTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "injectorTrig")
                    {
                        item_has[15] = true;
                        trig[TREATMENT].injectorTrig= reader.readElementText().toInt();
                    }
                    else if (reader.name() == "afcTrig")
                    {
                        item_has[16] = true;
                        trig[TREATMENT].afcTrig = reader.readElementText().toInt();
                    }
                    else if (reader.name() == "rfTrig")
                    {
                        item_has[17] = true;
                        trig[TREATMENT].rfTrig =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst0")
                    {
                        item_has[18] = true;
                        trig[TREATMENT].tst0 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "tst1")
                    {
                        item_has[19] = true;
                        trig[TREATMENT].tst1 =  reader.readElementText().toInt();
                    }
                    else if (reader.name() == "doseTrig")
                    {
                        item_has[20] = true;
                        trig[TREATMENT].doseTrig =  reader.readElementText().toInt();
                    }
                    reader.readNext();
                }
            }
        }

    }
    return true;
}

bool InputOutput::ReadOwnElement()
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
                //                if (reader.name() == "foil_ray_type")
                //                    foil_ray_type = (RAY_TYPE_T)reader.readElementText().toInt();
                //                else if (reader.name() == "foil_table")
                //                    foil_table = (FOIL_TABLE_T)reader.readElementText().toInt();
                //                else if (reader.name() == "gantry_mode")
                //                    gantry_mode = (GANTRY_ENABLE_T)reader.readElementText().toInt();
                //                else if (reader.name() == "tap_ray_type")
                //                    tap_ray_type = (RAY_TYPE_T)reader.readElementText().toInt();
                //                else if (reader.name() == "type")
                //                    type = (TAP_T)reader.readElementText().toInt();
                if (reader.name() == "accept_ray_type")
                    accept_ray_type = (RAY_TYPE_T)reader.readElementText().toInt();
                else if (reader.name() == "afc_loop_mode")
                    afc_loop_mode = (AFC_LOOP_CTL_T)reader.readElementText().toInt();
                else if (reader.name() == "dos_loop_mode")
                    dos_loop_mode = (DOSE_LOOP_CTL_T)reader.readElementText().toInt();
                //                else if (reader.name() == "setup_ray_type")
                //                    setup_ray_type = (RAY_TYPE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e1")
                    foils[E1] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e2")
                    foils[E2] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e3")
                    foils[E3] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e4")
                    foils[E4] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e5")
                    foils[E5] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "foil_table_e6")
                    foils[E6] = (FOIL_TABLE_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e1")
                    taps[E1] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e2")
                    taps[E2] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e3")
                    taps[E3] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e4")
                    taps[E4] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e5")
                    taps[E5] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_e6")
                    taps[E6] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_x_lo")
                    taps[X_LO] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_x_hi")
                    taps[X_HI] = (TAP_T)reader.readElementText().toInt();
                else if (reader.name() == "taps_film")
                    taps[FILM] = (TAP_T)reader.readElementText().toInt();

                reader.readNext();
            }
        }
        //        reader.readNext();
    }
    if(!CheckFoilDataIntegrity())
        InsertInterlock(62, 805, ERROR_IL, IL_ALL, LATCHED);
    if(!CheckTapDataIntegrity())
        InsertInterlock(62, 807, ERROR_IL, IL_ALL, LATCHED);
    return true;
}

bool InputOutput::WriteOwnFile()
{
    QFile file(".\\AccConfig\\io.xml");

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
            if (root.tagName() == "IO")
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

void InputOutput::WriteOwnElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        //        if (node.nodeName() == "foil_ray_type")
        //            node.firstChild().setNodeValue(QString::number(foil_ray_type));
        //        else if (node.nodeName() == "foil_table")
        //            node.firstChild().setNodeValue(QString::number(foil_table));
        //        else if (node.nodeName() == "gantry_mode")
        //            node.firstChild().setNodeValue(QString::number(gantry_mode));
        //        else if (node.nodeName() == "tap_ray_type")
        //            node.firstChild().setNodeValue(QString::number(tap_ray_type));
        //        else if (node.nodeName() == "type")
        //            node.firstChild().setNodeValue(QString::number(type));
        if (node.nodeName() == "accept_ray_type")
            node.firstChild().setNodeValue(QString::number(accept_ray_type));
        else if (node.nodeName() == "afc_loop_mode")
            node.firstChild().setNodeValue(QString::number(afc_loop_mode));
        else if (node.nodeName() == "dos_loop_mode")
            node.firstChild().setNodeValue(QString::number(dos_loop_mode));
        //        else if (node.nodeName() == "setup_ray_type")
        //            node.firstChild().setNodeValue(QString::number(setup_ray_type));
        else if (node.nodeName() == "foil_table_e1")
            node.firstChild().setNodeValue(QString::number(foils[E1]));
        else if (node.nodeName() == "foil_table_e2")
            node.firstChild().setNodeValue(QString::number(foils[E2]));
        else if (node.nodeName() == "foil_table_e3")
            node.firstChild().setNodeValue(QString::number(foils[E3]));
        else if (node.nodeName() == "foil_table_e4")
            node.firstChild().setNodeValue(QString::number(foils[E4]));
        else if (node.nodeName() == "foil_table_e5")
            node.firstChild().setNodeValue(QString::number(foils[E5]));
        else if (node.nodeName() == "foil_table_e6")
            node.firstChild().setNodeValue(QString::number(foils[E6]));
        else if (node.nodeName() == "taps_e1")
            node.firstChild().setNodeValue(QString::number(taps[E1]));
        else if (node.nodeName() == "taps_e2")
            node.firstChild().setNodeValue(QString::number(taps[E2]));
        else if (node.nodeName() == "taps_e3")
            node.firstChild().setNodeValue(QString::number(taps[E3]));
        else if (node.nodeName() == "taps_e4")
            node.firstChild().setNodeValue(QString::number(taps[E4]));
        else if (node.nodeName() == "taps_e5")
            node.firstChild().setNodeValue(QString::number(taps[E5]));
        else if (node.nodeName() == "taps_e6")
            node.firstChild().setNodeValue(QString::number(taps[E6]));
        else if (node.nodeName() == "taps_x_lo")
            node.firstChild().setNodeValue(QString::number(taps[X_LO]));
        else if (node.nodeName() == "taps_x_hi")
            node.firstChild().setNodeValue(QString::number(taps[X_HI]));
        else if (node.nodeName() == "taps_film")
            node.firstChild().setNodeValue(QString::number(taps[FILM]));

        node = node.nextSibling();
    }
}

void InputOutput::WriteElement()
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
                if (node_l2.nodeName() == "modulatorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].modulatorTrig));
                else if (node_l2.nodeName() == "injectorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].injectorTrig));
                else if (node_l2.nodeName() == "afcTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].afcTrig));
                else if (node_l2.nodeName() == "rfTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].rfTrig));
                else if (node_l2.nodeName() == "tst0")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].tst0));
                else if (node_l2.nodeName() == "tst1")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].tst1));
                else if (node_l2.nodeName() == "doseTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[RESEARCH].doseTrig));
            }
        }
        else if (node_l1.nodeName() == "default")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "modulatorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].modulatorTrig));
                else if (node_l2.nodeName() == "injectorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].injectorTrig));
                else if (node_l2.nodeName() == "afcTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].afcTrig));
                else if (node_l2.nodeName() == "rfTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].rfTrig));
                else if (node_l2.nodeName() == "tst0")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].tst0));
                else if (node_l2.nodeName() == "tst1")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].tst1));
                else if (node_l2.nodeName() == "doseTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[DEFAULT].doseTrig));
            }
        }
        else if (node_l1.nodeName() == "treatment")
        {
            list_l2 = node_l1.childNodes();
            for (int j = 0; j < list_l2.count(); j++)
            {
                node_l2 = list_l2.at(j);
                if (node_l2.nodeName() == "modulatorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].modulatorTrig));
                else if (node_l2.nodeName() == "injectorTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].injectorTrig));
                else if (node_l2.nodeName() == "afcTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].afcTrig));
                else if (node_l2.nodeName() == "rfTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].rfTrig));
                else if (node_l2.nodeName() == "tst0")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].tst0));
                else if (node_l2.nodeName() == "tst1")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].tst1));
                else if (node_l2.nodeName() == "doseTrig")
                    node_l2.firstChild().setNodeValue(QString::number(trig[TREATMENT].doseTrig));
            }
        }
    }
}

bool InputOutput::CheckSoftpotDataLimit()
{
    bool suc = true;
    int i;
    for(i=0;i<3;i++)        //RESEARCH=0; DEFAULT=1; TREATMENT=2
    {
        suc &= CheckXMLData(trig[i].modulatorTrig, MODT_RSCH_CK);
        suc &= CheckXMLData(trig[i].injectorTrig, INJT_RSCH_CK);
        suc &= CheckXMLData(trig[i].afcTrig, AFCT_RSCH_CK);
        suc &= CheckXMLData(trig[i].rfTrig, RFT_RSCH_CK);
        suc &= CheckXMLData(trig[i].tst0, TST0_RSCH_CK);
        suc &= CheckXMLData(trig[i].tst1, TST1_RSCH_CK);
        suc &= CheckXMLData(trig[i].doseTrig, DOS_T_CK);
    }
    suc &= CheckReschTreatToDefault(TREATMENT);

    return suc;
}

bool InputOutput::CheckReschTreatToDefault(RE_TE_MODE mode)
{
    if( (trig[mode].modulatorTrig<(trig[DEFAULT].modulatorTrig*0.9) )
            || ( trig[mode].modulatorTrig>(trig[DEFAULT].modulatorTrig*1.1) ) )
        return false;
    if( (trig[mode].injectorTrig<(trig[DEFAULT].injectorTrig*0.9) )
            || ( trig[mode].injectorTrig>(trig[DEFAULT].injectorTrig*1.1) ) )
        return false;
    if( (trig[mode].afcTrig<(trig[DEFAULT].afcTrig*0.9) ) || ( trig[mode].afcTrig>(trig[DEFAULT].afcTrig*1.1) ) )
        return false;
    if( (trig[mode].rfTrig<(trig[DEFAULT].rfTrig*0.9) ) || ( trig[mode].rfTrig>(trig[DEFAULT].rfTrig*1.1) ) )
        return false;
    if( (trig[mode].tst0<(trig[DEFAULT].tst0*0.9) ) || ( trig[mode].tst0>(trig[DEFAULT].tst0*1.1) ) )
        return false;
    if( (trig[mode].tst1<(trig[DEFAULT].tst1*0.9) ) || ( trig[mode].tst1>(trig[DEFAULT].tst1*1.1) ) )
        return false;
    if( (trig[mode].doseTrig<(trig[DEFAULT].doseTrig*0.9) ) || ( trig[mode].doseTrig>(trig[DEFAULT].doseTrig*1.1) ) )
        return false;

    return true;
}

bool InputOutput::CheckSoftpotDataIntegrity()
{
    for(int i=0; i<21; i++)
    {
        if(!item_has[i])
        {
            qDebug()<<"io softpot integrity error:  i ="<<i;
            return false;
        }
    }
    return true;
}

bool InputOutput::CheckFoilDataIntegrity()
{
    for(int i=1;i<7;i++)       //E1-E6 = 1-6
    {
        if( foils[i]!=FOIL1 && foils[i]!=FOIL2 && foils[i]!=FOIL3
                && foils[i]!=FOIL4 && foils[i]!=FOIL5 )
            return false;
    }
    return true;
}

bool InputOutput::CheckTapDataIntegrity()
{
    for(int i=1;i<11;i++)       //E1-E6=1-6, ,X-lo=8,X-hi=9,File=10
    {
        if(i==7) continue;      //跳过7
        if( taps[i]!=TAP100V && taps[i]!=TAP122V && taps[i]!=TAP143V
                && taps[i]!=NOTAP )
            return false;
    }
    return true;
}

bool InputOutput::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
{
    UINT8 low = 0, tmplow = 0;
    if (len > 18)
        return false;

    // send_frame_num_mutex.lock();
    fc = send_frame_num << 4;
    send_frame_num = (send_frame_num + 1) % 16;
    // send_frame_num_mutex.unlock();

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return true;
}


UINT8 InputOutput::ComputeRevCHK(UINT8* data, UINT8 len)
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

bool InputOutput::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x26;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe6;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendAccept()
{
    TransRecvT tmpSend;

    accept_ray_type = (RAY_TYPE_T)g_ray_type;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x49;     //I
    tmpSend.data[2] = accept_ray_type;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;       //fixme

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendClear()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof(TransRecvT));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x4c;
    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendAfcLoopCtl()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x4D;     //M
    tmpSend.data[2] = afc_loop_mode;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendDosLoopCtl()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x4E;     //M
    tmpSend.data[2] = dos_loop_mode;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendUserMode(const USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&io_mutex);
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

bool InputOutput::SendFoilTable(const RAY_TYPE_T rayType)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (rayType < E1 || rayType > E6)
        return false;

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x44;     //D
    tmpSend.data[2] = rayType;
    memcpy(&(tmpSend.data[3]), &(foils[rayType]), 2);

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 5, tmpSend.data[5]))
        return false;
    tmpSend.data[6] = ComputeLstCHK(&tmpSend.data[0], 6);
    tmpSend.len = 7;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendTrigger()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    trig[g_mode].rayType = (RAY_TYPE_T)g_ray_type;
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x46;     //F
    tmpSend.data[2] = trig[g_mode].rayType;
    tmpSend.data[3] = trig[g_mode].doseTrig;
    tmpSend.data[4] = trig[g_mode].injectorTrig;
    tmpSend.data[5] = trig[g_mode].tst1;
    tmpSend.data[6] = trig[g_mode].rfTrig;
    tmpSend.data[7] = trig[g_mode].afcTrig;
    tmpSend.data[8] = trig[g_mode].modulatorTrig;
    tmpSend.data[9] = trig[g_mode].tst0;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 10, tmpSend.data[10]))
        return false;
    tmpSend.data[11] = ComputeLstCHK(&tmpSend.data[0], 11);
    tmpSend.len = 12;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendTap(const RAY_TYPE_T rayType)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (rayType < E1 || rayType > FILM || rayType == 7)
        return false;

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x48;     //H
    tmpSend.data[2] = rayType;
    tmpSend.data[3] = taps[rayType];

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;
    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendReadyStatus(const READYSTATE_TYPE_T &status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    // ready_status_now = status;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendSetup()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x53;     //S
    //tmpSend.data[2] = setup_ray_type;
    tmpSend.data[2] = g_ray_type;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendSetupFieldLight()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x53;     //S
    tmpSend.data[2] = FIELD_LIGHT;      //0x88

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendRadOn(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x52;     //R

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendInterrupt()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x4F;

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::SendTerminateOrComplete(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOIO;
    tmpSend.data[1] = 0x54;     //T

    QMutexLocker locker(&io_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool InputOutput::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C')
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case 'A':
        return RevAccessoryA(revdata);
    case 'B':
        return RevAccessoryB(revdata);
    case 'E':
        return RevErrorReport(revdata);
    case 'K':
        return RevAcknowledge(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'C':
        return RevResetEcho(revdata);
    case 'Y':
        return RevEmergencyLineStatus(revdata);
    default:   //Invalid command type received from controller 7
        InsertInterlock(79, 957, ERROR_IL);
        break;
    }

    return false;
}

//duanzhibin ----9/24
bool InputOutput::RevAccessoryA(TransRecvT& revdata)
{
    //    ACCESSORY_A2_TYPE accessory_2_type;
    ACCESSORY_2_DIC  accessory_2_dic;
    ACCESSORY_A1_TYPE accessory_1_type;

    if (revdata.len !=13)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }
    accA.flag = (EQUAL_NOT_T)revdata.data[2];

    accA.ACC1A = ((revdata.data[3] & 0x00FF) | ((revdata.data[4]<<8) & 0xFF00));
    accA.ACC1B = ((revdata.data[5] & 0x00FF) | ((revdata.data[6]<<8) & 0xFF00));
    accA.ACC2A = ((revdata.data[7] & 0x00FF) | ((revdata.data[8]<<8) & 0xFF00));
    accA.ACC2B = ((revdata.data[9] & 0x00FF) | ((revdata.data[10]<<8) & 0xFF00));

    accessory_1_type = (ACCESSORY_A1_TYPE)(accA.ACC1A & 0xFFF0);
    accessory_2_dic = (ACCESSORY_2_DIC)(accA.ACC2A & 0xFF00);//ACC2的a寄存器代表是方向
    accessory_2_type = (ACCESSORY_A2_TYPE)(accA.ACC2B & 0xFE00);//ACC2的b寄存器达标数据  去掉低9位

    accui->SetAccessoy_1_Disp(accessory_1_type);//显示acc1
    accui->SetAccessoy_2_Disp(accessory_2_type,accessory_2_dic);

    switch (accessory_1_type)
    {
    case RETI:
    case RETI_1:
        acc1_exist = true;
        acc1_invalid =false;
        break;
    case RETI_NULL:
    case RETI_NULL_1:
        acc1_exist = false;
        acc1_invalid =false;
        break;
    default:
        acc1_exist=true;
        acc1_invalid = true;
        break;
    }

    switch(accessory_2_dic)
    {
    case DIC_1:
    case DIC_2:
    case DIC_3:
    case DIC_4:
        switch (accessory_2_type) {
        case RW_15:
        case RW_30:
        case RW_45:
        case RW_60:
            acc2_exist=true;
            acc2_invalid = false;
            break;
        case RWNULL_1:
        case RWNULL_2:
            acc2_exist=false;
            acc2_invalid = false;
            break;
        default:
            acc2_exist=true;
            acc2_invalid = true;
            break;
        }
        break;
    case DNULL_1:
    case DNULL_2:
        acc2_exist=false;
        acc2_invalid=false;
        break;
    default:
        acc2_exist=true;
        acc2_invalid=true;
        break;
    }
    return true;
}

bool InputOutput::RevAccessoryB(TransRecvT& revdata)
{

    if (revdata.len !=13)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    accB.flag = (EQUAL_NOT_T)revdata.data[2];
    /*不知道是否要移位操作*/
    //    memcpy(&accB.ACC3A, &(revdata.data[3]), sizeof (accB.ACC3A));
    //    memcpy(&accB.ACC3B, &(revdata.data[5]), sizeof (accB.ACC3B));
    //    memcpy(&accB.inPlane, &(revdata.data[7]), sizeof (accB.inPlane));
    //    memcpy(&accB.crossPlane, &(revdata.data[9]), sizeof (accB.crossPlane));
    accB.ACC3A = ((revdata.data[3] & 0x00FF) | ((revdata.data[4]<<8) & 0xFF00));
    accB.ACC3B = ((revdata.data[5] & 0x00FF) | ((revdata.data[6]<<8) & 0xFF00));
    accB.inPlane = ((revdata.data[7] & 0x00FF) | ((revdata.data[8]<<8) & 0xFF00));
    accB.crossPlane = ((revdata.data[9] & 0x00FF) | ((revdata.data[10]<<8) & 0xFF00));

    accessory_3_type = (ACCESSORY_B_TYPE)(accB.ACC3A & 0xFFC0); // 去除后6位

    io_jawxy_control=true;
    g_acc3_change = true;

    switch (accessory_3_type)
    {
    case EA205:
    case EA210:
    case EA215:
    case EA220:
    case EA225:
        acc3_exist = true;
        acc3_invalid =false;
        break;
    case EANULL:
    case EANULL_1:
        acc3_exist = false;
        io_jawxy_control=false;
        acc3_invalid =false;
        break;
    default:
        acc3_exist = true;
        acc3_invalid = true;
        break;

    }
    return true;
}

void InputOutput::CheckAccInterlock()
{

    int type;
    bool ch1,ch2,ch3,ch4,ch5;

    type=g_ray_type & (0xF);
    //电子线状态下没有限光筒，或有契型板
    ch1 = (type < 7) && ( !acc3_exist || acc2_exist) ;  // ch1=true ,has interlock
    //电子线下有十字交叉板
    ch2 = (type < 7) && acc1_exist ; // if ch2=true, has interlock 20
    //x射线下有限光筒
    ch3 = ((type == 8) || (type == 9)) &&  acc3_exist;  // if ch3=true, has interlock 19

    ch4 = acc1_exist && acc2_exist ;//accessory A
    ch5 = acc3_exist && (acc1_exist || acc2_exist);//accessoryB

    if(ch1 ||ch3 || ch4 ||ch5)
    {
        if (!has_inl19)
        {
            has_inl19=true;
            InsertInterlockNoErr(19, SYSTEM_IL, IL_READY | IL_INTERRUPT, NOT_LATCHED);
        }
    }
    else
    {
        if (has_inl19)
        {
            has_inl19=false;
            DeleteInterlock(19);
        }
    }
    if(ch2)
    {
        if (!has_inl20)
        {
            InsertInterlockNoErr(20, SYSTEM_IL, IL_READY | IL_INTERRUPT, NOT_LATCHED);
            has_inl20=true;
        }
    }
    else
    {
        if (has_inl20)
        {
            DeleteInterlock(20);
            has_inl20=false;
        }

    }
    qDebug()<<"ACC: "<<acc1_invalid<<acc2_invalid<<acc3_invalid<<", ilk52="<<has_inl52;
    if(acc1_invalid || acc2_invalid || acc3_invalid )
    {
        if (!has_inl52)
        {
            InsertInterlockNoErr(52, SYSTEM_IL, IL_READY | IL_INTERRUPT | IL_RADON, NOT_LATCHED);
            has_inl52=true;
        }
    }
    else
    {
        if (has_inl52)
        {
            DeleteInterlock(52);
            has_inl52=false;
        }
    }
}

bool InputOutput::RevErrorReport(TransRecvT& revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    error_now = revdata.data[2];

    ProcessErrorReportIL();

    return true;
}


bool InputOutput::RevAcknowledge(TransRecvT& revdata)
{
    if (revdata.len != 10)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    //LJY-12-24
    if(revdata.data[2] == 0x88)      //Field light
    {
        //移动到默认位置
        mirror_default_pos = true;
        light->SendFieldIlluminate();
        return true;
    }

    ack.rayType = (RAY_TYPE_T)revdata.data[2];
    ack.headBit = (HEAD_BITS_T)revdata.data[3];
    /*head和bit数据还没做移位操作*/
    memcpy(&ack.head, &(revdata.data[4]), sizeof (ack.head));
    memcpy(&ack.bit, &(revdata.data[6]), sizeof (ack.bit));

    accui->headdata = (revdata.data[4]<<8) | revdata.data[5];
    accui->bitsdata = (revdata.data[6]<<8) | revdata.data[7];

    //errlist85 <<"869	‘K’ command from IO controller: bits unequal ("<<revdata.data[4]<<revdata.data[5]<<").";
    //errlist[85]<<"869	‘K’ command from IO controller: bits unequal (00 00).";
    //    if (ack.rayType !=  accept_ray_type)
    //        g_io_acknowledge = false;
    g_io_acknowledge = true;

    ProcessAcknowledgeIL();

    return true;
}

bool InputOutput::RevCheckReport(TransRecvT& revdata)
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    check_report = (CHECK_IO_REPORT_T)revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
        //        accui->SetActChkDispReport7(check_report);
        emit base->ShowActSvcValueSig(CHK_7);
    }

    if (check_status == CHECK1)
    {
        if (check_report == CHECK1_PASSED)
        {
            g_check_bits_mutex.lock();
            g_check_bits.setBit(7, true);
            g_check_bits_mutex.unlock();
        }
        else if (check_report == CK1_RAM_FAILED)
        {
            //interlock
            ;
        }
        else if (check_report == CK1_ROM_FAILED)
        {
            //interlock
            ;
        }
    }
    else if (check_status == CHECK2)
    {
        if (check_report == CHECK2_PASSED)
        {
            g_check_bits_mutex.lock();
            g_check_bits.setBit(7, true);
            g_check_bits_mutex.unlock();
        }
        else if (check_report == WATCHDOG_FAILED)
        {
            //interlock
            ;
        }
        else if (check_report == SPV_TOFF_FIALED)
        {
            //interlock
            ;
        }
        else if (check_report == SPV_RLS_FIALED)
        {
            //interlock
            ;
        }
        else if (check_report == T1_FAILED)
        {
            //interlock
            ;
        }
    }

    return true;
}

bool InputOutput::RevResetEcho(TransRecvT& revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(7, true);
    g_c0_echo_bits_lock.unlock();

    return true;
}

bool InputOutput::RevEmergencyLineStatus(TransRecvT& revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 967, ERROR_IL);   //Invalid length for command type from controller 7.
        return false;
    }

    line_status = (T1_IO_REPORT_T)revdata.data[2];

    //    accui->SetActT1DispReport6(line_status);k
    emit base->ShowActSvcValueSig(T1_7);

    return true;
}

void InputOutput::ProcessAcknowledgeIL()            // VICTOR
{
    //#15: headbit not equal
    if (ack.headBit==0x01)
    {
        InsertInterlock(15, 862, SAFETY_IL, IL_NOT_READY, LATCHED);
        InsertInterlock(85, 869, SYSTEM_IL, IL_ALL, LATCHED);
    }
    else if (ack.headBit==0x02)
        InsertInterlock(15, 862, SAFETY_IL, IL_NOT_READY, LATCHED);
    else if (ack.headBit==0x03)
        InsertInterlock(85, 869, SYSTEM_IL, IL_ALL, LATCHED);

    // mode not same as prset of ACCEPT command
    if (ack.rayType !=  accept_ray_type)
    {
        if (g_status == RAD_ON)
            InsertInterlock(15, 861, SAFETY_IL, IL_NOT_READY, LATCHED);
        else
            InsertInterlock(15, 861, SAFETY_IL, IL_NOT_READY, NOT_LATCHED);
    }
    else
        DeleteInterlock(15,861);


    //mode not same as mode of console      安全联锁
    /*
    if (ack.rayType != g_ray_type )  //||yy||zz...
        InsertInterlock(15, 861, SAFETY_IL, IL_NOT_READY| IL_READY, LATCHED);
        */
}


void InputOutput::ProcessErrorReportIL()
{
    /*
       702   Wrong command sequence .
       703   Check-time for written data is still active .
       704   Foil-table not initialized .
       705   Wrong foil doenload data .
       706   During rad_on : accessories are changed .
       707   Keyboard’s  /RAD_ON EN not released .
       708   Not ready to change enerigies (during HV) .
       709   Invalid parameter for ‘C’ command (Cx) .
       710   Rad_on not released .
       711   Hv_on not released .
       712   Rad_on and Hv_on not released .
       713   Gantry_en not released .
       714   Rad_on and gantry_en not released .
       715   Hv_on and gantry_en not released .
       716   Hv_on ,Rad_on & gantry_en not released .
       717   Foils/targets and BITs (RAD_ON,HV_ON,etc.) are wrong before hv_on .
       718   Foils/targets are wrong before hv_on .
       719   BITs (RAD_ON,HV_ON,etc.) are wrong before hv_on .
       720   Supervisor lines erroneously released .
       721   RAM test failed .
       722   ROM test failed .
       723   RAM test and ROM test failed .
       724   Interrupt lines not released .
       725   Invalid tap selections download (F1, F2) .
       726   Missing ‘0’ command for interrupt .
       727   Bad accessory data received (< 8 counts) .
       728   RAD-ON EN erroneously active .
       729   ‘R’ command received with field light on .
       730   Wrong DIP-switch position on slave-PCB

        797   Frame-# and checksum received by controller 7 are wrong .
        798   Checksum received by controller 7 is wrong .
        799   Frame-# received by controller 7 is wrong.

    */
    switch (error_now)
    {
    case 0x2:          //Wrong command sequence 非
        InsertInterlock(70, 702, ERROR_IL);
        break;
    case 0x3:
        InsertInterlock(70, 703, ERROR_IL);
        break;
    case 0x4:
        InsertInterlock(70, 704, ERROR_IL);
        break;
    case 0x5:
        InsertInterlock(70, 705, ERROR_IL);
        break;
    case 0x6:       //安全联锁
        InsertInterlock(70, 706, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x7:       //安全联锁
        InsertInterlock(70, 707, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x8:
        InsertInterlock(70, 708, ERROR_IL);
        break;
    case 0x9:
        InsertInterlock(70, 709, ERROR_IL);
        break;
    case 0xA:
        InsertInterlock(70, 710, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0xB:
        InsertInterlock(70, 711, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0xC:
        InsertInterlock(70, 712, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0xD:
        InsertInterlock(70, 713, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0xE:
        InsertInterlock(70, 714, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0xF:
        InsertInterlock(70, 715, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
    case 0x10:
        InsertInterlock(70, 716, ERROR_IL, IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0x11:
        InsertInterlock(70, 717, ERROR_IL, IL_RADON,
                        LATCHED);
        break;
    case 0x12:
        InsertInterlock(70, 718, ERROR_IL, IL_RADON,
                        LATCHED);
        break;
    case 0x13:       //安全联锁
        InsertInterlock(70, 719, ERROR_IL, IL_RADON,
                        LATCHED);
        break;
    case 0x14:
        InsertInterlock(70, 720, ERROR_IL, IL_PROGRAM,
                        LATCHED);
        break;
    case 0x15:
        InsertInterlock(70, 721, ERROR_IL);
        break;
    case 0x16:
        InsertInterlock(70, 722, ERROR_IL);
        break;
    case 0x17:
        InsertInterlock(70, 723, ERROR_IL);
        break;
    case 0x18:       //安全联锁
        InsertInterlock(70, 724, ERROR_IL, IL_ALL, LATCHED);
        break;
    case 0x19:
        InsertInterlock(70, 725, ERROR_IL);
        break;
    case 0x1A:       //安全联锁
        InsertInterlock(70, 726, ERROR_IL, IL_RADON, LATCHED);
        break;
    case 0x1B:
        InsertInterlock(70, 727, ERROR_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT,
                        LATCHED);
        break;
    case 0x1C:
        InsertInterlock(70, 728, ERROR_IL, IL_CHECK1 | IL_CHECK2 | IL_READY, LATCHED);
        break;
    case 0x1D:
        InsertInterlock(70, 729, ERROR_IL);
        break;
    case 0x1E:
        InsertInterlock(70, 730, ERROR_IL);
        break;
    case 0x61:
        InsertInterlock(79, 797, ERROR_IL);
        break;
    case 0x62:
        InsertInterlock(79, 798, ERROR_IL);
        break;
    case 0x63:
        InsertInterlock(79, 799, ERROR_IL);
        break;
    default:        //Invalid data received from controller 6 安全联锁
        InsertInterlock(79, 976, ERROR_IL);
        break;

    }
}

bool InputOutput::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 937, ERROR_IL);       // Invalid frame number detected from controller 7 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 947, ERROR_IL);       //Invalid checksum detected from controller 7 安全联锁

        return false;
    }
    return true;
}
