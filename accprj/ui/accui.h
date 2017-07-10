#ifndef ACCUI_H
#define ACCUI_H

#include <QWidget>
#include <QKeyEvent>
#include <QTableWidget>
#include <QThread>
#include <QPointer>
#include <QDir>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "keythread.h"
#include "ui/spkey.h"
#include "ui/clineedit.h"
#include "maincontrol/mastercontroller.h"
#include "maincontrol/remotecontroller.h"
#include "maincontrol/slave_base.h"
#include "language.h"
#include "csegprogress.h"
#include "cdoseprogress.h"


#define CPRESEARCH  0
#define CPDEFAULT   1
#define CPTREATMENT 2

extern QString g_max_dose_rate;
extern QString g_rad_on_delay_time;
extern bool g_is_remote_mode;
extern RemoteController *remote;

struct LAST_TREAT_DATA {
    RAY_TYPE_T ray_type;
    float dose1;
    float dose2;
    float time;
    float rate;
    float jawx1;
    float jawx2;
    float jawy1;
    float jawy2;
    float colli;
    float gpos;
    float arc;
    float stop;
    float gspeed;
    QString acc1;
    QString acc2;
    QString acc3;
};

typedef struct {
    bool colli;
    bool jawx;
    bool jawy;
    bool jawx1;
    bool jawx2;
    bool jawy1;
    bool jawy2;
}MOVE_STATUS_ST;

const QString svc_copy[3] = {"RESEARCH","DEFAULT","TREATMENT"};
typedef enum
{
    MODENULL,
    START,
    TREAT,
    SVC,
    STDBY
} MODE_T;

typedef enum
{
    TREAT_MODE,
    TREAT_PARAMETER,
    TREAT_ENERGY,
    TREAT_DOS_RATE,
    TREAT_DOS_TIME,
    TREAT_PST_DOS1,TREAT_PST_DOS1_SURE,TREAT_PST_DOS1_RE,
    TREAT_GPOS_VAL,
    TREAT_GPOS,
    TREAT_GAN_MOVE,
    TREAT_COLL_JAW,
    TREAT_COLL,
    TREAT_SYNCJ,
    TREAT_JAWX,
    TREAT_JAWY,
    TREAT_INDEPJ,
    TREAT_JAWX1,
    TREAT_JAWX2,
    TREAT_JAWY1,
    TREAT_JAWY2,
    TREAT_ROT,
    TREAT_ROT_DOSE1,TREAT_ROT_DOSE1_RE,             //TREAT_ROT_DOSE1
    TREAT_ROT_GPOS,              //TREAT_ROT_GPOS
    TREAT_ROT_ARC,             //TREAT_ROT_ARC
    TREAT_ROT_PARAMETER,        //LJY-9-25
    TREAT_ELTRN,
    TREAT_COLL_MOVE

    //TREAT_NULL  //LJY-9-9 用于屏蔽Rad On时部分操作
} SELECT_T;

typedef enum
{
    SVC_NULL,
    SVC_START,
    SVC_MENU,
    SVC_B_SOFTPOT,
    SVC_C_INTERLOCK,
    SVC_D_ERR_DISP,
    SVC_E_CMPSATION_FCTOR,
    SVC_F_VERIF_ENABLE,
    SVC_G_MEAN_ATTENUAT,
    SVC_H_SIP_ALYZER,
    SVC_I_MTOR_ADJST,
    SVC_J_CK_DISP,
    SVC_K_T1_DISP,
    SVC_L_PROHBIT_SECT,
    SVC_M_FOIL_SLCT,
    SVC_N_MTOR_CALIBRAT,
    SVC_O_RCAL_CONFIG,
    SVC_P_SET_COLI_TYPE,
    SVC_Q_GANT_SPD_CALIBRAT,
    SVC_R_SOFTPOT_UP_DL,
    SVC_S_SVC_WARN,
    SVC_T_TAP_SLT,
//    SVC_MODE,
//    SVC_PST_DOS1,
//    SVC_PARAMETER,
    SVC_TREAT
} SVC_T;

