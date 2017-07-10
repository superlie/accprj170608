#include "accui.h"
#include "ui_accui.h"

void AccUi::ShowActSvcValue(SVC_LINEEDIT type)
{
    float tmp;
    UINT32 dose_rate_offset_abs; //16-4-21 dose off negative or positive
    QString str;
    switch(type)
    {
    case AFCM:
        tmp = base->ConvertMemberToFloat(beam->afcm, AFCP_RSCH_CK);
        str.sprintf("%04d", (UINT32)tmp);
        ui->leSoftpotsBeam1Afcm->setText(str);
        break;
    case ELMI_BMIS_STCI_LENI:
//        tmp = (float)beam->elmi * 100.0 / 4095.0;
        str.sprintf("%.1f", beam->elmi_f);
        ui->leSoftpotsBeam1Elmi->setText(str);

//        tmp = (float)beam->bmis * 100.0 / 4095.0;
        str.sprintf("%.1f", beam->bmis_f);
        ui->leSoftpotsBeam1Bmis->setText(str);

//        tmp = (float)beam->stci * 10000.0 / 4095.0;
        str.sprintf("%04d", (UINT32)beam->stci_f);
        ui->leSoftpotsBeam1Stci->setText(str);

//        tmp = (float)beam->leni * 10.0 / 4095.0;
        str.sprintf("%.2f", beam->leni_f);
        ui->leSoftpotsBeam1Leni->setText(str);
        break;
    case DR_1:
        tmp = base->ConvertMemberToFloat(dosei->dose1_aver_rate, DOSE_RATE1_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose1Dr1->setText(str);
        break;
    case DR_2:
        tmp = base->ConvertMemberToFloat(doseii->dose2_aver_rate, DOSE_RATE1_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose1Dr2->setText(str);
        break;
    case SXE_FXE:
        str.sprintf("%.1f", dosei->symmetry);
        ui->leSoftpotsDose2Sxe->setText(str);

        str.sprintf("%.1f", dosei->flatness);
        ui->leSoftpotsDose2Fxe->setText(str);
        break;
    case SYE_FYE:
        str.sprintf("%.1f", doseii->symmetry);
        ui->leSoftpotsDose2Sye->setText(str);

        str.sprintf("%.1f", doseii->flatness);
        ui->leSoftpotsDose2Fye->setText(str);
        break;
    case OFF1:
        dose_rate_offset_abs = 0x7FFF & dosei->dose1_rate_offset;
        tmp = base->ConvertMemberToFloat(dose_rate_offset_abs, DOSE_RATE1_OFF);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose1Off1->setText(str);
        break;
    case OFF2:
        dose_rate_offset_abs = 0x7FFF & doseii->dose2_rate_offset;
        tmp = base->ConvertMemberToFloat(dose_rate_offset_abs, DOSE_RATE2_OFF);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose1Off2->setText(str);
        break;
    case SEG013:
        tmp = base->ConvertMemberToFloat(dosei->segment_0, S0_GAIN_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose2Seg0->setText(str);
        tmp = base->ConvertMemberToFloat(dosei->segment_1, S1_GAIN_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose2Seg1->setText(str);
        tmp = base->ConvertMemberToFloat(dosei->segment_3, S3_GAIN_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose2Seg3->setText(str);
        break;
    case SEG24:
        tmp = base->ConvertMemberToFloat(doseii->segment_2, S2_GAIN_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose2Seg2->setText(str);
        tmp = base->ConvertMemberToFloat(doseii->segment_4, S4_GAIN_CK);
        str.sprintf("%d", (UINT32)tmp);
        ui->leSoftpotsDose2Seg4->setText(str);
        break;
    case TEMP_PRESS1:
        str.sprintf("%.1f", dosei->float_temp);
        ui->leCpsFactorActTemp1->setText(str);
        str.sprintf("%.1f", dosei->float_press);
        ui->leCpsFactorActPress1->setText(str);
        break;
    case TEMP_PRESS2:
        str.sprintf("%.1f", doseii->float_temp);
        ui->leCpsFactorActTemp2->setText(str);
        str.sprintf("%.1f", doseii->float_press);
        ui->leCpsFactorActPress2->setText(str);
        break;
    case CPS_FACTOR:
        str.sprintf("%f", DoseBase::compen_factor);
        ui->leCpsFactor->setText(str);
        break;
    case T1_0:
        str.sprintf("%d", motor->line_status);
        ui->leT1DisplayReport0->setText(str);
        break;
    case T1_1:
        break;
    case T1_2:
        break;
    case T1_3:
        str.sprintf("%d", beam->line_status);
        ui->leT1DisplayReport3->setText(str);
        break;
    case T1_4:
        break;
    case T1_5:
        str.sprintf("%d", light->line_status);
        ui->leT1DisplayReport5->setText(str);
        break;
    case T1_6:
        str.sprintf("%d", interlock->line_status);
        ui->leT1DisplayReport6->setText(str);
        break;
    case T1_7:
        str.sprintf("%d", io->line_status);
        ui->leT1DisplayReport7->setText(str);
        break;
    case CHK_0:
        str.sprintf("%xH", motor->check_report);
        ui->leChkDispReport0->setText(str);
        break;
    case CHK_1:
        str.sprintf("%xH", dosei->check_report);
        ui->leChkDispReport1->setText(str);
        break;
    case CHK_2:
        str.sprintf("%xH", doseii->check_report);
        ui->leChkDispReport2->setText(str);
        break;
    case CHK_3:
        str.sprintf("%xH", beam->check_report);
        ui->leChkDispReport3->setText(str);
        break;
    case CHK_4:
        break;
    case CHK_5:
        str.sprintf("%xH", light->check_report);
        ui->leChkDispReport5->setText(str);
        break;
    case CHK_6:
        str.sprintf("%xH", interlock->check_report);
        ui->leChkDispReport6->setText(str);
        break;
    case CHK_7:
        str.sprintf("%xH", io->check_report);
        ui->leChkDispReport7->setText(str);
        break;
    default:
        break;
    }
}

void AccUi::Interlock_Redisplay()
{
    ProcessInterlockMsgDown();
}



void AccUi::downloadFinished(QNetworkReply *reply)
{
    QString str;
    QUrl url = reply->url();
    if (reply->error()==0)
    {
        QString filename = saveFileName(url);
        if (saveToDisk(filename, reply))
        {
            updownload_file_num++;
            str.sprintf("Have  downloaded  %d/24  files.",updownload_file_num);
            ui->leSptUpDownStatus->setText(str);
        }
    }
    else
    {
        qDebug() << "download error" << endl;
    }
    reply->deleteLater();
}

void AccUi::uploadFinished(QNetworkReply *reply)
{
    QString str;

    if(reply->error()==0)
    {
        updownload_file_num++;
        str.sprintf("Have  uploaded  %d/24  files.",updownload_file_num);
        ui->leSptUpDownStatus->setText(str);
    }
    else
    {
        qDebug() << "upload error" << endl;
        qDebug() << reply->errorString() << endl;
    }
    reply->deleteLater();
}

//beam1 return value
void AccUi::SetActAfcm(UINT32 val)
{
    QString temp;
    temp.sprintf("%04d", val);
    ui->leSoftpotsBeam1Afcm->setText(temp);
}

void AccUi::SetActElmi(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leSoftpotsBeam1Elmi->setText(temp);
}

void AccUi::SetActBmis(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leSoftpotsBeam1Bmis->setText(temp);
}

void AccUi::SetActStci(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsBeam1Stci->setText(temp);
}

void AccUi::SetActLeni(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leSoftpotsBeam1Leni->setText(temp);
}

//beam2 return value
void AccUi::SetActDscx(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsBeam2Dscx->setText(temp);
}

void AccUi::SetActDscy(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsBeam2Dscy->setText(temp);
}

//beam2 DS_XP  DS_YP..........not process

//dos1 return value
void AccUi::SetActDr1(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose1Dr1->setText(temp);
}

void AccUi::SetActDr2(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose1Dr2->setText(temp);
}

void AccUi::SetActOff1(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose1Off1->setText(temp);
}

void AccUi::SetActOff2(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose1Off2->setText(temp);
}

//dos2 return value
void AccUi::SetActSeg0(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose2Seg0->setText(temp);
}

void AccUi::SetActSeg1(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose2Seg1->setText(temp);
}

void AccUi::SetActSeg2(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose2Seg2->setText(temp);
}

void AccUi::SetActSeg3(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose2Seg3->setText(temp);
}

void AccUi::SetActSeg4(UINT32 val)
{
    QString temp;
    temp.sprintf("%d", val);
    ui->leSoftpotsDose2Seg4->setText(temp);
}

void AccUi::SetActSxe(float sxe)
{
    QString temp;
    temp.sprintf("%.1f", sxe);
    ui->leSoftpotsDose2Sxe->setText(temp);
}

void AccUi::SetActSye(float sye)
{
    QString temp;
    temp.sprintf("%.1f", sye);
    ui->leSoftpotsDose2Sye->setText(temp);
}

void AccUi::SetActFxe(float fxe)
{
    QString temp;
    temp.sprintf("%.1f", fxe);
    ui->leSoftpotsDose2Fxe->setText(temp);
}

void AccUi::SetActFye(float fye)
{
    QString temp;
    temp.sprintf("%.1f", fye);
    ui->leSoftpotsDose2Fye->setText(temp);
}

void AccUi::SetActTemp1(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leCpsFactorActTemp1->setText(temp);
}

void AccUi::SetActTemp2(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leCpsFactorActTemp2->setText(temp);
}

void AccUi::SetActPress1(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leCpsFactorActPress1->setText(temp);
}

void AccUi::SetActPress2(float val)
{
    QString temp;
    temp.sprintf("%.1f", val);
    ui->leCpsFactorActPress2->setText(temp);
}

//J  Check Display 检查显示
void AccUi::SetActChkDispReport0(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport0->setText(temp);
}

void AccUi::SetActChkDispReport1(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport1->setText(temp);
}

void AccUi::SetActChkDispReport2(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport2->setText(temp);
}

void AccUi::SetActChkDispReport3(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport3->setText(temp);
}

void AccUi::SetActChkDispReport5(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport5->setText(temp);
}

void AccUi::SetActChkDispReport6(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport6->setText(temp);
}

void AccUi::SetActChkDispReport7(UINT8 val)
{
    QString temp;
    temp.sprintf("%xH", val);
    ui->leChkDispReport7->setText(temp);
}

//K  T1 Display  T1显示
//void AccUi::SetActT1DispReport0(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport0->setText(temp);
//}

//void AccUi::SetActT1DispReport1(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport1->setText(temp);
//}

//void AccUi::SetActT1DispReport2(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport2->setText(temp);
//}

//void AccUi::SetActT1DispReport3(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport3->setText(temp);
//}

//void AccUi::SetActT1DispReport4(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport4->setText(temp);
//}

//void AccUi::SetActT1DispReport5(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport5->setText(temp);
//}

//void AccUi::SetActT1DispReport6(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport6->setText(temp);
//}

//void AccUi::SetActT1DispReport7(UINT8 val)
//{
//    QString temp;
//    temp.sprintf("%d", val);
//    ui->leT1DisplayReport7->setText(temp);
//}
//------------------------------------------
// duanzhibin 9/24
void AccUi::SetAccessoy_3_Disp(ACCESSORY_B_TYPE temp)
{
    switch(temp)
    {
    case EANULL:
    case EANULL_1:
        ui->lineEditAcc3->setText(ACC_NONE_UI);
        ui->labelX1->setText("");
        ui->labelY1->setText("");
        ui->lbColl1->setText("");
        ui->lineEditJawX1->setText("");
        ui->lineEditJawX1->setStyleSheet("border: 2px solid black;");
        ui->lineEditJawX1->setFrame(false);
        ui->lineEditJawY1->setText("");
        ui->lineEditJawY1->setStyleSheet("border: 2px solid black;");
        ui->lineEditJawY1->setFrame(false);

        show_jawx1x2y1y2 = false;       //LJY-12-24
        ui->labelX2->setText("");
        ui->labelY2->setText("");
        ui->lineEditJawX2->setText("");
        ui->lineEditJawX2->setStyleSheet("border: 2px solid black;");
        ui->lineEditJawX2->setFrame(false);
        ui->lineEditJawY2->setText("");
        ui->lineEditJawY2->setStyleSheet("border: 2px solid black;");
        ui->lineEditJawY2->setFrame(false);
        break;
    case EA205:
        Display_Acc_3();
        ui->lineEditAcc3->setText("EA205");
        ui->lineEditJawX1->setText("05.0");
        ui->lineEditJawY1->setText("05.0");
        SendJawX(13);
        SendJawY(13);
//        ui->lineEditJawX->setText("13.0");
//        ui->lineEditJawY->setText("13.0");
        //        if (g_status==PROGRAM)
        //        {
        //            SendJawX();
        //            SendJawY();
        //        }
        //        else
        //        {
        //            g_acc3_move_jaw = true;
        //        }
//        SendJawX();
//        SendJawY();
        break;
    case EA210:
        Display_Acc_3();
        ui->lineEditAcc3->setText("EA210");
        ui->lineEditJawX1->setText("10.0");
        ui->lineEditJawY1->setText("10.0");
//        ui->lineEditJawX->setText("19.0");
//        ui->lineEditJawY->setText("19.0");
        SendJawX(19);
        SendJawY(19);
        break;
    case EA215:
        Display_Acc_3();
        ui->lineEditAcc3->setText("EA215");
        ui->lineEditJawX1->setText("15.0");
        ui->lineEditJawY1->setText("15.0");
//        ui->lineEditJawX->setText("23.0");
//        ui->lineEditJawY->setText("23.0");
        SendJawX(23);
        SendJawY(23);
        break;
    case EA220:
        Display_Acc_3();
        ui->lineEditAcc3->setText("EA220");
        ui->lineEditJawX1->setText("20.0");
        ui->lineEditJawY1->setText("20.0");
//        ui->lineEditJawX->setText("27.0");
//        ui->lineEditJawY->setText("27.0");
        SendJawX(27);
        SendJawY(27);
        break;
    case EA225:
        Display_Acc_3();
        ui->lineEditAcc3->setText("EA225");
        ui->lineEditJawX1->setText("25.0");
        ui->lineEditJawY1->setText("25.0");
//        ui->lineEditJawX->setText("32.0");
//        ui->lineEditJawY->setText("32.0");
        SendJawX(32);
        SendJawY(32);
        break;
    default:
        ui->lineEditAcc3->setText(ACC_INVALID_UI);
        // InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
        break;
    }
    ui->svcTreat->update();

    //upper->SetAcc3(ui->lineEditAcc3->text());
}

void AccUi::SetAccessoy_2_Disp(ACCESSORY_A2_TYPE temp,ACCESSORY_2_DIC dir)
{
    if(dir == DIC_1)
    {
        switch(temp)
        {
        case RWNULL_1:
        case RWNULL_2:
            ui->lineEditAcc2->setText(ACC_NONE_UI);
            break;
        case RW_15:
            ui->lineEditAcc2->setText("1RW15");
            break;
        case RW_30:
            ui->lineEditAcc2->setText("1RW30");
            break;
        case RW_45:
            ui->lineEditAcc2->setText("1RW45");
            break;
        case RW_60:
            ui->lineEditAcc2->setText("1RW60");
            break;
        default:
            ui->lineEditAcc2->setText(ACC_INVALID_UI);
            //     InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
            break;
        }
    }
    else if(dir == DIC_2)
    {
        switch(temp)
        {
        case RWNULL_1:
        case RWNULL_2:
            ui->lineEditAcc2->setText(ACC_NONE_UI);
            break;
        case RW_15:
            ui->lineEditAcc2->setText("2RW15");
            break;
        case RW_30:
            ui->lineEditAcc2->setText("2RW30");
            break;
        case RW_45:
            ui->lineEditAcc2->setText("2RW45");
            break;
        case RW_60:
            ui->lineEditAcc2->setText("2RW60");
            break;
        default:
            ui->lineEditAcc2->setText(ACC_INVALID_UI);
            //   InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
            break;
        }
    }
    else if(dir == DIC_3)
    {
        switch(temp)
        {
        case RWNULL_1:
        case RWNULL_2:
            ui->lineEditAcc2->setText(ACC_NONE_UI);
            break;
        case RW_15:
            ui->lineEditAcc2->setText("3RW15");
            break;
        case RW_30:
            ui->lineEditAcc2->setText("3RW30");
            break;
        case RW_45:
            ui->lineEditAcc2->setText("3RW45");
            break;
        case RW_60:
            ui->lineEditAcc2->setText("3RW60");
            break;
        default:
            ui->lineEditAcc2->setText(ACC_INVALID_UI);
            //   InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
            break;
        }
    }
    else if(dir == DIC_4)
    {
        switch(temp)
        {
        case RWNULL_1:
        case RWNULL_2:
            ui->lineEditAcc2->setText(ACC_NONE_UI);
            break;
        case RW_15:
            ui->lineEditAcc2->setText("4RW15");
            break;
        case RW_30:
            ui->lineEditAcc2->setText("4RW30");
            break;
        case RW_45:
            ui->lineEditAcc2->setText("4RW45");
            break;
        case RW_60:
            ui->lineEditAcc2->setText("4RW60");
            break;
        default:
            ui->lineEditAcc2->setText(ACC_INVALID_UI);
            //   InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
            break;
        }
    }
    else if(dir == DNULL_1 || dir == DNULL_2)
    {
        ui->lineEditAcc2->setText(ACC_NONE_UI);
    }
    else
    {
        ui->lineEditAcc2->setText(ACC_INVALID_UI);
    }

    //upper->SetAcc2(ui->lineEditAcc2->text());
}
void AccUi::SetAccessoy_1_Disp(ACCESSORY_A1_TYPE temp)
{
    switch(temp)
    {
    case RETI_NULL:
    case RETI_NULL_1:
        ui->lineEditAcc1->setText(ACC_NONE_UI);
        break;
    case RETI:
    case RETI_1:
        ui->lineEditAcc1->setText("RETI");
        break;
    default:
        ui->lineEditAcc1->setText(ACC_INVALID_UI);
        //InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
        break;
    }

    //upper->SetAcc1(ui->lineEditAcc1->text());
}

void AccUi::SetInterlockTableItem(int num, bool ovrd, INTERLOCK_STATUS_T status, int error, UINT32 history)
{
    int row;
    row=num % 10;
    ui->tWPage1->item(row,0)->setText(interlock_list[num]);
    if(ovrd)
        ui->tWPage1->item(row,1)->setText("Y");
    else
        ui->tWPage1->item(row,1)->setText("");

    switch(status)
    {
    case IL_CLAER:
        ui->tWPage1->item(row,2)->setText("clear");
        break;
    case IL_ACTIVE:
        ui->tWPage1->item(row,2)->setText("active");
        break;
    case IL_LATCHED:
        ui->tWPage1->item(row,2)->setText("latched");
        break;
    default:
        break;
    }

    if(error < 0)
        ui->tWPage1->item(row, 3)->setText("");
    else
        ui->tWPage1->item(row, 3)->setText(QString("%1").arg(error));

    ui->tWPage1->item(row, 4)->setText(QString("%1").arg(history));
}

void AccUi::Display_Acc_3()
{    
    ui->lbColl1->setStyleSheet("color:white");
    ui->labelX1->setStyleSheet("color:white");
    ui->labelY1->setStyleSheet("color:white");

    ui->lbColl1->setText(TRIMMERS_UI);
    ui->labelX1->setText("X1");
    ui->labelY1->setText("Y1");

    //    ui->labelX1->show();
    //    ui->labelY1->show();

    ui->lineEditJawX1->setFrame(true);
    ui->lineEditJawY1->setFrame(true);

    ui->lineEditJawX1->setStyleSheet("border: 2px solid white;");
    ui->lineEditJawY1->setStyleSheet("border: 2px solid white;");

    show_jawx1x2y1y2 = false;       //LJY-12-24
    ui->labelX2->setText("");
    ui->labelY2->setText("");
    ui->lineEditJawX2->setText("");
    ui->lineEditJawX2->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawX2->setFrame(false);
    ui->lineEditJawY2->setText("");
    ui->lineEditJawY2->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawY2->setFrame(false);

}
