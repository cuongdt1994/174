
#ifndef __GNET_HOMEEDITEND_HPP
#define __GNET_HOMEEDITEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "homemodule"

namespace GNET
{

class HomeEditEnd : public GNET::Protocol
{
	#include "homeeditend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
