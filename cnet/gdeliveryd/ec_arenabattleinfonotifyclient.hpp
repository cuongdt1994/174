
#ifndef __GNET_EC_ARENABATTLEINFONOTIFYCLIENT_HPP
#define __GNET_EC_ARENABATTLEINFONOTIFYCLIENT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaBattleInfoNotifyClient : public GNET::Protocol
{
	#include "ec_arenabattleinfonotifyclient"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
