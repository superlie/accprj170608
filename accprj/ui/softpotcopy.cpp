#include "accui.h"
#include "ui_accui.h"

void AccUi::SvcCopyChoice()
{

    if(svc_copy_include)
    {
        if((svc_cp_from == CPRESEARCH) && (svc_cp_to == CPDEFAULT))
        {
            CopySvcBeamTrigRtoD();
            CopySvcDoseRtoD();
        }
        else if((svc_cp_from == CPRESEARCH) && (svc_cp_to == CPTREATMENT))
        {
            CopySvcBeamTrigRtoT();
            CopySvcDoseRtoT();

        }
        else if((svc_cp_from == CPDEFAULT) && (svc_cp_to == CPTREATMENT))
        {
            CopySvcBeamTrigDtoT();
            CopySvcDoseDtoT();
        }
        else if((svc_cp_from == CPDEFAULT) && (svc_cp_to == CPRESEARCH))
        {
            CopySvcBeamTrigDtoR();
            CopySvcDoseDtoR();
        }
        else if((svc_cp_from == CPTREATMENT) && (svc_cp_to == CPRESEARCH))
        {
            CopySvcBeamTrigTtoR();
            CopySvcDoseTtoR();
        }
        else if((svc_cp_from == CPTREATMENT) && (svc_cp_to == CPDEFAULT))
        {
            CopySvcBeamTrigTtoD();
            CopySvcDoseTtoD();
        }
    }
    else
    {
        if((svc_cp_from == CPRESEARCH) && (svc_cp_to == CPDEFAULT))
        {
            CopySvcBeamTrigRtoD();
        }
        else if((svc_cp_from == CPRESEARCH) && (svc_cp_to == CPTREATMENT))
        {
            CopySvcBeamTrigRtoT();
        }
        else if((svc_cp_from == CPDEFAULT) && (svc_cp_to == CPTREATMENT))
        {
            CopySvcBeamTrigDtoT();
        }
        else if((svc_cp_from == CPDEFAULT) && (svc_cp_to == CPRESEARCH))
        {
            CopySvcBeamTrigDtoR();
        }
        else if((svc_cp_from == CPTREATMENT) && (svc_cp_to == CPRESEARCH))
        {
            CopySvcBeamTrigTtoR();
        }
        else if((svc_cp_from == CPTREATMENT) && (svc_cp_to == CPDEFAULT))
        {
            CopySvcBeamTrigTtoD();
        }
    }
    SetSoftpotVal();
    ProcessSoftpotSelectPage();
}

