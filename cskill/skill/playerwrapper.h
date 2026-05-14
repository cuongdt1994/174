#ifndef __SKILL_PLAYERWRAPPER_H
#define __SKILL_PLAYERWRAPPER_H

#include "common/types.h"
#include "obj_interface.h"

#include "skilllevel.h"
#include "callup.h"

#ifdef _SKILL_SIMULATION 
#define VIRTUALFUN virtual
#else
#define VIRTUALFUN
#endif

namespace GNET
{

class Skill;
class SkillWrapper;

#define  COOLINGID_BEGIN 1024
#define  COOLINGID_LIMIT_BEGIN 10240
#define ELFSTR_ADJ_IN_SKILL_ATTACK 1.0f
#define ELFAGI_ADJ_IN_SKILL_ATTACK 1.0f
#define ELE_METAL 0
#define ELE_WOOD 1
#define ELE_WATER 2
#define ELE_FIRE 3
#define ELE_EARTH 4

class PlayerWrapper
{
public:
	typedef unsigned int	SID;

protected:
	object_interface object;
	SkillWrapper  * skillwrapper;
	Skill * skill;
	const XID * target;
	int         tsize;
	SkillLevel	skilllevel;
	BuffLevel	bufflevel;
	ComboArg	comboarg;
	Callup	callup;

	// temporary parameter
	float	probability;	// State Attack Parameters
	float	ratio;			// State Attack Parameters
	float	amount;			// State Attack Parameters
	float	value;			// State Attack Parameters
	float	newvalue;			
	float	newvalue2;			
	float	newvalue3;			
	float   newvalue4;          
	float   newvalue5;          
	float   newvalue6;          
	float   newvalue7; 
	double  newvalue8;         

	A3DVECTOR	targetpos;			
	bool	showicon;		// Whether to display the status icon
	int 	time;			// duration
	int     intarg;
	bool    enable;  

	char    success;
	short   immune;
	bool    invader;

public:
	PlayerWrapper() {}
	virtual ~PlayerWrapper() {}
	PlayerWrapper(object_interface o,SkillWrapper *w=NULL, Skill *s=NULL, const XID* t=NULL, int size=0)
		: object(o), skillwrapper(w), skill(s), target(t), tsize(size), skilllevel(w,o.GetClass()),bufflevel(object),
		comboarg(w), probability(0), ratio(0), amount(0), value(0), newvalue(0), newvalue2(0), newvalue3(0), newvalue4(0), 
		newvalue5(0), newvalue6(0), newvalue7(0), newvalue8(0), showicon(0), time(0), success(true), immune(0)
	{
		if(tsize<=0)
			target = NULL;
	}

	int GetWorldTag()   { return object.GetWorldTag(); }
	int GetId()			{ return object.GetSelfID().id; }
	int GetMp()         { return object.GetBasicProp().mp; }
	int GetHp()         { return object.GetBasicProp().hp; }
	int GetSp( )        { return object.GetBasicProp().skill_point; }
	int GetAp()         { return object.GetBasicProp().ap; }
	int GetCls()        { return object.GetClass(); }
	int GetForm()       { return object.GetForm(); }
	int GetMaxhp()      { return object.GetExtendProp().max_hp; }
	int GetMaxmp()      { return object.GetExtendProp().max_mp; }
	int GetLevel()      { return object.GetBasicProp().level; }
	int GetRank()       { return object.GetBasicProp().sec.level; }
	int GetFlag()       { return object.GetBasicProp().sec.flag; }
	int GetPettype()	{ return object.GetPetType(); }
	int GetMoney()      { return object.GetAllMoney(); }
	int GetCurWeapon()  { return object.GetCurWeapon().weapon_class; }
	int GetCurweapon()  { return object.GetCurWeapon().weapon_class; }
	int GetCurFakeWeapon()  { return object.GetFakeWeaponClass(); }
	int GetCurFakeweapon()  { return object.GetFakeWeaponClass(); }
	int GetCurRealWeapon()  { return object.GetRealWeaponClass(); }
	int GetCurRealweapon()  { return object.GetRealWeaponClass(); }
	float GetCurweaponRange();

	int GetWeapondamage()
	{
		const item_prop& item = object.GetCurWeapon();
		return (item.damage_low + rand()%(item.damage_high-item.damage_low+1));
	}
	int GetWeaponmagicdamage()
	{ 
		const item_prop& item = object.GetCurWeapon();
		return (item.damage_magic_low + rand()%(item.damage_magic_high-item.damage_magic_low+1));
	}
	bool GetInteam()    { return object.IsInTeam(); }
	bool CanAttack()    { return object.CanAttack(XID(-1,-1)); }
	float GetRange()    { return object.GetExtendProp().attack_range; }
	const XID& GetXid() { return object.GetSelfID(); }
	int GetAttack()     { return object.GenerateDamage(); }
	int GetMagicattack(){ return object.GenerateMagicDamage(); }
	int GetPureattack()		{ return object.GenerateDamageWithoutRune(); }
	int GetPuremagicattack(){ return object.GenerateMagicDamageWithoutRune(); }
	int GetDefenddegree(){ return object.GetDefendDegree(); }
	int GetAttackdegree(){ return object.GetAttackDegree(); }
	int GetDefense()	{ return object.GetExtendProp().defense;}
	int GetGoldresist()	{ return object.GetExtendProp().resistance[0]; }
	int GetWoodresist()	{ return object.GetExtendProp().resistance[1]; }
	int GetWaterresist(){ return object.GetExtendProp().resistance[2]; }
	int GetFireresist()	{ return object.GetExtendProp().resistance[3]; }
	int GetEarthresist(){ return object.GetExtendProp().resistance[4]; }
	int GetCrit()		{ return object.GetCrit(); }
	int GetCritDamage()		{ return object.GetCritDamage(); }
	int GetSoulpower()	{ return object.GetSoulPower(); }
	int GetHistoricalmaxlevel(){return object.GetHistoricalMaxLevel();}
	int GetRealm()		{ return object.GetRealm();}
	int GetEquipattack() { return object.GenerateEquipDamage();}
	int GetEquipmagicattack() { return object.GenerateEquipMagicDamage();}

