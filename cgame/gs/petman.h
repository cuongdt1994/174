#ifndef __ONLINE_GAME_GS_PET_MANAGER_H__
#define __ONLINE_GAME_GS_PET_MANAGER_H__

#include <common/types.h>
#include <amemory.h>
#include <common/base_wrapper.h>

class world;
class pet_gen_pos
{
public:
	static bool FindGroundPos(A3DVECTOR & pos,float dis,A3DVECTOR & offset, world * plane);
	static bool FindWaterPos(A3DVECTOR & pos,float dis,A3DVECTOR & offset, world * plane);
	static bool FindAirPos(A3DVECTOR & pos,float dis,A3DVECTOR & offset, world * plane);
	static bool IsValidInhabit(char leader_layer, int inhabit_type);
	static int FindValidPos(A3DVECTOR & pos, char & inhabit_mode, char leader_layer, int inhabit_type, world * plane, float dis=0.f, A3DVECTOR offset=A3DVECTOR(0.f,0.f,0.f));
};

struct extend_prop;
struct msg_pet_hp_notify;
#pragma pack(1)
struct pet_data
{
	enum
	{
		MAX_PET_SKILL_COUNT = 8,
		MAX_NAME_LEN = 16,
	};

	int honor_point;	//�øж�	�ᾭ���仯
	int hunger_gauge;	//������	�ᾭ���仯 // short
	int feed_period;	//�ϴ�ι�������ڵļ�ʱʱ��
	int pet_tid;		//�����ģ��ID
	int pet_vis_tid;	//����Ŀɼ�ID�����Ϊ0�����ʾ������ɼ�ID��
	int pet_egg_tid;	//���ﵰ��ID
	int pet_class;		//�������� ս�裬��裬���ͳ�
	float hp_factor;	//Ѫ��������������ջ�ʱʹ�ã� 0��Ϊ����
	short level;		//���Ｖ��
	unsigned short color;   //������ɫ�����λΪ1��ʾ��Ч��Ŀǰ���������Ч
	int exp;		//���ﵱǰ����
	int skill_point;	//ʣ�༼�ܵ�
	char is_bind;		//�Ƿ��ڰ�״̬,������һ��mask 0x01�� 0x02��Ѱ��������
	char unused;		
	unsigned short name_len;//���ֳ���
	char name[MAX_NAME_LEN];//��������
	struct __skills
	{
		int skill;
		int level;
	} skills[MAX_PET_SKILL_COUNT];
	struct __evo_prop
	{
		int r_attack;
		int r_defense;
		int r_hp;
		int r_atk_lvl;
		int r_def_lvl;
		int nature;
		int r_pen_lvl_f;
		int r_pen_lvl_m;
		int r_chi_point;
	} evo_prop;

	// Novos
	int reserved[7];
	
	enum
	{
		PET_CLASS_MOUNT,		//���
		PET_CLASS_COMBAT,		//ս������
		PET_CLASS_FOLLOW,		//�������
		PET_CLASS_SUMMON,		//�ٻ���
		PET_CLASS_PLANT,		//ֲ��
		PET_CLASS_EVOLUTION,	//��������
		PET_CLASS_MAX,
	};

	enum
	{
		HUNGER_LEVEL_0,			//��ʳ
		HUNGER_LEVEL_1,			//����
		HUNGER_LEVEL_2,			//���̶�һ�� 
		HUNGER_LEVEL_3,	
		HUNGER_LEVEL_4,			//���̶ȶ���
		HUNGER_LEVEL_5,
		HUNGER_LEVEL_6,
		HUNGER_LEVEL_7,			//���̶�����
		HUNGER_LEVEL_8,
		HUNGER_LEVEL_9,
		HUNGER_LEVEL_A,
		HUNGER_LEVEL_B,			//���̶��ļ� 
		HUNGER_LEVEL_COUNT
	};

	enum 
	{
		FEED_TIME_UNIT = 300,		//300	ι����ʱ�䵥λ
	};

	enum
	{
		HONER_LEVEL_0,			//Ұ����ѱ	0-50
		HONER_LEVEL_1,			//�����޳�	51-150
		HONER_LEVEL_2,			//��������	151-500
		HONER_LEVEL_3,			//���Ĺ���	501-999
		HONOR_LEVEL_COUNT,

		HONOR_POINT_MAX	= 999
	};

};

#pragma pack()

class gplayer_imp;
class pet_manager
{
public:
	enum
	{
		MAX_PET_CAPACITY = 40,
		NOTIFY_MASTER_TIME = 16,
		MAX_SUMMON_CAPACITY = 1,
		SUMMON_SLOT	= 20,
	};

