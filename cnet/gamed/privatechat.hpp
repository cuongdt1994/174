
#ifndef __GNET_PRIVATECHAT_HPP
#define __GNET_PRIVATECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

void private_chat(int roleid, char channel, const void * buf, unsigned int len, const void * aux_data, unsigned int dsize);
unsigned int handle_chatdata(int uid, const void * aux_data, unsigned int size, void * buffset, unsigned int len);
//bool query_player_level(int role, int & level, int & reputation);

namespace GNET
{

class PrivateChat : public GNET::Protocol
{
	#include "privatechat"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		if(data.size())
		{
			char buffer[1024];
			int size = handle_chatdata(srcroleid, data.begin(), data.size(), buffer , sizeof(buffer));
			data.clear();
			if(size > 0) data.insert(data.begin(), buffer, size);
		}
		src_level = 0;
		int reputation=0;
		query_player_level(srcroleid,src_level,reputation);
		GProviderClient::GetInstance()->DispatchProtocol(0,this);
		//private_chat(srcroleid, channel, msg.begin(), msg.size(), data.begin(), data.size() );
	}
};

};

#endif
