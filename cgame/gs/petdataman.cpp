#include "world.h"
#include "player_imp.h"
#include "petdataman.h"
#include "template/itemdataman.h"
#include "pathfinding/pathfinding.h"


bool 
pet_dataman::LoadTemplate(itemdataman & dataman)
{
	//ת��ģ��
	DATA_TYPE  dt; 
	unsigned int id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{       
		if(dt == DT_PET_ESSENCE)
		{
			const PET_ESSENCE &pet = *(const PET_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);                 
			ASSERT(&pet && dt == DT_PET_ESSENCE);
			pet_data_temp pt;
			memset(&pt,0,sizeof(pt));
			pt.tid = id;
			
			switch(pet.id_type)
			{
				case 8781:	//  ���
					pt.pet_class = pet_data::PET_CLASS_MOUNT;
					break;
				case 8782: 	//  ս������
					pt.pet_class = pet_data::PET_CLASS_COMBAT;
					break;
				case 8783:	//  ���ͳ���
					pt.pet_class = pet_data::PET_CLASS_FOLLOW;
					break;
				case 28752:	// �ٻ���
					pt.pet_class = pet_data::PET_CLASS_SUMMON;
					break;
				case 28913:	// ֲ��
					pt.pet_class = pet_data::PET_CLASS_PLANT;
					break;
				case 37698: //������
					pt.pet_class = pet_data::PET_CLASS_EVOLUTION;
					break;
				default:
					__PRINTINFO("Wrong pet id_type, pet id:%d\n",id);
					continue;
			}
			
			pt.hp_a = pet.hp_a;
			pt.hp_b = pet.hp_b;
			pt.hp_c = pet.hp_c;
			pt.hp_gen_a = pet.hp_gen_a;
			pt.hp_gen_b = pet.hp_gen_b;
			pt.hp_gen_c = pet.hp_gen_c;

			pt.damage_a = pet.damage_a;
			pt.damage_b = pet.damage_b;
			pt.damage_c = pet.damage_c;
			pt.damage_d = pet.damage_d;

			pt.speed_a = pet.speed_a;
			pt.speed_b = pet.speed_b;

			pt.attack_a = pet.attack_a;
			pt.attack_b = pet.attack_b;
			pt.attack_c = pet.attack_c;
			pt.armor_a = pet.armor_a;
			pt.armor_b = pet.armor_b;
			pt.armor_c = pet.armor_c;
			pt.defense_a = pet.physic_defence_a;
			pt.defense_b = pet.physic_defence_b;
			pt.defense_c = pet.physic_defence_c;
			pt.defense_d = pet.physic_defence_d;
			pt.resistance_a = pet.magic_defence_a;
			pt.resistance_b = pet.magic_defence_b;
			pt.resistance_c = pet.magic_defence_c;
			pt.resistance_d = pet.magic_defence_d;

			pt.mp_a = pet.mp_a;
			pt.mp_gen_a = pet.mp_gen_a;
			pt.attack_degree_a = pet.attack_degree_a;
			pt.defend_degree_a = pet.defence_degree_a;
			
			pt.body_size = pet.size;
			pt.attack_range = pet.attack_range;
			pt.damage_delay = (int)(pet.damage_delay * 20 + 0.1f);
			pt.attack_speed = (int)(pet.attack_speed * 20 + 0.1f);
			pt.sight_range = pet.sight_range;

			pt.food_mask = pet.food_mask;
			pt.inhabit_type = pet.inhabit_type;
			//�����������ܻ����ܳ���
			/*if(pt.inhabit_type != 0 && pt.inhabit_type != 1 && pt.inhabit_type != 2)
			{
				pt.inhabit_type = 0;
			}*/
			//����ģ���ʼ��ʱinhabit_type��inhabit_mode��Ӧ��ϵ�ο������趨��
			//����δʹ�ô�ֵ��ʵ���ϳ����inhabit_mode�Ǹ����˴��ڵ�layer���б仯��
			switch(pt.inhabit_type)
			{
				default:
				case 0:
					//����
					pt.inhabit_mode = NPC_MOVE_ENV_ON_GROUND;
					break;
				case 1:
					//ˮ��
					pt.inhabit_mode = NPC_MOVE_ENV_IN_WATER;
					break;
				case 2:
					//����
					pt.inhabit_mode = NPC_MOVE_ENV_ON_AIR;
					break;
				case 3:
					//�����ˮ��
					pt.inhabit_mode= NPC_MOVE_ENV_IN_WATER;
					break;
				case 4:
					//����ӿ���
					pt.inhabit_mode= NPC_MOVE_ENV_ON_GROUND;
					break;
				case 5:
					//ˮ�¼ӿ���
					pt.inhabit_mode= NPC_MOVE_ENV_ON_AIR;
					break;
				case 6:
					//��½��
					pt.inhabit_mode= NPC_MOVE_ENV_ON_GROUND;
					break;
			}

			pt.immune_type = pet.immune_type;
			pt.sacrifice_skill = pet.player_gain_skill;
			
			pt.max_level = pet.level_max;
			pt.level_require = pet.level_require;
			pt.plant_group = pet.plant_group;
			pt.group_limit = pet.group_limit;
			
			pt.evolution_id = pet.id_pet_egg_evolved;
			pt.max_r_attack = pet.attack_inherit_max_rate;
			pt.max_r_defense = pet.defence_inherit_max_rate;
			pt.max_r_hp = pet.hp_inherit_max_rate;
			pt.max_r_atk_lvl = pet.attack_level_inherit_max_rate;
			pt.max_r_def_lvl = pet.defence_level_inherit_max_rate;
			pt.min_r_attack = pet.attack_inherit_min_rate;
			pt.min_r_defense = pet.defence_inherit_min_rate;
			pt.min_r_hp = pet.hp_inherit_min_rate;

			pt.mult_r_attack = (pet.mult_r_attack < 1 ? 1 : pet.mult_r_attack);
      		pt.mult_r_defense = (pet.mult_r_defense < 1 ? 1 : pet.mult_r_defense);
      		pt.mult_r_hp = (pet.mult_r_hp  < 1 ? 1 : pet.mult_r_hp);
			
			pt.specific_skill_id = pet.specific_skill;
			
			if(pt.pet_class == pet_data::PET_CLASS_COMBAT 
					|| pt.pet_class == pet_data::PET_CLASS_SUMMON
					|| pt.pet_class == pet_data::PET_CLASS_PLANT 
					|| pt.pet_class == pet_data::PET_CLASS_EVOLUTION)
			{
				//�����ս��,���������������
				if(pt.damage_delay > 200 || pt.damage_delay <= 1
						|| pt.attack_speed <= 2 || pt.sight_range < 0.1f
						|| pt.hp_a <= 0.f)
				{
					__PRINTINFO("Wrong pet data, pet: %d\n",id);
					continue;
				}
			}
			
			Insert(pt);
		}
	}
	return true;
}

