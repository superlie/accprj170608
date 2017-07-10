#ifndef CSEGPROGRESS_H
#define CSEGPROGRESS_H

#include <QWidget>
#include <Qlabel>
#include <QPaintEvent>
#include <basetsd.h>

class CSegProgress : public QLabel
{
public:
    CSegProgress(QWidget *parent);

public:
    void UpdateProgress(UINT8 cur);
    void StepToNextProgress();
    void SetMax(UINT8 max);
    void SetCur(UINT8 cur);

protected:
    void paintEvent(QPaintEvent *event);

private:
    UINT8 mMax;
    UINT8 mCur;
    QColor mColor;
};

#endif // CSEGPROGRESS_H
