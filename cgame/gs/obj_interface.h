#ifndef __ONLINEGAME_GS_OBJECT_INTERFACE_H__
#define __ONLINEGAME_GS_OBJECT_INTERFACE_H__
#include <cstring>
#include "property.h"
#include "attack.h"
#include <iconv.h>

class gactive_imp;
class filter;
class world;
struct pet_data;
namespace GNET { class SkillWrapper; }
namespace GDB { struct itemdata; struct shoplog; struct pet; struct petcorral; }
class object_interface
{
	gactive_imp * _imp;
public: 
	object_interface():_imp(0)
	{}

	object_interface(gactive_imp * imp):_imp(imp)
	{}

	void Attach(gactive_imp * imp)
	{
		_imp = imp;
	}
	gactive_imp * GetImpl() { return _imp;}
public:

	void BeHurt(const XID & who,const attacker_info_t & info, int damage,char attacker_mode = 0);
	void BeHurt(const XID & who,const attacker_info_t & info, int damage,bool invader, char attacker_mode = 0);
	void SendHurtMsgToSelf(const XID & attacker, int damage, bool invader, char attack_mode);
	void Heal(const XID & healer,unsigned int life);
	void Heal(unsigned int life);
	void HealBySkill(const XID & healer,unsigned int life);
	void HealBySkill(unsigned int life);
	void HealByPotion(unsigned int life);
	void InjectMana(int mana);
	bool DrainMana(int mana);
	void DecHP(int hp);
	void DecSkillPoint(int sp);
	bool SessionOnAttacked(int session_id);
	bool ActionOnAttacked(int action_id);

	void SetATDefenseState(char state);

	//�������?
	int SpendFlyTime(int tick);
	int GetFlyTime();	//ȡ�ÿ���ʹ�õ�ʣ�����ʱ��?
	void TakeOff();	//���?
	void Landing(); //����
	bool IsPlayerClass();

	//�������λ��?
	bool IsOnGround();
	bool IsInAir();
	bool IsInWater();
	bool IsMountMode();
	bool IsInSanctuary(const A3DVECTOR & pos);
	bool IsCarrierMode();
	void SendClientCooldownCarrier(int skill_id, int cooldown);
	void SendClientPlayerWorldSpeakInfo( char enabled, char enabled2, int skills_count, int * skills );

	void Say(const char * msg);

	//�������?
	void SetNoFly(bool b);
	void SetNoChangeSelect(bool b);
	void SetNoMount(bool b);
	void SetNoBind(bool b);
	void SetNoAmulet(bool b);
	void SetNoLongJump(bool b);
	void SetNoSpeedUp(bool b);
	void SetNoInvisible(bool b);

    bool GetNoInvisible();

	void DenyAttackCmd();
	void AllowAttackCmd();
	void DenyElfSkillCmd();
	void AllowElfSkillCmd();
	void DenyUseItemCmd();
	void AllowUseItemCmd();
	void DenyNormalAttackCmd();
	void AllowNormalAttackCmd();
	void DenyPetCmd();
	void AllowPetCmd();
public:
	//filter ���?
	void AddFilter(filter*);
	void RemoveFilter(int filter_id);
	void ClearSpecFilter(int mask);
	void ClearRandomSpecFilter(int mask);
	bool IsFilterExist(int id);
	int  QueryFilter(int filterid,int index);
public:
	class MsgAdjust
	{
	public:
		virtual void AdjustAttack(attack_msg & attack) {}
	};

	int GenerateDamage();
	int GenerateMagicDamage();

	int GenerateDamage(int scale_enhance, int point_enhance);
	int GenerateMagicDamage(int scale_enhance, int point_enhance);

	// 100% ataque crítico
	int GenerateCritDamage(int scale_enhance, int point_enhance);
	int GenerateCritMagicDamage(int scale_enhance, int point_enhance);

	int GenerateDamageWithoutRune();
	int GenerateMagicDamageWithoutRune();

	int GenerateDamageWithoutRune(int scale_enhance, int point_enhance);
	int GenerateMagicDamageWithoutRune(int scale_enhance, int point_enhance);

