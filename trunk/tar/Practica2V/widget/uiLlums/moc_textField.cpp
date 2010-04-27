/****************************************************************************
** Meta object code from reading C++ file 'textField.h'
**
** Created: Tue Apr 27 18:10:04 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "textField.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'textField.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_textField[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      11,   10,   10,   10, 0x05,

 // slots: signature, parameters, type, tag, flags
      31,   10,   10,   10, 0x0a,
      47,   10,   10,   10, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_textField[] = {
    "textField\0\0valueChanged(float)\0"
    "setFloat(float)\0text(QString)\0"
};

const QMetaObject textField::staticMetaObject = {
    { &QLineEdit::staticMetaObject, qt_meta_stringdata_textField,
      qt_meta_data_textField, 0 }
};

const QMetaObject *textField::metaObject() const
{
    return &staticMetaObject;
}

void *textField::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_textField))
        return static_cast<void*>(const_cast< textField*>(this));
    return QLineEdit::qt_metacast(_clname);
}

int textField::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLineEdit::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: valueChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 1: setFloat((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 2: text((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void textField::valueChanged(float _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
