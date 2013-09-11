#ifndef OPTIONSMODEL_H
#define OPTIONSMODEL_H

#include <QAbstractListModel>

/** Interface from Qt to configuration data structure for Bitcoin client.
   To Qt, the options are presented as a list with the different options
   laid out vertically.
   This can be changed to a tree once the settings become sufficiently
   complex.
 */
class OptionsModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit OptionsModel(QObject *parent = 0);

    enum OptionID {
        StartAtStartup,    // bool
        MinimizeToTray,    // bool
        MapPortUPnP,       // bool
        MinimizeOnClose,   // bool
        ProxyUse,          // bool
        ProxyIP,           // QString
        ProxyPort,         // int
        ProxySocksVersion, // int
        Fee,               // qint64
        DisplayUnit,       // BitcoinUnits::Unit
        DisplayAddresses,  // bool
        DetachDatabases,   // bool
        Language,          // QString
        UseStartup,         // bool
        UseIncoming,        // bool
        UseSent,            // bool
        UseMining,          // bool
        UseSync,            // bool
        UseAbout,           // bool
        SoundVolume,        // int
        SoundStartup,       // QString
        SoundIncoming,      // QString
        SoundSent,          // QString
        SoundMining,        // QString
        SoundSync,          // QString
        SoundAbout,         // QString
        OptionIDRowCount
    };

    void Init();

    /* Migrate settings from wallet.dat after app initialization */
    bool Upgrade(); /* returns true if settings upgraded */

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

    /* Explicit getters */
    qint64 getTransactionFee();
    bool getMinimizeToTray();
    bool getMinimizeOnClose();
    int getDisplayUnit();
    bool getDisplayAddresses();

    bool getUseStartup();
    bool getUseIncoming();
    bool getUseSent();
    bool getUseMining();
    bool getUseSync();
    bool getUseAbout();
    QString getSoundMining();
    QString getSoundSent();
    QString getSoundIncoming();
    QString getSoundSync();
    QString getSoundStartup();
    QString getSoundAbout();
    int getSoundVolume();
    QString getLanguage() { return language; }

private:
    int nDisplayUnit;
    int nSoundVolume;
    bool bDisplayAddresses;
    bool fMinimizeToTray;
    bool fMinimizeOnClose;
    bool bSoundStartup;
    bool bSoundIncoming;
    bool bSoundMining;
    bool bSoundSent;
    bool bSoundSync;
    bool bSoundAbout;
    QString language;
    QString startup_sound;
    QString incoming_sound;
    QString mining_sound;
    QString sent_sound;
    QString sync_sound;
    QString about_sound;

signals:
    void displayUnitChanged(int unit);
};

#endif // OPTIONSMODEL_H
