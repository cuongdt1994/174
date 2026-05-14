#ifndef __SKILL_SKILL_H
#define __SKILL_SKILL_H

#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <wchar.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <ASSERT.h>
#include "range.h"
#include "common/types.h"
#include "attack.h"
#include "playerwrapper.h"
#include "targetwrapper.h"

//#include "skilllua.h"

#ifdef INTEPRETED_EXPR 
#include "marshal.h"
#endif

#pragma pack(1)
namespace SKILL
{
	struct Data;
}

struct enchant_msg;
struct attack_msg;
struct XID;

namespace GNET
{
class  PlayerWrapper;
class  TargetWrapper;
class Skill;

enum { 
	TYPE_ATTACK    = 1,  // active attack
	TYPE_BLESS     = 2,  // Active blessing
	TYPE_CURSE     = 3,  // active curse
	TYPE_RUNE      = 4,  // item skills
	TYPE_PASSIVE   = 5,  // passive
	TYPE_ENABLED   = 6,  // Weapon Attachment
	TYPE_LIVE      = 7,  // Life
	TYPE_JUMP      = 8,  // Teleport
	TYPE_PRODUCE   = 9,  // produce (mutex)
	TYPE_BLESSPET  = 10, // pet blessing
	TYPE_NEUTRALBLESS = 11, //neutral blessing
	TYPE_NEUTRALBLESS2 = 12, //Neutral Blessing 2
};

enum // Same as gs/playertemplate.h
{   
	USER_CLASS_SWORDSMAN,       //WR
	USER_CLASS_MAGE,            //MG
	USER_CLASS_NEC,             //WB
	USER_CLASS_HAG,             //WF
	USER_CLASS_ORGE,            //PSY
	USER_CLASS_ASN,             //MC
	USER_CLASS_ARCHER,          //EA
	USER_CLASS_ANGEL,           //EP
	USER_CLASS_BLADE,           //SK
	USER_CLASS_GENIE,           //MS
	USER_CLASS_SHADOW,          //RT
	USER_CLASS_FAIRY,           //TM
	USER_CLASS_SHOOTER,           //ATR
	USER_CLASS_PALADIN,           //PAL
	USER_CLASS_MONKY,           //AND
	USER_CLASS_COUNT
};

const short skills_genesis_enum[] = // Skills Ultimate Nível de Genesis
{
		3198, // WR
		3205, // MG
		3212, // WB
		3217, // WF
		3229, // PSY
		3240, // MC
		3248, // EA
		3257, // EP
		3263, // SK
		3269, // MS
		3280, // RT
		3281, // TM
		4954, // ATR
		5111, // PAL
		6504  // AND
};

enum EVENT{
	EVENT_RESET  = 0x01,
	EVENT_WIELD  = 0x02,
	EVENT_CHANGE = 0x04,
	EVENT_ENTER  = 0x08,
	EVENT_NEW_ARMOR  = 0x10,
};

enum FORM
{
	FROM_MASK_HIGH 	= 0xC0,
	FORM_NORMAL		= 0,	//normal form
	FORM_CLASS		= 1,	//career change
	FORM_BEASTIE	= 2,	//Small animals
};

/* State attack stub, save some static data */
#ifdef INTEPRETED_EXPR 
class StateAttackStub : public Marshal
#else
class StateAttackStub
#endif
{
public:
	typedef unsigned int ID;

	ID id;
	std::string	name;
	std::string	icon;
	std::string	effect;

public:
	StateAttackStub(ID i,std::string n,std::string ic,std::string e)
		: id(i), name(n), icon(ic), effect(e)
	{ if(!GetStub(id)) GetMap().insert(std::make_pair(id, this)); }
	virtual ~StateAttackStub()	{ }

	typedef std::unordered_map<ID, const StateAttackStub*> Map;
	static Map& GetMap() { static Map map; return map; }
	static const StateAttackStub *GetStub(ID i)
	{
		Map::const_iterator it = GetMap().find(i);
		return it == GetMap().end() ? NULL : (*it).second;
	}

#ifdef INTEPRETED_EXPR 
	virtual OctetsStream& marshal(OctetsStream &os)	const
	{ return os << id << name << icon << effect; }
	virtual const OctetsStream& unmarshal(const OctetsStream &os)
	{ return os >> id >> name >> icon >> effect; }
#endif
};

/* Skill stub, save some static data */
#ifdef INTEPRETED_EXPR 
class SkillStub : public Marshal
{
public:
	class State : public Marshal
#else
class SkillStub 
{
public:
	class State
#endif
	{
	public:
		int				stateid;
		std::string		name;
		State() { }
		virtual ~State() { }
#ifdef INTEPRETED_EXPR 
		std::string		time_exp;
		std::string		quit_exp;
		std::string		loop_exp;
		std::string		bypass_exp;
		std::string		calculate_exp;
		std::string		interrupt_exp;
		std::string		cancel_exp;
		std::string		skip_exp;
	public:
		State(int id, std::string n,std::string te, std::string ee, std::string le, std::string be,
				std::string ce, std::string ie, std::string cae, std::string ske )
				: stateid(id), name(n), time_exp(te), quit_exp(ee), loop_exp(le), bypass_exp(be),
				calculate_exp(ce), interrupt_exp(ie), cancel_exp(cae), skip_exp(ske)
		{ }
		State(const State& rhs) : stateid(rhs.stateid), name(rhs.name), time_exp(rhs.time_exp),
				quit_exp(rhs.quit_exp), loop_exp(rhs.loop_exp), bypass_exp(rhs.bypass_exp),
				calculate_exp(rhs.calculate_exp), interrupt_exp(rhs.interrupt_exp), cancel_exp(rhs.cancel_exp),
				skip_exp(rhs.skip_exp)
		{ }
		State& operator = (const State& rhs)
		{
			stateid = rhs.stateid;
			name = rhs.name;
			time_exp = rhs.time_exp;
			quit_exp = rhs.quit_exp;
			loop_exp = rhs.loop_exp;
			bypass_exp = rhs.bypass_exp;
			calculate_exp = rhs.calculate_exp;
			interrupt_exp = rhs.interrupt_exp;
			cancel_exp = rhs.cancel_exp;
			skip_exp = rhs.skip_exp;
			return *this;
		}

		virtual OctetsStream& marshal(OctetsStream &os)	const
		{
			os << stateid << name << time_exp << quit_exp << loop_exp << bypass_exp;
			os << calculate_exp << interrupt_exp << cancel_exp << skip_exp;
			return os;
		}
		virtual const OctetsStream& unmarshal(const OctetsStream &os)
		{
			os >> stateid >> name >> time_exp >> quit_exp >> loop_exp >> bypass_exp;
			os >> calculate_exp >> interrupt_exp >> cancel_exp >> skip_exp;
			return os;
		}

		virtual int  GetTime(Skill * skill) const;
		virtual bool Quit(Skill * skill) const;
		virtual bool Loop(Skill * skill) const;
		virtual bool Bypass(Skill * skill) const;
		virtual void Calculate(Skill * skill) const;
		virtual bool Interrupt(Skill * skill) const;
		virtual bool Cancel(Skill * skill) const;
		virtual bool Skip(Skill * skill) const;
#else
	public:
		State(int id, std::string n) : stateid(id), name(n) { }
		State(const State& rhs) : stateid(rhs.stateid), name(rhs.name) { }
		State& operator = (const State& rhs)
		{
			stateid = rhs.stateid;
			name = rhs.name;
			return *this;
		}
		virtual int  GetTime(Skill * skill) const = 0;
		virtual bool Quit(Skill * skill) const = 0;
		virtual bool Loop(Skill * skill) const = 0;
		virtual bool Bypass(Skill * skill) const = 0;
		virtual void Calculate(Skill * skill) const = 0;
		virtual bool Interrupt(Skill * skill) const = 0;
		virtual bool Cancel(Skill * skill) const = 0;
		virtual bool Skip(Skill * skill) const = 0;
#endif
	};

