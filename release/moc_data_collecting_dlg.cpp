/****************************************************************************
** Meta object code from reading C++ file 'data_collecting_dlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/data_collecting_dlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'data_collecting_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_CollectingDlg_t {
    QByteArrayData data[14];
    char stringdata0[142];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_CollectingDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_CollectingDlg_t qt_meta_stringdata_CollectingDlg = {
    {
QT_MOC_LITERAL(0, 0, 13), // "CollectingDlg"
QT_MOC_LITERAL(1, 14, 19), // "updatePlainTextEdit"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 13), // "dataCollected"
QT_MOC_LITERAL(4, 49, 5), // "fcube"
QT_MOC_LITERAL(5, 55, 8), // "_samples"
QT_MOC_LITERAL(6, 64, 4), // "fmat"
QT_MOC_LITERAL(7, 69, 7), // "_thetas"
QT_MOC_LITERAL(8, 77, 5), // "Hzmat"
QT_MOC_LITERAL(9, 83, 6), // "_freqs"
QT_MOC_LITERAL(10, 90, 27), // "samplesFileLocationReceived"
QT_MOC_LITERAL(11, 118, 4), // "path"
QT_MOC_LITERAL(12, 123, 7), // "newFile"
QT_MOC_LITERAL(13, 131, 10) // "importFile"

    },
    "CollectingDlg\0updatePlainTextEdit\0\0"
    "dataCollected\0fcube\0_samples\0fmat\0"
    "_thetas\0Hzmat\0_freqs\0samplesFileLocationReceived\0"
    "path\0newFile\0importFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CollectingDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       3,    3,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   49,    2, 0x0a /* Public */,
      12,    0,   52,    2, 0x08 /* Private */,
      13,    0,   53,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 4, 0x80000000 | 6, 0x80000000 | 8,    5,    7,    9,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void CollectingDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        CollectingDlg *_t = static_cast<CollectingDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->updatePlainTextEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->dataCollected((*reinterpret_cast< fcube(*)>(_a[1])),(*reinterpret_cast< fmat(*)>(_a[2])),(*reinterpret_cast< Hzmat(*)>(_a[3]))); break;
        case 2: _t->samplesFileLocationReceived((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->newFile(); break;
        case 4: _t->importFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (CollectingDlg::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CollectingDlg::updatePlainTextEdit)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (CollectingDlg::*)(fcube , fmat , Hzmat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&CollectingDlg::dataCollected)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject CollectingDlg::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_CollectingDlg.data,
      qt_meta_data_CollectingDlg,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *CollectingDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CollectingDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_CollectingDlg.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int CollectingDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
void CollectingDlg::updatePlainTextEdit(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void CollectingDlg::dataCollected(fcube _t1, fmat _t2, Hzmat _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
