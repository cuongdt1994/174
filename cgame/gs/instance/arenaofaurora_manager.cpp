#include <gsp_if.h>
#include "../world.h"
#include "../player_imp.h"
#include "arenaofaurora_ctrl.h"
#include "arenaofaurora_manager.h"
#include "../aei_filter.h"

world * arenaofaurora_world_manager::CreateWorldTemplate()
{
	world * pPlane  = new world;
	pPlane->Init(_world_index);
	pPlane->InitManager(this);
	
	pPlane->SetWorldCtrl(new arenaofaurora_ctrl());
	return pPlane;
}

world_message_handler * arenaofaurora_world_manager::CreateMessageHandler()
{
	return new arenaofaurora_world_message_handler(this);
}

void arenaofaurora_world_manager::Heartbeat()
{
	_msg_queue.OnTimer(0,100);
	world_manager::Heartbeat();
	unsigned int ins_count = _max_active_index;
	for(unsigned int i = 0; i < ins_count ; i ++)
	{
		if(_planes_state[i] == 0)
		{
			continue;
		}
		world * pPlane = _cur_planes[i];
		if(!pPlane) continue;
		pPlane->RunTick();
	}

	mutex_spinlock(&_heartbeat_lock);
	
	if((++_heartbeat_counter) > TICK_PER_SEC*HEARTBEAT_CHECK_INTERVAL)
	{
		for(unsigned int i = 0; i < ins_count ; i ++)
		{
			if(_planes_state[i] == 0) continue;	//������
			world * pPlane = _cur_planes[i];
			if(!pPlane) continue;
			if(pPlane->w_obsolete)
			{
				//���ڵȴ��ϳ�״̬
				if(pPlane->w_player_count)
				{
					pPlane->w_obsolete = 0;
				}
				else
				{
					if(pPlane->w_destroy_timestamp <= g_timer.get_systime())
					{
						FreeWorld(pPlane,i);
					}
				}
			}
			else
			{
				if(!pPlane->w_player_count)
				{
					pPlane->w_obsolete = 1;
				}
			}
			
		}
		_heartbeat_counter = 0;

		RegroupCoolDownWorld();
	}

	if((++_heartbeat_counter2) > TICK_PER_SEC*HEARTBEAT_CHECK_INTERVAL)
	{
		FillWorldPool();

		_heartbeat_counter2 = 0;
	}

	mutex_spinunlock(&_heartbeat_lock);

}

void arenaofaurora_world_manager::FinalInit(const char * servername)
{
	_npc_idle_heartbeat = 1;
	if(!city_region::QueryTransportExist(GetWorldTag()))
	{
		exit(-1);
	}
}

void arenaofaurora_world_manager::OnDeliveryConnected()
{
	GMSV::EC_SendArenaBattleServerRegister(GetWorldIndex(), GetWorldTag());
}

void arenaofaurora_world_manager::UserLogin(int cs_index,int cs_sid,int uid,const void * auth_data, unsigned int auth_size, bool isshielduser, char flag)
{
	GMSV::SendLoginRe(cs_index,uid,cs_sid,3,flag);
}

void 
arenaofaurora_world_manager::SetFilterWhenLogin(gplayer_imp * pImp, instance_key * ikey)
{
	pImp->_filters.AddFilter(new aeaa_filter(pImp, FILTER_CHECK_INSTANCE_KEY));
}

void arenaofaurora_world_manager::PlayerAfterSwitch(gplayer_imp * pImp)
{
	arenaofaurora_switch_data* pData = substance::DynamicCast<arenaofaurora_switch_data>(pImp->_switch_additional_data);
	if (pData)
	{
		pImp->EnterArenaOfAuroraStep2();
	}
	else
	{
		pImp->ClearSwitchAdditionalData();
	}
}

void arenaofaurora_world_manager::GetLogoutPos(gplayer_imp* pImp, int& world_tag, A3DVECTOR& pos)
{
	pImp->GetLastInstanceSourcePos(world_tag,pos);
}

bool arenaofaurora_world_manager::GetTownPosition(gplayer_imp* pImp, const A3DVECTOR &opos, A3DVECTOR &pos, int& tag)
{
	pos = opos;
	tag = _world_tag;

	return true;
}

void arenaofaurora_world_manager::RecordTownPos(const A3DVECTOR& pos, int faction)
{	
}

