#include "accui.h"
#include "ui_accui.h"

void AccUi::CaculateBypassMask()
{
    UINT16 tmp;

    tmp = BYPASS_NO1;
    if(g_interlocks[2].ovrd)
        tmp += BYPASS_TREAT_TIME1;
    if(g_interlocks[12].ovrd)
        tmp += BYPASS_DOSE_RATE1;
    if(g_interlocks[56].ovrd)
        tmp += BYPASS_DOSE1;
    if(g_interlocks[57].ovrd)
        tmp += BYPASS_DOSE_RATE_OFFSET1;
    if(g_interlocks[58].ovrd)
        tmp += BYPASS_DOSE_SATURATION1;
    if(g_interlocks[59].ovrd)
        tmp += BYPASS_PT_SATURATION1;
    if(g_interlocks[82].ovrd)
        tmp += BYPASS_DOSE_PER_DEGREE1;
    if(g_interlocks[93].ovrd)
        tmp += BYPASS_CAP1;
    if(g_interlocks[97].ovrd)
        tmp += BYPASS_DOSE_CALC1;

    dosei->dose1_by_pass_mask = tmp;

    tmp = BYPASS_NO2;
    if(g_interlocks[1].ovrd)
        tmp += BYPASS_DOSE2;
    if(g_interlocks[2].ovrd)
        tmp += BYPASS_TREAT_TIME2;
    if(g_interlocks[13].ovrd)
        tmp += BYPASS_DOSE_RATE2;
    if(g_interlocks[59].ovrd)
        tmp += BYPASS_PT_SATURATION2;
    if(g_interlocks[60].ovrd)
        tmp += BYPASS_DOSE_RATE_OFFSET2;
    if(g_interlocks[61].ovrd)
        tmp += BYPASS_DOSE_SATURATION2;
    if(g_interlocks[97].ovrd)
        tmp += BYPASS_DOSE_CALC2;

    doseii->dose2_by_pass_mask = tmp;
}

//E compensation factor
void AccUi::on_leCpsFactor_returnPressed()
{
    float cps_factor;
    cps_factor = ui->leCpsFactor->text().toFloat();
    DoseBase::compen_factor = cps_factor;
    return;
}

void AccUi::on_leCpsFactorRefTemp_returnPressed()
{
    QString disp;
    float ref_temp;
    //bool suc;
    ref_temp = ui->leCpsFactorRefTemp->text().toFloat();
    DoseBase::ref_temp = ref_temp;
    disp.sprintf("%.1f",ref_temp);
    ui->leCpsFactorRefTemp->setText(disp);
    SetSvcFinishEdit(ui->leCpsFactorRefTemp);
}

void AccUi::on_leCpsFactorRefPress_returnPressed()
{
    QString disp;
    float ref_press;
    //bool suc;
    ref_press = ui->leCpsFactorRefPress->text().toFloat();
    disp.sprintf("%.1f",ref_press);
    ui->leCpsFactorRefPress->setText(disp);
    SetSvcFinishEdit(ui->leCpsFactorRefPress);
    DoseBase::ref_press = ref_press;
}

//F

//G  Mean Attenuation 平均衰减
void AccUi::on_leMeanAttLoCalibrFact_returnPressed()
{
    QString disp;
    float loCalibrFact;
    loCalibrFact = ui->leMeanAttLoCalibrFact->text().toFloat();
    disp.sprintf("%.5f",loCalibrFact);
    ui->leMeanAttLoCalibrFact->setText(disp);
    DoseBase::x_lo_cf = loCalibrFact;
    SetSvcFinishEdit(ui->leMeanAttLoCalibrFact);
}

void AccUi::on_leMeanAttHiCalibrFact_returnPressed()
{
    QString disp;
    float hiCalibrFact;
    hiCalibrFact = ui->leMeanAttHiCalibrFact->text().toFloat();
    disp.sprintf("%.5f",hiCalibrFact);
    ui->leMeanAttHiCalibrFact->setText(disp);
    DoseBase::x_hi_cf = hiCalibrFact;
    SetSvcFinishEdit(ui->leMeanAttHiCalibrFact);
}

//I  Motor Adjustment 电机调节菜单
void AccUi::on_leMotAdjJawY1SDS_returnPressed()
{
    QString disp;
    UINT16 jawy1_sds;
    UINT32 jawy1_sds_ret;
    UINT16 old_jawy1_sds;
    bool suc;
    jawy1_sds = ui->leMotAdjJawY1SDS->text().toInt();
    suc = SlaveBase::CheckValue(jawy1_sds, JAWY1_SDS_RSCH_CK, jawy1_sds_ret);
    if(!suc)
    {
        old_jawy1_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_A1], JAWY1_SDS_RSCH_CK);
        disp.sprintf("%d", old_jawy1_sds);
        ui->leMotAdjJawY1SDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY1SDS);
        return;
    }

    motor->slow_down_speed[JAW_A1] = jawy1_sds_ret;
    ui->leMotAdjJawY1SDS->setText(QString::number(jawy1_sds));
    SetSvcFinishEdit(ui->leMotAdjJawY1SDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::on_leMotAdjJawY2SDS_returnPressed()
{
    QString disp;
    UINT16 jawy2_sds;
    UINT32 jawy2_sds_ret;
    UINT16 old_jawy2_sds;
    bool suc;
    jawy2_sds = ui->leMotAdjJawY2SDS->text().toInt();
    suc = SlaveBase::CheckValue(jawy2_sds, JAWY2_SDS_RSCH_CK, jawy2_sds_ret);
    if(!suc)
    {
        old_jawy2_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_A2], JAWY2_SDS_RSCH_CK);
        disp.sprintf("%d", old_jawy2_sds);
        ui->leMotAdjJawY2SDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY2SDS);
        return;
    }

    motor->slow_down_speed[JAW_A2] = jawy2_sds_ret;
    ui->leMotAdjJawY2SDS->setText(QString::number(jawy2_sds));
    SetSvcFinishEdit(ui->leMotAdjJawY2SDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        return;
    }

    return;
}

