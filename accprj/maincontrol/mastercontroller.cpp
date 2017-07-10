#include "mastercommon.h"

bool g_is_terminate;         //terminate标志
bool g_is_complete;          //complete标志
bool g_treat_finish=false;          //完成一次治疗后才能使用F9调回治疗参数

enum Status g_status;
UINT32 g_il_state;
RE_TE_MODE g_mode;

QMutex g_action_key_mutex;
UINT32 g_action_key = NULL_KEY;

extern QTimer *g_gantry_timer;
extern QString g_rad_on_delay_time;

QWaitCondition g_maincontrol_wait;

MasterController::MasterController()
{

}

MasterController::~MasterController()
{
}

void MasterController::NormalMasterCtrl(enum Status &state, enum Status &last_state)
{
    mc_interlock_status = CheckInterlock();

    switch (state)
    {
    case POWER_UP:

        can->CanTransmitHostStart();

        msleep(SLEEP_TIME);

        if (!isSipNormalWork)
        {
            emit base->Prompt_Msg("SIP no echo");
            break;
        }
        can->CanTransmitStartSyn();
        CleanInterlocks();
        msleep(SLEEP_TIME);
        if (ResetCmd())
        {
            msleep(SLEEP_TIME);
            //CleanInterlocks();
            state=NULL_STATUS1;
            g_status=NULL_STATUS1;
            g_il_state=IL_NULL;
            emit base->g_sipdog_start_signal();
            //emit base->Prompt_Msg("Please turn right key switch to STD BY then to PGM/RDY");
        }
        else
        {
            state=ERRORS;
            g_status=ERRORS;
            g_il_state=IL_ERROR;
            emit base->g_sipdog_start_signal();
        }
        //            msleep(SLEEP_TIME);
        //            CleanInterlocks();
        break;
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
            emit remote->showRemoteControl(false);
            emit base->ProcessShowUISig();
        }
        if (CheckActionKey() ==F3_KEY)
        {
            SendUserMode();
            CleanActionKey(F3_KEY);
            state=PROGRAM;
            g_status=PROGRAM;
            g_il_state=IL_PROGRAM;
            emit remote->showRemoteControl(false);
            emit base->ProcessShowUISig();
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
        if (CheckActionKey() == REMOTE_KEY)
        {
            CleanActionKey(REMOTE_KEY);
            if (PGM_RKEY == g_right_key)
            {
#ifdef PRINT_REMOTE_DEBUG
                qDebug() << "enter remote mode" << endl;
#endif

                upper->SendConnectReq();
                mlc->SendConnectReq();

                emit base->g_mlccon_start_signal();
                emit base->g_upcon_start_signal();

                g_is_remote_mode = true;
                remote->StartHeatBeat();

                emit remote->setDataThread.resetUISelectStep();
            }

            break;
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
            }
            else
            {
                state=ERRORS;
                g_status=ERRORS;
                g_il_state=IL_ERROR;
            }
            break;
        }
        if (CheckActionKey() == STANDBY_KEY)    //StandbyKey())
        {
            CleanActionKey(STANDBY_KEY);
            state=STANDBY;
            g_status = STANDBY;
            g_il_state=IL_PROGRAM;
            break;
        }
        if (CheckActionKey() == ACCEPT_KEY)    //AcceptKey())
        {
            accui->resetMoveStatus();
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
        if (CheckActionKey() == C_KEY && !g_check2_bypass)
        {
            CleanActionKey(C_KEY);
            state=CHECK2_1;
            g_status = CHECK2_1;
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
        if (mc_interlock_status != ILS_CLEARED)
            SendReadyCommands(NOTREADY);
        state=NOT_READY;
        g_status=NOT_READY;
        g_il_state=IL_NOT_READY;

#ifdef AUTHORIZATION
        if (!g_auth_check)
            io->InsertInterlockNoErr(0, SYSTEM_IL, IL_ALL, LATCHED);
#endif

        break;
    case NOT_READY:
        motor->CheckMotorSetActPos();
        io->CheckAccInterlock();
        if (mc_interlock_status == ILS_CLEARED)
        {
            SendReadyCommands(READY);
            state=READYS;
            g_status = READYS;
            g_il_state=IL_READY;
            qDebug()<<"NOT_READY no interlock";
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

            state= RAD_ON;
            g_status = RAD_ON;
            g_il_state=IL_RADON;

            g_treat_time=0.0f;
            dosei->ClearPcRadonTime();
            g_radon_time.start();

            //ZZ 16-11-8
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
        if (RadComplete())
        {
            SendTerminateCommands(false);
            //SendCheck2Commands();
            state= COMPLETE1;
            g_status = COMPLETE1;
            g_il_state=IL_COMPLELE;
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

            //ZZ 16-11-9
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
        }
        break;
    case COMPLETE2:
        if (CheckActionKey() == CLEAR_KEY)
        {
            emit base->Prompt_Msg(MSG_WAIT);
            CleanActionKey(CLEAR_KEY);

            g_il_state=IL_PROGRAM;
            CleanInterlocks();
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
                SendUserMode();
                state=PROGRAM;
                g_status=PROGRAM;
                DeleteReadyInterlock();
                g_il_state=IL_PROGRAM;
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


/*状态机*/
//切换状态时使用clear函数清除全局标志
void MasterController::run()
{
    enum Status state,last_state;    

#ifdef OFFLINE
    state=PROGRAM;
    g_status = PROGRAM;
    last_state = PROGRAM;
#else
    state=POWER_UP;
    g_il_state=IL_POWUP;
    g_status = POWER_UP;
    last_state = POWER_UP;
#endif

    msleep(SLEEP_TIME);
    while (1)
    {
        if(last_state!=state)
        {
            qDebug()<<"state"<<state;
            emit base->Status_Changed();
            last_state=state;
        }
        else
        {
            mc_wait_mutex.lock();
            g_maincontrol_wait.wait(&mc_wait_mutex, (UINT32)500);
            mc_wait_mutex.unlock();
        }

        if (g_is_remote_mode)
        {
            RemoteMasterCtrl(state, last_state);
        }
        else
        {
            NormalMasterCtrl(state, last_state);
        }
    }
}

UINT32 MasterController::CheckActionKey()
{
    UINT32 symbol;
    QMutexLocker locker(&g_action_key_mutex);
    symbol = g_action_key;

    /*if (g_is_remote_mode)
    {
        if (!remote->CheckKeyValidity(symbol))
            return NULL_KEY;
    }*/

    return symbol;
}

void MasterController::CleanActionKey(UINT32 key)
{
    //UINT32 tmp = key;
    QMutexLocker locker(&g_action_key_mutex);
    g_action_key = NULL_KEY;
    //g_action_key &= ~tmp;
}

void MasterController::CleanInterlocks()
{
    for (int i = 0; i < INTERLOCK_NUM; i++)
    {
        if (g_lock_bits.testBit(i) == true)
        {
            qDebug()<<"Delete interlock"<<i;
            g_interlocks[i].latched = NOT_LATCHED;
            motor->DeleteInterlock(i);
        }
    }
    io->has_inl19=false;
    io->has_inl20=false;
    io->has_inl52=false;
}

CHECK_IL_RES_T  MasterController::CheckInterlock()
{
    //XXX
    QBitArray tmp_bits;
    static QBitArray  lst_pass_lock_bit;
    static QBitArray  lst_lock_bit=g_lock_bits;

    if(g_usermode==SERVICE_MODE)
    {
        tmp_bits = g_lock_bits & ~g_lock_passby_bits;
    }
    else if(g_usermode==TREATMENT_MODE)
    {
        tmp_bits = g_lock_bits;
    }
    if ((lst_lock_bit !=g_lock_bits) || (lst_pass_lock_bit !=tmp_bits))
    {
        lst_lock_bit=g_lock_bits;
        lst_pass_lock_bit=tmp_bits;
        emit base->interlock_redisp_signal();
    }

    if (tmp_bits == g_zero_bits)
        return ILS_CLEARED;

    if ((tmp_bits & g_safety_il_bits) != g_zero_bits)
        return ILS_SAFETY;

    return ILS_SYSTEM;

}

bool MasterController::RevIOAcknowledge()
{
#ifdef DEBUG_MODE
    return true;
#endif

    for (int i=0; i<10; i++)
    {
        if (g_io_acknowledge == true)
        {
            g_io_acknowledge = false;
            msleep(100);
            return true;
        }
        else
        {
            msleep(100);
        }
    }

    //未收到io的k命令，产生interlock#15
    io->InsertInterlock(15, 860, SAFETY_IL, IL_NOT_READY, LATCHED);
    return false;
}

bool MasterController::RadComplete()
{
    //收到dosei或doseii的Last total dose transmission
    return g_is_complete;
}

bool MasterController::RevDoseOffset()
{
    return false;
}

bool MasterController::RevResetEcho_1()
{
#ifdef DEBUG_MODE
    return true;
#endif

    //接收到各个从机的reset echo
    QBitArray c0_echo(8);
    QBitArray tmp(8);

    tmp.fill(true);
    for (int i=0; i<10; i++)
    {
        g_c0_echo_bits_lock.lockForRead();
        c0_echo = g_c0_echo_bits;
        g_c0_echo_bits_lock.unlock();

        if (c0_echo==tmp)
        {
            g_c0_echo_bits.fill(false);
            msleep(200);
            return true;
        }
        else
        {
            msleep(200);
        }
    }
    g_c0_echo_bits.fill(false);

    /*
  910   C0 echo received from controller 0 .
  911   C0 echo received from controller 1 .
  912   C0 echo received from controller 2 .
  913   C0 echo received from controller 3 .
  914   C0 echo received from controller 4 .
  915   C0 echo received from controller 5 .
  916   C0 echo received from controller 6 .
  917   C0 echo received from controller 7 .
  918   C0 echo received from controller 8 .
  919   C0 echo received from controller 9 .
  */
    if (c0_echo[0] == 0)
        io->InsertInterlock(79, 910, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[1] == 0)
        io->InsertInterlock(79, 911, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[2] == 0)
        io->InsertInterlock(79, 912, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[3] == 0)
        io->InsertInterlock(79, 913, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[4] == 0)
        io->InsertInterlock(79, 914, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[5] == 0)
        io->InsertInterlock(79, 915, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[6] == 0)
        io->InsertInterlock(79, 916, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[7] == 0)
        io->InsertInterlock(79, 917, ERROR_IL, IL_PROGRAM, LATCHED);

    return false;
}

bool MasterController::Check1Passed()
{
#ifdef DEBUG_MODE
    return true;
#endif

    QBitArray check1(8), tmp(8, true);

    tmp.setBit(4, false);
    for (int i=0; i<15;i++)
    {
        g_check_bits_mutex.lock();
        check1 = g_check_bits;
        g_check_bits_mutex.unlock();
        if (check1==tmp)
        {
            g_check_bits.fill(false);
            msleep(50);
            return true;
        }
        else
        {
            msleep(500);
        }
    }
    g_check_bits.fill(false);

    //否则产生Interlock
    return false;
}

bool MasterController::RevResetEcho_2()
{
#ifdef DEBUG_MODE
    return true;
#endif

    //接收到各个从机的reset echo
    QBitArray c0_echo(8);
    QBitArray tmp(8);
    tmp.fill(true);

    tmp.setBit(0, false);
    tmp.setBit(4, false);

    for (int i=0; i<10; i++)
    {
        g_c0_echo_bits_lock.lockForRead();
        c0_echo = g_c0_echo_bits;
        g_c0_echo_bits_lock.unlock();
        if (c0_echo == tmp)
        {
            g_c0_echo_bits.fill(false);
            msleep(50);
            return true;
        }
        else
        {
            msleep(100);
        }
    }
    g_c0_echo_bits.fill(false);

    /*
  911   C0 echo received from controller 1 .
  912   C0 echo received from controller 2 .
  913   C0 echo received from controller 3 .
  915   C0 echo received from controller 5 .
  916   C0 echo received from controller 6 .
  917   C0 echo received from controller 7 .
  */
    if (c0_echo[1] == 0)
        io->InsertInterlock(79, 911, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[2] == 0)
        io->InsertInterlock(79, 912, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[3] == 0)
        io->InsertInterlock(79, 913, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[5] == 0)
        io->InsertInterlock(79, 915, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[6] == 0)
        io->InsertInterlock(79, 916, ERROR_IL, IL_PROGRAM, LATCHED);
    if (c0_echo[7] == 0)
        io->InsertInterlock(79, 917, ERROR_IL, IL_PROGRAM, LATCHED);

    return false;
}

void MasterController::SendClearCommands()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    io->SendClear();
    interlock->SendClear();
    beam->SendClear();
    dosei->SendClear();

    if (g_is_remote_mode)
    {
        //upper->SendClear();
        //mlc->SendClear();
    }
}

void MasterController::SendC0Commands()
{
    motor->SendC0();
    dosei->SendC0();
    doseii->SendC0();
    beam->SendC0();
    hand->SendC0();
    light->SendC0();
    io->SendC0();
    interlock->SendC0();
}

bool MasterController::Check2Passed()
{
#ifdef DEBUG_MODE
    return true;
#endif

    QBitArray check2(8), tmp(8, true);

    tmp.setBit(4, false);  //do not check slave4

    for (int i=0; i<10; i++)
    {
        g_check_bits_mutex.lock();
        check2 = g_check_bits;
        g_check_bits_mutex.unlock();

        if (check2 == tmp)
        {
            g_check_bits.fill(false);
            msleep(200);
            return true;
        }
        else
        {
            msleep(400);
        }
    }

    g_check_bits.fill(false);
    return false;

}

/*发送组合命令的方法*/
//发送Accept命令组合
void MasterController::SendAcceptCommands()
{
    // UINT16 param_bkup_conver_factor;
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);

    //停止所有motors
    motor->SendStop(ALL_MOTORS);
    STEPDELY;

    //向motor、IO、ineterlock、beam、light发送Accept命令
    motor->SendAccept();
    STEPDELY;
    io->SendAccept();
    STEPDELY;
    interlock->SendAccept();
    STEPDELY;
    beam->SendAccept();
    STEPDELY;
    light->SendAccept();
    STEPDELY;

    if (g_is_remote_mode)
    {
        upper->SendAccept();
        STEPDELY;
        //mlc->SendAccept();
        //STEPDELY;
    }

    //向dosei和doseii发送information命令
    dosei->SendInformation();
    STEPDELY;
    doseii->SendInformation();
    STEPDELY;
    //向dosei发送ParamBackupConverFact命令
    dosei->SendParamBackupconverFact();
    STEPDELY;

    //LJY-9-23
    if(g_ray_type==ROT_X_HI || g_ray_type==ROT_X_LO)
    {
        dosei->SendRotData();
    }
}

//发送check#1命令组合
void MasterController::SendCheck1Commands()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各个从机(除hand)发送check #1命令
    motor->SendCheckRequest(CHECK1);
    dosei->SendCheckRequest(CHECK1);
    doseii->SendCheckRequest(CHECK1);
    beam->SendCheckRequest(CHECK1);
    light->SendCheckRequest(CHECK1);
    interlock->SendCheckRequest(CHECK1);
    io->SendCheckRequest(CHECK1);

    /*if (g_is_remote_mode)
    {
        upper->SendCheckRequest(CHECK1);
        mlc->SendCheckRequest(CHECK1);
    }*/

    g_check_index_mutex.lock();
    g_check_index = CHECK1;
    g_check_index_mutex.unlock();
}

void MasterController::SendCheck2Commands()   //有问题？？？？？？？？？？
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各个从机(除hand)发送check #2命令，
    //其中，向某个从机发送check #2 with watchdog
    //发送check2每次都有一个是带watchdog的，每个从机依次带一个watchdog
    static int wdindex = 0;

    wdindex == 0 ? motor->SendCheckRequest(CHECK2_WATCHDOG):
                   motor->SendCheckRequest(CHECK2);
    wdindex == 1 ? dosei->SendCheckRequest(CHECK2_WATCHDOG):
                   dosei->SendCheckRequest(CHECK2);
    wdindex == 2 ? doseii->SendCheckRequest(CHECK2_WATCHDOG):
                   doseii->SendCheckRequest(CHECK2);
    wdindex == 3 ? beam->SendCheckRequest(CHECK2_WATCHDOG):
                   beam->SendCheckRequest(CHECK2);
    wdindex == 5 ? light->SendCheckRequest(CHECK2_WATCHDOG):
                   light->SendCheckRequest(CHECK2);
    wdindex == 6 ? interlock->SendCheckRequest(CHECK2_WATCHDOG):
                   interlock->SendCheckRequest(CHECK2);
    wdindex == 7 ? io->SendCheckRequest(CHECK2_WATCHDOG):
                   io->SendCheckRequest(CHECK2);

    /*if (g_is_remote_mode)
    {
        wdindex == 8 ? upper->SendCheckRequest(CHECK2_WATCHDOG):
                       upper->SendCheckRequest(CHECK2);
        wdindex == 9 ? mlc->SendCheckRequest(CHECK2_WATCHDOG):
                       mlc->SendCheckRequest(CHECK2);
    }*/

    wdindex = (wdindex + 1) % 8;

    g_check_index_mutex.lock();
    g_check_index = CHECK2;
    g_check_index_mutex.unlock();
}

void MasterController::SendReadyCommands(READYSTATE_TYPE_T readystate)
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各从机发送Ready(除hand)
    motor->SendReadyStatus(readystate);
    dosei->SendReadyStatus(readystate);
    doseii->SendReadyStatus(readystate);
    beam->SendReadyStatus(readystate);
    light->SendReadyStatus(readystate);
    interlock->SendReadyStatus(readystate);
    io->SendReadyStatus(readystate);

    if (g_is_remote_mode)
    {
        upper->SendReadyStatus(readystate);
        mlc->SendReadyStatus(readystate);
    }
}

