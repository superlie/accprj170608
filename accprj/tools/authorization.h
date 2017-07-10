#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QString>

class Authorization
{
public:
    Authorization();

public:
    void writeLastUseTime(QString curtime);
    bool check();

private:
    bool getCiphertext();
    qint64 string2qint64(QString str);
    QString qint642string(qint64 val);
    QString getMachineId();

private:
    QString machine_ciptxt;
    QString regtime_ciptxt;
    QString cirtime_ciptxt;
    QString lastuse_ciptxt;
};

#endif // AUTHORIZATION_H
