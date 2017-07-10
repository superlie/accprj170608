#include "accui.h"
#include "ui_accui.h"
#include "windows.h"
#include <QPalette>
#include "tools/cfloatcompare.h"

extern UINT32 g_right_key;
extern UINT32 g_left_key;
extern bool g_check2_bypass;

bool AccUi::Shutdown()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    //获取进程标志
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return false;

    //获取关机特权的LUID
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,    &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    //获取这个进程的关机特权
    AdjustTokenPrivileges(hToken, false, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);
    if (GetLastError() != ERROR_SUCCESS) return false;

    // 强制关闭计算机
    if ( !ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0))
        return false;
    return true;
}

void AccUi::ShowFuncClear()
{
    ui->lineEditF1->setText("");
    ui->lineEditF2->setText("");
    ui->lineEditF3->setText("");
    ui->lineEditF4->setText("");
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText("");
}

void AccUi::ShowFuncRay()
{
    ui->lineEditF1->setText(F1_X_FIX_L);
    ui->lineEditF2->setText(F2_X_FIX_H);
    ui->lineEditF3->setText(F3_E_FIX);
    ui->lineEditF4->setText(F4_ROT);
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText(F6_SETUP);
}

void AccUi::ShowFuncPara()
{
    ui->lineEditF1->setText(F1_ENERGY);
    ui->lineEditF2->setText(F2_MU);

    if(hand->hand_gantry_control)
        ui->lineEditF3->setText("");
    else
        ui->lineEditF3->setText(F3_GANTRY);

    ui->lineEditF4->setText(F4_MU_MIN);
    ui->lineEditF5->setText(F5_TIME);
    ui->lineEditF6->setText(F6_SETUP);
}

void AccUi::ShowFuncJawXY()
{
    if(hand->hand_jawx1_control || hand->hand_jawx2_control || io->io_jawxy_control)
        ui->lineEditF1->setText("");
    else
        ui->lineEditF1->setText(F1_X_JAWS);

    if(hand->hand_jawy1_control || hand->hand_jawy2_control || io->io_jawxy_control)
        ui->lineEditF2->setText("");
    else
        ui->lineEditF2->setText(F2_Y_JAWS);

    ui->lineEditF3->setText("");
    ui->lineEditF4->setText("");
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText(F6_PRV_MNU);
}

void AccUi::ShowFuncY1Y2()
{
    ui->lineEditF1->setText("");
    ui->lineEditF2->setText("");
    if(hand->hand_jawy1_control || io->io_jawxy_control)
        ui->lineEditF3->setText("");
    else
        ui->lineEditF3->setText(F3_Y1);


    if(hand->hand_jawy2_control || io->io_jawxy_control)
        ui->lineEditF4->setText("");
    else
        ui->lineEditF4->setText(F4_Y2);

    ui->lineEditF5->setText("");
    ui->lineEditF6->setText(F6_PRV_MNU);
}

void AccUi::ShowFuncX1X2Y1Y2()
{
    if(hand->hand_jawx1_control || io->io_jawxy_control)
        ui->lineEditF1->setText("");
    else
        ui->lineEditF1->setText(F1_X1);

    if(hand->hand_jawx2_control || io->io_jawxy_control)
        ui->lineEditF2->setText("");
    else
        ui->lineEditF2->setText(F2_X2);

    if(hand->hand_jawy1_control || io->io_jawxy_control)
        ui->lineEditF3->setText("");
    else
        ui->lineEditF3->setText(F3_Y1);

    if(hand->hand_jawy2_control || io->io_jawxy_control)
        ui->lineEditF4->setText("");
    else
        ui->lineEditF4->setText(F4_Y2);

    ui->lineEditF5->setText("");
    ui->lineEditF6->setText(F6_PRV_MNU);
}

void AccUi::ShowFuncMode()
{
    ShowFuncFrame();
    ui->lineEditF1->setText(F1_TERAT);
    ui->lineEditF2->setText("");
    ui->lineEditF3->setText(F3_SERVICE);
    ui->lineEditF4->setText(F4_DIAGS);
    ui->lineEditF5->setText(F5_RCL_DAT);
    ui->lineEditF6->setText(F6_SHUTDOWN);
}

void AccUi::ShowFuncFrame()
{
    ui->lineEditF1->setFrame(true);
    ui->lineEditF1->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditF2->setFrame(true);
    ui->lineEditF2->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditF3->setFrame(true);
    ui->lineEditF3->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditF4->setFrame(true);
    ui->lineEditF4->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditF5->setFrame(true);
    ui->lineEditF5->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditF6->setFrame(true);
    ui->lineEditF6->setStyleSheet("border: 2px solid yellow;");
}

void AccUi::ShowJawX1X2()
{
    ui->labelX1->setStyleSheet("color:white");
    ui->labelX2->setStyleSheet("color:white");

    ui->labelX1->setText("X1");
    ui->labelX2->setText("X2");

    ui->lineEditJawX1->setFrame(true);
    ui->lineEditJawX2->setFrame(true);

    ui->lineEditJawX1->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditJawX2->setStyleSheet("border: 2px solid yellow;");
}

void AccUi::ShowJawY1Y2()
{
    ui->labelY1->setStyleSheet("color:white");
    ui->labelY2->setStyleSheet("color:white");

    ui->labelY1->setText("Y1");
    ui->labelY2->setText("Y2");

    ui->lineEditJawY1->setFrame(true);
    ui->lineEditJawY2->setFrame(true);

    ui->lineEditJawY1->setStyleSheet("border: 2px solid yellow;");
    ui->lineEditJawY2->setStyleSheet("border: 2px solid yellow;");
}

void AccUi::HideJawX1X2()
{
    ui->labelX1->setStyleSheet("color:black");
    ui->labelX1->setText("");
    ui->lineEditJawX1->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawX1->setFrame(false);
    ui->lineEditJawX1->setText("");

    ui->labelX2->setStyleSheet("color:black");
    ui->labelX2->setText("");
    ui->lineEditJawX2->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawX2->setFrame(false);
    ui->lineEditJawX2->setText("");
    ui->svcTreat->update();
}

void AccUi::HideJawY1Y2()
{
    ui->labelY1->setStyleSheet("color:black");
    ui->labelY1->setText("");
    ui->lineEditJawY1->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawY1->setFrame(false);
    ui->lineEditJawY1->setText("");

    ui->labelY2->setStyleSheet("color:black");
    ui->labelY2->setText("");
    ui->lineEditJawY2->setStyleSheet("border: 2px solid black;");
    ui->lineEditJawY2->setFrame(false);
    ui->lineEditJawY2->setText("");
    ui->svcTreat->update();
}

void AccUi::ShowRotGans()
{
    ui->labelGanSpeed->setText(G_SPEED_UI);
    ui->lineEditGanSpeed->setFrame(true);
    ui->lineEditGanSpeed->setText("");

    ui->labelGanDirec->setText(G_DIREC_UI);
    ui->lineEditGanDirec->setStyleSheet("background:white;color:black");
    ui->lineEditGanDirec->setFrame(true);
    ui->lineEditGanDirec->setText("CW");

    ui->labelArc1->setText(ARC_UI);
    ui->lineEditArc1->setStyleSheet("background:white;color:black");
    ui->lineEditArc1->setFrame(true);
    ui->lineEditArc1->setText("");

    ui->labelStop1->setText(STOP_UI);
    ui->lineEditStop1->setStyleSheet("background:white;color:black");
    ui->lineEditStop1->setFrame(true);
    ui->lineEditStop1->setText("");
}



//Ljy-8-6 初始化 Foil Select UI
void AccUi::ShowFoilSelect()
{
    //
    ui->leFoilSelectCurrElecEgy1->setText(QString::number(SetFoilSelectNumber(E1)));
    ui->leFoilSelectCurrElecEgy2->setText(QString::number(SetFoilSelectNumber(E2)));
    ui->leFoilSelectCurrElecEgy3->setText(QString::number(SetFoilSelectNumber(E3)));
    ui->leFoilSelectCurrElecEgy4->setText(QString::number(SetFoilSelectNumber(E4)));
    ui->leFoilSelectCurrElecEgy5->setText(QString::number(SetFoilSelectNumber(E5)));
    ui->leFoilSelectCurrElecEgy6->setText(QString::number(SetFoilSelectNumber(E6)));

    ui->lbElecEgy2NewFoil->setText("");
    ui->lbElecEgy3NewFoil->setText("");
    ui->lbElecEgy4NewFoil->setText("");
    ui->lbElecEgy5NewFoil->setText("");
    ui->lbElecEgy6NewFoil->setText("");

    ui->leFoilSelectNewElecEgy1->setText("");
    ui->leFoilSelectNewElecEgy2->setText("");
    ui->leFoilSelectNewElecEgy3->setText("");
    ui->leFoilSelectNewElecEgy4->setText("");
    ui->leFoilSelectNewElecEgy5->setText("");
    ui->leFoilSelectNewElecEgy6->setText("");

    ui->leFoilSelectNewElecEgy1->setReadOnly(false);
    ui->leFoilSelectNewElecEgy1->setFocus();


    ui->leFoilSelectNewElecEgy2->setReadOnly(true);
    ui->leFoilSelectNewElecEgy3->setReadOnly(true);
    ui->leFoilSelectNewElecEgy4->setReadOnly(true);
    ui->leFoilSelectNewElecEgy5->setReadOnly(true);
    ui->leFoilSelectNewElecEgy6->setReadOnly(true);
}

