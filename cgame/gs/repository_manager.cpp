//------------------------------------------------------------------------------------------------------------------------
//--REPOSITORY MANAGER (C) 2022 deadraky
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
#include "repository_manager.h"

RepositoryConfig* RepositoryConfig::instance = 0;

RepositoryConfig::RepositoryConfig()
{
	memset(this,0x00,sizeof(*this));
}

RepositoryConfig::~RepositoryConfig()
{
	;
}

void RepositoryConfig::Init()
{
	std::vector<int> suite_list;
	world_manager::GetDataMan().get_list_by_data_type(ID_SPACE_ESSENCE,DT_SUITE_ESSENCE,suite_list);

	count = 0;
	for(int i = 0; i < suite_list.size() && i < MAX_REPOSITORY; i++)
	{
		DATA_TYPE dt;
		SUITE_ESSENCE *suite = (SUITE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(suite_list[i],ID_SPACE_ESSENCE,dt);
		if (suite && suite->used_in_treasury && dt == DT_SUITE_ESSENCE)
		{
			list[count].suite_id = suite->id;
			for (int j = 0; j < MAX_SUITE_ITEM; j++)
			{
				list[count].equip[j] = suite->equipments[j].id;
			}
			count++;
		}
	}
}

int RepositoryConfig::Check(int item_id)
{
	for(int i = 0; i < count && i < MAX_REPOSITORY; i++)
	{
		for (int j = 0; j < MAX_SUITE_ITEM; j++)
		{
			if ( list[i].equip[j] == item_id )
			{
				return list[i].suite_id;
			}
		}
	}
}

