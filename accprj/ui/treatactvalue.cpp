#include "accui.h"
#include "ui_accui.h"

#define ACT_PST_MAX_DELTA 1

//called by long
void AccUi::SetActDos1(UINT32 val)
{
    QString temp;
    temp.sprintf("%04d", val);
    ui->lineEditDos1->setText(temp);
}

void AccUi::SetActDos2(unsigned int val)
{
    QString temp;
    temp.sprintf("%04d", val);
    ui->lineEditDos2->setText(temp);
}

void AccUi::SetActDosTime(float val)
{
    QString temp;
    temp.sprintf("%05.2f", val);
    ui->lineEditDosTime->setText(temp);
}

void AccUi::SetActDosRate(unsigned int val)
{
    QString temp;
    temp.sprintf("%04d", val);
    ui->lineEditDosRate->setText(temp);
}

void AccUi::ShowActJawColliGan(int type)
{
    //float x1,x2,y1,y2,colli,x,y,gpos;
    QString str;
    float tmp_colli;
    int tmp;
    switch(type)
    {
    case 0:     //jaw+colli
//        if(g_il_state == IL_NULL)      //2016-8-17
//        {
//            jawx1_pst = motor->position[JAW_B2];
//            jawx2_pst = motor->position[JAW_B1];
//            jawy1_pst = motor->position[JAW_A1];
//            jawy2_pst = motor->position[JAW_A2];
//            colli_pst = motor->position[COLLIMATOR];
//        }

        if(g_il_state == IL_NULL || hand->hand_jawx1_control){
            jawx1_pst = motor->position[JAW_B2];
            motor->move_target_speed[JAW_B2].target_position = jawx1_pst;
        }
        if(g_il_state == IL_NULL || hand->hand_jawx2_control){
            jawx2_pst = motor->position[JAW_B1];
            motor->move_target_speed[JAW_B1].target_position = jawx2_pst;
        }

        if(g_il_state == IL_NULL || hand->hand_jawy1_control){
            jawy1_pst = motor->position[JAW_A1];
            motor->move_target_speed[JAW_A1].target_position = jawy1_pst;
        }
        if(g_il_state == IL_NULL || hand->hand_jawy2_control){
            jawy2_pst = motor->position[JAW_A2];
            motor->move_target_speed[JAW_A2].target_position = jawy2_pst;
        }
        if(g_il_state == IL_NULL || hand->hand_colli_control){
            colli_pst = motor->position[COLLIMATOR];
            motor->move_target_speed[COLLIMATOR].target_position = colli_pst;
        }

        jawy1_act_pos = base->ConvertMemberToFloat(motor->position[JAW_A1], JAWY1_CK);
        jawy2_act_pos = base->ConvertMemberToFloat(motor->position[JAW_A2], JAWY2_CK);
        jawx2_act_pos = base->ConvertMemberToFloat(motor->position[JAW_B1], JAWX2_CK);
        jawx1_act_pos = base->ConvertMemberToFloat(motor->position[JAW_B2], JAWX1_CK);
        colli_act_pos = base->ConvertMemberToFloat(motor->position[COLLIMATOR], COLLI_CK);
        jawx_act_pos=qAbs(jawx1_act_pos+jawx2_act_pos);
        jawy_act_pos=qAbs(jawy1_act_pos+jawy2_act_pos);
        tmp=int (colli_act_pos*10);
        colli_act_pos=((float) tmp)/10.0f;
        if (colli_act_pos < 0.0)
            tmp_colli = colli_act_pos + 360.0;
        else
            tmp_colli = colli_act_pos;
        SetActJawX1(-jawx1_act_pos);
        qDebug() << "jawx1_act_pos" << -jawx1_act_pos << endl;
        SetActJawX2(jawx2_act_pos);
        qDebug() << "jawx2_act_pos" << jawx2_act_pos << endl;
        SetActJawY1(-jawy1_act_pos);
        qDebug() << "jawy1_act_pos" << -jawy1_act_pos << endl;
        SetActJawY2(jawy2_act_pos);
        qDebug() << "jawy2_act_pos" << jawy2_act_pos << endl;
        SetActCollimat(tmp_colli);
        qDebug() << "tmp_colli" << tmp_colli << endl;

        //06-16
        if(focusWidget() != ui->lineEditJawX)
        {
            str.sprintf("%04.1f", jawx_act_pos);
            ui->lineEditJawX->setText(str);

            if (move_status.jawx)
            {
                float jawx_pst = base->ConvertMemberToFloat(jawx1_pst, JAWX1_CK)+
                        base->ConvertMemberToFloat(jawx2_pst, JAWX2_CK);
                if (!checkActPstMatch(jawx_act_pos, jawx_pst, ACT_PST_MAX_DELTA))
                {
                    ui->lineEditJawX->setStyleSheet("border: 2px solid white;");
                }
                else
                {
                    ui->lineEditJawX->setStyleSheet("border: 2px solid yellow;");
                }

                ui->lineEditJawX->update();
            }
        }
        if(focusWidget() != ui->lineEditJawY)
        {
            str.sprintf("%04.1f", jawy_act_pos);
            ui->lineEditJawY->setText(str);

            if (move_status.jawy)
            {
                float jawy_pst = base->ConvertMemberToFloat(jawy1_pst, JAWY1_CK)+
                        base->ConvertMemberToFloat(jawy2_pst, JAWY2_CK);
                if (!checkActPstMatch(jawy_act_pos, jawy_pst, ACT_PST_MAX_DELTA))
                {
                    ui->lineEditJawY->setStyleSheet("border: 2px solid white;");
                }
                else
                {
                    ui->lineEditJawY->setStyleSheet("border: 2px solid yellow;");
                }

                ui->lineEditJawY->update();
            }
        }
        break;
    case 1:     //gantry
        //2016-8-17
        if(g_il_state == IL_NULL  || hand->hand_gantry_control)        //LJY 16-05-25
        {
            gantry_pst=motor->gantry_position_speed.gantry_position;
            motor->move_target_speed[GANTRY].target_position = gantry_pst;
        }

        gantry_act_pos = base->ConvertMemberToFloat(motor->gantry_position_speed.gantry_position, GANTRY_CK);
        tmp=int (gantry_act_pos*10);
        gantry_act_pos=((float) tmp)/10;
        if (gantry_act_pos < 0.0)    // gpos is -185 ~ +185
            act_gantry_pos = gantry_act_pos + 360.0;    // act_gantry_pos is 0 ~ +360
        else
            act_gantry_pos = gantry_act_pos;
        SetActGanPos(act_gantry_pos);
        break;
    default:
        break;
    }
}

