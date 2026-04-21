
#ifndef __GNET_ARENATEAMINVITE_RE_HPP
#define __GNET_ARENATEAMINVITE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaTeamInvite_Re : public GNET::Protocol
{
	#include "arenateaminvite_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
