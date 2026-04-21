#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include <arandomgen.h>
#include "npc.h"
#include "world.h"
#include "usermsg.h"
#include "clstab.h"
#include "ainpc.h"
#include "actsession.h"
#include "npcgenerator.h"
#include "item.h"
#include "playertemplate.h"
#include "npc_filter.h"
#include "pathfinding/pathfinding.h"
#include "global_drop.h"
#include "antiwallow.h"
#include "faction.h"
#include "luamanager.h"
#include "player_imp.h"


DEFINE_SUBSTANCE(gnpc_imp,gobject_imp,CLS_NPC_IMP)
DEFINE_SUBSTANCE(gnpc_controller,controller,CLS_NPC_CONTROLLER)
DEFINE_SUBSTANCE(gnpc_dispatcher,dispatcher,CLS_NPC_DISPATCHER)

void 
gnpc_dispatcher::enter_slice(slice *pPiece ,const A3DVECTOR &pos)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc* pNPC = (gnpc*)_imp->_parent;
	CMD::Make<CMD::npc_enter_slice>::From(h1,pNPC,pos);

	cs_user_map map; 
	pPiece->Lock();
	if(pNPC->IsInvisible())
		gather_slice_cs_user_in_invisible(pPiece,map,pNPC->invisible_degree,0);
	else
		gather_slice_cs_user(pPiece,map);
	pPiece->Unlock();
	if(map.size()) multi_send_ls_msg(map,h1);
}

void 
gnpc_dispatcher::leave_slice(slice *pPiece ,const A3DVECTOR &pos)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc* pNPC = (gnpc*)_imp->_parent;
	CMD::Make<CMD::leave_slice>::From(h1,pNPC);
	
	cs_user_map map; 
	pPiece->Lock();
	if(pNPC->IsInvisible())
		gather_slice_cs_user_in_invisible(pPiece,map,pNPC->invisible_degree,0);
	else
		gather_slice_cs_user(pPiece,map);
	pPiece->Unlock();
	if(map.size()) multi_send_ls_msg(map,h1);
}

void
gnpc_dispatcher::enter_world()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc* pNPC = (gnpc*)_imp->_parent;
	slice *pPiece = pNPC->pPiece;
	CMD::Make<CMD::npc_enter_world>::From(h1,pNPC);
	if(pNPC->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,h1,pNPC->invisible_degree);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);

	//��ʱ�Ȳ������ڱ߽����������
	if(pPiece->IsBorder())
	{
		extern_object_manager::SendAppearMsg<0>(_imp->_plane,pNPC,pPiece);
	}
}

void
gnpc_dispatcher::move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pNPC = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_move>::From(h1,pNPC,target,cost_time,speed,move_mode);
//	__PRINTF("npc move:(%f %f %f)\n",target.x,target.y,target.z);

	slice * pPiece = pNPC->pPiece;
	if(pNPC->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,h1,pNPC->invisible_degree);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void
gnpc_dispatcher::stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mode)
{
	if(_imp->_parent->IsZombie()) 
	{
		//��������˾Ͳ��ط�����
		return ;
	}

	if(!((gnpc_imp*)_imp)->_fixed_direction)
		((gactive_imp*)_imp)->RecalcDirection();
	if(((gnpc_imp*)_imp)->_direction.squared_magnitude() < 1e-3)
	{
		__PRINTF("���һ���ƶ��ı�����С\n");
	}

	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc* pNPC = (gnpc*)_imp->_parent;
//	CMD::Make<CMD::npc_stop_move>::From(h1,pObj,speed,pObj->dir,move_mode);
	CMD::Make<CMD::object_stop_move>::From(h1,pNPC,target,speed,pNPC->dir,move_mode);

	slice * pPiece = pNPC->pPiece;
	if(pNPC->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,h1,pNPC->invisible_degree);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::on_death(const XID & attacker,bool is_delay)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	if(is_delay)
	{
		CMD::Make<CMD::npc_dead_2>::From(h1,pObj,attacker);
	}
	else
	{
		CMD::Make<CMD::npc_dead>::From(h1,pObj,attacker);
	}
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1,-1);
}

void 
gnpc_dispatcher::dodge_attack(const XID &attacker, int skill_id, const attacker_info_t& info, int at_state,char speed,bool orange,unsigned char section)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject *pObj = _imp->_parent;
	if(skill_id)
	{
		if(attacker.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			CMD::Make<CMD::self_skill_attack_result>::From(h1,pObj->ID,skill_id,0,at_state,speed,section);
			send_ls_msg(info.cs_index,attacker.id,info.sid,h1.data(),h1.size());
			h1.clear();
			CMD::Make<CMD::object_skill_attack_result>::From(h1,attacker,pObj->ID,skill_id,0,at_state ,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,attacker.id);
		}
		else
		{
			CMD::Make<CMD::object_skill_attack_result>::From(h1,attacker,pObj->ID,skill_id,0,at_state ,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
		}
	}
	else
	{
		if(attacker.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			CMD::Make<CMD::self_attack_result>::From(h1,pObj->ID,0,at_state,speed);
			send_ls_msg(info.cs_index,attacker.id,info.sid,h1.data(),h1.size());
			h1.clear();
			CMD::Make<CMD::object_attack_result>::From(h1,attacker,pObj->ID,0,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,attacker.id);
		}
		else
		{
			CMD::Make<CMD::object_attack_result>::From(h1,attacker,pObj->ID,0,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
		}
	}

}

void 
gnpc_dispatcher::be_hurt(const XID & id, const attacker_info_t & info,int damage,bool invader)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj = _imp->_parent;
	if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
	{
		CMD::Make<CMD::hurt_result>::From(h1,pObj->ID,damage);
		send_ls_msg(info.cs_index,id.id,info.sid,h1.data(),h1.size());
		h1.clear();
	}
}
	
void
gnpc_dispatcher::be_damaged(const XID & id, int skill_id, const attacker_info_t & info,int damage,int,int at_state,char speed,bool orange,unsigned char section)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	__PRINTF (" be damaged\n");
	gobject * pObj = _imp->_parent;
	if(skill_id)
	{
		if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			__PRINTF("�������Լ��Ĺ�����Ϣ\n");
			CMD::Make<CMD::self_skill_attack_result>::From(h1,pObj->ID,skill_id,damage,at_state,speed,section);
			send_ls_msg(info.cs_index,id.id,info.sid,h1.data(),h1.size());
			h1.clear();
			CMD::Make<CMD::object_skill_attack_result>::From(h1,id,pObj->ID,skill_id,damage,at_state ,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,id.id);
		}
		else
		{
			CMD::Make<CMD::object_skill_attack_result>::From(h1,id,pObj->ID,skill_id,damage,at_state ,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
		}
	}
	else
	{
		if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			__PRINTF("�������Լ��Ĺ�����Ϣ\n");
			CMD::Make<CMD::self_attack_result>::From(h1,pObj->ID,damage,at_state,speed);
			send_ls_msg(info.cs_index,id.id,info.sid,h1.data(),h1.size());
			h1.clear();
			CMD::Make<CMD::object_attack_result>::From(h1,id,pObj->ID,damage,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,id.id);
		}
		else
		{
			CMD::Make<CMD::object_attack_result>::From(h1,id,pObj->ID,damage,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
		}
	}
}

void
gnpc_dispatcher::cast_skill(const XID & target, int skill,unsigned short time, unsigned char level)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gobject * pObj= (gobject*)pImp->_parent;
	CMD::Make<CMD::object_cast_skill>::From(h1,pObj->ID,target,skill,time,level);
	AutoBroadcastCSMsg(pImp->_plane,pObj->pPiece,h1,-1);
}

void
gnpc_dispatcher::skill_interrupt(char reason)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp*)_imp;
	gobject * pObj= (gobject*)pImp->_parent;
	CMD::Make<CMD::skill_interrupted>::From(h1,pObj->ID);
	AutoBroadcastCSMsg(pImp->_plane,pObj->pPiece,h1,-1);
}

void
gnpc_dispatcher::notify_root(unsigned char type)
{
	if(type & 0x80) return;
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::notify_root>::From(h1,pObject);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}


void
gnpc_dispatcher::takeoff()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_takeoff>::From(h1,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
}

void 
gnpc_dispatcher::landing()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_landing>::From(h1,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,h1,-1);
}

void 
gnpc_dispatcher::disappear()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pNPC = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(h1,pNPC);
	slice * pPiece = pNPC->pPiece;
	if(pNPC->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,h1,pNPC->invisible_degree);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);

	//��ʱ�Ȳ������ڱ߽����������
	if(pPiece->IsBorder())
	{
		extern_object_manager::SendDisappearMsg<0>(_imp->_plane,pNPC,pPiece);
	}
}

void 
gnpc_dispatcher::query_info00(const XID & target, int cs_index,int sid)
{
	packet_wrapper h1(64);
	using namespace S2C;
	gactive_imp * pImp = (gactive_imp *)_imp;
	//_back_up[1];
	CMD::Make<CMD::npc_info_00>::From(h1,pImp->_parent->ID,pImp->_basic.hp,pImp->_basic,pImp->_cur_prop,pImp->GetCurTarget().id);
	send_ls_msg(cs_index,target.id,sid,h1);
}

void 
gnpc_dispatcher::query_info_1(int uid,int cs_index, int cs_sid)
{
	packet_wrapper h1(64);
	using namespace S2C;
	CMD::Make<multi_data_header>::From(h1,NPC_INFO_LIST,1);
	CMD::Make<INFO::npc_info>::From(h1,(gnpc*)_imp->_parent);
	send_ls_msg(cs_index,uid,cs_sid,h1);
}

void 
gnpc_dispatcher::send_turrent_leader(int id)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::turret_leader_notify>::From(h1,pObj->ID, id);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::level_up()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::level_up>::From(h1,pObj);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::appear_to_spec(int invi_degree)
{
	on_dec_invisible(invi_degree,0);
}

void 
gnpc_dispatcher::disappear_to_spec(int invi_degree)
{
	on_inc_invisible(0,invi_degree);
}

void 
gnpc_dispatcher::on_inc_invisible(int prev_invi_degree, int cur_invi_degree)
{
	ASSERT(cur_invi_degree > prev_invi_degree);
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pNPC = (gnpc *)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(h1,pNPC);
	slice * pPiece = pNPC->pPiece;
	AutoBroadcastCSMsgToSpec(_imp->_plane,pPiece,h1,cur_invi_degree,prev_invi_degree);
}

void 
gnpc_dispatcher::on_dec_invisible(int prev_invi_degree, int cur_invi_degree)
{
	ASSERT(cur_invi_degree < prev_invi_degree);
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc* pNPC = (gnpc*)_imp->_parent;
	CMD::Make<CMD::npc_enter_world>::From(h1,pNPC);
	slice * pPiece = pNPC->pPiece;
	AutoBroadcastCSMsgToSpec(_imp->_plane,pPiece,h1,prev_invi_degree,cur_invi_degree);
}

void 
gnpc_dispatcher::start_play_action(char action_name[128],int play_times,int action_last_time,int interval_time)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_start_play_action>::From(h1,pObj->ID,play_times,action_last_time,interval_time,strlen(action_name),action_name);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::stop_play_action()
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_stop_play_action>::From(h1,pObj->ID);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::forbid_be_selected(char b)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::object_forbid_be_selected>::From(h1,pObj->ID.id, b);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::add_multiobj_effect(int target, char type)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::add_multiobj_effect>::From(h1,pObj->ID.id, target, type);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::remove_multiobj_effect(int target, char type)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::remove_multiobj_effect>::From(h1,pObj->ID.id, target, type);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

void 
gnpc_dispatcher::notify_visible_tid_change() 
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gnpc * pObj = (gnpc*)_imp->_parent;
	CMD::Make<CMD::npc_visible_tid_notify>::From(h1,pObj->ID.id,pObj->vis_tid);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,h1);
}

