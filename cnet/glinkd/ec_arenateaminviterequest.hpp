
#ifndef __GNET_EC_ARENATEAMINVITEREQUEST_HPP
#define __GNET_EC_ARENATEAMINVITEREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamInviteRequest : public GNET::Protocol
{
	#include "ec_arenateaminviterequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
	}
};

};

#endif