void AccUi::SvcCopySwitch(QKeyEvent *event)
{
    QString strtemp = QString("Copy from %1 to %2 ? ( Y/N )").arg(svc_copy[svc_cp_from]).arg(svc_copy[svc_cp_to]);
    switch(copy_mode)
    {
    case SOFTPOT_COPY_FROM:
        switch (event->key())
        {
        case Qt::Key_D:
            svc_cp_from = CPDEFAULT;
            ui->lineEditMsg->setText("Copy from DEFAULT to (R)esearch, (D)efault, (T)reament");
            copy_mode=SOFTPOT_COPY_TO;
            break;
        case Qt::Key_R:
            svc_cp_from = CPRESEARCH;
            ui->lineEditMsg->setText("Copy from RESEARCH to (R)esearch, (D)efault, (T)reament");
            copy_mode=SOFTPOT_COPY_TO;
            break;
        case Qt::Key_T:
            svc_cp_from = CPTREATMENT;
            ui->lineEditMsg->setText("Copy from TREAMENT to (R)esearch, (D)efault, (T)reament");
            copy_mode=SOFTPOT_COPY_TO;
            break;
        default:
            break;
        }
        break;
    case SOFTPOT_COPY_TO:
        switch (event->key())
        {
        case Qt::Key_D:
            svc_cp_to = CPDEFAULT;
            ui->lineEditMsg->setText("Include Dosimetry softpots (K & L pages) in the copy?(Y/N)");
            copy_mode=SOFTPOT_COPY_INCLUDE;
            break;
        case Qt::Key_R:
            svc_cp_to = CPRESEARCH;
            ui->lineEditMsg->setText("Include Dosimetry softpots (K & L pages) in the copy?(Y/N)");
            copy_mode=SOFTPOT_COPY_INCLUDE;
            break;
        case Qt::Key_T:
            //LJY-12-22
            if(!beam->CheckReschTreatToDefault(RESEARCH)
                    || !dosei->CheckReschTreatToDefault(RESEARCH)
                    || !doseii->CheckReschTreatToDefault(RESEARCH)
                    || !io->CheckReschTreatToDefault(RESEARCH))
            {
                svc_softpot_copy = false;
                ui->lineEditMsg->setText(MSG_SOFTPOT_COPY_DEV);
                return;
            }
            //---------
            svc_cp_to = CPTREATMENT;
            ui->lineEditMsg->setText("Include Dosimetry softpots (K & L pages) in the copy?(Y/N)");
            copy_mode=SOFTPOT_COPY_INCLUDE;
            break;
        default:
            break;
        }
        break;
    case SOFTPOT_COPY_INCLUDE:

        switch (event->key())
        {
        case Qt::Key_Y:
            svc_copy_include = true;
            copy_mode=SOFTPOT_COPY_SUBMIT;
            ui->lineEditMsg->setText(strtemp);
            break;
        case Qt::Key_N:
            svc_copy_include = false;
            copy_mode=SOFTPOT_COPY_SUBMIT;
            ui->lineEditMsg->setText(strtemp);
            break;
        default:
            break;
        }
        break;
    case SOFTPOT_COPY_SUBMIT:
        switch (event->key())
        {
        case Qt::Key_Y:
            SvcCopyChoice();
            ui->lineEditMsg->setText("");
            svc_softpot_copy = false;
            svc_copy_include =false;
            break;
        case Qt::Key_N:
            ui->lineEditMsg->setText("");
            svc_softpot_copy = false;
            svc_copy_include =false;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}


//Beam Trig

void AccUi::CopySvcBeamTrigDtoR()
{
//    QString str;

    //Beam1
//    str = ui->leSoftpotsBeam1AfcpDef->text();
//    ui->leSoftpotsBeam1AfcpRsch->setText(str);

//    str = ui->leSoftpotsBeam1BmiDef->text();
//    ui->leSoftpotsBeam1BmiRsch->setText(str);

//    str = ui->leSoftpotsBeam1ElmDef->text();
//    ui->leSoftpotsBeam1ElmRsch->setText(str);

//    str = ui->leSoftpotsBeam1InjeDef->text();
//    ui->leSoftpotsBeam1InjeRsch->setText(str);

//    str = ui->leSoftpotsBeam1InjiDef->text();
//    ui->leSoftpotsBeam1InjiRsch->setText(str);

//    str = ui->leSoftpotsBeam1IpfnDef->text();
//    ui->leSoftpotsBeam1IpfnRsch->setText(str);

//    str = ui->leSoftpotsBeam1LensDef->text();
//    ui->leSoftpotsBeam1LensRsch->setText(str);

//    str = ui->leSoftpotsBeam1PfnDef->text();
//    ui->leSoftpotsBeam1PfnRsch->setText(str);

//    str = ui->leSoftpotsBeam1SominDef->text();
//    ui->leSoftpotsBeam1SominRsch->setText(str);

//    str = ui->leSoftpotsBeam1StcDef->text();
//    ui->leSoftpotsBeam1StcRsch->setText(str);

    beam->afcp[RESEARCH]=beam->afcp[DEFAULT];
    beam->bmi[RESEARCH]=beam->bmi[DEFAULT];
    beam->elm[RESEARCH]=beam->elm[DEFAULT];
    beam->inje[RESEARCH]=beam->inje[DEFAULT];
    beam->inji[RESEARCH]=beam->inji[DEFAULT];
    beam->ipfn[RESEARCH]=beam->ipfn[DEFAULT];
    beam->len[RESEARCH]=beam->len[DEFAULT];
    beam->pfn[RESEARCH]=beam->pfn[DEFAULT];
    beam->somin[RESEARCH]=beam->somin[DEFAULT];
    beam->stc[RESEARCH]=beam->stc[DEFAULT];

    //Beam2
//    str = ui->leSoftpotsBeam2BalxDef->text();
//    ui->leSoftpotsBeam2BalxResrch->setText(str);

//    str = ui->leSoftpotsBeam2BalyDef->text();
//    ui->leSoftpotsBeam2BalyResrch->setText(str);

//    str = ui->leSoftpotsBeam2BmickDef->text();
//    ui->leSoftpotsBeam2BmickResrch->setText(str);

//    str = ui->leSoftpotsBeam2DsgxDef->text();
//    ui->leSoftpotsBeam2DsgxResrch->setText(str);

//    str = ui->leSoftpotsBeam2DsgyDef->text();
//    ui->leSoftpotsBeam2DsgyResrch->setText(str);

//    str = ui->leSoftpotsBeam2DswxDef->text();
//    ui->leSoftpotsBeam2DswxResrch->setText(str);

//    str = ui->leSoftpotsBeam2DswyDef->text();
//    ui->leSoftpotsBeam2DswyResrch->setText(str);

//    str = ui->leSoftpotsBeam2ElmckDef->text();
//    ui->leSoftpotsBeam2ElmckResrch->setText(str);

//    str = ui->leSoftpotsBeam2LenckDef->text();
//    ui->leSoftpotsBeam2LenckResrch->setText(str);

//    str = ui->leSoftpotsBeam2StcckDef->text();
//    ui->leSoftpotsBeam2StcckResrch->setText(str);

    dosei->dyn_steer[RESEARCH].balx=dosei->dyn_steer[DEFAULT].balx;
    doseii->dose2_gain[RESEARCH].baly=doseii->dose2_gain[DEFAULT].baly;
    beam->bmick[RESEARCH]=beam->bmick[DEFAULT];
    dosei->dyn_steer[RESEARCH].ds_gainx=dosei->dyn_steer[DEFAULT].ds_gainx;
    doseii->dose2_gain[RESEARCH].dsGainY=doseii->dose2_gain[DEFAULT].dsGainY;
    dosei->dyn_steer[RESEARCH].ds_wind=dosei->dyn_steer[DEFAULT].ds_wind;
    doseii->dyn_steer_window[RESEARCH]=doseii->dyn_steer_window[DEFAULT];
    beam->elmck[RESEARCH]=beam->elmck[DEFAULT];
    beam->lenck[RESEARCH]=beam->lenck[DEFAULT];
    beam->stcck[RESEARCH]=beam->stcck[DEFAULT];

    beam->bmick_f[RESEARCH]=beam->bmick_f[DEFAULT];
    beam->elmck_f[RESEARCH]=beam->elmck_f[DEFAULT];
    beam->lenck_f[RESEARCH]=beam->lenck_f[DEFAULT];
    beam->stcck_f[RESEARCH]=beam->stcck_f[DEFAULT];

    //Trig
//    str = ui->leSoftpotsTrigAfctDef->text();
//    ui->leSoftpotsTrigAfctResrch->setText(str);

//    str = ui->leSoftpotsTrigInjtDef->text();
//    ui->leSoftpotsTrigInjtResrch->setText(str);

//    str = ui->leSoftpotsTrigModtDef->text();
//    ui->leSoftpotsTrigModtResrch->setText(str);

//    str = ui->leSoftpotsTrigRftDef->text();
//    ui->leSoftpotsTrigRftResrch->setText(str);

//    str = ui->leSoftpotsTrigTst0Def->text();
//    ui->leSoftpotsTrigTst0Resrch->setText(str);

//    str = ui->leSoftpotsTrigTst1Def->text();
//    ui->leSoftpotsTrigTst1Resrch->setText(str);

    io->trig[RESEARCH].afcTrig=io->trig[DEFAULT].afcTrig;
    io->trig[RESEARCH].injectorTrig=io->trig[DEFAULT].injectorTrig;
    io->trig[RESEARCH].modulatorTrig=io->trig[DEFAULT].modulatorTrig;
    io->trig[RESEARCH].rfTrig=io->trig[DEFAULT].rfTrig;
    io->trig[RESEARCH].tst0=io->trig[DEFAULT].tst0;
    io->trig[RESEARCH].tst1=io->trig[DEFAULT].tst1;
}

void AccUi::CopySvcBeamTrigDtoT()
{
//    QString str;

//    //Beam1
//    str = ui->leSoftpotsBeam1AfcpDef->text();
//    ui->leSoftpotsBeam1AfcpTreat->setText(str);

//    str = ui->leSoftpotsBeam1BmiDef->text();
//    ui->leSoftpotsBeam1BmiTreat->setText(str);

//    str = ui->leSoftpotsBeam1ElmDef->text();
//    ui->leSoftpotsBeam1ElmTreat->setText(str);

//    str = ui->leSoftpotsBeam1InjeDef->text();
//    ui->leSoftpotsBeam1InjeTreat->setText(str);

//    str = ui->leSoftpotsBeam1InjiDef->text();
//    ui->leSoftpotsBeam1InjiTreat->setText(str);

//    str = ui->leSoftpotsBeam1IpfnDef->text();
//    ui->leSoftpotsBeam1IpfnTreat->setText(str);

//    str = ui->leSoftpotsBeam1LensDef->text();
//    ui->leSoftpotsBeam1LensTreat->setText(str);

//    str = ui->leSoftpotsBeam1PfnDef->text();
//    ui->leSoftpotsBeam1PfnTreat->setText(str);

//    str = ui->leSoftpotsBeam1SominDef->text();
//    ui->leSoftpotsBeam1SominTreat->setText(str);

//    str = ui->leSoftpotsBeam1StcDef->text();
//    ui->leSoftpotsBeam1StcTreat->setText(str);

//    //Beam2
//    str = ui->leSoftpotsBeam2BalxDef->text();
//    ui->leSoftpotsBeam2BalxTreat->setText(str);

//    str = ui->leSoftpotsBeam2BalyDef->text();
//    ui->leSoftpotsBeam2BalyTreat->setText(str);

//    str = ui->leSoftpotsBeam2BmickDef->text();
//    ui->leSoftpotsBeam2BmickTreat->setText(str);

//    str = ui->leSoftpotsBeam2DsgxDef->text();
//    ui->leSoftpotsBeam2DsgxTreat->setText(str);

//    str = ui->leSoftpotsBeam2DsgyDef->text();
//    ui->leSoftpotsBeam2DsgyTreat->setText(str);

//    str = ui->leSoftpotsBeam2DswxDef->text();
//    ui->leSoftpotsBeam2DswxTreat->setText(str);

//    str = ui->leSoftpotsBeam2DswyDef->text();
//    ui->leSoftpotsBeam2DswyTreat->setText(str);

//    str = ui->leSoftpotsBeam2ElmckDef->text();
//    ui->leSoftpotsBeam2ElmckTreat->setText(str);

//    str = ui->leSoftpotsBeam2LenckDef->text();
//    ui->leSoftpotsBeam2LenckTreat->setText(str);

//    str = ui->leSoftpotsBeam2StcckDef->text();
//    ui->leSoftpotsBeam2StcckTreat->setText(str);

//    //Trig
//    str = ui->leSoftpotsTrigAfctDef->text();
//    ui->leSoftpotsTrigAfctTreat->setText(str);

//    str = ui->leSoftpotsTrigInjtDef->text();
//    ui->leSoftpotsTrigInjtTreat->setText(str);

//    str = ui->leSoftpotsTrigModtDef->text();
//    ui->leSoftpotsTrigModtTreat->setText(str);

//    str = ui->leSoftpotsTrigRftDef->text();
//    ui->leSoftpotsTrigRftTreat->setText(str);

//    str = ui->leSoftpotsTrigTst0Def->text();
//    ui->leSoftpotsTrigTst0Treat->setText(str);

//    str = ui->leSoftpotsTrigTst1Def->text();
//    ui->leSoftpotsTrigTst1Treat->setText(str);

    beam->afcp[TREATMENT]=beam->afcp[DEFAULT];
    beam->bmi[TREATMENT]=beam->bmi[DEFAULT];
    beam->elm[TREATMENT]=beam->elm[DEFAULT];
    beam->inje[TREATMENT]=beam->inje[DEFAULT];
    beam->inji[TREATMENT]=beam->inji[DEFAULT];
    beam->ipfn[TREATMENT]=beam->ipfn[DEFAULT];
    beam->len[TREATMENT]=beam->len[DEFAULT];
    beam->pfn[TREATMENT]=beam->pfn[DEFAULT];
    beam->somin[TREATMENT]=beam->somin[DEFAULT];
    beam->stc[TREATMENT]=beam->stc[DEFAULT];

    dosei->dyn_steer[TREATMENT].balx=dosei->dyn_steer[DEFAULT].balx;
    doseii->dose2_gain[TREATMENT].baly=doseii->dose2_gain[DEFAULT].baly;
    beam->bmick[TREATMENT]=beam->bmick[DEFAULT];
    dosei->dyn_steer[TREATMENT].ds_gainx=dosei->dyn_steer[DEFAULT].ds_gainx;
    doseii->dose2_gain[TREATMENT].dsGainY=doseii->dose2_gain[DEFAULT].dsGainY;
    dosei->dyn_steer[TREATMENT].ds_wind=dosei->dyn_steer[DEFAULT].ds_wind;
    doseii->dyn_steer_window[TREATMENT]=doseii->dyn_steer_window[DEFAULT];
    beam->elmck[TREATMENT]=beam->elmck[DEFAULT];
    beam->lenck[TREATMENT]=beam->lenck[DEFAULT];
    beam->stcck[TREATMENT]=beam->stcck[DEFAULT];

    beam->bmick_f[TREATMENT]=beam->bmick_f[DEFAULT];
    beam->elmck_f[TREATMENT]=beam->elmck_f[DEFAULT];
    beam->lenck_f[TREATMENT]=beam->lenck_f[DEFAULT];
    beam->stcck_f[TREATMENT]=beam->stcck_f[DEFAULT];

    io->trig[TREATMENT].afcTrig=io->trig[DEFAULT].afcTrig;
    io->trig[TREATMENT].injectorTrig=io->trig[DEFAULT].injectorTrig;
    io->trig[TREATMENT].modulatorTrig=io->trig[DEFAULT].modulatorTrig;
    io->trig[TREATMENT].rfTrig=io->trig[DEFAULT].rfTrig;
    io->trig[TREATMENT].tst0=io->trig[DEFAULT].tst0;
    io->trig[TREATMENT].tst1=io->trig[DEFAULT].tst1;


}

void AccUi::CopySvcBeamTrigRtoD()
{
//    QString str;

//    //Beam1
//    str = ui->leSoftpotsBeam1AfcpRsch->text();
//    ui->leSoftpotsBeam1AfcpDef->setText(str);

//    str = ui->leSoftpotsBeam1BmiRsch->text();
//    ui->leSoftpotsBeam1BmiDef->setText(str);

//    str = ui->leSoftpotsBeam1ElmRsch->text();
//    ui->leSoftpotsBeam1ElmDef->setText(str);

//    str = ui->leSoftpotsBeam1InjeRsch->text();
//    ui->leSoftpotsBeam1InjeDef->setText(str);

//    str = ui->leSoftpotsBeam1InjiRsch->text();
//    ui->leSoftpotsBeam1InjiDef->setText(str);

//    str = ui->leSoftpotsBeam1IpfnRsch->text();
//    ui->leSoftpotsBeam1IpfnDef->setText(str);

//    str = ui->leSoftpotsBeam1LensRsch->text();
//    ui->leSoftpotsBeam1LensDef->setText(str);

//    str = ui->leSoftpotsBeam1PfnRsch->text();
//    ui->leSoftpotsBeam1PfnDef->setText(str);

//    str = ui->leSoftpotsBeam1SominRsch->text();
//    ui->leSoftpotsBeam1SominDef->setText(str);

//    str = ui->leSoftpotsBeam1StcRsch->text();
//    ui->leSoftpotsBeam1StcDef->setText(str);

//    //Beam2
//    str = ui->leSoftpotsBeam2BalxResrch->text();
//    ui->leSoftpotsBeam2BalxDef->setText(str);

//    str = ui->leSoftpotsBeam2BalyResrch->text();
//    ui->leSoftpotsBeam2BalyDef->setText(str);

//    str = ui->leSoftpotsBeam2BmickResrch->text();
//    ui->leSoftpotsBeam2BmickDef->setText(str);

//    str = ui->leSoftpotsBeam2DsgxResrch->text();
//    ui->leSoftpotsBeam2DsgxDef->setText(str);

//    str = ui->leSoftpotsBeam2DsgyResrch->text();
//    ui->leSoftpotsBeam2DsgyDef->setText(str);

//    str = ui->leSoftpotsBeam2DswxResrch->text();
//    ui->leSoftpotsBeam2DswxDef->setText(str);

//    str = ui->leSoftpotsBeam2DswyResrch->text();
//    ui->leSoftpotsBeam2DswyDef->setText(str);

//    str = ui->leSoftpotsBeam2ElmckResrch->text();
//    ui->leSoftpotsBeam2ElmckDef->setText(str);

//    str = ui->leSoftpotsBeam2LenckResrch->text();
//    ui->leSoftpotsBeam2LenckDef->setText(str);

//    str = ui->leSoftpotsBeam2StcckResrch->text();
//    ui->leSoftpotsBeam2StcckDef->setText(str);

//    //Trig
//    str = ui->leSoftpotsTrigAfctResrch->text();
//    ui->leSoftpotsTrigAfctDef->setText(str);

//    str = ui->leSoftpotsTrigInjtResrch->text();
//    ui->leSoftpotsTrigInjtDef->setText(str);

//    str = ui->leSoftpotsTrigModtResrch->text();
//    ui->leSoftpotsTrigModtDef->setText(str);

//    str = ui->leSoftpotsTrigRftResrch->text();
//    ui->leSoftpotsTrigRftDef->setText(str);

//    str = ui->leSoftpotsTrigTst0Resrch->text();
//    ui->leSoftpotsTrigTst0Def->setText(str);

//    str = ui->leSoftpotsTrigTst1Resrch->text();
//    ui->leSoftpotsTrigTst1Def->setText(str);

    beam->afcp[DEFAULT]=beam->afcp[RESEARCH];
    beam->bmi[DEFAULT]=beam->bmi[RESEARCH];
    beam->elm[DEFAULT]=beam->elm[RESEARCH];
    beam->inje[DEFAULT]=beam->inje[RESEARCH];
    beam->inji[DEFAULT]=beam->inji[RESEARCH];
    beam->ipfn[DEFAULT]=beam->ipfn[RESEARCH];
    beam->len[DEFAULT]=beam->len[RESEARCH];
    beam->pfn[DEFAULT]=beam->pfn[RESEARCH];
    beam->somin[DEFAULT]=beam->somin[RESEARCH];
    beam->stc[DEFAULT]=beam->stc[RESEARCH];

    dosei->dyn_steer[DEFAULT].balx=dosei->dyn_steer[RESEARCH].balx;
    doseii->dose2_gain[DEFAULT].baly=doseii->dose2_gain[RESEARCH].baly;
    beam->bmick[DEFAULT]=beam->bmick[RESEARCH];
    dosei->dyn_steer[DEFAULT].ds_gainx=dosei->dyn_steer[RESEARCH].ds_gainx;
    doseii->dose2_gain[DEFAULT].dsGainY=doseii->dose2_gain[RESEARCH].dsGainY;
    dosei->dyn_steer[DEFAULT].ds_wind=dosei->dyn_steer[RESEARCH].ds_wind;
    doseii->dyn_steer_window[DEFAULT]=doseii->dyn_steer_window[RESEARCH];
    beam->elmck[DEFAULT]=beam->elmck[RESEARCH];
    beam->lenck[DEFAULT]=beam->lenck[RESEARCH];
    beam->stcck[DEFAULT]=beam->stcck[RESEARCH];

    beam->bmick_f[DEFAULT]=beam->bmick_f[RESEARCH];
    beam->elmck_f[DEFAULT]=beam->elmck_f[RESEARCH];
    beam->lenck_f[DEFAULT]=beam->lenck_f[RESEARCH];
    beam->stcck_f[DEFAULT]=beam->stcck_f[RESEARCH];

    io->trig[DEFAULT].afcTrig=io->trig[RESEARCH].afcTrig;
    io->trig[DEFAULT].injectorTrig=io->trig[RESEARCH].injectorTrig;
    io->trig[DEFAULT].modulatorTrig=io->trig[RESEARCH].modulatorTrig;
    io->trig[DEFAULT].rfTrig=io->trig[RESEARCH].rfTrig;
    io->trig[DEFAULT].tst0=io->trig[RESEARCH].tst0;
    io->trig[DEFAULT].tst1=io->trig[RESEARCH].tst1;
}

void AccUi::CopySvcBeamTrigRtoT()
{
//    QString str;

//    //Beam1
//    str = ui->leSoftpotsBeam1AfcpRsch->text();
//    ui->leSoftpotsBeam1AfcpTreat->setText(str);

//    str = ui->leSoftpotsBeam1BmiRsch->text();
//    ui->leSoftpotsBeam1BmiTreat->setText(str);

//    str = ui->leSoftpotsBeam1ElmRsch->text();
//    ui->leSoftpotsBeam1ElmTreat->setText(str);

//    str = ui->leSoftpotsBeam1InjeRsch->text();
//    ui->leSoftpotsBeam1InjeTreat->setText(str);

//    str = ui->leSoftpotsBeam1InjiRsch->text();
//    ui->leSoftpotsBeam1InjiTreat->setText(str);

//    str = ui->leSoftpotsBeam1IpfnRsch->text();
//    ui->leSoftpotsBeam1IpfnTreat->setText(str);

//    str = ui->leSoftpotsBeam1LensRsch->text();
//    ui->leSoftpotsBeam1LensTreat->setText(str);

//    str = ui->leSoftpotsBeam1PfnRsch->text();
//    ui->leSoftpotsBeam1PfnTreat->setText(str);

//    str = ui->leSoftpotsBeam1SominRsch->text();
//    ui->leSoftpotsBeam1SominTreat->setText(str);

//    str = ui->leSoftpotsBeam1StcRsch->text();
//    ui->leSoftpotsBeam1StcTreat->setText(str);

//    //Beam2
//    str = ui->leSoftpotsBeam2BalxResrch->text();
//    ui->leSoftpotsBeam2BalxTreat->setText(str);

//    str = ui->leSoftpotsBeam2BalyResrch->text();
//    ui->leSoftpotsBeam2BalyTreat->setText(str);

//    str = ui->leSoftpotsBeam2BmickResrch->text();
//    ui->leSoftpotsBeam2BmickTreat->setText(str);

//    str = ui->leSoftpotsBeam2DsgxResrch->text();
//    ui->leSoftpotsBeam2DsgxTreat->setText(str);

//    str = ui->leSoftpotsBeam2DsgyResrch->text();
//    ui->leSoftpotsBeam2DsgyTreat->setText(str);

//    str = ui->leSoftpotsBeam2DswxResrch->text();
//    ui->leSoftpotsBeam2DswxTreat->setText(str);

//    str = ui->leSoftpotsBeam2DswyResrch->text();
//    ui->leSoftpotsBeam2DswyTreat->setText(str);

//    str = ui->leSoftpotsBeam2ElmckResrch->text();
//    ui->leSoftpotsBeam2ElmckTreat->setText(str);

//    str = ui->leSoftpotsBeam2LenckResrch->text();
//    ui->leSoftpotsBeam2LenckTreat->setText(str);

//    str = ui->leSoftpotsBeam2StcckResrch->text();
//    ui->leSoftpotsBeam2StcckTreat->setText(str);

//    //Trig
//    str = ui->leSoftpotsTrigAfctResrch->text();
//    ui->leSoftpotsTrigAfctTreat->setText(str);

//    str = ui->leSoftpotsTrigInjtResrch->text();
//    ui->leSoftpotsTrigInjtTreat->setText(str);

//    str = ui->leSoftpotsTrigModtResrch->text();
//    ui->leSoftpotsTrigModtTreat->setText(str);

//    str = ui->leSoftpotsTrigRftResrch->text();
//    ui->leSoftpotsTrigRftTreat->setText(str);

//    str = ui->leSoftpotsTrigTst0Resrch->text();
//    ui->leSoftpotsTrigTst0Treat->setText(str);

//    str = ui->leSoftpotsTrigTst1Resrch->text();
//    ui->leSoftpotsTrigTst1Treat->setText(str);

    beam->afcp[TREATMENT]=beam->afcp[RESEARCH];
    beam->bmi[TREATMENT]=beam->bmi[RESEARCH];
    beam->elm[TREATMENT]=beam->elm[RESEARCH];
    beam->inje[TREATMENT]=beam->inje[RESEARCH];
    beam->inji[TREATMENT]=beam->inji[RESEARCH];
    beam->ipfn[TREATMENT]=beam->ipfn[RESEARCH];
    beam->len[TREATMENT]=beam->len[RESEARCH];
    beam->pfn[TREATMENT]=beam->pfn[RESEARCH];
    beam->somin[TREATMENT]=beam->somin[RESEARCH];
    beam->stc[TREATMENT]=beam->stc[RESEARCH];

    dosei->dyn_steer[TREATMENT].balx=dosei->dyn_steer[RESEARCH].balx;
    doseii->dose2_gain[TREATMENT].baly=doseii->dose2_gain[RESEARCH].baly;
    beam->bmick[TREATMENT]=beam->bmick[RESEARCH];
    dosei->dyn_steer[TREATMENT].ds_gainx=dosei->dyn_steer[RESEARCH].ds_gainx;
    doseii->dose2_gain[TREATMENT].dsGainY=doseii->dose2_gain[RESEARCH].dsGainY;
    dosei->dyn_steer[TREATMENT].ds_wind=dosei->dyn_steer[RESEARCH].ds_wind;
    doseii->dyn_steer_window[TREATMENT]=doseii->dyn_steer_window[RESEARCH];
    beam->elmck[TREATMENT]=beam->elmck[RESEARCH];
    beam->lenck[TREATMENT]=beam->lenck[RESEARCH];
    beam->stcck[TREATMENT]=beam->stcck[RESEARCH];

    beam->bmick_f[TREATMENT]=beam->bmick_f[RESEARCH];
    beam->elmck_f[TREATMENT]=beam->elmck_f[RESEARCH];
    beam->lenck_f[TREATMENT]=beam->lenck_f[RESEARCH];
    beam->stcck_f[TREATMENT]=beam->stcck_f[RESEARCH];

    io->trig[TREATMENT].afcTrig=io->trig[RESEARCH].afcTrig;
    io->trig[TREATMENT].injectorTrig=io->trig[RESEARCH].injectorTrig;
    io->trig[TREATMENT].modulatorTrig=io->trig[RESEARCH].modulatorTrig;
    io->trig[TREATMENT].rfTrig=io->trig[RESEARCH].rfTrig;
    io->trig[TREATMENT].tst0=io->trig[RESEARCH].tst0;
    io->trig[TREATMENT].tst1=io->trig[RESEARCH].tst1;
}

void AccUi::CopySvcBeamTrigTtoD()
{
//    QString str;

//    //Beam1
//    str = ui->leSoftpotsBeam1AfcpTreat->text();
//    ui->leSoftpotsBeam1AfcpDef->setText(str);

//    str = ui->leSoftpotsBeam1BmiTreat->text();
//    ui->leSoftpotsBeam1BmiDef->setText(str);

//    str = ui->leSoftpotsBeam1ElmTreat->text();
//    ui->leSoftpotsBeam1ElmDef->setText(str);

//    str = ui->leSoftpotsBeam1InjeTreat->text();
//    ui->leSoftpotsBeam1InjeDef->setText(str);

//    str = ui->leSoftpotsBeam1InjiTreat->text();
//    ui->leSoftpotsBeam1InjiDef->setText(str);

//    str = ui->leSoftpotsBeam1IpfnTreat->text();
//    ui->leSoftpotsBeam1IpfnDef->setText(str);

//    str = ui->leSoftpotsBeam1LensTreat->text();
//    ui->leSoftpotsBeam1LensDef->setText(str);

//    str = ui->leSoftpotsBeam1PfnTreat->text();
//    ui->leSoftpotsBeam1PfnDef->setText(str);

//    str = ui->leSoftpotsBeam1SominTreat->text();
//    ui->leSoftpotsBeam1SominDef->setText(str);

//    str = ui->leSoftpotsBeam1StcTreat->text();
//    ui->leSoftpotsBeam1StcDef->setText(str);

//    //Beam2
//    str = ui->leSoftpotsBeam2BalxTreat->text();
//    ui->leSoftpotsBeam2BalxDef->setText(str);

//    str = ui->leSoftpotsBeam2BalyTreat->text();
//    ui->leSoftpotsBeam2BalyDef->setText(str);

//    str = ui->leSoftpotsBeam2BmickTreat->text();
//    ui->leSoftpotsBeam2BmickDef->setText(str);

//    str = ui->leSoftpotsBeam2DsgxTreat->text();
//    ui->leSoftpotsBeam2DsgxDef->setText(str);

//    str = ui->leSoftpotsBeam2DsgyTreat->text();
//    ui->leSoftpotsBeam2DsgyDef->setText(str);

//    str = ui->leSoftpotsBeam2DswxTreat->text();
//    ui->leSoftpotsBeam2DswxDef->setText(str);

//    str = ui->leSoftpotsBeam2DswyTreat->text();
//    ui->leSoftpotsBeam2DswyDef->setText(str);

//    str = ui->leSoftpotsBeam2ElmckTreat->text();
//    ui->leSoftpotsBeam2ElmckDef->setText(str);

//    str = ui->leSoftpotsBeam2LenckTreat->text();
//    ui->leSoftpotsBeam2LenckDef->setText(str);

//    str = ui->leSoftpotsBeam2StcckTreat->text();
//    ui->leSoftpotsBeam2StcckDef->setText(str);

//    //Trig
//    str = ui->leSoftpotsTrigAfctTreat->text();
//    ui->leSoftpotsTrigAfctDef->setText(str);

//    str = ui->leSoftpotsTrigInjtTreat->text();
//    ui->leSoftpotsTrigInjtDef->setText(str);

//    str = ui->leSoftpotsTrigModtTreat->text();
//    ui->leSoftpotsTrigModtDef->setText(str);

//    str = ui->leSoftpotsTrigRftTreat->text();
//    ui->leSoftpotsTrigRftDef->setText(str);

//    str = ui->leSoftpotsTrigTst0Treat->text();
//    ui->leSoftpotsTrigTst0Def->setText(str);

//    str = ui->leSoftpotsTrigTst1Treat->text();
//    ui->leSoftpotsTrigTst1Def->setText(str);

    beam->afcp[DEFAULT]=beam->afcp[TREATMENT];
    beam->bmi[DEFAULT]=beam->bmi[TREATMENT];
    beam->elm[DEFAULT]=beam->elm[TREATMENT];
    beam->inje[DEFAULT]=beam->inje[TREATMENT];
    beam->inji[DEFAULT]=beam->inji[TREATMENT];
    beam->ipfn[DEFAULT]=beam->ipfn[TREATMENT];
    beam->len[DEFAULT]=beam->len[TREATMENT];
    beam->pfn[DEFAULT]=beam->pfn[TREATMENT];
    beam->somin[DEFAULT]=beam->somin[TREATMENT];
    beam->stc[DEFAULT]=beam->stc[TREATMENT];

    dosei->dyn_steer[DEFAULT].balx=dosei->dyn_steer[TREATMENT].balx;
    doseii->dose2_gain[DEFAULT].baly=doseii->dose2_gain[TREATMENT].baly;
    beam->bmick[DEFAULT]=beam->bmick[TREATMENT];
    dosei->dyn_steer[DEFAULT].ds_gainx=dosei->dyn_steer[TREATMENT].ds_gainx;
    doseii->dose2_gain[DEFAULT].dsGainY=doseii->dose2_gain[TREATMENT].dsGainY;
    dosei->dyn_steer[DEFAULT].ds_wind=dosei->dyn_steer[TREATMENT].ds_wind;
    doseii->dyn_steer_window[DEFAULT]=doseii->dyn_steer_window[TREATMENT];
    beam->elmck[DEFAULT]=beam->elmck[TREATMENT];
    beam->lenck[DEFAULT]=beam->lenck[TREATMENT];
    beam->stcck[DEFAULT]=beam->stcck[TREATMENT];

    beam->bmick_f[DEFAULT]=beam->bmick_f[TREATMENT];
    beam->elmck_f[DEFAULT]=beam->elmck_f[TREATMENT];
    beam->lenck_f[DEFAULT]=beam->lenck_f[TREATMENT];
    beam->stcck_f[DEFAULT]=beam->stcck_f[TREATMENT];

    io->trig[DEFAULT].afcTrig=io->trig[TREATMENT].afcTrig;
    io->trig[DEFAULT].injectorTrig=io->trig[TREATMENT].injectorTrig;
    io->trig[DEFAULT].modulatorTrig=io->trig[TREATMENT].modulatorTrig;
    io->trig[DEFAULT].rfTrig=io->trig[TREATMENT].rfTrig;
    io->trig[DEFAULT].tst0=io->trig[TREATMENT].tst0;
    io->trig[DEFAULT].tst1=io->trig[TREATMENT].tst1;
}

void AccUi::CopySvcBeamTrigTtoR()
{
//    QString str;

//    //Beam1
//    str = ui->leSoftpotsBeam1AfcpTreat->text();
//    ui->leSoftpotsBeam1AfcpRsch->setText(str);

//    str = ui->leSoftpotsBeam1BmiTreat->text();
//    ui->leSoftpotsBeam1BmiRsch->setText(str);

//    str = ui->leSoftpotsBeam1ElmTreat->text();
//    ui->leSoftpotsBeam1ElmRsch->setText(str);

//    str = ui->leSoftpotsBeam1InjeTreat->text();
//    ui->leSoftpotsBeam1InjeRsch->setText(str);

//    str = ui->leSoftpotsBeam1InjiTreat->text();
//    ui->leSoftpotsBeam1InjiRsch->setText(str);

//    str = ui->leSoftpotsBeam1IpfnTreat->text();
//    ui->leSoftpotsBeam1IpfnRsch->setText(str);

//    str = ui->leSoftpotsBeam1LensTreat->text();
//    ui->leSoftpotsBeam1LensRsch->setText(str);

//    str = ui->leSoftpotsBeam1PfnTreat->text();
//    ui->leSoftpotsBeam1PfnRsch->setText(str);

//    str = ui->leSoftpotsBeam1SominTreat->text();
//    ui->leSoftpotsBeam1SominRsch->setText(str);

//    str = ui->leSoftpotsBeam1StcTreat->text();
//    ui->leSoftpotsBeam1StcRsch->setText(str);

//    //Beam2
//    str = ui->leSoftpotsBeam2BalxTreat->text();
//    ui->leSoftpotsBeam2BalxResrch->setText(str);

//    str = ui->leSoftpotsBeam2BalyTreat->text();
//    ui->leSoftpotsBeam2BalyResrch->setText(str);

//    str = ui->leSoftpotsBeam2BmickTreat->text();
//    ui->leSoftpotsBeam2BmickResrch->setText(str);

//    str = ui->leSoftpotsBeam2DsgxTreat->text();
//    ui->leSoftpotsBeam2DsgxResrch->setText(str);

//    str = ui->leSoftpotsBeam2DsgyTreat->text();
//    ui->leSoftpotsBeam2DsgyResrch->setText(str);

//    str = ui->leSoftpotsBeam2DswxTreat->text();
//    ui->leSoftpotsBeam2DswxResrch->setText(str);

//    str = ui->leSoftpotsBeam2DswyTreat->text();
//    ui->leSoftpotsBeam2DswyResrch->setText(str);

//    str = ui->leSoftpotsBeam2ElmckTreat->text();
//    ui->leSoftpotsBeam2ElmckResrch->setText(str);

//    str = ui->leSoftpotsBeam2LenckTreat->text();
//    ui->leSoftpotsBeam2LenckResrch->setText(str);

//    str = ui->leSoftpotsBeam2StcckTreat->text();
//    ui->leSoftpotsBeam2StcckResrch->setText(str);

//    //Trig
//    str = ui->leSoftpotsTrigAfctTreat->text();
//    ui->leSoftpotsTrigAfctResrch->setText(str);

//    str = ui->leSoftpotsTrigInjtTreat->text();
//    ui->leSoftpotsTrigInjtResrch->setText(str);

//    str = ui->leSoftpotsTrigModtTreat->text();
//    ui->leSoftpotsTrigModtResrch->setText(str);

//    str = ui->leSoftpotsTrigRftTreat->text();
//    ui->leSoftpotsTrigRftResrch->setText(str);

//    str = ui->leSoftpotsTrigTst0Treat->text();
//    ui->leSoftpotsTrigTst0Resrch->setText(str);

//    str = ui->leSoftpotsTrigTst1Treat->text();
//    ui->leSoftpotsTrigTst1Resrch->setText(str);

    beam->afcp[RESEARCH]=beam->afcp[TREATMENT];
    beam->bmi[RESEARCH]=beam->bmi[TREATMENT];
    beam->elm[RESEARCH]=beam->elm[TREATMENT];
    beam->inje[RESEARCH]=beam->inje[TREATMENT];
    beam->inji[RESEARCH]=beam->inji[TREATMENT];
    beam->ipfn[RESEARCH]=beam->ipfn[TREATMENT];
    beam->len[RESEARCH]=beam->len[TREATMENT];
    beam->pfn[RESEARCH]=beam->pfn[TREATMENT];
    beam->somin[RESEARCH]=beam->somin[TREATMENT];
    beam->stc[RESEARCH]=beam->stc[TREATMENT];

    dosei->dyn_steer[RESEARCH].balx=dosei->dyn_steer[TREATMENT].balx;
    doseii->dose2_gain[RESEARCH].baly=doseii->dose2_gain[TREATMENT].baly;
    beam->bmick[RESEARCH]=beam->bmick[TREATMENT];
    dosei->dyn_steer[RESEARCH].ds_gainx=dosei->dyn_steer[TREATMENT].ds_gainx;
    doseii->dose2_gain[RESEARCH].dsGainY=doseii->dose2_gain[TREATMENT].dsGainY;
    dosei->dyn_steer[RESEARCH].ds_wind=dosei->dyn_steer[TREATMENT].ds_wind;
    doseii->dyn_steer_window[RESEARCH]=doseii->dyn_steer_window[TREATMENT];
    beam->elmck[RESEARCH]=beam->elmck[TREATMENT];
    beam->lenck[RESEARCH]=beam->lenck[TREATMENT];
    beam->stcck[RESEARCH]=beam->stcck[TREATMENT];

    beam->bmick_f[RESEARCH]=beam->bmick_f[TREATMENT];
    beam->elmck_f[RESEARCH]=beam->elmck_f[TREATMENT];
    beam->lenck_f[RESEARCH]=beam->lenck_f[TREATMENT];
    beam->stcck_f[RESEARCH]=beam->stcck_f[TREATMENT];

    io->trig[RESEARCH].afcTrig=io->trig[TREATMENT].afcTrig;
    io->trig[RESEARCH].injectorTrig=io->trig[TREATMENT].injectorTrig;
    io->trig[RESEARCH].modulatorTrig=io->trig[TREATMENT].modulatorTrig;
    io->trig[RESEARCH].rfTrig=io->trig[TREATMENT].rfTrig;
    io->trig[RESEARCH].tst0=io->trig[TREATMENT].tst0;
    io->trig[RESEARCH].tst1=io->trig[TREATMENT].tst1;
}

//Dose

void AccUi::CopySvcDoseDtoR()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gDef->text();
//    ui->leSoftpotsDose1D1gResrch->setText(str);

//    str = ui->leSoftpotsDose1D2gDef->text();
//    ui->leSoftpotsDose1D2gResrch->setText(str);

//    str = ui->leSoftpotsDose1DrcvDef->text();
//    ui->leSoftpotsDose1DrcvResrch->setText(str);

//    str = ui->leSoftpotsDose1PrfpDef->text();
//    ui->leSoftpotsDose1PrfpResrch->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostDef->text();
//    ui->leSoftpotsDose2DostResrch->setText(str);

//    str = ui->leSoftpotsDose2FwiDef->text();
//    ui->leSoftpotsDose2FwiResrch->setText(str);

//    str = ui->leSoftpotsDose2S0gDef->text();
//    ui->leSoftpotsDose2S0gResrch->setText(str);

//    str = ui->leSoftpotsDose2S1gDef->text();
//    ui->leSoftpotsDose2S1gResrch->setText(str);

//    str = ui->leSoftpotsDose2S2gDef->text();
//    ui->leSoftpotsDose2S2gResrch->setText(str);

//    str = ui->leSoftpotsDose2S3gDef->text();
//    ui->leSoftpotsDose2S3gResrch->setText(str);

//    str = ui->leSoftpotsDose2S4gDef->text();
//    ui->leSoftpotsDose2S4gResrch->setText(str);

//    str = ui->leSoftpotsDose2SwiDef->text();
//    ui->leSoftpotsDose2SwiResrch->setText(str);

    dosei->dose1_gain[RESEARCH].dose1_gain=dosei->dose1_gain[DEFAULT].dose1_gain;
    doseii->dose2_gain[RESEARCH].gain=doseii->dose2_gain[DEFAULT].gain;
    dosei->dose1_gain[RESEARCH].drcv=dosei->dose1_gain[DEFAULT].drcv;
    doseii->dose2_gain[RESEARCH].drcv=doseii->dose2_gain[DEFAULT].drcv;
    beam->prfp[RESEARCH]=beam->prfp[DEFAULT];

    io->trig[RESEARCH].doseTrig=io->trig[DEFAULT].doseTrig;
    dosei->swi[RESEARCH]=dosei->swi[DEFAULT];
    dosei->fwi[RESEARCH]=dosei->fwi[DEFAULT];
    doseii->dose2_segment_gain[RESEARCH].segment0Gain=doseii->dose2_segment_gain[DEFAULT].segment0Gain;
    dosei->dose1_gain[RESEARCH].segment1_gain=dosei->dose1_gain[DEFAULT].segment1_gain;
    doseii->dose2_segment_gain[RESEARCH].segment2Gain=doseii->dose2_segment_gain[DEFAULT].segment2Gain;
    dosei->dose1_gain[RESEARCH].segment3_gain=dosei->dose1_gain[DEFAULT].segment3_gain;
    doseii->dose2_segment_gain[RESEARCH].segment4Gain=doseii->dose2_segment_gain[DEFAULT].segment4Gain;
}

void AccUi::CopySvcDoseDtoT()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gDef->text();
//    ui->leSoftpotsDose1D1gTreat->setText(str);

//    str = ui->leSoftpotsDose1D2gDef->text();
//    ui->leSoftpotsDose1D2gTreat->setText(str);

//    str = ui->leSoftpotsDose1DrcvDef->text();
//    ui->leSoftpotsDose1DrcvTreat->setText(str);

//    str = ui->leSoftpotsDose1PrfpDef->text();
//    ui->leSoftpotsDose1PrfpTreat->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostDef->text();
//    ui->leSoftpotsDose2DostTreat->setText(str);

//    str = ui->leSoftpotsDose2FwiDef->text();
//    ui->leSoftpotsDose2FwiTreat->setText(str);

//    str = ui->leSoftpotsDose2S0gDef->text();
//    ui->leSoftpotsDose2S0gTreat->setText(str);

//    str = ui->leSoftpotsDose2S1gDef->text();
//    ui->leSoftpotsDose2S1gTreat->setText(str);

//    str = ui->leSoftpotsDose2S2gDef->text();
//    ui->leSoftpotsDose2S2gTreat->setText(str);

//    str = ui->leSoftpotsDose2S3gDef->text();
//    ui->leSoftpotsDose2S3gTreat->setText(str);

//    str = ui->leSoftpotsDose2S4gDef->text();
//    ui->leSoftpotsDose2S4gTreat->setText(str);

//    str = ui->leSoftpotsDose2SwiDef->text();
//    ui->leSoftpotsDose2SwiTreat->setText(str);

    dosei->dose1_gain[TREATMENT].dose1_gain=dosei->dose1_gain[DEFAULT].dose1_gain;
    doseii->dose2_gain[TREATMENT].gain=doseii->dose2_gain[DEFAULT].gain;
    dosei->dose1_gain[TREATMENT].drcv=dosei->dose1_gain[DEFAULT].drcv;
    doseii->dose2_gain[TREATMENT].drcv=doseii->dose2_gain[DEFAULT].drcv;
    beam->prfp[TREATMENT]=beam->prfp[DEFAULT];

    io->trig[TREATMENT].doseTrig=io->trig[DEFAULT].doseTrig;
    dosei->swi[TREATMENT]=dosei->swi[DEFAULT];
    dosei->fwi[TREATMENT]=dosei->fwi[DEFAULT];
    doseii->dose2_segment_gain[TREATMENT].segment0Gain=doseii->dose2_segment_gain[DEFAULT].segment0Gain;
    dosei->dose1_gain[TREATMENT].segment1_gain=dosei->dose1_gain[DEFAULT].segment1_gain;
    doseii->dose2_segment_gain[TREATMENT].segment2Gain=doseii->dose2_segment_gain[DEFAULT].segment2Gain;
    dosei->dose1_gain[TREATMENT].segment3_gain=dosei->dose1_gain[DEFAULT].segment3_gain;
    doseii->dose2_segment_gain[TREATMENT].segment4Gain=doseii->dose2_segment_gain[DEFAULT].segment4Gain;

}

