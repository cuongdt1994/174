#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arandomgen.h>
#include <iostream>
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdio>
#include <stdlib.h>

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
#include "instance/arenaofaurora_ctrl.h"

#include "global_controller.h"
#include "glyph_manager.h"
#include "luamanager.h"
#include "item/item_addon.h"
#include "world_points_manager.h"
#include "activity_event_manager.h"
#include "celestial_memorial_manager.h"
#include "repository_manager.h"
//#include "question_manager.h"
#include "emulate_settings.h"

#include "statedef.h"
#include "petdataman.h"

#include "codex_manager.h"

DEFINE_SUBSTANCE_ABSTRACT(switch_additional_data, substance, CLS_SWITCH_ADDITIONAL_DATA);
DEFINE_SUBSTANCE(countryterritory_switch_data, switch_additional_data, CLS_COUNTRYTERRITORY_SWITCH_DATA);
DEFINE_SUBSTANCE(trickbattle_switch_data, switch_additional_data, CLS_TRICKBATTLE_SWITCH_DATA);
DEFINE_SUBSTANCE(mnfaction_switch_data, switch_additional_data, CLS_MNFACTION_SWITCH_DATA);
// arena of aurora
DEFINE_SUBSTANCE(arenaofaurora_switch_data, switch_additional_data, CLS_ARENAOFAURORA_SWITCH_DATA);
// end arena of aurora

DEFINE_SUBSTANCE(gplayer_imp,gobject_imp,CLS_PLAYER_IMP)

void TrySwapPlayerData(world * pPlane,const int cid[3],gplayer * pPlayer)
{
	__PRINTF("��ʼ������ҵ������û�\n");
	gplayer_imp * imp = (gplayer_imp*) pPlayer->imp;
	gplayer_controller * ctrl = (gplayer_controller *)imp->_commander;
	gplayer_dispatcher * dis = (gplayer_dispatcher*)imp->_runner;
	if(imp->GetGUID() == cid[0] &&
			ctrl->GetGUID() == cid[1] &&
			dis->GetGUID() == cid[2])
	{
		//��ԭ��������ȫһ�£����봦��
		return ;
	}

	//�������� ǰ��Ӧ���Ѿ���֤��cid����ȷ����
	gplayer_imp * new_imp = (gplayer_imp *)CF_Create(cid[0],cid[2],cid[1],pPlane,pPlayer);
	ASSERT(new_imp);
//	gplayer_controller * new_ctrl =(gplayer_controller*)new_imp->_commander; 
//	gplayer_dispatcher * new_dis = (gplayer_dispatcher*)new_imp->_runner;
	
	new_imp->Swap(imp);
//	new_ctrl->LoadFrom(ctrl); �������Ѿ�Swap����
//	new_dis->LoadFrom(dis);

	pPlayer->imp = new_imp;
	delete  imp;
	delete  dis;
	delete  ctrl;
	return ;
}

gplayer_imp::gplayer_imp()
	:_inventory(item::INVENTORY,ITEM_LIST_BASE_SIZE),
	_equipment(item::BODY,item::EQUIP_INVENTORY_COUNT),
	_task_inventory(item::TASK_INVENTORY,TASKITEM_LIST_SIZE),
	_player_state(PLAYER_STATE_NORMAL),_combat_timer(0),
	_player_title(this),_player_dailysign(this),_player_fatering(this),_trashbox(TRASHBOX_BASE_SIZE,TRASHBOX_BASE_SIZE4,TRASHBOX_BASE_SIZE5,TRASHBOX_BASE_SIZE6),
    _user_trashbox(0u,0u,0u,0u),_player_force(this),_player_reincarnation(this)
{
	memset(&_instance_switch_key,0,sizeof(_instance_switch_key));
	_inventory.SetOwner(this);
	_equipment.SetOwner(this);
	_task_inventory.SetOwner(this);
	_trashbox.SetOwner(this);
	_user_trashbox.SetOwner(this);

	_provider.id = XID(-1,-1);

	memset(&_basic,0,sizeof(_basic));
	memset(&_cur_prop,0,sizeof(_cur_prop));
	memset(&_base_prop,0,sizeof(_base_prop));

	_disconnect_timeout = 0;
	_offline_type = PLAYER_OFF_LOGOUT;

	_inv_level = 0;
	_faction = 0;
	_enemy_faction = 0;
	_trade_obj = NULL;
	_stall_obj = NULL;
	_stall_trade_timer = 0;
	_stall_trade_id = g_timer.get_systime();
	_pvp_cooldown = 0;
	_write_timer = 513;
	_general_timeout = 0;
	_task_mask = 0;
	_link_notify_timer  = LINK_NOTIFY_TIMER;
	_cur_item.weapon_delay = UNARMED_ATTACK_DELAY;
	_trash_box_open_flag = false;
	_trash_box_open_view_only_flag = false;
	_user_trash_box_open_flag = false;
	_security_passwd_checked = false;
	_pvp_enable_flag = false;
	_force_attack = 0;
	_refuse_bless = 0;
	_tb_change_counter = 1;		//Ĭ������Ϊ�ֿ��������˱仯 �������
	_user_tb_change_counter = 1;//Ĭ������Ϊ�ʺŲֿ��������˱仯 �������
	_eq_change_counter = 1;		//Ĭ����Ϊװ���������˱仯 �������
	_kill_by_player = false;
	_free_pvp_mode = false;
	_nonpenalty_pvp_state = false;
	_resurrect_state = false;
	_resurrect_exp_reduce = 0.f;
	_resurrect_hp_factor = 0.f;
	_resurrect_mp_factor = 0.f;
	_resurrect_exp_lost_reduce = 0;
	_con_emote_target = 0;
	_con_emote_id = 0;
	_reputation = 0;
	_last_move_mode = 0;
	_logout_pos_flag = 0;
	_fall_counter = 0;
	_ap_per_hit = 0;
	_last_instance_tag = -1;
	_last_instance_timestamp = 0;
	_last_source_instance_tag = -1;
	_db_save_error = 0;
	memset(&move_checker,0,sizeof(move_checker));
	_username_len = 0;
	_pvp_combat_timer = 0;
	_double_exp_timeout = 0;
	_double_exp_mode = 0;
	_rest_counter_time = 0;
	_rest_time_used = 0;
	_rest_time_capacity = 0;
	_mafia_rest_time = 0;
	_mafia_rest_counter_time = 0;
	_login_timestamp = 0;
	_played_time = 0;
	_last_login_timestamp = 0;
	_create_timestamp = 0;
	_spec_task_reward = 0;
	_spec_task_reward2 = 0;
	_spec_task_reward_param = 0;
	_spec_task_reward_mask = 0;
	_duel_target = 0;
	_no_cooldown_mode = 0;
	_db_user_id = 0;

	_enemy_list.reserve(MAX_PLAYER_ENEMY_COUNT);

	_active_task_list.insert(_active_task_list.begin(),TASK_ACTIVE_LIST_BUF_SIZE,0);
	_finished_task_list.insert(_finished_task_list.begin(),TASK_FINISHED_LIST_BUF_SIZE,0);
	_finished_time_task_list.insert(_finished_time_task_list.begin(),TASK_FINISH_TIME_LIST_BUF_SIZE,0);
	_finish_task_count_list.insert(_finish_task_count_list.begin(),TASK_FINISH_COUNT_LIST_BUF_SIZE,0);
	_storage_task_list.insert(_storage_task_list.begin(),TASK_STORAGE_LIST_BUF_SIZE,0);

	_role_reputation_uchar.insert(_role_reputation_uchar.begin(),ROLE_REPUTATION_UCHAR_SIZE,0);
	_role_reputation_ushort.insert(_role_reputation_ushort.begin(),ROLE_REPUTATION_USHORT_SIZE,0);
	_role_reputation_uint.insert(_role_reputation_uint.begin(),ROLE_REPUTATION_UINT_SIZE,0);

	_speed_ctrl_factor = 16.0f;	//�����鲻��ĳ�ֵ����
	
	_mall_cash = 0;
	_mall_cash_used = 0;
	_mall_cash_offset = 0;
	_mall_cash_add = 0;
	_mall_order_id = 0;
	_mall_order_id_saved = 0;
	_mall_consumption = 0;
	_chat_emote = 0;
	_wallow_level = 0;
	_cheat_mode = 0;
	_cheat_punish = 0;
	_cheat_report = 0;

	_inv_switch_save_flag = false;
	_eqp_switch_save_flag = false;
	_tsk_switch_save_flag = false;

	_profit_time = 0;
	_profit_level = 0;
	_profit_timestamp = 0;
	_active_state_delay = 0;

	//lgc
	_min_elf_status_value = 0;
	memset(_equip_refine_level, 0, sizeof(_equip_refine_level));
	_soul_power = 0;
	_soul_power_en = 0;
	_min_addon_expire_date = 0;
	memset(&_pet_enhance,0,sizeof(_pet_enhance));
	memset(&_faction_contrib,0,sizeof(_faction_contrib));
	_level_up = false;
	_skill_attack_transmit_target = XID(-1,-1);
	_country_expire_time = 0;
	_in_central_server = false;
	_src_zoneid = 0;
	_king_expire_time = 0;
	_switch_additional_data = NULL;
	_need_refresh_equipment = false;
	_realm_exp = 0;
	_realm_level = 0;
	_leadership = 0;
	_leadership_occupied = 0;
	_world_contribution = 0;
	_world_contribution_cost = 0;
 	_astrolabe_extern_level = 0;
 	_astrolabe_extern_exp = 0;
	_fix_position_transmit_energy    = 0;
    _cash_resurrect_times_in_cooldown = 0;	
	
	_bind_state = 0;	
	auto_assist = false;
	check_assist_next_time = 0;
	_shield_energy = 0;
	_shield_energy_gen = 0;
	_shield_energy_max = 0;
	_hwid = 0;
	_increase_pet_time = 0;
	_real_weapon_class = 0;
	_fake_weapon_class = 0;

	//_question_day = 0;
	//_question_started = 0;
	//_question_mode = 0;
	//_question_progress = 0;
	//_question_answer = 0;
	//_question_correct = 0;
	//_question_time = 0;
	
	
	// nw
	
	_money.Init();
	memset(&_inviting_tm, 0x00, sizeof(_inviting_tm));
	
	_charge_merc = 0;
	_charge_merc_time = 0;
	memset(&_repository_addons, 0x00, sizeof(_repository_addons));
	memset(&_kids_addons, 0x00, sizeof(_kids_addons));

	_kid_transformation = 0;
	_kid_transformation_time = 0;
	memset(&_kid_transform_skill_state, 0, sizeof(_kid_transform_skill_state));

	_check_interface = 0;
	_check_genesis_lvl = 0;
	_dungeon_999_timer = 0;
	_check_codex_get_storage = 0;

	_color_name = 0;
	_pvp_rank = 0;
	_pvp_rank_exp = 0;
	_player_kill = 0;  
	_monster_kill = 0;  
	_player_death = 0;
	_monster_death = 0;
	_diary_exp = 0;
	_realm_day_verify = 0;
	_verify_itens_valid = 0;
	_has_astrolabe_lock = 0;
	_enabled_fashion_weapon = 0;

	_double_factor_exp = 0;
	_double_factor_sp = 0;
	_double_factor_realm = 0;
	
	memset(&_autoswap         , 0x00 , sizeof( _autoswap         ));
	memset(&_skillsender      , 0x00 , sizeof( _skillsender      ));
	memset(&_glyph            , 0x00 , sizeof( _glyph            ));
	memset(&_carrier          , 0x00 , sizeof( _carrier          ));
	memset(&_repository       , 0x00 , sizeof( _repository       ));
	memset(&_pet_skill_temp   , 0x00 , sizeof( _pet_skill_temp   ));
	memset(&_pet_skin         , 0x00 , sizeof( _pet_skin         ));
	memset(&_day_world_points , 0x00 , sizeof( _day_world_points ));
	memset(&_activity 		  , 0x00 , sizeof( _activity		 ));
	//memset(&_question_data	  , 0x00 , sizeof( _question_data	 ));
	
	_repository.lock_time = 2;
	// storage data 2

	_passwd_safe.Init();
	_treasure.Init();
	_lottery.Init();
	_treasure_items.Init();
	_lib_items.Init();
	_celestial.Init();
	
	_glua.Init();
	memset(&_codex 		  , 0x00 , sizeof( _codex		 ));
	memset(&_kid 		  , 0x00 , sizeof( _kid		 	 ));
	_kid_addon.Init();

}

gplayer_imp::~gplayer_imp()
{
	if(_trade_obj)
	{
		//ASSERT(false && "���׶���Ӧ����ǰ�ͷŵ�");
		delete _trade_obj;
	}
	if(_stall_obj)
	{
		//ASSERT(false && "��̯����Ӧ����ǰ�ͷŵ�");
		delete _stall_obj;
	}
	ClearSwitchAdditionalData();
}

void 
gplayer_imp::Init(world * pPlane,gobject*parent)
{
	gactive_imp::Init(pPlane,parent);
	_team.Init(this);
	_invade_ctrl.Init(this);
	InitClock();
}

struct cl_world_contrib_reset : public clock_listener
{
	void OnClock(gplayer_imp* player,int type)	
	{ 
		player->ClearWorldContrib();
	}
	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
	{ 
		player->ClearWorldContrib(); 
	}
};

struct cl_task_storage_refresh : public clock_listener
{
	void OnClock(gplayer_imp* player,int type)
	{
		PlayerTaskInterface  task_if(player); // ��0��ˢ�¹��׶�����,�������������Լ�ˢ��
		task_if.RefreshTaskStorage(0);
	}
	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) {}
};

struct cl_solo_tower_challenge : public clock_listener
{
 	void OnClock(gplayer_imp* player,int type)
 	{
		player->_solochallenge.OnClock(player);
 	}
 	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
 	{
		player->_solochallenge.OnPassClock(player, lastupdate, now);
 	}
};

struct cl_clear_day_item : public clock_listener
{
 	void OnClock(gplayer_imp* player,int type)
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, g_timer.get_systime());
		player->GetPurchaseLimit().SetDayItemClearTimeStamp(next_update_time, g_timer.get_systime());
		player->_runner->purchase_limit_all_info_notify();
 	}
 	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, now);
		player->GetPurchaseLimit().SetDayItemClearTimeStamp(next_update_time, now);
		player->_runner->purchase_limit_all_info_notify();
 	}
};

struct cl_clear_week_item : public clock_listener
{
 	void OnClock(gplayer_imp* player,int type)
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, g_timer.get_systime());
		player->GetPurchaseLimit().SetWeekItemClearTimeStamp(next_update_time, g_timer.get_systime());
		player->_runner->purchase_limit_all_info_notify();
 	}
 	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, now);
		player->GetPurchaseLimit().SetWeekItemClearTimeStamp(next_update_time, now);
		player->_runner->purchase_limit_all_info_notify();
 	}
};

struct cl_clear_month_item : public clock_listener
{
 	void OnClock(gplayer_imp* player,int type)
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, g_timer.get_systime());
		player->GetPurchaseLimit().SetMonthItemClearTimeStamp(next_update_time, g_timer.get_systime());
		player->_runner->purchase_limit_all_info_notify();
 	}
 	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
 	{
		int next_update_time = player_clock::GetNextUpdatetime(type, now);
		player->GetPurchaseLimit().SetMonthItemClearTimeStamp(next_update_time, now);
		player->_runner->purchase_limit_all_info_notify();
 	}
};

struct cl_clear_year_item : public clock_listener
{
 	void OnClock(gplayer_imp* player,int type)
 	{
		int next_update_time = player->GetPurchaseLimit().GetNextYearStamp();
		player->GetPurchaseLimit().SetYearItemClearTimeStamp(next_update_time, g_timer.get_systime());
		player->_runner->purchase_limit_all_info_notify();
 	}
 	void OnPassClock(gplayer_imp* player,int type,int lastupdate,int now) 
 	{
		int next_update_time = player->GetPurchaseLimit().GetNextYearStamp();
		player->GetPurchaseLimit().SetYearItemClearTimeStamp(next_update_time, now);
		player->_runner->purchase_limit_all_info_notify();
 	}
};

void
gplayer_imp::InitClock()
{
	/////////////////////////////////////////////////////////////////////////
	// declare
	static cl_world_contrib_reset cl_wcr;
	static cl_task_storage_refresh cl_tsr;
 	static cl_solo_tower_challenge cl_stc;
	static cl_clear_day_item cl_cdi;
	static cl_clear_week_item cl_cwi;
	static cl_clear_month_item cl_cmi;
	static cl_clear_year_item cl_cyi;
	/////////////////////////////////////////////////////////////////////////
	// registor
	_player_clock.AddNotice(&cl_wcr,player_clock::GPC_PER_HOUR_LOCAL,0); // ÿ��0�� ����
	_player_clock.AddNotice(&cl_tsr,player_clock::GPC_PER_HOUR_GLOBAL,0); // ÿ��0�� ȫ��
	
 	_player_clock.AddNotice(&cl_stc,player_clock::GPC_PER_HOUR_LOCAL,7); //ÿ��7�� ����

	//������Ʒ�޹�
	_player_clock.AddNotice(&cl_cdi,player_clock::GPC_PER_DAY_LOCAL,-1);   // ÿ��0�� ����
	_player_clock.AddNotice(&cl_cwi,player_clock::GPC_PER_WEEK_LOCAL,-1);    // ÿ��1 0�� ����
	_player_clock.AddNotice(&cl_cmi,player_clock::GPC_PER_MONTH_LOCAL,-1); // ÿ��1�� ����
	_player_clock.AddNotice(&cl_cyi,player_clock::GPC_PER_MONTH_LOCAL,0); // ÿ��1��1�� ����
	
}

void 
gplayer_imp::PlayerEnterWorld()
{
	int tag = world_manager::GetWorldTag();

	for(unsigned int i = 0; i < _equipment.Size(); i ++)
	{
		item & it = _equipment[i];
					
		if(it.type == -1) continue;
		if(it.count == 0) continue;
		if(it.proc_type & item::ITEM_PROC_TYPE_BIND2)
		{
			it.proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
			it.proc_type &= ~(item::ITEM_PROC_TYPE_BIND2);

			UpdateMallConsumptionBinding(it.type, it.proc_type, it.count);
		}
	}

	_inventory.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_equipment.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_task_inventory.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(1).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(2).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(3).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(4).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(5).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	
	_user_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);


	RefreshEquipment();
	CalcEquipmentInfo();

	if(!world_manager::GetWorldParam().pve_mode || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
	{
		{
			GetParent()->object_state |= gactive_object::STATE_PVPMODE;
			_pvp_enable_flag = true;
			_pvp_cooldown = PVP_STATE_COOLDOWN;
		}
	}

	_plane->SyncPlayerWorldGen((gplayer*)_parent);
	_runner->notify_pos(_parent->pos);	//add by liuguichen,20130715
	EnterWorld();
	if(_layer_ctrl.IsFalling())
	{
		
	}

	TestUnderWater();

	if(_parent->IsZombie() && _resurrect_state)
	{
		_runner->enable_resurrect_state(_resurrect_exp_reduce);
	}

    if (_parent->IsZombie() && CheckVipService(CVS_RESURRECT) && !world_manager::GetWorldLimit().nocash_resurrect)
    {
        int index = _cash_resurrect_times_in_cooldown;
        if (index < 0) index = 0;
        if (CheckCoolDown(COOLDOWN_INDEX_RESURRECT_BY_CASH))
        {
            index = 0;
        }
        else
        {
            ++index;
            if (index >= CASH_RESURRECT_COST_TABLE_SIZE)
                index = CASH_RESURRECT_COST_TABLE_SIZE - 1;
        }

        int cash_need = CASH_RESURRECT_COST_TABLE[index];
        _runner->cash_resurrect_info(cash_need, GetMallCash());
    }

	//�����Ƿ��ڰ�ȫ��
	TestSanctuary();
	
	if(!world_manager::GetWorldParam().pve_mode || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
	{
		//�����pvp������,����Ƿ������ְ�ȫ����
		TestPKProtected();
	}

	_runner->server_config_data();

	_ph_control.Initialize(this);
	UpdatePlayerLayer();	
	
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_CHEATER))
	{
		_cheat_punish = 1;
	}

	PlaneEnterNotify(true);
	_runner->send_world_life_time(_plane->w_life_time);

	if(GetCountryId()) GMSV::CountryBattleOnline(_parent->ID.id, GetCountryId(), tag, GetSoulPower(), GetParent()->IsKing());

	if(world_manager::GetIsSoloTowerChallengeInstance())
	{
		PlayerEnterSoloChallengeInstance();
	}
	
	LuaManager * lua = LuaManager::GetInstance();
	lua->PlayerEnterWorld(_parent->ID.id);
	lua->SendPlayerLuaInfo(_parent->ID.id);
	//ChecPetChell();

	if(HasRepositoryLock()) 
	{ 
		ProtocolRepositoryStorage();		
	}

	/*166+*/
	if (GetPlayerClass() == USER_CLASS_HAG)
	{
		PetEnterWorldSkinStorage();
	}

	/*160+*/
	SpeedSkillManager();

	/*174+*/
	ActivityEventActivate();

	if(_activity.enable_skill)
	ActivityEventSkillManager();

	ProtocolActivityEvent();

	// Memorial Celestial
	ProtocolCelestialMemorial();	
	ProtocolCelestialMemorialLottery();

	//QuestionTask();

	// Anedota
	SetAnecdotePoints();

	//CheckColorNameItemLogin();

	//if(_basic.sec.flag == 0 && _basic.sec.level >= 20 && _basic.sec.level <= 32)
	//{
	//	SetFlagForOldPlayer();
	//}

	_real_weapon_class = _cur_item.weapon_class;

	/*171+*/
	PlayerWeaponUpdateEnterWorld();

	/*170+ Códice*/
	FashionUpdateActivate(true);

	/*161+*/
	RefreshInventoryNewArmorEnter(false, false);
	// Correção de bug na paleta e nos tesouros
	int level_get = _basic.level;
	int reincarnation_get = GetReincarnationTimes();

	if(level_get <= 10 && reincarnation_get == 0)		
	{
		if(_lottery.GetScore() > 5000 ||
		 _treasure.GetScore() > 5000)
		 {
			PlayerTeasouresReset();
		 }
	}
	// Reseta Memorial celesital (Bebe Celestial)
	if (EmulateSettings::GetInstance()->GetEnabledMemorialReset() && !GetLua()->GetResetCelestialMemorial())
	{		
		GetLua()->SetResetCelestialMemorial(true);

		if (EmulateSettings::GetInstance()->GetEnabledChild())
		{
			for (unsigned int i = 0; i < gplayer_kid::MAX_CELESTIAL; i++)
			{
				_kid.SetCelestial(i, 0, 0, 0, 0);
			}
		} else
		{
			_celestial.ClearAllCelestialMemorial();
			ProtocolCelestialMemorial();
			ProtocolCelestialMemorialLottery();
		}
	}

	// Reseta a Paleta (07/05)
	if(!GetLua()->GetResetLottery())
	{
		GetLua()->SetResetLottery(true);

		if(_lottery.GetScore() > 0)
		{
			_lottery.SetScore(0);
			GetLotteryItems();
			GetLoteryInfo();
		}
	}
	if (EmulateSettings::GetInstance()->GetEnabledChild())
	{
		KidAwakeningInfoProtocol ();
		KidCelestialInfoProtocol (0);
		KidCelestialActivityProtocol ();
		_kid_addon.UpdateKidsAddonsProtocol(_parent->ID.id);

		// Nova contagem de pontos (notificação para o client)
		_runner->kid_system_points_notify(KidGetSuitePoints());
	}

	if (EmulateSettings::GetInstance()->GetEnabledChild())
	{
		_kid_addon.SetKidsWipe(_parent->ID.id);
	}	

}

void
gplayer_imp::GetCommonDataList(bool send_content)
{
	packet_wrapper  h1(8192);
	using namespace S2C;
	CMD::Make<CMD::common_data_list>::From(h1);
	if(send_content)
	{
		class my_stream : public common_data::stream
		{
			packet_wrapper  & _wrapper;
			int _count;
			public:
			my_stream(packet_wrapper & wrapper):_wrapper(wrapper),_count(0)
			{
			}
			void dump(int key, int value)
			{
				if(_count < 2048)
				{
					_count ++;
					_wrapper<< key << value;
				}
			}
		};

		my_stream t1(h1);
		_plane->w_common_data.Dump(100000, &t1);
	}
	send_ls_msg(GetParent(), h1);
}

int 
gplayer_imp::DispatchMessage(world *pPlane, const MSG & msg)
{
/*
	timeval t1,t2;
	gettimeofday(&t1,NULL);
	*/
	gplayer * pPlayer = (gplayer *)_parent;
	int rst = 0;
	switch(pPlayer->login_state)
	{
		case gplayer::WAITING_LOGOUT:
		case gplayer::WAITING_LOGIN:
		//�������κ���Ϣ
			break;
		case gplayer::WAITING_ENTER:
			if(msg.message == GM_MSG_ENTER_WORLD)
			{
				_general_timeout = 0; 
				pPlayer->login_state = gplayer::LOGIN_OK;
				PlayerEnterWorld();


				int locktime = msg.param;
				int maxlocktime =  0;
				if(msg.content_length == sizeof(int)) maxlocktime =  *(int*)msg.content;
				//��¼����ʱ��
				gplayer_controller * pCtrl = (gplayer_controller *) _commander;
				pCtrl->SetSafeLock(locktime, maxlocktime);
			}
			else
			if(msg.message == GM_MSG_HEARTBEAT)
			{
				_general_timeout ++;
				if(_general_timeout > 600)
				{
					//WAITING_ENTER 10 ���ӳ�ʱ
					int cs_index = ((gplayer*)_parent)->cs_index;
					int uid = ((gplayer*)_parent)->ID.id;
					int sid = ((gplayer*)_parent)->cs_sid;
					_commander->Release();
					GMSV::SendDisconnect(cs_index,uid,sid,0);
				}
			}
			break;

		case gplayer::DISCONNECTED:
		case gplayer::LOGIN_OK:
		{
			switch(_player_state)
			{
				case PLAYER_STATE_NORMAL:
				case PLAYER_STATE_COSMETIC:
				case PLAYER_STATE_BIND:
				rst = DispatchNormalMessage(pPlane,msg);
				break;

				case PLAYER_DISCONNECT:
				rst = DisconnectMessageHandler(pPlane,msg);
				break;

				case PLAYER_WAITING_TRADE:
				rst = WaitingTradeMessageHandler(pPlane,msg);
				break;

				case PLAYER_TRADE:
				rst = TradeMessageHandler(pPlane,msg);
				break;

				case PLAYER_WAIT_TRADE_COMPLETE:
				rst = WaitingTradeCompleteHandler(pPlane,msg);
				break;
					
				case PLAYER_WAIT_TRADE_READ:
				rst = WaitingTradeReadHandler(pPlane,msg);
				break;
				
				case PLAYER_WAIT_FACTION_TRADE_READ:
				rst = WatingFactionTradeReadHandler(pPlane,msg);
				break;

				case PLAYER_WAITING_FACTION_TRADE:
				rst = WaitingFactionTradeTradeHandler(pPlane,msg);
				break;

				case PLAYER_WAIT_SWITCH:
				rst = WaitingSwitchServer(pPlane,msg);
				break;

				case PLAYER_SIT_DOWN:
				rst = StayInHandler(pPlane,msg);
				break;

				case PLAYER_STATE_MARKET:
				rst = MarketHandler(pPlane,msg);
				break;

				case PLAYER_STATE_TRAVEL:
				rst = TravelMessageHandler(pPlane,msg);
				break;

				default:
				ASSERT(false);
				return 0;
			}
			break;
		}
		default:
			ASSERT(false);
			break;
	}
	return rst;
}

int
gplayer_imp::StayInHandler(world * pPlane ,const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_HEARTBEAT:
		case GM_MSG_QUERY_OBJ_INFO00:
		case GM_MSG_PICKUP_MONEY:
		case GM_MSG_PICKUP_MONEY2:
		case GM_MSG_PICKUP_ITEM:
		case GM_MSG_ERROR_MESSAGE:
		case GM_MSG_GROUP_EXPERIENCE:
		case GM_MSG_EXPERIENCE:
		case GM_MSG_TEAM_EXPERIENCE:
	//	case GM_MSG_GET_MEMBER_POS:
		case GM_MSG_TEAM_INVITE:
		case GM_MSG_TEAM_AGREE_INVITE:
		case GM_MSG_TEAM_REJECT_INVITE:
		case GM_MSG_TEAM_PICKUP:
		case GM_MSG_JOIN_TEAM:
		case GM_MSG_LEADER_UPDATE_MEMBER:
		case GM_MSG_JOIN_TEAM_FAILED:
		case GM_MSG_MEMBER_NOTIFY_DATA:
		case GM_MSG_NEW_MEMBER:
		case GM_MSG_LEAVE_PARTY_REQUEST:
		case GM_MSG_LEADER_CANCEL_PARTY:
		case GM_MSG_MEMBER_NOT_IN_TEAM:
		case GM_MSG_LEADER_KICK_MEMBER:
		case GM_MSG_MEMBER_LEAVE:
		case GM_MSG_QUERY_PLAYER_EQUIPMENT:
		case GM_MSG_PICKUP_TEAM_MONEY:
		case GM_MSG_PICKUP_TEAM_MONEY2: //172
		case GM_MSG_RECEIVE_MONEY:
		case GM_MSG_RECEIVE_MONEY2: //172
		case GM_MSG_NPC_BE_KILLED:
		case GM_MSG_HATE_YOU:
		case GM_MSG_PLAYER_TASK_TRANSFER:
		case GM_MSG_PLAYER_BECOME_PARIAH:
		case GM_MSG_PLAYER_BECOME_INVADER:
		case GM_MSG_NOTIFY_SELECT_TARGET:
		case GM_MSG_QUERY_SELECT_TARGET:
		case GM_MSG_SUBSCIBE_TARGET:
		case GM_MSG_UNSUBSCIBE_TARGET:
		case GM_MSG_SUBSCIBE_CONFIRM:
		case GM_MSG_SUBSCIBE_SUBTARGET:
		case GM_MSG_UNSUBSCIBE_SUBTARGET:
		case GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM:
		case GM_MSG_NOTIFY_SELECT_SUBTARGET:
		case GM_MSG_HP_STEAL:
		case GM_MSG_TEAM_APPLY_PARTY:
		case GM_MSG_TEAM_APPLY_REPLY:
		case GM_MSG_QUERY_INFO_1:
		case GM_MSG_TEAM_CHANGE_TO_LEADER:
		case GM_MSG_TEAM_LEADER_CHANGED:
		//û�д���session����Ϣ��Ŀǰ������״̬��û���κ�session�����
		case GM_MSG_GM_RECALL:
		case GM_MSG_GM_CHANGE_EXP:
		case GM_MSG_GM_ENDUE_ITEM:
		case GM_MSG_GM_ENDUE_SELL_ITEM:
		case GM_MSG_GM_REMOVE_ITEM:
		case GM_MSG_GM_ENDUE_MONEY:
		case GM_MSG_GM_OFFLINE:
		case GM_MSG_GM_MQUERY_MOVE_POS:
		case GM_MSG_GM_MQUERY_MOVE_POS_REPLY:
		case GM_MSG_GM_DEBUG_COMMAND:
		case GM_MSG_GM_RESET_PP:
		case GM_MSG_GM_QUERY_SPEC_ITEM:
		case GM_MSG_GM_REMOVE_SPEC_ITEM:
		case GM_MSG_DBSAVE_ERROR:	
		case GM_MSG_ENABLE_PVP_DURATION:
		case GM_MSG_PLAYER_DUEL_REQUEST:
		case GM_MSG_PLAYER_DUEL_REPLY:
		case GM_MSG_PLAYER_DUEL_PREPARE:
		case GM_MSG_PLAYER_DUEL_START:
		case GM_MSG_PLAYER_DUEL_CANCEL:
		case GM_MSG_PLAYER_DUEL_STOP:
		case GM_MSG_QUERY_EQUIP_DETAIL:
		case GM_MSG_PLAYER_RECALL_PET:
		case GM_MSG_MOB_BE_TRAINED:
		case GM_MSG_PET_SET_COOLDOWN:
		case GM_MSG_PET_ANTI_CHEAT:
		case GM_MSG_PET_NOTIFY_DEATH:
		case GM_MSG_PET_NOTIFY_HP:
		case GM_MSG_PET_RELOCATE_POS:
		case GM_MSG_QUERY_PROPERTY:
		case GM_MSG_QUERY_PROPERTY_REPLY:
		case GM_MSG_NOTIFY_CLEAR_INVISIBLE:
		case GM_MSG_PLANT_PET_NOTIFY_DEATH:
		case GM_MSG_PLANT_PET_NOTIFY_HP:
		case GM_MSG_PLANT_PET_NOTIFY_DISAPPEAR:
		case GM_MSG_CONGREGATE_REQUEST:
		case GM_MSG_REJECT_CONGREGATE:
		case GM_MSG_NPC_BE_KILLED_BY_OWNER:
		case GM_MSG_EXTERN_HEAL:
		case GM_MSG_QUERY_INVENTORY_DETAIL:
		case GM_MSG_PLAYER_KILLED_BY_PLAYER:
		case GM_MSG_PUNISH_ME:
		case GM_MSG_REDUCE_CD:	
        case GM_MSG_LOOKUP_ENEMY:
        case GM_MSG_LOOKUP_ENEMY_REPLY:
		//���µ�ʱ����Դ���GM��Ϣ
			return MessageHandler(pPlane,msg);

		//�ܵ��е���ķ���Ҳ�ᴦ��
		case GM_MSG_ENCHANT:
			if(!((enchant_msg*)msg.content)->helpful)
			{
				LeaveStayInState();
			}
			return MessageHandler(pPlane,msg);
		case GM_MSG_ATTACK:
		case GM_MSG_HURT:
		case GM_MSG_DUEL_HURT:
		case GM_MSG_TRANSFER_FILTER_DATA:
		case GM_MSG_TRANSFER_FILTER_GET:
		//�ܵ������ᵼ���뿪ͣ��״̬
			LeaveStayInState();
			return MessageHandler(pPlane,msg);

			
	}
	return 0;
}

int
gplayer_imp::TravelMessageHandler(world * pPlane ,const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_HEARTBEAT:
		case GM_MSG_QUERY_OBJ_INFO00:
		case GM_MSG_PICKUP_MONEY:
		case GM_MSG_PICKUP_MONEY2:
		case GM_MSG_PICKUP_ITEM:
		case GM_MSG_ERROR_MESSAGE:
		case GM_MSG_GROUP_EXPERIENCE:
		case GM_MSG_EXPERIENCE:
		case GM_MSG_TEAM_EXPERIENCE:
	//	case GM_MSG_GET_MEMBER_POS:
		case GM_MSG_TEAM_INVITE:
		case GM_MSG_TEAM_AGREE_INVITE:
		case GM_MSG_TEAM_REJECT_INVITE:
		case GM_MSG_TEAM_PICKUP:
		case GM_MSG_JOIN_TEAM:
		case GM_MSG_LEADER_UPDATE_MEMBER:
		case GM_MSG_JOIN_TEAM_FAILED:
		case GM_MSG_MEMBER_NOTIFY_DATA:
		case GM_MSG_NEW_MEMBER:
		case GM_MSG_LEAVE_PARTY_REQUEST:
		case GM_MSG_LEADER_CANCEL_PARTY:
		case GM_MSG_MEMBER_NOT_IN_TEAM:
		case GM_MSG_LEADER_KICK_MEMBER:
		case GM_MSG_MEMBER_LEAVE:
		case GM_MSG_QUERY_PLAYER_EQUIPMENT:
		case GM_MSG_QUERY_EQUIP_DETAIL:
		case GM_MSG_PICKUP_TEAM_MONEY:
		case GM_MSG_PICKUP_TEAM_MONEY2: //172
		case GM_MSG_RECEIVE_MONEY:
		case GM_MSG_RECEIVE_MONEY2: //172
		case GM_MSG_NPC_BE_KILLED:
		case GM_MSG_PLAYER_TASK_TRANSFER:
		case GM_MSG_PLAYER_BECOME_PARIAH:
		case GM_MSG_PLAYER_BECOME_INVADER:
		case GM_MSG_NOTIFY_SELECT_TARGET:
		case GM_MSG_QUERY_SELECT_TARGET:
		case GM_MSG_SUBSCIBE_TARGET:
		case GM_MSG_UNSUBSCIBE_TARGET:
		case GM_MSG_SUBSCIBE_CONFIRM:
		case GM_MSG_SUBSCIBE_SUBTARGET:
		case GM_MSG_UNSUBSCIBE_SUBTARGET:
		case GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM:
		case GM_MSG_NOTIFY_SELECT_SUBTARGET:
		case GM_MSG_HP_STEAL:
		case GM_MSG_TEAM_APPLY_PARTY:
		case GM_MSG_TEAM_APPLY_REPLY:
		case GM_MSG_QUERY_INFO_1:
		case GM_MSG_TEAM_CHANGE_TO_LEADER:
		case GM_MSG_TEAM_LEADER_CHANGED:
		case GM_MSG_QUERY_PROPERTY:	
		case GM_MSG_QUERY_PROPERTY_REPLY:	

		//����session,��ʵ������ֻ������·��session
		case GM_MSG_OBJ_SESSION_END:
		case GM_MSG_OBJ_SESSION_REPEAT:
		case GM_MSG_OBJ_SESSION_REPEAT_FORCE:
		case GM_MSG_DBSAVE_ERROR:


		//����GM��Ϣ
		case GM_MSG_GM_RECALL:
		case GM_MSG_GM_CHANGE_EXP:
		case GM_MSG_GM_ENDUE_ITEM:
		case GM_MSG_GM_ENDUE_SELL_ITEM:
		case GM_MSG_GM_REMOVE_ITEM:
		case GM_MSG_GM_ENDUE_MONEY:
		case GM_MSG_GM_OFFLINE:
		case GM_MSG_GM_DEBUG_COMMAND:
		case GM_MSG_GM_MQUERY_MOVE_POS:
		case GM_MSG_GM_MQUERY_MOVE_POS_REPLY:
		case GM_MSG_GM_RESET_PP:
		case GM_MSG_GM_QUERY_SPEC_ITEM:
		case GM_MSG_GM_REMOVE_SPEC_ITEM:
		case GM_MSG_ENABLE_PVP_DURATION:
        case GM_MSG_LOOKUP_ENEMY:
        case GM_MSG_LOOKUP_ENEMY_REPLY:
			return MessageHandler(pPlane,msg);

		//�������й�������Ϣ�ͼ�����Ϣ
		case GM_MSG_ENCHANT:
		case GM_MSG_ATTACK:
		case GM_MSG_HURT:
		case GM_MSG_DUEL_HURT:
			return 0;
			
	}
	return 0;
}

int 
gplayer_imp::ZombieMessageHandler(world * pPlane ,const MSG & msg)
{
	//ֻ��������
	switch(msg.message)
	{
		case GM_MSG_SWITCH_GET:
		//���账��
		break;

		case GM_MSG_GATHER_REPLY:
		{
			//ȡ���ɼ�
			SendTo<0>(GM_MSG_GATHER_CANCEL,msg.source,0);
		}
		return 0;

		case GM_MSG_OBJ_ZOMBIE_SESSION_END:
		case GM_MSG_GATHER_RESULT:
		case GM_MSG_HEARTBEAT:
		case GM_MSG_QUERY_OBJ_INFO00:
		case GM_MSG_PICKUP_MONEY:
		case GM_MSG_PICKUP_MONEY2:
		case GM_MSG_PICKUP_ITEM:
		case GM_MSG_ERROR_MESSAGE:
		case GM_MSG_GROUP_EXPERIENCE:
//		case GM_MSG_EXPERIENCE:
//		case GM_MSG_TEAM_EXPERIENCE:
//		case GM_MSG_GET_MEMBER_POS:
		case GM_MSG_TEAM_INVITE:
		case GM_MSG_TEAM_AGREE_INVITE:
		case GM_MSG_TEAM_REJECT_INVITE:
		case GM_MSG_TEAM_PICKUP:
		case GM_MSG_JOIN_TEAM:
		case GM_MSG_LEADER_UPDATE_MEMBER:
		case GM_MSG_JOIN_TEAM_FAILED:
		case GM_MSG_MEMBER_NOTIFY_DATA:
		case GM_MSG_NEW_MEMBER:
		case GM_MSG_LEAVE_PARTY_REQUEST:
		case GM_MSG_LEADER_CANCEL_PARTY:
		case GM_MSG_MEMBER_NOT_IN_TEAM:
		case GM_MSG_LEADER_KICK_MEMBER:
		case GM_MSG_MEMBER_LEAVE:
		case GM_MSG_QUERY_PLAYER_EQUIPMENT:
		case GM_MSG_PICKUP_TEAM_MONEY:
		case GM_MSG_PICKUP_TEAM_MONEY2:
//		case GM_MSG_RECEIVE_MONEY:
		case GM_MSG_NPC_BE_KILLED:
		case GM_MSG_PLAYER_TASK_TRANSFER:
		case GM_MSG_PLAYER_BECOME_PARIAH:
		case GM_MSG_PLAYER_BECOME_INVADER:
		case GM_MSG_NOTIFY_SELECT_TARGET:
		case GM_MSG_QUERY_SELECT_TARGET:
		case GM_MSG_SUBSCIBE_TARGET:
		case GM_MSG_UNSUBSCIBE_TARGET:
		case GM_MSG_SUBSCIBE_CONFIRM:
		case GM_MSG_SUBSCIBE_SUBTARGET:
		case GM_MSG_UNSUBSCIBE_SUBTARGET:
		case GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM:
		case GM_MSG_NOTIFY_SELECT_SUBTARGET:
		case GM_MSG_TEAM_APPLY_PARTY:
		case GM_MSG_TEAM_APPLY_REPLY:
		case GM_MSG_QUERY_INFO_1:
		case GM_MSG_TEAM_CHANGE_TO_LEADER:
		case GM_MSG_TEAM_LEADER_CHANGED:
		case GM_MSG_DBSAVE_ERROR:
		case GM_MSG_PLAYER_DUEL_START:
		case GM_MSG_PLAYER_DUEL_CANCEL:
		case GM_MSG_PLAYER_DUEL_STOP:

		case GM_MSG_GM_RECALL:
		case GM_MSG_PLANE_SWITCH_REPLY:
		case GM_MSG_ENABLE_PVP_DURATION:
		case GM_MSG_QUERY_EQUIP_DETAIL:
		case GM_MSG_REMOVE_ITEM:
		case GM_MSG_QUERY_PROPERTY:
		case GM_MSG_QUERY_PROPERTY_REPLY:
		case GM_MSG_CONTRIBUTION_TO_KILL_NPC:
		case GM_MSG_GROUP_CONTRIBUTION_TO_KILL_NPC:
		case GM_MSG_CONGREGATE_REQUEST:
		case GM_MSG_REJECT_CONGREGATE:
		case GM_MSG_NPC_BE_KILLED_BY_OWNER:
		case GM_MSG_QUERY_INVENTORY_DETAIL:
		case GM_MSG_PLAYER_KILLED_BY_PLAYER:
		
		//�ܹ�������GM��Ϣ
		case GM_MSG_GM_MQUERY_MOVE_POS:
		case GM_MSG_GM_MQUERY_MOVE_POS_REPLY:
		case GM_MSG_GM_OFFLINE:
		case GM_MSG_GM_QUERY_SPEC_ITEM:
		case GM_MSG_GM_REMOVE_SPEC_ITEM:
        case GM_MSG_LOOKUP_ENEMY:
        case GM_MSG_LOOKUP_ENEMY_REPLY:

		//��Щ��Ϣ�Ǻͻ��ŵ�ʱ��ӵ��һ���Ĵ���
		return MessageHandler(pPlane,msg);

		case GM_MSG_GM_RESURRECT:
		{
		//$$$$$$$$$�Ժ������µ���Ϣ
		//	Resurrect(0.f);
			gplayer_controller * pCtrl = (gplayer_controller *)(_commander);
			pCtrl->ResurrectInTown(0.f,msg.param);
		}
		return 0;

		case GM_MSG_SCROLL_RESURRECT:
		{
			if(!msg.param && _invader_state != INVADER_LVL_0)  
			{
				//��PVPģʽ�Ķ����ܸ�������ͷ���
				return 0;
			}
			EnterResurrectReadyState(0.05f,DEFAULT_RESURRECT_HP_FACTOR,DEFAULT_RESURRECT_MP_FACTOR);
		};
		return 0;

		case GM_MSG_ENCHANT_ZOMBIE:
		{
			__PRINTF("recv zombie enchant\n");
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if(!ech_msg.helpful)
			{
				return 0;
			}
			else
			{
				if(!TestHelpfulEnchant(msg,ech_msg)) return 0;
			}
			HandleEnchantMsg(pPlane,msg,&ech_msg);
		}
		return 0;

	}
	return 0;
}

void 
gplayer_imp::CancelSwitch()
{
	if(_player_state == PLAYER_WAIT_SWITCH)
	{
		FromSwitchToNormal();
	}
}

int 
gplayer_imp::WaitingSwitchServer(world * pPlane, const MSG & msg)
{
	//ֻ��������
	switch(msg.message)
	{
		case GM_MSG_SWITCH_GET:
		__PRINTF("�ܵ�ת�Ʒ���������Ϣ\n");
		return DoSwitch(msg);

		case GM_MSG_SWITCH_FAILED:
		{
			_runner->error_message(msg.param);
			gplayer *pPlayer = GetParent();
			GMSV::SendSwitchServerCancel(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid);
			//�ص���ͨ״̬
			__PRINTF("ת�Ʒ��������־ܾ� �� �ص�����״̬\n");

			FromSwitchToNormal();
			return 0;
		}

		case GM_MSG_HEARTBEAT:
		if(--_general_timeout<=0)
		{
			//״̬��ʱ��
			gplayer *pPlayer = GetParent();
			GMSV::SendSwitchServerCancel(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid);
			//�ص���ͨ״̬
			__PRINTF("�ȴ�ת�Ʒ�����ʱ��ʱ���ص�����״̬\n");

			FromSwitchToNormal();
		}
		default:
		return DispatchNormalMessage(pPlane,msg);
	}
}

int 
gplayer_imp::MessageHandler(world * pPlane ,const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
		{
			ASSERT(msg.content_length >= sizeof(attack_msg));

			attack_msg ack_msg = *(attack_msg*)msg.content;
			//����һ�µ����Ĺ�����Ϣ
			_filters.EF_TransRecvAttack(msg.source, ack_msg);
			
			if(!TestAttackMsg(msg,ack_msg)) return 0;

			if(TryTransmitSkillAttack(msg)) return 0;
			//����ѡ�����
			((gplayer_controller*)_commander)->TrySelect(msg.source);
			HandleAttackMsg(pPlane,msg,&ack_msg);
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			_filters.EF_TransRecvEnchant(msg.source, ech_msg);
			if(!ech_msg.helpful)
			{
				if(!TestHarmfulEnchant(msg,ech_msg)) return 0;
			}
			else
			{
				if(!TestHelpfulEnchant(msg,ech_msg)) return 0;
			}
			if(TryTransmitSkillAttack(msg)) return 0;
			HandleEnchantMsg(pPlane,msg,&ech_msg);
		}
		return 0;

		case GM_MSG_SWITCH_GET:
			return 0;
		case GM_MSG_ERROR_MESSAGE:
			_runner->error_message(msg.param);
			return 0;

		case GM_MSG_PICKUP_TEAM_MONEY:
			if(msg.content_length == sizeof(int))
			{
				int drop_id = *(int*)msg.content;
				if(drop_id)
				{
					GLog::log(LOG_INFO,"�û�%d��Ӽ����û�%d�����Ľ�Ǯ%d",_parent->ID.id,drop_id,msg.param);
				}
				if(msg.param > 0)
				{
					if(!_team.PickupTeamMoney(msg.pos,msg.param,0))
					{
						//����ַ�ʧ�� �������Լ���
						SendTo<0>(GM_MSG_RECEIVE_MONEY, GetParent()->ID, msg.param);
					}
				}
			}
			else
			{
				ASSERT(false);
			}
			return 0;
			
		case GM_MSG_PICKUP_TEAM_MONEY2:
			if(msg.content_length == sizeof(int))
			{
				int drop_id = *(int*)msg.content;
				if(drop_id)
				{
					GLog::log(LOG_INFO,"�û�%d��Ӽ����û�%d�����Ľ�Ǯ%d",_parent->ID.id,drop_id,msg.param);
				}
				if(msg.param > 0)
				{
					if(!_team.PickupTeamMoney(msg.pos,msg.param,1))
					{
						//����ַ�ʧ�� �������Լ���
						SendTo<0>(GM_MSG_RECEIVE_MONEY2, GetParent()->ID, msg.param);
					}
				}
			}
			else
			{
				ASSERT(false);
			}
			return 0;
				
		case GM_MSG_TEAM_EXPERIENCE:
			if(msg.content_length == sizeof(msg_grp_exp_t)) 
			{
				if(msg.pos.squared_distance(_parent->pos) <= (TEAM_EXP_DISTANCE * TEAM_EXP_DISTANCE))
				{
					msg_grp_exp_t * pExp = (msg_grp_exp_t*)msg.content;
					int exp = pExp->exp;
					int sp = pExp->sp;
					if(world_manager::AntiWallow())
					{
						anti_wallow::AdjustNormalExpSP(_wallow_level, exp, sp);
					}
					//��ֹ����ʱ��Ϊ0�Ķ�Ա��ȡ����ֵ
					if(_profit_level != PROFIT_LEVEL_NONE)
					{
						ReceiveExp(exp,sp);
					}
					if(pExp->level > 0)
					{
						if(_task_mask & TASK_MASK_KILL_MONSTER)
						{
							PlayerTaskInterface  task_if(this);
							OnTaskTeamKillMonster(&task_if,msg.param, pExp->level,pExp->rand);
						}
						//�����ɱ�ù���,��������ӿ� 
					}
				}
			}
			else
			{
				ASSERT(false && "����ֵ��Ϣ�����ݴ�С����ȷ");
			}
			return 0;
		case GM_MSG_EXPERIENCE:
			if(msg.content_length == sizeof(msg_exp_t))
			{
				if(msg.pos.squared_distance(_parent->pos) <= (NORMAL_EXP_DISTANCE * NORMAL_EXP_DISTANCE))
				{
					ReceiveExp(*(msg_exp_t*)msg.content);
				}
			}
			else
			{
				ASSERT(false && "����ֵ��Ϣ�����ݴ�С����ȷ");
			}
			return 0;
		case GM_MSG_GROUP_EXPERIENCE:
			if(msg.content_length && (msg.content_length % sizeof(msg_grpexp_t)) == 0)
			{
				int count = msg.content_length / sizeof(msg_grpexp_t);
				ASSERT(msg.param > 0);
				ASSERT(count > 2);
				msg_grpexp_t * pExp = (msg_grpexp_t*)msg.content;
				if(_team.IsLeader() && _team.GetTeamSeq() == pExp->damage)
				{	
					//�ڵ�һ��Ԫ�����汣���˾���ֵ������ֵ��seq
					//�ڵڶ���Ԫ�����汣����world tag��ֵ
					//�����message.h
					msg_grpexp_t *pExp = (msg_grpexp_t *) msg.content;
					int exp = pExp->who.type;
					int sp = pExp->who.id & 0xFFFFFF;
					int level = (pExp->who.id >> 24) & 0x00FF;
					float r = *(float*)&(pExp[1].who.id);
					ReceiveGroupExp(msg.pos,msg.param,exp,sp,level,count-2,pExp[1].who.type, pExp[1].damage & 0xFFFF, (pExp[1].damage >> 16) & 0xFFFF, r, pExp+2);
				}
			}
			else
			{
				ASSERT(false && "����ֵ��Ϣ�����ݴ�С����ȷ");
			}
			return 0;
			

		case GM_MSG_HATE_YOU:
			ActiveCombatState(true);
			SetCombatTimer(NORMAL_COMBAT_TIME);
			((gplayer_controller*)_commander)->TrySelect(msg.source);

			if(_enemy_list.size() < MAX_PLAYER_ENEMY_COUNT)
			{
				_enemy_list[msg.source.id] ++;
			}
			else
			{
				ENEMY_LIST::iterator it = _enemy_list.find(msg.source.id);
				if(it != _enemy_list.end())
				{
					it->second ++;
				}
			}

			return 0;


		case GM_MSG_TEAM_INVITE:
			ASSERT(msg.content_length == sizeof(int));
			if(msg.content_length == sizeof(int) && !_team.MsgInvite(msg.source,msg.param,*(int*)msg.content))
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_TEAM_CANNOT_INVITE);

			}
			return 0;

		case GM_MSG_TEAM_AGREE_INVITE:
			ASSERT(msg.content_length == sizeof(player_team::agree_invite_entry));
			if(msg.content_length == sizeof(player_team::agree_invite_entry))
			{
				_team.MsgAgreeInvite(msg.source,msg.pos,*(player_team::agree_invite_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_TEAM_REJECT_INVITE:
			_team.MsgRejectInvite(msg.source);
			return 0;

		case GM_MSG_JOIN_TEAM:
			{
				int count = msg.param & 0x7FFF;
				int pickup_flag = (msg.param >> 16 ) & 0x7FFF;
				unsigned int header_size = count * sizeof(player_team::member_entry);
				//ASSERT(msg.content_length == header_size + _cur_tag_counter.size()*sizeof(int)*3 + sizeof(int64_t));
				if(msg.content_length == header_size +  _cur_tag_counter.size()*sizeof(int)*3 + sizeof(int64_t))
				{
					
					void * buf = ((char*)msg.content) + header_size;
					int64_t team_uid = *((int64_t*)buf);
					buf = ((char*)msg.content) + header_size + sizeof(int64_t);
					unsigned int size = msg.content_length - header_size - sizeof(int64_t);
					_team.MsgJoinTeam(msg.source,(player_team::member_entry*)msg.content,
							count,pickup_flag,team_uid,buf,size);
				}
			}
			return 0;
		case GM_MSG_LEADER_UPDATE_MEMBER:
			ASSERT(msg.content_length == msg.param * sizeof(player_team::member_entry));
			if(msg.content_length == msg.param * sizeof(player_team::member_entry))
			{
				_team.MsgLeaderUpdateData(msg.source,
						(player_team::member_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_JOIN_TEAM_FAILED:
			_team.MsgJoinTeamFailed(msg.source);
			return 0;

		case GM_MSG_MEMBER_NOTIFY_DATA:
			ASSERT(msg.content_length == sizeof(team_mutable_prop));
			if(msg.content_length == sizeof(team_mutable_prop))
			{
				_team.MsgMemberUpdateData(msg.source,msg.pos,*(team_mutable_prop*)msg.content);
			}
			return 0;
			
		case GM_MSG_NEW_MEMBER:
			ASSERT(msg.content_length == msg.param * sizeof(player_team::member_entry));
			if(msg.content_length == msg.param * sizeof(player_team::member_entry))
			{
				_team.MsgNewMember(msg.source,(player_team::member_entry*)msg.content,msg.param);
			}
			return 0;

		case GM_MSG_LEAVE_PARTY_REQUEST:
			_team.MsgMemberLeaveRequest(msg.source);
			return 0;

		case GM_MSG_LEADER_CANCEL_PARTY:
			_team.MsgLeaderCancelParty(msg.source,msg.param);
			return 0;

		case GM_MSG_MEMBER_NOT_IN_TEAM:
			_team.MsgNotifyMemberLeave(msg.source,_parent->ID,0);
			return 0;

		case GM_MSG_LEADER_KICK_MEMBER:
			_team.MsgLeaderKickMember(msg.source,XID(GM_TYPE_PLAYER,msg.param));
			return 0;

		case GM_MSG_MEMBER_LEAVE:
			_team.MsgNotifyMemberLeave(msg.source,XID(GM_TYPE_PLAYER,msg.param));
			return 0;
/*	�����Ѿ�û��ʹ����
		case GM_MSG_GET_MEMBER_POS:
			_runner->teammate_get_pos(msg.source,*(int*)(msg.content), msg.param);
			return 0;
 */

		case GM_MSG_QUERY_PLAYER_EQUIPMENT:
			{
				float ox = msg.pos.x - _parent->pos.x;
				float oz = msg.pos.z - _parent->pos.z;
				if(ox*ox + oz*oz <= GET_EQUIP_INFO_DIS*GET_EQUIP_INFO_DIS)
				{
					_runner->send_equipment_info(msg.source,*(int*)(msg.content), msg.param);
				}
			}
			return 0;
		case  GM_MSG_TEAM_PICKUP:
			{
				_runner->team_member_pickup(msg.source,msg.param,*(int*)msg.content);
			}
			return 0;
		case  GM_MSG_SERVICE_GREETING:
			_provider.id = msg.source;
			_provider.pos = msg.pos;
			_provider.id_mafia = msg.param;
			_runner->npc_greeting(msg.source);
			return 0;
		
		//DEADRAKY
		case  GM_MSG_SERVICE_REQUEST:
		{
			if(_parent->IsZombie() || _player_state == PLAYER_STATE_MARKET) return 0;
			//ASSERT(!_parent->IsZombie() && "��������״̬�����κη���");
			int service_type = msg.param;
			service_executor *executor = service_manager::GetExecutor(service_type);
			if(executor)
			{
				executor->Serve(this,msg.source,msg.pos,msg.content,msg.content_length);
			}
		}
		return 0;
		//DEADRAKY
		case  GM_MSG_SERVICE_DATA:
		{
			if(_parent->IsZombie() || _player_state == PLAYER_STATE_MARKET) return 0;
			//ASSERT(!_parent->IsZombie() && "��������״̬�����κη���");
			int service_type = msg.param;
			service_executor *executor = service_manager::GetExecutor(service_type);
			if(executor)
			{
				executor->Serve(this,msg.source,msg.pos,msg.content,msg.content_length);
			}
		}
		return 0;
		
		case  GM_MSG_NPC_BE_KILLED:
		{
			//�����������ٵ����� 
			ASSERT(msg.content_length == sizeof(int));
			int level = *(int*)msg.content;
			if(_invader_state && level >= _basic.level)
			{
				_invade_ctrl.ReducePariah(PARIAH_TIME_REDUCE);
			}

			//�õ���ɱ��npc����Ϣ
			if(_task_mask & TASK_MASK_KILL_MONSTER)
			{
				__PRINTF("ɱ���˹���%d\b",msg.param);
				//��������ϵͳ�Ĳ���
				PlayerTaskInterface  task_if(this);
				OnTaskKillMonster(&task_if,msg.param, level,abase::RandUniform(),0,0);
			}

			//���������ʱ��,������Ҳ������
			if(_profit_level != PROFIT_LEVEL_NONE)
			{
				_petman.KillMob(this,level);
			}
		}
		return 0;

		case GM_MSG_PLAYER_TASK_TRANSFER:
		{
			__PRINTF("�ֵ�������Ҵ�����������Ϣ\n");
			PlayerTaskInterface  task_if(this);
			OnPlayerNotify(&task_if,msg.source.id,msg.content,msg.content_length);
		}
		return 0;

		case GM_MSG_PLAYER_BECOME_INVADER:
		{
			__PRINTF("%d��ɷ����� \n",_parent->ID.id);
			if(!_nonpenalty_pvp_state && !world_manager::GetWorldFlag().nonpenalty_pvp_flag) _invade_ctrl.BecomeInvader(msg.source,msg.param);
		}
		return 0;

		case GM_MSG_PLAYER_BECOME_PARIAH:
		{
			__PRINTF("%d��ɺ����� \n",_parent->ID.id);
			if(!_nonpenalty_pvp_state && !world_manager::GetWorldFlag().nonpenalty_pvp_flag) _invade_ctrl.BecomePariah();
		}
		return 0;

		case GM_MSG_QUERY_SELECT_TARGET:
		{
			SendTo<0>(GM_MSG_NOTIFY_SELECT_TARGET,msg.source,
					((gplayer_controller*)_commander)->GetCurTarget().id);
		}
		return 0;
		
		case GM_MSG_NOTIFY_SELECT_TARGET:
		{
			((gplayer_controller*)_commander)->SelectTarget(msg.param);
		}
		return 0;

		case GM_MSG_NOTIFY_SELECT_SUBTARGET:
		{
			((gplayer_controller*)_commander)->SelectSubTarget(msg.source,msg.param);
		}
		return 0;

		case GM_MSG_SUBSCIBE_CONFIRM:
		{
			((gplayer_controller*)_commander)->SubscibeConfirm(msg.source);
		}
		return 0;

		case GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM:
		{
			((gplayer_controller*)_commander)->SecondSubscibeConfirm(msg.source);
		}
		return 0;

		case GM_MSG_GATHER_REPLY:
		{
			//�ܵ����Կ��ɵ�֪ͨ
			if(HasSession())
			{
				//ȡ���ɼ�
				SendTo<0>(GM_MSG_GATHER_CANCEL,msg.source,0);
			}
			else
			{
				//��ʼ�ɼ�
				session_gather *pSession = new session_gather(this);
				bool can_be_interrupted = true;
				int eliminate_tool = -1;
				unsigned short gather_time_min=0,gather_time_max=0;
				if(msg.content_length == sizeof(gather_reply))
				{
					gather_reply * pr = (gather_reply*)msg.content;
					can_be_interrupted = pr->can_be_interrupted;
					eliminate_tool = pr->eliminate_tool;
					gather_time_min = pr->gather_time_min;
					gather_time_max = pr->gather_time_max;
				}
				if(eliminate_tool != -1)
				{
					//ֻ����Ҫɾ����Ʒ��������Ʒ��
					pSession->LockInventory();
				}
				int use_time_min = gather_time_min & 0xFF;
				int use_time_max = gather_time_max & 0xFF;
				int use_time = abase::Rand(use_time_min, use_time_max);
				pSession->SetTarget(msg.source.id,use_time,can_be_interrupted);
				AddSession(pSession);
				StartSession();
			}
		}
		return 0;

		case GM_MSG_GATHER_RESULT:
		{
			//�յ����ɽ��
			ASSERT(msg.content_length == sizeof(gather_result));
			gather_result * res = (gather_result*)msg.content;
			if(res->eliminate_tool > 0)
			{
				//�����Ʒ�Ƿ���ڣ�������ڲ��ܽ��д˲�����������Ʒ�Ļ���ɾ������Ʒ
				int rst = _inventory.Find(0,res->eliminate_tool);
				if(rst >= 0)
				{
					item& it = _inventory[rst];
					UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
					
					_inventory.DecAmount(rst,1);
					_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,
							res->eliminate_tool,1,S2C::DROP_TYPE_USE);
				}
				else
				{
					//�޷��ɼ�
					_runner->error_message(S2C::ERR_MINE_GATHER_FAILED);
					return 0;
				}
			}

			int item_id = msg.param;
			unsigned int count = res->amount;
			int task_id = res->task_id;
			int life = res->life;
			int mine_type = res->mine_type;
			if(item_id > 0 && count > 0)
			{
				
				element_data::item_tag_t tag = {element_data::IMT_NULL,0};
				item_data * data = world_manager::GetDataMan().generate_item_for_drop(item_id,&tag,sizeof(tag));
				if(data)
				{
					//FirstAcquireItem(data);
					if(count > data->pile_limit) count = data->pile_limit;
					GLog::log(GLOG_INFO,"�û�%d�ɼ��õ�%d��%d",_parent->ID.id, count,item_id);

					world_manager::TestCashItemGenerated(data->type, count);
					
					data->count = count;
					if (life)
						data->expire_date = g_timer.get_systime() + life; 
					int rst = _inventory.Push(*data);
                    FirstAcquireItem(data);

					
					//edit by ljj
					if(rst >=0)
					{
						_runner->obtain_item(item_id,data->expire_date,count - data->count,_inventory[rst].count, 0,rst);
						
						if(data->proc_type & item::ITEM_PROC_TYPE_AUTO_USE)
						{
							UseItem(_inventory, rst, IL_INVENTORY, data->type, 1);
						}
						
					}
					//
					
					
					if(data->count)
					{
						_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
						//ʣ������Ʒ�������Ԫ����䣬ֻ����30�룬��owner_timeͬ��
						int matter_life = 0;
						if (gmatter_mine_imp::MINE_TYPE_SOUL == mine_type)
							matter_life = MATTER_ITEM_SOUL_LIFE;
						DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,matter_life);
						//�����������Ҫ�ͷ��ڴ�
						//���������ж�
					}
					else
					{
						FreeItem(data);
					}
				}
				else
				{
					//��Ʒ����ʧ��
					_runner->error_message(S2C::ERR_MINE_GATHER_FAILED);
				}
			}

			if(task_id > 0)
			{
				//����󣬴��ݸ�����ϵͳ
				PlayerTaskInterface  task_if(this);
				OnTaskMining(&task_if,task_id);
			}

			if(gmatter_mine_imp::MINE_TYPE_SOUL == mine_type)
			{
				_player_fatering.AddGainTimes();
			}

			world_manager::GetInstance()->OnMineGathered(_plane, res->mine_tid, GetParent());
		}
		return 0;

		case GM_MSG_TEAM_APPLY_PARTY:
		{
			_team.ApplyParty(msg.source);
		}
		return 0;
		
		case GM_MSG_TEAM_CHANGE_TO_LEADER:
		{
			_team.ChangeToLeader(msg.source);
		}
		return 0;

		case GM_MSG_TEAM_LEADER_CHANGED:
		{
			_team.LeaderChanged(msg.source);
		}
		return 0;

		case GM_MSG_TEAM_APPLY_REPLY:
		{
			_team.ApplyPartyReply(msg.source,msg.param);
		}
		return 0;

		case GM_MSG_CON_EMOTE_REQUEST:
		{
			//�յ����ض����������󣬷��͸��ͻ���
			_runner->concurrent_emote_request(msg.source.id, msg.param & 0xFFFF);
		}
		return 0;

		case GM_MSG_CON_EMOTE_REPLY:
		{
			if(msg.param & 0xFFFF) 
			{
				if(_con_emote_target == msg.source.id && ((msg.param >> 16)&0xFFFF) == _con_emote_id)
				{
					_runner->do_concurrent_emote(_con_emote_target,_con_emote_id);
					ClearConcurrentEmote();
				}
			}
			else
			{
				if(_con_emote_target == msg.source.id)
				{
					_runner->error_message(S2C::ERR_CONCURRENT_EMOTE_REFUSED);
					ClearConcurrentEmote();
				}
			}
		}
		return 0;

		case GM_MSG_GM_RECALL:
		{
			LongJump(msg.pos,msg.param);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_MQUERY_MOVE_POS_REPLY:
		{
			ASSERT(sizeof(instance_key) == msg.content_length);
			//����򵥵�long jump��������������
			GLog::log(GLOG_INFO,"GM:%d�ƶ������%d��,tag:%d,(%f,%f,%f)",_parent->ID.id,msg.source.id,msg.param,msg.pos.x,msg.pos.y,msg.pos.z);
			if(msg.param == world_manager::GetWorldTag())
			{
				//ͬһ����������ת����Σ�
				//����������Ҳûɶ����,�������Լ���Ŀ�겻��һ��world��ʱ������Ŀ��
				/*if(!world_manager::GetInstance()->IsUniqueWorld())
				{
					_runner->error_message(S2C::ERR_CAN_NOT_JUMP_BETWEEN_INSTANCE);
				}
				else*/
				{
					LongJump(msg.pos,msg.param);
				}
				return 0;
			}

			//ִ�и�����ת�߼�
			instance_key key = *(instance_key*)msg.content;
			key.special_mask = IKSM_GM;

			//��Player���и������� 
			if(world_manager::GetInstance()->PlaneSwitch(this ,msg.pos,msg.param,key,0) < 0)
			{
				_runner->error_message(S2C::ERR_CANNOT_ENTER_INSTANCE);
			}
		}
		return 0;

		case GM_MSG_GM_MQUERY_MOVE_POS:
		{	
			int world_tag = world_manager::GetWorldTag();
			instance_key key;
			GetInstanceKey(world_tag,key);
			key.target = key.essence;
			if(world_manager::GetInstance()->IsBattleWorld())
			{
				key.target.key_level4 = _plane->w_ins_key.key1;
			}			
			else if(world_manager::GetInstance()->IsFactionWorld())
			{
				key.target.key_level3 = _plane->w_ins_key.key1;
			}
			else if(world_manager::GetInstance()->IsCountryBattleWorld())
			{
				key.target.key_level4 = _plane->w_ins_key.key1;
			}
			else if(world_manager::GetInstance()->GetWorldType() == WORLD_TYPE_PARALLEL_WORLD)
			{
				key.target.key_level4 = _plane->w_ins_key.key1;
			}
			else if(world_manager::GetInstance()->GetWorldType() == WORLD_TYPE_TRICKBATTLE)
			{
				key.target.key_level4 = _plane->w_ins_key.key1;
			}
// arena of aurora
			else if (world_manager::GetInstance()->GetWorldType() == WORLD_TYPE_ARENAOFAURORA)
			{
				key.target.key_level4 = _plane->w_ins_key.key1;
			}
// end arena of aurora
			SendTo<0>(GM_MSG_GM_MQUERY_MOVE_POS_REPLY,msg.source,world_manager::GetWorldTag(),&key,sizeof(key));
		}
		return 0;
		
		case GM_MSG_GM_CHANGE_EXP:
		{
			return 0;	//��ʱ��ֹ��
			int exp = msg.param;
			int sp = *(int*)msg.content;
			if(exp <0 ) exp = 0;
			if(sp < 0) sp = 0;
			if(exp > 100000000) exp = 100000000;
			if(sp > 100000000) sp = 100000000;
			ReceiveExp(exp,sp);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_ENDUE_ITEM:
		{
			return 0;	//��ʱ��ֹ��
			if(_inventory.GetEmptySlotCount() <=0)
			{
				//���ͻ�����Ϣ....
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_INVENTORY_IS_FULL);
				return 0;
			}
			int item_id = msg.param;
			unsigned int count  = *(int*)msg.content;
			element_data::item_tag_t tag = {element_data::IMT_NULL,0};
			item_data * data = world_manager::GetDataMan().generate_item_for_shop(item_id,&tag,sizeof(tag));
			if(!data) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_PRODUCE_FAILED);
				return 0;
			}
			if(count > data->pile_limit) count = data->pile_limit;
			data->count = count;
			if(ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_RESET_PP:
		{
			_basic.status_point += player_template::Rollback(GetPlayerClass(),_base_prop);
			//ʹ��װ����Ч
			RefreshEquipment();
			GlyphEquipRefresh();
			//����װ�������ݣ������˿���
			CalcEquipmentInfo();

			//���Լ���Ϣ
			PlayerGetProperty();
		}
		return 0;

		case GM_MSG_GM_ENDUE_SELL_ITEM:
		{
			return 0;	//��ʱ��ֹ��
			if(_inventory.GetEmptySlotCount() <=0)
			{
				//���ͻ�����Ϣ....
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_INVENTORY_IS_FULL);
				return 0;
			}
			int item_id = msg.param;
			unsigned int count  = *(int*)msg.content;
			const void *pBuf = world_manager::GetDataMan().get_item_for_sell(item_id);
			if(!pBuf) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_PRODUCE_FAILED);
				return 0;
			}
			item_data * data = DupeItem(*(const item_data*)pBuf);
			if(count > data->pile_limit) count = data->pile_limit;
			data->count = count;
			if(ObtainItem(gplayer_imp::IL_INVENTORY,data)) FreeItem(data);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_REMOVE_ITEM:
		{
			int item_id = msg.param;
			unsigned int num = *(int*)msg.content;
			if(item_id < 0) 
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_ITEM_NOT_IN_INVENTORY);
				return 0;
			}
			int rst = 0;
			while(num && (rst = _inventory.Find(rst,item_id)) >= 0)
			{
				unsigned int count = num;
				if(_inventory[rst].count < count) count = _inventory[rst].count;
				_inventory.DecAmount(rst,count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,item_id,count,S2C::DROP_TYPE_GM);
				num -= count;
				rst ++;
			}
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_ENDUE_MONEY:
		{
			return 0;	//��ʱ��ֹ��
		}
		return 0;

		case GM_MSG_GM_OFFLINE:
		{
			LostConnection(gplayer_imp::PLAYER_OFF_LPG_DISCONNECT);
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_DEBUG_COMMAND:
		{
			gplayer_controller * pCtrl = ((gplayer_controller*)_commander);
			pCtrl->SetDebugMode(!pCtrl->GetDebugMode());
			SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,0);
		}
		return 0;

		case GM_MSG_GM_QUERY_SPEC_ITEM:
		{
			ASSERT(msg.content_length == sizeof(msg_query_spec_item_t));
			msg_query_spec_item_t & data = *(msg_query_spec_item_t*)msg.content;
			raw_wrapper ar;
			FindSpecItem(IL_INVENTORY, data.type, ar);
			FindSpecItem(IL_EQUIPMENT, data.type, ar);
			FindSpecItem(IL_TASK_INVENTORY, data.type, ar);
			FindSpecItem(IL_TRASH_BOX, data.type, ar);
			FindSpecItem(IL_TRASH_BOX2, data.type, ar);
			FindSpecItem(IL_TRASH_BOX3, data.type, ar);
			FindSpecItem(IL_TRASH_BOX4, data.type, ar);
			FindSpecItem(IL_TRASH_BOX5, data.type, ar);
			FindSpecItem(IL_TRASH_BOX6, data.type, ar);
			FindSpecItem(IL_USER_TRASH_BOX, data.type, ar);
			_runner->send_spec_item_list(data.cs_index, data.cs_sid, msg.source.id, data.type, ar.data(), ar.size());
		}
		return 0;

		case GM_MSG_GM_REMOVE_SPEC_ITEM:
		{
			ASSERT(msg.content_length == sizeof(msg_remove_spec_item_t));
			msg_remove_spec_item_t & data = *(msg_remove_spec_item_t*)msg.content;
			int ret = RemoveSpecItem(data.where, data.index, data.count, data.type);
			_runner->send_error_message(data.cs_index, data.cs_sid, msg.source.id, ret);
		}
		return 0;

		case GM_MSG_PLANE_SWITCH_REPLY:
		{
			//����Ϣֻ��NORMAL״̬�¿��Խ��д���
			if(_player_state != PLAYER_STATE_NORMAL 
					&& _player_state != PLAYER_STATE_BIND
					&& _player_state != PLAYER_SIT_DOWN
					&& _player_state != PLAYER_STATE_MARKET)
			{
				return 0;
			}
			LeaveAbnormalState();

			if(msg.source.id == world_manager::GetWorldIndex())
			{
				//������Ϣת�����Ʋ������ظ�ת����Ϣ�����账��
				return 0;
			}
			ASSERT(msg.content_length == sizeof(instance_key));
			instance_key * key = (instance_key*)msg.content;
			SwitchSvr(msg.source.id,_parent->pos,msg.pos,key);
		}
		return 0;

		case GM_MSG_LEAVE_COSMETIC_MODE:
		{	
			if(_player_state == PLAYER_STATE_COSMETIC) LeaveCosmeticMode(msg.param); //��ʱ״̬���ܲ���
		}
		return 0;

		case GM_MSG_DBSAVE_ERROR:
		{
			_db_save_error ++;
			if(_db_save_error >= 3)
			{
				GLog::log(GLOG_ERR,"�û�%d���ڴ��̴���������࣬��ǿ��ִ�������߼�",_parent->ID.id);
				LostConnection(PLAYER_OFF_LPG_DISCONNECT);
			}
		}
		return 0;

		case GM_MSG_HURT:
		{
			if(!_parent->IsZombie()) 
			{
				ASSERT(msg.content_length == sizeof(msg_hurt_extra_info_t));
				msg_hurt_extra_info_t & data = *(msg_hurt_extra_info_t*)msg.content;
				int damage = msg.param;
				if(damage > _basic.hp)
				{
					if(!msg.source.IsPlayerClass())
					{
						//���������ҹ����������Ļ�����ô�ж϶Է��Ƿ�����
						//����Ѿ������������������
						world::object_info info;
						enum
						{
							ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE
						};
						if(!_plane->QueryObject(msg.source,info)  ||
							((info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE))
						{
							//���������������������������
							__PRINTF("�˺����뷢��\n");
							damage >>= 1;
						}
					}
				}

				DoDamage(damage);
				if(_basic.hp == 0)
				{
					Die(msg.source,data.orange_name && _invader_state == INVADER_LVL_0,data.attacker_mode,0);
				}
			}
		}
		return 0;

		case GM_MSG_ENABLE_PVP_DURATION:
		{
			__PRINTF("%d����PK״̬\n",_parent->ID.id);
			SetPVPCombatState();
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_REQUEST:
		{
			//������ 
			if(_parent->pos.squared_distance(msg.pos) <= 400)
			{
				_duel.MsgDuelRequest(this,msg.source);
			}
			else
			{
				SendTo<0>(GM_MSG_PLAYER_DUEL_REPLY,msg.source,player_duel::DUEL_REPLY_OUT_OF_RANGE);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 400)
			{
				_duel.MsgDuelReply(this,msg.source,msg.param);
			}
			else
			{
				SendTo<0>(GM_MSG_PLAYER_DUEL_CANCEL,msg.source,0);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_PREPARE:
		{
			_duel.MsgDuelPrepare(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_START:
		{
			if(msg.source != _parent->ID)
			{
				_duel.MsgDuelStart(this,msg.source);
			}
			else
			{
				ASSERT(msg.content_length == sizeof(XID));
				_duel.MsgDuelStart(this,*(XID *)msg.content);
			}
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_CANCEL:
		{
			_duel.MsgDuelCancel(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_DUEL_STOP:
		{
			if(msg.source != _parent->ID)
			{
				_duel.MsgDuelStop(this,msg.source,msg.param);
			}
			else
			{
				ASSERT(msg.content_length == sizeof(XID));
				_duel.MsgDuelStop(this,*(XID *)msg.content,msg.param);
			}
		}
		return 0;

		case GM_MSG_PLAYER_BIND_REQUEST:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgRequest(this,msg.source, msg.content, msg.content_length);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_INVITE:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgInvite(this,msg.source, msg.content, msg.content_length);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_REQUEST_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgRequestReply(this,msg.source,msg.param);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_INVITE_REPLY:
		{
			if(_parent->pos.squared_distance(msg.pos) <= 100)
			{
				_bind_player.MsgInviteReply(this,msg.source,msg.param);
			}
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_PREPARE:
		{
			_bind_player.MsgPrepare(this,msg.source);
		}
		return 0;

		case GM_MSG_PLAYER_BIND_LINK:
		{
			_bind_player.MsgBeLinked(this,msg.source,msg.pos);
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_STOP:
		{
			_bind_player.MsgStopLinked(this,msg.source);
		}
		return 0;
		
		case GM_MSG_PLAYER_BIND_FOLLOW:
		{
			//50���ڲ���follow
			if(_parent->pos.squared_distance(msg.pos) <= 50.f*50.f)
			{
				_bind_player.MsgFollowOther(this,msg.source,msg.pos);
			}
		}
		return 0;

		case GM_MSG_QUERY_EQUIP_DETAIL:
		{
			if(GetEnemyFaction() & msg.param)
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_CANNOT_QUERY_ENEMY_EQUIP);
				return 0;
			}
			float dis = msg.pos.squared_distance(_parent->pos);
			if(dis <= 30*30)
			{
				ASSERT(msg.content_length == sizeof(int)*2);
				int * tmp = (int*)(msg.content);
				int cs_index = tmp[0];
				int cs_sid = tmp[1];

				static const  int query_list[] = {
					item::EQUIP_INDEX_WEAPON,
					item::EQUIP_INDEX_HEAD,	
					item::EQUIP_INDEX_NECK,
					item::EQUIP_INDEX_SHOULDER,
					item::EQUIP_INDEX_BODY,	
					item::EQUIP_INDEX_WAIST,
					item::EQUIP_INDEX_LEG,
					item::EQUIP_INDEX_FOOT,
					item::EQUIP_INDEX_WRIST,
					item::EQUIP_INDEX_FINGER1,
					item::EQUIP_INDEX_FINGER2,
					item::EQUIP_INDEX_FLYSWORD,
					item::EQUIP_INDEX_FASHION_BODY,
					item::EQUIP_INDEX_FASHION_LEG,
					item::EQUIP_INDEX_FASHION_FOOT,
					item::EQUIP_INDEX_FASHION_WRIST,
					item::EQUIP_INDEX_BIBLE,
					item::EQUIP_INDEX_BUGLE,
					item::EQUIP_INDEX_TWEAK,
					item::EQUIP_INDEX_ELF,
					item::EQUIP_INDEX_FASHION_HEAD,
					item::EQUIP_INDEX_FASHION_WEAPON,
					item::EQUIP_INDEX_GENERALCARD1,
					item::EQUIP_INDEX_GENERALCARD2,
					item::EQUIP_INDEX_GENERALCARD3,
					item::EQUIP_INDEX_GENERALCARD4,
					item::EQUIP_INDEX_GENERALCARD5,
					item::EQUIP_INDEX_GENERALCARD6,
					item::EQUIP_INDEX_ASTROLABE,
				};

				raw_wrapper rw(2048);
				_equipment.DetailSavePartial(rw,query_list,sizeof(query_list)/sizeof(int));
				_runner->send_equip_detail(cs_index, cs_sid,msg.source.id,rw.data(),rw.size());
			}
		}
		return 0;

		case GM_MSG_PLAYER_RECALL_PET:
		{
			_petman.RecallPet(this);
		}
		return 0;
		
		case GM_MSG_REMOVE_ITEM:
		{
			int item_id = msg.param;
			if(item_id <= 0) 
			{
				return 0;
			}
			int rst = 0;
			rst = _inventory.Find(rst,item_id);
			if(rst >= 0)
			{
				item& it = _inventory[rst];
				UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
				
				_inventory.DecAmount(rst,1);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,item_id,1,S2C::DROP_TYPE_GM);
			}
		}
		return 0;

		case GM_MSG_PET_RELOCATE_POS:
		{
			int dis = *(int*)msg.content;
			if(!_petman.RelocatePos(this,msg.source, msg.param,dis))
			{
				SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
			}
		}
		return 0;

		case GM_MSG_PET_NOTIFY_HP:
		{
			if(msg.content_length == sizeof(msg_pet_hp_notify))
			{
				msg_pet_hp_notify * pInfo = (msg_pet_hp_notify *) msg.content;
				if(!_petman.NotifyPetHP(this, msg.source, msg.param,*pInfo))
				{
					SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
				}
			}
		}
		return 0;

		case GM_MSG_PET_NOTIFY_DEATH:
		{
			_petman.PetDeath(this, msg.source, msg.param);
		}
		return 0;

		case GM_MSG_PET_SET_COOLDOWN:
		{
			if(msg.content_length == sizeof(int))
			{
				_petman.PetSetCoolDown(this, msg.source, msg.param,*(int*)msg.content);
			}
		}
		return 0;

		case GM_MSG_PET_ANTI_CHEAT:
		{
			OnAntiCheatAttack(0.02f);
		}
		return 0;

		case GM_MSG_MOB_BE_TRAINED:
		{
			if(_inventory.GetEmptySlotCount() > 0)
			{
				OnAntiCheatAttack(0.10f);
				int id = msg.param;
				const void *pBuf = world_manager::GetDataMan().get_item_for_sell(id);
				if(!pBuf) 
				{
					return 0;
				}
				item it;
				if(MakeItemEntry(it, *(const item_data *)pBuf))
				{
					//�Ƿ���Ҫ �������ó��ﵰ�Ŀɼ�ID $$$$$$$		
					int rst = _inventory.PushInEmpty(0,it);
					if(rst >=0)
					{
						_runner->obtain_item(id,it.expire_date,1,_inventory[rst].count, IL_INVENTORY,rst);
					}
					it.Release();
				}
			}
			else
			{
				_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			}
		}
		return 0;

		case GM_MSG_QUERY_PROPERTY:
		{
			struct S2C::CMD::player_property::_data data;
			memset(&data,0,sizeof(data));
			ASSERT(MAGIC_CLASS == 5);
			data.id					= _parent->ID.id;
			data.hp 				= _basic.hp;
			data.mp 				= _basic.mp;
			data.damage_low 		= _cur_prop.damage_low;
			data.damage_high		= _cur_prop.damage_high;
			data.damage_magic_low 	= _cur_prop.damage_magic_low;
			data.damage_magic_high 	= _cur_prop.damage_magic_high;
			data.defense 			= _cur_prop.defense;		
			data.resistance[0] 		= _cur_prop.resistance[0];
			data.resistance[1] 		= _cur_prop.resistance[1];
			data.resistance[2] 		= _cur_prop.resistance[2];
			data.resistance[3] 		= _cur_prop.resistance[3];
			data.resistance[4] 		= _cur_prop.resistance[4];
			data.attack 			= _cur_prop.attack;	
			data.armor 				= _cur_prop.armor;	
			data.attack_speed 		= _cur_prop.attack_speed;
			data.run_speed 			= _cur_prop.run_speed;	
			data.attack_degree		= _attack_degree;
			data.defend_degree		= _defend_degree;
			data.crit_rate			= _crit_rate+_base_crit_rate;
			if(data.crit_rate > 100)	data.crit_rate = 100;
			data.damage_reduce		= _damage_reduce;
			if(data.damage_reduce > 75)	data.damage_reduce = 75;
			data.prayspeed			= _skill.GetPraySpeed();
			if(data.prayspeed >= 100) data.prayspeed = 99;	//It's weird, but that's what it says on the skill side
			data.crit_damage_bonus 	= _crit_damage_bonus;
			data.invisible_degree 	= ((gplayer*)_parent)->invisible_degree;
			data.anti_invisible_degree = ((gplayer*)_parent)->anti_invisible_degree;
			data.vigour = GetVigour();
			data.anti_defense_degree = _anti_defense_degree;
			data.anti_resistance_degree = _anti_resistance_degree;
			data.highest_level = _highest_level;
			SendTo<0>(GM_MSG_QUERY_PROPERTY_REPLY,msg.source,msg.param,&data,sizeof(data));	
		}
		return 0;

		case GM_MSG_QUERY_PROPERTY_REPLY:
		{
			if(msg.param >=0)
			{
				//����Ʒ����ͻ��˷�������
				ASSERT(msg.param >=0 && (unsigned int)msg.param < _inventory.Size());
				item & it = _inventory[msg.param];
				if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_QUERYOTHERPROPERTY)
					return 0;
				UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
				_runner->player_drop_item(IL_INVENTORY, msg.param, it.type, 1, S2C::DROP_TYPE_USE);
				_inventory.DecAmount(msg.param, 1);	
			}
			
			struct S2C::CMD::player_property::_self_data self_data;
			memset(&self_data,0,sizeof(self_data));
			self_data.damage_reduce = _damage_reduce;
			if(self_data.damage_reduce > 75) self_data.damage_reduce = 75;
			self_data.prayspeed		= _skill.GetPraySpeed();
			if(self_data.prayspeed >= 100) self_data.prayspeed = 99;	//It's weird, but that's what it says on the skill side
			_runner->send_others_property(msg.content, msg.content_length, &self_data, sizeof(self_data));
		}
		return 0;
		
		case GM_MSG_NOTIFY_CLEAR_INVISIBLE:
		{
			//�յ�����ȡ����������Ϣ
			if(msg.source == _petman.GetCurPet())
			{
				if(((gplayer*)_parent)->IsInvisible())
					_filters.RemoveFilter(FILTER_INDEX_INVISIBLE);
			}			
		}
		return 0;
		
		case GM_MSG_CONTRIBUTION_TO_KILL_NPC:
		{
			ASSERT(msg.content_length == sizeof(msg_contribution_t));
			if(_task_mask & TASK_MASK_KILL_PQ_MONSTER)
			{
				if(msg.param == world_manager::GetWorldTag() 
					&& msg.pos.squared_distance(_parent->pos) <= (NORMAL_EXP_DISTANCE * NORMAL_EXP_DISTANCE))
				{
					msg_contribution_t & contri = *(msg_contribution_t*)(msg.content);
					//��������ϵͳ�Ĳ���
					PlayerTaskInterface  task_if(this);
					OnTaskKillPQMonster(&task_if,contri.npc_id,contri.is_owner,contri.team_contribution,contri.team_member_count, contri.personal_contribution);
				}
			}
		}
		return 0;
		
		case GM_MSG_GROUP_CONTRIBUTION_TO_KILL_NPC:
		{
			msg_group_contribution_t& contri = *(msg_group_contribution_t*)msg.content;
			ASSERT(msg.content_length == sizeof(msg_group_contribution_t)+contri.count*sizeof(msg_group_contribution_t::_list));
			if(_team.IsLeader())
			{
				XID mlist[TEAM_MEMBER_CAPACITY];
				float mcontri[TEAM_MEMBER_CAPACITY];
				int mlist_count = 0;
				char flag_list[TEAM_MEMBER_CAPACITY];
				memset(flag_list,0,sizeof(flag_list));
				float total_contribution = 0.f;

				for(int i=0; i<contri.count; i++)
				{
					int index;
					A3DVECTOR pos;
					int level;
					int tag;
					int plane_index;
					if((index = _team.GetMember(contri.list[i].xid, pos, level, tag, plane_index)) >= 0)
					{
						flag_list[index] = 1;
						if(tag == msg.param && pos.squared_distance(msg.pos) <= TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE)				
						{
							mlist[mlist_count] = contri.list[i].xid;
							mcontri[mlist_count] = contri.list[i].contribution;
							mlist_count ++;
							total_contribution += contri.list[i].contribution;
						}
					}
					else
					{
						msg_contribution_t data;
						data.npc_id = contri.npc_id;
						data.is_owner = false;
						data.team_contribution = contri.list[i].contribution;
						data.team_member_count = 1;
						data.personal_contribution = contri.list[i].contribution;
						MSG new_msg;
						BuildMessage(new_msg, GM_MSG_CONTRIBUTION_TO_KILL_NPC, contri.list[i].xid, msg.source, msg.pos,
							msg.param, &data, sizeof(data));
						_plane->PostLazyMessage(new_msg);
					}
				}

				int member_count = _team.GetMemberNum();
				for(int i=0; i<member_count; i++)
				{
					if(flag_list[i]) continue;
					const player_team::member_entry &ent = _team.GetMember(i);			
					if(ent.data.world_tag == msg.param 
						&& ent.pos.squared_distance(msg.pos) <= TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE)
					{
						mlist[mlist_count] = ent.id;	
						mcontri[mlist_count] = 0;
						mlist_count ++;
					}
				}
			
				if(mlist_count)
				{
					msg_contribution_t data; 
					data.npc_id = contri.npc_id;
					data.is_owner = contri.is_owner;
					data.team_contribution = total_contribution;
					data.team_member_count = mlist_count;
					MSG new_msg;
					BuildMessage(new_msg, GM_MSG_CONTRIBUTION_TO_KILL_NPC, XID(-1,-1), msg.source, msg.pos,
						msg.param, &data, sizeof(data));
					for(int i=0; i<mlist_count; i++)
					{
						new_msg.target = mlist[i];
						data.personal_contribution = mcontri[i];
						_parent->plane->PostLazyMessage(new_msg);
					}
				}
			}
		}
		return 0;

		case GM_MSG_REBUILD_TEAM_INSTANCE_KEY_REQ:
		{
			ASSERT(msg.content_length == sizeof(instance_hash_key));
			instance_hash_key * ikey = (instance_hash_key *)msg.content;
			if(_team.IsLeader() && 
					_team_ins_key_list[msg.param].first == ikey->key1 && _team_ins_key_list[msg.param].second == ikey->key2)
				ResetInstance(msg.param);
		}
		return 0;

		case GM_MSG_REBUILD_TEAM_INSTANCE_KEY:
		{
			ASSERT(msg.content_length == 2*sizeof(instance_hash_key));
			instance_hash_key * ikey = (instance_hash_key *)msg.content;
			if(_team.IsInTeam() && !_team.IsLeader() && 
					_team_ins_key_list[msg.param].first == ikey->key1 && _team_ins_key_list[msg.param].second == ikey->key2)
			{
				ikey ++;
				_team_ins_key_list[msg.param].first = ikey->key1;
				_team_ins_key_list[msg.param].second = ikey->key2;
			}
		}
		return 0;

		case GM_MSG_PLANT_PET_NOTIFY_DEATH:
		{
			_plantpetman.PlantDeath(this, msg.source, msg.param);
		}
		return 0;

		case GM_MSG_PLANT_PET_NOTIFY_HP:
		{
			if(msg.content_length == sizeof(msg_plant_pet_hp_notify))
			{
				msg_plant_pet_hp_notify * pInfo = (msg_plant_pet_hp_notify*)msg.content;
				if(!_plantpetman.NotifyPlantHP(this, msg.source, msg.param, *pInfo))
				{
					SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
				}
			}		
		}
		return 0;

		case GM_MSG_PLANT_PET_NOTIFY_DISAPPEAR:
		{
			if(!_plantpetman.PlantDisappear(this,msg.source, msg.param))
			{
				SendTo<0>(GM_MSG_PET_DISAPPEAR,msg.source,msg.param);
			}
		}
		return 0;

		case GM_MSG_CONGREGATE_REQUEST:
		{
			ASSERT(msg.content_length == sizeof(msg_congregate_req_t));
			msg_congregate_req_t & data = *(msg_congregate_req_t *)msg.content;
			RecvCongregateRequest(msg.param, msg.source.id, data.world_tag, msg.pos, data.level_req, data.sec_level_req, data.reincarnation_times_req);
		}
		return 0;

		case GM_MSG_REJECT_CONGREGATE:
		{
			_runner->reject_congregate(msg.param, msg.source.id);
		}
		return 0;

		case GM_MSG_NPC_BE_KILLED_BY_OWNER:
		{
			//�����������ٵ����� 
			ASSERT(msg.content_length == sizeof(msg_dps_dph_t));
			msg_dps_dph_t & data = *(msg_dps_dph_t*)msg.content;
			if(_invader_state && data.level >= _basic.level)
			{
				_invade_ctrl.ReducePariah(PARIAH_TIME_REDUCE);
			}

			//�õ���ɱ��npc����Ϣ
			if(_task_mask & TASK_MASK_KILL_MONSTER)
			{
				__PRINTF("ɱ���˹���%d\b",msg.param);
				//��������ϵͳ�Ĳ���
				PlayerTaskInterface  task_if(this);
				if(data.update_rank)
					OnTaskKillMonster(&task_if, msg.param, data.level, abase::RandUniform(), data.dps, data.dph);
				else
					OnTaskKillMonster(&task_if, msg.param, data.level, abase::RandUniform(), 0, 0);
			}
			
			if(data.update_rank)
			{
				//���뵽���а���
				int cls = -1;
				bool gender = false;
				GetPlayerClass(cls, gender);
				world_manager::GetInstance()->DpsRankUpdateRankInfo(_parent->ID.id, _basic.level, cls, data.dps, data.dph);
			}
		}
		return 0;

		case GM_MSG_EXCHANGE_POS:
		{
			ASSERT(msg.content_length == sizeof(A3DVECTOR));
			if(_parent->ID == msg.source)
				Teleport(*(A3DVECTOR*)msg.content, 0, 0);
			else
				Teleport(msg.pos, 0, 0);
		}
		return 0;		

		case GM_MSG_QUERY_INVENTORY_DETAIL:
		{
			ASSERT(msg.content_length == sizeof(msg_player_t));
			msg_player_t & data = *(msg_player_t*)msg.content;
		
			raw_wrapper rw(1024);
			unsigned char size;
			_inventory.DetailSave(rw);
			size = _inventory.Size();
			_runner->send_inventory_detail(data.cs_index, data.cs_sid, data.id, _money.GetGold(), size, rw.data(),rw.size());
		}
		return 0;

		case GM_MSG_PLAYER_KILLED_BY_PLAYER:
		{
			ASSERT(msg.content_length == sizeof(msg_player_killed_info_t));
			msg_player_killed_info_t & data = *(msg_player_killed_info_t*)msg.content;

			if(_task_mask & TASK_MASK_KILL_PLAYER)
			{
				PlayerTaskInterface  task_if(this);
				OnTaskKillPlayer(&task_if, data.cls, data.gender, data.level, data.force_id, abase::RandUniform());
			}
		}
		return 0;

		case GM_MSG_LONGJUMP:
		{
			ASSERT(msg.content_length == sizeof(A3DVECTOR));
			A3DVECTOR & pos = *(A3DVECTOR *)msg.content;
			LongJump(pos, msg.param);
		}
		return 0;

		case GM_MSG_MAFIA_PVP_AWARD:
		{
			ASSERT(msg.content_length == sizeof(msg_mafia_pvp_award_t));
			msg_mafia_pvp_award_t& mfa = *(msg_mafia_pvp_award_t*)msg.content;
			OnMafiaPvPAward(msg.param, msg.source, msg.pos, mfa.mafia_id, mfa.domain_id);			
		}
		return 0;
		
		case GM_MSG_REDUCE_CD:
		{
			ASSERT(msg.content_length == sizeof(msg_reduce_cd_t));
			msg_reduce_cd_t & data = *(msg_reduce_cd_t *)msg.content;
			if(!CheckCoolDown(data.skill_id))
				SetCoolDown(data.skill_id,data.msec > 0 ? data.msec : 1);
		}
		return 0;
	
		case GM_MSG_DELIVER_TASK:
		{
			PlayerTaskInterface task_if(this);
			OnTaskManualTrig(&task_if, msg.param, false);
		}
		return 0;
		
		case GM_MSG_DELIVER_STORAGE_TASK:
		{
			DATA_TYPE dt;
			const TASK_LIST_CONFIG* pConfig = (TASK_LIST_CONFIG*)(world_manager::GetDataMan().get_data_ptr(msg.param, ID_SPACE_CONFIG, dt));
			if (dt != DT_TASK_LIST_CONFIG || !pConfig) return 0;
			int sizeTasks = sizeof(pConfig->tasks) / sizeof(pConfig->tasks[0]);
			int range = 0;
			for (; range < sizeTasks; ++range)
			{
				if(0 == pConfig->tasks[range].id) break;
			}
			if(0 == range && 0 == pConfig->tasks[0].id)	return 0;
			int index = abase::Rand(0,range > 0 ? range - 1 : 0);
			
			PlayerTaskInterface task_if(this);
			OnTaskManualTrig(&task_if, pConfig->tasks[index].id, false);
		}
		return 0;

        case GM_MSG_CHANGE_GENDER_LOGOUT:
        {
            PlayerLogout(PLAYER_OFF_OFFLINE);
        }
        return 0;

 		case GM_MSG_CLEAR_TOWER_TASK:
 		{
 			PlayerTaskInterface  task_if(this);
 			ClearAllTowerTask(&task_if);//������е��˸�������
 		}
		return 0;

        case GM_MSG_LOOKUP_ENEMY:
        {
            SendTo<0>(GM_MSG_LOOKUP_ENEMY_REPLY, msg.source, world_manager::GetWorldTag());
        }
        return 0;

        case GM_MSG_LOOKUP_ENEMY_REPLY:
        {
            OnLookupEnemyReply(msg);
        }
        return 0;

	}
	return gactive_imp::MessageHandler(pPlane,msg);
}

void 
gplayer_imp::LevelUp()
{
	int cls = GetPlayerClass();
	bool is_level_up = false;
	do
	{
		int exp = player_template::GetLvlupExp(cls,_basic.level);
		if(exp > _basic.exp)
		{	
			break;
		}

		_basic.exp -= exp;
		if(!player_template::LevelUp(cls,_basic.level,_base_prop))
		{
			GLog::log(GLOG_ERR,"�û�%d����ʱ��������,ְҵ%d,����%d",_parent->ID.id,cls,_basic.level);
			PlayerForceOffline();
			return ;
		}
		_basic.level ++;
		_basic.status_point += player_template::GetStatusPointPerLevel();
		is_level_up = true;
		_runner->level_up();
		unsigned int m = _money.GetGold() + _trashbox.GetMoney();
		int tu = GetPlayEd();
		GLog::upgrade(_parent->ID.id,_basic.level,m);
		GLog::log(GLOG_INFO,"�û�%d������%d����Ǯ%d,��Ϸʱ��%d:%02d:%02d",
				_parent->ID.id, _basic.level,m,tu/3600,(tu/60)%60,tu%60);
		if(_basic.level >= player_template::GetMaxLevel())
		{
			_basic.exp = 0;
			break;
		}
	}while(1);
	if(is_level_up)
	{
		//����Ҫ���¼�����ʷ��ߵȼ�����Ϊ�������Լ�����õ�
		_player_reincarnation.CalcHistoricalMaxLevel();

		gplayer * pPlayer = GetParent();
		int prev_invisible_degree = pPlayer->invisible_degree;
		int prev_anti_invisible_degree = pPlayer->anti_invisible_degree;
		bool invisible_changed = false;
		
		property_policy::UpdatePlayer(cls,this);
		if(!pPlayer->IsZombie())
		{
			//�����Ѫ��ħ����
			_basic.hp = _cur_prop.max_hp;
			_basic.mp = _cur_prop.max_mp;
		}

		if(!world_manager::GetWorldParam().pve_mode || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
		{
			TestPKProtected();
		}
	
		if(pPlayer->IsInvisible() && pPlayer->invisible_degree > prev_invisible_degree)
		{
			//����ǰ��������״̬���������ȼ�����
			_runner->on_inc_invisible(prev_invisible_degree,pPlayer->invisible_degree);
			__PRINTF("����%d��������\n",pPlayer->invisible_degree);
			invisible_changed = true;
		}
		if(pPlayer->anti_invisible_degree > prev_anti_invisible_degree)
		{
			__PRINTF("�����ȼ�%d\n",pPlayer->anti_invisible_degree);
			_runner->on_inc_anti_invisible(prev_anti_invisible_degree,pPlayer->anti_invisible_degree);
			invisible_changed = true;
		}
		if(invisible_changed) _petman.NotifyInvisibleData(this);
		
		UpdateBaseSoulPower();
		//�ȼ������޺������޷���þ��飬��ͼ����ת������
		if(_basic.level >= player_template::GetMaxLevel()) _player_reincarnation.TryActivateTome();
		_player_reincarnation.CalcExpBonus();
		
		GMSV::SendSynMutaData(pPlayer->ID.id,_basic.level,_player_reincarnation.GetTimes());
		PlayerTaskInterface  task_if(this);
		OnTaskPlayerLevelUp(&task_if);
		_level_up = true;

		/*160+*/
		SpeedSkillManager();

		/*174+*/
		ActivityEventActivate();

		if(_activity.enable_skill)
		ActivityEventSkillManager();

		ProtocolActivityEvent();

		//QuestionTask();
	}
}

void 
gplayer_imp::ReceiveGroupExp(const A3DVECTOR &pos,int total_damage,int exp,int sp,int level,unsigned int count, int npcid,int npctag, int npc_planeindex, float r, const msg_grpexp_t * list)
{
	//���ȳ�ȥ�����Ѿ��뿪�Ķ�Ա
	int member_damage = 0;
	ASSERT(total_damage >0);
	float factor = 1.f/(float)(total_damage);
	char flag_list[TEAM_MEMBER_CAPACITY];
	memset(flag_list,0,sizeof(flag_list));
	int mlist[TEAM_MEMBER_CAPACITY];
	int mlist_count = 0;
	int max_level = 0;
	int min_level = 0xFFFF;
	int total_level = 0;
	for(unsigned int i = 0; i < count ; i ++)
	{
		int damage = list[i].damage;
		A3DVECTOR mpos;
		int level;
		int index;
		int tag;
		int plane_index;
		if((index = _team.GetMember(list[i].who,mpos,level,tag,plane_index)) >= 0)
		{
			flag_list[index] = 1;
			if(tag == npctag && plane_index == npc_planeindex && mpos.squared_distance(pos) <= TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE)
			{
				member_damage += damage;
				mlist[mlist_count] = index;
				mlist_count ++;
				if(level >= MIN_TEAM_DISEXP_LEVEL)
				{
					total_level += level;
				}
				else
				{
					total_level += MIN_TEAM_DISEXP_LEVEL;
				}

				if(level > max_level) max_level = level;
				if(level < min_level) min_level = level;

			}

		}
		else
		{
			msg_exp_t data;
			float tmp = factor * damage;
			data.level = level;
			data.exp = (int)(exp * tmp + 0.5f);
			data.sp = (int)(sp * tmp + 0.5f);
			if(data.exp > 0)
			{
				MSG msg;
				BuildMessage(msg,GM_MSG_EXPERIENCE,list[i].who,list[i].who,pos,0,&data,sizeof(data));
				_plane->PostLazyMessage(msg);
			}
		}
	}

	if(!member_damage || !mlist_count || total_level <= 0) return ;
	if(member_damage < total_damage)
	{
		factor *= member_damage;
		exp = (int)( factor * exp + 0.5f);
		sp = (int)( factor * sp + 0.5f);
	}

	//�������е�������� �м�����������Ѿ������˵�
	for(int i = 0; i < _team.GetMemberNum(); i ++)
	{
		if(flag_list[i]) continue;
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.data.world_tag != npctag || ent.data.plane_index != npc_planeindex || ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE)
		{
			continue;
		}
		mlist[mlist_count] = i;
		mlist_count ++;
		int level = ent.data.level;
		if(level >= MIN_TEAM_DISEXP_LEVEL)
		{
			total_level += level;
		}
		else
		{
			total_level += MIN_TEAM_DISEXP_LEVEL;
		}
		if(level > max_level) max_level = level;
		if(level < min_level) min_level = level;
	}

	//_team.DispatchExp(pos,exp,sp,level);
	_team.DispatchExp(pos,mlist,mlist_count, exp,sp,level,total_level,max_level, min_level,npcid,r);
}

void 
gplayer_imp::ReceiveExp(const msg_exp_t & entry)
{
	int exp = entry.exp;
	int sp = entry.sp;
	float exp_adj = 0,sp_adj = 0;
	player_template::GetExpPunishment(_basic.level - entry.level,&exp_adj,&sp_adj);
	exp = (int)(exp * exp_adj + 0.5f);
	sp = (int)(sp * sp_adj + 0.5f);
	
	if(world_manager::AntiWallow())
	{
		anti_wallow::AdjustNormalExpSP(_wallow_level, exp, sp);
	}
	
	if(exp + sp) ReceiveExp(exp,sp);
}

void 
gplayer_imp::IncExp(int &exp ,int &sp,float double_exp_sp_factor, bool double_sp)
{

	/*174+*/
	if(_double_factor_exp)
	exp = (int)(exp * DOUBLE_EXP_FACTOR);

	if(_double_factor_sp)
	sp = (int)(sp * DOUBLE_EXP_FACTOR);

	if(double_exp_sp_factor > 1.00001f || double_exp_sp_factor < 0.99999f)
	{
		exp = (int)(exp * double_exp_sp_factor);
		sp = (int)(sp * double_exp_sp_factor);
	}
	else if(double_sp)
	{
		sp = (int)(sp * DOUBLE_EXP_FACTOR);
	}
	if(sp > 2000000000 - _basic.skill_point) sp = 2000000000 - _basic.skill_point;
	_basic.skill_point += sp; 
	SetRefreshState();

	if(_player_reincarnation.IsTomeActive())
	{
		//ת�����ἤ��ʱ����������
	}
	else
	{
		if(_basic.level >= player_template::GetMaxLevel())
		{	
			//�ﵽ��󼶱�
			exp = 0;
		}
	}

	if(sp || exp)
	{
		if(double_exp_sp_factor > 1.00001f || double_exp_sp_factor < 0.99999f)
		{
			GLog::log(GLOG_INFO,"�û�%d�õ�����(���鱶��%f) %d/%d",_parent->ID.id, double_exp_sp_factor, (int)exp,(int)sp);
		}
		else
		{
			if(double_sp)
			{
				GLog::log(GLOG_INFO,"�û�%d�õ�����(˫��Ԫ��) %d/%d",_parent->ID.id,(int)exp,(int)sp);
			}
			else
			{
				GLog::log(GLOG_INFO,"�û�%d�õ����� %d/%d",_parent->ID.id,(int)exp,(int)sp);
			}
		}
	}

	if(!exp) return;
	if(_player_reincarnation.IsTomeActive())
	{
		//ת�����ἤ��ʱ����������
		_player_reincarnation.IncTomeExp(exp);
	}
	else
	{
		_basic.exp += exp;
		int next_exp = player_template::GetLvlupExp(GetPlayerClass(),_basic.level);
		if(next_exp > _basic.exp)
		{
			return ;
		}
		LevelUp();
	}
	//DEADRAKY
	if( _basic.exp < 0 ) _basic.exp = 0;
	if( _basic.exp > 2000000000 ) _basic.exp = 2000000000;
	if( _basic.skill_point < 0 ) _basic.skill_point = 0;
	if( _basic.skill_point > 2000000000 ) _basic.skill_point = 2000000000;
	return;
}

void 
gplayer_imp::ReceiveExp(int exp,int sp)
{
	/*174+*/
	if(_double_factor_exp)
	exp = (int)(exp * DOUBLE_EXP_FACTOR);

	if(_double_factor_sp)
	sp = (int)(sp * DOUBLE_EXP_FACTOR);


	if(exp < 0 ) exp = 0;
	if(exp > 2000000000 ) exp = 2000000000;
	if(sp < 0 ) sp = 0;
	if(sp > 2000000000 ) sp = 2000000000;
	if(!exp && !sp) return;
	
	//ASSERT(exp >=0 && sp >=0);
	//���㾭�飬�Ծ����������
	//������װ������ӳ�
	exp += (int)((exp * _exp_addon * 0.01f) + 0.1f);
	
	//����˫�������ж�
	float double_exp_sp_factor = 1.0f;
	if(world_manager::GetWorldParam().double_exp) 
		double_exp_sp_factor = world_manager::GetDoubleExpFactor();
	else if(_double_exp_mode) 
		double_exp_sp_factor = DOUBLE_EXP_FACTOR;
	
	//�౶�������	
	double_exp_sp_factor += _multi_exp_ctrl.GetExpFactor();
	//ת���������
	double_exp_sp_factor += _player_reincarnation.GetExpBonus();

    double_exp_sp_factor += _exp_sp_factor;
    if (double_exp_sp_factor <= 0.0f) return;

	IncExp(exp,sp,
			double_exp_sp_factor, 
			world_manager::GetWorldParam().double_sp);
	_runner->receive_exp(exp,sp);
	//С�����þ���
	if(exp/10 > 0)
		ElfReceiveExp((unsigned int)(exp/10));
	return;
}

int
gplayer_imp::DoSwitch(const MSG & msg)
{
	ASSERT(msg.source.type == GM_TYPE_SERVER);
	if(msg.source.type != GM_TYPE_SERVER || msg.source.id != _switch_dest || msg.content_length != sizeof(_instance_switch_key)) 
	{
		GLog::log(GLOG_WARNING,"switch:����ķ�����������Ҫ��ת���û�(%d)������",_parent->ID.id);
		return 0;
	}

	//���key�Ƿ�ƥ�� �����ƥ�䣬ֱ�ӷ���
	if(memcmp(&_instance_switch_key, msg.content, sizeof(_instance_switch_key)))
	{
		//key��ƥ��
		GLog::log(GLOG_WARNING,"switch:����ķ�����������Ҫ��ת���û�(%d)������(Key��ƥ��)",_parent->ID.id);
		return 0;
	}

	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("�û�%d��ʼ������ݽ��з�����ת��:%u.%u\n",_parent->ID.id,tv.tv_sec,tv.tv_usec);
	
	int tag = msg.param;
	if(tag != world_manager::GetWorldTag() || _switch_pos.squared_distance(_parent->pos) > 125.f*125.f)
	{
		//���ڳ�Զ����ת�ƣ�����һ����ʧ����
		_runner->disappear();
	}
	
	PlayerLeaveServer();

	//���ת��Ŀ�겻�ǵ�ǰ��tag�� ��ô�����ǰ��session
	if(tag != world_manager::GetWorldTag())
	{
		ClearSession();
		ClearAction();
		//ɾ���л������������filter
		_filters.ClearSpecFilter(filter::FILTER_MASK_NOSAVE);
	}

	//���û������ݴ�������ͳ�ȥ
	gplayer * pPlayer = GetParent();
	raw_wrapper wrapper(4096);
	wrapper.SetLimit(raw_wrapper::SAVE_ONLY);
	wrapper << _instance_switch_key << world_manager::GetWorldTag();
	pPlayer->Export(wrapper);
	WrapObject(wrapper,_commander,this,_runner);

	MSG reply;
	BuildMessage(reply, GM_MSG_SWITCH_USER_DATA,msg.source,msg.target,
			 _switch_pos,_instance_switch_key.target.key_level1,wrapper.data(),wrapper.size());
	_plane->SendRemoteMessage(msg.source.id,reply);

	//���г����֪ͨ����
	_petman.PreSwitchServer(this);
	_plantpetman.PreSwitchServer(this);

	//���session
	ResetSession();

	//�ڱ�����������û����ⲿ����
	//ֻ��Ŀ��gs�뵱ǰgs����ͬһ����ͼ�Ž��д˲��� 20110117
	if(tag == world_manager::GetWorldTag())
	{
		extern_object_manager::object_appear app;
		app.body_size = pPlayer->body_size;
		app.race = pPlayer->base_info.race;
		app.faction = pPlayer->base_info.faction;
		app.level= pPlayer->base_info.level;
		app.hp= pPlayer->base_info.hp;
		app.state = pPlayer->IsZombie();
		app.where = _switch_dest;
		_plane->GetExtObjMan().Refresh(pPlayer->ID.id,_switch_pos,app);
	}

	
	//�ͷ��Լ�������
	_commander->Release();
	return 0;
}

void 
gplayer_imp::FromSwitchToNormal()
{
	ASSERT(_player_state == PLAYER_WAIT_SWITCH);

	_player_state = PLAYER_STATE_NORMAL;
	_switch_dest = -1;

	if(_parent->b_disconnect)
	{
		//����Ѿ�����
		_player_state = PLAYER_DISCONNECT;
		_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
	}
}

bool 
gplayer_imp::CanAttack(const XID & target)
{
	//�����κεط������Խ�����ͨ����
	if(!_layer_ctrl.CheckAttack()) return false;
	return _equipment[item::EQUIP_INDEX_WEAPON].CheckAttack(_equipment);
}

bool
gplayer_imp::CheckLevitate()
{
	return _ph_control.CheckLevitate();	
}

void 
gplayer_imp::PhaseControlInit()
{
	return _ph_control.Initialize(this);
}

const XID &
gplayer_imp::GetCurTarget()
{
	return ((gplayer_controller*)_commander)->GetCurTarget();
}

int 
gplayer_imp::GetAmmoCount()
{
	return _equipment[item::EQUIP_INDEX_PROJECTILE].count;
}

int 
gplayer_imp::DoAttack(const XID & target, char force_attack)
{
	if(!IsAttackMonster() && target.type == GM_TYPE_NPC && !target.IsPet())
	{
		SetAttackMonster(true);
	}

	ActiveCombatState(true);
	_combat_timer = MAX_COMBAT_TIME;
	
	unsigned char dec_arrow = 0;
	if(_cur_item.weapon_type == 1)	//Զ������
	{
		//��ͼ���ټ�֧
		int rst = _equipment.DecAmount(item::EQUIP_INDEX_PROJECTILE,1);
		dec_arrow = 1;
		__PRINTF("���ټ�֧ %d\n",rst);
	}
	
	if(_cur_item.weapon_type == 3)
	{
		//162 SHOOTER
		dec_arrow = 1; 
	}
	
	//��乥����Ϣ����������װ���;úͼ��ټ�֧
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack,dec_arrow);

	//����̫�죿����������!!!
	if(_cur_prop.attack_speed <= 5)
	{
		attack.physic_damage = (int)(attack.physic_damage * player_template::GetDmgAdjToSpecAtkSpeed());
		attack.attack_rate = (int)(attack.attack_rate * player_template::GetAtkRateAdjToSpecAtkSpeed());
	}
	
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg);

	//����ŭ��
	if(_ap_per_hit > 0) ModifyAP(_ap_per_hit);
	return 0;
}

void 
gplayer_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	gactive_imp::FillAttackMsg(target,attack);

	object_interface obj_if(this);

	attack.weapon_class = _cur_item.weapon_class;
	attack.fake_weapon_class = _fake_weapon_class;
	attack.ainfo.sid = GetParent()->cs_sid;
	attack.ainfo.cs_index = GetParent()->cs_index;
	attack.get_attack = obj_if.GenerateDamage();
	attack.get_magic_attack = obj_if.GenerateMagicDamage();
	attack.get_equip_attack = obj_if.GenerateEquipDamage();
	attack.get_equip_magic_attack = obj_if.GenerateEquipMagicDamage();
	attack.weapon_damage = (_cur_item.damage_low + rand()%(_cur_item.damage_high-_cur_item.damage_low+1));
	attack.weapon_magic_damage = (_cur_item.damage_magic_low + rand()%(_cur_item.damage_magic_high-_cur_item.damage_magic_low+1));
	if(!_pvp_enable_flag) attack.force_attack = 0; //If the PK switch is not turned on, turn off the forced attack option
	if(attack.force_attack) attack.force_attack |= C2S::FORCE_ATTACK;
	if(attack.force_attack & (C2S::FORCE_ATTACK_NO_MAFIA|C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE)) 
	{
		//If it is set to not hurt gang members/gang alliances, set the corresponding flag
		attack.ainfo.mafia_id = ((gplayer*)_parent)->id_mafia;
	}
	if(attack.force_attack & C2S::FORCE_ATTACK_NO_SAME_FORCE)
	{
		attack.ainfo.force_id = _player_force.GetForce();
	}

	//Do not actively set the PVP flag in duel mode
	if(_pvp_enable_flag && target.IsPlayerClass() && !GetParent()->IsDuelMode())
	{
		SetPVPCombatState();
	}

	attack.attacker_mode = _pvp_enable_flag?attack_msg::PVP_ENABLE:0; 
	if(IsInPVPCombatStateHigh()) attack.attacker_mode |= attack_msg::PVP_DURATION;
	if(_task_mask & TASK_MASK_KILL_PLAYER) attack.attacker_mode |= attack_msg::PVP_FEEDBACK_KILL;

	_team.GetTeamID(attack.ainfo.team_id, attack.ainfo.team_seq);
	attack.ainfo.eff_level = _team.GetEffLevel();
	attack.ainfo.wallow_level = _team.GetWallowLevel();
	attack.ainfo.profit_level = _profit_level;
	DoWeaponOperation<0>();
	_runner->attack_once(dec_arrow);	
	__PRINTF("Shoot an arrow %d\n",dec_arrow);
}
void 
gplayer_imp::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{
	gactive_imp::FillEnchantMsg(target,enchant);
	
	object_interface obj_if(this);

	enchant.weapon_class = _cur_item.weapon_class;
	enchant.fake_weapon_class = _fake_weapon_class;
	enchant.get_attack = obj_if.GenerateDamage();
	enchant.get_magic_attack = obj_if.GenerateMagicDamage();
	enchant.get_equip_attack = obj_if.GenerateEquipDamage();
	enchant.get_equip_magic_attack = obj_if.GenerateEquipMagicDamage();
	enchant.weapon_damage = (_cur_item.damage_low + rand()%(_cur_item.damage_high-_cur_item.damage_low+1));
	enchant.weapon_magic_damage = (_cur_item.damage_magic_low + rand()%(_cur_item.damage_magic_high-_cur_item.damage_magic_low+1));
	enchant.ainfo.sid = GetParent()->cs_sid;
	enchant.ainfo.cs_index = GetParent()->cs_index;
	if(!_pvp_enable_flag) enchant.force_attack = 0;//If the PK switch is not turned on, turn off the forced attack option
	if(enchant.force_attack) enchant.force_attack |= C2S::FORCE_ATTACK;
	if(enchant.force_attack & (C2S::FORCE_ATTACK_NO_MAFIA|C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE)) 
	{
		//If it is set to not hurt gang members/gang alliances, set the corresponding flag
		enchant.ainfo.mafia_id = ((gplayer*)_parent)->id_mafia;
	}
	if(enchant.force_attack & C2S::FORCE_ATTACK_NO_SAME_FORCE)
	{
		enchant.ainfo.force_id = _player_force.GetForce();
	}

	//Do not actively set the PVP flag in duel mode
	if(_pvp_enable_flag && target.IsPlayerClass() && !enchant.helpful && !GetParent()->IsDuelMode())
	{
		SetPVPCombatState();
	}

	enchant.attacker_mode = _pvp_enable_flag?attack_msg::PVP_ENABLE:0; 
	if(IsInPVPCombatStateHigh()) enchant.attacker_mode |= attack_msg::PVP_DURATION;
	if(_task_mask & TASK_MASK_KILL_PLAYER) enchant.attacker_mode |= attack_msg::PVP_FEEDBACK_KILL;


	_team.GetTeamID(enchant.ainfo.team_id, enchant.ainfo.team_seq);
	enchant.ainfo.eff_level = _team.GetEffLevel();
	enchant.ainfo.wallow_level = _team.GetWallowLevel();
	enchant.ainfo.profit_level = _profit_level;

	//DoWeaponOperation<0>();   It cannot be called here, otherwise an error will occur
}

void 
gplayer_imp::SendAttackMsg(const XID & target, attack_msg & attack)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg);
}

void 
gplayer_imp::SendEnchantMsg(int message,const XID & target, enchant_msg & attack)
{
	SendTo<0>(message,target,0,&attack,sizeof(attack));
}

void
gplayer_imp::SwitchSvr(int dest, const A3DVECTOR & oldpos, const A3DVECTOR &newpos, const instance_key * switch_key)
{
	__PRINTF("switch\n");
	if(_player_state == PLAYER_STATE_NORMAL && _switch_dest == -1)
	{
		//�����Լ���ת��Ŀ���ת�Ƶص�
		_switch_dest = dest;
		_switch_pos = newpos;
		if(switch_key) 
			_instance_switch_key = *switch_key;
		else
			memset(&_instance_switch_key,0,sizeof(_instance_switch_key));

		user_save_data((gplayer*)_parent,NULL ,1);
		LuaManager * lua = LuaManager::GetInstance();
		lua->EventPlayerSave(_parent->ID.id);
		
		
		GLog::log(GLOG_INFO,"�û�%d���л���ͼǰ�����˴���",_parent->ID.id);

		
		//�����л�������ʱ��Ҳ�����д��̲����ˣ��������л���ɺ��ٽ���
		_write_timer = abase::Rand(500, 513);

		//����������ת��״̬���г�ʱ��
		_player_state = PLAYER_WAIT_SWITCH;
		_general_timeout = 10;

		//����ת�Ƶ�����
		ASSERT(dest != world_manager::GetWorldIndex());
		gplayer *pPlayer = GetParent();
		GMSV::SendSwitchServerStart(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid,world_manager::GetWorldIndex(),dest,&_instance_switch_key,sizeof(_instance_switch_key));

		timeval tv;
		gettimeofday(&tv,NULL);
		__PRINTF("%d��ʼת�Ʒ�����:%u.%u\n",pPlayer->ID.id,tv.tv_sec,tv.tv_usec);
	}
}

void 
gplayer_imp::OnDeath(const XID & lastattack,bool is_pariah, char attacker_mode, int taskdead)
{
	__PRINTF("player %d dead \n",_parent->ID.id);
	if((gplayer*)_parent->IsZombie())
	{
		//�Ѿ���������Ӧ���ٴε�����
		return ;
	}
	
	int death_type = lastattack.type&0xFF;
	LuaManager::GetInstance()->EventOnPlayerKilled(_parent->ID.id, lastattack.id , death_type , is_pariah, attacker_mode );
	
	if(_player_state == PLAYER_SIT_DOWN)
	{
		LeaveStayInState();
		_player_state = PLAYER_STATE_NORMAL;
	}
	else if(_player_state == PLAYER_STATE_MARKET)
	{
		CancelPersonalMarket();
	}
	else if(_player_state == PLAYER_STATE_BIND)
	{
		//�����״̬ 
		_bind_player.PlayerCancel(this);
	}
	
	ClearSession();
	ClearAction();
	ActiveCombatState(false);


	bool bCanDrop = _player_state == PLAYER_STATE_NORMAL ||
			_player_state == PLAYER_DISCONNECT ||
			_player_state == PLAYER_SIT_DOWN ||
			_player_state == PLAYER_STATE_BIND;

	bool free_resurrect = false;
	if(!_free_pvp_mode && _basic.level > LOW_PROTECT_LEVEL && taskdead != 2 && !_nonpenalty_pvp_state && !world_manager::GetWorldFlag().nonpenalty_pvp_flag)
	{
		//����������޵Ĵ��ڣ�������ڣ������κ���ʧ����ɾ��һ����������
		/*
		int dummy_id = ITEM_POPPET_DUMMY_ID2;
		int dummy_index = _inventory.Find(0, ITEM_POPPET_DUMMY_ID2);
		if(dummy_index < 0)
		{
			dummy_id = ITEM_POPPET_DUMMY_ID;
			dummy_index = _inventory.Find(0, ITEM_POPPET_DUMMY_ID);
			if(dummy_index < 0)
			{
				dummy_id = ITEM_POPPET_DUMMY_ID3;
				dummy_index = _inventory.Find(0, ITEM_POPPET_DUMMY_ID3);
			}
		}
		*/
		//DEADRAKY
		int dummy_id = 0;
		int dummy_index = -1;
		for (int i = 0; LuaManager::GetInstance()->GetConfig()->ITEM_POPPET_DUMMY_TABLE[i] && i < 16 ; i++)
		{
			dummy_index = _inventory.Find(0,LuaManager::GetInstance()->GetConfig()->ITEM_POPPET_DUMMY_TABLE[i]);
			if(dummy_index >= 0) 
			{
				dummy_id = LuaManager::GetInstance()->GetConfig()->ITEM_POPPET_DUMMY_TABLE[i];
				break;
			}
		}

		if(dummy_index >= 0 && bCanDrop)
		{
			free_resurrect = true;

			if( !LuaManager::GetInstance()->IsGoodItem(dummy_id) )
			{
				item& it = _inventory[dummy_index];
				UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
				_inventory.DecAmount(dummy_index, 1);
				_runner->player_drop_item(IL_INVENTORY,dummy_index,dummy_id,1,S2C::DROP_TYPE_USE);
			}
		}	
	}

	if(bCanDrop && !free_resurrect)
	{
		if(!_free_pvp_mode && _basic.level > LOW_PROTECT_LEVEL && taskdead != 2 && !_nonpenalty_pvp_state && !world_manager::GetWorldFlag().nonpenalty_pvp_flag)
		{
			//������PVP�ͷǵͼ�������ʱ��Ų�������Ʒ
			if(_pvp_enable_flag)
			{
				//��PK���غ���PK���غ��״̬������
				_invade_ctrl.OnDeath(lastattack);
			}		
			else
			{
				/*//�����չ̶���ʽ�����
				//Ŀǰ������ɱ������ĸ���ͬ����
				float * pPropInv = NULL;
				pPropInv = player_template::GetMobNormalInventoryDropRate();
				int drop_count1 = abase::RandSelect(pPropInv,10);
				if(drop_count1 > 0)
				{
					DropItemOnDeath(drop_count1,0);
				}*/
			}
		}
	}

	if(bCanDrop)
	{
		//����ض��������Ʒ
		ThrowDeadDropItem();
	}

	//�����ٻس���
	_petman.RecallPet(this);

	_basic.mp = 0;
	_basic.ap = 0;

	if(lastattack.type == GM_TYPE_PLAYER && is_pariah && _invader_state == INVADER_LVL_0 && !_free_pvp_mode)
	{
		//�Ǻ��������ͺ�����Ϣ
		SendTo<0>(GM_MSG_PLAYER_BECOME_PARIAH,lastattack,0);
	}

    if ((CheckVipService(CVS_ENEMYLIST)) && (lastattack.type == GM_TYPE_PLAYER) &&
        !_free_pvp_mode && !world_manager::GetWorldFlag().nonpenalty_pvp_flag)
    {
        // 0 - updateenemylist.hpp : ENEMYLIST_INSERT
        GMSV::SendUpdateEnemyList(0, _parent->ID.id, lastattack.id);
    }

	if(lastattack.type == GM_TYPE_NPC && taskdead == 0)
	{
		//��NPCɱ��������һ����Ϣ
		SendTo<0>(GM_MSG_PLAYER_KILLED_BY_NPC,lastattack,0);
	}

	if(lastattack.type == GM_TYPE_PLAYER && (attacker_mode & attack_msg::PVP_FEEDBACK_KILL))
	{
		msg_player_killed_info_t data;
		GetPlayerClass(data.cls, data.gender);
		data.level 		= _basic.level;
		data.force_id 	= _player_force.GetForce();
		SendTo<0>(GM_MSG_PLAYER_KILLED_BY_PLAYER, lastattack, 0, &data, sizeof(data));
	}

	//�����Ĳ���������Zombie״̬
	_parent->b_zombie = true;
	if(lastattack.IsPlayerClass()) _kill_by_player = true; else _kill_by_player = false;

	//ע���ھ����������߸�����ܻ��в�Ӧ���еľ�����ʧ ���������Ǳ����ɱ���� ���Ի��ǲ����о�����ʧ��
	if(_free_pvp_mode || free_resurrect || taskdead == 2 || _nonpenalty_pvp_state || world_manager::GetWorldFlag().nonpenalty_pvp_flag)
	{
		//ֻҪ������PVPģʽ������Ϊ�Ǳ����ɱ�������������и������ʧ
		_kill_by_player = true;
	}

	_resurrect_state = false;
	_resurrect_exp_reduce = 1.0f;
	_resurrect_hp_factor = 0.f;
	_resurrect_mp_factor = 0.f;

	//����������Ϣ
	_runner->on_death(lastattack, false);
	if(is_pariah) death_type |= 0x100;
	if(taskdead) death_type |= taskdead<<12;
	GLog::die(_parent->ID.id,death_type,lastattack.id);
	GLog::log(GLOG_INFO,"�û�%d��%dɱ�������(%d)",_parent->ID.id,lastattack.id,death_type);

    if ((CheckVipService(CVS_RESURRECT)) && !world_manager::GetWorldLimit().nocash_resurrect)
    {
        int index = _cash_resurrect_times_in_cooldown;
        if (index < 0) index = 0;
        if (CheckCoolDown(COOLDOWN_INDEX_RESURRECT_BY_CASH))
        {
            index = 0;
        }
        else
        {
            ++index;
            if (index >= CASH_RESURRECT_COST_TABLE_SIZE)
                index = CASH_RESURRECT_COST_TABLE_SIZE - 1;
        }

        int cash_need = CASH_RESURRECT_COST_TABLE[index];
        _runner->cash_resurrect_info(cash_need, GetMallCash());
    }

	//��������Ĵ���
	PlayerTaskInterface task_if(this);
	OnTaskPlayerKilled(&task_if);

	//������������
	if(!world_manager::GetInstance()->IsUniqueWorld()) //����������
	{
		_plane->ModifyCommonValue(COMMON_VALUE_ID_PLAYERDEAD_COUNT, 1);
	}
	//�����������Զ���ʱ������session
//	session_player_reborn * pSession = new session_player_reborn(this);
//	pSession->SetDelay(50*10);
	// ����PVP ����
	if(((gplayer*)_parent)->mafia_pvp_mask && lastattack.IsPlayerClass())
	{
		MSG msg;
		msg_mafia_pvp_award_t mfa = { OI_GetMafiaID(), city_region::GetDomainID(_parent->pos.x,_parent->pos.z)};
		BuildMessage(msg, GM_MSG_MAFIA_PVP_AWARD, lastattack, _parent->ID, _parent->pos, AWARD_MAFIAPVP_HIJACK_KILL , &mfa, sizeof(mfa));
		_plane->PostLazyMessage(msg);
	}
}

int 
gplayer_imp::PlayerGetItemInfo(int where, int index)
{
	int rst;
	unsigned short crc = 0;
	item_data data;
	switch(where)
	{       
		case IL_INVENTORY:
			rst = _inventory.GetItemData(index,data,crc);
			break;
		case IL_EQUIPMENT:
			rst = _equipment.GetItemData(index,data,crc);
			break;
		case IL_TASK_INVENTORY:
			rst = _task_inventory.GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX:		//lgc
			rst = _trashbox.GetBackpack(0).GetItemData(index,data,crc);
			break;
		case IL_USER_TRASH_BOX:
			rst = _user_trashbox.GetBackpack(0).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX2:
			rst = _trashbox.GetBackpack(1).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX3:
			rst = _trashbox.GetBackpack(2).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX4:
			rst = _trashbox.GetBackpack(3).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX5:
			rst = _trashbox.GetBackpack(4).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX6:
			rst = _trashbox.GetBackpack(5).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX7:
			rst = _trashbox.GetBackpack(6).GetItemData(index,data,crc);
			break;
		case IL_TRASH_BOX8:
			rst = _trashbox.GetBackpack(7).GetItemData(index,data,crc);
			break;
		default:
			rst = -1;
	}
	if(rst == 0)
	{
		//������
		_runner->self_item_empty_info(where,index);
	}else if(rst > 0)
	{
		//�ҵ���
		_runner->self_item_info(where,index,data,crc);
	}

	return rst;
}

int 
gplayer_imp::PlayerGetItemInfoList(int where, unsigned int count , unsigned char *list)
{
	item_list &inv = GetInventory(where);

	unsigned int cap = inv.Size();
	for(unsigned int i = 0; i < count; i ++)
	{
		unsigned int index = list[i];
		if(index < cap)
		{

			unsigned short crc = 0;
			item_data data;
			if(inv.GetItemData(index,data,crc) > 0)
			{
				_runner->self_item_info(where,index,data,crc);
			}
		}
	}

	return 0;
}

void 
gplayer_imp::TrashBoxOpen(bool view_only)
{
	if(view_only)
	{
		_trash_box_open_view_only_flag = true;
	}
	else
	{
		_trash_box_open_flag = true;
		IncTrashBoxChangeCounter();//��ʱ����Ϊ��Ʒ�䷢���˱仯
	}
	_runner->trashbox_open(0);
	GLog::log(GLOG_INFO,"�û�%d�����˲ֿ�",_parent->ID.id);
}

void 
gplayer_imp::TrashBoxClose(bool view_only)
{
	if(view_only)
	{
		_trash_box_open_view_only_flag = false;
	}
	else
	{
		_trash_box_open_flag = false;
	}
	_runner->trashbox_close(0);
	GLog::log(GLOG_INFO,"�û�%d�ر��˲ֿ�",_parent->ID.id);
}

void 
gplayer_imp::UserTrashBoxOpen()
{
	_user_trash_box_open_flag = true;
	IncUserTrashBoxChangeCounter();//��ʱ����Ϊ��Ʒ�䷢���˱仯
	_runner->trashbox_open(1);
	GLog::log(GLOG_INFO,"�û�%d�������ʺŲֿ�",_parent->ID.id);
}

void 
gplayer_imp::UserTrashBoxClose()
{
	_user_trash_box_open_flag = false;
	_runner->trashbox_close(1);
	GLog::log(GLOG_INFO,"�û�%d�ر����ʺŲֿ�",_parent->ID.id);
}

void
gplayer_imp::PlayerGetTrashBoxInfo(bool detail)
{
	//printf("IL_TRASH_BOX & IL_TRASH_BOX2 \n");
	if(detail)
	{
		raw_wrapper rw(1024);
		_trashbox.GetBackpack(0).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX,_trashbox.GetBackpack(0).Size(),rw.data(),rw.size());

		rw.clear();
		_trashbox.GetBackpack(1).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX2,_trashbox.GetBackpack(1).Size(),rw.data(),rw.size());
	}
	else
	{
		abase::octets buf;
		_trashbox.GetBackpack(0).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX, _trashbox.GetBackpack(0).Size() , buf.begin(), buf.size());

		buf.clear();
		_trashbox.GetBackpack(1).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX2, _trashbox.GetBackpack(1).Size() , buf.begin(), buf.size());

	}
	_runner->trashbox_wealth(0,_trashbox.GetMoney());
}

void
gplayer_imp::PlayerGetPortableTrashBoxInfo(bool detail)
{
	//printf("IL_TRASH_BOX3 & IL_TRASH_BOX4 \n");
	if(detail)
	{
		raw_wrapper rw(1024);
		_trashbox.GetBackpack(2).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX3,_trashbox.GetBackpack(2).Size(),rw.data(),rw.size());

		rw.clear();
		_trashbox.GetBackpack(3).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX4,_trashbox.GetBackpack(3).Size(),rw.data(),rw.size());
		
		rw.clear();
		_trashbox.GetBackpack(4).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX5,_trashbox.GetBackpack(4).Size(),rw.data(),rw.size());
		
		rw.clear();
		_trashbox.GetBackpack(5).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX6,_trashbox.GetBackpack(5).Size(),rw.data(),rw.size());

		rw.clear();
		_trashbox.GetBackpack(6).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX7,_trashbox.GetBackpack(6).Size(),rw.data(),rw.size());
		
		rw.clear();
		_trashbox.GetBackpack(7).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_TRASH_BOX8,_trashbox.GetBackpack(7).Size(),rw.data(),rw.size());

	}
	else
	{
		abase::octets buf;
		_trashbox.GetBackpack(2).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX3, _trashbox.GetBackpack(2).Size() , buf.begin(), buf.size());
		
		buf.clear();
		_trashbox.GetBackpack(3).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX4, _trashbox.GetBackpack(3).Size() , buf.begin(), buf.size());
		
		buf.clear();
		_trashbox.GetBackpack(4).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX5, _trashbox.GetBackpack(4).Size() , buf.begin(), buf.size());
		
		buf.clear();
		_trashbox.GetBackpack(5).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX6, _trashbox.GetBackpack(5).Size() , buf.begin(), buf.size());
		
		buf.clear();
		_trashbox.GetBackpack(6).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX7, _trashbox.GetBackpack(6).Size() , buf.begin(), buf.size());
		
		buf.clear();
		_trashbox.GetBackpack(7).SimpleSave(buf);
		_runner->self_inventory_data(IL_TRASH_BOX8, _trashbox.GetBackpack(7).Size() , buf.begin(), buf.size());
	}
}

void
gplayer_imp::PlayerGetUserTrashBoxInfo(bool detail)
{
	//printf("IL_USER_TRASH_BOX \n");
	if(detail)
	{
		raw_wrapper rw(1024);
		_user_trashbox.GetBackpack(0).DetailSave(rw);
		_runner->self_inventory_detail_data(IL_USER_TRASH_BOX,_user_trashbox.GetBackpack(0).Size(),rw.data(),rw.size());
	}
	else
	{
		abase::octets buf;
		_user_trashbox.GetBackpack(0).SimpleSave(buf);
		_runner->self_inventory_data(IL_USER_TRASH_BOX, _user_trashbox.GetBackpack(0).Size() , buf.begin(), buf.size());
	}
	_runner->trashbox_wealth(1,_user_trashbox.GetMoney());
}

void 
gplayer_imp::PlayerGetInventoryDetail(int where)
{
	LuaManager::GetInstance()->EventPlayerGetInventory(_parent->ID.id,where);
	raw_wrapper rw;
	unsigned char size;
	switch(where)
	{
		case IL_INVENTORY:
			_inventory.DetailSave(rw);
			size = _inventory.Size();
			break;
		case IL_EQUIPMENT:
			_equipment.DetailSave(rw);
			size = _equipment.Size();
			break;
		case IL_TASK_INVENTORY:
			_task_inventory.DetailSave(rw);
			size = _task_inventory.Size();
			break;
		default:
		ASSERT(false);
		return;
	}
	_runner->self_inventory_detail_data(where,size, rw.data(),rw.size());
	/*161+*/
	RefreshInventoryNewArmorEnter(true, false);		
}

void
gplayer_imp::PlayerExchangeInvItem(unsigned int idx1,unsigned int idx2)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!_inventory.ExchangeItem(idx1,idx2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->exchange_inventory_item(idx1,idx2);
}

void 
gplayer_imp::PlayerMoveInvItem(unsigned int src,unsigned int dest,unsigned int count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!_inventory.MoveItem(src,dest,&count))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	_runner->move_inventory_item(src,dest,count);
}

void
gplayer_imp::PlayerExchangeTrashItem(int where, unsigned int idx1,unsigned int idx2)
{
	item_list & box = GetTrashInventory(where);
	if(!box.ExchangeItem(idx1,idx2))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	//�����ֿ���޸Ĳ���Ҫ�򿪲ֿ⣬��Ҫ���Ӹı����
	if(IsPortableTrashBox(where)) IncTrashBoxChangeCounter();
	_runner->exchange_trashbox_item(where, idx1,idx2);

}

void 
gplayer_imp::PlayerMoveTrashItem(int where, unsigned int src,unsigned int dest,unsigned int count)
{
	item_list & box = GetTrashInventory(where);
	if(!box.MoveItem(src,dest,&count))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	//�����ֿ���޸Ĳ���Ҫ�򿪲ֿ⣬��Ҫ���Ӹı����
	if(IsPortableTrashBox(where)) IncTrashBoxChangeCounter();
	_runner->move_trashbox_item(where,src,dest,count);

}

static bool VerifySpecTrashBox(int where, item_list & inv, unsigned int idx)
{
	if(idx >= inv.Size()) return false;
	int id = inv[idx].type;
	if(id <= 0 ) return true;
	if(where == gplayer_imp::IL_TRASH_BOX2)
	{
		DATA_TYPE dt = world_manager::GetDataMan().get_data_type(id, ID_SPACE_ESSENCE);
		return dt== DT_MATERIAL_ESSENCE || dt == DT_TASKNORMALMATTER_ESSENCE || dt == DT_SKILLTOME_ESSENCE;
	}
	else if(where == gplayer_imp::IL_TRASH_BOX3)
	{
		DATA_TYPE dt = world_manager::GetDataMan().get_data_type(id, ID_SPACE_ESSENCE);
		return (dt == DT_FASHION_ESSENCE || dt == DT_FASHION_NEW_ESSENCE);
	}
	else if(where == gplayer_imp::IL_TRASH_BOX4)
	{
		DATA_TYPE dt = world_manager::GetDataMan().get_data_type(id, ID_SPACE_ESSENCE);
		return dt == DT_POKER_ESSENCE || dt == DT_POKER_DICE_ESSENCE;
	}
	else if(where == gplayer_imp::IL_TRASH_BOX5)
	{
		DATA_TYPE dt = world_manager::GetDataMan().get_data_type(id, ID_SPACE_ESSENCE);
		return dt == DT_RUNE_ITEM_ESSENCE;
	}
	else if(where == gplayer_imp::IL_TRASH_BOX6)
	{
		DATA_TYPE dt = world_manager::GetDataMan().get_data_type(id, ID_SPACE_ESSENCE);
		return dt == DT_FLYSWORD_ESSENCE;
	}
	else if(where == gplayer_imp::IL_USER_TRASH_BOX)
	{
		if(inv[idx].proc_type & item::ITEM_PROC_TYPE_NOPUTIN_USERTRASH) return false;		
		if(world_manager::IsNoPutInUserTrashItem(id)) return false;
	}
	return true;
}

void 
gplayer_imp::PlayerExchangeTrashInv(int where, unsigned int idx_tra, unsigned int idx_inv)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!VerifySpecTrashBox(where, _inventory, idx_inv))
	{
		return ;
	}

	item_list & box = GetTrashInventory(where);
	if(idx_tra >= box.Size() || idx_inv >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}
	
	//�����ֿ�ȡ����ƷʱҪ�ܰ�ȫ��������
	if(IsPortableTrashBox(where) && box[idx_tra].type > 0 && !_security_passwd_checked)
	{
		_runner->error_message(S2C::ERR_SECURITY_PASSWD_UNCHECKED);
		return;
	}
	//�ʺŲֿ���Ʒ�����ɲ�����������������Ʒ
	if(where == gplayer_imp::IL_USER_TRASH_BOX && box.GetItemCount() >= box.Size()/2)
	{
		if(_inventory[idx_inv].type != -1 && box[idx_tra].type == -1)	return;
	}
	item it;
	box.Exchange(idx_tra,it);
	_inventory.Exchange(idx_inv,it);
	box.Exchange(idx_tra,it);
	//�����ֿ���޸Ĳ���Ҫ�򿪲ֿ⣬��Ҫ���Ӹı����
	if(IsPortableTrashBox(where)) IncTrashBoxChangeCounter();
	_runner->exchange_trashbox_inventory(where, idx_tra,idx_inv);

	//���ʺŲֿ��Ƴ�������Ʒ��Ҫ��¼log
	if(where == gplayer_imp::IL_USER_TRASH_BOX)
	{
		if(_inventory[idx_inv].type > 0)
			GLog::log(GLOG_INFO, "�û�%d��%d����Ʒ%d�Ƴ��ʺŲֿ�", _parent->ID.id, _inventory[idx_inv].count, _inventory[idx_inv].type);	
		if(box[idx_tra].type > 0)
			GLog::log(GLOG_INFO, "�û�%d��%d����Ʒ%d�����ʺŲֿ�", _parent->ID.id, box[idx_tra].count, box[idx_tra].type);	
	}
}

void 
gplayer_imp::PlayerTrashItemToInv(int where, unsigned int idx_tra, unsigned int idx_inv, unsigned int count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	//�����ֿ�ȡ����ƷʱҪ�ܰ�ȫ��������
	if(IsPortableTrashBox(where) && !_security_passwd_checked)
	{
		_runner->error_message(S2C::ERR_SECURITY_PASSWD_UNCHECKED);
		return;
	}
	int delta = MoveBetweenItemList(GetTrashInventory(where),_inventory,idx_tra,idx_inv,count);
	if(delta >= 0)
	{
		//�����ֿ���޸Ĳ���Ҫ�򿪲ֿ⣬��Ҫ���Ӹı����
		if(IsPortableTrashBox(where)) IncTrashBoxChangeCounter();
		_runner->trash_item_to_inventory(where, idx_tra,idx_inv,delta);
		
		//���ʺŲֿ��Ƴ�������Ʒ��Ҫ��¼log
		if(where == gplayer_imp::IL_USER_TRASH_BOX)
		{
			GLog::log(GLOG_INFO, "�û�%d��%d����Ʒ%d�Ƴ��ʺŲֿ�", _parent->ID.id, delta, _inventory[idx_inv].type);	
		}
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}

void 
gplayer_imp::PlayerInvItemToTrash(int where, unsigned int idx_inv, unsigned int idx_tra, unsigned int count)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	if(!VerifySpecTrashBox(where, _inventory, idx_inv))
	{
		return ;
	}
	item_list & box = GetTrashInventory(where);
	if(idx_tra >= box.Size())
	{
		return ;
	}

	if(where == gplayer_imp::IL_USER_TRASH_BOX && box.GetItemCount() >= box.Size()/2)
	{
		if(box[idx_tra].type == -1)	return;
	}
	int delta = MoveBetweenItemList(_inventory,box,idx_inv,idx_tra,count);
	if(delta >= 0)
	{
		if(IsPortableTrashBox(where)) IncTrashBoxChangeCounter();
		_runner->inventory_item_to_trash(where, idx_inv,idx_tra,delta);
		
		if(where == gplayer_imp::IL_USER_TRASH_BOX)
		{
		}
	}
	else
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
	}
}

void 
gplayer_imp::PlayerDropInvItem(unsigned int index,unsigned int count,bool isProtected)
{
	if(index >= _inventory.Size() || !count)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(world_manager::GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(_inventory[index].type == -1)
	{
		_runner->player_drop_item(IL_INVENTORY,index,-1,0,S2C::DROP_TYPE_PLAYER);
		return ;
	}

	if(_inventory[index].proc_type & item::ITEM_PROC_TYPE_NOTHROW) 
	{
		//����Ʒ�޷��������Ƿ񱨸����
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return;
	}

	//������ڿ��е���Ʒ�����Ƿ��㹻
	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	ThrowInvItem(index,count,isProtected,S2C::DROP_TYPE_PLAYER);
}

void 
gplayer_imp::ThrowInvItem(unsigned int index, unsigned int count, bool isProtected,unsigned char drop_type, const XID & spec_owner)
{
	if(world_manager::GetWorldLimit().nothrow_anyway) return;
	item_data data;
	int rst = _inventory.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}
	if(data.count > count)
	{
		data.count = count;
	}

	//���������λ��
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//��������˱߽磬��ô�Ͱ����Լ���λ������
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	
	//�������ڶ�������Ȼ��item�ͷź������content ���ܾʹ�����
	if(isProtected)
		DropItemFromData(_plane,pos,data,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id); 
	else if(spec_owner.IsPlayer())
		DropItemFromData(_plane,pos,data,spec_owner,0,0,_parent->ID.id); 
	else
		DropItemFromData(_plane,pos,data,XID(0,0),0,0,_parent->ID.id); 

	GLog::log(LOG_INFO,"�û�%d��������%d��%d",_parent->ID.id,data.count,data.type);

	_inventory.DecAmount(index,data.count);
	_runner->player_drop_item(IL_INVENTORY,index,data.type,data.count,drop_type);
}

void 
gplayer_imp::PlayerDropEquipItem(unsigned int index,bool isProtected)
{
	if(index >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
		
	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	if(world_manager::GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(_equipment[index].type == -1)
	{
		_runner->player_drop_item(IL_EQUIPMENT,index,-1,0,S2C::DROP_TYPE_PLAYER);
		return ;
	}
	if(_equipment[index].proc_type & item::ITEM_PROC_TYPE_NOTHROW) 
	{
		//����Ʒ�޷��������Ƿ񱨸����
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return;
	}

	//������ڿ��е���Ʒ�����Ƿ��㹻
	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}
	
	if(index == item::EQUIP_INDEX_HP_ADDON  || index == item::EQUIP_INDEX_MP_ADDON)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return;
	}	
	//��С���飬��ֹս��״̬�л���С����  //lgc
	if(index == item::EQUIP_INDEX_ELF)
	{
		if(IsCombatState())	
		{
			_runner->error_message(S2C::ERR_ELF_CANNOT_UNEQUIP_IN_COMBAT_STATE);
			return ;
		}	
	}
	//�ܰ�ȫ���뱣����װ��λ�ò��ܻ���
	if(!_security_passwd_checked)
	{
		if(item::CheckEquipProtectedByIndex(index))
		{
			_runner->error_message(S2C::ERR_SECURITY_PASSWD_UNCHECKED);
			return;
		}
	}

	ThrowEquipItem(index,isProtected,S2C::DROP_TYPE_PLAYER);
}

void 
gplayer_imp::ThrowEquipItem(unsigned int index, bool isProtected,unsigned char drop_type, int throw_count, const XID & spec_owner)
{
	if(world_manager::GetWorldLimit().nothrow_anyway) return;
	item_data data;
	int rst = _equipment.GetItemData(index, data);
	if(rst <=0) 
	{
		ASSERT(false);
		return;
	}
	if(throw_count > 0 && (int)data.count > throw_count)
	{
		data.count = throw_count;
	}

	//����װ���ı��־
	IncEquipChangeFlag();

	//���������λ��
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//��������˱߽磬��ô�Ͱ����Լ���λ������
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	//�������ڶ�������Ȼ��item�ͷź������content ���ܾʹ�����
	if(isProtected)
		DropItemFromData(_plane,pos,data,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id); 
	else if(spec_owner.IsPlayer())
		DropItemFromData(_plane,pos,data,spec_owner,0,0,_parent->ID.id); 
	else
		DropItemFromData(_plane,pos,data,XID(0,0),0,0,_parent->ID.id); 

	GLog::log(LOG_INFO,"�û�%d����װ��%d",_parent->ID.id,data.type);
	_equipment.DecAmount(index, data.count);
	_runner->player_drop_item(IL_EQUIPMENT,index,data.type,data.count,drop_type);

	//����ˢ��װ��
	RefreshEquipment();
	//����������Ϣ
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1ULL<<index,0,0); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	LuaManager::GetInstance()->EventOnPlayerEquipChange(_parent->ID.id);

}


void 
gplayer_imp::PlayerExchangeEquipItem(unsigned int index1,unsigned int index2)
{
	unsigned int size = _equipment.Size();
	if(index1 >= size || index2 >= size || index1 == index2)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	unsigned int type = ((_equipment[index1].type == -1)?1:0) + ((_equipment[index2].type == -1)?2:0);
	uint64_t mask1 = 1ULL << index1;
	uint64_t mask2 = 1ULL << index2;
	uint64_t mask_add = 0;
	uint64_t mask_del = 0;
	X_EQUIP id[2];
	
	//char id[12];
	int count=0;
	switch(type)
	{
		case 0:
			//index1 é—è·¨å–é‹å©šå¹é”Ÿï¿½ index2é—è·¨å–é‹å©šå¹é”Ÿï¿½
			if(!(_equipment[index2].CheckEquipPostion(index1)) || !(_equipment[index1].CheckEquipPostion(index2)))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask1 | mask2;
			mask_del = 0;
			count = 2;
			if(index1 > index2)
			{
				*(int*)&id[0].type = _equipment[index1].type; id[0].mask = _equipment[index1].GetIdModify();
				*(int*)&id[1].type = _equipment[index2].type; id[1].mask = _equipment[index2].GetIdModify();
			}
			else
			{
				*(int*)&id[1].type = _equipment[index1].type; id[1].mask = _equipment[index1].GetIdModify();
				*(int*)&id[0].type = _equipment[index2].type; id[0].mask = _equipment[index2].GetIdModify();
			}
		break;
		case 1:
			//index1 �� index 2��
			if(!(_equipment[index2].CheckEquipPostion(index1)))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask1;
			mask_del = mask2;
			count = 1;
			*(int*)&id[0].type = _equipment[index2].type; id[0].mask = _equipment[index2].GetIdModify();
		break;
		case 2:
			//index2 �� index 1��
			if(!(_equipment[index1].CheckEquipPostion(index2)))
			{
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}
			mask_add = mask2;
			mask_del = mask1;
			count = 1;			
			*(int*)&id[0].type = _equipment[index1].type; id[0].mask = _equipment[index1].GetIdModify();			
		break;
		case 3:
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
	}

	IncEquipChangeFlag();

	_equipment.ExchangeItem(index1,index2);
	_runner->exchange_equipment_item(index1,index2);
	
	//ʹ��װ����Ч
	RefreshEquipment();
	//����װ�������ݣ������˿���
	CalcEquipmentInfo();

	_runner->equipment_info_changed(mask_add,mask_del,&id, count * sizeof(X_EQUIP)); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	LuaManager::GetInstance()->EventOnPlayerEquipChange(_parent->ID.id);
	
	//ArenaStrongUpdate();
}

void 
gplayer_imp::PlayerEquipItem(unsigned int index_inv, unsigned int index_equip)
{
	LuaManager * lua = LuaManager::GetInstance();
	
	if(index_inv >= _inventory.Size() || index_equip >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}

	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	bool type1 = (_inventory[index_inv].type == -1);
	bool type2 = (_equipment[index_equip].type == -1);
	if(type1 && type2)
	{
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}

	if(!type2)
	{
		//װ�����ǿ�
		//�������������߻�������ô�����֮
		if( lua->GetSwap()->equip[index_equip] == 2 )//index_equip == item::EQUIP_INDEX_HP_ADDON  || index_equip == item::EQUIP_INDEX_MP_ADDON)
		{
			if(type1)
			{
				//��Щװ������ж��
				_runner->error_message(S2C::ERR_ITEM_CANNOT_UNEQUIP);
				return ;
			}
			if(!_inventory[index_inv].CheckEquipPostion(index_equip))
			{	
				//��Щ��Ʒ����װ��
				_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
				return;
			}

			//���װ���ϵ���������߻�����
			int t1 = _equipment[index_equip].type;
			int proc_type1 = _equipment[index_equip].proc_type;

			_equipment.Remove(index_equip);

			UpdateMallConsumptionDestroying(t1, proc_type1, 1);
			_runner->player_drop_item(IL_EQUIPMENT,index_equip,t1,1,S2C::DROP_TYPE_TAKEOUT);
			type2 = true;
		}
		//��С���飬��ֹս��״̬�л���С����  //lgc
		if( lua->GetSwap()->equip[index_equip] == 1 )//index_equip == item::EQUIP_INDEX_ELF)
		{
			if(IsCombatState())	
			{
				_runner->error_message(S2C::ERR_ELF_CANNOT_UNEQUIP_IN_COMBAT_STATE);
				return ;
			}	
		}

		//�ܰ�ȫ���뱣����װ��λ�ò��ܻ���
		if(!_security_passwd_checked)
		{
			if(item::CheckEquipProtectedByIndex(index_equip))
			{
				_runner->error_message(S2C::ERR_SECURITY_PASSWD_UNCHECKED);
				return;
			}
		}
	}
	
	if(!type1)
	{
		//�������ǿ�
		if(!_inventory[index_inv].CheckEquipPostion(index_equip))
		{	
			//װ��λ�ò���
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
			return;
		}
		
		//װ�����в�ͬ��λ���ܴ���ͬ��id�Ķ�̬������Ʒ
		if(item::CheckEquipDynSkillByIndex(index_equip))
		{
			for(unsigned int i=0; i<_equipment.Size(); i++)
			{
				if( item::CheckEquipDynSkillByIndex(i) && i != index_equip && _equipment[i].type == _inventory[index_inv].type)
				{
					_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
					return;
				}
			}		
		}

		bool is_fly_mode = _layer_ctrl.IsFlying();	//�ж��Ƿ��ڷ���״̬
		//����֮
		if(!EquipItem(index_inv,index_equip))
		{
			_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		}
		else
		{
			//�����ɹ������һ���Ƿ���Ҫ���а󶨲���
			item & it = _equipment[index_equip];
			ASSERT(it.type != -1);
			bool notify_equip_item = false;
			if(it.proc_type & item::ITEM_PROC_TYPE_BIND2)
			{
				//��������������а�
				it.proc_type |= item::ITEM_PROC_TYPE_NODROP |
					item::ITEM_PROC_TYPE_NOTHROW|
					item::ITEM_PROC_TYPE_NOSELL |
					item::ITEM_PROC_TYPE_NOTRADE|
					item::ITEM_PROC_TYPE_BIND;

				it.proc_type &= ~(item::ITEM_PROC_TYPE_BIND2);
				notify_equip_item = true;
				GLog::log(LOG_INFO,"�û�%dװ������Ʒ%d, GUID(%d,%d)",_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);

				UpdateMallConsumptionBinding(it.type, it.proc_type, it.count);
			}

			/*160+*/
			if(!CheckStorageItem(it.type))
			{
				CheckValidRepositoryItem(it.type, it);
			}
			
			int count1 = _inventory[index_inv].count;
			int count2 = _equipment[index_equip].count;
			ASSERT(count1 >= 0 && count2 >=0 && count1+count2 > 0);
			_runner->equip_item(index_inv,index_equip,count1,count2);
			
			X_EQUIP id1;
			*(int*)&id1.type = _equipment[index_equip].type; id1.mask = _equipment[index_equip].GetIdModify();
			CalcEquipmentInfo();
			_runner->equipment_info_changed(1ULL<<index_equip , 0, &id1,sizeof(X_EQUIP));//�˺���ʹ����CalcEquipmentInfo�Ľ��
			LuaManager::GetInstance()->EventOnPlayerEquipChange(_parent->ID.id);
			if (index_equip == item::EQUIP_INDEX_WEAPON)
			{
				_real_weapon_class = _cur_item.weapon_class;
				if (_filters.IsFilterExist(FILTER_SETNEWWEAPONADDON))
				{
					_filters.RemoveFilter(FILTER_SETNEWWEAPONADDON);
				}
			}
			//���ڷɽ�����������ж�
			if(is_fly_mode && index_equip == item::EQUIP_INDEX_FLYSWORD && !_layer_ctrl.IsFlying())
			{
				//���ηɽ�װ����ʧЧ
				//��ʼ��������ʹ�÷ɽ�
				int type = _equipment[item::EQUIP_INDEX_FLYSWORD].type;
				if(type > 0)
				{
					UseItem(_equipment,item::EQUIP_INDEX_FLYSWORD,IL_EQUIPMENT,type,1);
				}
			}
			
			if(notify_equip_item)
			{
				PlayerGetItemInfo(IL_EQUIPMENT,index_equip);
			}		
		}
		
		IncEquipChangeFlag();
		return ;
	}
	//������Ϊ�գ�����ԭ���ģ���ˢ��װ��
	item  it1;
	_equipment.Remove(index_equip,it1);
	bool bRst = _inventory.Put(index_inv,it1);
	ASSERT(bRst);
	if(bRst)
	{
		it1.Clear();
	}
	else
	{
		//��¼������־
		GLog::log(GLOG_ERR,"װ����Ʒʱ������������");
		it1.Release();
	}
	RefreshEquipment();
	int count1 = _inventory[index_inv].count;
	ASSERT(count1 > 0);
	_runner->equip_item(index_inv,index_equip,count1,0);
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1ULL<<index_equip , 0, 0); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	IncEquipChangeFlag();
	LuaManager::GetInstance()->EventOnPlayerEquipChange(_parent->ID.id);
	if(index_equip == item::EQUIP_INDEX_WEAPON)
	{
		_real_weapon_class = _cur_item.weapon_class;
		if(_filters.IsFilterExist(FILTER_SETNEWWEAPONADDON))
		{
			_filters.RemoveFilter(FILTER_SETNEWWEAPONADDON);
		}
	}
	
	//ArenaStrongUpdate();
}

void 
gplayer_imp::PlayerMoveEquipItem(unsigned int index_inv, unsigned int index_equip)
{
	if(index_inv >= _inventory.Size() || index_equip >= _equipment.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return ;
	}
	
	item & iteq = _equipment[index_equip];
	if(iteq.type == -1) 
	{
		//װ������Ӧλ�������ݣ����ͬ��װ��
		PlayerEquipItem(index_inv,index_equip);
		return ;
	}

	const item & itin = _inventory[index_inv];
	if(itin.type == -1 || !(itin.CheckEquipPostion(index_equip)) )
	{
		//��Ʒ����Ӧλ�������ݣ����޷�װ��
		//��Ʒ����Ӧ��Ʒ�޷�װ����ָ��λ��Ҳ�޷�װ��
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}

	if(itin.type != iteq.type || iteq.count >= iteq.pile_limit)
	{
		//��Ʒ��ƥ��,���������Ѿ����ˣ��޷�װ��
		_runner->error_message(S2C::ERR_ITEM_CANNOT_EQUIP);
		return;
	}
	
	//��̬������Ʒ��������Ӱ�켼�ܵ�Ч��,����Ҫ���¼���
	bool dynskillequip = item::CheckEquipDynSkillByIndex(index_equip);
	if(dynskillequip) iteq.Deactivate(item::BODY, index_equip, this);
	int delta = _equipment.IncAmount(index_equip,itin.count);
	if(delta < 0)
	{
		ASSERT(false);
		return ;
	}
	if(dynskillequip) iteq.Activate(item::BODY, _equipment, index_equip, this);

	IncEquipChangeFlag();
	_inventory.DecAmount(index_inv,delta);
	_runner->move_equipment_item(index_inv,index_equip,delta);
}

int  
gplayer_imp::GetInstanceReenterTimeout()
{
	return world_manager::GetInstance()->GetInstanceReenterTimeout(_plane);
}

int 
gplayer_imp::GetWorldType()
{
	return world_manager::GetInstance()->GetWorldType();
}

void 
gplayer_imp::CalcEquipmentInfo()
{
	_equip_info.data.clear();
	_equipment.GetEquipmentData(_equip_info.mask,_equip_info.data);
	_parent->crc = crc16(_equip_info.data.begin(),_equip_info.data.size()) ^ _equip_info.mask;
}

namespace
{
	struct cardset_counter
	{
		unsigned int count;
		bool active;
		cardset_counter():count(0),active(false){}
	};
}

void 
gplayer_imp::RefreshEquipment()
{
	static const int TEST_EQUIP_LIST[] =
	{
		item::EQUIP_INDEX_WEAPON,
		item::EQUIP_INDEX_HEAD,
		item::EQUIP_INDEX_NECK,
		item::EQUIP_INDEX_SHOULDER,
		item::EQUIP_INDEX_BODY,
		item::EQUIP_INDEX_WAIST,
		item::EQUIP_INDEX_LEG,
		item::EQUIP_INDEX_FOOT,
		item::EQUIP_INDEX_WRIST,
		item::EQUIP_INDEX_FINGER1,
		item::EQUIP_INDEX_FINGER2,
		item::EQUIP_INDEX_PROJECTILE,
		item::EQUIP_INDEX_FLYSWORD,
		item::EQUIP_INDEX_FASHION_BODY,
		item::EQUIP_INDEX_FASHION_LEG,
		item::EQUIP_INDEX_FASHION_FOOT,
		item::EQUIP_INDEX_FASHION_WRIST,
		item::EQUIP_INDEX_FASHION_HEAD,
		item::EQUIP_INDEX_FASHION_WEAPON,
		item::EQUIP_INDEX_RUNE_SLOT,
		item::EQUIP_INDEX_BIBLE,
		item::EQUIP_INDEX_TWEAK,
		item::EQUIP_INDEX_GENERALCARD1,
		item::EQUIP_INDEX_GENERALCARD2,
		item::EQUIP_INDEX_GENERALCARD3,
		item::EQUIP_INDEX_GENERALCARD4,
		item::EQUIP_INDEX_GENERALCARD5,
		item::EQUIP_INDEX_GENERALCARD6,
		item::EQUIP_INDEX_ASTROLABE,	
		item::EQUIP_INDEX_FORCE_TICKET,	
	};
	unsigned int count = sizeof(TEST_EQUIP_LIST) / sizeof(int);
	ASSERT(count <= _equipment.Size());
	abase::vector<int , abase::fast_alloc<> > temp_list(_equipment.Size(),0);
	item::LOCATION il = _equipment.GetLocation();

	int empty_slot = count;

	//����ȡ������װ��
	for(unsigned int i = 0; i < count; i ++ )
	{
		int index = TEST_EQUIP_LIST[i];
		if(_equipment[index].type == -1 || _equipment[index].body == NULL) 
		{
			empty_slot --;
			continue;
		}

		temp_list[index] = 1;
		_equipment[index].Deactivate(il,index,this);
	}
	//�������û������ݣ���װ���ж�֮��

	DeactivateRepositoryAddons();

	property_policy::UpdatePlayerLimit(this);
	
	int tcount;
	do
	{
		tcount = 0;
		for(unsigned int i = 0; i < _equipment.Size() ; i++)
		{
			if(!temp_list[i]) continue;
			if(LuaManager::GetInstance()->EventOnCanActivateItem(_parent->ID.id, _equipment[i].type, i, il)) continue;
			ASSERT(_equipment[i].type != -1 && _equipment[i].body);
			if(_equipment[i].CanActivate(_equipment,i,this))
			{
				_equipment[i].Activate(il,_equipment,i,this);
				temp_list[i] = 0;
				empty_slot --;
				tcount = 1;

				//���������������������
				property_policy::UpdatePlayerLimit(this);
			}
		}
	}while(tcount && empty_slot);

	// New Card Refresh
	RefreshCardEquipment();

	ProtocolRepositoryHistory();
	ActivateRepositoryAddons();


	property_policy::UpdatePlayer(GetPlayerClass(),this);

	if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
	if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;

	__PRINTF("�����ٶȵ���%f\n",_en_point.flight_speed);
	
	UpdateEquipRefineLevel();

	/*170+ Códice*/
	FashionUpdateActivate(true);
}

void 
gplayer_imp::RefreshCardEquipment()
{
	typedef abase::static_map<int, cardset_counter, item::EQUIP_INDEX_GENERALCARD6-item::EQUIP_INDEX_GENERALCARD1+1> CARDSET_MAP;
	CARDSET_MAP cardset_map;
	abase::vector<const generalcard_set *, abase::fast_alloc<> > temp_list2(_equipment.Size(),NULL);
	item::LOCATION il = _equipment.GetLocation();

	for(int i = item::EQUIP_INDEX_GENERALCARD1; i <= item::EQUIP_INDEX_GENERALCARD6; i++)
	{
		item & it = _equipment[i];
		if(it.type <= 0 || !it.IsActive()) continue;
		const generalcard_set * pCardset = generalcard_set_manager::Get(it.type);
		if(pCardset == NULL) continue;

		if(it.IsGeneralCardMatchPos(i))
		{
			temp_list2[i] = pCardset;
			cardset_counter & counter = cardset_map[pCardset->id];
			if(++ counter.count >= pCardset->total_count)
			{
				counter.active = true;
				counter.count = pCardset->total_count;	
			}
		}
		else
		{		
			if(it.IsSecActive())
			{
				it.Deactivate(il,i, this);
				it.SetSecActive(false, pCardset->enhance);
				it.Activate(il,_equipment,i,this);
			}
		}
	}

	for(int i = item::EQUIP_INDEX_GENERALCARD1; i <= item::EQUIP_INDEX_GENERALCARD6; i++)
	{
		if(!temp_list2[i]) continue;
		item & it = _equipment[i];
		const generalcard_set * pCardset = temp_list2[i];
		cardset_counter & counter = cardset_map[pCardset->id];
		bool active = counter.active && counter.count;

		if(active != it.IsSecActive())
		{
			it.Deactivate(il,i, this);
			it.SetSecActive(active, HasImperialSpiritEnabled() ? pCardset->enhance + 0.05f : pCardset->enhance);
			it.Activate(il,_equipment,i,this);
		}
		if(active) --counter.count;
	}

	// Imperial Spirit
	bool all_cards_valid = true;
	int count_cards = 0;

	for (int i = item::EQUIP_INDEX_GENERALCARD1; i <= item::EQUIP_INDEX_GENERALCARD6; i++)
    {
        item& it = _equipment[i];
        if (it.type <= 0 || !it.IsActive())
            continue;

        int card_id = it.type;
        bool is_valid_card = (card_id == 41784 || card_id == 41785 || card_id == 41786 || card_id == 41787 || card_id == 41831 || card_id == 41832);

		if(!is_valid_card) 
		{
			all_cards_valid = false;
			break;
		} else
        {
			count_cards++;

            if (it.body->GetCardLevel() < 80 || it.body->GetRebirthTimes() < 2)
            {				
                all_cards_valid = false;
                break;
            }
        }
    }

	if(count_cards < 6) all_cards_valid = false;

    ImperialSpirtHeartBeat(all_cards_valid);

}

bool
gplayer_imp::EquipItem(unsigned int index_inv,unsigned int index_equip)
{	
	ASSERT(index_inv < _inventory.Size());
	ASSERT(_inventory[index_inv].type != -1);
	ASSERT(_inventory[index_inv].CheckEquipPostion(index_equip));

	item & it = _inventory[index_inv];
	item & iteq = _equipment[index_equip];
	//���Ƽ���ʱ������ͳ������Ϊ��ֹ�¿��Ʋ����㼤���������Ȱ�ԭ����ȡ������
	bool need_deactivate = (index_equip >= item::EQUIP_INDEX_GENERALCARD1 && index_equip <= item::EQUIP_INDEX_GENERALCARD6 && iteq.type != -1);
	if(need_deactivate) iteq.Deactivate(item::BODY, index_equip, this);
	if(!it.CanActivate(_equipment,index_equip,this))
	{
		if(need_deactivate) iteq.Activate(item::BODY, _equipment, index_equip, this);
		return false;	
	}

	item  it1;
	item  it2;
	_inventory.Remove(index_inv, it1);
	_equipment.Remove(index_equip, it2);

	_equipment.Put(index_equip,it1);
	_inventory.Put(index_inv,it2);
	it1.Clear();
	it2.Clear();

	RefreshEquipment();
	return true;
}

// Weapon Update
void gplayer_imp::PlayerWeaponUpdateEnterWorld()
{
 	int equip_idx = item::EQUIP_INDEX_WEAPON;
 	item& it_eq = _equipment[equip_idx];
	
	if(it_eq.type > 0)
	{
		std::string ITEM1, ITEM2;
		it_eq.DumpDetail(ITEM1);

		// Desativa a Arma
		X_EQUIP id0;
		id0.type = it_eq.type;
		id0.mask = it_eq.GetIdModify();
		it_eq.Deactivate(item::BODY, equip_idx, this);

		// Ativa a Arma
		it_eq.Activate(item::BODY, _equipment, equip_idx, this);
		it_eq.DumpDetail(ITEM2);

		// Atualiza a Arma
		PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
		RefreshEquipment();

		X_EQUIP id1;
		id1.type = _equipment[equip_idx].type;
		id1.mask = _equipment[equip_idx].GetIdModify();
		if (id1.type != id0.type || id1.mask != id0.mask)
		{
			CalcEquipmentInfo();
			_runner->equipment_info_changed(1ULL << equip_idx, 0, &id1, sizeof(X_EQUIP));
		}
	}
}


void 
gplayer_imp::PlayerGetInventory(int where)
{
	//printf("PlayerGetInventory");
	abase::octets buf;
	unsigned char size;
	switch(where)
	{
		case IL_INVENTORY:
			_inventory.SimpleSave(buf);
			size = _inventory.Size();
			break;
		case IL_EQUIPMENT:
			_equipment.SimpleSave(buf);
			size = _equipment.Size();
			break;
		case IL_TASK_INVENTORY:
			_task_inventory.SimpleSave(buf);
			size = _task_inventory.Size();
			break;
		default:
		ASSERT(false);
		return;
	}
	_runner->self_inventory_data(where, size, buf.begin(), buf.size());
}

void 
gplayer_imp::PlayerGetProperty()
{
	_runner->self_get_property(_basic.status_point,_cur_prop,
								_attack_degree, _defend_degree, 
								_crit_rate+_base_crit_rate, _crit_damage_bonus, 
								((gplayer*)_parent)->invisible_degree, ((gplayer*)_parent)->anti_invisible_degree,
								_penetration, _resilience, GetVigour(), _anti_defense_degree,_anti_resistance_degree, _base_prop, _highest_level, 0);

}

void
gplayer_imp::PlayerSetStatusPoint(unsigned int vit, unsigned int eng, unsigned int str, unsigned int agi)
{
	unsigned int remain  = _basic.status_point;
	if(str > remain || agi > remain || vit > remain || eng > remain || (str+agi+vit+eng > remain) )
	{
		_runner->set_status_point(0,0,0,0,remain);
		return ;
	}
	player_template::UpdateBasic(GetPlayerClass(),_base_prop,vit,eng,str,agi);
	_cur_prop.vitality += vit;
	_cur_prop.energy   += eng;
	_cur_prop.strength += str;
	_cur_prop.agility  += agi;
	remain -= ( vit + eng + str + agi);
	_basic.status_point = remain;
	RefreshEquipment();
	_runner->set_status_point(vit,eng,str,agi,remain);
}

bool
gplayer_imp::LongJump(const A3DVECTOR &pos,int target_tag, int contrl_id)
{
	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_SIT_DOWN 
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return false;
	}
	if(target_tag == world_manager::GetWorldTag()) 
	{
		if(!LongJump(pos))
		  return false;
		return true;
	}
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;
	key.control_id = contrl_id;

	//��Player���и������� 
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,target_tag,key,0) < 0)
	{
		return false;
	}
	return true;
}


bool  
gplayer_imp::LongJump(const A3DVECTOR &pos)
{
	if(GetPlayerLimit(PLAYER_LIMIT_NOLONGJUMP))//��ֹ��ת
	{
		return false;
	}


	if(_plane->PosInWorld(pos))
	{
		//Ŀ���������У�ֱ�ӽ�����ת����

		__PRINTF("player goto %f %f\n",pos.x,pos.z);
		_runner->notify_pos(pos);
		A3DVECTOR newpos = pos;
		newpos -= _parent->pos;
		bool bRst = StepMove(newpos);
		//$$$$$$$$�����MoveModeӦ��ʹ�����⴦��
		_runner->stop_move(_parent->pos,0x1000,_parent->dir,0x01);
		ASSERT(bRst);
		//�����Ƿ��ڰ�ȫ��
		TestSanctuary();
		
		if(!world_manager::GetWorldParam().pve_mode  || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
		{
			//�����Ƿ������ְ�ȫ��
			TestPKProtected();
		}

		_ph_control.Initialize(this);
	}
	else 
	{	
		//Ŀ�겻���Լ��������У�����ȫ������
		//�˲����޷�Ҳ����Խ��tag ��ͬ��λ��
		int dest = _plane->GetGlobalServer(pos);
		if(dest <0)
		{
			_runner->error_message(0);
			return false;
		}
		//���з������л��Ĳ���
		//Ŀǰ�������ﲢû�и����������л�����
		//���Ҫ���Ǻ�����
		_commander->SwitchSvr(dest,_parent->pos,pos,0);
	}
	return true;
}

void 
gplayer_imp::PlayerGoto(const A3DVECTOR & pos)
{
	LongJump(pos);
}

bool 
gplayer_imp::Resurrect(float exp_reduce)
{
	if(!_parent->IsZombie()) return false;
	if(_player_state != PLAYER_STATE_NORMAL)  return false;
	session_resurrect * pSession= new session_resurrect(this);
	pSession->SetExpReduce(exp_reduce);
	if(AddSession(pSession)) StartSession();
	return true;
}

bool 
gplayer_imp::CanResurrect(int param)
{
	//�ж��Ƿ��ܹ����и������
	//��������״̬���ܹ������
	return (_player_state == PLAYER_STATE_NORMAL || _player_state == PLAYER_SIT_DOWN);
}

int 
gplayer_imp::Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int world_tag,float hp_factor, float mp_factor, int param, float ap_factor, int extra_invincible_time)
{
	exp_reduce = exp_reduce * (100 - _resurrect_exp_lost_reduce) * 0.01f;
	if(_kill_by_player) exp_reduce = 0.f;
	//�������� ���ȸ����Լ�
	_parent->b_zombie = false;
	
	//���Լ���hp��mp��Ϊԭʼ��һ��
	_basic.hp = (int)(_cur_prop.max_hp * hp_factor + 0.5f);
	_basic.mp = (int)(_cur_prop.max_mp * mp_factor + 0.5f);
    _basic.ap = (int)(_cur_prop.max_ap * ap_factor + 0.5f);

	SetRefreshState();
	_enemy_list.clear();

	((gplayer_controller*)_commander)->OnResurrect();

	//���������е�session
	ClearNextSession();

	
	if(!nomove)
	{
		_runner->resurrect(0);
		LongJump(pos,world_tag);
	}
	else
	{
		//������ʱԭ�ظ���

		//��ʱ�����޵кͲ��ɲ���״̬(�����µ�session) 
		AddSession(new session_resurrect_protect(this, extra_invincible_time));
		StartSession();


	}
	int exp = player_template::GetLvlupExp(GetPlayerClass(),_basic.level);
	int rexp = (int)(exp * exp_reduce + 0.5f);
	if(rexp > 0)
	{
		int new_exp = (_basic.exp - rexp);
	//	if(new_exp < -exp) new_exp = -exp;
		if(new_exp < 0) new_exp = 0;	//����ֵ�����ɸ���...
		if(_basic.level >= player_template::GetMaxLevel()) new_exp = 0;
//		_runner->receive_exp(new_exp - _basic.exp,0);
		rexp = new_exp - _basic.exp;
		_basic.exp = new_exp;
	}

	//��¼������־
	GLog::log(GLOG_INFO,"�û�%d��������(%d) ��ʧ����%d(%2.2f)", _parent->ID.id, nomove?1:0,rexp,exp_reduce);
	return 0;
}

int 
gplayer_imp::DispatchCommand(int cmd_type, const void * buf,unsigned int size)
{
	ASSERT(_commander);
	switch(_player_state)
	{
		case PLAYER_STATE_NORMAL:
			if((gplayer*)_parent->IsZombie())
				return ((gplayer_controller*)_commander)->ZombieCommandHandler(cmd_type,buf,size);
			else if(_idle_mode_flag + _seal_mode_flag)
				return ((gplayer_controller*)_commander)->SealedCommandHandler(cmd_type,buf,size);
			else
				return _commander->CommandHandler(cmd_type,buf,size);
		break;

		case PLAYER_STATE_COSMETIC:
			//��������ģʽ�ܹ�����ʲô��Ϣ
			return ((gplayer_controller*)_commander)->CosmeticCommandHandler(cmd_type,buf,size);
		break;

		case PLAYER_WAITING_FACTION_TRADE:
		case PLAYER_WAITING_TRADE:
		case PLAYER_WAIT_FACTION_TRADE_READ:
		case PLAYER_TRADE:
		case PLAYER_WAIT_TRADE_COMPLETE:
		case PLAYER_WAIT_TRADE_READ:
		case PLAYER_WAIT_LOGOUT:
		case PLAYER_DISCONNECT:
		break;
		case PLAYER_SIT_DOWN:
			return ((gplayer_controller*)_commander)->StayInCommandHandler(cmd_type,buf,size);
		break;

		case PLAYER_STATE_BIND:
			if((gplayer*)_parent->IsZombie())
				return ((gplayer_controller*)_commander)->ZombieCommandHandler(cmd_type,buf,size);
			else
				return ((gplayer_controller*)_commander)->BoundCommandHandler(cmd_type,buf,size);
		break;

		case PLAYER_STATE_MARKET:
			return ((gplayer_controller*)_commander)->MarketCommandHandler(cmd_type,buf,size);
		break;
		
		case PLAYER_STATE_TRAVEL:
			return ((gplayer_controller*)_commander)->TravelCommandHandler(cmd_type,buf,size);
		break;
		

		case PLAYER_WAIT_SWITCH:
		//������ʱ����������µ�����,�������ĳ���
		break;
		default:
		ASSERT(false);
	}
	return 0;
}

void
gplayer_imp::PickupMoneyInTrade(unsigned int money)
{
	((gplayer_dispatcher*)_runner)->pickup_money_in_trade(money);
	_trade_obj->PickupMoney(money);
}

void 
gplayer_imp::PickupItemInTrade(const A3DVECTOR & pos,const void * data, unsigned int size,bool isTeam, int drop_id)
{
	if(size < sizeof(item_data)) return ;
	item_data *pData = (item_data*)data;
	if(size != sizeof(item_data) + pData->content_length)
	{
		ASSERT(false && " invalid item data size");
		return;
	}
	pData->item_content = (char*)data + sizeof(item_data);
	int ocount = pData->count;

	if(drop_id)
	{
		GLog::log(GLOG_INFO,"�û�%d����ʱ����%d��%d[�û�%d����]",_parent->ID.id,ocount, pData->type,drop_id);
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����ʱ����%d��%d",_parent->ID.id,ocount, pData->type);
		FirstAcquireItem(pData);

	}


	if(isTeam && _team.IsInTeam() && _team.IsRandomPickup())
	{
		//�������ȡ֪ͨ���� �쵽����Ʒ
		_team.NotifyTeamPickup(pos,pData->type,pData->count);
	}

	int rst = _trade_obj->PickupItem(pData);
	if(rst >=0)
	{
		//���ͼ�����Ʒ����Ϣ
		//Ҫע����ܻ�ʣ����Ʒ
		__PRINTF("��������Ʒ%d %d to %d\n",pData->type,pData->classid,rst);
		((gplayer_dispatcher*)_runner)->pickup_item_in_trade(pData->type,ocount - pData->count);
	}

	if(pData->count)
	{
		//û����ȫ������,Ӧ�ý���Ʒ�����׳�
		//δ���ܹ������׳�(���ڷ�����Ʒ��ԭ��)
		DropItemFromData(_plane,_parent->pos,*pData,_parent->ID,0,0,drop_id);
		__PRINTF("����ʧ�ܣ�δ��ȫ����%d %d %d/%d\n",pData->type,pData->classid,pData->count,ocount);
	}
}

void 
gplayer_imp::PurchaseItem(abase::pair<const item_data *, int>* pItem,unsigned int size,unsigned int money, int consume_contrib, int force_contrib)
{
	ASSERT(GetAllMoney() >= money && size && GetFactionConsumeContrib() >= consume_contrib && _player_force.GetContribution() >= force_contrib && "����֮ǰ�Ѿ��������"  );
	int rst = 0;
	packet_wrapper h1(128);
	using namespace S2C;
	CMD::Make<CMD::player_purchase_item>::FirstStep(h1,(unsigned int)0,(unsigned int)0,size);

	char logtxt[24*14+512];	//������๺��12����Ʒ
	int index = 0;

	for(unsigned int i = 0; i < size; i ++,pItem++)
	{
		int count = pItem->second;
		rst =_inventory.Push(*pItem->first,count,0);
		ASSERT(rst >= 0 && count == 0);
        _inventory[rst].InitFromShop();

		CMD::Make<CMD::player_purchase_item>::SecondStep(h1,pItem->first, pItem->second,rst);
		index += snprintf(logtxt + index,sizeof(logtxt) - index,"%d��%d,",pItem->second,pItem->first->type);
	}
	ASSERT(index);
	index --;
	logtxt[index] = 0;
	GLog::log(GLOG_INFO,"�û�%d��NPC������%s",_parent->ID.id, logtxt);
	SpendAllMoney(money);
	if(consume_contrib > 0) DecFactionContrib(consume_contrib,0);
	if(force_contrib > 0) DecForceContribution(force_contrib);
	send_ls_msg(GetParent(), h1);
	SelfPlayerMoney();
}

void 
gplayer_imp::OnPickupItem(const A3DVECTOR &pos,const void * data, unsigned int size,bool isTeam,int drop_id)
{
	if(size < sizeof(item_data)) return ;
	item_data *pData = (item_data*)data;
	if(size != sizeof(item_data) + pData->content_length)
	{
		ASSERT(false && " invalid item data size");
		return;
	}
	pData->item_content = (char*)data + sizeof(item_data);
	int ocount = pData->count;

	if(drop_id)
	{
		GLog::log(GLOG_INFO,"�û�%d����%d��%d[�û�%d����]",_parent->ID.id,ocount, pData->type,drop_id);
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����%d��%d",_parent->ID.id,ocount, pData->type);
		FirstAcquireItem(pData);
	}

	if(isTeam && _team.IsInTeam())// && _team.IsRandomPickup())
	{
		//�������ȡ֪ͨ���� �쵽����Ʒ
		_team.NotifyTeamPickup(pos,pData->type,pData->count);
	}

	int rst = _inventory.Push(*pData);
	if(rst >=0)
	{
		//���ͼ�����Ʒ����Ϣ
		//Ҫע����ܻ�ʣ����Ʒ
		__PRINTF("��������Ʒ%d %d to %d\n",pData->type,pData->classid,rst);
		_runner->pickup_item(pData->type,pData->expire_date, ocount - pData->count,_inventory[rst].count, 0,rst);
		if(pData->proc_type & item::ITEM_PROC_TYPE_AUTO_USE)
		{
			//�Զ�ʹ��
			UseItem(_inventory, rst, IL_INVENTORY, pData->type, 1);
		}
	}

	if(pData->count)
	{
		//û����ȫ������,Ӧ�ý���Ʒ�����׳�
		if(isTeam)
			DropItemFromData(_plane,_parent->pos,*pData,_parent->ID,0,0,drop_id);
		else
			DropItemFromData(_plane,pos,*pData,_parent->ID,0,0,drop_id);
		__PRINTF("����ʧ�ܣ�δ��ȫ����%d %d %d/%d\n",pData->type,pData->classid,pData->count,ocount);
	}

	LuaManager::GetInstance()->EventPlayerObtianItem(_parent->ID.id, rst, pData->type, ocount - pData->count, pData->proc_type);
}

bool
gplayer_imp::ObtainItem(int where, item_data * pData, bool isTask)
{
	ASSERT(where != IL_EQUIPMENT);
	int ocount = pData->count;
	item_list & inv = GetInventory(where);
	int rst = inv.Push(*pData);
	if(rst >=0)
	{
		if(isTask)
			_runner->task_deliver_item(pData->type,pData->expire_date,ocount - pData->count,inv[rst].count, where,rst);
		else
			_runner->obtain_item(pData->type,pData->expire_date,ocount - pData->count,inv[rst].count, where,rst);

		if(where == IL_INVENTORY && pData->proc_type & item::ITEM_PROC_TYPE_AUTO_USE)
		{
			UseItem(_inventory, rst, IL_INVENTORY, pData->type, 1);
		}
	}
	FirstAcquireItem(pData);

	if(pData->count && !isTask)
	{
		//Ӧ�ò���ʣ����Ʒ�Ŷ�
		//����Ϊ�˱��գ������ж�һ��
		DropItemData(_plane,_parent->pos,pData,_parent->ID,0,0,0);
		return false;
	}

	LuaManager::GetInstance()->EventPlayerObtianItem(_parent->ID.id, rst, pData->type, ocount - pData->count, pData->proc_type);
	return true;
}


bool
gplayer_imp::StepMove(const A3DVECTOR &offset)
{
	_direction = offset;
	_is_moved = true;
	bool bRst = false;
	if((bRst = gobject_imp::StepMove(offset)))
	{
		TestUnderWater();
		if(_player_state == PLAYER_STATE_BIND)
		{
			if(_bind_player.IsPlayerLinked())
			{
				//ֻ�����汻����˲Żᷢ��Ϣ 
				SendTo<0>(GM_MSG_PLAYER_BIND_FOLLOW,_bind_player.GetLinkedPlayer(),0);
			}
		}
		UpdatePlayerLayer();
		//����ƶ�ʱ��������ʱ��
		SetActiveStateDelay(MAX_ACTIVE_STATE_DELAY);
	}
	return bRst;
}

class PlayerForceOfflineTask :public ONET::Thread::Runnable
{
	gplayer * _player;
	int _uid;
	int _cs_sid;
	int _cs_index;
public:
	PlayerForceOfflineTask(gplayer *pPlayer):_player(pPlayer)
						 ,_uid(pPlayer->ID.id),_cs_sid(pPlayer->cs_sid)
						 ,_cs_index(pPlayer->cs_index)
	{
	}
	
	virtual void Run()
	{
		spin_autolock keeper(_player->spinlock);
		if(!_player->IsActived() || _player->ID.id != _uid  ||
				_player->cs_index != _cs_index || _player->cs_sid != _cs_sid || !_player->imp)
		{
			//����û����������Ѿ���ʧ�ˣ�����ֱ�ӷ��أ������д���
			GLog::log(GLOG_WARNING,"�û�%d�쳣����ʱ���û��Ѿ���ʧ",_uid);
			return;
		}
		GLog::log(GLOG_WARNING,"�û�%d��Ϊ���ݴ�������쳣���߲���",_uid);
		_player->imp->PlayerForceOffline();
		delete this;
	}
};

void 
gplayer_imp::OnHeartbeat(unsigned int tick)
{
	LuaManager * lua = LuaManager::GetInstance();

	// Reseta os pontos diários
	CheckRealmDay();
	//CheckQuestionDay();


//	__PRINTF("player %d heart beat\n", _parent->ID.id);
	_filters.EF_Heartbeat(tick);
	if(_combat_timer)
	{
		_combat_timer --;
		if(_combat_timer <=0)
		{
			ActiveCombatState(false);
			_combat_timer = 0;
		}
	}
	if(_pvp_enable_flag)
	{
		//����PK����ʱ
		if((--_pvp_cooldown) < 0 || (_pvp_cooldown > lua->GetConfig()->pvp_cooldown_time && world_manager::GetWorldParam().pve_mode) ) _pvp_cooldown = 0;
		//if((_pvp_cooldown-=100) < 0) _pvp_cooldown = 0;
	}

	_plane->InspireNPC<0>(_parent->pPiece,0);

	bool bSaveMode = _player_state == PLAYER_STATE_NORMAL || _player_state == PLAYER_SIT_DOWN || _player_state == PLAYER_STATE_MARKET || _player_state == PLAYER_STATE_BIND; 	//���ʾ��ǰ�Ƿ���Դ���
	bool is_zombie = _parent->IsZombie();
	int cur_time = g_timer.get_systime();

	//auto gen hp/mp
	if(!is_zombie)
	{
		bool no_amulet = GetPlayerLimit(PLAYER_LIMIT_NOAMULET);
		//���л�Ѫҩ�ͻ�ħҩ�ļ��
		if(_auto_hp_value  > 0 && !no_amulet && !world_manager::GetWorldLimit().noauto_genhp)
		{
			if(_auto_hp_percent * _cur_prop.max_hp > _basic.hp)
			{
				int offset = _cur_prop.max_hp - _basic.hp;
				_auto_hp_value = AutoGenStat(COOLDOWN_INDEX_AUTO_HP,
						item::EQUIP_INDEX_HP_ADDON,offset, bSaveMode);
			}
		}

		if(_auto_mp_value  > 0 && !no_amulet && !world_manager::GetWorldLimit().noauto_genmp)
		{
			if(_auto_mp_percent * _cur_prop.max_mp > _basic.mp)
			{
				int offset = _cur_prop.max_mp - _basic.mp;
				_auto_mp_value = AutoGenStat(COOLDOWN_INDEX_AUTO_MP,
						item::EQUIP_INDEX_MP_ADDON,offset, bSaveMode);
			}
		}

		if(IsCombatState())
		{
			GenHPandMP(_cur_prop.hp_gen,_cur_prop.mp_gen);
		}
		else
		{
			GenHPandMP(_cur_prop.hp_gen*4,_cur_prop.mp_gen*4);
		}
		_breath.OnHeartbeat(this);

	}
	
	if(!is_zombie && (cur_time & 0x03) == 0)
	{
		//ÿ4�������һ�γ��
		ENEMY_LIST::iterator it = _enemy_list.end(); 
		for(;it > _enemy_list.begin(); )
		{
			--it;
			if(it->second <=0) 
			{
				__PRINTF("ɾ���˳�ʱ�ĳ�޼�¼ ����%x\n",it->first);
				it = _enemy_list.erase(it);
			}
			else
			{
				__PRINTF("����%x�ĳ�޼�¼ ��%d������\n",it->first,it->second);
				it->second = 0;
			}
		}
	}

	//���������ԭ�����ڻ�Ѫ�ͼ�Ѫ֮��������ݸ���֮ǰ��������������ȷ�ķ�������
	_team.OnHeartbeat();
	_invade_ctrl.OnHeartbeat();

	_touch_order.OnHeartbeat(this);
	_player_giftcard.OnHeartbeat(this);
	_player_title.OnHeartbeat(cur_time);	
	_player_fatering.OnHeartbeat(cur_time);
    _player_sanctuary_check.OnHeartbeat(this);
	_player_clock.OnHeartbeat(this,cur_time,InCentralServer());

	if(_expire_item_date && bSaveMode)
	{       
		if(cur_time >= _expire_item_date)
		{       
			//Ӧ�ý���ɾ��������
			RemoveAllExpireItems();
		}       
	}
	
	//lgc
	if(!is_zombie)
	{
		if(_cur_elf_info.id != (unsigned int)-1)
		{
			if(  lua->IsTrue(world_manager::GetWorldTag() , lua->GetConfig()->ELF_DISTABLE_LOCATIONS ) )
			{
			_cur_elf_info.vigor = 0;
			}
			else
			{
				//С����ظ�Ԫ��  
				if(_cur_elf_info.vigor < _cur_elf_info.max_vigor)
				{
					_cur_elf_info.vigor += _cur_elf_info.vigor_gen;
					if(_cur_elf_info.vigor > _cur_elf_info.max_vigor)
						_cur_elf_info.vigor = _cur_elf_info.max_vigor;
					_cur_elf_info.refresh_vigor = true;
				}
				//С����ľ���������������
				if(_cur_elf_info.refine_effect_active)
				{
					item & it = _equipment[item::EQUIP_INDEX_ELF];
					int second_cost = elf_refine_effect_table[_cur_elf_info.refine_level].std_cost * (_basic.level  + 105)/210;
					int cur_stamina = it.GetStamina();
					if(cur_stamina <= second_cost)
					{
						ElfRefineDeactivate(_cur_elf_info.refine_level);
						_runner->elf_refine_activate(0);

						it.DecStamina(cur_stamina);
						//֪ͨ�ͻ��˼�������
						_runner->query_elf_stamina(0);
					}
					else
					{
						it.DecStamina(second_cost);
						//֪ͨ�ͻ��˼�������
						_runner->query_elf_stamina(cur_stamina - second_cost);
					}
				}
			}
		}
	}
	// ����Ƿ���С�����ת��״̬�ѽ���
	if(_min_elf_status_value > 0 && bSaveMode)
	{       
		if(cur_time >= _min_elf_status_value)
			UpdateAllElfSecureStatus();
	}
	//����Ƿ��и��������ѹ���
	if(_min_addon_expire_date > 0 && bSaveMode)
	{
		if(cur_time >= _min_addon_expire_date)
			RemoveAllExpireAddon();
	}
	
	//�ж��Ƿ���� ֻ����ͨģʽ��������ж�
	if(--_write_timer <= 0)
	{
		if(bSaveMode)
		{
			AutoSaveData();
		}
		_write_timer = abase::Rand(500, 513);
	}

	//�ж��Ƿ������������Ϣ
	if(--_link_notify_timer <= 0)
	{

		//����Ҫ�ж��Լ��Ƿ��ܹ���ѯ���Լ���һ���취���ڴ��̵�ʱ���飩
		//��������޷���ѯ���Լ��Ļ��������������⣬������������ɾ������
		//��ΪHeartbeat��ʱ����Ҫ����һЩ����
		gplayer *pPlayer = GetParent();
		int index1;
		gplayer * pPlayer2 = world_manager::GetInstance()->FindPlayer(pPlayer->ID.id,index1);
		if(pPlayer2 != pPlayer || pPlayer2 == NULL)
		{
			//�������뽫�Լ��ͷ��Ҳ�����
			//����һ�����ܵ�����
			GLog::log(GLOG_WARNING,"%d�û�%d(%d,%d)���ݷ������󣬿�ʼ�쳣����(%d)",world_manager::GetWorldTag(),pPlayer->ID.id,pPlayer->cs_index, pPlayer->cs_sid,pPlayer2?1:0);
			ONET::Thread::Pool::AddTask(new PlayerForceOfflineTask(pPlayer));
		}
		else if(_player_state != PLAYER_DISCONNECT)
		{
			_link_notify_timer = LINK_NOTIFY_TIMER;
			GMSV::SendPlayerHeartbeat(pPlayer->cs_index,pPlayer->ID.id, pPlayer->cs_sid);
			__PRINTF("����������link������ %d\n",pPlayer->ID.id);
			//�����¼��־�ƺ������ ������һ�¼�¼��־�İ취

			//���������������־����
			if(abase::Rand(0,5) == 0)
			{
				GLog::log(GLOG_INFO,"%d�����û�%d(%d,%d)������Ϣ",world_manager::GetWorldTag(),pPlayer->ID.id,pPlayer->cs_index,pPlayer->cs_sid);
			}

			bool pos_log = false;
			if(_player_state == PLAYER_STATE_NORMAL)
			{
				if(abase::Rand(0,4) == 0)
				{
					pos_log = true;
				}
			}
			else
			{
				if(abase::Rand(0,8) == 0)
				{
					pos_log = true;
				}
			}
			if(pos_log)
			{
				GLog::log(GLOG_INFO,"�û�%d����λ��(%f,%f,%f)[%d],״̬%d",_parent->ID.id,_parent->pos.x,_parent->pos.y,_parent->pos.z,world_manager::GetWorldTag(), _layer_ctrl.GetLayer());
			}
		}


	}

	//�ж��Ƿ�˫��ʱ��
	if(_double_exp_mode)
	{
		if(cur_time > _double_exp_timeout)
		{
			LeaveDoubleExpMode();
		}
	}

	PVPCombatHeartbeat();
	_duel.Heartbeat(this);
	_bind_player.Heartbeat(this);
	_petman.Heartbeat(this);
	_plantpetman.Heartbeat(this);
	_meridianman.Heartbeat(this,cur_time);
	
	if ( _player_state == 9 && _combat_timer <= 0 ) 
		ModifyAP(lua->GetConfig()->shutdown_ap);
	if ( IsInTeam() && lua->IsTrue(world_manager::GetWorldTag() , lua->GetConfig()->PATTY_DISTABLE_TAGS ) ) 
		_team.CliLeaveParty();
	
	if (!(_write_timer % 10))
	LuaManager::GetInstance()->EventPlayerHeartBeat(_parent->ID.id,tick);
	ShieldEnergyHeartBeat();
	
	if ((signed)_inviting_tm.time > 0)
	{
		_inviting_tm.time--;
	}
	else
	{
		_inviting_tm.id = 0;
		_inviting_tm.time = 0;
	}

	if (!(_write_timer % 1))
	{
		_charge_merc_time++;

		if (_charge_merc_time >= 15)
		{
			ChargeMercHeartBeat();
		}
	}

	if (_kid_transformation)
	{
		if (!(_write_timer % 1))
		{
			_kid_transformation_time--;

			if(_kid_transformation_time <= 0)
			{
				KidCelestialTransformation(0);
			}
		}
	}
	
	if (!(_write_timer % 15)) // Verifica a cada 15 segundos
	{
		SetAnecdotePoints();
		FixExpHeartBeat();
		/*160+*/
		SpeedSkillManager();
		ActivityEventActivate();

		if(_activity.enable_skill)
		{
		ActivityEventSkillManager();
		}
	}

	// Espírito Imperial
	if (!(_write_timer % 3600)) // Verifica a cada 1 hora
	{
		RefreshCardEquipment();		
	}

	/*176+*/
	if(EmulateSettings::GetInstance()->GetEmulateVersion() >= 176)
	{
		if (!(_write_timer % 3) && !_check_interface)
		{
			if (_glua.GetPortatilPictureActive() > 0)
			{
				SetPortatilPicture(_glua.GetPortatilPictureActive());
				_check_interface = true;
			}
		}
	}
	

	/*171+*/
	if (!(_write_timer % 5) && !_check_genesis_lvl)
	{
		PlayerWeaponUpdateEnterWorld();			
		_check_genesis_lvl = true;
	}
	if (!(_write_timer % 5))
	{
		/*170+ Códice*/
		FashionUpdateActivate(true);				
	}
	

	//��������ϵͳ
	/*
	if(world_manager::AntiWallow())
	{
		int new_wallow = _wallow_obj.Tick(g_timer.get_systime());
		if(new_wallow != _wallow_level)
		{
			//����״̬�����仯
			_wallow_level = new_wallow;

			//֪ͨ�ͻ���
			time_t l_time;
			time_t h_time;
			int ptime;
			_wallow_obj.GetTimeLeft(&l_time, &h_time, &ptime);
			_runner->player_wallow_info(_wallow_level, ptime, l_time, h_time);
		}
	}
	*/

	if(_cheat_mode > 0)
	{
		_cheat_mode --;
		if(_cheat_mode <= 0) 
		{
			_cheat_mode = -1;
			PunishCheat();
		}
	}

	if(world_manager::GetWorldLimit().height_limit)
	{
		if(_parent->pos.y >= world_manager::GetHeightLimit())
		{               
			_parent->pos.y = 0;
		}                       
	}

	if(cur_time - _profit_timestamp >= TOTAL_SEC_PER_DAY)
	{
		//��������ʱ��
		time_t now = g_timer.get_systime();
		struct tm *tm_now = localtime(&now);
		ASSERT(tm_now);
		_profit_time = world_manager::GetWorldConfig().profit_time;
		_profit_timestamp = now - tm_now->tm_hour*3600 - tm_now->tm_min*60 - tm_now->tm_sec;
		CalcProfitLevel();
		_runner->update_profit_time(S2C::CMD::player_profit_time::PROFIT_LEVEL_CHANGE,_profit_time, _profit_level);
	}

	if(world_manager::ProfitTimeLimit())
	{
		if(IsCombatState() && IsAttackMonster())
		{
			//���������ͼ1
			//ս�����ʱ��������ʱ��
			UpdateProfitTime();
		}
	}
	else if(world_manager::ProfitTimeLimit2())
	{
		if(_active_state_delay > 0)
		{
			//���������ͼ2
			//ս����������ֹͣ�ƶ�����ӳ�����
			UpdateProfitTime();
			if(--_active_state_delay <= 0)
			{
				_runner->notify_profit_state(0);
			}
		}
	}

	_multi_exp_ctrl.Update(this,cur_time);
	_online_award.Update(this,cur_time);

	if(_country_expire_time && cur_time >= _country_expire_time)
	{
		GMSV::CountryBattleLeave(_parent->ID.id, GetCountryId(), Get16Por9JWeapon(), GetSoulPower());
		SetCountryId(0,0);
	}
	if(_king_expire_time && cur_time >= _king_expire_time)
	{
		SetKing(false, 0);	
	}

	if(_need_refresh_equipment)
	{
		RefreshEquipment();
		PlayerGetProperty();	

		_need_refresh_equipment = false;
	}
	
	if (auto_assist && IsInTeam() && cur_time >= check_assist_next_time)
	{
		check_assist_next_time = cur_time + 3; // set next check time
		ChangeLeaderAssist();
	}

	//QuestionHeartBeat();

	//GetServerTime();
}

void gplayer_imp::TryClearTBChangeCounter()
{
	_tb_change_counter = _trash_box_open_flag?1:0;
}

void gplayer_imp::TryClearUserTBChangeCounter()
{
	_user_tb_change_counter = _user_trash_box_open_flag?1:0;
}

void 
gplayer_imp::AutoSaveData()
{
	//write
	class AutoWrite: public GDB::Result, public abase::ASmallObject
	{	
		world * _plane;
		int _userid;
		unsigned int _counter;
		unsigned int _counter2; 
		unsigned int _counter3;
		int _mall_order_id;
	public:
		AutoWrite(gplayer_imp * imp)
		{
			_plane = imp->_plane;
			_userid = imp->_parent->ID.id;
			_counter = imp->_tb_change_counter;
			_counter2 = imp->_eq_change_counter;
			_counter3 = imp->_user_tb_change_counter;
			_mall_order_id = imp->_mall_order_id;
		}

		//ƽʱ���̲�������
		virtual void OnTimeOut() { OnFailed();}
		//�����ܵ�������� 
		virtual void OnFailed() {
			GLog::log(GLOG_WARNING,"��%d�Զ����̱����û�%d����ʧ��",world_manager::GetWorldTag(),_userid);
			MSG msg;
			BuildMessage(msg,GM_MSG_DBSAVE_ERROR,XID(GM_TYPE_PLAYER,_userid),XID(GM_TYPE_PLAYER,_userid),A3DVECTOR(0,0,0));
			//���_plane��һ������ʹ�� ������manager�Ľӿ������
			world_manager::GetInstance()->SendRemotePlayerMsg(_userid,msg);
			delete this;
		}

		virtual void OnPutRole(int retcode, GDB::PutRoleResData *data){
			//��ͼѰ��һ����ң�����Ʒ���д���־��Ϊfalse
			ASSERT(retcode == 0);

			int index = _plane->FindPlayer(_userid);
			if(index >= 0)
			{
				gplayer * pPlayer = _plane->GetPlayerByIndex(index);
				spin_autolock keeper(pPlayer->spinlock);
				if(pPlayer->IsActived() && pPlayer->imp && pPlayer->ID.id == _userid
						&& pPlayer->login_state >= gplayer::LOGIN_OK)
				{
					gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
					//ֻ��û���ٴδ򿪹���û�����ڴ����ӣ��Ż�������ӵĸ��ļ��� 
					if(_counter && pImp->_tb_change_counter == _counter)
					{
						pImp->TryClearTBChangeCounter();
					}

					if(_counter2 == pImp->_eq_change_counter )
					{
						pImp->_eq_change_counter = 0;
					}
					//ֻ��û���ٴδ򿪹���û�����ڴ����ӣ��Ż�������ӵĸ��ļ��� 
					if(_counter3 && pImp->_user_tb_change_counter == _counter3)
					{
						pImp->TryClearUserTBChangeCounter();
					}

					pImp->MallSaveDone(_mall_order_id);

					//�����̴�����0
					pImp->_db_save_error = 0;

					HandleResData(data, pImp);
				}
				
			}
			GLog::log(GLOG_INFO,"�û�%d�����Զ��������",_userid);
			delete this;
		}
		
		void HandleResData(GDB::PutRoleResData *data, gplayer_imp * pImp)
		{
			gplayer * pPlayer = (gplayer*)(pImp->_parent);
			pImp->GetCashVipInfo().SyncCashVipInfoFromDB(data->cash_vip_level, data->score_add, data->score_cost, pPlayer);
		}
	};
	_write_counter ++;
	user_save_data((gplayer*)_parent,new AutoWrite(this),1);
	LuaManager * lua = LuaManager::GetInstance();
	lua->EventPlayerSave(_parent->ID.id);
}

void
gplayer_imp::OnAttacked(world *pPlane,const MSG & msg,attack_msg *attack,damage_entry &dmg,bool is_hit)
{
	//����Ӧ�����ܶ��жϣ���ʱ�ȿ���ս��״̬
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}

	_petman.PlayerBeAttacked(this, msg.source);
	_plantpetman.PlayerBeAttacked(this, msg.source);
}

bool 
gplayer_imp::UseProjectile(int count)
{
	if(_equipment[item::EQUIP_INDEX_PROJECTILE].type == -1 ||
			_equipment[item::EQUIP_INDEX_PROJECTILE].count < (unsigned int)count)
	{
		return false;
	}

	__PRINTF("ʹ���˼�֧%d\n",count);

	if(!_equipment.DecAmount(item::EQUIP_INDEX_PROJECTILE,count))
	{	
		//�������ˢ�£���Ϊ���ܻ���һ�����Ե�Ӱ��
		RefreshEquipment();
	}
	return true;
}

void 
gplayer_imp::OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)
{
	//����ս��״̬����Ϣ
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}

	_runner->be_hurt(attacker, info, damage,invader);
}

void 
gplayer_imp::OnDamage(const XID & attacker,int skill_id, const attacker_info_t&info,int damage,int at_state,char speed,bool orange,unsigned char section)
{
	//����ս��״̬����Ϣ
	ActiveCombatState(true);
	if(_combat_timer < NORMAL_COMBAT_TIME)
	{
		_combat_timer = NORMAL_COMBAT_TIME;
	}

	int index = SelectRandomArmor();
	if(index >= 0 && _equipment.DecDurability(index, DURABILITY_DEC_PER_HIT))
	{
		_runner->equipment_damaged(index,0);
		//��֪��Ʒ������
		RefreshEquipment();
	}
	_runner->be_damaged(attacker,skill_id, info, damage,index,at_state,speed,orange,section);
}

int
gplayer_imp::SelectRandomArmor()
{
	int index = abase::Rand(item::EQUIP_ARMOR_START,item::EQUIP_ARMOR_END-1);
	if (_equipment[index].type == -1) return -1;
	return index;
/*
	unsigned short mask = _equip_info.mask & item::EQUIP_ARMOR_ALL_MASK;
	if(!mask) return -1;
	int index = abase::Rand(item::EQUIP_ARMOR_START,item::EQUIP_ARMOR_END-1);
	unsigned short pos = 1 << index;
	if(mask & pos) return index; //�ҵ�
	if(mask & (pos -1))
	{
		//�ڵ�λѡ��
		do
		{
			pos >>= 1;
			index --;
			if(mask & pos) return index;
			ASSERT(index > item::EQUIP_ARMOR_START);
		}while(index > item::EQUIP_ARMOR_START);
	}
	else
	{
		//�ڸߴ�ѡ��
		do
		{
			pos <<= 1;
			index ++;
			if(mask & pos) return index;
			ASSERT(index < item::EQUIP_ARMOR_END -1);
		}while(index < item::EQUIP_ARMOR_END -1);
	}
	return -1;
	*/
}

	

void 
gplayer_imp::AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg, float & damage_adjust)
{
	if(IS_HUMANSIDE(attack->ainfo.attacker))
	{
		int pp = (((attack->attacker_layer) & 0x03) << 2) | _layer_ctrl.GetLayer();
		ASSERT((_layer_ctrl.GetLayer() & ~0x03) == 0);
		switch(pp)
		{
			case ((LAYER_GROUND << 2) | LAYER_GROUND):
			case ((LAYER_AIR << 2) | LAYER_AIR):
			case ((LAYER_WATER << 2) | LAYER_WATER):
			case ((LAYER_GROUND << 2) | LAYER_AIR):
			case ((LAYER_GROUND << 2) | LAYER_WATER):
			case ((LAYER_WATER << 2) | LAYER_GROUND):
			case ((LAYER_WATER << 2) | LAYER_AIR):
				damage_adjust *= PVP_DAMAGE_REDUCE;
				break;

			case ((LAYER_AIR << 2) | LAYER_GROUND):
			case ((LAYER_AIR << 2) | LAYER_WATER):
				damage_adjust *= PVP_DAMAGE_REDUCE * 0.5f;
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
	}
	else
	{
		//���ֹ���
		damage_adjust *= (1.f - player_template::GetResilienceImpair(_resilience,attack->ainfo.level));
	}
	
	if(attack->skill_id && attack->skill_enhance2)
		damage_adjust *= (0.01f * (100 + attack->skill_enhance2));
}



void 
gplayer_imp::SendServiceRequest(int service_type,const void * buf, unsigned int length)
{
	LuaManager * lua = LuaManager::GetInstance();
	
	int npcid = _plane->GetNpcType(((gplayer_controller*)_commander)->GetCurTarget().id);
	if(LuaManager::GetInstance()->EventOnSendServiceRequest(_parent->ID.id, npcid, service_type))
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
		return;
	}

	service_executor * executor;

	if ( lua->IsTrue( world_manager::GetWorldTag() , lua->GetConfig()->DISTANCE_SERVICE_TAG ) &&  lua->IsTrue( service_type , lua->GetConfig()->DISTANCE_SERVICE_TYPE ) )
	{
		if(service_type != 1)
		{
			executor = service_manager::GetExecutor(service_type);
			if(executor)		
			executor->ServeRequest(this,_parent->ID, buf,length);
			return; 
		}
		if(service_type == 1 && CheckNPCSopDistance( lua->GetConfig()->IdDistanceShop,buf,length))
		{
			executor = service_manager::GetExecutor(service_type);
			if(executor)
			executor->ServeRequest(this,_parent->ID, buf,length);
			return;
		}
	}
	
	if(CanUseService() && (executor = service_manager::GetExecutor(service_type)) )
	{
		if(!executor->ServeRequest(this,_provider.id, buf,length))
		{
			_runner->error_message(S2C::ERR_SERVICE_ERR_REQUEST);
		}
	}
	else
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
	}
}

void 
gplayer_imp::QueryServiceContent(int service_type)
{
	if(CanUseService())
	{	
		struct 
		{
			int cs_index;
			int sid;
		} data;
		gplayer * pPlayer = (gplayer * )_parent;
		data.cs_index = pPlayer->cs_index;
		data.sid =  pPlayer->cs_sid;
		SendTo<0>(GM_MSG_SERVICE_QUIERY_CONTENT,_provider.id,service_type,&data,sizeof(data));
	}
	else
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
	}
}

void 
gplayer_imp::RepairAllEquipment()
{
	unsigned int cost = 0;
	int count = 0;
	int g17_item_count = 0;
	int g17_item_cost = 0;

	enum 
	{
		REPAIR_FEE_ITEM_G17 = 60017,
	};
	
	cost += _equipment.GetRepairCost(count, g17_item_count, g17_item_cost);

	if(count > 0)
	{
		if(cost == 0) cost = 1;

		if(g17_item_count > 0) // Caso tenha algum item g17 ele vai remover os itens e moedas
		{
			if(cost > GetAllMoney()) 
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
				return;
			}

			if (!InvPlayerSpendItem(0, REPAIR_FEE_ITEM_G17, g17_item_cost))
			{
				_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
				return;
			}
						
			_equipment.RepairAll();
			SpendAllMoney(cost);
			SelfPlayerMoney();
			_runner->repair_all(cost);
			RefreshEquipment();
				
		} else // Caso não tenha nenhum item g17 ele vai ignorar e remover apenas moedas
		{
			if(cost < GetAllMoney())
			{
				_equipment.RepairAll();
				SpendAllMoney(cost);
				SelfPlayerMoney();
				_runner->repair_all(cost);
				RefreshEquipment();
			}
			else
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
			}
		}
	}
}

void
gplayer_imp::RemoteAllRepair()
{
	if(!CheckVipService(CVS_REPAIR))
	{
		_runner->error_message(S2C::ERR_CASH_VIP_LIMIT);
		return;
	}
	
	unsigned int cost = 0;
	int count ;
	int g17_item_count ;
	int g17_item_cost ;

	enum 
	{
		REPAIR_FEE_ITEM_G17 = 60017,
	};

	cost += _equipment.GetRepairCost(count, g17_item_count, g17_item_cost);
	if(count > 0)
	{
		if(cost == 0) cost = 1;

		float cost_adjust_ratio = player_template::GetRemoteAllRepairCostRatio(GetCashVipLevel());
		float cost_adjust = cost * cost_adjust_ratio;
		if(cost_adjust > 2e9)
		{
			return;
		}
		cost = (int)cost_adjust;

		if(g17_item_count > 0) // Caso tenha algum item g17 ele vai remover os itens e moedas
		{
			if(cost > GetAllMoney()) 
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
				return;
			}

			if (!InvPlayerSpendItem(0, REPAIR_FEE_ITEM_G17, g17_item_cost))
			{
				_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
				return;
			}
						
			_equipment.RepairAll();
			SpendAllMoney(cost);
			SelfPlayerMoney();
			_runner->repair_all(cost);
			RefreshEquipment();
			
		} else // Caso não tenha nenhum item g17 ele vai ignorar e remover apenas moedas
		{
			if(cost < GetAllMoney())
			{
				_equipment.RepairAll();
				SpendAllMoney(cost);
				SelfPlayerMoney();
				_runner->repair_all(cost);
				RefreshEquipment();
			}
			else
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
			}
		}
	}
}

int 
gplayer_imp::Repair(item & it, int where,int index)
{
	if(it.proc_type & item::ITEM_PROC_TYPE_UNREPAIRABLE) return -1;
	int durability;
	int max_durability;
	it.GetDurability(durability,max_durability);
	int offset = max_durability - durability;
	if( offset > 0 && max_durability > 0)
	{
		int repair_fee_item = 0;
		int cost_item = 0;
		
		int repair_fee = world_manager::GetDataMan().get_item_repair_fee(it.type);
		unsigned int cost = (unsigned int)player_template::GetRepairCost(offset,max_durability,repair_fee);

		if(it.body->GetItemType() == item_body::ITEM_TYPE_NEW_ARMOR)
		{
			repair_fee_item += world_manager::GetDataMan().get_item_repair_g17_num(it.type);
			cost_item += (unsigned int)player_template::GetRepairCost(offset,max_durability,repair_fee_item);
		}

		enum 
		{
			REPAIR_FEE_ITEM_G17 = 60017,
		};

		if(cost == 0) cost = 1;

		if(cost_item > 0) // Caso tenha algum item g17 ele vai remover os itens e moedas
		{
			if (cost > GetAllMoney())
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
				return 1;
				
			}

			if (!InvPlayerSpendItem(0, REPAIR_FEE_ITEM_G17, cost_item))
			{
				_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
				return 1;

			}
			
			it.Repair();
			SpendAllMoney(cost);
			SelfPlayerMoney();
			_runner->repair(where, index, cost);
			RefreshEquipment();
			return 0;
			
		} else // Caso não tenha nenhum item g17 ele vai ignorar e remover apenas moedas
		{
			if (cost < GetAllMoney())
			{
				it.Repair();
				SpendAllMoney(cost);
				SelfPlayerMoney();
				_runner->repair(where, index, cost);
				RefreshEquipment();
				return 0;
			}
			else
			{
				_runner->error_message(S2C::ERR_OUT_OF_FUND);
				return 1;
			}
		}
	}
	return -1;
}

void 
gplayer_imp::RepairEquipment(int where, unsigned int index)
{
	switch(where)
	{
		case IL_INVENTORY:
			if(index < _inventory.Size())
			{
				item & it = _inventory[index];
				if(it.type != -1) Repair(it,where,index);
			}
			break;
		case IL_EQUIPMENT:
			if(index < _equipment.Size())
			{
				item & it = _equipment[index];
				if(it.type != -1) Repair(it,where,index);
			}
			break;
		case IL_TASK_INVENTORY:
			break;
	}
	return ;
}

void 
gplayer_imp::PlayerLogout(int type)
{
	if((_player_state != PLAYER_STATE_NORMAL
				&& _player_state != PLAYER_SIT_DOWN
				&& _player_state != PLAYER_STATE_BIND) 
				|| IsCombatState() || type < 0)
	{
		_runner->error_message(S2C::ERR_CANNOT_LOGOUT);
		return;
	}

	GLog::log(GLOG_INFO,"�û�%dִ�еǳ��߼�%d",_parent->ID.id, type);
	ASSERT(_offline_type == 0);
	//���������ҵĵǳ�����  ����LostConnection ����
	_team.PlayerLogout();
	Logout(type);
}

void 
gplayer_imp::Logout(int type)
{
	class UserInfoWriteBack : public GDB::Result, public abase::ASmallObject
	{
		gplayer * _player;
		int _userid;
		int _type;
	public:
		UserInfoWriteBack(gplayer * pPlayer,int type):_player(pPlayer),_userid(pPlayer->ID.id),_type(type)
		{}

		virtual void OnTimeOut()
		{
			//Ŀǰ��û�����·��ʹ�������
			GLog::log(GLOG_ERR,"����ʱ����%d���ݳ�ʱ",_userid);
			OnPutRole(1);
		}

		virtual void OnFailed()
		{	
			//�����ܵ�������� 
			GLog::log(GLOG_ERR,"����ʱ����%d����ʧ��",_userid);
			OnPutRole(2);
		}
		
		virtual void OnPutRole(int retcode, GDB::PutRoleResData *data = NULL)
		{
			//ASSERT(retcode == 0);
			//д����̳ɹ�
			_player->Lock();
			if(_player->ID.id !=  _userid || _player->login_state != gplayer::WAITING_LOGOUT) 
			{
				//���Դ���ֱ�ӷ���
				_player->Unlock();
				GLog::log(GLOG_INFO,"д���û�%d����״̬��һ�´��� id2:%d state:%d",_userid,_player->ID.id,_player->login_state);
				delete this;
				return ;
			}
			ASSERT(_player->imp);
			GLog::log(GLOG_INFO,"д���û�%d�������(%d)�����͵ǳ�����%d",_userid,retcode,_type);
			//��player����ɾ��
			((gplayer_imp*)(_player->imp))->SendLogoutRequest(_type, retcode);
			_player->imp->_commander->Release(); 
			_player->Unlock();
			delete this;
		}

	};
	ASSERT(_player_state == PLAYER_STATE_NORMAL || _player_state == PLAYER_DISCONNECT || _player_state == PLAYER_SIT_DOWN || _player_state == PLAYER_STATE_BIND);

	GLog::formatlog("playerlogout:roleid=%d:userid=%d:level=%d:occupation=%d:worldtag=%d:x=%f:y=%f:z=%f:playtime=%d",
			_parent->ID.id,_db_user_id,_basic.level,GetPlayerClass(),world_manager::GetWorldTag(),_parent->pos.x,_parent->pos.y,_parent->pos.z,(int)(g_timer.get_systime()-_login_timestamp));

	PlayerLeaveWorld();
	_runner->leave_world();
	
	//��Ҫ���Ƕ����߼�
	//�û���������߼�������ʼ����
	_player_state = PLAYER_WAIT_LOGOUT;
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->login_state = gplayer::WAITING_LOGOUT;

	ClearSession();
	ClearAction();

	//�Ƚ��û���������ɾ�� ��commander->Release�л���û��ӹ��������Ƴ�
	slice * pPiece = pPlayer->pPiece;
	if(pPiece) _plane->RemovePlayer(pPlayer);
	pPlayer->pPiece = NULL;

	if(type >= 0)
	{
		//���ǳ�ʱ���̵㲻�ٱ�������ʱ��ɾ���л�������ʧ��װ��
		int tag;
		A3DVECTOR pos;
		world_manager::GetInstance()->GetLogoutPos(this,tag,pos);
		if(tag != world_manager::GetWorldTag())
		{
			_inventory.ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_equipment.ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_task_inventory.ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(1).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(2).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(3).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(4).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(5).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(6).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_trashbox.GetBackpack(7).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			_user_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_NO_SAVE);
			//ɾ���л������������filter
			_filters.ClearSpecFilter(filter::FILTER_MASK_NOSAVE);
		}
		//д�����	��û�п��Ƕ�д��д��ʧ�ܵ����
		//��ʱǿ�Ʊ���ֿ�
		_tb_change_counter = 1;
		_user_tb_change_counter = 1;
		user_save_data((gplayer*)_parent,new UserInfoWriteBack(pPlayer,type),2);
		LuaManager * lua = LuaManager::GetInstance();
		lua->EventPlayerSave(_parent->ID.id);
	}
	else
	{
		//���������������д�̲���
		SendLogoutRequest(-1);
		_commander->Release(); 
	}
}

void gplayer_imp::PlayerForceOffline()
{
	//���ʱ���������ڵȴ�д��������Ȼ���ܶ��ߣ���ʱ����Ȼ�����
	//��Ҫ���Ǳ�Ҫ���ͷŲ���
	if(_player_state == PLAYER_DISCONNECT) return;
	if(_parent->pPiece) _runner->leave_world();

	//���ﻹ��Ҫ��ʲô?
	
	//���û���������ɾ��
	_commander->Release(); 
}

void 
gplayer_imp::ServerShutDown()
{
	//�رյ�ԭ���Ǻ���ս��״̬
	if(_parent->b_disconnect) return;
	ActiveCombatState(false);
	LostConnection(PLAYER_OFF_LPG_DISCONNECT);
}

void 
gplayer_imp::SendLogoutRequest(int type, int retcode)
{
	gplayer * pPlayer  =(gplayer*)_parent;
	bool bRst;

	//yyb send update info
	if(_level_up) GMSV::SendPlayerInfoUpdate(pPlayer->ID.id, _basic.level);
	GMSV::SendSNSRoleBrief(pPlayer->ID.id, object_interface(this));

	switch(_offline_type)
	{
		case PLAYER_OFF_LOGOUT:
		bRst = GMSV::SendLogout(pPlayer->cs_index, pPlayer->ID.id, pPlayer->cs_sid,type>=0?type:GMSV::PLAYER_LOGOUT_FULL);
		//ASSERT(bRst);
		//���ٱ�������ˣ�����linkserver��������ʱ���ܻ��жϣ������������ķ��������ĸ�
		break;

		case PLAYER_OFF_OFFLINE:
		bRst = GMSV::SendOfflineRe(pPlayer->cs_index, pPlayer->ID.id, pPlayer->cs_sid,0);
		//ASSERT(bRst);
		//���ٱ�������ˣ�����linkserver��������ʱ���ܻ��жϣ������������ķ��������ĸ�
		break;
		
		case PLAYER_OFF_KICKOUT:
		bRst = GMSV::SendKickoutRe(pPlayer->ID.id, pPlayer->cs_sid,0);
		//ASSERT(bRst);
		//���ٱ�������ˣ�����linkserver��������ʱ���ܻ��жϣ������������ķ��������ĸ�
		break;

		case PLAYER_OFF_LPG_DISCONNECT:
		bRst = GMSV::SendDisconnect(pPlayer->cs_index,pPlayer->ID.id,pPlayer->cs_sid,0);
		break;

		case PLAYER_OFF_CHANGEDS:
		bRst = GMSV::SendPlayerChangeDSRe(retcode, pPlayer->ID.id, type);
		break;
	};
	
}

void
gplayer_imp::LostConnection(int offline_type)
{
	if(_parent->b_disconnect) return;
	_parent->b_disconnect = true;
	_offline_type = offline_type;
	//���ߣ����ݸ���״̬���в���
	switch(_player_state)
	{
		case PLAYER_STATE_MARKET:
		case PLAYER_STATE_COSMETIC:
			if(_player_state == PLAYER_STATE_MARKET) CancelPersonalMarket();
			if(_player_state == PLAYER_STATE_COSMETIC) LeaveCosmeticMode(0);
		case PLAYER_STATE_NORMAL:
		case PLAYER_SIT_DOWN:
		case PLAYER_STATE_BIND:	
			{
				if(!IsCombatState())
				{
					//��ս��״̬���ȴ��������˳������������˳���
					_player_state = PLAYER_DISCONNECT;
					_disconnect_timeout = LOGOUT_TIME_IN_NORMAL;
					//Logout(GMSV::PLAYER_LOGOUT_FULL);
				}
				else
				{
					//ս��״̬
					//��������߼�״̬
					_player_state = PLAYER_DISCONNECT;
					_disconnect_timeout = LOGOUT_TIME_IN_COMBAT;
				}
			}
			break;

		case PLAYER_STATE_TRAVEL:
		{
			//��������߼�
			_player_state = PLAYER_DISCONNECT;
			_disconnect_timeout = LOGOUT_TIME_IN_TRAVEL;
			_logout_pos_flag = 1;
			_logout_pos = _provider.pos;
			
		};
		break;


		case PLAYER_WAITING_TRADE:
			{
				//ֱ�Ӻ��Խ���
				//����logout ״̬
				GMSV::ReplyTradeRequest(_trade_obj->GetTradeID(),_parent->ID.id,
						((gplayer*)_parent)->cs_sid,false);
				FromTradeToNormal();
			}
			break;

		case PLAYER_TRADE:
			{
				//����״̬������ȡ����������
				GMSV::DiscardTrade(_trade_obj->GetTradeID(),_parent->ID.id);
				//������ߵȴ�״̬
				_player_state = PLAYER_WAIT_TRADE_COMPLETE;
				_trade_obj->SetTimeOut(10);
			}
			break;

		case PLAYER_WAITING_FACTION_TRADE:
		case PLAYER_WAIT_FACTION_TRADE_READ:
		case PLAYER_WAIT_TRADE_COMPLETE:
		case PLAYER_WAIT_TRADE_READ:
		case PLAYER_DISCONNECT:			// ����֮
		case PLAYER_WAIT_LOGOUT:	//����֮
			break;
	}
	//���ö�����˳��߼�
	_team.PlayerLostConnection();
}

int 
gplayer_imp::DisconnectMessageHandler(world * pPlane ,const MSG & msg)
{
	if(msg.message == GM_MSG_HEARTBEAT)
	{
		ASSERT(_disconnect_timeout > 0 && _disconnect_timeout < 1000);
		if(--_disconnect_timeout <= 0)
		{
			Logout(GMSV::PLAYER_LOGOUT_FULL);
			return 0;
		}
	}
	return DispatchNormalMessage(pPlane,msg);
}

int 
gplayer_imp::TakeOutItem(int item_id)
{
	int rst = _inventory.Find(0,item_id);
	if(rst >= 0)
	{
		item& it = _inventory[rst];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		
		_inventory.DecAmount(rst,1);
		_runner->player_drop_item(IL_INVENTORY,rst,item_id,1,S2C::DROP_TYPE_TAKEOUT);
	}
	return rst;
}

int gplayer_imp::TakeOutItem(int item_id, unsigned int count)   //lgc
{
	int res = -1;
	int pos = _inventory.Find(0,item_id);
	if( pos >= 0 )
	{
		item & it = _inventory[pos];
		if ( it.count >= count )
		{
			UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
			
			_inventory.DecAmount(pos,count);
			_runner->player_drop_item(IL_INVENTORY,pos,item_id,count,S2C::DROP_TYPE_TAKEOUT);
			res = pos;
		}
	}
	return res;
}

void 
gplayer_imp::TakeOutItem(const int * id_list, unsigned int list_count, unsigned int count)
{
	for(unsigned int i=0; i<list_count; i++)
	{
		if(id_list[i] <= 0) continue;
		unsigned int tmp = RemoveItems(id_list[i], count, S2C::DROP_TYPE_TAKEOUT, true);
		ASSERT(tmp <= count);
		if(tmp == count) return;
		count -= tmp;
	}
}

bool 
gplayer_imp::CheckItemExist(int item_id, unsigned int count)
{
	int rst = 0;
	while((rst = _inventory.Find(rst,item_id)) >= 0)
	{
		if(_inventory[rst].count >= count) return true;
		count -= _inventory[rst].count;
		rst ++;
	}
	return false;
}

bool 
gplayer_imp::CheckItemExist(int inv_index, int item_id, unsigned int count)
{
	return IsItemExist(inv_index,item_id,count);
}

bool 
gplayer_imp::CheckItemExist(const int * id_list, unsigned int list_count, unsigned int count)
{
	for(unsigned int i=0; i<list_count; i++)
	{
		if(id_list[i] <= 0) continue;
		int tmp = GetItemCount(id_list[i]);
		if((unsigned int)tmp >= count) return true;
		count -= tmp;
	}
	return false;
}

void 
gplayer_imp::SendMsgToTeam(const MSG & msg, float range, bool exclude_self)
{
	ASSERT(IsInTeam());
	if(exclude_self)
		_team.SendMessage(msg,range);
	else
		_team.SendAllMessage(msg,range);
}

bool
gplayer_imp::Save(archive & ar)
{
	//���ﲻ����player_state,��Ϊ������normal����switch ״̬
	gactive_imp::Save(ar);
	ar << _combat_timer << _reputation << _provider.id << _provider.pos << _inv_level << _stall_trade_id << _stall_info << _last_move_mode << _pvp_cooldown << _security_passwd_checked << _pvp_enable_flag << _force_attack << _refuse_bless << _kill_by_player << _nonpenalty_pvp_state << _resurrect_state << _resurrect_exp_reduce << _resurrect_hp_factor << _resurrect_mp_factor << _resurrect_exp_lost_reduce << _ap_per_hit << _db_save_error << _pvp_combat_timer << _double_exp_timeout << _double_exp_mode << _rest_counter_time << _rest_time_used << _rest_time_capacity << _mafia_rest_time << _mafia_rest_counter_time << _login_timestamp << _played_time << _last_login_timestamp << _create_timestamp << _spec_task_reward << _spec_task_reward2 << _spec_task_reward_param << _spec_task_reward_mask << _db_timestamp << _db_user_id;

	ar << _mall_cash << _mall_cash_used << _mall_cash_offset << _mall_cash_add << _mall_order_id << _mall_order_id_saved << _mall_consumption << _chat_emote <<  _cheat_punish <<_cheat_mode  << _cheat_report << _wallow_level << _auto_hp_value << _auto_hp_percent << _auto_mp_value << _auto_mp_percent << _level_up /*<< _wallow_obj*/ << _profit_time << _profit_level << _profit_timestamp << _active_state_delay << _realm_level << _realm_exp << _leadership << _leadership_occupied << _world_contribution << _world_contribution_cost << _astrolabe_extern_level << _astrolabe_extern_exp;
	
	//��������
	ar << _username_len;
	ar.push_back(_username,_username_len);

	bool nosave;
	_inventory.Save(ar,nosave); ar << nosave;
	_equipment.Save(ar,nosave); ar << nosave;
	_task_inventory.Save(ar,nosave); ar << nosave;
	_trashbox.Save(ar);
	_user_trashbox.Save(ar);
	_team.Save(ar);
	_invade_ctrl.Save(ar);
	_breath.Save(ar);
	//_ph_control.Save(ar);

	//������� ������
	_duel.Save(ar);
	_bind_player.Save(ar);

	//������ȴ����
	_cooldown.Save(ar);

	//����������Ϣ
	PlayerTaskInterface task_if(this);
	unsigned int size = task_if.GetActLstDataSize(); ASSERT(size <= _active_task_list.size());
	ar << size;
	ar.push_back(task_if.GetActiveTaskList(),size);

	size = task_if.GetFnshLstDataSize(); ASSERT(size <= _finished_task_list.size());
	ar << size;
	ar.push_back(task_if.GetFinishedTaskList(),size);

	size = task_if.GetFnshTimeLstDataSize(); ASSERT(size <= _finished_time_task_list.size());
	ar << size;
	ar.push_back(task_if.GetFinishedTimeList(),size);

	size = task_if.GetFnshCntLstDataSize(); ASSERT(size <= _finish_task_count_list.size());
	ar << size;
	ar.push_back(task_if.GetFinishedCntList(),size);

	size = task_if.GetStorageTaskLstDataSize(); ASSERT(size <= _storage_task_list.size());
	ar << size;
	ar.push_back(task_if.GetStorageTaskList(),size);

	//����������
	size = _role_reputation_uchar.size();
	ar << size;
	ar.push_back(_role_reputation_uchar.begin(), _role_reputation_uchar.size()*sizeof(unsigned char));
	
	size = _role_reputation_ushort.size();
	ar << size;
	ar.push_back(_role_reputation_ushort.begin(), _role_reputation_ushort.size()*sizeof(unsigned short));
	
	size = _role_reputation_uint.size();
	ar << size;
	ar.push_back(_role_reputation_uint.begin(), _role_reputation_uint.size()*sizeof(unsigned int));
	
	//����·������
	unsigned int wp_size = 0;
	const void * buf = GetWaypointBuffer(wp_size);
	ar << wp_size;
	if(wp_size) ar.push_back(buf,wp_size);

	//���渱��key 
	size = _cur_tag_counter.size();
	ar << (int)size;
	ar.push_back(_cur_tag_counter.begin(),_cur_tag_counter.size() * sizeof(int)*2);
	ar.push_back(_ins_key_timer.begin(),_cur_tag_counter.size()*sizeof(int)*2);
	ar.push_back(_cur_ins_key_list.begin(),_cur_tag_counter.size() * sizeof(int)*3);
	ar.push_back(_team_ins_key_list.begin(),_cur_tag_counter.size() * sizeof(int)*3);

	//����ٱ������¼
	unsigned int shp_size = _mall_invoice.size();
	ar << shp_size;
	for(unsigned int i = 0;i < shp_size; i ++)
	{
		const netgame::mall_invoice & mi = _mall_invoice[i];
		ar << mi.order_id << mi.item_id << mi.item_count << mi.price<< mi.expire_date
		   << mi.guid1 << mi.guid2 << mi.timestamp;
	}
	

	//�������
	_petman.Save(ar);
	//����touch����
	_touch_order.Save(ar);
	//������Ʒ��
	_player_giftcard.Save(ar);
	//����ƺ�
	_player_title.Save(ar);
	//����ǩ��
	_player_dailysign.Save(ar);
    // ������Ұ�ȫ��״̬
    _player_sanctuary_check.Save(ar);

	//����С����  lgc
	_cur_elf_info >> ar;
	ar << _min_elf_status_value;
	_dividend_mall_info.Save(ar);
	ar.push_back(_equip_refine_level,sizeof(_equip_refine_level));
	ar << _soul_power << _soul_power_en << _min_addon_expire_date;
	_multi_exp_ctrl.Save(ar);
	ar.push_back(&_pet_enhance,sizeof(_pet_enhance));
	ar.push_back(&_faction_contrib,sizeof(_faction_contrib));
	
	
	size = _faction_alliance.size();
	ar << size;
	for(std::unordered_map<int,int>::iterator it=_faction_alliance.begin(); it!=_faction_alliance.end(); ++it)
		ar << it->first;
	
	size = _faction_hostile.size();
	ar << size;
	for(std::unordered_map<int,int>::iterator it=_faction_hostile.begin(); it!=_faction_hostile.end(); ++it)
		ar << it->first;
	
	size = _congregate_req_list.size();
	ar << size;
	for(abase::vector<congregate_req>::iterator it=_congregate_req_list.begin(); it!=_congregate_req_list.end(); ++it)
		ar << it->type << it->sponsor << it->timeout << it->world_tag << it->pos;
	
	_player_force.Save(ar);
	ar.push_back(&_force_ticket_info, sizeof(_force_ticket_info));
	_online_award.Save(ar);
	_player_limit.Save(ar);
	ar << _skill_attack_transmit_target << _country_expire_time << _in_central_server << _src_zoneid << _king_expire_time;
	if(_switch_additional_data)
		_switch_additional_data->SaveInstance(ar);
	else
		ar << (int)-1;
	_meridianman.Save(ar);

	_player_reincarnation.Save(ar);
	_player_fatering.Save(ar);
	//���濨���ռ���¼
	unsigned int gc_size = 0;
	const void * gc_data = _generalcard_collection.data(gc_size);
	ar << gc_size;
	if(gc_size) ar.push_back(gc_data, gc_size);

	_player_clock.Save(ar);
	_player_randmall.Save(ar);
	
	_solochallenge.Save(ar);
	ar << _player_mnfaction_info.unifid;
	ar << _player_visa_info.type << _player_visa_info.stay_timestamp << _player_visa_info.cost << _player_visa_info.count;

	ar << _fix_position_transmit_energy;
	unsigned int fptm_size = FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT;
	ar << fptm_size;
	for(unsigned int i = 0; i < fptm_size; ++i)
	{
		fix_position_transmit_info &info = _fix_position_transmit_infos[i];
		ar <<  info.index << info.world_tag << info.pos.x << info.pos.y << info.pos.z;
		ar.push_back(info.position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
	}
    _cash_vip_info.Save(ar);
	_purchase_limit_info.Save(ar);

    ar << _cash_resurrect_times_in_cooldown;
	ar << _bind_state;

	ar << _shield_energy;
	ar << _shield_energy_gen;
	ar << _shield_energy_max;
	ar << _hwid;
	ar << _increase_pet_time;
	ar << _real_weapon_class;
	ar << _fake_weapon_class;
	
	// nw
	ar << _charge_merc;
	ar << _charge_merc_time;

	ar << _kid_transformation;
	ar << _kid_transformation_time;

	ar << _dungeon_999_timer;

	ar << _check_interface;
	ar << _check_genesis_lvl;
	ar << _check_codex_get_storage;
	//ar << _question_day;
	//ar << _question_started;
	//ar << _question_mode;
	//ar << _question_progress;
	//ar << _question_answer;
	//ar << _question_correct;
	//ar << _question_time;
	
	ar << _color_name;
	ar << _pvp_rank;
	ar << _pvp_rank_exp;
	ar << _player_kill;  
	ar << _monster_kill;  
	ar << _player_death;
	ar << _monster_death;
	ar << _diary_exp;
	ar << _realm_day_verify;
	ar << _verify_itens_valid;
	ar << _has_astrolabe_lock;
	ar << _enabled_fashion_weapon;

	// storage 2

	ar << _double_factor_exp;
	ar << _double_factor_sp;
	ar << _double_factor_realm;

	// storage data 1
	
	ar.push_back(& _autoswap         , sizeof( _autoswap         ));
	ar.push_back(& _skillsender      , sizeof( _skillsender      ));
	ar.push_back(& _glyph            , sizeof( _glyph            ));
	ar.push_back(& _carrier          , sizeof( _carrier          ));
	ar.push_back(& _repository       , sizeof( _repository       ));
	ar.push_back(& _treasure         , sizeof( _treasure         ));
	ar.push_back(& _lottery          , sizeof( _lottery          ));
	ar.push_back(& _treasure_items   , sizeof( _treasure_items   ));
	ar.push_back(& _lib_items        , sizeof( _lib_items        ));
	ar.push_back(& _pet_skill_temp   , sizeof( _pet_skill_temp   ));
	ar.push_back(& _pet_skin         , sizeof( _pet_skin         ));
	ar.push_back(& _passwd_safe      , sizeof( _passwd_safe      ));
	ar.push_back(& _day_world_points , sizeof( _day_world_points ));
	//ar.push_back(& _question_data 	 , sizeof( _question_data 	 ));
	
	// storage data 2
	
	ar.push_back(& _activity  , sizeof( _activity  ));
	ar.push_back(& _celestial , sizeof( _celestial ));

	// nw end
	
	ar.push_back(&_money,sizeof(_money));
	ar.push_back(&_glua,sizeof(_glua));


	ar.push_back(&_codex,sizeof(_codex));
	ar.push_back(&_kid,sizeof(_kid));
	ar.push_back(&_kid_addon,sizeof(_kid_addon));
	return true;
}

bool
gplayer_imp::Load(archive & ar)
{
	gactive_imp::Load(ar);
	ar >> _combat_timer >> _reputation >> _provider.id >> _provider.pos >> _inv_level >> _stall_trade_id >> _stall_info >> _last_move_mode >> _pvp_cooldown >> _security_passwd_checked >> _pvp_enable_flag >> _force_attack >> _refuse_bless >> _kill_by_player >> _nonpenalty_pvp_state >> _resurrect_state >> _resurrect_exp_reduce >> _resurrect_hp_factor >> _resurrect_mp_factor >> _resurrect_exp_lost_reduce >> _ap_per_hit >> _db_save_error >> _pvp_combat_timer >> _double_exp_timeout >> _double_exp_mode >> _rest_counter_time >> _rest_time_used >> _rest_time_capacity >> _mafia_rest_time >> _mafia_rest_counter_time >> _login_timestamp >> _played_time >> _last_login_timestamp >> _create_timestamp >> _spec_task_reward >> _spec_task_reward2 >> _spec_task_reward_param >> _spec_task_reward_mask >> _db_timestamp >> _db_user_id;
	ar >> _mall_cash >> _mall_cash_used>> _mall_cash_offset >> _mall_cash_add >> _mall_order_id >> _mall_order_id_saved >> _mall_consumption >> _chat_emote >> _cheat_punish >> _cheat_mode >> _cheat_report >> _wallow_level >> _auto_hp_value >> _auto_hp_percent >> _auto_mp_value >> _auto_mp_percent >> _level_up /*>> _wallow_obj*/ >> _profit_time >> _profit_level >> _profit_timestamp >> _active_state_delay >> _realm_level >> _realm_exp >> _leadership >> _leadership_occupied >> _world_contribution >> _world_contribution_cost >> _astrolabe_extern_level >> _astrolabe_extern_exp;



	//ȡ������
	ar >> _username_len;
	ar.pop_back(_username,_username_len);

	_inventory.Load(ar); ar >> _inv_switch_save_flag;
	_equipment.Load(ar); ar >> _eqp_switch_save_flag;
	_task_inventory.Load(ar); ar >>_tsk_switch_save_flag;
	_trashbox.Load(ar);
	_user_trashbox.Load(ar);
	_team.Load(ar);
	_invade_ctrl.Load(ar);
	_breath.Load(ar);
	//_ph_control.Load(ar);

	//�ָ�����������
	_duel.Load(ar);
	_bind_player.Load(ar);

	//�ָ���ȴ����
	_cooldown.Load(ar);
	
	//���³�ʼ����������
	PlayerTaskInterface task_if(this);
	unsigned int size;
	ar >> size; ASSERT(size <= _active_task_list.size());
	ar.pop_back(_active_task_list.begin(),size);

	ar >> size; ASSERT(size <= _finished_task_list.size());
	ar.pop_back(_finished_task_list.begin(),size);

	ar >> size; ASSERT(size <= _finished_time_task_list.size());
	ar.pop_back(_finished_time_task_list.begin(),size);

	ar >> size; ASSERT(size <= _finish_task_count_list.size());
	ar.pop_back(_finish_task_count_list.begin(),size);
	
	ar >> size; ASSERT(size <= _storage_task_list.size());
	ar.pop_back(_storage_task_list.begin(),size);
	task_if.InitActiveTaskList();
	
	//���³�ʼ��������
	unsigned int rr_size;
	ar >> rr_size; ASSERT(rr_size <= _role_reputation_uchar.size());
	ar.pop_back(_role_reputation_uchar.begin(),rr_size*sizeof(unsigned char));
	ar >> rr_size; ASSERT(rr_size <= _role_reputation_ushort.size());
	ar.pop_back(_role_reputation_ushort.begin(),rr_size*sizeof(unsigned short));
	ar >> rr_size; ASSERT(rr_size <= _role_reputation_uint.size());
	ar.pop_back(_role_reputation_uint.begin(),rr_size*sizeof(unsigned int));
	
	//���³�ʼ��λ������ 
	unsigned int wp_size;
	ar >> wp_size;
	if(wp_size)
	{
		SetWaypointList(ar.cur_data(),wp_size);
		ar.shift(wp_size);
	}

	//�ָ�����key 
	ar >> size;
	_cur_tag_counter.reserve(size);
	_cur_tag_counter.clear();
	for(unsigned int i =0; i < size; i ++)
	{
		int tag,counter;
		ar >> tag >> counter;
		_cur_tag_counter.push_back(abase::pair<int,int>(tag,counter));
	}
	_ins_key_timer.reserve(size);
	_ins_key_timer.clear();
	for(unsigned int i =0; i < size; i ++)
	{
		int time,state;
		ar >> time >> state;
		_ins_key_timer.push_back(abase::pair<int,int>(time,state));
	} 
	_cur_ins_key_list.reserve(size);
	_cur_ins_key_list.clear();
	for(unsigned int i =0; i < size; i ++)
	{
		int tag,key1,key2;
		ar >> tag >> key1 >> key2;
		_cur_ins_key_list[tag] = abase::pair<int,int>(key1,key2);
	}

	_team_ins_key_list.reserve(size);
	_team_ins_key_list.clear();
	for(unsigned int i =0; i < size; i ++)
	{
		int tag,key1,key2;
		ar >> tag >> key1 >> key2;
		_team_ins_key_list[tag] = abase::pair<int,int>(key1,key2);
	}

	//�ָ��ٱ������¼
	unsigned int shp_size;
	ar >> shp_size;
	_mall_invoice.clear();
	_mall_invoice.reserve(shp_size);
	for(unsigned int i = 0;i < shp_size; i ++)
	{
		int order_id;      
		int item_id;       
		int item_count;    
		int price;         
		int expire_date;   
		int guid1;
		int guid2;
		int ts;
		ar >> order_id >> item_id >> item_count >> price >> expire_date
	           >> guid1 >> guid2 >> ts;
		_mall_invoice.push_back(netgame::mall_invoice(order_id,item_id,item_count,price,expire_date, ts,guid1,guid2));
	}

	//��ȡ����
	_petman.Load(ar);
	//Touch���׶�ȡ
	_touch_order.Load(ar);
	//��Ʒ���ȡ
	_player_giftcard.Load(ar);
	//�ƺŶ�ȡ
	_player_title.Load(ar);
	//ǩ����ȡ
	_player_dailysign.Load(ar);
    // ��ȡ��Ұ�ȫ��״̬
    _player_sanctuary_check.Load(ar);

	//��ȡС���� lgc
	_cur_elf_info << ar;
	ar >> _min_elf_status_value;
	_dividend_mall_info.Load(ar);
	ar.pop_back(_equip_refine_level,sizeof(_equip_refine_level));	
	ar >> _soul_power >> _soul_power_en >> _min_addon_expire_date;
	_multi_exp_ctrl.Load(ar);
	ar.pop_back(&_pet_enhance,sizeof(_pet_enhance));
	ar.pop_back(&_faction_contrib,sizeof(_faction_contrib));

	_faction_alliance.clear();
	unsigned int fsize;
	ar >> fsize;
	while(fsize)
	{
		int factionid;
		ar >> factionid;
		_faction_alliance[factionid] = 1;
		--fsize;
	}
	_faction_hostile.clear();
	ar >> fsize;
	while(fsize)
	{
		int factionid;
		ar >> factionid;
		_faction_hostile[factionid] = 1;
		--fsize;
	}
	_congregate_req_list.clear();
	unsigned int csize;
	ar >> csize;
	while(csize)
	{
		congregate_req req;
		ar >> req.type >> req.sponsor >> req.timeout >> req.world_tag >> req.pos;
		_congregate_req_list.push_back(req);
		--csize;
	}
	_player_force.Load(ar);
	ar.pop_back(&_force_ticket_info,sizeof(_force_ticket_info));	
	_online_award.Load(ar);
	_player_limit.Load(ar);
	ar >> _skill_attack_transmit_target >> _country_expire_time >> _in_central_server >> _src_zoneid >> _king_expire_time;
	int switch_additional_data_guid;
	ar >> switch_additional_data_guid;
	if(switch_additional_data_guid >= 0)
	{
		substance * pSub = substance::LoadInstance(switch_additional_data_guid,ar);
		_switch_additional_data = substance::DynamicCast<switch_additional_data>(pSub);
		if(!_switch_additional_data)
		{
			delete pSub;
		}
	}
	_meridianman.Load(ar);

	_player_reincarnation.Load(ar);
	_player_fatering.Load(ar);
	//��ȡ�����ռ���¼
	unsigned int gc_size = 0;
	ar >> gc_size;
	if(gc_size)
	{
		unsigned char * gc_data = (unsigned char *)abase::fastalloc(gc_size);
		ar.pop_back(gc_data, gc_size);
		_generalcard_collection.init(gc_data, gc_size);
		abase::fastfree(gc_data, gc_size);
	}
	
	if(!_player_clock.Load(ar))  GLog::log(GLOG_ERR,"�û�%d ʱ��������תʱ���ִ���",_parent->ID.id);
	_player_randmall.Load(ar);

	_solochallenge.Load(ar);
	ar >> _player_mnfaction_info.unifid;
	ar >> _player_visa_info.type >> _player_visa_info.stay_timestamp >> _player_visa_info.cost >> _player_visa_info.count;

	ar >> _fix_position_transmit_energy;
	unsigned int fptm_size;
	ar >> fptm_size;
	for(unsigned int i = 0; i < fptm_size; ++i)
	{
		fix_position_transmit_info &info = _fix_position_transmit_infos[i];
		ar >> info.index >> info.world_tag >> info.pos.x >> info.pos.y >> info.pos.z;
		ar.pop_back(info.position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
	}
	_cash_vip_info.Load(ar);
	_purchase_limit_info.Load(ar);

    ar >> _cash_resurrect_times_in_cooldown;

    ar >> _bind_state;

	ar >> _shield_energy;
	ar >> _shield_energy_gen;
	ar >> _shield_energy_max;
	ar >> _hwid;
	ar >> _increase_pet_time;
	ar >> _real_weapon_class;
	ar >> _fake_weapon_class;
	
	// nw
	ar >> _charge_merc;
	ar >> _charge_merc_time;

	ar >> _kid_transformation;
	ar >> _kid_transformation_time;

	ar >> _dungeon_999_timer;

	ar >> _check_interface;
	ar >> _check_genesis_lvl;
	ar >> _check_codex_get_storage;
	//ar >> _question_day;
	//ar >> _question_started;
	//ar >> _question_mode;
	//ar >> _question_progress;
	//ar >> _question_answer;
	//ar >> _question_correct;
	//ar >> _question_time;

	ar >> _color_name;
	ar >> _pvp_rank;
	ar >> _pvp_rank_exp;
	ar >> _player_kill;  
	ar >> _monster_kill;  
	ar >> _player_death;
	ar >> _monster_death;
	ar >> _diary_exp;
	ar >> _realm_day_verify;
	ar >> _verify_itens_valid;
	ar >> _has_astrolabe_lock;
	ar >> _enabled_fashion_weapon;

	// storage 2

	ar >> _double_factor_exp;
	ar >> _double_factor_sp;
	ar >> _double_factor_realm;

	// storage data 1
	
	ar.pop_back(& _autoswap         , sizeof( _autoswap         ));
	ar.pop_back(& _skillsender      , sizeof( _skillsender      ));
	ar.pop_back(& _glyph            , sizeof( _glyph            ));
	ar.pop_back(& _carrier          , sizeof( _carrier          ));
	ar.pop_back(& _repository       , sizeof( _repository       ));
	ar.pop_back(& _treasure         , sizeof( _treasure         ));
	ar.pop_back(& _lottery          , sizeof( _lottery          ));
	ar.pop_back(& _treasure_items   , sizeof( _treasure_items   ));
	ar.pop_back(& _lib_items        , sizeof( _lib_items        ));
	ar.pop_back(& _pet_skill_temp   , sizeof( _pet_skill_temp   ));
	ar.pop_back(& _pet_skin         , sizeof( _pet_skin         ));
	ar.pop_back(& _passwd_safe      , sizeof( _passwd_safe      ));
	ar.pop_back(& _day_world_points , sizeof( _day_world_points ));
	//ar.pop_back(& _question_data 	, sizeof( _question_data 	));
	
	// storage data 2
	
	ar.pop_back(& _activity  , sizeof( _activity  ));
	ar.pop_back(& _celestial , sizeof( _celestial ));

	// nw end
	
	ar.pop_back(&_money,sizeof(_money));
	ar.pop_back(&_glua,sizeof(_glua));
	ar.pop_back(&_codex,sizeof(_codex));
	ar.pop_back(&_kid,sizeof(_kid));
	ar.pop_back(&_kid_addon,sizeof(_kid_addon));
	return true;
}


bool 
gplayer_imp::EmbedChipToEquipment(unsigned int chip,unsigned int equip)
{
	if(chip >= _inventory.Size() || equip >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CANNOT_EMBED);
		return false;
	}
	int type = _inventory[chip].type;
	int proc_type = _inventory[chip].proc_type;
	
	if(_inventory.EmbedItem(chip,equip))
	{
		UpdateMallConsumptionDestroying(type, proc_type, 1);
		((gplayer_dispatcher*)_runner)->embed_item(chip,equip);
		return true;
	}
	else
	{
		_runner->error_message(S2C::ERR_CANNOT_EMBED);
		return false;
	}
}

bool 
gplayer_imp::SharpenEquipment(unsigned int index, addon_data * addon_list, unsigned int count, int sharpener_level, int sharpener_gfx)
{
	if(index >= item::EQUIP_INVENTORY_COUNT) return false;
	if(addon_list == NULL || !count) return false;
	
	item & it = _equipment[index];
	if(it.type <= 0 || it.body == NULL) return false;
	
	if(world_manager::GetDataMan().get_item_level(it.type) < sharpener_level) return false;
	
	it.Deactivate(item::BODY, index, this);
	if(!it.Sharpen(addon_list, count, sharpener_gfx))
	{
		_runner->error_message(S2C::ERR_ITEM_CANNOT_SHARPEN);
		it.Activate(item::BODY, _equipment, index, this);
		return false;	
	}
	it.Activate(item::BODY, _equipment, index, this);
	int addon_expire = it.GetAddonExpireDate();
	if(addon_expire > 0) UpdateMinAddonExpireDate(addon_expire);
	PlayerGetItemInfo(IL_EQUIPMENT, index);//�ͻ���װ������
	//װ�����Ա��ˣ�ˢ��һ��װ��
	RefreshEquipment();
	CalcEquipmentInfo();
	
	X_EQUIP id;
	*(int*)&id.type = it.type; id.mask = it.GetIdModify();
	_runner->equipment_info_changed(1ULL<<index, 0, &id,sizeof(X_EQUIP)); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	IncEquipChangeFlag();
	return true;
}

int 
gplayer_imp::SpendFlyTime(int tick)
{
	item & it = _equipment[item::EQUIP_INDEX_FLYSWORD];
	if(it.type == -1 || it.body == NULL) return -1;
	return it.body->SpendFlyTime(tick);
}

int 
gplayer_imp::GetFlyTime()
{
	item & it = _equipment[item::EQUIP_INDEX_FLYSWORD];
	if(it.type == -1 || it.body == NULL) return -1;
	return it.body->GetFlyTime();
}

void 
gplayer_imp::KnockBack(const XID & target, const A3DVECTOR & source, float distance,int time,int stun_time)
{
	LeaveAbnormalState();
	//����һ������session
	session_knockback * pses = new session_knockback(this);
	pses->SetInfo(target, source, distance, time, stun_time);
	if(AddSession(pses)) StartSession();
}

void 
gplayer_imp::PullOver(const XID & target, const A3DVECTOR & source,float distance, int time)
{
	LeaveAbnormalState();
	session_pullover * pses = new session_pullover(this);
	pses->SetInfo(target, distance, time);
	if(AddSession(pses)) StartSession();
}

void 
gplayer_imp::Teleport(const A3DVECTOR & pos, int time, char mode)
{
	LeaveAbnormalState();
	session_teleport * pses = new session_teleport(this);
	pses->SetInfo(pos, time, mode);
	if(AddSession(pses)) StartSession();
}

void 
gplayer_imp::Teleport2(const A3DVECTOR & pos, int time, char mode)
{
	LeaveAbnormalState();
	session_teleport2 * pses = new session_teleport2(this);
	pses->SetInfo(pos, time, mode);
	if(AddSession(pses)) StartSession();
}

void
gplayer_imp::KnockUp(float distance,int time)
{
	LeaveAbnormalState();
	session_knockup * pses = new session_knockup(this);
	pses->SetInfo(distance, time);
	if(AddSession(pses)) StartSession();
}

bool 
gplayer_imp::DrainMana(int mana)
{
	if(_basic.mp >= mana)
	{
		_basic.mp -= mana;
		SetRefreshState();
		return true;
	}
	else
	{
		if(_basic.mp) SetRefreshState();
		_basic.mp = 0;
	}
	return false;
}

bool 
gplayer_imp::RechargeEquippedFlySword(unsigned int index, unsigned int count)
{
	if(count == 0 || index >= _inventory.Size()) return false;
	item & element = _inventory[index];
	if(element.type == -1 || element.count < count) return false;
	item & it = _equipment[item::EQUIP_INDEX_FLYSWORD];
	if(it.type == -1 || !it.body) return false;
	int element_id = element.type;
	DATA_TYPE dt;
	ELEMENT_ESSENCE * ess = (ELEMENT_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)element_id, ID_SPACE_ESSENCE,dt);
	if( !ess || dt != DT_ELEMENT_ESSENCE) return false;
	int cur_time;
	int rst = it.body->Recharge(ess->level,count,cur_time);
	if(rst > 0)
	{
		UpdateMallConsumptionDestroying(element.type, element.proc_type, rst);
		
		_inventory.DecAmount(index,rst);
		_runner->player_drop_item(IL_INVENTORY,index,element_id,rst,S2C::DROP_TYPE_RECHARGE);
		_runner->flysword_time_capacity(IL_EQUIPMENT,item::EQUIP_INDEX_FLYSWORD,cur_time);
	}
	return true;
}

bool 
gplayer_imp::RechargeFlySword(unsigned int element_index,unsigned int count,unsigned int fw_index,int fw_id)
{
	if(count == 0 || element_index >= _inventory.Size()
			|| fw_id <= 0
			|| fw_index >= _inventory.Size()) return false;
	item & element = _inventory[element_index];
	item & fw = _inventory[fw_index];
	if(element.type == -1 || element.count < count) return false;
	if(fw.type == -1 || !fw.body || fw.body->GetItemType() != item_body::ITEM_TYPE_FLYSWORD) return false;
	int element_id = element.type;

	DATA_TYPE dt;
	ELEMENT_ESSENCE * ess = (ELEMENT_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)element_id, ID_SPACE_ESSENCE,dt);
	if( !ess || dt != DT_ELEMENT_ESSENCE) return false;
	
	int old_stamina = fw.GetStamina();//����С��������
	int cur_time;
	int rst = fw.body->Recharge(ess->level,count,cur_time);
	if(rst > 0)
	{
		UpdateMallConsumptionDestroying(element.type, element.proc_type, rst);

		_inventory.DecAmount(element_index,rst);
		_runner->player_drop_item(IL_INVENTORY,element_index,element_id,rst,S2C::DROP_TYPE_RECHARGE);
		if(fw.body->GetItemType() == item_body::ITEM_TYPE_ELF)	//С���� , lgc
		{
			PlayerGetItemInfo(IL_INVENTORY, fw_index);
			_runner->elf_cmd_result(S2C::ELF_RECHARGE, element_id, rst, cur_time - old_stamina);
		}
		else	//�ɽ�
			_runner->flysword_time_capacity(IL_INVENTORY,fw_index,cur_time);
	}
	return true;
}

bool 
gplayer_imp::PlayerUseItem(int where, unsigned int inv_index, int item_type,unsigned int count)
{
	DATA_TYPE dt;
	EASY_PRODUCE_ITEM_ESSENCE *ess = (EASY_PRODUCE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_type,ID_SPACE_ESSENCE,dt);
	if(ess && dt == DT_EASY_PRODUCE_ITEM_ESSENCE)
	{
		return true;
	}

	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
		inv[inv_index].type != item_type ) return false;
	
	int it_cmd = LuaManager::GetInstance()->EventOnPlayerUseItem(_parent->ID.id ,item_type);
	if (it_cmd == 1) return true;
	if (it_cmd == 2) 
	{
		item& it = _inventory[inv_index];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_inventory.DecAmount(inv_index, 1);
		_runner->player_drop_item(IL_INVENTORY,inv_index,item_type,1,S2C::DROP_TYPE_USE);
		return true;
	}	
	
	if(!inv[inv_index].CanUse(inv.GetLocation())) return false;
	int rst = inv[inv_index].GetUseDuration();
	if( rst < 0)
	{
		return UseItem(inv,inv_index,where,item_type, count);
	}
	else
	{
		AddSession(new session_use_item(this,where,inv_index,item_type,count,rst));
		StartSession();
		return true;
	}
	return false;
}

bool 
gplayer_imp::PlayerUseItem2(int where, unsigned int inv_index, int item_type,unsigned int count)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
		inv[inv_index].type != item_type ) return false;	
		
	if(!inv[inv_index].CanUse(inv.GetLocation())) return false;
	int rst = inv[inv_index].GetUseDuration();
	if( rst < 0)
	{
		return UseItem(inv,inv_index,where,item_type, count);
	}
	else
	{
		AddSession(new session_use_item(this,where,inv_index,item_type,count,rst));
		StartSession();
		return true;
	}
	return false;
}

bool 
gplayer_imp::PlayerSitDownUseItem(int where, unsigned int inv_index, int item_type,unsigned int count)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].SitDownCanUse(inv.GetLocation())) return false;
	int rst = inv[inv_index].GetUseDuration();
	if(rst >= 0) return false;
	return UseItem(inv,inv_index,where,item_type, count);
}

bool 
gplayer_imp::PlayerUseItemWithTarget(int where,unsigned int inv_index,int item_type,char force_attack)
{
	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	XID cur_target = ((gplayer_controller*)_commander)->GetCurTarget();
	if(!cur_target.IsActive()) return false;

	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].CanUseWithTarget(inv.GetLocation())) return false;
	int rst = inv[inv_index].GetUseDuration();
	if(rst < 0)
	{
		return UseItemWithTarget(inv,inv_index,where, item_type, cur_target,force_attack);
	}
	else
	{
		session_use_item_with_target * pSession = new session_use_item_with_target(this,where,inv_index,item_type,1,rst);
		pSession->SetTarget(cur_target,force_attack);
		AddSession(pSession);
		StartSession();
		return true;
	}
	return false;
}

void 
gplayer_imp::MakeDartAttack(int damage, float throw_range,attack_msg &attack,char force_attack)
{
	memset(&attack,0,sizeof(attack));
	attack.ainfo.level = _basic.level;
	attack.ainfo.team_id = -1; 
	attack.short_range = 0.f;
	attack.attack_range = throw_range;
	attack.attack_attr = attack_msg::PHYSIC_ATTACK; 
	attack.physic_damage  = (int)(damage*(0.005f*(_cur_prop.strength + _cur_prop.agility)));
	attack.attack_rate = _cur_prop.attack;
	attack.attacker_faction = GetFaction();
	attack.target_faction = GetEnemyFaction();
	attack.force_attack  = force_attack;
	attack.attacker_layer = _layer_ctrl.GetLayer();
}

int 
gplayer_imp::ThrowDart(const XID & target,int damage, float throw_range,char force_attack)
{
	if(!target.IsActive()) return -1;
	//�������Ƿ���� 
	enum
	{
		ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE
	};
	world::object_info info;
	if(!_plane->QueryObject(target,info) ||
			(info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE )
	{
		_runner->error_message(S2C::ERR_INVALID_TARGET);
		return -1;
	}
	
	float range = throw_range + info.body_size + _parent->body_size;
	if(info.pos.squared_distance(_parent->pos) > range * range)
	{
		//�޷����������ˣ������Զ
		_runner->error_message(S2C::ERR_OUT_OF_RANGE);
		return -3;
	}
	
	ActiveCombatState(true);
	_combat_timer = MAX_COMBAT_TIME;

	attack_msg attack;
	MakeDartAttack(damage, range,attack,force_attack);
	gplayer *pPlayer = GetParent();
	attack.ainfo.sid = pPlayer->cs_sid;
	
	//�������ӣ�����ֵ
	_team.GetTeamID(attack.ainfo.team_id, attack.ainfo.team_seq);

	//���ﲻ����Translate Msg ��Ϊ������ʵ��
	SendTo<0>(GM_MSG_ATTACK,target,0,&attack,sizeof(attack));

	//������ʱ�Ѿ���������ٴ����ã���Ҫ���±�д

	//����Ӧ�÷���ʹ�ð�������Ϣ
	return 0;
}

bool
gplayer_imp::ReturnToTown()
{
	A3DVECTOR pos;
	int world_tag;
	if(!world_manager::GetInstance()->GetTownPosition(this,_parent->pos,pos,world_tag)) return false;
	if(!LongJump(pos,world_tag))
		return false;
	return true;
}

bool 
gplayer_imp::PlayerSitDown()
{
	if(!CanSitDown()) return false;

	//���²���Ӧ�ñ���Ϊsession����������� 
	AddSession(new session_sit_down(this));
	StartSession();
	return true;
}

bool
gplayer_imp::PlayerStandUp()
{
	//������������
	//���²������ڲ�����
	if(_player_state != PLAYER_SIT_DOWN) return false;
	StandUp();
	return true;
}

bool 
gplayer_imp::CanSitDown()
{
	//Ҫ�ж��Ƿ��ܹ�����
	//if(IsCombatState()) return false;
	if(!_layer_ctrl.CanSitDown()) return false;
	if(_player_state != PLAYER_STATE_NORMAL) return false;
	if(((gplayer*)_parent)->IsInvisible())	return false;
	return true;
}

bool
gplayer_imp::SitDown()
{
	//���²������ڲ�����
	if(!CanSitDown()) return false;

	//Ҫ���뵽����״̬
	EnterStayInState();
	
	//�������µ�filter
	sit_down_filter * pFilter = new sit_down_filter(this);
	_filters.AddFilter(pFilter);

	//Ҫ��պ��������session
	ClearNextSession();
	return true;
}

void
gplayer_imp::StandUp()
{
	//���²������ڲ�����
	ASSERT(_player_state == PLAYER_SIT_DOWN);
	LeaveStayInState();
}

void gplayer_imp::SetIdleMode(bool sleep, bool stun)
{
	gactive_imp::SetIdleMode(sleep,stun);
	if(_idle_mode_flag)
	{
		//�����������session
		ClearSession();
		ClearAction();
	}
}

void gplayer_imp::SetSealMode(bool silent,bool root)
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

			//Ӧ��ֹͣ��ǰ�Ĺ�������
			AddSession(new session_empty());
			StartSession();
		}
	}
}

void 
gplayer_imp::SetCombatState()
{
	ActiveCombatState(true);
	SetCombatTimer(MAX_COMBAT_TIME);
}

void 
gplayer_imp::AddAggroToEnemy(const XID & who,int rage)
{
	unsigned int count = _enemy_list.size();
	if(!count || rage <= 0) return;
	XID list[MAX_PLAYER_ENEMY_COUNT];
	ENEMY_LIST::iterator it = _enemy_list.begin(); 
	for(unsigned int i = 0;it != _enemy_list.end();i ++, ++it )
	{
		MAKE_ID(list[i],it->first);
	}

	msg_aggro_info_t info;
	info.source = who;
	info.aggro = rage;
	info.aggro_type = 0;
	info.faction = 0xFFFFFFFF;
	info.level = 0;

	MSG msg;
	BuildMessage(msg,GM_MSG_GEN_AGGRO,XID(-1,-1),who,_parent->pos,0,&info,sizeof(info));

	_plane->SendMessage(list, list + count, msg);
}

void 
gplayer_imp::ClearAggroToEnemy()
{
	gplayer* pPlayer = (gplayer*)_parent;
	if(!pPlayer->IsInvisible()) return;
	
	unsigned int count = _enemy_list.size();
	if(!count) return;
	XID list[MAX_PLAYER_ENEMY_COUNT];
	ENEMY_LIST::iterator it = _enemy_list.begin(); 
	for(unsigned int i = 0;it != _enemy_list.end();i ++, ++it )
	{
		MAKE_ID(list[i],it->first);
	}

	MSG msg;
	BuildMessage(msg,GM_MSG_TRY_CLEAR_AGGRO,XID(-1,-1),_parent->ID,_parent->pos,((gplayer*)_parent)->invisible_degree);

	_plane->SendMessage(list, list + count, msg);
}

bool 
gplayer_imp::CheckInvaderAttack(const XID & who)
{
	ASSERT(_invader_state != INVADER_LVL_0);
	return _invade_ctrl.IsDefender(who.id);
}

void 
gplayer_imp::PlayerRestartSession()
{
	if(_cur_session)
	{
		if(!_cur_session->RestartSession())
		{
			EndCurSession();
			StartSession();
		}
	}
}

namespace
{
	struct drop_entry
	{
		short where;
		short index;
		drop_entry(short w, short i):where(w),index(i)
		{}
	};
}

void 
gplayer_imp::DropItemOnDeath(unsigned int drop_count_inv,unsigned int drop_count_equip,const XID & spec_owner)
{
	ASSERT(drop_count_inv + drop_count_equip);
	
	abase::vector<drop_entry,abase::fast_alloc<> > inv_list;
	abase::vector<drop_entry,abase::fast_alloc<> > equip_list;
	abase::vector<drop_entry,abase::fast_alloc<> > drop_list;
	if(drop_count_inv)
	{
		inv_list.reserve(_inventory.Size() + item::EQUIP_INVENTORY_COUNT);
		for(unsigned int i = 0; i < _inventory.Size(); i ++)
		{
			if(_inventory[i].type == -1) continue;
			inv_list.push_back(drop_entry(IL_INVENTORY, i));
		}
		static int inventory_equipment[] = { item::EQUIP_INDEX_PROJECTILE, item::EQUIP_INDEX_RUNE_SLOT};
		for(unsigned int i = 0; i < sizeof(inventory_equipment) / sizeof(int); i ++)
		{
			int index = inventory_equipment[i];
			if(_equipment[index].type == -1) continue;
			inv_list.push_back(drop_entry(IL_EQUIPMENT,index));
		}
	}

	if(drop_count_equip)
	{
		equip_list.reserve(item::EQUIP_INVENTORY_COUNT);
		static int equipment_drop[] = 
		{
			item::EQUIP_INDEX_WEAPON, item::EQUIP_INDEX_HEAD, item::EQUIP_INDEX_NECK,
			item::EQUIP_INDEX_SHOULDER, item::EQUIP_INDEX_BODY, item::EQUIP_INDEX_WAIST,
			item::EQUIP_INDEX_LEG, item::EQUIP_INDEX_FOOT, item::EQUIP_INDEX_WRIST,
			item::EQUIP_INDEX_FINGER1, item::EQUIP_INDEX_FINGER2, item::EQUIP_INDEX_FLYSWORD,
			item::EQUIP_INDEX_BIBLE, item::EQUIP_INDEX_BUGLE,
		};

		for(unsigned int i = 0; i < sizeof(equipment_drop) / sizeof(int); i ++)
		{
			int index = equipment_drop[i];
			if(_equipment[index].type == -1) continue;
			equip_list.push_back(drop_entry(IL_EQUIPMENT,index));
		}
	}


	drop_list.reserve(drop_count_inv+drop_count_equip);

	for(unsigned int i = 0; i < drop_count_inv && inv_list.size();i ++)
	{
		int index = abase::Rand(0,inv_list.size()-1);
		drop_list.push_back(inv_list[index]);
		inv_list.erase_noorder(inv_list.begin() + index);
	}

	for(unsigned int i = 0; i < drop_count_equip && equip_list.size();i ++)
	{
		int index = abase::Rand(0,equip_list.size()-1);
		drop_list.push_back(equip_list[index]);
		equip_list.erase_noorder(equip_list.begin() + index);
	}

	enum 
	{
		NO_DROP_TYPE = item::ITEM_PROC_TYPE_NODROP | item::ITEM_PROC_TYPE_NOTHROW | item::ITEM_PROC_TYPE_NOTRADE |item::ITEM_PROC_TYPE_BIND
	};
	for(unsigned int i = 0; i < drop_list.size(); i ++)
	{
		drop_entry ent = drop_list[i];
		if(ent.where == IL_INVENTORY)
		{
			if(_inventory[ent.index].proc_type & NO_DROP_TYPE) continue;
			ThrowInvItem(ent.index ,MAX_ITEM_DROP_COUNT,false,S2C::DROP_TYPE_DEATH,spec_owner);
		}
		else
		{
			if(_equipment[ent.index].proc_type & NO_DROP_TYPE) continue;
			ThrowEquipItem(ent.index ,false,S2C::DROP_TYPE_DEATH,MAX_ITEM_DROP_COUNT,spec_owner);
		}
	}
}

void 
gplayer_imp::DamageItemOnDeath(bool restrict_bind, const XID & killer)
{
	abase::vector<int,abase::fast_alloc<> > damage_list;	
	damage_list.reserve(item::EQUIP_INVENTORY_COUNT);
	
	static int equipment_damage[] = 
	{
		item::EQUIP_INDEX_WEAPON, item::EQUIP_INDEX_HEAD, item::EQUIP_INDEX_NECK,
		item::EQUIP_INDEX_SHOULDER, item::EQUIP_INDEX_BODY, item::EQUIP_INDEX_WAIST,
		item::EQUIP_INDEX_LEG, item::EQUIP_INDEX_FOOT, item::EQUIP_INDEX_WRIST,
		item::EQUIP_INDEX_FINGER1, item::EQUIP_INDEX_FINGER2, item::EQUIP_INDEX_FLYSWORD,
		item::EQUIP_INDEX_BIBLE,
	};
	for(unsigned int i = 0; i < sizeof(equipment_damage) / sizeof(int); i ++)
	{
		int index = equipment_damage[i];
		if(_equipment[index].type == -1) continue;
		if(_equipment[index].proc_type & item::ITEM_PROC_TYPE_DAMAGED) continue;
		if(restrict_bind && !(_equipment[index].proc_type & item::ITEM_PROC_TYPE_BIND)) continue;
		damage_list.push_back(index);
	}
	
	if(damage_list.size() == 0) return;
	int damage_index = damage_list[abase::Rand(0,damage_list.size()-1)];	
	item & it = _equipment[damage_index];
	
	//����Ʒ	
	unsigned int damage_drop;
	int damange_drop_count;
	if( (damange_drop_count = world_manager::GetDataMan().get_item_damaged_drop(it.type,damage_drop)) <= 0) return;
	element_data::item_tag_t tag = {element_data::IMT_DROP,0};
	item_data * data = world_manager::GetDataMan().generate_item_for_drop(damage_drop,&tag,sizeof(tag));
	if(data == NULL) return;
	if((unsigned int)damange_drop_count > data->pile_limit) damange_drop_count = data->pile_limit;
	data->count = damange_drop_count;
	//���������λ��
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();
	//��������˱߽磬��ô�Ͱ����Լ���λ������
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}
	DropItemData(_plane,pos,data,(killer.IsPlayerClass()?killer:XID(-1,-1)),0,0,0);

	//�����Ʒ
	it.proc_type |= item::ITEM_PROC_TYPE_DAMAGED;
	PlayerGetItemInfo(IL_EQUIPMENT,damage_index);
	_runner->equipment_damaged(damage_index,1);
	RefreshEquipment();
	PlayerGetProperty();

}

void 
gplayer_imp::ThrowDeadDropItem()
{
	enum 
	{
		NO_DROP_TYPE = item::ITEM_PROC_TYPE_NODROP | item::ITEM_PROC_TYPE_NOTHROW | item::ITEM_PROC_TYPE_NOTRADE |item::ITEM_PROC_TYPE_BIND
	};
	for(unsigned int i = 0; i < _inventory.Size(); i ++)
	{
		if(_inventory[i].type == -1) continue;
		item & it = _inventory[i];
//		if(it.proc_type & NO_DROP_TYPE ) continue;
		if(!(it.proc_type & item::ITEM_PROC_TYPE_DEATH_DROP)) continue;
		ThrowInvItem(i ,it.count,false,S2C::DROP_TYPE_DEATH);
	}

	for(unsigned int i = 0; i < _equipment.Size(); i ++)
	{
		if(_equipment[i].type == -1) continue;
		item & it = _equipment[i];
//		if(it.proc_type & NO_DROP_TYPE ) continue;
		if(!(it.proc_type & item::ITEM_PROC_TYPE_DEATH_DROP)) continue;
		ThrowEquipItem(i ,false,S2C::DROP_TYPE_DEATH);
	}
}

void 
gplayer_imp::PlayerAssistSelect(const XID & cur_target)
{
	if(cur_target != _parent->ID && IsMember(cur_target))
	{
		SendTo<0>(GM_MSG_QUERY_SELECT_TARGET,cur_target,0);
	}
}

void
gplayer_imp::SendDataToSubscibeList()
{
	ASSERT(_subscibe_list.size() || _second_subscibe_list.size());
	packet_wrapper h1(64);
	using namespace S2C;
	//_backup_hp[1]
	CMD::Make<CMD::player_info_00>::From(h1,_parent->ID,_basic.hp,_basic,_cur_prop,IsCombatState()?1:0, GetCurTarget().id, GlyphGetAttr() );
	if(_subscibe_list.size())
		send_ls_msg(_subscibe_list.begin(), _subscibe_list.end(), h1.data(),h1.size());
	if(_second_subscibe_list.size())
		send_ls_msg(_second_subscibe_list.begin(), _second_subscibe_list.end(), h1.data(),h1.size());
}

void
gplayer_imp::SendTeamDataToSubscibeList()
{
	if(!_team_visible_state_flag) return;
	
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
	send_ls_msg(_subscibe_list.begin(), _subscibe_list.end(), h1.data(),h1.size());
	send_ls_msg(((gplayer_dispatcher*)_runner)->_header,h1);
	if(IsInTeam()) _team.SendGroupData<0>(h1.data(),h1.size());
}

void 
gplayer_imp::SendTeamDataToMembers()
{
	ASSERT(IsInTeam());
	if(_visible_team_state.empty()) return;
	
	#pragma pack(push, 1)
	unsigned short vts_id[32];
	struct 
	{
		int time;
		int num;
	}	vts_time[32];
	#pragma pack(pop)
	
	unsigned int iSize = _visible_team_state.size();
	packet_wrapper h1(64+iSize*sizeof(vts_time[0]));
	using namespace S2C;
	//_backup_hp[1]
	//int x_it[64];
	//if(iSize && iSize <= 32)_filters.FilterTimerReport((short*)_visible_team_state.begin(), x_it, iSize);
	
	for (int i = 0; i < iSize && i < 32; i++ )
	{
		vts_id[i] = _visible_team_state[i].id;
		vts_time[i].time = _visible_team_state[i].time;
		vts_time[i].num = i;
	}
	
	CMD::Make<CMD::object_state_notify>::From(h1,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(), (int*)vts_time);
	_team.SendGroupData<0>(h1.data(),h1.size());
}


void 
gplayer_imp::Swap(gplayer_imp * rhs)
{
	((gplayer_controller *)_commander)->LoadFrom((gplayer_controller*)rhs->_commander);
	((gplayer_dispatcher*)_runner)->LoadFrom((gplayer_dispatcher*)rhs->_runner);

#define Set(var,cls) var = cls->var

	Set(_combat_timer,rhs);
	Set(_reputation,rhs);
	Set(_provider,rhs);
	Set(_inv_level ,rhs);
	Set(_stall_trade_id,rhs);
	Set(_stall_info,rhs);
	Set(_last_move_mode,rhs);
	Set(_pvp_cooldown,rhs);
	Set(_security_passwd_checked,rhs);
	Set(_pvp_enable_flag,rhs);
	Set(_force_attack,rhs);
	Set(_refuse_bless,rhs);
	Set(_kill_by_player,rhs);
	Set(_nonpenalty_pvp_state,rhs);
	Set(_resurrect_exp_reduce,rhs);
	Set(_resurrect_hp_factor,rhs);
	Set(_resurrect_mp_factor,rhs);
	Set(_resurrect_exp_lost_reduce,rhs);
	Set(_resurrect_state,rhs);
	Set(_ap_per_hit,rhs);
	Set(_db_save_error,rhs);
	Set(_pvp_combat_timer,rhs);
	Set(_username_len,rhs);
	memcpy(_username,rhs->_username,sizeof(_username));
	Set(_double_exp_timeout,rhs);
	Set(_double_exp_mode,rhs);
	Set(_rest_counter_time,rhs);
	Set(_rest_time_used,rhs);
	Set(_rest_time_capacity,rhs);
	Set(_mafia_rest_time,rhs);
	Set(_mafia_rest_counter_time,rhs);
	Set(_login_timestamp,rhs);
	Set(_played_time,rhs);
	Set(_last_login_timestamp,rhs);
	Set(_create_timestamp,rhs);
	Set(_spec_task_reward,rhs);
	Set(_spec_task_reward2,rhs);
	Set(_spec_task_reward_param,rhs);
	Set(_spec_task_reward_mask,rhs);
	Set(_db_timestamp,rhs);
	Set(_mall_cash          ,rhs);
	Set(_mall_cash_used 	,rhs);
	Set(_mall_cash_offset   ,rhs);
	Set(_mall_cash_add		,rhs);
	Set(_mall_order_id      ,rhs);
	Set(_mall_order_id_saved,rhs);
	Set(_mall_consumption	,rhs);
	Set(_chat_emote,rhs);
	Set(_cheat_punish,rhs);
	Set(_cheat_mode,rhs);
	Set(_cheat_report,rhs);
	Set(_wallow_level,rhs);
	Set(_profit_time,rhs);
	Set(_profit_level,rhs);
	Set(_profit_timestamp,rhs);
	Set(_active_state_delay,rhs);
	Set(_auto_hp_value,rhs);
	Set(_auto_hp_percent,rhs);
	Set(_auto_mp_value,rhs);
	Set(_auto_mp_percent,rhs);
	Set(_inv_switch_save_flag,rhs);
	Set(_eqp_switch_save_flag,rhs);
	Set(_tsk_switch_save_flag,rhs);
	Set(_db_user_id,rhs);
	Set(_level_up,rhs);

	_mall_invoice.swap(rhs->_mall_invoice);
	_inventory.Swap(rhs->_inventory);
	_equipment.Swap(rhs->_equipment);
	_task_inventory.Swap(rhs->_task_inventory);
	_trashbox.Swap(rhs->_trashbox);
	_user_trashbox.Swap(rhs->_user_trashbox);
	_team.Swap(rhs->_team);
	_invade_ctrl.Swap(rhs->_invade_ctrl);
	_breath.Swap(rhs->_breath);
	_ph_control.Swap(rhs->_ph_control);
	_duel.Swap(rhs->_duel);
	_bind_player.Swap(rhs->_bind_player);
//	_wallow_obj.Swap(rhs->_wallow_obj);
	
	_cooldown.Swap(rhs->_cooldown);

	_task_mask = rhs->_task_mask;
	_active_task_list.swap(rhs->_active_task_list);
	_finished_task_list.swap(rhs->_finished_task_list);
	_finished_time_task_list.swap(rhs->_finished_time_task_list);
	_finish_task_count_list.swap(rhs->_finish_task_count_list);
	_storage_task_list.swap(rhs->_storage_task_list);

	_role_reputation_uchar.swap(rhs->_role_reputation_uchar);
	_role_reputation_ushort.swap(rhs->_role_reputation_ushort);
	_role_reputation_uint.swap(rhs->_role_reputation_uint);
	
	_waypoint_list.swap(rhs->_waypoint_list);

	_cur_ins_key_list.swap(rhs->_cur_ins_key_list);
	_team_ins_key_list.swap(rhs->_team_ins_key_list);
	_cur_tag_counter.swap(rhs->_cur_tag_counter);
	_ins_key_timer.swap(rhs->_ins_key_timer);

	_petman.Swap(rhs->_petman);
	_touch_order.Swap(rhs->_touch_order);	
	_player_giftcard.Swap(rhs->_player_giftcard);	
	_player_title.Swap(rhs->_player_title);
	_player_dailysign.Swap(rhs->_player_dailysign);
	_player_fatering.Swap(rhs->_player_fatering);
    _player_sanctuary_check.Swap(rhs->_player_sanctuary_check);

	Set(_cur_elf_info, rhs);	//lgc
	Set(_min_elf_status_value, rhs);
	Set(_dividend_mall_info, rhs);
	memcpy(_equip_refine_level,rhs->_equip_refine_level,sizeof(_equip_refine_level));
	Set(_soul_power,rhs);
	Set(_soul_power_en,rhs);
	Set(_min_addon_expire_date,rhs);
	Set(_multi_exp_ctrl,rhs);
	Set(_pet_enhance,rhs);
	Set(_faction_contrib,rhs);
	Set(_realm_level,rhs);
	Set(_realm_exp,rhs);
	Set(_leadership,rhs);
	Set(_leadership_occupied,rhs);
	Set(_world_contribution,rhs);
	Set(_world_contribution_cost,rhs);
 	Set(_astrolabe_extern_level,rhs);
 	Set(_astrolabe_extern_exp,rhs);

	_faction_alliance.clear();
	for(std::unordered_map<int,int>::iterator it=rhs->_faction_alliance.begin(); it!=rhs->_faction_alliance.end(); ++it)
		_faction_alliance[it->first] = 1;
	_faction_hostile.clear();
	for(std::unordered_map<int,int>::iterator it=rhs->_faction_hostile.begin(); it!=rhs->_faction_hostile.end(); ++it)
		_faction_hostile[it->first] = 1;
	_congregate_req_list.swap(rhs->_congregate_req_list);
	_player_force.Swap(rhs->_player_force);
	Set(_force_ticket_info,rhs);
	_online_award.Swap(rhs->_online_award);
	_player_limit.Swap(rhs->_player_limit);
	Set(_skill_attack_transmit_target,rhs);
	Set(_country_expire_time,rhs);
	Set(_in_central_server,rhs);
	Set(_src_zoneid,rhs);
	Set(_king_expire_time,rhs);
	_switch_additional_data = rhs->_switch_additional_data;
	rhs->_switch_additional_data = NULL;
	_meridianman.Swap(rhs->_meridianman);
	_player_reincarnation.Swap(rhs->_player_reincarnation);
	_generalcard_collection.swap(rhs->_generalcard_collection);
	_player_clock.Swap(rhs->_player_clock);
	_player_randmall.Swap(rhs->_player_randmall);

	_solochallenge.Swap(rhs->_solochallenge);
	Set(_player_mnfaction_info, rhs);
	Set(_player_visa_info, rhs);
	Set(_fix_position_transmit_energy, rhs);
	memcpy(_fix_position_transmit_infos, rhs->_fix_position_transmit_infos, sizeof(_fix_position_transmit_infos));

	_cash_vip_info.Swap(rhs->_cash_vip_info);
	_purchase_limit_info.Swap(rhs->_purchase_limit_info);
    Set(_cash_resurrect_times_in_cooldown, rhs);
	
	Set(_bind_state,rhs);
		
	Set(_shield_energy, rhs);
	Set(_shield_energy_gen, rhs);
	Set(_shield_energy_max, rhs);
	
	Set(_hwid, rhs);

	Set(_increase_pet_time, rhs);
	Set(_real_weapon_class, rhs);
	Set(_fake_weapon_class, rhs);
	// nw
	Set(_charge_merc, rhs);
	Set(_charge_merc_time, rhs);

	Set(_kid_transformation, rhs);
	Set(_kid_transformation_time, rhs);
	memcpy(&_kid_transform_skill_state, &rhs->_kid_transform_skill_state, sizeof(_kid_transform_skill_state));

	Set(_check_interface, rhs);
	Set(_check_genesis_lvl, rhs);
	Set(_check_codex_get_storage, rhs);
	//Set(_question_day, rhs);
	//Set(_question_started, rhs);
	//Set(_question_mode, rhs);
	//Set(_question_progress, rhs);
	//Set(_question_answer, rhs);
	//Set(_question_correct, rhs);
	//Set(_question_time, rhs);
	
	Set(_color_name, rhs);
	Set(_pvp_rank, rhs);
	Set(_pvp_rank_exp, rhs);
	Set(_player_kill, rhs);  
	Set(_monster_kill, rhs);  
	Set(_player_death, rhs);
	Set(_monster_death, rhs);
	Set(_diary_exp, rhs);
	Set(_realm_day_verify, rhs);
	Set(_verify_itens_valid, rhs);
	Set(_has_astrolabe_lock, rhs);
	Set(_enabled_fashion_weapon, rhs);

	// storage 2

	Set(_double_factor_exp, rhs);
	Set(_double_factor_sp, rhs);
	Set(_double_factor_realm, rhs);

	// storage data 1
	
	memcpy(& _autoswap         , &rhs->_autoswap         , sizeof( _autoswap         ));
	memcpy(& _skillsender      , &rhs->_skillsender      , sizeof( _skillsender      ));
	memcpy(& _glyph            , &rhs->_glyph            , sizeof( _glyph            ));
	memcpy(& _carrier          , &rhs->_carrier          , sizeof( _carrier          ));
	memcpy(& _repository       , &rhs->_repository       , sizeof( _repository       ));
	memcpy(& _treasure         , &rhs->_treasure         , sizeof( _treasure         ));
	memcpy(& _lottery          , &rhs->_lottery          , sizeof( _lottery          ));
	memcpy(& _treasure_items   , &rhs->_treasure_items   , sizeof( _treasure_items   ));
	memcpy(& _lib_items        , &rhs->_lib_items        , sizeof( _lib_items        ));
	memcpy(& _pet_skill_temp   , &rhs->_pet_skill_temp   , sizeof( _pet_skill_temp   ));
	memcpy(& _pet_skin         , &rhs->_pet_skin         , sizeof( _pet_skin         ));
	memcpy(& _passwd_safe      , &rhs->_passwd_safe      , sizeof( _passwd_safe      ));
	memcpy(& _day_world_points , &rhs->_day_world_points , sizeof( _day_world_points ));
	//memcpy(& _question_data    , &rhs->_question_data 	 , sizeof( _question_data 	 ));
	
	// storage data 2
	
	memcpy(& _activity  , &rhs->_activity  , sizeof( _activity  ));
	memcpy(& _celestial , &rhs->_celestial , sizeof( _celestial ));

	// nw end
	
	memcpy(&_money,&rhs->_money,sizeof(_money));		
	memcpy(&_glua,&rhs->_glua,sizeof(_glua));

	memcpy(&_codex,&rhs->_codex,sizeof(_codex));
	memcpy(&_kid,&rhs->_kid,sizeof(_kid));
#undef Set
	gactive_imp::Swap(rhs);
}

void 
gplayer_imp::QueryOtherPlayerInfo1(unsigned int count, int ids[])
{	
	abase::vector<int, abase::fast_alloc<> > extern_user_list;

	_runner->begin_transfer();
	gplayer_dispatcher * pRunner = (gplayer_dispatcher * )_runner;
	using namespace S2C;
	for(unsigned int i = 0; i < count; i ++)
	{
		int id =  ids[i];
		int index = _plane->FindPlayer(id);
		if(index < 0) 
		{
			//���������ⲿ����ѯһ��
			world::object_info info;
			if(_plane->QueryObject(XID(GM_TYPE_PLAYER,id),info))
			{
				if(info.pos.horizontal_distance(_parent->pos) < 150*150
					&& ( ((gplayer*)_parent)->anti_invisible_degree >= info.invisible_degree
						 || (IsInTeam() && IsMember(XID(GM_TYPE_PLAYER,id))) || IsArenaMember(XID(GM_TYPE_PLAYER,id))) )
				{
					//�ռ��ö��󣬹����淢����ѯ��Ϣ
					extern_user_list.push_back(id);
				}
			}
			continue;
		}
		gplayer * pPlayer = _plane->GetPlayerByIndex(index);
		if(pPlayer->IsActived() && pPlayer->pos.horizontal_distance(_parent->pos) < 150*150 
			&& ( ((gplayer*)_parent)->anti_invisible_degree >= pPlayer->invisible_degree 
			     || ((gplayer*)_parent)->team_id > 0 && ((gplayer*)_parent)->team_id == pPlayer->team_id) ) 
		{
			if(CMD::Make<INFO::player_info_1>::From(pRunner->_pw,pPlayer))
			{
				pRunner->_pw._counter ++;
			}
		}
	}
	_runner->end_transfer();

	//��������ҵ���Ϣ ��Щ��Ϣ�϶������ⲿ��Ϣ
	if(extern_user_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_QUERY_INFO_1,XID(-1,-1),_parent->ID,_parent->pos,pRunner->_header.cs_id,&(pRunner->_header.cs_sid),sizeof(int));
		_plane->SendPlayerMessage(extern_user_list.size(),extern_user_list.begin(),msg);
	}
}

void 
gplayer_imp::QueryNPCInfo1(unsigned int count, int ids[])
{	
	abase::vector<XID, abase::fast_alloc<> > extern_npc_list;
	gplayer_dispatcher * pRunner = (gplayer_dispatcher * )_runner;
	using namespace S2C;
	for(unsigned int i = 0; i < count; i ++)
	{
		//npc�Ĵ����ȼ�һЩ
		world::object_info info;
		XID  id(GM_TYPE_NPC,ids[i]);
		if(_plane->QueryObject(id,info))
		{
			if(info.pos.horizontal_distance(_parent->pos) < 150*150
				&& ((gplayer*)_parent)->anti_invisible_degree >= info.invisible_degree)
			{
				//�ռ��ö��󣬹����淢����ѯ��Ϣ
				extern_npc_list.push_back(id);
			}
		}
	}

	//��������ҵ���Ϣ ��Щ��Ϣ�϶������ⲿ��Ϣ
	if(extern_npc_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_QUERY_INFO_1,XID(-1,-1),_parent->ID,_parent->pos,pRunner->_header.cs_id,&(pRunner->_header.cs_sid),sizeof(int));
		_plane->SendMessage(extern_npc_list.begin(),extern_npc_list.end(),msg);
	}
}

static bool regen_addon(int item_id, addon_data & data)
{
	return world_manager::GetDataMan().generate_addon_from_rands(item_id,data.id,data) >=0;
}


void 
gplayer_imp::IdentifyItemAddon(unsigned int index, unsigned int fee)
{
	if(index >= _inventory.Size())
	{
		ASSERT(false && "ǰ��Ӧ���Ѿ�������");
		return ;
	}
	ASSERT(GetAllMoney() >= fee && "ǰ���Ѿ�������");
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL) 
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		_runner->identify_result(index,1);
		return ;
	}

	if(it.body->RegenAddon(it.type,regen_addon))
	{
		//���ɳɹ������ٽ�Ǯ���������ݸ��ͻ���.....
		SpendAllMoney(fee,true);
		SelfPlayerMoney();
		PlayerGetItemInfo(IL_INVENTORY,index);
		_runner->identify_result(index,0);
	}
	else
	{
		_runner->error_message(S2C::ERR_SERVICE_ERR_REQUEST);
		_runner->identify_result(index,2);
	}
}

bool 
gplayer_imp::RefineItemAddon(unsigned int index, int item_type, int rt_index, int npcid)
{
	if(index >= _inventory.Size()) return false;
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;

	if(rt_index >= 0 &&  (unsigned int)rt_index  >= _inventory.Size()) return false;

	int material_need = 0xFFFFF;
	int refine_addon = world_manager::GetDataMan().get_item_refine_addon(item_type,material_need);
	if(refine_addon <=0 || material_need <= 0) return false;

	//������ʯ�Ƿ��㹻
	int material_id = 0;
	int material_index = -1;
	int * ITEM_REFINE = LuaManager::GetInstance()->GetConfig()->ITEM_REFINE;
	
	for (int i = 0; ITEM_REFINE[i] && i < 16 ; i++)
	{
		material_index = _inventory.Find(0,ITEM_REFINE[i]);
		if(material_index >= 0) 
		{
			material_id = ITEM_REFINE[i];
			break;
		}
	}	
	if (!material_id) return false;	
	

	//�����ʵ���װ���Ƿ����
	float adjust[4] = {0,0,0,0};
	float adjust2[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
	int rt_id = -1;
	if(rt_index >= 0)
	{
		rt_id = _inventory[rt_index].type;
		if(rt_id <= 0) return false;
		DATA_TYPE dt2;
		const REFINE_TICKET_ESSENCE &ess= *(const REFINE_TICKET_ESSENCE*)world_manager::GetDataMan().get_data_ptr(rt_id, ID_SPACE_ESSENCE,dt2);
		if(dt2 != DT_REFINE_TICKET_ESSENCE || &ess == NULL)
		{
			return false;
		}

		//����װ�����˺�һ
		if(ess.binding_only && !(it.proc_type & item::ITEM_PROC_TYPE_BIND)) return false;
		//����װ��Ʒ������
		if(ess.require_level_max && world_manager::GetDataMan().get_item_level(it.type) > ess.require_level_max) return false;

		float adj1 = ess.ext_succeed_prob;
		float adj2 = ess.ext_reserved_prob;
		if(adj1 < 0) adj1 = 0;
		if(adj2 < 0) adj2 = 0;
		if(adj1 > 1.0) adj1 = 1.0;
		if(adj2 > 1.0) adj2 = 1.0;
		if(adj1 != ess.ext_reserved_prob || adj2 != ess.ext_succeed_prob)
		{
			__PRINTF("ǿ��ʱ����״̬����\n");
		}

		adjust[0] = adj1;		//�ɹ�����
		adjust[2] = adj2;		//��һ������

		if(ess.fail_reserve_level )
		{
			adjust[1] = 2.0;	//ӵ������ı���ʯ
		}

		for(unsigned int i =0; i < 12; i ++)
		{
			adjust2[i] = ess.fail_ext_succeed_prob[i];
		}
	}
	int level_result = 0;
	LuaManager * lua = LuaManager::GetInstance();
	if( lua->IsTrue(rt_id,lua->GetConfig()->ITEM_REFINE_FULL) )
	level_result = 1;
	int rst = it.body->RefineAddon(refine_addon, level_result,adjust,adjust2);
	if(rst != item::REFINE_CAN_NOT_REFINE)
	{
		const char * tbuf[] = {"�ɹ�", "�޷�����" , "������ʧ", "���Խ���һ��", "���Ա���", "װ������","δ֪1","δ֪2","δ֪3"};
		GLog::log(GLOG_INFO,"�û�%d������Ʒ%d[%s]������ǰ����%d ���Ļ���ʯ%d ������Ʒ%d",_parent->ID.id, item_type,tbuf[rst],level_result, material_need, rt_id);
		if(level_result >= 6)
		{
			GLog::refine(_parent->ID.id,item_type, level_result, rst, material_need);
		}
	}
	
	switch(rst)
	{
		case item::REFINE_CAN_NOT_REFINE:
		_runner->error_message(S2C::ERR_REFINE_CAN_NOT_REFINE);
		return true;
		break;		

		case item::REFINE_SUCCESS:
		{
			_runner->refine_result(0);
			PlayerGetItemInfo(IL_INVENTORY,index);
			
			if (level_result+1 >= 9)
			SendRefineSuccessMessage(npcid, index, level_result+1);
		}		
		break;

		case item::REFINE_FAILED_LEVEL_0:
		_runner->refine_result(1);
		break;

		default:
		GLog::log(GLOG_ERR,"����װ��ʱ�������쳣����%d",rst);
		case item::REFINE_FAILED_LEVEL_1:
		_runner->refine_result(2);
		PlayerGetItemInfo(IL_INVENTORY,index);
		break;

		case item::REFINE_FAILED_LEVEL_2:
		_runner->refine_result(3);
		PlayerGetItemInfo(IL_INVENTORY,index);
		
		break;
	}
	
	//������ɾ����Ʒ
	RemoveItems(material_id,material_need, S2C::DROP_TYPE_USE, true);

	//���ʹ���˵���ʯ����ôɾ��֮
	if(rt_index >= 0)
	{
		item& it = _inventory[rt_index];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);

		_inventory.DecAmount(rt_index, 1);
		_runner->player_drop_item(IL_INVENTORY,rt_index,rt_id, 1 ,S2C::DROP_TYPE_USE);
	}
	return true;
}

#define MAFIA_PVP_LEVEL_LIMIT 30
void 
gplayer_imp::UpdateMafiaPvP(unsigned char pvp_mask)
{
	if(_basic.level < MAFIA_PVP_LEVEL_LIMIT) return; // 30��������Ҳ�����
	
	if(pvp_mask & 2) // �ܹ�����
		_enemy_faction |= FACTION_MPVP_MINE_CAR;
	else
		_enemy_faction &= ~FACTION_MPVP_MINE_CAR;

	if(pvp_mask & 4) // �ܹ�������
		_enemy_faction |= FACTION_MPVP_MINE_BASE;
	else
		_enemy_faction &= ~FACTION_MPVP_MINE_BASE;
}

void 
gplayer_imp::SetDBMNFactionInfo(int64_t unifid)
{
	_player_mnfaction_info.unifid = unifid;
	
	gplayer * pPlayer = (gplayer*)_parent;
	pPlayer->mnfaction_id = unifid;

	if(unifid)
		pPlayer->object_state2 |= gactive_object::STATE_MNFACTION_MASK;
	else
		pPlayer->object_state2 &= ~gactive_object::STATE_MNFACTION_MASK;
}

void 
gplayer_imp::UpdateMafiaInfo(int id, char rank, unsigned char pvp_mask, int64_t unifid)
{
	gplayer * pPlayer = (gplayer*)_parent;
	if(pPlayer->id_mafia != id)
	{
		//������ɷ����仯 �ȼ���һ�µ�ǰ˫��ʱ��
		CalcRestTime();
		//������ɱ��˾���չ��׶�
		_faction_contrib.consume_contrib = 0;
		_faction_contrib.exp_contrib = 0;
		_faction_contrib.cumulate_contrib = 0;
		_runner->faction_contrib_notify();
		if(id == 0)
		{
			//�����ǰû�а������ͬ�˵ж�
			_faction_alliance.clear();
			_faction_hostile.clear();
			_runner->faction_relation_notify();
			GLog::log(GLOG_INFO,"role%d mafiaid%d reset to 0",pPlayer->ID.id,pPlayer->id_mafia);
		}
	}
	_player_mnfaction_info.unifid = unifid;
	pPlayer->mnfaction_id = unifid;

	if(unifid)
		pPlayer->object_state2 |= gactive_object::STATE_MNFACTION_MASK;
	else
		pPlayer->object_state2 &= ~gactive_object::STATE_MNFACTION_MASK;

	pPlayer->id_mafia = id;
	pPlayer->rank_mafia = rank;
	if(id)
	{
		pPlayer->object_state |= gactive_object::STATE_MAFIA;
	}
	else
	{
		pPlayer->object_state &= ~gactive_object::STATE_MAFIA;
	}

	if(pPlayer->pPiece)
	{
		_runner->mafia_info_notify();
	}

	if(pvp_mask)
		pPlayer->object_state2 |= gactive_object::STATE_MAFIA_PVP_MASK;
	else
		pPlayer->object_state2 &= ~gactive_object::STATE_MAFIA_PVP_MASK;
	
	if(pPlayer->mafia_pvp_mask != pvp_mask)
	{
		UpdateMafiaPvP(pvp_mask);
		pPlayer->mafia_pvp_mask = pvp_mask;		
		if(pPlayer->pPiece)
		{
			_runner->player_mafia_pvp_mask_notify(pvp_mask);
		}
	}
}

void 
gplayer_imp::UpdateFactionRelation(int * alliance, unsigned int asize, int * hostile, unsigned int hsize, bool notify_client)
{
	_faction_alliance.clear();
	for(unsigned int i=0; i<asize; i++)
		_faction_alliance[ alliance[i] ] = 1;
	_faction_hostile.clear();
	for(unsigned int i=0; i<hsize; i++)
		_faction_hostile[ hostile[i] ] = 1;
	if(notify_client) _runner->faction_relation_notify();
}

bool 
gplayer_imp::StartTravelSession(unsigned int min_time,unsigned int max_time, const A3DVECTOR & dest_pos,float speed,int vehicle)
{
	ASSERT(_player_state == PLAYER_STATE_NORMAL && !_parent->IsZombie());
	_player_state = PLAYER_STATE_TRAVEL;
	gplayer * pPlayer = GetParent();
	pPlayer->vehicle = vehicle  & 0xFF;
	//����ȡ��������ģʽ
	//pPlayer->object_state |= gactive_object::STATE_TRAVEL;
	_provider.pos = dest_pos;
	_provider.id = XID(-1,-1);
	return true;
}

bool 
gplayer_imp::CompleteTravel(int vehicle, const A3DVECTOR & target)
{
	ASSERT(_player_state == PLAYER_STATE_TRAVEL);
	if(_player_state != PLAYER_STATE_TRAVEL) return false;
	gplayer * pPlayer = GetParent();
	_player_state = PLAYER_STATE_NORMAL;
	//����ȡ��������ģʽ
	//	pPlayer->object_state &= ~gactive_object::STATE_TRAVEL;
	if(target.squared_distance(pPlayer->pos) > 10)
	{
		LongJump(target);
	}
	return true;
}

bool 
gplayer_imp::TestSanctuary()
{
//	if(_filters.IsFilterExist(FILTER_INDEX_PVPLIMIT)) return true;
	if(player_template::IsInSanctuary(_parent->pos))
	{
		_filters.AddFilter(new pvp_limit_filter(this,FILTER_INDEX_PVPLIMIT));
		return true;
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%dδ���밲ȫ��(%f,%f)",_parent->ID.id, _parent->pos.x,_parent->pos.z);
	}
	return false;
}

bool
gplayer_imp::TestPKProtected()
{
	if(_basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
	{
		GetParent()->object_state |= gactive_object::STATE_PVPMODE;
		_pvp_enable_flag = true;
		_pvp_cooldown = PVP_STATE_COOLDOWN;
		_runner->enable_pvp_state(PLAYER_PVP_CLIENT);
		_runner->player_pvp_cooldown(_pvp_cooldown);
	}

	//��pve���������ֲ��ܿ���pk�����Բ�Ҫ�����������
	if (_basic.level <= LuaManager::GetInstance()->GetConfig()->pvp_protect_level_min && _invader_state == gactive_imp::INVADER_LVL_0)
	{
		//ֻ�����ֵȼ����²������û���ڷ�������״̬����Ч
		if (player_template::IsInPKProtectDomain(_parent->pos))
		{
			_filters.AddFilter(new pk_protected_filter(this,FILTER_INDEX_PK_PROTECTED));
			return true;
		}
	}
	//��ͼȥ��pk����Ч��
	_filters.RemoveFilter(FILTER_INDEX_PK_PROTECTED);
	return false;
}

void 
gplayer_imp::PlayerEnterServer(int source_tag)
{
	int tag = world_manager::GetWorldTag();

	ReInit();
	((gplayer_controller*)_commander)->ReInit();

	_skill.EventLeave(this,source_tag);
	_skill.EventEnter(this,tag);

	bool inv_changed = _inventory.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool eqp_changed = _equipment.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool task_changed = _task_inventory.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed1 = _trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed2 = _trashbox.GetBackpack(1).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed3 = _trashbox.GetBackpack(2).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed4 = _trashbox.GetBackpack(3).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed5 = _trashbox.GetBackpack(4).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed6 = _trashbox.GetBackpack(5).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed7 = _trashbox.GetBackpack(6).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool tb_changed8 = _trashbox.GetBackpack(7).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);
	bool utb_changed1 = _user_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP|item::ITEM_PROC_TYPE_NO_SAVE);

	//ʹ��װ����Ч
	RefreshEquipment();
	//GlyphEquipRefresh();

	CalcEquipmentInfo();
	if(world_manager::GetWorldLimit().nofly)
	{
		//��ͼȥ������Ч��
		_filters.RemoveFilter(FILTER_FLY_EFFECT);
	}

	if(world_manager::GetWorldLimit().nomount)
	{       
		//��ͼȥ�����Ч��
		if(_filters.IsFilterExist(FILTER_INDEX_MOUNT_FILTER))
		{
			_petman.RecallPet(this);
		}
	}
	//ĳЩ����������ʹ�ö౶����	
	if(world_manager::GetWorldLimit().no_multi_exp)
	{
		_multi_exp_ctrl.DeactivateMultiExp(this);	
	}
	
	//�����Ƿ���ˮ��
	TestUnderWater();
	
	TestSanctuary();

	if(!world_manager::GetWorldParam().pve_mode  || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max)
	{
		//�����pvp������,����Ƿ������ְ�ȫ����
		TestPKProtected();
	}

	_ph_control.Initialize(this);
	UpdatePlayerLayer();

	//��ͼ�ָ�����
	_petman.PostSwitchServer(this);

	if(_inv_switch_save_flag || inv_changed)
	{
		PlayerGetInventoryDetail(gplayer_imp::IL_INVENTORY);
	}

	if(_eqp_switch_save_flag || eqp_changed)
	{
		PlayerGetInventoryDetail(gplayer_imp::IL_EQUIPMENT);
	}

	if(_tsk_switch_save_flag || task_changed)
	{
		PlayerGetInventory(gplayer_imp::IL_TASK_INVENTORY);
	}

	if(tb_changed1 || tb_changed2)
	{
		PlayerGetTrashBoxInfo(true);	
	}

	if(tb_changed3 || tb_changed4 || tb_changed5 || tb_changed6 || tb_changed7 || tb_changed8)
	{
		PlayerGetPortableTrashBoxInfo(true);
	}

	if(utb_changed1)
	{
		PlayerGetUserTrashBoxInfo(true);	
	}
	
	PlaneEnterNotify(true);
	_runner->send_world_life_time(_plane->w_life_time);
	
	if(GetCountryId()) GMSV::CountryBattleEnterMap(_parent->ID.id, world_manager::GetWorldTag());

	PlayerTaskInterface tif(this);
	tif.PlayerSwitchScene();
	CalcProfitLevel();
	_runner->update_profit_time(S2C::CMD::player_profit_time::PLAYER_SWITCH_SERVER, _profit_time, _profit_level);
	_runner->send_timestamp(); 
	_plane->SyncPlayerWorldGen((gplayer*)_parent);
	
 	if(world_manager::GetIsSoloTowerChallengeInstance())
 	{
 		PlayerEnterSoloChallengeInstance();
	}
	
	LuaManager * lua = LuaManager::GetInstance();
	lua->PlayerEnterWorld(_parent->ID.id);
	lua->SendPlayerLuaInfo(_parent->ID.id);

	/*166+*/
	if (GetPlayerClass() == USER_CLASS_HAG)
	{
		PetEnterWorldSkinStorage();
	}

	_real_weapon_class = _cur_item.weapon_class;

	SetAnecdotePoints();

	/*171+*/
	PlayerWeaponUpdateEnterWorld();

	/*170+ Códice*/
	FashionUpdateActivate(true);

	/*161+*/
	RefreshInventoryNewArmorEnter(false, false);
	
	if (EmulateSettings::GetInstance()->GetEnabledChild())
	{
		_kid_addon.ActivateKidsAddons(_parent->ID.id);
	}

	// Kid transform skill state là runtime-only (không persist DB).
	// Khi player relog/enter world đang biến hình → end transformation cleanly.
	// Player mất phần thời gian biến hình còn lại nhưng tránh corrupt skill levels.
	if (_kid_transformation)
	{
		_kid_transformation = 0;
		_kid_transformation_time = 0;
		memset(&_kid_transform_skill_state, 0, sizeof(_kid_transform_skill_state));
		ChangeShape(0);
		object_interface obj_if_kid(this);
		obj_if_kid.RemoveTeamVisibleState(GNET::HSTATE_530);
	}

	FixExpHeartBeat();
}

void 
gplayer_imp::PlayerLeaveServer()
{
	LuaManager::GetInstance()->PlayerLeaveWorld(_parent->ID.id);
	
	if(_carrier.npcid > 0)
	LeaveCarrier(_carrier.npcid, _carrier.pos);

	//����ʲô������
	PlaneEnterNotify(false);

	PlayerTaskInterface tif(this);
	tif.PlayerLeaveScene();

	if(world_manager::GetIsSoloTowerChallengeInstance())
	{
		PlayerLeaveSoloChallengeInstance();
	}

	DeactivateRepositoryAddons();
	_kid_addon.DeactivateKidsAddons(_parent->ID.id);


}

void 
gplayer_imp::PlayerLeaveWorld()
{

	if(_carrier.npcid > 0)
	LeaveCarrier(_carrier.npcid, _carrier.pos);

	
	//���뿪����ʱ �������׼����������ߵ�����Ʒ
	_task_inventory.ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(1).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(2).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(3).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(4).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(5).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(6).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	_trashbox.GetBackpack(7).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);
	
	_user_trashbox.GetBackpack(0).ClearByProcType(item::ITEM_PROC_TYPE_LEAVE_DROP);

	//���ڰ��������Ʒ���е��䴦��
	for(unsigned int i = 0; i < _inventory.Size(); i ++)
	{
		item & it = _inventory[i];
		if(it.type == -1 || (it.proc_type & item::ITEM_PROC_TYPE_LEAVE_DROP) == 0) continue;
		ThrowInvItem(i,it.count,true,S2C::DROP_TYPE_PLAYER);
	}

	//��װ�������Ʒ���е��䴦��
	for(unsigned int i = 0; i < _equipment.Size(); i ++)
	{
		item & it = _equipment[i];
		if(it.type == -1 || (it.proc_type & item::ITEM_PROC_TYPE_LEAVE_DROP) == 0) continue;
		ThrowEquipItem(i,true,S2C::DROP_TYPE_PLAYER);
	}

	PlaneEnterNotify(false);
	
	if(GetCountryId()) GMSV::CountryBattleOffline(_parent->ID.id, GetCountryId());

	PlayerTaskInterface tif(this);
	tif.PlayerLeaveWorld();

	if(world_manager::GetIsSoloTowerChallengeInstance())
	{
		PlayerLeaveSoloChallengeInstance();
	}
	LuaManager * lua = LuaManager::GetInstance();
	lua->PlayerLeaveWorld(_parent->ID.id);
	if (EmulateSettings::GetInstance()->GetEnabledChild())
	{
		_kid_addon.DeactivateKidsAddons(_parent->ID.id);
	}
}

void 
gplayer_imp::PlayerEnterParallelWorld()
{
	_petman.PostSwitchServer(this);
	PlaneEnterNotify(true);
	_runner->send_world_life_time(_plane->w_life_time);
}

void 
gplayer_imp::PlayerLeaveParallelWorld()
{
	ClearSession();
	ClearAction();
	_petman.PreSwitchServer(this);
	_plantpetman.PreSwitchServer(this);
	PlaneEnterNotify(false);
}

void 
gplayer_imp::PlaneEnterNotify(bool is_enter)
{
	//���������Ǹ�log
	if(!world_manager::GetInstance()->IsUniqueWorld())
		GLog::log(LOG_INFO,"�û�%d%s����%d", _parent->ID.id, is_enter?"����":"�뿪", world_manager::GetWorldTag());

	if(is_enter)
	{
		abase::vector<int> list;
		_plane->GetSceneServiceNpc(list);			
		_runner->send_scene_service_npc_list(list.size()/2, list.begin());
	}
	
	if(is_enter) GetCommonDataList(world_manager::GetWorldLimit().common_data_notify);
	if(!world_manager::GetWorldLimit().common_data_notify) return;
	if(is_enter)
	{
		_plane->AddPlayerNode(GetParent());
		_plane->ModifyCommonValue(COMMON_VALUE_ID_PLAYER_COUNT, 1);
	}
	else
	{
		_plane->ModifyCommonValue(COMMON_VALUE_ID_PLAYER_COUNT, -1);
		_plane->DelPlayerNode(GetParent());
	}
}

int 
gplayer_imp::CheckPlayerMove(const A3DVECTOR & offset,int mode,int use_time)
{
	if(use_time  < 100 || use_time > 1000) return -1;
//	if(!CheckMove(use_time,mode)) return -1;
	//é–¼æ¿å•³å?¾ç»˜ç¨‰éˆ§ï¿½å¨‘æ’³ï¿½ï¿½å©€ç£‹ãéŠã‚†ï¿½æ´ªå´šé”Ÿï¿?
	int lmode = _last_move_mode;
	_last_move_mode = mode;
	float t = use_time*0.001f;
	float speed_square = 1000.f;
	float cur_speed = (GetCurSpeed() + 0.2f)*t;	//é–¸æ—‚å§µå©€ä½ºäº¸éŽ´ï¹€æ«¤é–»ã„¥å??é˜æˆ æ‹‹ç»‹åº“å“é”Ÿï¿½
	float max_speed = ((MAX_FLIGHT_SPEED+0.2f)*(MAX_FLIGHT_SPEED+0.3f))*(t*t);

	if(!(mode & C2S::MOVE_MASK_DEAD))
	{
		FindCheater2();
	}
	
	mode = mode & 0x0F;
	/* é‰â•‚ç‘©é£çƒ½å¼°é”Ÿä»‹å¹’æ¾¶åº¢å„°å¨´ç?ºå€›é”Ÿè§£å“é–»ã„¥å«?å©€æ’?å¼?é”Ÿï¿½
	if(mode == C2S::MOVE_MODE_FALL || mode == C2S::MOVE_MODE_JUMP)
	{
		_fall_counter += offset.y;
	}
	else
	{
		if(mode & (C2S::MOVE_MASK_SKY|C2S::MOVE_MASK_WATER))
		{
		}
		else
		{
			if(fabs(_fall_counter) > 15)
			{
			
				//é¡æŽ‘æ‡“ç»»å†?æ„å©µå ?æ‹·ç’è™?ç¸ºéžæ¶’å“
			//	int hp = (int)(fabs(_fall_counter*_fall_counter) * 10);
			//	bool b = false;
			//	SendTo<0>(GM_MSG_HURT,_parent->ID,hp,&b,sizeof(b));
			}
		}
		_fall_counter = 0;
	}
	*/
	
	if(mode == C2S::MOVE_MODE_FALL || mode ==  C2S::MOVE_MODE_FLY_FALL 
			|| mode == C2S::MOVE_MODE_JUMP || mode == C2S::MOVE_MODE_SLIDE)
	{
		//é‰â•‚ç‘©é£é”‹ä¿ŠéŒæ¶šäº?éŽå¹¿åžºé©æ¶šç²©é”Ÿä»‹æŸ…å?˜å´‡ç®–é–ºå›§ç“¨éç³¾ove_modeå¦¤çŠ³ï¿½â‚¬è¤°å›¨ç¦éžæ¿‹å¼¬éŸç‰ˆå€»é–»ã„¥å?°éï¿½é—ï¿½é”å³°ï¿½æŠ½æ•çžç•Œæ®ºå¨´å…¼è‰¾å©€çŒµhase_controlå¨‘æ“„æ‹·é‰â•‚ç?–é”Ÿè—‰çŸé‘ï¸¾æ„éâ•ä»˜é–¸æŽžæ‹?
		//é–¸å¿Žæµ‡é”Ÿç•Œï¿½ï¿½é–ºå‚œæ‡“éŽ?æ»„ç¨‰æ¿ å†©ç®’é‰å å•´éï¿½é—ï¿½é”å³°ï¿½ï¿½
		if(offset.y > (MAX_JUMP_SPEED + 0.3f)*t) 
			return -1;
		else
			speed_square = offset.x *offset.x + offset.z*offset.z;
	}
	else
	{
		speed_square = offset.x *offset.x + offset.z*offset.z;
		float my = fabs(offset.y);
		int xmode = lmode & 0x0F;
		if(my <= cur_speed)
		{
			//å¨´çŠ«å“å¨‘æ–¿ç‰…ç»¡å†©ç¨‰å?¥å‘¬ç²?
		}
		else
		if(offset.y > (MAX_JUMP_SPEED + 0.3f)*t) 
		{
			return -1;
		}
		else if(xmode == C2S::MOVE_MODE_FALL 
				|| xmode ==  C2S::MOVE_MODE_FLY_FALL
				|| xmode ==  C2S::MOVE_MODE_JUMP)
		{
			//å¨´çŠ«å“å¨‘æ–¿ç‰…ç»¡å†©ç¨‰å?¥å‘¬ç²?
		}
		else if(offset.y < -(MAX_JUMP_SPEED*2.0f + 0.3f)*t)
		{
			return -1;
		}
		else
		{	
			if(speed_square > max_speed) return -1;
			//é–¼è?§æ‹·éŽè?„è‰¾éŠ‡å›¨ç¦å?¸å”˜r_speed
			return 3;
		}
	}

	if(isnan(speed_square) || isnan(offset.y)) return -1;
	if(speed_square > max_speed)
	{
		//é¡æŽ‘æ‡“ç»»å†©ç?é¡æ¶’å“é”å³°ï¿½æŠ½å¼µéˆ§ï¿½å?¢å †å‹?â‚?ï¿½é—‚å‹?åŠ§ç»±æ¿‡ç¨‰å®¥å‘¯å®‰é å›¥å„»ç»±æ¿‹æƒ„ç€›æ¨ºå¤é–¹åž?æŒ¸é™å—›å´˜å®„å¸®æ‹·é”Ÿï¿?
		return -1;
	}

	float cur_speed_square = cur_speed * cur_speed;
	float stmp = 1.f/(t*t);
	if(speed_square > cur_speed_square)
	{
		//å©µâ€³å€¹éå¤ŽãéŽ¾å†²ï¿½çŠ»æŸ…é”å³°ï¿½å´‡æ¾é›æ‰®ç®–å¨´æ»ƒæ?™å¨“èˆµå¾„è¤”éˆ§ï¿½é”å³°ï¿½æŠ½æ•å®€å‹?äºå¨‘æ–¿ç‰†éŽ·ä¼´å´¢é¡æ¥€è•‰å¨‘æ“„æ‹·é–»ã„¥å«?å¨“èˆµå¾„è?”éˆ§ï¿½é”å³°ï¿½è™?æ½»å¨‘æ»æ‹·çžæ–¤æ‹·ç‘™å‹?æ§?
		//å¦?ï½†ç‰•é˜æ¶šäº¸é¡æ¶’å“é”å³°ï¿½è™«æ½?é”Ÿä»‹å¹‘éŠâ€³ç‡å¨‘æ“„æ‹?
		if(speed_square * stmp > _speed_ctrl_factor)
		{
			__PRINTF("CBTLé–»ï¿½éŠŠï¹€ç…?%dé¡æŽ‘æ‡“ç»»å†©ç?é¡æ¨»ä»˜å©¢å †å›£å“é”å³°ï¿½ï¿½, speed_square:%f , _speed_ctrl_factor:%f , mode:%d time:%d cur_speed:%f sp%f\n",
					_parent->ID.id,speed_square,_speed_ctrl_factor,mode,use_time,
					cur_speed ,GetCurSpeed()
					);
			__PRINTF("CBTLé–»ï¿½éŠŠï¹€ç…?%då¨´ï½…ç§¶é¤ï¿?(%f,%f,%f) é–¸å??ç¹’äº›(%f,%f,%f)\n",
					_parent->ID.id,_parent->pos.x,_parent->pos.y,_parent->pos.z,
					offset.x,offset.y,offset.z);

			//å©µâ€³å€¹éå¤Œæ¾é›æ‰®ç®–å¨´æ»ƒæ?™å¨“èˆµå¾„è¤”éˆ§ï¿½é”å³°ï¿½æŠ½æ•å®€å‹?äºå¨‘æ–¿ç‰å¨²å—å¹’éŠ‰ï¹€ï¿½æ´ªå´¶éå­˜é™
			_speed_ctrl_factor = cur_speed_square*stmp;
			return -1;
		}
		return 	2;
	}
	else
	{
		//é–ºå›§ç“¨éŒå©ƒç¨‰éˆ§ï¿½å¨‘æ’³ï¿½ï¿½å¨“èˆµå¾„è?å¨ˆæˆ¦å´£é”Ÿä»‹æ‚½éŠŠâ”¾å“é”å³°ï¿½ï¿½
		_speed_ctrl_factor = cur_speed_square * stmp;
	}
	return 0;
}

int gplayer_imp::TransformChatData(const void * data,unsigned int dsize, void * out_buffer, unsigned int len)
{
	if(dsize < sizeof(short)) return 0;
	int cmd = *(short*)data;
	switch(cmd)
	{
		case CHAT_C2S::CHAT_EQUIP_ITEM:
		{
			CHAT_C2S::chat_equip_item & cmd = *(CHAT_C2S::chat_equip_item *)data;
			if(sizeof(cmd) != dsize) return 0;

			item_list &inv  = GetInventory(cmd.where);
			unsigned int index = cmd.index;
			item_data data;
			if(inv.GetItemData(index, data) <= 0) return 0;
			packet_wrapper h1(data.content_length + sizeof(data));

			h1 << (short) CHAT_S2C::CHAT_EQUIP_ITEM << data.type << data.expire_date <<  data.proc_type  << (short)data.content_length;
			h1.push_back(data.item_content, data.content_length);

			if(len < h1.size()) return 0;
			memcpy(out_buffer, h1.data(), h1.size());
			return h1.size();
		}
		break;

		case CHAT_C2S::CHAT_GENERALCARD_COLLECTION:
		{
			CHAT_C2S::chat_generalcard_collection & cmd = *(CHAT_C2S::chat_generalcard_collection *)data;
			if(sizeof(cmd) != dsize) return 0;
		
			DATA_TYPE dt;
			const POKER_ESSENCE * ess = (const POKER_ESSENCE *)world_manager::GetDataMan().get_data_ptr(cmd.card_id, ID_SPACE_ESSENCE,dt);
			if(!ess || dt != DT_POKER_ESSENCE) return 0;
			if(!_generalcard_collection.get((unsigned int)ess->show_order)) return 0;

			packet_wrapper h1(8);
			h1 << (short)CHAT_S2C::CHAT_GENERALCARD_COLLECTION << cmd.card_id;

			if(len < h1.size()) return 0;
			memcpy(out_buffer, h1.data(), h1.size());
			return h1.size();
		}
		break;

        case CHAT_C2S::CHAT_PROPERTY_SCORE:
        {
            CHAT_C2S::chat_property_score& cmd = *(CHAT_C2S::chat_property_score*)data;
            if (sizeof(cmd) != dsize) return 0;

            CHAT_S2C::chat_property_score chat_data = {0};
            chat_data.cmd_id = CHAT_S2C::CHAT_PROPERTY_SCORE;

            unsigned int name_len = _username_len;
            if (name_len >= sizeof(chat_data.name))
                name_len = sizeof(chat_data.name) - 1;
            memcpy(chat_data.name, _username, name_len);
            chat_data.name[name_len] = 0;

            int cls = -1;
            bool gender = false;
            GetPlayerClass(cls, gender);
            chat_data.cls = cls;

            unsigned int value = 0;
            chat_data.level = _basic.level;
            chat_data.fighting_score = player_template::GetFightingScore(this, value);
            chat_data.viability_score = player_template::GetViabilityScore(this, value);

            chat_data.state_num = _visible_team_state.size();
			
			unsigned short vts_id[32];
			for (int i = 0; i < chat_data.state_num && i < 32; i++ )
			{
				vts_id[i] = _visible_team_state[i].id;
			}
			
            chat_data.state = vts_id;

            unsigned int state_size = sizeof(chat_data.state[0]) * chat_data.state_num;
            packet_wrapper h1(sizeof(chat_data) + state_size);
            h1 << chat_data.cmd_id;
            h1.push_back(chat_data.name, sizeof(chat_data.name));
            h1 << chat_data.cls << chat_data.level << chat_data.fighting_score << chat_data.viability_score << chat_data.state_num;
            h1.push_back(chat_data.state, state_size);

            if (len < h1.size()) return 0;
            memcpy(out_buffer, h1.data(), h1.size());
            return h1.size();
        }
        break;

        default:
        break;
	}
	return 0;
}

int
gplayer_imp::PhaseControl(const A3DVECTOR & target, float terrain_height, int mode, int use_time)
{
	return _ph_control.PhaseControl(this, target, terrain_height,mode, use_time);
}

bool 
gplayer_imp::PlayerDestroyItem(int where, unsigned int index, int item_type)
{
	item_list & list = GetInventory(where);
	if(index >= list.Size()) return false;
	if(list[index].type == -1 || list[index].type != item_type) return false;
	item it;
	list.Remove(index,it);
	_runner->player_drop_item(where,index,item_type,it.count,S2C::DROP_TYPE_DESTROY);
	it.Release();
	return true;
}

void 
gplayer_imp::PlayerEnablePVPState()
{
	LuaManager * lua = LuaManager::GetInstance();
	if(!world_manager::GetWorldParam().pve_mode || _basic.level > lua->GetConfig()->pvp_protect_level_max) return; //����PVPģʽ�������ֶ��򿪴˿��أ�ֱ�ӷ���
	if(_pvp_enable_flag) return;
	if(_basic.level <= lua->GetConfig()->pvp_protect_level_min) return;
	GetParent()->object_state |= gactive_object::STATE_PVPMODE;
	_pvp_enable_flag = true;
	_pvp_cooldown = lua->GetConfig()->pvp_cooldown_time;
	_runner->enable_pvp_state(PLAYER_PVP_CLIENT);
	_runner->player_pvp_cooldown(_pvp_cooldown);
	
}

void 
gplayer_imp::PlayerDisablePVPState()
{
	if(!world_manager::GetWorldParam().pve_mode || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max ) return; //����PVPģʽ�������ֶ��رմ˿��أ�ֱ�ӷ���
	if(!_pvp_enable_flag) return;
	if(_pvp_cooldown > 0) 
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DISABLE_PVP_STATE);
		_runner->player_pvp_cooldown(_pvp_cooldown);
		return;
	}
	if(_invader_state != INVADER_LVL_0)  
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DISABLE_PVP_STATE);
		return ;
	}
	GetParent()->object_state &= ~gactive_object::STATE_PVPMODE;
	_pvp_enable_flag = false;
	_pvp_cooldown = 0;
	_runner->disable_pvp_state(PLAYER_PVP_CLIENT);
}

const A3DVECTOR & 
gplayer_imp::GetLogoutPos(int &world_tag)
{
	if(_logout_pos_flag)
	{ 
		world_tag = _logout_tag;
		return _logout_pos;
	}
	else
	{ 
		world_tag = world_manager::GetWorldTag();
		return _parent->pos;
	}
}

void 
gplayer_imp::SetCoolDownData(const void * buf, unsigned int size)
{
	if(size == 0) return;
	raw_wrapper ar(buf,size);
	_cooldown.DBLoad(ar);
}

void 
gplayer_imp::GetCoolDownData(archive & ar)
{
	_cooldown.DBSave(ar);
}


void 
gplayer_imp::GetCoolDownDataForClient(archive & ar)
{
	_cooldown.SaveForClient(ar);
}

void 
gplayer_imp::EnhanceBreathCapacity(int value)
{
	_breath.EnhanceBreathCapacity(value);
}

void 
gplayer_imp::AdjustBreathDecPoint(int offset)
{
	_breath.AdjustPointAdjust(offset);
}

void 
gplayer_imp::ImpairBreathCapacity(int value)
{
	_breath.ImpairBreathCapacity(value);
}

void 
gplayer_imp::InjectBreath(int value) 
{
	_breath.IncBreath(value);
}

void 
gplayer_imp::EnableEndlessBreath(bool bVal)
{
	_breath.EnableEndlessBreath(bVal);
}

void 
gplayer_imp::EnableFreePVP(bool bVal)
{
	//���ﲻ���ͻ��˷�������
	_free_pvp_mode = bVal;
}

void 
gplayer_imp::SwitchFashionMode()
{
	gplayer * pPlayer = GetParent();
	bool is_fashion;
	if((is_fashion = (pPlayer->object_state & gactive_object::STATE_FASHION_MODE)))
	{
		pPlayer->object_state &= ~gactive_object::STATE_FASHION_MODE;
	}
	else
	{
		pPlayer->object_state |= gactive_object::STATE_FASHION_MODE;
	}

	/*171+*/
	_runner->player_enable_fashion_mode(!is_fashion, _enabled_fashion_weapon);
}

void 
gplayer_imp::ObjReturnToTown()
{
	ReturnToTown();
}

void 
gplayer_imp::AddEffectData(short effect)
{
	gplayer * pPlayer = GetParent();
	unsigned int count = pPlayer->effect_count;
	if(count >= MAX_PLAYER_EFFECT_COUNT)  return;
	for(unsigned int i = 0; i< count; i ++)
	{
		if(pPlayer->effect_list[i] == effect) return ;
	}
	pPlayer->effect_list[count] =effect;
	pPlayer->effect_count ++;
	if(_parent->pPiece)
	{
		_runner->player_enable_effect(effect);
	}
}

void 
gplayer_imp::RemoveEffectData(short effect)
{
	gplayer * pPlayer = GetParent();
	unsigned int count = pPlayer->effect_count;
	for(unsigned int i = 0; i< count; i ++)
	{
		if(pPlayer->effect_list[i] == effect) 
		{
			if( i < count -1)
			{
				pPlayer->effect_list[i] = pPlayer->effect_list[count - 1];
			}
			pPlayer->effect_count --; 
			if(_parent->pPiece)
			{
				_runner->player_disable_effect(effect);
			}
			return ;
		}
	}
}

void 
gplayer_imp::GetPlayerCharMode(archive & ar)
{
	char data;
	gplayer * pPlayer = GetParent();
	data = (pPlayer->object_state & gactive_object::STATE_FASHION_MODE)?1:0;
	if(data)
	{
		ar << (int) PLAYER_CHAR_MODE_FASHION  << (int)1;
	}
}


void 
gplayer_imp::SetPlayerCharMode(const void * buf, unsigned int size)
{
	if(size < sizeof(int)*2) return ; //do nothing 
	int * data = (int *)buf;
	gplayer * pPlayer = GetParent();
	unsigned int count = size / sizeof(int);
	for(unsigned int i= 0; i < count - 1 ; i += 2)
	{
		switch (data[i])
		{
			case PLAYER_CHAR_MODE_FASHION:
				if(data[i+1]) pPlayer->object_state |=  gactive_object::STATE_FASHION_MODE;
			break;
		}
	}
}

void 
gplayer_imp::EnterCosmeticMode(unsigned short inv_index, int cos_id)
{
	ASSERT(_player_state == PLAYER_STATE_NORMAL);
	_player_state = PLAYER_STATE_COSMETIC;
	//����Ϣ���ͻ���
	_runner->begin_cosmetic(inv_index);
	GLog::log(LOG_INFO,"�û�%d��ʼ����",_parent->ID.id);
}

void 
gplayer_imp::LeaveCosmeticMode(unsigned short inv_index)
{	
	ASSERT(_player_state == PLAYER_STATE_COSMETIC);
	_player_state = PLAYER_STATE_NORMAL;
	//����Ϣ���ͻ���
	_runner->end_cosmetic(inv_index);
	GLog::log(LOG_INFO,"�û�%d��������",_parent->ID.id);
}

void 
gplayer_imp::PlayerRegionTransport(int rindex, int tag)
{
	//���������ڸ�λ���й�,���Ա�����Ҫ�Ŷӵ�����
	AddSession(new session_region_transport(this, rindex, tag));
	StartSession();
}

bool
gplayer_imp::RegionTransport(int rindex, int tag)
{
	if(_player_state != PLAYER_STATE_NORMAL)
	{
		//������״̬�������л�
		return false;
	}

	//��鴫���Ƿ����Ҫ��
	A3DVECTOR target_pos; 
	int target_tag;
	if(!city_region::GetRegionTransport(_parent->pos,tag,rindex,target_pos,target_tag))
	{
		return false;
	}

	//���Ǽ���region transport����ȴ
	
	//�ֶ�����5�����Ա�֤������뽨����
	target_pos.y += 0.05f;
	//ClearNextSession();

	//��Player���и������� 
	return LongJump(target_pos,target_tag);
}

void 
gplayer_imp::EnterResurrectReadyState(float exp_reduce,float hp_factor, float mp_factor)
{
	//������Ը����״̬ ֪ͨ�ͻ����ǣ������Լ��ı�־ �;�����ʧ ��Щ���ݻ�Ҫ����
	if(!_parent->IsZombie()) return;

	_resurrect_state = true;
	if(exp_reduce < 0.f) exp_reduce =0.f;
	if(exp_reduce >1.f)  exp_reduce = 1.f;
	if(hp_factor <0.f) hp_factor = 0.f;
	if(hp_factor >1.0f) hp_factor = 1.0f;
	if(mp_factor <0.f) mp_factor = 0.f;
	if(mp_factor >1.0f) mp_factor = 1.0f;
	if(_resurrect_exp_reduce > exp_reduce)
	{
		_resurrect_exp_reduce = exp_reduce;
		_resurrect_hp_factor = hp_factor;
		_resurrect_mp_factor = mp_factor;
	}

	if(_parent->pPiece) _runner->enable_resurrect_state(_resurrect_exp_reduce);
}

bool 
gplayer_imp::CheckCoolDown(int idx) 
{ 
	return _cooldown.TestCoolDown(idx);
}

void 
gplayer_imp::SetCoolDown(int idx, int msec) 
{ 
	if(_no_cooldown_mode && player_template::GetDebugMode())
	{
		msec = 100;
	}
	_cooldown.SetCoolDown(idx,msec);
	_runner->set_cooldown(idx, msec);
}

void 
gplayer_imp::CoolDownReduceAtr(int idx, int msec) 
{ 
	int get_cooldown = _cooldown.CoolDownReduceAtr(idx,msec);
	_runner->set_cooldown(idx, get_cooldown);
}

void 
gplayer_imp::RebuildAllInstanceData(int create_time)
{
	//�����ݣ�Ҫ��ͷ��ʼ
	const abase::vector<int> & list = world_manager::_instance_tag_list;
	_cur_ins_key_list.reserve(list.size());
	_cur_tag_counter.reserve(list.size());
	_ins_key_timer.reserve(list.size());
	for(unsigned int i = 0; i < list.size(); i ++)
	{
		_cur_tag_counter.push_back(abase::pair<int,int>(list[i],create_time));
		_ins_key_timer.push_back(abase::pair<int,int>(0,0));
	}
	//_ins_key_timer�� timer/��Ƕ� ����_cur_tag_counter������һһ��Ӧ
	//����timer������tag�������ϴν��븱����ʱ�� ���������Ƿ������ø���
	//�Ѿ�����������Ļ��������������룬�����������timer��ʱ���5���ӵ�����
	RebuildInstanceKey();
	//���Ƴ�����ĸ���key������ת�Ʒ�����������ʧ��
	_team_ins_key_list = _cur_ins_key_list;
}

#define CUR_INSTANCE_KEY_DATA_VERSION  (int)0
void 
gplayer_imp::InitInstanceKey(int create_time, const void * buf , unsigned int size)
{
	if(size < sizeof(int)*2)
	{
		RebuildAllInstanceData(create_time);
		return ;
	}

	int tag_counter = world_manager::_instance_tag_list.size();
	int * pData = (int*)buf;
	if(pData[0] != CUR_INSTANCE_KEY_DATA_VERSION || pData[1] != tag_counter)
	{
		RebuildAllInstanceData(create_time);
		return ;
	}
	if(size != (sizeof(int)*2 + pData[1] *(sizeof(int) *2 + sizeof(int)*3 + sizeof(int)*2)))
	{
		//ASSERT(false && "���ݴ�С����ȷ");
		RebuildAllInstanceData(create_time);
		return ;
	}

	//�������ݸ�ʽ����:
	/*
	   struct
	   {
	   int version;
	   unsigned int count;

	   _cur_tag_counter ���� 
	   _ins_key_timer ����
	   _cur_ins_key_list ����
	   };
	 */
	unsigned int count = pData[1];
	pData += 2;
	_cur_tag_counter.reserve(count);
	_ins_key_timer.reserve(count);
	_cur_ins_key_list.reserve(count);

	_cur_tag_counter.clear();
	_ins_key_timer.clear();
	_cur_ins_key_list.clear();
	const abase::vector<int> & list = world_manager::_instance_tag_list;
	for(unsigned int i = 0;i < count ; i ++)
	{
		int tag = *pData++;
		int counter = *pData++;
		_cur_tag_counter.push_back(abase::pair<int,int>(tag,counter));
		int j = 0;
		for(j = 0; j < tag_counter; j ++)
		{
			if(list[j] == tag) break;
		}
		if(j == tag_counter)
		{
			//����Tag�����˱仯�������������ɸ���ID
			__PRINTF("����TAG���б仯���������ɸ���ID�б�\n");
			_cur_tag_counter.clear();
			RebuildAllInstanceData(create_time);
			return ;
		}
	}
	for(unsigned int i = 0;i < count ; i ++)
	{
		int time = *pData++;
		int state = *pData++;
		_ins_key_timer.push_back(abase::pair<int,int>(time,state));
	}
	for(unsigned int i = 0;i < count ; i ++)
	{
		int tag = *pData++;
		int key1= *pData++;
		int key2= *pData++;
		_cur_ins_key_list[tag] = abase::pair<int,int>(key1,key2);
	}

	//���Ƴ�����ĸ���key������ת�Ʒ�����������ʧ��
	_team_ins_key_list = _cur_ins_key_list;
}

void 
gplayer_imp::SaveInstanceKey(archive & ar)
{
	unsigned int count = _cur_tag_counter.size();

	ar << CUR_INSTANCE_KEY_DATA_VERSION;
	ar << count;
	ar.push_back(_cur_tag_counter.begin(), count * (sizeof(int)*2) );
	ar.push_back(_ins_key_timer.begin(), count * (sizeof(int)*2) );

	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
	for(it = _cur_ins_key_list.begin(); it != _cur_ins_key_list.end(); ++it)
	{
		ar << it->first << it->second.first << it->second.second;
	}
}

void
gplayer_imp::RebuildInstanceKey()
{
	_cur_ins_key_list.clear();
	for(unsigned int i = 0; i < _cur_tag_counter.size(); i ++)
	{
		abase::pair<int,int> & p = _cur_tag_counter[i];
		abase::pair<int,int> value = abase::pair<int,int>(_parent->ID.id, p.second++);
		_cur_ins_key_list[p.first] = value;
		_ins_key_timer[i].second = 0;
	}
}

bool 
gplayer_imp::GetInstanceKeyBuf(char * buf, unsigned int size)
{
	if(size != _team_ins_key_list.size() * (sizeof(int)*3)) return false;
	int * pBuf = (int*)buf;
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
	for(it = _team_ins_key_list.begin(); it != _team_ins_key_list.end(); ++it)
	{
		*pBuf++ = it->first;
		*pBuf++ = it->second.first;
		*pBuf++ = it->second.second;
	}
	return true;
}

bool 
gplayer_imp::SetInstanceKeyBuf(const void * buf, unsigned int size)
{
	if(size != _cur_tag_counter.size() * sizeof(int)*3)
	{
		return false;
	}

	_team_ins_key_list.clear();
	int * pBuf = (int*)buf;
	unsigned int offset = 0;
	for(unsigned int i = 0; i < size; i += sizeof(int)*3,offset ++)
	{
		int tag = *pBuf++;
		int key1= *pBuf++;
		int key2= *pBuf++;
		_team_ins_key_list[tag] = abase::pair<int,int>(key1,key2);

		//�������и����Ľ����־
		_ins_key_timer[offset].second = 0;
	}
	return true;
}

int
gplayer_imp::CheckInstanceTimer(int tag)
{
	int t = g_timer.get_systime();
	for(unsigned int i = 0; i < _cur_tag_counter.size(); i ++)
	{
		if(_cur_tag_counter[i].first == tag)
		{
			if(_ins_key_timer[i].second == 1) return 1;
			if(t - _ins_key_timer[i].first > 3)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}

void
gplayer_imp::SetInstanceTimer(int tag)
{
	int t = g_timer.get_systime();
	for(unsigned int i = 0; i < _cur_tag_counter.size(); i ++)
	{
		if(_cur_tag_counter[i].first == tag)
		{
			if(_ins_key_timer[i].second == 0) 
			{
				_ins_key_timer[i].first = t;
			}
			_ins_key_timer[i].second = 1;
			return ;
		}
	}
}

void 
gplayer_imp::ResetInstance(int world_tag)
{
	for(unsigned int i = 0; i < _cur_tag_counter.size(); i ++)
	{
		abase::pair<int,int> & p = _cur_tag_counter[i];
		if(p.first != world_tag) continue;

		if(_team.IsInTeam())
		{
			if(_team.IsLeader())
			{
				abase::pair<int,int> value = abase::pair<int,int>(_parent->ID.id, p.second++);
				_cur_ins_key_list[world_tag] = value;
				_ins_key_timer[i].second = 0;

				instance_hash_key ikey[2];
				ikey[0].key1 = _team_ins_key_list[world_tag].first;
				ikey[0].key2 = _team_ins_key_list[world_tag].second;
				ikey[1].key1 = value.first;
				ikey[1].key2 = value.second;
				_team_ins_key_list[world_tag] = value;
				MSG msg;
				BuildMessage(msg,GM_MSG_REBUILD_TEAM_INSTANCE_KEY,XID(-1,-1),_parent->ID,_parent->pos,world_tag,ikey,sizeof(ikey));
				_team.SendGroupMessage(msg);
			}
			else
			{
				instance_hash_key ikey;
				ikey.key1 = _team_ins_key_list[world_tag].first;
				ikey.key2 = _team_ins_key_list[world_tag].second;
				SendTo<0>(GM_MSG_REBUILD_TEAM_INSTANCE_KEY_REQ,_team.GetLeader(),world_tag,&ikey,sizeof(ikey));
			}		
		}
		else
		{
			abase::pair<int,int> value = abase::pair<int,int>(_parent->ID.id, p.second++);
			_cur_ins_key_list[world_tag] = value;
			_ins_key_timer[i].second = 0;
		}
		break;
	}
}

void 
gplayer_imp::CosmeticSuccess(int ticket_inv_idx, int ticket_id, int result, unsigned int crc)
{
	int rst = ticket_inv_idx;
	if(!IsItemExist(ticket_inv_idx,ticket_id,1))
	{
		rst = _inventory.Find(0,ticket_id);
	}

	if(rst >= 0)
	{
		item& it = _inventory[rst];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);

		_inventory.DecAmount(rst,1);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst, ticket_id, 1,S2C::DROP_TYPE_USE);
	}
	//������ȴʱ��
	SetCoolDown(COOLDOWN_INDEX_FACETICKET,FACETICKET_COOLDOWN_TIME);
	GetParent()->custom_crc = crc;
	_runner->cosmetic_success(crc);
	GLog::log(LOG_INFO,"�û�%d�������,��crc=%d,������%d��%d",_parent->ID.id,crc,rst>=0?1:0,ticket_id);
	//������ǰSession;
	EndCurSession();
	StartSession();
}

void 
gplayer_imp::SetPerHitAP(int ap_per_hit)
{
	_ap_per_hit = ap_per_hit;
}

void 
gplayer_imp::ModifyPerHitAP(int delta)
{
	_ap_per_hit += delta;
}

bool 
gplayer_imp::IsEquipWing()
{
	const item & it = _inventory[item::EQUIP_INDEX_FLYSWORD];
	if(it.type <= 0 || it.body == NULL) return false;
	return it.body->GetItemType() == item_body::ITEM_TYPE_WING;
}

void 
gplayer_imp::OnHeal(const XID & healer, int life)
{
	//���г�޵�����
	//AddAggroToEnemy(healer,life);
}

void 
gplayer_imp::Say(const char * msg,int channel)
{
	if(_parent->pPiece)
	{
		SaySomething(_plane,_parent->pPiece,msg,channel,0);
	}
}

void 
gplayer_imp::GBKSay(const char * msg, int channel)
{
	if(msg == NULL || *msg == 0) return;
	
	iconv_t cd = iconv_open("UCS2", "GBK");
	if(cd == (iconv_t)-1) return;
	char * ibuf = (char *)msg;
	size_t isize = strlen(msg);
	char * obuf = (char *)malloc(isize * 2);
	size_t osize = isize * 2;
	char * tmpbuf = obuf;
	size_t tmpsize = osize;
	if(iconv(cd, &ibuf, &isize, &tmpbuf, &tmpsize) == (size_t)(-1))
	{
		iconv_close(cd);
		free(obuf);
		return;
	}
	AutoBroadcastChatMsg(_plane, _parent->pPiece, obuf, osize-tmpsize, channel,0,0,0,0,0);
	iconv_close(cd);
	free(obuf);
}

void 
gplayer_imp::OnUseAttackRune()
{
	item& it = _equipment[item::EQUIP_INDEX_RUNE_SLOT];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
	
	int id = _equipment[item::EQUIP_INDEX_RUNE_SLOT].type;
	_equipment.DecAmount(item::EQUIP_INDEX_RUNE_SLOT,1);
	_runner->player_drop_item(IL_EQUIPMENT,item::EQUIP_INDEX_RUNE_SLOT,id,1,S2C::DROP_TYPE_RUNE);
}

int 
gplayer_imp::GetLinkIndex()
{
	return GetParent()->cs_index;
}

int 
gplayer_imp::GetLinkSID()
{
	return GetParent()->cs_sid;
}

int 
gplayer_imp::SummonMonster(int mod_id, int count, const XID& target, int target_distance, int remain_time, char die_with_who, int path_id)
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
	prop.parent_is_leader =false;
	prop.use_parent_faction = false;
	prop.die_with_who = die_with_who;
	prop.target_id = target;
	prop.path_id = path_id;
	for(int i =0; i < count; i ++)
	{
		int id = oi.CreateMinors(prop,target,target_distance);
		if(id != -1)	//����npc�ɹ����������ͺܴ���
		{
			XID xid;
			MAKE_ID(xid, id);

			msg_aggro_info_t info;
			info.source = _parent->ID;
			info.aggro = 10000;
			info.aggro_type = 0;
			info.faction = 0xFFFFFFFF;
			info.level = 0;
			
			SendTo<0>(GM_MSG_GEN_AGGRO,xid,0,&info,sizeof(info));
		}
		else if(i == 0) 
			return -1;
	}
	return 0;
}
	
int 
gplayer_imp::SummonNPC(int npc_id, int count, const XID& target, int target_distance, int remain_time)
{
	object_interface oi(this);
	object_interface::npc_param prop;
	memset(&prop,0,sizeof(prop));
	
	prop.npc_id = npc_id;
	prop.remain_time = remain_time;
	for(int i=0; i<count; i++)
	{
		int ret = oi.CreateNPC(prop,target,target_distance);
		if(ret != 0 && i == 0) return -1;
	}
	return 0;	
}

int 
gplayer_imp::SummonMine(int mine_id, int count, const XID& target, int target_distance, int remain_time)
{
	object_interface oi(this);
	object_interface::mine_param prop;
	memset(&prop,0,sizeof(prop));
	
	prop.mine_id = mine_id;
	prop.remain_time = remain_time;
	for(int i=0; i<count; i++)
	{
		int ret = oi.CreateMine(prop,target,target_distance);
		if(ret != 0 && i == 0) return -1;
	}
	return 0;	
}

bool
gplayer_imp::UseSoulItem(int type, int level, int power)
{
	return _player_fatering.OnUseSoul(type,level,power);
}

void 
gplayer_imp::IncAntiInvisiblePassive(int val)
{
	int prev = ((gplayer*)_parent)->anti_invisible_degree;
	_anti_invisible_passive += val;	
	property_policy::UpdatePlayerInvisible(this);
	int cur = ((gplayer*)_parent)->anti_invisible_degree;
	if(cur > prev && prev != 0)		//prev=0��ʾ��ʱ������ڽ��н�������ĳ�ʼ��
	{
		_petman.NotifyInvisibleData(this);
		_runner->on_inc_anti_invisible(prev,cur);
	}
	__PRINTF("�����ȼ�%d\n",cur);
}

void 
gplayer_imp::DecAntiInvisiblePassive(int val)
{
	int prev = ((gplayer*)_parent)->anti_invisible_degree;
	_anti_invisible_passive -= val;
	property_policy::UpdatePlayerInvisible(this);
	int cur = ((gplayer*)_parent)->anti_invisible_degree;
	if(cur < prev)
	{
		_petman.NotifyInvisibleData(this);
		_runner->on_dec_anti_invisible(prev,cur);
	}
	__PRINTF("�����ȼ�%d\n",cur);
}

void 
gplayer_imp::IncAntiInvisibleActive(int val)
{
	int prev = ((gplayer*)_parent)->anti_invisible_degree;
	_anti_invisible_active += val;	
	property_policy::UpdatePlayerInvisible(this);
	int cur = ((gplayer*)_parent)->anti_invisible_degree;
	if(cur > prev)
	{
		_petman.NotifyInvisibleData(this);
		_runner->on_inc_anti_invisible(prev,cur);
	}
	__PRINTF("�����ȼ�%d\n",cur);
}

void 
gplayer_imp::DecAntiInvisibleActive(int val)
{
	int prev = ((gplayer*)_parent)->anti_invisible_degree;
	_anti_invisible_active -= val;
	property_policy::UpdatePlayerInvisible(this);
	int cur = ((gplayer*)_parent)->anti_invisible_degree;
	if(cur < prev)
	{
		_petman.NotifyInvisibleData(this);
		_runner->on_dec_anti_invisible(prev,cur);
	}
	__PRINTF("�����ȼ�%d\n",cur);
}

void 
gplayer_imp::IncInvisiblePassive(int val)
{
	_invisible_passive += val;		
	gplayer * player = (gplayer*)_parent;
	if(player->IsInvisible())
	{
		//����ǰ��������״̬���������ȼ�����
		int prev_invisible_degree = player->invisible_degree;
		property_policy::UpdatePlayerInvisible(this);
		_runner->on_inc_invisible(prev_invisible_degree,player->invisible_degree);
		__PRINTF("����%d��������\n",player->invisible_degree);
		_petman.NotifyInvisibleData(this);
	}
}

void 
gplayer_imp::DecInvisiblePassive(int val)
{
	_invisible_passive -= val;
	gplayer * player = (gplayer*)_parent;
	if(player->IsInvisible())
	{
		//����ǰ��������״̬���������ȼ��½�
		int prev_invisible_degree = player->invisible_degree;
		property_policy::UpdatePlayerInvisible(this);
		_runner->on_dec_invisible(prev_invisible_degree,player->invisible_degree);
		__PRINTF("����%d��������\n",player->invisible_degree);
		_petman.NotifyInvisibleData(this);
	}
}

void 
gplayer_imp::SetInvisible(int invisible_degree)
{
	if(GetPlayerLimit(PLAYER_LIMIT_NOINVISIBLE))//��ֹ����
	{
		return;
	}
	
	gplayer * player = (gplayer*)_parent;
	_invisible_active += invisible_degree;
	property_policy::UpdatePlayerInvisible(this);
	player->object_state |= gactive_object::STATE_INVISIBLE;	
	_runner->disappear_to_spec(player->invisible_degree);
	_runner->toggle_invisible(player->invisible_degree);
	ClearAggroToEnemy();
	_petman.NotifyInvisibleData(this);
	__PRINTF("����%d��������\n",player->invisible_degree);
}

void 
gplayer_imp::ClearInvisible()
{
	gplayer * player = (gplayer*)_parent;
	_runner->appear_to_spec(player->invisible_degree);
	_invisible_active = 0;
	property_policy::UpdatePlayerInvisible(this);
	player->object_state &= ~gactive_object::STATE_INVISIBLE;
	_runner->toggle_invisible(0);
	_petman.NotifyInvisibleData(this);
	__PRINTF("����������\n");
}

void 
gplayer_imp::SendDisappearToTeam()
{
	gplayer* player = (gplayer*)_parent;
	if(!IsInTeam() || !player->IsInvisible()) return;	
	
	packet_wrapper h1(16);
	using namespace S2C;
	CMD::Make<CMD::object_disappear>::From(h1,player);
	int count = _team.GetMemberNum();
	for(int i=0;i<count;i++)
	{
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.id.id != _parent->ID.id && !CanSeeMe(ent.id.id))
			send_ls_msg(ent.cs_index,ent.id.id,ent.cs_sid,h1.data(),h1.size());
	}
}

void 
gplayer_imp::SendDisappearToTeamMember(int id,int cs_index,int cs_sid)
{
	gplayer* player = (gplayer*)_parent;
	if(!IsInTeam() || !player->IsInvisible()) return;	
	
	packet_wrapper h1(16);
	using namespace S2C;
	CMD::Make<CMD::object_disappear>::From(h1,player);
	if(!CanSeeMe(id))
		send_ls_msg(cs_index,id,cs_sid,h1.data(),h1.size());	
}

void 
gplayer_imp::SendAppearToTeam()
{
	gplayer* player = (gplayer*)_parent;
	if(!IsInTeam() || !player->IsInvisible()) return;	
	if(player->gm_invisible) return;

	packet_wrapper h1(32);
	using namespace S2C;
	CMD::Make<CMD::player_enter_world>::From(h1,player);
	int count = _team.GetMemberNum();
	for(int i=0;i<count;i++)
	{
		const player_team::member_entry &ent = _team.GetMember(i);
		if(ent.id.id != _parent->ID.id && !CanSeeMe(ent.id.id))
			send_ls_msg(ent.cs_index,ent.id.id,ent.cs_sid,h1.data(),h1.size());
	}
}

void 
gplayer_imp::SendAppearToTeamMember(int id,int cs_index,int cs_sid)
{
	gplayer* player = (gplayer*)_parent;
	if(!IsInTeam() || !player->IsInvisible()) return;	
	if(player->gm_invisible) return;
	
	packet_wrapper h1(32);
	using namespace S2C;
	CMD::Make<CMD::player_enter_world>::From(h1,player);
	if(!CanSeeMe(id))
		send_ls_msg(cs_index,id,cs_sid,h1.data(),h1.size());	
}

bool 
gplayer_imp::CanSeeMe(int player_id)
{
	int index = _plane->FindPlayer(player_id);
	if(index < 0)
	{
		//���ڿ������ⲿ�����,Ϊ�����,��Ϊ�������Լ�
		return false;	
	}
	gplayer* pPlayer = _plane->GetPlayerByIndex(index);
	if(pPlayer->IsActived() && pPlayer->pos.horizontal_distance(_parent->pos) < 150*150
		&& ((gplayer*)_parent)->invisible_degree <= pPlayer->anti_invisible_degree )
		return true;	
	return false;
}

void gplayer_imp::UpdateMinAddonExpireDate(int addon_expire)  
{
	//���õ�ʱ��֤ addon_expire>0
	if(_min_addon_expire_date <= 0)
		_min_addon_expire_date = addon_expire;
	else if(_min_addon_expire_date > addon_expire)
		_min_addon_expire_date = addon_expire;
}

void gplayer_imp::SetGMInvisible()
{
	if(_runner->is_gm_invisible()) return;
	if(_parent->pPiece) _runner->leave_world();
	_runner->set_gm_invisible(true);
	((gplayer*)_parent)->gm_invisible = true;
	_runner->gm_toggle_invisible(0);	//0������
	_commander->DenyCmd(controller::CMD_ATTACK);
}

void gplayer_imp::ClearGMInvisible()
{
	if(!_runner->is_gm_invisible()) return;
	_runner->set_gm_invisible(false);
	((gplayer*)_parent)->gm_invisible = false;
	_runner->appear();
	_runner->gm_toggle_invisible(1);	//1�Ƿ�����
	_commander->AllowCmd(controller::CMD_ATTACK);
}
	
bool gplayer_imp::ActivateSharpener(int id, int equip_idx)
{
	DATA_TYPE dt;
	SHARPENER_ESSENCE * ess = (SHARPENER_ESSENCE *)world_manager::GetDataMan().get_data_ptr(id,ID_SPACE_ESSENCE,dt);
	if(dt != DT_SHARPENER_ESSENCE || ess == NULL) return false;

	if(equip_idx < 0 || equip_idx >= item::EQUIP_INVENTORY_COUNT || !((1ULL<<equip_idx)&ess->equip_mask)) return false;
	
	if(ess->addon_time <= 0) return false;
	int expire_time = time(NULL) + ess->addon_time;
	
	addon_data addon_list[3];
	unsigned int i = 0;
	for( ; i<3 && i<sizeof(ess->addon)/sizeof(int); i++)
	{
		if(ess->addon[i] <= 0) break;
		if(!world_manager::GetDataMan().generate_addon(ess->addon[i],addon_list[i])) return false; 
		if(addon_list[i].arg[1] != 0xFFFF) return false; //������ʱЧ��������	
		addon_list[i].arg[1] = expire_time;
	}
	if(i == 0) return false;
	
	return SharpenEquipment(equip_idx,addon_list,i,ess->level,ess->gfx_index);
}

bool
gplayer_imp::SummonPet2(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack)
{
	DATA_TYPE datatype;
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_egg_id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_PET_EGG_ESSENCE)	return false;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess->id_pet, ID_SPACE_ESSENCE, datatype);
	if(petess == NULL || datatype != DT_PET_ESSENCE)	return false;
	if(petess->id_type != 28752) return false;			//���Ʊ������ٻ���

	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = 500;								//�øж�����
	data.hunger_gauge = pet_data::HUNGER_LEVEL_1;		//����״̬����
	data.pet_tid = ess->id_pet;
	data.pet_vis_tid = 0;
	data.pet_egg_tid = ess->id;
	data.pet_class = pet_data::PET_CLASS_SUMMON;
	data.hp_factor = 0.5f;								//��Ѫ
	data.level = _basic.level;							//�ȼ�������ͬ
	data.color = 0;
	data.exp = 0;
	data.skill_point = 0;
	data.name_len = 0;
	int skill_count = 0;
	for(unsigned int i = 0; i < 32 && skill_count < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		if(ess->skills[i].id_skill <= 0 
			|| ess->skills[i].level <= 0 ) continue;
		data.skills[skill_count].skill = ess->skills[i].id_skill;
		data.skills[skill_count].level = skill_level;	//ess->skills[i].level;//���＼�ܵȼ�ͬ�ٻ����ܵȼ�
		++ skill_count;
	}

	return _petman.ActivePet2(this, data, life_time, skill_level, target, force_attack);
}

bool
gplayer_imp::SummonPlantPet(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack)
{
	DATA_TYPE datatype;
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_egg_id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_PET_EGG_ESSENCE)	return false;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess->id_pet, ID_SPACE_ESSENCE, datatype);
	if(petess == NULL || datatype != DT_PET_ESSENCE)	return false;
	if(petess->id_type != 28913) return false;			//���Ʊ�����ֲ��

	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = 500;								//�øж�����
	data.hunger_gauge = pet_data::HUNGER_LEVEL_1;		//����״̬����
	data.pet_tid = ess->id_pet;
	data.pet_vis_tid = 0;
	data.pet_egg_tid = ess->id;
	data.pet_class = pet_data::PET_CLASS_PLANT;
	data.hp_factor = 1.f;								//��Ѫ
	data.level = _basic.level;							//�ȼ�������ͬ
	data.color = 0;
	data.exp = 0;
	data.skill_point = 0;
	data.name_len = 0;
	int skill_count = 0;
	for(unsigned int i = 0; i < 32 && skill_count < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		if(ess->skills[i].id_skill <= 0 
			|| ess->skills[i].level <= 0 ) continue;
		data.skills[skill_count].skill = ess->skills[i].id_skill;
		data.skills[skill_count].level = skill_level;	//ess->skills[i].level;//���＼�ܵȼ�ͬ�ٻ����ܵȼ�
		++ skill_count;
	}

	return _plantpetman.ActivePlant(this, data, life_time, skill_level, target, force_attack);
}

bool 
gplayer_imp::CalcPetEnhance(int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour)
{
	pet_enhance enhance;
	enhance.hp_percent 				= _pet_enhance.hp_percent + 5*skill_level;
	enhance.mp_percent 				= _pet_enhance.mp_percent + 5*skill_level;
	enhance.damage_percent 			= _pet_enhance.damage_percent + 5*skill_level;
	enhance.magic_damage_percent 	= _pet_enhance.magic_damage_percent + 5*skill_level;
	enhance.defense_percent 		= _pet_enhance.defense_percent + 5*skill_level;
	enhance.magic_defense_percent 	= _pet_enhance.magic_defense_percent + 5*skill_level;
	enhance.attack_degree_percent 	= _pet_enhance.attack_degree_percent + 5*skill_level;
	enhance.defend_degree_percent 	= _pet_enhance.defend_degree_percent + 5*skill_level;

	prop.max_hp += (int)((_base_prop.max_hp + player_template::GetVitHP(GetObjectClass(),_en_point.vit) + _en_point.max_hp) * 0.01f * enhance.hp_percent);
	prop.max_mp += (int)((_base_prop.max_mp + player_template::GetEngMP(GetObjectClass(),_en_point.eng) + _en_point.max_mp) * 0.01f * enhance.mp_percent);
	//int en_damage = (int)((_cur_item.damage_high + _en_point.damage_high + _base_prop.damage_high) * 0.01f * enhance.damage_percent);
	int en_damage = (int)((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * enhance.damage_percent);
	prop.damage_low += en_damage; 
	prop.damage_high += en_damage;
	int en_magic_damage = (int)((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * enhance.magic_damage_percent);
	prop.damage_magic_low += en_magic_damage;
	prop.damage_magic_high += en_magic_damage;
	prop.defense += (int)((_base_prop.defense + _en_point.defense) * 0.01f * enhance.defense_percent);
	for(unsigned int i = 0; i < MAGIC_CLASS; i ++)
		prop.resistance[i] += (int)((_base_prop.resistance[i] + _en_point.resistance[i]) * 0.01f * enhance.magic_defense_percent);
	attack_degree += (int)(_attack_degree * 0.01f * enhance.attack_degree_percent);
	defend_degree += (int)(_defend_degree * 0.01f * enhance.defend_degree_percent);
	vigour = GetVigour();
	
	return true;	
}

bool 
gplayer_imp::PetSacrifice()
{
	return _petman.PetSacrifice(this);
}

bool 
gplayer_imp::IsPetActive()
{
	return _petman.IsPetActive();
}

bool 
gplayer_imp::PlantSuicide(float distance, const XID & target, char force_attack)
{
	return _plantpetman.PlantSuicide(this, distance, target, force_attack);
}

void 
gplayer_imp::InjectPetHPMP(int hp, int mp)
{
	XID pet_id = _petman.GetCurPet();
	if(pet_id.IsValid())
	{
		msg_hp_mp_t data;
		data.hp = hp;
		data.mp = mp;
		SendTo<0>(GM_MSG_INJECT_HP_MP, pet_id, 0, &data, sizeof(data));
	}
}

void 
gplayer_imp::DrainPetHPMP(const XID & pet_id, int hp, int mp)
{
	if(pet_id.IsValid())
	{
		msg_hp_mp_t data;
		data.hp = hp;
		data.mp = mp;
		SendTo<0>(GM_MSG_DRAIN_HP_MP, pet_id, 0, &data, sizeof(data));
	}
}

void 
gplayer_imp::LongJumpToSpouse()
{
	//һ���򵥷��޴��Ͱ汾,û��ʹ��msg
	if(world_manager::GetWorldTag() != 1) return;
	
	int spouse_id = ((gplayer*)_parent)->spouse_id;
	if(spouse_id == 0) return;
	world::object_info info;
	if(!_plane->QueryObject(XID(GM_TYPE_PLAYER,spouse_id),info))
	{
		_runner->error_message(S2C::ERR_SPOUSE_NOT_IN_SAME_SCENE);
		return;
	}
	LongJump(info.pos);	
}

void 
gplayer_imp::WeaponDisabled(bool disable)
{
	gplayer * pPlayer = (gplayer*)_parent;
	if(disable)
	{
		if(pPlayer->disabled_equip_mask & item::EQUIP_MASK64_WEAPON) return;
		pPlayer->disabled_equip_mask |= item::EQUIP_MASK64_WEAPON;
		pPlayer->object_state |= gactive_object::STATE_EQUIPDISABLED;
	}
	else
	{
		if(!(pPlayer->disabled_equip_mask & item::EQUIP_MASK64_WEAPON)) return;
		pPlayer->disabled_equip_mask &= ~item::EQUIP_MASK64_WEAPON;
		if(!pPlayer->disabled_equip_mask)
			pPlayer->object_state &= ~gactive_object::STATE_EQUIPDISABLED;
	}
	_runner->player_equip_disabled(pPlayer->disabled_equip_mask);

	_need_refresh_equipment = true;
}

void 
gplayer_imp::ElmoDisabled(bool disable)
{
	gplayer * pPlayer = (gplayer*)_parent;
	if(disable)
	{
		if(pPlayer->disabled_equip_mask & item::EQUIP_MASK64_HEAD) return;
		pPlayer->disabled_equip_mask |= item::EQUIP_MASK64_HEAD;
		pPlayer->object_state |= gactive_object::STATE_EQUIPDISABLED;
	}
	else
	{
		if(!(pPlayer->disabled_equip_mask & item::EQUIP_MASK64_HEAD)) return;
		pPlayer->disabled_equip_mask &= ~item::EQUIP_MASK64_HEAD;
		if(!pPlayer->disabled_equip_mask)
			pPlayer->object_state &= ~gactive_object::STATE_EQUIPDISABLED;
	}
	_runner->player_equip_disabled(pPlayer->disabled_equip_mask);

	_need_refresh_equipment = true;
}

namespace
{
	struct invisible_collector
	{
		world * _plane;
		abase::vector<gobject *,abase::fast_alloc<> > &_list;
		float _squared_radius;
		invisible_collector(world * plane,abase::vector<gobject *,abase::fast_alloc<> > &list,float radius)
			:_plane(plane),_list(list),_squared_radius(radius*radius){}

		inline void operator()(slice *pPiece,const A3DVECTOR & pos)
		{
			if(!pPiece->player_list) return;
			pPiece->Lock();
			gplayer * pPlayer = (gplayer *)pPiece->player_list;
			while(pPlayer)
			{
				if(pos.squared_distance(pPlayer->pos) < _squared_radius && pPlayer->invisible_degree > 0)
				{
					_list.push_back(pPlayer);
				}
				pPlayer = (gplayer *)pPlayer->pNext;
			}
			pPiece->Unlock();
		}
	};
};
void 
gplayer_imp::DetectInvisible(float range)
{
	abase::vector<gobject *,abase::fast_alloc<> > list;
	invisible_collector worker(_plane,list,range);
	_plane->ForEachSlice(_parent->pos,range,worker);
	
	_runner->invisible_obj_list(list.begin(), list.size());
}

void 
gplayer_imp::ForceSelectTarget(const XID & target)
{
	((gplayer_controller*)_commander)->SelectTarget(target.id);
}

void 
gplayer_imp::ExchangePosition(const XID & target)
{
	if(!target.IsPlayer() || target == _parent->ID) return;
	world::object_info info;
	if(!_plane->QueryObject(target, info)) return;
	if(info.state & world::QUERY_OBJECT_STATE_ZOMBIE) return;
	if(info.pos.squared_distance(_parent->pos) >= 29.f * 29.f) return;

	MSG msg;
	BuildMessage(msg,GM_MSG_EXCHANGE_POS,XID(-1,-1),_parent->ID,_parent->pos,0,&info.pos,sizeof(info.pos));
	XID list[2] = {_parent->ID, target};
	_plane->SendMessage(list, list + 2, msg);
}

void 
gplayer_imp::SetSkillAttackTransmit(const XID & target)
{
	if(_skill_attack_transmit_target != _parent->ID)
		_skill_attack_transmit_target = target;
}

void 
gplayer_imp::QueryOtherInventory(const XID& target)
{
	if(!target.IsPlayer() || target.id == _parent->ID.id) return;
	
	world::object_info info;
	if(!_plane->QueryObject(target,info)) return;
	if(_parent->pos.squared_distance(info.pos) > 30.f*30.f) return;
	
	msg_player_t data;
	data.id 		= _parent->ID.id;
	data.cs_index 	= ((gplayer*)_parent)->cs_index;
	data.cs_sid 	= ((gplayer*)_parent)->cs_sid;
	SendTo<0>(GM_MSG_QUERY_INVENTORY_DETAIL,target,0,&data,sizeof(data));
}

void 
gplayer_imp::SetPlayerLimit(int index, bool b)
{ 
	if(_player_limit.SetLimit(index, b))
		_runner->set_player_limit(index, b);
}

bool 
gplayer_imp::GetPlayerLimit(int index)
{ 
	return _player_limit.GetLimit(index);
}

void
gplayer_imp::EnterNonpenaltyPVPState(bool b)
{
	_nonpenalty_pvp_state = b;
	_runner->enter_nonpenalty_pvp_state(b ? 1 : 0);
}

void gplayer_imp::SendAllData(bool detail_inv, bool detail_equip, bool detail_task)
{
	PlayerGetProperty();
	if(detail_inv)
		PlayerGetInventoryDetail(IL_INVENTORY);
	else
		PlayerGetInventory(IL_INVENTORY);

	if(detail_equip)
		PlayerGetInventoryDetail(IL_EQUIPMENT);
	else
		PlayerGetInventory(IL_EQUIPMENT);

	if(detail_task)
		PlayerGetInventoryDetail(IL_TASK_INVENTORY);
	else
		PlayerGetInventory(IL_TASK_INVENTORY);
	//���������ֿ������
	PlayerGetPortableTrashBoxInfo(true);

	/*
	   {
	   time_t l_time;
	   time_t h_time;
	   int ptime;
	   _wallow_obj.GetTimeLeft(&l_time, &h_time,&ptime);
	   _runner->player_wallow_info(_wallow_level, ptime,l_time, h_time);
	   }*/

	SelfPlayerMoney();
	_runner->get_skill_data();
	_runner->trashbox_passwd_state(_trashbox.HasPassword());
	_runner->get_reputation(GetReputation());
	_runner->player_waypoint_list(_waypoint_list.begin(),_waypoint_list.size());
	_runner->pet_room_capacity(_petman.GetAvailPetSlot());
	_runner->player_pvp_mode(_basic.level);
	_petman.ClientGetPetRoom(this);
	_runner->send_cooldown_data();
	_runner->player_pvp_cooldown(_pvp_cooldown);
	_invade_ctrl.ClientGetPariahTime();
	_runner->player_cash(GetMallCash());
	_runner->player_dividend(_dividend_mall_info.GetDividend());
	_runner->send_timestamp(); 
	_runner->available_double_exp_time();
	_runner->enable_double_exp_time(_double_exp_mode, _double_exp_timeout);
	_multi_exp_ctrl.NotifyClientInfo(this);
	_multi_exp_ctrl.NotifyClientState(this);
	_runner->faction_contrib_notify();
	_runner->faction_relation_notify();
	_player_force.NotifyClient();
	_online_award.NotifyClientAllData(this);
	_runner->update_profit_time(S2C::CMD::player_profit_time::PLAYER_ONLINE, _profit_time, _profit_level);
	_runner->self_country_notify(GetCountryId());
	if(_filters.IsFilterExist(FILTER_DEFENSE_RUNE))
	{
		_runner->defense_rune_enabled(base_defense_rune_filter::RUNE_TYPE_DEFENSE,1);
	}
	if(_filters.IsFilterExist(FILTER_RESISTANCE_RUNE))
	{
		_runner->defense_rune_enabled(base_defense_rune_filter::RUNE_TYPE_RESISTANCE,1);
	}
	_meridianman.NotifyMeridianData(this);
	_player_dailysign.ClientSync(player_dailysign::SYNC4INIT);
	_player_fatering.NotifyClientRefresh();
	_runner->self_king_notify(GetParent()->IsKing() ? 1 : 0, GetKingExpireTime());
	world_manager::GetUniqueDataMan().OnRoleLogin((gplayer*)_parent);
		
	_player_reincarnation.ClientGetTome();
	_runner->realm_exp_receive(_realm_exp,0,_diary_exp);
	_runner->player_leadership(_leadership,0);
	unsigned int gc_size = 0;
	const void * gc_data = _generalcard_collection.data(gc_size);
	_runner->generalcard_collection_data(gc_data, gc_size);
 	_runner->astrolabe_info_notify(_astrolabe_extern_level,_astrolabe_extern_exp);
	_player_instance_reenter.NotifyClient(this);
	_runner->get_task_data(); //���ָ��ͻ����ж�Ϊ�������ݷ����ָʾ
	_solochallenge.NotifySoloChallengeData(this);
	_runner->fix_position_energy_info(1, _fix_position_transmit_energy);
	_runner->fix_position_all_info(_fix_position_transmit_infos);
	_runner->purchase_limit_all_info_notify();
	int vip_score = _cash_vip_info.GetCurScore();
	int vip_level = _cash_vip_info.GetCalcVipLevel();
	_runner->cash_vip_info_notify(vip_level,vip_score);
	_runner->get_server_time(g_timer.get_systime());

	GetLoteryInfo();
	GetLoteryItems();
	TreasureInfo();
	GetLibItems();

	
	/*170+*/
	if(_passwd_safe.passwd_active != 2)
	{
		ChangeSafeLockNew(true);		
        ChangeSafeLockActive(1);
	}	 

	gplayer_controller * pCtrl = (gplayer_controller *) _commander;
	pCtrl->UpdateSafeLock();	

	/*160+*/
	if(HasRepositoryLock()) 
	{ 
		ProtocolRepositoryStorage();
	}
	

	/*166+*/
	if (GetPlayerClass() == USER_CLASS_HAG)
	{
		PetEnterWorldSkinStorage();
	}

	/*172+*/
	ProtocolWorldPoints();	

	/*160+*/
	ProtocolRepositoryHistory();


	/*160+*/
	SpeedSkillManager();

	/*174+*/
	ActivityEventActivate();

	if(_activity.enable_skill)
	{
	ActivityEventSkillManager();
	}

	ProtocolActivityEvent();
	/*QuestionTask();

	CheckColorNameItemLogin();

	if(_basic.sec.flag == 0 && _basic.sec.level >= 20 && _basic.sec.level <= 32)
	{
		SetFlagForOldPlayer();
	}*/


	_real_weapon_class = _cur_item.weapon_class;
	//printf("REAL_WEAPON_CLASS = %d \n", _real_weapon_class);

	/*171+*/
	PlayerWeaponUpdateEnterWorld();
	
	if (EmulateSettings::GetInstance()->GetEnabledChild())
	{
		_kid_addon.ActivateKidsAddons(_parent->ID.id);
	}

}

unsigned int 
gplayer_imp::OI_GetInventorySize()
{
	return _inventory.Size();
}

unsigned int
gplayer_imp::OI_GetEmptySlotSize()
{
	return _inventory.GetEmptySlotCount();
}

int 
gplayer_imp::OI_GetInventoryDetail(GDB::itemdata * list, unsigned int size)
{
	return _inventory.GetDBData(list,size);
}

int 
gplayer_imp::OI_GetEquipmentDetail(GDB::itemdata * list, unsigned int size) 
{ 
	return _equipment.GetDBData(list,size);
}

unsigned int 
gplayer_imp::OI_GetEquipmentSize() 
{ 
	return _equipment.Size();
}

int 
gplayer_imp::TradeLockPlayer(int get_mask, int put_mask)
{
	//��������Ĭ��tradeidΪ0 
	return StartFactionTrade(0,get_mask,put_mask,true)?0:-1;
}

int 
gplayer_imp::TradeUnLockPlayer()
{
	if(_player_state != PLAYER_WAITING_FACTION_TRADE) return -1;
	FromFactionTradeToNormal();
	return 0;
}

void 
gplayer_imp::LeaveDoubleExpMode()
{
	_double_exp_mode = 0;
	_double_exp_timeout = 0;
	_runner->enable_double_exp_time(0, 0);
}

int
gplayer_imp::EnterDoubleExpMode(int time_mode, int timeout)
{
	int t = g_timer.get_systime();
	if(_double_exp_mode)
	{
		_double_exp_mode = time_mode;
		_double_exp_timeout += timeout;
		if(_double_exp_timeout - t > MAX_DOUBLE_EXP_TIME)
		{
			_double_exp_timeout = t + MAX_DOUBLE_EXP_TIME;
		}
	}
	else
	{
		_double_exp_mode = time_mode;
		_double_exp_timeout = t + timeout;
	}
	_runner->enable_double_exp_time(_double_exp_mode, _double_exp_timeout);
	return _double_exp_timeout - t;
}

bool
gplayer_imp::ActiveDoubleExpTime(int time)
{
	int cur_t = g_timer.get_systime();
	if(_double_exp_mode)
	{
		//���������˫��ʱ����ʧ��
		if(_double_exp_timeout + time > cur_t + MAX_DOUBLE_EXP_TIME)
		{
			return false;
		}
	}

	//�����Ѿ����ٸ���rest time�� ��Ϊ����ʹ����
	
	int t = EnterDoubleExpMode(1,time);
	GLog::log(GLOG_INFO,"�û�%d����˫������ʱ��%d��,˫��ʱ���ά��%dСʱ%02d��%02d��,ʣ�����˫��ʱ��%d�룬���а���˫������ʱ��%d��",
			_parent->ID.id,time,t/3600, (t/60)%60,t%60,GetDoubleExpAvailTime(),_mafia_rest_time);
	_runner->available_double_exp_time();
	return true;
}

void 
gplayer_imp::PlayerDuelRequest(const XID & target)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return;
	}
	if(((gplayer*)_parent)->IsInvisible())
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_INVISIBLE);
		return;
	}
		
	_duel.PlayerDuelRequest(this,target);
}

void 
gplayer_imp::PlayerDuelReply(const XID & target,int param)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return;
	}
	_duel.PlayerDuelReply(this,target,param);
}

bool
gplayer_imp::TestCreatePet(unsigned int pet_id)
{
	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = 100;
	data.pet_tid = pet_id;
	data.pet_class = 0;
	data.level = 10;
	data.color = 0;

	int index = _petman.AddPetData(data, false);
	if(index >= 0)
	{
		_runner->gain_pet(index, &data,sizeof(data));
	}

/*
	pet_data * pData = _petman.GetPetData(index);
	if(0 && !pData) return false;
	
	object_interface oi(this);
	object_interface::pet_param pp = {3884,4080,0.3f,0,0};
	pp.use_pet_name = 1;
	unsigned int name_size = _username_len;
	if(name_size > 12) name_size = 12;
	memcpy(pp.pet_name,_username,name_size);
	pp.pet_name[name_size+0] = 0x84;
	pp.pet_name[name_size+1] = 0x76;
	pp.pet_name[name_size+2] = 0x0F;
	pp.pet_name[name_size+3] = 0x5C;
	pp.pet_name[name_size+4] = 0xD7;
	pp.pet_name[name_size+5] = 0x72;
	pp.pet_name_size = name_size + 6;
	XID  who;
	bool bRst = oi.CreatePet(_parent->pos,pp, who);

//	if(bRst) _petman.ActivePet(index,who);
	return true;
	*/
	return true;
}

bool
gplayer_imp::SummonPet(unsigned int index)
{
	return _petman.ActivePet(this,index);
}

bool
gplayer_imp::RecallPet()
{
	return _petman.RecallPet(this);
}

void 
gplayer_imp::SendPetCommand(int cur_target,int pet_cmd, const void * buf, unsigned int size)
{
	_petman.PlayerPetCtrl(this, cur_target, pet_cmd, buf, size);
}

void 
gplayer_imp::Die(const XID & attacker, bool is_pariah, char attacker_mode, int taskdead)
{
	//filter��������ǰ�Ĵ���
	_filters.EF_BeforeDeath(attacker,attacker_mode);
	//����filter��ʹ��������������	
	if(_basic.hp > 0 && taskdead == 0) return;
	
	//����Ƿ��������������
	if(attacker_mode & attack_msg::PVP_DUEL)
	{
		//�����������������������⴦��
		_basic.hp = 1;
		_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);

		//���ÿ��ܵľ�������
		_duel.OnDeath(this, true);
		return ;
	}
	_duel.OnDeath(this, false);

	//�����Ҫ�ı�־
	_idle_mode_flag = 0;
	_seal_mode_flag = 0;

	//ȥ������ʱӦ��ȥ����filter
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);

	//lgc ȥ��С���龫������Ч��,��vigor
	if(_cur_elf_info.id != (unsigned int)-1)
	{
		_cur_elf_info.vigor = 0;		
		_cur_elf_info.refresh_vigor = true;
		if(_cur_elf_info.refine_effect_active)
		{
			ElfRefineDeactivate(_cur_elf_info.refine_level);
			_runner->elf_refine_activate(0);
		}
	}		

	//�����������������������д���
	OnDeath(attacker, is_pariah, attacker_mode, taskdead);

	//
	slice * pPiece = _parent->pPiece;
	if(pPiece && pPiece->IsBorder())
	{
		extern_object_manager::SendRefreshMsg<0>(_plane,_parent,0,pPiece);
	}
	
}

bool 
gplayer_imp::CheckPlayerBindRequest()
{
	//����Ƿ��ܹ���������
	//������,ֻ����normal״̬
	//û�е�ǰ����
	if(_player_state != PLAYER_STATE_NORMAL) return false;
	if(_cur_session || HasNextSession()) return false;

	//ˮ�²��ܽ��д˲���
	if(IsUnderWater()) return false;
	
	//����״̬�²��ܷ�������
	if(((gplayer*)_parent)->IsInvisible()) return false;
	if(GetPlayerLimit(PLAYER_LIMIT_NOBIND)) return false;

	return true;
}

bool 
gplayer_imp::CheckPlayerBindInvite()
{
	//����Ƿ��ܹ���������
	//������,ֻ����normal״̬
	if(_player_state != PLAYER_STATE_NORMAL) return false;
	if(_cur_session || HasNextSession()) return false;

	//ˮ�²��ܽ��д˲���
	if(IsUnderWater()) return false;
	
	//����״̬�²��ܷ�������
	if(((gplayer*)_parent)->IsInvisible()) return false;
	if(GetPlayerLimit(PLAYER_LIMIT_NOBIND)) return false;

	return true;
	
}

bool
gplayer_imp::EnterBindMoveState(const XID & target)
{
	if(_player_state != PLAYER_STATE_NORMAL) return false;

	_player_state = PLAYER_STATE_BIND;

	gplayer *pParent = GetParent();
	pParent->bind_type = 1;
	pParent->bind_target = target.id;
	pParent->object_state |= gactive_object::STATE_IN_BIND;
	return true;
}

bool
gplayer_imp::EnterBindFollowState(const XID & target)
{
	if(_player_state != PLAYER_STATE_NORMAL) return false;
	//���������ƶ�״̬
	//���״̬���ı�player_state
	_commander->DenyCmd(controller::CMD_MOVE);
	_commander->DenyCmd(controller::CMD_PET);
	_player_state = PLAYER_STATE_BIND;

	//ȥ�����з���״̬
	_filters.RemoveFilter(FILTER_FLY_EFFECT);
	//ȥ�����г���״̬
	_petman.RecallPet(this);
	
	//��ֹ����
	_bind_to_ground ++;

	//�����ǰsession
	ClearSession();
	ClearAction();

	gplayer *pParent = GetParent();
	pParent->bind_type = 2;
	pParent->bind_target = target.id;
	pParent->object_state |= gactive_object::STATE_IN_BIND;
	return true;
}

void 
gplayer_imp::ReturnBindNormalState()
{
	if(_player_state != PLAYER_STATE_BIND)
	{
		return;
	}
	_player_state = PLAYER_STATE_NORMAL;
	gplayer *pParent = GetParent();
	if(pParent->bind_type == 2)
	{
		_commander->AllowCmd(controller::CMD_MOVE);
		_commander->AllowCmd(controller::CMD_PET);
		_bind_to_ground --;

		//����ǳ˿�
		//�����������Ҫͬ��һ������λ��
		int seq = _commander->GetCurMoveSeq();
		seq = (seq + 100) & 0xFFFF;
		_commander->SetNextMoveSeq(seq);
		_runner->trace_cur_pos(seq);
	}

	pParent->bind_type = 0;
	pParent->bind_target = 0;
	pParent->object_state &= ~gactive_object::STATE_IN_BIND;
}

void 
gplayer_imp::PlayerBindRequest(const XID & target, int param_id)
{
	_bind_player.PlayerLinkRequest(this,target, param_id);
}

void 
gplayer_imp::PlayerBindInvite(const XID & target, int param_id)
{
	_bind_player.PlayerLinkInvite(this,target, param_id);
}

void 
gplayer_imp::PlayerBindRequestReply(const XID & target, int param, int param_id)
{
	_bind_player.PlayerLinkReqReply(this,target,param, param_id);
}

void 
gplayer_imp::PlayerBindInviteReply(const XID & target, int param, int param_id)
{
	_bind_player.PlayerLinkInvReply(this,target,param, param_id);
}

void 
gplayer_imp::PlayerBindCancel()
{
	if(_player_state != PLAYER_STATE_BIND)
	{
		//��Ӧ���ǲ����ܵ�
		ASSERT(false);
		return;
	}
	_bind_player.PlayerCancel(this);
}

void 
gplayer_imp::ActiveMountState(int mount_id, unsigned short mount_color)
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsMountMode();
	if(is_mount) return;
	//������������
	_commander->DenyCmd(controller::CMD_ATTACK);
	_commander->DenyCmd(controller::CMD_MARKET);
	
	//��ֹ����
	_bind_to_ground ++;

	//����״̬�л� 
	pPlayer->mount_color = mount_color;
	pPlayer->mount_id  = mount_id;
	pPlayer->object_state |= gactive_object::STATE_MOUNT;

	//��������
	_runner->player_mounting(mount_id, mount_color);
}

void 
gplayer_imp::DeactiveMountState()
{
	gplayer * pPlayer = GetParent();
	bool is_mount = pPlayer->IsMountMode();
	if(!is_mount) return;

	//�ó���������ٻس���
	_petman.RecallPet(this);

	_bind_to_ground --;
	_commander->AllowCmd(controller::CMD_ATTACK);
	_commander->AllowCmd(controller::CMD_MARKET);

	//����״̬�л� 
	pPlayer->object_state &= ~gactive_object::STATE_MOUNT;
	
	//��������
	_runner->player_mounting(0,0);
}


void 
gplayer_imp::TestUnderWater()
{
	float y = path_finding::GetWaterHeight(_plane, _parent->pos.x,_parent->pos.z);
/*	if(y - WATER_BREATH_MARK <  _parent->pos.y)
	{
		_breath.ChangeState(this,false);
		_breath.SetUnderWater(y > _parent->pos.y);
	}
	else
	{
		_breath.ChangeState(this,true);
		_breath.SetUnderWater(true);
	}*/

//������Զ��������ˮ��
	float off = y - _parent->pos.y;
	if(off > 0.5f)
	{
		_breath.SetUnderWater(true,off);
		_bind_player.OnUnderWater(this,off);
		_petman.OnUnderWater(this,off);
	}
	else
	{
		_breath.SetUnderWater(false,0.0f);
	}

}

void
gplayer_imp::UpdatePlayerLayer()
{
	if(IsUnderWater())
		_layer_ctrl.UpdateLayer(LAYER_WATER);
	else if(GetParent()->IsFlyMode())
		_layer_ctrl.UpdateLayer(LAYER_AIR);
	else
		_layer_ctrl.UpdateLayer(LAYER_GROUND);
}

int 
gplayer_imp::ConvertPetDataToEggData(void * data, unsigned int size, pet_data * pData, const void * src_temp )
{
	//���ȼ������ݵĴ�С�Ƿ��㹻
	unsigned int nsize = sizeof(pe_essence);
	unsigned int skill_count = 0;
	for(unsigned int i = 0; i < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		if(pData->skills[i].skill)
		{
			nsize += sizeof(int)*2;
			skill_count++;
		}
		else
		{
			break;
		}
	}
	if(pData->pet_class == pet_data::PET_CLASS_EVOLUTION)
	{
		nsize += sizeof(pData->evo_prop);
	}
	if(nsize > size) return -1;

	//��С�㹻���������ݵ���֯
	pe_essence * pess = (pe_essence*)data;
	pe_essence * sess = (pe_essence*)src_temp;
	
	//����Ҫ��ģ��������ݸ��ƹ���һ�ݣ�Ȼ���ٸ���pet_data���л�ԭ
	*pess = *sess;
	pess->honor_point 	= 0;
	pess->pet_tid 		= pData->pet_tid;
	pess->pet_vis_tid 	= pData->pet_vis_tid;
	pess->level		= pData->level;
	pess->color		= pData->color;
	pess->exp		= pData->exp;
	pess->skill_point	= pData->skill_point;
	pess->name_len 		= pData->name_len;
	if(pess->name_len)
	{
		memcpy(pess->name,pData->name,sizeof(pess->name));
	}
	pess->skill_count = skill_count;
	for(unsigned int i = 0; i < skill_count ; i ++)
	{
		pess->skills[i].skill = pData->skills[i].skill;
		pess->skills[i].level = pData->skills[i].level;
	}
	if(pData->pet_class == pet_data::PET_CLASS_EVOLUTION)
	{
		memcpy((char*)data+sizeof(*pess)+sizeof(int)*2*skill_count,&(pData->evo_prop),sizeof(pData->evo_prop));
	}

	//ת����� ����ת����Ĵ�С
	return nsize;
}

bool 
gplayer_imp::AddPetToSlot(void * arg, int inv_index)
{
	item & it = _inventory[inv_index];
	ASSERT(it.type != -1);
	if(!CheckProctypeCodexCondition(inv_index))
	{
		_runner->error_message(S2C::ERR_CODEX_NOT_FOUND_IN_CODEX);
		return 0;
	}
	pe_essence * ess = (pe_essence*)arg;
	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = ess->honor_point;
	data.hunger_gauge = pet_data::HUNGER_LEVEL_1;		//���ﵰ��ԭ������������״̬�ĳ���
	data.pet_tid = ess->pet_tid;
	data.pet_vis_tid = ess->pet_vis_tid;
	data.pet_egg_tid = ess->pet_egg_tid;
	data.pet_class = ess->pet_class;
	data.hp_factor = 1.0f;
	data.level = ess->level;
	data.color = ess->color;
	data.exp = ess->exp;
	data.skill_point = ess->skill_point;
	if(it.proc_type & item::ITEM_PROC_TYPE_BIND) data.is_bind |= 0x01; 
	if(it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE) data.is_bind |= 0x02;
	data.name_len = ess->name_len;
	if(data.name_len)
	{
		memcpy(data.name,ess->name,sizeof(data.name));
	}
	unsigned int skill_count = ess->skill_count;
	for(unsigned int i = 0; i < skill_count && i < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		data.skills[i].skill = ess->skills[i].skill;
		data.skills[i].level = ess->skills[i].level;
	}

	if(ess->pet_class == pet_data::PET_CLASS_EVOLUTION)
	{
		memcpy((char *)&(data.evo_prop),(char*)ess+sizeof(*ess)+sizeof(int)*2*skill_count,sizeof(data.evo_prop));
	}
	if(data.pet_class == pet_data::PET_CLASS_SUMMON 
			|| data.pet_class == pet_data::PET_CLASS_PLANT) return false;	//�ٻ����ֲ�ﲻ�������ٻ���
	int index = _petman.AddPetData(data, false);

	if(index >= 0)
	{
		_runner->gain_pet(index, &data,sizeof(data));
		return true;
	}
	else
	{
		return false;
	}
}

bool
gplayer_imp::BanishPet(unsigned int index)
{
	return _petman.BanishPet(this,index);
}

int
gplayer_imp::PlayerSummonPet(unsigned int index)
{
	if(LuaManager::GetInstance()->GetConfig()->SummonPet136 && _petman.IsPetActive()) return S2C::ERR_PET_IS_ALEARY_ACTIVE;
	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;

	//����У��ͨ��������һ��session 
	session_summon_pet * pSession = new session_summon_pet(this);
	int tid = pData->pet_vis_tid;
	if(!tid) tid = pData->pet_tid;
	pSession->SetTarget(tid,index);
	pSession->SetDelay(60);
	AddSession(pSession);
	StartSession();
	return 0;
}

int
gplayer_imp::PlayerRecallPet()
{
	if(!_petman.IsPetActive()) return S2C::ERR_PET_IS_NOT_ACTIVE;
	unsigned int index = _petman.GetCurActivePet();
	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;


	//����У��ͨ��������һ��session 
	session_recall_pet * pSession = new session_recall_pet(this);
	int tid = pData->pet_vis_tid;
	if(!tid) tid = pData->pet_tid;
	pSession->SetTarget(tid,index);
	pSession->SetDelay(10);
	AddSession(pSession);
	StartSession();
	return 0;
}

int
gplayer_imp::ServiceConvertPetToEgg(unsigned int slot_index)
{
	if(_inventory.GetEmptySlotCount() == 0) return S2C::ERR_INVENTORY_IS_FULL;

	//�����Ƿ����
	pet_data * pData = _petman.GetPetData(slot_index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;

	if(_petman.GetCurActivePet() == (int)slot_index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;

	//����У��ͨ��������һ��session 
	session_restore_pet * pSession = new session_restore_pet(this);
	int tid = pData->pet_vis_tid;
	if(!tid) tid = pData->pet_tid;
	pSession->SetTarget(tid,slot_index);
	pSession->SetDelay(10*20);
	AddSession(pSession);
	StartSession();
	return 0;
}

int
gplayer_imp::PlayerBanishPet(unsigned int index)
{
	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;
	if(_petman.IsPetActive() && _petman.GetCurActivePet() == (int)index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;

	//����У��ͨ��������һ��session 
	session_free_pet * pSession = new session_free_pet(this);
	int tid = pData->pet_vis_tid;
	if(!tid) tid = pData->pet_tid;
	pSession->SetTarget(tid,index);
	pSession->SetDelay(200);
	AddSession(pSession);
	StartSession();
	return 0;
}

void 
gplayer_imp::SetPetSlotCapacity(unsigned int new_size, bool notify)
{
	_petman.SetAvailPetSlot(new_size);
	if(notify) _runner->pet_room_capacity(_petman.GetAvailPetSlot());
}

unsigned int
gplayer_imp::GetPetSlotCapacity()
{
	return _petman.GetAvailPetSlot();
}

pet_data * 
gplayer_imp::GetPetData(unsigned int index)
{
	return _petman.GetPetData(index);
}

bool 
gplayer_imp::FeedPet(int food_mask, int honor)
{
	return _petman.FeedCurPet(this, food_mask ,honor);
}

int
gplayer_imp::ConvertPetToEgg(unsigned int slot_index)
{
	//�����ﻹԭ�ɳ��ﵰ�Ĳ���
	//���л������
	//��������Ҫ�п�λ
	if(_inventory.GetEmptySlotCount() == 0) return S2C::ERR_INVENTORY_IS_FULL;

	//�����Ƿ����
	if(slot_index >= pet_manager::MAX_PET_CAPACITY) return S2C::ERR_PET_CAN_NOT_BE_RESTORED;
	pet_data * pData = _petman.GetPetData(slot_index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;

	//�Ѿ��ų��ĳ��ﲻ�ܻ�ԭ
	if(_petman.GetCurActivePet() == (int)slot_index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;

	//ȡ��ģ������ ����֤֮
	DATA_TYPE dt;
	PET_EGG_ESSENCE * ess;
	ess = (PET_EGG_ESSENCE*) world_manager::GetDataMan().get_data_ptr(pData->pet_egg_tid, ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_PET_EGG_ESSENCE) return S2C::ERR_PET_CAN_NOT_BE_RESTORED;

	//����Ǯ�Ƿ��㹻
	if(GetAllMoney() < (unsigned int)ess->money_restored) return S2C::ERR_OUT_OF_FUND;

	//������֤ͨ��

	//�ֶ����ɺ��ʵ����ԣ��ֶ������Դ��۳�ģ������
	const item_data * idata=(const item_data*)world_manager::GetDataMan().get_item_for_sell(pData->pet_egg_tid);
	if(!idata) return S2C::ERR_SERVICE_UNAVILABLE;

	item_data data = *idata;
	char content[sizeof(pe_essence) + sizeof(int)*22];

	//����ת������
	int rst = ConvertPetDataToEggData(content, sizeof(content), pData, idata->item_content);
	if(rst <= 0) return S2C::ERR_SERVICE_UNAVILABLE;

	data.item_content = content;
	data.content_length = rst;
	data.count = 1;
	if(data.proc_type & item::ITEM_PROC_TYPE_BIND2)		//�����item_dataman ��������proc_type����װ����󶨣��������̰�
	{
		//��������������а�
		data.proc_type |= item::ITEM_PROC_TYPE_NODROP |
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND;

		data.proc_type &= ~(item::ITEM_PROC_TYPE_BIND2);

		UpdateMallConsumptionBinding(data.type, data.proc_type, data.count);
	}
	else
	{
		if(pData->is_bind & 0x01)
		{
			data.proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		}
		if(pData->is_bind & 0x02)
		{
			data.proc_type |= item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
	}

	//����Ʒ�ָ�����������
	rst = _inventory.Push(data);
	if(rst < 0) return S2C::ERR_SERVICE_UNAVILABLE;		//���������ʧ��
	ASSERT(data.count == 0);
	_runner->obtain_item(data.type,data.expire_date,1 ,_inventory[rst].count, 0,rst);

	//���ٽ�Ǯ
	SpendAllMoney(ess->money_restored,true);
	SelfPlayerMoney();

	//ɾ��һ������
	_petman.FreePet(this,slot_index);

	GLog::log(GLOG_INFO,"�û�%d��ԭ�˳��ﵰ%d",_parent->ID.id,data.type);
	//���سɹ�
	return 0;
}

bool 
gplayer_imp::OI_IsMafiaMember()
{
	return GetParent()->id_mafia;
}

int
gplayer_imp::OI_GetMafiaID()
{
	return GetParent()->id_mafia;
}

char
gplayer_imp::OI_GetMafiaRank()
{
	return GetParent()->rank_mafia;
}

bool 
gplayer_imp::OI_IsMafiaMaster()
{
	return GetParent()->rank_mafia == 2;
}

bool 
gplayer_imp::OI_IsFactionAlliance(int fid)
{
	return _faction_alliance.find(fid) != _faction_alliance.end();
}

bool 
gplayer_imp::OI_IsFactionHostile(int fid)
{
	return _faction_hostile.find(fid) != _faction_hostile.end();
}

bool 
gplayer_imp::CheckGMPrivilege()
{
	return ((gplayer_controller*)_commander)->HasGMPrivilege();
}

void 
gplayer_imp::DBSetPetData(unsigned int index, const void * data, unsigned int size, bool codex)
{
	_petman.DBSetPetData(index,data,size,codex);
}


int 
gplayer_imp::CheckItemPrice(int inv_index, int item_id)
{
	if(IsItemExist(inv_index,item_id,1))
	{
		return _inventory[inv_index].price;
	}
	else
	{
		return 0;
	}
}

void 
gplayer_imp::DropSpecItem(bool isProtected, const XID & owner)
{
	bool bCanDrop = _player_state == PLAYER_STATE_NORMAL ||
			_player_state == PLAYER_DISCONNECT ||
			_player_state == PLAYER_SIT_DOWN ||
			_player_state == PLAYER_STATE_BIND;
	if(!bCanDrop) return;
	if(_free_pvp_mode) return;
	if(_basic.level <= LOW_PROTECT_LEVEL) return;
	if(_nonpenalty_pvp_state || world_manager::GetWorldFlag().nonpenalty_pvp_flag) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID2) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID3) >= 0) return;
	if(!_pvp_enable_flag) return;
	enum 
	{
		NO_DROP_TYPE = item::ITEM_PROC_TYPE_NODROP | item::ITEM_PROC_TYPE_NOTHROW | item::ITEM_PROC_TYPE_NOTRADE |item::ITEM_PROC_TYPE_BIND
	};
	for(unsigned int i=0; i<_inventory.Size(); i++)
	{
		if(_inventory[i].type == -1) continue;
		item & it = _inventory[i];
		if(it.proc_type & NO_DROP_TYPE) continue;
		if(!world_manager::IsDeathDropItem(it.type)) continue;
		ThrowInvItem(i,it.count,isProtected,S2C::DROP_TYPE_DEATH,owner);
	}
}

unsigned int 
gplayer_imp::OI_GetTrashBoxCapacity(int where)
{
	
    switch (where)
    {
    case IL_TRASH_BOX : return _trashbox.GetTrashBoxSize(0); break;
	case IL_TRASH_BOX2: return _trashbox.GetTrashBoxSize(1); break;
	case IL_TRASH_BOX3: return _trashbox.GetTrashBoxSize(2); break;
	case IL_TRASH_BOX4: return _trashbox.GetTrashBoxSize(3); break;
	case IL_TRASH_BOX5: return _trashbox.GetTrashBoxSize(4); break;
	case IL_TRASH_BOX6: return _trashbox.GetTrashBoxSize(5); break;
	case IL_TRASH_BOX7: return _trashbox.GetTrashBoxSize(6); break;
	case IL_TRASH_BOX8: return _trashbox.GetTrashBoxSize(7); break;
    default: break;
    }
	return 0;
}

int 
gplayer_imp::OI_GetTrashBoxDetail(int where, GDB::itemdata * list, unsigned int size)
{ 
	return GetTrashInventory(where).GetDBData(list,size);
}

bool 
gplayer_imp::OI_IsTrashBoxModified()
{
	return IsTrashBoxChanged();
}

bool 
gplayer_imp::OI_IsEquipmentModified()
{
	return _eq_change_counter;
}

unsigned int
gplayer_imp::OI_GetTrashBoxMoney()
{
	return _trashbox.GetMoney();
}

void 
gplayer_imp::EnterBattleground(int target_tag, int battle_id)
{
	if(world_manager::GetWorldTag() != 1) return;	//ֻ׼�Ӵ��������

	if(_player_state != PLAYER_STATE_NORMAL)
	{
		return;
	}

	if(target_tag == 1) return ;
	
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	key.target.key_level4 = battle_id;

	A3DVECTOR pos(0,0,0);
	//��Player���и������� 
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,target_tag,key,0) < 0)
	{
		return ;
	}
	return ;
}

void 
gplayer_imp::EnterFactionFortress(int target_tag, int dst_factionid)
{
	if(world_manager::GetWorldTag() != 1) return;	//ֻ׼�Ӵ��������

	if(_player_state != PLAYER_STATE_NORMAL)
	{
		return;
	}

	if(target_tag == 1) return ;
	
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	key.target.key_level3 = dst_factionid;

	A3DVECTOR pos(0,0,0);
	//��Player���и������� 
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,target_tag,key,0) < 0)
	{
		return ;
	}
	return ;
}

void 
gplayer_imp::EnterCountryBattle(int target_tag, int battle_id)
{
	if(!world_manager::GetInstance()->IsCountryTerritoryWorld()) return;		//�޶�����ӹ�ս�׶���������

	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_STATE_BIND
			&& _player_state != PLAYER_SIT_DOWN
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}
	LeaveAbnormalState();

	if(target_tag == world_manager::GetWorldTag()) return ;
	
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	key.target.key_level4 = battle_id;

	A3DVECTOR pos(0,0,0);
	//��Player���и������� 
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,target_tag,key,0) < 0)
	{
		return ;
	}
	return ;
}

int
gplayer_imp::RegroupPropPoint(int str, int agi, int vit,int eng)
{
	int t = _basic.status_point + _base_prop.vitality + _base_prop.energy + _base_prop.strength + _base_prop.agility;
	int offset = player_template::Rollback(GetPlayerClass(),_base_prop, str,agi,vit,eng);
	_basic.status_point +=  offset;
	int t2 = _basic.status_point + _base_prop.vitality + _base_prop.energy + _base_prop.strength + _base_prop.agility;
	if(t != t2)
	{
		ASSERT(false && "ϴ�����������");
	}
	if(offset == 0) return 0;

	//ʹ��װ����Ч
	RefreshEquipment();
	GlyphEquipRefresh();
	//����װ�������ݣ������˿���
	CalcEquipmentInfo();

	//���Լ���Ϣ
	PlayerGetProperty();

	return offset;
}


int 
gplayer_imp::OI_GetDBTimeStamp()
{
	return _db_timestamp;
}

int 
gplayer_imp::OI_InceaseDBTimeStamp()
{
	_db_timestamp ++;
	return _db_timestamp;
}

void 
gplayer_imp::LeaveAbnormalState()
{
	switch(_player_state)
	{
		case PLAYER_STATE_NORMAL:
		break;
		case PLAYER_STATE_BIND:
		_bind_player.PlayerCancel(this);
		break;
		case PLAYER_SIT_DOWN:
		StandUp();
		break;

		case PLAYER_TRADE:
			GMSV::DiscardTrade(_trade_obj->GetTradeID(),_parent->ID.id);
			//�ص��ȴ�������ɵ�״̬
			_player_state = PLAYER_WAIT_TRADE_COMPLETE;
			_trade_obj->SetTimeOut(10);
		break;

		case PLAYER_WAITING_TRADE:
		case PLAYER_WAIT_TRADE_COMPLETE:
		case PLAYER_WAIT_TRADE_READ:
		case PLAYER_WAITING_FACTION_TRADE:
		case PLAYER_WAIT_LOGOUT:
		case PLAYER_DISCONNECT:
		case PLAYER_WAIT_SWITCH:
		case PLAYER_WAIT_FACTION_TRADE_READ:
		case PLAYER_STATE_TRAVEL:	//���״̬�Ѿ�����
		//do nothing
		break;
		
		case PLAYER_STATE_MARKET:
		CancelPersonalMarket();
		break;

		case PLAYER_STATE_COSMETIC:
		//������δ����� Ŀǰ���Ժ���
		break;
	}
}


bool 
gplayer_imp::CheckWaypoint(int point_index, int & point_domain)
{
	short wp = point_index & 0xFFFF;
	if(!IsWaypointActived(wp)) return false;
	abase::vector<TRANS_TARGET_SERV> & waypoint_list = globaldata_gettranstargetsserver();
	for(unsigned int j = 0; j < waypoint_list.size(); j ++)
	{
		if(waypoint_list[j].id == point_index)
		{
			point_domain = waypoint_list[j].domain_id;
			return true;
		}
	}
	return false;
}

bool
gplayer_imp::ReturnWaypoint(int point)
{
	abase::vector<TRANS_TARGET_SERV> & waypoint_list = globaldata_gettranstargetsserver();
	for(unsigned int j = 0; j < waypoint_list.size(); j ++)
	{
		if(waypoint_list[j].id == point)
		{
			A3DVECTOR pos(waypoint_list[j].vecPos.x, waypoint_list[j].vecPos.y, waypoint_list[j].vecPos.z);
			return LongJump(pos, waypoint_list[j].world_tag, 0);
		}
	}
	return false;
}

int gplayer_imp::CheckUseTurretScroll()
{
	if(((gplayer*)_parent)->IsInvisible()) return S2C::ERR_OPERTION_DENYED_IN_INVISIBLE;
	return 0;
}

	
gactive_imp::attack_judge
gplayer_imp::GetPetAttackHook()
{
	return gplayer_imp::__GetPetAttackHook;
}

gactive_imp::enchant_judge 
gplayer_imp::GetPetEnchantHook()
{
	return gplayer_imp::__GetPetEnchantHook;
}

gactive_imp::attack_fill 
gplayer_imp::GetPetAttackFill()
{
	return __GetPetAttackFill;
}

gactive_imp::enchant_fill 
gplayer_imp::GetPetEnchantFill()
{
	return __GetPetEnchantFill;
}

template <typename MESSAGE,typename EFFECT_MSG>
inline static  bool PetTestHarmfulEffect(gactive_imp * __this, const MESSAGE & msg, EFFECT_MSG & emsg)
{
	if(emsg.attacker_mode & attack_msg::PVP_DUEL)
	{
		emsg.is_invader = false;
		emsg.target_faction = 0xFFFFFFFF;
		return true;
	}
	bool IsInvader = false;
	XID attacker = emsg.ainfo.attacker;
	if(attacker.IsPlayerClass())
	{
		//��ǿ�ƹ��������˺�
		if(!emsg.force_attack) return false;

		//����Է�δ��PK����Ҳ���ᱻ����
		if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE))
		{
			return false;
		}

		//��PK״̬�����˺�
		if(!__this->OI_IsPVPEnable()) return false;

		//���ܶ����˺�
		if(__this->OI_IsInTeam() && __this->OI_IsMember(attacker)) return false;

		//��������˰��ɱ��������ҹ����ߺͱ������ߵİ�����ͬ���򲻻ᱻ������
		int idmafia = emsg.ainfo.mafia_id;
		if(idmafia)
		{
			if(emsg.force_attack&C2S::FORCE_ATTACK_NO_MAFIA 
					&& idmafia == __this->OI_GetMafiaID()) return false;	
			if(emsg.force_attack&C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE 
					&& __this->OI_IsFactionAlliance(idmafia)) return false;	
		}
		
		//���������������������ͬ������Ҳ��ᱻ����
		if(emsg.ainfo.force_id && emsg.ainfo.force_id == __this->OI_GetForceID()) return false;

		int invader_state = __this->OI_GetInvaderState();
		//����ܹ������ǰ������ҹ�����Ϣ�Ǳ���������ô���ᱻ������
		if(invader_state == gactive_imp::INVADER_LVL_0 && (emsg.force_attack & C2S::FORCE_ATTACK_NO_WHITE))
		{
			return false;
		}

		//����Է�����PVP״̬����ش���Ϣ����Ҵ���PVP
		if(!(emsg.attacker_mode & attack_msg::PVP_DURATION))
		{
			MSG msg;
			BuildMessage(msg,GM_MSG_ENABLE_PVP_DURATION,attacker,
					__this->_parent->ID,__this->_parent->pos,0);
			__this->_plane->PostLazyMessage(msg);
		}

		//����������߲��Ǻ�����Ϊ�Ƿ�����
		//IsInvader = (invader_state != gactive_imp::INVADER_LVL_2);
		//���ڸĳ�һ�ɷǷ�������������������Ҳ�ǷǷ�����
		IsInvader = true;
	}
	emsg.is_invader = IsInvader;
	return true;
}

bool 
gplayer_imp::__GetPetAttackHook(gactive_imp * __this, const MSG & msg, attack_msg & amsg)
{
	//���г���Ĺ����ж�������һ���һ��
	//����Ǿ��� ���������ж� ���ﲻ���д��� 
	return PetTestHarmfulEffect(__this, msg, amsg);
}

bool 
gplayer_imp::__GetPetEnchantHook(gactive_imp * __this, const MSG & msg,enchant_msg & emsg)
{
	XID attacker = emsg.ainfo.attacker;
	if(emsg.helpful)
	{
		//�Ѻü���
		XID attacker = emsg.ainfo.attacker;
		bool pvpcombat = __this->OI_IsInPVPCombatState();
		if(attacker.IsPlayerClass())
		{
			if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE) && pvpcombat)
			{
				return false;
			}
		}
		int invader_state = __this->OI_GetInvaderState();
		emsg.is_invader = pvpcombat && (invader_state != INVADER_LVL_0) && (emsg.attacker_mode & attack_msg::PVP_ENABLE);
		
		//����Է�����PVP״̬�������Լ�����PVP״̬����һ����Ϣ�öԷ���ΪPVP״̬
		if(!(emsg.attacker_mode & attack_msg::PVP_DURATION) && pvpcombat)
		{
			__this->SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,attacker,0);
		}
	}
	else
	{
		//�к����� ��ͬ����ͨ����
		return PetTestHarmfulEffect(__this, msg, emsg);
	}
	return true;
}

template < typename EFFECT_MSG>
inline static void FillPetAttackMsg(gactive_imp * __this, EFFECT_MSG &attack)
{
	bool is_pvpenable = __this->OI_IsPVPEnable();
	if(!is_pvpenable) attack.force_attack = 0;
	if(attack.force_attack) attack.force_attack |= C2S::FORCE_ATTACK;
	if(attack.force_attack & (C2S::FORCE_ATTACK_NO_MAFIA|C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE))
	{
		//��������˲��˰��ɳ�Ա/����ͬ����������Ӧ��־
		attack.ainfo.mafia_id = __this->OI_GetMafiaID();
	}
	if(attack.force_attack & C2S::FORCE_ATTACK_NO_SAME_FORCE)
	{
		attack.ainfo.force_id = __this->OI_GetForceID();
	}

	attack.attacker_mode = is_pvpenable?attack_msg::PVP_ENABLE:0;
	if(__this->OI_GetTaskMask() & TASK_MASK_KILL_PLAYER) attack.attacker_mode |= attack_msg::PVP_FEEDBACK_KILL;

}

void 
gplayer_imp::__GetPetAttackFill(gactive_imp * __this, attack_msg & attack)
{
	FillPetAttackMsg(__this, attack);
}

void 
gplayer_imp::__GetPetEnchantFill(gactive_imp * __this, enchant_msg & enchant)
{
	FillPetAttackMsg(__this, enchant);
}

void 
gplayer_imp::SetPetLeaderData(pet_leader_prop & data)
{
	data.is_pvp_enable = OI_IsPVPEnable();
	data.pvp_combat_timer = GetPVPCombatTimer();
	data.mafia_id = OI_GetMafiaID();
	data.team_count = GetTeamMemberNum();
	data.team_efflevel = _team.GetEffLevel();
	data.wallow_level = _team.GetWallowLevel();
	data.profit_level = _profit_level;
	if(data.team_efflevel <=0) data.team_efflevel = _basic.level;
	GetMemberList(data.teamlist);
	data.team_id = 0;
	data.team_seq = 0;
	_team.GetTeamID(data.team_id, data.team_seq);
	data.cs_index = GetCSIndex();
	data.cs_sid = GetCSSid();
	data.duel_target = _duel_target;
	data.task_mask = _task_mask;
	data.force_id = _player_force.GetForce();
	data.invader_state = _invader_state;
	data.free_pvp_mode = _free_pvp_mode;
	data.object_state = ((gactive_object*)_parent)->object_state;
	
	int cls = GetObjectClass();
	if(cls >= 0 && ((1<<cls) & 0xACE)) // �߻��涨Զ��ְҵ USER_CLASS_COUNT
	{
		data.anti_def_degree = _anti_resistance_degree;
	}
	else
	{
		data.anti_def_degree = _anti_defense_degree;
	}
}

bool gplayer_imp::ResurrectPet(unsigned int index)
{
	int rst = _petman.ResurrectPet(this,index);
	if(!rst) return true;
	_runner->error_message(rst);
	return false;
}

void 
gplayer_imp::NotifyMasterInfoToPet(bool at_once)
{
	_petman.NotifyMasterInfo(this, at_once);
	_plantpetman.NotifyMasterInfo(this);
}

void 
gplayer_imp::OnDuelStart(const XID & target)
{
	gactive_imp::OnDuelStart(target);
	_duel_target = target.id;
	NotifyMasterInfoToPet(true);
}

void 
gplayer_imp::OnDuelStop()
{
	gactive_imp::OnDuelStop();
	_duel_target = 0;
	NotifyMasterInfoToPet(true);
}

void 
gplayer_imp::SetExtRestParam(const void * buf, unsigned int size)
{
	if(size < sizeof(int) || !buf)
	{	
		//��С����ȷ��������
		return;
	}
	const int * content = (const int*)buf;
	int version = content[0];
	switch(version)
	{
		case 1:
		if(size != sizeof(int) + sizeof(int) * 2) return;
		_mafia_rest_time = content[1];
		_mafia_rest_counter_time = content[2];
		break;
		default:
		break;
	}
}

void 
gplayer_imp::GetExtRestParam(archive & ar)
{
	ar << (int)1; //version 1;
	ar << _mafia_rest_time << _mafia_rest_counter_time;
}

void 
gplayer_imp::SayHelloToNPC(const XID & target)
{
	SendTo<0>(GM_MSG_SERVICE_HELLO,target,GetFaction());
}

XID 
gplayer_imp::OI_GetPetID()
{
	return _petman.GetCurPet();
}

int 
gplayer_imp::OI_GetPetType()
{
	int pindex = _petman.GetCurActivePet();
	if(pindex < 0) return 0;
	
	pet_data * pData = OI_GetPetData(pindex);
	if(!pData) return 0;
			
	return pData->pet_class;
}
void
gplayer_imp::OI_ResurrectPet()
{
	if(int rst = _petman.ResurrectPet(this))
	{
		_runner->error_message(rst);
	}
}

void 
gplayer_imp::Notify_StartAttack(const XID & target,char force_attack)
{
	if(target != _last_attack_target)
	{
		OnAntiCheatAttack(0.02f);
		_last_attack_target = target;
	}
	_petman.NotifyStartAttack(this,target, force_attack);
	_plantpetman.NotifyStartAttack(this,target, force_attack);
}

void 
gplayer_imp::FirstAcquireItem(const item_data* itemdata)
{
    if ((itemdata == NULL) || (itemdata->item_content == NULL)) return;
    int item_id = itemdata->type;

	//�״λ�ÿ�������뿨���ռ�ͼ��
	DATA_TYPE dt;
	const POKER_ESSENCE * ess = (const POKER_ESSENCE *)world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE,dt);
	if(ess && dt == DT_POKER_ESSENCE)
	{
		unsigned int collection_idx = (unsigned int)ess->show_order;
		if(collection_idx < GENERALCARD_MAX_COLLECTION)
		{
			_generalcard_collection.set(collection_idx, true);
			_runner->add_generalcard_collection(collection_idx);
		}
	}
	
	if(!world_manager::IsRareItem(item_id)) return;
	if(CheckGMPrivilege() && !player_template::GetDebugMode()) return;	//��ʽ����GM���ϡ����Ʒ������
	struct 
	{
		int item_id;
		char name[MAX_USERNAME_LENGTH];

        int proc_type;
        int expire_date;
        unsigned int content_length;
	}data;
	memset(&data,0,sizeof(data));

	data.item_id = item_id;
	unsigned int  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.name, _username,len);

    data.proc_type = itemdata->proc_type;
    data.expire_date = itemdata->expire_date;
    data.content_length = itemdata->content_length;

    packet_wrapper buf(sizeof(data) + itemdata->content_length);
    buf.push_back(&data, sizeof(data));

    if (itemdata->content_length > 0)
        buf.push_back(itemdata->item_content, itemdata->content_length);

	if(item_id >= 68975 && item_id <= 69425)
	broadcast_chat_msg(RARE_ITEM_CHAT_MSG_ID, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SUPERFARCRY,6,0,0); 
	else
	broadcast_chat_msg(RARE_ITEM_CHAT_MSG_ID, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0);

	__PRINTF("�����ƶ��㲥\n");
}

bool 
gplayer_imp::ChangePetName(unsigned int index,const char name[] , unsigned int name_len)
{
	int rst = _petman.ChangePetName(this, index, name, name_len);
	if(rst)
	{
		_runner->error_message(rst);
		return false;
	}
	return true;
}

bool 
gplayer_imp::ForgetPetSkill(int skill_id)
{
	int rst = _petman.ForgetPetSkill(this, skill_id);
	if(rst)
	{
		_runner->error_message(rst);
		return false;
	}
	return true;
}

int 
gplayer_imp::LearnPetSkill(int skill_id)
{
	int level = 0;
	int rst = _petman.LearnSkill(this,skill_id, &level);
	if(rst)
	{
		_runner->error_message(rst);
		return -1;
	}
	return level;
}

bool 
gplayer_imp::IsPetExist(unsigned int index, int pet_tid)
{
	pet_data * pData = _petman.GetPetData(index);
	return pData != NULL && pData->pet_tid == pet_tid;
}

int 
gplayer_imp::DyePet(unsigned int p_index, unsigned int d_index)
{
	//�������Ƿ����
	pet_data * pData = _petman.GetPetData(p_index);
	if(pData == NULL) return S2C::ERR_DYE_FAILED;
	//�����Ʒ�Ƿ����
	if(d_index >= _inventory.Size()) return S2C::ERR_DYE_FAILED;
	item & dye_item = _inventory[d_index];
	if(dye_item.type == -1) return S2C::ERR_DYE_FAILED;

	//�����Ʒ�Ƿ���� ��Ⱦɫ��
	itemdataman & dataman =  world_manager::GetDataMan();
	DATA_TYPE dt;
	PET_ESSENCE * pess = (PET_ESSENCE*)dataman.get_data_ptr(pData->pet_tid, ID_SPACE_ESSENCE,dt);
	if(pess == NULL || dt != DT_PET_ESSENCE) return S2C::ERR_DYE_FAILED;
	DYE_TICKET_ESSENCE * dess = (DYE_TICKET_ESSENCE*)dataman.get_data_ptr(dye_item.type, ID_SPACE_ESSENCE,dt);
	if(dess == NULL || dt != DT_DYE_TICKET_ESSENCE) return S2C::ERR_DYE_FAILED;

	//�������Ƿ����Ⱦɫ
	if(pess->require_dye_count <= 0) return S2C::ERR_PET_CAN_NOT_BE_DYED;
	if(_petman.GetCurActivePet() == (int)p_index) return S2C::ERR_DYE_FAILED;
	
	//���Ⱦɫ���Ƿ��㹻
	if(!_inventory.IsItemExist(dye_item.type, pess->require_dye_count)) return S2C::ERR_DYE_NOT_ENOUGH;
	
	float h,s,v;
	h = abase::Rand(dess->h_min, dess->h_max);
	s = abase::Rand(dess->s_min, dess->s_max);
	v = abase::Rand(dess->v_min, dess->v_max);
	int color = hsv2rgb(h,s,v);

	unsigned short r = ((color >> 16) >> 3) & 0x1F;
	unsigned short g = ((color >> 8) >> 3) & 0x1F;
	unsigned short b = (color >> 3) & 0x1F;
	unsigned short pet_color = ((r << 10) | (g << 5) | b) & 0x7FFF;
	pet_color |= 0x8000;	//����Ⱦɫ��ǣ�������δȾɫ���
	if(!_petman.DyePet(this, p_index, pet_color)) return S2C::ERR_PET_CAN_NOT_BE_DYED;

	//ɾ����Ʒ
	RemoveItems(dye_item.type,pess->require_dye_count, S2C::DROP_TYPE_USE, true);

	//��ȷ����
	return 0;
}

namespace {
class  clear_expire_item
{
	gplayer_imp * _imp;
	bool _notify;
	char _where;
	public:
	int _min_date;
	int _cur_t;
	int _remove_count;
	uint64_t _remove_mask;
	public:
	clear_expire_item(gplayer_imp * __this, bool notify, char where):_imp(__this),_notify(notify),_where(where)
	{
		_cur_t = g_timer.get_systime();
		_min_date = 0;
		_remove_count = 0;
		_remove_mask = 0;
	}

	void operator()(item_list * list, unsigned int index, item & it)
	{       
		if(_cur_t >= it.expire_date)
		{       
			item& it = (*list)[index];
			_imp->UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);

			if(_notify)
			{       
				_imp->_runner->player_drop_item(_where,index,it.type,it.count,S2C::DROP_TYPE_EXPIRE);
			}
			GLog::log(GLOG_INFO,"�û�%d����Ʒ%d(%d��)������ʧ",_imp->GetParent()->ID.id,it.type, it.count);
			list->Remove(index);
			_remove_count ++; 
			_remove_mask |= 1ULL << index;
		}
		else
		{       
			if(!_min_date)
			{       
				_min_date = it.expire_date;
			}
			else if(_min_date > it.expire_date)
			{
				_min_date = it.expire_date;
			}
		}

	}
};
}

void                    
gplayer_imp::RemoveAllExpireItems()
{               
	clear_expire_item  cei1(this, true, IL_INVENTORY);
	_inventory.ForEachExpireItems(cei1);

	clear_expire_item  cei2(this, true, IL_EQUIPMENT);
	_equipment.ForEachExpireItems(cei2);
	if(cei2._remove_count)
	{       
		RefreshEquipment();
		CalcEquipmentInfo();
		_runner->equipment_info_changed(0,cei2._remove_mask, 0, 0); //�˺���ʹ����CalcEquipmentInfo�Ľ��
		IncEquipChangeFlag();
	}                               

	clear_expire_item  cei3(this, true, IL_TRASH_BOX);
	_trashbox.GetBackpack(0).ForEachExpireItems(cei3);
	
	clear_expire_item  cei4(this, true, IL_TRASH_BOX2);
	_trashbox.GetBackpack(1).ForEachExpireItems(cei4);
	
	clear_expire_item  cei5(this, true, IL_TRASH_BOX3);
	_trashbox.GetBackpack(2).ForEachExpireItems(cei5);

	clear_expire_item  cei6(this, true, IL_TRASH_BOX4);
	_trashbox.GetBackpack(3).ForEachExpireItems(cei6);

	clear_expire_item  cei7(this, true, IL_TRASH_BOX5);
	_trashbox.GetBackpack(4).ForEachExpireItems(cei7);

	clear_expire_item  cei8(this, true, IL_TRASH_BOX6);
	_trashbox.GetBackpack(5).ForEachExpireItems(cei8);

	clear_expire_item  cei9(this, true, IL_TRASH_BOX7);
	_trashbox.GetBackpack(6).ForEachExpireItems(cei9);

	clear_expire_item  cei10(this, true, IL_TRASH_BOX8);
	_trashbox.GetBackpack(7).ForEachExpireItems(cei10);

	clear_expire_item  cei11(this, true, IL_USER_TRASH_BOX);
	_user_trashbox.GetBackpack(0).ForEachExpireItems(cei11);

	_expire_item_date = 0;
	if(cei1._min_date) UpdateExpireItem(cei1._min_date);
	if(cei2._min_date) UpdateExpireItem(cei2._min_date);
	if(cei3._min_date) UpdateExpireItem(cei3._min_date);
	if(cei4._min_date) UpdateExpireItem(cei4._min_date);
	if(cei5._min_date) UpdateExpireItem(cei5._min_date);
	if(cei6._min_date) UpdateExpireItem(cei6._min_date);
	if(cei7._min_date) UpdateExpireItem(cei7._min_date);
	if(cei8._min_date) UpdateExpireItem(cei8._min_date);
	if(cei9._min_date) UpdateExpireItem(cei9._min_date);
	if(cei10._min_date) UpdateExpireItem(cei10._min_date);
	if(cei11._min_date) UpdateExpireItem(cei11._min_date);

}

namespace {
class  clear_expire_addon
{
	gplayer_imp * _imp;
	bool _notify;
	char _where;
	public:
	int _min_date;
	int _cur_t;
	int _changed_count;
	uint64_t _changed_mask;
	public:
	clear_expire_addon(gplayer_imp * __this, bool notify, char where):_imp(__this),_notify(notify),_where(where)
	{
		_cur_t = g_timer.get_systime();
		_min_date = 0;
		_changed_count = 0;
		_changed_mask = 0;
	}

	void operator()(item_list * list, unsigned int index, item & it)
	{       
		int addon_expire = it.GetAddonExpireDate();
		if(addon_expire <= 0) return;
		if(_cur_t >= addon_expire)
		{
			if(_where == gplayer_imp::IL_EQUIPMENT)
				it.Deactivate(item::BODY,index,_imp);
			addon_expire = it.RemoveExpireAddon(_cur_t);
			if(_where == gplayer_imp::IL_EQUIPMENT)
				it.Activate(item::BODY, *list, index, _imp);
			if(_notify)
				_imp->PlayerGetItemInfo(_where, index);
			_changed_count ++;
			_changed_mask |= 1ULL << index;
		}
		if(addon_expire > 0)
		{       
			if(!_min_date)
			{       
				_min_date = addon_expire;
			}
			else if(_min_date > addon_expire)
			{
				_min_date = addon_expire;
			}
		}
	}
};
}

void 
gplayer_imp::RemoveAllExpireAddon()
{
	clear_expire_addon  cei1(this, true, IL_INVENTORY);
	_inventory.ForEachItems(cei1);

	clear_expire_addon  cei2(this, true, IL_EQUIPMENT);
	_equipment.ForEachItems(cei2);
	if(cei2._changed_count)
	{       
		RefreshEquipment();
		CalcEquipmentInfo();
		abase::octets os;
		os.reserve(cei2._changed_count*sizeof(int));
		for(int i=item::EQUIP_VISUAL_START; i<item::EQUIP_VISUAL_END; i++)
		{
			if(cei2._changed_mask & (1ULL<<i))
			{
				X_EQUIP type;
				*(int*)&type.type = _equipment[i].type; type.mask = _equipment[i].GetIdModify();
				os.push_back(&type,sizeof(X_EQUIP));
			}
		}
		_runner->equipment_info_changed(cei2._changed_mask, 0, os.begin(), os.size()); //�˺���ʹ����CalcEquipmentInfo�Ľ��
		IncEquipChangeFlag();
	}                               

	clear_expire_addon  cei3(this, true, IL_TRASH_BOX);
	_trashbox.GetBackpack(0).ForEachItems(cei3);
	
	clear_expire_addon  cei4(this, true, IL_TRASH_BOX2);
	_trashbox.GetBackpack(1).ForEachItems(cei4);
	
	clear_expire_addon  cei5(this, true, IL_TRASH_BOX3);
	_trashbox.GetBackpack(2).ForEachItems(cei5);

	clear_expire_addon  cei6(this, true, IL_TRASH_BOX4);
	_trashbox.GetBackpack(3).ForEachItems(cei6);

	clear_expire_addon  cei7(this, true, IL_TRASH_BOX5);
	_trashbox.GetBackpack(4).ForEachItems(cei7);

	clear_expire_addon  cei8(this, true, IL_TRASH_BOX6);
	_trashbox.GetBackpack(5).ForEachItems(cei8);

	clear_expire_addon  cei9(this, true, IL_TRASH_BOX7);
	_trashbox.GetBackpack(6).ForEachItems(cei9);

	clear_expire_addon  cei10(this, true, IL_TRASH_BOX8);
	_trashbox.GetBackpack(7).ForEachItems(cei10);

	clear_expire_addon  cei11(this, true, IL_USER_TRASH_BOX);
	_user_trashbox.GetBackpack(0).ForEachItems(cei11);

	_min_addon_expire_date = 0;
	if(cei1._min_date) UpdateMinAddonExpireDate(cei1._min_date);
	if(cei2._min_date) UpdateMinAddonExpireDate(cei2._min_date);
	if(cei3._min_date) UpdateMinAddonExpireDate(cei3._min_date);
	if(cei4._min_date) UpdateMinAddonExpireDate(cei4._min_date);
	if(cei5._min_date) UpdateMinAddonExpireDate(cei5._min_date);
	if(cei6._min_date) UpdateMinAddonExpireDate(cei6._min_date);
	if(cei7._min_date) UpdateMinAddonExpireDate(cei7._min_date);
	if(cei8._min_date) UpdateMinAddonExpireDate(cei8._min_date);
	if(cei9._min_date) UpdateMinAddonExpireDate(cei9._min_date);
	if(cei10._min_date) UpdateMinAddonExpireDate(cei10._min_date);
	if(cei11._min_date) UpdateMinAddonExpireDate(cei11._min_date);
}

bool
gplayer_imp::PlayerDoShopping(unsigned int goods_count,const int * order_list, int shop_tid)
{
	//������Ҫ���Ӹ�����Թ����״̬
	if(_player_state != PLAYER_SIT_DOWN && _player_state != PLAYER_STATE_NORMAL && _player_state != PLAYER_STATE_BIND) return false;

	if(goods_count == 0)
	{
		return false;
	}
	if(goods_count > _inventory.Size() || !InventoryHasSlot(goods_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	int gifts_count = 0;

	netgame::mall & shop = world_manager::GetPlayerMall();
	int __group_id = shop.GetGroupId();	//��ǰ�������趨��group_id,lgc
	time_t __time = time(NULL);			//
	netgame::mall_order  order(_mall_order_id);
	abase::vector<netgame::mall_invoice, abase::fast_alloc<> > invoice_list;
	invoice_list.reserve(goods_count);
	
	std::map<int, int> item_limit_type_map; // item_id -> limit_type

	ASSERT(netgame::mall::MAX_ENTRY == 4);
	unsigned int offset = 0;
	for(unsigned int i = 0; i < goods_count; i ++, offset += sizeof(C2S::CMD::mall_shopping::__entry) / sizeof(int))
	{
		int mall_rate = order_list[offset];
		int id = order_list[offset+1];
		unsigned int index = order_list[offset +2];
		unsigned int slot = order_list[offset +3];
		if(slot >= netgame::mall::MAX_ENTRY)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		netgame::mall::node_t node;
		if(!shop.QueryGoods(index,node) || node.goods_id != id)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(!node.check_owner(shop_tid))
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(node.entry[slot].cash_need <= 0)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		if(IsItemForbidShop(node.goods_id)
				|| node.gift_id > 0 && IsItemForbidShop(node.gift_id))
		{
			_runner->error_message(S2C::ERR_ITEM_FORBID_SHOP);
			return true;
		}
		
		// MALL RATE 172 BEGIN
		if (mall_rate <= 0) mall_rate = 1;
		if (mall_rate >= 1000) mall_rate = 1000;
		
		if (node.goods_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.goods_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.goods_count = new_count;
			node.entry[slot].cash_need *= mall_rate;
		}
		
		if (node.gift_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.gift_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.gift_count = new_count;
		}
		// MALL RATE 172 END
		
		if(!_purchase_limit_info.CheckShoppingLimitItem(id, node.buy_times_limit, node.buy_times_limit_mode, node.goods_count))
		{
			_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 1);
			return true;
		}
		
		if(GetCashVipLevel() < node.entry[slot].min_vip_level)
		{
			_runner->error_message(S2C::ERR_CASH_VIP_LIMIT);
			return true;
		}

		//lgc	
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}

		if(node.sale_time_active)
		{	
			if(node.entry[slot].group_id == active_group_id && node.entry[slot]._sale_time.CheckAvailable(__time))	
			{
				//���playerѡ���slot�����õ����۷�ʽ������Ҫɨ�赱ǰ��Ч���ڣ����Ƿ񻹴��ڷ��������۷�ʽ�����
				if(node.entry[slot]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
				{
					for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
					{
						if(node.entry[j].cash_need <= 0)
							break;
						if(node.entry[j].group_id == active_group_id 
									&& node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT
									&& node.entry[j]._sale_time.CheckAvailable(__time))
						{
							_runner->mall_item_buy_failed(index, 0);
							return false;
						}
					}
				}				
			}
			else
			{
				_runner->mall_item_buy_failed(index, 0);
				return false;
			}
		}
		else if(node.entry[slot].group_id != active_group_id)
		{
			_runner->mall_item_buy_failed(index, 0);
			return false;
		}

		if(node.gift_id > 0) gifts_count ++;  //ͳ����Ʒ��

		order.AddGoods(node.goods_id, node.goods_count , node.entry[slot].cash_need , node.entry[slot].expire_time,node.entry[slot].expire_type,node.gift_id,node.gift_count,node.gift_expire_time,node.gift_log_price);

		if(node.buy_times_limit_mode)
			item_limit_type_map[node.goods_id] = node.buy_times_limit_mode;

	}
	if(GetMallCash() < order.GetPointRequire())
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	if(!InventoryHasSlot(goods_count + gifts_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int total_cash = GetMallCash();
	int cash_used = 0;
	//��Ǯ�㹻�� ��ʼ������Ʒ
	int cur_t = g_timer.get_systime();
	int self_id = GetParent()->ID.id;
	for(unsigned int i = 0; i < goods_count; i ++)
	{
		int id;
		int count;
		int point;
		int expire_time;
		int expire_type;
		int gift_id;
		int gift_count;
		int gift_expire_time;
		int gift_log_price;
		bool bRst = order.GetGoods(i, id, count,point, expire_time,expire_type,gift_id,gift_count,gift_expire_time,gift_log_price);
		if(bRst)
		{
			if ( LuaManager::GetInstance()->EventOnPlayerDoShopping(self_id, id, count, point) )
			return false;

			//������Ʒ����Ʒ��log�۸�
			int log_price1 = point;
			int log_price2 = 0;
			int item_type = -1;
			int item_proc_type = 0;
			if(gift_id > 0 && gift_log_price > 0)
			{
				log_price1 = int((float)point*point/(point+gift_log_price));
				log_price2 = point - log_price1; 
			}
			
			const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(id);
			if(pItem)
			{
				item_data * pItem2 = DupeItem(*pItem);
				int expire_date = 0;
				if(expire_time) 
				{
					if(expire_type == netgame::mall::EXPIRE_TYPE_TIME)
					{
						//��Ч������һ������
						expire_date = cur_t + expire_time;
					}
					else
					{
						//��Ч���ǹ涨����ʧЧ
						expire_date = expire_time;
					}
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pItem2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pItem2->type, guid1,guid2);
					pItem2->guid.guid1 = guid1;
					pItem2->guid.guid2 = guid2;
				}

				pItem2->proc_type |= item::ITEM_PROC_TYPE_MALL;
				UpdateMallConsumptionShopping(pItem2->type, pItem2->proc_type, count, log_price1);
				item_type = pItem2->type;
				item_proc_type = pItem2->proc_type;
				
				int ocount = count;
				int rst =_inventory.Push(*pItem2,count,expire_date);
				ASSERT(rst >= 0 && count == 0);
				_runner->obtain_item(id,pItem2->expire_date,ocount,_inventory[rst].count, 0,rst);

				if(item_limit_type_map.find(id) != item_limit_type_map.end())
				{
					int have_purchase_count = _purchase_limit_info.AddShoppingLimit(id, item_limit_type_map[id], ocount);
					_runner->purchase_limit_info_notify(item_limit_type_map[id], id, have_purchase_count);
				}

				//�������³�ʼ��һ�¿��ܵ�ʱװ
				_inventory[rst].InitFromShop();

				total_cash -= log_price1;
				cash_used += log_price1;

				//��¼��־  
				GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:cash_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,_mall_order_id,id,expire_date,ocount,log_price1,total_cash,guid1,guid2);
				
				invoice_list.push_back(netgame::mall_invoice(_mall_order_id, id, ocount, point,expire_date,g_timer.get_systime(), guid1,guid2));
				world_manager::TestCashItemGenerated(id, ocount);
				FreeItem(pItem2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%dʱ������Ʒʧ��",self_id, id);
			}

			//����Ϊ������Ʒ
			if(gift_id > 0)
			{
			const item_data * pGift = (const item_data*)world_manager::GetDataMan().get_item_for_sell(gift_id);
			if(pGift)
			{
				item_data * pGift2 = DupeItem(*pGift);
				int expire_date = 0;
				if(gift_expire_time) 
				{
					//��Ч������һ������
					expire_date = cur_t + gift_expire_time;
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pGift2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pGift2->type, guid1,guid2);
					pGift2->guid.guid1 = guid1;
					pGift2->guid.guid2 = guid2;
				}

				// ͳ�Ƶ�����ֵ������Ķ��������ģ�������������д0
				UpdateMallConsumptionShopping(item_type, item_proc_type, 0, log_price2);

				int ocount = gift_count;
				int rst =_inventory.Push(*pGift2,gift_count,expire_date);
				ASSERT(rst >= 0 && gift_count == 0);
				_runner->obtain_item(gift_id,expire_date,ocount,_inventory[rst].count, 0,rst);
				
				//�������³�ʼ��һ�¿��ܵ�ʱװ
                _inventory[rst].InitFromShop();
				
				total_cash -= log_price2;
				cash_used += log_price2;
				//��¼��־  
				GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:cash_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,_mall_order_id,gift_id, expire_date,ocount,log_price2,total_cash,guid1,guid2);
				
				world_manager::TestCashItemGenerated(gift_id, ocount);
				FreeItem(pGift2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%dʱ������Ʒ%dʧ��",self_id, id, gift_id);
			}
			}
		}
		else
		{
			ASSERT(false);
		}
	}

	_mall_cash_offset -= cash_used;
	_runner->player_cash(GetMallCash());

	//�������¼����������б�
	/* $$$$$$$$$$$$$$ Ŀǰ���ײ����湺���б��� ��Ϊ����Ϊ�˱�̫�� ����������ʧ��
	for(unsigned int i =0;i < invoice_list.size(); i ++)
	{
		_mall_invoice.push_back(invoice_list[i]);
	}
	*/

	//�����Ѽ�¼���͸�gdelivery,�������ӿ��Ը������ߵĺ������������ѻ���
	GMSV::SendRefCashUsed(_parent->ID.id, cash_used, _basic.level);

	GLog::log(GLOG_INFO,"�û�%d�ڰٱ�����%d����Ʒ������%d��ʣ��%d��",self_id,goods_count,cash_used,GetMallCash());

	_mall_order_id ++;
	//���Ǽӿ�����ٶ�
	
	return true;
}

bool 
gplayer_imp::CheckItemBindCondition(unsigned int index,  int id, int can_webtrade)
{
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(((it.proc_type & item::ITEM_PROC_NO_BIND_MASK) == item::ITEM_PROC_NO_BIND_MASK ) && !(it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)) return false;	//�Ѿ��󶨹���
	if(((it.proc_type & item::ITEM_PROC_NO_BIND_MASK) == item::ITEM_PROC_NO_BIND_MASK ) && (it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE) && can_webtrade) return false;	//�Ѿ��󶨹���
	if((it.proc_type & item::ITEM_PROC_TYPE_BIND) && !(it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)) 
		return false;	//�Ѿ��󶨹���
	if((it.proc_type & item::ITEM_PROC_TYPE_BIND) && (it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE) && can_webtrade) 
		return false;
	if(it.proc_type & item::ITEM_PROC_TYPE_BIND2) return false;	//װ����󶨲��ð�
	if( !it.CheckEquipCanBind()			//�����ϵ�װ��λ�ã����Ҳ��ǳ��ﵰ�����ܰ�
		&& !(it.body!=NULL && it.body->GetItemType()==item_body::ITEM_TYPE_PET_EGG))
		return false;
	if(it.pile_limit != 1) 	return false;				//���Զѵ��Ĳ��ܰ�
	if((it.proc_type & item::ITEM_PROC_TYPE_NOTRADE) && can_webtrade)
		return false;
	return true;
}

bool gplayer_imp::BindItem(unsigned int index, int id, int can_webtrade)
{
	ASSERT(IsItemExist(index, id,1));
	//ǰ���Ѿ�ȷ�ϼ����Ƿ���Խ��а���
	item & it = _inventory[index];
	it.proc_type |= item::ITEM_PROC_TYPE_NODROP | 
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND;
	if(it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
		it.proc_type &= ~item::ITEM_PROC_TYPE_CAN_WEBTRADE;
	if(can_webtrade) it.proc_type |= item::ITEM_PROC_TYPE_CAN_WEBTRADE;

	UpdateMallConsumptionBinding(it.type, it.proc_type, it.count);
	
	//�ٴη���һ����Ʒ����
	PlayerGetItemInfo(IL_INVENTORY,index);

	_runner->player_bind_success(index, id);
	return true;
}


bool 
gplayer_imp::CheckBindItemDestory(unsigned int index,  int id)
{
	if(!IsItemExist(index, id,1)) return false;
	const item & it = _inventory[index];
	if(!(it.proc_type & item::ITEM_PROC_TYPE_BIND)) return false;	//δ�󶨵���Ʒ���ܽ��д�������
	if( !it.CheckEquipCanBind()			//�����ϵ�װ��λ�ã����Ҳ��ǳ��ﵰ����������
		&& !(it.body!=NULL && it.body->GetItemType()==item_body::ITEM_TYPE_PET_EGG))
		return false;
	return true;
}

bool 
gplayer_imp::DestoryBindItem(unsigned int index, int id)
{
	ASSERT(IsItemExist(index, id,1));
	//ǰ���Ѿ�ȷ�ϼ����Ƿ���Խ��а���
	item  it;
	_inventory.Remove(index, it);
	GDB::itemdata data;
	item_list::ItemToData(it, data);
	
	//��֯����Ʒ�����ݰ�
	packet_wrapper h1(200);
	h1 << data.id << data.index << data.count 
	   << data.max_count << data.guid1 << data.guid2 
	   << data.mask << data.proctype << data.expire_date;

	h1 << data.size;
	h1.push_back(data.data,data.size);

	//��������ݰ���֯�µ���Ʒ
	item_data nit;
	nit.type 	= ITEM_DESTROYING_ID;
	nit.count 	= 1;
	nit.pile_limit 	= 1;
	nit.equip_mask 	= 0;
	nit.proc_type 	= item::ITEM_PROC_TYPE_NODROP | 
			  item::ITEM_PROC_TYPE_NOTHROW|
			  item::ITEM_PROC_TYPE_NOSELL |
			  item::ITEM_PROC_TYPE_NOTRADE|
			  item::ITEM_PROC_TYPE_NOPUTIN_USERTRASH;
	nit.classid 	= CLS_ITEM_DUMMY;
	nit.guid.guid1 	= 0;
	nit.guid.guid2 	= 0;
	nit.price 	= 0;
	nit.expire_date = g_timer.get_systime() + 72*3600;
	nit.content_length = h1.size();
	nit.item_content = (char *)h1.data();

	//֪ͨ�ͻ���ɾ����Ʒ
	//�����ɾ����Ʒ
	_runner->player_drop_item(IL_INVENTORY,index, it.type, it.count,S2C::DROP_TYPE_USE);
	it.Release();

	//���µ���Ʒ���뵽��Ұ���֮��
	int rst = _inventory.PushInEmpty(0, nit, 1);
	if(rst >=0)
	{
		_runner->obtain_item(nit.type,nit.expire_date,1,_inventory[rst].count, IL_INVENTORY,rst);
	}
	else
	{
		ASSERT(false);	//���ǲ�Ӧ�ó��ֵ�
	}
	return true;
} 

bool 
gplayer_imp::CheckRestoreDestoryItem(unsigned int index, int id)
{
	if(!IsItemExist(index, id,1)) return false;
	if(id != ITEM_DESTROYING_ID) return false;
	return true;
}

bool 
gplayer_imp::RestoreDestoryItem(unsigned int index, int id)
{
	ASSERT(IsItemExist(index, id,1));
	
	const item  &it = _inventory[index];
	if(it.body == 0) return false;

	const void * data;
	unsigned int data_len;
	it.body->GetItemData(&data, data_len);

	if(data_len < sizeof(int)*10) return false;

	GDB::itemdata idata;

	raw_wrapper ar(data,data_len);
	ar >> idata.id >> idata.index >> idata.count 
	   >> idata.max_count >> idata.guid1 >> idata.guid2 
	   >> idata.mask >> idata.proctype >> idata.expire_date;

	ar >> idata.size;
	idata.data = ar.cur_data();
	if(idata.size != ar.size() - ar.offset()) return false;
	
	item new_item;
	if(!MakeItemEntry(new_item,idata)) return false;

	//ɾ��ԭ������Ʒ
	item  old_item;
	_inventory.Remove(index, old_item);
	_runner->player_drop_item(IL_INVENTORY,index, old_item.type, old_item.count,S2C::DROP_TYPE_USE);
	old_item.Release();

	//���µ���Ʒ���뵽����֮��
	int type = new_item.type;
	int expire_date = new_item.expire_date;
	int rst = _inventory.PushInEmpty(0, new_item);
	if(rst >=0)
	{
		_runner->obtain_item(type,expire_date,1,_inventory[rst].count, IL_INVENTORY,rst);
	}
	else
	{
		new_item.Release();
		ASSERT(false);	
	}

	new_item.Clear();
	return true;
}

bool
gplayer_imp::OI_GetMallInfo(int & cash, int & cash_used, int &cash_delta,  int &order_id)
{
	cash = _mall_cash;
	cash_used = _mall_cash_used;
	cash_delta = _mall_cash_offset;
	order_id = _mall_order_id;
	return true;
}

bool
gplayer_imp::OI_IsCashModified()
{
	return NeedSaveMallInfo();
}

void 
gplayer_imp::SendTeamChat(char channel, const void * buf, unsigned int len, const void * aux_data, unsigned int dsize, int use_id)
{
	if(IsInTeam())
	{
		_team.TeamChat(channel,_chat_emote,buf,len,use_id > 0?use_id:_parent->ID.id, aux_data , dsize);
	}
	else
	{
		GMSV::chat_msg msg;
		msg.speaker = use_id > 0?use_id:_parent->ID.id;
		msg.msg = buf;
		msg.size = len;
		msg.data = aux_data;
		msg.dsize = dsize;
		msg.channel = channel;
		msg.emote_id = _chat_emote;
		msg.speaker_level = 0;
		GMSV::SendChatMsg(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid ,msg);
	}
}

void
gplayer_imp::SendGlobalChat(char channel, const void * msg, unsigned int size, const void* data, unsigned int dsize, bool check_cooldown)
{
	if(_player_state != PLAYER_STATE_NORMAL
		&& _player_state != PLAYER_STATE_BIND
		&& _player_state != PLAYER_SIT_DOWN
		&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}

	if(InCentralServer())
	{
		_runner->error_message(S2C::ERR_SERVICE_UNAVILABLE);
		return;
	}

	if (OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	if(check_cooldown)
	{
		if (!CheckCoolDown(COOLDOWN_INDEX_GLOBAL_CRY))
		{
			_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
			return;
		}
		
		SetCoolDown(COOLDOWN_INDEX_GLOBAL_CRY, GLOABL_CRY_COOLDOWN_TIME);
	

	int item_need = 0;
	int item_idx = -1;

	int * ITEM_GLOBAL_SPEAKER_TABLE = LuaManager::GetInstance()->GetConfig()->ITEM_GLOBAL_SPEAKER_TABLE;
	for (int i = 0; ITEM_GLOBAL_SPEAKER_TABLE[i] && i < 16 ; i++)
	{
		item_idx = _inventory.Find(0,ITEM_GLOBAL_SPEAKER_TABLE[i]);
		if(item_idx >= 0) 
		{
			item_need = ITEM_GLOBAL_SPEAKER_TABLE[i];
			break;
		}
	}	
	if(item_idx < 0)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return ;
	}

	/*
	int	item_need = GLOBAL_SPEAKER_ID;
	int	item_idx = _inventory.Find(0, item_need);
	if(item_idx < 0)
	{       
		item_need = GLOBAL_SPEAKER_ID2;
		item_idx = _inventory.Find(0, item_need);
		if(item_idx < 0)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return ;
		}
	}
	*/

	if( !LuaManager::GetInstance()->IsGoodItem(item_need) ) 
	{
		item& it = _inventory[item_idx];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_inventory.DecAmount(item_idx,1);
		_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_need,1);
	}
	
	}
	
	broadcast_chat_msg(_parent->ID.id,msg,size,channel,7,data,dsize); 

}

bool
gplayer_imp::CheckAllianceChat(char channel)
{
	if(    _player_state != PLAYER_STATE_NORMAL
		&& _player_state != PLAYER_STATE_BIND
		&& _player_state != PLAYER_SIT_DOWN
		&& _player_state != PLAYER_STATE_MARKET)
	{
		return false;
	}
	
	if(!CheckCoolDown(COOLDOWN_INDEX_FARCRY))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return false;
	}	
	
	SetCoolDown(COOLDOWN_INDEX_FARCRY, 800);
	
	if(channel == GMSV::CHAT_CHANNEL_ALLIANCE)
	{
		int item_need = 0;
		int item_idx = -1;
		int * ITEM_ALLIANCE_SPEAKER_TABLE = LuaManager::GetInstance()->GetConfig()->ITEM_ALLIANCE_SPEAKER_TABLE;
		
		for (int i = 0; ITEM_ALLIANCE_SPEAKER_TABLE[i] && i < 16 ; i++)
		{
			item_idx = _inventory.Find(0,ITEM_ALLIANCE_SPEAKER_TABLE[i]);
			if(item_idx >= 0) 
			{
				item_need = ITEM_ALLIANCE_SPEAKER_TABLE[i];
				break;
			}
		}
		
		if(item_idx < 0)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		
		if( !LuaManager::GetInstance()->IsGoodItem(item_need) ) 
		{
			item& it = _inventory[item_idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
			_inventory.DecAmount(item_idx,1);
			_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_need,1);
		}
		
		return true;
	}
	
	return false;
}

void 
gplayer_imp::SendFarCryChat(char channel, const void * msg, unsigned int size, const void* data, unsigned int dsize, bool check_cooldown)
{
	if(_player_state != PLAYER_STATE_NORMAL
		&& _player_state != PLAYER_STATE_BIND
		&& _player_state != PLAYER_SIT_DOWN
		&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}

	if(check_cooldown)
	{
		if(!CheckCoolDown(COOLDOWN_INDEX_FARCRY))
		{
			_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
			return;
		}
	

	SetCoolDown(COOLDOWN_INDEX_FARCRY, 800);

	

	int item_need = 0;
	int item_idx = -1;
	
	if(channel == GMSV::CHAT_CHANNEL_FARCRY)
	{
		int * ITEM_WORLD_SPEAKER_TABLE = LuaManager::GetInstance()->GetConfig()->ITEM_WORLD_SPEAKER_TABLE;
		
		for (int i = 0; ITEM_WORLD_SPEAKER_TABLE[i] && i < 16 ; i++)
		{
			item_idx = _inventory.Find(0,ITEM_WORLD_SPEAKER_TABLE[i]);
			if(item_idx >= 0) 
			{
				item_need = ITEM_WORLD_SPEAKER_TABLE[i];
				break;
			}
		}	
		if(item_idx < 0)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return ;
		}
	}
	else if(channel == GMSV::CHAT_CHANNEL_SUPERFARCRY)
	{
		int * ITEM_SUPERWORLD_SPEAKER_TABLE = LuaManager::GetInstance()->GetConfig()->ITEM_SUPERWORLD_SPEAKER_TABLE;
		
		for (int i = 0; ITEM_SUPERWORLD_SPEAKER_TABLE[i] && i < 16 ; i++)
		{
			item_idx = _inventory.Find(0,ITEM_SUPERWORLD_SPEAKER_TABLE[i]);
			if(item_idx >= 0) 
			{
				item_need = ITEM_SUPERWORLD_SPEAKER_TABLE[i];
				break;
			}
		}
		if(item_idx < 0)
		{       
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return ;
		}		
	}
	else
	{
		ASSERT(false);
		return;
	}

	if( !LuaManager::GetInstance()->IsGoodItem(item_need) ) 
	{
		item& it = _inventory[item_idx];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_inventory.DecAmount(item_idx,1);
		_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_need,1);
	}	

	}
	
	broadcast_chat_msg(_parent->ID.id,msg,size,channel,channel==GMSV::CHAT_CHANNEL_FARCRY?_chat_emote:6,data,dsize); 
}

void 
gplayer_imp::SendNormalChat(char channel, const void * buf, unsigned int len, const void* data, unsigned int dsize)
{
	world *pPlane = _plane;
	if(pPlane == NULL) return ;
	gplayer * pPlayer = GetParent();
	slice * pPiece = pPlayer->pPiece;
	
	AutoBroadcastChatMsg(pPlane,pPiece,buf,len,channel,_chat_emote,data,dsize,pPlayer->ID.id,pPlayer->base_info.level);
}

void 
gplayer_imp::SendBattleFactionChat(char channel, const void * buf, unsigned int len, const void* data, unsigned int dsize)
{
	gplayer * pPlayer = GetParent();
	_plane->BattleFactionSay(GetFaction(), buf,len,_chat_emote,data,dsize,pPlayer->ID.id,pPlayer->base_info.level);
}

void 
gplayer_imp::SendCountryChat(char channel, const void * msg, unsigned int size, const void* data, unsigned int dsize)
{
	bool is_king = GetParent()->IsKing();
	if(!GetCountryId()) return;

	if(_player_state != PLAYER_STATE_NORMAL
		&& _player_state != PLAYER_STATE_BIND
		&& _player_state != PLAYER_SIT_DOWN
		&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}

	if(!CheckCoolDown(COOLDOWN_INDEX_COUNTRY_CHAT))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}

	if(!is_king && GetAllMoney() < COUNTRY_CHAT_FEE)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return;
	}

	SetCoolDown(COOLDOWN_INDEX_COUNTRY_CHAT, COUNTRY_CHAT_COOLDOWN_TIME);

	if(!is_king)
	{
		SpendAllMoney(COUNTRY_CHAT_FEE,true);
		SelfPlayerMoney();
	}

	country_chat_msg(_parent->ID.id,msg,size,channel,(is_king?_chat_emote|0x80:_chat_emote),data,dsize); 
}

void 
gplayer_imp::SetChatEmote(int emote_id)
{
	_chat_emote = emote_id & 0xFF;
	GMSV::SetChatEmote(GetParent()->ID.id, _chat_emote);
}

void 
gplayer_imp::ActivePetNoFeed(bool feed)
{
	_petman.ActiveNoFeed(feed);
}

void 
gplayer_imp::SetHPAutoGen(int value, float rate)
{
	_auto_hp_value = value;
	_auto_hp_percent = rate;
}

void 
gplayer_imp::SetMPAutoGen(int value, float rate)
{
	_auto_mp_value = value;
	_auto_mp_percent = rate;
}


bool 
gplayer_imp::StackProduceItem(const recipe_template & rt, unsigned int & act_count)
{
	int item_id = 0;
	int item_count = 0;
	recipe_template rt2;//
	memcpy(&rt2,&rt,sizeof(recipe_template));

	if(act_count > 1 && rt.targets[0].prob == 1.0f && rt.null_prob == 0.0f)
	{
		item_id = rt.targets[0].id;
		item_count = world_manager::GetDataMan().get_item_pile_limit(item_id);
		rt2.count *= act_count;
		if(item_id > 1 && item_count > 1 && rt2.count <= item_count)
		{

			rt2.level *= act_count; 
			rt2.exp *= act_count;
			rt2.sp *= act_count;
			rt2.fee *= act_count;
			rt2.material_total_count *= act_count;
			for(unsigned int j= 0; j < 32; j++)
			{
				if(rt2.material_list[j].item_id > 0)
				rt2.material_list[j].count *= act_count;
			}
			act_count = 1;
			return ProduceItem(rt2);
		}
	}
	
	return ProduceItem(rt);
}



bool 
gplayer_imp::ProduceItem(const recipe_template & rt)
{
	//��ͨ������������ѡ��ԭ��
	//����Ǯ
	if(GetAllMoney() < rt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.equipment_need_upgrade > 0) return false;

	/*if(rt.require_arena_personal_point > 0) 
	{		
		if(_glua.GetRole()->arena_self_score < rt.require_arena_personal_point)
		{
			_runner->error_message(S2C::ERR_ARENA_LOSE_POINTS);
			return false;
		}		
	}*/
	
	//�ж����ɺ�����Ʒ
	int item_id = 0;
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}

	//�������Ƿ��㹻
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));
	int num = rt.material_num;

	int nlist[_inventory.Size()];
	memset(nlist,0,sizeof(nlist));

	int total_count = rt.material_total_count;

	//�����״̬
	int bind_result = 0;	//0 ���� 1 ��  2 װ�����
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;

	//�����Ʒб��Ƿ���������

	for(unsigned int i = 0;i < _inventory.Size(); i++)
	{
		int type = _inventory[i].type;
		if(type == -1) continue;
		for(int j= 0; j < num; j++)
		{
			if(mlist[j].item_id == type)
			{
				unsigned int count = _inventory[i].count;
				if(count > mlist[j].count) count = mlist[j].count;
				nlist[i] = count;
				if(!(mlist[j].count -= count))
				{
					std::swap(mlist[j],mlist[num-1]);
					num --;
				}
				total_count -= count;
				if(rt.bind_type == 2)
				{
					int proc_type = _inventory[i].proc_type;
					if( proc_type & item::ITEM_PROC_TYPE_BIND)
					{
						if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
							bind_result = 3;
						else
							bind_result = 1;
					}
					if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
						bind_result = 2;
				}
				break;
			}
		}
		if(total_count == 0) break;
	}
	ASSERT(total_count >= 0);
	if(total_count > 0)
	{
		//ԭ�ϲ���
		//��������ж�
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false;
	}
	
	//������Ʒ����
	item_data * data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
		unsigned int  len;
		const void * name;
		name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;

		data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
		if(!data)
		{
			//��Ʒ����ʧ��
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < _inventory.Size(); ++i)
        {
            if (nlist[i] > 0)
            {
                snprintf(buf, sizeof(buf), "����%d, ����%d; ", _inventory[i].type, nlist[i]);
                itembuf += buf;
            }
        }

		GLog::log(GLOG_INFO, "�û�%d������%d��%d, �䷽%d, ����%s", _parent->ID.id, rt.count, item_id, rt.recipe_id, itembuf.c_str());
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����%dʱδ�ɹ� �䷽%d",_parent->ID.id,item_id,rt.recipe_id);
	}

	if(data)
	{
		//��һЩ����
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		//��������һ��������
		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	//�����������Ʒ����Ϣ
	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();
	//��player����ȡ��ԭ�Ϻ�������
	for(unsigned int i = 0; i < _inventory.Size(); i ++)
	{
		if(nlist[i]) 
		{
			item& it = _inventory[i];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, nlist[i]);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,i,_inventory[i].type,nlist[i],S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(i,nlist[i]);
		}
	}
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		} 
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}
		int rst = _inventory.Push(*data);
		if(rst >=0)
		{
		    FirstAcquireItem(data);
			//�������������Ʒ����Ϣ
			_runner->produce_once(item_id,rt.count - data->count,_inventory[rst].count, 0,rst);
		}

		if(data->count)
		{
			//ʣ������Ʒ
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
			//�����������Ҫ�ͷ��ڴ�
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			//���������ж�
			return false;
		}
		FreeItem(data);
		__PRINTF("�ɹ�����������Ʒ%d\n",item_id);
	}
	else
	{
		//��������δ�ɹ�����Ϣ
		_runner->produce_null(rt.recipe_id);
	}
	return true;
}

bool 
gplayer_imp::ProduceItem2(const recipe_template & rt,int materials[16], int idxs[16] )
{
	//�ϳɣ�����ѡ��ԭ�ϣ�bind_type��proc_type����Ч,bind_type���ȼ���
	//����Ǯ
	if(GetAllMoney() < rt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.bind_type == 0 && rt.proc_type == 0) return false;
	if(rt.equipment_need_upgrade > 0) return false;
	if(rt.material_total_count <= 0) return false;

	//�ж����ɺ�����Ʒ
	int item_id = 0;
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}

	//�������Ƿ��㹻
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));

	//�����״̬
	int bind_result = 0;	//0 ���� 1 ��  2 װ����� 3 ��Ѱ��������
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;

	//���в��ϼ��
	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 16; i++)
	{
		if(rt.material_list[i].item_id)
		{
			if(rt.material_list[i].item_id != materials[i]) return false;
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], rt.material_list[i].count) 
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
			mlist[i].item_id = materials[i];
			mlist[i].count = rt.material_list[i].count;

			//�����״̬
			if(rt.bind_type == 2)
			{
				int proc_type = _inventory[idx].proc_type;
				if( proc_type & item::ITEM_PROC_TYPE_BIND)
				{
					if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
						bind_result = 3;
					else
						bind_result = 1;
				}
				if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
					bind_result = 2;
			}
		}
	}
	
	//������Ʒ����
	item_data * data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
		unsigned int  len;
		const void * name;
		name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;

		data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
		if(!data)
		{
			//��Ʒ����ʧ��
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < 16; ++i)
        {
            if (mlist[i].item_id > 0)
            {
                snprintf(buf, sizeof(buf), "����%d, ����%d; ", _inventory[idxs[i]].type, mlist[i].count);
                itembuf += buf;
            }
        }

		GLog::log(GLOG_INFO, "�û�%d������%d��%d, �䷽%d, ����%s", _parent->ID.id, rt.count, item_id, rt.recipe_id, itembuf.c_str());
    }
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����%dʱδ�ɹ� �䷽%d",_parent->ID.id,item_id,rt.recipe_id);
	}

	if(data)
	{
		//��һЩ����
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		//��������һ��������
		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	//�����������Ʒ����Ϣ
	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();
	//��player����ȡ��ԭ�Ϻ�������
	for(unsigned int i = 0; i < 16; i ++)
	{
		if(mlist[i].item_id) 
		{
			int idx = idxs[i];

			item& it = _inventory[idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, mlist[i].count);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,mlist[i].count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx, mlist[i].count);
		}
	}
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		}
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}
		int rst = _inventory.Push(*data);
		if(rst >=0)
		{
		    FirstAcquireItem(data);

			//�������������Ʒ����Ϣ
			_runner->produce_once(item_id,rt.count - data->count,_inventory[rst].count, 0,rst);
		}

		if(data->count)
		{
			//ʣ������Ʒ
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
			//�����������Ҫ�ͷ��ڴ�
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			//���������ж�
			return false;
		}
		FreeItem(data);
		__PRINTF("�ɹ�����������Ʒ%d\n",item_id);
	}
	else
	{
		//��������δ�ɹ�����Ϣ
		_runner->produce_null(rt.recipe_id);
	}
	return true;
}

bool 
gplayer_imp::ProduceItem3(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
{
	//�������������Լ̳д�����װ���ľ���������ʯ������ѡ��ԭ�ϣ�bind_type��proc_type����Ч,bind_type���ȼ���
	//����Ǯ
	if(GetAllMoney() < rt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.equipment_need_upgrade <= 0) return false;
	if(rt.count != 1) return false;
	if(rt.material_total_count <= 0) return false;

	//�ж����ɺ�����Ʒ
	int item_id = 0;
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}
	//������������������������߻���Ʒ
	if(item_id <= 0) return false;
	DATA_TYPE target_eq_dt;
	const void * target_eq_ess = world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, target_eq_dt);
	if(!target_eq_ess || 
		target_eq_dt != DT_WEAPON_ESSENCE && 
			target_eq_dt != DT_ARMOR_ESSENCE && 
				target_eq_dt != DT_NEW_ARMOR_ESSENCE && // G17/G18
					target_eq_dt != DT_DECORATION_ESSENCE && 
					/*158+*/	target_eq_dt != DT_BIBLE_ESSENCE) return false;
	
	//�������Ƿ��㹻
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));

	//�����״̬
	int bind_result = 0;	//0 ���� 1 ��  2 װ�����
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;

	//������װ�����������ƷDATA_TYPE��ͬ
	if(equip_id != rt.equipment_need_upgrade) return false;
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) return false;
	item & eq_it = _inventory[equip_inv_idx];
	if(eq_it.body == NULL) return false;
	DATA_TYPE eq_dt;
	const void * eq_ess = world_manager::GetDataMan().get_data_ptr(eq_it.type, ID_SPACE_ESSENCE, eq_dt);
	if(!eq_ess || eq_dt != target_eq_dt) return false;
	if(eq_dt == DT_WEAPON_ESSENCE)
	{
		if(((WEAPON_ESSENCE*)eq_ess)->level > ((WEAPON_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_ARMOR_ESSENCE)
	{
		if(((ARMOR_ESSENCE*)eq_ess)->level > ((ARMOR_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
	{
		if(((NEW_ARMOR_ESSENCE*)eq_ess)->level > ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_DECORATION_ESSENCE)
	{
		if(((DECORATION_ESSENCE*)eq_ess)->level > ((DECORATION_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_BIBLE_ESSENCE) /*158+*/	
	{
		if(((BIBLE_ESSENCE*)eq_ess)->level > ((BIBLE_ESSENCE*)target_eq_ess)->level) return false;
	}

	int inherit_fee = 0;
	int eq_refine_level = 0;
	int eq_socket_count = 0;
	int eq_stone_type[4] = {0};
	addon_data eq_engrave_addon_list[3];
	unsigned int eq_engrave_addon_count = 0;
	if(inherit_type & PRODUCE_INHERIT_STONE) inherit_type |= PRODUCE_INHERIT_SOCKET;	//��̳б�ʯ����̳п���
	if(rt.inherit_fee_factor < 0.000001f) inherit_type |= (PRODUCE_INHERIT_REFINE | PRODUCE_INHERIT_SOCKET | PRODUCE_INHERIT_STONE); //û�м̳з���ʱ��ǿ�Ƽ̳о���������ʯ
	if(rt.inherit_engrave_fee_factor < 0.000001f) inherit_type |= PRODUCE_INHERIT_ENGRAVE;//û�м̳з���ʱ��ǿ�Ƽ̳��Կ�����
	DATA_TYPE inherit_cfg_dt;
	UPGRADE_PRODUCTION_CONFIG * inherit_cfg_ess = (UPGRADE_PRODUCTION_CONFIG *)world_manager::GetDataMan().get_data_ptr(694, ID_SPACE_CONFIG, inherit_cfg_dt);
	if(!inherit_cfg_ess || inherit_cfg_dt != DT_UPGRADE_PRODUCTION_CONFIG) return false;

	if(inherit_type & PRODUCE_INHERIT_REFINE && target_eq_dt != DT_BIBLE_ESSENCE)
	{
		int material_need;
		int refine_addon = world_manager::GetDataMan().get_item_refine_addon(eq_it.type, material_need);
		if(material_need > 0 && refine_addon > 0)
		{
			eq_refine_level = eq_it.body->GetRefineLevel(refine_addon);
			if(eq_refine_level > 0)
			{				
				ASSERT(eq_refine_level <= 12);
				inherit_fee += int(inherit_cfg_ess->num_refine[eq_refine_level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
	}

	if(inherit_type & PRODUCE_INHERIT_REFINE && target_eq_dt == DT_BIBLE_ESSENCE)
	{
		eq_refine_level = eq_it.body->GetRefineLevel();

		if(eq_refine_level > 12 || eq_refine_level <= 0) eq_refine_level = 0;
		if(eq_refine_level > 0)
		{
			ASSERT(eq_refine_level <= 12);
			inherit_fee += int(inherit_cfg_ess->num_refine[eq_refine_level-1] * rt.inherit_fee_factor + 0.5f);
		}	

	}

	if(inherit_type & PRODUCE_INHERIT_SOCKET)
	{
		if(eq_dt == DT_WEAPON_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 2 && ((WEAPON_ESSENCE*)target_eq_ess)->level >= 1 && ((WEAPON_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_weapon[eq_socket_count-1][((WEAPON_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
		else if(eq_dt == DT_ARMOR_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
		else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((NEW_ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
        else if (eq_dt == DT_DECORATION_ESSENCE)
        {
            eq_socket_count = eq_it.body->GetSocketCount();
            if (eq_socket_count > 0)
            {
                int decoration_level = ((DECORATION_ESSENCE*)target_eq_ess)->level;
                ASSERT((eq_socket_count <= 4) && (decoration_level >= 1) && (decoration_level <= 20));
                inherit_fee += int(inherit_cfg_ess->num_decoration[eq_socket_count - 1][decoration_level - 1] * rt.inherit_fee_factor + 0.5f);
            }
        }
	}

	if(inherit_type & PRODUCE_INHERIT_STONE && target_eq_dt != DT_BIBLE_ESSENCE)
	{
		//��ʯ�̳гɱ�
		if(eq_socket_count > 0)
		{
			for(int i=0; i<eq_socket_count; i++)
			{
				int stone_type = eq_it.body->GetSocketType(i);
				if(stone_type <= 0) continue;
				DATA_TYPE dt;
				STONE_ESSENCE * stone_ess = (STONE_ESSENCE *) world_manager::GetDataMan().get_data_ptr(stone_type,ID_SPACE_ESSENCE,dt);
				ASSERT(dt == DT_STONE_ESSENCE && stone_ess);
				ASSERT(stone_ess->level >= 1 && stone_ess->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_stone[stone_ess->level-1] * rt.inherit_fee_factor + 0.5f);
				eq_stone_type[i] = stone_type;
			}
		}
	}

	if(inherit_type & PRODUCE_INHERIT_ENGRAVE && target_eq_dt != DT_BIBLE_ESSENCE)
	{
		//�Կ̼̳з���
		eq_engrave_addon_count = eq_it.GetEngraveAddon(&eq_engrave_addon_list[0], sizeof(eq_engrave_addon_list)/sizeof(eq_engrave_addon_list[0]));
		if(eq_engrave_addon_count > 0)
		{
			ASSERT(eq_engrave_addon_count <= 3);
			inherit_fee += int(inherit_cfg_ess->num_engrave[eq_engrave_addon_count-1] * rt.inherit_engrave_fee_factor + 0.5f);
		}	
	}

	if(inherit_fee > 0 && !CheckItemExist(ALLSPARK_ID,inherit_fee)) 
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false; 
	}

	if(rt.bind_type == 2)
	{
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND)
		{
			if(eq_it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
				bind_result = 3;
			else
				bind_result = 1;
		}
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
			bind_result = 2;
	}

	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 16; i++)
	{
		if(rt.material_list[i].item_id)
		{
			if(rt.material_list[i].item_id != materials[i]) return false;
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], rt.material_list[i].count)
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
			mlist[i].item_id = materials[i];
			mlist[i].count = rt.material_list[i].count;

			if(rt.bind_type == 2)
			{
				int proc_type = _inventory[idx].proc_type;
				if( proc_type & item::ITEM_PROC_TYPE_BIND)
				{
					if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
						bind_result = 3;
					else
						bind_result = 1;
				}
				if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
					bind_result = 2;
			}
		}
	}
	
	item_data * data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
		unsigned int  len;
		const void * name;
		name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;

		data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
		if(!data)
		{
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < 16; ++i)
        {
            if (mlist[i].item_id > 0)
            {
                itembuf += buf;
            }
        }

        itembuf += buf;

    }

	if(data)
	{
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();

	item& it = _inventory[equip_inv_idx];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
	
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,equip_inv_idx,equip_id,1,S2C::DROP_TYPE_PRODUCE);
	_inventory.DecAmount(equip_inv_idx, 1);

	if(inherit_fee > 0)
		RemoveItems(ALLSPARK_ID, inherit_fee, S2C::DROP_TYPE_USE, true);

	for(unsigned int i = 0; i < 16; i ++)
	{
		if(mlist[i].item_id) 
		{
			int idx = idxs[i];

			item& it = _inventory[idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, mlist[idx].count);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,mlist[i].count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx, mlist[i].count);
		}
	}
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		} 
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}
		int rst = _inventory.Push(*data);
		if(rst >=0)
		{
		    FirstAcquireItem(data);


			item & target_eq_it = _inventory[rst];
			if(inherit_type & PRODUCE_INHERIT_REFINE && target_eq_dt != DT_BIBLE_ESSENCE)
			{
				if(eq_refine_level > 0)
				{
					int material_need;
					int refine_addon = world_manager::GetDataMan().get_item_refine_addon(target_eq_it.type, material_need);
					if(material_need > 0 && refine_addon > 0)
						target_eq_it.body->SetRefineLevel(refine_addon,eq_refine_level);		
				}

			} else if (inherit_type & PRODUCE_INHERIT_REFINE && target_eq_dt == DT_BIBLE_ESSENCE) /*158+*/
			{
				if(eq_refine_level > 0)
				{
					target_eq_it.body->TransmitRefine(eq_refine_level);		
				}
			}


			if(inherit_type & PRODUCE_INHERIT_SOCKET && target_eq_dt != DT_BIBLE_ESSENCE)
			{
				if(eq_socket_count > 0)
				{
					target_eq_it.body->SetSocketAndStone(eq_socket_count,eq_stone_type);	
				}
			}
			if(inherit_type & PRODUCE_INHERIT_ENGRAVE && target_eq_dt != DT_BIBLE_ESSENCE)
			{
				if(eq_engrave_addon_count > 0)
				{
					target_eq_it.Engrave(&eq_engrave_addon_list[0], eq_engrave_addon_count);
				}
			}
			
			_runner->produce_once(item_id,rt.count - data->count,_inventory[rst].count, 0,rst);
		}

		if(data->count)
		{
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}
		FreeItem(data);
	}
	else
	{
		_runner->produce_null(rt.recipe_id);
	}

	/*161+*/
	RefreshInventoryNewArmorEnter(true, false);
	return true;
}


bool 
gplayer_imp::ProduceItem4(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char & inherit_type, void **pitem, unsigned short &crc, int &eq_refine_level, int &eq_socket_count, int eq_stone_type[], addon_data eq_engrave_addon_list[3], unsigned int & eq_engrave_addon_count)
{
	//�¼��̳���������һ�����湦�ܣ���ҿ���ѡ���������Ի���ѡ��������
	if (*pitem)	//���pitemָ���ǲ���������
	{
		ASSERT(false);
		return false;
	}
	//����Ǯ
	if(GetAllMoney() < rt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.equipment_need_upgrade <= 0) return false;
	if(rt.count != 1) return false;
	if(rt.material_total_count <= 0) return false;

	int item_id = 0;
	//�ж����ɺ�����Ʒ
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}
	//������������������������߻���Ʒ
	if(item_id <= 0) return false;
	DATA_TYPE target_eq_dt;
	const void * target_eq_ess = world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, target_eq_dt);
	if(!target_eq_ess || target_eq_dt != DT_WEAPON_ESSENCE && target_eq_dt != DT_ARMOR_ESSENCE && target_eq_dt != DT_NEW_ARMOR_ESSENCE && target_eq_dt != DT_DECORATION_ESSENCE) return false;
	
	//�������Ƿ��㹻
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));

	//�����״̬
	int bind_result = 0;	//0 ���� 1 ��  2 װ�����
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;

	//������װ�����������ƷDATA_TYPE��ͬ
	if(equip_id != rt.equipment_need_upgrade) return false;
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) return false;
	item & eq_it = _inventory[equip_inv_idx];
	if(eq_it.body == NULL) return false;
	crc = eq_it.GetCRC();
	DATA_TYPE eq_dt;
	const void * eq_ess = world_manager::GetDataMan().get_data_ptr(eq_it.type, ID_SPACE_ESSENCE, eq_dt);
	if(!eq_ess || eq_dt != target_eq_dt) return false;
	if(eq_dt == DT_WEAPON_ESSENCE)
	{
		if(((WEAPON_ESSENCE*)eq_ess)->level > ((WEAPON_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_ARMOR_ESSENCE)
	{
		if(((ARMOR_ESSENCE*)eq_ess)->level > ((ARMOR_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
	{
		if(((NEW_ARMOR_ESSENCE*)eq_ess)->level > ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level) return false;
	}
	else if(eq_dt == DT_DECORATION_ESSENCE)
	{
		if(((DECORATION_ESSENCE*)eq_ess)->level > ((DECORATION_ESSENCE*)target_eq_ess)->level) return false;
	}
	//����̳д�����װ���ĵľ���������ʯ�ĳɱ�
	int inherit_fee = 0;
	if(inherit_type & PRODUCE_INHERIT_STONE) inherit_type |= PRODUCE_INHERIT_SOCKET;	//��̳б�ʯ����̳п���
	if(rt.inherit_fee_factor < 0.000001f) inherit_type |= (PRODUCE_INHERIT_REFINE | PRODUCE_INHERIT_SOCKET | PRODUCE_INHERIT_STONE); //û�м̳з���ʱ��ǿ�Ƽ̳о���������ʯ
	if(rt.inherit_engrave_fee_factor < 0.000001f) inherit_type |= PRODUCE_INHERIT_ENGRAVE;//û�м̳з���ʱ��ǿ�Ƽ̳��Կ�����
	DATA_TYPE inherit_cfg_dt;
	UPGRADE_PRODUCTION_CONFIG * inherit_cfg_ess = (UPGRADE_PRODUCTION_CONFIG *)world_manager::GetDataMan().get_data_ptr(694, ID_SPACE_CONFIG, inherit_cfg_dt);
	if(!inherit_cfg_ess || inherit_cfg_dt != DT_UPGRADE_PRODUCTION_CONFIG) return false;
	if(inherit_type & PRODUCE_INHERIT_REFINE)
	{
		//�����ȼ��̳гɱ�	
		int material_need;
		int refine_addon = world_manager::GetDataMan().get_item_refine_addon(eq_it.type, material_need);
		if(material_need > 0 && refine_addon > 0)
		{
			eq_refine_level = eq_it.body->GetRefineLevel(refine_addon);
			if(eq_refine_level > 0)
			{
				ASSERT(eq_refine_level <= 12);
				inherit_fee += int(inherit_cfg_ess->num_refine[eq_refine_level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
	}
	if(inherit_type & PRODUCE_INHERIT_SOCKET)
	{
		//�����̳гɱ�,����������2������4����Ʒ4
		if(eq_dt == DT_WEAPON_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 2 && ((WEAPON_ESSENCE*)target_eq_ess)->level >= 1 && ((WEAPON_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_weapon[eq_socket_count-1][((WEAPON_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
		else if(eq_dt == DT_ARMOR_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
		else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((NEW_ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
        else if (eq_dt == DT_DECORATION_ESSENCE)
        {
            eq_socket_count = eq_it.body->GetSocketCount();
            if (eq_socket_count > 0)
            {
                int decoration_level = ((DECORATION_ESSENCE*)target_eq_ess)->level;
                ASSERT((eq_socket_count <= 4) && (decoration_level >= 1) && (decoration_level <= 20));
                inherit_fee += int(inherit_cfg_ess->num_decoration[eq_socket_count - 1][decoration_level - 1] * rt.inherit_fee_factor + 0.5f);
            }
        }
	}
	if(inherit_type & PRODUCE_INHERIT_STONE)
	{
		//��ʯ�̳гɱ�
		if(eq_socket_count > 0)
		{
			for(int i=0; i<eq_socket_count; i++)
			{
				int stone_type = eq_it.body->GetSocketType(i);
				if(stone_type <= 0) continue;
				DATA_TYPE dt;
				STONE_ESSENCE * stone_ess = (STONE_ESSENCE *) world_manager::GetDataMan().get_data_ptr(stone_type,ID_SPACE_ESSENCE,dt);
				ASSERT(dt == DT_STONE_ESSENCE && stone_ess);
				ASSERT(stone_ess->level >= 1 && stone_ess->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_stone[stone_ess->level-1] * rt.inherit_fee_factor + 0.5f);
				eq_stone_type[i] = stone_type;
			}
		}
	}
	if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
	{
		//�Կ̼̳з���
		eq_engrave_addon_count = eq_it.GetEngraveAddon(&eq_engrave_addon_list[0], 3);
		if(eq_engrave_addon_count > 0)
		{
			ASSERT(eq_engrave_addon_count <= 3);
			inherit_fee += int(inherit_cfg_ess->num_engrave[eq_engrave_addon_count-1] * rt.inherit_engrave_fee_factor + 0.5f);
		}	
	}
	if(inherit_fee > 0 && !CheckItemExist(ALLSPARK_ID,inherit_fee)) 
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false; 
	}
	if(rt.bind_type == 2)
	{
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND)
		{
			if(eq_it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
				bind_result = 3;
			else
				bind_result = 1;
		}
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
			bind_result = 2;
	}

	//���в��ϼ��
	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 16; i++)
	{
		if(rt.material_list[i].item_id)
		{
			if(rt.material_list[i].item_id != materials[i]) return false;
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], rt.material_list[i].count)
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
			mlist[i].item_id = materials[i];
			mlist[i].count = rt.material_list[i].count;

			//�����״̬
			if(rt.bind_type == 2)
			{
				int proc_type = _inventory[idx].proc_type;
				if( proc_type & item::ITEM_PROC_TYPE_BIND)
				{
					if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
						bind_result = 3;
					else
						bind_result = 1;
				}
				if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
					bind_result = 2;
			}
		}
	}
	
	//������Ʒ����
	item_data * data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
		unsigned int  len;
		const void * name;
		name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;

		data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
		if(!data)
		{
			//��Ʒ����ʧ��
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < 16; ++i)
        {
            if (mlist[i].item_id > 0)
            {
                snprintf(buf, sizeof(buf), "����%d, ����%d; ", _inventory[idxs[i]].type, mlist[i].count);
                itembuf += buf;
            }
        }

        snprintf(buf, sizeof(buf), "����%d, ����%d.", equip_id, 1);
        itembuf += buf;

        GLog::log(GLOG_INFO, "�û�%d������%d��%d, �䷽%d, ����%s", _parent->ID.id, rt.count, item_id, rt.recipe_id, itembuf.c_str());
    }
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����%dʱδ�ɹ� �䷽%d",_parent->ID.id,item_id,rt.recipe_id);
	}

	if(data)
	{
		//��һЩ����
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		//��������һ��������
		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	//�����������Ʒ����Ϣ
	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();
	//ɾ���̳з���
	if(inherit_fee > 0)
		RemoveItems(ALLSPARK_ID, inherit_fee, S2C::DROP_TYPE_USE, true);
	//��player����ȡ��ԭ�Ϻ�������
	for(unsigned int i = 0; i < 16; i ++)
	{
		if(mlist[i].item_id) 
		{
			int idx = idxs[i];
			
			item& it = _inventory[idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, mlist[i].count);
				
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,mlist[i].count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx, mlist[i].count);
		}
	}
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		} 
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}

		*pitem = (void *)data;
		//������Ϣ֪ͨ�ͻ���
		_runner->produce4_item_info((int)g_timer.get_systime() + PRODUCE4_CHOOSE_ITEM_TIME, *data, 0);
		__PRINTF("�̳������ɹ�����������Ʒ%d\n",item_id);
	}
	else
	{
		//��������δ�ɹ�����Ϣ
		_runner->produce_null(rt.recipe_id);
	}
	return true;
}

bool
gplayer_imp::ProduceItem5(const recipe_template& rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
{
    //�����������Ե��������������Լ̳д�����װ���ľ���������ʯ������ѡ��ԭ�ϣ�bind_type��proc_type����Ч,bind_type���ȼ���
	//����Ǯ
	if(GetAllMoney() < rt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.equipment_need_upgrade <= 0) return false;
	if(rt.count != 1) return false;
	if(rt.material_total_count <= 0) return false;

	//�ж����ɺ�����Ʒ
	int item_id = 0;
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}
	//������������������������߻���Ʒ
	if(item_id <= 0) return false;
    
	DATA_TYPE target_eq_dt;
	const void* target_eq_ess = world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, target_eq_dt);
	if(!target_eq_ess || target_eq_dt != DT_WEAPON_ESSENCE && target_eq_dt != DT_ARMOR_ESSENCE && target_eq_dt != DT_NEW_ARMOR_ESSENCE && target_eq_dt != DT_DECORATION_ESSENCE) return false;
	
	//�������Ƿ��㹻
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist, rt.material_list, sizeof(mlist));

	//�����״̬
	int bind_result = 0;	//0 ���� 1 ��  2 װ�����
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;
    
    //������װ�����������ƷDATA_TYPE��ͬ
	if(equip_id != rt.equipment_need_upgrade) return false;
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) return false;
	item& eq_it = _inventory[equip_inv_idx];
	if(eq_it.body == NULL) return false;
    
	DATA_TYPE eq_dt;
	const void* eq_ess = world_manager::GetDataMan().get_data_ptr(eq_it.type, ID_SPACE_ESSENCE, eq_dt);
	if(!eq_ess || eq_dt != target_eq_dt) return false;
    
    if(rt.inherit_addon_fee_factor < 0.000001f) inherit_type |= PRODUCE_INHERIT_ADDON; //û�м̳з��ã�ǿ�Ƽ̳и�������
	if(eq_dt == DT_WEAPON_ESSENCE)
	{
		if(((WEAPON_ESSENCE*)eq_ess)->level > ((WEAPON_ESSENCE*)target_eq_ess)->level) return false;
        if( (inherit_type & PRODUCE_INHERIT_ADDON) && ((WEAPON_ESSENCE*)target_eq_ess)->fixed_props ) return false;
	}
	else if(eq_dt == DT_ARMOR_ESSENCE)
	{
		if(((ARMOR_ESSENCE*)eq_ess)->level > ((ARMOR_ESSENCE*)target_eq_ess)->level) return false;
        if( (inherit_type & PRODUCE_INHERIT_ADDON) && ((ARMOR_ESSENCE*)target_eq_ess)->fixed_props) return false;
	}
	else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
	{
		if(((NEW_ARMOR_ESSENCE*)eq_ess)->level > ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level) return false;
        if( (inherit_type & PRODUCE_INHERIT_ADDON) && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->fixed_props) return false;
	}
	else if(eq_dt == DT_DECORATION_ESSENCE)
	{
		if(((DECORATION_ESSENCE*)eq_ess)->level > ((DECORATION_ESSENCE*)target_eq_ess)->level) return false;
        if( (inherit_type & PRODUCE_INHERIT_ADDON) && ((DECORATION_ESSENCE*)target_eq_ess)->fixed_props) return false;
	}
    
	//����̳д�����װ���ĵľ���������ʯ�ĳɱ�
	int inherit_fee = 0;
    int eq_refine_addon_id = 0;
    int eq_refine_material_need = 0;
	int eq_refine_level = 0;
	int eq_socket_count = 0;
	int eq_stone_type[4] = {0};
	addon_data eq_engrave_addon_list[3];
	unsigned int eq_engrave_addon_count = 0;
    addon_data need_inherit_addon_list[5];
    unsigned int need_inherit_addon_count = 0;
    
	if(inherit_type & PRODUCE_INHERIT_STONE) inherit_type |= PRODUCE_INHERIT_SOCKET;	//��̳б�ʯ����̳п���
	if(rt.inherit_fee_factor < 0.000001f) inherit_type |= (PRODUCE_INHERIT_REFINE | PRODUCE_INHERIT_SOCKET | PRODUCE_INHERIT_STONE); //û�м̳з���ʱ��ǿ�Ƽ̳о���������ʯ
	if(rt.inherit_engrave_fee_factor < 0.000001f) inherit_type |= PRODUCE_INHERIT_ENGRAVE;//û�м̳з���ʱ��ǿ�Ƽ̳��Կ�����
    
	DATA_TYPE inherit_cfg_dt;
	UPGRADE_PRODUCTION_CONFIG* inherit_cfg_ess = (UPGRADE_PRODUCTION_CONFIG*)world_manager::GetDataMan().get_data_ptr(694, ID_SPACE_CONFIG, inherit_cfg_dt);
	if(!inherit_cfg_ess || inherit_cfg_dt != DT_UPGRADE_PRODUCTION_CONFIG) return false;
    
	if(inherit_type & PRODUCE_INHERIT_REFINE)
	{
		//�����ȼ��̳гɱ�	
		eq_refine_addon_id = world_manager::GetDataMan().get_item_refine_addon(eq_it.type, eq_refine_material_need);
		if(eq_refine_material_need > 0 && eq_refine_addon_id > 0)
		{
			eq_refine_level = eq_it.body->GetRefineLevel(eq_refine_addon_id);
			if(eq_refine_level > 0)
			{
				ASSERT(eq_refine_level <= 12);
				inherit_fee += int(inherit_cfg_ess->num_refine[eq_refine_level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
	}
	if(inherit_type & PRODUCE_INHERIT_SOCKET)
	{
		//�����̳гɱ�,����������2������4����Ʒ4
		if(eq_dt == DT_WEAPON_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 2 && ((WEAPON_ESSENCE*)target_eq_ess)->level >= 1 && ((WEAPON_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_weapon[eq_socket_count-1][((WEAPON_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
		else if(eq_dt == DT_ARMOR_ESSENCE)
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
		else if(eq_dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
		{
			eq_socket_count = eq_it.body->GetSocketCount();
			if(eq_socket_count > 0)
			{
				ASSERT(eq_socket_count <= 4 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level >= 1 && ((NEW_ARMOR_ESSENCE*)target_eq_ess)->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count-1][((NEW_ARMOR_ESSENCE*)target_eq_ess)->level-1] * rt.inherit_fee_factor + 0.5f);
			}	
		}
        else if (eq_dt == DT_DECORATION_ESSENCE)
        {
            eq_socket_count = eq_it.body->GetSocketCount();
            if (eq_socket_count > 0)
            {
                int decoration_level = ((DECORATION_ESSENCE*)target_eq_ess)->level;
                ASSERT((eq_socket_count <= 4) && (decoration_level >= 1) && (decoration_level <= 20));
                inherit_fee += int(inherit_cfg_ess->num_decoration[eq_socket_count - 1][decoration_level - 1] * rt.inherit_fee_factor + 0.5f);
            }
        }
	}
	if(inherit_type & PRODUCE_INHERIT_STONE)
	{
		//��ʯ�̳гɱ�
		if(eq_socket_count > 0)
		{
			for(int i=0; i<eq_socket_count; i++)
			{
				int stone_type = eq_it.body->GetSocketType(i);
				if(stone_type <= 0) continue;
				DATA_TYPE dt;
				STONE_ESSENCE* stone_ess = (STONE_ESSENCE*) world_manager::GetDataMan().get_data_ptr(stone_type, ID_SPACE_ESSENCE, dt);
				ASSERT(dt == DT_STONE_ESSENCE && stone_ess);
				ASSERT(stone_ess->level >= 1 && stone_ess->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_stone[stone_ess->level-1] * rt.inherit_fee_factor + 0.5f);
				eq_stone_type[i] = stone_type;
			}
		}
	}
	if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
	{
		//�Կ̼̳з���
		eq_engrave_addon_count = eq_it.GetEngraveAddon(&eq_engrave_addon_list[0], sizeof(eq_engrave_addon_list)/sizeof(eq_engrave_addon_list[0]));
		if(eq_engrave_addon_count > 0)
		{
			ASSERT(eq_engrave_addon_count <= 3);
			inherit_fee += int(inherit_cfg_ess->num_engrave[eq_engrave_addon_count-1] * rt.inherit_engrave_fee_factor + 0.5f);
		}	
	}
    if(inherit_type & PRODUCE_INHERIT_ADDON)
    {
        //�������Լ̳з���
        need_inherit_addon_count = eq_it.GetCanInheritAddon(&need_inherit_addon_list[0], sizeof(need_inherit_addon_list)/sizeof(need_inherit_addon_list[0]), eq_refine_addon_id);
        if(need_inherit_addon_count > 0)
        {
            ASSERT(need_inherit_addon_count <= 5);
            inherit_fee += int(inherit_cfg_ess->num_addon[need_inherit_addon_count-1] * rt.inherit_addon_fee_factor + 0.5f);
        }   
    }
    
	if(inherit_fee > 0 && !CheckItemExist(ALLSPARK_ID, inherit_fee)) 
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false; 
	}

	if(rt.bind_type == 2)
	{
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND)
		{
			if(eq_it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
				bind_result = 3;
			else
				bind_result = 1;
		}
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
			bind_result = 2;
	}

	//���в��ϼ��
	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 16; i++)
	{
		if(rt.material_list[i].item_id)
		{
			if(rt.material_list[i].item_id != materials[i]) return false;
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], rt.material_list[i].count)
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
			mlist[i].item_id = materials[i];
			mlist[i].count = rt.material_list[i].count;

			//�����״̬
			if(rt.bind_type == 2)
			{
				int proc_type = _inventory[idx].proc_type;
				if( proc_type & item::ITEM_PROC_TYPE_BIND)
				{
					if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
						bind_result = 3;
					else
						bind_result = 1;
				}
				if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
					bind_result = 2;
			}
		}
	}
	
	//������Ʒ����
	item_data* data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
        
		unsigned int len = 0;
		const void* name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;
        
        if(inherit_type & 0x10) //�̳и������ԣ�Ҫ������һ�������κθ������Ե���Ʒ
        {
            data = world_manager::GetDataMan().generate_item_for_shop(item_id, &tag, sizeof(short) + len);
        } 
        else 
        {
            data = world_manager::GetDataMan().generate_item_from_player(item_id, &tag,sizeof(short) + len);
        }
        
		if(!data)
		{
			//��Ʒ����ʧ��
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < 16; ++i)
        {
            if (mlist[i].item_id > 0)
            {
                snprintf(buf, sizeof(buf), "����%d, ����%d; ", _inventory[idxs[i]].type, mlist[i].count);
                itembuf += buf;
            }
        }

        snprintf(buf, sizeof(buf), "����%d, ����%d.", equip_id, 1); 
        itembuf += buf;

        GLog::log(GLOG_INFO, "�û�%d������%d��%d, �䷽%d, ����%s", _parent->ID.id, rt.count, item_id, rt.recipe_id, itembuf.c_str());
    }
	else
	{
		GLog::log(GLOG_INFO,"�û�%d����%dʱδ�ɹ� �䷽%d",_parent->ID.id,item_id,rt.recipe_id);
	}

	if(data)
	{
		//��һЩ����
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		//��������һ��������
		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	//�����������Ʒ����Ϣ
	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();
    
	//ɾ��������װ���Լ��̳з���
	item& it = _inventory[equip_inv_idx];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
	
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,equip_inv_idx,equip_id,1,S2C::DROP_TYPE_PRODUCE);
	_inventory.DecAmount(equip_inv_idx, 1);
	if(inherit_fee > 0)
		RemoveItems(ALLSPARK_ID, inherit_fee, S2C::DROP_TYPE_USE, true);
	//��player����ȡ��ԭ�Ϻ�������
	for(unsigned int i = 0; i < 16; i ++)
	{
		if(mlist[i].item_id) 
		{
			int idx = idxs[i];

			item& it = _inventory[idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, mlist[idx].count);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,mlist[i].count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx, mlist[i].count);
		}
	}
    
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		} 
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}

		int rst = _inventory.Push(*data);
		if(rst >=0)
		{
		    FirstAcquireItem(data);

			item& target_eq_it = _inventory[rst];
			if(inherit_type & PRODUCE_INHERIT_REFINE)
			{
				//�̳д�����װ���ľ���
				if(eq_refine_level > 0)
				{
					int material_need;
					int refine_addon = world_manager::GetDataMan().get_item_refine_addon(target_eq_it.type, material_need);
					if(material_need > 0 && refine_addon > 0)
						target_eq_it.body->SetRefineLevel(refine_addon,eq_refine_level);		
				}
			} 
			if(inherit_type & PRODUCE_INHERIT_SOCKET)
			{
				//�̳д�����װ���Ŀ���
				//�̳д�����װ���ı�ʯ
				if(eq_socket_count > 0)
				{
					target_eq_it.body->SetSocketAndStone(eq_socket_count,eq_stone_type);	
				}
			}
			if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
			{
				//�̳д�����װ�����Կ�����
				if(eq_engrave_addon_count > 0)
				{
					target_eq_it.Engrave(&eq_engrave_addon_list[0], eq_engrave_addon_count);
				}
			}
            if(inherit_type & PRODUCE_INHERIT_ADDON)
			{
                //�̳д�����װ���ĸ�������
                if(need_inherit_addon_count > 0)
                {
                    ASSERT(need_inherit_addon_count <= 5);
                    target_eq_it.InheritAddon(&need_inherit_addon_list[0], need_inherit_addon_count);
                }    
            }
            
			//�������������Ʒ����Ϣ
			_runner->produce_once(item_id,rt.count - data->count,_inventory[rst].count, 0,rst);
		}

		if(data->count)
		{
			//ʣ������Ʒ
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
			//�����������Ҫ�ͷ��ڴ�
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			//���������ж�
			return false;
		}
		FreeItem(data);
		__PRINTF("�ɹ�����������Ʒ%d\n",item_id);
	}
	else
	{
		//��������δ�ɹ�����Ϣ
		_runner->produce_null(rt.recipe_id);
	}

    return true;
}

bool
gplayer_imp::EquipSign(int ink_inv_idx, int ink_id, int equip_inv_idx, int equip_id, const char * signature, unsigned int signature_len)
{
	int ink_num = signature_len/2 > EQUIP_SIGNATURE_CLEAN_CONSUME ? signature_len/2 : EQUIP_SIGNATURE_CLEAN_CONSUME;
	//���īˮ�Ƿ����
	if(!_inventory.IsItemExist(ink_inv_idx, ink_id, ink_num)) return false; 

	//�����Ҫ����ǩ����װ���Ƿ����
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) return false;

	//���īˮ���ڱ���λ�õ���Ʒ�Ƿ�Ϊīˮ
	DATA_TYPE dt;
	DYE_TICKET_ESSENCE * ess = (DYE_TICKET_ESSENCE*) world_manager::GetDataMan().get_data_ptr(ink_id, ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_DYE_TICKET_ESSENCE) return false;

	if (ess->usage != 1) return false;

	//���װ�����ڱ���λ�õ���Ʒ�Ƿ�Ϊ����װ����Ʒ
	DATA_TYPE target_eq_dt;
	const void * target_eq_ess = world_manager::GetDataMan().get_data_ptr(equip_id, ID_SPACE_ESSENCE, target_eq_dt);
	if(!target_eq_ess || (target_eq_dt != DT_WEAPON_ESSENCE && target_eq_dt != DT_ARMOR_ESSENCE && target_eq_dt != DT_NEW_ARMOR_ESSENCE &&
			target_eq_dt != DT_DECORATION_ESSENCE && target_eq_dt != DT_FLYSWORD_ESSENCE))
		return false;

	//����īˮ��ɫ
	float h,s,v;
	h = abase::Rand(ess->h_min, ess->h_max);
	s = abase::Rand(ess->s_min, ess->s_max);
	v = abase::Rand(ess->v_min, ess->v_max);
	int color = hsv2rgb(h,s,v);

	unsigned short r = ((color >> 16) >> 3) & 0x1F;
	unsigned short g = ((color >> 8) >> 3) & 0x1F;
	unsigned short b = (color >> 3) & 0x1F;
	
	//��ʼ�޸�װ��ǩ��
	item & it = _inventory[equip_inv_idx];
	if (!it.body)	return false;
	if (!it.body->Sign(((r << 10) | (g << 5) | b) & 0x7FFF, signature, signature_len)) return false;

	//�۳�īˮ
	item & i_it = _inventory[ink_inv_idx];    
	UpdateMallConsumptionDestroying(i_it.type, i_it.proc_type, ink_num);
		
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,ink_inv_idx,ink_id,ink_num,S2C::DROP_TYPE_USE);
	_inventory.DecAmount(ink_inv_idx, ink_num);

	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,equip_inv_idx);
	return true;
}

bool
gplayer_imp::EngraveItem(const engrave_recipe_template& ert, unsigned int inv_index)
{
	if(inv_index >= _inventory.Size()) return false;
	item & it = _inventory[inv_index];
	if(it.type <= 0 || it.body == NULL) return false;
	//���װ������
	if(!(it.GetEquipMask64() & ert.equip_mask)) return false;
	int level = world_manager::GetDataMan().get_item_level(it.type);
	if(level < ert.equip_level_min || level > ert.equip_level_max) return false;
	//���ԭ��
	engrave_recipe_template::__material mlist[8];
	ASSERT(sizeof(mlist) == sizeof(ert.material_list));
	memcpy(mlist,ert.material_list,sizeof(mlist));
	int num = ert.material_num;
	int total_count = ert.material_total_count;
	
	int nlist[_inventory.Size()];
	memset(nlist,0,sizeof(nlist));
	
	for(unsigned int i=0; i<_inventory.Size(); i++)
	{
		int type = _inventory[i].type;
		if(type == -1) continue;
		for(int j= 0; j < num; j++)
		{
			if(mlist[j].item_id == type)
			{
				unsigned int count = _inventory[i].count;
				if(count > mlist[j].count) count = mlist[j].count;
				nlist[i] = count;
				if(!(mlist[j].count -= count))
				{
					std::swap(mlist[j],mlist[num-1]);
					num --;
				}
				total_count -= count;
				break;
			}
		}
		if(total_count == 0) break;
	}
	ASSERT(total_count >= 0);
	if(total_count > 0)
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false;
	}

	//��������
	addon_data addon_list[3];
	unsigned int addon_count;
	addon_count = abase::RandSelect(ert.prob_addon_num,4);
	for(unsigned int i=0; i<addon_count; i++)
	{
		int r = abase::RandSelect(&ert.target_addons[0].prob,sizeof(ert.target_addons[0]),32);
		int addon_id = ert.target_addons[r].addon_id;
		if(!world_manager::GetDataMan().generate_addon(addon_id,addon_list[i])) return false;
	}
	//�Կ�
	if(!it.Engrave(addon_list,addon_count))
	{
		return false;
	}
	PlayerGetItemInfo(IL_INVENTORY, inv_index);
	_runner->engrave_result(addon_count);
	//ȡ��ԭ��
	for(unsigned int i = 0; i < _inventory.Size(); i ++)
	{
		if(nlist[i]) 
		{
			item& it = _inventory[i];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, nlist[i]);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,i,_inventory[i].type,nlist[i],S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(i,nlist[i]);
		}
	}
	return true;
}

bool
gplayer_imp::ItemAddonRegen(const addonregen_recipe_template& arrt, unsigned int inv_index)
{
	if(inv_index >= _inventory.Size()) return false;
	item & it = _inventory[inv_index];
	if(it.type <= 0 || it.body == NULL) return false;
	//���װ������
	unsigned int i=0;
	for(; i<sizeof(arrt.equip_id_list)/sizeof(arrt.equip_id_list[0]); i++)
		if(it.type == arrt.equip_id_list[i]) break;
	if(i == sizeof(arrt.equip_id_list)/sizeof(arrt.equip_id_list[0])) return false;
	//����Ǯ
	if(GetAllMoney() < arrt.fee)
	{
		//����һ���ж���Ϣ
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
	//���ԭ��
	addonregen_recipe_template::__material mlist[8];
	ASSERT(sizeof(mlist) == sizeof(arrt.material_list));
	memcpy(mlist,arrt.material_list,sizeof(mlist));
	int num = arrt.material_num;
	int total_count = arrt.material_total_count;
	
	int nlist[_inventory.Size()];
	memset(nlist,0,sizeof(nlist));
	
	for(unsigned int i=0; i<_inventory.Size(); i++)
	{
		int type = _inventory[i].type;
		if(type == -1) continue;
		for(int j= 0; j < num; j++)
		{
			if(mlist[j].item_id == type)
			{
				unsigned int count = _inventory[i].count;
				if(count > mlist[j].count) count = mlist[j].count;
				nlist[i] = count;
				if(!(mlist[j].count -= count))
				{
					std::swap(mlist[j],mlist[num-1]);
					num --;
				}
				total_count -= count;
				break;
			}
		}
		if(total_count == 0) break;
	}
	ASSERT(total_count >= 0);
	if(total_count > 0)
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false;
	}
	//��������
	int regen_cnt;
	if( (regen_cnt = it.RegenInherentAddon()) <= 0)
	{
		_runner->addonregen_result(regen_cnt);
		return false;
	}
	PlayerGetItemInfo(IL_INVENTORY, inv_index);
	_runner->addonregen_result(regen_cnt);
	//��ȡ��ҽ�Һ�ԭ��
	if(arrt.fee)
	{
		SpendAllMoney(arrt.fee,true);
		SelfPlayerMoney();
	}
	for(unsigned int i = 0; i < _inventory.Size(); i ++)
	{
		if(nlist[i]) 
		{
			item& it = _inventory[i];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, nlist[i]);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,i,_inventory[i].type,nlist[i],S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(i,nlist[i]);
		}
	}
	return true;
}

void
gplayer_imp::ChangeInventorySize(int size)
{
	if(size > ITEM_LIST_MAX_SIZE) size = ITEM_LIST_MAX_SIZE;
	if(size < ITEM_LIST_BASE_SIZE) size = ITEM_LIST_BASE_SIZE;
	_inventory.SetSize(size);
	_task_inventory.SetSize(size);
	_runner->player_change_inventory_size(_inventory.Size());
}

bool 
gplayer_imp::UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, unsigned int arg_size)
{
	item & it = inv[inv_index];
	bool bBroadcast = it.IsBroadcastUseMsg();
	bool bBCArg = it.IsBroadcastArgUseMsg();
	int type = it.type;
	int proc_type = it.proc_type;
	int count = 0;
	int rst = inv.UseItemWithArg(inv_index,this,count,arg, arg_size);
	if(rst >= 0)
	{
		ASSERT(rst == item_type);
		UpdateMallConsumptionDestroying(type, proc_type, count);
		_runner->use_item(where,inv_index, item_type,count,arg,arg_size);
		if(bBroadcast)
		{
			if(bBCArg)
			{
				_runner->use_item(item_type, arg, arg_size);
			}
			else
			{
				_runner->use_item(item_type);
			}
		}
		return true;
	}
	return false;
}

bool
gplayer_imp::PlayerUseItemWithArg(int where,unsigned int inv_index,int item_type,unsigned int count, const char * buf, unsigned int buf_size)
{
	DATA_TYPE dt;
	EASY_PRODUCE_ITEM_ESSENCE *ess = (EASY_PRODUCE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_type,ID_SPACE_ESSENCE,dt);
	if(ess && dt == DT_EASY_PRODUCE_ITEM_ESSENCE)
	{		
		PlayerUseItem2(where,inv_index,item_type,count);
		return true;
	}

	if(where != IL_INVENTORY && where != IL_EQUIPMENT) return false;
	if(count == 0) return false;
	item_list & inv = GetInventory(where);
	if(inv_index >= inv.Size() || item_type == -1 ||
			inv[inv_index].type != item_type) return false;
	if(!inv[inv_index].CanUseWithArg(inv.GetLocation(),buf_size)) return false;
	int rst = inv[inv_index].GetUseDuration();
	if( rst < 0)
	{
		
		int tag = 1;
		int waypoint = (*(int*)buf) & 0xFFFF;
		abase::vector<TRANS_TARGET_SERV> & waypoint_list = globaldata_gettranstargetsserver();
		for(unsigned int j = 0; j < waypoint_list.size(); j ++)
		{
			if(waypoint_list[j].id == waypoint)
			{
				tag = waypoint_list[j].world_tag;
			}
		}
		
		LuaManager * lua = LuaManager::GetInstance();
		if(!lua->IsTrue(item_type, lua->GetConfig()->ITEM_SCROLL_TOWN) && tag != 1)
		return false;
		
		UseItemWithArg(inv,inv_index,where,item_type,buf , buf_size);
		return true;
	}
	else
	{
		//AddStartSession(new session_use_item(this,where,inv_index,item_type,count,rst));
		//$$$$$$$$$$  ������ʱû��ִ��ʱ��Ĳ���ʹ����Ʒ
		return true;
	}
}

unsigned int gplayer_imp::OI_GetMallOrdersCount()
{
	return _mall_invoice.size();
}

int gplayer_imp::OI_GetMallOrders(GDB::shoplog * list, unsigned int size)
{
	if(size < _mall_invoice.size()) return 0;
	for(unsigned int i = 0;i  < _mall_invoice.size(); i ++)
	{
		const netgame::mall_invoice & mi =  _mall_invoice[i];
		GDB::shoplog &sp =list[i];

		sp.order_id     = mi.order_id;
		sp.item_id      = mi.item_id;
		sp.expire       = mi.expire_date;
		sp.item_count   = mi.item_count;
		sp.order_count  = 1;
		sp.cash_need    = mi.price;
		sp.time         = mi.timestamp;
		sp.guid1        = mi.guid1;
		sp.guid2        = mi.guid2;
	}
	return (int)_mall_invoice.size();
}

int  
gplayer_imp::DyeFashionItem(unsigned int f_index, unsigned int d_index)
{
	//�����Ʒ�Ƿ����
	if(f_index >= _inventory.Size() || d_index >= _inventory.Size()) return S2C::ERR_DYE_FAILED;
	item & fashion_item = _inventory[f_index];
	item & dye_item = _inventory[d_index];
	if(fashion_item.type == -1 || dye_item.type == -1) return S2C::ERR_DYE_FAILED;
	
	//�����Ʒ�Ƿ�ʱװ ��Ⱦɫ��
	itemdataman & dataman =  world_manager::GetDataMan();
	DATA_TYPE dt;
	FASHION_ESSENCE * fess = (FASHION_ESSENCE*)dataman.get_data_ptr(fashion_item.type, ID_SPACE_ESSENCE,dt);
	if(fess == NULL || dt != DT_FASHION_ESSENCE) return S2C::ERR_DYE_FAILED;
	DYE_TICKET_ESSENCE * dess = (DYE_TICKET_ESSENCE*)dataman.get_data_ptr(dye_item.type, ID_SPACE_ESSENCE,dt);
	if(dess == NULL || dt != DT_DYE_TICKET_ESSENCE) return S2C::ERR_DYE_FAILED;

	if (dess->usage != 0) return S2C::ERR_DYE_FAILED;

	//���ʱװ�Ƿ����Ⱦɫ
	if(fess->require_dye_count <= 0) return S2C::ERR_FASHION_CAN_NOT_BE_DYED;

	//���Ⱦɫ���Ƿ��㹻
	if(!_inventory.IsItemExist(dye_item.type, fess->require_dye_count)) return S2C::ERR_DYE_NOT_ENOUGH;

	float h,s,v;
	h = abase::Rand(dess->h_min, dess->h_max);
	s = abase::Rand((float)pow(dess->s_min,2), (float)pow(dess->s_max,2));
	v = abase::Rand((float)pow(dess->v_min,3), (float)pow(dess->v_max,3));
	
	s = sqrtf(s);
	v = pow(v,1.0/3); 
	
	if(s > dess->s_max) s = dess->s_max; else if(s < dess->s_min) s = dess->s_min;
	if(v > dess->v_max) v = dess->v_max; else if(v < dess->v_min) v = dess->v_min;

	int color = hsv2rgb(h,s,v);

	unsigned short r = ((color >> 16) >> 3) & 0x1F;
	unsigned short g = ((color >> 8) >> 3) & 0x1F;
	unsigned short b = (color >> 3) & 0x1F;
    fashion_item.DyeItem(((r << 10) | (g << 5) | b) & 0x7FFF);

	//ɾ����Ʒ
	RemoveItems(dye_item.type,fess->require_dye_count, S2C::DROP_TYPE_USE, true);

	//֪ͨ�ͻ�������Ʒ����
	PlayerGetItemInfo(IL_INVENTORY, f_index);

	//��ȷ����
	return 0;
}

int 
gplayer_imp::DyeSuitFashionItem(unsigned char inv_idx_body,unsigned char inv_idx_leg,unsigned char inv_idx_foot,unsigned char inv_idx_wrist,unsigned char inv_idx_dye)
{
	//���ʱװ����������Ⱦɫ����		
	if(inv_idx_body == 255 && inv_idx_leg == 255 && inv_idx_foot == 255 && inv_idx_wrist == 255) return S2C::ERR_DYE_FAILED;
	
	int require_dye_count = 0;
	int count;
	if(inv_idx_body != 255)
	{
		count = GetRequireDyeCount(inv_idx_body,item::EQUIP_INDEX_FASHION_BODY);
		if(count <= 0) return S2C::ERR_DYE_FAILED;
		require_dye_count += count;
	}
	if(inv_idx_leg != 255)
	{
		count = GetRequireDyeCount(inv_idx_leg,item::EQUIP_INDEX_FASHION_LEG);
		if(count <= 0) return S2C::ERR_DYE_FAILED;
		require_dye_count += count;
	}
	if(inv_idx_foot != 255)
	{
		count = GetRequireDyeCount(inv_idx_foot,item::EQUIP_INDEX_FASHION_FOOT);
		if(count <= 0) return S2C::ERR_DYE_FAILED;
		require_dye_count += count;
	}
	if(inv_idx_wrist != 255)
	{
		count = GetRequireDyeCount(inv_idx_wrist,item::EQUIP_INDEX_FASHION_WRIST);
		if(count <= 0) return S2C::ERR_DYE_FAILED;
		require_dye_count += count;
	}

	//���Ⱦɫ������
	if(inv_idx_dye >= _inventory.Size()) return S2C::ERR_DYE_FAILED;
	item & dye_item = _inventory[inv_idx_dye];
	if(dye_item.type <= 0) return S2C::ERR_DYE_FAILED;
	DATA_TYPE dt;
	DYE_TICKET_ESSENCE * dess = (DYE_TICKET_ESSENCE*)world_manager::GetDataMan().get_data_ptr(dye_item.type, ID_SPACE_ESSENCE,dt);
	if(dess == NULL || dt != DT_DYE_TICKET_ESSENCE) return S2C::ERR_DYE_FAILED;

	if (dess->usage != 0) return S2C::ERR_DYE_FAILED;

	require_dye_count = require_dye_count*2;
	if(!CheckItemExist(dye_item.type, require_dye_count)) return S2C::ERR_DYE_NOT_ENOUGH;

	//����Ⱦɫ
	float h,s,v;
	h = abase::Rand(dess->h_min, dess->h_max);
	s = abase::Rand((float)pow(dess->s_min,2), (float)pow(dess->s_max,2));
	v = abase::Rand((float)pow(dess->v_min,3), (float)pow(dess->v_max,3));
	
	s = sqrtf(s);
	v = pow(v,1.0/3); 
	
	if(s > dess->s_max) s = dess->s_max; else if(s < dess->s_min) s = dess->s_min;
	if(v > dess->v_max) v = dess->v_max; else if(v < dess->v_min) v = dess->v_min;

	int color = hsv2rgb(h,s,v);

	unsigned short r = ((color >> 16) >> 3) & 0x1F;
	unsigned short g = ((color >> 8) >> 3) & 0x1F;
	unsigned short b = (color >> 3) & 0x1F;
	
	//ɾ��Ⱦɫ��
	RemoveItems(dye_item.type,require_dye_count, S2C::DROP_TYPE_USE, true);

	//Ⱦɫ��֪ͨ�ͻ���
	if(inv_idx_body != 255)
	{
        _inventory[inv_idx_body].DyeItem(((r << 10) | (g << 5) | b) & 0x7FFF);
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_body);
	}
	if(inv_idx_leg != 255)
	{
        _inventory[inv_idx_leg].DyeItem(((r << 10) | (g << 5) | b) & 0x7FFF);
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_leg);
	}
	if(inv_idx_foot != 255)
	{
        _inventory[inv_idx_foot].DyeItem(((r << 10) | (g << 5) | b) & 0x7FFF);
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_foot);
	}
	if(inv_idx_wrist != 255)
	{
        _inventory[inv_idx_wrist].DyeItem(((r << 10) | (g << 5) | b) & 0x7FFF);
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_wrist);
	}


	return 0;
}

int 
gplayer_imp::GetRequireDyeCount(unsigned char inv_idx, int equip_index)
{
	if(inv_idx >= _inventory.Size()) return -1;
	
	item & fashion_item = _inventory[inv_idx];
	if(fashion_item.type <= 0) return -1;
	if(!fashion_item.CheckEquipPostion(equip_index)) return -1;
	
	DATA_TYPE dt;
	FASHION_ESSENCE * fess = (FASHION_ESSENCE*)world_manager::GetDataMan().get_data_ptr(fashion_item.type, ID_SPACE_ESSENCE,dt);
	if(fess == NULL || dt != DT_FASHION_ESSENCE) return -1;
	return fess->require_dye_count;
}

void 
gplayer_imp::FindCheater2()
{
	if(_cheat_report) return;
	_cheat_report = 1;
	GMSV::ReportCheater2Gacd(_parent->ID.id, 1, NULL,0);

	A3DVECTOR pos = _parent->pos;
	GLog::log(LOG_INFO,"�û�%d����ʹ���ѻ���ң���ǰ����(%f,%f,%f)���ѻ㱨������ҷ�����", _parent->ID.id,pos.x, pos.y, pos.z);
}

void 
gplayer_imp::FindCheater(int type, bool noreport)
{
	if(!world_manager::AntiCheat()) return;
	_cooldown.SetCoolDown(COOLDOWN_INDEX_CHEATER,3600*1000);
	if(_cheat_mode) return ;	//�Ѿ���������״̬�Ͳ��ڼ�����
	
	_cheat_mode = abase::Rand(80, 127);
	__PRINTF("����%d���� %d\n", _parent->ID.id, type);
	if(!noreport)
	{
		GMSV::ReportCheater2Gacd(_parent->ID.id, type, NULL,0);
	}
	A3DVECTOR pos = _parent->pos;
	GLog::log(LOG_INFO,"�û�%d����ʹ�����׹���%d��ǰ����(%f,%f,%f)%d������ͷ�״̬", _parent->ID.id, type, pos.x, pos.y, pos.z, _cheat_mode);

}

void 
gplayer_imp::PunishCheat()
{
	//�ͷ������� 
	_cheat_punish = 1;
	__PRINTF("%d���׳ͷ���ʼ\n", _parent->ID.id);
}

void 
gplayer_imp::WallowControl(int level, int p_time, int l_time, int h_time, int reason)
{
	if(level < 0 || level >= anti_wallow::MAX_WALLOW_LEVEL - 1) return ;
	_wallow_level = level + 1;
	_runner->player_wallow_info(level, p_time, l_time, h_time, reason);
}

bool gplayer_imp::CheckPlayerAutoSupport()
{
	for(unsigned int i = 0; i < _inventory.Size(); ++i)
	{
		int item_type = _inventory[i].type;
		if(item_type == -1) continue;
		if((item_type == AUTO_SUPPORT_STONE1) || (item_type == AUTO_SUPPORT_STONE2) 
			|| (item_type == AUTO_SUPPORT_STONE3) || (item_type == AUTO_SUPPORT_STONE4))
		{
			return true;
		}
	}
	return false;
}

void gplayer_imp::OnAntiCheatAttack(float rate)
{
	if(!world_manager::GetWorldLimit().anti_cheat) return;
	if(abase::RandUniform() < rate)
	{
		if(CheckCoolDown(COOLDOWN_INDEX_ANTI_CHEAT))
		{
			SetCoolDown(COOLDOWN_INDEX_ANTI_CHEAT,20*60*1000);
			
			bool need_anticheat = !CheckPlayerAutoSupport();
			
			if(need_anticheat)
			{
				GMSV::TriggerQuestion2Gacd(_parent->ID.id);
				__PRINTF("�û�%d����Ҵ�����\n",_parent->ID.id);
			}
		}
	}
}

void gplayer_imp::QuestionBonus()
{
	float  val= abase::RandUniform();
	//__PRINTF("�û�%d����Ҵ������ˣ�����%d\n",_parent->ID.id,val < 0.5f?0:1);
	GLog::log(GLOG_INFO,"�û�%d����Ҵ������ˣ�����%d\n",_parent->ID.id,val < 0.5f?0:1);
	if(val < 0.5f)
	{
		int exp = int(_basic.level * (20.f+ _basic.level*0.1f)*(5.0/75.f)) * 100;
		ReceiveTaskExp(exp, 0);
	}
	else
	{
		int sp = int(_basic.level * (20.f+ _basic.level*0.1f)*(5.0/75.f)) * 25;
		ReceiveTaskExp(0, sp);
	}
}

bool    
gplayer_imp::CheckAndSetCoolDown(int idx, int msec)
{       
	if(!_cooldown.TestCoolDown(idx)) return false;
	_cooldown.SetCoolDown(idx,msec);
	_runner->set_cooldown(idx, msec);
	return true;
}       

void
gplayer_imp::DoTeamRelationTask(int reason) 
{
	//���봦�ڶ�����ֻ�жӳ��ܷ���������
	if(!IsInTeam() || !IsTeamLeader()) return; 
	if(GetTeamMemberNum() <= 1) return ;
	//�������������1������ȴ
	if(!CheckAndSetCoolDown(COOLDOWN_INDEX_TEAM_RELATION, 1000)) return ;

	class TeamRelationExec : public ONET::Thread::Runnable
	{       
		int _leader;
		int _reason;
		world * _plane; 
		abase::vector<int , abase::fast_alloc<> > _list;
		public: 
		TeamRelationExec(gplayer_imp * pImp, int reason, const XID * list, unsigned int count)
		{       
			_plane = pImp->_plane;
			_leader = pImp->GetParent()->ID.id;
			_reason = reason; 
			_list.reserve(count);
			for(unsigned int i = 0; i < count ; i ++) 
			{       
				_list.push_back(list[i].id);
			}       
		}       
		virtual void Run()
		{       
			OnRun();
			delete this;
		}       

		bool OnRun() 
		{       
			//���ȸ��������˵�ID�õ����е�pPlayer
			abase::vector<gplayer *, abase::fast_alloc<> > plist;
			abase::vector<int *, abase::fast_alloc<> > llist;
			gplayer * pLeader = NULL; 
			plist.reserve(_list.size());
			llist.reserve(_list.size());
			world_manager * pManager = _plane->GetWorldManager();
			for(unsigned int i = 0; i < _list.size();i ++)
			{       
				int foo;
				gplayer *pPlayer = pManager->FindPlayer(_list[i],foo);
				if(pPlayer == NULL) return false;       //�Ҳ���ָ���Ķ���
				plist.push_back(pPlayer);
				llist.push_back(&pPlayer->spinlock);
				if(_list[i] == _leader) pLeader = pPlayer;
			}       
			if(pLeader == NULL) return false;       //�Ҳ��������ߣ��ӳ���

			std::sort(llist.begin(), llist.end());

			//����������� 
			for(unsigned int i = 0; i < llist.size();i ++)
			{
				mutex_spinlock(llist[i]);
			}

			try
			{
				//��������˵�״̬λ���Ƿ���ȷ
				A3DVECTOR pos = pLeader->pos;
				for(unsigned int i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					if(pPlayer->ID.id != _list[i])  throw -3;
					if(!pPlayer->IsActived() ||
							pPlayer->IsZombie() ||
							pPlayer->login_state != gplayer::LOGIN_OK) throw -4;
					if(pPlayer->pos.squared_distance(pos) > 30.f*30.f) throw -5;
					if(!pPlayer->imp) throw -6;
					if(!pPlayer->imp->CanTeamRelation()) throw -7;
					if(!pPlayer->imp->GetRunTimeClass()->IsDerivedFrom(gplayer_imp::GetClass()))
						throw -8;

				}

				//������״̬�Ƿ����
				gplayer_imp * pLmp = (gplayer_imp *) pLeader->imp;
				if(!pLmp->IsInTeam() || !pLmp->IsTeamLeader()) throw -9;
				for(unsigned int i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					if(pPlayer == pLeader) continue;
					if(!pLmp->IsMember(pPlayer->ID)) throw -10;
				}

				PlayerTaskTeamInterface TaskTeam;
				//ʹ������ļ��ӿڽ��м��
				for(unsigned int i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp;
					PlayerTaskInterface TaskIf(pImp);
					if(!TaskIf.OnCheckTeamRelationship(_reason,&TaskTeam))
					{
						throw -11;
					}
				}

				//���м�鶼ͨ���ˣ����������ִ�нӿ�
				for(unsigned int i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp;
					PlayerTaskInterface TaskIf(pImp);
					if(!TaskIf.OnCheckTeamRelationship(_reason,&TaskTeam))
					{
						throw -11;
					}
				}

				//���м�鶼ͨ���ˣ����������ִ�нӿ�
				for(unsigned int i = 0; i < _list.size();i ++)
				{
					gplayer * pPlayer = plist[i];
					gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp;
					PlayerTaskInterface TaskIf(pImp);
					TaskIf.OnCheckTeamRelationshipComplete(_reason,&TaskTeam);
				}

				//����ִ�ж�����߼���ϵ
				TaskTeam.Execute(plist.begin(), plist.size());
			}catch(int e)
			{
				__PRINTF("��ӹ�ϵ������ʧ�ܣ������:%d\n", e);
			}


			//�����������
			for(unsigned int i = 0; i < llist.size();i ++)
			{
				mutex_spinunlock(llist[i]);
			}
			return true;
		}
	};

	XID list[TEAM_MEMBER_CAPACITY];
	int count = GetMemberList(list);
	ONET::Thread::Pool::AddTask( new TeamRelationExec(this, reason, list, count));
}

bool
gplayer_imp::CanTeamRelation()
{
	if(_player_state != PLAYER_STATE_NORMAL) return false;
	return true;
}

int  
gplayer_imp::RefineTransmit(unsigned int src_index, unsigned int dest_index)
{
	//�����Ʒ�Ƿ�����Լ��Ƿ�Ϸ�
	if(src_index >= _inventory.Size() || dest_index >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & src_item = _inventory[src_index];
	item & dest_item = _inventory[dest_index];
	if(dest_item.type == -1 || src_item.type == -1) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(dest_item.body == NULL || src_item.body == NULL) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	int src_bindlevel = 0;
	int dest_bindlevel = 0;
	if(src_item.proc_type & item::ITEM_PROC_TYPE_BIND)
	{
		if(src_item.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
		{
			src_bindlevel = 1;
		}
		else
		{
			src_bindlevel = 2;
		}
	}
	if(dest_item.proc_type & item::ITEM_PROC_TYPE_BIND)
	{
		if(dest_item.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
		{
			dest_bindlevel = 1;
		}
		else
		{
			dest_bindlevel = 2;
		}
	}
	//�������װ�������˺�һ״̬����
	if(dest_bindlevel <  src_bindlevel)
	{
		return S2C::ERR_TRANSMIT_REFINE_NEED_BIND;
	}

	//�������װ�����ǿ��Խ��о�������
	int material_need;
	int dest_refine_addon = world_manager::GetDataMan().get_item_refine_addon(dest_item.type, material_need);
	if(dest_refine_addon <= 0 || material_need <= 0) return S2C::ERR_DEST_CAN_NOT_TRANSMIT_REFINE;
	int src_refine_addon = world_manager::GetDataMan().get_item_refine_addon(src_item.type, material_need);
	if(src_refine_addon <= 0 || material_need <= 0) return S2C::ERR_DEST_CAN_NOT_TRANSMIT_REFINE;

	//�������װ���ľ����ȼ���ƥ���
	int dest_level = dest_item.body->GetRefineLevel(dest_refine_addon);
	int src_level = src_item.body->GetRefineLevel(src_refine_addon);
	if(dest_level < 0  || src_level < 0 || src_level > 12) return S2C::ERR_CAN_NOT_TRANSMIT_REFINE;
	if(src_level == 0 || dest_level >= src_level) return S2C::ERR_LOW_LEVEL_TRANSMIT_REFINE;
	
	//���Ǭ��ʯ�������Ƿ��㹻
	const int need_material[] = {1,5,12,28,60,80,100,150,250,400,650,1000};
	if(!CheckItemExist(ALLSPARK_ID,	need_material[src_level -1])) return S2C::ERR_TRANSMIT_REFINE_NO_MATERIAL;
	
	//����˫�����¾����ȼ�
	int n_src_level = src_item.body->SetRefineLevel(src_refine_addon, 0);
	int n_dest_level = dest_item.body->SetRefineLevel(dest_refine_addon, src_level);
	
	//����־
	GLog::log(GLOG_INFO,"���%dִ���˾�����ת��������(%d[%d->%d])��(%d[%d->%d])",_parent->ID.id, 
			src_item.type, src_level, n_src_level, 
			dest_item.type,dest_level,n_dest_level);

	//�۳�Ǭ��ʯ
	PlayerTaskInterface TaskIf(this);
	TaskIf.TakeAwayCommonItem(ALLSPARK_ID, need_material[src_level - 1]);
	
	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,src_index);
	PlayerGetItemInfo(IL_INVENTORY,dest_index);
	return 0;
}

bool
gplayer_imp::ForeignDoShoppingStep1(int goods_id, unsigned int goods_index, unsigned int goods_slot)
{
	//������Ҫ���Ӹ�����Թ����״̬
	if(_player_state != PLAYER_SIT_DOWN && _player_state != PLAYER_STATE_NORMAL && _player_state != PLAYER_STATE_BIND) return false;
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	netgame::mall & shop = world_manager::GetPlayerMall();
	if(goods_slot >= netgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	netgame::mall::node_t node;
	if(!shop.QueryGoods(goods_index,node) || node.goods_id != goods_id)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	if(node.gift_id > 0)	//�����ݲ�֧����һ��һ
	{
		return false;
	}

	if(!node.check_owner(0))
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}

	if(node.entry[goods_slot].cash_need <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return true;
	}
	if(node.entry[goods_slot].expire_time&&node.entry[goods_slot].expire_type!=netgame::mall::EXPIRE_TYPE_TIME)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
	}
	int name_len =0;
	unsigned char *goods_name = world_manager::GetDataMan().get_item_name(node.goods_id,name_len);
	if(goods_name  == NULL)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return false;
	}
	
	//����Э���Delivery
	if(world_manager::GetWorldParam().korea_shop)
	{
		GNET::SendBillingRequest(_parent->ID.id,node.goods_id, node.goods_count,node.entry[goods_slot].expire_time, node.entry[goods_slot].cash_need);
	}
	else if(world_manager::GetWorldParam().southamerican_shop)
	{
		int unit_price = node.entry[goods_slot].cash_need/node.goods_count; 
		GNET::SendBillingConfirmSA(_parent->ID.id,node.goods_id,node.goods_count,goods_name,name_len,node.entry[goods_slot].expire_time, unit_price);
	}
	return true;
}

bool
gplayer_imp::ForeignDoShoppingStep2(int item_id, unsigned int count, int expire_time, unsigned int cash_need)
{
	//������Ҫ���Ӹ�����Թ����״̬
	if(_player_state != PLAYER_SIT_DOWN && _player_state != PLAYER_STATE_NORMAL && _player_state != PLAYER_STATE_BIND) return false;
	if(!GetInventory().GetEmptySlotCount())
	{
		return false;
	}
	
	const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(item_id);
	if(!pItem) return false;
	item_data * pItem2 = DupeItem(*pItem);
	int guid1 = 0;
	int guid2 = 0;
	int expire_date = 0;
	if(expire_time)  expire_date = g_timer.get_systime() + expire_time;
	if(pItem2->guid.guid1 != 0)
	{
		//�����Ҫ������GUID
		get_item_guid(pItem2->type, guid1,guid2);
		pItem2->guid.guid1 = guid1;
		pItem2->guid.guid2 = guid2;
	}

	pItem2->proc_type |= item::ITEM_PROC_TYPE_MALL;
	UpdateMallConsumptionShopping(pItem2->type, pItem2->proc_type, count, cash_need);

	int ocount = count;
	if((unsigned int)ocount > (unsigned int)pItem2->pile_limit) ocount = pItem2->pile_limit;
	int rst =_inventory.Push(*pItem2,ocount,expire_date);
	ASSERT( rst >= 0);
	_runner->obtain_item(item_id,pItem2->expire_date,count,_inventory[rst].count, 0,rst);
	//�������³�ʼ��һ�¿��ܵ�ʱװ
	_inventory[rst].InitFromShop();
    
	//��¼��־  
	_mall_order_id ++;
	GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:guid1=%d:guid2=%d",
			_parent->ID.id,_db_user_id, _mall_order_id,item_id, expire_date,count,cash_need,guid1,guid2);
	world_manager::TestCashItemGenerated(item_id, count);
	FreeItem(pItem2);

	_mall_cash -= (int)cash_need;		//���ﲻ��ı�ʹ�ü�¼ ���� cash_offset��cash_usedӦʼ��Ϊ0
	/* $$$$$$$$$$$$$$ Ŀǰ���ײ����湺���б��� ��Ϊ����Ϊ�˱�̫�� ����������ʧ��
	_mall_invoice.push_back(netgame::mall_invoice(_mall_order_id, item_id, count,cash_need,expire_date, g_timer.get_systime(), guid1,guid2));*/
	_runner->player_cash(GetMallCash());

	if(world_manager::GetWorldParam().korea_shop)
	{
		GNET::SendBillingBalance(_parent->ID.id);
	}
	else if(world_manager::GetWorldParam().southamerican_shop)
	{
		GNET::SendBillingBalanceSA(_parent->ID.id);
	}
	GLog::log(GLOG_INFO,"�û�%d�ڰٱ�����%d��%d������%d�����ʣ��%d��",
			_parent->ID.id, count, item_id, cash_need, GetMallCash());

	return true;
}

bool gplayer_imp::PlayerGetMallItemPrice(int start_index, int end_index)	//lgc����������Ϊ0, ���ʾɨ��������,
{																			//����[start_index,end_index)�ڵ���Ʒ��ɨ��
	netgame::mall & __mall = world_manager::GetPlayerMall();	
	unsigned int __mall_goods_count = __mall.GetGoodsCount();
	int __group_id = __mall.GetGroupId();	//��ǰ�������趨��group_id
	
	if(start_index == 0 && end_index == 0)	//ɨ��ȫ��
	{
		start_index = 0;
		end_index = __mall_goods_count;
	}
	else
	{
		if(start_index < 0 || end_index <= 0 || (unsigned int)start_index >= __mall_goods_count || (unsigned int)end_index > __mall_goods_count || start_index >= end_index)
		{
			_runner->error_message(S2C::ERR_FATAL_ERR);
			return false;
		}
	}
	//ֻ��ɨ�跶Χ����ָ��ֵʱ��������ȴ
	if(end_index-start_index > 10 && !CheckCoolDown(COOLDOWM_INDEX_GET_MALL_PRICE)) return false;
	SetCoolDown(COOLDOWM_INDEX_GET_MALL_PRICE,GET_MALL_PRICE_COOLDOWN_TIME);
	
	//���ܷ����仯����Ʒ�б�	
	abase::vector<netgame::mall::index_node_t, abase::fast_alloc<> > & __limit_goods = __mall.GetLimitGoods();
	unsigned int __limit_goods_count = __limit_goods.size();
	
	time_t __time = time(NULL);
	packet_wrapper __h0(1024);
	int	__h0_count = 0; 
    using namespace S2C;
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	for(unsigned int i=0; i<__limit_goods_count; i++)
	{
		int index = __limit_goods[i]._index;
		if(index < start_index)
			continue;
		if(index >= end_index)
			break;
		netgame::mall::node_t & node = __limit_goods[i]._node;
			
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;		
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}
	
		if(!node.sale_time_active)		//ֻ�����õ�����ʱ��
		{
			//����Ч�ķ�Ĭ��group������	�����ͻ���
			ASSERT(node.group_active);
			if(active_group_id != 0)
			{
				for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
				{
					if(node.entry[j].cash_need <= 0)
						break;
					if(active_group_id == node.entry[j].group_id)	//�����������ҵ�һ��
					{
						CMD::Make<CMD::mall_item_price>::AddGoods(__h0, 
														index, 
														j, 
														node.goods_id, 
														node.entry[j].expire_type, 
														node.entry[j].expire_time, 
														node.entry[j].cash_need, 
														node.entry[j].status,
														node.entry[j].min_vip_level);
						__h0_count ++;
					}
				}
			}
		}
		else							//���ڷ����õ�����ʱ��
		{
			int available_permanent = -1;				//��Ч����������ʱ���slot������һ��
			int available_saletime = -1;				//��Ч�ķ���������ʱ���slot������һ��
			for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
			{
				if(node.entry[j].cash_need <= 0)
					break;
				if(node.entry[j].group_id == active_group_id && node.entry[j]._sale_time.CheckAvailable(__time))
				{
					if(node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT)
					{
						available_saletime = j;
						break;		///ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
					}
					else
						available_permanent = j;	//���౻ִ��һ��	
				}			
			}
			if(available_saletime >= 0)		//ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
			{
				CMD::Make<CMD::mall_item_price>::AddGoods(__h0, 
														index, 
														available_saletime, 
														node.goods_id, 
														node.entry[available_saletime].expire_type, 
														node.entry[available_saletime].expire_time, 
														node.entry[available_saletime].cash_need, 
														node.entry[available_saletime].status,
														node.entry[available_saletime].min_vip_level);
				__h0_count ++;
			}
			else if(available_permanent >= 0 )	//ͬ��������������ʱ������
			{
				if(active_group_id != 0)	//ֻҪ��Ĭ��group�����ݲŻᷢ���ͻ���	
				{
					CMD::Make<CMD::mall_item_price>::AddGoods(__h0, 
																index, 
																available_permanent, 
																node.goods_id, 
																node.entry[available_permanent].expire_type, 
																node.entry[available_permanent].expire_time, 
																node.entry[available_permanent].cash_need, 
																node.entry[available_permanent].status,
																node.entry[available_permanent].min_vip_level);
					__h0_count ++;
				}
			}
			else		//��Ʒ�¼�,ֻ���ڿͻ��˵�gshop.data�д������õĹ���ʽʱ����Ҫ������Ʒ�¼�
			{
				if(active_group_id != 0)	//����0�������ǰ���Ѿ��жϿͻ��˿϶�û�����õĹ���ʽ
				{
					int m;
					for(int m=0; m<netgame::mall::MAX_ENTRY; m++)
					{
						if(node.entry[m].cash_need > 0 && node.entry[m].group_id == 0 
							&& node.entry[m]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
							break;
					}
					if(m < netgame::mall::MAX_ENTRY)
					{
						CMD::Make<CMD::mall_item_price>::AddGoods(__h0, 
																index, 
																0, 
																node.goods_id, 
																0, 
																0, 
																0, 
																0,
																0);
						__h0_count ++;
					}
				}
			}
		}//end of if(!sale_time_active)
	}
	
	packet_wrapper __h1(1024);
	CMD::Make<CMD::mall_item_price>::From(__h1, start_index, end_index, __h0_count);
	if(__h0_count > 0)
		__h1.push_back(__h0.data(), __h0.size());
	gplayer * pPlayer = (gplayer*)_parent;
	send_ls_msg(pPlayer,__h1);
	
	return true;
}

bool 
gplayer_imp::PlayerEquipTrashboxFashionItem(unsigned char trash_idx_body,unsigned char trash_idx_leg,unsigned char trash_idx_foot,unsigned char trash_idx_wrist,unsigned char trash_idx_head,unsigned char trash_idx_weapon)
{
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_EQUIP_FASHION_ITEM))	return false;
	
	if(_lock_equipment)
	{	
		_runner->error_message(S2C::ERR_EQUIPMENT_IS_LOCKED);
		return false;
	}

	item_list & box3 = GetTrashInventory(IL_TRASH_BOX3);
	if(!box3.Size()) return false;
	
	if(trash_idx_body == 255 && trash_idx_leg == 255 && trash_idx_foot == 255 && trash_idx_wrist == 255 && trash_idx_head == 255 && trash_idx_weapon == 255) return false;

	if(trash_idx_body != 255 && !CheckEquipTrashboxItem(box3, trash_idx_body, item::EQUIP_INDEX_FASHION_BODY)) return false;
	if(trash_idx_leg != 255 && !CheckEquipTrashboxItem(box3, trash_idx_leg, item::EQUIP_INDEX_FASHION_LEG)) return false;
	if(trash_idx_foot != 255 && !CheckEquipTrashboxItem(box3, trash_idx_foot, item::EQUIP_INDEX_FASHION_FOOT)) return false;
	if(trash_idx_wrist != 255 && !CheckEquipTrashboxItem(box3, trash_idx_wrist, item::EQUIP_INDEX_FASHION_WRIST)) return false;
	if(trash_idx_head != 255 && !CheckEquipTrashboxItem(box3, trash_idx_head, item::EQUIP_INDEX_FASHION_HEAD)) return false;
	if(trash_idx_weapon != 255 && !CheckEquipTrashboxItem(box3, trash_idx_weapon, item::EQUIP_INDEX_FASHION_WEAPON)) return false;

	//��λ�ò�����ͬ
	unsigned char empty_idx[6] = {0};
	int empty_idx_cnt = 0;
	if(trash_idx_body != 255 && box3[trash_idx_body].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_body;
	if(trash_idx_leg != 255 && box3[trash_idx_leg].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_leg;
	if(trash_idx_foot != 255 && box3[trash_idx_foot].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_foot;
	if(trash_idx_wrist != 255 && box3[trash_idx_wrist].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_wrist;
	if(trash_idx_head != 255 && box3[trash_idx_head].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_head;
	if(trash_idx_weapon != 255 && box3[trash_idx_weapon].type <= 0) empty_idx[empty_idx_cnt++] = trash_idx_weapon;

	for(int i=0; i<empty_idx_cnt-1; i++)
	{
		for(int j=i+1; j<empty_idx_cnt; j++)
			if(empty_idx[i] == empty_idx[j]) return false;
	}

	if(trash_idx_body != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_body,item::EQUIP_INDEX_FASHION_BODY);	
	if(trash_idx_leg != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_leg,item::EQUIP_INDEX_FASHION_LEG);	
	if(trash_idx_foot != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_foot,item::EQUIP_INDEX_FASHION_FOOT);	
	if(trash_idx_wrist != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_wrist,item::EQUIP_INDEX_FASHION_WRIST);	
	if(trash_idx_head != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_head,item::EQUIP_INDEX_FASHION_HEAD);	
	if(trash_idx_weapon != 255) PlayerEquipTrashboxItem(IL_TRASH_BOX3,trash_idx_weapon,item::EQUIP_INDEX_FASHION_WEAPON);	

	SetCoolDown(COOLDOWN_INDEX_EQUIP_FASHION_ITEM,EQUIP_FASHION_ITEM_COOLDOWN_TIME);

	
	return true;	
}

bool 
gplayer_imp::CheckEquipTrashboxItem(item_list & trashbox, unsigned char trash_idx,unsigned char equip_idx)
{
	if(trash_idx >= trashbox.Size()) return false;
	item & it = trashbox[trash_idx]; 
	if(it.type <= 0)
	{
		if(_equipment[equip_idx].type <= 0) return false;
	}
	else
	{
		if(!it.CheckEquipPostion(equip_idx)) return false;
		if(!it.CanActivate(_equipment,equip_idx,this)) return false;
	}
	return true;
}

bool 
gplayer_imp::PlayerEquipTrashboxItem(int where, unsigned char trash_idx,unsigned char equip_idx)
{
	/*
	 �������ֿ�װ����Ʒ��Ŀǰֻ֧��ʱװ�Ϳ���
	 */
	if(!IsPortableTrashBox(where)) return false;
	item_list & trashbox = GetTrashInventory(where);

	if(trash_idx >= trashbox.Size() || equip_idx >= _equipment.Size()) return false;

	if(_lock_equipment) return false;

	item & it1 = trashbox[trash_idx];
	item & it2 = _equipment[equip_idx];
	bool type1 = (it1.type == -1);
	bool type2 = (it2.type == -1);
	if(type1 && type2) return false;
	
	if(!type1)
	{
		//�ֿ����ǿ�
		if(!it1.CheckEquipPostion(equip_idx)) return false;
		//���Ƽ���ʱ������ͳ������Ϊ��ֹ�¿��Ʋ����㼤���������Ȱ�ԭ����ȡ������
		bool need_deactivate = (equip_idx >= item::EQUIP_INDEX_GENERALCARD1 && equip_idx <= item::EQUIP_INDEX_GENERALCARD6 && !type2);
		if(need_deactivate) it2.Deactivate(item::BODY, equip_idx, this);
		if(!it1.CanActivate(_equipment,equip_idx,this))
		{
			if(need_deactivate) it2.Activate(item::BODY, _equipment, equip_idx, this);
			return false;
		}
		
		//����
		item temp1,temp2;
		trashbox.Remove(trash_idx, temp1);
		_equipment.Remove(equip_idx, temp2);

		_equipment.Put(equip_idx,temp1);
		trashbox.Put(trash_idx,temp2);
		temp1.Clear();
		temp2.Clear();

		RefreshEquipment();
		
		//�����ɹ������һ���Ƿ���Ҫ���а󶨲���
		item & it = _equipment[equip_idx];
		ASSERT(it.type != -1);
		bool notify_equip_item = false;
		if(it.proc_type & item::ITEM_PROC_TYPE_BIND2)
		{
			//��������������а�
			it.proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;

			it.proc_type &= ~(item::ITEM_PROC_TYPE_BIND2);
			notify_equip_item = true;
			GLog::log(LOG_INFO,"�û�%dװ������Ʒ%d, GUID(%d,%d)",_parent->ID.id,it.type,it.guid.guid1,it.guid.guid2);

			UpdateMallConsumptionBinding(it.type, it.proc_type, it.count);
		}
		
		_runner->equip_trashbox_item(where,trash_idx,equip_idx);

		X_EQUIP id1;
		id1.type = it.type; id1.mask = it.GetIdModify();	
		CalcEquipmentInfo();
		_runner->equipment_info_changed(1ULL<<equip_idx , 0, &id1,sizeof(X_EQUIP));//�˺���ʹ����CalcEquipmentInfo�Ľ��
		
		if(notify_equip_item)
		{
			PlayerGetItemInfo(IL_EQUIPMENT,equip_idx);
		}
		IncEquipChangeFlag();
		IncTrashBoxChangeCounter();
		return true;
	}
	
	//�ֿ���Ϊ�գ�����ԭ���ģ���ˢ��װ��
	item  temp;
	_equipment.Remove(equip_idx,temp);
	bool bRst = trashbox.Put(trash_idx,temp);
	ASSERT(bRst);
	if(bRst)
	{
		temp.Clear();
	}
	else
	{
		//��¼������־
		GLog::log(GLOG_ERR,"װ����Ʒʱ������������");
		temp.Release();
	}
	RefreshEquipment();
	_runner->equip_trashbox_item(where,trash_idx,equip_idx);
	CalcEquipmentInfo();
	_runner->equipment_info_changed(0,1ULL<<equip_idx , 0, 0); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	IncEquipChangeFlag();
	IncTrashBoxChangeCounter();
	return true;
}

void 
gplayer_imp::PlayerCheckSecurityPasswd(const char * passwd, unsigned int passwd_size)	
{
	if(!_trashbox.CheckPassword(passwd, passwd_size))
	{
		_runner->error_message(S2C::ERR_PASSWD_NOT_MATCH);
		return;
	}
	SetSecurityPasswdChecked(true);
}

void 
gplayer_imp::PlayerNotifyForceAttack(char force_attack, char refuse_bless)
{
	_force_attack = force_attack & C2S::FORCE_ATTACK_MASK_ALL;
	_refuse_bless = refuse_bless & C2S::REFUSE_BLESS_MASK_ALL;
}

bool
gplayer_imp::PlayerDoDividendShopping(unsigned int goods_count,const int * order_list, int shop_tid)
{
	//������Ҫ���Ӹ�����Թ����״̬
	if(_player_state != PLAYER_SIT_DOWN && _player_state != PLAYER_STATE_NORMAL && _player_state != PLAYER_STATE_BIND) return false;

	if(goods_count == 0)
	{
		return false;
	}
	if(goods_count > _inventory.Size() || !InventoryHasSlot(goods_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	int gifts_count = 0;
	
	netgame::mall & shop = world_manager::GetPlayerMall2();
	int __group_id = shop.GetGroupId();	//��ǰ�������趨��group_id,lgc
	time_t __time = time(NULL);			//
	netgame::mall_order  order(-1);

	std::map<int, int> item_limit_type_map; // item_id -> limit_type
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	unsigned int offset = 0;
	for(unsigned int i = 0; i < goods_count; i ++, offset += sizeof(C2S::CMD::dividend_mall_shopping::__entry) / sizeof(int))
	{
		int mall_rate = order_list[offset];
		int id = order_list[offset+1];
		unsigned int index = order_list[offset +2];
		unsigned int slot = order_list[offset +3];
		if(slot >= netgame::mall::MAX_ENTRY)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		netgame::mall::node_t node;
		if(!shop.QueryGoods(index,node) || node.goods_id != id)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(!node.check_owner(shop_tid))
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(node.entry[slot].cash_need <= 0)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		// MALL RATE 172 BEGIN
		if (mall_rate <= 0) mall_rate = 1;
		if (mall_rate >= 1000) mall_rate = 1000;
		
		if (node.goods_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.goods_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.goods_count = new_count;
			node.entry[slot].cash_need *= mall_rate;
		}
		
		if (node.gift_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.gift_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.gift_count = new_count;
		}
		// MALL RATE 172 END

		if(!_purchase_limit_info.CheckShoppingLimitItem(id, node.buy_times_limit, node.buy_times_limit_mode, node.goods_count))
		{
			_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 1);
			return true;
		}

		if(GetCashVipLevel() < node.entry[slot].min_vip_level)
		{
			_runner->error_message(S2C::ERR_CASH_VIP_LIMIT);
			return true;
		}
		
		//lgc	
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}

		if(node.sale_time_active)
		{	
			if(node.entry[slot].group_id == active_group_id && node.entry[slot]._sale_time.CheckAvailable(__time))	
			{
				//���playerѡ���slot�����õ����۷�ʽ������Ҫɨ�赱ǰ��Ч���ڣ����Ƿ񻹴��ڷ��������۷�ʽ�����
				if(node.entry[slot]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
				{
					for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
					{
						if(node.entry[j].cash_need <= 0)
							break;
						if(node.entry[j].group_id == active_group_id 
									&& node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT
									&& node.entry[j]._sale_time.CheckAvailable(__time))
						{
							_runner->dividend_mall_item_buy_failed(index, 0);
							return false;
						}
					}
				}				
			}
			else
			{
				_runner->dividend_mall_item_buy_failed(index, 0);
				return false;
			}
		}
		else if(node.entry[slot].group_id != active_group_id)
		{
			_runner->dividend_mall_item_buy_failed(index, 0);
			return false;
		}

		if(node.gift_id > 0) gifts_count ++;  //ͳ����Ʒ��
		
		order.AddGoods(node.goods_id, node.goods_count , node.entry[slot].cash_need, node.entry[slot].expire_time,node.entry[slot].expire_type,node.gift_id,node.gift_count,node.gift_expire_time,node.gift_log_price);

		if(node.buy_times_limit_mode)
			item_limit_type_map[node.goods_id] = node.buy_times_limit_mode;
	}
	if(_dividend_mall_info.GetDividend() < order.GetPointRequire())
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	if(!InventoryHasSlot(goods_count + gifts_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int total_cash = _dividend_mall_info.GetDividend();
	int cash_used = 0;
	//��Ǯ�㹻�� ��ʼ������Ʒ
	int cur_t = g_timer.get_systime();
	int self_id = GetParent()->ID.id;
	for(unsigned int i = 0; i < goods_count; i ++)
	{
		int id;
		int count;
		int point;
		int expire_time;
		int expire_type;
		int gift_id;
		int gift_count;
		int gift_expire_time;
		int gift_log_price;
		bool bRst = order.GetGoods(i, id, count,point, expire_time,expire_type,gift_id,gift_count,gift_expire_time,gift_log_price);
		if(bRst)
		{
			if( LuaManager::GetInstance()->EventOnPlayerDoDividendShopping(self_id, id, count, point) )
			return false;
			
			
			//������Ʒ����Ʒ��log�۸�
			int log_price1 = point;
			int log_price2 = 0;
			if(gift_id > 0 && gift_log_price > 0)
			{
				log_price1 = int((float)point*point/(point+gift_log_price));
				log_price2 = point - log_price1; 
			}

			const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(id);
			if(pItem)
			{
				item_data * pItem2 = DupeItem(*pItem);
				int expire_date = 0;
				if(expire_time) 
				{
					if(expire_type == netgame::mall::EXPIRE_TYPE_TIME)
					{
						//��Ч������һ������
						expire_date = cur_t + expire_time;
					}
					else
					{
						//��Ч���ǹ涨����ʧЧ
						expire_date = expire_time;
					}
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pItem2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pItem2->type, guid1,guid2);
					pItem2->guid.guid1 = guid1;
					pItem2->guid.guid2 = guid2;
				}

				int ocount = count;
				int rst =_inventory.Push(*pItem2,count,expire_date);
				ASSERT(rst >= 0 && count == 0);
				_runner->obtain_item(id,pItem2->expire_date,ocount,_inventory[rst].count, 0,rst);

				if(item_limit_type_map.find(id) != item_limit_type_map.end())
				{
					int have_purchase_count = _purchase_limit_info.AddShoppingLimit(id, item_limit_type_map[id], ocount);
					_runner->purchase_limit_info_notify(item_limit_type_map[id], id, have_purchase_count);
				}

				//�������³�ʼ��һ�¿��ܵ�ʱװ
				_inventory[rst].InitFromShop();

				total_cash -= log_price1;
				cash_used += log_price1;
				//��¼��־  
				GLog::formatlog("formatlog:gdividendshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%d:dividend_need=%d:dividend_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,id, expire_date,ocount,log_price1,total_cash,guid1,guid2);
				
				world_manager::TestCashItemGenerated(id, ocount);
				FreeItem(pItem2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ�������̳���Ʒ%dʱ������Ʒʧ��",self_id, id);
			}

			//����Ϊ������Ʒ
			if(gift_id > 0)
			{
			const item_data * pGift = (const item_data*)world_manager::GetDataMan().get_item_for_sell(gift_id);
			if(pGift)
			{
				item_data * pGift2 = DupeItem(*pGift);
				int expire_date = 0;
				if(gift_expire_time) 
				{
					//��Ч������һ������
					expire_date = cur_t + gift_expire_time;
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pGift2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pGift2->type, guid1,guid2);
					pGift2->guid.guid1 = guid1;
					pGift2->guid.guid2 = guid2;
				}

				int ocount = gift_count;
				int rst =_inventory.Push(*pGift2,gift_count,expire_date);
				ASSERT(rst >= 0 && gift_count == 0);
				_runner->obtain_item(gift_id,expire_date,ocount,_inventory[rst].count, 0,rst);

				//�������³�ʼ��һ�¿��ܵ�ʱװ
                _inventory[rst].InitFromShop();
				
				total_cash -= log_price2;
				cash_used += log_price2;
				//��¼��־  
				GLog::formatlog("formatlog:gdividendshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%d:dividend_need=%d:dividend_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,gift_id, expire_date,ocount,log_price2,total_cash,guid1,guid2);
				
				world_manager::TestCashItemGenerated(gift_id, ocount);
				FreeItem(pGift2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ�������̳���Ʒ%dʱ������Ʒ%dʧ��",self_id, id, gift_id);
			}
			}
		}
		else
		{
			ASSERT(false);
		}
	}

	bool rst = _dividend_mall_info.SpendDividend(cash_used);
	ASSERT(rst);
	_runner->player_dividend(_dividend_mall_info.GetDividend());

	GLog::log(GLOG_INFO,"�û�%d�ں����̳ǹ���%d����Ʒ������%d��ʣ��%d��",self_id,goods_count,cash_used,_dividend_mall_info.GetDividend());

	//���Ǽӿ�����ٶ� 
	return true;
}

bool gplayer_imp::PlayerGetDividendMallItemPrice(int start_index, int end_index)	//lgc����������Ϊ0, ���ʾɨ��������,
{																			//����[start_index,end_index)�ڵ���Ʒ��ɨ��
	netgame::mall & __mall = world_manager::GetPlayerMall2();	
	unsigned int __mall_goods_count = __mall.GetGoodsCount();
	int __group_id = __mall.GetGroupId();	//��ǰ�������趨��group_id
	
	if(start_index == 0 && end_index == 0)	//ɨ��ȫ��
	{
		start_index = 0;
		end_index = __mall_goods_count;
	}
	else
	{
		if(start_index < 0 || end_index <= 0 || (unsigned int)start_index >= __mall_goods_count || (unsigned int)end_index > __mall_goods_count || start_index >= end_index)
		{
			_runner->error_message(S2C::ERR_FATAL_ERR);
			return false;
		}
	}
	//ֻ��ɨ�跶Χ����ָ��ֵʱ��������ȴ
	if(end_index-start_index > 10 && !CheckCoolDown(COOLDOWM_INDEX_GET_CASH_VIP_MALL_PRICE)) return false;
	SetCoolDown(COOLDOWM_INDEX_GET_CASH_VIP_MALL_PRICE,GET_DIVIDEND_MALL_PRICE_COOLDOWN_TIME);
	
	//���ܷ����仯����Ʒ�б�	
	abase::vector<netgame::mall::index_node_t, abase::fast_alloc<> > & __limit_goods = __mall.GetLimitGoods();
	unsigned int __limit_goods_count = __limit_goods.size();
	
	time_t __time = time(NULL);
	packet_wrapper __h0(1024);
	int	__h0_count = 0; 
    using namespace S2C;
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	for(unsigned int i=0; i<__limit_goods_count; i++)
	{
		int index = __limit_goods[i]._index;
		if(index < start_index)
			continue;
		if(index >= end_index)
			break;
		netgame::mall::node_t & node = __limit_goods[i]._node;
			
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;		
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}
	
		if(!node.sale_time_active)		//ֻ�����õ�����ʱ��
		{
			//����Ч�ķ�Ĭ��group������	�����ͻ���
			ASSERT(node.group_active);
			if(active_group_id != 0)
			{
				for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
				{
					if(node.entry[j].cash_need <= 0)
						break;
					if(active_group_id == node.entry[j].group_id)	//�����������ҵ�һ��
					{
						CMD::Make<CMD::dividend_mall_item_price>::AddGoods(__h0, 
														index, 
														j, 
														node.goods_id, 
														node.entry[j].expire_type, 
														node.entry[j].expire_time, 
														node.entry[j].cash_need, 
														node.entry[j].status,
														node.entry[j].min_vip_level);
						__h0_count ++;
					}
				}
			}
		}
		else							//���ڷ����õ�����ʱ��
		{
			int available_permanent = -1;				//��Ч����������ʱ���slot������һ��
			int available_saletime = -1;				//��Ч�ķ���������ʱ���slot������һ��
			for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
			{
				if(node.entry[j].cash_need <= 0)
					break;
				if(node.entry[j].group_id == active_group_id && node.entry[j]._sale_time.CheckAvailable(__time))
				{
					if(node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT)
					{
						available_saletime = j;
						break;		///ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
					}
					else
						available_permanent = j;	//���౻ִ��һ��	
				}			
			}
			if(available_saletime >= 0)		//ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
			{
				CMD::Make<CMD::dividend_mall_item_price>::AddGoods(__h0, 
														index, 
														available_saletime, 
														node.goods_id, 
														node.entry[available_saletime].expire_type, 
														node.entry[available_saletime].expire_time, 
														node.entry[available_saletime].cash_need, 
														node.entry[available_saletime].status,
														node.entry[available_saletime].min_vip_level);
				__h0_count ++;
			}
			else if(available_permanent >= 0 )	//ͬ��������������ʱ������
			{
				if(active_group_id != 0)	//ֻҪ��Ĭ��group�����ݲŻᷢ���ͻ���	
				{
					CMD::Make<CMD::dividend_mall_item_price>::AddGoods(__h0, 
																index, 
																available_permanent, 
																node.goods_id, 
																node.entry[available_permanent].expire_type, 
																node.entry[available_permanent].expire_time, 
																node.entry[available_permanent].cash_need, 
																node.entry[available_permanent].status,
																node.entry[available_permanent].min_vip_level);
					__h0_count ++;
				}
			}
			else		//��Ʒ�¼�,ֻ���ڿͻ��˵�gshop.data�д������õĹ���ʽʱ����Ҫ������Ʒ�¼�
			{
				if(active_group_id != 0)	//����0�������ǰ���Ѿ��жϿͻ��˿϶�û�����õĹ���ʽ
				{
					int m;
					for(int m=0; m<netgame::mall::MAX_ENTRY; m++)
					{
						if(node.entry[m].cash_need > 0 && node.entry[m].group_id == 0 
							&& node.entry[m]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
							break;
					}
					if(m < netgame::mall::MAX_ENTRY)
					{
						CMD::Make<CMD::dividend_mall_item_price>::AddGoods(__h0, 
																index, 
																0, 
																node.goods_id, 
																0, 
																0, 
																0, 
																0,
																0);
						__h0_count ++;
					}
				}
			}
		}//end of if(!sale_time_active)
	}
	
	packet_wrapper __h1(1024);
	CMD::Make<CMD::dividend_mall_item_price>::From(__h1, start_index, end_index, __h0_count);
	if(__h0_count > 0)
		__h1.push_back(__h0.data(), __h0.size());
	gplayer * pPlayer = (gplayer*)_parent;
	send_ls_msg(pPlayer,__h1);
	
	return true;
}

bool gplayer_imp::PlayerChooseMultiExp(int index)
{
	DATA_TYPE dt;
	MULTI_EXP_CONFIG * cfg = (MULTI_EXP_CONFIG *)world_manager::GetDataMan().get_data_ptr(701,ID_SPACE_CONFIG,dt);
	if(!cfg || dt != DT_MULTI_EXP_CONFIG) return false;

	if(index < 0 || (unsigned int)index >= sizeof(cfg->choice)/sizeof(cfg->choice[0])) return false;
	if(cfg->choice[index].rate < 1.f || cfg->choice[index].multi_time < 0 || cfg->choice[index].demulti_wait_time < 0 || cfg->choice[index].demulti_time < 0) return false;

	int stone_need = cfg->choice[index].item_count;
	int stone1_count, stone2_count;
	if(stone_need > 0)
	{
		stone1_count = GetItemCount(MULTI_EXP_STONE_ID);
		stone2_count = GetItemCount(MULTI_EXP_STONE_ID2);
		if(stone1_count + stone2_count < stone_need) return false;
	}
	
	if(!_multi_exp_ctrl.IncMultiExpTime(this,cfg->choice[index].rate,cfg->choice[index].multi_time*60,cfg->choice[index].demulti_wait_time*60,cfg->choice[index].demulti_time*60)) return false;
		
	if(stone_need > 0)
	{
		if(stone1_count >= stone_need)
			RemoveItems(MULTI_EXP_STONE_ID, stone_need, S2C::DROP_TYPE_USE, true);
		else
		{
			RemoveItems(MULTI_EXP_STONE_ID, stone1_count, S2C::DROP_TYPE_USE, true);
			RemoveItems(MULTI_EXP_STONE_ID2, stone_need-stone1_count, S2C::DROP_TYPE_USE, true);
		}
	}	
	return true;
}

bool 
gplayer_imp::PlayerGetFactionFortressInfo()
{
	if(!OI_IsMafiaMember()) return false;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return false;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return false;
	
	gplayer * pPlayer = (gplayer*)_parent;
	return pCtrl->GetInfo(pPlayer->ID.id, pPlayer->cs_index, pPlayer->cs_sid);
}

bool
gplayer_imp::TeamCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return false;
	}
	if(((gplayer*)_parent)->IsInvisible()) 
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_INVISIBLE);
		return false;
	}
	if(!CheckCoolDown(COOLDOWN_INDEX_TEAM_CONGREGATE))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return false;
	}
	if(!IsInTeam()) return false;

	msg_congregate_req_t data;
	data.world_tag = world_manager::GetWorldTag();
	data.level_req = reply_level_req; 
	data.sec_level_req = reply_sec_level_req;
    data.reincarnation_times_req = reply_reincarnation_times_req;
	MSG msg;
	BuildMessage(msg, GM_MSG_CONGREGATE_REQUEST, XID(-1,-1), _parent->ID, _parent->pos, CONGREGATE_TYPE_TEAM, &data, sizeof(data));
	_team.SendGroupMessage(msg);
	SetCoolDown(COOLDOWN_INDEX_TEAM_CONGREGATE, TEAM_CONGREGATE_COOLDOWN_TIME);
	return true;
}

bool 
gplayer_imp::TeamMemberCongregateRequest(const XID& member)
{
	//ֻ���ڴ�����ʹ��
	if(world_manager::GetWorldTag() != 1)
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_CUR_SCENE);
		return false;
	}

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return false;
	}
	if(((gplayer*)_parent)->IsInvisible()) 
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_INVISIBLE);
		return false;
	}
	if(member == _parent->ID || !IsInTeam() || !IsMember(member)) return false;

	msg_congregate_req_t data;
	data.world_tag = world_manager::GetWorldTag();
	data.level_req = 0; 
	data.sec_level_req = 0;
    data.reincarnation_times_req = 0;
	SendTo<0>(GM_MSG_CONGREGATE_REQUEST,member,CONGREGATE_TYPE_TEAM_MEMBER,&data, sizeof(data));
	return true;
}

bool 
gplayer_imp::FactionCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req)
{
	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return false;
	}
	if(((gplayer*)_parent)->IsInvisible()) 
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_INVISIBLE);
		return false;
	}
	if(!CheckCoolDown(COOLDOWN_INDEX_FACTION_CONGREGATE))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return false;
	}
	//ֻ�а���/������/��������ʹ��
	char rank = OI_GetMafiaRank();
	if(rank != 2 && rank != 3 && rank != 4) return false;

	struct
	{
		world_pos wpos;
		int level_req;
		int sec_level_req;
        int reincarnation_times_req;
	}data;
	data.wpos.tag = world_manager::GetWorldTag();
	data.wpos.pos = _parent->pos;
	data.level_req = reply_level_req;
	data.sec_level_req = reply_sec_level_req;
    data.reincarnation_times_req = reply_reincarnation_times_req;
	bool b = GMSV::SendFactionCongregateRequest(OI_GetMafiaID(), _parent->ID.id, &data, sizeof(data));
	if(b) SetCoolDown(COOLDOWN_INDEX_FACTION_CONGREGATE, FACTION_CONGREGATE_COOLDOWN_TIME);
	return b;
}

namespace
{
	bool __check_sec_level(int req, int cur)
	{
		if(cur < req) return false;
		if(req >= 20) //���Ҫ�������Ϊ��ħ�������ƥ��
		{
			if(req/10 != cur/10) return false;
		}
		return true;
	}
}

void 
gplayer_imp::RecvCongregateRequest(char type, int sponsor, int world_tag, const A3DVECTOR& pos, int level_req, int sec_level_req, int reincarnation_times_req)
{
	if(GetPlayerLimit(PLAYER_LIMIT_NOLONGJUMP))//��ֹ���ܶ�����ɼ�����
	{
		return;
	}
	
	if(_basic.level < level_req) return;
	if(_basic.sec.level < sec_level_req) return;
	if( sec_level_req >= 20 && sec_level_req < 40 && ( (sec_level_req/10-1) != _basic.sec.flag) ) return;
	
    if (GetReincarnationTimes() < (size_t)reincarnation_times_req) return;
	size_t i = 0;
	for(; i<_congregate_req_list.size(); i++)
	{
		if(_congregate_req_list[i].type == type) break;
	}
	if(i < _congregate_req_list.size())
	{
		_congregate_req_list[i].sponsor 	= sponsor;
		_congregate_req_list[i].timeout 	= g_timer.get_systime() + CONGREGATE_REQUEST_TIMEOUT;
		_congregate_req_list[i].world_tag 	= world_tag;
		_congregate_req_list[i].pos 		= pos;
	}
	else
	{
		congregate_req req;
		req.type 		= type;
		req.sponsor 	= sponsor;
		req.timeout 	= g_timer.get_systime() + CONGREGATE_REQUEST_TIMEOUT;
		req.world_tag 	= world_tag;
		req.pos			= pos;
		_congregate_req_list.push_back(req);
	}
	_runner->congregate_request(type, sponsor, CONGREGATE_REQUEST_TIMEOUT);
}

void 
gplayer_imp::PlayerCongregateReply(char type, char agree, int sponsor)
{
	int cur_time = g_timer.get_systime();
	abase::vector<congregate_req>::iterator it = _congregate_req_list.begin();
	for(; it!=_congregate_req_list.end(); ++it)
	{
		if(it->type == type && it->sponsor == sponsor && cur_time <= it->timeout) break;
	}
	if(it == _congregate_req_list.end()) return;
		
	if(agree)
	{
		LeaveAbnormalState();
		session_congregate * pSession = new session_congregate(this);
		pSession->SetDestination(type, it->world_tag, it->pos);
		if(AddSession(pSession)) StartSession();
	}
	else
	{
		SendTo<0>(GM_MSG_REJECT_CONGREGATE, XID(GM_TYPE_PLAYER,sponsor), type);
	}
	_congregate_req_list.erase(it);
}

void 
gplayer_imp::PlayerGetDpsDphRank(unsigned char rank_mask)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_DPS_DPH_RANK))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}

	gplayer * pPlayer = (gplayer *)_parent;
	if(!world_manager::GetInstance()->DpsRankSendRank(pPlayer->cs_index, pPlayer->ID.id, pPlayer->cs_sid, rank_mask)) return;
	
	SetCoolDown(COOLDOWN_INDEX_DPS_DPH_RANK, DPS_DPH_RANK_COOLDOWN_TIME);
}

int 
gplayer_imp::PlayerJoinForce(int force_id)
{
	if(!world_manager::GetForceGlobalDataMan().IsDataReady()) return S2C::ERR_SERVICE_UNAVILABLE;
	if(!CheckCoolDown(COOLDOWN_INDEX_PLAYER_JOIN_FORCE)) return S2C::ERR_OBJECT_IS_COOLING;

	if(_player_force.GetForce() != 0) return -1;
	DATA_TYPE dt;
	FORCE_CONFIG * pCfg = (FORCE_CONFIG*) world_manager::GetDataMan().get_data_ptr(force_id, ID_SPACE_CONFIG,dt);
	if(dt != DT_FORCE_CONFIG || pCfg == NULL) return -1;

	if(pCfg->join_money_cost > 0)
	{

		if(GetAllMoney() < (unsigned int)pCfg->join_money_cost) return S2C::ERR_OUT_OF_FUND;
	}
	if(pCfg->join_item_cost > 0)
	{
		if(!IsItemExist(pCfg->join_item_cost)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	}
	
	_player_force.ChangeForce(force_id);
	GMSV::SendPlayerJoinOrLeaveForce(force_id, true);

	if(pCfg->join_money_cost > 0)
	{
		SpendAllMoney(pCfg->join_money_cost,true);
		SelfPlayerMoney();
	}
	if(pCfg->join_item_cost > 0)
	{
		TakeOutItem(pCfg->join_item_cost);
	}
	return 0;	
}

int 
gplayer_imp::PlayerLeaveForce()
{
	if(!world_manager::GetForceGlobalDataMan().IsDataReady()) return S2C::ERR_SERVICE_UNAVILABLE;
	if(!CheckCoolDown(COOLDOWN_INDEX_PLAYER_LEAVE_FORCE)) return S2C::ERR_OBJECT_IS_COOLING;

	int cur_force = _player_force.GetForce();
	if(cur_force == 0) return -1;
	DATA_TYPE dt;
	FORCE_CONFIG * pCfg = (FORCE_CONFIG*) world_manager::GetDataMan().get_data_ptr(cur_force, ID_SPACE_CONFIG,dt);
	if(dt != DT_FORCE_CONFIG || pCfg == NULL) return -1;

	if(pCfg->quit_repution_cost > 0 && pCfg->quit_repution_cost <= 100)
	{
		int repu = _player_force.GetReputation();
		int dec = (int)(repu * 0.01f * pCfg->quit_repution_cost);
		if(dec > 0)
			_player_force.DecReputation(dec);
	}
	if(pCfg->quit_contribution_cost > 0 && pCfg->quit_contribution_cost <= 100)
	{
		int contri = _player_force.GetContribution();
		int dec = (int)(contri * 0.01f * pCfg->quit_contribution_cost);
		if(dec > 0)
			_player_force.DecContribution(dec);
	}
	
	_player_force.ChangeForce(0);
	GMSV::SendPlayerJoinOrLeaveForce(cur_force, false);
	PlayerTaskInterface  task_if(this);
	OnTaskLeaveForce(&task_if);
	
	SetCoolDown(COOLDOWN_INDEX_PLAYER_JOIN_FORCE,PLAYER_JOIN_FORCE_COOLDOWN_TIME);
	SetCoolDown(COOLDOWN_INDEX_PLAYER_LEAVE_FORCE,PLAYER_LEAVE_FORCE_COOLDOWN_TIME);
	return 0;
}

void 
gplayer_imp::PlayerRechargeOnlineAward(int type, unsigned int count, int * list)
{
	if(type < 0 || type >= online_award::MAX_AWARD_TYPE) return;
	
	if(_basic.level < online_award::MIN_LEVEL_REQUIRED) return;
	
	DATA_TYPE dt;
	ONLINE_AWARDS_CONFIG * ess = (ONLINE_AWARDS_CONFIG *)world_manager::GetDataMan().get_data_ptr(ONLINE_AWARD_CONFIG_ID, ID_SPACE_CONFIG, dt);
	if(!ess || dt != DT_ONLINE_AWARDS_CONFIG) return;

	C2S::CMD::recharge_online_award::entry * ent = (C2S::CMD::recharge_online_award::entry *)list;
	abase::vector<char> flag_list;
	flag_list.insert(flag_list.begin(),_inventory.Size(),0);
	unsigned int total_count = 0;
	for(unsigned int i=0; i<count; i++)
	{
		int item_type = ent[i].type;
		if(!ent[i].count || !CheckItemExist(ent[i].index, item_type, ent[i].count)) return;
		if(flag_list[ent[i].index]++) return;
		
		bool find = false;
		for(unsigned int m=0; m<sizeof(ess->choice[type].ids)/sizeof(ess->choice[type].ids[0]); m++)
		{
			if(ess->choice[type].ids[m] <= 0) break;
			if(item_type == (int)ess->choice[type].ids[m])
			{
				find = true;
				break;
			}
		}
		if(!find) return;
		total_count += ent[i].count;
	}

	float inc_time = (float)total_count * ess->choice[type].time;
	if(inc_time <= 0.f || inc_time > 2e9) return;
	if(!_online_award.IncAwardTime(this, type, total_count * ess->choice[type].time)) return;

	for(unsigned int i=0; i<count; i++)
	{
		item& it = _inventory[ent[i].index];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, ent[i].count);
		
		_inventory.DecAmount(ent[i].index, ent[i].count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,ent[i].index,ent[i].type,ent[i].count,S2C::DROP_TYPE_USE);
	}
}

bool 
gplayer_imp::PlayerGetCountryBattlePersonalScore()
{
	if(!GetCountryId()) return false;

	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return false;

	 int combat_time, attend_time, kill_count, death_count, country_kill_count, country_death_count;
	 gplayer * pPlayer = GetParent();
	if(pCtrl->GetPersonalScore(pPlayer->IsBattleOffense(), pPlayer->ID.id, combat_time, attend_time, kill_count, death_count, country_kill_count, country_death_count))
	{
		_runner->countrybattle_personal_score(combat_time,attend_time,kill_count,death_count,country_kill_count,country_death_count);	
		return true;
	}
	return false;
}

bool 
gplayer_imp::PlayerGetCountryBattleStrongholdState()
{
	if(!GetCountryId()) return false;

	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return false;
	
	gplayer * pPlayer = (gplayer*)_parent;
	pCtrl->GetStongholdState(pPlayer->ID.id, pPlayer->cs_index, pPlayer->cs_sid);	
	return true;
}

bool 
gplayer_imp::PlayerGetCountryBattleLiveShow()
{
	if(!GetCountryId()) return false;

	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return false;	
	if(!CheckCoolDown(COOLDOWN_INDEX_COUNTRYBATTLE_LIVESHOW)) return false;

	gplayer * pPlayer = (gplayer*)_parent;
	bool ret = pCtrl->GetLiveShowResult(pPlayer->ID.id, pPlayer->cs_index, pPlayer->cs_sid, _plane);
	SetCoolDown(COOLDOWN_INDEX_COUNTRYBATTLE_LIVESHOW, COUNTRYBATTLE_LIVESHOW_COOLDOWN_TIME);
	return ret;
}

void 
gplayer_imp::PlayerLeaveCountryBattle()
{
	if(!world_manager::GetInstance()->IsCountryBattleWorld()) return;
	
	_filters.ModifyFilter(FILTER_CHECK_INSTANCE_KEY,FMID_CLEAR_AECB,NULL,0);
}

int
gplayer_imp::CheckChangeDs(int type,int climit,int item,int item_count,int level,int sec_lvl,int realm_lvl)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_CHANGEDS)) return S2C::ERR_OBJECT_IS_COOLING;
	if(GetHistoricalMaxLevel() < level) return S2C::ERR_LEVEL_NOT_MATCH;
	

	if( (_basic.sec.level < sec_lvl) || ( sec_lvl >= 20 && sec_lvl < 40 && ( (sec_lvl/10-1) != _basic.sec.flag)) ) return S2C::ERR_SEC_LEVEL_NOT_MATCH;
	
	if(GetRealmLevel() < realm_lvl) return S2C::ERR_REALM_LEVEL_NOT_MATCH;
	if(item && item_count && !CheckItemExist(item,item_count) ) return S2C::ERR_OUT_OF_FUND;
	
	UDOctets val(0);
	world_manager::GetUniqueDataMan().GetData(type + UDI_CARNIVAL_COUNT_LIMIT,val);
	if(climit <= (int)val) return S2C::ERR_CARNIVAL_COUNT_LIMIT; 

	return 0;
}

int 
gplayer_imp::PlayerTryChangeDS(int flag)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_CHANGEDS)) return S2C::ERR_OBJECT_IS_COOLING;
	SetCoolDown(COOLDOWN_INDEX_CHANGEDS,CHANGEDS_COOLDOWN_TIME);
	GMSV::SendTryChangeDS(_parent->ID.id, flag, _player_visa_info.type,_player_mnfaction_info.unifid);
	return 0;
}

void 
gplayer_imp::MakeVisaData(int type,int stay_timestamp,int item,int item_count)
{
	_player_visa_info.type = type;
	_player_visa_info.stay_timestamp = stay_timestamp;
	_player_visa_info.cost = item;
	_player_visa_info.count = item_count;

	GLog::log(GLOG_INFO,"�û�%d����VISA��Ϣ[%d,%d,%d,%d]",_parent->ID.id, type,stay_timestamp,item,item_count);
}

void 
gplayer_imp::PlayerChangeDSLogout(int flag)
{
	if((_player_state != PLAYER_STATE_NORMAL
				&& _player_state != PLAYER_SIT_DOWN
				&& _player_state != PLAYER_STATE_BIND) 
				|| IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_CHANGEDS);
		return;
	}
	LeaveAbnormalState();
	GLog::log(GLOG_INFO,"�û�%dִ�п�������߼�%d",_parent->ID.id, flag);
	_offline_type = PLAYER_OFF_CHANGEDS;
	_team.PlayerLogout();
	Logout(flag);
}

void 
gplayer_imp::PlayerExchangeWanmeiYinpiao(bool is_sell, unsigned int count)
{
	if(_player_state == PLAYER_STATE_MARKET)
	{
		if(_stall_info.stallcard_id == -1)
		{
			//û�а�̯ƾ֤��ֹ�һ�
			return;
		}
		ASSERT(_stall_obj);
		if(is_sell && _stall_obj->IsGoodsExist(WANMEI_YINPIAO_ID))
		{
			//�����б��д�����Ʊ��ֹ�һ�
			return;
		}
	}
	else
	{
		//�ǰ�̯״̬��ֹ�һ�
		return;
	}

	if(_lock_inventory)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return;
	}
	if(world_manager::GetDataMan().get_item_sell_price(WANMEI_YINPIAO_ID) != WANMEI_YINPIAO_PRICE)
	{
		return;
	}
	
	if(is_sell)
	{
		if(!count || !CheckItemExist(WANMEI_YINPIAO_ID, count)) return;
		if((float)count * WANMEI_YINPIAO_PRICE > 2e9) return;
		unsigned int incmoney = count * WANMEI_YINPIAO_PRICE;
		if(!CheckIncGold(incmoney))
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return;
		}
		//
		RemoveItems(WANMEI_YINPIAO_ID, count, S2C::DROP_TYPE_TAKEOUT, false);
		GainGold(incmoney);
		SelfPlayerMoney();
		GLog::log(GLOG_INFO,"�û�%d��%d����Ʊ�һ�Ϊ���%d", _parent->ID.id, count, incmoney);
	}
	else
	{
		if(!count || count > (unsigned int)world_manager::GetDataMan().get_item_pile_limit(WANMEI_YINPIAO_ID)) return;
		if((float)count * WANMEI_YINPIAO_PRICE > 2e9) return;
		unsigned int decmoney = count * WANMEI_YINPIAO_PRICE;
		if(GetGold() < decmoney)
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return;
		}
		if(!_inventory.HasSlot(WANMEI_YINPIAO_ID,count))
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return;
		}
		//
		SpendGold(decmoney,true);
		SelfPlayerMoney();

		element_data::item_tag_t tag = {element_data::IMT_NULL,0};
		item_data * data = world_manager::GetDataMan().generate_item_from_player(WANMEI_YINPIAO_ID, &tag, sizeof(tag));	
		if(data == NULL)
		{
			ASSERT(false);
			return;	
		}
		data->count = count;
		int rst = _inventory.Push(*data);
		if(rst < 0 || data->count)
		{
			ASSERT(false);
			FreeItem(data);
			return;
		}
		_runner->obtain_item(WANMEI_YINPIAO_ID,0,count,_inventory[rst].count,IL_INVENTORY,rst);
		
		FreeItem(data);
		GLog::log(GLOG_INFO,"�û�%d���ѽ��%d�һ�Ϊ%d����Ʊ", _parent->ID.id, decmoney, count);
	}
}

int 
gplayer_imp::PlayerDecomposeFashionItem(unsigned int inv_index, int fashion_type, unsigned int fee, int production_type)
{
	if(inv_index >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & it = _inventory[inv_index];
	if(it.type <= 0 || it.type != fashion_type) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	DATA_TYPE dt; 
	FASHION_ESSENCE * ess = (FASHION_ESSENCE *)world_manager::GetDataMan().get_data_ptr(fashion_type, ID_SPACE_ESSENCE, dt);
	if(ess == NULL || dt != DT_FASHION_ESSENCE) return S2C::ERR_DECOMPOSE_FAILED;
	//���ʱװ�õ���Ⱦɫ����������Ⱦɫʱ��Ҫ������
	if(ess->require_dye_count <= 0) return S2C::ERR_DECOMPOSE_FAILED;
	//���˵�ʱ�޵�ʱװ���ܲ��
	if((it.proc_type & item::ITEM_PROC_TYPE_BIND) || it.expire_date > 0) return S2C::ERR_DECOMPOSE_FAILED;
	
	if(GetAllMoney() < fee) return S2C::ERR_OUT_OF_FUND;

	element_data::item_tag_t tag = {element_data::IMT_NULL, 0};
	item_data * data = world_manager::GetDataMan().generate_item_from_player(production_type, &tag, sizeof(tag));
	if(!data) return S2C::ERR_DECOMPOSE_FAILED;
		
	GLog::log(GLOG_INFO,"�û�%d�ֽ�ʱװ%d�õ�%d��%d",_parent->ID.id,fashion_type,ess->require_dye_count,production_type);
	SpendAllMoney(fee,true);
	SelfPlayerMoney();
	
	UpdateMallConsumptionDestroying(fashion_type, it.proc_type, 1);
	_inventory.DecAmount(inv_index, 1);
	_runner->player_drop_item(IL_INVENTORY,inv_index,fashion_type,1,S2C::DROP_TYPE_DECOMPOSE);

	unsigned int count = ess->require_dye_count;
	bool inv_isfull = false;
	while(count)
	{
		data->count = count;
		if(data->count > data->pile_limit) data->count = data->pile_limit;
		count -= data->count;
		if(!inv_isfull)
		{
			int ocount = data->count;
			int rst;
			if((rst = _inventory.Push(*data)) >= 0)
			{
				_runner->obtain_item(data->type,data->expire_date,ocount-data->count,_inventory[rst].count,0,rst);
			}
		}
		if(data->count)
		{
			DropItemFromData(_plane,_parent->pos,*data,_parent->ID,0,0,_parent->ID.id);
			inv_isfull = true;
		}
	}

	FreeItem(data);
	return 0;
}

void
gplayer_imp::TaskSendMessage(int task_id, int channel, int param)
{
	struct
	{
		int self_id;
		int task_id;
		int channel;
		int param; 
		int world_tag;
		char name[MAX_USERNAME_LENGTH];
	}data;          
	memset(&data, 0,sizeof(data)); 
	data.self_id = _parent->ID.id;
	data.task_id = task_id;
	data.channel = channel;
	data.param = param;
	data.world_tag  = world_manager::GetWorldTag();
	unsigned int  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.name, _username,len);

	switch(channel)
	{
		case GMSV::CHAT_CHANNEL_LOCAL:
		case GMSV::CHAT_CHANNEL_TRADE:
			{
				slice * pPiece = GetParent()->pPiece;
				if(pPiece && _plane)
				{
					AutoBroadcastChatMsg(_plane,pPiece,&data,sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, _chat_emote ,0,0,TASK_CHAT_MESSAGE_ID,0);
				}
				return ;
			}

		case GMSV::CHAT_CHANNEL_FARCRY:
		case GMSV::CHAT_CHANNEL_BROADCAST:
			broadcast_chat_msg(TASK_CHAT_MESSAGE_ID,&data,sizeof(data),GMSV::CHAT_CHANNEL_SYSTEM, _chat_emote,0,0);
			return;

		case GMSV::CHAT_CHANNEL_TEAM:
			SendTeamChat(GMSV::CHAT_CHANNEL_SYSTEM, &data, sizeof(data), 0, 0, TASK_CHAT_MESSAGE_ID);
			return;

		case GMSV::CHAT_CHANNEL_WHISPER:
		case GMSV::CHAT_CHANNEL_FACTION:
			return ;
	}

	return ;
}

void gplayer_imp::ItemMakeSlot(unsigned int index, int id, unsigned int material_id, int material_count)
{
    if (_lock_inventory)
    {
        _runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
        return;
    }

    if(id <= 0) return;

	//�����Ʒ�Ƿ�����Լ��Ƿ�Ϸ�
	if(index >= _inventory.Size() ) return;
	item & it = _inventory[index];
	if(it.type <= 0 || it.type != id || it.body == NULL) return;

	int new_slot = 0;
	if(int err_code = it.body->MakeSlot(this, new_slot, material_id, material_count))
	{
		//�޷���׻��ߴ��ʧ��
		_runner->error_message(err_code);
		return ;
	}
	else
	{
		//����־
		GLog::log(GLOG_INFO,"���%d����Ʒ%d�����˴�ײ�������������%d����", _parent->ID.id, id, new_slot);
		_runner->error_message(S2C::ERR_MAKE_SLOT_SUCCESS);
	}
	
	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,index);
	return ;
}

void 
gplayer_imp::RepairDamagedItem(unsigned char inv_idx)
{
	if(inv_idx >= _inventory.Size()) return;
	item & it = _inventory[inv_idx];
	if(it.type <= 0 || !(it.proc_type & item::ITEM_PROC_TYPE_DAMAGED)) return;

	unsigned int damaged_drop;
	int damaged_drop_count;
	if( (damaged_drop_count = world_manager::GetDataMan().get_item_damaged_drop(it.type,damaged_drop)) <= 0) return;
	DATA_TYPE dt;
	const void * ess = world_manager::GetDataMan().get_data_ptr(damaged_drop,ID_SPACE_ESSENCE,dt);
	if(ess == NULL || dt == DT_INVALID) return;

	damaged_drop_count = (int)ceil(damaged_drop_count*1.2);
	if(!CheckItemExist(damaged_drop, damaged_drop_count)) return;
	
	RemoveItems(damaged_drop, damaged_drop_count, S2C::DROP_TYPE_USE, true);
	it.proc_type &= ~item::ITEM_PROC_TYPE_DAMAGED;
	PlayerGetItemInfo(IL_INVENTORY,inv_idx);
}

void 
gplayer_imp::GodEvilConvert(unsigned char mode)
{
    enum
    {
        MODE_0 = 0,
        MODE_1,
        MODE_MAX,
    };
    static const int ticket_id_list[MODE_MAX] = { GOD_EVIL_CONVERT_TICKET_ID, GOD_EVIL_CONVERT_TICKET_ID2 };
    static const int config_id_list[MODE_MAX][USER_CLASS_COUNT] =
    { 
        { 1774, 1775, 1781, 1777, 1776, 1780, 1778, 1779, 1782, 1783, 1784, 1785, 5148,	5149, 7323 /*monky*/ },  
        { 1786, 1787, 1793, 1789, 1788, 1792, 1790, 1791, 1794, 1795, 1796, 1797, 5150, 5151, 7324 /*monky*/ }
    };

	if (_basic.sec.flag == 0 ) return;
	if (_basic.sec.level != 22 && _basic.sec.level != 32 && 
		_basic.sec.level != 40 && _basic.sec.level != 41 ) return;

	if(mode >= MODE_MAX) return;
    int ticket_id = ticket_id_list[mode];
	if(!CheckItemExist(ticket_id, 1)) return;
    
    int cls = GetPlayerClass();
    if((cls < USER_CLASS_SWORDSMAN) && (cls >= USER_CLASS_COUNT)) return;
    int config_id = config_id_list[mode][cls];

	DATA_TYPE convert_dt;
	GOD_EVIL_CONVERT_CONFIG * convert_ess = (GOD_EVIL_CONVERT_CONFIG *)world_manager::GetDataMan().get_data_ptr(config_id, ID_SPACE_CONFIG, convert_dt);	
	if(!convert_ess || convert_dt != DT_GOD_EVIL_CONVERT_CONFIG) return;	

	RemoveItems(ticket_id, 1, S2C::DROP_TYPE_USE, true);
	std::unordered_map<int,int> convert_table;
	
	for(size_t i=0; i<sizeof(convert_ess->skill_map)/sizeof(convert_ess->skill_map[0]); i++)
	{
		int skill1 = convert_ess->skill_map[i][0], skill2 = convert_ess->skill_map[i][1];
		if(skill1 <= 0 || skill2 <= 0)
		{
			if(skill1 > 0 || skill2 > 0)
                GLog::log(GLOG_ERR, "The skills in the conversion table between fairy and devil are zero at the same time skill_map[%d]", i);
			continue;	
		}
		if(_skill.GetType(skill1) == -1 || _skill.GetType(skill2) == -1)
		{
            GLog::log(GLOG_ERR, "Does not exist in the fairy conversion table skill_map[%d]", i);
			continue;	
		}
		if(convert_table.find(skill1) != convert_table.end() || convert_table.find(skill2) != convert_table.end())
		{
            GLog::log(GLOG_ERR, "There are duplications of skills in the fairy conversion table skill_map[%d]", i);
			continue;
		}
		convert_table.insert(std::make_pair(skill1,skill2));
		convert_table.insert(std::make_pair(skill2,skill1));
	}

	_skill.GodEvilConvert(convert_table, object_interface(this), _cur_item.weapon_class, GetForm(), world_manager::GetWorldTag());
	_runner->get_skill_data();
	
	if (_basic.sec.flag == 1)
		_basic.sec.flag = 2;
	else
		_basic.sec.flag = 1;
	
	if (_basic.sec.level < 40)
	{
		if(_basic.sec.level == 22)
		{
			_basic.sec.level = 32;
		}
		else 
		{
			_basic.sec.level = 22;
		}
	}
	
	SetRefreshState();
	GetParent()->sec_level = _basic.sec.level;
	_runner->task_deliver_level2(_basic.sec.level);
	GetSkillAttr();
}

int 
gplayer_imp::WeddingBook(int start_time, int end_time, int scene, int bookcard_index)
{
	//���봦�ڶ�����ֻ�жӳ��ܷ���������
	if(!IsInTeam() || !IsTeamLeader()) return S2C::ERR_WEDDING_CANNOT_BOOK; 
	if(GetTeamMemberNum() != 2) return S2C::ERR_WEDDING_CANNOT_BOOK;
	//�������������1������ȴ
	if(!CheckAndSetCoolDown(COOLDOWN_INDEX_TEAM_RELATION, 1000)) return S2C::ERR_WEDDING_CANNOT_BOOK;

	//�Ƚ��м򵥵ļ��
	if(!IsMarried()) return S2C::ERR_WEDDING_CANNOT_BOOK;

	if(_inventory.GetEmptySlotCount() < 2) return S2C::ERR_INVENTORY_IS_FULL;	
	if(!CheckItemExist(WEDDING_BOOK_TICKET_ID, 1)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
	int year = 0, month = 0, day = 0;
	if(bookcard_index >= 0)
	{
		if((unsigned int)bookcard_index >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;	
		item & it = _inventory[bookcard_index];
		if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_WEDDING_BOOKCARD) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
		if(!it.GetBookCardData(year,month,day)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	}
	if(!world_manager::GetInstance()->WeddingCheckBook(start_time,end_time,scene,year,month,day)) return S2C::ERR_WEDDING_CANNOT_BOOK;
	
	if(scene < 0 || scene >= WEDDING_SCENE_COUNT) return S2C::ERR_WEDDING_CANNOT_BOOK;
	
	XID list[TEAM_MEMBER_CAPACITY];
	int count = GetMemberList(list);
	ONET::Thread::Pool::AddTask( new WeddingBookJob(this, list, count, start_time, end_time, scene, bookcard_index));
	return 0;
}

int 
gplayer_imp::WeddingCancelBook(int _start_time, int _end_time, int _scene)
{
	//���봦�ڶ�����ֻ�жӳ��ܷ���������
	if(!IsInTeam() || !IsTeamLeader()) return S2C::ERR_WEDDING_CANNOT_CANCELBOOK; 
	if(GetTeamMemberNum() != 2) return S2C::ERR_WEDDING_CANNOT_CANCELBOOK;
	//�������������1������ȴ
	if(!CheckAndSetCoolDown(COOLDOWN_INDEX_TEAM_RELATION, 1000)) return S2C::ERR_WEDDING_CANNOT_CANCELBOOK;

	if(_inventory.GetEmptySlotCount() < 1) return S2C::ERR_INVENTORY_IS_FULL;	
	int start_time, end_time, groom, bride, scene, invitee;
	int rst = 0;
	while( (rst = _inventory.Find(rst,WEDDING_INVITECARD_ID1)) >= 0)
	{
		item & it = _inventory[rst];
		if(it.GetInviteCardData(start_time, end_time, groom, bride, scene, invitee))
		{
			if(start_time == _start_time
					&& end_time == _end_time
					&& scene == _scene
					&& (groom == _parent->ID.id || bride == _parent->ID.id)
					&& invitee == _parent->ID.id)
				break;
		}
		rst++;
	}
	if(rst < 0) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(GetAllMoney() < WEDDING_CANCELBOOK_FEE) return S2C::ERR_OUT_OF_FUND; 
	if(!world_manager::GetInstance()->WeddingCheckCancelBook(start_time,end_time,groom,bride,scene)) return S2C::ERR_WEDDING_CANNOT_CANCELBOOK;

	XID list[TEAM_MEMBER_CAPACITY];
	int count = GetMemberList(list);
	ONET::Thread::Pool::AddTask( new WeddingCancelBookJob(this, list, count, start_time, end_time, scene));
	return 0;
}

int 
gplayer_imp::WeddingInvite(int invitecard_index, int invitee)
{
	if(invitecard_index < 0 || (unsigned int)invitecard_index >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & it = _inventory[invitecard_index];
	if(it.type <= 0 || it.type != WEDDING_INVITECARD_ID1 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_WEDDING_INVITECARD) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	int start_time, end_time, groom, bride, scene, self;
	if(!it.GetInviteCardData(start_time, end_time, groom, bride, scene, self)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(_parent->ID.id != self) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(invitee < 0 || invitee == groom || invitee == bride) return S2C::ERR_PLAYER_NOT_EXIST;
	
	if(!CheckItemExist(WEDDING_INVITE_TICKET_ID,1)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(_inventory.GetEmptySlotCount() < 1) return S2C::ERR_INVENTORY_IS_FULL;	
	//���ɿյ����		
	element_data::item_tag_t tag = {element_data::IMT_NULL, 0};
	item_data * invitecard_data = world_manager::GetDataMan().generate_item_from_player(WEDDING_INVITECARD_ID2, &tag, sizeof(tag)); 
	if(!invitecard_data)
	{
		GLog::log(GLOG_ERR,"���ɱ���ʹ�õ����ʧ��");
		return S2C::ERR_FATAL_ERR;
	}
	//����ڻ������24Сʱ��ʧ
	invitecard_data->expire_date = end_time + 86400;
	int rst = _inventory.PushInEmpty(0, *invitecard_data, 1);
	if(rst >= 0)
	{
		item & it2 = _inventory[rst];
		it2.SetInviteCardData(start_time, end_time, groom, bride, scene, invitee);
		_runner->obtain_item(invitecard_data->type, invitecard_data->expire_date, 1, it2.count, IL_INVENTORY, rst);	
	}
	else
	{
		ASSERT(false);
		FreeItem(invitecard_data);
		return S2C::ERR_FATAL_ERR;
	}
	RemoveItems(WEDDING_INVITE_TICKET_ID, 1, S2C::DROP_TYPE_USE, true);
	FreeItem(invitecard_data);
	return 0;
}

int	
gplayer_imp::GetKnockBackPos(const A3DVECTOR& attacker_pos, float back_distance, A3DVECTOR& back_pos) 
{
	//ʵ�ַ���:ֱ�ߺ��ˣ����ݵ��Ρ���ײ�������˵�
	//����ֵ:0 ���˳ɹ� 1 ����һ���� -1 ��ȫû����
	//������˷���
	A3DVECTOR back_dir = _parent->pos;
	back_dir -= attacker_pos;
	float sq = back_dir.squared_magnitude();
	if(sq <= 1e-6)
	{
		//��������²�����
		return -1;
	}
	back_dir *= 1/sqrt(sq);
	//������˲���
	static const float step_distance = 0.5f;		//������˵�ʱ���õĲ����
	int step_count = int(back_distance/step_distance+0.5f) + 1; //��Ϊʹ��ǰһ���ɹ����˵Ľ��������ʱ�����һ��
	if(step_count < 2) return -1;	//���˾���̫С 
	
	static const float threshold = 0.6f;			//�������˵�ʱ�ĵ�����ֵ
	const A3DVECTOR & ext = aExts2[(IsPlayerFemale() ? 2*GetPlayerClass()+1 : 2*GetPlayerClass())];
	
	A3DVECTOR curpos = _parent->pos;
	A3DVECTOR prevpos = curpos, nextpos;
	A3DVECTOR step_offset;
	int i = 0;
	for( ; i<step_count; i++)
	{
		step_offset = back_dir;
		step_offset *= step_distance;

		nextpos = curpos;
		nextpos += step_offset;
		//���߶�
		float terrain_height = _plane->GetHeightAt(nextpos.x,nextpos.z);
		if(nextpos.y < terrain_height)
		{
			//��������
			float terrain_height0 = _plane->GetHeightAt(curpos.x,curpos.z);
			A3DVECTOR ground_dir(nextpos.x,terrain_height,nextpos.z);
			ground_dir -= A3DVECTOR(curpos.x,terrain_height0,curpos.z);
			if(step_offset.dot_product(ground_dir) <= 0.707*step_distance*sqrt(ground_dir.squared_magnitude()))
			{
				//���˷��������н�>45�ȣ�ֹͣ����
				break;
			}
			
			if(terrain_height > nextpos.y + step_distance*threshold)
			{
				//�Ժ��˵��yֵ�ĵ���Ҫ��������
				break;
			}
			
			//char buf[100] = {0};
			//sprintf(buf,"Height : up adjust %f", terrain_height - nextpos.y);
			//Say(buf);
			
			//�������˵���������
			step_offset.y += terrain_height - nextpos.y ;
			nextpos.y = terrain_height;
		}
		//�����ײ
		trace_manager2 & man = *(_plane->GetTraceMan());
		if(man.Valid())
		{
			bool is_solid;
			float ratio;
			bool bRst = man.AABBTrace(curpos, step_offset, ext, is_solid,ratio,&_plane->w_collision_flags);
			if(bRst)
			{
				if(is_solid) break;//��̫����
				//����ײ����ߵ�ǰ��߶��ڲ���һ�Σ�������ײб�µ����	
				A3DVECTOR tmp = curpos;
				tmp.y += step_distance*threshold;
				bool bRst1 = man.AABBTrace(tmp, step_offset, ext, is_solid,ratio,&_plane->w_collision_flags);
				if(bRst1)
				{
					//����Ҳ����ײ������ֹͣ
					break;
				}
				else
				{
					//����û����ײ,������֮��Ѱ�Һ��˵�	
					tmp += step_offset;
					bool bRst2 = man.AABBTrace(tmp, A3DVECTOR(0.f, -step_distance*threshold, 0.f), ext, is_solid,ratio,&_plane->w_collision_flags);
					if(bRst2)
					{
						if(is_solid) break;//��̫����
						step_offset.y += step_distance*threshold*(1.f-ratio);
						nextpos.y += step_distance*threshold*(1.f-ratio);					
						
						//char buf[100] = {0};
						//sprintf(buf,"AABBTrace : up adjust %f, bRst2 %d solid %d ratio %f",
						//		step_distance*threshold*(1.f-ratio),bRst2,is_solid,ratio);
						//Say(buf);
					}
					else
					{
						;//�����������Ϊ�˺��˵���Ч
					}
				}
			}
		}
	
		prevpos = curpos;
		curpos = nextpos;	
		
		//DropMoneyItem(_plane,curpos,i+1,XID(0,0),0,0,_parent->ID.id);
	}
	//��Ϊʹ��ǰһ���ɹ����˵Ľ�����ɹ������������ϲ���ɹ�
	if(i < 2) return -1;
	//ʹ��ǰһ���ɹ����˵Ľ�����������￨�뽨���ڸ���
	back_pos = prevpos;	
	return (i==step_count ? 0 : 1);	
}

int
gplayer_imp::FactionFortressLevelUp()
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	if(!pCtrl->LevelUp()) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	PlayerGetFactionFortressInfo();
	return 0;
}

int
gplayer_imp::FactionFortressSetTechPoint(unsigned int tech_index)
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	if(!pCtrl->SetTechPoint(tech_index)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	PlayerGetFactionFortressInfo();
	return 0;
}

int
gplayer_imp::FactionFortressResetTechPoint(unsigned int tech_index, unsigned int inv_index, int id)
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;

	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;

	if(FACTION_FORTRESS_RESET_TECH_ITEM_ID != id || !CheckItemExist(inv_index,id,1)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	if(!pCtrl->ResetTechPoint(_plane, tech_index)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	_inventory.DecAmount(inv_index, 1);
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index,id,1,S2C::DROP_TYPE_USE);
	PlayerGetFactionFortressInfo();
	return 0;
}

int
gplayer_imp::FactionFortressConstruct(int id, int accelerate)
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	DATA_TYPE dt;
	FACTION_BUILDING_ESSENCE * ess = (FACTION_BUILDING_ESSENCE *)world_manager::GetDataMan().get_data_ptr(id,ID_SPACE_ESSENCE,dt);
	if(!ess || dt != DT_FACTION_BUILDING_ESSENCE) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	if(GetAllMoney() < (unsigned int)ess->money) return S2C::ERR_OUT_OF_FUND;
	if(!pCtrl->Construct(_plane,id,accelerate)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	SpendAllMoney((unsigned int)ess->money,true);
	SelfPlayerMoney();
	PlayerGetFactionFortressInfo();
	return 0;
}

int
gplayer_imp::FactionFortressHandInMaterial(unsigned int inv_index, int id, unsigned int count)
{
	if(!OI_IsMafiaMember()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;

	if(!count || !CheckItemExist(inv_index,id,count)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(!pCtrl->HandInMaterial(id,count)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	_inventory.DecAmount(inv_index, count);
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index,id,count,S2C::DROP_TYPE_USE);
	PlayerGetFactionFortressInfo();
	
	if(count >= 10)
	{
		struct
		{
			int material_id;
			unsigned int material_count;
			char name[MAX_USERNAME_LENGTH];
		}data;
		memset(&data,0,sizeof(data));
		data.material_id = id;
		data.material_count = count;
		unsigned int  len = _username_len;
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(data.name, _username,len);
		GMSV::FactionBroadcastMsg(OI_GetMafiaID(),GMSV::CMSG_FF_HANDINMATERIAL,&(data),sizeof(data));
	}
	return 0;
}

int
gplayer_imp::FactionFortressHandInContrib(int contrib)
{
	if(!OI_IsMafiaMember()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(contrib <= 0 || GetFactionExpContrib() < contrib) return S2C::ERR_NOT_ENOUGH_FACTION_CONTRIB;
	if(!pCtrl->HandInContrib(contrib)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	DecFactionContrib(0,contrib);
	PlayerGetFactionFortressInfo();
	
	if(contrib >= 5000)
	{
		struct
		{
			int contrib;
			char name[MAX_USERNAME_LENGTH];
		}data;
		memset(&data,0,sizeof(data));
		data.contrib = contrib;
		unsigned int  len = _username_len;
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(data.name, _username,len);
		GMSV::FactionBroadcastMsg(OI_GetMafiaID(),GMSV::CMSG_FF_HANDINCONTRIB,&(data),sizeof(data));
	}
	return 0;
}

int 
gplayer_imp::FactionFortressMaterialExchange(unsigned int src_index,unsigned int dst_index,int material)
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	if(!pCtrl->MaterialExchange(src_index,dst_index,material)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	PlayerGetFactionFortressInfo();
	return 0;
}

int 
gplayer_imp::FactionFortressDismantle(int id)
{
	if(!OI_IsMafiaMaster()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return S2C::ERR_FACTION_FORTRESS_OP_DENYED;
	if(!pCtrl->Dismantle(_plane,id)) return S2C::ERR_FACTION_FORTRESS_OP_FAILED;
	PlayerGetFactionFortressInfo();
	return 0;
}

void 
gplayer_imp::FindSpecItem(unsigned char where, int type, archive& ar)
{
	item_list * plist = NULL;
	if(where <= IL_INVENTORY_END) plist = &GetInventory(where);
	else if(where >= IL_TRASH_BOX_BEGIN && where <= IL_TRASH_BOX_END) plist = &GetTrashInventory(where);
	else return;
	
	int start = 0;
	while( (start = (*plist).Find(start, type)) >= 0)
	{
		ar << where << (unsigned char)start << (*plist)[start].count;
		start ++;	
	}
}

int
gplayer_imp::RemoveSpecItem(unsigned char where, unsigned char index, unsigned int count, int type)
{
	item_list * plist = NULL;
	if(where <= IL_INVENTORY_END) plist = &GetInventory(where);
	else if(where >= IL_TRASH_BOX_BEGIN && where <= IL_TRASH_BOX_END) plist = &GetTrashInventory(where);
	else return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	if(index >= (*plist).Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & it = (*plist)[index];
	if(it.type != type || it.count < count) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	(*plist).DecAmount(index, count);
	_runner->player_drop_item(where,index,type,count,S2C::DROP_TYPE_GM);
	if(where == IL_EQUIPMENT)
	{
		//����װ���ı��־
		IncEquipChangeFlag();
		//����ˢ��װ��
		RefreshEquipment();
		//����������Ϣ
		CalcEquipmentInfo();
		_runner->equipment_info_changed(0,1ULL<<index,0,0); //�˺���ʹ����CalcEquipmentInfo�Ľ��
	}
	
	return S2C::ERR_SUCCESS;
}

bool
gplayer_imp::TryTransmitSkillAttack(const MSG & msg)
{
	if(!_skill_attack_transmit_target.IsActive()) return false;
	if(_skill_attack_transmit_target == msg.source) return false;
	world::object_info info;
	if(!_plane->QueryObject(_skill_attack_transmit_target,info)) return false;
	if(info.state & world::QUERY_OBJECT_STATE_ZOMBIE) return false;
	
	if(msg.message == GM_MSG_ATTACK)
	{
		const attack_msg & ack_msg = *(const attack_msg*)msg.content;
		if(ack_msg.skill_id == 0) return false;
		//���ӹ�����Χ
		attack_msg new_ack_msg(ack_msg);
		new_ack_msg.attack_range += sqrt(info.pos.squared_distance(_parent->pos));
		MSG newmsg(msg);
		newmsg.target = _skill_attack_transmit_target;
		newmsg.content_length = sizeof(new_ack_msg);
		newmsg.content = &new_ack_msg;
		_plane->PostLazyMessage(newmsg);
		return true;
	}
	else if(msg.message == GM_MSG_ENCHANT)
	{
		const enchant_msg & ech_msg = *(const enchant_msg*)msg.content;
		if(ech_msg.helpful) return false;
		MSG newmsg(msg);
		newmsg.target = _skill_attack_transmit_target;
		_plane->PostLazyMessage(newmsg);
		return true;
	}
	else
	{
		ASSERT(false);
		return false;
	}
}

bool 
gplayer_imp::IncForceReputation(int value)
{
	if(_player_force.GetForce() == 0) return false;
	DATA_TYPE dt;
	FORCE_CONFIG * pCfg = (FORCE_CONFIG*) world_manager::GetDataMan().get_data_ptr(_player_force.GetForce(), ID_SPACE_CONFIG,dt);
	if(dt != DT_FORCE_CONFIG || pCfg == NULL) return false;

	int repu = _player_force.GetReputation();
	if(value > pCfg->reputation_max - repu) value = pCfg->reputation_max - repu;
	if(value <= 0) return false;

	//�������ߵ�������
	if(_force_ticket_info.repu_inc_ratio > 0 && 
			(_force_ticket_info.require_force == 0 || _force_ticket_info.require_force == _player_force.GetForce()) )
	{
		int rst = _equipment[item::EQUIP_INDEX_FORCE_TICKET].AutoAdjust(value, pCfg->reputation_max - repu);
		if(rst == 0)
		{
			item& it = _equipment[item::EQUIP_INDEX_FORCE_TICKET];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
			
			int type = _equipment[item::EQUIP_INDEX_FORCE_TICKET].type;
			_equipment.DecAmount(item::EQUIP_INDEX_FORCE_TICKET,1);
			_runner->player_drop_item(IL_EQUIPMENT,item::EQUIP_INDEX_FORCE_TICKET,type,1,S2C::DROP_TYPE_USE);
		}
		else if(rst > 0)
		{
			PlayerGetItemInfo(IL_EQUIPMENT,item::EQUIP_INDEX_FORCE_TICKET);
		}
	}

	if(value > 0) _player_force.IncReputation(value);
	return true;
}

bool
gplayer_imp::DecForceReputation(int value)
{
	if(_player_force.GetForce() == 0) return false;
	int repu = _player_force.GetReputation();
	if(value > repu) value = repu;
	if(value <= 0) return false;
	_player_force.DecReputation(value);
	return true;
}

bool
gplayer_imp::IncForceContribution(int value)
{
	if(_player_force.GetForce() == 0) return false;
	DATA_TYPE dt;
	FORCE_CONFIG * pCfg = (FORCE_CONFIG*) world_manager::GetDataMan().get_data_ptr(_player_force.GetForce(), ID_SPACE_CONFIG,dt);
	if(dt != DT_FORCE_CONFIG || pCfg == NULL) return false;

	int contri = _player_force.GetContribution();
	if(value > pCfg->contribution_max - contri) value = pCfg->contribution_max - contri;
	if(value <= 0) return false;
	_player_force.IncContribution(value);
	return true;
}

bool
gplayer_imp::DecForceContribution(int value)
{
	if(_player_force.GetForce() == 0) return false;
	int contri = _player_force.GetContribution();
	if(value > contri) value = contri;
	if(value <= 0) return false;
	_player_force.DecContribution(value);
	return true;
}

void 
gplayer_imp::UpdateForceTicketInfo(int require_force, int repu_inc_ratio)
{
	_force_ticket_info.require_force = require_force;
	_force_ticket_info.repu_inc_ratio = repu_inc_ratio;
}

int 
gplayer_imp::CountryJoinApply()
{
	if(!CheckCoolDown(COOLDOWN_INDEX_COUNTRY_JOIN_APPLY)) return S2C::ERR_OBJECT_IS_COOLING; // Em espera

	if(GetCountryId()) return S2C::ERR_ALREADY_JOIN_COUNTRY; // Já está em um mapa

	if(EmulateSettings::GetInstance()->GetNwConfig()->min_level_required_nw > 0)
	if(GetHistoricalMaxLevel() < EmulateSettings::GetInstance()->GetNwConfig()->min_level_required_nw) return S2C::ERR_LEVEL_NOT_MATCH;

	if(EmulateSettings::GetInstance()->GetNwConfig()->min_level2_required_nw > 0)
	if(_basic.sec.level < EmulateSettings::GetInstance()->GetNwConfig()->min_level2_required_nw) return S2C::ERR_SEC_LEVEL_NOT_MATCH;

	if(EmulateSettings::GetInstance()->GetNwConfig()->min_realm_level_required_nw > 0)
	if(GetRealmLevel() < EmulateSettings::GetInstance()->GetNwConfig()->min_realm_level_required_nw) return S2C::ERR_REALM_LEVEL_NOT_MATCH;

	if(!IsItemExist(COUNTRYJOIN_APPLY_TICKET)) return S2C::ERR_ITEM_NOT_IN_INVENTORY; // Item

	if(IsInTeam() && EmulateSettings::GetInstance()->GetNwConfig()->enabled_team > 0) return S2C::ERR_NOT_JOIN_COUNTRY; // Verifica se está em um grupo, caso esteja, ele cancela a entrada
		
	if(!IsInTeam())
	{
		SetCoolDown(COOLDOWN_INDEX_COUNTRY_JOIN_APPLY,COUNTRY_JOIN_APPLY_COOLDOWN_TIME);
		
		GMSV::CBApplyEntry entry;
		entry.roleid = _parent->ID.id;
		entry.major_strength = Get16Por9JWeapon();
		entry.minor_strength = GetSoulPower();
		GMSV::CountryBattleApply(&entry, 1);
		return 0;
	} 
	
	if(EmulateSettings::GetInstance()->GetNwConfig()->enabled_team > 0 && IsInTeam()) 
	{
		for(int i=0; i<GetTeamMemberNum(); i++)
		{
			if(GetTeamMember(i).data.level < EmulateSettings::GetInstance()->GetNwConfig()->min_level_required_nw) return S2C::ERR_LEVEL_NOT_MATCH;
		}

		XID list[TEAM_MEMBER_CAPACITY];
		int count = GetMemberList(list);
		ONET::Thread::Pool::AddTask(new CountryJoinApplyJob(this, list, count, EmulateSettings::GetInstance()->GetNwConfig()->min_level_required_nw, COUNTRYJOIN_APPLY_TICKET));
		return 0;
	}	
}

bool 
gplayer_imp::CountryJoinStep1(int country_id, int country_expiretime, int major_strength, int minor_strength, int world_tag, const A3DVECTOR & pos)
{
	ASSERT(country_expiretime > 0 && "������Ҷ�Ӧ���й���ʱ��");
	if(GetCountryId()) return false;
	if(!IsItemExist(COUNTRYJOIN_APPLY_TICKET)) return false;
	
	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_STATE_BIND
			&& _player_state != PLAYER_SIT_DOWN
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return false;
	}
	LeaveAbnormalState();

	if(world_tag == world_manager::GetWorldTag()) return false;
	//׼����������ս�׶������ͳɹ�����м�����Ҳ���
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(world_tag, key);
	key.target = key.essence;

	key.target.key_level4 = (country_id >> 16) &0xffff; // group id

	ClearSwitchAdditionalData();
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,world_tag,key,0) < 0)
	{
		return false;
	}
	
	_switch_additional_data = new countryterritory_switch_data(country_id, country_expiretime, major_strength, minor_strength);

	GLog::log(GLOG_INFO,"���%d�������step1(id=%d,expire=%d)׼������(tag=%d,pos=%f,%f,%f)", _parent->ID.id, country_id, country_expiretime, world_tag, pos.x, pos.y, pos.z);
	return true;
}

bool 
gplayer_imp::CountryJoinStep2()
{
	countryterritory_switch_data * pData = substance::DynamicCast<countryterritory_switch_data>(_switch_additional_data);
	ASSERT(pData);
	if(GetCountryId()) return false;
	if(!IsItemExist(COUNTRYJOIN_APPLY_TICKET)) return false;
	
	TakeOutItem(COUNTRYJOIN_APPLY_TICKET);
	SetCountryId(pData->country_id, pData->country_expiretime);
	GMSV::CountryBattleJoin(_parent->ID.id, pData->country_id, world_manager::GetWorldTag(), pData->major_strength, pData->minor_strength, GetParent()->IsKing());
	GLog::log(GLOG_INFO,"���%d�������step2(id=%d,expire=%d)�ɹ�", _parent->ID.id, pData->country_id, pData->country_expiretime);

	ClearSwitchAdditionalData();
	return true;
}

bool 
gplayer_imp::CountryReturn()
{
	LeaveAbnormalState();

	A3DVECTOR pos;
	int tag;	
	GetLastInstanceSourcePos(tag,pos);

	if(tag != 143)
	{
		tag = 143;
		pos = A3DVECTOR(0,0,0);
		GLog::log(GLOG_ERR,"������������ world_tag=%d roleid=%d country=%d", world_manager::GetWorldTag(), _parent->ID.id, GetCountryId());
	}

	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(tag, key);
	key.target = key.essence;

	key.target.key_level4 = GetCountryGroup(); // group id
	
	ClearSwitchAdditionalData();
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,tag,key,0) < 0)
	{
		return false;
	}
	return true;
}

bool 
gplayer_imp::ReturnRestWorld()
{
	LeaveAbnormalState();

	if(InCentralServer())
	{
		int groupid = 0;
		int tag = 142; 
		A3DVECTOR pos = world_manager::GetCentralServerBrithPos(_src_zoneid,groupid);

		instance_key key;
		memset(&key,0,sizeof(key));
		GetInstanceKey(tag, key);
		key.target = key.essence;

		key.target.key_level4 = groupid; // group id
		
		ClearSwitchAdditionalData();
		if(world_manager::GetInstance()->PlaneSwitch(this ,pos,tag,key,0) < 0)
		{
			return false;
		}
	}
	else
	{
		A3DVECTOR pos = A3DVECTOR(1485,225,1269);
		int tag = 1;
		return LongJump(pos,tag);
	}
	
	return true;
}

int 
gplayer_imp::CountryLeave()
{
	if(!GetCountryId()) return S2C::ERR_NOT_JOIN_COUNTRY;

	GMSV::CountryBattleLeave(_parent->ID.id, GetCountryId(), Get16Por9JWeapon(), GetSoulPower());
	//�������ID �ȴ���ս��ͼ�߼�������߳�
	SetCountryId(0,0);
	return 0;
}

void 
gplayer_imp::CountryTerritoryMove(const A3DVECTOR & pos, bool capital)
{
	if(!world_manager::GetInstance()->IsCountryTerritoryWorld()) return;

	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_STATE_BIND
			&& _player_state != PLAYER_SIT_DOWN
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}
	LeaveAbnormalState();
	LongJump(pos);
	//���׶�������ΪGM����
	object_interface obj_if(this);
	if(capital)
		obj_if.SetGMInvisibleFilter(true, -1, filter::FILTER_MASK_NOSAVE);
	else
		obj_if.SetGMInvisibleFilter(false, -1, 0);
}

void 
gplayer_imp::GetCarnivalKickoutPos(int & world_tag, A3DVECTOR & pos)
{
	if(InCentralServer())
	{
		int group = 0;
		world_tag = 142;
		pos = world_manager::GetCentralServerBrithPos(_src_zoneid,group);
	}
	else
	{
		world_tag = 1;
		pos = A3DVECTOR(1485,225,1269);
	}
}

bool
gplayer_imp::OI_TestSafeLock()
{
	return ((gplayer_controller*)_commander)->InSafeLock();
}

void
gplayer_imp::OI_TryCancelPlayerBind()
{
	if(_player_state == PLAYER_STATE_BIND)
		_bind_player.PlayerCancel(this);
}

//lgc
void gplayer_imp::UpdateCurElfInfo(unsigned int id, short refine_level, short str, short agi, short vit, short eng, const char * skilldata, int skillcnt)
{
	//�˺�������С����װ��ʱOnActivate()�е���,�˺��������ʼ��refine_effect_active,vigor
	if(_cur_elf_info.id != (unsigned int)-1)	//��ֹС���鱻�ظ�װ��
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}

	_cur_elf_info.id = id;
	_cur_elf_info.refine_level = refine_level;
	_cur_elf_info.strength = str;
	_cur_elf_info.agility = agi;
	_cur_elf_info.vitality = vit;
	_cur_elf_info.energy = eng;

	ASSERT(_cur_elf_info.skillvect.size() == 0);

	struct elf_skill_data * pskill = (struct elf_skill_data *)skilldata;
	if(skilldata != NULL && skillcnt > 0 && skillcnt <= MAX_ELF_SKILL_CNT)
		for(int i=0; i<skillcnt; i++, pskill++)
			_cur_elf_info.skillvect.push_back(*pskill);
	
	//�������Ҫ���Ӹ���_elf_en�Ĵ��룬Ȼ�������������
	UpdateElfProp();
	UpdateElfVigor();	

}

void gplayer_imp::ClearCurElfInfo()
{
	//�˺�������С����ж��ʱOnDeactivate()�е���, �˺��������ʼ��refine_effect_active,vigor
	_cur_elf_info.clear();
}

void gplayer_imp::UpdateElfProp() //��elf_enhance���µ�_cur_elf_info��final_...��
{
	_cur_elf_info.final_strength = _cur_elf_info.strength + _elf_en.str_point;	
	_cur_elf_info.final_agility = _cur_elf_info.agility + _elf_en.agi_point;	
	_cur_elf_info.final_vitality = _cur_elf_info.vitality + _elf_en.vit_point;	
	_cur_elf_info.final_energy = _cur_elf_info.energy + _elf_en.eng_point;	
	_cur_elf_info.refresh_enhance = true;
	
}

void gplayer_imp::UpdateElfVigor()
{
	_cur_elf_info.max_vigor = INITIAL_MAX_VIGOR + _cur_elf_info.final_vitality;
	_cur_elf_info.vigor_gen = INITIAL_VIGOR_GEN + _cur_elf_info.final_energy * 0.02f;
	if(_cur_elf_info.vigor > _cur_elf_info.max_vigor)
		_cur_elf_info.vigor = _cur_elf_info.max_vigor;
	_cur_elf_info.refresh_vigor = true;
	
}

void gplayer_imp::UpdateMinElfStatusValue(int value)
{
	//���õ�ʱ��֤value > 0
	if(_min_elf_status_value <= 0)
		_min_elf_status_value = value;
	else if(_min_elf_status_value > value)
		_min_elf_status_value = value;
}

void gplayer_imp::UpdateAllElfSecureStatus()
{
	_min_elf_status_value = 0;
	int t = g_timer.get_systime();
	unsigned int i, size;
	//���_inventory
	size = _inventory.Size();
	for(i=0; i<size; i++)
	{
		int value = _inventory[i].GetStatusValue();
		if(value <= 0) 
			continue;
		if(value <= t)
		{
			_inventory[i].UpdateElfSecureStatus();
			_inventory[i].proc_type &= ~ (item::ITEM_PROC_TYPE_NOTHROW|item::ITEM_PROC_TYPE_NOSELL|item::ITEM_PROC_TYPE_NOTRADE);
			PlayerGetItemInfo(IL_INVENTORY, i);//�ͻ��˸���
		}
		else
		{
			if(_min_elf_status_value <= 0)
				_min_elf_status_value = value;
			else if(_min_elf_status_value > value)
				_min_elf_status_value = value;
		}
	}
	//���_equipment
	size = _equipment.Size();
	for(i=0; i<size; i++)
	{
		int value = _equipment[i].GetStatusValue();
		if(value <= 0) 
			continue;
		if(value <= t)
		{
			_equipment[i].UpdateElfSecureStatus();
			_equipment[i].proc_type &= ~ (item::ITEM_PROC_TYPE_NOTHROW|item::ITEM_PROC_TYPE_NOSELL|item::ITEM_PROC_TYPE_NOTRADE);
			PlayerGetItemInfo(IL_EQUIPMENT, i);//�ͻ��˸���
		}
		else
		{
			if(_min_elf_status_value <= 0)
				_min_elf_status_value = value;
			else if(_min_elf_status_value > value)
				_min_elf_status_value = value;
		}
	}
	//���_backpack
	item_list & backpack1 = _trashbox.GetBackpack(0);
	size = backpack1.Size();
	for(i=0; i<size; i++)
	{
		int value = backpack1[i].GetStatusValue();
		if(value <= 0) 
			continue;
		if(value <= t)
		{
			backpack1[i].UpdateElfSecureStatus();
			backpack1[i].proc_type &= ~ (item::ITEM_PROC_TYPE_NOTHROW|item::ITEM_PROC_TYPE_NOSELL|item::ITEM_PROC_TYPE_NOTRADE);
			PlayerGetItemInfo(IL_TRASH_BOX, i);//�ͻ��˸���
		}
		else
		{
			if(_min_elf_status_value <= 0)
				_min_elf_status_value = value;
			else if(_min_elf_status_value > value)
				_min_elf_status_value = value;
		}
	}
	//��鹫���ֿ�
	item_list & user_backpack1 = _user_trashbox.GetBackpack(0);
	size = user_backpack1.Size();
	for(i=0; i<size; i++)
	{
		int value = user_backpack1[i].GetStatusValue();
		if(value <= 0) 
			continue;
		if(value <= t)
		{
			user_backpack1[i].UpdateElfSecureStatus();
			user_backpack1[i].proc_type &= ~ (item::ITEM_PROC_TYPE_NOTHROW|item::ITEM_PROC_TYPE_NOSELL|item::ITEM_PROC_TYPE_NOTRADE);
			PlayerGetItemInfo(IL_USER_TRASH_BOX, i);//�ͻ��˸���
		}
		else
		{
			if(_min_elf_status_value <= 0)
				_min_elf_status_value = value;
			else if(_min_elf_status_value > value)
				_min_elf_status_value = value;
		}
	}
}

void gplayer_imp::TriggerElfRefineEffect()
{
	//�Ƿ�װ����С����
	if(_cur_elf_info.id == (unsigned int)-1)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	//�����ȼ�����	
	if(_cur_elf_info.refine_level <= 0 || _cur_elf_info.refine_level > MAX_ELF_REFINE_LEVEL)
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_ACTIVATE_FAILED);
		return;
	}
	if(!_cur_elf_info.refine_effect_active)
	{
		//�жϵ�ǰ�����Ƿ��㹻
		int startup_cost = elf_refine_effect_table[_cur_elf_info.refine_level].std_cost * (_basic.level  + 105)/210 * 60;
		int cur_stamina = it.GetStamina();
		if(cur_stamina < startup_cost)
		{
			_runner->error_message(S2C::ERR_ELF_NOT_ENOUGH_STAMINA);
			return;
		}
		//��������
		it.DecStamina(startup_cost);
		_runner->query_elf_stamina(cur_stamina - startup_cost);//֪ͨ�ͻ���
		ElfRefineActivate(_cur_elf_info.refine_level);
		_runner->elf_refine_activate(1);
	}
	else
	{
		ElfRefineDeactivate(_cur_elf_info.refine_level);
		_runner->elf_refine_activate(0);
	}
}

void gplayer_imp::ElfRefineActivate(short refine_level)
{
	if(refine_level <= 0 || refine_level > MAX_ELF_REFINE_LEVEL)
		return;
	_en_point.max_hp += elf_refine_effect_table[refine_level].max_hp;
	_defend_degree += elf_refine_effect_table[refine_level].defend_degree;
	_attack_degree += elf_refine_effect_table[refine_level].attack_degree;
	//player ����
	property_policy::UpdateLife(this);
	PlayerGetProperty();
	SetRefreshState();
	
	gplayer * player = GetParent();
	player->object_state |= gactive_object::STATE_ELF_REFINE_ACTIVATE;
	
	_cur_elf_info.refine_effect_active = true;
}

void gplayer_imp::ElfRefineDeactivate(short refine_level)
{
	if(refine_level <= 0 || refine_level > MAX_ELF_REFINE_LEVEL)
		return;
	_en_point.max_hp -= elf_refine_effect_table[refine_level].max_hp;
	_defend_degree -= elf_refine_effect_table[refine_level].defend_degree;
	_attack_degree -= elf_refine_effect_table[refine_level].attack_degree;
	//player ����
	property_policy::UpdateLife(this);
	PlayerGetProperty();
	SetRefreshState();
	
	gplayer * player = GetParent();
	player->object_state &= ~gactive_object::STATE_ELF_REFINE_ACTIVATE;
		
	_cur_elf_info.refine_effect_active = false;
}

void gplayer_imp::ElfAddAttribute(short str, short agi, short vit, short eng)
{
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	//�����Ƿ����
	if(!it.AddAttributePoint(str, agi, vit, eng, true))
	{
		_runner->error_message(S2C::ERR_ELF_ADD_ATTRIBUTE_FAILED);
		return;
	}
	//�޸�
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);
	it.AddAttributePoint(str, agi, vit, eng, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);
	PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
}

void gplayer_imp::ElfAddGenius(short g0, short g1, short g2, short g3, short g4)
{
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	//�����Ƿ����
	if(!it.AddGeniusPoint(g0, g1, g2, g3, g4, true))
	{
		_runner->error_message(S2C::ERR_ELF_ADD_GENIUS_FAILED);
		return;
	}
	//�޸�
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);
	it.AddGeniusPoint(g0, g1, g2, g3, g4, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);
	PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
	_runner->elf_cmd_result(S2C::ELF_ADD_GENIUS, it.type, 0, 0);
}

bool ElfFixLevelUp(item it, unsigned int exp)
{
  return exp && 
  exp < 140000000u && 
  exp <= (unsigned int)player_template::GetLvlupExp(0, 105) && 
  (unsigned int)it.GetLevel() < 105;
}


void gplayer_imp::ElfPlayerInsertExp(unsigned int exp, char use_sp)
{
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_ELF_CMD))
	{
		_runner->error_message(S2C::ERR_ELF_CMD_IN_COOLDOWN);
		return;	
	}
	_cooldown.SetCoolDown(COOLDOWN_INDEX_ELF_CMD, ELF_CMD_COOLDOWN_TIME);

	// Novas rates 
	exp *= EmulateSettings::GetInstance()->GetRatesConfig()->genie_exp;
	
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	if( !ElfFixLevelUp(it,exp) ) return; 
	
	
	//����ʹ������Ԫ��;�����ɽ��׵�С����ע����
	if(it.GetSecureStatus() == elf_item::STATUS_TRADABLE)
	{
		_runner->error_message(S2C::ERR_ELF_PLAYER_INSERT_EXP_FAILED);
		return;
	}
	//�����Ƿ����
	if(use_sp)
	{
		if(exp <= 0 || _basic.skill_point <= 0 || exp > (unsigned int)_basic.skill_point || exp > 840000000)
		{
			_runner->error_message(S2C::ERR_ELF_PLAYER_INSERT_EXP_FAILED);
			return;
		}
		exp *= 5; 
	}
	else
	{
		if(exp <= 0 || _basic.exp <= 0 || exp > (unsigned int)_basic.exp)
		{
			_runner->error_message(S2C::ERR_ELF_PLAYER_INSERT_EXP_FAILED);
			return;
		}
	}
	bool is_levelup = false;
	if(it.InsertExp(exp, GetHistoricalMaxLevel(), this, is_levelup, true) == (unsigned int)-1)//����player level>=elf level
	{
		_runner->error_message(S2C::ERR_ELF_PLAYER_INSERT_EXP_FAILED);
		return;
	}
	//�޸�	
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);	//����û�б�Ҫ�ӣ�������������������ʽ��һ��
	unsigned int _exp = it.InsertExp(exp, GetHistoricalMaxLevel(), this, is_levelup, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);	//����û�б�Ҫ�ӣ�������������������ʽ��һ��
	//player����_exp
	if(use_sp)
		_basic.skill_point -= (_exp%5 ? (int)(_exp/5+1) : (int)(_exp/5));
	else
		_basic.exp -= (int)_exp;
	SetRefreshState();
	GLog::log(GLOG_INFO,"�û�%d��%d��%s��С����ע����,���о���%d,Ԫ��%d", _parent->ID.id, (use_sp?_exp/5:_exp), (use_sp?"Ԫ��":"����"), _basic.exp, _basic.skill_point);
	if(is_levelup)
		PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
}

unsigned int gplayer_imp::ElfInsertExpUsePill(unsigned int exp, int exp_level)
{
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_ELF_CMD))
	{
		_runner->error_message(S2C::ERR_ELF_CMD_IN_COOLDOWN);
		return (unsigned int)-1;	
	}
	_cooldown.SetCoolDown(COOLDOWN_INDEX_ELF_CMD, ELF_CMD_COOLDOWN_TIME);
	
	// Novas rates 
	exp *= EmulateSettings::GetInstance()->GetRatesConfig()->genie_exp;
	
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	if( !ElfFixLevelUp(it,exp) ) return -1; 
	//�����Ƿ����
	if(exp <= 0) return (unsigned int)-1;
	bool is_levelup = false;
	if(it.InsertExp(exp, exp_level, this, is_levelup, true) == (unsigned int)-1)//����player level>=elf level
	{
		_runner->error_message(S2C::ERR_ELF_INSERT_EXP_USE_PILL_FAILED);
		return (unsigned int)-1;
	}
	//�޸�	
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);	//����û�б�Ҫ�ӣ�������������������ʽ��һ��
	unsigned int _exp = it.InsertExp(exp, exp_level, this, is_levelup, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);	//����û�б�Ҫ�ӣ�������������������ʽ��һ��
	if(is_levelup)
		PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
	return _exp;
}

void gplayer_imp::ElfReceiveExp(unsigned int exp)
{
	// Novas rates 
	exp *= EmulateSettings::GetInstance()->GetRatesConfig()->genie_exp;

	if(!exp) return;
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	if( !ElfFixLevelUp(it,exp) ) return; 
	
	bool is_levelup = false;
	if(it.InsertExp(exp, GetHistoricalMaxLevel(), this, is_levelup, false) == (unsigned int)-1) return;
	if(is_levelup)
		PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);
}

void gplayer_imp::ElfEquipItem(unsigned int index_inv)
{
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	if(index_inv >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it1 = _inventory[index_inv];
	if(it1.type <= 0 || it1.body == NULL || it1.body->GetItemType() != item_body::ITEM_TYPE_ELF_EQUIP)
	{
		_runner->error_message(S2C::ERR_ELF_EQUIP_ITEM_FAILED);
		return;
	}
	//�����Ƿ����
	if(!it.EquipElfItem(it1.type, true))
	{
		_runner->error_message(S2C::ERR_ELF_EQUIP_ITEM_FAILED);
		return;
	}
	//�޸�
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);
	it.EquipElfItem(it1.type, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);
	//��Ʒ��������Ʒ
	_runner->player_drop_item(IL_INVENTORY, index_inv, it1.type, 1, S2C::DROP_TYPE_TAKEOUT);
	_inventory.DecAmount(index_inv, 1);
	PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
	_runner->elf_cmd_result(S2C::ELF_EQUIP_ITEM, it.type, it1.type, 0);
}

void gplayer_imp::ElfChangeSecureStatus(int status)
{
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_ELF_CMD))
	{
		_runner->error_message(S2C::ERR_ELF_CMD_IN_COOLDOWN);
		return;	
	}
	_cooldown.SetCoolDown(COOLDOWN_INDEX_ELF_CMD, ELF_CMD_COOLDOWN_TIME);
	
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	//�����Ƿ����
	if(!it.ChangeElfSecureStatus(status, true))
	{
		_runner->error_message(S2C::ERR_ELF_CHANGE_SECURE_STATUS_FAILED);
		return;
	}
	//�޸�
	it.Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);	//û�ñ���������һ��
	it.ChangeElfSecureStatus(status, false);
	it.Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);	//û�ñ���������һ��
	//
	if(status == elf_item::STATUS_SECURE)
		it.proc_type |= (item::ITEM_PROC_TYPE_NOTHROW|item::ITEM_PROC_TYPE_NOSELL|item::ITEM_PROC_TYPE_NOTRADE);
	else if(status == elf_item::STATUS_TRANSFORM) 
		UpdateMinElfStatusValue(it.GetStatusValue());//����_min_elf_status_value
	
	PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);//�ͻ���װ������
}

void gplayer_imp::CastElfSkill(unsigned short skill_id, char force_attack, int target_count, int * targets)
{
	//�Ƿ�װ����С����
	if(_cur_elf_info.id == (unsigned int)-1)
	{
		_runner->error_message(S2C::ERR_CAST_ELF_SKILL_FAILED);
		return;
	}
	//��ǰС���� �Ƿ���ü��ܣ�id level��
	short skill_level;
	unsigned int i, size = _cur_elf_info.skillvect.size();
	for(i=0; i<size; i++)
	{
		if(_cur_elf_info.skillvect[i].id == skill_id) 
		{
			ASSERT(_cur_elf_info.skillvect[i].level > 0);
			skill_level = _cur_elf_info.skillvect[i].level;
			break;
		}
	}
	if(i == size)
	{
		_runner->error_message(S2C::ERR_CAST_ELF_SKILL_FAILED);
		return;
	}
	//���С���鼼�ܵĹ�����ȴ
	if(!_cooldown.TestCoolDown(COOLDOWN_INDEX_CAST_ELF_SKILL))
	{
		_runner->error_message(S2C::ERR_CAST_ELF_SKILL_IN_COOLDOWN);
		return;	
	}
	//��鼼�ܵ���ȴ
	int cd_id = GNET::SkillWrapper::GetCooldownID(skill_id);
	if(!_cooldown.TestCoolDown(cd_id))
	{
		_runner->error_message(S2C::ERR_CAST_ELF_SKILL_IN_COOLDOWN);
		return;	
	}
	//ʩ��
	SKILL::Data _data(0);
	_data.id = skill_id;
	_data.forceattack = force_attack;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
	if(target_count > 0)
	{
		XID id;
		_target_list.reserve(target_count);
		for(int i=0; i<target_count; i++, targets++)
		{
			MAKE_ID(id, *targets);
			_target_list.push_back(id);
		}
	}
	if( _skill.RunElfSkill(_data, skill_level, object_interface(this), _target_list.begin(), _target_list.size()) != 0)
	{
		_runner->error_message(S2C::ERR_CAST_ELF_SKILL_FAILED);
		return;	
	}
	else	//�ɹ������蹫����ȴ
		SetCoolDown(COOLDOWN_INDEX_CAST_ELF_SKILL, CAST_ELF_SKILL_COOLDOWN_TIME);
	
}

bool gplayer_imp::RechargeEquippedElf(unsigned int index, unsigned int count)	//��ɽ��������߼���ȫ��ͬ
{
	if(count == 0 || index >= _inventory.Size()) return false;
	item & element = _inventory[index];
	if(element.type == -1 || element.count < count) return false;
	item & it = _equipment[item::EQUIP_INDEX_ELF];//��С�����
	if(it.type == -1 || !it.body) return false;
	int element_id = element.type;
	DATA_TYPE dt;
	ELEMENT_ESSENCE * ess = (ELEMENT_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)element_id, ID_SPACE_ESSENCE,dt);
	if( !ess || dt != DT_ELEMENT_ESSENCE) return false;
	int old_stamina = it.GetStamina();
	int cur_stamina;
	int rst = it.body->Recharge(ess->level,count,cur_stamina);
	if(rst > 0)
	{
		UpdateMallConsumptionDestroying(element.type, element.proc_type, rst);

		_inventory.DecAmount(index,rst);
		_runner->player_drop_item(IL_INVENTORY,index,element_id,rst,S2C::DROP_TYPE_RECHARGE);
		_runner->query_elf_stamina(cur_stamina);	//֪ͨ�ͻ���С���������仯
		_runner->elf_cmd_result(S2C::ELF_RECHARGE, element_id, rst, cur_stamina - old_stamina);
	}
	return true;
}

void gplayer_imp::ElfDecAttribute(unsigned int inv_idx_elf, unsigned int inv_idx_ticket, short str, short agi, short vit,  short eng)
{
	//����inv_idx_ticket����
	if(inv_idx_elf >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it1 = _inventory[inv_idx_elf];
	if(it1.type <= 0 || it1.body == NULL || it1.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_DEC_ATTRIBUTE_FAILED);
		return;
	}
	int req_ticket = str + agi + vit + eng;
	int ticket1_count = GetItemCount(ELF_DEC_ATTRIBUTE_TICKET_ID);
	int ticket2_count = GetItemCount(ELF_DEC_ATTRIBUTE_TICKET_ID2);
	if(ticket1_count + ticket2_count < req_ticket)
	{
		_runner->error_message(S2C::ERR_ELF_DEC_ATTRIBUTE_FAILED);
		return;
	}
	if(!it1.DecAttributePoint(str, agi, vit, eng))
	{
		_runner->error_message(S2C::ERR_ELF_DEC_ATTRIBUTE_FAILED);
		return;
	}
	else
	{
		if(ticket1_count >= req_ticket)
			RemoveItems(ELF_DEC_ATTRIBUTE_TICKET_ID ,req_ticket, S2C::DROP_TYPE_USE, true);
		else
		{
			RemoveItems(ELF_DEC_ATTRIBUTE_TICKET_ID ,ticket1_count, S2C::DROP_TYPE_USE, true);
			RemoveItems(ELF_DEC_ATTRIBUTE_TICKET_ID2 ,req_ticket - ticket1_count, S2C::DROP_TYPE_USE, true);	
		}
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);//�ͻ���װ������
		int param1 = agi, param2 = eng;
		param1 <<= 16, param2 <<=16;
		param1 |= str, param2 |= vit;
		_runner->elf_cmd_result(S2C::ELF_DEC_ATTRIBUTE, it1.type, param1, param2);
		return;
	}
}

void gplayer_imp::ElfFlushGenius(unsigned int inv_idx_elf, unsigned int inv_idx_ticket)
{
	if(inv_idx_elf >= _inventory.Size() || inv_idx_ticket >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it1 = _inventory[inv_idx_elf];
	if(it1.type <= 0 || it1.body == NULL || it1.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_FLUSH_GENIUS_FAILED);
		return;
	}
	item & it2 = _inventory[inv_idx_ticket];
	if(it2.type != ELF_FLUSH_GENIUS_TICKET_ID && it2.type != ELF_FLUSH_GENIUS_TICKET_ID2 
		|| it2.count < 1)
	{
		_runner->error_message(S2C::ERR_ELF_FLUSH_GENIUS_FAILED);
		return;
	}
	if(!it1.FlushGeniusPoint())
	{
		_runner->error_message(S2C::ERR_ELF_FLUSH_GENIUS_FAILED);
		return;
	}
	else
	{
		UpdateMallConsumptionDestroying(it2.type, it2.proc_type, 1);

		_runner->player_drop_item(IL_INVENTORY, inv_idx_ticket, it2.type, 1, S2C::DROP_TYPE_USE);
		_inventory.DecAmount(inv_idx_ticket, 1);
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);//�ͻ���װ������
		return;
	}
}

void gplayer_imp::ElfLearnSkill(unsigned int inv_idx_elf, unsigned short skill_id)
{
	if(inv_idx_elf >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it = _inventory[inv_idx_elf];
	if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_LEARN_SKILL_FAILED);
		return;
	}
	int new_level;
	if( (new_level = it.LearnSkill(this, skill_id)) < 0)
	{
		_runner->error_message(S2C::ERR_ELF_LEARN_SKILL_FAILED);
		return;
	}
	else
	{
		//��С���鼼����Ҫ���ļ����飬��LearnSkill()��ɾ����֪ͨ�ͻ���
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);//�ͻ���װ������
		_runner->elf_cmd_result(S2C::ELF_LEARN_SKILL, it.type, skill_id, new_level);
		return;
	}
	
}

void gplayer_imp::ElfForgetSkill(unsigned int inv_idx_elf, unsigned short skill_id, short forget_level)
{
	//��С����
	if(inv_idx_elf >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it = _inventory[inv_idx_elf];
	if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_FORGET_SKILL_FAILED);
		return;
	}
	if(it.ForgetSkill(this, skill_id, forget_level) < 0)
	{
		_runner->error_message(S2C::ERR_ELF_FORGET_SKILL_FAILED);
		return;
	}
	else
	{
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);//�ͻ���װ������
		_runner->elf_cmd_result(S2C::ELF_FORGET_SKILL, it.type, skill_id, forget_level);
		return;
	}
	
}

void gplayer_imp::ElfRefine(unsigned int inv_idx_elf, unsigned int inv_idx_ticket, int ticket_cnt)
{
//���λõ��ɷŶ��������ֻ�ܷ�1������Ŷ࣬��ʵ�����ļ��㣬�����ش���
	if(inv_idx_elf >= _inventory.Size() || inv_idx_ticket >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	//��С����
	item & it = _inventory[inv_idx_elf];
	if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_CANNOT_REFINE);
		return;
	}
	//��ǰ�����ȼ����ܾ���?
	short elf_ref_lvl = it.GetRefineLevel();
	if(elf_ref_lvl < 0 || elf_ref_lvl >= MAX_ELF_REFINE_LEVEL)
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_CANNOT_REFINE);
		return;
	}
	//�Ǿ������ϣ�ȷ��ʹ������
	int ticket_id = -1;
	item & it2 = _inventory[inv_idx_ticket];
	if(it2.type == ELF_REFINE_TICKET0_ID && it2.count >= 1)
		ticket_cnt = 1;
	else if(it2.type == ELF_REFINE_TICKET1_ID && it2.count >= 1)
		ticket_cnt = 1;
	else if(it2.type == ELF_REFINE_TICKET2_ID && it2.count >= 1)
		ticket_cnt = 1;
	else if(it2.type == ELF_REFINE_TICKET3_ID && ticket_cnt > 0)
	{
		if(ticket_cnt > elf_refine_max_use_ticket3[elf_ref_lvl+1])
			ticket_cnt = elf_refine_max_use_ticket3[elf_ref_lvl+1];
		//����Ӧ�ü����������е�����
		if(!CheckItemExist(it2.type, ticket_cnt))
		{
			_runner->error_message(S2C::ERR_ELF_REFINE_CANNOT_REFINE);
			return;
		}
	}
	else
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_CANNOT_REFINE);
		return;
	}
	ticket_id = it2.type;
	
	//����
	int level_original = 0;
	int rst = it.ElfRefine(ticket_id, ticket_cnt, level_original);
	if(rst != item::REFINE_CAN_NOT_REFINE)
	{
		const char * tbuf[] = {"�ɹ�", "�޷�����" , "������ʧ", "���Խ���һ��", "���Ա���", "װ������","δ֪1","δ֪2","δ֪3"};
		GLog::log(GLOG_INFO,"�û�%d����С����(%d)[%s]������ǰ����%d ����С���龫������(%d)%d��",_parent->ID.id, it.type, tbuf[rst],level_original, ticket_id, ticket_cnt);
		if(level_original >= 6)
		{
			GLog::refine(_parent->ID.id,it.type, level_original, rst, ticket_cnt);
		}
	}
	switch(rst)
	{
		case item::REFINE_CAN_NOT_REFINE:
		//�޷����о��������;���ʧ��  ������������κα仯 
		_runner->error_message(S2C::ERR_ELF_REFINE_CANNOT_REFINE);
		return;
		break;

		case item::REFINE_SUCCESS:
		//�����ɹ�
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);
		RemoveItems(ticket_id, ticket_cnt, S2C::DROP_TYPE_USE, true);
		_runner->elf_cmd_result(S2C::ELF_REFINE, it.type, 0, level_original);
		break;

		case item::REFINE_FAILED_LEVEL_0:
		//����һ��ʧ�ܣ�ɾ������
		RemoveItems(ticket_id, ticket_cnt, S2C::DROP_TYPE_USE, true);
		_runner->elf_cmd_result(S2C::ELF_REFINE, it.type, 1, level_original);
		break;

		case item::REFINE_FAILED_LEVEL_1:
		//��������ʧ�ܣ�ɾ�����ϣ�����һ�� �����Ѿ����Զ�����
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);
		RemoveItems(ticket_id, ticket_cnt, S2C::DROP_TYPE_USE, true);
		_runner->elf_cmd_result(S2C::ELF_REFINE, it.type, 2, level_original);
		break;

		case item::REFINE_FAILED_LEVEL_2:
		//��������ʧ�ܣ�ɾ�����ϣ������Ѿ����Զ����
		PlayerGetItemInfo(IL_INVENTORY,inv_idx_elf);
		RemoveItems(ticket_id, ticket_cnt, S2C::DROP_TYPE_USE, true);
		_runner->elf_cmd_result(S2C::ELF_REFINE, it.type, 3,level_original);
		break;
		
		default:
		GLog::log(GLOG_ERR,"����װ��ʱ�������쳣����%d",rst);
	}

	return;

}

void gplayer_imp::ElfRefineTransmit(unsigned int inv_idx_src, unsigned int inv_idx_dst)
{
	//�Ƿ���С���飿
	if(inv_idx_src == inv_idx_dst || inv_idx_src >= _inventory.Size() || inv_idx_dst >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & src_item = _inventory[inv_idx_src];
	item & dst_item = _inventory[inv_idx_dst];
	if(src_item.type <= 0 || src_item.body == NULL || src_item.body->GetItemType() != item_body::ITEM_TYPE_ELF
		|| dst_item.type <= 0 || dst_item.body == NULL || dst_item.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_TRANSMIT_FAILED);
		return;
	}

	//���С����İ�ȫ״̬����
	bool src_tradable = (src_item.GetSecureStatus() == elf_item::STATUS_TRADABLE);
	bool dst_tradable = (dst_item.GetSecureStatus() == elf_item::STATUS_TRADABLE);
	if(!src_tradable && dst_tradable)
	{
		_runner->error_message(S2C::ERR_ELF_REF_TRANS_FROM_TRADE_TO_UNTRADE);
		return;
	}

	//�������װ���ľ����ȼ���ƥ���
	short src_level = src_item.GetRefineLevel();
	short dst_level = dst_item.GetRefineLevel();
	if(src_level < 0 || dst_level < 0 || src_level > MAX_ELF_REFINE_LEVEL || dst_level > MAX_ELF_REFINE_LEVEL 
		||src_level == 0 || dst_level >= src_level) 
	{
		_runner->error_message(S2C::ERR_ELF_REFINE_TRANSMIT_FAILED);
		return;
	}
	
	//���Ǭ��ʯ�������Ƿ��㹻
	if(!CheckItemExist(ELF_REFINE_TRANSMIT_TICKET_ID, elf_refine_transmit_cost[src_level]))
	{
		_runner->error_message(S2C::ERR_TRANSMIT_REFINE_NO_MATERIAL);
		return;
	}
	//����˫�����¾����ȼ�
	short n_src_level = src_item.SetRefineLevel(0);
	short n_dst_level = dst_item.SetRefineLevel(src_level);
	
	//����־
	GLog::log(GLOG_INFO,"���%dִ���˾�����ת��������(%d[%d->%d])��(%d[%d->%d])",_parent->ID.id, 
			src_item.type, src_level, n_src_level, 
			dst_item.type,dst_level,n_dst_level);

	//�۳�Ǭ��ʯ
	RemoveItems(ELF_REFINE_TRANSMIT_TICKET_ID, elf_refine_transmit_cost[src_level], S2C::DROP_TYPE_USE, true);
	
	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,inv_idx_src);
	PlayerGetItemInfo(IL_INVENTORY,inv_idx_dst);
	
}

void gplayer_imp::ElfDecompose(unsigned int inv_idx_elf)
{
	//���С�����Ƿ����
	if(inv_idx_elf >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it = _inventory[inv_idx_elf];
	if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_DECOMPOSE_FAILED);
		return;
	}
	//���С�����Ƿ�ɽ���
	if(it.GetLevel() > 40 && it.GetSecureStatus() != elf_item::STATUS_TRADABLE)
	{
		_runner->error_message(S2C::ERR_DECOMPOSE_UNTRADABLE_ELF);
		return;
	}
	//��ȡ�ɷֽ⾭��
	unsigned int exp = 0;
	int exp_level = 0;
	if(!it.GetDecomposeElfExp(exp, exp_level) || exp <= 0 || exp_level <= 0)
	{
		_runner->error_message(S2C::ERR_ELF_DECOMPOSE_EXP_ZERO);
		return;
	}
	//���ɾ�����item_data
	element_data::item_tag_t tag = {element_data::IMT_NULL,0};
	item_data * data = world_manager::GetDataMan().generate_item_from_player(ELF_EXPPILL_ID, &tag, sizeof(tag));	
	if(data == NULL)
	{
		_runner->error_message(S2C::ERR_ELF_DECOMPOSE_FAILED);
		return;
	}
	*(unsigned int *)(data->item_content) = exp;
	*((int *)(data->item_content)+1) = exp_level;
	GLog::log(GLOG_INFO,"�û�%d�ֽ�С����%d�õ�С���龭����%dexp=%uexp_level=%d",_parent->ID.id, it.type, data->type, exp, exp_level);
	//����С����
	_runner->player_drop_item(IL_INVENTORY, inv_idx_elf, it.type, 1, S2C::DROP_TYPE_DECOMPOSE);
	//int elf_id = it.type;		//����һ��С����id
	_inventory.DecAmount(inv_idx_elf, 1);
	//��������������
	int rst = _inventory.PushInEmpty(0, *data, 1);
	if(rst >= 0)
	{
		_runner->obtain_item(data->type, data->expire_date, 1, _inventory[rst].count, IL_INVENTORY, rst);
		__PRINTF("��Ʒid:%d  ����������%d slot����%d λ��%d\n",data->type,1,_inventory[rst].count,rst);
		_runner->elf_cmd_result(S2C::ELF_DECOMPOSE, ELF_EXPPILL_ID, exp_level, exp);
	}
	else
	{
		ASSERT(false && "����������û�пռ�");
		FreeItem(data);
		return;
	}
	
	FreeItem(data);
	return;
}
void gplayer_imp::ElfDestroyItem(unsigned int inv_idx_elf, int mask, unsigned int inv_idx_equip)
{
	//���С�����Ƿ����
	if(inv_idx_elf >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return;
	}
	item & it = _inventory[inv_idx_elf];
	if(it.type <= 0 || it.body == NULL || it.body->GetItemType() != item_body::ITEM_TYPE_ELF)
	{
		_runner->error_message(S2C::ERR_ELF_DESTROY_ITEM_FAILED);
		return;
	}
	//���mask
	if(mask != 0x01 && mask != 0x02 && mask != 0x04 && mask != 0x08)
	{
		_runner->error_message(S2C::ERR_ELF_DESTROY_ITEM_FAILED);
		return;
	}
	//���inv_idx_equip
	int equip_type = -1;
	if(inv_idx_equip != 255)
	{
		if(inv_idx_equip >= _inventory.Size())
		{
			_runner->error_message(S2C::ERR_FATAL_ERR);
			return;
		}
		item & it2 = _inventory[inv_idx_equip];
		if(it2.type <= 0 || it2.body == NULL || it2.body->GetItemType() != item_body::ITEM_TYPE_ELF_EQUIP)
		{
			_runner->error_message(S2C::ERR_ELF_DESTROY_ITEM_FAILED);
			return;
		}
		equip_type = it2.type;
	}
	//���٣��ͻ��˸���
	int old_equip_type = it.DestroyElfItem(mask,equip_type);
	if(old_equip_type > 0)
	{
		if(equip_type != -1){
			item & it = _inventory[inv_idx_equip];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);

			_runner->player_drop_item(IL_INVENTORY, inv_idx_equip, equip_type, 1, S2C::DROP_TYPE_TAKEOUT);
			_inventory.DecAmount(inv_idx_equip, 1);
		}
		PlayerGetItemInfo(IL_INVENTORY, inv_idx_elf);
		_runner->elf_cmd_result(S2C::ELF_DESTROY_ITEM, it.type, old_equip_type, equip_type);
	}
	else
		_runner->error_message(S2C::ERR_ELF_DESTROY_ITEM_FAILED);

	return;
}

void gplayer_imp::UpdateStallInfo(int id, int max_sell, int max_buy, int max_name) //�ڰ�̯ƾ֤װ��ʱ����	
{
	//ASSERT(_stall_info.stallcard_id == -1);
	_stall_info.stallcard_id = id;
	_stall_info.max_sell_slot = (max_sell>PLAYER_MARKET_MAX_SELL_SLOT ? PLAYER_MARKET_MAX_SELL_SLOT : max_sell);	
	_stall_info.max_buy_slot = (max_buy>PLAYER_MARKET_MAX_BUY_SLOT ? PLAYER_MARKET_MAX_BUY_SLOT : max_buy);	
	_stall_info.max_name_length = (max_name>PLAYER_MARKET_MAX_NAME_LEN ? PLAYER_MARKET_MAX_NAME_LEN : max_name);	
}

void gplayer_imp::ClearStallInfo()	//�ڰ�̯ƾ֤ж��ʱ����
{
	//ASSERT(_stall_info.stallcard_id != -1);
	_stall_info.stallcard_id = -1;
	_stall_info.max_sell_slot = PLAYER_MARKET_SELL_SLOT;	
	_stall_info.max_buy_slot = PLAYER_MARKET_BUY_SLOT;	
	_stall_info.max_name_length = PLAYER_MARKET_NAME_LEN;	
}

void gplayer_imp::OnStallCardTakeOut()
{
	if(_player_state == PLAYER_STATE_MARKET)
	{
		ASSERT(_stall_obj != NULL);
		if((int)_stall_obj->GetSellSlot() > _stall_info.max_sell_slot
			|| (int)_stall_obj->GetBuySlot() > _stall_info.max_buy_slot
			|| (int)_stall_obj->GetNameLen() > _stall_info.max_name_length)
			CancelPersonalMarket();
	}
}

//obj_interface�ӿ�	
bool gplayer_imp::OI_GetElfProp(short& level, short& str, short& agi, short& vit, short& eng)
{
	if(_cur_elf_info.id == (unsigned int)-1)
		return false;
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	level = it.GetLevel() & 0xFF;
	if(level <=0)	return false;
	str = _cur_elf_info.final_strength;
	agi = _cur_elf_info.final_agility;
	vit = _cur_elf_info.final_vitality;
	eng = _cur_elf_info.final_energy;
	
	return true;
}
bool gplayer_imp::OI_DrainElfVigor(int dec)
{
	if(_cur_elf_info.id == (unsigned int)-1)
		return false;
	if((int)_cur_elf_info.vigor < dec)
		dec = (int)_cur_elf_info.vigor;
	_cur_elf_info.vigor -= dec;
	_cur_elf_info.refresh_vigor = true;
	return true;
}
bool gplayer_imp::OI_DrainElfStamina(int dec)
{
	if(_cur_elf_info.id == (unsigned int)-1)
		return false;
	item & it = _equipment[item::EQUIP_INDEX_ELF];
	int cur_stamina;
	if( (cur_stamina = it.GetStamina()) < dec)
		dec = cur_stamina;
	it.DecStamina(dec);
	_runner->query_elf_stamina(cur_stamina - dec);
	return true;
}
//debug only
void gplayer_imp::dump_elf_info()
{
	printf("---------------------------------------------------------------\n");
	printf("player uid: --%d-- _cur_elf_info:\n", _parent->ID.id);
	printf("id=%d refresh_vigor=%d refresh_enhance=%d refine_effect_active=%d refine_level=%d strength=%d agility=%d vitality=%d energy=%d final_strength=%d final_agility=%d final_vitality=%d final_energy=%d vigor=%f max_vigor=%f  vigor_gen=%f _min_elf_status_value=%d\n",
	_cur_elf_info.id ,_cur_elf_info.refresh_vigor ,_cur_elf_info.refresh_enhance ,_cur_elf_info.refine_effect_active ,_cur_elf_info.refine_level ,_cur_elf_info.strength ,_cur_elf_info.agility ,_cur_elf_info.vitality ,_cur_elf_info.energy ,_cur_elf_info.final_strength ,_cur_elf_info.final_agility ,_cur_elf_info.final_vitality ,_cur_elf_info.final_energy ,_cur_elf_info.vigor ,_cur_elf_info.max_vigor  ,_cur_elf_info.vigor_gen, _min_elf_status_value);
	printf("active skill:\n");
	for(unsigned int i=0; i<_cur_elf_info.skillvect.size(); i++)
		printf("skill[%d] id=%d level=%d", i, _cur_elf_info.skillvect[i].id, _cur_elf_info.skillvect[i].level);
	printf("\n");
	if(_cur_elf_info.id != (unsigned int)-1)
		((class elf_item*)_equipment[item::EQUIP_INDEX_ELF].body)->dump_all();
	printf("---------------------------------------------------------------\n");
}

void gplayer_imp::get_mall_detail()
{
	netgame::mall & __mall = world_manager::GetPlayerMall();	
	//���ܷ����仯����Ʒ�б�	
	abase::vector<netgame::mall::index_node_t, abase::fast_alloc<> > & __limit_goods = __mall.GetLimitGoods();
	unsigned int __limit_goods_count = __limit_goods.size();
	
	char buf[120]={0};
	int len;
	for(unsigned int i=0; i<__limit_goods_count; i++)
	{
		int index = __limit_goods[i]._index;
		netgame::mall::node_t node = __limit_goods[i]._node;
		Say("----------\n");
		memset(buf, 0, sizeof(buf));
		sprintf(buf,"index%d item%d count%d:\n",index,node.goods_id,node.goods_count);
		Say(buf);
		for(int j=0; j<4; j++)
		{
			int price = node.entry[j].cash_need;
			if(price <= 0)
				break;
			memset(buf, 0, sizeof(buf));
			sprintf(buf, "(%d) price:%d.%02d ", j, price/100, price%100);
			Say(buf);
			int type = node.entry[j]._sale_time.GetType();
			int param1 = node.entry[j]._sale_time.GetParam1();
			int param2 = node.entry[j]._sale_time.GetParam2();
			int f;
			switch(type)
			{
				case netgame::mall::sale_time::TYPE_NOLIMIT:
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "saletime:NOLIMIT\n");
					Say(buf);
					break;
				case netgame::mall::sale_time::TYPE_INTERZONE:
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "saletime:INTERZONE: ");
					len = strlen(buf);
					if(param1)
						ctime_r((time_t*)&param1, buf+len);
					else
						buf[len]='0';
					len = strlen(buf);
					buf[len] = '-';
					buf[len+1] = '-';
					len = strlen(buf);
					if(param2)
						ctime_r((time_t*)&param2, buf+len);
					else
						buf[len]='0';
					Say(buf);
					break;
				break;
				case netgame::mall::sale_time::TYPE_WEEK:
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "saletime:WEEK: ");
					f = 1;
					for(int m=0; m<7; m++, f<<=1)
					{
						if(param1 & f)
						{
							len = strlen(buf);
							sprintf(buf+len, "%d ",m);
						}
					}
					len = strlen(buf);
					sprintf(buf+len, "\n");
					Say(buf);
					break;
				break;
				case netgame::mall::sale_time::TYPE_MONTH:
					memset(buf, 0, sizeof(buf));
					sprintf(buf, "saletime:MONTH: ");
					f = 2;
					for(int m=1; m<32; m++, f<<=1)
					{
						if(param1 & f)
						{
							len = strlen(buf);
							sprintf(buf+len, "%d ",m);
						}
					}
					len = strlen(buf);
					sprintf(buf+len, "\n");
					Say(buf);
					break;
				break;
				default:
				break;
			}
		}
	}
	
}
void gplayer_imp::change_elf_property(int index, int value)
{
	if(_cur_elf_info.id != (unsigned int)-1)
	{
		_equipment[item::EQUIP_INDEX_ELF].Deactivate(item::BODY, item::EQUIP_INDEX_ELF, this);
		((class elf_item*)_equipment[item::EQUIP_INDEX_ELF].body)->change_elf_property(index, value, this);
		_equipment[item::EQUIP_INDEX_ELF].Activate(item::BODY, _equipment, item::EQUIP_INDEX_ELF, this);
		PlayerGetItemInfo(IL_EQUIPMENT, item::EQUIP_INDEX_ELF);		
		
		
		X_EQUIP id1;
		id1.type = _equipment[item::EQUIP_INDEX_ELF].type; id1.mask = _equipment[item::EQUIP_INDEX_ELF].GetIdModify();
		CalcEquipmentInfo();
		_runner->equipment_info_changed(1ULL<<item::EQUIP_INDEX_ELF , 0, &id1, sizeof(X_EQUIP));//�˺���ʹ����CalcEquipmentInfo�Ľ��
	}
}

void gplayer_imp::change_factionfortress(int index, int value)
{
	if(!OI_IsMafiaMember()) return;
	
	world_data_ctrl * pCtrl = _plane->w_ctrl;
	if(!pCtrl) return;
	if(pCtrl->GetFactionId() != OI_GetMafiaID()) return;
	
	faction_world_ctrl * ctrl = (faction_world_ctrl *)pCtrl;
	switch(index)
	{
		case 1:
			ctrl->exp += value;
			ctrl->exp_today += value;
			break;
		case 2:
			for(unsigned int i=0; i<faction_world_ctrl::MATERIAL_COUNT; i++)
				ctrl->material[i] += value;
			break;
		default:
			break;
	}
	
	PlayerGetFactionFortressInfo();
}

void
gplayer_imp::GetAUMailTask(int level,char ex_reward)
{
	PlayerTaskInterface  task_if(this);
	OnTaskExternEvent(&task_if,level);
	if(ex_reward)
	{
		OnTaskExternEvent(&task_if,EX_TK_SENDAUMAIL_EXAWARD);
	}
}

void
gplayer_imp::PlayerProduce4Choose(bool remain)
{
	if (!_cur_session || _cur_session->GetRunTimeClass()->GetGUID() != CLS_SESSION_PRODUCE4 || _cur_session->_session_id < 0)
	{
		return;
	}
	((session_produce4*)_cur_session)->ChooseItem(remain);
}

bool
gplayer_imp::Produce4ChooseExec(const recipe_template & rt, int equip_id, int equip_inv_idx, char inherit_type, void **pItem, unsigned short crc, int eq_refine_level, int eq_socket_count, int eq_stone_type[], addon_data eq_engrave_addon_list[3], unsigned int eq_engrave_addon_count)
{
	//�ȼ��ɵ�װ�����ڲ��ڣ�Ҫ�ǲ������Ǿͻ�ɾ��ʧ�ܣ���������������Ʒ
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) 
	{
		return false;
	}
	item & eq_it = _inventory[equip_inv_idx];
	if(eq_it.body == NULL) return false;
	if (eq_it.GetCRC() != crc) // ��Ȼλ�ú�idһ�������Ǳ���һ�������һ��װ����
	{
		return false;
	}
	//ɾ����װ��
	item& it = _inventory[equip_inv_idx];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);

	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,equip_inv_idx,equip_id,1,S2C::DROP_TYPE_PRODUCE);
	_inventory.DecAmount(equip_inv_idx, 1);

	//�������������Ʒ
	item_data *data = (item_data *)(*pItem);
	int rst = _inventory.Push(*data);
	if(rst >=0)
	{
	    FirstAcquireItem(data);

		item & target_eq_it = _inventory[rst];
		if(inherit_type & PRODUCE_INHERIT_REFINE)
		{
			//�̳д�����װ���ľ���
			if(eq_refine_level > 0)
			{
				int material_need;
				int refine_addon = world_manager::GetDataMan().get_item_refine_addon(target_eq_it.type, material_need);
				if(material_need > 0 && refine_addon > 0)
					target_eq_it.body->SetRefineLevel(refine_addon,eq_refine_level);		
			}
		}
		if(inherit_type & PRODUCE_INHERIT_SOCKET)
		{
			//�̳д�����װ���Ŀ���
			//�̳д�����װ���ı�ʯ
			if(eq_socket_count > 0)
			{
				target_eq_it.body->SetSocketAndStone(eq_socket_count,eq_stone_type);	
			}
		}
		if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
		{
			//�̳д�����װ�����Կ�����
			if(eq_engrave_addon_count > 0)
			{
				target_eq_it.Engrave(&eq_engrave_addon_list[0], eq_engrave_addon_count);
			}
		
		}
		
		//�������������Ʒ����Ϣ
		_runner->produce_once(target_eq_it.type,rt.count - data->count,_inventory[rst].count, 0,rst);
	}

	if(data->count)
	{
		//ʣ������Ʒ
		DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
		//�����������Ҫ�ͷ��ڴ�
		*pItem = NULL;
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		//���������ж�,�����ʵҲ�������ɹ���
		return true;
	}
	return true;
}

void gplayer_imp::PlayerRenameRet(const void *new_name, unsigned int name_len, int ret)
{
	__PRINTF("PlayerRenameRet namelen=%d ret=%d\n",name_len,ret);
	if (ret == 0)
	{
		//�޸��ڴ�����������
		SetPlayerName(new_name, name_len);
	}
}

// ����ʱ�����������ֵ
void gplayer_imp::UpdateMallConsumptionShopping(int id, unsigned int proc_type, int count, int total_price)
{
	if (world_manager::IsMallConsumptionItemShopping(id))
	{
		int tmp = _mall_consumption + total_price;
		if (tmp < _mall_consumption)
		{
			return;
		}
		_mall_consumption = tmp;
		
		char name_base64[MAX_USERNAME_LENGTH*2] = {0};
		base64_encode((unsigned char *)_username,(int)_username_len,name_base64);
		GLog::formatlog("mallconsumption:userid=%d:roleid=%d:rolename=%s:delta=%d:total=%d:type=%d:item=%d:item_count=%d",
				_db_user_id,_parent->ID.id,name_base64,total_price,_mall_consumption,0,id,count);
	}
}

// ���˺�һʱ�����������ֵ
void gplayer_imp::UpdateMallConsumptionBinding(int id, unsigned int proc_type, int count)
{
	if (proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
	{
		return;
	}

	int value;
	if (world_manager::GetMallConsumptionValueBinding(id, value))
	{
		float tmp1 = (float)value * (float)count;
		if (tmp1 > 2e9 || tmp1 < 0.f)
		{
			return;
		}

		int tmp2 = _mall_consumption + (value * count);
		if (tmp2 < _mall_consumption)
		{
			return;
		}
		_mall_consumption = tmp2;
		
		char name_base64[MAX_USERNAME_LENGTH*2] = {0};
		base64_encode((unsigned char *)_username,(int)_username_len,name_base64);
		GLog::formatlog("mallconsumption:userid=%d:roleid=%d:rolename=%s:delta=%d:total=%d:type=%d:item=%d:item_count=%d",
				_db_user_id,_parent->ID.id,name_base64,value*count,_mall_consumption,1,id,count);
	}
}

// ���Ļ�۳�ʱ�����������ֵ
void gplayer_imp::UpdateMallConsumptionDestroying(int id, unsigned int proc_type, int count)
{
	if (!(proc_type & item::ITEM_PROC_TYPE_MALL))
	{
		return;
	}

	int value;
	if (world_manager::GetMallConsumptionValueDestroying(id, value))
	{
		float tmp1 = (float)value * (float)count;
		if (tmp1 > 2e9 || tmp1 < 0.f)
		{
			return;
		}

		int tmp2 = _mall_consumption + (value * count);
		if (tmp2 < _mall_consumption)
		{
			return;
		}
		_mall_consumption = tmp2;
		
		char name_base64[MAX_USERNAME_LENGTH*2] = {0};
		base64_encode((unsigned char *)_username,(int)_username_len,name_base64);
		GLog::formatlog("mallconsumption:userid=%d:roleid=%d:rolename=%s:delta=%d:total=%d:type=%d:item=%d:item_count=%d",
				_db_user_id,_parent->ID.id,name_base64,value*count,_mall_consumption,2,id,count);
	}
}

int
gplayer_imp::PlayerEvolutionPet(unsigned int index,int formula_index)
{
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return S2C::ERR_PET_IS_NOT_EXIST;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;
	if(_petman.GetCurActivePet() == (int)index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;
	if(pData->level < 100) return S2C::ERR_PET_CANNOT_EVOLUTION;
	//�����Ϲ�����
	if(formula_index > 2 || formula_index < 0)
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	DATA_TYPE dt;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pData->pet_tid, ID_SPACE_ESSENCE, dt);
	if(dt != DT_PET_ESSENCE || petess == NULL) return S2C::ERR_PET_CANNOT_EVOLUTION;
	
	PET_EVOLVE_CONFIG * pec = (PET_EVOLVE_CONFIG *)world_manager::GetDataMan().get_data_ptr(PET_EVOLVE_CONFIG_ID,ID_SPACE_CONFIG,dt);	
	if(dt != DT_PET_EVOLVE_CONFIG || pec == NULL) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	int cost_index = petess->cost_index;
	int cost1 = pec->cost[cost_index].num_evolve[formula_index][0];
	int cost2 = pec->cost[cost_index].num_evolve[formula_index][1];
	
	if((cost1 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM1,cost1)) ||(cost2 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM2,cost2)))
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	//ȡ��������ĳ���id
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(petess->id_pet_egg_evolved,ID_SPACE_ESSENCE,dt);
	if(ess == NULL || dt != DT_PET_EGG_ESSENCE) return S2C::ERR_PET_CANNOT_EVOLUTION;

	int evolution_id = ess->id_pet; 
	
	//ȡ������������Ը��
	PET_ESSENCE * petess2 = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(evolution_id, ID_SPACE_ESSENCE, dt);
	if(dt != DT_PET_ESSENCE || petess2 == NULL) return S2C::ERR_PET_CANNOT_EVOLUTION;
	
	PET_EVOLVED_SKILL_RAND_CONFIG *pskill = (PET_EVOLVED_SKILL_RAND_CONFIG *)world_manager::GetDataMan().get_data_ptr(petess2->id_evolved_skill_rand,ID_SPACE_CONFIG,dt);
	if(dt != DT_PET_EVOLVED_SKILL_RAND_CONFIG || pskill == NULL) return S2C::ERR_PET_CANNOT_EVOLUTION;
	//�˴�ֱ��������Ը�
	int nature_group = abase::RandSelect(&(pskill->rand_skill_group[0].probability),sizeof(pskill->rand_skill_group[0]),3);
	int count = 0;
	unsigned int *pList = pskill->rand_skill_group[nature_group].list;
	for(int i=0;i<30;i++)
	{
		if(*(pList+i) == 0)
			continue;
		count ++;
	}
	int offset = abase::Rand(0,count-1);
	int pet_nature = pskill->rand_skill_group[nature_group].list[offset];

	//��ȡ����
	PET_EVOLVED_SKILL_CONFIG *pesc = (PET_EVOLVED_SKILL_CONFIG *)world_manager::GetDataMan().get_data_ptr(pet_nature,ID_SPACE_CONFIG,dt);
	if(dt != DT_PET_EVOLVED_SKILL_CONFIG || pesc == NULL) return S2C::ERR_PET_CANNOT_EVOLUTION;
	int skill1 = pesc->skills[0].id;
	int level1 = pesc->skills[0].level;
	int skill2 = pesc->skills[1].id;
	int level2 = pesc->skills[1].level;
	
	//����
	if(!_petman.EvolutionPet(this,index,evolution_id,pet_nature,skill1,level1,skill2,level2))
	{
		return S2C::ERR_PET_CANNOT_EVOLUTION;
	}
	//������Ʒ
	RemoveItems(PET_EVOLUTION_ITEM1,cost1, S2C::DROP_TYPE_USE, true);
	RemoveItems(PET_EVOLUTION_ITEM2,cost2, S2C::DROP_TYPE_USE, true);
	_runner->pet_evolution_done(index);
	return 0;
}

/*166*/
bool
gplayer_imp::PlayerSkillDataInc(int index_from, int index_to)
{
	if(index_from < 0 || index_to > pet_data::MAX_PET_SKILL_COUNT)
	{
		return false;
	}

	if(!_petman.SkillDataInc(this,index_from,index_to))
	{
		return false;
	}
	
	return true;
}

bool
gplayer_imp::PlayerPetUseSkinInc(int index, int pos_data, int mode)
{
	if(index < 0 || index > pet_manager::MAX_PET_CAPACITY)
	{
		return false;
	}

	pet_data * pData = GetPetData(index);
	if(pData == NULL) return false;

	switch (mode)
	{
	case 1:
		if(!_petman.PetUseSkinInc(this,index,pos_data)) return false;
		break;
	case 0:
		if(!_petman.PetBuySkinInc(this,index,pos_data)) return false;
		break;
	case 10:
		if(!_petman.RebuildStorage(this)) return false;
		break;

	
	default:
		break;
	}
			
	return true;
}

void 
gplayer_imp::PetEnterWorldSkinStorage()
{
	_petman.RebuildStorage(this);
}

bool 
gplayer_imp::CalcPetEnhance2(const pet_data *pData, extend_prop &prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag)
{
	prop.max_hp += (int)(((_base_prop.max_hp + player_template::GetVitHP(GetObjectClass(),_en_point.vit) + _en_point.max_hp) - 2400) * 0.01f * pData->evo_prop.r_hp);
	prop.damage_low += (int)((_cur_item.damage_magic_low + _en_point.magic_dmg_low + _base_prop.damage_magic_low) * 0.01f * pData->evo_prop.r_attack);
	prop.damage_high += (int)((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * pData->evo_prop.r_attack);
	prop.defense += (int)((_base_prop.defense + _en_point.defense) * 0.01f * pData->evo_prop.r_defense);
	for(unsigned int i = 0; i < MAGIC_CLASS; i ++)
		prop.resistance[i] += (int)((_base_prop.resistance[i] + _en_point.resistance[i]) * 0.01f * pData->evo_prop.r_defense);
	attack_degree += (int)(_attack_degree * 0.01f * pData->evo_prop.r_atk_lvl);
	defend_degree += (int)(_defend_degree * 0.01f * pData->evo_prop.r_def_lvl);
	vigour = GetVigour();
	pen_fis += (int)(_anti_defense_degree * 0.01f * pData->evo_prop.r_pen_lvl_f);
	pen_mag += (int)(_anti_resistance_degree * 0.01f * pData->evo_prop.r_pen_lvl_m);

	return true;	
}

bool 
gplayer_imp::CalcPetEnhance3(const pet_data *pData, int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag)
{
	const pet_data_temp * pTemp = pet_dataman::Get(pData->pet_tid);

	pet_enhance enhance;
	enhance.hp_percent 				= _pet_enhance.hp_percent + 5*skill_level;
	enhance.mp_percent 				= _pet_enhance.mp_percent + 5*skill_level;
	enhance.damage_percent 			= _pet_enhance.damage_percent + 5*skill_level;
	enhance.magic_damage_percent 	= _pet_enhance.magic_damage_percent + 5*skill_level;
	enhance.defense_percent 		= _pet_enhance.defense_percent + 5*skill_level;
	enhance.magic_defense_percent 	= _pet_enhance.magic_defense_percent + 5*skill_level;
	enhance.attack_degree_percent 	= _pet_enhance.attack_degree_percent + 5*skill_level;
	enhance.defend_degree_percent 	= _pet_enhance.defend_degree_percent + 5*skill_level;
	int max_hp1 = (int)((_base_prop.max_hp + player_template::GetVitHP(GetObjectClass(),_en_point.vit) + _en_point.max_hp) * 0.01f * enhance.hp_percent);
	prop.max_hp += (int)(max_hp1 * pTemp->mult_r_hp);
	prop.max_mp += (int)((_base_prop.max_mp + player_template::GetEngMP(GetObjectClass(),_en_point.eng) + _en_point.max_mp) * 0.01f * enhance.mp_percent);
	int en_damage = (int)(((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * enhance.damage_percent)* pTemp->mult_r_attack);
	prop.damage_low += en_damage; 
	prop.damage_high += en_damage;
	int en_magic_damage = (int)(((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * enhance.magic_damage_percent)* pTemp->mult_r_attack);
	prop.damage_magic_low += en_magic_damage;
	prop.damage_magic_high += en_magic_damage;
	int defense1 = (int)((_base_prop.defense + _en_point.defense) * 0.01f * enhance.defense_percent);
	prop.defense += (int)(defense1 * pTemp->mult_r_defense);
	for(unsigned int i = 0; i < MAGIC_CLASS; i ++)
		prop.resistance[i] += (int)(((_base_prop.resistance[i] + _en_point.resistance[i]) * 0.01f * enhance.magic_defense_percent) * pTemp->mult_r_defense);
	attack_degree += (int)(_attack_degree);
	defend_degree += (int)(_defend_degree);
	vigour = GetVigour();
	pen_fis += (int)(_anti_defense_degree);
	pen_mag += (int)(_anti_resistance_degree);
	return true;	
}

bool 
gplayer_imp::CalcPetEnhance4(const pet_data *pData, extend_prop &prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag)
{
	const pet_data_temp * pTemp = pet_dataman::Get(pData->pet_tid);

	int max_hp1 = (int)(((_base_prop.max_hp + player_template::GetVitHP(GetObjectClass(),_en_point.vit) + _en_point.max_hp) - 2400) * 0.01f * pData->evo_prop.r_hp);
	prop.max_hp += (int)(max_hp1 * pTemp->mult_r_hp);
	prop.damage_low += (int)(((_cur_item.damage_magic_low + _en_point.magic_dmg_low + _base_prop.damage_magic_low) * 0.01f * pData->evo_prop.r_attack) * pTemp->mult_r_attack);
	prop.damage_high += (int)(((_cur_item.damage_magic_high + _en_point.magic_dmg_high + _base_prop.damage_magic_high) * 0.01f * pData->evo_prop.r_attack) * pTemp->mult_r_attack);
	prop.defense += (int)(((_base_prop.defense + _en_point.defense) * 0.01f * pData->evo_prop.r_defense) * pTemp->mult_r_defense);
	for(unsigned int i = 0; i < MAGIC_CLASS; i ++)
		prop.resistance[i] += (int)(((_base_prop.resistance[i] + _en_point.resistance[i]) * 0.01f * pData->evo_prop.r_defense) * pTemp->mult_r_defense);
	attack_degree += (int)(_attack_degree);
	defend_degree += (int)(_defend_degree);
	vigour = GetVigour();
	pen_fis += (int)(_anti_defense_degree);
	pen_mag += (int)(_anti_resistance_degree);

	return true;	
}


void gplayer_imp::PlayerPetLevelUp()
{
	int pindex = _petman.GetCurActivePet();
	if(pindex < 0) return;
	
	pet_data * pData = OI_GetPetData(pindex);
	if(!pData) return;
	
	int exp = player_template::GetPetLvlupExp(pData->level);
	
	// Novas rates 
	exp *= EmulateSettings::GetInstance()->GetRatesConfig()->pet_exp;

	_petman.RecvExp(this,pindex,exp);
	return;
}





int 
gplayer_imp::PlayerAddPetExp(unsigned int index,unsigned int num)
{
	//�������Ƿ�����Լ��Ƿ��ܱ�ι��
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return S2C::ERR_PET_IS_NOT_EXIST;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;
	if((pData->pet_class != pet_data::PET_CLASS_COMBAT && pData->pet_class != pet_data::PET_CLASS_EVOLUTION) || num == 0 || _petman.GetCurActivePet() != (int)index )
	{
		return S2C::ERR_PET_TYPE_WRONG;
	}
	//�����Ʒ�Ƿ��㹻
	if(!_inventory.IsItemExist(PET_ADDEXP_ITEM,num))
	{
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	}
	//ι��
	int exp_list[5] = {500,1250,3125,7812,19531};
	int cost_num = 0;
	for(unsigned int i=0;i < num; i++)
	{
		short pet_level = pData->level;
		int exp_gain = 0;
		if(pet_level <=100)
		{
			exp_gain = exp_list[0];
		}
		else if(pet_level <=104)
		{
			exp_gain = exp_list[pet_level - 100];
		}
		else
		{
			exp_gain = exp_list[4];
		}

		// Novas rates 
		exp_gain *= EmulateSettings::GetInstance()->GetRatesConfig()->pet_exp;

		if(_petman.AddExp(this,index,exp_gain))
		{
			cost_num ++;
		}
		else
		{
			break;
		}
	}
	//�۳����p
	if(cost_num > 0)
	{	
		RemoveItems(PET_ADDEXP_ITEM,cost_num,S2C::DROP_TYPE_USE,true);
	}
	return 0;
}

int
gplayer_imp::PlayerRebuildPetNature(unsigned int index,int formula_index)
{
	//�������Ƿ�Ϸ�
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return S2C::ERR_PET_IS_NOT_EXIST;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;
	if(_petman.GetCurActivePet() == (int)index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION)
	{
		return S2C::ERR_PET_TYPE_WRONG;
	}
	//����ײͲ����Ƿ����
	DATA_TYPE dt;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pData->pet_tid, ID_SPACE_ESSENCE, dt);
	if(petess == NULL || dt != DT_PET_ESSENCE)	return S2C::ERR_ITEM_NOT_IN_INVENTORY;	
	PET_EVOLVE_CONFIG * pec = (PET_EVOLVE_CONFIG *)world_manager::GetDataMan().get_data_ptr(PET_EVOLVE_CONFIG_ID,ID_SPACE_CONFIG,dt);	
	if(dt != DT_PET_EVOLVE_CONFIG || pec == NULL) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(formula_index < 0 || formula_index > 2) return S2C::ERR_ITEM_NOT_IN_INVENTORY; 
	int cost_index = petess->cost_index;
	int cost1 = pec->cost[cost_index].num_rand_skill[formula_index][0];
	int cost2 = pec->cost[cost_index].num_rand_skill[formula_index][1];
	if((cost1 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM1,cost1)) ||(cost2 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM2,cost2)))
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(HasSession())
		return S2C::ERR_CANNOT_REBUILD;

	session_rebuild_pet_nature *pSession = new session_rebuild_pet_nature(this,pData->pet_tid,index,formula_index);
	if(AddSession(pSession)) StartSession();
	return 0;
}

int
gplayer_imp::PlayerRebuildPetInheritRatio(unsigned int index,int formula_index)
{
	//�������Ƿ�Ϸ�
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return S2C::ERR_PET_IS_NOT_EXIST;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData) return S2C::ERR_PET_IS_NOT_EXIST;
	if(_petman.GetCurActivePet() == (int)index) return S2C::ERR_PET_IS_ALEARY_ACTIVE;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION)
	{
		return S2C::ERR_PET_TYPE_WRONG;
	}
	//�������ײ��Ƿ����
	DATA_TYPE dt;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pData->pet_tid, ID_SPACE_ESSENCE, dt);
	if(petess == NULL || dt != DT_PET_ESSENCE)	return S2C::ERR_ITEM_NOT_IN_INVENTORY;	
	PET_EVOLVE_CONFIG * pec = (PET_EVOLVE_CONFIG *)world_manager::GetDataMan().get_data_ptr(PET_EVOLVE_CONFIG_ID,ID_SPACE_CONFIG,dt);	
	if(dt != DT_PET_EVOLVE_CONFIG || pec == NULL) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(formula_index < 0 || formula_index > 2) return S2C::ERR_ITEM_NOT_IN_INVENTORY; 
	int cost_index = petess->cost_index;
	int cost1 = pec->cost[cost_index].num_inherit[formula_index][0];
	int cost2 = pec->cost[cost_index].num_inherit[formula_index][1];
	if((cost1 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM1,cost1)) || (cost2 && !_inventory.IsItemExist(PET_EVOLUTION_ITEM2,cost2)))
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(HasSession())
		return S2C::ERR_CANNOT_REBUILD;
	
	session_rebuild_pet_inheritratio *pSession = new session_rebuild_pet_inheritratio(this,pData->pet_tid,index,formula_index);
	if(AddSession(pSession)) StartSession();
	return 0;
}

bool 
gplayer_imp::RebulidPetInheritRatio(int pet_id,unsigned int index,int formula_index,int &r_attack,int &r_defense,int &r_hp,int &r_atk_lvl,int &r_def_lvl/*, int &r_pen_lvl_f, int &r_pen_lvl_m, int &r_chi_point*/)
{
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return false;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData || pData->pet_tid != pet_id) return false;
	if(_petman.GetCurActivePet() == (int)index) return false;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION)
	{
		return false;
	}
	if(!_petman.RebuildInheritRatio(pet_id,r_attack,r_defense,r_hp,r_atk_lvl,r_def_lvl/*,r_pen_lvl_f,r_pen_lvl_m,r_chi_point*/))
	{
		return false;
	}
	//���Ͽ۳�
	DATA_TYPE dt;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_id, ID_SPACE_ESSENCE, dt);
	if(petess == NULL || dt != DT_PET_ESSENCE)	return false;	
	PET_EVOLVE_CONFIG * pec = (PET_EVOLVE_CONFIG *)world_manager::GetDataMan().get_data_ptr(PET_EVOLVE_CONFIG_ID,ID_SPACE_CONFIG,dt);	
	if(dt != DT_PET_EVOLVE_CONFIG || pec == NULL) return false;
	int cost_index = petess->cost_index;
	int cost1 = pec->cost[cost_index].num_inherit[formula_index][0];
	int cost2 = pec->cost[cost_index].num_inherit[formula_index][1];
	RemoveItems(PET_EVOLUTION_ITEM1,cost1, S2C::DROP_TYPE_USE, true);
	RemoveItems(PET_EVOLUTION_ITEM2,cost2, S2C::DROP_TYPE_USE, true);
	//֪ͨ�ͻ���
	_runner->pet_rebuild_inherit_info((int)g_timer.get_systime() + 30,pet_id,index,r_attack,r_defense,r_hp,r_atk_lvl,r_def_lvl/*,r_pen_lvl_f,r_pen_lvl_m,r_chi_point*/);
	return true;
}

void 
gplayer_imp::PlayerAcceptRebuildInheritResult(bool isaccept)
{
	if (!_cur_session || _cur_session->GetRunTimeClass()->GetGUID() != CLS_SESSION_REBUILD_PET_INHERITRATIO || _cur_session->_session_id < 0)
	{
		return;
	}

	((session_rebuild_pet_inheritratio*)_cur_session)->AcceptResult(isaccept);
}

void
gplayer_imp::AcceptInheritRatioResult(int pet_id,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/)
{
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData || pData->pet_tid != pet_id) return;
	if(_petman.GetCurActivePet() == (int)index) return;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION) return;

	_petman.PetAcceptInheritRatioResult(this,index,r_attack,r_defense,r_hp,r_atk_lvl,r_def_lvl/*,r_pen_lvl_f,r_pen_lvl_m,r_chi_point*/);
}

bool
gplayer_imp::RebuildPetNature(int pet_id,unsigned int index,int formula_index,int &nature)
{
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return false;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData || pData->pet_tid != pet_id) return false;
	if(_petman.GetCurActivePet() == (int)index) return false;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION)
	{
		return false;
	}
	//�ٴ�����Ը�
	DATA_TYPE dt;
	PET_EVOLVE_CONFIG * pec = (PET_EVOLVE_CONFIG *)world_manager::GetDataMan().get_data_ptr(PET_EVOLVE_CONFIG_ID,ID_SPACE_CONFIG,dt);	
	if(dt != DT_PET_EVOLVE_CONFIG || pec == NULL) return false;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pData->pet_tid, ID_SPACE_ESSENCE, dt);
	if(dt != DT_PET_ESSENCE || petess == NULL) return false;
	PET_EVOLVED_SKILL_RAND_CONFIG *pskill = (PET_EVOLVED_SKILL_RAND_CONFIG *)world_manager::GetDataMan().get_data_ptr(petess->id_evolved_skill_rand,ID_SPACE_CONFIG,dt);
	if(dt != DT_PET_EVOLVED_SKILL_RAND_CONFIG || pskill == NULL) return false;
	int nature_group = abase::RandSelect(&(pskill->rand_skill_group[0].probability),sizeof(pskill->rand_skill_group[0]),3);
	int count = 0;
	unsigned int *pList = pskill->rand_skill_group[nature_group].list;
	for(int i=0;i<30;i++)
	{
		if(*(pList+i) == 0)
			continue;
		count ++;
	}
	int offset = abase::Rand(0,count-1);
	nature = pskill->rand_skill_group[nature_group].list[offset];
	//�۳�����
	int cost_index = petess->cost_index;
	int cost1 = pec->cost[cost_index].num_rand_skill[formula_index][0];
	int cost2 = pec->cost[cost_index].num_rand_skill[formula_index][1];
	RemoveItems(PET_EVOLUTION_ITEM1,cost1, S2C::DROP_TYPE_USE, true);
	RemoveItems(PET_EVOLUTION_ITEM2,cost2, S2C::DROP_TYPE_USE, true);
	//֪ͨ�ͻ���
	_runner->pet_rebuild_nature_info((int)g_timer.get_systime()+30,pet_id,index,nature);
	return true;
}
void 
gplayer_imp::PlayerAcceptRebuildNatureResult(bool isaccept)
{
	if (!_cur_session || _cur_session->GetRunTimeClass()->GetGUID() != CLS_SESSION_REBUILD_PET_NATURE || _cur_session->_session_id < 0)
	{
		return;
	}

	((session_rebuild_pet_nature*)_cur_session)->AcceptResult(isaccept);
}

void
gplayer_imp::PlayerAskForPresent(int roleid, int goods_id, int goods_index, int goods_slot)
{
	if (roleid <= 0) return;
	
	int self_id = GetParent()->ID.id;
	if (self_id == roleid) return;
	
	if(_player_state != PLAYER_STATE_NORMAL) return;

	netgame::mall & shop = world_manager::GetPlayerMall();
	int __group_id = shop.GetGroupId();
	time_t __time = time(NULL);
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	if(goods_slot >= netgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}
	netgame::mall::node_t node;
	if(!shop.QueryGoods(goods_index,node) || node.goods_id != goods_id)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	if(!node.check_owner(0))
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	if(node.entry[goods_slot].cash_need <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}
	if(IsItemForbidShop(node.goods_id)
			|| node.gift_id > 0 && IsItemForbidShop(node.gift_id))
	{
		_runner->error_message(S2C::ERR_ITEM_FORBID_SHOP);
		return;
	}
	if(node.buy_times_limit_mode)
	{
		_runner->error_message(S2C::ERR_SHOPPING_TIMES_LIMIT_ITEM_CANNOT_ASK_FOR);
		return;
	}
	if(node.entry[goods_slot].min_vip_level)
	{
		_runner->error_message(S2C::ERR_SHOPPING_VIP_LIMIT_ITEM_CANNOT_ASK_FOR);
		return;
	}
	
	//�ҵ���ǰ��Ч��group
	int active_group_id = 0;
	if(node.group_active && __group_id != 0)
	{
		if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
			active_group_id = __group_id;	
	}

	if(node.sale_time_active)
	{	
		if(node.entry[goods_slot].group_id == active_group_id && node.entry[goods_slot]._sale_time.CheckAvailable(__time))	
		{
			//���playerѡ���slot�����õ����۷�ʽ������Ҫɨ�赱ǰ��Ч���ڣ����Ƿ񻹴��ڷ��������۷�ʽ�����
			if(node.entry[goods_slot]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
			{
				for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
				{
					if(node.entry[j].cash_need <= 0)
						break;
					if(node.entry[j].group_id == active_group_id 
								&& node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT
								&& node.entry[j]._sale_time.CheckAvailable(__time))
					{
						_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
						return;
					}
				}
			}				
		}
		else
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return;
		}
	}
	else if(node.entry[goods_slot].group_id != active_group_id)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	GMSV::SendPlayerAskForPresent(self_id, roleid, goods_id, goods_index, goods_slot);

	GLog::log(GLOG_INFO,"�û�%d��ʼ�ڰٱ�����%d��ȡ��Ʒ%d",self_id,roleid,goods_id);
}

void
gplayer_imp::PlayerGivePresent(int roleid, int mail_id, int goods_id, int goods_index, int goods_slot)
{
	if (roleid <= 0 || mail_id < -1) return;
	
	int self_id = GetParent()->ID.id;
	if (self_id == roleid) return;
	
	if(_player_state != PLAYER_STATE_NORMAL) return;

	if (OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	netgame::mall & shop = world_manager::GetPlayerMall();
	int __group_id = shop.GetGroupId();
	time_t __time = time(NULL);
	netgame::mall_order  order(_mall_order_id);
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	if(goods_slot >= netgame::mall::MAX_ENTRY)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}
	netgame::mall::node_t node;
	if(!shop.QueryGoods(goods_index,node) || node.goods_id != goods_id)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	if(!node.check_owner(0))
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}

	if(node.entry[goods_slot].cash_need <= 0)
	{
		_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
		return;
	}
	if(IsItemForbidShop(node.goods_id)
			|| node.gift_id > 0 && IsItemForbidShop(node.gift_id))
	{
		_runner->error_message(S2C::ERR_ITEM_FORBID_SHOP);
		return;
	}
	if(node.buy_times_limit_mode)
	{
		_runner->error_message(S2C::ERR_SHOPPING_TIMES_LIMIT_ITEM_CANNOT_GIVE);
		return;
	}

	if(node.entry[goods_slot].min_vip_level)
	{
		_runner->error_message(S2C::ERR_SHOPPING_VIP_LIMIT_ITEM_CANNOT_GIVE);
		return;
	}
	
	//�ҵ���ǰ��Ч��group
	int active_group_id = 0;
	if(node.group_active && __group_id != 0)
	{
		if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
			active_group_id = __group_id;	
	}

	if(node.sale_time_active)
	{	
		if(node.entry[goods_slot].group_id == active_group_id && node.entry[goods_slot]._sale_time.CheckAvailable(__time))	
		{
			//���playerѡ���slot�����õ����۷�ʽ������Ҫɨ�赱ǰ��Ч���ڣ����Ƿ񻹴��ڷ��������۷�ʽ�����
			if(node.entry[goods_slot]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
			{
				for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
				{
					if(node.entry[j].cash_need <= 0)
						break;
					if(node.entry[j].group_id == active_group_id 
								&& node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT
								&& node.entry[j]._sale_time.CheckAvailable(__time))
					{
						_runner->mall_item_buy_failed(goods_index, 0);
						return;
					}
				}
			}				
		}
		else
		{
			_runner->mall_item_buy_failed(goods_index, 0);
			return;
		}
	}
	else if(node.entry[goods_slot].group_id != active_group_id)
	{
		_runner->mall_item_buy_failed(goods_index, 0);
		return;
	}

	//���ӵ�order���ȥ����ôд��Ϊ�˸�ʽ�Ϻ��̳ǹ��򱣳�һ��
	order.AddGoods(node.goods_id, node.goods_count,node.entry[goods_slot].cash_need, node.entry[goods_slot].expire_time,node.entry[goods_slot].expire_type,node.gift_id,node.gift_count,node.gift_expire_time,node.gift_log_price);

	if(GetMallCash() < order.GetPointRequire())
	{
		//no engouh mall cash 
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return;
	}
	
	unsigned int cash_used = 0;
	//��Ǯ�㹻�� ��ʼ����
	int cur_t = g_timer.get_systime();

	int id;
	int count;
	int point;
	int expire_time;
	int expire_type;
	int gift_id;
	int gift_count;
	int gift_expire_time;
	int gift_log_price;
	int log_price1;
	int log_price2;
	item new_item;
	item new_gift;
	bool has_gift = false;
	bool bRst = order.GetGoods(0, id, count,point, expire_time,expire_type,gift_id,gift_count,gift_expire_time,gift_log_price);
	if(bRst)
	{
		//������Ʒ����Ʒ��log�۸�
		log_price1 = point;
		log_price2 = 0;
		if(gift_id > 0 && gift_log_price > 0)
		{
			log_price1 = int((float)point*point/(point+gift_log_price));
			log_price2 = point - log_price1; 
		}
		
		const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(id);
		if(pItem)
		{
			item_data * pItem2 = DupeItem(*pItem);
			int expire_date = 0;
			if(expire_time) 
			{
				if(expire_type == netgame::mall::EXPIRE_TYPE_TIME)
				{
					//��Ч������һ������
					expire_date = cur_t + expire_time;
				}
				else
				{
					//��Ч���ǹ涨����ʧЧ
					expire_date = expire_time;
				}
			}
			int guid1 = 0;
			int guid2 = 0;
			if(pItem2->guid.guid1 != 0)
			{
				get_item_guid(pItem2->type, guid1,guid2);
				pItem2->guid.guid1 = guid1;
				pItem2->guid.guid2 = guid2;
			}

			pItem2->proc_type |= item::ITEM_PROC_TYPE_MALL;
			pItem2->count = count;
			pItem2->expire_date = expire_date;

			//����item����,��Ҫ�ͷ�new_item
			if(!MakeItemEntry(new_item,*pItem2))
			{
				FreeItem(pItem2);
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%d����%dʱ������ʱ��Ʒʧ��",self_id, id, roleid);
				return;
			}
			
			//�������³�ʼ��һ�¿��ܵ�ʱװ
            new_item.InitFromShop();

			cash_used += (unsigned int)log_price1;

			FreeItem(pItem2);
		}
		else
		{
			//��¼������־
			GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%d����%dʱ������Ʒʧ��",self_id, id, roleid);
			return;
		}

		//����Ϊ������Ʒ
		if(gift_id > 0)
		{
			const item_data * pGift = (const item_data*)world_manager::GetDataMan().get_item_for_sell(gift_id);
			if(pGift)
			{
				has_gift = true;
				item_data * pGift2 = DupeItem(*pGift);
				int expire_date = 0;
				if(gift_expire_time) 
				{
					//��Ч������һ������
					expire_date = cur_t + gift_expire_time;
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pGift2->guid.guid1 != 0)
				{
					get_item_guid(pGift2->type, guid1,guid2);
					pGift2->guid.guid1 = guid1;
					pGift2->guid.guid2 = guid2;
				}
				
				pGift2->count = gift_count;
				pGift2->expire_date = expire_date;

				//����item����,��Ҫ�ͷ�new_gift
				if(!MakeItemEntry(new_gift,*pGift2))
				{
					//�ͷ����ɵ���Ʒ
					new_item.Release();
					FreeItem(pGift2);
					//��¼������־
					GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%d����%dʱ������ʱ��Ʒʧ��",self_id, id, roleid);
					return;
				}

				//�������³�ʼ��һ�¿��ܵ�ʱװ
                new_gift.InitFromShop();
				
				cash_used += (unsigned int)log_price2;
				
				FreeItem(pGift2);
			}
			else
			{
				//�ͷ����ɵ���Ʒ
				new_item.Release();
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���ٱ�����Ʒ%d����%dʱ������Ʒ%dʧ��",self_id, goods_id, roleid,gift_id);
				return;
			}
		}
	}
	else
	{
		ASSERT(false);
	}

	GDB::itemlist list;
	if (!has_gift)
	{
		list.list = (GDB::itemdata*)abase::fast_allocator::alloc(sizeof(GDB::itemdata));
		list.count = 1;

		GDB::itemdata * pData = list.list;
		item_list::ItemToData(new_item, pData[0]);
	}
	else
	{
		list.list = (GDB::itemdata*)abase::fast_allocator::alloc(2*sizeof(GDB::itemdata));
		list.count = 2;

		GDB::itemdata * pData = list.list;
		item_list::ItemToData(new_item, pData[0]);
		item_list::ItemToData(new_gift, pData[1]);
	}

	//������Ϣ��gdeliveryd
	object_interface oi(this);
	GMSV::SendPlayerGivePresent(self_id, roleid, cash_used, has_gift, log_price1, log_price2, mail_id, list, oi);

	//�ͷ����ɵ���Ʒ
	new_item.Release();
	new_gift.Release();
	//�ͷ�itemlist
	GetInventory().ReleaseDBData(list);

	GLog::log(GLOG_INFO,"�û�%d��ʼ�ڰٱ�����%d��Ʒ����%d��Ԥ�ƻ���%d��ʣ��%d��",self_id,goods_id,roleid,cash_used,GetMallCash()-(int)cash_used);
}

void
gplayer_imp::AcceptNatureResult(int pet_id,unsigned int index,int nature)
{
	if(index >= pet_manager::MAX_PET_CAPACITY)
		return;

	pet_data * pData = _petman.GetPetData(index);
	if(!pData || pData->pet_tid != pet_id) return;
	if(_petman.GetCurActivePet() == (int)index) return;
	if(pData->pet_class != pet_data::PET_CLASS_EVOLUTION) return;

	//�����µ��Ը��ȡ����
	DATA_TYPE dt;
	PET_EVOLVED_SKILL_CONFIG *pesc = (PET_EVOLVED_SKILL_CONFIG *)world_manager::GetDataMan().get_data_ptr(nature,ID_SPACE_CONFIG,dt);
	if(dt != DT_PET_EVOLVED_SKILL_CONFIG || pesc == NULL) return;

	_petman.PetAcceptNatureResult(this,index,nature, pesc->skills[0].id,pesc->skills[0].level,pesc->skills[1].id,pesc->skills[1].level);
}

void 
gplayer_imp::GetNatureSkill(int nature,int &skill1,int &skill2)
{
	if(!nature) return;
	DATA_TYPE dt;
	PET_EVOLVED_SKILL_CONFIG *pesc = (PET_EVOLVED_SKILL_CONFIG *)world_manager::GetDataMan().get_data_ptr(nature,ID_SPACE_CONFIG,dt);
	if(dt != DT_PET_EVOLVED_SKILL_CONFIG || pesc == NULL) return;
	
	skill1 = pesc->skills[0].id;
	skill2 = pesc->skills[1].id;
}

bool 
gplayer_imp::ChangeEquipAddon(unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type,int recipe_type,int* materials_ids,unsigned char* idxs,int count)
{
	const addonchange_recipe_template * acrt = recipe_manager::GetAddonChangeRecipe(recipe_type);
	
	if(!acrt) return false; // before onserve already check

    if (world_manager::GetGlobalController().CheckServerForbid(SERVER_FORBID_RECIPE, acrt->recipe_id)) return false;

	int ret = 0;
	item_list & inv = GetInventory();
	unsigned int inv_size = inv.Size();

	if(equip_idx >= inv_size || inv[equip_idx].type <= 0 
	   || !inv[equip_idx].HasAddonAtSocket(equip_socket_idx,old_stone_type))// HasAddonAtSocket ˳���ж��� inv[equip_idx].body != NULL 
	{
		//У�����Ʒ�Ƿ����
		ret = S2C::ERR_NO_EQUAL_EQUIPMENT_FAIL;
	}
	else if(GetAllMoney() < acrt->fee)
	{
		ret = S2C::ERR_ENOUGH_MONEY_IN_TRASH_BOX;
	}
	else
	{
		std::set<unsigned char> unique_idx;
		for(int idx = 0; idx < count; ++idx)
		{
			if(0 == acrt->material_list[idx].item_id) continue;

			if(!inv.IsItemExist(idxs[idx], acrt->material_list[idx].item_id , acrt->material_list[idx].count )
					|| !unique_idx.insert(idx).second ) // �Ѿ�����materials_id[idx] == ess->materials[idx]
			{
				ret = S2C::ERR_NO_EQUAL_RECIPE_FAIL;
				break;
			}
		}

		if(ret == 0)
		{
			//�ж��Ƿ�ʯ�ͱ�ʯ�����Ƿ���ȷ
			DATA_TYPE dt;
			STONE_ESSENCE * st_ess = (STONE_ESSENCE *) world_manager::GetDataMan().get_data_ptr(new_stone_type,ID_SPACE_ESSENCE,dt);
			if(dt != DT_STONE_ESSENCE || ! st_ess) 
			{
				ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
			}
			else
			{
				const void * pess = world_manager::GetDataMan().get_data_ptr(inv[equip_idx].type,ID_SPACE_ESSENCE,dt);
				if(pess) 
				{
					switch(dt)
					{
						case DT_WEAPON_ESSENCE:
						if(st_ess->level > ((WEAPON_ESSENCE*)pess)->level) 
							ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                        if (!IsStoneFit(DT_WEAPON_ESSENCE, st_ess->combined_switch))
                            ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
						break;
						case DT_ARMOR_ESSENCE:
						if(st_ess->level > ((ARMOR_ESSENCE*)pess)->level) 
							ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                        if (!IsStoneFit(DT_ARMOR_ESSENCE, st_ess->combined_switch))
                            ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
						break;
						case DT_NEW_ARMOR_ESSENCE: // G17/G18
						if(st_ess->level > ((NEW_ARMOR_ESSENCE*)pess)->level) 
							ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                        if (!IsStoneFit(DT_NEW_ARMOR_ESSENCE, st_ess->combined_switch))
                            ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
						break;
                        case DT_DECORATION_ESSENCE:
                        if (st_ess->level > ((DECORATION_ESSENCE*)pess)->level)
                            ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                        if (!IsStoneFit(DT_DECORATION_ESSENCE, st_ess->combined_switch))
                            ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                        break;
						default:	
							ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
					}
				}
				else
				{
					ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
				}
			}
		}
	}

	if(ret > 0)	//У���ɿͻ��˴��ݵĲ������ִ���
	{
		_runner->error_message(ret);
		GLog::log(GLOG_ERR, "�û�[%d]��ʯת�������ִ���%d",GetParent()->ID.id,ret);
		return false;
	}
	
	if(false == inv[equip_idx].ModifyAddonAtSocket(equip_socket_idx,new_stone_type))
	{
		_runner->error_message(S2C::ERR_MODIFY_ADDON_FAIL);
		GLog::log(GLOG_ERR, "�û�[%d]��ʯת��ִ�г��ִ���",GetParent()->ID.id);
		return false;	
	}

	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,equip_idx);

        //�۳���Ǯ
	if(acrt->fee)
	{
		SpendAllMoney(acrt->fee,true);
		SelfPlayerMoney();
	}
		
	//�۳�����
	for(int idx = 0; idx < count; ++idx)
	{
		if(0 == acrt->material_list[idx].item_id) continue;

		item& it = inv[idxs[idx]];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, acrt->material_list[idx].count);
			
		inv.DecAmount(idxs[idx],acrt->material_list[idx].count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idxs[idx], acrt->material_list[idx].item_id,acrt->material_list[idx].count,S2C::DROP_TYPE_PRODUCE);
	}

	//֪ͨ�ͻ��˳ɹ�ת��
	_runner->equip_addon_update_notify(0,equip_idx,equip_socket_idx,old_stone_type,new_stone_type);

	GLog::log(GLOG_INFO,"�û�%d��װ��%d��%dʹ���䷽%d�����ɱ�ʯ%d���±�ʯ%d",
		GetParent()->ID.id,inv[equip_idx].type,equip_socket_idx,recipe_type,old_stone_type,new_stone_type);

	return true;
}

bool 
gplayer_imp::ReplaceEquipAddon(unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type,unsigned char new_stone_idx)
{
	item_list & inv = GetInventory();
	unsigned int inv_size = inv.Size();

	int ret = 0, total_cost = 0;

	if(equip_idx >= inv_size || inv[equip_idx].type <= 0 || !inv[equip_idx].HasAddonAtSocket(equip_socket_idx,old_stone_type)) 
	{
		//У�����Ʒ�Ƿ����
		ret = S2C::ERR_NO_EQUAL_EQUIPMENT_FAIL;
	}
	else if(new_stone_idx >= inv_size || inv[new_stone_idx].type != new_stone_type)
	{
		ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
	}
	else
	{
		//�ж��Ƿ�ʯ�ͱ�ʯ�����Ƿ���ȷ
		DATA_TYPE dt_old ,dt_new, dt_eq;
		STONE_ESSENCE * ess_old = (STONE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(old_stone_type, ID_SPACE_ESSENCE, dt_old);
		STONE_ESSENCE * ess_new = (STONE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(new_stone_type, ID_SPACE_ESSENCE, dt_new);
		if(dt_old != DT_STONE_ESSENCE || dt_new != DT_STONE_ESSENCE ||! ess_old ||! ess_new) 
		{
			ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
		}
		else if((unsigned int)(total_cost = ess_old->uninstall_price + ess_new->install_price) > GetAllMoney() || total_cost < 0)
		{
			ret = S2C::ERR_OUT_OF_FUND;
		}
		else
		{
			const void * pess = world_manager::GetDataMan().get_data_ptr(inv[equip_idx].type,ID_SPACE_ESSENCE,dt_eq);
			if(pess) 
			{
				switch(dt_eq)
				{
					case DT_WEAPON_ESSENCE:
					if(ess_new->level > ((WEAPON_ESSENCE*)pess)->level) 
						ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                    if (!IsStoneFit(DT_WEAPON_ESSENCE, ess_new->combined_switch))
                        ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
					break;
					case DT_ARMOR_ESSENCE:
					if(ess_new->level > ((ARMOR_ESSENCE*)pess)->level) 
						ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                    if (!IsStoneFit(DT_ARMOR_ESSENCE, ess_new->combined_switch))
                        ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
					break;
					case DT_NEW_ARMOR_ESSENCE: // G17/G18
					if(ess_new->level > ((NEW_ARMOR_ESSENCE*)pess)->level) 
						ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                    if (!IsStoneFit(DT_NEW_ARMOR_ESSENCE, ess_new->combined_switch))
                        ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
					break;
                    case DT_DECORATION_ESSENCE:
                    if (ess_new->level > ((DECORATION_ESSENCE*)pess)->level)
                        ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                    if (!IsStoneFit(DT_DECORATION_ESSENCE, ess_new->combined_switch))
                        ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
                    break;
					default:	
						ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
				}
			}			
			else
			{
				ret = S2C::ERR_NO_EQUAL_DEST_FAIL;
			}
		}
	}


	if(ret > 0)	//У���ɿͻ��˴��ݵĲ������ִ���

	{
		_runner->error_message(ret);
		GLog::log(GLOG_ERR, "�û�[%d]��ʯ�滻�����ִ���%d",GetParent()->ID.id,ret);
		return false;
	}

	if(false == inv[equip_idx].ModifyAddonAtSocket(equip_socket_idx,new_stone_type))
	{
		_runner->error_message(S2C::ERR_MODIFY_ADDON_FAIL);
		GLog::log(GLOG_ERR, "�û�[%d]��ʯ�滻ִ�г��ִ���",GetParent()->ID.id);
		return false;	
	}
	
	//֪ͨ�ͻ������ݸ���
	PlayerGetItemInfo(IL_INVENTORY,equip_idx);
	
	//�۳��滻�û�ʯ
	item& it = inv[new_stone_idx];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
			
	inv.DecAmount(new_stone_idx,1);
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,new_stone_idx, new_stone_type, 1,S2C::DROP_TYPE_PRODUCE);

	// �۳��滻�ý�Ǯ
	SpendAllMoney(total_cost,true);
	SelfPlayerMoney();

	//֪ͨ�ͻ��˳ɹ��滻
	_runner->equip_addon_update_notify(1,equip_idx,equip_socket_idx,old_stone_type,new_stone_type);

	GLog::log(GLOG_INFO,"�û�%d��װ��%d��%dʹ���±�ʯ%d�滻�ɱ�ʯ%d",
		GetParent()->ID.id,inv[equip_idx].type,equip_socket_idx,new_stone_type,old_stone_type);

	return true;

}

int
gplayer_imp::PlayerTryRefineMeridian(int index)
{
	//������ŵ�index�Ƿ�Ϸ� 0-47
	if(index < 0 || index >= MERIDIAN_TRIGRAMS_SIZE) return S2C::ERR_TRY_REFINE_MERIDIAN_FAIL;
	//�жϵȼ��Ƿ�Ϸ�
	if(!_meridianman.CheckMeridianCondition(GetHistoricalMaxLevel())) return S2C::ERR_TRY_REFINE_MERIDIAN_FAIL;
	int free_cost_id = MERIDIAN_REFINE_COST1;
	int free_cost_index = _inventory.Find(0,MERIDIAN_REFINE_COST1);
	//�����ѳ�������Ƿ��㹻
	if(free_cost_index < 0)
	{
		free_cost_id = MERIDIAN_REFINE_COST2;
		free_cost_index = _inventory.Find(0,MERIDIAN_REFINE_COST2);
		if(free_cost_index < 0)
		{
			free_cost_id = MERIDIAN_REFINE_COST3;
			free_cost_index = _inventory.Find(0,MERIDIAN_REFINE_COST3);
		}
	}
	if(free_cost_index < 0)
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
	bool have_free_times = _meridianman.CheckMeridianFreeRefineTimes();
	int paid_cost_id = MERIDIAN_PAID_REFINE_COST1;
	//���û����ѳ��������Ҫ����շѳ������
	if(!have_free_times)
	{
		int paid_cost_index = _inventory.Find(0,MERIDIAN_PAID_REFINE_COST1);
		if(paid_cost_index < 0)
		{
			paid_cost_id = MERIDIAN_PAID_REFINE_COST2;
			paid_cost_index = _inventory.Find(0,MERIDIAN_PAID_REFINE_COST2);
			if(paid_cost_index < 0)
			{
				paid_cost_id = MERIDIAN_PAID_REFINE_COST3;
				paid_cost_index = _inventory.Find(0,MERIDIAN_PAID_REFINE_COST3);
				if(paid_cost_index < 0)
				{
					paid_cost_id = MERIDIAN_PAID_REFINE_COST4;
					paid_cost_index = _inventory.Find(0,MERIDIAN_PAID_REFINE_COST4);
				}
			}
		}
		if(paid_cost_index < 0)
			return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	}
	//���г��	
	_meridianman.Deactivate(this);	
	int rst = _meridianman.TryRefineMeridian(this,index);
	if(rst == meridian_manager::MERIDIAN_FATAL_ERR)
	{
		_meridianman.Activate(this);
		return S2C::ERR_TRY_REFINE_MERIDIAN_FAIL;
	}
	_meridianman.Activate(this);
	//����һ�¾�������
	if(rst == meridian_manager::MERIDIAN_LIFE_REFINE)
	{
		property_policy::UpdatePlayer(GetPlayerClass(),this);
	}
	//�۳�����
	RemoveItems(free_cost_id,1,S2C::DROP_TYPE_USE, true);
	if(!have_free_times)
	{
		RemoveItems(paid_cost_id,1,S2C::DROP_TYPE_USE, true);
	}
	//֪ͨ�ͻ���
	_runner->try_refine_meridian_result(index,rst);	
	return 0;
}

void
gplayer_imp::PlayerAddFreeRefineTimes(int num)
{
	_meridianman.AddFreeRefineTime(num);
	_meridianman.NotifyMeridianData(this);
}

void 
gplayer_imp::OnTouchPointQuery(int64_t income,int64_t remain)
{
	_touch_order.income = income;
	_touch_order.remain = remain;

	_runner->notify_touch_query(income,remain,0);
}

void 
gplayer_imp::OnTouchPointCost(int64_t orderid,unsigned int cost,int64_t income,int64_t remain,int retcode)
{
	_touch_order.income = income;
	_touch_order.remain = remain;

	if(!_touch_order.IsHalfTrade(orderid))
	{
		GLog::log(LOG_ERR,"roleid:%d cost point callback fail [orderid%lld cost%d income%lld remain%lld retcode%d]",_parent->ID.id,orderid,cost,income,remain,retcode);
		return;
	}

	GLog::log(LOG_INFO,"roleid:%d cost point orderid%lld cost%d income%lld remain%lld retcode%d",_parent->ID.id,orderid,cost,income,remain,retcode);

	// log
	switch(retcode)
	{
		case touch_trade::TPC_SUCCESS:
		case touch_trade::TPC_COMPLETE:
			_touch_order.Complete(this);
			break;
		case touch_trade::TPC_BUSY:
			GLog::log(LOG_INFO,"roleid:%d touch %lld cost point busy",_parent->ID.id,orderid);
			break;
		case touch_trade::TPC_ORDER_CLASH:
			_touch_order.OnIdClash(this);
			break;
		default:
			_touch_order.ClearData();
			_runner->notify_touch_cost(income,remain,cost,0,0,retcode);
			break;
	}
}

void 
gplayer_imp::OnAuAddupMoneyQuery(int64_t addupmoney)
{
	_runner->notify_addup_money(addupmoney);
}

void 
gplayer_imp::PlayerTouchPointQuery()
{
	if(!_touch_order.Query(this))
	{
		_runner->notify_touch_query(_touch_order.income,_touch_order.remain,1); // busy
	}
}

void 
gplayer_imp::PlayerTouchPointCost(unsigned int index,int type,unsigned int count,unsigned int price,int expire,unsigned int lots)
{
	if(type <= 0 || count <= 0 || lots <= 0) return;

	float total_costf = (float)lots*(float)price;
	if(total_costf > 2e9) // unsigned int max
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,lots*price,index,lots,touch_trade::TPC_NEED_MORE);
		return;
	}

	unsigned int total_cost32 = lots*price;

	if(_touch_order.remain < total_cost32)
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_NEED_MORE);
		return;
	}

	if(!_touch_order.IsFree())
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_BUSY);
		return;
	}

	if(!world_manager::GetTouchShop().CheckGoods(index,type,count,price,expire))
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_OTHER);
		return;
	}

	if(OI_TestSafeLock() || _lock_inventory)
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_OTHER);
		return;
	}

	int pile_limit = world_manager::GetDataMan().get_item_pile_limit(type);
	if( 0 >= pile_limit)
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_OTHER);
		return;
	}

	unsigned int needslots = ((count*lots + pile_limit - 1)/pile_limit);	
	if(!InventoryHasSlot(needslots))
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_NEED_SLOT);
		return;
	}

	if(!_touch_order.TryCost(this,index,type,count,total_cost32,expire,lots))
	{
		_runner->notify_touch_cost(_touch_order.income,_touch_order.remain,total_cost32,index,lots,touch_trade::TPC_BUSY);
	}
}

void 
gplayer_imp::PlayerRedeemGiftCard(const char(&cn)[player_giftcard::GIFT_CARDNUMBER_LEN])
{
	int retcode = 0;
	if((retcode = _player_giftcard.TryRedeem(this,cn)) != 0)
	{
		_runner->notify_giftcard_redeem(retcode,0,0,cn);	
	}
}

void 
gplayer_imp::OnGiftCardRedeem(const char(&cn)[player_giftcard::GIFT_CARDNUMBER_LEN],int type,int parenttype,int retcode)
{
	_player_giftcard.OnRedeem(this,cn,type,parenttype,retcode);
}

void gplayer_imp::PlayerQueryTitle(int roleid)
{
	_player_title.QueryTitleData(_runner);
}

void gplayer_imp::PlayerChangeTitle(TITLE_ID titleid)
{
	_player_title.ChangeCurrTitle(titleid);
}

void gplayer_imp::UpdateDisplayTitle(TITLE_ID titleid)
{
	GetParent()->title_id = titleid;

	if(titleid)
		GetParent()->object_state2 |= gactive_object::STATE_TITLE;
	else
		GetParent()->object_state2 &= ~gactive_object::STATE_TITLE;
}

void gplayer_imp::OnObtainRareTitle(TITLE_ID titleid)
{
	struct 
	{
		unsigned short titleid;
		char name[MAX_USERNAME_LENGTH];
	}data;
	memset(&data,0,sizeof(data));

	data.titleid = titleid;
	unsigned int  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.name, _username,len);

	broadcast_chat_msg(TITLE_RARE_CHAT_MSG_ID, &data, sizeof(data), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0);
	__PRINTF("�����ƶ��㲥\n");
}

void gplayer_imp::PlayerDailySignin()
{
	if(_inventory.GetEmptySlotCount() < 1) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	
	/*
	PlayerApplySigninAward(player_dailysign::REQ_DAILY_AWARD , 0);
	if(_player_dailysign.DaySignIn(0))
	{
		 GLog::log(LOG_INFO,"roleid:%d daily signin success",_parent->ID.id);	
	}
	*/
	_player_dailysign.DaySignIn2(0);
}

void gplayer_imp::PlayerLateSignin(char type,int itempos,int desttime)
{
	if(_lock_inventory || OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return;
	}

	if(_inventory.GetEmptySlotCount() < 1) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}

	int result = 0, item_need = 0, item_idx = -1;
	int * ITEM_DELAY_SIGNIN = LuaManager::GetInstance()->GetConfig()->ITEM_DELAY_SIGNIN;
	for (int i = 0; ITEM_DELAY_SIGNIN[i] && i < 16 ; i++)
	{
		item_idx = _inventory.Find(0,ITEM_DELAY_SIGNIN[i]);
		if(item_idx >= 0) 
		{
			item_need = ITEM_DELAY_SIGNIN[i];
			break;
		}
	}	
	if(item_idx < 0)
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return ;
	}
	
	
	switch (type)
	{
	case 0:
	{
		result = _player_dailysign.DaySignIn2(1);
		break;
	}
	case 1:
	{
		result = _player_dailysign.DaySignIn2(2);
		break;
	}	
	default:
	break;
	}
	
	if( result && !LuaManager::GetInstance()->IsGoodItem(item_need) ) 
	{
		item& it = _inventory[item_idx];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_inventory.DecAmount(item_idx,1);
		_runner->use_item(gplayer_imp::IL_INVENTORY,item_idx, item_need,1);
	}

	/*
	else if(1 == type && player_template::GetDebugMode()) // ������
	{
		_player_dailysign.MonthSignIn(desttime);
		GLog::log(LOG_INFO,"roleid:%d month late signin success",_parent->ID.id); 
	}
	else if(2 == type && player_template::GetDebugMode()) // ������
	{
		_player_dailysign.YearSignIn(desttime);
		GLog::log(LOG_INFO,"roleid:%d year late signin success",_parent->ID.id); 
	}
	*/
}

void gplayer_imp::PlayerApplySigninAward(char type ,int mon)
{
	if(_lock_inventory || OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return;
	}

	if(_inventory.GetEmptySlotCount() < 1) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}

	//_player_dailysign.RequestAwards(type, mon);
}

void gplayer_imp::PlayerRefreshSignin()
{
	if(_inventory.GetEmptySlotCount() < 1) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}

	_player_dailysign.CheckPoint2();
	_player_dailysign.ClientSync(player_dailysign::SYNC4INIT);
}

void gplayer_imp::PlayerSwitchInParallelWorld(const instance_hash_key & key)
{
	if((_player_state != PLAYER_STATE_NORMAL
				&& _player_state != PLAYER_SIT_DOWN) 
				|| IsCombatState())
	{
		_runner->error_message(S2C::ERR_CANNOT_SWITCH_IN_PARALLEL_WORLD);
		return;
	}
	if(!CheckCoolDown(COOLDOWN_INDEX_SWITCH_IN_PARALLEL_WORLD))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	LeaveAbnormalState();
	ASSERT(_player_state == PLAYER_STATE_NORMAL);
	SetCoolDown(COOLDOWN_INDEX_SWITCH_IN_PARALLEL_WORLD, SWITCH_IN_PARALLEL_WORLD_COOLDOWN_TIME);

	int rst = world_manager::GetInstance()->PlayerSwitchWorld(GetParent(), key);
	if(rst > 0) _runner->error_message(rst);
}

void gplayer_imp::PlayerQueryParallelWorld()
{
	if(!CheckCoolDown(COOLDOWN_INDEX_QUERY_PARALLEL_WORLD))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	SetCoolDown(COOLDOWN_INDEX_QUERY_PARALLEL_WORLD, QUERY_PARALLEL_WORLD_COOLDOWN_TIME);
	
	world_manager::GetInstance()->PlayerQueryWorld(GetParent());
}

void gplayer_imp::PlayerReincarnation()
{
	//����״̬�½�ֹת��,��ֹת������Ϊmp�����޷�ʹ�ü��ָܻ�����״̬
	if(GetForm()) return;
	
	if(!_player_reincarnation.CheckCondition())
	{
		_runner->error_message(S2C::ERR_REINCARNATION_CONDITION);
		return;
	}

	//���ͨ������ʼת��
	_player_reincarnation.DoReincarnation();

	RefreshEquipment();
	GlyphEquipRefresh();
	CalcEquipmentInfo();
	PlayerGetProperty();

	_petman.RecallPet(this);

	if(!world_manager::GetWorldParam().pve_mode  || _basic.level > LuaManager::GetInstance()->GetConfig()->pvp_protect_level_max )
	{
		TestPKProtected();
	}
	GMSV::SendSynMutaData(_parent->ID.id,_basic.level, _player_reincarnation.GetTimes());
	_level_up  = true;
}

void gplayer_imp::PlayerGetReincarnationTome()
{
	_player_reincarnation.ClientGetTome();
}

void gplayer_imp::PlayerRewriteReincarnationTome(unsigned int record_index, int record_level)
{
	if(!_player_reincarnation.RewriteTome(record_index, record_level))
	{
		_runner->error_message(S2C::ERR_REINCARNATION_REWRITE_TOME);
		return;
	}

	//��дת���������ʷ��ߵȼ���������,�������������ȼ�/�����ȼ�
	gplayer * pPlayer = GetParent();
	int prev_invisible_degree = pPlayer->invisible_degree;
	int prev_anti_invisible_degree = pPlayer->anti_invisible_degree;
	bool invisible_changed = false;
	property_policy::UpdatePlayerInvisible(this);
	if(pPlayer->IsInvisible() && pPlayer->invisible_degree > prev_invisible_degree)
	{
		_runner->on_inc_invisible(prev_invisible_degree,pPlayer->invisible_degree);
		__PRINTF("����%d��������\n",pPlayer->invisible_degree);
		invisible_changed = true;
	}
	if(pPlayer->anti_invisible_degree > prev_anti_invisible_degree)
	{
		__PRINTF("�����ȼ�%d\n",pPlayer->anti_invisible_degree);
		_runner->on_inc_anti_invisible(prev_anti_invisible_degree,pPlayer->anti_invisible_degree);
		invisible_changed = true;
	}
	if(invisible_changed) _petman.NotifyInvisibleData(this);
}

void gplayer_imp::PlayerActiveReincarnationTome(bool b)
{
	if(!b)
	{
		//ȡ������
		_player_reincarnation.DeactivateTome();
		return;
	}
	//����	
	if(!_player_reincarnation.TryActivateTome())
	{
		_runner->error_message(S2C::ERR_REINCARNATION_ACTIVE_TOME);
	}
}

void gplayer_imp::OnAutoTeamPlayerReady(int leader_id)
{
	int retcode = -1;
	int world_tag = world_manager::GetWorldTag();
	//��ҽ������ڴ��ͼ �����ع� ��ħ�ع� ת���ع� ���͡�� �����Զ����
	if(world_tag == 1 || world_tag == 121 || world_tag == 122 
		|| world_tag == 137 || world_tag == 161 || world_tag == 163 || world_tag == 178)  
	{
		retcode = _team.OnAutoTeamPlayerReady(leader_id);
	}

	GMSV::SendAutoTeamPlayerReady_Re(_parent->ID.id, leader_id, retcode);
}

void gplayer_imp::OnAutoTeamComposeFailed(int leader_id)
{
	_team.OnAutoTeamComposeFailed(leader_id);
}

void gplayer_imp::OnAutoTeamComposeStart(int member_list[], unsigned int cnt)
{
	_team.OnAutoTeamComposeStart(member_list, cnt);
}

void gplayer_imp::PlayerSetAutoTeamGoal(char goal_type, char op, int goal_id)
{
	if(_team.IsInTeam()) return;
	
	int world_tag = world_manager::GetWorldTag();
	//û�ж������ҿ����ڴ��ͼ�������ع�����ħ�ع���ת���µع� ���͡�� ������������
	if(op > 0 && world_tag != 1 && world_tag != 121 && world_tag != 122 && world_tag != 137 && world_tag != 161 && world_tag != 163 && world_tag != 178 ) return;
	if(goal_id <= 0) return;
	if(goal_type <= player_team::auto_team_info_t::GOAL_TYPE_INVALID || goal_type > player_team::auto_team_info_t::GOAL_TYPE_ACTIVITY) return;

	if((goal_type == player_team::auto_team_info_t::GOAL_TYPE_ACTIVITY) && (op > 0))
	{
		if(!CheckCoolDown(COOLDOWM_INDEX_AUTOTEAM_SET_GOAL)) return;
		bool ret = world_manager::GetAutoTeamMan().CanPlayerAutoComposeTeam(goal_id, this);
		if(!ret) return;
		SetCoolDown(COOLDOWM_INDEX_AUTOTEAM_SET_GOAL, AUTOTEAM_SET_GOAL_COOLDOWN_TIME);
	}

	GMSV::SendPlayerSetAutoTeamGoal(_parent->ID.id, goal_type, op, goal_id);
}

void gplayer_imp::PlayerJumpToGoal(int goal_id)
{
	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_STATE_BIND
			&& _player_state != PLAYER_SIT_DOWN
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return;
	}
	if(IsCombatState() || HasSession()) return;
	if(!CheckCoolDown(COOLDOWM_INDEX_AUTOTEAM_SET_GOAL)) return;
	
	int current_worldtag = world_manager::GetWorldTag();
	int target_tag = -1;
	A3DVECTOR target_pos;
	bool ret = world_manager::GetAutoTeamMan().GetGoalEntrancePos(goal_id, this, current_worldtag, target_tag, target_pos);
	if(!ret) return;

	LeaveAbnormalState();
	
	int jump_item_idx = -1;
	for(unsigned int i = 0; i < _inventory.Size(); ++i)
	{
		item& it = _inventory[i];
		if(it.type == -1) continue;
		if((it.type == AUTO_TEAM_JUMP_ITEM1) || (it.type == AUTO_TEAM_JUMP_ITEM2) 
				|| (it.type == AUTO_TEAM_JUMP_ITEM3) || (it.type == AUTO_TEAM_JUMP_ITEM4))
		{
			if((jump_item_idx == -1) || (it.type < _inventory[jump_item_idx].type))
			{
				jump_item_idx = i;
				if(it.type == AUTO_TEAM_JUMP_ITEM1) break;
			}
		}
	}
	
	if(jump_item_idx >= 0)
	{
		item& it = _inventory[jump_item_idx];
		int item_id = it.type;
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		
		_inventory.DecAmount(jump_item_idx, 1);
		_runner->player_drop_item(IL_INVENTORY, jump_item_idx, item_id, 1, S2C::DROP_TYPE_TAKEOUT);

		SetCoolDown(COOLDOWM_INDEX_AUTOTEAM_SET_GOAL, AUTOTEAM_SET_GOAL_COOLDOWN_TIME);
		LongJump(target_pos, target_tag);
	}
}


int gplayer_imp::PlayerTrickBattleApply(int chariot)
{
	if(GetHistoricalMaxLevel() < TRICKBATTLE_LEVEL_REQUIRED) return S2C::ERR_LEVEL_NOT_MATCH;
	if(_basic.sec.level < TRICKBATTLE_SEC_LEVEL_REQUIRED) return S2C::ERR_SEC_LEVEL_NOT_MATCH;
	if(!CheckCoolDown(COOLDOWM_INDEX_TRICKBATTLE_APPLY)) return S2C::ERR_OBJECT_IS_COOLING;

	DATA_TYPE dt;
	CHARIOT_CONFIG * cfg = (CHARIOT_CONFIG *)world_manager::GetDataMan().get_data_ptr(chariot, ID_SPACE_CONFIG, dt);
	if(!cfg || dt != DT_CHARIOT_CONFIG) return -1;
	if(cfg->pre_chariot > 0) return -1;	//ֻ��ѡ�����ս��

	GMSV::SendTrickBattleApply(_parent->ID.id, chariot);
	SetCoolDown(COOLDOWM_INDEX_TRICKBATTLE_APPLY, TRICKBATTLE_APPLY_COOLDOWN_TIME);
	return 0;
}

void gplayer_imp::PlayerLeaveTrickBattle()
{
	if(world_manager::GetInstance()->GetWorldType() != WORLD_TYPE_TRICKBATTLE) return;
	
	_filters.ModifyFilter(FILTER_CHECK_INSTANCE_KEY,FMID_CLEAR_AETB,NULL,0);	
}

void gplayer_imp::EnterTrickBattleStep1(int target_tag, int battle_id, int chariot)
{
	if(_player_state != PLAYER_STATE_NORMAL)
	{
		return;
	}

	if(target_tag == world_manager::GetWorldTag()) return ;
	
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	key.target.key_level4 = battle_id;

	ClearSwitchAdditionalData();
	A3DVECTOR pos(0,0,0);
	//��Player���и������� 
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,target_tag,key,0) < 0)
	{
		return ;
	}
	_switch_additional_data = new trickbattle_switch_data(chariot);
}

void gplayer_imp::EnterTrickBattleStep2()
{
	trickbattle_switch_data * pData = substance::DynamicCast<trickbattle_switch_data>(_switch_additional_data);
	ASSERT(pData);

	TrickBattleTransformChariot(pData->chariot);

	ClearSwitchAdditionalData();
}

void gplayer_imp::ReceiveRealmExp(int exp, int daily_exp)
{
    exp = (int)(exp * (1.0f + _realm_exp_factor) + 0.1f);
    if (exp <= 0) return;

	// Novas rates 
	exp *= EmulateSettings::GetInstance()->GetRatesConfig()->realm_exp;

	/*174+*/
	if(_double_factor_realm)
	exp = (int)(exp * DOUBLE_EXP_FACTOR + 0.1f);
	
	_realm_exp += exp;
	// Pontos diários
	if(daily_exp > 0)
	{
		if(daily_exp > 4000 && daily_exp < 0) return;

		int resultrealm = _diary_exp + (int)(daily_exp * (1.0f + _realm_exp_factor) + 0.1f);
		_diary_exp = resultrealm;
	}
	
	bool levelup =false;
	do
	{
		int levelup_exp = player_template::GetRealmLvlupExp(_realm_level);
		if(_realm_exp < levelup_exp) break;
		if(_realm_level %10 == 0)
		{
			exp -= (_realm_exp - levelup_exp);
			_realm_exp = levelup_exp;			
			break;
		}

		_realm_exp -= levelup_exp;
		_realm_level ++;
		levelup = true;
	}while(1);
	
	if(levelup)
	{
		UpdateRealmLevel();

		/*160+*/
		SpeedSkillManager();

		/*174+*/
		ActivityEventActivate();

		if(_activity.enable_skill)
		ActivityEventSkillManager();

		ProtocolActivityEvent();
	}

 	_runner->realm_exp_receive(_realm_exp, exp, _diary_exp);
}

void gplayer_imp::UpdateRealmLevel()
{
	GetParent()->realmlevel = _realm_level;
	GetParent()->object_state2 |= gactive_object::STATE_REALMLEVEL;
	
	_runner->realm_level_up(_realm_level);

	SetVigourBase( player_template::GetRealmVigour(_realm_level) );
	
	PlayerGetProperty();
}

bool gplayer_imp::ExpandRealmLevelMax()
{
	if(!IsRealmExpFull())
		return false;
	
	_realm_exp = 0;
	_runner->realm_exp_receive(_realm_exp, 0, _diary_exp);
	++_realm_level; 
	UpdateRealmLevel();

	return true;
}

bool gplayer_imp::IsRealmExpFull()
{
	return _realm_level%10 == 0 
		&& _realm_level < player_template::GetMaxRealmLevelLimit() 
		&& _realm_exp >= player_template::GetRealmLvlupExp(_realm_level);
}

int gplayer_imp::GetObtainedGeneralCardCountByRank(int rank)
{
	if (rank < 0 || rank > 4)
		return 0;

	int count = 0;
	unsigned int size = _inventory.Size();
	for(unsigned int i = 0; i < size; ++i)
	{
		item& it = _inventory[i];

		if (it.type <= 0 || it.body == NULL || 
				it.body->GetItemType() != item_body::ITEM_TYPE_GENERALCARD)
			continue;
		
		if (it.body->GetRank() == rank)
			count ++;
	}

	for(unsigned int i = item::EQUIP_INDEX_GENERALCARD1; i < item::EQUIP_INDEX_GENERALCARD6 + 1; i++)
	{
		item& it = _equipment[i];
		
		if (it.type <= 0 || it.body == NULL || 
				it.body->GetItemType() != item_body::ITEM_TYPE_GENERALCARD)
			continue;
		
		if (it.body->GetRank() == rank)
			count ++;
	}
	
	item_list & backpack4 = _trashbox.GetBackpack(3);
	size = backpack4.Size();
	for(unsigned int i = 0; i < size; ++i)
	{
		item& it = backpack4[i];
		
		if (it.type <= 0 || it.body == NULL || 
				it.body->GetItemType() != item_body::ITEM_TYPE_GENERALCARD)
			continue;
		
		if (it.body->GetRank() == rank)
			count ++;
	}

	return count;
}

int gplayer_imp::PlayerGeneralCardRebirth(unsigned int major_inv_idx, unsigned int minor_inv_idx)
{
	if(major_inv_idx >= _inventory.Size() || minor_inv_idx >= _inventory.Size()) 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	item & it_major = _inventory[major_inv_idx];
	item & it_minor = _inventory[minor_inv_idx];
	if(it_major.type != it_minor.type
			|| it_major.type <= 0 || it_major.body == NULL || it_major.body->GetItemType() != item_body::ITEM_TYPE_GENERALCARD
			|| it_minor.type <= 0 || it_minor.body == NULL || it_minor.body->GetItemType() != item_body::ITEM_TYPE_GENERALCARD)
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	if(!it_major.CheckRebirthCondition(it_minor.GetRebirthTimes()))
		return S2C::ERR_GENERALCARD_REBIRTH_CONDITION;

	if(it_major.GetRank() >= LuaManager::GetInstance()->GetConfig()->GENERALCARD_RANK_S)
	{
		//S�����Ͽ���ת����log
		GLog::formatlog("rebirthcard:roleid=%d:cardid=%d", _parent->ID.id, it_major.type);
	}

	UpdateMallConsumptionDestroying(it_minor.type, it_minor.proc_type, 1);
	_runner->player_drop_item(IL_INVENTORY, minor_inv_idx, it_minor.type, 1, S2C::DROP_TYPE_TAKEOUT);
	_inventory.DecAmount(minor_inv_idx, 1);

	it_major.DoRebirth(0);
	PlayerGetItemInfo(IL_INVENTORY, major_inv_idx);
	return 0;
}

int gplayer_imp::PlayerSwallowGeneralCard(unsigned int equip_idx, bool is_inv, unsigned int inv_idx, unsigned int count)
{
	item_list * pInv = NULL;
	if(is_inv) 
		pInv = &GetInventory();
	else
		pInv = &GetTrashInventory(IL_TRASH_BOX4);

	if(equip_idx < item::EQUIP_INDEX_GENERALCARD1 
			|| equip_idx > item::EQUIP_INDEX_GENERALCARD6
			|| inv_idx >= (*pInv).Size()) 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	item & it_eq = _equipment[equip_idx];
	item & it_inv = (*pInv)[inv_idx];
	if(it_eq.type <= 0 || it_inv.type <= 0 || !count || it_inv.count < count) 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
	DATA_TYPE dt;
	const void * ess = world_manager::GetDataMan().get_data_ptr(it_inv.type, ID_SPACE_ESSENCE, dt);
	if(!ess) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	int exp = 0;
	if(dt == DT_POKER_ESSENCE) 
		exp = ((const POKER_ESSENCE *)ess)->swallow_exp;
	else if(dt == DT_POKER_DICE_ESSENCE) 
		exp = ((const POKER_DICE_ESSENCE*)ess)->swallow_exp;
	else 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	//���ɿ���ʱҪ����ÿ�����ǰ������ת�����ĵľ���
	if(dt == DT_POKER_ESSENCE) 
		exp += it_inv.GetSwallowExp();

	if(exp <= 0) return S2C::ERR_GENERALCARD_INSERT_EXP;
	float tmp = (float)exp * count;
	if(tmp > 2e9) return S2C::ERR_GENERALCARD_INSERT_EXP;
	exp *= count;
	
	//ֻ������Ʒ��С�ڵ��������Ŀ���	
	if(dt == DT_POKER_ESSENCE && it_inv.GetRank() > it_eq.GetRank()) 
		return S2C::ERR_GENERALCARD_INSERT_EXP;
	
	//����Ƿ�������Ӿ���
	if(!it_eq.InsertExp(exp, 0,0, true))
	{
		return S2C::ERR_GENERALCARD_INSERT_EXP;
	}

	if(dt == DT_POKER_ESSENCE && it_inv.GetRank() >= LuaManager::GetInstance()->GetConfig()->GENERALCARD_RANK_S)
	{
		//����S�����Ͽ���LOG
		//GLog::formatlog("swallowcard:roleid=%d:cardid=%d:exp=%d", _parent->ID.id, it_inv.type, exp);

        // ��������S�����Ͽ���
        return S2C::ERR_GENERALCARD_INSERT_EXP;
	}

	it_eq.Deactivate(item::BODY, equip_idx, this);
	it_eq.InsertExp(exp, 0,0, false);	
	it_eq.Activate(item::BODY, _equipment, equip_idx, this);

	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
	RefreshEquipment();

	UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
	_runner->player_drop_item((is_inv ? IL_INVENTORY : IL_TRASH_BOX4), inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
	(*pInv).DecAmount(inv_idx, count);
	//�����ֿ���޸Ĳ���Ҫ�򿪲ֿ⣬��Ҫ���Ӹı����
	if(!is_inv) IncTrashBoxChangeCounter();
	return 0;
}

void gplayer_imp::PlayerQueryChariots()
{
	//�ɿͻ����������𣬿���Ҫ�Ӽ�ⲻ��������ȴ
	QueryTrickBattleChariots();
}

int gplayer_imp::PlayerImproveFlysword(unsigned int inv_idx, int flysword_id)
{
	//�����Ʒ�Ƿ����
	if(inv_idx >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & it = _inventory[inv_idx];
	if(it.type == -1 || it.type != flysword_id) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	//���ɽ��Ƿ�ɸ���
	DATA_TYPE dt;
	FLYSWORD_ESSENCE * ess = (FLYSWORD_ESSENCE *)world_manager::GetDataMan().get_data_ptr(flysword_id, ID_SPACE_ESSENCE, dt);
	if(ess == NULL || dt != DT_FLYSWORD_ESSENCE) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(ess->max_improve_level <= 0 || (unsigned int)ess->max_improve_level > sizeof(ess->improve_config)/sizeof(ess->improve_config[0])) return S2C::ERR_ITEM_CANNOT_IMPROVE;

	int ilevel = it.GetImproveLevel();
	if(ilevel >= ess->max_improve_level) return S2C::ERR_ITEM_CANNOT_IMPROVE;
	//�������Ƿ��㹻
	unsigned int icost = ess->improve_config[ilevel].require_item_num;
	int ticket_list[] = {IMPROVE_FLYSWORD_TICKET_ID1, IMPROVE_FLYSWORD_TICKET_ID2, IMPROVE_FLYSWORD_TICKET_ID3};
	if(icost == 0 || !CheckItemExist(ticket_list, sizeof(ticket_list)/sizeof(int), icost)) return S2C::ERR_NOT_ENOUGH_MATERIAL; 
	//���и���
	if(!it.FlyswordImprove(ess->improve_config[ilevel].speed_increase, ess->improve_config[ilevel].speed_rush_increase)) return S2C::ERR_ITEM_CANNOT_IMPROVE;
	//���Ĳ���
	TakeOutItem(ticket_list, sizeof(ticket_list)/sizeof(int), icost);
	//�ͻ��˸��·ɽ�
	PlayerGetItemInfo(IL_INVENTORY, inv_idx);
	return 0;
}

void gplayer_imp::PlayerRandMallQuery(int eid)
{
	DATA_TYPE dt;
	RAND_SHOP_CONFIG * ess = (RAND_SHOP_CONFIG*) world_manager::GetDataMan().get_data_ptr(eid, ID_SPACE_CONFIG, dt);

	if(ess == NULL || dt != DT_RAND_SHOP_CONFIG)
	{
		_runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_QUERY,
				random_mall_info::RM_ENTRY_ERR,0,0,false);
		return ;
	}

	random_mall_info::random_mall_result record;
	bool firstflag = !_player_randmall.QueryResult(eid,record);	
	_runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_QUERY,
			0, record.itemid, record.price, firstflag);
}

int RandSelectInRange(const void * prob, int stride, unsigned int beg, unsigned int end)
{	
	int count = end - beg;
	if(count == 1) return beg;
	ASSERT(prob && stride >= (int)sizeof(float) && count > 0);
	float* prolist = (float*)abase::fast_allocator::align_alloc(count*sizeof(float));

	const char * option = (const char*)prob;
	float total_pro = 0;
	for(unsigned int i = beg; i < end; i++,option += stride)
	{
		float tmp = *(float*)option; ASSERT(tmp >= 0 && tmp <=1.f);
		prolist[i-beg] = tmp;
		total_pro += tmp; ASSERT(total_pro <=1.f);
	}

	float total_pro_div = 1.f/total_pro;
	total_pro = 0;
	for(int n = 0; n < count - 1; n++)
	{
		prolist[n] *= total_pro_div;
		total_pro += prolist[n];
	}
	prolist[count - 1] = 1.f - total_pro;

	int idx = abase::RandSelect(prolist, sizeof(float), count); 
	abase::fast_allocator::align_free(prolist,count*sizeof(float));

	return beg + idx;
}

void gplayer_imp::PlayerRandMallRoll(int eid)
{
#define RM_ROLL_ERR_RET(re) { _runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_ROLL,re,0,0,false);return; }
	if(_lock_inventory || OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}
	
	DATA_TYPE dt;
	const RAND_SHOP_CONFIG * ess = (RAND_SHOP_CONFIG*) world_manager::GetDataMan().get_data_ptr(eid, ID_SPACE_CONFIG, dt);

	if(ess == NULL || dt != DT_RAND_SHOP_CONFIG || ess->first_buy_price < 0 || ess->price <= 0)
		RM_ROLL_ERR_RET(random_mall_info::RM_ENTRY_ERR)

	if(InCentralServer() || GetHistoricalMaxLevel() < random_mall_info::RAND_MALL_LEVEL_LIMIT)
		RM_ROLL_ERR_RET(random_mall_info::RM_ROLE_STATE_ERR)
	
	random_mall_info::random_mall_result record;
	bool firstflag = !_player_randmall.QueryResult(eid,record);	
	
	if(!firstflag && record.itemid)
		RM_ROLL_ERR_RET(random_mall_info::RM_OPT_STATE_ERR)

	// ��ʼroll 
	if(firstflag)
	{
		unsigned int beg = 0;
		unsigned int end = beg + ess->first_buy_range;
		
		unsigned int beg_sz = (sizeof(ess->list)/sizeof(ess->list[0]) - 1);
		unsigned int end_sz = (sizeof(ess->list)/sizeof(ess->list[0]));
		
		beg = std::min(beg, beg_sz);
		end = std::min(end, end_sz);
		
		ASSERT(beg < end);
		int idx = RandSelectInRange(&(ess->list[0].probability), sizeof(ess->list[0]), beg, end);		
		record.itemid = ess->list[idx].id;
		record.price = ess->first_buy_price;
	}
	else
	{
		int idx = abase::RandSelect(&(ess->list[0].probability), sizeof(ess->list[0]),sizeof(ess->list)/sizeof(ess->list[0]));
		record.itemid = ess->list[idx].id;
		record.price = ess->price;
	}

	_player_randmall.PendingPay(eid, record);
	_runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_ROLL, 0, record.itemid, record.price, firstflag);
	
	GLog::log(GLOG_INFO,"�û�%d������̳�%d Roll����Ʒ%d���۸�%d��",GetParent()->ID.id, eid, record.itemid, record.price);
#undef RM_ROLL_ERR_RET
}

void gplayer_imp::PlayerRandMallPay(int eid)
{
#define RM_PAY_ERR_RET(re) { _runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_PAY,re,0,0,false);return; }
	
	if(_lock_inventory || OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return;
	}

	if(InCentralServer() || GetHistoricalMaxLevel() < random_mall_info::RAND_MALL_LEVEL_LIMIT)
		RM_PAY_ERR_RET(random_mall_info::RM_ROLE_STATE_ERR)

	random_mall_info::random_mall_result record;
	_player_randmall.QueryResult(eid,record);	

	if(0 == record.itemid)
		RM_PAY_ERR_RET(random_mall_info::RM_OPT_STATE_ERR)

	if(record.price < 0 || record.price > GetMallCash())
		RM_PAY_ERR_RET(random_mall_info::RM_CASH_ERR)	

	if(!InventoryHasSlot(1))
		RM_PAY_ERR_RET(random_mall_info::RM_INV_ERR)
	
	element_data::item_tag_t tag = {element_data::IMT_SHOP, 0};
	item_data * pItem = world_manager::GetDataMan().generate_item_from_player(record.itemid, &tag, sizeof(tag));
	if(pItem)
	{
		int total_cash = GetMallCash();
		int cash_used = 0;
		//��Ǯ�㹻�� ��ʼ������Ʒ
		int self_id = GetParent()->ID.id;
		int count = 1;
		int expire_date = 0;
		int log_price1 = record.price;
		int id = record.itemid;

		pItem->proc_type |= item::ITEM_PROC_TYPE_MALL;
		UpdateMallConsumptionShopping(pItem->type, pItem->proc_type, count, log_price1);
		
		int ocount = count;
		int rst =_inventory.Push(*pItem,count,expire_date);
		ASSERT(rst >= 0 && count == 0);
		_runner->obtain_item(id,pItem->expire_date,ocount,_inventory[rst].count, 0,rst);
		
		//�������³�ʼ��һ�¿��ܵ�ʱװ
		_inventory[rst].InitFromShop();

		FirstAcquireItem(pItem);					

		total_cash -= log_price1;
		cash_used += log_price1;

		//��¼��־  
		GLog::formatlog("formatlog:gshop_trade:userid=%d:db_magic_number=%d:order_id=%d:item_id=%d:expire=%d:item_count=%d:cash_need=%d:cash_left=%d:guid1=%d:guid2=%d",
				self_id,_db_user_id,_mall_order_id,id,expire_date,ocount,log_price1,total_cash,pItem->guid.guid1,pItem->guid.guid2);
		
		world_manager::TestCashItemGenerated(id, ocount);
		FreeItem(pItem);

		_mall_cash_offset -= cash_used;
		_runner->player_cash(GetMallCash());
		//�����Ѽ�¼���͸�gdelivery,�������ӿ��Ը������ߵĺ������������ѻ���
		GMSV::SendRefCashUsed(_parent->ID.id, cash_used, _basic.level);
		

		GLog::log(GLOG_INFO,"�û�%d������̳�%d����%d����Ʒ%d������%d��ʣ��%d��",self_id,eid,ocount,id,cash_used,GetMallCash());

		_mall_order_id ++;

	}
	else
	{
		GLog::log(GLOG_ERR,"�û�%d������̳�%d������Ʒ%dʧ��",GetParent()->ID.id,eid,record.itemid);
	}

	_player_randmall.ClosingPay(eid);	
	_runner->random_mall_shopping_result(eid,random_mall_info::RAND_MALL_OPT_PAY, 0, record.itemid, record.price, false);


#undef RM_PAY_ERR_RET
}

namespace
{
	struct mafia_staff_collector
	{
		int _self_id;
		int _mafia_id;
		float _squared_radius;
		std::vector<GMSV::MafiaMemberInfo> &_list;
		mafia_staff_collector(int self,int mid, float radius, std::vector<GMSV::MafiaMemberInfo> &list)
			: _self_id(self),_mafia_id(mid),_squared_radius(radius*radius),_list(list){}

		inline void operator()(slice *pPiece,const A3DVECTOR & pos)
		{
			if(!pPiece->player_list) return;
			pPiece->Lock();
			gplayer * pPlayer = (gplayer *)pPiece->player_list;
			while(pPlayer)
			{
				if(pPlayer->ID.id != _self_id && pPlayer->base_info.level >= MAFIA_PVP_LEVEL_LIMIT 
				&& pPlayer->id_mafia == _mafia_id && pos.squared_distance(pPlayer->pos) < _squared_radius)
				{
					GMSV::MafiaMemberInfo member;
					member.roleid = pPlayer->ID.id;
					member.rank = pPlayer->rank_mafia;
					_list.push_back(member);
				}
				pPlayer = (gplayer *)pPlayer->pNext;
			}
			pPiece->Unlock();
		}
	};
}

void gplayer_imp::OnMafiaPvPAward(int type,const XID& sourceid, const A3DVECTOR& pos, int mafiaid, int domainid)
{
	int selfmafia = OI_GetMafiaID();

	if(!world_manager::GetWorldFlag().mafia_pvp_flag || !selfmafia || _basic.level < MAFIA_PVP_LEVEL_LIMIT || 0 == ((gplayer*)_parent)->mafia_pvp_mask)
	{
		GLog::log(GLOG_ERR,"�û�%d�ڰ���pvpδ�в����ʸ�ʱ�ܵ�����%d [ms:%d md:%d d:%d l:%d]",_parent->ID.id,type,selfmafia,mafiaid,domainid,_basic.level);

		if(type == AWARD_MAFIAPVP_MINEBASE) // �����¼�������Ҫͬ��map ��ʾ���¼�����֪ͨ
			GMSV::SendMafiaPvPEvent(AWARD_MAFIAPVP_NO_OWNER_MINEBASE,0,mafiaid,0,0,domainid);
		else if(type == AWARD_MAFIAPVP_MINECAR) // ���¼�������Ҫ������ص÷ֹ��¼�����֪ͨ
			GMSV::SendMafiaPvPEvent(AWARD_MAFIAPVP_NO_OWNER_MINECAR,0,mafiaid,0,0,domainid);
		return;
	}

	switch(type)
	{
		case AWARD_MAFIAPVP_MINECAR:
		case AWARD_MAFIAPVP_MINEBASE:
			{
				std::vector<GMSV::MafiaMemberInfo> list;
				mafia_staff_collector worker(_parent->ID.id,selfmafia,NORMAL_EXP_DISTANCE,list);
				_plane->ForEachSlice(pos,NORMAL_EXP_DISTANCE,worker);

				GMSV::SendMafiaPvPEvent(type,selfmafia,mafiaid,_parent->ID.id,((gplayer*)_parent)->rank_mafia,domainid,&list);	
			}
			break;
		case AWARD_MAFIAPVP_HIJACK_KILL:
			{
				std::vector<GMSV::MafiaMemberInfo> list;
				GMSV::MafiaMemberInfo victim;
				victim.roleid = sourceid.id; 
				victim.rank = 0;
				list.push_back(victim);
				GMSV::SendMafiaPvPEvent(type,selfmafia,mafiaid,_parent->ID.id,((gplayer*)_parent)->rank_mafia,domainid,&list);	
			}
			break;
		case AWARD_MAFIAPVP_MINECAR_PROTECT:
			{
				GMSV::SendMafiaPvPEvent(type,selfmafia,mafiaid,_parent->ID.id,((gplayer*)_parent)->rank_mafia,domainid);		
			}
			break;
	}
}

void gplayer_imp::PlayerQueryMafiaPvPInfo()
{
	if(!world_manager::GetWorldFlag().mafia_pvp_flag) return;
	
	int selfmafia = OI_GetMafiaID();
	if(selfmafia && CheckCoolDown(COOLDOWN_INDEX_QUERY_MAFIA_PVP_INFO))
	{
		GMSV::SendMafiaPvPQuery(_parent->ID.id, selfmafia);
		SetCoolDown(COOLDOWN_INDEX_QUERY_MAFIA_PVP_INFO,PLAYER_QUERY_MAFIA_PVP_COOLDOWN_TIME);
	}
}

void gplayer_imp::OnLookupEnemyReply(const MSG& msg)
{
    int item_id = LOOKUP_ENEMY_ITEM_ID2;
    int item_index = _inventory.Find(0, item_id);
    if (item_index < 0)
    {
        item_id = LOOKUP_ENEMY_ITEM_ID;
        item_index = _inventory.Find(0, item_id);

        if (item_index < 0)
        {
            _runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
            return;
        }
    }

    if (!CheckCoolDown(COOLDOWN_INDEX_LOOKUP_ENEMY))
    {
        _runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
        return;
    }

    if (_lock_inventory)
    {
        _runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
        return;
    }

    _runner->lookup_enemy_result(msg.source.id, msg.param, msg.pos);
    SetCoolDown(COOLDOWN_INDEX_LOOKUP_ENEMY, LOOKUP_ENEMY_COOLDOWN_TIME);

    item& it = _inventory[item_index];
    UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);

    _inventory.DecAmount(item_index, 1);
    _runner->player_drop_item(IL_INVENTORY, item_index, item_id, 1, S2C::DROP_TYPE_USE);
}

void gplayer_imp::copy_other_role_data(int src_roleid)
{
    class CopyCallBack : public GDB::CopyRoleResult, public abase::ASmallObject
	{
    private:
        int _src_roleid;
        int _dst_roleid;
        int _cs_index;
        int _cs_sid;
        gplayer* _pPlayer;
        
    public:
        CopyCallBack(int src_roleid, int dst_roleid, gplayer* pPlayer): 
            _src_roleid(src_roleid), _dst_roleid(dst_roleid), _pPlayer(pPlayer)
        {
            _cs_index = pPlayer->cs_index;
            _cs_sid = pPlayer->cs_sid;
        }

        virtual void OnTimeOut() { OnFailed();}
        virtual void OnFailed() 
        {
            printf("�����������ʧ�� src_roleid=%d, dst_roleid=%d", _src_roleid, _dst_roleid);
            delete this;
        }

        virtual void OnSucceed()
        { 
            spin_autolock keeper(_pPlayer->spinlock);
            if(_pPlayer->ID.id != _dst_roleid || !_pPlayer->IsActived()
                    || _pPlayer->cs_index != _cs_index || _pPlayer->cs_sid != _cs_sid)
            {
                delete this;
                return;
            }

            ASSERT(_pPlayer->imp);
            ((gplayer_imp*)(_pPlayer->imp))->_team.PlayerLogout();
            ((gplayer_imp*)(_pPlayer->imp))->Logout(-1); 
            
            char buff[] = {0};
            GMSV::SendDebugCommand(_dst_roleid, 31, buff, 0);
            delete this;
        }
    };
    
    int dst_roleid = _parent->ID.id;
    gplayer* pPlayer = (gplayer*)_parent;
    GDB::copy_role(src_roleid, dst_roleid, new CopyCallBack(src_roleid, dst_roleid, pPlayer));    
}

void gplayer_imp::PlayerQueryCanInheritAddons(int equip_id, unsigned int inv_idx)
{
    if(!_inventory.IsItemExist(inv_idx, equip_id, 1)) return;
    item& eq_it = _inventory[inv_idx];
	if(eq_it.body == NULL) return;

    DATA_TYPE eq_dt;
	const void* eq_ess = world_manager::GetDataMan().get_data_ptr(eq_it.type, ID_SPACE_ESSENCE, eq_dt);
	if(!eq_ess || eq_dt != DT_WEAPON_ESSENCE && eq_dt != DT_ARMOR_ESSENCE && eq_dt != DT_NEW_ARMOR_ESSENCE && eq_dt != DT_DECORATION_ESSENCE && eq_dt != DT_BIBLE_ESSENCE) return;
    
    int eq_refine_material_need = 0;
    int eq_refine_addon_id = world_manager::GetDataMan().get_item_refine_addon(eq_it.type, eq_refine_material_need);
    addon_data need_inherit_addon_list[5];
    int need_inherit_addon_count = eq_it.GetCanInheritAddon(&need_inherit_addon_list[0], sizeof(need_inherit_addon_list)/sizeof(need_inherit_addon_list[0]), eq_refine_addon_id);
    
    int addon_id_list[5];
    for (unsigned char i = 0; i < need_inherit_addon_count; ++i) {
        addon_id_list[i] = need_inherit_addon_list[i].id;
    } 

    ((gplayer_dispatcher*)_runner)->equip_can_inherit_addons(equip_id, inv_idx, need_inherit_addon_count, addon_id_list);
}

void gplayer_imp::ActivateRegionWayPoints(int num, int waypoints[])
{
    if (num <= 0) return;
    CELRegion* pRegion = city_region::GetRegion(_parent->pos.x, _parent->pos.z); 
    if (pRegion == NULL) return;

    const std::unordered_map< int, abase::vector<int> >& waypoint_map = world_manager::GetRegionWaypoints();
    std::unordered_map< int, abase::vector<int> >::const_iterator it = waypoint_map.find(pRegion->GetID());
    if (it == waypoint_map.end()) return;
    const abase::vector<int>& vec = it->second;

    for (int i = 0; i < num; ++i)
    {
        for (unsigned int j = 0; j < vec.size(); ++j)
        {
            if (waypoints[i] == vec[j])
            {
                GLog::log(GLOG_INFO,"�û�%d�����˴��͵�%d" , _parent->ID.id, waypoints[i] & 0xFFFF);
                ActivateWaypoint(waypoints[i] & 0xFFFF);
                break;
            }
        }
    }
}

void gplayer_imp::PlayerReenterInstance()
{
	if(!_player_instance_reenter.TryReenter(this))
	{
		_runner->error_message(S2C::ERR_INSTANCE_REENTER_FAIL);
	}
}

void gplayer_imp::SetHasPVPLimitFilter(bool has_pvp_limit_filter)
{
    _player_sanctuary_check.SetHasPVPLimitFilter(has_pvp_limit_filter);
}

void gplayer_imp::PlayerStartEnterSanctuarySession()
{
    _player_sanctuary_check.OnStartEnterSanctuarySession();
}

void gplayer_imp::PlayerAddPVPLimitFilter()
{
    _filters.AddFilter(new pvp_limit_filter(this, FILTER_INDEX_PVPLIMIT));
}

void gplayer_imp::EnhanceMountSpeedEn(float sp)
{
	_mount_speed_en += sp;
	_petman.OnMountSpeedEnChanged(this);
}

void gplayer_imp::ImpairMountSpeedEn(float sp)
{
	_mount_speed_en -= sp;
	_petman.OnMountSpeedEnChanged(this);
}

void gplayer_imp::EnhanceFlySpeedEn(float sp)
{
	_en_point.flight_speed += sp;
	property_policy::UpdateSpeed(this);
	_runner->get_extprop_move();
}

void gplayer_imp::ImpairFlySpeedEn(float sp)
{
	_en_point.flight_speed -= sp;
	property_policy::UpdateSpeed(this);
	_runner->get_extprop_move();
}

bool gplayer_imp::IncAstrolabeExternExp(int exp)
{
#define GLORY_VIP_LEVEL 5
	if(_astrolabe_extern_level >= ASTROLABE_VIP_GRADE_MAX) return false;
 	int tmp = _astrolabe_extern_exp + exp;
 	if(tmp <= _astrolabe_extern_exp) return false;

 	_astrolabe_extern_exp = tmp;
 	do
 	{
 		int lvlup_exp = player_template::GetAstrolabeVipGradeExp(_astrolabe_extern_level);
 		if(_astrolabe_extern_exp < lvlup_exp) break;
 
 		_astrolabe_extern_exp -= lvlup_exp;
 		++_astrolabe_extern_level;

		if(_astrolabe_extern_level >= GLORY_VIP_LEVEL)
		{
			struct
			{
				int level;
				char name[MAX_USERNAME_LENGTH];
			}data;
			memset(&data,0,sizeof(data));
			data.level = _astrolabe_extern_level + 1;
			unsigned int  len = _username_len;
			if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
			memcpy(data.name, _username,len);
			
			packet_wrapper buf(sizeof(data));
			buf.push_back(&data, sizeof(data));
			broadcast_chat_msg(AT_VIP_LVL_CHAT_MSG_ID, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SYSTEM,0, 0, 0);

		}

 		if(_astrolabe_extern_level >= ASTROLABE_VIP_GRADE_MAX)
 		{
 			_astrolabe_extern_exp = 0;
 			break;
 		}
 	}while(1);
 
 	_runner->astrolabe_info_notify(_astrolabe_extern_level,_astrolabe_extern_exp);
 	return true;
 }
 
 #define ASTROLABE_OPT_DEFAULT_CHK(num)  \
 {\
 	if(_lock_inventory || OI_TestSafeLock()) return S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK;\
 	if(_equipment.IsSlotEmpty(item::EQUIP_INDEX_ASTROLABE)) return S2C::ERR_NO_EQUAL_EQUIPMENT_FAIL;\
 	if(!IsItemExist(item_inv_idx,item_id,num)) return S2C::ERR_ITEM_NOT_IN_INVENTORY;\
 }
 
int gplayer_imp::GetAstrolabeLevelMax() 
{
    return (EmulateSettings::GetInstance()->GetEmulateVersion() < 176) ? 70 : 105;
}

int gplayer_imp::GetAstralLevel(int index) 
{
    if (EmulateSettings::GetInstance()->GetEmulateVersion() < 176) {
        int values[] = {55, 56, 57, 58, 59, 60, 62, 64, 67, 70};
        return values[index];
    } else {
        int values[] = {56, 57, 58, 59, 60, 62, 64, 68, 78, 105};
        return values[index];
    }
}


int  gplayer_imp::PlayerAstrolabeSwallow(int type, int item_inv_idx, int item_id)
{
	enum 
	{
		IDX_ASTROLABE_EXP = 2014,
		MIDLE_ASTROLABE_LEVEL = 50,
	};

 	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
 	item& it_inv = _inventory[item_inv_idx];
 	item& it_eq = _equipment[equip_idx];
 	int exp = 0, count = 0;

	if(it_eq.type == 0 || it_eq.type < 0) return S2C::ERR_NO_EQUAL_EQUIPMENT_FAIL;
	if(it_inv.type == 0 || it_inv.type < 0) return S2C::ERR_INVALID_ITEM;

	int get_astrolabe_exp = it_eq.GetSwallowExp();
	int get_astrolabe_level = it_eq.NewGetLevel()+1;	
	int get_astral_level = _astrolabe_extern_level;
	int get_astral_max_level = GetAstralLevel(get_astral_level);
	int get_max_level_default = GetAstrolabeLevelMax();

	DATA_TYPE dt_exp;
	const ASTROLABE_LEVELEXP_CONFIG * conf_exp = (ASTROLABE_LEVELEXP_CONFIG*) world_manager::GetDataMan().get_data_ptr(IDX_ASTROLABE_EXP, ID_SPACE_CONFIG, dt_exp);
	if(!conf_exp || dt_exp != DT_ASTROLABE_LEVELEXP_CONFIG) return S2C::ERR_ASTROLABE_OPT_FAIL;

	bool res_error = false;
 	bool inherit = false;

	std::string ITEM1,ITEM2,ITEM3;
	
	it_eq.DumpDetail(ITEM1);
	it_inv.DumpDetail(ITEM2);

	// Com outro astrolábio
	DATA_TYPE dt1;
	const ASTROLABE_ESSENCE * newess = (ASTROLABE_ESSENCE*) world_manager::GetDataMan().get_data_ptr(it_inv.type, ID_SPACE_ESSENCE, dt1);
	if(newess && dt1 == DT_ASTROLABE_ESSENCE)
	{
		int newcount = 1;
		int newexp = it_inv.GetSwallowExp();
		int newlevel = it_inv.NewGetLevel()+1;

		int exp_finish = 0;
		int exp_next_level = 0;
		int exp_astral_level = 0;
		int next_level = 0;

		// Não dá exp com a regra do nível 50
		if(newlevel < MIDLE_ASTROLABE_LEVEL && get_astrolabe_level >= MIDLE_ASTROLABE_LEVEL) 
		{
			exp_finish = 0;
		} else // Caso contrário verifica a lógica
		{
			for (int i = 0; i < get_astral_max_level; ++i)
			{
				exp_astral_level += conf_exp->exp[i];
			}

			if(get_astrolabe_exp+newexp > exp_astral_level)
			{
				exp_finish = exp_astral_level - get_astrolabe_exp;
			} else {
				exp_finish = newexp;
			}				
		}

		if(get_astrolabe_level >= get_astral_max_level) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;

		count = newcount;

		if(count <= 0 || exp_finish <= 0 && get_astrolabe_level >= get_max_level_default) 
		{
			count++;
			exp_finish++;
		}

		if(count <= 0 || exp_finish <= 0) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;

 		if(type) inherit = true;

		if (!it_eq.InsertExp(exp_finish, 0, 0, true)) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;
 	}

	// Com itens de exp
	DATA_TYPE dt2;
	const ASTROLABE_INC_EXP_ESSENCE * newess2 = (ASTROLABE_INC_EXP_ESSENCE*) world_manager::GetDataMan().get_data_ptr(it_inv.type, ID_SPACE_ESSENCE, dt2);
	if(newess2 && dt2 == DT_ASTROLABE_INC_EXP_ESSENCE)
	{	
		int newcount = it_inv.count;
		int newexp = newess2->swallow_exp * newcount;
		int level_min_inset = newess2->require_level_min;
    	int level_max_inset = newess2->require_level_max;
		
		int new_level = get_astrolabe_level;
		int exp_finish = 0;
		int exp_next_level = 0;
		int used_items_count = 0;

		int exp_astral_level = 0;
		int exp_stone_max_required = 0;
	
		for (int i = 0; i < get_astral_max_level; ++i)
		{
			exp_astral_level += conf_exp->exp[i];
		}

		for (int i = 0; i < level_max_inset; ++i)
		{
			exp_stone_max_required += conf_exp->exp[i];
		}

		if (get_astrolabe_exp + newexp > exp_stone_max_required)
		{
			exp_finish = exp_stone_max_required - get_astrolabe_exp;
		}
		else
		{
			if (get_astrolabe_exp + newexp > exp_astral_level)
			{
				exp_finish = exp_astral_level - get_astrolabe_exp;
			}
			else
			{
				exp_finish = newexp;
			}
		}

		if (exp_finish < newexp)
		{
   			newcount = (exp_finish / newess2->swallow_exp) + (exp_finish % newess2->swallow_exp ? 1 : 0);
		}	
	
		if(get_astrolabe_level >= get_astral_max_level) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;

		count = newcount;

		if(count <= 0 || exp_finish <= 0 && get_astrolabe_level >= get_max_level_default) 
		{
			count++;
			exp_finish++;
		}

		if(count <= 0 || exp_finish <= 0) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;

		if (!it_eq.InsertExp(exp_finish, 0, 0, true)) return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;
	}
		
	// Atualiza o nível do astrolábio
 	it_eq.Deactivate(item::BODY, equip_idx, this);		

	// Adiciona a apitidão
	if(inherit)
	{
	 	if(!it_eq.Inherit(it_inv))
		{
			return S2C::ERR_ASTROLABE_SWALLOW_LIMIT;
		}
	}

 	it_eq.Activate(item::BODY, _equipment, equip_idx, this);

	it_eq.DumpDetail(ITEM3);

 	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();

	// Remove os itens
 	UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
 	_runner->player_drop_item(IL_INVENTORY, item_inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
 	_inventory.DecAmount(item_inv_idx, count);

  	return 0;
}

int  gplayer_imp::PlayerAstrolabeAddonRoll(int times, int limit, int item_inv_idx, int item_id, int (&args)[3] )
{
 	ASTROLABE_OPT_DEFAULT_CHK(times)
 	
 	DATA_TYPE dt;
 	const ASTROLABE_RANDOM_ADDON_ESSENCE * ess = (const ASTROLABE_RANDOM_ADDON_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_id,ID_SPACE_ESSENCE,dt);
 	if(ess == NULL || dt != DT_ASTROLABE_RANDOM_ADDON_ESSENCE) return S2C::ERR_INVALID_ITEM;
 
 	int count = 0;
 	int vipexp = ess->addon_random_exp_gained;
 	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
 	item& it_inv = _inventory[item_inv_idx];
 	item& it_eq = _equipment[equip_idx];
 
	std::string ITEM1,ITEM2;
	it_eq.DumpDetail(ITEM1);

	X_EQUIP id0;
 	id0.type = it_eq.type; id0.mask = it_eq.GetIdModify();
 	it_eq.Deactivate(item::BODY, equip_idx, this);
 
 	while (count < times)
 	{
 		int addon_count = player_template::GetAstrolabeAddonCount(_astrolabe_extern_level);
 		it_eq.DoRebirth(addon_count);
 		if(vipexp) IncAstrolabeExternExp(vipexp);
 		count ++;
 		if(it_eq.GetIdModify() >= limit ) 
		{
			args[2] = 1;
			break; // idmodify >> 16 ��Ϊ��������
		}
 	}
 
	args[0] = times;
	args[1] = count;

 	it_eq.Activate(item::BODY, _equipment, equip_idx, this);

	it_eq.DumpDetail(ITEM2);

 	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();
 
 	// ���Ը����漰��۸ı�
 	
 	X_EQUIP id1;
	id1.type = _equipment[equip_idx].type; id1.mask =  _equipment[equip_idx].GetIdModify();
 	if(id1.type != id0.type || id1.mask != id0.mask )
 	{
 		CalcEquipmentInfo();
 		_runner->equipment_info_changed(1ULL<<equip_idx , 0,&id1,sizeof(X_EQUIP));//�˺���ʹ����CalcEquipmentInfo�Ľ��
 	}
 
 	UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
 	_runner->player_drop_item(IL_INVENTORY, item_inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
 	_inventory.DecAmount(item_inv_idx, count);
	
 	return 0;
 }
 
int  gplayer_imp::PlayerAstrolabeAptitInc(int item_inv_idx, int item_id)
{
 	ASTROLABE_OPT_DEFAULT_CHK(1)
 	DATA_TYPE dt;
 	const ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE * ess = (const ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_id,ID_SPACE_ESSENCE,dt);
 	if(ess == NULL || dt != DT_ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE) return S2C::ERR_INVALID_ITEM;
 
 	int index = abase::RandSelect(ess->increase_probability,sizeof(ess->increase_probability)/sizeof(ess->increase_probability[0]));
 	short inc = (short)( (1+index)* ess->increase_base );
 
 	int count = 1;
 	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
 	item& it_inv = _inventory[item_inv_idx];
 	item& it_eq = _equipment[equip_idx];
 	
	std::string ITEM1,ITEM2;
	it_eq.DumpDetail(ITEM1);

	int ret = 0;
 	it_eq.Deactivate(item::BODY, equip_idx, this);
 	if(!it_eq.AddGeniusPoint(inc,ess->require_min_all_inner_point_value,ess->require_max_all_inner_point_value,0,0,false)) ret = S2C::ERR_ASTROLABE_OPT_FAIL; 
 	it_eq.Activate(item::BODY, _equipment, equip_idx, this);
 
	if(ret != 0) return ret;

	it_eq.DumpDetail(ITEM2);

 	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();
 
 	UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
 	_runner->player_drop_item(IL_INVENTORY, item_inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
	_inventory.DecAmount(item_inv_idx, count);
 
 	return 0;
 }
 
int  gplayer_imp::PlayerAstrolabeSlotRoll(int item_inv_idx, int item_id)
{
 	ASTROLABE_OPT_DEFAULT_CHK(1)
 	if(item_id != ASTROLABE_SLOT_ROLL_ITEM_1 && item_id != ASTROLABE_SLOT_ROLL_ITEM_2) return S2C::ERR_INVALID_ITEM;
 
 	int count = 1;
 	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
 	item& it_inv = _inventory[item_inv_idx];
 	item& it_eq = _equipment[equip_idx];
 	
	std::string ITEM1,ITEM2;
	it_eq.DumpDetail(ITEM1);

 	it_eq.Deactivate(item::BODY, equip_idx, this);
 	it_eq.FlushGeniusPoint();
 	it_eq.Activate(item::BODY, _equipment, equip_idx, this);
 
	it_eq.DumpDetail(ITEM2);

 	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();
 
 	UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
 	_runner->player_drop_item(IL_INVENTORY, item_inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
 	_inventory.DecAmount(item_inv_idx, count);
 	return 0;
 }

int  gplayer_imp::PlayerAstrolabeLockUnlock(char inc)
{
	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
	item& it_eq = _equipment[equip_idx];
	
	if(inc == (char)1)
	{		

		if( GetAllMoney() > ASTROLABE_COST_MONEY_LOCK)
		{
			SpendAllMoney(ASTROLABE_COST_MONEY_LOCK,true);
			SelfPlayerMoney();

			it_eq.proc_type |= item::ITEM_PROC_TYPE_NODROP | 
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_NOPUTIN_USERTRASH|
			item::ITEM_PROC_TYPE_LOCK_ASTRO;

			UpdateMallConsumptionBinding(it_eq.type, it_eq.proc_type, it_eq.count);

		} else	{		
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
		}

	} else
	{

		if( GetAllMoney() > ASTROLABE_COST_MONEY_UNLOCK)
		{
			SpendAllMoney(ASTROLABE_COST_MONEY_UNLOCK,true);
			SelfPlayerMoney();

			it_eq.proc_type |= item::ITEM_PROC_TYPE_NODROP | 
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_NOPUTIN_USERTRASH;

			it_eq.proc_type &= ~(item::ITEM_PROC_TYPE_LOCK_ASTRO);

			UpdateMallConsumptionBinding(it_eq.type, it_eq.proc_type, it_eq.count);
			
		} else
		{			
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
		}
		
	}

	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();
 	return 0;
}

// Salvar atributos (176+)
int  gplayer_imp::PlayerAstrolabeAddonSave(short * addon_pos)
{
 	int equip_idx = item::EQUIP_INDEX_ASTROLABE;
 	item& it_eq = _equipment[equip_idx];
 
	std::string ITEM1,ITEM2;
	it_eq.DumpDetail(ITEM1);

	// Desativa o astrolábio
	X_EQUIP id0;
 	id0.type = it_eq.type; id0.mask = it_eq.GetIdModify();
 	it_eq.Deactivate(item::BODY, equip_idx, this);
	
	// Adiciona os novos atributos
	if(!it_eq.ForceAddonsGenerate(addon_pos)) return 0;
 
	// Ativa o astrolábio
 	it_eq.Activate(item::BODY, _equipment, equip_idx, this);
	it_eq.DumpDetail(ITEM2);

 	// Atualiza o astrolábio
 	PlayerGetItemInfo(IL_EQUIPMENT, equip_idx);
 	RefreshEquipment();
 
 	X_EQUIP id1;
	id1.type = _equipment[equip_idx].type; id1.mask =  _equipment[equip_idx].GetIdModify();
 	if(id1.type != id0.type || id1.mask != id0.mask )
 	{
 		CalcEquipmentInfo();
 		_runner->equipment_info_changed(1ULL<<equip_idx , 0,&id1,sizeof(X_EQUIP));
 	}
 
 	return 0;
}

instance_hash_key gplayer_imp::GetLogoutInstanceKey()
{
	return world_manager::GetInstance()->GetLogoutInstanceKey(this);
}

int gplayer_imp::PlayerSoloChallengeUserSelectAward(int stage_level, int args[])
{
	return _solochallenge.UserSelectAward(this, stage_level, args);
}

int gplayer_imp::PlayerSoloChallengeScoreCost(int filter_index, int args[])
{
	return _solochallenge.ScoreCost(this, filter_index, args);
}

int gplayer_imp::PlayerSoloChallengeClearFilter(int args[])
{
	return _solochallenge.ClearFilter(this, args);
}

void gplayer_imp::PlayerSoloChallengeSelectStage(int stage_level)
{
	_solochallenge.SelectStage(this, stage_level);
}

void gplayer_imp::PlayerSoloChallengeStageComplete(bool isCompleteSuccess)
{
	_solochallenge.StageComplete(this, isCompleteSuccess);
}

void gplayer_imp::PlayerSoloChallengeStartTask(bool isStartSuccess)
{
	_solochallenge.StageStart(this, isStartSuccess);
}

void gplayer_imp::PlayerEnterSoloChallengeInstance()
{
	_solochallenge.PlayerEnterSoloChallengeInstance(this);
}

void gplayer_imp::PlayerLeaveSoloChallengeInstance()
{
	_solochallenge.PlayerLeaveSoloChallengeInstance(this);
}

void gplayer_imp::PlayerDeliverSoloChallengeScore(int score)
{
	_solochallenge.PlayerDeliverSoloChallengeScore(this, score);
}

int gplayer_imp::PlayerSoloChallengeLeaveTheRoom()
{
	return _solochallenge.PlayerSoloChallengeLeaveTheRoom(this);
}

bool gplayer_imp::CheckVisaValid()
{
	if(_player_visa_info.stay_timestamp < g_timer.get_systime()) return false;
	if(_player_visa_info.cost && _player_visa_info.count)
	{
		if(!CheckItemExist(_player_visa_info.cost,_player_visa_info.count)) return false;

		if(OI_TestSafeLock()||_lock_inventory) return true; // �����ٿ۳�

		PlayerTaskInterface TaskIf(this);
		TaskIf.TakeAwayCommonItem(_player_visa_info.cost,_player_visa_info.count);
		_player_visa_info.cost = _player_visa_info.count = 0;

	}
	return true;
}

void gplayer_imp::Repatriate()
{
	PlayerTryChangeDS(GMSV::CHDS_FLAG_CENTRALDS_TO_DS);
	GLog::log(LOG_INFO,"�û�%d�����ķ�ǲ��",_parent->ID.id);
}

int 
gplayer_imp::MnfactionJoinApply(int domain_id)
{
	if(!CheckCoolDown(COOLDOWN_INDEX_MNFACTION_JOIN_APPLY)) return S2C::ERR_OBJECT_IS_COOLING;
	int64_t unifid = _player_mnfaction_info.unifid;
	if(!unifid) return S2C::ERR_NOT_IN_FACTION;
		
		//���˱���ģʽ
	SetCoolDown(COOLDOWN_INDEX_MNFACTION_JOIN_APPLY,MNFACTION_JOIN_APPLY_COOLDOWN_TIME);
		
	GMSV::MnfactionEnterEntry entry;
	entry.roleid     = _parent->ID.id;
	entry.faction_id = unifid;
	entry.domain_id  = domain_id;
	GMSV::MnfactionEnter(&entry);
	return 0;
}

bool 
gplayer_imp::MnfactionJoinStep1(int retcode, int64_t faction_id, int domain_id, int world_tag)
{
	enum
	{
		ERR_MNF_MULTI_DOMAIN = 603,             //һ����ɫֻ�ܽ���һ��ս��
		ERR_MNF_INVITE_COUNT_PERDOMAIN_MAXMUM = 604,    //����ս���Ľ�ɫ̫��
		ERR_MNF_FORBID_ENTER = 639,           //�����ɲ���������
	};
	if(retcode != S2C::ERR_SUCCESS)
	{
		if(retcode == ERR_MNF_MULTI_DOMAIN)
			_runner->error_message(S2C::ERR_MNFACTION_MULTI_DOMAIN);
		else if(retcode == ERR_MNF_INVITE_COUNT_PERDOMAIN_MAXMUM)
			_runner->error_message(S2C::ERR_MNFACTION_INVITE_COUNT_PERDOMAIN_MAXMUM);
		else
			_runner->error_message(S2C::ERR_MNFACTION_FORBID_ENTER);
		return false;
	}
	int64_t unifid = _player_mnfaction_info.unifid;
	if(!unifid || unifid != faction_id)
		return false;
	if(_player_state != PLAYER_STATE_NORMAL 
			&& _player_state != PLAYER_STATE_BIND
			&& _player_state != PLAYER_SIT_DOWN
			&& _player_state != PLAYER_STATE_MARKET)
	{
		return false;
	}
	LeaveAbnormalState();

	if(world_tag == world_manager::GetWorldTag()) return false;
	
	instance_key key;
	memset(&key,0,sizeof(key));
	GetInstanceKey(world_tag, key);
	key.target = key.essence;

	key.target.key_level4 = domain_id;

	ClearSwitchAdditionalData();
	A3DVECTOR pos(0,0,0);
	if(world_manager::GetInstance()->PlaneSwitch(this ,pos,world_tag,key,0) < 0)
	{
		return false;
	}
	
	_switch_additional_data = new mnfaction_switch_data(faction_id, domain_id);

	GLog::log(GLOG_INFO,"���%d����������սstep1(domain_id=%d)׼������(tag=%d,pos=%f,%f,%f)", _parent->ID.id, domain_id, world_tag, pos.x, pos.y, pos.z);
	return true;
}

bool 
gplayer_imp::MnfactionJoinStep2()
{
	mnfaction_switch_data * pData = substance::DynamicCast<mnfaction_switch_data>(_switch_additional_data);
	ASSERT(pData);
	
	GMSV::MNDomainBattleEnterSuccessNotice(_parent->ID.id, pData->_faction_id, pData->_domain_id);
	GLog::log(GLOG_INFO,"���%d��������սstep2(domain_id=%d)�ɹ�", _parent->ID.id, pData->_domain_id);

	ClearSwitchAdditionalData();
	return true;
}

int
gplayer_imp::MnfactionLeave()
{
	if(world_manager::GetInstance()->GetWorldType() != WORLD_TYPE_MNFACTION)
		return 1;
	_filters.ModifyFilter(FILTER_CHECK_INSTANCE_KEY,FMID_CLEAR_AEMF,NULL,0);
	return 0;
}

int gplayer_imp::UseFireWorks2(char is_friend_list, int target_role_id, int item_type, const char * target_user_name)
{
	if(target_role_id)
	{
		DATA_TYPE dt;
		FIREWORKS2_ESSENCE * ess = (FIREWORKS2_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)item_type, ID_SPACE_ESSENCE,dt);
		if( !ess || dt != DT_FIREWORKS2_ESSENCE) return -1;
	
		A3DVECTOR to_target_direction = _direction;
		if(target_role_id != _parent->ID.id)//���̻�ʩ��Ŀ�겻���Լ�ʱ,������˾���С��10��,�����һ�¿�ķ���ʹ�������ָ��ʩ����
		{
			int index;
			gplayer * pPlayer = world_manager::GetInstance()->FindPlayer(target_role_id,index);
			if(pPlayer) 
			{
				if(pPlayer->pos.squared_distance(_parent->pos) < 10.f * 10.f)
				{
					to_target_direction = pPlayer->pos;
					to_target_direction -= _parent->pos;
				}
			}
		}
	
		//�ı��̻��ĳ���,ʹ���η���ָ��ʩ����
		unsigned char mine_dir;
		mine_dir = a3dvector_to_dir(to_target_direction);//��ָ��ʩ���ߵ�����ת��Ϊ��x��ļн�
		unsigned char num = 255;//a3dvector_to_dir ������ת������0-255֮�����
		mine_dir = num - mine_dir;//ȡ��ǰ�ǶȵĲ���,����Ϊ�˽����εļ�ָ��ʩ����,ת������Ϊ������Դģ�Ͱ�͹����Ҫ�ķ����Ƿ���
		unsigned int tmp = mine_dir + 192;//�������ɿ�ʱ����z��������ļнǼ����,ƫ��-90��, -255/4 +255/4*3 ~~ 192
		if (tmp > 255) tmp -= 255;//����255��,������360�Ⱥ󣬼�ȥ360��
		mine_dir = tmp;

		_direction = to_target_direction;//ǿ�иı��Լ��ĳ���,ʹ�Լ�����ʩ����
		_parent->dir = a3dvector_to_dir(to_target_direction);
		_runner->stop_move(_parent->pos,0x1000,_parent->dir,0x01);//�㲥,�ı�������ҿ��Լ��ĳ���,�Լ��ĳ���ͻ����޸�

		A3DVECTOR offset;	
		float magnitude = to_target_direction.x * to_target_direction.x + to_target_direction.y * to_target_direction.y + to_target_direction.z * to_target_direction.z;
		if(magnitude < 0.00001f)
			offset = A3DVECTOR(0.f, 0.f, 0.f);
		else
		{
			offset = to_target_direction;
			offset.normalize();
		}

		A3DVECTOR mine_pos = _parent->pos;
		mine_pos   += offset;
		object_interface::mine_param param;
		if(IsPlayerFemale())
			param.mine_id     = ess->female_mine_id;
		else
			param.mine_id     = ess->male_mine_id; 
		param.remain_time = ess->time_to_fire; 
		object_interface oi(this);
		XID target(GM_TYPE_PLAYER, target_role_id);
		int ret = oi.CreateMine(mine_pos, param, mine_dir, target);
		if(ret != 0) 
			return -1;
	}
	
 	packet_wrapper buf;
	char user_name[MAX_USERNAME_LENGTH];
	memset(user_name, 0, MAX_USERNAME_LENGTH);
	unsigned int  len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(user_name, _username,len);
	if(target_role_id)
	{
		buf << item_type;
		buf.push_back(user_name, MAX_USERNAME_LENGTH);
		buf << _parent->ID.id;
		buf.push_back(target_user_name, MAX_USERNAME_LENGTH);
		
		broadcast_chat_msg(FIREWORK2_PRIVATE_CHAT_MSG_ID, 0, 0, GMSV::CHAT_CHANNEL_SYSTEM,0, buf.data(), buf.size());
	}
	else
	{
		buf << item_type;
		buf.push_back(user_name, MAX_USERNAME_LENGTH);
		buf << _parent->ID.id;
		buf.push_back(user_name, MAX_USERNAME_LENGTH);
	}
	broadcast_chat_msg(FIREWORK2_PUBLIC_CHAT_MSG_ID, 0, 0, GMSV::CHAT_CHANNEL_SYSTEM,0, buf.data(), buf.size());
	return 1;
}

void gplayer_imp::PlayerFixPositionTransmitAdd(float *pos, const char* position_name)
{
	int world_tag = world_manager::GetWorldTag();
	if(!world_manager::GetWorldLimit().permit_fix_position_transmit)
	{
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_CANNOT_ADD_IN_THIS_WORLDTAG);
		return;
	}
	int max_num = player_template::GetCashVipFixPositionNum(GetCashVipLevel());
	if(GetFixPositionCount() >= max_num)
	{
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_MAX_NUM);
		return;
	}
	int i = 0;
	for(; i < FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT; ++i)
	{
		if(_fix_position_transmit_infos[i].index == -1)
		{
			_fix_position_transmit_infos[i].index = i;
			_fix_position_transmit_infos[i].world_tag = world_tag;
			_fix_position_transmit_infos[i].pos = _parent->pos;
			memcpy(_fix_position_transmit_infos[i].position_name, position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
			break;
		}
	}
	if(i == FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT)
	{
		return;
	}
	fix_position_transmit_info &fpti = _fix_position_transmit_infos[i];
	_runner->fix_position_transmit_add_position(fpti.index, fpti.world_tag, fpti.pos, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH, position_name);
}
void gplayer_imp::PlayerFixPositionTransmitDelete(int index)
{
	if(index < 0 || index >= FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT || _fix_position_transmit_infos[index].index == -1)
	{
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_CANNOT_FIND);
		return;
	}
	_fix_position_transmit_infos[index].Reset();
	_runner->fix_position_transmit_delete_position(index);
}
void gplayer_imp::PlayerFixPositionTransmit(int index)
{
	if(GetFixPositionTransmitEnergy() < 1)
	{
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_ENERGY_NOT_ENOUGH);
		return;
	}
	
	if(index >= FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT || index < 0 || _fix_position_transmit_infos[index].index == -1)
	{	
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_CANNOT_FIND);
		return;
	}

	ReduceFixPositionTransmitEnergy(1);
	LongJump(_fix_position_transmit_infos[index].pos, _fix_position_transmit_infos[index].world_tag);
}
void gplayer_imp::PlayerFixPositionTransmitRename(int index, char *position_name)
{
	if(index >= FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT  || index < 0 || _fix_position_transmit_infos[index].index == -1)
	{	
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_CANNOT_FIND);
		return;
	}
	fix_position_transmit_info &info = _fix_position_transmit_infos[index];

	memset(info.position_name, 0, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
	memcpy(info.position_name, position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);

	_runner->fix_position_transmit_rename(info.index, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH, info.position_name);
}

void gplayer_imp::SetDBFixPositionTransmit(archive & ar)
{
	if(0 == ar.size())
		return;
	ar >> _fix_position_transmit_energy;
	unsigned int size;
	ar >> size;
	for(unsigned int i = 0; i < size; ++i)
	{
		int index = 0;
		ar >> index;
		fix_position_transmit_info &info = _fix_position_transmit_infos[index];
		info.index = index;
		ar >> info.world_tag >> info.pos.x >> info.pos.y >> info.pos.z;
		ar.pop_back(info.position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
		
	}
}

void gplayer_imp::GetDBFixPositionTransmit(archive & ar)
{
	ar << _fix_position_transmit_energy;
	unsigned int size = GetFixPositionCount();
	ar << size;
	for(unsigned int i = 0; i < FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT; ++i)
	{
		fix_position_transmit_info &info = _fix_position_transmit_infos[i];
		if(info.index == -1)
			continue;
		ar << info.index << info.world_tag << info.pos.x << info.pos.y << info.pos.z;
		ar.push_back(info.position_name, FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH);
	}
}

bool gplayer_imp::PlayerGetCashVipMallItemPrice(int start_index, int end_index)	//lgc����������Ϊ0, ���ʾɨ��������,
{																			//����[start_index,end_index)�ڵ���Ʒ��ɨ��
	netgame::mall & __mall = world_manager::GetPlayerMall3();	
	unsigned int __mall_goods_count = __mall.GetGoodsCount();
	int __group_id = __mall.GetGroupId();	//��ǰ�������趨��group_id
	
	if(start_index == 0 && end_index == 0)	//ɨ��ȫ��
	{
		start_index = 0;
		end_index = __mall_goods_count;
	}
	else
	{
		if(start_index < 0 || end_index <= 0 || (unsigned int)start_index >= __mall_goods_count || (unsigned int)end_index > __mall_goods_count || start_index >= end_index)
		{
			_runner->error_message(S2C::ERR_FATAL_ERR);
			return false;
		}
	}
	//ֻ��ɨ�跶Χ����ָ��ֵʱ��������ȴ
	if(end_index-start_index > 10 && !CheckCoolDown(COOLDOWM_INDEX_GET_DIVIDEND_MALL_PRICE)) return false;
	SetCoolDown(COOLDOWM_INDEX_GET_DIVIDEND_MALL_PRICE,GET_CASH_VIP_MALL_PRICE_COOLDOWN_TIME);
	
	//���ܷ����仯����Ʒ�б�	
	abase::vector<netgame::mall::index_node_t, abase::fast_alloc<> > & __limit_goods = __mall.GetLimitGoods();
	unsigned int __limit_goods_count = __limit_goods.size();
	
	time_t __time = time(NULL);
	packet_wrapper __h0(1024);
	int	__h0_count = 0; 
    using namespace S2C;
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	for(unsigned int i=0; i<__limit_goods_count; i++)
	{
		int index = __limit_goods[i]._index;
		if(index < start_index)
			continue;
		if(index >= end_index)
			break;
		netgame::mall::node_t & node = __limit_goods[i]._node;
			
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;		
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}
	
		if(!node.sale_time_active)		//ֻ�����õ�����ʱ��
		{
			//����Ч�ķ�Ĭ��group������	�����ͻ���
			ASSERT(node.group_active);
			if(active_group_id != 0)
			{
				for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
				{
					if(node.entry[j].cash_need <= 0)
						break;
					if(active_group_id == node.entry[j].group_id)	//�����������ҵ�һ��
					{
						CMD::Make<CMD::cash_vip_mall_item_price>::AddGoods(__h0, 
														index, 
														j, 
														node.goods_id, 
														node.entry[j].expire_type, 
														node.entry[j].expire_time, 
														node.entry[j].cash_need, 
														node.entry[j].status,
														node.entry[j].min_vip_level);
						__h0_count ++;
					}
				}
			}
		}
		else							//���ڷ����õ�����ʱ��
		{
			int available_permanent = -1;				//��Ч����������ʱ���slot������һ��
			int available_saletime = -1;				//��Ч�ķ���������ʱ���slot������һ��
			for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
			{
				if(node.entry[j].cash_need <= 0)
					break;
				if(node.entry[j].group_id == active_group_id && node.entry[j]._sale_time.CheckAvailable(__time))
				{
					if(node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT)
					{
						available_saletime = j;
						break;		///ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
					}
					else
						available_permanent = j;	//���౻ִ��һ��	
				}			
			}
			if(available_saletime >= 0)		//ͬ����ֻ����һ������������ʱ�����㣬����ȷ������Ʒ��ǰ�ļ۸�
			{
				CMD::Make<CMD::cash_vip_mall_item_price>::AddGoods(__h0, 
														index, 
														available_saletime, 
														node.goods_id, 
														node.entry[available_saletime].expire_type, 
														node.entry[available_saletime].expire_time, 
														node.entry[available_saletime].cash_need, 
														node.entry[available_saletime].status,
														node.entry[available_saletime].min_vip_level);
				__h0_count ++;
			}
			else if(available_permanent >= 0 )	//ͬ��������������ʱ������
			{
				if(active_group_id != 0)	//ֻҪ��Ĭ��group�����ݲŻᷢ���ͻ���	
				{
					CMD::Make<CMD::cash_vip_mall_item_price>::AddGoods(__h0, 
																index, 
																available_permanent, 
																node.goods_id, 
																node.entry[available_permanent].expire_type, 
																node.entry[available_permanent].expire_time, 
																node.entry[available_permanent].cash_need, 
																node.entry[available_permanent].status,
																node.entry[available_permanent].min_vip_level);
					__h0_count ++;
				}
			}
			else		//��Ʒ�¼�,ֻ���ڿͻ��˵�gshop.data�д������õĹ���ʽʱ����Ҫ������Ʒ�¼�
			{
				if(active_group_id != 0)	//����0�������ǰ���Ѿ��жϿͻ��˿϶�û�����õĹ���ʽ
				{
					int m;
					for(int m=0; m<netgame::mall::MAX_ENTRY; m++)
					{
						if(node.entry[m].cash_need > 0 && node.entry[m].group_id == 0 
							&& node.entry[m]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
							break;
					}
					if(m < netgame::mall::MAX_ENTRY)
					{
						CMD::Make<CMD::cash_vip_mall_item_price>::AddGoods(__h0, 
																index, 
																0, 
																node.goods_id, 
																0, 
																0, 
																0, 
																0,
																0);
						__h0_count ++;
					}
				}
			}
		}//end of if(!sale_time_active)
	}
	
	packet_wrapper __h1(1024);
	CMD::Make<CMD::cash_vip_mall_item_price>::From(__h1, start_index, end_index, __h0_count);
	if(__h0_count > 0)
		__h1.push_back(__h0.data(), __h0.size());
	gplayer * pPlayer = (gplayer*)_parent;
	send_ls_msg(pPlayer,__h1);
	
	return true;
}

bool
gplayer_imp::PlayerDoCashVipShopping(unsigned int goods_count,const int * order_list, int shop_tid)
{
	//������Ҫ���Ӹ�����Թ����״̬
	if(_player_state != PLAYER_SIT_DOWN && _player_state != PLAYER_STATE_NORMAL && _player_state != PLAYER_STATE_BIND) return false;

	if(!CheckVipService(CVS_SHOPPING))
		return false;

	if(goods_count == 0)
	{
		return false;
	}
	if(goods_count > _inventory.Size() || !InventoryHasSlot(goods_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	int gifts_count = 0;
	
	netgame::mall & shop = world_manager::GetPlayerMall3();
	int __group_id = shop.GetGroupId();	//��ǰ�������趨��group_id,lgc
	time_t __time = time(NULL);			//
	netgame::mall_order  order(-1);

	std::map<int, int> item_limit_type_map; // item_id -> limit_type
	
	ASSERT(netgame::mall::MAX_ENTRY == 4);
	unsigned int offset = 0;
	for(unsigned int i = 0; i < goods_count; i ++, offset += sizeof(C2S::CMD::cash_vip_mall_shopping::__entry) / sizeof(int))
	{
		int mall_rate = order_list[offset];
		int id = order_list[offset+1];
		unsigned int index = order_list[offset +2];
		unsigned int slot = order_list[offset +3];
		if(slot >= netgame::mall::MAX_ENTRY)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}
		netgame::mall::node_t node;
		if(!shop.QueryGoods(index,node) || node.goods_id != id)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(!node.check_owner(shop_tid))
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		if(node.entry[slot].cash_need <= 0)
		{
			_runner->error_message(S2C::ERR_GSHOP_INVALID_REQUEST);
			return true;
		}

		// MALL RATE 172 BEGIN
		if (mall_rate <= 0) mall_rate = 1;
		if (mall_rate >= 1000) mall_rate = 1000;
		
		if (node.goods_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.goods_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.goods_count = new_count;
			node.entry[slot].cash_need *= mall_rate;
		}
		
		if (node.gift_id > 0)
		{
			int new_count = node.goods_count * mall_rate;
			int max_limit = world_manager::GetDataMan().get_item_pile_limit(node.gift_id);
			if (new_count >= max_limit) new_count = max_limit;
			if (new_count <= 0) new_count = 1;
			
			node.gift_count = new_count;
		}
		// MALL RATE 172 END

		if(!_purchase_limit_info.CheckShoppingLimitItem(id, node.buy_times_limit, node.buy_times_limit_mode, node.goods_count))
		{
			_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 1);
			return true;
		}

		if(GetCashVipLevel() < node.entry[slot].min_vip_level)
		{
			_runner->error_message(S2C::ERR_CASH_VIP_LIMIT);
			return true;
		}
		
		//lgc	
		//�ҵ���ǰ��Ч��group
		int active_group_id = 0;
		if(node.group_active && __group_id != 0)
		{
			if(__group_id == node.entry[0].group_id || __group_id == node.entry[1].group_id || __group_id == node.entry[2].group_id || __group_id == node.entry[3].group_id)
				active_group_id = __group_id;	
		}

		if(node.sale_time_active)
		{	
			if(node.entry[slot].group_id == active_group_id && node.entry[slot]._sale_time.CheckAvailable(__time))	
			{
				//���playerѡ���slot�����õ����۷�ʽ������Ҫɨ�赱ǰ��Ч���ڣ����Ƿ񻹴��ڷ��������۷�ʽ�����
				if(node.entry[slot]._sale_time.GetType() == netgame::mall::sale_time::TYPE_NOLIMIT)
				{
					for(int j=0; j<netgame::mall::MAX_ENTRY; j++)
					{
						if(node.entry[j].cash_need <= 0)
							break;
						if(node.entry[j].group_id == active_group_id 
									&& node.entry[j]._sale_time.GetType() != netgame::mall::sale_time::TYPE_NOLIMIT
									&& node.entry[j]._sale_time.CheckAvailable(__time))
						{
							_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 0);
							return false;
						}
					}
				}				
			}
			else
			{
				_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 0);
				return false;
			}
		}
		else if(node.entry[slot].group_id != active_group_id)
		{
			_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_FAILED, index, 0);
			return false;
		}

		if(node.gift_id > 0) gifts_count ++;  //ͳ����Ʒ��
		
		order.AddGoods(node.goods_id, node.goods_count, node.entry[slot].cash_need, node.entry[slot].expire_time,node.entry[slot].expire_type,node.gift_id,node.gift_count,node.gift_expire_time,node.gift_log_price);
		
		if(node.buy_times_limit_mode)
			item_limit_type_map[node.goods_id] = node.buy_times_limit_mode;
	}
	if(GetCashVipScore() < order.GetPointRequire())
	{
		//no engouh cash vip score
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return true;
	}
	if(!InventoryHasSlot(goods_count + gifts_count))
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int total_cash = GetCashVipScore();
	int cash_used = 0;
	//��Ǯ�㹻�� ��ʼ������Ʒ
	int cur_t = g_timer.get_systime();
	int self_id = GetParent()->ID.id;
	for(unsigned int i = 0; i < goods_count; i ++)
	{
		int id;
		int count;
		int point;
		int expire_time;
		int expire_type;
		int gift_id;
		int gift_count;
		int gift_expire_time;
		int gift_log_price;
		bool bRst = order.GetGoods(i, id, count,point, expire_time,expire_type,gift_id,gift_count,gift_expire_time,gift_log_price);
		if(bRst)
		{
			//������Ʒ����Ʒ��log�۸�
			int log_price1 = point;
			int log_price2 = 0;
			if(gift_id > 0 && gift_log_price > 0)
			{
				log_price1 = int((float)point*point/(point+gift_log_price));
				log_price2 = point - log_price1; 
			}

			const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(id);
			if(pItem)
			{
				item_data * pItem2 = DupeItem(*pItem);
				int expire_date = 0;
				if(expire_time) 
				{
					if(expire_type == netgame::mall::EXPIRE_TYPE_TIME)
					{
						//��Ч������һ������
						expire_date = cur_t + expire_time;
					}
					else
					{
						//��Ч���ǹ涨����ʧЧ
						expire_date = expire_time;
					}
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pItem2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pItem2->type, guid1,guid2);
					pItem2->guid.guid1 = guid1;
					pItem2->guid.guid2 = guid2;
				}

				int ocount = count;
				int rst =_inventory.Push(*pItem2,count,expire_date);
				ASSERT(rst >= 0 && count == 0);
				_runner->obtain_item(id,pItem2->expire_date,ocount,_inventory[rst].count, 0,rst);

				if(item_limit_type_map.find(id) != item_limit_type_map.end())
				{
					int have_purchase_count = _purchase_limit_info.AddShoppingLimit(id, item_limit_type_map[id], ocount);
					_runner->purchase_limit_info_notify(item_limit_type_map[id], id, have_purchase_count);
				}
				
				//�������³�ʼ��һ�¿��ܵ�ʱװ
				_inventory[rst].InitFromShop();

				total_cash -= log_price1;
				cash_used += log_price1;
				//��¼��־  
				GLog::formatlog("formatlog:gcashvipshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%dcashvipscore_need=%d:cashvipscore_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,id, expire_date,ocount,log_price1,total_cash,guid1,guid2);
				
				world_manager::TestCashItemGenerated(id, ocount);
				FreeItem(pItem2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���CASH_VIP�̳���Ʒ%dʱ������Ʒʧ��",self_id, id);
			}

			//����Ϊ������Ʒ
			if(gift_id > 0)
			{
			const item_data * pGift = (const item_data*)world_manager::GetDataMan().get_item_for_sell(gift_id);
			if(pGift)
			{
				item_data * pGift2 = DupeItem(*pGift);
				int expire_date = 0;
				if(gift_expire_time) 
				{
					//��Ч������һ������
					expire_date = cur_t + gift_expire_time;
				}
				int guid1 = 0;
				int guid2 = 0;
				if(pGift2->guid.guid1 != 0)
				{
					//void get_item_guid(int id, int & g1, int &g2);
					//�����Ҫ������GUID
					get_item_guid(pGift2->type, guid1,guid2);
					pGift2->guid.guid1 = guid1;
					pGift2->guid.guid2 = guid2;
				}

				int ocount = gift_count;
				int rst =_inventory.Push(*pGift2,gift_count,expire_date);
				ASSERT(rst >= 0 && gift_count == 0);
				_runner->obtain_item(gift_id,expire_date,ocount,_inventory[rst].count, 0,rst);

				//�������³�ʼ��һ�¿��ܵ�ʱװ
                _inventory[rst].InitFromShop();
				
				total_cash -= log_price2;
				cash_used += log_price2;
				//��¼��־  
				GLog::formatlog("formatlog:gcashvipshop_trade:userid=%d:db_magic_number=%d:item_id=%d:expire=%d:item_count=%d:cashvipscore_need=%d:cashvipscore_left=%d:guid1=%d:guid2=%d",
						self_id,_db_user_id,gift_id, expire_date,ocount,log_price2,total_cash,guid1,guid2);
				
				world_manager::TestCashItemGenerated(gift_id, ocount);
				FreeItem(pGift2);
			}
			else
			{
				//��¼������־
				GLog::log(GLOG_ERR,"�û�%d�ڹ���CASH_VIP�̳���Ʒ%dʱ������Ʒ%dʧ��",self_id, id, gift_id);
			}
			}
		}
		else
		{
			ASSERT(false);
		}
	}

	bool rst = _cash_vip_info.SpendCashVipScore(cash_used, (gplayer *)_parent);
	ASSERT(rst);

	_runner->cash_vip_mall_item_buy_result(CASH_VIP_BUY_SUCCESS, 0, 0);
	
	GLog::log(GLOG_INFO,"�û�%d��CASH_VIP�̳ǹ���%d����Ʒ������%d��ʣ��%d��",self_id,goods_count,cash_used,GetCashVipScore());

	//���Ǽӿ�����ٶ� 
	return true;
}

int gplayer_imp::AddFixPositionEnergy(int item_type)
{
	DATA_TYPE dt;
	FIX_POSITION_TRANSMIT_ESSENCE * ess = (FIX_POSITION_TRANSMIT_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)item_type, ID_SPACE_ESSENCE,dt);
	if( !ess || dt != DT_FIX_POSITION_TRANSMIT_ESSENCE) return -1;
	int energy = ess->energy;
	if(energy < 0)
		return -1;
	AddFixPositionTransmitEnergy(energy);
	return 1;
}

bool gplayer_imp::CheckVipService(int type)
{
	if(!LuaManager::GetInstance()->GetConfig()->vip_enable)
		return false;
	
	if(world_manager::GetGlobalController().CheckServerForbid(SERVER_FORBID_SERVICE,type))	
		return false;

	int viplevel = _cash_vip_info.GetCalcVipLevel();

	switch(type)
	{
		case CVS_SHOPPING:
			return true;
		case CVS_RESURRECT:
			return viplevel >= CASH_RESURRECT_VIP_LEVEL_LIMIT;
		case CVS_PICKALL:
			return viplevel >= 3;
		case CVS_FIX_POSITION:
			return viplevel >= MIN_FIX_POSITION_TRANSMIT_VIP_LEVEL;
		case CVS_ENEMYLIST:
			return viplevel >= ENEMY_VIP_LEVEL_LIMIT;
		case CVS_ONLINEAWARD:
			return viplevel >= 2;
		case CVS_REPAIR:
			return viplevel >= MIN_REMOTE_ALL_REPAIR_VIP_LEVEL;
	}

	return false;
}

//---------------------------------------------------------------------------------------------------------------------------
//--162 new functions
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::S2CGameDataSend(void * buf, unsigned int size)
{
	send_ls_msg( GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid, buf, size );
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::S2CGameDataSendElse(void * buf, unsigned int size)
{
	packet_wrapper _tbuf;
	_tbuf.clear();
	_tbuf.push_back(buf, size);
	AutoBroadcastCSMsg(_plane,GetParent()->pPiece,_tbuf,GetParent()->ID.id);
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::ShieldEnergyHeartBeat()
{
	if ( GetPlayerClass() == USER_CLASS_PALADIN && _shield_energy_gen)
	{
		float _shield_energy_new = _shield_energy;
		
		if (_shield_energy_gen) 
			_shield_energy_new += _shield_energy_gen;
		if (_shield_energy_new < 0.0f) 
			_shield_energy_new = 0.0f;
		if (_shield_energy_new > _shield_energy_max) 
			_shield_energy_new = _shield_energy_max;
		
		if (_shield_energy_new != _shield_energy)
		{
			_shield_energy = _shield_energy_new;
			_runner->get_shield_energy(_shield_energy, _shield_energy_max);
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------------
bool gplayer_imp::ShieldEnergyUseSkill(float cost)
{
	if (GetPlayerClass() == USER_CLASS_PALADIN && _shield_energy >= cost)
	{
		_shield_energy -= cost;
		_runner->get_shield_energy(_shield_energy, _shield_energy_max);
		return true;
	}
	return false;
}

bool gplayer_imp::EnhanceShieldEnergy(float cost)
{
	if (GetPlayerClass() == USER_CLASS_PALADIN && _shield_energy < _shield_energy_max)
	{
		_shield_energy += cost;

		if(_shield_energy > _shield_energy_max)
			_shield_energy = _shield_energy_max;

		_runner->get_shield_energy(_shield_energy, _shield_energy_max);
		return true;
	}
	return false;
}

bool gplayer_imp::ImpairShieldEnergy(float cost)
{
	if (GetPlayerClass() == USER_CLASS_PALADIN && _shield_energy >= cost)
	{
		_shield_energy -= cost;

		if(_shield_energy < 0)
			_shield_energy = 0;

		_runner->get_shield_energy(_shield_energy, _shield_energy_max);
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::EnablePVPFlag(char type)
{
	if(!_pvp_enable_flag)
	{
		GetParent()->object_state |= gactive_object::STATE_PVPMODE;
		_pvp_enable_flag = true;
		_pvp_cooldown = LuaManager::GetInstance()->GetConfig()->pvp_cooldown_time;
		_runner->enable_pvp_state(type);
		_runner->player_pvp_cooldown(_pvp_cooldown);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::DisablePVPFlag(char type)
{
	if(_pvp_enable_flag)
	{
		GetParent()->object_state &= ~gactive_object::STATE_PVPMODE;
		_pvp_enable_flag = false;
		_pvp_cooldown = 0;
		_runner->disable_pvp_state(type);
		_runner->player_pvp_cooldown(_pvp_cooldown);
	}
}
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::CheckNPCSopDistance(int id_item_list,const void *buf, unsigned int size)
{
	struct BAG
	{
		int item_type;
		unsigned int index;
		unsigned int count;
	};
	
	struct REQUEST
	{
		unsigned int money;
		int consume_contrib;
		int cumulate_contrib;
		int force_id;
		int force_repu;
		int force_contrib;
		unsigned int money_silver; //173
		unsigned int money_guild;  //173
		unsigned int count;
		BAG item_list[];
		
		void AdjustVendorFee(int & fee)
		{
			if(fee < 100) return ;
			if(fee < 1000)
			{
				int r = fee % 10;
				if(r) fee += 10 - r;
			}
			else //if(fee < 10000)
			{
				int r = fee % 100;
				if(r) fee += 100 - r;
			}
		}
	};
	
	if(!buf || size < sizeof(REQUEST)) return 0;
	DATA_TYPE dt;
	NPC_SELL_SERVICE *item_list = (NPC_SELL_SERVICE*)world_manager::GetDataMan().get_data_ptr(id_item_list,ID_SPACE_ESSENCE,dt);
	if (!item_list && dt != DT_NPC_SELL_SERVICE ) return 0;
	
	REQUEST * req= (REQUEST*)buf;
	if (req->force_id == -700)
	{
		req->force_id = 0;
		return 0;
	}
	if (
		req->consume_contrib ||
		req->cumulate_contrib ||
		req->force_id ||
		req->force_repu ||
		req->force_contrib
	) return 0;

	unsigned int item_id, item_count, item_price, i, j, money_res = 0, res = 0;
	float money = 0.0f, max_money = 2000000000.0f;

	for(i = 0; i < req->count && i < 12; i++)
	{
		item_count = req->item_list[i].count;
		item_id  =  req->item_list[i].item_type;
		
		for (j = 0; j < 256; j++)
		{
			if (item_count && item_id == item_list->pages[j/32].goods[j%32].id)
			{
				item_price = world_manager::GetDataMan().get_item_shop_price(item_id);
				money_res += item_price * item_count;
				money += (float)item_price * (float)item_count;
				res++;
			}
		}
	}

	money += money / 100 * 5; //комиссия 5%
	money_res += money_res / 100 * 5; //комиссия 5%	
	if (req->count == res && money < max_money)
	{
		int fee = money_res;
		req->AdjustVendorFee(fee);
		req->money = fee;
		req->force_id = -700;
		return 1;
	}
	
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
bool gplayer_imp::CooldownManager(char type, int idx, time_t time)
{
	//0 дальше, 1 останавливаем
	
	switch (type)
	{
		case LuaManager::COOLDOWN_CHECK:
		{
			return GetCooldown().TestCoolDown(idx) ? false : true;
			break;
		}
		case LuaManager::COOLDOWN_SET:
		{
			GetCooldown().SetCoolDown(idx,time); return false;
			break;
		}
		case LuaManager::COOLDOWN_CHECK_AND_SET:
		{
			return CheckAndSetCoolDown(idx,time) ? false : true;
		}
		default: break;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
bool gplayer_imp::CooldownManagerMassive(char type, unsigned int * table)
{
	//0 дальше, 1 останавливаем
	
	LuaManager::COOLDOWN * cd = (LuaManager::COOLDOWN*)table;
	
	switch (type)
	{
		case LuaManager::COOLDOWN_CHECK:
		{
			for ( unsigned int i = 0; cd[i].xid && i < LuaManager::COOLDOWN_MAX_COUNT; i++ )
				if ( GetCooldown().TestCoolDown(cd[i].xid) )
					return false;
			return true;
			break;
		}
		case LuaManager::COOLDOWN_SET:
		{
			for ( unsigned int i = 0; cd[i].xid && i < LuaManager::COOLDOWN_MAX_COUNT; i++ )
				GetCooldown().SetCoolDown(cd[i].xid,cd[i].time); 
					return false;
			return true;
			break;
		}
		case LuaManager::COOLDOWN_CHECK_AND_SET:
		{
			for ( unsigned int i = 0; cd[i].xid && i < LuaManager::COOLDOWN_MAX_COUNT; i++ )
				if ( CheckAndSetCoolDown(cd[i].xid,cd[i].time) )
					return false;
			return true;
			break;
		}
		default: break;
	}
	return false;
}
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::GetInvPosItemTable(int inv , int * table)
{
	int index = -1;
	if(table)
	{
		for (int i = 0; table[i] && i < 16 ; i++)
		{
			index = GetInventory().Find(inv,table[i]);
			if(index >= 0) break;
		}
	}
	return index;
}
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::GetInvIdItemTable(int inv , int * table)
{
	int id = 0;
	int index = -1;
	if(table)
	{
		for (unsigned int i = 0; table[i] && i < 16 ; i++)
		{
			index = GetInventory().Find(0,table[i]);
			if(index >= 0) 
			{
				id = table[i];
				break;
			}
		}
	}
	return id;
}
//---------------------------------------------------------------------------------------------------------------------------
unsigned int gplayer_imp::InvPlayerGiveItem(int id, int count)
{
	unsigned int res = 0;
	if(id > 0 && count > 0 && count <= world_manager::GetDataMan().get_item_pile_limit(id))
	{
		if(_inventory.GetEmptySlotCount() < 1) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return 0;
		}
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * it = world_manager::GetDataMan().generate_item_from_player(id,&tag,sizeof(tag));
		
		if (it)
		{
			it->count = count;
			int pos = GetInventory(0).Push(*it);
			if (pos >= 0)
			{
				item inv_it = GetInventory(0)[pos];
				_runner->pickup_item(it->type, it->expire_date, count - it->count, inv_it.count,0,pos);
				if(inv_it.proc_type & item::ITEM_PROC_TYPE_AUTO_USE)
				{
					UseItem(GetInventory(0),pos,0,inv_it.type, 1);
				}
				res = 1;
			}
			FreeItem(it);
		}
	}
	return res;
}

//---------------------------------------------------------------------------------------------------------------------------
unsigned int gplayer_imp::InvPlayerGiveItem(int inv, int id, int count, int time, int proctype)
{
	unsigned int res = 0;
	if(id > 0 && count > 0 && count < world_manager::GetDataMan().get_item_pile_limit(id))
	{
		if(_inventory.GetEmptySlotCount() < 1) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return 0;
		}
		if ( time ) 
		{	
			time += g_timer.get_systime();
		}
		
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * it = world_manager::GetDataMan().generate_item_from_player(id,&tag,sizeof(tag));
		
		if (it)
		{
			it->count = count;
			it->expire_date = time;
			it->proc_type = proctype;
		
			int pos = GetInventory(inv).Push(*it);
			if (pos >= 0)
			{
				item inv_it = GetInventory(inv)[pos];
				_runner->pickup_item(it->type, it->expire_date, count - it->count, inv_it.count,inv,pos);
				if(inv_it.proc_type & item::ITEM_PROC_TYPE_AUTO_USE)
				{
					UseItem(GetInventory(inv),pos,0,inv_it.type, 1);
				}
				FirstAcquireItem(it);

				res = 1;
			}
			FreeItem(it);
		}
	}
	return res;
}
//---------------------------------------------------------------------------------------------------------------------------
unsigned int gplayer_imp::InvPlayerSpendItem(int inv, int id, int count)
{
	unsigned int res = 0;
	if(id > 0 && count > 0)
	{
		int index = GetInventory(inv).Find(0,id);
		if (index != -1)
		{
			item it   = GetInventory(inv)[index];

			if(it.count < count)
			{
				return 0;
			}

			if(it.count >= count)
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
				GetInventory(inv).DecAmount(index, count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,count,S2C::DROP_TYPE_USE);
				res = count;
			}
			else
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
				GetInventory(inv).DecAmount(index, it.count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,it.count,S2C::DROP_TYPE_USE);
				res = it.count;
			}
		}
	}
	return res;
}
//---------------------------------------------------------------------------------------------------------------------------
unsigned int gplayer_imp::InvPlayerSpendItem(int inv, int id)
{
	unsigned int res = 0;
	int index = GetInventory(inv).Find(0,id);
	
	if (index != -1)
	{
		item it   = GetInventory(inv)[index];
		if(it.count)
		{
			UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
			GetInventory(inv).DecAmount(index, it.count);
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,it.count,S2C::DROP_TYPE_USE);
			res = it.count;
		}
	}
	return res;
}
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::GatcherMineSupressor(int mine_id , int time)
{
	LuaManager * lua = LuaManager::GetInstance();
	
	if ( time >= 2 && 
		 lua->IsTrueMune(mine_id) && 
		 GetInvPosItemTable(0, lua->GetConfig()->ITEM_GATCHER_SPEED) != -1 )
		 time /= 2;
	return time;
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::SetAllCoolDown(int id1, int id2, int msec, int except_id) 
{ 
	if(_no_cooldown_mode && player_template::GetDebugMode())
	{
		msec = 100;
	}
	_cooldown.SetAllCoolDown(id1,id2,msec,except_id);
	_runner->send_cooldown_data();
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::LearnSkill(int skill_id, int skill_level)
{
	_skill.Push(skill_id, object_interface(this), skill_level);
	_runner->learn_skill(skill_id, skill_level);
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::DeleteSkill(int skill_id)
{
	_skill.Remove(skill_id);
	_runner->get_skill_data();
}
//---------------------------------------------------------------------------------------------------------------------------
//--AUTOSWAP
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::SetAutoSwap(void *buf, int size)
{
	/*
	if (size >= sizeof( autoswap_data ) && size <= (sizeof(autoswap_data) * 64) )
	{
		int autoswap_count = *(short *)buf;
		autoswap_data * src = (autoswap_data *)buf;
		src++;
		
		if (autoswap_count > 1 && autoswap_count < 64 && size >= (autoswap_count * sizeof(autoswap_data)))
		{
			for(unsigned int i = 0; i < autoswap_count && i < 64; i++ )
			{
				if (src[i].inv < 127 && src[i].equip < 64)
				{
					_autoswap[i] = src[i];
					_autoswap_count++;
				}
			}
			return 1;
		}
	}
	*/
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
int gplayer_imp::AutoSwapRelease(int swap_count)
{
	LuaManager * lua = LuaManager::GetInstance();
	if ( !CooldownManagerMassive(LuaManager::COOLDOWN_CHECK_AND_SET, (unsigned int*)lua->GetSwap()->cd_auto) )
	{
		for( unsigned int i = 0; i < _autoswap.count && i < swap_count && i < 64; i++ )
		{
			PlayerEquipItem(_autoswap.it[i].inv, _autoswap.it[i].equip);
		}
		return 1;
	}
	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------
//-- Item Equip Modificate
//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::InstallEquipStone(int chip_id, unsigned int chip_index, int equip_index)
{
	if ( chip_index >= _inventory.Size() || equip_index >= _inventory.Size() )
	{
		_runner->error_message(S2C::ERR_CANNOT_EMBED);
		return false;
	}
	
	DATA_TYPE dt;
	STONE_ESSENCE * ess = (STONE_ESSENCE*) world_manager::GetDataMan().get_data_ptr(chip_id, ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_STONE_ESSENCE) 
	{
		_runner->error_message(S2C::ERR_CANNOT_EMBED);
		return false;
	}
	
	unsigned int money = (unsigned int)ess->install_price;

	if( GetAllMoney() < money)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if( EmbedChipToEquipment(chip_index ,equip_index) )
	{
		SpendAllMoney(ess->install_price,true);
		SelfPlayerMoney();
		return true;
	}
	
	return true;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::ItemSetEquip(unsigned int it_index , unsigned int eq_index)
{
	LuaManager * lua = LuaManager::GetInstance();
	if ( !lua->GetConfig()->enable_patch_equip )
	{
		return false;
	}
	if (_lock_inventory)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return false;
	}

	if(OI_TestSafeLock())
	{
		_runner->error_message(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
		return true;
	}
	
	if ( !CheckAndSetCoolDown(COOLDOWN_INDEX_REFINE,REFINE_COOLDOWN_TIME) )
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return true;
	}
	
	if ( it_index >= _inventory.Size() || eq_index >= _inventory.Size() )
	{
		_runner->error_message(S2C::ERR_CANNOT_EMBED);
		return false;
	}
	
	DATA_TYPE it_dt, eq_dt;
	int it_type, eq_type;
	it_type = _inventory[it_index].type;
	eq_type = _inventory[eq_index].type;
	
	if (it_type > 0 && eq_type > 0)
	{
		const void* it_ess = world_manager::GetDataMan().get_data_ptr(it_type, ID_SPACE_ESSENCE, it_dt);
		const void* eq_ess = world_manager::GetDataMan().get_data_ptr(eq_type, ID_SPACE_ESSENCE, eq_dt);
		
		if (it_ess && eq_ess)
		{
			switch (it_dt)
			{
				case DT_STONE_ESSENCE:
				{
					if(eq_dt == DT_WEAPON_ESSENCE || eq_dt == DT_ARMOR_ESSENCE || eq_dt == DT_NEW_ARMOR_ESSENCE)
					{
						InstallEquipStone(it_type, it_index, eq_index);
						return true;
					}
					break;
				}
				case DT_ELEMENT_ESSENCE:
				{
					if(eq_dt == DT_WEAPON_ESSENCE || eq_dt == DT_ARMOR_ESSENCE || eq_dt == DT_NEW_ARMOR_ESSENCE || eq_dt == DT_DECORATION_ESSENCE)
					{
						if(it_type == 11208 || it_type == 56046 || it_type == 59663 )
						{
							if( !RefineItemAddon(eq_index, eq_type, -1, 2222) )
							{
								_runner->error_message(S2C::ERR_REFINE_CAN_NOT_REFINE);
							}
							return true;
						}
					}
					break;
				}
				case DT_REFINE_TICKET_ESSENCE:
				{
					if(eq_dt == DT_WEAPON_ESSENCE || eq_dt == DT_ARMOR_ESSENCE || eq_dt == DT_NEW_ARMOR_ESSENCE || eq_dt == DT_DECORATION_ESSENCE)
					{
						if( !RefineItemAddon(eq_index, eq_type, it_index, 2222) )
						{
							_runner->error_message(S2C::ERR_REFINE_CAN_NOT_REFINE);
						}
						return true;
					}
					break;
				}				
				default: break;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------

void gplayer_imp::ShortJump(A3DVECTOR & newpos)
{
   if( _player_state != PLAYER_STATE_NORMAL || _parent->IsZombie() ) 
   {
	   //printf("ShortJump: STATE ERROR!");
	   return;
   }

	_parent->pos = newpos;
	_runner->player_teleport(newpos,20,0); // Correção
	return;
}

void gplayer_imp::ShortJump(A3DVECTOR newpos, int tag)
{
   if( _player_state != PLAYER_STATE_NORMAL || _parent->IsZombie() ) 
   {
	   //printf("ShortJump: STATE ERROR!");
	   return;
   }

   const A3DVECTOR & pos = newpos;

	_parent->pos = pos;
	_runner->player_teleport(pos,20,0); // Correção
	return;
}
//---------------------------------------------------------------------------------------------------------------------------

void gplayer_imp::PrivateChat(int roleid, char channel, const void * buf, unsigned int len, const void * aux_data, unsigned int dsize)
{
	GMSV::chat_msg msg;
	msg.speaker = roleid;
	msg.msg = buf;
	msg.size = len;
	msg.data = aux_data;
	msg.dsize = dsize;
	msg.channel = channel;
	msg.emote_id = _chat_emote;
	msg.speaker_level = _basic.level;
	GMSV::SendChatMsg(GetParent()->cs_index, GetParent()->ID.id, GetParent()->cs_sid ,msg);
}

void gplayer_imp::GetLoteryInfo()
{
	unsigned int manager, score, count;
	unsigned int buf[256];
	memset(buf, 0x00, sizeof(buf));
	_lottery.GetInfo(manager, score, count,buf);
	// Corrige o bug de score bugado
	if(score < 0) score = 0;
	if(score > 50000) score = 0;
	_runner->get_lotery_info( manager, score, count, buf );
}

void gplayer_imp::GetLoteryItems()
{
	unsigned int manager, count;
	unsigned int buf[256];
	memset(buf, 0x00, sizeof(buf));
	_treasure_items.GetInfo(count, buf);
	_runner->get_lotery_items( count , buf );
}

void gplayer_imp::LotteryAward()
{
	LotteryConfig * lc = LotteryConfig::GetInstance();
	unsigned int score = _lottery.GetScore();
	unsigned int level = _lottery.GetLevel();
	unsigned int item, count;
	
	if ( lc->GetAward(score, level, item, count) )
	{
		_lottery.SetLevel(level);
		if (item && count)
		{
			element_data::item_tag_t tag = {element_data::IMT_NULL,0};
			item_data * data = world_manager::GetDataMan().generate_item_for_drop(item,&tag,sizeof(tag));
			if(data)
			{
          	  	FirstAcquireItem(data);
				FreeItem(data);
			}
			
			_runner->lottery_reward_info( 14, score, 1, item);
			_treasure_items.Add(item , count);
		}
	}
}

void gplayer_imp::Lottery2Init(unsigned int param)
{
	LotteryConfig * lc = LotteryConfig::GetInstance();
	if (lc->IsEnable())
	{
		if ( !CheckAndSetCoolDown(gplayer_lottery::COOLDOWN_IDX,gplayer_lottery::COOLDOWN_TIME) )
		{
			_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
			return;
		}
		
		if ( _lottery.IsStart() )
		{
			if (param == 2)
			{		
				if(!CheckItemExist(61729,5)) 
				{
					_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;	
				}	
				RemoveItems(61729,5, S2C::DROP_TYPE_USE, true);							
				_lottery.StartX5();
			}
			else
			{				
				if(!CheckItemExist(61729,1)) 
				{
					_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
					return;	
				}	
				RemoveItems(61729,1, S2C::DROP_TYPE_USE, true);	
				_lottery.StartX1();
			}
			LotteryAward();
			GetLoteryInfo();
			GetLoteryItems();
		}
	}
}

void gplayer_imp::Lottery2OpenBox(unsigned int param)
{
	LotteryConfig * lc = LotteryConfig::GetInstance();
	if (lc->IsEnable())
	{	
		if(_inventory.GetEmptySlotCount() < 1 ) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return;
		}	
		if ( !_lottery.IsStart() )
		{
			bool update = false;
			unsigned int pItem, pCount;
			if ( _lottery.GetLotRank(param) != gplayer_lottery::WHITE_LOT || param == gplayer_lottery::MAX_LOT )
			{
				for(unsigned int i = 0; i < gplayer_lottery::MAX_LOT; i++)
				{
					if( _treasure_items.GetCleanSlot() < 1 ) 
					{
						_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
						update = true;
						break;
					}
					pItem = pCount = 0;
					_lottery.OpenLot(i , pItem , pCount );
					if (pItem && pCount)
					{
						GLog::log( LOG_NOTICE, "formatlog:lotery_items:roleid=%d:itemid=%d:itemcount=%d",GetParent()->ID.id, pItem , pCount);
						
						element_data::item_tag_t tag = {element_data::IMT_NULL,0};
						item_data * data = world_manager::GetDataMan().generate_item_for_drop(pItem , &tag,sizeof(tag));
						if(data)
						{
          	  	 			FirstAcquireItem(data);
							FreeItem(data);
						}

						_treasure_items.Add(pItem , pCount);
						update = true;
					}
				}
			}
			else
			{
				if( _treasure_items.GetCleanSlot() < 1 ) 
				{
					_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
					return;
				}
				pItem = pCount = 0;
				_lottery.OpenLot(param , pItem , pCount );
				if (pItem && pCount)
				{
					GLog::log( LOG_NOTICE, "formatlog:lotery_items:roleid=%d:itemid=%d:itemcount=%d",GetParent()->ID.id, pItem , pCount);
					
					element_data::item_tag_t tag = {element_data::IMT_NULL,0};
					item_data * data = world_manager::GetDataMan().generate_item_for_drop(pItem,&tag,sizeof(tag));
					if(data)
					{
          	  		 	FirstAcquireItem(data);
						FreeItem(data);
					}

					_runner->lottery_reward_info( 14, _lottery.GetScore (), 0, 0);
					_treasure_items.Add(pItem , pCount);
					update = true;
				}
			}
			if (update)
			{
				GetLoteryInfo();
				GetLoteryItems();
			}
		}
	}
}

void gplayer_imp::LotteryGiveItem(unsigned int pItem)
{
	if(_inventory.GetEmptySlotCount() < 1 ) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	
	if (pItem)
	{
		unsigned int pCount = _treasure_items.Del(pItem);
		if (pCount)
		{
			InvPlayerGiveItem(pItem, pCount);
		}
		GetLoteryInfo();
		GetLoteryItems();
	}
}

void gplayer_imp::GetTreasureInfo()
{
	unsigned char manager = 0;
	unsigned int score = 0;
	unsigned int free_count = 0;
	unsigned int lot_count = 0;
	unsigned int box_count = 0;
	unsigned int lot[256];
	unsigned int box[256];
	memset(lot, 0x00, sizeof(lot));
	memset(box, 0x00, sizeof(box));
	
	_treasure.GetInfo(manager,score,free_count,lot_count,box_count,lot,box);
	// Corrige o bug de score bugado
	if(score < 0) score = 0;
	if(score > 31000) score = 0;
	_runner->get_treasure_info( manager, score, free_count, lot_count, box_count, lot, box );
}

void gplayer_imp::TreasureInit(unsigned int param)
{
	if ( !CheckAndSetCoolDown(gplayer_treasure::COOLDOWN_IDX,gplayer_treasure::COOLDOWN_TIME) )
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}

	TreasureConfig * tc = TreasureConfig::GetInstance();
	if ( tc->IsEnable() )
	{
		unsigned int req_item = tc->GetReqItem();
		unsigned int box_count = tc->GetReqCount();

		bool check = true;
		if(!CheckItemExist(req_item,box_count)) 
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			check = false;
		}	
		RemoveItems(req_item,box_count, S2C::DROP_TYPE_USE, true);	

		if (check)
		{
			if ( param && _treasure.GetScore() >= 20 )
			{
				_treasure.SetScore( (_treasure.GetScore() - 20) );
				_treasure.InitLot(2);
				GetTreasureInfo();
				_treasure.SetManager(0);
			}
			else
			{
				_treasure.InitLot(1);
				GetTreasureInfo();
				_treasure.SetManager(0);
			}
		}
	}
}

void gplayer_imp::GetTreasureOpenBox(unsigned int lot)
{
	TreasureConfig * tc = TreasureConfig::GetInstance();
	if ( tc->IsEnable() )
	{
		unsigned int req_item = tc->GetReqItem();
		unsigned int req_item_count = _treasure.GetFreeCount() ? 0 : tc->GetReqCost( _treasure.GetOpenLotCount() );

		bool check = true;
		if(!CheckItemExist(req_item,req_item_count)) 
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			check = false;
		}	
		RemoveItems(req_item,req_item_count, S2C::DROP_TYPE_USE, true);	

		if ( !req_item_count || check)
		{
			unsigned int item_id, item_count;
			item_id = item_count = 0;
			_treasure.OpenLot(lot, item_id , item_count);
			if (item_id && item_count)
			{
				_treasure_items.Add(item_id , item_count);

				//Rare Item
				element_data::item_tag_t tag = {element_data::IMT_NULL,0};
				item_data * data = world_manager::GetDataMan().generate_item_for_drop(item_id,&tag,sizeof(tag));
				if(data)
				{
          	  	 	FirstAcquireItem(data);
					FreeItem(data);
				}

				GetTreasureInfo();
				GetLoteryItems();
			}
		}
	}
}

void gplayer_imp::TreasureInfo()
{
	if( _treasure.GetScore() )
	{
		_treasure.SetManager(0);
		GetTreasureInfo();
	}
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::FactionAllianceCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req)
{
	bool result = false;
	LuaManager * lua = LuaManager::GetInstance();
	
	if (!lua->IsTrue(world_manager::GetWorldTag(), lua->GetConfig()->ALLIANCE_CONGREGATE_TAG))
	{
		_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_CANNOT_ADD_IN_THIS_WORLDTAG);
		return result;
	}

	if(IsCombatState())
	{
		_runner->error_message(S2C::ERR_INVALID_OPERATION_IN_COMBAT);
		return result;
	}
	if(((gplayer*)_parent)->IsInvisible()) 
	{
		_runner->error_message(S2C::ERR_OPERTION_DENYED_IN_INVISIBLE);
		return result;
	}
	if(!CheckCoolDown(COOLDOWN_INDEX_FACTION_ALLIANCE_CONGREGATE))
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return result;
	}
	//Ц?УР°пЦч/ё±°пЦч/МГЦч?ЙТФК№УГ
	char rank = OI_GetMafiaRank();
	if(rank != 2 && rank != 3 && rank != 4)
		return result;
	
	struct
	{
		world_pos wpos;
		int level_req;
		int sec_level_req;
        int reincarnation_times_req;
	}data;
	data.wpos.tag = world_manager::GetWorldTag();
	data.wpos.pos = _parent->pos;
	data.level_req = reply_level_req;
	data.sec_level_req = reply_sec_level_req;
    data.reincarnation_times_req = reply_reincarnation_times_req;

	for (std::unordered_map<int,int>::iterator it = _faction_alliance.begin(); it != _faction_alliance.end(); ++it)
	{
		int fanctionId = it->first;
		bool result = GMSV::SendFactionCongregateRequest(fanctionId, _parent->ID.id, &data, sizeof(data));
		if (result)
		{
			SetCoolDown(COOLDOWN_INDEX_FACTION_ALLIANCE_CONGREGATE, FACTION_ALLIANCE_CONGREGATE_COOLDOWN_TIME);
		}
	}

	return result;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::ChangeLeaderAssist()
{
	if(IsInTeam())
	{
		PlayerAssistSelect(GetTeamLeader());
	}
	return true;
}

bool gplayer_imp::SetAutoAssist()
{
	return auto_assist ^= true;
}

void gplayer_imp::SetColorName(unsigned int value)
{
	if ( !CheckAndSetCoolDown(7100,1000) )
	{
		_runner->error_message(S2C::ERR_OBJECT_IS_COOLING);
		return;
	}
	
	_color_name = value;
	SendPlayerOtherData(1, GetParent()->ID.id);
	//printf("SendPlayerOtherData: roleid = %d , color_name = %d , lua_name = %d \n", GetParent()->ID.id, value, _color_name );
}

void gplayer_imp::SendPlayerOtherData(bool is_other, int roleid)
{
	S2C::CMD::get_main_info_other gmio;
	memset(&gmio, 0x00, sizeof(gmio));
	gmio.header.cmd	= S2C::GET_MAIN_INFO_OTHER;
	
	if (roleid == GetParent()->ID.id)
	{
		gmio.roleid	= GetParent()->ID.id;
		gmio.colot_name	= _color_name;
		gmio.rank_exp	= _pvp_rank;
		gmio.rank_level	= _pvp_rank_exp;
	}
	else
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
		if(gPlayer && gPlayer->imp)
		{
			gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
			gmio.roleid	= pImp->GetParent()->ID.id;
			gmio.colot_name	= pImp->_color_name;
			gmio.rank_exp	= pImp->_pvp_rank;
			gmio.rank_level	= pImp->_pvp_rank_exp;
		}
	}
	
	is_other ? S2CGameDataSendElse(&gmio, sizeof(gmio)) : S2CGameDataSend(&gmio, sizeof(gmio));
	//printf("SendPlayerOtherData: roleid = %d , color_name = %d , lua_name = %d \n", gmio.roleid, gmio.colot_name, _color_name );
}

//---------------------------------------------------------------------------------------------------------------------------
// Nova Gravura
bool
gplayer_imp::NewEngraveItem(const engrave_recipe_template& ert, unsigned int inv_index, addon_data * session_addon_list, unsigned int session_count)
{

	if(inv_index >= _inventory.Size()) return false;
	item & it = _inventory[inv_index];
	if(it.type <= 0 || it.body == NULL) return false;

	//Faz as verificações necessárias de nível e etc..
	if(!(it.GetEquipMask64() & ert.equip_mask)) return false;
	int level = world_manager::GetDataMan().get_item_level(it.type);
	if(level < ert.equip_level_min || level > ert.equip_level_max) return false;

	//Consulta a lista de materiais
	engrave_recipe_template::__material mlist[8];
	ASSERT(sizeof(mlist) == sizeof(ert.material_list));
	memcpy(mlist,ert.material_list,sizeof(mlist));
	int num = ert.material_num;
	int total_count = ert.material_total_count;
	
	int nlist[_inventory.Size()];
	memset(nlist,0,sizeof(nlist));
	
	for(unsigned int i=0; i<_inventory.Size(); i++)
	{
		int type = _inventory[i].type;
		if(type == -1) continue;
		for(int j= 0; j < num; j++)
		{
			if(mlist[j].item_id == type)
			{
				unsigned int count = _inventory[i].count;
				if(count > mlist[j].count) count = mlist[j].count;
				nlist[i] = count;
				if(!(mlist[j].count -= count))
				{
					std::swap(mlist[j],mlist[num-1]);
					num --;
				}
				total_count -= count;
				break;
			}
		}
		if(total_count == 0) break;
	}
	ASSERT(total_count >= 0);
	if(total_count > 0)
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false;
	}

	// Gera os atributos para serem aplicados no item
	addon_data addon_list[3];
	unsigned int addon_count;
	addon_count = abase::RandSelect(ert.prob_addon_num,4);
	for(unsigned int i=0; i<addon_count; i++)
	{
		int r = abase::RandSelect(&ert.target_addons[0].prob,sizeof(ert.target_addons[0]),32);
		int addon_id = ert.target_addons[r].addon_id;
		if(!world_manager::GetDataMan().generate_addon(addon_id,addon_list[i])) return false;
	}

	struct //Estrutura dos Atributos
	{
		int addon_id;
		int addon_value;
		int reserve1; 
		int reserve2;

	}newengrave[4];

	for (int i = 0; session_addon_list && i < session_count ; i++)
	{
		newengrave[i].addon_id = session_addon_list[i].id+ENGRAVE_PACKET_SUM_ID;
		newengrave[i].addon_value = session_addon_list[i].arg[0];
		newengrave[i].reserve1 = 0;
		newengrave[i].reserve2 = 0;
	}

	// Envia o pacote para verificação
	_runner->engrave_new_info(session_count, (unsigned int*)newengrave);
			

		// Remove os itens usados para a Gravura
		for(unsigned int i = 0; i < _inventory.Size(); i ++)
		{
			if(nlist[i]) 
			{
				item& it = _inventory[i];
				UpdateMallConsumptionDestroying(it.type, it.proc_type, nlist[i]);

				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,i,_inventory[i].type,nlist[i],S2C::DROP_TYPE_PRODUCE);
				_inventory.DecAmount(i,nlist[i]);
			}
		}
	

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------

void
gplayer_imp::PlayerEngraveChoose(bool remain)
{	
	if (!_cur_session || _cur_session->GetRunTimeClass()->GetGUID() != CLS_SESSION_NEW_ENGRAVE || _cur_session->_session_id < 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);		
		return;
	}

	((session_new_engrave*)_cur_session)->ChooseItem(remain);
}

bool
gplayer_imp::EngraveChooseExec(const engrave_recipe_template& ert, unsigned int inv_index, addon_data * session_addon_list, unsigned int session_count)
{
	if(inv_index >= _inventory.Size()) return false;
	item & it = _inventory[inv_index];

	// Grava o item e envia o resultado
	if(!it.Engrave(session_addon_list, session_count))
	{
		return false;
	}

	PlayerGetItemInfo(IL_INVENTORY, inv_index);
	_runner->engrave_result((int)session_count); //_runner->engrave_result(1);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::LoadTrashBox(GDB::itemlist & box, unsigned int box_num)
{
	unsigned int iBox = 0, iBoxSize = 0;
	switch (box_num)
	{
	case 4:  { iBox = gplayer_imp::IL_TRASH_BOX5; iBoxSize = player_trashbox::BOX8_BOX_SIZE; } break;
	case 5:  { iBox = gplayer_imp::IL_TRASH_BOX6; iBoxSize = player_trashbox::BOX9_BOX_SIZE; } break;
	case 6:  { iBox = gplayer_imp::IL_TRASH_BOX7; iBoxSize = player_trashbox::BOX10_BOX_SIZE; } break;
	case 7:  { iBox = gplayer_imp::IL_TRASH_BOX8; iBoxSize = player_trashbox::BOX11_BOX_SIZE; } break;
	default: { return; 																		 } break;
	}

	if (box.count)
	{
		_trashbox.SetTrashBoxSize(box_num,box.count);
		GetTrashInventory(iBox).InitFromDBData(box);
		return;
	}
	else
	{
		if (iBoxSize > 0)
		{
			_trashbox.SetTrashBoxSize(box_num,iBoxSize);
		}
		return;
	}
	return;
}
//---------------------------------------------------------------------------------------------------------------------------
void gplayer_imp::SaveTrashBox()
{
	GDB::itemlist trash_box4;
	GDB::itemlist trash_box5;
	GDB::itemlist trash_box6;
	GDB::itemlist trash_box7;
	
	memset(&trash_box4, 0x00, sizeof(GDB::itemlist));
	memset(&trash_box5, 0x00, sizeof(GDB::itemlist));
	memset(&trash_box6, 0x00, sizeof(GDB::itemlist));
	memset(&trash_box7, 0x00, sizeof(GDB::itemlist));
	
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX5).MakeDBData(trash_box4);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX6).MakeDBData(trash_box5);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX7).MakeDBData(trash_box6);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX8).MakeDBData(trash_box7);
	
	LuaManager::GetInstance()->game__SaveTrashBox(_parent->ID.id, trash_box4, trash_box5, trash_box6, trash_box7 );
	
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX5).ReleaseDBData(trash_box4);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX6).ReleaseDBData(trash_box5);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX7).ReleaseDBData(trash_box6);
	GetTrashInventory(gplayer_imp::IL_TRASH_BOX8).ReleaseDBData(trash_box7);
}

bool gplayer_imp::GetTrashBoxPosItem(int where, int id, int & pos)
{
	pos = -1;
	bool res = false;
	
	if ( id > 0 && where >= IL_TRASH_BOX && where <= IL_TRASH_BOX8 )
	{
		pos = GetTrashInventory(where).Find(0,id);
		if(pos >= 0)
		{
			res = true;
		}
	}
	return res;
}

bool gplayer_imp::GetTrashBoxItemByTable(int where, int * table, int & id, int & pos)
{
	id = 0;
	pos = -1;
	bool res = false;
	
	if ( table && where >= IL_TRASH_BOX && where <= IL_TRASH_BOX8 )
	{
		for (unsigned int i = 0; table[i] && i < 16 ; i++)
		{
			pos = GetTrashInventory(where).Find(0,table[i]);
			if(pos >= 0) 
			{
				id = table[i];
				res = true;
				break;
			}
		}	
	}
	return res;
}

bool gplayer_imp::GiveTrashBoxItem(int where, int id, int count /*1*/, int time /*0*/, int proctype /*-1*/)
{
	bool res = false;
	
	if ( id > 0 && where >= IL_TRASH_BOX && where <= IL_TRASH_BOX8 )
	{
		if(GetTrashInventory(where).GetEmptySlotCount() < 1) 
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return res;
		}
		
		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * it = world_manager::GetDataMan().generate_item_from_player(id,&tag,sizeof(tag));
		
		if (it)
		{
			if (count > 0 && count <= world_manager::GetDataMan().get_item_pile_limit(id))
			{
				it->count = count;
			}
			else
			{
				count = it->count;
			}
			
			if (time) 
			{
				it->expire_date = time += g_timer.get_systime();
			}
			
			if (proctype != -1)
			{
				it->proc_type = proctype;
			}
		
			int pos = GetTrashInventory(where).Push(*it);
			if (pos >= 0)
			{
				item & inv_it = GetTrashInventory(where)[pos];
				_runner->produce_once(it->type, inv_it.count, inv_it.count, where, pos);
				FirstAcquireItem(it);
				res = true;
			}
			FreeItem(it);
		}
	}
	return res;
}

bool gplayer_imp::SpendTrashBoxItem(int where, int id, int count /*1*/)
{
	bool res = false;
	
	if ( id > 0 && count > 0 && where >= IL_TRASH_BOX && where <= IL_TRASH_BOX8 )
	{
		int pos = GetTrashInventory(where).Find(16,id);
		if (pos >= 0)
		{
			item it = GetTrashInventory(where)[pos];
			if(it.count >= count)
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
				GetTrashInventory(where).DecAmount(pos, count);
				_runner->player_drop_item(where,pos,it.type,count,S2C::DROP_TYPE_USE);
				res = true;
			}
			else
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
				GetTrashInventory(where).DecAmount(pos, it.count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,pos,it.type,it.count,S2C::DROP_TYPE_USE);
				res = true;
			}
		}
	}
	
	return res;
}

bool gplayer_imp::SpendTrashBoxItem2(int where, int inv, int count)
{
	bool res = false;
	
	if ( inv >= 0 && count > 0 && where >= IL_TRASH_BOX && where <= IL_TRASH_BOX8 )
	{
		if (inv >= 0)
		{
			item it = GetTrashInventory(where)[inv];
			if(it.count >= count)
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
				GetTrashInventory(where).DecAmount(inv, count);
				_runner->player_drop_item(where,inv,it.type,count,S2C::DROP_TYPE_PRODUCE);
				res = true;
			}
			else
			{
				UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
				GetTrashInventory(where).DecAmount(inv, it.count);
				_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv,it.type,it.count,S2C::DROP_TYPE_PRODUCE);
				res = true;
			}
		}
	}
	
	return res;
}

void gplayer_imp::SetBodyScale(float scale)
{
	if ( scale >= 0.0 )
	{
		gplayer* pPlayer = (gplayer*)_parent;
		pPlayer->body_scale = scale;
		if ( scale <= 0.999999 || scale >= 1.000001 )
		{
			pPlayer->object_state2 |= 4u;
		}
		else
		{
			pPlayer->object_state2 &= 0xFBu;
		}
		_runner->body_scale_notify(2,scale);
	}
}

void gplayer_imp::GetLibItems()
{
	unsigned int manager, count;
	unsigned int buf[256];
	memset(buf, 0x00, sizeof(buf));
	_lib_items.GetInfo(count, buf);	
	_runner->get_lib_items( count , buf );
}


void gplayer_imp::LibGiveItem(unsigned int pItem)
{
	if(_inventory.GetEmptySlotCount() < 1 ) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	
	if (pItem)
	{
		unsigned int pCount = _lib_items.Del(pItem);
		if (pCount)
		{
			InvPlayerGiveItem(pItem, pCount);
		}
		GetLibItems();
	}
}

bool 
gplayer_imp::ProduceLibItem(const lib_recipe_template & rt, int * materials, int * counts, int * idxs)
{	
	// Variáveis 
	lib_recipe_template::__material mlist[256];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));

	unsigned int money_cost = 0;
	int point_cost = 0;
	int point_required = rt.consume_point; // Ok
	int result_points = 0;
	int mutiple_points = 0;
	int result = 0;
	int count_itens = 0;

	struct get_material
	{
		int id;
		int point;

	}material[64];

	struct materials_info
	{
		int id;
		int count;
				
	}materialsinfo[64];	

	DATA_TYPE dt;
	const LIB_PRODUCE_RECIPE* recipe = (LIB_PRODUCE_RECIPE*)(world_manager::GetDataMan().get_data_ptr(rt.recipe_id, ID_SPACE_RECIPE, dt));
	if (dt != DT_LIB_PRODUCE_RECIPE || !recipe) return false;
		
	for (unsigned int n = 0; n < 256; n++)
	{		
		material[n].id = recipe->materials[n].id;
		material[n].point = recipe->materials[n].point;										

		for (unsigned int i = 0; i < 64; i++)
		{		
			materialsinfo[i].id = materials[i];
			materialsinfo[i].count = counts[i];
				
			if (material[n].id == materialsinfo[i].id)
			{
				if(materialsinfo[i].count != 0) 
				{ 
					point_cost += materialsinfo[i].count*material[n].point;		
				}					
			}			
		}
	} // Faz a busca do item e o ponto de produção

	// 	Verifica se ele tem pontos necessários
	if(point_cost < point_required) {								
		_runner->error_message(208); // Pontos insuficientes
		return false;
	}

	result_points = point_cost / point_required;
	
	for (unsigned int j = 0; j < result_points; j++) // Preço
	{
		money_cost += rt.fee;
	}

	// 	Verifica se ele tem moedas de prata suficientes
	if(GetAllMoney() < money_cost)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	// Materiais
	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 64; i++)
	{
		if(materials[i])
		{
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], counts[i]) 
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
					
			mlist[i].item_id = materials[i];
		}
	}
		
	// Remove as moedas:
	SpendAllMoney(money_cost,true);
	SelfPlayerMoney();

	// Remove os materiais usados

	bool is_reflesh = false;

	for(unsigned int i = 0; i < 64; i ++)
	{
		if(mlist[i].item_id)
		{
			item& it = _inventory[idxs[i]];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, counts[i]);
		
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idxs[i],_inventory[idxs[i]].type,counts[i],S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idxs[i],counts[i]);

			is_reflesh = true;
			
		}
	}

	if(is_reflesh)
	{
		_runner->result_lib_produce();
	}
	// Gera os itens 
	
	for (unsigned int n = 0; n < result_points; n++) // Preço
	{		
		unsigned int item_id, item_count;
		item_id = item_count = 0;
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 64);
		item_id = rt.targets[item_idx].id;
		item_count = rt.targets[item_idx].count;
		
		if (item_id)
		{	
			_lib_items.Add(item_id, item_count);

			//Rare Item
			element_data::item_tag_t tag = {element_data::IMT_NULL,0};
			item_data * data = world_manager::GetDataMan().generate_item_for_drop(item_id,&tag,sizeof(tag));
			if(data)
			{
          	    FirstAcquireItem(data);
				FreeItem(data);
			}
		}		
	}
	
	GetLibItems();
	return false;
}


// Easy Produce

bool 
gplayer_imp::ProduceEasyItem(unsigned int recipe_id, int times)
{
	// Variáveis 
	DATA_TYPE dt;
	EASY_PRODUCE_ITEM_ESSENCE * recipe = (EASY_PRODUCE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr((unsigned int)recipe_id, ID_SPACE_ESSENCE,dt);
	if( !recipe || dt != DT_EASY_PRODUCE_ITEM_ESSENCE) return false;
			
	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}	

	// Remove os materiais usados
	for(unsigned int i = 0; i < 10; i ++)
	{
		if(recipe->materials[i].id)
		{
			// Verifica se existe
			if(!_inventory.IsItemExist(recipe->materials[i].id, recipe->materials[i].num))
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}

			int index = GetInventory(gplayer_imp::IL_INVENTORY).Find(0,recipe->materials[i].id);
			if (index != -1)
			{
				item it	= GetInventory(gplayer_imp::IL_INVENTORY)[index];
				if(it.count >= recipe->materials[i].num)
				{
					UpdateMallConsumptionDestroying(it.type, it.proc_type, recipe->materials[i].num);
					GetInventory(gplayer_imp::IL_INVENTORY).DecAmount(index, recipe->materials[i].num);
					_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,recipe->materials[i].num,S2C::DROP_TYPE_USE);
				}
			}
		}
	}

	// Gera os itens 	
	unsigned int item_id, item_count;
	item_id = item_count = 0;
	int item_idx = abase::RandSelect(&(recipe->targets[0].probability),sizeof(recipe->targets[0]), 6);
		
	item_id = recipe->targets[item_idx].id_to_make;
	item_count = recipe->targets[item_idx].num_to_make;

	//Entrega Premiação		

	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * it = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(tag));
		
	if (it)
	{
		it->count = item_count;
		int pos = GetInventory(0).Push(*it);
		if (pos >= 0)
		{
			item inv_it = GetInventory(0)[pos];
			_runner->produce_once(it->type, item_count - it->count, inv_it.count,0,pos);
			
		}
	    FirstAcquireItem(it);	
		FreeItem(it);
	}
	
	return true;
}

// Card Produce

bool 
gplayer_imp::ProduceCardItem(int count_itens, int recipe_id, int * index_inv)
{	
	struct SS_TAG
	{
		char type;
		char size;
		char name[MAX_USERNAME_LENGTH];
	};
	
	DATA_TYPE dt;
	const RECIPE_ESSENCE* recipe = (RECIPE_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(recipe_id, ID_SPACE_RECIPE, dt));
	if (dt != DT_RECIPE_ESSENCE || !recipe) return false;

	if(recipe->poker_recipe != 1)
	{
		_runner->error_message(S2C::ERR_PRODUCE_FAILED);
		return false;
	}

	if(GetAllMoney() < recipe->price)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	int item_id = 0;
	if(abase::RandUniform() > recipe->fail_probability)
	{
		int item_idx = abase::RandSelect(&(recipe->targets[0].probability),sizeof(recipe->targets[0]), 4);
		item_id = recipe->targets[item_idx].id_to_make;
	}

	if (item_id < 1)
	{
		_runner->error_message(S2C::ERR_PRODUCE_FAILED);
		return false;
	}

	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	int bind_result = 0;
	if(recipe->bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(recipe->bind_type == 0) proc_type = recipe->proc_type;

	for(int i = 0; i < count_itens && i < 32; i ++)
	{
		auto itemIndex = index_inv[i];
		
		if (recipe->materials[i].id < 1)
		{
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		
		if (itemIndex < 0)
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}
		
		if (itemIndex >= _inventory.Size())
		{
			_runner->error_message(S2C::ERR_OUT_OF_FUND);
			return false;
		}
		
		if (_inventory[itemIndex].type == -1)
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}
		
		item& it = _inventory[itemIndex];
		if (it.type != recipe->materials[i].id)
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return false;
		}
		
		if (it.count < recipe->materials[i].num)
		{
			_runner->error_message(S2C::ERR_INVALID_ITEM);
			return false;
		}
		
		UpdateMallConsumptionDestroying(it.type, it.proc_type, recipe->materials[i].num);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,itemIndex,_inventory[itemIndex].type,recipe->materials[i].num,S2C::DROP_TYPE_PRODUCE);
		_inventory.DecAmount(itemIndex,recipe->materials[i].num);
	}
	
	SS_TAG tag;
	tag.type = element_data::IMT_PRODUCE;
	unsigned int  len;
	const void * name;
	name = GetPlayerName(len);
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(tag.name,name,len);
	tag.size = len;

	item_data* data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
	if(!data)
	{
		_runner->error_message(S2C::ERR_PRODUCE_FAILED);
		return false;
	}
	
	data->count = recipe->num_to_make;
	
	if(data->pile_limit > 1) 
	{
		bind_result = 0;
		proc_type = 0;
	}
	if(bind_result == 1)
	{
		data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND;
	} 
	else if(bind_result == 2)
	{
		data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
	}
	else if(bind_result == 3)
	{
		data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND |
			item::ITEM_PROC_TYPE_CAN_WEBTRADE;
	}
	else if(recipe->bind_type == 0)
	{
		data->proc_type |= proc_type;	
	}
	int rst = _inventory.Push(*data);
	if(rst >=0)
	{
	    FirstAcquireItem(data);
		SpendAllMoney(recipe->price,true);
		SelfPlayerMoney();
		
		_runner->produce_once(item_id,recipe->num_to_make - data->count,_inventory[rst].count, 0,rst);
	}

	if(data->count)
	{
		DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	FreeItem(data);
	return true;
}

bool
gplayer_imp::SummonPet3(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack)
{
	DATA_TYPE datatype;
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_egg_id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_PET_EGG_ESSENCE)	return false;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess->id_pet, ID_SPACE_ESSENCE, datatype);
	if(petess == NULL || datatype != DT_PET_ESSENCE)	return false;
	if(petess->id_type != 28752) return false;			//

	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = 500;								//
	data.hunger_gauge = pet_data::HUNGER_LEVEL_1;		//
	data.pet_tid = ess->id_pet;
	data.pet_vis_tid = 0;
	data.pet_egg_tid = ess->id;
	data.pet_class = pet_data::PET_CLASS_SUMMON;
	data.hp_factor = 0.5f;								//
	data.level = _basic.level;							//
	data.color = 0;
	data.exp = 0;
	data.skill_point = 0;
	data.name_len = 0;
	int skill_count = 0;
	for(unsigned int i = 0; i < 32 && skill_count < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		if(ess->skills[i].id_skill <= 0 
			|| ess->skills[i].level <= 0 ) continue;
		data.skills[skill_count].skill = ess->skills[i].id_skill;
		data.skills[skill_count].level = skill_level;
		++ skill_count;
	}
	return _petman.ActivePet3(this, data, life_time, skill_level, target, force_attack);
}

void 
gplayer_imp::ChecPetChell()
{
	// Verificação de Mascotes Válidos
	for(unsigned int i = 0; i < _petman.GetAvailPetSlot() && i < _petman.MAX_PET_CAPACITY; i ++)
	{
		pet_data * pData = _petman.GetPetData(i);		
		if(pData)
		{
			int tid = pData->pet_vis_tid;
			if(!tid) tid = pData->pet_tid;

			DATA_TYPE dt;
			PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(tid, ID_SPACE_ESSENCE, dt);
			if(petess == NULL || dt != DT_PET_ESSENCE)
			{
				_petman.FreePet(this,i);
			}
		}
	}
}

bool 
gplayer_imp::RefineMaterial(int item_type,int item_count, int inv_index, int refine_material, int inv_refine_material)
{
	// Verificações de existência
	if(inv_index >= _inventory.Size()) return false;
	item & it = _inventory[inv_index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;

	DATA_TYPE dt;
	DATA_TYPE dt2;
	const MATERIAL_REFINE_ESSENCE *ess = (const MATERIAL_REFINE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_type,ID_SPACE_ESSENCE,dt);
	const MATERIAL_REFINE_TICKET_ESSENCE *ess2 = (const MATERIAL_REFINE_TICKET_ESSENCE*)world_manager::GetDataMan().get_data_ptr(refine_material, ID_SPACE_ESSENCE,dt2);
	if(!ess && dt != DT_MATERIAL_REFINE_ESSENCE) return false;

	// Variáveis
	int max_item_count = ess->max_count;
	unsigned int id_to_make = ess->id_to_make;
	int num_to_make = ess->num_to_make;
	int count = 0;
	float prob = 0.0f;
	float prob_bonus = 0.0f;
	unsigned int price = 0;

	// Tabela de Materiais
	for(unsigned int i = 0; i < max_item_count; i++)
	{		
		if(i == item_count-1)
		{
			count = ess->list[i].count;
			prob = ess->list[i].probability;
			price = ess->list[i].price;
			break;
		}
	}

	// Tabela de Materiais extras
	for(unsigned int j =0; j < max_item_count; j ++)
	{
		if(j == item_count-1 && refine_material > 0)
		{
			prob_bonus = ess2->ext_succeed_prob[j];
			break;
		}
	}

	if(GetAllMoney() < price)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	// Variáveis de Finalização
	float prob_final = prob*100.0f;
	if(prob_bonus !=0) prob_final = prob_bonus*100.0f;
	
	// Remove as moedas
	SpendAllMoney(price,true);
	SelfPlayerMoney();
	
	if(item_type) // Remove o material principal
	{
		item& it = _inventory[inv_index];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_index,_inventory[inv_index].type,count,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(inv_index,count);

		PlayerGetItemInfo(IL_INVENTORY,inv_index);
	}
	if(refine_material > 0) // Remove o material de melhoria nas probabilidades
	{
		item& it = _inventory[inv_refine_material];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,inv_refine_material,_inventory[inv_refine_material].type,1,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(inv_refine_material,1);

		PlayerGetItemInfo(IL_INVENTORY,inv_refine_material);
	}

	float rand = abase::Rand(0,99);

	// Probabilidade de sucesso
	if(prob_final > rand)
	{
		// Entrega
		//InvPlayerGiveItem(id_to_make,num_to_make);

		element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
		item_data * it = world_manager::GetDataMan().generate_item_from_player(id_to_make,&tag,sizeof(tag));
		
		if (it)
		{
			it->count = num_to_make;
			int pos = GetInventory(0).Push(*it);
			if (pos >= 0)
			{
				item inv_it = GetInventory(0)[pos];
				_runner->produce_once(it->type, num_to_make - it->count, inv_it.count,0,pos);
			
			}
			
			FreeItem(it);
		}

		_runner->material_refine_result(0, 1);
		
	} else
	{
		// Falha
		_runner->material_refine_result(0, 0);
	}
	
	return true;
}


unsigned int 
gplayer_imp::GetTableDailyExp(unsigned int level)
{
	if(level > 0 && level < 10)
	{
		return 4000;
	} else if (level < 20)
	{
		return 8000;
	} else if (level < 30)
	{
		return 12000;
	} else if (level < 40)
	{
		return 16000;
	} else if (level < 50)
	{
		return 20000;
	} else if (level < 60)
	{
		return 25000;
	} else if (level < 70)
	{
		return 40000;
	} else if (level < 80)
	{
		return 80000;
	} else if (level < 100)
	{
		return 120000;
	}

	return 0;	
}

void 
gplayer_imp::SystemRealmExpDaily(unsigned int mobid)
{
	int exp = 0;
	int daily_exp = 0;
	unsigned int daily_exp_max = 0;

	if(mobid == 3218) return;

	// Busca o monstro
	DATA_TYPE dt;
	const MONSTER_ESSENCE *mon = (const MONSTER_ESSENCE*)world_manager::GetDataMan().get_data_ptr(mobid,ID_SPACE_ESSENCE,dt);
	if(!mon && dt != DT_MONSTER_ESSENCE) return;

	daily_exp_max = GetTableDailyExp(_realm_level);

	if(mon->vigour > 4000 || mon->vigour < 0) return;

	if(daily_exp_max > _diary_exp && mon->vigour > 0)
	{	
		if(_realm_level < mon->realm_level)
		{
			exp = mon->vigour;
			daily_exp = mon->vigour;
		} else
		{
			exp = mon->vigour*0.01;
			daily_exp = mon->vigour*0.01;

			if(exp < 1) exp = 1;
			if(daily_exp < 1) daily_exp = 1;
		}

		ReceiveRealmExp(exp, daily_exp);	

	}

}

void 
gplayer_imp::CheckRealmDay()
{
	time_t nnow;
	struct tm *tm_now;
	time(&nnow);
	tm_now = localtime(&nnow);
	enum
	{
		DAY_SUNDAY = 0,
		DAY_MONDAY = 1,
		DAY_TUESDAY = 2,
		DAY_WEDNESDAY = 3,
		DAY_THURSDAY = 4,
		DAY_FRIDAY = 5,
		DAY_SATURDAY = 6,
	};

	// Reseta a torre do martírio
	if(tm_now->tm_wday == DAY_WEDNESDAY && _glua.GetResetTowerDay() != tm_now->tm_wday)
	{
		PlayerResetSoloTower();
		_glua.SetResetTowerDay(tm_now->tm_wday);

	} else if(tm_now->tm_wday != _glua.GetResetTowerDay() && _glua.GetResetTowerDay() != DAY_WEDNESDAY)
	{
		_glua.SetResetTowerDay(tm_now->tm_wday);
	}
	
	if (tm_now->tm_mday != _realm_day_verify)
	{
		_realm_day_verify = tm_now->tm_mday;
		_diary_exp = 0;
		/*172+*/
		memset(&_day_world_points, 0x00, sizeof(_day_world_points) );
	}
	// Bebê Celestial
	if (tm_now->tm_mday != (int)GetLua()->GetChildResetDay())
	{
		if(EmulateSettings::GetInstance()->GetEnabledChild())
		{
			KidUnlockNewDay();
			GetLua()->SetChildResetDay((char)tm_now->tm_mday);

			GLog::log(GLOG_ERR,"formatlog: reset child:roleid=%d:resetday=%d",GetParent()->ID.id,tm_now->tm_mday);
		}	
	}	
}


/*156+*/
bool gplayer_imp::CanEnterArenaQueue(int is_6x6)
{
	if (!IsInTeam())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	if (!IsTeamLeader())
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	if ( (!is_6x6 && GetTeamMemberNum() != 3) || (is_6x6 && GetTeamMemberNum() != 6))
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	int check_race_count = 0;
	int check_race_mask = 0;
	for (int i = 0; i < GetTeamMemberNum(); i++)
	{
		int race = 1 << (GetTeamMember(i).race & 0x1F);
		if ((check_race_mask & race) != 0)
			continue;

		check_race_mask |= race;
		check_race_count++;
	}

	if (check_race_count != 3)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return false;
	}

	return true;
}

int gplayer_imp::PlayerArenaOfAuroraApply()
{
	return 0;
}

void gplayer_imp::PlayerLeaveArenaOfAurora()
{
	if (world_manager::GetInstance()->GetWorldType() != WORLD_TYPE_ARENAOFAURORA)
		return;
			
	_filters.ModifyFilter(FILTER_CHECK_INSTANCE_KEY, FMID_CLEAR_AEAA, NULL, 0);
}

void gplayer_imp::EnterArenaOfAuroraStep1(int target_tag, int arena_id)
{
	if (_player_state != PLAYER_STATE_NORMAL)
		return;

	if (target_tag == world_manager::GetWorldTag())
		return;

	_team.CliLeaveParty();

	instance_key key;
	memset(&key, 0, sizeof(key));
	GetInstanceKey(target_tag, key);
	key.target = key.essence;

	key.target.key_level4 = arena_id;

	ClearSwitchAdditionalData();

	A3DVECTOR pos(0,0,0);
	if (world_manager::GetInstance()->PlaneSwitch(this, pos, target_tag, key, 0) < 0)
		return;

	_switch_additional_data = new arenaofaurora_switch_data(0);
}

void gplayer_imp::EnterArenaOfAuroraStep2()
{
	arenaofaurora_switch_data* pData = substance::DynamicCast<arenaofaurora_switch_data>(_switch_additional_data);
	ASSERT(pData);

	ClearSwitchAdditionalData();
}

// end arena of aurora

// Carrier System		
bool
gplayer_imp::EnterCarrier(unsigned int npcid, int pos, int reserved1)
{
	if(_player_state != PLAYER_STATE_NORMAL)
		return false;

	if(!npcid)
		return false;

	gplayer * pPlayer = GetParent();
	pPlayer->object_state2 |= gactive_object::STATE_CARRIER;


int monster_id = _plane->GetNpcType(((gplayer_controller*)_commander)->GetCurTarget().id);

// Salva os valores de entrada no storage
	_carrier.npcid = npcid;
	_carrier.monster_id = monster_id;
	_carrier.pos = pos;
	_carrier.reserved1 = reserved1;	
// Fim

// Atualiza os valores com base no monstro

	DATA_TYPE dt;
	DATA_TYPE dt2;
	const MONSTER_ESSENCE *mon = (const MONSTER_ESSENCE*)world_manager::GetDataMan().get_data_ptr(monster_id,ID_SPACE_ESSENCE,dt);
	if(!mon && dt != DT_MONSTER_ESSENCE) return false;
	const CARRIER_CONFIG *carrier = (const CARRIER_CONFIG*)world_manager::GetDataMan().get_data_ptr(mon->carrier_id,ID_SPACE_CONFIG,dt2);
	if(!carrier && dt2 != DT_CARRIER_CONFIG) return false;

	_basic.hp = mon->life;
	_cur_prop.max_hp = mon->life;
	if(mon->damage_min > _cur_prop.damage_low)
	_cur_prop.damage_low = mon->damage_min;

	if(mon->damage_max > _cur_prop.damage_high)
	_cur_prop.damage_high = mon->damage_max;

	if(mon->magic_damage_min > _cur_prop.damage_magic_low)
	_cur_prop.damage_magic_low = mon->magic_damage_min;

	if(mon->magic_damage_max > _cur_prop.damage_magic_high)
	_cur_prop.damage_magic_high = mon->magic_damage_max;

	if(mon->defence > _cur_prop.defense)
	_cur_prop.defense = mon->defence;

	for (int i = 0; i < CARRIER_MAX_DEFENSE_MAGIC; i++)
	{
		_cur_prop.resistance[i] = mon->defence;
	}
	_cur_prop.attack_range = mon->attack_range;
	_cur_prop.walk_speed = mon->walk_speed;
	_cur_prop.run_speed = mon->run_speed;
	_cur_prop.swim_speed = mon->swim_speed;
	_cur_prop.flight_speed = mon->fly_speed;

	PlayerGetProperty();
// Fim 

// Envia os pacotes
	_runner->player_carrier_up(pos, CARRIER_RESERVE_RESULT, (unsigned int)npcid);
	_runner->player_carrier_self_up(pos, carrier->slots[0].unk3, (unsigned int)npcid,CARRIER_RES, mon->walk_speed, mon->run_speed, mon->swim_speed, mon->fly_speed);
	_runner->carrier_sync_info((unsigned int)npcid, CARRIER_RESERVE_RESULT, 2);
	_runner->carrier_prop((unsigned int)npcid, 2, 0);
// Fim

	return true;
}

bool 
gplayer_imp::LeaveCarrier(unsigned int npcid, int pos)
{

// Remove os valores de entrada no storage
	memset(&_carrier, 0x00, sizeof(_carrier));
//Fim

	gplayer * pPlayer = GetParent();
	pPlayer->object_state2 &= ~gactive_object::STATE_CARRIER;

// Retorna os valores originais
	property_policy::UpdatePlayer(GetPlayerClass(),this);
	PlayerGetProperty();
// Fim

// Envia os pacotes
	_runner->player_carrier_down(pos, npcid);
// Fim

	return true;
}

// Lock System

void
gplayer_imp::ChangeSafeLockNew(bool b)
{
	_passwd_safe.lock = b;
	gplayer_controller * pCtrl = (gplayer_controller *) _commander;
	pCtrl->UpdateSafeLock();
}

void
gplayer_imp::ChangeSafeLockActive(char inc)
{
	_passwd_safe.passwd_active = inc;
	gplayer_controller * pCtrl = (gplayer_controller *) _commander;
	pCtrl->UpdateSafeLock();
}

// Repositório (Suite)

int 
gplayer_imp::GetItensStorageRepository()
{
	int res = 0;
	for (int i = 0; i < 64; i++)
	{
		if (_repository.storage_itens[i] != 0)
			res++;
	}
	return res;
}

void
gplayer_imp::ProtocolRepositoryStorage()
{
	int itens[64];
	for (int i = 0; i < GetItensStorageRepository(); i++)
	{
		itens[i] = _repository.storage_itens[i];
	}	
	_runner->player_repository_storage(GetItensStorageRepository(), itens);
}

bool 
gplayer_imp::UnlockRepositorySystem()
{
	if(HasRepositoryLock()) { return false; }
	 
	_repository.lock = true;
	ProtocolRepositoryStorage();
	return true;
}

bool 
gplayer_imp::CheckStorageItem(int item_id)
{
	//if(!HasRepositoryLock()) { return false; }
	 
	for (int i = 0; i < 64; i++)
	{
		if (_repository.storage_itens[i] == item_id)
			return true;
	}
	return false;
}

bool 
gplayer_imp::CheckActiveItem(int item_id)
{
	for (int i = 0; i < 12; i++)
	{
		if (_repository.active_itens[i] == item_id)
			return true;
	}
	return false;
}

void
gplayer_imp::ClearRepositoryAddons()
{
	if(HasRepositoryLock())
	{	
		for (int i = 0; i < 12; i++)
		{
			_repository_addons[i] = NULL;
		}
	}
}

void 
gplayer_imp::ActivateRepositoryAddons()
{
	if(HasRepositoryLock())
	{	
		gactive_imp * imp = (gactive_imp*)_commander->_imp;
		for(int i = 0; i < 12; i++)
		{
			int item_id = _repository.active_itens[i];
			if(item_id != 0)
			{
				_repository_addons[i] = set_addon_manager::GetAddonList(item_id);
			}			
		}

		// Ativa os novos Addons
		for (int j = 0; j < 12; j++)
		{
			if(_repository_addons[j])
			{    
				for(int i = 0;i < _repository_addons[j]->size(); i++)
				{	
					addon_manager::Activate((*_repository_addons[j])[i],NULL,imp);	
				}
			}
		}
	}	
}

void 
gplayer_imp::DeactivateRepositoryAddons()
{
	if(HasRepositoryLock())
	{	
		gactive_imp * imp = (gactive_imp*)_commander->_imp;

		// Desativa os Addons
		for (int j = 0; j < 12; j++)
		{
			if(_repository_addons[j])
			{    
				for(int i = 0;i < _repository_addons[j]->size(); i++)
				{	
					addon_manager::Deactivate((*_repository_addons[j])[i],NULL,imp);	
				}
			}
		}
	}	
}

static int suite_ess[] = { 28620, 28621, 28622, 28623, 28624, 
						28625, 28626, 28627, 28638, 28639, 
						32734, 32735, 32736, 32737, 32738,
						32739, 32740, 32741, 32742, 32743,
						32990, 32991, 32992, 32993, 32994,
						32995, 32996, 32997, 32998, 32999,
						36540, 36541, 36542, 36543, 36544,
						36545, 36546, 36547, 36548, 36549,
						36626, 36627, 36628, 36629, 36630,
						36631, 36632, 36633, 36634, 36635,
						45611, 45612, 45613, 45614, 45615,
						45616, 45617, 45618, 45619, 45620,
						45717, 45718, 45719, 45720, 45721,
						45722, 45723, 45724, 45725, 45726,
						45727, 45728, 60007, 60008, 60009,
						60010, 60011, 60012, 60013, 60014,
						60015, 60837, 60838, 60839, 60840,
						60841, 60842, 60844, 60845, 60846,
						60847, 60848, 60849, 70147, 70148,
						70149, 70150, 70151, 70152,// 75629, 
						//75654, 75655, 75656, 75657, 75658, 
						0 }; // Em caso de novos equipamentos, adicionar	 

void 
gplayer_imp::CheckValidRepositoryItem(int item_id, item & iqt)
{
	//if(!HasRepositoryLock()) { return; }
				
	bool res = false;
	int count_suite_ess = sizeof(suite_ess) / sizeof(int);

	for(int i = 0; i < count_suite_ess; i++)
	{
		DATA_TYPE dt;
		SUITE_ESSENCE *suite = (SUITE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(suite_ess[i],ID_SPACE_ESSENCE,dt);
		if (suite && dt == DT_SUITE_ESSENCE)
		{
			for (int j = 0; j < 12; j++)
			{
				if (suite->equipments[j].id == item_id)
				{
					res = true;
					break;
				}
			}
		}
	}

	if(res)
	{
		for (int i = 0; i < 64; i++)
		{
			if (_repository.storage_itens[i] == item_id)
			break;

			if (_repository.storage_itens[i] == 0)
			{
				_repository.storage_itens[i] = item_id;
				break;
			}
		}

		ProtocolRepositoryStorage();
	}
}


void
gplayer_imp::ProtocolRepositoryHistory()
{
	int storage_itens = 12;
	int used_itens = 12;
	int equip[12];	
	int itens_storage[12];

	for (int i = 0; i < 12; i++)
	{
		itens_storage[i] = _repository.history_itens[i];
	}
	
	int itens_active[12];
	for (int j = 0; j < 12 && j <_equipment.Size(); j++)
	{
		if(_equipment[j].type == -1) { equip[j] = 0; }
		else { equip[j] = _equipment[j].type; }

		if(itens_storage[j] != 0) {	itens_active[j] = itens_storage[j];	}
		else {	itens_active[j] = equip[j];	}

		_repository.active_itens[j] = itens_active[j];		
	}

	_runner->player_repository_info(_parent->ID.id, storage_itens, itens_storage, used_itens, itens_active);
}

void 
gplayer_imp::ChangeRepositoryHistory(int item_id, int index)
{
	if(!HasRepositoryLock()) { return; }

	if ( index >= 0 && index <= 24 )
	{
		_repository.history_itens[index] = item_id;
	}

	ProtocolRepositoryHistory();
	RefreshEquipment();
}


bool 
gplayer_imp::RefineBibleAddon(unsigned int index, int item_type, int rt_index, int npcid)
{
	if(index >= _inventory.Size()) return false;
	item & it = _inventory[index];
	if(it.type ==-1 || it.body == NULL || item_type != it.type )  return false;
	if(rt_index >= 0 &&  (unsigned int)rt_index  >= _inventory.Size()) return false;

	// Verifica se o item está preso ( caso não esteja ele prende)
	if(it.CheckEquipCanBind()) {
			ASSERT(IsItemExist(index, item_type,1));

	it.proc_type |= item::ITEM_PROC_TYPE_NODROP | 
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND;
	if(it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
		it.proc_type &= ~item::ITEM_PROC_TYPE_CAN_WEBTRADE;

		UpdateMallConsumptionBinding(it.type, it.proc_type, it.count);
		PlayerGetItemInfo(IL_INVENTORY,index);
	}

	// Verifica o nível de refino
	int dest_level = it.body->GetRefineLevel();
	if(dest_level >= 12) return false;

	int material_need = 0xFFFFF;
	int refine_addon = world_manager::GetDataMan().get_item_refine_addon(item_type,material_need);

	int material_count = 0;

	// Busca na tabela do elements.data (todas as informações de refino)
	int id_table = 5072; // ID da tabela na elements.data
	for(unsigned int i = 0; i < 12; i++)
	{
		DATA_TYPE dt;
		BIBLE_REFINE_CONFIG *it = (BIBLE_REFINE_CONFIG*)world_manager::GetDataMan().get_data_ptr(id_table,ID_SPACE_CONFIG,dt);
		if (it && dt == DT_BIBLE_REFINE_CONFIG)
		{
			material_count = it->level[i].cost;
			break;
		}
		else
		{
			return false;
		}
	}
	
	// Busca os materiais necessários para refinar
	int material_id = 0;
	int material_index = -1;
	int * ITEM_BIBLE_REFINE = LuaManager::GetInstance()->GetConfig()->ITEM_BIBLE_REFINE;
	
	for (int i = 0; ITEM_BIBLE_REFINE[i] && i < 16 ; i++)
	{
		material_index = _inventory.Find(0,ITEM_BIBLE_REFINE[i]);
		if(material_index >= 0) 
		{
			material_id = ITEM_BIBLE_REFINE[i];
			break;
		}
	}	
		
	if(!CheckItemExist(material_id,	material_count))
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false; 
	}

	bool bonus_item = false;
	int item_bonus_id = 0;
	// Remove os materiais de bonus usados para refinar
	if(rt_index >= 0)
	{
		item& it = _inventory[rt_index];
		item_bonus_id = it.type;
		bonus_item = true;
		RemoveItems(it.type,1, S2C::DROP_TYPE_USE, true);
	}

	// Remove os materias usados para refinar
	RemoveItems(material_id,material_count, S2C::DROP_TYPE_USE, true);

	int result = 0;
	// Probabilidade de sucesso
	result = it.body->RefineBibleAddon(this, bonus_item, item_bonus_id);
	PlayerGetItemInfo(IL_INVENTORY,index);	

	switch(result)
	{
		case 1:
		{
			// Refino com sucesso
			_runner->refine_bible_result(0);

			if (dest_level+1 >= 9)
			SendRefineSuccessMessage(npcid, index, dest_level+1);

		}
		break;

		case 0:
		{
			// Refino com falha mantendo o refino
			_runner->refine_bible_result(1);
		}
		break;

		case 2:
		{
			// Refino com falha volta ao nível zero
			_runner->refine_bible_result(3);
		}
		break;

		case 3:
		{
			// Refino com falha reduzindo um nível
			_runner->refine_bible_result(2);
		}	
		break;
	}

	return true;
}


int  
gplayer_imp::RefineBibleTransmit(unsigned int src_index, unsigned int dest_index)
{
	if(src_index >= _inventory.Size() || dest_index >= _inventory.Size()) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	item & src_item = _inventory[src_index];
	item & dest_item = _inventory[dest_index];
	if(dest_item.type == -1 || src_item.type == -1) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	if(dest_item.body == NULL || src_item.body == NULL) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	

	int dest_level = dest_item.body->GetRefineLevel();
	int src_level = src_item.body->GetRefineLevel();
	if(dest_level < 0  || src_level < 0 || src_level > 12) return S2C::ERR_CAN_NOT_TRANSMIT_REFINE;
	
	// Verifica se o item está preso ( caso não esteja ele prende)
	if(dest_item.CheckEquipCanBind()) {
			ASSERT(IsItemExist(dest_index, dest_item.type,1));

	dest_item.proc_type |= item::ITEM_PROC_TYPE_NODROP | 
			item::ITEM_PROC_TYPE_NOTHROW|
			item::ITEM_PROC_TYPE_NOSELL |
			item::ITEM_PROC_TYPE_NOTRADE|
			item::ITEM_PROC_TYPE_BIND;
	if(dest_item.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE)
		dest_item.proc_type &= ~item::ITEM_PROC_TYPE_CAN_WEBTRADE;

		UpdateMallConsumptionBinding(dest_item.type, dest_item.proc_type, dest_item.count);
		PlayerGetItemInfo(IL_INVENTORY,dest_index);
	}
	int need_material = 0;

	int id_table = 5072;
	DATA_TYPE dt;
	BIBLE_REFINE_CONFIG *it = (BIBLE_REFINE_CONFIG*)world_manager::GetDataMan().get_data_ptr(id_table,ID_SPACE_CONFIG,dt);
	need_material = it->level[src_level-1].trans_cost;

	if(!CheckItemExist(ALLSPARK_ID,	need_material)) return S2C::ERR_TRANSMIT_REFINE_NO_MATERIAL;	
	RemoveItems(ALLSPARK_ID,need_material, S2C::DROP_TYPE_USE, true);

	//Lgc
	if(!src_item.body->TransmitRefine(0))
	{
		return S2C::ERR_CAN_NOT_TRANSMIT_REFINE;
	}
	if(!dest_item.body->TransmitRefine(src_level))
	{
		return S2C::ERR_CAN_NOT_TRANSMIT_REFINE;
	}

	_runner->transfer_refine_bible(0,0,dest_index);
	
	PlayerGetItemInfo(IL_INVENTORY,src_index);
	PlayerGetItemInfo(IL_INVENTORY,dest_index);
	return 0;
}

// Novos vstate Paladino
void gplayer_imp::GetPaladinWeaponLight( unsigned char state )
{
	if ( GetPlayerClass() == USER_CLASS_PALADIN && state <= 6 )
	{
		_runner->get_weapon_vstate(state);
	}
}

// Novas cargas do mercenário

void gplayer_imp::EnhanceChargeMerc(int val)
{ 
	if(GetPlayerClass() == USER_CLASS_ASN && _basic.sec.level >= 40)
	{
		_charge_merc += val;		
		if (_charge_merc > 6)
		{
			_charge_merc = 6;
		}
		_runner->get_charge_merc(_charge_merc);	

		// Reseta o contador de 15 segundos
		_charge_merc_time = 0;
	}
}

void gplayer_imp::ImpairChargeMerc(int val)
{ 
	if(GetPlayerClass() == USER_CLASS_ASN && _basic.sec.level >= 40)
	{
		_charge_merc -= val;
		if (_charge_merc < 0 )
		{
			_charge_merc = 0;
		}
		_runner->get_charge_merc(_charge_merc);	

		// Reseta o contador de 15 segundos
		_charge_merc_time = 0;
	}
}

void gplayer_imp::ChargeMercHeartBeat()
{	
	if(GetPlayerClass() == USER_CLASS_ASN && _basic.sec.level >= 40)
	{
		if(_charge_merc > 0)
		{
			_charge_merc--;
			_runner->get_charge_merc(_charge_merc);

			// Reseta o contador de 15 segundos
			_charge_merc_time = 0;
		}
	}		
}

void gplayer_imp::GetChargeMerc()
{	
	if(GetPlayerClass() == USER_CLASS_ASN && _basic.sec.level >= 40)
	{
		_runner->get_charge_merc(_charge_merc);		
	}		
}

// Segment

// cd skill limit begin

void gplayer_imp::GetClearSkillCD( int skill_id )
{
	if (skill_id)
	{
		unsigned int cdlimit = _skill.GetCoolDownLimit( skill_id );
		if (cdlimit)
		{
			_runner->skill_cooldown_limit( (skill_id + 1024) , 0 , 0 , cdlimit , cdlimit ) ;
		}
	}
}

void gplayer_imp::GetUseSkillCD(int skill_id, unsigned int cdlimit, unsigned int coolingtime)
{
	unsigned int cdcount = cdlimit;

	for(unsigned int i = 1; i <= cdlimit && i <= GNET::MAIN::MAX_MULTICOOLDOWN_SKILL; i++ )
	{
		int idx = ( ( ( skill_id * GNET::MAIN::MAX_MULTICOOLDOWN_SKILL ) + 8192) + i );
		unsigned int cdtime = (coolingtime * cdcount--);
		
		if ( CheckAndSetCoolDown( idx , cdtime ) )
		{
			break;
		}
		
		_cooldown.SetCoolDown ( idx , cdtime );
		
		if (i == cdlimit)
		{
			_cooldown.SetCoolDown( skill_id , coolingtime );
			break;
		}
	}
	
	_runner->skill_cooldown_limit( skill_id , time(NULL) , (coolingtime * 0.001) , cdcount , cdlimit ) ;
}

// cd skill limit end


//===========================================================================================================================
//-- GLYPH MANAGER
//===========================================================================================================================

bool gplayer_imp::SetGlyphSkill( int m_skill, unsigned int m_pos )
{
	if ( m_pos < _glyph.count && m_pos < MAX_GLYPH_SLOTS )
	{
		m_skill != 0 && GlyphConfig::GetInstance()->CheckBaseSkill(GetPlayerClass(), m_skill) ? _glyph.base_skills[m_pos] = m_skill : _glyph.base_skills[m_pos] = 0;
		return true;
	}
	return false;
}

int gplayer_imp::MakeGlyphSkill( glyph_skill_data & gsd , unsigned int m_pos )
{
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	memset(&gsd, 0x00, sizeof(gsd));
	
	// glyph enable slots
	if ( m_pos >= MAX_GLYPH_SLOTS )
	{
		return -1;
	}
	
	// item inventory size
	if ( m_pos >= box.Size() )
	{
		return -2;
	}
	
	// glyph open slots
	if ( m_pos >= _glyph.count || !_glyph.base_skills[m_pos] )
	{
		return -3;
	}
	
	// item glyph
	if ( box[m_pos].type <= 0 || !box[m_pos].body )
	{
		return -4;
	}
	
	// check item data and list
	DATA_TYPE dt;
	RUNE_ITEM_ESSENCE *it = (RUNE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr( box[m_pos].type ,ID_SPACE_ESSENCE,dt);
	if (!it || dt != DT_RUNE_ITEM_ESSENCE)
	{
		return -5;
	}
	
	gsd.base_skill = _glyph.base_skills[m_pos];
	gsd.item = box[m_pos].type;
	gsd.type = it->type;
	gsd.level = it->level;
	gsd.cls = GetPlayerClass();
	gsd.skill = GlyphConfig::GetInstance()->GetGlyph( gsd.cls , gsd.base_skill , gsd.type );
	
	return 0;
}

int gplayer_imp::GlyphGetAttr()
{
	int res = 0;
	std::map<int, int> attrCount;

	int attr_marcial = 0;
	int attr_dragao = 0;
	int attr_tigre = 0;

	if (_glyph.count >= 6)
	{ //MAX_GLYPH_SLOTS
		for (int i = 0; i < _glyph.count; i++)
		{
			int attr = _skill.GetSkillRuneAttr(_glyph.base_skills[i]);
			//attrCount[attr]++;
			
			switch (attr)
			{
			case 1:
				attr_marcial++;
				break;
			case 2:
				attr_dragao++;
				break;
			case 3:
				attr_tigre++;
				break;
			
			default:
				break;
			}
		}
	}


	if (attr_marcial >= 3 && attr_marcial > attr_dragao && attr_marcial > attr_tigre ) { return 1; }

	if (attr_dragao >= 3 && attr_dragao > attr_marcial && attr_dragao > attr_tigre ) { return 2; }

	if (attr_tigre >= 3 && attr_tigre > attr_dragao && attr_tigre > attr_marcial ) { return 3; }

	return 0;

}

// glyph packets
void gplayer_imp::GetSkillAttr()
{
	_runner->glyph_slot_info( _basic.sec.flag , _glyph.count , GlyphGetAttr() );
}

void gplayer_imp::GetGlyphData()
{
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	
	packet_wrapper h1(320);
	using namespace S2C;
	CMD::Make<CMD::glyph_data>::From(h1);
	
	h1 << _glyph.count; // count
	for(unsigned int i = 0; i < _glyph.count && i < MAX_GLYPH_SLOTS; i++)
	{
		h1 << i; // pos
		h1 << _glyph.base_skills[i]; // skill
		int m_item = (i < box.Size() && box[i].type > 0) ? box[i].type : 0;
		h1 << m_item; // item

	}
	
	send_ls_msg(GetParent(), h1);
}
// glyph packets end

// glyph skill 
bool gplayer_imp::GlyphSkillInc(int m_skill, unsigned int m_pos)
{
	bool res = false;
	int pvp_attr = _skill.GetSkillRuneAttr(m_skill);
	
	if (pvp_attr)
	{
		bool no_cooldown = false;
		for(int i=0; i < 6; i++)
		{
			printf( "gplayer_imp::GlyphSkillInc:GetCooldownID skill=%d \n", m_skill+i );
			int cd_id = GNET::SkillWrapper::GetCooldownID(m_skill+i);
			if(!CheckCoolDown(cd_id))
			{	
				_runner->error_message(S2C::ERR_RUNE_SKILL_TIME_COOLDOWN);
				return false;
			}
			else 
			{
				if(i == 5)
				{
					no_cooldown = true;
				}
			}
		}
		if(no_cooldown)
		{
			GlyphDectivate(m_pos);
			SetGlyphSkill(m_skill , m_pos);
			GlyphActivate(m_pos);
			
			property_policy::UpdatePlayerLimit(this);
			property_policy::UpdatePlayer(GetPlayerClass(),this);
			if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
			if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
			
			GetGlyphData();
			GetSkillAttr();
			PlayerGetProperty();
			res = true;
		}	
	}

	return res;
}

bool gplayer_imp::GlyphSkillDec(int m_skill, unsigned int m_pos)
{
	bool res = false;
	bool no_cooldown = false;
	for(int i=0; i < 6; i++)
	{
		printf( "gplayer_imp::GlyphSkillDec:GetCooldownID skill=%d \n", m_skill+i );
		int cd_id = GNET::SkillWrapper::GetCooldownID(m_skill+i);
		if(!CheckCoolDown(cd_id))
		{	
			_runner->error_message(S2C::ERR_RUNE_SKILL_TIME_COOLDOWN);
			return false;
		}
		else 
		{
			if(i == 5)
			{
				no_cooldown = true;
			}
		}
	}
	if(no_cooldown)
	{
		GlyphDectivate(m_pos);
		SetGlyphSkill(0 , m_pos);

		property_policy::UpdatePlayerLimit(this);
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
		if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
		
		GetSkillAttr();
		GetGlyphData();
		PlayerGetProperty();
		res = true;
	}
	
	return res;
}
// glyph skill end

// glyph item 
int gplayer_imp::GlyphActivate(unsigned int m_pos)
{
	// glyph enable slots
	if ( m_pos >= _glyph.count && m_pos >= MAX_GLYPH_SLOTS && _glyph.base_skills[m_pos] )
	{
		return -1;
	}
	
	// item inventory size
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	if ( m_pos >= box.Size() )
	{
		return -2;
	}
	
	// item glyph
	if ( box[m_pos].type <= 0 || !box[m_pos].body )
	{
		return -3;
	}
	
	// check make glyph
	glyph_skill_data gsd;
	if ( MakeGlyphSkill( gsd , m_pos ) != 0 )
	{
		return -4;
	}
	
	// item activete bonus
	item::LOCATION il = box.GetLocation();
	box[m_pos].Activate(il,box, m_pos, this);
	
	// glyph activate skill
	_skill.ActivateGlyphSkill( gsd.skill , gsd.base_skill , gsd.level );
	
	// update player state
	property_policy::UpdatePlayerLimit(this);
	
	// send glyph skills
	_runner->get_glyph_skill(gplayer_glyph::MODE_INC, m_pos, _glyph.base_skills[m_pos] , box[m_pos].type );
	
	// send skill cd
	GetClearSkillCD( gsd.skill );
	return 0;
}

int gplayer_imp::GlyphActivate()
{
	int res = 0;
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	
	for(unsigned int i = 0; i < _glyph.count && i < box.Size() && i < MAX_GLYPH_SLOTS; i++)
	{
		if ( GlyphActivate(i) == 0 ) res++;
	}
	
	return res;
}

int gplayer_imp::GlyphDectivate(unsigned int m_pos)
{
	// glyph enable slots
	if ( m_pos >= _glyph.count && m_pos >= MAX_GLYPH_SLOTS && _glyph.base_skills[m_pos] )
	{
		return -1;
	}
	
	// item inventory size
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	if ( m_pos >= box.Size() )
	{
		return -2;
	}
	
	// item glyph
	if ( box[m_pos].type <= 0 || !box[m_pos].body )
	{
		return -3;
	}
	
	// check make glyph
	glyph_skill_data gsd;
	if ( MakeGlyphSkill( gsd , m_pos ) != 0 )
	{
		return -4;
	}
	
	// all check success 
	
	// item activete bonus
	item::LOCATION il = box.GetLocation();
	box[m_pos].Deactivate(il, m_pos, this);
	
	// glyph activate skill
	_skill.DeactivateGlyphSkill( gsd.skill, gsd.base_skill );
	
	// update player state
	property_policy::UpdatePlayerLimit(this);
	
	// send glyph skills
	_runner->get_glyph_skill(gplayer_glyph::MODE_DEC, m_pos, _glyph.base_skills[m_pos] , box[m_pos].type );

	//printf("gplayer_imp::GlyphDectivate: success=%d \n", 0);
	return 0;
}

int gplayer_imp::GlyphDectivate()
{
	int res = 0;
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	
	for(unsigned int i = 0; i < _glyph.count && i < box.Size() && i < MAX_GLYPH_SLOTS; i++)
	{
		if ( GlyphDectivate(i) == 0 ) res++;
	}
	
	return res;
}

void gplayer_imp::GlyphUpdate(unsigned int m_pos)
{
	int res = 0;
	
	if ( GlyphDectivate(m_pos) == 0 ) res++;
	if ( GlyphActivate(m_pos) == 0 ) res++;
	
	GetSkillAttr();
	
	if ( res > 0 )
	{
		property_policy::UpdatePlayerLimit(this);
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
		if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
		
		GetGlyphData();
		PlayerGetProperty();
	}
}

void gplayer_imp::GlyphUpdate()
{
	int res = 0;
	res += GlyphDectivate();
	res += GlyphActivate();
	
	if ( res )
	{
		property_policy::UpdatePlayerLimit(this);
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
		if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
		PlayerGetProperty();
	}
	
	GetSkillAttr();
	GetGlyphData();
}
// glyph item end

// glyph equip

int gplayer_imp::GlyphEquipRefresh()
{
	int res = 0;
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	
	for(unsigned int i = 0; i < _glyph.count && i < box.Size() && i < MAX_GLYPH_SLOTS; i++)
	{
		if ( !GlyphDectivate(i) ) res++;		
	}

	for(unsigned int i = 0; i < _glyph.count && i < box.Size() && i < MAX_GLYPH_SLOTS; i++)
	{
		if ( !GlyphActivate(i) ) res++;
	}
	
	if (res)
	{
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
		if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
	}
	
	return res;
}

// glyph equip end


// glyph inventory 
bool gplayer_imp::GlyphVerifyInv(char where, unsigned char src, unsigned char dst, unsigned int count, bool is_move)
{
	// trashbox 5 inv 
	
	if (dst >= 24)
	{
		return true;
	}
	
	if (count != 1)
	{
		return false;
	}
	
	if (where != IL_TRASH_BOX5)
	{
		return false;
	}
	
	item_list & box = GetTrashInventory(gplayer_imp::IL_TRASH_BOX5);
	if ( dst > box.Size() || src > box.Size() )
	{
		return false;
	}
	
	int src_id = box[src].type;
	int dst_id = box[dst].type;
	
	if (src_id == dst_id)
	{
		return false;
	}
	
	if (dst > MAX_GLYPH_SLOTS)
	{
		return true;
	}
	
	// glyph 
	
	if ( dst >= _glyph.count )
	{
		return false;
	}
	
	if(IsCombatState())	
	{
		_runner->error_message(S2C::ERR_ELF_CANNOT_UNEQUIP_IN_COMBAT_STATE);
		return false;
	}
	
	return is_move;
}

int gplayer_imp::GlyphPlayerMoveTrashItem(int where, unsigned int src,unsigned int dest,unsigned int count)
{
	int res = 0;
	
	if ( GlyphDectivate(src) == 0 ) res++;
	if ( GlyphDectivate(dest) == 0 ) res++;
	
	PlayerMoveTrashItem(where, src, dest, count);

	if ( GlyphActivate(src) == 0 ) res++;
	if ( GlyphActivate(dest) == 0 ) res++;
	
	if (res)
	{
		property_policy::UpdatePlayerLimit(this);
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		if(_basic.hp > _cur_prop.max_hp) _basic.hp = _cur_prop.max_hp;
		if(_basic.mp > _cur_prop.max_mp) _basic.mp = _cur_prop.max_mp;
		PlayerGetProperty();
	}
	
	GetSkillAttr();
	GetGlyphData();
	
	return res;
}
// glyph inventory end

// glyph other system
void gplayer_imp::GlyphAttrRelease(int else_attr, int & damege)
{
	int host_attr =  GlyphGetAttr();
	if (host_attr == else_attr) // если атрибуты одинаковые то не увеличивать.
	{
		return;
	}
	
	if (host_attr == 0) // если атрибут игрока 0 , то он получает на 5% больше дамага.
	{
		damege *= 1.05f;
		return;
	}
	
	if (else_attr == 1 && host_attr == 2) // если у противника будет мощь , а у игрока будет мудрость
	{
		damege *= 1.15f;
		return;
	}
	
	if (else_attr == 2 && host_attr == 3) // если у противника будет мудрость , а у игрока будет храбрость
	{
		damege *= 1.15f;
		return;
	}	
	
	if (else_attr == 3 && host_attr == 1) // если у противника будет храбрость , а у игрока будет мощь
	{
		damege *= 1.15f;
		return;
	}	
	
}

bool gplayer_imp::UseGlyphUnlockItem(unsigned int pos, int rank, int level)
{
	//1 sucesso , 0 falha

	if(_basic.level < level)
	{
		_runner->error_message(S2C::ERR_LAST_LEVEL_INCORRET);
		return false;
	}

	if(_basic.sec.level < rank)
	{
		_runner->error_message(S2C::ERR_RUNE_CULTIVE_LOSE);
		return false;
	}

	if ( _basic.level >= level && _basic.sec.level >= rank && pos == (_glyph.count+1) && pos <= MAX_GLYPH_SLOTS )
	{
		_glyph.count++;
		GetSkillAttr();
		GlyphUpdate();

		_runner->error_message(S2C::ERR_RUNE_SLOT_OPEN);
		return true;
	}

	_runner->error_message(S2C::ERR_RUNE_SLOT_NOT_OPEN);
	return false;
}

bool gplayer_imp::GlyphItemUpgrade( C2S::CMD::glyph_item_upgrade::req_item * req )
{
	if ( !req || req[0].id <= 0 )
	{
		return false;
	}	

	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	
	DATA_TYPE dt;
	RUNE_ITEM_ESSENCE *ess = (RUNE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(req[0].id,ID_SPACE_ESSENCE,dt);
	if (!ess || dt != DT_RUNE_ITEM_ESSENCE)
	{
		return false;
	}
	
	float prob = 0.0f;
	
	int level_next = ess->level;
	int id_next = ess->next_level_item_id;
	int id_previous = ess->previous_level_item_id;
	
	GlyphConfig * conf = GlyphConfig::GetInstance();
	
	for(size_t i = 0; i < 6; i++)
	{
		DATA_TYPE dt_req;
		RUNE_ITEM_ESSENCE *ess_req = (RUNE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(req[i].id,ID_SPACE_ESSENCE,dt_req);
		if ( ess_req && dt_req == DT_RUNE_ITEM_ESSENCE )
		{
			if ( SpendTrashBoxItem( IL_TRASH_BOX5 , req[i].id) && i )
			{
				float plus = conf->GetUpgradeProb((level_next-1), (ess_req->level-1));
				prob += plus;
			}
		}
	}
	
	if ( prob > (float)((rand()%100)*0.01f) )
	{
		InvPlayerGiveItem(id_next , 1);
		_runner->get_glyph_upgrade(2, 0, 0, id_previous, req[0].id );
		int pos = _inventory.Find(0, id_next);		
		PlayerInvItemToTrash(IL_TRASH_BOX5, pos, 16, 1);
	}
	else
	{
		InvPlayerGiveItem(id_previous, 1);
		_runner->get_glyph_upgrade(2, 1, 0, id_previous, req[0].id );
		int pos = _inventory.Find(0, id_previous);		
		PlayerInvItemToTrash(IL_TRASH_BOX5, pos, 16, 1);
	}
	
	return true;
}

// glyph other system end

//===========================================================================================================================
//-- GLYPH MANAGER END
//===========================================================================================================================


// Notificação de Refino 166+
void 
gplayer_imp::SendRefineSuccessMessage(int npcid, unsigned int index, int refine_level)
{
	if(npcid == 0 || npcid == -1) return;
	
	int rst;
	unsigned short crc = 0;
	item_data data;
	rst = _inventory.GetItemData(index,data,crc);

	if (data.item_content == NULL) return;
   unsigned int item_id = data.type;

	DATA_TYPE dt;
	NPC_ESSENCE *ess = (NPC_ESSENCE*)world_manager::GetDataMan().get_data_ptr(npcid,ID_SPACE_ESSENCE,dt);
	if ( ess && dt == DT_NPC_ESSENCE )
	{
		struct
		{
			int npc_id;	
			unsigned int item_id;
			int refine_level;
			char name[MAX_USERNAME_LENGTH];

			int proc_type;
			int expire_date;
			unsigned int content_length;
		} data2;
		memset(&data2, 0, sizeof(data2));

		data2.npc_id = npcid;
		data2.item_id = item_id;
		data2.refine_level = refine_level;

		unsigned int len = _username_len;
		if (len > MAX_USERNAME_LENGTH)
			len = MAX_USERNAME_LENGTH;
		memcpy(data2.name, _username, len);

		data2.proc_type = data.proc_type;
		data2.expire_date = data.expire_date;
		data2.content_length = data.content_length;

		packet_wrapper buf(sizeof(data2) + data.content_length);
		buf.push_back(&data2, sizeof(data2));

		if (data.content_length > 0)
			buf.push_back(data.item_content, data.content_length);


		broadcast_chat_msg(REFINE_SUCCESS_NOTIFY, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0);
	}

}

// Notificação Equipamentos especiais

void 
gplayer_imp::SpecialItensNotify(int item_id) // Obsoleto
{
	//if(item_id == 0 || item_id == -1) return;
	//if(!world_manager::IsSpecialItem(item_id)) return;
	//if(CheckGMPrivilege() && !player_template::GetDebugMode()) return;

	/*struct SPECIAL_ITEM_MSG
	{
		char role_name[MAX_USERNAME_LENGTH];
		int item_id;
	};
	SPECIAL_ITEM_MSG data;

	if(item_id > 0)
	{
		short msg_ptoro = 119;			
		memset(&data,0,sizeof(data));
		unsigned int  len = _username_len;
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(data.role_name, _username, len);
		data.item_id = item_id;
			
		broadcast_chat_msg(msg_ptoro, &data, sizeof(data), GMSV::CHAT_CHANNEL_GLOBAL, 0, 0, 0);	
	}*/
}

//===========================================================================================================================
//-- NEW MONEY 172
//===========================================================================================================================

int gplayer_imp::PlayerChangeMoney(bool mode)
{
	if(_inventory.GetEmptySlotCount() < 1) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return 0;
	}
	
	unsigned int counter = _money.GetGold() / 10000000;
	
	if(counter)
	{
		if(!mode)
		{
			counter = 1;
		}
		
		if( _money.SpendGold(10000000 * counter) )
		{
			InvPlayerGiveItem(0, 21652, counter, 0, 0);
			SelfPlayerMoney();
		}
		
	}
	return counter;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::PlayerSpendGold(unsigned int money)
{
	if(_money.GetGold() >= money)
	{
		_money.SpendGold(money);
		SelfPlayerMoney();
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::PlayerGiveGold(unsigned int money)
{
	if ( _money.AddGold( money ) )
	{
		_runner->pickup_silver(money);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::PlayerSpendSilver(unsigned int money)
{
	if(_money.GetSilver() >= money)
	{
		_money.SpendSilver(money);
		SelfPlayerMoney();
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------

bool gplayer_imp::PlayerGiveSilver(unsigned int money)
{
	if ( _money.AddSilver( money ) )
	{
		_runner->pickup_silver(money);
		return true;
	}
	return false;
}

//---------------------------------------------------------------------------------------------------------------------------

void gplayer_imp::SelfPlayerMoney()
{
	_runner->get_player_money( _money.GetGold() , _money.GetSilver() );
}

bool gplayer_imp::ConvertMoney(unsigned int value)
{
	if ( _money.ConvertMoney( value ) )
	{
		_runner->get_player_money( _money.GetGold() , _money.GetSilver() );
		return true;
	}
	return false;
}

bool gplayer_imp::SyncMoney(unsigned int value)
{
	return _money.SyncMoney(value);
}

//-----------------------------------------------------------------

bool gplayer_imp::DecGoldAmount(unsigned int value)
{
	if ( _money.SpendGold(value) )
	{
		_runner->get_player_money( _money.GetGold() , _money.GetSilver() );
		return true;
	}
	return false;
}

bool gplayer_imp::DecSilverAmount(unsigned int value)
{
	if ( _money.SpendSilver(value) )
	{
		_runner->get_player_money( _money.GetGold() , _money.GetSilver() );
		return true;
	}
	return false;
}

bool gplayer_imp::DecAllMoneyAmount(unsigned int value)
{
	if ( _money.SpendMoney(value) )
	{
		_runner->get_player_money( _money.GetGold() , _money.GetSilver() );
		return true;
	}
	return false;
}

//-----------------------------------------------------------------

void 
gplayer_imp::GainGoldWithDrop(unsigned int inc,bool write_log)
{
	unsigned int newmoney = _money.GetGold() + inc;
	ASSERT((signed)_money.GetGold() >= 0);
	
	if((signed)newmoney < (signed)_money.GetGold() || newmoney > gplayer_money::MAX_GOLD)
	{
		newmoney = (unsigned int)-1;
		unsigned int delta = gplayer_money::MAX_GOLD - _money.GetGold();
		unsigned int drop = inc - delta;
		_money.SetGold(gplayer_money::MAX_GOLD);
		
		DropMoneyItem(_plane,_parent->pos,drop,_parent->ID,0,0,_parent->ID.id);
		GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d,���е���%d",_parent->ID.id,inc,drop);
	}
	else
	{
		if(write_log) GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d",_parent->ID.id,inc);
		_money.SetGold(newmoney);
	}
}

void 
gplayer_imp::GainGold(unsigned int inc)
{
	unsigned int player_money = _money.GetGold();
	unsigned int newmoney = player_money + inc;

	if((signed)newmoney < (signed)player_money || newmoney > gplayer_money::MAX_GOLD)
	{
		return ;
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d",_parent->ID.id,inc);
		_money.SetGold(newmoney);
	}
}

void 
gplayer_imp::SpendGold(unsigned int delta, bool write_log)
{
	unsigned int player_money = _money.GetGold();
	ASSERT(delta <= player_money);
	player_money -= delta;
	if(write_log) GLog::log(GLOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,delta);
	_money.SetGold(player_money);
}

bool 
gplayer_imp::CheckIncGold(unsigned int inc)
{
	unsigned long long player_money = _money.GetGold();
	unsigned long long v = player_money + inc;
	return ((signed)v > (signed)player_money && v <= gplayer_money::MAX_GOLD);
}


bool 
gplayer_imp::CheckDecGold(unsigned int inc)
{
	return _money.GetGold() >= inc;
}

//-------------------------------------------------------------------------------

void 
gplayer_imp::GainSilverWithDrop(unsigned int inc,bool write_log)
{
	unsigned int newmoney = _money.GetSilver() + inc;
	ASSERT((signed)_money.GetSilver() >= 0);
	
	if((signed)newmoney < (signed)_money.GetSilver() || newmoney > gplayer_money::MAX_GOLD)
	{
		newmoney = (unsigned int)-1;
		unsigned int delta = gplayer_money::MAX_GOLD - _money.GetSilver();
		unsigned int drop = inc - delta;
		_money.SetSilver(gplayer_money::MAX_GOLD);
		
		DropMoneyItem(_plane,_parent->pos,drop,_parent->ID,0,0,_parent->ID.id);
		GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d,���е���%d",_parent->ID.id,inc,drop);
	}
	else
	{
		if(write_log) GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d",_parent->ID.id,inc);
		_money.SetSilver(newmoney);
	}
}

void 
gplayer_imp::GainSilver(unsigned int inc)
{
	unsigned int player_money = _money.GetSilver();
	unsigned int newmoney = player_money + inc;

	if((signed)newmoney < (signed)player_money || newmoney > gplayer_money::MAX_GOLD)
	{
		return ;
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d�õ���Ǯ%d",_parent->ID.id,inc);
		_money.SetSilver(newmoney);
	}
}

void 
gplayer_imp::SpendSilver(unsigned int delta, bool write_log)
{
	unsigned int player_money = _money.GetSilver();
	ASSERT(delta <= player_money);
	player_money -= delta;
	if(write_log) GLog::log(GLOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,delta);
	_money.SetSilver(player_money);
}

bool 
gplayer_imp::CheckIncSilver(unsigned int inc)
{
	unsigned long long player_money = _money.GetSilver();
	unsigned long long v = player_money + inc;
	return ((signed)v > (signed)player_money && v <= gplayer_money::MAX_GOLD);
}

bool 
gplayer_imp::CheckDecSilver(unsigned int inc)
{
	return _money.GetSilver() >= inc;
}

//-------------------------------------------------------------------------------

void 
gplayer_imp::SpendAllMoney(unsigned int delta, bool write_log)
{
	unsigned long long player_money = _money.GetMoney();
	ASSERT(delta <= player_money);
	_money.SpendMoney( delta );
	if(write_log) GLog::log(GLOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,delta);
}

void 
gplayer_imp::SpendGoldMoney(unsigned int delta, bool write_log)
{
	unsigned long long player_money = _money.GetGold();
	ASSERT(delta <= player_money);
	_money.SpendGold( delta );
	if(write_log) GLog::log(GLOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,delta);
}

void 
gplayer_imp::SpendSilverMoney(unsigned int delta, bool write_log)
{
	unsigned long long player_money = _money.GetSilver();
	ASSERT(delta <= player_money);
	_money.SpendSilver( delta );
	if(write_log) GLog::log(GLOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,delta);
}

bool 
gplayer_imp::CheckDecAllMoney(unsigned int inc)
{
	return _money.GetMoney() >= inc;
}

//-------------------------------------------------------------------------------

void 
gplayer_imp::ThrowGold(unsigned int amount, bool isProtected)
{	
	if(world_manager::GetWorldLimit().nothrow_anyway) return;
	unsigned int player_money = _money.GetGold();
	if(amount >= player_money) amount = player_money;
	if(amount == 0) return ;
	GLog::log(LOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,amount);
	SpendGold(amount,false);

	//���������λ��
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();

	//��������˱߽磬��ô�Ͱ����Լ���λ������
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	if(isProtected)
		DropMoneyItem(_plane,pos,amount,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id);
	else
		DropMoneyItem(_plane,pos,amount,XID(0,0),0,0,_parent->ID.id);

	SelfPlayerMoney();
}

void 
gplayer_imp::PlayerDropGold(unsigned int amount,bool isProtected)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(world_manager::GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}
	ThrowGold(amount,isProtected);
}

void 
gplayer_imp::DropGoldAmount(unsigned int money, bool isProtected)
{ 
	bool bCanDrop = _player_state == PLAYER_STATE_NORMAL ||
			_player_state == PLAYER_DISCONNECT ||
			_player_state == PLAYER_SIT_DOWN ||
			_player_state == PLAYER_STATE_BIND;
	if(!bCanDrop) return;
	if(_free_pvp_mode) return;
	if(_basic.level <= LOW_PROTECT_LEVEL) return;
	if(_nonpenalty_pvp_state || world_manager::GetWorldFlag().nonpenalty_pvp_flag) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID2) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID3) >= 0) return;
	if(!_pvp_enable_flag) return;
	PlayerDropGold(money,isProtected); 
}

//-------------------------------------------------------------------------------

void 
gplayer_imp::ThrowSilver(unsigned int amount, bool isProtected)
{	
	if(world_manager::GetWorldLimit().nothrow_anyway) return;
	unsigned int player_money = _money.GetSilver();
	if(amount >= player_money) amount = player_money;
	if(amount == 0) return ;
	GLog::log(LOG_INFO,"�û�%d������Ǯ%d",_parent->ID.id,amount);
	SpendSilver(amount,false);

	//���������λ��
	A3DVECTOR pos(_parent->pos);
	pos.x += abase::Rand(-0.5f,+0.5f);
	pos.z += abase::Rand(-0.5f,+0.5f);
	const grid * pGrid = &_plane->GetGrid();

	//��������˱߽磬��ô�Ͱ����Լ���λ������
	if(!pGrid->IsLocal(pos.x,pos.z))
	{
		pos.x = _parent->pos.x;
		pos.z = _parent->pos.z;
	}

	if(isProtected)
		DropMoney2Item(_plane,pos,amount,_parent->ID,GetTeamID(),GetTeamSeq(),_parent->ID.id);
	else
		DropMoney2Item(_plane,pos,amount,XID(0,0),0,0,_parent->ID.id);

	SelfPlayerMoney();
}

void 
gplayer_imp::PlayerDropSilver(unsigned int amount,bool isProtected)
{
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}

	if(world_manager::GetWorldLimit().nothrow)
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}

	if(!_plane->CheckPlayerDropCondition())
	{
		_runner->error_message(S2C::ERR_CAN_NOT_DROP_ITEM);
		return ;
	}
	ThrowSilver(amount,isProtected);
}

void 
gplayer_imp::DropSilverAmount(unsigned int money, bool isProtected)
{ 
	bool bCanDrop = _player_state == PLAYER_STATE_NORMAL ||
			_player_state == PLAYER_DISCONNECT ||
			_player_state == PLAYER_SIT_DOWN ||
			_player_state == PLAYER_STATE_BIND;
	if(!bCanDrop) return;
	if(_free_pvp_mode) return;
	if(_basic.level <= LOW_PROTECT_LEVEL) return;
	if(_nonpenalty_pvp_state || world_manager::GetWorldFlag().nonpenalty_pvp_flag) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID2) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID) >= 0) return;
	if(_inventory.Find(0, ITEM_POPPET_DUMMY_ID3) >= 0) return;
	if(!_pvp_enable_flag) return;
	PlayerDropSilver(money,isProtected); 
}

//-------------------------------------------------------------------------------


void 
gplayer_imp::OnPickupGold(unsigned int money,int drop_id)
{	
	//printf("");
	
	
	if(drop_id)
	{
		GLog::log(GLOG_INFO,"�û�%d�����Ǯ%d[�û�%d����]",_parent->ID.id,money,drop_id);
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d�����Ǯ%d",_parent->ID.id,money);
	}

	//unsigned int player_money = _money.GetGold();

	// Verificação de Moedas
	if((_money.GetGold() + money) > TRASHBOX_MONEY_CAPACITY)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	

	unsigned int nmoney = _money.GetGold();
	GainGoldWithDrop(money,false);	//���治�ټ�¼��־
	if(_money.GetGold() > nmoney)
	{
		_runner->pickup_gold(_money.GetGold() - nmoney);
		SelfPlayerMoney();
	}
	//if((signed)player_money < 0) player_money = 0x7FFFFFFF;
	__PRINTF("������Ǯ%u\n",_money.GetGold());
	
	//_money.SetGold(player_money);
}

void 
gplayer_imp::OnPickupSilver(unsigned int money,int drop_id)
{	
	if(drop_id)
	{
		GLog::log(GLOG_INFO,"�û�%d�����Ǯ%d[�û�%d����]",_parent->ID.id,money,drop_id);
	}
	else
	{
		GLog::log(GLOG_INFO,"�û�%d�����Ǯ%d",_parent->ID.id,money);
	}

	//unsigned int player_money = _money.GetGold();

	// Verificação de Moedas
	if((_money.GetSilver() + money) > TRASHBOX_MONEY_CAPACITY)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return;
	}
	

	unsigned int nmoney = _money.GetSilver();
	GainSilverWithDrop(money,false);	//���治�ټ�¼��־
	if(_money.GetSilver() > nmoney)
	{
		_runner->pickup_silver(_money.GetSilver() - nmoney);
		SelfPlayerMoney();
	}
	//if((signed)player_money < 0) player_money = 0x7FFFFFFF;
	__PRINTF("������Ǯ%u\n",_money.GetSilver());
	
	//_money.SetGold(player_money);
}

//-------------------------------------------------------------------------------

void 
gplayer_imp::PlayerExchangeTrashMoney(char is_usertrashbox, unsigned int inv_money, unsigned int tra_money)
{
	//printf("gplayer_imp::PlayerExchangeTrashMoney: fuck=%d \n", 1);
	
	if(_lock_inventory)
	{	
		_runner->error_message(S2C::ERR_INVENTORY_IS_LOCKED);
		return ;
	}
	player_trashbox * box;
	
	if(is_usertrashbox)
		box = &_user_trashbox;
	else
		box = &_trashbox;
	if( (inv_money && tra_money) || (!inv_money && !tra_money) || inv_money > _money.GetGold() || tra_money > box->GetMoney())  
	{
		//printf("gplayer_imp::PlayerExchangeTrashMoney: err=%d \n", 1);
		_runner->error_message(S2C::ERR_FATAL_ERR);
		return ;
	}

	if(inv_money)
	{
		//ǮŲ��Ǯ��
		unsigned int delta = inv_money;
		unsigned int tm = box->GetMoney();
		unsigned int rst = tm + delta;
		if(rst < tm || rst > TRASHBOX_MONEY_CAPACITY)
		{
			delta = TRASHBOX_MONEY_CAPACITY - tm;
		}
		box->GetMoney() += delta;
		SpendGold(delta,true);
		GLog::log(GLOG_INFO,"�û�%d%s�ֿ�����Ǯ%d",_parent->ID.id, is_usertrashbox?"�ʺ�":"", delta);
		_runner->exchange_trash_money(is_usertrashbox,-(int)delta,delta);
		//printf("gplayer_imp::PlayerExchangeTrashMoney: fuck=%d \n", 2);
	}
	else
	{
		//��Ǯ�����ó�Ǯ

		//���ȼ��������Ƿ��㹻
		if(!CheckIncGold(tra_money))
		{
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return ;
		}

		//ֻ��Ǯ����ʱ��Ż�����Ǯ
		unsigned int delta = _money.GetGold();
		GainGold(tra_money);
		delta = _money.GetGold() - delta;
		box->GetMoney() -= delta;
		GLog::log(GLOG_INFO,"�û�%d%s�ֿ��Ƴ���Ǯ%d",_parent->ID.id, is_usertrashbox?"�ʺ�":"", delta);
		_runner->exchange_trash_money(is_usertrashbox,delta,-(int)delta);
		
		//printf("gplayer_imp::PlayerExchangeTrashMoney: fuck=%d \n", 2);
	}
}

//-------------------------------------------------------------------------------

bool gplayer_imp::IsGoldItem(int item)
{
	if(item > 0)
	{
		DATA_TYPE dt;
		CURRENCY_ITEM_WHITELIST_CONFIG *it = (CURRENCY_ITEM_WHITELIST_CONFIG*)world_manager::GetDataMan().get_data_ptr(6039,ID_SPACE_CONFIG,dt);
		if (it && dt == DT_CURRENCY_ITEM_WHITELIST_CONFIG)
		{
			for (unsigned int j = 0; j < 256; j++)
			{
				if (item == it->items[j])
				{
					return false;
				}
			}
		}
	}
	return true;
}

//-------------------------------------------------------------------------------

bool 
gplayer_imp::ItemToMoney(unsigned int inv_index, int type, unsigned int count, int price)
{
	if(inv_index >= _inventory.Size()) return false;
	const item & it = _inventory[inv_index];
	if(it.type == -1 || it.type != type || it.count < count) return false;
	if(it.proc_type & item::ITEM_PROC_TYPE_NOSELL) return false;
	if(IsItemForbidSell(type))
	{
		_runner->error_message(S2C::ERR_ITEM_FORBID_SELL);
		return false;
	}

	bool is_silver = IsGoldItem(type);
	unsigned int player_money = is_silver ? _money.GetSilver() : _money.GetGold();

	if(player_money > gplayer_money::MAX_GOLD) 
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		//printf("gplayer_imp::ItemToMoney: err=%d \n", 1);
		return false;
	}

	int real_price = GetItemRealTimePrice(it);
	if(price != real_price)
	{
		_runner->error_message(S2C::ERR_ITEM_PRICE_MISMATCH);
		return false;
	}
	float incmoney = (float)real_price * count;

	if(incmoney < 0 || incmoney == 0) incmoney = 1;

	int durability, max_durability;
	it.GetDurability(durability,max_durability);
	if(durability < max_durability && max_durability > 0)
	{
		incmoney = (incmoney * durability) / max_durability;
	}
	if(incmoney < 0) incmoney = 0;
	//if(incmoney >1e11) incmoney = 0;
	if(incmoney > gplayer_money::MAX_GOLD)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		//printf("gplayer_imp::ItemToMoney: err=%d \n", 2);
		return false;
	}
	unsigned int imoney = (unsigned int)(incmoney+0.5f);

	if(imoney < 0 || imoney == 0) imoney = 1;

	if(! (is_silver ? CheckIncSilver(imoney) : CheckIncGold(imoney)) )
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		//printf("gplayer_imp::ItemToMoney: err=%d \n", 3);
		return false;
	}

	GLog::log(GLOG_INFO,"�û�%d����%d��%d",_parent->ID.id,count,type);
	//��ȥ��Ʒ������Ǯ��
	UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
	_inventory.DecAmount(inv_index,count);
	unsigned int nmoney = player_money;
	is_silver ? GainSilver(imoney) : GainGold(imoney);

	//FIX172
	((gplayer_dispatcher*)_runner)->item_to_money(inv_index, type, count, is_silver ? 0 : _money.GetGold() - nmoney, is_silver ? GetSilver() - nmoney : 0 , 0 );
	return true;
}

/*172+*/

void 
gplayer_imp::ProtocolWorldPoints()
{
	int  total_world_points = _day_world_points.total;
	int  world_points = _day_world_points.points;
	char world_points_reward = _day_world_points.reward;
	char day_world_points_count = _day_world_points.count;
	char day_world_points_values[WorldPointsConfig::WORLD_POINT_LIST_COUNT];

	for (int i = 0; i < WorldPointsConfig::WORLD_POINT_LIST_COUNT; i++)
	{
		if( _day_world_points.it[i].valid_points == 1)
		{
			day_world_points_values[i] = _day_world_points.it[i].points;
		}		
	}

	_runner->player_world_contribution(total_world_points,world_points,world_points_reward,day_world_points_count,day_world_points_values);
}

bool 
gplayer_imp::ManagerWorldPoints(unsigned int value_task, unsigned int count_tasks)
{
	if(count_tasks != WorldPointsConfig::GetInstance()->GetList(value_task)->require_task_count)
	return false;
	
	bool res = false;

	for (int i = 0; i < WorldPointsConfig::WORLD_POINT_LIST_COUNT; i++)
	{
		
		if( _day_world_points.it[i].points == value_task)
		return false;

		if( _day_world_points.it[i].valid_points == 0)
		{
			_day_world_points.it[i].points = value_task;
			_day_world_points.it[i].valid_points  = 1;	
			res = true;
			break;
		}
	}
	
	if(res)
	{
		_day_world_points.count++;
		_day_world_points.points = 0;
		_day_world_points.total += WorldPointsConfig::GetInstance()->GetList(value_task)->point_award;

		unsigned int nmoney = _money.GetSilver();
		GainSilverWithDrop(WorldPointsConfig::GetInstance()->GetList(value_task)->money_award,false);
		if(_money.GetSilver() > nmoney)
		{
			_runner->pickup_silver(_money.GetSilver() - nmoney);
			SelfPlayerMoney();
		}
		
		ProtocolWorldPoints();
		return true;
	}
	
}

void 
gplayer_imp::RewardWorldPoints()
{
	int item_id = WorldPointsConfig::GetInstance()->GetReward(_day_world_points.reward)->item_award_id;
	int item_count = WorldPointsConfig::GetInstance()->GetReward(_day_world_points.reward)->item_award_count;
	int require_points = WorldPointsConfig::GetInstance()->GetReward(_day_world_points.reward)->require_points;

	if(require_points > _day_world_points.total )
		return;

	_day_world_points.reward++;

	InvPlayerGiveItem(item_id, item_count);	
	ProtocolWorldPoints();
}

/*160+*/
void 
gplayer_imp::SpeedSkillManager()
{
	if(LuaManager::GetInstance()->GetConfig()->speed_skill_enable < 1)
	return;
	
	int level = _basic.level;
	int reincarnation = GetReincarnationTimes();
	float speed = 10.4f;
	object_interface obj_if(this);
	
	if(level <= ActivityEventConfig::GetInstance()->GetExpBonusMaxLevel()
		&& reincarnation <= ActivityEventConfig::GetInstance()->GetExpBonusMaxReincarnation())
	{
		_cur_prop.walk_speed = speed;
		_cur_prop.run_speed = speed;
		_cur_prop.swim_speed = speed;
		_cur_prop.flight_speed = speed;
		obj_if.InsertTeamVisibleState(GNET::HSTATE_NEWBUFFSPEED, 99999);
	
		PlayerGetProperty();
	} else 
	{
		property_policy::UpdatePlayer(GetPlayerClass(),this);
		PlayerGetProperty();
		obj_if.RemoveTeamVisibleState(GNET::HSTATE_NEWBUFFSPEED);
	}
}

/*174+*/
void 
gplayer_imp::ActivityEventActivate()
{
	if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
	return;

	int level = _basic.level;
	int reincarnation = GetReincarnationTimes();
	int realm_level = GetRealmLevel();
	object_interface obj_if(this);

	if(realm_level < ActivityEventConfig::GetInstance()->GetRealmExpMaxRealmLevel())
	{
		obj_if.InsertTeamVisibleState(GNET::HSTATE_547, 99999);
	} else 
	{
		obj_if.RemoveTeamVisibleState(GNET::HSTATE_547);
	}
}


void 
gplayer_imp::ActivityEventSkillManager()
{
	if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
	return;

	if(!IsExistTeamVisibleState(GNET::HSTATE_547))
	return;

	int level = _basic.level;
	int reincarnation = GetReincarnationTimes();
	int realm_level = GetRealmLevel();
	object_interface obj_if(this);

	if(realm_level < ActivityEventConfig::GetInstance()->GetRealmExpMaxRealmLevel())
	{
		obj_if.InsertTeamVisibleState(GNET::HSTATE_548, 99999);	
		_double_factor_exp = true;
		_double_factor_sp = true;	
	} 
	else 
	{
		obj_if.RemoveTeamVisibleState(GNET::HSTATE_548);
		_double_factor_exp = false;
		_double_factor_sp = false;
	}

	if(realm_level < ActivityEventConfig::GetInstance()->GetRealmExpMaxRealmLevel())
	{
		obj_if.InsertTeamVisibleState(GNET::HSTATE_549, 99999);
		_double_factor_realm = true;
	} else 
	{
		obj_if.RemoveTeamVisibleState(GNET::HSTATE_549);
		_double_factor_realm = false;
	}

}

bool 
gplayer_imp::CheckActivityDay()
{
	time_t nnow;
	struct tm *tm_now;
	time(&nnow);
	tm_now = localtime(&nnow);
	
	if (tm_now->tm_mday != _activity.login_day_verify)
	{
		_activity.login_day_verify = tm_now->tm_mday;
		return true;
	}
	return false;
}

bool 
gplayer_imp::ActivityRewardLogin(int idx, int pos)
{
	if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
	return false;

	if(!IsExistTeamVisibleState(GNET::HSTATE_547))
	return false;

	int item_id = ActivityEventConfig::GetInstance()->GetRewardLogin(pos)->item_id;
	int item_count = ActivityEventConfig::GetInstance()->GetRewardLogin(pos)->item_count;

	if(item_id <= 0 || item_count <= 0)
	return false;

	if(CheckActivityDay())
	{
		if( _activity.event_login.count > pos)
		return false;	
		_activity.event_login.mode = 1;
		_activity.event_login.index = ActivityEventConfig::INDEX_REWARD_INTERFACE;
		_activity.event_login.count = pos;
		
		InvPlayerGiveItem(item_id, item_count);
	} 
	else
	{
		_runner->error_message(581);
	}

	_runner->reward_interface_notify( _activity.event_login.mode , _activity.event_login.index, _activity.event_login.count );
	return true;
}

bool 
gplayer_imp::ActivityShopBuy(int idx, int pos, int count)
{
	if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
	return false;

	if(!IsExistTeamVisibleState(GNET::HSTATE_547))
	return false;

	bool res = false;
	unsigned int price = ActivityEventConfig::GetInstance()->GetShop(pos)->price;

	if(GetMallCash() < price)
	{
		_runner->error_message(619);
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
	} else 
	{
		RemoveMallCash(price);
		_runner->player_cash(GetMallCash());
		res = true;
	}
		
	if(res)
	{
		if ( ActivityEventConfig::GetInstance()->GetShop(pos)->item_id != _activity.event_shop[pos].item_id )
		{
			_activity.event_shop_count++;
		}
		
		_activity.event_shop[pos].limit_mode = ActivityEventConfig::GetInstance()->GetShop(pos)->buy_times_limit_mode;
		_activity.event_shop[pos].item_id = ActivityEventConfig::GetInstance()->GetShop(pos)->item_id;
		_activity.event_shop[pos].limit_count = count;
	
		InvPlayerGiveItem(ActivityEventConfig::GetInstance()->GetShop(pos)->item_id, count);
	}	

	struct 
	{
		int limit_mode;
		int item_id;
		int limit_count;

	} item[ActivityEventConfig::MAX_SHOP_ITENS];

	for (int i = 0; i < _activity.event_shop_count && i < ActivityEventConfig::MAX_SHOP_ITENS; i++)
	{		
		item[i].limit_mode = _activity.event_shop[pos].limit_mode;
		item[i].item_id = _activity.event_shop[pos].item_id;
		item[i].limit_count = _activity.event_shop[pos].limit_count;
	}
	
	if(_activity.event_shop_count > 0)
	_runner->activity_event_shop(_activity.event_shop_count, (char)0, (unsigned int*)item);	

	if(_activity.event_cash_history > 0)
	_runner->activity_event_spend_cash(_parent->ID.id, _activity.event_cash_history);

	return true;
}

void 
gplayer_imp::ProtocolActivityEvent()
{

	if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
	return;

	if(!IsExistTeamVisibleState(GNET::HSTATE_547))
	return;

	if(_activity.event_shop_count > 0)
	{
		struct 
		{
			int limit_mode;
			int item_id;
			int limit_count;

		} item[ActivityEventConfig::MAX_SHOP_ITENS];

		for (int i = 0; i < _activity.event_shop_count && i < ActivityEventConfig::MAX_SHOP_ITENS; i++)
		{		
			item[i].limit_mode = _activity.event_shop[i].limit_mode;
			item[i].item_id = _activity.event_shop[i].item_id;
			item[i].limit_count = _activity.event_shop[i].limit_count;
		}	
	
		_runner->activity_event_shop( _activity.event_shop_count , (char)0, (unsigned int*)item);	
	}

	if( _activity.event_login.count > 0)
	_runner->reward_interface_notify( _activity.event_login.mode , _activity.event_login.index, _activity.event_login.count );

	if(_activity.event_cash_history > 0)
	_runner->activity_event_spend_cash(_parent->ID.id, _activity.event_cash_history);
	
}

bool
gplayer_imp::CheckEventValidTask(int task_id)
{
	int table[] = { 7029,7069,7070,7071,7072,
					7073,7074,7075,7076,7077,
					7078,7079,7080,7081,7082,
					7083,7084,7085,7086,7087,
					7088,7089,7090,7091,7092,
					7093,7094,7095,7096,7097,
					7098,7099,7100,7101,7102,
					7103,7104,7105,7106,7107,
					7108,7109,7110 };

	int count_table = sizeof(table) / sizeof(int);
	bool rest = false;

	for (int i = 0; i < count_table; i++)
	{
		DATA_TYPE dt;
		ACTIVITY_CHALLENGE_CONFIG * config = (ACTIVITY_CHALLENGE_CONFIG*) world_manager::GetDataMan().get_data_ptr(table[i], ID_SPACE_CONFIG, dt);
		if(config || dt == DT_ACTIVITY_CHALLENGE_CONFIG)
		{
			if(config->task_id == task_id)
			{
				rest = true;
				break;
			}
		}
	}

	if(rest)
	{	
		if(LuaManager::GetInstance()->GetConfig()->activity_event_enable < 1)
		return false;
		
		if(!IsExistTeamVisibleState(GNET::HSTATE_547))
		return false;
	}

	return true;
}

/*156+*/
bool gplayer_imp::EC_ArenaTeamInviteRequest(int inviting_roleid, int team_type, int invited_roleid, int idx)
{
	GMSV::EC_SendArenaTeamInviteRequest(inviting_roleid,team_type,invited_roleid,idx);
	return true;
}

bool gplayer_imp::EC_ArenaTeamInviteRequest_Re(int inviting_roleid, int team_type)
{
	if ( _inviting_tm.time <= 0 )
	{
		_inviting_tm.time = 30;
		_inviting_tm.id = inviting_roleid;
		_runner->arena_team_invite(inviting_roleid, team_type);
		return true;
	}
	return false;
}

bool gplayer_imp::EC_ArenaTeamInvite(int idx, int team_type, int inviting_roleid, int invited_roleid, int invited_cls)
{
	if ( _inviting_tm.time > 0 && _inviting_tm.id == inviting_roleid )
	{
		invited_cls = GetPlayerClass();
		GMSV::EC_SendArenaTeamInvite(idx, team_type, inviting_roleid, invited_roleid, invited_cls, _username, _username_len);
		return true;
	}
	return false;
}

bool gplayer_imp::EC_ArenaTeamKickout(int idx, int roleid, int kickid, int team_type)
{
	GMSV::EC_SendArenaTeamKickout(idx, roleid, kickid, team_type);
	return true;
}

bool gplayer_imp::EC_ArenaTeamCaptainTransfer(int idx, int roleid, int transferid, int team_type)
{
	GMSV::EC_SendArenaTeamCaptainTransfer(idx, roleid, transferid, team_type);
	return true;
}

bool gplayer_imp::EC_ArenaTeamLeave(int idx, int roleid, int team_type)
{
	GMSV::EC_SendArenaTeamLeave(idx,roleid,team_type);
	return true;
}
bool gplayer_imp::ArenaBattleEnter(int roleid, int is_team, int is_6x6)
{
	// add check tag and player 
	//if ( enter == 1 )
	{
		GMSV::EC_SendArenaAcceptBattle(roleid);
		return true;
	}
	
	return false;
}

bool gplayer_imp::EC_ArenaQuery(int roleid)
{
	//GMSV::EC_SendArenaQuery(roleid);
	return true;
}


// Memorial Celestial

void 
gplayer_imp::ProtocolCelestialMemorial()
{
	packet_wrapper h1(96);
	packet_wrapper h2(2048);
	packet_wrapper h3(2048);

	struct DATA
	{
		int level;
		int exp;
		int perg_03;
		int perg_02;
		int perg_01;
		int level_max;
		int ticket_1;
		int ticket_2;
		int level_max_2;
		int count_level_2;
		int count_level_1;
	};
	DATA new_info;

	int pos_level_2[CelestialMemorialConfig::MAX_LEVEL_RED_BOX];
	int pos_level_1[CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1];

	_celestial.ClearPos500MemorialRewardPos_1();

	bool type = _celestial.GetCelestialMemorialStruct()._type;

	new_info.level = _celestial.GetCelestialMemorialStruct()._memorial_level;
	new_info.exp = _celestial.GetCelestialMemorialStruct()._memorial_exp;
	new_info.perg_03 = _celestial.GetCelestialMemorialPerg(0);
	new_info.perg_02 = _celestial.GetCelestialMemorialPerg(1);
	new_info.perg_01 = _celestial.GetCelestialMemorialPerg(2);
	new_info.level_max = CelestialMemorialConfig::MAX_LEVEL_RED_BOX;
	new_info.ticket_1 = _celestial.GetCelestialMemorialTicket(0);
	new_info.ticket_2 = _celestial.GetCelestialMemorialTicket(1);
	new_info.level_max_2 =  (CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1 + (_celestial.GetCelestialNewMaxLevel() > 0 ? _celestial.GetCelestialNewMaxLevel() : 0));
	new_info.count_level_2 = _celestial.GetCelestialMemorialRewardCount(1);
	new_info.count_level_1 = _celestial.GetCelestialMemorialRewardCount(0);

	for(unsigned int i = 0; i < _celestial.GetCelestialMemorialRewardCount(1); i++)
	{
		pos_level_2[i] = (unsigned int)_celestial.GetCelestialMemorialRewardPos_2(i).item_pos;
	}

	for(unsigned int j = 0; j < _celestial.GetCelestialMemorialRewardCount(0); j++)
	{
		pos_level_1[j] = (unsigned int)_celestial.GetCelestialMemorialRewardPos(j).item_pos;	
	}

	h1.push_back(&new_info, sizeof(new_info));	
	h3.push_back(pos_level_2, new_info.count_level_2 * sizeof(int));
	h2.push_back(pos_level_1, new_info.count_level_1 * sizeof(int));

	_runner->celestial_memorial_info(type, h1.size(), h1.data(), h2.size(), h2.data(), h3.size(), h3.data());
}

void
gplayer_imp::TaskExpCelestialMemorial(int swallow_exp)
{
	int new_exp = 0;
	int temp_exp = 0;
	int new_level = 0;

	int levelup_exp = CelestialMemorialConfig::EXP_REQUIRED_PER_LEVEL;
	int max_level = CelestialMemorialConfig::MAX_LEVEL_RED_BOX_FINISH;
	int exp_celestial = _celestial.GetCelestialMemorialStruct()._memorial_exp;
	int level_celestial = _celestial.GetCelestialMemorialStruct()._memorial_level;

	if(swallow_exp <= 0)
	return;

	if((exp_celestial + swallow_exp) < levelup_exp) // Adiciona o exp sem evoluir
	{
		new_exp = exp_celestial + swallow_exp;
		_celestial.SetLevelandExpMemorial(level_celestial, new_exp);
	}
	else // Evolui o nível
	{
		temp_exp = exp_celestial + swallow_exp;
		new_level = temp_exp / levelup_exp;

		new_exp = temp_exp - (new_level * levelup_exp);

		new_level += level_celestial;

		// Removido limite de 500
		//if(new_level > max_level)
		//new_level = max_level;
		
		_celestial.SetLevelandExpMemorial(new_level, new_exp);
	}
	
	// Atualiza o Protocolo
	ProtocolCelestialMemorial();	
}

void
gplayer_imp::ConsumCelestialMemorial(int idx_inv)
{
	if ( idx_inv >= _inventory.Size() )
	{
		return;
	}
		
	item & it = _inventory[idx_inv];
	if ( it.type <= 0 )
	{
		return;
	}

	int IDX_ITEMS_UOGRADE[] = { 62342, 62343, 62456, 68012, 68086, 71286, 71287, 71288, 71289, 74781, 74782, 74783, 74784, 0};
	int count_itens = sizeof(IDX_ITEMS_UOGRADE) / sizeof(int);

	bool has_item_exp = false;
	int swallow_exp = 0;
	int new_exp = 0;
	int temp_exp = 0;
	int new_level = 0;
	int count_it = 0;
	int open_red_book = 0;
	int max_count_required = 0;

	int levelup_exp = CelestialMemorialConfig::EXP_REQUIRED_PER_LEVEL;
	int max_level = CelestialMemorialConfig::MAX_LEVEL_RED_BOX_FINISH;
	int exp_celestial = _celestial.GetCelestialMemorialStruct()._memorial_exp;
	int level_celestial = _celestial.GetCelestialMemorialStruct()._memorial_level;
	
	for(unsigned int i = 0; i < count_itens; i++)
	{
		DATA_TYPE dt;
		RED_BOOK_UPGRADE_ITEM *ess = (RED_BOOK_UPGRADE_ITEM*)world_manager::GetDataMan().get_data_ptr(IDX_ITEMS_UOGRADE[i],ID_SPACE_ESSENCE,dt);
		if (ess && dt == DT_RED_BOOK_UPGRADE_ITEM)
		{
			if(it.type == ess->id)
			{
				swallow_exp = ess->swallow_exp;
				open_red_book = ess->open_red_book;
				break;
			}	
		}		
	}

	if(swallow_exp > 0)
	{
		count_it += it.count;

		int var1;
		int var2;
		var1 = level_celestial * levelup_exp;
    	var2 = (var1 - exp_celestial) / swallow_exp;

		if(count_it > var2)
		count_it = var2;
		
		swallow_exp *= count_it;
	}


	bool rest = false;
	if(open_red_book == 1 && _celestial.GetCelestialMemorialStruct()._type == 0) // Abre o Nível 2
	{
		if(it.type > 0)
		{		
			// Remove os itens
			UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
		
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_inv,it.type,it.count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx_inv,it.count);

			rest = true;	
		}

		if(rest)
		{
			if(level_celestial == 0)
			_celestial.SetLevelandExpMemorial(1, exp_celestial);

			// Desbloqueia
			_celestial.SetTypeCelestialMemorial(true);
			_celestial.SetHasCelestialMemorialUnlock(true);
		}
	} 
	else // Adiciona Exp
	{	
		if(it.type > 0)
		{
			// Remove os itens
			UpdateMallConsumptionDestroying(it.type, it.proc_type, count_it);
		
			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_inv,it.type,count_it,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx_inv,count_it);

			rest = true;
		}

		if(rest)
		{
			// Multiplica pela quantidade de itens

			if((exp_celestial + swallow_exp) < levelup_exp) // Adiciona o exp sem evoluir
			{
				new_exp = exp_celestial + swallow_exp;
				_celestial.SetLevelandExpMemorial(level_celestial, new_exp);
			}
			else // Evolui o nível
			{
				temp_exp = exp_celestial + swallow_exp;
				new_level = temp_exp / levelup_exp;

				new_exp = temp_exp - (new_level * levelup_exp);

				new_level += level_celestial;

				// Removido Limite 500
				//if(new_level > max_level)
				//new_level = max_level;
		
				_celestial.SetLevelandExpMemorial(new_level, new_exp);
			}
		}		
	}
	
	// Atualiza o Protocolo
	ProtocolCelestialMemorial();	
}


void 
gplayer_imp::ProtocolCelestialMemorialLottery()
{
	packet_wrapper h1(150);

	struct DATA
	{
		int type;

		int item_id_1;	
		int item_id_2;
		int item_count_1;
		int item_count_2;

		int storage_rest_cnt[5];
		char storage_reward[CelestialMemorialConfig::MAX_STORAGE_LOTTERY];		

	};

	DATA lotterys[CelestialMemorialConfig::COUNT_LOTTERY_RED_BOX];

	for(unsigned int i = 0; i < CelestialMemorialConfig::COUNT_LOTTERY_RED_BOX; i++)
	{
		lotterys[i].type = i;

		lotterys[i].item_id_1 = _celestial.GetCelestialMLottery(i)._item_id_1;
		lotterys[i].item_id_2 = _celestial.GetCelestialMLottery(i)._item_id_2;
		lotterys[i].item_count_1 = _celestial.GetCelestialMLottery(i)._item_count_1;
		lotterys[i].item_count_2 = _celestial.GetCelestialMLottery(i)._item_count_2;

		for(unsigned int n = 0; n < 5; n++)
		{
			lotterys[i].storage_rest_cnt[n] = (int)_celestial.GetCelestialMemorialPerg(i);
		}

		for(unsigned int j = 0; j < CelestialMemorialConfig::MAX_STORAGE_LOTTERY; j++)
		{
			lotterys[i].storage_reward[j] = (char)_celestial.CountStorageReward(i, j);
		}
	}

	h1.push_back(lotterys, sizeof(lotterys));
		
	_runner->celestial_memorial_lottery(CelestialMemorialConfig::COUNT_LOTTERY_RED_BOX, h1.size(), h1.data());
}


void 
gplayer_imp::ProtocolCelestialMemorialLotteryReward(unsigned int idx_pos_type)
{
	packet_wrapper h1(150);

	struct DATA
	{
		int type;

		int item_id_1;	
		int item_id_2;
		int item_count_1;
		int item_count_2;

		int storage_rest_cnt[5];
		char storage_reward[CelestialMemorialConfig::MAX_STORAGE_LOTTERY];		

	};

	DATA lotterys;

	lotterys.type = idx_pos_type;

	lotterys.item_id_1 = _celestial.GetCelestialMLottery(idx_pos_type)._item_id_1;
	lotterys.item_id_2 = _celestial.GetCelestialMLottery(idx_pos_type)._item_id_2;
	lotterys.item_count_1 = _celestial.GetCelestialMLottery(idx_pos_type)._item_count_1;
	lotterys.item_count_2 = _celestial.GetCelestialMLottery(idx_pos_type)._item_count_2;

	for(unsigned int n = 0; n < 5; n++)
	{
		lotterys.storage_rest_cnt[n] = (int)_celestial.GetCelestialUsePergProb(idx_pos_type);
	}

	for(unsigned int j = 0; j < CelestialMemorialConfig::MAX_STORAGE_LOTTERY; j++)
	{
		lotterys.storage_reward[j] = (char)_celestial.CountStorageReward(idx_pos_type, j);
	}

	h1.push_back(&lotterys, sizeof(lotterys));
		
	_runner->celestial_memorial_lottery(1, h1.size(), h1.data());
}


void
gplayer_imp::GetCelestialMemorialReward(unsigned int pos, unsigned int type)
{
	int item_id;
	int item_count;

	bool res = false;
	// Busca os itens
	if (type == 0)
	{
		item_id = CelestialMemorialConfig::GetInstance()->GetLevel_1_Rewards(pos)->item_id;
		item_count = CelestialMemorialConfig::GetInstance()->GetLevel_1_Rewards(pos)->item_count;

		res = true;	
	}
	else
	{
		item_id = CelestialMemorialConfig::GetInstance()->GetLevel_2_Rewards(pos)->item_id;
		item_count = CelestialMemorialConfig::GetInstance()->GetLevel_2_Rewards(pos)->item_count;

		res = true;
	}
	
	// Salva no banco de dados
	if(res)
	{
		if(pos >= 500 && type == 1)
		{
			item_id = CelestialMemorialConfig::PERG_RANDOM_LOTTERY_ID;
			item_count = 1 * ((_celestial.GetCelestialMemorialStruct()._memorial_level - (CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1 + _celestial.GetCelestialNewMaxLevel())) / 2);

			 _celestial.SetCelestialNewMaxLevel(_celestial.GetCelestialNewMaxLevel()+(item_count * 2));			 

		} else 
		{
			if(!_celestial.SetCelestialMemorialRewardPos(type, item_id, pos)) 
				return;
		}		
	}
	else
	{
		return;
	}
	
	// Entrega os itens ao jogador (caso não seja um item do sistema)

	switch (item_id)
	{
		case CelestialMemorialConfig::TICKET_1_ID:
			_celestial.SetCelestialMemorialTicket(0, item_count);
			break;

		case CelestialMemorialConfig::TICKET_2_ID:
			_celestial.SetCelestialMemorialTicket(1, item_count);
			break;

		case CelestialMemorialConfig::PERG_1_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(0, item_count);
			break;

		case CelestialMemorialConfig::PERG_2_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(1, item_count);
			break;

		case CelestialMemorialConfig::PERG_3_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(2, item_count);
			break;
		case CelestialMemorialConfig::PERG_RANDOM_LOTTERY_ID:
		{
			int rand = abase::Rand(0, 2);
			_celestial.SetCelestialMemorialPerg(rand, item_count);

			_runner->celestial_memorial_lottery_count( rand == 0 ? item_count : 0, rand == 1 ? item_count : 0, rand == 2 ? item_count : 0 );
		}	break;
		
		default:
			InvPlayerGiveItem(item_id, item_count);
			break;
	}

	// Atualiza o Protocolo
	ProtocolCelestialMemorial();	
}

void
gplayer_imp::InitCelestialLottery(unsigned int idx_pos_type)
{	
	if(_celestial.GetCelestialMemorialPerg(idx_pos_type) < 1)
	return;

	if(idx_pos_type > 3 || idx_pos_type < 0)
	return;

	_celestial.SetCelestialMemorialPerg2(idx_pos_type, 1);
	_celestial.SetCelestialUsePergProb(idx_pos_type, _celestial.GetCelestialUsePergProb(idx_pos_type)+1);

	CelestialMemorialConfig * conf = CelestialMemorialConfig::GetInstance();
	float prob_low = 0.0f;
	float prob_mid = 0.0f;	
	float prob_add = 0.0f;	
	float prob_rare = 0.0f;	
	int max_itens = CelestialMemorialConfig::MAX_ITEM_LOTTERY;
	int lottery_type = conf->GetLotteryConfig(idx_pos_type)->lottery_draw_lv;
	int lottery_perg_cost = conf->GetLotteryConfig(idx_pos_type)->cost_item_count;

	int item_id = 0;
	int item_count = 0;
	int item_pos = 0;
	int award_times_max = 0;

	int rare_item = 0;
	int rare_item_count = 0;
	
	float prob_rand = (float)((rand()%100)*0.01f);	 

	for(unsigned int i = 0; i < 8; i++)
	{
		if(i < 4)
		{
			award_times_max = conf->GetLotteryConfig(idx_pos_type)->award_pool_middle[i].award_times_max;

			prob_mid += conf->GetLotteryConfig(idx_pos_type)->award_pool_middle[i].award_prob_init;			
			prob_add += (conf->GetLotteryConfig(idx_pos_type)->award_pool_middle[i].award_prob_add * _celestial.GetCelestialUsePergProb(idx_pos_type));

			if((prob_mid + prob_add) >= prob_rand && _celestial.GetCountLottery(idx_pos_type) < award_times_max)
			{
				item_id = conf->GetLotteryConfig(idx_pos_type)->award_pool_middle[i].award_item_id;
				item_count = conf->GetLotteryConfig(idx_pos_type)->award_pool_middle[i].award_item_count;
				item_pos = 9;
				break;
			}
		}

		prob_low += conf->GetLotteryConfig(idx_pos_type)->award_pool_low[i].award_item_probability;	

		if(prob_low >= prob_rand)
		{
			item_id = conf->GetLotteryConfig(idx_pos_type)->award_pool_low[i].award_item_id;
			item_count = conf->GetLotteryConfig(idx_pos_type)->award_pool_low[i].award_item_count;
			item_pos = i;
			break;
		}
	}

	// Aumenta +1 no max reward times
	_celestial.SetCountLottery(idx_pos_type);

	float rare_prob = (float)((rand()%100)*0.01f);	
	prob_rare = conf->GetLotteryConfig(idx_pos_type)->award_pool_high[0].award_item_probability;

	if(prob_rare >= rare_prob)
	{
		rare_item = conf->GetLotteryConfig(idx_pos_type)->award_pool_high[0].award_item_id;
		rare_item_count = conf->GetLotteryConfig(idx_pos_type)->award_pool_high[0].award_item_count;
	}

	if(item_id > 0)
	{
		_celestial.SetCelestialMLottery(idx_pos_type, item_id, item_count, 0);

		if(item_pos != 9)
		_celestial.SetCountStorageReward(idx_pos_type, item_pos);
	}

	if(rare_item > 0)
	{
		_celestial.SetCelestialMLottery2(idx_pos_type, rare_item, rare_item_count, 0);
	}


	// Atualiza o Protocolo
	ProtocolCelestialMemorialLotteryReward(idx_pos_type);
}


void
gplayer_imp::GetCelestiralLotteryReward(unsigned int idx_pos_type)
{
	if(idx_pos_type > 3 || idx_pos_type < 0)
	return;

	int item_id_1 = _celestial.GetCelestialMLottery(idx_pos_type)._item_id_1;
	int item_id_2 = _celestial.GetCelestialMLottery(idx_pos_type)._item_id_2;
	int item_count_1 = _celestial.GetCelestialMLottery(idx_pos_type)._item_count_1;
	int item_count_2 = _celestial.GetCelestialMLottery(idx_pos_type)._item_count_2;

	bool res = false;

	if(item_id_1> 0)
	{
		switch (item_id_1)
		{
		case CelestialMemorialConfig::TICKET_1_ID:
			_celestial.SetCelestialMemorialTicket(0, item_count_1);
			break;

		case CelestialMemorialConfig::TICKET_2_ID:
			_celestial.SetCelestialMemorialTicket(1, item_count_1);
			break;

		case CelestialMemorialConfig::PERG_1_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(0, item_count_1);
			break;

		case CelestialMemorialConfig::PERG_2_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(1, item_count_1);
			break;

		case CelestialMemorialConfig::PERG_3_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(2, item_count_1);
			break;
		case CelestialMemorialConfig::PERG_RANDOM_LOTTERY_ID:
		{
			int rand = abase::Rand(0, 2);
			_celestial.SetCelestialMemorialPerg(rand, item_count_1);
		}	break;
		
		default:
			InvPlayerGiveItem(item_id_1, item_count_1);
			break;
		}

		res = true;

	} 
	
	if (item_id_2 > 0)
	{
		switch (item_id_2)
		{
		case CelestialMemorialConfig::TICKET_1_ID:
			_celestial.SetCelestialMemorialTicket(0, item_count_2);
			break;

		case CelestialMemorialConfig::TICKET_2_ID:
			_celestial.SetCelestialMemorialTicket(1, item_count_2);
			break;

		case CelestialMemorialConfig::PERG_1_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(0, item_count_2);
			break;

		case CelestialMemorialConfig::PERG_2_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(1, item_count_2);
			break;

		case CelestialMemorialConfig::PERG_3_LOTTERY_ID:
			_celestial.SetCelestialMemorialPerg(2, item_count_2);
			break;
		case CelestialMemorialConfig::PERG_RANDOM_LOTTERY_ID:
		{
			int rand = abase::Rand(0, 2);
			_celestial.SetCelestialMemorialPerg(rand, item_count_2);
		}	break;
		
		default:
			InvPlayerGiveItem(item_id_2, item_count_2);
			break;
		}

		res = true;

	}

	if(res)
	{
		_celestial.SetCelestialMLottery(idx_pos_type, 0, 0, 0);
		_celestial.SetCelestialMLottery2(idx_pos_type, 0, 0, 0);
	}

	// Atualiza o Protocolo
	ProtocolCelestialMemorialLotteryReward(idx_pos_type);
}


void gplayer_imp::SetPetSkillTemp(int *skills, int count)
{
	struct SKILLTEMPSTRUCT
	{
		int skill_id;
		int skill_level;
	};

	SKILLTEMPSTRUCT * skilltemp = (SKILLTEMPSTRUCT *)skills;

	for (int i = 0; i < count; i++ )
	{
		_pet_skill_temp.skill_id[i] = skilltemp[i].skill_id;
		_pet_skill_temp.skill_level[i] = skilltemp[i].skill_level;

	}
}


void gplayer_imp::SetPetSkinStorage(int value, int pos)
{
	
	bool res = false;
	if (pos >= 0 && pos < 32)
	{
		_pet_skin.storage[pos] = value;
	}	
}

void gplayer_imp::SetPetSkinStoragePos(int value, int pos)
{
	
	if (pos >= 0 && pos < 32)
	{
		_pet_skin.storage_pos[pos] = value;
	}	
}

void gplayer_imp::SetPetSkinStorageBuy(int value, int pos)
{
    
    if (pos >= 0 && pos < 32)
    {
        _pet_skin.storage_buy[pos] = value;
    }    
}
// new database 

bool gplayer_imp::DBLoadNewData( const GDB::vecdata_new & base )
{
	if ( _parent->ID.id == base.roleid )
	{
		_money.SetSilver(          base.money_silver      	   );
		_pvp_cooldown 			 = base.pk_time           	    ;
		_pvp_enable_flag 		 = base.pk_status         	    ;
		
		_color_name              = base.color_name              ;
		_pvp_rank                = base.pvp_rank                ;
		_pvp_rank_exp            = base.pvp_rank_exp            ;
		_player_kill             = base.player_kill             ;
		_monster_kill            = base.monster_kill            ;
		_player_death            = base.player_death            ;
		_monster_death           = base.monster_death           ;
		_diary_exp               = base.diary_exp               ;
		_realm_day_verify        = base.realm_day_verify        ;
		_verify_itens_valid      = base.verify_itens_valid      ;
		_has_astrolabe_lock      = base.has_astrolabe_lock      ;
		_enabled_fashion_weapon  = base.enabled_fashion_weapon  ;

		_double_factor_exp       = base.double_factor_exp       ;
		_double_factor_sp        = base.double_factor_sp        ;
		_double_factor_realm     = base.double_factor_realm     ;
		
		GDB::DBSGet (base.autoswap         , &_autoswap         , sizeof( _autoswap         ));
		GDB::DBSGet (base.skillsender      , &_skillsender      , sizeof( _skillsender      ));
		GDB::DBSGet (base.glyph            , &_glyph            , sizeof( _glyph            ));
		GDB::DBSGet (base.carrier          , &_carrier          , sizeof( _carrier          ));
		GDB::DBSGet (base.repository       , &_repository       , sizeof( _repository       ));
		GDB::DBSGet (base.pet_skill_temp   , &_pet_skill_temp   , sizeof( _pet_skill_temp   ));
		GDB::DBSGet (base.pet_skin         , &_pet_skin         , sizeof( _pet_skin         ));
		GDB::DBSGet (base.day_world_points , &_day_world_points , sizeof( _day_world_points ));
		GDB::DBSGet (base.activity 		   , &_activity 		, sizeof( _activity		   ));
		
		GDB::DBSGet (base.passwd_safe      , &_passwd_safe      , sizeof( _passwd_safe      ));
		GDB::DBSGet (base.treasure         , &_treasure         , sizeof( _treasure         ));
		GDB::DBSGet (base.lottery          , &_lottery          , sizeof( _lottery          ));
		GDB::DBSGet (base.treasure_items   , &_treasure_items   , sizeof( _treasure_items   ));
		GDB::DBSGet (base.lib_items        , &_lib_items        , sizeof( _lib_items        ));
		GDB::DBSGet (base.celestial        , &_celestial        , sizeof( _celestial        ));
		GDB::DBSGet (base.codex       	   , &_codex        	, sizeof( _codex        ));
		GDB::DBSGet (base.kid       	   , &_kid        		, sizeof( _kid      		));
		GDB::DBSGet (base.kid_addon        , &_kid_addon        , sizeof( _kid_addon        ));

	}

	return true;
}

bool gplayer_imp::DBSaveNewData( GDB::vecdata_new & base )
{
	base.roleid 			 	     = _parent->ID.id		    ;
	{
		base.money_silver 		     = _money.GetSilver();
		base.pk_time      		     = _pvp_cooldown;
		base.pk_status    		     = _pvp_enable_flag;
		
		base.color_name              = _color_name              ;
		base.pvp_rank                = _pvp_rank                ;
		base.pvp_rank_exp            = _pvp_rank_exp            ;
		base.player_kill             = _player_kill             ;
		base.monster_kill            = _monster_kill            ;
		base.player_death            = _player_death            ;
		base.monster_death           = _monster_death           ;
		base.diary_exp               = _diary_exp               ;
		base.realm_day_verify        = _realm_day_verify        ;
		base.verify_itens_valid      = _verify_itens_valid      ;
		base.has_astrolabe_lock      = _has_astrolabe_lock      ;
		base.enabled_fashion_weapon  = _enabled_fashion_weapon  ;

		base.double_factor_exp       = _double_factor_exp       ;
		base.double_factor_sp        = _double_factor_sp        ;
		base.double_factor_realm     = _double_factor_realm     ;
		
		GDB::DBSSet (base.autoswap         , &_autoswap         , sizeof( _autoswap         ));
		GDB::DBSSet (base.skillsender      , &_skillsender      , sizeof( _skillsender      ));
		GDB::DBSSet (base.glyph            , &_glyph            , sizeof( _glyph            ));
		GDB::DBSSet (base.carrier          , &_carrier          , sizeof( _carrier          ));
		GDB::DBSSet (base.repository       , &_repository       , sizeof( _repository       ));
		GDB::DBSSet (base.pet_skill_temp   , &_pet_skill_temp   , sizeof( _pet_skill_temp   ));
		GDB::DBSSet (base.pet_skin         , &_pet_skin         , sizeof( _pet_skin         ));
		GDB::DBSSet (base.day_world_points , &_day_world_points , sizeof( _day_world_points ));
		GDB::DBSSet (base.activity 		   , &_activity 		   , sizeof( _activity		   ));
		
		GDB::DBSSet (base.passwd_safe      , &_passwd_safe      , sizeof( _passwd_safe      ));
		GDB::DBSSet (base.treasure         , &_treasure         , sizeof( _treasure         ));
		GDB::DBSSet (base.lottery          , &_lottery          , sizeof( _lottery          ));
		GDB::DBSSet (base.treasure_items   , &_treasure_items   , sizeof( _treasure_items   ));
		GDB::DBSSet (base.lib_items        , &_lib_items        , sizeof( _lib_items        ));
		GDB::DBSSet (base.celestial        , &_celestial        , sizeof( _celestial        ));
		GDB::DBSSet (base.codex        , &_codex        	, sizeof( _codex        ));
		GDB::DBSSet (base.kid        	   , &_kid        		, sizeof( _kid        		));
		GDB::DBSSet (base.kid_addon        , &_kid_addon        , sizeof( _kid_addon        ));

	}
	return true;
}

void
gplayer_imp::SetGlyphInfo(int type, int count, int feature1, int value1, int feature2, int value2, int feature3, int value3)
{
	struct INFO
	{
		int feature;
		int value;
	}glyphinfo[3];

	if(count == 1)
	{
		glyphinfo[0].feature = feature1;
		glyphinfo[0].value = value1;
	} 
	else if(count == 2)
	{
		glyphinfo[0].feature = feature1;
		glyphinfo[0].value = value1;
		glyphinfo[1].feature = feature2;
		glyphinfo[1].value = value2;
	}
	else if(count == 3)
	{
		glyphinfo[0].feature = feature1;
		glyphinfo[0].value = value1;
		glyphinfo[1].feature = feature2;
		glyphinfo[1].value = value2;
		glyphinfo[2].feature = feature3;
		glyphinfo[2].value = value3;
	}

	if(count > 0)
	_runner->skill_glyph_info(type, count,  (unsigned int*)glyphinfo);
	else 
	_runner->skill_glyph_info(type, count, NULL);
}

void gplayer_imp::SetGlyphRange(int type, unsigned int count, int mode_1, int range_plus_1, int mode_2, int range_plus_2)
{
	struct PRAY
	{
		int feature;
		int info;
	}pray_range[2];

	if(count == 1)
	{
		pray_range[0].feature = mode_1;
		pray_range[0].info = range_plus_1;
	} else if(count == 2)
	{
		pray_range[0].feature = mode_1;
		pray_range[0].info = range_plus_1;
		pray_range[1].feature = mode_2;
		pray_range[1].info = range_plus_2;
	}

	if(count > 0)
	_runner->skill_glyph_info(type, count,  (unsigned int*)pray_range);
	else 
	_runner->skill_glyph_info(type, count, NULL);
	
}

void 
gplayer_imp::ImmuneTurrent(int test)
{
	object_interface obj_if(this);
	if (test ==  1)
	{
		//_skill.SetRedTeamIcon(object_interface(this),1200);
		obj_if.IncImmuneMask(IMMUNEREPEL);
	}
	else
	{
		//_skill.SetRedTeamIcon(object_interface(this),1);
		obj_if.DecImmuneMask(IMMUNEREPEL);
	}
}

// Sistema completo de G17

uint16_t 
gplayer_imp::floatTohalf(float value)
{
    union {
        float f;
        uint32_t i;
    } data;
    data.f = value;

    uint32_t sign = (data.i >> 31) & 0x00000001;
    uint32_t exponent = (data.i >> 23) & 0x000000FF;
    uint32_t fraction = data.i & 0x007FFFFF;

    if (exponent == 0) {
        if (fraction == 0) {
            return static_cast<uint16_t>(sign << 15);
        } else {
            int shift = 126 - 15;
            uint32_t fractionShifted = (fraction | 0x00800000) >> shift;
            return static_cast<uint16_t>((sign << 15) | fractionShifted);
        }
    } else if (exponent == 255) {
        return static_cast<uint16_t>((sign << 15) | 0x7C00 | (fraction >> 13));
    } else {
        int shift = 126 - 15;
        uint32_t exponentShifted = ((exponent - 127 + 15) << 10) & 0x7C00;
        uint32_t fractionShifted = (fraction >> 13) & 0x03FF;
        return static_cast<uint16_t>((sign << 15) | exponentShifted | fractionShifted);
    }
}

int
gplayer_imp::GetClassPos()
{
	int class_pos = -1;
	
	if(GetPlayerClass() == USER_CLASS_SWORDSMAN || GetPlayerClass() == USER_CLASS_ORGE 
	|| GetPlayerClass() == USER_CLASS_BLADE || GetPlayerClass() == USER_CLASS_PALADIN)
	{
		class_pos = 0;
	}
	if(GetPlayerClass() == USER_CLASS_ASN || GetPlayerClass() == USER_CLASS_ARCHER 
	|| GetPlayerClass() == USER_CLASS_SHADOW || GetPlayerClass() == USER_CLASS_SHOOTER || GetPlayerClass() == USER_CLASS_MONKY)
	{
		class_pos = 1;
	}
	if(GetPlayerClass() == USER_CLASS_MAGE || GetPlayerClass() == USER_CLASS_HAG || GetPlayerClass() == USER_CLASS_NEC 
	|| GetPlayerClass() == USER_CLASS_ANGEL || GetPlayerClass() == USER_CLASS_GENIE || GetPlayerClass() == USER_CLASS_FAIRY)
	{
		class_pos = 2;
	}

	return class_pos;
}

bool
gplayer_imp::SetAwakeningSpirit(unsigned int idx_inv, int count)
{
	enum 
	{
		silver_cost_lvl1 = 800000,
		cash_cost_lvl2 = 200,
		cash_cost_lvl3 = 1000,
	};

	if ( idx_inv >= _inventory.Size() )
	{
		return false;
	}
		
	item & it = _inventory[idx_inv];
	if ( it.type <= 0 )
	{
		return false;
	}

	DATA_TYPE dt;
	const QIHUN_COVER_ESSENCE & ess = *(const QIHUN_COVER_ESSENCE *)world_manager::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt);
	if(!&ess && dt != DT_QIHUN_COVER_ESSENCE) return false;

	DATA_TYPE dt2;
	const QIHUN_ESSENCE & ess2 = *(const QIHUN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess.item_id,ID_SPACE_ESSENCE,dt2);
	if(!&ess2 && dt2 != DT_QIHUN_ESSENCE) return false;
	
	
	// 	Verifica se ele tem moedas de prata suficientes
	if(ess2.level == 1 && GetAllMoney() < silver_cost_lvl1)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(ess2.level == 2 && GetMallCash() < cash_cost_lvl2)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(ess2.level == 3 && GetMallCash() < cash_cost_lvl3)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
	
	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	// Remove as moedas
	if(ess2.level < 2)
	{
		SpendAllMoney(silver_cost_lvl1,true);
		SelfPlayerMoney();
	}

	if(ess2.level > 1)
	{
		RemoveMallCash(ess2.level == 2 ? cash_cost_lvl2 : cash_cost_lvl3);
		_runner->player_cash(GetMallCash());
	}
	
	if(it.type) // Remove o material principal
	{
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_inv,_inventory[idx_inv].type,count,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(idx_inv,count);

		PlayerGetItemInfo(IL_INVENTORY,idx_inv);
	}
	
	// Gera e entrega o Espírito
	element_data::item_tag_t tag = {element_data::IMT_SHOP, 0};
	item_data * pItem = world_manager::GetDataMan().generate_item_from_player_g17(ess.item_id, &tag, sizeof(tag),GetClassPos ());
	if(pItem)
	{
		int expire_date = 0;
		
		int ocount = count;
		int rst =_inventory.Push(*pItem,count,expire_date);
		ASSERT(rst >= 0 && count == 0);
			
		_runner->obtain_item(pItem->type,pItem->expire_date,ocount,_inventory[rst].count, 0,rst);
			
		FirstAcquireItem(pItem);					
		FreeItem(pItem);

		if(ess2.level < 2)
		_runner->armor_info_notify(0, 3, 1, silver_cost_lvl1, rst);
		else 
		_runner->armor_info_notify(0, 3, 2, ess2.level == 2 ? cash_cost_lvl2 : cash_cost_lvl3, rst);

		return true;
	}
		
	return false;	
}

bool
gplayer_imp::SetDecomposeSpirit(unsigned int count_itens, int * idx_pos)
{
	int count = 0;
	int decompose_item = 0;

	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	// Soma a quantidade de itens que serão entregues
	for(unsigned int i = 0; i < count_itens; i ++)
	{
		item& it = _inventory[idx_pos[i]];

		if(it.body->GetLockUnlockSpirit() == (char)1)
		{
			_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
			return false;
		}
	
		DATA_TYPE dt;		
		DATA_TYPE dt2;
		DATA_TYPE dt3;
		const QIHUN_ESSENCE & ess2 = *(const QIHUN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt2);
		if(&ess2 && dt2 == DT_QIHUN_ESSENCE)
		{
			count += (ess2.decompose_item[0].num * _inventory[idx_pos[i]].count);
			decompose_item = ess2.decompose_item[0].id;
		} else 
		{
			const QIHUN_COVER_ESSENCE & ess = *(const QIHUN_COVER_ESSENCE *)world_manager::GetDataMan().get_data_ptr(it.type,ID_SPACE_ESSENCE,dt);
			if(&ess && dt == DT_QIHUN_COVER_ESSENCE)
			{
				const QIHUN_ESSENCE & ess3 = *(const QIHUN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess.item_id,ID_SPACE_ESSENCE,dt3);
				if(&ess3 && dt3 == DT_QIHUN_ESSENCE)
				{
					int mutiple = 0;

					if(ess3.level == 1) mutiple = 1;
					if(ess3.level == 2) mutiple = 10;
					if(ess3.level == 3) mutiple = 30;

					count += (_inventory[idx_pos[i]].count * mutiple);
					decompose_item = 60000;
				}				
			}		
		}
	}

	// Remove os itens necessário
	for(unsigned int i = 0; i < count_itens; i ++)
	{
		item& it = _inventory[idx_pos[i]];
		UpdateMallConsumptionDestroying(it.type, it.proc_type, _inventory[idx_pos[i]].count);

		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_pos[i],_inventory[idx_pos[i]].type,_inventory[idx_pos[i]].count,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(idx_pos[i],_inventory[idx_pos[i]].count);
	}

	// Gera e entrega os itens de decomposição
	element_data::item_tag_t tag = {element_data::IMT_SHOP, 0};
	item_data * pItem = world_manager::GetDataMan().generate_item_from_player(decompose_item, &tag, sizeof(tag));
	if(pItem)
	{
		int expire_date = 0;
		
		int ocount = count;
		int rst =_inventory.Push(*pItem,count,expire_date);
		ASSERT(rst >= 0 && count == 0);
		
		_runner->obtain_item(pItem->type,pItem->expire_date,ocount,_inventory[rst].count, 0,rst);
			
		FirstAcquireItem(pItem);					
		FreeItem(pItem);

		_runner->armor_info_notify(4, 0, 0, 0, 0);

		return true;
	}

	return false;
}

bool
gplayer_imp::SetLockSpirit(unsigned int idx_inv, int lock)
{

	if ( idx_inv >= _inventory.Size() )
	{
		return false;
	}

	enum 
	{
		silver_cost_lvl_lock = 1000000,
		silver_cost_lvl_unlock = 9000000,
	};
		
	// 	Verifica se ele tem moedas de prata suficientes
	if(lock == 1 && GetAllMoney() < silver_cost_lvl_lock)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	} else if (lock == 0 && GetAllMoney() < silver_cost_lvl_unlock)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
		
	// Remove as moedas
	SpendAllMoney(lock == 1 ? silver_cost_lvl_lock : silver_cost_lvl_unlock,true);
	SelfPlayerMoney();

	// Bloquea/Desbloqueia
	item& it = _inventory[idx_inv];
	it.body->LockUnlockSpirit((char)lock);
	PlayerGetItemInfo(IL_INVENTORY,idx_inv);

	_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.

	return true;
}

bool
gplayer_imp::SetExtractSpirit(unsigned int idx_inv_rmv, unsigned int idx_inv_rcv)
{

	if ( idx_inv_rmv >= _inventory.Size() )
	{
		return false;
	}

	if ( idx_inv_rcv >= _inventory.Size() )
	{
		return false;
	}

	// Verificações
	item& it = _inventory[idx_inv_rmv];
	item& it_rcv = _inventory[idx_inv_rcv];

	if(it.body->GetLuarSpiritLevel() == (char)0)
	{
		_runner->error_message(501);
		return false;
	}

	if(it_rcv.body->GetLuarSpiritLevel() > it.body->GetLuarSpiritLevel())
	{
		_runner->error_message(501);
		return false;
	}

	if(it.body->GetLockUnlockSpirit() == (char)1 || it_rcv.body->GetLockUnlockSpirit() == (char)1)
	{
		_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
		return false;
	}


	//Verificações
	DATA_TYPE dt;
	const QIHUN_ESSENCE& ess = *(QIHUN_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE, dt));
	if (dt != DT_QIHUN_ESSENCE || !&ess) return false;

	DATA_TYPE dt2;
	const QIHUN_ESSENCE& ess2 = *(QIHUN_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it_rcv.type, ID_SPACE_ESSENCE, dt2));
	if (dt2 != DT_QIHUN_ESSENCE || !&ess2) return false;

	if(ess.equip_mask != ess2.equip_mask)
	{
		_runner->error_message(500); //Espaço Incorreto para Extração da Fase Lunar
		return false;
	}
	
	if(ess.level != ess2.level)
	{
		_runner->error_message(501); //Nível Incorreto para Extração da Fase Lunar
		return false;
	}

	char crystal_slot_level = it.body->GetLuarSpiritLevel();

	// Remove o espírito que perdeu o cristal 
	if(it.type)
	{
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_inv_rmv,_inventory[idx_inv_rmv].type,1,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(idx_inv_rmv,1);

		PlayerGetItemInfo(IL_INVENTORY,idx_inv_rmv);
	}

	// Adiciona o Cristal no Recebedor
	it_rcv.body->TransmitLuarSpirit((char)crystal_slot_level);
	_runner->armor_info_notify(3, 0, 0, 0, 0);

	PlayerGetItemInfo(IL_INVENTORY,idx_inv_rcv);
	return true;
}

bool
gplayer_imp::SetPurificationSpirit(unsigned int idx_inv, int* addons_lock_pos, addon_data * addons_new, addon_data * addons_notify, int addons_count_notify, int count_rem_item)
{
	// Verificações
	item& it = _inventory[idx_inv];

	if ( idx_inv >= _inventory.Size() )
	{
		return false;
	}
		
	if ( it.type <= 0 )
	{
		return false;
	}

	if(it.body->GetLockUnlockSpirit() == (char)1)
	{
		_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
		return false;
	}

	//Buscando o Recipe
	DATA_TYPE dt2;
	const QIHUN_ESSENCE& ess2 = *(QIHUN_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE, dt2));
	if (dt2 != DT_QIHUN_ESSENCE || !&ess2) return false;

	DATA_TYPE dt;
	const RECIPE_ESSENCE* recipe = (RECIPE_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(ess2.id_recipe, ID_SPACE_RECIPE, dt));
	if (dt != DT_RECIPE_ESSENCE || !recipe) return false;

	// Verifica se há espaço no inventário
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}	

	// Remove os materiais usados
	for(unsigned int i = 0; i < 32; i ++)
	{
		if(recipe->materials[i].id)
		{
			// Verifica se existe
			if(!_inventory.IsItemExist(recipe->materials[i].id, recipe->materials[i].num))
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}

			int index = GetInventory(gplayer_imp::IL_INVENTORY).Find(0,recipe->materials[i].id);
			if (index != -1)
			{
				item it	= GetInventory(gplayer_imp::IL_INVENTORY)[index];
				if(it.count >= recipe->materials[i].num)
				{
					UpdateMallConsumptionDestroying(it.type, it.proc_type, recipe->materials[i].num);
					GetInventory(gplayer_imp::IL_INVENTORY).DecAmount(index, recipe->materials[i].num);
					_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,recipe->materials[i].num,S2C::DROP_TYPE_USE);
				}
			}
		}
	}

	// Remove o item de travamento
	if(count_rem_item > 0)
	{
		InvPlayerSpendItem(0, LOCK_ADDON_ITEM, count_rem_item);
	}

	struct
	{
		int addon_id;
		int addon_value;
		int reserve1; 
		int reserve2;
	}newaddons[5];

	int get_addons_count = it.body->GetAddonsSpiritCount();

	int i = 0;
	int newaddons_count = 0;

	while (i < get_addons_count)
	{
		if (addons_notify[i].id != 0)
		{
			newaddons[newaddons_count].addon_id = addons_notify[i].id;
			newaddons[newaddons_count].addon_value = addons_notify[i].arg[0];
			newaddons[newaddons_count].reserve1 = addons_notify[i].arg[1];
			newaddons[newaddons_count].reserve2 = addons_notify[i].arg[2];
			newaddons_count++;
		}

		i++;
	}

	_runner->armor_info_purification(newaddons_count, (unsigned int*)newaddons);		
	return true;
}

bool
gplayer_imp::SetPurificationChangeSpirit(unsigned int idx_inv, int change)
{	
	if (!_cur_session || _cur_session->GetRunTimeClass()->GetGUID() != CLS_SESSION_NEW_ARMOR || _cur_session->_session_id < 0)
	{
		_runner->error_message(S2C::ERR_FATAL_ERR);		
		return false;
	}

	((session_new_armor*)_cur_session)->ChooseItem(change);
}

bool
gplayer_imp::SetPurificationChangeSpiritExec(unsigned int idx_inv, addon_data * addons_new)
{
	if(idx_inv >= _inventory.Size()) return false;
	item & it = _inventory[idx_inv];

	if ( it.type <= 0 )
	{
		return false;
	}
	
	// Addons
	for (int i = 0; i < it.body->GetAddonsSpiritCount(); i++)
	{
		it.body->SetAddonsSpirit(i, addons_new[i].id, addons_new[i].arg[0]);		
	}

	PlayerGetItemInfo(IL_INVENTORY, idx_inv);
	return true;
}


bool
gplayer_imp::SetIncSpiritArmor(unsigned int idx_pos_spirit, unsigned int idx_pos_armor, bool apply)
{	
	// Espírito
	
	if ( idx_pos_spirit >= _inventory.Size() )
	{
		return false;
	}

	if ( idx_pos_armor >= _inventory.Size() )
	{
		return false;
	}

	item& it = _inventory[idx_pos_spirit];
	item& it_armor = _inventory[idx_pos_armor];

	if ( it.type <= 0 || it_armor.type <= 0 )
	{
		return false;
	}

	// Verificações Necessárias
	DATA_TYPE dt;
	const QIHUN_ESSENCE& ess = *(QIHUN_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE, dt));
	if (dt != DT_QIHUN_ESSENCE || !&ess) return false;

	DATA_TYPE dt2;
	const NEW_ARMOR_ESSENCE& ess2 = *(NEW_ARMOR_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it_armor.type, ID_SPACE_ESSENCE, dt2));
	if (dt2 != DT_NEW_ARMOR_ESSENCE || !&ess2) return false;

	DATA_TYPE dtsubtype;
	const NEW_ARMOR_SUB_TYPE& subtype = *(NEW_ARMOR_SUB_TYPE*)(world_manager::GetDataMan().get_data_ptr(ess2.id_sub_type, ID_SPACE_ESSENCE, dtsubtype));
	if (dtsubtype != DT_NEW_ARMOR_SUB_TYPE || !&subtype) return false;

	// Verifica se a Mask é correta
	if(ess.equip_mask != subtype.equip_mask)
	{
		_runner->error_message(502); // Espaço Incorreto para Fusão da Alma do Crepúsculo.
		return false;
	}	

	if(ess.level > ess2.installed_qihun_max_level)
	{
		_runner->error_message(503); // Nível do Espírito é muito alto.
		return false;
	}
	
	// Adiciona as informações do espírito no equipamento
	int count = it.body->GetAddonsSpiritCount();

	for (int i = 0; i < count; i++)
	{
		it_armor.body->SetAddonsSpirit(i, it.body->GetAddonsSpiritid(i), it.body->GetAddonsSpiritarg(i));
	}
	
	it_armor.body->EnabledAddonsSpirit(true); // Habilita/Desabilita
	it_armor.body->SetLuarSpiritid(it.type);
	it_armor.body->SetAddonsSpiritCount(count);
	it_armor.body->SetLuarSpiritLevel(it.body->GetLuarSpiritLevel());
	it_armor.body->LockUnlockSpirit(it.body->GetLockUnlockSpirit());
	
	// Remove o Espírito do Inventário
	if(it.type)
	{
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_pos_spirit,_inventory[idx_pos_spirit].type,1,S2C::DROP_TYPE_PRODUCE);
		_inventory.DecAmount(idx_pos_spirit,1);
	}

	// Atualiza a Armadura
	PlayerGetItemInfo(IL_INVENTORY,idx_pos_armor);

	if(!apply)
	_runner->armor_info_notify(2, 1, 58, 0, 0);

	return true;
}

bool
gplayer_imp::SetDecSpiritArmor(unsigned int idx_pos_armor, bool remove)
{	
	if ( idx_pos_armor >= _inventory.Size() )
	{
		return false;
	}

	item& it_armor = _inventory[idx_pos_armor];

	if ( it_armor.type <= 0 )
	{
		return false;
	}

	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
	int count = 1;
	int item_id = it_armor.body->GetLuarSpiritid();

	if(it_armor.body->GetCrystalId() != 0)
	{
		_runner->error_message(505); // O Cristal do Luar já existe.
		return false;
	}
		
	// Gera e entrega o Espírito
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * it = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(tag));
	if (it)
	{
		it->count = count;
		int pos = GetInventory(0).Push(*it);
		if (pos >= 0)
		{
			item inv_it = GetInventory(0)[pos];
			_runner->obtain_item(it->type,it->expire_date,count,_inventory[pos].count, 0,pos);
		}
		FreeItem(it);

		// Adiciona as informações do espírito no equipamento
		item &new_it = _inventory[pos];
		int newcoount = it_armor.body->GetAddonsSpiritCount();

		char crystal_slot_level = it_armor.body->GetLuarSpiritLevel();

		// Infos
		new_it.body->LockUnlockSpirit(it_armor.body->GetLockUnlockSpirit());
		new_it.body->CrystalLevelEnable((char)1);
		new_it.body->TransmitLuarSpirit((char)crystal_slot_level);

		new_it.body->SetAddonsSpiritCount(newcoount);
		// Addons
		for (int i = 0; i < newcoount; i++)
		{
			new_it.body->SetAddonsSpirit(i, it_armor.body->GetAddonsSpiritid(i), it_armor.body->GetAddonsSpiritarg(i));
		}

		// Salva as informações no banco de dados (caso esteja no inventário)
		if(remove)
		{
			for (unsigned int i = 0; i < 32; i++)
			{
				if(_glua.GetNewArmorSpirit(i)->armor_id == 0)
				{
					_glua.SetNewArmorSpirit(i, it_armor.type, idx_pos_armor, pos);
					break;
				}
			}	
		}

		PlayerGetItemInfo(IL_INVENTORY, pos);
	}
		
	// Remove as informações de espírito
	int count_addons = it_armor.body->GetAddonsSpiritCount();
	for (int i = 0; i < count_addons; i++)
	{
		it_armor.body->SetAddonsSpirit(i, 0, 0);
	}

	it_armor.body->EnabledAddonsSpirit(false);
	it_armor.body->SetLuarSpiritid(0);
	it_armor.body->LockUnlockSpirit(0);
	it_armor.body->SetAddonsSpiritCount(0);
	it_armor.body->SetLuarSpiritLevel(0);
	
	// Atualiza a Armadura
	PlayerGetItemInfo(IL_INVENTORY,idx_pos_armor);
	_runner->armor_info_notify(5, 0, 0, 0, 0);
	
	return true;
}

// Cristal

bool
gplayer_imp::SetIncCrystalArmor(unsigned int idx_pos_crystal, unsigned int idx_pos_armor, bool apply)
{	
	if ( idx_pos_crystal >= _inventory.Size() )
	{
		return false;
	}

	if ( idx_pos_armor >= _inventory.Size() )
	{
		return false;
	}

	// Cristal
	item& it = _inventory[idx_pos_crystal];
	item& it_armor = _inventory[idx_pos_armor];

	if ( it.type <= 0 || it_armor.type <= 0 )
	{
		return false;
	}
		
	if(it_armor.body->GetLuarSpiritid() == 0)
	{
		_runner->error_message(506); //Você deve fundir a Alma do Crepúsculo correta antes de fundir um Cristal do Luar.
		return false;
	}
	
	// Adiciona as informações do espírito no equipamento
	int count = it.body->GetAddonsCrystalCount();
	
	for (int i = 0; i < count; i++)
	{		
		it_armor.body->SetAddonsCrystal(i, it.body->GetAddonsCrystalid(i), it.body->GetAddonsCrystalarg(i));
	}

	it_armor.body->EnabledAddonsCrystal(true); // Habilita/Desabilita
	it_armor.body->SetCrystalId(it.body->GetCrystalId()); // Id do Cristal
	it_armor.body->SetAddonsCrystalCount(count); // Quantidade de Addons
	it_armor.body->SetCrystalLevel(it.body->GetCrystalLevel()); // Level do Cristal
	it_armor.body->SetCrystalColor(it.body->GetCrystalColor()); // Cor do Cristal
	it_armor.body->SetCrystalExp(it.body->GetCrystalExp()); // Experiência do Cristal
	it_armor.body->SetCrystalLock(it.body->GetCrystalLock()); // Bloqueio do Cristal

	for (int i = 0; i < 5; i++)
	{
		it_armor.body->SetCrystalStar(i, it.body->GetCrystalStar(i)); // Estrelas do Cristal
	}	
	
	// Remove o Cristal do Inventário
	if(it.type)
	{
		UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx_pos_crystal,_inventory[idx_pos_crystal].type,1,S2C::DROP_TYPE_PRODUCE);
		_inventory.DecAmount(idx_pos_crystal,1);
	}

	// Atualiza a Armadura
	PlayerGetItemInfo(IL_INVENTORY,idx_pos_armor);

	if(!apply)
	_runner->armor_info_notify(2, 1, 59, 0, 0);

	return true;
}

bool
gplayer_imp::SetDecCrystalArmor(unsigned int idx_pos_armor, bool remove)
{	
	if ( idx_pos_armor >= _inventory.Size() )
	{
		return false;
	}

	item& it_armor = _inventory[idx_pos_armor];

	if ( it_armor.type <= 0 )
	{
		return false;
	}

	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}
		
	int count = 1;
	int item_id = it_armor.body->GetCrystalId();

	// Gera e entrega o Cristal
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0};
	item_data * it = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(tag));
	if (it)
	{
		it->count = count;
		int pos = GetInventory(0).Push(*it);
		if (pos >= 0)
		{
			item inv_it = GetInventory(0)[pos];
			_runner->obtain_item(it->type,it->expire_date,count,_inventory[pos].count, 0,pos);
		}
		FreeItem(it);

		// Adiciona as informações do espírito no equipamento
		item &new_it = _inventory[pos];
		int newcoount = it_armor.body->GetAddonsCrystalCount();

		new_it.body->SetCrystalColor(it_armor.body->GetCrystalColor()); // Cor do Cristal	
		new_it.body->SetCrystalLevel(it_armor.body->GetCrystalLevel()); // Level do Cristal
		new_it.body->SetCrystalExp(it_armor.body->GetCrystalExp()); // Experiência do Cristal
		new_it.body->SetCrystalLock(it_armor.body->GetCrystalLock()); // Bloqueio do Cristal

		for (int i = 0; i < 5; i++)
		{
			new_it.body->SetCrystalStar(i, it_armor.body->GetCrystalStar(i)); // Estrelas do Cristal
		}

		// Addons
		new_it.body->SetAddonsCrystalCount(newcoount);
		for (int i = 0; i < newcoount; i++)
		{
			new_it.body->SetAddonsCrystal(i, it_armor.body->GetAddonsCrystalid(i), it_armor.body->GetAddonsCrystalarg(i));
		}

		// Salva as informações no banco de dados (caso esteja no inventário)
		if(remove)
		{
			for (unsigned int i = 0; i < 32; i++)
			{
				if(_glua.GetNewArmorCrystal(i)->armor_id == 0)
				{
					_glua.SetNewArmorCrystal(i, it_armor.type, idx_pos_armor, pos);
					break;
				}
			}	
		}
		
		PlayerGetItemInfo(IL_INVENTORY, pos);
	}
		
	// Remove as informações de cristal
	int countnew = it_armor.body->GetAddonsCrystalCount();

	for (int i = 0; i < countnew; i++)
	{
		it_armor.body->SetAddonsCrystal(i, 0, 0);
	}
	
	it_armor.body->EnabledAddonsCrystal(false); // Habilita/Desabilita
	it_armor.body->SetAddonsCrystalCount(0); // Quantidade de Addons
	it_armor.body->SetCrystalId(0); // Id do Cristal
	it_armor.body->SetCrystalLevel(0); // Level do Cristal
	it_armor.body->SetCrystalColor(0); // Cor do Cristal
	it_armor.body->SetCrystalExp(0); // Experiência do Cristal
	it_armor.body->SetCrystalLock(0); // Bloqueio do Cristal
	
	// Atualiza a Armadura
	PlayerGetItemInfo(IL_INVENTORY,idx_pos_armor);
	_runner->armor_info_notify(5, 0, 0, 0, 0);

	return true;
}

bool
gplayer_imp::SetLockCrystal(unsigned int idx_inv, int lock)
{

	if ( idx_inv >= _inventory.Size() )
	{
		return false;
	}

	item& it = _inventory[idx_inv];

	if ( it.type <= 0 )
	{
		return false;
	}

	enum 
	{
		silver_cost_lvl_lock = 1000000,
		silver_cost_lvl_unlock = 9000000,
	};
		
	// 	Verifica se ele tem moedas de prata suficientes
	if(lock == 1 && GetAllMoney() < silver_cost_lvl_lock)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	} else if (lock == 0 && GetAllMoney() < silver_cost_lvl_unlock)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}
		
	// Remove as moedas
	SpendAllMoney(lock == 1 ? silver_cost_lvl_lock : silver_cost_lvl_unlock,true);
	SelfPlayerMoney();

	// Bloquea/Desbloqueia	
	it.body->SetCrystalLock((char)lock);
	PlayerGetItemInfo(IL_INVENTORY,idx_inv);

	_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
	return true;
}

bool
gplayer_imp::SetDevourCrystal(unsigned int count_cystals, int * pos_crystal_consum)
{	
	if ( pos_crystal_consum[0] >= _inventory.Size() )
	{
		return false;
	}

	item& it = _inventory[pos_crystal_consum[0]];

	if ( it.type <= 0 )
	{
		return false;
	}

	if(it.body->GetCrystalLock())
	{
		_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
		return false;
	}

	DATA_TYPE dt;
	const QILING_ESSENCE& ess = *(QILING_ESSENCE*)(world_manager::GetDataMan().get_data_ptr(it.type, ID_SPACE_ESSENCE, dt));
	if (dt != DT_QILING_ESSENCE || !&ess)
	{
		return false;
	}

	int max_level = ess.max_level;
	int base_swallow_exp = ess.base_swallow_exp;
	int level = it.body->GetCrystalLevel();
	bool levelup_ok = false;
	int levelup_count = 0;

	// Adiciona o Exp, e evoluí o nível
	int levelup_exp = ess.require_exp[level]; // adicionar o novo nível
	int item_exp = it.body->GetCrystalExp();

	if(levelup_exp == 0 || levelup_exp == -1)
	{
		levelup_exp = 4;
	}
	
	if(level >= max_level)
	{
		_runner->error_message(508); //O Cristal do Luar está no nível máximo.
		return false;
	}
	
	int crystals_exp = 0;
	int new_exp = 0;
	int new_level = 0;
	int temp_exp = 0;

	// Remove os cristais devorados
	for(unsigned int i = 1; i < count_cystals; i ++)
	{
		if ( pos_crystal_consum[i] >= _inventory.Size() )
		{
			return false;
		}
		item& itrem = _inventory[pos_crystal_consum[i]];

		if ( itrem.type <= 0 )
		{
			return false;
		}

		if(itrem.body->GetCrystalLock())
		{
			_runner->error_message(497); //A Alma do Crepúsculo ou o Cristal do Luar foram trancados.
			return false;
		}
		
		crystals_exp += ((itrem.body->GetCrystalLevel() * base_swallow_exp) + itrem.body->GetCrystalExp() + base_swallow_exp);

		UpdateMallConsumptionDestroying(itrem.type, itrem.proc_type, _inventory[pos_crystal_consum[i]].count);

		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,pos_crystal_consum[i],_inventory[pos_crystal_consum[i]].type,_inventory[pos_crystal_consum[i]].count,S2C::DROP_TYPE_USE);
		_inventory.DecAmount(pos_crystal_consum[i],_inventory[pos_crystal_consum[i]].count);
	}

	// Calcula o nível

	if((item_exp + crystals_exp) < levelup_exp) // Adiciona o exp sem evoluir
	{
		new_exp = item_exp + crystals_exp;
		it.body->SetCrystalExp(new_exp);
	}
	else // Evolui o nível
	{
		temp_exp = item_exp + crystals_exp;
		new_level = temp_exp / levelup_exp;

		new_exp = temp_exp - (new_level * levelup_exp);

		new_level += level;

		if(new_level > max_level)
		new_level = max_level;
		
		it.body->SetCrystalLevel((char)new_level);
		it.body->SetCrystalExp(new_exp);

		levelup_ok = true;
		levelup_count = new_level - level;
	}

	if(levelup_ok)
	{
		// Gera aleatóriamente as estrelas
		for (int i = 0; i < levelup_count; i++)
		{
			char generate_color = abase::Rand(0, 4);

			for (int j = 0; j < 5; j++)
			{
				if (it.body->GetCrystalStar(j) >= (char)ess.max_color_level[j])
				{
					_runner->error_message(508); // O Cristal do Luar está no nível máximo.
					return false;
				}

				if (j == generate_color) // Adiciona a estrela
				{
					it.body->SetCrystalStar(j, it.body->GetCrystalStar(j) + (char)1);
				}				
			}
		}

		// Atualiza o Grau
		int get_star = it.body->GetCrystalStar(it.body->GetCrystalColor());

		for (int i = 0; i < 5; i++)
		{
			if (it.body->GetCrystalStar(i) > get_star)
			{
				it.body->SetCrystalColor((char)i);
				break;
			}			
		}
	
	}	

	// Atualiza o Cristal
	PlayerGetItemInfo(IL_INVENTORY,pos_crystal_consum[0]);
	_runner->armor_info_notify(1, 0, 0, 0, 0);
	return true;
}

void 
gplayer_imp::RefreshInventoryNewArmorEnter(bool trade, bool equip)
{
    UpdateInventoryType(_equipment, IL_EQUIPMENT, true);

    if (equip) return;

    UpdateInventoryType(_inventory, IL_INVENTORY, false);

    if (trade) return;

    UpdateInventoryType(GetTrashInventory(IL_TRASH_BOX), IL_TRASH_BOX, false);

    UpdateInventoryType(GetTrashInventory(IL_USER_TRASH_BOX), IL_USER_TRASH_BOX, false);
}

void 
gplayer_imp::UpdateInventoryType(item_list& itemList, int inventoryType, bool refreshEquipment)
{
    unsigned int size = itemList.Size();
    for (unsigned int i = 0; i < size; i++)
    {
        item& it_eq = itemList[i];
        if (it_eq.type != -1 && it_eq.body)
        {
            if (it_eq.body->GetItemType() == item_body::ITEM_TYPE_NEW_ARMOR || 
                it_eq.body->GetItemType() == item_body::ITEM_TYPE_QIHUN ||  
                it_eq.body->GetItemType() == item_body::ITEM_TYPE_QILING)
            {
                std::string ITEM1, ITEM2;
                it_eq.DumpDetail(ITEM1);

                X_EQUIP id0;
                id0.type = it_eq.type;
                id0.mask = it_eq.GetIdModify();

                it_eq.Activate(item::BODY, _equipment, i, this);
                it_eq.DumpDetail(ITEM2);
                it_eq.Deactivate(item::BODY, i, this);

                //it_eq.ForceOnRefreshItem();

                PlayerGetItemInfo(inventoryType, i);
            }
		}
    }

    if (refreshEquipment)
    {
        RefreshEquipment();
    }
}

void
gplayer_imp::AutoGenStatAmulet(int cooldown_idx, int idx ,int offset, bool bSave)
{
	AutoGenStat(cooldown_idx, idx,offset, bSave);
}

// Sistema de Anedota
bool
gplayer_imp::GetTaskFinishExist(short uID)
{

struct FnshedTaskEntryOld
{
	unsigned short m_uTaskId;
};

struct FnshedTaskListHeader
{
	unsigned short		m_uTaskCount;
	unsigned char		m_Version;
	unsigned char		m_Reserved;
};
struct FnshedTaskListOld
{
	union
	{
		struct
		{
			FnshedTaskListHeader	m_FnshHeader;
			FnshedTaskEntryOld		m_aTaskList[TASK_FINISHED_LIST_MAX_LEN];
		};
		unsigned char m_Buf[TASK_FINISHED_LIST_BUF_SIZE_OLD];
	};
};
struct FnshedTaskEntry
{
	unsigned short m_uTaskId;
	unsigned char  m_Mask : 1;
	unsigned char  m_Reserved : 7;
	unsigned char  m_FnshedCount;
};

struct FinishedTaskList
{

public:
	union
	{
		struct
		{
			FnshedTaskListHeader	m_FnshHeader;
			FnshedTaskEntry			m_aTaskList[TASK_FINISHED_LIST_MAX_LEN];
		};
		unsigned char m_Buf[TASK_FINISHED_LIST_BUF_SIZE];
	};
};
	
	PlayerTaskInterface task_if(this);
	FinishedTaskList * items = (FinishedTaskList*)task_if.GetFinishedTaskList();
		
	if( items->m_FnshHeader.m_uTaskCount < TASK_FINISHED_LIST_MAX_LEN && items->m_FnshHeader.m_Version == 1 ) 
	{
		for(unsigned int i = 0; i < items->m_FnshHeader.m_uTaskCount && i < TASK_FINISHED_LIST_MAX_LEN; ++i)
		{
			if(items->m_aTaskList[i].m_uTaskId == 0) continue;

			if(items->m_aTaskList[i].m_uTaskId == uID)
			{
				return true;				
			}
		}
	}
	
	return false;
}

void
gplayer_imp::SetAnecdotePoints()
{
	unsigned int point_quest_id[] = { 31807, 3440, 1388, 31920, 908, 27495, 21202, 1887, 27329, 28953, 3178, 29243, 27434, 3499, 29251, 29287, 2905, 37163, 30639, 
							 30503, 27455, 1608, 1651, 1641, 27452, 1507, 2268, 3472, 27364, 27462, 2369, 2254, 29262, 2531, 2428, 3065, 3307, 25953, 30223, 30195,
							 31291, 33336, 36078, 27352, 2539, 22357, 22386, 2397, 2390, 2462, 1509, 3389, 2288, 22409, 22338, 22412, 27685, 3345, 40006, 37758,
							 37384, 37590, 37289, 41009, 41687, 43259, 44143, 37485, 37488, 45969, 45827 };

	for( unsigned int i = 0; i < sizeof(point_quest_id) / sizeof(int); i++)
	{
		if(GetTaskFinishExist(point_quest_id[i]) && !_glua.CheckAnecdoteTaskid(point_quest_id[i]))
		{
			// Adiciona a quest da anedota no storage
			_glua.SetAnecdoteTaskid((short)point_quest_id[i]);
		}
	}

	ProtcolAnecdotePoints();
}

void
gplayer_imp::ProtcolAnecdotePoints()
{
	int count_points = 0; // Pontos na anedota.

	for( unsigned int i = 0; i < 71; i++)
	{
		if(_glua.GetAnecdoteTaskid(i) != 0)
		{
			count_points++;
		}
	}

	// Protocolo (Modo Arcaico)
	if(count_points == 0)
	{
		_runner->get_anecdote_info(0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0);
		return;
	}

	short arrraaay_value[] = { -1, 0, 8, 24, 56, 120, 248, 504, 1016, 2040, 4088, 8184, 16376, 32760 };

	if(count_points < 13)
	{
		_runner->get_anecdote_info(2,arrraaay_value[count_points],0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	} else if(count_points < 26)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[(int)count_points/2],0,0,0,0,0,0,0,0,0,0,0,0);
	} else if(count_points < 39)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/3],0,0,0,0,0,0,0,0,0,0);
	} else if(count_points < 52)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/4],0,0,0,0,0,0,0,0);
	} else if(count_points < 64)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/5],0,0,0,0,0,0);
	} else if(count_points < 78)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/6],0,0,0,0);
	} else if(count_points < 91)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/7],0,0);
	} else if(count_points < 104)
	{
		_runner->get_anecdote_info(2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[13],2,arrraaay_value[(int)count_points/8]);
	}

}


bool 
gplayer_imp::DecomposeCardS(int inv_idx)
{
	// Item entregue
	int decompose_item = 42190;
	int count = 1;

	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	item_list * pInv = &GetTrashInventory(IL_TRASH_BOX4);

	item & it_inv = (*pInv)[inv_idx];
	if(it_inv.type <= 0 || it_inv.count < count) 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
	DATA_TYPE dt;
	const void * ess = world_manager::GetDataMan().get_data_ptr(it_inv.type, ID_SPACE_ESSENCE, dt);
	if(!ess) return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
		
	bool success = false;

	if(ess)
	{
		UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
		_runner->player_drop_item(IL_TRASH_BOX4, inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
		(*pInv).DecAmount(inv_idx, count);

		success = true;
	}

	if(success) 
	{
		element_data::item_tag_t tag = {element_data::IMT_SHOP, 0};
		item_data *pItem = world_manager::GetDataMan().generate_item_from_player(decompose_item, &tag, sizeof(tag));
		if (pItem)
		{
			int expire_date = 0;

			int ocount = count;
			int rst = _inventory.Push(*pItem, count, expire_date);

			_runner->obtain_item(pItem->type, pItem->expire_date, ocount, _inventory[rst].count, 0, rst);
			
			PlayerGetItemInfo(IL_INVENTORY, rst);

			FirstAcquireItem(pItem);
			FreeItem(pItem);

			return true;
		}
	}

	return false;
}

// Restaure Card
bool 
gplayer_imp::RestaureCardS(int inv_idx)
{
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	item_list * pInv = &GetTrashInventory(IL_TRASH_BOX4);
	int count = 1;

	item & it_inv = (*pInv)[inv_idx];
	if(it_inv.type <= 0 || it_inv.count < count) 
		return S2C::ERR_ITEM_NOT_IN_INVENTORY;
	
	DATA_TYPE dt;
	const void * ess = world_manager::GetDataMan().get_data_ptr(it_inv.type, ID_SPACE_ESSENCE, dt);
	if(!ess) return S2C::ERR_ITEM_NOT_IN_INVENTORY;

	if(it_inv.GetRebirthTimes() == 0)
	{
		PlayerGetItemInfo(IL_TRASH_BOX4, inv_idx);
		return false;
	} 
			
	bool success = false;	
	int for_count = it_inv.GetRebirthTimes() * 2;
	int item_id = it_inv.type;

	if(ess)
	{
		UpdateMallConsumptionDestroying(it_inv.type, it_inv.proc_type, count);
		_runner->player_drop_item(IL_TRASH_BOX4, inv_idx, it_inv.type, count, S2C::DROP_TYPE_TAKEOUT);
		(*pInv).DecAmount(inv_idx, count);

		success = true;
	}

	if(success) 
	{
		for (int i = 0; i < for_count; i++)
		{
			InvPlayerGiveItem(item_id, count);			
		}
	}

	return true;
}


bool 
gplayer_imp::ProduceItemNewArmor(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
{
	if(GetAllMoney() < rt.fee)
	{
		_runner->error_message(S2C::ERR_OUT_OF_FUND);
		return false;
	}

	if(rt.equipment_need_upgrade <= 0) return false;
	if(rt.count != 1) return false;
	if(rt.material_total_count <= 0) return false;

	int item_id = 0;
	if(abase::RandUniform() > rt.null_prob)
	{
		int item_idx = abase::RandSelect(&(rt.targets[0].prob),sizeof(rt.targets[0]), 4);
		item_id = rt.targets[item_idx].id;
	}

	if(item_id <= 0) return false;
	DATA_TYPE target_eq_dt;
	const void * target_eq_ess = world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, target_eq_dt);
	if(!target_eq_dt && target_eq_dt != DT_NEW_ARMOR_ESSENCE) return false;
	
	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	recipe_template::__material mlist[32];
	ASSERT(sizeof(mlist) == sizeof(rt.material_list));
	memcpy(mlist,rt.material_list,sizeof(mlist));

	int bind_result = 0;
	if(rt.bind_type == 1) bind_result = 1;
	int proc_type = 0;
	if(rt.bind_type == 0) proc_type = rt.proc_type;
	if(equip_id != rt.equipment_need_upgrade) return false;
	if(!_inventory.IsItemExist(equip_inv_idx, equip_id, 1)) return false;
	item & eq_it = _inventory[equip_inv_idx];
	if(eq_it.body == NULL) return false;
	
	DATA_TYPE eq_dt;
	const void * eq_ess = world_manager::GetDataMan().get_data_ptr(eq_it.type, ID_SPACE_ESSENCE, eq_dt);
	if(!eq_ess) return false;
		
	int inherit_fee = 0;
	int eq_refine_level = 0;
	int eq_socket_count = 0;
	int eq_stone_type[4] = {0};
	addon_data eq_engrave_addon_list[3];
	unsigned int eq_engrave_addon_count = 0;
	if(inherit_type & PRODUCE_INHERIT_STONE) inherit_type |= PRODUCE_INHERIT_SOCKET;	//��̳б�ʯ����̳п���
	if(rt.inherit_fee_factor < 0.000001f) inherit_type |= (PRODUCE_INHERIT_REFINE | PRODUCE_INHERIT_SOCKET | PRODUCE_INHERIT_STONE); //û�м̳з���ʱ��ǿ�Ƽ̳о���������ʯ
	if(rt.inherit_engrave_fee_factor < 0.000001f) inherit_type |= PRODUCE_INHERIT_ENGRAVE;//û�м̳з���ʱ��ǿ�Ƽ̳��Կ�����
	DATA_TYPE inherit_cfg_dt;
	UPGRADE_PRODUCTION_CONFIG * inherit_cfg_ess = (UPGRADE_PRODUCTION_CONFIG *)world_manager::GetDataMan().get_data_ptr(694, ID_SPACE_CONFIG, inherit_cfg_dt);
	if(!inherit_cfg_ess || inherit_cfg_dt != DT_UPGRADE_PRODUCTION_CONFIG) return false;

	if(inherit_type & PRODUCE_INHERIT_REFINE)
	{
		int material_need;
		int refine_addon = world_manager::GetDataMan().get_item_refine_addon(eq_it.type, material_need);
		if(material_need > 0 && refine_addon > 0)
		{
			eq_refine_level = eq_it.body->GetRefineLevel(refine_addon);
			if(eq_refine_level > 0)
			{
				ASSERT(eq_refine_level <= 12);
				inherit_fee += int(inherit_cfg_ess->num_refine[eq_refine_level-1] * rt.inherit_fee_factor + 0.5f);
			}
		}
	}

	if(inherit_type & PRODUCE_INHERIT_SOCKET)
	{
		eq_socket_count = eq_it.body->GetSocketCount();
		if (eq_socket_count > 0)
		{
			ASSERT(eq_socket_count <= 4 && ((NEW_ARMOR_ESSENCE *)target_eq_ess)->level >= 1 && ((NEW_ARMOR_ESSENCE *)target_eq_ess)->level <= 20);
			inherit_fee += int(inherit_cfg_ess->num_armor[eq_socket_count - 1][((NEW_ARMOR_ESSENCE *)target_eq_ess)->level - 1] * rt.inherit_fee_factor + 0.5f);
		}
	}

	if(inherit_type & PRODUCE_INHERIT_STONE)
	{
		if(eq_socket_count > 0)
		{
			for(int i=0; i<eq_socket_count; i++)
			{
				int stone_type = eq_it.body->GetSocketType(i);
				if(stone_type <= 0) continue;
				DATA_TYPE dt;
				STONE_ESSENCE * stone_ess = (STONE_ESSENCE *) world_manager::GetDataMan().get_data_ptr(stone_type,ID_SPACE_ESSENCE,dt);
				ASSERT(dt == DT_STONE_ESSENCE && stone_ess);
				ASSERT(stone_ess->level >= 1 && stone_ess->level <= 20);
				inherit_fee += int(inherit_cfg_ess->num_stone[stone_ess->level-1] * rt.inherit_fee_factor + 0.5f);
				eq_stone_type[i] = stone_type;
			}
		}
	}

	if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
	{
		eq_engrave_addon_count = eq_it.GetEngraveAddon(&eq_engrave_addon_list[0], sizeof(eq_engrave_addon_list)/sizeof(eq_engrave_addon_list[0]));
		if(eq_engrave_addon_count > 0)
		{
			ASSERT(eq_engrave_addon_count <= 3);
			inherit_fee += int(inherit_cfg_ess->num_engrave[eq_engrave_addon_count-1] * rt.inherit_engrave_fee_factor + 0.5f);
		}	
	}

	if(inherit_fee > 0 && !CheckItemExist(ALLSPARK_ID,inherit_fee)) 
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		return false; 
	}

	if(rt.bind_type == 2)
	{
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND)
		{
			if(eq_it.proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
				bind_result = 3;
			else
				bind_result = 1;
		}
		if(eq_it.proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
			bind_result = 2;
	}

	std::set<int> unique_idx;
	for(unsigned int i = 0;i < 16; i++)
	{
		if(rt.material_list[i].item_id)
		{
			if(rt.material_list[i].item_id != materials[i]) return false;
			int idx = idxs[i];
			if(!_inventory.IsItemExist(idx, materials[i], rt.material_list[i].count)
					|| !unique_idx.insert(idx).second)
			{
				_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
				return false;
			}
			mlist[i].item_id = materials[i];
			mlist[i].count = rt.material_list[i].count;

			if(rt.bind_type == 2)
			{
				int proc_type = _inventory[idx].proc_type;
				if( proc_type & item::ITEM_PROC_TYPE_BIND)
				{
					if(proc_type & item::ITEM_PROC_TYPE_CAN_WEBTRADE && bind_result != 1)
						bind_result = 3;
					else
						bind_result = 1;
				}
				if(proc_type & item::ITEM_PROC_TYPE_BIND2 && bind_result != 1 && bind_result != 3) 
					bind_result = 2;
			}
		}
	}

	item_data * data = NULL;
	if(item_id)
	{
		struct 
		{
			char type;
			char size;
			char name[MAX_USERNAME_LENGTH];
		} tag;
		tag.type = element_data::IMT_PRODUCE;
		unsigned int  len;
		const void * name;
		name = GetPlayerName(len);
		if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
		memcpy(tag.name,name,len);
		tag.size = len;

		data = world_manager::GetDataMan().generate_item_from_player(item_id,&tag,sizeof(short) + len);
		if(!data)
		{
			_runner->error_message(S2C::ERR_PRODUCE_FAILED);
			return false;
		}
		data->count = rt.count;

        char buf[128] = {0};
        std::string itembuf;

        for (unsigned int i = 0; i < 16; ++i)
        {
            if (mlist[i].item_id > 0)
            {
                itembuf += buf;
            }
        }

        itembuf += buf;

    }

	if(data)
	{
		if(rt.exp || rt.sp)
		{
			msg_exp_t expdata = {rt.level,rt.exp,rt.sp};
			SendTo<0>(GM_MSG_EXPERIENCE,_parent->ID,0,&expdata,sizeof(expdata));
		}

		if(rt.produce_skill > 0)
		{
			int skill_level = GetSkillLevel(rt.produce_skill);
			if(skill_level  < rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 2);
			}
			else if(skill_level  == rt.recipe_level)
			{
				IncSkillAbility(rt.produce_skill, 1);
			}
		}
	}

	SpendAllMoney(rt.fee,true);
	SelfPlayerMoney();

	item& it = _inventory[equip_inv_idx];
	UpdateMallConsumptionDestroying(it.type, it.proc_type, 1);
	
	_runner->player_drop_item(gplayer_imp::IL_INVENTORY,equip_inv_idx,equip_id,1,S2C::DROP_TYPE_PRODUCE);
	_inventory.DecAmount(equip_inv_idx, 1);

	if(inherit_fee > 0)
		RemoveItems(ALLSPARK_ID, inherit_fee, S2C::DROP_TYPE_USE, true);

	for(unsigned int i = 0; i < 16; i ++)
	{
		if(mlist[i].item_id) 
		{
			int idx = idxs[i];

			item& it = _inventory[idx];
			UpdateMallConsumptionDestroying(it.type, it.proc_type, mlist[idx].count);

			_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,mlist[i].count,S2C::DROP_TYPE_PRODUCE);
			_inventory.DecAmount(idx, mlist[i].count);
		}
	}
	if(data)
	{
		if(data->pile_limit > 1) 
		{
			bind_result = 0;
			proc_type = 0;
		}
		if(bind_result == 1)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND;
		} 
		else if(bind_result == 2)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_BIND2;
		}
		else if(bind_result == 3)
		{
			data->proc_type |= item::ITEM_PROC_TYPE_NODROP |
				item::ITEM_PROC_TYPE_NOTHROW|
				item::ITEM_PROC_TYPE_NOSELL |
				item::ITEM_PROC_TYPE_NOTRADE|
				item::ITEM_PROC_TYPE_BIND |
				item::ITEM_PROC_TYPE_CAN_WEBTRADE;
		}
		else if(rt.bind_type == 0)
		{
			data->proc_type |= proc_type;	
		}
		int rst = _inventory.Push(*data);
		if(rst >=0)
		{
		    FirstAcquireItem(data);


			item & target_eq_it = _inventory[rst];
			if(inherit_type & PRODUCE_INHERIT_REFINE)
			{
				if(eq_refine_level > 0)
				{
					int material_need;
					int refine_addon = world_manager::GetDataMan().get_item_refine_addon(target_eq_it.type, material_need);
					if(material_need > 0 && refine_addon > 0)
						target_eq_it.body->SetRefineLevel(refine_addon,eq_refine_level);		
				}

			}

			if(inherit_type & PRODUCE_INHERIT_SOCKET)
			{
				if(eq_socket_count > 0)
				{
					target_eq_it.body->SetSocketAndStone(eq_socket_count,eq_stone_type);	
				}
			}
			if(inherit_type & PRODUCE_INHERIT_ENGRAVE)
			{
				if(eq_engrave_addon_count > 0)
				{
					target_eq_it.Engrave(&eq_engrave_addon_list[0], eq_engrave_addon_count);
				}
			}

			_runner->produce_once(item_id,rt.count - data->count,_inventory[rst].count, 0,rst);
		}

		if(data->count)
		{
			DropItemData(_plane,_parent->pos,data,_parent->ID,0,0,0);
			_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
			return false;
		}
		FreeItem(data);
	}
	else
	{
		_runner->produce_null(rt.recipe_id);
	}

	/*161+*/
	RefreshInventoryNewArmorEnter(true);
	return true;
}

// Imperial Spirit
void gplayer_imp::ImperialSpirtHeartBeat(bool is_active)
{
    int now = g_timer.get_systime();
    int expire_time = _glua.GetImperialExpireTime();

    if (is_active)
    {
        if (now > expire_time)
        {
            _glua.SetImperialEnabled(0);
			_runner->get_imperial_spirit(0, 0);

        } else 
		{
			_glua.SetImperialEnabled(1);
		}
    }
    else
    {
        _glua.SetImperialEnabled(0);
    }
	
	if(expire_time > 0 && expire_time > now)
    _runner->get_imperial_spirit(_glua.GetImperialEnabled(), expire_time);
}

void gplayer_imp::SetImperialSpirtActivate()
{
    enum
    {
        IMPERIAL_SPIRIT_ITEM_ID = 57645,
        IMPERIAL_MAX_DAYS_ACUMULATE = 300,
        IMPERIAL_SECOND_TIME = 86400,
        IMPERIAL_BASIC_DAYS = 30,
    };

    int now = g_timer.get_systime();
    int expire_time = _glua.GetImperialExpireTime();
    int expire_date = (now > expire_time ? now + (IMPERIAL_BASIC_DAYS * IMPERIAL_SECOND_TIME) : expire_time + (IMPERIAL_BASIC_DAYS * IMPERIAL_SECOND_TIME));

    if (expire_date - now + (IMPERIAL_BASIC_DAYS * IMPERIAL_SECOND_TIME) > (IMPERIAL_MAX_DAYS_ACUMULATE * IMPERIAL_SECOND_TIME))
    {
        expire_date = now + (IMPERIAL_MAX_DAYS_ACUMULATE * IMPERIAL_SECOND_TIME);
    }
	
	if(!CheckItemExist(IMPERIAL_SPIRIT_ITEM_ID,1)) 
	{
		_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
		return;
	}	
	RemoveItems(IMPERIAL_SPIRIT_ITEM_ID,1, S2C::DROP_TYPE_USE, true);	

    _glua.SetImperialEnabled(1);
    _glua.SetImperialExpireTime(expire_date);

	for(int i = item::EQUIP_INDEX_GENERALCARD1; i <= item::EQUIP_INDEX_GENERALCARD6; i++)
	{
		PlayerGetItemInfo(IL_TRASH_BOX4, i);
	}
    RefreshCardEquipment();

	_runner->get_imperial_spirit(_glua.GetImperialEnabled(), expire_date);
}

/*176+*/

void 
gplayer_imp::SetPortatilPicture(int idx)
{
	_glua.SetPortatilPictureActive(idx);	

	_runner->reward_interface_notify(1, 251, _glua.GetPortatilPictureActive());
	_runner->portatil_picture_info(_parent->ID.id,0,0,_glua.GetPortatilPictureActive(),0);
}

void 
gplayer_imp::ProtocolPortatilPicture()
{
	// Adiciona a interface clássica
	if(_glua.GetPortatilPictureStorage(0) == 0)
	{
		_glua.SetPortatilPictureStorage(0, 74891);
	}

	int count = 0;
	int storage[4];

	for (int i = 0; i < 6; i++)
	{
		if(_glua.GetPortatilPictureStorage(i) > 0)
		{
			count++;
			storage[i] = _glua.GetPortatilPictureStorage(i);
		}
	}

	_runner->portatil_picture_storage(count, storage);

	if(_glua.GetPortatilPictureActive() > 0)
	{
		_runner->reward_interface_notify(1, 251, _glua.GetPortatilPictureActive());
		_runner->portatil_picture_info(_parent->ID.id,0,0,_glua.GetPortatilPictureActive(),0);
	}
}

/*170+ Códice*/
void gplayer_imp::FashionUpdateActivate(bool enter_world)
{
	packet_wrapper h1(16);
	
	struct FASHION
	{
		int fashion_id;
		int fashion_color;
	};
	FASHION fashions[5];
	memset(&fashions,0,sizeof(fashions));	
	
	if(!enter_world || enter_world)
	{
		for (unsigned int i = 0; i < 5; i++)
		{
			if(_codex.GetCodexActivateFashion()->fashions[i].fashion_id > 0)
			{	
				fashions[i].fashion_id = _codex.GetCodexActivateFashion()->fashions[i].fashion_id;
				fashions[i].fashion_color = _codex.GetCodexActivateFashion()->fashions[i].fashion_color;
			}
		}
	}
			
	h1.push_back(&fashions, sizeof(fashions));	
	_runner->codex_fashion_activate(_parent->ID.id, _codex.GetCodexActivateFashion()->mode_classic, h1.size(), h1.data());

}

bool gplayer_imp::FashionCodexApplyStorageSingle(int idx)
{
	bool res = false;
	item& it = _inventory[idx];
	int fashion_id = it.type;
	int get_color = it.body->GetDataCRC();

	// Protocolo
	packet_wrapper h1(16);
	struct FASHION
	{
		int fashion_id;
		int fashion_color;
		int fashion_pos;
	};

	if (idx >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}
	if(fashion_id <= 0)
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}

	int get_pos = 0;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
	{
		if(_codex.GetCodexFashions(i)->fashion_id == fashion_id)
		{
			get_pos = _codex.GetCodexFashions(i)->fashion_count_pos;
			break;
		}		
	}
	
	int newpos = 0;	
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
	{
		if(_codex.GetCodexFashions(i)->fashion_id == 0)
		{			
			newpos = i;
			break;
		}		
	}

	// Protocolo
	FASHION newfashions[1];
	memset(&newfashions, 0, sizeof(newfashions));

	if(get_pos == 0) // Adiciona um novo
	{	
		// Protocolo	
		newfashions[0].fashion_id = fashion_id;
		newfashions[0].fashion_color = get_color;
		newfashions[0].fashion_pos = get_pos+1;

		_codex.SetCodexFashions(newpos, fashion_id, get_color, get_pos+1);

		res = true;
	}
	
	if(res)
	{		
		// Remove a roupa do inventário
		int count = 1;	
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,count,S2C::DROP_TYPE_FASHION_CODEX);
		_inventory.DecAmount(idx, count);

		_runner->error_message(S2C::ERR_SUCCESS);

		// Envia o Protocolo
		h1.push_back(&newfashions, sizeof(newfashions));
		_runner->codex_fashion_storage(1, h1.size(), h1.data(), _codex.GetCodexFashionsPoints());
	}
	return res;
}

bool gplayer_imp::FashionCodexChange(int idx, int fashion_id, int color)
{
	bool res = false;
	item& it = _inventory[idx];
	int get_color = color;

	// Protocolo
	packet_wrapper h1(16);
	struct FASHION
	{
		int fashion_id;
		int fashion_color;
		int fashion_pos;
	};

	if (idx >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}
	if(fashion_id <= 0)
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}

	int get_pos = 0;
	int newpos = -1;

	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
	{
		if(_codex.GetCodexFashions(i)->fashion_id == fashion_id)
		{
			get_pos = _codex.GetCodexFashions(i)->fashion_count_pos;
			newpos = i;
			break;
		}		
	}
	
	// Protocolo
	FASHION newfashions[1];
	memset(&newfashions, 0, sizeof(newfashions));

	if(get_pos > -1)
	{	
		// Protocolo	
		newfashions[0].fashion_id = fashion_id;
		newfashions[0].fashion_color = get_color;
		newfashions[0].fashion_pos = get_pos;

		_codex.SetNewColorFashion(newpos, get_color, get_pos);
		res = true;
	}
	
	if(res)
	{		
		// Remove a roupa do inventário
		int count = 1;	
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,count,S2C::DROP_TYPE_FASHION_CODEX);
		_inventory.DecAmount(idx, count);

		_runner->error_message(S2C::ERR_SUCCESS);

		// Envia o Protocolo
		h1.push_back(&newfashions, sizeof(newfashions));
		_runner->codex_fashion_storage(1, h1.size(), h1.data(), _codex.GetCodexFashionsPoints());
	}
	return res;
}

unsigned short gplayer_imp::GetCodexColorGenerate(int item_id)
{
	DATA_TYPE dt3;
	const FASHION_ESSENCE &ess_color = *(const FASHION_ESSENCE *)world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, dt3);
	ASSERT(&ess_color && dt3 == DT_FASHION_ESSENCE);

	unsigned short color = 0;
	float h, s, v;
	h = abase::Rand(ess_color.h_min, ess_color.h_max);
	s = abase::Rand((float)pow(ess_color.s_min, 2), (float)pow(ess_color.s_max, 2));
	v = abase::Rand((float)pow(ess_color.v_min, 3), (float)pow(ess_color.v_max, 3));

	s = sqrtf(s);
	v = pow(v, 1.0 / 3);

	if (s > ess_color.s_max)
		s = ess_color.s_max;
	else if (s < ess_color.s_min)
		s = ess_color.s_min;
	if (v > ess_color.v_max)
		v = ess_color.v_max;
	else if (v < ess_color.v_min)
		v = ess_color.v_min;

	int color_tmp = hsv2rgb(h, s, v);

	unsigned short r = ((color_tmp >> 16) >> 3) & 0x1F;
	unsigned short g = ((color_tmp >> 8) >> 3) & 0x1F;
	unsigned short b = (color_tmp >> 3) & 0x1F;
	color = ((r << 10) | (g << 5) | b) & 0x7FFF;

	return color;
}

bool gplayer_imp::FashionCodexApplyStorageMutiple (int idx, int * fashions_pos)
{
	bool res = false;
	item& it = _inventory[idx];
	int illustrated_fashion_id = it.type;
	int count = 0;
	int count_fashions = 0;
	int pos_begin = 0;
	int fashion_pos = 0;

	// Protocolo
	packet_wrapper h1(16);
	struct FASHION
	{
		int fashion_id;
		int fashion_color;
		int fashion_pos;
	};	

	if (idx >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}
	if(illustrated_fashion_id <= 0)
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}

	if(_codex.GetCodexFashionsCount() >= gplayer_codex::MAX_STORAGE_FASHION)
	{
		_runner->error_message(S2C::ERR_CODEX_MAX_COLLECTIBLES_CAPACITY_REACHED);
		return false;
	}

	DATA_TYPE dt;
	const ILLUSTRATED_FASHION_ESSENCE & ess = *(const ILLUSTRATED_FASHION_ESSENCE *)world_manager::GetDataMan().get_data_ptr(illustrated_fashion_id,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_ILLUSTRATED_FASHION_ESSENCE);

	DATA_TYPE dt2;
	const FASHION_SUITE_ESSENCE & ess2 = *(const FASHION_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess.id_fashion_suite,ID_SPACE_ESSENCE,dt2);
	ASSERT(&ess2 && dt2 == DT_FASHION_SUITE_ESSENCE);

	for (int y = 0; y < 6; y++)
	{
		if (ess2.list[y] > 0)
		{
			count_fashions++;
		}		
	}

	if(count_fashions + _codex.GetCodexFashionsCount() > gplayer_codex::MAX_STORAGE_FASHION) 
	{
		_runner->error_message(S2C::ERR_CODEX_MAX_COLLECTIBLES_CAPACITY_REACHED);
		return false;
	}

	// Protocolo
	FASHION newfashions[6];
	memset(&newfashions, 0, sizeof(newfashions));

	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
	{
		if(ess2.list[1] == _codex.GetCodexFashions(i)->fashion_id)
		{			
			fashion_pos = _codex.GetCodexFashions(i)->fashion_count_pos;
		}			
	}

	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
	{
		if (_codex.GetCodexFashions(i)->fashion_id == 0)
		{
			pos_begin = i;
			break;
		}			
	}

	unsigned short color = GetCodexColorGenerate(ess2.list[1]);
	
	if (fashion_pos == 0) // Adiciona um novo
	{
		for (unsigned int j = 0; j < 6; j++)
		{
			if (ess2.list[j] > 0)
			{		
				// Protocolo	
				newfashions[j].fashion_id = ess2.list[j];
				newfashions[j].fashion_color = color;
				newfashions[j].fashion_pos = fashion_pos+1;

				_codex.SetCodexFashions(pos_begin++, ess2.list[j], color, fashion_pos+1);
			}
		}

		res = true;
	} else if (fashion_pos >= 1 && fashion_pos <= 3) // Adiciona um existente ( nova cor )
	{
		for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
		{
			if(ess2.list[0] == _codex.GetCodexFashions(i)->fashion_id)
			{
				pos_begin = i;
				break;
			}				
		}
		for (unsigned int j = 0; j < 6; j++)
		{
			if (ess2.list[j] > 0)
			{	
				// Protocolo		
				newfashions[j].fashion_id = ess2.list[j];
				newfashions[j].fashion_color = color;
				newfashions[j].fashion_pos = fashion_pos+1;

				_codex.SetNewColorFashion(pos_begin++, color, fashion_pos+1);
			}
		}
		res = true;

	} else  // Substitui os existentes
	{
		for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FASHION; i++)
		{
			if(ess2.list[0] == _codex.GetCodexFashions(i)->fashion_id)
			{
				pos_begin = i;
				break;
			}				
		}

		for (unsigned int j = 0; j < 6; j++)
		{
			if (ess2.list[j] > 0)
			{			
				// Protocolo		
				newfashions[j].fashion_id = ess2.list[j];
				newfashions[j].fashion_color = color;
				newfashions[j].fashion_pos = fashions_pos[0];

				_codex.SetNewColorFashion(pos_begin++, color, fashions_pos[0]);
			}
		}
	
		res = true;
	} 

	// Libera o quadro do avatar /*176+*/
	/*bool newres = false;
	if(ess2.award_portraits_id > 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if(_glua.GetPortatilPictureStorage(i) == ess2.award_portraits_id)
			{
				newres = true;
				break;
			}
		}
			
		if(!newres)
		{
			for (int i = 0; i < 6; i++)
			{
				if(_glua.GetPortatilPictureStorage(i) == 0)
				{
					_glua.SetPortatilPictureStorage(i, ess2.award_portraits_id);
					ProtocolPortatilPicture();
					break;
				}
			}
		}
	}*/
	
	if(res)
	{
		// Adiciona os pontos
		/*if(ess2.charm_value > 0)
		{		
			_codex.SetCodexFashionPoints(ess2.charm_value);
		}*/

		// Remove a roupa do inventário
		int count = 1;	
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,count,S2C::DROP_TYPE_FASHION_CODEX);
		_inventory.DecAmount(idx, count);

		_runner->error_message(S2C::ERR_SUCCESS);


		// Envia o Protocolo
		h1.push_back(&newfashions, sizeof(newfashions));
		_runner->codex_fashion_storage(6, h1.size(), h1.data(), _codex.GetCodexFashionsPoints());
	}
	return res;
}

bool gplayer_imp::FashionCodexSetActivate (unsigned int size, const void * buf)
{
	bool res = false;
	C2S::CMD::codex_fashion_equip & activate = *(C2S::CMD::codex_fashion_equip*)buf;

	if(GetInventory().GetEmptySlotCount() < 6)
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	static const int FASHION_EQUIP_LIST[] =
	{
		item::EQUIP_INDEX_FASHION_BODY,
		item::EQUIP_INDEX_FASHION_LEG,
		item::EQUIP_INDEX_FASHION_FOOT,
		item::EQUIP_INDEX_FASHION_WRIST,
		item::EQUIP_INDEX_FASHION_HEAD,
	};

	// Remove
	if(activate.fashion_count == 1)
	{
		if (_equipment[FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos]].type > 0)
		{
			int t1 = _equipment[FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos]].type;
			int proc_type1 = _equipment[FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos]].proc_type;

			_equipment.Remove(FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos]);
			UpdateMallConsumptionDestroying(t1, proc_type1, 1);
			_runner->player_drop_item(gplayer_imp::IL_EQUIPMENT, FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos], t1, 1, S2C::DROP_TYPE_FLYSWORD_CODEX);

			_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, FASHION_EQUIP_LIST[activate.fashions[0].fashion_pos]);
		}

		_codex.SetCodexActivateFashionUnique(activate.fashions[0].fashion_pos);

		FashionUpdateActivate(false);
		return true;
	}

	for (int i = 0; i < sizeof(FASHION_EQUIP_LIST) / sizeof(FASHION_EQUIP_LIST[0]); ++i)
	{
		if (_codex.GetCodexActivateFashion()->fashions[i].fashion_id == _equipment[FASHION_EQUIP_LIST[i]].type)
		{
			_codex.SetCodexActivateFashionUnique(i);

			int t1 = _equipment[FASHION_EQUIP_LIST[i]].type;
			int proc_type1 = _equipment[FASHION_EQUIP_LIST[i]].proc_type;

			_equipment.Remove(FASHION_EQUIP_LIST[i]);
			UpdateMallConsumptionDestroying(t1, proc_type1, 1);
			_runner->player_drop_item(gplayer_imp::IL_EQUIPMENT, FASHION_EQUIP_LIST[i], t1, 1, S2C::DROP_TYPE_FLYSWORD_CODEX);
		
			_runner->unlock_inventory_slot(gplayer_imp::IL_EQUIPMENT, FASHION_EQUIP_LIST[i]);
		}
	}
	
	struct
	{
		int fashion_id;
		int fashion_color;
	}fashions[5];

	for (unsigned int i = 0; i < 5; i++)
	{
		fashions[i].fashion_id = activate.fashions[i].fashion_id;
		fashions[i].fashion_color = activate.fashions[i].fashion_color;
	}

	if (_codex.SetCodexActivateFashion(/*176+ activate.classic_mode*/0, (int *)fashions))
	{
		res = true;
	}

	if(res)
	{
		for (int j = 0; j < sizeof(FASHION_EQUIP_LIST) / sizeof(FASHION_EQUIP_LIST[0]); ++j)
		{
			if (activate.fashions[j].fashion_id > 0)
			{
				element_data::item_tag_t tag = {element_data::IMT_CREATE, 0};
				item_data *it = world_manager::GetDataMan().generate_item_from_player(activate.fashions[j].fashion_id, &tag, sizeof(tag));

				int count = 1;
				if (it)
				{
					it->count = count;
					int pos = GetInventory(0).Push(*it);
					if (pos >= 0)
					{						
						item inv_it = GetInventory(0)[pos];
						inv_it.DyeItem(activate.fashions[j].fashion_color);
					}
					FreeItem(it);

					if (pos >= 0)
					{
						PlayerEquipItem(pos, FASHION_EQUIP_LIST[j]);
						_runner->unlock_inventory_slot(IL_INVENTORY, pos);
						_runner->unlock_inventory_slot(IL_EQUIPMENT, FASHION_EQUIP_LIST[j]);
					}
				}				
			}
		}

		FashionUpdateActivate(false);
	}

	return res;
}
unsigned short gplayer_imp::GetDyeCodexColorGenerate(int item_id)
{
	DATA_TYPE dt3;
	const DYE_TICKET_ESSENCE &ess_color = *(const DYE_TICKET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(item_id, ID_SPACE_ESSENCE, dt3);
	ASSERT(&ess_color && dt3 == DT_DYE_TICKET_ESSENCE);

	unsigned short color = 0;
	float h, s, v;
	h = abase::Rand(ess_color.h_min, ess_color.h_max);
	s = abase::Rand((float)pow(ess_color.s_min, 2), (float)pow(ess_color.s_max, 2));
	v = abase::Rand((float)pow(ess_color.v_min, 3), (float)pow(ess_color.v_max, 3));

	s = sqrtf(s);
	v = pow(v, 1.0 / 3);

	if (s > ess_color.s_max)
		s = ess_color.s_max;
	else if (s < ess_color.s_min)
		s = ess_color.s_min;
	if (v > ess_color.v_max)
		v = ess_color.v_max;
	else if (v < ess_color.v_min)
		v = ess_color.v_min;

	int color_tmp = hsv2rgb(h, s, v);

	unsigned short r = ((color_tmp >> 16) >> 3) & 0x1F;
	unsigned short g = ((color_tmp >> 8) >> 3) & 0x1F;
	unsigned short b = (color_tmp >> 3) & 0x1F;
	color = ((r << 10) | (g << 5) | b) & 0x7FFF;

	return color;
}


int  
gplayer_imp::CodexDyeFashionItem(int mode, int fashion_count, int dye_index, int * fashions)
{
	if(dye_index >= _inventory.Size()) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;

	item & dye_item = _inventory[dye_index];
	if(dye_item.type == -1) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;

	bool res = false;
	int require_dye_count = 0;
	
	itemdataman & dataman =  world_manager::GetDataMan();
	DATA_TYPE dt;
	DYE_TICKET_ESSENCE * dess = (DYE_TICKET_ESSENCE*)dataman.get_data_ptr(dye_item.type, ID_SPACE_ESSENCE,dt);
	if(dess == NULL || dt != DT_DYE_TICKET_ESSENCE) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;

	if (dess->usage != 0) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;

	unsigned short new_color = GetDyeCodexColorGenerate(dye_item.type);

	struct GET_FASHION
	{
		int fashion_id;
		int fashion_pos;
	};
	GET_FASHION * getfashions = (GET_FASHION*)fashions;

	for (int i = 0; i < fashion_count; i++)
	{
		DATA_TYPE dt2;
		FASHION_ESSENCE * fess = (FASHION_ESSENCE*)dataman.get_data_ptr(getfashions[i].fashion_id, ID_SPACE_ESSENCE,dt2);
		if(fess == NULL || dt2 != DT_FASHION_ESSENCE) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;	

		require_dye_count += fess->require_dye_count;	
		res = true;	
	}

	if(require_dye_count <= 0) return S2C::ERR_FASHION_CAN_NOT_BE_DYED;
	if(!_inventory.IsItemExist(dye_item.type, require_dye_count)) return S2C::ERR_DYE_NOT_ENOUGH;

	if(!res) return S2C::ERR_CODEX_NOT_FOUND_IN_CODEX;

	if(res)
	{
		for (int i = 0; i < fashion_count; i++)
		{
			DATA_TYPE dt2;
			FASHION_ESSENCE * fess = (FASHION_ESSENCE*)dataman.get_data_ptr(getfashions[i].fashion_id, ID_SPACE_ESSENCE,dt2);
			if(fess == NULL || dt2 != DT_FASHION_ESSENCE) return S2C::ERR_CODEX_CANNOT_DYE_FASHION;	

			for (unsigned int y = 0; y < gplayer_codex::MAX_STORAGE_FASHION; y++)
			{
				if(getfashions[i].fashion_id == _codex.GetCodexFashions(y)->fashion_id)
				{
					_codex.SetNewDyeColorFashion(y, new_color, getfashions[i].fashion_pos);
				}
			}
		}

		RemoveItems(dye_item.type,require_dye_count, S2C::DROP_TYPE_FASHION_CODEX, true);

		// Protocolo de Tintura
		packet_wrapper h1(16);

		struct FASHION
		{
			int fashion_id;
			int fashion_pos;
		};
		FASHION newfashions[fashion_count];
		memset(&newfashions, 0, sizeof(newfashions));

		for (unsigned int i = 0; i < fashion_count; i++)
		{
			newfashions[i].fashion_id = getfashions[i].fashion_id;
			newfashions[i].fashion_pos = getfashions[i].fashion_pos;
		}

		h1.push_back(&newfashions, sizeof(newfashions));
		_runner->codex_fashion_dye(new_color, fashion_count, h1.size(), h1.data());
	}
	
	return 0;
}

bool gplayer_imp::CodexRewardTitle (int type)
{
	bool res = false;
	int newpos = 0;
	CodexConfig * conf = CodexConfig::GetInstance();
	
	switch (type)
	{
		case 0:
		{
			newpos = _codex.GetCodexFashionsTitleReward();	
			if(newpos > CodexConfig::COUNT_MAX_TITLE_REWARD) 
				return false;

			_codex.SetCodexFashionsTitleReward();
			res = true;			
			InvPlayerGiveItem(conf->GetTitleReward(0)->award_item_id[newpos], 1);

		}break;
		case 1:
		{
			newpos = _codex.GetCodexMountsTitleReward();	
			if(newpos > CodexConfig::COUNT_MAX_TITLE_REWARD) 
				return false;

			_codex.SetCodexMountsTitleReward();
			res = true;
			InvPlayerGiveItem(conf->GetTitleReward(1)->award_item_id[newpos], 1);

		}break;
		case 2:
		{
			newpos = _codex.GetCodexPetsTitleReward();	
			if(newpos > CodexConfig::COUNT_MAX_TITLE_REWARD) 
				return false;
					
			_codex.SetCodexPetsTitleReward();
			res = true;
			InvPlayerGiveItem(conf->GetTitleReward(3)->award_item_id[newpos], 1);

		}break;
		case 3:
		{		
			newpos = _codex.GetCodexFlysTitleReward();	
			if(newpos > CodexConfig::COUNT_MAX_TITLE_REWARD) 
				return false;

			_codex.SetCodexFlysTitleReward();
			res = true;
			InvPlayerGiveItem(conf->GetTitleReward(2)->award_item_id[newpos], 1);

		}break;
		default:
			return false;
	}

	if(res)
	{
		_runner->codex_reward_title(type, newpos+1);
	}

	return res;
}

bool gplayer_imp::CodexRewardFirst (int type, int idx)
{
	bool res = false;
	int newpos = 0;
	CodexConfig * conf = CodexConfig::GetInstance();

	return res;
	/*
	switch (type)
	{
		case 0:
		{	
			DATA_TYPE dt;
			ILLUSTRATED_FASHION_SERIES_CONFIG *it = (ILLUSTRATED_FASHION_SERIES_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx, ID_SPACE_CONFIG, dt);
			if (it && dt == DT_ILLUSTRATED_FASHION_SERIES_CONFIG)
			{			
				newpos = _codex.GetCodexFashionsFirstReward();	
				if(newpos > 0) return false;

				_codex.SetCodexFashionsFirstReward(idx);
				InvPlayerGiveItem(it->award_item_id, it->award_item_count);
			}
			res = true;			

		}break;
		case 1:
		{		

			DATA_TYPE dt3;
			ILLUSTRATED_PET_SERIES_CONFIG *it = (ILLUSTRATED_PET_SERIES_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx, ID_SPACE_CONFIG, dt3);
			if (it && dt3 == DT_ILLUSTRATED_PET_SERIES_CONFIG)
			{
				newpos = _codex.GetCodexMountsFirstReward();	
				if(newpos > 0) return false;

				_codex.SetCodexMountsFirstReward(idx);
				InvPlayerGiveItem(it->award_item_id, it->award_item_count);
				res = true;
			}
			res = true;

		}break;
		case 2:
		{
			DATA_TYPE dt2;
			ILLUSTRATED_WING_SERIES_CONFIG *it = (ILLUSTRATED_WING_SERIES_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx, ID_SPACE_CONFIG, dt2);
			if (it && dt2 == DT_ILLUSTRATED_WING_SERIES_CONFIG)
			{
				newpos = _codex.GetCodexFlysFirstReward();	
				if(newpos > 0) return false;

				_codex.SetCodexFlysFirstReward(idx);
				InvPlayerGiveItem(it->award_item_id, it->award_item_count);
			}
			res = true;

		}break;	
		default:
			return false;
	}

	if(res)
	{
		_runner->codex_reward_first(type, idx, 1);
	}

	return res;
	*/
}

bool gplayer_imp::PetCodexApplyStorageSingle(int idx)
{
	enum 
	{
		PET_TYPE_MOUNT = 8781,
	};

	bool res = false;
	item& it = _inventory[idx];
	int pet_id = it.type;
	int get_color = 0;

	if (idx >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}
	if(pet_id <= 0)
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}

	DATA_TYPE dt;
	const PET_EGG_ESSENCE & ess = *(const PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_id,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_PET_EGG_ESSENCE);

	DATA_TYPE dt2;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess.id_pet, ID_SPACE_ESSENCE, dt2);
	if(petess == NULL || dt2 != DT_PET_ESSENCE)	return false;

	int get_type = petess->id_type;

	int get_pos = 0;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_MOUNT; i++)
	{
		if(get_type == PET_TYPE_MOUNT)
		{
			if(_codex.GetCodexMounts(i)->mount_id == pet_id)
			{
				get_pos++;
				break;
			}
		} else 
		{
			if(_codex.GetCodexPets(i)->pet_id == pet_id)
			{
				get_pos++;
				break;
			}
		}		
	}

	if(get_pos > 0) return false;
	
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_MOUNT; i++)
	{
		if(get_type == PET_TYPE_MOUNT)
		{
			if(_codex.GetCodexMounts(i)->mount_id == 0)
			{
				_codex.SetCodexMounts(i, pet_id, get_color, 1);
				res = true;
				break;
			}
		} else 
		{
			if(_codex.GetCodexPets(i)->pet_id == 0)
			{
				_codex.SetCodexPets(i, pet_id, get_color, 1);
				res = true;
				break;
			}
		}		
	}
	
	if(res)
	{
		// Adiciona os pontos
		/*if(ess.pet_charm_value > 0 && get_type == PET_TYPE_MOUNT)
		{		
			_codex.SetCodexMountsPoints(ess.pet_charm_value);
		}*/

		// Remove a roupa do inventário
		int count = 1;	
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,count,S2C::DROP_TYPE_FASHION_CODEX);
		_inventory.DecAmount(idx, count);

		// Protocolos de sucesso
		_runner->error_message(S2C::ERR_SUCCESS);
		_runner->codex_pet_consum_info(pet_id, 0, 0, _codex.GetCodexMountsPoints());
	}
	
	return res;
}

bool gplayer_imp::FlyCodexApplyStorageSingle(int idx)
{
	bool res = false;
	item& it = _inventory[idx];
	int fly_id = it.type;
	int get_color = 0;

	if (idx >= _inventory.Size())
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}
	if(fly_id <= 0)
	{
		_runner->error_message(S2C::ERR_CODEX_COLLECTION_FAILED);	
	 	return false;
	}

	DATA_TYPE dt;
	const FLYSWORD_ESSENCE & ess = *(const FLYSWORD_ESSENCE *)world_manager::GetDataMan().get_data_ptr(fly_id,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_FLYSWORD_ESSENCE);

	int get_pos = 0;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FLY; i++)
	{
		if (_codex.GetCodexFlys(i)->fly_id == fly_id)
		{
			get_pos++;
			break;
		}
	}

	if(get_pos > 0) return false;
	
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FLY; i++)
	{
		if(_codex.GetCodexFlys(i)->fly_id == 0)
		{
			_codex.SetCodexFlys(i, fly_id, 0, static_cast<unsigned int>(ess.time_max_max));
			res = true;
			break;
		}		
	}
	
	if(res)
	{
		// Adiciona os pontos
		/*if(ess.fly_charm_value > 0)
		{		
			_codex.SetCodexFlysPoints(ess.fly_charm_value);
		}*/

		// Remove a roupa do inventário
		int count = 1;	
		UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
		_runner->player_drop_item(gplayer_imp::IL_INVENTORY,idx,_inventory[idx].type,count,S2C::DROP_TYPE_FASHION_CODEX);
		_inventory.DecAmount(idx, count);

		// Protocolos de sucesso
		_runner->error_message(S2C::ERR_SUCCESS);
		_runner->codex_fly_consum_info(fly_id, 0, static_cast<unsigned int>(ess.time_max_max), _codex.GetCodexFlysPoints());
	}
	
	return res;
}

int gplayer_imp::CodexPlayerImproveFlysword(int mode, int fashion_count, int dye_index, int * fashions)
{
	struct FLYS
	{
		int fly_old;
		int fly_new;
	}fly[1];

	const FLYS * newfly = (FLYS *)fashions;

	DATA_TYPE dt;
	FLYSWORD_ESSENCE * ess = (FLYSWORD_ESSENCE *)world_manager::GetDataMan().get_data_ptr(newfly[0].fly_old, ID_SPACE_ESSENCE, dt);
	if(ess == NULL || dt != DT_FLYSWORD_ESSENCE) return S2C::ERR_CODEX_BACKPACK_FULL_FLYING_MOUNT;
	if(ess->max_improve_level <= 0 || (unsigned int)ess->max_improve_level > sizeof(ess->improve_config)/sizeof(ess->improve_config[0])) return S2C::ERR_CODEX_FLYING_MOUNT_ALREADY_EQUIPPED;

	int ilevel = -1;
	int posaprimored = -1;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FLY; i++)
	{
		if(_codex.GetCodexFlys(i)->fly_id == newfly[0].fly_old)
		{
			ilevel = (int)_codex.GetCodexFlys(i)->apprimored_level;
			posaprimored = i;
			break;
		}
	}

	// Verificações
	if(ilevel < 0) return S2C::ERR_CODEX_FLYING_MOUNT_ALREADY_EQUIPPED;
	if(posaprimored < 0) return S2C::ERR_CODEX_FLYING_MOUNT_ALREADY_EQUIPPED;
	if(ilevel >= ess->max_improve_level) return S2C::ERR_CODEX_FLYING_MOUNT_ALREADY_EQUIPPED;

	// Soma de itens necessários e verificações
	unsigned int icost = ess->improve_config[ilevel].require_item_num;
	int ticket_list[] = {IMPROVE_FLYSWORD_TICKET_ID1, IMPROVE_FLYSWORD_TICKET_ID2, IMPROVE_FLYSWORD_TICKET_ID3};
	if(icost == 0 || !CheckItemExist(ticket_list, sizeof(ticket_list)/sizeof(int), icost)) return S2C::ERR_NOT_ENOUGH_MATERIAL; 

	// Adiciona mais um nível de aprimoramento
	_codex.AddApprimoredLevel(posaprimored);

	// Remove os itens necessários
	TakeOutItem(ticket_list, sizeof(ticket_list)/sizeof(int), icost);
	_runner->codex_improve_info(_codex.GetCodexFlys(posaprimored)->fly_id, _codex.GetCodexFlys(posaprimored)->apprimored_level, _codex.GetCodexFlys(posaprimored)->max_time_speed); 

	return 0;
}

// Equipa o Voo
bool gplayer_imp::FlyCodexChangeEquip(int fly_id)
{
	bool res = true;
	if(fly_id <= 0) return false;

	if(!GetInventory().GetEmptySlotCount())
	{
		_runner->error_message(S2C::ERR_INVENTORY_IS_FULL);
		return false;
	}

	DATA_TYPE dt;
	const FLYSWORD_ESSENCE & ess = *(const FLYSWORD_ESSENCE *)world_manager::GetDataMan().get_data_ptr(fly_id,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_FLYSWORD_ESSENCE);

	int get_pos = -1;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_FLY; i++)
	{
		if (_codex.GetCodexFlys(i)->fly_id == fly_id)
		{
			get_pos = i;
			break;
		}
	}

	if(get_pos < 0) return false;	
	int count = 1;

	bool active = false;
	item &it_old = _equipment[item::EQUIP_INDEX_FLYSWORD];
	if (it_old.type > 0)
	{
		if (_codex.GetFlyActive() == it_old.type)
		{
			active = true;
		}

		if (active)
		{
			_codex.SetFlyActive(0);

			int t1 = _equipment[item::EQUIP_INDEX_FLYSWORD].type;
			int proc_type1 = _equipment[item::EQUIP_INDEX_FLYSWORD].proc_type;

			_equipment.Remove(item::EQUIP_INDEX_FLYSWORD);
			UpdateMallConsumptionDestroying(t1, proc_type1, 1);
			_runner->player_drop_item(gplayer_imp::IL_EQUIPMENT, item::EQUIP_INDEX_FLYSWORD, t1, 1, S2C::DROP_TYPE_FLYSWORD_CODEX);
		}
	}

	int idx_inv = -1;
	element_data::item_tag_t tag = {element_data::IMT_CREATE, 0};
	item_data *it = world_manager::GetDataMan().generate_item_from_player(fly_id, &tag, sizeof(tag));
	if (it)
	{
		it->count = count;
		int pos = GetInventory(0).Push(*it);
		if (pos >= 0)
		{
			//item inv_it = GetInventory(0)[pos];
			//_runner->pickup_item(it->type, it->expire_date, count - it->count, inv_it.count, 0, pos);
			idx_inv = pos;			
		}
		FreeItem(it);

		// Adiciona as novas velocidades
		item & newseed = _inventory[idx_inv];
		if(newseed.type > 0)
		{
			newseed.body->SetFlyTime(_codex.GetCodexFlys(get_pos)->max_time_speed);
			int ilevel = _codex.GetCodexFlys(get_pos)->apprimored_level;

			for (int i = 0; i < ilevel; i++)
			{
				if (!newseed.FlyswordImprove(ess.improve_config[i].speed_increase, ess.improve_config[i].speed_rush_increase))
				{
					res = false;
					break;
				}
			}
		}
		
	}

	if(idx_inv > -1)
	{
		PlayerEquipItem(idx_inv, item::EQUIP_INDEX_FLYSWORD);
		_runner->unlock_inventory_slot(IL_INVENTORY, idx_inv);
		_runner->unlock_inventory_slot(IL_EQUIPMENT, item::EQUIP_INDEX_FLYSWORD);
	}

	// Adiciona no voo ativo
	_codex.SetFlyActive(fly_id);

	// Protocolo de Notificação
	_runner->codex_fly_equip_info(item::EQUIP_INDEX_FLYSWORD, 0, 0, /*size*/9, _codex.GetCodexFlys(get_pos)->fly_id,  _codex.GetCodexFlys(get_pos)->max_time_speed,  _codex.GetCodexFlys(get_pos)->apprimored_level);
	return res;
}

bool 
gplayer_imp::CodexAddPetToSlot(int pet_egg_id)
{
	DATA_TYPE datatype;
	PET_EGG_ESSENCE * ess = (PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(pet_egg_id, ID_SPACE_ESSENCE, datatype);
	if(ess == NULL || datatype != DT_PET_EGG_ESSENCE)	return false;

	DATA_TYPE datatype1;
	PET_ESSENCE * petess = (PET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(ess->id_pet, ID_SPACE_ESSENCE, datatype1);
	if(petess == NULL || datatype1 != DT_PET_ESSENCE)	return false;

	pet_data data;
	memset(&data,0,sizeof(data));
	data.honor_point = pet_data::HONOR_POINT_MAX;
	data.hunger_gauge = pet_data::HUNGER_LEVEL_1;
	data.pet_tid = ess->id_pet;
	data.pet_vis_tid = 0;
	data.pet_egg_tid = pet_egg_id;
	data.pet_class = petess->id_type == 8781 ? pet_data::PET_CLASS_MOUNT : pet_data::PET_CLASS_FOLLOW;
	data.hp_factor = 1.0f;
	data.level = petess->level_max;
	data.color = 0;
	data.exp = ess->exp;
	data.skill_point = ess->skill_point;

	int get_pos = -1;

	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_PET; i++)
	{
		if(_codex.GetCodexPets(i)->pet_id == pet_egg_id)
		{
			get_pos = i;
			break;
		}
	}

	data.name_len = _codex.GetCodexNamePetLen(get_pos);
	if(data.name_len)
	{
		memcpy(data.name,_codex.GetCodexNamePet(get_pos),data.name_len);
	}

	unsigned int skill_count = 0;
	for (unsigned int i = 0; i < pet_data::MAX_PET_SKILL_COUNT; i++)
	{
		if(ess->skills[i].id_skill > 0) skill_count++;
	}
	
	for(unsigned int i = 0; i < skill_count && i < pet_data::MAX_PET_SKILL_COUNT; i ++)
	{
		data.skills[i].skill = ess->skills[i].id_skill;
		data.skills[i].level = ess->skills[i].level;
	}

	if(data.pet_class == pet_data::PET_CLASS_SUMMON 
			|| data.pet_class == pet_data::PET_CLASS_PLANT) return false;

	int index = _petman.AddPetData(data, true);
	if(index >= 0)
	{
		_runner->gain_pet(index, &data,sizeof(data));
		return true;
	}
	else
	{
		return false;
	}
}

bool
gplayer_imp::ConvertPetToCodex(unsigned int slot_index)
{
	if(slot_index >= pet_manager::MAX_PET_CAPACITY) 
	{
		_runner->error_message(S2C::ERR_PET_CAN_NOT_BE_RESTORED);
		return false;
	}
	
	pet_data * pData = _petman.GetPetData(slot_index);
	if(!pData)
	{
		_runner->error_message(S2C::ERR_CODEX_PET_ACTIVATED_CANNOT_BE_COLLECTED);
		return false;
	}

	if(_petman.GetCurActivePet() == (int)slot_index)
	{
		_runner->error_message(S2C::ERR_CODEX_CANNOT_RECEIVE_CODEX_REWARD);
		return false;
	}

	DATA_TYPE dt;
	PET_EGG_ESSENCE * ess;
	ess = (PET_EGG_ESSENCE*) world_manager::GetDataMan().get_data_ptr(pData->pet_egg_tid, ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_PET_EGG_ESSENCE)
	{
		_runner->error_message(S2C::ERR_PET_CAN_NOT_BE_RESTORED);
		return false;
	}

	_petman.FreePet(this,slot_index);
	return true;
}

bool 
gplayer_imp::CodexChangePetName(unsigned int pet_id,const char name[] , unsigned int name_len)
{	
	int pos = -1;
	bool res = true;
	for (unsigned int i = 0; i < gplayer_codex::MAX_STORAGE_MOUNT; i++)
	{
		if(_codex.GetCodexPets(i)->pet_id == pet_id)
		{
			pos = i;
			break;
		}
	}

	if(!CheckItemExist(12403,1)) 
	{
		_runner->error_message(S2C::ERR_NOT_ENOUGH_MATERIAL);
		res = false;
		return false; 
	}

	if(pos == -1)
	{
		_runner->error_message(S2C::ERR_CODEX_TIME_LIMITED_ITEM);
		res = false;
		return false;
	}

	// Altera o nome do pet
	if(!_codex.SetCodexNamePet(pos, name, name_len))
	{
		_runner->error_message(S2C::ERR_CODEX_TIME_LIMITED_ITEM);
		res = false;
		return false;
	}
	
	// Remove o item
	if(res)
	{
		if (!InvPlayerSpendItem(0 , 12403, 1))
		{
			_runner->error_message(S2C::ERR_ITEM_NOT_IN_INVENTORY);
			return false;
		}

		_runner->codex_rename_pet_info(pet_id, _codex.GetCodexNamePet(pos), _codex.GetCodexNamePetLen(pos));	
	}

	return true;
}

bool 
gplayer_imp::CheckProctypeCodexCondition(unsigned int index)
{
	if(index >= _inventory.Size()) return true;
	const item & it = _inventory[index];
	if(it.type <= 0) return true;
	
	if(it.proc_type == item::ITEM_PROC_NO_CODEX_USE_MASK ||
	it.proc_type == item::ITEM_PROC_NO_CODEX_USE_MASK2)
	{
		return false;
	}	
	return true;
}

int 
gplayer_imp::GetEquipSlotCount()
{
	return _equipment[item::EQUIP_INDEX_DYNSKILL0].count;
}


/*170+ Bebe Celestial*/
void 
gplayer_imp::KidAwakeningNameProtocol()
{
	_runner->kid_name_awakening(_kid.GetNameLength(), _kid.GetName());
}

void 
gplayer_imp::KidAwakeningInfoProtocol()
{
	if(_kid.GetNameLength() <= 0) return;

	packet_wrapper h1(512);

#pragma pack(push, 1)
	struct AWAKENING_INFO
	{
		struct COURSE_INFO
		{
			int course_id;
			char course_level;
		};

		int course_info[gplayer_kid::MAX_RANDOM_COURSE];
		char course_level;
		char course_random_cost;
		int exp_course_required;
		COURSE_INFO course_equip[gplayer_kid::MAX_EQUIPED_COURSE];
		COURSE_INFO course_storage[gplayer_kid::MAX_STORAGE_COURSE];
		char name[gplayer_kid::MAX_NAME_LENGTH];
		char type;
		int points_awakening;
		int days_awakening;
		char enabled_day;
		int cash_awakening;
		char block_day;
		char reserve5;
	};
#pragma pack(pop)

	AWAKENING_INFO getinfo;
	memset(&getinfo, 0, sizeof(getinfo));

	for (unsigned int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; i++)
	{
		getinfo.course_info[i] = _kid.GetRandomCourse(i);
	}

	getinfo.course_level = _kid.GetCardLevel();
	getinfo.course_random_cost = _kid.GetCourseRandomCost();
		
	int exp_min_level[] = { 0, 0, 2, 6, 14, 26, 46, 66, 86, 106 };
	getinfo.exp_course_required = _kid.GetExpCourseRequired () + exp_min_level[_kid.GetCardLevel()];

	for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++)
	{
		getinfo.course_equip[i].course_id = _kid.GetEquipedCourse(i)->course_id;
		getinfo.course_equip[i].course_level = _kid.GetEquipedCourse(i)->course_level;
	}

	for (unsigned int i = 0; i < gplayer_kid::MAX_STORAGE_COURSE; i++)
	{
		getinfo.course_storage[i].course_id = _kid.GetStorageCourse(i)->course_id;
		getinfo.course_storage[i].course_level = _kid.GetStorageCourse(i)->course_level;
	}

	memcpy(getinfo.name, _kid.GetName(), gplayer_kid::MAX_NAME_LENGTH);
	
	if(!_kid.GetType()) 
		getinfo.type = 4; 
	else 
		getinfo.type = 1; 

	getinfo.points_awakening = _kid.GetPointsAwakening();
	getinfo.days_awakening = _kid.GetAwakeningDayCount();

	getinfo.enabled_day = _kid.IsAwakening();
	getinfo.cash_awakening = _kid.GetAwakeningCash();
	getinfo.block_day = _kid.IsBlockDay();

	getinfo.reserve5 = true;

	h1.push_back(&getinfo, sizeof(getinfo));
	_runner->kid_awakening_info(h1.size(), h1.data());
}

void 
gplayer_imp::KidCelestialInfoProtocol(int type)
{
	packet_wrapper h1(512);
	struct KID_INFO
	{
		int level;
		int rank;
		int exp;
		int idx;
		int atk;
		int atk_mag;
		int def;
		int def_mag[5];
		int hp;
		int crit;
	};
	
	int count = gplayer_kid::MAX_CELESTIAL;

	KID_INFO _kid_info[gplayer_kid::MAX_CELESTIAL];	
	for (unsigned int i = 0; i < gplayer_kid::MAX_CELESTIAL; i++)
	{
		_kid_info[i].level = _kid.GetCelestial(i)->level;
		_kid_info[i].rank = _kid.GetCelestial(i)->rank;
		_kid_info[i].exp = _kid.GetCelestial(i)->exp;
		_kid_info[i].idx = _kid.GetCelestial(i)->idx;

		int level = _kid_info[i].level;
		int rank = _kid_info[i].rank;

		DATA_TYPE data;
		const KID_PROPERTY_CONFIG *config = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(_kid_info[i].idx, ID_SPACE_CONFIG, data);
		if (config || data == DT_KID_PROPERTY_CONFIG)
		{
			float mutiple_val = 0.0f; 

			DATA_TYPE data2;
			const KID_UPGRADE_STAR_CONFIG *config2 = (const KID_UPGRADE_STAR_CONFIG *)world_manager::GetDataMan().get_data_ptr(gplayer_kid::IDX_KID_STAR_CONFIG, ID_SPACE_CONFIG, data2);
			if (config2 || data2 == DT_KID_UPGRADE_STAR_CONFIG)
			{
				if(rank > 0)
				{
					mutiple_val += config2->upgrade_star_info[rank-1].star_param;
				} else 
				{
					mutiple_val +=  config2->zero_star_param;
				}	
			}

			_kid_info[i].atk = ((config->damage * level) * mutiple_val);
			_kid_info[i].atk_mag = ((config->magic_damage * level) * mutiple_val);
			_kid_info[i].def = ((config->defence * level) * mutiple_val);
	
			for (unsigned int y = 0; y < 5; y++)
			{
				_kid_info[i].def_mag[y] = ((config->magic_defence * level) * mutiple_val);
			}

			_kid_info[i].hp = ((config->hp * level) * mutiple_val);
			_kid_info[i].crit = (static_cast<int>((config->crit_hit_probability * 100) * level) * mutiple_val);					
		}	
	}

	h1 << type << count;
	h1.push_back(&_kid_info, sizeof(_kid_info));
	
	_runner->kid_celestial_info(h1.size(), h1.data());
}


void 
gplayer_imp::KidAwakeningCashProtocol()
{
	_runner->kid_awakening_cash_info(_kid.GetAwakeningCash(), _kid.GetAwakeningPotential());
}

void 
gplayer_imp::KidAwakeningPercProtocol()
{
	_runner->kid_course_perc(_kid.GetCardLevel (), 0);
}

static const int card_level_1[] = { 66225, 66227, 66228, 66229,	66230, 66231 };
static const int card_level_2[] = { 66232, 66233, 66234, 66235, 66236, 66237, 66238, 66239 };
static const int card_level_3[] = { 66240, 66241, 66242, 66243, 66244, 66245, 66246 };
static const int card_level_4[] = { 66247, 66248, 66249, 66250, 66251, 66252, 66253 };
static const int card_level_5[] = { 66254 };

int 
gplayer_imp::GetCardLevel(std::mt19937& rng, std::uniform_real_distribution<double>& dist, int level) 
{
    double random_value = dist(rng) * 100.0;

    if (level == 1) {
        return (random_value <= 70) ? card_level_1[rand() % 6] : card_level_2[rand() % 8];
    } else if (level == 2) {
        return (random_value <= 60) ? card_level_1[rand() % 6] : ((random_value <= 95) ? card_level_2[rand() % 8] : card_level_3[rand() % 7]);
    } else if (level == 3) {
        return (random_value <= 50) ? card_level_1[rand() % 6] : ((random_value <= 90) ? card_level_2[rand() % 8] : card_level_3[rand() % 7]);
    } else if (level == 4) {
        return (random_value <= 40) ? card_level_1[rand() % 6] : ((random_value <= 80) ? card_level_2[rand() % 8] : card_level_3[rand() % 7]);
    } else if (level == 5) {
        return (random_value <= 30) ? card_level_1[rand() % 6] : ((random_value <= 65) ? card_level_2[rand() % 8] : ((random_value <= 95) ? card_level_3[rand() % 7] : card_level_4[rand() % 7]));
    } else if (level == 6) {
        return (random_value <= 30) ? card_level_1[rand() % 6] : ((random_value <= 60) ? card_level_2[rand() % 8] : ((random_value <= 90) ? card_level_3[rand() % 7] : card_level_4[rand() % 7]));
    } else if (level == 7) {
        return (random_value <= 24) ? card_level_1[rand() % 6] : ((random_value <= 54) ? card_level_2[rand() % 8] : ((random_value <= 84) ? card_level_3[rand() % 7] : ((random_value <= 99) ? card_level_4[rand() % 7] : card_level_5[0])));
    } else if (level == 8) {
        return (random_value <= 17) ? card_level_1[rand() % 6] : ((random_value <= 47) ? card_level_2[rand() % 8] : ((random_value <= 77) ? card_level_3[rand() % 7] : ((random_value <= 97) ? card_level_4[rand() % 7] : card_level_5[0])));
    } else if (level == 9) {
        return (random_value <= 15) ? card_level_1[rand() % 6] : ((random_value <= 40) ? card_level_2[rand() % 8] : ((random_value <= 70) ? card_level_3[rand() % 7] : ((random_value <= 95) ? card_level_4[rand() % 7] : card_level_5[0])));
    } else if (level == 10) {
        return (random_value <= 10) ? card_level_1[rand() % 6] : ((random_value <= 35) ? card_level_2[rand() % 8] : ((random_value <= 60) ? card_level_3[rand() % 7] : ((random_value <= 90) ? card_level_4[rand() % 7] : card_level_5[0])));
    } else {
        return -1;
    }
}

void
gplayer_imp::GenerateCards(int level) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    for (int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; ++i) 
	{
        int card = GetCardLevel(rng, dist, level);        
		_kid.SetRandomCourse(i, card);
    }
}

bool 
gplayer_imp::KidAwakeningCreate(char type, char name_len, const char name[])
{
	if(name_len > gplayer_kid::MAX_NAME_LENGTH) 
	{
		_runner->error_message(633);
		return false;
	}

	if(_kid.GetNameLength() > 0 || _kid.GetAwakeningDayCount() > 0) 
	{
		_runner->error_message(627);
		return false;		
	}

	if(type > 2) type = 0;	

	// Init
	_kid.SetNameLength(name_len);
	_kid.SetName(name);
	_kid.SetCardLevel(1);
	_kid.SetCourseRandomCost(false);

	GenerateCards(_kid.GetCardLevel());

	_kid.SetType(type);

	_runner->kid_created_info_dialog();

	KidAwakeningNameProtocol ();
	KidAwakeningInfoProtocol ();
	return true;
}

void 
gplayer_imp::KidUnlockNewDay()
{
	if (_kid.GetCheckDay())
	{
		_kid.SetCheckDay(false);
		_kid.SetBlockDay(true);
		_kid.SetAwakening(false);

		KidAwakeningInfoProtocol();
	}
}

bool 
gplayer_imp::KidAwakeningNewDay()
{
	if (!_kid.GetCheckDay())
	{
		_kid.SetBlockDay(false);
		_kid.SetAwakening(true);
		_kid.SetAwakeningDayCount(_kid.GetAwakeningDayCount() + 1);
		_kid.SetAwakeningCash(EmulateSettings::GetInstance()->GetKidAwakeningCash());
		_kid.SetCourseRandomCost(false);
		_kid.SetCheckDay(true);

		KidAwakeningInfoProtocol();
		KidAwakeningPercProtocol();
		KidAwakeningCashProtocol();
	}

	return true;
}

static const int suite_idx_course[] = { 66226, 66255, 66256, 66257,	66258, 66259, 66260, 66261, 66262, 66263, 66264, 66265 };
static const int suite_idx_mask[] = { 1, 2, 4, 8, 16, 32, 64, 128, 65536, 131072, 262144, 524288 };

int gplayer_imp::KidGetSuitePoints()
{
	int points_now = _kid.GetPointsAwakening();
	int points_recv = 0;

	std::unordered_map<unsigned int, int> suite_trigger_count;

	for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++)
	{
		DATA_TYPE dt;
		COURSE_ESSENCE *ess;
		ess = (COURSE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_kid.GetEquipedCourse(i)->course_id, ID_SPACE_ESSENCE, dt);
		if (ess && dt == DT_COURSE_ESSENCE)
		{
			points_recv += ess->score[_kid.GetEquipedCourse(i)->course_level - 1];

			for (unsigned int suite_id : {66226, 66255, 66256, 66257, 66258, 66259, 66260, 66261, 66262, 66263, 66264, 66265})
			{
				DATA_TYPE dt2;
				COURSE_SUITE_ESSENCE *suite_ess;
				suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(suite_id, ID_SPACE_ESSENCE, dt2);
				if (suite_ess && dt2 == DT_COURSE_SUITE_ESSENCE)
				{
					if ((ess->course_mask & suite_ess->suite_mask) == suite_ess->suite_mask)
					{
						suite_trigger_count[suite_id]++;
					}
				}
			}
		}
	}

	float mutiple_add = 0.0f;

	for (const auto &pair : suite_trigger_count)
	{
		unsigned int suite_id = pair.first;
		int count = pair.second;

		COURSE_SUITE_ESSENCE *suite_ess;
		DATA_TYPE dt3;
		suite_ess = (COURSE_SUITE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(suite_id, ID_SPACE_ESSENCE, dt3);
		if (suite_ess && dt3 == DT_COURSE_SUITE_ESSENCE)
		{
			float max_bonus_multiplier = 0.0f;
			for (int level = 0; level < 3; level++)
			{
				if (count >= suite_ess->bonus_info[level].bonus_require_count)
				{
					float bonus_multiplier = 0.0f;
					if (suite_ess->bonus_info[level].bonus_percent_add > 0)
					{
						float bonus_multiplier = (suite_ess->bonus_info[level].bonus_percent_add / suite_ess->bonus_info[level].bonus_trigger_probability);
					}

					if (bonus_multiplier > max_bonus_multiplier)
					{
						max_bonus_multiplier = bonus_multiplier;
					}
				}
			}

			if (max_bonus_multiplier > 0.0f)
			{
				mutiple_add += max_bonus_multiplier;
				max_bonus_multiplier = 0.0f;
			}
		}
	}

	points_recv += (points_recv * mutiple_add);

	// Bônus pela redução para 7 dias
	if (points_recv > 0)
		points_recv *= (3.5f + (0.2f * _kid.GetAwakeningDayCount()));

	points_recv *= EmulateSettings::GetInstance()->GetKidPointsRate();

	return points_recv;
}

bool gplayer_imp::KidAwakeningNewDay2()
{
    if (_kid.GetCheckDay() && !_kid.IsBlockDay())
	{
        int points_now = _kid.GetPointsAwakening();
        int points_recv = 0;

        std::unordered_map<unsigned int, int> suite_trigger_count;

        for (unsigned int i = 0; i < gplayer_kid::MAX_EQUIPED_COURSE; i++) 
		{
            DATA_TYPE dt;
            COURSE_ESSENCE* ess;
            ess = (COURSE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_kid.GetEquipedCourse(i)->course_id, ID_SPACE_ESSENCE, dt);
            if (ess && dt == DT_COURSE_ESSENCE) 
			{
                points_recv += ess->score[_kid.GetEquipedCourse(i)->course_level - 1];

                for (unsigned int suite_id : {66226, 66255, 66256, 66257, 66258, 66259, 66260, 66261, 66262, 66263, 66264, 66265}) 
				{
					DATA_TYPE dt2;
                    COURSE_SUITE_ESSENCE* suite_ess;
                    suite_ess = (COURSE_SUITE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(suite_id, ID_SPACE_ESSENCE, dt2);
                    if (suite_ess && dt2 == DT_COURSE_SUITE_ESSENCE)
					{
                        if ((ess->course_mask & suite_ess->suite_mask) == suite_ess->suite_mask) 
						{						
                            suite_trigger_count[suite_id]++;
                        }
                    }
                }
            }
        }

		float mutiple_add = 0.0f;

        for (const auto& pair : suite_trigger_count) 
		{
            unsigned int suite_id = pair.first;
            int count = pair.second;

            COURSE_SUITE_ESSENCE* suite_ess;
            DATA_TYPE dt3;
            suite_ess = (COURSE_SUITE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(suite_id, ID_SPACE_ESSENCE, dt3);
            if (suite_ess && dt3 == DT_COURSE_SUITE_ESSENCE) 
			{
                float max_bonus_multiplier = 0.0f;
                for (int level = 0; level < 3; level++)
				{
                    if (count >= suite_ess->bonus_info[level].bonus_require_count) 
					{
						float bonus_multiplier = 0.0f;
                        if(suite_ess->bonus_info[level].bonus_percent_add > 0)
						{
							float bonus_multiplier = (suite_ess->bonus_info[level].bonus_percent_add / suite_ess->bonus_info[level].bonus_trigger_probability);
						}

                        if (bonus_multiplier > max_bonus_multiplier) 
						{
                            max_bonus_multiplier = bonus_multiplier;
                        }
                    }
                }

	
				if(max_bonus_multiplier > 0.0f)
				{
					mutiple_add += max_bonus_multiplier;
					max_bonus_multiplier = 0.0f;
				}
            }
        }
	
		points_recv += (points_recv * mutiple_add);

		// Bônus pela redução para 7 dias
		if (points_recv > 0)
			points_recv *= (3.5f + (0.2f * _kid.GetAwakeningDayCount()));

		points_recv *= EmulateSettings::GetInstance()->GetKidPointsRate();

        _kid.SetPointsAwakening(points_now + points_recv);
        _kid.SetAwakeningCash(0);
        _kid.SetBlockDay(true);

        _runner->kid_awakening_points(/*_kid.GetPointsAwakening()*/ points_recv);
    }

    return true;
}

static const int exp_required_next_level[] = { 0, 2, 4, 8, 12, 20, 20, 20, 20, 20 };
static const int exp_min_level[] = { 0, 0, 2, 6, 14, 26, 46, 66, 86, 106 };

bool 
gplayer_imp::KidAwakeningCardLevel()
{
	char level_now = _kid.GetCardLevel ();
	if(level_now >= 10) return false;

	int exp_now = _kid.GetExpCourseRequired ();
	int cash_awakening = _kid.GetAwakeningCash ();
	int cash_cost = 4;
	
	// Verifica os ouros e remove
	if(cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening-cash_cost);

	if(exp_now+4 >= exp_required_next_level[level_now])
	{
		_kid.SetCardLevel(level_now+1);
		_kid.SetExpCourseRequired(exp_now+4-exp_required_next_level[level_now] < 0 ? 0 : exp_now+4-exp_required_next_level[level_now]);		
	} else 
	{
		_kid.SetExpCourseRequired(exp_now+4);
	}

	_runner->kid_course_perc(_kid.GetCardLevel (), _kid.GetExpCourseRequired () + exp_min_level[_kid.GetCardLevel()]);
	KidAwakeningCashProtocol ();	
	return true;
}

bool 
gplayer_imp::KidAwakeningCardRandom()
{	
	std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

	int cash_awakening = _kid.GetAwakeningCash ();
	int cash_cost = 2;

	if(!_kid.GetCourseRandomCost ())
	{
		cash_cost -= 2;
		_kid.SetCourseRandomCost (true);
	}
	
	if(cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening-cash_cost);

	unsigned int cards[5];
	for (int i = 0; i < gplayer_kid::MAX_RANDOM_COURSE; ++i) 
	{
        int card = GetCardLevel(rng, dist, _kid.GetCardLevel ());   
		cards[i] = card;     
		_kid.SetRandomCourse(i, card);
    }
	
	_runner->kid_course_info(cards, gplayer_kid::MAX_RANDOM_COURSE);
	KidAwakeningCashProtocol ();
	return true;
}

bool 
gplayer_imp::KidAwakeningCardPutInventory(char old_slot)
{	
	int cash_awakening = _kid.GetAwakeningCash ();
	int cash_cost = 0;

	DATA_TYPE dt;
	COURSE_ESSENCE * ess;
	ess = (COURSE_ESSENCE*) world_manager::GetDataMan().get_data_ptr(_kid.GetRandomCourse(old_slot), ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_COURSE_ESSENCE) return false;

	cash_cost += ess->cost;

	if(cash_cost > cash_awakening) return false;
	_kid.SetAwakeningCash(cash_awakening-cash_cost);

	int new_slot = -1;
	
	for (unsigned int i = 0; i < gplayer_kid::MAX_STORAGE_COURSE; i++)
	{
		if(_kid.GetStorageCourse(i)->course_id == 0 && new_slot < 0)
		{
			new_slot = i;
			break;
		}			
	}
	
	if(new_slot < 0) return false;
		
	bool res = false;
	int card_id = _kid.GetRandomCourse(old_slot);

	if(!res)
	{
		_kid.SetStorageCourse(new_slot,_kid.GetRandomCourse(old_slot), 1);
		_kid.SetRandomCourse(old_slot, 0);
	}

	_runner->kid_course_change(old_slot, new_slot+6);
	KidAwakeningCashProtocol ();

	_runner->kid_system_points_notify(KidGetSuitePoints());
	return true;
}

void 
gplayer_imp::KidAwakeningCardSwitchInventory(char new_slot, char old_slot1, char old_slot2)
{
	int get_level = _kid.GetStorageCourse(new_slot-6)->course_level;
	int get_id = _kid.GetStorageCourse(new_slot-6)->course_id;

	if(get_level > 3) return;
	if(get_id <= 0) return;

	_kid.SetStorageCourse(new_slot-6, get_id, get_level+1);
	_kid.SetStorageCourse(old_slot1-6, 0, 0);
	_kid.SetStorageCourse(old_slot2-6, 0, 0);

	_runner->kid_course_switch(new_slot, old_slot1, old_slot2);

	_runner->kid_system_points_notify(KidGetSuitePoints());
}

bool 
gplayer_imp::KidAwakeningCardRemoveInventory(char old_slot)
{
	int cash_awakening = _kid.GetAwakeningCash ();
	int cash_cost = 0;
	int course_id = 0;
	int course_level = 0;

	if(old_slot < 6)
	{
		course_id = _kid.GetEquipedCourse(old_slot)->course_id;
		course_level = _kid.GetEquipedCourse(old_slot)->course_level;
	}
	else
	{
		course_id = _kid.GetStorageCourse(old_slot-6)->course_id;
		course_level = _kid.GetStorageCourse(old_slot-6)->course_level;
	}

	DATA_TYPE dt;
	COURSE_ESSENCE * ess;
	ess = (COURSE_ESSENCE*) world_manager::GetDataMan().get_data_ptr(course_id, ID_SPACE_ESSENCE, dt);
	if(!ess || dt != DT_COURSE_ESSENCE) return false;

	cash_cost += ess->cost;

	_kid.SetAwakeningCash(cash_awakening+cash_cost);
		
	bool res = false;
	if(!res)
	{
		if(old_slot < 6)
			_kid.SetEquipedCourse(old_slot, 0, 0);
		else
			_kid.SetStorageCourse(old_slot-6, 0, 0);
	}

	_runner->kid_course_remove(old_slot);
	KidAwakeningCashProtocol ();

	_runner->kid_system_points_notify(KidGetSuitePoints());
	return true;
}

void 
gplayer_imp::KidAwakeningCardMoveEquipInventory(char old_slot, char new_slot)
{
	if(old_slot < 6) 
	{
		int course_id = _kid.GetEquipedCourse(old_slot)->course_id;
		char course_level = _kid.GetEquipedCourse(old_slot)->course_level;	
	
		if(new_slot < 6)
			_kid.SetEquipedCourse(new_slot, course_id, course_level);
		else
			_kid.SetStorageCourse(new_slot-6, course_id, course_level);

		_kid.SetEquipedCourse(old_slot, 0, 0);

		_runner->kid_course_insert(old_slot, new_slot);
	} else 
	{
		int course_id = _kid.GetStorageCourse(old_slot-6)->course_id;
		char course_level = _kid.GetStorageCourse(old_slot-6)->course_level;

		if(new_slot < 6)
			_kid.SetEquipedCourse(new_slot, course_id, course_level);
		else
			_kid.SetStorageCourse(new_slot-6, course_id, course_level);

		_kid.SetStorageCourse(old_slot-6, 0, 0);

		_runner->kid_course_insert(old_slot, new_slot);
	}

	_runner->kid_system_points_notify(KidGetSuitePoints());
}


bool 
gplayer_imp::KidAwakeningNewDay3()
{
	if (_kid.GetAwakeningDayCount() >= EmulateSettings::GetInstance()->GetChildAwakeningDays())
	{
		int get_points = _kid.GetPointsAwakening();
		int gender = _kid.GetType();
		int get_pos = -1;

		DATA_TYPE data;
		const KID_QUALITY_CONFIG *config = (const KID_QUALITY_CONFIG *)world_manager::GetDataMan().get_data_ptr(gplayer_kid::IDX_KID_QUALITY_CONFIG, ID_SPACE_CONFIG, data);
		if (!config || data != DT_KID_QUALITY_CONFIG)
			return false;

		for (unsigned int i = 0; i < 4; i++)
		{
			if (get_points >= config->list[i].require_score_min && get_points <= config->list[i].require_score_max)
			{
				get_pos = i;
				break;
			}
		}

		int finish_idx = abase::RandSelect(&(config->list[get_pos].gender_list[gender].kid[0].probability), sizeof(config->list[get_pos].gender_list[gender].kid[0]), 8);
		int idx_item = config->list[get_pos].gender_list[gender].kid[finish_idx].id;
		bool item_get = false;

		DATA_TYPE data2;
		const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx_item, ID_SPACE_CONFIG, data2);
		if (!config2 || data2 != DT_KID_PROPERTY_CONFIG)
			return false;

		if (_kid.GetCelestial(config2->kid_debri_type)->idx > 0)
		{
			if (_kid.GetCelestial(config2->kid_debri_type)->idx < idx_item)
			{				
				_kid.SetCelestial(config2->kid_debri_type, _kid.GetCelestial(config2->kid_debri_type)->level, _kid.GetCelestial(config2->kid_debri_type)->rank, _kid.GetCelestial(config2->kid_debri_type)->exp+config2->kid_debri_exp, idx_item);
				item_get = true;
			}						
			InvPlayerGiveItem(config2->kid_debri_id, 1);
		}
		else
		{		
			if(config2->broadcast > 0)
			{
				SendClientMsgChild(_kid.GetName(), _kid.GetNameLength(), config2->kid_debri_type);
			}			

			DATA_TYPE data3;
			const KID_LEVEL_MAX_CONFIG *config3 = (const KID_LEVEL_MAX_CONFIG *)world_manager::GetDataMan().get_data_ptr(6877, ID_SPACE_CONFIG, data3);
			if (!config3 || data3 != DT_KID_LEVEL_MAX_CONFIG)
				return false;

			int newlevel = 0;
			newlevel += config3->level_max[config2->rahk];
			int newexp = 0;	

			_kid.SetCelestial(config2->kid_debri_type, newlevel < 1 ? 1 : newlevel, config2->rahk >= 3 ? 1 : 0, newexp, idx_item);	
		}
		
		_kid.ClearAwakening();
		
		KidCelestialInfoProtocol(0);
		_runner->kid_celestial_awakening(item_get ? _kid.GetCelestial(config2->kid_debri_type)->rank : 0, idx_item);

	}
	return true;
}

void 
gplayer_imp::KidCelestialActivityProtocol()
{
	if(_kid.GetActivity()->reserved != -1) return;

	_runner->kid_active_info(_kid.GetActivity()->active_slot, _kid.GetActivity()->reserved);
}

bool 
gplayer_imp::KidCelestialActivity(int val1, int val2, int val3)
{
	if(val1 < 0 && val2 > 5) return false;

	_kid.SetActivity(val1, -1);
	KidCelestialActivityProtocol();
	return true;
}

bool gplayer_imp::KidCelestialUpgradeRank(int pos, int where, int inv_idx) 
{
    item_list & _trashbox = GetTrashInventory(IL_TRASH_BOX8);

    if(inv_idx >= _trashbox.Size()) 
        return false;

    item & item_box = _trashbox[inv_idx];

    if(item_box.type == -1) 
        return false;

    int celestial_idx = _kid.GetCelestial(pos)->idx;

    DATA_TYPE dt;
    const KID_DEBRIS_ESSENCE *ess = (KID_DEBRIS_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_box.type, ID_SPACE_ESSENCE, dt);
    if(!ess || dt != DT_KID_DEBRIS_ESSENCE) return false;

    DATA_TYPE dt2;
    const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(celestial_idx, ID_SPACE_CONFIG, dt2);
    if (!config2 || dt2 != DT_KID_PROPERTY_CONFIG) return false; 

    bool new_star = false;
    int total_count = item_box.count;
    int base_stone_exp = ess->swallow_exp;

    int current_exp = _kid.GetCelestial(pos)->exp;
    int current_star = _kid.GetCelestial(pos)->rank;
    int new_idx = celestial_idx;
    int stones_used = 0;

    if (config2->id_kid_upgrade == 0) 
    {
        DATA_TYPE dt3;
        const KID_UPGRADE_STAR_CONFIG *config3 = (const KID_UPGRADE_STAR_CONFIG *)world_manager::GetDataMan().get_data_ptr(config2->kid_upgrade_star_config, ID_SPACE_CONFIG, dt3);
        if (!config3 || dt3 != DT_KID_UPGRADE_STAR_CONFIG) return false;

        while (total_count > 0) 
        {
            int required_exp = config3->upgrade_star_info[current_star].start_exp;
            if (current_exp + base_stone_exp >= required_exp) 
            {
                current_exp = (current_exp + base_stone_exp) - required_exp;
                current_star++;
                stones_used++;
                total_count--;

                if (current_star >= 6) 
                {
                    current_star = 6;
                    current_exp = 0;
                    break;
                }
            } 
            else 
            {
                current_exp += base_stone_exp;
                stones_used++;
                total_count--;
            }
        }

        if(SpendTrashBoxItem2(IL_TRASH_BOX8, inv_idx, stones_used))
        {
            _kid.SetCelestial(pos, _kid.GetCelestial(pos)->level, current_star, current_exp, celestial_idx);
            KidCelestialInfoProtocol(0);   
        }  
        return true;
    } 
    else 
    {        
        while (total_count > 0 && config2->id_kid_upgrade != 0) 
        {
            int required_exp = config2->upgrade_exp;

            if (current_exp + base_stone_exp >= required_exp) 
            {
                current_exp = (current_exp + base_stone_exp) - required_exp;
                total_count--;
                stones_used++;

                new_idx = config2->id_kid_upgrade;
                config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(new_idx, ID_SPACE_CONFIG, dt2);
                if (!config2 || dt2 != DT_KID_PROPERTY_CONFIG) return false;
            } 
            else 
            {
                current_exp += base_stone_exp;
                total_count--;
                stones_used++;
            }
        }

        if(SpendTrashBoxItem2(IL_TRASH_BOX8, inv_idx, stones_used))
        {
            _kid.SetCelestial(pos, _kid.GetCelestial(pos)->level, current_star, current_exp, new_idx);   
            KidCelestialInfoProtocol(1);            
        }  
        return true;
    }    
}


void
gplayer_imp::KidCelestialTransformation(int mode)
{
	struct
	{
		int id;
		int level;
	} _skills_shape[16];

	int skills_count = 0;
	int slot = _kid.GetActivity()->active_slot;
	int now = g_timer.get_systime();

	memset(_skills_shape, 0, sizeof(_skills_shape));
	object_interface obj_if(this);

	// === DEACTIVATE (mode == 0) ===
	if (!mode)
	{
		if (!_kid_transformation)
			return;

		// Restore skill levels từ saved state
		int saved_count = _kid_transform_skill_state.saved_count;
		if (saved_count < 0) saved_count = 0;
		if (saved_count > 16) saved_count = 16;

		for (int i = 0; i < saved_count; i++)
		{
			int sk_id = _kid_transform_skill_state.saved_skill_id[i];
			int sk_old = _kid_transform_skill_state.saved_skill_level[i];
			if (sk_id <= 0) continue;
			if (sk_old > 0)
				_skill.SetLevel(sk_id, sk_old);
			else
				_skill.Remove(sk_id);

			// Build packet payload với level kid (như activate) — flag enabled=0 ở packet sẽ báo client gỡ skill
			_skills_shape[skills_count].id = sk_id;
			_skills_shape[skills_count].level = _kid_transform_skill_state.saved_kid_skill_level[i];
			skills_count++;
		}

		// Áp post-transform buff (giant/blessmagic/stoneskin/incresist/inchp/ironshield)
		if (_kid_transformation_time > 0)
			_skill.AddFilterKidDecTransformation(obj_if, 1800);

		_kid_transformation = 0;
		_kid_transformation_time = 0;
		memset(&_kid_transform_skill_state, 0, sizeof(_kid_transform_skill_state));

		ChangeShape(0);
		_runner->kid_celestial_transformation(0, _parent->ID.id, 0, 0);
		_runner->player_world_speak_info((char)0, (char)1, (char)1, skills_count, (int*)_skills_shape);
		obj_if.RemoveTeamVisibleState(GNET::HSTATE_530);
		return;
	}

	// === ACTIVATE (mode == 1) ===

	// Validate slot/idx/level
	if (slot < 0 || slot >= 6)
	{
		_runner->error_message(627);
		return;
	}
	int idx = _kid.GetCelestial(slot)->idx;
	int level = _kid.GetCelestial(slot)->level;
	int rank = _kid.GetCelestial(slot)->rank;
	if (idx <= 0 || level <= 0)
	{
		_runner->error_message(627);
		return;
	}

	// Đã đang biến hình → không cho activate đè
	if (_kid_transformation)
	{
		_runner->error_message(627);
		return;
	}

	DATA_TYPE data2;
	const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(idx, ID_SPACE_CONFIG, data2);
	if (!config2 || data2 != DT_KID_PROPERTY_CONFIG) return;

	DATA_TYPE data3;
	const KID_SKILL_CONFIG *configskill = (const KID_SKILL_CONFIG *)world_manager::GetDataMan().get_data_ptr(config2->id_kid_skill, ID_SPACE_CONFIG, data3);
	if (!configskill || data3 != DT_KID_SKILL_CONFIG) return;

	// Check forms khác đang active
	if (obj_if.IsFilterExist(FILTER_BEASTIEFORM) ||
		obj_if.IsFilterExist(FILTER_TIGERFORM) ||
		obj_if.IsFilterExist(FILTER_FOXFORM) ||
		obj_if.IsFilterExist(FILTER_SWIFTFORM) ||
		obj_if.IsFilterExist(FILTER_FISHFORM) ||
		obj_if.IsFilterExist(FILTER_THUNDERFORM) ||
		obj_if.IsFilterExist(FILTER_SHADOWFORM) ||
		obj_if.IsFilterExist(FILTER_FAIRYFORM) ||
		obj_if.IsFilterExist(FILTER_GIANTFORM))
	{
		_runner->error_message(627);
		return;
	}

	if (!CheckCoolDown(COOLDOWN_INDEX_KID_TRANSFORMATION))
	{
		_runner->error_message(628);
		return;
	}
	SetCoolDown(COOLDOWN_INDEX_KID_TRANSFORMATION, IDX_TIME_COOLDOWN);

	// Build skill list (j chạy ngược 9..0, BREAK ngay khi tìm thấy level đủ)
	memset(&_kid_transform_skill_state, 0, sizeof(_kid_transform_skill_state));
	for (int i = 0; i < 16; i++)
	{
		if (configskill->skill[i].id <= 0) continue;
		for (int j = 9; j >= 0; j--)
		{
			if (configskill->skill[i].level[j] > 0 && level >= configskill->skill[i].level[j])
			{
				int sk_id = configskill->skill[i].id;
				int sk_lv = j;
				int sk_old = _skill.GetLevel(sk_id, GetPlayerClass(), false);

				_skills_shape[skills_count].id = sk_id;
				_skills_shape[skills_count].level = sk_lv;

				_kid_transform_skill_state.saved_skill_id[skills_count] = sk_id;
				_kid_transform_skill_state.saved_skill_level[skills_count] = sk_old;
				_kid_transform_skill_state.saved_kid_skill_level[skills_count] = sk_lv;
				skills_count++;
				break;
			}
		}
	}
	_kid_transform_skill_state.saved_count = skills_count;
	_kid_transform_skill_state.saved_idx = idx;
	_kid_transform_skill_state.saved_slot = slot;

	// Tính buff ratios từ KID_PROPERTY_CONFIG
	float rank_param = 1.0f;
	DATA_TYPE dt_star;
	const KID_UPGRADE_STAR_CONFIG *cfg_star = (const KID_UPGRADE_STAR_CONFIG *)world_manager::GetDataMan().get_data_ptr(config2->kid_upgrade_star_config, ID_SPACE_CONFIG, dt_star);
	if (cfg_star && dt_star == DT_KID_UPGRADE_STAR_CONFIG)
	{
		if (rank > 0 && rank <= 6)
			rank_param = cfg_star->upgrade_star_info[rank - 1].star_param;
		else
			rank_param = cfg_star->zero_star_param;
	}
	if (rank_param <= 0.0f) rank_param = 1.0f;

	float kid_hp   = (float)config2->hp            * (float)level * rank_param;
	float kid_dmg  = (float)config2->damage        * (float)level * rank_param;
	float kid_def  = (float)config2->defence       * (float)level * rank_param;
	float kid_mdef = (float)config2->magic_defence * (float)level * rank_param;

	float hp_ratio = 0.0f, dmg_ratio = 0.0f, def_ratio = 0.0f, resist_ratio = 0.0f;
	if (_cur_prop.max_hp > 0)
		hp_ratio = kid_hp / (float)_cur_prop.max_hp;

	int base_dmg = (_cur_prop.damage_low + _cur_prop.damage_high) / 2;
	if (base_dmg > 0)
		dmg_ratio = kid_dmg / (float)base_dmg;

	if (_cur_prop.defense > 0)
		def_ratio = kid_def / (float)_cur_prop.defense;

	int resist_sum = _cur_prop.resistance[0] + _cur_prop.resistance[1] + _cur_prop.resistance[2] + _cur_prop.resistance[3] + _cur_prop.resistance[4];
	if (resist_sum > 0)
		resist_ratio = kid_mdef * 5.0f / (float)resist_sum;

	// Cap để tránh data lỗi gây buff quá mức
	if (hp_ratio     > 5.0f) hp_ratio     = 5.0f;
	if (dmg_ratio    > 5.0f) dmg_ratio    = 5.0f;
	if (def_ratio    > 5.0f) def_ratio    = 5.0f;
	if (resist_ratio > 5.0f) resist_ratio = 5.0f;

	// Set transform state — 1800s = 30 phút (khớp với cooldown IDX_TIME_COOLDOWN)
	_kid_transformation = 1;
	_kid_transformation_time = 1800;

	// Áp filter: HSTATE_530 + Cleardebuff (visual + bảo vệ debuff)
	_skill.AddFilterKidIncTransformation(obj_if, 1800);
	// Áp filter: stat buff (HP/dmg/def/resist)
	_skill.AddFilterKidTransformStats(obj_if, 1800, hp_ratio, dmg_ratio, def_ratio, 0.0f, resist_ratio);

	// Đổi shape
	ChangeShape(config2->unk1 | (3 << 6));

	// Apply skill levels (sau khi đã lưu level cũ)
	for (int i = 0; i < skills_count; i++)
	{
		if (_skills_shape[i].id > 0)
			_skill.SetLevel(_skills_shape[i].id, _skills_shape[i].level + 1);
	}

	// Full heal sau khi áp buff (max_hp đã được tăng bởi SetGiant)
	_basic.hp = _cur_prop.max_hp;

	_runner->kid_celestial_transformation(config2->unk1, _parent->ID.id, 1800, now + 1800);
	_runner->player_world_speak_info((char)1, (char)1, (char)1, skills_count, (int*)_skills_shape);
}
void
gplayer_imp::FixChildSystem()
{
	for (unsigned int i = 0; i < 6; i++)
	{	
		if (_kid.GetCelestial(i)->idx > 0)
		{
			DATA_TYPE data2;
			const KID_PROPERTY_CONFIG *config2 = (const KID_PROPERTY_CONFIG *)world_manager::GetDataMan().get_data_ptr(_kid.GetCelestial(i)->idx, ID_SPACE_CONFIG, data2);
			if (config2 && data2 == DT_KID_PROPERTY_CONFIG)
			{
				DATA_TYPE data3;
				const KID_LEVEL_MAX_CONFIG *config3 = (const KID_LEVEL_MAX_CONFIG *)world_manager::GetDataMan().get_data_ptr(6877, ID_SPACE_CONFIG, data3);
				if (!config3 || data3 != DT_KID_LEVEL_MAX_CONFIG)
					return;

				int newlevel = 0;
				newlevel += config3->level_max[config2->rahk];
				int old_level = config3->level_max[_kid.GetCelestial(i)->rank];

				if(_kid.GetCelestial(i)->level > old_level)
				{	
					_kid.SetCelestial(i, newlevel < 1 ? 1 : newlevel, config2->rahk >= 3 ? 1 : 0, _kid.GetCelestial(i)->exp, _kid.GetCelestial(i)->idx);
				}
			}
		}
	}
	KidCelestialInfoProtocol(0);
}

void gplayer_imp::FixExpHeartBeat()
{
	if(_basic.exp < 0)
	{
		int next_exp = player_template::GetLvlupExp(GetPlayerClass(),_basic.level);
		_basic.exp = next_exp+1;

		LevelUp();
	}
	if(_player_reincarnation.GetTomeExp() < 0)
	{
		_player_reincarnation.SetTomeExp(0);
	}
}

void
gplayer_imp::SendClientMsgChild(char* child_name, int child_name_len, int type)
{
	struct
	{
		char player_name[MAX_USERNAME_LENGTH];
		char child_name[MAX_USERNAME_LENGTH_NOTIFY];
		int type;
	}data;
	memset(&data, 0, sizeof(data));

	unsigned int len = _username_len;
	if(len > MAX_USERNAME_LENGTH) len = MAX_USERNAME_LENGTH;
	memcpy(data.player_name, _username, len);

	unsigned int len2 = MAX_USERNAME_LENGTH_NOTIFY;
	if(len2 > MAX_USERNAME_LENGTH_NOTIFY) len2 = MAX_USERNAME_LENGTH_NOTIFY;
	memcpy(data.child_name, child_name, len2);

	data.type = type;

	packet_wrapper buf(sizeof(data));
	buf.push_back(&data, sizeof(data));

	broadcast_chat_msg(CHILD_AWAKENING_CHAT_MSG_ID, buf.data(), buf.size(), GMSV::CHAT_CHANNEL_SYSTEM, 0, 0, 0);
}