void AccUi::ShowActDoseTimeMU()
{
    float tmp;
    QString str;

    tmp = dosei->ConvertDose(dosei->dose1_act_data);
    if(tmp > 9999)
        dosei->InsertInterlock(62, 870, ERROR_IL, IL_ALL, LATCHED);
    str.sprintf("%04d", (UINT32)tmp);
    ui->lineEditDos1->setText(str);

    if (g_status == RAD_ON)
    {
        upper->SetActDose((UINT32)tmp);
        mlc->SetActDose((UINT32)tmp);
        ui->lbDoseProgress->UpdateProgress((UINT32)tmp);
    }

    tmp = doseii->ConvertDose(doseii->dose2_act_data);
    if(tmp > 9999)
        doseii->InsertInterlock(62, 871, ERROR_IL, IL_ALL, LATCHED);
    str.sprintf("%04d", (UINT32)tmp);
    ui->lineEditDos2->setText(str);

    tmp = dosei->ConvertMemberToFloat(dosei->treatment_time, TIMER_PRE_CK);
    str.sprintf("%05.2f", tmp);
    ui->lineEditDosTime->setText(str);

    tmp = dosei->ConvertDoserate(dosei->dose1_aver_rate);
    str.sprintf("%04d", (UINT32)tmp);
    ui->lineEditDosRate->setText(str);
}

