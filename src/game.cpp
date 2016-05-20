#ifndef MCOIN_POW_H
#define MCOIN_POW_H

//#define GAME_DEBUG


#include "primitives/block.h"
#include "uint256.h"
#include "hash.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <boost/thread.hpp>

#include "mscrypt.h"


#define MAX_SHAPE_ENCODE 1 * (8 * 8 * 8 * 8)
#define MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES MAX_SHAPE_ENCODE * 1.3
//#define current_board_side_size 4
#define BOARD_MAX_SIDE_SIZE 64

using namespace std;
//ofstream myfile;


/********************************************************************************************
shapes
*********************************************************************************************/

struct shape_s{
	//
	int white;
	int lc;
	int uc;
	int rc;
	int dc;
	//ff-max
	int lfm;
	int ufm;
	int rfm;
	int dfm;
	//ff-val
	int lfv;
	int ufv;
	int rfv;
	int dfv;
	// coords x + y * side_size
	int vpos;
	unsigned int val;
};

//shape_s shape_arry[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE], shape_arry_orig[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE];
//unsigned int a[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE];

//int current_board_side_size = 4;
//int current_board_size = 16;

/********************************************************************************************
calc_s	
*********************************************************************************************/

#define get_pos(a,b) a + (b) * current_board_side_size
#define left_val(a,b) ((((sa[get_pos(a - 1, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 1) != 0 ? 1 : 0)
#define left_val2(a,b) ((((sa[get_pos(a - 2, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 2) != 0 ? 1 : 0)
#define left_val3(a,b) ((((sa[get_pos(a - 3, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 4) != 0 ? 1 : 0)

#define up_val(a,b) ((((sa[get_pos(a - current_board_side_size, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 8) != 0 ? 1 : 0)
#define up_val2(a,b) ((((sa[get_pos(a - current_board_side_size * 2, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 16) != 0 ? 1 : 0)
#define up_val3(a,b) ((((sa[get_pos(a - current_board_side_size * 3, b)].val >> 6) ^ sa[get_pos(a, b)].val) & 32) != 0 ? 1 : 0)

#define right_val(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + 1, b)].val) & 1) != 0 ? 1 : 0)
#define right_val2(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + 2, b)].val) & 2) != 0 ? 1 : 0)
#define right_val3(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + 3, b)].val) & 4) != 0 ? 1 : 0)

#define down_val(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + current_board_side_size, b)].val) & 8) != 0 ? 1 : 0)
#define down_val2(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + current_board_side_size * 2, b)].val) & 16) != 0 ? 1 : 0)
#define down_val3(a,b) ((((sa[get_pos(a, b)].val >> 6) ^ sa[get_pos(a + current_board_side_size * 3, b)].val) & 32) != 0 ? 1 : 0)

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define rightleft(a,b) (((sa[a] >> 6) ^ sa[b]) & 1) != 0
#define right2left2(a,b) (((sa[a] >> 6) ^ sa[b]) & 2) != 0
#define right3left3(a,b) (((sa[a] >> 6) ^ sa[b]) & 4) != 0

#define downup(a,b) (((sa[a] >> 6) ^ sa[b]) & 8) != 0
#define down2up2(a,b) (((sa[a] >> 6) ^ sa[b]) & 16) != 0
#define down3up3(a,b) (((sa[a] >> 6) ^ sa[b]) & 32) != 0
///////////////////////////////////////////////////////////////////////////////////////////////////////

#define ml_left_val(a,b) ((((sa[get_pos(a - 1, b)] >> 6) ^ sa[get_pos(a, b)]) & 1) != 0 ? 1 : 0)
#define ml_left_val2(a,b) ((((sa[get_pos(a - 2, b)] >> 6) ^ sa[get_pos(a, b)]) & 2) != 0 ? 1 : 0)
#define ml_left_val3(a,b) ((((sa[get_pos(a - 3, b)] >> 6) ^ sa[get_pos(a, b)]) & 4) != 0 ? 1 : 0)

#define ml_up_val(a,b) ((((sa[get_pos(a, b - 1)] >> 6) ^ sa[get_pos(a, b)]) & 8) != 0 ? 1 : 0)
#define ml_up_val2(a,b) ((((sa[get_pos(a, b - 2)] >> 6) ^ sa[get_pos(a, b)]) & 16) != 0 ? 1 : 0)
#define ml_up_val3(a,b) ((((sa[get_pos(a, b - 3)] >> 6) ^ sa[get_pos(a, b)]) & 32) != 0 ? 1 : 0)
///////////////////////////////////////////////////////////////////////////////////////////////////////



