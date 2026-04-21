
#ifndef __GNET_EC_ARENATEAMDATANOTIFY_HPP
#define __GNET_EC_ARENATEAMDATANOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateamsync"
#include "ec_arenaplayersync"

namespace GNET
{

class EC_ArenaTeamDataNotify : public GNET::Protocol
{
	#include "ec_arenateamdatanotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GLinkServer* lsm = GLinkServer::GetInstance();
		GLinkServer::RoleMap::iterator it=lsm->roleinfomap.find(roleid);
		if (it!=lsm->roleinfomap.end()&&(*it).second.roleid==roleid&&(*it).second.status==_STATUS_ONGAME)
		{
			lsm->Send((*it).second.sid,this);
		}
	}
};

};

#endif
