
#include "accui.h"
#include "ui_accui.h"
#include "../maincontrol/remotecontroller.h"

void AccUi::processKeyCom(QByteArray temp)
{
    unsigned char hexTemp = *((unsigned char *)temp.data());

    if(key_thread.isKeyDiagsUI)                 //键盘诊断
    {
        QString str;
        if(hexTemp==0x15)                    //Key_Q 退出
        {
            key_thread.isKeyDiagsUI = false;
            ShowAccReset();
        }
        else
        {
            ui->lineEditKey->setText(code[hexTemp]);
            str.sprintf("0x%x",hexTemp);
            ui->lineEditKeyCode->setText(str);
        }
    }
    else
    {

        switch(hexTemp)
        {
        case 0x45:
        case 0x70:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_0]);
            break;
        case 0x16:
        case 0x69:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_1]);
            break;
        case 0x1e:
        case 0x72:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_2]);
            break;
        case 0x26:
        case 0x7a:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_3]);
            break;
        case 0x25:
        case 0x6b:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_4]);
            break;
        case 0x2e:
        case 0x73:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_5]);
            break;
        case 0x36:
        case 0x74:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_6]);
            break;
        case 0x3d:
        case 0x6c:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_7]);
            break;
        case 0x3e:
        case 0x75:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_8]);
            break;
        case 0x46:
        case 0x7d:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_9]);
            break;

            //-
        case 0x4e:
        case 0x84:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_MINUS]);
            break;

            //A
        case 0x1C:
            QApplication::sendEvent(this, key[KEY_A]);
            break;
        case 0x32:
            QApplication::sendEvent(this, key[KEY_B]);
            break;
        case 0x21:
            QApplication::sendEvent(this, key[KEY_C]);
            break;
        case 0x23:
            QApplication::sendEvent(this, key[KEY_D]);
            break;
        case 0x24:
            QApplication::sendEvent(this, key[KEY_E]);
            break;
        case 0x2B:
            QApplication::sendEvent(this, key[KEY_F]);
            break;
        case 0x34:
            QApplication::sendEvent(this, key[KEY_G]);
            break;
        case 0x33:
            QApplication::sendEvent(this, key[KEY_H]);
            break;
        case 0x43:
            QApplication::sendEvent(this, key[KEY_I]);
            break;
        case 0x3B:
            QApplication::sendEvent(this, key[KEY_J]);
            break;
        case 0x42:
            QApplication::sendEvent(this, key[KEY_K]);
            break;
        case 0x4B:
            QApplication::sendEvent(this, key[KEY_L]);
            break;
        case 0x3A:
            QApplication::sendEvent(this, key[KEY_M]);
            break;
        case 0x31:
            QApplication::sendEvent(this, key[KEY_N]);
            break;
        case 0x44:
            QApplication::sendEvent(this, key[KEY_O]);
            break;
        case 0x4D:
            QApplication::sendEvent(this, key[KEY_P]);
            break;
        case 0x15:
            QApplication::sendEvent(this, key[KEY_Q]);
            break;
        case 0x2D:
            QApplication::sendEvent(this, key[KEY_R]);
            break;
            //s
        case 0x1B:
            if(alt_key == true)
            {
                alt_key = false;
                QApplication::sendEvent(this, key[KEY_F13]);
            }
            else
            {
                QApplication::sendEvent(this, key[KEY_S]);
            }
            break;
        case 0x2C:
            QApplication::sendEvent(this, key[KEY_T]);
            break;
        case 0x3C:
            QApplication::sendEvent(this, key[KEY_U]);
            break;
        case 0x2A:
            QApplication::sendEvent(this, key[KEY_V]);
            break;
        case 0x1D:
            QApplication::sendEvent(this, key[KEY_W]);
            break;
        case 0x22:
            QApplication::sendEvent(this, key[KEY_X]);
            break;
        case 0x35:
            QApplication::sendEvent(this, key[KEY_Y]);
            break;
        case 0x1A:
            QApplication::sendEvent(this, key[KEY_Z]);
            break;

        case 0x07:
            QApplication::sendEvent(this, key[KEY_F1]);
            break;
        case 0x0F:
            QApplication::sendEvent(this, key[KEY_F2]);
            break;
        case 0x17:
            QApplication::sendEvent(this, key[KEY_F3]);
            break;
        case 0x1F:
            QApplication::sendEvent(this, key[KEY_F4]);
            break;
        case 0x27:
            QApplication::sendEvent(this, key[KEY_F5]);
            break;
        case 0x2F:
            QApplication::sendEvent(this, key[KEY_F6]);
            break;
        case 0x37:
            QApplication::sendEvent(this, key[KEY_F7]);
            break;
        case 0x3F:
            QApplication::sendEvent(this, key[KEY_F8]);
            break;
        case 0x47:
            QApplication::sendEvent(this, key[KEY_F9]);
            break;
        case 0x4F:
            QApplication::sendEvent(this, key[KEY_F10]);
            break;
        case 0x56:
            QApplication::sendEvent(this, key[KEY_F11]);
            break;
        case 0x5E:
            QApplication::sendEvent(this, key[KEY_F12]);
            break;

            //enable
        case 0x0c:
            if(alt_key)
            {
                enable_key = true;
                QApplication::sendEvent(this, key[KEY_F14]);
            }
            break;

            //alt
        case 0x19:
            if(!release_key)
            {
                alt_key = true;
            }
            else
            {
                release_key = false;
                alt_key = false;
                if(enable_key)
                    QApplication::sendEvent(this, key[KEY_F14_RELEASE]);
            }
            break;
            //release flag
        case 0xF0:
            //qDebug() << "zhegezhege";
            release_key = true;
            break;
            //.
        case 0x49:
        case 0x71:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_PERIOD]);
            break;

            //Enter/ctrl+M
        case 0x79:
        case 0x5a:
            if(UI == SVC_C_INTERLOCK)
            {
                QApplication::sendEvent(this, key[KEY_RETURN]);
            }
            else
            {
                t = focusWidget();
                if(!t) return;
                QApplication::sendEvent(t, key[KEY_RETURN]);
            }
            break;
            //reset
        case 0x40:
            QApplication::sendEvent(this, key[KEY_RESET]);
            break;
            //treat
        case 0x38:
            QApplication::sendEvent(this, key[KEY_TREAT]);
            break;
            //svc
        case 0x30:
            QApplication::sendEvent(this, key[KEY_SVC]);
            break;
            //prg/rdy
        case 0x50:
            QApplication::sendEvent(this, key[KEY_PRGRDY]);
            break;
            //backspace
        case 0x66:
            t = focusWidget();
            if(!t) return;
            QApplication::sendEvent(t, key[KEY_BACKSPACE]);
            break;
            //set/act
        case 0x0b:
            QApplication::sendEvent(this, key[KEY_SETACT]);
            break;
            //clear
        case 0x0a:
            QApplication::sendEvent(this, key[KEY_CLEAR]);
            break;
            //stdby
        case 0x48:
            QApplication::sendEvent(this, key[KEY_STDBY]);
            break;
            //accept
        case 0x28:
            QApplication::sendEvent(this, key[KEY_ACCEPT]);
            break;
            //switch radon
        case 0x06:
            QApplication::sendEvent(this, key[SWITCH_RADON]);
            break;
            //key radon
        case 0x18:
            QApplication::sendEvent(this, key[KEY_RADON]);
            break;
            //key radoff
        case 0x20:
            QApplication::sendEvent(this, key[KEY_RADOFF]);
            break;
            //STOP
        case 0x10:
            QApplication::sendEvent(this, key[KEY_STOP]);
            break;
            //page up
        case 0x6F:
            QApplication::sendEvent(this, key[KEY_PAGEUP]);
            break;
            //page down
        case 0x6D:
            QApplication::sendEvent(this, key[KEY_PAGEDOWN]);
            break;
            //left
        case 0x61:
            QApplication::sendEvent(this, key[KEY_LEFT]);
            break;
            //right
        case 0x6A:
            QApplication::sendEvent(this, key[KEY_RIGHT]);
            break;
            //up
        case 0x63:
            QApplication::sendEvent(this, key[KEY_UP]);
            break;
            //down
        case 0x60:
            QApplication::sendEvent(this, key[KEY_DOWN]);
            break;
        case 0x08:
            QApplication::sendEvent(this, key[KEY_ESC]);
            break;
        default:
            //ui->lineEditMsg->setText("default");
            break;
        }
    }
}

