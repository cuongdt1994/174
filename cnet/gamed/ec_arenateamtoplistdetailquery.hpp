
#ifndef __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_HPP
#define __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamTopListDetailQuery : public GNET::Protocol
{
	#include "ec_arenateamtoplistdetailquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
