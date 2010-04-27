/****************************************************************************
** Meta object code from reading C++ file 'boto.h'
**
** Created: Tue Apr 27 18:10:03 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "boto.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'boto.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_boto[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
       6,    5,    5,    5, 0x0a,
      16,    5,    5,    5, 0x0a,
      26,    5,    5,    5, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_boto[] = {
    "boto\0\0setR(int)\0setG(int)\0setB(int)\0"
};

const QMetaObject boto::staticMetaObject = {
    { &QPushButton::staticMetaObject, qt_meta_stringdata_boto,
      qt_meta_data_boto, 0 }
};

const QMetaObject *boto::metaObject() const
{
    return &staticMetaObject;
}

void *boto::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_boto))
        return static_cast<void*>(const_cast< boto*>(this));
    return QPushButton::qt_metacast(_clname);
}

int boto::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: setR((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: setG((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: setB((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