int count_bits(int val){

	if(val == 0) return 0;
	if(val < 3)return 1;
	
	return 2;
	
}

int calc_s(shape_s *sa, int current_board_side_size){
	int x,y;
	unsigned int shape_val;
	 
	unsigned int s_val_norm = 0;
	
	for(y = 0; y < current_board_side_size; y++){
		for(x = 0; x < current_board_side_size; x++){
			shape_val = sa[get_pos(x,y)].val;
			if(shape_val != 0){
		
				/*
				s_max_norm += count_bits(get_lfm(shape_val));
				s_max_norm += count_bits(get_ufm(shape_val));
				s_max_norm += count_bits(get_rfm(shape_val));
				s_max_norm += count_bits(get_dfm(shape_val));
				*/
				
				if(x > 0 && (shape_val & 1) != 0) s_val_norm += left_val(x,y);
				if(x > 1 && (shape_val & 2) != 0) s_val_norm += left_val2(x,y);
				if(x > 2 && (shape_val & 4) != 0) s_val_norm += left_val3(x,y);
				if(y > 0 && (shape_val & 8) != 0) s_val_norm += up_val(x,y);
				if(y > 1 && (shape_val & 16) != 0) s_val_norm += up_val2(x,y);
				if(y > 2 && (shape_val & 32) != 0) s_val_norm += up_val3(x,y);
				if(x < current_board_side_size - 1 && (shape_val & 64) != 0) s_val_norm += right_val(x,y);
				if(x < current_board_side_size - 2 && (shape_val & 128) != 0) s_val_norm += right_val2(x,y);
				if(x < current_board_side_size - 3 && (shape_val & 256) != 0) s_val_norm += right_val3(x,y);
				if(y < current_board_side_size - 1 && (shape_val & 512) != 0) s_val_norm += down_val(x,y);
				if(y < current_board_side_size - 2 && (shape_val & 1024) != 0) s_val_norm += down_val2(x,y);
				if(y < current_board_side_size - 3 && (shape_val & 2048) != 0) s_val_norm += down_val3(x,y);
				
			}
		}
	}
	return s_val_norm;
}

bool check_mark(unsigned int *m, unsigned char idx, unsigned char val){
	for(int i = 0; i < idx; i++)
		if(m[i] == val) return false;
		
	return true;
}

uint64_t hit_milestone;
time_t time_report_middles_per_sec;

bool construct_sol(unsigned int *ssa, unsigned char idx, unsigned char level, unsigned int *sa,  unsigned int *mark, time_t s_time, time_t max_run_time, int current_board_side_size){

	//unsigned int *h_SA = sa;
	
	time_t tdiff;
	//int current_board_side_size = 4;
	int current_board_size = current_board_side_size * current_board_side_size;

	
	time_t now_time = time(NULL);
	
	tdiff = now_time - s_time;

	if(max_run_time > 0 && tdiff > max_run_time) return false;
	//if(idx == current_board_size * 2/ 3) hit_milestone++;
	if(now_time > time_report_middles_per_sec + 10){
		//printf("%lu milestones per sec\n", hit_milestone/10);
		hit_milestone = 0;
		time_report_middles_per_sec = now_time;
		//if((row_idx % 1000) == 0) 
		boost::this_thread::interruption_point();
	}
	
	
	if(level > 0){
		for(int i = 0; i < current_board_size; i++){
			// tdiff = time(NULL) - s_time;
			// if(max_run_time > 0 && tdiff > max_run_time) return false;
			if( check_mark(mark, idx, i)){
				sa[idx] = ssa[i];
				//sa_pos[idx] = i;
				mark[idx] = i;
				if(level == current_board_size){
					//printf("At first shape: %d, max_run_time: %lu, run_time: %ld sec\n", i, max_run_time, (long )tdiff);
					//printf("At first shape: %d\n", i);
					//printf("At first shape: %d, max_run_time: %lu, ", i, max_run_time);
					//std::cout << "run_time: " << tdiff << "\n";
				}
				int x = idx % current_board_side_size;
				int y = idx / current_board_side_size;
				if(x > 0 && ml_left_val(x,y)) continue;
				if(x > 1 && ml_left_val2(x,y)) continue;
				if(x > 2 && ml_left_val3(x,y)) continue;
				
				if(y > 0 && ml_up_val(x,y)) continue;
				if(y > 1 && ml_up_val2(x,y)) continue;
				if(y > 2 && ml_up_val3(x,y)) continue;
				
				if( construct_sol(ssa, idx + 1, level -1, sa, mark, s_time, max_run_time, current_board_side_size) )return true;
			}
		}	
	}else{
		
		//unsigned char marksa[128];
		#if 0		
		ofstream myfile;
		char pbuf[1024];
		
		printf("Success!\n");
		myfile.open("sol.txt",std::ofstream::out);
		printf("Sol: ");
		for(int a = 0; a < current_board_size - 1; a++){
			printf("%04x",sa[a]);
			sprintf(pbuf,"%d ", sa[a]);
			myfile << pbuf;
		}
		printf("%04x",sa[current_board_size - 1]);
		sprintf(pbuf,"%d", sa[current_board_size - 1]);
		myfile << pbuf;
		myfile << "\n";	
		myfile.close();	
		printf("\n");
		
		myfile.open("sol_hist.txt",std::ofstream::out|std::ofstream::app);
		for(int a = 0; a < current_board_size; a++){
			sprintf(pbuf,"%04x", sa[a]);
			myfile << pbuf;
		}
		myfile  << " in " << tdiff << " sec since start\n";							
		myfile.close();
		#endif
		return true;		
	}
	return false;
}