	int GenerateEquipDamage();
	int GenerateEquipMagicDamage();
	//����
	bool UseProjectile(unsigned int count);
	bool CanAttack(const XID & target);	//�Ƿ���Խ�����������?
	void SetRetortState();			//�����´ι���Ϊ���𹥻���һ����Ч
	bool GetRetortState();                  //�����´ι����Ƿ�Ϊ�����Ա�������TranslateSendAttack
	void SetAuraAttackState();		//�����´ι���Ϊ�⻷������һ����Ч
	bool GetAuraAttackState();		//�����´ι���Ϊ�����Ա�������TranslateSendAttack
	void SetReboundState();			//�����´ι���Ϊ����������һ����Ч
	void SetBeatBackState();		//�����´ι���Ϊ����������һ����Ч
	void Attack(const XID & target, attack_msg & attack,int use_projectile);
	void RegionAttack1(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile,const XID& target);		//��
	void RegionAttack2(const A3DVECTOR& pos, float radius,attack_msg & attack,int use_projectile);		//��
	void RegionAttack3(const A3DVECTOR& pos, float cos_half_angle,attack_msg & attack,int use_projectile);	//׵
	void MultiAttack(const XID * target,unsigned int size, attack_msg & attack,int use_projectile);
	void MultiAttack(const XID * target,unsigned int size, attack_msg & attack,MsgAdjust & adj,int use_projectile);
	void DelayAttack(const XID & target, attack_msg & attack,int use_projectile, unsigned int delay_tick);//���ڻ���Ч����ʹ����
	void SendAttack(const XID & target, attack_msg & attack);
	//����
	void Enchant(const XID & target, enchant_msg & msg);
	void EnchantZombie(const XID & target, enchant_msg & msg);

	void TransferPetEgg(const XID & target, int pet_egg_id);

	void RegionEnchant1(const A3DVECTOR& pos, float radius,enchant_msg & msg, const XID& target);		//��
	void RegionEnchant2(const A3DVECTOR& pos, float radius,enchant_msg & msg );		//��
	void RegionEnchant3(const A3DVECTOR& pos, float cos_half_angle,enchant_msg & msg );	//׵
	void MultiEnchant(const XID * target,unsigned int size, enchant_msg & msg);
	void TeamEnchant(enchant_msg & msg,bool exclude_self);
	void DelayEnchant(const XID & target, enchant_msg & msg, unsigned int delay_tick);//���ڻ���Ч����ʹ����

	void SetAttackMonster();
	void EnterCombatState();
	bool IsCombatState();

	void KnockBack(const XID & attacker, const A3DVECTOR &source,float distance,int time=0/*����*/,int stun_time=0/*��*/);
	void PullOver(const XID & attacker, const A3DVECTOR &source,float distance, int time);
	void KnockUp(float distance,int time=0/*����*/);

	//����
	bool Resurrect(float exp_reduce, float hp_factor, float mp_factor); //0.0 ~ 1.0
	void SendHealMsg(const XID & target, int hp);		//��ĳ��������?
	void Reclaim();					//��������ʧ

	//��ѯ 
	bool IsPet();
	bool IsDead();
	bool IsMember(const XID & member);
	bool IsInTeam();
	bool IsTeamLeader();
	bool IsPalsyImmune();
	int  GetClass();
	int  GetFaction();
	int  GetEnemyFaction();
	bool IsMafiaMember();
	int  GetMafiaID();
	char  GetMafiaRank();
	int GetSpouseID();
	int GetWorldTag();
	const A3DVECTOR & GetPos();
	A3DVECTOR GetPos2();
	const XID & GetSelfID();
	const XID & GetCurTargetID();
	const XID & GetLeaderID();
	float GetBodySize();
	int QueryObject(const XID & who, A3DVECTOR & pos, float & body_size); 	//0 ������ 1: ���� 2: ����
	int QueryObject(const XID & who, A3DVECTOR & pos, int & hp, int & mp); 	//0 ������ 1: ���� 2: ����
	int QueryObject(const XID & who, int & invisible_degree, int & anti_invisible_degree); 	//0 ������ 1: ���� 2: ����
	bool CheckGMPrivilege();
	bool IsPVPEnable();	//�Ƿ�����PK����
	char GetForceAttack();
	int GetInvaderState();	//ȡ�ú�۰�����״�?
	int GetPetEggID();
	int GetPetType();
	XID GetPetID();
	int GetSpherePlayerListSize(const A3DVECTOR& pos,float radius);
	float GetHpScale();
	int  GetTargetClass(const XID & target);
	int  GetTargetHp(const XID & target);	
	int  GetTargetMp(const XID & target);
	int  GetTargetMaxhp(const XID & target);
	int  GetTargetLevel(const XID & target);
	const A3DVECTOR & GetTargetPos(const XID & target);
	bool GetTargetIsFlyMode(const XID & target);
	bool GetTargetIsMountMode(const XID & target);
	bool GetTargetIsInvisible(const XID & target);
	bool GetTargetIsSealed(const XID & target);
	bool GetTargetIsStun(const XID & target);
	void SetTargetFreeMove(bool b);
	bool GetTargetIsSleep(const XID & target);
	bool GetTargetIsRoot(const XID & target);

	void SetDebuffSeeker1(bool b);
	void SetDebuffSeeker2(bool b);
	void SetDebuffSeeker3(bool b);
	void SetDebuffSeeker4(bool b);

