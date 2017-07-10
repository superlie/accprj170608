#include "csegprogress.h"
#include <QDebug>
#include <QPainter>
#include <QPen>

CSegProgress::CSegProgress(QWidget *parent) :
    QLabel(parent)
{
    mMax = 1;
    mCur = 0;
    mColor = QColor(26,168,60);
}

void CSegProgress::UpdateProgress(UINT8 cur)
{
    SetCur(cur);
    this->update();
}

void CSegProgress::StepToNextProgress()
{
    if (mCur < mMax)
        mCur++;

    this->update();
}

void CSegProgress::SetMax(UINT8 max)
{
    mMax = max;
}

void CSegProgress::SetCur(UINT8 cur)
{
    mCur = cur;
}

void CSegProgress::paintEvent(QPaintEvent *event)
{
    QPainter painter(this); //this为绘图设备，即表明在该部件上进行绘制
    QBrush brush(mColor);
    QPen pen(QColor(255,255,255), 2);
    painter.setPen(pen);
    painter.setBrush(brush);

    int width = this->frameGeometry().width()/mMax;
    //qDebug() << "width" << width;

    for (int i=0; i<mCur; i++)
    {
        painter.drawRect(i*width, 0, width, this->frameGeometry().height());
    }

    //qDebug() << "progress cur" << mCur << "progress max" << mMax << endl;

    QLabel::paintEvent(event);
}
