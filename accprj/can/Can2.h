#ifndef CAN2_H
#define CAN2_H

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
#define CAN2_DEV_INDEX   0        //can设备索引号
#define CAN2_INDEX           1       //第1路CAN
#define NORMAL_MODE         0       //CAN正常模式
#define CAN2_NORMAL_SEND    0       //CAN正常发送
#define CAN2_NOT_REMOTE     0       //是否是远程帧
#define CAN2_NOT_EXTERN     0        //是否是扩展帧

#if(ACC==ACC_PC)
#define CAN2_TYPE VCI_PCI9810     //can设备类型,PCI-CAN
#elif(ACC==ACC_LAPTOP)
#define CAN2_TYPE VCI_USBCAN2     //can设备类型,USB-CAN
#endif

/*加速器发给上位机数据包的id位*/
#define TOUPPER_ADDR 0x0220

/*加速器发给MLC数据包的id位*/
#define TOMLC_ADDR 0x0420

/*加速器发给上位机数据包的HDR*/
#define TOUPPER_HDR 0x1F

/*加速器发给MLC数据包的HDR*/
#define TOMLC_HDR 0x18

//struct TransRecvT {
//  unsigned char len;
//  unsigned char data[20];
//};

class RecvThread2 : public QThread
{
public:
    RecvThread2()
    {
    }
    ~RecvThread2(){}

protected:
    void run();

signals:
    void recv();//
};


class Can2
{
public:
    Can2(){

    }
    ~Can2();
    bool Can2Init();                                 //初始化can设备
    bool Can2TransmitCmd(TransRecvT *data);    //发送一条命令给slave
    RecvThread2 recvThread2;                          //接收数据线程

private:
    //bool VCI_CanTransmit(PVCI_CAN_OBJ pSend);
    bool TransmitCan2Frame(const unsigned char*buf , int len, int can_id);//发送一个can数据帧
    bool Can2TransmitStatus(int canId);              //can发送只有标示符没有数据的数据帧

};
#endif
