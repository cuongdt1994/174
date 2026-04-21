#include <stdlib.h>
#include <arandomgen.h>
#include "actobject.h"
#include "object.h"
#include "world.h"
#include "usermsg.h"
#include "actsession.h"
#include "property_ar.h"
#include "playertemplate.h"
#include "pathfinding/pathfinding.h"
#include "luamanager.h"

int ai_actobject::QueryTarget(const XID & id, target_info & info)
{
    world::object_info base_info;
    bool rst = _imp->_plane->QueryObject(id,base_info);
    if(!rst) return TARGET_STATE_NOT_FOUND;
    info.pos     = base_info.pos;
    info.race     = base_info.race;
    info.faction     = base_info.faction;
    info.level     = base_info.level;
    info.hp     = base_info.hp;
    info.mp        = base_info.mp;
    info.body_size     = base_info.body_size;
    info.max_hp = base_info.max_hp;
    info.invisible_degree = base_info.invisible_degree;
    info.object_state = base_info.object_state;
    info.object_state2 = base_info.object_state2;
    if(base_info.state & world::QUERY_OBJECT_STATE_ZOMBIE) return TARGET_STATE_ZOMBIE;
    return TARGET_STATE_NORMAL;
}

void 
ai_actobject::SendMessage(const XID & id, int message)
{
	MSG msg;
	gobject * pObj = _imp->_parent;
	BuildMessage(msg,message,id,pObj->ID,pObj->pos);
	_imp->_plane->PostLazyMessage(msg);
}

gactive_imp::gactive_imp():_session_id(0),_session_process(false),_switch_dest(-1),_direction(-1,0,0),_session_state(STATE_SESSION_IDLE),_cur_session(NULL),_hp_gen_counter(0),_mp_gen_counter(0),_moving_action_env(this)
{
	memset(&_basic,0,sizeof(_basic));
	memset(&_base_prop,0,sizeof(_base_prop));
	memset(&_cur_prop,0,sizeof(_cur_prop));
	memset(&_cur_item,0,sizeof(_cur_item));
	memset(&_cur_rune,0,sizeof(_cur_rune));
	memset(&_en_point,0,sizeof(_en_point));
	memset(&_en_percent,0,sizeof(_en_percent));
	memset(&_elf_en, 0, sizeof(_elf_en));	//lgc
	memset(&_plus_enhanced_param, 0, sizeof(_plus_enhanced_param));
	
	_server_notify_timestamp = 0;
	_damage_reduce = 0;
	memset(_magic_damage_reduce,0,sizeof(_magic_damage_reduce));
	_crit_rate = 0;
	_base_crit_rate = 0;
	_crit_damage_bonus = 0;
	_crit_damage_reduce = 0;
	_crit_resistance = 0;
	_exp_addon = 0;
	_immune_state = 0;
	_immune_state_adj = 0;

	_faction = 0;
	_enemy_faction = 0;
	_combat_state = false;
	_refresh_state = false;
	_visible_team_state.reserve(32);
	_team_visible_state_flag = false;
	_visiable_state_flag = false;
	_cur_form = 0;
	_idle_mode_flag = 0;
	_seal_mode_flag = 0;
	_invader_state = INVADER_LVL_0;
	__at_attack_state = 0;
	__at_defense_state = 0;
	_layer_ctrl.Ground();
	_subscibe_timer = 0;
	_visible_state_list.insert(_visible_state_list.end(),MAX_VISIBLE_STATE_COUNT,0);
	_lock_equipment = false;
	_lock_inventory = false;
	_bind_to_ground = 0;
	_deny_all_session = 0;
	_last_attack_target = XID(-1,-1);
	_attack_degree = 0;
	_defend_degree = 0;
	_invisible_passive = 0;
	_invisible_active = 0;
	_anti_invisible_passive = 0;
	_anti_invisible_active = 0;
	_damage_dodge_rate = 0;
	_debuff_dodge_rate = 0;
	_hp_steal_rate = 0;
	_heal_cool_time_adj = 0;
	_skill_enhance = 0;
	_penetration = 0;
	_resilience = 0;
	_attack_monster = false;
	_vigour_base = 0;
	_vigour_en = 0;
	_skill_enhance2 = 0;
	_near_normal_dmg_reduce = 0;
	_near_skill_dmg_reduce = 0;
	_far_normal_dmg_reduce = 0;
	_far_skill_dmg_reduce = 0;
	_mount_speed_en = 0;
    _exp_sp_factor = 0;
    _realm_exp_factor = 0;
	_anti_defense_degree = 0;
	_anti_resistance_degree = 0;
	_highest_level = 0;
	_infected_skill_id = 0;
	_infected_skill_lvl = 0;
	_idle_seal_mode_counter.insert(_idle_seal_mode_counter.end(),4,0);//lgc
	_immune_state_adj_counter.insert(_immune_state_adj_counter.end(),32,0);
	//	_skill_list.SetImp(this);
	//	_skill_list.AdjustSkill(0,10,1);
	_new_shield_retort = false; // Novo escudo paladino/wf
	_new_shield_retort_paladin = false; // Novo escudo paladino/wf
	_is_critical_damage = false;
}

gactive_imp::~gactive_imp()
{		
	//clear session queue
	ClearSession();
	ReleaseAction();
}

	void
gactive_imp::MH_query_info00(const MSG & msg)
{
	if(msg.source.type == GM_TYPE_PLAYER && msg.content_length == sizeof(int))
	{
		_runner->query_info00(msg.source,*(int*)(msg.content), msg.param);
	}
	else
	{
		ASSERT(false);
	}
}

	bool
gactive_imp::CurSessionValid(int id)
{
	return _cur_session && _cur_session->_session_id == id;
}

	int
