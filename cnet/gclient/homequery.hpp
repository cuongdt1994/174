
#ifndef __GNET_HOMEQUERY_HPP
#define __GNET_HOMEQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeQuery : public GNET::Protocol
{
	#include "homequery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
