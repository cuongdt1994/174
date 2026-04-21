
#ifndef __GNET_EC_FACTIONSIGNALREQUEST_HPP
#define __GNET_EC_FACTIONSIGNALREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_FactionSignalRequest : public GNET::Protocol
{
	#include "ec_factionsignalrequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
