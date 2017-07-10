#include "remotecontroller.h"
#include "mastercontroller.h"
#include "../ui/accui.h"
#include <windows.h>
#include "defines.h"

extern enum Status g_status;
extern SlaveBase *base;
extern UpperMonitor *upper;
extern Mlc *mlc;
extern TREAT_RESUME_ST g_resume_info;
extern AccUi *accui;
extern QMutex g_treatqueue_mutex;

bool g_is_remote_mode = false;

RemoteController::RemoteController()
{
    isSettingData = false;
    mIsUpperWork = false;
    mIsMlcWork = false;
    isMlcMoveStop = true;
    mCurSegId = 1;
    mTotalSegNum = 0;
    mRemoteStatus = INIT;
    mFileCheck = NOCHECK;
    mUpperAcceptEcho = EUR_NOCHECK;
    isSendBeam = false;
    hasSetdata = false;
    mMlcMoveToBeamEcho = MMBE_NOCHECK;

    mRemoteParam.mlcConnect = false;
    mRemoteParam.upperConnect = false;
    mRemoteParam.mlcdataFeed = false;
    mRemoteParam.treatdataGet = false;
    mRemoteParam.treatdataSet = false;
}

void RemoteController::DownloadDataInit()
{
    ResetSetting(WAIT_TREATDATA);

    //Deleteinterlock   ???
}

void RemoteController::ExitRemoteMode()
{
    /*emit base->g_upcon_stop_signal();
    emit base->g_mlccon_stop_signal();*/

    emit this->setRemoteMsg("");

    StopHeartBeat();

    DeleteRemoteInterlock();
}

void RemoteController::DeleteRemoteInterlock()
{
    //upper->DeleteInterlock(126, 100);   //mUpperAcceptEcho
    upper->DeleteInterlock(126, 109);   //heartbeat

    //还有一些mlc interlock
    mlc->DeleteInterlock(81, 101);     //mlcMovstop
    mlc->DeleteInterlock(81, 102);     //mlc move exception
    mlc->DeleteInterlock(124);
    mlc->DeleteInterlock(81, 108);   //heartbeat
    mlc->DeleteInterlock(81);
}

bool RemoteController::SendUnconnect()
{
    upper->SendUnconnect();
    mlc->SendUnconnect();
}

bool RemoteController::StartHeatBeat()
{
    remote->mIsUpperWork = true;
    remote->mIsMlcWork = true;
    upper->mIsHeartbeatGet = false;
    mlc->mIsHeartbeatGet = false;
    remote->SendHeartBeat();
    emit base->g_heartbeat_sender_start();
    emit base->g_heartbeat_receiver_start();
}

bool RemoteController::SendHeartBeat()
{
    if (mIsUpperWork)
    {
        upper->SendHeartbeat();
    }

    if (mIsMlcWork)
    {
        mlc->SendHeartbeat();
    }
}

bool RemoteController::StopHeartBeat()
{
    mIsUpperWork = false;
    mIsMlcWork = false;

    emit base->HeartbeatSenderStop();
    emit base->HeartbeatReceiverStop();
}

bool RemoteController::CheckCtrlpointPostion()
{
    return true;

    if (upper->GetBeamType() != BT_DYNAMIC)
        return true;

    if (mlc->beamPos != upper->beamPos)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "CheckCtrlpointPostion:beam->" << mlc->beamPos << upper->beamPos << endl;
#endif
        mlc->InsertInterlock(81, 102, ERROR_IL, IL_RADON, NOT_LATCHED);
        return false;
    }

    if (abs(mlc->ctrlpointPos-upper->ctrlpointPos) > BEAM_POSITION_DELTA)
    {
#ifdef PRINT_REMOTE_DEBUG
        qDebug() << "CheckCtrlpointPostion:ctrlpoint->" << mlc->ctrlpointPos << upper->ctrlpointPos << endl;
#endif
        mlc->InsertInterlock(81, 102, ERROR_IL, IL_RADON, NOT_LATCHED);
        return false;
    }

    return true;
}

