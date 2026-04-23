#include <common/protocol.h>
#include "obj_interface.h"
#include <stocklib.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include "userlogin.h"
#include "playertemplate.h"
#include "serviceprovider.h"
#include <common/protocol_imp.h>
#include "trade.h"
#include "task/taskman.h"
#include "sitdown_filter.h"
#include "playerstall.h"
#include "pvplimit_filter.h"
#include "pk_protected_filter.h"
#include <glog.h>
#include "pathfinding/pathfinding.h"
#include "player_mode.h"
#include "cooldowncfg.h"
#include "item/item_petegg.h"
#include "template/globaldataman.h"
#include "petnpc.h"
#include "antiwallow.h"
#include "../common/chatdata.h"
#include "teamrelationjob.h"
#include "instance/faction_world_ctrl.h"
#include <base64.h>
#include <db_if.h>
#include "rune_filter.h"
#include "item/generalcard_set_man.h"
#include "item/item_generalcard.h"
#include "template/el_region.h"
#include "playermnfaction.h"

#include "global_controller.h"
#include "player_money.h"
#include "luamanager.h"

DEFINE_SUBSTANCE(gplayer_dispatcher,dispatcher,CLS_PLAYER_DISPATCHER)

using namespace S2C;

enum
{	//?????
	MIN_SEND_COUNT = 128
};


void 
gplayer_dispatcher::begin_transfer()
{
	ASSERT(_mw.size() == 0);
	ASSERT(_nw.size() == 0);
	ASSERT(_pw.size() == 0);
	ASSERT(_self.size() == 0);
	ASSERT(_leave_list.size() == 0);
}

void 
gplayer_dispatcher::end_transfer()
{
	if(!_self.empty()){
		send_ls_msg(_header,_self.data(),_self.size());
		_self.clear();
	}
	wrapper_test<0>(_pw,_header,S2C::PLAYER_INFO_1_LIST);
	wrapper_test<0>(_mw,_header,S2C::MATTER_INFO_LIST);
	wrapper_test<0>(_nw,_header,S2C::NPC_INFO_LIST);
	if(_leave_list.size())
	{
		_tbuf.clear();

		CMD::Make<CMD::OOS_list>::From(_tbuf,_leave_list.size(), _leave_list.begin());
		send_ls_msg(_header,_tbuf.data(),_tbuf.size());
		_leave_list.clear();
	}
	
}

void 
gplayer_dispatcher::enter_slice(slice *  pPiece,const A3DVECTOR &pos)
{
	_tbuf.clear();

	gplayer * player = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_slice>::From(_tbuf,player,pos);

	cs_user_map map;
	pPiece->Lock();
	if(player->IsInvisible())
		gather_slice_cs_user_in_invisible(pPiece,map,player->invisible_degree,player->team_id);
	else
		gather_slice_cs_user(pPiece,map);
	pPiece->Unlock();
	
	multi_send_ls_msg(map,_tbuf,_imp->_parent->ID.id);

	//?????????????
	//get_slice_info(pPiece,_nw,_mw,_pw);
	get_slice_info_visible(pPiece,_nw,_mw,_pw,player->anti_invisible_degree,-1,player->team_id);
}

void 
gplayer_dispatcher::leave_slice(slice * pPiece,const A3DVECTOR &pos)
{
	_tbuf.clear();

	gplayer * player = (gplayer*)_imp->_parent;
	CMD::Make<CMD::leave_slice>::From(_tbuf,player);
	//?????????
	cs_user_map map;
	pPiece->Lock();
	if(player->IsInvisible())
		gather_slice_cs_user_in_invisible(pPiece,map,player->invisible_degree,player->team_id);
	else
		gather_slice_cs_user(pPiece,map);
	//gather_slice_object(pPiece,_leave_list);
	gather_slice_object_visible(pPiece,_leave_list,player->anti_invisible_degree);
	pPiece->Unlock();
	
	multi_send_ls_msg(map,_tbuf,_imp->_parent->ID.id);
}

void 
gplayer_dispatcher::start_attack(const XID & target)
{
/*
	_tbuf.clear();
	gactive_imp * pImp = (gactive_imp *)_imp;
	gplayer * pObj = (gplayer*)pImp->_parent;
	CMD::Make<CMD::object_start_attack>::From(_tbuf,pObj,target.id,pImp->_cur_prop.attack_speed);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf.data(),_tbuf.size(),pObj->ID.id);
	*/

	//?????????????????????????????
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp *)_imp;
	gplayer * pObj = (gplayer*)pImp->_parent;
	unsigned char speed = pImp->_cur_prop.attack_speed;
	CMD::Make<CMD::self_start_attack>::From(_tbuf,target.id,speed,pImp->GetAmmoCount());
	send_ls_msg(pObj,_tbuf);
}

void 
gplayer_dispatcher::attack_once(unsigned char state)
{
	_tbuf.clear();

	gplayer * pObj = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_attack_once>::From(_tbuf,state);
	send_ls_msg(pObj,_tbuf);
}

void 
gplayer_dispatcher::be_damaged(const XID & id, int skill_id ,const attacker_info_t & info,int damage,int dura_index,int at_state,char speed,bool orange,unsigned char section)
{

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(skill_id)
	{
		if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			_tbuf.clear();
			CMD::Make<CMD::self_skill_attack_result>::From(_tbuf,pPlayer->ID,skill_id,damage,at_state,speed ,section);
			send_ls_msg(info.cs_index,id.id,info.sid,_tbuf);
			_tbuf.clear();
			CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,id,pPlayer->ID,skill_id,damage,at_state ,speed ,section);
			AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,id.id,pPlayer->ID.id,0);
		}
		else
		{
			_tbuf.clear();
			CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,id,pPlayer->ID,skill_id,damage,at_state ,speed ,section);
			AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
		}

		_tbuf.clear();
		CMD::Make<CMD::be_skill_attacked>::From(_tbuf,id,skill_id,damage,dura_index,orange,at_state,speed, section);
		send_ls_msg(pPlayer,_tbuf);
	}
	else
	{
		if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			_tbuf.clear();
			CMD::Make<CMD::self_attack_result>::From(_tbuf,pPlayer->ID,damage,at_state,speed);
			send_ls_msg(info.cs_index,id.id,info.sid,_tbuf);
			_tbuf.clear();
			CMD::Make<CMD::object_attack_result>::From(_tbuf,id,pPlayer->ID,damage,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,id.id,pPlayer->ID.id,0);
		}
		else
		{
			_tbuf.clear();
			CMD::Make<CMD::object_attack_result>::From(_tbuf,id,pPlayer->ID,damage,at_state ,speed);
			AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
		}

		_tbuf.clear();
		CMD::Make<CMD::be_attacked>::From(_tbuf,id,damage,dura_index,orange,at_state,speed);
		send_ls_msg(pPlayer,_tbuf);
	}
}

void 
gplayer_dispatcher::be_hurt(const XID & id, const attacker_info_t & info,int damage,bool invader)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(id.type == GM_TYPE_PLAYER && info.cs_index >= 0)
	{
		
		CMD::Make<CMD::hurt_result>::From(_tbuf,pPlayer->ID,damage);
		send_ls_msg(info.cs_index,id.id,info.sid,_tbuf);
		_tbuf.clear();
	}

	CMD::Make<CMD::be_hurt>::From(_tbuf,id,damage,invader);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::dodge_attack(const XID &attacker, int skill_id, const attacker_info_t & info, int at_state,char speed,bool orange,unsigned char section)
{

	gplayer *pObj = (gplayer*)_imp->_parent;
	if(skill_id)
	{
		_tbuf.clear();
		if(attacker.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,attacker,pObj->ID,skill_id,0,at_state,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id,attacker.id,0);

			_tbuf.clear();
			CMD::Make<CMD::self_skill_attack_result>::From(_tbuf,pObj->ID,skill_id,0,at_state,speed,section);
			send_ls_msg(info.cs_index,attacker.id,info.sid,_tbuf);

		}
		else
		{
			CMD::Make<CMD::object_skill_attack_result>::From(_tbuf,attacker,pObj->ID,skill_id,0,at_state,speed,section);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id);
		}

		_tbuf.clear();
		CMD::Make<CMD::be_skill_attacked>::From(_tbuf,attacker,skill_id,0,0xff,orange,at_state,speed, section);
		send_ls_msg(pObj,_tbuf);
	}
	else
	{
		_tbuf.clear();
		if(attacker.type == GM_TYPE_PLAYER && info.cs_index >= 0)
		{
			CMD::Make<CMD::object_attack_result>::From(_tbuf,attacker,pObj->ID,0,at_state,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id,attacker.id,0);

			_tbuf.clear();
			CMD::Make<CMD::self_attack_result>::From(_tbuf,pObj->ID,0,at_state,speed);
			send_ls_msg(info.cs_index,attacker.id,info.sid,_tbuf);

		}
		else
		{
			CMD::Make<CMD::object_attack_result>::From(_tbuf,attacker,pObj->ID,0,at_state,speed);
			AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id);
		}

		_tbuf.clear();
		CMD::Make<CMD::be_attacked>::From(_tbuf,attacker,0,0xff,orange,at_state,speed);
		send_ls_msg(pObj,_tbuf);
	}
}

void
gplayer_dispatcher::equipment_damaged(int index,char reason)
{
	_tbuf.clear();

	CMD::Make<CMD::equipment_damaged>::From(_tbuf,index,reason);
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
}

void 
gplayer_dispatcher::on_death(const XID &killer,bool )
{
	_tbuf.clear();

	gobject * pObj = _imp->_parent;
	
	CMD::Make<CMD::be_killed>::From(_tbuf,pObj,killer);
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
	__PRINTF("send be killed message\n");
	
	_tbuf.clear();
	CMD::Make<CMD::player_dead>::From(_tbuf,killer,pObj->ID);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,pObj->ID.id);
}

void 
gplayer_dispatcher::stop_attack(int flag)
{
	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("player stop attack %ld.%06ld\n",tv.tv_sec,tv.tv_usec);

	_tbuf.clear();

	CMD::Make<CMD::self_stop_attack>::From(_tbuf,flag);

	gobject * pObj = _imp->_parent;
	send_ls_msg((gplayer*)pObj,_tbuf);
}

void 
gplayer_dispatcher::resurrect(int level)
{	
//	timeval tv;
//	gettimeofday(&tv,NULL);
//	__PRINTF("%d.%06d player resurrect\n",tv.tv_sec,tv.tv_usec);
	_tbuf.clear();

	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::player_revival>::From(_tbuf,pObj,level);
	//????????????????
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);//?????????????????
//	gettimeofday(&tv,NULL);
//	__PRINTF("%d.%06d player resurrect\n",tv.tv_sec,tv.tv_usec);
}

void 
gplayer_dispatcher::notify_move(const A3DVECTOR &oldpos, const A3DVECTOR & newpos)
{
	//???????????????????????
	rect rt(oldpos,newpos);

	packet_raw_wrapper d1(128);
	packet_wrapper l1(64),e1(64); 	
	//????????????????????????????link server????????????????????????????

	CMD::Make<CMD::player_enter_slice>::From(e1,(gplayer*)_imp->_parent,newpos);
	CMD::Make<CMD::leave_slice>::From(l1,_imp->_parent);

	msg_usermove_t mt;
	mt.cs_index = _header.cs_id;
	mt.cs_sid =  _header.cs_sid;
	mt.user_id = _header.user_id;
	mt.newpos = newpos;
	mt.leave_data_size = l1.size();
	mt.enter_data_size = e1.size();
	d1.push_back(&mt,sizeof(mt));
	//mt.Export(e1);
	d1.push_back(l1.data(),l1.size());
	d1.push_back(e1.data(),e1.size());

	MSG msg;
	BuildMessage(msg,GM_MSG_USER_MOVE_OUTSIDE,XID(GM_TYPE_BROADCAST,-1),_imp->_parent->ID,
					oldpos,0,d1.data(),d1.size());
	_imp->_plane->BroadcastSvrMessage(rt,msg,GRID_SIGHT_RANGE);
}

void 
gplayer_dispatcher::notify_pos(const A3DVECTOR & pos)
{	
	_tbuf.clear();

	CMD::Make<CMD::notify_pos>::From(_tbuf,pos,world_manager::GetWorldTag(),
			world_manager::GetInstance()->GetWorldType() == WORLD_TYPE_PARALLEL_WORLD ? _imp->_plane->w_ins_key : instance_hash_key());
	send_ls_msg((gplayer*)(_imp->_parent),_tbuf);
}

void 
gplayer_dispatcher::get_base_info()
{

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::self_info_1>::From(_self,(gplayer*)pImp->_parent,pImp->_basic.exp,pImp->_basic.skill_point);

	//_imp->GetBaseInfo(_self);
}

