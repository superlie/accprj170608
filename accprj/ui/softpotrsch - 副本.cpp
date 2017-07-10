#include "accui.h"
#include "ui_accui.h"

void AccUi::on_leSoftpotsBeam1AfcpRsch_returnPressed()
{
    QString disp;
    UINT16 afcp;
    UINT32 afcp_ret;
    UINT16 old_afcp;
    bool suc;
    afcp = ui->leSoftpotsBeam1AfcpRsch->text().toInt();
    suc = SlaveBase::CheckValue(afcp, AFCP_RSCH_CK, afcp_ret);
    if(!suc)
    {
        old_afcp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->afcp[0], AFCP_RSCH_CK);
        disp.sprintf("%d", old_afcp);
        ui->leSoftpotsBeam1AfcpRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1AfcpRsch);

        return;
    }
    ui->leSoftpotsBeam1AfcpRsch->setText(QString::number(afcp));
    beam->afcp[0] = afcp_ret;
    suc = beam->SendAfcpInjeInji();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1AfcpRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1InjeRsch_returnPressed()
{
    QString disp;
    UINT16 inje;
    UINT32 inje_ret;
    UINT16 old_inje;
    bool suc;
    inje = ui->leSoftpotsBeam1InjeRsch->text().toInt();
    suc = SlaveBase::CheckValue(inje, INJE_RSCH_CK, inje_ret);
    if(!suc)
    {
        old_inje = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inje[0], INJE_RSCH_CK);
        disp.sprintf("%d", old_inje);
        ui->leSoftpotsBeam1InjeRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1InjeRsch);

        return;
    }
    beam->inje[0] = inje_ret;
    ui->leSoftpotsBeam1InjeRsch->setText(QString::number(inje));
    suc = beam->SendAfcpInjeInji();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1InjeRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1InjiRsch_returnPressed()
{
    QString disp;
    UINT16 inji;
    UINT32 inji_ret;
    UINT16 old_inji;
    bool suc;
    inji = ui->leSoftpotsBeam1InjiRsch->text().toInt();
    suc = SlaveBase::CheckValue(inji, INJI_RSCH_CK, inji_ret);
    if(!suc)
    {
        old_inji = (UINT16)SlaveBase::ConvertMemberToFloat(beam->inji[0], INJI_RSCH_CK);
        disp.sprintf("%d", old_inji);
        ui->leSoftpotsBeam1InjiRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1InjiRsch);

        return;
    }
    beam->inji[0] = inji_ret;
    ui->leSoftpotsBeam1InjiRsch->setText(QString::number(inji));
    suc = beam->SendAfcpInjeInji();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1InjiRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1ElmRsch_returnPressed()
{
    QString disp;
    UINT16 elm;
    UINT32 elm_ret;
    UINT16 old_elm;
    bool suc;
    elm = ui->leSoftpotsBeam1ElmRsch->text().toInt();
    suc = SlaveBase::CheckValue(elm, ELM_RSCH_CK, elm_ret);
    if(!suc)
    {
        old_elm = (UINT16)SlaveBase::ConvertMemberToFloat(beam->elm[0], ELM_RSCH_CK);
        disp.sprintf("%d", old_elm);
        ui->leSoftpotsBeam1ElmRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1ElmRsch);

        return;
    }
    beam->elm[0] = elm_ret;
    ui->leSoftpotsBeam1ElmRsch->setText(QString::number(elm));
    suc = beam->SendElmBmiStcLen();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1ElmRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1BmiRsch_returnPressed()
{
    QString disp;
    UINT16 bmi;
    UINT32 bmi_ret;
    UINT16 old_bmi;
    bool suc;
    bmi = ui->leSoftpotsBeam1BmiRsch->text().toInt();
    suc = SlaveBase::CheckValue(bmi, BMI_RSCH_CK, bmi_ret);
    if(!suc)
    {
        old_bmi = (UINT16)SlaveBase::ConvertMemberToFloat(beam->bmi[0], BMI_RSCH_CK);
        disp.sprintf("%d", old_bmi);
        ui->leSoftpotsBeam1BmiRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1BmiRsch);

        return;
    }
    beam->bmi[0] = bmi_ret;
    ui->leSoftpotsBeam1BmiRsch->setText(QString::number(bmi));
    suc = beam->SendElmBmiStcLen();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1BmiRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1StcRsch_returnPressed()
{
    QString disp;
    UINT16 stc;
    UINT32 stc_ret;
    UINT16 old_stc;
    bool suc;
    stc = ui->leSoftpotsBeam1StcRsch->text().toInt();
    suc = SlaveBase::CheckValue(stc, STC_RSCH_CK, stc_ret);
    if(!suc)
    {
        old_stc = (UINT16)SlaveBase::ConvertMemberToFloat(beam->stc[0], STC_RSCH_CK);
        disp.sprintf("%d", old_stc);
        ui->leSoftpotsBeam1StcRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1StcRsch);

        return;
    }
    beam->stc[0] = stc_ret;
    ui->leSoftpotsBeam1StcRsch->setText(QString::number(stc));
    suc = beam->SendElmBmiStcLen();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1StcRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1LensRsch_returnPressed()
{
    QString disp;
    UINT16 lens;
    UINT32 lens_ret;
    UINT16 old_lens;
    bool suc;
    lens = ui->leSoftpotsBeam1LensRsch->text().toInt();
    suc = SlaveBase::CheckValue(lens, LENS_RSCH_CK, lens_ret);
    if(!suc)
    {
        old_lens = (UINT16)SlaveBase::ConvertMemberToFloat(beam->len[0], LENS_RSCH_CK);
        disp.sprintf("%d", old_lens);
        ui->leSoftpotsBeam1LensRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1LensRsch);

        return;
    }
    beam->len[0] = lens_ret;
    ui->leSoftpotsBeam1LensRsch->setText(QString::number(lens));
    suc = beam->SendElmBmiStcLen();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1LensRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1PfnRsch_returnPressed()
{
    QString disp;
    UINT16 pfn;
    UINT32 pfn_ret;
    UINT16 old_pfn;
    bool suc;
    pfn = ui->leSoftpotsBeam1PfnRsch->text().toInt();
    suc = SlaveBase::CheckValue(pfn, PFN_RSCH_CK, pfn_ret);
    if(!suc)
    {
        old_pfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->pfn[0], PFN_RSCH_CK);
        disp.sprintf("%d", old_pfn);
        ui->leSoftpotsBeam1PfnRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1PfnRsch);

        return;
    }
    beam->pfn[0] = pfn_ret;
    ui->leSoftpotsBeam1PfnRsch->setText(QString::number(pfn));
    suc = beam->SendPfnPrfpIpfnSomin();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1PfnRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1IpfnRsch_returnPressed()
{
    QString disp;
    UINT16 ipfn;
    UINT32 ipfn_ret;
    UINT16 old_ipfn;
    bool suc;
    ipfn = ui->leSoftpotsBeam1IpfnRsch->text().toInt();
    suc = SlaveBase::CheckValue(ipfn, IPFN_RSCH_CK, ipfn_ret);
    if(!suc)
    {
        old_ipfn = (UINT16)SlaveBase::ConvertMemberToFloat(beam->ipfn[0], IPFN_RSCH_CK);
        disp.sprintf("%d", old_ipfn);
        ui->leSoftpotsBeam1IpfnRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1IpfnRsch);

        return;
    }
    beam->ipfn[0] = ipfn_ret;
    ui->leSoftpotsBeam1IpfnRsch->setText(QString::number(ipfn));
    suc = beam->SendPfnPrfpIpfnSomin();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1IpfnRsch);
    return;
}

