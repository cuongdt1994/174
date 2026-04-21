
#ifndef __GNET_ARENATEAMLEAVE_RE_HPP
#define __GNET_ARENATEAMLEAVE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamLeave_Re : public GNET::Protocol
{
	#include "arenateamleave_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