	typedef unsigned int ID;

	enum { MIN_LEVEL = 1, MAX_LEVEL = 10 };

public:

	ID				id;					
	int				cls;				
	const wchar_t*	name;				
	const char*		nativename;				
	const char*		icon;					
	int				max_level;			
	char            type;               

	// execute condition
	bool			allow_ride;			
	short           attr;               
	int             rank;               
	int             eventflag;
	char            is_senior;
	char			is_inherent;		
	char			is_movingcast;		
	int             npcdelay;
	int             showorder;
	char            allow_forms;

	int             apcost;
	int             apgain;
	char            doenchant;
	char            dobless;
	int             arrowcost;

	typedef std::vector<State*>	StateVector;
	StateVector	statestub;	

	std::vector<std::pair<unsigned int, int> > pre_skills;

	// execute condition
	bool            allow_land;             
	bool            allow_air;              
	bool            allow_water;            
	bool			notuse_in_combat;		
	int             restrict_corpse;        
	bool            restrict_change;        
	bool            restrict_attach;        
	bool            auto_attack;            
	char            time_type;              
	char            long_range;
	char			posdouble;				
	int				clslimit;				

	int				commoncooldown;			
	int				commoncooldowntime;		

	int				itemcost;		
	int				itemcostcount; // Quantidade de itens usados
	
