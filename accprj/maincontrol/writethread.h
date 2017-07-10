#ifndef WRITETHREAD_H
#define WRITETHREAD_H

#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QSemaphore>

class WriteThread : public QThread
{
    Q_OBJECT
public:
    explicit WriteThread(QObject *parent = 0);
    void addTask(int type);
    void kill();

protected:
    void run();

private:
    bool mRunning;
    QQueue<int> mQueue;
    QSemaphore mSem;
    QMutex mMtx;
};

#endif // WRITETHREAD_H
