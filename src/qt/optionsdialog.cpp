#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "guiutil.h"

#include "bitcoinamountfield.h"
#include "bitcoinunits.h"
#include "monitoreddatamapper.h"
#include "netbase.h"
#include "main.h"
#include "optionsmodel.h"
#include "qvalidatedlineedit.h"
#include "qvaluecombobox.h"
#include "boost/filesystem.hpp"

#include <QCheckBox>
#include <QDir>
#include <QIntValidator>
#include <QLabel>
#include <QLineEdit>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QRegExpValidator>
#include <QStringList>
#include <QSettings>
#include <QTabWidget>
#include <QWidget>
#include <QString>


OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog),
    model(0),
    mapper(0),
    fRestartWarningDisplayed_Proxy(false),
    fRestartWarningDisplayed_Lang(false),
    fProxyIpValid(true)
{
    printf("*******************  Initializing Options Dialog ****************************\n");
    ui->setupUi(this);
    populateSoundCombos();

    /* Network elements init */
#ifndef USE_UPNP
    ui->mapPortUpnp->setEnabled(false);
#endif

    ui->socksVersion->setEnabled(false);
    ui->socksVersion->addItem("5", 5);
    ui->socksVersion->addItem("4", 4);
    ui->socksVersion->setCurrentIndex(0);

    ui->proxyIp->setEnabled(false);
    ui->proxyPort->setEnabled(false);
    ui->proxyPort->setValidator(new QIntValidator(0, 65535, this));

    ui->editMaxAcceptedHeight->setText( QString(nMaxHeightAccepted));

    connect(ui->connectSocks, SIGNAL(toggled(bool)), ui->socksVersion, SLOT(setEnabled(bool)));
    connect(ui->connectSocks, SIGNAL(toggled(bool)), ui->proxyIp, SLOT(setEnabled(bool)));
    connect(ui->connectSocks, SIGNAL(toggled(bool)), ui->proxyPort, SLOT(setEnabled(bool)));

    ui->proxyIp->installEventFilter(this);

    /* Window elements init */
#ifdef Q_WS_MAC
    ui->tabWindow->setVisible(false);
#endif

    /* Display elements init */
    QDir translations(":translations");
    ui->lang->addItem(QString("(") + tr("default") + QString(")"), QVariant(""));
    foreach(const QString &langStr, translations.entryList())
    {
        QLocale locale(langStr);

        /** check if the locale name consists of 2 parts (language_country) */
        if(langStr.contains("_"))
        {
#if QT_VERSION >= 0x040800
            /** display language strings as "native language - native country (locale name)", e.g. "Deutsch - Deutschland (de)" */
            ui->lang->addItem(locale.nativeLanguageName() + QString(" - ") + locale.nativeCountryName() + QString(" (") + langStr + QString(")"), QVariant(langStr));
#else
            /** display language strings as "language - country (locale name)", e.g. "German - Germany (de)" */
            ui->lang->addItem(QLocale::languageToString(locale.language()) + QString(" - ") + QLocale::countryToString(locale.country()) + QString(" (") + langStr + QString(")"), QVariant(langStr));
#endif
        }
        else
        {
#if QT_VERSION >= 0x040800
            /** display language strings as "native language (locale name)", e.g. "Deutsch (de)" */
            ui->lang->addItem(locale.nativeLanguageName() + QString(" (") + langStr + QString(")"), QVariant(langStr));
#else
            /** display language strings as "language (locale name)", e.g. "German (de)" */
            ui->lang->addItem(QLocale::languageToString(locale.language()) + QString(" (") + langStr + QString(")"), QVariant(langStr));
#endif
        }
    }

    ui->unit->setModel(new BitcoinUnits(this));

    connect(ui->connectSocks, SIGNAL(clicked(bool)), this, SLOT(showRestartWarning_Proxy()));
    connect(ui->lang, SIGNAL(activated(int)), this, SLOT(showRestartWarning_Lang()));

    /* Sound Elements Init */

    connect(ui->s_startup_enable, SIGNAL(toggled(bool)), ui->s_startup_enable, SLOT(setChecked(bool)));
    connect(ui->s_incoming_enable, SIGNAL(toggled(bool)), ui->s_incoming_enable, SLOT(setChecked(bool)));
    connect(ui->s_sent_enable, SIGNAL(toggled(bool)), ui->s_sent_enable, SLOT(setChecked(bool)));
    connect(ui->s_mining_enable, SIGNAL(toggled(bool)), ui->s_mining_enable, SLOT(setChecked(bool)));
    connect(ui->s_sync_enable, SIGNAL(toggled(bool)), ui->s_sync_enable, SLOT(setChecked(bool)));
    connect(ui->s_about_enable, SIGNAL(toggled(bool)), ui->s_about_enable, SLOT(setChecked(bool)));

    connect(ui->aboutSoundFile, SIGNAL(currentIndexChanged(int)), ui->aboutSoundFile, SLOT(setCurrentIndex(int)));
    connect(ui->syncSoundFile, SIGNAL(currentIndexChanged(int)), ui->syncSoundFile, SLOT(setCurrentIndex(int)));
    connect(ui->startupSoundFile, SIGNAL(currentIndexChanged(int)), ui->startupSoundFile, SLOT(setCurrentIndex(int)));
    connect(ui->incomingSoundFile, SIGNAL(currentIndexChanged(int)), ui->incomingSoundFile, SLOT(setCurrentIndex(int)));
    connect(ui->sentSoundFile, SIGNAL(currentIndexChanged(int)), ui->sentSoundFile, SLOT(setCurrentIndex(int)));
    connect(ui->miningSoundFile, SIGNAL(currentIndexChanged(int)), ui->miningSoundFile, SLOT(setCurrentIndex(int)));

    connect(ui->aboutSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));
    connect(ui->startupSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));
    connect(ui->incomingSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));
    connect(ui->sentSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));
    connect(ui->miningSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));
    connect(ui->syncSoundFile, SIGNAL(currentIndexChanged(QString)),this,SLOT(enableSaveButtons()));


    /* Widget-to-option mapper */
    mapper = new MonitoredDataMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Vertical);

    /* enable save buttons when data modified */
    connect(mapper, SIGNAL(viewModified()), this, SLOT(enableSaveButtons()));
    /* disable save buttons when new data loaded */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(disableSaveButtons()));
    /* disable/enable save buttons when proxy IP is invalid/valid */
    connect(this, SIGNAL(proxyIpValid(bool)), this, SLOT(setSaveButtonState(bool)));



}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::setModel(OptionsModel *model)
{
    this->model = model;

    if(model)
    {
        connect(model, SIGNAL(displayUnitChanged(int)), this, SLOT(updateDisplayUnit()));

        mapper->setModel(model);
        setMapper();
        mapper->toFirst();
    }

    // update the display unit, to not use the default ("BTC")
    updateDisplayUnit();

    // Set the sound combo boxes to the selected sound file
    // Set the starting tab back to 0
    // Disable the Save button because setting the sound choice emits a 'change' signal
    setSoundChoice();
    ui->tabWidget->setCurrentIndex(0);
    disableSaveButtons();

}

