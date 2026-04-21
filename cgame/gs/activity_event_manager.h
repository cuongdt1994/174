#ifndef __GNET_ACTIVITY_EVENT_MANAGER_H
#define __GNET_ACTIVITY_EVENT_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class ActivityEventConfig
{
public:
	enum
	{
		IDX_ACTIVITY_EXPUP = 7028,	
		IDX_ACTIVITY_CONSUMPTION = 7030,
		IDX_ACTIVITY_SHOP = 7031,
		IDX_ACTIVITY_LOGIN_REWARD = 7024,

		MAX_CONSUMPTION_ITENS = 7,
		MAX_SHOP_ITENS = 16,
		MAX_CHALLENGE_COUNT = 43,

		MAX_REWARD_LOGIN_COUNT = 30,

		INDEX_REWARD_INTERFACE = 21,

	};
	static ActivityEventConfig * instance;

private:

	struct SHOP
	{
		int	item_id;
		int	buy_times_limit;
		int	buy_times_limit_mode;
		int	unk6;
		int price;
	};

	struct CONSUMPTION
	{
		int	require_rmb;
		int	item_id;
		int	item_count;
	};

	struct REWARD_LOGIN
	{
		unsigned int item_id;
		int	item_count;
		int	item_expire_date;
		unsigned int item_cooldown;
	};

	struct QUEST
	{
		int	unk2;
		int	reward_count;
		int	reward_id;
		int	task_id_space;
		int	task_id;
	};

private:

	unsigned int exp_bonus_max_reincarnation;
	int	exp_bonus_max_level;
	int	exp_bonus_rate;	
	unsigned int realm_exp_max_relm_level;
	int realm_exp_rate;

	SHOP shop_item[MAX_SHOP_ITENS];
	CONSUMPTION consumption_item[MAX_CONSUMPTION_ITENS];
	REWARD_LOGIN reward_login[MAX_REWARD_LOGIN_COUNT];
	QUEST quest_info;
	
public:
	void Init();
	SHOP * GetShop(int pos) { return &shop_item[pos]; }
	REWARD_LOGIN * GetRewardLogin(int pos) { return &reward_login[pos]; }

	inline QUEST * GetQuest(int idx) 
	{
		DATA_TYPE dt;
		ACTIVITY_CHALLENGE_CONFIG *it = (ACTIVITY_CHALLENGE_CONFIG*)world_manager::GetDataMan().get_data_ptr(idx,ID_SPACE_CONFIG,dt);
		if (it && dt == DT_ACTIVITY_CHALLENGE_CONFIG)
		{
			quest_info.unk2 = it->unk2;
			quest_info.reward_id = it->reward_id;
			quest_info.reward_count = it->reward_count;
			quest_info.task_id = it->task_id;
			quest_info.task_id_space = it->task_id_space;
		}		
		return &quest_info; 
	}
	
	int GetExpBonusMaxReincarnation() { return exp_bonus_max_reincarnation; }
	int GetExpBonusMaxLevel() { return exp_bonus_max_level; }
	int GetExpBonusRate() { return exp_bonus_rate; }
	int GetRealmExpMaxRealmLevel() { return realm_exp_max_relm_level; }
	int GetRealmExpRate() { return realm_exp_rate; }
	
ActivityEventConfig()
{

}

~ActivityEventConfig()
{
	
}

static ActivityEventConfig * GetInstance()
{
	if (!instance)
	{
		instance = new ActivityEventConfig();
	}
	return instance;
}

};


#endif