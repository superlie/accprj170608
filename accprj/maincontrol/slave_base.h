#ifndef SLAVE_BASE_H
#define SLAVE_BASE_H

#include <QObject>
#include <QMutex>
#include <QWaitCondition>
#include <QSemaphore>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QBitArray>
#include <QMetaType>
#include "acc_type.h"
#include <QList>
#include <QTimer>
#include <QTime>
#include <string.h>
#include <QReadWriteLock>
#include "slave_public.h"
#include "defines.h"
//#include "ui/accui.h"

//#define OUTQUEUELEN 10
//#define INQUEUELEN 24
const int OUTQUEUELEN = 200;
const int INQUEUELEN = 200;

#define ACC_DEBUG

struct TransRecvT {
  UINT8 len;
  UINT8 data[20];
};

//Interlock属性1：联锁类型
typedef enum
{
    SAFETY_IL,
    SYSTEM_IL,
    ERROR_IL,
}IL_TYPE_T;

//Interlock属性2：是否锁存
typedef enum
{
    NOT_LATCHED,
    LATCHED,
    RADON_LATCHED,
}LATCHED_T;

//Interlock属性3：联锁发生的阶段
const UINT32
            IL_POWUP        = 0x0001,
            IL_NULL         = 0x0002,
            IL_PROGRAM      = 0x0004,
            IL_STANDBY      = 0x0008,
            IL_NOT_READY    = 0x0010,
            IL_READY        = 0x0020,
            IL_RADON        = 0x0040,
            IL_INTERRUPT    = 0x0080,
            IL_INTERRUPT_NR = 0x0100,
            IL_COMPLELE     = 0x0200,
            IL_TERMINATE    = 0x0400,
            IL_ERROR        = 0x0800,
            IL_CHECK1       = 0x1000,
            IL_CHECK2       = 0x2000,
            IL_ALL          = 0xffff;

typedef enum Status
{
    POWER_UP = 0,
    NULL_STATUS1 = 1,
    PROGRAM = 2,
    STANDBY = 3,
    NOT_READY = 4,
    READYS = 5,
    RAD_ON = 6,
    INTERRUPT = 7,
    INTERRUPT_NR = 8,
//    COMPLETE = 9,
//    TERMINATE = 10,
    ERRORS = 11,
    COMPLETE1=12,
    COMPLETE2=13,
    TERMINATE1=14,
    TERMINATE2=15,
    TERMINATE3=16,
    CHECK2_1=17,
    CHECK2_2=18,
    NULL_STATUS2=19,
    NOT_READY1=20,
    NOT_READY2=21

}MAIN_STATUS_T;

typedef enum
{
    NO_OVRD = 0,
    ALWAYS_OVRD = 1,
    CAN_OVRD = 2,

}OVRD_TYPE;
// interlock是否可以被旁路，是否被屏蔽

const int OVRD[]={
    //  1 2 3 4 5 6 7 8 9 10
      0,2,2,2,2,2,2,2,2,2,2,    //0-10
        1,2,2,2,0,2,2,2,2,2,    //11-20
        0,0,0,0,0,0,0,0,0,0,    //21-30
        0,1,0,2,0,0,0,0,0,0,    //31-40
        0,0,0,0,0,0,0,0,2,2,   //41-50
        2,2,0,0,2,2,2,2,2,2,    //51-60
        2,2,2,0,0,0,0,0,0,0,   //61-70
        2,2,2,2,2,2,2,2,2,0,    //71-80
        0,2,0,0,0,0,2,2,2,2,    //81-90
        2,2,2,0,0,2,2,0,1,1,    //91-100
        1,1,1,1,2,2,2,2,2,2,    //101-110
        0,0,0,0,0,0,0,0,0,0,    //111-120
        0,0,0,0,0,0,0,            //121-127
};

typedef enum {
    EANULL = 0x1000,//ACC3
    //EAERR = 0x1000,
    EANULL_1 = 0x0FC0,
    EA205 = 0x06c0,
    EA210 = 0x07c0,
    EA215 = 0x0900,
    EA220 = 0x09c0,
    EA225 = 0x0a80,
//      EANULL_1 = 0x0FF0,
//    EA205 = 0x06e0,
//    EA210 = 0x07d0,
//    EA215 = 0x0920,
//    EA220 = 0x09c0,
//    EA225 = 0x0a80,
} ACCESSORY_B_TYPE; //插入附件AccessoryB后产生的情况