void AccUi::SetActCollimat(float val)
{
    QString temp;
    temp.sprintf("%05.1f", val);
    ui->lineEditCollimator->setText(temp);
    ui->leMotAdjColliAct->setText(temp);
    temp.sprintf("%x", motor->position[COLLIMATOR]);
    ui->leMotClbColliCurCount->setText(temp);

    //upper->SetColliPos(val);
    if (move_status.colli)
    {
        if (!checkActPstMatch(val, collimat_pst, ACT_PST_MAX_DELTA))
        {
            ui->lineEditCollimator->setStyleSheet("border: 2px solid white;");
        }
        else
        {
            ui->lineEditCollimator->setStyleSheet("border: 2px solid yellow;");
        }

        ui->lineEditCollimator->update();
    }
}

void AccUi::SetActJawX(float x1, float x2)
{
    float x;
    x = x1 + x2;
    QString temp;
    if(x > 0)
        temp.sprintf("%04.1f", x);
    else
        temp.sprintf("%05.1f", x);
    ui->lineEditJawX->setText(temp);
}

void AccUi::SetActJawY(float y1, float y2)
{
    float y;
    y = y1 + y2;
    QString temp;
    if(y > 0)
        temp.sprintf("%04.1f", y);
    else
        temp.sprintf("%05.1f", y);
    ui->lineEditJawY->setText(temp);
}

void AccUi::SetActJawX1(float val)
{
    QString temp;

    if(val<=-0.1)
        temp.sprintf("%05.1f", val);
    else if(val<0.1)                //-0.1<x<0.1
    {
        val=0;
        temp.sprintf("%04.1f", val);
    }
    else
        temp.sprintf("+%04.1f", val);

    if(show_jawx1x2y1y2 && (focusWidget() != ui->lineEditJawX1) )
    {
        if (move_status.jawx1)
        {
            if (!checkActPstMatch(val, base->ConvertMemberToFloat(jawx1_pst, JAWX1_CK), ACT_PST_MAX_DELTA))
            {
                ui->lineEditJawX1->setStyleSheet("border: 2px solid white;");
            }
            else
            {
                ui->lineEditJawX1->setStyleSheet("border: 2px solid yellow;");
            }
        }
        else
        {
            ui->lineEditJawX1->setStyleSheet("border: 2px solid yellow;");
        }
        ui->lineEditJawX1->setText(temp);
    }
    ui->leMotAdjJawX1Act->setText(temp);

    temp.sprintf("%x", motor->position[JAW_B2]);
    ui->leMotClbJawX1CurCount->setText(temp);

    //upper->SetX1Pos(val);
}

void AccUi::SetActJawX2(float val)
{
    QString temp;

    if(val<=-0.1)
        temp.sprintf("%05.1f", val);
    else if(val<0.1)                //-0.1<x<0.1
    {
        val=0;
        temp.sprintf("%04.1f", val);
    }
    else
        temp.sprintf("+%04.1f", val);

    if(show_jawx1x2y1y2 && (focusWidget() != ui->lineEditJawX2) )
    {
        if (move_status.jawx2)
        {
            if (!checkActPstMatch(val, base->ConvertMemberToFloat(jawx2_pst, JAWX2_CK), ACT_PST_MAX_DELTA))
            {
                ui->lineEditJawX2->setStyleSheet("border: 2px solid white;");
            }
            else
            {
                ui->lineEditJawX2->setStyleSheet("border: 2px solid yellow;");
            }
        }
        else
        {
            ui->lineEditJawX2->setStyleSheet("border: 2px solid yellow;");
        }
        ui->lineEditJawX2->setText(temp);
    }
    ui->leMotAdjJawX2Act->setText(temp);

    temp.sprintf("%x", motor->position[JAW_B1]);
    ui->leMotClbJawX2CurCount->setText(temp);

    //upper->SetX2Pos(val);
}