void AccUi::ShowMotorAdjust()
{
    float tmp;
    ui->leMotAdjJawX1SS->setText(QString::number(motor->slow_speed[JAW_B2]));
    ui->leMotAdjJawX1FS->setText(QString::number(motor->fast_speed[JAW_B2]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_B2],JAWX1_SDS_RSCH_CK);
    ui->leMotAdjJawX1SDS->setText(QString::number((int)tmp));
    ui->leMotAdjJawX1SDD->setText(QString::number(motor->slow_down_distance[JAW_B2]));

    ui->leMotAdjJawX2SS->setText(QString::number(motor->slow_speed[JAW_B1]));
    ui->leMotAdjJawX2FS->setText(QString::number(motor->fast_speed[JAW_B1]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_B1],JAWX2_SDS_RSCH_CK);
    ui->leMotAdjJawX2SDS->setText(QString::number((int)tmp));
    ui->leMotAdjJawX2SDD->setText(QString::number(motor->slow_down_distance[JAW_B1]));

    ui->leMotAdjJawY1SS->setText(QString::number(motor->slow_speed[JAW_A1]));
    ui->leMotAdjJawY1FS->setText(QString::number(motor->fast_speed[JAW_A1]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_A1],JAWY1_SDS_RSCH_CK);
    ui->leMotAdjJawY1SDS->setText(QString::number((int)tmp));
    ui->leMotAdjJawY1SDD->setText(QString::number(motor->slow_down_distance[JAW_A1]));

    ui->leMotAdjJawY2SS->setText(QString::number(motor->slow_speed[JAW_A2]));
    ui->leMotAdjJawY2FS->setText(QString::number(motor->fast_speed[JAW_A2]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[JAW_A2],JAWY2_SDS_RSCH_CK);
    ui->leMotAdjJawY2SDS->setText(QString::number((int)tmp));
    ui->leMotAdjJawY2SDD->setText(QString::number(motor->slow_down_distance[JAW_A2]));

    ui->leMotAdjColliSS->setText(QString::number(motor->slow_speed[COLLIMATOR]));
    ui->leMotAdjColliFS->setText(QString::number(motor->fast_speed[COLLIMATOR]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[COLLIMATOR],COLLI_SDS_RSCH_CK);
    ui->leMotAdjColliSDS->setText(QString::number((int)tmp));
    ui->leMotAdjColliSDD->setText(QString::number(motor->slow_down_distance[COLLIMATOR]));

    ui->leMotAdjGantrySS->setText(QString::number(motor->slow_speed[GANTRY]));
    ui->leMotAdjGantryFS->setText(QString::number(motor->fast_speed[GANTRY]));
    tmp = SlaveBase::ConvertMemberToFloat(motor->slow_down_speed[GANTRY],GANTRY_SDS_RSCH_CK);
    ui->leMotAdjGantrySDS->setText(QString::number((int)tmp));
    ui->leMotAdjGantrySDD->setText(QString::number(motor->slow_down_distance[GANTRY]));

    ui->lbMotorAdjC->hide();
    ui->lbMotorAdjI->hide();
    ui->lbMotorAdjT->hide();
    ui->lbMotorAdjU->hide();
    ui->lbMotorAdjV->hide();
    ui->lbMotorAdjX->hide();
    ui->lbMotorAdjY->hide();
}

void AccUi::ShowMotorCalibFocus()
{
    int index = ui->sWMotorCalib->currentIndex();
    QString style = "background:white;color:black";
    QString style2 = "background:black;";

    switch (index) {
    case 0:
        if(ui->leMotClbGantryRefPosn2->hasFocus())
        {
            ui->leMotClbGantryRefPosn1->setStyleSheet(style2);
            ui->leMotClbGantryCapCount1->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn1->setStyleSheet(style2);

            ui->leMotClbGantryRefPosn2->setStyleSheet(style);
            ui->leMotClbGantryCapCount2->setStyleSheet(style);
            ui->leMotClbGantryCapPosn2->setStyleSheet(style);

            ui->leMotClbGantryRefPosn3->setStyleSheet(style2);
            ui->leMotClbGantryCapCount3->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn3->setStyleSheet(style2);
        }
        else if(ui->leMotClbGantryRefPosn3->hasFocus())
        {
            ui->leMotClbGantryRefPosn1->setStyleSheet(style2);
            ui->leMotClbGantryCapCount1->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn1->setStyleSheet(style2);

            ui->leMotClbGantryRefPosn2->setStyleSheet(style2);
            ui->leMotClbGantryCapCount2->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn2->setStyleSheet(style2);

            ui->leMotClbGantryRefPosn3->setStyleSheet(style);
            ui->leMotClbGantryCapCount3->setStyleSheet(style);
            ui->leMotClbGantryCapPosn3->setStyleSheet(style);
        }
        else
        {
            ui->leMotClbGantryRefPosn1->setFocus();

            ui->leMotClbGantryRefPosn1->setStyleSheet(style);
            ui->leMotClbGantryCapCount1->setStyleSheet(style);
            ui->leMotClbGantryCapPosn1->setStyleSheet(style);

            ui->leMotClbGantryRefPosn2->setStyleSheet(style2);
            ui->leMotClbGantryCapCount2->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn2->setStyleSheet(style2);

            ui->leMotClbGantryRefPosn3->setStyleSheet(style2);
            ui->leMotClbGantryCapCount3->setStyleSheet(style2);
            ui->leMotClbGantryCapPosn3->setStyleSheet(style2);
        }
        break;
    case 1:
        if(ui->leMotClbJawY1ReqPosn2->hasFocus())
        {
            ui->leMotClbJawY1ReqPosn1->setStyleSheet(style2);
            ui->leMotClbJawY1CapCount1->setStyleSheet(style2);
            ui->leMotClbJawY1MsuPosn1->setStyleSheet(style2);

            ui->leMotClbJawY1ReqPosn2->setStyleSheet(style);
            ui->leMotClbJawY1CapCount2->setStyleSheet(style);
            ui->leMotClbJawY1MsuPosn2->setStyleSheet(style);
            ui->leMotClbJawY1ReqPosn2->selectAll();
        }
        else
        {
            ui->leMotClbJawY1ReqPosn1->setFocus();
            ui->leMotClbJawY1ReqPosn1->selectAll();

            ui->leMotClbJawY1ReqPosn1->setStyleSheet(style);
            ui->leMotClbJawY1CapCount1->setStyleSheet(style);
            ui->leMotClbJawY1MsuPosn1->setStyleSheet(style);

            ui->leMotClbJawY1ReqPosn2->setStyleSheet(style2);
            ui->leMotClbJawY1CapCount2->setStyleSheet(style2);
            ui->leMotClbJawY1MsuPosn2->setStyleSheet(style2);
        }
        break;
    case 2:
        if(ui->leMotClbJawY2ReqPosn2->hasFocus())
        {
            ui->leMotClbJawY2ReqPosn1->setStyleSheet(style2);
            ui->leMotClbJawY2CapCount1->setStyleSheet(style2);
            ui->leMotClbJawY2MsuPosn1->setStyleSheet(style2);

            ui->leMotClbJawY2ReqPosn2->setStyleSheet(style);
            ui->leMotClbJawY2CapCount2->setStyleSheet(style);
            ui->leMotClbJawY2MsuPosn2->setStyleSheet(style);
            ui->leMotClbJawY2ReqPosn2->selectAll();
        }
        else
        {
            ui->leMotClbJawY2ReqPosn1->setFocus();
            ui->leMotClbJawY2ReqPosn1->selectAll();

            ui->leMotClbJawY2ReqPosn1->setStyleSheet(style);
            ui->leMotClbJawY2CapCount1->setStyleSheet(style);
            ui->leMotClbJawY2MsuPosn1->setStyleSheet(style);

            ui->leMotClbJawY2ReqPosn2->setStyleSheet(style2);
            ui->leMotClbJawY2CapCount2->setStyleSheet(style2);
            ui->leMotClbJawY2MsuPosn2->setStyleSheet(style2);
        }
        break;
    case 3:
        if(ui->leMotClbJawX2ReqPosn2->hasFocus())
        {
            ui->leMotClbJawX2ReqPosn1->setStyleSheet(style2);
            ui->leMotClbJawX2CapCount1->setStyleSheet(style2);
            ui->leMotClbJawX2MsuPosn1->setStyleSheet(style2);

            ui->leMotClbJawX2ReqPosn2->setStyleSheet(style);
            ui->leMotClbJawX2CapCount2->setStyleSheet(style);
            ui->leMotClbJawX2MsuPosn2->setStyleSheet(style);
            ui->leMotClbJawX2ReqPosn2->selectAll();
        }
        else
        {
            ui->leMotClbJawX2ReqPosn1->setFocus();
            ui->leMotClbJawX2ReqPosn1->selectAll();

            ui->leMotClbJawX2ReqPosn1->setStyleSheet(style);
            ui->leMotClbJawX2CapCount1->setStyleSheet(style);
            ui->leMotClbJawX2MsuPosn1->setStyleSheet(style);

            ui->leMotClbJawX2ReqPosn2->setStyleSheet(style2);
            ui->leMotClbJawX2CapCount2->setStyleSheet(style2);
            ui->leMotClbJawX2MsuPosn2->setStyleSheet(style2);
        }
        break;
    case 4:
        if(ui->leMotClbJawX1ReqPosn2->hasFocus())
        {
            ui->leMotClbJawX1ReqPosn1->setStyleSheet(style2);
            ui->leMotClbJawX1CapCount1->setStyleSheet(style2);
            ui->leMotClbJawX1MsuPosn1->setStyleSheet(style2);

            ui->leMotClbJawX1ReqPosn2->setStyleSheet(style);
            ui->leMotClbJawX1CapCount2->setStyleSheet(style);
            ui->leMotClbJawX1MsuPosn2->setStyleSheet(style);
            ui->leMotClbJawX1ReqPosn2->selectAll();
        }
        else
        {
            ui->leMotClbJawX1ReqPosn1->setFocus();
            ui->leMotClbJawX1ReqPosn1->selectAll();

            ui->leMotClbJawX1ReqPosn1->setStyleSheet(style);
            ui->leMotClbJawX1CapCount1->setStyleSheet(style);
            ui->leMotClbJawX1MsuPosn1->setStyleSheet(style);

            ui->leMotClbJawX1ReqPosn2->setStyleSheet(style2);
            ui->leMotClbJawX1CapCount2->setStyleSheet(style2);
            ui->leMotClbJawX1MsuPosn2->setStyleSheet(style2);
        }
        break;
    case 5:
        if(ui->leMotClbColliRefPosn2->hasFocus())
        {
            ui->leMotClbColliRefPosn1->setStyleSheet(style2);
            ui->leMotClbColliCapCount1->setStyleSheet(style2);
            ui->leMotClbColliCapPosn1->setStyleSheet(style2);

            ui->leMotClbColliRefPosn2->setStyleSheet(style);
            ui->leMotClbColliCapCount2->setStyleSheet(style);
            ui->leMotClbColliCapPosn2->setStyleSheet(style);

            ui->leMotClbColliRefPosn3->setStyleSheet(style2);
            ui->leMotClbColliCapCount3->setStyleSheet(style2);
            ui->leMotClbColliCapPosn3->setStyleSheet(style2);
        }
        else if(ui->leMotClbColliRefPosn3->hasFocus())
        {
            ui->leMotClbColliRefPosn1->setStyleSheet(style2);
            ui->leMotClbColliCapCount1->setStyleSheet(style2);
            ui->leMotClbColliCapPosn1->setStyleSheet(style2);

            ui->leMotClbColliRefPosn2->setStyleSheet(style2);
            ui->leMotClbColliCapCount2->setStyleSheet(style2);
            ui->leMotClbColliCapPosn2->setStyleSheet(style2);

            ui->leMotClbColliRefPosn3->setStyleSheet(style);
            ui->leMotClbColliCapCount3->setStyleSheet(style);
            ui->leMotClbColliCapPosn3->setStyleSheet(style);
        }
        else
        {
            ui->leMotClbColliRefPosn1->setFocus();

            ui->leMotClbColliRefPosn1->setStyleSheet(style);
            ui->leMotClbColliCapCount1->setStyleSheet(style);
            ui->leMotClbColliCapPosn1->setStyleSheet(style);

            ui->leMotClbColliRefPosn2->setStyleSheet(style2);
            ui->leMotClbColliCapCount2->setStyleSheet(style2);
            ui->leMotClbColliCapPosn2->setStyleSheet(style2);

            ui->leMotClbColliRefPosn3->setStyleSheet(style2);
            ui->leMotClbColliCapCount3->setStyleSheet(style2);
            ui->leMotClbColliCapPosn3->setStyleSheet(style2);
        }
        break;
    default:
        break;
    }
}

void AccUi::ShowMotorCalibGainOffset()
{
    ui->leMotClbGantryGain->setText(QString::number(motor->gantry_gain));
    ui->leMotClbGantryOffset->setText(QString::number(motor->gantry_offset));
    ui->leMotClbJawX1Gain->setText(QString::number(motor->jaw_x1_gain));
    ui->leMotClbJawX1Offset->setText(QString::number(motor->jaw_x1_offset));
    ui->leMotClbJawX2Gain->setText(QString::number(motor->jaw_x2_gain));
    ui->leMotClbJawX2Offset->setText(QString::number(motor->jaw_x2_offset));
    ui->leMotClJawY1Gain->setText(QString::number(motor->jaw_y1_gain));
    ui->leMotClbJawY1Offset->setText(QString::number(motor->jaw_y1_offset));
    ui->leMotClbJawY2Gain->setText(QString::number(motor->jaw_y2_gain));
    ui->leMotClbJawY2Offset->setText(QString::number(motor->jaw_y2_offset));
    ui->leMotClbColliGain->setText(QString::number(motor->colli_gain));
    ui->leMotClbColliOffset->setText(QString::number(motor->colli_offset));
}
//--------------------------------------------------------

//--------------------------------------------------------
//Ljy-8-7 初始化 Tap Select UI
void AccUi::ShowTapSelect()
{
    QList<QLineEdit*> editlist;
    editlist<<ui->leTapSelectCurrE1 <<ui->leTapSelectCurrE2
            <<ui->leTapSelectCurrE3 <<ui->leTapSelectCurrE4
            <<ui->leTapSelectCurrE5 <<ui->leTapSelectCurrE6<<NULL
            <<ui->leTapSelectCurrXL <<ui->leTapSelectCurrXH
            <<ui->leTapSelectCurrFILM;
    for(int i=1;i<11;i++)           //tap中元素E1=0x01，循环应从1开始到9
    {
        if(i==7) continue;             //电子线1-6，X射线89，Flim 10，无7
        switch(io->taps[i])
        {
        case NOTAP:
            editlist[i-1]->setText("N");
            break;
        case TAP100V:
            editlist[i-1]->setText("H");
            break;
        case TAP122V:
            editlist[i-1]->setText("M");
            break;
        case TAP143V:
            editlist[i-1]->setText("L");
            break;
        default:
            break;
        }
    }

    ui->lbTapSelectNewE2->setText("");
    ui->lbTapSelectNewE3->setText("");
    ui->lbTapSelectNewE4->setText("");
    ui->lbTapSelectNewE5->setText("");
    ui->lbTapSelectNewE6->setText("");
    ui->lbTapSelectNewXL->setText("");
    ui->lbTapSelectNewXH->setText("");
    ui->lbTapSelectNewFILM->setText("");

    ui->leTapSelectNewE1->setText("");
    ui->leTapSelectNewE2->setText("");
    ui->leTapSelectNewE3->setText("");
    ui->leTapSelectNewE4->setText("");
    ui->leTapSelectNewE5->setText("");
    ui->leTapSelectNewE6->setText("");
    ui->leTapSelectNewXL->setText("");
    ui->leTapSelectNewXH->setText("");
    ui->leTapSelectNewFILM->setText("");

    ui->leTapSelectNewE1->setReadOnly(false);
    ui->leTapSelectNewE1->setFocus();

    ui->leTapSelectNewE2->setReadOnly(true);
    ui->leTapSelectNewE3->setReadOnly(true);
    ui->leTapSelectNewE4->setReadOnly(true);
    ui->leTapSelectNewE5->setReadOnly(true);
    ui->leTapSelectNewE6->setReadOnly(true);
    ui->leTapSelectNewXL->setReadOnly(true);
    ui->leTapSelectNewXH->setReadOnly(true);
    ui->leTapSelectNewFILM->setReadOnly(true);
}
//--------------------------------------------------------

void AccUi::SetXFixL()
{
    SetPstClear();

    ui->lineEditPstDosRate->setText(g_max_dose_rate);
    ui->lineEditEnergy->setText("06");
    ui->lineEditMod->setText(X_RAY_UI);
    ui->lineEditTech->setText(FIX_UI);

    SetPstDos1();
}

void AccUi::SetXFixH()
{
    SetPstClear();

    ui->lineEditPstDosRate->setText(g_max_dose_rate);
    ui->lineEditEnergy->setText("10");
    ui->lineEditMod->setText(X_RAY_UI);
    ui->lineEditTech->setText(FIX_UI);

    SetPstDos1();
}

void AccUi::SetEFix()
{
    SetPstClear();

    ui->lineEditPstDosRate->setText("");
    ui->lineEditEnergy->setText("");
    ui->lineEditMod->setText(ELTRN_UI);
    ui->lineEditTech->setText(FIX_UI);

    ShowFuncEltrn();
}

void AccUi::SetTreatClear()
{
    QString str;
    SetPstClear();
    //不注释会导致lineeditstatus变成黑色
    //LineeditStopBlink();
    if (input_lock)
    {
        AllLineEditStopBlink();
        input_lock = false;
    }

    ui->lineEditDos1->setText("");
    ui->lineEditDos2->setText("");
    ui->lineEditDosTime->setText("");
    ui->lineEditDosRate->setText("");
    ui->lineEditEnergy->setText("");
    ui->lineEditMod->setText("");
    ui->lineEditTech->setText("");
    ui->lineEditMsg->setText("");

    ClearRemoteControl();

    preset_dose1=0.0;
    preset_dose2=0.0;
    preset_doserate=0.0;
    preset_time=0.0;
    preset_gpos=0.0;
    gantry_rot_arc=0.0;
    gantry_start_pos=0.0;
    gantry_stop_pos=0.0;
    gantry_speed=0.0;

    if(!io->acc3_exist)
    {
        HideJawX1X2();
        HideJawY1Y2();
    }

    ui->svcTreat->update();
}

void AccUi::SetPstClear()
{
    ui->lineEditPstDos1->setText("");
    ui->lineEditPstDos2->setText("");
    ui->lineEditPstTime->setText("");
    ui->lineEditPstDosRate->setText("");
}

void AccUi::SetPstDosRate()
{
    ui->lineEditF1->setText(F1_LOW);
    ui->lineEditF2->setText(F2_HIGH);
    ui->lineEditF3->setText("");
    ui->lineEditF4->setText("");
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText("");
    ui->lineEditPstDosRate->startBlink(RED);
}

void AccUi::SetPstDos2Time()
{
    QString temp;

    //preset_dose1 = ui->lineEditPstDos1->text().toFloat();
    preset_dose2 =qMin ((preset_dose1*1.1+0.5) , preset_dose1+25.0);
    preset_doserate = ui->lineEditPstDosRate->text().toFloat();
    preset_time = preset_dose2 / preset_doserate+0.005;

    temp.sprintf("%04d",(int)preset_dose2);
    ui->lineEditPstDos2->setText(temp);
    temp.sprintf("%05.2f",preset_time);
    ui->lineEditPstTime->setText(temp);
}

void AccUi::SetPstDos1()
{
    input_lock = true;

    ShowFuncClear();
    ui->lineEditF6->setText(F6_SETUP);
    ui->lineEditPstDos1->startBlink(RED);
    //ui->lineEditPstDos2->startBlink(RED);
    //ui->lineEditPstTime->startBlink(RED);
    ui->lineEditPstDos1->setReadOnly(false);
    ui->lineEditPstDos1->setFocus();
    ui->lineEditMsg->setText(MSG_ENTER_DOSE1);
}

void AccUi::SetPstDosTime()
{
    //float pst_dos_time;
    QString temp;
    preset_time = ui->lineEditPstTime->text().toFloat();
    temp.sprintf("%05.2f", preset_time);
    ui->lineEditPstTime->setText(temp);
}

void AccUi::SetCollimat()
{
    QString temp;
    collimat_pst = ui->lineEditCollimator->text().toFloat();
    temp.sprintf("%05.1f", collimat_pst);
    ui->lineEditCollimator->setText(temp);
}

void AccUi::SetJawX()
{
    QString temp;
    float jawx_pst;
    jawx_pst = ui->lineEditJawX->text().toFloat();
    temp.sprintf("%04.1f", jawx_pst);
    ui->lineEditJawX->setText(temp);
}

void AccUi::SetJawX1()
{
    QString temp;
    float jawx1_pst;
    jawx1_pst = ui->lineEditJawX1->text().toFloat();
    if(jawx1_pst > 0)
        temp.sprintf("+%04.1f", jawx1_pst);
    else
        temp.sprintf("%05.1f", jawx1_pst);
    ui->lineEditJawX1->setText(temp);
}

void AccUi::SetJawX2()
{
    QString temp;
    float jawx2_pst;
    jawx2_pst = ui->lineEditJawX2->text().toFloat();
    if(jawx2_pst > 0)
        temp.sprintf("+%04.1f", jawx2_pst);
    else
        temp.sprintf("%05.1f", jawx2_pst);
    ui->lineEditJawX2->setText(temp);
}

void AccUi::SetJawY()
{
    QString temp;
    float jawy_pst;
    jawy_pst = ui->lineEditJawY->text().toFloat();
    temp.sprintf("%04.1f", jawy_pst);
    ui->lineEditJawY->setText(temp);
}

void AccUi::SetJawY1()
{
    QString temp;
    float jawy1_pst;
    jawy1_pst = ui->lineEditJawY1->text().toFloat();
    if(jawy1_pst > 0)
        temp.sprintf("+%04.1f", jawy1_pst);
    else
        temp.sprintf("%05.1f", jawy1_pst);
    ui->lineEditJawY1->setText(temp);
}

void AccUi::SetJawY2()
{
    QString temp;
    float jawy2_pst;
    jawy2_pst = ui->lineEditJawY2->text().toFloat();
    if(jawy2_pst > 0)
        temp.sprintf("+%04.1f", jawy2_pst);
    else
        temp.sprintf("%05.1f", jawy2_pst);
    ui->lineEditJawY2->setText(temp);
}

//换算和设置大机架的旋转角度
void AccUi::SetGanPos()
{
    float gantry;
//    float act_gantry_pos_2;
    bool suc;
    QString temp;

    gantry = preset_gpos;

    temp.sprintf("%05.1f", gantry);
    ui->lineEditGanPos->setText(temp);

    if(gantry_act_pos>=0)
    {
        if (gantry > (180 + GANTRY_LIMIT))
        {
            gantry  = gantry - 360;
        }
    }
    else
    {
        if (gantry >= (180 - GANTRY_LIMIT))  //12-01 if (gantry > (180 - GANTRY_LIMIT))
        {
            gantry = gantry - 360;
        }
    }

    if (gantry > gantry_act_pos)
    {
        //gantry_is_cw = true;
        ui->labelGanRight->setText("->");
        ui->labelGanLeft->setText("");
        ui->svcTreat->update();
    }
    else
    {
        //gantry_is_cw = false;
        ui->labelGanRight->setText("");
        ui->labelGanLeft->setText("<-");
        ui->svcTreat->update();
    }


    suc = SlaveBase::CheckValue(gantry, GANTRY_CK, gantry_pst);
    if(!suc)
    {
        //erro
        return;
    }
}

//换算和设置大机架的在旋转治疗时的旋转角度
//void AccUi::SetRotGanPos()
//{
//    float r_gpos;
//    if(preset_gpos<170)
//    {
//        r_gpos = preset_gpos;

//    }
//    else
//    {
//        r_gpos = preset_gpos - 360;
//    }


//}

void AccUi::SetPstDos1Dos2()
{
    ShowFuncClear();
    input_lock = true;
    ui->lineEditPstDos1->startBlink(RED);
    //ui->lineEditPstDos2->startBlink(RED);
    ui->lineEditPstDos1->setReadOnly(false);
    ui->lineEditPstDos1->setFocus();
    ui->lineEditMsg->setText(MSG_ENTER_DOSE1);
}

void AccUi::SetRotDos2TimeRate()
{
    QString temp;

    //    preset_dose1 = ui->lineEditPstDos1->text().toFloat();
    preset_dose2 = (preset_dose1*1.1 > preset_dose1+25.0) ? preset_dose1+25.0 : preset_dose1*1.1;

    //    temp.sprintf("%04d",(int)preset_dose1);
    //    ui->lineEditPstDos1->setText(temp);

    temp.sprintf("%04d",(int)preset_dose2);
    ui->lineEditPstDos2->setText(temp);

    //    if(g_ray_type==ROT_X_HI)
    //    {
    //        preset_doserate=50.0;
    //        ui->lineEditPstDosRate->setText("50");
    //    }
    //    else
    //    {
    //        preset_doserate=200.0;
    //        ui->lineEditPstDosRate->setText("200");
    //    }

    preset_doserate=g_max_dose_rate.toFloat();
    ui->lineEditPstDosRate->setText(g_max_dose_rate);

    preset_time=preset_dose2/preset_doserate;
    temp.sprintf("%05.2f",preset_time);
    ui->lineEditPstTime->setText(temp);
}

void AccUi::DiaplayErrorHistoryTable()
{
    int head = err_page_index*10;
    int tail = (err_page_index+1)*10;
    for(int i = head;i<tail;i++)
    {
        ui->tableWidget->item((i%10),0)->setText(QString("%1").arg(i));
        ui->tableWidget->item((i%10),1)->setText(QString("%1").arg(g_error_history[i]));
    }
}

//11-19
// Set Roting Treat start position of Gantry
void AccUi::SetRotGanStratPos()
{
    bool suc;
    preset_gpos = ui->lineEditGanPos->text().toFloat();

    if(preset_gpos<170 && preset_gpos>=0)
    {
        gantry_start_pos = preset_gpos;

        if((gantry_act_pos > gantry_start_pos) && (gantry_act_pos < (180 + GANTRY_LIMIT)))
        {
            //gantry_is_cw = false;
            ui->labelGanRight->setText("");
            ui->labelGanLeft->setText("<-");
        }
        else
        {
            //gantry_is_cw = true;
            ui->labelGanRight->setText("->");
            ui->labelGanLeft->setText("");
        }
        ui->svcTreat->update();
    }
    else
    {
        gantry_start_pos = preset_gpos - 360;
        if( (gantry_act_pos > (-180-GANTRY_LIMIT) ) && ( gantry_act_pos< (gantry_start_pos ) ) )
        {
            //gantry_is_cw = true;
            ui->labelGanRight->setText("->");
            ui->labelGanLeft->setText("");
        }
        else
        {
            //gantry_is_cw = false;
            ui->labelGanRight->setText("");
            ui->labelGanLeft->setText("<-");
        }
        ui->svcTreat->update();
    }

    suc = SlaveBase::CheckValue(gantry_start_pos, GANTRY_CK, motor->rot_start_position);
    if(!suc)
    {
        //erro
        return;
    }
    gantry_pst = motor->rot_start_position;
//    suc = SlaveBase::CheckValue(gantry_start_pos, GANTRY_CK, gantry_pst);
//    if(!suc)
//    {
//        //erro
//        return;
//    }
}

bool AccUi::SetRotGanStopPos()
{
    QString temp;
    float stop_pos;
    bool suc;

    gantry_stop_pos=gantry_start_pos+gantry_rot_arc;
    //qDebug()<<"stop_pos1="<<stop_pos;
    if(gantry_stop_pos>(180+GANTRY_LIMIT))
    {
        ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
        return false;
    }
    else
    {
        suc = SlaveBase::CheckValue(gantry_stop_pos, GANTRY_CK, motor->rot_stop_position);
    }

    if(gantry_stop_pos<0)
        stop_pos = gantry_stop_pos + 360;
    else
        stop_pos = gantry_stop_pos;
    temp.sprintf("%05.1f",stop_pos);
    ui->lineEditStop1->setText(temp);

    return true;
}


//LJY-9-23
bool AccUi::SetRotArc1()
{
    //float dose1=ui->lineEditPstDos1->text().toFloat();
    float arc=ui->lineEditArc1->text().toFloat();
    if(arc<0.1 || arc>360)
    {
        ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
        return false;
    }
    else if(preset_dose1/arc<0.33 || preset_dose1/arc>5)
    {
        //qDebug()<<"dose1/arc="<<(preset_dose1/arc);
        ui->lineEditMsg->setText(MSG_LIMIT_REENTER);
        return false;
    }
    else
    {
        gantry_rot_arc=arc;
        return true;
    }
}

bool AccUi::CheckRotGanStartPos()
{
    float abs;
    abs=qAbs(gantry_start_pos-gantry_act_pos);
    qDebug()<<"gantry_pst(UINT32)="<<gantry_pst<<"  gantry_start_pos(Float)="<<gantry_start_pos<<"  act_gantry_pos"<<gantry_act_pos;
    if(abs<1 && abs>-1)     //预设位置与实际位置允许左右各一度的误差
        return true;
    else
        return false;
}

bool AccUi::CheckAcceptMotorSetAct()
{
    //Warning: 无符号数需要先转换为有符号数再相减，然后才能使用qAbs取绝对值
    //例：qAbs( (UINT16)2227-(UINT16)2228 )=65535
    UINT16 tmp_gantry = qAbs((int)gantry_pst - (int)motor->gantry_position_speed.gantry_position);
    UINT16 tmp_colli=qAbs((int)colli_pst - (int)motor->position[COLLIMATOR]);
    UINT16 tmp_jawx1=qAbs((int)jawx1_pst - (int)motor->position[JAW_B2]);
    UINT16 tmp_jawx2=qAbs((int)jawx2_pst - (int)motor->position[JAW_B1]);
    UINT16 tmp_jawy1=qAbs((int)jawy1_pst - (int)motor->position[JAW_A1]);
    UINT16 tmp_jawy2=qAbs((int)jawy2_pst - (int)motor->position[JAW_A2]);

    qDebug()<<"Gantry:"<<tmp_gantry<<gantry_pst<<motor->gantry_position_speed.gantry_position
            <<", colli:"<<tmp_colli<<colli_pst<<motor->position[COLLIMATOR]
            <<", jawx1:"<<tmp_jawx1<<jawx1_pst<<motor->position[JAW_B2]
            <<", jawx2:"<<tmp_jawx2<<jawx2_pst<<motor->position[JAW_B1]
            <<", jawy1:"<<tmp_jawy1<<jawy1_pst<<motor->position[JAW_A1]
            <<", jawy2:"<<tmp_jawy2<<jawy2_pst<<motor->position[JAW_A2];

    if(tmp_gantry > motor->gantry_max_toler ||
            tmp_colli > motor->colli_max_toler ||
            tmp_jawx1 > motor->jawx1_max_toler ||
            tmp_jawx2 > motor->jawx2_max_toler ||
            tmp_jawy1 > motor->jawy1_max_toler ||
            tmp_jawy2 > motor->jawy2_max_toler)
        return false;
    else
        return true;
}

bool AccUi::CheckAcceptData()
{
    if (input_lock)
        return false;

    if (ui->lineEditMsg->text() == MSG_LIMIT_REENTER)
        return false;
    if (ui->lineEditMsg->text() == MSG_CTRL_X)
        return false;
    if (qAbs(preset_dose1 - ui->lineEditPstDos1->text().toFloat()) > 0.1)
        return false;

    if(g_ray_type==ROT_X_HI || g_ray_type==ROT_X_LO)
    {
        if(gantry_rot_arc<1 || preset_dose1<1)
        {
            ui->lineEditMsg->setText(MSG_PARA_COMPLETE);
            return false;
        }
        else
            return true;
    }
    else if(preset_dose1<1)
    {
        ui->lineEditMsg->setText(MSG_PARA_COMPLETE);
        return false;
    }
    else
        return true;
}

void AccUi::CheckSoftpotLimitIntegrity()
{
    if(!base->open_softpot_file)
        io->InsertInterlock(62, 800, ERROR_IL, IL_ALL, LATCHED);

    if(!beam->CheckSoftpotDataLimit() || !dosei->CheckSoftpotDataLimit()
            || !doseii->CheckSoftpotDataLimit() || !io->CheckSoftpotDataLimit())
        io->InsertInterlock(62, 801, ERROR_IL, IL_ALL, LATCHED);

    if(!beam->CheckSoftpotDataIntegrity() || !dosei->CheckSoftpotDataIntegrity()
            || !doseii->CheckSoftpotDataIntegrity() || !io->CheckSoftpotDataIntegrity())
        io->InsertInterlock(62, 802, ERROR_IL, IL_ALL, LATCHED);
}

void AccUi::InitInterlockTable()
{
    ui->tWPage1->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWPage1->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tWPage1->horizontalHeader()->setStyleSheet("QHeaderView::section{background:black;font:75 20pt}");
    ui->tWPage1->horizontalHeader()->setVisible(true);
    ui->tWPage1->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AccUi::InitErrorHistoryTable()
{
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:black;font:75 20pt}");
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void AccUi::DisplayInterlockTable()
{
    int head;
    int tail;
    head = ilk_page_index*10;
    tail = (ilk_page_index+1)*10;
    if(ilk_page_index < 12)
    {
        for(int num = head;num<tail;num++)
        {
            SetInterlockTableItem(num,g_interlocks[num].ovrd,
                                  g_interlocks[num].status,g_interlocks[num].last_error_num,
                                  g_interlocks[num].history);

        }
    }
    else
    {
        for(int num = head;num<INTERLOCK_NUM;num++)
        {
            SetInterlockTableItem(num,g_interlocks[num].ovrd,
                                  g_interlocks[num].status,g_interlocks[num].last_error_num,
                                  g_interlocks[num].history);
        }
        for(int num = INTERLOCK_NUM;num<130;num++)
        {
            for(int i =0;i<5;i++)
                ui->tWPage1->item(num % 10,i)->setText("");
        }

    }
}

void AccUi::InterlockSetOvrd(int num, bool setvalue)
{
    int row = num%10;
    if(g_interlocks[num].ovrd_type != CAN_OVRD)
        return;
    if(setvalue)
    {
        //设置旁路
        SlaveBase::OvrdInterlock(num,true);
        ui->tWPage1->item(row,1)->setText("Y");
    }
    else
    {
        //去除旁路
        SlaveBase::OvrdInterlock(num,false);
        ui->tWPage1->item(row,1)->setText("");
    }

    //motor error masks
    if(num==63)
    {
        if(setvalue)    //63号Interlock设置旁路
        {
            for(int i=0; i<7; i++)
            {
                motor->error_mask_1.error_mask_bytes[i] = 0xff;
                motor->error_mask_2.error_mask_bytes[i] = 0xff;
            }
        }
        else
        {
            for(int i=0; i<7; i++)
            {
                motor->error_mask_1.error_mask_bytes[i] = 0x00;
                motor->error_mask_2.error_mask_bytes[i] = 0x00;
            }
        }
        if(g_status == PROGRAM)
            motor->SendErrorMask();
        return;
    }

    //dose1,dose2 bypass mask
    switch(num)
    {
    case 12:
    case 56:
    case 82:
    case 93:
    case 58:
    case 57:
        CaculateBypassMask();
        if(g_status == RAD_ON || g_status == INTERRUPT || g_status == INTERRUPT_NR
                || g_status == READYS || g_status == NOT_READY)
        {
            dosei->SendInformation();
        }
        break;
    case 1:
    case 13:
    case 61:
    case 60:
        CaculateBypassMask();
        qDebug()<<"Dose2 bypass, g_status ="<<g_status;
        if(g_status == RAD_ON || g_status == INTERRUPT || g_status == INTERRUPT_NR
                || g_status == READYS || g_status == NOT_READY)
        {
            qDebug()<<"Dose2 SendInformation";
            doseii->SendInformation();
        }
        break;
    case 2:
    case 59:
    case 97:
        CaculateBypassMask();
        if(g_status == RAD_ON || g_status == INTERRUPT || g_status == INTERRUPT_NR
                || g_status == READYS || g_status == NOT_READY)
        {
            dosei->SendInformation();
            doseii->SendInformation();
        }
        break;
    default:
        break;
    }
}

void AccUi::ShowInterlockError(int num)
{
    qDebug()<<"ShowInterlockError";
    if(num>127 || num<0)
        return;

    //LJY 15-12-11
    if(num==15)
    {
        //QString tmp=QString::number(g_err862data,16);
        QString tmp;
        tmp.sprintf("%04x", headdata);
        errlist[15].clear();
        errlist[15]<<"860	‘K’ command from IO controller was not received."
                  <<"861	‘K’ command from IO controller echoed wrong mode."
                 <<"862	‘K’ command from IO controller: head unequal ("+tmp+").";
    }
    if(num==85)
    {
        QString tmp;
        tmp.sprintf("%04x", bitsdata);
        errlist[85].clear();
        errlist[85]<<"869	‘K’ command from IO controller: bits unequal ("+tmp+").";
    }

    ui->textEdit->clear();
    QStringList::iterator it = errlist[num].begin();
    for( ; it != errlist[num].end(); it++)
        ui->textEdit->append(*it);
    ui->textEdit->moveCursor(QTextCursor::Start);
    ui->sWInterlock->setCurrentWidget(ui->interlockPage14);
    ui->lbInterlockErr->setText(interlock_list[num]);
    ui->lbInterlockErrInfo->setText(ilckinfolist[num]);
}

void AccUi::ShowFuncCollJaw()
{
    ui->lineEditF1->setText(F1_SYNC);
    ui->lineEditF2->setText(F2_INDEP);

    if(hand->hand_colli_control)
        ui->lineEditF3->setText("");
    else
        ui->lineEditF3->setText(F3_COLLI);

    ui->lineEditF4->setText("");
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText(F6_PRV_MNU);
}

void AccUi::ShowFuncRot()
{
    ui->lineEditF1->setText(F1_X_ROT_L);
    ui->lineEditF2->setText(F2_X_ROT_H);
    ui->lineEditF3->setText("");
    ui->lineEditF4->setText("");
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText("");
}

void AccUi::ShowFuncRotPar()
{
    ui->lineEditF1->setText(F1_ENERGY);
    ui->lineEditF2->setText(F2_MU);
    ui->lineEditF3->setText(F3_GANTRY);
    ui->lineEditF4->setText(F4_ARC);
    ui->lineEditF5->setText("");
    ui->lineEditF6->setText("");
}

void AccUi::ShowFuncEltrn()
{
    ui->lineEditF1->setText("F1  5 MEV");
    ui->lineEditF2->setText("F2  7 MEV");
    ui->lineEditF3->setText("F3  8 MEV");
    ui->lineEditF4->setText("F4  10 MEV");
    ui->lineEditF5->setText("F5  12 MEV");
    ui->lineEditF6->setText("F6  14 MEV");
}

void AccUi::UpdateFuncKey()
{
    switch (select) {
    case TREAT_PARAMETER:
        ShowFuncPara();
        break;
    case TREAT_COLL_JAW:
        ShowFuncCollJaw();
        break;
    case TREAT_SYNCJ:
        ShowFuncJawXY();
        break;
    case TREAT_INDEPJ:
        if(g_treat_svc_mode==SVC)
        {
            ShowFuncX1X2Y1Y2();
        }
        else
        {
            ShowFuncX1X2Y1Y2();
            //ShowFuncY1Y2();
        }
        break;
    default:
        break;
    }
}

bool AccUi::CheckPstDos1()
{
    int pst_dos1;

    pst_dos1 = ui->lineEditPstDos1->text().toInt();

    if(pst_dos1 > 0 && pst_dos1 < 9976)
    {
        preset_dose1=(float)pst_dos1;
        return true;
    }
    else
        return false;
}

bool AccUi::CheckPstTime()
{
    float pst_time;

    pst_time = ui->lineEditPstTime->text().toFloat();

    if(pst_time>0 && pst_time<100)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWX()
{
    float pst_jawx;

    pst_jawx = ui->lineEditJawX->text().toFloat();

    if(CFloatCompare::FloatGreatEqualZero(pst_jawx) && pst_jawx <= 40)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWY()
{
    float pst_jawy;

    pst_jawy = ui->lineEditJawY->text().toFloat();

    if(CFloatCompare::FloatGreatEqualZero(pst_jawy) && pst_jawy <= 40)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWX1()
{
    float pst_jawx1;
    float pst_jawx2;

    if (g_is_remote_mode)
    {
        return true;
    }

    pst_jawx1 =-ui->lineEditJawX1->text().toFloat();
    pst_jawx2 = ui->lineEditJawX2->text().toFloat();

    if(pst_jawx1 >= -2 && pst_jawx1 <= 20
            && (pst_jawx1 + pst_jawx2) >= 0)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWX2()
{
    float pst_jawx1;
    float pst_jawx2;

    if (g_is_remote_mode)
    {
        return true;
    }

    pst_jawx1 = -ui->lineEditJawX1->text().toFloat();
    pst_jawx2 = ui->lineEditJawX2->text().toFloat();

    if(pst_jawx2 >= -2 && pst_jawx2 <= 20
            && (pst_jawx1 + pst_jawx2) >= 0)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWY1()
{
    float pst_jawy1;
    float pst_jawy2;

    if (g_is_remote_mode)
    {
        return true;
    }

    pst_jawy1 = -ui->lineEditJawY1->text().toFloat();
    pst_jawy2 = ui->lineEditJawY2->text().toFloat();

    if(pst_jawy1 >= -10 && pst_jawy1 <= 20
            && (pst_jawy1 + pst_jawy2) >= 0)
        return true;
    else
        return false;
}

bool AccUi::CheckPstJAWY2()
{
    float pst_jawy1;
    float pst_jawy2;

    if (g_is_remote_mode)
    {
        return true;
    }

    pst_jawy1 = -ui->lineEditJawY1->text().toFloat();
    pst_jawy2 = ui->lineEditJawY2->text().toFloat();

    if(pst_jawy2 >= -10 && pst_jawy2 <= 20
            && (pst_jawy1 + pst_jawy2) >= 0)
        return true;
    else
        return false;
}

bool AccUi::CheckPstCollimator()
{
    float pst_colli;

    pst_colli = ui->lineEditCollimator->text().toFloat();

    if((pst_colli >= 0 && pst_colli <= 95) || (pst_colli >= 265 && pst_colli < 360))
        return true;
    else
        return false;
}

bool AccUi::CheckPstGantry()
{
    float pst_gantry;
    pst_gantry = ui->lineEditGanPos->text().toFloat();

    if(pst_gantry >= 0 && pst_gantry <= 360)
    {
        preset_gpos=pst_gantry;
        return true;
    }
    else
        return false;
}

void AccUi::doDownload(QString dir, QString destination)
{
    direc = dir;
    QUrl url(destination);
    url.setUserName("711");
    url.setPassword("711");
    QNetworkRequest request(url);
    qDebug()<<url.toString();
    downloadManager.get(request);
}

void AccUi::doUpload(QString file, QString destination)
{
    QUrl url(destination);
    url.setUserName("711");
    url.setPassword("711");

    upfile = new QFile(file);

    if (upfile->open(QIODevice::ReadOnly)) {
        uploadManager.put(QNetworkRequest(url), upfile);
    }
    else
    {
        //uploadManager.deleteResource(QNetworkRequest(url));
        qDebug() << "The file "<< file <<" is not exist!\n" ;
    }
}

QString AccUi::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();
    basename = direc + basename;
    return basename;
}

bool AccUi::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        fprintf(stderr, "Could not open %s for writing: %s\n",
                qPrintable(filename),
                qPrintable(file.errorString()));
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void AccUi::SetStatus()
{
    /*if (g_is_remote_mode)
    {
        if (ui->lineEditMsg->text()==MSG_REMOTE_CANCEL || ui->lineEditMsg->text()==MSG_REMOTE_TERMINATE)
        {
            ui->lineEditMsg->setText("");
        }

        RemoteSetStatus();
    }
    else
    {*/
        NormalSetStatus();
    //}
}

void AccUi::NormalSetStatus()
{
    switch(g_status)
    {
    case POWER_UP:
        ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case NULL_STATUS1 :
        if (g_right_key==PGM_RKEY)
            ui->lineEditMsg->setText("");
        else if(g_right_key==NULL_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_STDBY_PGM);
        else
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        break;
    case NULL_STATUS2 :
        break;
    case STANDBY:
        ui->lineEditStatus->setText(STANDBY_UI);
        if (g_treat_svc_mode==SVC && treat_switch)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && svc_switch)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else
            ui->lineEditMsg->setText("");
        break;
    case PROGRAM:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(PROGRAM_UI);
        //ui->lineEditMsg->setText("");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (ui->lineEditMsg->text()!=MSG_REMOTE_CANCEL && ui->lineEditMsg->text()!=MSG_REMOTE_TERMINATE)
            ui->lineEditMsg->setText("");

        //ui->lineEditMsg->setStyleSheet("color:white");
        break;
    case READYS:
        ShowFuncClear();
        ui->lineEditStatus->setStyleSheet("background:green");
        ui->lineEditStatus->setText(READY_UI);
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=RADON_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_RADON);

        break;
    case NOT_READY:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(NOT_READY_UI);
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText("");
        break;
    case INTERRUPT_NR:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(INTERRUPT_NR_UI);
        ui->lineEditF1->setText("F1 QUIT");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_CLEAR_QUIT);
        break;
    case RAD_ON:
        ui->lineEditStatus->setStyleSheet("background:yellow");
        ui->lineEditStatus->setText("RAD ON");
        ui->lineEditStatus->startBlink(YELLOW);
        ui->lineEditF1->setText("");
        ui->lineEditMsg->setText("");
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        break;
    case TERMINATE1:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(TERMINATE_UI);
        ui->lineEditF1->setText("");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        reset_pressed=false;
        break;

    case TERMINATE2:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(TERMINATE_UI);

        if (!reset_pressed)
            ui->lineEditMsg->setText(MSG_LKEY_RESET);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case TERMINATE3:
        reset_pressed=false;
        ui->lineEditStatus->setText(TERMINATE_UI);
        if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;

    case COMPLETE1:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        if (g_is_remote_mode)
        {
            if (upper->HasNextData())
                ui->lineEditStatus->setText(SETUP_UI);
            else
                ui->lineEditStatus->setText(COMPLETE_UI);
        }
        else
        {
            ui->lineEditStatus->setText(COMPLETE_UI);
        }

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (!g_check2_bypass)
            ui->lineEditMsg->setText(MSG_WAIT);
        else
            ui->lineEditMsg->setText("Check #2 bypass");
        break;

    case COMPLETE2:
        if (g_is_remote_mode)
        {
            if (upper->HasNextData())
                ui->lineEditStatus->setText(SETUP_UI);
            else
                ui->lineEditStatus->setText(COMPLETE_UI);
        }
        else
        {
            ui->lineEditStatus->setText(COMPLETE_UI);
        }

        if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;

    case INTERRUPT:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:green");
        ui->lineEditStatus->setText(INTERRUPT_UI);

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_RADON_QUIT);
        break;
    case ERRORS:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(ERROR_UI);
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_LKEY_RESET);
        break;
    case CHECK2_1:

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case CHECK2_2:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText("CHECK #2");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    default:
        break;
    }
}

void AccUi::RemoteSetStatus()
{
    switch(g_status)
    {
    case POWER_UP:
        ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case NULL_STATUS1 :
        if (g_right_key==PGM_RKEY)
            ui->lineEditMsg->setText("");
        else if(g_right_key==NULL_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_STDBY_PGM);
        else
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        break;
    case NULL_STATUS2 :
        break;
    case STANDBY:
        ui->lineEditStatus->setText(STANDBY_UI);
        if (g_treat_svc_mode==SVC && treat_switch)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && svc_switch)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else
            ui->lineEditMsg->setText("");
        break;
    case PROGRAM:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(PROGRAM_UI);
        ui->lineEditMsg->setText("");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (!remote->hasSetdata)
            ui->lineEditMsg->setText(MSG_REMOTE_CANCEL);
        else
            ui->lineEditMsg->setText("");

        //ui->lineEditMsg->setStyleSheet("color:white");
        break;
    case READYS:
        ShowFuncClear();
        ui->lineEditStatus->setStyleSheet("background:green");
        ui->lineEditStatus->setText(READY_UI);

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=RADON_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_RADON);
        else
            ui->lineEditMsg->setText(MSG_REMOTE_TERMINATE);

        break;
    case NOT_READY:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(NOT_READY_UI);
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_REMOTE_TERMINATE);
        break;
    case INTERRUPT_NR:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(INTERRUPT_NR_UI);
        ui->lineEditF1->setText("F1 QUIT");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_CLEAR_QUIT);
        break;
    case RAD_ON:
        ui->lineEditStatus->setStyleSheet("background:yellow");
        ui->lineEditStatus->setText("RAD ON");
        ui->lineEditStatus->startBlink(YELLOW);
        ui->lineEditF1->setText("");
        ui->lineEditMsg->setText("");
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (ui->lineEditMsg->text() == MSG_REMOTE_TERMINATE)
            ui->lineEditMsg->setText("");
        break;
    case TERMINATE1:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(TERMINATE_UI);
        ui->lineEditF1->setText("");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        reset_pressed=false;
        break;

    case TERMINATE2:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(TERMINATE_UI);

        if (!reset_pressed)
            ui->lineEditMsg->setText(MSG_LKEY_RESET);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case TERMINATE3:
        reset_pressed=false;
        ui->lineEditStatus->setText(TERMINATE_UI);
        if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;

    case COMPLETE1:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:blue");
        if (g_is_remote_mode)
        {
            if (upper->HasNextData())
                ui->lineEditStatus->setText(SETUP_UI);
            else
                ui->lineEditStatus->setText(COMPLETE_UI);
        }
        else
        {
            ui->lineEditStatus->setText(COMPLETE_UI);
        }

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (!g_check2_bypass)
            ui->lineEditMsg->setText(MSG_WAIT);
        else
            ui->lineEditMsg->setText("Check #2 bypass");
        break;

    case COMPLETE2:
        if (g_is_remote_mode)
        {
            if (upper->HasNextData())
                ui->lineEditStatus->setText(SETUP_UI);
            else
                ui->lineEditStatus->setText(COMPLETE_UI);
        }
        else
        {
            ui->lineEditStatus->setText(COMPLETE_UI);
        }

        if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;

    case INTERRUPT:
        ui->lineEditStatus->stopBlink();
        ui->lineEditStatus->setStyleSheet("background:green");
        ui->lineEditStatus->setText(INTERRUPT_UI);

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_RADON_QUIT);
        break;
    case ERRORS:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText(ERROR_UI);
        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else
            ui->lineEditMsg->setText(MSG_LKEY_RESET);
        break;
    case CHECK2_1:

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    case CHECK2_2:
        ui->lineEditStatus->setStyleSheet("background:blue");
        ui->lineEditStatus->setText("CHECK #2");

        if (g_treat_svc_mode==SVC && g_left_key==TREAT_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_SVC);
        else if (g_treat_svc_mode==TREAT && g_left_key==SVC_LKEY)
            ui->lineEditMsg->setText(MSG_LKEY_TREAT);
        else if (g_right_key!=PGM_RKEY)
            ui->lineEditMsg->setText(MSG_RKEY_PGM);
        else if (g_action_key!=CLEAR_KEY)
            ui->lineEditMsg->setText(MSG_CLEAR_KEY);
        else
            ui->lineEditMsg->setText(MSG_WAIT);
        break;
    default:
        break;
    }
}


