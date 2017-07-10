#include "mlc.h"
#include "uppermonitor.h"
#include "can/Can2.h"
#include "remotecontroller.h"
#include "tools/watchdogtimer.h"
#include "defines.h"

extern bool g_is_remote_mode;
extern QWaitCondition g_maincontrol_wait;
extern Status g_status;
extern UpperMonitor *upper;
extern RemoteController *remote;
extern QTimer *g_mlc_movestop_timer;
extern Mlc *mlc;
extern QString g_rad_on_delay_time;

Mlc::Mlc()
{
    mSendFrameNum = 0;
    mRevFrameNum = 0;

    beamPos = 0;
    ctrlpointPos = 0;
}

bool Mlc::AnalysRevFram(TransRecvT& revdata)
{
    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case '7':
        RevMlcConnectAck(revdata);
        break;
    case 'N':
        RevFileParseEcho(revdata);
        break;
    case 'C':
        RevResetEcho(revdata);
        break;
    case 'S':
        RevMoveStop(revdata);
        break;
    case 'M':
        RevCtrlpointPosition(revdata);
        break;
    case 'B':
        RevMoveToBeam(revdata);
        break;
    case 'E':
        RevErrorReport(revdata);
        break;
    case 'H':
        RevHeartbeat(revdata);
        break;
    default:
        InsertInterlock(79, 958, ERROR_IL);     //invalid command type
        break;
    }

    return false;
}

bool Mlc::RevMlcConnectAck(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (g_is_remote_mode && !remote->mRemoteParam.mlcConnect)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "RecvMlcConnectAck" << endl;
#endif
        remote->mRemoteParam.mlcConnect = true;
        emit base->g_mlccon_stop_signal();
        //DeleteInterlock(124, 108);  need delete?
        g_maincontrol_wait.wakeOne();

        return true;
    }

    return false;
}

bool Mlc::RevResetEcho(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (g_is_remote_mode)
    {
        if (revdata.data[2] != 0)
        {
            return false;
        }

        g_c0_echo_bits_lock.lockForWrite();
        g_c0_echo_bits.setBit(8, true);
        g_c0_echo_bits_lock.unlock();

        g_maincontrol_wait.wakeOne();

        return true;
    }

    return false;
}

