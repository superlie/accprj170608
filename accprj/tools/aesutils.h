#ifndef AESUTILS_H
#define AESUTILS_H

#include "aes.h"
#include <QString>

class AesUtils
{
public:
    AesUtils(UCHAR *key);
    ~AesUtils();

    QString OnEncrypt(QString str_cleartext);
    QString OnUncrypt(QString str_ciphertext);

private:
    void InitializePrivateKey(DWORD KeySize,UCHAR *KeyBytes);                       //AES 密钥初始化
    DWORD Encrypt(LPVOID InBuffer,DWORD InLength,LPVOID OutBuffer);			//AES 加密数据
    DWORD Uncrypt(LPVOID InBuffer,DWORD InLength,LPVOID OutBuffer);			//AES 解密数据

private:
    Aes  *m_lpAes;
    UCHAR *key;
};

#endif // AESUTILS_H


