
#ifndef __GNET_EC_ARENAPLAYERTOTALINFOQUERY_RE_HPP
#define __GNET_EC_ARENAPLAYERTOTALINFOQUERY_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenateam"
#include "ec_arenaplayer"

namespace GNET
{

class EC_ArenaPlayerTotalInfoQuery_Re : public GNET::Protocol
{
	#include "ec_arenaplayertotalinfoquery_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
