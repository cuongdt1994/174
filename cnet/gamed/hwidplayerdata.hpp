
#ifndef __GNET_HWIDPLAYERDATA_HPP
#define __GNET_HWIDPLAYERDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void set_player_hwid(int roleid, unsigned long long hwid);

namespace GNET
{

class HwidPlayerData : public GNET::Protocol
{
	#include "hwidplayerdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if ( hwid.size() == 8 && *(unsigned long long*)hwid.begin() )
		{
			set_player_hwid(roleid, *(unsigned long long*)hwid.begin());
		}
	}
};

};

#endif