	int GetAttack(int ratio, int plus)      { return object.GenerateDamage(ratio, plus); }
	int GetMagicattack(int ratio, int plus) { return object.GenerateMagicDamage(ratio, plus); }

	int GetCritAttack(int ratio, int plus)      { return object.GenerateCritDamage(ratio, plus); }
	int GetCritMagicattack(int ratio, int plus) { return object.GenerateCritMagicDamage(ratio, plus); }

	int GetLocalval0() { return object.GetLocalVal(0); }
	int GetLocalval1() { return object.GetLocalVal(1); }
	int GetLocalval2() { return object.GetLocalVal(2); }

	void SetCombat(bool)     { object.EnterCombatState(); }
	bool GetCombat()		 { return object.IsCombatState(); }	
	bool UseArrow(int count) { return object.UseProjectile(count); }
	//bool SetDecmp(float dec) { return object.DrainMana((int)dec); }
	bool SetDecmp(float dec);
	//elf ���??  lgc
	int GetElfmp()         	{ return object.GetElfVigor(); }
	int GetElfap()         	{ return object.GetElfStamina(); }
	bool SetDecelfmp(int dec) { return object.DrainElfVigor(dec); }
	bool SetDecelfap(int dec) { return object.DrainElfStamina(dec); }

	int GetLanding()   { return object.IsOnGround(); }
	int GetAiring()    { return object.IsInAir(); }
	int GetWatering()  { return object.IsInWater(); }
	int GetRiding()    { return object.IsMountMode(); }

	float GetSkillenhance2() { return object.GetSkillEnhance2() / 100.0f; }

	float GetRangetotarget();
	bool GetInrange( float range, float& correction, char type = 0 );
	bool GetInrange( A3DVECTOR& tpos, float tbody,  float range, float& correction );
	bool IsUsingWeapon(int weapon) { return weapon == object.GetCurWeapon().weapon_class; }
	bool IsUsingFakeWeapon(int weapon) { return weapon == object.GetFakeWeaponClass(); }
	bool IsUsingRealWeapon(int weapon) { return weapon == object.GetRealWeaponClass(); }
	bool IsBuffWeapon(int weapon);

	int GetSkilllevel(SID id);
	int GetProduceSkill();
	int GetDynskillcounter();
	int GetLvalue();
	float GetPrayrangeplus();

	VIRTUALFUN int GetRand() {  return rand()%100; }

	void SetSkill(Skill* s) { skill = s; }
	bool SetSkilllevel(SkillLevel *) { return false; }
	SkillLevel * GetSkilllevel() { return &skilllevel; }
	bool SetBufflevel(BuffLevel *) { return false; }
	BuffLevel * GetBufflevel() { return &bufflevel; }
	bool SetComboarg(ComboArg*)  { return false; }
	ComboArg * GetComboarg() { return &comboarg; }

	int  GetBalls();
	void SetAddball(int type);
	void SetFilpball(int);

	void SetComboid(int id);

	void SetProbability(float d) { if(d>=0) probability = d; }//Note: The planner may set the probability to -1 in the editor, so that several hit effects will trigger at the same time
	float GetProbability() { return probability; }
	int GetProbabilityInt()    { return (int)(probability+0.00001f); }

	bool ThrowDice()
	{
		if(probability>99)
			return true;
		if(probability<0.001)
			return false;
		bool ret = (rand()%100)<probability;
		probability = ret ? 100:0;
		return ret;
	}

	float GetRatio()    { return ratio; }
	float GetValue()    { return value; }
	float GetAmount()   { return amount; }
	float GetNewValue()   { return newvalue; }
	float GetNewValue2()   { return newvalue2; }
	float GetNewValue3()   { return newvalue3; }
	float GetNewValue4()   { return newvalue4; }
	float GetNewValue5()   { return newvalue5; }
	float GetNewValue6()   { return newvalue6; }
	float GetNewValue7()   { return newvalue7; }
	double GetNewValue8()   { return newvalue8; }

	int GetRatioInt()   { return (int)(ratio+0.00001f);	}
	int GetValueInt()   { return (int)(value+0.00001f); }
	int GetAmountInt()   { return (int)(amount+0.00001f); }
	int GetNewValueInt()   { return (int)(newvalue+0.00001f); }
	int GetNewValue2Int()   { return (int)(newvalue2+0.00001f); }
	int GetNewValue3Int()   { return (int)(newvalue3+0.00001f); }
	int GetNewValue4Int()   { return (int)(newvalue4+0.00001f); }
	int GetNewValue5Int()   { return (int)(newvalue5+0.00001f); }
	int GetNewValue6Int()   { return (int)(newvalue6+0.00001f); }
	int GetNewValue7Int()   { return (int)(newvalue7+0.00001f); }
	//int GetNewValue8Int()   { return static_cast<int>(newvalue8 < 0 ? newvalue8 - 0.5 : newvalue8 + 0.5); }
	int GetNewValue8Int()   { return (int)(newvalue8+0.00001); }

	bool GetShowicon() { return showicon; }
	bool GetSuccess()   { return success; }
	short GetImmune()    { return immune; }
	void SetTalentData(int *list);
 
	int GetTime()       { return time; }
	bool SetTime(float t)     
	{ 
		time = (int)((t+0.00001f)/1000); 
		//if(time==0)
		//	time = 1;
		return true; 
	}
	bool SetRatio(float d)    { ratio = d; return true; }
	bool SetValue(float d)    { value = d; return true; }
	bool SetAmount(float d)   { amount = d; return true; }
	bool SetNewValue(float d)   { newvalue = d; return true; }
	bool SetNewValue2(float d)   { newvalue2 = d; return true; }
	bool SetNewValue3(float d)   { newvalue3 = d; return true; }
	bool SetNewValue4(float d)   { newvalue4 = d; return true; }
	bool SetNewValue5(float d)   { newvalue5 = d; return true; }
	bool SetNewValue6(float d)   { newvalue6 = d; return true; }
	bool SetNewValue7(float d)   { newvalue7 = d; return true; }
	bool SetNewValue8(double d)   { newvalue8 = d; return true; }

