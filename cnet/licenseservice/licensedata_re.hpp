
#ifndef __GNET_LICENSEDATA_RE_HPP
#define __GNET_LICENSEDATA_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseData_Re : public GNET::Protocol
{
	#include "licensedata_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
