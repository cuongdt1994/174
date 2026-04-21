
#ifndef __GNET_EC_ARENAACCEPTBATTLE_HPP
#define __GNET_EC_ARENAACCEPTBATTLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaAcceptBattle : public GNET::Protocol
{
	#include "ec_arenaacceptbattle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
