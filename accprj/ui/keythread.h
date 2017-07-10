#ifndef KEYTHREAD_H
#define KEYTHREAD_H

#include <QThread>
#include "win_qextserialport.h"

class KeyThread : public QThread
{
    Q_OBJECT
public:
    explicit KeyThread(QObject *parent = 0);
    ~KeyThread();

    void run();
    Win_QextSerialPort *key_com;

    bool isKeyDiagsUI;

signals:
    void serialFinished(QByteArray temp);
private:
    void configSerial();
};

#endif // KEYTHREAD_H
