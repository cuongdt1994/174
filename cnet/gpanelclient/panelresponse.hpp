
#ifndef __GNET_PANELRESPONSE_HPP
#define __GNET_PANELRESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class PanelResponse : public GNET::Protocol
{
	#include "panelresponse"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
