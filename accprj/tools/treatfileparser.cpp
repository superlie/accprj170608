#include "treatfileparser.h"
#include <QDebug>
#include <QFile>
#include <QTextStream>

TreatfileParser::TreatfileParser()
{
    mInterrupt = false;
    mFilename = "D:\\ftp\\treatment.xml";
}

bool TreatfileParser::ReadFromXml(TREATDATA_SEQ_ST &mTreatQueue, TREAT_FILE_CHECK_T &fileCheck)
{
    return ReadTreatFile(mTreatQueue, fileCheck);
}

//读取自己模块的文件
bool TreatfileParser::ReadTreatFile(TREATDATA_SEQ_ST &mTreatQueue, TREAT_FILE_CHECK_T &fileCheck)
{
    QFile file(mFilename);
    mTreatSqu = "";

    //QMutexLocker locker(&mQueueMutex);
    mTreatQueue.curBeamPosIdx = 0;
    mTreatQueue.beamNumber = 0;
    mTreatQueue.interrupted = IR_FALSE;

    for (int i=0; i<mTreatQueue.treatSequ.size(); i++)
    {
        mTreatQueue.beamSequ[mTreatQueue.treatSequ[i]].ctrlPointSequ.clear();
    }
    mTreatQueue.treatSequ.clear();
    mTreatQueue.beamSequ.clear();

    try
    {
        if (!file.open(QFile::ReadOnly | QFile::Text))  //以只读方式打开
        {
            fileCheck = TF_OPEN_ERR;
            return false;
        }

        QString errMsg = "";
        int errorLine, errorCol;
        if(!mDomDoc.setContent(&file, false, &errMsg, &errorLine, &errorCol))
        {
            qDebug() << "doc setContent error! errMag:" << errMsg
                     << "errLine:" << errorLine << "errCol:" << errorCol << endl;
            file.close();
            return false;
        }

        QDomElement root = mDomDoc.documentElement();
        if (root.tagName() == "TreatmentData")
        {
            ReadTreatDataElement(root, mTreatQueue);
        }
        else
        {
            file.close();
            fileCheck = TF_READ_ERR;
            return false;
        }

        file.close();
    }
    catch (QString exception)
    {
        return false;
    }

    return true;
}

bool TreatfileParser::ReadTreatDataElement(QDomElement treatdata, TREATDATA_SEQ_ST &mTreatQueue)
{
    QString tmp;
    QDomNode child = treatdata.firstChild();

    while (!child.isNull())
    {
        if (child.toElement().tagName() == "MachineName")
        {
            mTreatQueue.machineName = child.toElement().text();
        }
        else if (child.toElement().tagName() == "BeamNumber")
        {
            mTreatQueue.beamNumber = child.toElement().text().toInt();
        }
        else if (child.toElement().tagName() == "TreatSequence")
        {
            ConvertSequencesToInt(mTreatQueue, child.toElement().text());
        }
        else if (child.toElement().tagName() == "Interrupted")
        {
            tmp = child.toElement().text();
            if (tmp == "TRUE")
            {
                mTreatQueue.interrupted = IR_TRUE;
            }
            else if (tmp == "FALSE")
            {
                mTreatQueue.interrupted = IR_FALSE;
            }
            else
            {
                mTreatQueue.interrupted = IR_ERR;
            }
        }
        else if ("BeamSequence" == child.toElement().tagName())
        {
            QDomNode beamNode = child.firstChild();

            while (!beamNode.isNull())
            {
                ReadBeamElement(beamNode, mTreatQueue);
                beamNode = beamNode.nextSibling();
            }
        }

        child = child.nextSibling();
    }

    return true;
}