gactive_imp::MessageHandler(world *pPlane,const MSG & msg)
{
	//	__PRINTF("active object recv message (%d,%d)%d\n",_parent->ID.type,_parent->ID.id,msg.message);
	switch(msg.message)
	{
		case GM_MSG_ATTACK:
			{
				ASSERT(msg.content_length >= sizeof(attack_msg));
				//Here, due to the copy operation, there is a certain cost.
				attack_msg ack_msg = *(attack_msg*)msg.content;
				//Handle incoming attack messages
				_filters.EF_TransRecvAttack(msg.source, ack_msg);
				HandleAttackMsg(pPlane,msg,&ack_msg);
				return 0;
			}
		case GM_MSG_ENCHANT:
			{
				__PRINTF("recv enchant\n");
				ASSERT(msg.content_length >= sizeof(enchant_msg));
				enchant_msg ech_msg = *(enchant_msg*)msg.content;
				//Here, due to the copy operation, there is a certain cost.
				_filters.EF_TransRecvEnchant(msg.source, ech_msg);
				HandleEnchantMsg(pPlane,msg,&ech_msg);
				return 0;
			}

		case GM_MSG_OBJ_ZOMBIE_SESSION_END:
		case GM_MSG_OBJ_SESSION_END:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
			EndCurSession();
			StartSession();
			return 0;

		case GM_MSG_OBJ_SESSION_REPEAT:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
			if(HasNextSession())
			{
				EndCurSession();
				StartSession();
			}
			else
			{
				int guid = _cur_session->GetGUID();
				int p = _session_state;
				if(!RepeatSession())		
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR wrong session1 state classid:%d session_state%d",guid,p);
					}
					EndCurSession();
					StartSession();
				}
				else
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR wrong session4 state classid:%d session_state%d",guid,p);
					}
				}
			}
			return 0;
		case GM_MSG_OBJ_SESSION_REPEAT_FORCE:
			if(!CurSessionValid(msg.param))
			{
				break;
			}
			__PRINTF("-----session repeat force\n");
			{
				int guid = _cur_session->GetGUID();
				int p = _session_state;
				if(!RepeatSession())
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR wrong session2 state classid:%d session_state%d",guid,p);
					}

					EndCurSession();
					StartSession();
				}
				else
				{
					if(_cur_session == NULL)
					{
						GLog::log(GLOG_ERR,"FATALERROR wrong session3 state classid:%d session_state%d",guid,p);
					}
				}
			}
			return 0;
		case GM_MSG_HEARTBEAT:
			DoHeartbeat(msg.param);
			return 0;

		case GM_MSG_RECEIVE_MONEY:
			OnPickupGold(msg.param,0);
		break;

		case GM_MSG_PICKUP_MONEY:
		{
			ASSERT(msg.content && msg.content_length == sizeof(int));
			int id = 0;
			if(msg.content_length) id = *(int*)msg.content;
			OnPickupGold(msg.param,id);
		}
		break;

		case GM_MSG_RECEIVE_MONEY2:
			OnPickupSilver(msg.param,0);
		break;

		case GM_MSG_PICKUP_MONEY2:
		{
			ASSERT(msg.content && msg.content_length == sizeof(int));
			int id = 0;
			if(msg.content_length) id = *(int*)msg.content;
			OnPickupSilver(msg.param,id);
		}
		break;

		case GM_MSG_PICKUP_ITEM:
			ASSERT(msg.content && msg.content_length);
			OnPickupItem(msg.pos,msg.content,msg.content_length,msg.param&0x80000000,msg.param&0x7FFFFFFF);
			break;
		case GM_MSG_QUERY_OBJ_INFO00:
			MH_query_info00(msg);
			break;
		case GM_MSG_SUBSCIBE_TARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			InsertInfoSubscibe(msg.source,*(link_sid*)msg.content);
			SendTo<0>(GM_MSG_SUBSCIBE_CONFIRM,msg.source,0);
			break;

		case GM_MSG_UNSUBSCIBE_TARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			RemoveInfoSubscibe(msg.source);
			break;
			
		case GM_MSG_SUBSCIBE_SUBTARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			InsertInfoSecondSubscibe(msg.source,*(link_sid*)msg.content);
			SendTo<0>(GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM,msg.source,0);
			break;

		case GM_MSG_UNSUBSCIBE_SUBTARGET:
			ASSERT(msg.source.type == GM_TYPE_PLAYER);
			RemoveInfoSecondSubscibe(msg.source);
			break;

		case GM_MSG_HP_STEAL:
			if(!_parent->IsZombie() && msg.param > 0)
			{
				IncHP(msg.param);
				_runner->hp_steal(msg.param);
			}
			break;
		case GM_MSG_ATTACK_CRIT_FEEDBACK:
			if(!_parent->IsZombie() && msg.param > 0)
			{
				_filters.ModifyFilter(msg.param,FMID_CRIT_FEEDBACK,0,0);
			}
			break;
		case GM_MSG_QUERY_INFO_1:
			{
				ASSERT(msg.content_length == sizeof(int));
				_runner->query_info_1(msg.source.id, msg.param, *(int*)(msg.content));
			}
			break;

		case GM_MSG_HURT:
			{
				if(!_parent->IsZombie()) 
				{
					ASSERT(msg.content_length == sizeof(msg_hurt_extra_info_t));
					msg_hurt_extra_info_t & data = *(msg_hurt_extra_info_t*)msg.content;
					DoDamage(msg.param);
					if(_basic.hp == 0)
					{
						Die(msg.source,data.orange_name && _invader_state == INVADER_LVL_0,data.attacker_mode,0);
					}
				}
			}
			break;

		case GM_MSG_DUEL_HURT:
			{
				if(!_parent->IsZombie()) 
				{
					int state = ((gactive_object*)_parent)->object_state;
					if(state & gactive_object::STATE_IN_DUEL_MODE)
					{
						DoDamage(msg.param);
						if(_basic.hp == 0)
						{
							Die(msg.source,false,attack_msg::PVP_DUEL,0);
						}
					}
				}
			}
			break;

		case GM_MSG_DEATH:
			{
				if(!_parent->IsZombie()) 
				{
					DoDamage(_basic.hp + 1000);
					if(_basic.hp == 0)
					{
						if(msg.param == 0)
							Die(msg.source,false,0,msg.param);
						else
							Die(XID(GM_TYPE_NPC,-1),false,0,msg.param);
					}		
				}
			}
			break;

		case GM_MSG_TRANSFER_FILTER_DATA:
			{
				if(!_parent->IsZombie())
				{
					raw_wrapper ar(msg.content, msg.content_length);
					_filters.AddSpecFilters(ar, msg.param, this);	
				}
			}
			break;

		case GM_MSG_TRANSFER_FILTER_GET:
			{
				ASSERT(msg.content_length == sizeof(int));
				if(!_parent->IsZombie())
				{
					TransferSpecFilters(msg.param, msg.source, *(int*)msg.content);
				}
			}
			break;
		
		case GM_MSG_INJECT_HP_MP:
			{
				ASSERT(msg.content_length == sizeof(msg_hp_mp_t));
				msg_hp_mp_t & data = *(msg_hp_mp_t*)msg.content;
				if(!_parent->IsZombie())
				{
					if(data.hp > 0) IncHP(data.hp);
					if(data.mp > 0) InjectMana(data.mp);
				}
			}
			break;
			
		case GM_MSG_DRAIN_HP_MP:
			{
				ASSERT(msg.content_length == sizeof(msg_hp_mp_t));
				msg_hp_mp_t & data = *(msg_hp_mp_t*)msg.content;
				if(!_parent->IsZombie())
				{
					if(data.mp > 0) DrainMana(data.mp);
					if(data.hp > 0)
					{
						int dec = data.hp>_basic.hp-1 ? _basic.hp-1 : data.hp;
						DecHP(dec);
						if(msg.param)
						{
							XID attacker;
							MAKE_ID(attacker, msg.param);
							struct attacker_info_t info = {XID(-1,-1),0,0,0,0,-1,-1,0,0};
							_runner->be_hurt(attacker, info, dec, false);
						}
					}
				}
			}
			break;
			
		case GM_MSG_EXTERN_HEAL:
			if(!_parent->IsZombie() && msg.param > 0)
			{
				IncHP(msg.param);
			}
			break;
		
		case GM_MSG_PUNISH_ME:
			if(!_parent->IsZombie())
			{
				ASSERT(msg.content_length == sizeof(msg_punish_me_t));
				msg_punish_me_t & data = *(msg_punish_me_t*)msg.content;
		
				if(data.skill_id)
				{
					enchant_msg enchant_m;
					memset(&enchant_m,0,sizeof(enchant_m));
					enchant_m.skill = data.skill_id;
					enchant_m.skill_level = data.skill_lvl;

					FillEnchantMsg(msg.source,enchant_m);
					TranslateEnchant(msg.source,enchant_m);
					SendEnchantMsg(GM_MSG_ENCHANT,msg.source,enchant_m);
				}
				else
				{
					attack_msg attack_m;
					MakeAttackMsg(attack_m,1);
					FillAttackMsg(msg.source,attack_m);
					attack_m.physic_damage = data.skill_lvl;
					attack_m.attack_range = 1e3; // Ensure that physical attacks are not limited by distance
					memset(attack_m.magic_damage, 0, sizeof(attack_m.magic_damage));
					SendAttackMsg(msg.source,attack_m);
				}
			}
			break;

		case GM_MSG_OBJ_ACTION_END:
			if(_moving_action_env.ActionValid(msg.param))
			{
				_moving_action_env.EndAction();	
			}
			break;

		case GM_MSG_OBJ_ACTION_REPEAT:
			if(_moving_action_env.ActionValid(msg.param))
			{
				_moving_action_env.RepeatAction();	
			}
			break;
			
		default:
			__PRINTF("An unhandled message was encountered %d to %p\n",msg.message,this);
			break;
	}
	return 0;
}

