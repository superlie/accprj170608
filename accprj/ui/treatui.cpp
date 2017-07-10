#include "accui.h"
#include "ui_accui.h"
#include <QPointer>
#include <QMutexLocker>

extern bool g_treat_finish;

void AccUi::ProcessTreat(QKeyEvent *event)
{
    if (g_is_remote_mode)
    {
        if (event->key()>=Qt::Key_F1 && event->key()<=Qt::Key_F6)
        {
            remote->hasSetdata = true;
        }
    }

    switch(event->key())
    {
    case Qt::Key_F1:
        ProcessTreatF1();
        break;
    case Qt::Key_F2:
        ProcessTreatF2();
        break;
    case Qt::Key_F3:
        ProcessTreatF3();
        break;
    case Qt::Key_F4:
        ProcessTreatF4();
        break;
    case Qt::Key_F5:
        ProcessTreatF5();
        break;
    case Qt::Key_F6:
        ProcessTreatF6();
        break;
    case Qt::Key_F9:
        if(g_treat_finish)
        {
            RecallLastTreatData();
        }
        break;
    case Qt::Key_F12:
        ProcessTreatF12();
        break;
    case Qt::Key_Return:
        ProcessTreatReturn();
        break;
    case Qt::Key_Y:
        if(select==TREAT_PST_DOS1_SURE)
        {
            ui->lineEditPstDos1->startBlink(RED);
            ui->lineEditPstDos1->setReadOnly(false);
            ui->lineEditPstDos1->setFocus();
            ui->lineEditMsg->setText(MSG_REENTER_DOSE1);
            ui->lineEditPstDos1->setText("");
            select = TREAT_PST_DOS1_RE;
        }
        break;
    case Qt::Key_N:
        if(select==TREAT_PST_DOS1_SURE)
        {
            ui->lineEditMsg->setText("");
            input_lock = false;
            ui->lineEditPstDos1->setText("");
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos1->setReadOnly(true);
            ui->lineEditPstDos2->setText("");
            ui->lineEditPstTime->setText("");

            preset_dose1 = 0;
            preset_dose2 = 0;
            preset_time = 0;

            if(ui->lineEditTech->text()==FIX_UI)
            {
                ShowFuncPara();
                select = TREAT_PARAMETER;
            }
            else if(ui->lineEditTech->text()==ROT_UI)
            {
                ShowFuncRotPar();
                select =  TREAT_ROT_PARAMETER;
            }
        }
        break;
    case Qt::Key_X:
        if(select==TREAT_PST_DOS1_RE)
        {
            ui->lineEditMsg->setText(MSG_ENTER_DOSE1);
            ui->lineEditPstDos1->setText("");
            select = TREAT_PST_DOS1;
        }
        break;

        //Alt+Enable
    case UI_ALTEN_KEY:
        if(event->isAutoRepeat()) return;
        ProcessAltEnable();
        break;
        //SET/ACT key
    case UI_SETACT_KEY:
        //ProcessSetAct();
        break;

    default:
        break;
    }
}

