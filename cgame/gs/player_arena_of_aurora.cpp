#include "clstab.h"
#include "faction.h"
#include "world.h"
#include "player_imp.h"
#include "player_arena_of_aurora.h"
#include "instance/arenaofaurora_ctrl.h"
#include "invisible_filter.h"

DEFINE_SUBSTANCE(gplayer_arenaofaurora,gplayer_imp,CLS_ARENAOFAURORA_PLAYER_IMP)

void gplayer_arenaofaurora::OnHeartbeat(size_t tick)
{
	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;

	if (pCtrl)
	{
		if (is_arena_zombie)
		{
			if (pCtrl->_battle_timer >= wait_resurrect_timer)
			{
				SendTo<0>(GM_MSG_GM_RESURRECT, _parent->ID, 0);

				// Renasce automaticamente quando morto dentro do tempo
				if (_parent->b_zombie)
				{
					_parent->b_zombie = false;
					_basic.hp = (int)(_cur_prop.max_hp * 0.1f + 0.5f);
					_basic.mp = (int)(_cur_prop.max_mp * 0.1f + 0.5f);

					SetRefreshState();
					_enemy_list.clear();

					((gplayer_controller*)_commander)->OnResurrect();

					ClearNextSession();

					_runner->resurrect(0);
				}
			}
		}

		if (pCtrl->_battle_timer < 0)
		{
			int teamType = pCtrl->GetTeamType(_parent->ID.id);

			A3DVECTOR trans_pos(0, 0, 0);

			switch (teamType)
			{
			case ARENA_TEAM_RED:
				trans_pos = pCtrl->_trans_pos_1;
				break;
			case ARENA_TEAM_BLUE:
				trans_pos = pCtrl->_trans_pos_2;
				break;
			}

			if (trans_pos.squared_distance(_parent->pos) >= MAX_AREA_DISTANCE*MAX_AREA_DISTANCE)
			{
				gplayer_imp::LongJump(trans_pos);
			}
		}
	}

	gplayer_imp::OnHeartbeat(tick);

}

// Filtros de ataque, para não atacar membros
int gplayer_arenaofaurora::MessageHandler(world* pPlane, const MSG& msg)
{
	switch (msg.message)
	{

	case GM_MSG_ATTACK:
	{
		attack_msg ack_msg = *(attack_msg*)msg.content;
		_filters.EF_TransRecvAttack(msg.source, ack_msg);
		XID attacker = ack_msg.ainfo.attacker;

		if (attacker.id == _parent->ID.id) return 0;
		if (!(ack_msg.target_faction & (GetFaction())))
		{
			return 0;
		}
		ack_msg.target_faction = 0xFFFFFFFF;
		ack_msg.is_invader = false;

		((gplayer_controller*)_commander)->TrySelect(msg.source);
		HandleAttackMsg(pPlane, msg, &ack_msg);
		return 0;
	}
	break;

	case GM_MSG_ENCHANT:
	{
		enchant_msg ech_msg = *(enchant_msg*)msg.content;
		_filters.EF_TransRecvEnchant(msg.source, ech_msg);
		if (!ech_msg.helpful)
		{
			XID attacker = ech_msg.ainfo.attacker;
			if (attacker.id == _parent->ID.id) return 0;
			if (!(ech_msg.target_faction & (GetFaction())))
			{
				return 0;
			}

			ech_msg.target_faction = 0xFFFFFFFF;
			((gplayer_controller*)_commander)->TrySelect(msg.source);
		}
		else
		{
			XID attacker = ech_msg.ainfo.attacker;
			if (attacker.id != _parent->ID.id)
			{
				if (!(ech_msg.attacker_faction & defense_faction))
				{
					return 0;
				}
				if (ech_msg.helpful == 1)
				{
					if ((_refuse_bless & C2S::REFUSE_NON_TEAMMATE_BLESS)
						&& (!IsArenaMember(attacker))) return 0;
				}
				else if (ech_msg.helpful == 2)
				{
					if (_refuse_bless & C2S::REFUSE_NEUTRAL_BLESS) return 0;
				}
			}
		}
		ech_msg.is_invader = false;
		HandleEnchantMsg(pPlane, msg, &ech_msg);
		return 0;
	}

	case GM_MSG_TEAM_INVITE:
	case GM_MSG_TEAM_AGREE_INVITE:
	case GM_MSG_TEAM_REJECT_INVITE:
		return 0;
	}

	return gplayer_imp::MessageHandler(pPlane, msg);
}