typedef enum
{
    KEY_0   = 0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,
    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,

    KEY_MINUS,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F10_RELEASE,
    KEY_F11,
    KEY_F12,
    KEY_F13,
    KEY_F14,
    KEY_F14_RELEASE,

    KEY_PERIOD,
    KEY_RETURN,
    KEY_RESET,
    KEY_TREAT,
    KEY_SVC,
    KEY_PRGRDY,
    KEY_BACKSPACE,
    KEY_SETACT,
    KEY_CLEAR,
    KEY_STDBY,
    KEY_ACCEPT,
    SWITCH_RADON,
    KEY_RADON,
    KEY_RADOFF,
    KEY_STOP,

    KEY_PAGEUP,
    KEY_PAGEDOWN,

    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,

    KEY_ESC
} KEYBOARD_T;

typedef enum
{
    BYPASS_NO1              = 0x0000,
    BYPASS_DOSE_RATE1       = 0x0001,       // interlock 12
    BYPASS_TREAT_TIME1      = 0x0002,       // interlock 2
    BYPASS_DOSE1            = 0x0004,       // interlock 56
    BYPASS_DOSE_PER_DEGREE1 = 0x0008,       // interlock 82
    BYPASS_CAP1             = 0x0010,       // interlock 93
    BYPASS_DOSE_SATURATION1 = 0x0020,       // interlock 58
    BYPASS_DOSE_RATE_OFFSET1= 0x0040,       // interlock 57
    BYPASS_PT_SATURATION1   = 0x0080,       // interlock 59
    BYPASS_DOSE_CALC1       = 0x0100        // interlock 97

} DOSE1_BYPASS_MASK_T;

typedef enum
{
    BYPASS_NO2              = 0x0000,
    BYPASS_DOSE_RATE2       = 0x0001,       //interlock 13
    BYPASS_TREAT_TIME2      = 0x0002,       //interlock 2
    BYPASS_DOSE2            = 0x0004,       // interlock 1

    BYPASS_DOSE_SATURATION2 = 0x0020,       // interlock 61
    BYPASS_DOSE_RATE_OFFSET2= 0x0040,       // interlock 60
    BYPASS_PT_SATURATION2   = 0x0080,       // interlock 59
    BYPASS_DOSE_CALC2       = 0x0100        // interlock 97
} DOSE2_BYPASS_MASK_T;

//---------------------------
typedef enum
{
    SOFTPOT_COPY_FROM,
    SOFTPOT_COPY_TO,
    SOFTPOT_COPY_INCLUDE,
    SOFTPOT_COPY_SUBMIT
} SOFTPOT_COPY_T;
//---------------------------

extern UINT32 g_il_state;

namespace Ui {
class AccUi;
}

class AccUi : public QWidget
{
    Q_OBJECT

public:
    explicit AccUi(QWidget *parent = 0);
    ~AccUi();

public:
    void SetActDos1(UINT32 val);
    void SetActDos2(UINT32 val);
    void SetActDosTime(float val);
    void SetActDosRate(UINT32 val);
    void SetActCollimat(float val);
    void SetActJawX(float jawx1_act_pos, float jawx2_act_pos);
    void SetActJawY(float jawy1_act_pos, float jawy2_act_pos);
    void SetActJawX1(float val);
    void SetActJawX2(float val);
    void SetActJawY1(float val);
    void SetActJawY2(float val);
    void SetActGanPos(float val);

    //2016-8-24
    void CurrentPositAsTarget();

    void UpdateFuncKey();       //更新显示F1-F6功能键

    void InitLastTreatData();
    void SaveLastTreatData();
    void RecallLastTreatData();
    bool ReadLastDataFile();
    bool ReadLastDataElement();
    bool WriteLastDataFile();
    void WriteLastDataElement();
    void ShowLastData();

