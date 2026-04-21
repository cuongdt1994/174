
#ifndef __GNET_FACTIONALLIANCECHAT_HPP
#define __GNET_FACTIONALLIANCECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class FactionAllianceChat : public GNET::Protocol
{
	#include "factionalliancechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
