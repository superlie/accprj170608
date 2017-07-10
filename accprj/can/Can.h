#ifndef CAN_H
#define CAN_H

#include <QThread>
#include "maincontrol/slave_base.h"
#include "maincontrol/beam.h"
#include "maincontrol/motor.h"
#include "maincontrol/dosei.h"
#include "maincontrol/doseii.h"
#include "maincontrol/light.h"

extern Beam *beam;
extern Motor *motor;
extern SlaveBase *base;
extern DoseOne *dosei;
extern DoseTwo *doseii;

#define ACC_PC 1
#define ACC_LAPTOP 0
#define ACC ACC_LAPTOP      ///

#define VCI_PCI9810		2
#define VCI_USBCAN1		3
#define VCI_USBCAN2		4
#define VCI_USBCAN_E_U 	20
#define CAN_DEV_INDEX   0        //can设备索引号
#define CAN_INDEX           0       //第0路CAN
#define NORMAL_MODE         0       //CAN正常模式
#define CAN_NORMAL_SEND    0       //CAN正常发送
#define CAN_NOT_REMOTE     0       //是否是远程帧
#define CAN_NOT_EXTERN     0        //是否是扩展帧

#if(ACC==ACC_PC)
#define CAN_TYPE VCI_PCI9810     //can设备类型,PCI-CAN
#elif(ACC==ACC_LAPTOP)
#define CAN_TYPE VCI_USBCAN2     //can设备类型,USB-CAN
#endif

/*主机发给sip数据包的status位*/
#define HOST_SIP_STATUS 0x0040

/*主机发给sip数据包的cannum位*/
#define HOST_SIP_CAN_NUM 0x0020

/*主机发给sip数据包的cmdtype位*/
#define HOST_SIP_CMD_TYPE 0x0010

/*主机发给sip数据包的cb3数据位*/
#define HOST_SIP_CB3 0x0008

/*主机发给sip数据包的cb2数据位*/
#define HOST_SIP_CB2 0x0004

/*主机发给sip数据包的cb1位*/
#define HOST_SIP_CB1 0x0002

/*主机发给sip数据包的cb0位*/
#define HOST_SIP_CB0 0x0001

/*sip发给主机数据包的CANNUM位*/
#define SIP_HOST_CAN_NUM 0x0100

/*sip发给主机数据包的CMDTYPE位*/
#define SIP_HOST_CMD_TYPE 0x0080

/*sip发给主机数据包的CB3位*/
#define SIP_HOST_CB3 0x00040

/*sip发给主机数据包的CB2位*/
#define SIP_HOST_CB2 0x0020

/*sip发给主机数据包的CB1位*/
#define SIP_HOST_CB1 0x0010

/*sip发给主机数据包的CB0位*/
#define SIP_HOST_CB0 0x0008

/*sip发给主机数据包的SLAVE2位*/
#define SIP_HOST_SLAVE2 0x0004

/*sip发给主机数据包的SLAVE1位*/
#define SIP_HOST_SLAVE1 0x0002

/*sip发给主机数据包的SLAVE0位*/
#define SIP_HOST_SLAVE0 0x0001

#define ACC_CAN_CMD 0x0600

//struct TransRecvT {
//  unsigned char len;
//  unsigned char data[20];
//};

class RecvThread : public QThread
{
public:
    RecvThread()
    {
    }
    ~RecvThread(){}

protected:
    void run();

signals:
    void recv();//
};


class Can
{
public:
    Can(){

    }
    ~Can();
    bool CanInit();                                 //初始化can设备
    bool CanTransmitCmd(TransRecvT *data);    //发送一条命令给slave
    bool CanTransmitStartSyn();                     //开机同步
    bool CanTransmitSlaveReset();                   //复位slave
    bool CanTransmitSipReset();                     //复位sip
    bool CanTransmitSipBeat();                      //发心跳给sip
    bool CanReset();                                //复位主机can设备
    RecvThread recvThread;                          //接收数据线程
    bool CanTransmitHostStart();

private:
    //bool VCI_CanTransmit(PVCI_CAN_OBJ pSend);
    bool TransmitCanFrame(const unsigned char*buf , int len, int can_id);//发送一个can数据帧
    bool CanTransmitStatus(int canId);              //can发送只有标示符没有数据的数据帧

};
extern bool isSipNormalWork;
extern QMutex g_can_transmit_mutex;
#endif
