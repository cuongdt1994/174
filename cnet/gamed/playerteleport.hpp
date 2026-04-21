
#ifndef __GNET_PLAYERTELEPORT_HPP
#define __GNET_PLAYERTELEPORT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void player_teleport(int roleid, int tag, float x, float y, float z);

namespace GNET
{

class PlayerTeleport : public GNET::Protocol
{
	#include "playerteleport"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (roleid >= 1024 && tag > 0)
		{
			player_teleport(roleid, tag, x, y, z);
		}
	}
};

};

#endif
