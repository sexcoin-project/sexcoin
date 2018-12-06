// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"
#include "consensus/merkle.h"

#include "tinyformat.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>
#include <auxpow/consensus.h>

#include "chainparamsseeds.h"
#include "arith_uint256.h"

#include "crypto/scrypt.h"

static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;
    genesis.vtx.push_back(MakeTransactionRef(std::move(txNew)));
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = BlockMerkleRoot(genesis);
    return genesis;
}

/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    //const char* pszTimestamp = "Block #310379:0000000000000000241252a7bb27be97befe9218c1290dff36c1ff129ec7211a";
    const char* pszTimestamp = "Disaster from the sky in Oklahoma";
    const CScript genesisOutputScript = CScript() << ParseHex("04a5814813115273a109cff99907ba4a05d951873dae7acb6c973d0c9e7c88911a3dbc9aa600deac241b91707e7b4ffb30ad91c8e56e695a1ddf318592988afe0a") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

void CChainParams::UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    consensus.vDeployments[d].nStartTime = nStartTime;
    consensus.vDeployments[d].nTimeout = nTimeout;
}

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        consensus.nSubsidyHalvingInterval = 600000;// sexcoin
        consensus.BIP34Height = 0; // blockV2
        consensus.BIP34Hash = uint256S("0x4e9b54001f9976049830128ec0331515eaabe35a70970d79971da1539a400ba1");
        consensus.BIP65Height = 3106030; // OP_CHECKLOCKTIMEVERIFY
        consensus.BIP66Height = 3106030; // Strict DER signatures
        consensus.BlockVer5Height = 3106030; //sexcoin ... calculate for auxpow
        
        consensus.Fork1Height = 155000;
        consensus.Fork2Height = 572000;
        consensus.Fork3Height = 643808;
        consensus.BlockVer4Height = 2348569; // age verification start

        consensus.powLimit = ArithToUint256(~(arith_uint256(0)) >> 5); // sexcoin

        consensus.nPowTargetTimespan = 8 * 60 * 60; // 8 hours(sexcoin) //14 * 24 * 60 * 60; // two weeks
        consensus.nPowTargetSpacing = 1 * 60; // 1 minute(sexcoin) //1 * 24;
        consensus.fPowAllowMinDifficultyBlocks = false;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 144; // // 10% of nMinerConfirmationWindow
        consensus.nMinerConfirmationWindow = 14400; // Sexcoin retargets every block ... aribitrary number (nPowTargetTimespan / nPowTargetSpacing => 3 days)
        
        consensus.nPowTargetTimespan2 = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing2 = 30; // 30 seconds
        
        consensus.nPowTargetTimespan3 = 15 * 60; // 15 minutes
        consensus.nPowTargetSpacing3 = 60; // 60 seconds


        consensus.nAuxPowStartHeight = AuxPow::START_MAINNET; // auxpow/consensus.h
        //consensus.nWitnessStartHeight = 4040000;
        consensus.nWitnessStartHeight = 3106030; // ~Dec 27

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1230767999; // December 31, 2008

        // Deployment of BIP68, BIP112, and BIP113.OP_CHECKSEQUENCEVERIFY
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1539892620; // October 18, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1760817419;   // October 18, 2025

        // Deployment of SegWit (BIP141, BIP143, and BIP147)
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 1539892620; // October 18, 2018
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 1760817419;   // October 18, 2025

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("00000000000000000000000000000000000000000000000001c9d8a96f2293ef"); // auxpow start: headers become large

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0000000000000000003b9ce759c2a087d52abc4266f8f4ebd6d768b89defa50a"); //477890

        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 32-bit integer with any alignment.
         */
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0x69;
        pchMessageStart[3] = 0x69;
        nDefaultPort = 9560;
        nPruneAfterHeight = 100000;

        genesis = CreateGenesisBlock(1369146359, 244086, 0x1e7fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xf42b9553085a1af63d659d3907a42c3a0052bbfa2693d3acf990af85755f2279"));
        assert(genesis.hashMerkleRoot == uint256S("0x661de12dc8dd26989adb169733b5a99150d52b8b6e8332976277856e246101f4"));

        // Note that of those with the service bits flag, most only support a subset of possible options
        vSeeds.emplace_back("dnsseed.sexcoin.info", true); // hosted by @lavajumper
        vSeeds.emplace_back("dnsseed.lavajumper.com", true); // hosted by @lavajumper

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,62);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,69);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,190);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x88, 0xB2, 0x1E};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x88, 0xAD, 0xE4};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_main, pnSeed6_main + ARRAYLEN(pnSeed6_main));

        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;

        checkpointData = (CCheckpointData) {
            {
                { 0, uint256S("0x2946a91685f253cd2ca29cde8cc35d7773cab280cdab4a075f613636e697aca4")},
                { 5363, uint256S("0xc5dd0d66a07c176a4463be3df7d9309986a3918b75935dde1c4769e4a64f9593")},
                { 5369, uint256S("0xdcd139890a39921876ab035eca34ee48c5239889f1dcdb8e3de3d097847f12d8")},
                { 5380, uint256S("0xb105b9cbb7b0ff4f2f6aef1d040c196edc2ab4318f7e6811a4373e8278cd5bb4")},
                { 13899, uint256S("0x883879d5325e48511e96557fff17df10123f062de23bc1f91f4e153154dbc764")},
                { 14050, uint256S("0x5be09cdd886573a50d543e3cca35a03eff2e46e4596bb2f509cede9e28dd33e9")},
                { 22984, uint256S("0x87ecfd9aa3c722132dd1786caa5ccb25b8ff821a3797aa0c424e10662aca509d")},
                { 39986, uint256S("0x9dba252fa6eebbf2b6c790965806c51916870bdf1e91bb7bf11eea55e64f12f8")},
                { 49979, uint256S("0xe564a2434f3acb7fe4af103927083fee3fa6429afa430e53b6eade3249dfe026")},
                { 80493, uint256S("0x6da822b8d4b5c060aee57523952630ac2262d5f56759ffc451ba6298b5fa423b")},
                { 94458, uint256S("0x084c2dec2c0da13e8f0143303d8f27ae79c81311ec804b2f746fbc1ad83bff14")},
                { 136354,uint256S("0x4f75d45e08213d5bb0584ce1e65666d47596cb8059b20d1c354b5bfd26309fbe")},
                {146221,uint256S("0xc9d38afb57b0b25c822b1287197de413204cacfb27ca9c974772d8d8399737cb")},
                {146849,uint256S("0xc5e18cab151a7eca95b02bd469c5a2aee301ef1b01e3b72add7f04a9c11f8b60")},
                {249936,uint256S("0x6722b04059d14fce5f74eb4a9ea02784ae690c4985ba32801e2cf1f8b65582f3")},
                {279841,uint256S("0xeb3bdef3524a2b0fd89f5480ac2a0a82108539b8e3156b598675e7109803cafa")},
                {319767,uint256S("0x8fbcfa3dac1721fd899f4cf67a7381a86fdcfb5fb504e1729d6a9fe3b389a790")},
                {359900,uint256S("0xfc4faa77d8e6c01941170e131125d5ebb5c9453fbaf3e6c2b0974b66c00f3bcd")},
                {499996,uint256S("0xd28773f08f4747ff6e7e4d113753b5a79b60d905e59ae4046fa4b5ee9965badc")},
                {599825,uint256S("0x0ddf7a53506b99acd201c13fba89b13837eb1707e97c27416f7513052cfd14af")},
                {699886,uint256S("0x1663390cdccecaeea59f35affa91d04f57f9b790b8f1493b7f62d4de2279449a")},
                {809963,uint256S("0xe7c094afaeaf37d20ce7d912b8353c41ac51c5219b1984acda32bfc889898203")},
                {1000293,uint256S("0x40cb1f758e1c3f71b22326f0f9c610202600bd5f83aea5272f4a2d978d344163")},
                {1200283,uint256S("0x6a1238c4d255d45d2669b83730b015ac0534e9e61af543fa66832c918747260f")},
                {1400278,uint256S("0x5c75334308a26b9220b50b8d0adf06fed4921e7a2fbc2b5c551bb9a807533b9f")},
                {1600189,uint256S("0x4b0608c7e733c1b6d2d660469f1b3c17be857ccb19d8e102f41503ab549e2f69")},
                {1800085,uint256S("0x422e9d5dab710fae371a1e182243af38a49db0cfb3d075a5c67da2c4f35df9ef")},
                {2000124,uint256S("0x34710dfebf36429ee09c7bd351671a2716f62f60fbbf9fb231be2314e88615ce")},
                {2100141,uint256S("0xb449eb898b032e00ec87458991a5182cc541c3b479250ed0087860dc60980412")},
                {2399993,uint256S("0xce314cabe66fb60e79a00170b584595d8113e379f165ed9b530db8cc4cb9da0b")},
                {2699990,uint256S("0xfc077d18f64576094c6a6397a7588c6b85ddf2c7a2d41b52ba200ab875aea4e2")},
                {3013737,uint256S("0xeff50a7e9b94b04662d2209dbe8f0f6d0a3796b6f3915cee8ca8dbbae606455c")},
            }
        };

        chainTxData = ChainTxData{
            // (height 2100141).
            1487294544, // * UNIX timestamp of last known number of transactions
            2818059,  // * total number of transactions between genesis and that timestamp
                        //   (the tx=... number in the SetBestChain debug.log lines)
            60000.0         // * estimated number of transactions per second after that timestamp
        };
    }
};

