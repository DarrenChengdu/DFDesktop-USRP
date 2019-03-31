/****************************************************************************
** Meta object code from reading C++ file 'simpletable.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/simpletable.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'simpletable.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SimpleTable_t {
    QByteArrayData data[9];
    char stringdata0[136];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SimpleTable_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SimpleTable_t qt_meta_stringdata_SimpleTable = {
    {
QT_MOC_LITERAL(0, 0, 11), // "SimpleTable"
QT_MOC_LITERAL(1, 12, 17), // "freqOffsetChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 18), // "dataCorrectedByCAL"
QT_MOC_LITERAL(4, 50, 16), // "azimIndexChanged"
QT_MOC_LITERAL(5, 67, 22), // "slotCurrentAzimChanged"
QT_MOC_LITERAL(6, 90, 15), // "setSamplingAzim"
QT_MOC_LITERAL(7, 106, 11), // "updateTable"
QT_MOC_LITERAL(8, 118, 17) // "const DFSettings*"

    },
    "SimpleTable\0freqOffsetChanged\0\0"
    "dataCorrectedByCAL\0azimIndexChanged\0"
    "slotCurrentAzimChanged\0setSamplingAzim\0"
    "updateTable\0const DFSettings*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SimpleTable[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,
       4,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   53,    2, 0x08 /* Private */,
       6,    1,   56,    2, 0x0a /* Public */,
       7,    1,   59,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::Int,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, 0x80000000 | 8,    2,

       0        // eod
};

void SimpleTable::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SimpleTable *_t = static_cast<SimpleTable *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->freqOffsetChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->dataCorrectedByCAL((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->azimIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->slotCurrentAzimChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->setSamplingAzim((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 5: _t->updateTable((*reinterpret_cast< const DFSettings*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SimpleTable::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleTable::freqOffsetChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SimpleTable::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleTable::dataCorrectedByCAL)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SimpleTable::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SimpleTable::azimIndexChanged)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SimpleTable::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_SimpleTable.data,
      qt_meta_data_SimpleTable,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SimpleTable::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SimpleTable::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SimpleTable.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int SimpleTable::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void SimpleTable::freqOffsetChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SimpleTable::dataCorrectedByCAL(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SimpleTable::azimIndexChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
