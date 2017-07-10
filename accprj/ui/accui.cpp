#include "accui.h"
#include "ui_accui.h"
#include "csegprogress.h"
#include "cdoseprogress.h"
#include "can/Can2.h"
#include "can/Can.h"
#include "maincontrol/writethread.h"

UINT32 g_right_key;//右钥匙的位置
UINT32 g_left_key;
USERMODE_TYPE_T g_usermode;
extern bool g_check1_bypass;
extern bool g_check2_bypass;
extern QString g_ftp_ip;
extern Can2 *can2;
extern Can *can;
extern MasterController *master;
extern WriteThread *writethread;

//#define OFFLINE

AccUi::AccUi(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AccUi)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    mode = START;
    prv_mode = START;
    old_mode = START;
    g_treat_svc_mode=START;
    treat_switch = false;
    svc_switch = false;
    password_check = false;
    svc_cp_from = 0;
    svc_cp_to = 0;
    interlock_page = 0;
    motor_calib_page = 0;
    svc_softpot_copy = false;
    //svc_select = SVC_MODE;
    collimat_act = false;
    set_act_pressed = false;
    collimat_pst = 0;
    start_prg = false;
    softpot_page = 0;
    alt_key = false;
    release_key = false;
    enable_key = false;
    gpos_val_set = false;
    gpos_green = false;
    coll_move = false;
    switch_radon = false;
    is_stdby = false;
    show_jawx1x2y1y2 = false;

//    init_jawcolli_pst=false;        //LJY 16-05-25
//    init_gantry_pst=false;

    //    check_0to3_bypass = false;
    //    check_5to7_bypass = false;
    UI = SVC_NULL;
    input_lock = false;

    //    tmp_value = "";
    // first_input =false ;
    tmp_value = "";
    has_press_return = false   ;//duanzhibin1128

    ilk_page_index = 0;
    err_page_index = 0;

    headdata=0;
    bitsdata=0;

    //interlock message
    safety_index = 0;
    system_index = 0;
    error_index = 0;

    updownload_file_num=0;

    g_mode = TREATMENT;

    preset_dose1=0.0f;
    preset_dose2=0.0f;
    preset_doserate=0.0f;
    preset_time=0.0f;
    preset_gpos=0.0f;
    gantry_rot_arc=0.0f;
    gantry_start_pos=0.0f;
    gantry_stop_pos=0.0f;
    gantry_speed=0.0f;

    jawx1_act_pos=0.0f;
    jawx2_act_pos=0.0f;
    jawy1_act_pos=0.0f;
    jawy2_act_pos=0.0f;
    colli_act_pos=0.0f;
    jawx_act_pos=0.0f;
    jawy_act_pos=0.0f;
    gantry_act_pos=0.0f;

    //初始化上次治疗数据
    InitLastTreatData();

    resetMoveStatus();

    file_tmp <<"config.xml"<<"init.xml"<<"motor.xml"<<"dosei.xml"
             <<"doseii.xml"<<"hand.xml"<<"interlock.xml"<<"io.xml"
             <<"xray_high_50.xml"<<"xray_high_300.xml"<<"xray_low_50.xml"<<"xray_low_300.xml"
             <<"elc_5_300.xml"<<"elc_5_900.xml"<<"elc_7_300.xml"<<"elc_7_900.xml"
             <<"elc_8_300.xml"<<"elc_8_900.xml"<<"elc_10_300.xml"<<"elc_10_900.xml"
             <<"elc_12_300.xml"<<"elc_12_900.xml"<<"elc_14_300.xml"<<"elc_14_900.xml";

    QString tmp;
    for(int i=0;i<file_tmp.length();i++)
    {
        tmp = ".\\AccConfig\\" + file_tmp[i];
        clientfile.append(tmp);
        tmp = "ftp://" + g_ftp_ip + "/"+ file_tmp[i];
        serverfile.append(tmp);
        //qDebug()<<clientfile[i]<<"  "<<serverfile[i];
    }

    g_usermode=RESET_MODE;

    w=NULL;

    g_right_key=NULL_RKEY;
    g_left_key=NULL_LKEY;

    SetSoftpotVal();

    ShowAccStart();

    SetInterlockErr();
    InitSvc();

