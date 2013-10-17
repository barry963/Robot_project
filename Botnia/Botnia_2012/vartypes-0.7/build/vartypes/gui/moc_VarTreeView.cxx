/****************************************************************************
** Meta object code from reading C++ file 'VarTreeView.h'
**
** Created: Thu Nov 8 16:48:59 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../vartypes/gui/VarTreeView.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VarTreeView.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_VarTypes__VarTreeView[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      40,   23,   22,   22, 0x09,
      99,   79,   22,   22, 0x09,
     146,  141,   22,   22, 0x0a,
     162,   22,   22,   22, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_VarTypes__VarTreeView[] = {
    "VarTypes::VarTreeView\0\0parent,start,end\0"
    "newItemChecksRows(QModelIndex,int,int)\0"
    "topLeft,bottomRight\0"
    "checkDataChanged(QModelIndex,QModelIndex)\0"
    "text\0search(QString)\0nextSearchResult()\0"
};

const QMetaObject VarTypes::VarTreeView::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_VarTypes__VarTreeView,
      qt_meta_data_VarTypes__VarTreeView, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &VarTypes::VarTreeView::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *VarTypes::VarTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *VarTypes::VarTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_VarTypes__VarTreeView))
        return static_cast<void*>(const_cast< VarTreeView*>(this));
    return QWidget::qt_metacast(_clname);
}

int VarTypes::VarTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: newItemChecksRows((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 1: checkDataChanged((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< const QModelIndex(*)>(_a[2]))); break;
        case 2: search((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: nextSearchResult(); break;
        default: ;
        }
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