bool 
gactive_imp::AttackJudgement(attack_msg * attack,damage_entry &dmg,bool is_short_range,float dist)
{
	memset(&dmg,0,sizeof(dmg));
	//The first is the physical attack judgment
	//ASSERT(attack->physic_damage >=0);
	if ( attack->physic_damage < 0 ) attack->physic_damage = 0;
	
	if(attack->attack_attr == attack_msg::PHYSIC_ATTACK)	//Only physical attacks are used to judge the attack judgment
	{
		int rate = attack->attack_rate;
		float attack_pb= (float)rate  / ((float)(rate + (_cur_prop.armor >> 1)));
		//if(attack_pb > 0.95f) attack_pb = 0.95f;
		if(attack_pb < 0.05f) attack_pb = 0.05f;
		if(abase::RandUniform() > attack_pb)
		{
			//no hit
			return false;
		}
		//if(is_short_range)
		//{
		//	attack->physic_damage /= 2;
		//}
	}
	
	if(is_short_range)
	{
		if(attack->skill_id)		//Skill attack melee damage adjustment lgc
		{
			//ASSERT(attack->short_range_adjust_factor > 0);
			if( attack->short_range_adjust_factor < 1 ) attack->short_range_adjust_factor = 1;
			attack->physic_damage = int(attack->physic_damage * attack->short_range_adjust_factor);
			for (int i = 0; i < MAGIC_CLASS; i ++)
				attack->magic_damage[i] = int(attack->magic_damage[i] * attack->short_range_adjust_factor);
		}
		else					//non-skill attack
		{
			attack->physic_damage /= 2; 
		}
	}

	// Calculates the damage falloff given to players by distance
	if(attack->ainfo.attacker.IsPlayer() || attack->ainfo.attacker.IsPet())
	{
		if(attack->skill_id)
		{
			float dist_ratio = 1.f - player_template::GetNearDamageReduceFactor(dist,_near_skill_dmg_reduce) 
				- player_template::GetFarDamageReduceFactor(dist,_far_skill_dmg_reduce);
			if(dist_ratio <= 0) dist_ratio = 0;
			attack->physic_damage = int(attack->physic_damage * dist_ratio);
			for (int i = 0; i < MAGIC_CLASS; i ++)
				attack->magic_damage[i] = int(attack->magic_damage[i] * dist_ratio);
		}
		else
		{
			float dist_ratio = 1.f - player_template::GetNearDamageReduceFactor(dist,_near_normal_dmg_reduce) 
				- player_template::GetFarDamageReduceFactor(dist,_far_normal_dmg_reduce);
			if(dist_ratio <= 0) dist_ratio = 0;
			attack->physic_damage = int(attack->physic_damage * dist_ratio);
			
		}
	}

	bool attacked = false;
	int imask = _immune_state | _immune_state_adj;
	int alevel = attack->ainfo.level;
	if(attack->physic_damage)
	{
		if(imask & IMMUNE_MASK_PHYSIC)
		{
			attack->_attack_state |= AT_STATE_IMMUNE;
		}
		else
		{
			float damage = attack->physic_damage;
			//Perform attack ratio correction, not completed, still use the original formula
			int def = attack->anti_defense_degree == 0 ? _cur_prop.defense :
				player_template::CalcAntiDef(attack->anti_defense_degree,_cur_prop.defense);
			float reduce = player_template::GetDamageReduce(def,alevel);
			damage = damage * (1 - reduce);
			DoDamageReduce(damage);
			if(damage <0) damage = 0;
			dmg.physic_damage = damage;
			attacked = true;
		}
	}
	int i;
	for (i = 0; i < MAGIC_CLASS; i ++)
	{
		if(attack->magic_damage[i])
		{
			if(imask & (1 << (i+1)))	 //IMMUNE_GOLD = 0x02 
			{
				attack->_attack_state |= AT_STATE_IMMUNE;
			}
			else
			{
				float damage =attack->magic_damage[i];
				int def = attack->anti_resistance_degree == 0 ? _cur_prop.resistance[i] :
					player_template::CalcAntiDef(attack->anti_resistance_degree,_cur_prop.resistance[i]);
				float reduce = player_template::GetDamageReduce(def,alevel);
				damage = damage * (1-reduce);
				DoMagicDamageReduce(i,damage);
				if(damage <0) damage = 0;
				dmg.magic_damage[i] = damage;
				attacked = true;
			}
		}
	}
	return attacked;
}

void 
gactive_imp::Die(const XID & attacker, bool is_pariah, char attacker_mode, int taskdead)
{
	//The processing before the filter call dies
	_filters.EF_BeforeDeath(attacker,attacker_mode);
	//The skill filter will cause the object to respawn after death	
	if(_basic.hp > 0 && taskdead == 0) return;

	//Clear necessary flags
	_idle_mode_flag = 0;
	_seal_mode_flag = 0;

	//Remove the filter that should be removed when dying
	_filters.ClearSpecFilter(filter::FILTER_MASK_REMOVE_ON_DEATH);

	//Death, calling the death function, processing
	OnDeath(attacker, is_pariah, attacker_mode, taskdead);

	//
	slice * pPiece = _parent->pPiece;
	if(pPiece && pPiece->IsBorder())
	{
		extern_object_manager::SendRefreshMsg<0>(_plane,_parent,0,pPiece);
	}
	
}

int 
gactive_imp::HandleEnchantMsg(world * pPlane,const MSG & msg, enchant_msg * enchant)
{
	if(!enchant->target_faction)
	{
		//Untargeted attack, canceled by previous attack
		//But if it is canceled because of immunization, then send immunization data
		if(enchant->_attack_state & gactive_imp::AT_STATE_IMMUNE)
		{
			_runner->enchant_result(msg.source,enchant->skill,enchant->skill_level,false,enchant->_attack_state,enchant->section);
		}
		return 0;
	}
	bool fac_match = (GetFaction() & enchant->target_faction);
	bool orange_name;// = enchant->is_invader && !fac_match;
	if(!enchant->helpful)
	{
		if(!fac_match && !enchant->force_attack)
		{
			//It may be necessary to return an error to let the other party stop the attack
			return 0;
		}
		//Perform status dodge judgment
		if(abase::Rand(0,99) < _debuff_dodge_rate)
		{
			enchant->_attack_state |= AT_STATE_DODGE_DEBUFF;
			_runner->enchant_result(msg.source,enchant->skill,enchant->skill_level,false,enchant->_attack_state,enchant->section);
			return 0;	
		}
		//Is it an illegal attack
		orange_name = enchant->is_invader && !fac_match;
	}
	else
	{
		if(fac_match && enchant->helpful != 3) return 0;

		//Whether the gain operation is illegal is completely judged by the outside
		orange_name = enchant->is_invader;
	}

	_filters.EF_DoEnchant(msg.source, *enchant);

	// Ignora o efeito negativo (novo escudo)
	if(_new_shield_retort && !enchant->helpful)
	{
		return 0;
	}

	if(_new_shield_retort_paladin && !enchant->helpful)
	{
		return 0;
	}

	 // Infection priority calculation
	if(enchant->infected_skill.skill && !enchant->helpful)
	{
		_skill.Infect(this,msg.source,msg.pos,*enchant,orange_name);
	}

	_skill.Attack(this,msg.source,msg.pos,*enchant,orange_name);
	if(orange_name)
	{
		//Determine whether to send a fan name message
		//Death will automatically send a red name message
		SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,enchant->ainfo.attacker,60);
	}
	return 0;
}

