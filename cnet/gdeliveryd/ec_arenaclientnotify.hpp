
#ifndef __GNET_EC_ARENACLIENTNOTIFY_HPP
#define __GNET_EC_ARENACLIENTNOTIFY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaClientNotify : public GNET::Protocol
{
	#include "ec_arenaclientnotify"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
