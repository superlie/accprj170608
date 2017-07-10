#include "senderthread.h"
#include "mlc.h"
#include "uppermonitor.h"
#include "remotecontroller.h"
#include "defines.h"

using namespace::std;
//SenderThread::SenderThread()
//{
//}

#define PRINT_SENDING
extern RemoteController *remote;
extern Mlc *mlc;
extern UpperMonitor *upper;
extern bool g_is_remote_mode;

bool SenderThread::CanSend(TransRecvT & data)
{
#ifdef DEBUG_MODE
    return true;
#else
    return can->CanTransmitCmd(&data);
#endif
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

        tmpOut=outQueue[outFrontP];
        outFrontP = (outFrontP + 1) % OUTQUEUELEN;

        out_queue_mutex.unlock();

#ifdef PRINT_SENDING
        if (tmpOut.data[1] != 'H')
        {
            str.append("Sending: ");
            for (int i = 0; i < tmpOut.len; i++)
            {
                temp.sprintf("%x ",tmpOut.data[i]);
                str.append(temp);
            }
            qDebug()<<str;
            str.clear();
        }
#endif

       // outqueue_free.release();

        res = CanSend(tmpOut);

        msleep(SENDSLEEPTIME);
    }
}

