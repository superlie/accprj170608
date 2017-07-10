#include "Can.h"
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
#include "ECanVci.h".H"
#elif(ACC==ACC_LAPTOP)
#include "ControlCAN.h"
#endif
}

bool  isSipNormalWork = false;

//ctim0和ctim1用来设置can波特率
const char* ctim0[10]=
{
    "00",		//1000Kbps
    "00",		// 800Kbps
    "00",		// 500Kbps
    "01",		// 250Kbps
    "03",		// 125Kbps
    "04",		// 100Kbps
    "09",		// 50Kbps
    "18",		// 20Kbps
    "31",		// 10Kbps
    "BF"		// 5Kbps
};

const char* ctim1[10]=
{
    "14",		//1000Kbps
    "16",		// 800Kbps
    "1C",		// 500Kbps
    "1C",		// 250Kbps
    "1C",		// 125Kbps
    "1C",		// 100Kbps
    "1C",		// 50Kbps
    "1C",		// 20Kbps
    "1C",		// 10Kbps
    "FF"		// 5Kbps
};

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


Can::~Can()
{
    VCI_ClearBuffer(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX);
    VCI_CloseDevice(CAN_TYPE, CAN_DEV_INDEX);
}

/*!
 * \brief Can::CanInit can初始化及打开设备
 * \return
 */
bool Can::CanInit()
{
    VCI_INIT_CONFIG initConfig;
    memset((void *)&initConfig, 0, sizeof (VCI_INIT_CONFIG));
    initConfig.AccCode = 0;
    initConfig.Filter = 0;
    initConfig.Mode = (UCHAR)NORMAL_MODE;
    initConfig.AccMask = 0xFFFFFFFF;
    initConfig.Timing0 = (UCHAR)strHexToDec(ctim0[0]);//设置1000k波特率
    initConfig.Timing1 = (UCHAR)strHexToDec(ctim1[0]);

    if (VCI_OpenDevice(CAN_TYPE, 0, 0) == STATUS_ERR)
    {
        qDebug() << "open can error" << endl;
        return false;
    }

    if (VCI_InitCAN(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX, &initConfig) == STATUS_ERR)
    {
        VCI_CloseDevice(CAN_TYPE, CAN_DEV_INDEX);
        qDebug() << "init can error" << endl;
        return false;
    }

    if (VCI_StartCAN(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX) == STATUS_ERR)
    {
        VCI_CloseDevice(CAN_TYPE, CAN_DEV_INDEX);
        qDebug() << "start can error" << endl;
        return false;
    }

    VCI_ClearBuffer(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX);
    return true;
}

static bool VCI_CanTransmit(PVCI_CAN_OBJ pSend)
{
    ULONG ret;
    // g_can_transmit_mutex.lock();
    ret = VCI_Transmit(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX, pSend, 1);
    // g_can_transmit_mutex.unlock();
    return (ret == 1);
}



/*!
 * \brief Can::TransmitCanFrame can发送一个数据帧
 * \param buf
 * \param len
 * \param can_id
 * \return bool
 */
bool Can::TransmitCanFrame(const unsigned char *buf, int len, int can_id)
{

    ULONG ret = 0;
    VCI_CAN_OBJ vco;
    memset((void *)&vco, 0, sizeof(VCI_CAN_OBJ));
    vco.ID = (UINT)can_id;                  //can标示符
    vco.DataLen = (BYTE)len;                //can数据帧长度
    vco.SendType = CAN_NORMAL_SEND;        //正常发送
    vco.RemoteFlag = CAN_NOT_REMOTE;       //数据帧
    vco.ExternFlag = CAN_NOT_EXTERN;        //标准帧
    memcpy(&vco.Data, buf, len * sizeof (unsigned char));

    ret = VCI_CanTransmit(&vco);
    delay(1000);

    return (ret == 1);
}


/*!
 * \brief Can::CanTransmitCmd 发送一条命令
 * \param data
 * \return true/fasle
 */
bool Can::CanTransmitCmd(TransRecvT *data)
{
    int remain = 0;
    bool ret = false;

    if (data->len < 8)
    {
        ret = TransmitCanFrame((unsigned char*)data, data->len + 1, HOST_SIP_CMD_TYPE);     //长度为data->len + 1为了传输TransRecvT结构体的len变量
    }
    else if (data->len >= 8 && data->len < 16)
    {
        remain = data->len - 7; //第二次发送的数据长度
        ret = TransmitCanFrame((unsigned char*)data, 8, HOST_SIP_CMD_TYPE);
        ret &= TransmitCanFrame(&(data->data[7]), remain, HOST_SIP_CMD_TYPE | HOST_SIP_CAN_NUM);
    }
    return ret;
}


/*!
 * \brief Can::CanTransmitStatus 发送只带标示符的can数据帧
 * \param canId
 * \return true/fasle
 */
bool Can::CanTransmitStatus(int canId)
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
 * \brief Can::CanTransmitStartSyn 开机同步sip
 * \return true/false 发送成功/发送失败
 */
bool Can::CanTransmitStartSyn()
{
    return CanTransmitStatus(0x0000);
}