void AccUi::CopySvcDoseRtoD()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gResrch->text();
//    ui->leSoftpotsDose1D1gDef->setText(str);

//    str = ui->leSoftpotsDose1D2gResrch->text();
//    ui->leSoftpotsDose1D2gDef->setText(str);

//    str = ui->leSoftpotsDose1DrcvResrch->text();
//    ui->leSoftpotsDose1DrcvDef->setText(str);

//    str = ui->leSoftpotsDose1PrfpResrch->text();
//    ui->leSoftpotsDose1PrfpDef->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostResrch->text();
//    ui->leSoftpotsDose2DostDef->setText(str);

//    str = ui->leSoftpotsDose2FwiResrch->text();
//    ui->leSoftpotsDose2FwiDef->setText(str);

//    str = ui->leSoftpotsDose2S0gResrch->text();
//    ui->leSoftpotsDose2S0gDef->setText(str);

//    str = ui->leSoftpotsDose2S1gResrch->text();
//    ui->leSoftpotsDose2S1gDef->setText(str);

//    str = ui->leSoftpotsDose2S2gResrch->text();
//    ui->leSoftpotsDose2S2gDef->setText(str);

//    str = ui->leSoftpotsDose2S3gResrch->text();
//    ui->leSoftpotsDose2S3gDef->setText(str);

