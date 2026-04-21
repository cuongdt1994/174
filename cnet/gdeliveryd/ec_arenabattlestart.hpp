
#ifndef __GNET_EC_ARENABATTLESTART_HPP
#define __GNET_EC_ARENABATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_arenamanager.h"

namespace GNET
{

class EC_ArenaBattleStart : public GNET::Protocol
{
	#include "ec_arenabattlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