//Interlock状态：是否发生
typedef enum
{
    IL_CLAER,
    IL_ACTIVE,
    IL_LATCHED,
}INTERLOCK_STATUS_T;

//CheckInterlock返回的结果
typedef enum
{
    ILS_CLEARED,
    ILS_SAFETY,
    ILS_SYSTEM,
}CHECK_IL_RES_T;

//联锁中用于表示错误的结构
struct ERROR_ST
{
    UINT32 error_num;
    IL_TYPE_T type;
    UINT32 state;
    LATCHED_T latched;

    bool operator ==(const ERROR_ST &a)
    {
        return error_num == a.error_num;
    }
};

//表示联锁的数据结构
struct INTERLOCK_ST
{   OVRD_TYPE ovrd_type;
    bool ovrd;
    UINT32 history;
    //IL_SHOW_TYPE show_type;
    INTERLOCK_STATUS_T status;
    int last_error_num;
    bool has_err;
//如果has_err为flase, 信息记录在以下3条
    IL_TYPE_T type;
    UINT32 state;
    LATCHED_T latched;
//否则，信息记录在error里
    QList<ERROR_ST> error;

};

typedef enum{
        AFCP_RSCH_CK,       //0
        INJE_RSCH_CK,
        INJI_RSCH_CK,
        ELM_RSCH_CK,
        BMI_RSCH_CK,
        STC_RSCH_CK,
        LENS_RSCH_CK,
        PFN_RSCH_CK,
        IPFN_RSCH_CK,
        SOMIN_RSCH_CK,
        DSGX_RSCH_CK,       //10
        DSGY_RSCH_CK,
        DSWX_RSCH_CK,
        DSWY_RSCH_CK,
        BALX_RSCH_CK,
        BALY_RSCH_CK,
        ELMCK_RSCH_CK,
        BMICK_RSCH_CK,
        STCCK_RSCH_CK,
        LENCK_RSCH_CK,
        MODT_RSCH_CK,       //20
        INJT_RSCH_CK,
        AFCT_RSCH_CK,
        RFT_RSCH_CK,
        TST0_RSCH_CK,
        TST1_RSCH_CK,
        DOSE1_GAIN_CK,
        DOSE2_GAIN_CK,
        DOSE_RATE1_CK,
        DOSE_RATE2_CK,
        DOSE_RATE1_OFF,     //30
        DOSE_RATE2_OFF,
        PRFP_CK,
        DRCV_CK,
        S0_GAIN_CK,
        S1_GAIN_CK,
        S2_GAIN_CK,
        S3_GAIN_CK,
        S4_GAIN_CK,
        SWI_CK,      //只做检查
        FWI_CK,      //只做检查 40
        DOS_T_CK,
        DOS1_PRE_CK,    //只做检查
        DOS2_PRE_CK,    //只做检查
        TIMER_PRE_CK,   //treat timer value

        JAWX_CK,    //同步    转换交给独立坞门
        JAWY_CK,

        JAWX1_CK,   //独立
        JAWX2_CK,
        JAWY1_CK,
        JAWY2_CK,       //50

        COLLI_CK,
        GANTRY_CK,

        //12-04
        JAWY1_SS_RSCH_CK,
        JAWY2_SS_RSCH_CK,
        JAWX2_SS_RSCH_CK,
        JAWX1_SS_RSCH_CK,
        COLLI_SS_RSCH_CK,
        GANTRY_SS_RSCH_CK,
        JAWY1_FS_RSCH_CK,
        JAWY2_FS_RSCH_CK,       //60
        JAWX2_FS_RSCH_CK,
        JAWX1_FS_RSCH_CK,
        COLLI_FS_RSCH_CK,
        GANTRY_FS_RSCH_CK,

        JAWY1_SDS_RSCH_CK,
        JAWY2_SDS_RSCH_CK,
        JAWX2_SDS_RSCH_CK,
        JAWX1_SDS_RSCH_CK,
        COLLI_SDS_RSCH_CK,
        GANTRY_SDS_RSCH_CK,     //70
        JAWY1_SDD_RSCH_CK,
        JAWY2_SDD_RSCH_CK,
        JAWX2_SDD_RSCH_CK,
        JAWX1_SDD_RSCH_CK,
        COLLI_SDD_RSCH_CK,
        GANTRY_SDD_RSCH_CK,

        GANTRY_SPEED_FACTOR_CW_CK,        //只做检查
        GANTRY_SPEED_FACTOR_CCW_CK,        //只做检查   78
        EDGEARRLEN,
}EDGE_CHECK_T;