/**
 * Testnet (v3)
 */
class CTestNetParams : public CChainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        consensus.nSubsidyHalvingInterval = 200000;
        consensus.BIP34Height = -1; // Block v2, Height in Coinbase
        consensus.BIP34Hash = uint256S("0x0");
        consensus.BIP65Height = 2100; // OP_CHECKLOCKTIMEVERIFY
        consensus.BIP66Height = 2100; // Strict DER signatures
        consensus.BlockVer5Height = 2100; 
        
        consensus.Fork1Height = 980;
        consensus.Fork2Height = 1200;
        consensus.Fork3Height = 1608;
        consensus.BlockVer4Height = 1700;
        consensus.powLimit = ArithToUint256(~(arith_uint256(0)) >> 5);
        consensus.nPowTargetTimespan = 8 * 60 * 60; // 8 Hours 
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = false;
        consensus.nRuleChangeActivationThreshold = 1; // sexcoin retargets every block //75% for testchains
        consensus.nMinerConfirmationWindow = 25; // from other coins:nPowTargetTimespan / nPowTargetSpacing
                                                 // however sexcoin retargets every block so this is also arbitrary
        
        consensus.nPowTargetTimespan2 = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing2 = 30; // 30 seconds
        
        consensus.nPowTargetTimespan3 = 15 * 60; // 15 minutes
        consensus.nPowTargetSpacing3 = 60; // 60 seconds


        consensus.nAuxPowStartHeight = AuxPow::START_TESTNET;
        consensus.nWitnessStartHeight = 2100; // this is a hard block height that Litecoin does not have.
                                              // BIP 65 and 66 have to be active for this to work.
                                              // Set BIP65, BIP66, BlockV5Height and this to the same value

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 1199145601; // January 1, 2008
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 1232032894; // start + (1year/25)

        // Deployment of BIP68, BIP112, and BIP113. - OP_CHECKSEQUENCEVERIFY
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 1494547200; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 1760294955; // Oct 12, 2025

        // Deployment of SegWit (BIP141, BIP143, and BIP147) - 
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = Consensus::BIP9Deployment::ALWAYS_ACTIVE;//1494547200; // May 12, 2017
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = Consensus::BIP9Deployment::NO_TIMEOUT;//1760294955; // Oct 12, 2025

        // The best chain should have at least this much work.
        //consensus.nMinimumChainWork = uint256S("0x00000000000000000000000000000000000000000000002830dab7f76dbb7d63");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x0"); //1135275

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0x96;
        pchMessageStart[3] = 0x69;
        nDefaultPort = 19560;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1473215502, 517454, 0x1e7fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        LogPrint(1,"Current Genesis: %s\n",genesis.ToString());
        
        // Sexcoin is generating a new testnet genesis. This code will only be used once
        // check that genesis block is valid
        bool fNegative;
        bool fOverflow;
        arith_uint256 bnTarget;
        bnTarget.SetCompact(genesis.nBits, &fNegative, &fOverflow);
        
        if (UintToArith256(consensus.hashGenesisBlock) > bnTarget)
            genesis = FindNewGenesisBlock(genesis);
        
        // end of one use code

        
        //assert(consensus.hashGenesisBlock == uint256S("0x73dc70a1698579360b62e724ecfeacfd938f45283162f3cf18f1b9eb3fc9fcd7"));
        //assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));
        LogPrint(1,"Testnet Genesis:\n %s \n\n",genesis.ToString());
        vFixedSeeds.clear();
        vSeeds.clear();
        // nodes with support for servicebits filtering should be at the top
        vSeeds.emplace_back("testnet.sexcoin.info", true);
        
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,124);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY] =     std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;


        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0x73dc70a1698579360b62e724ecfeacfd938f45283162f3cf18f1b9eb3fc9fcd7")},
            }
        };

        chainTxData = ChainTxData{
            // Data as of block 00000000000001c200b9790dc637d3bb141fe77d155b966ed775b17e109f7c6c (height 1156179)
            1405625749,
            4440,
            5000
        };

    }
};

