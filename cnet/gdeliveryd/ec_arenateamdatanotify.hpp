
#ifndef __GNET_EC_ARENATEAMDATANOTIFY_HPP
#define __GNET_EC_ARENATEAMDATANOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateamsync"
#include "ec_arenaplayersync"

namespace GNET
{

class EC_ArenaTeamDataNotify : public GNET::Protocol
{
	#include "ec_arenateamdatanotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