	bool SetPos(const A3DVECTOR & d)   { targetpos = d; return true; }
	bool SetShowicon(int d) { showicon = d; return true; }

	bool SetIntarg(int n){ intarg = n; return true; }
	int GetIntarg() { return intarg; }

	bool SetEnable(bool b){ enable = b; return true; }
	bool GetEnable() { return enable; }

	bool SetInvader(bool b){ invader = b; return true; }
	bool GetInvader() { return invader; }
	int GetElfstr();	//What is actually obtained is the adjusted value, the coefficient ELFSTR_ADJ_IN_SKILL_ATTACK, this value is used in skill attack and status attack
	int GetElfagi();	//What is actually obtained is the adjusted value, the coefficient ELFAGI_ADJ_IN_SKILL_ATTACK, this value is used in skill attack and status attack
	int GetElfvit();
	int GetElfeng();
	int GetElflevel();
	int GetTargetregionplayernum();
	unsigned int GetCharging();
public:
	bool SetHp(float hp) 
	{
		int dec = (int)(object.GetBasicProp().hp - hp);
		if(dec>0)
			object.DecHP(dec);
		else if(dec<0)
			object.Heal(-dec);
		return true;
	}

	bool SetDecsp(int sp)
	{
		if(sp>0)
			object.DecSkillPoint(sp);
		return true;
	}

	bool SetMp(float mp) 
	{ 
		int dec = (int)(object.GetBasicProp().mp - mp);
		if(dec>0)
			return object.DrainMana((int)dec);
		else if(dec<0)
			object.InjectMana(-dec);
		return false;
	}

	bool SetForm(int b)   { return true; }
	bool SetMaxhp(int hp) { return true; }
	bool SetInteam(bool)  { return true; }
	bool SetSp(int sp)    { return false; }
	bool SetRange(float)  { return false; }
	bool SetLevel(int)    { return false; }
	bool SetRank(int)     { return false; }
	int  GetDecsp()       { return 0; }
	int  GetDecmp()       { return 0; }
	bool SetHpgen(float);
	bool SetMpgen(float);


	float GetIncgold();
	float GetIncwood();
	float GetIncwater();
	float GetIncfire();
	float GetIncearth();

	bool SetPray(int) { return true; }
	bool SetInform(int) { return true; }
	bool SetPerform(int);
	bool SetDirecthurt(bool);
	bool SetHeal(bool);

	bool SetResurrect(bool);
	bool SetSpeedup(bool);
	bool SetIncdefence(bool);
	bool SetIncattack(bool);
	bool SetIncsmite(bool);
	bool SetIncmagic(bool);
	bool SetDecmagic(bool);
	bool SetAbsorbhp(bool);
	bool SetInchp(bool);
	bool SetDechp(bool);
	bool SetIncmp(bool);
	bool SetIncdodge(bool);
	bool SetDecdodge(bool);

	bool SetCallup(Callup*) { return false; }
	Callup* GetCallup() { return &callup; }

	bool SetStartcallup(int);
	int GetStartcallup() { return 0; }

	// The following properties are mainly used for Victim objects
	bool SetRepel(bool);
	bool SetRepel2(bool);
	bool SetRepel3(bool);
	bool SetRepelAtr(bool);
	bool SetBlind(bool);
	bool SetDecresist(bool);
	bool SetIncresist(bool);
	bool SetIncresist2(bool);
	bool SetInchurt(bool);
	bool SetDechurt(bool);
	bool SetDecaccuracy(bool);
	bool SetIncaccuracy(bool);

	bool SetMagicleak(bool);
	bool SetDecattack(bool);
	bool SetDecdefence(bool);	
	bool SetFrighten(bool);
	bool SetDizzy(bool);
	bool SetSleep(bool);
	bool SetFix(bool);
	bool SetSealed(bool);
	bool SetSlow(bool);
	bool SetBleeding(bool);
	bool SetThunder(bool);
	bool SetToxic(bool);
	bool SetBurning(bool);
	bool SetFallen(bool);
	bool SetFlood(bool);
	bool SetEnmity(int n);
	bool SetFeathershield(bool);
	bool SetFireshield(bool) ;
	bool SetIceshield(bool);
	bool SetRetort(bool);
	bool SetSlowpray(bool);
	bool SetFastmpgen(bool);
	bool SetScaleinchp(bool);
	bool SetScaleincmp(bool);
	bool SetScaledecmp(bool);
	bool SetScaledechp(bool);
	bool SetFasthpgen(bool);
	bool SetFastattack(bool);
	bool SetFastattack2(bool);
	bool SetSlowattack(bool);
	bool SetSoilshield(bool);
	bool SetIceblade(bool);
	bool SetEnhancegold(bool);
	bool SetEnhancewood(bool);
	bool SetEnhancewater(bool);
	bool SetEnhancefire(bool);
	bool SetEnhancesoil(bool);

	bool SetReducegold(bool);
	bool SetReducewood(bool);
	bool SetReducewater(bool);
	bool SetReducefire(bool);
	bool SetReducesoil(bool);

	bool SetFireblade(bool);
	bool SetToxicblade(bool);
	bool SetTigerform(bool b);
	bool SetFastride(bool b);
	bool SetFoxform(bool b);
	bool SetSwiftform(bool b);
	bool SetCleardebuff(bool);
	bool SetCleardebuff2(bool);
	bool SetCleardebuff3(bool);
	bool SetInvincible(bool);
	bool SetInvincible2(bool);
	bool SetInvincible3(bool);
	bool SetInvincible4(bool);
	bool SetInvincible5(bool);
	bool SetInvincible6(bool);
	bool SetInvincible7(bool);
	bool SetInvincibleAtr(bool);
	bool SetInvincibleWf(bool);
	bool SetBarbianAssault(bool);
	bool SetBarbianJumpToTarget(bool);
	bool SetWfJumpToTarget(bool);

	bool SetFastpray(bool);
	bool SetAntiwater(bool);
	bool SetPanruo(bool);
	bool SetBreak(bool);
	bool SetAssault(bool m);
	bool SetAssault2(bool m);
	bool SetAssault3(bool m);

