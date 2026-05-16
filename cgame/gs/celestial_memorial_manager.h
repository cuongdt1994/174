#ifndef __GNET_CELESTIAL_MEMORIAL_MANAGER_H
#define __GNET_CELESTIAL_MEMORIAL_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class CelestialMemorialConfig
{
public:
	enum
	{
		IDX_RED_BOX = 6095,	
		MAX_LEVEL_RED_BOX = 500,
		MAX_LEVEL_RED_BOX_1 = 500,
		MAX_LEVEL_RED_BOX_FINISH = 500,
		LEVEL_ADD_RED_BOX_2 = 2,
		EXP_REQUIRED_PER_LEVEL = 1000,
		LOTTERY_COUNT = 3,
		TICKET_COUNT = 2,
		MAX_STORAGE_LOTTERY = 8,
		MAX_ITEM_LOTTERY = 12,

		RARE_ITEM_COUNT = 1,
		MIDLE_ITEM_COUNT = 4,
		COMMON_ITEM_COUNT = 8,

		COUNT_KEY_OPEN_RED_BOX = 2,
		COUNT_STONES_RED_BOX = 6,
		COUNT_LOTTERY_RED_BOX = 3,

		// Reward 

		TICKET_1_ID = 1,
		TICKET_2_ID = 2,

		PERG_1_LOTTERY_ID = 3,
		PERG_2_LOTTERY_ID = 4,
		PERG_3_LOTTERY_ID = 5,
		PERG_RANDOM_LOTTERY_ID = 6,
	};

	static CelestialMemorialConfig * instance;

private:

	struct LEVEL_CONFIG_RED_BOX
	{
		int item_id;
		int item_count;
		int award_type;
	};
		
	struct VIP_LEVEL_CONFIG_RED_BOX
	{
		int item_id;
		int item_count;
		int award_type;
	};

	struct OPEN_TIME
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	struct CLOSE_TIME
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	struct TASK_DLG_CLOSE_TIME
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	struct EAT_CLOSE_TIME
	{
		int year;
		int month;
		int day;
		int hour;
		int minute;
		int second;
	};

	struct LOTTERY_CONFIG
	{
		unsigned int lottery_draw_lv;
		int cost_item_id;
		int	cost_item_count;

		unsigned int open_time;
		unsigned int close_time;

		struct
		{
			int	award_item_id;
			int	award_item_count;
			float award_item_probability;

		}award_pool_high[RARE_ITEM_COUNT];

		struct
		{
			unsigned int award_item_id;
			int	award_item_count;
			float award_prob_init;
			float award_prob_add;
			int	award_times_max;

		}award_pool_middle[MIDLE_ITEM_COUNT];

		struct
		{
			unsigned int award_item_id;
			int	award_item_count;
			float award_item_probability;

		}award_pool_low[COMMON_ITEM_COUNT];
		
	};

private:

	OPEN_TIME open_time;
	CLOSE_TIME close_time;
	TASK_DLG_CLOSE_TIME task_dlg_close_time;
	EAT_CLOSE_TIME eat_close_time;

	LEVEL_CONFIG_RED_BOX level_config[MAX_LEVEL_RED_BOX];
	VIP_LEVEL_CONFIG_RED_BOX vip_level_config[MAX_LEVEL_RED_BOX];

	int interval_after_500;
	int item_id_after_500;
	int item_count_after_500;
	int award_type_after_500;

	int vip_item_id_after_500;
	int vip_item_count_after_500;
	int vip_award_type_after_500;

	LOTTERY_CONFIG lottery_config[LOTTERY_COUNT];
	
public:
	void Init();

	LEVEL_CONFIG_RED_BOX* GetLevel_1_Rewards(int pos) { return &level_config[pos]; }
	VIP_LEVEL_CONFIG_RED_BOX* GetLevel_2_Rewards(int pos) { return &vip_level_config[pos]; }

	LOTTERY_CONFIG* GetLotteryConfig(int pos) { return &lottery_config[pos]; }

	
CelestialMemorialConfig()
{

}

~CelestialMemorialConfig()
{
	
}

static CelestialMemorialConfig * GetInstance()
{
	if (!instance)
	{
		instance = new CelestialMemorialConfig();
	}
	return instance;
}

};


#endif