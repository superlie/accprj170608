#include "ui/accui.h"
#include <QApplication>
#include <QCoreApplication>
#include <QMetaType>
#include "maincontrol/mastercontroller.h"
#include "maincontrol/receiverthread.h"
#include "maincontrol/senderthread.h"
#include "main.h"
#include "can/Can.h"
#include "can/can2.h"
#include "maincontrol/senderthread.h"
#include "maincontrol/remotecontroller.h"
#include "maincontrol/writethread.h"
#include "maincontrol/remotesenderthread.h"
#include <QThread>
#include <QReadWriteLock>
#include <QDateTime>
#include <QTextStream>
#include <QFile>
#include <QMessageLogContext>
#include <QMessageBox>
#include "windows.h"
#include "tools/authorization.h"


//全局对象的定义
//Ftp ftp;
bool g_auth_check = false;

QFile *logfile;
SlaveBase *base;
Motor *motor;
DoseOne *dosei;
DoseTwo *doseii;
Beam *beam;
HandCon *hand;
Light *light;
Interlock *interlock;
InputOutput *io;
MasterController *master;
RemoteController *remote;
Can *can;
Can2 *can2;
Mlc *mlc;
UpperMonitor *upper;
SenderThread *sender;
RemoteSenderThread *remotesender;
ReceiverThread *receiver;
AccUi *accui;
HandleTimer *handletimer;
WriteThread *writethread;

QTimer *g_sipdog_timer;
QTimer *g_commdog_timer;//射线过程中看门狗
QTimer *g_gantry_timer;
QTimer *g_dosei_timer;
QTimer *g_doseii_acc_timer;
QTimer *g_warm_timer;
QTimer *g_lighton_timer;
QTimer *g_lightrep_timer;
QTimer *g_upcon_timer;
QTimer *g_mlccon_timer;
QTimer *g_heartbeat_sender;
QTimer *g_heartbeat_receiver;
QTimer *g_mlc_movestop_timer;

//QTimer *g_light_timer;

//发送接受队列的信号量
QSemaphore SlaveBase::outqueue_free(OUTQUEUELEN);
QSemaphore SlaveBase::inqueue_free(INQUEUELEN);
QSemaphore SlaveBase::outqueue_used(0);
QSemaphore SlaveBase::inqueue_used(0);

//remote发送队列的信号量
QSemaphore SlaveBase::routqueue_free(OUTQUEUELEN);
QSemaphore SlaveBase::routqueue_used(0);

//发送数据队列
QMutex SlaveBase::out_queue_mutex(QMutex::NonRecursive);
UINT8 SlaveBase::outTailP = 0;
UINT8 SlaveBase::outFrontP = 0;
TransRecvT SlaveBase::outQueue[OUTQUEUELEN];

//remote发送数据队列
QMutex SlaveBase::rout_queue_mutex(QMutex::NonRecursive);
UINT8 SlaveBase::routTailP = 0;
UINT8 SlaveBase::routFrontP = 0;
TransRecvT SlaveBase::routQueue[OUTQUEUELEN];

//接受数据队列
UINT8 SlaveBase::inTailP = 0;
UINT8 SlaveBase::inFrontP = 0;
QMutex SlaveBase::in_queue_mutex(QMutex::NonRecursive);
TransRecvT SlaveBase::inQueue[INQUEUELEN];

//射线类型
UINT8 g_ray_type;

//bool g_download_finish;

//文件读写
QXmlStreamReader reader;
QDomDocument doc;
QDomElement root;
QDomNode node;

QString g_svc_password;   //LJY-12-15
QString g_ftp_ip;
QString g_ftp_name;
QString g_ftp_password;
QString g_max_dose_rate;     //2016-8-18
QString g_rad_on_delay_time; //2016-8-25
QString g_machine_name;


//c0标志位
QBitArray g_c0_echo_bits;
QReadWriteLock g_c0_echo_bits_lock;
//check#1&2的标志位
QBitArray g_check_bits;
QMutex g_check_bits_mutex;
//check的阶段
CHECKREQ_TYPE_T g_check_index;
QMutex g_check_index_mutex;

//Check 1\2 Bypass 标志位
bool g_check1_bypass = false;
bool g_check2_bypass = false;


//Compensation or Override
bool g_compensate = true;
float g_treat_time=0.0f;

