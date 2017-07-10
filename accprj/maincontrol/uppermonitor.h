#ifndef UPPER_MONITOR_H
#define UPPER_MONITOR_H

#include "slave_base.h"
#include "tools/treatfileparser.h"
#include <vector>
#include <map>
#include "tools/treatfileparser.h"
#include "tools/ftpdownloader.h"
#include "tools/ftpuploader.h"
using namespace std;

typedef enum{
    EUR_NOCHECK             = 0x80,
    EUR_CANCEL              = 0xFF,
    EUR_NOERR             = 0x00,
    EUR_BEAM_ID,
    EUR_CONTROLPOINT_ID,
    EUR_BEAM_TYPE,
    EUR_RADIATION_TYPE,
    EUR_BEAM_MU,
    EUR_CONTROLPOINT_MU,
    EUR_ENERGY,
    EUR_DOSERATE,
    EUR_DOSE,
    EUR_GANTRY_ANGLE,
    EUR_COLLIMATOR_ANGLE,
    EUR_SLOT1,
    EUR_SLOT2,
    EUR_SLOT3,
    EUR_TECH_TYPE,
    EUR_JAW_X1,
    EUR_JAW_X2,
    EUR_JAW_Y1,
    EUR_JAW_Y2,
    EUR_MODE,   //110
    EUR_HEARTBEAT,  //111
    EUR_INVALID_FRAME,  //112
    EUR_BEAM_NUMBER,    //113
    EUR_DELIVERED_MU, //114
    EUR_CONTROLPOINT_NUMBER,    //115
    EUR_NO_FIRST_JAW,   //116
    EUR_MACHINE_NAME,    //117
    EUR_INVALID_CHECKSUM
}ERROR_UPPER_REPORT_T;


typedef enum {
    MMBE_NOCHECK      = 0x80,
    MMBE_NOERR        = 0x00,
    MMBE_MOV_FAIL     = 0x01,    //移动到第一个子眼失败
}MLC_MOVE_BEAM_ECHO_T;

typedef enum {
    RE_SUCCESS,
    RE_FAIL
}RESUME_ECHO_T;

typedef struct
{
    bool isGet;
    UINT8 beamId;
    UINT8 ctrlpointId;
    UINT16 deliveredMu;
}TREAT_RESUME_ST;

class UpperMonitor : public SlaveBase
{
    Q_OBJECT
public:
    UpperMonitor();

    bool AnalysRevFram(TransRecvT& revdata);       //收到并且解析命令，分配到下面函数中处理

    bool HasNextData();
    void GetCurBeamData(BEAMDATA_ST &beamData);
    UINT16 GetCurBeamDeltaMu();
    void GetCurCtrlPointData(CONTROLPOINT_ST &ctrlPointData);
    bool IsFirstBeam(UINT8 id);
    void NextData();
    UINT8 GetCurBeamId();
    UINT8 GetCurCtrlPointId();
    UINT8 GetCurCtrlPointIdx();
    BEAMTYPE_T GetBeamType(INT8 beamId = -1);
    bool GetPrevCtrlPoint(CONTROLPOINT_ST &ctrlPointData);
    bool SendResumeEcho(RESUME_ECHO_T value);

    bool SendConnectReq();
    bool SendFileChkResult();
    bool SendAccept();
    bool SendAcceptOther();
    bool SendReadyStatus(const READYSTATE_TYPE_T &status);
    bool SendRadOn(void);
    bool SendInterrupt();
    bool SendTerminateOrComplete(bool terminate);
    bool SendTreatEnd();
    bool SendUserMode(const USERMODE_TYPE_T mode);
    bool SendPgmReach();
    bool SendGantryPos();
    bool SendColliPos();
    bool SendX1X2Pos();
    bool SendY1Y2Pos();
    bool SendAcc12Value();
    bool SendAcc3CtrlpointValue();
    bool SendActDos();
    bool SendLastDos();
    bool SendHeartbeat();
    bool SendUnconnect();

public:
    void SetActDose(UINT32 actDose);
    void SetBeamJawByCtrlPoint(BEAMDATA_ST &beamData, CONTROLPOINT_ST ctrlPoint);

    bool IsRadonComplete();
    bool ResumeTreat();

    bool DownloadData();
    void CheckAcceptData();

    UINT8 getBeamNumber();

private:
    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);
    bool ProcessCHKIL(TransRecvT& revdata);

    void InitAccMap();
    void DealInterruptedBeam(UINT8 beamId);
    void ClearFrameNumber();
    UINT16 CntSegFromCur();
    void SetCurBeamFirstPoint();
    void SetLastDose(UINT32 lastDose);

    bool RevDownloadFile(TransRecvT& revdata);
    bool RevUpperConnectAck(TransRecvT& revdata);
    bool RevResetEcho(TransRecvT& revdata);
    bool RevAcceptEcho(TransRecvT& revdata);
    bool RevErrorReport(TransRecvT& revdata);
    bool RevHeartbeat(TransRecvT& revdata);
    bool RevResume(TransRecvT& revdata);
    bool RevCtrlpointPosition(TransRecvT& revdata);
    void ProcessErrorReportIL(ERROR_UPPER_REPORT_T type);

    bool HasNextBeam();

private:
    void CheckTreatFile();
    void CheckConformalFile(UINT8 beamId);
    void CheckStaticFile(UINT8 beamId);
    void CheckDynamicFile(UINT8 beamId);

    INT32 ConvertFloatToInt(float val, UINT8 mul);

signals:
    void SetCurSeg(UINT8 cur);

public:
    BEAMDATA_ST curBeamData;
    RAYTYPE_T setedRayType;
    UINT16 setedMeterSet;
    ENERGY_T setedEnergy;
    TECH_T setedTech;
    DOSERATE_T setedDosrate;
    float setedGantry;
    float setedColli;
    float setedX1Pos;
    float setedX2Pos;
    float setedY1Pos;
    float setedY2Pos;
    QString setedAcc1;
    QString setedAcc2;
    QString setedAcc3;

    UINT16 beamPos;
    UINT16 ctrlpointPos;

public:
    bool mIsHeartbeatGet;

private:
    UINT32 mActDose;
    UINT32 mLastDose;

private:
    map<QString, UINT8> mAccMap[3];

private:
    QMutex mUpperMutex;
    UINT8 mSendFrameNum;
    UINT8 mRevFrameNum;

private:
    TREATDATA_SEQ_ST mTreatQueue;
    TreatfileParser mParser;
    FtpDownloader mFtpDonwloader;
};

#endif // UPPER_MONITOR_H
