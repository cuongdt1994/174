
#ifndef __GNET_EC_ARENABATTLESTART_HPP
#define __GNET_EC_ARENABATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gsp_if.h"

#include "ec_sqlarenateam"
#include "ec_sqlarenaplayer"


void EC_GSArenaBattleStart( GMSV::EC_GSArena & arena );

namespace GNET
{

class EC_ArenaBattleStart : public GNET::Protocol
{
	#include "ec_arenabattlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GMSV::EC_GSArena arena;
		arena.arena_id = arena_id;
		arena.end_time = end_time;
		
		arena.red_team.GSConvert(&red_team);
		for (unsigned int i = 0; i < red_members.size() && i < 6; i++)
		{
			GMSV::EC_GSArenaPlayer player;
			player.GSConvert(&red_members[i]);
			arena.red_members.push_back(player);
		}

		arena.blue_team.GSConvert(&blue_team);
		for (unsigned int i = 0; i < blue_members.size() && i < 6; i++)
		{
			GMSV::EC_GSArenaPlayer player;
			player.GSConvert(&blue_members[i]);
			arena.blue_members.push_back(player);
		}
		
		EC_GSArenaBattleStart( arena );
	}
};

};

#endif
