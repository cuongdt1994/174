#ifndef __ONLINEGAME_GS_ACTIVEOBJ_H__
#define __ONLINEGAME_GS_ACTIVEOBJ_H__

#include "config.h"
#include "world.h"
#include "gimp.h"
#include "object.h"
#include "attack.h"
#include "property.h"
#include "filter_man.h"
#include "aipolicy.h"
#include "skillwrapper.h"
#include <timer.h>
#include <arandomgen.h>
#include <common/protocol.h>
#include <glog.h>
#include "sfilterdef.h"
#include "moving_action_env.h"

//lgc
#pragma pack(1)
struct elf_enhance
{
	short str_point;
	short agi_point;
	short vit_point;
	short eng_point;

	short str_scale;
	short agi_scale;
	short vit_scale;
	short eng_scale;
};
#pragma pack()

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const struct elf_enhance & en)
{
	wrapper.push_back(&en, sizeof(en));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, struct elf_enhance & en)
{
	wrapper.pop_back(&en, sizeof(en));
	return wrapper;
}



namespace GDB { struct itemdata;}
/*
*	Logical judgment structure of the location of the object (temporary)
*/

enum
{
	//The values ??and order here have been used elsewhere 
	LAYER_GROUND,
	LAYER_AIR,
	LAYER_WATER,
	LAYER_INVALID,
};

enum IMMUNE_MASK
{
	IMMUNE_PHYSIC = 0,
	IMMUNE_GOLD,
	IMMUNE_WOOD,
	IMMUNE_WATER,
	IMMUNE_FIRE,
	IMMUNE_EARTH,

	IMMUNE_MASK_PHYSIC	= 0x0001,
	IMMUNE_MASK_GOLD	= 0x0002,
	IMMUNE_MASK_WOOD	= 0x0004,
	IMMUNE_MASK_WATER	= 0x0008,
	IMMUNE_MASK_FIRE	= 0x0010,
	IMMUNE_MASK_EARTH	= 0x0020,

};

struct object_layer_ctrl
{
	enum 
	{
		MODE_GROUND,
		MODE_FLY,
		MODE_FALL,
		MODE_SWIM,
	};

	char _layer;		//What level the object is in, 0 ground, 1 sky, 2 water, controlled by the server instead
	char move_mode;		//player's movement pattern

	char GetLayer() { return _layer;}
	char GetMode() { return move_mode;}
	bool CanSitDown() { return move_mode == MODE_GROUND; }
	bool IsFalling() { return move_mode == MODE_FALL;}
	bool IsOnGround() { return _layer == LAYER_GROUND;}
	bool IsOnAir()
	{
		return _layer == LAYER_AIR || move_mode == MODE_FALL;
	}

	bool IsFlying()
	{
		return move_mode == MODE_FLY;
	}

	bool CheckAttack()
	{
		return move_mode != MODE_FALL;
	}

	//take off
	void TakeOff()
	{
		move_mode = MODE_FLY;
		_layer = LAYER_AIR;
	}

	//landing
	void Landing()
	{
		move_mode = MODE_FALL;
		_layer = LAYER_AIR;
	}

	void Swiming()
	{
		move_mode = MODE_SWIM;
		_layer = LAYER_WATER;
	}

	//landing complete
	void Ground()
	{
		move_mode = MODE_GROUND;
		_layer = LAYER_GROUND;
	}

	void UpdateMoveMode(int mode)
	{
		move_mode = MODE_GROUND;
		if(mode & C2S::MOVE_MASK_SKY) 
			move_mode = MODE_FLY;
		else if(mode & C2S::MOVE_MASK_WATER) 
			move_mode = MODE_SWIM;

		switch(mode & 0x3F)
		{
			case C2S::MOVE_MODE_FALL:
			case C2S::MOVE_MODE_SLIDE:
			case C2S::MOVE_MODE_FLY_FALL:
				move_mode = MODE_FALL;
			break;
		}
	}


	void UpdateStopMove(int mode)
	{
		//Assuming this has been verified
		move_mode = MODE_GROUND;
		if(mode & C2S::MOVE_MASK_SKY) 
			move_mode = MODE_FLY;
		else if(mode & C2S::MOVE_MASK_WATER) 
			move_mode = MODE_SWIM;
	}

	void UpdateLayer(int layer)	//Only used for player calls
	{
		_layer = layer;
	}
};

template <typename WRAPPER> WRAPPER &  operator >>(WRAPPER & ar, object_layer_ctrl & ctrl)
{
	ar.pop_back(&ctrl,sizeof(ctrl)); return ar;
}

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & ar, object_layer_ctrl & ctrl)
{
	ar.push_back(&ctrl,sizeof(ctrl)); return ar;
}


class act_session;
/**
 *	Base class implementation for objects capable of motion
 */
class gactive_imp : public gobject_imp 
{
	#pragma pack(push, 1)
	struct FID
	{
		unsigned short id;
		int time;
		FID(short id, int time)
		{
			this->id = id;
			this->time = time;
		}
	};
	#pragma pack(pop)
	int	_session_id;
	bool _session_process;
protected:
	int _switch_dest;
	A3DVECTOR _switch_pos;
	int _server_notify_timestamp;		//Send existing timers to other servers

	bool _team_visible_state_flag;		//Flag to save the team state whether to refresh
	bool _visiable_state_flag;		//changes to the visible state are saved
	unsigned char _subscibe_timer;
	abase::vector<FID, abase::fast_alloc<> > _visible_team_state; //icons buff 
	abase::vector<int, abase::fast_alloc<> > _visible_team_state_param; 	//Save the state visible when teaming up
	abase::vector<int, abase::fast_alloc<> > _visible_state_list; 		//Save the state parameters visible when forming a team
	abase::vector<link_sid, abase::fast_alloc<> > _subscibe_list;		 //A reference-counted list that holds the visible state
	abase::vector<link_sid, abase::fast_alloc<> > _second_subscibe_list; //subscribe list
	abase::static_multimap<int,int, abase::fast_alloc<> >  _set_addon_map;
	abase::vector<int, abase::fast_alloc<> > _idle_seal_mode_counter;	//Save the reference count lgc of idle_mode and seal_mode

	int  _cur_form;		//Current Transformation Status
	int _idle_mode_flag;		//sleep mode flag
	int _seal_mode_flag;		//seal mode logo
	A3DVECTOR _direction;		//facing direction

public:
	GNET::SkillWrapper _skill;	//skill structure
	unsigned int _faction;		//own camp
	unsigned int _enemy_faction;	//enemy camp
	filter_man _filters;		//filter manager
	basic_prop _basic;		//Basic attribute value
	extend_prop _base_prop;		//The basic attribute value, which excludes the basic value affected by equipment, and these data are also stored in the database
	extend_prop _cur_prop;		//current attribute value
	item_prop  _cur_item;		//The property generated by the item (ontology)
	property_rune _cur_rune;	//The current two attack optimizers
	int _damage_reduce;		//Damage reduction or increase (negative number means increased damage, representing an integer in the percentage)
	int _magic_damage_reduce[MAGIC_CLASS];//Spell damage reduction or increase (negative number means increased damage, representing an integer in the percentage)
	int _crit_rate;			//Critical Hit Probability Indicates the percentage of critical hits that occur
	int _base_crit_rate;		//Agility Critical Chance Bonus
	int _crit_damage_bonus;	//����ı����˺��ӳ�
	int _crit_damage_reduce;//Crit Damage Reduction
	int _crit_resistance;	//Critical Strike Resistance
	int _exp_addon;			//Experience bonus, only valid for players
	int _immune_state;		//The effect of innate immunity
	int _immune_state_adj;		//later added immunity
	abase::vector<int, abase::fast_alloc<> > _immune_state_adj_counter;//Holds the refcount for later attached immunity effects
	enhanced_param _en_point;	//Attributes Enhanced by Points
	scale_enhanced_param _en_percent;//Attributes Enhanced by Percentage
	object_layer_ctrl _layer_ctrl;	//position control structure
	struct elf_enhance _elf_en;	//Wisp skills or other attribute enhancements to Wisps//lgc

	bool _combat_state; 		//Whether the object is in combat state 
	bool _refresh_state;		//Refresh the state, when the properties of the object change, this value should be true
	char _invader_state;		//Pink name status
	bool _lock_equipment;		//Equipment Locked Sign
	bool _lock_inventory;		//Parcel Locked Sign
	char _bind_to_ground;		//Unable to fly this counts now
	char _deny_all_session;		//It is forbidden to join the session operation, this is now a count
	int __at_attack_state;		//Temporarily save the state of some attacks No need to save
	int __at_defense_state;		//Temporarily save some states during defense, this state overlaps with the attack state, no need to save
	int _session_state;		//The state of the current object (this state saves the session state)
	act_session * _cur_session;	//current session
	int _hp_gen_counter;		//Record the count value of blood addition, for a small amount of blood addition
	int _mp_gen_counter;		//Record the count value of blood addition, for a small amount of blood addition
	abase::vector<act_session *, abase::fast_alloc<> > _session_list;	//Consider implementing it with a queue in the future
	int _expire_item_date;          //Whether there is a time-limited item, the expiration time of the latest expiration is saved here
	XID _last_attack_target;        //The target of the last attack, used for anti-addiction judgment
	int _attack_degree;		//attack level
	int _defend_degree;		//defense level
	int _invisible_passive;			//Stealth level enhanced by passive skills, dedicated to assassins
	int _invisible_active;			//Stealth level for active skill boosts; for monsters the stealth level defined in the template is saved
	int _anti_invisible_passive;	//The anti-stealth level of passive skills is improved, only for assassins
	int _anti_invisible_active;		//The anti-invisibility level of the skill item promotion; for monsters, the anti-invisibility level defined in the template is saved
	int _damage_dodge_rate;			//Damage Dodge Chance
	int _debuff_dodge_rate;			//Status Dodge Probability
	int _hp_steal_rate;				//Lifesteal percentage, subject to actual damage
	int _heal_cool_time_adj;		//The cooling time of the red symbol is additionally increased, and the reduction value is in milliseconds
	int _skill_enhance;				//Enhance the damage of using skills, it will only take effect if the target is an npc
	int _penetration;				//Enhanced damage, only effective if the target is an npc
	int _resilience;				//Reduce damage, only effective if the attacker is an npc
	bool _attack_monster;			//Indicates whether the player or pet is fighting monsters
	int _vigour_base;				// Bravery is the base value brought by the realm level
	int _vigour_en;					// The enhancement value obtained by Chimo in skills and equipment
	int _skill_enhance2;			// Amplified skill damage to all objects
	float _near_normal_dmg_reduce;	// Melee basic attack damage reduction
	float _near_skill_dmg_reduce;	// Melee skill damage reduction
	float _far_normal_dmg_reduce;	// Long-distance basic attack damage reduction
	float _far_skill_dmg_reduce;	// Reduced ranged skill damage
	float _mount_speed_en;			// Riding speed increase
    float _exp_sp_factor;           // ����ľ���Ԫ�����ϵ��
    float _realm_exp_factor;        // Additional Realm Experience Adjustment Factor
	int _anti_defense_degree; 		// Ignore physical defense level
	int _anti_resistance_degree; 	// Ignores defense level
	
	int _infected_skill_id;			// Passive Attach Status Pack
	int _infected_skill_lvl;		// 	
	
