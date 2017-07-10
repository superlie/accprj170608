/****************************************************************************
** Meta object code from reading C++ file 'remotecontroller.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../accprj/maincontrol/remotecontroller.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'remotecontroller.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AutosetDataThread_t {
    QByteArrayData data[11];
    char stringdata[127];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AutosetDataThread_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AutosetDataThread_t qt_meta_stringdata_AutosetDataThread = {
    {
QT_MOC_LITERAL(0, 0, 17),
QT_MOC_LITERAL(1, 18, 15),
QT_MOC_LITERAL(2, 34, 0),
QT_MOC_LITERAL(3, 35, 3),
QT_MOC_LITERAL(4, 39, 19),
QT_MOC_LITERAL(5, 59, 3),
QT_MOC_LITERAL(6, 63, 20),
QT_MOC_LITERAL(7, 84, 8),
QT_MOC_LITERAL(8, 93, 3),
QT_MOC_LITERAL(9, 97, 11),
QT_MOC_LITERAL(10, 109, 17)
    },
    "AutosetDataThread\0setBigDataInput\0\0"
    "dos\0setTreatdataWithKey\0key\0"
    "setTreatdataWithData\0lineedit\0str\0"
    "ShowTreatUI\0resetUISelectStep"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AutosetDataThread[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       6,    2,   45,    2, 0x06 /* Public */,
       9,    0,   50,    2, 0x06 /* Public */,
      10,    0,   51,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void AutosetDataThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AutosetDataThread *_t = static_cast<AutosetDataThread *>(_o);
        switch (_id) {
        case 0: _t->setBigDataInput((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setTreatdataWithKey((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->setTreatdataWithData((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 3: _t->ShowTreatUI(); break;
        case 4: _t->resetUISelectStep(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AutosetDataThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AutosetDataThread::setBigDataInput)) {
                *result = 0;
            }
        }
        {
            typedef void (AutosetDataThread::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AutosetDataThread::setTreatdataWithKey)) {
                *result = 1;
            }
        }
        {
            typedef void (AutosetDataThread::*_t)(int , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AutosetDataThread::setTreatdataWithData)) {
                *result = 2;
            }
        }
        {
            typedef void (AutosetDataThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AutosetDataThread::ShowTreatUI)) {
                *result = 3;
            }
        }
        {
            typedef void (AutosetDataThread::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AutosetDataThread::resetUISelectStep)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject AutosetDataThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_AutosetDataThread.data,
      qt_meta_data_AutosetDataThread,  qt_static_metacall, 0, 0}
};


const QMetaObject *AutosetDataThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AutosetDataThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_AutosetDataThread.stringdata))
        return static_cast<void*>(const_cast< AutosetDataThread*>(this));
    return QThread::qt_metacast(_clname);
}

int AutosetDataThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AutosetDataThread::setBigDataInput(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AutosetDataThread::setTreatdataWithKey(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AutosetDataThread::setTreatdataWithData(int _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AutosetDataThread::ShowTreatUI()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void AutosetDataThread::resetUISelectStep()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
struct qt_meta_stringdata_RemoteController_t {
    QByteArrayData data[14];
    char stringdata[184];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RemoteController_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RemoteController_t qt_meta_stringdata_RemoteController = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 12),
QT_MOC_LITERAL(2, 30, 0),
QT_MOC_LITERAL(3, 31, 19),
QT_MOC_LITERAL(4, 51, 18),
QT_MOC_LITERAL(5, 70, 17),
QT_MOC_LITERAL(6, 88, 4),
QT_MOC_LITERAL(7, 93, 12),
QT_MOC_LITERAL(8, 106, 3),
QT_MOC_LITERAL(9, 110, 20),
QT_MOC_LITERAL(10, 131, 20),
QT_MOC_LITERAL(11, 152, 3),
QT_MOC_LITERAL(12, 156, 13),
QT_MOC_LITERAL(13, 170, 13)
    },
    "RemoteController\0setTreatData\0\0"
    "startHeartBeatTimer\0stopHeartBeatTimer\0"
    "showRemoteControl\0show\0setRemoteMsg\0"
    "msg\0updateSegProgressBar\0updateDosProgressBar\0"
    "dos\0resetProgress\0backToProgram"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RemoteController[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    0,   61,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       7,    1,   65,    2, 0x06 /* Public */,
       9,    0,   68,    2, 0x06 /* Public */,
      10,    1,   69,    2, 0x06 /* Public */,
      12,    0,   72,    2, 0x06 /* Public */,
      13,    0,   73,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RemoteController::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RemoteController *_t = static_cast<RemoteController *>(_o);
        switch (_id) {
        case 0: _t->setTreatData(); break;
        case 1: _t->startHeartBeatTimer(); break;
        case 2: _t->stopHeartBeatTimer(); break;
        case 3: _t->showRemoteControl((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->setRemoteMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->updateSegProgressBar(); break;
        case 6: _t->updateDosProgressBar((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->resetProgress(); break;
        case 8: _t->backToProgram(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::setTreatData)) {
                *result = 0;
            }
        }
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::startHeartBeatTimer)) {
                *result = 1;
            }
        }
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::stopHeartBeatTimer)) {
                *result = 2;
            }
        }
        {
            typedef void (RemoteController::*_t)(bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::showRemoteControl)) {
                *result = 3;
            }
        }
        {
            typedef void (RemoteController::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::setRemoteMsg)) {
                *result = 4;
            }
        }
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::updateSegProgressBar)) {
                *result = 5;
            }
        }
        {
            typedef void (RemoteController::*_t)(int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::updateDosProgressBar)) {
                *result = 6;
            }
        }
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::resetProgress)) {
                *result = 7;
            }
        }
        {
            typedef void (RemoteController::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RemoteController::backToProgram)) {
                *result = 8;
            }
        }
    }
}

const QMetaObject RemoteController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RemoteController.data,
      qt_meta_data_RemoteController,  qt_static_metacall, 0, 0}
};


const QMetaObject *RemoteController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RemoteController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteController.stringdata))
        return static_cast<void*>(const_cast< RemoteController*>(this));
    return QObject::qt_metacast(_clname);
}

int RemoteController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void RemoteController::setTreatData()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void RemoteController::startHeartBeatTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void RemoteController::stopHeartBeatTimer()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void RemoteController::showRemoteControl(bool _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void RemoteController::setRemoteMsg(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void RemoteController::updateSegProgressBar()
{
    QMetaObject::activate(this, &staticMetaObject, 5, 0);
}

// SIGNAL 6
void RemoteController::updateDosProgressBar(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void RemoteController::resetProgress()
{
    QMetaObject::activate(this, &staticMetaObject, 7, 0);
}

// SIGNAL 8
void RemoteController::backToProgram()
{
    QMetaObject::activate(this, &staticMetaObject, 8, 0);
}
QT_END_MOC_NAMESPACE
