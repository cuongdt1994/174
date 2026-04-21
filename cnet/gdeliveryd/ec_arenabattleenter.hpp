
#ifndef __GNET_EC_ARENABATTLEENTER_HPP
#define __GNET_EC_ARENABATTLEENTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaBattleEnter : public GNET::Protocol
{
	#include "ec_arenabattleenter"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
