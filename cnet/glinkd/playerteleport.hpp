
#ifndef __GNET_PLAYERTELEPORT_HPP
#define __GNET_PLAYERTELEPORT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PlayerTeleport : public GNET::Protocol
{
	#include "playerteleport"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