    //beam1
    void SetActAfcm(UINT32 val);
    void SetActElmi(float val);
    void SetActBmis(float val);
    void SetActStci(UINT32 val);
    void SetActLeni(float val);
    //beam2
    void SetActDscx(UINT32 val);
    void SetActDscy(UINT32 val);
    //dos1
    void SetActDr1(UINT32 val);
    void SetActDr2(UINT32 val);
    void SetActOff1(UINT32 val);
    void SetActOff2(UINT32 val);
    //dos2
    void SetActSeg0(UINT32 val);
    void SetActSeg1(UINT32 val);
    void SetActSeg2(UINT32 val);
    void SetActSeg3(UINT32 val);
    void SetActSeg4(UINT32 val);
    void SetActSxe(float sxe);
    void SetActSye(float sye);
    void SetActFxe(float fxe);
    void SetActFye(float fye);

    void SetActTemp1(float val);
    void SetActTemp2(float val);
    void SetActPress1(float val);
    void SetActPress2(float val);

    //J  Check Display 检查显示
    void SetActChkDispReport0(UINT8 val);
    void SetActChkDispReport1(UINT8 val);
    void SetActChkDispReport2(UINT8 val);
    void SetActChkDispReport3(UINT8 val);
    void SetActChkDispReport5(UINT8 val);
    void SetActChkDispReport6(UINT8 val);
    void SetActChkDispReport7(UINT8 val);

    //K  T1 Display  T1显示
//    void SetActT1DispReport0(UINT8 val);
//    void SetActT1DispReport1(UINT8 val);
//    void SetActT1DispReport2(UINT8 val);
//    void SetActT1DispReport3(UINT8 val);
//    void SetActT1DispReport4(UINT8 val);
//    void SetActT1DispReport5(UINT8 val);
//    void SetActT1DispReport6(UINT8 val);
//    void SetActT1DispReport7(UINT8 val);

    //Softpot Upload\Download
    void SoftpotFtpUpload();
    void SoftpotFtpDownload();

    void SetRotSpeedRateTime();//LJY-9-23



    //duanzhibin-----------9/24 显示accessory
    void SetAccessoy_2_Disp(ACCESSORY_A2_TYPE temp,ACCESSORY_2_DIC dir);
    void SetAccessoy_1_Disp(ACCESSORY_A1_TYPE temp);
    void Display_Acc_3();

private:
   void NormalSetStatus();
   void RemoteSetStatus();
   void AllLineEditStopBlink();

private slots:
    void SetAccessoy_3_Disp(ACCESSORY_B_TYPE temp);
    void SetStatus();
    void ShowPromptMsg(QString str);//LJY-9-15
    void ShowRemoteMsg(QString msg);
    void ProcessShowUI();
    void ShowActJawColliGan(int type);
    void ShowActDoseTimeMU();
    void ShowActSvcValue(SVC_LINEEDIT type);

    void Interlock_Redisplay();

    void ShowGantryDirec(bool cw);

    void downloadFinished(QNetworkReply *reply);
    void uploadFinished(QNetworkReply *reply);

//set preset value to uppermonitor
private slots:
    void SetPresetDos1(QString value);
    void SetPresetEnergy(QString value);
    void SetPresetTech(QString value);
    void SetPresetDosrate(QString value);
    void SetPresetAcc1(QString value);
    void SetPresetAcc2(QString value);
    void SetPresetAcc3(QString value);
    /*void SetPresetX1Pos(QString value);
    void SetPresetX2Pos(QString value);
    void SetPresetY1Pos(QString value);
    void SetPresetY2Pos(QString value);
    void SetPresetGantry(QString value);
    void SetPresetColli(QString value);*/

public:
    void SetInterlockTableItem(int num, bool ovrd,INTERLOCK_STATUS_T status, int error, UINT32 history);


public:
    QString tmp_value;
    QString tmp_value_2;
    QString tmp_finish_value;
    QList <QLineEdit*> softpot_widge[5];
    QList <QLineEdit*> motor_adjust[4];
    QList <QLineEdit*> svc_waring;
    bool has_press_return;
    bool has_press_return_2;
    int  svc_warn_size;
    int  motadj_column;
    int  motadj_row;

    UINT16 headdata;    //LJY 5-12-11
    UINT16 bitsdata;

    bool collimat_act;
    bool gantry_act;



