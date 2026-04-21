#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "activity_event_manager.h"
#include "gsp_if.h"

ActivityEventConfig* ActivityEventConfig::instance = NULL;

void ActivityEventConfig::Init()
{
	memset(this,0x00,sizeof(ActivityEventConfig));
	
	DATA_TYPE dt;
	ACTIVITY_EXPUP_CONFIG *it = (ACTIVITY_EXPUP_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_ACTIVITY_EXPUP,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_ACTIVITY_EXPUP_CONFIG)
	{
		exp_bonus_max_reincarnation = it->exp_bonus_max_reincarnation;
		exp_bonus_max_level = it->exp_bonus_max_level;
		exp_bonus_rate = it->exp_bonus_rate;
		realm_exp_max_relm_level = it->realm_exp_max_relm_level;
		realm_exp_rate = it->realm_exp_rate;
	}
	else
	{
		printf("ACTIVITY_EXPUP_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}	

	DATA_TYPE dt2;
	ACTIVITY_CONSUMPTION_CONFIG *it2 = (ACTIVITY_CONSUMPTION_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_ACTIVITY_CONSUMPTION,ID_SPACE_CONFIG,dt2);
	if (it2 && dt2 == DT_ACTIVITY_CONSUMPTION_CONFIG)
	{
		for (int i = 0; i < MAX_CONSUMPTION_ITENS; i++)
		{
			consumption_item[i].require_rmb = it2->list[i].require_rmb;
			consumption_item[i].item_id = it2->list[i].item_id;
			consumption_item[i].item_count = it2->list[i].item_count;
		}
	}
	else
	{
		printf("ACTIVITY_CONSUMPTION_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}

	DATA_TYPE dt3;
	ACTIVITY_SHOP_CONFIG *it3 = (ACTIVITY_SHOP_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_ACTIVITY_SHOP,ID_SPACE_CONFIG,dt3);
	if (it3 && dt3 == DT_ACTIVITY_SHOP_CONFIG)
	{
		for (int i = 0; i < MAX_SHOP_ITENS; i++)
		{
			shop_item[i].item_id = it3->item[i].id;
			shop_item[i].buy_times_limit = it3->item[i].buy_times_limit;
			shop_item[i].buy_times_limit_mode = it3->item[i].buy_times_limit_mode;
			shop_item[i].unk6 = it3->item[i].unk6;
			shop_item[i].price = it3->item[i].price;
		}
	}
	else
	{
		printf("ACTIVITY_SHOP_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}

	DATA_TYPE dt4;
	REWARD_INTERFACE_CONFIG *it4 = (REWARD_INTERFACE_CONFIG*)world_manager::GetDataMan().get_data_ptr(IDX_ACTIVITY_LOGIN_REWARD,ID_SPACE_CONFIG,dt4);
	if (it4 && dt4 == DT_REWARD_INTERFACE_CONFIG)
	{
		for (int i = 0; i < MAX_REWARD_LOGIN_COUNT; i++)
		{
			reward_login[i].item_id = it4->reward[i].item_id;
			reward_login[i].item_count = it4->reward[i].item_count;
			reward_login[i].item_expire_date = it4->reward[i].item_expire_date;
			reward_login[i].item_cooldown = it4->reward[i].item_cooldown;
		}
	}
	else
	{
		printf("REWARD_INTERFACE_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}

}

