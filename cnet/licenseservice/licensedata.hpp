
#ifndef __GNET_LICENSEDATA_HPP
#define __GNET_LICENSEDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseData : public GNET::Protocol
{
	#include "licensedata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