int 
gactive_imp::HandleAttackMsg(world * pPlane,const MSG & msg, attack_msg * attack)
{
	//msg.param is the attacker's cs_index
	//Judgment distance
	A3DVECTOR pos = _parent->pos;
	float fTmp = pos.squared_distance(msg.pos);
	float squared_range = attack->attack_range + _parent->body_size;
	if( fTmp > squared_range * squared_range)
	{
		__PRINTF("Attack beyond range%f\t%f\n",fTmp,attack->attack_range + _parent->body_size);
		//The attack distance is too far, you may need to send a message
		//_runner->be_damaged(msg.source, msg.param, attack->ainfo.sid,0,-1, attack->_attack_state,attack->speed,0);
		return 0;
	}
	__PRINTF("Attack message received, from %d to %d, skillid:%d\n",msg.source.id, _parent->ID.id, attack->skill_id);

	//According to the camp to determine the attack
	int fac_match = (GetFaction() & attack->target_faction);
	if(attack->target_faction == 0 || (attack->_attack_state & AT_STATE_IMMUNE) 
			|| (!fac_match && !attack->force_attack && _invader_state == INVADER_LVL_0))
	{
		//Unmatched attacks for non-forced attacks, automatically ignored (no more attacks)
		//Send invalid effect
		_runner->be_damaged(msg.source, attack->skill_id, attack->ainfo, 0,-1,
				attack->_attack_state| AT_STATE_EVADE,attack->speed,0,attack->section);
		return 0;
	}

	bool orange_name = attack->is_invader && !fac_match;

	damage_entry dmg;
	bool is_hit = AttackJudgement(attack,dmg, fTmp < (attack->short_range * attack->short_range),sqrtf(fTmp));
	//The attack_state here may change
	OnAttacked(pPlane,msg,attack,dmg,is_hit);
	if(!is_hit)
	{
		//Miss, return message
		_runner->dodge_attack(msg.source, attack->skill_id,attack->ainfo
				,attack->_attack_state,attack->speed&0x7F,orange_name,attack->section);
		if(orange_name) SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,attack->ainfo.attacker,60);
		return 0;
	}
	//Judgment of damage dodge
	if(abase::Rand(0,99) < _damage_dodge_rate)
	{
		memset(&dmg,0,sizeof(dmg));
		attack->_attack_state |= AT_STATE_DODGE_DAMAGE;
	}
	//Make a state dodge judgment in advance, because the result will be sent to the client in the OnDamage function?
	if(attack->attached_skill.skill && abase::Rand(0,99) < _debuff_dodge_rate)
	{
		attack->_attack_state |= AT_STATE_DODGE_DEBUFF;
	}
		
	int at_state = attack->_attack_state;

	//Adjust the damage first
	float damage_adjust = 1.0f;
	AdjustDamage(msg,attack,dmg,damage_adjust);
	//Clearing Select damage adjustments	lgc
	ASSERT(!attack->target_layer_adjust_mask || attack->target_layer_adjust_factor > 1.0f);
	ASSERT( (attack->target_layer_adjust_mask & ~0x07) == 0);
	if(attack->target_layer_adjust_mask & (1<<_layer_ctrl.GetLayer()) )	
		damage_adjust *= attack->target_layer_adjust_factor;
	//Judgment on heavy hit
	if(abase::Rand(0,99) < attack->crit_rate - _crit_resistance)
	{
		at_state |= AT_STATE_ATTACK_CRIT;
		damage_adjust *= (CRIT_DAMAGE_BONUS + attack->crit_damage_bonus*0.01f - _crit_damage_reduce*0.01f);
		if(attack->_attack_state & AT_STATE_CRIT_FEEDBACK)
		{
			attack->_attack_state &= ~AT_STATE_CRIT_FEEDBACK;
			if(attack->feedback_filter) SendTo<0>(GM_MSG_ATTACK_CRIT_FEEDBACK,attack->ainfo.attacker,attack->feedback_filter);
		}

		// Mensagem de aviso de dano critico
		_is_critical_damage = true;
	}
	//Carry out the spirit operation
	damage_adjust *= player_template::GetVigourEnhance(attack->vigour, GetVigour(),
			 (IsPlayerClass() || OI_IsPet()) && (attack->ainfo.attacker.IsPlayerClass() || attack->ainfo.attacker.IsPet()) ? 1 : 4);		

	// filter deal with damage
	__at_defense_state = 0;
	_filters.EF_AdjustDamage(dmg,msg.source,*attack,damage_adjust);
	_filters.EF_DoDamage(dmg);
	
	int glyph_else_attr = 0;

	int windex1;
	gplayer * aPlayer = world_manager::GetInstance()->FindPlayer(attack->ainfo.attacker.id , windex1);

	if ( attack->ainfo.attacker.IsPlayerClass() && _parent->ID.IsPlayerClass() )
	{		
		if ( aPlayer )
		{
			glyph_else_attr = ((gactive_imp*)aPlayer->imp)->GlyphGetAttr();
			LuaManager::GetInstance()->EventOnPlayerAttackMsg(_parent->ID.id , attack->ainfo.attacker.id , (aPlayer->base_info.race  & 0x7FFFFFFF), (((gplayer*)_parent)->base_info.race  & 0x7FFFFFFF),  &dmg );
		}
	}
		
	float damage = dmg.physic_damage + dmg.magic_damage[0] + dmg.magic_damage[1] + dmg.magic_damage[2] + dmg.magic_damage[3] + dmg.magic_damage[4];
	ASSERT(MAGIC_CLASS == 5); //Due to the previous addition, add a number to determine
	
	at_state |= __at_defense_state;
	int final_adjust = attack->attack_degree - _defend_degree;
	//if(final_adjust < -90) final_adjust = -90;
	//int int_damage = (int)((damage * damage_adjust + 0.5f) * ( 1.0 + final_adjust*.01f));
	int int_damage;
	if(final_adjust >= 0)
		int_damage = (int)((damage * damage_adjust + 0.5f) * ( 1.0 + final_adjust*.01f));
	else
		int_damage = (int)((damage * damage_adjust + 0.5f) / (1-final_adjust*0.012));
	if( int_damage <=0) int_damage = 1;

	//Nivel de Pico do Atacante
	if(_parent->ID.IsPlayerClass() && attack->highest_level > 0)
	{
		int_damage = int_damage + (100.0 / (int_damage * attack->highest_level));
	}

	// glyph
	if ( _parent->ID.IsPlayerClass() && glyph_else_attr)
	{
		GlyphAttrRelease(glyph_else_attr, int_damage);
	}
	
	OnDamage(msg.source,attack->skill_id, attack->ainfo,int_damage,at_state,attack->speed,orange_name,attack->section);
	InsertDamageEntry(int_damage,attack->speed,attack->ainfo.attacker,orange_name,attack->attacker_mode);

	//deal with bloodsucking
	if(attack->hp_steal_rate > 0)
	{
		int hp_steal = int(attack->hp_steal_rate * 0.01f * int_damage);
		if(hp_steal > 0) SendTo<0>(GM_MSG_HP_STEAL,attack->ainfo.attacker,hp_steal);
	}

	if(orange_name && (_basic.hp > 0 || _invader_state != INVADER_LVL_0))
	{
		//Determine whether to send a fan name message
		SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,attack->ainfo.attacker,60);
	}

	if(_basic.hp == 0)
	{
		//Only the attack with orange name will turn the opponent into red name
		//Only by killing the white name will the opponent become a red name
		Die(attack->ainfo.attacker,orange_name && _invader_state == INVADER_LVL_0,attack->attacker_mode,0);
	}
	else
	{
		 // Infection priority calculation
		if(attack->infected_skill.skill)
		{
			_skill.Infect(this,msg.source,msg.pos,*attack,orange_name);
		}

		//Call the processing interface of the skill
		if(attack->attached_skill.skill)
		{
			if(!(attack->_attack_state & AT_STATE_DODGE_DEBUFF))
				_skill.Attack(this,msg.source,msg.pos,*attack,orange_name);
		}
	}

	// Remove a mensagem de dano crítico
	if(_is_critical_damage)	_is_critical_damage = false;

	return 0;
}


static inline int normalrand(int low,int high)
{
	if(low == high) return low;
	return abase::RandNormal(low,high);
}

	int 
gactive_imp::MakeAttackMsg(attack_msg & attack,char force_attack)
{
	memset(&attack,0,sizeof(attack));
	attack.ainfo.level = _basic.level;
	attack.ainfo.team_id = -1; 
	attack.speed = _cur_item.attack_delay;
	attack.short_range = _cur_item.short_range;
	//attack.short_range_adjust_factor = 0.5f;//lgc
	//attack.target_layer_adjust_mask = 0;
	//attack.target_layer_adjust_factor = 1.0f;
	attack.attack_range = _cur_prop.attack_range;
	attack.attack_attr = attack_msg::PHYSIC_ATTACK;
	attack.physic_damage  = GenerateAttackDamage();
	attack.attack_rate = _cur_prop.attack;
	attack.attacker_faction = GetFaction();
	attack.target_faction = GetEnemyFaction();
	attack.force_attack  = force_attack;
	attack.magic_damage[0]  = normalrand(_cur_prop.addon_damage[0].damage_low,_cur_prop.addon_damage[0].damage_high);
	attack.magic_damage[1]  = normalrand(_cur_prop.addon_damage[1].damage_low,_cur_prop.addon_damage[1].damage_high);
	attack.magic_damage[2]  = normalrand(_cur_prop.addon_damage[2].damage_low,_cur_prop.addon_damage[2].damage_high);
	attack.magic_damage[3]  = normalrand(_cur_prop.addon_damage[3].damage_low,_cur_prop.addon_damage[3].damage_high);
	attack.magic_damage[4]  = normalrand(_cur_prop.addon_damage[4].damage_low,_cur_prop.addon_damage[4].damage_high);

	return 0;
}

int 
gactive_imp::DoAttack(const XID & target,char force_attack)
{
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack);

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,0,&attack,sizeof(attack));
	//try attack
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg);
	__PRINTF("send attack message,target(%d,%d)\n",target.type,target.id);
	//	_plane->BroadcastMessage(msg,10.f,gobject::MSG_MASK_ATTACK);
	return 0;
}

bool gactive_imp::Save(archive & ar)
{//lgc
	ar << _basic << _base_prop << _cur_prop << _cur_item << _cur_rune <<
		_en_point << _en_percent << _faction << _enemy_faction <<
		_damage_reduce << _crit_rate << _base_crit_rate << _crit_damage_bonus << _crit_resistance << _exp_addon << _immune_state << 
		_immune_state_adj <<_layer_ctrl << _elf_en << _combat_state << _refresh_state << 
		_invader_state << _lock_equipment << _lock_inventory << _bind_to_ground << _deny_all_session<<
		_session_state << _session_id << _seal_mode_flag << _idle_mode_flag << _expire_item_date << _last_attack_target << _highest_level << _attack_degree << _defend_degree << _invisible_passive << _invisible_active << _anti_invisible_passive << _anti_invisible_active << 
		_damage_dodge_rate << _debuff_dodge_rate << _hp_steal_rate << 
		_heal_cool_time_adj << _skill_enhance << _penetration << _resilience << _attack_monster << _vigour_base << _vigour_en << _skill_enhance2 << _crit_damage_reduce << _near_normal_dmg_reduce << _near_skill_dmg_reduce << _far_normal_dmg_reduce << _far_skill_dmg_reduce << _mount_speed_en << _exp_sp_factor << _realm_exp_factor
		<< _anti_defense_degree << _anti_resistance_degree << _infected_skill_id << _infected_skill_lvl;
	
	ar.push_back(_magic_damage_reduce,sizeof(_magic_damage_reduce));
	_skill.Store(ar);
	if(_cur_session)
		_cur_session->SaveInstance(ar);
	else
		ar << -1;
	SaveSessionList(ar);
	_filters.Save(ar);
	SaveAllState(ar);
	SaveSetAddon(ar);
	ASSERT(_immune_state_adj_counter.size() == 32);
	ar.push_back(_immune_state_adj_counter.begin(), 32*sizeof(int));

	ar << _plus_enhanced_param;
	ar << _new_shield_retort << _new_shield_retort_paladin;
	ar << _is_critical_damage;
	return true;
}