//发送RadOn命令组合
void MasterController::SendRadOnCommands()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //Stop all motors
    motor->SendStop(ALL_MOTORS);

    //rad on: 3->1->2->5->6->7
    //beam rad on
    beam->SendRadOn();

    //dosei doseii rad on
    dosei->SendRadOn();
    doseii->SendRadOn();

    //light interlock io rad on
    light->SendRadOn();
    interlock->SendRadOn();
    io->SendRadOn();

    if (g_is_remote_mode)
    {
        upper->SendRadOn();
        mlc->SendRadOn();
    }

    if(g_ray_type==ROT_X_HI || g_ray_type==ROT_X_LO)
    {
        motor->SendMoveInRadOn();
    }
}

void MasterController::SendInterruptCommands()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各从机发送interrupt命令7->3->6->5->1->2
    io->SendInterrupt();
    beam->SendInterupt();
    interlock->SendInterrupt();
    light->SendInterrupt();
    dosei->SendInterrupt();
    doseii->SendInterrupt();

    if (g_is_remote_mode)
    {
        upper->SendInterrupt();
        mlc->SendInterrupt();
    }
}

void MasterController::SendTerminateCommands(bool terminate)
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各从机发送terminate命令7->3->6->5->1->2
    io->SendTerminateOrComplete();
    beam->SendTerminateOrComplete();
    interlock->SendTerminateOrComplete();
    light->SendTerminateOrComplete();
    dosei->SendTerminateOrComplete();
    doseii->SendTerminateOrComplete();

    /*if (g_is_remote_mode)
    {
        if (terminate)
        {
            upper->SendTerminateOrComplete(true);
            mlc->SendTerminateOrComplete(true);
        }
        else
        {
            if ( upper->HasNextData() )
            {
                mlc->SendTerminateOrComplete(false);
            }

            upper->SendTerminateOrComplete(false);
        }
    }*/

    if (g_is_remote_mode)
    {
        if (terminate)
        {
            upper->SendTerminateOrComplete(true);
            mlc->SendTerminateOrComplete(true);
        }
        else
        {
            if ( upper->HasNextData() )
            {
                upper->SendTerminateOrComplete(false);
                mlc->SendTerminateOrComplete(false);
            }
        }
    }

    //向motor发送stop all motors
    motor->SendStop(ALL_MOTORS);

    light->SendBeamShieldStop();  // MOTION STOP need this cmd, seem it is not need in other
}