/*
 * 当返回true时，表示mastercontroller状态机可以响应ACCEPT按键
*/
bool RemoteController::RemoteStateMachine()
{
    switch(mRemoteStatus)
    {
    case REMOTE_ERROR:
        break;
    case INIT:
        qDebug() << "RemoteController: WAIT_CONNECT" << endl;
        emit this->setRemoteMsg(MSG_REMOTE_CANCEL);
        mRemoteStatus = WAIT_CONNECT;
        break;
    case WAIT_CONNECT:
        if (mRemoteParam.upperConnect && mRemoteParam.mlcConnect)
        {
            qDebug() << "RemoteController: CONNECT" << endl;
            emit showRemoteControl(true);
            emit resetProgress();

            mlc->SendRadonDelayTime();

            mRemoteStatus = WAIT_TREATDATA;
        }
        break;
    case WAIT_TREATDATA:
        if ( mRemoteParam.treatdataGet )
        {
            if (TF_COR == mFileCheck)
            {
                mlc->SendFileReach();
                qDebug() << "RemoteController: FILE_CHECK" << endl;
                mRemoteStatus = FILE_CHECK;
            }
            else
            {
                upper->SendFileChkResult();
                upper->InsertInterlock(125, 90, ERROR_IL, IL_PROGRAM, NOT_LATCHED);
                qDebug() << "RemoteController: REMOTE_ERROR" << endl;
                mRemoteStatus = REMOTE_ERROR;
            }
        }
        break;
    case FILE_CHECK:
        if ( mRemoteParam.mlcdataFeed )
        {
            if (MF_COR == mFileCheck)
            {
                qDebug() << "RemoteController: SET_TREATDATA" << endl;
                mFileCheck = ALL_COR;
                mRemoteStatus = SET_TREATDATA;
            }
            else
            {
                qDebug() << "RemoteController: REMOTE_ERROR" << endl;
                upper->InsertInterlock(125, 90, ERROR_IL, IL_PROGRAM, NOT_LATCHED);
                mRemoteStatus = REMOTE_ERROR;
            }

            upper->SendFileChkResult();
        }
        break;
    case SET_TREATDATA:
        if ( !isSendBeam )
        {
            //17-3-14
            if (g_resume_info.isGet)
            {
                upper->ResumeTreat();
                g_resume_info.isGet = false;
            }
            isSendBeam = true;
            mlc->SendBeamNumber(upper->GetCurBeamDeltaMu());
            setDataThread.start(QThread::HighestPriority);
        }

        if (mRemoteParam.treatdataSet)
        {
            qDebug() << "RemoteController: WAIT_ACCEPT" << endl;
            mRemoteStatus = WAIT_ACCEPT;
            emit remote->setRemoteMsg(MSG_REMOTE_TERMINATE);
        }

        break;
    case WAIT_ACCEPT:
        return true;
    default:
        break;
    }

    return false;
}

void RemoteController::ResetSetting(RemoteStatus remote_status)
{
    qDebug() << "clear parameter and set remote_status=" << remote_status << endl;
    mRemoteStatus = remote_status;

    switch(remote_status)
    {
    case INIT:
        g_is_remote_mode = false;
        qDebug() << "quit remote mode" << endl;
        SendUnconnect();
        emit showRemoteControl(false);
        ExitRemoteMode();
    case WAIT_CONNECT:
        mRemoteParam.upperConnect = false;
        mRemoteParam.mlcConnect = false;
        isMlcMoveStop = true;
        g_resume_info.isGet = false;
    case WAIT_TREATDATA:
        upper->DeleteInterlock(125, 90);
        //upper->DeleteInterlock(126, 100);
        mlc->DeleteInterlock(81, 101);
        mFileCheck = NOCHECK;
        mRemoteParam.treatdataGet = false;
        mCurSegId = 1;
    case FILE_CHECK:
        mRemoteParam.mlcdataFeed = false;
    case SET_TREATDATA:
        mRemoteParam.treatdataSet = false;
        mMlcMoveToBeamEcho = MMBE_NOCHECK;
        isSendBeam = false;
        hasSetdata = false;
        isSettingData = false;
    case WAIT_ACCEPT:
        mUpperAcceptEcho = EUR_NOCHECK;
        mlc->DeleteInterlock(124);
    default:
        break;
    }
}