void AccUi::on_leMotAdjJawX2SDS_returnPressed()
{
    QString disp;
    UINT16 jawx2_sds;
    UINT32 jawx2_sds_ret;
    UINT16 old_jawx2_sds;
    bool suc;
    jawx2_sds = ui->leMotAdjJawX2SDS->text().toInt();
    suc = SlaveBase::CheckValue(jawx2_sds, JAWX2_SDS_RSCH_CK, jawx2_sds_ret);
    if(!suc)
    {
        old_jawx2_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_B1], JAWX2_SDS_RSCH_CK);
        disp.sprintf("%d", old_jawx2_sds);
        ui->leMotAdjJawX2SDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX2SDS);
        return;
    }

    motor->slow_down_speed[JAW_B1] = jawx2_sds_ret;
    ui->leMotAdjJawX2SDS->setText(QString::number(jawx2_sds));
    SetSvcFinishEdit(ui->leMotAdjJawX2SDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjJawX1SDS_returnPressed()
{
    QString disp;
    UINT16 jawx1_sds;
    UINT32 jawx1_sds_ret;
    UINT16 old_jawx1_sds;
    bool suc;
    jawx1_sds = ui->leMotAdjJawX1SDS->text().toInt();
    suc = SlaveBase::CheckValue(jawx1_sds, JAWX1_SDS_RSCH_CK, jawx1_sds_ret);
    if(!suc)
    {
        old_jawx1_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_B2], JAWX1_SDS_RSCH_CK);
        disp.sprintf("%d", old_jawx1_sds);
        ui->leMotAdjJawX1SDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX1SDS);
        return;
    }

    motor->slow_down_speed[JAW_B2] = jawx1_sds_ret;
    ui->leMotAdjJawX1SDS->setText(QString::number(jawx1_sds));
    SetSvcFinishEdit(ui->leMotAdjJawX1SDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        return;
    }

    return;
}

void AccUi::on_leMotAdjColliSDS_returnPressed()
{
    QString disp;
    UINT16 colli_sds;
    UINT32 colli_sds_ret;
    UINT16 old_colli_sds;
    bool suc;
    colli_sds = ui->leMotAdjColliSDS->text().toInt();
    suc = SlaveBase::CheckValue(colli_sds, COLLI_SDS_RSCH_CK, colli_sds_ret);
    if(!suc)
    {
        old_colli_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[COLLIMATOR], COLLI_SDS_RSCH_CK);
        disp.sprintf("%d", old_colli_sds);
        ui->leMotAdjColliSDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjColliSDS);
        return;
    }

    motor->slow_down_speed[COLLIMATOR] = colli_sds_ret;
    ui->leMotAdjColliSDS->setText(QString::number(colli_sds));
    SetSvcFinishEdit(ui->leMotAdjColliSDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjGantrySDS_returnPressed()
{
    QString disp;
    UINT16 gantry_sds;
    UINT32 gantry_sds_ret;
    UINT16 old_gantry_sds;
    bool suc;
    gantry_sds = ui->leMotAdjGantrySDS->text().toInt();
    suc = SlaveBase::CheckValue(gantry_sds, GANTRY_SDS_RSCH_CK, gantry_sds_ret);
    if(!suc)
    {
        old_gantry_sds = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[GANTRY], GANTRY_SDS_RSCH_CK);
        disp.sprintf("%d", old_gantry_sds);
        ui->leMotAdjGantrySDS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjGantrySDS);
        return;
    }

    motor->slow_down_speed[GANTRY] = gantry_sds_ret;
    ui->leMotAdjGantrySDS->setText(QString::number(gantry_sds));
    SetSvcFinishEdit(ui->leMotAdjGantrySDS);

    suc = motor->SendSlowDownSpeed();
    if(!suc)
    {
        return;
    }
    return;
}


