#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#include <QObject>
#include "acc_type.h"
#include "slave_base.h"
#include "uppermonitor.h"
#include "maincontrol/io.h"
#include <QThread>
#include <map>
#include <vector>
using namespace std;

typedef enum RemoteStatus
{
    REMOTE_ERROR = 0,
    INIT = 1,
    WAIT_CONNECT = 2,                      //等待上位机和MLC的connect的反馈，接到反馈后上报数据
    WAIT_TREATDATA = 3,                    //上报数据完毕，等待治疗数据到达
    FILE_CHECK = 4,            //治疗数据已读取完毕，等待MLC读取文件的反馈
    SET_TREATDATA = 5,             //MLC数据校验完毕，发送beam号给上位机和MLC之后设置治疗数据
    MLC_CHECK = 6,              //治疗数据设置完毕，等待MLC检查结果
    WAIT_ACCEPT = 7,         //MLC检查完成，等待按下ACCEPT按键
}REMOTE_STATUS_T;

typedef struct
{
    //CONNECT
    bool upperConnect;
    bool mlcConnect;
    //WAIT_TREATDATA
    bool treatdataGet;
    //FILE_CHECK
    bool mlcdataFeed;
    //SET_TREATDATA
    bool treatdataSet;
}REMOTE_PARAM_ST;

typedef enum
{
    TS_GET_DATA = 0x00,
    TS_DEAL_FIRST = 0x01,
    TS_SET_SEG = 0x02,
    TS_RAY_ENERGY = 0x03,
    TS_DOS_VALUE = 0x04,
    TS_DOSERATE = 0x05,
    TS_COLLI_ANGLE = 0x06,
    TS_GANTRY_ANGLE = 0x07,
    TS_X1POS = 0x08,
    TS_X2POS = 0x09,
    TS_Y1POS = 0x0A,
    TS_Y2POS = 0x0B,
    TS_BACK_INIT = 0x0C,
    TS_IDLE = 0x0D
}TREATDATA_SETSTEP_T;

typedef enum
{
    TK_F1 = 0x01,
    TK_F2 = 0x02,
    TK_F3 = 0x03,
    TK_F4 = 0x04,
    TK_F5 = 0x05,
    TK_F6 = 0x06,
    TK_RETURN = 0x07,
}TREATDATA_KEY_T;

typedef enum
{
    TL_DOSE1_VALUE = 0x01,
    TL_DOSE_TIME = 0x02,
    TL_GANTRY = 0x03,
    TL_COLLI = 0x04,
    TL_X1 = 0x05,
    TL_X2 = 0x06,
    TL_Y1 = 0x07,
    TL_Y2 = 0x08,
    TL_PRESET_SEG = 0x09,
    TL_CUR_SEG = 0x0A,
}TREATDATA_LINEEDIT_T;

class AutosetDataThread : public QThread
{
    Q_OBJECT
public:
    AutosetDataThread()
    {
    }
    ~AutosetDataThread(){}

protected:
    void run();
    QString ConvertGateToString(float val);

signals:
    void setBigDataInput(int dos);
    void setTreatdataWithKey(int key);
    void setTreatdataWithData(int lineedit, QString str);
    void ShowTreatUI();
    void resetUISelectStep();
};

class RemoteController : public QObject
{
    Q_OBJECT
public:
    RemoteController();
    bool RemoteStateMachine();
    void ResetSetting(RemoteStatus remote_status);

    void DownloadDataInit();

    void ExitRemoteMode();
    void DeleteRemoteInterlock();

    bool SendUnconnect();

    bool StartHeatBeat();
    bool SendHeartBeat();
    bool StopHeartBeat();

    bool CheckCtrlpointPostion();

private:
    void SetTreatData();
    void BigDosInput(UINT16 dos);

public:
    enum RemoteStatus mRemoteStatus;
    REMOTE_PARAM_ST mRemoteParam;
    TREAT_FILE_CHECK_T mFileCheck;
    ERROR_UPPER_REPORT_T mUpperAcceptEcho;
    MLC_MOVE_BEAM_ECHO_T mMlcMoveToBeamEcho;
    bool isMlcMoveStop;
    bool isSendBeam;
    bool hasSetdata;
    bool isSettingData;
    UINT16 mTotalSegNum;
    UINT16 mCurSegId;
    bool mIsUpperWork;
    bool mIsMlcWork;
    AutosetDataThread setDataThread;                          //接收数据线程

signals:
    void setTreatData();
    void startHeartBeatTimer();
    void stopHeartBeatTimer();
    void showRemoteControl(bool show);
    void setRemoteMsg(QString msg);

    void updateSegProgressBar();
    void updateDosProgressBar(int dos);
    void resetProgress();
    void backToProgram();
};

#endif // REMOTECONTROLLER_H
