
#ifndef __GNET_CHATROOMSPEAK_HPP
#define __GNET_CHATROOMSPEAK_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "base64.h"

namespace GNET
{

class ChatRoomSpeak : public GNET::Protocol
{
	#include "chatroomspeak"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GDeliveryClient* dcm = GDeliveryClient::GetInstance();
		GLinkServer*     lsm = GLinkServer::GetInstance();
		
		if(manager==dcm)
		{
			unsigned int id = localsid;
			this->localsid = 0;
			GLinkServer::GetInstance()->Send(id,this);	
		}
		else
		{
			SessionInfo * sinfo = lsm->GetSessionInfo(sid);
			if (!sinfo || src<=0 || !sinfo->policy.Update(WHISPER_POLICY)) return;
			if(message.size()>256 || message.size()<2) return;	
			
			if (!GLinkServer::ValidRole(sid,src))
			{
				lsm->SessionError(sid,ERR_INVALID_ACCOUNT,"Error userid or roleid.");
				return;
			}

			if (lsm->GetCountSmile((unsigned char*)message.begin(),message.size()) > 3) return;
			
			{
				Octets out;
				Base64Encoder::Convert(out, message);
				Log::log(LOG_CHAT, "Group: room=%d src=%d msg=%.*s", roomid, src, out.size(), (char*)out.begin()); 
			}
			localsid = sid;
			dcm->SendProtocol(this);
		}
	}
};

};

#endif
