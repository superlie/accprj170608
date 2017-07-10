#include "Can2.h"
#include <windows.h>
#include <windef.h>
#include <winuser.h>
#include <QDebug>
#include <stdlib.h>
#include <string.h>
#include <QTimer>

extern "C"
{
#if(ACC==ACC_PC)
#include "ECanVci.h"
#elif(ACC==ACC_LAPTOP)
#include "ControlCAN.h"
#endif
}

//ctim0和ctim1用来设置can波特率
extern const char* ctim0[10];

extern const char* ctim1[10];

static int charToInt(unsigned char ch)
{
    if (ch >= '0' && ch <= '9')
        return (ch - '0');
    else if (ch >= 'a' && ch <= 'f')
        return (ch - 'a' + 10);
    else if (ch >= 'A' && ch <= 'F')
        return (ch - 'A' + 10);
    else
        return -1;
}

//两位十六进制字符串转换成十进制数
static int strHexToDec(const char *str)
{
    return (int)(charToInt(str[0]) * 16 + charToInt(str[1]));
}

static void delay(int count)
{
    while (--count > 0);
}


Can2::~Can2()
{
    VCI_ClearBuffer(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX);
    VCI_CloseDevice(CAN2_TYPE, CAN2_DEV_INDEX);
}

/*!
 * \brief Can::CanInit can初始化及打开设备
 * \return
 */
bool Can2::Can2Init()
{
    VCI_INIT_CONFIG initConfig;
    memset((void *)&initConfig, 0, sizeof (VCI_INIT_CONFIG));

    initConfig.AccCode = 0;
    initConfig.Filter = 0;
    initConfig.Mode = (UCHAR)NORMAL_MODE;
    initConfig.AccMask = 0xFFFFFFFF;
    initConfig.Timing0 = (UCHAR)strHexToDec(ctim0[0]);//设置1000k波特率
    initConfig.Timing1 = (UCHAR)strHexToDec(ctim1[0]);

    if (VCI_InitCAN(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX, &initConfig) == STATUS_ERR)
    {
        VCI_CloseDevice(CAN2_TYPE, CAN2_DEV_INDEX);
        qDebug() << "init can2 error" << endl;
        return false;
    }

    if (VCI_StartCAN(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX) == STATUS_ERR)
    {
        VCI_CloseDevice(CAN2_TYPE, CAN2_DEV_INDEX);
        qDebug() << "start can2 error" << endl;
        return false;
    }

    VCI_ClearBuffer(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX);
    return true;
}

static bool VCI_CanTransmit(PVCI_CAN_OBJ pSend)
{
    ULONG ret;
    // g_can_transmit_mutex.lock();
    ret = VCI_Transmit(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX, pSend, 1);
    // g_can_transmit_mutex.unlock();
    return (ret == 1);
}



/*!
 * \brief Can2::TransmitCan2Frame can2发送一个数据帧
 * \param buf
 * \param len
 * \param can_id
 * \return bool
 */
bool Can2::TransmitCan2Frame(const unsigned char *buf, int len, int can_id)
{
    ULONG ret = 0;
    VCI_CAN_OBJ vco;
    memset((void *)&vco, 0, sizeof(VCI_CAN_OBJ));
    vco.ID = (UINT)can_id;                  //can标示符
    vco.DataLen = (BYTE)len;                //can数据帧长度
    vco.SendType = CAN2_NORMAL_SEND;        //正常发送
    vco.RemoteFlag = CAN2_NOT_REMOTE;       //数据帧
    vco.ExternFlag = CAN2_NOT_EXTERN;        //标准帧
    memcpy(&vco.Data, buf, len * sizeof (unsigned char));

    ret = VCI_CanTransmit(&vco);
    //delay(1000);

    if (ret != 1)
        qDebug() << "can2send ret" << ret << endl;
    return (ret == 1);
}


/*!
 * \brief Can2::Can2TransmitCmd 发送一条命令
 * \param data
 * \return true/fasle
 */
bool Can2::Can2TransmitCmd(TransRecvT *data)
{
    bool ret = false;

    switch (data->data[0])
    {
    case TOUPPER_HDR:
        ret = TransmitCan2Frame((unsigned char*)data->data, data->len, TOUPPER_ADDR);
        break;
    case TOMLC_HDR:
        ret = TransmitCan2Frame((unsigned char*)data->data, data->len, TOMLC_ADDR);
        break;
    default:
        break;
    }

    return ret;
}


/*!
 * \brief Can::CanTransmitStatus 发送只带标示符的can数据帧
 * \param canId
 * \return true/fasle
 */
bool Can2::Can2TransmitStatus(int canId)
{
    ULONG ret;
    VCI_CAN_OBJ vco;
    memset((void *)&vco, 0, sizeof(VCI_CAN_OBJ));
    vco.ID = (UINT)canId;

    ret = VCI_CanTransmit(&vco);
    delay(1000);
    return (ret == 1);
}

/*!
 * \brief RecvThread2::run can接收线程
 */
void RecvThread2::run()
{
    int recvLen = 1;                //接收到的can数据帧个数
    VCI_CAN_OBJ frameinfo[50];      //can接收缓冲区
    VCI_ERR_INFO errinfo;           //错误信息
    TransRecvT cmdRecv;             //接收到的命令
    int i,j;

    for (;;)
    {
        //msleep(5);
        recvLen = VCI_Receive(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX, frameinfo, 50, 0);
        if (recvLen <= 0)
        {
            //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码
            //千万不能省略这一步（即使你可能不想知道错误码是什么）
            VCI_ReadErrInfo(CAN2_TYPE, CAN2_DEV_INDEX, CAN2_INDEX, &errinfo);
            msleep(10);
        }
        else
        {

            for (i = 0; i < recvLen; i ++)
            {
                cmdRecv.len = frameinfo[i].DataLen;

                if (cmdRecv.len == 0)
                    continue;

                for (j = 0; j < frameinfo[i].DataLen; j++)
                    cmdRecv.data[j] = frameinfo[i].Data[j];

                SlaveBase::InsertInQueue(cmdRecv);
            }
        }
    }
}