void AccUi::on_leMotAdjJawY1SDD_returnPressed()
{
    QString disp;
    UINT16 jawy1_sdd;
    UINT32 jawy1_sdd_ret;
    UINT16 old_jawy1_sdd;
    bool suc;
    jawy1_sdd = ui->leMotAdjJawY1SDD->text().toInt();
    suc = SlaveBase::CheckValue(jawy1_sdd, JAWY1_SDD_RSCH_CK, jawy1_sdd_ret);
    if(!suc)
    {
        old_jawy1_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[JAW_A1], JAWY1_SDD_RSCH_CK);
        disp.sprintf("%d", old_jawy1_sdd);
        ui->leMotAdjJawY1SDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY1SDD);

        return;
    }

    motor->slow_down_distance[JAW_A1] = jawy1_sdd_ret;
    ui->leMotAdjJawY1SDD->setText(QString::number(jawy1_sdd));
    SetSvcFinishEdit(ui->leMotAdjJawY1SDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjJawY2SDD_returnPressed()
{
    QString disp;
    UINT16 jawy2_sdd;
    UINT32 jawy2_sdd_ret;
    UINT16 old_jawy2_sdd;
    bool suc;
    jawy2_sdd = ui->leMotAdjJawY2SDD->text().toInt();
    suc = SlaveBase::CheckValue(jawy2_sdd, JAWY2_SDD_RSCH_CK, jawy2_sdd_ret);
    if(!suc)
    {
        old_jawy2_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[JAW_A2], JAWY2_SDD_RSCH_CK);
        disp.sprintf("%d", old_jawy2_sdd);
        ui->leMotAdjJawY2SDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY2SDD);
        return;
    }

    motor->slow_down_distance[JAW_A2] = jawy2_sdd_ret;
    ui->leMotAdjJawY2SDD->setText(QString::number(jawy2_sdd));
    SetSvcFinishEdit(ui->leMotAdjJawY2SDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjJawX2SDD_returnPressed()
{
    QString disp;
    UINT16 jawx2_sdd;
    UINT32 jawx2_sdd_ret;
    UINT16 old_jawx2_sdd;
    bool suc;
    jawx2_sdd = ui->leMotAdjJawX2SDD->text().toInt();
    suc = SlaveBase::CheckValue(jawx2_sdd, JAWX2_SDD_RSCH_CK, jawx2_sdd_ret);
    if(!suc)
    {
        old_jawx2_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[JAW_B1], JAWX2_SDD_RSCH_CK);
        disp.sprintf("%d", old_jawx2_sdd);
        ui->leMotAdjJawX2SDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX2SDD);
        return;
    }

    motor->slow_down_distance[JAW_B1] = jawx2_sdd_ret;
    ui->leMotAdjJawX2SDD->setText(QString::number(jawx2_sdd));
    SetSvcFinishEdit(ui->leMotAdjJawX2SDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjJawX1SDD_returnPressed()
{
    QString disp;
    UINT16 jawx1_sdd;
    UINT32 jawx1_sdd_ret;
    UINT16 old_jawx1_sdd;
    bool suc;
    jawx1_sdd = ui->leMotAdjJawX1SDD->text().toInt();
    suc = SlaveBase::CheckValue(jawx1_sdd, JAWX1_SDD_RSCH_CK, jawx1_sdd_ret);
    if(!suc)
    {
        old_jawx1_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[JAW_B2], JAWX1_SDD_RSCH_CK);
        disp.sprintf("%d", old_jawx1_sdd);
        ui->leMotAdjJawX1SDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX1SDD);

        return;
    }

    motor->slow_down_distance[JAW_B2] = jawx1_sdd_ret;
    ui->leMotAdjJawX1SDD->setText(QString::number(jawx1_sdd));
    SetSvcFinishEdit(ui->leMotAdjJawX1SDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjColliSDD_returnPressed()
{
    QString disp;
    UINT16 colli_sdd;
    UINT32 colli_sdd_ret;
    UINT16 old_colli_sdd;
    bool suc;
    colli_sdd = ui->leMotAdjColliSDD->text().toInt();
    suc = SlaveBase::CheckValue(colli_sdd, COLLI_SDD_RSCH_CK, colli_sdd_ret);
    if(!suc)
    {
        old_colli_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[COLLIMATOR], COLLI_SDD_RSCH_CK);
        disp.sprintf("%d", old_colli_sdd);
        ui->leMotAdjColliSDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjColliSDD);
        return;
    }

    motor->slow_down_distance[COLLIMATOR] = colli_sdd_ret;
    ui->leMotAdjColliSDD->setText(QString::number(colli_sdd));
    SetSvcFinishEdit(ui->leMotAdjColliSDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}

void AccUi::on_leMotAdjGantrySDD_returnPressed()
{
    QString disp;
    UINT16 gantry_sdd;
    UINT32 gantry_sdd_ret;
    UINT16 old_gantry_sdd;
    bool suc;
    gantry_sdd = ui->leMotAdjGantrySDD->text().toInt();
    suc = SlaveBase::CheckValue(gantry_sdd, GANTRY_SDD_RSCH_CK, gantry_sdd_ret);
    if(!suc)
    {
        old_gantry_sdd = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_down_distance[GANTRY], GANTRY_SDD_RSCH_CK);
        disp.sprintf("%d", old_gantry_sdd);
        ui->leMotAdjGantrySDD->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjGantrySDD);
        return;
    }

    motor->slow_down_distance[GANTRY] = gantry_sdd_ret;
    ui->leMotAdjGantrySDD->setText(QString::number(gantry_sdd));
    SetSvcFinishEdit(ui->leMotAdjGantrySDD);

    suc = motor->SendSlowDownDistance();
    if(!suc)
    {
        return;
    }
    return;
}


void AccUi::on_leMotAdjJawY1SS_returnPressed()
{
    QString disp;
    UINT16 jawy1_ss;
    UINT32 jawy1_ss_ret;
    UINT16 old_jawy1_ss;
    bool suc;
    jawy1_ss = ui->leMotAdjJawY1SS->text().toInt();
    suc = SlaveBase::CheckValue(jawy1_ss, JAWY1_SS_RSCH_CK, jawy1_ss_ret);
    if(!suc)
    {
        old_jawy1_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[JAW_A1], JAWY1_SS_RSCH_CK);
        disp.sprintf("%d", old_jawy1_ss);
        ui->leMotAdjJawY1SS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY1SS);
        return;
    }
    motor->slow_speed[JAW_A1] = jawy1_ss_ret;
    ui->leMotAdjJawY1SS->setText(QString::number(jawy1_ss));
    SetSvcFinishEdit(ui->leMotAdjJawY1SS);
    return;
}

void AccUi::on_leMotAdjJawY2SS_returnPressed()
{
    QString disp;
    UINT16 jawy2_ss;
    UINT32 jawy2_ss_ret;
    UINT16 old_jawy2_ss;
    bool suc;
    jawy2_ss = ui->leMotAdjJawY2SS->text().toInt();
    suc = SlaveBase::CheckValue(jawy2_ss, JAWY2_SS_RSCH_CK, jawy2_ss_ret);
    if(!suc)
    {
        old_jawy2_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[JAW_A2], JAWY2_SS_RSCH_CK);
        disp.sprintf("%d", old_jawy2_ss);
        ui->leMotAdjJawY2SS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY2SS);
        return;
    }
    motor->slow_speed[JAW_A2] = jawy2_ss_ret;
    ui->leMotAdjJawY2SS->setText(QString::number(jawy2_ss));
    SetSvcFinishEdit(ui->leMotAdjJawY2SS);
    return;
}

void AccUi::on_leMotAdjJawX2SS_returnPressed()
{
    QString disp;
    UINT16 jawx2_ss;
    UINT32 jawx2_ss_ret;
    UINT16 old_jawx2_ss;
    bool suc;
    jawx2_ss = ui->leMotAdjJawX2SS->text().toInt();
    suc = SlaveBase::CheckValue(jawx2_ss, JAWX2_SS_RSCH_CK, jawx2_ss_ret);
    if(!suc)
    {
        old_jawx2_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[JAW_B1], JAWX2_SS_RSCH_CK);
        disp.sprintf("%d", old_jawx2_ss);
        ui->leMotAdjJawX2SS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX2SS);
        return;
    }
    motor->slow_speed[JAW_B1] = jawx2_ss_ret;
    ui->leMotAdjJawX2SS->setText(QString::number(jawx2_ss));
    SetSvcFinishEdit(ui->leMotAdjJawX2SS);
    return;
}

