
#ifndef __GNET_EC_ARENAONLINEINFONOTIFY_HPP
#define __GNET_EC_ARENAONLINEINFONOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateammemberonlineinfo"

namespace GNET
{

class EC_ArenaOnlineInfoNotify : public GNET::Protocol
{
	#include "ec_arenaonlineinfonotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
