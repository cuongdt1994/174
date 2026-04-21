
#ifndef __GNET_FACTIONALLIANCECHAT_HPP
#define __GNET_FACTIONALLIANCECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

namespace GNET
{

class FactionAllianceChat : public GNET::Protocol
{
	#include "factionalliancechat"

	struct MEMBER
	{
		unsigned int name_len;
		unsigned char name[64];
		unsigned int localsid;
		unsigned int linkid;
	};

	void SendForbidInfo(GLinkServer* lsm,Manager::Session::ID sid,const GRoleForbid& forbid)
	{
		lsm->Send(sid,AnnounceForbidInfo(src_roleid,_SID_INVALID,forbid));
	}
	void Process(Manager *manager, Manager::Session::ID sid)
	{
		//转发帮派服务发来的聊天信息
		if (manager==GFactionClient::GetInstance())
		{
			if(alliance.size() == sizeof(MEMBER))
			{
				MEMBER member;
				memcpy(&member, alliance.begin(), alliance.size() );
			
				if ( GLinkServer::IsRoleOnGame( member.localsid ) )
				{
					if(channel!=GN_CHAT_CHANNEL_SYSTEM)
					{
						alliance.replace(member.name, member.name_len);
						GLinkServer::GetInstance()->Send(member.localsid,this);
					}
					else
					{
						ChatMessage chatmsg(GN_CHAT_CHANNEL_SYSTEM,0,src_roleid,msg,data);
						GLinkServer::GetInstance()->Send(member.localsid,chatmsg);
					}
				}
			}
		}
		//转发客户端发来的聊天信息
		else if(manager==GLinkServer::GetInstance())
		{
			if(msg.size()>256 || msg.size()<2 || data.size()>8)
				return;
			GLinkServer* lsm=GLinkServer::GetInstance();
			
			if (lsm->GetCountSmile((unsigned char*)msg.begin(),msg.size()) > 3) 
				return;	
				
			SessionInfo * sinfo = lsm->GetSessionInfo(sid);
			if (!sinfo || sinfo->roleid!=src_roleid || src_roleid<=0 || !sinfo->policy.Update(CHAT_POLICY))
				return;
			GRoleForbid forbid;
			if (lsm->IsForbidChat(src_roleid, sinfo->userid, forbid))
			{
				SendForbidInfo(lsm, sid, forbid);
				return;
			}
			//GFactionClient::GetInstance()->SendProtocol(this);
			GProviderServer::GetInstance()->DispatchProtocol(sinfo->gsid,this);
		}
	}
	
};

};

#endif