void MasterController::SendResetCmdAll()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    //向各从机发送select mode reset命令 0->1->2->3->4->5->6->7
    motor->SendUserMode(RESET_MODE);
    STEPDELY;
    dosei->SendUserMode(RESET_MODE);
    STEPDELY;
    doseii->SendUserMode(RESET_MODE);
    STEPDELY;
    beam->SendUserMode(RESET_MODE);
    STEPDELY;
    hand->SendUserMode(RESET_MODE);
    STEPDELY;
    light->SendUserMode(RESET_MODE);
    STEPDELY;
    interlock->SendUserMode(RESET_MODE);
    STEPDELY;
    io->SendUserMode(RESET_MODE);
    STEPDELY;

    if (g_is_remote_mode)
    {
        STEPDELY;
        upper->SendUserMode(RESET_MODE);
        STEPDELY;
        mlc->SendUserMode(RESET_MODE);
        STEPDELY;
    }
}

void MasterController::SendResetCmdN04()
{
    //向各从机发送Reset命令1->2->3->5->6->7
    dosei->SendUserMode(RESET_MODE);
    STEPDELY;
    doseii->SendUserMode(RESET_MODE);
    STEPDELY;
    beam->SendUserMode(RESET_MODE);
    STEPDELY;
    light->SendUserMode(RESET_MODE);
    STEPDELY;
    interlock->SendUserMode(RESET_MODE);
    STEPDELY;
    io->SendUserMode(RESET_MODE);
    STEPDELY;

    /*if (g_is_remote_mode)
    {
        upper->SendUserMode(RESET_MODE);
        STEPDELY;
        mlc->SendUserMode(RESET_MODE);
        STEPDELY;
    }*/
}