/********************************************************************************************

*********************************************************************************************/
void uint256tochar(uint256 idata, unsigned char *odata){
	memcpy(odata, &idata, 32);
}

/********************************************************************************************
g-mix	
*********************************************************************************************/
uint256 g_mix(uint256 hash){
	unsigned char idata[96], tdata[96], odata[32];
	uint256 t, t2;
	int i;
	//t1 = hash;
	
	uint32_t nNonce = 0x5a77dda5;
	
	uint256tochar(hash, idata);
	
	CHash256 hasher;
	hasher.Reset();			
	hasher.Write((unsigned char*)&hash, 32);
	hasher.Write((unsigned char*)&nNonce, 4);
	hasher.Write((unsigned char*)&hash, 32);
	CHash256(hasher).Finalize((unsigned char*)&t);
	#ifdef GAME_DEBUG
	std::cout << "g_mix->First t: "<< t.ToString() << "\n";
	#endif
	uint256tochar(t, &idata[32]);

	hasher.Reset();			
	hasher.Write((unsigned char*)&t, 32);
	hasher.Write((unsigned char*)&nNonce, 4);
	hasher.Write((unsigned char*)&t, 32);
	CHash256(hasher).Finalize((unsigned char*)&t2);
	#ifdef GAME_DEBUG
	std::cout << "g_mix->Second t: "<< t2.ToString() << "\n";
	#endif

	uint256tochar(t2, &idata[64]);
	
	memcpy(tdata, idata, 96);
	
	#ifdef GAME_DEBUG
	printf("idata before mix:\n");
	for(i = 0; i < 96; i++)
		printf("%02x ", idata[i]);
	printf("\n");
	#endif
	
	for(i = 0; i < 96; i++)
		idata[i] += tdata[95 - i] ^ 0xa5;
	
	#ifdef GAME_DEBUG
	//std::cout << "g_mix->idata: "<< idata.ToString() << "\n";'
	printf("idata:\n");
	for(i = 0; i < 96; i++)
		printf("%02x ", idata[i]);
	printf("\n");
	#endif
	
	//mscrypt

	if(! scrypt_run(idata, odata)){
		return (uint256 ) 0;
	}	


	#ifdef GAME_DEBUG
	printf("odata:\n");
	for(i = 0; i < 32; i++)
		printf("%02x ", odata[i]);
	printf("\n");
	#endif	
	
	uint256tochar(hash, tdata);
	t = 0;
	for(i = 0; i < 32; i++){
		t *= 256;
		t += (((odata[i] + 0x77) ^ tdata[31 -i]) & 0xff);
	}
	#ifdef GAME_DEBUG
	std::cout << "g_mix->Return t: "<< t.ToString() << "\n";
	#endif
	
	return t;
}