void AccUi::on_leMotAdjJawX1SS_returnPressed()
{
    QString disp;
    UINT16 jawx1_ss;
    UINT32 jawx1_ss_ret;
    UINT16 old_jawx1_ss;
    bool suc;
    jawx1_ss = ui->leMotAdjJawX1SS->text().toInt();
    suc = SlaveBase::CheckValue(jawx1_ss, JAWX1_SS_RSCH_CK, jawx1_ss_ret);
    if(!suc)
    {
        old_jawx1_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[JAW_B2], JAWX1_SS_RSCH_CK);
        disp.sprintf("%d", old_jawx1_ss);
        ui->leMotAdjJawX1SS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX1SS);
        return;
    }
    motor->slow_speed[JAW_B2] = jawx1_ss_ret;
    ui->leMotAdjJawX1SS->setText(QString::number(jawx1_ss));
    SetSvcFinishEdit(ui->leMotAdjJawX1SS);
    return;
}

void AccUi::on_leMotAdjColliSS_returnPressed()
{
    QString disp;
    UINT16 colli_ss;
    UINT32 colli_ss_ret;
    UINT16 old_colli_ss;
    bool suc;
    colli_ss = ui->leMotAdjColliSS->text().toInt();
    suc = SlaveBase::CheckValue(colli_ss, COLLI_SS_RSCH_CK, colli_ss_ret);
    if(!suc)
    {
        old_colli_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[COLLIMATOR], COLLI_SS_RSCH_CK);
        disp.sprintf("%d", old_colli_ss);
        ui->leMotAdjColliSS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjColliSS);
        return;
    }
    motor->slow_speed[COLLIMATOR] = colli_ss_ret;
    ui->leMotAdjColliSS->setText(QString::number(colli_ss));
    SetSvcFinishEdit(ui->leMotAdjColliSS);
    return;
}

void AccUi::on_leMotAdjGantrySS_returnPressed()
{
    QString disp;
    UINT16 gantry_ss;
    UINT32 gantry_ss_ret;
    UINT16 old_gantry_ss;
    bool suc;
    gantry_ss = ui->leMotAdjGantrySS->text().toInt();
    suc = SlaveBase::CheckValue(gantry_ss, GANTRY_SS_RSCH_CK, gantry_ss_ret);
    if(!suc)
    {
        old_gantry_ss = (UINT16)SlaveBase::ConvertMemberToFloat(motor->slow_speed[GANTRY], GANTRY_SS_RSCH_CK);
        disp.sprintf("%d", old_gantry_ss);
        ui->leMotAdjGantrySS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjGantrySS);
        return;
    }
    motor->slow_speed[GANTRY] = gantry_ss_ret;
    ui->leMotAdjGantrySS->setText(QString::number(gantry_ss));
    SetSvcFinishEdit(ui->leMotAdjGantrySS);
    return;
}

void AccUi::on_leMotAdjJawY1FS_returnPressed()
{
    QString disp;
    UINT16 jawy1_fs;
    UINT32 jawy1_fs_ret;
    UINT16 old_jawy1_fs;
    bool suc;
    jawy1_fs = ui->leMotAdjJawY1FS->text().toInt();
    suc = SlaveBase::CheckValue(jawy1_fs, JAWY1_FS_RSCH_CK, jawy1_fs_ret);
    if(!suc)
    {
        old_jawy1_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[JAW_A1], JAWY1_FS_RSCH_CK);
        disp.sprintf("%d", old_jawy1_fs);
        ui->leMotAdjJawY1FS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY1FS);
        return;
    }
    motor->fast_speed[JAW_A1] = jawy1_fs_ret;
    ui->leMotAdjJawY1FS->setText(QString::number(jawy1_fs));
    SetSvcFinishEdit(ui->leMotAdjJawY1FS);
    return;
}

void AccUi::on_leMotAdjJawY2FS_returnPressed()
{
    QString disp;
    UINT16 jawy2_fs;
    UINT32 jawy2_fs_ret;
    UINT16 old_jawy2_fs;
    bool suc;
    jawy2_fs = ui->leMotAdjJawY2FS->text().toInt();
    suc = SlaveBase::CheckValue(jawy2_fs, JAWY2_FS_RSCH_CK, jawy2_fs_ret);
    if(!suc)
    {
        old_jawy2_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[JAW_A2], JAWY2_FS_RSCH_CK);
        disp.sprintf("%d", old_jawy2_fs);
        ui->leMotAdjJawY2FS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawY2FS);
        return;
    }

    motor->fast_speed[JAW_A2] = jawy2_fs_ret;
    ui->leMotAdjJawY2FS->setText(QString::number(jawy2_fs));
    SetSvcFinishEdit(ui->leMotAdjJawY2FS);
    return;
}

void AccUi::on_leMotAdjJawX2FS_returnPressed()
{
    QString disp;
    UINT16 jawx2_fs;
    UINT32 jawx2_fs_ret;
    UINT16 old_jawx2_fs;
    bool suc;
    jawx2_fs = ui->leMotAdjJawX2FS->text().toInt();
    suc = SlaveBase::CheckValue(jawx2_fs, JAWX2_FS_RSCH_CK, jawx2_fs_ret);
    if(!suc)
    {
        old_jawx2_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[JAW_B1], JAWX2_FS_RSCH_CK);
        disp.sprintf("%d", old_jawx2_fs);
        ui->leMotAdjJawX2FS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX2FS);
        return;
    }
    motor->fast_speed[JAW_B1] = jawx2_fs_ret;
    ui->leMotAdjJawX2FS->setText(QString::number(jawx2_fs));
    SetSvcFinishEdit(ui->leMotAdjJawX2FS);
    return;
}

