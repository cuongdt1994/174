
#ifndef __GNET_EC_ARENAPLAYERTOTALINFOQUERY_RE_HPP
#define __GNET_EC_ARENAPLAYERTOTALINFOQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateam"
#include "ec_arenaplayer"

namespace GNET
{

class EC_ArenaPlayerTotalInfoQuery_Re : public GNET::Protocol
{
	#include "ec_arenaplayertotalinfoquery_re"

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
