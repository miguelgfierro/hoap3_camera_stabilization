/****************************************************************************
** Meta object code from reading C++ file 'maingui.h'
**
** Created: Fri Sep 17 19:03:16 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "maingui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maingui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainGui[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       9,    8,    8,    8, 0x08,
      28,    8,   24,    8, 0x08,
      48,    8,   24,    8, 0x08,
      69,    8,   24,    8, 0x08,
      89,    8,   24,    8, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainGui[] = {
    "MainGui\0\0timer_update()\0int\0"
    "updatescreen_left()\0updatescreen_right()\0"
    "render_screenleft()\0render_screenright()\0"
};

const QMetaObject MainGui::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainGui,
      qt_meta_data_MainGui, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainGui::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainGui::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainGui::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainGui))
        return static_cast<void*>(const_cast< MainGui*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainGui::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: timer_update(); break;
        case 1: { int _r = updatescreen_left();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 2: { int _r = updatescreen_right();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 3: { int _r = render_screenleft();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        case 4: { int _r = render_screenright();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = _r; }  break;
        default: ;
        }
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