void AccUi::on_leMotAdjJawX1FS_returnPressed()
{
    QString disp;
    UINT16 jawx1_fs;
    UINT32 jawx1_fs_ret;
    UINT16 old_jawx1_fs;
    bool suc;
    jawx1_fs = ui->leMotAdjJawX1FS->text().toInt();
    suc = SlaveBase::CheckValue(jawx1_fs, JAWX1_FS_RSCH_CK, jawx1_fs_ret);
    if(!suc)
    {
        old_jawx1_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[JAW_B2], JAWX1_FS_RSCH_CK);
        disp.sprintf("%d", old_jawx1_fs);
        ui->leMotAdjJawX1FS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjJawX1FS);
        return;
    }
    motor->fast_speed[JAW_B2] = jawx1_fs_ret;
    ui->leMotAdjJawX1FS->setText(QString::number(jawx1_fs));
    SetSvcFinishEdit(ui->leMotAdjJawX1FS);
    return;
}

void AccUi::on_leMotAdjColliFS_returnPressed()
{
    QString disp;
    UINT16 colli_fs;
    UINT32 colli_fs_ret;
    UINT16 old_colli_fs;
    bool suc;
    colli_fs = ui->leMotAdjColliFS->text().toInt();
    suc = SlaveBase::CheckValue(colli_fs, COLLI_FS_RSCH_CK, colli_fs_ret);
    if(!suc)
    {
        old_colli_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[COLLIMATOR], COLLI_FS_RSCH_CK);
        disp.sprintf("%d", old_colli_fs);
        ui->leMotAdjColliFS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjColliFS);
        return;
    }
    motor->fast_speed[COLLIMATOR] = colli_fs_ret;
    ui->leMotAdjColliFS->setText(QString::number(colli_fs));
    SetSvcFinishEdit(ui->leMotAdjColliFS);
    return;
}

void AccUi::on_leMotAdjGantryFS_returnPressed()
{
    QString disp;
    UINT16 gantry_fs;
    UINT32 gantry_fs_ret;
    UINT16 old_gantry_fs;
    bool suc;
    gantry_fs = ui->leMotAdjGantryFS->text().toInt();
    suc = SlaveBase::CheckValue(gantry_fs, GANTRY_FS_RSCH_CK, gantry_fs_ret);
    if(!suc)
    {
        old_gantry_fs = (UINT16)SlaveBase::ConvertMemberToFloat(motor->fast_speed[GANTRY], GANTRY_FS_RSCH_CK);
        disp.sprintf("%d", old_gantry_fs);
        ui->leMotAdjGantryFS->setText(disp);
        SetSvcFinishEdit(ui->leMotAdjGantryFS);
        return;
    }
    motor->fast_speed[GANTRY] = gantry_fs_ret;
    ui->leMotAdjGantryFS->setText(QString::number(gantry_fs));
    SetSvcFinishEdit(ui->leMotAdjGantryFS);
    return;
}

//M  Foil Selection  散射箔选择 设置新值
void AccUi::on_leFoilSelectNewElecEgy1_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy1->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->lbElecEgy2NewFoil->setText("Electron energy #2:");
        ui->leFoilSelectNewElecEgy1->setReadOnly(true);
        ui->leFoilSelectNewElecEgy2->setReadOnly(false);
        ui->leFoilSelectNewElecEgy2->setFocus();
    }
    else
    {
        ui->leFoilSelectNewElecEgy1->setText("");
        ui->leFoilSelectNewElecEgy1->setFocus();
    }
}


void AccUi::on_leFoilSelectNewElecEgy2_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy2->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->lbElecEgy3NewFoil->setText("Electron energy #3:");
        ui->leFoilSelectNewElecEgy2->setReadOnly(true);
        ui->leFoilSelectNewElecEgy3->setReadOnly(false);
        ui->leFoilSelectNewElecEgy3->setFocus();
    }
    else
    {
        ui->leFoilSelectNewElecEgy2->setText("");
        ui->leFoilSelectNewElecEgy2->setFocus();
    }
}

void AccUi::on_leFoilSelectNewElecEgy3_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy3->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->lbElecEgy4NewFoil->setText("Electron energy #4:");
        ui->leFoilSelectNewElecEgy3->setReadOnly(true);
        ui->leFoilSelectNewElecEgy4->setReadOnly(false);
        ui->leFoilSelectNewElecEgy4->setFocus();
    }
    else
    {
        ui->leFoilSelectNewElecEgy3->setText("");
        ui->leFoilSelectNewElecEgy3->setFocus();
    }
}

void AccUi::on_leFoilSelectNewElecEgy4_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy4->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->lbElecEgy5NewFoil->setText("Electron energy #5:");
        ui->leFoilSelectNewElecEgy4->setReadOnly(true);
        ui->leFoilSelectNewElecEgy5->setReadOnly(false);
        ui->leFoilSelectNewElecEgy5->setFocus();
    }
    else
    {
        ui->leFoilSelectNewElecEgy4->setText("");
        ui->leFoilSelectNewElecEgy4->setFocus();
    }
}

void AccUi::on_leFoilSelectNewElecEgy5_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy5->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->lbElecEgy6NewFoil->setText("Electron energy #6:");
        ui->leFoilSelectNewElecEgy5->setReadOnly(true);
        ui->leFoilSelectNewElecEgy6->setReadOnly(false);
        ui->leFoilSelectNewElecEgy6->setFocus();
    }
    else
    {
        ui->leFoilSelectNewElecEgy5->setText("");
        ui->leFoilSelectNewElecEgy5->setFocus();
    }
}

