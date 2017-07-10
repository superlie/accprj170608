#include "accui.h"
#include "ui_accui.h"

extern bool g_check1_bypass;
extern bool g_check2_bypass;
extern bool g_compensate;
extern bool isSipNormalWork;

void AccUi::ProcessSvc(QKeyEvent *event)
{
    switch (UI) {
    case SVC_START:
        ProcessSvcF1toF6Keys(event);
        break;
    case SVC_MENU:
        ProcessSvcMenu(event);
        break;
    case SVC_B_SOFTPOT:
        ProcessSvcSoftpot(event);
        break;
    case SVC_C_INTERLOCK:
        ProcessSvcCInterlock(event);
        break;
    case SVC_D_ERR_DISP:
        ProcessSvcDErrDisp(event);
        break;
    case SVC_E_CMPSATION_FCTOR:
        ProcessSvcECmpstFct(event);
        break;
        //    case SVC_F_VERIF_ENABLE:
        //        ProcessSvcFVerif(event);
        //        break;
    case SVC_G_MEAN_ATTENUAT:
        ProcessSvcGMeanAtt(event);
        break;
    case SVC_H_SIP_ALYZER:
        ProcessSvcHSipAnaly(event);
        break;
    case SVC_I_MTOR_ADJST:
        ProcessSvcIMotorAdj(event);
        break;
    case SVC_J_CK_DISP:
        ProcessSvcJChkDisp(event);
        break;
    case SVC_K_T1_DISP:
        ProcessSvcKT1Disp(event);
        break;
        //    case SVC_L_PROHBIT_SECT:
        //        ProcessSvcLProhbSec(event);
        //        break;
    case SVC_M_FOIL_SLCT:
        ProcessSvcMFoilSel(event);
        break;
    case SVC_N_MTOR_CALIBRAT:
        ProcessSvcNMotorCal(event);
        break;
    case SVC_O_RCAL_CONFIG:
        ProcessSvcORecalConf(event);
        break;
        //    case SVC_P_SET_COLI_TYPE:
        //        ProcessSvcPSetColTyp(event);
        //        break;
    case SVC_Q_GANT_SPD_CALIBRAT:
        ProcessSvcQGanSpdCal(event);
        break;
    case SVC_R_SOFTPOT_UP_DL:
        ProcessSvcRSoftpotLoad(event);
        break;
    case SVC_S_SVC_WARN:
        ProcessSvcSSvcWarn(event);
        break;
    case SVC_T_TAP_SLT:
        ProcessSvcTTapSel(event);
        break;
    default:
        break;
    }

}

