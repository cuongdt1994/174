
#ifndef __GNET_ARENABATTLERESULT_HPP
#define __GNET_ARENABATTLERESULT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaBattleResult : public GNET::Protocol
{
	#include "arenabattleresult"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