bool gactive_imp::Load(archive & ar)
{//lgc
	ar >> _basic >> _base_prop >> _cur_prop >> _cur_item >> _cur_rune >>
		_en_point >> _en_percent >> _faction >> _enemy_faction >>
		_damage_reduce >> _crit_rate >> _base_crit_rate >> _crit_damage_bonus >> _crit_resistance >> _exp_addon >> _immune_state >>
		_immune_state_adj >> _layer_ctrl >> _elf_en >> _combat_state >> _refresh_state >> 
		_invader_state >> _lock_equipment >> _lock_inventory >> _bind_to_ground >> _deny_all_session >> _session_state >> _session_id >> _seal_mode_flag >> _idle_mode_flag >> _expire_item_date >> _last_attack_target >> _highest_level >>  _attack_degree >> _defend_degree >> _invisible_passive >> _invisible_active >> _anti_invisible_passive >> _anti_invisible_active >> 
		_damage_dodge_rate >> _debuff_dodge_rate >> _hp_steal_rate >> 
		_heal_cool_time_adj >> _skill_enhance >> _penetration >> _resilience >> _attack_monster >> _vigour_base >> _vigour_en >> _skill_enhance2 >> _crit_damage_reduce >> _near_normal_dmg_reduce >> _near_skill_dmg_reduce >> _far_normal_dmg_reduce >> _far_skill_dmg_reduce >> _mount_speed_en >> _exp_sp_factor >> _realm_exp_factor 
		>> _anti_defense_degree >> _anti_resistance_degree >> _infected_skill_id >> _infected_skill_lvl;
		
	ar.pop_back(_magic_damage_reduce,sizeof(_magic_damage_reduce));
	_skill.Load(ar);
	int session_guid;
	ar >> session_guid;
	if(session_guid >= 0)
	{
		substance * pSub = substance::LoadInstance(session_guid,ar);
		_cur_session = substance::DynamicCast<act_session>(pSub);
		if(!_cur_session)
		{
			_session_state = STATE_SESSION_IDLE;
			delete pSub;
		}
	}
	LoadSessionList(ar);
	_filters.Load(ar);
	LoadAllState(ar);
	LoadSetAddon(ar);
	ASSERT(_immune_state_adj_counter.size() == 32);
	ar.pop_back(_immune_state_adj_counter.begin(), 32*sizeof(int));

	ar >> _plus_enhanced_param;
	ar >> _new_shield_retort >> _new_shield_retort_paladin;
	ar >> _is_critical_damage;
	return true;
}
void 
gactive_imp::SaveAllState(archive &ar)
{
	ar << _cur_form << _idle_mode_flag << _seal_mode_flag << _team_visible_state_flag << _visiable_state_flag << _subscibe_timer;
	unsigned int count = _visible_team_state.size();
	ar << count;
	for(unsigned int i = 0; i < count; i++)
	{
		ar << _visible_team_state[i].id << _visible_team_state[i].time;
	}
	count = _visible_team_state_param.size();
	ar << count;
	if(count)
	{
		ar.push_back(_visible_team_state_param.begin(), count*sizeof(int));
	}

	ASSERT(_visible_state_list.size() == MAX_VISIBLE_STATE_COUNT);
	ar.push_back(_visible_state_list.begin(), MAX_VISIBLE_STATE_COUNT * sizeof(int));
	
	//save subscription list
	count = _subscibe_list.size(); 
	ar << count;
	if(count)
	{
		ar.push_back(_subscibe_list.begin(), count * sizeof(link_sid));
	}
	//Keep reference counts for idle_mode and seal_mode,lgc
	ASSERT(_idle_seal_mode_counter.size() == 4);
	ar.push_back(_idle_seal_mode_counter.begin(), 4 * sizeof(int));
}

void 
gactive_imp::LoadAllState(archive &ar)
{
	ar >> _cur_form >> _idle_mode_flag >> _seal_mode_flag >> _team_visible_state_flag >> _visiable_state_flag >> _subscibe_timer;
	unsigned int count;
	unsigned short tmp;
	int time;
	ar >> count; 
	_visible_team_state.clear();
	for(unsigned int i = 0; i < count; i++)
	{
		ar >> tmp >> time;
		_visible_team_state.push_back(FID(tmp,time));
	}
	int param;
	ar >> count;
	for(unsigned int i=0; i<count; i++)
	{
		ar >> param;
		_visible_team_state_param.push_back(param);
	}

	ASSERT(_visible_state_list.size() == MAX_VISIBLE_STATE_COUNT);
	ar.pop_back(_visible_state_list.begin(), MAX_VISIBLE_STATE_COUNT * sizeof(int));
	
	//Restore subscription list
	ar >> count;
	if(count)
	{
		link_sid lid;
		_subscibe_list.reserve(count);
		_subscibe_list.clear();
		for(unsigned int i = 0; i < count; i ++)
		{
			ar.pop_back(&lid,sizeof(lid));
			_subscibe_list.push_back(lid);
		}
	}
	//Restore reference counts for idle_mode and seal_mode,lgc
	ASSERT(_idle_seal_mode_counter.size() == 4);
	ar.pop_back(_idle_seal_mode_counter.begin(), 4 * sizeof(int));
}

void 
gactive_imp::SaveSetAddon(archive &ar)
{
	unsigned int count = _set_addon_map.size();
	ar << count;
	if(count)
	{
		abase::static_multimap<int,int, abase::fast_alloc<> >::iterator it = _set_addon_map.begin();
		for(;it != _set_addon_map.end(); ++it)
		{
			ar << it->first << it->second;
		}
	}
}

void 
gactive_imp::LoadSetAddon(archive &ar)
{
	unsigned int count;
	ar >> count; 
	_set_addon_map.clear();
	for(unsigned int i = 0; i < count ; i++)
	{
		int id,value;
		ar >> id >> value;
		if(value)
		{
			_set_addon_map[id] = value;
		}
	}
}

void gactive_imp::SaveSessionList(archive & ar)
{
	unsigned int size = _session_list.size();
	ar << size;
	for(unsigned int i = 0; i < size; i ++)
	{
		_session_list[i]->SaveInstance(ar);
	}
}

bool gactive_imp::LoadSessionList(archive & ar)
{
	unsigned int size;
	ar >> size;
	ASSERT(_session_list.empty());
	for(unsigned int i = 0; i < size; i ++)
	{
		substance * pSub = substance::LoadInstance(ar);
		act_session * session = substance::DynamicCast<act_session>(pSub);
		if(!session)
		{
			ASSERT(false);
			delete pSub;
			ClearSession();
			return false;
		}
		_session_list.push_back(session);
	}
	if(!_cur_session)
	{
		ASSERT(_session_state == STATE_SESSION_IDLE);
		StartSession();
	}
	return true;
}


bool gactive_imp::StartSession()
{
	//	ASSERT(_cur_session == NULL);
	if(_cur_session != NULL || _session_state != STATE_SESSION_IDLE) return false;
	bool rst = false;
	while(_cur_session == NULL && HasNextSession())  
	{
		_cur_session = _session_list[0];
		_session_list.erase(_session_list.begin());
		if(GetAction() && _cur_session->IsAttackSession())
		{
			//General attack or skill session conflicts with movement action, just ignore it
			_runner->error_message(S2C::ERR_ATTACK_SESSION_DENYED_IN_ACTION);
			delete _cur_session;
			_cur_session = NULL;
			continue;
		}
		_session_process = true;
		if(!(rst = _cur_session->StartSession(HasNextSession())))
		{
			_session_process = false;
			EndCurSession();
		}
		_session_process = false;
	}
	return rst;
}

bool gactive_imp::EndCurSession()
{
	//ASSERT(_cur_session );	$$$$$$ This verification is temporarily commented out, because the core is found outside
	//The purpose of not checking _session_state here is that some sessions will not change the state
	if(_cur_session == NULL) return false;
	_cur_session->EndSession();
	return SafeDeleteCurSession();
}

