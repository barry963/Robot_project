/****************************************************************************
** Meta object code from reading C++ file 'paintwidget.h'
**
** Created: Mon Jul 22 08:57:17 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QtPaint/gui/paintwidget.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'paintwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_PaintWidget[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x05,
      35,   12,   12,   12, 0x05,
      57,   12,   12,   12, 0x05,

 // slots: signature, parameters, type, tag, flags
      82,   80,   12,   12, 0x0a,
     111,   12,   12,   12, 0x0a,
     133,   12,   12,   12, 0x0a,
     154,   12,   12,   12, 0x0a,
     181,  176,   12,   12, 0x0a,
     214,  209,   12,   12, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_PaintWidget[] = {
    "PaintWidget\0\0currentIteration(int)\0"
    "currentNodeCount(int)\0currentPathLength(int)\0"
    "s\0setCurrentShape(Shape::Code)\0"
    "startSimulationSlot()\0stopSimulationSlot()\0"
    "resetSimulationSlot()\0name\0"
    "rrtTextChangedSlot(QString)\0step\0"
    "stepChangedSlot(QString)\0"
};

const QMetaObject PaintWidget::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PaintWidget,
      qt_meta_data_PaintWidget, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &PaintWidget::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *PaintWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *PaintWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_PaintWidget))
        return static_cast<void*>(const_cast< PaintWidget*>(this));
    return QWidget::qt_metacast(_clname);
}

int PaintWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: currentIteration((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: currentNodeCount((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: currentPathLength((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: setCurrentShape((*reinterpret_cast< Shape::Code(*)>(_a[1]))); break;
        case 4: startSimulationSlot(); break;
        case 5: stopSimulationSlot(); break;
        case 6: resetSimulationSlot(); break;
        case 7: rrtTextChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 8: stepChangedSlot((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void PaintWidget::currentIteration(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PaintWidget::currentNodeCount(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PaintWidget::currentPathLength(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