/********************************************************************************************
get_min_whites() // connected with lut in pow
*********************************************************************************************/

int fix_target( int target){
	if(target < 0x400)
		return 0x400;
	
	if(target > 0x4000)
		return 0x4000;
	
	return target;

}

int get_target_minor(int target){
	
	return (fix_target(target) & 0xff);
}

int get_target_major(int target){
	
	return ((fix_target(target) >> 8) & 0xff);
}

int get_brd_size(int target){
	return get_target_major(target) * get_target_major(target);
}


#define SIZEOF_ARRAY( a ) (sizeof( a ) / sizeof( a[ 0 ] ))

int minor_diff_whites_lut[] = {1, 2, 4, 5, 6, 7, 8, 9};

int get_min_whites( int target){

	//int fixed_target = fix_target(target) >> 8;
	//int target_major = (fixed_target >> 8) & 0xff;
	unsigned int target_minor = get_target_minor(target);
	int brd_size = get_brd_size(target);
	
	if(target_minor > SIZEOF_ARRAY(minor_diff_whites_lut) - 1)
		target_minor = SIZEOF_ARRAY(minor_diff_whites_lut) - 1;
	//if(target_minor <= 0) target_minor = 0;

	return minor_diff_whites_lut[target_minor] * brd_size / 25;
	
}

int get_max_whites( int target){

	//int fixed_target = fix_target(target) >> 8;
	//int target_major = (fixed_target >> 8) & 0xff;
	//int target_minor = (fixed_target & 0xff);
	
	return get_brd_size(target) / 3 + 1;
}


uint64_t perm_count = 0;
uint64_t perm_nr = 1;

/********************************************************************************************
check sol from disk or net
*********************************************************************************************/

