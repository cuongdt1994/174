
#ifndef __GNET_EC_ARENATEAMMATCH_HPP
#define __GNET_EC_ARENATEAMMATCH_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamMatch : public GNET::Protocol
{
	#include "ec_arenateammatch"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
