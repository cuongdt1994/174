
#ifndef __GNET_HOMEFOUNDRYFORMULASSYNC_HPP
#define __GNET_HOMEFOUNDRYFORMULASSYNC_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gfoundryformulas"

namespace GNET
{

class HomeFoundryFormulasSync : public GNET::Protocol
{
	#include "homefoundryformulassync"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