/*!
 * \brief Can::CanTransmitSipReset 复位sip
 * \return true/false 发送成功/发送失败
 */
bool Can::CanTransmitSipReset()
{
    return CanTransmitStatus(0x0001);
}

/*!
 * \brief Can::CanTransmitSipBeat 发送心跳给sip
 * \return true/false 发送成功/发送失败
 */
bool Can::CanTransmitSipBeat()
{
    return CanTransmitStatus(0x0002);
}


/*!
 * \brief Can::CanTransmitSlaveReset 复位slave
 * \return true/false 发送成功/发送失败
 */
bool Can::CanTransmitSlaveReset()
{
    return CanTransmitStatus(0x0004);
}

/*!
 * \brief Can::CanReset can控制器复位
 * \return true/false
 */
bool Can::CanReset()
{
    int ret = 0;
    ret = VCI_ResetCAN(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX);
    return (ret == 1);
}

/*!
 * \brief Can::CanTransmitHostStart 主机开机握手
 * \return true/false
 */
bool Can::CanTransmitHostStart()
{
    int id = 0x0002;
    return CanTransmitStatus(id);
}


/*!
 * \brief RecvThread::run can接收线程
 */
void RecvThread::run()
{
    int recvLen = 1;                //接收到的can数据帧个数
    int cmdLen = 0;                 //命令的长度
    bool hasRecvFirst = false;     //是否接收到第一个can数据帧
    bool recvCmdComplete = false;  //完整命令是否接收完成
    VCI_CAN_OBJ frameinfo[50];      //can接收缓冲区
    VCI_ERR_INFO errinfo;           //错误信息
    TransRecvT cmdRecv;             //接收到的命令
    int i,j;

    for (;;)
    {
        //msleep(5);
        recvLen = VCI_Receive(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX, frameinfo, 50, 0);
        if (recvLen <= 0)
        {
            //注意：如果没有读到数据则必须调用此函数来读取出当前的错误码
            //千万不能省略这一步（即使你可能不想知道错误码是什么）
            VCI_ReadErrInfo(CAN_TYPE, CAN_DEV_INDEX, CAN_INDEX, &errinfo);
            //qDebug() << "no data" << endl;
            //qDebug() << errinfo.ErrCode << errinfo.Passive_ErrData << errinfo.ArLost_ErrData;
            msleep(10);
        }
        else
        {

            for (i = 0; i < recvLen; i ++)
            {
                if ((frameinfo[i].ID & ACC_CAN_CMD) == 0)   //9,10bit = 0
                {
                    //自定义命令
                    if ((frameinfo[i].ID & SIP_HOST_CMD_TYPE) == 0) //7bit = 0
                    {
                        UINT16 specialCmd = frameinfo[i].ID & 0x00f8;   //3-7bit cmdtype
                        UINT16 slaveId = frameinfo[i].ID & 0x0007;      //0-2bit slaveid
                        switch (specialCmd)
                        {
                        /*sip回应的心跳信号*/
                        case 0x0010:
                            isSipNormalWork = true;
                            break;

                            /*log off interlock*/
                        case 0x0038:
                            qDebug()<<"CAN logoff id="<<slaveId;
#ifndef DEBUG_MODE
                            beam->InsertInterlock(71 + slaveId, 900 + slaveId, ERROR_IL);
#endif
                            break;

                        default:
                            break;
                        }
                    }
                    //非自定义命令
                    else {  //7bit = 1

                        //接收命令第一个can数据帧
                        if ((frameinfo[i].ID & SIP_HOST_CAN_NUM) == 0)      //bit8 = 0  命令的第一部分
                        {
                            hasRecvFirst = true;
                            //slave发回来的缓冲区,0号高四位存放slaveid号,低四位存放命令长度
                            cmdRecv.len = (frameinfo[i].Data[0] & 0x0F) + 1;
                            cmdLen = cmdRecv.len;
                            frameinfo[i].Data[0] = ((frameinfo[i].Data[0] & 0xF0) >> 4);

                            for (j = 0; j < frameinfo[i].DataLen; j++)
                                cmdRecv.data[j] = frameinfo[i].Data[j];
                            //若数据包中的命令长度小于等于8，则表示接收结束
                            if (cmdLen <= 8)
                                recvCmdComplete = true;
                        }
                        //接收到第二个can数据帧,进行判断
                        else if (hasRecvFirst && ((frameinfo[i].ID & SIP_HOST_CAN_NUM) == SIP_HOST_CAN_NUM))    //bit8 = 1      命令的第二部分
                        {
                            for (j = 0; j < frameinfo[i].DataLen; j++)
                                cmdRecv.data[8+j] = frameinfo[i].Data[j];
                            if (cmdLen > 8 && cmdLen < 16)
                                recvCmdComplete = true;
                        }

                        //接收一条完整命令,插入至接收队列中
                        if (recvCmdComplete)
                        {
                            recvCmdComplete = false;
                            hasRecvFirst = false;
                            SlaveBase::InsertInQueue(cmdRecv);
                        }
                    }
                }
            }
        }
    }
}
