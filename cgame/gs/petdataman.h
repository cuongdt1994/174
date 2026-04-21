#ifndef __NETGAME_GS_PET_DATA_MAN_H__
#define __NETGAME_GS_PET_DATA_MAN_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>
#include "petman.h"
#include "playertemplate.h"
#include "property.h"

class itemdataman;

//��������ģ��
struct pet_data_temp
{
	int tid;
	int pet_class; //�������� ��pet_data�ж���
	//��������

	inline int CalcHP(int level) const
	{
		return (int)(hp_a * (level - hp_b*level_require + hp_c));
	}

	inline int CalcHP2(int level) const
	{
		int hp1 = (int)(hp_a * (level - hp_b*level_require + hp_c)); 
		return (int)(hp1 * mult_r_hp);
	}
	inline int CalcHPGEN(int level) const
	{
		return (int)(hp_gen_a *(level - hp_gen_b*level_require + hp_gen_c));
	}

	inline int CalcDamage(int level) const
	{
		return (int)(damage_a * (damage_b * level*level + damage_c * level + damage_d));
	}

	inline int CalcDefense(int level) const
	{
		return (int)(defense_a * (defense_b*(level - defense_c*level_require) + defense_d));

	}

	inline int CalcDefense2(int level) const
	{
		int def1 = (int)(defense_a * (defense_b*(level - defense_c*level_require) + defense_d));
		return (int)(def1 * mult_r_defense);

	}
	inline int CalcAttack(int level) const
	{
		return (int)(attack_a * (level - attack_b*level_require + attack_c));
	}

	inline int CalcAttack2(int level) const
	{
		int attack1 = (int)(attack_a * (level - attack_b*level_require + attack_c));
		return (int)(attack1 * mult_r_attack);
	}
	
	inline int CalcArmor(int level) const
	{
		return (int)(armor_a * (level - armor_b*level_require + armor_c));
	}

	inline int CalcResistance(int level) const
	{
		return (int)(resistance_a * (resistance_b*(level - resistance_c*level_require) + resistance_d));
	}
	
	inline int CalcMP(int level) const
	{
		return (int)(mp_a * (level + 5));
	}

	inline int CalcMPGEN(int level) const
	{
		return (int)(mp_gen_a *(level + 5));
	}
	
	inline int CalcAttackDegree(int level) const
	{
		return (int)(attack_degree_a * (level + 5));
	}
	inline int CalcDefendDegree(int level) const
	{
		return (int)(defend_degree_a * (level + 5));
	}

	float hp_a;
	float hp_b;
	float hp_c;			//hpϵ��  hp = hp_a * (level - hp_b*level_require + hp_c);
	
	float hp_gen_a;
	float hp_gen_b;
	float hp_gen_c;			//hpgenϵ�� hp_gen = hp_gen_a *(level - hp_gen_b*level_require + hp_gen_c);

	float damage_a;
	float damage_b;
	float damage_c;			
	float damage_d;			//������ϵ�� damage = damage_a * (damage_b * level*level + damage_c * level + damage_d);
	float speed_a;
	float speed_b;			//�ٶȺ��� speed = speed_a + speed_b*(level - 1);

	float attack_a;
	float attack_b;
	float attack_c;			//����ϵ��  attack = attack_a * (level - attack_b*level_require + attack_c);
	
	float armor_a;
	float armor_b;
	float armor_c;			//����ϵ��  armor = armor_a * (level - armor_b*level_require + armor_c);

	float defense_a;
	float defense_b;
	float defense_c;
	float defense_d;			//����ϵ��  defense = defense_a * (defense_b*(level - defense_c*level_require) + defense_d); 

	float resistance_a;
	float resistance_b;
	float resistance_c;
	float resistance_d;		//ħ��ϵ��  resistance = resistance_a * (resistance_b*(level - resistance_c*level_require) + resistance_d); 

	float mp_a;				//mpϵ��
	
	float mp_gen_a;			//mp�ظ�ϵ��
	
	float attack_degree_a;	//�����ȼ�ϵ��

	float defend_degree_a;	//�����ȼ�ϵ��

	float 	body_size;		//�����С
	float 	attack_range;		//���﹥������
	int	damage_delay;		//�����ӳ�
	int	attack_speed;		//�����ٶ�
	float	sight_range;		//��Ұ��Χ 

	unsigned int food_mask;		//ʳ������
	unsigned int inhabit_type;	//��Ϣ��
	unsigned int inhabit_mode;	//��Ϣ�� ���ڲ�ʹ��

	int immune_type; 
	int sacrifice_skill;	//����������������˿ɻ�ô���Ʒ���ܵ�Ч����

	int	max_level;		//�����Ｖ��
	int 	level_require;

	int plant_group;		//ֲ��������
	int group_limit; 		//��ǰ������������ٻ�����
	
	int evolution_id; 		//������ĳ��ﵰģ��id��Ϊ0�򲻿��Խ���
	int max_r_attack;
	int max_r_defense;
	int max_r_hp;
	int max_r_atk_lvl;
	int max_r_def_lvl;
	int min_r_hp;
	int min_r_attack;
	int min_r_defense;

	float mult_r_hp;
	float mult_r_attack;
	float mult_r_defense;

	int max_r_pen_lvl_f;
	int max_r_pen_lvl_m;
	int max_r_chi_point;
	
	int specific_skill_id;

};

class pet_dataman
{
	typedef abase::hashtab<pet_data_temp , int ,abase::_hash_function> MAP;
	MAP _pt_map;
	
	bool __InsertTemplate(const pet_data_temp & pt)
	{       
		return _pt_map.put(pt.tid, pt);
	}       

	const pet_data_temp * __GetTemplate(int tid)
	{       
		return _pt_map.nGet(tid);
	}

	static pet_dataman & __GetInstance()
	{
		static pet_dataman __Singleton;
		return __Singleton; 
	}
	pet_dataman():_pt_map(1024) {}
	unsigned int __GetLvlupExp(unsigned int cur_lvl) const;
public:
	
	static bool Insert(const pet_data_temp & pt)
	{
		bool rst = __GetInstance().__InsertTemplate(pt);
		ASSERT(rst);
		return rst;
	}

	static const pet_data_temp * Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman & dataman);

	static bool CalcMountParam(int tid, int lvl , float & speedup)
	{
		const pet_data_temp * pTmp = Get(tid);
		if(!pTmp) return false;
		speedup = pTmp->speed_a + pTmp->speed_b * (lvl - 1);
		//speedup = pTmp->speed_b + pTmp->speed_a * lvl;

		return true;
	}
	static bool GenerateBaseProp(int tid, int level, extend_prop & prop);
	static bool GenerateBaseProp2(int tid, int level, int skill_level, extend_prop & prop, int& attack_degree, int& defend_degree);
	static bool GenerateBaseProp3(int tid, int level, int skill_level, extend_prop & prop, int& attack_degree, int& defend_degree);

	static unsigned int GetLvlupExp(unsigned int cur_lvl)
	{
		return player_template::GetPetLvlupExp(cur_lvl);
	}

	static int GetMaxLevel(int tid)
	{
		const pet_data_temp * pTmp = Get(tid);
		if(!pTmp) return 0;
		return pTmp->max_level;
	}
};

#endif

