
#ifndef __GNET_EC_ARENAMATCHCANCELNOTIFY_HPP
#define __GNET_EC_ARENAMATCHCANCELNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaMatchCancelNotify : public GNET::Protocol
{
	#include "ec_arenamatchcancelnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
