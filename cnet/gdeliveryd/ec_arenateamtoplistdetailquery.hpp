
#ifndef __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_HPP
#define __GNET_EC_ARENATEAMTOPLISTDETAILQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaTeamTopListDetailQuery : public GNET::Protocol
{
	#include "ec_arenateamtoplistdetailquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->EC_ArenaTeamTopListDetail(roleid,teamid);
	}
};

};

#endif