//ACCESSORY_B_TYPE accessory_3_type = EANULL;
bool g_acc3_change = false;

//旋转治疗标志
bool g_is_rotate_treatment = false;

//interlocks结构
QBitArray g_lock_bits;
QBitArray g_lock_passby_bits;       //旁路标识
INTERLOCK_ST g_interlocks[INTERLOCK_NUM];
UINT32 g_error_history[ERROR_NUM];
QMutex g_interlocks_mutex;

const QBitArray g_zero_bits(INTERLOCK_NUM, false);
QBitArray g_system_il_bits(INTERLOCK_NUM, false);
QBitArray g_safety_il_bits(INTERLOCK_NUM, false);
QBitArray g_latched_il_bits(INTERLOCK_NUM, false);

//DoseBase 的静态变量
float DoseBase::ref_temp;
float DoseBase::ref_press;
float DoseBase::dose_rate;
float DoseBase::swi[3];     //未赋值
float DoseBase::fwi[3];
float DoseBase::compen_factor = 0.0;
float DoseBase::x_lo_cf = 0.0;
float DoseBase::x_hi_cf = 0.0;

bool g_io_acknowledge = false;
int g_check_bypass = 0;
//射线时间
QTime g_radon_time;


HandleTimer::HandleTimer(QObject *parent):QObject(parent)
{
    g_lighton_timer = new QTimer();
    g_lightrep_timer = new QTimer();
    g_warm_timer = new QTimer();
    g_sipdog_timer = new QTimer();
    g_commdog_timer = new QTimer();
    g_gantry_timer = new QTimer();
    g_dosei_timer = new QTimer();
    g_doseii_acc_timer = new QTimer();
    g_upcon_timer = new QTimer();
    g_mlccon_timer = new QTimer();
    g_heartbeat_sender = new QTimer();
    g_heartbeat_receiver = new QTimer();
    g_mlc_movestop_timer = new QTimer();

    g_lighton_timer->setSingleShot(true);
    g_lightrep_timer->setSingleShot(true);
    g_warm_timer->setSingleShot(true);
    g_commdog_timer->setSingleShot(true);
    g_gantry_timer->setSingleShot(true);
    g_dosei_timer->setSingleShot(true);
    g_doseii_acc_timer->setSingleShot(true);
    g_upcon_timer->setSingleShot(true);
    g_mlccon_timer->setSingleShot(true);
    g_heartbeat_sender->setSingleShot(false);
    g_heartbeat_receiver->setSingleShot(false);
    g_mlc_movestop_timer->setSingleShot(true);

    //Light on
    QObject::connect(base, SIGNAL(g_lighton_stop_signal()), base,
                     SLOT(LightOnTimerStop()));
    QObject::connect(base,SIGNAL(g_lighton_start_signal()), base,
                     SLOT(LightOnTimerStart()));
    QObject::connect(g_lighton_timer, SIGNAL(timeout()), base,
                     SLOT(LightOnTimeOut()));       //Interlock 110 895

    //Light Report
    QObject::connect(base, SIGNAL(g_lightrep_stop_signal()), base,
                     SLOT(LightReportTimerStop()));
    QObject::connect(base,SIGNAL(g_lightrep_start_signal()), base,
                     SLOT(LightReportTimerStart()));
    QObject::connect(g_lightrep_timer, SIGNAL(timeout()), base,
                     SLOT(LightReportTimeOut()));    //Interlock 110 896

    //SIP dog
    QObject::connect(base, SIGNAL(g_sipdog_stop_signal()), base,
                     SLOT(SipDogStop()));
    QObject::connect(base,SIGNAL(g_sipdog_start_signal()), base,
                     SLOT(SipDogReStart()));
    QObject::connect(g_sipdog_timer, SIGNAL(timeout()), beam,
                     SLOT(SipDogTestOkAndCallSip()));
    //watchdog
    QObject::connect(base, SIGNAL(g_commdog_stop_signal()), g_commdog_timer,
                     SLOT(stop()));
    QObject::connect(base,SIGNAL(g_commdog_start_signal()),this,
                     SLOT(g_commdog_timer_start()));
    QObject::connect(g_commdog_timer, SIGNAL(timeout()), beam,
                     SLOT(InsertCommTimeoutIL()));

    //gantry
    QObject::connect(base,SIGNAL(g_gantry_start_signal()),this,
                     SLOT(g_gantry_timer_start()));
    QObject::connect(g_gantry_timer, SIGNAL(timeout()), base,
                     SLOT(InsertGantryTimeoutIL()));

    //dosei
    QObject::connect(base,SIGNAL(g_dosei_start_signal(int)),this,
                     SLOT(g_dosei_timer_start(int)));
    QObject::connect(base, SIGNAL(g_dosei_stop_signal()), g_dosei_timer,
                     SLOT(stop()));
    QObject::connect(g_dosei_timer, SIGNAL(timeout()), dosei,
                     SLOT(InsertDoseiAccTimeoutIL()));

    //doseii
    QObject::connect(base,SIGNAL(g_doseii_start_signal()),this,
                     SLOT(g_doseii_timer_start()));
    QObject::connect(base, SIGNAL(g_doseii_stop_signal()), g_doseii_acc_timer,
                     SLOT(stop()));
    QObject::connect(g_doseii_acc_timer, SIGNAL(timeout()), doseii,
                     SLOT(InsertDoseiiAccTimeoutIL()));

    // warmup for interlock(14)
    QObject::connect(base, SIGNAL(g_warmup_stop_signal()), base,
                     SLOT(WarmTimerStop()));
    QObject::connect(base,SIGNAL(g_warmup_start_signal()), base,
                     SLOT(WarmTimerStart()));
    QObject::connect(g_warm_timer, SIGNAL(timeout()), interlock,
                     SLOT(WarmUpTimeOut()));

    //upper connect
    QObject::connect(base, SIGNAL(g_upcon_start_signal()), base ,
                     SLOT(UpconTimerStart()));
    QObject::connect(base, SIGNAL(g_upcon_stop_signal()), base ,
                     SLOT(UpconTimerStop()));
    QObject::connect(g_upcon_timer, SIGNAL(timeout()), base ,
                     SLOT(UpconTimeOut()));

    //mlc connect
    QObject::connect(base, SIGNAL(g_mlccon_start_signal()), base ,
                     SLOT(MlcconTimerStart()));
    QObject::connect(base, SIGNAL(g_mlccon_stop_signal()), base ,
                     SLOT(MlcconTimerStop()));
    QObject::connect(g_mlccon_timer, SIGNAL(timeout()), base ,
                     SLOT(MlcconTimeOut()));

    //remoote send timer
    QObject::connect(base, SIGNAL(g_heartbeat_sender_start()), base,
                     SLOT(HeartbeatSenderStart()));
    QObject::connect(base, SIGNAL(g_heartbeat_sender_stop()), base,
                     SLOT(HeartbeatSenderStop()));
    QObject::connect(g_heartbeat_sender, SIGNAL(timeout()), base,
                     SLOT(HeartbeatSenderTimeout()));

    //remoote receive timer
    QObject::connect(base, SIGNAL(g_heartbeat_receiver_start()), base,
                     SLOT(HeartbeatReceiverStart()));
    QObject::connect(base, SIGNAL(g_heartbeat_receiver_stop()), base,
                     SLOT(HeartbeatReceiverStop()));
    QObject::connect(g_heartbeat_receiver, SIGNAL(timeout()), base,
                     SLOT(HeartbeatReceiverTimeout()));

    //mlc movestop
    QObject::connect(base, SIGNAL(g_mlc_movestop_start_signal()), base,
                     SLOT(MlcMovestopTimerStart()));
    QObject::connect(base, SIGNAL(g_mlc_movestop_stop_signal()), base,
                     SLOT(MlcMovestopTimerStop()));
    QObject::connect(g_mlc_movestop_timer, SIGNAL(timeout()), base,
                     SLOT(MlcMovestopTimeOut()));
}
//--------------duanzhibin // 9/15

