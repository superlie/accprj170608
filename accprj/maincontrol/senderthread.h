#ifndef SENDERTHREAD_H
#define SENDERTHREAD_H

#include <QThread>
#include "slave_public.h"
#include "acc_type.h"

#include "can/Can.h"

const int SENDSLEEPTIME = 80;   //10

class SenderThread : public SlaveBase, public QThread
{
public:
//    SenderThread();
//    ~SenderThread();

    bool CanSend(TransRecvT &data);

protected:
    void run();
};
extern Can *can;

#endif // SENDERTHREAD_H
