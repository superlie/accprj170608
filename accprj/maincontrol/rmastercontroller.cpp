#include "mastercommon.h"
extern bool g_is_remote_mode;
extern QTimer *g_mlc_movestop_timer;

static int mu = 0;

void MasterController::RemoteMasterCtrl(enum Status &state, enum Status &last_state)
{
    mc_interlock_status = CheckInterlock();

    switch (state)
    {
    case NULL_STATUS1 :
        if (g_right_key==PGM_RKEY)
        {
            state=NULL_STATUS2;
            g_status=NULL_STATUS2;
            g_il_state=IL_NULL;

            //                emit base->Prompt_Msg("");
        }
        //            else if(g_right_key==NULL_RKEY)
        //            {
        //                emit base->Prompt_Msg(MSG_RKEY_STDBY_PGM);
        //            }
        //            else
        //            {
        //                emit base->Prompt_Msg(MSG_RKEY_PGM);
        //            }
        if (CheckActionKey() == RESET_KEY)    //ResetKey())
        {
            CleanInterlocks();
            //向各从机发送reset命令
            if (ResetCmd())
            {
                g_usermode=RESET_MODE;
            }
            else
            {
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
            CleanActionKey(RESET_KEY);
        }
        break;
    case NULL_STATUS2 :
        if (CheckActionKey() == RESET_KEY)    //ResetKey())
        {
            CleanInterlocks();
            //向各从机发送reset命令
            if (ResetCmd())
            {
                state=NULL_STATUS1;
                g_status=NULL_STATUS1;
                g_il_state=IL_NULL;
                g_usermode=RESET_MODE;
            }
            else
            {
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
            CleanActionKey(RESET_KEY);
        }
        //waiting UI change g_usermode
        if (CheckActionKey() ==F1_KEY)
        {
            SendUserMode();
            CleanActionKey(F1_KEY);
            state=PROGRAM;
            g_status=PROGRAM;
            g_il_state=IL_PROGRAM;
            emit base->ProcessShowUISig();
            //emit base->Prompt_Msg("");
        }
        if (CheckActionKey() ==F3_KEY)
        {
            SendUserMode();
            CleanActionKey(F3_KEY);
            state=PROGRAM;
            g_status=PROGRAM;
            g_il_state=IL_PROGRAM;
            emit base->ProcessShowUISig();
            //emit base->Prompt_Msg("");
        }
        break;
    case STANDBY:
        if (CheckActionKey() == PROGRAM_KEY)    //ProgramKey())
        {
            CleanActionKey(PROGRAM_KEY);
            state=PROGRAM;
            g_status=PROGRAM;
            g_il_state=IL_PROGRAM;
        }
        break;
    case PROGRAM:
        if(g_acc3_change)
        {
            emit io->Set_Acc3_Display(io->accessory_3_type);
            g_acc3_change = false;
        }

        if (CheckActionKey() == RESET_KEY)    //ResetKey())
        {
            CleanActionKey(RESET_KEY);
            CleanInterlocks();
            //向各从机发送reset命令
            if (ResetCmd())
            {
                state=NULL_STATUS1;
                g_status=NULL_STATUS1;
                g_il_state=IL_NULL;
                g_usermode=RESET_MODE;

                remote->ResetSetting(INIT);
            }
            else
            {
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
            break;
        }

        if (CheckActionKey() == CLEAR_KEY)
        {
            CleanActionKey(CLEAR_KEY);

            if (!remote->hasSetdata)
            {
                remote->hasSetdata = false;
                remote->ResetSetting(INIT);
            }
            else
            {
                remote->hasSetdata = false;
                emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);
                remote->ResetSetting(WAIT_TREATDATA);
                upper->SendPgmReach();
                mlc->SendPgmReach();
            }

            break;
        }

        if (remote->RemoteStateMachine())
        {
            if (CheckActionKey() == ACCEPT_KEY)    //AcceptKey())
            {
                accui->resetMoveStatus();
                upper->CheckAcceptData();

                //LJY-12-25-------------
                if(light->light_on)
                {
                    light->SendFieldIlluminate();
                    base->g_lightrep_start_signal();
                    io->SendSetup();
                }

                CleanActionKey(ACCEPT_KEY);
                g_il_state=IL_NOT_READY;

                SendAcceptCommands();

                if (!RevIOAcknowledge())
                {
                    g_status = ERRORS;
                    state=ERRORS;
                    g_il_state=IL_ERROR;
                    break;
                }
                if (!g_check1_bypass)
                {
                    g_il_state=IL_CHECK1;
                    SendCheck1Commands();
                    state=NOT_READY1;
                    g_status = NOT_READY;
                    g_il_state=IL_NOT_READY;
                    //motor->CheckMotorSetActPos();
                    break;
                }
                else
                {
                    emit base->Prompt_Msg("Check #1 bypass ");
                    state=NOT_READY2;
                    g_status = NOT_READY;
                    g_il_state=IL_NOT_READY;
                    //motor->CheckMotorSetActPos();
                    break;
                }
            }
        }
        else
        {
            if (CheckActionKey() == ACCEPT_KEY)
            {
                CleanActionKey(ACCEPT_KEY);
            }
        }

        if (CheckActionKey() == STANDBY_KEY)    //StandbyKey())
        {
            CleanActionKey(STANDBY_KEY);
            state=STANDBY;
            g_status = STANDBY;
            g_il_state=IL_PROGRAM;
            break;
        }

        break;
    case NOT_READY1:
        if (!Check1Passed())
        {
            state=ERRORS;
            g_status = ERRORS;
            g_il_state=IL_ERROR;
            qDebug()<<"NOT READY1 ERROR";
        }
        else
        {
            state=NOT_READY2;
            g_status = NOT_READY;
            g_il_state=IL_NOT_READY;
        }
        break;
    case NOT_READY2:
        qDebug()<<"NOT_READY2";
        emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);

        if (mc_interlock_status != ILS_CLEARED)
            SendReadyCommands(NOTREADY);
        state=NOT_READY;
        g_status=NOT_READY;
        g_il_state=IL_NOT_READY;
        if (remote->mMlcMoveToBeamEcho != MMBE_NOERR)
        {
            mlc->InsertInterlockNoErr(124, SYSTEM_IL, IL_ALL, NOT_LATCHED);
        }

#ifdef AUTHORIZATION
        if (!g_auth_check)
            io->InsertInterlockNoErr(0, SYSTEM_IL, IL_ALL, LATCHED);
#endif

        break;
    case NOT_READY:
        motor->CheckMotorSetActPos();
        io->CheckAccInterlock();

        if (CheckActionKey() == CLEAR_KEY)
        {
            CleanActionKey(CLEAR_KEY);
            SendClearCommands();
            state= PROGRAM;
            g_status = PROGRAM;

            DeleteReadyInterlock();
            g_il_state=IL_PROGRAM;

            remote->ResetSetting(WAIT_ACCEPT);
            /*upper->SendPgmReach();
            mlc->SendPgmReach();*/
            emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);

            break;
        }

        if (remote->mUpperAcceptEcho==EUR_NOCHECK)
        {
            break;
        }
        else if (remote->mUpperAcceptEcho==EUR_CANCEL)
        {
            emit remote->setRemoteMsg(MSG_REMOTE_TERMINATE);
            remote->ResetSetting(WAIT_ACCEPT);

            state= PROGRAM;
            g_status = PROGRAM;
            DeleteReadyInterlock();
            g_il_state=IL_PROGRAM;

            break;
        }

        if (mc_interlock_status == ILS_CLEARED)
        {
            SendReadyCommands(READY);
            state=READYS;
            g_status = READYS;
            g_il_state=IL_READY;
            qDebug()<<"NOT_READY no interlock";
            break;
        }

        break;
    case READYS:
        motor->CheckMotorSetActPos();
        io->CheckAccInterlock();
        if (mc_interlock_status != ILS_CLEARED)
        {
            SendReadyCommands(NOTREADY);
            state= NOT_READY;
            g_status = NOT_READY;
            g_il_state=IL_NOT_READY;
            break;
        }
        if (CheckActionKey() == RADON_KEY)    //RadOnKey())
        {
            CleanActionKey(RADON_KEY);
            SendRadOnCommands();
            emit base->g_sipdog_stop_signal();

            remote->isMlcMoveStop = false;

            state= RAD_ON;
            g_status = RAD_ON;
            mu = 0;
            g_il_state=IL_RADON;

            g_treat_time=0.0f;
            dosei->ClearPcRadonTime();
            g_radon_time.start();

            //ZZ 17-3-8 便于debug
            emit base->g_dosei_start_signal(g_rad_on_delay_time.toInt());

            break;
        }
        if (CheckActionKey() == CLEAR_KEY)
        {
            CleanActionKey(CLEAR_KEY);
            SendClearCommands();
            state= PROGRAM;
            g_status = PROGRAM;
            DeleteReadyInterlock();
            g_il_state=IL_PROGRAM;

            remote->ResetSetting(WAIT_ACCEPT);
            emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);
            upper->SendPgmReach();
            mlc->SendPgmReach();
            break;
        }
        break;
        //开始治疗的2秒钟之后进入
    case RAD_ON:
        motor->CheckMotorSetActPos();
        if (CheckActionKey() == RADOFF_KEY)    //RadOffKey())
        {
            CleanActionKey(RADOFF_KEY);
            //向各从机发送interrupt命令3->1->2->5->6->7
            SendInterruptCommands();
            mlc->InsertInterlockNoErr(124, SYSTEM_IL, IL_ALL, NOT_LATCHED);
            emit base->g_sipdog_start_signal();

            state= INTERRUPT;
            g_status = INTERRUPT;
            g_il_state=IL_INTERRUPT;

            dosei->SyncPcRadonTime();
            emit base->g_dosei_stop_signal();
            emit base->g_doseii_stop_signal();
            emit base->g_commdog_stop_signal();
            if (g_is_rotate_treatment)
            {
                g_gantry_timer->stop();
            }

            break;
        }
        if (CheckActionKey() == PROGRAM_KEY || mc_interlock_status == ILS_SAFETY)  //ProgramKey()
            //if (mc_interlock_status == ILS_SAFETY)  //ProgramKey()
        {
            CleanActionKey(PROGRAM_KEY);
            SendTerminateCommands(true);
            state= TERMINATE1;
            g_status = TERMINATE1;
            g_il_state=IL_TERMINATE;
            //ProcessTerminate();
            emit base->g_dosei_stop_signal();
            emit base->g_doseii_stop_signal();
            emit base->g_commdog_stop_signal();
            emit base->g_sipdog_start_signal();
            if (g_is_rotate_treatment)
            {
                g_gantry_timer->stop();
            }
            dosei->dose_rate_new = false;
            doseii->dose_rate_new = false;

            break;
        }
        if (mc_interlock_status == ILS_SYSTEM)
        {
            //向各从机发送interrupt命令3->1->2->5->6->7
            SendInterruptCommands();
            dosei->SyncPcRadonTime();
            mlc->InsertInterlockNoErr(124, SYSTEM_IL, IL_ALL, NOT_LATCHED);

            state= INTERRUPT_NR;
            g_status = INTERRUPT_NR;
            g_il_state=IL_INTERRUPT_NR;
            emit base->g_dosei_stop_signal();
            emit base->g_doseii_stop_signal();
            emit base->g_commdog_stop_signal();
            emit base->g_sipdog_start_signal();
            if (g_is_rotate_treatment)
            {
                g_gantry_timer->stop();
            }
            // ProcessInterrupt();

            break;
        }

#ifdef DEBUG_MODE
        msleep(500);
    {
        int meterset = 0;

        if (upper->GetBeamType() == BT_DYNAMIC) {
            BEAMDATA_ST beamData;
            upper->GetCurBeamData(beamData);
            meterset = beamData.finalMeterSet;
        } else {
            CONTROLPOINT_ST ctrlpointData;
            upper->GetCurCtrlPointData(ctrlpointData);
            meterset = ctrlpointData.meterSet;
        }

        int tmp = 2;

        if (mu+tmp >= meterset)
            mu = meterset;
        else
            mu += tmp;

        upper->SetActDose(mu);
        mlc->SetActDose(mu);
        emit remote->updateDosProgressBar(mu);
        qDebug() << mu << endl;

        if (mu == meterset)
        {
            g_is_complete = true;
            mu = 0;
        }
    }
#endif

        if (RadComplete())
        {
            upper->SendLastDos();

            SendTerminateCommands(false);
            //SendCheck2Commands();

            if ( !upper->HasNextData() )
            {
                upper->SendTreatEnd();
                mlc->SendTreatEnd();
            }

            emit remote->updateSegProgressBar();

            state= COMPLETE1;
            g_status = COMPLETE1;
            g_il_state=IL_COMPLELE;
            emit base->g_dosei_stop_signal();
            emit base->g_doseii_stop_signal();
            emit base->g_commdog_stop_signal();
            emit base->g_sipdog_start_signal();

            if (!remote->isMlcMoveStop)
                emit base->g_mlc_movestop_start_signal();

            if (g_is_rotate_treatment)
            {
                g_gantry_timer->stop();
            }
            dosei->dose_rate_new = false;
            doseii->dose_rate_new = false;
            g_is_complete = false;
            //ProcessComplete();

            break;
        }
        beam->SendCommDog();
        break;

    case INTERRUPT:
        io->CheckAccInterlock();
        if (mc_interlock_status == ILS_SYSTEM)
        {
            SendReadyCommands(INTERRUPTNOTREADY);
            state = INTERRUPT_NR;
            g_status = INTERRUPT_NR;
            g_il_state=IL_INTERRUPT_NR;
            break;
        }
        if (mc_interlock_status == ILS_SAFETY || CheckActionKey() == F1_KEY)
        {
            CleanActionKey(F1_KEY);
            SendTerminateCommands(true);
            if (g_right_key==RADON_RKEY )
            {
                state= TERMINATE1;
                g_status = TERMINATE1;
                g_il_state=IL_TERMINATE;
            }
            else if (g_right_key==PGM_RKEY)
            {
                state=TERMINATE2;
                g_status=TERMINATE2;
                g_il_state=IL_TERMINATE;
            }
            break;
        }
        if (CheckActionKey() == RADON_KEY)    //RadOnKey())
        {
            CleanActionKey(RADON_KEY);
            SendRadOnCommands();
            state= RAD_ON;
            g_status = RAD_ON;
            //g_il_state=IL_RADON;
            g_radon_time.restart();

            emit base->g_dosei_start_signal(g_rad_on_delay_time.toInt());

            emit base->g_sipdog_stop_signal();
            break;
        }
        break;

    case INTERRUPT_NR:
        io->CheckAccInterlock();
        if (mc_interlock_status == ILS_CLEARED)
        {
            // send ready status interrupt
            SendReadyCommands(INTERRUPTREDY);
            state=INTERRUPT;
            g_status = INTERRUPT;
            g_il_state=IL_INTERRUPT;
            break;
        }
        if (mc_interlock_status == ILS_SAFETY || CheckActionKey() == F1_KEY)
        {
            CleanActionKey(F1_KEY);
            SendTerminateCommands(true);
            if (g_right_key==RADON_RKEY )
            {
                state= TERMINATE1;
                g_status = TERMINATE1;
                g_il_state=IL_TERMINATE;
            }
            else if (g_right_key==PGM_RKEY)
            {
                state=TERMINATE2;
                g_status=TERMINATE2;
                g_il_state=IL_TERMINATE;
            }
            break;
        }
        break;

    case COMPLETE1:
        if (CheckActionKey() == PROGRAM_KEY)
        {
            CleanActionKey(PROGRAM_KEY);

            emit base->Status_Changed();
            accui->SaveLastTreatData();//ljy-11-9
            g_treat_finish=true;

            if (!g_check2_bypass)
            {
                emit base->Prompt_Msg(MSG_WAIT);
                //向0、1、2、3、5、6、7发送check #2命令

                g_il_state=IL_CHECK2;
                SendCheck2Commands();

                //msleep(4 * SLEEP_TIME);
                if (!Check2Passed())
                {
                    state=ERRORS;
                    g_status = ERRORS;
                    g_il_state=IL_ERROR;
                }
                else
                {
                    state=COMPLETE2;
                    g_status=COMPLETE2;
                    g_il_state=IL_COMPLELE;
                }
            }
            else
            {
                emit base->Prompt_Msg("Check #2 bypass");
                qDebug()<<"Check #2 bypass";
                state=COMPLETE2;
                g_status=COMPLETE2;
                g_il_state=IL_COMPLELE;
            }

            emit remote->backToProgram();
        }
        break;
    case COMPLETE2:
        emit base->Status_Changed();
        emit base->Prompt_Msg(MSG_WAIT);
        CleanActionKey(CLEAR_KEY);

        g_il_state=IL_PROGRAM;
        //ZZ 16-12-6
        //CleanInterlocks();
        //向各从机发送reset命令
        SendResetCmdN04();

        //interlock->DeleteInterlock(47);
        //msleep(1 * SLEEP_TIME);

        if (RevResetEcho_2())
        {
            SendIOConfigCommands();
            SendOtherCmdNoStop();
            SendUserMode();

            state=PROGRAM;
            g_status=PROGRAM;

            if (upper->HasNextData())
            {
#ifdef PRINT_REMOTE_DEBUG
                qDebug() << "next beam" << endl;
#endif
                remote->mCurSegId++;
                upper->NextData();
                remote->ResetSetting(SET_TREATDATA);
            }
            else
            {
#ifdef PRINT_REMOTE_DEBUG
                qDebug() << "this treat end" << endl;
#endif
                remote->mCurSegId = 1;
                //upper->SendPgmReach();
                remote->ResetSetting(WAIT_TREATDATA);
                emit remote->resetProgress();
                emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);
            }

            motor->DeleteInterlock(9);
            motor->DeleteInterlock(10);
            motor->DeleteInterlock(49);
            motor->DeleteInterlock(51);
            g_il_state=IL_PROGRAM;
        }
        else
        {
            state=ERRORS;
            g_status=ERRORS;
            g_il_state=IL_ERROR;
        }
        break;
    case TERMINATE1:
        if (CheckActionKey() == PROGRAM_KEY)
        {
            CleanActionKey(PROGRAM_KEY);
            state = TERMINATE2;
            g_status=TERMINATE2;
            g_il_state=IL_TERMINATE;
            //emit base->Prompt_Msg("Please turn left key switch to RESET.");
        }
        break;
    case TERMINATE2:
        if (CheckActionKey() == RESET_KEY)    //ResetKey())
        {
            CleanActionKey(RESET_KEY);
            //emit base->Prompt_Msg("Waiting ......");
            if (!g_check2_bypass)
            {
                g_il_state=IL_CHECK2;
                SendCheck2Commands();
                //msleep(4 * SLEEP_TIME);
                if (!Check2Passed())
                {
                    msleep(200);
                    state=ERRORS;
                    g_status = ERRORS;
                    g_il_state=IL_ERROR;
                }
                else
                {
                    state=TERMINATE3;
                    g_status = TERMINATE3;
                    g_il_state=IL_TERMINATE;
                }
                break;
            }
            else
            {
                emit base->Prompt_Msg("Check #2 bypass");
                qDebug()<<"Check #2 bypass";
                state=TERMINATE3;
                g_status = TERMINATE3;
                g_il_state=IL_TERMINATE;
            }
        }
        break;

    case TERMINATE3:
        if (CheckActionKey() == CLEAR_KEY)
        {
            CleanActionKey(CLEAR_KEY);
            emit base->Prompt_Msg(MSG_WAIT);
            //向各从机发送reset命令
            g_il_state=IL_PROGRAM;
            CleanInterlocks();
            SendResetCmdN04();

            //msleep(1 * SLEEP_TIME);
            if (RevResetEcho_2())
            {
                emit base->Prompt_Msg(MSG_WAIT);
                SendIOConfigCommands();
                SendOtherCmdNoStop();
                SendUserMode(false);
                state=PROGRAM;
                g_status=PROGRAM;
                DeleteReadyInterlock();
                g_il_state=IL_PROGRAM;

                remote->ResetSetting(WAIT_TREATDATA);
                emit remote->setRemoteMsg(MSG_REMOTE_CANCEL);
            }
            else
            {
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
            break;
        }
        break;
    case ERRORS:
        if (CheckActionKey() == RESET_KEY)    //ResetKey())
        {
            CleanActionKey(RESET_KEY);
            CleanInterlocks();
            //向各从机发送reset命令
            if (ResetCmd())
            {
                state=NULL_STATUS1;
                g_status=NULL_STATUS1;
                g_il_state=IL_NULL;
                g_usermode=RESET_MODE;

                remote->ResetSetting(INIT);
            }
            else
            {
                msleep(200);
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
        }
        break;
    default:
        break;
    }
}
