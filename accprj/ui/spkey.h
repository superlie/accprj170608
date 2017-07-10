#ifndef SPKEY_H
#define SPKEY_H

#include <qnamespace.h>

#define SPKEYON

#ifdef SPKEYON

#define UI_STANDBY_RKEY    Qt::Key_F21
#define UI_RADON_RKEY      Qt::Key_F23
#define UI_PRMRDY_RKEY     Qt::Key_F18

#define UI_RESET_LKEY      Qt::Key_F15
#define UI_TREAT_LKEY      Qt::Key_F16
#define UI_SVC_LKEY        Qt::Key_F17

#define UI_ACCEPT_KEY      Qt::Key_F22
#define UI_RADON_KEY       Qt::Key_F24
#define UI_RADOFF_KEY      Qt::Key_F25
#define UI_STOP_KEY        Qt::Key_F26
#define UI_CLEAR_KEY       Qt::Key_F20
#define UI_SETACT_KEY      Qt::Key_F19

#define UI_ALTEN_KEY       Qt::Key_F14
#define UI_ALTS_KEY        Qt::Key_F13

#else

#define UI_STANDBY_RKEY    Qt::Key_Escape
#define UI_RADON_RKEY      Qt::Key_O
#define UI_PRMRDY_RKEY     Qt::Key_P

#define UI_RESET_LKEY      Qt::Key_F11
#define UI_TREAT_LKEY      Qt::Key_T
#define UI_SVC_LKEY        Qt::Key_S

#define UI_ACCEPT_KEY      Qt::Key_L
#define UI_RADON_KEY       Qt::Key_K
#define UI_RADOFF_KEY      Qt::Key_J
#define UI_STOP_KEY        Qt::Key_H
#define UI_CLEAR_KEY       Qt::Key_F10
#define UI_SETACT_KEY      Qt::Key_F8

#define UI_ALTEN_KEY       Qt::Key_F7
#define UI_ALTS_KEY        Qt::Key_A

#endif // SPKEYON

#endif // SPKEY_H
