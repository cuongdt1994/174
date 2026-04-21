
#ifndef __GNET_EC_ARENAENTERQUEUE_HPP
#define __GNET_EC_ARENAENTERQUEUE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaEnterQueue : public GNET::Protocol
{
	#include "ec_arenaenterqueue"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (members.size() == 3 || members.size() == 6)
		{
			ArenaOfAuroraManager::GetInstance()->OnStartSearch(members);
		}
		else
		{
			ArenaOfAuroraManager::GetInstance()->OnStartSearch(roleid, cls, name);
		}
	}
};

};

#endif
