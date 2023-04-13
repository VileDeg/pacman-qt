/****************************************************************************
** Meta object code from reading C++ file 'ui.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/ui.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_WindowUI_t {
    QByteArrayData data[12];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_WindowUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_WindowUI_t qt_meta_stringdata_WindowUI = {
    {
QT_MOC_LITERAL(0, 0, 8), // "WindowUI"
QT_MOC_LITERAL(1, 9, 22), // "onloadMapMenuTriggered"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "QAction*"
QT_MOC_LITERAL(4, 42, 6), // "action"
QT_MOC_LITERAL(5, 49, 28), // "onloadRecordingMenuTriggered"
QT_MOC_LITERAL(6, 78, 28), // "onReplayFromStartButtonClick"
QT_MOC_LITERAL(7, 107, 26), // "onReplayFromEndButtonClick"
QT_MOC_LITERAL(8, 134, 20), // "onReplayFlagsChanged"
QT_MOC_LITERAL(9, 155, 11), // "ReplayFlags"
QT_MOC_LITERAL(10, 167, 18), // "onGameStateChanged"
QT_MOC_LITERAL(11, 186, 9) // "GameState"

    },
    "WindowUI\0onloadMapMenuTriggered\0\0"
    "QAction*\0action\0onloadRecordingMenuTriggered\0"
    "onReplayFromStartButtonClick\0"
    "onReplayFromEndButtonClick\0"
    "onReplayFlagsChanged\0ReplayFlags\0"
    "onGameStateChanged\0GameState"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_WindowUI[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x08 /* Private */,
       5,    1,   47,    2, 0x08 /* Private */,
       6,    0,   50,    2, 0x08 /* Private */,
       7,    0,   51,    2, 0x08 /* Private */,
       8,    1,   52,    2, 0x08 /* Private */,
      10,    1,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9,    2,
    QMetaType::Void, 0x80000000 | 11,    2,

       0        // eod
};

void WindowUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<WindowUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onloadMapMenuTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 1: _t->onloadRecordingMenuTriggered((*reinterpret_cast< QAction*(*)>(_a[1]))); break;
        case 2: _t->onReplayFromStartButtonClick(); break;
        case 3: _t->onReplayFromEndButtonClick(); break;
        case 4: _t->onReplayFlagsChanged((*reinterpret_cast< ReplayFlags(*)>(_a[1]))); break;
        case 5: _t->onGameStateChanged((*reinterpret_cast< GameState(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        case 1:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QAction* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject WindowUI::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_WindowUI.data,
    qt_meta_data_WindowUI,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *WindowUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *WindowUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_WindowUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int WindowUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
