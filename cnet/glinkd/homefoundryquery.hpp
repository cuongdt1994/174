
#ifndef __GNET_HOMEFOUNDRYQUERY_HPP
#define __GNET_HOMEFOUNDRYQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeFoundryQuery : public GNET::Protocol
{
	#include "homefoundryquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