//    str = ui->leSoftpotsDose2S4gResrch->text();
//    ui->leSoftpotsDose2S4gDef->setText(str);

//    str = ui->leSoftpotsDose2SwiResrch->text();
//    ui->leSoftpotsDose2SwiDef->setText(str);

    dosei->dose1_gain[DEFAULT].dose1_gain=dosei->dose1_gain[RESEARCH].dose1_gain;
    doseii->dose2_gain[DEFAULT].gain=doseii->dose2_gain[RESEARCH].gain;
    dosei->dose1_gain[DEFAULT].drcv=dosei->dose1_gain[RESEARCH].drcv;
    doseii->dose2_gain[DEFAULT].drcv=doseii->dose2_gain[RESEARCH].drcv;
    beam->prfp[DEFAULT]=beam->prfp[RESEARCH];

    io->trig[DEFAULT].doseTrig=io->trig[RESEARCH].doseTrig;
    dosei->swi[DEFAULT]=dosei->swi[RESEARCH];
    dosei->fwi[DEFAULT]=dosei->fwi[RESEARCH];
    doseii->dose2_segment_gain[DEFAULT].segment0Gain=doseii->dose2_segment_gain[RESEARCH].segment0Gain;
    dosei->dose1_gain[DEFAULT].segment1_gain=dosei->dose1_gain[RESEARCH].segment1_gain;
    doseii->dose2_segment_gain[DEFAULT].segment2Gain=doseii->dose2_segment_gain[RESEARCH].segment2Gain;
    dosei->dose1_gain[DEFAULT].segment3_gain=dosei->dose1_gain[RESEARCH].segment3_gain;
    doseii->dose2_segment_gain[DEFAULT].segment4Gain=doseii->dose2_segment_gain[RESEARCH].segment4Gain;



}

