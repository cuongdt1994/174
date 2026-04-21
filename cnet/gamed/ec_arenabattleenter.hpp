
#ifndef __GNET_EC_ARENABATTLEENTER_HPP
#define __GNET_EC_ARENABATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gproviderclient.hpp"

void player_enter_arenabattle(int role, int world_tag, int arena_id);

namespace GNET
{

class EC_ArenaBattleEnter : public GNET::Protocol
{
	#include "ec_arenabattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		player_enter_arenabattle(roleid, world_tag, arena_id);
	}
};

};

#endif