	enum RECALL_REASON
	{
		NONE = 0,
		PET_DEATH,			//��������
		PET_LIFE_EXHAUST,	//����������
		PET_SACRIFICE,		//��������
	};
	
protected:
	pet_data * 	_pet_list[MAX_PET_CAPACITY + MAX_SUMMON_CAPACITY];		//�����б�+�ٻ����б�
	unsigned int		_active_pet_slot;			//����ж��ٸ��������ܼ���
	int 		_cur_active_pet;			//��ǰ�����ĳ�������

public:
	XID		_cur_pet_id;				//��ǰ����ID
	unsigned char	_pet_summon_stamp;			//�����ٻ���������������ͬ���ٻ�
	bool		_is_on_underwater;			//�Ƿ���ˮ�½������⴦��
	char		_cur_pet_aggro_state;			//��ǰ����ĳ�޷�ʽ
	char		_cur_pet_stay_state;			//��ǰ����ĸ��淽ʽ
	char		_cur_pet_combat_state;			//��ǰ����Ĺ���״̬
	bool		_need_feed;				//��Ҫιʳ����
	short		_cur_pet_counter;			//��ǰ�����ٻ���ʱ������ͳ�ƾ���ֵ
	short		_cur_pet_notify_counter;		//��ǰ���͸�����������ݵ���Ϣ��ʱ
	unsigned int		_cur_pet_state;				//�������֪ͨ���Լ�����Ϣ����˱�־��λ
	int		_cur_pet_hp;				//��ǰ�����Ѫֵ 
	int		_cur_pet_inhabit;			//��ǰ�������Ϣ��
	float	_cur_pet_mp_factor;
	int		_cur_pet_mp;
	int 	_cur_pet_life;				//��ǰ��������,0Ϊ����
	int		_summon_skill_level;		//�ٻ�����ǰ�������õļ��ܵȼ�

	int		_codex_active_pet_slot;
protected:
	void ClearCurPet(gplayer_imp * pImp)
	{
		_cur_active_pet = -1;
		_cur_pet_id = XID(-1,-1);
		//do others ....
	}
	void HandleFeedTimeTick(gplayer_imp * pImp, pet_data * pData);
	void ModifyHonor(pet_data * pData, int offset);
	void ModifyHungerGauge(pet_data * pData, int offset);
public:
	pet_manager();
	~pet_manager();

	void ActiveNoFeed(bool nofeed)
	{
		_need_feed = !nofeed;
	}

	void SetAvailPetSlot(unsigned int slot)
	{
		if(slot > _active_pet_slot && slot <= MAX_PET_CAPACITY/2)
		{
			_active_pet_slot = slot;
		}
	}

	unsigned int GetAvailPetSlot()
	{
		return _active_pet_slot;
	}

	bool IsPetActive() 
	{
		return _cur_active_pet >= 0;
	}

	int GetCurActivePet()
	{
		return _cur_active_pet;
	}

	void SetTestUnderWater(bool val)
	{
		_is_on_underwater = val;
	}

	const XID & GetCurPet()
	{
		return _cur_pet_id;
	}

	pet_data * GetPetData(unsigned int index)
	{
		if(index < MAX_PET_CAPACITY + MAX_SUMMON_CAPACITY)
		{
			return _pet_list[index];
		}
		else
		{
			return NULL;
		}
	}

	unsigned int GetPetsCount()
	{
		unsigned int cnt = 0;
		for(unsigned int i=0; i<MAX_PET_CAPACITY; i++)
			if(_pet_list[i]) ++cnt;
		return cnt;
	}

	bool DBSetPetData(unsigned int index, const void * data, unsigned int size, bool codex = false);
	void ClientGetPetRoom(gplayer_imp * pImp);

protected:
	bool DoActiveMount(gplayer_imp * pImp,pet_data * pData);
	void TestUnderWater(gplayer_imp * pImp, float offset);
	void DoHeartbeat(gplayer_imp * pImp);

public:
	void OnUnderWater(gplayer_imp * pImp, float offset)
	{
		if(!_is_on_underwater) return;
		TestUnderWater(pImp,offset);
	}	
	void Save(archive & ar);
	void Load(archive & ar);
	void Swap(pet_manager & rhs);

public:
	int AddPetData(const pet_data & data, bool codex = false);
	bool BanishPet(gplayer_imp * pImp, unsigned int index);
	void FreePet(gplayer_imp * pIMp, unsigned int index);		//����У����ͷ�

	bool ActivePet(gplayer_imp * pImp, unsigned int index);
	bool RecallPet(gplayer_imp * pImp);
	bool RecallPetWithoutFree(gplayer_imp * pImp, char reason=0);	//�����ڲ�����

