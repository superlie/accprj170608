#include "accui.h"
#include "ui_accui.h"

void AccUi::InitLastTreatData()
{
    lastdata.ray_type = X_LO;
    lastdata.dose1 = 0;
    lastdata.dose2 = 0;
    lastdata.time = 0;
    lastdata.rate = 0;
    lastdata.jawx1 = 0;
    lastdata.jawx2 = 0;
    lastdata.jawy1 = 0;
    lastdata.jawy2 = 0;
    lastdata.colli = 0;
    lastdata.gpos = 0;
    lastdata.arc = 0;
    lastdata.stop = 0;
    lastdata.gspeed = 0;
    lastdata.acc1 = "";
    lastdata.acc2 = "";
    lastdata.acc3 = "";

}

void AccUi::SaveLastTreatData()
{
    //qDebug()<<"SaveLastTreatData";
    lastdata.ray_type = io->accept_ray_type;
    lastdata.dose1 = dosei->f_preset_dose1;
    lastdata.dose2 = doseii->f_preset_dose2;
    lastdata.time = preset_time;
    lastdata.rate = dosei->dose_rate;
    lastdata.jawx1 = jawx1_act_pos;
    lastdata.jawx2 = jawx2_act_pos;
    lastdata.jawy1 = jawy1_act_pos;
    lastdata.jawy2 = jawy2_act_pos;
    lastdata.colli = colli_act_pos;
    if(lastdata.ray_type == ROT_X_LO || lastdata.ray_type == ROT_X_HI)
    {
        lastdata.gpos = gantry_start_pos;
        lastdata.arc = gantry_rot_arc;
        lastdata.stop = gantry_stop_pos;
        lastdata.gspeed = gantry_speed;
    }
    else
    {
        lastdata.gpos = gantry_act_pos;
        lastdata.arc = 0;
        lastdata.stop = 0;
        lastdata.gspeed = 0;
    }
    lastdata.acc1 = ui->lineEditAcc1->text();
    lastdata.acc2 = ui->lineEditAcc2->text();
    lastdata.acc3 = ui->lineEditAcc3->text();
    WriteLastDataFile();
}


bool AccUi::ReadLastDataFile()
{
    QFile file(".\\AccConfig\\last.xml");

    bool res = false;

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            return false;
        }
        reader.setDevice(&file);
        reader.readNext();

        while (!reader.atEnd())
        {
            if (reader.isStartElement())
            {
                if (reader.name() == "last")
                {
                    res = ReadLastDataElement();
                }
                else
                {
                    reader.readNext();
                }
            }
            else
            {
                reader.readNext();
            }
        }
        file.close();
    }
    catch (QString exception)
    {
        //interlock
        qDebug() << exception << endl;
        return false;
    }
    return res;
}

bool AccUi::ReadLastDataElement()
{
    reader.readNext();
    while (!reader.atEnd())
    {
        reader.readNext();
        if (reader.isEndElement())
            break;

        if (reader.isStartElement())
        {
            while (!reader.isEndElement())
            {
                if (reader.name() == "ray_type")
                    lastdata.ray_type = (RAY_TYPE_T)reader.readElementText().toInt();
                else if(reader.name() == "dose1")
                    lastdata.dose1 = reader.readElementText().toFloat();
                else if(reader.name() == "dose2")
                    lastdata.dose2 = reader.readElementText().toFloat();
                else if(reader.name() == "time")
                    lastdata.time = reader.readElementText().toFloat();
                else if(reader.name() == "rate")
                    lastdata.rate = reader.readElementText().toFloat();
                else if(reader.name() == "jawx1")
                    lastdata.jawx1 = reader.readElementText().toFloat();
                else if(reader.name() == "jawx2")
                    lastdata.jawx2 = reader.readElementText().toFloat();
                else if(reader.name() == "jawy1")
                    lastdata.jawy1 = reader.readElementText().toFloat();
                else if(reader.name() == "jawy2")
                    lastdata.jawy2 = reader.readElementText().toFloat();
                else if(reader.name() == "colli")
                    lastdata.colli = reader.readElementText().toFloat();
                else if(reader.name() == "gpos")
                    lastdata.gpos = reader.readElementText().toFloat();
                else if(reader.name() == "arc")
                    lastdata.arc = reader.readElementText().toFloat();
                else if(reader.name() == "stop")
                    lastdata.stop = reader.readElementText().toFloat();
                else if(reader.name() == "gspeed")
                    lastdata.gspeed = reader.readElementText().toFloat();
                else if(reader.name() == "acc1")
                    lastdata.acc1 = reader.readElementText();
                else if(reader.name() == "acc2")
                    lastdata.acc2 =reader.readElementText();
                else if(reader.name() == "acc3")
                    lastdata.acc3 = reader.readElementText();
                reader.readNext();
            }
        }
    }
    return true;
}


