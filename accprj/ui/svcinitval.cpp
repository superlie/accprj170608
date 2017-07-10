#include "accui.h"
#include "ui_accui.h"

void AccUi::SetSoftpotVal()
{
    QString disp;
    UINT16 afcp;
    afcp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->afcp[RESEARCH], AFCP_RSCH_CK);
    disp.sprintf("%d", afcp);
    ui->leSoftpotsBeam1AfcpRsch->setText(disp);

    afcp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->afcp[DEFAULT], AFCP_RSCH_CK);
    disp.sprintf("%d", afcp);
    ui->leSoftpotsBeam1AfcpDef->setText(disp);

    afcp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->afcp[TREATMENT], AFCP_RSCH_CK);
    disp.sprintf("%d", afcp);
    ui->leSoftpotsBeam1AfcpTreat->setText(disp);

    UINT16 inje;
    inje = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inje[RESEARCH], INJE_RSCH_CK);
    disp.sprintf("%d", inje);
    ui->leSoftpotsBeam1InjeRsch->setText(disp);

    inje = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inje[DEFAULT], INJE_RSCH_CK);
    disp.sprintf("%d", inje);
    ui->leSoftpotsBeam1InjeDef->setText(disp);

    inje = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inje[TREATMENT], INJE_RSCH_CK);
    disp.sprintf("%d", inje);
    ui->leSoftpotsBeam1InjeTreat->setText(disp);

    UINT16 inji;
    inji = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inji[RESEARCH], INJI_RSCH_CK);
    disp.sprintf("%d", inji);
    ui->leSoftpotsBeam1InjiRsch->setText(disp);

    inji = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inji[DEFAULT], INJI_RSCH_CK);
    disp.sprintf("%d", inji);
    ui->leSoftpotsBeam1InjiDef->setText(disp);

    inji = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inji[TREATMENT], INJI_RSCH_CK);
    disp.sprintf("%d", inji);
    ui->leSoftpotsBeam1InjiTreat->setText(disp);

    UINT16 elm;
    elm = (UINT16)SlaveBase::ConvertMemberToFloat(beam->elm[RESEARCH], ELM_RSCH_CK);
    disp.sprintf("%d", elm);
    ui->leSoftpotsBeam1ElmRsch->setText(disp);

    elm = (UINT16)SlaveBase::ConvertMemberToFloat(beam->elm[DEFAULT], ELM_RSCH_CK);
    disp.sprintf("%d", elm);
    ui->leSoftpotsBeam1ElmDef->setText(disp);

    elm = (UINT16)SlaveBase::ConvertMemberToFloat(beam->elm[TREATMENT], ELM_RSCH_CK);
    disp.sprintf("%d", elm);
    ui->leSoftpotsBeam1ElmTreat->setText(disp);

    UINT16 bmi;
    bmi = (UINT16)SlaveBase::ConvertMemberToFloat(beam->bmi[RESEARCH], BMI_RSCH_CK);
    disp.sprintf("%d", bmi);
    ui->leSoftpotsBeam1BmiRsch->setText(disp);

    bmi = (UINT16)SlaveBase::ConvertMemberToFloat(beam->bmi[DEFAULT], BMI_RSCH_CK);
    disp.sprintf("%d", bmi);
    ui->leSoftpotsBeam1BmiDef->setText(disp);

    bmi = (UINT16)SlaveBase::ConvertMemberToFloat(beam->bmi[TREATMENT], BMI_RSCH_CK);
    disp.sprintf("%d", bmi);
    ui->leSoftpotsBeam1BmiTreat->setText(disp);

    UINT16 stc;
    stc = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stc[RESEARCH], STC_RSCH_CK);
    disp.sprintf("%d", stc);
    ui->leSoftpotsBeam1StcRsch->setText(disp);

    stc = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stc[DEFAULT], STC_RSCH_CK);
    disp.sprintf("%d", stc);
    ui->leSoftpotsBeam1StcDef->setText(disp);

    stc = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stc[TREATMENT], STC_RSCH_CK);
    disp.sprintf("%d", stc);
    ui->leSoftpotsBeam1StcTreat->setText(disp);

    UINT16 lens;
    lens = (UINT16)SlaveBase::ConvertMemberToFloat(beam->len[RESEARCH], LENS_RSCH_CK);
    disp.sprintf("%d", lens);
    ui->leSoftpotsBeam1LensRsch->setText(disp);

    lens = (UINT16)SlaveBase::ConvertMemberToFloat(beam->len[DEFAULT], LENS_RSCH_CK);
    disp.sprintf("%d", lens);
    ui->leSoftpotsBeam1LensDef->setText(disp);

    lens = (UINT16)SlaveBase::ConvertMemberToFloat(beam->len[TREATMENT], LENS_RSCH_CK);
    disp.sprintf("%d", lens);
    ui->leSoftpotsBeam1LensTreat->setText(disp);

    UINT16 pfn;
    pfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->pfn[RESEARCH], PFN_RSCH_CK);
    disp.sprintf("%d", pfn);
    ui->leSoftpotsBeam1PfnRsch->setText(disp);

    pfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->pfn[DEFAULT], PFN_RSCH_CK);
    disp.sprintf("%d", pfn);
    ui->leSoftpotsBeam1PfnDef->setText(disp);

    pfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->pfn[TREATMENT], PFN_RSCH_CK);
    disp.sprintf("%d", pfn);
    ui->leSoftpotsBeam1PfnTreat->setText(disp);

    UINT16 ipfn;
    ipfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->ipfn[RESEARCH], IPFN_RSCH_CK);
    disp.sprintf("%d", ipfn);
    ui->leSoftpotsBeam1IpfnRsch->setText(disp);

    ipfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->ipfn[DEFAULT], IPFN_RSCH_CK);
    disp.sprintf("%d", ipfn);
    ui->leSoftpotsBeam1IpfnDef->setText(disp);

    ipfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->ipfn[TREATMENT], IPFN_RSCH_CK);
    disp.sprintf("%d", ipfn);
    ui->leSoftpotsBeam1IpfnTreat->setText(disp);

    UINT16 somin;
    somin = (UINT16)SlaveBase::ConvertMemberToFloat(beam->somin[RESEARCH], SOMIN_RSCH_CK);
    disp.sprintf("%d", somin);
    ui->leSoftpotsBeam1SominRsch->setText(disp);

    somin = (UINT16)SlaveBase::ConvertMemberToFloat(beam->somin[DEFAULT], SOMIN_RSCH_CK);
    disp.sprintf("%d", somin);
    ui->leSoftpotsBeam1SominDef->setText(disp);

    somin = (UINT16)SlaveBase::ConvertMemberToFloat(beam->somin[TREATMENT], SOMIN_RSCH_CK);
    disp.sprintf("%d", somin);
    ui->leSoftpotsBeam1SominTreat->setText(disp);

    UINT16 dsgx;
    dsgx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[RESEARCH].ds_gainx, DSGX_RSCH_CK);
    disp.sprintf("%d", dsgx);
    ui->leSoftpotsBeam2DsgxResrch->setText(disp);

    dsgx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[DEFAULT].ds_gainx, DSGX_RSCH_CK);
    disp.sprintf("%d", dsgx);
    ui->leSoftpotsBeam2DsgxDef->setText(disp);

    dsgx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[TREATMENT].ds_gainx, DSGX_RSCH_CK);
    disp.sprintf("%d", dsgx);
    ui->leSoftpotsBeam2DsgxTreat->setText(disp);

    UINT16 dsgy;
    dsgy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[RESEARCH].dsGainY, DSGY_RSCH_CK);
    disp.sprintf("%d", dsgy);
    ui->leSoftpotsBeam2DsgyResrch->setText(disp);

    dsgy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[DEFAULT].dsGainY, DSGY_RSCH_CK);
    disp.sprintf("%d", dsgy);
    ui->leSoftpotsBeam2DsgyDef->setText(disp);

    dsgy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[TREATMENT].dsGainY, DSGY_RSCH_CK);
    disp.sprintf("%d", dsgy);
    ui->leSoftpotsBeam2DsgyTreat->setText(disp);

    UINT16 dswx;
    dswx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[RESEARCH].ds_wind, DSWX_RSCH_CK);
    disp.sprintf("%d", dswx);
    ui->leSoftpotsBeam2DswxResrch->setText(disp);

    dswx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[DEFAULT].ds_wind, DSWX_RSCH_CK);
    disp.sprintf("%d", dswx);
    ui->leSoftpotsBeam2DswxDef->setText(disp);

    dswx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[TREATMENT].ds_wind, DSWX_RSCH_CK);
    disp.sprintf("%d", dswx);
    ui->leSoftpotsBeam2DswxTreat->setText(disp);

    //UINT16 dswy;
    UINT16 dswy;
    dswy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dyn_steer_window[RESEARCH], DSWY_RSCH_CK);
    disp.sprintf("%d", dswy);
    ui->leSoftpotsBeam2DswyResrch->setText(disp);

    dswy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dyn_steer_window[DEFAULT], DSWY_RSCH_CK);
    disp.sprintf("%d", dswy);
    ui->leSoftpotsBeam2DswyDef->setText(disp);

    dswy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dyn_steer_window[TREATMENT], DSWY_RSCH_CK);
    disp.sprintf("%d", dswy);
    ui->leSoftpotsBeam2DswyTreat->setText(disp);

    UINT16 balx;
    balx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[RESEARCH].balx, BALX_RSCH_CK);
    disp.sprintf("%d", balx);
    ui->leSoftpotsBeam2BalxResrch->setText(disp);

    balx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[DEFAULT].balx, BALX_RSCH_CK);
    disp.sprintf("%d", balx);
    ui->leSoftpotsBeam2BalxDef->setText(disp);

    balx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[TREATMENT].balx, BALX_RSCH_CK);
    disp.sprintf("%d", balx);
    ui->leSoftpotsBeam2BalxTreat->setText(disp);

    UINT16 baly;
    baly = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[RESEARCH].baly, BALY_RSCH_CK);
    disp.sprintf("%d", baly);
    ui->leSoftpotsBeam2BalyResrch->setText(disp);

    baly = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[DEFAULT].baly, BALY_RSCH_CK);
    disp.sprintf("%d", baly);
    ui->leSoftpotsBeam2BalyDef->setText(disp);

    baly = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[TREATMENT].baly, BALY_RSCH_CK);
    disp.sprintf("%d", baly);
    ui->leSoftpotsBeam2BalyTreat->setText(disp);

    float elmck;
    elmck = SlaveBase::ConvertMemberToFloat(beam->elmck[RESEARCH], ELMCK_RSCH_CK);
    disp.sprintf("%0.1f", elmck);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    beam->elmck_f[RESEARCH] = elmck;

    elmck = SlaveBase::ConvertMemberToFloat(beam->elmck[DEFAULT], ELMCK_RSCH_CK);
    disp.sprintf("%0.1f", elmck);
    ui->leSoftpotsBeam2ElmckDef->setText(disp);
    beam->elmck_f[DEFAULT] = elmck;

    elmck = SlaveBase::ConvertMemberToFloat(beam->elmck[TREATMENT], ELMCK_RSCH_CK);
    disp.sprintf("%0.1f", elmck);
    ui->leSoftpotsBeam2ElmckTreat->setText(disp);
    beam->elmck_f[TREATMENT] = elmck;

    float bmick;
    bmick = SlaveBase::ConvertMemberToFloat(beam->bmick[RESEARCH], BMICK_RSCH_CK);
    disp.sprintf("%0.1f", bmick);
    ui->leSoftpotsBeam2BmickResrch->setText(disp);
    beam->bmick_f[RESEARCH] = bmick;

    bmick = SlaveBase::ConvertMemberToFloat(beam->bmick[DEFAULT], BMICK_RSCH_CK);
    disp.sprintf("%0.1f", bmick);
    ui->leSoftpotsBeam2BmickDef->setText(disp);
    beam->bmick_f[DEFAULT] = bmick;

    bmick = SlaveBase::ConvertMemberToFloat(beam->bmick[TREATMENT], BMICK_RSCH_CK);
    disp.sprintf("%0.1f", bmick);
    ui->leSoftpotsBeam2BmickTreat->setText(disp);
    beam->bmick_f[TREATMENT] = bmick;

    UINT16 stcck;
    stcck = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stcck[RESEARCH], STCCK_RSCH_CK);
    disp.sprintf("%d", stcck);
    ui->leSoftpotsBeam2StcckResrch->setText(disp);
    beam->stcck_f[RESEARCH] = stcck;

    stcck = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stcck[DEFAULT], STCCK_RSCH_CK);
    disp.sprintf("%d", stcck);
    ui->leSoftpotsBeam2StcckDef->setText(disp);
    beam->stcck_f[DEFAULT] = stcck;

    stcck = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stcck[TREATMENT], STCCK_RSCH_CK);
    disp.sprintf("%d", stcck);
    ui->leSoftpotsBeam2StcckTreat->setText(disp);
    beam->stcck_f[TREATMENT] = stcck;

    float lenck;
    lenck = SlaveBase::ConvertMemberToFloat(beam->lenck[RESEARCH], LENCK_RSCH_CK);
    disp.sprintf("%0.2f", lenck);
    ui->leSoftpotsBeam2LenckResrch->setText(disp);
    beam->lenck_f[RESEARCH] = lenck;

    lenck = SlaveBase::ConvertMemberToFloat(beam->lenck[DEFAULT], LENCK_RSCH_CK);
    disp.sprintf("%0.2f", lenck);
    ui->leSoftpotsBeam2LenckDef->setText(disp);
    beam->lenck_f[DEFAULT] = lenck;

    lenck = SlaveBase::ConvertMemberToFloat(beam->lenck[TREATMENT], LENCK_RSCH_CK);
    disp.sprintf("%0.2f", lenck);
    ui->leSoftpotsBeam2LenckTreat->setText(disp);
    beam->lenck_f[TREATMENT] = lenck;

    float modt;
    modt = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].modulatorTrig, MODT_RSCH_CK);
    disp.sprintf("%0.1f", modt);
    ui->leSoftpotsTrigModtResrch->setText(disp);

    modt = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].modulatorTrig, MODT_RSCH_CK);
    disp.sprintf("%0.1f", modt);
    ui->leSoftpotsTrigModtDef->setText(disp);

    modt = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].modulatorTrig, MODT_RSCH_CK);
    disp.sprintf("%0.1f", modt);
    ui->leSoftpotsTrigModtTreat->setText(disp);

    float injt;
    injt = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].injectorTrig, INJT_RSCH_CK);
    disp.sprintf("%0.1f", injt);
    ui->leSoftpotsTrigInjtResrch->setText(disp);

    injt = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].injectorTrig, INJT_RSCH_CK);
    disp.sprintf("%0.1f", injt);
    ui->leSoftpotsTrigInjtDef->setText(disp);

    injt = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].injectorTrig, INJT_RSCH_CK);
    disp.sprintf("%0.1f", injt);
    ui->leSoftpotsTrigInjtTreat->setText(disp);

    float afct;
    afct = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].afcTrig, AFCT_RSCH_CK);
    disp.sprintf("%0.1f", afct);
    ui->leSoftpotsTrigAfctResrch->setText(disp);

    afct = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].afcTrig, AFCT_RSCH_CK);
    disp.sprintf("%0.1f", afct);
    ui->leSoftpotsTrigAfctDef->setText(disp);

    afct = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].afcTrig, AFCT_RSCH_CK);
    disp.sprintf("%0.1f", afct);
    ui->leSoftpotsTrigAfctTreat->setText(disp);

    float rft;
    rft = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].rfTrig, RFT_RSCH_CK);
    disp.sprintf("%0.1f", rft);
    ui->leSoftpotsTrigRftResrch->setText(disp);

    rft = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].rfTrig, RFT_RSCH_CK);
    disp.sprintf("%0.1f", rft);
    ui->leSoftpotsTrigRftDef->setText(disp);

    rft = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].rfTrig, RFT_RSCH_CK);
    disp.sprintf("%0.1f", rft);
    ui->leSoftpotsTrigRftTreat->setText(disp);

    float tst0;
    tst0 = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].tst0, TST0_RSCH_CK);
    disp.sprintf("%0.1f", tst0);
    ui->leSoftpotsTrigTst0Resrch->setText(disp);

    tst0 = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].tst0, TST0_RSCH_CK);
    disp.sprintf("%0.1f", tst0);
    ui->leSoftpotsTrigTst0Def->setText(disp);

    tst0 = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].tst0, TST0_RSCH_CK);
    disp.sprintf("%0.1f", tst0);
    ui->leSoftpotsTrigTst0Treat->setText(disp);

    float tst1;
    tst1 = SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].tst1, TST1_RSCH_CK);
    disp.sprintf("%0.1f", tst1);
    ui->leSoftpotsTrigTst1Resrch->setText(disp);

    tst1 = SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].tst1, TST1_RSCH_CK);
    disp.sprintf("%0.1f", tst1);
    ui->leSoftpotsTrigTst1Def->setText(disp);

    tst1 = SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].tst1, TST1_RSCH_CK);
    disp.sprintf("%0.1f", tst1);
    ui->leSoftpotsTrigTst1Treat->setText(disp);

    UINT16 d1g;
    d1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[RESEARCH].dose1_gain, DOSE1_GAIN_CK);
    disp.sprintf("%d", d1g);
    ui->leSoftpotsDose1D1gResrch->setText(disp);

    d1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[DEFAULT].dose1_gain, DOSE1_GAIN_CK);
    disp.sprintf("%d", d1g);
    ui->leSoftpotsDose1D1gDef->setText(disp);

    d1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[TREATMENT].dose1_gain, DOSE1_GAIN_CK);
    disp.sprintf("%d", d1g);
    //qDebug()<<"dose1 treat gain ="<<dosei->dose1_gain[TREATMENT].dose1_gain<<", value ="<<d1g;
    ui->leSoftpotsDose1D1gTreat->setText(disp);

    UINT16 d2g;
    d2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[RESEARCH].gain, DOSE2_GAIN_CK);
    disp.sprintf("%d", d2g);
    ui->leSoftpotsDose1D2gResrch->setText(disp);

    d2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[DEFAULT].gain, DOSE2_GAIN_CK);
    disp.sprintf("%d", d2g);
    ui->leSoftpotsDose1D2gDef->setText(disp);

    d2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[TREATMENT].gain, DOSE2_GAIN_CK);
    disp.sprintf("%d", d2g);
    ui->leSoftpotsDose1D2gTreat->setText(disp);

    UINT16 prfp;
    prfp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->prfp[RESEARCH], PRFP_CK);
    disp.sprintf("%d", prfp);
    ui->leSoftpotsDose1PrfpResrch->setText(disp);

    prfp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->prfp[DEFAULT], PRFP_CK);
    disp.sprintf("%d", prfp);
    ui->leSoftpotsDose1PrfpDef->setText(disp);

    prfp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->prfp[TREATMENT], PRFP_CK);
    disp.sprintf("%d", prfp);
    ui->leSoftpotsDose1PrfpTreat->setText(disp);

    UINT16 drcv;
    drcv = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[RESEARCH].drcv, DRCV_CK);
    disp.sprintf("%d", drcv);
    ui->leSoftpotsDose1DrcvResrch->setText(disp);

    drcv = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[DEFAULT].drcv, DRCV_CK);
    disp.sprintf("%d", drcv);
    ui->leSoftpotsDose1DrcvDef->setText(disp);

    drcv = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[TREATMENT].drcv, DRCV_CK);
    disp.sprintf("%d", drcv);
    ui->leSoftpotsDose1DrcvTreat->setText(disp);

    UINT16 s0g;
    s0g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[RESEARCH].segment0Gain, S0_GAIN_CK);
    disp.sprintf("%d", s0g);
    ui->leSoftpotsDose2S0gResrch->setText(disp);

    s0g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[DEFAULT].segment0Gain, S0_GAIN_CK);
    disp.sprintf("%d", s0g);
    ui->leSoftpotsDose2S0gDef->setText(disp);

    s0g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[TREATMENT].segment0Gain, S0_GAIN_CK);
    disp.sprintf("%d", s0g);
    ui->leSoftpotsDose2S0gTreat->setText(disp);

    UINT16 s1g;
    s1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[RESEARCH].segment1_gain, S1_GAIN_CK);
    disp.sprintf("%d", s1g);
    ui->leSoftpotsDose2S1gResrch->setText(disp);

    s1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[DEFAULT].segment1_gain, S1_GAIN_CK);
    disp.sprintf("%d", s1g);
    ui->leSoftpotsDose2S1gDef->setText(disp);

    s1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[TREATMENT].segment1_gain, S1_GAIN_CK);
    disp.sprintf("%d", s1g);
    ui->leSoftpotsDose2S1gTreat->setText(disp);

    UINT16 s2g;
    s2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[RESEARCH].segment2Gain, S2_GAIN_CK);
    disp.sprintf("%d", s2g);
    ui->leSoftpotsDose2S2gResrch->setText(disp);

    s2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[DEFAULT].segment2Gain, S2_GAIN_CK);
    disp.sprintf("%d", s2g);
    ui->leSoftpotsDose2S2gDef->setText(disp);

    s2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[TREATMENT].segment2Gain, S2_GAIN_CK);
    disp.sprintf("%d", s2g);
    ui->leSoftpotsDose2S2gTreat->setText(disp);

    UINT16 s3g;
    s3g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[RESEARCH].segment3_gain, S3_GAIN_CK);
    disp.sprintf("%d", s3g);
    ui->leSoftpotsDose2S3gResrch->setText(disp);

    s3g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[DEFAULT].segment3_gain, S3_GAIN_CK);
    disp.sprintf("%d", s3g);
    ui->leSoftpotsDose2S3gDef->setText(disp);

    s3g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[TREATMENT].segment3_gain, S3_GAIN_CK);
    disp.sprintf("%d", s3g);
    ui->leSoftpotsDose2S3gTreat->setText(disp);

    UINT16 s4g;
    s4g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[RESEARCH].segment4Gain, S4_GAIN_CK);
    disp.sprintf("%d", s4g);
    ui->leSoftpotsDose2S4gResrch->setText(disp);

    s4g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[DEFAULT].segment4Gain, S4_GAIN_CK);
    disp.sprintf("%d", s4g);
    ui->leSoftpotsDose2S4gDef->setText(disp);

    s4g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[TREATMENT].segment4Gain, S4_GAIN_CK);
    disp.sprintf("%d", s4g);
    ui->leSoftpotsDose2S4gTreat->setText(disp);

    disp.sprintf("%0.1f", dosei->swi[RESEARCH]);
    ui->leSoftpotsDose2SwiResrch->setText(disp);

    disp.sprintf("%0.1f", dosei->swi[DEFAULT]);
    ui->leSoftpotsDose2SwiDef->setText(disp);

    disp.sprintf("%0.1f", dosei->swi[TREATMENT]);
    ui->leSoftpotsDose2SwiTreat->setText(disp);

    disp.sprintf("%0.1f", dosei->fwi[RESEARCH]);
    ui->leSoftpotsDose2FwiResrch->setText(disp);

    disp.sprintf("%0.1f", dosei->fwi[DEFAULT]);
    ui->leSoftpotsDose2FwiDef->setText(disp);

    disp.sprintf("%0.1f", dosei->fwi[TREATMENT]);
    ui->leSoftpotsDose2FwiTreat->setText(disp);

    UINT16 dost;
    dost = (UINT16)SlaveBase::ConvertMemberToFloat(io->trig[RESEARCH].doseTrig, DOS_T_CK);
    disp.sprintf("%d", dost);
    ui->leSoftpotsDose2DostResrch->setText(disp);

    dost = (UINT16)SlaveBase::ConvertMemberToFloat(io->trig[DEFAULT].doseTrig, DOS_T_CK);
    disp.sprintf("%d", dost);
    ui->leSoftpotsDose2DostDef->setText(disp);

    dost = (UINT16)SlaveBase::ConvertMemberToFloat(io->trig[TREATMENT].doseTrig, DOS_T_CK);
    disp.sprintf("%d", dost);
    ui->leSoftpotsDose2DostTreat->setText(disp);
}