void MasterController::SendMotorConfigCommands()
{
    //向motor发送reset之后的配置命令
    motor->SendSlowDownSpeed();
    STEPDELY;
    motor->SendSlowDownDistance();
    STEPDELY;
}

void MasterController::SendHandConfigCommands()
{
    //向hand发送reset之后的配置命令
    hand->SendConfiguration();
    STEPDELY;
}

void MasterController::SendIOConfigCommands()
{
    //向IO发送reset之后的配置命令
    /*
    E1                  = 0x01,
    E2                  = 0x02,
    E3                  = 0x03,
    E4                  = 0x04,
    E5                  = 0x05,
    E6                  = 0x06,
    X_LO                = 0x08,
    X_HI                = 0x09,
    FILM                = 0x0A,
    */

    io->SendFoilTable(E1);
    STEPDELY;
    io->SendFoilTable(E2);
    STEPDELY;
    io->SendFoilTable(E3);
    STEPDELY;
    io->SendFoilTable(E4);
    STEPDELY;
    io->SendFoilTable(E5);
    STEPDELY;
    io->SendFoilTable(E6);
    STEPDELY;

    io->SendTap(E1);
    STEPDELY;
    io->SendTap(E2);
    STEPDELY;
    io->SendTap(E3);
    STEPDELY;
    io->SendTap(E4);
    STEPDELY;
    io->SendTap(E5);
    STEPDELY;
    io->SendTap(E6);
    STEPDELY;
    io->SendTap(X_LO);
    STEPDELY;
    io->SendTap(X_HI);
    STEPDELY;
    io->SendTap(FILM);
    STEPDELY;
}