void AccUi::ProcessTreatF1()
{
    switch(select)
    {
    case TREAT_MODE:
        SetXFixL();
        //        g_action_key_mutex.lock();
        //        g_action_key = F1_KEY;
        //        g_maincontrol_wait.wakeOne();
        //        g_action_key_mutex.unlock();

        g_ray_type = X_LO;
        DoseBase::dose_rate = g_max_dose_rate.toFloat();
        if(!base->ReadSoftpotsFile())
        {
            qDebug()<<"Read softpot files error!";
            ui->lineEditSafeLock->setText("读取配置文件错误");
        }
        //LJY-12-14
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        select = TREAT_PST_DOS1;

        SendRayModeCommands();
        break;
    case TREAT_PARAMETER:
    case TREAT_ROT_PARAMETER:       //LJY-9-25
        //set ENERGY
        SetPstClear();

        input_lock = true;

        //在设置ENERGY时应把变量置为0
        preset_dose1=0;
        preset_dose2=0;
        preset_time=0;

        ui->lineEditEnergy->setText("");
        ui->lineEditEnergy->startBlink(RED);

        if(ui->lineEditMod->text()==X_RAY_UI)
        {
            ui->lineEditF1->setText("F1  6MV");
            ui->lineEditF2->setText("F2  10MV");
            ui->lineEditF3->setText("");
            ui->lineEditF4->setText("");
            ui->lineEditF5->setText("");
            ui->lineEditF6->setText("");
            select = TREAT_ENERGY;
        }
        else if(ui->lineEditMod->text()==ELTRN_UI)
        {
            ui->lineEditF1->setText("F1  5MEV");
            ui->lineEditF2->setText("F2  7MV");
            ui->lineEditF3->setText("F3  8MEV");
            ui->lineEditF4->setText("F4  10MEV");
            ui->lineEditF5->setText("F5  12MEV");
            ui->lineEditF6->setText("F6  14MEV");
            select = TREAT_ELTRN;
        }
        break;
    case TREAT_ENERGY:
        //set 6MV
        ui->lineEditEnergy->setText("06");
        ui->lineEditPstDosRate->setText(g_max_dose_rate);
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        if(ui->lineEditTech->text()==FIX_UI)
        {
            g_ray_type = X_LO;
            ShowFuncPara();
            select = TREAT_PARAMETER;
        }
        else if(ui->lineEditTech->text()==ROT_UI)
        {
            g_ray_type = ROT_X_LO;
            ShowFuncRotPar();
            select = TREAT_ROT_PARAMETER;
        }

        DoseBase::dose_rate = g_max_dose_rate.toFloat();
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();
        SendRayModeCommands();
        break;
    case TREAT_DOS_RATE:
        //LOW=50MU/MIN
        //电子线应是300
        if(ui->lineEditMod->text() == ELTRN_UI)
        {
            ui->lineEditPstDosRate->setText("0300");
            DoseBase::dose_rate = 300;
        }
        else
        {
            ui->lineEditPstDosRate->setText("0050");
            DoseBase::dose_rate = 50;
        }
        ui->lineEditPstDosRate->stopBlink();
        if(preset_dose1>1)
        {
            SetPstDos2Time();
            SendPresetTimer();
        }
        ShowFuncPara();

        //ZZ 16-11-7
        if(!base->ReadSoftpotsFile())
        {

            qDebug()<<"Read softpot files error!";
            ui->lineEditSafeLock->setText("读取配置文件错误");
        }
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();
        SendRayModeCommands();

        select = TREAT_PARAMETER;
        break;
    case TREAT_COLL_JAW:
        //set SYNC J
        //同步钨门
        //--------------Interlock 79?----------------
        //        SendJawX();
        //        SendJawY();
        //-------------------------------------------
        ShowFuncJawXY();
        select = TREAT_SYNCJ;
        break;
    case TREAT_SYNCJ:
        //set JAWX
        if(ui->lineEditF1->text()==F1_X_JAWS)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawX->setStyleSheet("background:yellow");
            ui->lineEditJawX->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_JAWX);
            ui->lineEditJawX->startBlink(YELLOW);
            ui->lineEditJawX->setReadOnly(false);
            ui->lineEditJawX->setEnabled(true);
            ui->lineEditJawX->setFocus();
            ShowFuncClear();
            select = TREAT_JAWX;
        }
        break;
    case TREAT_ROT:
        //选择Rot-X-Low
        ui->lineEditEnergy->setText("06");
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(ROT_UI);

        g_ray_type = ROT_X_LO;//LJY-9-23
        DoseBase::dose_rate = g_max_dose_rate. toFloat();
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

//        //LJY-9-23
//        dosei->dose1_gain[g_mode].bitArray=0x12;
//        doseii->dose2_gain[g_mode].bitArray=0x32;

        ShowRotGans();
        SetPstDos1Dos2();
        select = TREAT_ROT_DOSE1;
        SendRayModeCommands();
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("05");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        DoseBase::dose_rate = 300;
        g_ray_type = E1;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    case TREAT_INDEPJ:
        //set jaw X1
        if(ui->lineEditF1->text()==F1_X1)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawX1->startBlink(YELLOW);
            ui->lineEditJawX1->setReadOnly(false);
            ui->lineEditJawX1->selectAll();
            ui->lineEditJawX1->setFocus();
            ui->lineEditMsg->setText(MSG_ENTER_JAWX1);
            ShowFuncClear();
            select = TREAT_JAWX1;
        }
        break;
    default:
        break;
    }
}

