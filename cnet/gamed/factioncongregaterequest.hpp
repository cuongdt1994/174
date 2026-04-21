
#ifndef __GNET_FACTIONCONGREGATEREQUEST_HPP
#define __GNET_FACTIONCONGREGATEREQUEST_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void RecvFactionCongregateRequest(int factionid, int roleid, int sponsor, void * data, unsigned int size);

namespace GNET
{

class FactionCongregateRequest : public GNET::Protocol
{
	#include "factioncongregaterequest"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		// TODO
		for(unsigned int i=0; i<member.size(); i++)
		{
			RecvFactionCongregateRequest(factionid, member[i], sponsor, data.begin(), data.size());
		}
	}
};

};

#endif
