#include "uppermonitor.h"
#include "can/Can2.h"
#include "remotecontroller.h"
#include "tools/watchdogtimer.h"
#include "tools/cfloatcompare.h"
#include <math.h>
#include "../ui/accui.h"
#include "defines.h"

extern bool g_is_remote_mode;
extern QWaitCondition g_maincontrol_wait;
extern Status g_status;
extern bool g_is_complete;          //complete标志
extern USERMODE_TYPE_T g_usermode;
extern RemoteController *remote;
extern UpperMonitor *upper;
extern AccUi *accui;
extern QString g_machine_name;

UINT16 g_next_total_dos = 0;
TREAT_RESUME_ST g_resume_info;
QMutex g_treatqueue_mutex;

UpperMonitor::UpperMonitor()
{
    mSendFrameNum = 0;
    mRevFrameNum = 0;

    beamPos = 0;
    ctrlpointPos = 0;

    setedAcc1 = "NONE";
    setedAcc2 = "NONE";
    setedAcc3 = "NONE";

    //error_now;
}

void UpperMonitor::InitAccMap()
{
    mAccMap[0]["无"] = 0x00;
    mAccMap[0]["NONE"] = 0x00;
    mAccMap[0]["RETI"] = 0x01;
    mAccMap[0]["未识别"] = 0x02;
    mAccMap[0]["INVALID"] = 0x02;

    mAccMap[1]["无"] = 0x00;
    mAccMap[1]["NONE"] = 0x00;
    mAccMap[1]["1RW15"] = 0x01;
    mAccMap[1]["1RW30"] = 0x02;
    mAccMap[1]["1RW45"] = 0x03;
    mAccMap[1]["1RW60"] = 0x04;
    mAccMap[1]["2RW15"] = 0x05;
    mAccMap[1]["2RW30"] = 0x06;
    mAccMap[1]["2RW45"] = 0x07;
    mAccMap[1]["2RW60"] = 0x08;
    mAccMap[1]["3RW15"] = 0x09;
    mAccMap[1]["3RW30"] = 0x0A;
    mAccMap[1]["3RW45"] = 0x0B;
    mAccMap[1]["3RW60"] = 0x0C;
    mAccMap[1]["4RW15"] = 0x0D;
    mAccMap[1]["4RW30"] = 0x0E;
    mAccMap[1]["4RW45"] = 0x0F;
    mAccMap[1]["4RW60"] = 0x10;
    mAccMap[1]["未识别"] = 0x11;
    mAccMap[1]["INVALID"] = 0x11;

    mAccMap[2]["无"] = 0x00;
    mAccMap[2]["NONE"] = 0x00;
    mAccMap[2]["EA205"] = 0x01;
    mAccMap[2]["EA210"] = 0x02;
    mAccMap[2]["EA215"] = 0x03;
    mAccMap[2]["EA220"] = 0x04;
    mAccMap[2]["EA225"] = 0x05;
    mAccMap[2]["未识别"] = 0x06;
    mAccMap[2]["INVALID"] = 0x06;
}

