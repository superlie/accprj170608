#include "clineedit.h"
#include <QDebug>

CLineEdit::CLineEdit(QWidget *parent) :
    QLineEdit(parent),
    timer(0), color(ZERO)
{
    blink_timer = new QTimer();
    connect(blink_timer, SIGNAL(timeout()),this,SLOT(SetBlinkStyle()));
}

void CLineEdit::startBlink(BG_COLOR_T c)
{
    color = c;

    if (g_status == RAD_ON){
        if(!blink_timer->isActive())
            blink_timer->start(g_rad_on_delay_time.toInt());
    } else {
        if(!blink_timer->isActive())
            blink_timer->start(500);
    }


}

void CLineEdit::stopBlink()
{
    switch (color)
    {
    case RED:
        setStyleSheet("background-color:red");
        break;
    case WHITE:
    case YELLOW:
        setStyleSheet("");
        break;
    default:
        break;
    }

//    if (timer)
//    {
//        killTimer(timer);
//        timer = 0;
//    }
    if(blink_timer->isActive())
        blink_timer->stop();

    //rad_on_timer->stop();

}

//void CLineEdit::timerEvent(QTimerEvent *)
//{
//    switch (color)
//    {
//    case RED:
//        setStyleSheet(styleSheet().isEmpty() ? "background-color:red" : "");
//        break;
//    case WHITE:
//        setStyleSheet(styleSheet().isEmpty() ? "background-color:white" : "");
//        break;
//    case YELLOW:
//        setStyleSheet(styleSheet().isEmpty() ? "background-color:yellow" : "");
//        break;
//    default:
//        break;
//    }
//}

void CLineEdit::SetBlinkStyle()
{
    switch (color)
    {
    case RED:
        setStyleSheet(styleSheet().isEmpty() ? "background-color:red" : "");
        break;
    case WHITE:
        setStyleSheet(styleSheet().isEmpty() ? "background-color:white" : "");
        break;
    case YELLOW:
        setStyleSheet(styleSheet().isEmpty() ? "background-color:yellow" : "");
        break;
    default:
        break;
    }
    blink_timer->start(500);
}

