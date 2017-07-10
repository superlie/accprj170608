#include "writethread.h"
#include "interlock.h"

#define SLEEP_TIME 100

extern Interlock *interlock;

WriteThread::WriteThread(QObject *parent) :
    QThread(parent)
{
    mRunning = true;
}

void WriteThread::addTask(int type)
{
    mMtx.lock();

    if (mQueue.isEmpty())
    {
        mQueue.enqueue(type);
        mSem.release();
    }
    else
    {
        if (type != mQueue.last())
        {
            mQueue.enqueue(type);
            mSem.release();
        }
    }

    mMtx.unlock();
}

void WriteThread::kill()
{
    mRunning = false;
}

void WriteThread::run()
{
    int type;

    while (mRunning)
    {
        mSem.acquire();

        if (mQueue.isEmpty())
        {
            msleep(SLEEP_TIME);
            continue;
        }

        mMtx.lock();
        type = mQueue.dequeue();
        mMtx.unlock();

        if (type == 1)
        {
            interlock->WriteOwnFile();
            interlock->WriteErrDisplayFile();
        }
    }
}