/**
 * Regression test
 */
class CRegTestParams : public CChainParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        consensus.nSubsidyHalvingInterval = 900;
        consensus.BIP34Height = -1; // BIP34 has not activated on regtest (far in the future so block v1 are not rejected in tests)
        consensus.BIP34Hash = uint256();
        consensus.BIP65Height = 1251; // BIP65 activated on regtest (Used in rpc activation tests)
        consensus.BIP66Height = 1351; // BIP66 activated on regtest (Used in rpc activation tests)
        consensus.BlockVer5Height = 9000;
        
        consensus.Fork1Height = 4500;
        consensus.Fork2Height = 5000;
        consensus.Fork3Height = 6508;
        consensus.BlockVer4Height = 7569;
        consensus.powLimit = uint256S("efffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // ~uint25(0) >> 1
        consensus.nPowTargetTimespan = 8 * 60 * 60; // 8 hours
        consensus.nPowTargetSpacing = 1 * 60;
        consensus.fPowAllowMinDifficultyBlocks = true;
        consensus.fPowNoRetargeting = true;
        consensus.nRuleChangeActivationThreshold = 108; // 75% for testchains
        consensus.nMinerConfirmationWindow = 30; // Faster than normal for regtest (144 instead of 2016)
        
        consensus.nPowTargetTimespan2 = 30 * 60; // 30 minutes
        consensus.nPowTargetSpacing2 = 30; // 30 seconds
        
        consensus.nPowTargetTimespan3 = 15 * 60; // 15 minutes
        consensus.nPowTargetSpacing3 = 60; // 60 seconds

        consensus.nAuxPowStartHeight = AuxPow::START_REGTEST;
        consensus.nWitnessStartHeight = 20000;

        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].bit = 6;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_TESTDUMMY].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].bit = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_CSV].nTimeout = 999999999999ULL;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].bit = 1;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nStartTime = 0;
        consensus.vDeployments[Consensus::DEPLOYMENT_SEGWIT].nTimeout = 999999999999ULL;

        // The best chain should have at least this much work.
        consensus.nMinimumChainWork = uint256S("0x00");

        // By default assume that the signatures in ancestors of this block are valid.
        consensus.defaultAssumeValid = uint256S("0x00");

        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xce;
        pchMessageStart[2] = 0x99;
        pchMessageStart[3] = 0x99;
        nDefaultPort = 19569;
        nPruneAfterHeight = 1000;

        genesis = CreateGenesisBlock(1405166035, 517454, 0x1e1fffff, 1, 50 * COIN);
        consensus.hashGenesisBlock = genesis.GetHash();
        assert(consensus.hashGenesisBlock == uint256S("0xf0dae070f24fbc35311533a22aa85c0a616c84a1f22881612304d802acda286f"));
        //assert(genesis.hashMerkleRoot == uint256S("0x97ddfbbae6be97fd6cdf3e7ca13232a3afff2353e29badfab7f73011edd4ced9"));

        vFixedSeeds.clear(); //!< Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();      //!< Regtest mode doesn't have any DNS seeds.

        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;

        checkpointData = (CCheckpointData) {
            {
                {0, uint256S("0xf0dae070f24fbc35311533a22aa85c0a616c84a1f22881612304d802acda286f")},
            }
        };

        chainTxData = ChainTxData{
            1405166035,
            0,
            0
        };

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]     = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY] = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY] = {0x04, 0x35, 0x83, 0x94};
    }
};

