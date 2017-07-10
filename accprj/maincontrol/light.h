#ifndef LIGHT_H
#define LIGHT_H

#include "slave_base.h"

//const UINT8 TOLIGHT = 0x15;
typedef enum
{
    NOT_TREATMENT=0x00,
    IS_TREATMENT=0x01
} RADON_T;



typedef enum
{
    FIELDLIGHT_OFF=0x00,
    FIELDLIGHT_ON=0x01
} FIELD_LIGHT_REPORT_T;

typedef enum
{
    CHECK_PASSDE=0x00,
    RAM_CHECK_FAILED=0x01,
    //RAM_CHECK_FAILED=0x08,        老版本
    ROM_CHECK_FAILED=0x02,
    //ROM_CHECK_FAILED=0x10,        老版本
    WATCHDOG_TEST_FAILED=0x04,
    //WATCHDOG_TEST_FAILED=0x01,    老版本
    T1_TEST_FAILED=0x40
} CHECK_LIGHT_REPORT_T;

typedef enum
{
    ACTIVE=0x00,
    INACTIVE=0x01,
} T1_STATUS_T;


class Light: public SlaveBase
{
public:

    Light( );
    ~Light( );

    //bool ComputeCHK(UINT8* data, UINT8 len, UINT8& fc);
    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);

     /*Send (console to light)*/
    bool SendC0();
    bool SendBeamShield();//0F
    bool SendFieldIlluminate();//10
    bool SendRoomLight();//11
    bool SendOpticalRangeFinder();//12
    bool SendOpticalBackPointer();//13
    bool SendExtendBeamshield();//14
    bool SendRetractBeamshield();//18
    bool SendBeamShieldStop();//2C
    bool SendExtendAndHandcontrol();//34
    bool SendRetractAndHandcontrol();//38
    bool SendUserMode(const USERMODE_TYPE_T mode);
    bool SendAccept();
    bool SendClear(void);
    bool SendRadOn(void);
    bool SendTerminateOrComplete(void);
    bool SendCheckRequest(const  CHECKREQ_TYPE_T index);
    bool SendReadyStatus(const READYSTATE_TYPE_T status);
    bool SendInterrupt(void);

    /*Receive (light to console)*/
    bool AnalysRevFram(TransRecvT& revdata);       //收到并且解析命令，分配到下面函数中处理
    bool RevResetEcho(TransRecvT& revdata);
    bool RevErrorReport(TransRecvT& revdata);
    bool RevFieldlightReport(TransRecvT& revdata);
    bool RevCheckReport(TransRecvT& revdata);
    bool RevEmergencyLineStatus(TransRecvT& revdata);

    void ProcessErrorIL();
    bool ProcessCHKIL(TransRecvT &revdata);


public:


    UINT32 prom_version;    //RESET_ECHO_MSG_ST reset_echo_msg;版本号等信息
    USERMODE_TYPE_T mode_now;
    CHECKREQ_TYPE_T check_index;
    READYSTATE_TYPE_T ready_status_now;
    UINT8 error_now;
    FIELD_LIGHT_REPORT_T fieldlight_report;
    CHECK_LIGHT_REPORT_T check_report;
    T1_STATUS_T line_status;

    bool light_on;

private:
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;
    QMutex light_mutex;
};

#endif // LIGHT_H
