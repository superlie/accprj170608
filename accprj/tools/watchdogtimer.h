#ifndef WATCHDOGTIMER_H
#define WATCHDOGTIMER_H

#include <QTimer>
#include <basetsd.h>

class WatchdogTimer
{
    Q_OBJECT
public:
    WatchdogTimer(UINT16 sendCycle, UINT16 recvCycle);
    void start();
    void stop();

private slots:
    void senderTimeout();
    void receiverTimeout();

public:
    QTimer mSender;
    QTimer mReceiver;
    UINT16 mSendCycle;
    UINT16 mRecvCycle;
};

#endif // WATCHDOGTIMER_H
