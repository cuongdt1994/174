
#ifndef __GNET_EC_ARENATEAMLEAVE_HPP
#define __GNET_EC_ARENATEAMLEAVE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamLeave : public GNET::Protocol
{
	#include "ec_arenateamleave"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