gnpc_controller::gnpc_controller()
	:_svr_belong(-1),_extern_svr(-1),_ai_core(0),_cry_for_help_timestamp(0),_ignore_range(70.f),_cur_target_cache(-1,-1),_cache_refresh_state(false)
{
}
int
gnpc_controller::CommandHandler(int cmd_type,const void * buf, unsigned int size)
{
	ASSERT(false && "��ͨNPC ��ô����ܵ���Ϣ�أ��պ�");
	return 0;
}

void gnpc_controller::CheckNPCData()
{
	if(_ai_core) _ai_core->CheckNPCData();
}

int 
gnpc_controller::MH_MOVEOUT(world * pPlane, const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_NPC_SVR_UPDATE:
			__PRINTF("�ⲿ������·�����\n");
			break;
		case GM_MSG_EXT_NPC_DEAD:
			__PRINTF("�ⲿ��������\n");
			/*
			{
				gnpc_imp * pImp = (gnpc_imp*)_imp;
				pImp->_parent->b_zombie = true;
				session_npc_zombie *pSession = new session_npc_zombie(pImp);
				pSession->SetDelay(200);	//10�����ӳ�
				pImp->AddSession(pSession);
				pImp->StartSession();
			}*/
			//����ֱ��ִ���ͷŲ���
			//�����κ��ӳ�
			_imp->_parent->b_zombie = true;
			((gnpc_imp*)_imp)->LifeExhaust();
			break;
		case GM_MSG_EXT_NPC_HEARTBEAT:
			//Ҫ����֮
			{
				_imp->_parent->pos = msg.pos;
				gnpc_imp *pImp = ((gnpc_imp*)_imp);
				pImp->_basic.hp = msg.param;
				if(pImp->_native_notify) pImp->_native_notify->OnUpdate(pImp);
			}
			break;

		case GM_MSG_HEARTBEAT:
			{
				gnpc_imp *pImp = ((gnpc_imp*)_imp);
				if(pImp->_native_notify) 
				{
					if(pImp->_native_notify->MoveOutHeartbeat(pImp,msg.param) < 0)
					{
						//��ʱ�������ù�����ʧ
						//pPlane->SendRemoteMessage(_extern_svr,tmpmsg);
						_imp->_parent->b_zombie = true;
						pImp->LifeExhaust();
					}
				}
			}
			break;
			
		case GM_MSG_OBJ_ZOMBIE_END:
			((gnpc_imp*)_imp)->LifeExhaust();
			return 0;

		case GM_MSG_EXT_AGGRO_FORWARD:
			ASSERT(msg.content_length == sizeof(XID));
			((gnpc_imp*)_imp)->ForwardFirstAggro(*(XID*)msg.content,msg.param);
			return 0;

		default:
			//�����δ��������Ϣ����ôת������֪��Զ�̷�������
			if(msg.ttl > 0  && _extern_svr >= 0)
			{
				MSG tmpmsg = msg;
				tmpmsg.ttl --;
				pPlane->SendRemoteMessage(_extern_svr,tmpmsg);
			}
			break;
	}
	return 0;
}


bool 
gnpc_controller::Save(archive & ar)
{
	ar << _svr_belong ;
	//ar << _cur_target;
	ar << _ignore_range;
	ar << _cur_target_cache;
	ar << _cache_refresh_state;
	return _ai_core->Save(ar);
}

bool 
gnpc_controller::Load(archive & ar)
{
	ar >> _svr_belong ;
	//ar >> _cur_target;
	ar >> _ignore_range;
	ar >> _cur_target_cache;
	ar >> _cache_refresh_state;
	_extern_svr = -1;
	ASSERT(_ai_core == NULL);

	_ai_core = new gnpc_ai();
	return _ai_core->Load(ar);
}

/*
int 
gnpc_controller::DispatchControllerMessage(world * pPlane, const MSG & msg)
{
	switch(_npc_state)
	{
		case NPC_STATE_MOVEOUT:
			return MH_MOVEOUT(pPlane,msg);
			break;
	}
	return MessageHandler(pPlane, msg);
}
*/

int 
gnpc_controller::GetFactionAskHelp()
{
	if(_ai_core) 
		return _ai_core->GetFactionAskHelp();
	else
		return 0;
}

void 
gnpc_controller::SetLifeTime(int life)
{
	if(_ai_core) _ai_core->SetLife(life);
}

//void 
//gnpc_controller::SetDieWithLeader(bool val)
//{
//	if(_ai_core) _ai_core->SetDieWithLeader(val);
//}

void 
gnpc_controller::SetDieWithWho(char val)
{
	if(_ai_core) _ai_core->SetDieWithWho(val);
}

int 
gnpc_controller::MessageHandler(world * pPlane, const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_WATCHING_YOU:
			if(msg.content_length == sizeof(msg_watching_t))
			{
				msg_watching_t * pMsg = (msg_watching_t *)msg.content;
				//�ж��ܲ��ܿ���
				gnpc * pNPC = (gnpc*)_imp->_parent;
				if(pNPC->anti_invisible_degree < pMsg->invisible_degree) return 0;

				ActiveCombatState(true);
				if((pMsg->faction & ((gactive_imp*)_imp)->GetEnemyFaction()) && _ai_core)
				{
					 _ai_core->AggroWatch(msg);
				}
			}
			else
			{
				ASSERT(false);
			}
			return 0;
		break;
		case GM_MSG_GEN_AGGRO:
			ActiveCombatState(true);
			if(_ai_core) _ai_core->AggroGen(msg);
			return 0;
		break;
		case GM_MSG_TRANSFER_AGGRO:
			ActiveCombatState(true);
			if(_ai_core) _ai_core->AggroTransfer(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_ALARM:
			ActiveCombatState(true);
			if(_ai_core) _ai_core->AggroAlarm(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_WAKEUP:
			ActiveCombatState(true);
			if(_ai_core) _ai_core->AggroWakeUp(msg);
			return 0;
		break;
		case GM_MSG_AGGRO_TEST:
			ActiveCombatState(true);
			if(_ai_core) _ai_core->AggroTest(msg);
			return 0;
		case GM_MSG_NPC_CRY_FOR_HELP:
			if(msg.content_length == sizeof(msg_cry_for_help_t)) 
			{
				msg_cry_for_help_t & mcht = *(msg_cry_for_help_t*)msg.content;
				if(mcht.helper_faction & ((gnpc*)(_imp->_parent))->monster_faction)
				{
					if(_ai_core) _ai_core->AggroHelp(mcht.attacker,mcht.lamb_faction);
				}
			}
			else
			{
				ASSERT(false);
			}
		return 0;

		case GM_MSG_PLAYER_KILLED_BY_NPC:
			if(_ai_core) _ai_core->KillTarget(msg.target);
		break;

		case GM_MSG_BECOME_TURRET_MASTER:
		{
			ASSERT(msg.content_length == sizeof(int));
			gnpc_imp * pImp = (gnpc_imp*)_imp;
			gnpc * pParent = (gnpc*)(pImp->_parent);
			int faction = *(int*)msg.content;
			if(pParent->pos.squared_distance(msg.pos) < 10.f*10.f && (faction & pImp->GetFaction()))
			{
				int tid  = msg.param;
				if(_ai_core)
				{
					if(_ai_core->ChangeTurretMaster(msg.source))
					{
						pImp->SendTo<0>(GM_MSG_REMOVE_ITEM,msg.source,tid);
					}
				}
			}
		}
		break;
		
		case GM_MSG_TURRET_OUT_OF_CONTROL:
		{
			ASSERT(msg.content_length == sizeof(int));
			gnpc_imp * pImp = (gnpc_imp*)_imp;
			gnpc * pParent = (gnpc*)(pImp->_parent);
			int faction = *(int*)msg.content;
			if(pParent->pos.squared_distance(msg.pos) < 150.f*150.f && (faction & pImp->GetFaction()))
			{
				if(_ai_core)
				{
					_ai_core->ClearTurretMaster();
				}
			}
		
		}
		break;

		case GM_MSG_TRY_CLEAR_AGGRO:
		{
			//npc���ܿ���ʱ��Ҫ������,msg.param����������
			gnpc * pNPC = (gnpc*)_imp->_parent;
			if(pNPC->anti_invisible_degree >= msg.param) break;
			if(_ai_core) _ai_core->AggroClear(msg.source);
		}
		break;
		
	}
	return 0;
}

void 
gnpc_controller::AddAggro(const XID & who, int rage)
{
	if(_ai_core) _ai_core->AggroGen(who,rage);
}

void
gnpc_controller::RemoveAggro(const XID& src, const XID& dest, float ratio)
{
    if (_ai_core) _ai_core->AggroRemove(src, dest, ratio);
}

void 
gnpc_controller::BeTaunted(const XID & who, int rage)
{
	if(_ai_core) _ai_core->BeTaunted(who,rage);
}

int 
gnpc_controller::MoveBetweenSlice(gobject *obj, slice * src, slice * dest)
{
	ASSERT(obj == _imp->_parent);
	return src->MoveNPC((gnpc *)obj,dest);
}

gnpc_controller::~gnpc_controller()
{
	if(_ai_core) ReleaseAI();
}

void 
gnpc_controller::Release(bool free_parent)
{
	gnpc *pNPC = GetParent();
	gactive_imp * pImp = (gactive_imp*)_imp;
	pImp->ResetSession();

	world *pPlane = _imp->_plane;
	if(pNPC->pPiece) pPlane->RemoveNPC(pNPC);
	ReleaseAI();
	dispatcher * runner = _imp->_runner;
	delete _imp;
	delete runner;
	delete this;
	pNPC->imp = NULL;
	if(free_parent) pPlane->FreeNPC(pNPC);
}


gnpc_imp::gnpc_imp():_native_notify(0),_npc_state(0),_dmg_list(20),_spawner(NULL),_money_scale(1.0f),_drop_rate(1.0f),_fast_regen(1),_regen_spawn(0)
{
	_faction = 0;
	_enemy_faction = 0;
	_native_server = -1;
	_birth_place.y = -1e8;
	_leader_id = XID(-1,0);
	_target_id = XID(-1,-1);
	_owner_id = XID(-1,-1);
	_first_attacker = XID(-1,-1);
	_first_attacked_tick = 0;
	_total_damage = 0;
	_max_damage = 0;
	_last_damage = 0;
	_aggro_adj_attacker = XID(-1,-1);
	_aggro_on_damage_adj = 0;
	_corpse_delay = 20;
	_knocked_back = 0;
	_petegg_id = 0;
	_drop_no_protected = 0;
	_drop_no_profit_limit = 0;
	_drop_mine_prob = 0.f;
	_drop_mine_group = 0;
	_no_accept_player_buff = 0;
	_fixed_direction = 0;
	_collision_actived = false;
	_record_dps_rank = false;
	_chief_gainer_id = XID(-1,0);

	_after_death_summon_monster_id = 0;
	_after_death_summon_monster_radius = 0;
	_after_death_summon_monster_num = 0;

	_max_received_damage_per_hit = 0;

	memset(_local_var,0,sizeof(_local_var));
}

gnpc_imp::~gnpc_imp()
{
	if(_native_notify) delete _native_notify;
}

void 
gnpc_imp::SetInhabitMode(char inhabit_mode)
{
	_inhabit_mode = inhabit_mode;
	gnpc* pNPC = (gnpc*)_parent;
	pNPC->object_state &= ~(gactive_object::STATE_NPC_FLY | gactive_object::STATE_NPC_SWIM);
	switch(_inhabit_mode)
	{
		case NPC_MOVE_ENV_ON_AIR:
			pNPC->object_state |= gactive_object::STATE_NPC_FLY;
			_layer_ctrl.TakeOff();
			break;
		case NPC_MOVE_ENV_IN_WATER:
			pNPC->object_state |= gactive_object::STATE_NPC_SWIM;
			_layer_ctrl.Swiming();
			break;
		case NPC_MOVE_ENV_ON_GROUND:
		default:
			_layer_ctrl.Ground();
			break;
	}
}

void 
gnpc_imp::SetFastRegen(bool b)
{
        _fast_regen = b?1:0;
	gnpc_controller * pCmd = (gnpc_controller*)_commander;
	pCmd->SetFastRegen(b);
}

void 
gnpc_imp::ActiveCollision(bool active)
{       
	if(active)
	{
		if(!_collision_actived)
		{
			_collision_actived = true;
			if(_parent->collision_id >0) _plane->GetTraceMan()->EnableElement(_parent->collision_id, true, &_plane->w_collision_flags);
		}
	}
	else
	{
		if(_collision_actived)
		{
			_collision_actived = false;
			if(_parent->collision_id >0) _plane->GetTraceMan()->EnableElement(_parent->collision_id, false, &_plane->w_collision_flags);
		}
	}
}

void 
gnpc_imp::LifeExhaust()
{
	ActiveCollision(false);
	if(_spawner)
	{
		//��ʱ��� �����б�
		_subscibe_timer = 0;
		_subscibe_list.clear();

		if(_spawner->Reclaim(_plane, (gnpc*)this->_parent,this,_regen_spawn))
		{
			//����false�ͱ��� Reclaim�ڲ��Ѿ��ͷ��˱�����
			_npc_state = NPC_STATE_SPAWNING;
		}
	}
	else
	{
		_commander->Release();
	}
}

int 
gnpc_imp::ZombieMessageHandler(world * pPlane ,const MSG & msg)
{	
	__PRINTF("recv message in zombie state msg %d\n",msg.message);
	//��ʱ�������κ���Ϣ��������״̬
	switch(msg.message)
	{
	case GM_MSG_OBJ_ZOMBIE_END:
		//�����ʬ�����ʱ��ŷ���ʬ����ʧ��Ϣ
		if(_corpse_delay)
		{
			_runner->disappear();
		}
		LifeExhaust();
		break;
	case GM_MSG_HEARTBEAT:
		DoHeartbeat(msg.param);
		break;
	case GM_MSG_QUERY_OBJ_INFO00:
		MH_query_info00(msg);
		break;

	case GM_MSG_ENCHANT_ZOMBIE:
		{
			__PRINTF("recv zombie enchant\n");
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			HandleEnchantMsg(pPlane,msg,&ech_msg);
			return 0;
		}

	case GM_MSG_SUBSCIBE_TARGET:
	case GM_MSG_UNSUBSCIBE_TARGET:
		return MessageHandler(pPlane,msg);

	default:
		break;
	}
	return 0;
}

int 
gnpc_imp::DispatchMessage(world * pPlane ,const MSG & msg)
{
	switch(_npc_state)
	{
		case NPC_STATE_NORMAL:
		case NPC_STATE_WAITING_SWITCH:
		if(_parent->IsZombie())
			return ZombieMessageHandler(pPlane ,msg); 
		else
			return MessageHandler(pPlane,msg);
		case NPC_STATE_SPAWNING:
		return 0;

		case NPC_STATE_MOVEOUT:
		return 	((gnpc_controller*)_commander)->MH_MOVEOUT(pPlane, msg);
		default:
		ASSERT(false);
		return 0;
	}
}

int 
gnpc_imp::MessageHandler(world * pPlane ,const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_WATCHING_YOU:
		case GM_MSG_GEN_AGGRO:
		case GM_MSG_TRANSFER_AGGRO:
		case GM_MSG_AGGRO_ALARM:
		case GM_MSG_AGGRO_WAKEUP:
		case GM_MSG_AGGRO_TEST:
		case GM_MSG_NPC_CRY_FOR_HELP:
		case GM_MSG_PLAYER_KILLED_BY_NPC:
		case GM_MSG_BECOME_TURRET_MASTER:
		case GM_MSG_TURRET_OUT_OF_CONTROL:
		case GM_MSG_TRY_CLEAR_AGGRO:
		//������й��ڳ�޵���Ϣת����controller��ȥ
		return _commander->MessageHandler(pPlane,msg);

		return 0;

		case GM_MSG_HATE_YOU:
		return 0;

		case GM_MSG_ATTACK:
			{
				ASSERT(msg.content_length >= sizeof(attack_msg));
				//�������ڶ�Ҫ���и��Ʋ�������һ���ĺķѴ���
				attack_msg ack_msg = *(attack_msg*)msg.content;
				if(_owner_id.IsPlayerClass() && ack_msg.ainfo.attacker != _owner_id) return 0;
				if(((gnpc*)_parent)->mafia_id && ((gnpc*)_parent)->mafia_id == ack_msg.ainfo.mafia_id) return 0;
									
				if(!msg.source.IsPlayerClass())
				{
					if(!ack_msg.force_attack && !(GetFaction() & ack_msg.target_faction))
					{
						return 0;
					}
				}
				_filters.EF_TransRecvAttack(msg.source, ack_msg);
				HandleAttackMsg(pPlane,msg,&ack_msg);
				return 0;
			}
		case GM_MSG_ENCHANT:
			{
				__PRINTF("recv enchant\n");
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				//�������ڶ�Ҫ���и��Ʋ�������һ���ĺķѴ���
				if(_owner_id.IsPlayerClass() && ech_msg.ainfo.attacker != _owner_id) return 0;
				if(!ech_msg.helpful && ((gnpc*)_parent)->mafia_id && ((gnpc*)_parent)->mafia_id == ech_msg.ainfo.mafia_id) return 0;

				if(!msg.source.IsPlayerClass())
				{
					if(!ech_msg.helpful && !ech_msg.force_attack && !(GetFaction() & ech_msg.target_faction))
					{
						return 0;
					}
				}
				else
				{
					if(_no_accept_player_buff && ech_msg.helpful) return 0;
				}
				_filters.EF_TransRecvEnchant(msg.source, ech_msg);
				HandleEnchantMsg(pPlane,msg,&ech_msg);
				return 0;
			}
		case GM_MSG_SPAWN_DISAPPEAR:
			{
				ActiveCollision(false);
				gnpc * pNPC = (gnpc*)_parent;
				OnNpcLeaveWorld();
				_runner->disappear();
				pNPC->b_zombie = true;
				_commander->Release();
			}
			return 0;

		case GM_MSG_NPC_TRANSFORM2:
		{
			if(msg.param)
			{
				gnpc_imp * __this = TransformMob(msg.param);
				if(__this == NULL)
				{
					GLog::log(GLOG_ERR,"NPC��ת��2��ʱ��������");
					return 0;
				}

				//���͸���״̬����
				__this->_runner->disappear();
				__this->_runner->enter_world();
			}
		}
		return 0;

		case GM_MSG_TRANSFER_FILTER_DATA:
		{
			if(_owner_id.IsPlayerClass() && msg.source != _owner_id) return 0;
			if(!_parent->IsZombie())
			{
				raw_wrapper ar(msg.content, msg.content_length);
				_filters.AddSpecFilters(ar, msg.param, this);	
			}
		}
		return 0;

	default:
		return gactive_imp::MessageHandler(pPlane,msg);
	}
	return 0;
}



