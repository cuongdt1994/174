
#ifndef __GNET_EC_LDBATTLECLIENTOPERATE_HPP
#define __GNET_EC_LDBATTLECLIENTOPERATE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_LDBattleClientOperate : public GNET::Protocol
{
	#include "ec_ldbattleclientoperate"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
