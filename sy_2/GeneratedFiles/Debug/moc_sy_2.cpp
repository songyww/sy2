/****************************************************************************
** Meta object code from reading C++ file 'sy_2.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../sy_2.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sy_2.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_sy_2[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      18,    5,    5,    5, 0x0a,
      43,   30,    5,    5, 0x0a,
      67,    5,    5,    5, 0x0a,
      88,    5,    5,    5, 0x0a,
     106,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_sy_2[] = {
    "sy_2\0\0OpenFile1()\0OpenFile2()\0"
    "DstImagePath\0OpenResultFile1(string)\0"
    "BigMapRegistration()\0OnClearMapLayer()\0"
    "AdBigMapRegistration()\0"
};

void sy_2::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        sy_2 *_t = static_cast<sy_2 *>(_o);
        switch (_id) {
        case 0: _t->OpenFile1(); break;
        case 1: _t->OpenFile2(); break;
        case 2: _t->OpenResultFile1((*reinterpret_cast< string(*)>(_a[1]))); break;
        case 3: _t->BigMapRegistration(); break;
        case 4: _t->OnClearMapLayer(); break;
        case 5: _t->AdBigMapRegistration(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData sy_2::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject sy_2::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_sy_2,
      qt_meta_data_sy_2, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &sy_2::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *sy_2::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *sy_2::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_sy_2))
        return static_cast<void*>(const_cast< sy_2*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int sy_2::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