	bool GetDebuffSeeker1(const XID & target);
	bool GetDebuffSeeker2(const XID & target);
	bool GetDebuffSeeker3(const XID & target);
	bool GetDebuffSeeker4(const XID & target);


	//���ͻ��˲���
	void SendClientMsgSkillCasting(const XID &target, int skill, unsigned short time,unsigned char level);
	void SendClientMsgRuneSkillCasting(const XID &target, int skill, unsigned short time,unsigned char level);
	void SendClientMsgSkillInterrupted(char reason);
	void SendClientMsgSkillPerform();
	void SendClientNotifyRoot(unsigned char type);	//�㲥��Ϊ
	void SendClientDispelRoot(unsigned char type);	//ֻ���͸�����
	void SendClientCurSpeed();		//�����ٶȸ��ͻ���
	void SendClientEnchantResult(const XID & caster, int skill, char level, bool invader,int at_state,unsigned char section);
	void SendClientInstantSkill(const XID & target, int skill, unsigned char level);
	void SendClientRuneInstantSkill(const XID & target, int skill, unsigned char level);
	void SendClientElfSkill(const XID & target, int skill, unsigned char level);	//lgc
	void SendClientSkillAbility(int id, int ability);
	void SendClientCastPosSkill(const A3DVECTOR & pos, const XID &target, int skill,unsigned short time, unsigned char level);
	void SendClientRushMode(unsigned char is_active);
	void SendClientAttackData();
	void SendClientDefenseData();
	void UpdateAll();
	void SendClientMsgFlySwordTime(int time);
	void SendClientDuelStart(const XID & target);
	void SendClientDuelStop(const XID & target);
	void SendClientScreenEffect(int effectid, int state);
	void SendClientGfxEffect(int effectid, int state);
	void SendClientComboSkillPerpare(int skillid,int timestamp,int arg1, int arg2, int arg3);
	void SendClientPrayDistanceChange(float pd);

	// Nível de Genesis
	void SendClientGenesisLevel(bool b = false);

public:
	//����״̬����
	void IncVisibleState(unsigned short state);
	void DecVisibleState(unsigned short state);
	void ClearVisibleState(unsigned short state);
	bool IsExistTeamVisibleState(unsigned short state);

	void InsertTeamVisibleState(unsigned short state, int t);
	void InsertTeamVisibleState(unsigned short state, int param, int t);
	void InsertTeamVisibleState(unsigned short state, int param, int param2, int t);
	void ModifyTeamVisibleState(unsigned short state, int param, int t);
	void ModifyTeamVisibleState(unsigned short state, int param, int param2, int t);
	void RemoveTeamVisibleState(unsigned short state);

	//���ñ�����־	
	void ChangeShape(int shape);
	int GetForm();
	void SetFormCls(int form);

	void LockEquipment(bool is_lock);
	void BindToGound(bool is_bind);

	//����״̬����
	//void SetIdleMode(bool sleep,bool stun);	//�ö��������λ���˯��״̬��������
	//void SetSealMode(bool silent,bool root);//�ö����ڶ��������޷�����״̬ 

	//void GetIdleMode(bool & sleep, bool & stun);
	//void GetSealMode(bool & silent, bool & root);

	void IncIdleSealMode(unsigned char mode);//lgc
	void DecIdleSealMode(unsigned char mode);

	bool IsAggressive();
	void SetAggressive(bool isActive = true);

	void DuelStart(const XID & target);
	void DuelStop();
	
	//���Լ��ĳ���б�������һ�����
	void AddAggro(const XID & attacker , int rage);
	void AddAggroToEnemy(const XID & helper, int rage);

    void RemoveAggro(const XID& src, const XID& dest, float ratio);

	void BeTaunted(const XID & who,int aggro);

	void AddPlayerEffect(short effect);
	void RemovePlayerEffect(short effect);

	void AddMultiObjEffect(const XID& target, char type);
	void RemoveMultiObjEffect(const XID& target, char type);

	//ȡ�������Ĳ�������
	int GetImmuneMask();
	//void SetImmuneMask(int mask);		//Ϊ1��λ��Ҫ�������ߵ�����,������
	//void ClearImmuneMask(int mask);		//Ϊ1��λ��Ҫ�������������?
	void IncImmuneMask(int mask);
	void DecImmuneMask(int mask);

	//ŭ�����?
	void ModifyAP(int ap);

    void ModifyScaleHP(int hp);

	void ActiveMountState(int mount_id, unsigned short mount_color);
	void DeactiveMountState();

	void ResurrectPet();

	void Disappear();
	void Die();
public:
	//��ǿ����
	void EnhanceHPGen(int hpgen);		//������ø��º���?
	void ImpairHPGen(int hpgen);		//������ø��º���?