int gplayer_arenaofaurora::ZombieMessageHandler(world* pPlane, const MSG& msg)
{
	switch (msg.message)
	{
		default:
			return gplayer_imp::ZombieMessageHandler(pPlane, msg);
	}

	return 0;
}

void gplayer_arenaofaurora::FillAttackMsg(const XID& target, attack_msg& attack, int dec_arrow)
{
	gplayer_imp::FillAttackMsg(target, attack, dec_arrow);
	attack.force_attack = 0;
	attack.attacker_faction |= defense_faction;
	attack.target_faction |= attack_faction;
}

void gplayer_arenaofaurora::FillEnchantMsg(const XID& target, enchant_msg& enchant)
{
	gplayer_imp::FillEnchantMsg(target, enchant);
	enchant.force_attack = 0;
	enchant.attacker_faction |= defense_faction;
	enchant.target_faction |= attack_faction;
}


void gplayer_arenaofaurora::SendDataToSubscibeList()
{
	gplayer_imp::SendDataToSubscibeList();
}

void gplayer_arenaofaurora::SendTeamDataToSubscibeList()
{
	gplayer_imp::SendTeamDataToSubscibeList();

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (!pCtrl)
		return;

	if (_team_visible_state_flag || GetRefreshState())
	{
		spin_autolock keeper(pCtrl->_user_list_lock);

		if (pCtrl->_all_list.size())
		{

			// Buff de aliados
			if (_team_visible_state_flag)
			{
				#pragma pack(push, 1)
				unsigned short vts_id[32];
				struct 
				{
					int time;
					int num;
				}	vts_time[32];
				#pragma pack(pop)
				
				unsigned int iSize = _visible_team_state.size();
				packet_wrapper h1(64 + iSize*sizeof(vts_time[0]));
				using namespace S2C;

				for (int i = 0; i < iSize && i < 32; i++ )
				{
					vts_id[i] = _visible_team_state[i].id;
					vts_time[i].time = _visible_team_state[i].time;
					vts_time[i].num = i;
				}
				
				CMD::Make<CMD::object_state_notify>::From(h1,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(),(int*)vts_time);
				multi_send_ls_msg(pCtrl->_all_list, h1.data(), h1.size(), 0);

				CMD::Make<CMD::arena_battle_object_state_notify>::From(h1,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(),(int*)vts_time);
				multi_send_ls_msg(pCtrl->_all_list, h1.data(), h1.size(), 0);
			}

			// Buff de inimigos
			if (_team_visible_state_flag)
			{
				#pragma pack(push, 1)
				unsigned short vts_id[32];
				struct 
				{
					int time;
					int num;
				}	vts_time[32];
				#pragma pack(pop)
				
				unsigned int iSize = _visible_team_state.size();
				packet_wrapper h2(64 + iSize*sizeof(vts_time[0]));
				using namespace S2C;

				for (int i = 0; i < iSize && i < 32; i++ )
				{
					vts_id[i] = _visible_team_state[i].id;
					vts_time[i].time = _visible_team_state[i].time;
					vts_time[i].num = i;
				}

				CMD::Make<CMD::arena_battle_object_state_notify>::From(h2,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(),(int*)vts_time);
				multi_send_ls_msg(pCtrl->_all_list, h2.data(), h2.size(), 0);
			}

			if (GetRefreshState())
			{
				// Status do seu personagem
				packet_wrapper h1(64);
				using namespace S2C;
				CMD::Make<CMD::player_info_00>::From(h1,_parent->ID,_basic.hp,_basic,_cur_prop,IsCombatState()?1:0, GetCurTarget().id, GlyphGetAttr() );
				multi_send_ls_msg(pCtrl->_all_list, h1.data(), h1.size(), 0);

				// Status dos seus aliados e inimigos
				packet_wrapper h2(64);
				using namespace S2C;
				CMD::Make<CMD::arena_battle_group_prop>::From(h2,_parent->ID.id,_basic.level,IsCombatState()?1:0,GetPlayerClass(),_basic.hp,_cur_prop.max_hp,_basic.mp,_cur_prop.max_mp,-1);
				multi_send_ls_msg(pCtrl->_all_list, h2.data(), h2.size(), 0);					
			}
		}
	}
}