    QStringList errlist[130];
    QStringList ilckinfolist;
    QStringList interlock_list;
    QStringList file_tmp;
    QStringList clientfile;
    QStringList serverfile;
    int last_status;
    QString code[0xff];                 //按键及代码

    MODE_T g_treat_svc_mode;

    LAST_TREAT_DATA lastdata;

    QPointer<QWidget> w;

    bool input_lock;         //输入标志

public slots:
    void CollimatMoveComplet();
    void GantryMoveComplet();

    void autosetTreatdataWithKey(int key);
    void autosetTreatdataWithData(int lineedit, QString str);
    void showTreatUI();
    void resetUISelectStep();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private slots:
    //B
    void on_leSoftpotsBeam1AfcpRsch_returnPressed();
    void on_leSoftpotsBeam1InjeRsch_returnPressed();
    void on_leSoftpotsBeam1InjiRsch_returnPressed();
    void on_leSoftpotsBeam1ElmRsch_returnPressed();
    void on_leSoftpotsBeam1BmiRsch_returnPressed();
    void on_leSoftpotsBeam1StcRsch_returnPressed();
    void on_leSoftpotsBeam1LensRsch_returnPressed();
    void on_leSoftpotsBeam1PfnRsch_returnPressed();
    void on_leSoftpotsBeam1IpfnRsch_returnPressed();
    void on_leSoftpotsBeam1SominRsch_returnPressed();
    void on_leSoftpotsBeam2DsgxResrch_returnPressed();
    void on_leSoftpotsBeam2DsgyResrch_returnPressed();
    void on_leSoftpotsBeam2DswxResrch_returnPressed();
    void on_leSoftpotsBeam2DswyResrch_returnPressed();
    void on_leSoftpotsBeam2BalxResrch_returnPressed();
    void on_leSoftpotsBeam2BalyResrch_returnPressed();
    void on_leSoftpotsBeam2ElmckResrch_returnPressed();
    void on_leSoftpotsBeam2BmickResrch_returnPressed();
    void on_leSoftpotsBeam2StcckResrch_returnPressed();
    void on_leSoftpotsBeam2LenckResrch_returnPressed();
    void on_leSoftpotsTrigModtResrch_returnPressed();
    void on_leSoftpotsTrigInjtResrch_returnPressed();
    void on_leSoftpotsTrigAfctResrch_returnPressed();
    void on_leSoftpotsTrigRftResrch_returnPressed();
    void on_leSoftpotsTrigTst0Resrch_returnPressed();
    void on_leSoftpotsTrigTst1Resrch_returnPressed();
    void on_leSoftpotsDose1D1gResrch_returnPressed();
    void on_leSoftpotsDose1D2gResrch_returnPressed();
    void on_leSoftpotsDose1PrfpResrch_returnPressed();
    void on_leSoftpotsDose1DrcvResrch_returnPressed();
    void on_leSoftpotsDose2S0gResrch_returnPressed();
    void on_leSoftpotsDose2S1gResrch_returnPressed();
    void on_leSoftpotsDose2S2gResrch_returnPressed();
    void on_leSoftpotsDose2S3gResrch_returnPressed();
    void on_leSoftpotsDose2S4gResrch_returnPressed();
    void on_leSoftpotsDose2SwiResrch_returnPressed();
    void on_leSoftpotsDose2FwiResrch_returnPressed();
    void on_leSoftpotsDose2DostResrch_returnPressed();

    //C interlock
    void InterlockSetOvrd(int num, bool setvalue );
    void ShowInterlockError(int num);


    //E 补偿因子菜单
    void on_leCpsFactor_returnPressed();
    void on_leCpsFactorRefTemp_returnPressed();
    void on_leCpsFactorRefPress_returnPressed();
    //G 平均衰减
    void on_leMeanAttLoCalibrFact_returnPressed();
    void on_leMeanAttHiCalibrFact_returnPressed();
    //I 电机调节菜单
    void on_leMotAdjJawY1SDS_returnPressed();
    void on_leMotAdjJawY2SDS_returnPressed();
    void on_leMotAdjJawX2SDS_returnPressed();
    void on_leMotAdjJawX1SDS_returnPressed();
    void on_leMotAdjColliSDS_returnPressed();
    void on_leMotAdjGantrySDS_returnPressed();

