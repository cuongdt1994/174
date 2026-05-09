
#ifndef __GNET_EC_SPOUSEQUERY_HPP
#define __GNET_EC_SPOUSEQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_SpouseQuery : public GNET::Protocol
{
	#include "ec_spousequery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