void AccUi::SetInterlockErr()
{
#ifdef CHINESE
    interlock_list << "0. 授权认证失败"
                   << "1. 2级剂量监视器"
                   << "2. 治疗时间"
                   << "3. 剂量监视器同步（软件）"
                   << "4. 温度"
                   << "5. 压力"
                   << "6. 温度/压力补偿因子"
                   << "7. 均整度"
                   << "8. 对称性"
                   << "9. 辐射野尺寸"
                   << "10. 旋转机架位置"
                   << "11. DYNAM STEERING"
                   << "12. 剂量率1（软件）"
                   << "13. 剂量率2（软件）"
                   << "14. 预热计时器"
                   << "15. 模式（软件）"
                   << "16. 磁透镜"
                   << "17. 校正"
                   << "18. 电磁铁电流（ELMI）"
                   << "19. 附件码错误"
                   << "20. 十字线板"
                   << "21. 高率 1"
                   << "22. 高率 2"
                   << "23. 电源检查"
                   << "24. 模式（硬件）"
                   << "25. 真空度"
                   << "26. 附件未充分插入"
                   << "27. 高压过流"
                   << "28. 旋转机架转速（硬件）"
                   << "29. 注入器"
                   << "30. 调制器未准备好"
                   << "31. 控制台尚未完成设置"
                   << "32. RDS INTERFACE"
                   << "33. GANTRY OVERSHOOT"
                   << "34. BEAM SHIELD (SW)"
                   << "35. 偏转磁场电流（BMIS）超范围"
                   << "36. 电离室高压超范围"
                   << "37. 旋转机架限位"
                   << "38. BEAM SHIELD (HW)"
                   << "39. 运动停止"
                   << "40. 内冷却水系统水位过低"
                   << "41. 内冷却水系统水温过高"
                   << "42. 冷却水流量"
                   << "43. WATER CONDUCTIVITY"
                   << "44. 固定机架散热风量过低"
                   << "45. 治疗室防护门"
                   << "46. 高压释放器/风量"
                   << "47. 偏转磁场电流"
                   << "48. 治疗床未锁定"
                   << "49. 准直器位置"
                   << "50. TABLE POSITION"
                   << "51. 钨门位置"
                   << "52. 附件预置码"
                   << "53. 剂量监视器同步"
                   << "54. 剂量率2（硬件）"
                   << "55. 联锁电路"
                   << "56. 剂量监视器1"
                   << "57. 剂量监视系统1的零点漂移漂移"
                   << "58. 饱和"
                   << "59. 压力/温度饱和"
                   << "60. 剂量监视系统2的零点漂移"
                   << "61. 剂量监视系统2饱和"
                   << "62. 控制台SIP错误"
                   << "63. 功能控制器0错误"
                   << "64. 功能控制器1错误"
                   << "65. 功能控制器2错误"
                   << "66. 功能控制器3错误"
                   << "67. 功能控制器4错误"
                   << "68. 功能控制器5错误"
                   << "69. 功能控制器6错误"
                   << "70. 功能控制器7错误"
                   << "71. 功能控制器0通讯中断"
                   << "72. 功能控制器1通讯中断"
                   << "73. 功能控制器2通讯中断"
                   << "74. 功能控制器3通讯中断"
                   << "75. 功能控制器4通讯中断"
                   << "76. 功能控制器5通讯中断"
                   << "77. 功能控制器6通讯中断"
                   << "78. 功能控制器7通讯中断"
                   << "79. 通讯"
                   << "80. 偏转磁场电源超时"
                   << "81. MLC 未响应"
                   << "82. 每度剂量"
                   << "83. 预热未完成"
                   << "84. 时钟电路故障"
                   << "85. I/O（输入/输出）电路故障"
                   << "86. 电磁铁（硬件）"
                   << "87. 钨门Y1运动"
                   << "88. 钨门Y2运动"
                   << "89. 钨门X2运动"
                   << "90. 钨门X1运动"
                   << "91. 限束系统运动"
                   << "92. 旋转机架运动"
                   << "93. 备份电容"
                   << "94. 预置高率"
                   << "95. VERIFICATION"
                   << "96. V&R INTERFACE"
                   << "97. 剂量1或剂量2计算"
                   << "98. 剂量模拟"
                   << "99. VW BEAM INTENSITY 1"
                   << "100. VM BEAM INTENSITY 2"
                   << "101. VM JAW Y1 SPEED"
                   << "102. VM JAW Y2 SPEED"
                   << "103. VM JAW X2 SPEED"
                   << "104. VM JAW X1 SPEED"
                   << "105. MLC FIELD POSITION"
                   << "106. MLC COVERAGE"
                   << "107. MLC INTERFACE"
                   << "108. MLC ERROR"
                   << "109. MLC 通讯中断"
                   << "110. FIELD LIGHT"
                   << "111. 上位机 通讯中断"
                   << "112. PCB 4, BIT 2"
                   << "113. PCB 4, BIT 3"
                   << "114. PCB 4, BIT 4"
                   << "115. PCB 4, BIT 5"
                   << "116. PCB 4, BIT 6"
                   << "117. PCB 4, BIT 7"
                   << "118. PCB 4, BIT 8"
                   << "119. PCB 4, BIT 9"
                   << "120. PCB 4, BIT 10"
                   << "121. PCB 4, BIT 11"
                   << "122. PCB 4, BIT 12"
                   << "123. PCB 4, BIT 13"
                   << "124. 等待MLC到位"
                   << "125. 治疗数据错误"
                   << "126. 上位机 未响应"
                   << "ABEND (SERVICE USE ONLY)";
#else
    interlock_list << "0.AUTHORIZATION FAIL"
                   << "1.MONTIOR2"
                   << "2.TREATMENT TIME"
                   << "3.MONITOR SYNC (SW)"
                   << "4.TEMPERATURE"
                   << "5.PRESSURE"
                   << "6.PRESSURE/TEMP C.F."
                   << "7.FLATNESS"
                   << "8.SYMMETRY"
                   << "9.FIELD SIZE"
                   << "10.GANTRY POSITION"
                   << "11.DYNAM STEERING"
                   << "12.DOSE RATE 1 (SW)"
                   << "13.DOSE RATE 2 (SW)"
                   << "14.WARUP TIMER"
                   << "15.MODE (SW)"
                   << "16.LENS"
                   << "17.STEERING"
                   << "18.ELEC. MAGNET (SW)"
                   << "19.ACCESSORY READ FAULT"
                   << "20.RETICLE IN ELECTRON"
                   << "21.HI RATE 1"
                   << "22.HI RATE 2"
                   << "23.POWER SUPPLY CHECK"
                   << "24.MODE (HW)"
                   << "25.VACUUM OVER CURRENT"
                   << "26.ACCESSORY INSERTION"
                   << "27.HIGH VOLTAGE O.C."
                   << "28.GANTRY SPEED"
                   << "29.INJECTOR"
                   << "30.MODULATOR INCOMPLETE"
                   << "31.PROGRAM READY"
                   << "32.RDS INTERFACE"
                   << "33.GANTRY OVERSHOOT"
                   << "34.BEAM SHIELD (SW)"
                   << "35.BENDING MAGNET (SW)"
                   << "36.CHAMBER HIGH VOLTAGE"
                   << "37.GANTRY LIMIT SWITCH"
                   << "38.BEAM SHIELD (HW)"
                   << "39.MOTION STOP"
                   << "40.WATER LEVEL"
                   << "41.WATER OVER TEMP"
                   << "42.WATER FLOW"
                   << "43.WATER CONDUCTIVITY"
                   << "44.AIR FLOW"
                   << "45.TREATMENT ROOM DOOR"
                   << "46.HOOK STICK"
                   << "47.BENDING MAGNET (HW)"
                   << "48.TABLE NOT LOCKED"
                   << "49.COLLIMATOR POSITION"
                   << "50.TABLE POSITION"
                   << "51.JAW FIELD POSITION"
                   << "52.ACCESSORY PRESET"
                   << "53.MOTION SYNC (HW)"
                   << "54.DOSE RATE 2 (HW)"
                   << "55.INTERLOCK CIRCUITS"
                   << "56.MONITOR 1"
                   << "57.DOSE 1 OFFSET"
                   << "58.DOSE 1 SATURATION"
                   << "59.RRES/TEMP SATURATION"
                   << "60.DOSE 2 OFFSET"
                   << "61.DOSE 2 SATURATION"
                   << "62.CONSOLE"
                   << "63.CONTROLLER 0"
                   << "64.CONTROLLER 1"
                   << "65.CONTROLLER 2"
                   << "66.CONTROLLER 3"
                   << "67.CONTROLLER 4"
                   << "68.CONTROLLER 5"
                   << "69.CONTROLLER 6"
                   << "70.CONTROLLER 7"
                   << "71.LOGOFF 0"
                   << "72.LOGOFF 1"
                   << "73.LOGOFF 2"
                   << "74.LOGOFF 3"
                   << "75.LOGOFF 4"
                   << "76.LOGOFF 5"
                   << "77.LOGOFF 6"
                   << "78.LOGOFF 7"
                   << "79.COMMUNICATION"
                   << "80.BMI TIMEOUT"
                   << "81.MLC NOT RESPONSE"
                   << "82.DOSE PER DEGREE"
                   << "83.WARM UP"
                   << "84.CLOCK"
                   << "85.IO RELAY BOARD"
                   << "86.ELEC.MAGENT (HW)"
                   << "87.JAW Y1 MOTION"
                   << "88.JAW Y2 MOTION"
                   << "89.JAW X2 MOTION"
                   << "90.JAW X1 MOTION"
                   << "91.COLLI MOTION"
                   << "92.GANTRY MOTION"
                   << "93.BACKUP CAPS"
                   << "94.PRE HI RATE"
                   << "95.VERIFICATION"
                   << "96.V&R INTERFACE"
                   << "97.DOSE 1 OR 2 CALC"
                   << "98.DOSE SIMULATION"
                   << "99.VW BEAM INTENSITY 1"
                   << "100.VM BEAM INTENSITY 2"
                   << "101.VM JAW Y1 SPEED"
                   << "102.VM JAW Y2 SPEED"
                   << "103.VM JAW X2 SPEED"
                   << "104.VM JAW X1 SPEED"
                   << "105.MLC FIELD POSITION"
                   << "106.MLC COVERAGE"
                   << "107.MLC INTERFACE"
                   << "108.MLC ERROR"
                   << "109.MLC LOGOFF"
                   << "110.FIELD LIGHT"
                   << "111.UPPER LOGOFF"
                   << "112.PCB 4, BIT 2"
                   << "113.PCB 4, BIT 3"
                   << "114.PCB 4, BIT 4"
                   << "115. PCB 4, BIT 5"
                   << "116.PCB 4, BIT 6"
                   << "117.PCB 4, BIT 7"
                   << "118.PCB 4, BIT 8"
                   << "119.PCB 4, BIT 9"
                   << "120.PCB 4, BIT 10"
                   << "121.PCB 4, BIT 11"
                   << "122.PCB 4, BIT 12"
                   << "123.PCB 4, BIT 13"
                   << "124.WAIT MLC Ready"
                   << "125.Treatdata DATA ERROR"
                   << "126.UPPER NOT RESPONSE"
                   << "ABEND (SERVICE USE ONLY)";
#endif


    ilckinfolist<<"(spare)"//0
               <<"Coincidence with preset value."
              <<"Coincidence with preset value."
             <<"Dose rate 2 differs from dose rate 1 by more than +- 15%."
            <<"TEMP1 and TEMP2 differ by more than +- 1.5degree C."
           <<"PRES1 and PRES2 differ by more than +- 7 mm Hg."//5
          <<"Compensation factor outside the interval (0.86,1.14)."
         <<"Flatness outside flatness window; 2 * spec."
        <<"Symmetry outside symmetry window, 2 * spec."
       <<"Field size must match preset accessories."
      <<"Compare gantry with preset data, +- 2 degrees."//10
     <<"Dynamic steering current outside window."
    <<"Compare with DRCV data, +- 20%."
    <<"Compare with DRCV data, +- 20%."
    <<"Warmup timer malfunction detected."
    <<"Foil data must match mode and energy."//15
    <<"Compare LENI with preset window, +- 5%."
    <<"Compare STCI with preset window, +- 5%."
    <<"Compare ELMI with proset window, +- 5%."
    <<"Accessories must match mode selection."
    <<"No reticle allowed in electron mode."//20
    <<"Excessive dose 1 per pulse."
    <<"Excessive dose 2 per pulse."
    <<"A dose analog power supply has drifted by more than 4%."
    <<"Compare driving data with indicators."
    <<"Current > 15uA."//25
    <<"Accessory not fully inserted."
    <<"High voltage over current."
    <<"Gantry speed too slow or too fast (+- 10% of preset)."
    <<"Filament has not warmed up yet."
    <<"Some interlocks of modelator or klystron are not OK."//30
    <<"Console not fully programmed yet."
    <<"Error in the RDS interface module."
    <<"Gantry passed stop angle during treatment with beam on."
    <<"Colli angle and field size allow overshoot of beam shield."
    <<"Compare BMIS with preset window ,← 5%."//35
    <<"HV < 550V or HV > 630V."
    <<"Gantry has hit a limit switch."
    <<"Beam shield required for prohibited sectors."
    <<"Inhibit all machine motion."
    <<"Water level is too low."//40
    <<"Water temperature is too high."
    <<"Water flow is too slow."
    <<"Water not pure enough."
    <<"Not enough air flow"
    <<"Treatment door must be closed for radiation."//45
    <<"Hook stick not in holder or possible M class air flow program."
    <<"BMIS too low."
    <<"Table not locked."
    <<"Collimator angle must match preset."
    <<"Table setup must match preset."//50
    <<"Jaw field size must match preset."
    <<"Accessories must match preset."
    <<"DOSE rate 2 differs from dose rate 1 by more than ←15% of DRCV or 300mv."
    <<"Dose rate 2 differs from DRCV by more than +-20%."
    <<"Failure detected in interlock circuits."//55
    <<"(reserved)"
    <<"Excessive dosimetry 1 offset detected (more than 256 counts read)."
    <<"110	Dose rate 1 > 4095 counts (saturated) (00)."
    <<"Reported pressure or temperature is not within expected range."
    <<"Excessive dosimetry 2 offset detected (more than 256 counts read)."//60
    <<"More than3584 counts read by dose 2 controller."
    <<"Console error."
    <<"Controller 0 error."
    <<"Controller 1 error."
    <<"Controller 2 error."//65
    <<"Controller 3 error."
    <<"Controller 4 error."
    <<"Controller 5 error."
    <<"Controller 6 error."
    <<"Controller 7 error."//70
    <<"Controller 0 not communicating."
    <<"Controller 1 not communicating."
    <<"Controller 2 not communicating."
    <<"Controller 3 not communicating."
    <<"(RESERVED)"//75
    <<"Controller 5 not communicating."
    <<"Controller 6 not communicating."
    <<"Controller 7 not communicating."
    <<"SIP communication error."
    <<"Timeout after applying bending magnet current."//80
    <<"(spare)"
    <<"Dose per degree out of tolerance (+-20%) during rotational therapy."
    <<"Hardware filament timer interlock."
    <<"Checks I/O controller 1MHz clock. "
    <<"I/O relay board broken."//85
    <<"Electro-magnet current too low."
    <<"JawY1 motion error detected by controller 0."
    <<"JawY2 motion error detected by controller 0."
    <<"JawX2 motion error detected by controller 0."
    <<"JawX1 motion error detected by controller 0."//90
    <<"Unexpected collimator motion detected by controller 0."
    <<"Unexpected gantry motion detected by controller 0."
    <<"Malfunction in parameter backup system."
    <<"Electron foil in place and injection > 200 mA."
    <<"Waiting for V & R to authorize or deny treatment."//95
    <<"Error in the V & R system interface module."
    <<"Dose calculation error"
    <<"Dose simulation circuit active in error."
    <<"Beam intensity statistics for monitor 1 exceed threshold."
    <<"Beam intensity statistics for monitor 2 exceed threshold."//100
    <<"Jaw Y1 speed statistics exceed threshold."
    <<"Jaw Y2 speed statistics exceed threshold."
    <<"Jaw X2 speed statistics exceed threshold."
    <<"Jaw X1 speed statistics exceed threshold."
    <<"MLC field size must match preset."//105
    <<"Y jaws do not cover MLC leaves closest to edges of X field."
    <<"MLC Interface error."
    <<"MLC Controller error."
    <<"MLC Controller not communicating."
    <<"Field light activation / deactivation failure."//110
    <<"(SPARE)"
    <<"Active HW interlock: PCB 4; Bit 2..or.. MLC Controller Failure."
    <<"Active HW interlock: PCB 4; Biit 3."
    <<"Active HW interlock: PCB 4; Biit 4."
    <<"Active HW interlock: PCB 4; Biit 5."//115
    <<"Active HW interlock: PCB 4; Biit 6."
    <<"Active HW interlock: PCB 4; Biit 7."
    <<"Active HW interlock: PCB 4; Biit 8."
    <<"Active HW interlock: PCB 4; Biit 9."
    <<"Active HW interlock: PCB 4; Biit 10."//120
    <<"Active HW interlock: PCB 4; Biit 11."
    <<"Active HW interlock: PCB 4; Biit 12..or.. Unexpected MLC leaf motion."
    <<"Active HW interlock: PCB 4; Biit 13."
    <<"(SPARE)"
    <<"(SPARE)"//125
    <<"(SPARE)"
    <<"ABEND INFO – SERVICE USE ONLY";
    //----------------------------------------------------------------------------------
    errlist[0]<<"";
    errlist[1]<<"213	Dose 2 actual >= dose 2 preset or HVON occurred before RAD ON.";
    errlist[2]<<"107	Treatment time actual >= treatment time preset."
             <<"207	Treatment time actual >= treatment time preset.";
    errlist[3]<<"";
    errlist[4]<<"";
    errlist[5]<<"";
    errlist[6]<<"";
    errlist[7]<<"";
    errlist[8]<<"";
    errlist[9]<<"";

    errlist[10]<<"";
    errlist[11]<<"";
    errlist[12]<<"109	Dose rate 1 more than 20% away from DRCV."
              <<"123	Dose rate 1 deviates from virtual wedge DRCV profile by 25%.";
    errlist[13]<<"209	Dose rate 2 more than 20% away from DRCV."
              <<"223	Dose rate 2 deviates from VW DRCV profile by 50 mV-sec per sec.";
    errlist[14]<<"";
    errlist[15]<<"860	‘K’ command from IO controller was not received."
              <<"861	‘K’ command from IO controller echoed wrong mode."
             <<"862	‘K’ command from IO controller: head unequal (00 00).";
    errlist[16]<<"858	Console detected LENI versus LENCK mismatch.";
    errlist[17]<<"857	Console detected STCI versus STCCK mismatch.";
    errlist[18]<<"855	Console detected ELMI versus ELMCK mismatch.";
    errlist[19]<<"";

    errlist[20]<<"";
    errlist[21]<<"";
    errlist[22]<<"";
    errlist[23]<<"";
    errlist[24]<<"";
    errlist[25]<<"";
    errlist[26]<<"";
    errlist[27]<<"";
    errlist[28]<<"";
    errlist[29]<<"";

    errlist[30]<<"";
    errlist[31]<<"";
    errlist[32]<<"1106	Undefined packet type received."
              <<"1107	Error in USP open for RDS interface."
             <<"1108	Softpot unload error."
            <<"1110	General read USP error."
           <<"1111	Received packet is too small."
          <<"1112	Received packet not of sequence."
         <<"1113	Too many bytes received from verification system."
        <<"1114	Received segment length does not match buffer count."
       <<"1115	CRC error in received packet."
      <<"1116	Total received count not equal expected count."
     <<"1117	Out of range packet type."
    <<"1118	Packet to send has an invalid type."
    <<"1119	Packet to send is too long."
    <<"1120	Failure in read RIO."
    <<"1121	Failure in write RIO."
    <<"1122	Data packet received is not expected length.";
    errlist[33]<<"52	Gantry overshoot start or stop during rotational treatment.";
    errlist[34]<<"";
    errlist[35]<<"305	BMIS level detected outside of programmed window (← 5)."
              <<"856	Console detected BMIS versus BMICK mismatch.";
    errlist[36]<<"";
    errlist[37]<<"";
    errlist[38]<<"";
    errlist[39]<<"";

    errlist[40]<<"";
    errlist[41]<<"";
    errlist[42]<<"";
    errlist[43]<<"";
    errlist[44]<<"";
    errlist[45]<<"";
    errlist[46]<<"";
    errlist[47]<<"";
    errlist[48]<<"";
    errlist[49]<<"";

    errlist[50]<<"";
    errlist[51]<<"";
    errlist[52]<<"";
    errlist[53]<<"";
    errlist[54]<<"";
    errlist[55]<<"612	Bypass-jumper on pcbs is stikk there."
              <<"613	Interlock-interfaces are missing."
             <<"850	interlock buffer versus relay data mismatch."
            <<"851	Redundant hardware interlock do not match."
           <<"852	Interlock compare chain failure.";
    errlist[56]<<"";
    errlist[57]<<"108	Abs value of dose 1 offset >500mV.";
    errlist[58]<<"110	Dose rate 1 > 4095 counts (saturated) (00).";
    errlist[59]<<"114	Received pressure or temperature < 10 or > 3839 counts."
              <<"211	Received pressure or temperature < 10 or > 3839 counts.";

    errlist[60]<<"208	Abs value of dose 2 offset > 500 mV.";
    errlist[61]<<"210	Dose rate 2 > 4095 counts (saturated) (00).";
    errlist[62]<<"800	Softpot selection error."
              <<"801	Softpot limit error."
             <<"802	Softpot data integrity error."
            <<"803	Mean Attenuation Factor integrity error."
           <<"804	Compensation factor data integrity error."
          <<"805	Foil selection table data integrity error."
         <<"806	Motor calibration data integrity error."
        <<"807	Tap selection table data integrity error."
       <<"808	Trimmers incorrectly calibrated."
      <<"842	Motor selection error."
     <<"870	Dose 1 exceeded display limit of 9999 rads."
    <<"871	Dose 2 exceeded display limit of 9999 rads."
    <<"872	Dose 1 decreasing."
    <<"873	Dose 2 decreasing."
    <<"874	Missing pressure , temperature data."
    <<"875	Missing dose offset 1."
    <<"876	Missing dose offset 2."
    <<"880	Console data item out of range."
    <<"881	Console software detected unintended situation."
    <<"882	Collimator type not defined."
    <<"883	Gantry speed calibration values ate uninitialized."
    <<"884	Gantry direction not set."
    <<"885	Invalid gantry speed. Too fast or too slow."
    <<"890	Wrong gantry direction (during rotational treatment)."
    <<"891	Gantry completion error."
    <<"893	Table position out of range"
    <<"894	Bad length of received table data packet.";
    errlist[63]<<"0	Read-write test to jaw Y controller failed."
              <<"1	F to D circuit failure (jaw Y1)."
             <<"2	Possible jaw Y1 collision or protentiometer failure."
            <<"3	Test for DAC-DRIVE-ADC chain failed (Jaw Y1)."
           <<"7   Jaw Y1 speed control error during speed test."
          <<"11	F to D circuit failure (Jaw Y2)."
         <<"12	Possible jaw Y2 collision or protentiometer failure."
        <<"13	Test for DAC-DRIVE_DAC chain failed (Jaw Y2)."
       <<"17	Jaw Y2 speed control error during speed test."
      <<"20	Read-write test to jaw X controller failed."
     <<"21	F to D circuit failure (jaw X2)."
    <<"22	Possible jaw X2 collision or protentiometer failure."
    <<"23	Test for DAC-DRIVE-ADC chain failed (Jaw X2)."
    <<"27	Jaw X2 speed control error during speed test."
    <<"31	F to D circuit failure (Jaw X1)."
    <<"32	Possible jaw X1 collision or protentiometer failure."
    <<"33	Test for DAC-DRIVE_DAC chain failed (Jaw X1)."
    <<"37	Jaw X1 speed control error during speed test."
    <<"40	Read-write test to jaw Collimator-Gantry controller failed."
    <<"41	F to D circuit failure (Collimator)."
    <<"43	Test for DAC-DRIVE-ADC chain failed (Collimator)."
    <<"47	Collimator speed control error during speed test."
    <<"51	F to D circuit failure (Gantry)."
    <<"53	Test for DAC-DRIVE_DAC chain failed (Gantry)."
    <<"54	Gantry hardware backup counter stopped rotational treatment."
    <<"56	Arc treatment gantry speed error."
    <<"57	Gantry speed control error during speed rest."
    <<"61	RAM test failed."
    <<"62	ROM test failed."
    <<"95	Illegal command received by controller 0."
    <<"4    Jaw Y1 Motor Driver Overcurrent."
    <<"8    Jaw Y1 Motor Driver Self Test Failed."
    <<"9    Jaw Y1 Pot and Encoder Mismatch."
    <<"11   Failure of Y2 jaw F-to-D conversion circuit test.(V7.0)"
    <<"14   Jaw Y2 Motor Driver Overcurrent."
    <<"18   Jaw Y2 Motor Driver Self Test Failed."
    <<"19   Jaw Y2 Pot and Encoder Mismatch."
    <<"24   Jaw X2 Motor Driver Overcurrent."
    <<"28   Jaw X2 Motor Driver Self Test Failed."
    <<"29   Jaw X2 Pot and Encoder Mismatch."
    <<"31   Failure of X1 jaw F-to-D conversion circuit test.(V7.0)."
    <<"34   Jaw X1 Motor Driver Overcurrent."
    <<"38   Jaw X1 Motor Driver Self Test Failed."
    <<"39   Jaw X1 Pot and Encoder Mismatch."
    <<"42   Collimator Motion Stalled."
    <<"44   Collimator Motor Driver Overcurrent."
    <<"48   Collimator Y1 Motor Driver Self Test Failed."
    <<"49   Collimator Pot and Encoder Mismatch."
    <<"51   Failure of gantry F-to-D conversion circuit test.(V7.0)"
    <<"58   Gantry Motor Driver Self Test Failed."
    <<"59   Gantry Pot and Encoder Mismatch."
    <<"61   RAM test failed."
    <<"63   +24V Power Supply monitor Self Test Failed."
    <<"64   +15V Power Supply monitor Self Test Failed"
    <<"65   -15V Power Supply monitor Self Test Failed"
    <<"66   AC Power Fail, Brake Fault, Driver Overtemp Self Test Failed"
    <<"67   ADC Offset Self Test Failed"
    <<"69   +/- 15V Power Supply OK Line Self Test Failed."
       ;
    errlist[64]<<"103	Dose rate 1 offset greater than dose rate 1."
              <<"104	‘O’ or ‘T’ command was received but HV is still on."
             <<"105	Dynamic steering crossplane current exceeds window."
            <<"106	Treatment time is 0 sec."
           <<"111	HV on detected before RAD ON or after Complete or Treatment."
          <<"112	Radiation is ‘on’ but ‘R’ ,’G’ or ‘I’ cmd not received."
         <<"113	‘R’ command was received but HV is ‘off’."
        <<"120	Gantry has moved between READY and RAD ON."
       <<"121	RAM test failed."
      <<"122	ROM test failed."
     <<"124	Jaw position error for VW jaw motion (> 0.4 cm for >500 msec).";
    errlist[65]<<"203	Dose rate 2 offset greater than dose rate 2."
              <<"204	‘O’ or ‘T’ command was received but HV is ‘on’."
             <<"205	Dynamic steering inplane error."
            <<"206	Treatment time programmed to 0.0 seconds."
           <<"214	RAdiation is on but ‘R’ , ‘H’ , or ‘L’ command not received."
          <<"215	‘R’ command was received but HV is OFF."
         <<"221	RAM test failed."
        <<"222	ROM test failed.";
    errlist[66]<<"306	Missing HV-ON singal after RAD-ON command."
              <<"307	Wartchdog (‘F’) command not received from console for 3 seconds."
             <<"308	HV_ON , RAD_ON ,and/or GANTRY_EN erroneously active."
            <<"309	Segment 0 feedback value <segment 0 minimum limit (S0MIN)."
           <<"321	RAM test failed."
          <<"322	RAM test failed."
         <<"323	ROM test and ROM test failed."
        <<"330	Wrong DIP switch setting on controller 3 PCB.";
    errlist[67]<<"405	LED RAM test failed."
              <<"421	RAM test failed."
             <<"422	ROM test failed."
            <<"423	RAM test and ROM test failed.";
    errlist[68]<<"501	Handcont , -slave created emergency (pulled down emergency-line)."
              <<"521	RAM test failed."
             <<"522	ROM test failed."
            <<"525	Wrong DIP-switch setting on controller 5 PCB.";
    errlist[69]<<"602	Wrong command-sequence."
              <<"609	Hv-on , rad-on&gantry_en not released."
             <<"611	Supervisor lines erroneously released."
            <<"621	RAM test failed."
           <<"622	ROM test failed."
          <<"623	RAM test and ROM test failed."
         <<"630	Wrong DIP-switch setting on controller 6 PCB.";

    errlist[70]<<"702	Wrong command sequence."
              <<"703	Check-time for written data is still active."
             <<"704	Foil-table not initialized."
            <<"705	Wrong foil doenload data."
           <<"706	During rad_on : accessories are changed."
          <<"707	Keyboard’s	/RAD_ON EN not released."
         <<"708	Not ready to change enerigies (during HV)."
        <<"709	Invalid parameter for ‘C’ command (Cx)."
       <<"710	Rad_on not released."
      <<"711	Hv_on not released."
     <<"712	Rad_on and Hv_on not released."
    <<"713	Gantry_en not released."
    <<"714	Rad_on and gantry_en not released."
    <<"715	Hv_on and gantry_en not released."
    <<"716	Hv_on ,Rad_on & gantry_en not released."
    <<"717	Foils/targets and BITs (RAD_ON,HV_ON,etc.) are wrong before hv_on."
    <<"718	Foils/targets are wrong before hv_on."
    <<"719	BITs (RAD_ON,HV_ON,etc.) are wrong before hv_on."
    <<"720	Supervisor lines erroneously released."
    <<"721	RAM test failed."
    <<"722	ROM test failed."
    <<"723	RAM test and ROM test failed."
    <<"724	Interrupt lines not released."
    <<"725	Invalid tap selections download (F1, F2)."
    <<"726	Missing ‘0’ command for interrupt."
    <<"727	Bad accessory data received (< 8 counts)."
    <<"728	RAD-ON EN erroneously active."
    <<"729	‘R’ command received with field light on."
    <<"730	Wrong DIP-switch position on slave-PCB.";
    errlist[71]<<"900	Controller 0 logoff detected.";
    errlist[72]<<"901	Controller 1 logoff detected.";
    errlist[73]<<"902	Controller 4 logoff detected.";
    errlist[74]<<"903	Controller 3 logoff detected.";
    errlist[75]<<"";
    errlist[76]<<"905	Controller 5 logoff detected.";
    errlist[77]<<"906	Controller 6 logoff detected.";
    errlist[78]<<"907	Controller 7 logoff detected.";
    errlist[79]<<"96	INvalid data received by controller 0."
              <<"97	Frame-# and checksum received by controller 0 are wrong."
             <<"98	Checksum received by controller 0 is wrong."
            <<"99	Frame-# received by controller 0 is wrong."
           <<"101	Timeout on SIP polling detected by controller 1."
          <<"102	Wrong command sequence detected by controller 1."
         <<"197	Frame-# and checksum received by controller 1 are wrong."
        <<"198	Checksum received by controller 1 is wrong."
       <<"199	Frame-# received by controller 1 is wrong."
      <<"201	Timeout on SIP polling detected by controller 2."
     <<"202	Wrong command sequence detected by controller 2."
    <<"297	Frame-# and checksum received by controller 2 are wrong."
    <<"298	Checksum received by controller 2 is wrong."
    <<"299	Frame-# received by controller 2 is wrong."
    <<"397	Frame-# and checksum received by controller 3 are wrong."
    <<"398	Checksum received by controller 3 is wrong."
    <<"399	Frame-# received by controller 3 is wrong."
    <<"497	Frame-# and checksum received by controller 4 are wrong."
    <<"498	Checksum received by controller 4 is wrong."
    <<"499	Frame-# received by controller 4 is wrong."
    <<"597	Frame-# and checksum received by controller 5 are wrong."
    <<"598	Checksum received by controller 5 is wrong."
    <<"599	Frame-# received by controller 5 is wrong."
    <<"697	Frame-# and checksum received by controller 6 are wrong."
    <<"698	Checksum received by controller 6 is wrong."
    <<"699	Frame-# received by controller 6 is wrong."
    <<"797	Frame-# and checksum received by controller 7 are wrong."
    <<"798	Checksum received by controller 7 is wrong."
    <<"799	Frame-# received by controller 7 is wrong."
    <<"910	C0 echo received from controller 0."
    <<"911	C0 echo received from controller 1."
    <<"912	C0 echo received from controller 2."
    <<"913	C0 echo received from controller 3."
    <<"914	C0 echo received from controller 4."
    <<"915	C0 echo received from controller 5."
    <<"916	C0 echo received from controller 6."
    <<"917	C0 echo received from controller 7."
    <<"920	Timeout on tx to controller 7."
    <<"921	Buffer overflow for data from controllers."
    <<"922	Improper controller address (8-15) received."
    <<"930	Invalid frame number detected from controller 0."
    <<"931	Invalid frame number detected from controller 1."
    <<"932	Invalid frame number detected from controller 2."
    <<"933	Invalid frame number detected from controller 3."
    <<"934	Invalid frame number detected from controller 4."
    <<"935	Invalid frame number detected from controller 5."
    <<"936	Invalid frame number detected from controller 7."
    <<"937	Invalid frame number detected from controller 7."
    <<"938	Invalid frame number detected from controller 8."
    <<"939	Invalid frame number detected from controller 9."
    <<"940	Invalid checksum detected from controller 0."
    <<"941	Invalid checksum detected from controller 1."
    <<"942	Invalid checksum detected from controller 2."
    <<"943	Invalid checksum detected from controller 3."
    <<"944	Invalid checksum detected from controller 4."
    <<"945	Invalid checksum detected from controller 5."
    <<"946	Invalid checksum detected from controller 6."
    <<"947	Invalid checksum detected from controller 7."
    <<"948	Invalid checksum detected from controller 8."
    <<"949	Invalid checksum detected from controller 9."
    <<"950	Invalid command type received from controller 0."
    <<"951	Invalid command type received from controller 1."
    <<"952	Invalid command type received from controller 2."
    <<"953	Invalid command type received from controller 3."
    <<"954	Invalid command type received from controller 4."
    <<"955	Invalid command type received from controller 5."
    <<"956	Invalid command type received from controller 6."
    <<"957	Invalid command type received from controller 7."
    <<"958	Invalid command type received from controller 8."
    <<"959	Invalid command type received from controller 9."
    <<"960	Invalid length for command type from controller 0."
    <<"961	Invalid length for command type from controller 1."
    <<"962	Invalid length for command type from controller 2."
    <<"963	Invalid length for command type from controller 3."
    <<"964	Invalid length for command type from controller 4."
    <<"965	Invalid length for command type from controller 5."
    <<"966	Invalid length for command type from controller 6."
    <<"967	Invalid length for command type from controller 7."
    <<"968	Invalid length for command type from controller 8."
    <<"969	Invalid length for command type from controller 9."
    <<"970	Invalid data received from controller 0."
    <<"971	Invalid data received from controller 1."
    <<"972	Invalid data received from controller 2."
    <<"973	Invalid data received from controller 3."
    <<"974	Invalid data received from controller 4."
    <<"975	Invalid data received from controller 5."
    <<"976	Invalid data received from controller 6."
    <<"977	Invalid data received from controller 7."
    <<"978	Invalid data received from controller 8."
    <<"979	Invalid data received from controller 9."
    <<"980	Motor controller deadman timeout (arc or skip only)."
    <<"981	Dose 1 deadman timeout."
    <<"982	Dose 2 deadman timeout."
    <<"983	Beam controller deadman timeout."
    <<"990	Dose 1 vs. dose 2 time synch error."
    <<"991	Dose 1 vs. console time synch error.";

    errlist[80]<<"350	BMI timeout.";
    errlist[81]<<"100	MLC not response accept signal."
              <<"101	MLC move stop exception."
                <<"102  MLC move exception"
            <<"108	    MLC connect fail.";
    errlist[82]<<"116	Dose out-of-range for 8 degree arc sample."
              <<"117	Gantry not moving or moving in the wrong direction.";
    errlist[83]<<"";
    errlist[84]<<"";
    errlist[85]<<"869	‘K’ command from IO controller: bits unequal (00 00).";
    errlist[86]<<"";
    errlist[87]<<"5	A change in jaw Y1 position (at least ← 0.2 cm) without command."
              <<"6	Jaw Y1 position wrong during motion.";
    errlist[88]<<"15	A change in jaw Y2 position (at least ← 0.2 cm) without command."
              <<"16	Jaw Y2 position wrong during motion.";
    errlist[89]<<"25	A change in jaw X2 position (at least ← 0.2 cm) without command."
              <<"26	Jaw X2 position wrong during motion.";

    errlist[90]<<"35	A change in jaw X1 position (at least ← 0.2 cm) without command."
              <<"36	Jaw X1 position wrong during motion.";
    errlist[91]<<"45	Change in collimator angle (at least 21 counts) without command.";
    errlist[92]<<"55	Change in gantry angle (at least 21 counts) without command.";
    errlist[93]<<"118	Malfunction in the parameter back-up system.";
    errlist[94]<<"";
    errlist[95]<<"";
    errlist[96]<<"1000 	V&R interface in wrong state for setup packet."
              <<"1001	V&R interface in wrong state for setup request."
             <<"1002	V&R interface in wrong state for permission packet."
            <<"1003	V&R interface in wrong state for actual treatment request."
           <<"1004	V&R interface in wrong state for recall request."
          <<"1005	There is no patient available for recall."
         <<"1006	Undefined packet type received."
        <<"1007	Error in USP open for V&R interface."
       <<"1008	Softpot upload error."
      <<"1009	Softpot download error."
     <<"1010	General read USP error."
    <<"1011	Received packet is too small."
    <<"1012	Received packet out of sequence."
    <<"1013	Too many bytes raceived from verification system."
    <<"1014	Received segment length does not match buffer count."
    <<"1015	CRC error in received packet."
    <<"1016	total recevied count not equal expected count."
    <<"1017	Out of range packet type."
    <<"1018	Packet to send has an invalid type."
    <<"1019 	Packet to send is too long."
    <<"1020	Failure in read RIO."
    <<"1021	Failure in write RIO."
    <<"1022	Data packet received is not expected length."
    <<"1030	Out-of-range leaf data received."
    <<"1031	Overlapping leaf pair data received.";
    errlist[97]<<"119	Dose calculation timeout (check parameter backup)."
              <<"219	Dose calculation error.";
    errlist[98]<<"";
    errlist[99]<<"";

    errlist[100]<<"";
    errlist[101]<<"";
    errlist[102]<<"";
    errlist[103]<<"";
    errlist[104]<<"";
    errlist[105]<<"";
    errlist[106]<<"";
    errlist[107]<<"1050	Line establishment timeout."
               <<"1051	Packet transmission timeout (rx or tx)."
              <<"1060	Receive buffer not long enough."
             <<"1061	Transmit buffer not long enough."
            <<"1062	Too many packets queued for transmission."
           <<"1070 Length of received packet does not match type."
          <<"1071	Out-of-range leaf data received."
         <<"1072	Overlapping leaf pair data received."
        <<"1080	Timeout waiting for leaf data."
       <<"1081	Timeout waiting for MLC status data.";
    errlist[108]<<"1150	Slave 9 CPU 1 ROM check error."
               <<"1151	Slave 9 CPU 1 RAM check error."
              <<"1152 Slave 9 CPU 2 ROM check error."
             <<"1153	Slave 9 CPU 2 RAM check error."
            <<"1154	Slave 9 DPRAM check error."
           <<"1155	Slave 9 PROTRACTOR	check error."
          <<"1157	Master ROM check error."
         <<"1158	Master RAM check error."
        <<"1159	Master DPRAM Slave 9 check error."
       <<"1160	Master DPRAM Slave 0 check error."
      <<"1161	Master DPRAM Slave 2 check error."
     <<"1162	Master HC serial line check error."
    <<"1163	Master console serial line check error."
    <<"1164	Slave 0 ROM check error."
    <<"1165	Slave 0 RAM check error."
    <<"1166	Slave 0 DPRAM master check error."
    <<"1167	Slave 0 DPRAM Slave 1 check error."
    <<"1171	Slave 1 CPU 1 ROM check error."
    <<"1172	Slave 1 CPU 1 RAM check error."
    <<"1173	Slave 1 CPU 2 ROM check error."
    <<"1174	Slave 1 CPU 2 RAM check error."
    <<"1175	Slave 1 DPRAM check error."
    <<"1176	Slave 1 sensor interface ROM check error."
    <<"1177	Slave 1 sensor interface RAM check error."
    <<"1178	Slave 2 ROM check error."
    <<"1179	Slave 2 RAM check error."
    <<"1180	Slave 2 DPRAM master check error."
    <<"1181	Slave 2 DPRAM Slave 3 check error."
    <<"1185	Slave 3 CPU 1 ROM check error."
    <<"1186	Slave 3 CPU 1 RAM check error."
    <<"1187	Slave 3 CPU 2 ROM check error."
    <<"1188	Slave 3 CPU 2 RAM check error."
    <<"1189	Slave 3 DPRAM check error."
    <<"1190	Slave 3 sensor interface ROM check error."
    <<"1191	Slave 3 sensor interface RAM check error."
    <<"1192	MASTER2 MLC HC ROM check error."
    <<"1193	MASTER2 MLC HC RAM check error."
    <<"1197	MASTER2 BBRAM check error.";
    errlist[109]<<"";

    errlist[110]<<"895	Timeout while waiting for upper defining head setup completion."
               <<"896	Timeout while waiting for field light hulb acknowledgement.";
    errlist[111]<<"";
    errlist[112]<<"hardware fail";
    errlist[113]<<"";
    errlist[114]<<"";
    errlist[115]<<"";
    errlist[116]<<"";
    errlist[117]<<"";
    errlist[118]<<"";
    errlist[119]<<"";

    errlist[120]<<"";
    errlist[121]<<"";
    errlist[122]<<"";
    errlist[123]<<"";
    errlist[124]<<"";
    errlist[125]<<"91   Beam ID Error"
                  <<"92    Controlpoint ID Error"
                  <<"93 Beam Type Error"
                  <<"94    Radiation Type Error"
                  <<"95  Beam MU Error"
                  <<"96    Controlpoint MU Error"
                  <<"97  Energy  Error"
                  <<"98    Doserate Error"
                  <<"99  Beam Dose Error"
                  <<"100   Gantry Angle Error"
                  <<"101 Collimator Angle Error"
                  <<"102   Accessory1 Error"
                  <<"103   Accessory2 Error"
                  <<"104   Accessory3 Error"
                  <<"105 Technique Type Error"
                   <<"106   Jaw X1 Error"
                 <<"107   Jaw X2 Error"
               <<"108   Jaw Y1 Error"
                 <<"109   Jaw Y2 Error"
                   <<"110   Mode Error"
                     <<"111 Heartbeat Lost"
                       <<"112   Invalid Frame"
                         <<"113 Beam Number Error"
                           <<"114   Delivered MU Error"
                             <<"115 Controlpoint Number Error"
                               <<"116   First Jaw Set Error"
                                 <<"117 Machine Name Error";
    errlist[126]<<"100	Upper not response ACCEPT."
               <<"109	Upper connect fail.";
    errlist[127]<<"1200	No ABEND has occurred";
}