void AccUi::on_leSoftpotsBeam1SominRsch_returnPressed()
{
    QString disp;
    UINT16 somin;
    UINT32 somin_ret;
    UINT16 old_somin;
    bool suc;
    somin = ui->leSoftpotsBeam1SominRsch->text().toInt();
    suc = SlaveBase::CheckValue(somin, IPFN_RSCH_CK, somin_ret);
    if(!suc)
    {
        old_somin = (UINT16)SlaveBase::ConvertMemberToFloat(beam->somin[0], SOMIN_RSCH_CK);
        disp.sprintf("%d", old_somin);
        ui->leSoftpotsBeam1SominRsch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam1SominRsch);

        return;
    }
    beam->somin[0] = somin_ret;
    ui->leSoftpotsBeam1SominRsch->setText(QString::number(somin));
    suc = beam->SendPfnPrfpIpfnSomin();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam1SominRsch);
    return;
}

void AccUi::on_leSoftpotsBeam2DsgxResrch_returnPressed()
{
    QString disp;
    UINT16 dsgx;
    UINT32 dsgx_ret;
    UINT16 old_dsgx;
    bool suc;
    dsgx = ui->leSoftpotsBeam2DsgxResrch->text().toInt();
    suc = SlaveBase::CheckValue(dsgx, DSGX_RSCH_CK, dsgx_ret);
    if(!suc)
    {
        old_dsgx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[0].ds_gainx, DSGX_RSCH_CK);
        disp.sprintf("%d", old_dsgx);
        ui->leSoftpotsBeam2DsgxResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2DsgxResrch);

        return;
    }
    dosei->dyn_steer[0].ds_gainx = dsgx_ret;
    ui->leSoftpotsBeam2DsgxResrch->setText(QString::number(dsgx));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2DsgxResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2DsgyResrch_returnPressed()
{
    QString disp;
    UINT16 dsgy;
    UINT32 dsgy_ret;
    UINT16 old_dsgy;
    bool suc;
    dsgy = ui->leSoftpotsBeam2DsgyResrch->text().toInt();
    suc = SlaveBase::CheckValue(dsgy, DSGY_RSCH_CK, dsgy_ret);
    if(!suc)
    {
        old_dsgy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[0].dsGainY, DSGY_RSCH_CK);
        disp.sprintf("%d", old_dsgy);
        ui->leSoftpotsBeam2DsgyResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2DswxResrch);

        return;
    }
    doseii->dose2_gain[0].dsGainY = dsgy_ret;
    ui->leSoftpotsBeam2DsgyResrch->setText(QString::number(dsgy));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2DsgyResrch);
    //return;
    return;
}

