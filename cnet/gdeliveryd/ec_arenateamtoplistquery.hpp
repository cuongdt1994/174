
#ifndef __GNET_EC_ARENATEAMTOPLISTQUERY_HPP
#define __GNET_EC_ARENATEAMTOPLISTQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaTeamTopListQuery : public GNET::Protocol
{
	#include "ec_arenateamtoplistquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->EC_ArenaTeamTopList_Re(roleid);
	}
};

};

#endif