	int _highest_level;
	bool _new_shield_retort; // Novo escudo paladino/wf
	bool _new_shield_retort_paladin; // Novo escudo paladino/wf
	bool _is_critical_damage;

	moving_action_env _moving_action_env;	//The action that can be executed during the movement, only the Player will use it

	plus_enhanced_param _plus_enhanced_param;//Additional value, this value is calculated after _en_percent and _en_point are calculated.

	enum 
	{
		MAX_HP_DEC_DELAY = 4
	};
	struct damage_delay_t
	{
		XID who;
		int damage;
		bool orange_name;
	};
	
	enum
	{
		INVADER_LVL_0,		//white name
		INVADER_LVL_1,		//fan name
		INVADER_LVL_2,		//red name
	};

	enum
	{
		AT_STATE_ATTACK_RUNE1  	= 0x0001,		//physic attack rune
		AT_STATE_ATTACK_RUNE2  	= 0x0002,		//magic attack rune
		AT_STATE_DEFENSE_RUNE1 	= 0x0004,		//physic defense rune
		AT_STATE_DEFENSE_RUNE2 	= 0x0008,		//magic defense rune
		AT_STATE_ATTACK_CRIT  	= 0x0010,		//crit
		AT_STATE_ATTACK_RETORT	= 0x0020,		//Shock attack
		AT_STATE_EVADE			= 0x0040,		//invalid attack
		AT_STATE_IMMUNE			= 0x0080,		//Immune to this attack
		AT_STATE_ENCHANT_FAILED	= 0x0100,		//In fact, it is only used when the skill catches pets
		AT_STATE_ENCHANT_SUCCESS= 0x0200,		//In fact, it is only used when the skill catches pets
		AT_STATE_DODGE_DAMAGE	= 0x0400,		//damage dodge
		AT_STATE_DODGE_DEBUFF	= 0x0800,		//status dodge
		AT_STATE_ATTACK_AURA	= 0x1000,		//halo attack
		AT_STATE_REBOUND        = 0x2000,		//rebound
		AT_STATE_BEAT_BACK      = 0x4000,		//fight back
		AT_STATE_CRIT_FEEDBACK  = 0x8000,       //crit feedback
	};

	enum
	{
		STATE_SESSION_IDLE,
		STATE_SESSION_MOVE,
		STATE_SESSION_ATTACK,
		STATE_SESSION_USE_SKILL,
		STATE_SESSION_GATHERING,
		STATE_SESSION_TRAHSBOX,
		STATE_SESSION_EMOTE,
		STATE_SESSION_OPERATE,
		STATE_SESSION_TRADE,
		STATE_SESSION_ZOMBIE,
		STATE_SESSION_SKILL,
		STATE_SESSION_COSMETIC,
		STATE_SESSION_GENERAL,
	};

	enum 
	{
		SEAL_MODE_NULL		= 0x00,
		SEAL_MODE_ROOT		= 0x01,
		SEAL_MODE_SILENT 	= 0x02,

		IDLE_MODE_NULL		= 0x00,
		IDLE_MODE_SLEEP		= 0x01,
		IDLE_MODE_STUN		= 0x02,
	};
	//lgc
	enum	//seal/idle mode Index counted in _idle_seal_mode_counter
	{
		MODE_INDEX_SLEEP	= 0,
		MODE_INDEX_STUN,	
		MODE_INDEX_ROOT,
		MODE_INDEX_SILENT,
	};

	enum	//notify_root,Body type in the dispel_root protocol
	{
		ROOT_TYPE_SLEEP = 0,
		ROOT_TYPE_STUN,
		ROOT_TYPE_ROOT,
	};

	inline void UpdateDataToParent()		//Update your own basic data in _parent
	{
		gactive_object * pObj  = (gactive_object*)_parent;
		pObj->base_info.faction = GetFaction();	//alignment may change
		pObj->base_info.level = _basic.level;
		pObj->base_info.hp = _basic.hp;
		pObj->base_info.max_hp = _cur_prop.max_hp;
		pObj->base_info.mp = _basic.mp;
	}

	inline void SetAttackMonster(bool flag) {_attack_monster = flag;}
	inline bool IsAttackMonster() {return _attack_monster;}
	inline void ActiveCombatState(bool state) {_combat_state = state; if(!_combat_state) _attack_monster = false;} 
	inline bool IsCombatState() {return _combat_state;} 
	inline bool GetRefreshState() {return _refresh_state;}
	inline void SetRefreshState() {_refresh_state = true;} 
	inline void ClearRefreshState() {_refresh_state = false;} 
	inline void RecalcDirection() {_parent->dir = a3dvector_to_dir(_direction);} 
	inline void SetDirection(unsigned char dir) {_parent->dir = dir;}
	inline void CheckVisibleDataForTeam() {if(_team_visible_state_flag) SendTeamDataToMembers();}
	inline int ActivateSetAddon(int addon_id)
	{
		addon_id &= 0xFFFF;
		return ++_set_addon_map[addon_id];
	}

	inline int DeactivateSetAddon(int addon_id)
	{
		addon_id &= 0xFFFF;
		return --_set_addon_map[addon_id];
	}
	inline int  OI_GetInvaderState() { return _invader_state;}

	inline void UpdateExpireItem(int date)
	{
		if(_expire_item_date <= 0)
		{
			_expire_item_date = date;
		}
		else if(_expire_item_date > date)
		{
			_expire_item_date = date;
		}
	}

public:
	typedef bool (*attack_judge)(gactive_imp * __this , const MSG & msg, attack_msg & amsg);
	typedef bool (*enchant_judge)(gactive_imp * __this, const MSG & msg, enchant_msg & emsg);
	typedef void (*attack_fill)(gactive_imp * __this, attack_msg & attack);
	typedef void (*enchant_fill)(gactive_imp * __this, enchant_msg & enchant);
public:

	gactive_imp();
	~gactive_imp();
	virtual void Init(world * pPlane,gobject*parent);
	virtual void ReInit();
public:
	void SaveAllState(archive &ar);
	void SaveSetAddon(archive &ar);
	void LoadAllState(archive &ar);
	void LoadSetAddon(archive &ar);
	bool StartSession();
	bool EndCurSession();
	bool RepeatSession();
	void TryStopCurSession();	//Trying to stop the current session may not be successful, and will try to start at the same time
	bool AddSession(act_session * ses);
	act_session * GetCurSession() { return _cur_session;}
	act_session * GetNextSession() { if(HasNextSession()) return _session_list[0]; else return NULL;}
	bool HasNextSession() { return _session_list.size();}
	bool HasSession() { return _cur_session || _session_list.size();}
	bool InNonMoveSession();	//Whether a non-mobile session is in progress
	void SaveSessionList(archive & ar);
	bool LoadSessionList(archive & ar);
	void ClearSession();
	void ClearMoveSession();
	void ClearAttackSession();
	void ClearSpecSession(int exclusive_mask);
	void ResetSession();	//cur_session does not end with endsession
	void ClearNextSession();
	bool CurSessionValid(int id);
	int GetCurSessionID() { return _session_id;}
	inline int GetNextSessionID() 
	{
		_session_id++;
		_session_id &= 0x7FFFFFFF;
		return _session_id;
	}
	
