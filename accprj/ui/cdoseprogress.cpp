#include "cdoseprogress.h"
#include <QDebug>
#include <QPainter>
#include <QPen>

CDoseProgress::CDoseProgress(QWidget *parent) :
    QLabel(parent)
{
    mMax = 300;
    mCur = 0;
    mColor = QColor(26,168,60);
}

void CDoseProgress::SetMax(UINT32 max)
{
    mMax = max;
}

void CDoseProgress::SetCur(UINT32 cur)
{
    mCur = cur;
}

void CDoseProgress::UpdateProgress(UINT32 cur)
{
    SetCur(cur);
    this->update();
}

void CDoseProgress::paintEvent(QPaintEvent *event)
{
    QPainter painter(this); //this为绘图设备，即表明在该部件上进行绘制
    QBrush brush(mColor);
    QPen pen(mColor, 2);
    painter.setPen(pen);
    painter.setBrush(brush);

    painter.drawRect(0, 0, ((float)mCur/mMax)*frameGeometry().width(), this->frameGeometry().height());

    //qDebug() << "progress cur" << mCur << endl;

    QLabel::paintEvent(event);
}
