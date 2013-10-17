/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Mon Jul 22 08:57:19 2013
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "QtPaint/gui/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: signature, parameters, type, tag, flags
      21,   12,   11,   11, 0x05,
      53,   11,   11,   11, 0x05,
      77,   11,   11,   11, 0x05,
     100,   11,   11,   11, 0x05,
     129,  124,   11,   11, 0x05,
     170,  163,   11,   11, 0x05,
     211,  201,   11,   11, 0x05,

 // slots: signature, parameters, type, tag, flags
     241,   11,   11,   11, 0x08,
     267,   11,   11,   11, 0x08,
     293,   11,   11,   11, 0x08,
     321,   11,   11,   11, 0x08,
     354,   11,   11,   11, 0x08,
     386,   11,   11,   11, 0x08,
     408,   11,   11,   11, 0x08,
     430,   11,   11,   11, 0x08,
     453,   11,   11,   11, 0x08,
     475,   11,   11,   11, 0x08,
     498,  124,   11,   11, 0x08,
     530,  163,   11,   11, 0x08,
     565,  201,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0newShape\0"
    "changeCurrentShape(Shape::Code)\0"
    "startSimulationSignal()\0stopSimulationSignal()\0"
    "resetSimulationSignal()\0name\0"
    "rrtTextChangedSignalMain(QString)\0"
    "length\0edgeLengthChangedMain(QString)\0"
    "iteration\0iterationChangedMain(QString)\0"
    "drawLineActionTriggered()\0"
    "drawRectActionTriggered()\0"
    "drawCircleActionTriggered()\0"
    "drawStartCircleActionTriggered()\0"
    "drawGoalCircleActionTriggered()\0"
    "saveActionTriggered()\0loadActionTriggered()\0"
    "startActionTriggered()\0stopActionTriggered()\0"
    "resetActionTriggered()\0"
    "rrtTextChangedSlotMain(QString)\0"
    "edgeLengthChangedSlotMain(QString)\0"
    "iterationChangedSlotMain(QString)\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: changeCurrentShape((*reinterpret_cast< Shape::Code(*)>(_a[1]))); break;
        case 1: startSimulationSignal(); break;
        case 2: stopSimulationSignal(); break;
        case 3: resetSimulationSignal(); break;
        case 4: rrtTextChangedSignalMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: edgeLengthChangedMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 6: iterationChangedMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: drawLineActionTriggered(); break;
        case 8: drawRectActionTriggered(); break;
        case 9: drawCircleActionTriggered(); break;
        case 10: drawStartCircleActionTriggered(); break;
        case 11: drawGoalCircleActionTriggered(); break;
        case 12: saveActionTriggered(); break;
        case 13: loadActionTriggered(); break;
        case 14: startActionTriggered(); break;
        case 15: stopActionTriggered(); break;
        case 16: resetActionTriggered(); break;
        case 17: rrtTextChangedSlotMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 18: edgeLengthChangedSlotMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 19: iterationChangedSlotMain((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::changeCurrentShape(Shape::Code _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::startSimulationSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void MainWindow::stopSimulationSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void MainWindow::resetSimulationSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 3, 0);
}

// SIGNAL 4
void MainWindow::rrtTextChangedSignalMain(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void MainWindow::edgeLengthChangedMain(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void MainWindow::iterationChangedMain(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_END_MOC_NAMESPACE
