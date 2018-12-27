// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "arith_uint256.h"
#include "chain.h"
#include "util.h"
#include "auxpow/check.h"

unsigned int CalculateNextWorkRequired_V1(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan/4)
        nActualTimespan = params.nPowTargetTimespan/4;
    if (nActualTimespan > params.nPowTargetTimespan*4)
        nActualTimespan = params.nPowTargetTimespan*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Viacoin: intermediate uint256 can overflow by 1 bit
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    //bool fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    //if (fShift)
    //    bnNew >>= 1;
    
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan;
    //if (fShift)
    //    bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired_V1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // Viacoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval())
        blockstogoback = params.DifficultyAdjustmentInterval();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired_V1(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int CalculateNextWorkRequired_V2(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - nFirstBlockTime;
    if (nActualTimespan < params.nPowTargetTimespan2/4)
        nActualTimespan = params.nPowTargetTimespan2/4;
    if (nActualTimespan > params.nPowTargetTimespan2*4)
        nActualTimespan = params.nPowTargetTimespan2*4;

    // Retarget
    arith_uint256 bnNew;
    arith_uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
    bnOld = bnNew;
    // Viacoin: intermediate uint256 can overflow by 1 bit
    const arith_uint256 bnPowLimit = UintToArith256(params.powLimit);
    //bool fShift = bnNew.bits() > bnPowLimit.bits() - 1;
    //if (fShift)
    //    bnNew >>= 1;
    
    bnNew *= nActualTimespan;
    bnNew /= params.nPowTargetTimespan2;
    //if (fShift)
    //    bnNew <<= 1;

    if (bnNew > bnPowLimit)
        bnNew = bnPowLimit;

    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired_V2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    assert(pindexLast != nullptr);
    unsigned int nProofOfWorkLimit = UintToArith256(params.powLimit).GetCompact();

    // Only change once per difficulty adjustment interval
    if ((pindexLast->nHeight+1) % params.DifficultyAdjustmentInterval2() != 0)
    {
        if (params.fPowAllowMinDifficultyBlocks)
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + params.nPowTargetSpacing2*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % params.DifficultyAdjustmentInterval2() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    // Viacoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = params.DifficultyAdjustmentInterval2()-1;
    if ((pindexLast->nHeight+1) != params.DifficultyAdjustmentInterval2())
        blockstogoback = params.DifficultyAdjustmentInterval2();

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;

    assert(pindexFirst);

    return CalculateNextWorkRequired_V2(pindexLast, pindexFirst->GetBlockTime(), params);
}

unsigned int KimotoGravityWell(const CBlockIndex* pindexLast, const Consensus::Params& params) {

    const CBlockIndex   *BlockLastSolved                 = pindexLast;
    const CBlockIndex   *BlockReading                    = pindexLast;

    uint64_t             PastBlocksMass                  = 0;
    int64_t              PastRateActualSeconds           = 0;
    int64_t              PastRateTargetSeconds           = 0;
    double               PastRateAdjustmentRatio         = double(1);
    arith_uint256              PastDifficultyAverage;
    arith_uint256              PastDifficultyAveragePrev;
    arith_uint256              BlockReadingDifficulty;
    double               EventHorizonDeviation;
    double               EventHorizonDeviationFast;
    double               EventHorizonDeviationSlow;

    static const int64_t TargetBlockSpacing              = 60;
    unsigned int         TimeDaySeconds                  = 60 * 60 * 24;
    int64_t              PastSecondsMin                  = TimeDaySeconds * 0.25;
    int64_t              PastSecondsMax                  = TimeDaySeconds * 7;
    uint64_t             PastBlocksMin                   = PastSecondsMin / TargetBlockSpacing;
    uint64_t             PastBlocksMax                   = PastSecondsMax / TargetBlockSpacing;
    arith_uint256        nProofOfWorkLimit               = UintToArith256(params.powLimit);
    
    int64_t              TimeWarpFixHeight               = params.Fork3Height;

    
    //LogPrint(1, "KimotoGravityWell()[in function]: block: %s\n", BlockLastSolved->ToString());  // LEDTMP
    //LogPrint(1, "PastBlocksMin = %d, PastBlocksMax=%d\n", PastBlocksMin, PastBlocksMax);
    if (BlockLastSolved == NULL || BlockLastSolved->nHeight == 0 ||
        (uint64_t)BlockLastSolved->nHeight < PastBlocksMin) {
        return nProofOfWorkLimit.GetCompact();
    }
    
    int64_t LatestBlockTime = BlockLastSolved->GetBlockTime();
    
    for (unsigned int i = 1; BlockReading && BlockReading->nHeight > 0; i++) {
        if (PastBlocksMax > 0 && i > PastBlocksMax) { break; }
        PastBlocksMass++;

        if (i == 1) {
            PastDifficultyAverage.SetCompact(BlockReading->nBits);
        } else {
            BlockReadingDifficulty.SetCompact(BlockReading->nBits);
            if (BlockReadingDifficulty > PastDifficultyAveragePrev) {
                PastDifficultyAverage = PastDifficultyAveragePrev + ((BlockReadingDifficulty - PastDifficultyAveragePrev) / i);
            } else {
                PastDifficultyAverage = PastDifficultyAveragePrev - ((PastDifficultyAveragePrev - BlockReadingDifficulty) / i);
            }
        }
        PastDifficultyAveragePrev = PastDifficultyAverage;


        if (LatestBlockTime < BlockReading->GetBlockTime()) {
            if (BlockReading->nHeight > TimeWarpFixHeight)
                LatestBlockTime = BlockReading->GetBlockTime();
        }
        PastRateActualSeconds = LatestBlockTime - BlockReading->GetBlockTime();
        PastRateTargetSeconds = TargetBlockSpacing * PastBlocksMass;
        PastRateAdjustmentRatio = double(1);
        if (BlockReading->nHeight > TimeWarpFixHeight) {
            if (PastRateActualSeconds < 1) { PastRateActualSeconds = 1; }
        } else {
            if (PastRateActualSeconds < 0) { PastRateActualSeconds = 0; }
        }
        if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
            PastRateAdjustmentRatio = double(PastRateTargetSeconds) / double(PastRateActualSeconds);
        }
        EventHorizonDeviation                   = 1 + (0.7084 * pow((double(PastBlocksMass)/double(144)), -1.228));
        EventHorizonDeviationFast               = EventHorizonDeviation;
        EventHorizonDeviationSlow               = 1 / EventHorizonDeviation;

        if (PastBlocksMass >= PastBlocksMin) {
            if ((PastRateAdjustmentRatio <= EventHorizonDeviationSlow) ||
                (PastRateAdjustmentRatio >= EventHorizonDeviationFast)) {
                assert(BlockReading);
                break;
            }
        }
        if (BlockReading->pprev == NULL) { assert(BlockReading); break; }
        BlockReading = BlockReading->pprev;
    }

    arith_uint256 bnNew(PastDifficultyAverage);
    if (PastRateActualSeconds != 0 && PastRateTargetSeconds != 0) {
        bnNew *= PastRateActualSeconds;
        //LogPrint(1,"* bnNew = %08x\n", ArithToUint256(bnNew).ToString());
        bnNew /= PastRateTargetSeconds;
        //LogPrint(1,"/ bnNew = %08x\n", ArithToUint256(bnNew).ToString());
    }
    if (bnNew > nProofOfWorkLimit) { bnNew = nProofOfWorkLimit; }

    // debug print
    /*
    LogPrint(1,"PastDifficultyAverage = %08x\n", ArithToUint256(PastDifficultyAverage).ToString());
    LogPrint(1,"PastRateAdjustmentRatio =  %g    PastRateTargetSeconds = %d    PastRateActualSeconds = %d\n",
               PastRateAdjustmentRatio, PastRateTargetSeconds, PastRateActualSeconds);
    LogPrint(1,"Before:            %s\n", ArithToUint256(arith_uint256(BlockLastSolved->nBits)).ToString() );
    LogPrint(1,"After:             %s\n", ArithToUint256(bnNew.GetCompact()).ToString());
    LogPrint(1,"nProofOfWorkLimit: %s\n", ArithToUint256(nProofOfWorkLimit.GetCompact()).ToString());
    */
    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, const Consensus::Params& params)
{
    // -regtest mode
    if (params.fPowNoRetargeting)
        return pindexLast->nBits;

    if (pindexLast->nHeight+1 >= params.Fork3Height || (params.fPowAllowMinDifficultyBlocks && pindexLast->nHeight+1 >= 300000)) {
        //return AntiGravityWave(2, pindexLast, pblock, params);
        return KimotoGravityWell(pindexLast, params);
    } else if (pindexLast->nHeight+1 >= params.Fork2Height) {
        //return AntiGravityWave(1, pindexLast, params);
        LogPrint(1, "KimotoGravityWell()[switcher]: block: %s\n", (pblock->GetHash()).ToString());  // LEDTMP
        return KimotoGravityWell(pindexLast, params);
    } else if (pindexLast->nHeight+1 >= params.Fork1Height) { 
        //LogPrint(1, "GNR_V2(): block: %s\n", (pblock->GetHash()).ToString());  // LEDTMP
        return GetNextWorkRequired_V2(pindexLast, pblock, params);
    } else {
        //LogPrint(1, "GNR_V1: block: %s\n", (pblock->GetHash()).ToString());  // LEDTMP
        return GetNextWorkRequired_V1(pindexLast, pblock, params);
    }
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, const Consensus::Params& params)
{
    bool fNegative;
    bool fOverflow;
    arith_uint256 bnTarget;

    bnTarget.SetCompact(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > UintToArith256(params.powLimit))
        return false;

    // Check proof of work matches claimed amount
    return UintToArith256(hash) <= bnTarget;

}

bool CheckBlockProofOfWork(const CBlockHeader *pblock, const Consensus::Params& params)
{
    // LogPrint("txdb", "CheckBlockProofOfWork(): block: %s\n", pblock->ToString());  // LEDTMP

    if (pblock->auxpow && (pblock->auxpow.get() != nullptr))
    {
        if (!CheckAuxpow(pblock->auxpow, pblock->GetHash(), pblock->GetChainID(), params))
            return error("CheckBlockProofOfWork() : AUX POW is not valid");
        // Check proof of work matches claimed amount
        if (!CheckProofOfWork(pblock->auxpow->GetParentBlockHash(), pblock->nBits, params))
            return error("CheckBlockProofOfWork() : AUX proof of work failed");
    }
    else
    {
        // Check proof of work matches claimed amount
        if (!CheckProofOfWork(pblock->GetPoWHash(), pblock->nBits, params))
            return error("CheckBlockProofOfWork() : proof of work failed");
    }
    return true;
}

bool CheckAuxPowValidity(const CBlockHeader* pblock, const Consensus::Params& params)
{
    if (!params.fPowAllowMinDifficultyBlocks)
    {
        if (pblock->GetChainID() != AuxPow::CHAIN_ID)
            return error("CheckAuxPowValidity() : block does not have our chain ID");
    }
    return true;
}

// TODO LED TMP temporary public interface for passing the build of test/pow_tests.cpp only
// TODO LED TMP this code should be removed and test/pow_test.cpp changed to call
// TODO LED TMP our interface to PoW --> GetNextWorkRequired()
unsigned int CalculateNextWorkRequired(const CBlockIndex* pindexLast, int64_t nFirstBlockTime, const Consensus::Params& params)
{
    return CalculateNextWorkRequired_V1(pindexLast, nFirstBlockTime, params);
}
