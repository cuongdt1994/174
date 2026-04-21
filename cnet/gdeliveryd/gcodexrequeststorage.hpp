#ifndef __GNET_GCODEXREQUESTSTORAGE_HPP
#define __GNET_GCODEXREQUESTSTORAGE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "mapuser.h"

#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"

#include "gcodexrequeststorage_re.hpp"

namespace GNET
{

class GCodexRequestStorage : public GNET::Protocol
{
	#include "gcodexrequeststorage"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(roleid == 0) {
			LOG_TRACE("CODEX ERROR!! Gdeliveryd:: GCodexRequestStorage :: roleid= %d", roleid);
			return;
		}

		GCodexRequestStorage_Re re;
		GDeliveryServer* dsm = GDeliveryServer::GetInstance();
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());	
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole((roleid));

		re.roleid = roleid;	
		re.reserve = 0;
		re.fashions_count = fashions_count;
		re.fashions = fashions;
		re.flys_count = flys_count;
		re.flys = flys;
		re.mounts_count = mounts_count;
		re.mounts = mounts;
		re.pets_count = pets_count;
		re.pets = pets;		
			
		dsm->Send(pinfo->linksid, re);

		LOG_TRACE("CODEX SUCCESS!! Gdeliveryd:: GCodexRequestStorage Send Protocol :: roleid=%d", roleid);

	}
};

};

#endif
