
#ifndef __GNET_ARENATEAMLEAVE_HPP
#define __GNET_ARENATEAMLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamLeave : public GNET::Protocol
{
	#include "arenateamleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