void AccUi::on_leSoftpotsBeam2DswxResrch_returnPressed()
{
    QString disp;
    UINT16 dswx;
    UINT32 dswx_ret;
    UINT16 old_dswx;
    bool suc;
    dswx = ui->leSoftpotsBeam2DswxResrch->text().toInt();
    suc = SlaveBase::CheckValue(dswx, DSWX_RSCH_CK, dswx_ret);
    if(!suc)
    {
        old_dswx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[0].ds_wind, DSWX_RSCH_CK);
        disp.sprintf("%d", old_dswx);
        ui->leSoftpotsBeam2DswxResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2DswxResrch);

        return;
    }
    dosei->dyn_steer[0].ds_wind = dswx_ret;
    suc = dosei->SendDynamicSteer();
    ui->leSoftpotsBeam2DswxResrch->setText(QString::number(dswx));
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2DswxResrch);
    return;
}
//not found
void AccUi::on_leSoftpotsBeam2DswyResrch_returnPressed()
{
    QString disp;
    UINT16 dswy;
    UINT32 dswy_ret;
    UINT16 old_dswy;
    bool suc;
    dswy = ui->leSoftpotsBeam2DswyResrch->text().toInt();
    suc = SlaveBase::CheckValue(dswy, DSWY_RSCH_CK, dswy_ret);
    if(!suc)
    {
        old_dswy = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dyn_steer_window[0], DSWY_RSCH_CK);
        disp.sprintf("%d", old_dswy);
        ui->leSoftpotsBeam2DswyResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2DswyResrch);
        return;
    }
    doseii->dyn_steer_window[0] = dswy_ret;
    ui->leSoftpotsBeam2DswyResrch->setText(QString::number(dswy));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();

    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2DswyResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2BalxResrch_returnPressed()
{
    QString disp;
    UINT16 balx;
    UINT32 balx_ret;
    UINT16 old_balx;
    bool suc;
    balx = ui->leSoftpotsBeam2BalxResrch->text().toInt();
    suc = SlaveBase::CheckValue(balx, BALX_RSCH_CK, balx_ret);
    if(!suc)
    {
        old_balx = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dyn_steer[0].balx, BALX_RSCH_CK);
        disp.sprintf("%d", old_balx);
        ui->leSoftpotsBeam2BalxResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2BalxResrch);

        return;
    }
    dosei->dyn_steer[0].balx = balx_ret;
    ui->leSoftpotsBeam2BalxResrch->setText(QString::number(balx));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2BalxResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2BalyResrch_returnPressed()
{
    QString disp;
    UINT16 baly;
    UINT32 baly_ret;
    UINT16 old_baly;
    bool suc;
    baly = ui->leSoftpotsBeam2BalyResrch->text().toInt();
    suc = SlaveBase::CheckValue(baly, BALY_RSCH_CK, baly_ret);
    if(!suc)
    {
        old_baly = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[0].baly, BALY_RSCH_CK);
        disp.sprintf("%d", old_baly);
        ui->leSoftpotsBeam2BalyResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2BalyResrch);
        return;
    }
    doseii->dose2_gain[0].baly = baly_ret;
    ui->leSoftpotsBeam2BalyResrch->setText(QString::number(baly));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2BalyResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2ElmckResrch_returnPressed()
{
    QString disp;
    float elmck;
    UINT32 elmck_ret;
    float old_elmck;
    bool suc;
    elmck = ui->leSoftpotsBeam2ElmckResrch->text().toFloat();
    suc = SlaveBase::CheckValue(elmck, ELMCK_RSCH_CK, elmck_ret);
    if(!suc)
    {
        old_elmck = SlaveBase::ConvertMemberToFloat(beam->elmck[0], ELMCK_RSCH_CK);
        disp.sprintf("%0.1f", old_elmck);
        ui->leSoftpotsBeam2ElmckResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2ElmckResrch);
        return;
    }
    beam->elmck[0] = elmck_ret;
    disp.sprintf("%.1f",elmck);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2ElmckResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2BmickResrch_returnPressed()
{
    QString disp;
    float bmick;
    UINT32 bmick_ret;
    float old_bmick;
    bool suc;
    bmick = ui->leSoftpotsBeam2BmickResrch->text().toFloat();
    suc = SlaveBase::CheckValue(bmick, BMICK_RSCH_CK, bmick_ret);
    if(!suc)
    {
        old_bmick = SlaveBase::ConvertMemberToFloat(beam->bmick[0], BMICK_RSCH_CK);
        disp.sprintf("%0.1f", old_bmick);
        ui->leSoftpotsBeam2BmickResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2BmickResrch);
        return;
    }
    beam->bmick[0] = bmick_ret;
    disp.sprintf("%.1f",bmick);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2BmickResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2StcckResrch_returnPressed()
{
    QString disp;
    float stcck;
    UINT32 stcck_ret;
    float old_stcck;
    bool suc;
    stcck = ui->leSoftpotsBeam2StcckResrch->text().toFloat();
    suc = SlaveBase::CheckValue(stcck, STCCK_RSCH_CK, stcck_ret);
    if(!suc)
    {
        old_stcck = SlaveBase::ConvertMemberToFloat(beam->stcck[0], STCCK_RSCH_CK);
        disp.sprintf("%0.1f", old_stcck);
        ui->leSoftpotsBeam2StcckResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2StcckResrch);
        return;
    }
    beam->stcck[0] = stcck_ret;
    disp.sprintf("%.1f",stcck);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2StcckResrch);
    return;
}

