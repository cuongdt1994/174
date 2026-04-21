
#ifndef __GNET_ARENATEAMCREATE_RE_HPP
#define __GNET_ARENATEAMCREATE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamCreate_Re : public GNET::Protocol
{
	#include "arenateamcreate_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
