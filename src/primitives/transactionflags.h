// Copyright (c) 2016 The Sexcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_PRIMITIVES_TRANSACTIONFLAGS_H
#define BITCOIN_PRIMITIVES_TRANSACTIONFLAGS_H

#include <string>

using namespace std;

enum transflag_t{
    
    TX_F_NONE               = 0x0000,
    TX_F_IS_OVER_CONSENT    = 0x0001,
    TX_F_IS_OVER_18         = 0x0002,
    TX_F_IS_OVER_21         = 0x0004,
    TX_F_4                  = 0x0008,
    TX_F_5                  = 0x0010,
    TX_F_6                  = 0x0020,
    TX_F_7                  = 0x0040,
    TX_F_8                  = 0x0080,
    TX_F_9                  = 0x0100,
    TX_F_10                 = 0x0200,
    TX_F_11                 = 0x0400,
    TX_F_12                 = 0x0800,
    TX_F_13                 = 0x1000,
    TX_F_14                 = 0x2000,
    TX_F_15                 = 0x4000,
    TX_F_INVALID_CODE       = 0x8000,

};

#endif // BITCOIN_PRIMITIVES_TRANSACTIONFLAGS_H