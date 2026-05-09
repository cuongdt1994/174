
#ifndef __GNET_EC_ARENAPLAYERTOTALINFOQUERY_HPP
#define __GNET_EC_ARENAPLAYERTOTALINFOQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_proxy.h"

namespace GNET
{

class EC_ArenaPlayerTotalInfoQuery : public GNET::Protocol
{
	#include "ec_arenaplayertotalinfoquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if (!GLinkServer::ValidRole(sid, roleid))
		{
			GLinkServer::GetInstance()->SessionError(sid, ERR_INVALID_ACCOUNT, "Error userid or roleid.");
			return;
		}

		//send it to delivery
		GDeliveryClient::GetInstance()->SendProtocol(this);
	}
};

};

#endif