	bool SetReturntown(bool m);
	bool SetYijin(bool);
	bool SetXisui(bool);
	bool SetJingji(bool);
	bool SetFlower1(bool);
	bool SetFlower2(bool);
	bool SetFlower3(bool);
	bool SetFlower4(bool);

	bool SetMnfactionDecresist(bool b);
	bool SetPowerup(bool b);
	bool SetApgen(bool b);
	bool SetIncswim(float r);
	bool SetStoneskin(bool);
	bool SetIronshield(bool);
	bool SetWingshield(bool);
	bool SetFirearrow(bool);
	bool SetGiant(bool);
	bool SetBlessmagic(bool);
	bool SetEaglecurse(bool);
	bool SetDevilstate(bool);
	bool SetFreemove(bool);

	bool SetMoney(int n)        { return true; }
	bool SetUseitem(int n)      { return (object.TakeOutItem(n)!=-1); }
	bool SetUsemoney(int n)     { object.DecAllMoney(n); return true; }
	bool SetCheckitem(int n)    { return object.TakeOutItem(n)!=-1; }
	bool SetCheckmoney(int n)   { return object.GetAllMoney()>=(unsigned)n; }

	bool SetAp(bool b);
	bool SetEntrap(bool b);
	bool SetSummon(bool b);
	bool SetClearbuff(bool b);
	bool SetNoregain(bool b);
	bool SetApleak(bool b);
	bool SetCanti(bool b);
	bool SetDisappear(bool b);
	bool SetDisappearAtr(bool b);
	bool SetHp2mp(bool b);
	bool SetSharpblade(bool b);
	bool SetImmunesealed(bool b);//lgc
	bool SetImmunesleep(bool b);
	bool SetImmuneslowdizzy(bool b);
	bool SetImmunewound(bool b);
	bool SetImmuneall(bool b);
	bool SetImmunephysical(bool b);
	bool SetImmunefire(bool b);
	bool SetImmunewater(bool b);
	bool SetImmunemetal(bool b);
	bool SetImmunewood(bool b);
	bool SetImmunesoil(bool b);
	bool SetImmunemagical(bool b);
	bool SetArrogant(bool b);
	bool SetSlowswim(bool b);
	bool SetFastswim(bool b);
	bool SetSlowfly(bool b);
	bool SetFastfly(bool b);
	bool SetSlowride(bool b);
	bool SetApgencont(bool b);
	bool SetApgencont2(bool b);
	bool SetApleakcont(bool b);
	bool SetIncelfstr(bool b);
	bool SetIncelfagi(bool b);
	bool SetIncdefence2(bool b);
	bool SetWeakelement(bool b);
	bool SetDeeppoision(bool b);
	bool SetDeeppoisionPlayer(bool b);
	bool SetRooted(bool b);
	bool SetEarthguard(bool b);
	bool SetFury(bool b);
	bool SetSandstorm(bool b);
	bool SetHomefeeling(bool b);
	bool SetReducewater2(bool b);
	bool SetIncsmite2(bool b);
	bool SetDecdefence2(bool b);
	bool SetReducefire2(bool b);
	bool SetSlowattackpray(bool b);
	bool SetBurning2(bool b);
	bool SetBurningfeet(bool b);
	bool SetHardenskin(bool b);
	bool SetReducegold2(bool b);
	bool SetLeafdance(bool b);
	bool SetCharred(bool b);
	bool SetVacuum(bool b);
	bool SetImmuneblooding(bool b);
	bool SetAbsorbphysicdamage(bool b);
	bool SetAbsorbmagicdamage(bool b);
	bool SetRetortmagic(bool b);
	bool SetWindshield(bool b);
 	bool SetAirstreamlock(bool b);
	bool SetClosed(bool b);
	bool SetInsertvstate(bool b);
	bool SetImmuneweak(bool b);
	bool SetBefrozen(bool b);
	bool SetClearsealed(bool b);
	bool SetClearrooted(bool b);
	bool SetClearbleeding(bool b);
	bool SetCleardizzy(bool b);
	bool SetSelfburning(bool);
	bool SetFallen2(bool);
	bool SetSealed2(bool);
	bool SetFix2(bool);
	bool SetDechurt2(bool);
	bool SetDechurtBarbian(bool);
	bool SetInchurt2(bool);
	bool SetInchurt3(bool);
	bool SetInchurt4(bool);
	bool SetInchurt5(bool);
	bool SetInchurtMonkey(bool);
	bool SetDechurt3(bool);
	bool SetInchp2(bool);
	bool SetIncattack2(bool);
	bool SetIncmagic2(bool);
	bool SetIncmagic3(bool);
	bool SetFastpray2(bool);
	bool SetSpeedup2(bool);
	bool SetSkillcooldown(bool);
	bool SetSkillcooldownCount(bool);
	bool SetSkillcooldownAtr(bool);
	bool SetSkillcooldownAtrDec(bool);
	bool SetCommoncooldown(bool);
	bool SetAurafireattack(bool);
	bool SetAurabless(bool);
	bool SetAuracurse(bool);
	bool SetAurabless2(bool);
	bool SetAuracurse2(bool);
	bool SetAurabless3(bool);
	bool SetInvisible(bool);
	bool SetIncantiinvisibleactive(bool);
	bool SetInchpsteal(bool);
	bool SetInccritdamage(bool);
	bool SetIncdamagedodge(bool);
	bool SetIncdebuffdodge(bool);
	bool SetRebirth(bool);
	bool SetDeepenbless(bool);
	bool SetWeakenbless(bool);
	bool SetWeakenbless2(bool);
	bool SetHurtwhenuseskill(bool);
	bool SetInterruptwhenuseskill(bool);
	bool SetSoulretort(bool);
	bool SetSoulsealed(bool);
	bool SetSoulbeatback(bool);
	bool SetSoulbeatbackWf(bool);
	bool SetSoulbeatbackPaladin(bool);
	bool SetSoulstun(bool);
	bool SetFishform(bool);
	bool SetEntrap2(bool b);
	bool SetDeepicethrust(bool b);
	bool SetAdjustattackdefend(bool b);
	bool SetClearinvisible(bool b);
	bool SetBeastieform(bool b);
	bool SetGminvisible(bool);
	bool SetActivatesharpener(bool);
	bool SetInchurtphysicgold(bool);
	bool SetInchurtwoodwater(bool);
	bool SetInchurtfireearth(bool);
	bool SetAttackattachstate1(bool);
	bool SetAttackattachstate2(bool);
	bool SetAttackattachstate3(bool);
	bool SetAttackattachstate4(bool);
	bool SetBeattackattachstate1(bool);
	bool SetBeattackattachstate2(bool);
	bool SetBeattackattachstate3(bool);
	bool SetBeattackattachstate4(bool);
	bool SetBeattackattachstate5(bool);
	bool SetBeattackattachstate6(bool);
	bool SetBeattackattachstate7(bool);
	bool SetBeattackattachstate8(bool);
	bool SetTransferbuff(bool);
	bool SetTransferdebuff(bool);
	bool SetAbsorbbuff(bool);
	bool SetAbsorbdebuff(bool);
	bool SetSummonpet2(bool);
	bool SetSummonplantpet(bool);
	bool SetSummonplantpet2(bool);
	bool SetSummonexplosivepet(bool); // Atr Skill 1
	bool SetPetsacrifice(bool);
	bool SetPlantsuicide(bool);
	bool SetPlantsuicide2(bool); // Atr
	bool SetPoisionseed(bool);
	bool SetHpgenseed(bool);
	bool SetPhysichurt(bool);
	bool SetPhysichurtRuge(bool);
	bool SetGoldhurt(bool);
	bool SetWoodhurt(bool);
	bool SetWaterhurt(bool);
	bool SetFirehurt(bool);
	bool SetEarthhurt(bool);
	bool SetFastprayincmagic(bool);
	bool SetIncwoodwaterdefense(bool);
	bool SetSpecialslow(bool);
	bool SetSpecialphysichurt(bool);
	bool SetIncdefencesmite(bool);
	bool SetIncresistmagic(bool);
	bool SetTransportmptopet(bool);
	bool SetTransportdamagetopet(bool);
	bool SetAbsorbdamageincdefense(bool);
	bool SetIncrementalhpgen(bool);
	bool SetChanceofrebirth(bool);
	bool SetSpecialphysichurttrigger(bool);
	bool SetLongjumptospouse(bool);
	bool SetInccountedsmite(bool);
	bool SetWeapondisabled(bool);
	bool SetWeaponenabled(bool);
	bool SetIncaggroondamage(bool);
	bool SetEnhanceskilldamage(bool);
	bool SetDetectinvisible(bool);
	bool SetDetectinvisible2(bool);
	bool SetFastmpgen2(bool);
	bool SetClearinvisible2(bool);
	bool SetPositionrollback(bool);
	bool SetPositionrollback2(bool);
	bool SetHprollback(bool);
	bool SetNofly(bool);
	bool SetForceselecttarget(bool);
	bool SetForceselecttarget2(bool);
	bool SetForceselecttarget3(bool);
	bool SetHealabsorb(bool);
	bool SetRepelonnormalattack(bool);
	bool SetInccritresistance(bool);
	bool SetDeccritresistance(bool);
	bool SetExchangeposition(bool);
	bool SetPullover(bool);
	bool SetTransmitskillattack(bool);
	bool SetAdditionalheal(bool);
	bool SetAdditionalattack(bool);
	bool SetTransportdamagetoleader(bool);
	bool SetForbidbeselected(bool);
	bool SetEnhanceskilldamage2(bool);
	bool SetCallupteammember(bool);
	bool SetDelayearthhurt(bool);
	bool SetDizzyinchurt(bool);
	bool SetSoulretort2(bool);
	bool SetFixondamage(bool);
	bool SetApgen2(bool);
	bool SetIncattackondamage(bool);
	bool SetRebirth2(bool);
	bool SetHealsteal(bool);
	bool SetDropmoneyondeath(bool);
	bool SetIncattackrange(bool);
	bool SetQueryotherinventory(bool);
	bool SetThunderform(bool);
	bool SetDelaytransmit(bool);
	bool SetDecnormalattackhurt(bool);
	bool SetFreemoveapgen(bool);
	bool SetIncatkdefhp(bool);
	bool SetDenyattackcmd(bool);
	bool SetHpmpgennotincombat(bool);
	bool SetInchpmp(bool);
	bool SetEnternonpenaltypvp(bool);
	bool SetLeavenonpenaltypvp(bool);
	bool SetIncmaxhpatkdfdlevel(bool);
	bool SetRetort2(bool);
	bool SetKnockup(bool);
	bool SetPalsy(bool);

