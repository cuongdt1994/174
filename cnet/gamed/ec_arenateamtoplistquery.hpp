
#ifndef __GNET_EC_ARENATEAMTOPLISTQUERY_HPP
#define __GNET_EC_ARENATEAMTOPLISTQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamTopListQuery : public GNET::Protocol
{
	#include "ec_arenateamtoplistquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