void AccUi::on_leSoftpotsBeam2LenckResrch_returnPressed()
{
    QString disp;
    float lenck;
    UINT32 lenck_ret;
    float old_lenck;
    bool suc;
    lenck = ui->leSoftpotsBeam2LenckResrch->text().toFloat();
    suc = SlaveBase::CheckValue(lenck, LENCK_RSCH_CK, lenck_ret);
    if(!suc)
    {
        old_lenck = SlaveBase::ConvertMemberToFloat(beam->lenck[0], LENCK_RSCH_CK);
        disp.sprintf("%0.1f", old_lenck);
        ui->leSoftpotsBeam2LenckResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsBeam2LenckResrch);

        return;
    }
    beam->lenck[0] = lenck_ret;
    disp.sprintf("%.1f",lenck);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsBeam2LenckResrch);
    return;
}

void AccUi::on_leSoftpotsTrigModtResrch_returnPressed()
{
    QString disp;
    float modt;
    UINT32 modt_ret;
    float old_modt;
    bool suc;
    modt = ui->leSoftpotsTrigModtResrch->text().toFloat();
    suc = SlaveBase::CheckValue(modt, MODT_RSCH_CK, modt_ret);
    if(!suc)
    {
        old_modt = SlaveBase::ConvertMemberToFloat(io->trig[0].modulatorTrig, MODT_RSCH_CK);
        disp.sprintf("%0.1f", old_modt);
        ui->leSoftpotsTrigModtResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsTrigModtResrch);

        return;
    }
    io->trig[0].modulatorTrig = modt_ret;
    disp.sprintf("%.1f",modt);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsTrigModtResrch);
    return;
}

