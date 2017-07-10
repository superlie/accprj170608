#include "interlock.h"
#include "ui/accui.h"

extern AccUi *accui;
bool g_warmup_timeout;

Interlock::Interlock()
{
    usrmode = RESET_MODE;

    rdystat = NOTREADY;
    error_now = NoWrg;
    interlock_status.pcb = 0;
    interlock_status.bit_array = 0;

    check_report = 0;
    line_status = 0;

    prom_version = 0;

    send_frame_num = 0;
    rev_frame_num = 0;

    mode_now = RESET_MODE;

    for (int i = 0; i < INTERLOCK_NUM; i++)
    {
        g_interlocks[i].status = IL_CLAER;
        g_interlocks[i].latched = NOT_LATCHED;
    }

    ReadOwnFile();
    SetInterlockOvrdType();
    ReadErrorDisplayFile();
}

Interlock::~Interlock()
{

}

//-------------------------------------------------
//MODified by duanzhibin 2014/9/4
bool Interlock::ReadOwnFile()
{
    QFile file(".\\AccConfig\\interlock.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            qDebug()<<"error:open interlock.xml failed!";
            return false;
        }
        reader.setDevice(&file);
        reader.readNext();

        while (!reader.atEnd())
        {

            if (reader.isStartElement())
            {
                if (reader.name() == "Interlock")
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
    g_interlocks[11].ovrd = true;
    g_interlocks[32].ovrd = true;
    g_interlocks[32].ovrd = true;
    g_interlocks[99].ovrd = true;
    g_interlocks[100].ovrd = true;
    g_interlocks[101].ovrd = true;
    g_interlocks[102].ovrd = true;
    g_interlocks[103].ovrd = true;
    g_interlocks[104].ovrd = true;
    return true;
}

bool Interlock::ReadOwnElement()
{
    do
    {
        reader.readNext();
    }
    while (!reader.isStartElement());

    for (int i = 0; i < INTERLOCK_NUM; i++)
    {
        if (i == (reader.name().mid(4)).toInt())
        {
            while (!reader.isEndElement())
            {

                if(reader.name() == "ovrd")
                {
                    g_interlocks[i].ovrd = (bool)reader.readElementText().toInt();
                    if(g_interlocks[i].ovrd) g_lock_passby_bits.setBit(i);
                }
//                else if(reader.name() == "status")
//                    g_interlocks[i].status = (INTERLOCK_STATUS_T)reader.readElementText().toInt();
                else if(reader.name() == "error")
                {
                    g_interlocks[i].last_error_num = reader.readElementText().toInt();
                }
                else if(reader.name() == "history")
                    g_interlocks[i].history = reader.readElementText().toInt();

                reader.readNext();
            }
        }
        else
            reader.readNext();

        while (!reader.isStartElement())
        {
            reader.readNext();
            if (reader.isEndElement())
                break;
        }
    }
    return true;
}
bool Interlock::SetInterlockOvrdType()
{
    for(int i =0;i<INTERLOCK_NUM;i++)
    g_interlocks[i].ovrd_type = (OVRD_TYPE)(OVRD[i]);
    return true;
}
bool Interlock::WriteOwnFile()
{
    QFile file(".\\AccConfig\\interlock.xml");

    try
    {
        if (!file.open(QFile::ReadWrite | QFile::Text))
        {
            //interlock
            qDebug()<<"error: open interlock.xml failed!";
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
            if (root.tagName() == "Interlock")
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

//test by duanzhibin 9月2号
bool Interlock::WriteOwnElement()
{
    QDomNode node_l1;
    QDomNodeList  list_l1;
    node = root.firstChild();
    while(!node.isNull())
    {

        for(int i = 0;i<INTERLOCK_NUM;i++)
        {

            if(i == (node.nodeName().mid(4)).toInt())
            {
                list_l1 = node.childNodes();
                for(int j = 0;j < list_l1.count(); j++)
                {
                    node_l1 = list_l1.at(j);
                    if(node_l1.nodeName() == "ovrd")
                        node_l1.firstChild().setNodeValue(
                                    QString::number(g_interlocks[i].ovrd));
//                    else if (node_l1.nodeName() == "status")
//                        node_l1.firstChild().setNodeValue(
//                                    QString::number(g_interlocks[i].status));
                    else if(node_l1.nodeName() == "error")
                        node_l1.firstChild().setNodeValue(
                                    QString::number(g_interlocks[i].last_error_num));
                    else if(node_l1.nodeName() == "history")
                    {
                        node_l1.firstChild().setNodeValue(
                                    QString::number(g_interlocks[i].history));
                    }
                }
            }

        }
        node = node.nextSibling();
    }
    return true;
}


bool Interlock::ReadErrorDisplayFile()
{
    QFile file(".\\AccConfig\\errHistory.xml");

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            qDebug()<<"error:open errHistory.xml failed!";
            return false;
        }
        reader.setDevice(&file);
        reader.readNext();

        while (!reader.atEnd())
        {

            if (reader.isStartElement())
            {
                //if (reader.name() == "Interlock")
                if(reader.name() == "ErrorHistory")
                {
                    ReadErrorDiaplayElement();
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

bool Interlock::ReadErrorDiaplayElement()
{
    do
    {
        reader.readNext();
    }
    while (!reader.isStartElement());

    for (int i = 0; i < ERROR_NUM; i++)
    {
        if (i == (reader.name().mid(4)).toInt())
        {
            while (!reader.isEndElement())
            {

                 if(reader.name() == "history")
                    g_error_history[i] = reader.readElementText().toInt();
                    reader.readNext();
            }
        }

        while (!reader.isStartElement())
        {
            reader.readNext();
            if (reader.isEndElement())
                break;
        }
    }
    return true;
}

bool Interlock::WriteErrDisplayFile()
{
    QFile file(".\\AccConfig\\errHistory.xml");

    try
    {
        if (!file.open(QFile::ReadWrite | QFile::Text))
        {
            //interlock
            qDebug()<<"error: open interlock.xml failed!";
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
            if (root.tagName() == "ErrorHistory")
            {
                WriteErrDisplayElement();
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

bool Interlock::WriteErrDisplayElement()
{
    QDomNode node_l1;
    QDomNodeList  list_l1;
    QDomNode node = root.firstChild();
    while(!node.isNull())
    {

        for(int i = 0;i<ERROR_NUM;i++)
        {

            if(i == (node.nodeName().mid(4)).toInt())
            {
                list_l1 = node.childNodes();
                for(int j = 0;j < list_l1.count(); j++)
                {
                    node_l1 = list_l1.at(j);
                   if(node_l1.nodeName() == "history")
                    {
                        node_l1.firstChild().setNodeValue(
                                    QString::number(g_error_history[i]));

                    }
                }
            }

        }
        node = node.nextSibling();
    }
    return true;
}


bool Interlock::ComputeCHK(UINT8* data, UINT8 len, UINT8& fc)
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

bool Interlock::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x27;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe7;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

UINT8 Interlock::ComputeRevCHK(UINT8* data, UINT8 len)
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

bool Interlock::SendAccept()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x49;     //I

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendUserMode(USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&interlock_mutex);
    if (mode_now == RESET_MODE)    //fixme
    {
        send_frame_num = 0;         //发送reset命令，将序列号清零
        //rev_frame_num = 0;
    }

    if (!ComputeCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;       //fixme

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendSetup()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x53;     //S

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Interlock::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Interlock::SendReadyStatus(const READYSTATE_TYPE_T status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendRadOn(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x52;     //R

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendInterrupt()
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x4F;

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendTerminateOrComplete(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x54;     //T

    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Interlock::SendClear()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof(TransRecvT));
    tmpSend.data[0] = TOINTERLOCK;
    tmpSend.data[1] = 0x4c;
    QMutexLocker locker(&interlock_mutex);
    if (!ComputeCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Interlock::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C')
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch (revdata.data[1]) {
    case 'E':
        return RevErrorReprot(revdata);
    case 'K':
    case 'k':
        return RevInterlockStatus(revdata);
    case 'W':
        return RevCheckReport(revdata);
    case 'Y':
        return RevT1Report(revdata);
    case 'C':
        return RevResetEcho(revdata);
    default:   //Invalid command type received from controller 6
        InsertInterlock(79, 956, ERROR_IL);
        break;
    }
    return false;
}

bool Interlock::RevErrorReprot(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 966, ERROR_IL);   //Invalid length for command type from controller 6.
        return false;
    }

    error_now = (ERROR_INTERLOCK_REPORT_T)revdata.data[2];

    ProcessErrorReprotIL();
    return true;
}

bool Interlock::RevInterlockStatus(TransRecvT &revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 966, ERROR_IL);   //Invalid length for command type from controller 6.
        return false;
    }

    interlock_status.pcb = revdata.data[2];
    memcpy(&(interlock_status.bit_array),
           &(revdata.data[3]), sizeof (interlock_status.bit_array));
    memcpy(&(interlock_status.bit_array2),
           &(revdata.data[3]), sizeof (interlock_status.bit_array2));

    ProcessInterlockStatusIL();
    return true;
}

bool Interlock::RevCheckReport(TransRecvT &revdata)
{
    CHECKREQ_TYPE_T check_index;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 966, ERROR_IL);   //Invalid length for command type from controller 6.
        return false;
    }

    check_report = (CHECKREPORT_MOTOR_T)revdata.data[2];


    g_check_index_mutex.lock();
    check_index = g_check_index;
    g_check_index_mutex.unlock();

    if(check_index==CHECK2)
    {
//        accui->SetActChkDispReport6(check_report);
        emit base->ShowActSvcValueSig(CHK_6);
    }

    //按照新文档来写
    if (check_report == 0x0)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(6, true);
        g_check_bits_mutex.unlock();
    }
    else if (check_report == 0x1)
    {
        if (check_index == CHECK1)
        {
            ;
        }
        else if (check_index == CHECK2)
        {
            ;
        }
        //interlock
        ;
    }
    else if (check_report == 0x2)
    {
        if (check_index == CHECK1)
        {
            ;
        }
        else if (check_index == CHECK2)
        {
            ;
        }

        //interlock
        ;
    }
    else if (check_report == 0x4)
    {

    }


    return true;
}

bool Interlock::RevT1Report(TransRecvT &revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 966, ERROR_IL);   //Invalid length for command type from controller 6.
        return false;
    }

    line_status = revdata.data[2];

//    accui->SetActT1DispReport7(line_status);
    emit base->ShowActSvcValueSig(T1_6);

    return true;
}

bool Interlock::RevResetEcho(TransRecvT &revdata)
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 966, ERROR_IL);   //Invalid length for command type from controller 6.
        return false;
    }
    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]),
            sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(6, true);
    g_c0_echo_bits_lock.unlock();

    return true;
}

void Interlock::ProcessInterlockStatusIL()
{
    static bool warmup = false;
    if (interlock_status.bit_array != interlock_status.bit_array2)
        InsertInterlock(55, 850, SYSTEM_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT|
                        IL_RADON, LATCHED);       //非

    switch (interlock_status.pcb)
    {
    case 0: //pcb 2
        if (interlock_status.bit_array & (1 << 5))  //安全联锁
            InsertInterlockNoErr(21, SAFETY_IL, IL_RADON, LATCHED);
        if (interlock_status.bit_array & (1 << 7))  //安全联锁
            InsertInterlockNoErr(22, SAFETY_IL, IL_RADON, LATCHED);
        if (interlock_status.bit_array & (1 << 4))  //安全联锁
            InsertInterlockNoErr(23, SAFETY_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 12)) //安全联锁
            InsertInterlockNoErr(24, SAFETY_IL, IL_READY|IL_INTERRUPT|IL_RADON,
                            NOT_LATCHED);
        else
            DeleteInterlock(24);
        if (interlock_status.bit_array & (1 << 11)) //安全联锁
        {
            InsertInterlockNoErr(83, SYSTEM_IL, IL_ALL,
                            NOT_LATCHED);
            if(!warmup)
            {
                warmup = true;
                g_warmup_timeout = false;
                emit base->g_warmup_start_signal();
            }
        }
        else
        {
            DeleteInterlock(83);
            if(warmup)
            {
                warmup=false;
                if(!g_warmup_timeout)
                {
                    emit base->g_warmup_stop_signal();
                    InsertInterlockNoErr(14, ERROR_IL, IL_ALL, LATCHED);

                }
                g_warmup_timeout = false;
            }
        }

        //???
        if(interlock_status.bit_array & (1 << 10))      //  Clock
            InsertInterlockNoErr(84, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(84);
        //
        if(interlock_status.bit_array & (1 << 3))       // Dynamic steering
            InsertInterlockNoErr(11,SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(11);

        if(interlock_status.bit_array & (1 << 13))      // BMI (HW)
            InsertInterlockNoErr(47,SAFETY_IL, IL_RADON | IL_READY | IL_INTERRUPT | IL_TERMINATE | IL_COMPLELE, NOT_LATCHED);
        else
            DeleteInterlock(47);

        //去掉27号连锁,不适合本系统
        //???
        if (interlock_status.bit_array & (1 << 9))  //High voltage over current 安全联锁
        {
            InsertInterlockNoErr(27, SYSTEM_IL, IL_ALL, NOT_LATCHED);
            InsertInterlockNoErr(86, SYSTEM_IL, IL_ALL, LATCHED);
        }
        else
            DeleteInterlock(27);
        if (interlock_status.bit_array & (1 << 8))  //安全联锁 not latched
            InsertInterlockNoErr(26, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(26);
        if (interlock_status.bit_array & (1 << 2))  //安全联锁
            InsertInterlockNoErr(28, SAFETY_IL, IL_RADON, LATCHED);
        if (interlock_status.bit_array & (1 << 6))  //water flow 安全联锁
            InsertInterlockNoErr(42, SYSTEM_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 0))  //Monitor Synchronization安全联锁
            InsertInterlockNoErr(53, SAFETY_IL, IL_RADON, LATCHED);
        if (interlock_status.bit_array & (1 << 1))  //Dose rate 2 安全联锁
            InsertInterlockNoErr(54, SAFETY_IL, IL_RADON, LATCHED);

        break;
    case 1: //pcb 3
        if (interlock_status.bit_array & (1 << 10)) //安全联锁
            InsertInterlockNoErr(29, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(29);
        if (interlock_status.bit_array & (1 << 11)) //安全联锁
            InsertInterlockNoErr(30, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(30);
        if (interlock_status.bit_array & (1 << 3))  //安全联锁
            InsertInterlockNoErr(36, SAFETY_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 4))  //安全联锁
            InsertInterlockNoErr(37, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(37);
        if (interlock_status.bit_array & (1 << 5))  //37. GANTRY LIMIT SWITCH安全联锁
            InsertInterlockNoErr(38, SAFETY_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(38);
        if (interlock_status.bit_array & (1 << 6))  //安全联锁
            InsertInterlockNoErr(39, SYSTEM_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 12)) //Treatment room door安全联锁
            InsertInterlockNoErr(45, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(45);
        if (interlock_status.bit_array & (1 << 13)) //Table not locked 安全联锁
        {
            if (g_status==RAD_ON)
                InsertInterlockNoErr(48, SAFETY_IL, IL_ALL, LATCHED);
            else
                InsertInterlockNoErr(48, SAFETY_IL, IL_ALL, NOT_LATCHED);
        }
        else
            DeleteInterlock(48);
        if (interlock_status.bit_array & (1 << 7))  //25.	VACUUM OVER CURRENT
            InsertInterlockNoErr(25, SYSTEM_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 0))  //Water level is too low
            InsertInterlockNoErr(40, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(40);
        if (interlock_status.bit_array & (1 << 1))  //Water temperature is too high
            InsertInterlockNoErr(41, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(41);
        if (interlock_status.bit_array & (1 << 2))  //Water not pure enough
            InsertInterlockNoErr(43, SAFETY_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 9))  //Not enough air flow
            InsertInterlockNoErr(44, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(44);
        if (interlock_status.bit_array & (1 << 8))  //Hook stick not in holder or possible M class air flow program
            InsertInterlockNoErr(46, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        else
            DeleteInterlock(46);
        break;
    case 2: //pcb 4
        if (interlock_status.bit_array & (1 << 0))  //Electron foil in place and injection > 200 mA.安全
            InsertInterlockNoErr(94, SAFETY_IL, IL_RADON, LATCHED);
        if (interlock_status.bit_array & (1 << 1))  //Dose simulation circuit active in error.安全
            InsertInterlockNoErr(98, SAFETY_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 2))
            InsertInterlockNoErr(112, SAFETY_IL, IL_ALL, LATCHED);
        if (interlock_status.bit_array & (1 << 3))
            InsertInterlockNoErr(113, SAFETY_IL, IL_ALL, LATCHED);

        //下面第4,5,6,7,8,9,10,11,12,13,14,15位 应该都不适合本系统
//        if (interlock_status.bit_array & (1 << 4))
//            InsertInterlock(114, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 5))
//            InsertInterlock(115, SAFETY_IL, IL_ALL, LATCHED);
////        if (interlock_status.bit_array & (1 << 6))
////            InsertInterlock(116, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 7))
//            InsertInterlock(117, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 8))
//            InsertInterlock(118, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 9))
//            InsertInterlock(119, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 10))
//            InsertInterlock(120, SAFETY_IL, IL_ALL, LATCHED);
//        //不适用本系统
//        if (interlock_status.bit_array & (1 << 11))
//            InsertInterlock(121, SAFETY_IL, IL_ALL, LATCHED);
//        //不适用本系统
//        //        if (interlock_status.bit_array & (1 << 12))
//        //            InsertInterlock(122, SAFETY_IL, IL_ALL, LATCHED);
//        if (interlock_status.bit_array & (1 << 13))
//            InsertInterlock(123, SAFETY_IL, IL_ALL, LATCHED);
        break;
    }
}

void Interlock::ProcessErrorReprotIL()
{
    switch (error_now)
    {
    case JumpPCB:
        InsertInterlock(55, 612, SYSTEM_IL, IL_ALL,
                        NOT_LATCHED);   //非
        break;
    case InterlockIfcMiss:  //one of 3 interlocks is not installed.
        InsertInterlock(55, 613, SYSTEM_IL, IL_POWUP, LATCHED);
        break;
    case WrgCmd:    //Wrong command squence 非
        InsertInterlock(69, 602, ERROR_IL);
        break;
    case NotRelease:    //Hv-on,  rad-on&gantry_en not released非
        InsertInterlock(69, 609, ERROR_IL);
        break;
    case SpvLineErr:    //Supervisor lines erroneously released非
        InsertInterlock(69, 611, ERROR_IL, IL_PROGRAM | IL_TERMINATE, LATCHED);
        break;
    case RamFaile:      //RAM test failed非
        InsertInterlock(69, 621, ERROR_IL);
        break;
    case RomFaile:      //ROM test failed非
        InsertInterlock(69, 622, ERROR_IL);
        break;
    case RamRomFaile:   //RAM test and ROM test failed非
        InsertInterlock(69, 623, ERROR_IL);
        break;
    case WrgDIP:        //Wrong DIP-switch setting on controller 6 PCB非
        InsertInterlock(69, 630, ERROR_IL);
        break;

        /*
    697   Frame-# and checksum received by controller 6 are wrong .
    698   Checksum received by controller 6 is wrong .
    699   Frame-# received by controller 6 is wrong.
        */
    case WrgFrameNumChkSum:
        InsertInterlock(79, 697, ERROR_IL);
        break;
    case WrgChkSum:
        InsertInterlock(79, 698, ERROR_IL);
        break;
    case WrgFrameNum:
        InsertInterlock(79, 699, ERROR_IL);
        break;
    default:        //Invalid data received from controller 7 安全联锁
        InsertInterlock(79, 977, ERROR_IL);
        break;

    }
}

bool Interlock::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 936, ERROR_IL);       // Invalid frame number detected from controller 6 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 946, ERROR_IL);       //Invalid checksum detected from controller 6 安全联锁

        return false;
    }
    return true;
}