void HandleTimer::g_doseii_timer_start()
{
    g_doseii_acc_timer->start(7200);

}
void HandleTimer::g_dosei_timer_start(int interval)
{
    g_dosei_timer->start(7200+interval);
    qDebug() << "g_dosei_timer_start:" << 7200+interval << endl;
}
void HandleTimer::g_gantry_timer_start()
{
    g_gantry_timer->start(2400);
}
void HandleTimer::g_commdog_timer_start()
{
    g_commdog_timer->start(3600);
}

//第一个初始化函数
bool Init()
{
    bool res;
    g_check_bits.resize(SLAVE_NUM);
    g_check_bits.fill(false);
    g_lock_bits.resize(INTERLOCK_NUM);        //set bits false
    g_lock_bits.fill(false);
    g_lock_passby_bits.resize(INTERLOCK_NUM);   //旁路，可能要存文件
    g_lock_passby_bits.fill(false);
    g_c0_echo_bits.resize(SLAVE_NUM);       //8  slaves

    g_c0_echo_bits.fill(false);

    g_safety_il_bits.fill(true);
    g_safety_il_bits.setBit(45, false);     //45号不是安全连锁
    g_safety_il_bits.setBit(124, false);     //124号不是安全连锁

    res = ReadConfigFile();
    res &= base->ReadInitFile();
    SlaveBase::BaseInit();
    return res;
}