	int				combosk_preskill;
	int				combosk_interval;
	int				combosk_nobreak;
	const char*		effect;
	const char*		aerial_effect;
	Range			range;
	bool			has_stateattack;
	char			runes_attr; // Maestria (0- Sem Maestria, 1-Marcial, 2-Drag�o, 3-Tigre, 4-Comum)

	const char*		gfxfilename; // efeito de habilidade gfx adicional
	const char*		gfxhangpoint; // gancho (onde voc� prende a habilidade)
	
	char			gfxtarget;  // ?? 
	int				feature;  // Melhoria no Alcance (Glyph)
	int				extra_effect; // 
	bool			immune_casting; // Imunidade Monkey //int

	std::vector<int> restrict_weapons;

	int				stubs_num; //n�mero do cont�iner de armazenamento
	int				reserve1; //� usado apenas na habilidade 2578
	
protected:
	SkillStub(ID i) : id(i), is_senior(0),is_inherent(false),is_movingcast(false),notuse_in_combat(false),posdouble(0),clslimit(0),itemcost(0),itemcostcount(0),combosk_preskill(0),combosk_interval(0),combosk_nobreak(0),runes_attr(0),immune_casting(false),feature(0)
	{ 
		if(!GetStub(id)) 
		{
			GetMap().insert(std::make_pair(id, this));
		}
	}
	virtual ~SkillStub()	{ Clear(); }
	virtual void Clear();

	typedef std::unordered_map<ID, const SkillStub*> Map;
	static Map& GetMap() { static Map map; return map; }

	typedef std::unordered_map<ID, int> ComboMap; 
	static ComboMap& GetComboPreskill() { static ComboMap cmap; return cmap; }
	
	typedef std::set<ID> Set;
	static Set& GetClsInherent(int cls) { static Set clsinhmap[USER_CLASS_COUNT+1]; return clsinhmap[cls < USER_CLASS_COUNT ? cls : USER_CLASS_COUNT ];}
	
	bool LearnCondition(Skill * skill ) const;

	bool ReqSecLevel(Skill * skill ) const;
	bool ReqPreSKill(Skill * skill ) const;

public:

	static const SkillStub *GetStub(ID i)
	{
		Map::const_iterator it = GetMap().find(i);
		return it == GetMap().end() ? NULL : (*it).second;
	}

	static bool IsClsInherent(ID i,int cls)
	{
		return GetClsInherent(cls).find(i) != GetClsInherent(cls).end();
	}

	static bool IsComboPreskill(ID i)
	{
		return GetComboPreskill().find(i) != GetComboPreskill().end();
	}

	static int GetComboExpire(ID i)
	{
		ComboMap::iterator iter = GetComboPreskill().find(i);
		return iter != GetComboPreskill().end() ? iter->second : 0;
	}

	static void Initialize()
	{
		Map::const_iterator iter = GetMap().begin();
		Map::const_iterator iend = GetMap().end();

		for(;iter != iend; ++iter)
		{
			const SkillStub* sk = iter->second;
			if(sk->is_inherent)	GetClsInherent(sk->cls).insert(sk->id); 
			if( sk->combosk_preskill &&	GetComboExpire(sk->combosk_preskill) < sk->combosk_interval)
				GetComboPreskill()[sk->combosk_preskill] = sk->combosk_interval;
		}
	}

	unsigned int GetStateSize( ) const { return statestub.size(); }
	const State * GetState( int index ) const { 
		ASSERT((unsigned int)index<statestub.size());
		return statestub[index]; 
	}

	ID GetId() const { return id; }
	int GetCls() const { return cls; }
	const wchar_t * GetName() const { return name; }
	const char * GetIcon() const { return icon; }
	int GetMaxLevel() const { return max_level; }
	char GetType() const { return type; }
	char GetAttr() const { return attr; }
	bool IsAttack() const { return  TYPE_ATTACK == type; }
	bool IsBless() const { return TYPE_BLESS == type; }
	bool IsCurse() const { return TYPE_CURSE == type; }
	bool IsRune() const { return TYPE_RUNE == type; }
	bool IsPassive() const { return TYPE_PASSIVE == type; }
	bool IsEnabled() const { return TYPE_ENABLED == type; }
	bool IsInstant() const { return time_type==1; }
	bool IsInstantRun() const { return time_type==4; }
	bool GetAllowLand() const { return allow_land; }
	bool GetAllowAir() const { return allow_air; }
	bool GetAllowWater() const { return allow_water; }
	bool GetAllowRide() const { return allow_ride; }
	bool GetNotuseInCombat() const { return notuse_in_combat; }
	char GetRestrictCorpse() const { return restrict_corpse; }
	bool DoEnchant() const { return doenchant; }
	bool DoBless() const { return dobless; }
	int GetEventFlag() const { return eventflag; }
	const Range & GetRange() const { return range; }
	int GetItemCost(){ return itemcost; }
	unsigned int GetItemCostCount(){ return itemcostcount; }
	bool IsComboSkill() const { return combosk_preskill > 0; }
	int  GetComboPreSkill() const { return combosk_preskill; }
	int  GetComboPreInterval() const { return (int)(0.001*combosk_interval); }
	int  GetComboBreakType() const { return combosk_nobreak;}
	bool IsMovingSkill() const { return is_movingcast; }
	bool IsImmineCasting() const { return immune_casting; }
	int GetGlyphFeature() const { return feature; }

