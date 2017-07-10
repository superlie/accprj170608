#ifndef FTPUPLOADER_H
#define FTPUPLOADER_H

#include <QNetworkAccessManager>
#include <QFile>
#include <basetsd.h>
#include <QUrl>
#include <QNetworkReply>
#include <QIODevice>

typedef enum
{
    RUT_ERR      = 0x00,
    RUT_NO_COMP  = 0x01,
    RUT_SUCESS   = 0x02,
}RES_UPLOAD_T;

class FtpUploader : public QObject
{
    Q_OBJECT
public:
    FtpUploader();

public:
    bool UploadByUrl(QString url, QString username, QString password, int port, QString uploadfilename);

public slots:
    void uploadFinished(QNetworkReply *reply);

private:
    void MySleep(UINT32 mesc);

private:
    RES_UPLOAD_T mUploadComp;
    QNetworkAccessManager mNetAccessMgr;
};

#endif // FTPUPLOADER_H
