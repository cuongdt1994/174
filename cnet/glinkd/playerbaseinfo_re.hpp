
#ifndef __GNET_PLAYERBASEINFO_RE_HPP
#define __GNET_PLAYERBASEINFO_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "groleforbid"
#include "grolebase"

#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
namespace GNET
{

class PlayerBaseInfo_Re : public GNET::Protocol
{
	#include "playerbaseinfo_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//return;
		/*
		const char name[] = { 'P', 0x00 , 'C' , 0x00 };
		
		player.name.replace(name, sizeof(name));
		player.custom_data.clear();
		*/
		GLinkServer::GetInstance()->Send(localsid,this);

	}
};

};

#endif
