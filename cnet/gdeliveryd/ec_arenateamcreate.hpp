
#ifndef __GNET_EC_ARENATEAMCREATE_HPP
#define __GNET_EC_ARENATEAMCREATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaTeamCreate : public GNET::Protocol
{
	#include "ec_arenateamcreate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->EC_CreateArenaTeam(roleid, cls, player_name, team_type, team_name);
	}
};

};

#endif
