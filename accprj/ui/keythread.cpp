#include "keythread.h"
#include "../maincontrol/remotecontroller.h"
#include <QKeyEvent>
#include <QDebug>
#include "defines.h"

KeyThread::KeyThread(QObject *parent) :
    QThread(parent)
{
    configSerial();
    isKeyDiagsUI = false;
}

//! [0]
KeyThread::~KeyThread()
{
    delete key_com;
}
void KeyThread::run()
{
    QByteArray temp = "";

    while(1)
    {
        msleep(25);    //delay 200ms
        temp.clear();

        temp = key_com->read(1);
        if(temp.isEmpty())
            continue;
        emit this->serialFinished(temp);
    }
}

void KeyThread::configSerial()
{
#ifdef DEBUG_MODE
    QString portName = "COM3";
#else
    QString portName = "COM4";
#endif

    key_com = new Win_QextSerialPort(portName,QextSerialBase::EventDriven);

    key_com->open(QIODevice::ReadWrite);

    key_com->setBaudRate(BAUD2400);

    key_com->setDataBits(DATA_8);

    key_com->setParity(PAR_NONE);

    //    key_com->setParity(PAR_ODD);

    //    key_com->setParity(PAR_EVEN);

    key_com->setStopBits(STOP_1);
    //key_com->setStopBits(STOP_2);

    key_com->setFlowControl(FLOW_OFF); //设置数据流控制，我们使用无数据流控制的默认设置
    //key_com->setTimeout(500); //设置延时
}