//发送开机、reset时的末尾几个命令的组合
void MasterController::SendOtherCmd()
{
    //向motor发送error mask
    motor->SendErrorMask();
    STEPDELY;
    //向motor, dosei, doseii发送max jaws speed & collimator type命令
    motor->SendMaxJawSpeedColliType();
    STEPDELY;
    dosei->SendMaxJawSpeedAndColType();
    STEPDELY;
    doseii->SendMaxJawSpeedAndColType();
    STEPDELY;
    motor->SendStop(ALL_MOTORS);
    STEPDELY;
    light->SendBeamShieldStop();
    STEPDELY;
}

void MasterController::ProcessComplete()
{
    emit base->g_commdog_stop_signal();
    dosei->dose_rate_new = false;
    doseii->dose_rate_new = false;
    g_is_complete = false;
}

void MasterController::ProcessTerminate()
{

}

void MasterController::ProcessInterrupt()
{
    emit base->g_commdog_stop_signal();
}

//bool MasterController::CheckAcc()
//{
//    int type=g_ray_type & (0xF);
//    if(type < 7)                        //电子线
//    {
//        if(!io->acc3_exist || io->acc2_exist || io->acc1_exist)
//        {
//            //电子线下无限光筒，或有契型板
//            io->InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
//            return false;
//        }
//    }
//    else if(type == 8 || type == 9)     //X射线
//    {
//        if(io->acc3_exist)
//        {
//            //X射线下有限光筒
//            io->InsertInterlock(19, SYSTEM_IL, IL_READY, NOT_LATCHED);
//            return false;
//        }
//    }
//    return true;
//}

