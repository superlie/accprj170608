#ifndef CLINEEDIT_H
#define CLINEEDIT_H

#include <QLineEdit>
#include <QTimerEvent>
#include <QTimer>
#include "maincontrol/slave_base.h"

extern QString g_rad_on_delay_time;
extern enum Status g_status;

typedef enum
{
    ZERO,
    RED,
    WHITE,
    YELLOW
} BG_COLOR_T;

class CLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    QTimer *blink_timer;
    explicit CLineEdit(QWidget *parent = 0);
    void startBlink(BG_COLOR_T c);
    void stopBlink();
protected:
    //void timerEvent(QTimerEvent *);
private:

   // QTimer *rad_on_timer;
    int timer;
    BG_COLOR_T color;

private slots:
    void SetBlinkStyle();
};

#endif // CLINEEDIT_H