	bool FeedCurPet(gplayer_imp * pImp, int food_type, int honor);
	bool RelocatePos(gplayer_imp * pImp,const XID & who , int stamp,bool is_disappear);
	bool NotifyPetHP(gplayer_imp * pImp,const XID & who , int stamp,const msg_pet_hp_notify & info);
	bool PetDeath(gplayer_imp * pImp,const XID & who , int stamp);
	bool PlayerPetCtrl(gplayer_imp * pImp,int cur_target,int pet_cmd, const void * buf, unsigned int size);
	void KillMob(gplayer_imp * pImp, int mob_level);
	int ResurrectPet(gplayer_imp * pImp, unsigned int index);
	int ResurrectPet(gplayer_imp * pImp);
	void NotifyMasterInfo(gplayer_imp * pImp,bool atonce);	
	void PreSwitchServer(gplayer_imp * pImp);
	void PostSwitchServer(gplayer_imp * pImp);
	void PlayerBeAttacked(gplayer_imp * pImp, const XID & target);
	void PetSetCoolDown(gplayer_imp *pImp, const XID & who, int idx, int msec);
	void NotifyStartAttack(gplayer_imp *pImp, const XID & target, char force_attack);
	int ChangePetName(gplayer_imp * pImp, unsigned int index, const char * name, unsigned int name_len);
	int ForgetPetSkill(gplayer_imp * pImp,int skill_id);
	int LearnSkill(gplayer_imp * pImp,int skill_id, int * level_result);
	void NotifyInvisibleData(gplayer_imp *pImp);

	/*166+*/
	int GetSkillsPetCount();
	bool SkillDataInc(gplayer_imp * pImp, int index_from, int index_to);
	bool PetBuySkinInc(gplayer_imp * pImp, int index, int pos_data);
	bool PetUseSkinInc(gplayer_imp * pImp, int index, int pos_data);
	int GetSkinsPetCount(gplayer_imp * pImp);
	bool RebuildStorage(gplayer_imp * pImp);

	void RecvExp(gplayer_imp * pImp, unsigned int pet_index,int exp);	
	bool AddExp(gplayer_imp *pImp,unsigned int pet_index,int exp);
	int x_LevelUp(gplayer_imp *pImp, unsigned int index, int level_counter);
	inline void Heartbeat(gplayer_imp * pImp);
	friend class petdata_imp;

	bool ActivePet2(gplayer_imp * pImp, pet_data & data, int life, int skill_level, const XID & target, char force_attack);
	void TryFreePet(gplayer_imp * pImp);		//��ͼ�ͷ��Ѿ����ٻص��ٻ���
	bool PetSacrifice(gplayer_imp * pImp);
	bool DyePet(gplayer_imp * pImp, unsigned int index, unsigned short color);
	bool EvolutionPet(gplayer_imp *pImp,unsigned int index,int evolution_id,int pet_nature,int skill1,int level1,int skill2,int level2);
	bool RebuildInheritRatio(int pet_id,int &r_attack,int &r_defense,int &r_hp,int &r_atk_lvl,int &r_def_lvl/*, int &r_pen_lvl_f, int &r_pen_lvl_m, int &r_chi_point*/);
	void PetAcceptInheritRatioResult(gplayer_imp *pImp,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/);
	void PetAcceptNatureResult(gplayer_imp *pImp,unsigned int index,int nature,int skill1,int level1,int skill2,int level2);
	int GetNormalSkillNum(gplayer_imp *pImo,pet_data *pData);
	bool IsSkillNormal(gplayer_imp *pImp,pet_data *pData,int skill_id);
	void OnMountSpeedEnChanged(gplayer_imp *pImp);
	
	bool ActivePet3(gplayer_imp * pImp, pet_data & data, int life, int skill_level, const XID & target, char force_attack);
};