//发送开机、reset时的末尾几个命令的组合
void MasterController::SendOtherCmdNoStop()
{
    //向motor发送error mask
    motor->SendErrorMask();
    STEPDELY;
    //向motor, dosei, doseii发送max jaws speed & collimator type命令
    motor->SendMaxJawSpeedColliType();
    dosei->SendMaxJawSpeedAndColType();
    doseii->SendMaxJawSpeedAndColType();

    //    motor->SendStop(ALL_MOTORS);
    //    light->SendBeamShieldStop();
}

//选择模式触发的命令组合
void MasterController::SendRayModeCommands()
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    io->SendTrigger();
    STEPDELY;
    interlock->SendSetup();
    io->SendSetup();

    //发送beam 31-35 命令
    beam->SendAfcpInjeInji();
    STEPDELY;
    beam->SendElmBmiStcLen();
    STEPDELY;
    beam->SendPfnPrfpIpfnSomin();
    STEPDELY;
    beam->SendDataNotUsed();
    STEPDELY;
    beam->SendBmisMinMax();

    dosei->SendDynamicSteer();      //11 44D
    STEPDELY;
    dosei->SendGainAndWorkMode();   //11 47G
    doseii->SendGainAndWorkMode();  //12 48
    STEPDELY;
    doseii->SendSegmentGain();      //12 53
    io->SendAfcLoopCtl();
    STEPDELY;
    io->SendDosLoopCtl();
}

