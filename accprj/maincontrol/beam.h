#ifndef BEAM_H
#define BEAM_H

#include "slave_base.h"

//const int TOBEAM=0x13;

//typedef enum{
//    RESEARCH = 0,
//    DEFAULT = 1,
//    TREATMENT = 2,
//}  THREE_STATE_T;



class Beam :public SlaveBase
{
public:
        Beam();
        ~Beam(void);

        bool ReadFile();
        bool ReadElement();
        void WriteElement();

        bool CheckSoftpotDataLimit();
        bool CheckReschTreatToDefault(RE_TE_MODE mode);
        bool CheckSoftpotDataIntegrity();

        bool ComputeSendCHK(UINT8* data, UINT8 len, UINT8& fc);
        UINT8 ComputeRevCHK(UINT8* data, UINT8 len);
             //发送到从机的命令函数
        //bool SendAfcpInjeInji(UINT16 afcp,UINT16 inje, UINT16 inji);

        bool SendC0();
        bool SendAfcpInjeInji();
        bool SendElmBmiStcLen();//UINT16 elm, UINT16 bmi,UINT16 stc, UINT16 len);
        bool SendPfnPrfpIpfnSomin();//UINT16 pfn, UINT16 prfp, UINT16 ipfn, UINT16 somin);
        bool SendDataNotUsed();
        bool SendBmisMinMax();//UINT16 bmisMin,UINT16 bmisMax);

        bool SendUserMode(USERMODE_TYPE_T slectmode);
        bool SendAccept(void);
        bool SendRadOn(void);
        bool SendInterupt(void);
        bool SendTerminateOrComplete();
        bool SendCommDog(void);
        bool SendCheckRequest(CHECKREQ_TYPE_T check);
        bool SendReadyStatus(READYSTATE_TYPE_T state);
        bool SendClear(void);

        //接收从机发送过来的数据
        bool AnalysRevFram(TransRecvT& revdata);

        void ProcessElectBendSteerLenIL();
private:

        //解析从机发送过来的数据
        //解析从机发送过来的数据
        bool RevAfcmBeamiInjpiPusm(TransRecvT &revdata);
        bool RevElectBendSteerLen(TransRecvT &revdata);
        bool RevPulseVoltage(TransRecvT &revdata);
        bool RevErrorReport(TransRecvT &revdata);
        bool RevCommDog(TransRecvT &revdata);
        bool RevEmergenLineStatus(TransRecvT &revdata);
        bool RevCheckReport(TransRecvT &revdata);
        bool RevResetEcho(TransRecvT &revdata);


        //void ProcessElectBendSteerLenIL();
        void ProcessErrorReportIL();
        bool ProcessCHKIL(TransRecvT &revdata);


public:

        //存储发送到Beam的数据
        UINT16 afcp[3];
        UINT16 inje[3];
        UINT16 inji[3];

        UINT16 elm[3];
        UINT16 bmi[3];
        UINT16 stc[3];
        UINT16 len[3];

        UINT16 pfn[3];
        UINT16 prfp[3];
        UINT16 ipfn[3];
        UINT16 somin[3];

        UINT16 bmisMin[3];
        UINT16 bmisMax[3];

        //不发送
        UINT16 elmck[3];
        UINT16 bmick[3];
        UINT16 stcck[3];
        UINT16 lenck[3];

        //LJY-12-18
        float elmck_f[3];
        float bmick_f[3];
        float stcck_f[3];
        float lenck_f[3];

        USERMODE_TYPE_T slectmode;
        CHECKREQ_TYPE_T check;
        READYSTATE_TYPE_T state;
        //存储接收到Beam的数据

        UINT16 afcm;   //should be removed , same as afcp
        UINT16 beami;   // shoul
        UINT16 injpi;   // inji
        UINT16 phsm;

        UINT16 elmi;   //elm
        UINT16 bmis;      //bmi
        UINT16 stci;     //stc
        UINT16 leni;      //len

        float elmi_f;   //elm
        float bmis_f;      //bmi
        float stci_f;     //stc
        float leni_f;      //len

        UINT16 pfne;

        //ERROR_REPORT_T error_now;
        UINT8   error_now;
        //LINE_STATUS_BEAM_T line_status;
        UINT8  line_status;
        //CHECKREPORT_BEAM_T check_report;
        UINT8 check_report;
        UINT32  prom_version;



private:
    QMutex send_frame_num_mutex;
    UINT8 send_frame_num;
    QMutex rev_frame_num_mutex;
    UINT8 rev_frame_num;
    QMutex beam_mutex;

    bool item_has[51];

};

extern RE_TE_MODE g_mode;
//extern bool g_commdog;
extern QTimer *g_commdog_timer;


#endif // BEAM_H
