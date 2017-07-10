#include "ftpdownloader.h"
#include <QCoreApplication>

FtpDownloader::FtpDownloader()
{
    mFilename = "F:\\treatdata.xml";
    mDownloadComp = RDT_NO_COMP;
    connect(&mNetAccessMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(downloadFinished(QNetworkReply*)));
}

QString FtpDownloader::DownloadByUrl(QString url, QString username, QString password, int port)
{
    QUrl qurl(url);
    qurl.setPort(port);
    //qurl.setUserName(username);
    //qurl.setPassword(password);
    QNetworkRequest request(qurl);
    QNetworkReply* reply = mNetAccessMgr.get(request);
    reply->ignoreSslErrors();

    while (RDT_NO_COMP == mDownloadComp)
    {
        MySleep(1000);
    }

    if (mDownloadComp == RDT_ERR)
    {
        mDownloadComp = RDT_NO_COMP;
        return "";
    }
    else
    {
        mDownloadComp = RDT_NO_COMP;
        return saveFileName();
    }
}

void FtpDownloader::downloadFinished(QNetworkReply *reply)
{
    if (reply->error() != 0)
    {
        mDownloadComp = RDT_ERR;
    }
    else
    {
        QString filename = saveFileName();
        if (saveToDisk(filename, reply))
        {
            mDownloadComp = RDT_SUCESS;
        }
        else
        {
            mDownloadComp = RDT_ERR;
        }
    }

    reply->deleteLater();
}

QString FtpDownloader::saveFileName()
{
    return mFilename;
}

bool FtpDownloader::saveToDisk(const QString &filename, QIODevice *data)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        return false;
    }

    file.write(data->readAll());
    file.close();

    return true;
}

void FtpDownloader::MySleep(UINT32 mesc)
{
    QTime reachTime = QTime::currentTime().addMSecs(mesc);

    while (QTime::currentTime() < reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