    void on_leMotAdjJawY1SDD_returnPressed();
    void on_leMotAdjJawY2SDD_returnPressed();
    void on_leMotAdjJawX2SDD_returnPressed();
    void on_leMotAdjJawX1SDD_returnPressed();
    void on_leMotAdjColliSDD_returnPressed();
    void on_leMotAdjGantrySDD_returnPressed();

    void on_leMotAdjJawY1SS_returnPressed();
    void on_leMotAdjJawY2SS_returnPressed();
    void on_leMotAdjJawX2SS_returnPressed();
    void on_leMotAdjJawX1SS_returnPressed();
    void on_leMotAdjColliSS_returnPressed();
    void on_leMotAdjGantrySS_returnPressed();

    void on_leMotAdjJawY1FS_returnPressed();
    void on_leMotAdjJawY2FS_returnPressed();
    void on_leMotAdjJawX2FS_returnPressed();
    void on_leMotAdjJawX1FS_returnPressed();
    void on_leMotAdjColliFS_returnPressed();
    void on_leMotAdjGantryFS_returnPressed();

    //M  Foil Selection  散射箔选择 设置新值
    void on_leFoilSelectNewElecEgy1_returnPressed();
    void on_leFoilSelectNewElecEgy2_returnPressed();
    void on_leFoilSelectNewElecEgy3_returnPressed();
    void on_leFoilSelectNewElecEgy4_returnPressed();
    void on_leFoilSelectNewElecEgy5_returnPressed();
    void on_leFoilSelectNewElecEgy6_returnPressed();
    //----------------------------------------------

    //----------------------------------------------
    //Ljy-8-6
    //T  Tap Selection  抽头选择菜单
    void on_leTapSelectNewE1_returnPressed();
    void on_leTapSelectNewE2_returnPressed();
    void on_leTapSelectNewE3_returnPressed();
    void on_leTapSelectNewE4_returnPressed();
    void on_leTapSelectNewE5_returnPressed();
    void on_leTapSelectNewE6_returnPressed();
    void on_leTapSelectNewXL_returnPressed();
    void on_leTapSelectNewXH_returnPressed();
    void on_leTapSelectNewFILM_returnPressed();
    //----------------------------------------------
    //Q Gantry Speed Calibration
    void on_leGantrySpdFactClk_returnPressed();
    void on_leGantrySpdFactCountClk_returnPressed();

    //S Svc warning
    void on_leSvcWarnBI1_returnPressed();
    void on_leSvcWarnBI2_returnPressed();
    void on_leSvcWarnJawA1_returnPressed();
    void on_leSvcWarnJawA2_returnPressed();
    void on_leSvcWarnJawB1_returnPressed();
    void on_leSvcWarnJawB2_returnPressed();

public:
    void resetMoveStatus();

private:
    bool checkActPstMatch(float act, float pst, float delta);

    void ShowChineseLabel();
    bool Shutdown();
    void ShowAccStart();
    void ShowAccReset();
    void ShowTreat();
    void ShowSvc();
    //void ShowJaws();
    void ShowJawX1X2();
    void ShowJawY1Y2();
    void ShowRotGans();

    void HideJawX1X2();
    void HideJawY1Y2();

    void ShowFoilSelect();  //Foil Select UI
    void ShowMotorAdjust();
    void ShowMotorCalibFocus();
    void ShowMotorCalibGainOffset();
    void ShowTapSelect();

    void ShowFuncMode();
    void ShowFuncFrame();
    void ShowFuncClear();
    void ShowFuncRay();
    void ShowFuncPara();
    void ShowFuncJawXY();
    void ShowFuncY1Y2();
    void ShowFuncX1X2Y1Y2();
    void ShowFuncCollJaw();
    void ShowFuncRot();
    void ShowFuncRotPar();
    void ShowFuncEltrn();