void gplayer_arenaofaurora::PlayerEnterWorld()
{
	gplayer_imp::PlayerEnterWorld();

	attack_faction = 0;
	defense_faction = 0;
}

void gplayer_arenaofaurora::PlayerEnterServer(int source_tag)
{
	SetBattleFaction();
	gplayer_imp::PlayerEnterServer(source_tag);
	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF | filter::FILTER_MASK_BUFF);
	gplayer* pPlayer = GetParent();

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{

		case ARENA_TEAM_RED:
			pCtrl->OnPlayerEnter(pPlayer/*,0x01*/);
			pPlayer->team_id = 1;
			break;

		case ARENA_TEAM_BLUE:
			pCtrl->OnPlayerEnter(pPlayer/*,0x02*/);
			pPlayer->team_id = 2;
			break;

		default:
			pCtrl->OnPlayerEnter(pPlayer/*,0x0*/);
			pPlayer->team_id = 0;
			break;
		}
	}

	EnableFreePVP(true);
	_runner->player_enable_free_pvp(true);
}
 
void gplayer_arenaofaurora::PlayerLeaveServer()
{
	gplayer_imp::PlayerLeaveServer();
	gplayer* pPlayer = GetParent();

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x01*/);
			pPlayer->team_id = 0;
			break;
		case ARENA_TEAM_BLUE:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x02*/);
			pPlayer->team_id = 0;
			break;
		default:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x0*/);
			pPlayer->team_id = 0;
			break;
		}
	}

	if (_filters.IsFilterExist(FILTER_INDEX_INVISIBLE))
	{
		gplayer_imp* pImp = (gplayer_imp*)GetParent()->imp;

		object_interface obj_if(pImp);
		obj_if.SetInvisibleFilter(false, -1, 0, 160, 0);
		obj_if.RemoveTeamVisibleState((short)369);
		
		ClearGMInvisible();
		_commander->AllowCmd(controller::CMD_MARKET);
		_commander->AllowCmd(controller::CMD_PET);
		_commander->AllowCmd(controller::CMD_ELF_SKILL);
		_commander->AllowCmd(controller::CMD_USE_ITEM);
		_commander->AllowCmd(controller::CMD_NORMAL_ATTACK);
	}

	EnableFreePVP(false);
	_runner->player_enable_free_pvp(false);

	pPlayer->ClrBattleMode();

	if (_parent->b_zombie)
	{
		_parent->b_zombie = false;
		_basic.hp = (int)(_cur_prop.max_hp * 0.1f + 0.5f);
		_basic.mp = (int)(_cur_prop.max_mp * 0.1f + 0.5f);

		SetRefreshState();
		_enemy_list.clear();

		((gplayer_controller*)_commander)->OnResurrect();

		ClearNextSession();

		_runner->resurrect(0);
	}

	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

}
 
