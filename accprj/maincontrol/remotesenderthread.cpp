#include "remotesenderthread.h"
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

bool RemoteSenderThread::Can2Send(TransRecvT & data)
{
    return can2->Can2TransmitCmd(&data);
}

void RemoteSenderThread::run()
{
    bool res;
    QString str;
    QString temp;
    TransRecvT tmpOut;

    while(1)
    {
        routqueue_used.acquire();
        rout_queue_mutex.lock();

        tmpOut=routQueue[routFrontP];
        routFrontP = (routFrontP + 1) % OUTQUEUELEN;

        rout_queue_mutex.unlock();

#ifdef PRINT_SENDING
        if (tmpOut.data[1] != 'H')
        {
            str.append("Sending2: ");
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

        res = Can2Send(tmpOut);
    }
}