void AccUi::ProcessSvcMenu(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_A:
        prv_mode = mode;
        mode = TREAT;
        ShowTreat();
        UI = SVC_TREAT;
        //select = TREAT_MODE;
        break;
    case Qt::Key_B:
        if(ui->lineEditMod->text()!="")
        {
            if(io->afc_loop_mode)
                ui->lbSoftpotsAM->setText("MENUAL");
            else
                ui->lbSoftpotsAM->setText("AUTO");
            if(io->dos_loop_mode)
                ui->lbSoftpotsCO->setText("OPEN");
            else
                ui->lbSoftpotsCO->setText("CLOSE");
            if(g_mode == TREATMENT)
            {
                ui->lbSoftpotsTR->setText("TREAT");
                softpot_widge[0].at(softpot_beam1_focus)->setFocus();
                softpot_widge[0].at(softpot_beam1_focus)->clearFocus();
            }
            else if(g_mode == RESEARCH)
            {
                ui->lbSoftpotsTR->setText("RESRCH");
                softpot_widge[0].at(softpot_beam1_focus)->setFocus();
                softpot_widge[0].at(softpot_beam1_focus)->selectAll();
            }
            last_softpot_page =0;
            tmp_value =softpot_widge[0].at(softpot_beam1_focus)->text();
            ui->stackedWidget->setCurrentWidget(ui->svcSoftpot);
            ui->sWSoftpot->setCurrentWidget(ui->beam1);
            UI = SVC_B_SOFTPOT;
        }
        break;
    case Qt::Key_C:
        InitInterlockTable();
        DisplayInterlockTable();
        ui->stackedWidget->setCurrentWidget(ui->svcInterlock);
        ui->sWInterlock->setCurrentWidget(ui->interlockPage1);
        UI = SVC_C_INTERLOCK;
        break;
    case Qt::Key_D:
        InitErrorHistoryTable();
        DiaplayErrorHistoryTable();
        ui->stackedWidget->setCurrentWidget(ui->svcErrDisp);
        UI = SVC_D_ERR_DISP;
        break;
    case Qt::Key_E:
        if(g_compensate)
        {
            ui->leCpsFactorCO->setText("COMPENSATION");
        }
        else
        {
            ui->leCpsFactorCO->setText("OVERRIDE");
        }
        ui->leCpsFactorRefTemp->setText(QString::number(DoseBase::ref_temp));
        ui->leCpsFactorRefPress->setText(QString::number(DoseBase::ref_press));
        GetOriginalValue(ui->leCpsFactorRefTemp);
        ui->stackedWidget->setCurrentWidget(ui->svcCpsFactor);
        UI = SVC_E_CMPSATION_FCTOR;
        break;
        //    case Qt::Key_F:
        //        ui->stackedWidget->setCurrentWidget(ui->svcVerificat);
        //        UI = SVC_F_VERIF_ENABLE;
        //        break;
    case Qt::Key_G:
        ui->stackedWidget->setCurrentWidget(ui->svcMeanAttenuat);
        GetOriginalValue(ui->leMeanAttLoCalibrFact);
        UI = SVC_G_MEAN_ATTENUAT;
        break;
    case Qt::Key_H:
        ui->leSipAnalyzerState->setText("");
        ui->stackedWidget->setCurrentWidget(ui->svcSipAnalyzer);
        UI = SVC_H_SIP_ALYZER;
        break;
    case Qt::Key_I:
        motadj_column = 6;
        motadj_row = 4;
        ShowMotorAdjust();
        ui->stackedWidget->setCurrentWidget(ui->svcMotorAdjust);
        GetOriginalValue(ui->leMotAdjJawY1SS);
        UI = SVC_I_MTOR_ADJST;
        break;
    case Qt::Key_J:
        if(g_check1_bypass)
            ui->leChkDispBypass1->setText("BYPASS 1");
        else
            ui->leChkDispBypass1->setText("");
        if(g_check2_bypass)
            ui->leChkDispBypass2->setText("BYPASS 2");
        else
            ui->leChkDispBypass2->setText("");
        ui->stackedWidget->setCurrentWidget(ui->svcChkDisp);
        UI = SVC_J_CK_DISP;
        break;
    case Qt::Key_K:
        ui->stackedWidget->setCurrentWidget(ui->svcT1Display);
        UI = SVC_K_T1_DISP;
        break;
        //    case Qt::Key_L:
        //        ui->stackedWidget->setCurrentWidget(ui->svcProhibitSector);
        //        UI = SVC_L_PROHBIT_SECT;
        //        break;
    case Qt::Key_M:
        ShowFoilSelect();
        ui->stackedWidget->setCurrentWidget(ui->svcFoilSelect);
        UI = SVC_M_FOIL_SLCT;
        break;
    case Qt::Key_N:
        ui->leMotClbGantryRefPosn1->setFocus();
        ShowMotorCalibGainOffset();
        ShowMotorCalibFocus();
        ui->stackedWidget->setCurrentWidget(ui->svcMotorCalib);
        UI = SVC_N_MTOR_CALIBRAT;
        break;
    case Qt::Key_O:
         ui->leRecallConfigStatus->setText("");
        ui->stackedWidget->setCurrentWidget(ui->svcRecallConf);
        UI = SVC_O_RCAL_CONFIG;
        break;
        //    case Qt::Key_P:
        //        ui->stackedWidget->setCurrentWidget(ui->svcColliType);
        //        UI = SVC_P_SET_COLI_TYPE;
        //        break;
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcGantrySpeed);
        GetOriginalValue(ui->leGantrySpdFactClk);
        UI = SVC_Q_GANT_SPD_CALIBRAT;
        break;
    case Qt::Key_R:
        ui->leSptUpDownStatus->setText("");
        ui->stackedWidget->setCurrentWidget(ui->svcSoftpotUpDown);
        UI = SVC_R_SOFTPOT_UP_DL;
        break;
    case Qt::Key_S:
        svc_warn_size = svc_waring.size();
        ui->stackedWidget->setCurrentWidget(ui->svcWarning);
        GetOriginalValue(ui->leSvcWarnBI1);
        UI = SVC_S_SVC_WARN;
        break;
    case Qt::Key_T:
        ShowTapSelect();
        ui->stackedWidget->setCurrentWidget(ui->svcTapSelect);
        UI = SVC_T_TAP_SLT;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcSoftpot(QKeyEvent *event)
{
    if(svc_softpot_copy)
    {
        SvcCopySwitch(event);
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_A:
            ui->lbSoftpotsAM->setText("AUTO");
            io->afc_loop_mode = AUTO;
            io->SendAfcLoopCtl();
            break;
        case Qt::Key_M:
            ui->lbSoftpotsAM->setText("MANUAL");
            io->afc_loop_mode = MANUAL;
            io->SendAfcLoopCtl();
            break;
        case Qt::Key_C:
            ui->lbSoftpotsCO->setText("CLOSE");
            io->dos_loop_mode = CLOSE;
            io->SendDosLoopCtl();
            break;
        case Qt::Key_O:
            ui->lbSoftpotsCO->setText("OPEN");
            io->dos_loop_mode = OPEN;
            io->SendDosLoopCtl();
            break;
        case Qt::Key_T:
            g_mode = TREATMENT;
            ui->lbSoftpotsTR->setText("TREAT");
            ProcessSoftpotSwitchPage();
            ui->leSoftpotsBeam1AfcpRsch->setFocus();////duanzhibin1215
            ui->leSoftpotsBeam2DsgxResrch->setFocus();
            ui->leSoftpotsTrigModtResrch->setFocus();
            ui->leSoftpotsDose1D1gResrch->setFocus();
            ui->leSoftpotsDose2S0gResrch->setFocus();
            ui->leSoftpotsBeam1AfcpRsch->clearFocus();
            ui->leSoftpotsBeam2DsgxResrch->clearFocus();
            ui->leSoftpotsTrigModtResrch->clearFocus();
            ui->leSoftpotsDose1D1gResrch->clearFocus();
            ui->leSoftpotsDose2S0gResrch->clearFocus();
            SendSoftpotValue();
            break;
        case Qt::Key_R:
            g_mode = RESEARCH;
            ui->lbSoftpotsTR->setText("RESRCH");
            softpot_beam1_focus = 0;
            softpot_beam2_focus = 0;
            softpot_trig_focus = 0;
            softpot_dose1_focus = 0;
            softpot_dose2_focus = 0;
            softpot_page = ui->sWSoftpot->currentIndex();
            ProcessSoftpotSelectPage();
            SendSoftpotValue();
            break;
        case Qt::Key_H:
            ui->sWSoftpot->setCurrentWidget(ui->beam1);
            if(g_mode==RESEARCH)
            {
                ProcessSoftpotSwitchPage();
                softpot_widge[0].at(softpot_beam1_focus)->setFocus();
                tmp_value = softpot_widge[0].at(softpot_beam1_focus)->text();
                softpot_widge[0].at(softpot_beam1_focus)->selectAll();
            }
            last_softpot_page = 0;
            break;
        case Qt::Key_I:
            ui->sWSoftpot->setCurrentWidget(ui->beam2);
            if(g_mode==RESEARCH)
            {
                ProcessSoftpotSwitchPage();
                softpot_widge[1].at(softpot_beam2_focus)->setFocus();
                tmp_value = softpot_widge[1].at(softpot_beam2_focus)->text();
                softpot_widge[1].at(softpot_beam2_focus)->selectAll();
            }
            last_softpot_page = 1;
            break;
        case Qt::Key_J:
            ui->sWSoftpot->setCurrentWidget(ui->trig);
            if(g_mode==RESEARCH)
            {
                ProcessSoftpotSwitchPage();
                softpot_widge[2].at(softpot_trig_focus)->setFocus();
                tmp_value = softpot_widge[2].at(softpot_trig_focus)->text();
                softpot_widge[2].at(softpot_trig_focus)->selectAll();
            }
            last_softpot_page = 2;
            break;
        case Qt::Key_K:
            ui->sWSoftpot->setCurrentWidget(ui->dos1);
            if(g_mode==RESEARCH)
            {
                ProcessSoftpotSwitchPage();
                softpot_widge[3].at(softpot_dose1_focus)->setFocus();
                tmp_value = softpot_widge[3].at(softpot_dose1_focus)->text();
                softpot_widge[3].at(softpot_dose1_focus)->selectAll();
            }
            last_softpot_page = 3;
            break;
        case Qt::Key_L:
            ui->sWSoftpot->setCurrentWidget(ui->dos2);
            if(g_mode==RESEARCH)
            {
                ProcessSoftpotSwitchPage();
                softpot_widge[4].at(softpot_dose2_focus)->setFocus();
                tmp_value = softpot_widge[4].at(softpot_dose2_focus)->text();
                softpot_widge[4].at(softpot_dose2_focus)->selectAll();
            }
            last_softpot_page = 4;
            break;
        case Qt::Key_N:
            if(g_mode==RESEARCH)
                ProcessSoftpotSwitchPage();
            softpot_page = ui->sWSoftpot->currentIndex();
            if(softpot_page >= 4)
                softpot_page = 0;
            else
                softpot_page++;
            last_softpot_page = softpot_page;
            ui->sWSoftpot->setCurrentIndex(softpot_page);
            if(g_mode==RESEARCH)
                ProcessSoftpotSelectPage();
            break;
        case Qt::Key_Q:
            if(g_mode==RESEARCH)
                ProcessSoftpotSwitchPage();
            if(ui->lineEditMsg->text()==MSG_SOFTPOT_COPY_DEV)
                ui->lineEditMsg->setText("");
            ui->stackedWidget->setCurrentWidget(ui->svcMenu);
            UI = SVC_MENU;
            break;
        case Qt::Key_S:
            SlaveBase::WriteSoftpotsFile();
            break;
        case Qt::Key_Z:
            ui->lineEditMsg->setText("Copy from (R)esearch, (D)efault, (T)reament");
            copy_mode = SOFTPOT_COPY_FROM;
            svc_softpot_copy = true;
            break;
        case Qt::Key_Down:
            //QString tmp_value ;
            if(g_mode == RESEARCH)
                ProcessSoftpotDown();
            break;
        case Qt::Key_Up:
            if(g_mode == RESEARCH)
                ProcessSoftpotUp();
            break;
        case Qt::Key_Right:
            if(g_mode == RESEARCH)
                ProcessSoftpotRight();
            break;
        case Qt::Key_Left:
            if(g_mode == RESEARCH)
                ProcessSoftpotLeft();
            break;
        default:
            ProcessSvcF1toF6Keys(event);
            break;
        }
    }

}