void AccUi::CopySvcDoseRtoT()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gResrch->text();
//    ui->leSoftpotsDose1D1gTreat->setText(str);

//    str = ui->leSoftpotsDose1D2gResrch->text();
//    ui->leSoftpotsDose1D2gTreat->setText(str);

//    str = ui->leSoftpotsDose1DrcvResrch->text();
//    ui->leSoftpotsDose1DrcvTreat->setText(str);

//    str = ui->leSoftpotsDose1PrfpResrch->text();
//    ui->leSoftpotsDose1PrfpTreat->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostResrch->text();
//    ui->leSoftpotsDose2DostTreat->setText(str);

//    str = ui->leSoftpotsDose2FwiResrch->text();
//    ui->leSoftpotsDose2FwiTreat->setText(str);

//    str = ui->leSoftpotsDose2S0gResrch->text();
//    ui->leSoftpotsDose2S0gTreat->setText(str);

//    str = ui->leSoftpotsDose2S1gResrch->text();
//    ui->leSoftpotsDose2S1gTreat->setText(str);

//    str = ui->leSoftpotsDose2S2gResrch->text();
//    ui->leSoftpotsDose2S2gTreat->setText(str);

//    str = ui->leSoftpotsDose2S3gResrch->text();
//    ui->leSoftpotsDose2S3gTreat->setText(str);