bool game_check_sol(uint256 block_hash, unsigned int nBits, GSol sol){
//bool game_check_sol(uint256 block_hash, unsigned int nBits, GSol sol, unsigned int height){
	
	shape_s shape_arry[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE], shape_arry_orig[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE];	
	uint256 in_hash, t, t2;

	int current_board_side_size = 4;
	
	
	
	//#define DEBUG_DUMP_BLOCK_SOLS
	#ifdef DEBUG_DUMP_BLOCK_SOLS
	ofstream myfile;
	char pbuf2[256];
	#endif
	
	#ifdef GAME_DEBUG	
	char pbuf[256];
	int pbuf_idx = 0;
	pbuf[0] = 0;
	int shape_val = 0;
	#endif
	//char buf[1024];
	//int i;
	std::stringstream ss;
	//CHash256 hasher;
	uint32_t nNonce = 0xa5a5a5a5;
	//shape_s temp_shape;
    int brd_size = 4 * 4;
	int s_val, white_count = 0;
	
	
	current_board_side_size = get_target_major(nBits);
	if(current_board_side_size < 4) current_board_side_size = 4;
	if(current_board_side_size > 64) current_board_side_size = 64;
    brd_size = get_brd_size(nBits);	
	

	
	#ifdef GAME_DEBUG
	int current_board_size = 16;
	current_board_size = brd_size;
	int s_max;
	std::cout << "MAX_SHAPE_ENCODE: "<< MAX_SHAPE_ENCODE << "\n";
	std::cout <<"MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES: " <<  MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES << "\n";
	#endif
	
	in_hash = block_hash;
	#ifdef GAME_DEBUG
	std::cout << "game_check_sol->Orig hash: "<< in_hash.ToString() << "\n";
	#endif
	in_hash = g_mix(in_hash);
	if(in_hash == (uint256 ) 0){
		//printf("game_check_sol() in_hash null!!\n");
		//printf("game_check_sol(): nBits :%d, current_board_side_size: %d, current_board_size: %d\n",
		//nBits, current_board_side_size, current_board_size);
		return false;
	}
	#ifdef GAME_DEBUG
	std::cout << "game_check_sol->Orig hash g-mixed: "<< in_hash.ToString() << "\n";
	#endif
	int shapes_to_gen = brd_size;
	int max_encode = MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES;
	#ifdef GAME_DEBUG
	std::cout << "max_encode: " << max_encode << "\n";
	std::cout << "shapes_to_gen: "<< shapes_to_gen << "\n";
	#endif
	int shape_count = 0;

	//myfile.open ("shape_dump.txt");
	//myfile << current_board_side_size << "\n";

	t2 = in_hash;
	
	//unsigned int temp;
	while(shapes_to_gen){
		
		if(in_hash == uint256(0)){
			#ifdef GAME_DEBUG
			printf("NEED_NEW_DATA_FROM_HASH! Regen(shapes_to_gen: %d)\n", shapes_to_gen);
			#endif
			
			//exit(1);
			CHash256 hasher;
			hasher.Reset();			
			hasher.Write((unsigned char*)&t2, 32);
			hasher.Write((unsigned char*)&nNonce, 4);
			hasher.Write((unsigned char*)&t2, 32);
			CHash256(hasher).Finalize((unsigned char*)&in_hash);
			#ifdef GAME_DEBUG
			std::cout << "Regen hash: "<< in_hash.ToString() << "\n";
			#endif
			t2 = in_hash;
		}

		t = in_hash;
		in_hash /= max_encode;
		t -= in_hash * max_encode;
		int modulo1;
		ss << std::hex << t.ToString();
		//ss >> modulo1;
		//modulo1 = std::stoul(t.ToString(), nullptr, 16);
		//modulo1 = t;
		modulo1 = strtol( t.ToString().c_str(), (char **)0, 16 );
		#ifdef GAME_DEBUG
		//std::cout << "modulo1: "<< modulo1 << "\n";
		//printf("shape_count: %d, modulo1: %s\n", shape_count, t.ToString().c_str());
		printf("shape_count: %d, modulo1: %d\n", shape_count, modulo1);
		#endif
		
		shape_arry[shape_count].vpos = shape_count;
		if(t >=MAX_SHAPE_ENCODE){
			//std::cout << "white_shape\n";		
			shape_arry[shape_count].white = 1;
			shape_arry[shape_count].vpos = shape_count;
			shape_arry[shape_count].val = 0;
			//myfile << shape_count << " -1 -1 \n"; 
			white_count++;
	
			#ifdef GAME_DEBUG
			sprintf(&pbuf[pbuf_idx], "%04x ", 0);
			pbuf_idx += 5;
			#endif
		}else{
			shape_arry[shape_count].white = 0;
			int side, ff;
			//shape_val = 0;
			//shape_arry[shape_count].val = 0;
			for(side = 0; side < 4; side ++){
				ff = modulo1 % 8;
				modulo1 /= 8;
				#ifdef GAME_DEBUG
				printf("side: %d, ff: %d, modulo1: %d\n", side, ff, modulo1);
				#endif
				//myfile << shape_count << " " << side << " " << ff << "\n"; 
				//shape_val <<= 3;
				//shape_val |= ff;
				switch (side){
					case 0:
						shape_arry[shape_count].lc = 1;
						shape_arry[shape_count].lfm = ff;
						shape_arry[shape_count].lfv = ff;
						
					case 1:
						shape_arry[shape_count].uc = 1;
						shape_arry[shape_count].ufm = ff;
						shape_arry[shape_count].ufv = ff;

					case 2:
						shape_arry[shape_count].rc = 1;
						shape_arry[shape_count].rfm = ff;
						shape_arry[shape_count].rfv = ff;

					default:
						shape_arry[shape_count].dc = 1;
						shape_arry[shape_count].dfm = ff;
						shape_arry[shape_count].dfv = ff;					
				}
				
				//shape_arry[shape_count].val <<= 3;
				//shape_arry[shape_count].val |= ff;
			}
			shape_arry[shape_count].val = ( shape_arry[shape_count].dfm << 9) | (shape_arry[shape_count].rfm << 6) | (shape_arry[shape_count].ufm << 3) | shape_arry[shape_count].lfm;
			#ifdef GAME_DEBUG
			sprintf(&pbuf[pbuf_idx], "%04x ", shape_arry[shape_count].val);
			pbuf_idx += 5;
			#endif
		}
		shape_count++;
		shapes_to_gen--;
	}
	//int x = 1;
	//if(nBits < 5) x = 0;
	//if(white_count < brd_size / 8 + x || white_count > brd_size / 3 + 1){
	#ifdef GAME_DEBUG
	printf("game_check_sol() nBits: %04x, min_whites: %d, max_whites: %d\n", nBits, get_min_whites(nBits), get_max_whites(nBits));
	#endif
	if(white_count < get_min_whites(nBits) || white_count > get_max_whites(nBits)){
		
		#ifdef GAME_DEBUG
		std::cout << "game_check_sol()->sol REJECTED BY RULES ( whites: " << white_count << ")\n";
		#endif				
		//printf("game_check_sol(): nBits :%d, current_board_side_size: %d, current_board_size: %d\n",
		//nBits, current_board_side_size, current_board_size);	
		return false;
	}	
	//myfile.close();
	for(int vi =0; vi < shape_count; vi++)
		memcpy(&shape_arry_orig[vi], &shape_arry[vi], sizeof(shape_s));
	#ifdef GAME_DEBUG
	std::cout << "sol dump: ";
	#endif
	for(int vi =0; vi < shape_count; vi++){
		// temp_shape = shape_arry[vi];
		// shape_arry[vi] = shape_arry[sol->GetVal(vi)];
		// shape_arry[sol->SetVal(vi,temp_shape)
		#ifdef GAME_DEBUG
		std::cout << sol.GetVal(vi) << " ";
		#endif
		int sol_vi = sol.GetVal(vi) - 1;
		if(sol_vi < 0 && sol_vi > shape_count - 1){
			//printf("game_check_sol() err, sol_vi: %d, shape_count: %d\n", sol_vi, shape_count);
			
			//printf("game_check_sol(): nBits :%d, current_board_side_size: %d, current_board_size: %d\n",
			//nBits, current_board_side_size, current_board_size);		
			
			return false; //err!!! may crash daemon
		}
		memcpy(&shape_arry[vi], &shape_arry_orig[sol_vi], sizeof(shape_s));
		
	}
	#ifdef GAME_DEBUG
	std::cout << "\n";
	pbuf[pbuf_idx] = 0;
	//std::cout << "sol_buf: " << pbuf << "whites: " << white_count << " block_height: " << height << "\n";
	std::cout << "sol_buf: " << pbuf << "whites: " << white_count << "\n";
	#endif
	s_val = calc_s(&shape_arry[0], current_board_side_size);	
	if(s_val == 0){
		
		
		#ifdef DEBUG_DUMP_BLOCK_SOLS
		myfile.open("sol_check_hist.txt",std::ofstream::out|std::ofstream::app);
		myfile << "Block: " << block_hash.ToString() << " Sol: ";
		for(int i = 0; i < brd_size; i++){
			sprintf(pbuf2, "%04x", shape_arry[i].val);
			myfile << pbuf2;
		}
		myfile << "\n";
		myfile.close();
		#endif
		
		#ifdef GAME_DEBUG
		std::cout << "game_check_sol()->sol accepted\n";
		#endif
		return true;
	}
	#ifdef GAME_DEBUG
	std::cout << "game_check_sol()->sol not verifying\n";
	printf("game_check_sol(): nBits :%d, current_board_side_size: %d, current_board_size: %d\n",
		nBits, current_board_side_size, current_board_size);	
	#endif
	return false;

}