	inline moving_action * GetAction(){ return _moving_action_env.GetAction(); }
	inline bool StartAction(moving_action * pAction){ return _moving_action_env.StartAction(pAction); }
	inline void TryBreakAction(){ return _moving_action_env.TryBreakAction(); } 
	inline void RestartAction(){ return _moving_action_env.RestartAction(); } 
	inline void ClearAction(){ return _moving_action_env.ClearAction(); } 
	inline void ReleaseAction(){ return _moving_action_env.ReleaseAction(); } 
	inline bool ActionOnAttacked(int action_id)
	{
		if(!_moving_action_env.ActionValid(action_id)) return true;
		return _moving_action_env.ActionOnAttacked(); 
	}


public:
//virtual function
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	virtual int MessageHandler(world * pPlane ,const MSG & msg);
	virtual int DoAttack(const XID & target,char force_attack);
	virtual bool CanAttack(const XID & target) { return true;}
	virtual bool CheckLevitate(){ return false;}
	virtual void PhaseControlInit(){}
	virtual const XID & GetCurTarget(){ ASSERT(false); return _parent->ID;}
	virtual const XID & GetLeaderID(){ ASSERT(false); return _parent->ID;}
	virtual int GetAmmoCount() { return 0;}
	virtual void OnHeal(const XID & healer, int life){}
	virtual int TakeOutItem(int item_id) { return -1; }
	virtual int TakeOutItem(int item_id, unsigned int count){ return -1; }//lgc
	virtual void TakeOutItem(const int * id_list, unsigned int list_count, unsigned int count){}
	virtual bool CheckItemExist(int item_id, unsigned int count) {return false;}
	virtual bool CheckItemExist(int inv_index, int item_id, unsigned int count) {return false;}
	virtual bool CheckItemExist(const int * id_list, unsigned int list_count, unsigned int count) {return false;}
	virtual int CheckItemPrice(int inv_index, int item_id) { return 0;}
	virtual void DropSpecItem(bool isProtected, const XID & owner){}
	//virtual unsigned int GetMoneyAmount(bool is_silver) { return 0;}
	//virtual void DecMoneyAmount(unsigned int money, bool is_silver) {}
	//virtual void DropMoneyAmount(unsigned int money, bool isProtected, bool is_silver){}
	virtual bool UseProjectile(int count) { return true;}
	virtual bool OI_IsMember(const XID & member) { return false;}
	virtual bool OI_IsInTeam() { return false;}
	virtual bool OI_IsTeamLeader() { return false;}
	virtual int SpendFlyTime(int tick) {return 1;}
	virtual int GetFlyTime() {return 0;}
	virtual bool Resurrect(float exp_reduce) {return false;}
	virtual void EnterResurrectReadyState(float exp_reduce, float hp_factor, float mp_factor) {}
	virtual void KnockBack(const XID & target, const A3DVECTOR & source, float distance,int time,int stun_time) {}
	virtual void PullOver(const XID & target, const A3DVECTOR & source, float distance, int time){}
	virtual void Teleport(const A3DVECTOR & pos, int time, char mode){}
	virtual void Teleport2(const A3DVECTOR & pos, int time, char mode){}
	virtual void KnockUp(float distance, int time) {}
	virtual bool DrainMana(int mana) { return true; }
	virtual void SendDataToSubscibeList() = 0;	//Send self data to player
	virtual void SendTeamDataToSubscibeList() = 0;	//Send team visibility information to the selected object
	virtual void SendTeamDataToMembers(){} ;	//Send team visible information to teammates
	virtual void SetIdleMode(bool sleep, bool stun) { _idle_mode_flag = (sleep?IDLE_MODE_SLEEP:0) | (stun?IDLE_MODE_STUN:0); } 
	virtual void SetSealMode(bool silent,bool root) { _seal_mode_flag = (silent?SEAL_MODE_SILENT:0) | (root?SEAL_MODE_ROOT:0); }
	virtual void SendAttackMsg(const XID & target, attack_msg & attack);
	virtual void SendDelayAttackMsg(const XID & target, attack_msg & attack, unsigned int delay_tick);
	virtual int GetCSIndex() { return -1;}
	virtual int  GetCSSid() { return -1;}
	virtual void SendEnchantMsg(int message,const XID & target, enchant_msg & attack);
	virtual void SendDelayEnchantMsg(int message,const XID & target, enchant_msg & attack, unsigned int delay_tick);
	virtual void SendMsgToTeam(const MSG & msg,float range,bool exclude_self){}
	virtual void AddNPCAggro(const XID & who,int rage){}
    virtual void RemoveNPCAggro(const XID& src, const XID& dest, float ratio) {}
	virtual void BeTaunted(const XID & who,int rage){}
	virtual void AddAggroToEnemy(const XID & who,int rage){}
	virtual void ClearAggroToEnemy(){}
	virtual void SetCombatState() {}
	virtual bool CheckInvaderAttack(const XID & who) {return false;}	//Check yourself for the orange name of this target
	virtual void FillAttackMsg(const XID & target, attack_msg & attack, int dec_arrow = 0);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int GetObjectClass() { return -1;}			//Occupation of the subject
	virtual bool CheckCoolDown(int idx) { return true;}
	virtual void SetCoolDown(int idx, int msec) {}
	virtual void CoolDownReduceAtr(int idx, int msec) {}
	virtual void NotifySkillStillCoolDown(int cd_id){}
	virtual int GetMonsterFaction() { return 0;}
	virtual int GetFactionAskHelp() { return 0;}
	virtual void SetLifeTime(int life) {} //Set lifespan, only NPC has this corresponding
	virtual void EnhanceBreathCapacity(int value) {}
	virtual void ImpairBreathCapacity(int value) {}
	virtual void InjectBreath(int value) {}
	virtual void EnableEndlessBreath(bool bVal) {}
	virtual void AdjustBreathDecPoint(int offset) {}
	virtual void EnableFreePVP(bool bVal) {}
	virtual void ObjReturnToTown() {}
	virtual void AddEffectData(short effect) {}  		//For player use only
	virtual void RemoveEffectData(short effect) {}		//For player use only
	virtual void AddMultiObjEffect(const XID& target, char type);
	virtual void RemoveMultiObjEffect(const XID& target, char type);
	virtual void EnterCosmeticMode(unsigned short inv_index,int cos_id) {}		//only for player
	virtual void LeaveCosmeticMode(unsigned short inv_index) {}			//For player use only
	virtual void SetPerHitAP(int ap_per_hit){}
	virtual void ModifyPerHitAP(int delta){}
	virtual bool IsPlayerClass() { return 0;}
	virtual bool IsEquipWing() { return false;}
	virtual int GetLinkIndex() { return -1;}
	virtual int GetLinkSID() { return -1;}
	virtual int SummonMonster(int mod_id, int count, const XID& target, int target_distance, int remain_time=0, char die_with_who=0, int path_id=0){ return -1; }
	virtual int SummonNPC(int npc_id, int count, const XID& target, int target_distance, int remain_time=0){ return -1;}
	virtual int SummonMine(int mine_id, int count, const XID& target, int target_distance, int remain_time=0){ return -1;}
	virtual bool UseSoulItem(int type, int level, int power) {return false;}	//For player use only
	virtual void IncAntiInvisiblePassive(int val){}//For player use only
	virtual void DecAntiInvisiblePassive(int val){}//only for player
	virtual void IncAntiInvisibleActive(int val){}
	virtual void DecAntiInvisibleActive(int val){}
	virtual void IncInvisiblePassive(int val){}//only for player
	virtual void DecInvisiblePassive(int val){}//only for player
	virtual void SetInvisible(int invisible_degree){}//Invalid parameter for npc
	virtual void ClearInvisible(){}
	virtual int GetSoulPower(){ return 0; }
	virtual void EnhanceSoulPower(int val){}
	virtual void ImpairSoulPower(int val){}
	virtual void UpdateMinAddonExpireDate(int addon_expire){}
	virtual void SetGMInvisible(){}
	virtual void ClearGMInvisible(){}
	virtual bool ActivateSharpener(int id, int equip_index){ return false; }
	virtual void TransferSpecFilters(int filter_mask, const XID & target, int count);
	virtual void AbsorbSpecFilters(int filter_mask, const XID & target, int count);
	virtual bool SummonPet2(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack){ return false; }
	virtual bool SummonPlantPet(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack){ return false; }
	virtual bool CalcPetEnhance(int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree,int& vigour){ return false; }
	virtual bool PetSacrifice(){ return false; }
	virtual bool PlantSuicide(float distance, const XID & target, char force_attack){ return false; }
	virtual void InjectPetHPMP(int hp, int mp){}
	virtual void DrainPetHPMP(const XID & pet_id, int hp, int mp){}
	virtual void DrainLeaderHPMP(const XID & attacker, int hp, int mp){}
	virtual void LongJumpToSpouse(){}
	virtual void WeaponDisabled(bool disable){}
	virtual void ElmoDisabled(bool disable){}
	virtual void DetectInvisible(float range){}
	virtual void ForceSelectTarget(const XID & target){}
	virtual void ExchangePosition(const XID & target){}
	virtual void SetSkillAttackTransmit(const XID & target){}
	virtual void CallUpTeamMember(const XID& member){}
	virtual void QueryOtherInventory(const XID& target){}
	virtual void IncPetHp(int inc){}
	virtual void IncPetMp(int inc){}
	virtual void IncPetDamage(int inc){}
	virtual void IncPetMagicDamage(int inc){}
	virtual void IncPetDefense(int inc){}
	virtual void IncPetMagicDefense(int inc){}
	virtual void IncPetAttackDegree(int inc){}
	virtual void IncPetDefendDegree(int inc){}
	virtual void IncAggroOnDamage(const XID & attacker, int val){}
	virtual void DecAggroOnDamage(const XID & attacker, int val){}
	virtual void FestiveAward(int fa_type,int type,const XID& target){}
	virtual void AdjustLocalControlID(int& cid){}
	virtual int  GetMazeRoomIndex() { return 0;}
	virtual void ReduceResurrectExpLost(int value){}
	virtual void IncreaseResurrectExpLost(int value){}
	virtual void SetPlayerLimit(int index, bool b){}
	virtual bool GetPlayerLimit(int index){ return false;}
	virtual void DenyAttackCmd(){}
	virtual void AllowAttackCmd(){}
	virtual void DenyElfSkillCmd(){}
	virtual void AllowElfSkillCmd(){}
	virtual void DenyUseItemCmd(){}
	virtual void AllowUseItemCmd(){}
	virtual void DenyNormalAttackCmd(){}
	virtual void AllowNormalAttackCmd(){}
	virtual void DenyPetCmd(){}
	virtual void AllowPetCmd(){}
	virtual void TurretOutOfControl(){}
	virtual void EnterNonpenaltyPVPState(bool b){}
	virtual int GetHistoricalMaxLevel(){ return _basic.level; }
	virtual int GetAvailLeadership(){ return 0; }	
	virtual void OccupyLeadership(int v){}
	virtual void RestoreLeadership(int v){}
	virtual unsigned int OI_GetInventorySize() { return 0;}
	virtual unsigned int OI_GetEmptySlotSize() { return 0;}
	virtual int OI_GetInventoryDetail(GDB::itemdata * list, unsigned int size) { return -1;}
	virtual unsigned int OI_GetMallOrdersCount() { return 0;}
	virtual int OI_GetMallOrders(GDB::shoplog * list, unsigned int size) { return 0;}
	virtual int TradeLockPlayer(int get_mask,int put_mask) { return -1;}
	virtual int TradeUnLockPlayer() { return -1;}
	virtual void OnDuelStart(const XID & target);
	virtual void OnDuelStop();
	virtual void Die(const XID & attacker, bool is_pariah, char attacker_mode, int taskdead);	//taskdead = 0 non-task death = 1 task lossy death = 2 task lossless death
	virtual void ActiveMountState(int mount_id, unsigned short mount_color) {};
	virtual void DeactiveMountState() {};
	virtual bool AddPetToSlot(void * data, int inv_index) { return false;}
	virtual bool FeedPet(int food_mask, int hornor) { return false;}
	virtual bool OI_IsMafiaMember() { return false;}
	virtual int OI_GetMafiaID() { return 0;}
	virtual char OI_GetMafiaRank() { return 0;}
	virtual bool OI_IsMafiaMaster() { return false;}
	virtual bool OI_IsFactionAlliance(int fid) { return false;}
	virtual bool OI_IsFactionHostile(int fid) { return false;}
	virtual int OI_GetSpouseID(){ return 0; }
	virtual int OI_GetReputation(){ return 0; }
	virtual bool CheckGMPrivilege() { return false;}
	virtual int GetFaction() { return _faction;}
	virtual int GetEnemyFaction() { return _enemy_faction;}
	virtual unsigned int OI_GetTrashBoxCapacity(int where) { return 0;}
	virtual int OI_GetTrashBoxDetail(int where, GDB::itemdata * list, unsigned int size) { return -1;}
	virtual bool OI_IsTrashBoxModified() {return false;}
	virtual bool OI_IsEquipmentModified() {return false;}
	virtual unsigned int OI_GetTrashBoxMoney() {return 0;}
	virtual int OI_GetEquipmentDetail(GDB::itemdata * list, unsigned int size) { return -1;}
	virtual unsigned int OI_GetEquipmentSize() { return 0;}
	virtual int OI_GetDBTimeStamp() { return 0;}
	virtual int OI_InceaseDBTimeStamp() { return 0;}
	virtual bool CheckWaypoint(int point_index, int & point_domain) { return false;}
	virtual bool ReturnWaypoint(int point) { return false;}
	virtual attack_judge GetPetAttackHook() { return NULL;}
	virtual enchant_judge GetPetEnchantHook() { return NULL;}
	virtual attack_fill GetPetAttackFill() { return NULL;}
	virtual enchant_fill GetPetEnchantFill() { return NULL;}
	virtual bool OI_IsPVPEnable() { return false;}
	virtual char OI_GetForceAttack() { return 0;}
	virtual bool OI_IsInPVPCombatState() { return  false;}
	virtual bool OI_IsInventoryFull() { return true;}
	virtual bool OI_IsPet() { return false;}
	virtual bool IsPetActive() { return false;}
	virtual int OI_GetPetEggID() { return 0;}
	virtual XID OI_GetPetID() { return XID(-1,-1);}
	virtual void OI_ResurrectPet() {} 
	virtual void OI_RecallPet() {}
	virtual void OI_TransferPetEgg(const XID & who, int pet_egg){}
	virtual void OI_Disappear() {}
	virtual void OI_Die();
	virtual void Notify_StartAttack(const XID & target,char force_attack) {}
	virtual bool OI_GetMallInfo(int & cash, int & cash_used, int &cash_delta,  int &order_id) { return false;}
	virtual bool OI_IsCashModified() { return false;}
	virtual void ActivePetNoFeed(bool feed) {}
	virtual bool OI_TestSafeLock() { return false;}
	virtual unsigned int OI_GetPetsCount() { return 0; }
	virtual unsigned int OI_GetPetSlotCapacity() { return 0; }
	virtual pet_data * OI_GetPetData(unsigned int index){ return NULL; }
	virtual void OI_TryCancelPlayerBind(){}
	virtual int OI_GetTaskMask(){ return 0; }
	virtual int OI_GetForceID(){ return 0; }
	virtual void UpdateMallConsumptionShopping(int id, unsigned int proc_type, int count, int total_price){};
	virtual void UpdateMallConsumptionBinding(int id, unsigned int proc_type, int count){}
	virtual void UpdateMallConsumptionDestroying(int id, unsigned int proc_type, int count){}
	virtual bool CalcPetEnhance3(const pet_data *pData, int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree,int& vigour,int& pen_fis,int& pen_mag){ return false; }
	virtual bool CalcPetEnhance4(const pet_data *pData, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag){ return false; }
	virtual	void GetNatureSkill(int nature,int &skill1,int &skill2) {}
	virtual int OI_GetRealm() { return 0; }
	virtual int GetLocalVal(int index) { return 0;}
	virtual void SetLocalVal(int index,int val) {}	
	virtual void DeliverTaskToTarget(const XID& target, int taskid) {}
	virtual int ChangeVisibleTypeId(int tid) { return -1;}
    virtual void SetHasPVPLimitFilter(bool has_pvp_limit_filter) {}
	virtual void SetTargetCache(const XID& target) {}
	virtual void DispatchTaskToDmgList(int taskid, int count, int dis) {}
	virtual void EnhanceMountSpeedEn(float sp) {}
	virtual void ImpairMountSpeedEn(float sp) {}
	
