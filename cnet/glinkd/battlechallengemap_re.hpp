
#ifndef __GNET_BATTLECHALLENGEMAP_RE_HPP
#define __GNET_BATTLECHALLENGEMAP_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "challengewish"
#include "gbattlechallenge"

namespace GNET
{

class BattleChallengeMap_Re : public GNET::Protocol
{
	#include "battlechallengemap_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		printf("battlechallengemap_re roleid = %d, status = %d, retcode = %d \n", roleid, status, retcode);
		GLinkServer::GetInstance()->Send(localsid,this);
	}
};

};

#endif
