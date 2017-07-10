#include "receiverthread.h"

bool ReceiverThread::CanRev(TransRecvT &data)
{
    return SlaveBase::AnalysRevFram(data);
}

void ReceiverThread::run()
{
    TransRecvT tmp;
    while (1)
    {
        inqueue_used.acquire();
        in_queue_mutex.lock();
        tmp=inQueue[inFrontP];
      //  qDebug() << "rev queue" << inFrontP << inTailP << endl;
        inFrontP = (inFrontP + 1) % INQUEUELEN;
        in_queue_mutex.unlock();

        SlaveBase::AnalysRevFram(tmp);


//        msleep(10);
    }
}
