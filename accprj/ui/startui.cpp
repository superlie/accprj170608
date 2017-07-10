#include "accui.h"
#include "ui_accui.h"
#include <QThread>

extern USERMODE_TYPE_T g_usermode;
extern bool g_check1_bypass;
extern bool g_check2_bypass;
extern QString g_svc_password;

void AccUi::ProcessStart(QKeyEvent *event)
{
    switch (event->key())
    {
    case UI_PRMRDY_RKEY:
        if(!start_prg)
        {
            start_prg = true;
            is_stdby = false;
            ShowFuncMode();
            ui->lineEditMsg->setText("");
        } 
        break;

    case UI_RADON_RKEY:
        ui->lineEditMsg->setText(MSG_RKEY_PGM);
        start_prg = false;
        break;

    case Qt::Key_F1:
        if(treat_switch && g_status==NULL_STATUS2)
        {
            g_usermode=TREATMENT_MODE;
            dosei->dose1_by_pass_mask = 0x0000;
            doseii->dose2_by_pass_mask =0x0000;
            for(int i=0; i<7; i++)
            {
                motor->error_mask_1.error_mask_bytes[i] = 0x00;
                motor->error_mask_2.error_mask_bytes[i] = 0x00;
            }

            g_action_key_mutex.lock();
            g_action_key = F1_KEY;
            g_maincontrol_wait.wakeOne();
            g_action_key_mutex.unlock();
        }
        break;

    case Qt::Key_F3:
        if(svc_switch)
        {
            if (g_is_remote_mode)
            {
                password_check = false;
                g_usermode=SERVICE_MODE;
                CaculateBypassMask();

                g_action_key_mutex.lock();
                g_action_key = F1_KEY;
                g_maincontrol_wait.wakeOne();
                g_action_key_mutex.unlock();
            }
            else
            {
                password_check = true;
                ui->lineEditMsg->setText(MSG_SVC_PASSWORD);
                ui->lineEditDate->setReadOnly(false);
                ui->lineEditDate->setFocus();
            }
        }
        break;
        //check the password
    case Qt::Key_Return:
        if(password_check)
        {
            if(ui->lineEditDate->text() == g_svc_password)
            {
                ui->lineEditMsg->setText("");
                ui->lineEditDate->setText("");
                ui->lineEditDate->setReadOnly(true);

                g_usermode=SERVICE_MODE;
                CaculateBypassMask();

                g_action_key_mutex.lock();
                g_action_key = F3_KEY;
                g_maincontrol_wait.wakeOne();
                g_action_key_mutex.unlock();
            }
            else
            {
                ui->lineEditMsg->setText(MSG_REENTER_PASSWORD);
                ui->lineEditDate->setText("");
                ui->lineEditDate->setFocus();
            }
        }
        break;

    case Qt::Key_F4:
        //        if(ui->lineEditF4->text()!=F4_DIAGS)
        //            break;
        InitKeyCode();
        key_thread.isKeyDiagsUI = true;
        ui->topFrame->hide();
        ui->lineEditF1->setText("Q - quit");
        ui->lineEditF2->setText("");
        ui->lineEditF3->setText("");
        ui->lineEditF4->setText("");
        ui->lineEditF5->setText("");
        ui->lineEditF6->setText("");
        ui->stackedWidget->setCurrentWidget(ui->accKeyboardDiags);
        break;

    case Qt::Key_F5:
        if(ui->lineEditF5->text()!=F5_RCL_DAT)
            break;
        if(!ReadLastDataFile())
        {
            ui->lineEditMsg->setText("Read last treat data error.");
        }
        ShowLastData();
        break;

    case Qt::Key_F6:
        if(ui->lineEditF6->text()!=F6_SHUTDOWN)
            break;
        if(!Shutdown())
            ui->lineEditMsg->setText("Shutdown error.");
        break;


    default:
        break;
    }
}

void AccUi::ProcessShowUI()
{
    if(g_usermode == TREATMENT_MODE)
    {
        ShowTreat();
        ui->lineEditUsrType->setStyleSheet("");
        ui->lineEditUsrType->setText("");
        mode = TREAT;
        select = TREAT_MODE;
        g_mode = TREATMENT;
        g_treat_svc_mode = TREAT;
        start_prg=false;

        g_check1_bypass=false;
        g_check2_bypass=false;
    }
    else if(g_usermode == SERVICE_MODE)
    {
        ShowSvc();
        mode = SVC;
        UI = SVC_START;
        //to use treat program
        select = TREAT_MODE;
//        g_mode = RESEARCH;
        g_treat_svc_mode = SVC;
        start_prg=false;
    }
}

