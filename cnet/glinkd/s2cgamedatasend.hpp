
#ifndef __GNET_S2CGAMEDATASEND_HPP
#define __GNET_S2CGAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gamedatasend.hpp"
#include "gdeliveryclient.hpp"
#include "statusannounce.hpp"

namespace GNET
{
class S2CGamedataSend : public GNET::Protocol
{
	#include "s2cgamedatasend"

	#pragma pack(1)
	struct CMD
	{
		short cmd;
		char data[];
	};
	#pragma pack()

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size() < 2) return;
		GLinkServer* lsm=GLinkServer::GetInstance();
		if (!lsm->ValidRole(localsid,roleid)) return;

		CMD* it = (CMD*)data.begin();
		switch (it->cmd)
		{
		case OfflineMarket::SELF_OPEN_MARKET:
			OfflineMarket::GetInstance()->Open(data.begin(), data.size(), localsid);
			break;
		case OfflineMarket::PLAYER_MARKET_INFO:
			OfflineMarket::GetInstance()->Info(data.begin(), data.size());
			break;
		default:
			break;
		}	
		
		lsm->AccumulateSend(localsid,GamedataSend(data));
	}
};

};

#endif