/********************************************************************************************
miner
*********************************************************************************************/
bool game_run_main(uint256 *block_hash, unsigned int nBits, GSol *sol){
	shape_s shape_arry[BOARD_MAX_SIDE_SIZE * BOARD_MAX_SIDE_SIZE];
	uint256 in_hash, t, t2;
	
	int current_board_side_size = 4;
	int current_board_size = 16;

	
	std::stringstream ss;
	
	uint32_t nNonce = 0xa5a5a5a5;
	ofstream myfile;
    int brd_size = 4 * 4;
	int white_count = 0;

	current_board_side_size = get_target_major(nBits);
	if(current_board_side_size < 4) current_board_side_size = 4;
	if(current_board_side_size > 64) current_board_side_size = 64;
    brd_size = get_brd_size(nBits);	
	current_board_size = brd_size;
	
	
	in_hash.SetHex(block_hash->GetHex());
	//in_hash = block_hash;
	#ifdef GAME_DEBUG
	int s_val, s_max;
	std::cout << "MAX_SHAPE_ENCODE: "<< MAX_SHAPE_ENCODE << "\n";
	std::cout <<"MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES: " <<  MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES << "\n";
	#endif

	#ifdef GAME_DEBUG
	std::cout << "game_run_main()->Orig hash: "<< in_hash.ToString() << "\n";
	#endif
	in_hash = g_mix(in_hash);
	if(in_hash == (uint256 ) 0)
		return false;
	#ifdef GAME_DEBUG
	std::cout << "game_check_sol->Orig hash g-mixed: "<< in_hash.ToString() << "\n";
	#endif
	int shapes_to_gen = brd_size;
	int max_encode = MAX_SHAPE_ENCODE_WITH_WHITE_SHAPES;
	#ifdef GAME_DEBUG
	std::cout << "shapes_to_gen: "<< shapes_to_gen << "\n";
	#endif
	int shape_count = 0;

	#ifdef GAME_SOL_VIA_FILE
	myfile.open ("shape_dump.txt");
	myfile << current_board_side_size << "\n";
	#endif
	t2 = in_hash;
	while(shapes_to_gen){
		
		if(in_hash == uint256(0)){
			#ifdef GAME_DEBUG
			printf("NEED_NEW_DATA_FROM_HASH! Regen(shapes_to_gen: %d)\n", shapes_to_gen);
			#endif
			
			//exit(1);
			CHash256 hasher;
			hasher.Reset();
			hasher.Write((unsigned char*)&t2, 32);
			hasher.Write((unsigned char*)&nNonce, 4);
			hasher.Write((unsigned char*)&t2, 32);
			CHash256(hasher).Finalize((unsigned char*)&in_hash);
			#ifdef GAME_DEBUG
			std::cout << "Regen hash: "<< in_hash.ToString() << "\n";
			#endif
			t2 = in_hash;
		}

		t = in_hash;
		in_hash /= max_encode;
		t -= in_hash * max_encode;
		int modulo1;
		ss << std::hex << t.ToString();
		//ss >> modulo1;
		//modulo1 = std::stoul(t.ToString(), nullptr, 16);
		//modulo1 = t;
		modulo1 = strtol( t.ToString().c_str(), (char **)0, 16 );
		#ifdef GAME_DEBUG
		//std::cout << "modulo1: "<< modulo1 << "\n";
		//printf("shape_count: %d, modulo1: %s\n", shape_count, t.ToString().c_str());
		printf("shape_count: %d, modulo1: %d\n", shape_count, modulo1);
		#endif
		shape_arry[shape_count].vpos = shape_count; //redundant
		if(t >=MAX_SHAPE_ENCODE){
			//std::cout << "white_shape\n";		
			shape_arry[shape_count].white = 1;
			//shape_arry[shape_count].vpos = shape_count;
			shape_arry[shape_count].val = 0;
			#ifdef GAME_SOL_VIA_FILE
			myfile << shape_count << " -1 -1 \n"; 
			#endif
			white_count++;
	
		}else{
			shape_arry[shape_count].white = 0;
			int side, ff;
			for(side = 0; side < 4; side ++){
				ff = modulo1 % 8;
				modulo1 /= 8;
				#ifdef GAME_DEBUG
				printf("side: %d, ff: %d, modulo1: %d\n", side, ff, modulo1);
				#endif
				#ifdef GAME_SOL_VIA_FILE
				myfile << shape_count << " " << side << " " << ff << "\n"; 
				#endif
				switch (side){
					case 0:
						shape_arry[shape_count].lc = 1;
						shape_arry[shape_count].lfm = ff;
						shape_arry[shape_count].lfv = ff;
						
					case 1:
						shape_arry[shape_count].uc = 1;
						shape_arry[shape_count].ufm = ff;
						shape_arry[shape_count].ufv = ff;

					case 2:
						shape_arry[shape_count].rc = 1;
						shape_arry[shape_count].rfm = ff;
						shape_arry[shape_count].rfv = ff;

					default:
						shape_arry[shape_count].dc = 1;
						shape_arry[shape_count].dfm = ff;
						shape_arry[shape_count].dfv = ff;					
				}
				
				//shape_arry[shape_count].val <<= 3;
				//shape_arry[shape_count].val |= ff;
			}
			shape_arry[shape_count].val = ( shape_arry[shape_count].dfm << 9) | (shape_arry[shape_count].rfm << 6) | (shape_arry[shape_count].ufm << 3) | shape_arry[shape_count].lfm;
		}
		shape_count++;
		shapes_to_gen--;
	}
	//int x = 1;
	//if(nBits < 5) x = 0;
	//if(white_count < brd_size / 8 + x || white_count > brd_size / 3 + 1){
	#ifdef GAME_DEBUG
	printf("game_run_main() nBits: %04x, min_whites: %d, max_whites: %d\n", nBits, get_min_whites(nBits), get_max_whites(nBits));
	#endif
	if(white_count < get_min_whites(nBits) || white_count > min(get_min_whites(nBits) + 1, get_max_whites(nBits)) ){
		
		#ifdef GAME_DEBUG
		std::cout << "game_run_main()->config REJECTED BY RULES ( whites: " << white_count << ")\n";
		#endif				
		
		return false;
	}	
	#ifdef GAME_SOL_VIA_FILE
	myfile.close();
	#endif
	calc_s(&shape_arry[0], current_board_side_size);
	#ifdef GAME_DEBUG
	//std::cout << "s_val: " << s_val << " s_max: " << s_max << "\n";
	std::time_t start_time = std::time(0);
	#endif
	
	//unsigned int *h_SA = (unsigned int *)malloc(1024 * sizeof(int));
	unsigned int h_SA[64 * 64 + 16];
	
	/*
	if(h_SA == NULL){
		printf("game_run_main() h_SA malloc() failed\n");
		return false;
	}
	*/
	
	for(int i = 0; i < brd_size; i++)
		h_SA[i] = shape_arry[i].val;
	//// h_SA got data
	
	bool found = false;

	// use "memoryless" alg
	unsigned int sol_SA[64 * 64 + 16];
	unsigned int sol_SA_pos[64 * 64 + 16];
	//unsigned char mark[64 * 64 + 16];

	if(construct_sol(h_SA, 0, current_board_size, sol_SA, sol_SA_pos, time(NULL), 0, current_board_side_size)){
		//printf("Sol found! Check sol.txt and sol_hist.txt\n");
		found = true;
	}
	//else
	//	printf("Sol not found or time out.\n");	
	
	if(found){
		

		#ifdef GAME_DEBUG
		std::time_t diff_time = std::time(0) - start_time;
		std::cout << "SOLUTION FOUND, after " << perm_count << " permutations in " << diff_time << "s, Solution dumped in file\n";
		#endif
		#ifdef GAME_SOL_VIA_FILE
		myfile.open ("shape_dump_sol.txt");
		myfile << current_board_side_size << "\n";
		#endif
		shape_count = 0;
		shapes_to_gen = brd_size;
		if(0)
		while(shapes_to_gen){
			#ifdef GAME_SOL_VIA_FILE
			if(shape_arry[shape_count].white == 1){
				myfile << shape_count << " -1 -1 \n";
			}else{
				myfile << shape_count << " 0 " << shape_arry[shape_count].lfm << "\n";
				myfile << shape_count << " 1 " << shape_arry[shape_count].ufm << "\n";
				myfile << shape_count << " 2 " << shape_arry[shape_count].rfm << "\n";
				myfile << shape_count << " 3 " << shape_arry[shape_count].dfm << "\n";
			}
			#endif
			shapes_to_gen--;
			shape_count++;
		}
		#ifdef GAME_SOL_VIA_FILE
		myfile.close();
		#endif
		//int idx = 0;
		int vi;
		#ifdef GAME_DEBUG
		std::cout << "sol dump: ";
		#endif
		
		//#ifndef USE_VECTOR_ALG
		//swap i and sol[i]
		// unsigned int sol_SA_pos_sw[64 * 64 + 16];
		// for(vi=0; vi < brd_size; vi++){
			// sol_SA_pos_sw[sol_SA_pos[vi]] = vi;
		// }
		// #endif
		
		for(vi=0; vi < brd_size; vi++){
			/*
			for(int vj=0; vj < brd_size; vj++){
				if(shape_arry[vj].vpos == vi){
					sol->SetVal(vj, vi + 1);
					#ifdef GAME_DEBUG
					std::cout << vj << " ";
					#endif
					break;
				}
			}
			*/
			sol->SetVal(vi, sol_SA_pos[vi] + 1);
		}
		sol->SetVal(brd_size,0);
		#ifdef GAME_DEBUG
		std::cout << "\n";
		#endif
		return true;
	}else{
		#ifdef GAME_DEBUG
		std::cout << "END GAME, NO SOLUTION FOUND\n";
		#endif
	}

	//// end ////
	return false;
}

#endif
