
#ifndef __GNET_ARENATEAMCHANGE_RE_HPP
#define __GNET_ARENATEAMCHANGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamChange_Re : public GNET::Protocol
{
	#include "arenateamchange_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
