
#ifndef __GNET_HOMEFOUNDRYFACTORYPRODUCEREQUEST_HPP
#define __GNET_HOMEFOUNDRYFACTORYPRODUCEREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeFoundryFactoryProduceRequest : public GNET::Protocol
{
	#include "homefoundryfactoryproducerequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
