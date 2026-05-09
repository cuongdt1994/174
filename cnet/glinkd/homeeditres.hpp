
#ifndef __GNET_HOMEEDITRES_HPP
#define __GNET_HOMEEDITRES_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeEditRes : public GNET::Protocol
{
	#include "homeeditres"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
