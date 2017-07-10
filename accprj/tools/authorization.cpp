#include "authorization.h"
#include "aesutils.h"
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <qDebug>
#include <QMessageBox>
#include <QDateTime>

UCHAR aes_key[50] = "fAew@234%^_34`1";

AesUtils *aesutils = new AesUtils(aes_key);

Authorization::Authorization()
{
    machine_ciptxt = "";
    regtime_ciptxt = "";
    cirtime_ciptxt = "";
    lastuse_ciptxt = "";
}

bool Authorization::getCiphertext()
{
    QFile file("./config.ini");
    if (!file.exists())
    {
        QMessageBox::information(NULL, "Auth", "config file no exists", QMessageBox::Yes);
        return false;
    }

    QSettings ciptxt_settings("config.ini", QSettings::IniFormat);
    machine_ciptxt = ciptxt_settings.value("config/a").toString();
    regtime_ciptxt = ciptxt_settings.value("config/b").toString();
    cirtime_ciptxt = ciptxt_settings.value("config/c").toString();
    lastuse_ciptxt = ciptxt_settings.value("config/d").toString();

    return true;
}

qint64 Authorization::string2qint64(QString str)
{
    qint64 val = 0;
    qint64 count = 1;
    for (int i=str.size()-1; i>=0; i--)
    {
        if (str.at(i).isDigit())
        {
            qint64 temp = str.at(i).digitValue()*count;
            val += temp;
            count *= 10;
        }
        else
            return 0;
    }

    return val;
}

QString Authorization::qint642string(qint64 val)
{
    QString str = "";
    while (val > 0)
    {
        int temp = val%10;

        str.insert(0, QString::number(temp));

        val /= 10;
    }

    return str;
}

QString Authorization::getMachineId()
{
    return "werwfwaf234";
}

void Authorization::writeLastUseTime(QString curtime)
{
    //write config.txt
    QSettings config_settings("./config.ini", QSettings::IniFormat);
    config_settings.beginGroup("config");
    config_settings.setValue("d", aesutils->OnEncrypt(curtime));
    config_settings.endGroup();
}

bool Authorization::check()
{
    if (!getCiphertext())
        return false;

    //machine id check
    QString machine_id = getMachineId();
    QString machine_clrtxt = aesutils->OnUncrypt(machine_ciptxt);
    if (machine_clrtxt.toLower() != machine_id.toLower())
    {
        //QMessageBox::information(NULL, "Auth", "machine id check not pass", QMessageBox::Yes);
        return false;
    }

    //regtime check
    QString regtime_clrtxt = aesutils->OnUncrypt(regtime_ciptxt);
    if (regtime_clrtxt.size()/2 > sizeof(qint64))
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }
    qint64 regtime = string2qint64(regtime_clrtxt);
    if (regtime == 0)
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }

    //cirtime check
    QString cirtime_clrtxt = aesutils->OnUncrypt(cirtime_ciptxt);
    if (cirtime_clrtxt.size()/2 > sizeof(qint64))
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }
    qint64 cirtime = string2qint64(cirtime_clrtxt);
    if (cirtime == 0)
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }

    //lastuse check
    QString lastuse_clrtxt = aesutils->OnUncrypt(lastuse_ciptxt);
    if (lastuse_clrtxt.size()/2 > sizeof(qint64))
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }
    qint64 lasttime = string2qint64(lastuse_clrtxt);
    if (lasttime == 0)
    {
        //QMessageBox::information(NULL, "Auth", "time check not pass", QMessageBox::Yes);
        return false;
    }

    //last use time check
    QFileInfo fi("./config.ini");
    qint64 ini_lastmod = fi.lastModified().currentMSecsSinceEpoch();
    qint64 curtime = QDateTime::currentDateTime().currentMSecsSinceEpoch();

    lasttime = lasttime>ini_lastmod?lasttime:ini_lastmod;

    if (lasttime > curtime)
        curtime = lasttime+86400000;

    if (curtime > regtime+cirtime)
    {
        //QMessageBox::information(NULL, "Auth", "auth has expire", QMessageBox::Yes);
        writeLastUseTime(qint642string(curtime));
        return false;
    }

    writeLastUseTime(qint642string(curtime));
    return true;
}
