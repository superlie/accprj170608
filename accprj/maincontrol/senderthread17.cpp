#include "senderthread.h"
using namespace::std;
//SenderThread::SenderThread()
//{
//}

bool SenderThread::CanSend(TransRecvT & data)
{
    return can->CanTransmitCmd(&data);
}

void SenderThread::run()
{
    bool res;
    QString str;
    QString temp;
    TransRecvT tmpOut;
    while(1)
    {

        outqueue_used.acquire();
        out_queue_mutex.lock();

        str.append("Sending: ");
        for (int i = 0; i < outQueue[outFrontP].len; i++)
        {
            temp.sprintf("%x ",outQueue[outFrontP].data[i]);
            str.append(temp);
        }
        qDebug()<<str;
        str.clear();

        tmpOut=outQueue[outFrontP];
        outFrontP = (outFrontP + 1) % OUTQUEUELEN;

        out_queue_mutex.unlock();

       // outqueue_free.release();

        res = CanSend(tmpOut);

        msleep(SENDSLEEPTIME);
    }
}

