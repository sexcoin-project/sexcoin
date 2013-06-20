/****************************************************************************
** Meta object code from reading C++ file 'bitcoingui.h'
**
** Created: Sat May 11 23:32:22 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../src/qt/bitcoingui.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bitcoingui.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BitcoinGUI[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      28,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x0a,
      60,   41,   11,   11, 0x0a,
      98,   82,   11,   11, 0x0a,
     125,  118,   11,   11, 0x0a,
     170,  150,   11,   11, 0x0a,
     218,  198,   11,   11, 0x0a,
     246,  239,   11,   11, 0x0a,
     265,   11,   11,   11, 0x08,
     284,   11,   11,   11, 0x08,
     301,   11,   11,   11, 0x08,
     319,   11,   11,   11, 0x08,
     341,   11,   11,   11, 0x08,
     364,   11,   11,   11, 0x08,
     389,  384,   11,   11, 0x08,
     417,   11,   11,   11, 0x28,
     438,  384,   11,   11, 0x08,
     468,   11,   11,   11, 0x28,
     491,   11,   11,   11, 0x08,
     508,   11,   11,   11, 0x08,
     530,  523,   11,   11, 0x08,
     600,  583,   11,   11, 0x08,
     641,  118,   11,   11, 0x08,
     661,   11,   11,   11, 0x08,
     676,   11,   11,   11, 0x08,
     695,   11,   11,   11, 0x08,
     724,  710,   11,   11, 0x08,
     752,   11,   11,   11, 0x28,
     776,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_BitcoinGUI[] = {
    "BitcoinGUI\0\0count\0setNumConnections(int)\0"
    "count,countOfPeers\0setNumBlocks(int,int)\0"
    "mining,hashrate\0setMining(bool,int)\0"
    "status\0setEncryptionStatus(int)\0"
    "title,message,modal\0error(QString,QString,bool)\0"
    "nFeeRequired,payFee\0askFee(qint64,bool*)\0"
    "strURI\0handleURI(QString)\0gotoOverviewPage()\0"
    "gotoMiningPage()\0gotoHistoryPage()\0"
    "gotoAddressBookPage()\0gotoReceiveCoinsPage()\0"
    "gotoSendCoinsPage()\0addr\0"
    "gotoSignMessageTab(QString)\0"
    "gotoSignMessageTab()\0gotoVerifyMessageTab(QString)\0"
    "gotoVerifyMessageTab()\0optionsClicked()\0"
    "aboutClicked()\0reason\0"
    "trayIconActivated(QSystemTrayIcon::ActivationReason)\0"
    "parent,start,end\0"
    "incomingTransaction(QModelIndex,int,int)\0"
    "encryptWallet(bool)\0backupWallet()\0"
    "changePassphrase()\0unlockWallet()\0"
    "fToggleHidden\0showNormalIfMinimized(bool)\0"
    "showNormalIfMinimized()\0toggleHidden()\0"
};

void BitcoinGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BitcoinGUI *_t = static_cast<BitcoinGUI *>(_o);
        switch (_id) {
        case 0: _t->setNumConnections((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->setNumBlocks((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->setMining((*reinterpret_cast< bool(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->setEncryptionStatus((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->error((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 5: _t->askFee((*reinterpret_cast< qint64(*)>(_a[1])),(*reinterpret_cast< bool*(*)>(_a[2]))); break;
        case 6: _t->handleURI((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->gotoOverviewPage(); break;
        case 8: _t->gotoMiningPage(); break;
        case 9: _t->gotoHistoryPage(); break;
        case 10: _t->gotoAddressBookPage(); break;
        case 11: _t->gotoReceiveCoinsPage(); break;
        case 12: _t->gotoSendCoinsPage(); break;
        case 13: _t->gotoSignMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 14: _t->gotoSignMessageTab(); break;
        case 15: _t->gotoVerifyMessageTab((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->gotoVerifyMessageTab(); break;
        case 17: _t->optionsClicked(); break;
        case 18: _t->aboutClicked(); break;
        case 19: _t->trayIconActivated((*reinterpret_cast< QSystemTrayIcon::ActivationReason(*)>(_a[1]))); break;
        case 20: _t->incomingTransaction((*reinterpret_cast< const QModelIndex(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 21: _t->encryptWallet((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 22: _t->backupWallet(); break;
        case 23: _t->changePassphrase(); break;
        case 24: _t->unlockWallet(); break;
        case 25: _t->showNormalIfMinimized((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 26: _t->showNormalIfMinimized(); break;
        case 27: _t->toggleHidden(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BitcoinGUI::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BitcoinGUI::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_BitcoinGUI,
      qt_meta_data_BitcoinGUI, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BitcoinGUI::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BitcoinGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BitcoinGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BitcoinGUI))
        return static_cast<void*>(const_cast< BitcoinGUI*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int BitcoinGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 28)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 28;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