	void EnhanceScaleHPGen(int hpgen);
	void ImpairScaleHPGen(int hpgen);

	void EnhanceScaleMPGen(int mpgen);
	void ImpairScaleMPGen(int mpgen);

	void EnhanceMPGen(int mpgen);		//������ø��º���?
	void ImpairMPGen(int mpgen);		//������ø��º���?

	void EnhanceMaxHP(int hp);		//������ø��º���?
	void ImpairMaxHP(int hp);		//������ø��º���?

	void EnhanceMaxMP(int mp);		//������ø��º���?
	void ImpairMaxMP(int mp);		//������ø��º���?

	void EnhanceScaleMaxHP(int hp,bool update=true);		//������ø��º���?
	void ImpairScaleMaxHP(int hp,bool update=true);		//������ø��º���?

	void EnhanceScaleMaxMP(int mp);		//������ø��º���?
	void ImpairScaleMaxMP(int mp);		//������ø��º���?

    void EnhanceScaleExp(float exp_sp_factor, float realm_exp_factor);
    void ImpairScaleExp(float exp_sp_factor, float realm_exp_factor);

	void EnhanceDefense(int def);
	void ImpairDefense(int def);
	void EnhanceScaleDefense(int def);
	void ImpairScaleDefense(int def);

	void EnhanceArmor(int ac);
	void ImpairArmor(int ac);
	void EnhanceScaleArmor(int ac);
	void ImpairScaleArmor(int ac);

	void EnhanceResistance(unsigned int cls, int res);	 //cls = [0,4]
	void ImpairResistance(unsigned int cls, int res);

	void EnhanceScaleResistance(unsigned int cls, int res);	 //cls = [0,4]
	void ImpairScaleResistance(unsigned int cls, int res);

	void EnhanceAllScaleResistance(int res);	 //cls = [0,4]
	void ImpairAllScaleResistance(int res);

    void IncAntiDefenseDegree(int val);
    void DecAntiDefenseDegree(int val);

	// Nível de Pico
	void IncHighestLevel(int val);
    void DecHighestLevel(int val);

    void IncAntiResistanceDegree(int val);
    void DecAntiResistanceDegree(int val);

	void EnhanceDamage(int dmg);
	void ImpairDamage(int dmg);

	void EnhancePercDamage(float dmg_low, float dmg_high);
	void ImpairPercDamage(float dmg_low, float dmg_high);

	void EnhanceMagicDamage(int dmg);
	void ImpairMagicDamage(int dmg);

	void EnhanceScaleDamage(int dmg);
	void ImpairScaleDamage(int dmg);

	void EnhanceScaleMagicDamage(int dmg);
	void ImpairScaleMagicDamage(int dmg);

	void EnhanceSpeed0(float speedup);
	void ImpairSpeed0(float speedup);

	void EnhanceOverrideSpeed(float speedup);
	void ImpairOverrideSpeed(float speedup);

	void EnhanceSpeed(int speedup);
	void ImpairSpeed(int speedup);

	void EnhanceSwimSpeed(int speedup);
	void ImpairSwimSpeed(int speedup);

	void EnhanceFlySpeed(float speedup);
	void ImpairFlySpeed(float speedup);
	
	void EnhanceScaleFlySpeed(int speedup);
	void ImpairScaleFlySpeed(int speedup);

	void EnhanceMountSpeed(int speedup);
	void ImpairMountSpeed(int speedup);

	void EnhanceAttackSpeed(int speedup);
	void ImpairAttackSpeed(int speedup);
	
	void EnhanceScaleAttackSpeed(int speedup);
	void ImpairScaleAttackSpeed(int speedup);

	void SetWeaponClass(int weapon_class);

	void SetRealWeaponClass(int weapon_class);
	int GetRealWeaponClass();

	void SetFakeWeaponClass(int  fake_weapon_class);
	int GetFakeWeaponClass();
	void SetWeaponRangeInc(int new_range);
	void SetWeaponRangeDec(int new_range);

	void EnhanceScaleAttack(int attack);
	void ImpairScaleAttack(int attack);

	void EnhanceBreathCapacity(int value);
	void ImpairBreathCapacity(int value);

	void EnhanceAttackRange(float range);
	void ImpairAttackRange(float range);

	void EnhanceAttackAcc(int inc);
	void ImpairAttackAcc(int inc);

	void EnhanceAttackDegree(int value);
	void ImpairAttackDegree(int value);

	void EnhanceDefendDegree(int value);
	void ImpairDefendDegree(int value);

	void EnableEndlessBreath(bool bVal);
	void InjectBreath(int value);
	void AdjustBreathDecPoint(int offset);