    void ProcessStart(QKeyEvent *event);

    //void ProcessStartF7(QKeyEvent *event);
    void ProcessStdByKey();
    void ProcessInterlockMsgDown();
    void ProcessInterlockMsgUp();

    void ProcessStdByState(QKeyEvent *event);
    void ProcessRadOnAltS(QKeyEvent *event);

    void ProcessTreat(QKeyEvent *event);
    void ProcessTreatF1();
    void ProcessTreatF2();
    void ProcessTreatF3();
    void ProcessTreatF4();
    void ProcessTreatF5();
    void ProcessTreatF6();
    void ProcessSetAct();
    void ProcessTreatF12();
    void ProcessTreatReturn();
//    void ProcessTreatReset();
    void ProcessAltEnable();

    //treatset.cpp
    void SendPresetDos1();
    void SendPresetDos2();
    void SendPresetTimer();
    void SendJawX();
    void SendJawY();
    void SendJawX(float jawx);       //only used by acc3
    void SendJawY(float jawy);       //only used by acc3
    void SendJawX1();
    void SendJawX2();
    void SendJawY1();
    void SendJawY2();
    void SendCollimator();
    void SendGantryStart();
    void SendGantryStop();

    void LineeditStopBlink();


    void ProcessSvc(QKeyEvent *event);
    void ProcessSvcStart(QKeyEvent *event);
    void ProcessSvcMenu(QKeyEvent *event);
    void ProcessSvcSoftpot(QKeyEvent *event);
    void ProcessSvcBhBeam1(QKeyEvent *event);
    void ProcessSvcBiBeam2(QKeyEvent *event);
    void ProcessSvcBjTrig(QKeyEvent *event);
    void ProcessSvcBkDos1(QKeyEvent *event);
    void ProcessSvcBlDos2(QKeyEvent *event);
    void ProcessSvcCInterlock(QKeyEvent *event);
    void ProcessSvcDErrDisp(QKeyEvent *event);
    void ProcessSvcECmpstFct(QKeyEvent *event);
    void ProcessSvcFVerif(QKeyEvent *event);
    void ProcessSvcGMeanAtt(QKeyEvent *event);
    void ProcessSvcHSipAnaly(QKeyEvent *event);
    void ProcessSvcIMotorAdj(QKeyEvent *event);
    void ProcessSvcJChkDisp(QKeyEvent *event);
    void ProcessSvcKT1Disp(QKeyEvent *event);
    void ProcessSvcLProhbSec(QKeyEvent *event);
    void ProcessSvcMFoilSel(QKeyEvent *event);

    void ProcessSoftpotSelectPage();
    void ProcessSoftpotSwitchPage();    //未完成输入切换页面，恢复原值
    void ProcessSoftpotUp();
    void ProcessSoftpotDown();
    void ProcessSoftpotLeft();
    void ProcessSoftpotRight();

//    void ComputeGanCollikb(UINT16 y1, UINT16 y2, float x1, float x2, float& k, float& b);
//    void ComputeJawkb(UINT16 y1, UINT16 y2, float x1, float x2, float& k, float& b);
    void ComputeMotorkb(UINT16 jawy1_act_pos, UINT16 jawy2_act_pos, float jawx1_act_pos, float jawx2_act_pos, float& k, float& b);
    void ProcessSvcNMotorCal(QKeyEvent *event);
    void ProcessSvcORecalConf(QKeyEvent *event);
    void ProcessSvcPSetColTyp(QKeyEvent *event);
    void ProcessSvcQGanSpdCal(QKeyEvent *event);
    void ProcessSvcRSoftpotLoad(QKeyEvent *event);
    void ProcessSvcSSvcWarn(QKeyEvent *event);
    void ProcessSvcTTapSel(QKeyEvent *event);
    void ProcessSvcTTapSelInput(QString str);

    void ProcessSvcF1toF6Keys(QKeyEvent *event);

