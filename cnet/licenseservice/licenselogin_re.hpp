
#ifndef __GNET_LICENSELOGIN_RE_HPP
#define __GNET_LICENSELOGIN_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseLogin_Re : public GNET::Protocol
{
	#include "licenselogin_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
