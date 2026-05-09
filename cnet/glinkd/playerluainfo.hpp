
#ifndef __GNET_PLAYERLUAINFO_HPP
#define __GNET_PLAYERLUAINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "hwidplayerdata.hpp"

namespace GNET
{

class PlayerLuaInfo : public GNET::Protocol
{
	#include "playerluainfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//player hwid
		SessionInfo * sinfo = GLinkServer::GetInstance()->GetSessionInfo(localsid);
		if (sinfo)
		{
			HwidPlayerData hpd(roleid , localsid, sinfo->hwid);
			GDeliveryClient::GetInstance()->SendProtocol(hpd);			
		}
		// TODO
	}
};

};

#endif