	void EnhanceCrit(int val);
	void ImpairCrit(int val);
	//���Ӽ��ٱ����˺��ٷֱ�
	void EnhanceCritDamage(int percent);
	void ImpairCritDamage(int percent);

	void EnhanceCritDamageReduce(int percent);
	void ImpairCritDamageReduce(int percent);

	void EnhanceCritResistance(int val);
	void ImpairCritResistance(int val);
	
	//���ڱ��������������������𣬴̿�ר��
	void IncAntiInvisiblePassive(int val);
	void DecAntiInvisiblePassive(int val);
	//���ڼ�����Ʒ��������������
	void IncAntiInvisibleActive(int val);
	void DecAntiInvisibleActive(int val);
	//���ڱ������������������𣬴̿�ר��
	void IncInvisiblePassive(int val);
	void DecInvisiblePassive(int val);
	//���Ӽ����˺����ܸ���
	void EnhanceDamageDodge(int val);
	void ImpairDamageDodge(int val);
	//���Ӽ���״̬���ܸ���
	void EnhanceDebuffDodge(int val);
	void ImpairDebuffDodge(int val);

	void SetNewShieldRetort(char b);
	void SetNewShieldRetortPaladin(char b);

	bool GetCriticalDamageResult();

	//���Ӽ�����Ѫ�ٷֱ�
	void EnhanceHpSteal(int val);
	void ImpairHpSteal(int val);
	//���Ӽ��ٺ����ȴʱ��?
	void IncHealCoolTime(int ms);
	void DecHealCoolTime(int ms);
	//���Ӽ������˺�ʱ�����ĳ��?
	void IncAggroOnDamage(const XID & attacker, int val);
	void DecAggroOnDamage(const XID & attacker, int val);
	
	void EnhanceSkillDamage(int value);
	void ImpairSkillDamage(int value);
	void EnhanceSkillDamage2(int value);
	void ImpairSkillDamage2(int value);

	void IncApPerHit(int value);
	void DecApPerHit(int value);

	void ReduceResurrectExpLost(int value);
	void IncreaseResurrectExpLost(int value);

	void IncPenetration(int val);
	void DecPenetration(int val);
	void IncResilience(int val);
	void DecResilience(int val);

	void IncVigour(int val);
	void DecVigour(int val);

	void EnhanceNearNormalDmgReduce(float scale);
	void ImpairNearNormalDmgReduce(float scale);
	void EnhanceNearSkillDmgReduce(float scale);
	void ImpairNearSkillDmgReduce(float scale);
	void EnhanceFarNormalDmgReduce(float scale);
	void ImpairFarNormalDmgReduce(float scale);
	void EnhanceFarSkillDmgReduce(float scale);
	void ImpairFarSkillDmgReduce(float scale);

	//���˸���״̬�����Բ���
	void ImpairPlusDamage(int dmg);
	void EnhancePlusDamage(int dmg);
	void ImpairPlusMagicDamage(int dmg);
	void EnhancePlusMagicDamage(int dmg);
	void ImpairSoulPower(int val);
	void EnhanceSoulPower(int val);
	void ImpairPlusDefense(int defence);
	void EnhancePlusDefense(int defence);
	void ImpairPlusResistance(unsigned int cls, int res);
	void EnhancePlusResistance(unsigned int cls, int res);
	void ImpairPlusMaxHP(int hp, bool update = true);
	void EnhancePlusMaxHP(int hp);

	void ImpairHighestLevel(int value);
	void EnhanceHighestLevel(int value);
	int GetHighestLevel();





	//���Ե����¼���
	void UpdateDefenseData();
	void UpdateAttackData();
	void UpdateMagicData();	//����ħ������
	void UpdateSpeedData();
	void UpdateHPMPGen();

	void SetInvincibleFilter(bool is_vin, int timeout,bool immune);
	void SetInvincibleFilter2(bool is_vin, int timeout,bool immune);
	

//��ȴ
	bool TestCoolDown(unsigned short id);
	void SetCoolDown(unsigned short id, int ms);
	void CoolDownReduceAtr(unsigned short id, int ms);
	void ResetCoolDown(unsigned short id, int ms = 1);
	void ResetAllCoolDown(int ms, int except_id);
	void SendReduceCDMsg(const XID& target,int id, int ms);
	void SetAutoGenAmulet(bool b);

	bool TestCommonCoolDown(unsigned short i);		//i:0-4 ������ȴ0-4, 5-9��Ʒ��ȴ0-4
	void SetCommonCoolDown(unsigned short i, int ms);
	
	void ReturnToTown();
	bool CanReturnToTown();

	void ReturnWaypoint(int point);
	bool CheckWaypoint(int point_index, int & point_domain);

