#ifndef RECEIVERTHREAD_H
#define RECEIVERTHREAD_H

#include <QThread>
#include "slave_public.h"
#include "acc_type.h"

#include "can/can.h"

class ReceiverThread : public SlaveBase, public QThread
{
public:

    bool CanRev(TransRecvT &);

protected:
    void run();
//    receiverthread();
};

extern Can *can;
#endif // RECEIVERTHREAD_H
