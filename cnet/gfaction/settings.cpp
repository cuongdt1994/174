#include "rpcdefs.h"
#include "settings.h"
#include "luaman.hpp"

namespace GNET
{
	int FactionConfig::UpgradeCost(int level)
	{
		//static int costs[MAX_LEVEL+1] = {100000, 2000000, 10000000};
		if(level>MAX_LEVEL || level<0)
			return -1;
		//return costs[level];
		return LuaManager::GetInstance()->GetConfig()->GUILD_UPGRADE_COST[level];
	}

	int FactionConfig::MaxRole(int role)
	{
		//static int max[7] = {0, 0, 1, 1, 4, 12, 120};
		if(role>6 || role<0)
			return -1;
		//return max[role];
		return LuaManager::GetInstance()->GetConfig()->GUILD_ROLE_RANK[role];
	}

	int FactionConfig::MaxMember(int level)
	{
		//static int members[MAX_LEVEL+1] = {60, 90, 120};
		if(level>MAX_LEVEL || level<0)
			return -1;
		//return members[level];
		return LuaManager::GetInstance()->GetConfig()->GUILD_MAX_MEMBERS[level];
	}
}
