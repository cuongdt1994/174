
#ifndef __GNET_S2CMULTICAST_HPP
#define __GNET_S2CMULTICAST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "player"
#include "gamedatasend.hpp"
#include "glinkserver.hpp"

#include <algorithm>
#include <sys/time.h>

namespace GNET
{

class S2CMulticast : public GNET::Protocol
{
	#include "s2cmulticast"
	
	#pragma pack(1)
	struct CMD
	{
		short cmd;
		char data[];
	};
	#pragma pack()
	
	struct GetPlayerID
	{
		unsigned int operator () (const Player& player) const{
			return player.localsid;
		}
	};
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size() < 2) return;
		
		GamedataSend gds(data);
		if (! playerlist.GetVector().size() ) return;
		
		CMD* it = (CMD*)data.begin();
		switch (it->cmd)
		{
		case OfflineMarket::PLAYER_CANCEL_MARKET:
			OfflineMarket::GetInstance()->Cancel(data.begin(), data.size());
			break;
		default:
			break;
		}

		GLinkServer::GetInstance()->AccumulateSend( playerlist.GetVector().begin(), playerlist.GetVector().end(),
				gds, GetPlayerID());
	}
};

};

#endif