//SVC中显示返回值
typedef enum
{
    AFCM,
    ELMI_BMIS_STCI_LENI,
    DR_1,
    DR_2,
    SXE_FXE,
    SYE_FYE,
    OFF1,
    OFF2,
    SEG013,
    SEG24,
    TEMP_PRESS1,
    TEMP_PRESS2,
    CPS_FACTOR,
    T1_0,
    T1_1,
    T1_2,
    T1_3,
    T1_4,
    T1_5,
    T1_6,
    T1_7,
    CHK_0,
    CHK_1,
    CHK_2,
    CHK_3,
    CHK_4,
    CHK_5,
    CHK_6,
    CHK_7,
} SVC_LINEEDIT;



typedef struct
{
    float min_value;
    float max_value;
    float k;      //y = k * x + b;
    float b;
}EDGE_CHECK_ST;

const int ERROR_NUM = 1300;
const int INTERLOCK_NUM = 128;
const int SLAVE_NUM = 8;
const float MAX1 = 10000.0;
const float MAX2 = 500.0;
const float MAX3 = 100.0;
const float MIN1 = 100.0;
const float MAX_MOTOR_SPEED = 2047.9;
const float MIN_MOTOR_SPEED = 0;
const float MAX4 = 6.3;
const float MIN2 = 0.051;
const float MIN_MOTOR_SDD = 10.0;
const float MAX5 = 10.0;
const float MAX_MOTOR_SDS = 1020.9;
const float MAX_MOTOR_SDD = 255;
const float MAX8 = 4095.0;
const float MIN5 = 500.0;
const float MIN_MOTOR_SDS = 4;
const float K1 = 0.4095;
const float K2 = 8.19;
const float K3 = 10.0;
const float K4 = 0.2;
const float K5 = 0.25;
const float GANTRY_LIMIT = 7.0;

class SlaveBase:public QObject
{
    Q_OBJECT
    public:
       SlaveBase(QObject *parent = 0);
       ~SlaveBase(void);

       static bool AnalysRevFram();
       static bool AnalysRevFram(TransRecvT &);

    public:
        static TransRecvT outQueue[OUTQUEUELEN];   //发送队列
        static UINT8 outFrontP;
        static UINT8 outTailP;
        static QMutex out_queue_mutex;

        static QSemaphore outqueue_free;
        static QSemaphore outqueue_used;

        static TransRecvT routQueue[OUTQUEUELEN];   //remote发送队列
        static UINT8 routFrontP;
        static UINT8 routTailP;
        static QMutex rout_queue_mutex;

        static QSemaphore routqueue_free;
        static QSemaphore routqueue_used;

        static TransRecvT inQueue[INQUEUELEN];     //接收队列
        static UINT8 inFrontP;
        static UINT8 inTailP;
        static QMutex in_queue_mutex;
        static QSemaphore inqueue_free;
        static QSemaphore inqueue_used;

        static bool open_softpot_file;

        static bool InsertOutQueue(TransRecvT &data);
        static bool InsertRemoteOutQueue(TransRecvT &data);
        static bool DleOutQueue();

        static bool InsertInQueue(TransRecvT & data);
        static bool DelInQueue(TransRecvT& data);

        //virtual UINT8 ComputeCHK();
        static UINT8 ComputeLstCHK(UINT8 data[], int len);
        static bool CheckValue(float srcvalue, EDGE_CHECK_T type, UINT32 &dstvalue);
        static bool CheckValue(float srcvalue, EDGE_CHECK_T type);
        static float ConvertMemberToFloat(UINT32 srcvalue, EDGE_CHECK_T type);

        static bool CheckXMLData(UINT16 srcvalue, EDGE_CHECK_T type);    //LJY-12-16

      //  QQueue<UINT8> queue;
        static void BaseInit();
        static bool ReadSoftpotsFile();
        static bool WriteSoftpotsFile();

        static bool ReadInitFile();
        static bool ReadInitElement();
        static bool WriteInitFile();
        static void WriteInitElement();

        // interlock#3 : dosei & doseii dose rates
         void ProcessDoseRatesIL();
        // interlock#4 : dosei & doseii temperature
         void ProcessPressurenTemperatureIL();


        static void OvrdInterlock(UINT32 num, bool value);

        void InsertInterlock(UINT32 num, UINT32 err, IL_TYPE_T response = SAFETY_IL,
                                    UINT32 state = IL_ALL, LATCHED_T latched = LATCHED);

