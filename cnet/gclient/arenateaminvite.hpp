
#ifndef __GNET_ARENATEAMINVITE_HPP
#define __GNET_ARENATEAMINVITE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamInvite : public GNET::Protocol
{
	#include "arenateaminvite"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
