#include"light.h"
#include "ui/accui.h"

extern AccUi *accui;

Light::Light()
{
    prom_version = 0;    //RESET_ECHO_MSG_ST reset_echo_msg;版本号等信息
    mode_now = RESET_MODE;
    check_index = CHECK1;
    ready_status_now = NOTREADY;
    error_now = 0;
    fieldlight_report = FIELDLIGHT_OFF;
    check_report = CHECK_PASSDE;
    line_status = ACTIVE;

    send_frame_num = 0;
    rev_frame_num = 0;

    light_on = false;
}

Light::~Light()
{

}

bool Light::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
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

UINT8 Light::ComputeRevCHK(UINT8* data, UINT8 len)
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

bool Light::SendC0()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = 0x25;
    tmpSend.data[1] = 0xc0;
    tmpSend.data[2] = 0x00;
    tmpSend.data[3] = 0xe5;
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Light::SendBeamShield()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x0f;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendFieldIlluminate()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x10;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendRoomLight()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x11;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendOpticalRangeFinder()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x12;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendOpticalBackPointer()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x13;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendExtendBeamshield()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x14;
    tmpSend.data[2] = 0x01;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendRetractBeamshield()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x18;
    tmpSend.data[2] = 0x01;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendBeamShieldStop()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x2c;
    tmpSend.data[2] = 0x00;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendExtendAndHandcontrol()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x34;
    tmpSend.data[2] = 0x01;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendRetractAndHandcontrol()
{
    TransRecvT tmpSend;

    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x38;
    tmpSend.data[2] = 0x01;
    memset(&tmpSend, 0, sizeof (tmpSend));
    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}


bool Light::SendUserMode(const USERMODE_TYPE_T mode)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;
    mode_now = mode;

    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x43;     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&light_mutex);
    if (mode_now == RESET_MODE)
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

bool Light::SendAccept()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x49;     //I
    tmpSend.data[2] = g_ray_type;//LJY-9-23

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendClear(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x49;     //L

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Light::SendRadOn(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x52;     //R

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}

bool Light::SendTerminateOrComplete(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x54;     //T

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertOutQueue(tmpSend);
}


bool Light::SendCheckRequest(const CHECKREQ_TYPE_T index)
{
    TransRecvT tmpSend;

    if (index != CHECK1 && index != CHECK2 &&
            index != CHECK2_WATCHDOG)
        return false;

    check_index = index;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x57;     //W
    tmpSend.data[2] = index;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;
    return InsertOutQueue(tmpSend);
}

bool Light::SendReadyStatus(const READYSTATE_TYPE_T status)
{
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    ready_status_now = status;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x5A;     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertOutQueue(tmpSend);
}

bool Light::SendInterrupt(void)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    tmpSend.data[0] = TOLIGHT;
    tmpSend.data[1] = 0x4f;     //O

    QMutexLocker locker(&light_mutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
    return InsertOutQueue(tmpSend);
}

bool Light::AnalysRevFram(TransRecvT& revdata)
{
    if (revdata.data[1]=='C')
        rev_frame_num = 0;

    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case 'C':
        return RevResetEcho(revdata);//C43
    case 'E':
        return RevErrorReport(revdata);//E45
    case 'F':
        return RevFieldlightReport(revdata);//F46
    case 'W':
        return RevCheckReport(revdata);//W57
    case 'Y':
        return RevEmergencyLineStatus(revdata);//Y59
    default:   //Invalid command type received from controller 5
        InsertInterlock(79, 955, ERROR_IL);

    }

    return false;
}

bool Light::RevResetEcho(TransRecvT& revdata) //C43
{
    if (revdata.len != 9)
    {
        InsertInterlock(79, 965, ERROR_IL);   //Invalid length for command type from controller 5.
        return false;
    }

    if (revdata.data[2] != 0)
        return false;

    memcpy(&prom_version, &(revdata.data[3]), sizeof (prom_version));
    g_c0_echo_bits_lock.lockForWrite();
    g_c0_echo_bits.setBit(5, true);
    g_c0_echo_bits_lock.unlock();

    return true;

}