//    str = ui->leSoftpotsDose2S4gResrch->text();
//    ui->leSoftpotsDose2S4gTreat->setText(str);

//    str = ui->leSoftpotsDose2SwiResrch->text();
//    ui->leSoftpotsDose2SwiTreat->setText(str);

    dosei->dose1_gain[TREATMENT].dose1_gain=dosei->dose1_gain[RESEARCH].dose1_gain;
    doseii->dose2_gain[TREATMENT].gain=doseii->dose2_gain[RESEARCH].gain;
    dosei->dose1_gain[TREATMENT].drcv=dosei->dose1_gain[RESEARCH].drcv;
    doseii->dose2_gain[TREATMENT].drcv=doseii->dose2_gain[RESEARCH].drcv;
    beam->prfp[TREATMENT]=beam->prfp[RESEARCH];

    io->trig[TREATMENT].doseTrig=io->trig[RESEARCH].doseTrig;
    dosei->swi[TREATMENT]=dosei->swi[RESEARCH];
    dosei->fwi[TREATMENT]=dosei->fwi[RESEARCH];
    doseii->dose2_segment_gain[TREATMENT].segment0Gain=doseii->dose2_segment_gain[RESEARCH].segment0Gain;
    dosei->dose1_gain[TREATMENT].segment1_gain=dosei->dose1_gain[RESEARCH].segment1_gain;
    doseii->dose2_segment_gain[TREATMENT].segment2Gain=doseii->dose2_segment_gain[RESEARCH].segment2Gain;
    dosei->dose1_gain[TREATMENT].segment3_gain=dosei->dose1_gain[RESEARCH].segment3_gain;
    doseii->dose2_segment_gain[TREATMENT].segment4Gain=doseii->dose2_segment_gain[RESEARCH].segment4Gain;

}