bool pet_dataman::GenerateBaseProp(int tid, int level, extend_prop & prop)
{
	const pet_data_temp * pTmp = Get(tid);
	if(!pTmp) return false;
	memset(&prop,0,sizeof(prop));

	prop.vitality = 1;
	prop.energy = 1;
	prop.strength = 1;
	prop.agility = 1;

	prop.max_hp =  pTmp->CalcHP(level);
	prop.max_mp =  0;
	prop.hp_gen = pTmp->CalcHPGEN(level);

	float speed = pTmp->speed_a + pTmp->speed_b * (level - 1);
	prop.walk_speed = speed * 0.5f;
	prop.run_speed = speed;
	prop.swim_speed = speed;
	prop.flight_speed = speed;

	int damage = pTmp->CalcDamage(level);
	prop.attack = pTmp->CalcAttack(level);
	prop.damage_low = damage;
	prop.damage_high = damage;
	prop.attack_speed = pTmp->attack_speed;
	prop.attack_range = pTmp->attack_range;
	prop.damage_magic_low = damage;
	prop.damage_magic_high = damage;

	int res = pTmp->CalcResistance(level);
	prop.resistance[0] = res;
	prop.resistance[1] = res;
	prop.resistance[2] = res;
	prop.resistance[3] = res;
	prop.resistance[4] = res;

	prop.defense = pTmp->CalcDefense(level);
	prop.armor = pTmp->CalcArmor(level);
	return true;
}

