
#ifndef __GNET_LICENSECHALLENGE_HPP
#define __GNET_LICENSECHALLENGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class LicenseChallenge : public GNET::Protocol
{
	#include "licensechallenge"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