void AccUi::InitKeyCode()
{
    code[0x30] = "REASKY";
    code[0x38] = "OPERAT";
    code[0x40] = "RESET";
    code[0x0a] = "CLEAR";
    code[0x0b] = "ACTPRE";
    code[0x0c] = "ENABLE";
    code[0x10] = "STOP";
    code[0x18] = "RADON";
    code[0x20] = "RADOFF";
    code[0x28] = "ACCEPT";
    code[0x48] = "STNDBY";
    code[0x50] = "PROGRM";
    code[0x6]  = "RADONK";
    code[0x8]  = "ESC";
    code[0x7]  = "F1";
    code[0x0f] = "F2";
    code[0x17] = "F3";
    code[0x1f] = "F4";
    code[0x27] = "F5";
    code[0x2f] = "F6";
    code[0x37] = "F7";
    code[0x3f] = "F8";
    code[0x47] = "F9";
    code[0x4f] = "F10";
    code[0x56] = "F11";
    code[0x5e] = "F12";
    code[0x57] = "PrintScrn";
    code[0x5f] = "ScrollLock";
    code[0x62] = "Pause";
    code[0x0e] = "`";
    code[0x16] = "1";
    code[0x1e] = "2";
    code[0x26] = "3";
    code[0x25] = "4";
    code[0x2e] = "5";
    code[0x36] = "6";
    code[0x1c] = "A";
    code[0x1b] = "S";
    code[0x23] = "D";
    code[0x2b] = "F";
    code[0x34] = "G";
    code[0x33] = "H";
    code[0x3b] = "J";
    code[0x42] = "K";
    code[0x4b] = "L";
    code[0x4c] = ";";
    code[0x52] = "'";
    code[0x5a] = "Enter";
    code[0x6b] = "4";
    code[0x73] = "5";
    code[0x74] = "6";
    code[0x1a] = "Z";
    code[0x22] = "X";
    code[0x21] = "C";
    code[0x2a] = "V";
    code[0x32] = "B";
    code[0x31] = "N";
    code[0x3a] = "M";
    code[0x41] = ",";
    code[0x49] = ".";
    code[0x4a] = "/";
    code[0x69] = "1";
    code[0x72] = "2";
    code[0x7a] = "3";
    code[0x79] = "Enter";
    code[0x29] = "SPACE";
    code[0x61] = "←";
    code[0x6a] = "→";
    code[0x70] = "0";
    code[0x71] = ".";
    code[0x0e] = "~";
    code[0x16] = "!";
    code[0x1e] = "@";
    code[0x26] = "#";
    code[0x25] = "$";
    code[0x15] = "Q";
    code[0x1d] = "W";
    code[0x24] = "E";
    code[0x2d] = "R";
    code[0x2c] = "T";
    code[0x35] = "Y";
    code[0x3c] = "U";
    code[0x43] = "I";
    code[0x44] = "O";
    code[0x4d] = "P";
    code[0x1c] = "A";
    code[0x1b] = "S";
    code[0x23] = "D";
    code[0x2b] = "F";
    code[0x34] = "G";
    code[0x33] = "H";
    code[0x3b] = "J";
    code[0x42] = "K";
    code[0x4b] = "L";
    code[0x1a] = "Z";
    code[0x22] = "X";
    code[0x21] = "C";
    code[0x2a] = "V";
    code[0x32] = "B";
    code[0x31] = "N";
    code[0x3a] = "M";
    code[0x3d] = "7";
    code[0x3e] = "8";
    code[0x46] = "9";
    code[0x45] = "0";
    code[0x4e] = "-";
    code[0x55] = "=";
    code[0x66] = "BackSpace";
    code[0x67] = "Insert";
    code[0x6e] = "Home";
    code[0x6f] = "PageUp";
    code[0x76] = "NumberLock";
    code[0x77] = "/";
    code[0x7e] = "*";
    code[0x84] = "-";
    code[0x0d] = "Tab";
    code[0x15] = "Q";
    code[0x1d] = "W";
    code[0x24] = "E";
    code[0x2d] = "R";
    code[0x2c] = "T";
    code[0x35] = "Y";
    code[0x3c] = "U";
    code[0x43] = "I";
    code[0x44] = "O";
    code[0x4d] = "P";
    code[0x54] = "[";
    code[0x5b] = "]";
    code[0x5c] = "\\";
    code[0x64] = "DEL";
    code[0x65] = "End";
    code[0x6d] = "PageDown";
    code[0x6c] = "7";
    code[0x75] = "8";
    code[0x7d] = "9";
    code[0x7c] = "+";
    code[0x2e] = "%";
    code[0x36] = "^(↑)";
    code[0x3d] = "&";
    code[0x3e] = "*";
    code[0x46] = "(";
    code[0x45] = ")";
    code[0x4e] = "_";
    code[0x55] = "+";
    code[0x54] = "{";
    code[0x5b] = "}";
    code[0x5c] = "|";
    code[0x4c] = ":";
    code[0x52] = "”";
    code[0x41] = "<";
    code[0x49] = ">";
    code[0x4a] = "?";
    code[0x15] = "q";
    code[0x1d] = "w";
    code[0x24] = "e";
    code[0x2d] = "r";
    code[0x2c] = "t";
    code[0x35] = "y";
    code[0x3c] = "u";
    code[0x43] = "i";
    code[0x44] = "o";
    code[0x4d] = "p";
    code[0x1c] = "a";
    code[0x1b] = "s";
    code[0x23] = "d";
    code[0x2b] = "f";
    code[0x34] = "g";
    code[0x33] = "h";
    code[0x3b] = "j";
    code[0x42] = "k";
    code[0x4b] = "l";
    code[0x1a] = "z";
    code[0x22] = "x";
    code[0x21] = "c";
    code[0x2a] = "v";
    code[0x32] = "b";
    code[0x31] = "n";
    code[0x3a] = "m";

}