	bool SkillMove(const A3DVECTOR & pos, bool notarget);
	void BreakCurAction();

	int CalcPhysicDamage(int raw_damage, int attacker_level);
	int CalcMagicDamage(int dmg_class, int raw_damage, int attacker_level);

	float CalcLevelDamagePunish(int atk_level , int def_level);
	int CalcPenetrationEnhanceDamage(int penetration, int raw_damage);
	int CalcVigourEnhanceDamage(int atk_vigour,int def_vigour,bool pvp, int raw_damage);

	bool IsInvisible();
	void SetInvisible(int invisible_degree);	//ֻ��filter_invisible�б�����
	void ClearInvisible();						//ֻ��filter_invisible�б�����
	void SetInvisibleFilter(bool is_invisible, int time, int mana_per_sec,int invisible_degree,int speeddown);	//�л�����/������״̬,playerʹ��
	bool IsGMInvisible();
	void SetGMInvisible();		//ֻ��gm_invisible_filter�б�����
	void ClearGMInvisible();	//ֻ��gm_invisible_filter�б�����
	void SetGMInvisibleFilter(bool is_invisible, int time, int mask);	//�л�gm����/������״̬,playerʹ��

	bool ActivateSharpener(int id, int equip_index);
	void TransferSpecFilters(int filter_mask, const XID & target, int count);	
	void AbsorbSpecFilters(int filter_mask, const XID & target, int count);	
	bool SummonPet2(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);
	bool PetSacrifice();
	bool IsPetActive();
	bool SummonPlantPet(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);
	bool PlantSuicide(float distance, const XID & target, char force_attack);
	void InjectPetHPMP(int hp, int mp);
	void DrainPetHPMP(const XID & pet_id, int hp, int mp);
	void DrainLeaderHPMP(const XID & attacker, int hp, int mp);
	void LongJump(A3DVECTOR & pos, int tag);
	void LongJump2(A3DVECTOR pos, int tag);
	void LongJumpToSpouse();
	void ShortJump(float distance,bool is_circle);
	void ShortJumpToTarget(A3DVECTOR & pos);
	void WeaponDisabled(bool disable);
	void ElmoDisabled(bool disable);
	void DetectInvisible(float range);
	void ForceSelectTarget(const XID & target);
	void ExchangePosition(const XID & target);
	void SetSkillAttackTransmit(const XID & target);
	void ForbidBeSelected(bool b);
	void CallUpTeamMember(const XID& member);
	void QueryOtherInventory(const XID& target); 
	void TurretOutOfControl();
	void EnterNonpenaltyPVPState(bool b);
	void SetNonpenaltyPVPFilter(bool b, int time);
	void GiveMafiaPvPAward(const XID & target, int type); 
	void RequestPunish(const XID & target, int skillid, int skilllevel);
	int	 ChangeVisibleTypeId(int tid);
	bool ModifyFilter(int filterid, int ctrlname, void * ctrlval, unsigned int ctrllen);
	void SetInfectSkill(int skill,int level);
	void ClrInfectSkill(int skill);

	void SetSoloChallengeFilterData(int filter_id, int num);
	void ShortJump(A3DVECTOR & newpos);
	void ShortJump(A3DVECTOR newpos, int tag);

public:
	//lgc С�������?
	bool GetElfProp(short& level, short& str, short& agi, short& vit, short& eng);
	bool EnhanceElfProp(short str, short agi, short vit, short eng);
	bool ImpairElfProp(short str, short agi, short vit, short eng);
	int GetElfVigor();
	int GetElfStamina();
	bool DrainElfVigor(int dec);
	bool DrainElfStamina(int dec);

public:
	//��ǿ�ٻ���
	void IncPetHp(int inc);
	void IncPetMp(int inc);
	void IncPetDamage(int inc);
	void IncPetMagicDamage(int inc);
	void IncPetDefense(int inc);
	void IncPetMagicDefense(int inc);
	void IncPetAttackDegree(int inc);
	void IncPetDefendDegree(int inc);

public:
	//��Ʒ
	int TakeOutItem(int item_id);			//ȥ��һ����Ʒ
	bool CheckItem(int item_id,unsigned int count);	//����Ƿ����ĳ����Ʒ
	bool CheckItem(int inv_index, int item_id, unsigned int count); //����ƶ�λ���Ƿ����ĳ����Ʒ
	void DropSpecItem(bool isProtected, const XID & owner);
	bool IsInventoryFull();
	//unsigned int GetMoney();
	//void DecMoney(unsigned int money);
	//void DropMoney(unsigned int money, bool isProtected);
	unsigned int GetInventorySize();
	unsigned int GetEmptySlotSize();
	unsigned int QueryItemPrice(int inv_index,int item_id);
	unsigned int QueryItemPrice(int item_id);
	unsigned int GetMallOrdersCount();
	int GetMallOrders(GDB::shoplog * list, unsigned int size);

