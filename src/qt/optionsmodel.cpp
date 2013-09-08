#include "optionsmodel.h"
#include "bitcoinunits.h"
#include <QSettings>

#include "init.h"
#include "walletdb.h"
#include "guiutil.h"

OptionsModel::OptionsModel(QObject *parent) :
    QAbstractListModel(parent)
{
    Init();
}

bool static ApplyProxySettings()
{
    QSettings settings;
    CService addrProxy(settings.value("addrProxy", "127.0.0.1:9050").toString().toStdString());
    int nSocksVersion(settings.value("nSocksVersion", 5).toInt());
    if (!settings.value("fUseProxy", false).toBool()) {
        addrProxy = CService();
        nSocksVersion = 0;
        return false;
    }
    if (nSocksVersion && !addrProxy.IsValid())
        return false;
    if (!IsLimited(NET_IPV4))
        SetProxy(NET_IPV4, addrProxy, nSocksVersion);
    if (nSocksVersion > 4) {
#ifdef USE_IPV6
        if (!IsLimited(NET_IPV6))
            SetProxy(NET_IPV6, addrProxy, nSocksVersion);
#endif
        SetNameProxy(addrProxy, nSocksVersion);
    }
    return true;
}

void OptionsModel::Init()
{
    QSettings settings;

    // These are Qt-only settings:
    nDisplayUnit = settings.value("nDisplayUnit", BitcoinUnits::BTC).toInt();
    bDisplayAddresses = settings.value("bDisplayAddresses", false).toBool();
    fMinimizeToTray = settings.value("fMinimizeToTray", false).toBool();
    fMinimizeOnClose = settings.value("fMinimizeOnClose", false).toBool();
    nTransactionFee = settings.value("nTransactionFee").toLongLong();
    nSoundVolume = settings.value("nSoundVolume",75).toInt();
    language = settings.value("language", "").toString();
    startup_sound=settings.value("SoundStartup", "startup.wav").toString();
    incoming_sound=settings.value("SoundIncoming", "incoming.wav").toString();
    mining_sound=settings.value("SoundMining", "mining.wav").toString();
    sent_sound=settings.value("SoundSent", "coinssent.wav").toString();
    sync_sound=settings.value("SoundSync", "sync.mp3").toString();
    about_sound=settings.value("SoundAbout", "about.wav").toString();
    bSoundStartup=settings.value("bUseStartup",true).toBool();
    bSoundIncoming=settings.value("bUseIncoming",true).toBool();
    bSoundMining=settings.value("bUseMining",true).toBool();
    bSoundSent=settings.value("bUseSent",true).toBool();
    bSoundSync=settings.value("bUseSync",true).toBool();
    bSoundAbout=settings.value("bUseAbout",true).toBool();


    // These are shared with core Bitcoin; we want
    // command-line options to override the GUI settings:
    if (settings.contains("fUseUPnP"))
        SoftSetBoolArg("-upnp", settings.value("fUseUPnP").toBool());
    if (settings.contains("addrProxy") && settings.value("fUseProxy").toBool())
        SoftSetArg("-proxy", settings.value("addrProxy").toString().toStdString());
    if (settings.contains("nSocksVersion") && settings.value("fUseProxy").toBool())
        SoftSetArg("-socks", settings.value("nSocksVersion").toString().toStdString());
    if (settings.contains("detachDB"))
        SoftSetBoolArg("-detachdb", settings.value("detachDB").toBool());
    if (!language.isEmpty())
        SoftSetArg("-lang", language.toStdString());
}