bool pet_dataman::GenerateBaseProp2(int tid, int level, int skill_level, extend_prop & prop, int& attack_degree, int& defend_degree)
{
	const pet_data_temp * pTmp = Get(tid);
	if(!pTmp) return false;
	memset(&prop,0,sizeof(prop));

	prop.vitality = 1;
	prop.energy = 1;
	prop.strength = 1;
	prop.agility = 1;

	prop.max_hp = pTmp->CalcHP(level);
	prop.max_mp = pTmp->CalcMP(level);
	prop.hp_gen = pTmp->CalcHPGEN(level);
	prop.mp_gen = pTmp->CalcMPGEN(level);

	float speed = pTmp->speed_a + pTmp->speed_b * (level - 1);
	prop.walk_speed = speed * 0.5f;
	prop.run_speed = speed;
	prop.swim_speed = speed;
	prop.flight_speed = speed;

	int damage = pTmp->CalcDamage(level);
	prop.attack = pTmp->CalcAttack(level);
	prop.damage_low = damage;
	prop.damage_high = damage;
	prop.attack_speed = pTmp->attack_speed;
	prop.attack_range = pTmp->attack_range;
	prop.damage_magic_low = damage;
	prop.damage_magic_high = damage;

	int res = pTmp->CalcResistance(level);
	prop.resistance[0] = res;
	prop.resistance[1] = res;
	prop.resistance[2] = res;
	prop.resistance[3] = res;
	prop.resistance[4] = res;

	prop.defense = pTmp->CalcDefense(level);
	prop.armor = pTmp->CalcArmor(level);

	attack_degree = pTmp->CalcAttackDegree(level);
	defend_degree = pTmp->CalcDefendDegree(level);
	return true;
}

bool pet_dataman::GenerateBaseProp3(int tid, int level, int skill_level, extend_prop & prop, int& attack_degree, int& defend_degree)
{
	const pet_data_temp * pTmp = Get(tid);
	if(!pTmp) return false;
	memset(&prop,0,sizeof(prop));

	prop.vitality = 1;
	prop.energy = 1;
	prop.strength = 1;
	prop.agility = 1;

	prop.max_hp = pTmp->CalcHP2(level);
	prop.max_mp = pTmp->CalcMP(level);
	prop.hp_gen = pTmp->CalcHPGEN(level);
	prop.mp_gen = pTmp->CalcMPGEN(level);

	float speed = pTmp->speed_a + pTmp->speed_b * (level - 1);
	prop.walk_speed = speed * 0.5f;
	prop.run_speed = speed;
	prop.swim_speed = speed;
	prop.flight_speed = speed;

	int damage = pTmp->CalcDamage(level);
	prop.attack = pTmp->CalcAttack2(level);
	prop.damage_low = damage;
	prop.damage_high = damage;
	prop.attack_speed = pTmp->attack_speed;
	prop.attack_range = pTmp->attack_range;
	prop.damage_magic_low = damage;
	prop.damage_magic_high = damage;

	int res = pTmp->CalcResistance(level);
	prop.resistance[0] = res;
	prop.resistance[1] = res;
	prop.resistance[2] = res;
	prop.resistance[3] = res;
	prop.resistance[4] = res;

	prop.defense = pTmp->CalcDefense2(level);
	prop.armor = pTmp->CalcArmor(level);

	attack_degree = pTmp->CalcAttackDegree(level);
	defend_degree = pTmp->CalcDefendDegree(level);
	return true;
}