	virtual void EnhanceFlySpeedEn(float sp) {}
	virtual void ImpairFlySpeedEn(float sp) {}	
	
	virtual float GetMountSpeedEnhance() const { return _mount_speed_en;}
	virtual void SetInfectSkill(int skill,int level) { _infected_skill_id = skill; _infected_skill_lvl = level; }
	virtual int  GetInfectLevel(int skill) { if(_infected_skill_id == skill) return  _infected_skill_lvl ; return -1; }
	virtual int  UseFireWorks2(char is_cast_action, int target_role_id, int item_type, const char * target_user_name){return -1;}
	virtual int  AddFixPositionEnergy(int item_id){return -1;}
	virtual int  SkillSenderContinue() { return 0; }
	
	virtual bool CooldownManager(char type, int idx, time_t time) { return false; }
	virtual bool CooldownManagerMassive(char type, unsigned int * table) { return false; }
	
	virtual int GetInvPosItemTable(int inv , int * table) { return -1; }
	virtual int GatcherMineSupressor(int mine_id , int time) { return time; }
	
	virtual bool IO_IsBattleOffense() { return false; }
	virtual bool IO_IsBattleDefence() { return false; }
	
	virtual void ShortJump(A3DVECTOR & newpos){}
	virtual void ShortJump(A3DVECTOR newpos, int tag){}
	virtual void SetBodyScale(float scale) {}
	virtual void EnhanceShieldEnergyMax(int val){}
	virtual void ImpairShieldEnergyMax(int val){}
	virtual void EnhanceShieldGen(int val){}
	virtual void ImpairShieldGen(int val){}
	virtual bool ShieldEnergyUseSkill(float cost) { return false; }
	virtual bool EnhanceShieldEnergy(float cost) { return false; }
	virtual bool ImpairShieldEnergy(float cost) { return false; }
	virtual void GlyphAttrRelease(int else_attr, int & damege) {}
	virtual int GetOldSecLevel() { return 0; }
	virtual int GlyphGetAttr() { return 0; }
	virtual bool SummonPet3(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack){ return false; }
	virtual void SystemRealmExpDaily(int mobid) { return; }

	virtual int AutoGenStat(int cooldown_idx, int idx ,int offset, bool bSave) { return 0; }

	// Segment
	virtual void GetClearSkillCD(int skill_id) {}
	virtual void GetUseSkillCD(int skill_id, unsigned int cdlimit, unsigned int coolingtime) {}

	// Novos vstate Paladino
	virtual void GetPaladinWeaponLight( unsigned char state ) {}

	// Novas Cargas do Mercenario
	virtual void EnhanceChargeMerc(int val){}
	virtual void ImpairChargeMerc(int val){}
	virtual int GetCountChargeMerc(){}
	
	// money 172+
	virtual unsigned int GetGoldAmount() { return 0;}
	virtual bool DecGoldAmount(unsigned int value) {}

	virtual unsigned int GetSilverAmount() { return 0;}
	virtual bool DecSilverAmount(unsigned int value) {}

	virtual unsigned long long GetAllMoneyAmount() { return 0;}
	virtual bool DecAllMoneyAmount(unsigned int value) {}
	
	virtual void DropGoldAmount(unsigned int value, bool isProtected){}	
	virtual void DropSilverAmount(unsigned int value, bool isProtected){}	
	// money 172+ end

	//history

	virtual void IncPetTime(int val){}
	virtual void DecPetTime(int val){}
	virtual int GetPetTime(){}
	virtual int OI_GetPetType(){}

	virtual void SetRealWeaponClass(int weapon_class){}
	virtual int GetRealWeaponClass(){}

	virtual void SetFakeWeaponClass(int weapon_class){}
	virtual int GetFakeWeaponClass(){}

	virtual void AutoGenStatAmulet(int cooldown_idx, int idx ,int offset, bool bSave){}

	//history end

public:
	inline void TranslateAttack(const XID & target , attack_msg & attack)
	{
		_filters.EF_TransSendAttack(target,attack);
	}

	inline void TranslateEnchant(const XID & target , enchant_msg & enchant)
	{
		_filters.EF_TransSendEnchant(target,enchant);
	}
	
//inlnie logical operation
	inline void TakeOff()	//take off
	{
		_layer_ctrl.TakeOff();
		((gactive_object*)_parent)->object_state |= gactive_object::STATE_FLY;
		_runner->takeoff();
	}

	inline void Landing()
	{
		_layer_ctrl.Landing();
		((gactive_object*)_parent)->object_state &= ~gactive_object::STATE_FLY;
		_runner->landing();
	}

	inline void UpdateStopMove(int move_mode) { _layer_ctrl.UpdateStopMove(move_mode); } 
	inline void UpdateMoveMode(int move_mode) { _layer_ctrl.UpdateMoveMode(move_mode); }
	inline void DecSkillPoint(int sp)
	{
		ASSERT(sp > 0 && sp <= _basic.skill_point);
		_basic.skill_point -= sp;
		_runner->cost_skill_point(sp);
		GLog::log(GLOG_INFO,"User %d consumed sp %d",_parent->ID.id,sp);
	}
	inline void Heal(const XID & healer, int life)
	{
		if(_parent->IsZombie()) return;
		int newhp = _basic.hp + life;
		if(newhp >= _cur_prop.max_hp)
		{
			newhp = _cur_prop.max_hp;
		}
		life = newhp - _basic.hp;
		_basic.hp = newhp;
		SetRefreshState();
		OnHeal(healer,life);
	}
	inline void Heal(int life)
	{
		if(_parent->IsZombie()) return;
		if(_basic.hp <_cur_prop.max_hp)
		{
			_basic.hp += life;
			if(_basic.hp >= _cur_prop.max_hp)
			{
				_basic.hp = _cur_prop.max_hp;
			}
			SetRefreshState();
		}
	}
	inline void HealBySkill(const XID & healer, int life)
	{
		_filters.EF_AdjustHeal(life,1);
		if(life > 0) Heal(healer,life);	
	}
	inline void HealBySkill(int life)
	{
		_filters.EF_AdjustHeal(life,1);
		if(life > 0) Heal(life);
	}
	inline void HealByPotion(int life)
	{
		_filters.EF_AdjustHeal(life,0);
		if(life > 0) Heal(life);
	}
	inline void InjectMana(int mana)
	{
		if(_basic.mp < _cur_prop.max_mp)
		{
			_basic.mp += mana;	
			if(_basic.mp > _cur_prop.max_mp)
			{
				_basic.mp = _cur_prop.max_mp;
			}
			SetRefreshState();
		}
	}

	//Check for mobility
	bool CheckMove(int usetime,int move_mode)
	{
		if(usetime  < 80 || usetime > 1000) return -1;
		if((move_mode & C2S::MOVE_MASK_SKY) && !_layer_ctrl.IsFlying())
		{
			return false;
		}

		//Is it too strict here??
		if(!(move_mode & C2S::MOVE_MASK_SKY) && _layer_ctrl.IsFlying())
		{
			return false;
		}
		return true;
	}

	/*float GetSpeedByMode(int mode)
	{
		float speed[]={_cur_prop.run_speed,_cur_prop.flight_speed,_cur_prop.swim_speed,_cur_prop.run_speed};
		int index = (mode & (C2S::MOVE_MASK_SKY|C2S::MOVE_MASK_WATER)) >> 6; //0 ground 1, sky,2 water, 3 other
		return speed[index];
	}*/

	bool CheckStopMove(const A3DVECTOR & target,int usetime,int move_mode)
	{
		return true;
	}

	inline int TestInvader(bool & orange_name,char force_attack,const XID & attacker)
	{
		orange_name = false;
		if(_invader_state == INVADER_LVL_1)
		{
			//Pink name ignores other logos
			bool self_is_invader = CheckInvaderAttack(attacker);
			if(!self_is_invader)
			{
				//I am not an attacker
				if(!force_attack)
				{	
					//Non-aggressive and not an attacker themselves
					return -1;
				}
				else 
				{
					//turns pink needs to return a message from the past and turns orange
					orange_name = true;
				}
			}
			//I'm an attacker, so I don't have to say anything
			//about to be beaten
		}
		else
		{
			if(_invader_state == INVADER_LVL_0 )
			{
				//turns pink needs to return a message from the past and turns orange
				orange_name = true;
			}
			else
			{
				//Definitely a red name is a legal attack, nothing needs to be done
				//But it is possible that the attack message does not match, so an error may be returned
				if(!force_attack) return 1;
			}
		}
		return 0;
	}
	inline void ObjectSitDown()
	{
		((gactive_object*)_parent)->object_state |= gactive_object::STATE_SITDOWN;
	}

	inline void ObjectStandUp()
	{
		((gactive_object*)_parent)->object_state &= ~gactive_object::STATE_SITDOWN;
	}

	inline void ChangeShape(int shape) 
	{ 
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->shape_form = shape & 0xFF;
		pObj->object_state &= ~gactive_object::STATE_SHAPE;
		if(shape) pObj->object_state |= gactive_object::STATE_SHAPE;

		_cur_form = (shape&0xC0)>>6; 
	}

	inline int GetForm() { return _cur_form; }
	inline void SetFormCls(int form) { _cur_form = form; }

