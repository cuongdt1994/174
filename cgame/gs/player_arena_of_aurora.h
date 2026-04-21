#ifndef __ONLINEGAME_GS_PLAYER_ARENAOFAURORA_H__
#define __ONLINEGAME_GS_PLAYER_ARENAOFAURORA_H__

#define MAX_AREA_DISTANCE 5.0f

class gplayer_arenaofaurora : public gplayer_imp
{
	bool is_arena_zombie;
	int wait_resurrect_timer;

	int attack_faction;
	int defense_faction;

public:
	gplayer_arenaofaurora()
	{
		is_arena_zombie = false;
		wait_resurrect_timer = 0;
		attack_faction = 0;
		defense_faction = 0;
	}
	
private:
	static bool __GetPetAttackHook(gactive_imp* __this, const MSG& msg, attack_msg& amsg);
	static bool __GetPetEnchantHook(gactive_imp* __this, const MSG& msg, enchant_msg& emsg);
	static void __GetPetAttackFill(gactive_imp* __this, attack_msg& amsg);
	static void __GetPetEnchantFill(gactive_imp* __this, enchant_msg& emsg);

	void SetBattleFaction();

	void SendTeamMessage(const MSG& msg, float range);
	void SendTeamAllMessage(const MSG& msg, float range);
public:
	DECLARE_SUBSTANCE(gplayer_arenaofaurora);
	virtual	void OnHeartbeat(size_t tick);
	virtual int MessageHandler(world* pPlane, const MSG& msg);
	virtual int ZombieMessageHandler(world* pPlane, const MSG& msg);
	virtual void FillAttackMsg(const XID& target, attack_msg& attack, int dec_arrow);
	virtual void FillEnchantMsg(const XID& target, enchant_msg& enchant);
	virtual	void SendDataToSubscibeList();
	virtual	void SendTeamDataToSubscibeList();
	virtual void PlayerEnterWorld();
	virtual void PlayerEnterServer(int source_tag); 
	virtual void PlayerLeaveServer(); 
	virtual void PlayerLeaveWorld();
	virtual int GetFaction();
	virtual int GetEnemyFaction();
	virtual attack_judge GetPetAttackHook();
	virtual enchant_judge GetPetEnchantHook();
	virtual attack_fill GetPetAttackFill();
	virtual enchant_fill GetPetEnchantFill();
	virtual void OnDamage(const XID& attacker, int skill_id, const attacker_info_t& info, int damage, int at_state, char speed, bool orange, unsigned char section);
	virtual void OnHurt(const XID& attacker, const attacker_info_t& info, int damage, bool invader);
	virtual void OnDeath(const XID & lastattack,bool is_pariah, char attacker_mode, int taskdead);
	virtual bool CanResurrect(int param);
	virtual int  Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int target_tag,float hp_factor, float mp_factor, int param/*, float ap_factor, int extra_invincible_time*/);
	virtual void SendMsgToTeam(const MSG& msg, float range, bool exclude_self);
	virtual bool OI_IsInTeam();
	virtual int DispatchCommand(int cmd_type, const void* buf, size_t size);
	virtual void SendTeamChat(char channel, const void* buf, size_t len, const void* data, size_t dsize, int use_id = 0);

	virtual bool IsArenaMember(const XID& member);
};

#endif
