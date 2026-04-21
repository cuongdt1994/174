
#ifndef __GNET_EC_ARENATEAMINVITE_HPP
#define __GNET_EC_ARENATEAMINVITE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamInvite : public GNET::Protocol
{
	#include "ec_arenateaminvite"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
