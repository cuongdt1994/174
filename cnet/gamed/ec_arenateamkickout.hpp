
#ifndef __GNET_EC_ARENATEAMKICKOUT_HPP
#define __GNET_EC_ARENATEAMKICKOUT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamKickout : public GNET::Protocol
{
	#include "ec_arenateamkickout"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
