#include "aesutils.h"

AesUtils::AesUtils(UCHAR *key)
{
    this->key = key;
    m_lpAes=NULL;
    //InitializePrivateKey(16,(unsigned char*)"\x79\x76\x68\x6B\x77\x66\x6E\x68\x72\x65\x73\x63\x6C\x6B\x70\x6E");
    InitializePrivateKey(16, key);
}

AesUtils::~AesUtils()
{
    if (m_lpAes!=NULL)
    {
        delete m_lpAes;
    }
}

//返回16进制密文
QString AesUtils::OnEncrypt(QString str_cleartext)
{
    QByteArray ba = str_cleartext.toLatin1();
    char *chararr_cleartext = ba.data();
    DWORD size = strlen(chararr_cleartext);
    char chararr_ciphertext[1024];
    memset(chararr_ciphertext, '\0', 1024);

    size = Encrypt((LPVOID)chararr_cleartext, size, (LPVOID)chararr_ciphertext); //进行加密

    int i = 0;
    int tmpc = 0;
    QString str_ciphertext = "";
    while (i < size)
    {
        tmpc = (UCHAR)chararr_ciphertext[i];
        if (tmpc == 0)
            str_ciphertext += "00";
        else if (tmpc < 16)
            str_ciphertext += QString().sprintf("0%x", tmpc);
        else
            str_ciphertext += QString().sprintf("%x", tmpc);
        i++;
    }

    return str_ciphertext;
}

QString AesUtils::OnUncrypt(QString str_ciphertext)
{
    char chararr_ciphertext[1024];
    char chararr_cleartext[1024];
    memset(chararr_ciphertext, '\0', 1024);
    memset(chararr_cleartext, '\0', 1024);

    DWORD size = 0;
    for (int i=0; i<str_ciphertext.size(); i+=2)
    {
        QString tmp = str_ciphertext.at(i);
        tmp += str_ciphertext.at(i+1);
        chararr_ciphertext[size++] = tmp.toInt(false, 16);
    }

    size = Uncrypt((LPVOID)chararr_ciphertext, size,(LPVOID)chararr_cleartext); //进行解密

    QString str_cleartext(chararr_cleartext);

    return str_cleartext;
}

VOID AesUtils::InitializePrivateKey(DWORD KeySize,UCHAR *KeyBytes)
{
    if (m_lpAes)
    {
        delete m_lpAes;
        m_lpAes=NULL;
    }
    m_lpAes=new Aes(KeySize,KeyBytes);
}

DWORD AesUtils::Encrypt(LPVOID InBuffer,DWORD InLength,LPVOID OutBuffer)
{
    DWORD OutLength=0;
    if (m_lpAes==NULL||OutBuffer==NULL)
    {
        return 0;
    }

    UCHAR *lpCurInBuff=(UCHAR *)InBuffer;
    UCHAR *lpCurOutBuff=(UCHAR *)OutBuffer;
    long blocknum=InLength/16;
    long leftnum=InLength%16;
    for(long i=0;i<blocknum;i++)
    {
        m_lpAes->Cipher(lpCurInBuff,lpCurOutBuff);
        lpCurInBuff+=16;
        lpCurOutBuff+=16;
        OutLength+=16;
    }
    if(leftnum)                   //多余出leftnum 字节  则加密时 多出16-leftnum 个字节
    {
        UCHAR inbuff[16];
        memset(inbuff,0,16);
        memcpy(inbuff,lpCurInBuff,leftnum);
        m_lpAes->Cipher(inbuff,lpCurOutBuff);
        lpCurOutBuff+=16;
        OutLength+=16;
    }
                                //新增16个字节，用以确定增加的字节数
    UCHAR extrabuff[16];
    memset(extrabuff,0,16);
    *((LPDWORD)extrabuff)=16+(16-leftnum)%16;      //多出16+(16-leftnum)%16个字节
    m_lpAes->Cipher(extrabuff,lpCurOutBuff);
    OutLength+=16;

    return OutLength;
}

DWORD AesUtils::Uncrypt(LPVOID InBuffer,DWORD InLength,LPVOID OutBuffer)
{
    DWORD OutLength=0;
    if (m_lpAes==NULL||OutBuffer==NULL)
    {
        return 0;
    }
    UCHAR *lpCurInBuff=(UCHAR *)InBuffer;
    UCHAR *lpCurOutBuff=(UCHAR *)OutBuffer;
    long blocknum=InLength/16;
    long leftnum=InLength%16;
    if(leftnum)
    {
                return 0;
    }
    for(long i=0;i<blocknum;i++)
    {
        m_lpAes->InvCipher(lpCurInBuff,lpCurOutBuff);
        lpCurInBuff+=16;
        lpCurOutBuff+=16;
        OutLength+=16;
    }

    UCHAR *lpExtraInBuff=lpCurOutBuff-16;
    DWORD  dwExtraBytes=*((LPDWORD)lpExtraInBuff);

    return (OutLength-dwExtraBytes);
}