bool TreatfileParser::ReadBeamElement(QDomNode beamNode, TREATDATA_SEQ_ST &mTreatQueue)
{
    BEAMDATA_ST beamData;
    QString tmp;

    beamData.curCtrlPointIdx = 0;
    QDomNode child = beamNode.firstChild();
    float tmp1 = 0;

    while (!child.isNull())
    {
        if ("BeamIndex" == child.toElement().tagName())
        {
            beamData.beamIdx = child.toElement().text().toInt();
        }
        else if (child.toElement().tagName() == "BeamType")
        {
            tmp = child.toElement().text();
            if (tmp == "CONFORMAL")
            {
                beamData.beamType = BT_CONFORMAL;
            }
            else if (tmp == "STATIC")
            {
                beamData.beamType = BT_STATIC;
            }
            else if (tmp == "DYNAMIC")
            {
                beamData.beamType = BT_DYNAMIC;
            }
            else
            {
                beamData.beamType = BT_ERR;
            }
        }
        else if ("BeamMU" == child.toElement().tagName())
        {
            beamData.finalMeterSet = child.toElement().text().toInt();
        }
        else if ("DeliveredMU" == child.toElement().tagName())
        {
            beamData.beamDeliveredMu = child.toElement().text().toInt();
        }
        else if ("InterruptedControlPointId" == child.toElement().tagName())
        {
            beamData.interruptedControlPointId = child.toElement().text().toInt();
        }
        else if ("InterruptedControlPointDeliveredMU" == child.toElement().tagName())
        {
            beamData.interruptedControlPointDeliveredMU = child.toElement().text().toInt();
        }
        else if ("RayType" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            if ("PHOTON" == tmp)
            {
                beamData.rayType = RT_XRAY;
            }
            else
            {
                beamData.rayType = RT_ERR;
            }
        }
        else if ("Energy" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            if ("6" == tmp)
            {
                beamData.energy = EG_LOW;
            }
            else if ("10" == tmp)
            {
                beamData.energy = EG_HIGH;
            }
            else
            {
                beamData.energy = EG_ERR;
            }
        }
        else if ("DoseRate" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            if ("50" == tmp)
            {
                beamData.doserate = DR_LOW;
            }
            else if ("300" == tmp)
            {
                beamData.doserate = DR_HIGH;
            }
            else
            {
                beamData.doserate = DR_ERR;
            }
        }
        else if ("Technique" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            if ("Fix" == tmp)
            {
                beamData.tech = TECH_FIX;
            }
            else if ("Rotation" == tmp)
            {
                beamData.tech = TECH_ROT;
            }
            else
            {
                beamData.tech = TECH_ERR;
            }
        }
        else if ("CollimatorAngle" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }

            beamData.colliAngle = tmp.toFloat();        //只保留一位小数
        }
        else if ("GantryAngle" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }

            beamData.gantryAngle = tmp.toFloat();
        }
        else if ("X1" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }

            beamData.x1Pos = tmp.toFloat();
        }
        else if ("X2" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            beamData.x2Pos = tmp.toFloat();
        }
        else if ("Y1" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            beamData.y1Pos = tmp.toFloat();
        }
        else if ("Y2" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            beamData.y2Pos = tmp.toFloat();
        }
        else if ("Accessory1" == child.toElement().tagName())
        {
            beamData.acc1 = child.toElement().text();
        }
        else if ("Accessory2" == child.toElement().tagName())
        {
            beamData.acc2 = child.toElement().text();
        }
        else if ("Accessory3" == child.toElement().tagName())
        {
            beamData.acc3 = child.toElement().text();
        }
        else if ("ControlPointNumber" == child.toElement().tagName())
        {
            beamData.ctrlPointNumber = child.toElement().text().toInt();
        }
        else if ("ControlPointSequence" == child.toElement().tagName())
        {
            QDomNode pointNode = child.firstChild();

            while (!pointNode.isNull())
            {
                ReadCtrlPointElement(pointNode, beamData);

                pointNode = pointNode.nextSibling();
            }
        }

        child = child.nextSibling();
    }

    beamData.ctrlPointNumber = beamData.ctrlPointSequ.size();

    if (beamData.ctrlPointNumber == 1)
        beamData.beamType = BT_CONFORMAL;

    mTreatQueue.beamSequ[beamData.beamIdx] = beamData;
    return true;
}

bool TreatfileParser::ReadCtrlPointElement(QDomNode pointNode, BEAMDATA_ST &beamData)
{
    CONTROLPOINT_ST ctrlPointData;
    QDomNode child = pointNode.firstChild();
    ctrlPointData.setXY = false;
    QString tmp;

    while (!child.isNull())
    {
        if ("ControlPointIndex" == child.toElement().tagName())
        {
            ctrlPointData.ctrlPointIdx = child.toElement().text().toInt();
        }
        else if ("ControlPointMU" == child.toElement().tagName())
        {
            ctrlPointData.meterSet = child.toElement().text().toInt();
        }
        else if ("X1" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            ctrlPointData.x1Pos = tmp.toFloat();
            ctrlPointData.setXY = true;
        }
        else if ("X2" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            ctrlPointData.x2Pos = tmp.toFloat();
        }
        else if ("Y1" == child.toElement().tagName())
        {
            ctrlPointData.setXY = true;
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            ctrlPointData.y1Pos = tmp.toFloat();
        }
        else if ("Y2" == child.toElement().tagName())
        {
            tmp = child.toElement().text();
            QStringList vs = tmp.split(".");
            if (vs.size() == 2)
            {
                tmp = vs[0]+"."+vs[1][0];
            }
            ctrlPointData.y2Pos = tmp.toFloat();
        }

        child = child.nextSibling();
    }

    beamData.ctrlPointSequ.push_back(ctrlPointData);
    return true;
}

void TreatfileParser::ConvertSequencesToInt(TREATDATA_SEQ_ST &mTreatQueue, QString sequences)
{
    QStringList list = sequences.split(",", QString::SkipEmptyParts);

    for(int i=0; i<list.count(); i++)
    {
        mTreatQueue.treatSequ.push_back(list[i].toInt());
    }
}
