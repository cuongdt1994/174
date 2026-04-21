
#ifndef __GNET_ARENABATTLEDATA_HPP
#define __GNET_ARENABATTLEDATA_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "arenabattle"

namespace GNET
{

class ArenaBattleData : public GNET::Protocol
{
	#include "arenabattledata"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
