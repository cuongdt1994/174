
#ifndef __GNET_EC_SYSTEMMONITORLOG_HPP
#define __GNET_EC_SYSTEMMONITORLOG_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_SystemMonitorLog : public GNET::Protocol
{
	#include "ec_systemmonitorlog"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