void gplayer_arenaofaurora::PlayerLeaveWorld()
{
	gplayer_imp::PlayerLeaveWorld();

	gplayer* pPlayer = GetParent();

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x01*/);
			pPlayer->team_id = 0;
			break;
		case ARENA_TEAM_BLUE:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x02*/);
			pPlayer->team_id = 0;
			break;
		default:
			pCtrl->OnPlayerLeave(pPlayer/*, 0x0*/);
			pPlayer->team_id = 0;
			break;
		}
	}

	if (_filters.IsFilterExist(FILTER_INDEX_GM_INVISIBLE))
	{
		gplayer_imp* pImp = (gplayer_imp*)GetParent()->imp;

		object_interface obj_if(pImp);
		obj_if.SetInvisibleFilter(false, -1, 0, 160, 0);
		obj_if.RemoveTeamVisibleState((short)369);

		ClearGMInvisible();
		_commander->AllowCmd(controller::CMD_MARKET);
		_commander->AllowCmd(controller::CMD_PET);
		_commander->AllowCmd(controller::CMD_ELF_SKILL);
		_commander->AllowCmd(controller::CMD_USE_ITEM);
		_commander->AllowCmd(controller::CMD_NORMAL_ATTACK);	
	}

	EnableFreePVP(false);
	_runner->player_enable_free_pvp(false);
	
	pPlayer->ClrBattleMode();

	if (_parent->b_zombie)
	{
		_parent->b_zombie = false;
		_basic.hp = (int)(_cur_prop.max_hp * 0.1f + 0.5f);
		_basic.mp = (int)(_cur_prop.max_mp * 0.1f + 0.5f);

		SetRefreshState();
		_enemy_list.clear();

		((gplayer_controller*)_commander)->OnResurrect();

		ClearNextSession();

		_runner->resurrect(0);
	}

	_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

	//GMSV::SendTrickBattleLeave(_parent->ID.id, pCtrl->_data.battle_id, world_manager::GetWorldTag());*/
}

int gplayer_arenaofaurora::GetFaction()
{
	return _faction | defense_faction;
}

int gplayer_arenaofaurora::GetEnemyFaction()
{
	return _enemy_faction | attack_faction;
}

gactive_imp::attack_judge gplayer_arenaofaurora::GetPetAttackHook()
{
	return gplayer_arenaofaurora::__GetPetAttackHook;
}

gactive_imp::enchant_judge gplayer_arenaofaurora::GetPetEnchantHook()
{
	return gplayer_arenaofaurora::__GetPetEnchantHook;
}

gactive_imp::attack_fill gplayer_arenaofaurora::GetPetAttackFill()
{
	return __GetPetAttackFill;
}

gactive_imp::enchant_fill gplayer_arenaofaurora::GetPetEnchantFill()
{
	return __GetPetEnchantFill;
}

bool gplayer_arenaofaurora::__GetPetAttackHook(gactive_imp* __this, const MSG& msg, attack_msg& amsg)
{
	//��ϵ����ȷ�򷵻�
	if (!(amsg.target_faction & (__this->GetFaction()))) return false;
	amsg.target_faction = 0xFFFFFFFF;
	amsg.is_invader = false;
	return true;
}

bool gplayer_arenaofaurora::__GetPetEnchantHook(gactive_imp* __this, const MSG& msg, enchant_msg& emsg)
{
	if (!emsg.helpful)
	{
		//��ϵ����ȷ�򷵻�
		if (!(emsg.target_faction & (__this->GetFaction()))) return false;
		emsg.target_faction = 0xFFFFFFFF;
	}
	else
	{
		//ֻ�жԷ������Լ��ǵ��˲��ܹ�ʹ�����淨�� �����player_battle�Ĳ�����ȫһ��
		if (emsg.target_faction & __this->GetFaction()) return false;
	}
	emsg.is_invader = false;
	return true;
}

void gplayer_arenaofaurora::__GetPetAttackFill(gactive_imp* __this, attack_msg& attack)
{
	gplayer_imp::__GetPetAttackFill(__this, attack);
	attack.force_attack = 0;
}

void gplayer_arenaofaurora::__GetPetEnchantFill(gactive_imp* __this, enchant_msg& enchant)
{
	gplayer_imp::__GetPetEnchantFill(__this, enchant);
	enchant.force_attack = 0;
}

void gplayer_arenaofaurora::SetBattleFaction()
{
	gplayer* pPlayer = GetParent();
	if (pPlayer->IsBattleOffense())
	{
		attack_faction = FACTION_BATTLEDEFENCE;
		defense_faction = FACTION_BATTLEOFFENSE | FACTION_OFFENSE_FRIEND;
	}
	else if (pPlayer->IsBattleDefence())
	{
		attack_faction = FACTION_BATTLEOFFENSE;
		defense_faction = FACTION_BATTLEDEFENCE | FACTION_DEFENCE_FRIEND;
	}
	else
	{
		attack_faction = 0;
		defense_faction = 0;
	}
}

