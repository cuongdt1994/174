
#ifndef __GNET_EC_ARENAPLAYERTOPLISTQUERY_RE_HPP
#define __GNET_EC_ARENAPLAYERTOPLISTQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenaplayertoplist"

namespace GNET
{

class EC_ArenaPlayerTopListQuery_Re : public GNET::Protocol
{
	#include "ec_arenaplayertoplistquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
