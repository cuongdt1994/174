
#ifndef __GNET_EC_ARENAPLAYERTOTALINFOQUERY_HPP
#define __GNET_EC_ARENAPLAYERTOTALINFOQUERY_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaPlayerTotalInfoQuery : public GNET::Protocol
{
	#include "ec_arenaplayertotalinfoquery"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