	//Revise long-term expression strategies
	inline void SetEmoteState(unsigned short emote)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->emote_form = emote;
		pObj->object_state |= gactive_object::STATE_EMOTE;
	}
	inline void ClearEmoteState()
	{
		gactive_object * pObj = (gactive_object *)_parent;
		pObj->emote_form = 0;
		pObj->object_state &= ~gactive_object::STATE_EMOTE;
	}

	inline void LockEquipment(bool is_lock)
	{
		_lock_equipment = is_lock;
	}

	inline void LockInventory(bool is_lock)
	{
		_lock_inventory = is_lock;
	}
	
	
	inline void BindToGound(bool is_bind)
	{
		_bind_to_ground += is_bind?1:-1;
	}

	inline bool IsBindGound()
	{
		return _bind_to_ground;
	}

	inline void ForbidBeSelected(bool b)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		if(b)
		{
			pObj->object_state |= gactive_object::STATE_FORBIDBESELECTED;
		}
		else
		{
			pObj->object_state &= ~gactive_object::STATE_FORBIDBESELECTED;
		}
		_runner->forbid_be_selected(b);
	}

	inline void SetDebuffSeeker1(bool b)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		if(b)
		{
			pObj->object_state2 |= gactive_object::STATE_DEBUFF_SK1;
		}
		else
		{
			pObj->object_state2 &= ~gactive_object::STATE_DEBUFF_SK1;
		}
	}

	inline void SetDebuffSeeker2(bool b)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		if(b)
		{
			pObj->object_state2 |= gactive_object::STATE_DEBUFF_SK2;
		}
		else
		{
			pObj->object_state2 &= ~gactive_object::STATE_DEBUFF_SK2;
		}
	}

	inline void SetDebuffSeeker3(bool b)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		if(b)
		{
			pObj->object_state2 |= gactive_object::STATE_DEBUFF_SK3;
		}
		else
		{
			pObj->object_state2 &= ~gactive_object::STATE_DEBUFF_SK3;
		}
	}

	inline void SetDebuffSeeker4(bool b)
	{
		gactive_object * pObj = (gactive_object *)_parent;
		if(b)
		{
			pObj->object_state2 |= gactive_object::STATE_DEBUFF_SK4;
		}
		else
		{
			pObj->object_state2 &= ~gactive_object::STATE_DEBUFF_SK4;
		}
	}

public:
//The family of functions affected by equipment
	template <typename BASE_DATA , typename SCALE_DATA>
		inline void WeaponItemEnhance(short weapon_type,short weapon_delay,int weapon_class,int weapon_level,int attack_speed,float attack_range, float attack_short_range,
				const BASE_DATA & base, const SCALE_DATA & scale)
		{
			_cur_item.weapon_type 		= weapon_type;
			_cur_item.weapon_delay		= weapon_delay;
			_cur_item.weapon_class		= weapon_class;
			_cur_item.weapon_level 		= weapon_level;
			_cur_item.attack_speed		= attack_speed;
			_cur_item.attack_range		= attack_range;
			_cur_item.short_range		= 0; //attack_short_range (Removido Fragilidade a distancia Arqueiro)
			_cur_item.damage_low		= base.damage_low;
			_cur_item.damage_high		= base.damage_high;
			_cur_item.damage_magic_low	= base.magic_damage_low;
			_cur_item.damage_magic_high	= base.magic_damage_high;

			_en_point.defense 		+= base.defense;
			_en_point.armor			+= base.armor;

			_en_percent.defense 		+= scale.defense;
			_en_percent.armor		+= scale.armor;
			_skill.EventWield(this,weapon_class);
		}

	template <typename BASE_DATA , typename SCALE_DATA>
		inline void WeaponItemImpair(const BASE_DATA & base, const SCALE_DATA & scale)
		{
			_skill.EventUnwield(this,_cur_item.weapon_class);
			_cur_item.weapon_type 		= 0;
			_cur_item.weapon_delay		= UNARMED_ATTACK_DELAY;
			_cur_item.weapon_class		= 0;
			_cur_item.weapon_level		= 0;
			_cur_item.damage_low		= 0;
			_cur_item.damage_high		= 0;
			_cur_item.damage_magic_low	= 0;
			_cur_item.damage_magic_high	= 0;
			_cur_item.attack_speed		= 0;
			_cur_item.attack_range		= 0;
			_cur_item.short_range		= 0;

			_en_point.defense 		-= base.defense;
			_en_point.armor			-= base.armor;

			_en_percent.defense 		-= scale.defense;
			_en_percent.armor		-= scale.armor;
		}

	template <typename BASE_DATA , typename SCALE_DATA>
		inline void ArmorEnhance(const BASE_DATA & base, const SCALE_DATA & scale,int hp, int mp)
		{
			_en_point.defense		+= base.defense;
			_en_point.armor			+= base.armor;

			_en_point.damage_low		+= base.damage_low;
			_en_point.damage_high		+= base.damage_high;
			_en_point.magic_dmg_low		+= base.magic_damage_low;
			_en_point.magic_dmg_high	+= base.magic_damage_high;

			_en_percent.damage		+= scale.damage;
			_en_percent.magic_dmg		+= scale.magic_damage;

			_en_point.max_hp += hp;
			_en_point.max_mp += mp;
		}

	template <typename BASE_DATA , typename SCALE_DATA>
		inline void ArmorImpair(const BASE_DATA & base, const SCALE_DATA & scale,int hp, int mp)
		{
			_en_point.defense 		-= base.defense;
			_en_point.armor			-= base.armor;

			_en_point.damage_low		-= base.damage_low;
			_en_point.damage_high		-= base.damage_high;
			_en_point.magic_dmg_low		-= base.magic_damage_low;
			_en_point.magic_dmg_high	-= base.magic_damage_high;

			_en_percent.damage		-= scale.damage;
			_en_percent.magic_dmg		-= scale.magic_damage;

			_en_point.max_hp -= hp;
			_en_point.max_mp -= mp;
		}

	template <typename BASE_DATA , typename SCALE_DATA>
		inline void NormalEnhance(const BASE_DATA & base, const SCALE_DATA & scale)
		{
			_en_point.damage_low		+= base.damage_low;
			_en_point.damage_high		+= base.damage_high;
			_en_point.defense 		+= base.defense;
			_en_point.armor			+= base.armor;
			_en_point.magic_dmg_low		+= base.magic_damage_low;
			_en_point.magic_dmg_high	+= base.magic_damage_high;

			_en_percent.defense 		+= scale.defense;
			_en_percent.armor		+= scale.armor;
			_en_percent.damage		+= scale.damage;
			_en_percent.magic_dmg		+= scale.magic_damage;
		}

	template <typename BASE_DATA , typename SCALE_DATA>
		inline void NormalImpair(const BASE_DATA & base, const SCALE_DATA & scale)
		{
			_en_point.damage_low		-= base.damage_low;
			_en_point.damage_high		-= base.damage_high;
			_en_point.defense 		-= base.defense;
			_en_point.armor			-= base.armor;
			_en_point.magic_dmg_low		-= base.magic_damage_low;
			_en_point.magic_dmg_high	-= base.magic_damage_high;

			_en_percent.defense 		-= scale.defense;
			_en_percent.armor		-= scale.armor;
			_en_percent.damage		-= scale.damage;
			_en_percent.magic_dmg		-= scale.magic_damage;
		}
		
	inline void EnhanceResistance(int res[MAGIC_CLASS])
	{
		_en_point.resistance[0] += res[0];
		_en_point.resistance[1] += res[1];
		_en_point.resistance[2] += res[2];
		_en_point.resistance[3] += res[3];
		_en_point.resistance[4] += res[4];
	}

	inline void ImpairResistance(int res[MAGIC_CLASS])
	{
		_en_point.resistance[0] -= res[0];
		_en_point.resistance[1] -= res[1];
		_en_point.resistance[2] -= res[2];
		_en_point.resistance[3] -= res[3];
		_en_point.resistance[4] -= res[4];
	}

	inline void EnhanceAllResistance(int res)
	{
		_en_point.resistance[0] += res;
		_en_point.resistance[1] += res;
		_en_point.resistance[2] += res;
		_en_point.resistance[3] += res;
		_en_point.resistance[4] += res;
	}

	inline void ImpairAllResistance(int res)
	{
		_en_point.resistance[0] -= res;
		_en_point.resistance[1] -= res;
		_en_point.resistance[2] -= res;
		_en_point.resistance[3] -= res;
		_en_point.resistance[4] -= res;
	}

	inline void EnhanceScaleAllResistance(int res)
	{
		_en_percent.resistance[0] += res;
		_en_percent.resistance[1] += res;
		_en_percent.resistance[2] += res;
		_en_percent.resistance[3] += res;
		_en_percent.resistance[4] += res;
	}

	inline void ImpairScaleAllResistance(int res)
	{
		_en_percent.resistance[0] -= res;
		_en_percent.resistance[1] -= res;
		_en_percent.resistance[2] -= res;
		_en_percent.resistance[3] -= res;
		_en_percent.resistance[4] -= res;
	}
	
	inline void EnhanceAllMagicDamageReduce(int mdr)
	{
		_magic_damage_reduce[0] += mdr;	
		_magic_damage_reduce[1] += mdr;	
		_magic_damage_reduce[2] += mdr;	
		_magic_damage_reduce[3] += mdr;	
		_magic_damage_reduce[4] += mdr;	
	}

	inline void ImpairAllMagicDamageReduce(int mdr)
	{
		_magic_damage_reduce[0] -= mdr;	
		_magic_damage_reduce[1] -= mdr;	
		_magic_damage_reduce[2] -= mdr;	
		_magic_damage_reduce[3] -= mdr;	
		_magic_damage_reduce[4] -= mdr;	
	}

	inline void TitleEnhance(int phyd,int magicd,int phydf,int magicdf,int attack,int armor, int hp)
	{
		_en_point.damage_low	+= phyd;
		_en_point.damage_high	+= phyd;
		_en_point.magic_dmg_low	+= magicd;
		_en_point.magic_dmg_high+= magicd;
		_en_point.attack 		+= attack;
		_en_point.armor			+= armor;
		_en_point.defense		+= phydf;
		// Novo
		_en_point.max_hp			+= hp;
		EnhanceAllResistance(magicdf);
	}

	inline void TitleImpair(int phyd,int magicd,int phydf,int magicdf,int attack,int armor, int hp)
	{
		_en_point.damage_low	-= phyd;
		_en_point.damage_high	-= phyd;
		_en_point.magic_dmg_low	-= magicd;
		_en_point.magic_dmg_high-= magicd;
		_en_point.attack 		-= attack;
		_en_point.armor			-= armor;
		_en_point.defense		-= phydf;
		// Novo
		_en_point.max_hp			-= hp;
		ImpairAllResistance(magicdf);
	}

	inline void EnhanceVigour(int vigour)
	{
		_vigour_en += vigour;
	}
	
	inline void ImpairVigour(int vigour)
	{
		_vigour_en -= vigour;
	}

	inline void GeneralCardEnhance(int max_hp, int damage_low, int damage_high, int damage_magic_low, int damage_magic_high, int defense, int res[5], int vigour)
	{
		_en_point.max_hp 			+= max_hp;
		_en_point.damage_low        += damage_low;
		_en_point.damage_high       += damage_high;
		_en_point.magic_dmg_low     += damage_magic_low;
		_en_point.magic_dmg_high    += damage_magic_high;
		_en_point.defense       	+= defense;
		_en_point.resistance[0] 	+= res[0];
		_en_point.resistance[1] 	+= res[1];
		_en_point.resistance[2] 	+= res[2];
		_en_point.resistance[3] 	+= res[3];
		_en_point.resistance[4] 	+= res[4];
		_vigour_en 					+= vigour;
	}

	inline void GeneralCardImpair(int max_hp, int damage_low, int damage_high, int damage_magic_low, int damage_magic_high, int defense, int res[5], int vigour)
	{
		_en_point.max_hp 			-= max_hp;
		_en_point.damage_low        -= damage_low;
		_en_point.damage_high       -= damage_high;
		_en_point.magic_dmg_low     -= damage_magic_low;
		_en_point.magic_dmg_high    -= damage_magic_high;
		_en_point.defense       	-= defense;
		_en_point.resistance[0] 	-= res[0];
		_en_point.resistance[1] 	-= res[1];
		_en_point.resistance[2] 	-= res[2];
		_en_point.resistance[3] 	-= res[3];
		_en_point.resistance[4] 	-= res[4];
		_vigour_en 					-= vigour;
	}

