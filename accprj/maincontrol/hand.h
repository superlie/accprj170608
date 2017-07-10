#ifndef HAND_H
#define HAND_H

#include "slave_base.h"

typedef enum{
    PRESET_POSITION=0x21,
    ACTUAL_POSITION=0x2a,
    UPDATE_POSITION=0x2f,
}POSITION_TYPE_T;

typedef enum{
    NULL_MODE=0x0,
    GANTRY_MODE=0x01,
    COLLIMATOR_MODE=0x02,
    SYN_JAWA_MODE=0x03,
    JAWA_1_MODE=0x04,
    JAWA_2_MODE=0x05,
    SYN_JAWB_MODE=0x06,
    JAWB_1_MODE=0x07,
    JAWB_2_MODE=0x08,
}CTRL_MODE_T;

typedef enum{
    NULL_START,
    COLLIMATOR_START,
    SYN_JAWA_START,
    JAWA_1_START,
    JAWA_2_START,
    SYN_JAWB_START,
    JAWB_1_START,
    JAWB_2_START,
}COLLI_JAW_START_FLAG_T;


typedef enum
{
    NO_SPEED=0x0,
    FAST_SPEED=0x2b,
    SLOW_SPEED=0x2d,
}MOTOR_REQUEST_SPEED_T;



typedef enum{
    NATIVE_ZXT,
    IEC_ZXT,
}ZXT_T;

typedef enum{
    AB_JAW_LABELING,
    XY_JAW_LABELING,
}JAW_LABELING_T;

typedef enum{
    NON_MLC,
    MLC,
}MLC_T;

typedef enum{
    OPEN_JAWS,
    CLOSE_JAWS,
}JAWS_MODE_T;

typedef enum{
    CW_GANTRY,
    CCW_GANTRY,
}GANTRY_MODE_T;

typedef struct{
    UINT8 tens_digit;
    UINT8 uints_digit;
}MLC_BANK_ST;


class HandCon : public SlaveBase
{
public:
    HandCon();
    ~HandCon();

    bool ReadOwnFile();
    bool ReadOwnElement();
    bool WriteOwnFile();
    bool WriteOwnElement();
    bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);

    bool SendC0();
    bool SendInvalidPosition();//20h
    bool SendValidPosition();
    bool SendPosition(float position,POSITION_TYPE_T type,CTRL_MODE_T ctrlmode);//
    //ctrlmode=01/02/06

    //    bool SendConfiguration(UINT8 zxt,
    //                          UINT8 jawLabeling,
    //                          UINT8 mlc,
    //                          UINT8 language);
    bool SendConfiguration();
    bool SendDeselectCurTableFunction();
    bool SendUserMode(const USERMODE_TYPE_T mode);


    bool AnalysRevFram(TransRecvT& revdata);

    void ProcessErrorIL();
    bool ProcessCHKIL(TransRecvT& revdata);
public:
    void UpdatePositionToHand(void);//在motor接收到31命令返回position时调用

public:
    bool hand_gantry_control;
    bool hand_colli_control;
    bool hand_jawx1_control;
    bool hand_jawx2_control;
    bool hand_jawy1_control;
    bool hand_jawy2_control;

    //bool field_light_open;//11-27
public:
    bool gantry_rotat_mode;

private:
    bool RevGantryRotationMode(TransRecvT& revdata);
    bool RevColimatorRotationMode(TransRecvT& revdata);
    bool RevJawAMode(TransRecvT& revdata);
    bool RevJawBMode(TransRecvT& revdata);
    bool RevBeamShield(TransRecvT& revdata);
    bool RevFieldLightToggle(TransRecvT& revdata);
    bool RevRoomLightToggle(TransRecvT& revdata);
    bool RevRangeFinderToggle(TransRecvT& revdata);
    bool RevBackPointerToggle(TransRecvT& revdata);
    bool RevMLCBankSelect(TransRecvT& revdata);
    bool RevMLCSpeedSelect(TransRecvT& revdata);
    bool RevDisplayMLCToggle(TransRecvT& revdata);
    bool RevClearMLCInterlocks(TransRecvT& revdata);
    bool RevEnableDeadmanNoSelect(TransRecvT& revdata);
    bool RevEnableDeadmanSelected(TransRecvT& revdata);
    bool RevMLCModeToggle(TransRecvT& revdata);
    bool RevRequestActOrPreData(TransRecvT& revdata);
    bool RevKeyReleaseNotifi(TransRecvT& revdata);
    bool RevMotorSpeedRequest(TransRecvT& revdata);
    bool RevTablePositionRequest(TransRecvT& revdata);
    bool RevOpenJaw(TransRecvT& revdata);
    bool RevCloseJaw(TransRecvT& revdata);
    bool RevCW(TransRecvT& revdata);
    bool RevCCW(TransRecvT& revdata);
    bool RevPrePositData(TransRecvT& revdata);
    bool RevErrorReport(TransRecvT& revdata);
    bool RevResetEcho(TransRecvT& revdata);
    bool RevDeSelctCurHandCtr(TransRecvT& revdata);


    //void GantryPosFtoUINT16(float &preset_position,UINT16 &gantry_position);
    void ClearGantryFlags(void);

    //发送
    ZXT_T zxt;      //
    JAW_LABELING_T jaw_labeling;    //
    MLC_T mlc;      //

    //接收

    bool collim_rotat_mode;
    bool jawa_mode;
    bool jawb_mode;

    bool req_act_or_pre_data;
    UINT8 key_release;
    UINT8 motor_speed_request;
    float preset_position;//手柄返回的预设目标值no-ascii
    COLLI_JAW_START_FLAG_T collimatorOrjaw_start;//Collimator and Jaw start flags
    CTRL_MODE_T gan_coli_jaw_mode;
    int gantry_enable_count;

    bool beam_shield_mode;
    bool field_light_tog;
    bool room_light_tog;
    bool range_finder_tog;
    bool back_pointer_tog;
    int mlc_bank_1;
    int mlc_bank_2;
    UINT8 mlc_speed_mode_sel;
    bool console_mlc_disp;
    bool clear_mlc_interlock;
    bool enable_active;
    bool mlc_block_mode;

    bool have_send_movenow;//11-26




    UINT8 table_position_request;
    bool enable_active_and_motion;
    JAWS_MODE_T jaws_mode;
    GANTRY_MODE_T gantry_mode;
    UINT8 error_report;
    UINT32 prom_version;
    bool deselect;
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;
    QMutex hand_mutex;
    USERMODE_TYPE_T mode_now;

    // UINT8 language;

};


#endif // HAND_H
