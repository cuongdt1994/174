
#ifndef __GNET_HOMEBRIEF_HPP
#define __GNET_HOMEBRIEF_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "homebase"
#include "homeproperty"
#include "homecropper"

namespace GNET
{

class HomeBrief : public GNET::Protocol
{
	#include "homebrief"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
