#include "blockbrowser.h"
#include "ui_blockbrowser.h"
#include "main.h"
#include "walletmodel.h"
#include "base58.h"
#include "clientmodel.h"
//#include "bitcoinrpc.h"

#include "rpcclient.h"
#include "transactionrecord.h"

#include <sstream>
#include <string>
#include <boost/exception/to_string.hpp>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include <QSettings>
#include <QSlider>

double BlockBrowser::getBlockHardness(int height)
{
    const CBlockIndex* blockindex = getBlockIndex(height);

    int nShift = (blockindex->nBits >> 24) & 0xff;

    double dDiff =
        (double)0x0000ffff / (double)(blockindex->nBits & 0x00ffffff);

    while (nShift < 29)
    {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29)
    {
        dDiff /= 256.0;
        nShift--;
    }

    return dDiff;
}

int BlockBrowser::getBlockHashrate(int height)
{
    int lookup = height;

    double timeDiff = getBlockTime(height) - getBlockTime(1);
    double timePerBlock = timeDiff / lookup;

    return (boost::int64_t)(((double)getBlockHardness(height) * pow(2.0, 32)) / timePerBlock);
}

const CBlockIndex* BlockBrowser::getBlockIndex(int height)
{
    std::string hex = getBlockHash(height);
    uint256 hash(hex);
    return mapBlockIndex[hash];
}

std::string BlockBrowser::getBlockHash(int Height)
{
    CBlockIndex* pblockindex = chainActive[Height];
    if(Height > pblockindex->nHeight) { return "73dc70a1698579360b62e724ecfeacfd938f45283162f3cf18f1b9eb3fc9fcd7"; }
    if(Height < 0) { return "73dc70a1698579360b62e724ecfeacfd938f45283162f3cf18f1b9eb3fc9fcd7"; }
    int desiredheight;
    desiredheight = Height;
    if (desiredheight < 0 || desiredheight > chainActive.Height())
        return 0;

    CBlock block;
    //pblockindex = mapBlockIndex[hashBestChain];
    while (pblockindex->nHeight > desiredheight)
        pblockindex = pblockindex->pprev;
    return pblockindex->phashBlock->GetHex();
}

int BlockBrowser::getBlockTime(int Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nTime;
}

std::string BlockBrowser::getBlockMerkle(int Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->hashMerkleRoot.ToString().substr(0,10).c_str();
}

int BlockBrowser::getBlocknBits(int Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nBits;
}

int BlockBrowser::getBlockNonce(int Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nNonce;
}

std::string BlockBrowser::getBlockDebug(int Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->ToString();
}

int BlockBrowser::blocksInPastHours(int hours)
{
    CBlockIndex* pindexBest = pindexBestHeader;
    int wayback = hours * 3600;
    bool check = true;
    int height = pindexBest->nHeight;
    int heightHour = pindexBest->nHeight;
    int utime = (int)time(NULL);
    int target = utime - wayback;

    while(check)
    {
        if(getBlockTime(heightHour) < target)
        {
            check = false;
            return height - heightHour;
        } else {
            heightHour = heightHour - 1;
        }
    }

    return 0;
}

double BlockBrowser::getTxTotalValue(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock,true))
        return 11;

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    double value = 0;
    double buffer = 0;
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];

        buffer = value + convertCoins(txout.nValue);
        value = buffer;
    }

    return value;
}

double BlockBrowser::convertCoins(int64_t amount)
{
    return (double)amount / ((double)COIN);
}

std::string BlockBrowser::getOutputs(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock,true))
        return "fail";

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    std::string str = "";
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];
        CTxDestination source;
        ExtractDestination(txout.scriptPubKey, source);
        CBitcoinAddress addressSource(source);
        std::string lol7 = addressSource.ToString();
        double buffer = convertCoins(txout.nValue);
        std::string amount = boost::to_string(buffer);
        str.append(lol7);
        str.append(": ");
        str.append(amount);
        str.append(" SXC");
        str.append("\n");
    }

    return str;
}

std::string BlockBrowser::getInputs(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock, true))
        return "fail";

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    std::string str = "";
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        uint256 hash;
        const CTxIn& vin = tx.vin[i];
        hash.SetHex(vin.prevout.hash.ToString());
        
        CTransaction wtxPrev;
        uint256 hashBlock = 0;
        if (!GetTransaction(hash, wtxPrev, hashBlock, true))
             return "fail (you probably need to enable transaction indexing)";

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << wtxPrev;

        CTxDestination source;
        ExtractDestination(wtxPrev.vout[vin.prevout.n].scriptPubKey, source);
        CBitcoinAddress addressSource(source);
        std::string lol6 = addressSource.ToString();
        const CScript target = wtxPrev.vout[vin.prevout.n].scriptPubKey;
        double buffer = convertCoins(getInputValue(wtxPrev, target));
        std::string amount = boost::to_string(buffer);
        str.append(lol6);
        str.append(": ");
        str.append(amount);
        str.append("SXC");
        str.append("\n");
    }

    return str;
}

