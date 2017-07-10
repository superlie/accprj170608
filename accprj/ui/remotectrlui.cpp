#include "accui.h"
#include "ui_accui.h"

extern USERMODE_TYPE_T g_usermode;
extern UpperMonitor *upper;

void AccUi::BigDosInput(int dos)     //dos值大于974时需要按下Y后再重新输入一次
{
    if (dos > 974)
    {
        // Y
        if(select==TREAT_PST_DOS1_SURE)
        {
            ui->lineEditPstDos1->startBlink(RED);
            ui->lineEditPstDos1->setReadOnly(false);
            ui->lineEditPstDos1->setFocus();
            ui->lineEditMsg->setText(MSG_REENTER_DOSE1);
            ui->lineEditPstDos1->setText("");
            select = TREAT_PST_DOS1_RE;
        }

        ui->lineEditPstDos1->setText(QString::number(dos));
        ProcessTreatReturn();
    }
}

void AccUi::setCurSeg(UINT8 cur)
{
    ui->lbSegProgress->SetCur(cur);
}

void AccUi::SetPresetDos1(QString value)
{
    if (PROGRAM == g_status)
    {
        upper->setedMeterSet = value.toInt();
    }
}

void AccUi::SetPresetEnergy(QString value)
{
    if (PROGRAM == g_status)
    {
        if ("06" == value)
        {
            upper->setedEnergy = EG_LOW;
        }
        else if ("10" == value)
        {
            upper->setedEnergy = EG_HIGH;
        }
    }
}

void AccUi::SetPresetTech(QString value)
{
    if (PROGRAM == g_status)
    {
        if (FIX_UI == value)
        {
            upper->setedTech = TECH_FIX;
        }
        else if (ROT_UI == value)
        {
            upper->setedTech = TECH_ROT;
        }
    }
}

void AccUi::SetPresetDosrate(QString value)
{
    if (PROGRAM == g_status)
    {
        if ("0050" == value)
        {
            upper->setedDosrate = DR_LOW;
        }
        else if ("0300" == value)
        {
            upper->setedDosrate = DR_HIGH;
        }
    }
}

void AccUi::SetPresetAcc1(QString value)
{
    upper->setedAcc1 = value;
    if (upper->setedAcc1 == "无")
        upper->setedAcc1 = "NONE";
}

void AccUi::SetPresetAcc2(QString value)
{
    upper->setedAcc2 = value;
    if (upper->setedAcc2 == "无")
        upper->setedAcc2 = "NONE";
}

void AccUi::SetPresetAcc3(QString value)
{
    upper->setedAcc3 = value;
    if (upper->setedAcc3 == "无")
        upper->setedAcc3 = "NONE";
}

void AccUi::ShowRemoteControl(bool show)
{
    //ZZ
    if (show)
    {
        ui->lineEditDosSeg->setVisible(true);
        ui->lbSeg->setVisible(true);
        ui->lineEditPstDosSeg->setVisible(true);

        ui->lbSegProgress->setVisible(true);
        ui->lbDoseProgress->setVisible(true);
    }
    else
    {
        ui->lineEditDosSeg->setVisible(false);
        ui->lbSeg->setVisible(false);
        ui->lineEditPstDosSeg->setVisible(false);

        ui->lbSegProgress->setVisible(false);
        ui->lbDoseProgress->setVisible(false);
        //ResetProgress();
    }
}

void AccUi::ClearRemoteControl()
{
    ui->lineEditDosSeg->setText("");
    ui->lineEditPstDosSeg->setText("");
}

void AccUi::ShowRemoteMsg(QString msg)
{
    if (msg == "")
    {
        if (ui->lineEditMsg->text()==MSG_REMOTE_CANCEL || ui->lineEditMsg->text()==MSG_REMOTE_TERMINATE)
            ui->lineEditMsg->setText("");
    }
    else
        ui->lineEditMsg->setText(msg);
}

