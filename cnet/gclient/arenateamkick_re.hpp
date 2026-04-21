
#ifndef __GNET_ARENATEAMKICK_RE_HPP
#define __GNET_ARENATEAMKICK_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamKick_Re : public GNET::Protocol
{
	#include "arenateamkick_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