void AccUi::on_leSoftpotsTrigInjtResrch_returnPressed()
{
    QString disp;
    float injt;
    UINT32 injt_ret;
    float old_injt;
    bool suc;
    injt = ui->leSoftpotsTrigInjtResrch->text().toFloat();
    suc = SlaveBase::CheckValue(injt, INJT_RSCH_CK, injt_ret);
    if(!suc)
    {
        old_injt = SlaveBase::ConvertMemberToFloat(io->trig[0].injectorTrig, INJT_RSCH_CK);
        disp.sprintf("%0.1f", old_injt);
        ui->leSoftpotsTrigInjtResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsTrigInjtResrch);

        return;
    }
    io->trig[0].injectorTrig = injt_ret;
    disp.sprintf("%.1f",injt);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsTrigInjtResrch);
    return;
}

void AccUi::on_leSoftpotsTrigAfctResrch_returnPressed()
{
    QString disp;
    float afct;
    UINT32 afct_ret;
    float old_afct;
    bool suc;
    afct = ui->leSoftpotsTrigAfctResrch->text().toFloat();
    suc = SlaveBase::CheckValue(afct, AFCT_RSCH_CK, afct_ret);
    if(!suc)
    {
        old_afct = SlaveBase::ConvertMemberToFloat(io->trig[0].afcTrig, AFCT_RSCH_CK);
        disp.sprintf("%0.1f", old_afct);
        ui->leSoftpotsTrigAfctResrch->setText(disp);
        SetSoftpotFinishEdit( ui->leSoftpotsTrigAfctResrch);

        return;
    }
    io->trig[0].afcTrig = afct_ret;
    disp.sprintf("%.1f",afct);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit( ui->leSoftpotsTrigAfctResrch);
    return;
}

void AccUi::on_leSoftpotsTrigRftResrch_returnPressed()
{
    QString disp;
    float rft;
    UINT32 rft_ret;
    float old_rft;
    bool suc;
    rft = ui->leSoftpotsTrigRftResrch->text().toFloat();
    suc = SlaveBase::CheckValue(rft, RFT_RSCH_CK, rft_ret);
    if(!suc)
    {
        old_rft = SlaveBase::ConvertMemberToFloat(io->trig[0].rfTrig, RFT_RSCH_CK);
        disp.sprintf("%0.1f", old_rft);
        ui->leSoftpotsTrigRftResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsTrigRftResrch);

        return;
    }
    io->trig[0].rfTrig = rft_ret;
    disp.sprintf("%.1f",rft);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsTrigRftResrch);
    return;
}

