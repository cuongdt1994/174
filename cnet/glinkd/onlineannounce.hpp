
#ifndef __GNET_ONLINEANNOUNCE_HPP
#define __GNET_ONLINEANNOUNCE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "glinkserver.hpp"
#include "statusannounce.hpp"
#include "antiddos.hpp"
namespace GNET
{

class OnlineAnnounce : public GNET::Protocol
{
	#include "onlineannounce"
	
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		DEBUG_PRINT("glinkd::onlineannounce:user %d(sid=%d) will online,zoneid=%d, remain_playtime is %d,free_left=%d,free_end=%d\n",
			userid,localsid,zoneid,remain_time,free_time_left,free_time_end);
		GLinkServer* lsm=GLinkServer::GetInstance();
		
		if (!lsm->ValidUser(localsid,userid))
		{
			DEBUG_PRINT("linkd:: receive onlineannoucne from delivery,user(%d) is invalid.\n",userid);
			manager->Send(sid,StatusAnnounce(userid,localsid,_STATUS_OFFLINE));
		}
		else
		{
			//用户是否需要更新密码或密保卡
			SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
			if(sinfo)
			{
				AntiDDoS::GetInstance()->L7OnlineAnounce(inet_ntoa(((const struct sockaddr_in*)(sinfo->GetPeer()))->sin_addr));
				
				int cli_type = -1;
				if(sinfo->cli_fingerprint.size() >= sizeof(int)) memcpy(&cli_type, sinfo->cli_fingerprint.begin(), sizeof(cli_type));
				if(cli_type != -1) {
				  Log::trace("glinkd::onlineannounce:userid=%d,client_type=%d\n", userid, cli_type);
				}
			}
			lsm->ChangeState(localsid, &state_GRoleList);
			lsm->Send(localsid,this);
		}
	}
};

};

#endif