void AccUi::ProcessSvcCInterlock(QKeyEvent *event)
{
    int row = ui->tWPage1->currentRow();
    int num = ilk_page_index*10 + row;
    switch (event->key())
    {
    case Qt::Key_Down:
        row++;
        if(row>9)
        {
            row=0;
            ilk_page_index++;
            if(ilk_page_index>12)
                ilk_page_index=0;
            DisplayInterlockTable();
        }
        ui->tWPage1->setCurrentCell(row,0);
        break;
    case Qt::Key_Up:
        row--;
        if(row<0)
        {
            row=9;
            ilk_page_index--;
            if(ilk_page_index<0)
                ilk_page_index=12;
            DisplayInterlockTable();
        }
        ui->tWPage1->setCurrentCell(row,0);
        break;
    case Qt::Key_PageUp:
        //ui->textEdit->setReadOnly(false);
        if(ui->sWInterlock->currentWidget()==ui->interlockPage14)
        {
            QTextCursor txtCursor = ui->textEdit->textCursor();
            txtCursor.movePosition(QTextCursor::PreviousBlock,QTextCursor::MoveAnchor,10);
            ui->textEdit->setTextCursor(txtCursor);
        }
        else if(ilk_page_index <=0)
            ilk_page_index = 12;
        else
            --ilk_page_index;
        DisplayInterlockTable();
        break;
    case Qt::Key_PageDown:
        if(ui->sWInterlock->currentWidget()==ui->interlockPage14)
        {
            QTextCursor txtCursor = ui->textEdit->textCursor();
            txtCursor.movePosition(QTextCursor::NextBlock,QTextCursor::MoveAnchor,10);
            ui->textEdit->setTextCursor(txtCursor);
        }
        else if(ilk_page_index>=12)
            ilk_page_index = 0;
        else
            ++ilk_page_index;
        DisplayInterlockTable();
        break;
    case Qt::Key_Return:
        ShowInterlockError(num);
        break;
    case Qt::Key_Y:
        InterlockSetOvrd(num,true);
        break;
    case Qt::Key_N:
        InterlockSetOvrd(num,false);
        break;
    case Qt::Key_Q:
        if(ui->sWInterlock->currentWidget()==ui->interlockPage14)
            ui->sWInterlock->setCurrentWidget(ui->interlockPage1);
        else
        {
            ui->stackedWidget->setCurrentWidget(ui->svcMenu);
            UI = SVC_MENU;
        }
        break;
    case Qt::Key_X:
        //旁路所有可以旁路的联锁
        Ovrd_Availabl_Interlock();
        break;
    case Qt::Key_Z:
        //去除所有旁路
        Clear_Ovrd_Interlock();
        break;
    case Qt::Key_R:
        //消除所有history信息
        g_interlocks_mutex.lock();
        //ERROR_ST tmp;
        for(int i=0;i<128;i++)
        {
            //tmp.error_num =0;
            g_interlocks[i].history = 0;
            g_interlocks[i].last_error_num = -1;
        }
        interlock->WriteOwnFile();
        g_interlocks_mutex.unlock();
        DisplayInterlockTable();
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcDErrDisp(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        if(err_page_index ==0||err_page_index < 0)
            err_page_index =129;
        else
            --err_page_index;
        DiaplayErrorHistoryTable();
        break;
    case Qt::Key_Down:
        if(err_page_index>128)
            err_page_index = 0;
        else
            ++err_page_index;
        DiaplayErrorHistoryTable();
        break;
    case Qt::Key_Left:
        if(err_page_index<10)
            err_page_index  = 129;
        else
            err_page_index -=10;
        DiaplayErrorHistoryTable();
        break;
    case Qt::Key_Right:
        if(err_page_index>119)
            err_page_index = 0;
        else
            err_page_index +=10;
        DiaplayErrorHistoryTable();
        break;
    case Qt::Key_R:
        //row=ui->tableWidget->rowCount();
        for(int i=0;i<ERROR_NUM;i++)
        {
            g_error_history[i] =0;
        }
        DiaplayErrorHistoryTable();
        break;
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}


void AccUi::ProcessSvcECmpstFct(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        base->WriteInitFile();
        dosei->WriteOwnFile();
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_C:
        g_compensate = true;
        ui->leCpsFactorCO->setText("COMPENSATION");
        break;
    case Qt::Key_O:
        g_compensate = false;
        ui->leCpsFactorCO->setText("OVERRIDE");
        break;
    case Qt::Key_Right: //duanzhibin1203
        if(ui->leCpsFactorRefTemp->hasFocus())
        {
            SetSvcOldEdit(ui->leCpsFactorRefTemp);
            SetSvcNextEdit(ui->leCpsFactorRefPress);
        }
        else if(ui->leCpsFactorRefPress->hasFocus())
        {
            SetSvcOldEdit(ui->leCpsFactorRefPress);
            SetSvcNextEdit(ui->leCpsFactorRefTemp);
        }
        break;
    case Qt::Key_Left:
        if(ui->leCpsFactorRefTemp->hasFocus())
        {
            SetSvcOldEdit(ui->leCpsFactorRefTemp);
            SetSvcNextEdit(ui->leCpsFactorRefPress);
        }
        else if(ui->leCpsFactorRefPress->hasFocus())
        {
            SetSvcOldEdit(ui->leCpsFactorRefPress);
            SetSvcNextEdit(ui->leCpsFactorRefTemp);
        }
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcFVerif(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcGMeanAtt(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_Left:  //duanzhibin1203
        if(ui->leMeanAttLoCalibrFact->hasFocus())
        {
            SetSvcOldEdit(ui->leMeanAttLoCalibrFact);
            SetSvcNextEdit(ui->leMeanAttHiCalibrFact);
        }
        else if(ui->leMeanAttHiCalibrFact->hasFocus())
        {
            SetSvcOldEdit(ui->leMeanAttHiCalibrFact);
            SetSvcNextEdit(ui->leMeanAttLoCalibrFact);
        }
        break;
    case Qt::Key_Right:
        if(ui->leMeanAttLoCalibrFact->hasFocus())
        {
            SetSvcOldEdit(ui->leMeanAttLoCalibrFact);
            SetSvcNextEdit(ui->leMeanAttHiCalibrFact);
        }
        else if(ui->leMeanAttHiCalibrFact->hasFocus())
        {
            SetSvcOldEdit(ui->leMeanAttHiCalibrFact);
            SetSvcNextEdit(ui->leMeanAttLoCalibrFact);
        }
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcHSipAnaly(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_C:
        if(isSipNormalWork)
            ui->leSipAnalyzerState->setText("Check Sucess");
        else
            ui->leSipAnalyzerState->setText("Check Failure");
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcIMotorAdj(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_S:
        motor->WriteOwnFile();  //将slow speed和fast speed写入配置文件
        break;
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_Left:
        SetMotAdjLeftEdit(motadj_row,motadj_column);
        break;
    case Qt::Key_Right:
        SetMotAdjRightEdit(motadj_row,motadj_column);
        break;
    case Qt::Key_Up:
        SetMotAdjUpEdit(motadj_row,motadj_column);
        break;
    case Qt::Key_Down:
        SetMotAdjDownnEdit(motadj_row,motadj_column);
        break;
    case Qt::Key_C:
//        motadj_length_size = 6;
//        motadj_width_size = 6;
        break;

    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}



void AccUi::ProcessSvcJChkDisp(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        base->WriteInitFile();
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_A:
        g_check1_bypass ^= true;
        if(g_check1_bypass)
        {
            ui->leChkDispBypass1->setText("BYPASS 1");
        }
        else
        {
            ui->leChkDispBypass1->setText("");
        }
        break;
    case Qt::Key_B:
        g_check2_bypass ^= true;
        if(g_check2_bypass)
        {
            ui->leChkDispBypass2->setText("BYPASS 2");
        }
        else
        {
            ui->leChkDispBypass2->setText("");
        }
        break;
    case Qt::Key_C:
        ui->leChkDispReport0->setText("");
        ui->leChkDispReport1->setText("");
        ui->leChkDispReport2->setText("");
        ui->leChkDispReport3->setText("");
        ui->leChkDispReport5->setText("");
        ui->leChkDispReport6->setText("");
        ui->leChkDispReport7->setText("");
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcKT1Disp(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcLProhbSec(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcMFoilSel(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSoftpotSelectPage()
{
    switch(softpot_page)
    {
    case 0:     //beam1
        softpot_widge[0].at(softpot_beam1_focus)->setFocus();
        tmp_value = softpot_widge[0].at(softpot_beam1_focus)->text();
        softpot_widge[0].at(softpot_beam1_focus)->selectAll();
        break;
    case 1:     //beam2
        softpot_widge[1].at(softpot_beam2_focus)->setFocus();
        tmp_value = softpot_widge[1].at(softpot_beam2_focus)->text();
        softpot_widge[1].at(softpot_beam2_focus)->selectAll();
        break;
    case 2:     //trig
        softpot_widge[2].at(softpot_trig_focus)->setFocus();
        tmp_value = softpot_widge[2].at(softpot_trig_focus)->text();
        softpot_widge[2].at(softpot_trig_focus)->selectAll();
        break;
    case 3:     //dose1
        softpot_widge[3].at(softpot_dose1_focus)->setFocus();
        tmp_value = softpot_widge[3].at(softpot_dose1_focus)->text();
        softpot_widge[3].at(softpot_dose1_focus)->selectAll();
        break;
    case 4:     //dose2
        softpot_widge[4].at(softpot_dose2_focus)->setFocus();
        tmp_value = softpot_widge[4].at(softpot_dose2_focus)->text();
        softpot_widge[4].at(softpot_dose2_focus)->selectAll();
        break;
    default:
        break;
    }
}

void AccUi::ProcessSoftpotSwitchPage()
{
    if(has_press_return)
    {
        has_press_return = false;
    }
    else
    {
        switch(last_softpot_page)
        {
        case 0:     //beam1
            softpot_widge[0].at(softpot_beam1_focus)->setText(tmp_value);
            break;
        case 1:     //beam2
            softpot_widge[1].at(softpot_beam2_focus)->setText(tmp_value);
            // first_input_beam2 = true;
            break;
        case 2:     //trig
            softpot_widge[2].at(softpot_trig_focus)->setText(tmp_value);
            break;
        case 3:     //dose1
            softpot_widge[3].at(softpot_dose1_focus)->setText(tmp_value);
            break;
        case 4:     //dose2
            softpot_widge[4].at(softpot_dose2_focus)->setText(tmp_value);
            break;
        default:
            break;
        }
    }
}

void AccUi::ProcessSoftpotUp()
{
    if(ui->sWSoftpot->currentWidget() == ui->beam1)
    {
        if(ui->leSoftpotsBeam1AfcpRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1AfcpRsch);
            on_leSoftpotsBeam1AfcpRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1BmiRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1BmiRsch);
            on_leSoftpotsBeam1BmiRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1ElmRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1ElmRsch);
            on_leSoftpotsBeam1ElmRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1InjeRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1InjeRsch);
            on_leSoftpotsBeam1InjeRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1InjiRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1InjiRsch);
            on_leSoftpotsBeam1InjiRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1IpfnRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1IpfnRsch);
            on_leSoftpotsBeam1IpfnRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1LensRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1LensRsch);
            on_leSoftpotsBeam1LensRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1PfnRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1PfnRsch);
            on_leSoftpotsBeam1PfnRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1SominRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1SominRsch);
            on_leSoftpotsBeam1SominRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1StcRsch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam1StcRsch);
            on_leSoftpotsBeam1StcRsch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() == ui->beam2)
    {
        if(ui->leSoftpotsBeam2BalxResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2BalxResrch);
            on_leSoftpotsBeam2BalxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2BalyResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2BalyResrch);
            on_leSoftpotsBeam2BalyResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2BmickResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsBeam2BmickResrch);
            on_leSoftpotsBeam2BmickResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DsgxResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2DsgxResrch);
            on_leSoftpotsBeam2DsgxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DsgyResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2DsgyResrch);
            on_leSoftpotsBeam2DsgyResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DswxResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2DswxResrch);
            on_leSoftpotsBeam2DswxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DswyResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2DswyResrch);
            on_leSoftpotsBeam2DswyResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2ElmckResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsBeam2ElmckResrch);
            on_leSoftpotsBeam2ElmckResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2LenckResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsBeam2LenckResrch);
            on_leSoftpotsBeam2LenckResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2StcckResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsBeam2StcckResrch);
            on_leSoftpotsBeam2StcckResrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() ==  ui->trig)
    {
        if(ui->leSoftpotsTrigAfctResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigAfctResrch);
            on_leSoftpotsTrigAfctResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigInjtResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigInjtResrch);
            on_leSoftpotsTrigInjtResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigModtResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigModtResrch);
            on_leSoftpotsTrigModtResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigRftResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigRftResrch);
            on_leSoftpotsTrigRftResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigTst0Resrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigTst0Resrch);
            on_leSoftpotsTrigTst0Resrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigTst1Resrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsTrigTst1Resrch);
            on_leSoftpotsTrigTst1Resrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos1)
    {
        if(ui->leSoftpotsDose1D1gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose1D1gResrch);
            on_leSoftpotsDose1D1gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1D2gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose1D2gResrch);
            on_leSoftpotsDose1D2gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1DrcvResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose1DrcvResrch);
            on_leSoftpotsDose1DrcvResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1PrfpResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose1PrfpResrch);
            on_leSoftpotsDose1PrfpResrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget()==ui->dos2)
    {
        if(ui->leSoftpotsDose2S0gResrch->hasFocus())
        {
            SetSoftpotDownEdit( ui->leSoftpotsDose2S0gResrch);
            on_leSoftpotsDose2S0gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S1gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose2S1gResrch);
            on_leSoftpotsDose2S1gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S2gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose2S2gResrch);
            on_leSoftpotsDose2S2gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S3gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose2S3gResrch);
            on_leSoftpotsDose2S3gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S4gResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose2S4gResrch);
            on_leSoftpotsDose2S4gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2SwiResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsDose2SwiResrch);
            on_leSoftpotsDose2SwiResrch_returnPressed();
        }
        else if( ui->leSoftpotsDose2FwiResrch->hasFocus())
        {
            SetSoftpotDownFloatEdit(ui->leSoftpotsDose2FwiResrch);
            on_leSoftpotsDose2FwiResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2DostResrch->hasFocus())
        {
            SetSoftpotDownEdit(ui->leSoftpotsDose2DostResrch);
            on_leSoftpotsDose2DostResrch_returnPressed();
        }
    }
}