bool Light::RevErrorReport(TransRecvT &revdata)//E45
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 965, ERROR_IL);   //Invalid length for command type from controller 5.
        return false;
    }

    error_now = revdata.data[2];

    ProcessErrorIL();
    return true;
}

bool Light::RevFieldlightReport(TransRecvT& revdata)//F46
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 965, ERROR_IL);   //Invalid length for command type from controller 5.
        return false;
    }
    fieldlight_report = (FIELD_LIGHT_REPORT_T)revdata.data[2];
    if(fieldlight_report == FIELDLIGHT_ON)
    {
        light_on = true;
        base->g_lighton_stop_signal();
    }
    else
        light_on = false;
    base->g_lightrep_stop_signal();

    return true;
}

bool Light::RevCheckReport(TransRecvT& revdata)//W57
{
    CHECKREQ_TYPE_T check_status;
    if (revdata.len != 5)
    {
        InsertInterlock(79, 965, ERROR_IL);   //Invalid length for command type from controller 5.
        return false;
    }

    check_report = (CHECK_LIGHT_REPORT_T)revdata.data[2];

    g_check_index_mutex.lock();
    check_status = g_check_index;
    g_check_index_mutex.unlock();

    if(check_status==CHECK2)
    {
//        accui->SetActChkDispReport5(check_report);
        emit base->ShowActSvcValueSig(CHK_5);
    }

    if (check_report == CHECK_PASSDE)
    {
        g_check_bits_mutex.lock();
        g_check_bits.setBit(5, true);
        g_check_bits_mutex.unlock();
    }
    else if (check_report == RAM_CHECK_FAILED)
    {
        //interlock
        ;
    }
    else if (check_report == ROM_CHECK_FAILED)
    {
        //interlock
        ;
    }
    else if (check_report == WATCHDOG_TEST_FAILED)
    {
        //interlock
        ;
    }
    else if (check_report == T1_TEST_FAILED)
    {
        //interlock
        ;
    }

    return true;
}

bool Light::RevEmergencyLineStatus(TransRecvT& revdata)
{
    if (revdata.len != 5)
    {
        InsertInterlock(79, 965, ERROR_IL);   //Invalid length for command type from controller 5.
        return false;
    }

    line_status = (T1_STATUS_T)revdata.data[2];

//    accui->SetActT1DispReport5(T1_status);
    emit base->ShowActSvcValueSig(T1_5);

    return true;
}

void Light::ProcessErrorIL()
{
    switch (error_now)
    {
    case 0x1:     //Handcont , -slave created emergency (pulled down emergency-line) .PDF没有
        InsertInterlock(68, 501, ERROR_IL);
        break;
    case 0x02:      //Wrong cmd sequence
        InsertInterlock(68, 502, ERROR_IL);
        break;
    case 0x03:      //Bad motion-enable condition
        InsertInterlock(68, 503, ERROR_IL);
        break;
    case 0x15:      //RAM test failed  非
        InsertInterlock(68, 521, ERROR_IL);
        break;
    case 0x16:      // ROM test failed .非
        InsertInterlock(68, 522, ERROR_IL);
        break;
    case 0x17:      // ROM and RAM test failed
        InsertInterlock(68, 523, ERROR_IL);
        break;
    case 0x1E:      //Wrong DIP-switch setting on controller 5 PCB 非
        InsertInterlock(68, 525, ERROR_IL);
        break;
        /*
    597   Frame-# and checksum received by controller 5 are wrong .
    598   Checksum received by controller 5 is wrong .
    599   Frame-# received by controller 5 is wrong.
    */
    case 0x61:
        InsertInterlock(79, 597, ERROR_IL);
        break;
    case 0x62:
        InsertInterlock(79, 598, ERROR_IL);
        break;
    case 0x63:
        InsertInterlock(79, 599, ERROR_IL);
        break;
    default:        //Invalid data received from controller 5 安全联锁
        InsertInterlock(79, 975, ERROR_IL);
        break;
    }
}

bool Light::ProcessCHKIL(TransRecvT &revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 935, ERROR_IL);       // Invalid frame number detected from controller 5 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 945, ERROR_IL);       //Invalid checksum detected from controller 5 安全联锁

        return false;
    }
    return true;
}
