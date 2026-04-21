
#ifndef __GNET_EC_ARENATEAMCAPTAINTRANSFER_HPP
#define __GNET_EC_ARENATEAMCAPTAINTRANSFER_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"


namespace GNET
{

class EC_ArenaTeamCaptainTransfer : public GNET::Protocol
{
	#include "ec_arenateamcaptaintransfer"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		ArenaOfAuroraManager::GetInstance()->ChangeLeader(roleid, transfer_id);
	}
};

};

#endif
