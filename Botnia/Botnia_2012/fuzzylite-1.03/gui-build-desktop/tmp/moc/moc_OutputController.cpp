/****************************************************************************
** Meta object code from reading C++ file 'OutputController.h'
**
** Created: Thu Nov 15 23:38:32 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../gui/gui/OutputController.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'OutputController.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_fl_gui__OutputController[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      26,   25,   25,   25, 0x09,
      45,   25,   25,   25, 0x09,
      67,   25,   25,   25, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_fl_gui__OutputController[] = {
    "fl_gui::OutputController\0\0onClickAddOutput()\0"
    "onClickRemoveOutput()\0onClickEditOutput()\0"
};

const QMetaObject fl_gui::OutputController::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_fl_gui__OutputController,
      qt_meta_data_fl_gui__OutputController, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &fl_gui::OutputController::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *fl_gui::OutputController::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *fl_gui::OutputController::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_fl_gui__OutputController))
        return static_cast<void*>(const_cast< OutputController*>(this));
    return QObject::qt_metacast(_clname);
}

int fl_gui::OutputController::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: onClickAddOutput(); break;
        case 1: onClickRemoveOutput(); break;
        case 2: onClickEditOutput(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
