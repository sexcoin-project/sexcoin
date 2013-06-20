/****************************************************************************
** Meta object code from reading C++ file 'miningpage.h'
**
** Created: Sat May 11 23:32:42 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/qt/miningpage.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'miningpage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MiningPage[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      12,   11,   11,   11, 0x0a,
      27,   11,   11,   11, 0x0a,
      45,   11,   11,   11, 0x0a,
      62,   11,   11,   11, 0x0a,
      76,   11,   11,   11, 0x0a,
      91,   11,   11,   11, 0x0a,
     109,  106,   11,   11, 0x0a,
     143,   11,   11,   11, 0x0a,
     158,   11,   11,   11, 0x0a,
     193,   11,   11,   11, 0x0a,
     209,   11,   11,   11, 0x0a,
     237,   11,  229,   11, 0x0a,
     261,  254,   11,   11, 0x0a,
     288,  254,   11,   11, 0x0a,
     343,   11,  319,   11, 0x0a,
     365,  359,   11,   11, 0x0a,
     390,  382,   11,   11, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_MiningPage[] = {
    "MiningPage\0\0startPressed()\0startPoolMining()\0"
    "stopPoolMining()\0updateSpeed()\0"
    "loadSettings()\0saveSettings()\0,,\0"
    "reportToList(QString,int,QString)\0"
    "minerStarted()\0minerError(QProcess::ProcessError)\0"
    "minerFinished()\0readProcessOutput()\0"
    "QString\0getTime(QString)\0enable\0"
    "enableMiningControls(bool)\0"
    "enablePoolMiningControls(bool)\0"
    "ClientModel::MiningType\0getMiningType()\0"
    "index\0typeChanged(int)\0checked\0"
    "debugToggled(bool)\0"
};

void MiningPage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        MiningPage *_t = static_cast<MiningPage *>(_o);
        switch (_id) {
        case 0: _t->startPressed(); break;
        case 1: _t->startPoolMining(); break;
        case 2: _t->stopPoolMining(); break;
        case 3: _t->updateSpeed(); break;
        case 4: _t->loadSettings(); break;
        case 5: _t->saveSettings(); break;
        case 6: _t->reportToList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 7: _t->minerStarted(); break;
        case 8: _t->minerError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        case 9: _t->minerFinished(); break;
        case 10: _t->readProcessOutput(); break;
        case 11: { QString _r = _t->getTime((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = _r; }  break;
        case 12: _t->enableMiningControls((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->enablePoolMiningControls((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: { ClientModel::MiningType _r = _t->getMiningType();
            if (_a[0]) *reinterpret_cast< ClientModel::MiningType*>(_a[0]) = _r; }  break;
        case 15: _t->typeChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 16: _t->debugToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData MiningPage::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject MiningPage::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MiningPage,
      qt_meta_data_MiningPage, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MiningPage::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MiningPage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MiningPage::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MiningPage))
        return static_cast<void*>(const_cast< MiningPage*>(this));
    return QWidget::qt_metacast(_clname);
}

int MiningPage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
