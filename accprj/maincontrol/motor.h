#ifndef MOTOR_H
#define MOTOR_H

#include "slave_base.h"

typedef enum {
    JAW_A1 = 0,     //y1
    JAW_A2,         //y2
    JAW_B1,         //x1
    JAW_B2,         //x2
    COLLIMATOR,
    GANTRY,
    ALL_MOTORS = 0xFF,
} MOTOR_PARTS_T;

typedef enum {
    CHECKPASSED = 0,
    RAMFAILED,
    ROMFAILED,
    WATCHDOGFAILED = 4,
}CHECKREPORT_MOTOR_T;

typedef enum {
    T1_ACTIVE_LOW = 0,
    T1_INACTIVE_HIGH,
} LINE_STATUS_T;

typedef enum{
    ERR_MASK1 = 0x00,
    ERR_MASK2 = 0x01,
}ERRINDEX_T;

typedef enum{       //限束头类型
    COLLI_180,
    COLLI_360,
}COLLI_TYPE_T;

typedef struct {
    UINT8 error_mask_bytes[7];
} ERRORMASK_ST;

typedef struct{
    UINT8 xx;
    UINT8 yy;
    UINT8 zz;
}ERROR_REPORT_ST;

typedef struct {
    UINT16 gantry_position;
    UINT8 sampled_speed;
    UINT8 expected_speed;
} GANTRY_POSITION_SPEED_ST;

typedef struct{
    UINT16 position;
    UINT16 timestamp;       //motor speed test timestamp
}SPEED_TEST_ST;

typedef struct{
    UINT16 requested_position;
    UINT16 actual_position;
}SPEED_COMPLETE_ST;


typedef struct{
    UINT16 target_position;
    UINT16 move_speed;
}MOVE_TARGET_SPEED_ST;

typedef struct{
    UINT16 target_position1;
    UINT16 target_position2;
    UINT16 speed;
}CALIBRATION_SPEED_ST;

typedef struct{
    UINT16 speed;
    UINT16 target;
}RADON_SPEED_TARGET_ST;

typedef struct{
    UINT16 speed;
    UINT16 position1;
    UINT16 position2;
}TEST_TARGET_SPEED_ST;


class Motor : public SlaveBase
{
    //Q_OBJECT
public:
    Motor();
    ~Motor();

    bool ReadOwnFile();
    bool ReadOwnElement();

    bool WriteOwnFile();
    bool WriteOwnElement();

    bool CheckCalibDataIntegrity();
    bool CheckMotorSpeedData();
    bool CheckGantrySpeedFactor();

    void ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
    UINT8 ComputeRevCHK(UINT8* data, UINT8 len);

    /*Send (console to motor)*/
    bool SendC0();
    bool SendSlowDownSpeed();    //发送slowdown速度
    bool SendSlowDownDistance();
    bool SendUserMode(const USERMODE_TYPE_T mode);
    bool SendAccept();
    bool SendErrorMask();   //error mask对应7字节的YY
    bool SendInterrupt();
    bool SendMoveNow(const MOTOR_PARTS_T part, const UINT16 target);
    bool SendMoveNow(const MOTOR_PARTS_T part);
    bool SendMoveNow(const MOTOR_PARTS_T part, const UINT16 target, UINT8 speed);

    bool SendSpeedCalibration(const MOTOR_PARTS_T part, const UINT16 target_postion1,
                              const UINT16 target_position2, const UINT16 speed);
    //bool SendMoveInRadOn(const MOTOR_PARTS_T part, const UINT16 postion);
    bool SendMoveInRadOn();

    bool SendSpeedTest(const MOTOR_PARTS_T part, const UINT16 target_position1,
                       const UINT16 target_position2, const UINT16 speed);    //RevSpeedTest
    bool SendMaxJawSpeedColliType(const UINT16 speed, const MOTOR_PARTS_T part);
    bool SendMaxJawSpeedColliType();

    bool SendCheckRequest(const CHECKREQ_TYPE_T index);
    bool SendStop(const MOTOR_PARTS_T);                                  //stop jaw, colli, gantry, or all motors.
    bool SendReadyStatus(const READYSTATE_TYPE_T status);

    void CheckMotorSetActPos();

    /*Receive (motors to console)*/
    bool AnalysRevFram(TransRecvT& revdata);       //收到并且解析命令，分配到下面函数中处理
    bool RevJawColliPosition(TransRecvT& revdata);
    bool RevGantryPosnSpeed(TransRecvT& revdata);


    //返回部件的ID, 回应SendSpeedTest();
    bool RevSpeedTest(TransRecvT& revdata);
    bool RevResetEcho(TransRecvT& revdata);
    bool RevErrorReport(TransRecvT& revdata);

    //更新 gantry_position
    bool RevGantryCompletionInRadOn(TransRecvT& revdata);
    bool RevVirtualWedgeSpeedTestComplete(TransRecvT& revdata);
    bool RevSpeedTestComplete(TransRecvT& revdata);
    bool RevCheckReport(TransRecvT& revdata);
    //LINE_STATUS_T
    bool RevEmergencyLineStatus(TransRecvT& revdata);