void AccUi::ProcessTreatF2()
{
//    QString temp;
//    float tmp_position;
    switch(select)
    {
    case TREAT_MODE:
        SetXFixH();

        g_ray_type = X_HI;
        DoseBase::dose_rate = g_max_dose_rate.toFloat();
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        select = TREAT_PST_DOS1;
        SendRayModeCommands();
        break;
    case TREAT_PARAMETER:
    case TREAT_ROT_PARAMETER:
        //set MU
        //ui->lineEditPstDos1->setText("");
        ui->lineEditPstDos1->selectAll();
        ui->lineEditPstDos2->setText("");
        ui->lineEditPstTime->setText("");

        SetPstDos1();
        select = TREAT_PST_DOS1;
        break;
    case TREAT_ENERGY:
        //set 10MV
        ui->lineEditEnergy->setStyleSheet("background:red");
        ui->lineEditEnergy->setText("10");
        ui->lineEditPstDosRate->setText(g_max_dose_rate);
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        if(ui->lineEditTech->text()==FIX_UI)
        {
            g_ray_type = X_HI;
            ShowFuncPara();
            select = TREAT_PARAMETER;
        }
        else if(ui->lineEditTech->text()==ROT_UI)
        {
            g_ray_type = ROT_X_HI;
            ShowFuncRotPar();
            select = TREAT_ROT_PARAMETER;
        }

        DoseBase::dose_rate = g_max_dose_rate. toFloat();
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        //SetPstDos1();
        //select = TREAT_PST_DOS1;
        ShowFuncPara();
        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    case TREAT_DOS_RATE:
        //High=200MU/MIN
        if(ui->lineEditMod->text() == ELTRN_UI)
        {
            DoseBase::dose_rate = 900;
            ui->lineEditPstDosRate->setText("0900");
        }
        else
        {
            ui->lineEditPstDosRate->setText(g_max_dose_rate);
            DoseBase::dose_rate = g_max_dose_rate. toFloat();
        }
        if(preset_dose1>1)
        {
            SetPstDos2Time();
            SendPresetTimer();
        }
        ui->lineEditPstDosRate->stopBlink();
        ShowFuncPara();

        //ZZ 16-11-7
        if(!base->ReadSoftpotsFile())
        {

            qDebug()<<"Read softpot files error!";
            ui->lineEditSafeLock->setText("读取配置文件错误");
        }
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();
        SendRayModeCommands();

        select = TREAT_PARAMETER;
        break;
    case TREAT_SYNCJ:
        //set JAWY
        if(ui->lineEditF2->text()==F2_Y_JAWS)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawY->setStyleSheet("background:yellow");
            ui->lineEditJawY->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_JAWY);
            ui->lineEditJawY->startBlink(YELLOW);
            ui->lineEditJawY->setReadOnly(false);
            ui->lineEditJawY->setEnabled(true);
            ui->lineEditJawY->setFocus();
            ShowFuncClear();
            select = TREAT_JAWY;
        }
        break;
    case TREAT_COLL_JAW:
        //Set INDEP JAW
        //SetFuncY1Y2();
        //ShowJaws();
        ShowJawX1X2();
        ShowJawY1Y2();
        ui->svcTreat->update();
        show_jawx1x2y1y2 = true;

        SetActJawX1(-jawx1_act_pos);
        SetActJawX2(jawx2_act_pos);
        SetActJawY1(-jawy1_act_pos);
        SetActJawY2(jawy2_act_pos);

        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else if(g_treat_svc_mode==TREAT)
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }
        select = TREAT_INDEPJ;
        break;
    case TREAT_ROT:
        //Rot-X-Hi
        ui->lineEditEnergy->setText("10");
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(ROT_UI);

        g_ray_type = ROT_X_HI;//LJY-9-23
        DoseBase::dose_rate = g_max_dose_rate. toFloat();
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        //LJY-9-23
//        dosei->dose1_gain[g_mode].bitArray=0x1a;
//        doseii->dose2_gain[g_mode].bitArray=0x3a;

        ShowRotGans();
        SetPstDos1Dos2();
        select = TREAT_ROT_DOSE1;
        SendRayModeCommands();
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("07");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        g_ray_type = E2;
        DoseBase::dose_rate = 300;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    case TREAT_INDEPJ:
        //set jaw X2
        if(ui->lineEditF2->text()==F2_X2)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawX2->startBlink(YELLOW);
            ui->lineEditJawX2->setReadOnly(false);
            ui->lineEditJawX2->selectAll();
            ui->lineEditJawX2->setFocus();
            ui->lineEditMsg->setText(MSG_ENTER_JAWX2);
            ShowFuncClear();
            select = TREAT_JAWX2;
        }
        break;
    default:
        break;
    }

}

void AccUi::ProcessTreatF3()
{
    switch (select)
    {
    case TREAT_MODE:
        SetEFix();
        input_lock = true;
        select = TREAT_ELTRN;
        break;
    case TREAT_PARAMETER:
    case TREAT_ROT_PARAMETER:
        //set ganpos
        if(ui->lineEditF3->text()==F3_GANTRY)
        {
            input_lock = true;
            ui->lineEditGanPos->setStyleSheet("background:white");
            ui->lineEditGanPos->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_GPOS);
            ui->lineEditGanPos->startBlink(WHITE);
            ui->lineEditGanPos->setReadOnly(false);
            ui->lineEditGanPos->setEnabled(true);
            ui->lineEditGanPos->setFocus();
            ShowFuncClear();
            select = TREAT_GPOS_VAL;
        }
        break;
    case TREAT_COLL_JAW:
        //set colli
        if(ui->lineEditF3->text()==F3_COLLI)
        {
            show_jawx1x2y1y2 = false;
            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditCollimator->setStyleSheet("background:yellow");
            ui->lineEditCollimator->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_COLLI);
            ui->lineEditCollimator->startBlink(YELLOW);
            ui->lineEditCollimator->setReadOnly(false);
            ui->lineEditCollimator->setEnabled(true);
            ui->lineEditCollimator->setFocus();
            ShowFuncClear();
            select = TREAT_COLL;
        }
        break;
    case TREAT_INDEPJ:
        //set jaw y1
        if(ui->lineEditF3->text()==F3_Y1)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawY1->startBlink(YELLOW);
            ui->lineEditJawY1->setReadOnly(false);
            ui->lineEditJawY1->selectAll();
            ui->lineEditJawY1->setFocus();
            ui->lineEditMsg->setText(MSG_ENTER_JAWY1);
            ShowFuncClear();
            select = TREAT_JAWY1;
        }
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("08");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        g_ray_type = E3;
        DoseBase::dose_rate = 300;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    default:
        break;
    }
}