void AccUi::on_leFoilSelectNewElecEgy6_returnPressed()
{
    int foil=ui->leFoilSelectNewElecEgy6->text().toInt();
    if(foil>0 && foil<6)
    {
        ui->leFoilSelectNewElecEgy6->setReadOnly(true);
        ui->leFoilSelectNewElecEgy6->clearFocus();

        foil=ui->leFoilSelectNewElecEgy1->text().toInt();
        SetFoilSelect(E1,foil);
        foil=ui->leFoilSelectNewElecEgy2->text().toInt();
        SetFoilSelect(E2,foil);
        foil=ui->leFoilSelectNewElecEgy3->text().toInt();
        SetFoilSelect(E3,foil);
        foil=ui->leFoilSelectNewElecEgy4->text().toInt();
        SetFoilSelect(E4,foil);
        foil=ui->leFoilSelectNewElecEgy5->text().toInt();
        SetFoilSelect(E5,foil);
        foil=ui->leFoilSelectNewElecEgy6->text().toInt();
        SetFoilSelect(E6,foil);

        io->SendFoilTable(E1);
        io->SendFoilTable(E2);
        io->SendFoilTable(E3);
        io->SendFoilTable(E4);
        io->SendFoilTable(E5);
        io->SendFoilTable(E6);

        io->WriteOwnFile();

        ui->leFoilSelectCurrElecEgy1->setText(ui->leFoilSelectNewElecEgy1->text());
        ui->leFoilSelectCurrElecEgy2->setText(ui->leFoilSelectNewElecEgy2->text());
        ui->leFoilSelectCurrElecEgy3->setText(ui->leFoilSelectNewElecEgy3->text());
        ui->leFoilSelectCurrElecEgy4->setText(ui->leFoilSelectNewElecEgy4->text());
        ui->leFoilSelectCurrElecEgy5->setText(ui->leFoilSelectNewElecEgy5->text());
        ui->leFoilSelectCurrElecEgy6->setText(ui->leFoilSelectNewElecEgy6->text());
    }
    else
    {
        ui->leFoilSelectNewElecEgy6->setText("");
        ui->leFoilSelectNewElecEgy6->setFocus();
    }
}


void AccUi::SetFoilSelect(RAY_TYPE_T type, int val)
{
    switch (val) {
    case 1:
        io->foils[type] = FOIL1;
        break;
    case 2:
        io->foils[type] = FOIL2;
        break;
    case 3:
        io->foils[type] = FOIL3;
        break;
    case 4:
        io->foils[type] = FOIL4;
        break;
    case 5:
        io->foils[type] = FOIL5;
        break;
    default:
        break;
    }
}

int AccUi::SetFoilSelectNumber(RAY_TYPE_T type)
{
    switch (io->foils[type])
    {
    case FOIL1:
        return 1;
        break;
    case FOIL2:
        return 2;
        break;
    case FOIL3:
        return 3;
        break;
    case FOIL4:
        return 4;
        break;
    case FOIL5:
        return 5;
        break;
    default:
        return 0;
        break;
    }
}

void AccUi::SetTapSelect(RAY_TYPE_T type, QString tap)
{
    if(tap=="N")
    {
        io->taps[type] = NOTAP;
    }
    else if(tap=="L")
    {
        io->taps[type] = TAP143V;
    }
    else if(tap=="M")
    {
        io->taps[type] = TAP122V;
    }
    else if(tap=="H")
    {
        io->taps[type] = TAP100V;
    }
}

//----------------------------------------------------
//T  Tap Selection  抽头选择菜单
void AccUi::on_leTapSelectNewE1_returnPressed()
{
    QString tap=ui->leTapSelectNewE1->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewE2->setText("E-2    =");
        ui->leTapSelectNewE2->setReadOnly(false);
        ui->leTapSelectNewE2->setFocus();
    }
    else
    {
        ui->leTapSelectNewE1->setText("");
        ui->leTapSelectNewE1->setFocus();
    }
}

void AccUi::on_leTapSelectNewE2_returnPressed()
{
    QString tap=ui->leTapSelectNewE2->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewE3->setText("E-3    =");
        ui->leTapSelectNewE3->setReadOnly(false);
        ui->leTapSelectNewE3->setFocus();
    }
    else
    {
        ui->leTapSelectNewE2->setText("");
        ui->leTapSelectNewE2->setFocus();
    }
}

void AccUi::on_leTapSelectNewE3_returnPressed()
{
    QString tap=ui->leTapSelectNewE3->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewE4->setText("E-4    =");
        ui->leTapSelectNewE4->setReadOnly(false);
        ui->leTapSelectNewE4->setFocus();
    }
    else
    {
        ui->leTapSelectNewE3->setText("");
        ui->leTapSelectNewE3->setFocus();
    }
}

void AccUi::on_leTapSelectNewE4_returnPressed()
{
    QString tap=ui->leTapSelectNewE4->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewE5->setText("E-5    =");
        ui->leTapSelectNewE5->setReadOnly(false);
        ui->leTapSelectNewE5->setFocus();
    }
    else
    {
        ui->leTapSelectNewE4->setText("");
        ui->leTapSelectNewE4->setFocus();
    }
}

void AccUi::on_leTapSelectNewE5_returnPressed()
{
    QString tap=ui->leTapSelectNewE5->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewE6->setText("E-6    =");
        ui->leTapSelectNewE6->setReadOnly(false);
        ui->leTapSelectNewE6->setFocus();
    }
    else
    {
        ui->leTapSelectNewE5->setText("");
        ui->leTapSelectNewE5->setFocus();
    }
}

void AccUi::on_leTapSelectNewE6_returnPressed()
{
    QString tap=ui->leTapSelectNewE6->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewXL->setText("X-L    =");
        ui->leTapSelectNewXL->setReadOnly(false);
        ui->leTapSelectNewXL->setFocus();
    }
    else
    {
        ui->leTapSelectNewE6->setText("");
        ui->leTapSelectNewE6->setFocus();
    }
}

void AccUi::on_leTapSelectNewXL_returnPressed()
{
    QString tap=ui->leTapSelectNewXL->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewXH->setText("X-H    =");
        ui->leTapSelectNewXH->setReadOnly(false);
        ui->leTapSelectNewXH->setFocus();
    }
    else
    {
        ui->leTapSelectNewXL->setText("");
        ui->leTapSelectNewXL->setFocus();
    }
}

