
#ifndef __GNET_ARENABATTLEEND_HPP
#define __GNET_ARENABATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaBattleEnd : public GNET::Protocol
{
	#include "arenabattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
