
#ifndef __GNET_EC_ARENAENTERQUEUE_HPP
#define __GNET_EC_ARENAENTERQUEUE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaEnterQueue : public GNET::Protocol
{
	#include "ec_arenaenterqueue"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
