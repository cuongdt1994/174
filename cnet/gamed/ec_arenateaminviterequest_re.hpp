
#ifndef __GNET_EC_ARENATEAMINVITEREQUEST_RE_HPP
#define __GNET_EC_ARENATEAMINVITEREQUEST_RE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void EC_RecvArenaTeamInviteRequest_Re(int invited_roleid, int roleid, int team_type);

namespace GNET
{

class EC_ArenaTeamInviteRequest_Re : public GNET::Protocol
{
	#include "ec_arenateaminviterequest_re"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		EC_RecvArenaTeamInviteRequest_Re(invited_roleid, roleid, team_type);
	}
};

};

#endif