bool 
gnpc_controller::CreateAI(const aggro_param & aggp, const ai_param & aip)
{
	if(_ai_core) {
		ASSERT(false && "ԭ����AI����û��ɾ��");
		return false;
	}
	gnpc_ai *pTmp = new gnpc_ai();
	pTmp->Init(this,aggp,aip);

	_ai_core = pTmp;
	return true;
}

void 
gnpc_controller::Init(gobject_imp * imp) 
{
	_imp = imp;
	if(_ai_core)
	{
		_ai_core->ReInit(this);
	}
}

void
gnpc_controller::ReleaseAI()
{
	if(_ai_core) delete _ai_core;
	_ai_core = NULL;
}

void 
gnpc_controller::SwitchSvr(int dest,const A3DVECTOR &oldpos,const A3DVECTOR & newpos,const instance_key * switch_key)
{
	gnpc * pNPC = GetParent();
	if(pNPC->ID.type != GM_TYPE_NPC)
	{
		ASSERT(false);
		//������NPC����������������ʲô�ģ��������NPC
		//����NPC�Ŀ�������һ�����ǵ�����ɵ�
		return ;
	}
	gnpc_imp * pImp = (gnpc_imp *)_imp;
	if(pImp->_npc_state != gnpc_imp::NPC_STATE_NORMAL) return ;
	//����NPC���л�״̬
	pImp->_npc_state = gnpc_imp::NPC_STATE_WAITING_SWITCH;
	pImp->_switch_pos = newpos;
	pImp->_switch_dest = dest;

	class npc_switch_task : public ONET::Thread::Runnable,public abase::ASmallObject
	{
		gnpc * _npc;
	public:
		npc_switch_task(gnpc * npc):_npc(npc)
		{
		}
	public:
		virtual void Run()
		{
			_npc->Lock();
			if(_npc->imp)
			{
				gnpc_imp * pImp = (gnpc_imp*)_npc->imp;
				if(pImp->_npc_state == gnpc_imp::NPC_STATE_WAITING_SWITCH)
				{
					((gnpc_controller*)(pImp->_commander))->DoSwitch();
				}
			}
			_npc->Unlock();
			delete this;
		}
	};
	ONET::Thread::Pool::AddTask(new npc_switch_task(pNPC));
}
	
void
gnpc_controller::DoSwitch()
{
	gnpc * pNPC = (gnpc*)(_imp->_parent);
	gnpc_imp * pImp = (gnpc_imp*)_imp;

	//���� ��������NPC����
	//�������ⲿ��Ϣ�����ǽ��˲�����ȡ����
	world * pPlane = _imp->_plane;
	raw_wrapper wrapper;
	wrapper.SetLimit(raw_wrapper::SAVE_ONLY);

	pNPC->Export(wrapper);
	WrapObject(wrapper,this,_imp,_imp->_runner);
	MSG msg;
	ASSERT(pImp->_switch_dest >= 0);
	BuildMessage(msg,GM_MSG_SWITCH_NPC,XID(GM_TYPE_SERVER,pImp->_switch_dest),pNPC->ID,
			pImp->_switch_pos,0,
			wrapper.data(),wrapper.size());
	pPlane->SendRemoteMessage(pImp->_switch_dest,msg);

	//����Լ������񣬸����б���������,
	pImp->ClearDamageList();
	pImp->ResetSession();		//�������endsession
	_ai_core->GetAggroCtrl()->Clear();
	_ai_core->GetAICtrl()->Clear();

	//���������Ƴ��Լ���������ֹͣ�����˹㲥��Ϣ
	slice *pPiece = pNPC->pPiece;
//	ASSERT(false && "'����û��ʵ�����ƣ�Ӧ����g_plans������RemoveNPC��������ʱ������Ӧ�������NPC");
	pPiece->Lock();
	pPiece->RemoveNPC(pNPC);
	pPiece->Unlock();

	//Ȼ������Լ���״̬�趨�Լ���״̬���߽��в���
	//Ŀǰ��ִ��NPC�߼�
	if(pNPC->IsNative())
	{
		//���ض����ת��ͨ�����ñ�־���
		//Ҫ���Ĳ��������ı�־����ʼ�ɿ�����������Ϣ��������ֻ���ܸ�����Ϣ������ת��������Ϣ 
		// Ӧ�øĳ���imp��������Щ
		((gnpc_imp*)_imp)->_npc_state = gnpc_imp::NPC_STATE_MOVEOUT;
		_extern_svr = pImp->_switch_dest;
	}
	else
	{
		//�ڱ����������NPC���ⲿ����
		extern_object_manager::object_appear app;
		app.body_size = pNPC->body_size;
		app.race = pNPC->base_info.race;
		app.faction = pNPC->base_info.faction;
		app.level= pNPC->base_info.level;
		app.hp= pNPC->base_info.hp;
		app.state = pNPC->IsZombie();
		app.where = pImp->_switch_dest;
		pPlane->GetExtObjMan().Refresh(pNPC->ID.id,pImp->_switch_pos,app);

		//��ͼ���ⲿ�б����Ƴ�����
		pPlane->EraseExternNPC(pNPC->ID.id);
		Release();

		//�Ǳ��ض��󣬼�ת�ƶ���ֱ��ɾ�������ҷ�����Ϣ�����ض���ĳ�ʼ������
		//�����жϵĻ��������Ǳ��ض����Ƿ����ת�ƶ����أ���һ���ɣ�����Ҳ��˵,�ȿ��Ǻ����Ժ��پ���
		//2006 ����NPC�Ѿ��������������ת�Ʋ����ˣ����ǽ�������ش���ȥ�� $$$$$$$$$$
	}

}
/*
void 
gnpc_imp::SwitchSvr(int dest,const A3DVECTOR & oldpos, const A3DVECTOR & newpos)
{
}*/