void OptionsDialog::setMapper()
{
    /* Main */
    mapper->addMapping(ui->transactionFee, OptionsModel::Fee);
    mapper->addMapping(ui->bitcoinAtStartup, OptionsModel::StartAtStartup);
    mapper->addMapping(ui->detachDatabases, OptionsModel::DetachDatabases);

    /* Network */
    mapper->addMapping(ui->mapPortUpnp, OptionsModel::MapPortUPnP);
    mapper->addMapping(ui->connectSocks, OptionsModel::ProxyUse);
    mapper->addMapping(ui->socksVersion, OptionsModel::ProxySocksVersion);
    mapper->addMapping(ui->proxyIp, OptionsModel::ProxyIP);
    mapper->addMapping(ui->proxyPort, OptionsModel::ProxyPort);
    mapper->addMapping(ui->editMaxAcceptedHeight, OptionsModel::MaxHeightAccepted);

    /* Window */
#ifndef Q_WS_MAC
    mapper->addMapping(ui->minimizeToTray, OptionsModel::MinimizeToTray);
    mapper->addMapping(ui->minimizeOnClose, OptionsModel::MinimizeOnClose);
#endif

    /* Display */
    mapper->addMapping(ui->lang, OptionsModel::Language);
    mapper->addMapping(ui->unit, OptionsModel::DisplayUnit);
    mapper->addMapping(ui->displayAddresses, OptionsModel::DisplayAddresses);

    /* Sounds */
    mapper->addMapping(ui->s_about_enable, OptionsModel::UseAbout);
    mapper->addMapping(ui->s_startup_enable, OptionsModel::UseStartup);
    mapper->addMapping(ui->s_incoming_enable, OptionsModel::UseIncoming);
    mapper->addMapping(ui->s_sent_enable, OptionsModel::UseSent);
    mapper->addMapping(ui->s_mining_enable, OptionsModel::UseMining);
    mapper->addMapping(ui->s_sync_enable, OptionsModel::UseSync);

    mapper->addMapping(ui->aboutSoundFile, OptionsModel::SoundAbout,QByteArray("currentText") );
    mapper->addMapping(ui->startupSoundFile, OptionsModel::SoundStartup,QByteArray("currentText") );
    mapper->addMapping(ui->incomingSoundFile, OptionsModel::SoundIncoming,QByteArray("currentText") );
    mapper->addMapping(ui->sentSoundFile, OptionsModel::SoundSent,QByteArray("currentText") );
    mapper->addMapping(ui->miningSoundFile, OptionsModel::SoundMining,QByteArray("currentText") );
    mapper->addMapping(ui->syncSoundFile, OptionsModel::SoundSync,QByteArray("currentText"));
    mapper->addMapping(ui->volumeSlider, OptionsModel::SoundVolume);


}

void OptionsDialog::enableSaveButtons()
{
    // prevent enabling of the save buttons when data modified, if there is an invalid proxy address present
    if(fProxyIpValid)
        setSaveButtonState(true);
}

void OptionsDialog::disableSaveButtons()
{
    setSaveButtonState(false);
}

