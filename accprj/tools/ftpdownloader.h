#ifndef FTPDOWNLOADER_H
#define FTPDOWNLOADER_H

#include <QNetworkAccessManager>
#include <QFile>
#include <basetsd.h>
#include <QUrl>
#include <QNetworkReply>
#include <QIODevice>

typedef enum
{
    RDT_ERR = -1,
    RDT_NO_COMP = 0,
    RDT_SUCESS = 1,
}RES_DOWNLOAD_T;

class FtpDownloader : public QObject
{
    Q_OBJECT
public:
    FtpDownloader();

public:
    QString DownloadByUrl(QString url, QString username, QString password, int port);
    bool UploadByUrl(QString url, QString username, QString password, int port, QString uploadfilename);

public slots:
    void downloadFinished(QNetworkReply *reply);

private:
    void MySleep(UINT32 mesc);
    QString saveFileName();
    bool saveToDisk(const QString &filename, QIODevice *data);

private:
    QString mFilename;
    RES_DOWNLOAD_T mDownloadComp;
    QNetworkAccessManager mNetAccessMgr;
};

#endif // FTPDOWNLOADER_H