void gplayer_arenaofaurora::OnDamage(const XID& attacker, int skill_id, const attacker_info_t& info, int damage, int at_state, char speed, bool orange, unsigned char section)
{
	gplayer_imp::OnDamage(attacker, skill_id, info, damage, at_state, speed, orange, section);

	if (attacker.IsPlayer())
	{
		arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
		pCtrl->OnDamage(attacker.id, damage);
	}
}

void gplayer_arenaofaurora::OnHurt(const XID& attacker, const attacker_info_t& info, int damage, bool invader)
{
	gplayer_imp::OnHurt(attacker, info, damage, invader);

	if (attacker.IsPlayer())
	{
		arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
		pCtrl->OnDamage(attacker.id, damage);
	}
}

void gplayer_arenaofaurora::OnDeath(const XID & lastattack, bool is_pariah, char attacker_mode, int taskdead)
{
	is_arena_zombie = true;

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		pCtrl->OnDeath(GetParent());
		wait_resurrect_timer = pCtrl->_battle_timer + 10;
	}

	// Adiciona a invisibilidade e o Buff
	if (is_arena_zombie)
	{
		gplayer_imp* pImp = (gplayer_imp*)GetParent()->imp;

		object_interface obj_if(pImp);
		obj_if.SetInvisibleFilter(true, -1, 0, 160, 0);
		obj_if.InsertTeamVisibleState((short)369, -1);
		
		SetGMInvisible();
		_commander->DenyCmd(controller::CMD_MARKET);
		_commander->DenyCmd(controller::CMD_PET);
		_commander->DenyCmd(controller::CMD_ELF_SKILL);
		_commander->DenyCmd(controller::CMD_USE_ITEM);
		_commander->DenyCmd(controller::CMD_NORMAL_ATTACK);
	}

	gplayer_imp::OnDeath(lastattack, false, attacker_mode, taskdead);
}

bool gplayer_arenaofaurora::CanResurrect(int param)
{
	return true;
}

int gplayer_arenaofaurora::Resurrect(const A3DVECTOR & pos, bool nomove, float exp_reduce, int target_tag, float hp_factor, float mp_factor, int param/*, float ap_factor, int extra_invincible_time*/)
{
	gplayer_imp::Resurrect(pos, false, exp_reduce, world_manager::GetWorldTag(), hp_factor, mp_factor, param, 0,0);
	
	return 0;
}

void gplayer_arenaofaurora::SendMsgToTeam(const MSG& msg, float range, bool exclude_self)
{
	//printf(" [ gplayer_arenaofaurora::SendMsgToTeam ] range=%d, exclude_self=%d\n", range, exclude_self);

	if (exclude_self)
	{
		SendTeamMessage(msg, range);
	}
	else
	{
		SendTeamAllMessage(msg, range);
	}
}

bool gplayer_arenaofaurora::OI_IsInTeam()
{
	bool result(false);

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			if (pCtrl->_red_alive_list.size())
				result = true;
			break;
		case ARENA_TEAM_BLUE:
			if (pCtrl->_blue_alive_list.size())
				result = true;
			break;
		}
	}

	//printf(" [ gplayer_arenaofaurora::IsInTeam ] playerid=%d, result=%d\n", _parent->ID.id, result);

	return result;
}

void gplayer_arenaofaurora::SendTeamMessage(const MSG& msg, float range)
{
	abase::vector<arenaofaurora_ctrl::arena_member_entry> mates;

	world* pPlane = _plane;

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)pPlane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			mates = pCtrl->_red_alive_list;
			break;
		case ARENA_TEAM_BLUE:
			mates = pCtrl->_blue_alive_list;
			break;
		}
	}

	if (!mates.size())
		return;

	size_t count = mates.size();
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];

	ASSERT(msg.source == _parent->ID);

	range *= range;

	for (size_t i = 0; i < count; i++)
	{
		const XID& memberId = mates[i].id;

		world::object_info info;
		if (msg.source == memberId ||
			!pPlane->QueryObject(memberId, info) ||
			info.pos.squared_distance(msg.pos) >= range)
		{
			continue;
		}

		list[index++] = memberId;
	}

	if (index)
	{
		pPlane->SendMessage(list, list + index, msg);
	}
}

