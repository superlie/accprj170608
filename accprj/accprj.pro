QT       += core gui xml network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AccPrj
TEMPLATE = app


HEADERS += \
    maincontrol/acc_type.h \
    maincontrol/beam.h \
    maincontrol/dosei.h \
    maincontrol/doseii.h \
    maincontrol/hand.h \
    maincontrol/interlock.h \
    maincontrol/io.h \
    maincontrol/light.h \
    maincontrol/main.h \
    maincontrol/mastercontroller.h \
    maincontrol/motor.h \
    maincontrol/senderthread.h \
    maincontrol/slave_base.h \
    maincontrol/slave_public.h \
    ui/accui.h \
    ui/clineedit.h \
    maincontrol/dosebase.h \
    can/Can.h \
    ui/keythread.h \
    ui/qextserialbase.h \
    ui/qextserialport.h \
    ui/win_qextserialport.h \
    ui_accui.h \
    ui/spkey.h \
    maincontrol/receiverthread.h \
    can/ControlCAN.H \
    can/ECanVci.h \
    ui/language.h \
    maincontrol/uppermonitor.h \
    maincontrol/mlc.h \
    maincontrol/remotecontroller.h \
    can/Can2.h \
    main.h \
    maincontrol/mastercommon.h \
    tools/ftpdownloader.h \
    tools/ftpuploader.h \
    tools/treatfileparser.h \
    ui/cdoseprogress.h \
    ui/csegprogress.h \
    tools/cfloatcompare.h \
    maincontrol/writethread.h \
    version.h \
    defines.h \
    maincontrol/remotesenderthread.h \
    tools/aes.h \
    tools/aesutils.h \
    tools/authorization.h

SOURCES += \
    maincontrol/beam.cpp \
    maincontrol/dosei.cpp \
    maincontrol/doseii.cpp \
    maincontrol/hand.cpp \
    maincontrol/inerlock.cpp \
    maincontrol/io.cpp \
    maincontrol/light.cpp \
    maincontrol/mastercontroller.cpp \
    maincontrol/motor.cpp \
    maincontrol/senderthread.cpp \
    maincontrol/slave_base.cpp \
    ui/accui.cpp \
    ui/clineedit.cpp \
    ui/setui.cpp \
    ui/softpotcopy.cpp \
    ui/startui.cpp \
    ui/treatui.cpp \
    ui/softpotrsch.cpp \
    maincontrol/dosebase.cpp \
    can/Can.cpp \
    ui/keyserial.cpp \
    ui/keythread.cpp \
    ui/qextserialbase.cpp \
    ui/qextserialport.cpp \
    ui/svcactvalue.cpp \
    ui/svcinitval.cpp \
    ui/svcset.cpp \
    ui/svcui.cpp \
    ui/treatactvalue.cpp \
    ui/treatset.cpp \
    ui/win_qextserialport.cpp \
    maincontrol/receiverthread.cpp \
    maincontrol/uppermonitor.cpp \
    maincontrol/mlc.cpp \
    maincontrol/remotecontroller.cpp \
    can/Can2.cpp \
    main.cpp \
    ui/remotectrlui.cpp \
    maincontrol/rmastercontroller.cpp \
    tools/ftpdownloader.cpp \
    tools/ftpuploader.cpp \
    tools/treatfileparser.cpp \
    ui/cdoseprogress.cpp \
    ui/csegprogress.cpp \
    tools/cfloatcompare.cpp \
    maincontrol/writethread.cpp \
    maincontrol/remotesenderthread.cpp \
    tools/aes.cpp \
    tools/aesutils.cpp \
    tools/authorization.cpp

FORMS += \
    accui.ui

LIBS += -L..\accprj\can -lControlCAN
#LIBS += -LD:\accprj0309\accprj\can -lECanVci

#DEFINES += QT_NO_WARNING_OUTPUT\
#            QT_NO_DEBUG_OUTPUT
