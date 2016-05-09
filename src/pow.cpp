// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The FacileCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chain.h"
#include "chainparams.h"
#include "primitives/block.h"
#include "uint256.h"
#include "util.h"

#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock)
{
	
	int minor_diff_lut[] = { 30, 40, 60, 136, 325, 889, 3161,  3161 * 4};
	//// whites per lut 1, 2, 4, 5, 6, 7, 8, 9
	int64_t minor_diff_lut_steps = SIZEOF_ARRAY(minor_diff_lut);
	
	unsigned int target_major, target_minor;
	
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit().GetCompact_dummy();

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;
		//return 4 + 0;
	
	if(pindexLast->nHeight == (Params().RC2SoftForkHeight() - 1))
		return nProofOfWorkLimit;
	
	
	//debgug
	//if(pindexLast->nHeight == 1 )
	//	return pindexLast->nBits;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % Params().Interval() != 0)
    {
        if (Params().AllowMinDifficultyBlocks())
        {
            // Special difficulty rule for testnet:
            // If the new block's timestamp is more than 2* 10 minutes
            // then allow mining of a min-difficulty block.
            if (pblock->GetBlockTime() > pindexLast->GetBlockTime() + Params().TargetSpacing()*2)
                return nProofOfWorkLimit;
            else
            {
                // Return the last non-special-min-difficulty-rules-block
                const CBlockIndex* pindex = pindexLast;
                while (pindex->pprev && pindex->nHeight % Params().Interval() != 0 && pindex->nBits == nProofOfWorkLimit)
                    pindex = pindex->pprev;
                return pindex->nBits;
            }
        }
        return pindexLast->nBits;
    }

    // Go back by what we want to be 14 days worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < Params().Interval()-1; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    LogPrintf("  nActualTimespan = %d  before bounds\n", nActualTimespan);

	

	
    // Retarget
    uint256 bnNew;
    uint256 bnOld;
    bnNew.SetCompact_dummy(pindexLast->nBits);
    bnOld = bnNew;
	
	if(pindexLast->nHeight < (Params().RC2SoftForkHeight() - 1)){
	
		if (bnNew == uint256(0x400)){
			if(nActualTimespan < Params().TargetTimespan()/120)
				bnNew = uint256(0x401);		
			//return bnNew.GetCompact_dummy();
		}else{
		
			// revert target increase/decrease direction compared with bitecoin
			// bnNew *= nActualTimespan;
			// bnNew /= Params().TargetTimespan();

			
			// bnNew *= Params().TargetTimespan();
			// if(nActualTimespan > 0)
				// bnNew /= nActualTimespan;
			
			if(bnNew < nProofOfWorkLimit)
				bnNew = nProofOfWorkLimit;
			
			if (bnNew > 0x4000)
				bnNew = 0x4000; // 64 x 64	
			
			int64_t raw_target = bnNew.GetLow64();
			
			target_major = ((raw_target >> 8) & 0xff);
			target_minor = (raw_target & 0xff);
			
			if(target_minor >= minor_diff_lut_steps) target_minor = minor_diff_lut_steps - 1;
			
			//increase or decrease?
			if (nActualTimespan < Params().TargetTimespan() * (minor_diff_lut_steps - 1) / minor_diff_lut_steps){
				//increase
				bool found_next = false;
				int64_t how_many_times = Params().TargetTimespan() / (nActualTimespan + 1);
				int64_t current_minor_diff = minor_diff_lut[target_minor];
				for(int i = target_minor; i < minor_diff_lut_steps - 1; i++)
					if(minor_diff_lut[i] > current_minor_diff * how_many_times){
						found_next = true;
						target_minor = i;
						break;
					}
				if(!found_next){
					target_major ++;
					target_minor = 0;
				}
				LogPrintf("GetNextWorkRequired increase diff. found_next: %d, how_many_times: %lu, current_minor_diff: %lu, target_major: %d, target_minor: %d\n", found_next, how_many_times, current_minor_diff, target_major, target_minor);
			}
			
			if (nActualTimespan > Params().TargetTimespan() * minor_diff_lut_steps / (minor_diff_lut_steps - 1)  ){ //warning divide by zero if minor_diff_lut empty 
				//decrease
				bool found_next = false;
				int64_t how_many_times = nActualTimespan / Params().TargetTimespan();
				int64_t current_minor_diff = minor_diff_lut[target_minor];
				for(int i = target_minor; i >= 0; i--)
					if(minor_diff_lut[i] < current_minor_diff / ( how_many_times + 1)){
						found_next = true;
						target_minor = i;
						break;
					}
				if(!found_next){
					target_major --;
					target_minor = minor_diff_lut_steps - 1;
				}
				LogPrintf("GetNextWorkRequired decrease diff. found_next: %d, how_many_times: %lu, current_minor_diff: %lu, target_major: %d, target_minor: %d\n", found_next, how_many_times, current_minor_diff, target_major, target_minor);		
				
			}
			
			bnNew = target_major & 0xff;
			bnNew <<= 8;
			bnNew += (target_minor & 0xff);
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//RC2 soft fork
	//Board solving difficulty grows almost geometrically
	//RC1 attempts to calculate best new diff
	//RC2 - increase / decrease diff by one when treshold is passed (sort of delta modulation)
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	else
	{
		int64_t how_many_times = Params().TargetTimespan() / (nActualTimespan + 1) / 6;
		
		if(bnNew < nProofOfWorkLimit)
			bnNew = nProofOfWorkLimit;
		
		if (bnNew > 0x4000)
			bnNew = 0x4000; // 64 x 64	
		
		int64_t raw_target = bnNew.GetLow64();
		
		target_major = ((raw_target >> 8) & 0xff);
		target_minor = (raw_target & 0xff);
		
		if(target_minor >= minor_diff_lut_steps) target_minor = minor_diff_lut_steps - 1;
		
		//increase or decrease?
		//Params().TargetTimespan() - 1 day before fork, 1/6 day after fork (2h -> 20 min)
		if (nActualTimespan < Params().TargetTimespan() / 100 / 6 ){

			if(	target_minor < minor_diff_lut_steps -1){
				target_minor++;
			}
			else
			{
				target_major ++;
				target_minor = 0;
			}
			LogPrintf("GetNextWorkRequired increase diff. how_many_times: %lu, target_major: %d, target_minor: %d\n", how_many_times, target_major, target_minor);
		}
		
		if (nActualTimespan > Params().TargetTimespan() / 6  ){ 
			//decrease
			if(	target_minor > 0){
				target_minor--;
			}
			else
			{
				target_major --;
				target_minor = minor_diff_lut_steps - 1;
			}
			LogPrintf("GetNextWorkRequired decrease diff. how_many_times: %lu, target_major: %d, target_minor: %d\n", how_many_times, target_major, target_minor);
			
		}
		
		bnNew = target_major & 0xff;
		bnNew <<= 8;
		bnNew += (target_minor & 0xff);
			
		
	}
	
	/*
    if (nActualTimespan < Params().TargetTimespan()/100){
        //nActualTimespan = Params().TargetTimespan()/2;
		bnNew++;
		// if (nActualTimespan < Params().TargetTimespan()/4)
			// bnNew++;
	}
    if (bnNew != 0 && nActualTimespan > Params().TargetTimespan()*100){
        //nActualTimespan = Params().TargetTimespan()*2;
		bnNew--;
		// if (bnNew != 0 && nActualTimespan > Params().TargetTimespan()*4)
			// bnNew--;
	}
	*/
	
	if(bnNew < nProofOfWorkLimit)
		bnNew = nProofOfWorkLimit;
	
    if (bnNew > 0x4000)
        bnNew = 0x4000; // 64 x 64

    /// debug print
    LogPrintf("GetNextWorkRequired RETARGET\n");
    //LogPrintf("Params().TargetTimespan() = %d    nActualTimespan = %d\n", Params().TargetTimespan(), nActualTimespan);
	LogPrintf("nActualTimespan = %d\n", nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact_dummy(), bnNew.ToString());

    return bnNew.GetCompact_dummy();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, GSol sol)
{
    bool fNegative;
    bool fOverflow;
    uint256 bnTarget;

	/*
	//not used
    if (Params().SkipProofOfWorkCheck())
       return true;	
	*/
	
	
	return game_check_sol(hash, nBits, sol);
	

    bnTarget.SetCompact_dummy(nBits, &fNegative, &fOverflow);

    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit())
        return error("CheckProofOfWork() : nBits below minimum work");

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork() : hash doesn't match nBits");

    return true;
}
//// chainwork
uint256 GetBlockProof(const CBlockIndex& block)
{
    uint256 bnTarget;
    bool fNegative;
    bool fOverflow;
    bnTarget.SetCompact_dummy(block.nBits, &fNegative, &fOverflow);
    if (fNegative || fOverflow || bnTarget == 0)
        return 0;
	if (bnTarget >= 0xfffffff0)
		return 0xfffffff0;
    // We need to compute 2**256 / (bnTarget+1), but we can't represent 2**256
    // as it's too large for a uint256. However, as 2**256 is at least as large
    // as bnTarget+1, it is equal to ((2**256 - bnTarget - 1) / (bnTarget+1)) + 1,
    // or ~bnTarget / (nTarget+1) + 1.
    //return (~bnTarget / (bnTarget + 1)) + 1;
	//return (uint256(0xfffffff0) / (bnTarget + 1)) + 1;
	return bnTarget + 1;
}
