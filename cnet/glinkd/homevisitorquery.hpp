
#ifndef __GNET_HOMEVISITORQUERY_HPP
#define __GNET_HOMEVISITORQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeVisitorQuery : public GNET::Protocol
{
	#include "homevisitorquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
