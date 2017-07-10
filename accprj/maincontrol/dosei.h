#ifndef DOSEI_H
#define DOSEI_H

#include "slave_base.h"
#include "dosebase.h"

typedef enum {
     NON_PRIMART=0x00,
     PRIMART=0x01,
}MACHINE_TYPE_T;

typedef enum {
	JAW_Y1=0x00,
	JAW_Y2=0x01,
	JAW_X1=0x03,
	JAW_X2=0x02,
} JAW_SELECT_T;

typedef struct {
    UINT16 jawX1MaxSpeed;
    UINT16 jawX2MaxSpeed;
    UINT16 jawY1MaxSpeed;
    UINT16 jawY2MaxSpeed;
}DOSE1_JAWSPEED_ST;

struct DOSE1_GAIN_BITS
{
    UINT8  elecXRay;
    UINT8  rotationalFixed;
    UINT8  stdSkipScan;
    UINT8  xLowHigh;
    UINT8  inPlaneDynSteer;
};
typedef struct {
    UINT16 dose1_gain;
    UINT16 segment1_gain;
    UINT16 segment3_gain;
    UINT16 drcv;
    //struct DOSE1_GAIN_BITS bitArray;
    UINT8 bitArray;
}DOSE1_GAIN_ST;


typedef struct dynamicSteer
{
     UINT16 ds_wind;
     UINT16 ds_gainx;
     UINT16 balx;
}DYNAMIC_STEER_ST;

typedef struct{
    UINT8 xx;
    UINT8 yy;
}DOSE_ERROR_REPORT_ST;

class DoseOne:public DoseBase
{
public :
    DoseOne();
    ~DoseOne();

    bool ReadOwnFile();
    bool ReadElement();
    bool ReadOwnElement();

    bool WriteOwnFile();
    void WriteElement();
    void WriteOwnElement();

    bool CheckSoftpotDataLimit();
    bool CheckReschTreatToDefault(RE_TE_MODE mode);
    bool CheckSoftpotDataIntegrity();
//    bool CheckCompenFactorIntegrity();
//    bool CheckPressTempIntegrity();

    bool AnalysRevFram(TransRecvT& revdata);
    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);
    UINT32 ComputePresetDose(float rads);
    UINT16 ComputeConvertFactor();
    float ConvertDose(UINT32 mcounts);
    float ConvertDoserate(UINT32 mcounts);

    void ClearPcRadonTime();
    void SyncPcRadonTime();
    bool SendC0();
    bool SendUserMode(const  USERMODE_TYPE_T);
    bool SendGainAndWorkMode();//const DOSE1_GAIN_ST &gain);
    //bool SendInformation(UINT16 pTime, UINT32 presetDose1, UINT16 byPass);
    bool SendInformation();

    bool SendRotData();//LJY-9-23

    bool SendDynamicSteer();//const DYNAMIC_STEER_ST &dynSteer);
    // bool SendRadOn (UINT16 compensatedGain1);
    bool SendRadOn();

    bool SendInterrupt();
    //bool SendtreatComplete();
    bool SendTerminateOrComplete();
    bool SendCheckRequest(const CHECKREQ_TYPE_T);
    //bool SendParamBackupConverFact(UINT16 conversionFactor);
    bool SendParamBackupconverFact();

    //bool SendMaxJawSpeedAndColType(const DOSE1_JAWSPEED_ST &speed);
    bool SendMaxJawSpeedAndColType();

    bool SendReadyStatus(const READYSTATE_TYPE_T);
    bool SendClear();

    bool SendMachineType();   //seem not use this command
    bool SendReqBackupParam();  //seem not use this command
    bool SendCalibFunc();     //seem not use this command

    bool RevPressurenTemperature(TransRecvT& revdata);
    bool RevDose1Data(TransRecvT& revdata);
    bool RevLastTransmissionForCompletion(TransRecvT& revdata);
    bool RevParameterBackupData(TransRecvT& revdata);
    bool RevCheckReport(TransRecvT& revdata);
    bool RevResetEcho(TransRecvT& revdata);
    bool RevDose1Offset(TransRecvT& revdata);
    bool RevErrReport(TransRecvT& revdata);
    bool RevPeaks(TransRecvT& revdata);
    bool RevMUGAPTreatmentTime(TransRecvT& revdata);
    bool RevVirtualWedgeJawPosition(TransRecvT& revdata);
    bool RevLastTransmissionForTermination(TransRecvT& revdata);
    bool RevGantryPosition(TransRecvT& revdata);

    void ProcessErrorIL();
    void ProcessCompensationFactorIL();
    void ProcessFlatnessIL();
    bool ProcessCHKIL(TransRecvT& revdata);

    //发送
    USERMODE_TYPE_T  work_mode;
    MACHINE_TYPE_T machine_type;
    CHECKREQ_TYPE_T  check_request;
    READYSTATE_TYPE_T  ready_status;
    JAW_SELECT_T   jaw_select;
    DOSE1_GAIN_ST dose1_gain[3];     //RE_TE_MODE
    //
    DYNAMIC_STEER_ST  dyn_steer[3];      //RE_TE_MODE
    UINT16 preset_treat_time;
    UINT16 dose1_by_pass_mask;   //
    float f_preset_dose1;
    UINT32 preset_dose1;
    UINT16 rot_dose_per_degree; //
    UINT8  gantry_speed;     //
    UINT16 compensate_gain;  //
    UINT16 param_bkup_conver_factor; //M
    //V
    UINT16 max_position_speed[5];

    //LJY-9-23
    UINT16 rot_speed;
    UINT16 rot_rate;
    UINT8 rot_speed_per_sec;



    //接收到
    UINT16 temperature;
    UINT16 pressure;
    float float_temp;
    float float_press;
    UINT32 dose1_act_data;                //dose 1 accumulated dose rate
    UINT16 dose1_aver_rate;               //dose 1 average dose rate
    UINT16 treatment_time;
    UINT16 currx;     //Dynamic jaw posision for virtual wedge treatment
    //Gantry position for rotation treatment
    UINT32 backup_dose_data;
    UINT16 backup_conversion_factor;
    UINT16 backup_time;     //可能可以与treatment_time 合在一起
    UINT16 backup_gantry_position;      //可能可以与cross_plane_steering 合在一起
    UINT8 check_report;
    UINT32 prom_version; //版本号等信息
    UINT16 dose1_rate_offset;  //Transmitted once after READY state transition
    DOSE_ERROR_REPORT_ST error_now;
    UINT16 segment_0;    //Segment 0  read back
    UINT16 segment_1;
    UINT16 segment_3;
    UINT16 MUGAP_time;  //MUGAP treatment time
    UINT16 virtual_wedge_jaw_position;
    UINT16 gantry_position_counts;

    float flatness, symmetry;

    bool press_temp_new;
    bool dose_rate_new;

    UINT16 last_pc_radon_time;
    UINT16 pc_radon_time;
    UINT16 last_treat_time;
    UINT32 dose_aver_rate_count;

private:
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;
    QMutex dosei_mutex;

    USERMODE_TYPE_T mode_now;

    float f_drcv;

    bool item_has[27];

};


extern bool g_is_terminate;         //terminate标志
extern bool g_is_complete;          //complete标志
extern RE_TE_MODE g_mode;
extern QTimer *g_dosei_timer;
#endif // DOSEI_H
