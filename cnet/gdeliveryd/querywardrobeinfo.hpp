
#ifndef __GNET_QUERYWARDROBEINFO_HPP
#define __GNET_QUERYWARDROBEINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class QueryWardrobeInfo : public GNET::Protocol
{
	#include "querywardrobeinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