	int GetSystime();
	int GetLinkIndex();
	int GetLinkSID();
	int GetInventoryDetail(GDB::itemdata * list, unsigned int size);
	int GetTrashBoxDetail(GDB::itemdata * list, unsigned int size);
	int GetTrashBox2Detail(GDB::itemdata * list, unsigned int size);
	int GetTrashBox3Detail(GDB::itemdata * list, unsigned int size);
	int GetTrashBox4Detail(GDB::itemdata * list, unsigned int size);
	int GetTrashBox5Detail(GDB::itemdata * list, unsigned int size);
	int GetTrashBox6Detail(GDB::itemdata * list, unsigned int size);
	int GetEquipmentDetail(GDB::itemdata * list, unsigned int size);
	bool GetMallInfo(int & cash, int & cash_used, int &cash_delta,  int &order_id);
	bool IsCashModified();
	unsigned int GetEquipmentSize();
	unsigned int GetTrashBoxCapacity();
	unsigned int GetTrashBox2Capacity();
	unsigned int GetTrashBox3Capacity();
	unsigned int GetTrashBox4Capacity();
	unsigned int GetTrashBox5Capacity();
	unsigned int GetTrashBox6Capacity();
	unsigned int GetTrashBoxMoney();
	bool IsTrashBoxModified();
	bool IsEquipmentModified();
	int TradeLockPlayer(int get_mask,int put_mask);
	int TradeUnLockPlayer();
	int GetDBTimeStamp();
	int InceaseDBTimeStamp();
	unsigned int GetSkillDataSize();					//��ȡ��������ռ�õ��ֽ���
	bool GetSkillData(void * buf, unsigned int size);	//���������ݿ�����buf��ָ����ڴ�?
	unsigned int GetPetsCount();					//��ȡ��������
	bool GetPetsData(GDB::petcorral pets);	//���������ݿ�����pets.listָ����ڴ�?

	int GetCityOwner(int city_id);
	bool TestSafeLock();

	float ModifyShieldEnergy(float shieldenergy);
	void SetMaxShieldEnergy(int max_shieldenergy);

	int FirstRunRelease(int cls, int incast);
	int BugSkillRelease(int id, int cls);
	
public:
	//��һ��С��
	struct minor_param
	{
		int mob_id;		//ģ��ID�Ƕ���
		int vis_id;		//�ɼ�id������?0��ֵ��Ч
		int remain_time;	//0 ��ʾ���� �����ʾ����������?
		int policy_classid;	//����������д�� ����ҪĬ����д0
		int policy_aggro;	//����������д�� ����ҪĬ����д0
		float exp_factor;	//����ֵ����
		float sp_factor;	//sp ����
		float drop_rate;	//����������
		float money_scale;	//������?����
		XID spec_leader_id;	//ָ����leader��˭ 
		bool parent_is_leader;	//�����߾���leader ��ʱ spec_leader_id ��Ч
		bool use_parent_faction;//ʹ�õ����ߵ���Ӫ��Ϣ������ʹ��Ĭ������
		//bool die_with_leader;	//leader ��������ʧ���Լ�Ҳ��ʧ
		char die_with_who;//mask, 0x01����leader������0x02����target����
		XID	owner_id;	//�����ߣ���Ϊ0�Ļ�ֻ�и��������ܶԹ�����й���?,���õ�ʱ���������ٻ���
		XID target_id;
		int path_id;
		unsigned char mob_name_size;	//��0��������
		char mob_name[18];
	};

	struct mine_param
	{
		int mine_id;		//ģ��ID�Ƕ���
		int remain_time;	//0 ��ʾ���� �����ʾ����������?
		bool bind_target;
	};

	struct npc_param
	{
		int npc_id;			
		int remain_time;
		int path_id;
	};

	//����������������npc��id(ע���Ǹ�ֵ)����ʧ�ܷ���-1
	int CreateMinors(const A3DVECTOR & pos ,const minor_param & p);//��ָ��λ�ô���С��
	int CreateMinors(const minor_param & p,float radius = 6.0f);	//�ڸ��������λ�ô���С��?
	int CreateMinors(const minor_param & p, const XID & target, float radius = 6.0f);	//��Ŀ�긽�������λ�ô���С��?
	//���º����ɹ�����0��ʧ�ܷ���-1
	int CreateMine(const A3DVECTOR & pos , const mine_param & p, const XID & target = XID(-1,-1));	//���ƶ�λ�ô�������
	int CreateMine(const mine_param & p, const XID & target, float radius = 6.0f);	//��Ŀ�긽�����λ�ô�������?
	int CreateMine(const A3DVECTOR & pos , const mine_param & p, const int dir, const XID & target = XID(-1,-1));	//���ƶ�λ�ô�������
	int CreateNPC(const A3DVECTOR & pos , const npc_param & p);	//���ƶ�λ���ٻ�npc
	int CreateNPC(const npc_param & p, const XID & target, float radius = 6.0f);	//��Ŀ�긽�����λ���ٻ�npc
	
