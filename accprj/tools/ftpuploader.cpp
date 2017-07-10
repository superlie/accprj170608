#include "ftpuploader.h"
#include <QCoreApplication>

FtpUploader::FtpUploader()
{
    mUploadComp = RUT_NO_COMP;
    connect(&mNetAccessMgr, SIGNAL(finished(QNetworkReply*)), this, SLOT(uploadFinished(QNetworkReply*)));
}

bool FtpUploader::UploadByUrl(QString url, QString username, QString password, int port, QString uploadfilename)
{
    QFile uploadfile(uploadfilename);
    if( !uploadfile.open(QIODevice::ReadOnly) )
    {
        qDebug()<< "open uploadfile error!" << endl;
        return false;
    }

    QByteArray buf = uploadfile.readAll();

    QUrl qurl(url);
    qurl.setPort(port);
    //qurl.setUserName(username);
    //qurl.setPassword(password);
    QNetworkRequest request(qurl);
    QNetworkReply* reply = mNetAccessMgr.put(request, buf);
    reply->ignoreSslErrors();

    while (RUT_NO_COMP == mUploadComp)
    {
        MySleep(1000);
    }

    if (mUploadComp == RUT_ERR)
    {
        mUploadComp = RUT_NO_COMP;
        return false;
    }
    else
    {
        mUploadComp = RUT_NO_COMP;
        return true;
    }

    return true;
}

void FtpUploader::uploadFinished(QNetworkReply *reply)
{
    if (reply->error() != 0)
    {
        mUploadComp = RUT_ERR;
    }
    else
    {
        mUploadComp = RUT_SUCESS;
    }

    reply->deleteLater();
}

void FtpUploader::MySleep(UINT32 mesc)
{
    QTime reachTime = QTime::currentTime().addMSecs(mesc);

    while (QTime::currentTime() < reachTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