void AccUi::on_leSoftpotsTrigTst0Resrch_returnPressed()
{
    QString disp;
    float tst0;
    UINT32 tst0_ret;
    float old_tst0;
    bool suc;
    tst0 = ui->leSoftpotsTrigTst0Resrch->text().toFloat();
    suc = SlaveBase::CheckValue(tst0, TST0_RSCH_CK, tst0_ret);
    if(!suc)
    {
        old_tst0 = SlaveBase::ConvertMemberToFloat(io->trig[0].tst0, TST0_RSCH_CK);
        disp.sprintf("%0.1f", old_tst0);
        ui->leSoftpotsTrigTst0Resrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsTrigTst0Resrch);

        return;
    }
    io->trig[0].tst0 = tst0_ret;
    disp.sprintf("%.1f",tst0);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsTrigTst0Resrch);
    return;
}

void AccUi::on_leSoftpotsTrigTst1Resrch_returnPressed()
{
    QString disp;
    float tst1;
    UINT32 tst1_ret;
    float old_tst1;
    bool suc;
    tst1 = ui->leSoftpotsTrigTst1Resrch->text().toFloat();
    suc = SlaveBase::CheckValue(tst1, TST1_RSCH_CK, tst1_ret);
    if(!suc)
    {
        old_tst1 = SlaveBase::ConvertMemberToFloat(io->trig[0].tst1, TST1_RSCH_CK);
        disp.sprintf("%0.1f", old_tst1);
        ui->leSoftpotsTrigTst1Resrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsTrigTst1Resrch);

        return;
    }
    io->trig[0].tst1 = tst1_ret;
    disp.sprintf("%.1f",tst1);
    ui->leSoftpotsBeam2ElmckResrch->setText(disp);
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsTrigTst1Resrch);
    return;
}

void AccUi::on_leSoftpotsDose1D1gResrch_returnPressed()
{
    QString disp;
    UINT16 d1g;
    UINT32 d1g_ret;
    UINT32 old_d1g;
    bool suc;

    d1g = ui->leSoftpotsDose1D1gResrch->text().toInt();
    suc = SlaveBase::CheckValue(d1g, DOSE1_GAIN_CK, d1g_ret);
    if(!suc)
    {
        old_d1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[0].dose1_gain, DOSE1_GAIN_CK);
        disp.sprintf("%d", old_d1g);
        ui->leSoftpotsDose1D1gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose1D1gResrch);

        return;
    }
    dosei->dose1_gain[0].dose1_gain = d1g_ret;
    ui->leSoftpotsDose1D1gResrch->setText(QString::number(d1g));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose1D1gResrch);
}

void AccUi::on_leSoftpotsDose1D2gResrch_returnPressed()
{
    QString disp;
    UINT16 d2g;
    UINT32 d2g_ret;
    UINT32 old_d2g;
    bool suc;

    d2g = ui->leSoftpotsDose1D2gResrch->text().toInt();
    suc = SlaveBase::CheckValue(d2g, DOSE2_GAIN_CK, d2g_ret);
    if(!suc)
    {
        old_d2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_gain[0].gain, DOSE2_GAIN_CK);
        disp.sprintf("%d", old_d2g);
        ui->leSoftpotsDose1D2gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose1D2gResrch);

        return;
    }
    doseii->dose2_gain[0].gain = d2g_ret;
    ui->leSoftpotsDose1D2gResrch->setText(QString::number(d2g));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose1D2gResrch);
}

void AccUi::on_leSoftpotsDose1PrfpResrch_returnPressed()
{
    QString disp;
    UINT16 prfp;
    UINT32 prfp_ret;
    UINT32 old_prfp;
    bool suc;

    prfp = ui->leSoftpotsDose1PrfpResrch->text().toInt();
    suc = SlaveBase::CheckValue(prfp, PRFP_CK, prfp_ret);
    if(!suc)
    {
        old_prfp = (UINT16)SlaveBase::ConvertMemberToFloat(beam->prfp[0], PRFP_CK);
        disp.sprintf("%d", old_prfp);
        ui->leSoftpotsDose1PrfpResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose1PrfpResrch);

        return;
    }
    beam->prfp[0] = prfp_ret;
    ui->leSoftpotsDose1PrfpResrch->setText(QString::number(prfp));
    suc = beam->SendPfnPrfpIpfnSomin();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose1PrfpResrch);
}

