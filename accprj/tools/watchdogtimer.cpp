#include "watchdogtimer.h"

extern bool g_heart_beat_flag;

WatchdogTimer::WatchdogTimer(UINT16 sendCycle, UINT16 recvCycle)
{
    mSendCycle = sendCycle;
    mRecvCycle = recvCycle;

    mSender.setSingleShot(false);
    mReceiver.setSingleShot(false);
    connect(&mSender, SIGNAL(timeout()), this, SLOT(senderTimeout()));
    connect(&mReceiver, SIGNAL(timeout()), this, SLOT(receiverTimeout()));
}

void WatchdogTimer::start()
{
    if (mSender.isActive())
        mSender.stop();

    if (mReceiver.isActive())
        mReceiver.stop();

    mSender.start(mSendCycle);
    mReceiver.start(mRecvCycle);
}

void WatchdogTimer::stop()
{
    if (mSender.isActive())
        mSender.stop();

    if (mReceiver.isActive())
        mReceiver.stop();
}

void WatchdogTimer::senderTimeout()
{
    //send heart beat
}

void WatchdogTimer::receiverTimeout()
{
    if (g_heart_beat_flag)
        g_heart_beat_flag = false;
    else
    {
        //time out
    }
}