void AccUi::on_leTapSelectNewXH_returnPressed()
{
    QString tap=ui->leTapSelectNewXH->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->lbTapSelectNewFILM->setText("FILM   =");
        ui->leTapSelectNewFILM->setReadOnly(false);
        ui->leTapSelectNewFILM->setFocus();
    }
    else
    {
        ui->leTapSelectNewXH->setText("");
        ui->leTapSelectNewXH->setFocus();
    }
}

void AccUi::on_leTapSelectNewFILM_returnPressed()
{
    QString tap=ui->leTapSelectNewFILM->text();
    if(tap=="N" || tap=="L" || tap=="M" || tap=="H")
    {
        ui->leTapSelectNewFILM->clearFocus();
        ui->leTapSelectNewFILM->setReadOnly(true);

        tap=ui->leTapSelectNewE1->text();
        SetTapSelect(E1,tap);
        tap=ui->leTapSelectNewE2->text();
        SetTapSelect(E2,tap);
        tap=ui->leTapSelectNewE3->text();
        SetTapSelect(E3,tap);
        tap=ui->leTapSelectNewE4->text();
        SetTapSelect(E4,tap);
        tap=ui->leTapSelectNewE5->text();
        SetTapSelect(E5,tap);
        tap=ui->leTapSelectNewE6->text();
        SetTapSelect(E6,tap);
        tap=ui->leTapSelectNewXL->text();
        SetTapSelect(X_LO,tap);
        tap=ui->leTapSelectNewXH->text();
        SetTapSelect(X_HI,tap);
        tap=ui->leTapSelectNewFILM->text();
        SetTapSelect(FILM,tap);

        io->SendTap(E1);
        io->SendTap(E2);
        io->SendTap(E3);
        io->SendTap(E4);
        io->SendTap(E5);
        io->SendTap(E6);
        io->SendTap(X_LO);
        io->SendTap(X_HI);
        io->SendTap(FILM);

        io->WriteOwnFile();

        ui->leTapSelectCurrE1->setText(ui->leTapSelectNewE1->text());
        ui->leTapSelectCurrE2->setText(ui->leTapSelectNewE2->text());
        ui->leTapSelectCurrE3->setText(ui->leTapSelectNewE3->text());
        ui->leTapSelectCurrE4->setText(ui->leTapSelectNewE4->text());
        ui->leTapSelectCurrE5->setText(ui->leTapSelectNewE5->text());
        ui->leTapSelectCurrE6->setText(ui->leTapSelectNewE6->text());
        ui->leTapSelectCurrXL->setText(ui->leTapSelectNewXL->text());
        ui->leTapSelectCurrXH->setText(ui->leTapSelectNewXH->text());
        ui->leTapSelectCurrFILM->setText(ui->leTapSelectNewFILM->text());
    }
    else
    {
        ui->leTapSelectNewFILM->setText("");
        ui->leTapSelectNewFILM->setFocus();
    }
}

void AccUi::on_leGantrySpdFactClk_returnPressed()
{
    QString disp;
    float speed_fact_cw;
    float old_speed_fact_cw;
    bool suc;
    speed_fact_cw = ui->leGantrySpdFactClk->text().toFloat();
    suc = SlaveBase::CheckValue(speed_fact_cw, GANTRY_SPEED_FACTOR_CW_CK);
    if(!suc)
    {
        old_speed_fact_cw = motor->gantry_speed_factor_cw;
        disp.sprintf("%.6f", old_speed_fact_cw);
        ui->leGantrySpdFactClk->setText(disp);
        SetSvcFinishEdit(ui->leGantrySpdFactClk);
        return;
    }
    motor->gantry_speed_factor_cw = speed_fact_cw;

    disp.sprintf("%.6f", speed_fact_cw);
    ui->leGantrySpdFactClk->setText(disp);

    SetSvcFinishEdit(ui->leGantrySpdFactClk);
    return;
}

void AccUi::on_leGantrySpdFactCountClk_returnPressed()
{
    QString disp;
    float speed_fact_ccw;
    float old_speed_fact_ccw;
    bool suc;
    speed_fact_ccw = ui->leGantrySpdFactCountClk->text().toFloat();
    suc = SlaveBase::CheckValue(speed_fact_ccw, GANTRY_SPEED_FACTOR_CCW_CK);
    if(!suc)
    {
        old_speed_fact_ccw = motor->gantry_speed_factor_ccw;
        disp.sprintf("%.6f", old_speed_fact_ccw);
        ui->leGantrySpdFactCountClk->setText(disp);
        SetSvcFinishEdit(ui->leGantrySpdFactCountClk);

        return;
    }
    motor->gantry_speed_factor_ccw = speed_fact_ccw;

    disp.sprintf("%.6f", speed_fact_ccw);
    ui->leGantrySpdFactCountClk->setText(disp);

    SetSvcFinishEdit(ui->leGantrySpdFactCountClk);

    return;
}


void AccUi::on_leSvcWarnBI1_returnPressed()
{
    QString disp;
    float bi_1 = ui->leSvcWarnBI1->text().toFloat();
    disp.sprintf("%.2f", bi_1);
    ui->leSvcWarnBI1->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnBI1);
}

void AccUi::on_leSvcWarnBI2_returnPressed()
{
    QString disp;
    float bi_2 = ui->leSvcWarnBI2->text().toFloat();
    disp.sprintf("%.2f", bi_2);
    ui->leSvcWarnBI2->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnBI2);
}

void AccUi::on_leSvcWarnJawA1_returnPressed()
{
    QString disp;
    float jawa1 = ui->leSvcWarnJawA1->text().toFloat();
    disp.sprintf("%.2f", jawa1);
    ui->leSvcWarnJawA1->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnJawA1);
}

