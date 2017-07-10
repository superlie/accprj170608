#include "slave_base.h"
#include "mastercontroller.h"
#include "ui/accui.h"
#include "tools/watchdogtimer.h"
#define PRINT_RECEIVED

#define MOVESTOP_WAIT 4000

extern  AccUi *accui;
extern bool g_compensate;
extern bool g_check1_bypass;
extern bool g_check2_bypass;
extern UINT32 g_il_state;
extern QTimer *g_sipdog_timer;
extern QTimer *g_lighton_timer;
extern QTimer *g_lightrep_timer;
extern QTimer *g_warm_timer;
extern QTimer *g_upcon_timer;
extern QTimer *g_mlccon_timer;
extern QTimer *g_heartbeat_sender;
extern QTimer *g_heartbeat_receiver;
extern QTimer *g_mlc_movestop_timer;
extern bool g_warmup_timeout;
extern float g_treat_time;

EDGE_CHECK_ST g_edge_arr[EDGEARRLEN];

SlaveBase::SlaveBase(QObject *parent):
    QObject(parent)
{
}

bool SlaveBase::open_softpot_file = false;

SlaveBase::~SlaveBase()
{
}

//边界检测参数的初始化
void SlaveBase::BaseInit()
{
    int i;
    for (i = 0; i < EDGEARRLEN; i++)
    {
        g_edge_arr[i].min_value = 0.0;
        g_edge_arr[i].b = 0.0;
    }

    g_edge_arr[AFCP_RSCH_CK].max_value = MAX1;
    g_edge_arr[INJE_RSCH_CK].max_value = MAX1;
    g_edge_arr[INJI_RSCH_CK].max_value = MAX1;
    g_edge_arr[ELM_RSCH_CK].max_value = MAX1;
    g_edge_arr[BMI_RSCH_CK].max_value = MAX1;
    g_edge_arr[BMI_RSCH_CK].min_value = MIN1;
    g_edge_arr[STC_RSCH_CK].max_value = MAX1;
    g_edge_arr[LENS_RSCH_CK].max_value = MAX1;
    g_edge_arr[PFN_RSCH_CK].max_value = MAX1;
    g_edge_arr[IPFN_RSCH_CK].max_value = MAX1;
    g_edge_arr[SOMIN_RSCH_CK].max_value = MAX1;
    g_edge_arr[DSGX_RSCH_CK].max_value = MAX1;
    g_edge_arr[DSGY_RSCH_CK].max_value = MAX1;
    g_edge_arr[DSWX_RSCH_CK].max_value = MAX2;
    g_edge_arr[DSWY_RSCH_CK].max_value = MAX2;
    g_edge_arr[BALX_RSCH_CK].max_value = MAX1;
    g_edge_arr[BALY_RSCH_CK].max_value = MAX1;
    g_edge_arr[ELMCK_RSCH_CK].max_value = MAX3;
    g_edge_arr[ELMCK_RSCH_CK].k = K1 * 100;
    g_edge_arr[BMICK_RSCH_CK].max_value = MAX3;
    g_edge_arr[BMICK_RSCH_CK].k = K1 * 100;
    g_edge_arr[STCCK_RSCH_CK].max_value = MAX1;
    g_edge_arr[STCCK_RSCH_CK].k = K1;
    g_edge_arr[LENCK_RSCH_CK].max_value = MAX5;
    g_edge_arr[LENCK_RSCH_CK].k = K1 * 1000;

    g_edge_arr[MODT_RSCH_CK].max_value = MAX4;
    g_edge_arr[INJT_RSCH_CK].max_value = MAX4;
    g_edge_arr[AFCT_RSCH_CK].max_value = MAX4;
    g_edge_arr[RFT_RSCH_CK].max_value = MAX4;
    g_edge_arr[TST0_RSCH_CK].max_value = MAX4;
    g_edge_arr[TST1_RSCH_CK].max_value = MAX4;

    g_edge_arr[RFT_RSCH_CK].min_value = MIN2;
    g_edge_arr[TST0_RSCH_CK].min_value = MIN2;
    g_edge_arr[TST1_RSCH_CK].min_value = MIN2;

    g_edge_arr[DOSE1_GAIN_CK].max_value = MAX1;
    g_edge_arr[DOSE2_GAIN_CK].max_value = MAX1;
    g_edge_arr[DOSE_RATE1_CK].max_value = MAX1;
    g_edge_arr[DOSE_RATE2_CK].max_value = MAX1;
    g_edge_arr[DOSE_RATE1_OFF].max_value = MAX1;
    g_edge_arr[DOSE_RATE2_OFF].max_value = MAX1;
    g_edge_arr[PRFP_CK].max_value = MAX1;
    g_edge_arr[DRCV_CK].max_value = MAX1;
    g_edge_arr[S0_GAIN_CK].max_value = MAX1;
    g_edge_arr[S1_GAIN_CK].max_value = MAX1;
    g_edge_arr[S2_GAIN_CK].max_value = MAX1;
    g_edge_arr[S3_GAIN_CK].max_value = MAX1;
    g_edge_arr[S4_GAIN_CK].max_value = MAX1;
    g_edge_arr[SWI_CK].max_value = MAX3;
    g_edge_arr[FWI_CK].max_value = MAX3;
    g_edge_arr[DOS_T_CK].max_value = 315.0;
    g_edge_arr[DOS_T_CK].min_value = 5.0;
    g_edge_arr[DOS1_PRE_CK].max_value = 9974;
    g_edge_arr[DOS1_PRE_CK].min_value = 0;
    //只做检查
    g_edge_arr[DOS2_PRE_CK].max_value = 9999;
    g_edge_arr[DOS2_PRE_CK].min_value = 0;
    //
    g_edge_arr[TIMER_PRE_CK].max_value = 99.99;
    g_edge_arr[TIMER_PRE_CK].min_value = 0;
    g_edge_arr[TIMER_PRE_CK].k = 600;
    //同步坞门
    g_edge_arr[JAWX_CK].max_value = 40.0;
    g_edge_arr[JAWY_CK].max_value = 40.0;
    //独立坞门
    g_edge_arr[JAWY1_CK].max_value = 20.0;
    g_edge_arr[JAWY2_CK].max_value = 20.0;
    g_edge_arr[JAWX2_CK].max_value = 20.0;
    g_edge_arr[JAWX1_CK].max_value = 20.0;
    g_edge_arr[JAWY1_CK].min_value = -10.0;
    g_edge_arr[JAWY2_CK].min_value = -10.0;
    g_edge_arr[JAWX2_CK].min_value = -2.0;
    g_edge_arr[JAWX1_CK].min_value = -2.0;

    //10度的余留
    g_edge_arr[GANTRY_CK].min_value = -190.0;
    g_edge_arr[GANTRY_CK].max_value = 190.0;

    //12-04
    g_edge_arr[JAWY1_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;       //2048
    g_edge_arr[JAWY1_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;       //0
    g_edge_arr[JAWY2_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWY2_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[JAWX2_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWX2_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[JAWX1_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWX1_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[GANTRY_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[GANTRY_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[COLLI_SS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[COLLI_SS_RSCH_CK].min_value = MIN_MOTOR_SPEED;

    g_edge_arr[JAWY1_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWY1_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[JAWY2_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWY2_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[JAWX2_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWX2_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[JAWX1_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[JAWX1_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[GANTRY_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[GANTRY_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;
    g_edge_arr[COLLI_FS_RSCH_CK].max_value = MAX_MOTOR_SPEED;
    g_edge_arr[COLLI_FS_RSCH_CK].min_value = MIN_MOTOR_SPEED;


    g_edge_arr[JAWY1_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[JAWY1_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;
    g_edge_arr[JAWY2_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[JAWY2_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;
    g_edge_arr[JAWX2_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[JAWX2_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;
    g_edge_arr[JAWX1_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[JAWX1_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;
    g_edge_arr[GANTRY_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[GANTRY_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;
    g_edge_arr[COLLI_SDS_RSCH_CK].max_value = MAX_MOTOR_SDS;
    g_edge_arr[COLLI_SDS_RSCH_CK].min_value = MIN_MOTOR_SDS;

    g_edge_arr[JAWY1_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[JAWY1_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    g_edge_arr[JAWY2_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[JAWY2_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    g_edge_arr[JAWX2_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[JAWX2_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    g_edge_arr[JAWX1_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[JAWX1_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    g_edge_arr[GANTRY_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[GANTRY_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    g_edge_arr[COLLI_SDD_RSCH_CK].max_value = MAX_MOTOR_SDD;
    g_edge_arr[COLLI_SDD_RSCH_CK].min_value = MIN_MOTOR_SDD;
    //没有


    // Gantry Speed Calibration 瞎猜的
    g_edge_arr[GANTRY_SPEED_FACTOR_CW_CK].min_value = 0.9;
    g_edge_arr[GANTRY_SPEED_FACTOR_CW_CK].max_value = 1.1;
    g_edge_arr[GANTRY_SPEED_FACTOR_CCW_CK].min_value = 0.9;
    g_edge_arr[GANTRY_SPEED_FACTOR_CCW_CK].max_value = 1.1;

    g_edge_arr[AFCP_RSCH_CK].k = K1;
    g_edge_arr[INJE_RSCH_CK].k = K1;
    g_edge_arr[INJI_RSCH_CK].k = K1;
    g_edge_arr[ELM_RSCH_CK].k = K1;
    g_edge_arr[BMI_RSCH_CK].k = K1;
    g_edge_arr[STC_RSCH_CK].k = K1;
    g_edge_arr[LENS_RSCH_CK].k = K1;
    g_edge_arr[PFN_RSCH_CK].k = K1;
    g_edge_arr[IPFN_RSCH_CK].k = K1;
    g_edge_arr[SOMIN_RSCH_CK].k = K1;
    g_edge_arr[DSGX_RSCH_CK].k = K1;
    g_edge_arr[DSGY_RSCH_CK].k = K1;
    g_edge_arr[DSWX_RSCH_CK].k = K2;
    g_edge_arr[DSWY_RSCH_CK].k = K2;
    g_edge_arr[BALX_RSCH_CK].k = K1;
    g_edge_arr[BALY_RSCH_CK].k = K1;
    g_edge_arr[MODT_RSCH_CK].k = K3;
    g_edge_arr[INJT_RSCH_CK].k = K3;
    g_edge_arr[AFCT_RSCH_CK].k = K3;
    g_edge_arr[RFT_RSCH_CK].k = K3;
    g_edge_arr[TST0_RSCH_CK].k = K3;
    g_edge_arr[TST1_RSCH_CK].k = K3;



    g_edge_arr[DOSE1_GAIN_CK].k = K1;
    g_edge_arr[DOSE2_GAIN_CK].k = K1;
    g_edge_arr[DOSE_RATE1_CK].k = K1;
    g_edge_arr[DOSE_RATE2_CK].k = K1;
    g_edge_arr[DOSE_RATE1_OFF].k = K1;
    g_edge_arr[DOSE_RATE2_OFF].k = K1;

    g_edge_arr[PRFP_CK].k = K1;
    g_edge_arr[DRCV_CK].k = K1;

    g_edge_arr[S0_GAIN_CK].k = K1;
    g_edge_arr[S1_GAIN_CK].k = K1;
    g_edge_arr[S2_GAIN_CK].k = K1;
    g_edge_arr[S3_GAIN_CK].k = K1;
    g_edge_arr[S4_GAIN_CK].k = K1;
    g_edge_arr[DOS_T_CK].k = K4;

    //12-04
    g_edge_arr[JAWY1_SS_RSCH_CK].k = 1;
    g_edge_arr[JAWY2_SS_RSCH_CK].k = 1;
    g_edge_arr[JAWX2_SS_RSCH_CK].k = 1;
    g_edge_arr[JAWX1_SS_RSCH_CK].k = 1;
    g_edge_arr[COLLI_SS_RSCH_CK].k = 1;
    g_edge_arr[GANTRY_SS_RSCH_CK].k = 1;

    g_edge_arr[JAWY1_FS_RSCH_CK].k = 1;
    g_edge_arr[JAWY2_FS_RSCH_CK].k = 1;
    g_edge_arr[JAWX2_FS_RSCH_CK].k = 1;
    g_edge_arr[JAWX1_FS_RSCH_CK].k = 1;
    g_edge_arr[COLLI_FS_RSCH_CK].k = 1;
    g_edge_arr[GANTRY_FS_RSCH_CK].k = 1;

    g_edge_arr[JAWY1_SDS_RSCH_CK].k = K5;
    g_edge_arr[JAWY2_SDS_RSCH_CK].k = K5;
    g_edge_arr[JAWX2_SDS_RSCH_CK].k = K5;
    g_edge_arr[JAWX1_SDS_RSCH_CK].k = K5;
    g_edge_arr[COLLI_SDS_RSCH_CK].k = K5;
    g_edge_arr[GANTRY_SDS_RSCH_CK].k = K5;

    g_edge_arr[SWI_CK].k = 1;
    g_edge_arr[FWI_CK].k = 1;

    g_edge_arr[JAWY1_SDD_RSCH_CK].k = 1;
    g_edge_arr[JAWY2_SDD_RSCH_CK].k = 1;
    g_edge_arr[JAWX2_SDD_RSCH_CK].k = 1;
    g_edge_arr[JAWX1_SDD_RSCH_CK].k = 1;
    g_edge_arr[COLLI_SDD_RSCH_CK].k = 1;
    g_edge_arr[GANTRY_SDD_RSCH_CK].k = 1;
}

//计算最后一位的校验和
UINT8 SlaveBase::ComputeLstCHK(UINT8 data[], int len)
{
    //qDebug()<<"ComputeLstCHK:";
    int i;
    UINT8 sum = 0;
    if (len < 1 || len > 19)
        return 0;

    for (i = 0; i < len; i++)
        sum = data[i] + sum;

    sum = sum | 0x80;

    return sum;
}



//命令插入到发送队列
bool SlaveBase::InsertOutQueue(TransRecvT &data)
{
    // outqueue_free.acquire();

    out_queue_mutex.lock();

    //if (outTailP==outFrontP)
    memcpy(&(outQueue[outTailP]), &data, sizeof(data));

    outTailP = (outTailP + 1) % OUTQUEUELEN;
    out_queue_mutex.unlock();
    outqueue_used.release();
    return true;
}

//命令插入到remote发送队列
bool SlaveBase::InsertRemoteOutQueue(TransRecvT &data)
{
    // outqueue_free.acquire();

    rout_queue_mutex.lock();

    //if (outTailP==outFrontP)
    memcpy(&(routQueue[routTailP]), &data, sizeof(data));

    routTailP = (routTailP + 1) % OUTQUEUELEN;
    rout_queue_mutex.unlock();
    routqueue_used.release();
    return true;
}

bool SlaveBase::InsertInQueue(TransRecvT &data)
{
   // inqueue_free.acquire();
    in_queue_mutex.lock();

//    if ((inTailP - inFrontP + 1) % INQUEUELEN == 0)
//    {
//        in_queue_mutex.unlock();
//       // inqueue_free.release();
//        return false;
//    }

    memcpy(&(inQueue[inTailP]), &data, sizeof(data));

    inTailP = (inTailP + 1) % INQUEUELEN;
    in_queue_mutex.unlock();
    inqueue_used.release();
    return true;
}

//从接收队列读取命令
bool SlaveBase::DelInQueue(TransRecvT &data)
{
    inqueue_used.acquire();
    in_queue_mutex.lock();

    memcpy(&data, &(inQueue[inFrontP]), sizeof(data));

    inFrontP = (inFrontP + 1) % INQUEUELEN;
    in_queue_mutex.unlock();
   // inqueue_free.release();
    return true;
}

//分析命令，分配到各个模块。
bool SlaveBase::AnalysRevFram()
{
    TransRecvT tmpRev;
    memset(&tmpRev, 0, sizeof(tmpRev));

    if (!DelInQueue(tmpRev))
        return false;
    if (tmpRev.len < 4)
        return false;

    switch (tmpRev.data[0])
    {
    case FROMMOTOR:
        motor->AnalysRevFram(tmpRev);
        break;
    case FROMDOSEI:
        dosei->AnalysRevFram(tmpRev);
        break;
    case FROMDOSEII:
        doseii->AnalysRevFram(tmpRev);
        break;
    case FROMBEAM:
        beam->AnalysRevFram(tmpRev);
        break;
    case FROMHAND:
        hand->AnalysRevFram(tmpRev);
        break;
    case FROMLIGHT:
        light->AnalysRevFram(tmpRev);
        break;
    case FROMINTERLOCK:
        interlock->AnalysRevFram(tmpRev);
        break;
    case FROMIO:
        io->AnalysRevFram(tmpRev);
        break;

    default:
        break;
    }
    return true;
}

//分析命令，分配到各个模块。
bool SlaveBase::AnalysRevFram( TransRecvT& tmpRev)
{
#ifdef PRINT_RECEIVED
    QString str;
    QString temp;

    if (tmpRev.data[1] != 'H')
    {
        if (FROMMLC==tmpRev.data[0] || FROMUPPER==tmpRev.data[0])
            str.append("Received2: ");
        else
            str.append("Received: ");

        for (int i = 0; i < tmpRev.len; i++)
        {
            temp.sprintf("%x ", tmpRev.data[i]);
            str.append(temp);
        }
        qDebug()<<str;
    }
#endif

    switch (tmpRev.data[0])
    {
    case FROMMOTOR:
        motor->AnalysRevFram(tmpRev);
        break;
    case FROMDOSEI:
        dosei->AnalysRevFram(tmpRev);
        break;
    case FROMDOSEII:
        doseii->AnalysRevFram(tmpRev);
        break;
    case FROMBEAM:
        beam->AnalysRevFram(tmpRev);
        break;
    case FROMHAND:
        hand->AnalysRevFram(tmpRev);
        break;
    case FROMLIGHT:
        light->AnalysRevFram(tmpRev);
        break;
    case FROMINTERLOCK:
        interlock->AnalysRevFram(tmpRev);
        break;
    case FROMIO:
        io->AnalysRevFram(tmpRev);
        break;
    case FROMMLC:
        mlc->AnalysRevFram(tmpRev);
        break;
    case FROMUPPER:
        upper->AnalysRevFram(tmpRev);
        break;
    default:
        break;

    }
    return true;
}

//检查值的有效性，若有效进行转换
bool SlaveBase::CheckValue(float srcvalue, EDGE_CHECK_T type, UINT32 &dstvalue)
{
    if (type > EDGEARRLEN || type < 0)
        return false;
    if (srcvalue > g_edge_arr[type].max_value ||
            srcvalue < g_edge_arr[type].min_value)
        return false;
    dstvalue = (UINT32)(g_edge_arr[type].k * srcvalue + g_edge_arr[type].b);
    return true;
}

bool SlaveBase::CheckValue(float srcvalue, EDGE_CHECK_T type)
{
    if (type > EDGEARRLEN || type < 0)
        return false;
    if (srcvalue > g_edge_arr[type].max_value ||
            srcvalue < g_edge_arr[type].min_value)
        return false;
    return true;
}

//LJY-12-16
bool SlaveBase::CheckXMLData(UINT16 srcvalue, EDGE_CHECK_T type)
{
    UINT16 max, min;
    if (type > EDGEARRLEN || type < 0)
        return false;
    max = g_edge_arr[type].k * g_edge_arr[type].max_value + g_edge_arr[type].b;
    min = g_edge_arr[type].k * g_edge_arr[type].min_value + g_edge_arr[type].b;
    if((srcvalue>=min) && (srcvalue<=max))
        return true;
    else
    {
        qDebug()<<"CheckXMLData type ="<<type<<" error";
        return false;
    }
}

//转换私有成员值到标准值（UINT -> Float）
float SlaveBase::ConvertMemberToFloat(UINT32 srcvalue, EDGE_CHECK_T type)
{
    return ((float)srcvalue - g_edge_arr[type].b) / (g_edge_arr[type].k);
}

//读取软电位器xml文件
bool SlaveBase::ReadSoftpotsFile()
{
    QFile file;
    bool res = false;
    //不知道固定模式和旋转模式参数是不是一样的
    switch(g_ray_type)
    {
    case 0x01:
    case 0x11:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_5_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_5_900.xml");
        }
        break;
    case 0x02:
    case 0x12:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_7_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_7_900.xml");
        }
        break;
    case 0x03:
    case 0x13:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_8_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_8_900.xml");
        }
        break;
    case 0x04:
    case 0x14:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_10_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_10_900.xml");
        }
        break;
    case 0x05:
    case 0x15:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_12_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_12_900.xml");
        }
        break;
    case 0x06:
    case 0x16:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_14_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_14_900.xml");
        }

        break;
    case 0x08:
    case 0x18:
        if (DoseBase::dose_rate == 50)
        {
            file.setFileName(".\\AccConfig\\xray_low_50.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\xray_low_300.xml");
        }

        break;
    case 0x09:
    case 0x19:
        if (DoseBase::dose_rate == 50)
        {
            file.setFileName(".\\AccConfig\\xray_high_50.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\xray_high_300.xml");
        }

        break;
    case 0x0A:
    case 0x1A:
        file.setFileName(".\\AccConfig\\film.xml");

        break;
    case 0x88:
        file.setFileName(".\\AccConfig\\filed_light.xml");

        break;
    }

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            //interlock
            SlaveBase::open_softpot_file = false;
            qDebug()<<"open softpot file error:  file name ="<<file.fileName();
            return false;
        }
        SlaveBase::open_softpot_file = true;
        reader.setDevice(&file);
        reader.readNext();

        qDebug()<<"open softpot file name ="<<file.fileName();

        while (!reader.atEnd())
        {
            if (reader.isStartElement())
            {
                if (reader.name() == "dosei")   //分别调用各个模块的ReadElement函数
                {
                    res = dosei->ReadElement();
                }
                else if (reader.name() == "doseii")
                {
                    res = doseii->ReadElement();
                }
                else if (reader.name() == "beam")
                {
                    res = beam->ReadElement();
                }
                else if (reader.name() == "io")
                {
                    res = io->ReadElement();
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

//写软电位器文件
bool SlaveBase::WriteSoftpotsFile()
{
    QFile file;
    bool res = false;
    //?不知道固定模式和旋转模式参数是不是一样的
    switch(g_ray_type)
    {
    case 0x01:
    case 0x11:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_5_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_5_900.xml");
        }

        break;
    case 0x02:
    case 0x12:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_7_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_7_900.xml");
        }

        break;
    case 0x03:
    case 0x13:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_8_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_8_900.xml");
        }

        break;
    case 0x04:
    case 0x14:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_10_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_10_900.xml");
        }

        break;
    case 0x05:
    case 0x15:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_12_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_12_900.xml");
        }

        break;
    case 0x06:
    case 0x16:
        if (DoseBase::dose_rate == 300)
        {
            file.setFileName(".\\AccConfig\\elc_14_300.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\elc_14_900.xml");
        }

        break;
    case 0x08:
    case 0x18:
        if (DoseBase::dose_rate == 50)
        {
            file.setFileName(".\\AccConfig\\xray_low_50.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\xray_low_300.xml");
        }

        break;
    case 0x09:
    case 0x19:
        if (DoseBase::dose_rate == 50)
        {
            file.setFileName(".\\AccConfig\\xray_high_50.xml");
        }
        else
        {
            file.setFileName(".\\AccConfig\\xray_high_300.xml");
        }

        break;
    case 0x0A:
    case 0x1A:
        file.setFileName(".\\AccConfig\\film.xml");

        break;
    case 0x88:
        file.setFileName(".\\AccConfig\\filed_light.xml");

        break;
    }

        if (!file.open(QFile::ReadOnly | QFile::Text))  //以只读方式读入到内存
        {
            //interlock
            return false;
        }
        QString errMsg = "";
        int errorLine, errorCol;
        if (!doc.setContent(&file, &errMsg, &errorLine, &errorCol))
        {
            //interlock
            qDebug() << "WriteSoftpotsFile doc setContent error! errMag:" << errMsg
                     << "errLine:" << errorLine << "errCol:" << errorCol << endl;
            file.close();
            return false;
        }
        file.close();
        root = doc.documentElement();
        node = root.firstChild();   //关闭文件
        while (!node.isNull())  //修改内存中的xml节点
        {
            if (node.nodeName() == "beam")
                beam->WriteElement();
            else if (node.nodeName() == "dosei")
                dosei->WriteElement();
            else if (node.nodeName() == "doseii")
                doseii->WriteElement();
            else if (node.nodeName() == "io")
                io->WriteElement();
            node = node.nextSibling();
        }

        file.open(QFile::WriteOnly | QFile::Truncate);  //再以只写的方式打开文件
        QTextStream ts(&file);
        ts.reset();
        ts.setCodec("utf-8");
        doc.save(ts, 4, QDomNode::EncodingFromTextStream);  //保存内存中的xml到本地
        file.close();

    return res;
}

void SlaveBase::ProcessDoseRatesIL()
{
    float ex=0.0f;

    if (!dosei->dose_rate_new || !doseii->dose_rate_new)
        return;
    //dosei和doseii的dose rate相差超过15%，安全联锁

    if ((dosei->dose1_aver_rate!=0) && (doseii->dose2_aver_rate!=0))
    {
        ex = ((float)qAbs((int)(doseii->dose2_aver_rate) - (int)(dosei->dose1_aver_rate))
             / (float)(dosei->dose1_aver_rate));
        //dosei->dose_aver_rate_count++;
        //if ((dosei->dose_aver_rate_count > 15) && (ex > 0.15))
        //qDebug()<<"ex="<<ex;
        if ((g_treat_time > 2.0f) && (ex > 0.15))
            dosei->InsertInterlockNoErr(3, SAFETY_IL, IL_RADON, LATCHED);
    }
    //Dose 1 vs . dose 2 time synch error .安全联锁
    if (qAbs((int)(dosei->treatment_time) - (int)(doseii->treatment_time)) > 120)
        dosei->InsertInterlock(79, 990, ERROR_IL, IL_RADON, LATCHED);

    dosei->dose_rate_new = false;
    doseii->dose_rate_new = false;
}

void SlaveBase::ProcessPressurenTemperatureIL()
{
    if (dosei->press_temp_new  && doseii->press_temp_new)
    {
        //dosei和doseii的温度差的绝对值大于等于1.5℃ not latched
        if ( qAbs(dosei->float_temp - doseii->float_temp) >= 1.5f)
            dosei->InsertInterlockNoErr(4, SAFETY_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT,
                            NOT_LATCHED);
        else
            DeleteInterlock(4);
        //dosei和doseii的压力差的绝对值大于等于0.47kPa(3.53mmHg) not latched
        if ( qAbs(dosei->float_press - doseii->float_press) >= 3.5f)
            dosei->InsertInterlockNoErr(5, SAFETY_IL, IL_PROGRAM|IL_READY|IL_INTERRUPT,
                            NOT_LATCHED);
        else
            DeleteInterlock(5);

        dosei->press_temp_new = doseii->press_temp_new = false;
    }
}

//旁路Interlock，设置编号为num的Interlock的旁路信息
void SlaveBase::OvrdInterlock(UINT32 num, bool value)
{
    if (num >= INTERLOCK_NUM)
        return;

    g_interlocks_mutex.lock();
    g_interlocks[num].ovrd = value;

    //qDebug()<<"Ovrd interlock:"<<num<<" value"<<value;
    g_lock_passby_bits.setBit(num, value);
    g_interlocks_mutex.unlock();
}

void SlaveBase::InsertInterlock(UINT32 num, UINT32 err, IL_TYPE_T type,
                                UINT32 state, LATCHED_T latched)
{
    ERROR_ST tmperr;

    if (num >= INTERLOCK_NUM)
        return;
    if (num != 15){
        if((state & IL_READY) == IL_READY)
            state = state | IL_NOT_READY | IL_RADON;       //ready include not_ready according to Primus Mid-Energy V7.0
        if((state & IL_INTERRUPT) == IL_INTERRUPT)
            state = state | IL_INTERRUPT_NR;
    }

    if ((g_il_state & state) == 0)
        return;

    tmperr.error_num = err;
    tmperr.type = type;
    tmperr.state = state;
    tmperr.latched = latched;

    g_interlocks_mutex.lock();
    g_lock_bits.setBit(num, true);
    g_interlocks[num].has_err = true;
    g_interlocks[num].type=type;
    g_interlocks[num].status = IL_ACTIVE;
    g_interlocks[num].history++;
    g_interlocks[num].last_error_num = err;

    g_error_history[err]++;
    //qDebug()<<g_error_history[err]<<"产生的interlock次数"<<"标号"<<err;
    //qDebug()<<"Error no:"<<err<<", count ="<<g_error_history[err];
    //g_interlocks[num].show_type = show_type;

    if (tmperr.latched == NOT_LATCHED &&
            !g_interlocks[num].error.contains(tmperr))
    {
        g_interlocks[num].error.append(tmperr);
    }
    else if (tmperr.latched == LATCHED)         //replace with LATCHED if have
    {
        g_interlocks[num].error.removeAll(tmperr);
        g_interlocks[num].error.append(tmperr);
    }


    emit this->Interlock_Inserted(type, num);

    g_maincontrol_wait.wakeOne();
    qDebug()<<"Insert Interlock"<<num<<" err"<<err;
    g_interlocks_mutex.unlock();
}

void SlaveBase::InsertInterlockNoErr(UINT32 num, IL_TYPE_T type,
                                UINT32 state, LATCHED_T latched)
{
    if (num >= INTERLOCK_NUM)
        return;


    if((state & IL_READY) == IL_READY)
        state = state | IL_NOT_READY;       //ready include not_ready according to Primus Mid-Energy V7.0
    if((state & IL_INTERRUPT) == IL_INTERRUPT)
        state = state | IL_INTERRUPT_NR;


    if ((g_il_state & state)==0)
        return;

    g_interlocks_mutex.lock();
    g_lock_bits.setBit(num, true);

    g_interlocks[num].has_err = false;
    g_interlocks[num].status = IL_ACTIVE;
    g_interlocks[num].history++;
    g_interlocks[num].type = type;
    g_interlocks[num].state = state;    //?

    if(g_interlocks[num].latched == NOT_LATCHED) //if it was NOT_LATCHED, let the new value of latched
        g_interlocks[num].latched = latched;
    //g_interlocks[num].show_type = show_type;

    emit this->Interlock_Inserted(type, num);
    g_maincontrol_wait.wakeOne();
    qDebug()<<"Insert Interlock"<<num;
    g_interlocks_mutex.unlock();
}


void SlaveBase::DeleteInterlock(UINT32 num)
{
    if (num >= INTERLOCK_NUM)
        return;

    g_interlocks_mutex.lock();

    if (g_interlocks[num].latched == LATCHED)
    {
       g_interlocks_mutex.unlock();
       return;
    }

    g_lock_bits.setBit(num, false);
    g_interlocks[num].status = IL_CLAER;
    g_interlocks[num].error.clear();
    //accui->SetInterlockPara(num, IL_CLAER, 0, g_interlocks[num].history);
    qDebug()<<"emit interlock deleted"<<num;
    emit this->Interlock_Deleted( g_interlocks[num].type, num);
    g_interlocks_mutex.unlock();
}

void SlaveBase::DeleteInterlock(UINT32 num, UINT32 err)
{
    ERROR_ST tmperr;
    if (num >= INTERLOCK_NUM)
        return;
    qDebug()<<"del interlock "<<num<<" "<<err;
    tmperr.error_num = err;
    g_interlocks_mutex.lock();

    for (int i = 0; i < g_interlocks[num].error.size(); i++)
    {
        if (g_interlocks[num].error.value(i) == tmperr
                && g_interlocks[num].error.value(i).latched == LATCHED)
        {
            g_interlocks_mutex.unlock();
            return;
        }
    }
    g_interlocks[num].error.removeAll(tmperr);
    if (g_interlocks[num].error.isEmpty())
    {
        g_interlocks[num].status = IL_CLAER;
        emit this->Interlock_Deleted( g_interlocks[num].type, num);
        g_lock_bits.setBit(num, false);
    }
    g_interlocks_mutex.unlock();

}

bool SlaveBase::SendNotFinished()
{
    if(outqueue_used.available()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool SlaveBase::ReadInitFile()
{
    QFile file(".\\AccConfig\\init.xml");

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
                if (reader.name() == "init")
                {
                    res = ReadInitElement();
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

bool SlaveBase::ReadInitElement()
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
                //qDebug()<<reader.name()<<", "<<(reader.name() == "svc_password");
                if (reader.name() == "ray_type")
                    g_ray_type = reader.readElementText().toInt();
                else if(reader.name() == "compen_ovrd")
                    g_compensate = (bool)reader.readElementText().toInt();
                else if(reader.name() == "check1_bypass")
                    g_check1_bypass = (bool)reader.readElementText().toInt();
                else if(reader.name() == "check2_bypass")
                    g_check2_bypass = (bool)reader.readElementText().toInt();
                reader.readNext();
            }
        }
    }
    return true;
}

bool SlaveBase::WriteInitFile()
{
    QFile file(".\\AccConfig\\init.xml");

//    QFileInfo fileinfo(file);
//    qDebug()<<"write file to"<<fileinfo.absoluteFilePath();

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
            if (root.tagName() == "init")
            {
                WriteInitElement();
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

void SlaveBase::WriteInitElement()
{
    node = root.firstChild();
    while (!node.isNull())
    {
        if (node.nodeName() == "ray_type")
            node.firstChild().setNodeValue(QString::number(g_ray_type));
        else if (node.nodeName() == "compen_ovrd")
            node.firstChild().setNodeValue(QString::number((int)g_compensate));
        else if (node.nodeName() == "check1_bypass")
            node.firstChild().setNodeValue(QString::number((int)g_check1_bypass));
        else if (node.nodeName() == "check2_bypass")
            node.firstChild().setNodeValue(QString::number((int)g_check2_bypass));

        node = node.nextSibling();
    }
}

void SlaveBase::InsertGantryTimeoutIL()
{
    motor->InsertInterlock(79, 980, ERROR_IL, IL_RADON, LATCHED);
}

void SlaveBase::InsertDoseiAccTimeoutIL()
{
    //Dose 1 deadman timeout
    dosei->InsertInterlock(79, 981, ERROR_IL, IL_RADON, LATCHED);
}

void SlaveBase::InsertDoseiiAccTimeoutIL()
{
    // Dose 2 deadman timeout .
    doseii->InsertInterlock(79, 982, ERROR_IL, IL_RADON, LATCHED);
}

void SlaveBase::InsertCommTimeoutIL()
{
    qDebug()<<"Beam controller deadman timeout";
    io->InsertInterlock(79, 983, ERROR_IL, IL_RADON, LATCHED);
}

void SlaveBase::LightOnTimerStop()
{
    if (g_lighton_timer->isActive())
        g_lighton_timer->stop();
}

void SlaveBase::LightOnTimerStart()
{
    if (!g_lighton_timer->isActive())
        g_lighton_timer->start(15000);       //15000
    else
    {
        g_lighton_timer->stop();
        g_lighton_timer->start(1000);
    }
}

void SlaveBase::LightOnTimeOut()
{
    light->InsertInterlock(110, 895, SYSTEM_IL, IL_ALL, LATCHED);
    g_lighton_timer->stop();
}


void SlaveBase::LightReportTimerStop()
{
    if (g_lightrep_timer->isActive())
        g_lightrep_timer->stop();
}

void SlaveBase::LightReportTimerStart()
{
    if (!g_lightrep_timer->isActive())
        g_lightrep_timer->start(15000);
    else
    {
        g_lightrep_timer->stop();
        g_lightrep_timer->start(15000);
    }
}

void SlaveBase::LightReportTimeOut()
{
    light->InsertInterlock(110, 896, SYSTEM_IL, IL_ALL, LATCHED);
    g_lightrep_timer->stop();
}

void SlaveBase::SipDogReStart()
{
    if (!g_sipdog_timer->isActive())
        g_sipdog_timer->start(500);
    else
    {
        g_sipdog_timer->stop();
        g_sipdog_timer->start(500);
    }
}

void SlaveBase::SipDogStop()
{
    if(g_sipdog_timer->isActive())
        g_sipdog_timer->stop();
}

void SlaveBase::SipDogTestOkAndCallSip()
{

//    if(!isSipNormalWork)
//    {
//        InsertInterlock(62, ERROR_IL, IL_ALL, LATCHED);
//        can->CanTransmitSipBeat();
//    }
//    else
//    {
//        isSipNormalWork=false;
//        can->CanTransmitSipBeat();
    //    }
}

void SlaveBase::WarmTimerStart()            // VICTOR
{
    if (!g_warm_timer->isActive())
        g_warm_timer->start(14*60*1000);
    else
    {
        g_warm_timer->stop();
        g_warm_timer->start(14*60*1000);
    }
}

void SlaveBase::WarmTimerStop()
{
    if(g_warm_timer->isActive())
        g_warm_timer->stop();
}

void SlaveBase::WarmUpTimeOut()
{
    g_warmup_timeout=true;
}

void SlaveBase::UpconTimerStart()
{
    if(!g_upcon_timer->isActive())
        g_upcon_timer->start(CONNECT_TIMEOUT);
    else
    {
        g_upcon_timer->stop();
        g_upcon_timer->start(CONNECT_TIMEOUT);
    }
}

void SlaveBase::UpconTimerStop()
{
    if(g_upcon_timer->isActive())
        g_upcon_timer->stop();
}

void SlaveBase::UpconTimeOut()
{
    if (g_is_remote_mode)
    {
        upper->InsertInterlock(126, 109, ERROR_IL, IL_PROGRAM, NOT_LATCHED);
    }

    emit base->g_upcon_stop_signal();
}

void SlaveBase::MlcconTimerStart()
{
    if(!g_mlccon_timer->isActive())
        g_mlccon_timer->start(CONNECT_TIMEOUT);
    else
    {
        g_mlccon_timer->stop();
        g_mlccon_timer->start(CONNECT_TIMEOUT);
    }
}

void SlaveBase::MlcconTimerStop()
{
    if(g_mlccon_timer->isActive())
        g_mlccon_timer->stop();
}

void SlaveBase::MlcconTimeOut()
{
    if (g_is_remote_mode)
    {
        mlc->InsertInterlock(81, 108, ERROR_IL, IL_PROGRAM, NOT_LATCHED);
    }

    emit base->g_mlccon_stop_signal();
}

void SlaveBase::HeartbeatSenderStart()
{
    if (!g_heartbeat_sender->isActive())
        g_heartbeat_sender->start(HEARTBEAT_SEND_CYCLE);
    else
    {
        g_heartbeat_sender->stop();
        g_heartbeat_sender->start(HEARTBEAT_SEND_CYCLE);
    }
}

void SlaveBase::HeartbeatSenderStop()
{
    if(g_heartbeat_sender->isActive())
        g_heartbeat_sender->stop();
}

void SlaveBase::HeartbeatSenderTimeout()
{
    if (g_is_remote_mode)
    {
        remote->SendHeartBeat();
    }
}

void SlaveBase::HeartbeatReceiverStart()
{
    if (!g_heartbeat_receiver->isActive())
        g_heartbeat_receiver->start(HEARTBEAT_RECV_CYCLE);
    else
    {
        g_heartbeat_receiver->stop();
        g_heartbeat_receiver->start(HEARTBEAT_RECV_CYCLE);
    }
}

void SlaveBase::HeartbeatReceiverStop()
{
    if(g_heartbeat_receiver->isActive())
        g_heartbeat_receiver->stop();
}

void SlaveBase::HeartbeatReceiverTimeout()
{
    if (g_is_remote_mode)
    {
        bool exist = false;

        if (remote->mIsUpperWork)
        {
            if (upper->mIsHeartbeatGet)
            {
                upper->mIsHeartbeatGet = false;
                exist = true;
            }
            else
            {
                qDebug() << "upper timeout" << endl;
                remote->mIsUpperWork = false;
                upper->InsertInterlockNoErr(111, ERROR_IL, IL_ALL, LATCHED);
            }
        }

        if (remote->mIsMlcWork)
        {
            if (mlc->mIsHeartbeatGet)
            {
                mlc->mIsHeartbeatGet = false;
                exist = true;
            }
            else
            {
                qDebug() << "mlc timeout" << endl;
                remote->mIsMlcWork = false;
                mlc->InsertInterlockNoErr(109, ERROR_IL, IL_ALL, LATCHED);
            }
        }

        if (!exist)
        {
            remote->StopHeartBeat();
        }
    }
}

void SlaveBase::MlcMovestopTimerStart()
{
    if (!g_mlc_movestop_timer->isActive())
    {
        qDebug() << "MlcMovestopTimerStart" << endl;
        g_mlc_movestop_timer->start(MOVESTOP_WAIT);
    }

    //remote->isMlcMoveStop = false;
}

void SlaveBase::MlcMovestopTimerStop()
{
    if (g_mlc_movestop_timer->isActive())
    {
        qDebug() << "MlcMovestopTimerStop" << endl;
        g_mlc_movestop_timer->stop();
    }
}

void SlaveBase::MlcMovestopTimeOut()
{
    if (g_is_remote_mode && remote->mIsMlcWork)
    {
        qDebug() << "MlcMovestopTimeOut" << endl;
        mlc->InsertInterlock(81, 101, ERROR_IL, IL_ALL, LATCHED);
    }
}
