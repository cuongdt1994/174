
#ifndef __GNET_ARENAACCEPTBATTLE_HPP
#define __GNET_ARENAACCEPTBATTLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaAcceptBattle : public GNET::Protocol
{
	#include "arenaacceptbattle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
