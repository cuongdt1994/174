
#ifndef __GNET_HOMEBROWSEINFOQUERY_RE_HPP
#define __GNET_HOMEBROWSEINFOQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "homeitem"
#include "homeequipment"
#include "homecropper"

namespace GNET
{

class HomeBrowseInfoQuery_Re : public GNET::Protocol
{
	#include "homebrowseinfoquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
