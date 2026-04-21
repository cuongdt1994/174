
#ifndef __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_RE_HPP
#define __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenaplayertoplist"

namespace GNET
{

class EC_ArenaTeamTopListDetailQuery_Re : public GNET::Protocol
{
	#include "ec_arenateamtoplistdetailquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
