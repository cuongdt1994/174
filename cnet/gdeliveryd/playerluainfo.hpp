
#ifndef __GNET_PLAYERLUAINFO_HPP
#define __GNET_PLAYERLUAINFO_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "luaman.hpp"

namespace GNET
{

class PlayerLuaInfo : public GNET::Protocol
{
	#include "playerluainfo"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		LuaManager::GetInstance()->EventOnPlayerLuaInfo(roleid, info);
	}
};

};

#endif