void AccUi::ProcessSoftpotDown()
{
    if(ui->sWSoftpot->currentWidget() == ui->beam1)
    {
        if(ui->leSoftpotsBeam1AfcpRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1AfcpRsch);
            on_leSoftpotsBeam1AfcpRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1BmiRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1BmiRsch);
            on_leSoftpotsBeam1BmiRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1ElmRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1ElmRsch);
            on_leSoftpotsBeam1ElmRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1InjeRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1InjeRsch);
            on_leSoftpotsBeam1InjeRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1InjiRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1InjiRsch);
            on_leSoftpotsBeam1InjiRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1IpfnRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1IpfnRsch);
            on_leSoftpotsBeam1IpfnRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1LensRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1LensRsch);
            on_leSoftpotsBeam1LensRsch_returnPressed();
        }
        else if( ui->leSoftpotsBeam1PfnRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1PfnRsch);
            on_leSoftpotsBeam1PfnRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1SominRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1SominRsch);
            on_leSoftpotsBeam1SominRsch_returnPressed();
        }
        else if(ui->leSoftpotsBeam1StcRsch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam1StcRsch);
            on_leSoftpotsBeam1StcRsch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() == ui->beam2)
    {
        if(ui->leSoftpotsBeam2BalxResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2BalxResrch);
            on_leSoftpotsBeam2BalxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2BalyResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2BalyResrch);
            on_leSoftpotsBeam2BalyResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2BmickResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsBeam2BmickResrch);
            on_leSoftpotsBeam2BmickResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DsgxResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2DsgxResrch);
            on_leSoftpotsBeam2DsgxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DsgyResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2DsgyResrch);
            on_leSoftpotsBeam2DsgxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DswxResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2DswxResrch);
            on_leSoftpotsBeam2DswxResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2DswyResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2DswyResrch);
            on_leSoftpotsBeam2DswyResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2ElmckResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsBeam2ElmckResrch);
            on_leSoftpotsBeam2ElmckResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2LenckResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsBeam2LenckResrch);
            on_leSoftpotsBeam2LenckResrch_returnPressed();
        }
        else if(ui->leSoftpotsBeam2StcckResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsBeam2StcckResrch);
            on_leSoftpotsBeam2StcckResrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() ==  ui->trig)
    {
        if(ui->leSoftpotsTrigAfctResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigAfctResrch);
            on_leSoftpotsTrigAfctResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigInjtResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigInjtResrch);
            on_leSoftpotsTrigInjtResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigModtResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigModtResrch);
            on_leSoftpotsTrigModtResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigRftResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigRftResrch);
            on_leSoftpotsTrigRftResrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigTst0Resrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigTst0Resrch);
            on_leSoftpotsTrigTst0Resrch_returnPressed();
        }
        else if(ui->leSoftpotsTrigTst1Resrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsTrigTst1Resrch);
            on_leSoftpotsTrigTst1Resrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos1)
    {
        if(ui->leSoftpotsDose1D1gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose1D1gResrch);
            on_leSoftpotsDose1D1gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1D2gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose1D2gResrch);
            on_leSoftpotsDose1D2gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1DrcvResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose1DrcvResrch);
            on_leSoftpotsDose1DrcvResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose1PrfpResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose1PrfpResrch);
            on_leSoftpotsDose1PrfpResrch_returnPressed();
        }
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos2)
    {
        if(ui->leSoftpotsDose2S0gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2S0gResrch);
            on_leSoftpotsDose2S0gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S1gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2S1gResrch);
            on_leSoftpotsDose2S1gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S2gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2S2gResrch);
            on_leSoftpotsDose2S2gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S3gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2S3gResrch);
            on_leSoftpotsDose2S3gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2S4gResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2S4gResrch);
            on_leSoftpotsDose2S4gResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2SwiResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsDose2SwiResrch);
            on_leSoftpotsDose2SwiResrch_returnPressed();
        }
        else if( ui->leSoftpotsDose2FwiResrch->hasFocus())
        {
            SetSoftpotUpFloatEdit(ui->leSoftpotsDose2FwiResrch);
            on_leSoftpotsDose2FwiResrch_returnPressed();
        }
        else if(ui->leSoftpotsDose2DostResrch->hasFocus())
        {
            SetSoftpotUpEdit(ui->leSoftpotsDose2DostResrch);
            on_leSoftpotsDose2DostResrch_returnPressed();
        }
    }
}

