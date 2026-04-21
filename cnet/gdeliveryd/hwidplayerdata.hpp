
#ifndef __GNET_HWIDPLAYERDATA_HPP
#define __GNET_HWIDPLAYERDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "playerteleport.hpp"
#include "luaman.hpp"

namespace GNET
{

class HwidPlayerData : public GNET::Protocol
{
	#include "hwidplayerdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( hwid.size() == 8 && *(unsigned long long*)hwid.begin() )
		{
			int gameid = 0; 
			int is_transport = LuaManager::GetInstance()->EventOnEnterServer(roleid, hwid, gameid);
			if (gameid > 0)
			{
				GProviderServer::GetInstance()->DispatchProtocol( gameid , this );
				if ( is_transport )
				{
					PlayerTeleport pt(roleid,1,1290.0f, 220.0f, 1135.0f);
					GProviderServer::GetInstance()->DispatchProtocol( gameid , pt );
				}
			}
		}
	}
};

};

#endif