void AccUi::on_leSoftpotsDose1DrcvResrch_returnPressed()
{
    QString disp;
    UINT16 drcv;
    UINT32 drcv_ret;
    UINT32 old_drcv;
    bool suc;
    bool suc1;

    drcv = ui->leSoftpotsDose1DrcvResrch->text().toInt();
    suc = SlaveBase::CheckValue(drcv, DRCV_CK, drcv_ret);
    if(!suc)
    {
        old_drcv = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[0].drcv, DRCV_CK);
        disp.sprintf("%d", old_drcv);
        ui->leSoftpotsDose1DrcvResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose1DrcvResrch);

        return;
    }
    dosei->dose1_gain[0].drcv = drcv_ret;
    doseii->dose2_gain[0].drcv = drcv_ret;
    ui->leSoftpotsDose1DrcvResrch->setText(QString::number(drcv));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    suc1 = doseii->SendGainAndWorkMode();
    suc1 &= doseii->SendSegmentGain();
    if(!(suc && suc1))
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose1DrcvResrch);
}

void AccUi::on_leSoftpotsDose2S0gResrch_returnPressed()
{
    QString disp;
    UINT16 s0g;
    UINT32 s0g_ret;
    UINT32 old_s0g;
    bool suc;

    s0g = ui->leSoftpotsDose2S0gResrch->text().toInt();
    suc = SlaveBase::CheckValue(s0g, S0_GAIN_CK, s0g_ret);
    if(!suc)
    {
        old_s0g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[0].segment0Gain, S0_GAIN_CK);
        disp.sprintf("%d", old_s0g);
        ui->leSoftpotsDose2S0gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose2S0gResrch);

        return;
    }
    doseii->dose2_segment_gain[0].segment0Gain = s0g_ret;
    ui->leSoftpotsDose2S0gResrch->setText(QString::number(s0g));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose2S0gResrch);
}

void AccUi::on_leSoftpotsDose2S1gResrch_returnPressed()
{
    QString disp;
    UINT16 s1g;
    UINT32 s1g_ret;
    UINT32 old_s1g;
    bool suc;

    s1g = ui->leSoftpotsDose2S1gResrch->text().toInt();
    suc = SlaveBase::CheckValue(s1g, S1_GAIN_CK, s1g_ret);
    if(!suc)
    {
        old_s1g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[0].segment1_gain, S1_GAIN_CK);
        disp.sprintf("%d", old_s1g);
        ui->leSoftpotsDose2S1gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose2S1gResrch);

        return;
    }
    dosei->dose1_gain[0].segment1_gain = s1g_ret;
    ui->leSoftpotsDose2S1gResrch->setText(QString::number(s1g));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose2S1gResrch);
}

void AccUi::on_leSoftpotsDose2S2gResrch_returnPressed()
{
    QString disp;
    UINT16 s2g;
    UINT32 s2g_ret;
    UINT32 old_s2g;
    bool suc;

    s2g = ui->leSoftpotsDose2S2gResrch->text().toInt();
    suc = SlaveBase::CheckValue(s2g, S2_GAIN_CK, s2g_ret);
    if(!suc)
    {
        old_s2g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[0].segment2Gain, S2_GAIN_CK);
        disp.sprintf("%d", old_s2g);
        ui->leSoftpotsDose2S2gResrch->setText(disp);
        SetSoftpotFinishEdit( ui->leSoftpotsDose2S2gResrch);

        return;
    }
    doseii->dose2_segment_gain[0].segment2Gain = s2g_ret;
    ui->leSoftpotsDose2S2gResrch->setText(QString::number(s2g));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }

    SetSoftpotFinishEdit( ui->leSoftpotsDose2S2gResrch);
}

