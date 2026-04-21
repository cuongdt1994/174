
#ifndef __GNET_EC_ARENABATTLESERVERREGISTER_HPP
#define __GNET_EC_ARENABATTLESERVERREGISTER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaBattleServerRegister : public GNET::Protocol
{
	#include "ec_arenabattleserverregister"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
