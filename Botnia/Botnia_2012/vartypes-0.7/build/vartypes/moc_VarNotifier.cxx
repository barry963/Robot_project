/****************************************************************************
** Meta object code from reading C++ file 'VarNotifier.h'
**
** Created: Thu Nov 8 16:48:59 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../vartypes/VarNotifier.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VarNotifier.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VarTypes__VarNotifier[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      28,   23,   22,   22, 0x05,

 // slots: signature, parameters, type, tag, flags
      51,   22,   22,   22, 0x0a,
      75,   23,   22,   22, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_VarTypes__VarNotifier[] = {
    "VarTypes::VarNotifier\0\0item\0"
    "changeOccurred(VarPtr)\0changeSlotOtherChange()\0"
    "changeSlot(VarPtr)\0"
};

const QMetaObject VarTypes::VarNotifier::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_VarTypes__VarNotifier,
      qt_meta_data_VarTypes__VarNotifier, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VarTypes::VarNotifier::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VarTypes::VarNotifier::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VarTypes::VarNotifier::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VarTypes__VarNotifier))
        return static_cast<void*>(const_cast< VarNotifier*>(this));
    return QObject::qt_metacast(_clname);
}

int VarTypes::VarNotifier::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeOccurred((*reinterpret_cast< VarPtr(*)>(_a[1]))); break;
        case 1: changeSlotOtherChange(); break;
        case 2: changeSlot((*reinterpret_cast< VarPtr(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void VarTypes::VarNotifier::changeOccurred(VarPtr _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
