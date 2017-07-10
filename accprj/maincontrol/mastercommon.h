#ifndef MASTERCOMMON_H
#define MASTERCOMMON_H

#include "mastercontroller.h"
#include "ui/accui.h"
#include "Can/Can.h"
#include "remotecontroller.h"
//#define STEPDELY msleep(100)
#define STEPDELY    // msleep(20)
#define CMDGROUPDLY 0//  50

//#define OFFLINE

extern float g_treat_time;

extern RemoteController *remote;
extern AccUi *accui;
extern UpperMonitor *upper;
extern Mlc *mlc;
extern USERMODE_TYPE_T g_usermode;
extern UINT32 g_right_key;
//extern MODE_T g_treat_svc_mode;

extern bool g_auth_check;
extern bool g_acc3_change;
//extern ACCESSORY_B_TYPE accessory_3_type;

extern bool g_is_rotate_treatment;    //旋转治疗标志
extern bool g_download_finish;
extern bool isSipNormalWork;

extern bool g_is_terminate;         //terminate标志
extern bool g_is_complete;          //complete标志
extern bool g_treat_finish;          //完成一次治疗后才能使用F9调回治疗参数

extern enum Status g_status;
extern bool g_check1_bypass;
extern bool g_check2_bypass;

extern UINT32 g_il_state;
extern RE_TE_MODE g_mode;

extern QMutex g_action_key_mutex;
extern UINT32 g_action_key;

#endif // MASTERCOMMON_H