void AccUi::ShowPromptMsg(QString str)
{
//    switch(c)
//    {
//    case 'R':
//        ui->lineEditMsg->setStyleSheet("color:red");
//        break;
//    case 'W':
//        ui->lineEditMsg->setStyleSheet("color:white");
//        break;
//    default:
//        break;
//    }
    ui->lineEditMsg->setText(str);
}

//interlock message
void AccUi::SetInterlockHappened(int type, int num)
{
    //QString tmp;
    //qDebug()<<"SetInterlockHappened  type="<<type<<"  num="<<num;
    interlock_mutex.lock();
    switch(type)
    {
    case 0:
        if(!safety_interlock.contains(num))
            safety_interlock.append(num);
        break;
    case 1:
        if(!system_interlock.contains(num))
            system_interlock.append(num);
        break;
    case 2:
        if(!error_interlock.contains(num))
            error_interlock.append(num);
        break;
    default:
        break;
    }
    if(safety_interlock.size() == 1)
    {
        ui->lineEditSafeLock->setText(interlock_list[safety_interlock[0]]);
        if(g_treat_svc_mode == SVC && g_interlocks[safety_interlock[0]].ovrd)
        {
            ui->lineEditSafeLock->setStyleSheet("background-color:blue");
        }
        else
        {
            ui->lineEditSafeLock->setStyleSheet("background-color:red");
        }
    }
    if(system_interlock.size() == 1)
    {
        ui->lineEditSysLock->setText(interlock_list[system_interlock[0]]);
        if(g_treat_svc_mode == SVC && g_interlocks[system_interlock[0]].ovrd)
        {
            ui->lineEditSysLock->setStyleSheet("background-color:blue");
        }
        else
        {
            ui->lineEditSysLock->setStyleSheet("background-color:red");
        }
    }
    if(error_interlock.size() == 1)
    {
        ui->lineEditErrLock->setText(interlock_list[error_interlock[0]]);
        if(g_treat_svc_mode == SVC && g_interlocks[error_interlock[0]].ovrd)
        {
            ui->lineEditErrLock->setStyleSheet("background-color:blue");
        }
        else
        {
            ui->lineEditErrLock->setStyleSheet("background-color:red");
        }
    }
    interlock_mutex.unlock();

    //ProcessInterlockMsgDown();
}

