/****************************************************************************
** Meta object code from reading C++ file 'sweep_central.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/sweep_central.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sweep_central.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SweepCentral_t {
    QByteArrayData data[12];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SweepCentral_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SweepCentral_t qt_meta_stringdata_SweepCentral = {
    {
QT_MOC_LITERAL(0, 0, 12), // "SweepCentral"
QT_MOC_LITERAL(1, 13, 10), // "doaResults"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 16), // "QVector<QPointF>"
QT_MOC_LITERAL(4, 42, 17), // "plotsAxisYUpdated"
QT_MOC_LITERAL(5, 60, 3), // "mat"
QT_MOC_LITERAL(6, 64, 17), // "plotsAxisXUpdated"
QT_MOC_LITERAL(7, 82, 4), // "xmin"
QT_MOC_LITERAL(8, 87, 4), // "xmax"
QT_MOC_LITERAL(9, 92, 15), // "settingsChanged"
QT_MOC_LITERAL(10, 108, 17), // "const DFSettings*"
QT_MOC_LITERAL(11, 126, 1) // "s"

    },
    "SweepCentral\0doaResults\0\0QVector<QPointF>\0"
    "plotsAxisYUpdated\0mat\0plotsAxisXUpdated\0"
    "xmin\0xmax\0settingsChanged\0const DFSettings*\0"
    "s"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SweepCentral[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   34,    2, 0x06 /* Public */,
       4,    1,   39,    2, 0x06 /* Public */,
       6,    2,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    1,   47,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Float, 0x80000000 | 3,    2,    2,
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void, QMetaType::Double, QMetaType::Double,    7,    8,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 10,   11,

       0        // eod
};

void SweepCentral::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SweepCentral *_t = static_cast<SweepCentral *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->doaResults((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< QVector<QPointF>(*)>(_a[2]))); break;
        case 1: _t->plotsAxisYUpdated((*reinterpret_cast< mat(*)>(_a[1]))); break;
        case 2: _t->plotsAxisXUpdated((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2]))); break;
        case 3: _t->settingsChanged((*reinterpret_cast< const DFSettings*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QVector<QPointF> >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SweepCentral::*)(float , QVector<QPointF> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SweepCentral::doaResults)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SweepCentral::*)(mat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SweepCentral::plotsAxisYUpdated)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SweepCentral::*)(double , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SweepCentral::plotsAxisXUpdated)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SweepCentral::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SweepCentral.data,
      qt_meta_data_SweepCentral,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SweepCentral::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SweepCentral::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SweepCentral.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SweepCentral::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void SweepCentral::doaResults(float _t1, QVector<QPointF> _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void SweepCentral::plotsAxisYUpdated(mat _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void SweepCentral::plotsAxisXUpdated(double _t1, double _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
