
#ifndef __GNET_EC_ARENACLIENTNOTIFY_HPP
#define __GNET_EC_ARENACLIENTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaClientNotify : public GNET::Protocol
{
	#include "ec_arenaclientnotify"

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