void AccUi::autosetTreatdataWithKey(int key)
{
    TREATDATA_KEY_T enumKey = (TREATDATA_KEY_T)key;

    /*if (SET_TREATDATA != remote->mRemoteStatus)
    {
        return;
    }*/

    switch(enumKey)
    {
    case TK_F1:
        this->ProcessTreatF1();
        break;
    case TK_F2:
        this->ProcessTreatF2();
        break;
    case TK_F3:
        this->ProcessTreatF3();
        break;
    case TK_F4:
        this->ProcessTreatF4();
        break;
    case TK_F5:
        this->ProcessTreatF5();
        break;
    case TK_F6:
        this->ProcessTreatF6();
        break;
    case TK_RETURN:
        this->ProcessTreatReturn();
        break;
    }
}

void AccUi::autosetTreatdataWithData(int lineedit, QString str)
{
    TREATDATA_LINEEDIT_T enumLineedit = (TREATDATA_LINEEDIT_T)lineedit;

    /*if (SET_TREATDATA != remote->mRemoteStatus)
    {
        return;
    }*/

    switch(enumLineedit)
    {
    case TL_DOSE1_VALUE:
        ui->lineEditPstDos1->setText(str);
        ui->lbDoseProgress->SetMax(str.toInt());
        ui->lbDoseProgress->SetCur(0);
        break;
    case TL_DOSE_TIME:
        ui->lineEditPstTime->setText(str);
        break;
    case TL_GANTRY:
        ui->lineEditGanPos->setText(str);
        break;
    case TL_COLLI:
        ui->lineEditCollimator->setText(str);
        break;
    case TL_X1:
        qDebug() << "want to set x1" << str << endl;
        ui->lineEditJawX1->setText(str);
        break;
    case TL_X2:
        qDebug() << "want to set x2" << str << endl;
        ui->lineEditJawX2->setText(str);
        break;
    case TL_Y1:
        qDebug() << "want to set y1" << str << endl;
        ui->lineEditJawY1->setText(str);
        break;
    case TL_Y2:
        qDebug() << "want to set y2" << str << endl;
        ui->lineEditJawY2->setText(str);
        break;
    case TL_PRESET_SEG:
        ui->lineEditPstDosSeg->setText(str);
        ui->lbSegProgress->SetMax(str.toInt());
        break;
    case TL_CUR_SEG:
        ui->lineEditDosSeg->setText(str);
        //ui->lbSegProgress->SetCur(str.toInt());
        break;
    }
}

void AccUi::showTreatUI()
{
    /*if (SET_TREATDATA != remote->mRemoteStatus)
    {
        return;
    }*/

    if (SERVICE_MODE==g_usermode && upper->IsFirstBeam(upper->GetCurBeamId()))  //显示机架准直器界面
    {
        select=TREAT_MODE;
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF6();
        ProcessTreatF6();
    }
}

void AccUi::resetUISelectStep()
{
    resetMoveStatus();

    hand->gantry_rotat_mode = false;
    hand->hand_gantry_control = false;
    hand->hand_colli_control = false;
    hand->hand_jawx1_control = false;
    hand->hand_jawx2_control = false;
    hand->hand_jawy1_control = false;
    hand->hand_jawy2_control = false;

    svc_softpot_copy = false;

    SetTreatClear();
    ShowFuncRay();

    select=TREAT_MODE;
}

void AccUi::UpdateSegProgressBar()
{
    ui->lbSegProgress->StepToNextProgress();
    ui->lbDoseProgress->UpdateProgress(0);
}

void AccUi::UpdateDosProgressBar(int dos)
{
    ui->lbDoseProgress->UpdateProgress(dos);
}

void AccUi::ResetProgress()
{
    ui->lbDoseProgress->UpdateProgress(0);
    ui->lbSegProgress->UpdateProgress(0);
}

void AccUi::BackToProgram()
{
    //2016-8-17
    //hand->SendUserMode(RESET_MODE);
    //case CHECK2_2:    //11-24

    resetMoveStatus();

    hand->gantry_rotat_mode = false;
    hand->hand_gantry_control = false;
    hand->hand_colli_control = false;
    hand->hand_jawx1_control = false;
    hand->hand_jawx2_control = false;
    hand->hand_jawy1_control = false;
    hand->hand_jawy2_control = false;

    svc_softpot_copy = false;

    SetTreatClear();
    ShowActJawColliGan(0);  //Jaw + Colli
    ShowActJawColliGan(1);  //Gantry
    //ShowTreat();
    ShowFuncRay();
    CurrentPositAsTarget();

    select=TREAT_MODE;

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
}