class petdata_imp
{
public:
	virtual bool DoActivePet(gplayer_imp * pImp, pet_manager * pMan,pet_data * pData, extend_prop * pProp) = 0;
	virtual bool DoRecallPet(gplayer_imp * pImp, pet_manager * pMan,pet_data * pData) = 0;
	virtual void TestUnderWater(gplayer_imp * pImp, pet_manager * pMan,pet_data * pData,float offset) = 0;
	virtual void LevelUp(gplayer_imp *pImp, pet_manager * pMan,pet_data * pData) = 0;
	virtual void Heartbeat(gplayer_imp *pImp, pet_manager * pMan,pet_data * pData) = 0;
	virtual void OnHonorModify(gplayer_imp *pImp, pet_manager * pMan,pet_data * pData, int old_honor) = 0;
	virtual bool OnPetRelocate(gplayer_imp *pImp, pet_manager * pMan,pet_data *pData,const XID &who,int stamp,bool is_disappear) = 0; 
	virtual bool OnPetNotifyHP(gplayer_imp *pImp, pet_manager * pMan,pet_data *pData,const XID &who,int stamp,const msg_pet_hp_notify & info) = 0;
	virtual bool OnPetDeath(gplayer_imp *pImp, pet_manager * pMan,pet_data *pData,const XID &who,int stamp) = 0;
	virtual bool OnPetCtrl(gplayer_imp * pImp, pet_manager * pMan,pet_data *pData,int cur_target,int pet_cmd, const void * buf, unsigned int size) = 0;
	virtual void OnKillMob(gplayer_imp * pImp, pet_manager * pMan, pet_data * pData, int mob_level) = 0;
	virtual void OnNotifyMasterInfo(gplayer_imp * pImp,pet_manager * pMan, pet_data * pData, bool at_once) = 0;
	virtual void PreSwitchServer(gplayer_imp * pImp, pet_manager * pman, pet_data * pData) = 0;
	virtual void PostSwitchServer(gplayer_imp * pImp, pet_manager * pman, pet_data * pData) = 0;
	virtual void OnMasterBeAttacked(gplayer_imp * pImp,pet_manager *pman, pet_data *pData,const XID &who) = 0; 
	virtual bool OnChangeName(gplayer_imp * pImp,pet_manager *pman, pet_data *pData,const char *name, unsigned int len) = 0; 
	virtual bool OnForgetSkill(gplayer_imp * pImp,pet_manager *pman, pet_data *pData, int skill_id) = 0;
	virtual bool OnLearnSkill(gplayer_imp * pImp,pet_manager *pMan,pet_data *pData,int skill_id, int * level_result) = 0;
	virtual bool OnDyePet(gplayer_imp * pImp,pet_manager *pMan,pet_data *pData,unsigned short color) = 0;
	virtual bool OnEvolution(gplayer_imp *pImp,pet_manager *pMan,pet_data *pData,int evolution_pet_id,int pet_nature,int skill1,int level1,int skill2,int level2) = 0;
	virtual void OnMountSpeedEnChanged(gplayer_imp *pImp,pet_manager * pMan, pet_data * pData) = 0;
protected:
	inline void ClearCurPet(gplayer_imp * pImp, pet_manager * pMan)
	{
		pMan->ClearCurPet(pImp);
	}
};

inline void pet_manager::Heartbeat(gplayer_imp * pImp)
{
	if(_cur_active_pet >= 0)
	{
		DoHeartbeat(pImp);
	}
}


//������ֲ��������,��ֲ���л���������ʧ�����Բ��豣��
struct msg_plant_pet_hp_notify;
struct plant_pet_data
{
	XID id;
	int tid;
	int plant_group;
	int group_limit;
	int life;
	float hp_factor;
	int hp;
	float mp_factor;
	int mp;
	unsigned int plant_state;
	bool is_suicide;
};

class plant_pet_manager
{
public:
	enum
	{
		PET_STATE_THRESHOLD		 = 15,	//15���ڳ���û��֪ͨ��Ϣ�����ٻس���
		NOTIFY_MASTER_TIME		 = 15,	//ÿ15��֪ͨ����master info
	};
	
	enum
	{
		PLANT_DEATH = 0,	//ֲ������
		PLANT_LIFE_EXHAUST,	//ֲ��������
		PLANT_OUT_OF_RANGE,	//ֲ�ﳬ����Χ
		PLANT_SUICIDE,		//ֲ���Ա�
		PLANT_GROUP_LIMIT,	//ֲ����������������
	};
	
	typedef abase::vector<plant_pet_data, abase::fast_alloc<> > PLANT_LIST;
	PLANT_LIST _plant_list;					//ֲ���б�
	unsigned int _plant_notify_counter;

public:
	plant_pet_manager():_plant_notify_counter(0){}

	bool ActivePlant(gplayer_imp * pImp, pet_data & data, int life, int skill_level, const XID & target, char force_attack);
	bool PlantSuicide(gplayer_imp * pImp, float distance, const XID & target, char force_attack);
	void Heartbeat(gplayer_imp * pImp);
	void NotifyMasterInfo(gplayer_imp * pImp);	
	void PreSwitchServer(gplayer_imp * pImp);
	void NotifyStartAttack(gplayer_imp *pImp, const XID & target, char force_attack);
	void PlayerBeAttacked(gplayer_imp * pImp, const XID & target);
public:
	bool PlantDeath(gplayer_imp * pImp,const XID & who , int stamp);
	bool NotifyPlantHP(gplayer_imp * pImp,const XID & who , int stamp,const msg_plant_pet_hp_notify & info);
	bool PlantDisappear(gplayer_imp * pImp,const XID & who , int stamp);

};	
#endif