void gplayer_arenaofaurora::SendTeamAllMessage(const MSG& msg, float range)
{
	abase::vector<arenaofaurora_ctrl::arena_member_entry> mates;

	world* pPlane = _plane;

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			mates = pCtrl->_red_alive_list;
			break;
		case ARENA_TEAM_BLUE:
			mates = pCtrl->_blue_alive_list;
			break;
		}
	}

	if (!mates.size())
		return;

	size_t count = mates.size();
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];

	ASSERT(msg.source == _parent->ID);

	range *= range;

	for (size_t i = 0; i < count; i++)
	{
		const XID& memberId = mates[i].id;

		world::object_info info;
		if (msg.source != memberId &&
			(!pPlane->QueryObject(memberId, info) ||
				info.pos.squared_distance(msg.pos) >= range))
		{
			continue;
		}

		list[index++] = memberId;
	}

	if (index)
	{
		pPlane->SendMessage(list, list + index, msg);
	}
}

int gplayer_arenaofaurora::DispatchCommand(int cmd_type, const void* buf, size_t size)
{
	switch (cmd_type)
	{
	case C2S::TEAM_INVITE:
	case C2S::TEAM_AGREE_INVITE:
	case C2S::TEAM_REJECT_INVITE:
	case C2S::TEAM_LEAVE_PARTY:
	case C2S::TEAM_KICK_MEMBER:
	case C2S::TEAM_GET_TEAMMATE_POS:
	case C2S::SIT_DOWN:
		return 0;

	default:
		return gplayer_imp::DispatchCommand(cmd_type, buf, size);
	}
}

void gplayer_arenaofaurora::SendTeamChat(char channel, const void* buf, size_t len, const void* aux_data, size_t dsize, int use_id)
{
	abase::vector<arenaofaurora_ctrl::arena_member_entry> mates;

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			mates = pCtrl->_red_alive_list;
			break;
		case ARENA_TEAM_BLUE:
			mates = pCtrl->_blue_alive_list;
			break;
		}
	}

	if (!mates.size())
		return;

	int self = (use_id > 0 ? use_id : _parent->ID.id);

	GMSV::chat_msg msg;
	msg.speaker = self;
	msg.msg = buf;
	msg.size = len;
	msg.data = aux_data;
	msg.dsize = dsize;
	msg.channel = channel;
	msg.emote_id = _chat_emote;
	msg.speaker_level = 0;

	size_t count = mates.size();
	for (size_t i = 0; i < count; i++)
	{
		const arenaofaurora_ctrl::arena_member_entry& member = mates[i];

		if (member.id.id != self)
		{
			GMSV::SendChatMsg(member.cs_index, member.id.id, member.cs_sid, msg);
		}
	}
}

bool gplayer_arenaofaurora::IsArenaMember(const XID& member)
{
	//printf("gplayer_arenaofaurora::IsArenaMember : %d\n", member.id);

	abase::vector<arenaofaurora_ctrl::arena_member_entry> team_mate_list;

	arenaofaurora_ctrl* pCtrl = (arenaofaurora_ctrl*)_plane->w_ctrl;
	if (pCtrl)
	{
		switch (pCtrl->GetTeamType(_parent->ID.id))
		{
		case ARENA_TEAM_RED:
			team_mate_list = pCtrl->_red_alive_list;
			break;
		case ARENA_TEAM_BLUE:
			team_mate_list = pCtrl->_blue_alive_list;
			break;
		}
	}

	if (team_mate_list.size())
	{
		for (size_t i = 0; i < team_mate_list.size(); i++)
		{
			if (team_mate_list[i].id.id == member.id)
			{
				return true;
			}
		}
	}

	return false;
}