bool gactive_imp::RepeatSession()
{
	ASSERT(_cur_session );
	_session_process = true;
	bool flag = _cur_session->RepeatSession();
	_session_process = false;
	return flag;
}

void gactive_imp::ClearNextSession()
{
	abase::clear_ptr_vector(_session_list);
}

bool gactive_imp::SafeDeleteCurSession()
{
	if(_session_process)
	{
		GLog::log(GLOG_ERR,"world[%d] session %s has nested release",world_manager::GetWorldTag(),_cur_session->GetRunTimeClass()->GetName());
		return false;
	}
	else
	{
		_session_state = STATE_SESSION_IDLE;
		delete _cur_session;
		_cur_session = NULL;
		return true;
	}
}

void gactive_imp::ClearSession()
{
	if(_cur_session)
	{
		_cur_session->TerminateSession();
		SafeDeleteCurSession();
	}
	

	if(_session_list.size())
	{
		abase::clear_ptr_vector(_session_list);
	}
}
void 
gactive_imp::ResetSession()
{
	if(_cur_session)
	{
		//TerminateSession should not be called here, because it will cause additional message sending
		//_cur_session->TerminateSession();
		SafeDeleteCurSession();
	}

	if(_session_list.size())
	{
		abase::clear_ptr_vector(_session_list);
	}
}

void 
gactive_imp::TryStopCurSession()
{
	if(_cur_session && _cur_session->TerminateSession(false))
	{
		if(SafeDeleteCurSession())
			StartSession();
	}
}

void gactive_imp::ClearSpecSession(int exclusive_mask)
{
	act_session ** it = _session_list.end();
	for(;it != _session_list.begin();)
	{
		--it;
		if((*(it))->GetMask() & exclusive_mask)
		{
			delete *it;
			_session_list.erase(it);
		}
	}
	ASSERT(it == _session_list.begin());
}

bool gactive_imp::AddSession(act_session * ses)
{
	if(_deny_all_session) 
	{
		delete ses;
		return false;
	}

	int exclusive_mask = ses->GetExclusiveMask();
	if(_session_list.size() >= MAX_PLAYER_SESSION)	//Maximum queue limit exceeded
	{
		__PRINTF("The session queue is full, clear the original queue content\n");
		abase::clear_ptr_vector(_session_list);
	}

	if(exclusive_mask) 
	{
		act_session ** it = _session_list.end();
		for(;it != _session_list.begin();)
		{
			--it;
			if((*(it))->GetMask() & exclusive_mask)
			{
				__PRINTF("Deleted the excluded session %x\n",(*(it))->GetMask());
				delete *it;
				_session_list.erase(it);
			}
		}
		ASSERT(it == _session_list.begin());
	}

	_session_list.push_back(ses);
	return !_cur_session;
}

bool 
gactive_imp::InNonMoveSession()
{
	return _cur_session && !_cur_session->IsMoveSession();
}

bool 
gactive_imp::CheckAttack(const XID & target,bool report_err)
{
	enum
	{
		ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE
	};
	if(!CanAttack(target))
	{	
		if(report_err) _commander->error_cmd(S2C::ERR_CANNOT_ATTACK);
		return false;
	}

	world::object_info info;
	if(!_plane->QueryObject(target,info) ||
		(info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE ||
		((gactive_object*)_parent)->anti_invisible_degree < info.invisible_degree )
	{
		//Unable to hit the enemy, invalid target
		if(report_err) _commander->error_cmd(S2C::ERR_INVALID_TARGET);
		return false;
	}
	float squared_range = _cur_prop.attack_range + info.body_size;
	if(info.pos.squared_distance(_parent->pos) > squared_range * squared_range)
	{
		//Unable to attack the enemy, the distance is too far?
		if(report_err)	_commander->error_cmd(S2C::ERR_OUT_OF_RANGE);
		return false;
	}
	return true;
}

bool 
gactive_imp::CheckAttack(const XID & target,int * flag,float * pDis, A3DVECTOR & pos)
{
	enum
	{
		ALIVE = world::QUERY_OBJECT_STATE_ACTIVE | world::QUERY_OBJECT_STATE_ZOMBIE,

		CANNOT_ATTACK 	= 1,
		TARGET_INVALID 	= 2,
		OUT_OF_RANGE 	= 4,
	};
	*flag = 0;
	if(!CanAttack(target))
	{	
		*flag |= CANNOT_ATTACK;
		return false;
	}

	world::object_info info;
	if(!_plane->QueryObject(target,info) ||
		(info.state & ALIVE) != world::QUERY_OBJECT_STATE_ACTIVE ||
		((gactive_object*)_parent)->anti_invisible_degree < info.invisible_degree )
	{
		//Unable to hit the enemy, invalid target
		*flag |= TARGET_INVALID;
		return false;
	}
	pos = info.pos;
	float squared_range = _cur_prop.attack_range + info.body_size;
	float distance = info.pos.squared_distance(_parent->pos);
	if(distance > squared_range * squared_range)
	{
		//Unable to attack the enemy, the distance is too far?
		*flag |= OUT_OF_RANGE;
		return false;
	}
	if(pDis) *pDis = distance;
	return true;
}

void 
gactive_imp::DoHeartbeat(unsigned int tick)
{
	
	//Check if the status of the session is correct
	ASSERT(_cur_session || (!_cur_session && ! _session_list.size()) );
	
	OnHeartbeat(tick);
	UpdateDataToParent();
	_commander->OnHeartbeat(tick);
	UpdateVisibleState();
//	_backup_hp[0] = _backup_hp[1];
//	_backup_hp[1] = _basic.hp;

	RefreshSubscibeList();

	//Clear change flag
	ClearRefreshState();
	_team_visible_state_flag = false;
}

void 
gactive_imp::ClearSubscibeList()
{
	link_sid * last = _subscibe_list.end();
	link_sid * first = _subscibe_list.begin();
	XID id(GM_TYPE_PLAYER,0);
	for(;last != first;)
	{
		--last;
		id.id = last->user_id;
		//Send a message to deselect the target
		SendTo<0>(GM_MSG_NOTIFY_SELECT_TARGET,id,-1);
	}

	last = _second_subscibe_list.end();
	first = _second_subscibe_list.begin();
	for(;last != first;)
	{
		--last;
		id.id = last->user_id;
		//Send a message to deselect the target
		SendTo<0>(GM_MSG_NOTIFY_SELECT_SUBTARGET,id,-1);
	}
	
	_subscibe_list.clear();
	_second_subscibe_list.clear();
	_subscibe_timer = 0; 
}

void
gactive_imp::RefreshSubscibeList()
{
	SendTeamDataToSubscibeList();
	if(_subscibe_list.size() || _second_subscibe_list.size())
	{
		if(GetRefreshState()) 
			SendDataToSubscibeList();

		_subscibe_timer ++;
		if(_subscibe_timer >= 0x0F)
		{
			//Handle a timeout every 16 seconds
			link_sid * last = _subscibe_list.end();
			link_sid * first = _subscibe_list.begin();
			XID id(GM_TYPE_PLAYER,0);
			for(;last != first;)
			{
				--last;
				id.id = last->user_id;
				world::object_info info;
				if(!_plane->QueryObject(id,info) ||
						(info.state & world::QUERY_OBJECT_STATE_DISCONNECT))
				{
					//Target could not send data
					_subscibe_list.erase_noorder(last);
				}
				else
				{
					//Send a message
					SendTo<0>(GM_MSG_SUBSCIBE_CONFIRM,id,0);
				}
			}
			last = _second_subscibe_list.end();
			first = _second_subscibe_list.begin();
			for (;last != first;)
			{
				--last;
				id.id = last->user_id;
				world::object_info info;
				if (!_plane->QueryObject(id,info))
				{
					//Target could not send data
					_second_subscibe_list.erase_noorder(last);
				}
				else
				{
					//Send a message
					SendTo<0>(GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM,id,0);
				}
			}
			_subscibe_timer = 0; 
		}
		
	}
}

void gactive_imp::NotifyTargetChange(XID& target)
{
	link_sid * last = _subscibe_list.end();
	link_sid * first = _subscibe_list.begin();
	XID id(GM_TYPE_PLAYER,0);
	for(;last != first;)
	{
		--last;
		id.id = last->user_id;
		SendTo<0>(GM_MSG_NOTIFY_SELECT_SUBTARGET,id,target.id);
	}
}

void 
gactive_imp::SendAttackMsg(const XID & target, attack_msg & attack)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg);
			
}

