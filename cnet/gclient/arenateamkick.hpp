
#ifndef __GNET_ARENATEAMKICK_HPP
#define __GNET_ARENATEAMKICK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamKick : public GNET::Protocol
{
	#include "arenateamkick"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