bool Mlc::RevFileParseEcho(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (g_is_remote_mode && !remote->mRemoteParam.mlcdataFeed )
    {
        remote->mFileCheck = (TREAT_FILE_CHECK_T)revdata.data[2];
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "Mlc::RevFileParseEcho" << remote->mFileCheck << endl;
#endif
        remote->mRemoteParam.mlcdataFeed = true;

        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool Mlc::RevMoveToBeam(TransRecvT& revdata)
{
    if (revdata.len != 7)
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (PROGRAM==g_status || NOT_READY==g_status)
    {
        if (revdata.data[2] == 0x01)    //success
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "RevMoveToBeam:success" << endl;
#endif
            remote->mMlcMoveToBeamEcho = MMBE_NOERR;
            mlc->DeleteInterlock(124);
        }
        else
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "RevMoveToBeam:fail" << endl;
#endif
            remote->mMlcMoveToBeamEcho = MMBE_MOV_FAIL;
            return false;
        }

        g_maincontrol_wait.wakeOne();
        return true;
    }

    if (INTERRUPT==g_status || INTERRUPT_NR==g_status)
    {
        if (revdata.data[2] == 0x01)    //success
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "RevMoveToBeam:success" << endl;
#endif
            mlc->DeleteInterlock(124);
        }
        else
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "RevMoveToBeam:fail" << endl;
#endif
            return false;
        }

        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool Mlc::RevMoveStop(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (g_is_remote_mode)
    {
        remote->isMlcMoveStop = true;
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "RevMoveStop" << endl;
#endif

        if (g_mlc_movestop_timer->isActive())
            emit base->g_mlc_movestop_stop_signal();

        if (g_status==RAD_ON && !upper->IsRadonComplete())
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "MLC move stop,RAD_ON, end abnormal" << endl;
#endif
            mlc->InsertInterlock(81, 101);
        }

        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool Mlc::RevErrorReport(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

    if (g_is_remote_mode)
    {
        //insert interlock
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "Mlc::RevErrorReport" << (ERROR_MLC_REPORT_T)revdata.data[2] << endl;
#endif
        ProcessErrorReportIL((ERROR_MLC_REPORT_T)(revdata.data[2]));
        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool Mlc::RevHeartbeat(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 8 .
        return false;
    }

#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "RevHeartbeat:mlc " << QTime::currentTime().toString("hh:mm:ss.zzz") << endl;
#endif
    if (g_is_remote_mode && remote->mIsMlcWork)
    {
        mlc->mIsHeartbeatGet = true;
        return true;
    }

    return false;
}

bool Mlc::RevCtrlpointPosition(TransRecvT &revdata)
{
    if (revdata.len != 7)  //判断长度是否符合要求
    {
        InsertInterlock(79, 968, ERROR_IL);   //Invalid length for command type from controller 9
        return false;
    }

    if (g_is_remote_mode)
    {
        beamPos = revdata.data[2];
        ctrlpointPos = (revdata.data[3] & 0x00FF) | ((revdata.data[4] << 8) & 0xFF00);
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "Mlc::RevCtrlpointPosition" << beamPos << ctrlpointPos << endl;
#endif
        remote->CheckCtrlpointPostion();
        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

void Mlc::ProcessErrorReportIL(ERROR_MLC_REPORT_T error_now)
{
    switch (error_now)
    {
    case EMR_FIELD_POSITION:
        if (RAD_ON == g_status)
        {
            InsertInterlockNoErr(105, SAFETY_IL, IL_ALL, LATCHED);
        }
        else
        {
            InsertInterlockNoErr(105, SAFETY_IL, IL_ALL, NOT_LATCHED);
        }
        break;
    case EMR_COVERAGE:
        if (RAD_ON == g_status)
        {
            InsertInterlockNoErr(106, SYSTEM_IL, IL_ALL, LATCHED);
        }
        else
        {
            InsertInterlockNoErr(106, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        }
        break;
    case EMR_LEAF_OUTOF_RANGE:
        InsertInterlock(107, 1071, SAFETY_IL, IL_ALL, LATCHED);
        break;
    case EMR_LEAF_OVERLAP:
        InsertInterlock(107, 1072, ERROR_IL, IL_ALL, LATCHED);
        break;
    case EMR_LEAF_UNINTEND_MOTION:
        if (RAD_ON == g_status)
        {
            InsertInterlockNoErr(122, SYSTEM_IL, IL_ALL, LATCHED);
        }
        else
        {
            InsertInterlockNoErr(122, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        }
        break;
    case EMR_HARDWARE_FAIL:
        InsertInterlockNoErr(112, ERROR_IL, IL_ALL, LATCHED);
        break;
    case EMR_LOGOFF:
        InsertInterlockNoErr(109, ERROR_IL, IL_ALL, LATCHED);  //logoff
        break;
    case EMR_MOVE_STOP:
        InsertInterlock(81, 101, ERROR_IL, IL_ALL, LATCHED);  //move stop
        break;
    case EMR_INVALID_FRAME:
        InsertInterlock(79, 938, ERROR_IL);
        break;
    case EMR_INVALID_CHECKSUM:
        InsertInterlock(79, 948, ERROR_IL);
        break;
    default:
        InsertInterlock(79, 978, ERROR_IL);
    }
}

bool Mlc::ProcessCHKIL(TransRecvT& revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    //heartbeat not check recv frame id
    if (revdata.data[1] == 0x48)
        return true;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 938, ERROR_IL);       // Invalid frame number detected from controller 8 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 948, ERROR_IL);       //Invalid checksum detected from controller 8 安全联锁

        return false;
    }
    return true;
}

//计算倒数第2位校验和，高4位表示序号，低4位异或得到
bool Mlc::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
{
    UINT8 low = 0, tmplow = 0;

    fc = mSendFrameNum << 4;

    mSendFrameNum = (mSendFrameNum + 1) % 16;

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return true;
}

UINT8 Mlc::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    // rev_frame_num_mutex.lock();
    fc = mRevFrameNum << 4;
    mRevFrameNum = (mRevFrameNum + 1) % 16;
    // rev_frame_num_mutex.unlock();

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

void Mlc::SetActDose(UINT32 actDose)
{
    if (mActDose != actDose)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "SetActDose:" << actDose << endl;
#endif
        mActDose = actDose;
        if (g_is_remote_mode && remote->mRemoteStatus!=WAIT_CONNECT)
        {
            SendActDos();
        }
    }
}

bool Mlc::SendConnectReq()
{
    mlc->ClearFrameNumber();
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendConnectReq" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOMLC;
    tmpSend.data[1]='7';
    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendFileReach()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendFileReach" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOMLC;
    tmpSend.data[1]='N';
    tmpSend.data[2]=upper->getBeamNumber();
    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);

    tmpSend.len = 5;
    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendResume()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendResume" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOMLC;
    tmpSend.data[1]='G';
    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendActDos()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendActDos" << mSendFrameNum << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'M';
    tmpSend.data[2] = (UINT8)(mActDose & 0xff);
    tmpSend.data[3] = (UINT8)(mActDose >> 8);

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendRadonDelayTime()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendRadonDelayTime" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    UINT16 delaytime = g_rad_on_delay_time.toUInt();
    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'D';
    tmpSend.data[2] = (UINT8)(delaytime & 0xff);
    tmpSend.data[3] = (UINT8)(delaytime >> 8);

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendPgmReach()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendPgmReach" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOMLC;
    tmpSend.data[1]='P';
    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendAccept()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendAccept" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOMLC;
    tmpSend.data[1]='I';
    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendCheck()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendCheck" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'W';

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendReadyStatus(const READYSTATE_TYPE_T &status)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendReadyStatus:" << status << endl;
#endif
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    // ready_status_now = status;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'Z';     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendRadOn(void)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendRadOn" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'R';     //R

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendInterrupt()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendInterrupt" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'O';

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendTerminateOrComplete(bool terminate)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'T';     //T
    if (terminate)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "mlc:SendTerminate" << endl;
#endif
        tmpSend.data[2] = 1;    //terminate
    }
    else
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "mlc:SendComplete" << endl;
#endif
        tmpSend.data[2] = 0;    //complete
    }

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendTreatEnd()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendTreatEnd" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'X';     //'X'

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendUserMode(const USERMODE_TYPE_T mode)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendUserMode:" << mode << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'C';     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&mMlcMutex);

    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;       //fixme

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendBeamNumber(UINT16 deliveredMU)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendBeamNumber" << deliveredMU << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'B';
    tmpSend.data[2] = upper->GetCurBeamId();
    if (BT_CONFORMAL==upper->GetBeamType() || BT_STATIC==upper->GetBeamType())     //适型和静态调强
    {
        tmpSend.data[3] = upper->GetCurCtrlPointId();
        tmpSend.data[4] = 0;
        tmpSend.data[5] = 0;
    }
    else        //动态调强
    {
        tmpSend.data[3] = 0;
        tmpSend.data[4] = (UINT8)(deliveredMU & 0xFF);
        tmpSend.data[5] = (UINT8)(deliveredMU >> 8);
    }

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;

    tmpSend.data[7] = ComputeLstCHK(&tmpSend.data[0], 7);
    tmpSend.len = 8;

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendHeartbeat()
{
#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "mlc:SendHeartbeat" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'H';

    QMutexLocker locker(&mMlcMutex);
    //heartbeat not add frame id
    /*if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;*/

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;
#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "发送心跳给MLC:" << QTime::currentTime().toString("hh:mm:ss.zzz") << endl;
#endif

    return InsertRemoteOutQueue(tmpSend);
}

bool Mlc::SendUnconnect()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc:SendUnconnect" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOMLC;
    tmpSend.data[1] = 'U';

    QMutexLocker locker(&mMlcMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

void Mlc::ClearFrameNumber()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "mlc set frame 0" << endl;
#endif
    mSendFrameNum = 0;
    mRevFrameNum = 0;
}