	bool Learn(Skill * skill) const;
	bool Push(Skill * skill) const;
	int Condition(Skill *skill) const;
	int ElfCondition(Skill *skill) const;

#ifdef INTEPRETED_EXPR 
	static void Store( std::string filename );
	static bool Load( std::string filename );
	virtual int GetExecutetime(Skill *skill) const;
	virtual int GetCoolingtime(Skill *skill) const;
	virtual int GetEnmity(Skill* skill) const;
	virtual int GetRequiredSp(Skill* skill) const;
	virtual int GetRequiredLevel(Skill* skill) const;
	virtual int GetRequiredItem(Skill* skill) const;
	virtual int GetRequiredMoney(Skill* skill) const;
	virtual int GetMaxAbility(Skill* skill) const;
	virtual int GetRequiredRealmLevel(Skill* skill) const;
	virtual bool StateAttack(Skill * skill) const;
	virtual bool TakeEffect(Skill *skill) const;
	virtual float GetRadius(Skill *skill) const;
	virtual float GetAttackdistance(Skill *skill) const;
	virtual float GetAngle(Skill *skill) const;
	virtual float GetPraydistance(Skill *skill) const;
	virtual float GetEffectdistance(Skill *skill) const;
	virtual int GetShoworder() const { return showorder; }
	virtual int GetAttackspeed(Skill* skill) const { return 1; }
	virtual float GetMpcost(Skill* skill) const { return 0; }
	virtual bool CheckHpCondition(int hp, int max_hp) const { return true; }
	virtual bool CheckComboSkExtraCondition(Skill* skill) const { return true; }
	virtual void ComboSkEndAction(Skill* skill) const {}
	
	virtual int GetChargeMerc (Skill * skill) const { return 0; }
	virtual int GetCoolDownLimit (Skill * skill) const { return 0; }
	virtual int GetCostShieldEnergy (Skill * skill) const { return 0; }
#else
	virtual int GetExecutetime(Skill *skill) const = 0;
	virtual int GetCoolingtime(Skill *skill) const = 0;
	virtual int GetEnmity(Skill* skill) const = 0;
	virtual int GetRequiredSp(Skill* skill) const { return 0; }
	virtual int GetRequiredLevel(Skill* skill) const { return 0; }
	virtual int GetRequiredItem(Skill* skill) const { return 0; }
	virtual int GetRequiredMoney(Skill* skill) const { return 0; }
	virtual int GetMaxAbility(Skill* skill) const { return 0; }
	virtual int GetRequiredRealmLevel(Skill* skill) const { return 0; }
	virtual bool StateAttack(Skill * skill) const { return 0; }
	virtual bool BlessMe(Skill * skill) const { return 0; }
	virtual float GetRadius(Skill *skill) const = 0;
	virtual float GetAttackdistance(Skill *skill) const = 0;
	virtual float GetAngle(Skill *skill) const = 0;
	virtual float GetPraydistance(Skill *skill) const = 0;
	virtual float GetEffectdistance(Skill *skill) const = 0;
	virtual float GetTalent0 (PlayerWrapper * player) const { return 0; }
	virtual float GetTalent1 (PlayerWrapper * player) const { return 0; }
	virtual float GetTalent2 (PlayerWrapper * player) const { return 0; }
	virtual float GetTalent3 (PlayerWrapper * player) const { return 0; }
	virtual float GetTalent4 (PlayerWrapper * player) const { return 0; }

	virtual int GetShoworder() const { return showorder; }
	virtual int GetAttackspeed(Skill* skill) const { return 1; }
	virtual float GetMpcost(Skill* skill) const { return 0; }
	virtual bool TakeEffect(Skill *skill) const { return true; }
	virtual float GetHitrate(Skill *skill) const { return 1.0; }
	virtual bool CheckHpCondition(int hp, int max_hp) const { return true; }
	virtual bool CheckComboSkExtraCondition(Skill* skill) const { return true; }
	virtual void ComboSkEndAction(Skill* skill) const {}
	
