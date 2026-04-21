
#ifndef __GNET_EC_ARENAQUERY_RE_HPP
#define __GNET_EC_ARENAQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateamsync"
#include "ec_arenaplayersync"

namespace GNET
{

class EC_ArenaQuery_Re : public GNET::Protocol
{
	#include "ec_arenaquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