	bool CheckTarget(float range, char type);
	bool CheckComboState(unsigned int sid, int interval);

	// Passive skill interface
	bool SetIncrange(float);
	bool SetIncrangeatr(float); // Atiradora
	bool SetIncacertatr(float); // Atiradora

	bool SetIncgold(float inc);
	bool SetIncwood(float inc);
	bool SetIncwater(float inc);
	bool SetIncfire(float inc);
	bool SetIncearth(float inc);

	void SetTimerAdittionPetSummon(int inc);

	bool SetIncbow(float);
	bool SetIncsword(float);
	bool SetIncspear(float);
	bool SetInchammer(float);
	bool SetIncboxing(float);
	bool SetIncfight(float);
	bool SetIncdagger(float);
	bool SetInctalisman(float);
	bool SetIncscimitar(float);
	bool SetIncfeather(float);
	
	bool SetIncfirearm(float); // Atiradora
	bool SetIncswordshield(float); // Paladino
	bool SetInccritshield(float); // Paladino 2

	bool SetIncmpgen(float);
	bool SetInchpgen(float);

	bool SetAdddefence(float m);

	bool SetInccrit(int point);
	bool SetInchitrate(float ratio);

	bool SetIncantiinvisiblepassive(int inc);
	bool SetIncinvisiblepassive(int inc);