void OptionsDialog::setSaveButtonState(bool fState)
{
    ui->applyButton->setEnabled(fState);
    ui->okButton->setEnabled(fState);
}

void OptionsDialog::on_okButton_clicked()
{
    mapper->submit();
    accept();
}

void OptionsDialog::on_cancelButton_clicked()
{
    reject();
}

void OptionsDialog::on_applyButton_clicked()
{
    mapper->submit();
    ui->applyButton->setEnabled(false);
}

void OptionsDialog::showRestartWarning_Proxy()
{
    if(!fRestartWarningDisplayed_Proxy)
    {
        QMessageBox::warning(this, tr("Warning"), tr("This setting will take effect after restarting Sexcoin."), QMessageBox::Ok);
        fRestartWarningDisplayed_Proxy = true;
    }
}

void OptionsDialog::showRestartWarning_Lang()
{
    if(!fRestartWarningDisplayed_Lang)
    {
        QMessageBox::warning(this, tr("Warning"), tr("This setting will take effect after restarting Sexcoin."), QMessageBox::Ok);
        fRestartWarningDisplayed_Lang = true;
    }
}

void OptionsDialog::updateDisplayUnit()
{
    if(model)
    {
        // Update transactionFee with the current unit
        ui->transactionFee->setDisplayUnit(model->getDisplayUnit());
    }
}

bool OptionsDialog::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->proxyIp && event->type() == QEvent::FocusOut)
    {
        // Check proxyIP for a valid IPv4/IPv6 address
        CService addr;
        if(!LookupNumeric(ui->proxyIp->text().toStdString().c_str(), addr))
        {
            ui->proxyIp->setValid(false);
            fProxyIpValid = false;
            ui->statusLabel->setStyleSheet("QLabel { color: red; }");
            ui->statusLabel->setText(tr("The supplied proxy address is invalid."));
            emit proxyIpValid(false);
        }
        else
        {
            fProxyIpValid = true;
            ui->statusLabel->clear();
            emit proxyIpValid(true);
        }
    }


//    if(object == ui->editMaxAcceptedHeight && event->type() == QEvent::FocusOut)
//    {
//        this->enableSaveButtons();
//    }
    return QDialog::eventFilter(object, event);
}

void OptionsDialog::populateSoundCombos()
{
    printf("******* Poplating **********************************\n");
    // get list of files in /Sounds
    QString soundpath = QCoreApplication::applicationDirPath() + "/Sounds";
    QDir folder(soundpath);
    QStringList files = folder.entryList(QDir::Files);
    ui->aboutSoundFile->addItems(files);
    ui->startupSoundFile->addItems(files);
    ui->incomingSoundFile->addItems(files);
    ui->sentSoundFile->addItems(files);
    ui->miningSoundFile->addItems(files);
    ui->syncSoundFile->addItems(files);
}

void OptionsDialog::setSoundChoice()
{
    //QMessageBox::information(this, tr("Debug"), model->getSoundSent(), QMessageBox::Ok);
    ui->aboutSoundFile->setCurrentIndex(ui->aboutSoundFile->findText(model->getSoundAbout()));
    ui->startupSoundFile->setCurrentIndex(ui->startupSoundFile->findText(model->getSoundStartup()));
    ui->incomingSoundFile->setCurrentIndex(ui->incomingSoundFile->findText(model->getSoundIncoming()));
    ui->sentSoundFile->setCurrentIndex(ui->sentSoundFile->findText(model->getSoundSent()));
    ui->miningSoundFile->setCurrentIndex(ui->miningSoundFile->findText(model->getSoundMining()));
    ui->syncSoundFile->setCurrentIndex(ui->syncSoundFile->findText(model->getSoundSync()));
}


void OptionsDialog::on_toggle_all_clicked()
{
    bool state = true;
    if(ui->s_startup_enable->isChecked())
        state=false;
    ui->s_startup_enable->setChecked(state);
    ui->s_incoming_enable->setChecked(state);
    ui->s_sent_enable->setChecked(state);
    ui->s_mining_enable->setChecked(state);
    ui->s_about_enable->setChecked(state);
    ui->s_sync_enable->setChecked(state);

}

void OptionsDialog::on_playStartup_clicked()
{
    GUIUtil::PlaySound(ui->startupSoundFile->currentText());
}

void OptionsDialog::on_playIncoming_clicked()
{
    GUIUtil::PlaySound(ui->incomingSoundFile->currentText());
}

void OptionsDialog::on_playSent_clicked()
{
    GUIUtil::PlaySound(ui->sentSoundFile->currentText());
}

void OptionsDialog::on_playMining_clicked()
{
    GUIUtil::PlaySound(ui->miningSoundFile->currentText());
}

void OptionsDialog::on_playAbout_clicked()
{
   GUIUtil::PlaySound(ui->aboutSoundFile->currentText());
}

void OptionsDialog::on_playSync_clicked()
{
    GUIUtil::PlaySound(ui->syncSoundFile->currentText());
}

void OptionsDialog::on_volumeSlider_sliderMoved(int position)
{
    QSettings settings;
    settings.setValue("nSoundVolume",position);
}

