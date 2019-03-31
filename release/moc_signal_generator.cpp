/****************************************************************************
** Meta object code from reading C++ file 'signal_generator.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/signal_generator.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signal_generator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CSignalGenerator_t {
    QByteArrayData data[13];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CSignalGenerator_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CSignalGenerator_t qt_meta_stringdata_CSignalGenerator = {
    {
QT_MOC_LITERAL(0, 0, 16), // "CSignalGenerator"
QT_MOC_LITERAL(1, 17, 7), // "updated"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 23), // "const CSignalGenerator*"
QT_MOC_LITERAL(4, 50, 13), // "SetSignalRFOn"
QT_MOC_LITERAL(5, 64, 5), // "_RFOn"
QT_MOC_LITERAL(6, 70, 14), // "SetSignalModOn"
QT_MOC_LITERAL(7, 85, 6), // "_ModOn"
QT_MOC_LITERAL(8, 92, 15), // "SetSignalGenPow"
QT_MOC_LITERAL(9, 108, 6), // "PowdBm"
QT_MOC_LITERAL(10, 115, 16), // "SetSignalGenFreq"
QT_MOC_LITERAL(11, 132, 9), // "Frequency"
QT_MOC_LITERAL(12, 142, 1) // "f"

    },
    "CSignalGenerator\0updated\0\0"
    "const CSignalGenerator*\0SetSignalRFOn\0"
    "_RFOn\0SetSignalModOn\0_ModOn\0SetSignalGenPow\0"
    "PowdBm\0SetSignalGenFreq\0Frequency\0f"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CSignalGenerator[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   42,    2, 0x0a /* Public */,
       6,    1,   45,    2, 0x0a /* Public */,
       8,    1,   48,    2, 0x0a /* Public */,
      10,    1,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Bool, QMetaType::Bool,    5,
    QMetaType::Bool, QMetaType::Bool,    7,
    QMetaType::Bool, QMetaType::Double,    9,
    QMetaType::Bool, 0x80000000 | 11,   12,

       0        // eod
};

void CSignalGenerator::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CSignalGenerator *_t = static_cast<CSignalGenerator *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updated((*reinterpret_cast< const CSignalGenerator*(*)>(_a[1]))); break;
        case 1: { bool _r = _t->SetSignalRFOn((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 2: { bool _r = _t->SetSignalModOn((*reinterpret_cast< bool(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: { bool _r = _t->SetSignalGenPow((*reinterpret_cast< double(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 4: { bool _r = _t->SetSignalGenFreq((*reinterpret_cast< Frequency(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CSignalGenerator::*)(const CSignalGenerator * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CSignalGenerator::updated)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CSignalGenerator::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CSignalGenerator.data,
      qt_meta_data_CSignalGenerator,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CSignalGenerator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CSignalGenerator::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CSignalGenerator.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CSignalGenerator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void CSignalGenerator::updated(const CSignalGenerator * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