	bool SetIncpethp(float inc);
	bool SetIncpetmp(float inc);
	bool SetIncpetdamage(float inc);
	bool SetIncpetmagicdamage(float inc);
	bool SetIncpetdefense(float inc);
	bool SetIncpetmagicdefense(float inc);
	bool SetIncpetattackdegree(float inc);
	bool SetIncpetdefenddegree(float inc);
	bool SetReduceresurrectexplost(int reduce);
	bool SetImmunedrop(int);
	bool SetAddresistance(float inc);
	bool SetAddskilldamage(float inc);
	
	bool SetIncnearnormaldmgreduce(float);
	bool SetIncnearskilldmgreduce(float);
	bool SetIncfarnormaldmgreduce(float);
	bool SetIncfarskilldmgreduce(float);
	bool SetAddmaxhp(float inc);
	bool SetDisturbrecover(int val);
	bool SetIncswimspeed(float inc);
	bool SetIncdefenselevel(float inc); // New
	bool SetIncCritReduce(float inc); // New
	void CriticAttackEnhance(float inc); // New

	// Weapon continuous activation skills
	bool SetFrenetic(bool b);
	bool SetAddattackdegree(bool);
	bool SetSubattackdegree(bool);
	bool SetAdddefencedegree(bool);
	bool SetAdddefencedegreeAtr(bool);
	bool SetAddattackedegreeAtr(bool);
	bool SetSubdefencedegree(bool);

	bool SetSubdefencedegree2(bool);
	bool SetIncatkdefhp2(bool);
	bool SetIncsmite3(bool);
	bool SetIncpenres(bool);
	bool SetAttachstatetoself(bool);
	bool SetAttachstatetotarget(bool);
	bool SetImmunephysical2(bool b);
	bool SetImmunefire2(bool b);
	bool SetImmunewater2(bool b);
	bool SetImmunemetal2(bool b);
	bool SetImmunewood2(bool b);
	bool SetImmunesoil2(bool b);
	bool SetIncbecritrate(bool b);
	bool SetModifyspecskillpray(bool b);	
	bool SetIncspecskilldamage(bool b);	
	bool SetFireshield2(bool) ;
	bool SetIceshield2(bool);
	bool SetHealshield(bool);
	bool SetIncflyspeed(bool);
	bool SetIncvigour(bool);
	bool SetIncvigour2(bool);
	bool SetShortjump(bool);
	bool SetShortjump2(bool);
	bool SetHigherShortjump(bool);
	bool SetMovepunish(bool);
	bool SetStandpunish(bool);
	bool SetStandpunish2(bool);
	bool SetChantshield(bool);
	bool SetChangeselfaggro(bool);	
	bool SetSummonmobs(bool);
	bool SetIntervalpalsy(bool);
	bool SetInternalinjury(bool);
	bool SetAtkdamagereduce(bool);
	bool SetDeathresetcd(bool);
	bool SetDeathresetcd2(bool);
	bool SetAppendenchant(bool);
	bool SetAppenddamage(bool);
	bool SetCooldownaward(bool);
	bool SetHuntersoul(bool);
	bool SetNeverdead(bool);
	bool SetChangemodel(bool);
	bool SetScreeneffect(bool);
    bool SetShadowform(bool b);
    bool SetFairyform(bool b);
    bool SetAddfrosteffect(bool);
    bool SetActivefrosteffect(bool);
    bool SetIncspecskillcrit(bool);
    bool SetClearfilterbyid(bool);
    bool SetMoongod(bool);
    bool SetDelayskilleffect(bool);
    bool SetEnhanceskilldamage3(bool);
	bool SetCritdamagereduce(bool);
	bool SetImmunemagical2(bool);
	bool SetPullover2(bool);
    bool SetIncphysicaldamage(bool);
    bool SetIncmagicaldamage(bool);
    bool SetRemoveaggro(bool);
	bool SetCooldownpunish(bool);
	bool SetAnticlearbuf(bool);
	bool SetIncenchantrange(bool);
    bool SetIncphysicalmagicaldefense(bool);
    bool SetReducegold3(bool);
    bool SetReducewater3(bool);
    bool SetGenhpap(bool);
	bool SetDetaindart(bool);
    bool SetExtraexpfactor(bool);
	bool SetSoloIncAttackAndMagic(bool);
	bool SetSoloIncDefence(bool);
	bool SetSoloEnhanceResist(bool);
	bool SetSoloIncMaxHP(bool);
	bool SetInvincible8(bool);
	bool SetSoloHpGen(bool);
	bool SetSoloDecHurt(bool);
	bool SetSoloAddAttackRange(bool);
	bool SetThunder3(bool);
	bool SetToxic3(bool);
	bool SetFlood3(bool);
	bool SetBurning3(bool);
	bool SetFallen3(bool);
	bool SetCannonform(bool b);
	bool SetShattereddream(bool b);
	bool SetShattereddreammob(bool);
	bool SetReduceSpeeddebuff(bool);
	bool SetShieldNew1(bool);
	bool SetShieldNew2(bool);
	bool SetNewSpeedBuff(bool);
	bool SetIraAtiradora(bool);
	bool SetGeloAtiradora(bool b);
	bool SetFeAtiradora(bool);
	bool SetNewRemove(bool);
	bool SetAccumulateAttack(bool b); //UltWR
	bool SetAccumulateAttack2(bool b); //UltMG
	bool SetDecRegionDMG(bool b); //CuraWF
	bool SetIncAntiDefenseDegree(bool); //UltEA
	bool SetIncPlusMaxHP(bool); //UltEP
	bool SetInvisible2(bool); //UltMC
	bool SetAuracurse4ASN(bool); //UltMC
	bool SetIncmagic4(bool); //UltESP
	bool SetDecsoul(bool); //UltESP
	bool SetGiantform(bool); //UltSK
	bool SetTransdmg2pet(bool); //UltMS
	bool SetDecRegionDMG2(bool b);
	bool SetLoaddmg4master(bool b);
	bool SetDecfardmg(bool b);
	bool SetSkillcooldown2(bool);
	bool SetDecphysicalmagicaldefense(bool);