    //--------------------
    void SvcCopyChoice();
    void SvcCopySwitch(QKeyEvent *event);
    //Beam1
    void CopySvcBeamTrigDtoR();
    void CopySvcBeamTrigDtoT();
    void CopySvcBeamTrigRtoD();
    void CopySvcBeamTrigRtoT();
    void CopySvcBeamTrigTtoD();
    void CopySvcBeamTrigTtoR();

    void CopySvcDoseDtoR();
    void CopySvcDoseDtoT();
    void CopySvcDoseRtoD();
    void CopySvcDoseRtoT();
    void CopySvcDoseTtoD();
    void CopySvcDoseTtoR();

    void SendAllIncludeDose();
    void SendAllNotIncludeDose();

    void SendSoftpotValue();
    //--------------------

    //LJY-10-09-----O-Recall Config----------
    bool RecallConfigFiles();

    void SetXFixL();
    void SetXFixH();
    void SetEFix();
    void SetTreatClear();
    //void SetSvcClear();
    void SetPstClear();
    void SetPstDosRate();
    void SetPstDos2Time();
    void SetPstDos1();
    void SetPstDosTime();
    void SetCollimat();
    void SetJawX();
    void SetJawX1();
    void SetJawX2();
    void SetJawY();
    void SetJawY1();
    void SetJawY2();

    void SetGanPos();
//    void SetRotGanPos();

    void SetPstDos1Dos2();
    void SetRotDos2TimeRate();

    //LJY-9-23---------------------
    //void SetRotStopTime();
    void SetRotGanStratPos();
    bool SetRotGanStopPos();
    bool SetRotArc1();
    //LJY-9-25
    bool CheckRotGanStartPos();
    bool CheckAcceptMotorSetAct();  //LJY 16-05-25 在pgm转ready前需判断预设位置与实际位置
    bool CheckAcceptData();
    void CheckSoftpotLimitIntegrity();
    //-----------------------------
    //duanzhibin 10/14
     void Ovrd_Availabl_Interlock();
     void Clear_Ovrd_Interlock();

     void CaculateBypassMask();     //LJY-12-16


    void InitInterlockTable();
    void InitErrorHistoryTable();
    void DisplayInterlockTable();
    void DiaplayErrorHistoryTable();

    void SetSoftpotVal();
    void SetFoilSelect(RAY_TYPE_T type, int val);
    int SetFoilSelectNumber(RAY_TYPE_T type);
    void SetTapSelect(RAY_TYPE_T type, QString tap);

    void SetInterlockErr();
    void InitKeyCode();

    void InitSvc();

    void SetSoftpotUpEdit(QLineEdit *lineedit); //duanzhibin 1128
    void SetSoftpotDownEdit(QLineEdit *lineedit);
    void SetSoftpotUpFloatEdit(QLineEdit *lineedit);
    void SetSoftpotDownFloatEdit(QLineEdit *lineedit);
    void SetSoftpotFinishEdit(QLineEdit *lineedit);
    void SetSoftpotNextEdit(QLineEdit *lineedit);
    void SetSoftpotOldEdit(QLineEdit *lineedit);

    void GetOriginalValue(QLineEdit *lineedit);
    void SetSvcFinishEdit(QLineEdit *lineedit);
    void SetSvcOldEdit(QLineEdit *lineedit);
    void SetSvcNextEdit(QLineEdit *lineedit);
    void SetMotAdjLeftEdit(int row,int column);
    void SetMotAdjRightEdit(int row,int column);
    void SetMotAdjUpEdit(int row,int column);
    void SetMotAdjDownnEdit(int row,int column);

private:
    Ui::AccUi *ui;
    //QFtp *ftp;
    QNetworkAccessManager *netmanager;
    MODE_T mode;
    MODE_T prv_mode;
    MODE_T old_mode;
    SELECT_T old_select;//LJY-9-9
    SOFTPOT_COPY_T copy_mode;
    bool treat_switch;
    bool svc_switch;
    SELECT_T select;
    SVC_T UI;
    //SVC_T svc_select;
    int interlock_page;
    int motor_calib_page;
    bool reset_pressed;

    int updownload_file_num;

