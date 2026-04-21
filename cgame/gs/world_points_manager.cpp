#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "world_points_manager.h"
#include "gsp_if.h"

WorldPointsConfig* WorldPointsConfig::instance = NULL;

void WorldPointsConfig::Init()
{
	memset(this,0x00,sizeof(WorldPointsConfig));
	
	DATA_TYPE dt;
	TASK_DICE_BY_WEIGHT_CONFIG *it = (TASK_DICE_BY_WEIGHT_CONFIG*)world_manager::GetDataMan().get_data_ptr(GROUP_IDX0,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_TASK_DICE_BY_WEIGHT_CONFIG)
	{
		max_points = it->max_weight;

		for (int i = 0; i < WORLD_POINT_LIST_COUNT; i++)
		{
			list[i].probability = it->uniform_weight_list[i].probability;
			list[i].point_award = it->uniform_weight_list[i].weight;
			list[i].require_task_count = it->uniform_weight_list[i].complete_num;
			list[i].money_award = it->uniform_weight_list[i].gold;
			list[i].realm_exp_award = it->uniform_weight_list[i].realm_exp;
			list[i].exp_award = it->uniform_weight_list[i].exp;
			list[i].sp_award = it->uniform_weight_list[i].sp;
			list[i].dividend_award = it->uniform_weight_list[i].dividend;
		}

		for (int i = 0; i < REWARD_MAX_COUNT; i++)
		{
			award[i].require_points = it->period_award[i].period;
			award[i].item_award_id = it->period_award[i].award;
			award[i].item_award_count = it->period_award[i].num;
		}

	}
	else
	{
		printf("TASK_DICE_BY_WEIGHT_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}	
}