	virtual int GetChargeMerc (Skill * skill) const { return 0; }
	virtual int GetCoolDownLimit (Skill * skill) const { return 0; }
	virtual int GetCostShieldEnergy (Skill * skill) const { return 0; }
#endif

};

class Skill
{
public:
	typedef unsigned int ID;

protected:
	ID					id;
	int                 level;

	const SkillStub     * stub;
	PlayerWrapper       * player;
	TargetWrapper       * target;
	SKILL::Data  * pdata;
	const enchant_msg   * enchantmsg;  
	const attack_msg    * attackmsg;  

	XID                 performerid;
	int                 performerlevel;
	A3DVECTOR           performerpos;

	float	ratio; 
	float	plus;          
	int	damage;
	int	magic;
	int	degree;
	int	magicdamage[5];	// 0 Gold 1 Wood 2 Water 3 Fire 4 Earth
	int	t0;
	int	t1;
	int	t2;
	int t3;
	int t4;

	typedef std::unordered_map<ID, const Skill*> Map;
	static Map& GetMap() { static Map map; return map; }

	friend class SkillStub;
public:
	Skill(ID i=0) : id(i), level(0), stub(SkillStub::GetStub(i)), player(NULL), pdata(NULL), enchantmsg(NULL), attackmsg(NULL), performerid(0,0), 
			ratio(0), plus(0), damage(0), t0(0), t1(0), t2(0), t3(0), t4(0)
	{
		if(!GetStub(id)) 
			GetMap().insert(std::make_pair(id, this));	
		memset(magicdamage,0,sizeof(magicdamage));
		magic = 0;
		degree = 0;
	}

	Skill(const Skill &r) : id(r.id), level(r.level), stub(r.stub), player(r.player),
				pdata(r.pdata), enchantmsg(r.enchantmsg), attackmsg(r.attackmsg), 
				performerid(r.performerid),ratio(r.ratio), plus(r.plus), damage(r.damage)
	{ 
		memcpy(magicdamage,r.magicdamage,sizeof(magicdamage));
		magic = r.magic;
		degree = r.degree;
	}

	virtual ~Skill()		{ Clear(); }
	virtual void Clear()	{ }

	static	ID NextSkill(ID start)
	{
		static Map::iterator it = GetMap().begin();
		if(start==0)
			it = GetMap().begin();
		else 
			it++;
		if(it==GetMap().end())
			return 0;
		return (*it).second->GetId();
	}

public:
	static const Skill *GetStub(ID i)
	{
		Map::const_iterator it = GetMap().find(i);
		return it == GetMap().end() ? NULL : (*it).second;
	}

	virtual Skill * Clone() const { return new Skill(*this); }

	static Skill * Create(ID i)
	{
		//return LuaSkill::GetInstance()->Create(i);
		const Skill * sk = GetStub(i);
		return sk?sk->Clone():NULL;
	}

	void SetId(ID i) { id = i; }
	ID GetId() const { return id; }
	const std::vector<std::pair<ID,int> > & GetJunior() const { return stub->pre_skills; }

	void SetLevel(int l) {  level = l;}
	int GetLevel() const { return level; }

	void Destroy() { delete this; }

	void SetMessage(const enchant_msg* msg) 
	{ 
		performerlevel = msg->ainfo.level;
		magic = msg->skill_reserved1;
		enchantmsg = msg; 
	}
	void SetMessage(const attack_msg* msg) 
	{ 
		damage = msg->physic_damage;
		memcpy( magicdamage, msg->magic_damage, sizeof(magicdamage));
		performerlevel = msg->ainfo.level;
		attackmsg = msg; 
	}

	const SkillStub * GetStub() const { return stub; }

	unsigned int GetStateSize( ) const { return stub->GetStateSize(); }
	bool IsStateEnd() { return GetNextindex() == -1;  }

	void SetStateindex(int s);
	int GetStateindex();

	void SetNextindex(int s);
	int GetNextindex();

	void SetData(SKILL::Data* data) { pdata = data; }
	const SKILL::Data& GetData() { return *pdata; }


	bool Learn()              	{ return stub->Learn(this); }
	bool Push()          		{ return stub->Push(this); }