#ifdef OFFLINE
    qDebug()<<"OFFLINE";
    g_usermode=SERVICE_MODE;
    ShowSvc();
    ui->lineEditStatus->setText(PROGRAM_UI);
    ui->stackedWidget->setCurrentWidget(ui->svcMenu);
    mode = SVC;
    UI = SVC_MENU;
    //to use treat program
    select = TREAT_MODE;
    g_mode = RESEARCH;
    g_treat_svc_mode = SVC;
    g_status = PROGRAM;
    input_lock =false;

    start_prg=false;
    ShowFuncFrame();
#endif

#ifdef CHINESE
    ShowChineseLabel();
#endif

#ifdef SPKEYON
    ConstructKey();
    connect(&key_thread, SIGNAL(serialFinished(QByteArray)),
            this, SLOT(processKeyCom(QByteArray)));
#endif
    connect(upper, SIGNAL(SetCurSeg(UINT8)), this, SLOT(setCurSeg(UINT8)));

    connect(motor,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(dosei,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(doseii,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(beam,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(hand,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(light,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(interlock,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(io,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(upper,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));
    connect(mlc,  SIGNAL(Interlock_Inserted(int,int)), this, SLOT(SetInterlockHappened(int,int)));

    connect(motor,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(dosei,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(doseii,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(beam,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(hand,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(light,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(interlock,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(io,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(upper,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));
    connect(mlc,  SIGNAL(Interlock_Deleted(int,int)), this, SLOT(SetInterlockCleared(int,int)));

    connect(base, SIGNAL(Status_Changed()), this, SLOT(SetStatus()));

    connect(base,SIGNAL(Prompt_Msg(QString)), this,SLOT(ShowPromptMsg(QString)));//LJY-9-15

    connect(io,SIGNAL(Set_Acc3_Display(ACCESSORY_B_TYPE )),this,SLOT(SetAccessoy_3_Disp(ACCESSORY_B_TYPE)));
    connect(base,SIGNAL(ProcessShowUISig()),this,SLOT(ProcessShowUI()));
    connect(base,SIGNAL(ShowActJawColliGanSig(int)),this,SLOT(ShowActJawColliGan(int)));
    connect(base,SIGNAL(ShowActDoseTimeMUSig()),this,SLOT(ShowActDoseTimeMU()));
    connect(base,SIGNAL(ShowActSvcValueSig(SVC_LINEEDIT)),this,SLOT(ShowActSvcValue(SVC_LINEEDIT)));

    connect(base,SIGNAL(interlock_redisp_signal()),this,SLOT(Interlock_Redisplay()));
    connect(base,SIGNAL(GantryDirecSignal(bool)),this,SLOT(ShowGantryDirec(bool)));

    connect(&downloadManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(downloadFinished(QNetworkReply*)));
    connect(&uploadManager, SIGNAL(finished(QNetworkReply*)), this,
            SLOT(uploadFinished(QNetworkReply*)));
    connect(remote, SIGNAL(showRemoteControl(bool)), this, SLOT(ShowRemoteControl(bool)));
    connect(remote, SIGNAL(updateDosProgressBar(int)), this, SLOT(UpdateDosProgressBar(int)));
    connect(remote, SIGNAL(updateSegProgressBar()), this, SLOT(UpdateSegProgressBar()));
    connect(remote, SIGNAL(resetProgress()), this, SLOT(ResetProgress()));
    connect(remote, SIGNAL(backToProgram()), this, SLOT(BackToProgram()));

    connect(&(remote->setDataThread), SIGNAL(setBigDataInput(int)), this, SLOT(BigDosInput(int)));
    connect(&(remote->setDataThread), SIGNAL(setTreatdataWithKey(int)), this, SLOT(autosetTreatdataWithKey(int)));
    connect(&(remote->setDataThread), SIGNAL(setTreatdataWithData(int,QString)), this, SLOT(autosetTreatdataWithData(int,QString)));
    connect(&(remote->setDataThread), SIGNAL(ShowTreatUI()), this, SLOT(showTreatUI()));
    connect(&(remote->setDataThread), SIGNAL(resetUISelectStep()), this, SLOT(resetUISelectStep()));
    connect(remote, SIGNAL(setRemoteMsg(QString)), this, SLOT(ShowRemoteMsg(QString)));
    connect(ui->lineEditPstDos1, SIGNAL(textChanged(QString)), this, SLOT(SetPresetDos1(QString)));
    connect(ui->lineEditEnergy, SIGNAL(textChanged(QString)), this, SLOT(SetPresetEnergy(QString)));
    connect(ui->lineEditTech, SIGNAL(textChanged(QString)), this, SLOT(SetPresetTech(QString)));
    connect(ui->lineEditPstDosRate, SIGNAL(textChanged(QString)), this, SLOT(SetPresetDosrate(QString)));
    connect(ui->lineEditAcc1, SIGNAL(textChanged(QString)), this, SLOT(SetPresetAcc1(QString)));
    connect(ui->lineEditAcc2, SIGNAL(textChanged(QString)), this, SLOT(SetPresetAcc2(QString)));
    connect(ui->lineEditAcc3, SIGNAL(textChanged(QString)), this, SLOT(SetPresetAcc3(QString)));
}

AccUi::~AccUi()
{
    delete ui;

    writethread->kill();
    key_thread.exit();

    can->recvThread.exit();
    delete can;
    can = NULL;

    can2->recvThread2.exit();
    delete can2;
    can2 = NULL;

    master->exit();
    delete master;
    master = NULL;
}

void AccUi::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_F8:        //remote mode
        g_action_key_mutex.lock();
        g_action_key = REMOTE_KEY;
        g_maincontrol_wait.wakeOne();
        g_action_key_mutex.unlock();
        break;
    //right switch - Stdby
    case UI_STANDBY_RKEY:
        /*if (g_is_remote_mode && !treatdataSet)
        {
            qDebug() << "mask standby key" << endl;
            break;
        }*/

        g_right_key=STDBY_RKEY;
        if(g_status==PROGRAM)
        {
            g_action_key_mutex.lock();
            g_action_key = STANDBY_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
            ProcessStdByKey();
        }
        emit base->Status_Changed();
        break;

        //right switch - PGM/RDY
    case UI_PRMRDY_RKEY:
        g_right_key=PGM_RKEY;

        if((g_status==RAD_ON) || (g_status==STANDBY) || (g_status==COMPLETE1) || (g_status==TERMINATE1))
        {
            g_action_key_mutex.lock();
            g_is_rotate_treatment = false;
            g_action_key = PROGRAM_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }

        emit base->Status_Changed();
        break;

        //right switch - Radon
    case UI_RADON_RKEY:
        g_right_key=RADON_RKEY;
        if(g_status!=READYS)
        {
            //            Fixme 可能有Interrupt状态
            //            switch_radon = true;
            //ui->lineEditMsg->setText("Please turn right key switch to PGM/RDY.");
        }
        emit base->Status_Changed();
        break;

        //left switch - Reset
    case UI_RESET_LKEY:
        if (g_is_remote_mode && remote->isSettingData)
        {
            break;
        }

        reset_pressed=true;
        g_left_key=RESET_LKEY;
        if (g_status == PROGRAM)
        {
            resetMoveStatus();
        }

        if((g_status==POWER_UP) || (g_status==NULL_STATUS1)) break;
        if((g_status==PROGRAM) || (g_status==NULL_STATUS1) || (g_status==NULL_STATUS2)
                || (g_status==ERRORS))
        {

            SetTreatClear();
            ShowAccReset();
            ui->lineEditDate->setText("");

            //LineeditStopBlink();
            UI = SVC_NULL;
            svc_switch = false;
            treat_switch=false;
            select=TREAT_MODE;
            prv_mode = START;
            mode = START;
            g_treat_svc_mode = START;
            gpos_val_set = false;
            coll_move = false;
            start_prg = true;
            show_jawx1x2y1y2 = false;//开始与Reset之后，jawx1  x2 y1 y2均不显示

            key_thread.isKeyDiagsUI = false;

            svc_softpot_copy = false;

            hand->hand_gantry_control = false;
            hand->hand_colli_control = false;
            hand->hand_jawx1_control = false;
            hand->hand_jawx2_control = false;
            hand->hand_jawy1_control = false;
            hand->hand_jawy2_control = false;
            io->io_jawxy_control = false;
        }

        if((g_status==PROGRAM) || (g_status==TERMINATE2) || (g_status==ERRORS) ||
                (g_status==NULL_STATUS1) || (g_status==NULL_STATUS2))
        {
            g_action_key_mutex.lock();
            g_action_key = RESET_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();

            //ZZ-17-3-27
            //interlock->WriteOwnFile();
            //interlock->WriteErrDisplayFile();
            writethread->addTask(1);

            //ui->lineEditMsg->setText("Please waiting for RESET ......");
        }
        CurrentPositAsTarget();
        emit base->Status_Changed();
        break;

        //left switch - Treat
    case UI_TREAT_LKEY:
        g_left_key=TREAT_LKEY;
        if(g_status==POWER_UP || g_status==NULL_STATUS1) break;
        if(start_prg)
        {
            treat_switch = true;
            svc_switch = false;
        }
        if(g_status!=TERMINATE2) emit base->Status_Changed();
        break;

        //left switch - Service
    case UI_SVC_LKEY:
        g_left_key=SVC_LKEY;
        if(g_status==POWER_UP || g_status==NULL_STATUS1) break;
        if(start_prg)
        {
            svc_switch = true;
            treat_switch = false;
        }
        emit base->Status_Changed();
        break;

        //Enable key
        //Put in treatui.cpp

        //Clear key
    case UI_CLEAR_KEY:
        if (g_is_remote_mode && remote->isSettingData)
        {
            break;
        }

        switch(g_status)
        {
        case READYS:
        case NOT_READY:
        case COMPLETE2:
        case TERMINATE3:
        case CHECK2_2:
            g_action_key_mutex.lock();
            g_action_key = CLEAR_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
            break;
        default:
            break;
        }

        if (g_is_remote_mode && PROGRAM==g_status)
        {
            g_action_key_mutex.lock();
            g_action_key = CLEAR_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }

        switch(g_status)
        {
        case READYS:
        case NOT_READY:

            //hand->SendUserMode(RESET_MODE);
            //case CHECK2_2:    //11-24
            input_lock = false;
            hand->hand_gantry_control = false;
            hand->hand_colli_control = false;
            hand->hand_jawx1_control = false;
            hand->hand_jawx2_control = false;
            hand->hand_jawy1_control = false;
            hand->hand_jawy2_control = false;

            svc_softpot_copy = false;

            if(!io->acc3_exist)
            {
                HideJawX1X2();
                HideJawY1Y2();
            }
            ShowActJawColliGan(0);  //Jaw + Colli
            ShowActJawColliGan(1);  //Gantry

            //ShowTreat();
            //ShowFuncRay();

            if(ui->lineEditTech->text()==FIX_UI)
            {
                select = TREAT_PARAMETER;
            }
            else if(ui->lineEditTech->text()==ROT_UI)
            {
                select =  TREAT_ROT_PARAMETER;
            }

            if(g_treat_svc_mode == TREAT)
            {
                ShowTreat();
                ui->stackedWidget->update();
            }
            else if(g_treat_svc_mode == SVC)    //LJY
            {
                if(UI == SVC_TREAT)
                {
                    ShowTreat();
                    ui->stackedWidget->update();
                }
                else if(UI == SVC_B_SOFTPOT)
                {
                    ui->stackedWidget->setCurrentWidget(ui->svcMenu);
                    UI = SVC_MENU;
                }
            }
            ShowFuncPara();
            break;
        case COMPLETE2:
        case TERMINATE3:
        case PROGRAM:
            BackToProgram();

            break;
        default:
            break;
        }
        emit base->Status_Changed();
        break;

        //RadOn key
    case UI_RADON_KEY:
        if(g_right_key==RADON_RKEY && ((g_status==READYS) || (g_status==INTERRUPT)))
        {
            g_action_key_mutex.lock();
            if (ui->lineEditTech->text() == ROT_UI)
            {
                g_is_rotate_treatment = true;
            }
            g_action_key = RADON_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }
        break;

        //RadOff key
    case UI_RADOFF_KEY:
        if(g_status==RAD_ON)
        {
            g_action_key_mutex.lock();
            g_action_key = RADOFF_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }
        break;

        //Accept key
    case UI_ACCEPT_KEY:
        if(g_status==PROGRAM && CheckAcceptData())
        {
            if((g_ray_type==ROT_X_LO) || (g_ray_type==ROT_X_HI))
            {
                if(!CheckRotGanStartPos())      //旋转治疗在Accept前检测Gantry的当前位置和设置位置
                {
                    ui->lineEditMsg->setText(MSG_GPOS_NOTMATCH);
                    return;
                }
            }
            else    //LJY 16-05-25 在pgm转ready前需判断gantry预设位置与实际位置，若不同则提示，只有在gantry移动后到指定位置才能进入ready
            {
                /*if(!CheckAcceptMotorSetAct())
                {
                    ui->lineEditMsg->setText(MSG_GPOS_NOTMATCH);
                    return;
                }
                else if(ui->lineEditMsg->text()==MSG_GPOS_NOTMATCH)
                {
                        ui->lineEditMsg->setText("");
                }*/
            }
            g_action_key_mutex.lock();
            g_action_key = ACCEPT_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }
        break;

        //Stop key
        //LJY-12-23
//    case UI_STOP_KEY:
//        g_action_key_mutex.lock();
//        g_action_key = MOTIONSTOP_KEY;
//        g_maincontrol_wait.wakeOne();
//        g_action_key_mutex.unlock();
//        break;


        //F1
    case Qt::Key_F1:
        if((g_status==INTERRUPT) || (g_status==INTERRUPT_NR))
        {
            g_action_key_mutex.lock();
            g_action_key = F1_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();

            ui->lineEditF1->setText("");//清除F1 QUIT
        }
        break;

        //Alt+S
    case UI_ALTS_KEY:
        if(UI == SVC_START)
        {
            ui->stackedWidget->setCurrentWidget(ui->svcMenu);
            UI = SVC_MENU;
        }
        else if((UI == SVC_TREAT) && !input_lock)
        {
            ui->stackedWidget->setCurrentWidget(ui->svcMenu);
            UI = SVC_MENU;
            if(mode==TREAT)
            {
                //由Treat界面转过来
                mode=SVC;
                prv_mode=START;
            }
            break;
        }
        break;

    case Qt::Key_C:
        if((g_status==PROGRAM) && (UI==SVC_J_CK_DISP) && !g_check2_bypass)
        {
            g_action_key_mutex.lock();
            g_action_key = C_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }
        break;

        //interlock message up and down
    case Qt::Key_Down:
        ProcessInterlockMsgDown();
        break;
    case Qt::Key_Up:
        ProcessInterlockMsgUp();
        break;
    default:
        break;
    }

    //qDebug()<<"mode=="<<mode;
    switch (mode)
    {
    case START:
        ProcessStart(event);
        break;
    case TREAT:
        //LJY-9-16 在Program状态下设置参数，其他状态下不响应，若在SVC模式下可响应Alt+S等
        if(g_status==PROGRAM)
        {
            ProcessTreat(event);
        }
        break;
    case SVC:
        ProcessSvc(event);
        break;
    case STDBY:
        ProcessStdByState(event);
        break;
    default:
        break;
    }

}

void AccUi::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    //alt+enable release
    case UI_ALTEN_KEY:
        if(event->isAutoRepeat()) return;
        if(gpos_green)
        {
            SendGantryStop();
            //ui->lineEditMsg->setText("Gantry move complete");
            gpos_green = false;
            ui->labelGanPos->setStyleSheet("color:rgb(255,255,255)");
        }
        break;
    default:
        break;
    }
}

void AccUi::ProcessStdByKey()
{
    //ui->lineEditStatus->setText("STAND BY");

    start_prg = false;
    is_stdby = true;

    if(mode != START)
    {
        old_mode = mode;
        mode = STDBY;
    }
}

void AccUi::ProcessInterlockMsgDown()
{
    interlock_mutex.lock();
    if(safety_interlock.size() != 0)
    {
        safety_index++;
        if(safety_index >= safety_interlock.size())
            safety_index = 0;
        if(g_treat_svc_mode == SVC && g_interlocks[safety_interlock[safety_index]].ovrd)
        {
            ui->lineEditSafeLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditSafeLock->setStyleSheet("background:red");
        }
        ui->lineEditSafeLock->setText(interlock_list[safety_interlock[safety_index]]);
        ui->lineEditSafeLock->update();
    }
    if(system_interlock.size() != 0)
    {
        system_index++;
        if(system_index >= system_interlock.size())
            system_index = 0;
        if(g_treat_svc_mode == SVC && g_interlocks[system_interlock[system_index]].ovrd)
        {
            ui->lineEditSysLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditSysLock->setStyleSheet("background:red");
        }
        ui->lineEditSysLock->setText(interlock_list[system_interlock[system_index]]);
        ui->lineEditSysLock->update();
    }
    if(error_interlock.size() != 0)
    {
        error_index++;
        if(error_index >= error_interlock.size())
            error_index = 0;
        if(g_treat_svc_mode == SVC && g_interlocks[error_interlock[error_index]].ovrd)
        {
            ui->lineEditErrLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditErrLock->setStyleSheet("background:red");
        }
        ui->lineEditErrLock->setText(interlock_list[error_interlock[error_index]]);
        ui->lineEditErrLock->update();
    }
    interlock_mutex.unlock();
}

void AccUi::ProcessInterlockMsgUp()
{
    interlock_mutex.lock();
    if(safety_interlock.size() != 0)
    {
        safety_index--;
        if(safety_index < 0)
            safety_index = safety_interlock.size() - 1;
        ui->lineEditSafeLock->setText(interlock_list[safety_interlock[safety_index]]);
        if(g_treat_svc_mode == SVC && g_interlocks[safety_interlock[safety_index]].ovrd)
        {
            ui->lineEditSafeLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditSafeLock->setStyleSheet("background:red");
        }
    }
    if(system_interlock.size() != 0)
    {
        system_index--;
        if(system_index < 0)
            system_index = system_interlock.size() - 1;
        ui->lineEditSysLock->setText(interlock_list[system_interlock[system_index]]);
        if(g_treat_svc_mode == SVC && g_interlocks[system_interlock[system_index]].ovrd)
        {
            ui->lineEditSysLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditSysLock->setStyleSheet("background:red");
        }
    }
    if(error_interlock.size() != 0)
    {
        error_index--;
        if(error_index < 0)
            error_index = error_interlock.size() - 1;
        ui->lineEditErrLock->setText(interlock_list[error_interlock[error_index]]);
        if(g_treat_svc_mode == SVC && g_interlocks[error_interlock[error_index]].ovrd)
        {
            ui->lineEditErrLock->setStyleSheet("background:blue");
        }
        else
        {
            ui->lineEditErrLock->setStyleSheet("background:red");
        }
    }
    interlock_mutex.unlock();
}

void AccUi::ProcessStdByState(QKeyEvent *event)
{
    if(event->key()==UI_PRMRDY_RKEY)
    {
        is_stdby = false;
        //Fixme 不一定会返回到Program，应交由状态机控制
        ui->lineEditMsg->setText("");
        mode = old_mode;
    }
}

void AccUi::resetMoveStatus()
{
    move_status.colli = false;
    move_status.jawx = false;
    move_status.jawy = false;
    move_status.jawx1 = false;
    move_status.jawx2 = false;
    move_status.jawy1 = false;
    move_status.jawy2 = false;
}

bool AccUi::checkActPstMatch(float act, float pst, float delta)
{
    if (qAbs(qAbs(act)-qAbs(pst)) <= delta)
    {
        qDebug() << "act" << act << "pst" << pst << "equal" << endl;
        return true;
    }

    qDebug() << "act" << act << "pst" << pst << "not equal" << endl;
    return false;
}

void AccUi::AllLineEditStopBlink()
{
    switch (select)
    {
    case TREAT_JAWX:
        ui->lineEditJawX->stopBlink();
        ui->lineEditJawX->setReadOnly(true);
        ui->lineEditJawX->clearFocus();
        ui->lineEditJawX->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_JAWX1:
        ui->lineEditJawX1->stopBlink();
        ui->lineEditJawX1->setReadOnly(true);
        ui->lineEditJawX1->clearFocus();
        ui->lineEditJawX1->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_JAWY:
        ui->lineEditJawY->stopBlink();
        ui->lineEditJawY->setReadOnly(true);
        ui->lineEditJawY->clearFocus();
        ui->lineEditJawY->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_JAWX2:
        ui->lineEditJawX2->stopBlink();
        ui->lineEditJawX2->setReadOnly(true);
        ui->lineEditJawX2->clearFocus();
        ui->lineEditJawX2->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_ROT_GPOS:
    case TREAT_GPOS_VAL:
        ui->lineEditGanPos->stopBlink();
        ui->lineEditGanPos->clearFocus();
        ui->lineEditGanPos->setReadOnly(true);
        break;
    case TREAT_COLL:
        ui->lineEditCollimator->stopBlink();
        ui->lineEditCollimator->setReadOnly(true);
        ui->lineEditCollimator->clearFocus();
        ui->lineEditCollimator->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_JAWY1:
        ui->lineEditJawY1->stopBlink();
        ui->lineEditJawY1->setReadOnly(true);
        ui->lineEditJawY1->clearFocus();
        ui->lineEditJawY1->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_JAWY2:
        ui->lineEditJawY2->stopBlink();
        ui->lineEditJawY2->setReadOnly(true);
        ui->lineEditJawY2->clearFocus();
        ui->lineEditJawY2->setStyleSheet("border: 2px solid yellow;");
        break;
    case TREAT_ROT_ARC:
        ui->lineEditArc1->stopBlink();
        ui->lineEditArc1->setStyleSheet("background:white;color:black");
        ui->lineEditArc1->setReadOnly(true);
        ui->lineEditArc1->clearFocus();
        break;
    case TREAT_DOS_TIME:
        ui->lineEditPstTime->stopBlink();
        ui->lineEditPstTime->clearFocus();
        ui->lineEditPstTime->setReadOnly(true);
        break;
    case TREAT_PST_DOS1:
    case TREAT_PST_DOS1_RE:
        ui->lineEditPstDos1->stopBlink();
        ui->lineEditPstDos1->clearFocus();
        ui->lineEditPstDos1->setReadOnly(true);
        break;
    case TREAT_ENERGY:
    case TREAT_ELTRN:
        ui->lineEditEnergy->stopBlink();
        break;
    default:
        break;
    }
}
