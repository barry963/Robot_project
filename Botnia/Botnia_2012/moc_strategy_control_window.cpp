/****************************************************************************
** Meta object code from reading C++ file 'strategy_control_window.h'
**
** Created: Thu Dec 26 02:25:41 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "user_interface/strategy_control_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'strategy_control_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      36,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x08,
      28,   11,   11,   11, 0x08,
      35,   11,   11,   11, 0x08,
      42,   11,   11,   11, 0x08,
      52,   11,   11,   11, 0x08,
      59,   11,   11,   11, 0x08,
      73,   11,   11,   11, 0x08,
      81,   11,   11,   11, 0x08,
      88,   11,   11,   11, 0x08,
      96,   11,   11,   11, 0x08,
     121,   11,   11,   11, 0x08,
     153,   11,   11,   11, 0x08,
     181,   11,   11,   11, 0x08,
     216,  210,   11,   11, 0x08,
     235,   11,   11,   11, 0x08,
     256,   11,   11,   11, 0x08,
     274,   11,   11,   11, 0x08,
     286,   11,   11,   11, 0x08,
     300,   11,   11,   11, 0x08,
     314,   11,   11,   11, 0x08,
     335,   11,   11,   11, 0x08,
     358,   11,   11,   11, 0x08,
     379,   11,   11,   11, 0x08,
     400,   11,   11,   11, 0x08,
     421,   11,   11,   11, 0x08,
     442,   11,   11,   11, 0x08,
     463,   11,   11,   11, 0x08,
     484,   11,   11,   11, 0x08,
     505,   11,   11,   11, 0x08,
     526,   11,   11,   11, 0x08,
     547,   11,   11,   11, 0x08,
     577,  568,   11,   11, 0x08,
     601,  568,   11,   11, 0x08,
     626,   11,   11,   11, 0x08,
     646,   11,   11,   11, 0x08,
     666,  568,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0apply_changes()\0open()\0"
    "save()\0save_as()\0exit()\0preferences()\0"
    "start()\0stop()\0pause()\0specify_control_source()\0"
    "select_position_strategy_type()\0"
    "select_move_strategy_type()\0"
    "select_shoot_strategy_type()\0value\0"
    "speed_control(int)\0match_time_control()\0"
    "excute_strategy()\0add_score()\0"
    "minus_score()\0reset_score()\0"
    "select_up_side(bool)\0select_down_side(bool)\0"
    "brobot5_changed(int)\0brobot4_changed(int)\0"
    "brobot3_changed(int)\0brobot2_changed(int)\0"
    "brobot1_changed(int)\0yrobot5_changed(int)\0"
    "yrobot4_changed(int)\0yrobot3_changed(int)\0"
    "yrobot2_changed(int)\0yrobot1_changed(int)\0"
    "selected\0run_mode_selected(bool)\0"
    "test_mode_selected(bool)\0strategy1_clicked()\0"
    "strategy2_clicked()\0start_referee(bool)\0"
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MainWindow *_t = static_cast<MainWindow *>(_o);
        switch (_id) {
        case 0: _t->apply_changes(); break;
        case 1: _t->open(); break;
        case 2: _t->save(); break;
        case 3: _t->save_as(); break;
        case 4: _t->exit(); break;
        case 5: _t->preferences(); break;
        case 6: _t->start(); break;
        case 7: _t->stop(); break;
        case 8: _t->pause(); break;
        case 9: _t->specify_control_source(); break;
        case 10: _t->select_position_strategy_type(); break;
        case 11: _t->select_move_strategy_type(); break;
        case 12: _t->select_shoot_strategy_type(); break;
        case 13: _t->speed_control((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 14: _t->match_time_control(); break;
        case 15: _t->excute_strategy(); break;
        case 16: _t->add_score(); break;
        case 17: _t->minus_score(); break;
        case 18: _t->reset_score(); break;
        case 19: _t->select_up_side((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 20: _t->select_down_side((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 21: _t->brobot5_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 22: _t->brobot4_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 23: _t->brobot3_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 24: _t->brobot2_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 25: _t->brobot1_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 26: _t->yrobot5_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 27: _t->yrobot4_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 28: _t->yrobot3_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 29: _t->yrobot2_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 30: _t->yrobot1_changed((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 31: _t->run_mode_selected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 32: _t->test_mode_selected((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 33: _t->strategy1_clicked(); break;
        case 34: _t->strategy2_clicked(); break;
        case 35: _t->start_referee((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MainWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, &staticMetaObjectExtraData }
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
        if (_id < 36)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 36;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