bool AccUi::WriteLastDataFile()
{
    QFile file(".\\AccConfig\\last.xml");

    try
    {
        if (!file.open(QFile::ReadWrite | QFile::Text))
        {
            //interlock
            return false;
        }
        if (!doc.setContent(&file))
        {
            //interlock
            file.close();
            return false;
        }
        file.close();
        root = doc.documentElement();
        while (!root.isNull())
        {
            if (root.tagName() == "last")
            {
                WriteLastDataElement();
                file.open(QFile::WriteOnly | QFile::Truncate);
                QTextStream ts(&file);
                ts.reset();
                ts.setCodec("utf-8");
                doc.save(ts, 4, QDomNode::EncodingFromTextStream);

            }
            root = root.nextSibling().toElement();
        }
        file.close();
        return true;

    }
    catch (QString exception)
    {
        //interlock
        return false;
    }
    return true;
}

void AccUi::WriteLastDataElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "ray_type")
            node.firstChild().setNodeValue(QString::number(lastdata.ray_type));
        else if (node.nodeName() == "dose1")
            node.firstChild().setNodeValue(QString::number(lastdata.dose1));
        else if (node.nodeName() == "dose2")
            node.firstChild().setNodeValue(QString::number(lastdata.dose2));
        else if (node.nodeName() == "time")
            node.firstChild().setNodeValue(QString::number(lastdata.time));
        else if (node.nodeName() == "rate")
            node.firstChild().setNodeValue(QString::number(lastdata.rate));
        else if (node.nodeName() == "jawx1")
            node.firstChild().setNodeValue(QString::number(lastdata.jawx1));
        else if (node.nodeName() == "jawx2")
            node.firstChild().setNodeValue(QString::number(lastdata.jawx2));
        else if (node.nodeName() == "jawy1")
            node.firstChild().setNodeValue(QString::number(lastdata.jawy1));
        else if (node.nodeName() == "jawy2")
            node.firstChild().setNodeValue(QString::number(lastdata.jawy2));
        else if (node.nodeName() == "colli")
            node.firstChild().setNodeValue(QString::number(lastdata.colli));
        else if (node.nodeName() == "gpos")
            node.firstChild().setNodeValue(QString::number(lastdata.gpos));
        else if (node.nodeName() == "arc")
            node.firstChild().setNodeValue(QString::number(lastdata.arc));
        else if (node.nodeName() == "stop")
            node.firstChild().setNodeValue(QString::number(lastdata.stop));
        else if (node.nodeName() == "gspeed")
            node.firstChild().setNodeValue(QString::number(lastdata.gspeed));
        else if (node.nodeName() == "acc1")
            node.firstChild().setNodeValue(lastdata.acc1);
        else if (node.nodeName() == "acc2type")
            node.firstChild().setNodeValue(lastdata.acc2);
        else if (node.nodeName() == "acc3")
            node.firstChild().setNodeValue(lastdata.acc3);

        node = node.nextSibling();
    }
}

void AccUi::ShowLastData()
{
    QString str,tmp;

    switch(lastdata.ray_type)
    {
    case E1:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 05\r");
            break;
        case E2:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 07\r");
            break;
        case E3:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 08\r");
            break;
        case E4:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 10\r");
            break;
        case E5:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 12\r");
            break;
        case E6:
            str.append("Mode : ELTRN                    Technique : FIXED                    Energy : 14\r");
            break;
        case X_LO:
            str.append("Mode : X-RAY                    Technique : FIXED                    Energy : 06\r");
            break;
        case X_HI:
            str.append("Mode : X-RAY                    Technique : FIXED                    Energy : 10\r");
            break;
        case ROT_X_LO:
            str.append("Mode : X-RAY                    Technique : ROT                      Energy : 06\r");
            break;
        case ROT_X_HI:
            str.append("Mode : X-RAY                    Technique : ROT                      Energy : 10\r");
            break;
    default:
        break;
    }

    tmp.sprintf("Preset Dose1 : %04d            Preset Dose2 : %04d\r",(int)lastdata.dose1,(int)lastdata.dose2);
    str.append(tmp);
    tmp.sprintf("Preset Time : %05.2f             Preset Dose Rate : %04d\r",lastdata.time,(int)lastdata.rate);
    str.append(tmp);
    tmp.sprintf("Jaw X1 : %05.2f                    Jaw X2 : %05.2f\r",lastdata.jawx1,lastdata.jawx2);
    str.append(tmp);
    tmp.sprintf("Jaw Y1 : %05.2f                    Jaw Y2 : %05.2f\r",lastdata.jawy1,lastdata.jawy2);
    str.append(tmp);
    tmp.sprintf("Collimator : %06.2f            Gantry Position: %06.2f\r",lastdata.colli,lastdata.gpos);
    str.append(tmp);
    tmp.sprintf("Rot Arc : %06.2f                 Stop Position: %06.2f                Gantry Speed : %03d\r",lastdata.arc,lastdata.stop,(int)lastdata.gspeed);
    str.append(tmp);


    str.append("Accessory 1 : "+lastdata.acc1+"\t\r");
    str.append("Accessory 2 : "+lastdata.acc2+"\t\r");
    str.append("Accessory 3 : "+lastdata.acc3+"\t\r");

    ui->topFrame->hide();
    ui->teTreatData->setText(str);
    ui->teTreatData->setLineWrapColumnOrWidth(3);
    ui->stackedWidget->setCurrentWidget(ui->accReacllData);
    //str.append()
}