void UpperMonitor::CheckTreatFile()
{
    UINT8 beamId;
    BEAMTYPE_T beamType;
    BEAMDATA_ST beamData;

    remote->mFileCheck = TF_COR;

    //verify machine name
    if (g_machine_name != mTreatQueue.machineName)
    {
        qDebug() << g_machine_name << mTreatQueue.machineName << endl;
        qDebug() << "CheckTreatFile:machineName error" << endl;
        InsertInterlock(125, 117, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify beam number
    if (mTreatQueue.beamNumber <= 0)
    {
        qDebug() << "CheckTreatFile:beamNumber error" << endl;
        InsertInterlock(125, 113, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify beam number,beams count, treatseq count
    UINT8 tmp1 = (UINT8)mTreatQueue.treatSequ.size();
    UINT8 tmp2 = (UINT8)mTreatQueue.beamSequ.size();
    if ( (mTreatQueue.beamNumber!=tmp1) || (mTreatQueue.beamNumber!=tmp2) )
    {
        qDebug() << "CheckTreatFile:beamNumber error" << endl;
        InsertInterlock(125, 113, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    for (int i=0; i<mTreatQueue.beamNumber; i++)
    {
        beamId = mTreatQueue.treatSequ[i];
        beamData = mTreatQueue.beamSequ[beamId];
        beamType = GetBeamType(beamId);

        //verify delivered mu
        if (beamData.beamDeliveredMu<0 || beamData.beamDeliveredMu>beamData.finalMeterSet)
        {
            qDebug() << "CheckTreatFile:beamDeliveredMu error" << endl;
            InsertInterlock(125, 114, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
            remote->mFileCheck = TF_VERIFY_ERR;
            return;
        }

        //verify beam type
        if (BT_CONFORMAL == beamType)
        {
            CheckConformalFile(beamId);
        }
        else if (BT_STATIC == beamType)
        {
            CheckStaticFile(beamId);
        }
        else if (BT_DYNAMIC == beamType)
        {
            CheckDynamicFile(beamId);
        }
        else
        {
            remote->mFileCheck = TF_VERIFY_ERR;
            InsertInterlock(125, 93, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
            return;
        }

        if (remote->mFileCheck != TF_COR)
            return;

        DealInterruptedBeam(beamId);
    }
}

void UpperMonitor::CheckConformalFile(UINT8 beamId)
{
    BEAMDATA_ST tmpBeam;
    float tmpx1, tmpx2, tmpy1, tmpy2;

    tmpBeam = mTreatQueue.beamSequ[beamId];

    //verify dos
    if (tmpBeam.finalMeterSet<=0 || tmpBeam.finalMeterSet>=9976)
    {
        qDebug() << "CheckTreatFile:beamMu error" << endl;
        InsertInterlock(125, 95, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify ray type
    if (tmpBeam.rayType == RT_ERR)
    {
        qDebug() << "CheckTreatFile:rayType error" << endl;
        InsertInterlock(125, 94, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify energy
    if (tmpBeam.energy == EG_ERR)
    {
        qDebug() << "CheckTreatFile:energy error" << endl;
        InsertInterlock(125, 97, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify dosrate
    if (tmpBeam.doserate == DR_ERR)
    {
        qDebug() << "CheckTreatFile:doserate error" << endl;
        InsertInterlock(125, 98, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify gantry angle
    if (tmpBeam.gantryAngle<0 || tmpBeam.gantryAngle>360)
    {
        qDebug() << "CheckTreatFile:gantryAngle error" << endl;
        InsertInterlock(125, 100, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify colli angle
    if ( !(tmpBeam.colliAngle>=0 && tmpBeam.colliAngle<=95)
            && !(tmpBeam.colliAngle>=265 && tmpBeam.colliAngle<360) )
    {
        qDebug() << "CheckTreatFile:colliAngle error" << endl;
        InsertInterlock(125, 101, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //control point number
    if (tmpBeam.ctrlPointNumber!=1 || tmpBeam.ctrlPointSequ.size()!=1)
    {
        qDebug() << "CheckTreatFile:ctrlPointNumber error" << endl;
        InsertInterlock(125, 115, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //first control point must set XY
    if (true != tmpBeam.ctrlPointSequ[0].setXY)
    {
        qDebug() << "CheckTreatFile:first ctrlPointSequ no set jaw" << endl;
        InsertInterlock(125, 116, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify x1 x2
    tmpx1 = tmpBeam.ctrlPointSequ[0].x1Pos;
    tmpx2 = tmpBeam.ctrlPointSequ[0].x2Pos;
    if ( tmpx1<-20 || tmpx1>2 || tmpx2>20 || tmpx2<-2 || tmpx2<tmpx1)
    {
        qDebug() << "CheckTreatFile:controlpoint X jaw error" << endl;
        InsertInterlock(125, 106, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify y1 y2
    tmpy1 = tmpBeam.ctrlPointSequ[0].y1Pos;
    tmpy2 = tmpBeam.ctrlPointSequ[0].y2Pos;
    if ( tmpy1<-20 || tmpy1>10 || tmpy2<-10 || tmpy2>20 || tmpy2<tmpy1 )
    {
        qDebug() << "CheckTreatFile:controlpoint Y jaw error" << endl;
        InsertInterlock(125, 108, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }
}

void UpperMonitor::CheckStaticFile(UINT8 beamId)
{
    BEAMDATA_ST tmpBeam = mTreatQueue.beamSequ[beamId];
    bool setXY;
    float tmpx1, tmpx2, tmpy1, tmpy2;

    //verify dos
    if (tmpBeam.finalMeterSet<0 || tmpBeam.finalMeterSet>=9976)
    {
        qDebug() << "CheckTreatFile:ControlPointMU error" << endl;
        InsertInterlock(125, 95, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify ray type
    if (tmpBeam.rayType == RT_ERR)
    {
        qDebug() << "CheckTreatFile:rayType error" << endl;
        InsertInterlock(125, 94, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify energy
    if (tmpBeam.energy == EG_ERR)
    {
        qDebug() << "CheckTreatFile:energy error" << endl;
        InsertInterlock(125, 97, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify doserate
    if (tmpBeam.doserate == DR_ERR)
    {
        qDebug() << "CheckTreatFile:doserate error" << endl;
        InsertInterlock(125, 98, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify gantry angle
    if (tmpBeam.gantryAngle<0 || tmpBeam.gantryAngle>360)
    {
        qDebug() << "CheckTreatFile:gantryAngle error" << endl;
        InsertInterlock(125, 100, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify colli angle
    if ( !(tmpBeam.colliAngle>=0 && tmpBeam.colliAngle<=95)
            && !(tmpBeam.colliAngle>=265 && tmpBeam.colliAngle<360) )
    {
        qDebug() << "CheckTreatFile:colliAngle error" << endl;
        InsertInterlock(125, 101, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //control point number
    if (tmpBeam.ctrlPointNumber<2 || tmpBeam.ctrlPointSequ.size()<2
            || tmpBeam.ctrlPointNumber!=tmpBeam.ctrlPointSequ.size())
    {
        qDebug() << "CheckTreatFile:ctrlPointNumber error" << endl;
        InsertInterlock(125, 115, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //every control point verify
    setXY = false;
    for (int j=0; j<tmpBeam.ctrlPointNumber; j++)
    {
        //verify dos
        if (tmpBeam.ctrlPointSequ[j].meterSet<0 || tmpBeam.ctrlPointSequ[j].meterSet>=9976)
        {
            qDebug() << "CheckTreatFile:controlpointMu error" << endl;
            InsertInterlock(125, 95, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
            remote->mFileCheck = TF_VERIFY_ERR;
            return;
        }

        if (true == tmpBeam.ctrlPointSequ[j].setXY)
        {
            if (0==j || 1==j)
            {
                setXY = true;
            }

            //verify x1 x2
            tmpx1 = tmpBeam.ctrlPointSequ[j].x1Pos;
            tmpx2 = tmpBeam.ctrlPointSequ[j].x2Pos;
            if ( tmpx1<-20 || tmpx1>2 || tmpx2>20 || tmpx2<-2 || tmpx2<tmpx1)
            {
                qDebug() << "CheckTreatFile:controlpoint X jaw error" << endl;
                InsertInterlock(125, 106, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
                remote->mFileCheck = TF_VERIFY_ERR;
                return;
            }

            //verify y1 y2
            tmpy1 = tmpBeam.ctrlPointSequ[j].y1Pos;
            tmpy2 = tmpBeam.ctrlPointSequ[j].y2Pos;
            if ( tmpy1<-20 || tmpy1>10 || tmpy2<-10 || tmpy2>20 || tmpy2<tmpy1 )
            {
                qDebug() << "CheckTreatFile:controlpoint Y jaw error" << endl;
                InsertInterlock(125, 108, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
                remote->mFileCheck = TF_VERIFY_ERR;
                return;
            }
        }
    }
    if ( !setXY )
    {
        qDebug() << "CheckTreatFile:CheckTreatFile:first ctrlPointSequ no set jaw" << endl;
        InsertInterlock(125, 116, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }
}

void UpperMonitor::CheckDynamicFile(UINT8 beamId)
{
    BEAMDATA_ST tmpBeam;
    float tmpx1, tmpx2, tmpy1, tmpy2;

    tmpBeam = mTreatQueue.beamSequ[beamId];
    //verify dos
    if (tmpBeam.finalMeterSet<=0 || tmpBeam.finalMeterSet>=9976)
    {
        qDebug() << "CheckTreatFile:beamMu error" << endl;
        InsertInterlock(125, 95, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify ray type
    if (tmpBeam.rayType == RT_ERR)
    {
        qDebug() << "CheckTreatFile:rayType error" << endl;
        InsertInterlock(125, 94, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify energy
    if (tmpBeam.energy == EG_ERR)
    {
        qDebug() << "CheckTreatFile:energy error" << endl;
        InsertInterlock(125, 97, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify dosrate
    if (tmpBeam.doserate == DR_ERR)
    {
        qDebug() << "CheckTreatFile:doserate error" << endl;
        InsertInterlock(125, 98, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify gantry angle
    if (tmpBeam.gantryAngle<0 || tmpBeam.gantryAngle>360)
    {
        qDebug() << "CheckTreatFile:gantryAngle error" << endl;
        InsertInterlock(125, 100, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify colli angle
    if ( !(tmpBeam.colliAngle>=0 && tmpBeam.colliAngle<=95)
            && !(tmpBeam.colliAngle>=265 && tmpBeam.colliAngle<360) )
    {
        qDebug() << "CheckTreatFile:colliAngle error" << endl;
        InsertInterlock(125, 101, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify x1 x2
    tmpx1 = tmpBeam.x1Pos;
    tmpx2 = tmpBeam.x2Pos;
    if ( tmpx1<-20 || tmpx1>2 || tmpx2>20 || tmpx2<-2 || tmpx2<tmpx1)
    {
        qDebug() << "CheckTreatFile:beam X jaw error" << endl;
        InsertInterlock(125, 106, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }

    //verify y1 y2
    tmpy1 = tmpBeam.y1Pos;
    tmpy2 = tmpBeam.y2Pos;
    if ( tmpy1<-20 || tmpy1>10 || tmpy2<-10 || tmpy2>20 || tmpy2<tmpy1 )
    {
        qDebug() << "CheckTreatFile:beam Y jaw error" << endl;
        InsertInterlock(125, 108, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        remote->mFileCheck = TF_VERIFY_ERR;
        return;
    }
}

INT32 UpperMonitor::ConvertFloatToInt(float val, UINT8 mul)
{
    return round(val*mul);
}

bool UpperMonitor::AnalysRevFram(TransRecvT& revdata)
{
    if (!ProcessCHKIL(revdata))
        return false;

    switch(revdata.data[1])
    {
    case 'F':
        RevUpperConnectAck(revdata);
        break;
    case 'N':
        RevDownloadFile(revdata);
        break;
    case 'C':
        RevResetEcho(revdata);
        break;
    case 'A':
        RevAcceptEcho(revdata);
        break;
    case 'M':
        RevCtrlpointPosition(revdata);
        break;
    case 'E':
        RevErrorReport(revdata);
        break;
    case 'H':
        RevHeartbeat(revdata);
        break;
    case 'R':
        RevResume(revdata);
        break;
    default:
        InsertInterlock(79, 959, ERROR_IL);     //invalid command type
        break;
    }

    return false;
}

bool UpperMonitor::RevDownloadFile(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

    if ( g_is_remote_mode && PROGRAM==g_status )
    {
        bool res = false;
        emit remote->resetProgress();

        if (!g_treatqueue_mutex.tryLock())
        {
            qDebug() << "treatqueue is using" << endl;
            return false;
        }

#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "download data" << endl;
#endif
        remote->DownloadDataInit();

        //QString filename = mFtpDonwloader.DownloadByUrl("ftp://192.168.1.91/treatment.xml", "", "", 21);
        res = mParser.ReadFromXml(mTreatQueue, remote->mFileCheck);

        if ( res )
        {
            CheckTreatFile();
            if (TF_COR != remote->mFileCheck)
            {
                qDebug() << "treatfile error" << endl;
                g_treatqueue_mutex.unlock();
                return false;
            }
            else
            {
                remote->mTotalSegNum = CntSegFromCur();
            }
        }
        else
        {
            remote->mFileCheck = TF_OPEN_ERR;
        }

        remote->mRemoteParam.treatdataGet = true;
        g_treatqueue_mutex.unlock();
        return true;
    }

    return false;
}

bool UpperMonitor::RevUpperConnectAck(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

    if (g_is_remote_mode && !remote->mRemoteParam.upperConnect)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "RecvUpperConnectAck" << endl;
#endif
        remote->mRemoteParam.upperConnect = true;
        emit base->g_upcon_stop_signal();
        //DeleteInterlock(124, 109);   need delete?
        g_maincontrol_wait.wakeOne();

        return true;
    }

    return false;
}

bool UpperMonitor::RevResetEcho(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

    if (g_is_remote_mode)
    {
        if (revdata.data[2] != 0)
        {
            return false;
        }

        g_c0_echo_bits_lock.lockForWrite();
        g_c0_echo_bits.setBit(9, true);
        g_c0_echo_bits_lock.unlock();

        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool UpperMonitor::RevAcceptEcho(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9
        return false;
    }

    if (g_is_remote_mode && remote->mUpperAcceptEcho==EUR_NOCHECK)
    {
        remote->mUpperAcceptEcho = (ERROR_UPPER_REPORT_T)revdata.data[2];
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "UpperMonitor::RevAcceptEcho" << remote->mUpperAcceptEcho << endl;
#endif

        if (remote->mUpperAcceptEcho != EUR_NOERR)
        {
            ProcessErrorReportIL(remote->mUpperAcceptEcho);
        }
        g_maincontrol_wait.wakeOne();

        return true;
    }

    return false;
}

bool UpperMonitor::RevErrorReport(TransRecvT& revdata)
{
    if (revdata.len != 5)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

    if (g_is_remote_mode)
    {
        //insert interlock
        ERROR_UPPER_REPORT_T error_now = (ERROR_UPPER_REPORT_T)revdata.data[2];
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "UpperMonitor::RevErrorReport" << error_now << endl;
#endif
        ProcessErrorReportIL(error_now);
        g_maincontrol_wait.wakeOne();
        return true;
    }

    return false;
}

bool UpperMonitor::RevHeartbeat(TransRecvT& revdata)
{
    if (revdata.len != 4)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "RevHeartbeat:upper " << mRevFrameNum << QTime::currentTime().toString("hh:mm:ss.zzz") << endl;
#endif

    if (g_is_remote_mode && remote->mIsUpperWork)
    {
        upper->mIsHeartbeatGet = true;
        return true;
    }

    return false;
}

bool UpperMonitor::RevResume(TransRecvT& revdata)
{
    if (revdata.len != 8)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9 .
        return false;
    }

    if (g_is_remote_mode)
    {
        if (PROGRAM == g_status)
        {
            g_resume_info.isGet = true;
            g_resume_info.beamId = revdata.data[2];
            g_resume_info.ctrlpointId = revdata.data[3];
            g_resume_info.deliveredMu = (revdata.data[4] & 0x00FF) | ((revdata.data[5] << 8) & 0xFF00);
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "RevResume=>" << g_resume_info.beamId << "," << g_resume_info.ctrlpointId << ","
                     << g_resume_info.deliveredMu << endl;
#endif

            upper->SendResumeEcho(RE_SUCCESS);
        }
        else
        {
            upper->SendResumeEcho(RE_FAIL);
        }

        return true;
    }

    return false;
}

bool UpperMonitor::RevCtrlpointPosition(TransRecvT &revdata)
{
    if (revdata.len != 7)  //判断长度是否符合要求
    {
        InsertInterlock(79, 969, ERROR_IL);   //Invalid length for command type from controller 9
        return false;
    }

    if (g_is_remote_mode)
    {
        beamPos = revdata.data[2];
        ctrlpointPos = (revdata.data[3] & 0x00FF) | ((revdata.data[4] << 8) & 0xFF00);
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "UpperMonitor::RevCtrlpointPosition" << beamPos << ctrlpointPos << endl;
#endif

        if (g_status==PROGRAM || g_status==NOT_READY || g_status==READYS)
        {
            mlc->beamPos = beamPos;
            mlc->ctrlpointPos = ctrlpointPos;
        }
        else if (g_status == RAD_ON)
        {
            remote->CheckCtrlpointPostion();
            g_maincontrol_wait.wakeOne();
        }

        return true;
    }

    return false;
}

void UpperMonitor::ProcessErrorReportIL(ERROR_UPPER_REPORT_T type)
{
    switch (type)
    {
    case EUR_CANCEL:
        break;
    case EUR_BEAM_ID:
        InsertInterlock(125, 91, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_CONTROLPOINT_ID:
        InsertInterlock(125, 92, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_BEAM_TYPE:
        InsertInterlock(125, 93, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_RADIATION_TYPE:
        InsertInterlock(125, 94, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_BEAM_MU:
        InsertInterlock(125, 95, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_CONTROLPOINT_MU:
        InsertInterlock(125, 96, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_ENERGY:
        InsertInterlock(125, 97, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_DOSERATE:
        InsertInterlock(125, 98, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_DOSE:
        InsertInterlock(125, 99, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_GANTRY_ANGLE:
        InsertInterlock(125, 100, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_COLLIMATOR_ANGLE:
        InsertInterlock(125, 101, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_SLOT1:
        InsertInterlock(125, 102, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_SLOT2:
        InsertInterlock(125, 103, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_SLOT3:
        InsertInterlock(125, 104, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_TECH_TYPE:
        InsertInterlock(125, 105, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_JAW_X1:
        InsertInterlock(125, 106, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_JAW_X2:
        InsertInterlock(125, 107, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_JAW_Y1:
        InsertInterlock(125, 108, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_JAW_Y2:
        InsertInterlock(125, 109, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_MODE:
        InsertInterlock(125, 110, ERROR_IL, IL_READY|IL_PROGRAM, NOT_LATCHED);
        break;
    case EUR_HEARTBEAT:
        InsertInterlockNoErr(111, ERROR_IL, IL_ALL, LATCHED);
        break;
    case EUR_INVALID_FRAME:
        InsertInterlock(79, 939, ERROR_IL);
        break;
    case EUR_INVALID_CHECKSUM:
        InsertInterlock(79, 949, ERROR_IL);
        break;
    default:
        break;
    }
}

bool UpperMonitor::ProcessCHKIL(TransRecvT& revdata)
{
    UINT8 fc;
    if ((ComputeLstCHK(revdata.data, revdata.len - 1) & 0x7F) !=
            (revdata.data[revdata.len - 1] & 0x7F))
        return false;

    //heartbeat not check frame id
    if (revdata.data[1] == 0x48)
        return true;

    fc = ComputeRevCHK(revdata.data, revdata.len - 2);
    if (fc != revdata.data[revdata.len - 2])
    {
        if ((fc & 0xF0) != (revdata.data[revdata.len - 2] & 0xF0))
            InsertInterlock(79, 939, ERROR_IL);       // Invalid frame number detected from controller 9 安全联锁
        if ((fc & 0x0F) !=  (revdata.data[revdata.len - 2] & 0x0F))
            InsertInterlock(79, 949, ERROR_IL);       //Invalid checksum detected from controller 9 安全联锁

        return false;
    }
    return true;
}

//计算倒数第2位校验和，高4位表示序号，低4位异或得到
bool UpperMonitor::ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc)
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

UINT8 UpperMonitor::ComputeRevCHK(UINT8* data, UINT8 len)
{
    UINT8 fc;
    UINT8 low = 0, tmplow = 0;

    // mRevFrameNum_mutex.lock();
    fc = mRevFrameNum << 4;
    mRevFrameNum = (mRevFrameNum + 1) % 16;
    // mRevFrameNum_mutex.unlock();

    low = data[0] ^ len;
    for (int i = 1; i < len; i++)
        low = low ^ data[i];

    tmplow = (low & 0x0F) ^ ((low & 0xF0) >> 4);
    fc += tmplow;

    return fc;
}

bool UpperMonitor::HasNextBeam()
{
    if (mTreatQueue.curBeamPosIdx+1 < mTreatQueue.beamNumber)
    {
        return true;
    }

    return false;
}

bool UpperMonitor::HasNextData()
{
    //QMutexLocker locker(&mQueueMutex);
    UINT8 beamId,ctrlPointId;
    BEAMDATA_ST beamData;

    if (BT_STATIC == GetBeamType())  //静态调强
    {
        beamId = GetCurBeamId();
        beamData = mTreatQueue.beamSequ[beamId];
        ctrlPointId = beamData.curCtrlPointIdx;
        if (ctrlPointId+1 < beamData.ctrlPointNumber)
        {
            return true;
        }
        else
        {
            return HasNextBeam();
        }
    }
    else
    {
        return HasNextBeam();
    }

    return false;
}

void UpperMonitor::NextData()
{
    //QMutexLocker locker(&mQueueMutex);
    UINT8 beamId,ctrlPointId;
    BEAMDATA_ST beamData;

    if (BT_STATIC == GetBeamType())
    {
        beamId = GetCurBeamId();
        beamData = mTreatQueue.beamSequ[beamId];
        ctrlPointId = beamData.curCtrlPointIdx;

        if (ctrlPointId+1 < beamData.ctrlPointNumber)
        {
            mTreatQueue.beamSequ[beamId].curCtrlPointIdx += 1;
        }
        else
        {
            mTreatQueue.curBeamPosIdx++;
        }
    }
    else
    {
        mTreatQueue.curBeamPosIdx++;
    }
}

void UpperMonitor::GetCurBeamData(BEAMDATA_ST &beamData)
{
    //QMutexLocker locker(&mQueueMutex);
    UINT8 beamId = GetCurBeamId();
    beamData = mTreatQueue.beamSequ[beamId];
}

UINT16 UpperMonitor::GetCurBeamDeltaMu()
{
    UINT16 beamId = GetCurBeamId();
    if (BT_DYNAMIC == GetBeamType(beamId))
    {
        return mTreatQueue.beamSequ[beamId].beamDeliveredMu;
    }
    else
    {
        return mTreatQueue.beamSequ[beamId].interruptedControlPointDeliveredMU;
    }
}

void UpperMonitor::GetCurCtrlPointData(CONTROLPOINT_ST &ctrlPointData)
{
    //QMutexLocker locker(&mQueueMutex);
    UINT8 beamId = GetCurBeamId();
    ctrlPointData = mTreatQueue.beamSequ[beamId].ctrlPointSequ[mTreatQueue.beamSequ[beamId].curCtrlPointIdx];
}

bool UpperMonitor::IsFirstBeam(UINT8 id)
{
    //QMutexLocker locker(&mQueueMutex);
    if (id == mTreatQueue.treatSequ[0])
    {
        return true;
    }

    return false;
}

UINT8 UpperMonitor::GetCurBeamId()
{
    //QMutexLocker locker(&mQueueMutex);
    return mTreatQueue.treatSequ[mTreatQueue.curBeamPosIdx];
}

UINT8 UpperMonitor::GetCurCtrlPointIdx()
{
    UINT8 beamId = GetCurBeamId();
    return mTreatQueue.beamSequ[beamId].curCtrlPointIdx;
}

UINT8 UpperMonitor::GetCurCtrlPointId()
{
    UINT8 beamId = GetCurBeamId();
    UINT8 idx = GetCurCtrlPointIdx();
    BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
    CONTROLPOINT_ST ctrlData = beamData.ctrlPointSequ[idx];
    return ctrlData.ctrlPointIdx;
}

BEAMTYPE_T UpperMonitor::GetBeamType(INT8 beamId)
{
    if (beamId <= -1)           //当前的beamid
    {
        UINT8 beamId = GetCurBeamId();
        BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
        return beamData.beamType;
    }
    else
    {
        return mTreatQueue.beamSequ[beamId].beamType;
    }
}

bool UpperMonitor::GetPrevCtrlPoint(CONTROLPOINT_ST &ctrlPointData)
{
    UINT8 beamId = GetCurBeamId();
    BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
    UINT8 curCtrlPointId = beamData.curCtrlPointIdx;
    curCtrlPointId--;

    while (curCtrlPointId >= 0)
    {
        if (true == beamData.ctrlPointSequ[curCtrlPointId].setXY)
        {
            ctrlPointData = beamData.ctrlPointSequ[curCtrlPointId];
            return true;
        }

        curCtrlPointId--;
    }

    return false;
}

bool UpperMonitor::SendResumeEcho(RESUME_ECHO_T value)
{
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOUPPER;
    tmpSend.data[1]='A';
    tmpSend.data[2]=value;
    qDebug() << "upper:SendResumeAck:" << value << endl;

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);

    tmpSend.len = 5;
    return InsertRemoteOutQueue(tmpSend);
}

//处理完成后中断的beam可以和正常beam进行同样的后续处理
void UpperMonitor::DealInterruptedBeam(UINT8 beamId)
{
    //QMutexLocker locker(&mQueueMutex);
    if (BT_CONFORMAL == GetBeamType(beamId))  //适型
    {
        mTreatQueue.beamSequ[beamId].ctrlPointSequ[0].meterSet -= mTreatQueue.beamSequ[beamId].interruptedControlPointDeliveredMU;
    }
    else if (BT_STATIC == GetBeamType(beamId))     //静态调强
    {
        UINT16 ctrlpointId = 0;
        BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
        for (int i=0; i<beamData.ctrlPointNumber; i++)
        {
            if (beamData.ctrlPointSequ[i].ctrlPointIdx == mTreatQueue.beamSequ[beamId].interruptedControlPointId)
            {
                ctrlpointId = i;
                break;
            }
        }
        mTreatQueue.beamSequ[beamId].ctrlPointSequ[ctrlpointId].meterSet -= mTreatQueue.beamSequ[beamId].interruptedControlPointDeliveredMU;
        mTreatQueue.beamSequ[beamId].curCtrlPointIdx = ctrlpointId;
    }
    else    //动态调强
    {
        mTreatQueue.beamSequ[beamId].finalMeterSet -= mTreatQueue.beamSequ[beamId].beamDeliveredMu;
    }
}

bool UpperMonitor::IsRadonComplete()
{
    UINT16 totalDose = 0;
    if (GetBeamType() == BT_DYNAMIC)
    {
        BEAMDATA_ST beamData;
        GetCurBeamData(beamData);
        totalDose = beamData.finalMeterSet;
    }
    else
    {
        CONTROLPOINT_ST ctrlpointData;
        GetCurCtrlPointData(ctrlpointData);
        totalDose = ctrlpointData.meterSet;
    }

    //判断阀值
    if (mActDose < totalDose*DELIVERED_DOSE_PRECISION)
        return false;

    g_is_complete = true;
    return true;
}

bool UpperMonitor::ResumeTreat()
{
    for (int i=0; i<mTreatQueue.treatSequ.size(); i++)
    {
        if (mTreatQueue.treatSequ[i] == g_resume_info.beamId)
        {
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "ResumeTreat:curBeamPosIdx" << i << endl;
#endif
            mTreatQueue.curBeamPosIdx = i;
            break;
        }
    }

    if (BT_DYNAMIC == GetBeamType(g_resume_info.beamId))
    {
        UINT8 beamId = GetCurBeamId();
        remote->mTotalSegNum = CntSegFromCur();
        remote->mCurSegId = 1;
        mTreatQueue.beamSequ[beamId].finalMeterSet -= g_resume_info.deliveredMu;
        mTreatQueue.beamSequ[beamId].beamDeliveredMu += g_resume_info.deliveredMu;
    }
    else
    {
        BEAMDATA_ST beamData = mTreatQueue.beamSequ[g_resume_info.beamId];
        for (int i=0; i<beamData.ctrlPointNumber; i++)
        {
            if (beamData.ctrlPointSequ[i].ctrlPointIdx == g_resume_info.ctrlpointId)
            {
#ifdef PRINT_REMOTE_DEBUG
                qDebug() << "ResumeTreat:curCtrlPointIdx" << i << endl;
#endif
                mTreatQueue.beamSequ[g_resume_info.beamId].curCtrlPointIdx = i;
                mTreatQueue.beamSequ[g_resume_info.beamId].ctrlPointSequ[i].meterSet -= g_resume_info.deliveredMu;
                mTreatQueue.beamSequ[g_resume_info.beamId].interruptedControlPointDeliveredMU += g_resume_info.deliveredMu;
                break;
            }
        }

        //SetCurBeamFirstPoint();
        remote->mTotalSegNum = CntSegFromCur();
        remote->mCurSegId = 1;
    }

    return true;
}

UINT16 UpperMonitor::CntSegFromCur()
{
    int segNum = 0;
    UINT8 beamId;

    for (int beamIdx=mTreatQueue.curBeamPosIdx; beamIdx<mTreatQueue.beamNumber; beamIdx++)
    {
        beamId = mTreatQueue.treatSequ[beamIdx];
        if (BT_DYNAMIC==GetBeamType(beamId) || BT_CONFORMAL==GetBeamType(beamId))
        {
            segNum++;
        }
        else
        {
            BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
            for (int ctrlpointIdx=beamData.curCtrlPointIdx; ctrlpointIdx<beamData.ctrlPointNumber; ctrlpointIdx++)
            {
                segNum++;
            }
        }
    }

    return segNum;
}

void UpperMonitor::ClearFrameNumber()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper set frame 0" << endl;
#endif
    mSendFrameNum = 0;
    mRevFrameNum = 0;
}

void UpperMonitor::CheckAcceptData()
{
    INT32 tmp1, tmp2;

    //doserate
    if (setedDosrate != curBeamData.doserate)
    {
        ProcessErrorReportIL(EUR_DOSERATE);
        return;
    }

    //energy
    if (setedEnergy != curBeamData.energy)
    {
        ProcessErrorReportIL(EUR_ENERGY);
        return;
    }

    //tech

    //ray type

    //dose
    tmp1 = setedMeterSet;
    tmp2 = curBeamData.finalMeterSet;
    if (tmp1 != tmp2)
    {
        ProcessErrorReportIL(EUR_DOSE);
        return;
    }

    //gantry
    tmp1 = ConvertFloatToInt(setedGantry, 10);
    tmp2 = ConvertFloatToInt(curBeamData.gantryAngle, 10);
    //qDebug() << "check gantry:" << tmp1 << tmp2 << endl;
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_GANTRY_ANGLE);
        return;
    }

    //colli
    tmp1 = ConvertFloatToInt(setedColli, 10);
    tmp2 = ConvertFloatToInt(curBeamData.colliAngle, 10);
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_COLLIMATOR_ANGLE);
        return;
    }

    //x1
    tmp1 = ConvertFloatToInt(setedX1Pos, 10);
    tmp2 = ConvertFloatToInt(curBeamData.x1Pos, 10);
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_JAW_X1);
        return;
    }

    //x2
    tmp1 = ConvertFloatToInt(setedX2Pos, 10);
    tmp2 = ConvertFloatToInt(curBeamData.x2Pos, 10);
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_JAW_X2);
        return;
    }

    //y1
    tmp1 = ConvertFloatToInt(setedY1Pos, 10);
    tmp2 = ConvertFloatToInt(curBeamData.y1Pos, 10);
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_JAW_Y1);
        return;
    }

    //y2
    tmp1 = ConvertFloatToInt(setedY2Pos, 10);
    tmp2 = ConvertFloatToInt(curBeamData.y2Pos, 10);
    if (abs(tmp1-tmp2) > INACCURACY)
    {
        ProcessErrorReportIL(EUR_JAW_Y2);
        return;
    }

    //acc1
    qDebug() << "setedAcc1" << setedAcc1 << "curBeamData.acc1" << curBeamData.acc1 << endl;
    if (setedAcc1 != curBeamData.acc1)
    {
        ProcessErrorReportIL(EUR_SLOT1);
        return;
    }

    //acc2
    qDebug() << "setedAcc2" << setedAcc2 << "curBeamData.acc2" << curBeamData.acc2 << endl;
    if (setedAcc2 != curBeamData.acc2)
    {
        ProcessErrorReportIL(EUR_SLOT2);
        return;
    }

    //acc3
    qDebug() << "setedAcc3" << setedAcc3 << "curBeamData.acc3" << curBeamData.acc3 << endl;
    if (setedAcc3 != curBeamData.acc3)
    {
        ProcessErrorReportIL(EUR_SLOT3);
        return;
    }
}

UINT8 UpperMonitor::getBeamNumber()
{
    return mTreatQueue.beamNumber;
}

void UpperMonitor::SetCurBeamFirstPoint()
{
    UINT8 beamId = GetCurBeamId();
    BEAMDATA_ST beamData = mTreatQueue.beamSequ[beamId];
    UINT8 ctrlPointIdx = beamData.curCtrlPointIdx;
    UINT16 curDos = beamData.ctrlPointSequ[ctrlPointIdx].meterSet;

    while (curDos == beamData.ctrlPointSequ[ctrlPointIdx].meterSet)
    {
        ctrlPointIdx++;
    }

    g_next_total_dos = beamData.ctrlPointSequ[ctrlPointIdx].meterSet;
    mTreatQueue.beamSequ[beamId].curCtrlPointIdx = ctrlPointIdx-1;
}

bool UpperMonitor::SendConnectReq()
{
    upper->ClearFrameNumber();
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendConnectReq" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOUPPER;
    tmpSend.data[1]='F';
    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3]=ComputeLstCHK(&tmpSend.data[0],3);

    tmpSend.len = 4;
    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendFileChkResult()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendFileChkResult" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOUPPER;
    tmpSend.data[1]='S';
    tmpSend.data[2]=remote->mFileCheck;
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:mFileCheckValue:" << remote->mFileCheck << endl;
#endif

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4]=ComputeLstCHK(&tmpSend.data[0],4);

    tmpSend.len = 5;
    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendAcceptOther()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendAcceptOther" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend,0,sizeof(TransRecvT));
    tmpSend.data[0]=TOUPPER;
    tmpSend.data[1]='I';
    tmpSend.data[2]=GetCurBeamId();

    //byte3:mode-0-beam type-Doserate-energy-0-0-raytype
    tmpSend.data[3]=0;
    if (setedEnergy == EG_LOW)
    {
        tmpSend.data[3] |= ((0<<3)&0x08);
    }
    else
    {
        tmpSend.data[3] |= ((1<<3)&0x08);
    }
    if (setedTech == TECH_FIX)
    {
        tmpSend.data[3] |= ((0<<1)&0x08);
    }
    else
    {
        tmpSend.data[3] |= ((1<<1)&0x08);
    }
    if (setedDosrate == DR_LOW)
    {
        tmpSend.data[3] |= ((0<<4)&0x10);
    }
    else
    {
        tmpSend.data[3] |= ((1<<4)&0x10);
    }

    BEAMTYPE_T beamType = GetBeamType();
    if (beamType==BT_CONFORMAL || beamType==BT_STATIC)
        tmpSend.data[3] |= (0<<5);
    else
        tmpSend.data[3] |= (1<<5);

    if (SERVICE_MODE == g_usermode)
    {
        tmpSend.data[3] |= (1<<7);
    }

    tmpSend.data[4] = (UINT8)(setedMeterSet & 0xff);
    tmpSend.data[5] = (UINT8)(setedMeterSet >> 8);

#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "setedMeterSet:" << setedMeterSet << tmpSend.data[4] << tmpSend.data[5] << endl;
#endif

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;
    tmpSend.data[7]=ComputeLstCHK(&tmpSend.data[0],7);

    tmpSend.len = 8;
    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendReadyStatus(const READYSTATE_TYPE_T &status)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendReadyStatus:" << status << endl;
#endif
    TransRecvT tmpSend;

    if (status != NOTREADY && status != READY && status != INTERRUPTREDY
            && status != PAUSE && status != INTERRUPTNOTREADY)
        return false;

    // ready_status_now = status;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'Z';     //Z
    tmpSend.data[2] = status;

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;

    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendRadOn(void)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendRadOn" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'R';     //R

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendInterrupt()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendInterrupt" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'O';

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendTerminateOrComplete(bool terminate)
{
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'T';     //T
    if (terminate)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "upper:SendTerminate" << endl;
#endif
        tmpSend.data[2] = 1;    //terminate
    }
    else
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "upper:SendComplete" << endl;
#endif
        tmpSend.data[2] = 0;    //complete
    }

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendTreatEnd()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendTreatEnd" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'X';     //'X'

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendUserMode(const USERMODE_TYPE_T mode)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendUserMode:" << mode << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof (tmpSend));
    if (mode > SERVICE_MODE || mode < RESET_MODE)
        return false;

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'C';     //C
    tmpSend.data[2] = mode;

    QMutexLocker locker(&mUpperMutex);

    if (!ComputeSendCHK(&tmpSend.data[0], 3, tmpSend.data[3]))
        return false;
    tmpSend.data[4] = ComputeLstCHK(&tmpSend.data[0], 4);
    tmpSend.len = 5;       //fixme

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendPgmReach()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendPgmReach" << endl;
#endif
    TransRecvT tmpSend;

    memset(&tmpSend, 0, sizeof(tmpSend));
    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'P';     //'P'

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;
    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendGantryPos()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    UINT16 tmp;

    tmp = round(setedGantry*10);
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendGantryPos" << tmp << endl;
#endif

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '4';
    tmpSend.data[2] = (UINT8)(tmp & 0xff);
    tmpSend.data[3] = (UINT8)(tmp >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendColliPos()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));
    UINT16 tmp;

    tmp = round(setedColli*10);
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendColliPos" << tmp << endl;
#endif

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '3';
    tmpSend.data[2] = (UINT8)(tmp & 0xff);
    tmpSend.data[3] = (UINT8)(tmp >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendX1X2Pos()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    INT16 tmpx1, tmpx2;
    tmpx1 = round(setedX1Pos*10);
    tmpx2 = round(setedX2Pos*10);
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendX1X2Pos:" << tmpx1 << tmpx2 << endl;
#endif

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '1';
    tmpSend.data[2] = (UINT8)(tmpx1 & 0xff);
    tmpSend.data[3] = (UINT8)(tmpx1 >> 8);
    tmpSend.data[4] = (UINT8)(tmpx2 & 0xff);
    tmpSend.data[5] = (UINT8)(tmpx2 >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;

    tmpSend.data[7] = ComputeLstCHK(&tmpSend.data[0], 7);
    tmpSend.len = 8;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendY1Y2Pos()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    INT16 tmpy1, tmpy2;
    tmpy1 = round(setedY1Pos*10);
    tmpy2 = round(setedY2Pos*10);
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendY1Y2Pos:" << tmpy1 << tmpy2 << endl;
#endif

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '2';
    tmpSend.data[2] = (UINT8)(tmpy1 & 0xff);
    tmpSend.data[3] = (UINT8)(tmpy1 >> 8);
    tmpSend.data[4] = (UINT8)(tmpy2 & 0xff);
    tmpSend.data[5] = (UINT8)(tmpy2 >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 6, tmpSend.data[6]))
        return false;

    tmpSend.data[7] = ComputeLstCHK(&tmpSend.data[0], 7);
    tmpSend.len = 8;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendAcc12Value()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendAcc12Value" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '5';

    tmpSend.data[2] = mAccMap[0][setedAcc1];
    tmpSend.data[3] = mAccMap[1][setedAcc2];

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendAcc3CtrlpointValue()
{
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '6';

    tmpSend.data[2] = mAccMap[2][setedAcc3];
    if (GetBeamType() == BT_DYNAMIC)
        tmpSend.data[3] = 1;
    else
        tmpSend.data[3] = GetCurCtrlPointId();

    #ifdef PRINT_REMOTE_DEBUG
        qDebug() << "upper:SendAcc3Value" << endl;
    #endif

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendActDos()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendActDos" << mSendFrameNum << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '7';
    tmpSend.data[2] = (UINT8)(mActDose & 0xff);
    tmpSend.data[3] = (UINT8)(mActDose >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendLastDos()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendLastDos" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = '8';
    tmpSend.data[2] = (UINT8)(mLastDose & 0xff);
    tmpSend.data[3] = (UINT8)(mLastDose >> 8);

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 4, tmpSend.data[4]))
        return false;

    tmpSend.data[5] = ComputeLstCHK(&tmpSend.data[0], 5);
    tmpSend.len = 6;

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendHeartbeat()
{
#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "upper:SendHeartbeat" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'H';

    QMutexLocker locker(&mUpperMutex);
    //heartbeat not add frame id
    /*if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;*/

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

#ifdef PRINT_REMOTE_DEBUG
    //qDebug() << "insert upper heartbeat" << QTime::currentTime().toString("hh:mm:ss.zzz") << endl;
#endif

    return InsertRemoteOutQueue(tmpSend);
}

bool UpperMonitor::SendUnconnect()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendUnconnect" << endl;
#endif
    TransRecvT tmpSend;
    memset(&tmpSend, 0, sizeof (tmpSend));

    tmpSend.data[0] = TOUPPER;
    tmpSend.data[1] = 'U';

    QMutexLocker locker(&mUpperMutex);
    if (!ComputeSendCHK(&tmpSend.data[0], 2, tmpSend.data[2]))
        return false;

    tmpSend.data[3] = ComputeLstCHK(&tmpSend.data[0], 3);
    tmpSend.len = 4;

    return InsertRemoteOutQueue(tmpSend);
}

void UpperMonitor::SetActDose(UINT32 actDose)
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
        SetLastDose(actDose);
    }
}

void UpperMonitor::SetLastDose(UINT32 lastDose)
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "SetLastDose:" << lastDose << endl;
#endif
    mLastDose = lastDose;
}

void UpperMonitor::SetBeamJawByCtrlPoint(BEAMDATA_ST &beamData, CONTROLPOINT_ST ctrlPoint)
{
    beamData.x1Pos = ctrlPoint.x1Pos;
    beamData.x2Pos = ctrlPoint.x2Pos;
    beamData.y1Pos = ctrlPoint.y1Pos;
    beamData.y2Pos = ctrlPoint.y2Pos;
}

bool UpperMonitor::SendAccept()
{
#ifdef PRINT_REMOTE_DEBUG
    qDebug() << "upper:SendAccept" << endl;
#endif
    SendAcceptOther();
    SendX1X2Pos();
    SendY1Y2Pos();
    SendColliPos();
    SendGantryPos();
    SendAcc12Value();
    SendAcc3CtrlpointValue();

    return true;
}