void AccUi::InitSvc()
{
    softpot_beam1_focus = 0;
    softpot_beam2_focus = 0;
    softpot_trig_focus = 0;
    softpot_dose1_focus = 0;
    softpot_dose2_focus = 0;
    last_softpot_page = 0;

    tmp_value_2 = "";
    tmp_finish_value = "";

    softpot_widge[0]<<ui->leSoftpotsBeam1AfcpRsch<<ui->leSoftpotsBeam1InjeRsch
                    <<ui->leSoftpotsBeam1InjiRsch<<ui->leSoftpotsBeam1ElmRsch
                    <<ui->leSoftpotsBeam1BmiRsch<<ui->leSoftpotsBeam1StcRsch
                    <<ui->leSoftpotsBeam1LensRsch<<ui->leSoftpotsBeam1PfnRsch
                    <<ui->leSoftpotsBeam1IpfnRsch<<ui->leSoftpotsBeam1SominRsch;

    softpot_widge[1]<<ui->leSoftpotsBeam2DsgxResrch<<ui->leSoftpotsBeam2DsgyResrch
                    <<ui->leSoftpotsBeam2DswxResrch<<ui->leSoftpotsBeam2DswyResrch
                    <<ui->leSoftpotsBeam2BalxResrch<<ui->leSoftpotsBeam2BalyResrch
                    <<ui->leSoftpotsBeam2ElmckResrch<<ui->leSoftpotsBeam2BmickResrch
                    <<ui->leSoftpotsBeam2StcckResrch<<ui->leSoftpotsBeam2LenckResrch;

    softpot_widge[2]<<ui->leSoftpotsTrigModtResrch<<ui->leSoftpotsTrigInjtResrch
                    <<ui->leSoftpotsTrigAfctResrch<<ui->leSoftpotsTrigRftResrch
                    <<ui->leSoftpotsTrigTst0Resrch<<ui->leSoftpotsTrigTst1Resrch;

    softpot_widge[3]<<ui->leSoftpotsDose1D1gResrch<<ui->leSoftpotsDose1D2gResrch
                    <<ui->leSoftpotsDose1PrfpResrch<<ui->leSoftpotsDose1DrcvResrch;

    softpot_widge[4]<<ui->leSoftpotsDose2S0gResrch<<ui->leSoftpotsDose2S1gResrch
                    <<ui->leSoftpotsDose2S2gResrch<<ui->leSoftpotsDose2S3gResrch
                    <<ui->leSoftpotsDose2S4gResrch<<ui->leSoftpotsDose2SwiResrch
                    <<ui->leSoftpotsDose2FwiResrch<<ui->leSoftpotsDose2DostResrch;


    motor_adjust[0]<<ui->leMotAdjJawY1SS<<ui->leMotAdjJawY2SS
                   <<ui->leMotAdjJawX2SS<<ui->leMotAdjJawX1SS
                   <<ui->leMotAdjColliSS<<ui->leMotAdjGantrySS;
    motor_adjust[1]<<ui->leMotAdjJawY1FS<<ui->leMotAdjJawY2FS
                   <<ui->leMotAdjJawX2FS<<ui->leMotAdjJawX1FS
                   <<ui->leMotAdjColliFS<<ui->leMotAdjGantryFS;
    motor_adjust[2]<<ui->leMotAdjJawY1SDS<<ui->leMotAdjJawY2SDS
                   <<ui->leMotAdjJawX2SDS<<ui->leMotAdjJawX1SDS
                   <<ui->leMotAdjColliSDS<<ui->leMotAdjGantrySDS;
    motor_adjust[3]<<ui->leMotAdjJawY1SDD<<ui->leMotAdjJawY2SDD
                   <<ui->leMotAdjJawX2SDD<<ui->leMotAdjJawX1SDD
                   <<ui->leMotAdjColliSDD<<ui->leMotAdjGantrySDD;

    svc_waring <<ui->leSvcWarnBI1<<ui->leSvcWarnBI2
               <<ui->leSvcWarnJawA1<<ui->leSvcWarnJawA2
               <<ui->leSvcWarnJawB1<<ui->leSvcWarnJawB2;
    svc_warn_size =0;
    has_press_return_2 = false;
    motadj_column =6;
    motadj_row =4;

}
void AccUi::SetSoftpotDownEdit(QLineEdit *lineedit)  //duanzhibin1128
{

    //tmp_value = lineedit->text();
    int k = 0;
   // lineedit->setStyleSheet("background:white");

    k =lineedit->text().toInt();
    k++;
    lineedit->setText(QString::number(k));
    //qDebug()<<lineedit->text();
    tmp_value = lineedit->text();
    lineedit->selectAll();
    //do something that enterPress do
}

