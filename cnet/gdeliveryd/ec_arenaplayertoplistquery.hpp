
#ifndef __GNET_EC_ARENAPLAYERTOPLISTQUERY_HPP
#define __GNET_EC_ARENAPLAYERTOPLISTQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaPlayerTopListQuery : public GNET::Protocol
{
	#include "ec_arenaplayertoplistquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->EC_ArenaPlayerTopList_Re(roleid);
	}
};

};

#endif
