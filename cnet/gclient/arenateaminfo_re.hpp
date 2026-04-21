
#ifndef __GNET_ARENATEAMINFO_RE_HPP
#define __GNET_ARENATEAMINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "arenateamdetail"
#include "arenaplayerdetail"

namespace GNET
{

class ArenaTeamInfo_Re : public GNET::Protocol
{
	#include "arenateaminfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