void AccUi::ProcessTreatF4()
{
    switch(select)
    {
    case TREAT_MODE:
        //SetRot
        //ZZ 16-11-7
        ShowFuncRot();
        select = TREAT_ROT;
        break;
    case TREAT_PARAMETER:
        //set DosRate
        ui->lineEditPstTime->selectAll();
        ui->lineEditPstDosRate->setText("");

        SetPstDosRate();
        select = TREAT_DOS_RATE;
        break;
    case TREAT_INDEPJ:
        //set jaw y2
        if(ui->lineEditF4->text()==F4_Y2)
        {
//            motor->SendStop(ALL_MOTORS);
            input_lock = true;
            ui->lineEditJawY2->startBlink(YELLOW);
            ui->lineEditJawY2->setReadOnly(false);
            ui->lineEditJawY2->selectAll();
            ui->lineEditJawY2->setFocus();
            ui->lineEditMsg->setText(MSG_ENTER_JAWY2);
            ShowFuncClear();
            select = TREAT_JAWY2;
        }
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("10");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        g_ray_type = E4;
        DoseBase::dose_rate = 300;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    case TREAT_ROT_PARAMETER:
        input_lock = true;
        ui->lineEditArc1->setStyleSheet("background:white");
        ui->lineEditArc1->selectAll();
        ui->lineEditMsg->setText(MSG_ENTER_ARC);
        ui->lineEditArc1->startBlink(WHITE);
        ui->lineEditArc1->setReadOnly(false);
        ui->lineEditArc1->setEnabled(true);
        ui->lineEditArc1->setFocus();
        ShowFuncClear();
        //select = TREAT_ROT_ARC;
        select = TREAT_ROT_ARC;
        break;
    default:
        break;
    }
}

void AccUi::ProcessTreatF5()
{
    switch(select)
    {
    case TREAT_MODE:
        //SetFilm();
        break;
    case TREAT_PARAMETER:
        //set DosTime
        input_lock = true;
        ui->lineEditPstTime->selectAll();
        ui->lineEditPstTime->startBlink(RED);
        ui->lineEditPstTime->setReadOnly(false);
        ui->lineEditPstTime->setFocus();
        ui->lineEditMsg->setText(MSG_ENTER_TIME);
        ShowFuncClear();
        select = TREAT_DOS_TIME;
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("12");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        g_ray_type = E5;
        DoseBase::dose_rate = 300;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    default:
        break;
    }

}

void AccUi::ProcessTreatF6()
{
    switch(select)
    {
    case TREAT_MODE:
        //SetSetup();  ??
        //break;
    case TREAT_PARAMETER:
    case TREAT_SYNCJ:
    case TREAT_INDEPJ:
        //F6 SET UP
        //TODO 11-24 插入附件后不应隐藏X1,Y1
        if(!io->acc3_exist)
        {
            HideJawX1X2();
            HideJawY1Y2();
        }
        ShowFuncCollJaw();
        select = TREAT_COLL_JAW;
        break;
    case TREAT_COLL_JAW:
        //F6 PRV MNU

        if(ui->lineEditMod->text()=="")
        {
            select=TREAT_MODE;
            ShowFuncRay();
        }
        else
        {
            select = TREAT_PARAMETER;
            ShowFuncPara();
        }
        break;
    case TREAT_ELTRN:
        ui->lineEditEnergy->setText("14");
        ui->lineEditPstDosRate->setText("0300");
        ui->lineEditEnergy->stopBlink();
        input_lock = false;

        g_ray_type = E6;
        DoseBase::dose_rate = 300;
        if(!base->ReadSoftpotsFile())
            ui->lineEditMsg->setText("读取配置文件错误");
        CheckSoftpotLimitIntegrity();
        SetSoftpotVal();

        SetPstDos1();
        select = TREAT_PST_DOS1;
        //        SetFuncPara();
        //        select = TREAT_PARAMETER;
        SendRayModeCommands();
        break;
    default:
        break;
    }
}

void AccUi::ProcessSetAct()
{
    if(coll_move == true)
    {
        if(!set_act_pressed)
        {
            //call SetActCollimat(float) from long
            collimat_act = true;
            set_act_pressed = true;
            ui->lineEditCollimator->setText("222.2");
        }
        else
        {
            set_act_pressed = false;
            collimat_act = false;
            //show preset value
            QString temp;
            temp.sprintf("%05.1f", collimat_pst);
            ui->lineEditCollimator->setText(temp);
        }

    }
}

void AccUi::ProcessTreatF12()
{
    if(gpos_val_set == true)
    {
        ui->labelGanPos->setStyleSheet("color:rgb(0,255,0)");
        gpos_green = true;
    }
}

