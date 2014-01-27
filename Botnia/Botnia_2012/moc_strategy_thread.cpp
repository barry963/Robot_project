/****************************************************************************
** Meta object code from reading C++ file 'strategy_thread.h'
**
** Created: Mon Jan 27 21:37:37 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "control_hub/computer_control/intelligence/strategy_extutor/strategy_thread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'strategy_thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_StrategyThread[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      27,   15,   15,   15, 0x05,

       0        // eod
};

static const char qt_meta_stringdata_StrategyThread[] = {
    "StrategyThread\0\0explains()\0guiRefresh()\0"
};

void StrategyThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StrategyThread *_t = static_cast<StrategyThread *>(_o);
        switch (_id) {
        case 0: _t->explains(); break;
        case 1: _t->guiRefresh(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObjectExtraData StrategyThread::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject StrategyThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_StrategyThread,
      qt_meta_data_StrategyThread, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &StrategyThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *StrategyThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *StrategyThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StrategyThread))
        return static_cast<void*>(const_cast< StrategyThread*>(this));
    return QThread::qt_metacast(_clname);
}

int StrategyThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void StrategyThread::explains()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void StrategyThread::guiRefresh()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
