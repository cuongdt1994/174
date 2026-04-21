
#ifndef __GNET_ARENATEAMINFO_HPP
#define __GNET_ARENATEAMINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamInfo : public GNET::Protocol
{
	#include "arenateaminfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