        void InsertInterlockNoErr(UINT32 num, IL_TYPE_T response,
                                    UINT32 state, LATCHED_T latched);
         void DeleteInterlock(UINT32 num);
         void DeleteInterlock(UINT32 num, UINT32 err);

         bool SendNotFinished();

signals:
    void Interlock_Inserted(int type, int num);
    void Interlock_Deleted(int type, int num);
    void Status_Changed();

    //void DownloadStatus(bool);//LJY-9-9
    void Prompt_Msg(QString str);

    void ProcessF1TreatSig();
    void ProcessF3ServiceSig();

    void ProcessShowUISig();

    void ShowActJawColliGanSig(int type);
    void ShowActDoseTimeMUSig();
    void ShowActSvcValueSig(SVC_LINEEDIT type);

signals:
   //duanzhibin 10/23upe
    void Set_Acc3_Display(ACCESSORY_B_TYPE type);
    void g_commdog_stop_signal();
    //duanzhibin---9/18
    void g_commdog_start_signal();
    void g_dosei_start_signal(int interval);
    void g_doseii_start_signal();
    void g_gantry_start_signal();

    void g_dosei_stop_signal();
    void g_doseii_stop_signal();

    void interlock_redisp_signal() ;

    void GantryDirecSignal(bool cw);

    void g_lighton_start_signal();
    void g_lighton_stop_signal();

    void g_lightrep_start_signal();
    void g_lightrep_stop_signal();

    void g_sipdog_stop_signal();
    void g_sipdog_start_signal();

    void g_warmup_stop_signal();
    void g_warmup_start_signal();

    void g_upcon_start_signal();
    void g_upcon_stop_signal();

    void g_mlccon_start_signal();
    void g_mlccon_stop_signal();

    void g_heartbeat_sender_start();
    void g_heartbeat_sender_stop();

    void g_heartbeat_receiver_start();
    void g_heartbeat_receiver_stop();

    void g_mlc_movestop_start_signal();
    void g_mlc_movestop_stop_signal();
public:
        UINT8 numSendCmd;
//private:
//        static bool open_softpot_file;

public slots:

        void InsertGantryTimeoutIL();
        void InsertDoseiAccTimeoutIL();
        void InsertDoseiiAccTimeoutIL();
        void InsertCommTimeoutIL();

        void SipDogReStart();
        void SipDogStop();
        void SipDogTestOkAndCallSip();

        void WarmTimerStart();
        void WarmTimerStop();
        void WarmUpTimeOut();

        void LightOnTimerStop();
        void LightOnTimerStart();
        void LightOnTimeOut();

        void LightReportTimerStop();
        void LightReportTimerStart();
        void LightReportTimeOut();

        void UpconTimerStart();
        void UpconTimerStop();
        void UpconTimeOut();

        void MlcconTimerStart();
        void MlcconTimerStop();
        void MlcconTimeOut();

        void HeartbeatSenderStart();
        void HeartbeatSenderStop();
        void HeartbeatSenderTimeout();

        void HeartbeatReceiverStart();
        void HeartbeatReceiverStop();
        void HeartbeatReceiverTimeout();

        void MlcMovestopTimerStart();
        void MlcMovestopTimerStop();
        void MlcMovestopTimeOut();
};
class HandleTimer:public QObject
{
   Q_OBJECT
public:
    HandleTimer(QObject *parent = 0);
    ~HandleTimer(){}

private slots:
    void    g_commdog_timer_start();
    void    g_gantry_timer_start();
    void    g_dosei_timer_start(int interval);
    void    g_doseii_timer_start();
};

extern EDGE_CHECK_ST g_edge_arr[EDGEARRLEN];
extern UINT8 g_ray_type;
extern QXmlStreamReader reader;
extern QDomDocument doc;
extern QDomElement root;
extern QDomNode node;
//interlock结构
extern QBitArray g_lock_bits;
extern INTERLOCK_ST g_interlocks[INTERLOCK_NUM];
extern QMutex g_interlocks_mutex;
extern UINT32 g_error_history[ERROR_NUM];

extern QReadWriteLock g_c0_echo_bits_lock;
extern QBitArray g_c0_echo_bits;
extern QBitArray g_check_bits;
extern QMutex g_check_bits_mutex;
//check的阶段
extern CHECKREQ_TYPE_T g_check_index;
extern QMutex g_check_index_mutex;
extern QWaitCondition g_maincontrol_wait;


#endif
