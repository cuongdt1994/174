
#ifndef __GNET_EC_ARENAPLAYERDATANOTIFY_HPP
#define __GNET_EC_ARENAPLAYERDATANOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenaplayersync"

namespace GNET
{

class EC_ArenaPlayerDataNotify : public GNET::Protocol
{
	#include "ec_arenaplayerdatanotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