void AccUi::ConstructKey()
{
    key[KEY_0] = new QKeyEvent(QEvent::KeyPress, Qt::Key_0, Qt::NoModifier, "0");
    key[KEY_1] = new QKeyEvent(QEvent::KeyPress, Qt::Key_1, Qt::NoModifier, "1");
    key[KEY_2] = new QKeyEvent(QEvent::KeyPress, Qt::Key_2, Qt::NoModifier, "2");
    key[KEY_3] = new QKeyEvent(QEvent::KeyPress, Qt::Key_3, Qt::NoModifier, "3");
    key[KEY_4] = new QKeyEvent(QEvent::KeyPress, Qt::Key_4, Qt::NoModifier, "4");
    key[KEY_5] = new QKeyEvent(QEvent::KeyPress, Qt::Key_5, Qt::NoModifier, "5");
    key[KEY_6] = new QKeyEvent(QEvent::KeyPress, Qt::Key_6, Qt::NoModifier, "6");
    key[KEY_7] = new QKeyEvent(QEvent::KeyPress, Qt::Key_7, Qt::NoModifier, "7");
    key[KEY_8] = new QKeyEvent(QEvent::KeyPress, Qt::Key_8, Qt::NoModifier, "8");
    key[KEY_9] = new QKeyEvent(QEvent::KeyPress, Qt::Key_9, Qt::NoModifier, "9");

    key[KEY_MINUS] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Minus, Qt::NoModifier, "-");//11-24

    key[KEY_A] = new QKeyEvent(QEvent::KeyPress, Qt::Key_A, Qt::NoModifier, "A");
    key[KEY_B] = new QKeyEvent(QEvent::KeyPress, Qt::Key_B, Qt::NoModifier, "B");
    key[KEY_C] = new QKeyEvent(QEvent::KeyPress, Qt::Key_C, Qt::NoModifier, "C");
    key[KEY_D] = new QKeyEvent(QEvent::KeyPress, Qt::Key_D, Qt::NoModifier, "D");
    key[KEY_E] = new QKeyEvent(QEvent::KeyPress, Qt::Key_E, Qt::NoModifier, "E");
    key[KEY_F] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F, Qt::NoModifier, "F");
    key[KEY_G] = new QKeyEvent(QEvent::KeyPress, Qt::Key_G, Qt::NoModifier, "G");
    key[KEY_H] = new QKeyEvent(QEvent::KeyPress, Qt::Key_H, Qt::NoModifier, "H");
    key[KEY_I] = new QKeyEvent(QEvent::KeyPress, Qt::Key_I, Qt::NoModifier, "I");
    key[KEY_J] = new QKeyEvent(QEvent::KeyPress, Qt::Key_J, Qt::NoModifier, "J");
    key[KEY_K] = new QKeyEvent(QEvent::KeyPress, Qt::Key_K, Qt::NoModifier, "K");
    key[KEY_L] = new QKeyEvent(QEvent::KeyPress, Qt::Key_L, Qt::NoModifier, "L");
    key[KEY_M] = new QKeyEvent(QEvent::KeyPress, Qt::Key_M, Qt::NoModifier, "M");
    key[KEY_N] = new QKeyEvent(QEvent::KeyPress, Qt::Key_N, Qt::NoModifier, "N");
    key[KEY_O] = new QKeyEvent(QEvent::KeyPress, Qt::Key_O, Qt::NoModifier, "O");
    key[KEY_P] = new QKeyEvent(QEvent::KeyPress, Qt::Key_P, Qt::NoModifier, "P");
    key[KEY_Q] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Q, Qt::NoModifier, "Q");
    key[KEY_R] = new QKeyEvent(QEvent::KeyPress, Qt::Key_R, Qt::NoModifier, "R");
    key[KEY_S] = new QKeyEvent(QEvent::KeyPress, Qt::Key_S, Qt::NoModifier, "S");
    key[KEY_T] = new QKeyEvent(QEvent::KeyPress, Qt::Key_T, Qt::NoModifier, "T");
    key[KEY_U] = new QKeyEvent(QEvent::KeyPress, Qt::Key_U, Qt::NoModifier, "U");
    key[KEY_V] = new QKeyEvent(QEvent::KeyPress, Qt::Key_V, Qt::NoModifier, "V");
    key[KEY_W] = new QKeyEvent(QEvent::KeyPress, Qt::Key_W, Qt::NoModifier, "W");
    key[KEY_X] = new QKeyEvent(QEvent::KeyPress, Qt::Key_X, Qt::NoModifier, "X");
    key[KEY_Y] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Y, Qt::NoModifier, "Y");
    key[KEY_Z] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Z, Qt::NoModifier, "Z");

    key[KEY_F1] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F1, Qt::NoModifier, "F1");
    key[KEY_F2] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F2, Qt::NoModifier, "F2");
    key[KEY_F3] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F3, Qt::NoModifier, "F3");
    key[KEY_F4] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F4, Qt::NoModifier, "F4");
    key[KEY_F5] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F5, Qt::NoModifier, "F5");
    key[KEY_F6] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F6, Qt::NoModifier, "F6");
    key[KEY_F7] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F7, Qt::NoModifier, "F7");
    key[KEY_F8] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F8, Qt::NoModifier, "F8");
    key[KEY_F9] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F9, Qt::NoModifier, "F9");
    key[KEY_F10] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F10, Qt::NoModifier, "F10");
    key[KEY_F10_RELEASE] = new QKeyEvent(QEvent::KeyRelease, Qt::Key_F10, Qt::NoModifier, "F10");
    key[KEY_F11] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F11, Qt::NoModifier, "F11");
    key[KEY_F12] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F12, Qt::NoModifier, "F12");
    //alt+s
    key[KEY_F13] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F13, Qt::NoModifier, "F13");
    //alt+enable
    key[KEY_F14] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F14, Qt::NoModifier, "F14");
    key[KEY_F14_RELEASE] = new QKeyEvent(QEvent::KeyRelease, Qt::Key_F14, Qt::NoModifier, "F14");


    key[KEY_PERIOD] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Period, Qt::NoModifier, ".");
    key[KEY_RETURN] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);

    key[KEY_RESET] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F15, Qt::NoModifier);
    key[KEY_TREAT] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F16, Qt::NoModifier);
    key[KEY_SVC] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F17, Qt::NoModifier);

    key[KEY_PRGRDY] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F18, Qt::NoModifier);

    key[KEY_BACKSPACE] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Backspace, Qt::NoModifier);
    key[KEY_SETACT] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F19, Qt::NoModifier);
    key[KEY_CLEAR] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F20, Qt::NoModifier);
    key[KEY_STDBY] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F21, Qt::NoModifier);
    key[KEY_ACCEPT] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F22, Qt::NoModifier);
    key[SWITCH_RADON] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F23, Qt::NoModifier);
    key[KEY_RADON] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F24, Qt::NoModifier);
    key[KEY_RADOFF] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F25, Qt::NoModifier);
    key[KEY_STOP] = new QKeyEvent(QEvent::KeyPress, Qt::Key_F26, Qt::NoModifier);

    key[KEY_PAGEUP] = new QKeyEvent(QEvent::KeyPress, Qt::Key_PageUp, Qt::NoModifier);
    key[KEY_PAGEDOWN] = new QKeyEvent(QEvent::KeyPress, Qt::Key_PageDown, Qt::NoModifier);

    key[KEY_LEFT] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
    key[KEY_RIGHT] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
    key[KEY_UP] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
    key[KEY_DOWN] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);

    key[KEY_ESC] = new QKeyEvent(QEvent::KeyPress, Qt::Key_Escape, Qt::NoModifier);
}
