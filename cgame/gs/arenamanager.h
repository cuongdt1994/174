//------------------------------------------------------------------------------------------------------------------------
//--ARENA AURURE (C) 2022 deadraky
//------------------------------------------------------------------------------------------------------------------------
#ifndef __GNET_ARENAMANAGER_H
#define __GNET_ARENAMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class ArenaConfig
{
public:
	enum
	{
		GROUP_IDX0 = 2367,
		GROUP_IDX1 = 2381,
		//GROUP_IDX2 = 6107,
		//GROUP_IDX3 = 6108,

		GROUP_3X3 = 0,
		GROUP_6X6 = 1,
		//GROUP_3X3_TEMPLATE = 2,
		//GROUP_6X6_TEMPLATE = 3,
		GROUP_MAX = 2,
		
		TAG_IDX0 = 2388,
		TAG_IDX1 = 2574,
		TAG_IDX2 = 2575,
		TAG_IDX3 = 2576,
		
		TAG_179 = 0, //a79
		TAG_187 = 1, //a87
		TAG_188 = 2, //a88
		TAG_189 = 3, //a89
		TAG_MAX = 4,
		TAG_MAX_POS = 2,
		
		SCORE_CONFIG_IDX1 = 2366,
		FORBID_SKILL_IDX = 5089,
		FORBID_ITEM_IDX = 5090,
		
		RANK_MAX = 16,
		FORBID_MAX = 64,
		
	};
	static ArenaConfig * instance;

private:

	struct REQ
	{
		int level;
		int create_money;
		int battle_money;
		int item;
	};
	
	struct TAG
	{
		int tag;
		A3DVECTOR pos[TAG_MAX_POS];
		int player_count;
		int player_level;
	};
	
	struct RANK
	{
		int rank;
		int score;
		int max_score;
	};

	struct FORBID
	{
		int skill_id;
		int item_id;
	};


private:

	REQ req[GROUP_MAX];
	TAG tag[TAG_IDX3];
	RANK rank[RANK_MAX];
	FORBID forbid[FORBID_MAX];
	
public:
	void Init();
	int GetTag();
	A3DVECTOR GetPos(int tag, bool red);
	int GetMoneyRequired(int mode);
	
	
ArenaConfig()
{

}

~ArenaConfig()
{
	
}

static ArenaConfig * GetInstance()
{
	if (!instance)
	{
		instance = new ArenaConfig();
	}
	return instance;
}

};


#endif