	struct  pet_param
	{
		int mob_id;		//����һ��ģ���ﴴ������
		int vis_id;		//�ɼ�id
		float body_size;	//����
		int exp;
		int sp;
		char use_pet_name;
		unsigned char pet_name_size;
		char pet_name[18];
	};
	
//	bool CreatePet(const A3DVECTOR & pos, const pet_param & p, XID & id);
	bool CreatePet(const A3DVECTOR & pos, char inhabit_mode, const pet_data * pData , unsigned char pet_stamp, int honor_level, XID & id, char aggro_state, char stay_state);
	bool CreatePet2(const A3DVECTOR & pos, char inhabit_mode, const pet_data * pData , unsigned char pet_stamp, int honor_level, XID & id, char aggro_state, char stay_state, int skill_level, extend_prop * pProp=NULL);
	bool CreatePet3(const A3DVECTOR & pos, char inhabit_mode, const pet_data * pData , unsigned char pet_stamp, int honor_level, XID & id, char aggro_state, char stay_state, extend_prop * pProp = NULL);

	static void CreateMob(world * pPlane,const A3DVECTOR & pos ,const minor_param & p);//����һ������ ��С��
	bool IsEquipWing();

public:
	//ȡ�û�������
	const basic_prop & 		GetBasicProp();
	const extend_prop & 		GetExtendProp();
	const enhanced_param & 		GetEnhancedParam();
	const scale_enhanced_param & 	GetScaleEnhanecdParam();
	const item_prop &		GetCurWeapon();
	GNET::SkillWrapper &		GetSkillWrapper();
	int GetDefendDegree();
	int GetAttackDegree();
	int GetCrit();
	int GetCritDamage();
	int GetSoulPower();
	int GetInvisibleDegree();
	int GetAntiInvisibleDegree();
	int GetReputation();
	int GetSkillEnhance();
	int GetSkillEnhance2();
	int GetHistoricalMaxLevel();
	int GetVigour();
	int GetRealm();
	int GetLocalVal(int index);
	bool UseSheild(float cost);
	void EnhanceShieldEnergyMax(int val);
	void ImpairShieldEnergyMax(int val);
	void EnhanceShieldGen(int val);
	void ImpairShieldGen(int val);
	bool EnhanceShieldEnergy(float cost);
	bool ImpairShieldEnergy(float cost);
	void SetBodyScale(float scale);
	int GetOldSecLevel();
	int TakeOutItem(int item_id, unsigned int count);
	bool SummonPet3(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);

	// Novas cargas do mercenário
	void EnhanceChargeMerc(int val); //Adiciona
	void ImpairChargeMerc(int val); //Remove
	int GetCountChargeMerc();
	
	// Segment
	bool CooldownManager(char type, int idx, unsigned int time);
	bool CooldownManagerMassive(char type, unsigned int * table);
	
	void GetClearSkillCD(int skill_id);
	void GetUseSkillCD(int skill_id, unsigned int cdlimit, unsigned int coolingtime);
	
	void GetPaladinWeaponLight( unsigned char state );
	// money 172+
	
	unsigned int GetGold();
	void DecGold(unsigned int money);
	void DropGold(unsigned int money, bool isProtected);

	unsigned int GetSilver();
	void DecSilver(unsigned int money);
	void DropSilver(unsigned int money, bool isProtected);

	unsigned long long GetAllMoney();
	void DecAllMoney(unsigned long long money);
	
	// money 172 end

	// Novo Glyph Range
	void SetGlyphRange(int type, unsigned int count, int mode_1, int range_plus_1, int mode_2, int range_plus_2);
	void SetGlyphInfo(int type, unsigned int count, int feature1, int value1, int feature2, int value2, int feature3, int value3);

	void IncPetTime(int val);
	void DecPetTime(int val);
	int GetPetTime();

	//void SetRealWeaponClass(int weapon_class);
	//int GetRealWeaponClass();

	void CooldownAmulet();
	void ConvertPercDamageMtoF(int type, float ratio);

	void SummonNPC(int npc_id, int count, const XID& target, int target_distance, int remain_time=0);
	void FactionCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req);

	int GetEquipSlotCount();
	int QueryObjectRealmLevel(const XID & who);
	int QueryObjectSoulPower(const XID & who);

};
#endif

