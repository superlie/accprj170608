#ifndef TREATFILEPARSER_H
#define TREATFILEPARSER_H

#include <basetsd.h>
#include <QString>
#include <QMutex>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <vector>
#include <map>
#include <QDateTime>
using namespace std;

typedef enum
{
    IR_ERR       = 0x00,
    IR_FALSE     = 0x01,
    IR_TRUE      = 0x02,
}INTERRUPTED_T;

typedef enum
{
    EG_ERR       = 0x00,
    EG_LOW       = 0x01,
    EG_HIGH      = 0x02,
}ENERGY_T;

typedef enum
{
    RT_ERR       = 0x00,
    RT_XRAY      = 0x01,
}RAYTYPE_T;

typedef enum
{
    DR_ERR       = 0x00,
    DR_LOW       = 0x01,
    DR_HIGH      = 0x02,
}DOSERATE_T;

typedef enum
{
    TECH_ERR       = 0x00,
    TECH_FIX       = 0x01,
    TECH_ROT      = 0x02,
}TECH_T;

typedef enum
{
    BT_ERR       = 0x00,
    BT_CONFORMAL = 0x01,
    BT_STATIC    = 0x02,
    BT_DYNAMIC   = 0x03,
}BEAMTYPE_T;

typedef struct
{
    UINT8 ctrlPointIdx;
    UINT16 meterSet;
    bool setXY;
    float x1Pos;
    float x2Pos;
    float y1Pos;
    float y2Pos;
}CONTROLPOINT_ST;

typedef struct
{
    UINT8 beamIdx;
    BEAMTYPE_T beamType;
    UINT16 finalMeterSet;
    UINT16 beamDeliveredMu;
    UINT16 interruptedControlPointId;
    UINT16 interruptedControlPointDeliveredMU;
    RAYTYPE_T rayType;
    ENERGY_T energy;
    DOSERATE_T doserate;
    TECH_T tech;
    QString acc1;
    QString acc2;
    QString acc3;
    UINT8 ctrlPointNumber;
    UINT8 curCtrlPointIdx;
    float colliAngle;
    float gantryAngle;
    float x1Pos;
    float x2Pos;
    float y1Pos;
    float y2Pos;
    vector<CONTROLPOINT_ST> ctrlPointSequ;
}BEAMDATA_ST;

typedef struct
{
    QString machineName;
    UINT8 beamNumber;
    UINT8 curBeamPosIdx;
    vector<UINT8> treatSequ;
    INTERRUPTED_T interrupted;
    map<UINT8, BEAMDATA_ST> beamSequ;
}TREATDATA_SEQ_ST;

typedef enum
{
    NOCHECK        = 0x80,
    ALL_COR        = 0,
    TF_COR         = 1,
    TF_OPEN_ERR,
    TF_READ_ERR,
    TF_VERIFY_ERR,

    MF_COR         = 0xF0,
    MF_OPEN_ERR,
    MF_READ_ERR,
    MF_VERIFY_ERR,
}TREAT_FILE_CHECK_T;

typedef struct
{
    QString x1;
    QString x2;
    QString y1;
    QString y2;
}INDP_JAW_ST;

class TreatfileParser
{
public:
    TreatfileParser();
    bool ReadFromXml(TREATDATA_SEQ_ST &mTreatQueue, TREAT_FILE_CHECK_T &fileCheck);

private:
    //read
    bool ReadTreatFile(TREATDATA_SEQ_ST &mTreatQueue, TREAT_FILE_CHECK_T &fileCheck);
    bool ReadTreatDataElement(QDomElement treatdata, TREATDATA_SEQ_ST &mTreatQueue);
    bool ReadBeamElement(QDomNode beamNode, TREATDATA_SEQ_ST &mTreatQueue);
    bool ReadCtrlPointElement(QDomNode pointNode, BEAMDATA_ST &beamData);
    void ConvertSequencesToInt(TREATDATA_SEQ_ST &mTreatQueue, QString sequences);

public:
    QDomDocument mDomDoc;
    QString mFilename;
    QString mTreatSqu;
    bool mInterrupt;

};

#endif // TREATFILEPARSER_H