void AccUi::SetInterlockCleared(int type, int num)
{
    //qDebug()<<"SetInterlockCleared type="<<type<<"  num="<<num;
    interlock_mutex.lock();
    switch(type)
    {
    case 0:
        if(safety_interlock.contains(num))
            safety_interlock.removeOne(num);
        break;
    case 1:
        if(system_interlock.contains(num))
            system_interlock.removeOne(num);
        break;
    case 2:
        if(error_interlock.contains(num))
            error_interlock.removeOne(num);
        break;
    default:
        break;
    }

    if(safety_interlock.size() == 0)
    {
        ui->lineEditSafeLock->setStyleSheet("");
        ui->lineEditSafeLock->setText("");
    }
    if(system_interlock.size() == 0)
    {
        ui->lineEditSysLock->setStyleSheet("");
        ui->lineEditSysLock->setText("");
    }
    if(error_interlock.size() == 0)
    {
        ui->lineEditErrLock->setStyleSheet("");
        ui->lineEditErrLock->setText("");
    }
    interlock_mutex.unlock();

    //ProcessInterlockMsgDown();
}
//duanzhibin  10/14
void AccUi::Clear_Ovrd_Interlock()
{
    for(int i = 0;i < INTERLOCK_NUM;i++)
    {
        if(g_interlocks[i].ovrd_type == CAN_OVRD)
        {
            g_interlocks_mutex.lock();
            g_interlocks[i].ovrd = false;
            g_interlocks_mutex.unlock();
            g_lock_passby_bits.setBit(i,false);
        }
    }

    //motor error masks
    for(int i=0; i<7; i++)
    {
        motor->error_mask_1.error_mask_bytes[i] = 0x00;
        motor->error_mask_2.error_mask_bytes[i] = 0x00;
    }
    if(g_status == PROGRAM)
        motor->SendErrorMask();

    //dose1,dose2 bypass masks
    CaculateBypassMask();
    if(g_status == RAD_ON || g_status == INTERRUPT || g_status == INTERRUPT_NR
            || g_status == READYS || g_status == NOT_READY)
    {
        dosei->SendInformation();
        doseii->SendInformation();
    }
    DisplayInterlockTable();
    //interlock->WriteOwnFile();
}
void AccUi::Ovrd_Availabl_Interlock()
{
    for(int i = 0; i < INTERLOCK_NUM;i++)
    {
        if(g_interlocks[i].ovrd_type == CAN_OVRD)
        {
            g_interlocks_mutex.lock();
            g_interlocks[i].ovrd = true;
            g_interlocks_mutex.unlock();
            g_lock_passby_bits.setBit(i);
        }
    }

    //motor error masks
    for(int i=0; i<7; i++)
    {
        motor->error_mask_1.error_mask_bytes[i] = 0xff;
        motor->error_mask_2.error_mask_bytes[i] = 0xff;
    }
    if(g_status == PROGRAM)
        motor->SendErrorMask();

    //dose1,dose2 bypass masks
    CaculateBypassMask();
    if(g_status == RAD_ON || g_status == INTERRUPT || g_status == INTERRUPT_NR
            || g_status == READYS || g_status == NOT_READY)
    {
        dosei->SendInformation();
        doseii->SendInformation();
    }

    DisplayInterlockTable();
}

void AccUi::ShowGantryDirec(bool cw)
{
    if(cw)
    {
        ui->labelGanRight->setText("->");
        ui->labelGanLeft->setText("");
    }
    else
    {
        ui->labelGanRight->setText("");
        ui->labelGanLeft->setText("<-");

    }
    ui->svcTreat->update();
}


