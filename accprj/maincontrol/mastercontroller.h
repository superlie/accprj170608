#ifndef MASTERCONTROLLER_H
#define MASTERCONTROLLER_H
#include <QThread>
#include <QMutexLocker>
#include "motor.h"
#include "dosei.h"
#include "doseii.h"
#include "beam.h"
#include "hand.h"
#include "light.h"
#include "interlock.h"
#include "mlc.h"
#include "uppermonitor.h"
#include "io.h"
#include "can/Can.h"

const int TRY_TIMES = 3;
const int SLEEP_TIME = 1000;



//typedef enum{
//    NULL_KEY = 0,
//    STANDBY_KEY = 1,
//    PROGRAM_KEY = 2,
//    ACCEPT_KEY = 4,
//    RADON_KEY = 8,
//    RADOFF_KEY = 16,
//    RESET_KEY = 32,
//    MOTIONSTOP_KEY = 64,
//    F1_KEY = 128,
//    F2_KEY = 256,
//    F3_KEY = 512,
//    TREATMENT_KEY = 1024,
//}ACTION_KEY_T;


const UINT32    NULL_KEY        = 0,
                STANDBY_KEY     = 0x1,
                PROGRAM_KEY     = 0x2,
                ACCEPT_KEY      = 0x4,
                RADON_KEY       = 0x8,
                RADOFF_KEY      = 0x10,
                RESET_KEY       = 0x20,
                MOTIONSTOP_KEY  = 0x40,
                F1_KEY          = 0x80,
                F2_KEY          = 0x100,
                F3_KEY          = 0x200,
                TREATMENT_KEY   = 0x400,
                CLEAR_KEY       = 0x800,
                SERVICE_KEY     = 0x1000,
                C_KEY           = 0x2000,
                REMOTE_KEY      = 0x4000;


const UINT32    NULL_RKEY=0,
                STDBY_RKEY=1,
                PGM_RKEY=2,
                RADON_RKEY=4;

const UINT32    NULL_LKEY=0,
                RESET_LKEY=1,
                TREAT_LKEY=2,
                SVC_LKEY=4;

class MasterController: public QThread//, SlaveBase
{
public:
    MasterController();
    ~MasterController();

//    bool ConnectSIP();   //与Slaves链接成功返回true
//    bool StandbyKey();      //按动Standby按键返回true
//    bool ProgramKey();
//    bool AcceptKey();
//    bool RadOnKey();
//    bool RadOffKey();
//    bool ResetKey();
//    bool MotionStopKey();
//    bool F1Key();
//    bool F2Key();
//    bool F3Key();
    UINT32 CheckActionKey();      //检测按键的动作
    void CleanActionKey(UINT32);
    void CleanInterlocks();
    bool Check1Passed();          //check1通过返回true
    bool Check2Passed();

    bool RadComplete();
    CHECK_IL_RES_T CheckInterlock();
    bool RevIOAcknowledge();
    bool RevDoseOffset();
    bool RevResetEcho_1();
    bool RevResetEcho_2();

    void SendClearCommands();
    void SendC0Commands();
    void SendAcceptCommands();
    void SendCheck1Commands();
    void SendCheck2Commands();
    void SendResetCmdAll();
    void SendResetCmdN04();
    void SendReadyCommands(READYSTATE_TYPE_T readystate);
    void SendRadOnCommands();
    void SendInterruptCommands();
    void SendTerminateCommands(bool terminate);
    void SendMotorConfigCommands();
    void SendHandConfigCommands();
    void SendIOConfigCommands();
    void SendOtherCmdNoStop();
    void SendRayModeCommands();
    void SendUserMode(bool upperSend = true);

    bool ResetCmd();
    void SendOtherCmd();

    void ProcessComplete();
    void ProcessTerminate();
    void ProcessInterrupt();
    void DeleteReadyInterlock();

    //bool CheckAcc();

    void NormalMasterCtrl(enum Status &state, enum Status &last_state);
    void RemoteMasterCtrl(enum Status &state, enum Status &last_state);

protected:
    void run();

private:
    CHECK_IL_RES_T mc_interlock_status;
    QMutex mc_wait_mutex;
};

/*全局变量*/

extern QBitArray g_lock_passby_bits;
extern enum Status g_status;
extern QTime g_radon_time;

extern QMutex g_action_key_mutex;
extern UINT32 g_action_key;

extern bool g_is_terminate;         //terminate标志
extern bool g_is_complete;          //complete标志


//全局对象
extern SlaveBase *base;
extern Motor *motor;
extern DoseOne *dosei;
extern DoseTwo *doseii;
extern Beam *beam;
extern HandCon *hand;
extern Light *light;
extern Interlock *interlock;
extern InputOutput *io;
extern Can *can;
extern Mlc *mlc;
extern UpperMonitor *upper;
extern const QBitArray g_zero_bits;
extern QBitArray g_system_il_bits;
extern QBitArray g_safety_il_bits;
extern QBitArray g_latched_il_bits;

#endif // MASTERCONTROLLER_H
