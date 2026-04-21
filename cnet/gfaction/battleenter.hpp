
#ifndef __GNET_BATTLEENTER_HPP
#define __GNET_BATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "battleenter_re.hpp"

#include "luaman.hpp"


namespace GNET
{

class BattleEnter : public GNET::Protocol
{
	#include "battleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		LuaManager * lua = LuaManager::GetInstance();
		GFactionServer* gfs = GFactionServer::GetInstance();
		GFactionServer::Player player;
		
		if (!(gfs->GetPlayer(roleid,player)))
		{
			Log::log(LOG_ERR,"BattleEnter: roleid=%d not found on faction server\n",roleid);
			return;
		}

		time_t now = Timer::GetTime();
		if(now-player.ext.jointime < lua->GetConfig()->time_battle_enter )
		{
			BattleEnter_Re re(ERR_BS_NEWBIE_BANNED, player.localsid);
			gfs->DispatchProtocol(player.linkid, re);
			return;
		}
		GFactionServer::GetInstance()->DispatchProtocol(0, this);
	}
};

};

#endif