bool OptionsModel::Upgrade()
{
    QSettings settings;

    if (settings.contains("bImportFinished"))
        return false; // Already upgraded

    settings.setValue("bImportFinished", true);

    // Move settings from old wallet.dat (if any):
    CWalletDB walletdb("wallet.dat");

    QList<QString> intOptions;
    intOptions << "nDisplayUnit" << "nTransactionFee" << "nSoundVolume";
    foreach(QString key, intOptions)
    {
        int value = 0;
        if (walletdb.ReadSetting(key.toStdString(), value))
        {
            settings.setValue(key, value);
            walletdb.EraseSetting(key.toStdString());
        }
    }
    QList<QString> boolOptions;
    boolOptions << "bDisplayAddresses" << "fMinimizeToTray" << "fMinimizeOnClose" << "fUseProxy" << "fUseUPnP";
    boolOptions << "bUseSound" << "bSoundIncoming" << "bSoundStartup" << "bSoundMining" << "bSoundIncoming";
    boolOptions << "bSoundSent" << "bSoundSync" << "bSoundAbout";

    foreach(QString key, boolOptions)
    {
        bool value = false;
        if (walletdb.ReadSetting(key.toStdString(), value))
        {
            settings.setValue(key, value);
            walletdb.EraseSetting(key.toStdString());
        }
    }
    try
    {
        CAddress addrProxyAddress;
        if (walletdb.ReadSetting("addrProxy", addrProxyAddress))
        {
            settings.setValue("addrProxy", addrProxyAddress.ToStringIPPort().c_str());
            walletdb.EraseSetting("addrProxy");
        }
    }
    catch (std::ios_base::failure &e)
    {
        // 0.6.0rc1 saved this as a CService, which causes failure when parsing as a CAddress
        CService addrProxy;
        if (walletdb.ReadSetting("addrProxy", addrProxy))
        {
            settings.setValue("addrProxy", addrProxy.ToStringIPPort().c_str());
            walletdb.EraseSetting("addrProxy");
        }
    }
    ApplyProxySettings();

    Init();

    return true;
}


int OptionsModel::rowCount(const QModelIndex & parent) const
{
    return OptionIDRowCount;
}

QVariant OptionsModel::data(const QModelIndex & index, int role) const
{
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
            return QVariant(GUIUtil::GetStartOnSystemStartup());
        case MinimizeToTray:
            return QVariant(fMinimizeToTray);
        case MapPortUPnP:
            return settings.value("fUseUPnP", GetBoolArg("-upnp", true));
        case MinimizeOnClose:
            return QVariant(fMinimizeOnClose);
        case ProxyUse:
            return settings.value("fUseProxy", false);
        case ProxyIP: {
            CService addrProxy;
            if (GetProxy(NET_IPV4, addrProxy))
                return QVariant(QString::fromStdString(addrProxy.ToStringIP()));
            else
                return QVariant(QString::fromStdString("127.0.0.1"));
        }
        case ProxyPort: {
            CService addrProxy;
            if (GetProxy(NET_IPV4, addrProxy))
                return QVariant(addrProxy.GetPort());
            else
                return 9050;
        }
        case ProxySocksVersion:
            return settings.value("nSocksVersion", 5);
        case Fee:
            return QVariant(nTransactionFee);
        case DisplayUnit:
            return QVariant(nDisplayUnit);
        case DisplayAddresses:
            return QVariant(bDisplayAddresses);
        case DetachDatabases:
            return QVariant(bitdb.GetDetach());
        case Language:
            return settings.value("language", "");
        case SoundVolume:
            return QVariant(nSoundVolume);
        case SoundStartup:
            return QVariant(startup_sound);
        case SoundAbout:
            return QVariant(about_sound);
        case SoundIncoming:
            return QVariant(incoming_sound);
        case SoundSent:
            return QVariant(sent_sound);
        case SoundMining:
            return QVariant(mining_sound);
        case SoundSync:
            return QVariant(sync_sound);
        case UseAbout:
            return QVariant(bSoundAbout);
        case UseIncoming:
            return QVariant(bSoundIncoming);
        case UseSent:
            return QVariant(bSoundSent);
        case UseStartup:
            return QVariant(bSoundStartup);
        case UseSync:
            return QVariant(bSoundSync);
        case UseMining:
            return QVariant(bSoundMining);
        default:
            return QVariant();
        }
    }
    return QVariant();
}
bool OptionsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    bool successful = true; /* set to false on parse error */
    if(role == Qt::EditRole)
    {
        QSettings settings;
        switch(index.row())
        {
        case StartAtStartup:
            successful = GUIUtil::SetStartOnSystemStartup(value.toBool());
            break;
        case MinimizeToTray:
            fMinimizeToTray = value.toBool();
            settings.setValue("fMinimizeToTray", fMinimizeToTray);
            break;
        case MapPortUPnP:
            fUseUPnP = value.toBool();
            settings.setValue("fUseUPnP", fUseUPnP);
            MapPort();
            break;
        case MinimizeOnClose:
            fMinimizeOnClose = value.toBool();
            settings.setValue("fMinimizeOnClose", fMinimizeOnClose);
            break;
        case ProxyUse:
            settings.setValue("fUseProxy", value.toBool());
            ApplyProxySettings();
            break;
        case ProxyIP:
            {
                CService addrProxy("127.0.0.1", 9050);
                GetProxy(NET_IPV4, addrProxy);
                CNetAddr addr(value.toString().toStdString());
                addrProxy.SetIP(addr);
                settings.setValue("addrProxy", addrProxy.ToStringIPPort().c_str());
                successful = ApplyProxySettings();
            }
            break;
        case ProxyPort:
            {
                CService addrProxy("127.0.0.1", 9050);
                GetProxy(NET_IPV4, addrProxy);
                addrProxy.SetPort(value.toInt());
                settings.setValue("addrProxy", addrProxy.ToStringIPPort().c_str());
                successful = ApplyProxySettings();
            }
            break;
        case ProxySocksVersion:
            settings.setValue("nSocksVersion", value.toInt());
            ApplyProxySettings();
            break;
        case Fee:
            nTransactionFee = value.toLongLong();
            settings.setValue("nTransactionFee", nTransactionFee);
            break;
        case DisplayUnit:
            nDisplayUnit = value.toInt();
            settings.setValue("nDisplayUnit", nDisplayUnit);
            emit displayUnitChanged(nDisplayUnit);
            break;
        case DisplayAddresses:
            bDisplayAddresses = value.toBool();
            settings.setValue("bDisplayAddresses", bDisplayAddresses);
            break;
        case DetachDatabases: {
            bool fDetachDB = value.toBool();
            bitdb.SetDetach(fDetachDB);
            settings.setValue("detachDB", fDetachDB);
            }
            break;
        case Language:
            settings.setValue("language", value);
            break;
        case SoundVolume:
            nSoundVolume = value.toInt();
            settings.setValue("nSoundVolume",nSoundVolume);
            break;
        case UseStartup:
            bSoundStartup = value.toBool();
            settings.setValue("bUseStartup", bSoundStartup);
            break;
        case UseIncoming:
            bSoundIncoming = value.toBool();
            settings.setValue("bUseIncoming",bSoundIncoming);
            break;
        case UseSent:
            bSoundSent = value.toBool();
            settings.setValue("bUseSent",bSoundSent);
            break;
        case UseMining:
            bSoundMining = value.toBool();
            settings.setValue("bUseMining",bSoundMining);
            break;
        case UseSync:
            bSoundSync = value.toBool();
            settings.setValue("bUseSync",bSoundSync);
            break;
        case UseAbout:
            bSoundAbout = value.toBool();
            settings.setValue("bUseAbout",bSoundAbout);
            break;
        case SoundStartup:
            startup_sound=value.toString();
            settings.setValue("SoundStartup",startup_sound);
            break;
        case SoundIncoming:
            incoming_sound=value.toString();
            settings.setValue("SoundIncoming",incoming_sound);
            break;
        case SoundSent:
            sent_sound=value.toString();
            settings.setValue("SoundSent",sent_sound);
            break;
        case SoundMining:
            mining_sound=value.toString();
            settings.setValue("SoundMining",mining_sound);
            break;
        case SoundSync:
            sync_sound=value.toString();
            settings.setValue("SoundSync",sync_sound);
            break;
        case SoundAbout:
            about_sound=value.toString();
            settings.setValue("SoundAbout",about_sound);
            break;
        default:
            break;
        }
    }
    emit dataChanged(index, index);

    return successful;
}

