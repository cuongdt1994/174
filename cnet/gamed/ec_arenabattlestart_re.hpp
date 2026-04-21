
#ifndef __GNET_EC_ARENABATTLESTART_RE_HPP
#define __GNET_EC_ARENABATTLESTART_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaBattleStart_Re : public GNET::Protocol
{
	#include "ec_arenabattlestart_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
