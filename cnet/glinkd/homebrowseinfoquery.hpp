
#ifndef __GNET_HOMEBROWSEINFOQUERY_HPP
#define __GNET_HOMEBROWSEINFOQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class HomeBrowseInfoQuery : public GNET::Protocol
{
	#include "homebrowseinfoquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