void AccUi::RecallLastTreatData()
{
    qDebug()<<"RecallLastTreatData";
    QString str;
    g_ray_type = lastdata.ray_type;
    SendRayModeCommands();
//    dosei->f_preset_dose1 = lastdata.dose1;
//    doseii->f_preset_dose2 = lastdata.dose2;
//    preset_time = lastdata.time;
//    dosei->dose_rate = lastdata.rate;

    ui->lineEditPstDos1->setText(QString::number((int)lastdata.dose1));
    ui->lineEditPstDosRate->setText(QString::number((int)lastdata.rate));
    CheckPstDos1();
    SetPstDos2Time();           //    preset_time = lastdata.time;
    SendPresetDos1();           //    dosei->f_preset_dose1 = lastdata.dose1;
    SendPresetDos2();           //    doseii->f_preset_dose2 = lastdata.dose2;
    SendPresetTimer();

    str.sprintf("%05.2f",lastdata.time);
    ui->lineEditPstTime->setText(str);
    SetPstDosTime();
    SendPresetTimer();

    if(g_ray_type == ROT_X_LO || g_ray_type == ROT_X_HI)
    {
        ShowRotGans();

        str.sprintf("%05.1f", lastdata.gpos);
        ui->lineEditGanPos->setText(str);
        CheckPstGantry();
        SetRotGanStratPos();        //gantry_start_pos = lastdata.gpos;
        SetGanPos();
        gpos_val_set=true;

        str.sprintf("%.1f", lastdata.arc);
        ui->lineEditArc1->setText(str);
        if(!SetRotArc1())           //gantry_rot_arc = lastdata.arc;
        {
            ui->lineEditMsg->setText("Reacll ARC data is error ");
        }
        if(!SetRotGanStopPos())      //gantry_stop_pos = lastdata.stop;
        {
            ui->lineEditMsg->setText("Reacll STOP data is error");;
        }
        SetRotSpeedRateTime();      //gantry_speed = lastdata.gspeed;
    }
    else
    {
//        str.sprintf("%05.1f", lastdata.gpos);
//        ui->lineEditGanPos->setText(str);
//        CheckPstGantry();
//        SetGanPos();

        gantry_rot_arc = 0;
        gantry_stop_pos = 0;
        gantry_speed = 0;
    }

    switch(lastdata.ray_type)
    {
    case E1:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("05");
        break;
    case E2:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("07");
        break;
    case E3:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("08");
        break;
    case E4:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("10");
        break;
    case E5:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("12");
        break;
    case E6:
        ui->lineEditMod->setText(ELTRN_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("14");
        break;
    case X_LO:
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("06");
        break;
    case X_HI:
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(FIX_UI);
        ui->lineEditEnergy->setText("10");
        break;
    case ROT_X_LO:
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(ROT_UI);
        ui->lineEditEnergy->setText("06");
        break;
    case ROT_X_HI:
        ui->lineEditMod->setText(X_RAY_UI);
        ui->lineEditTech->setText(ROT_UI);
        ui->lineEditEnergy->setText("10");
        break;
    default:
        break;
    }

}

void AccUi::SendPresetDos1()
{
    UINT16 dos1;
    bool suc;
    //dos1 = ui->lineEditPstDos1->text().toInt();
    dos1=(int)preset_dose1;
    suc = SlaveBase::CheckValue(dos1, DOS1_PRE_CK);

    if(!suc)
    {
        //erro
        return;
    }
    dosei->f_preset_dose1 = dos1;
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendPresetDos2()
{
    UINT16 dos2;
    bool suc;
    //dos2 = ui->lineEditPstDos2->text().toInt();
    dos2=(int)preset_dose2;
    suc = SlaveBase::CheckValue(dos2, DOS2_PRE_CK);

    if(!suc)
    {
        //erro
        return;
    }
    doseii->f_preset_dose2 = dos2;
    //suc = doseii->SendInformation();
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendPresetTimer()
{
    UINT32 timer_ret;
    bool suc;

    suc = SlaveBase::CheckValue(preset_time, TIMER_PRE_CK, timer_ret);
    if(!suc)
    {
        //erro
        return;
    }
    dosei->preset_treat_time = timer_ret;
    doseii->preset_treat_time = timer_ret;
    //qDebug()<<"Fix doos preset_treat_time="<<dosei->preset_treat_time;
    return;
}

void AccUi::SendJawX()
{
    float jawx;
    UINT32 jawx1_ret, jawx2_ret;
    UINT32 jawx_ret;
    bool suc;
    bool suc1;
    jawx = ui->lineEditJawX->text().toFloat();
    suc = SlaveBase::CheckValue(jawx, JAWX_CK, jawx_ret);

    if(!suc)
    {
        //erro
        return;
    }
    SlaveBase::CheckValue(jawx / 2.0, JAWX1_CK, jawx1_ret);
    SlaveBase::CheckValue(jawx / 2.0, JAWX2_CK, jawx2_ret);

    suc = motor->SendMoveNow(JAW_B2, jawx1_ret);
    suc1 = motor->SendMoveNow(JAW_B1, jawx2_ret);

    jawx1_pst=jawx1_ret;
    jawx2_pst=jawx2_ret;
    qDebug() << "jawx1_pst:" << jawx1_pst << "jawx2_pst" << jawx2_pst << endl;
    if(!(suc && suc1))
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawY()
{
    float jawy;
    UINT32 jawy1_ret, jawy2_ret;
    UINT32 jawy_ret;
    bool suc;
    bool suc1;
    jawy = ui->lineEditJawY->text().toFloat();
    suc = SlaveBase::CheckValue(jawy, JAWY_CK, jawy_ret);
    if(!suc)
    {
        //erro
        return;
    }
    SlaveBase::CheckValue(jawy / 2.0, JAWY1_CK, jawy1_ret);
    SlaveBase::CheckValue(jawy / 2.0, JAWY2_CK, jawy2_ret);

    suc = motor->SendMoveNow(JAW_A1, jawy1_ret);
    suc1 = motor->SendMoveNow(JAW_A2, jawy2_ret);
    jawy1_pst=jawy1_ret;
    jawy2_pst=jawy2_ret;
    if(!(suc && suc1))
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawX(float jawx)
{
    bool suc,suc1;
    UINT32 jawx_ret,jawx1_ret,jawx2_ret;

    suc = SlaveBase::CheckValue(jawx, JAWX_CK, jawx_ret);

    if(!suc)
    {
        //erro
        return;
    }
    SlaveBase::CheckValue(jawx / 2.0, JAWX1_CK, jawx1_ret);
    SlaveBase::CheckValue(jawx / 2.0, JAWX2_CK, jawx2_ret);

    suc = motor->SendMoveNow(JAW_B2, jawx1_ret);
    suc1 = motor->SendMoveNow(JAW_B1, jawx2_ret);
    jawx1_pst=jawx1_ret;
    jawx2_pst=jawx2_ret;
    if(!(suc && suc1))
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawY(float jawy)
{
    UINT32 jawy1_ret, jawy2_ret;
    UINT32 jawy_ret;
    bool suc;
    bool suc1;

    suc = SlaveBase::CheckValue(jawy, JAWY_CK, jawy_ret);
    if(!suc)
    {
        //erro
        return;
    }
    SlaveBase::CheckValue(jawy / 2.0, JAWY1_CK, jawy1_ret);
    SlaveBase::CheckValue(jawy / 2.0, JAWY2_CK, jawy2_ret);

    suc = motor->SendMoveNow(JAW_A1, jawy1_ret);
    suc1 = motor->SendMoveNow(JAW_A2, jawy2_ret);
    jawy1_pst=jawy1_ret;
    jawy2_pst=jawy2_ret;
    if(!(suc && suc1))
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawX1()
{
    float jawx1;
    UINT32 jawx1_ret;
    bool suc;
    jawx1 = -ui->lineEditJawX1->text().toFloat();
    suc = SlaveBase::CheckValue(jawx1, JAWX1_CK, jawx1_ret);
    if(!suc)
    {
        //erro
        return;
    }
    suc = motor->SendMoveNow(JAW_B2, jawx1_ret);
    jawx1_pst=jawx1_ret;     //LJY 16-05-25
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawX2()
{
    float jawx2;
    UINT32 jawx2_ret;
    bool suc;
    jawx2 = ui->lineEditJawX2->text().toFloat();
    suc = SlaveBase::CheckValue(jawx2, JAWX2_CK, jawx2_ret);
    if(!suc)
    {
        //erro
        return;
    }
    suc = motor->SendMoveNow(JAW_B1, jawx2_ret);
    jawx2_pst=jawx2_ret;     //LJY 16-05-25
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawY1()
{
    float jawy1;
    UINT32 jawy1_ret;
    bool suc;
    jawy1 = -ui->lineEditJawY1->text().toFloat();
    suc = SlaveBase::CheckValue(jawy1, JAWY1_CK, jawy1_ret);
    if(!suc)
    {
        //erro
        return;
    }
    suc = motor->SendMoveNow(JAW_A1, jawy1_ret);
    jawy1_pst=jawy1_ret;     //LJY 16-05-25
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendJawY2()
{
    float jawy2;
    UINT32 jawy2_ret;
    bool suc;
    jawy2 = ui->lineEditJawY2->text().toFloat();
    suc = SlaveBase::CheckValue(jawy2, JAWY2_CK, jawy2_ret);
    if(!suc)
    {
        //erro
        return;
    }
    suc = motor->SendMoveNow(JAW_A2, jawy2_ret);
    jawy2_pst=jawy2_ret;     //LJY 16-05-25
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendCollimator()
{
    float colli;
    UINT32 colli_ret;
    bool suc;
    colli = ui->lineEditCollimator->text().toFloat();
    if(colli>180.0) colli=colli-360.0;
    suc = SlaveBase::CheckValue(colli, COLLI_CK, colli_ret);
    if(!suc)
    {
        //erro
        return;
    }
    suc = motor->SendMoveNow(COLLIMATOR, colli_ret);
    colli_pst=colli_ret;     //LJY 16-05-25
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::SendGantryStart()
{
    bool suc;
    suc = motor->SendMoveNow(GANTRY,(UINT16)gantry_pst);
    //qDebug()<<"sendmove gantry_past="<<gantry_pst;
    if (!suc)
    {
        //error
        return;
    }
    return;
}

void AccUi::SendGantryStop()
{
    bool suc;

    suc = motor->SendStop(GANTRY);
    if(!suc)
    {
        //goto error;
        return;
    }
    return;
}

void AccUi::LineeditStopBlink()
{
    ui->lineEditPstDos1->stopBlink();
    ui->lineEditPstDos1->clearFocus();
    ui->lineEditPstDos1->setReadOnly(true);

    ui->lineEditPstDos2->stopBlink();

    ui->lineEditPstTime->stopBlink();
    ui->lineEditPstTime->clearFocus();
    ui->lineEditPstTime->setReadOnly(true);

    ui->lineEditPstDosRate->stopBlink();

    ui->lineEditStatus->stopBlink();

    ui->lineEditEnergy->stopBlink();

    ui->lineEditCollimator->stopBlink();
    ui->lineEditCollimator->clearFocus();
    ui->lineEditCollimator->setReadOnly(true);

    ui->lineEditJawX->stopBlink();
    ui->lineEditJawX->clearFocus();
    ui->lineEditJawX->setReadOnly(true);

    ui->lineEditJawY->stopBlink();
    ui->lineEditJawY->clearFocus();
    ui->lineEditJawY->setReadOnly(true);

    ui->lineEditJawX1->stopBlink();
    ui->lineEditJawX1->clearFocus();
    ui->lineEditJawX1->setReadOnly(true);

    ui->lineEditJawY1->stopBlink();
    ui->lineEditJawY1->clearFocus();
    ui->lineEditJawY1->setReadOnly(true);

    ui->lineEditJawX2->stopBlink();
    ui->lineEditJawX2->clearFocus();
    ui->lineEditJawX2->setReadOnly(true);

    ui->lineEditJawY2->stopBlink();
    ui->lineEditJawY2->clearFocus();
    ui->lineEditJawY2->setReadOnly(true);

    ui->lineEditGanPos->stopBlink();
    ui->lineEditGanPos->clearFocus();
    ui->lineEditGanPos->setReadOnly(true);

    ui->lineEditArc1->stopBlink();
    ui->lineEditArc1->clearFocus();
    ui->lineEditArc1->setReadOnly(true);

}
