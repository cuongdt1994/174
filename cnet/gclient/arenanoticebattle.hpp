
#ifndef __GNET_ARENANOTICEBATTLE_HPP
#define __GNET_ARENANOTICEBATTLE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaNoticeBattle : public GNET::Protocol
{
	#include "arenanoticebattle"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
