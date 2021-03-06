// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The FacileCoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */
 

static Checkpoints::MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
        ( 0, uint256("a6cf4867070a4e77f5a3e4b320ac1f94985d8d66e00014bd21c0d846ab3bff40"))
		( 200, uint256("d97bfe9e4f28e96426377c5fa8e8611f801b5f55551d448f3ba826a22e962660"))
		( 372, uint256("6ee59ef308a727e2684485d11f178f0bfb808c80b9245487f7693ebbb52a5634"))
		( 481, uint256("728b8c2d6e89e7d4c9d06c846395285acc688687986d16df668e03a491669d3d"))
		( 484, uint256("c66a2a29529cb605e6c7fe32459f288e5473d4529f2fcfdc66b76ff335fcf704"))
		( 487, uint256("2a4d6aa87eb90cd04b41df362b93b100923d6ca856f5275d009a319a0442c937"))
		( 491, uint256("ebc8a09ae0d945759c7c204aed8c651be9fa79a72dc6825f56cd4f51be00d250"))
		( 3865, uint256("86f1d56c066db0416e28b216ef71aff455175e64efd25f34ce285bb6b1158e77"))
		( 20143, uint256("4f990fdec9e1e9e501c6dff25b96580f3d482465d7862e342d5cc907cc30935b"))
        ;
static const Checkpoints::CCheckpointData data = {
        &mapCheckpoints,
        1464133921,
        20688,
        0
    };

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
        ( 0, uint256("0000000007890bd7c13d1b9cc5260a405f3b093c91c2554bcff131bfd10447e8"))
		
        ;
static const Checkpoints::CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
        0,
        0,
        300
    };

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0000000007890bd7c13d1b9cc5260a405f3b093c91c2554bcff131bfd10447e8"))
        ;
static const Checkpoints::CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

class CMainParams : public CChainParams {
public:
    CMainParams() {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /** 
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xf5;
        pchMessageStart[1] = 0xb9;
        pchMessageStart[2] = 0xba;
        pchMessageStart[3] = 0xd5;
        vAlertPubKey = ParseHex("0416a36f052e1a51a3dd1112bbe484cbe2a40ea073af3c240cc5cecc28767037081b1fbc77976e669179a21273d5824bbec9e74838ad569644e6ed612af406ee02");
        nDefaultPort = 8764;
        //bnProofOfWorkLimit = uint256(0xfffffff0);
		bnProofOfWorkLimit = 0x400;
        nSubsidyHalvingInterval = 17500 * 6;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        //nTargetTimespan = 30 * 24 * 60 * 60; // one month
		nTargetTimespan = 24 * 60 * 60; // one day
        //nTargetTimespan = 2 * 60 * 60; // two hours retarget every odd block
        nTargetSpacing = 2 * 60 * 60;

		 
        const char* pszTimestamp = "Phys.org March/12/2016 Game over! Computer wins series against Go champion (Update)";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
		//// first CScriptNum(4) - nBits 
        txNew.vin[0].scriptSig = CScript() << CScriptNum(0x400) << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 300 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("0436d22bb33090acd17a036ceda97c29a831d7b200cc81d47f8b0c05131d759b137c01a3204d0f2d20c2d47267f3372f3ec9120c5ea10f9ee3eea5bc1f29781009") << OP_CHECKSIG;
        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
		//std::cout << "strlen(pszTimestamp)=" << strlen(pszTimestamp) << "\n";
		//std::cout << "hashMerkleRoot: " << genesis.hashMerkleRoot.ToString() <<"\n";
        genesis.nVersion = 1;
        genesis.nTime    = 1461109524;
		genesis.nBits    = 0x400;
        genesis.nNonce   = 4;
		
		genesis.sol.SetVal(0,7);
		genesis.sol.SetVal(1,6);
		genesis.sol.SetVal(2,8);
		genesis.sol.SetVal(3,13);
		genesis.sol.SetVal(4,14);
		genesis.sol.SetVal(5,3);
		genesis.sol.SetVal(6,9);
		genesis.sol.SetVal(7,10);
		genesis.sol.SetVal(8,5);
		genesis.sol.SetVal(9,11);
		genesis.sol.SetVal(10,12);
		genesis.sol.SetVal(11,15);
		genesis.sol.SetVal(12,4);
		genesis.sol.SetVal(13,1);
		genesis.sol.SetVal(14,16);
		genesis.sol.SetVal(15,2);
		
		
        hashGenesisBlock = genesis.GetHash();
		assert(game_check_sol(hashGenesisBlock, genesis.nBits, genesis.sol) == true);
        assert(genesis.hashMerkleRoot == uint256("5dea40e54bdf447aed4387a69b3e941885b2db46fc4f0d8015e111126b0af03b"));

        // vSeeds.push_back(CDNSSeedData("facilecoin.net", "seed.facilecoin.net"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(0x62);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(128+0x62);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = false;
        fTestnetToBeDeprecatedFieldRPC = false;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0xca;
        pchMessageStart[1] = 0xaf;
        pchMessageStart[2] = 0xe5;
        pchMessageStart[3] = 0xb9;
        vAlertPubKey = ParseHex("04085bd862303f95fe2180e93e5b85b6af1fed4b149733a019b22b0378dcc0817465fb68200e6f7be01b7c6d2e82c7788e6221d3a317de5c2cddcc66ffa7af1627");
        nDefaultPort = 18763;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 30 * 24 * 60 * 60; //! two weeks
        nTargetSpacing = 2 * 60 * 60 / 10;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        genesis.nTime = 1444427680;
        genesis.nNonce = 3023279188;
        hashGenesisBlock = genesis.GetHash();
        //assert(hashGenesisBlock == uint256("0000000007890bd7c13d1b9cc5260a405f3b093c91c2554bcff131bfd10447e8"));

        vFixedSeeds.clear();
        vSeeds.clear();

        base58Prefixes[PUBKEY_ADDRESS] = list_of(111);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(196);
        base58Prefixes[SECRET_KEY]     = list_of(239);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x35)(0x87)(0xCF);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x35)(0x83)(0x94);

        convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 1;
        nTargetTimespan = 30 * 24 * 60 * 60; //! two weeks
        nTargetSpacing = 2 * 60 * 60;
        //bnProofOfWorkLimit = ~uint256(0) >> 1;
		bnProofOfWorkLimit = 0x400;
        genesis.nTime = 1296688602;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 2;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 28763;
        //assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams {
public:
    CUnitTestParams() {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 18445;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Unit test mode doesn't have any DNS seeds.

        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const 
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval)  { nSubsidyHalvingInterval=anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority)  { nEnforceBlockUpgradeMajority=anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority)  { nRejectBlockOutdatedMajority=anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority)  { nToCheckBlockUpgradeMajority=anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks)  { fDefaultConsistencyChecks=afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) {  fAllowMinDifficultyBlocks=afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams *pCurrentParams = 0;

CModifiableParams *ModifiableParams()
{
   assert(pCurrentParams);
   assert(pCurrentParams==&unitTestParams);
   return (CModifiableParams*)&unitTestParams;
}

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        case CBaseChainParams::UNITTEST:
            return unitTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