void AccUi::ProcessTreatReturn()
{
    QString temp;
    int tmp;
    float colli;
    switch(select)
    {
    case TREAT_PST_DOS1:
        if(!CheckPstDos1())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            ui->lineEditPstDos1->setText("");
            return;
        }

        //11-18-------当dose1大于974时需要再次确认------------------
        if(preset_dose1>974)
        {
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos1->setReadOnly(true);
            ui->lineEditMsg->setText(MSG_SURE_DOSE1);
            select = TREAT_PST_DOS1_SURE;
        }
        else{
        //--------------------------------------------------------

            if (ui->lineEditTech->text() == ROT_UI){
                if (!SetRotArc1()){
                    return;
                } else if (ui->lineEditMsg->text() == MSG_LIMIT_REENTER)
                    ui->lineEditMsg->setText("");
            }
            ui->lineEditMsg->setText("");
            input_lock = false;
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos1->setReadOnly(true);


            if(ui->lineEditTech->text()==FIX_UI)
            {
                SetPstDos2Time();
                ShowFuncPara();
                select = TREAT_PARAMETER;
            }
            else if(ui->lineEditTech->text()==ROT_UI)
            {
                qDebug()<<"ROT_UI";
                SetPstDos2Time();
                ShowFuncRotPar();
                if(gantry_rot_arc>0.1)
                {
                    //此时是已设置其他参数后从新设置Dose1，需从新计算其他参数
                    SetRotSpeedRateTime();
                }
                select =  TREAT_ROT_PARAMETER;
            }
            SendPresetDos1();
            SendPresetDos2();
            SendPresetTimer();

        }
        break;

        //11-18-------当dose1大于1000时需要再次确认------------------
    case TREAT_PST_DOS1_RE:
        if((int)preset_dose1==ui->lineEditPstDos1->text().toInt())
        {
            ui->lineEditMsg->setText("");
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos1->setReadOnly(true);

            if(ui->lineEditTech->text()==FIX_UI)
            {
                SetPstDos2Time();
                ShowFuncPara();
                input_lock = false;
                select = TREAT_PARAMETER;
            }
            else if(ui->lineEditTech->text()==ROT_UI)
            {
                SetPstDos2Time();
                ShowFuncRotPar();
                if(gantry_rot_arc>0.1)
                {
                    //此时是已设置其他参数后从新设置Dose1，需从新计算其他参数
                    SetRotSpeedRateTime();
                }
                input_lock = false;
                select =  TREAT_ROT_PARAMETER;
            }
            SendPresetDos1();
            SendPresetDos2();
            SendPresetTimer();
        }
        else
        {
            ui->lineEditMsg->setText(MSG_CTRL_X);
        }
        break;
        //--------------------------------------------------------


    case TREAT_DOS_TIME:
        if(!CheckPstTime())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            ui->lineEditPstTime->setText("");
            return;
        }
        input_lock = false;
        ui->lineEditPstTime->stopBlink();
        ui->lineEditPstTime->clearFocus();
        ui->lineEditPstTime->setReadOnly(true);

        SetPstDosTime();
        SendPresetTimer();

        ui->lineEditMsg->setText("");

        ShowFuncPara();
        select = TREAT_PARAMETER;
        break;
    case TREAT_GPOS_VAL:
        if(!CheckPstGantry())   //赋值给preset_gpos
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            temp.sprintf("%05.1f", act_gantry_pos);
            ui->lineEditGanPos->setText(temp);
            ui->lineEditGanPos->selectAll();
            return;
        }
        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditGanPos->stopBlink();
        ui->lineEditGanPos->clearFocus();
        ui->lineEditGanPos->setReadOnly(true);
        ui->lineEditMsg->setText("");
        if(ui->lineEditTech->text()==FIX_UI)
        {
            ShowFuncPara();
            SetGanPos();                // 固定治疗 F3 后的普通旋转机架
            select = TREAT_PARAMETER;

            if (g_is_remote_mode)
            {
                upper->setedGantry = ui->lineEditGanPos->text().toFloat();
            }
        }
        else if(ui->lineEditTech->text()==ROT_UI)
        {
            ShowFuncRotPar();
            if(gantry_rot_arc>0.1)
            {
                SetRotGanStratPos();     //旋转治疗 的设置起始位置
                SetRotGanStopPos();
                SetRotSpeedRateTime();
            }
            select = TREAT_ROT_PARAMETER;
        }        
        gpos_val_set = true;
        //select = TREAT_GPOS;
        qDebug() << "return gantry_preset:" << gantry_pst << endl;
        break;
    case TREAT_COLL:
        if(!CheckPstCollimator())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);

            tmp=int (colli_act_pos*10);
            colli_act_pos=((float) tmp)/10.0f;
            if (colli_act_pos < 0.0)
                colli = colli_act_pos + 360.0;
            else
                colli = colli_act_pos;
            temp.sprintf("%05.1f", colli);
            ui->lineEditCollimator->setText(temp);
            ui->lineEditCollimator->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");

        input_lock = false;
        ui->lineEditCollimator->stopBlink();
        ui->lineEditCollimator->setReadOnly(true);
        ui->lineEditCollimator->clearFocus();
        //ZZ 16-11-14
        ui->lineEditCollimator->setStyleSheet("border: 2px solid yellow;");

        SetCollimat();
        SendCollimator();
        move_status.colli = true;

        if (g_is_remote_mode)
        {
            upper->setedColli = ui->lineEditCollimator->text().toFloat();
        }

        ui->lineEditMsg->setText("");

        ShowFuncCollJaw();
        coll_move = true;
        //select = TREAT_COLL_MOVE;
        select = TREAT_COLL_JAW;
        break;
    case TREAT_JAWX:
        if(!CheckPstJAWX())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            temp.sprintf("%04.1f", jawx_act_pos);
            ui->lineEditJawX->setText(temp);
            ui->lineEditJawX->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawX->stopBlink();
        ui->lineEditJawX->setReadOnly(true);
        ui->lineEditJawX->clearFocus();
        ui->lineEditJawX->setStyleSheet("border: 2px solid yellow;");

        SetJawX();
        SendJawX();
        move_status.jawx = true;
        move_status.jawx1 = true;
        move_status.jawx2 = true;

        ui->lineEditMsg->setText("");
        ui->lineEditF1->setText(F1_X_JAWS);
        ui->lineEditF2->setText(F2_Y_JAWS);
        ui->lineEditF6->setText(F6_PRV_MNU);

        //HideJawY1Y2();
        ShowJawX1X2();
        ShowJawY1Y2();
        show_jawx1x2y1y2 = true;

        select = TREAT_SYNCJ;
        break;
    case TREAT_JAWY:
        if(!CheckPstJAWY())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            temp.sprintf("%04.1f", jawy_act_pos);
            ui->lineEditJawY->setText(temp);
            ui->lineEditJawY->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawY->stopBlink();
        ui->lineEditJawY->setReadOnly(true);
        ui->lineEditJawY->clearFocus();
        ui->lineEditJawY->setStyleSheet("border: 2px solid yellow;");

        SetJawY();
        SendJawY();
        move_status.jawy = true;
        move_status.jawy1 = true;
        move_status.jawy2 = true;

        ui->lineEditMsg->setText("");
        ui->lineEditF1->setText(F1_X_JAWS);
        ui->lineEditF2->setText(F2_Y_JAWS);
        ui->lineEditF6->setText(F6_PRV_MNU);

        //ShowJaws();
        //HideJawX1X2();
        ShowJawX1X2();
        ShowJawY1Y2();
        show_jawx1x2y1y2 = true;

        select = TREAT_SYNCJ;
        break;
    case TREAT_JAWX1:
        if(!CheckPstJAWX1())
        {
            qDebug() << "set x1 pos fail" << endl;
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            SetActJawX1(-jawx1_act_pos);

            if (g_is_remote_mode)
            {
                upper->setedX1Pos = ui->lineEditJawX1->text().toFloat();
            }

            ui->lineEditJawX1->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawX1->stopBlink();
        ui->lineEditJawX1->setReadOnly(true);
        ui->lineEditJawX1->clearFocus();
        ui->lineEditJawX1->setStyleSheet("border: 2px solid yellow;");
        ui->lineEditMsg->setText("");

        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else if(g_treat_svc_mode==TREAT)
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }

        SetJawX1();
        //send JAWx1
        SendJawX1();
        show_jawx1x2y1y2 = true;
        move_status.jawx1 = true;

        if (g_is_remote_mode)
        {
            upper->setedX1Pos = ui->lineEditJawX1->text().toFloat();
        }

