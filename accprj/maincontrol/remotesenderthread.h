#ifndef REMOTESENDERTHREAD_H
#define REMOTESENDERTHREAD_H

#include <QThread>
#include "slave_public.h"
#include "acc_type.h"

#include "can/Can2.h"

class RemoteSenderThread : public SlaveBase, public QThread
{
public:
//    SenderThread();
//    ~SenderThread();

    bool Can2Send(TransRecvT &data);

protected:
    void run();
};
extern Can2 *can2;

#endif // REMOTESENDERTHREAD_H