	bool StateAttack()        { return stub->StateAttack(this); }
	bool BlessMe()            { return stub->BlessMe(this); }
	//bool StateAttack()        	{ double res = LuaSkill::GetInstance()->StateAttack(this, id) ; res != -0 ? res = true : res = stub->StateAttack(this); return res; }
	//bool BlessMe()            	{ double res = LuaSkill::GetInstance()->BlessMe(this, id) 	; res != -0 ? res = true : res = stub->BlessMe(this); 	return res; }
	bool IsInstant()          	{ return stub->time_type==1; }
	bool IsDurative()         	{ return stub->time_type==2; }
	bool IsWarmup()           	{ return stub->time_type==3; }
	bool IsInstantRun()         { return stub->time_type==4; }
	bool IsPassive() 			{ return stub->type==TYPE_PASSIVE; }
	bool IsRune()             	{ return stub->type==TYPE_RUNE; }
	bool IsProduceSkill()     	{ return stub->type==TYPE_PRODUCE; }
	int Condition()           	{ return stub->Condition(this); }
	int ElfCondition()        	{ return stub->ElfCondition(this); }
	//int GetCoolingtime()      	{ double res = LuaSkill::GetInstance()->GetCoolingtime(this, id) 			; res != -0 ? res = 0.001 * res : res = 0.001 * stub->GetCoolingtime(this) ; return (int) res; }	
	//int GetExecutetime()      	{ double res = LuaSkill::GetInstance()->GetExecutetime(this, id) 			; res != -0 ? res : res = stub->GetExecutetime(this); 			return (int) res; }
	int GetExecutetime()      	{ return stub->GetExecutetime(this); }
	int GetCoolingtime()      	{ return (int)(0.001*stub->GetCoolingtime(this)); }
	int GetEnmity()           	{ return stub->GetEnmity(this); }
	int GetRequiredSp()       	{ return stub->GetRequiredSp(this); }
	int GetRequiredLevel()    	{ return stub->GetRequiredLevel(this); }
	int GetRequiredItem()     	{ return stub->GetRequiredItem(this); }
	int GetRequiredMoney()    	{ return stub->GetRequiredMoney(this); }
	int GetMaxability()       	{ return stub->GetMaxAbility(this); }
	int GetRequiredRealmLevel()	{ return stub->GetRequiredRealmLevel(this); }
	int GetAttackspeed()      	{ return stub->GetAttackspeed(this); }
	int GetMaxLevel()         	{ return stub->max_level; }
	int GetAbility()          	{ return 0; }
	int GetApcost()           	
	{
		// Custo de Chi dobrado
		int newapcost = 0;
		if (GetPlayer()->CheckBuffState(543))
		{
			newapcost = stub->apcost * 1.5;
		}
		else
		{
			// Ap Cost Runa
			int newap = GetPlayer()->GetGlyphApCostInc();
			if (newap > 0)
			{
				newapcost = (stub->apcost - newap);
			}
			else
			{
				// Ap Cost Normal
				newapcost = stub->apcost;
			}
		}
		return newapcost;
	}
	int GetApgain()           	{ return stub->apgain; }
	float GetRadius()         	{ return stub->GetRadius(this); }
	float GetAttackdistance() 	{ return stub->GetAttackdistance(this); }
	float GetAngle()          	{ return stub->GetAngle(this); }
	float GetPraydistance()   	{ return stub->GetPraydistance(this); }
	float GetEffectdistance() 	{ return stub->GetEffectdistance(this); }
	float GetHitrate()        	{ return stub->GetHitrate(this); }
	
	bool  DoBless() const     	{ return stub->dobless; }
	bool  DoEnchant() const   	{ return stub->doenchant; }
	bool  IsSenior() const    	{ return stub->is_senior; }
	char  GetRestrictCorpse() const { return stub->restrict_corpse; }
	bool  IsLongRange() const { return stub->long_range==1; }
	int   RangeAdjust() const { return stub->long_range; }
	int   PositionAdjust() const { return stub->posdouble; }
	const Range & GetRange()  { return stub->GetRange(); }
	bool  IsCastSelf() const { return stub->range.type==5; }
	bool  IsElfSkill() const { return stub->cls==258; }
	bool CanAttack(bool usearrow=true);
	bool CanAttackCondition(bool usearrow=true);
	char GetType() const { return stub->type; }
	int   GetCommonCoolDown() { return stub->commoncooldown; }
	int   GetCommonCoolDownTime() { return stub->commoncooldowntime; }
	int GetItemCost(){ return stub->itemcost; }
	unsigned int GetItemCostCount(){ return stub->itemcostcount; }
	bool IsComboSkill() { return stub->IsComboSkill(); }
	int  GetComboPreSkill() { return stub->GetComboPreSkill(); }
	int  GetComboPreInterval() { return stub->GetComboPreInterval(); }
	int  GetComboBreakType() { return stub->GetComboBreakType(); }
	int  GetComboExpire() { return SkillStub::GetComboExpire(id); }
	bool IsMovingSkill() { return stub->IsMovingSkill(); }
	bool IsImmineCasting() { return stub->IsImmineCasting(); }