//        x1=ui->lineEditJawX1->text().toFloat();
//        x2=ui->lineEditJawX2->text().toFloat();
//        temp.sprintf("%0.1f",x1+x2);
//        ui->lineEditJawX->setText(temp);
        select = TREAT_INDEPJ;
        break;
    case TREAT_JAWX2:
        if(!CheckPstJAWX2())
        {
            qDebug() << "set x2 pos fail" << endl;
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            SetActJawX2(jawx2_act_pos);

            if (g_is_remote_mode)
            {
                upper->setedX2Pos = ui->lineEditJawX2->text().toFloat();
            }

            ui->lineEditJawX2->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawX2->stopBlink();
        ui->lineEditJawX2->setReadOnly(true);
        ui->lineEditJawX2->clearFocus();
        ui->lineEditJawX2->setStyleSheet("border: 2px solid yellow;");
        ui->lineEditMsg->setText("");

        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else if(g_treat_svc_mode==TREAT)
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }

        SetJawX2();
        //send JAWX2
        SendJawX2();
        show_jawx1x2y1y2 = true;
        move_status.jawx2 = true;

        if (g_is_remote_mode)
        {
            QString str = ui->lineEditJawX2->text();
            upper->setedX2Pos = str.toFloat();
        }

//        x1=ui->lineEditJawX1->text().toFloat();
//        x2=ui->lineEditJawX2->text().toFloat();
//        temp.sprintf("%0.1f",x1+x2);
//        ui->lineEditJawX->setText(temp);
        select = TREAT_INDEPJ;
        break;
    case TREAT_JAWY1:
        if(!CheckPstJAWY1())
        {
            qDebug() << "set y1 pos fail" << endl;
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            SetActJawY1(-jawy1_act_pos);

            if (g_is_remote_mode)
            {
                upper->setedY1Pos = ui->lineEditJawY1->text().toFloat();
            }

            ui->lineEditJawY1->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawY1->stopBlink();
        ui->lineEditJawY1->setReadOnly(true);
        ui->lineEditJawY1->clearFocus();
        ui->lineEditJawY1->setStyleSheet("border: 2px solid yellow;");
        ui->lineEditMsg->setText("");

        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else if(g_treat_svc_mode==TREAT)
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }

        SetJawY1();
        SendJawY1();
        show_jawx1x2y1y2 = true;
        move_status.jawy1 = true;

        if (g_is_remote_mode)
        {
            upper->setedY1Pos = ui->lineEditJawY1->text().toFloat();
        }

