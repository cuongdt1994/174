#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "celestial_memorial_manager.h"
#include "gsp_if.h"

CelestialMemorialConfig* CelestialMemorialConfig::instance = NULL;

void CelestialMemorialConfig::Init()
{
	memset(this,0x00,sizeof(CelestialMemorialConfig));
	
	int IDX_LOTTERY_RED_BOX[] = {6092, 6099, 6100, 0};

	DATA_TYPE dt;
	RED_BOOK_CONFIG *it = (RED_BOOK_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_RED_BOX,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_RED_BOOK_CONFIG)
	{
		// Open
		open_time.year = it->open_time_year;
		open_time.month = it->open_time_month;
		open_time.day = it->open_time_day;
		open_time.hour = it->open_time_hour;
		open_time.minute = it->open_time_minute;
		open_time.second = it->open_time_second;

		// Close

		close_time.year = it->close_time_year;
		close_time.month = it->close_time_month;
		close_time.day = it->close_time_day;
		close_time.hour = it->close_time_hour;
		close_time.minute = it->close_time_minute;
		close_time.second = it->close_time_second;

		// TaskDlgClose

		task_dlg_close_time.year = it->task_dlg_close_time_year;
		task_dlg_close_time.month = it->task_dlg_close_time_month;
		task_dlg_close_time.day = it->task_dlg_close_time_day;
		task_dlg_close_time.hour = it->task_dlg_close_time_hour;
		task_dlg_close_time.minute = it->task_dlg_close_time_minute;
		task_dlg_close_time.second = it->task_dlg_close_time_second;

		// EatClose

		eat_close_time.year = it->eat_close_time_year;
		eat_close_time.month = it->eat_close_time_month;
		eat_close_time.day = it->eat_close_time_day;
		eat_close_time.hour = it->eat_close_time_hour;
		eat_close_time.minute = it->eat_close_time_minute;
		eat_close_time.second = it->eat_close_time_second;


		// LevelConfig

		for (unsigned int i = 0; i < MAX_LEVEL_RED_BOX; i++)
		{
			level_config[i].item_id = it->level_award[i].item_id;
			level_config[i].item_count = it->level_award[i].item_count;
			level_config[i].award_type = it->level_award[i].award_type;
		}

		// VipLevelConfig

		for (unsigned int i = 0; i < MAX_LEVEL_RED_BOX; i++)
		{
			vip_level_config[i].item_id = it->vip_level_award[i].item_id;
			vip_level_config[i].item_count = it->vip_level_award[i].item_count;
			vip_level_config[i].award_type = it->vip_level_award[i].award_type;
		}


		interval_after_500 = it->interval_after_500;

		item_id_after_500 = it->item_id_after_500;
		item_count_after_500 = it->item_count_after_500;
		award_type_after_500 = it->award_type_after_500;

		vip_item_id_after_500 = it->vip_item_id_after_500;
		vip_item_count_after_500 = it->vip_item_count_after_500;
		vip_award_type_after_500 = it->vip_award_type_after_500;

	}
	else
	{
		printf("RED_BOOK_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}	

	for (unsigned int i = 0; i < LOTTERY_COUNT; i++)
	{
		DATA_TYPE dt2;
		RED_BOOK_LOTTERY_CONFIG *it2 = (RED_BOOK_LOTTERY_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_LOTTERY_RED_BOX[i],ID_SPACE_CONFIG,dt2);
		if (it2 && dt2 == DT_RED_BOOK_LOTTERY_CONFIG)
		{
			lottery_config[i].lottery_draw_lv = it2->lottery_draw_lv;
			lottery_config[i].cost_item_id = it2->cost_item_id;
			lottery_config[i].cost_item_count = it2->cost_item_count;

			lottery_config[i].open_time = it2->open_time;
			lottery_config[i].close_time = it2->close_time;

			for (unsigned int j = 0; j < RARE_ITEM_COUNT; j++)
			{
				lottery_config[i].award_pool_high[j].award_item_id = it2->award_pool_high[j].award_item_id;	
				lottery_config[i].award_pool_high[j].award_item_count = it2->award_pool_high[j].award_item_count;
				lottery_config[i].award_pool_high[j].award_item_probability = it2->award_pool_high[j].award_item_probability;			
			}

			for (unsigned int j = 0; j < MIDLE_ITEM_COUNT; j++)
			{
				lottery_config[i].award_pool_middle[j].award_item_id = it2->award_pool_middle[j].award_item_id;	
				lottery_config[i].award_pool_middle[j].award_item_count = it2->award_pool_middle[j].award_item_count;
				lottery_config[i].award_pool_middle[j].award_prob_init = it2->award_pool_middle[j].award_prob_init;			
				lottery_config[i].award_pool_middle[j].award_prob_add = it2->award_pool_middle[j].award_prob_add;			
				lottery_config[i].award_pool_middle[j].award_times_max = it2->award_pool_middle[j].award_times_max;			
			}

			for (unsigned int j = 0; j < COMMON_ITEM_COUNT; j++)
			{
				lottery_config[i].award_pool_low[j].award_item_id = it2->award_pool_low[j].award_item_id;	
				lottery_config[i].award_pool_low[j].award_item_count = it2->award_pool_low[j].award_item_count;
				lottery_config[i].award_pool_low[j].award_item_probability = it2->award_pool_low[j].award_item_probability;			
			}			

		}
		else
		{
			printf("RED_BOOK_LOTTERY_CONFIG INIT FAILED!!! \n");
			ASSERT(false);
			return;
		}
		
	}

}