void AccUi::CopySvcDoseTtoD()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gTreat->text();
//    ui->leSoftpotsDose1D1gDef->setText(str);

//    str = ui->leSoftpotsDose1D2gTreat->text();
//    ui->leSoftpotsDose1D2gDef->setText(str);

//    str = ui->leSoftpotsDose1DrcvTreat->text();
//    ui->leSoftpotsDose1DrcvDef->setText(str);

//    str = ui->leSoftpotsDose1PrfpTreat->text();
//    ui->leSoftpotsDose1PrfpDef->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostTreat->text();
//    ui->leSoftpotsDose2DostDef->setText(str);

//    str = ui->leSoftpotsDose2FwiTreat->text();
//    ui->leSoftpotsDose2FwiDef->setText(str);

//    str = ui->leSoftpotsDose2S0gTreat->text();
//    ui->leSoftpotsDose2S0gDef->setText(str);

//    str = ui->leSoftpotsDose2S1gTreat->text();
//    ui->leSoftpotsDose2S1gDef->setText(str);

//    str = ui->leSoftpotsDose2S2gTreat->text();
//    ui->leSoftpotsDose2S2gDef->setText(str);

//    str = ui->leSoftpotsDose2S3gTreat->text();
//    ui->leSoftpotsDose2S3gDef->setText(str);

