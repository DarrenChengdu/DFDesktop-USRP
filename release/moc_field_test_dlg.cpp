/****************************************************************************
** Meta object code from reading C++ file 'field_test_dlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.11.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/field_test_dlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QVector>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'field_test_dlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.11.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FieldTestDlg_t {
    QByteArrayData data[24];
    char stringdata0[269];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FieldTestDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FieldTestDlg_t qt_meta_stringdata_FieldTestDlg = {
    {
QT_MOC_LITERAL(0, 0, 12), // "FieldTestDlg"
QT_MOC_LITERAL(1, 13, 15), // "setSamplingAzim"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12), // "setDFEnabled"
QT_MOC_LITERAL(4, 43, 19), // "updatePlainTextEdit"
QT_MOC_LITERAL(5, 63, 13), // "dataCollected"
QT_MOC_LITERAL(6, 77, 5), // "fcube"
QT_MOC_LITERAL(7, 83, 8), // "_samples"
QT_MOC_LITERAL(8, 92, 4), // "fmat"
QT_MOC_LITERAL(9, 97, 7), // "_thetas"
QT_MOC_LITERAL(10, 105, 5), // "Hzmat"
QT_MOC_LITERAL(11, 111, 6), // "_freqs"
QT_MOC_LITERAL(12, 118, 27), // "samplesFileLocationReceived"
QT_MOC_LITERAL(13, 146, 11), // "QEventLoop*"
QT_MOC_LITERAL(14, 158, 2), // "el"
QT_MOC_LITERAL(15, 161, 4), // "path"
QT_MOC_LITERAL(16, 166, 20), // "onDOAResultsReceived"
QT_MOC_LITERAL(17, 187, 16), // "QVector<QPointF>"
QT_MOC_LITERAL(18, 204, 14), // "onTableChanged"
QT_MOC_LITERAL(19, 219, 2), // "Hz"
QT_MOC_LITERAL(20, 222, 9), // "updateDlg"
QT_MOC_LITERAL(21, 232, 17), // "const DFSettings*"
QT_MOC_LITERAL(22, 250, 7), // "newFile"
QT_MOC_LITERAL(23, 258, 10) // "importFile"

    },
    "FieldTestDlg\0setSamplingAzim\0\0"
    "setDFEnabled\0updatePlainTextEdit\0"
    "dataCollected\0fcube\0_samples\0fmat\0"
    "_thetas\0Hzmat\0_freqs\0samplesFileLocationReceived\0"
    "QEventLoop*\0el\0path\0onDOAResultsReceived\0"
    "QVector<QPointF>\0onTableChanged\0Hz\0"
    "updateDlg\0const DFSettings*\0newFile\0"
    "importFile"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FieldTestDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,
       4,    1,   70,    2, 0x06 /* Public */,
       5,    3,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      12,    2,   80,    2, 0x0a /* Public */,
      16,    2,   85,    2, 0x0a /* Public */,
      18,    2,   90,    2, 0x0a /* Public */,
      20,    1,   95,    2, 0x0a /* Public */,
      22,    0,   98,    2, 0x08 /* Private */,
      23,    0,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    2,
    QMetaType::Void, QMetaType::Bool,    2,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, 0x80000000 | 6, 0x80000000 | 8, 0x80000000 | 10,    7,    9,   11,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 13, QMetaType::QString,   14,   15,
    QMetaType::Void, QMetaType::Float, 0x80000000 | 17,    2,    2,
    QMetaType::Void, 0x80000000 | 19, QMetaType::Int,    2,    2,
    QMetaType::Void, 0x80000000 | 21,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FieldTestDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FieldTestDlg *_t = static_cast<FieldTestDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setSamplingAzim((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: _t->setDFEnabled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->updatePlainTextEdit((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->dataCollected((*reinterpret_cast< fcube(*)>(_a[1])),(*reinterpret_cast< fmat(*)>(_a[2])),(*reinterpret_cast< Hzmat(*)>(_a[3]))); break;
        case 4: _t->samplesFileLocationReceived((*reinterpret_cast< QEventLoop*(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 5: _t->onDOAResultsReceived((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< QVector<QPointF>(*)>(_a[2]))); break;
        case 6: _t->onTableChanged((*reinterpret_cast< Hz(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 7: _t->updateDlg((*reinterpret_cast< const DFSettings*(*)>(_a[1]))); break;
        case 8: _t->newFile(); break;
        case 9: _t->importFile(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 5:
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
            using _t = void (FieldTestDlg::*)(QStringList );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldTestDlg::setSamplingAzim)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FieldTestDlg::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldTestDlg::setDFEnabled)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FieldTestDlg::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldTestDlg::updatePlainTextEdit)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FieldTestDlg::*)(fcube , fmat , Hzmat );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FieldTestDlg::dataCollected)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FieldTestDlg::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FieldTestDlg.data,
      qt_meta_data_FieldTestDlg,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FieldTestDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FieldTestDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FieldTestDlg.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int FieldTestDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void FieldTestDlg::setSamplingAzim(QStringList _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FieldTestDlg::setDFEnabled(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FieldTestDlg::updatePlainTextEdit(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FieldTestDlg::dataCollected(fcube _t1, fmat _t2, Hzmat _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
