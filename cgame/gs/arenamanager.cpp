//------------------------------------------------------------------------------------------------------------------------
//--ARENA AURURE (C) 2022 deadraky
//------------------------------------------------------------------------------------------------------------------------
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "arenamanager.h"
#include "gsp_if.h"

ArenaConfig* ArenaConfig::instance = NULL;

void ArenaConfig::Init()
{
	memset(this,0x00,sizeof(ArenaConfig));
	
	// req group configs
	int group_configs[] = { GROUP_IDX0, GROUP_IDX1, /*GROUP_IDX2, GROUP_IDX3,*/ 0 };
	for(unsigned int i = 0; i < GROUP_MAX; i++)
	{
		DATA_TYPE dt;
		ARENA_CONFIG *it = (ARENA_CONFIG*)world_manager::GetDataMan().get_data_ptr(group_configs[i],ID_SPACE_CONFIG,dt);
		if (it && dt == DT_ARENA_CONFIG)
		{
			req[i].level 		= it->require_level;
			req[i].create_money	= it->team_create_cost;
			req[i].battle_money	= it->battle_match_cost;
			req[i].item 		= it->require_item_id;
		}
		else
		{
			printf("ARENA_CONFIG INIT FAILED!!! \n");
			ASSERT(false);
			return;
		}
	}

	// tags teleports config
	int tags_configs[] = { TAG_IDX0, TAG_IDX1, TAG_IDX2, TAG_IDX3, 0 };
	for(unsigned int i = 0; i < TAG_MAX; i++)
	{
		DATA_TYPE dt;
		ARENA_SCENE_CONFIG *it = (ARENA_SCENE_CONFIG*)world_manager::GetDataMan().get_data_ptr(tags_configs[i],ID_SPACE_CONFIG,dt);
		if (it && dt == DT_ARENA_SCENE_CONFIG)
		{
			tag[i].tag			= it->area_id;
			tag[i].pos[0].x		= it->trans_pos_1_1;
			tag[i].pos[0].y		= it->trans_pos_1_2;
			tag[i].pos[0].z		= it->trans_pos_1_3;
			tag[i].pos[1].x		= it->trans_pos_2_1;
			tag[i].pos[1].y		= it->trans_pos_2_2;
			tag[i].pos[1].z		= it->trans_pos_2_3;
			tag[i].player_count	= it->teammate_max_count;
			tag[i].player_level	= it->require_level;
		}
		else
		{
			printf("ARENA_SCENE_CONFIG INIT FAILED!!! \n");
			ASSERT(false);
			return;
		}
	}
	
	//rank score config
	DATA_TYPE dt;
	ARENA_POINT_CONFIG *it = (ARENA_POINT_CONFIG*)world_manager::GetDataMan().get_data_ptr(SCORE_CONFIG_IDX1,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_ARENA_POINT_CONFIG)
	{
		for(unsigned int i = 0; i < RANK_MAX; i++)
		{
			rank[i].rank 		= it->rank[i].unk1;
			rank[i].score		= it->rank[i].points_min;
			rank[i].max_score	= it->rank[i].points_max;
		}
	}
	else
	{
		printf("ARENA_POINT_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}
	
	//forbid item config
	DATA_TYPE dt2;
	ARENA_FORBID_ITEM_CONFIG *it2 = (ARENA_FORBID_ITEM_CONFIG*)world_manager::GetDataMan().get_data_ptr(FORBID_ITEM_IDX,ID_SPACE_CONFIG,dt2);
	if (it2 && dt2 == DT_ARENA_FORBID_ITEM_CONFIG)
	{
		for(unsigned int i = 0; i < FORBID_MAX; i++)
		{
			forbid[i].item_id = it2->items[i];
		}
	}
	else
	{
		printf("ARENA_FORBID_ITEM_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}

	//forbid skill config
	DATA_TYPE dt3;
	ARENA_FORBID_SKILL_CONFIG *it3 = (ARENA_FORBID_SKILL_CONFIG*)world_manager::GetDataMan().get_data_ptr(FORBID_SKILL_IDX,ID_SPACE_CONFIG,dt3);
	if (it2 && dt3 == DT_ARENA_FORBID_SKILL_CONFIG)
	{
		for(unsigned int i = 0; i < FORBID_MAX; i++)
		{
			forbid[i].skill_id = it3->skills[i];
		}
	}
	else
	{
		printf("ARENA_FORBID_SKILL_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}
		
}

int ArenaConfig::GetTag()
{
	return tag[ rand() % TAG_MAX ].tag;
}

A3DVECTOR ArenaConfig::GetPos(int _tag, bool red)
{
	A3DVECTOR pos(0,0,0);
	for(unsigned int i = 0; i < TAG_MAX; i++)
	{
		if (tag[i].tag == _tag)
		{
			pos.x = abase::Rand(tag[i].pos[red].x - 1, tag[i].pos[red].x + 1);
			pos.y = tag[i].pos[red].y;
			pos.z = abase::Rand(tag[i].pos[red].z - 1, tag[i].pos[red].z + 1);
			break;
		}
	}
	return pos;
}


int ArenaConfig::GetMoneyRequired(int mode)
{
	switch(mode)
	{
	case 0:
		return req[0].create_money;
	case 1:
		return req[1].create_money;
	default:
		return 0;
	}
}