QString AutosetDataThread::ConvertGateToString(float val)
{
    QString temp;

    if(val <= -0.1)
        temp.sprintf("%05.1f", val);
    else if(val < 0.1)                //-0.1<x<0.1
    {
        val = 0;
        temp.sprintf("%04.1f", val);
    }
    else
        temp.sprintf("+%04.1f", val);

    return temp;
}

void AutosetDataThread::run()
{
    QString temp;
    BEAMDATA_ST beamData;
    CONTROLPOINT_ST ctrlPointData, tmp;
    TREATDATA_SETSTEP_T step = TS_GET_DATA;

    if (!g_treatqueue_mutex.tryLock())
        return;

    remote->hasSetdata = true;
    remote->isSettingData = true;

    while ( step < TS_BACK_INIT )
    {
        /*if (SET_TREATDATA != remote->mRemoteStatus)
        {
            upper->isTreatQueueUse = false;
            return;
        }*/

        switch (step)
        {
        case TS_GET_DATA:
            emit resetUISelectStep();
            if (BT_CONFORMAL == upper->GetBeamType())
            {
                upper->GetCurBeamData(beamData);
                upper->GetCurCtrlPointData(ctrlPointData);
                beamData.finalMeterSet = ctrlPointData.meterSet;
                upper->SetBeamJawByCtrlPoint(beamData, ctrlPointData);
            }
            else if (BT_STATIC == upper->GetBeamType())
            {
                upper->GetCurBeamData(beamData);
                upper->GetCurCtrlPointData(ctrlPointData);
                beamData.finalMeterSet = ctrlPointData.meterSet;

                if (false == ctrlPointData.setXY)
                {
                    upper->GetPrevCtrlPoint(tmp);
                    upper->SetBeamJawByCtrlPoint(beamData, tmp);
                }
                else
                {
                    upper->SetBeamJawByCtrlPoint(beamData, ctrlPointData);
                }
            }
            else if (BT_DYNAMIC == upper->GetBeamType())
            {
                upper->GetCurBeamData(beamData);
            }

            upper->curBeamData = beamData;

            step = TS_DEAL_FIRST;
            break;
        case TS_DEAL_FIRST:
            emit ShowTreatUI();

            step = TS_SET_SEG;
            break;
        case TS_SET_SEG:
            emit setTreatdataWithData(TL_CUR_SEG, QString::number(remote->mCurSegId));
            emit setTreatdataWithData(TL_PRESET_SEG, QString::number(remote->mTotalSegNum));

            step = TS_RAY_ENERGY;
            break;
        case TS_RAY_ENERGY:
            if (RT_XRAY == beamData.rayType)  //x ray
            {
                if (EG_LOW == beamData.energy)   //low energy
                {
                    emit setTreatdataWithKey(TK_F1);
                    INPUT_DELAY;
                    emit setTreatdataWithData(TL_DOSE1_VALUE, QString::number((UINT16)beamData.finalMeterSet));
                    emit setTreatdataWithKey(TK_RETURN);
                    emit setBigDataInput(beamData.finalMeterSet);
                }
                else if (EG_HIGH == beamData.energy)   //high energy
                {
                    emit setTreatdataWithKey(TK_F2);
                    INPUT_DELAY;
                    emit setTreatdataWithData(TL_DOSE1_VALUE, QString::number((UINT16)beamData.finalMeterSet));
                    emit setTreatdataWithKey(TK_RETURN);
                    emit setBigDataInput(beamData.finalMeterSet);
                }
            }

            step = TS_DOSERATE;
            break;
        case TS_DOS_VALUE:
            emit setTreatdataWithKey(TK_F2);
            INPUT_DELAY;
            emit setTreatdataWithData(TL_DOSE1_VALUE, QString::number((UINT16)beamData.finalMeterSet));
            emit setTreatdataWithKey(TK_RETURN);
            emit setBigDataInput(beamData.finalMeterSet);

            step = TS_DOSERATE;
            break;
        case TS_DOSERATE:
            if (DR_LOW == beamData.doserate) //low dosrate
            {
                emit setTreatdataWithKey(TK_F4);
                emit setTreatdataWithKey(TK_F1);
            }
            else if (DR_HIGH == beamData.doserate)   //high dosrate
            {
                emit setTreatdataWithKey(TK_F4);
                emit setTreatdataWithKey(TK_F2);
            }

            step = TS_COLLI_ANGLE;
            break;
        case TS_COLLI_ANGLE:
            emit setTreatdataWithKey(TK_F6);
            emit setTreatdataWithKey(TK_F3);
            INPUT_DELAY;
            emit setTreatdataWithData(TL_COLLI, temp.sprintf("%05.1f", beamData.colliAngle));
            emit setTreatdataWithKey(TK_RETURN);
            INPUT_DELAY;
            emit setTreatdataWithKey(TK_F6);

            step = TS_GANTRY_ANGLE;
            break;
        case TS_GANTRY_ANGLE:
            emit setTreatdataWithKey(TK_F3);
            INPUT_DELAY;
            emit setTreatdataWithData(TL_GANTRY, temp.sprintf("%05.1f", beamData.gantryAngle));
            emit setTreatdataWithKey(TK_RETURN);

            step = TS_X1POS;
            break;
        case TS_X1POS:
            emit setTreatdataWithKey(TK_F6);
            emit setTreatdataWithKey(TK_F2);
            emit setTreatdataWithKey(TK_F1);
            INPUT_DELAY;
            //qDebug() << "want to set x1" << ConvertGateToString(beamData.x1Pos) << endl;
            emit setTreatdataWithData(TL_X1, ConvertGateToString(beamData.x1Pos));
            emit setTreatdataWithKey(TK_RETURN);

            step = TS_X2POS;
            break;
        case TS_X2POS:
            emit setTreatdataWithKey(TK_F2);
            INPUT_DELAY;
            //qDebug() << "want to set x2" << ConvertGateToString(beamData.x2Pos) << endl;
            emit setTreatdataWithData(TL_X2, ConvertGateToString(beamData.x2Pos));
            emit setTreatdataWithKey(TK_RETURN);

            step = TS_Y1POS;
            break;
        case TS_Y1POS:
            emit setTreatdataWithKey(TK_F3);
            INPUT_DELAY;
            //qDebug() << "want to set y1" << ConvertGateToString(beamData.y1Pos) << endl;
            emit setTreatdataWithData(TL_Y1, ConvertGateToString(beamData.y1Pos));
            emit setTreatdataWithKey(TK_RETURN);

            step = TS_Y2POS;
            break;
        case TS_Y2POS:
            emit setTreatdataWithKey(TK_F4);
            INPUT_DELAY;
            //qDebug() << "want to set y2" << ConvertGateToString(beamData.y2Pos) << endl;
            emit setTreatdataWithData(TL_Y2, ConvertGateToString(beamData.y2Pos));
            emit setTreatdataWithKey(TK_RETURN);

            step = TS_BACK_INIT;
        case TS_BACK_INIT:
            //ProcessTreatF6();
            //ProcessTreatF6();
            INPUT_DELAY;

            break;
        default:
            break;
        }

        INPUT_DELAY;

        if (step == TS_BACK_INIT)
        {
            remote->mRemoteParam.treatdataSet = true;
#ifdef PRINT_REMOTE_DEBUG
            qDebug() << "treatdataSet = true" << endl;
#endif
        }
    }

    remote->isSettingData = false;
    g_treatqueue_mutex.unlock();
}
