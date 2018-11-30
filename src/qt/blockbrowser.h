#ifndef BITCOIN_QT_BLOCKBROWSER_H
#define BITCOIN_QT_BLOCKBROWSER_H

#include "clientmodel.h"
#include "main.h"
#include "walletmodel.h"
#include "base58.h"
#include "guiutil.h"

#include <string>
#include <QWidget>
#include <QDialog>
#include <QString>

#include "ui_interface.h"

class ClientModel;
class WalletModel;

QT_BEGIN_NAMESPACE
class QSlider;
class QSettings;
class QSpinBox;
QT_END_NAMESPACE


namespace Ui {
 class BlockBrowser;
}

class BlockBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit BlockBrowser(QWidget *parent = 0);
    ~BlockBrowser();
    
    void setClientModel(ClientModel *clientModel);
    void setModel(WalletModel *walletModel);
    void showOutOfSyncWarning(bool fShow);
    
    double getBlockHardness(int);
    double getTxTotalValue(std::string);
    double convertCoins(int64_t);
    double getTxFees(std::string);
    int getBlockTime(int);
    int getBlocknBits(int);
    int getBlockNonce(int);
    int blocksInPastHours(int);
    int getBlockHashrate(int);
    std::string getInputs(std::string);
    std::string getOutputs(std::string);
    std::string getBlockHash(int);
    std::string getBlockMerkle(int);
    std::string getBlockDebug(int);
    bool addnode(std::string);
    const CBlockIndex* getBlockIndex(int);
    int64_t getInputValue(CTransaction, CScript);
    
public slots:
    
    void blockClicked();
    void txClicked();
    void updateExplorer(bool);

private slots:

private:
    Ui::BlockBrowser *ui;
    ClientModel *clientModel;
    WalletModel *walletModel;
    
};

#endif // BITCOIN_QT_BLOCKBROWSER_H
