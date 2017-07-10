#ifndef INTERLOCK_C_H
#define INTERLOCK_C_H

#include "slave_base.h"

//const UINT8 TOINTERLOCK = 0X16;

typedef enum{
    NoWrg = 0x0,
    WrgCmd=0x02,
    NotRelease=0x09,
    SpvLineErr=0x0b,
    JumpPCB=0x0c,
    InterlockIfcMiss=0x0d,
    RamFaile=0x15,
    RomFaile=0x16,
    RamRomFaile=0x17,
    WrgDIP=0x1e,
    WrgFrameNumChkSum=0x61,
    WrgChkSum=0x62,
    WrgFrameNum=0x63
} ERROR_INTERLOCK_REPORT_T;


typedef struct{
    UINT8 pcb;
    UINT16 bit_array;
    UINT16 bit_array2;
}INTERLOCK_STATUS_ST;

class Interlock : public SlaveBase
{
    public:
        Interlock(void);
        ~Interlock(void);
        bool ReadOwnFile();
        bool ReadOwnElement();
        bool SetInterlockOvrdType();
        bool WriteOwnFile();
        bool WriteOwnElement();

        bool ReadErrorDisplayFile();
        bool ReadErrorDiaplayElement();
        bool WriteErrDisplayFile();
        bool WriteErrDisplayElement();

        bool ComputeCHK(UINT8* data, UINT8 len, UINT8& fc);

        bool SendC0();
        bool SendUserMode(USERMODE_TYPE_T mode);
        bool SendSetup(void);
        bool SendAccept(void);
        bool SendRadOn(void);
        bool SendInterrupt(void);
        bool SendTerminateOrComplete(void);
        bool SendClear(void);
        bool SendCheckRequest(const CHECKREQ_TYPE_T index);
        bool SendReadyStatus(READYSTATE_TYPE_T rdystat);

        bool AnalysRevFram(TransRecvT& revdata);
        UINT8 ComputeRevCHK(UINT8* data, UINT8 len);

        bool RevErrorReprot(TransRecvT& revdata);
        bool RevInterlockStatus(TransRecvT& revdata);
        bool RevCheckReport(TransRecvT& revdata);
        bool RevT1Report(TransRecvT& revdata);
        bool RevResetEcho(TransRecvT& revdata);

        bool GetErrorReport(ERROR_INTERLOCK_REPORT_T err);
        bool GetIntelockStatus(UINT8 pcbNo,UINT16 buf1Bits,UINT16 buf2Bits);
        bool GetCheckReport(UINT8 checkNo,UINT8 faile);
        bool GetT1Report(UINT8 active);
        bool GetResetEcho(UINT32 version);

        void ProcessInterlockStatusIL();
        void ProcessErrorReprotIL();
        bool ProcessCHKIL(TransRecvT &revdata);

        USERMODE_TYPE_T usrmode;
        READYSTATE_TYPE_T rdystat;
        ERROR_INTERLOCK_REPORT_T error_now;
        INTERLOCK_STATUS_ST interlock_status;
        //CHECKREQ_TYPE_T check_report;
        UINT8 line_status;
        UINT8 check_report;
        UINT32 prom_version;

private:
        QMutex send_frame_num_mutex;
        UINT8 send_frame_num;
        QMutex rev_frame_num_mutex;
        UINT8 rev_frame_num;
        QMutex interlock_mutex;

        USERMODE_TYPE_T mode_now;
};


#endif // INTERLOCK_C_H
