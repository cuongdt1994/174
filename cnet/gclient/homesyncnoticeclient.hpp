
#ifndef __GNET_HOMESYNCNOTICECLIENT_HPP
#define __GNET_HOMESYNCNOTICECLIENT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ghomesync"

namespace GNET
{

class HomeSyncNoticeClient : public GNET::Protocol
{
	#include "homesyncnoticeclient"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
