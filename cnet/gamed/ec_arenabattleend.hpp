
#ifndef __GNET_EC_ARENABATTLEEND_HPP
#define __GNET_EC_ARENABATTLEEND_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "ec_sqlarenateam"
#include "ec_sqlarenaplayer"

namespace GNET
{

class EC_ArenaBattleEnd : public GNET::Protocol
{
	#include "ec_arenabattleend"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