void AccUi::ProcessSoftpotLeft()
{
    if(ui->sWSoftpot->currentWidget() == ui->beam1)
    {
        SetSoftpotOldEdit(softpot_widge[0].at(softpot_beam1_focus));
        softpot_beam1_focus = (softpot_beam1_focus-1+softpot_widge[0].size())%softpot_widge[0].size();
        SetSoftpotNextEdit(softpot_widge[0].at(softpot_beam1_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->beam2)
    {
        SetSoftpotOldEdit(softpot_widge[1].at(softpot_beam2_focus));
        softpot_beam2_focus = (softpot_beam2_focus-1+softpot_widge[1].size())%softpot_widge[1].size();
        SetSoftpotNextEdit(softpot_widge[1].at(softpot_beam2_focus));
    }
    else if(ui->sWSoftpot->currentWidget() ==  ui->trig)
    {
        SetSoftpotOldEdit(softpot_widge[2].at(softpot_trig_focus));
        softpot_trig_focus = (softpot_trig_focus-1+softpot_widge[2].size())%softpot_widge[2].size();
        SetSoftpotNextEdit(softpot_widge[2].at(softpot_trig_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos1)
    {
        SetSoftpotOldEdit(softpot_widge[3].at(softpot_dose1_focus));
        softpot_dose1_focus = (softpot_dose1_focus-1+softpot_widge[3].size())%softpot_widge[3].size();
        SetSoftpotNextEdit(softpot_widge[3].at(softpot_dose1_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos2)
    {
        SetSoftpotOldEdit(softpot_widge[4].at(softpot_dose2_focus));
        softpot_dose2_focus = (softpot_dose2_focus-1+softpot_widge[4].size())%softpot_widge[4].size();
        SetSoftpotNextEdit(softpot_widge[4].at(softpot_dose2_focus));

    }

}

void AccUi::ProcessSoftpotRight()
{
    if(ui->sWSoftpot->currentWidget() == ui->beam1)
    {
        SetSoftpotOldEdit(softpot_widge[0].at(softpot_beam1_focus));
        softpot_beam1_focus = (softpot_beam1_focus+1)%softpot_widge[0].size();
        SetSoftpotNextEdit(softpot_widge[0].at(softpot_beam1_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->beam2)
    {
        SetSoftpotOldEdit(softpot_widge[1].at(softpot_beam2_focus));
        softpot_beam2_focus = (softpot_beam2_focus+1)%softpot_widge[1].size();
        SetSoftpotNextEdit(softpot_widge[1].at(softpot_beam2_focus));
    }
    else if(ui->sWSoftpot->currentWidget() ==  ui->trig)
    {
        SetSoftpotOldEdit(softpot_widge[2].at(softpot_trig_focus));
        softpot_trig_focus = (softpot_trig_focus+1)%softpot_widge[2].size();
        SetSoftpotNextEdit(softpot_widge[2].at(softpot_trig_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos1)
    {
        SetSoftpotOldEdit(softpot_widge[3].at(softpot_dose1_focus));
        softpot_dose1_focus = (softpot_dose1_focus+1)%softpot_widge[3].size();
        SetSoftpotNextEdit(softpot_widge[3].at(softpot_dose1_focus));
    }
    else if(ui->sWSoftpot->currentWidget() == ui->dos2)
    {
        SetSoftpotOldEdit(softpot_widge[4].at(softpot_dose2_focus));
        softpot_dose2_focus = (softpot_dose2_focus+1)%softpot_widge[4].size();
        SetSoftpotNextEdit(softpot_widge[4].at(softpot_dose2_focus));
    }

}

//y=kx+b    y=UINT16
//void AccUi::ComputeGanCollikb(UINT16 y1, UINT16 y2, float x1, float x2, float &k, float &b)
//{
//    k = ((float)y1 - (float)y2) / (x1 - x2);
//    b = (float)y1 - k * x1;
//}

//void AccUi::ComputeMotorkb(UINT16 y1, UINT16 y2, float x1, float x2, float &k, float &b)
//{
//    k = ((float)y1 - (float)y2) / (x1 - x2);
//    b = (float)y1 - k * x1;
//}

void AccUi::ComputeMotorkb(UINT16 y1, UINT16 y2, float x1, float x2, float &k, float &b)
{
    k = ((float)y1 - (float)y2) / (x1 - x2);
    b = (float)y1 - k * x1;
}

void AccUi::ProcessSvcNMotorCal(QKeyEvent *event)
{
    QString temp;
    float pos;
    float recal_msu_pos;            //重新计算得到的测量值
    float jaw_req_pos1, jaw_req_pos2;
    float k1,k2,b1,b2,k,b;
    int index = ui->sWMotorCalib->currentIndex();

    switch (event->key()) {
    case Qt::Key_Right:
        switch(index)
        {
        case 0:
            if(ui->leMotClbGantryRefPosn1->hasFocus())
            {
                ui->leMotClbGantryRefPosn2->setFocus();
            }
            else if(ui->leMotClbGantryRefPosn2->hasFocus())
            {
                ui->leMotClbGantryRefPosn3->setFocus();
            }
            break;
        case 1:
            if(ui->leMotClbJawY1ReqPosn1->hasFocus())
            {
                ui->leMotClbJawY1ReqPosn2->setFocus();
            }
            break;
        case 2:
            if(ui->leMotClbJawY2ReqPosn1->hasFocus())
            {
                ui->leMotClbJawY2ReqPosn2->setFocus();
            }
            break;
        case 3:
            if(ui->leMotClbJawX2ReqPosn1->hasFocus())
            {
                ui->leMotClbJawX2ReqPosn2->setFocus();
            }
            break;
        case 4:
            if(ui->leMotClbJawX1ReqPosn1->hasFocus())
            {
                ui->leMotClbJawX1ReqPosn2->setFocus();
            }
            break;
        case 5:
            if(ui->leMotClbColliRefPosn1->hasFocus())
            {
                ui->leMotClbColliRefPosn2->setFocus();
            }
            else if(ui->leMotClbColliRefPosn2->hasFocus())
            {
                ui->leMotClbColliRefPosn3->setFocus();
            }
            break;
        default:
            break;
        }
        ShowMotorCalibFocus();
        break;
    case Qt::Key_Left:
        switch(index)
        {
        case 0:
            if(ui->leMotClbGantryRefPosn3->hasFocus())
                ui->leMotClbGantryRefPosn2->setFocus();
            else if(ui->leMotClbGantryRefPosn2->hasFocus())
                ui->leMotClbGantryRefPosn1->setFocus();
            break;
        case 1:
            if(ui->leMotClbJawY1ReqPosn2->hasFocus())
                ui->leMotClbJawY1ReqPosn1->setFocus();
            break;
        case 2:
            if(ui->leMotClbJawY2ReqPosn2->hasFocus())
                ui->leMotClbJawY2ReqPosn1->setFocus();
            break;
        case 3:
            if(ui->leMotClbJawX2ReqPosn2->hasFocus())
                ui->leMotClbJawX2ReqPosn1->setFocus();
            break;
        case 4:
            if(ui->leMotClbJawX1ReqPosn2->hasFocus())
                ui->leMotClbJawX1ReqPosn1->setFocus();
            break;
        case 5:
            if(ui->leMotClbColliRefPosn3->hasFocus())
                ui->leMotClbColliRefPosn2->setFocus();
            else if(ui->leMotClbColliRefPosn2->hasFocus())
                ui->leMotClbColliRefPosn1->setFocus();
            break;
        default:
            break;
        }
        ShowMotorCalibFocus();
        break;
    case Qt::Key_N:
        if(motor_calib_page < 5)
        {
            motor_calib_page++;
        }
        else
        {
            motor_calib_page = 0;
        }
        ui->sWMotorCalib->setCurrentIndex(motor_calib_page);
        ShowMotorCalibFocus();
        break;
    case Qt::Key_P:
        if(motor_calib_page > 0)
        {
            motor_calib_page--;
        }
        else
        {
            motor_calib_page = 5;
        }
        ui->sWMotorCalib->setCurrentIndex(motor_calib_page);
        ShowMotorCalibFocus();
        break;
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_C:
        w = focusWidget();
        if(w == ui->leMotClbGantryRefPosn1)
        {
            gantry_calib_pos[0] =motor->gantry_position_speed.gantry_position;
            temp.sprintf("%x", gantry_calib_pos[0]);
            ui->leMotClbGantryCapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(gantry_calib_pos[0], GANTRY_CK);
            if(pos<0) pos=pos+360;
            temp.sprintf("%5.1f", pos);
            ui->leMotClbGantryCapPosn1->setText(temp);
        }
        else if(w == ui->leMotClbGantryRefPosn2)
        {
            gantry_calib_pos[1] = motor->gantry_position_speed.gantry_position;
            temp.sprintf("%x", gantry_calib_pos[1]);
            ui->leMotClbGantryCapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(gantry_calib_pos[1], GANTRY_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbGantryCapPosn2->setText(temp);
        }
        else if(w == ui->leMotClbGantryRefPosn3)
        {
            gantry_calib_pos[2] = motor->gantry_position_speed.gantry_position;
            temp.sprintf("%x", gantry_calib_pos[2]);
            ui->leMotClbGantryCapCount3->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(gantry_calib_pos[2], GANTRY_CK);
            if(pos<0) pos=pos+360;
            temp.sprintf("%5.1f", pos);
            ui->leMotClbGantryCapPosn3->setText(temp);
        }
        else if(w == ui->leMotClbColliRefPosn1)
        {
            cilli_calib_pos[0] = motor->position[COLLIMATOR];
            temp.sprintf("%x", motor->position[COLLIMATOR]);
            ui->leMotClbColliCapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);
            if(pos<0) pos=pos+360;
            temp.sprintf("%5.1f", pos);
            ui->leMotClbColliCapPosn1->setText(temp);
        }
        else if(w == ui->leMotClbColliRefPosn2)
        {
            cilli_calib_pos[1] = motor->position[COLLIMATOR];
            temp.sprintf("%x", motor->position[COLLIMATOR]);
            ui->leMotClbColliCapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbColliCapPosn2->setText(temp);
        }
        else if(w == ui->leMotClbColliRefPosn3)
        {
            cilli_calib_pos[2] = motor->position[COLLIMATOR];
            temp.sprintf("%x", motor->position[COLLIMATOR]);
            ui->leMotClbColliCapCount3->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbColliCapPosn3->setText(temp);
        }
        else if(w == ui->leMotClbJawY1ReqPosn1)
        {
            jaw_y1_pos[0] = motor->position[JAW_Y1];
            temp.sprintf("%x", motor->position[JAW_Y1]);
            ui->leMotClbJawY1CapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_Y1], JAWY1_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawY1MsuPosn1->setText(temp);
        }
        else if(w == ui->leMotClbJawY1ReqPosn2)
        {
            jaw_y1_pos[1] = motor->position[JAW_Y1];
            temp.sprintf("%x", motor->position[JAW_Y1]);
            ui->leMotClbJawY1CapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_Y1], JAWY1_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawY1MsuPosn2->setText(temp);
        }
        else if(w == ui->leMotClbJawY2ReqPosn1)
        {
            jaw_y2_pos[0] = motor->position[JAW_Y2];
            temp.sprintf("%x", motor->position[JAW_Y2]);
            ui->leMotClbJawY2CapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_Y2], JAWY2_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawY2MsuPosn1->setText(temp);
        }
        else if(w == ui->leMotClbJawY2ReqPosn2)
        {
            jaw_y2_pos[1] = motor->position[JAW_Y2];
            temp.sprintf("%x", motor->position[JAW_Y2]);
            ui->leMotClbJawY2CapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_Y2], JAWY2_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawY2MsuPosn2->setText(temp);
        }
        else if(w == ui->leMotClbJawX2ReqPosn1)
        {
            jaw_x2_pos[0] = motor->position[JAW_X2];
            temp.sprintf("%x", motor->position[JAW_X2]);
            ui->leMotClbJawX2CapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_X2], JAWX2_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawX2MsuPosn1->setText(temp);
        }
        else if(w == ui->leMotClbJawX2ReqPosn2)
        {
            jaw_x2_pos[1] = motor->position[JAW_X2];
            temp.sprintf("%x", motor->position[JAW_X2]);
            ui->leMotClbJawX2CapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_X2], JAWX2_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawX2MsuPosn2->setText(temp);
        }
        else if(w == ui->leMotClbJawX1ReqPosn1)
        {
            jaw_x1_pos[0] = motor->position[JAW_X1];
            temp.sprintf("%x", motor->position[JAW_X1]);
            ui->leMotClbJawX1CapCount1->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_X1], JAWX1_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawX1MsuPosn1->setText(temp);
        }
        else if(w == ui->leMotClbJawX1ReqPosn2)
        {
            jaw_x1_pos[1] = motor->position[JAW_X1];
            temp.sprintf("%x", motor->position[JAW_X1]);
            ui->leMotClbJawX1CapCount2->setText(temp);
            pos = (float)SlaveBase::ConvertMemberToFloat(motor->position[JAW_X1], JAWX1_CK);
            temp.sprintf("%5.1f", pos);
            ui->leMotClbJawX1MsuPosn2->setText(temp);
        }
        break;
    case Qt::Key_R:
        // compute gain and offset
        motor_calib_page=ui->sWMotorCalib->currentIndex();
        switch(motor_calib_page)
        {
        case 0:
            //gantry
            ComputeMotorkb(gantry_calib_pos[0], gantry_calib_pos[1],
                    180, 0, k1, b1);
            ComputeMotorkb(gantry_calib_pos[1], gantry_calib_pos[2],
                    0, -180, k2, b2);
            k=(k1+k2)/2.0f;
            b=(b1+b2)/2.0f;
            motor->gantry_gain=-1.0f/k;
            motor->gantry_offset=190+b/k;
            g_edge_arr[GANTRY_CK].k=k;
            g_edge_arr[GANTRY_CK].b=b;

            recal_msu_pos=base->ConvertMemberToFloat(gantry_calib_pos[0],GANTRY_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbGantryCapPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(gantry_calib_pos[1],GANTRY_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbGantryCapPosn2->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(gantry_calib_pos[2],GANTRY_CK);
            if(recal_msu_pos<0) recal_msu_pos=recal_msu_pos+360;
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbGantryCapPosn3->setText(temp);

            temp.sprintf("%f", motor->gantry_gain);
            ui->leMotClbGantryGain->setText(temp);
            temp.sprintf("%f", motor->gantry_offset);
            ui->leMotClbGantryOffset->setText(temp);
            break;

        case 1:
            //JAW Y1
            //公式: y = k * x + b
            jaw_req_pos1 = ui->leMotClbJawY1ReqPosn1->text().toFloat();
            jaw_req_pos2 = ui->leMotClbJawY1ReqPosn2->text().toFloat();
            if(jaw_req_pos1<0.1 || jaw_req_pos2<0.1)
            {
                ui->lineEditMsg->setText("Please input Requested Position.");
                return;
            }
            ComputeMotorkb(jaw_y1_pos[0], jaw_y1_pos[1], jaw_req_pos1,
                    jaw_req_pos2, k, b);
            motor->jaw_y1_gain = 1.0f/k;
            motor->jaw_y1_offset = -b/k + 11.0;
            g_edge_arr[JAWY1_CK].k=k;
            g_edge_arr[JAWY1_CK].b=b;

            //recal_req_pos=(float)jaw_y1_pos[0] * k + b;
            recal_msu_pos=base->ConvertMemberToFloat(jaw_y1_pos[0],JAWY1_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawY1MsuPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(jaw_y1_pos[1],JAWY1_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawY1MsuPosn2->setText(temp);

            temp.sprintf("%f", motor->jaw_y1_gain);
            ui->leMotClJawY1Gain->setText(temp);
            temp.sprintf("%f", motor->jaw_y1_offset);
            ui->leMotClbJawY1Offset->setText(temp);
            break;

        case 2:
            //JAW Y2
            //公式: y = k * x + b
            jaw_req_pos1 = ui->leMotClbJawY2ReqPosn1->text().toFloat();
            jaw_req_pos2 = ui->leMotClbJawY2ReqPosn2->text().toFloat();
            if(jaw_req_pos1<0.1 || jaw_req_pos2<0.1)
            {
                ui->lineEditMsg->setText("Please input Requested Position.");
                return;
            }
            ComputeMotorkb(jaw_y2_pos[0], jaw_y2_pos[1], jaw_req_pos1,
                    jaw_req_pos2, k, b);
            motor->jaw_y2_gain = 1.0f/k;
            motor->jaw_y2_offset = -b/k + 11.0;
            g_edge_arr[JAWY2_CK].k=k;
            g_edge_arr[JAWY2_CK].b=b;

            recal_msu_pos=base->ConvertMemberToFloat(jaw_y2_pos[0],JAWY2_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawY2MsuPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(jaw_y2_pos[1],JAWY2_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawY2MsuPosn2->setText(temp);

            temp.sprintf("%f", motor->jaw_y2_gain);
            ui->leMotClbJawY2Gain->setText(temp);
            temp.sprintf("%f", motor->jaw_y2_offset);
            ui->leMotClbJawY2Offset->setText(temp);
            break;

        case 3:
            //JAW X2
            //公式: y = k * x + b
            jaw_req_pos1 = ui->leMotClbJawX2ReqPosn1->text().toFloat();
            jaw_req_pos2 = ui->leMotClbJawX2ReqPosn2->text().toFloat();
            if(jaw_req_pos1<0.1 || jaw_req_pos2<0.1)
            {
                ui->lineEditMsg->setText("Please input Requested Position.");
                return;
            }
            ComputeMotorkb(jaw_x2_pos[0], jaw_x2_pos[1], jaw_req_pos1,
                    jaw_req_pos2, k, b);
            motor->jaw_x2_gain = 1.0f/k;
            motor->jaw_x2_offset = -b/k + 3.0;
            g_edge_arr[JAWX2_CK].k=k;
            g_edge_arr[JAWX2_CK].b=b;

            recal_msu_pos=base->ConvertMemberToFloat(jaw_x2_pos[0],JAWX2_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawX2MsuPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(jaw_x2_pos[1],JAWX2_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawX2MsuPosn2->setText(temp);

            temp.sprintf("%f", motor->jaw_x2_gain);
            ui->leMotClbJawX2Gain->setText(temp);
            temp.sprintf("%f", motor->jaw_x2_offset);
            ui->leMotClbJawX2Offset->setText(temp);
            break;

        case 4:
            //JAW X1
            //公式: y = k * x + b
            jaw_req_pos1 = ui->leMotClbJawX1ReqPosn1->text().toFloat();
            jaw_req_pos2 = ui->leMotClbJawX1ReqPosn2->text().toFloat();
            if(jaw_req_pos1<0.1 || jaw_req_pos2<0.1)
            {
                ui->lineEditMsg->setText("Please input Requested Position.");
                return;
            }
            ComputeMotorkb(jaw_x1_pos[0], jaw_x1_pos[1], jaw_req_pos1,
                    jaw_req_pos2, k, b);
            motor->jaw_x1_gain = 1.0f/k;
            motor->jaw_x1_offset = -b/k + 3.0;
            g_edge_arr[JAWX1_CK].k=k;
            g_edge_arr[JAWX1_CK].b=b;

            recal_msu_pos=base->ConvertMemberToFloat(jaw_x1_pos[0],JAWX1_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawX1MsuPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(jaw_x1_pos[1],JAWX1_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbJawX1MsuPosn2->setText(temp);

            temp.sprintf("%f", motor->jaw_x1_gain);
            ui->leMotClbJawX1Gain->setText(temp);
            temp.sprintf("%f", motor->jaw_x1_offset);
            ui->leMotClbJawX1Offset->setText(temp);
            break;

        case 5:
            //COLLI
            //公式: y = k * x + b
            ComputeMotorkb(cilli_calib_pos[0], cilli_calib_pos[1],
                    -90.0, 0.0, k1, b1);
            ComputeMotorkb(cilli_calib_pos[1], cilli_calib_pos[2],
                    0.0, 90.0, k2, b2);
            k=(k1+k2)/2.0f;
            b=(b1+b2)/2.0f;
            motor->colli_gain=1.0f/k;
            motor->colli_offset=-b/k;
            g_edge_arr[COLLI_CK].k=k;
            g_edge_arr[COLLI_CK].b=b;

            recal_msu_pos=base->ConvertMemberToFloat(cilli_calib_pos[0],COLLI_CK);
            if(recal_msu_pos<0.0) recal_msu_pos=recal_msu_pos+360.0;
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbColliCapPosn1->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(cilli_calib_pos[1],COLLI_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbColliCapPosn2->setText(temp);
            recal_msu_pos=base->ConvertMemberToFloat(cilli_calib_pos[2],COLLI_CK);
            temp.sprintf("%4.1f",recal_msu_pos);
            ui->leMotClbColliCapPosn3->setText(temp);

            temp.sprintf("%f", motor->colli_gain);
            ui->leMotClbColliGain->setText(temp);
            temp.sprintf("%f", motor->colli_offset);
            ui->leMotClbColliOffset->setText(temp);
            break;

        default:
            break;
        }
        motor->WriteOwnFile();  //将各页面的gain和offset写入配置文件
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcORecalConf(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_R:
        if(RecallConfigFiles())
        {
            ui->leRecallConfigStatus->setText("Recall completed");
        }
        else
        {
            ui->leRecallConfigStatus->setText("Recall error");
        }
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcPSetColTyp(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcQGanSpdCal(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_Up:  //duanzhibin1203
        if(ui->leGantrySpdFactClk->hasFocus())
        {
            SetSvcOldEdit(ui->leGantrySpdFactClk);
            SetSvcNextEdit(ui->leGantrySpdFactCountClk);
        }
        else if(ui->leGantrySpdFactCountClk->hasFocus())
        {
            SetSvcOldEdit(ui->leGantrySpdFactCountClk);
            SetSvcNextEdit(ui->leGantrySpdFactClk);
        }
        break;
    case Qt::Key_Down:
        if(ui->leGantrySpdFactClk->hasFocus())
        {
            SetSvcOldEdit(ui->leGantrySpdFactClk);
            SetSvcNextEdit(ui->leGantrySpdFactCountClk);
        }
        else if(ui->leGantrySpdFactCountClk->hasFocus())
        {
            SetSvcOldEdit(ui->leGantrySpdFactCountClk);
            SetSvcNextEdit(ui->leGantrySpdFactClk);
        }
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcRSoftpotLoad(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_U:
        updownload_file_num = 0;
        ui->leSptUpDownStatus->setText("Have  uploaded  0/24  files.");
        SoftpotFtpUpload();
        break;
    case Qt::Key_D:
        updownload_file_num = 0;
        ui->leSptUpDownStatus->setText("Have  downloaded  0/24  files.");
        SoftpotFtpDownload();
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcSSvcWarn(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Q:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_Left:
        for(int i=0;i<svc_warn_size;i++)
        {
            if(svc_waring.at(i)->hasFocus())
            {
                SetSvcOldEdit(svc_waring.at(i));
                SetSvcNextEdit(svc_waring.at((i-1+svc_warn_size)%svc_warn_size));
                break;
            }
        }
        break;
    case Qt::Key_Right:
        //svc_size =svc_waring.size();
        for(int i= 0;i<svc_warn_size;i++)
        {
            if(svc_waring.at(i)->hasFocus())
            {
                SetSvcOldEdit(svc_waring.at(i));
                SetSvcNextEdit(svc_waring.at((i+1)%svc_warn_size));
                break;
            }
        }
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcTTapSel(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Escape:
        ui->stackedWidget->setCurrentWidget(ui->svcMenu);
        UI = SVC_MENU;
        break;
    case Qt::Key_L:
        ProcessSvcTTapSelInput(QString("L"));
        break;
    case Qt::Key_M:
        ProcessSvcTTapSelInput(QString("M"));
        break;
    case Qt::Key_H:
        ProcessSvcTTapSelInput(QString("H"));
        break;
    case Qt::Key_N:
        ProcessSvcTTapSelInput(QString("N"));
        break;
    default:
        ProcessSvcF1toF6Keys(event);
        break;
    }
}

void AccUi::ProcessSvcTTapSelInput(QString str)
{
    w = focusWidget();
    if(w == ui->leTapSelectNewE1)
    {
        ui->leTapSelectNewE1->setText(str);
    }
    else if(w == ui->leTapSelectNewE2)
    {
        ui->leTapSelectNewE2->setText(str);
    }
    else if(w == ui->leTapSelectNewE3)
    {
        ui->leTapSelectNewE3->setText(str);
    }
    else if(w == ui->leTapSelectNewE4)
    {
        ui->leTapSelectNewE4->setText(str);
    }
    else if(w == ui->leTapSelectNewE5)
    {
        ui->leTapSelectNewE5->setText(str);
    }
    else if(w == ui->leTapSelectNewE6)
    {
        ui->leTapSelectNewE6->setText(str);
    }
    else if(w == ui->leTapSelectNewXL)
    {
        ui->leTapSelectNewXL->setText(str);
    }
    else if(w == ui->leTapSelectNewXH)
    {
        ui->leTapSelectNewXH->setText(str);
    }
    else if(w == ui->leTapSelectNewFILM)
    {
        ui->leTapSelectNewFILM->setText(str);
    }
}


void AccUi::ProcessSvcF1toF6Keys(QKeyEvent *event)
{
    if (g_is_remote_mode)
    {
        if (event->key()>=Qt::Key_F1 && event->key()<=Qt::Key_F6)
        {
            remote->hasSetdata = true;
        }
    }

    if(g_status!=PROGRAM)
    {
        return;//LJY-9-16 当处于Program状态下处理F1-F6参数设置，否则退出
    }
    if(input_lock) return;   //输入过程中不处理F1-F6
    switch (event->key())
    {
    case Qt::Key_F1:
        prv_mode = mode;
        mode = TREAT;
        //TODO select=?
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF1();
        break;
    case Qt::Key_F2:
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF2();
        break;
    case Qt::Key_F3:
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF3();
        break;
    case Qt::Key_F4:
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF4();
        break;
    case Qt::Key_F5:
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF5();
        break;
    case Qt::Key_F6:
        prv_mode = mode;
        mode = TREAT;
        UI = SVC_TREAT;
        ShowTreat();
        ProcessTreatF6();
        break;
    default:
        break;
    }
}