//        y1=ui->lineEditJawY1->text().toFloat();
//        y2=ui->lineEditJawY2->text().toFloat();
//        temp.sprintf("%f",y1+y2);
//        ui->lineEditJawY->setText(temp);
        select = TREAT_INDEPJ;
        break;
    case TREAT_JAWY2:
        if(!CheckPstJAWY2())
        {
            qDebug() << "set y2 pos fail" << endl;
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            SetActJawY2(jawy2_act_pos);

            if (g_is_remote_mode)
            {
                upper->setedY2Pos = ui->lineEditJawY2->text().toFloat();
            }

            ui->lineEditJawY2->selectAll();
            return;
        }

        ui->lineEditMsg->setText("");
        input_lock = false;
        ui->lineEditJawY2->stopBlink();
        ui->lineEditJawY2->setReadOnly(true);
        ui->lineEditJawY2->clearFocus();
        ui->lineEditJawY2->setStyleSheet("border: 2px solid yellow;");
        ui->lineEditMsg->setText("");

        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else if(g_treat_svc_mode==TREAT)
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }

        SetJawY2();
        //send JAW y2
        SendJawY2();
        show_jawx1x2y1y2 = true;
        move_status.jawy2 = true;

        if (g_is_remote_mode)
        {
            upper->setedY2Pos = ui->lineEditJawY2->text().toFloat();
        }

//        y1=ui->lineEditJawY1->text().toFloat();
//        y2=ui->lineEditJawY2->text().toFloat();
//        temp.sprintf("%f",y1+y2);
//        ui->lineEditJawY->setText(temp);
        select = TREAT_INDEPJ;
        break;
    case TREAT_ROT_DOSE1:
        //Dose1输入完成
        //11-19-------当dose1大于1000时需要再次确认------------------
        if(!CheckPstDos1())
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            ui->lineEditPstDos1->setText("");
            return;
        }
        if(preset_dose1>1000)
        {
            ui->lineEditPstDos1->setText("");
            ui->lineEditMsg->setText(MSG_REENTER_DOSE1);
            select = TREAT_ROT_DOSE1_RE;
        }
        else
        {
            SetRotDos2TimeRate();
            input_lock = false;
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->setReadOnly(true);
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos2->stopBlink();
            ui->lineEditMsg->setText("");

            SendPresetDos1();
            SendPresetDos2();
            SendPresetTimer();

            input_lock = true;
            ui->lineEditGanPos->startBlink(WHITE);
            ui->lineEditGanPos->setReadOnly(false);
            ui->lineEditGanPos->setFocus();
            ui->lineEditGanPos->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_GPOS);
            select = TREAT_ROT_GPOS;
        }
        break;
    case TREAT_ROT_DOSE1_RE:
        if((int)preset_dose1==ui->lineEditPstDos1->text().toInt())
        {
            ui->lineEditMsg->setText("");
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos1->setReadOnly(true);

            SetRotDos2TimeRate();
            input_lock = false;
            ui->lineEditPstDos1->stopBlink();
            ui->lineEditPstDos1->setReadOnly(true);
            ui->lineEditPstDos1->clearFocus();
            ui->lineEditPstDos2->stopBlink();
            ui->lineEditMsg->setText("");

            SendPresetDos1();
            SendPresetDos2();
            SendPresetTimer();

            input_lock = true;
            ui->lineEditGanPos->startBlink(WHITE);
            ui->lineEditGanPos->setReadOnly(false);
            ui->lineEditGanPos->setFocus();
            ui->lineEditGanPos->selectAll();
            ui->lineEditMsg->setText(MSG_ENTER_GPOS);
            select = TREAT_ROT_GPOS;
        }
        else
        {
            ui->lineEditPstDos1->setText("");
        }
        break;
    case TREAT_ROT_GPOS:
        //G-POS输入完成
        if(!CheckPstGantry() )   //赋值给preset_gpos
        {
            ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
            ui->lineEditGanPos->setText("");
            return;
        }
        input_lock = false;
        ui->lineEditGanPos->stopBlink();
        ui->lineEditGanPos->setReadOnly(true);
        ui->lineEditGanPos->clearFocus();
        ui->lineEditMsg->setText("");

        //LJY-9-23-------------------
        SetRotGanStratPos();   //seting starting pos of roting treat
        gpos_val_set=true;
        //SendGantryStart();
        input_lock = true;
        ui->lineEditArc1->startBlink(WHITE);
        ui->lineEditArc1->setReadOnly(false);
        ui->lineEditArc1->setFocus();
        ui->lineEditArc1->setText("");
        ui->lineEditMsg->setText(MSG_ENTER_ARC);
        select = TREAT_ROT_ARC;
        break;
    case TREAT_ROT_ARC:
        //ARC输入完成
        if(!SetRotArc1())   //判断Dose1/Arc比值
            return;
        if(!SetRotGanStopPos()) //判断Stop位置
            return;
        input_lock = false;
        ui->lineEditArc1->stopBlink();
        ui->lineEditArc1->setStyleSheet("background:white;color:black");
        ui->lineEditArc1->setReadOnly(true);
        ui->lineEditArc1->clearFocus();
        ui->lineEditMsg->setText("");
        ShowFuncRotPar();

        //SetRotStopTime();
        //LJY-9-23-------------------
        SetRotSpeedRateTime();

        //---------------------------
        SendGantryStop();

        //SendPresetTimer();

        //rot
        //select = TREAT_PARAMETER;
        select = TREAT_ROT_PARAMETER;       //LJY-9-25
        break;
    default:
        break;
    }
}

