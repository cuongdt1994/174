#include "glinkserver.hpp"
#include "gdeliveryclient.hpp"
#include "statusannounce.hpp"
#include "offmarket.hpp"

OfflineMarket* OfflineMarket::instance = 0;

void OfflineMarket::Cancel(void * data, size_t size)
{
	player_cancel_market* pcm = (player_cancel_market*)data;
	if(size >= sizeof(player_cancel_market) && pcm->cmd == PLAYER_CANCEL_MARKET && pcm->pid >= MIN_ROLEID)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		RoleData* role = lsm->GetRoleInfo(pcm->pid);
		if(role)
		{
			SessionInfo * sinfo = lsm->GetSessionInfo(role->sid);
			if (sinfo && sinfo->market_state)
			{
				if(sinfo->market_state == 2) //только если в оффлайн коте
				{
					sinfo->market_state = 0;
					lsm->Close(role->sid);
					lsm->ActiveCloseSession(role->sid);
					GDeliveryClient::GetInstance()->SendProtocol(StatusAnnounce(sinfo->userid, role->sid, _STATUS_OFFLINE));
				}
				else
				{
					sinfo->market_state = 0;
				}
			}
		}
	}
}

void OfflineMarket::Open(void * data, size_t size, int sid)
{
	self_open_market* som = (self_open_market*)data;
	
	if(size >= sizeof(self_open_market) && som->cmd == SELF_OPEN_MARKET && (som->count & 0x8000) )
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);
		if(sinfo && !sinfo->market_state && sinfo->roleid >= MIN_ROLEID)
		{
			som->count ^= 0x8000;
			sinfo->market_state = 1;
		}
	}
}

void OfflineMarket::Info(void * data, size_t size)
{
	player_market_info* pmi = (player_market_info*)data;
	if(size >= sizeof(player_market_info) && pmi->cmd == PLAYER_MARKET_INFO && pmi->pid >= MIN_ROLEID && pmi->count <= MAX_ITEMS_MARKET)
	{
		GLinkServer* lsm=GLinkServer::GetInstance();
		RoleData* role = lsm->GetRoleInfo(pmi->pid);
		if(role)
		{
			size_t item_on_market = 0;
			for (int i = 0; i < pmi->count && i < MAX_ITEMS_MARKET; i++)
			{
				if( pmi->item_list[i] ) { item_on_market++; break; }
			}
			
			if(!item_on_market) //если итемы кончились 
			{
				SessionInfo * sinfo = lsm->GetSessionInfo(role->sid);
				if (sinfo && sinfo->market_state == 2) //только если в оффлайн коте
				{
					sinfo->market_state = 0;
					lsm->Close(role->sid);
					lsm->ActiveCloseSession(role->sid);
					GDeliveryClient::GetInstance()->SendProtocol(StatusAnnounce(sinfo->userid, role->sid, _STATUS_OFFLINE));
				}
			}
		}
	}
}



