
#ifndef __GNET_ARENABATTLESTART_HPP
#define __GNET_ARENABATTLESTART_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaBattleStart : public GNET::Protocol
{
	#include "arenabattlestart"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