void arenaofaurora_world_manager::SetIncomingPlayerPos(gplayer* pPlayer, const A3DVECTOR& origin_pos, int special_mask)
{
	world* pPlane = pPlayer->imp->_plane;
	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl *)pPlane->w_ctrl;

	switch (pCtrl->GetTeamType(pPlayer->ID.id))
	{
	case ARENA_TEAM_RED:
		printf(" [ arenaofaurora_world_manager::SetIncomingPlayerPos ] %d is red team\n", pPlayer->ID.id);
		pPlayer->pos = pCtrl->_trans_pos_1;
		//pPlayer->SetBattleOffense();
		break;
	case ARENA_TEAM_BLUE:
		printf(" [ arenaofaurora_world_manager::SetIncomingPlayerPos ] %d is blue team\n", pPlayer->ID.id);
		pPlayer->pos = pCtrl->_trans_pos_2;
		//pPlayer->SetBattleDefence();
		break;
	default:
		printf(" [ arenaofaurora_world_manager::GetTownPosition ] %d is undefined player\n", pPlayer->ID.id);
		pPlayer->pos = A3DVECTOR(0, 0, 0);
		break;
	}

	//instance_world_manager::SetIncomingPlayerPos(pPlayer, origin_pos, special_mask);
}

int arenaofaurora_world_manager::CheckPlayerSwitchRequest(const XID& who, const instance_key* ikey, const A3DVECTOR& pos, int ins_timer)
{
	if (ikey->target.key_level4 == 0)
	{
		printf("CheckPlayerSwitchRequest, ikey->target.key_level4 == 0\n");
		return S2C::ERR_CANNOT_ENTER_INSTANCE;
	}

	instance_hash_key key;
	TransformInstanceKey(ikey->target,key);
	world *pPlane = NULL;
	int rst = 0;
	mutex_spinlock(&_key_lock);
	int * pTmp = _key_map.nGet(key);
	if(!pTmp)
	{
		mutex_spinunlock(&_key_lock);
		printf("CheckPlayerSwitchRequest, plane not found 1\n");
		return S2C::ERR_BATTLEFIELD_IS_CLOSED;
	}
	pPlane = _cur_planes[*pTmp];
	if(pPlane)
	{
		if(pPlane->w_player_count >= _player_limit_per_instance)
		{
			//��������������
			rst = S2C::ERR_TOO_MANY_PLAYER_IN_INSTANCE;
		}
		else
		{
			arenaofaurora_ctrl * pCtrl = (arenaofaurora_ctrl*)pPlane->w_ctrl;

			//��������Ƿ��Ѿ�����
			/*if(pCtrl->_data.attacker_count >= pCtrl->_data.player_count_limit 
					&& pCtrl->_data.defender_count >= pCtrl->_data.player_count_limit)
			{
				rst = S2C::ERR_TOO_MANY_PLAYER_IN_INSTANCE;
			}*/

			if(!rst)
			{
				//��������Ƿ��Ѿ������ر�
				if(pCtrl->_data.end_time <= g_timer.get_systime())
				{
					rst = S2C::ERR_BATTLEFIELD_IS_CLOSED;
				}
				/*else if (pPlane->w_battle_result)
				{
					rst = S2C::ERR_BATTLEFIELD_IS_FINISHED;
				}*/
			}
		}
	}
	else
	{
		rst = S2C::ERR_CANNOT_ENTER_INSTANCE;
		printf("CheckPlayerSwitchRequest, plane not found 2\n");
	}

	//�����ҵ������� ״̬�����������Ƿ�ƥ��
	mutex_spinunlock(&_key_lock);
	return rst;
}

world * arenaofaurora_world_manager::GetWorldInSwitch(const instance_hash_key& ikey, int& world_index, int)
{
	spin_autolock keeper(_key_lock);
	world *pPlane = NULL;
	int * pTmp = _key_map.nGet(ikey);
	world_index = -1;
	if(pTmp)
	{
		//�������������� 
		world_index = *pTmp;;
		pPlane = _cur_planes[world_index];
		ASSERT(pPlane);

		//����Ҫ��������Ƿ���������  ����������¼,��ֱ�ӷ���NULL
		//$$$$$$ 
		
		pPlane->w_obsolete = 0;
	}
	if(world_index < 0) return NULL;
	return pPlane;
}