void 
gactive_imp::SendDelayAttackMsg(const XID & target, attack_msg & attack, unsigned int delay_tick)
{
	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,
			0,&attack,sizeof(attack));
	TranslateAttack(target,attack);
	_plane->PostLazyMessage(msg,delay_tick);
			
}

void 
gactive_imp::SendEnchantMsg(int message,const XID & target, enchant_msg & enchant)
{
	MSG msg;
	BuildMessage(msg,message,target,_parent->ID,_parent->pos,
			0,&enchant,sizeof(enchant));
	_plane->PostLazyMessage(msg);
}

void 
gactive_imp::SendDelayEnchantMsg(int message,const XID & target, enchant_msg & enchant, unsigned int delay_tick)
{
	MSG msg;
	BuildMessage(msg,message,target,_parent->ID,_parent->pos,
			0,&enchant,sizeof(enchant));
	_plane->PostLazyMessage(msg, delay_tick);
}

void gactive_imp::ReInit()
{
	if(_cur_session)
	{
		ASSERT(_runner && _commander);
		//This happens only when the server is transferred
		_cur_session->Restore(this,GetNextSessionID());
		for(unsigned int i =0; i < _session_list.size(); i ++)
		{
			_session_list[i]->Restore(this,-1);
		}
	}
	else
	{
		ASSERT(_session_list.size() == 0);
	}
}

void 
gactive_imp::Init(world * pPlane,gobject*parent)
{
	gobject_imp::Init(pPlane,parent);
	_filters.Init(this);
	_server_notify_timestamp = g_timer.get_tick() + 25*20;  //Suppose it is every 25 seconds
}

void 
gactive_imp::FillAttackMsg(const XID & target,attack_msg & attack,int dec_arrow)
{
	attack.ainfo.attacker = _parent->ID;
	attack._attack_state = __at_attack_state;
	attack.ainfo.level = _basic.level;
	attack.ainfo.profit_level = PROFIT_LEVEL_NORMAL;
	attack.attacker_faction = GetFaction();
	attack.target_faction = GetEnemyFaction();
	attack.ainfo.team_id = attack.ainfo.team_seq = attack.ainfo.sid = 0;
	attack.attacker_layer = _layer_ctrl.GetLayer();
	int crit_rate = _crit_rate + _base_crit_rate;
	if(crit_rate > 100) crit_rate = 100;
	attack.crit_rate = crit_rate;
	attack.crit_damage_bonus = _crit_damage_bonus;
	__at_attack_state = 0;
	
	attack.attack_degree += _attack_degree;


	attack.highest_level = _highest_level;
	attack.penetration = _penetration;
	attack.vigour = GetVigour();
	attack.anti_defense_degree = _anti_defense_degree;
	attack.anti_resistance_degree = _anti_resistance_degree;
	attack.infected_skill.skill = _infected_skill_id;
	attack.infected_skill.level = _infected_skill_lvl;
	if((attack.attack_attr == attack_msg::PHYSIC_ATTACK || attack.attack_attr == attack_msg::PHYSIC_ATTACK_HIT_DEFINITE)
			&& attack.short_range <= 0.f) attack.hp_steal_rate = _hp_steal_rate;
}

void 
gactive_imp::FillEnchantMsg(const XID & target,enchant_msg & enchant)
{
	enchant.ainfo.attacker = _parent->ID;
	enchant.ainfo.level = _basic.level;
	enchant.ainfo.profit_level = PROFIT_LEVEL_NORMAL;
	enchant.attacker_faction = GetFaction();
	enchant.target_faction = GetEnemyFaction();
	enchant.ainfo.team_id = enchant.ainfo.team_seq = enchant.ainfo.sid = 0;
	enchant.attack_degree += _attack_degree;	//lgc
	enchant.penetration = _penetration;
	enchant.vigour = GetVigour();
	enchant._attack_state = __at_attack_state;
	__at_attack_state = 0;
	enchant.infected_skill.skill = _infected_skill_id;
	enchant.infected_skill.level = _infected_skill_lvl;
}


void gactive_imp::UpdateVisibleState()
{
	if(_visiable_state_flag)
	{
		//Watch for status bit changes
		unsigned int state = 0;
		unsigned int state2 = 0;
		unsigned int state3 = 0;
		unsigned int state4 = 0;
		unsigned int state5 = 0;
		unsigned int state6 = 0;
		unsigned int state7 = 0;
		unsigned int state8 = 0;
		unsigned int state9 = 0;
		
		for(unsigned int i= 0; i < 32;i ++)
		{
			if(_visible_state_list[i]) state |= 1 << i;
		}
		for(unsigned int i= 32; i < 64;i ++)
		{
			if(_visible_state_list[i]) state2 |= 1 << (i-32);
		}
		for(unsigned int i= 64; i < 96;i ++)
		{
			if(_visible_state_list[i]) state3 |= 1 << (i-64);
		}
		for(unsigned int i= 96; i < 128;i ++)
		{
			if(_visible_state_list[i]) state4 |= 1 << (i-96);
		}
		for(unsigned int i= 128; i < 160;i ++)
		{
			if(_visible_state_list[i]) state5 |= 1 << (i-160);
		}
		for(unsigned int i= 160; i < 192;i ++)
		{
			if(_visible_state_list[i]) state6 |= 1 << (i-192);
		}
		for(unsigned int i= 192; i < 224;i ++)
		{
			if(_visible_state_list[i]) state7 |= 1 << (i-224);
		}
		for(unsigned int i= 224; i < 256;i ++)
		{
			if(_visible_state_list[i]) state8 |= 1 << (i-256);
		}
		for(unsigned int i= 256; i < 288;i ++)
		{
			if(_visible_state_list[i]) state9 |= 1 << (i-288);
		}
	
		//Update state to broadcast data
		gactive_object * pObj = (gactive_object*)_parent;
		if(    pObj->extend_state  != state  || pObj->extend_state2 != state2
			|| pObj->extend_state3 != state3 || pObj->extend_state4 != state4
			|| pObj->extend_state5 != state5 || pObj->extend_state6 != state6
			|| pObj->extend_state7 != state7 || pObj->extend_state8 != state8
			|| pObj->extend_state9 != state9 )
		{		
			//Fix structure and broadcast
			pObj->extend_state = state;
			pObj->extend_state2 = state2;
			pObj->extend_state3 = state3;
			pObj->extend_state4 = state4;
			pObj->extend_state5 = state5;
			pObj->extend_state6 = state6;
			pObj->extend_state7 = state7;
			pObj->extend_state8 = state8;
			pObj->extend_state9 = state9;
			
			_runner->update_visible_state(state, state2, state3, state4, state5, state6, state7, state8, state9);
		}

		_visiable_state_flag = false;
	}
}

void 
gactive_imp::InsertInfoSubscibe(const XID & target, const link_sid & ld)
{
	ASSERT(target.id == ld.user_id);
	if(target == _parent->ID) 
	{
		SendTo<0>(GM_MSG_NOTIFY_SELECT_SUBTARGET,target,GetCurTarget().id); // Rely on this to cancel the previous subtarget
		return; //Subscribing yourself is not necessary
	}
	for(unsigned int i =0; i < _subscibe_list.size(); i ++)
	{
		link_sid & tmp  = _subscibe_list[i];
		if(ld.user_id == tmp.user_id)
		{
			tmp.cs_id = ld.cs_id;
			tmp.cs_sid = ld.cs_sid;
			return;
		}
	}
	_subscibe_list.push_back(ld);
	_runner->query_info00(target,ld.cs_id,ld.cs_sid);
// Let subscribers resubscribe to their target information
	SendTo<0>(GM_MSG_NOTIFY_SELECT_SUBTARGET,target,GetCurTarget().id);
//Immediately send the current state to the subscribed player
	SendTeamVisibleStateToOther(ld.user_id,ld.cs_id,ld.cs_sid);
}

void 
gactive_imp::InsertInfoSecondSubscibe(const XID & target, const link_sid & ld)
{
	ASSERT(target.id == ld.user_id);
	if(target == _parent->ID) return; //Subscribing yourself is not necessary
	for(unsigned int i =0; i < _second_subscibe_list.size(); i ++)
	{
		link_sid & tmp  = _second_subscibe_list[i];
		if(ld.user_id == tmp.user_id)
		{
			tmp.cs_id = ld.cs_id;
			tmp.cs_sid = ld.cs_sid;
			return;
		}
	}
	_second_subscibe_list.push_back(ld);
	_runner->query_info00(target,ld.cs_id,ld.cs_sid);
}

