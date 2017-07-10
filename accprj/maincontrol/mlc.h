#ifndef MLC_H
#define MLC_H

#include "slave_base.h"

typedef enum{
    EMR_NOERR                   = 0x00,     //NO ERROR
    EMR_FIELD_POSITION          = 0x01,  //MLC leaf actual position does not match preset position
    EMR_COVERAGE                = 0x02,   //A jaws do not cover contiguous closed leaf pairs outside the field. For MLC in conformal mode only.
    EMR_LEAF_OUTOF_RANGE        = 0x03,      //Out of range leaf data received.
    EMR_LEAF_OVERLAP            = 0x04,        //Overlapping leaf pair data received.
    EMR_LEAF_UNINTEND_MOTION    = 0x05,   //Unintended MLC leaf motion.
    EMR_HARDWARE_FAIL           = 0x06,         //MLC H/W FAILURE
    EMR_LOGOFF                  = 0x07,    //logoff
    EMR_MOVE_STOP               = 0x08,
    EMR_INVALID_FRAME,
    EMR_INVALID_CHECKSUM
}ERROR_MLC_REPORT_T;

class Mlc : public SlaveBase
{
public:
    Mlc();
    bool AnalysRevFram(TransRecvT& revdata);       //收到并且解析命令，分配到下面函数中处理
    void SetActDose(UINT32 actDose);

    bool SendConnectReq();
    bool SendFileReach();
    bool SendResume();
    bool SendPgmReach();
    bool SendAccept();
    bool SendCheck();
    bool SendReadyStatus(const READYSTATE_TYPE_T &status);
    bool SendRadOn(void);
    bool SendInterrupt();
    bool SendTerminateOrComplete(bool terminate);
    bool SendTreatEnd();
    bool SendUserMode(const USERMODE_TYPE_T mode);
    bool SendBeamNumber(UINT16 deliveredMU);
    bool SendHeartbeat();
    bool SendUnconnect();
    bool SendActDos();
    bool SendRadonDelayTime();

private:
    void ClearFrameNumber();

    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);
    bool ProcessCHKIL(TransRecvT& revdata);

    bool RevMlcConnectAck(TransRecvT& revdata);
    bool RevResetEcho(TransRecvT& revdata);
    bool RevFileParseEcho(TransRecvT& revdata);
    bool RevMoveToBeam(TransRecvT& revdata);
    bool RevMoveStop(TransRecvT& revdata);
    bool RevErrorReport(TransRecvT& revdata);
    bool RevHeartbeat(TransRecvT& revdata);
    bool RevCtrlpointPosition(TransRecvT &revdata);
    void ProcessErrorReportIL(ERROR_MLC_REPORT_T error_now);

public:
    bool mIsHeartbeatGet;

    UINT16 beamPos;
    UINT16 ctrlpointPos;

private:
    QMutex mMlcMutex;
    UINT8 mSendFrameNum;
    UINT8 mRevFrameNum;

    UINT32 mActDose;
};

#endif // MLC_H