void AccUi::on_leSoftpotsDose2S3gResrch_returnPressed()
{
    QString disp;
    UINT16 s3g;
    UINT32 s3g_ret;
    UINT32 old_s3g;
    bool suc;

    s3g = ui->leSoftpotsDose2S3gResrch->text().toInt();
    suc = SlaveBase::CheckValue(s3g, S3_GAIN_CK, s3g_ret);
    if(!suc)
    {
        old_s3g = (UINT16)SlaveBase::ConvertMemberToFloat(dosei->dose1_gain[0].segment3_gain, S3_GAIN_CK);
        disp.sprintf("%d", old_s3g);
        ui->leSoftpotsDose2S3gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose2S3gResrch);

        return;
    }
    dosei->dose1_gain[0].segment3_gain = s3g_ret;
    ui->leSoftpotsDose2S3gResrch->setText(QString::number(s3g));
    suc = dosei->SendDynamicSteer();
    suc &= dosei->SendGainAndWorkMode();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose2S3gResrch);
}

void AccUi::on_leSoftpotsDose2S4gResrch_returnPressed()
{
    QString disp;
    UINT16 s4g;
    UINT32 s4g_ret;
    UINT32 old_s4g;
    bool suc;

    s4g = ui->leSoftpotsDose2S4gResrch->text().toInt();
    suc = SlaveBase::CheckValue(s4g, S4_GAIN_CK, s4g_ret);
    if(!suc)
    {
        old_s4g = (UINT16)SlaveBase::ConvertMemberToFloat(doseii->dose2_segment_gain[0].segment4Gain, S4_GAIN_CK);
        disp.sprintf("%d", old_s4g);
        ui->leSoftpotsDose2S4gResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose2S4gResrch);

        return;
    }
    doseii->dose2_segment_gain[0].segment4Gain = s4g_ret;
    ui->leSoftpotsDose2S4gResrch->setText(QString::number(s4g));
    suc = doseii->SendGainAndWorkMode();
    suc &= doseii->SendSegmentGain();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose2S4gResrch);
}
//not found
void AccUi::on_leSoftpotsDose2SwiResrch_returnPressed()
{
    QString disp;
    float swi;
    swi = ui->leSoftpotsDose2SwiResrch->text().toFloat();
    disp.sprintf("%.1f",swi);
    ui->leSoftpotsDose2SwiResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsDose2SwiResrch);
}
//not found
void AccUi::on_leSoftpotsDose2FwiResrch_returnPressed()
{
    QString disp;
    float fwi;
    fwi = ui->leSoftpotsDose2FwiResrch->text().toFloat();
    disp.sprintf("%.1f",fwi);
    ui->leSoftpotsDose2FwiResrch->setText(disp);
    SetSoftpotFinishEdit(ui->leSoftpotsDose2FwiResrch);
}

void AccUi::on_leSoftpotsDose2DostResrch_returnPressed()
{
    QString disp;
    UINT16 dost;
    UINT32 dost_ret;
    UINT32 old_dost;
    bool suc;

    dost = ui->leSoftpotsDose2DostResrch->text().toInt();
    suc = SlaveBase::CheckValue(dost, DOS_T_CK, dost_ret);
    if(!suc)
    {
        old_dost = (UINT16)SlaveBase::ConvertMemberToFloat(io->trig[0].doseTrig, DOS_T_CK);
        disp.sprintf("%d", old_dost);
        ui->leSoftpotsDose2DostResrch->setText(disp);
        SetSoftpotFinishEdit(ui->leSoftpotsDose2DostResrch);

        return;
    }
    io->trig[0].doseTrig = dost_ret;
    ui->leSoftpotsDose2DostResrch->setText(QString::number(dost));
    suc = io->SendTrigger();
    if(!suc)
    {
        //goto error;
        return;
    }
    SetSoftpotFinishEdit(ui->leSoftpotsDose2DostResrch);
}
