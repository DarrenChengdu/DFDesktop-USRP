/****************************************************************************
** Meta object code from reading C++ file 'tablesource.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/tablesource.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tablesource.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TableSource_t {
    QByteArrayData data[12];
    char stringdata0[135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TableSource_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TableSource_t qt_meta_stringdata_TableSource = {
    {
QT_MOC_LITERAL(0, 0, 11), // "TableSource"
QT_MOC_LITERAL(1, 12, 12), // "tableChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 2), // "Hz"
QT_MOC_LITERAL(4, 29, 13), // "setFreqObserv"
QT_MOC_LITERAL(5, 43, 22), // "setSamplingDataEnabled"
QT_MOC_LITERAL(6, 66, 15), // "setSamplingData"
QT_MOC_LITERAL(7, 82, 14), // "QVector<float>"
QT_MOC_LITERAL(8, 97, 13), // "setCALEnabled"
QT_MOC_LITERAL(9, 111, 3), // "sta"
QT_MOC_LITERAL(10, 115, 15), // "setAzimuthIndex"
QT_MOC_LITERAL(11, 131, 3) // "ind"

    },
    "TableSource\0tableChanged\0\0Hz\0setFreqObserv\0"
    "setSamplingDataEnabled\0setSamplingData\0"
    "QVector<float>\0setCALEnabled\0sta\0"
    "setAzimuthIndex\0ind"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TableSource[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   49,    2, 0x0a /* Public */,
       5,    1,   52,    2, 0x0a /* Public */,
       6,    1,   55,    2, 0x0a /* Public */,
       8,    1,   58,    2, 0x0a /* Public */,
      10,    1,   61,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Int,    2,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, 0x80000000 | 7,    2,
    QMetaType::Void, QMetaType::Bool,    9,
    QMetaType::Void, QMetaType::Int,   11,

       0        // eod
};

void TableSource::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        TableSource *_t = static_cast<TableSource *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->tableChanged((*reinterpret_cast< Hz(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 1: _t->setFreqObserv((*reinterpret_cast< Hz(*)>(_a[1]))); break;
        case 2: _t->setSamplingDataEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setSamplingData((*reinterpret_cast< QVector<float>(*)>(_a[1]))); break;
        case 4: _t->setCALEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 5: _t->setAzimuthIndex((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<float> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TableSource::*)(Hz , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TableSource::tableChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TableSource::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_TableSource.data,
      qt_meta_data_TableSource,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *TableSource::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TableSource::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TableSource.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int TableSource::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void TableSource::tableChanged(Hz _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