void 
gplayer_dispatcher::query_info00(const XID & target, int cs_index,int sid)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp *)_imp;
	//_back_up_hp[1]
	CMD::Make<CMD::player_info_00>::From(_tbuf,pImp->_parent->ID,pImp->_basic.hp,pImp->_basic,pImp->_cur_prop,pImp->IsCombatState()?1:0, pImp->GetCurTarget().id, pImp->GlyphGetAttr() );
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::query_info00()
{
/*
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_info_00>::From(_tbuf,pImp->_parent->ID,pImp->_basic,pImp->_cur_prop);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
	*/
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::self_info_00>::From(_tbuf,pImp->_basic,pImp->_cur_prop,pImp->IsCombatState()?1:0,
			pImp->_cheat_punish);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}
//lgc
void gplayer_dispatcher::query_elf_vigor()
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::elf_vigor>::From(_tbuf, (int)pImp->_cur_elf_info.vigor, (int)pImp->_cur_elf_info.max_vigor, int(pImp->_cur_elf_info.vigor_gen*100+0.5f));
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}

void gplayer_dispatcher::query_elf_enhance()
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::elf_enhance>::From(_tbuf,
						pImp->_cur_elf_info.final_strength - pImp->_cur_elf_info.strength,
						pImp->_cur_elf_info.final_agility - pImp->_cur_elf_info.agility, 
						pImp->_cur_elf_info.final_vitality - pImp->_cur_elf_info.vitality, 
						pImp->_cur_elf_info.final_energy - pImp->_cur_elf_info.energy);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}

void gplayer_dispatcher::query_elf_stamina(int sta)
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::elf_stamina>::From(_tbuf, sta);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}

void gplayer_dispatcher::query_elf_exp(int exp)
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::elf_exp>::From(_tbuf, exp);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}

void gplayer_dispatcher::elf_cmd_result(int cmd, int result, int param1, int param2)
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::elf_cmd_result>::From(_tbuf, cmd, result, param1, param2);
	send_ls_msg((gplayer *)pImp->_parent,_tbuf);
}

void gplayer_dispatcher::elf_levelup()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_elf_levelup>::From(_tbuf, pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_move>::From(_tbuf,pPlayer,target,cost_time,speed,move_mode);

	slice * pPiece = pPlayer->pPiece;
	
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);//?????????????????
}

void
gplayer_dispatcher::stop_move(const A3DVECTOR & target, unsigned short speed ,unsigned char dir,unsigned char move_mode)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_stop_move>::From(_tbuf,pPlayer,target,speed,dir,move_mode);

	pPlayer->dir = dir;
	slice * pPiece = pPlayer->pPiece;

	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);
}


void
gplayer_dispatcher::leave_world()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)(_imp->_parent);
	world * pPlane = _imp->_plane;
	CMD::Make<CMD::player_leave_world>::From(_tbuf,pPlayer);
	slice *pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);

	//???????????????????????
	if(pPiece->IsBorder())
	{
		extern_object_manager::SendDisappearMsg<0>(_imp->_plane,pPlayer,pPiece);
	}
}

void 
gplayer_dispatcher::enter_region()
{
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	//get_slice_info(pPiece,_nw,_mw,_pw);
	gplayer * pPlayer = (gplayer*)(_imp->_parent);
	get_slice_info_visible(pPiece,_nw,_mw,_pw,pPlayer->anti_invisible_degree,-1,pPlayer->team_id);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{
		world::off_node_t &node = pPlane->w_off_list[i]; 
		int nx = slice_x + node.x_off; 
		int nz = slice_z + node.z_off; 
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{
			//get_slice_info(pNewPiece,_nw,_mw,_pw);
			get_slice_info_visible(pNewPiece,_nw,_mw,_pw,pPlayer->anti_invisible_degree,-1,pPlayer->team_id);
		}
		else
		{
			//get_slice_info(pNewPiece,_nw,_mw,_pw);
			get_slice_info_visible(pNewPiece,_nw,_mw,_pw,pPlayer->anti_invisible_degree,-1,pPlayer->team_id);
		}
		wrapper_test<MIN_SEND_COUNT>(_pw,_header,S2C::PLAYER_INFO_1_LIST);
		wrapper_test<MIN_SEND_COUNT>(_mw,_header,S2C::MATTER_INFO_LIST);
		wrapper_test<MIN_SEND_COUNT>(_nw,_header,S2C::NPC_INFO_LIST);
	}
	return ;
}

void 
gplayer_dispatcher::enter_world()
{
	world * pPlane = _imp->_plane;
	//??????????????????
	_tbuf.clear();

	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_world>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);

	//???????????????????????
	if(pPiece->IsBorder())
	{
		extern_object_manager::SendAppearMsg<0>(_imp->_plane,pPlayer,pPiece);
		MSG msg;
		BuildMessage(msg,GM_MSG_USER_APPEAR_OUTSIDE,XID(GM_TYPE_BROADCAST,-1),pPlayer->ID,
				pPlayer->pos,pPlayer->cs_index,&(pPlayer->cs_sid),sizeof(int));
		_imp->_plane->BroadcastSvrMessage(pPiece->slice_range,msg,GRID_SIGHT_RANGE);  //?????????????
	}

	//???????????????????????
	enter_region();
}

