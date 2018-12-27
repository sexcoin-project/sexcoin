// Copyright (c) 2009-2010 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.
#ifndef CONSENSUS_AUXPOW_H
#define CONSENSUS_AUXPOW_H

namespace AuxPow {

// old 0.10 code used the the testnet height for regtest too
const int START_MAINNET = 3080000;
const int START_TESTNET = 1750;
const int START_REGTEST = 4000;

const int CHAIN_ID = 0x69;

enum
{
    // primary version
    BLOCK_VERSION_DEFAULT        = (1 << 0),

    // modifiers
    BLOCK_VERSION_AUXPOW         = (1 << 8),

    // bits allocated for chain ID
    BLOCK_VERSION_CHAIN_START    = (1 << 16),
    BLOCK_VERSION_CHAIN_END      = (1 << 30),
};
} // namespace AuxPow

#endif // CONSENSUS_AUXPOW_H
