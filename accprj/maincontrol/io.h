#ifndef INPUTOUTPUT_H
#define INPUTOUTPUT_H

#include "slave_base.h"

//const UINT8 TOIO = 0x16;
const int TAPLEN = 0x0B;
const int FOILLEN = 0x07;

typedef enum
{
    FOIL1               = 0x0840,       //2112
    FOIL2               = 0x0880,       //2176
    FOIL3               = 0x0900,       //2304
    FOIL4               = 0x0A00,       //2560
    FOIL5               = 0x0C00        //3072
} FOIL_TABLE_T;

typedef enum {
    E1                  = 0x01,
    E2                  = 0x02,
    E3                  = 0x03,
    E4                  = 0x04,
    E5                  = 0x05,
    E6                  = 0x06,
    X_LO                = 0x08,
    X_HI                = 0x09,
    FILM                = 0x0A,

    ROT_E1              = 0x11,
    ROT_E2              = 0x12,
    ROT_E3              = 0x13,
    ROT_E4              = 0x14,
    ROT_E5              = 0x15,
    ROT_E6              = 0x16,
    ROT_X_LO            = 0x18,
    ROT_X_HI            = 0x19,
    ROT_FILM            = 0x1A,

    FIELD_LIGHT         = 0x88
} RAY_TYPE_T;

typedef enum {
    DISABLE_GANTRY      = 0x00,
    ENABLE_GANTRY       = 0x01
} GANTRY_ENABLE_T;

typedef enum {
    NOTAP               = 0x00,
    TAP122V             = 0x01,     //M
    TAP143V             = 0x02,     //L
    TAP100V             = 0x03      //H
} TAP_T;

typedef enum {
    AUTO                = 0x00,
    MANUAL              = 0x01
}AFC_LOOP_CTL_T;

typedef enum {
    CLOSE               = 0x00,
    OPEN                = 0x01
} DOSE_LOOP_CTL_T;

typedef enum {
    EQUAL               = 0x00,
    NOT_EQUAL           = 0x01
} EQUAL_NOT_T;

typedef enum {
    CHECK1_PASSED       = 0x00,
    CK1_RAM_FAILED      = 0x08,
    CK1_ROM_FAILED      = 0x10,

    CHECK2_PASSED       = 0x00,
    WATCHDOG_FAILED     = 0x01,
    SPV_TOFF_FIALED     = 0x02,
    SPV_RLS_FIALED      = 0x04,
    IO_SPV_FIALED       = 0x20,
    T1_FAILED           = 0x40
} CHECK_IO_REPORT_T;

typedef enum {
    T1_ACTIVE           = 0x00,
    T1_INACTIVE         = 0x01
} T1_IO_REPORT_T;

typedef enum {
    OKEY                =0x00,
    HEAD_BITS_UNEQUAL   =0x01,
    HEAD_UNEQUAL        =0x02,
    BITS_UNEQUAL        =0x03,
} HEAD_BITS_T;

typedef struct {
    RAY_TYPE_T     rayType;
    UINT8       doseTrig;
    UINT8       injectorTrig;
    UINT8       tst1;
    UINT8       rfTrig;
    UINT8       afcTrig;
    UINT8       modulatorTrig;
    UINT8       tst0;
} TRIGGER_ST;
//----------------duanzhibin --9/22

typedef enum {
    RWNULL_1 = 0x1000,// ACC2
    RWNULL_2 = 0x0E00 ,
    RW_15 = 0x0200,
    RW_30 = 0x0400,
    RW_45 = 0x0600,
    RW_60 = 0x0800,
}ACCESSORY_A2_TYPE;

typedef enum{
    RETI_NULL = 0x0FF0,
    RETI_NULL_1 = 0x1000,
    RETI  = 0x0920,     //ACC1
    RETI_1 = 0x0930,
    RETI_INVALID = 0x100
}ACCESSORY_A1_TYPE;

typedef enum{
    DNULL_1 = 0x1000,
    DNULL_2 = 0x0F00,
    DIC_1 = 0x0400,
    DIC_2 = 0x0500, //DIRECTION ACC2
    DIC_3 = 0x0600,
    DIC_4 = 0x0700,
}ACCESSORY_2_DIC;

typedef struct {
    EQUAL_NOT_T  flag;
    UINT16      ACC1A;
    UINT16      ACC1B;
    UINT16      ACC2A;
    UINT16      ACC2B;
} ACCESSORY_A_ST;

typedef struct {
    EQUAL_NOT_T  flag;
    UINT16      ACC3A;
    UINT16      ACC3B;
    UINT16      inPlane;
    UINT16      crossPlane;
} ACCESSORY_B_ST;

typedef struct {
    RAY_TYPE_T     rayType;
    HEAD_BITS_T       headBit;
    UINT16      head;
    UINT16      bit;
} ACKNOWLEDGE_ST;

typedef struct {
    UINT8       data;
    UINT32      version;
} RESET_ST;



class InputOutput : public SlaveBase
{
public:
    InputOutput();
    ~InputOutput();
    //send to slave7

