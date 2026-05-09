
#ifndef __GNET_HOMEVISITORINFO_HPP
#define __GNET_HOMEVISITORINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "homevisitorinfodata"

namespace GNET
{

class HomeVisitorInfo : public GNET::Protocol
{
	#include "homevisitorinfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