void AccUi::SetActJawY1(float val)
{
    QString temp;

    if(val<=-0.1)
        temp.sprintf("%05.1f", val);
    else if(val<0.1)                //-0.1<x<0.1
    {
        val=0;
        temp.sprintf("%04.1f", val);
    }
    else
        temp.sprintf("+%04.1f", val);

    if(show_jawx1x2y1y2 && (focusWidget() != ui->lineEditJawY1))
    {
        if (move_status.jawy1)
        {
            if (!checkActPstMatch(val, base->ConvertMemberToFloat(jawy1_pst, JAWY1_CK), ACT_PST_MAX_DELTA))
            {
                ui->lineEditJawY1->setStyleSheet("border: 2px solid white;");
            }
            else
            {
                ui->lineEditJawY1->setStyleSheet("border: 2px solid yellow;");
            }
        }
        else
        {
            ui->lineEditJawY1->setStyleSheet("border: 2px solid yellow;");
        }
        ui->lineEditJawY1->setText(temp);
    }
    ui->leMotAdjJawY1Act->setText(temp);

    temp.sprintf("%x", motor->position[JAW_A1]);
    ui->leMotClbJawY1CurCount->setText(temp);

    //upper->SetY1Pos(val);
}

void AccUi::SetActJawY2(float val)
{
    QString temp;

    if(val<=-0.1)
        temp.sprintf("%05.1f", val);
    else if(val<0.1)                //-0.1<x<0.1
    {
        val=0;
        temp.sprintf("%04.1f", val);
    }
    else
        temp.sprintf("+%04.1f", val);

    if(show_jawx1x2y1y2 && (focusWidget() != ui->lineEditJawY2))
    {
        if (move_status.jawy2)
        {
            if (!checkActPstMatch(val, base->ConvertMemberToFloat(jawy2_pst, JAWY2_CK), ACT_PST_MAX_DELTA))
            {
                ui->lineEditJawY2->setStyleSheet("border: 2px solid white;");
            }
            else
            {
                ui->lineEditJawY2->setStyleSheet("border: 2px solid yellow;");
            }

            ui->lineEditJawY2->update();
        }
        else
        {
            ui->lineEditJawY2->setStyleSheet("border: 2px solid yellow;");
        }
        ui->lineEditJawY2->setText(temp);
    }
    ui->leMotAdjJawY2Act->setText(temp);

    temp.sprintf("%x", motor->position[JAW_A2]);
    ui->leMotClbJawY2CurCount->setText(temp);

    //upper->SetY2Pos(val);
}

void AccUi::SetActGanPos(float val)
{
    QString temp;

    act_gantry_pos = val;
    temp.sprintf("%05.1f", val);
    ui->lineEditGanPos->setText(temp);
    ui->leMotAdjGantryAct->setText(temp);
    temp.sprintf("%x",motor->gantry_position_speed.gantry_position);
    ui->leMotClbGantryCurCount->setText(temp);

    //upper->SetGantryPos(val);
}

void AccUi::CurrentPositAsTarget()
{
    jawx1_pst = motor->position[JAW_B2];
    motor->move_target_speed[JAW_B2].target_position = jawx1_pst;
    jawx2_pst = motor->position[JAW_B1];
    motor->move_target_speed[JAW_B1].target_position = jawx2_pst;
    jawy1_pst = motor->position[JAW_A1];
    motor->move_target_speed[JAW_A1].target_position = jawy1_pst;
    jawy2_pst = motor->position[JAW_A2];
    motor->move_target_speed[JAW_A2].target_position = jawy2_pst;
    colli_pst = motor->position[COLLIMATOR];
    motor->move_target_speed[COLLIMATOR].target_position = colli_pst;
    gantry_pst=motor->gantry_position_speed.gantry_position;
    motor->move_target_speed[GANTRY].target_position = gantry_pst;
}


