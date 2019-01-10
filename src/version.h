// Copyright (c) 2012-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_VERSION_H
#define BITCOIN_VERSION_H

/**
 * network protocol versioning
 */

/**
 * Sexcoin network protocol 60703 is equvilent to Litecoin 70003. 
 * We are finally fixing that so we can be consistent with Litecoin conventions
 */
static const int PROTOCOL_VERSION = 70015;

//! initial proto version, to be increased after version/verack negotiation
static const int INIT_PROTO_VERSION = 209;

//! In this version, 'getheaders' was introduced.
static const int GETHEADERS_VERSION = 60703; //sexcoin

//! disconnect from peers older than this proto version
static const int MIN_PEER_PROTO_VERSION = 70013; //GETHEADERS_VERSION;

//! nTime field added to CAddress, starting with this version;
//! if possible, avoid requesting addresses nodes older than this
static const int CADDR_TIME_VERSION = 31402;

//! BIP 0031, pong message, is enabled for all versions AFTER this one
static const int BIP0031_VERSION = 60000;

// Sexcoin: The following have not ever been in Sexcoin. We are keeping the defines to hopefully make future updates easier
//          Any tests performed against these version will all be below these thresholds except for this version.

//! "filter*" commands are disabled without NODE_BLOOM after and including this version 
static const int NO_BLOOM_VERSION = 70011;

//! "sendheaders" command and announcing blocks with headers starts with this version
static const int SENDHEADERS_VERSION = 70012;

//! "feefilter" tells peers to filter invs to you by fee starts with this version
static const int FEEFILTER_VERSION = 70013;

//! short-id-based block download starts with this version
static const int SHORT_IDS_BLOCKS_VERSION = 70014;

//! not banning for invalid compact blocks starts with this version
static const int INVALID_CB_NO_BAN_VERSION = 70015;

#endif // BITCOIN_VERSION_H
