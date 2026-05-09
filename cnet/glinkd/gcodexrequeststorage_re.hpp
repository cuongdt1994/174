#ifndef __GNET_GCODEXREQUESTSTORAGE_RE_HPP
#define __GNET_GCODEXREQUESTSTORAGE_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gcodexrequeststorage.hpp"

namespace GNET
{

class GCodexRequestStorage_Re : public GNET::Protocol
{
	#include "gcodexrequeststorage_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{		
		GLinkServer* lsm = GLinkServer::GetInstance();
		GLinkServer::RoleMap::iterator it=lsm->roleinfomap.find(roleid);

		lsm->Send((*it).second.sid,this);

	}
};

};

#endif
