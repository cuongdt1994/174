
#ifndef __GNET_GETSAVEDMSG2_HPP
#define __GNET_GETSAVEDMSG2_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class GetSavedMsg2 : public GNET::Protocol
{
	#include "getsavedmsg2"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