qint64 OptionsModel::getTransactionFee()
{
    return nTransactionFee;
}

bool OptionsModel::getMinimizeToTray()
{
    return fMinimizeToTray;
}

bool OptionsModel::getMinimizeOnClose()
{
    return fMinimizeOnClose;
}

int OptionsModel::getDisplayUnit()
{
    return nDisplayUnit;
}

bool OptionsModel::getDisplayAddresses()
{
    return bDisplayAddresses;
}

bool OptionsModel::getUseStartup()
{
    return(bSoundStartup);
}

bool OptionsModel::getUseIncoming()
{
    return(bSoundIncoming);
}
bool OptionsModel::getUseSent()
{
    return(bSoundSent);
}
bool OptionsModel::getUseMining()
{
    return(bSoundMining);
}
bool OptionsModel::getUseSync()
{
    return(bSoundSync);
}
bool OptionsModel::getUseAbout()
{
    return(bSoundAbout);
}

QString OptionsModel::getSoundStartup()
{
    return(startup_sound);
}

QString OptionsModel::getSoundIncoming()
{
    return(incoming_sound);
}

QString OptionsModel::getSoundSent()
{
    return(sent_sound);
}

QString OptionsModel::getSoundMining()
{
    return(mining_sound);
}

QString OptionsModel::getSoundSync()
{
    return(sync_sound);
}

QString OptionsModel::getSoundAbout()
{
    return(about_sound);
}

int OptionsModel::getSoundVolume()
{
    return(nSoundVolume);
}


