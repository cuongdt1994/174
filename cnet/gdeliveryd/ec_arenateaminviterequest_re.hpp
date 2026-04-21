
#ifndef __GNET_EC_ARENATEAMINVITEREQUEST_RE_HPP
#define __GNET_EC_ARENATEAMINVITEREQUEST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamInviteRequest_Re : public GNET::Protocol
{
	#include "ec_arenateaminviterequest_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
