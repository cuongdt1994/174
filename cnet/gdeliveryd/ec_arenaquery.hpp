
#ifndef __GNET_EC_ARENAQUERY_HPP
#define __GNET_EC_ARENAQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaQuery : public GNET::Protocol
{
	#include "ec_arenaquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->SendArenaInfoToPlayer(roleid);
	}
};

};

#endif