void 
gplayer_dispatcher::appear()
{
	world * pPlane = _imp->_plane;
	//??????????????????
	_tbuf.clear();

	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_world>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(pPlane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(pPlane,pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::error_message(int msg)
{
	_tbuf.clear();

	CMD::Make<CMD::error_msg>::From(_tbuf,msg);
	//?????????
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void
gplayer_dispatcher::disappear()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer *)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	if(pPlayer->IsInvisible())
		AutoBroadcastCSMsgInInvisible(_imp->_plane,pPiece,_tbuf,pPlayer->invisible_degree,pPlayer->team_id,pPlayer->ID.id);
	else
		AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf,pPlayer->ID.id);

	//???????????????????????
	if(pPiece->IsBorder())
	{
		extern_object_manager::SendDisappearMsg<0>(_imp->_plane,pPlayer,pPiece);
	}
}

void 
gplayer_dispatcher::receive_exp(int exp,int sp)
{
	_tbuf.clear();

	CMD::Make<CMD::receive_exp>::From(_tbuf,exp,sp);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void
gplayer_dispatcher::embed_item(unsigned int chip_idx, unsigned int equip_idx)
{
	_tbuf.clear();

	CMD::Make<CMD::embed_item>::From(_tbuf,chip_idx,equip_idx);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::level_up()
{
	_tbuf.clear();

	gobject * pObj = _imp->_parent;
	CMD::Make<CMD::level_up>::From(_tbuf,pObj);
	slice * pPiece = pObj->pPiece;
	AutoBroadcastCSMsg(_imp->_plane,pPiece,_tbuf);
}

void 
gplayer_dispatcher::unselect()
{
	_tbuf.clear();

	CMD::Make<CMD::unselect>::From(_tbuf);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::player_select_target(int id)
{
	_tbuf.clear();

	CMD::Make<CMD::player_select_target>::From(_tbuf,id);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_base()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_base>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
	
}

void 
gplayer_dispatcher::get_extprop_move()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_move>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_attack()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_attack>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::get_extprop_defense()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::player_extprop_defense>::From(_tbuf,pImp);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::player_reject_invite(const XID & member)
{
	__PRINTF("team::player_reject_invite %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_reject_invite>::From(_tbuf,member);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leader_invite(const XID & leader,int seq,int pickup_flag)
{
	__PRINTF("team::leader_invite %d  seq %d\n",_imp->_parent->ID.id,seq);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leader_invite>::From(_tbuf,leader,seq,pickup_flag);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::join_team(const XID & leader,int pickup_flag)
{
	__PRINTF("team::join_team %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_join_team>::From(_tbuf,leader,pickup_flag);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::member_leave(const XID & leader, const XID& member,int type)
{
	__PRINTF("team::member_leave %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_leave>::From(_tbuf,leader,member,type);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leave_party(const XID & leader, int type)
{
	__PRINTF("team::leave_party %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leave_party>::From(_tbuf,leader,type);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::new_member(const XID & member)
{
	__PRINTF("team::new_member %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_new_member>::From(_tbuf,member);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::leader_cancel_party(const XID & leader)
{
	//????????????
	ASSERT(false && "????????????");
	__PRINTF("team::leader_cancel_party %d\n",_imp->_parent->ID.id);
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_leader_cancel_party>::From(_tbuf,leader);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::npc_greeting(const XID & provider)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::npc_greeting>::From(_tbuf,provider);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::repair_all(unsigned int cost)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::repair_all>::From(_tbuf,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::repair(int where,int index, unsigned int cost)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::repair>::From(_tbuf,where,index,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::renew()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::renew>::From(_tbuf);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

/*
void 
gplayer_dispatcher::spend_money(unsigned int cost)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::spend_money>::From(_tbuf,cost);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}
*/

void 
gplayer_dispatcher::get_player_money(unsigned int money, unsigned long long silver)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::get_own_money>::From(_tbuf,money, gplayer_money::MAX_GOLD, silver);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::cast_skill(const XID & target, int skill,unsigned short time, unsigned char level)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::object_cast_skill>::From(_tbuf,pPlayer->ID,target,skill,time,level);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cast_rune_skill(const XID & target, int skill,unsigned short time, unsigned char level)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::player_cast_rune_skill>::From(_tbuf,pPlayer->ID,target,skill,time,level);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::skill_interrupt(char reason)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::skill_interrupted>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_skill_interrupted>::From(_tbuf,reason);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::stop_skill()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_stop_skill>::From(_tbuf);
	send_ls_msg(pPlayer, _tbuf);
}


void
gplayer_dispatcher::takeoff()
{
	_tbuf.clear();

	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_takeoff>::From(_tbuf,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::landing()
{
	_tbuf.clear();

	gobject * pObj= (gobject*)_imp->_parent;
	CMD::Make<CMD::object_landing>::From(_tbuf,pObj);
	AutoBroadcastCSMsg(_imp->_plane,pObj->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::flysword_time_capacity(unsigned char where, unsigned char index, int cur_time)
{
	_tbuf.clear();

	CMD::Make<CMD::flysword_time_capacity>::From(_tbuf,where,index,cur_time);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::skill_perform()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::skill_perform>::From(_tbuf);
//	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1); ??????
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::teammate_get_pos(const XID & target,const A3DVECTOR & pos,int tag,bool same_plane) 
{
	_tbuf.clear();

	CMD::Make<CMD::teammate_pos>::From(_tbuf,target,pos,tag,(same_plane?1:0));
	send_ls_msg((gplayer*)_imp->_parent,_tbuf);
}

void 
gplayer_dispatcher::send_equipment_info(const XID & target, int cs_index, int sid)
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::send_equipment_info>::From(_tbuf,pPlayer,pImp->_equip_info.mask,pImp->_equip_info.data);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::equipment_info_changed(uint64_t madd,uint64_t mdel,const void * buf, unsigned int size)
{
	_tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp *)_imp;
	gplayer * pPlayer = (gplayer*)pImp->_parent;
	CMD::Make<CMD::equipment_info_changed>::From(_tbuf,pPlayer,madd,mdel,buf,size);
	AutoBroadcastCSMsg(pImp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::team_member_pickup(const XID & member, int type, int count)
{
	_tbuf.clear();

	CMD::Make<CMD::team_member_pickup>::From(_tbuf,member, type, count);
	send_ls_msg((gplayer*)(_imp->_parent), _tbuf);
}

void 
gplayer_dispatcher::send_team_data(const XID & leader,unsigned int team_count,unsigned int data_count, const player_team::member_entry ** list)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_data>::From(_tbuf,leader,team_count,data_count,list);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::send_team_data(const XID & leader,unsigned int team_count,const player_team::member_entry * list)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::team_member_data>::From(_tbuf,leader,team_count,list);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

void 
gplayer_dispatcher::item_to_money(unsigned int index, int type, unsigned int count,unsigned int price, unsigned int silver_price, unsigned int guild_price)
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp*)_imp;
	CMD::Make<CMD::item_to_money>::From(_tbuf,index,type,count,price, silver_price, guild_price);
	send_ls_msg((gplayer*)(pImp->_parent), _tbuf);
}

/*
void 
gplayer_dispatcher::pickup_money(int money, unsigned char is_silver)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_money>::From(_tbuf,money,is_silver);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(abase::Rand(0,0) == 1)
	{
		std::pair<int,int> pp(pPlayer->ID.id,pPlayer->cs_sid);
		GMSV::MultiSendClientData(pPlayer->cs_index,&pp,&pp,_tbuf.data(),_tbuf.size(),0);

	}
	else
	{
		send_ls_msg(pPlayer, _tbuf);
	}
}
*/

void 
gplayer_dispatcher::pickup_item(int type,int expire_date, int amount, int slot_amount, int where,int index)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_item>::From(_tbuf,type,expire_date,amount,slot_amount,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::obtain_item(int type, int expire_date, int amount, int slot_amount, int where,int index)
{
	_tbuf.clear();

	CMD::Make<CMD::player_obtain_item>::From(_tbuf,type, expire_date,amount,slot_amount,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_once(int type, int amount, int slot_amount, int where,int index)
{
	_tbuf.clear();

	CMD::Make<CMD::produce_once>::From(_tbuf,type,amount,slot_amount,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_start(int type, int use_time ,int count)
{
	_tbuf.clear();

	CMD::Make<CMD::produce_start>::From(_tbuf,type,use_time,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::produce_end()
{
	_tbuf.clear();

	CMD::Make<CMD::produce_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::decompose_start(int use_time,int type)
{
	_tbuf.clear();

	CMD::Make<CMD::decompose_start>::From(_tbuf,use_time,type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::decompose_end()
{
	_tbuf.clear();

	CMD::Make<CMD::decompose_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::pickup_money_in_trade(unsigned int money)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_money_in_trade>::From(_tbuf,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::pickup_item_in_trade(int type, int amount)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_item_in_trade>::From(_tbuf,type,amount);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::receive_money_after_trade(unsigned int money)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_money_after_trade>::From(_tbuf,money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::receive_item_after_trade(int type, int expire_date, int amount, int slot_amount,int index)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_item_after_trade>::From(_tbuf,type,expire_date,amount, slot_amount,index & 0xFFFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void 
gplayer_dispatcher::purchase_item(int type, unsigned int money,int amount, int slot_amount, int where,int index)
{
	ASSERT(false);
/*
	_tbuf.clear();

	CMD::Make<CMD::player_purchase_item>::From(_tbuf,type,money,amount & 0xFFFF,slot_amount & 0xFFFF,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
	*/
}

void 
gplayer_dispatcher::self_item_empty_info(int where,int index) 
{
	_tbuf.clear();

	CMD::Make<CMD::self_item_empty_info>::From(_tbuf,where, index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_item_info(int where,int index,item_data & data,unsigned short crc) 
{
	_tbuf.clear();

	CMD::Make<CMD::self_item_info>::From(_tbuf,where, index,data,crc);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_inventory_data(int where,unsigned char inv_size, const void * data, unsigned int len)
{
	_tbuf.clear();

	CMD::Make<CMD::self_inventory_data>::From(_tbuf,where,inv_size,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::self_inventory_detail_data(int where,unsigned char inv_size,const void * data, unsigned int len)
{
	_tbuf.clear();

	CMD::Make<CMD::self_inventory_detail_data>::From(_tbuf,where,inv_size,data,len);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_inventory_item(unsigned int idx1,unsigned int idx2)
{
	_tbuf.clear();

	CMD::Make<CMD::exchange_inventory_item>::From(_tbuf,idx1,idx2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_inventory_item(unsigned int src,unsigned int dest,unsigned int count)
{
	_tbuf.clear();

	CMD::Make<CMD::move_inventory_item>::From(_tbuf,src,dest,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::player_drop_item(unsigned int where, unsigned int index,int type,unsigned int count, unsigned char drop_type)
{
	_tbuf.clear();

	CMD::Make<CMD::player_drop_item>::From(_tbuf,where,index,type,count,drop_type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_equipment_item(unsigned int index1,unsigned int index2)
{
	_tbuf.clear();

	CMD::Make<CMD::exchange_equipment_item>::From(_tbuf,index1,index2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::equip_item(unsigned int index_inv,unsigned int index_equip,int count_inv,int count_eq)
{
	_tbuf.clear();

	CMD::Make<CMD::equip_item>::From(_tbuf,index_inv,index_equip,count_inv,count_eq);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_equipment_item(unsigned int index_inv,unsigned int index_equip, unsigned int count)
{
	_tbuf.clear();

	CMD::Make<CMD::move_equipment_item>::From(_tbuf,index_inv,index_equip,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void 
gplayer_dispatcher::self_get_property(unsigned int status_point, const extend_prop & prop , int attack_degree, int defend_degree, int crit_rate, int crit_damage_bonus, int invisible_degree, int anti_invisible_degree, int penetration, int resilience, int vigour, int anti_def_degree, int anti_resist_degree, const extend_prop & base_prop, int peak_grade, int reserve)
{
	_tbuf.clear();
	CMD::Make<CMD::self_get_property>::From(_tbuf,status_point,prop,attack_degree, defend_degree, crit_rate, crit_damage_bonus, invisible_degree, anti_invisible_degree, penetration, resilience, vigour, anti_def_degree, anti_resist_degree, base_prop, (int)0, reserve); 
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;

	_tbuf << 0;
	_tbuf << 0;
	_tbuf << peak_grade;
	_tbuf << pImp->_soul_power;
	_tbuf << pImp->_basic.hp;

	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::set_status_point(unsigned int vit, unsigned int eng, unsigned int str, unsigned int agi, unsigned int remain)
{
	_tbuf.clear();

	CMD::Make<CMD::set_status_point>::From(_tbuf,vit,eng,str,agi,remain);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::get_skill_data()
{
	_tbuf.clear();

	gactive_imp * pImp = (gactive_imp *)_imp;
	raw_wrapper wrapper(256);
	pImp->_skill.StorePartial(wrapper);
	CMD::Make<CMD::skill_data>::From(_tbuf,wrapper.data(),wrapper.size());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::get_task_data()
{
	//���ﲻ��tbuf����Ϊ̫����
	packet_wrapper  h1(8192);
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	PlayerTaskInterface  task_if(pImp);

	unsigned int iActiveListLen = 0;
	char pActiveListBuf[TASK_ACTIVE_LIST_BUF_SIZE];
	task_if.SaveFixedEntry(pActiveListBuf, iActiveListLen);
	
	CMD::Make<CMD::task_data>::From(h1,pActiveListBuf,iActiveListLen,task_if.GetFinishedTaskList(),task_if.GetFnshLstDataSize(), task_if.GetFinishedTimeList(), task_if.GetFnshTimeLstDataSize(), task_if.GetFinishedCntList(), task_if.GetFnshCntLstDataSize(), task_if.GetStorageTaskList(), task_if.GetStorageTaskLstDataSize());
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, h1);
}

void 
gplayer_dispatcher::send_task_var_data(const void * buf, unsigned int size)
{
	//���ﲻ��tbuf����Ϊ̫����
	_tbuf.clear();
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	PlayerTaskInterface  task_if(pImp);
	CMD::Make<CMD::task_var_data>::From(_tbuf,buf,size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::clear_embedded_chip(unsigned short equip_idx,unsigned int use_money)
{
	_tbuf.clear();

	CMD::Make<CMD::clear_embedded_chip>::From(_tbuf,equip_idx, use_money);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::cost_skill_point(int sp)
{
	_tbuf.clear();

	CMD::Make<CMD::cost_skill_point>::From(_tbuf,sp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::learn_skill(int skill ,int level)
{
	_tbuf.clear();

	CMD::Make<CMD::learn_skill>::From(_tbuf,skill,level);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::use_item(char where, unsigned char index , int item_type , unsigned short count)
{
	_tbuf.clear();

	CMD::Make<CMD::player_use_item>::From(_tbuf,where, index, item_type, count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, unsigned int 
		arg_size)
{
	_tbuf.clear();

	CMD::Make<CMD::player_use_item_with_arg>::From(_tbuf,where, index, item_type, count,arg,arg_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf); 
}

void 
gplayer_dispatcher::use_item(int item_type,const char * arg, unsigned int arg_size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_use_item_with_arg>::From(_tbuf,pPlayer,item_type,arg,arg_size);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}


void 
gplayer_dispatcher::start_use_item(int item_type, int use_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_start_use>::From(_tbuf,pPlayer,item_type,use_time);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::start_use_item_with_target(int item_type, int use_time,const XID & target)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_start_use_with_target>::From(_tbuf,pPlayer,item_type,use_time,target);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cancel_use_item()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cancel_use>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::use_item(int item_type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_use_item>::From(_tbuf,pPlayer,item_type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::sit_down()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_sit_down>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::stand_up()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_stand_up>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_emote(unsigned short emotion)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_emote>::From(_tbuf,pPlayer,emotion);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_emote_restore(unsigned short emotion)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_emote_restore>::From(_tbuf,pPlayer,emotion);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::do_action(unsigned char action)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_do_action>::From(_tbuf,pPlayer,action);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_timestamp()
{
	_tbuf.clear();

	__PRINTF("%d %d\n",g_timer.get_systime(),time(NULL));
	gplayer * pPlayer = (gplayer*)_imp->_parent;
/*	struct timeval tv;
	struct timezone tz;
	gettimeofday(&tv, &tz);
*/
	time_t t1 = time(NULL);
	struct tm tm1;
	localtime_r(&t1, &tm1);
	CMD::Make<CMD::server_timestamp>::From(_tbuf,t1, -(tm1.tm_gmtoff/60), world_manager::GetLuaVersion());
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::notify_root(unsigned char type)
{
	_tbuf.clear();

	gplayer * pObject =(gplayer*) _imp->_parent;
	CMD::Make<CMD::notify_root>::From(_tbuf,pObject);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,_tbuf,pObject->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_notify_root>::From(_tbuf,pObject,type&0x7F);
	send_ls_msg(pObject, _tbuf);

}

void
gplayer_dispatcher::dispel_root(unsigned char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::dispel_root>::From(_tbuf,type);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::invader_rise()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::invader_rise>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::pariah_rise()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	__PRINTF("?????????state %d\n",pPlayer->pariah_state);
	CMD::Make<CMD::pariah_rise>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::invader_fade()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::invader_fade>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::gather_start(int mine, unsigned char t)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_gather_start>::From(_tbuf,pPlayer->ID,mine,t);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::gather_stop()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_gather_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::trashbox_passwd_changed(bool has_passwd)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_passwd_changed>::From(_tbuf,has_passwd);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_open(char is_usertrashbox)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp *pImp = (gplayer_imp*)_imp;
	player_trashbox * box;
	if(is_usertrashbox)
		box = &pImp->_user_trashbox;
	else
		box = &pImp->_trashbox;
	CMD::Make<CMD::trashbox_open>::From(_tbuf,is_usertrashbox,box->GetTrashBoxSize(0),box->GetTrashBoxSize(1),box->GetTrashBoxSize(2));
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_close(char is_usertrashbox)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_close>::From(_tbuf,is_usertrashbox);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_wealth(char is_usertrashbox, unsigned int money)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_wealth>::From(_tbuf,is_usertrashbox,money);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::trashbox_passwd_state(bool has_passwd)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_passwd_state>::From(_tbuf,has_passwd);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trashbox_item(int where, unsigned int idx1, unsigned int idx2)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trashbox_item>::From(_tbuf,where, idx1,idx2);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::move_trashbox_item(int where,unsigned int src, unsigned int dest, unsigned int delta)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::move_trashbox_item>::From(_tbuf,where, src,dest,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trashbox_inventory(int where,unsigned int idx_tra,unsigned int idx_inv)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trashbox_inventory>::From(_tbuf,where,idx_tra,idx_inv);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::trash_item_to_inventory(int where,unsigned int idx_tra, unsigned int idx_inv, unsigned int delta)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trash_item_to_inventory>::From(_tbuf,where, idx_tra,idx_inv,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::inventory_item_to_trash(int where,unsigned int idx_inv, unsigned int idx_tra, unsigned int delta)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::inventory_item_to_trash>::From(_tbuf,where, idx_inv,idx_tra,delta);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::exchange_trash_money(char is_usertrashbox, int inv_money, int tra_money)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::exchange_trash_money>::From(_tbuf,is_usertrashbox,inv_money,tra_money);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::set_adv_data(int data1,int data2)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_set_adv_data>::From(_tbuf,pPlayer,data1,data2);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::clear_adv_data()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_clr_adv_data>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_in_team(unsigned char state)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_in_team>::From(_tbuf,pPlayer,state);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::send_party_apply(int id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::team_apply_request>::From(_tbuf,id);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::query_info_1(int uid,int cs_index, int cs_sid)
{
	_tbuf.clear();

	CMD::Make<single_data_header>::From(_tbuf,PLAYER_INFO_1);
	if(CMD::Make<INFO::player_info_1>::From(_tbuf,(gplayer*)_imp->_parent))
	{
		send_ls_msg(cs_index,uid,cs_sid,_tbuf);
	}
}

void 
gplayer_dispatcher::concurrent_emote_request(int id, unsigned short action)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::concurrent_emote_request>::From(_tbuf,id,action);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::do_concurrent_emote(int id, unsigned short action)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::do_concurrent_emote>::From(_tbuf,pPlayer->ID.id, id,action);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::mafia_info_notify()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_info_notify>::From(_tbuf,pPlayer); 
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::mafia_trade_start()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_trade_start>::From(_tbuf); 
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::mafia_trade_end()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::mafia_trade_end>::From(_tbuf); 
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_item(int type, int expire_date, int amount, int slot_amount, int where,int index)
{
	_tbuf.clear();

	CMD::Make<CMD::task_deliver_item>::From(_tbuf,type,expire_date,amount,slot_amount,where & 0xFF,index & 0xFF);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_reputaion(int offset,int cur_reputaion)
{
	_tbuf.clear();

	CMD::Make<CMD::task_deliver_reputaion>::From(_tbuf,offset,cur_reputaion);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_exp(int exp, int sp)
{
	_tbuf.clear();

	CMD::Make<CMD::task_deliver_exp>::From(_tbuf,exp,sp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_money(unsigned int amount, unsigned int money, unsigned int silver)
{
	_tbuf.clear();

	CMD::Make<CMD::task_deliver_money>::From(_tbuf,amount,money, silver);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::task_deliver_level2(int level2)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::task_deliver_level2>::From(_tbuf,pPlayer,level2);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::get_reputation(int reputation)
{
	_tbuf.clear();

	CMD::Make<CMD::player_reputation>::From(_tbuf,reputation);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::identify_result(char index, char result)
{
	_tbuf.clear();

	CMD::Make<CMD::identify_result>::From(_tbuf,index,result);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::change_shape(char shape)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_shape>::From(_tbuf,pPlayer, shape);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

//lgc
void gplayer_dispatcher::elf_refine_activate(char status)	//status 0-??????? 1-????
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_elf_refine_activate>::From(_tbuf,pPlayer, status);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::enter_sanctuary()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_enter_sanctuary>::From(_tbuf, pPlayer->ID.id);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::leave_sanctuary()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_leave_sanctuary>::From(_tbuf, pPlayer->ID.id);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::begin_personal_market(int market_id, const char * name, unsigned int len)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_open_market>::From(_tbuf,pPlayer, market_id & 0xFF, name, len);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::cancel_personal_market()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cancel_market>::From(_tbuf,pPlayer);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::market_trade_success(int trader)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_market_trade_success>::From(_tbuf,trader);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::send_market_name(const XID & target, int cs_index, int sid,const char * name ,unsigned int len)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_market_name>::From(_tbuf,pPlayer,name,len);
	send_ls_msg(cs_index,target.id,sid,_tbuf);
}

void 
gplayer_dispatcher::player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_travel>::From(_tbuf,pPlayer,vehicle);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);

	_tbuf.clear();
	CMD::Make<CMD::self_start_travel>::From(_tbuf,speed,dest_pos,line_no,vehicle);
	send_ls_msg(pPlayer,_tbuf);

}

void 
gplayer_dispatcher::player_complete_travel(int vehicle)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_complete_travel>::From(_tbuf,pPlayer,vehicle);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::gm_toggle_invisible(char tmp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::gm_toggle_invisible>::From(_tbuf,tmp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::toggle_invincible(char tmp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::gm_toggle_invincible>::From(_tbuf,tmp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trace_cur_pos(unsigned short seq)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_trace_cur_pos>::From(_tbuf,pPlayer->pos,seq);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::cast_instant_skill(const XID & target, int skill,unsigned char level)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cast_instant_skill>::From(_tbuf,pPlayer,target,skill,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cast_rune_instant_skill(const XID & target, int skill,unsigned char level)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cast_rune_instant_skill>::From(_tbuf,pPlayer,target,skill,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cast_elf_skill(const XID & target, int skill,unsigned char level)	//lgc
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cast_elf_skill>::From(_tbuf,pPlayer,target,skill,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::activate_waypoint(unsigned short waypoint)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::activate_waypoint>::From(_tbuf,waypoint);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_waypoint_list(const unsigned short * list, unsigned int count)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_waypoint_list>::From(_tbuf,list,count);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::unlock_inventory_slot(unsigned char where, unsigned short index)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::unlock_inventory_slot>::From(_tbuf,where, index);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::team_invite_timeout(int who)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::team_invite_timeout>::From(_tbuf,who);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::enable_pvp_state(char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enable_pvp>::From(_tbuf,pPlayer,type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::disable_pvp_state(char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_disable_pvp>::From(_tbuf,pPlayer,type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_pvp_cooldown(int cool_down)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pvp_cooldown>::From(_tbuf,cool_down,LuaManager::GetInstance()->GetConfig()->pvp_cooldown_time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::send_cooldown_data()
{
	raw_wrapper rw;
	((gplayer_imp*)_imp)->GetCoolDownDataForClient(rw);
	
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cooldown_data>::From(_tbuf,rw.data(),rw.size());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::skill_ability_notify(int id, int ability)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::skill_ability_notify>::From(_tbuf,id, ability);
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::personal_market_available()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::personal_market_available>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::breath_data(int breath, int breath_capacity)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::breath_data>::From(_tbuf,breath, breath_capacity);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::stop_dive()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_dive>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::trade_away_item(int buyer,short inv_idx,int type, unsigned int count)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trade_away_item>::From(_tbuf,buyer, inv_idx,type, count);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_enable_fashion_mode(unsigned char is_enable, unsigned char new_model )
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enable_fashion_mode>::From(_tbuf,pPlayer, is_enable, new_model );
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_enable_free_pvp(char is_enable)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enable_free_pvp_mode>::From(_tbuf,is_enable);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_enable_effect(short effect)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enable_effect>::From(_tbuf,pPlayer->ID.id, effect);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::player_disable_effect(short effect)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_disable_effect>::From(_tbuf,pPlayer->ID.id, effect);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::enable_resurrect_state(float exp_reduce)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enable_resurrect_state>::From(_tbuf,exp_reduce);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::object_is_invalid(int id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_is_invalid>::From(_tbuf,id);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::set_cooldown(int idx, int cooldown)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::set_cooldown>::From(_tbuf,idx, cooldown);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::change_team_leader(const XID & old_leader, const XID & new_leader)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_team_leader>::From(_tbuf,old_leader,new_leader);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::kickout_instance(char reason, int timeout)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::kickout_instance>::From(_tbuf,world_manager::GetWorldTag(), reason, timeout);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::begin_cosmetic(unsigned short inv_index)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cosmetic_begin>::From(_tbuf,inv_index);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::end_cosmetic(unsigned short inv_index)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_cosmetic_end>::From(_tbuf,inv_index);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::cosmetic_success(unsigned short crc)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cosmetic_success>::From(_tbuf,pPlayer->ID,crc);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::cast_pos_skill(const A3DVECTOR & pos, const XID &target, int skill,unsigned short time,unsigned char level)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_cast_pos_skill>::From(_tbuf,pPlayer->ID,pos,target,skill,time,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::change_move_seq(unsigned short seq)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::change_move_seq>::From(_tbuf,seq);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::server_config_data()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::server_config_data>::From(_tbuf,globaldata_getmalltimestamp(),globaldata_getmall2timestamp(),globaldata_getmall3timestamp());
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::active_rush_mode(char is_active)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_rush_mode>::From(_tbuf,is_active);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::produce_null(int recipe_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::produce_null>::From(_tbuf,recipe_id);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trashbox_capacity_notify(int where, int cap)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::trashbox_capacity_notify>::From(_tbuf,where, cap);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::enable_double_exp_time(int mode, int end_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::double_exp_time>::From(_tbuf,mode, end_time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::available_double_exp_time()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp *pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::available_double_exp_time>::From(_tbuf,pImp->GetDoubleExpAvailTime());
	send_ls_msg(pPlayer,_tbuf);
}


void
gplayer_dispatcher::active_pvp_combat_state(bool is_active)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::active_pvp_combat_state>::From(_tbuf,pPlayer,is_active);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::duel_recv_request(const XID &target)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_recv_request>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_reject_request(const XID &target,int reason) 
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_reject_request>::From(_tbuf,target,reason);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_prepare(const XID & target, int delay)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_prepare>::From(_tbuf,target,delay);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_cancel(const XID & target) 
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_cancel>::From(_tbuf,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::duel_start(const XID & who) 
{
	_tbuf.clear();
	//????????????

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_start>::From(_tbuf,who);
	send_ls_msg(pPlayer,_tbuf);

	//????????????
	_tbuf.clear();
	CMD::Make<CMD::else_duel_start>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void 
gplayer_dispatcher::duel_stop()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::duel_result(const XID & target, bool is_failed) 
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::duel_result>::From(_tbuf,target, pPlayer->ID, is_failed);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_bind_request(const XID & target, int param_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;	
	CMD::Make<CMD::player_bind_request>::From(_tbuf, param_id ,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_invite(const XID & target, int param_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_invite>::From(_tbuf, param_id ,target);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_request_reply(const XID & target, int param)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_bind_request_reply>::From(_tbuf,target,param,pImp->GetBindParam());
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_invite_reply(const XID & target, int param)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_bind_invite_reply>::From(_tbuf,target,param,pImp->GetBindParam());
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_bind_start(const XID & target)
{
	//???start??????????????????
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::player_bind_start>::From(_tbuf,target, pPlayer->ID, pImp->GetBindParam());
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_bind_stop()
{
	//???start??????????????????????
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_bind_stop>::From(_tbuf,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_mounting(int mount_id, unsigned short mount_color)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_mounting>::From(_tbuf,pPlayer->ID, mount_id, mount_color);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_equip_detail(int cs_index, int cs_sid,int target, const void * data, unsigned int size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_equip_detail>::From(_tbuf,pPlayer->ID, data,size);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}

void 
gplayer_dispatcher::send_inventory_detail(int cs_index, int cs_sid, int target, unsigned int money, unsigned char inv_size, const void * data, unsigned int size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_inventory_detail>::From(_tbuf,pPlayer->ID, money, inv_size,data,size);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}

void gplayer_dispatcher::send_others_property(const void * data, unsigned int size, const void * self_data, unsigned int self_size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_property>::From(_tbuf,data,size,self_data,self_size);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pariah_duration(int remain_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pariah_duration>::From(_tbuf,remain_time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::gain_pet(int index, const void * buf, unsigned int size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_gain_pet>::From(_tbuf,index, buf,size);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::free_pet(int index, int pet_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_free_pet>::From(_tbuf,index, pet_id);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::summon_pet(int index, int pet_tid, int pet_id, int life_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_summon_pet>::From(_tbuf,index, pet_tid,pet_id,life_time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::start_pet_operation(int index, int pet_id,int delay,int operation)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_start_pet_op>::From(_tbuf,index, pet_id,delay,operation);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::end_pet_operation()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_stop_pet_op>::From(_tbuf);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::recall_pet(int index, int pet_id, char reason)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_recall_pet>::From(_tbuf,index, pet_id, reason);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::pet_recv_exp(int index, int pet_id, int exp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_recv_exp>::From(_tbuf,index, pet_id,exp);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::pet_level_up(int index, int pet_id, int level, int cur_exp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_levelup>::From(_tbuf,index, pet_id,level,cur_exp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pet_room_capacity(int cap)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_room_capacity>::From(_tbuf,cap);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::notify_pet_honor(int index,int honor)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_honor_point>::From(_tbuf,index,honor);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::notify_pet_hunger(int index, int hunger)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_hunger_gauge>::From(_tbuf,index,hunger);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::send_pet_room(pet_data ** pData, unsigned int start, unsigned int end)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_pet_room>::From(_tbuf,pData,start, end);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_change_spouse(int id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_change_spouse>::From(_tbuf,pPlayer, id);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::notify_safe_lock(char active, int time, int max_time, bool paswd)
{
	_tbuf.clear();

	CMD::Make<CMD::notify_safe_lock>::From( _tbuf, active, time + g_timer.get_systime(), max_time, paswd);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::enter_battleground(int role,int battle_id,int end_timestamp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_battleground>::From(_tbuf,role,battle_id, end_timestamp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_result(int result)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battle_result>::From(_tbuf,result);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::battle_score(int oscore, int ogoal, int dscore, int dgoal)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::battle_score>::From(_tbuf,oscore,ogoal,dscore,dgoal);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::pet_dead(int index)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_dead>::From(_tbuf,index);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::pet_revive(int index,float hp_factor)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_revive>::From(_tbuf,index,hp_factor);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::pet_hp_notify(int index, float hp_factor, int cur_hp, float mp_factor, int cur_mp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_hp_notify>::From(_tbuf,index,hp_factor,cur_hp,mp_factor,cur_mp);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::pet_ai_state(char aggro_state, char stay_state)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_ai_state>::From(_tbuf,aggro_state,stay_state);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::refine_result(int rst)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::refine_result>::From(_tbuf,rst);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::pet_set_cooldown(int index, int cd_index, int msec)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_set_cooldown>::From(_tbuf,index, cd_index, msec);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_cash(int cash)
{
	_tbuf.clear();

	CMD::Make<CMD::player_cash>::From(_tbuf,cash);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_bind_success(unsigned int index, int id)
{
	_tbuf.clear();

	CMD::Make<CMD::player_bind_success>::From(_tbuf,index, id);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_change_inventory_size(int new_size)
{
	_tbuf.clear();

	CMD::Make<CMD::player_change_inventory_size>::From(_tbuf,new_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_pvp_mode( int level )
{
	_tbuf.clear();

	CMD::Make<CMD::player_pvp_mode>::From(_tbuf,(world_manager::GetWorldParam().pve_mode || level < LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)?0:1);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_wallow_info(int level, int ptime, int light_t, int heavy_t, int reason)
{
	_tbuf.clear();

	CMD::Make<CMD::player_wallow_info>::From(_tbuf, world_manager::AntiWallow()?1:0,level, ptime, light_t , heavy_t, reason);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::mall_item_buy_failed(short index, char reason)
{
	_tbuf.clear();

	CMD::Make<CMD::mall_item_buy_failed>::From(_tbuf, index, reason);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::equip_trashbox_item(int where, unsigned char trash_idx,unsigned char equip_idx) 
{
	_tbuf.clear();

	CMD::Make<CMD::equip_trashbox_item>::From(_tbuf,where,trash_idx,equip_idx);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::security_passwd_checked()
{
	_tbuf.clear();

	CMD::Make<CMD::security_passwd_checked>::From(_tbuf);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::appear_to_spec(int invi_degree)
{
	on_dec_invisible(invi_degree,0);
}

void gplayer_dispatcher::disappear_to_spec(int invi_degree)
{
	on_inc_invisible(0,invi_degree);
}

void gplayer_dispatcher::on_inc_invisible(int prev_invi_degree, int cur_invi_degree)
{
	ASSERT(cur_invi_degree > prev_invi_degree);
	_tbuf.clear();

	gplayer * pPlayer = (gplayer *)_imp->_parent;
	CMD::Make<CMD::object_disappear>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsgToSpec(_imp->_plane,pPiece,_tbuf,cur_invi_degree,prev_invi_degree,pPlayer->team_id,pPlayer->ID.id);

	//???????????????????????
	/*if(pPiece->IsBorder())
	{
		extern_object_manager::SendDisappearMsg<0>(_imp->_plane,pPlayer,pPiece);
	}*/
}

void gplayer_dispatcher::on_dec_invisible(int prev_invi_degree, int cur_invi_degree)
{
	ASSERT(cur_invi_degree < prev_invi_degree);
	_tbuf.clear();

	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_enter_world>::From(_tbuf,pPlayer);
	slice * pPiece = pPlayer->pPiece;
	AutoBroadcastCSMsgToSpec(_imp->_plane,pPiece,_tbuf,prev_invi_degree,cur_invi_degree,pPlayer->team_id,pPlayer->ID.id);
}

void 
gplayer_dispatcher::on_inc_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree)
{
	ASSERT(cur_a_invi_degree > prev_a_invi_degree);	
	begin_transfer();
	//??????????????????????????
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	get_slice_info_visible(pPiece,_nw,_mw,_pw,cur_a_invi_degree,prev_a_invi_degree);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{
		world::off_node_t &node = pPlane->w_off_list[i]; 
		int nx = slice_x + node.x_off; 
		int nz = slice_z + node.z_off; 
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{
			get_slice_info_visible(pNewPiece,_nw,_mw,_pw,cur_a_invi_degree,prev_a_invi_degree);
		}
		else
		{
			get_slice_info_visible(pNewPiece,_nw,_mw,_pw,cur_a_invi_degree,prev_a_invi_degree);
		}
		wrapper_test<MIN_SEND_COUNT>(_pw,_header,S2C::PLAYER_INFO_1_LIST);
		wrapper_test<MIN_SEND_COUNT>(_mw,_header,S2C::MATTER_INFO_LIST);
		wrapper_test<MIN_SEND_COUNT>(_nw,_header,S2C::NPC_INFO_LIST);
	}

	end_transfer();
	return ;
}

void 
gplayer_dispatcher::on_dec_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree)
{
	ASSERT(cur_a_invi_degree < prev_a_invi_degree);	
	begin_transfer();
	//???????????????????????????????
	world * pPlane = _imp->_plane;
	slice * pPiece = _imp->_parent->pPiece;
	int team_id = ((gplayer*)_imp->_parent)->team_id;
	gather_slice_object_visible(pPiece,_leave_list,prev_a_invi_degree,cur_a_invi_degree,team_id);

	int i;
	int total = pPlane->w_far_vision;
	int index = pPlane->GetGrid().GetSliceIndex(pPiece);
	int slice_x,slice_z;
	pPlane->GetGrid().Index2Pos(index,slice_x,slice_z);
	for(i = 0; i <total; i ++)
	{
		world::off_node_t &node = pPlane->w_off_list[i]; 
		int nx = slice_x + node.x_off; 
		int nz = slice_z + node.z_off; 
		if(nx < 0 || nz < 0 || nx >= pPlane->GetGrid().reg_column || nz >= pPlane->GetGrid().reg_row) continue;
		slice * pNewPiece = pPiece + node.idx_off;
		if(i <= pPlane->w_true_vision)
		{
			gather_slice_object_visible(pNewPiece,_leave_list,prev_a_invi_degree,cur_a_invi_degree,team_id);
		}
		else
		{
			gather_slice_object_visible(pNewPiece,_leave_list,prev_a_invi_degree,cur_a_invi_degree,team_id);
		}
	}

	end_transfer();
	return ;
}

void 
gplayer_dispatcher::hp_steal(int hp)
{
	_tbuf.clear();

	CMD::Make<CMD::hp_steal>::From(_tbuf,hp);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::player_dividend(int dividend)
{
	_tbuf.clear();

	CMD::Make<CMD::player_dividend>::From(_tbuf,dividend);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::dividend_mall_item_buy_failed(short index, char reason)
{
	_tbuf.clear();

	CMD::Make<CMD::dividend_mall_item_buy_failed>::From(_tbuf, index, reason);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::multi_exp_info(int last_timestamp, float enhance_factor)
{
	_tbuf.clear();

	CMD::Make<CMD::multi_exp_info>::From(_tbuf, last_timestamp, enhance_factor);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::change_multi_exp_state(char state, int enchance_time, int buffer_time, int impair_time, int activate_times_left)
{
	_tbuf.clear();

	CMD::Make<CMD::change_multi_exp_state>::From(_tbuf, state, enchance_time, buffer_time, impair_time, activate_times_left);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::send_world_life_time(int life_time)
{
	_tbuf.clear();

	CMD::Make<CMD::world_life_time>::From(_tbuf, life_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::wedding_book_success(int type)
{
	_tbuf.clear();

	CMD::Make<CMD::wedding_book_success>::From(_tbuf,type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::calc_network_delay(int timestamp)
{
	_tbuf.clear();

	CMD::Make<CMD::calc_network_delay>::From(_tbuf,timestamp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_knockback(const A3DVECTOR & pos, int time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_knockback>::From(_tbuf,pPlayer->ID.id,pos,time);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf);
}

void
gplayer_dispatcher::summon_plant_pet(int plant_tid, int plant_id, int life_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_summon_plant_pet>::From(_tbuf,plant_tid,plant_id,life_time);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::plant_pet_disappear(int id, char reason)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::plant_pet_disappear>::From(_tbuf,id,reason);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::plant_pet_hp_notify(int id, float hp_factor, int cur_hp, float mp_factor, int cur_mp)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::plant_pet_hp_notify>::From(_tbuf,id,hp_factor,cur_hp,mp_factor,cur_mp);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pet_property(int index, const extend_prop & prop, int r_def_lvl_f, int r_def_lvl_m, int r_chi_point)
{
	_tbuf.clear();
	int reserved = 0;

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_property>::From(_tbuf,index,prop,reserved,r_def_lvl_f,r_def_lvl_m,r_chi_point);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::faction_contrib_notify()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::faction_contrib_notify>::From(_tbuf,pImp->_faction_contrib.consume_contrib,pImp->_faction_contrib.exp_contrib,pImp->_faction_contrib.cumulate_contrib);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::faction_relation_notify()
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	CMD::Make<CMD::faction_relation_notify>::From(_tbuf);
	
	unsigned int size = 0;
	
	size = pImp->_faction_alliance.size();
	_tbuf << size;
	for(std::unordered_map<int,int>::iterator it=pImp->_faction_alliance.begin(); it!=pImp->_faction_alliance.end(); ++it)
		_tbuf << it->first;
	
	size = pImp->_faction_hostile.size();
	_tbuf << size;
	for(std::unordered_map<int,int>::iterator it=pImp->_faction_hostile.begin(); it!=pImp->_faction_hostile.end(); ++it)
		_tbuf << it->first;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::enter_factionfortress(int role_in_war, int factionid, int offense_endtime)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_factionfortress>::From(_tbuf,role_in_war,factionid,offense_endtime);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_equip_disabled(int64_t mask) 
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_equip_disabled>::From(_tbuf, pPlayer->ID, mask);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_spec_item_list(int cs_index, int cs_sid, int target, int type, void * data, unsigned int size)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_spec_item_list>::From(_tbuf,pPlayer->ID,type,data,size);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}

void 
gplayer_dispatcher::send_error_message(int cs_index, int cs_sid, int target, int message)
{
	_tbuf.clear();

	CMD::Make<CMD::error_msg>::From(_tbuf,message);
	send_ls_msg(cs_index,target,cs_sid,_tbuf);
}

void 
gplayer_dispatcher::congregate_request(char type, int sponsor, int timeout)
{
	_tbuf.clear();

	CMD::Make<CMD::congregate_request>::From(_tbuf,type,sponsor,timeout);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::reject_congregate(char type, int id)
{
	_tbuf.clear();

	CMD::Make<CMD::reject_congregate>::From(_tbuf,type,id);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::congregate_start(char type, int time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::congregate_start>::From(_tbuf,type,pPlayer->ID,time);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::cancel_congregate(char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cancel_congregate>::From(_tbuf,type,pPlayer->ID);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::engrave_start(int recipe_id, int use_time)
{
	_tbuf.clear();

	CMD::Make<CMD::engrave_start>::From(_tbuf,recipe_id,use_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);	
}

void
gplayer_dispatcher::engrave_end()
{
	_tbuf.clear();

	CMD::Make<CMD::engrave_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::engrave_result(int addon_num)
{
	_tbuf.clear();

	CMD::Make<CMD::engrave_result>::From(_tbuf,addon_num);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::addonregen_start(int recipe_id, int use_time)
{
	_tbuf.clear();

	CMD::Make<CMD::addonregen_start>::From(_tbuf,recipe_id,use_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::addonregen_end()
{
	_tbuf.clear();

	CMD::Make<CMD::addonregen_end>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::addonregen_result(int addon_num)
{
	_tbuf.clear();

	CMD::Make<CMD::addonregen_result>::From(_tbuf,addon_num);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::invisible_obj_list(gobject ** ppObject, unsigned int count)
{
	_tbuf.clear();

	CMD::Make<CMD::invisible_obj_list>::From(_tbuf,_imp->_parent->ID.id,ppObject,count);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::set_player_limit(int index, char b)
{
	_tbuf.clear();

	CMD::Make<CMD::set_player_limit>::From(_tbuf,index,b);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_teleport(const A3DVECTOR& pos, unsigned short use_time, char mode)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_teleport>::From(_tbuf,pPlayer->ID.id,pos,use_time,mode);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::forbid_be_selected(char b)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::object_forbid_be_selected>::From(_tbuf,pPlayer->ID.id,b);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::send_player_force_data(int cur_force, unsigned int count, const void * data, unsigned int data_size)
{
	_tbuf.clear();

	CMD::Make<CMD::player_force_data>::From(_tbuf,cur_force,count,data,data_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_force_changed(int force)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_force_changed>::From(_tbuf,pPlayer->ID.id,force);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_force_data_update(int force, int repu, int contri)
{
	_tbuf.clear();

	CMD::Make<CMD::player_force_data_update>::From(_tbuf,force,repu,contri);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::send_force_global_data(char data_ready, unsigned int count, const void * data, unsigned int data_size)
{
	_tbuf.clear();

	CMD::Make<CMD::force_global_data>::From(_tbuf,data_ready,count,data,data_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::add_multiobj_effect(int target, char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::add_multiobj_effect>::From(_tbuf,pPlayer->ID.id,target,type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::remove_multiobj_effect(int target, char type)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::remove_multiobj_effect>::From(_tbuf,pPlayer->ID.id,target,type);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::enter_wedding_scene(int groom, int bride)
{
	_tbuf.clear();

	CMD::Make<CMD::enter_wedding_scene>::From(_tbuf,groom,bride);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::produce4_item_info(int stime, item_data & data, unsigned short crc)
{
	_tbuf.clear();

	CMD::Make<CMD::produce4_item_info>::From(_tbuf,stime, data, crc);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::online_award_data(int total_award_time, unsigned int count, const void * data, unsigned int data_size)
{
	_tbuf.clear();

	CMD::Make<CMD::online_award_data>::From(_tbuf,total_award_time,count,data,data_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::toggle_online_award(int type, char activate)
{
	_tbuf.clear();

	CMD::Make<CMD::toggle_online_award>::From(_tbuf,type,activate);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::update_profit_time(char flag, int profit_time, int profit_level)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	char profit_map = 0;
	if(world_manager::ProfitTimeLimit2()) profit_map = 0x02;
	else if(world_manager::ProfitTimeLimit()) profit_map = 0x01;
	CMD::Make<CMD::player_profit_time>::From(_tbuf, flag, (char)profit_map, profit_time, profit_level);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::notify_profit_state(char state)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::set_profittime_state>::From(_tbuf, state);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::enter_nonpenalty_pvp_state(char state)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_nonpenalty_pvp_state>::From(_tbuf, state);
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::self_country_notify(int country_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_country_notify>::From(_tbuf, country_id);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::player_country_changed(int country_id)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_country_changed>::From(_tbuf,pPlayer->ID,country_id);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::enter_countrybattle(int role, int battle_id,int end_time,int offense_country, int defence_country)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::enter_countrybattle>::From(_tbuf,role,battle_id, end_time,offense_country, defence_country);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::countrybattle_result(int result)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_result>::From(_tbuf,result);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::countrybattle_score(int oscore, int ogoal, int dscore, int dgoal)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_score>::From(_tbuf,oscore,ogoal,dscore,dgoal);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::countrybattle_resurrect_rest_times(int times)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_resurrect_rest_times>::From(_tbuf,times);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::countrybattle_became_flag_carrier(char is_carrier)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_became_flag_carrier>::From(_tbuf,is_carrier);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::countrybattle_personal_score(int combat_time, int attend_time, int kill_count, int death_count, int country_kill_count, int country_death_count)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_personal_score>::From(_tbuf,combat_time,attend_time,kill_count,death_count,country_kill_count,country_death_count);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::defense_rune_enabled(char rune_type, char enable)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::defense_rune_enabled>::From(_tbuf,rune_type,enable);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::countrybattle_info(int attacker_count, int defender_count)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::countrybattle_info>::From(_tbuf,attacker_count,defender_count);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::cash_money_exchg_rate(char open, int rate)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::cash_money_exchg_rate>::From(_tbuf,open,rate);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::pet_rebuild_inherit_start(unsigned int index,int use_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_rebuild_inherit_start>::From(_tbuf,index,use_time);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pet_rebuild_inherit_info(int stime,int pet_id,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_rebuild_inherit_info>::From(_tbuf,stime,pet_id,index,r_attack,r_defense,r_hp,r_atk_lvl,r_def_lvl/*,r_pen_lvl_f,r_pen_lvl_m,r_chi_point*/);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pet_rebuild_inherit_end(unsigned int index)
{
	_tbuf.clear();

	CMD::Make<CMD::pet_rebuild_inherit_end>::From(_tbuf,index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::pet_evolution_done(unsigned int index)
{
	_tbuf.clear();

	CMD::Make<CMD::pet_evolution_done>::From(_tbuf,index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::pet_rebuild_nature_start(unsigned int index,int use_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_rebuild_nature_start>::From(_tbuf,index,use_time);
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::pet_rebuild_nature_info(int stime,int pet_id,unsigned int index,int nature)
{
	_tbuf.clear();

	gplayer *pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::pet_rebuild_nature_info>::From(_tbuf,stime,pet_id,index,nature);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::pet_rebuild_nature_end(unsigned int index)
{
	_tbuf.clear();

	CMD::Make<CMD::pet_rebuild_nature_end>::From(_tbuf,index);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::notify_meridian_data(int meridian_level,int lifegate_times,int deathgate_times,int free_refine_times,int paid_refine_times,int continu_login_days,int trigrams_map[3])
{
	_tbuf.clear();

	CMD::Make<CMD::notify_meridian_data>::From(_tbuf,meridian_level,lifegate_times,deathgate_times,free_refine_times,paid_refine_times,continu_login_days,trigrams_map);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::try_refine_meridian_result(int index,int result)
{
	_tbuf.clear();

	CMD::Make<CMD::try_refine_meridian_result>::From(_tbuf,index,result);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::equip_addon_update_notify(unsigned char update_type,unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type)
{
	_tbuf.clear();

	CMD::Make<CMD::equip_addon_update_notify>::From(_tbuf,update_type,equip_idx,equip_socket_idx,old_stone_type,new_stone_type);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::self_king_notify(char is_king, int expire_time)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::self_king_notify>::From(_tbuf, is_king, expire_time);
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::player_king_changed(char is_king)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_king_changed>::From(_tbuf,pPlayer->ID.id,is_king);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::notify_touch_query(int64_t income,int64_t remain,int retcode)
{
	_tbuf.clear();

	CMD::Make<CMD::query_touch_point>::From(_tbuf,income,remain,retcode);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::notify_addup_money(int64_t addupmoney)
{
	_tbuf.clear();

	CMD::Make<CMD::query_addup_money>::From(_tbuf,addupmoney);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::notify_touch_cost(int64_t income,int64_t remain,unsigned int cost,unsigned int index,unsigned int lots,int retcode)
{
	_tbuf.clear();

	CMD::Make<CMD::cost_touch_point>::From(_tbuf,income,remain,cost,index,lots,retcode);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::query_title(int roleid,int count,int ecount, const void * data, unsigned int data_size, const void *edata, unsigned int edata_size)
{
	_tbuf.clear();

	CMD::Make<CMD::query_title>::From(_tbuf,roleid,count,ecount,data,data_size,edata,edata_size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::notify_curr_title_change(int roleid,unsigned short titleid)
{
	_tbuf.clear();

	CMD::Make<CMD::change_title>::From(_tbuf,roleid,titleid);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);

}

void 
gplayer_dispatcher::notify_title_modify(unsigned short titleid,int expiretime,char flag)
{
	_tbuf.clear();

	CMD::Make<CMD::notify_title_modify>::From(_tbuf,titleid,expiretime,flag);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);

}

void 
gplayer_dispatcher::refresh_signin(char type,int lys, int moncal, int uptime, char latesignintimes, char awardedtimes, char awardedtimes2)
{
	_tbuf.clear();

	CMD::Make<CMD::refresh_signin>::From(_tbuf,type,lys, moncal, uptime, latesignintimes, awardedtimes, awardedtimes2);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::notify_giftcard_redeem(int retcode,int cardtype,int parenttype, const char(&cardnumber)[player_giftcard::GIFT_CARDNUMBER_LEN] )
{
	_tbuf.clear();

	CMD::Make<CMD::notify_giftcard_redeem>::From(_tbuf,retcode, cardtype, parenttype, cardnumber);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_reincarnation(unsigned int reincarnation_times)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_reincarnation>::From(_tbuf,pPlayer->ID.id,(int)reincarnation_times);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::activate_reincarnation_tome(char active)
{
	_tbuf.clear();

	CMD::Make<CMD::activate_reincarnation_tome>::From(_tbuf,active);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::enter_trickbattle(int role, int battle_id,int end_time)
{
	_tbuf.clear();

	CMD::Make<CMD::enter_trickbattle>::From(_tbuf,role,battle_id,end_time);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trickbattle_personal_score(int kill, int death, int score, int multi_kill)
{
	_tbuf.clear();

	CMD::Make<CMD::trickbattle_personal_score>::From(_tbuf,kill, death, score, multi_kill);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::trickbattle_chariot_info(int chariot, int energy)
{
	_tbuf.clear();

	CMD::Make<CMD::trickbattle_chariot_info>::From(_tbuf,chariot,energy);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::realm_exp_receive(int exp,int receive_exp, int diary_exp)
{
	_tbuf.clear();

	CMD::Make<CMD::realm_exp>::From(_tbuf,exp,receive_exp,diary_exp);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::realm_level_up(unsigned char level)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::realm_level_up>::From(_tbuf,pPlayer->ID.id,level);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void 
gplayer_dispatcher::player_leadership(int leadership, int inc_leadership)
{
	_tbuf.clear();

	CMD::Make<CMD::player_leadership>::From(_tbuf,leadership,inc_leadership);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_world_contribution(int points_total, int points_rest, char reward_stage, char count, const char * values)
{
	_tbuf.clear();

	CMD::Make<CMD::player_world_contribution>::From(_tbuf,points_total,points_rest,reward_stage,count,values);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::send_scene_service_npc_list(unsigned int count, int * data)
{
	_tbuf.clear();

	CMD::Make<CMD::scene_service_npc_list>::From(_tbuf, count, data);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_screen_effect_notify(int type ,int eid, int state)
{
	_tbuf.clear();

	CMD::Make<CMD::client_screen_effect>::From(_tbuf, type, eid, state);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_combo_skill_prepare(int skillid,int timestamp,int arg1, int arg2, int arg3)
{
	_tbuf.clear();

	CMD::Make<CMD::combo_skill_prepare>::From(_tbuf,skillid,timestamp,arg1,arg2,arg3);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_pray_distance_change(float pd)
{
	_tbuf.clear();

	CMD::Make<CMD::pray_distance_change>::From(_tbuf,pd);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::instance_reenter_notify(int tag, int timeout)
{
	_tbuf.clear();

	CMD::Make<CMD::instance_reenter_notify>::From(_tbuf,tag,timeout);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::generalcard_collection_data(const void * buf, unsigned int size)
{
	_tbuf.clear();

	CMD::Make<CMD::generalcard_collection_data>::From(_tbuf,buf,size);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::add_generalcard_collection(unsigned int collection_idx)
{
	_tbuf.clear();

	CMD::Make<CMD::add_generalcard_collection>::From(_tbuf,collection_idx);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void
gplayer_dispatcher::player_active_combat(bool is_active)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_active_combat>::From(_tbuf,pPlayer,is_active);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,pPlayer->ID.id);
}

void
gplayer_dispatcher::randommap_order_init(int row,int col,const int* room_src)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::random_map_order>::From(_tbuf,world_manager::GetWorldTag(),row,col,room_src);
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::random_mall_shopping_result(int eid,int opt,int res,int itemid,int price,bool flag)
{
	_tbuf.clear();

	CMD::Make<CMD::random_mall_shopping_result>::From(_tbuf,eid,opt,res,itemid,price,flag);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void 
gplayer_dispatcher::player_mafia_pvp_mask_notify(unsigned char mafia_pvp_mask)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::player_mafia_pvp_mask_notify>::From(_tbuf,pPlayer->ID.id,mafia_pvp_mask);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::equip_can_inherit_addons(int equip_id, unsigned char inv_idx, unsigned char addon_num, int addon_id_list[])
{    
    _tbuf.clear();

	CMD::Make<CMD::equip_can_inherit_addons>::From(_tbuf, equip_id, inv_idx, addon_num, addon_id_list);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::astrolabe_info_notify(unsigned char level, int exp)
{
    _tbuf.clear();

 	CMD::Make<CMD::astrolabe_info_notify>::From(_tbuf, level, exp);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::astrolabe_operate_result(int opt, int ret, int a0, int a1, int a2) 
{
    _tbuf.clear();

 	CMD::Make<CMD::astrolabe_operate_result>::From(_tbuf, opt, ret, a0, a1, a2);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::property_score_result(int fighting_score, int viability_score, int client_data)
{
    _tbuf.clear();

    CMD::Make<CMD::property_score_result>::From(_tbuf, fighting_score, viability_score, client_data);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::lookup_enemy_result(int rid, int worldtag, const A3DVECTOR& pos)
{
    _tbuf.clear();

    CMD::Make<CMD::lookup_enemy_result>::From(_tbuf, rid, worldtag, pos);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::solo_challenge_award_info_notify(int max_stage_level, int total_time, int total_score, int cur_score, int last_success_stage_level, int last_success_stage_cost_time, int draw_award_times,int have_draw_award_times, abase::vector<struct playersolochallenge::player_solo_challenge_award>& award_info)
{
	_tbuf.clear();

	CMD::Make<CMD::solo_challenge_award_info_notify>::From(_tbuf, max_stage_level, total_time, total_score, cur_score, last_success_stage_level, last_success_stage_cost_time, draw_award_times, award_info.size());
	for(unsigned int i = 0; i < award_info.size(); i++)
	{
		CMD::Make<CMD::solo_challenge_award_info_notify>::Add(_tbuf, award_info[i].item_id, award_info[i].item_count);
	}
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::solo_challenge_operate_result(int opttype, int retcode, int arg0, int arg1, int arg2)
{	
    _tbuf.clear();

 	CMD::Make<CMD::solo_challenge_operate_result>::From(_tbuf, opttype, retcode, arg0, arg1, arg2);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::solo_challenge_challenging_state_notify(int climbed_layer, unsigned char notify_type)
{	
    _tbuf.clear();

 	CMD::Make<CMD::solo_challenge_challenging_state_notify>::From(_tbuf, climbed_layer, notify_type);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::solo_challenge_buff_info_notify(int *buff_index, int *buff_num, int count, int cur_score)
{
    _tbuf.clear();

 	CMD::Make<CMD::solo_challenge_buff_info_notify>::From(_tbuf, count, cur_score);
	for(int i = 0; i < count; i++)
	{
		CMD::Make<CMD::solo_challenge_buff_info_notify>::Add(_tbuf, buff_index[i], buff_num[i]);
	}
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_player_faction_info(int player_faction, int domain_id)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_player_faction_info>::From(_tbuf, player_faction, domain_id);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_resource_point_info(int attacker_resource_point, int defender_resource_point)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_resource_point_info>::From(_tbuf, attacker_resource_point, defender_resource_point);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_player_count_info(int attend_attacker_player_count, int attend_defender_player_count)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_player_count_info>::From(_tbuf, attend_attacker_player_count, attend_defender_player_count);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_resource_point_state_info(int index, int cur_degree)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_resource_point_state_info>::From(_tbuf, index, cur_degree);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::mnfaction_resource_tower_state_info(int num, MNFactionStateInfo &mnfaction_state_info)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_resource_tower_state_info>::From(_tbuf, num);
	for(int i = 0; i < num; i++)
	{
		CMD::Make<CMD::mnfaction_resource_tower_state_info>::Add(_tbuf, mnfaction_state_info._index, mnfaction_state_info._own_faction, mnfaction_state_info._state, mnfaction_state_info._time_out);
	}
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::mnfaction_switch_tower_state_info(int num, MNFactionStateInfo &mnfaction_state_info)
{  
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_switch_tower_state_info>::From(_tbuf, num);
	for(int i = 0; i < num; i++)
	{
		CMD::Make<CMD::mnfaction_switch_tower_state_info>::Add(_tbuf, mnfaction_state_info._index, mnfaction_state_info._own_faction, mnfaction_state_info._state, mnfaction_state_info._time_out);
	}
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::mnfaction_transmit_pos_state_info(int num, MNFactionStateInfo &mnfaction_state_info)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_transmit_pos_state_info>::From(_tbuf, num);
	for(int i = 0; i < num; i++)
	{
		CMD::Make<CMD::mnfaction_transmit_pos_state_info>::Add(_tbuf, mnfaction_state_info._index, mnfaction_state_info._own_faction, mnfaction_state_info._state, mnfaction_state_info._time_out);
	}
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::mnfaction_result(int result)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_result>::From(_tbuf, result);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_battle_ground_have_start_time(int battle_ground_have_start_time)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_battle_ground_have_start_time>::From(_tbuf, battle_ground_have_start_time);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_faction_killed_player_num(int attacker_killed_player_count, int defender_killed_player_count)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_faction_killed_player_num>::From(_tbuf, attacker_killed_player_count, defender_killed_player_count);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::mnfaction_shout_at_the_client(int type, int args)
{
    _tbuf.clear();

 	CMD::Make<CMD::mnfaction_shout_at_the_client>::From(_tbuf, type, args);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fix_position_transmit_add_position(int index, int world_tag, A3DVECTOR &pos, unsigned int position_length, const char *position_name)
{
	_tbuf.clear();

	CMD::Make<CMD::fix_position_transmit_add_position>::From(_tbuf, index, world_tag, pos.x, pos.y, pos.z, position_length, position_name);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fix_position_transmit_delete_position(int index)
{
	_tbuf.clear();

	CMD::Make<CMD::fix_position_transmit_delete_position>::From(_tbuf, index);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fix_position_transmit_rename(int index, unsigned int position_length, char *position_name)
{
	_tbuf.clear();

	CMD::Make<CMD::fix_position_transmit_rename>::From(_tbuf, index, position_length, position_name);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fix_position_energy_info(char is_login, int cur_energy)
{
	_tbuf.clear();

	CMD::Make<CMD::fix_position_energy_info>::From(_tbuf, is_login, cur_energy);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::fix_position_all_info(fix_position_transmit_info *info)
{
    _tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp*)_imp;
	int count = pImp->GetFixPositionCount();
 	CMD::Make<CMD::fix_position_all_info>::From(_tbuf, count);
	for(int i = 0; i < FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT; i++)
	{
		if(info[i].index != -1)
		{
			CMD::Make<CMD::fix_position_all_info>::Add(_tbuf, info[i].index, info[i].world_tag, info[i].pos.x, info[i].pos.y, info[i].pos.z, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH, info[i].position_name);
		}
	}
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::cash_vip_mall_item_buy_result(char result, short index, char reason)
{
	_tbuf.clear();

	CMD::Make<CMD::cash_vip_mall_item_buy_result>::From(_tbuf, result, index, reason);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::cash_vip_info_notify(int level, int score)
{
	_tbuf.clear();

	if(LuaManager::GetInstance()->GetConfig()->vip_enable)
		CMD::Make<CMD::cash_vip_info_notify>::From(_tbuf, level, score);
	else
		CMD::Make<CMD::cash_vip_info_notify>::From(_tbuf, 0, 0);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::purchase_limit_all_info_notify()
{
    _tbuf.clear();

	gplayer_imp * pImp = (gplayer_imp*)_imp;
	int count = pImp->GetPurchaseLimit().GetPurchaseLimitItemCount();
	CMD::Make<CMD::purchase_limit_info_notify>::From(_tbuf, count, 1);
	pImp->GetPurchaseLimit().SaveAllMap(_tbuf);
 	gplayer* pPlayer = (gplayer*)_imp->_parent;
 	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::purchase_limit_info_notify(int limit_type, int item_id, int have_purchase_count)
{
	_tbuf.clear();

	CMD::Make<CMD::purchase_limit_info_notify>::From(_tbuf, 1, 1);
	CMD::Make<CMD::purchase_limit_info_notify>::Add(_tbuf, limit_type, item_id, have_purchase_count);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::cash_resurrect_info(int cash_need, int cash_left)
{
    _tbuf.clear();

    CMD::Make<CMD::cash_resurrect_info>::From(_tbuf, cash_need, cash_left);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::body_scale_notify(int mode, float scale)
{
    _tbuf.clear();

	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::body_scale_notify>::From(_tbuf, mode, pPlayer->ID.id, scale);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}


void gplayer_dispatcher::filter_soul_info(int soul)
{
    _tbuf.clear();

	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::filter_soul_info>::From(_tbuf, pPlayer->ID.id, soul);
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::home_task_info( void *buf, unsigned int size, int task_trigger_times, int task_refresh_times)
{
    _tbuf.clear();

	gplayer* pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::home_task_info>::From(_tbuf,buf,size,task_trigger_times,task_refresh_times);
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_lotery_info( unsigned int manager, unsigned int score, unsigned int count, unsigned int * item )
{
    _tbuf.clear();

    CMD::Make<CMD::get_lotery_info>::From(_tbuf, manager, score, count, item );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::lottery_reward_info( unsigned int unk /* 14 */, unsigned int score, unsigned int count, unsigned int itemid )
{
    _tbuf.clear();

    CMD::Make<CMD::lottery_reward_info>::From(_tbuf, unk, score, count, itemid );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_treasure_info( unsigned char manager, unsigned int score, unsigned int free_count, unsigned int lot_count, unsigned int box_count, void * _lot, void * _box )
{
    _tbuf.clear();

    CMD::Make<CMD::get_treasure_info>::From(_tbuf, manager, score, free_count, lot_count, box_count, _lot, _box );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_lotery_items( unsigned int count, unsigned int * item )
{
    _tbuf.clear();

    CMD::Make<CMD::get_lotery_items>::From(_tbuf, count, item );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

/*
void gplayer_dispatcher::glyph_info( unsigned int count, void * item )
{
    _tbuf.clear();

    CMD::Make<CMD::glyph_info>::From(_tbuf, count, item );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}
*/

void gplayer_dispatcher::glyph_slot_info( int reserve1, int count, int pvp_point )
{
    _tbuf.clear();

    CMD::Make<CMD::glyph_slot_info>::From(_tbuf, reserve1, count, pvp_point );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_glyph_skill( int mode, int pos, int skill, int item ) 
{
	_tbuf.clear();

	CMD::Make<CMD::glyph_manager_re>::From(_tbuf, mode, 0, pos, skill, item );
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_glyph_upgrade( int mode, int result, int pos,  int new_item, int old_item ) 
{
	_tbuf.clear();

	CMD::Make<CMD::glyph_manager_re>::From(_tbuf, mode, result, pos, new_item, old_item );
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::engrave_new_info(int addon_count, unsigned int * engraves) //Nova Gravura
{
	_tbuf.clear();

	CMD::Make<CMD::engrave_new_info>::From(_tbuf,addon_count, engraves);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::refine_bible_result(int rst)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::refine_bible_result>::From(_tbuf,rst);
	send_ls_msg(pPlayer,_tbuf);
}

void gplayer_dispatcher::get_lib_items( unsigned int count, unsigned int * item ) // Lib Itens
{
    _tbuf.clear();

    CMD::Make<CMD::get_lib_items>::From(_tbuf, count, item );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::result_lib_produce()
{
	_tbuf.clear();

	CMD::Make<CMD::result_lib_produce>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::transfer_refine_bible(int rst1, int rst2, int item_pos)
{
    _tbuf.clear();

    CMD::Make<CMD::transfer_refine_bible>::From(_tbuf, rst1, rst2, item_pos);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_storage_points(short rst1, short rst2)
{
    _tbuf.clear();

    CMD::Make<CMD::get_storage_points>::From(_tbuf, rst1, rst2);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_finish_notify(int rst1, int rst2)
{
    _tbuf.clear();

    CMD::Make<CMD::get_finish_notify>::From(_tbuf, rst1, rst2);
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_world_speak_info( char enabled, char enabled2, char kid, int skills_count, int * skills )
{
    _tbuf.clear();

    CMD::Make<CMD::player_world_speak_info>::From(_tbuf, enabled, enabled2, kid, skills_count, skills );
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::get_server_time( unsigned int time ) 
{
    _tbuf.clear();

    CMD::Make<CMD::get_server_time>::From(_tbuf, time );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::get_weapon_vstate( char state ) 
{
    _tbuf.clear();
	
	gplayer* pPlayer = (gplayer*)_imp->_parent;
    CMD::Make<CMD::get_weapon_vstate>::From(_tbuf, pPlayer->ID.id, state );
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::get_shield_energy( short shgen, short shmax ) 
{
    _tbuf.clear();

    CMD::Make<CMD::get_shield_energy>::From(_tbuf, shgen, shmax );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::material_refine_result(int reserve, int rst)
{
	_tbuf.clear();

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	CMD::Make<CMD::material_refine_result>::From(_tbuf,reserve,rst);
	send_ls_msg(pPlayer,_tbuf);
}

/*166+*/

void gplayer_dispatcher::pet_skill_data( unsigned int count, int index,  unsigned int * skills )
{
    _tbuf.clear();

    CMD::Make<CMD::pet_skill_data>::From(_tbuf, count, index, skills );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::pet_skin_info( int index,  int skin_pet_id )
{
    _tbuf.clear();

    CMD::Make<CMD::pet_skin_info>::From(_tbuf, index, skin_pet_id );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::pet_skin_storage( unsigned int count,  unsigned int * values )
{
    _tbuf.clear();

    CMD::Make<CMD::pet_skin_storage>::From(_tbuf, count, values );
    gplayer* pPlayer = (gplayer*)_imp->_parent;
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_carrier_up(int pos, int reserve1, unsigned int npcid)
{
	_tbuf.clear();

	CMD::Make<CMD::player_carrier_up>::From(_tbuf, pos, reserve1, npcid);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_carrier_down(int pos, unsigned int npcid)
{
	_tbuf.clear();

	CMD::Make<CMD::player_carrier_down>::From(_tbuf, pos, npcid);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::player_carrier_self_up(int pos, int count, unsigned int npcid, int res, float speed_walk, float speed_run, float speed_swim, float speed_fly)
{
	_tbuf.clear();

	CMD::Make<CMD::player_carrier_self_up>::From(_tbuf, pos, count, npcid, res, speed_walk, speed_run, speed_swim, speed_fly);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::carrier_sync_info(unsigned int npcid, int reserve1, int reserve2)
{
	_tbuf.clear();

	CMD::Make<CMD::carrier_sync_info>::From(_tbuf, npcid, reserve1, reserve2);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
    send_ls_msg(pPlayer, _tbuf);
}

/*156+*/
void gplayer_dispatcher::get_imperial_spirit(int enabled, unsigned int expired_time)
{
	_tbuf.clear();

	CMD::Make<CMD::get_imperial_spirit>::From(_tbuf, enabled, expired_time);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


void gplayer_dispatcher::carrier_prop(unsigned int npcid, int reserve1, unsigned int carrier_data)
{
	_tbuf.clear();

	CMD::Make<CMD::carrier_prop>::From(_tbuf, npcid, reserve1, carrier_data);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
    send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::carrier_skill_cooldown(unsigned int skill_id, int cooldown)
{
	_tbuf.clear();

	CMD::Make<CMD::carrier_skill_cooldown>::From(_tbuf, skill_id, cooldown);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_repository_info(unsigned int roleid, unsigned int storage_itens, int itens_storage[], int used_itens, int itens_active[])
{    
    _tbuf.clear();

	CMD::Make<CMD::player_repository_info>::From(_tbuf, roleid, storage_itens, itens_storage, used_itens, itens_active);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::player_repository_storage(unsigned int count_values, int itens[])
{    
    _tbuf.clear();

	CMD::Make<CMD::player_repository_storage>::From(_tbuf, count_values, itens);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

/*171+*/
void gplayer_dispatcher::dynamic_genesis_level(unsigned int count, const void * skills_array)
{    
    _tbuf.clear();

	CMD::Make<CMD::dynamic_genesis_level>::From(_tbuf, count, skills_array);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}


// Novas cargas do mercenário
void gplayer_dispatcher::get_charge_merc(char charges)
{    
    _tbuf.clear();

	CMD::Make<CMD::get_charge_merc>::From(_tbuf, charges);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher:: skill_glyph_info( int type, unsigned int count, unsigned int * values )
{
	_tbuf.clear();

	CMD::Make<CMD::skill_glyph_info>::From(_tbuf, type, count, values);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

// Anedota
void gplayer_dispatcher::get_anecdote_info(short uTask1, short uTask2, short uTask3, short uTask4, short uTask5, short uTask6, short uTask7, short uTask8, short uTask9, short uTask10, short uTask11, short uTask12, short uTask13, short uTask14, short uTask15, short uTask16)
{    
	_tbuf.clear();

	CMD::Make<CMD::get_anecdote_info>::From(_tbuf, uTask1, uTask2, uTask3, uTask4, uTask5, uTask6, uTask7, uTask8, uTask9, uTask10, uTask11, uTask12, uTask13, uTask14, uTask15, uTask16);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

// Segment
void gplayer_dispatcher::skill_cooldown_limit(int skill_id , unsigned int skill_last_use_time , unsigned int skill_cooling_sec , unsigned short count , unsigned short max_count)
{    
	_tbuf.clear();

	CMD::Make<CMD::skill_cooldown_limit>::From(_tbuf, skill_id , skill_last_use_time , skill_cooling_sec , count , max_count);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

// new money 172+
void gplayer_dispatcher::pickup_gold(unsigned int value)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_money>::From(_tbuf,value,0);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(abase::Rand(0,0) == 1)
	{
		std::pair<int,int> pp(pPlayer->ID.id,pPlayer->cs_sid);
		GMSV::MultiSendClientData(pPlayer->cs_index,&pp,&pp,_tbuf.data(),_tbuf.size(),0);

	}
	else
	{
		send_ls_msg(pPlayer, _tbuf);
	}

}

void gplayer_dispatcher::pickup_silver(unsigned int value)
{
	_tbuf.clear();

	CMD::Make<CMD::player_pickup_money>::From(_tbuf,value,1);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	if(abase::Rand(0,0) == 1)
	{
		std::pair<int,int> pp(pPlayer->ID.id,pPlayer->cs_sid);
		GMSV::MultiSendClientData(pPlayer->cs_index,&pp,&pp,_tbuf.data(),_tbuf.size(),0);

	}
	else
	{
		send_ls_msg(pPlayer, _tbuf);
	}

}

/*156+*/
void gplayer_dispatcher::arena_team_invite(int inviting_id, int team_type)
{
	_tbuf.clear();

	CMD::Make<CMD::arena_team_invite>::From(_tbuf, inviting_id, team_type);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::arena_battle_group_info(unsigned int time_rest, int red_team_count_rest, int blue_team_count_rest, int team_count_members_red, int team_count_members_blue, int members_red[], int members_blue[])
{
	_tbuf.clear();
	CMD::Make<CMD::arena_battle_group_info>::From(_tbuf, time_rest, red_team_count_rest, blue_team_count_rest, team_count_members_red, team_count_members_blue, members_red, members_blue);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::arena_player_faction_info(int team_id, int type, int time_rest)
{
	_tbuf.clear();

	CMD::Make<CMD::arena_player_faction_info>::From(_tbuf, team_id, type, time_rest);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

/*170+ Códice*/
void gplayer_dispatcher::codex_fashion_activate(int roleid, int mode_classic, unsigned int size, const void * fashion_activate)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_fashion_activate>::From(_tbuf, roleid, mode_classic, size, fashion_activate);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	//send_ls_msg(pPlayer, _tbuf);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::codex_fashion_storage(int skins_count, unsigned int size,  const void * skins, int fashion_points)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_fashion_storage>::From(_tbuf, skins_count, size, skins, fashion_points);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_fashion_dye(int mode, int count_fashions, unsigned int size, const void * fashion_dye)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_fashion_dye>::From(_tbuf, mode, count_fashions, size, fashion_dye);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_reward_title(int type, int pos)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_reward_title>::From(_tbuf, type, pos);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_reward_first(int type, int reward_idx, int result)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_reward_first>::From(_tbuf, type, reward_idx, result);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_pet_consum_info(int pet_egg_id, int res, short res2, int mount_points)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_pet_consum_info>::From(_tbuf, pet_egg_id, res, res2, mount_points);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_fly_consum_info(int fly_id, char res, int speed, int fly_points)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_fly_consum_info>::From(_tbuf, fly_id, res, speed, fly_points);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_improve_info(int fly_id, char apprimored_level, int max_time_speed)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_improve_info>::From(_tbuf, fly_id, apprimored_level, max_time_speed);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}
void gplayer_dispatcher::codex_fly_equip_info(short index_equip, char index_inv, char where_equip, int pos_codex_inv, int fly_item_id, int speed, char apprimored_level)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_fly_equip_info>::From(_tbuf, index_equip, index_inv, where_equip, pos_codex_inv, fly_item_id, speed, apprimored_level);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

void gplayer_dispatcher::codex_rename_pet_info(int pet_id ,const char * name, short name_len)
{
	_tbuf.clear();

	CMD::Make<CMD::codex_rename_pet_info>::From(_tbuf, pet_id, name, name_len);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);

}

/*170+ Bebe Celestial*/
void 
gplayer_dispatcher::kid_course_change (char old_slot, char new_slot)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_change>::From(_tbuf, old_slot, new_slot);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}
void 
gplayer_dispatcher::kid_course_remove (char old_slot)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_remove>::From(_tbuf, old_slot);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_name_awakening (short name_len, const char * name)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_name_awakening>::From(_tbuf, name_len, name);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_course_info (unsigned int * course_info, int count)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_info>::From(_tbuf, course_info, count);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_course_perc (char level, int reserved)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_perc>::From(_tbuf, level, reserved);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_awakening_points(int points)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_awakening_points>::From(_tbuf, points);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void 
gplayer_dispatcher::kid_created_info_dialog()
{
	_tbuf.clear();

	CMD::Make<CMD::kid_created_info_dialog>::From(_tbuf);
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_course_switch (char new_slot, char old_slot1, char old_slot2)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_switch>::From(_tbuf, new_slot, old_slot1, old_slot2);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_course_insert (char old_slot, char new_slot)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_course_insert>::From(_tbuf, old_slot, new_slot);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_awakening_info (unsigned int size, const void * kid_awakening_info)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_awakening_info>::From(_tbuf, size, kid_awakening_info);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_awakening_cash_info (int awakening_cash, int awakening_potential)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_awakening_cash_info>::From(_tbuf, awakening_cash, awakening_potential);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_celestial_info ( unsigned int size, const void * kid_info)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_celestial_info>::From(_tbuf, size, kid_info);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_celestial_transformation (int shape, int roleid, int reserve, int reserve2)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_celestial_transformation>::From(_tbuf, shape, roleid, reserve, reserve2);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void
gplayer_dispatcher::kid_celestial_awakening (int type, int reserve)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_celestial_awakening>::From(_tbuf, type, reserve);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_award_addon(unsigned int size, const void * info)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_award_addon>::From(_tbuf, size, info);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void
gplayer_dispatcher::kid_active_info (int active_slot, int reserved)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_active_info>::From(_tbuf, active_slot, reserved);
	gplayer* pPlayer = (gplayer*)_imp->_parent;		
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::reward_interface_notify(unsigned int mode, unsigned int value_index, int count)
{
	_tbuf.clear();

	CMD::Make<CMD::reward_interface_notify>::From(_tbuf, mode, value_index, count);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	
	//send_ls_msg(pPlayer, _tbuf);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::activity_event_spend_cash(unsigned int roleid, unsigned int cash)
{
	_tbuf.clear();

	CMD::Make<CMD::activity_event_spend_cash>::From(_tbuf, roleid, cash);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::activity_event_shop(int count, char unk6, unsigned int * values)
{
	_tbuf.clear();

	CMD::Make<CMD::activity_event_shop>::From(_tbuf, count, unk6, values);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::activity_event_buy_bar(unsigned int time_end, unsigned int silver_used, int awards_pos)
{
	_tbuf.clear();

	CMD::Make<CMD::activity_event_buy_bar>::From(_tbuf, time_end, silver_used, awards_pos);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

// Memorial Celestial

void gplayer_dispatcher::celestial_memorial_info(bool type, unsigned int size, const void * info,unsigned int size2, const void * info2, unsigned int size3, const void * info3)
{
	_tbuf.clear();

	CMD::Make<CMD::celestial_memorial_info>::From(_tbuf, type, size, info, size2, info2, size3, info3);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::celestial_memorial_lottery( int count, unsigned int size, const void * lotterys)
{
	_tbuf.clear();

	CMD::Make<CMD::celestial_memorial_lottery>::From(_tbuf, count, size, lotterys);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::celestial_memorial_lottery_count( int perg1, int perg2, int perg3)
{
	_tbuf.clear();

	CMD::Make<CMD::celestial_memorial_lottery_count>::From(_tbuf, perg1, perg2, perg3);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::armor_info_notify(unsigned int reserved, unsigned int count, int reserved3, int reserved4, int inv_slot) 
{
	_tbuf.clear();

	CMD::Make<CMD::armor_info_notify>::From(_tbuf, reserved, count, reserved3, reserved4, inv_slot);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

void gplayer_dispatcher::armor_info_purification(int count, unsigned int * values)
{
	_tbuf.clear();

	CMD::Make<CMD::armor_info_purification>::From(_tbuf, count, values);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}

/*176+*/
void gplayer_dispatcher::portatil_picture_info(int roleid, int res, int res2, int idx, int res3)
{
	_tbuf.clear();

	CMD::Make<CMD::portatil_picture_info>::From(_tbuf, roleid, res, res2, idx, res3);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	//send_ls_msg(pPlayer, _tbuf);
	AutoBroadcastCSMsg(_imp->_plane,pPlayer->pPiece,_tbuf,-1);
}

void gplayer_dispatcher::portatil_picture_storage(int count, int * storage)
{
	_tbuf.clear();

	CMD::Make<CMD::portatil_picture_storage>::From(_tbuf, count, storage);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}
void gplayer_dispatcher::kid_system_points_notify(int points)
{
	_tbuf.clear();

	CMD::Make<CMD::kid_system_points_notify>::From(_tbuf, points);
	gplayer* pPlayer = (gplayer*)_imp->_parent;
	send_ls_msg(pPlayer, _tbuf);
}
