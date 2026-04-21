
#ifndef __GNET_LICENSELOGIN_HPP
#define __GNET_LICENSELOGIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseLogin : public GNET::Protocol
{
	#include "licenselogin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
