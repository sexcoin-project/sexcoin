// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c)      2014 The Sexcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "forkparams.h"
#include "main.h"
#include "util.h"

using namespace std;

static CForkParams forkParams;

#define FORK1_HEIGHT 155000
#define FORK2_HEIGHT 572000
#define FORK3_HEIGHT 643808

// Constructor
CForkParams::CForkParams() {
        // Timespan and Spacing values are moved here from definition as
        // global vars in main.cpp.
        nTargetTimespan = 8 * 60 * 60; // 8 hours
        nTargetSpacing = 60; // 60 Seconds
        // Interval 480 blocks

        // Fork1 - Tweak Difficulty Calc window etc
        nTargetTimespan_Fork1 = 30 * 60; // 30 minutes
        nTargetSpacing_Fork1 = 30; // 30 seconds
        // Interval 60 blocks
        
        // Fork2 - Replace Retarget Algo with KGW
        //         In Fork2 these vars are only used by ComputeMinWork
        nTargetTimespan_Fork2 = 15 * 60; // 15 Minutes
        nTargetSpacing_Fork2 = 60; // 60 seconds
        // Interval 15 blocks

        nKGWHeight = FORK2_HEIGHT; // when to start using KGW algo
        nKGWTimeWarpFixHeight = FORK3_HEIGHT; // when to use "fixed" KGW algo code
}

int64_t CForkParams::Timespan(int64_t height) const {
    //int64_t height=chainActive.Height();
    //LogPrintf("CForkParams: height = %d\n", height);
    if ( height > FORK1_HEIGHT ) { return nTargetTimespan_Fork1; }
    if ( height > FORK2_HEIGHT ) { return nTargetTimespan_Fork2; }
    return nTargetTimespan;
}

int64_t CForkParams::Spacing(int64_t height) const {
    //int64_t height=chainActive.Height();
    if  ( height > FORK1_HEIGHT ) { return nTargetSpacing_Fork1; }
    if  ( height > FORK2_HEIGHT ) { return nTargetSpacing_Fork2; }
    return nTargetSpacing;
}


int64_t CForkParams::Interval(int64_t height) const { return Timespan(height)/Spacing(height); }
int64_t CForkParams::KGWHeight() const { return nKGWHeight; }
int64_t CForkParams::KGWTimeWarpFixHeight() const { return nKGWTimeWarpFixHeight; }

static CForkParams *pCurrentForkingParams = &forkParams;
const CForkParams &ForkingParams() { return *pCurrentForkingParams; }