void outputMessage(QtMsgType type, const QMessageLogContext &context,const QString &msg)
{
    static QMutex logmutex;
    logmutex.lock();
     //  QString text;
   // QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    QString message = QString("%1").arg(msg);
    logfile->open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream text_stream(logfile);
    text_stream << message <<"\r\n";
    logfile->flush();
    logfile->close();
    logmutex.unlock();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

#ifdef AUTHORIZATION
    Authorization auth;
    if (!auth.check())
        g_auth_check = false;
    else
        g_auth_check = true;
#endif

    qRegisterMetaType<ACCESSORY_B_TYPE>("ACCESSORY_B_TYPE");
    qRegisterMetaType<SVC_LINEEDIT>("SVC_LINEEDIT");
    //   bool res;
    QString str;

#ifndef DEBUG_MODE
    /*QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss");
    str.append(".\\LogData\\").append(current_date_time).append(".txt");
    logfile = new QFile(str);
    qInstallMessageHandler(outputMessage);*/
#endif

    Init(); //初始化

    base = new SlaveBase();
    motor = new Motor();
    dosei = new DoseOne();
    doseii = new DoseTwo();
    beam = new Beam();
    hand = new HandCon();
    light = new Light();
    mlc = new Mlc();
    upper = new UpperMonitor();
    remote = new RemoteController();
    io = new InputOutput();
    interlock = new Interlock();
    sender = new SenderThread();
    remotesender = new RemoteSenderThread();
    receiver = new ReceiverThread();
    can = new Can();
    can2 = new Can2();
    accui = new AccUi();
    handletimer = new HandleTimer();
    writethread = new WriteThread();

    if (!can->CanInit())
    {
        //interlock
        qDebug()<<"Can init error"<<endl;
        // return a.exec();
    }

    if (!can2->Can2Init())
    {
        //interlock
        qDebug()<<"Can2 init error"<<endl;
        // return a.exec();
    }

    //QThread::NormalPriority   3
    //QThread::HighestPriority  5
    can->recvThread.start(QThread::HighestPriority);
    can2->recvThread2.start(QThread::HighestPriority);

    receiver->start();
    sender->start();
    remotesender->start();
    writethread->start();

    master = new MasterController();
    master->start();

#ifdef DEBUG_MODE
    accui->resize(800,800);
    accui->show();
#else
    accui->showFullScreen();
#endif

#ifdef SPKEYON
    accui->key_thread.start();
#endif

    return a.exec();
}

bool ReadConfigFile()
{
    QFile file(".\\AccConfig\\Config.xml");

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
                if (reader.name() == "config")
                {
                    res = ReadConfigElement();
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
        qDebug() << exception << endl;
        return false;
    }
    return res;
}


bool ReadConfigElement()
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

                if(reader.name() == "svc_password")
                    g_svc_password = reader.readElementText();
                else if(reader.name() == "ftp_ip")
                    g_ftp_ip = reader.readElementText();
                else if(reader.name() == "ftp_name")
                    g_ftp_name = reader.readElementText();
                else if(reader.name() == "ftp_password")
                    g_ftp_password = reader.readElementText();
                else if(reader.name() == "dose_rate")
                    g_max_dose_rate = reader.readElementText();
                else if(reader.name() == "rad_on_delay_time")
                    g_rad_on_delay_time = reader.readElementText();
                else if(reader.name() == "machine_name")
                {
                    g_machine_name = reader.readElementText();
                    qDebug() << "machine_name" << g_machine_name << endl;
                }

                reader.readNext();
            }

        }
    }
    return true;
}