	// GLyph Skills 

	int GetGlyphType() { return stub->runes_attr; }
	int GetChargeMerc () { return stub->GetChargeMerc(this); }
	int GetCoolDownLimit () { return stub->GetCoolDownLimit(this); }
	int GetCostShieldEnergy () { return stub->GetCostShieldEnergy(this); }
	//void SkillEdit(int type, double value) { SkillEdit(type, value); }
	int GetGlyphFeature(){ return stub->feature; }

public:
	void SetPlayer(PlayerWrapper * p) { player = p; }
	void SetVictim(PlayerWrapper * p) { player = p; }
	void SetTarget(TargetWrapper * t) { target = t; }
	PlayerWrapper * GetPlayer() const { return player; }
	PlayerWrapper * GetVictim() const { return player; }
	TargetWrapper * GetTarget() const { return target; }

	void SetPerformerid(const XID& p) { performerid = p; }
	const XID& GetPerformerid() const { return performerid; }
	void SetPerformerpos(const A3DVECTOR& p) { performerpos = p; }
	const A3DVECTOR& GetPerformerpos() const { return performerpos; }

	char GetForceattack();

public:

	void SetPlus(float p)  { plus = p; }
	void SetRatio(float r) { ratio = r*100; }

	int GetPlus()        { return (int)plus; }
	int GetRatio()       { return (int)ratio; }

	int GetAttack();    
	int GetMagicattack();
	
	int GetCritAttack();    
	int GetCritMagicattack();   

	void SetDamage(int attack)      { damage = attack; }
	void SetGolddamage(int attack);
	void SetWooddamage(int attack);
	void SetWaterdamage(int attack);
	void SetFiredamage(int attack);
	void SetEarthdamage(int attack);
	void SetMagicDamage(int attack) { magic = attack; }
	void SetDegree(int d) { degree = d; }

	int GetDamage()      const { return damage; }
	int GetGolddamage()  const { return magicdamage[0]; }
	int GetWooddamage()  const { return magicdamage[1]; }
	int GetWaterdamage() const { return magicdamage[2]; }
	int GetFiredamage()  const { return magicdamage[3]; }
	int GetEarthdamage() const { return magicdamage[4]; }
	int GetMagicdamage() const { return magic; }
	int GetDegree() const { return degree; }

	const int* GetFivedamage() const{ return magicdamage; }

	const attacker_info_t& GetPerformerinfo() const 
	{ 
		if(attackmsg)
			return attackmsg->ainfo;
		ASSERT(enchantmsg);
		return enchantmsg->ainfo;
	}
	int GetPerformerCurWeapon() const
	{
		if(attackmsg)
			return attackmsg->weapon_class;
		if(enchantmsg)
			return enchantmsg->weapon_class;
		return 0;
	}

	int GetPerformerCurAttack() const
	{
		if(attackmsg)
			return attackmsg->physic_damage;
		//if(enchantmsg)
		//	return enchantmsg->physic_damage;
		return 0;
	}

	int GetPerformerCurAttackMagic() const
	{
		if(attackmsg)
			return attackmsg->magic_damage[0] + attackmsg->magic_damage[1] + attackmsg->magic_damage[2] + attackmsg->magic_damage[3] + attackmsg->magic_damage[4];
		//if(enchantmsg)
		//	return enchantmsg->physic_damage;
		return 0;
	}

	int GetPerformerCurGetAttack() const
	{
		if(attackmsg)
			return attackmsg->get_attack;
		if(enchantmsg)
			return enchantmsg->get_attack;
		return 0;
	}

	int GetPerformerCurGetMagicAttack() const
	{
		if(attackmsg)
			return attackmsg->get_magic_attack;
		if(enchantmsg)
			return enchantmsg->get_magic_attack;
		return 0;
	}

	int GetPerformerCurGetEquipAttack() const
	{
		if(attackmsg)
			return attackmsg->get_equip_attack;
		if(enchantmsg)
			return enchantmsg->get_equip_attack;
		return 0;
	}

	int GetPerformerCurGetEquipMagicAttack() const
	{
		if(attackmsg)
			return attackmsg->get_equip_magic_attack;
		if(enchantmsg)
			return enchantmsg->get_equip_magic_attack;
		return 0;
	}

	int GetPerformerCurGetWeaponAttack() const
	{
		if(attackmsg)
			return attackmsg->weapon_damage;
		if(enchantmsg)
			return enchantmsg->weapon_damage;
		return 0;
	}

	int GetPerformerCurGetWeaponMagicAttack() const
	{
		if(attackmsg)
			return attackmsg->weapon_magic_damage;
		if(enchantmsg)
			return enchantmsg->weapon_magic_damage;
		return 0;
	}