    bool svc_copy_include;
    bool svc_softpot_copy;
    int svc_cp_from;
    int svc_cp_to;

    bool set_act_pressed;
    float collimat_pst;
    UINT32 gantry_pst;

    //LJY 16-05-25-------------------------------CheckMotorSetAct
    UINT32 jawx1_pst;
    UINT32 jawx2_pst;
    UINT32 jawy1_pst;
    UINT32 jawy2_pst;
    UINT32 colli_pst;
    //-------------------------------------------

    bool start_prg;
    bool password_check;

    int softpot_page;

    //焦点所在控件的index
    int softpot_beam1_focus;
    int softpot_beam2_focus;
    int softpot_trig_focus;
    int softpot_dose1_focus;
    int softpot_dose2_focus;
    int last_softpot_page;

    QPointer<QWidget> t;
public:
    KeyThread key_thread;
    int ilk_page_index;
    int err_page_index;
    //bool gantry_is_cw;
private slots:
    void processKeyCom(QByteArray temp);



private:
    void ConstructKey();
private:
    QKeyEvent *key[150];

    bool alt_key;
    bool release_key;
    bool enable_key;

    bool gpos_val_set;
    bool gpos_green;

    //set/act collimator
    bool coll_move;

    bool switch_radon;

    bool is_stdby;

    //interlock reset
    QList<QTableWidget*> pagelist;
    QTableWidget *page;

    //calculate gain and offset in Motor Calibration
    UINT16 gantry_calib_pos[3];
    UINT16 cilli_calib_pos[3];
    UINT16 jaw_y1_pos[2];
    UINT16 jaw_y2_pos[2];
    UINT16 jaw_x2_pos[2];
    UINT16 jaw_x1_pos[2];
    MOVE_STATUS_ST move_status;

    float act_gantry_pos;   //返回的gantry实际位置
    bool show_jawx1x2y1y2;

    //LJY-10-01----------------------------------
    float preset_dose1;
    float preset_dose2;
    float preset_doserate;
    float preset_time;
    float preset_gpos;        //0-360
    float gantry_start_pos;   // -185 ~ +185
    float gantry_rot_arc;
    float gantry_stop_pos;    // -185 ~ +185
    float gantry_speed;
    //-------------------------------------------

//    bool init_jawcolli_pst;     //LJY 16-05-25
//    bool init_gantry_pst;

    float jawx1_act_pos;
    float jawx2_act_pos;
    float jawy1_act_pos;
    float jawy2_act_pos;
    float colli_act_pos;
    float jawx_act_pos;
    float jawy_act_pos;
    float gantry_act_pos;   //jaws,colli,gand的计算位置



    //interlock message
    QList<int> safety_interlock;
    QList<int> system_interlock;
    QList<int> error_interlock;
    int safety_index;
    int system_index;
    int error_index;
public slots:
    void SetInterlockHappened(int type, int num);
    void SetInterlockCleared(int type, int num);
    void ShowRemoteControl(bool show);
    void BigDosInput(int dos);
    void UpdateSegProgressBar();
    void UpdateDosProgressBar(int dos);
    void ResetProgress();
    void BackToProgram();

public slots:
    void setCurSeg(UINT8 cur);

private:
    QMutex interlock_mutex;
    bool CheckPstDos1();
    bool CheckPstTime();
    bool CheckPstJAWX();
    bool CheckPstJAWY();
    bool CheckPstJAWX1();
    bool CheckPstJAWX2();
    bool CheckPstJAWY1();
    bool CheckPstJAWY2();
    bool CheckPstCollimator();
    bool CheckPstGantry();

    QFile *upfile;
    QString direc;
    QNetworkAccessManager downloadManager;
    QNetworkAccessManager uploadManager;
    void doDownload(QString dir, QString destination);
    void doUpload(QString file, QString destination);
    QString saveFileName(const QUrl &url);
    bool saveToDisk(const QString &filename, QIODevice *data);

//    bool check_0to3_bypass;
//    bool check_5to7_bypass;

    void SendRayModeCommands();
    void ClearRemoteControl();
};



#endif // ACCUI_H
