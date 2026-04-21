
#ifndef __GNET_ARENATEAMCHANGE_HPP
#define __GNET_ARENATEAMCHANGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamChange : public GNET::Protocol
{
	#include "arenateamchange"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