int64_t BlockBrowser::getInputValue(CTransaction tx, CScript target)
{
    
    double valueOut = 0;
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];
        if(txout.scriptPubKey == target)
        {
            valueOut+= txout.nValue;
        }
    }
    return valueOut;
}

double BlockBrowser::getTxFees(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);


    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock, true))
        return 1;

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    double valueOut = 0;
    double buffer = 0;
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];

        buffer = valueOut + convertCoins(txout.nValue);
        valueOut = buffer;
    }

    double valueIn = 0;
    double buffer0 = 0;
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        uint256 hash0;
        const CTxIn& vin = tx.vin[i];
        hash0.SetHex(vin.prevout.hash.ToString());
        CTransaction wtxPrev;
        uint256 hashBlock0 = 0;
        if (!GetTransaction(hash0, wtxPrev, hashBlock0, true))
             return 0;
        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << wtxPrev;
        const CScript target = wtxPrev.vout[vin.prevout.n].scriptPubKey;
        buffer0 = valueIn + convertCoins(getInputValue(wtxPrev, target));
        valueIn = buffer0;
    }

    return valueIn - valueOut;
}


BlockBrowser::BlockBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlockBrowser)
{
    ui->setupUi(this);

    setFixedSize(400, 420);
        
    connect(ui->blockButton, SIGNAL(pressed()), this, SLOT(blockClicked()));
    connect(ui->txButton, SIGNAL(pressed()), this, SLOT(txClicked()));
}

void BlockBrowser::updateExplorer(bool block)
{    
    CBlockIndex* pindexBest = pindexBestHeader;
    if(block)
    {
        ui->heightLabel->show();
        ui->heightLabel_2->show();
        ui->hashLabel->show();
        ui->hashBox->show();
        ui->merkleLabel->show();
        ui->merkleBox->show();
        ui->nonceLabel->show();
        ui->nonceBox->show();
        ui->bitsLabel->show();
        ui->bitsBox->show();
        ui->timeLabel->show();
        ui->timeBox->show();
        ui->hardLabel->show();
        ui->hardBox->show();;
        ui->pawLabel->show();
        ui->pawBox->show();
        int height = ui->heightBox->value();
        if (height > pindexBest->nHeight)
        {
            ui->heightBox->setValue(pindexBest->nHeight);
            height = pindexBest->nHeight;
        }
        int Pawrate = getBlockHashrate(height);
        double Pawrate2 = 0.000;
        Pawrate2 = ((double)Pawrate / 1000);
        std::string hash = getBlockHash(height);
        std::string merkle = getBlockMerkle(height);
        int nBits = getBlocknBits(height);
        int nNonce = getBlockNonce(height);
        int atime = getBlockTime(height);
        double hardness = getBlockHardness(height);
        QString QHeight = QString::number(height);
        QString QHash = QString::fromUtf8(hash.c_str());
        QString QMerkle = QString::fromUtf8(merkle.c_str());
        QString QBits = QString::number(nBits);
        QString QNonce = QString::number(nNonce);
        QString QTime = QString::number(atime);
        QString QHardness = QString::number(hardness, 'f', 6);
        QString QPawrate = QString::number(Pawrate2, 'f', 3);
        ui->heightLabel->setText(QHeight);
        ui->hashBox->setText(QHash);
        ui->merkleBox->setText(QMerkle);
        ui->bitsBox->setText(QBits);
        ui->nonceBox->setText(QNonce);
        ui->timeBox->setText(QTime);     
        ui->hardBox->setText(QHardness);
        ui->pawBox->setText(QPawrate + " KH/s");
    } 
    
    if(block == false) {
        ui->txID->show();
        ui->txLabel->show();
        ui->valueLabel->show();
        ui->valueBox->show();
        ui->inputLabel->show();
        ui->inputBox->show();
        ui->outputLabel->show();
        ui->outputBox->show();
        ui->feesLabel->show();
        ui->feesBox->show();
        std::string txid = ui->txBox->text().toUtf8().constData();
        double value = getTxTotalValue(txid);
        double fees = getTxFees(txid);
        std::string outputs = getOutputs(txid);
        std::string inputs = getInputs(txid);
        QString QValue = QString::number(value, 'f', 6);
        QString QID = QString::fromUtf8(txid.c_str());
        QString QOutputs = QString::fromUtf8(outputs.c_str());
        QString QInputs = QString::fromUtf8(inputs.c_str());
        QString QFees = QString::number(fees, 'f', 6);
        ui->valueBox->setText(QValue + " SXC");
        ui->txID->setText(QID);
        ui->outputBox->setText(QOutputs);
        ui->inputBox->setText(QInputs);
        ui->feesBox->setText(QFees + " SXC");
    }
}


void BlockBrowser::txClicked()
{
    updateExplorer(false);
}

void BlockBrowser::blockClicked()
{
    updateExplorer(true);
}

void BlockBrowser::setModel(WalletModel *walletModel)
{
    this->walletModel = walletModel;
}

void BlockBrowser::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
}

BlockBrowser::~BlockBrowser()
{
    delete ui;
}