    void ProcessGantryPosIL();
    void ProcessErrorReportIL();
    void ProcessJawnColliPositionIL();
    bool ProcessCHKIL(TransRecvT& revdata);

    //12-01
    bool CheckConvertGantry(float preset_gpos, UINT32& gantry_pst);

private slots:
    //void InsertGantryTimeoutIL();

public:
    //private:
    /*Param for send*/

    UINT8 slow_down_speed[6];       //motor slow down speed
    UINT8 slow_down_distance[6];    //motor slow-down distance from target position
    UINT16 max_position_speed[5];   //Maximum jaw speeds, collimator type

    /*motors speed & target position for move now */
    MOVE_TARGET_SPEED_ST move_target_speed[6];

    /*Vritual Wedge Motor Speed Calibration Test*/
    CALIBRATION_SPEED_ST calibration_speed[4];

    /*motor speed tests*/
    TEST_TARGET_SPEED_ST test_target_speed[4];

    /*move in RAD ON*/
    RADON_SPEED_TARGET_ST radon_speed_target[6];

    /*Slow speed*/
    UINT16 slow_speed[6];

    /*Fast speed*/
    UINT16 fast_speed[6];


    /*Param for compute*/
    float jaw_y1_gain;
    float jaw_y1_offset;
    float jaw_y2_gain;
    float jaw_y2_offset;
    float jaw_x2_gain;
    float jaw_x2_offset;
    float jaw_x1_gain;
    float jaw_x1_offset;
    float colli_gain;
    float colli_offset;
    float gantry_gain;
    float gantry_offset;

    COLLI_TYPE_T colli_type;

    /*Param for received*/
    /*received motors positions*/
    UINT16 position[5];

    //Motor Speed test ID, position & timestamp
    SPEED_TEST_ST speed_test[6];

    //Motor Speed Test Complete
    SPEED_COMPLETE_ST speed_complete[6];
    //Virtual Wedge Motor Speed Calibration Test Complete
    SPEED_COMPLETE_ST speed_virtual_wedge_complete[4];

    USERMODE_TYPE_T mode_now;
    READYSTATE_TYPE_T ready_status_now;
    UINT32 prom_version;   //版本号等信息

    ERRORMASK_ST error_mask_1;
    ERRORMASK_ST error_mask_2;

    GANTRY_POSITION_SPEED_ST gantry_position_speed;   //Gantry position and speed   cmd'2'
    GANTRY_POSITION_SPEED_ST gantry_completion;     //Gantry completion in RAD ON   cmd'K'
    //ERROR_REPORT_ST error_list[ERRLISTLEN];        //错误列表
    //QQueue<ERROR_REPORT_ST> error_queue;
    ERROR_REPORT_ST error_now;                 //刚刚报告的错误

    CHECKREPORT_MOTOR_T check_report;

    LINE_STATUS_T line_status;            //Emergency line status
    CHECKREQ_TYPE_T check_index;

    float gantry_speed_factor_cw;
    float gantry_speed_factor_ccw;

    //LJY-9-23
    UINT32 rot_stop_position;
    UINT32 rot_start_position;

    //LJY-12-12
    UINT16 jawx1_max_toler;
    UINT16 jawx2_max_toler;
    UINT16 jawy1_max_toler;
    UINT16 jawy2_max_toler;
    UINT16 colli_max_toler;
    UINT16 gantry_max_toler;

    //ACC2
    UINT32 jawx1_15_3_pos;      //15+0.3
    UINT32 jawx2_15_3_pos;
    UINT32 jawy1_10_3_pos;      //10+0.3
    UINT32 jawy2_10_3_pos;
    UINT32 jawy1_12_8_pos;      //12.5+0.3
    UINT32 jawy2_12_8_pos;

    //ACC3
    UINT32 jawx1_6_5_pos;       //EA205 13*13
    UINT32 jawx2_6_5_pos;
    UINT32 jawy1_6_5_pos;
    UINT32 jawy2_6_5_pos;

    UINT32 jawx1_9_5_pos;       //EA210 19*19
    UINT32 jawx2_9_5_pos;
    UINT32 jawy1_9_5_pos;
    UINT32 jawy2_9_5_pos;

    UINT32 jawx1_11_5_pos;      //EA215 23*23
    UINT32 jawx2_11_5_pos;
    UINT32 jawy1_11_5_pos;
    UINT32 jawy2_11_5_pos;

    UINT32 jawx1_13_5_pos;      //EA220 27*27
    UINT32 jawx2_13_5_pos;
    UINT32 jawy1_13_5_pos;
    UINT32 jawy2_13_5_pos;

    UINT32 jawx1_16_pos;        //EA225 32*32
    UINT32 jawx2_16_pos;
    UINT32 jawy1_16_pos;
    UINT32 jawy2_16_pos;
private:
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;
    QMutex motor_mutex;
    //bool interrupt;          //中断射线的标志

    bool init_jawcolli_target_pos;       //Used in CheckMotorSetAct
    bool init_gantry_target_pos;

    bool gain_offset_has[12];

};

extern bool g_is_rotate_treatment;
extern    QTimer *g_gantry_timer;
#endif // MOTOR_H