public:
//Attack function series
	inline int GetMagicRuneDamage()
	{
		if(_cur_rune.rune_level_min)
		{
			//Here to do the corresponding operation
			if(_cur_rune.rune_type)
			{
				if(_cur_rune.rune_level_max >= _cur_item.weapon_level 
						&& _cur_rune.rune_level_min <= _cur_item.weapon_level)
				{
					__at_attack_state |= AT_STATE_ATTACK_RUNE2;
					int enh = _cur_rune.rune_enhance;
					//reduce optimizer
					OnUseAttackRune();
					return enh;
				}
			}
		}
		return 0;
	}
	
	inline int GetPhysicRuneDamage()
	{
		if(_cur_rune.rune_level_min)
		{
			//Here to do the corresponding operation
			if(!_cur_rune.rune_type)
			{
				if(_cur_rune.rune_level_max >= _cur_item.weapon_level 
						&& _cur_rune.rune_level_min <= _cur_item.weapon_level)
				{
					__at_attack_state |= AT_STATE_ATTACK_RUNE1;
					int enh = _cur_rune.rune_enhance;
					OnUseAttackRune();
					return enh;
				}
			}
		}
		return 0;
	}
	
	inline int GenerateEquipPhysicDamage()
	{
		//Get basic attack power
		int low = _base_prop.damage_low  + _en_point.damage_low  + _cur_item.damage_low;
		int high= _base_prop.damage_high + _en_point.damage_high + _cur_item.damage_high;
		low = abase::Rand(low,high);
		return low;
	}
	
	inline int GenerateEquipMagicDamage()
	{
		//Get basic attack power
		int low = _base_prop.damage_magic_low  + _en_point.magic_dmg_low  + _cur_item.damage_magic_low;
		int high= _base_prop.damage_magic_high + _en_point.magic_dmg_high + _cur_item.damage_magic_high;
		low = abase::Rand(low,high);
		return low;
	}

	inline int GeneratePhysicDamage(int scale_damage, int point_damage)
	{
		//Get basic attack power
		int low = _base_prop.damage_low  + _en_point.damage_low  + _cur_item.damage_low;
		int high= _base_prop.damage_high + _en_point.damage_high + _cur_item.damage_high;
		low = abase::Rand(low,high);

		//Calculation of optimizer effects
		low += GetPhysicRuneDamage();
		
		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.damage + _en_percent.base_damage + scale_damage);
		low = (int)(low * scale);

		//Calculate extra values,Avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.damage;

		//Return to Attack Z
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	inline int GenerateMaigicDamage2(int scale_damage, int point_damage)
	{
		//Get basic attack power
		int low = _base_prop.damage_magic_low  + _en_point.magic_dmg_low  + _cur_item.damage_magic_low;
		int high= _base_prop.damage_magic_high + _en_point.magic_dmg_high + _cur_item.damage_magic_high;
		low = abase::Rand(low,high);
		
		//Calculation of optimizer effects
		low += GetMagicRuneDamage();

		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.magic_dmg + _en_percent.base_magic + scale_damage);
		low = (int)(low * scale);

		//Calculate additional values ??to avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.magic_dmg;
		
		//return attack
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	//100% de critico

	inline int GenerateCritPhysicDamage(int scale_damage, int point_damage)
	{	
		//Get basic attack power
		int low = _base_prop.damage_low  + _en_point.damage_low  + _cur_item.damage_low;
		int high= _base_prop.damage_high + _en_point.damage_high + _cur_item.damage_high;
		low = abase::Rand(low,high);

		//Calculation of optimizer effects
		low += GetPhysicRuneDamage();
		
		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.damage + _en_percent.base_damage + scale_damage);
		low = (int)(low * scale);

		//Calculate additional values ??to avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.damage;

		//return attack
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	inline int GenerateCritMaigicDamage2(int scale_damage, int point_damage)
	{		
		//Get basic attack power
		int low = _base_prop.damage_magic_low  + _en_point.magic_dmg_low  + _cur_item.damage_magic_low;
		int high= _base_prop.damage_magic_high + _en_point.magic_dmg_high + _cur_item.damage_magic_high;
		low = abase::Rand(low,high);
		
		//Calculation of optimizer effects
		low += GetMagicRuneDamage();

		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.magic_dmg + _en_percent.base_magic + scale_damage);
		low = (int)(low * scale);

		//Calculate additional values ??to avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.magic_dmg;
		
		//return attack
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	inline int GenerateAttackDamage()
	{
		int damage = abase::Rand(_cur_prop.damage_low,_cur_prop.damage_high);
		damage += GetPhysicRuneDamage();
		return damage;
	}

	inline int GenerateMagicDamage()
	{
		int damage = abase::Rand(_cur_prop.damage_magic_low,_cur_prop.damage_magic_high);
		damage += GetMagicRuneDamage();
		return damage;
	}

	inline int GeneratePhysicDamageWithoutRune(int scale_damage, int point_damage)
	{
		//Get basic attack power
		int low = _base_prop.damage_low  + _en_point.damage_low  + _cur_item.damage_low;
		int high= _base_prop.damage_high + _en_point.damage_high + _cur_item.damage_high;
		low = abase::Rand(low,high);

		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.damage + _en_percent.base_damage + scale_damage);
		low = (int)(low * scale);

		//Calculate additional values ??to avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.damage;

		//return attack
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	inline int GenerateMaigicDamage2WithoutRune(int scale_damage, int point_damage)
	{
		//Get basic attack power
		int low = _base_prop.damage_magic_low  + _en_point.magic_dmg_low  + _cur_item.damage_magic_low;
		int high= _base_prop.damage_magic_high + _en_point.magic_dmg_high + _cur_item.damage_magic_high;
		low = abase::Rand(low,high);
		
		//Calculate Proportional Markup
		float scale = 0.01f*(float)(100 + _en_percent.magic_dmg + _en_percent.base_magic + scale_damage);
		low = (int)(low * scale);

		//Calculate additional values ??to avoid the influence of en_percent
		plus_enhanced_param &plus = _plus_enhanced_param;
		low += plus.magic_dmg;
		
		//return attack
		low = low + point_damage;
		if(low < 0) low = 0;
		return low;
	}

	inline int GenerateAttackDamageWithoutRune()
	{
		int damage = abase::Rand(_cur_prop.damage_low,_cur_prop.damage_high);
		return damage;
	}

	inline int GenerateMagicDamageWithoutRune()
	{
		int damage = abase::Rand(_cur_prop.damage_magic_low,_cur_prop.damage_magic_high);
		return damage;
	}

	//Set the type of the next attack to anti-shock attack, which is valid once
	inline void SetRetortState()
	{
		__at_attack_state = AT_STATE_ATTACK_RETORT;
	}

	inline bool GetRetortState()
	{
		return __at_attack_state & AT_STATE_ATTACK_RETORT;
	}

	inline void SetAuraAttackState()
	{
		__at_attack_state = AT_STATE_ATTACK_AURA;	
	}
	
	inline bool GetAuraAttackState()
	{
		return __at_attack_state & AT_STATE_ATTACK_AURA;	
	}

	inline void SetReboundState()
	{
		__at_attack_state = AT_STATE_REBOUND;	
	}

	inline void SetBeatBackState()
	{
		__at_attack_state = AT_STATE_BEAT_BACK;	
	}

	inline void DoDamageReduce(float & damage)
	{
		if(_damage_reduce)
		{
			int dr= _damage_reduce;
			if(dr > 40) dr = 40;
			damage *= (100.f -  dr)*0.01f;
		}
	}
	
	inline void DoMagicDamageReduce(unsigned int cls, float & damage)
	{
		if(_magic_damage_reduce[cls])
		{
			int dr= _magic_damage_reduce[cls];
			if(dr > 40) dr = 40;
			damage *= (100.f -  dr)*0.01f;
		}
	}
	int MakeAttackMsg(attack_msg & attack,char force_attack);
	bool CheckAttack(const XID & target,bool report_err=true);
	bool CheckAttack(const XID & target,int * flag,float * pDis ,A3DVECTOR & pos);

public:
//Message sending functione sending function
	template<int>
		void SendTo(int message,const XID & target, int param) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param);
			_plane->PostLazyMessage(msg);
		}

	template<int>
		void LazySendTo(int message,const XID & target, int param, int latancy) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param);
			_plane->PostLazyMessage(msg,latancy);
		}

	template<int>
		void LazySendTo(int message,const XID & target, int param, int latancy,const void * buf, unsigned int len) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param,buf,len);
			_plane->PostLazyMessage(msg,latancy);
		}

	template<int>
		void SendTo(int message,const XID & target, int param,const void * buf, unsigned int len) 
		{
			MSG msg;
			BuildMessage(msg,message,target,_parent->ID,_parent->pos,param,buf,len);
			_plane->PostLazyMessage(msg);
		}

	void SendTeamVisibleStateToOther(int user_id,int cs_index, int cs_sid);


	inline void SetMaxAP(int max_ap)
	{
		ASSERT(max_ap >=0);
		_cur_prop.max_ap += max_ap - _base_prop.max_ap;  
		_base_prop.max_ap = max_ap;  
		SetRefreshState();
	}
	
	inline void ModifyAP(int ap)
	{
		ap += _basic.ap;
		if(ap < 0)
		{
			ap = 0;
		}
		else if(ap > _cur_prop.max_ap) 
		{
			ap = _cur_prop.max_ap;
		}
		if(_basic.ap != ap)
		{
			_basic.ap = ap;
			SetRefreshState();
		}
	}
	
	inline void SetMaxShieldEnergy(int max_shieldenergy) // Define Max Shield Energy
	{
		/*
		ASSERT(max_shieldenergy >=0);
		_cur_prop.max_shieldenergy += max_shieldenergy - _base_prop.max_shieldenergy;  
		_base_prop.max_shieldenergy = max_shieldenergy;  
		SetRefreshState();
		*/
	}
	
	inline void ModifyShieldEnergy(int shieldenergy) // Modify Shield Energy
	{
		/*
		shieldenergy += _basic.shieldenergy;
		if(shieldenergy < 0)
		{
			shieldenergy = 0;
		}
		else if(shieldenergy > _cur_prop.max_shieldenergy) 
		{
			shieldenergy = _cur_prop.max_shieldenergy;
		}
		if(_basic.shieldenergy != shieldenergy)
		{
			_basic.shieldenergy = shieldenergy;
			SetRefreshState();
		}
		*/
	}

    inline void ModifyHP(int hp)
    {
        hp += _basic.hp;
        if (hp < 0)
        {
            hp = 0;
        }
        else if (hp > _cur_prop.max_hp)
        {
            hp = _cur_prop.max_hp;
        }

        if (_basic.hp != hp)
        {
            _basic.hp = hp;
            SetRefreshState();
        }
    }

    inline void ModifyScaleHP(int hp)
    {
        if (hp > 0)
        {
            ModifyHP(_cur_prop.max_hp * hp / 100);
        }
    }

	void ClearSubscibeList();

	inline bool IsExistTeamVisibleState(unsigned short state)
	{
		unsigned int count = _visible_team_state.size();
		for(unsigned int i = count; i > 0; i--)
		{
			unsigned int index = i-1;
			if(_visible_team_state[index].id == state)
			{
				return true;
			}
		}
		return false;
	}
