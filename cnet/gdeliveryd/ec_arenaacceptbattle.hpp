
#ifndef __GNET_EC_ARENAACCEPTBATTLE_HPP
#define __GNET_EC_ARENAACCEPTBATTLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaAcceptBattle : public GNET::Protocol
{
	#include "ec_arenaacceptbattle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->BeginEnter(roleid);
	}
};

};

#endif