class gnpc_notify_link_native : public gnpc_notify
{
	int _timeout;
	
public:
	gnpc_notify_link_native():_timeout(10)
	{}

	virtual void OnDeath(gnpc_imp * imp)
	{
		//֪ͨԭ��
		MSG msg;
		const XID & id = imp->_parent->ID;
		BuildMessage(msg,GM_MSG_EXT_NPC_DEAD,id, id, imp->_parent->pos);
		imp->_plane->SendRemoteMessage(imp->_native_server,msg);
		_timeout = 10;
	}
	
	virtual void OnMove(gnpc_imp * imp)
	{
		//֪ͨԭ�� �ƶ�
		MSG msg;
		const XID & id = imp->_parent->ID;
		BuildMessage(msg,GM_MSG_EXT_NPC_HEARTBEAT,id,id,imp->_parent->pos,imp->_basic.hp);
		imp->_plane->SendRemoteMessage(imp->_native_server,msg);
		_timeout = 10;
	}

	virtual void ForwardFirstAggro(gnpc_imp * imp, const XID & id, int rage)
	{
		//֪ͨԭ�� ���г��ת��
		MSG msg;
		const XID & self = imp->_parent->ID;
		BuildMessage(msg,GM_MSG_EXT_AGGRO_FORWARD,self,self,imp->_parent->pos,rage, &id,sizeof(id));
		imp->_plane->SendRemoteMessage(imp->_native_server,msg);
	}

	virtual void OnHeartbeat(gnpc_imp * imp,unsigned int tick)
	{	
		gnpc * pNPC = (gnpc*)imp->_parent;
		pNPC->idle_timer = 20;	//�ⲿnpc��Զ������idle״̬����Ϊ����ܹ����룬�����Զ��Ӹ÷�������ʧ��
		if((--_timeout) <= 0)
		{
			//֪ͨԭ��
			MSG msg;
			const XID & id = imp->_parent->ID;
			BuildMessage(msg,GM_MSG_EXT_NPC_HEARTBEAT,id,id,pNPC->pos,imp->_basic.hp);
			imp->_plane->SendRemoteMessage(imp->_native_server,msg);
			_timeout = 10;
		}
	}
	
	virtual void OnDisappear(gnpc_imp * imp)
	{
		//֪ͨԭ��
		//�����ʱ��֪ͨԭ���ˣ��ɱ��غ�ԭ����ͬʱ��ʱ���ɡ�
	}
};              

class gnpc_notify_native_npc : public gnpc_notify
{
	int _timeout;
public:
	virtual int MoveOutHeartbeat(gnpc_imp * impl, int tick)
	{
		_timeout -= tick;
		if(_timeout <= 0)
		{
			//�Լ�������ʧ
			return -1;
		}
		return 0;
	}
	
	virtual void OnUpdate(gnpc_imp * impl) 
	{
		//ˢ��һ��
		_timeout = 60;
	} 
};

void 
gnpc_imp::Init(world * pPlane,gobject*parent)
{
	gactive_imp::Init(pPlane,parent);
	((gnpc*)parent)->idle_timer = NPC_IDLE_TIMER;
	_npc_state = NPC_STATE_NORMAL;
	
	//��һ�γ�ʼ���Ÿ��³�����
	if(_birth_place.y < -1e3) 
	{
		_birth_place = _parent->pos;
		_birth_dir = _parent->dir;
	}
	//NPC����Ĭ��Ϊ�ܸ�
	_basic.ap = 199999;

	ASSERT(_native_notify == NULL);
	_native_server  = ID2WIDX(_parent->ID.id);
	if(_native_server != world_manager::GetWorldIndex()) 
	{
		//��ԭ��
		_native_notify = new gnpc_notify_link_native;
	}
	else
	{
		_native_notify = new gnpc_notify_native_npc;
	}
	_chief_gainer_id = XID(-1,0);
}

void 
gnpc_imp::OnDeath(const XID & attacker,bool is_invader,char attacker_mode, int)
{
	if(_parent->IsZombie())
	{
		//�Ѿ���zombie��
		return ;
	}
	
	// Novo sistema de summon de monstros após a morte
	if(_after_death_summon_monster_id > 0)
	{
		SummonMonster(_after_death_summon_monster_id, _after_death_summon_monster_num>10?10: _after_death_summon_monster_num, _parent->ID, _after_death_summon_monster_radius, 0, 0, 0);
	}

	LuaManager::GetInstance()->EventOnMonsterKilled(_parent->ID.id, attacker.id, ((gnpc*)_parent)->tid);	
	
	// Pontos diarios	
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(attacker.id,windex1);
	if(gPlayer)
	{	
		gplayer_imp * pImp = (gplayer_imp*)gPlayer->imp;
		if(pImp)
		{
			if(pImp->GetTeamMemberNum() > 0 && pImp->IsInTeam())
			{
				for(int i=0; i<pImp->GetTeamMemberNum(); i++)
				{
					int windexteam;
					gplayer * gPlayer_team = world_manager::GetInstance()->FindPlayer(pImp->GetTeamMember(i).id.id,windexteam);

					if(gPlayer_team)
					{
						gplayer_imp * pImpTeam = (gplayer_imp*)gPlayer_team->imp;

						if(pImpTeam)
						pImpTeam->SystemRealmExpDaily(((gnpc*)_parent)->tid);	
					}	
				}
			}			
			pImp->SystemRealmExpDaily(((gnpc*)_parent)->tid);
		}	
	}

	unsigned int boss_id = ((gnpc*)_parent)->tid;
	
	if(boss_id == 42749)
	{
		// Ativa os baús
		_plane->TriggerSpawn(5021);
	}

	switch(_after_death)
	{
	case 0:
	case 2:
		//����
		break;
	case 1:
		//�Ա�
		{
			int next;
			if(NPCStartSkill(SUICIDE_ATTACK_SKILL_ID,1,attacker,next) >= 0)
			{
				NPCEndSkill(SUICIDE_ATTACK_SKILL_ID,1,attacker);
			}
		}
		break;
	}
	//�����Ĳ������ǣ�����zombie״̬��
	_parent->b_zombie = true;
	if(_corpse_delay)
	{
		((gnpc*)_parent)->object_state |= gactive_object::STATE_STATE_CORPSE;
	}
	else
	{
		((gnpc*)_parent)->object_state &= ~gactive_object::STATE_STATE_CORPSE;
	}

	world_manager::GetInstance()->OnMobDeath(_plane,GetFaction(), ((gnpc*)_parent)->tid);

	int team_id = 0;
	int team_seq = -1;
	XID owner(-1,-1);
	XID task_owner(-1,-1);
	int owner_level = 99;
	int max_wallow = 0;
	int min_profit_level = PROFIT_LEVEL_NORMAL;

	_team_visible_state_flag = false;
	_visible_team_state.clear();
	_visible_team_state_param.clear();

	//�ַ�����ֵ
	DispatchExp(owner,team_id,team_seq,owner_level,task_owner,max_wallow,min_profit_level);

	//��ʼ���ɽ�Ǯ ֻ����һ��߶���ɱ���Ĺ���Ż�����Ǯ����Ʒ
	bool is_drop = ((owner.type != GM_TYPE_NPC && owner.IsActive()) || team_id > 0);

	//�������ӵ����ݣ���ô���Ϊ������� ���ԭ�е�owner
	if(team_id>0) 
	{
		owner = XID(-1,0);
		_chief_gainer_id = XID(GM_TYPE_PLAYER,team_id);
	}
	else
		_chief_gainer_id = owner;

	//ִ�в��Ե�OnDeath
	((gnpc_controller*)_commander) -> OnDeath(attacker);
	//�����ǰSession ע�������commander��OnDeath֮����ã�����������ڲ������Ŷӵ�����������µ�session������Ĳ���
	ClearSession();

	_native_notify->OnDeath(this);
	
	//����������Ϣ
	_runner->on_death(attacker,_corpse_delay);

	//Ŀǰtask_owner��owner�϶���һ��
	//���ͱ�ɱ������Ϣ���ض�Player
	if(task_owner.type == GM_TYPE_PLAYER) 
	{
		if(_owner_id.IsPlayerClass() && attacker == _owner_id)
		{
			int tick = g_timer.get_tick() - _first_attacked_tick;
			if(tick <= 0) tick = 1;
			msg_dps_dph_t data = {_basic.level, (int)(20.f*_total_damage/tick), _max_damage, _record_dps_rank};
			SendTo<0>(GM_MSG_NPC_BE_KILLED_BY_OWNER, _owner_id, GetNPCID(), &data, sizeof(data));	
		}
		else
		{
			int level = _basic.level;
			SendTo<0>(GM_MSG_NPC_BE_KILLED,task_owner,GetNPCID(),&level,sizeof(level));
		}
	}
	ClearDamageList();

	//Ҫ���ǳ���ɱ���Ĺ���Ҫ�������������

	if(is_drop) 
	{
		if(_drop_no_protected)
		{
			owner = XID(-1,0);
			team_id = 0;
		}

		if(_drop_no_profit_limit || min_profit_level != PROFIT_LEVEL_NONE)
		{
			DropItem(owner, owner_level, team_id, team_seq, max_wallow);
		}
		
		//����ĵ��䣬�ں�����߼���Ƿ�����
		DropMine();
	}

	{
		//������ʧ����Ϣ����ʱ���ͣ�
		//��Ϊ������ʧ�Ļ������������������� ���кܶ����Ĵ���
		MSG msg;
		BuildMessage(msg,GM_MSG_OBJ_ZOMBIE_END,_parent->ID,_parent->ID,_parent->pos);
		int delay = _corpse_delay*20;
		if(delay > 200 * 20 ) delay = 200 * 20;
		if(delay)
		{
			_plane->PostLazyMessage(msg,delay);
		}
		else
			_plane->PostLazyMessage(msg);
	}
	OnNpcLeaveWorld();
	
	
	
	
}

namespace
{
	struct TempDmgNode 
	{
		//������˳��Ҫ�������
		//�������msg_grpexp_tһ��
		msg_grpexp_t content;
		TempDmgNode(const XID & id, int dmg)
		{
			content.who = id;
			content.damage = dmg;
		}
	};
	struct  TempDmgEntry
	{
		int damage;
		int profit_level;//������һ���ӵ���С����ʱ��
		abase::vector<TempDmgNode, abase::fast_alloc<> > list;
		TempDmgEntry():damage(0),profit_level(PROFIT_LEVEL_NORMAL)
		{}
	};
};

void
gnpc_imp::DispatchTaskToDmgList(int taskid, int count, int dis)
{
	DAMAGE_MAP::iterator iter = _dmg_list.begin();
	DAMAGE_MAP::iterator iend = _dmg_list.end();

	float sq = (float)dis*dis;
	world::object_info info;
	MSG msg;
	BuildMessage(msg, GM_MSG_DELIVER_TASK, XID(GM_TYPE_PLAYER,0),_parent->ID,_parent->pos,taskid);
	for(;iter != iend && count > 0; ++iter)
	{
		const XID& target = iter->first;
		if(target.type == GM_TYPE_PLAYER && _plane->QueryObject(target,info))
		{
			A3DVECTOR offset = info.pos;
			offset -= _parent->pos;  
			if(sq>offset.squared_magnitude())
			{
				msg.target = target;
				_plane->PostLazyMessage(msg);				
				--count;
			}
		}
	}
}

