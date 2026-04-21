
#ifndef __GNET_HOMEDETAIL_HPP
#define __GNET_HOMEDETAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "homebase"
#include "homeproperty"
#include "homecropper"
#include "homeitem"

namespace GNET
{

class HomeDetail : public GNET::Protocol
{
	#include "homedetail"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