void AccUi::ShowAccStart()
{
    ui->topFrame->hide();
    ui->stackedWidget->setCurrentWidget(ui->accStart);
    ui->lineEditF1->setFrame(false);
    ui->lineEditF1->setText("");
    ui->lineEditF1->setStyleSheet("");
    ui->lineEditF2->setFrame(false);
    ui->lineEditF2->setText("");
    ui->lineEditF2->setStyleSheet("");
    ui->lineEditF3->setFrame(false);
    ui->lineEditF3->setText("");
    ui->lineEditF3->setStyleSheet("");
    ui->lineEditF4->setFrame(false);
    ui->lineEditF4->setText("");
    ui->lineEditF4->setStyleSheet("");
    ui->lineEditF5->setFrame(false);
    ui->lineEditF5->setText("");
    ui->lineEditF5->setStyleSheet("");
    ui->lineEditF6->setFrame(false);
    ui->lineEditF6->setText("");
    ui->lineEditF6->setStyleSheet("");

    //ui->lineEditMsg->setText("Please turn right key switch to STD BY then to PGM/RDY");

    ui->lineEditSafeLock->setStyleSheet("");
    ui->lineEditSysLock->setStyleSheet("");
    ui->lineEditErrLock->setStyleSheet("");
}

void AccUi::ShowAccReset()
{
    ui->topFrame->hide();
    ui->stackedWidget->setCurrentWidget(ui->accStart);
    ShowFuncMode();
    ui->lineEditMsg->setText("");
}

void AccUi::ShowTreat()
{
    ui->topFrame->show();

    ui->stackedWidget->setCurrentWidget(ui->svcTreat);

    ui->lineEditStatus->setStyleSheet("background:blue");
    ui->lineEditDos1->setStyleSheet("border: 2px solid red;");
    ui->lineEditDos2->setStyleSheet("border: 2px solid red;");
    ui->lineEditDosTime->setStyleSheet("border: 2px solid red;");
    ui->lineEditDosRate->setStyleSheet("border: 2px solid red;");
    ui->lineEditDosSeg->setStyleSheet("border: 2px solid red;");

    ui->lineEditCollimator->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditJawX->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditJawY->setStyleSheet("border: 2px solid yellow;");

    ui->labelGanPos->setStyleSheet("color:white");
    ui->labelGanLeft->setText("");
    ui->labelGanRight->setText("");

    if(!io->acc3_exist)
    {
        ui->labelX1->setText("");
        ui->lineEditJawX1->setFrame(false);
        ui->labelY1->setText("");
        ui->lineEditJawY1->setFrame(false);
    }
    ui->labelX2->setText("");
    ui->lineEditJawX2->setFrame(false);
    ui->labelY2->setText("");
    ui->lineEditJawY2->setFrame(false);

    if(ui->lineEditTech->text()!=ROT_UI)
    {
        ui->labelStart2->setText("");
        ui->lineEditStart2->setFrame(false);
        ui->labelStart3->setText("");
        ui->lineEditStart3->setFrame(false);

        ui->labelGanSpeed->setText("");
        ui->lineEditGanSpeed->setStyleSheet("");
        ui->lineEditGanSpeed->setText("");
        ui->lineEditGanSpeed->setFrame(false);

        ui->labelArc1->setText("");
        ui->lineEditArc1->setStyleSheet("");
        ui->lineEditArc1->setText("");
        ui->lineEditArc1->setFrame(false);

        ui->labelArc2->setText("");
        ui->lineEditAcc2->setStyleSheet("");
        ui->lineEditArc2->setText("");
        ui->lineEditArc2->setFrame(false);

        ui->labelArc3->setText("");
        ui->lineEditArc3->setStyleSheet("");
        ui->lineEditArc3->setText("");
        ui->lineEditArc3->setFrame(false);

        ui->labelGanDirec->setText("");
        ui->lineEditGanDirec->setStyleSheet("");
        ui->lineEditGanDirec->setText("");
        ui->lineEditGanDirec->setFrame(false);

        ui->labelStop1->setText("");
        ui->lineEditStop1->setStyleSheet("");
        ui->lineEditStop1->setText("");
        ui->lineEditStop1->setFrame(false);

        ui->labelStop2->setText("");
        ui->lineEditStop2->setStyleSheet("");
        ui->lineEditStop2->setText("");
        ui->lineEditStop2->setFrame(false);

        ui->labelStop3->setText("");
        ui->lineEditStop3->setStyleSheet("");
        ui->lineEditStop3->setText("");
        ui->lineEditStop3->setFrame(false);
    }
    if(prv_mode != SVC)
        ShowFuncRay();

}

void AccUi::ShowSvc()
{
    ui->topFrame->show();
    ui->lineEditUsrType->setText(SERVICE_UI);
    ui->lineEditUsrType->setStyleSheet("background-color:rgb(255,0,255)");
    ui->stackedWidget->setCurrentWidget(ui->svcStart);
    ShowFuncRay();

    ui->sWMotorCalib->setCurrentIndex(0);
}