void 
gnpc_imp::DispatchExp(XID & owner, int &team_id, int &team_seq, int &level, XID & task_owner, int & max_wallow_level, int & min_profit_level)
{
	int exp = _basic.exp;
	int sp = _basic.skill_point;
	if(!_dmg_list.size()) return;
	int total_damage = 0;

	player_template::AdjustGlobalExpSp(exp,sp);

	typedef std::unordered_map<XID,TempDmgEntry,XID_HashFunc >  TempDmgMap;
	TempDmgMap dlist(_dmg_list.size());
	
	DAMAGE_MAP::iterator it = _dmg_list.begin();
	int sig_max_damage = -1;
	int team_max_damage = -1;
	int max_team_id = 0;
	int max_team_seq = -1;
	int max_wallow = 0;
	for(;it != _dmg_list.end(); ++it)
	{
		int damage = it->second.damage;
		int equivalent_damage = damage;
		if(it->first == _first_attacker)
		{
			equivalent_damage += (_cur_prop.max_hp >> 2);
		}
		if(max_wallow  < it->second.wallow_level)
		{
			max_wallow = it->second.wallow_level;
		}
		if(min_profit_level > it->second.profit_level)
		{
			min_profit_level = it->second.profit_level;
		}

		total_damage += damage;
		if(sig_max_damage < equivalent_damage)
		{
			sig_max_damage = equivalent_damage;
			owner = it->first;
		}
		if(it->second.team_id > 0)
		{
			TempDmgEntry &ent = dlist[XID(-it->second.team_id,it->second.team_seq)];
			ent.damage += damage;
			if(ent.list.empty())
			{
				//����������λ�����澭��ֵ��sp�����ݣ���������һ���͹�ȥ
				ent.list.push_back(TempDmgNode(XID(0,0),0));
				ent.list.push_back(TempDmgNode(XID(0,0),0));
			}

			//����������0��Ԫ�ر�����߼������߼����Ӧ�����
			TempDmgNode & node = ent.list[0];
			if(node.content.damage < it->second.level) 
			{
				node.content.damage = it->second.level;
				node.content.who = it->first;
			}

			//���¶�Ա����С����ʱ��
			if(it->second.profit_level < ent.profit_level)
			{
				ent.profit_level = it->second.profit_level;
			}

			ent.list.push_back(TempDmgNode(it->first,damage));

			if(team_max_damage < ent.damage) 
			{
				team_max_damage = ent.damage;
				max_team_id = it->second.team_id;
				max_team_seq = it->second.team_seq;
			}
		}
		else
		{
			TempDmgEntry &ent = dlist[it->first];
			ent.damage += damage;
			ent.profit_level = it->second.profit_level;
			if(team_max_damage < equivalent_damage) 
			{
				team_max_damage = equivalent_damage;
				max_team_id = 0;
				max_team_seq = -1;
			}
		}
	}

	if(total_damage < _cur_prop.max_hp) total_damage = _cur_prop.max_hp;
	float factor = 1.f / total_damage;

	TempDmgMap::iterator it2 = dlist.begin();
	msg_exp_t exp_data;
	exp_data.level = _basic.level;
	int group_level = ((unsigned) exp_data.level) << 24 ;
	int world_tag = world_manager::GetWorldTag();
	int plane_index = _plane->w_plane_index;

	for(;it2 != dlist.end(); ++it2)
	{
		int damage = it2->second.damage;
		exp_data.exp = (int)(exp * factor * damage + 0.5f);
		exp_data.sp = (int)(sp * factor * damage + 0.5f);
		if(it2->first.type > 0)//�����
		{
			//�������漶�����õ���Ӧ���������
			float rate = player_template::GetProfitRate(it2->second.profit_level);
			exp_data.exp = (int)(exp_data.exp * rate);
			exp_data.sp = (int)(exp_data.sp * rate);

			if(exp_data.exp > 0 && it2->first.type != GM_TYPE_NPC)
			{
				//��NPC
				SendTo<0>(GM_MSG_EXPERIENCE,it2->first,0,&exp_data,sizeof(exp_data));
			}
		}
		else
		{
			TempDmgEntry &ent = it2->second; 
			int id = -it2->first.type;
			if(ent.list.size())
			{
				if(exp_data.exp > 0 )//���
				{
					//�������漶�����õ���Ӧ���������
					float rate = player_template::GetProfitRate(it2->second.profit_level);
					exp_data.exp = (int)(exp_data.exp * rate);
					exp_data.sp = (int)(exp_data.sp * rate);

					ent.list[0].content.who.type = exp_data.exp;
					ent.list[0].content.who.id = (exp_data.sp & 0xFFFFFF) | group_level;
					ent.list[0].content.damage= it2->first.id;	//���ֵʵ������seq

					if(id == max_team_id) 
					{
						//��������������˺��Ķ��飬�����������������ݵķ���
						//��Ҫ�����������У�ɱ�����ĸ�NPC
						//�� NPC�ļ���
						//������Ҫ������ռ��һλ
						ent.list[1].content.who.type = GetNPCID() ;
						float r = abase::RandUniform();
						ent.list[1].content.who.id = *(int*)&r;
					}

					ent.list[1].content.damage = world_tag | (plane_index << 16);
					XID team_leader(GM_TYPE_PLAYER,id);
					SendTo<0>(GM_MSG_GROUP_EXPERIENCE,team_leader,damage,
						ent.list.begin(), ent.list.size() * sizeof(TempDmgNode));
				}
			}
		}
		
		//������Ҷ�ɱ������Ĺ��׶�
		if(it2->first.type > 0)
		{
			//����ӣ���NPC
			if(it2->first.type != GM_TYPE_NPC)
			{
				msg_contribution_t data;
				data.npc_id = GetNPCID();
				data.is_owner = (owner == it2->first && max_team_id == 0);
				data.team_contribution = factor * damage;
				data.team_member_count = 1;
				data.personal_contribution = factor * damage;
				SendTo<0>(GM_MSG_CONTRIBUTION_TO_KILL_NPC,it2->first,world_tag,&data,sizeof(data));
			}			
		}
		else
		{
			TempDmgEntry &ent = it2->second; 
			int id = -it2->first.type;
			if(ent.list.size())
			{
				char buf[160];
				memset(buf,0,sizeof(buf));
				msg_group_contribution_t& data = *(msg_group_contribution_t*)buf;
				data.npc_id = GetNPCID();
				data.is_owner = (id == max_team_id);
				data.count = ent.list.size()-2;
				if(data.count > 12) data.count = 12;
				for(int i=0; i<data.count; i++)
				{
					data.list[i].xid = ent.list[i+2].content.who;
					data.list[i].contribution = factor * ent.list[i+2].content.damage;
				}
				XID team_leader(GM_TYPE_PLAYER,id);
				SendTo<0>(GM_MSG_GROUP_CONTRIBUTION_TO_KILL_NPC,team_leader,world_tag,
						&data,sizeof(msg_group_contribution_t)+data.count*sizeof(msg_group_contribution_t::_list));		
			}
		}
	}

	//�ȼ����յ�����󹥻�������
	level = _dmg_list[owner].level;

	//�������ɱ�ֱ�־
	team_id = max_team_id;
	team_seq = max_team_seq;
	
	//���������������ǹ����������(����������ۼ�)
	task_owner = owner;

	max_wallow_level = max_wallow;
	
	ASSERT(level > 0);

	__PRINTF("��������,min_profit_level = %d.\n", min_profit_level);

}

void 
gnpc_imp::AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry &dmg,float & damage_adjust)
{
	if(IS_HUMANSIDE(msg.source))
	{
		float adjust = 1.0f;
		player_template::GetAttackLevelPunishment(attack->ainfo.level - _basic.level, adjust);

		int pp = (((attack->attacker_layer) & 0x03) << 2) | _layer_ctrl.GetLayer();
		ASSERT((_layer_ctrl.GetLayer() & ~0x03) == 0);
		switch(pp)
		{
			case ((LAYER_GROUND << 2) | LAYER_GROUND):
			case ((LAYER_AIR << 2) | LAYER_AIR):
			case ((LAYER_WATER << 2) | LAYER_WATER):
			case ((LAYER_GROUND << 2) | LAYER_AIR):
				break;


			case ((LAYER_GROUND << 2) | LAYER_WATER):
			case ((LAYER_AIR << 2) | LAYER_GROUND):
			case ((LAYER_AIR << 2) | LAYER_WATER):
			case ((LAYER_WATER << 2) | LAYER_GROUND):
			case ((LAYER_WATER << 2) | LAYER_AIR):
				adjust *= 0.5f;
				break;                          

			case ((LAYER_INVALID<< 2) | LAYER_GROUND):
			case ((LAYER_INVALID<< 2) | LAYER_AIR):
			case ((LAYER_INVALID<< 2) | LAYER_WATER):
			case ((LAYER_INVALID<< 2) | LAYER_INVALID):
			case ((LAYER_GROUND << 2) | LAYER_INVALID):
			case ((LAYER_AIR << 2) | LAYER_INVALID):
			case ((LAYER_WATER << 2) | LAYER_INVALID):
				ASSERT(false);
				break;
			default:
				ASSERT(false);
		}
		damage_adjust *= adjust;

		int world_tag = world_manager::GetWorldTag();

		// Se o personagem tiver no carrier 
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(attack->ainfo.attacker.id,windex1);
		if(gPlayer && gPlayer->imp)
		{
			if (((gactive_object *)gPlayer->imp->_parent)->IsCarrierMode() && world_tag == 178)
			{
				attack->attack_degree = 999999;
			}
		}

	}
	if(attack->skill_id &&  (attack->skill_enhance || attack->skill_enhance2) )
		damage_adjust *= (0.01f * (100 + attack->skill_enhance + attack->skill_enhance2));

	damage_adjust *= (1.f + player_template::GetPenetrationEnhance(attack->penetration));
}

void 
gnpc_imp::OnAttacked(world *pPlane,  const MSG & msg, attack_msg * attack, damage_entry & dmg, bool is_hit)
{
	if(!is_hit)
	{
		//ֻ��δ���в������������� 
		unsigned int speed = attack->speed;
		if(speed > 200) speed = 200;	//�ӳٲ��ܹ���
		AddAggroEntry(msg.source,attack->attacker_faction,attack->ainfo.level,3,speed);
		if(attack->ainfo.attacker != msg.source)
		{
			//����һЩ�����˵ĳ��
			AddAggroEntry(attack->ainfo.attacker,attack->attacker_faction,attack->ainfo.level,1,speed);
		}
	}
}

void  
gnpc_imp::AddHurtEntry(const XID & attacker, int damage, int team_id,int team_seq,int level, int wallow_level, int profit_level)
{
	if(damage > _basic.hp) damage = _basic.hp;
	if(_dmg_list.empty())
	{
		//���Ǽ�¼��һ�˵�����
		_first_attacker = attacker;
		_first_attacked_tick = g_timer.get_tick();
	}
	DAMAGE_MAP::iterator it = _dmg_list.find(attacker);
	if(it != _dmg_list.end())
	{
		hurt_entry & ent = it->second;
		ent.team_id = team_id;
		ent.team_seq = team_seq;
		ent.damage += damage;
		ent.level = level;
		ent.wallow_level = wallow_level;
		ent.profit_level = profit_level;
	}
	else
	{
		if(_dmg_list.size() >= MAX_HURT_ENTRY)
		{	
			//�������ˣ����ټ����µ�������
			return ;
		}
		hurt_entry & ent = _dmg_list[attacker];
		ent.team_id = team_id;
		ent.team_seq= team_seq;
		ent.damage = damage;
		ent.level = level;
		ent.wallow_level = wallow_level;
		ent.profit_level = profit_level;
	}
}

