/****************************************************************************
** Meta object code from reading C++ file 'signalgen_page.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/widgets/signalgen_page.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'signalgen_page.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SignalGenPage_t {
    QByteArrayData data[9];
    char stringdata0[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SignalGenPage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SignalGenPage_t qt_meta_stringdata_SignalGenPage = {
    {
QT_MOC_LITERAL(0, 0, 13), // "SignalGenPage"
QT_MOC_LITERAL(1, 14, 15), // "setGenFrequency"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 9), // "Frequency"
QT_MOC_LITERAL(4, 41, 1), // "f"
QT_MOC_LITERAL(5, 43, 19), // "onConnectionClicked"
QT_MOC_LITERAL(6, 63, 22), // "onDisconnectionClicked"
QT_MOC_LITERAL(7, 86, 10), // "updatePage"
QT_MOC_LITERAL(8, 97, 23) // "const CSignalGenerator*"

    },
    "SignalGenPage\0setGenFrequency\0\0Frequency\0"
    "f\0onConnectionClicked\0onDisconnectionClicked\0"
    "updatePage\0const CSignalGenerator*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SignalGenPage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x0a /* Public */,
       5,    0,   37,    2, 0x08 /* Private */,
       6,    0,   38,    2, 0x08 /* Private */,
       7,    1,   39,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,

       0        // eod
};

void SignalGenPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        SignalGenPage *_t = static_cast<SignalGenPage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setGenFrequency((*reinterpret_cast< Frequency(*)>(_a[1]))); break;
        case 1: _t->onConnectionClicked(); break;
        case 2: _t->onDisconnectionClicked(); break;
        case 3: _t->updatePage((*reinterpret_cast< const CSignalGenerator*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SignalGenPage::staticMetaObject = {
    { &DockPage::staticMetaObject, qt_meta_stringdata_SignalGenPage.data,
      qt_meta_data_SignalGenPage,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *SignalGenPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SignalGenPage::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SignalGenPage.stringdata0))
        return static_cast<void*>(this);
    return DockPage::qt_metacast(_clname);
}

int SignalGenPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = DockPage::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
