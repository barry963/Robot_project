/****************************************************************************
** Meta object code from reading C++ file 'strategy_thread.h'
**
** Created: Mon Feb 17 21:59:16 2014
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
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      16,   15,   15,   15, 0x05,
      27,   15,   15,   15, 0x05,
      44,   40,   15,   15, 0x05,

 // slots: signature, parameters, type, tag, flags
      79,   40,   15,   15, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_StrategyThread[] = {
    "StrategyThread\0\0explains()\0guiRefresh()\0"
    "x,y\0Coordinate_Location(double,double)\0"
    "Coordinate(double,double)\0"
};

void StrategyThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        StrategyThread *_t = static_cast<StrategyThread *>(_o);
        switch (_id) {
        case 0: _t->explains(); break;
        case 1: _t->guiRefresh(); break;
        case 2: _t->Coordinate_Location((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->Coordinate((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        default: ;
        }
    }
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
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
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

// SIGNAL 2
void StrategyThread::Coordinate_Location(double _t1, double _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
