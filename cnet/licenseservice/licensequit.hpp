
#ifndef __GNET_LICENSEQUIT_HPP
#define __GNET_LICENSEQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseQuit : public GNET::Protocol
{
	#include "licensequit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