//void AccUi::ProcessTreatReset()
//{
//    SetTreatClear();
//    ShowAccReset();
//    treat_switch = false;
//    gpos_val_set = false;
//    coll_move = false;
//    start_prg = true;
//    //开始与Reset之后，jawx1  x2 y1 y2均不显示
//    show_jawx1x2y1y2 = false;
//    //reset之后进入治疗
//    prv_mode = START;
//    select = TREAT_MODE;
//    mode = START;
//}

void AccUi::ProcessAltEnable()
{
    if(gpos_green == true && !io->acc3_exist )
    {
        qDebug() << "alt-enable gantry_preset:" << gantry_pst << endl;
        SendGantryStart();
        //ui->lineEditMsg->setText("Gantry start to move");
    }
}

void AccUi::CollimatMoveComplet()
{
    collimat_act = false;
    set_act_pressed = false;
    coll_move = false;
    //select = TREAT_COLL_JAW;
}

void AccUi::GantryMoveComplet()
{
    gantry_act = false;
    set_act_pressed = false;
    gpos_val_set = false;
    //select = TREAT_PARAMETER;
}

//选择模式触发的命令组合
void AccUi::SendRayModeCommands()
{
    io->SendTrigger();
    interlock->SendSetup();
    io->SendSetup();

    //发送beam 31-35 命令
    beam->SendAfcpInjeInji();
    beam->SendElmBmiStcLen();
    beam->SendPfnPrfpIpfnSomin();
    beam->SendDataNotUsed();
    beam->SendBmisMinMax();

    dosei->SendDynamicSteer();      //11 44D
    dosei->SendGainAndWorkMode();   //11 47G
    doseii->SendGainAndWorkMode();  //12 48
    doseii->SendSegmentGain();      //12 53
    io->SendAfcLoopCtl();
    io->SendDosLoopCtl();

    //LJY-12-24
    if( (g_ray_type & 0xF) < 7)             //Electronic
        io->mirror_default_pos = false;
    else
        io->mirror_default_pos = true;
}

//LJY-9-23
void AccUi::SetRotSpeedRateTime()
{
    float rot_time=0.0f;
    UINT32 pst_time_ret;
    bool suc;
    QString temp;

    preset_doserate=g_max_dose_rate.toFloat();      //每次计算都应先将DoseRate置为200

    gantry_speed=gantry_rot_arc*preset_doserate/preset_dose1;
    if(gantry_speed>300.0)
    {
        gantry_speed=300.0;
        preset_doserate=preset_dose1*gantry_speed/gantry_rot_arc;
        dosei->rot_rate=(int)preset_doserate;

        temp.sprintf("%03d",(int)preset_doserate);
        ui->lineEditPstDosRate->setText(temp);
        ui->lineEditGanSpeed->setText("300");
    }
    else
    {
        dosei->rot_rate=g_max_dose_rate.toInt();

        ui->lineEditPstDosRate->setText(g_max_dose_rate);
        temp.sprintf("%03d",(int)gantry_speed);
        ui->lineEditGanSpeed->setText(temp);
    }
    //qDebug()<<"rate="<<rate<<"  speed="<<speed;
    DoseBase::dose_rate=dosei->rot_rate;

    //pst_time=dose2/rate+0.005;
    preset_time=preset_dose2/preset_doserate+0.005;
    temp.sprintf("%05.2f",preset_time);
    ui->lineEditPstTime->setText(temp);

    rot_time=preset_dose1/preset_doserate;

    suc = SlaveBase::CheckValue(preset_time, TIMER_PRE_CK, pst_time_ret);
    if(!suc)
    {
        //erro
        return;
    }
    dosei->preset_treat_time=pst_time_ret;
    doseii->preset_treat_time=pst_time_ret;

    qDebug()<<"pst_time="<<preset_time
           <<"   pst_time_ret=dosei->preset_treat_time="
          <<dosei->preset_treat_time;

    dosei->rot_dose_per_degree=
            (int)((preset_dose1*dosei->dose1_gain[g_mode].drcv*10000.0f)
                  /(gantry_rot_arc*40.0f*preset_doserate));//0x4a Rot data
    dosei->rot_speed_per_sec=(int)(gantry_rot_arc/(6.0f*rot_time));//0x4a Rot data
    dosei->rot_speed=(int)(gantry_speed*5.63f);//0x52 Motor:Move in Radon

    qDebug()<<"0x4a::   rot_dose_per_degree="<<dosei->rot_dose_per_degree;
    qDebug()<<"0x4a::   rot_speed_per_sec="<<dosei->rot_speed_per_sec;
    qDebug()<<"0x52::   dosei->rot_speed"<<dosei->rot_speed;
}
