
#ifndef __GNET_FACTIONALLIANCECHAT_HPP
#define __GNET_FACTIONALLIANCECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#define _FACTIONSERVER_ID 101

unsigned int handle_chatdata(int uid, const void * aux_data, unsigned int size, void * buffer, unsigned int len);
bool check_alliance_chat(int uid, char channel);
namespace GNET
{

class FactionAllianceChat : public GNET::Protocol
{
	#include "factionalliancechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(check_alliance_chat(src_roleid, channel))
		{
			if(data.size())
			{
				char buffer[1024];
				int size = handle_chatdata(src_roleid, data.begin(), data.size(), buffer , sizeof(buffer));
				data.clear();
				if(size > 0) data.insert(data.begin(), buffer, size);
			}
			GProviderClient::GetInstance()->DispatchProtocol(_FACTIONSERVER_ID, this);
		}
	}
};

};

#endif