protected:

	void Swap(gactive_imp * rhs);
	void InsertInfoSubscibe(const XID & target, const link_sid & ld);
	void RemoveInfoSubscibe(const XID & target)
	{
		link_sid * last = _subscibe_list.end();
		link_sid * first = _subscibe_list.begin();
		for(;last != first;)
		{
			--last;
			if(target.id == last->user_id)
			{
				_subscibe_list.erase_noorder(last);
				if(_subscibe_list.empty() && _second_subscibe_list.empty()) 
					_subscibe_timer = 0;
				return;
			}
		}
	}
	void InsertInfoSecondSubscibe(const XID & target, const link_sid & ld);
	void RemoveInfoSecondSubscibe(const XID & target)
	{
		link_sid * last = _second_subscibe_list.end();
		link_sid * first = _second_subscibe_list.begin();
		for(;last != first;)
		{
			--last;
			if(target.id == last->user_id)
			{
				_second_subscibe_list.erase_noorder(last);
				if(_subscibe_list.empty() && _second_subscibe_list.empty()) 
					_subscibe_timer = 0;
				return;
			}
		}
	}
	void RefreshSubscibeList();			//Refresh subscription list Required to be called before clearing the visible state of the team and its own flag
	void NotifyTargetChange(XID& target);          //Notification Subscription List Own Goal Changes

	inline void IncVisibleState(unsigned short state)
	{
		int & counter= _visible_state_list[state];
		//ASSERT(counter >= 0);
		if(!counter) _visiable_state_flag= true;
		counter ++;

	}
	
	inline void DecVisibleState(unsigned short state)
	{
		int & counter= _visible_state_list[state];
		//ASSERT(counter > 0);
		if(counter > 0)
		{
			if(counter == 1) _visiable_state_flag= true;
			counter --; 
		}
		else
		{
			ASSERT(false);
			GLog::log(GLOG_ERR,"FATALERROR: DECVEISBLESTATE state %d,counter:%d\n:",state,counter);
		}
	}

	inline void ClearVisibleState(unsigned short state)
	{
		int & counter= _visible_state_list[state];
		ASSERT(counter >= 0);
		if(counter) 
		{
			_visiable_state_flag= true;
			counter = 0;
		}	
	}
	inline unsigned short __TVSGetState(unsigned short s){ return s & 0x3FFF; }
	inline unsigned int __TVSGetParamCnt(unsigned short s){ return (s & 0xC000) >> 14; }

	inline void InsertTeamVisibleState(unsigned short state, int * param, unsigned int param_count, int time)
	{
		ASSERT(param_count >= 0 && param_count <= 3);
		unsigned int count = _visible_team_state.size();
		for(unsigned int i = count ; i > 0; i--) 
		{
			unsigned int index = i - 1;
			if(__TVSGetState(_visible_team_state[index].id) == state)
			{
				_visible_team_state[index].time = time;
				return;
			}
		}
		if(param_count) state |= ((param_count & 0x03) << 14);
	
		_visible_team_state.push_back(FID(state,time));
		for(unsigned int m=0; m<param_count; m++)
			_visible_team_state_param.push_back(param[m]);
			
		_team_visible_state_flag = true;

	}

	inline void RemoveTeamVisibleState(unsigned short state)
    {
        unsigned int param_index = _visible_team_state_param.size();
        unsigned int count = _visible_team_state.size();
        for(unsigned int i = count ; i > 0; i--) 
        {
            unsigned int index = i - 1;
            unsigned int param_count = __TVSGetParamCnt(_visible_team_state[index].id);
            if(param_count) param_index -= param_count;

                if(__TVSGetState(_visible_team_state[index].id) == state || _visible_team_state[index].id == state)
                {
                    _visible_team_state.erase(_visible_team_state.begin() + index);
                    if(param_count)    _visible_team_state_param.erase(_visible_team_state_param.begin() + param_index, _visible_team_state_param.begin() + param_index + param_count);
                    
                    _team_visible_state_flag = true;
                }
        }
    }

inline void ModifyTeamVisibleState(unsigned short state, int * param, unsigned int param_count, int time)
    {
        ASSERT(param_count >= 1 && param_count <= 3);
        unsigned int param_index = _visible_team_state_param.size();
        unsigned int count = _visible_team_state.size();
        for(unsigned int i = count ; i > 0; i--) 
        {
            unsigned int index = i - 1;
            unsigned int pcount = __TVSGetParamCnt(_visible_team_state[index].id);
            if(pcount) param_index -= pcount;
            if(__TVSGetState(_visible_team_state[index].id) == state || _visible_team_state[index].id == state)
            {
                ASSERT(param_count == pcount);
                
                _visible_team_state[index].time = time;
                
                for(unsigned int m=0; m<param_count; m++)
                    _visible_team_state_param[param_index + m] = param[m];
                
                _team_visible_state_flag = true;
                return;    
            }
        }
    }
	
	void UpdateVisibleState();
	//lgc
	inline void IncIdleSealMode(unsigned char mode)
	{
		ASSERT(mode < 4);
		int & counter= _idle_seal_mode_counter[mode];
		ASSERT(counter >= 0);
		if(!counter)
		{
			if(mode == MODE_INDEX_SLEEP)
			{
				bool sleep, stun;
				GetIdleMode(sleep, stun);
				SetIdleMode(true, stun);

				((gactive_object*)_parent)->object_state2 |= gactive_object::STATE_SLEEP;

				_runner->notify_root(ROOT_TYPE_SLEEP);
			}
			else if(mode == MODE_INDEX_STUN)
			{
				bool sleep, stun;
				GetIdleMode(sleep, stun);
				SetIdleMode(sleep, true);

				((gactive_object*)_parent)->object_state2 |= gactive_object::STATE_STUN;

				_runner->notify_root(ROOT_TYPE_STUN);
			}
			else if(mode == MODE_INDEX_ROOT)
			{
				bool silent, root;
				GetSealMode(silent, root);
				SetSealMode(silent, true);

				((gactive_object*)_parent)->object_state2 |= gactive_object::STATE_ROOT;

				_runner->notify_root(ROOT_TYPE_ROOT); 
			}
			else if(mode == MODE_INDEX_SILENT)
			{
				bool silent, root;
				GetSealMode(silent, root);
				SetSealMode(true, root);
				
				((gactive_object*)_parent)->object_state2 |= gactive_object::STATE_SEALED;
			
			}
		}
		counter ++;
	}
	
	inline void DecIdleSealMode(unsigned char mode)
	{
		ASSERT(mode < 4);
		int & counter= _idle_seal_mode_counter[mode];
		if(counter > 0)
		{
			if(counter == 1)
			{
			if(mode == MODE_INDEX_SLEEP)
			{
				bool sleep, stun;
				GetIdleMode(sleep, stun);
				SetIdleMode(false, stun);

				((gactive_object*)_parent)->object_state2 &= ~gactive_object::STATE_SLEEP;

				_runner->dispel_root(ROOT_TYPE_SLEEP);
			}
			else if(mode == MODE_INDEX_STUN)
			{
				bool sleep, stun;
				GetIdleMode(sleep, stun);
				SetIdleMode(sleep, false);

				((gactive_object*)_parent)->object_state2 &= ~gactive_object::STATE_STUN;

				_runner->dispel_root(ROOT_TYPE_STUN);
			}
			else if(mode == MODE_INDEX_ROOT)
			{
				bool silent, root;
				GetSealMode(silent, root);
				SetSealMode(silent, false);

				((gactive_object*)_parent)->object_state2 &= ~gactive_object::STATE_ROOT;

				_runner->dispel_root(ROOT_TYPE_ROOT); 
			}
			else if(mode == MODE_INDEX_SILENT)
			{
				bool silent, root;
				GetSealMode(silent, root);
				SetSealMode(false, root);

				((gactive_object*)_parent)->object_state2 &= ~gactive_object::STATE_SEALED;

			}
			}
			counter --; 
		}
		else
		{
			ASSERT(false);
			GLog::log(GLOG_ERR,"FATALERROR: DECIDLESEALMODE mode %d,counter:%d\n:",mode,counter);
		}
	}  

	void IncImmuneMask(int mask)
	{
		int i=0;
		while(mask)
		{
			if(mask & 1)
			{
				if(_immune_state_adj_counter[i] == 0)
					_immune_state_adj |= 1<<i;
				_immune_state_adj_counter[i] ++;
			}
			i++;
			mask >>= 1;
		}
	}

	void DecImmuneMask(int mask)
	{
		int i=0;
		while(mask)
		{
			if(mask & 1)
			{
				if(_immune_state_adj_counter[i] == 1)
					_immune_state_adj &= ~(1<<i);
				_immune_state_adj_counter[i] --;
			}
			i++;
			mask >>= 1;
		}
	}

	friend class object_interface;
	friend class ai_actobject;
public:
	inline void DecHP(int hp)
	{
		if(_parent->IsZombie()) return;
		DoDamage(hp);
		if(_basic.hp <=0)
		{
			_basic.hp = 0;
			Die(XID(-1,-1),false,0,0);
		}
	}
	//Received specific damage for some reason This function is mainly called by the filter
	template<typename ATTACK_INFO>
	void BeHurt(const XID & who,const ATTACK_INFO & info,int damage,bool invader, char attacker_mode)
	{	
		if(_parent->IsZombie()) return;
		if(damage <= 0) damage = 1;

		OnHurt(who,info,damage,invader);
		DoDamage(damage);

		if(_basic.hp <=0)
		{
			_basic.hp = 0;
			Die(info.attacker,invader,attacker_mode,0);
		}
		else if (invader)
		{
			SendTo<0>(GM_MSG_PLAYER_BECOME_INVADER,who,60);
		}
	}


	inline void GetIdleMode(bool & sleep, bool & stun)
	{
		sleep = _idle_mode_flag & IDLE_MODE_SLEEP;
		stun  = _idle_mode_flag & IDLE_MODE_STUN ;
	}

	inline void GetSealMode(bool & silent,bool & root)
	{
		silent = _seal_mode_flag & SEAL_MODE_SILENT;
		root  = _seal_mode_flag & SEAL_MODE_ROOT;
	}
	
	inline int GetSealMode()
	{ 
		return _seal_mode_flag;
	}

	inline bool IsRootMode()
	{
		return _seal_mode_flag & SEAL_MODE_ROOT;
	}

	int GetVigour();
	/*
	{
		int res = _vigour_base + _vigour_en;
		if( res > MaxVigour ) res = MaxVigour;
		return res;
	}
	*/

	inline void SetVigourBase(int vb)
	{
		_vigour_base = vb;
	}

protected:
//Judging by attacks and skills
	int HandleAttackMsg(world * pPlane,const MSG & msg, attack_msg * );
	int HandleEnchantMsg(world * pPlane,const MSG & msg, enchant_msg * );
	bool AttackJudgement(attack_msg * attack,damage_entry &dmg,bool is_short_range,float dist );
	inline int DoDamage(int damage)
	{
		if(damage > _basic.hp) damage = _basic.hp;
		if(damage <= 0) damage = 1;
		_basic.hp -= damage;
		SetRefreshState();
		return damage;
	}
	int InsertDamageEntry(int damage,int delay,const XID & target, bool orange_name, char attacker_mode);