void 
gnpc_imp::OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)
{	
	//���ȷ�����������
	_runner->be_hurt(info.attacker,info, damage,invader);
	//ʹ��info���attacker��������ԭ���ģ�����Ϊ�����ֳ���Ĺ���
	if(info.attacker.IsValid() && info.level > 0)
	{
		int level = info.level;
		if(info.eff_level) level = info.eff_level;
		AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq,level, info.wallow_level, info.profit_level);
	}
	
	if(world_manager::GetInstance()->IsCountryBattleWorld() && !OI_IsPet() && attacker.IsPlayer())
	{
		int s = 0;
		SendTo<0>(GM_MSG_COUNTRYBATTLE_HURT_RESULT, attacker, damage, &s, sizeof(int));
	}
}

void 
gnpc_imp::OnDamage(const XID & attacker,int skill_id,const attacker_info_t & info,int damage,int at_state,char speed,bool orange,unsigned char section)
{
	if(_max_received_damage_per_hit > 0)
	{
		if(damage > _max_received_damage_per_hit)
		damage = _max_received_damage_per_hit;
	}

	_combat_state = true;
	_runner->be_damaged(attacker,skill_id, info, damage,0,at_state,speed,orange,section);

	int level = info.level;
	if(info.eff_level) level = info.eff_level;
	AddHurtEntry(info.attacker,damage,info.team_id,info.team_seq,level, info.wallow_level, info.profit_level);

	unsigned int aspeed= speed;
	if(aspeed > 200) aspeed = 200;
	int rage = damage;
	if(attacker == _aggro_adj_attacker)
		rage = int(rage * (1.f + 0.01f*_aggro_on_damage_adj));
	AddAggroEntry(attacker,0xFFFFFFFF,info.level,rage+2,aspeed);
	if(info.attacker != attacker)
	{
		AddAggroEntry(info.attacker,0xFFFFFFFF,info.level,1,speed);
	}

	_total_damage += damage;
	if(damage > _max_damage) _max_damage = damage;
	_last_damage = damage;
	((gnpc_controller*)_commander)->OnDamage();
	
	if(_basic.hp < damage )
	{
		((gnpc_controller*)_commander)->TryCryForHelp(attacker);
	}

	if(world_manager::GetInstance()->IsCountryBattleWorld() && !OI_IsPet() && attacker.IsPlayer())
	{
		int s = 0;
		SendTo<0>(GM_MSG_COUNTRYBATTLE_HURT_RESULT, attacker, damage, &s, sizeof(int));
	}
}

bool 
gnpc_imp::CheckInvader(world * pPlane, const XID & source)
{
	return false;
}


void
gnpc_controller::OnHeartbeat(unsigned int tick)
{
	if(_ai_core) _ai_core->Heartbeat();
	gnpc_imp * pImp = (gnpc_imp *)_imp;
	slice * pPiece = pImp->_parent->pPiece;
	if(pPiece && pPiece->IsBorder())
	{
		if(pImp->CheckServerNotify<0>())
		{
			extern_object_manager::SendAppearMsg<0>(_imp->_plane,(gnpc*)pImp->_parent,pPiece);
		}
	}

	if(_cache_refresh_state)
	{
		pImp->NotifyTargetChange(_cur_target_cache);
		_cache_refresh_state = false;
	}
}

void 
gnpc_controller::OnDeath(const XID & attacker)
{
	if(_ai_core) _ai_core->OnDeath(attacker);
}

void 
gnpc_controller::OnDamage()
{
	if(_ai_core) _ai_core->OnDamage();
}

void 
gnpc_imp::OnHeartbeat(unsigned int tick)
{
	ActiveCollision(true);
	_filters.EF_Heartbeat(1);

	//������ٻ��ﲻһ����ѭ���ֹ�����Ҫ�������ǣ���������Щ����������һ������״̬����
	//��һ�������������������npc�Ļ�Ѫ��ai�߼�������
	//���ﵽʱ����Ҫ�����Ƿ�idle״̬ ��
	//���idle״̬��ָһ�������idle״̬����Ѫ������û�г�޶�ʱ��idle״̬����Ҫ����һ��
	//auto gen hp/mp
	if(!_parent->IsZombie())
	{
		if(_combat_state || !_fast_regen)
			//ս��״̬������Ѫ
			GenHPandMP(_cur_prop.hp_gen,_cur_prop.mp_gen);
		else
		{
			if(_cur_prop.hp_gen)
			{
				//�ָ��ٶȲ�Ϊ0�� NPC�ڷ�ս��״̬�ſ��ٻ�Ѫ
				GenHPandMP(_cur_prop.max_hp ,_cur_prop.max_mp);
			}
		}
	}

	//����ʱ��ļ��٣������жϽ���idle״̬  
	if(((gnpc*)_parent)->idle_timer > 0)
	{
		((gnpc*)_parent)->idle_timer -= 1;
	}
	_native_notify->OnHeartbeat(this,tick);
	LuaManager::GetInstance()->EventNpcHeartBeat(_parent->ID.id, tick);
}

void 
gnpc_imp::Reborn()
{
	ASSERT(_npc_state == NPC_STATE_SPAWNING);
	_npc_state = NPC_STATE_NORMAL;
	_birth_place = _parent->pos;
	_birth_dir = _parent->dir;
	//��û��������в���״̬ 
	//�������в���״̬Ӧ���������ͻָ�ʱ�������
	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

	_basic.hp = _cur_prop.max_hp;
	_basic.mp = _cur_prop.max_mp;
	_basic.ap = 199999;
	_basic.ap = 0;

	_idle_mode_flag = 0;
	_seal_mode_flag = 0;

	_chief_gainer_id = XID(-1,0);

	_npc_state = NPC_STATE_NORMAL;
	_parent->b_zombie = false;

	npc_template * pTemplate = npc_stubs_manager::Get(GetNPCID());
	if(pTemplate != NULL)
	{
		memcpy(_local_var,pTemplate->local_var,sizeof(_local_var));
	}

	ClearDamageList();
	ClearSession();
	
	_commander->Reborn();

	_filters.AddFilter(new npc_passive_filter(this));
	
	//��������
	gnpc* pNPC = (gnpc*)_parent;
	pNPC->invisible_degree = _invisible_active;
	pNPC->anti_invisible_degree = _anti_invisible_active;
	if(pNPC->invisible_degree)
	{
		pNPC->object_state |= gactive_object::STATE_INVISIBLE;
		__PRINTF("Reborn����npc,��������%d\n",pNPC->invisible_degree);
	}

	if(_fixed_direction)
		pNPC->object_state |= gactive_object::STATE_NPC_FIXDIR;
	
	_runner->enter_world();
}

void 
gnpc_controller::Reborn()
{
	if(_ai_core)
	{
		_ai_core->Reborn();
	}
	_cur_target_cache = XID(-1,-1);
	_cache_refresh_state = false;
}


bool 
gnpc_imp::StepMove(const A3DVECTOR &offset)
{
	_direction = offset;
	_knocked_back = 0;
	if(_seal_mode_flag & SEAL_MODE_ROOT) return false;		//����ģʽ�޷��ƶ�
	bool bRst = gobject_imp::StepMove(offset);
	slice *pPiece = _parent->pPiece;
	if(pPiece && pPiece->IsBorder())
	{
		extern_object_manager::SendRefreshMsg<0>(_plane,_parent,_basic.hp,pPiece);
	}
	_native_notify->OnMove(this);
	return bRst;
}

bool 
gnpc_imp::CanMove()
{
	return !(_seal_mode_flag & SEAL_MODE_ROOT);
}


bool 
gnpc_imp::Save(archive & ar)
{
	gactive_imp::Save(ar);
	ar << _npc_state << _money_scale << _birth_place << _leader_id << _target_id << _owner_id << _damage_delay << _inhabit_type << _inhabit_mode << _after_death << _first_attacker << _first_attacked_tick << _total_damage << _max_damage << _last_damage << _aggro_adj_attacker << _aggro_on_damage_adj << _corpse_delay << _fast_regen << _regen_spawn <<_drop_no_protected << _drop_no_profit_limit << _record_dps_rank << _drop_mine_prob << _drop_mine_group << _chief_gainer_id << _local_var[0] << _local_var[1] << _local_var[2] << _after_death_summon_monster_id << _after_death_summon_monster_radius << _after_death_summon_monster_num << _max_received_damage_per_hit;
	unsigned int size = _dmg_list.size();
	ar << size;
	DAMAGE_MAP::iterator it = _dmg_list.begin();
	for(;it != _dmg_list.end(); ++it)
	{
		const DAMAGE_MAP::value_type & val = *it;
		ar << val.first.type << val.first.id;
		ar.push_back(&(val.second), sizeof(val.second));
	}
	return true;
}

bool 
gnpc_imp::Load(archive & ar)
{
	gactive_imp::Load(ar);
	ar >> _npc_state >> _money_scale >> _birth_place >> _leader_id >> _target_id >> _owner_id >> _damage_delay >>  _inhabit_type >> _inhabit_mode >> _after_death >> _first_attacker >> _first_attacked_tick >> _total_damage >> _max_damage >> _last_damage >> _aggro_adj_attacker >> _aggro_on_damage_adj >> _corpse_delay >> _fast_regen >> _regen_spawn >> _drop_no_protected >> _drop_no_profit_limit >> _record_dps_rank >> _drop_mine_prob >> _drop_mine_group >> _chief_gainer_id >> _local_var[0] >> _local_var[1] >> _local_var[2] >> _after_death_summon_monster_id >> _after_death_summon_monster_radius >> _after_death_summon_monster_num >> _max_received_damage_per_hit;
	unsigned int size;
	ar >> size;
	ASSERT(_dmg_list.size() == 0);
	for(unsigned int i = 0; i < size; i ++)
	{
		XID id;
		ar >> id.type >> id.id;
		hurt_entry ent;
		ar.pop_back(&ent,sizeof(ent));
		_dmg_list[id] = ent;
	}
	return true;
}

void 
gnpc_controller::NPCSessionStart(int task_id, int session_id)
{
	if(_ai_core) _ai_core->SessionStart(task_id, session_id);
}

void 
gnpc_controller::NPCSessionEnd(int task_id,int session_id, int retcode)
{
	if(_ai_core) _ai_core->SessionEnd(task_id, session_id,retcode);
}

void 
gnpc_controller::NPCSessionUpdateChaseInfo(int task_id,const void * buf ,unsigned int size)
{
	if(_ai_core) _ai_core->SessionUpdateChaseInfo(task_id, buf, size);
}


int
gnpc_imp::DoAttack(const XID & target,char force_attack)
{
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack);

	attack.speed = _damage_delay;

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,0,&attack,sizeof(attack));
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg);

	return 0;
}

void 
gnpc_imp::SetIdleMode(bool sleep, bool stun)
{
	gactive_imp::SetIdleMode(sleep,stun);
	((gnpc_controller*)_commander)->SetIdleMode(_idle_mode_flag);
	if(_idle_mode_flag)
	{
		//�����������session
		ClearSession();
	}
}

void 
gnpc_imp::SetSealMode(bool silent,bool root)
{
	gactive_imp::SetSealMode(silent,root);
	if(_seal_mode_flag)
	{
		if(root)
		{
			//������е��ƶ�����
			ClearSpecSession(act_session::SS_MASK_MOVE | act_session::SS_MASK_SITDOWN);
		}

		if(silent)
		{
			//������еĹ�������
			ClearSpecSession(act_session::SS_MASK_ATTACK | act_session::SS_MASK_SITDOWN);

		}
		//Ӧ��ֹͣ��ǰ�����в���
		AddSession(new session_empty());
		StartSession();
	}
	((gnpc_controller*)_commander)->SetSealMode(_seal_mode_flag);
}

void 
gnpc_controller::TryCryForHelp(const XID & attacker)
{
	if(_ai_core)
	{
		_ai_core->TryCryForHelp(attacker);
	}
}

