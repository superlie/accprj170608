#ifndef CDOSEPROGRESS_H
#define CDOSEPROGRESS_H

#include <QPaintEvent>
#include <basetsd.h>
#include <QWidget>
#include <QLabel>

class CDoseProgress : public QLabel
{
public:
    CDoseProgress(QWidget *parent);

public:
    void SetMax(UINT32 max);
    void SetCur(UINT32 cur);

    void UpdateProgress(UINT32 cur);

protected:
    void paintEvent(QPaintEvent *event);

private:
    UINT32 mMax;
    UINT32 mCur;
    QColor mColor;
};

#endif // CDOSEPROGRESS_H