protected:
//miscellaneous
	bool SafeDeleteCurSession();
	void MH_query_info00(const MSG & msg);
	void DoHeartbeat(unsigned int tick);
	inline void IncHP(int hp_gen)
	{
		int tmp = _basic.hp + hp_gen;
		if(tmp > _cur_prop.max_hp) tmp = _cur_prop.max_hp;
		if(tmp != _basic.hp)
		{
			_basic.hp = tmp;
			SetRefreshState();
		}
	}

	struct func
	{
		//This is to increase the incoming value every eight seconds
		static inline int  Update(int & base, int & counter,int offset,int max)
		{    
			counter += offset;
			int xo = counter & 0x07;
			base += counter >> 3;
			counter = xo;
			if(base > max) 
				base = max;
			else if(base < 0)
				base = 0;
			return base;
		}
	}; 

	inline int GenHP(int hp_gen)
	{
		int tmp = _basic.hp;
		if( tmp != func::Update(_basic.hp,_hp_gen_counter,hp_gen,_cur_prop.max_hp))
		{
			SetRefreshState();
		}
		return _basic.hp;
	}
	
	inline void GenHPandMP(int hp_gen,int mp_gen)
	{
		int tmp = _basic.hp;
		if( tmp != func::Update(_basic.hp,_hp_gen_counter,hp_gen,_cur_prop.max_hp))
		{
			SetRefreshState();
		}

		tmp = _basic.mp;
		if( tmp != func::Update(_basic.mp,_mp_gen_counter,mp_gen,_cur_prop.max_mp))
		{
			SetRefreshState();
		}
	}

	template <int foo>
	bool CheckServerNotify()
	{
		int tick = g_timer.get_tick();
		if(tick - _server_notify_timestamp > 0)
		{
			_server_notify_timestamp = tick + 25*20;
			return true;
		}
		return false;
	}

private:
//private virtual function group
	virtual void OnHeartbeat(unsigned int tick) = 0;
	virtual void OnDamage(const XID & attacker,int skill_id, const attacker_info_t&info,int damage,int at_state,char speed,bool orange,unsigned char section)=0;
	virtual void OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader)=0;
	virtual void OnDeath(const XID & lastattack,bool is_invader, char attacker_mode, int taskdead) {};
	virtual void OnAttacked(world *pPlane,const MSG & msg, attack_msg * attack,damage_entry & dmg,bool hit) = 0;
	virtual void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg,float & damage_adjust) = 0;
	virtual bool CheckInvader(world * pPlane, const XID & source){ return false;} //ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ç·ï¿½ï¿½ï¿½Ð§
	//virtual void OnPickupMoney(unsigned int money,int drop_id, bool ){}
	virtual void OnPickupItem(const A3DVECTOR &pos,const void * data, unsigned int size,bool isTeam, int drop_id) {}
	virtual void OnUseAttackRune() {}
	virtual void OnPickupGold(unsigned int money,int drop_id ){}
	virtual void OnPickupSilver(unsigned int money,int drop_id ){}

public:
//All interfaces of skills
	inline int  CheckSkillCondition(unsigned int skill,const XID * target, int size)
	{
		object_interface obj_if(this);
		return _skill.Condition(skill,obj_if,target,size);
	}
	
	inline int NPCStartSkill(unsigned int skill_id, int level, const XID & target,int & next_interval)
	{
		return _skill.NpcStart(skill_id,this,level,&target,1,next_interval);
	}
	
	inline void NPCEndSkill(unsigned int skill_id,int level,const XID & target)
	{
		_skill.NpcEnd(skill_id,this,level,&target,1);
	}

	inline bool NPCSkillOnAttacked(unsigned int skill_id,int level)
	{
		return _skill.NpcInterrupt(skill_id, object_interface(this),level);
	}

	inline float NPCSkillRange(unsigned int skill_id,int level)
	{
		return _skill.NpcCastRange(skill_id,level);
	}
	

	inline int StartSkill(SKILL::Data & data, const XID * target,int size,int & next_interval)
	{
		return _skill.StartSkill(data,object_interface(this),target,size,next_interval);
	}

	inline int RunSkill(SKILL::Data & data, const XID * target,int size,int & next_interval)
	{
		return _skill.Run(data, object_interface(this),target,size,next_interval);
	}

	inline int StartSkill(SKILL::Data & data, const A3DVECTOR &pos, const XID * target,int size,int & next_interval)
	{
		return _skill.StartSkill( data, object_interface(this), pos, target,size,next_interval);
	}

	inline int RunSkill(SKILL::Data & data, const A3DVECTOR & pos, const XID * target,int size,int & next_interval)
	{
		return _skill.Run( data, object_interface(this), pos, target,size, next_interval);
	}
	
	inline int ContinueSkill(SKILL::Data & data,const XID * target,int size,int & next_interval,int elapse_time) 
	{
		return _skill.Continue(data, object_interface(this),target,size,next_interval,elapse_time);
	}

	inline int CastInstantSkill(SKILL::Data & data, const XID * target,int size)
	{
		return _skill.InstantSkill(data,object_interface(this),target,size);
	}

	inline bool SkillOnAttacked(SKILL::Data & data)
	{
		return _skill.Interrupt(data, object_interface(this));
	}

	inline bool CancelSkill(SKILL::Data & data)
	{
		return _skill.Cancel(data, object_interface(this));
	}

	inline int GetSkillLevel(int skill_id)
	{
		return _skill.GetSkillLevel(skill_id);
	}

	inline void IncSkillAbility(int skill_id, int inc = 1)
	{
		_skill.IncAbility(object_interface(this),skill_id,inc);
	}

	inline int GetSkillAbility(int skill_id)
	{
		return _skill.GetAbility(skill_id);
	}

	inline float GetSkillAbilityRatio(int skill_id)
	{
		return _skill.GetAbilityRatio(skill_id);
	}
	
	inline void IncSkillAbilityRatio(int id, float ratio)
	{
		_skill.IncAbilityRatio(object_interface(this),id,ratio);	
	}

	inline int RemoveSkill(int skill_id)
	{
		return _skill.Remove(skill_id);
	}

	inline bool CastRune(int skill_id, int skill_level)
	{
		SKILL::Data sd((unsigned int)skill_id);
		return _skill.CastRune(sd, object_interface(this),skill_level) == 0;
	}
	
	inline int StartRuneSkill(SKILL::Data & data, const XID * target,int size,int & next_interval,int level)
	{
		return _skill.StartRuneSkill(data,object_interface(this),target,size,next_interval,level);
	}

	inline int RunRuneSkill(SKILL::Data & data, const XID * target,int size,int & next_interval, int level)
	{
		return _skill.RunRuneSkill(data, object_interface(this),target,size,next_interval,level);
	}
	
	inline int ContinueRuneSkill(SKILL::Data & data,const XID * target,int size,int & next_interval,int elapse_time,int level) 
	{
		return _skill.ContinueRuneSkill(data, object_interface(this),target,size,next_interval,elapse_time,level);
	}

	inline bool RuneSkillOnAttacked(SKILL::Data & data,int level)
	{
		return _skill.InterruptRuneSkill(data, object_interface(this),level);
	}

	inline bool CancelRuneSkill(SKILL::Data & data,int level)
	{
		return _skill.CancelRuneSkill(data, object_interface(this),level);
	}

	inline int CastRuneInstantSkill(SKILL::Data & data, const XID * target,int size,int level)
	{
		return _skill.RuneInstantSkill(data,object_interface(this),target,size,level);
	}

	inline int GetProduceSkillLevel(int id)
	{
		return _skill.GetProduceSkillLevel(id);	
	}

	inline void ActivateDynSkill(int id, int counter)
	{
		return _skill.ActivateDynSkill(id, counter);
	}

	inline void DeactivateDynSkill(int id, int counter)
	{
		return _skill.DeactivateDynSkill(id, counter);
	}
	
public:	
	//lgc
	virtual bool IsElfEquipped(){return false;}
	virtual void UpdateCurElfInfo(unsigned int id, short refine_level, short str, short agi, short vit, short eng, const char * skilldata, int cnt){}
	virtual void ClearCurElfInfo(){}
	virtual void ClearCurElfVigor(){}
	virtual void UpdateElfProp(){}
	virtual void UpdateElfVigor(){}
	virtual void UpdateMinElfStatusValue(int value){}
	virtual void TriggerElfRefineEffect(){}
	virtual bool IsElfRefineEffectActive(){return false;}
	virtual void UpdateStallInfo(int id, int max_sell, int max_buy, int max_name){}
	virtual void ClearStallInfo(){}
	virtual void OnStallCardTakeOut(){}
	
	//obj_interface interface	
	virtual bool OI_GetElfProp(short& level, short& str, short& agi, short& vit, short& eng){return false;}
	virtual int OI_GetElfVigor(){return -1;}
	virtual int OI_GetElfStamina(){return -1;}
	virtual bool OI_DrainElfVigor(int dec){return false;}
	virtual bool OI_DrainElfStamina(int dec){return false;}
	
	virtual void SetAllCoolDown(int id1, int id2, int msec, int except_id) {}
	virtual bool UseGlyphUnlockItem(unsigned int pos, int rank, int level) {}
	
};


/*	active object The packaging excuse for AI */

class  ai_actobject : public ai_object
{
	protected:
		gactive_imp * _imp;
	public:
		ai_actobject(gactive_imp * imp):_imp(imp)
		{}

		//destructor
		virtual ~ai_actobject()
		{
		}

		virtual gactive_imp * GetImpl()
		{
			return _imp;
		}

		//property
		virtual void GetID(XID & id)
		{
			id = _imp->_parent->ID;
		}

		virtual void GetPos(A3DVECTOR & pos)
		{
			pos = _imp->_parent->pos;
		}

		//virtual int GetState() = 0;

		virtual int GetHP()
		{
			return _imp->_basic.hp;
		}

		virtual int GetMaxHP()
		{
			return _imp->_cur_prop.max_hp;
		}

		virtual int GenHP(int hp)
		{
			return _imp->GenHP(hp);
		}

		virtual int GetMP()
		{
			return _imp->_basic.mp;
		}

		virtual float GetAttackRange()
		{
			return _imp->_cur_prop.attack_range;
		}

		virtual float GetMagicRange(unsigned int id,int level)
		{	
			return _imp->NPCSkillRange(id,level);
		}
		
		virtual float GetBodySize()
		{
			return _imp->_parent->body_size;
		}

		virtual int GetFaction()
		{
			return _imp->_faction;
		}

		virtual int GetEnemyFaction()
		{
			return _imp->_enemy_faction;
		}

		virtual int GetAntiInvisibleDegree()
		{
			return ((gactive_object*)_imp->_parent)->anti_invisible_degree;	
		}

		//operation
		virtual void AddSession(act_session * pSession)
		{
			_imp->AddSession(pSession);
			_imp->StartSession();
		}

		virtual bool HasSession()
		{
			return _imp->_cur_session || _imp->_session_list.size();
		}

		virtual void ClearSession()
		{
			_imp->ClearSession();
		}

		virtual void SendMessage(const XID & id, int msg);
		virtual void ActiveCombatState(bool state)
		{
			_imp->ActiveCombatState(state);
		}

		virtual bool GetCombatState()
		{
			return _imp->IsCombatState();
		}

		virtual int GetAttackSpeed()
		{
			return _imp->_cur_prop.attack_speed;
		}
	public:

		virtual int QueryTarget(const XID & id, target_info & info);
		virtual int GetSealMode()
		{
			return _imp->GetSealMode();
		}
};
#endif
