/****************************************************************************
** Meta object code from reading C++ file 'replay.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/replay.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'replay.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Serializer_t {
    QByteArrayData data[17];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Serializer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Serializer_t qt_meta_stringdata_Serializer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "Serializer"
QT_MOC_LITERAL(1, 11, 18), // "replayFlagsChanged"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 11), // "ReplayFlags"
QT_MOC_LITERAL(4, 43, 16), // "gameStateChanged"
QT_MOC_LITERAL(5, 60, 9), // "GameState"
QT_MOC_LITERAL(6, 70, 18), // "onGameStateChanged"
QT_MOC_LITERAL(7, 89, 18), // "toggleReplayPaused"
QT_MOC_LITERAL(8, 108, 15), // "toggleReplayDir"
QT_MOC_LITERAL(9, 124, 16), // "toggleReplayMode"
QT_MOC_LITERAL(10, 141, 14), // "replayStepNext"
QT_MOC_LITERAL(11, 156, 14), // "replayStepBack"
QT_MOC_LITERAL(12, 171, 17), // "replayJumpToStart"
QT_MOC_LITERAL(13, 189, 15), // "replayJumpToEnd"
QT_MOC_LITERAL(14, 205, 11), // "onSerialize"
QT_MOC_LITERAL(15, 217, 13), // "onDeserialize"
QT_MOC_LITERAL(16, 231, 13) // "onStepTimeout"

    },
    "Serializer\0replayFlagsChanged\0\0"
    "ReplayFlags\0gameStateChanged\0GameState\0"
    "onGameStateChanged\0toggleReplayPaused\0"
    "toggleReplayDir\0toggleReplayMode\0"
    "replayStepNext\0replayStepBack\0"
    "replayJumpToStart\0replayJumpToEnd\0"
    "onSerialize\0onDeserialize\0onStepTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Serializer[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    1,   85,    2, 0x0a /* Public */,
       7,    0,   88,    2, 0x0a /* Public */,
       8,    0,   89,    2, 0x0a /* Public */,
       9,    0,   90,    2, 0x0a /* Public */,
      10,    0,   91,    2, 0x0a /* Public */,
      11,    0,   92,    2, 0x0a /* Public */,
      12,    0,   93,    2, 0x0a /* Public */,
      13,    0,   94,    2, 0x0a /* Public */,
      14,    0,   95,    2, 0x08 /* Private */,
      15,    0,   96,    2, 0x08 /* Private */,
      16,    0,   97,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    2,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void Serializer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Serializer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->replayFlagsChanged((*reinterpret_cast< ReplayFlags(*)>(_a[1]))); break;
        case 1: _t->gameStateChanged((*reinterpret_cast< GameState(*)>(_a[1]))); break;
        case 2: _t->onGameStateChanged((*reinterpret_cast< GameState(*)>(_a[1]))); break;
        case 3: _t->toggleReplayPaused(); break;
        case 4: _t->toggleReplayDir(); break;
        case 5: _t->toggleReplayMode(); break;
        case 6: _t->replayStepNext(); break;
        case 7: _t->replayStepBack(); break;
        case 8: _t->replayJumpToStart(); break;
        case 9: _t->replayJumpToEnd(); break;
        case 10: _t->onSerialize(); break;
        case 11: _t->onDeserialize(); break;
        case 12: _t->onStepTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Serializer::*)(ReplayFlags );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Serializer::replayFlagsChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Serializer::*)(GameState );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Serializer::gameStateChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Serializer::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_Serializer.data,
    qt_meta_data_Serializer,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Serializer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Serializer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Serializer.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Serializer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void Serializer::replayFlagsChanged(ReplayFlags _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Serializer::gameStateChanged(GameState _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
