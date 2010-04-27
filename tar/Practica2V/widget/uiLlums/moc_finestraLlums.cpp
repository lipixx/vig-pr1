/****************************************************************************
** Meta object code from reading C++ file 'finestraLlums.h'
**
** Created: Tue Apr 27 18:10:02 2010
**      by: The Qt Meta Object Compiler version 59 (Qt 4.4.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "finestraLlums.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'finestraLlums.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.4.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_finestraLlums[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
      20,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // signals: signature, parameters, type, tag, flags
      15,   14,   14,   14, 0x05,

 // slots: signature, parameters, type, tag, flags
      48,   37,   14,   14, 0x0a,
      68,   14,   14,   14, 0x0a,
      83,   14,   14,   14, 0x0a,
     100,   14,   14,   14, 0x0a,
     116,   14,   14,   14, 0x0a,
     136,  134,   14,   14, 0x0a,
     154,  134,   14,   14, 0x0a,
     174,  172,   14,   14, 0x0a,
     196,  194,   14,   14, 0x0a,
     218,  216,   14,   14, 0x0a,
     240,  238,   14,   14, 0x0a,
     255,  172,   14,   14, 0x0a,
     276,  194,   14,   14, 0x0a,
     297,  216,   14,   14, 0x0a,
     320,  318,   14,   14, 0x0a,
     339,  335,   14,   14, 0x0a,
     357,  238,   14,   14, 0x0a,
     379,  318,   14,   14, 0x0a,
     405,  238,   14,   14, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_finestraLlums[] = {
    "finestraLlums\0\0llumModificada(llum*)\0"
    "focus,size\0iniciar(llum**,int)\0"
    "carregarLlum()\0ambientPressed()\0"
    "difusePressed()\0specularPressed()\0b\0"
    "activarSpot(bool)\0activarLlum(bool)\0"
    "x\0coorXChanged(float)\0y\0coorYChanged(float)\0"
    "z\0coorZChanged(float)\0i\0puntVista(int)\0"
    "direccioXSpot(float)\0direccioYSpot(float)\0"
    "direccioZSpot(float)\0a\0angleSpot(int)\0"
    "exp\0exponentSpot(int)\0canviarAtenuacio(int)\0"
    "modificarAtenuacio(float)\0canviarLlum(int)\0"
};

const QMetaObject finestraLlums::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_finestraLlums,
      qt_meta_data_finestraLlums, 0 }
};

const QMetaObject *finestraLlums::metaObject() const
{
    return &staticMetaObject;
}

void *finestraLlums::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_finestraLlums))
        return static_cast<void*>(const_cast< finestraLlums*>(this));
    return QDialog::qt_metacast(_clname);
}

int finestraLlums::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: llumModificada((*reinterpret_cast< llum*(*)>(_a[1]))); break;
        case 1: iniciar((*reinterpret_cast< llum**(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: carregarLlum(); break;
        case 3: ambientPressed(); break;
        case 4: difusePressed(); break;
        case 5: specularPressed(); break;
        case 6: activarSpot((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: activarLlum((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: coorXChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 9: coorYChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 10: coorZChanged((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 11: puntVista((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 12: direccioXSpot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 13: direccioYSpot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 14: direccioZSpot((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 15: angleSpot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: exponentSpot((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 17: canviarAtenuacio((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 18: modificarAtenuacio((*reinterpret_cast< float(*)>(_a[1]))); break;
        case 19: canviarLlum((*reinterpret_cast< int(*)>(_a[1]))); break;
        }
        _id -= 20;
    }
    return _id;
}

// SIGNAL 0
void finestraLlums::llumModificada(llum * _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
