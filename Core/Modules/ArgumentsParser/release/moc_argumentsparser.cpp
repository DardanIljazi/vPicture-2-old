/****************************************************************************
** Meta object code from reading C++ file 'argumentsparser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../argumentsparser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'argumentsparser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ArgumentsParser_t {
    QByteArrayData data[15];
    char stringdata[212];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_ArgumentsParser_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_ArgumentsParser_t qt_meta_stringdata_ArgumentsParser = {
    {
QT_MOC_LITERAL(0, 0, 15),
QT_MOC_LITERAL(1, 16, 11),
QT_MOC_LITERAL(2, 28, 0),
QT_MOC_LITERAL(3, 29, 11),
QT_MOC_LITERAL(4, 41, 13),
QT_MOC_LITERAL(5, 55, 11),
QT_MOC_LITERAL(6, 67, 4),
QT_MOC_LITERAL(7, 72, 8),
QT_MOC_LITERAL(8, 81, 10),
QT_MOC_LITERAL(9, 92, 15),
QT_MOC_LITERAL(10, 108, 14),
QT_MOC_LITERAL(11, 123, 25),
QT_MOC_LITERAL(12, 149, 23),
QT_MOC_LITERAL(13, 173, 22),
QT_MOC_LITERAL(14, 196, 14)
    },
    "ArgumentsParser\0hideWindows\0\0showWindows\0"
    "noEnumWindows\0connectUser\0mail\0password\0"
    "noKeyEvent\0isWindowsHidden\0isWindowsShown\0"
    "isEnumWindowsFunctionUsed\0"
    "isConnectUserOptionUsed\0isKeyEventFunctionUsed\0"
    "parseArguments\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ArgumentsParser[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x06,
       3,    0,   70,    2, 0x06,
       4,    0,   71,    2, 0x06,
       5,    2,   72,    2, 0x06,
       8,    0,   77,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       9,    0,   78,    2, 0x0a,
      10,    0,   79,    2, 0x0a,
      11,    0,   80,    2, 0x0a,
      12,    0,   81,    2, 0x0a,
      13,    0,   82,    2, 0x0a,
      14,    0,   83,    2, 0x08,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,
    QMetaType::Bool,

       0        // eod
};

void ArgumentsParser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ArgumentsParser *_t = static_cast<ArgumentsParser *>(_o);
        switch (_id) {
        case 0: _t->hideWindows(); break;
        case 1: _t->showWindows(); break;
        case 2: _t->noEnumWindows(); break;
        case 3: _t->connectUser((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->noKeyEvent(); break;
        case 5: { bool _r = _t->isWindowsHidden();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 6: { bool _r = _t->isWindowsShown();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 7: { bool _r = _t->isEnumWindowsFunctionUsed();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 8: { bool _r = _t->isConnectUserOptionUsed();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 9: { bool _r = _t->isKeyEventFunctionUsed();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 10: { bool _r = _t->parseArguments();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (ArgumentsParser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArgumentsParser::hideWindows)) {
                *result = 0;
            }
        }
        {
            typedef void (ArgumentsParser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArgumentsParser::showWindows)) {
                *result = 1;
            }
        }
        {
            typedef void (ArgumentsParser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArgumentsParser::noEnumWindows)) {
                *result = 2;
            }
        }
        {
            typedef void (ArgumentsParser::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArgumentsParser::connectUser)) {
                *result = 3;
            }
        }
        {
            typedef void (ArgumentsParser::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&ArgumentsParser::noKeyEvent)) {
                *result = 4;
            }
        }
    }
}

const QMetaObject ArgumentsParser::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_ArgumentsParser.data,
      qt_meta_data_ArgumentsParser,  qt_static_metacall, 0, 0}
};


const QMetaObject *ArgumentsParser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ArgumentsParser::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ArgumentsParser.stringdata))
        return static_cast<void*>(const_cast< ArgumentsParser*>(this));
    return QObject::qt_metacast(_clname);
}

int ArgumentsParser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void ArgumentsParser::hideWindows()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void ArgumentsParser::showWindows()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void ArgumentsParser::noEnumWindows()
{
    QMetaObject::activate(this, &staticMetaObject, 2, 0);
}

// SIGNAL 3
void ArgumentsParser::connectUser(QString _t1, QString _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void ArgumentsParser::noKeyEvent()
{
    QMetaObject::activate(this, &staticMetaObject, 4, 0);
}
QT_END_MOC_NAMESPACE
