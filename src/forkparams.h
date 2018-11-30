// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin developers
// Copyright (c)      2014 The Sexcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef SEXCOIN_FORK_PARAMS_H
#define SEXCOIN_FORK_PARAMS_H

#include "main.h"
#include "util.h"

using namespace std;


/*
 * CForkParams compliments the CChainParams class by providing blockheight
 * dependent values needed in various places throughout sexcoind.
 *
 * These are values that were purposely changed during the life of the
 * blockchain and caused hardforks in the blockchain when they occured. They
 * cannot be deprecated and must be used when syncing old portions of the
 * blockchain such as when starting a new client for the first time.
 */
class CForkParams {
    public:
        CForkParams();
        int64_t Timespan(int64_t height) const;
        int64_t Spacing(int64_t height) const;
        int64_t Interval(int64_t height) const;
        int64_t KGWHeight() const;
        int64_t KGWTimeWarpFixHeight() const;

        
    protected:
        // Timespan and Spacing values are moved here from global vars in main.cpp

        // Original
        int64_t nTargetTimespan;
        int64_t nTargetSpacing;

        // Fork1 - Tweak Difficulty Calc window etc
        int64_t nTargetTimespan_Fork1;
        int64_t nTargetSpacing_Fork1;

        // Fork2 - Replace Retarget Algo with KGW
        //         In Fork2 these vars are only used by ComputeMinWork
        int64_t nTargetTimespan_Fork2;
        int64_t nTargetSpacing_Fork2;

        // When to switch to Original KGW Algorithm (Vulnerable to Time Warp Attack)
        int64_t nKGWHeight;

        // When to switch to Anti-TimeWarp Attack KGW Algorithm
        int64_t nKGWTimeWarpFixHeight;
        
        // Used to set the height to check

};

/*
 * Remain as identical as possible to "CChain::Params()" method which we
 * will be replacing for height-dependent parameter calls
 */
const CForkParams &ForkingParams();

#endif