static std::unique_ptr<CChainParams> globalChainParams;

const CChainParams &Params() {
    assert(globalChainParams);
    return *globalChainParams;
}

std::unique_ptr<CChainParams> CreateChainParams(const std::string& chain)
{
    if (chain == CBaseChainParams::MAIN)
        return std::unique_ptr<CChainParams>(new CMainParams());
    else if (chain == CBaseChainParams::TESTNET)
        return std::unique_ptr<CChainParams>(new CTestNetParams());
    else if (chain == CBaseChainParams::REGTEST)
        return std::unique_ptr<CChainParams>(new CRegTestParams());
    throw std::runtime_error(strprintf("%s: Unknown chain %s.", __func__, chain));
}

void SelectParams(const std::string& network)
{
    SelectBaseParams(network);
    globalChainParams = CreateChainParams(network);
}

void UpdateVersionBitsParameters(Consensus::DeploymentPos d, int64_t nStartTime, int64_t nTimeout)
{
    globalChainParams->UpdateVersionBitsParameters(d, nStartTime, nTimeout);
}


// Mine a new genesis block
CBlock CChainParams::FindNewGenesisBlock(CBlock block){
    // if this gets called, it means the genesis block has failed the nBits POW test
    // we need to mine a new one that works. Just like mining, we loop through changing
    // nonces until we get a hash low enough to satisfy the nBits
   
    bool fNegative;
    bool fOverflow;
    arith_uint256 hashTarget; 
    arith_uint256 thash;
    char scratchpad[SCRYPT_SCRATCHPAD_SIZE];
    
    hashTarget.SetCompact(block.nBits, &fNegative, &fOverflow);
    LogPrint(1,"Finding new Genesis Block...\n");
    while(true)
    {
#if defined(USE_SSE2)
        // Detection would work, but in cases where we KNOW it always has SSE2,
        // it is faster to use directly than to use a function pointer or conditional.
#if defined(_M_X64) || defined(__x86_64__) || defined(_M_AMD64) || (defined(MAC_OSX) && defined(__i386__))
        // Always SSE2: x86_64 or Intel MacOS X
        scrypt_1024_1_1_256_sp_sse2(BEGIN(block.nVersion), BEGIN(thash), scratchpad);
#else
        // Detect SSE2: 32bit x86 Linux or Windows
        scrypt_1024_1_1_256_sp(BEGIN(block.nVersion), BEGIN(thash), scratchpad);
#endif
#else
       // Generic scrypt
        scrypt_1024_1_1_256_sp_generic(BEGIN(block.nVersion), BEGIN(thash), scratchpad);
#endif

        if (thash <= hashTarget)
            break;
        if ((block.nNonce & 0xFFF) == 0)
        {
            printf("nonce %08X: hash = %s (target = %s)\n", block.nNonce, thash.ToString().c_str(), hashTarget.ToString().c_str());
        }
        ++block.nNonce;
        if (block.nNonce == 0)
        {
            printf("NONCE WRAPPED, incrementing time\n");
            ++block.nTime;
        }
    }
    LogPrint(1,"block.nTime = %u \n", block.nTime);
    LogPrint(1,"block.nNonce = %u \n", block.nNonce);
    LogPrint(1,"block.GetHash = %s\n", block.GetHash().ToString().c_str());
    LogPrint(1,"block = %s\n",block.ToString());
    
    return(block);
}
