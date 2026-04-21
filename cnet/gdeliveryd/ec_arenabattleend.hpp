
#ifndef __GNET_EC_ARENABATTLEEND_HPP
#define __GNET_EC_ARENABATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_sqlarenateam"
#include "ec_sqlarenaplayer"
#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaBattleEnd : public GNET::Protocol
{
	#include "ec_arenabattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//void ArenaOfAuroraManager::OnArenaEnd(int arena_id, int result, int red_alive, int blue_alive, int red_damage, int blue_damage, EC_SQLArenaTeam & red_team, EC_SQLArenaPlayerVector & red_members, EC_SQLArenaTeam & blue_team, EC_SQLArenaPlayerVector & blue_members)
		
		
		ArenaOfAuroraManager::GetInstance()->OnArenaEnd(arena_id, result, red_alive, blue_alive, red_damage, blue_damage, red_team, red_members, blue_team, blue_members);
	}
};

};

#endif
