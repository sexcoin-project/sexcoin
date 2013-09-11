// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Copyright (c) 2013 Sexcoin Developers

// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
	
    static MapCheckpoints mapCheckpoints =
            boost::assign::map_list_of
            (   0, uint256("0x2946a91685f253cd2ca29cde8cc35d7773cab280cdab4a075f613636e697aca4"))
            (   5363, uint256("0xc5dd0d66a07c176a4463be3df7d9309986a3918b75935dde1c4769e4a64f9593"))
            (   5369, uint256("0xdcd139890a39921876ab035eca34ee48c5239889f1dcdb8e3de3d097847f12d8"))
            (   5380, uint256("0xb105b9cbb7b0ff4f2f6aef1d040c196edc2ab4318f7e6811a4373e8278cd5bb4"))
            (   13899, uint256("0x883879d5325e48511e96557fff17df10123f062de23bc1f91f4e153154dbc764"))
            (   14050, uint256("0x5be09cdd886573a50d543e3cca35a03eff2e46e4596bb2f509cede9e28dd33e9"))
            (   22984, uint256("0x87ecfd9aa3c722132dd1786caa5ccb25b8ff821a3797aa0c424e10662aca509d"))
            (   39986, uint256("0x9dba252fa6eebbf2b6c790965806c51916870bdf1e91bb7bf11eea55e64f12f8"))
            (   49979, uint256("0xe564a2434f3acb7fe4af103927083fee3fa6429afa430e53b6eade3249dfe026"))
            (   80493, uint256("0x6da822b8d4b5c060aee57523952630ac2262d5f56759ffc451ba6298b5fa423b"))
            (   94458, uint256("0x084c2dec2c0da13e8f0143303d8f27ae79c81311ec804b2f746fbc1ad83bff14"))
            (   136354,uint256("0x4f75d45e08213d5bb0584ce1e65666d47596cb8059b20d1c354b5bfd26309fbe"))
       /*
       */
            ;


    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints
        //return true;
        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
		
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;
		
        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