bool arenaofaurora_world_manager::CreateArenaOfAurora(const GMSV::EC_GSArena & param)
{
	spin_autolock keeper(_key_lock);

	instance_hash_key hkey;
	hkey.key1 = param.arena_id;
	hkey.key2 = 0;
	int world_index;
	world* pPlane = AllocWorldWithoutLock(hkey, world_index);

	if (pPlane == NULL)
	{
		return false;
	}

	arenaofaurora_ctrl* pCtrl = dynamic_cast<arenaofaurora_ctrl*>(pPlane->w_ctrl);
	if (pCtrl == NULL)
	{
		ASSERT(false);
		return false;
	}

	if (pCtrl->_data.arena_id != 0)
	{
		return false;
	}

	pCtrl->_data = param;

	/*
	pCtrl->_data.arena_id = param.arena_id;

	for (unsigned int i = 0; i < param.red_count && i < 6; i++)
	{
		pCtrl->_data.init_members_red.push_back(param.red[i]);
	}

	for (unsigned int i = 0; i < param.blue_count && i < 6; i++)
	{
		pCtrl->_data.init_members_blue.push_back(param.blue[i]);
	}
	
	pCtrl->_data.player_count_limit = 12;

	pCtrl->_data.end_time = param.end_time;
	*/

	pCtrl->_red_damage = 0;
	pCtrl->_blue_damage = 0;
	pCtrl->_battle_timer = -80;

	pPlane->w_destroy_timestamp = param.end_time + 600;
	printf("create arenaofaurora arenaid=%d, end_timestamp=%d, destroy=%d, now=%d red_count=%d , blue_count=%d \n", (int)param.arena_id, (int)param.end_time, (int)pPlane->w_destroy_timestamp, (int)g_timer.get_systime(), pCtrl->_data.red_members.size() , pCtrl->_data.blue_members.size() );

	if (world_manager::GetWorldTag() == 179)
	{
		pCtrl->_trans_pos_1 = A3DVECTOR(-306.452000, 47.000000, 271.366000);
		pCtrl->_trans_pos_2 = A3DVECTOR(-306.355000, 47.000000, 352.418000);
		pCtrl->_target_world_tag = 179;
	}
	else if (world_manager::GetWorldTag() == 187)
	{
		pCtrl->_trans_pos_1 = A3DVECTOR(162.800000, 33.000000, -13.000000);
		pCtrl->_trans_pos_2 = A3DVECTOR(23.500000, 34.000000, -12.400000);
		pCtrl->_target_world_tag = 187;
	}
	else if (world_manager::GetWorldTag() == 188)
	{
		pCtrl->_trans_pos_1 = A3DVECTOR(-67.900000, 68.000000, 41.000000);
		pCtrl->_trans_pos_2 = A3DVECTOR(58.900000, 68.000000, 35.800000);
		pCtrl->_target_world_tag = 188;
	}
	else if (world_manager::GetWorldTag() == 189)
	{
		pCtrl->_trans_pos_1 = A3DVECTOR(89.400000, 35.200000, 108.700000);
		pCtrl->_trans_pos_2 = A3DVECTOR(-53.300000, 35.200000, 108.600000);
		pCtrl->_target_world_tag = 189;
	}

	return true;
}

void arenaofaurora_world_message_handler::PlayerPreEnterServer(gplayer* pPlayer, gplayer_imp* pImp,instance_key& ikey)
{
	world* pPlane = pImp->_plane;
	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)(pPlane->w_ctrl);

	switch (pCtrl->GetTeamType(pPlayer->ID.id))
	{
	case ARENA_TEAM_RED:
		printf(" [ arenaofaurora_world_message_handler::PlayerPreEnterServer ] %d is red team\n", pPlayer->ID.id);
		pPlayer->SetBattleOffense();
		break;
	case ARENA_TEAM_BLUE:
		printf(" [ arenaofaurora_world_message_handler::PlayerPreEnterServer ] %d is blue team\n", pPlayer->ID.id);
		pPlayer->SetBattleDefence();
		break;
	default:
		printf(" [ arenaofaurora_world_message_handler::PlayerPreEnterServer ] %d is undefined player\n", pPlayer->ID.id);
		break;
	}
}