void gactive_imp::SendTeamVisibleStateToOther(int user_id,int cs_index, int cs_sid)
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
	if(!iSize) return;
	packet_wrapper h1(iSize *sizeof(unsigned short) + 32  + (iSize * sizeof(vts_time[0])) );
	using namespace S2C;
	
	for (int i = 0; i < iSize && i < 32; i++ )
	{
		vts_id[i] = _visible_team_state[i].id;
		vts_time[i].time = _visible_team_state[i].time;
		vts_time[i].num = i;
	}

	CMD::Make<CMD::object_state_notify>::From(h1,_parent->ID,vts_id,iSize,_visible_team_state_param.begin(),_visible_team_state_param.size(),(int *)vts_time);
	send_ls_msg(cs_index,user_id,cs_sid,h1.data(),h1.size());
}

void gactive_imp::Swap(gactive_imp * rhs)
{
#define Set(var,cls) var = cls->var
	//Replace the data of gactive_imp
	Set(_basic,rhs);
	Set(_base_prop,rhs); 
	Set(_cur_prop,rhs); 
	Set(_cur_item,rhs); 
	Set(_cur_rune,rhs);
	Set(_en_point,rhs); 
	Set(_en_percent,rhs);
	Set(_faction,rhs); 
	Set(_enemy_faction,rhs);
	Set(_damage_reduce,rhs); 
	Set(_crit_rate,rhs); 
	Set(_base_crit_rate,rhs); 
	Set(_crit_damage_bonus,rhs);
	Set(_crit_damage_reduce,rhs);
	Set(_crit_resistance,rhs);
	Set(_exp_addon,rhs);
	Set(_immune_state,rhs);
	Set(_immune_state_adj,rhs);
	Set(_base_crit_rate,rhs);
	Set(_layer_ctrl,rhs); 
	Set(_elf_en, rhs);//lgc
	Set(_combat_state,rhs); 
	Set(_refresh_state,rhs);
	Set(_invader_state,rhs);
	Set(_lock_equipment,rhs);
	Set(_lock_inventory,rhs);
	Set(_bind_to_ground,rhs);
	Set(_deny_all_session,rhs);
	Set(_session_state,rhs); 
	Set(_session_id,rhs); 
	Set(_seal_mode_flag,rhs); 
	Set(_idle_mode_flag,rhs);
	Set(_expire_item_date, rhs);
	Set(_last_attack_target, rhs);
	Set(_attack_monster, rhs);
	Set(_attack_degree, rhs);
	Set(_highest_level, rhs);
	Set(_defend_degree, rhs);
	Set(_invisible_passive, rhs);
	Set(_invisible_active, rhs);
	Set(_anti_invisible_passive, rhs);
	Set(_anti_invisible_active, rhs);
	Set(_damage_dodge_rate, rhs);
	Set(_debuff_dodge_rate, rhs);
	Set(_hp_steal_rate, rhs);
	Set(_heal_cool_time_adj, rhs);
	Set(_skill_enhance, rhs);
	Set(_penetration, rhs);
	Set(_resilience, rhs);
	Set(_vigour_base, rhs);		
	Set(_vigour_en, rhs);		
	Set(_skill_enhance2, rhs);
	Set(_near_normal_dmg_reduce, rhs);
	Set(_near_skill_dmg_reduce, rhs);
	Set(_far_normal_dmg_reduce, rhs);
	Set(_far_skill_dmg_reduce, rhs);
	Set(_mount_speed_en, rhs);
    Set(_exp_sp_factor, rhs);
    Set(_realm_exp_factor, rhs);
	Set(_anti_defense_degree, rhs);
	Set(_anti_resistance_degree, rhs);
	Set(_infected_skill_id, rhs);
	Set(_infected_skill_lvl, rhs);

	memcpy(_magic_damage_reduce, rhs->_magic_damage_reduce, sizeof(_magic_damage_reduce));
	_skill.Swap(rhs->_skill);
	_set_addon_map.swap(rhs->_set_addon_map);

	_cur_session = rhs->_cur_session;
	rhs->_cur_session = NULL;
	_session_list.swap(rhs->_session_list);
	_filters.Swap(rhs->_filters,this);

	Set(_cur_form,rhs); 
	Set(_idle_mode_flag,rhs); 
	Set(_seal_mode_flag,rhs); 
	Set(_team_visible_state_flag,rhs); 
	Set(_visiable_state_flag,rhs); 
	Set(_subscibe_timer,rhs);

	_visible_team_state.swap(rhs->_visible_team_state);
	_visible_team_state_param.swap(rhs->_visible_team_state_param);
	_visible_state_list.swap(rhs->_visible_state_list);
	_subscibe_list.swap(rhs->_subscibe_list);
	_idle_seal_mode_counter.swap(rhs->_idle_seal_mode_counter);//lgc
	_immune_state_adj_counter.swap(rhs->_immune_state_adj_counter);

	Set(_new_shield_retort,rhs);
	Set(_new_shield_retort_paladin,rhs);
	Set(_is_critical_damage,rhs);

#undef Set
}

int 
gactive_imp::InsertDamageEntry(int damage,int delay,const XID & attacker, bool orange_name, char attacker_mode)
{
	//return DoDamage(damage);
	if(delay <= 0) return DoDamage(damage);
	if(delay >= 255) delay = 255;
	if(damage <=0) damage = 1;

	msg_hurt_extra_info_t data = {orange_name, attacker_mode};
	MSG msg;
	BuildMessage(msg,GM_MSG_HURT,_parent->ID,attacker,_parent->pos,damage,&data,sizeof(data));
	if(attacker_mode & attack_msg::PVP_DUEL)
	{
		msg.message = GM_MSG_DUEL_HURT;
	}
	_plane->PostLazyMessage(msg,delay);
	return damage;
}

void 
gactive_imp::AddMultiObjEffect(const XID& target, char type)
{
	gactive_object * pObject = (gactive_object *)_parent;
	int count = pObject->multiobj_effect_count;
	if(count >= MAX_MULTIOBJ_EFFECT_COUNT) return;
	for(int i=0; i<count; i++)
	{
		if(pObject->multiobj_effect_list[i].target == target.id
				&& pObject->multiobj_effect_list[i].type == type) return;
	}
	pObject->multiobj_effect_list[count].target = target.id;
	pObject->multiobj_effect_list[count].type = type;
	++ pObject->multiobj_effect_count;
	if(pObject->multiobj_effect_count == 1) pObject->object_state |= gactive_object::STATE_MULTIOBJ_EFFECT;
	_runner->add_multiobj_effect(target.id, type);
}

void 
gactive_imp::RemoveMultiObjEffect(const XID& target, char type)
{
	gactive_object * pObject = (gactive_object *)_parent;
	int count = pObject->multiobj_effect_count;
	for(int i=0; i<count; i++)
	{
		if(pObject->multiobj_effect_list[i].target == target.id
				&& pObject->multiobj_effect_list[i].type == type)
		{
			if(i != count-1)
			{
				pObject->multiobj_effect_list[i].target = pObject->multiobj_effect_list[count-1].target;
				pObject->multiobj_effect_list[i].type = pObject->multiobj_effect_list[count-1].type;
			}
			-- pObject->multiobj_effect_count;
			if(pObject->multiobj_effect_count == 0) pObject->object_state &= ~gactive_object::STATE_MULTIOBJ_EFFECT;
			_runner->remove_multiobj_effect(target.id, type);
		}	
	}
}

void
gactive_imp::TransferSpecFilters(int filter_mask, const XID & target, int count)
{
	if(count <= 0) return;
	raw_wrapper ar;	
	int ret = _filters.RandomRemoveSpecFilters(ar, filter_mask, count);
	if(ret > 0)
	{
		SendTo<0>(GM_MSG_TRANSFER_FILTER_DATA, target, ret, ar.data(), ar.size());	
	}
}

void
gactive_imp::AbsorbSpecFilters(int filter_mask, const XID & target, int count)
{
	if(count <= 0) return;
	SendTo<0>(GM_MSG_TRANSFER_FILTER_GET, target, filter_mask, &count, sizeof(count));	
}

void 
gactive_imp::OnDuelStart(const XID & target)
{
	((gactive_object*)_parent)->object_state |= gactive_object::STATE_IN_DUEL_MODE;
}

void 
gactive_imp::OnDuelStop()
{
	((gactive_object*)_parent)->object_state &= ~gactive_object::STATE_IN_DUEL_MODE;
}

void 
gactive_imp::OI_Die()
{
	SendTo<0>(GM_MSG_DEATH,_parent->ID,0);
}

int 
gactive_imp::GetVigour()
{
	int MaxVigour = LuaManager::GetInstance()->GetConfig()->MaxVigour;
	int res = _vigour_base + _vigour_en;
	if( res > MaxVigour ) res = MaxVigour;
	return res;
}