//    str = ui->leSoftpotsDose2S4gTreat->text();
//    ui->leSoftpotsDose2S4gDef->setText(str);

//    str = ui->leSoftpotsDose2SwiTreat->text();
//    ui->leSoftpotsDose2SwiDef->setText(str);

    dosei->dose1_gain[DEFAULT].dose1_gain=dosei->dose1_gain[TREATMENT].dose1_gain;
    doseii->dose2_gain[DEFAULT].gain=doseii->dose2_gain[TREATMENT].gain;
    dosei->dose1_gain[DEFAULT].drcv=dosei->dose1_gain[TREATMENT].drcv;
    doseii->dose2_gain[DEFAULT].drcv=doseii->dose2_gain[TREATMENT].drcv;
    beam->prfp[DEFAULT]=beam->prfp[TREATMENT];

    io->trig[DEFAULT].doseTrig=io->trig[TREATMENT].doseTrig;
    dosei->swi[DEFAULT]=dosei->swi[TREATMENT];
    dosei->fwi[DEFAULT]=dosei->fwi[TREATMENT];
    doseii->dose2_segment_gain[DEFAULT].segment0Gain=doseii->dose2_segment_gain[TREATMENT].segment0Gain;
    dosei->dose1_gain[DEFAULT].segment1_gain=dosei->dose1_gain[TREATMENT].segment1_gain;
    doseii->dose2_segment_gain[DEFAULT].segment2Gain=doseii->dose2_segment_gain[TREATMENT].segment2Gain;
    dosei->dose1_gain[DEFAULT].segment3_gain=dosei->dose1_gain[TREATMENT].segment3_gain;
    doseii->dose2_segment_gain[DEFAULT].segment4Gain=doseii->dose2_segment_gain[TREATMENT].segment4Gain;

}

void AccUi::CopySvcDoseTtoR()
{
//    QString str;

//    //Dose1
//    str = ui->leSoftpotsDose1D1gTreat->text();
//    ui->leSoftpotsDose1D1gResrch->setText(str);

//    str = ui->leSoftpotsDose1D2gTreat->text();
//    ui->leSoftpotsDose1D2gResrch->setText(str);

//    str = ui->leSoftpotsDose1DrcvTreat->text();
//    ui->leSoftpotsDose1DrcvResrch->setText(str);

//    str = ui->leSoftpotsDose1PrfpTreat->text();
//    ui->leSoftpotsDose1PrfpResrch->setText(str);

//    //Dose2
//    str = ui->leSoftpotsDose2DostTreat->text();
//    ui->leSoftpotsDose2DostResrch->setText(str);

//    str = ui->leSoftpotsDose2FwiTreat->text();
//    ui->leSoftpotsDose2FwiResrch->setText(str);

//    str = ui->leSoftpotsDose2S0gTreat->text();
//    ui->leSoftpotsDose2S0gResrch->setText(str);

//    str = ui->leSoftpotsDose2S1gTreat->text();
//    ui->leSoftpotsDose2S1gResrch->setText(str);

//    str = ui->leSoftpotsDose2S2gTreat->text();
//    ui->leSoftpotsDose2S2gResrch->setText(str);

//    str = ui->leSoftpotsDose2S3gTreat->text();
//    ui->leSoftpotsDose2S3gResrch->setText(str);

//    str = ui->leSoftpotsDose2S4gTreat->text();
//    ui->leSoftpotsDose2S4gResrch->setText(str);

//    str = ui->leSoftpotsDose2SwiTreat->text();
//    ui->leSoftpotsDose2SwiResrch->setText(str);

    dosei->dose1_gain[RESEARCH].dose1_gain=dosei->dose1_gain[TREATMENT].dose1_gain;
    doseii->dose2_gain[RESEARCH].gain=doseii->dose2_gain[TREATMENT].gain;
    dosei->dose1_gain[RESEARCH].drcv=dosei->dose1_gain[TREATMENT].drcv;
    doseii->dose2_gain[RESEARCH].drcv=doseii->dose2_gain[TREATMENT].drcv;
    beam->prfp[RESEARCH]=beam->prfp[TREATMENT];

    io->trig[RESEARCH].doseTrig=io->trig[TREATMENT].doseTrig;
    dosei->swi[RESEARCH]=dosei->swi[TREATMENT];
    dosei->fwi[RESEARCH]=dosei->fwi[TREATMENT];
    doseii->dose2_segment_gain[RESEARCH].segment0Gain=doseii->dose2_segment_gain[TREATMENT].segment0Gain;
    dosei->dose1_gain[RESEARCH].segment1_gain=dosei->dose1_gain[TREATMENT].segment1_gain;
    doseii->dose2_segment_gain[RESEARCH].segment2Gain=doseii->dose2_segment_gain[TREATMENT].segment2Gain;
    dosei->dose1_gain[RESEARCH].segment3_gain=dosei->dose1_gain[TREATMENT].segment3_gain;
    doseii->dose2_segment_gain[RESEARCH].segment4Gain=doseii->dose2_segment_gain[TREATMENT].segment4Gain;

}