	//G17
	bool SetMagicFrenetic(bool b); 
	bool SetDecheal(bool b); 
	bool SetIncsmitedamage(bool);
	bool SetDecphysicalmagicaldefense2(bool);
	bool SetFastpray3(bool);
	bool SetIncdebuffdodge2(bool);
	bool SetIncdamagedodge2(bool);
	bool SetDebithurt(bool);

	// Nova torre 
	bool SetSoloAddAttackLevel(bool);
	bool SetSoloAddDefendLevel(bool);

	// Ulti Paladino
		
	bool SetUltimatePaladin(bool);

	// Ulti Atiradora 

	bool SetAttackplusAtr(bool);
	bool SetAddPraga(bool);
	bool SetskillplusAtr(bool);

	// Novo Atributo 

	bool SetHighestLevel(bool b);
	bool SetIncdefencePatente(float inc);
	
	// Novos Versos

	bool SetVersoSombrio01(bool);
	bool SetVersoSombrio02(bool);
	bool SetVersoSombrio03(bool);
	bool SetVersoSombrio04(bool);
	bool SetVersoSombrio05(bool);
	bool SetVersoSombrio06(bool);


	void SetCommonCoolDown(int cd_mask, int ms);
	
	int FirstRunRelease(int cls, int incast) { return object.FirstRunRelease(cls, incast); }
	int BugSkillRelease(int id, int cls) { return object.BugSkillRelease(id, cls); }
	bool UseSheild(float cost);
	bool SetNewSheildMax(float inc);
	bool SetNewSheildGen(float inc);
	bool EnhanceShieldEnergy(bool);
	bool ImpairShieldEnergy(bool);
	
	bool SetHealnurse(bool);
	
	bool SetUseitem(int n, unsigned int c)      { return (object.TakeOutItem(n,c)!=-1); }
	bool SetCheckitem(int n, unsigned int c)    { return object.TakeOutItem(n,c)!=-1; }
	
	bool SetRetort3(bool);
	bool SetElfLock(bool);
	bool SetFreecooldown(bool);
	bool SetSleep2(bool b);	
	
	// skill coldown limit 
	bool CooldownManager(char type, int idx, unsigned int time);
	bool CooldownManagerMassive(char type, unsigned int * table);
	
	void SetMultiCoolDown();
	
	// asn combo skill
	int GetChargeMerc();
	void GainChargeMerc( int val );
	void SpendChargeMerc( int val );
	// asn combo skill end
	
	int GetTargetClass();
	int GetTargetHp();	
	int GetTargetMp();
	int GetTargetMaxhp();
	int GetTargetLevel();

	bool GetTargetIsFlyMode();
	bool GetTargetIsMountMode();
	bool GetTargetIsInvisible();
	bool GetTargetIsSealed();
	bool GetTargetIsStun();
	bool GetTargetIsSleep();
	bool GetTargetIsRoot();

	bool GetDebuffSeeker1();
	bool GetDebuffSeeker2();
	bool GetDebuffSeeker3();
	bool GetDebuffSeeker4();
	
	//++++++++++++++++172+++++++++++++++++++++
	
	bool SetInvincibleMonkey(int b);
	bool SetInvincibleMonkeyNew(bool);
	bool SetClearbuffRandom(bool b);
	bool SetClearbuffRandom3(bool b);
	bool SetClearbuffRandom2(bool b);
	bool SetCleardebuffRandom(bool b);
	bool SetAuracurseMonkey(bool);
	bool SetMonkeyinvisible(bool);
	bool SetSummonplantpetNew(bool);
				
	bool SetIncfeather2(float);
	bool SetIncsbastao(float); // Andarilho 01
	bool SetIncsbastaoCritico(float); // Andarilho 01
	bool SetIncsformmonkey(float); // Andarilho 02
	bool SetIncsformmonkeyCritico(float); // Andarilho 02
	bool SetIncsformmonkeyDefense(float inc); // Andarilho 03
	
	bool SetIncCritReduce2(bool b); // New
	bool SetIncAtkFToDefense(bool);
	bool SetIncNewShieldMage(bool);
	bool SetIncNewShieldMage2(bool);
	bool SetNewShieldMage(bool);
	bool SetBuffAtiradora(bool b);
	bool SetBuffPaladin(bool);
	
	bool SetShattereddreamMonkey(bool);
	bool SetArmadilhaPvP(bool);
	
	// Monkey new Skills

	bool SetMonkeyform(bool b);
	bool SetMonkeyformUltimate(bool b);
	bool SetMarcadoVento(bool b);
	bool SetMarcadaFloresta(bool b);
	bool SetMarcadofogo(bool b);
	bool SetMarcadamontanha(bool b);
	bool SetMarcaYin(bool b);
	bool SetMarcaYang(bool b);
	bool SetFreemoveMonkey(bool);
	bool SetFreemoveMonkey2(bool);
	bool SetFreemoveMonkey3(bool);
	bool SetMovimentodasMaos(bool);
	bool SetFeMonkey(bool);
	bool SetComboMonkey(bool b);
	bool SetComboYinYang(bool b);
	bool SetComboYinYang2(bool b);
	bool SetRemoveFilterMonkey(bool);
	bool SetRemoveFilterMonkey2(bool);
	bool SetReduzDanoaDistancia(bool b);

	// Novas Passivas 
	bool SetAddskilldamage2(float inc);
	bool SetAttackMPasive(int inc);
	bool SetAttackFPasive(int inc);
	bool SetIncPercDefenseMagicFisic(float inc);
	bool SetReduceDefensePassive(float inc);
	// Arena Guan Yu 

	bool SetRedTeamIcon(bool);
	bool SetBlueTeamIcon(bool);
	
	bool SetNewWeapon(bool);

	int GetFilterID(int id)	{ return object.IsFilterExist(id); } // Novo

	bool SetFormCls(int inc);

