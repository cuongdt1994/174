
#ifndef __GNET_ARENABATTLEQUIT_HPP
#define __GNET_ARENABATTLEQUIT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class ArenaBattleQuit : public GNET::Protocol
{
	#include "arenabattlequit"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