    bool ReadOwnFile();
    bool ReadElement();
    bool ReadOwnElement();
    bool WriteOwnFile();
    void WriteOwnElement();
    void WriteElement();

    bool CheckSoftpotDataLimit();
    bool CheckReschTreatToDefault(RE_TE_MODE mode);
    bool CheckSoftpotDataIntegrity();
    bool CheckFoilDataIntegrity();
    bool CheckTapDataIntegrity();

    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);

    bool SendC0();
    //C
    bool SendUserMode(const USERMODE_TYPE_T mode);// const;
    //D  散射箔选择
    bool SendFoilTable(const RAY_TYPE_T rayType, const FOIL_TABLE_T foil);// const;
    bool SendFoilTable(const RAY_TYPE_T rayType);
    //F
    bool SendTrigger();
    //G
    bool SendGantryMotion(const GANTRY_ENABLE_T &mode) const;
    //H  抽头选择
    bool SendTap(const RAY_TYPE_T &rayType, const TAP_T &type);
    bool SendTap(const RAY_TYPE_T rayType);
    //I
//    bool SendAccept(const RAY_TYPE_T &rayType);// const;
    bool SendAccept();
    //L
    bool SendClear(void);
    //M
    bool SendAfcLoopCtl(const AFC_LOOP_CTL_T &mode) const;
    bool SendAfcLoopCtl();
    //N
    bool SendDosLoopCtl(const DOSE_LOOP_CTL_T &mode) const;
    bool SendDosLoopCtl();
    //O
    bool SendInterrupt(void);
    //Q
    bool SendTurnOffHv(void) const;
    //R
    bool SendRadOn(void);
    //T
    bool SendTerminateOrComplete(void);
    //Z
    bool SendReadyStatus(const READYSTATE_TYPE_T &status);
    //S
    bool SendSetup();//const RAY_TYPE_T &ray) const;
    bool SendSetupFieldLight();
    //W
    bool SendCheckRequest(const CHECKREQ_TYPE_T index);



    //receive from master
    //receive from master
    bool AnalysRevFram(TransRecvT& revdata);

    //A
    bool RevAccessoryA(TransRecvT& revdata);
    //B
    bool RevAccessoryB(TransRecvT& revdata);

    void CheckAccInterlock();

    //E
    bool RevErrorReport(TransRecvT& revdata);
    //K
    bool RevAcknowledge(TransRecvT& revdata);
    //W
    bool RevCheckReport(TransRecvT& revdata);
    //C
    bool RevResetEcho(TransRecvT& revdata);
    //Y
    bool RevEmergencyLineStatus(TransRecvT& revdata);

    void ProcessAcknowledgeIL();
    void ProcessErrorReportIL();
    bool ProcessCHKIL(TransRecvT &revdata);

    ACCESSORY_A_ST GetAccessoryA() const;
    ACCESSORY_B_ST GetAccessoryB() const;
    UINT8 GetErrorType() const;
    ACKNOWLEDGE_ST GetAcknoledge() const;
    CHECK_IO_REPORT_T GetCheck() const;
    RESET_ST GetReset() const;
    T1_IO_REPORT_T GetSetT1() const;

public:
    ACCESSORY_A2_TYPE accessory_2_type;
    ACCESSORY_B_TYPE accessory_3_type;

    USERMODE_TYPE_T mode;
    RAY_TYPE_T foil_ray_type;   //
    FOIL_TABLE_T foil_table;    //
    UINT8 foid_table_int;      //这是什么?
    TRIGGER_ST trig[3];     //RE_TE_MODE
    GANTRY_ENABLE_T gantry_mode;    //
    RAY_TYPE_T tap_ray_type;    //
    TAP_T type;     //
    RAY_TYPE_T accept_ray_type;     //
    AFC_LOOP_CTL_T afc_loop_mode;   //
    DOSE_LOOP_CTL_T dos_loop_mode;  //
    READYSTATE_TYPE_T ready_status;
    RAY_TYPE_T setup_ray_type; //
    CHECKREQ_TYPE_T checkeq;
    UINT32 prom_version;    //RESET_ECHO_MSG_ST reset_echo_msg;版本号等信息
    T1_IO_REPORT_T line_status;

    bool acc1_exist;
    bool acc2_exist;
    bool acc3_exist;
    bool acc1_invalid;
    bool acc2_invalid;
    bool acc3_invalid;
    bool has_inl19;
    bool has_inl20;
    bool has_inl52;
    bool io_jawxy_control;          //插入附件后，屏蔽主机对JAW的操作

    bool mirror_default_pos;

    CHECK_IO_REPORT_T check_report;
    UINT8 error_now;
    TAP_T taps[TAPLEN];
    FOIL_TABLE_T foils[FOILLEN];

    bool item_has[21];

private:
    ACCESSORY_A_ST accA;
    ACCESSORY_B_ST accB;

    UINT8 error_type;
    ACKNOWLEDGE_ST ack;
    CHECK_IO_REPORT_T check;
    RESET_ST rst;


    QMutex io_mutex;
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;

    USERMODE_TYPE_T mode_now;


};

extern RE_TE_MODE g_mode;
extern bool g_io_acknowledge;

#endif // INPUTOUTPUT_H