void AccUi::on_leSvcWarnJawA2_returnPressed()
{
    QString disp;
    float jawa2 = ui->leSvcWarnJawA2->text().toFloat();
    disp.sprintf("%.2f", jawa2);
    ui->leSvcWarnJawA2->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnJawA2);
}

void AccUi::on_leSvcWarnJawB1_returnPressed()
{
    QString disp;
    float jawb1 = ui->leSvcWarnJawB1->text().toFloat();
    disp.sprintf("%.2f", jawb1);
    ui->leSvcWarnJawB1->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnJawB1);
}

void AccUi::on_leSvcWarnJawB2_returnPressed()
{
    QString disp;
    float jawb2 = ui->leSvcWarnJawB2->text().toFloat();
    disp.sprintf("%.2f", jawb2);
    ui->leSvcWarnJawB2->setText(disp);
    SetSvcFinishEdit(ui->leSvcWarnJawB2);
}

void AccUi::ShowChineseLabel()
{
    ui->lbMon1->setText(MON_1_UI);
    ui->lbMon2->setText(MON_2_UI);
    ui->lbTime->setText(TIME_UI);
    ui->lbMumin->setText(MU_MIN_UI);
    ui->lbMod->setText(MODE_UI);
    ui->lbSeg->setText(SEG_UI);
    ui->lbEnergy->setText(ENERGY_UI);
    ui->lbTechnique->setText(TECHNIQUE_UI);

    ui->lbColli->setText(COLLIMATOR_UI);
    ui->lbJawFlag->setText(JAWS_UI);

    ui->labelGanDirec->setText(G_DIREC_UI);
    ui->labelGanPos->setText(G_POS_UI);
    ui->labelGanSpeed->setText(G_SPEED_UI);
    ui->labelArc1->setText(ARC_UI);
    ui->lbAcc1->setText(ACC1_UI);

    ui->lbAcc1->setText(ACC1_UI);
}

bool AccUi::RecallConfigFiles()
{
    QDir sourceDir(".\\Backup\\AccConfig");
    QDir targetDir(".\\AccConfig");
    if(!targetDir.exists())
    {    /**< 如果目标目录不存在，则进行创建 */
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir())
        {    /**< 当为目录时，递归的进行copy */
            if(!RecallConfigFiles())
                return false;
        }
        else
        {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
            if(targetDir.exists(fileInfo.fileName()))
            {
                targetDir.remove(fileInfo.fileName());
            }

            /// 进行文件copy
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName())))
                return false;
        }
    }
    return true;
}

void AccUi::SoftpotFtpUpload()
{
    for(int i=0;i<serverfile.length();i++)
    {
        doUpload(clientfile[i],serverfile[i]);
    }
}

void AccUi::SoftpotFtpDownload()
{
    for(int i=0;i<serverfile.length();i++)
    {
        doDownload(".\\AccConfig\\",serverfile[i]);
    }
}

void AccUi::SendSoftpotValue()
{
    //发送beam 31-35 命令
    beam->SendAfcpInjeInji();
    beam->SendElmBmiStcLen();
    beam->SendPfnPrfpIpfnSomin();
    beam->SendDataNotUsed();
    beam->SendBmisMinMax();

    io->SendTrigger();      //46
    io->SendDosLoopCtl();   //4e

    dosei->SendDynamicSteer();      //44
    dosei->SendGainAndWorkMode();   //47

    doseii->SendGainAndWorkMode();  //48
    doseii->SendSegmentGain();      //53
}

void AccUi::GetOriginalValue(QLineEdit *lineedit)
{

    lineedit->setFocus();
    lineedit->selectAll();
    tmp_value_2  = lineedit->text();
    has_press_return_2 =false;
}

void AccUi::SetSvcFinishEdit(QLineEdit *lineedit)
{
    has_press_return_2 = true;
    lineedit->setFocus();
    lineedit->selectAll();
}

void AccUi::SetSvcOldEdit(QLineEdit *lineedit)
{
    if(!has_press_return_2)
    {
        lineedit->setText(tmp_value_2);
        has_press_return_2 = true;
    }
    else
    {
        if(lineedit->text() == "")
            lineedit->setText(tmp_value_2);
    }
}

void AccUi::SetSvcNextEdit(QLineEdit *lineedit)
{
    lineedit->setFocus();
    lineedit->selectAll();
    tmp_value_2 = lineedit->text();
    // lineedit->setStyleSheet("background:white");
    has_press_return_2 = false;
}

void AccUi::SetMotAdjLeftEdit(int row, int column)
{
    for(int i = 0;i<row;i++)
    {
        for(int j =0;j<column;j++)
        {
            if(motor_adjust[i].at(j)->hasFocus())
            {
                SetSvcOldEdit(motor_adjust[i].at(j));
                SetSvcNextEdit(motor_adjust[i].at((j+column-1)%column));
                return;
            }
        }
    }
}

void AccUi::SetMotAdjRightEdit(int row,int column)
{
    for(int i = 0;i<row;i++)
    {
        for(int j =0;j<column;j++)
        {
            if(motor_adjust[i].at(j)->hasFocus())
            {
                SetSvcOldEdit(motor_adjust[i].at(j));
                SetSvcNextEdit(motor_adjust[i].at((j+1)%column));
                return;
            }
        }
    }
}

void AccUi::SetMotAdjUpEdit(int row, int column)
{
    for(int i = 0;i<row;i++)
    {
        for(int j =0;j<column;j++)
        {
            if(motor_adjust[i].at(j)->hasFocus())
            {
                SetSvcOldEdit(motor_adjust[i].at(j));
                SetSvcNextEdit(motor_adjust[(i+row-1)%row].at(j));
                return;
            }
        }
    }
}

void AccUi::SetMotAdjDownnEdit(int row, int column)
{

    for(int i = 0;i<row;i++)
    {
        for(int j =0;j<column;j++)
        {
            if(motor_adjust[i].at(j)->hasFocus())
            {
                SetSvcOldEdit(motor_adjust[i].at(j));
                SetSvcNextEdit(motor_adjust[(i+1)%row].at(j));
                return;
            }
        }
    }
}