void AccUi::SetSoftpotUpFloatEdit(QLineEdit *lineedit)
{
    float k = 0;
    QString tmpFloat;
    // float tmp_float;
   //lineedit->setStyleSheet("background:white");

    k = lineedit->text().toFloat();
    k = k - 0.1;
    tmpFloat.sprintf("%.1f",k);
    lineedit->setText(tmpFloat);
    //qDebug()<<lineedit->text();
    tmp_value =lineedit->text();
    lineedit->selectAll();
}

void AccUi::SetSoftpotDownFloatEdit(QLineEdit *lineedit)
{
    float k = 0;
    QString tmpFloat;
  // lineedit->setStyleSheet("background:white");

    k =lineedit->text().toFloat();
    k+= 0.1;
    tmpFloat.sprintf("%.1f",k);
    lineedit->setText(tmpFloat);
    qDebug()<<lineedit->text();
    tmp_value = lineedit->text();
    lineedit->selectAll();
}

void AccUi::SetSoftpotFinishEdit(QLineEdit *lineedit)
{
    has_press_return = true;
    tmp_finish_value =lineedit->text();
    if(tmp_finish_value != "")
        tmp_value = tmp_finish_value;
    lineedit->setFocus();
    lineedit->selectAll();
   // lineedit->setStyleSheet("background:black");
}

void AccUi::SetSoftpotNextEdit(QLineEdit *lineedit)
{
    lineedit->setFocus();
    lineedit->selectAll();
    tmp_value = lineedit->text();
   // lineedit->setStyleSheet("background:white");
    has_press_return = false;
}

void AccUi::SetSoftpotOldEdit(QLineEdit *lineedit)
{
    if(!has_press_return )
    {
        lineedit->setText(tmp_value);
        has_press_return =true;
    }
    else
    {
        if(tmp_finish_value =="")
            lineedit->setText(tmp_value);
        else
        {
            if( tmp_finish_value != lineedit->text())
                lineedit->setText(tmp_finish_value);
        }
    }
   // lineedit->setStyleSheet("background:black");
    lineedit->clearFocus();
}

void AccUi::SetSoftpotUpEdit(QLineEdit *lineedit)
{
    int k = 0;
   //lineedit->setStyleSheet("background:white");

    k = lineedit->text().toInt();
    k--;
    lineedit->setText(QString::number(k));
    //  qDebug()<<lineedit->text();
    tmp_value = lineedit->text();
    lineedit->selectAll();
}