void MasterController::SendUserMode(bool upperSend)
{
    while(base->SendNotFinished())
        msleep(CMDGROUPDLY);
    motor->SendUserMode(g_usermode);
    dosei->SendUserMode(g_usermode);
    doseii->SendUserMode(g_usermode);
    beam->SendUserMode(g_usermode);
    hand->SendUserMode(g_usermode);
    light->SendUserMode(g_usermode);
    interlock->SendUserMode(g_usermode);
    io->SendUserMode(g_usermode);

    if (g_is_remote_mode)
    {
        if (upperSend)
        {
            upper->SendUserMode(g_usermode);
        }

        mlc->SendUserMode(g_usermode);
    }
}
//上电复位命令, 开机画面下旋转RESET按键
bool MasterController::ResetCmd()
{
    SendResetCmdAll();
    //msleep(1 * SLEEP_TIME);
    can->CanTransmitSipReset();

    if (RevResetEcho_1())
    {
        SendMotorConfigCommands();
        SendHandConfigCommands();
        SendIOConfigCommands();
        SendOtherCmd();
        return true;
    }
    else
    {
        SendMotorConfigCommands();
        SendHandConfigCommands();
        SendIOConfigCommands();
        SendOtherCmd();

        base->Prompt_Msg("Not receive reset echo");
        return true;

//        base->Prompt_Msg("Not receive reset echo");
//        return false;
    }
}
void MasterController::DeleteReadyInterlock()
{
    motor->DeleteInterlock(9);
    motor->DeleteInterlock(10);
    motor->DeleteInterlock(49);
    motor->DeleteInterlock(51);
    motor->DeleteInterlock(19);
    motor->DeleteInterlock(20);
    motor->DeleteInterlock(52);

    if (g_is_remote_mode)
    {
        //upper->DeleteInterlock
        //mlc->DeleteInterlock
        for (int i=91; i<=117; i++)
        {
            upper->DeleteInterlock(125, i);
        }
    }

    io->has_inl19=false;
    io->has_inl20=false;
    io->has_inl52=false;
}
