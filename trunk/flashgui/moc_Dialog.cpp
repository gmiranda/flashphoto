/****************************************************************************
** Meta object code from reading C++ file 'Dialog.h'
**
** Created: lun 4. sep 11:46:53 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.1.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../Dialog.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Dialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.1.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_FlashDialog[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x08,
      30,   12,   12,   12, 0x08,
      49,   12,   12,   12, 0x08,
      67,   12,   12,   12, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_FlashDialog[] = {
    "FlashDialog\0\0openFlashImage()\0openNoFlashImage()\0saveResultImage()\0"
    "launchFlashExec()\0"
};

const QMetaObject FlashDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_FlashDialog,
      qt_meta_data_FlashDialog, 0 }
};

const QMetaObject *FlashDialog::metaObject() const
{
    return &staticMetaObject;
}

void *FlashDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_FlashDialog))
	return static_cast<void*>(const_cast<FlashDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int FlashDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: openFlashImage(); break;
        case 1: openNoFlashImage(); break;
        case 2: saveResultImage(); break;
        case 3: launchFlashExec(); break;
        }
        _id -= 4;
    }
    return _id;
}