	int GetPerformerCurFakeWeapon() const
	{
		if(attackmsg)
			return attackmsg->fake_weapon_class;
		if(enchantmsg)
			return enchantmsg->fake_weapon_class;
		return 0;
	}

	int GetPerformerCurRealWeapon() const
	{
		if(attackmsg)
			return attackmsg->real_weapon_class;
		if(enchantmsg)
			return enchantmsg->real_weapon_class;
		return 0;
	}

	int GetPerformerCurWeaponDamage() const
	{
		if(enchantmsg)
			return enchantmsg->skill_reserved1;
		return 0;
	}
	char GetAttackerMode() const
	{
		if(attackmsg)
			return attackmsg->attacker_mode;
		if(enchantmsg)
			return enchantmsg->attacker_mode;
		return 0;
	}
	int GetAttackerDegree() const
	{
		if(attackmsg)
			return attackmsg->attack_degree;
		if(enchantmsg)
			return enchantmsg->attack_degree;
		return 0;
	}
	int GetAttackState() const
	{
		if(attackmsg)
			return attackmsg->_attack_state;
		if(enchantmsg)
			return enchantmsg->_attack_state;
		return 0;
	}
	int GetPenetration() const
	{
		if(attackmsg)
			return attackmsg->penetration;
		if(enchantmsg)
			return enchantmsg->penetration;
		return 0;
	}
	int GetVigour() const
	{
		if(attackmsg)
			return attackmsg->vigour;
		if(enchantmsg)
			return enchantmsg->vigour;
		return 0;
	}

	void SetPlayerlevel(int n ) { }
	int  GetPlayerlevel() const { return performerlevel; }

	void SetCharging(int w);
	unsigned int GetCharging();

	void SetSection(int s);
	unsigned int GetSection();
	
	void SetLvalue(int v);
	int  GetLvalue();

	void SetRand(int) { }
	int  GetRand() { return rand()%100; }
	int GetT0() { return t0; }
	int GetT1() { return t1; }
	int GetT2() { return t2; }
	int GetT3() { return t3; }
	int GetT4() { return t4; }

	void  SetTalent(int index, int value)
	{
		switch(index)
		{
			case 0:
				t0 = value;
				break;
			case 1:
				t1 = value;
				break;
			case 2:
				t2 = value;
				break;
			case 3:
				t3 = value;
				break;
			case 4:
				t4 = value;
				break;		
		}
	}
	void PrepareTalent(PlayerWrapper* player);

public:
	int FirstRun( int & next_interval, int prayspeed );

	int Run( int & next_interval );

	int InstantRun()
	{
		//r_Calculate(0);
		const SkillStub::State * state = stub->GetState(0);
		if(state)
			Run(state);
		return 1;
	}

	bool Interrupt()
	{
		if(InvalidState())
			return false;
		//return r_Interrupt(GetStateindex());
		const SkillStub::State * state = stub->GetState(GetStateindex());
		return state->Interrupt(this);
	}

	bool Cancel()
	{
		if(InvalidState())
			return false;
		//return r_Cancel(GetStateindex());
		const SkillStub::State * state = stub->GetState(GetStateindex());
		return state->Cancel(this);
	}

	void NpcFirstRun()
	{
		//r_Calculate(0);
		const SkillStub::State * state = stub->GetState(0);
		Run( state );
	}

	void NpcRun()
	{
		//r_Calculate(1);
		const SkillStub::State * state = stub->GetState(1);
		Run( state );
	}

	bool TakeEffect(PlayerWrapper* player, int arg);
	bool UndoEffect(PlayerWrapper* player, int arg);

	int GetEventFlag()
	{
		return GetStub()->GetEventFlag();
	}

	void ComboSkEndAction()
	{
		stub->ComboSkEndAction(this);
	}
protected:
	bool Run(const SkillStub::State *state)
	{
		state->Calculate( this );
		return true;
	}

	bool InvalidState();
	int NextState( int index );

};
typedef Skill* SkillPTR;
class SkillKeeper
{
	Skill* pointer;
	SkillKeeper()
	{
		pointer = NULL;
	}
public:
	SkillKeeper(Skill* p) : pointer(p)
	{
	}
	~SkillKeeper()
	{
		if(pointer)
			pointer->Destroy();
	}
	Skill* operator -> ()
	{
		return pointer;
	}
	operator SkillPTR ()
	{
		return pointer;
	}
	operator bool ()
	{
		return pointer;
	}
	SkillKeeper& operator = (Skill* p)
	{
		if(pointer)
			pointer->Destroy();
		pointer = p;
		return *this;
	}
};

};

#pragma pack()

#endif