void 
gnpc_controller::CryForHelp(const XID & attacker,int faction_ask_help,float sight_range)
{
	//�������κ���֮����ٶȲ���������
	int ts = g_timer.get_systime(); 
	if(_cry_for_help_timestamp + 4 >= ts)  return;
	_cry_for_help_timestamp = ts;

	gnpc * pNPC = (gnpc*)_imp->_parent;
	MSG msg;
	msg_cry_for_help_t mcht;
	mcht.attacker = attacker;
	mcht.lamb_faction = pNPC->monster_faction;
	mcht.helper_faction = faction_ask_help;

	BuildMessage(msg,GM_MSG_NPC_CRY_FOR_HELP,XID(GM_TYPE_NPC,-1),pNPC->ID,pNPC->pos,0,&mcht,sizeof(mcht));
	_imp->_plane->BroadcastMessage(msg,sight_range*HELP_RANGE_FACTOR,gobject::MSG_MASK_CRY_FOR_HELP);
}

void
gnpc_imp::SetCombatState()
{
	ActiveCombatState(true);
	ClearInvisible();
}

void 
gnpc_imp::AddNPCAggro(const XID & who, int rage)
{
	((gnpc_controller*)_commander)->AddAggro(who,rage);
}

void
gnpc_imp::RemoveNPCAggro(const XID& src, const XID& dest, float ratio)
{
    ((gnpc_controller*)_commander)->RemoveAggro(src, dest, ratio);
}

void 
gnpc_imp::BeTaunted(const XID & who, int rage)
{
	((gnpc_controller*)_commander)->BeTaunted(who,rage);
}

void
gnpc_imp::SendDataToSubscibeList()
{
	ASSERT(_subscibe_list.size()|| _second_subscibe_list.size());
	packet_wrapper h1(64);
	using namespace S2C;
	//_backup_hp[1]
	CMD::Make<CMD::npc_info_00>::From(h1,_parent->ID,_basic.hp,_basic,_cur_prop, GetCurTarget().id);
	if(_subscibe_list.size())
		send_ls_msg(_subscibe_list.begin(), _subscibe_list.end(), h1.data(),h1.size());
	if(_second_subscibe_list.size())
		send_ls_msg(_second_subscibe_list.begin(), _second_subscibe_list.end(), h1.data(),h1.size());
	
}

void
gnpc_imp::SendTeamDataToSubscibeList()
{
	if(!_team_visible_state_flag || _subscibe_list.empty())  return;
	
	#pragma pack(push, 1)
	unsigned short vts_id[32];
	struct 
	{
		int time;
		int num;
	}	vts_time[32];
	#pragma pack(pop)
	
	unsigned int size = _subscibe_list.size();
	unsigned int iSize = _visible_team_state.size();
	packet_wrapper h1(size *sizeof(unsigned short) + 32 + (iSize*sizeof(vts_time[0])));
	using namespace S2C;
	//_backup_hp[1]
	//int x_it[64];
	//if(size && size <= 32)_filters.FilterTimerReport((short*)_visible_team_state.begin(), x_it, iSize);
	
	for (int i = 0; i < iSize && i < 32; i++ )
	{
		vts_id[i] = _visible_team_state[i].id;
		vts_time[i].time = _visible_team_state[i].time;
		vts_time[i].num = i;
	}	
	
	CMD::Make<CMD::object_state_notify>::From(h1,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(),(int*)vts_time);
	send_ls_msg(_subscibe_list.begin(), _subscibe_list.end(), h1.data(),h1.size());
}

void 
gnpc_imp::ForwardFirstAggro(const XID & id,int rage)
{
	if(_spawner)
	{
		//���������ڣ�ֱ��֪ͨ 
		_spawner->ForwardFirstAggro(_plane,id,rage);
	}
	else
	{	
		
		//��ͼ����ת��֮
	}
	return ;
}

int 
gnpc_imp::GetMonsterFaction()
{
	return ((gnpc*)_parent)->monster_faction;
}

int 
gnpc_imp::GetFactionAskHelp()
{
	return ((gnpc_controller*)_commander)->GetFactionAskHelp();
}

void 
gnpc_imp::SetLifeTime(int life)
{
	((gnpc_controller*)_commander)->SetLifeTime(life);
}

void 
gnpc_imp::KnockBack(const XID & target, const A3DVECTOR & source, float distance,int time,int stun_time)
{
	A3DVECTOR offset = _parent->pos;
	offset -= source;
	float sq = offset.squared_magnitude();
	if(sq <= 1e-6)
	{
		//��������²�����
		return ;
	}
	
	offset *= distance/sqrt(sq);

	offset += _parent->pos;
	path_finding::GetKnockBackPos(_plane,_parent->pos,offset,_inhabit_mode);
	offset -= _parent->pos;

	StepMove(offset);
	_direction.x = -_direction.x;
	_direction.y = -_direction.y;
	_direction.z = -_direction.z;
	_knocked_back = 1;

	_runner->stop_move(_parent->pos,0x0f00,1,C2S::MOVE_MODE_KNOCK);
}

void
gnpc_imp::PullOver(const XID & target, const A3DVECTOR & source,float distance, int time)
{
	A3DVECTOR offset = source;
	offset -= _parent->pos;
	float sq = offset.squared_magnitude();
	if(sq <= 1e-6)
	{
		//��������²�����
		return ;
	}
	
	if(distance*distance > sq)
		distance = sqrt(sq);
	offset *= distance/sqrt(sq);

	offset += _parent->pos;
	path_finding::GetKnockBackPos(_plane,_parent->pos,offset,_inhabit_mode);
	offset -= _parent->pos;

	StepMove(offset);
	_direction.x = -_direction.x;
	_direction.y = -_direction.y;
	_direction.z = -_direction.z;

	_runner->stop_move(_parent->pos,0x0f00,1,C2S::MOVE_MODE_PULL);
}

void
gnpc_imp::Teleport2(const A3DVECTOR & pos, int time, char mode)
{
	A3DVECTOR offset = pos;
	path_finding::GetKnockBackPos(_plane,_parent->pos,offset,_inhabit_mode);
	offset -= _parent->pos;
	float distance = offset.squared_magnitude();
	if(distance <= 1e-6)
	{
		//��������²�����
		return ;
	}

	StepMove(offset);

	_runner->stop_move(_parent->pos,0x0f00,1,C2S::MOVE_MODE_BLINK);
}

void 
gnpc_imp::AddAggroEntry(const XID & who , int faction, int level, int rage, unsigned int speed)
{
	//ͨ��������Ϣ�ķ�ʽ����aggro
	msg_aggro_info_t info = { who,rage,0,faction,level};
	MSG newmsg;
	BuildMessage(newmsg,GM_MSG_GEN_AGGRO,_parent->ID,who,_parent->pos,1,&info,sizeof(info));
	if(speed)
		_plane->PostLazyMessage(newmsg,speed+1);
	else 
		MessageHandler(_plane,newmsg);
}

gnpc_imp * 
gnpc_imp::TransformMob(int target_id)
{
	gnpc *pNPC = (gnpc*)_parent;
	//��ʱ���Լ����������Ƴ�
	_plane->RemoveNPC(pNPC);

	gnpc npc = *pNPC;

	npc_spawner::entry_t ent;
	memset(&ent,0,sizeof(ent));
	ent.npc_tid = target_id;
	ent.mobs_count = 1;
	ent.msg_mask_or = 0;
	ent.msg_mask_and = 0xFFFFFFFF;
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC2 = npc_spawner::CreateMobBase(_spawner,_plane,ent,npc.spawn_index,npc.pos,cid,npc.dir,CLS_NPC_AI_POLICY_BASE,0,&npc);
	if(pNPC2 == NULL) 
	{
		//�޷������м����
		_plane->InsertNPC(pNPC);
		return  NULL;
	}

	ASSERT(pNPC2 == &npc);

	gnpc_imp *__this = (gnpc_imp *)npc.imp;

	//�����¶����Ѫֵ
	float hp_factor = _basic.hp / (float)(_cur_prop.max_hp);
	__this->_basic.hp = (int)(__this->_cur_prop.max_hp * hp_factor);
	if(__this->_basic.hp <= 0) __this->_basic.hp = 1;

	//��¼pPlane�������ͷ��Ժ�Ͳ���ʹ��_plane��
	world * pPlane = _plane;

	//�����ǰ��ѡ�ж���
	ClearSubscibeList();

	//�ͷ��Լ� �����ͷ� pNPC����
	_commander->Release(false);

	//���ԭ��NPC������
	pNPC->Clear();

	//��������ע��
	*pNPC = npc;

	//�������������parent��Ϣ����������
	__this->_parent = pNPC;
	__this->_regen_spawn = 1;

	//����NPC���뵽������
	pPlane->InsertNPC(pNPC);

	return __this;
}

void 
gnpc_controller::SetFastRegen(bool b)
{
	if(_ai_core) _ai_core->SetFastRegen(b);
}

int 
gnpc_imp::SummonMonster(int mod_id, int count, const XID& target, int target_distance, int remain_time, char die_with_who, int path_id)
{
	object_interface oi(this);
	object_interface::minor_param prop;
	memset(&prop,0,sizeof(prop));
	
	prop.mob_id = mod_id;
	prop.remain_time = remain_time;
	prop.exp_factor = 1.f;
	prop.sp_factor = 1.f;
	prop.drop_rate = 1.f;
	prop.money_scale = 1.f;
	prop.spec_leader_id = XID(-1,-1);
	prop.parent_is_leader = true;
	prop.use_parent_faction = true;
	prop.die_with_who = die_with_who;
	if(target != _parent->ID)
		prop.target_id = target;
	else
		prop.target_id = XID(-1,-1);
	prop.path_id = path_id;
	for(int i =0; i < count; i ++)
	{
		int id = oi.CreateMinors(prop,target,target_distance);
		if(id != -1 && target != _parent->ID)	//����npc�ɹ�������npc��target�ĳ��
		{
			XID xid;
			MAKE_ID(xid, id);

			msg_aggro_info_t info;
			info.source = target;
			info.aggro = 10000;
			info.aggro_type = 0;
			info.faction = 0xFFFFFFFF;
			info.level = 0;
			
			MSG msg;
			BuildMessage(msg,GM_MSG_GEN_AGGRO,xid,target,_parent->pos,0,&info,sizeof(info));//�����msg posû������
			
			_plane->PostLazyMessage(msg);
		}

	}
	return 0;
}

void 
gnpc_imp::IncAntiInvisibleActive(int val)
{
	_anti_invisible_active += val;
	((gnpc*)_parent)->anti_invisible_degree = _anti_invisible_active;
	__PRINTF("npc�����ȼ�%d\n",_anti_invisible_active);
}

void 
gnpc_imp::DecAntiInvisibleActive(int val)
{
	_anti_invisible_active -= val;
	((gnpc*)_parent)->anti_invisible_degree = _anti_invisible_active;
	__PRINTF("npc�����ȼ�%d\n",_anti_invisible_active);
}

void 
gnpc_imp::SetInvisible(int)
{
	if(_invisible_active <= 0) return;
	gnpc* pNPC = (gnpc*)_parent;
	if(pNPC->invisible_degree > 0) return;
	pNPC->invisible_degree = _invisible_active;
	pNPC->object_state |= gactive_object::STATE_INVISIBLE;	
	_runner->disappear_to_spec(pNPC->invisible_degree);
	_runner->toggle_invisible(pNPC->invisible_degree);
	__PRINTF("npc����%d��������\n",pNPC->invisible_degree);
}

void 
gnpc_imp::ClearInvisible()
{
	if(_invisible_active <= 0) return;
	gnpc* pNPC = (gnpc*)_parent;
	if(pNPC->invisible_degree <= 0) return;
	_runner->appear_to_spec(pNPC->invisible_degree);
	pNPC->invisible_degree = 0;
	pNPC->object_state &= ~gactive_object::STATE_INVISIBLE;
	_runner->toggle_invisible(0);
	__PRINTF("npc����������\n");
}

int 
gnpc_imp::OI_GetPetEggID()  
{
	return _petegg_id;
}

