/****************************************************************************
** Meta object code from reading C++ file 'performancewidget.h'
**
** Created: Mon Jul 22 07:27:28 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QtPaint/gui/performancewidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'performancewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PerformanceWidget[] = {

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
      29,   19,   18,   18, 0x0a,
      72,   62,   18,   18, 0x0a,
     116,  105,   18,   18, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PerformanceWidget[] = {
    "PerformanceWidget\0\0iteration\0"
    "currentIterationChangedSlot(int)\0"
    "nodecount\0currentNodecountChangedSlot(int)\0"
    "pathlength\0currentPathLengthChangedSlot(int)\0"
};

const QMetaObject PerformanceWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PerformanceWidget,
      qt_meta_data_PerformanceWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PerformanceWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PerformanceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PerformanceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PerformanceWidget))
        return static_cast<void*>(const_cast< PerformanceWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PerformanceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentIterationChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: currentNodecountChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: currentPathLengthChangedSlot((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
