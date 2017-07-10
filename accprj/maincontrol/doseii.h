#ifndef DOSETWO_H
#define DOSETWO_H



#include "slave_base.h"
#include "dosebase.h"

typedef enum {
    CHECK_PASSED = 0x00,
    WATCHDOG_FAILED_CHECK2 = 0x14,
    T1_FAILED_CHECK2 = 0x40,
    T1_WATCHDOG_FAILED_CHECK2 = 0x54,
} CHECKREPORT_TYPE_T;


struct DOSE2_GAIN_BIT
{
    UINT8  elecXRay;
    UINT8  rotationalFixed;
    UINT8  stdSkipScan;
    UINT8  xLowHigh;
    UINT8  inPlaneDynSteer;
};

typedef struct {
    UINT16 gain;
    UINT16 drcv;
    UINT16 dsGainY;
    UINT16 baly;
    //struct DOSE2_GAIN_BIT bitArray;
    UINT8 bitArray;
} DOSE2_GAIN_ST;

typedef struct {
    UINT16 segment0Gain;        //以dose1的seg0为准
    UINT16 segment2Gain;
    UINT16 segment4Gain;
} DOSE2_SEG_GAIN_ST;

typedef struct {
    UINT16 jawX1MaxSpeed;
    UINT16 jawX2MaxSpeed;
    UINT16 jawY1MaxSpeed;
    UINT16 jawY2MaxSpeed;
} DOSE2_JAW_SPEED_ST;

typedef struct{
    UINT8 xx;
    UINT8 yy;
}DOSE2_ERROR_REPORT_ST;

class DoseTwo :public DoseBase
{
public:

    DoseTwo();
    ~DoseTwo();

    bool ReadOwnFile();
    bool ReadElement();
    bool ReadOwnElement();

    bool WriteOwnFile();
    void WriteElement();
    void WriteOwnElement();

    bool CheckSoftpotDataLimit();
    bool CheckReschTreatToDefault(RE_TE_MODE mode);
    bool CheckSoftpotDataIntegrity();

    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);
    UINT32 ComputePresetDose(float rads);
    float ConvertDose(UINT32 mcounts);
    float ConvertDoserate(UINT32 mcounts);



    bool SendC0();
    bool SendUserMode(const  USERMODE_TYPE_T mode);
    bool SendGainAndWorkMode();
    bool SendSegmentGain();//const DOSE2_SEG_GAIN_ST &segment, UINT16 dynSteerWindow);
    bool SendInformation(UINT16 pTime, UINT32 presetDose2, UINT16 byPass);
    bool SendInformation();

    bool SendRadOn(UINT16 compensatedGain);
    bool SendRadOn();

    bool SendInterrupt();
    bool SendTerminateOrComplete();
    bool SendCheckRequest(const CHECKREQ_TYPE_T);
    bool SendReadyStatus(const READYSTATE_TYPE_T);
    bool SendMaxJawSpeedAndColType(const DOSE2_JAW_SPEED_ST &speed, UINT8 collimatorType);
    bool SendMaxJawSpeedAndColType();

    bool AnalysRevFram(TransRecvT& revdata);
    bool RevDose2Offset(TransRecvT& revdata);   //N
    bool RevAccumlatedDose2(TransRecvT& revdata); //D
    bool RevPeaks(TransRecvT& revdata);     //P
    bool RevResetEcho(TransRecvT& revdata); //C
    bool RevCheckReport(TransRecvT& revdata); //W
    bool RevErrReport(TransRecvT& revdata); //E
    bool RevPressurenTemperature(TransRecvT& revdata);  //1
    bool RevJawPositionReport(TransRecvT& revdata); //G
    bool RevLastTransmissionForCompletion(TransRecvT& revdata); //B

    void ComputeCompensateFactor();     //10-16

    void ProcessErrorIL();
    void ProcessFlatnessIL();
    bool ProcessCHKIL(TransRecvT &revdata);
    //发送
    DOSE2_GAIN_ST dose2_gain[3];        //RE_TE_ST

    //Segment gains
    DOSE2_SEG_GAIN_ST dose2_segment_gain[3];    //RE_TE_ST
    UINT16 dyn_steer_window[3];          //DS_WY       //RE_TE_ST

    UINT16 jaw_x1_max_speed;    //
    UINT16 jaw_x2_max_speed;
    UINT16 jaw_y1_max_speed;
    UINT16 jaw_y2_max_speed;

    UINT16 dose2_by_pass_mask;          //
    //V
    UINT16 max_position_speed[5];

    USERMODE_TYPE_T  work_mode;

    CHECKREQ_TYPE_T  check_request;
    READYSTATE_TYPE_T  ready_status;
    UINT16 preset_treat_time;
    float f_preset_dose2;
    UINT32 preset_dose2;
    UINT16 compensate_gain;     //
    UINT32 collimator_type;     //

    //收到
    UINT16 dose2_rate_offset;
    //UINT32 dose2_rate2_offset;
    UINT32 dose2_act_data;  //dose 2 accumlated dose rate
    UINT16 dose2_aver_rate; // dose 2 average dose rate
    UINT16 treatment_time;//
    UINT16 curry;    //In-plane dynamic steering current

    UINT16 segment_0;   //以dose1的seg0为准
    UINT16 segment_2;
    UINT16 segment_4;
    //UINT16 dyn_steer_current;
    UINT32 prom_version;

    //CHECKREPORT_TYPE_T check_report;
    UINT8 check_report;

    DOSE2_ERROR_REPORT_ST error_now;
    UINT16 pressure;
    UINT16 temperature;
    float float_temp;
    float float_press;

    float dose2_compen_factor;        //10-16

    UINT16 jawA1_pos_counts;
    UINT16 jawA2_pos_counts;
    UINT16 jawB1_pos_counts;
    UINT16 jawB2_pos_counts;
    float flatness, symmetry;
    bool press_temp_new;
    bool dose_rate_new;



signals:
public slots:
    void InsertDoseiiAccTimeoutIL();
private:
   QMutex send_frame_num_mutex;
   UINT8 send_frame_num;
   QMutex rev_frame_num_mutex;
   UINT8 rev_frame_num;
   QMutex doseii_mutex;

   USERMODE_TYPE_T mode_now;

    float f_drcv;   //drcv的float值

    bool item_has[24];
};



extern RE_TE_MODE g_mode;
extern QTimer *g_doseii_acc_timer;
#endif // DOSETWO_H