void 
gnpc_imp::OI_TransferPetEgg(const XID & who, int pet_egg)
{
	SendTo<0>(GM_MSG_MOB_BE_TRAINED, who, pet_egg);
}

void 
gnpc_imp::OI_Disappear()
{
	_parent->b_zombie = true;

	//ִ������OnDeath���߼�
	ClearSession(); 
	_team_visible_state_flag = false;
	_visible_team_state.clear();
	_visible_team_state_param.clear();

	//�����Ҫ�ı�־
	_idle_mode_flag = 0;
	_seal_mode_flag = 0;

	//ȥ������ʱӦ��ȥ����filter
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);

	SendTo<0>(GM_MSG_OBJ_ZOMBIE_END,_parent->ID,0);
	OnNpcLeaveWorld();
	_runner->disappear();
}

void
gnpc_imp::DropMine()
{
	// �жϵ������͵������
	if (_drop_mine_prob <= 0.f || abase::RandUniform() > _drop_mine_prob)
		return;

	if (_drop_mine_group)
	{
		UDOctets val((int)0);
		if (!world_manager::GetUniqueDataMan().GetData(_drop_mine_group,val))
			return;
		if (0 == (int)val)	//�жϷ���
			return;
	}

	// ���Ե��������
	object_interface oi(this);
	object_interface::mine_param param;
	memset(&param, 0, sizeof(object_interface::mine_param));
	if (!world_manager::GetDataMan().generate_mine_from_monster(GetSrcMonster(),param.mine_id,param.remain_time))
	{
		GLog::log(GLOG_ERR,"NPC drop mine error. id=%d",GetSrcMonster());
		return;
	}
	if (param.mine_id <= 0)
	{
		GLog::log(GLOG_ERR,"NPC drop mine error. npcid=%d mineid=%d",GetSrcMonster(),param.mine_id);
		return;
	}
	oi.CreateMine(_parent->pos,param);
}

bool
gnpc_imp::DropItemFromGlobal(const XID & owner, int owner_level , int team_id, int team_seq, int wallow_level)
{
	//����Ƿ����ȫ�ֵ����
	drop_template::drop_entry * pEntry = drop_template::GetDropList(GetSrcMonster());
	if(!pEntry) return true;
	float money_adj;
	float global_money_adj;
	float drop_adj;
	player_template::GetDropPunishment(owner_level - _basic.level, &money_adj, &drop_adj);
	drop_adj *= _drop_rate;
	
	player_template::GetGlobalMoneyBonus(&global_money_adj);
	money_adj *= 1.0f + global_money_adj;

	if(world_manager::AntiWallow())
	{
		anti_wallow::AdjustNormalMoneyItem(wallow_level, money_adj,drop_adj);
	}
		
	int money = 0;
	if(pEntry->type == EDT_TYPE_REPLACE) 
	{
		int low,high;
		world_manager::GetDataMan().get_monster_drop_money(GetSrcMonster(),low,high);
		money = (int)(abase::Rand(low,high) * _money_scale* money_adj + 0.5f);
		float md = abase::RandUniform();
		if(md > MONEY_DROP_RATE) money = 0;
	}

	if(abase::RandUniform() <= drop_adj)
	{
		int drop_list[48];
		//������Ʒ 
		int count = sizeof(drop_list)/sizeof(int)-4;
		int rst = drop_template::GenerateItem(pEntry ,drop_list + 4 ,count);
		if(rst > 0)
		{
			drop_list[0] = team_id;
			drop_list[1] = team_seq;
			drop_list[2] = GetSrcMonster();
			drop_list[3] = rst;
			MSG msg;
			BuildMessage(msg,GM_MSG_PRODUCE_MONSTER_DROP,world_manager::GetServerID(),
					owner ,_parent->pos,money,drop_list,(rst + 4)*sizeof(int));
			_plane->PostLazyMessage(msg);

			money = 0;
		}
	}

	if(money > 0)
	{
		MSG msg;
		msg_gen_money mgm = { team_id, team_seq};
		BuildMessage(msg,GM_MSG_PRODUCE_MONEY,world_manager::GetServerID(),owner
				,_parent->pos,money , &mgm,sizeof(mgm));
		_plane->PostLazyMessage(msg);
	}
	return pEntry->type != EDT_TYPE_REPLACE;
} 

void
gnpc_imp::DropItemFromData(const XID & owner, int owner_level, int team_id,int team_seq, int wallow_level)
{
	float money_adj,drop_adj;
	player_template::GetDropPunishment(owner_level - _basic.level, &money_adj, &drop_adj);
	drop_adj *= _drop_rate;

	if(world_manager::AntiWallow())
	{
		anti_wallow::AdjustNormalMoneyItem(wallow_level, money_adj,drop_adj);
	}

	//��õ������
	int drop_times = world_manager::GetDataMan().get_monster_drop_times(GetSrcMonster());
	int item_more_times = world_manager::GetWorldParam().double_drop?2:1;
	//if(drop_times > 4) item_more_times = 1;	//��������������4 �� �򲻽���˫�����������

	for(int t= 0; t < item_more_times; t ++)
	{
		if(abase::RandUniform() <= drop_adj)
		{
			int drop_list[48];
			int tmp;
			int rst = world_manager::GetDataMan().generate_item_from_monster(GetSrcMonster(),drop_list+4,sizeof(drop_list)/sizeof(int) - 4,&tmp);
			if(rst > 0)
			{
				drop_list[0] = team_id;
				drop_list[1] = team_seq;
				drop_list[2] = GetSrcMonster();
				drop_list[3] = rst;
				MSG msg;
				BuildMessage(msg,GM_MSG_PRODUCE_MONSTER_DROP,world_manager::GetServerID(),owner
						,_parent->pos,0,drop_list,(rst + 4)*sizeof(int));
				_plane->PostLazyMessage(msg);
			}
		}
	}

	//�����Ǯ
	int low;
	int high;
	world_manager::GetDataMan().get_monster_drop_money(GetSrcMonster(),low,high);
	int money_more_times = world_manager::GetWorldParam().double_money?2:1;
	//if(drop_times > 4) money_more_times = 1;
	if(high > 0)
	{
		float global_money_adj;
		player_template::GetGlobalMoneyBonus(&global_money_adj);
		money_adj *= 1.0f + global_money_adj;
		int times = drop_times * money_more_times;
		for(int i = 0; i < times; i ++)
		{
			int money = abase::Rand(low,high);
			if(money > 0 && abase::RandUniform() < MONEY_DROP_RATE)
			{
				// ����Ǯ
				money = (int)(money * _money_scale* money_adj + 0.5f);
				if(money > 0)
				{
					MSG msg;
					msg_gen_money mgm = { team_id, team_seq};
					BuildMessage(msg,GM_MSG_PRODUCE_MONEY,world_manager::GetServerID(),owner
							,_parent->pos,money , &mgm,sizeof(mgm));
					_plane->PostLazyMessage(msg);
				}
			}
		}
	}
}


void 
gnpc_imp::DropItem(const XID & owner, int owner_level, int team_id,int team_seq, int wallow_level)
{
	if(!DropItemFromGlobal(owner, owner_level , team_id, team_seq,wallow_level)) return;
	DropItemFromData(owner, owner_level, team_id,team_seq,wallow_level);
}

void
gnpc_imp::FestiveAward(int fa_type,int type,const XID & target)
{
	if(fa_type == FAT_MAFIA_PVP)
	{
		enum { NO_OWNER_EVENT_OFFSET = 5 }; 
		int domain = city_region::GetDomainID(_birth_place.x,_birth_place.z);
		
		if(!world_manager::GetWorldFlag().mafia_pvp_flag)
		{
			GLog::log(GLOG_ERR,"���:%d NPC:%d�ڰ���pvpδ����ʱ������:%d ",domain,((gnpc*)_parent)->tid,type);
			return;
		}

		if(target.IsErrorType()) 
		{
			GMSV::SendMafiaPvPEvent(type+NO_OWNER_EVENT_OFFSET,0,OI_GetMafiaID(),0,0,domain);
			GLog::log(GLOG_ERR,"���:%d NPC:%d�Դ���Ŀ�귢�����pvp����:%d ",domain,((gnpc*)_parent)->tid,type);
			return;
		}

		if(target.type == GM_TYPE_PLAYER)
		{
			world::object_info info;
			if(!_plane->QueryObject(target,info))
			{
				GMSV::SendMafiaPvPEvent(type+NO_OWNER_EVENT_OFFSET,0,OI_GetMafiaID(),0,0,domain);
				GLog::log(GLOG_ERR,"���:%d NPC:%d�Բ��ڱ�����ɫ:%d�������pvp����:%d ",domain,((gnpc*)_parent)->tid,target.id,type);
			}
			else
			{
				MSG msg;
				msg_mafia_pvp_award_t mfa = { OI_GetMafiaID(), domain};
				BuildMessage(msg, GM_MSG_MAFIA_PVP_AWARD, target, _parent->ID, _parent->pos, type , &mfa, sizeof(mfa));
				_plane->PostLazyMessage(msg);
				GLog::log(GLOG_INFO,"���:%d NPC:%d ���ɫ:%d �������pvp����:%d ֪ͨ",domain, ((gnpc*)_parent)->tid, target.id, type);
			}
		}
		else if(target.type == GM_TYPE_MAFIA)
		{
			GMSV::SendMafiaPvPEvent(type,OI_GetMafiaID(),OI_GetMafiaID(),-1,0,domain);	
			GLog::log(GLOG_INFO,"���:%d NPC:%d �����:%d �������pvp����:%d ֪ͨ",domain, ((gnpc*)_parent)->tid, target.id, type);
		}

		if(GetFaction() & FACTION_MPVP_MINE_BASE)
		{
			DATA_TYPE dt;
			FACTION_PVP_CONFIG * config = (FACTION_PVP_CONFIG*) world_manager::GetDataMan().get_data_ptr(FACTION_PVP_CONFIG_ID, ID_SPACE_CONFIG, dt);
			if(config == NULL || dt != DT_FACTION_PVP_CONFIG || domain < 1 || domain > int(sizeof(config->controller_id)/sizeof(config->controller_id[0])))
			{
				GLog::log(GLOG_ERR,"����pvp FACTION_PVP_CONFIG ����:%d���� ",domain);
				return;
			}

			_plane->ClearSpawn(config->controller_id[domain - 1]);
			GLog::log(GLOG_INFO,"����pvp�󳵻���:%d�򱬹رտ�����:%d",domain, config->controller_id[domain - 1]);
		}
	}
}

int
gnpc_imp::OI_GetMafiaID()
{
	return ((gnpc*)_parent)->mafia_id;
}

void
gnpc_imp::AdjustLocalControlID(int& cid)
{
	// do not adjust globalid 0
	if(cid) cid = npc_generator::GenBlockUniqueID(cid, _plane->GetBlockID(_birth_place.x,_birth_place.z));	 
}

int
gnpc_imp::GetMazeRoomIndex()
{
	return _plane->GetRoomIndex(_birth_place.x,_birth_place.z);
}

void 
gnpc_imp::DeliverTaskToTarget(const XID& target,int taskid) 
{
	SendTo<0>(GM_MSG_DELIVER_TASK,target,taskid);	
}	

int
gnpc_imp::ChangeVisibleTypeId(int tid)
{
	gnpc* pObj = (gnpc*)_parent;
	int oldvtid = pObj->vis_tid;
	pObj->vis_tid = tid;
	_runner->notify_visible_tid_change();
	return oldvtid;
}

void 
gnpc_controller::SetTargetCache(const XID& target)
{
	if(target != _cur_target_cache)
	{
		_cache_refresh_state = true;
		((gnpc_imp*)_imp)->SetRefreshState();
	}
	_cur_target_cache = target;
}

void
gnpc_imp::SetTargetCache(const XID& target)
{
	((gnpc_controller*)_commander)->SetTargetCache(target);
}

const XID &
gnpc_imp::GetCurTarget()
{
	return ((gnpc_controller*)_commander)->GetCurTarget();
}


