
#ifndef __GNET_FACTIONALLIANCECHAT_HPP
#define __GNET_FACTIONALLIANCECHAT_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "gfactionserver.hpp"
#include "base64.h"
#include "localmacro.h"
#include "factionmsg.hpp"


namespace GNET
{

class FactionAllianceChat : public GNET::Protocol
{
	#include "factionalliancechat"

	struct MEMBERS
	{
		size_t count;
		struct
		{
			unsigned int localsid;
			unsigned int linkid;
		}	member[512];
	};
	
	struct MEMBER
	{
		unsigned int name_len;
		unsigned char name[64];
		unsigned int localsid;
		unsigned int linkid;
	};

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		GFactionServer* gfs=GFactionServer::GetInstance();
		unsigned int src_factionid=_FACTION_ID_INVALID;
		unsigned int src_link_id = -1;
		MEMBER smem;
		if(channel==GN_CHAT_CHANNEL_ALLIANCE)
		{
			alliance.clear();
			Thread::Mutex::Scoped l(gfs->locker_player);
			if(Factiondb::GetInstance()->AlreadyDelayExpel(src_roleid)) // ·ЗСУіЩЙѕіэ
				return;
			
			GFactionServer::PlayerMap::const_iterator it=gfs->playermap.find(src_roleid);
			if (it!=gfs->playermap.end()) {
				src_factionid=(*it).second->faction_id;
				src_link_id=(*it).second->linkid;
				emotion = (*it).second->emotion;
				memset(&smem, 0x00, sizeof(MEMBER));
				smem.name_len = (*it).second->name.size();
				memcpy(smem.name, (*it).second->name.begin(), smem.name_len);
			}
			if (src_factionid!=_FACTION_ID_INVALID && src_link_id != -1 && gfs->factionmembermap.count(src_factionid)!=0)
			{
				int afids[64];
				memset(afids, 0x00, sizeof(afids));
				afids[0] = src_factionid;
				Factiondb::GetInstance()->GetAlliance(src_factionid, &afids[1]);
				
				MEMBERS members;
				memset(&members, 0x00, sizeof(members));
				
				for(size_t i=0; afids[i] && i < 64; i++)
				{
					
					for(GFactionServer::FactionMemberMap::iterator
						itb=gfs->factionmembermap.lower_bound(afids[i]),
						ite=gfs->factionmembermap.upper_bound(afids[i]);
						itb!=ite; ++itb)
					{
						if ((*itb).second && members.count < 512)
						{
							members.member[members.count].localsid 	= (*itb).second->localsid;
							members.member[members.count].linkid 	= (*itb).second->linkid;
							members.count++;
						}
					}
				}
				
				for(size_t j=0; j < members.count; j++)
				{
					smem.localsid = members.member[j].localsid;
					smem.linkid = members.member[j].linkid;
					alliance.replace(&smem, sizeof(smem));
					GFactionServer::GetInstance()->DispatchProtocol(members.member[j].linkid,this);
				}

				//printf("FactionAllianceChat: roleid = %d , faction = %d , afids[0] = %d , members.count = %d \n", src_roleid, src_factionid, afids[0], members.count );
			}
			
			
		}
		
		// TODO
	}
};

};

#endif
