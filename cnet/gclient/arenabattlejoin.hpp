
#ifndef __GNET_ARENABATTLEJOIN_HPP
#define __GNET_ARENABATTLEJOIN_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaBattleJoin : public GNET::Protocol
{
	#include "arenabattlejoin"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
