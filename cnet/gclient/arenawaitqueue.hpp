
#ifndef __GNET_ARENAWAITQUEUE_HPP
#define __GNET_ARENAWAITQUEUE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaWaitQueue : public GNET::Protocol
{
	#include "arenawaitqueue"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
