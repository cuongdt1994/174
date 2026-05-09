
#ifndef __GNET_HWIDPLAYERDATA_HPP
#define __GNET_HWIDPLAYERDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HwidPlayerData : public GNET::Protocol
{
	#include "hwidplayerdata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