	// Novas Runas
	bool SetDoubleChiCost(bool); // Custo de Chi é dobrado
	bool CheckBuffState(int hstate);
	bool CheckDoubleChiCost(int hstate);
	bool SetGlyphRange(bool);
	bool SetGlyphRange2(bool);
	float GetGlyphPrayDistanceInc();
	int GetGlyphApCostInc();
	int GetGlyphCastingInc();
	bool SetGlyphCritical(bool);
	bool SetGlyphCriticalAtr(bool);
	bool SetGlyphCritical2(bool);
	bool SetGlyphCriticalPaladin(bool);
	bool SetGlyphCritical3(bool);
	bool SetGlyphDamage(bool);
	bool SetGlyphApCost(bool);
	bool SetGlyphApCostMulti(bool);
	bool SetGlyphCasting(bool);
	bool SetShieldDamageReduce(bool);
	bool SetApplyDamageforRange(bool);
	bool SetIncShadowRollback(bool);
	bool SetDecShadowRollback(bool);
	bool SetGlyphJumpRollback(bool);
	bool SetGlyphJumpRollbackDec(bool);
	bool IsFilterExist(int id);
	bool SetShieldDamageReduceBarbian(bool);
	bool SetShieldTransferDebuffBarbian(bool);

	bool SetSummonpet3(bool);
	bool SetDizzyPerformer(bool);
	bool SetForceselecttargetRoar(bool);
	bool SetImmuneRoar(bool); // ImmuneRoar

	bool SetInchurtphysicgoldDesordem(bool);
	bool SetInchurtwoodwaterDesordem(bool);
	bool SetInchurtfireearthDesordem(bool);

	bool SetAddPetTime(int inc);

	bool SetForceselecttargetSurfImpact(bool);
	bool SetDizzyNoImune(bool);
	bool SetCantiBlow(bool b);

	bool SetResetAmuletCD(bool);

	bool SetCooldownAmulet(bool);
	bool SetDeathresetcdVoidStep(bool);
	bool SetForceselecttargetPet(bool);
	bool SetSummonpet4(bool);

	bool SetBorboletaSombria(bool);
	bool SetRastrodeLotus(bool);
	bool SetRastrodePorco(bool);

	bool SetEnhanceskilldamagePELOTAO(bool);

	bool SetForcePetCoolDown(bool);
	bool SetGlyphCriticalValid(bool);
	bool SetForceCriticalAttack(bool);

	bool SetEnhanceskilldamageBarbian(bool);
	bool SetEnhanceskilldamagePlayer(bool);
	bool SetContinuousDamageGold(bool);
	bool SetContinuousDamageWood(bool);
	bool SetContinuousDamageWater(bool);
	bool SetContinuousDamageFire(bool);
	bool SetContinuousDamageEarth(bool);
	bool SetContinuousDamageActive(bool);
	int CheckContinuousExist();
	bool IsPetActive();
	bool SetPurehurtFromHp(bool);
	bool SetNewShapeAndSkills(bool);
	bool AddPointdsDay(bool);
	bool RemovePointdsDay(bool);
	bool SetCriticalFix(bool b);

	bool CheckCriticalDamage();
	bool SetAmuletInstantCoolDown(bool);
	bool SetAmuletInstantFreeCoolDown(bool);

	bool SetElmodisabled(bool);
	bool SetGlyphMercInvisible(bool);

	bool SetFilterDebuffSK1Inc(bool);
	bool SetFilterDebuffSK2Inc(bool);
	bool SetFilterDebuffSK3Inc(bool);
	bool SetFilterDebuffSK4Inc(bool);

	bool SetFilterDebuffSK1Dec(bool);
	bool SetFilterDebuffSK2Dec(bool);
	bool SetFilterDebuffSK3Dec(bool);

	inline bool GetDebuffSeeker1IsActive() { return object.IsFilterExist(4568); }
	inline bool GetDebuffSeeker2IsActive() { return object.IsFilterExist(4569); }
	inline bool GetDebuffSeeker3IsActive() { return object.IsFilterExist(4570); }
	inline bool GetDebuffSeeker4IsActive() { return object.IsFilterExist(4571); }

	bool SetDizzyondamage(bool);

	bool SetSwordForm(bool b);
	bool SetAxeForm(bool b);
	bool SetFistForm(bool b);
	bool SetPoleArmForm(bool b);

	bool SetImmuneTransfWB(bool);
	bool SetIncdefensedegreeondamage(bool);
	bool SetPhysMagReducwithstackWB(bool);

	bool SetNewStun(bool b);
	bool SetShieldDamageMystic(bool);

	bool SetRepel4(bool);

	bool SetRedClear(bool);

	bool SetAnticlearbuf2(bool);
	bool SetGiant2(bool);
	bool SetFastpray5(bool);
	bool SetConvertAtaqMtoAtaqF2(bool b);
	bool SetConvertAtaqMtoAtaqF(bool b);
	bool SetDecdefenceNew(bool);
	bool SetIncDefenseResitWithDamageStart(bool);

	bool SetElementDamage(bool);

	// Novos Bosses extraidos do emulador
	bool SetFilter14307(bool);
	bool SetRemoveFilter(int filter_id);

	// Guild 
	bool SendGuildLongJump(bool);
	bool SetGuildResurrect(bool);
	bool SetSummonGuildNpc(bool);
	bool SetFactionCongregateRequest(bool);
	bool SetFactionMountSpeed(float inc);
	bool SetFactionSpeed(float inc);
	bool SetGuildReduceDefensePassive(float inc);
	bool SetFactionEnhanceskilldamage(float inc);

	//G17/G18
	bool SetIncAttackAcc(int inc);
	bool SetInvisibleForced(bool);
	bool SetForceTransform(bool);
	bool SetRecoveryApperSec(bool);
	bool SetAdjustDamageSendRecv(bool);
	bool SetAdjustDamageSendRecv2(bool);
	bool SetAdjustHPFromMP(bool);
	bool SetIncDamageReducePerc(bool);
	bool SetIncImmuneNegativeEffect(bool);
	bool SetReduceNormalDamage(bool);
	bool SetSpeedNoCombat(bool);
	bool SetInccritNewDamage(int inc);
	bool SetIncAttackLevel(int inc);
	bool SetPlusDamageFromHP(bool);

	// Passe de Batalha 
	bool SetBattlePassEffect(bool);

};


}

#endif

