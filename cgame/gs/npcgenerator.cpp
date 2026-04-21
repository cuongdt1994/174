#include "world.h"
#include "npcgenerator.h"
#include "clstab.h"
#include "npc.h"
#include "matter.h"
#include "ainpc.h"
#include "faction.h"
#include <arandomgen.h>
#include <glog.h>
#include "template/itemdataman.h"
#include "template/npcgendata.h"
#include "template/globaldataman.h"
#include "pathfinding/pathfinding.h"
#include "servicenpc.h"
#include "petman.h"
#include "petdataman.h"
#include "petnpc.h"
#include "player_imp.h"
#include "item/set_addon.h"

static unsigned int enemy_factions_table[32];
static float sctab[256][2];
static int InitSinCosTable()
{
	for(unsigned int i = 0; i <256; i++)
	{
		double ang = 3.1415926535*2/256.0 *i;
		sctab[i][0] = sin(ang);
		sctab[i][1] = cos(ang);
	}
	return 0;
}
namespace{
	struct  __TSKILL
	{
		inline static void copy_skill(int &t_count, void *skill_list, int skill, int level)
		{
			int index = t_count;
			ASSERT(index < 8);
			int * list = (int*)skill_list;
			list += index * 2;
			*list = skill;
			*(list + 1) = level;
			t_count ++;
		}
	};
}

bool 
npc_stubs_manager::LoadTemplate(itemdataman & dataman)
{
//��ʼ��һ��sin cos ��
	InitSinCosTable();

	DATA_TYPE  dt;
	unsigned int id = dataman.get_first_data_id(ID_SPACE_CONFIG,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_CONFIG,dt))
	{
		if(dt == DT_ENEMY_FACTION_CONFIG)
		{
			const ENEMY_FACTION_CONFIG &fac = *(const ENEMY_FACTION_CONFIG*)dataman.get_data_ptr(id,ID_SPACE_CONFIG,dt);
			ASSERT(dt == DT_ENEMY_FACTION_CONFIG);
			memcpy(enemy_factions_table,fac.enemy_factions,sizeof(enemy_factions_table));
		}
	}
#ifdef __TEST_PERFORMANCE__
	for(unsigned int i = 0; i < 32; i ++)
	{
		enemy_factions_table[i] = 0xFFFFFFFF;
	}
#endif

	unsigned int num = dataman.get_data_num(ID_SPACE_ESSENCE);
	ASSERT(num);
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_MONSTER_ESSENCE)
		{	
			const MONSTER_ESSENCE &mob = *(const MONSTER_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&mob && dt == DT_MONSTER_ESSENCE);

			const MONSTER_TYPE & mt = *(const MONSTER_TYPE *)dataman.get_data_ptr(mob.id_type,ID_SPACE_ESSENCE,dt);		
			ASSERT(&mt && dt == DT_MONSTER_TYPE);

			npc_template nt;
			memset(&nt,0,sizeof(nt));
			nt.tid = mob.id;

			//���ø������Եĸ���
			float prob = 0.f;
			for(unsigned int j = 0; j < 16; j ++)
			{
				if(mt.addons[j].probability_addon <= 1e-8) continue;
				int addon_id = mt.addons[j].id_addon;
				DATA_TYPE dt2;
				const MONSTER_ADDON & mad = *(const MONSTER_ADDON*)dataman.get_data_ptr(addon_id,ID_SPACE_ADDON,dt2);
				int mb_addon_id = 0;
				if(dt2 == DT_MONSTER_ADDON && &mad) 
				{
					mb_addon_id = mad.param1;
				}
				int index = nt.addon_choice_count;
				nt.id_addon[index] = mb_addon_id;
				ASSERT(mb_addon_id >=0 && mb_addon_id <= 9);
				nt.probability_addon[index] = mt.addons[j].probability_addon;
				nt.addon_choice_count ++;
				prob += mt.addons[j].probability_addon;
			}
			ASSERT(fabs(prob - 1.f) < 1e-5);

			//����npc�������� ����ͺ����layer����Ӧ��һ�� ���Կ������Ժ��mode���趨
			nt.inhabit_type = mob.inhabit_type;
			switch(mob.inhabit_type)
			{
			case 1:
				//ˮ��
				nt.inhabit_mode= NPC_MOVE_ENV_IN_WATER;
				break;
			case 2:
				//����
				nt.inhabit_mode= NPC_MOVE_ENV_ON_AIR;
				break;
			case 5:
				//ˮ�¼ӿ���
				nt.inhabit_mode= NPC_MOVE_ENV_ON_AIR;
				break;
			case 3:
				//�����ˮ��
				nt.inhabit_mode= NPC_MOVE_ENV_IN_WATER;
				break;
			case 4:
				//����ӿ���
			case 6:
				//��½��
				nt.inhabit_mode= NPC_MOVE_ENV_ON_GROUND;
				break;
			case 0:	
				//����
			default:
				nt.inhabit_mode= NPC_MOVE_ENV_ON_GROUND;
			}

			
			nt.petegg_id = mob.id_pet_egg_captured;
			nt.drop_no_protected = mob.drop_protected;
			nt.drop_no_profit_limit = mob.drop_for_no_profit_time;
			nt.drop_mine_prob = mob.drop_mine_probability;
			nt.drop_mine_group = mob.drop_mine_condition_flag;
			nt.body_size = mob.size;
			nt.faction = mob.faction;
			//����ж�����
			for(unsigned int i = 0; i < 32; i ++)
			{
				if(nt.faction & ( 1 << i))
				{
					nt.enemy_faction |= enemy_factions_table[i];
				}
			}

			nt.monster_faction = mob.monster_faction;

			nt.role_in_war =  mob.role_in_war;

			if(mob.role_in_war)
			{
				// ����ǳ�ս˫�� ������빥���Ѿ����ط��Ѿ���faction
				if(nt.faction & FACTION_BATTLEOFFENSE)
				{
					//����
					nt.faction |= FACTION_OFFENSE_FRIEND;
				}
				else if(nt.faction & FACTION_BATTLEDEFENCE)
				{
					//�ط�
					nt.faction |= FACTION_DEFENCE_FRIEND;
				}
			}

			// Fulano
			

			//���ݳ�ս���ݽ�������
			switch (mob.role_in_war)
			{
				case 1:	//��־�Խ���
					//���ﲻ���κ�����
					break;
					
				case 2:	//���� ������㱣������
					//���ｫ������faction���������ɲ����κι�������
					nt.faction &= FACTION_DEFENCE_FRIEND|FACTION_OFFENSE_FRIEND;
					break;
					
				case 3: //����
					//����ӵ������ı����߼�
					//������д���Ǽ������߼�����������ʹ�õ�
					//��������NPCҲʹ�ô���
					break;

				case 4: //���ǳ�
					nt.faction &= ~(FACTION_BATTLEDEFENCE|FACTION_BATTLEOFFENSE);
					break;

				case 6: //�������NPC ����NPC���ᱻ����������ֻ��һ��DUMMY
					break;
				
				case 7: //����npc
					nt.faction &= FACTION_DEFENCE_FRIEND|FACTION_OFFENSE_FRIEND;
					break;
					
				case 8:	
					//�ɱ�����NPC
					//���ﲻ���κ�����
					break;
			}
			
			nt.id_strategy = mob.id_strategy;
			nt.sight_range = mob.sight_range;
			nt.trigger_policy = mob.common_strategy; //�������ID
			
			if(nt.trigger_policy)
			{
				if(world_manager::GetTriggerMan().GetPolicy(nt.trigger_policy) == NULL)
				{
					//�Ҳ������ʵĲ���
					__PRINTINFO("����%d�Ĳ���%d�޷��ڲ����ļ����ҵ�\n",nt.tid,nt.trigger_policy);
					nt.trigger_policy = 0;
				}
			}
		



//			nt.id_skill = mob.id_skill;
//			nt.id_skill_level = mob.skill_level;

			nt.immune_type = mob.immune_type; 
			nt.aggressive_mode = mob.aggressive_mode;
			nt.monster_faction_ask_help = mob.monster_faction_ask_help;
			nt.monster_faction_can_help = mob.monster_faction_can_help;
			nt.aggro_range = mob.aggro_range;
			nt.aggro_time = (int)mob.aggro_time;
			if(nt.aggro_time <= 0) nt.aggro_time = 1;
			nt.damage_delay = (int)(mob.damage_delay * 20.f);
			ASSERT(sizeof(nt.skill_hp75) == sizeof(mob.skill_hp75));
			memcpy(nt.skill_hp75 , &mob.skill_hp75,sizeof(nt.skill_hp75));
			memcpy(nt.skill_hp50 , &mob.skill_hp50,sizeof(nt.skill_hp50));
			memcpy(nt.skill_hp25 , &mob.skill_hp25,sizeof(nt.skill_hp25));
			nt.patrol_mode = mob.patroll_mode?1:0;
			nt.after_death = mob.after_death;
#ifdef __TEST_PERFORMANCE__			
			nt.sight_range = mob.sight_range*3;
			nt.aggressive_mode = 1;
			nt.aggro_range = 150;
			nt.aggro_time = 180;
#endif

			nt.after_death_summon_monster_id = mob.after_death_summon_monster_id;
			nt.after_death_summon_monster_radius = mob.after_death_summon_monster_radius;
			nt.after_death_summon_monster_num = mob.after_death_summon_monster_num;

			nt.max_received_damage_per_hit = mob.max_received_damage_per_hit;

			for(unsigned int j = 0; j < 32; j ++)
			{
				if(mob.skills[j].id_skill <= 0) continue;
				int id_skill = mob.skills[j].id_skill;
				int lvl_skill = mob.skills[j].level;
				int type = GNET::SkillWrapper::GetType(id_skill);
				if(type < 0) 
				{
					__PRINTINFO("����%d��ʹ�ü���%d������\n",nt.tid,id_skill);
					continue;
				}

				switch(type)
				{
					case 1: //����
						__TSKILL::copy_skill(nt.skills.as_count,nt.skills.attack_skills,
								id_skill,lvl_skill);
						break;

					case 2: //ף��
						__TSKILL::copy_skill(nt.skills.bs_count,nt.skills.bless_skills,
								id_skill,lvl_skill);
						break;

					case 3: //����
						__TSKILL::copy_skill(nt.skills.cs_count,nt.skills.curse_skills,
								id_skill,lvl_skill);
						break;

					default:
					//����
					__PRINTINFO("����%d�ļ���%d����δ֪%d\n",nt.tid,id_skill,type);
				}
				
			}

			if((nt.id_strategy == 2 || nt.id_strategy == 3) && (nt.skills.as_count==0 && nt.skills.cs_count==0))
			{
				__PRINTINFO("���ܹ���%dû�����ü��ܺ� %d\n",nt.tid,nt.skills.cs_count);
			}
			if(nt.damage_delay > 256)
			{
				__PRINTINFO("���﹥���˺��ͺ�ʱ��̫��npc:%d  delay:%d\n",nt.tid,nt.damage_delay);
				continue;
			}


			//���ó�޶Ȳ���
			prob = 0.f;
			for(unsigned int j= 0; j < 4; j ++)
			{
				if(mob.aggro_strategy[j].probability < 1e-7) continue;
				int index = nt.aggro_strategy_count;
				nt.aggro_strategy_ids[index] = mob.aggro_strategy[j].id;
				nt.aggro_strategy_probs[index] = mob.aggro_strategy[j].probability;
				nt.aggro_strategy_count ++;
				prob += mob.aggro_strategy[j].probability;
			}
			ASSERT(fabs(prob - 1.f) < 1e-5);

			nt.normal_heartbeat_in_idle = mob.highest_frequency?1:0;
			if(mob.max_move_range > 1e-6)
				nt.max_move_range = mob.max_move_range >= 20.f ? mob.max_move_range : 20.f;
			else
				nt.max_move_range = 0.f;

			//����ģ���������
			nt.bp.level = mob.level;
			nt.bp.exp = mob.exp;
			nt.bp.skill_point = mob.skillpoint;
			nt.bp.hp = mob.life;
			nt.bp.mp = 1;
			nt.bp.status_point = 1;
			nt.hp_adjust_common_value = mob.hp_adjust_common_value;
			nt.defence_adjust_common_value = mob.defence_adjust_common_value;
			nt.attack_adjust_common_value = mob.attack_adjust_common_value;
			nt.attack_degree = mob.attack_degree;
			nt.defend_degree = mob.defend_degree;
			nt.invisible_degree = mob.invisible_lvl;
			if(nt.invisible_degree) nt.invisible_degree += nt.bp.level;
			nt.anti_invisible_degree = mob.uninvisible_lvl;
			if(nt.anti_invisible_degree < 0) nt.anti_invisible_degree = 0;
			else nt.anti_invisible_degree += nt.bp.level;
			nt.no_accept_player_buff = mob.no_accept_player_buff;
			nt.no_auto_fight = mob.no_auto_fight;
			nt.fixed_direction = mob.fixed_direction;
			nt.faction_building_id = mob.id_building;
			nt.set_owner = mob.combined_switch & MCS_SUMMONER_ATTACK_ONLY;
			if(nt.set_owner && mob.combined_switch & MCS_RECORD_DPS_RANK) nt.record_dps_rank = true;
			nt.domain_related = mob.domain_related;
			memcpy(nt.local_var,mob.local_var,sizeof(nt.local_var));

			nt.ep.vitality = 1;
			nt.ep.energy = 1;
			nt.ep.strength = 1;
			nt.ep.agility = 1;
			nt.ep.max_hp = mob.life;
			nt.ep.max_mp = 1;
			nt.ep.hp_gen = mob.hp_regenerate;
			nt.ep.mp_gen = 1;

			nt.ep.walk_speed = mob.walk_speed;
			nt.ep.run_speed = mob.run_speed;
			nt.ep.swim_speed = mob.swim_speed;
			nt.ep.flight_speed = mob.fly_speed;

			if(mob.walk_speed <= 1e-1 ||  mob.run_speed < 1e-1)
			{
				__PRINTINFO("�����ٶȹ�С%d������:%f, ��:%f\n",nt.tid,mob.walk_speed,mob.run_speed);
			}

			nt.ep.attack = mob.attack;
			nt.ep.damage_low  = mob.damage_min;
			nt.ep.damage_high = mob.damage_max;
			nt.ep.attack_speed = (int)((mob.attack_speed*20.f) + 0.5f);
			if(nt.ep.attack_speed <= 0 || nt.ep.damage_low <= 0 || nt.ep.attack <= 0)
			{
				__PRINTINFO("���ִ���Ĺ���ģ�� %d\n",nt.tid);
				continue; 
			}
			if(nt.ep.attack_speed > 256)
			{
				__PRINTINFO("���ִ���Ĺ���ģ�� %d ����ʱ�����%d\n",nt.tid,nt.ep.attack_speed);
				continue; 
			}

			nt.ep.attack_range = mob.attack_range;
			if(mob.attack_range > 6.0f) //�����������6�׵Ĺ�����Զ�̹���
			{
				nt.short_range_mode = 1;
			}

			nt.ep.addon_damage[0].damage_low = mob.magic_damages_ext[0].damage_min;
			nt.ep.addon_damage[0].damage_high = mob.magic_damages_ext[0].damage_max;
			nt.ep.addon_damage[1].damage_low = mob.magic_damages_ext[1].damage_min;
			nt.ep.addon_damage[1].damage_high = mob.magic_damages_ext[1].damage_max;
			nt.ep.addon_damage[2].damage_low = mob.magic_damages_ext[2].damage_min;
			nt.ep.addon_damage[2].damage_high = mob.magic_damages_ext[2].damage_max;
			nt.ep.addon_damage[3].damage_low = mob.magic_damages_ext[3].damage_min;
			nt.ep.addon_damage[3].damage_high = mob.magic_damages_ext[3].damage_max;
			nt.ep.addon_damage[4].damage_low = mob.magic_damages_ext[4].damage_min;
			nt.ep.addon_damage[4].damage_high = mob.magic_damages_ext[4].damage_max;

			nt.ep.damage_magic_low = mob.magic_damage_min;
			nt.ep.damage_magic_high = mob.magic_damage_max;

			nt.ep.resistance[0] = mob.magic_defences[0];
			nt.ep.resistance[1] = mob.magic_defences[1];
			nt.ep.resistance[2] = mob.magic_defences[2];
			nt.ep.resistance[3] = mob.magic_defences[3];
			nt.ep.resistance[4] = mob.magic_defences[4];
			
			nt.ep.defense = mob.defence;
			nt.ep.armor = mob.armor;
			Insert(nt);
		}
	}

	//����NPC�ڲ��Ĺ���ID�������ȶ���ȫ���Ĺ������ݲ��ܽ���NPC���ݵĶ�ȡ
	abase::vector<TRANS_TARGET_SERV> & waypoint_list = globaldata_gettranstargetsserver();
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_NPC_ESSENCE)
		{	
			//ö��NPC������
			const NPC_ESSENCE &npc = *(const NPC_ESSENCE *)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&npc && dt == DT_NPC_ESSENCE);
			//ʹ�������ģ��
			int mobs_id = npc.id_src_monster;
			npc_template * mobs = NULL;
			if(mobs_id == 0 || (mobs = Get(mobs_id)) == NULL)
			{
				__PRINTINFO("����Ĺ���NPCģ��%d��û�ж�Ӧ�Ĺ���id %d\n",npc.id,mobs_id);
				continue;
			}
			npc_template nt;
			nt = *mobs;
			nt.tid = npc.id;

			nt.has_collision = !npc.no_collision;
			nt.npc_data.is_npc = 1;
			nt.npc_data.src_monster = mobs_id;
			nt.npc_data.refresh_time = (int)(npc.refresh_time * 20);
			nt.npc_data.attack_rule = npc.attack_rule;
			nt.npc_data.tax_rate = npc.tax_rate ;
			nt.npc_data.need_domain = npc.domain_related;
			if(npc.combined_switch & NCS_IGNORE_DISTANCE_CHECK) nt.npc_data.serve_distance_unlimited = true;

			//�������ַ���
			if(npc.id_sell_service)
			{
				//��������
				DATA_TYPE dt2;
				const NPC_SELL_SERVICE &service = *(const NPC_SELL_SERVICE*)dataman.get_data_ptr(npc.id_sell_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_SELL_SERVICE))
				{
					__PRINTINFO("�����˴����sell service %d ��NPC %d\n",npc.id_sell_service,nt.tid);
					continue;
				}
				//��������
				int number = 0;
				for(int i = 0; i < 8 ; ++i)
				for(int j = 0; j < 32; ++j)
				{
					nt.npc_data.service_sell_goods[6*number] = service.pages[i].goods[j].id;
					nt.npc_data.service_sell_goods[6*number+1] = service.pages[i].goods[j].contrib_cost;
					nt.npc_data.service_sell_goods[6*number+2] = service.pages[i].require_contrib;
					nt.npc_data.service_sell_goods[6*number+3] = service.pages[i].goods[j].force_contribution_cost;
					nt.npc_data.service_sell_goods[6*number+4] = service.pages[i].require_force;
					nt.npc_data.service_sell_goods[6*number+5] = service.pages[i].require_force_reputation;
					number ++;
				}
				nt.npc_data.service_sell_num = number;

				//ֻҪ��������������������
				nt.npc_data.service_purchase = 1;
			}
/*
			//�������ַ���
			if(npc.id_buy_service)
			{
				//��������
				nt.npc_data.service_purchase = 1;
			}
			*/

			if(npc.id_repair_service)
			{
				nt.npc_data.service_repair = 1;
			}

			if(npc.id_heal_service)
			{
				nt.npc_data.service_heal= 1;
			}

			if(npc.id_transmit_service)
			{
				DATA_TYPE dt2;
				const NPC_TRANSMIT_SERVICE &service = *(const NPC_TRANSMIT_SERVICE*)dataman.get_data_ptr(npc.id_transmit_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TRANSMIT_SERVICE))
				{
					__PRINTINFO("�����˴����transmit service %d ��NPC %d\n",npc.id_transmit_service,nt.tid);
					continue;
				}

				nt.npc_data.service_transmit_target_num = service.num_targets;
				ASSERT(service.num_targets);
				for(int i = 0; i < service.num_targets; ++i)
				{
					nt.npc_data.transmit_entry[i].fee = service.targets[i].fee;
					nt.npc_data.transmit_entry[i].require_level = service.targets[i].required_level;

					int target = service.targets[i].idTarget;
					nt.npc_data.transmit_entry[i].target_waypoint =  target;
					unsigned int j = 0;
					for(; j < waypoint_list.size(); j ++)
					{
						if(waypoint_list[j].id == target)
						{
							
							A3DVECTOR pos(waypoint_list[j].vecPos.x, waypoint_list[j].vecPos.y, waypoint_list[j].vecPos.z);
							nt.npc_data.transmit_entry[i].target = pos;
							nt.npc_data.transmit_entry[i].world_tag = waypoint_list[j].world_tag;
							break;
						}
					}
					if(j == waypoint_list.size()) 
					{
						__PRINTINFO("npc %d ������ڴ���Ĵ���Ŀ��(index:%d,target:%d)\n",npc.id,i, target);
						nt.npc_data.transmit_entry[i].target = A3DVECTOR(0.f,0.f,0.f);
						nt.npc_data.transmit_entry[i].world_tag = 0;
					}
				}
			}

			if(npc.id_task_in_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_IN_SERVICE &service = *(const NPC_TASK_IN_SERVICE*)dataman.get_data_ptr(npc.id_task_in_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_IN_SERVICE))
				{
					__PRINTINFO("�����˴����task in service %d ��NPC %d\n",npc.id_task_in_service,nt.tid);
					continue;
				}
				
				int num = 0;
				for(int i = 0; i < 256; ++i)
				{
					if(!service.tasks[i].id) continue;
					nt.npc_data.service_task_in_list[num] = service.tasks[i].id;
					num ++;
				}
				nt.npc_data.service_task_in_num = num;
			}

			if(npc.id_task_out_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_OUT_SERVICE &service = *(const NPC_TASK_OUT_SERVICE*)dataman.get_data_ptr(npc.id_task_out_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_OUT_SERVICE))
				{
					__PRINTINFO("�����˴����task out service %d ��NPC %d\n",npc.id_task_out_service,nt.tid);
					continue;
				}

				int num = 2;
				nt.npc_data.service_task_out_list[0] = service.storage_id;
				nt.npc_data.service_task_out_list[1] = service.storage_refresh_item;
				for(int i = 0; i < 256; ++i)
				{
					if(!service.tasks[i].id) continue;
					nt.npc_data.service_task_out_list[num] = service.tasks[i].id;
					num ++;
				}
				nt.npc_data.service_task_out_num = num;
				if(num == 2)
				{
					//�����������
					nt.npc_data.service_task_out_num = 0;
				}
			}

			if(npc.id_task_matter_service)
			{	
				DATA_TYPE dt2;
				const NPC_TASK_MATTER_SERVICE &service = *(const NPC_TASK_MATTER_SERVICE*)dataman.get_data_ptr(npc.id_task_matter_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_TASK_MATTER_SERVICE))
				{
					__PRINTINFO("�����˴����task matter service %d ��NPC %d\n",npc.id_task_matter_service,nt.tid);
					continue;
				}

				int num = 0;
				for(int i = 0; i < 16; ++i)
				{
					if(!service.tasks[i].id_task) continue;
					nt.npc_data.service_task_matter_list[num] = service.tasks[i].id_task;
					num ++;
				}
				nt.npc_data.service_task_matter_num = num;
			}

			if(npc.id_install_service)
			{
				nt.npc_data.service_install = 1;
			}

			if(npc.id_uninstall_service)
			{
				nt.npc_data.service_uninstall = 1;
			}

			if(npc.id_skill_service)
			{
				DATA_TYPE dt2;
				const NPC_SKILL_SERVICE &service = *(const NPC_SKILL_SERVICE*)dataman.get_data_ptr(npc.id_skill_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_SKILL_SERVICE))
				{
					__PRINTINFO("�����˴����skill service %d ��NPC %d\n",npc.id_skill_service,nt.tid);
					continue;
				}
				int num = 0;
				for(int i = 0; i < 256; ++i)
				{
					if(!service.id_skills[i]) continue;
					nt.npc_data.service_teach_skill_list[num] = service.id_skills[i];
					num ++;
				}
				nt.npc_data.service_teach_skill_num = num;
			}

			if(npc.id_make_service)
			{
				DATA_TYPE dt2;
				const NPC_MAKE_SERVICE &service = *(const NPC_MAKE_SERVICE*)dataman.get_data_ptr(npc.id_make_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_MAKE_SERVICE))
				{
					__PRINTINFO("�����˴����make service %d ��NPC %d\n",npc.id_make_service,nt.tid);
					continue;
				}
				nt.npc_data.service_produce.produce_skill = service.id_make_skill;
				int num = 0;
				for(unsigned int  i = 0; i < 8; i ++)
				{
					for(unsigned int j = 0; j < 32;j ++)
					{
						if(service.pages[i].id_goods[j])
						{
							nt.npc_data.service_produce.produce_items[num] = service.pages[i].id_goods[j];
							num ++;
						}
					}
				}
				nt.npc_data.service_produce.produce_num = num;
				switch(service.produce_type)
				{
					default:
					case 0:		//����
					case 2:		//�Ĳ�
						nt.npc_data.service_produce.type = 0;
						break;
					case 1:		//�ϳ�
						nt.npc_data.service_produce.type = 1;
						break;
					case 3:		//��������
						nt.npc_data.service_produce.type = 2;
						break;
					case 4:		//�¼̳�����
						nt.npc_data.service_produce.type = 3;
						break;
                    case 5:		//�̳и�����������
						nt.npc_data.service_produce.type = 4;
						break;
					case 6:
						nt.npc_data.service_produce.type = 5;
						break;
				}
			}
			
			if(npc.id_decompose_service)
			{
				DATA_TYPE dt2;
				const NPC_DECOMPOSE_SERVICE &service = *(const NPC_DECOMPOSE_SERVICE*)dataman.get_data_ptr(npc.id_decompose_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_DECOMPOSE_SERVICE))
				{
					__PRINTINFO("�����˴����decompose service %d ��NPC %d\n",npc.id_decompose_service,nt.tid);
					continue;
				}
				nt.npc_data.service_decompose_skill = service.id_decompose_skill;
			}
			
			if(npc.id_storage_service)
			{
				nt.npc_data.service_storage = 1;
				nt.npc_data.service_user_trashbox = 1;	
			}

			if(npc.id_identify_service)
			{
				DATA_TYPE dt2;
				const NPC_IDENTIFY_SERVICE &service = *(const NPC_IDENTIFY_SERVICE*)dataman.get_data_ptr(npc.id_identify_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_IDENTIFY_SERVICE))
				{
					__PRINTINFO("�����˴����identify service %d ��NPC %d\n",npc.id_identify_service,nt.tid);
					continue;
				}
				nt.npc_data.service_identify  = true;
				nt.npc_data.service_identify_fee  = service.fee;
				if(service.fee < 0)
				{
					__PRINTINFO("�����˴����identify service %d ��NPC %d\n",npc.id_identify_service,nt.tid);
					continue;
				}
			}

			if(npc.id_war_towerbuild_service)
			{

				if(nt.role_in_war != 3)
				{
					__PRINTINFO("�����˴���ļ�������%d �ڷǼ���NPC�� %d\n",npc.id_war_towerbuild_service,nt.tid);
					continue;
				}

				//��Ϊһ��NPC
				//����Լ���faction�еĹ���˫���� �����ܵ�����
				nt.faction &= ~(FACTION_BATTLEOFFENSE|FACTION_BATTLEDEFENCE);
				
				DATA_TYPE dt2;
				const NPC_WAR_TOWERBUILD_SERVICE &service = *(const NPC_WAR_TOWERBUILD_SERVICE*)dataman.get_data_ptr(npc.id_war_towerbuild_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_WAR_TOWERBUILD_SERVICE))
				{
					__PRINTINFO("�����˴����identify service %d ��NPC %d\n",npc.id_war_towerbuild_service,nt.tid);
					continue;
				}
				unsigned int t = 0;
				for(unsigned int i = 0; i < 4; i ++)
				{
					if(service.build_info[i].id_in_build == 0 ||
							service.build_info[i].id_buildup == 0)
					{
						continue;
					}
					nt.npc_data.npc_tower_build[t].id_in_build=service.build_info[i].id_in_build;
					nt.npc_data.npc_tower_build[t].id_buildup = service.build_info[i].id_buildup;
					nt.npc_data.npc_tower_build[t].id_object_need = service.build_info[i].id_object_need;
					nt.npc_data.npc_tower_build[t].time_use	= service.build_info[i].time_use;
					nt.npc_data.npc_tower_build[t].fee	= service.build_info[i].fee;
					if(nt.npc_data.npc_tower_build[t].fee < 0) continue;
					t ++;
				}
				
				if(t == 0)
				{
					__PRINTINFO("��������%d û����Ч�Ľ�������\n", npc.id_war_towerbuild_service);
					continue;
				}
				nt.npc_data.npc_tower_build_size = t;

			}

			if(npc.id_resetprop_service)
			{
				DATA_TYPE dt2;
				const NPC_RESETPROP_SERVICE &service = *(const NPC_RESETPROP_SERVICE*)dataman.get_data_ptr(npc.id_resetprop_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_RESETPROP_SERVICE))
				{
					__PRINTINFO("�����˴����resetprop service %d ��NPC %d\n",npc.id_resetprop_service,nt.tid);
					continue;
				}
				unsigned int t = 0;
				for(unsigned int i = 0; i < 15; i ++)
				{
					if(service.prop_entry[i].id_object_need <=0) continue;
					nt.npc_data.reset_prop[t].object_need	= service.prop_entry[i].id_object_need;
					nt.npc_data.reset_prop[t].str_delta	= service.prop_entry[i].strength_delta;
					nt.npc_data.reset_prop[t].agi_delta	= service.prop_entry[i].agility_delta; 
					nt.npc_data.reset_prop[t].vit_delta	= service.prop_entry[i].vital_delta;   
					nt.npc_data.reset_prop[t].eng_delta	= service.prop_entry[i].energy_delta;  

					if( nt.npc_data.reset_prop[t].str_delta < 0 || nt.npc_data.reset_prop[t].agi_delta < 0 ||  nt.npc_data.reset_prop[t].vit_delta < 0 || nt.npc_data.reset_prop[t].eng_delta  < 0)
					{
						__PRINTINFO("�����˴����resetprop service %d ��NPC %d\n",npc.id_resetprop_service,nt.tid);
						ASSERT(false && "ϴ����������ݲ���ȷ");
						continue;
					}
					t ++;
				}
				nt.npc_data.service_reset_prop_count = t;

			}

			if(npc.id_petname_service)
			{
				DATA_TYPE dt2;
				const NPC_PETNAME_SERVICE &service = *(const NPC_PETNAME_SERVICE*)dataman.get_data_ptr(npc.id_petname_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_PETNAME_SERVICE))
				{
					__PRINTINFO("�����˴����petname service %d ��NPC %d\n",npc.id_petname_service,nt.tid);
					continue;
				}
				nt.npc_data.service_change_pet_name = 1;
				nt.npc_data.change_pet_name_prop.money_need = service.price;
				nt.npc_data.change_pet_name_prop.item_need = service.id_object_need;				
			}

			if(npc.id_petlearnskill_service)
			{
				DATA_TYPE dt2;
				const NPC_PETLEARNSKILL_SERVICE &service = *(const NPC_PETLEARNSKILL_SERVICE*)dataman.get_data_ptr(npc.id_petlearnskill_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_PETLEARNSKILL_SERVICE))
				{
					__PRINTINFO("�����˴����learnskill service %d ��NPC %d\n",npc.id_petlearnskill_service,nt.tid);
					continue;
				}
				int num = 0;
				for(unsigned int i = 0; i < 128; i ++)
				{
					if(service.id_skills[i])
					{
						nt.npc_data.service_pet_skill_list[num] = service.id_skills[i]; 
						num ++;
					}
				}
				nt.npc_data.service_pet_skill_num  = num;
			}

			if(npc.id_petforgetskill_service)
			{
				DATA_TYPE dt2;
				const NPC_PETFORGETSKILL_SERVICE &service = *(const NPC_PETFORGETSKILL_SERVICE*)dataman.get_data_ptr(npc.id_petforgetskill_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_PETFORGETSKILL_SERVICE))
				{
					__PRINTINFO("�����˴����petforgetskill service %d ��NPC %d\n",npc.id_petforgetskill_service,nt.tid);
					continue;
				}
				nt.npc_data.service_forget_pet_skill = 1;
				nt.npc_data.forget_pet_skill_prop.money_need = service.price;
				nt.npc_data.forget_pet_skill_prop.item_need = service.id_object_need;
			}

			if(npc.id_equipbind_service)
			{
				DATA_TYPE dt2;
				const NPC_EQUIPBIND_SERVICE &service = *(const NPC_EQUIPBIND_SERVICE*)dataman.get_data_ptr(npc.id_equipbind_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPBIND_SERVICE))
				{
					__PRINTINFO("�����˴����bind service %d ��NPC %d\n",npc.id_equipbind_service,nt.tid);
					continue;
				}
				nt.npc_data.service_equip_bind = 1;
				nt.npc_data.service_bind_prop.money_need = service.price;
				nt.npc_data.service_bind_prop.can_webtrade = (service.bind_type==ITEM_BIND_WEBTRADE ? 1 : 0);
				ASSERT(sizeof(nt.npc_data.service_bind_prop.item_need) == sizeof(service.id_object_need));
				memcpy(nt.npc_data.service_bind_prop.item_need, service.id_object_need, sizeof(service.id_object_need));

			}

			if(npc.id_equipdestroy_service)
			{
				DATA_TYPE dt2;
				const NPC_EQUIPDESTROY_SERVICE &service = *(const NPC_EQUIPDESTROY_SERVICE*)dataman.get_data_ptr(npc.id_equipdestroy_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPDESTROY_SERVICE))
				{
					__PRINTINFO("�����˴����destroy service %d ��NPC %d\n",npc.id_equipdestroy_service,nt.tid);
					continue;
				}
				nt.npc_data.service_destroy_bind = 1;
				nt.npc_data.service_destroy_bind_prop.money_need = service.price;
				nt.npc_data.service_destroy_bind_prop.item_need = service.id_object_need;

			}
			
			if(npc.id_equipundestroy_service)
			{
				DATA_TYPE dt2;
				const NPC_EQUIPUNDESTROY_SERVICE &service = *(const NPC_EQUIPUNDESTROY_SERVICE*)dataman.get_data_ptr(npc.id_equipundestroy_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_EQUIPUNDESTROY_SERVICE))
				{
					__PRINTINFO("�����˴����undestroy service %d ��NPC %d\n",npc.id_equipundestroy_service,nt.tid);
					continue;
				}
				nt.npc_data.service_undestroy_bind = 1;
				nt.npc_data.service_undestroy_bind_prop.money_need = service.price;
				nt.npc_data.service_undestroy_bind_prop.item_need = service.id_object_need;

			}
	
			//���ñ�����·����Ϣ
			if(npc.combined_services & 0x0008)
			{
				if(npc.id_to_discover)
				{
					nt.npc_data.service_waypoint_id = npc.id_to_discover;
					unsigned int j = 0;
					for(; j < waypoint_list.size(); j ++)
					{
						if((int)waypoint_list[j].id == (int)npc.id_to_discover)
						{
							
							break;
						}
					}
					if(j == waypoint_list.size()) 
					{
						__PRINTINFO("npc%d���ַ����id����ȷ %d\n", npc.id,npc.id_to_discover);
					}
				}
				else
				{
					__PRINTINFO("NPC%d���ַ���IDΪ0\n",npc.id);
				}
			}

			if(npc.combined_services & 0x01)
			{
				//�������������ܷ���
				nt.npc_data.service_unlearn_skill = 1;
			}

			if(npc.combined_services & 0x02)
			{
				nt.npc_data.service_make_slot = 1;
			}

			if(npc.combined_services & 0x10)
			{
				//�а��ɷ��� 
				nt.npc_data.service_faction = 1;
			}

			if(npc.combined_services & 0x20)
			{
				//�����ݷ���
				nt.npc_data.service_face_ticket = 1;
			}

			if(npc.combined_services & 0x40)
			{
				//���ʼ�����
				nt.npc_data.service_mail = 1;
			}
			
			if(npc.combined_services & 0x80)
			{
				//����������
				nt.npc_data.service_auction = 1;
			}

			if(npc.combined_services & 0x100)
			{
				//��˫���������
				nt.npc_data.service_double_exp = 1;
			}

			if(npc.combined_services & 0x200)
			{
				//�������ﵰ����
				nt.npc_data.service_hatch_pet = 1;
			}
			
			if(npc.combined_services & 0x400)
			{
				//��ԭ���ﵰ����
				nt.npc_data.service_recover_pet = 1;
			}

			if(npc.combined_services & 0x800)
			{
				//��ս����
				nt.npc_data.service_war_management = 1;
			}

			if(npc.combined_services & 0x1000)
			{
				//��ս����
				nt.npc_data.service_war_leave_battle = 1;
			}

			if(npc.combined_services & 0x2000)
			{
				//�㿨����
				nt.npc_data.service_cash_trade = 1;
			}

			if(npc.combined_services & 0x4000)
			{
				nt.npc_data.service_refine = 1;
			}

			if(npc.combined_services & 0x8000)
			{
				nt.npc_data.service_dye = 1;
				nt.npc_data.service_dye_suit = 1;
				nt.npc_data.service_dye_pet = 1;
			}

			if(npc.combined_services & 0x10000)
			{
				nt.npc_data.service_refine_transmit = 1;
			}
			//lgc
			if(npc.combined_services & 0x20000)
			{
				nt.npc_data.service_elf_dec_attributie = 1;
			}
			if(npc.combined_services & 0x40000)
			{
				nt.npc_data.service_elf_flush_genius = 1;
			}
			if(npc.combined_services & 0x80000)
			{
				nt.npc_data.service_elf_forget_skill = 1;
			}
			if(npc.combined_services & 0x100000)
			{
				nt.npc_data.service_elf_refine = 1;
				nt.npc_data.service_elf_refine_transmit = 1;
			}
			if(npc.combined_services & 0x200000)
			{
				nt.npc_data.service_elf_decompose = 1;
			}
			if(npc.combined_services & 0x400000)
			{
				nt.npc_data.service_elf_destroy_item = 1;
			}
			if(npc.combined_services & 0x800000)
			{
				nt.npc_data.service_repair_damaged_item = 1;	
			}
			if(npc.combined_services & 0x1000000)
			{
				nt.npc_data.service_webtrade = 1;	
			}
			if(npc.combined_services & 0x2000000)
			{
				nt.npc_data.service_god_evil_convert = 1;	
			}
			if(npc.combined_services & 0x4000000)
			{
				nt.npc_data.service_wedding_book = 1;	
				nt.npc_data.service_wedding_invite = 1;	
			}
			if(npc.combined_services & 0x8000000)
			{
				nt.npc_data.service_factionfortress = 1;
			}
			if(npc.combined_services & 0x10000000)
			{
				nt.npc_data.service_factionfortress2 = 1;
			}
			if(npc.combined_services & 0x20000000)
			{
				nt.npc_data.service_factionfortress_material_exchange = 1;
			}
			if(npc.combined_services & 0x40000000)
			{
				nt.npc_data.service_trashbox_view = 1;
			}
			if(npc.combined_services & 0x80000000)
			{
				nt.npc_data.service_dpsrank = 1;
			}
			if(npc.id_goblin_skill_service)
			{
				DATA_TYPE dt2;
				const NPC_SKILL_SERVICE &service = *(const NPC_SKILL_SERVICE*)dataman.get_data_ptr(npc.id_goblin_skill_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_SKILL_SERVICE))
				{
					__PRINTINFO("�����˴����goblin skill service %d ��NPC %d\n",npc.id_goblin_skill_service,nt.tid);
					continue;
				}
				int num = 0;
				for(int i = 0; i < 128; ++i)
				{
					if(!service.id_skills[i]) continue;
					nt.npc_data.service_elf_learn_skill_list[num] = service.id_skills[i];
					num ++;
				}
				nt.npc_data.service_elf_learn_skill_num = num;
			}

			if(npc.id_engrave_service)
			{
				DATA_TYPE dt2;
				const NPC_ENGRAVE_SERVICE & service = *(const NPC_ENGRAVE_SERVICE*)dataman.get_data_ptr(npc.id_engrave_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_ENGRAVE_SERVICE))
				{
					__PRINTINFO("�����˴����engrave service %d ��NPC %d\n",npc.id_engrave_service,nt.tid);
					continue;
				}
				int num = 0;
				for(int i=0; i<16; i++)
				{
					if(!service.id_engrave[i]) continue;
					nt.npc_data.service_engrave.engrave_entrys[num] = service.id_engrave[i];
					num ++;
				}
				nt.npc_data.service_engrave.engrave_num = num;
			}

			if(npc.id_randprop_service)
			{
				DATA_TYPE dt2;
				const NPC_RANDPROP_SERVICE & service = *(const NPC_RANDPROP_SERVICE *)dataman.get_data_ptr(npc.id_randprop_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_RANDPROP_SERVICE))
				{
					__PRINTINFO("�����˴����addonregen service %d ��NPC %d\n",npc.id_randprop_service,nt.tid);
					continue;
				}
				int num = 0;
				for(int i=0; i<8; i++)
				{
					if(!service.pages[i].id_recipe) continue;
					nt.npc_data.service_addonregen.addonregen_entrys[num] = service.pages[i].id_recipe;
					num ++;
				}
				nt.npc_data.service_addonregen.addonregen_num = num;
			}
			if(npc.id_force_service)
			{
				DATA_TYPE dt;
				const NPC_FORCE_SERVICE * service = (const NPC_FORCE_SERVICE *)dataman.get_data_ptr(npc.id_force_service, ID_SPACE_ESSENCE,dt);
				if(dt != DT_NPC_FORCE_SERVICE || service == NULL)
				{
					__PRINTINFO("�����˴����force service %d ��NPC %d\n",npc.id_force_service, nt.tid);
					continue;
				}
				nt.npc_data.service_playerforce_tid = service->force_id;
			}
			if(npc.id_cross_server_service)
			{
				DATA_TYPE dt;
				const NPC_CROSS_SERVER_SERVICE * service = (const NPC_CROSS_SERVER_SERVICE*)dataman.get_data_ptr(npc.id_cross_server_service, ID_SPACE_ESSENCE,dt);
				if(dt != DT_NPC_CROSS_SERVER_SERVICE || service == NULL)
				{
					__PRINTINFO("�����˴����cross service %d ��NPC %d\n",npc.id_cross_server_service, nt.tid);
					continue;
				}
				nt.npc_data.service_cross.activity_type = service->activity_type;
				nt.npc_data.service_cross.player_count_limit = service->player_count_limit ;
				nt.npc_data.service_cross.time_out = service->time_out ;
				nt.npc_data.service_cross.need_item_tid = service->need_item_tid ;
				nt.npc_data.service_cross.need_item_count = service->need_item_count ;
				nt.npc_data.service_cross.cost_item = service->cost_item > 0;
				nt.npc_data.service_cross.history_max_level = service->history_max_level_require ;
				nt.npc_data.service_cross.second_level = service->taoist_rank_require ;
				nt.npc_data.service_cross.realm_level = service->realm_level_require ;

				nt.npc_data.service_change_ds_forward = 1;
			}
			if(npc.combined_services2 & 0x00000001)
			{
				nt.npc_data.service_country_management = 1;
			}
			if(npc.combined_services2 & 0x00000002)
			{
				nt.npc_data.service_countrybattle_leave = 1;
			}
			if(npc.combined_services2 & 0x00000004)
			{
				nt.npc_data.service_equip_sign = 1;
			}
//			if(npc.combined_services2 & 0x00000008)
//			{
//				nt.npc_data.service_change_ds_forward = 1;
//				��������id_cross_server_service �м���
//			}
			if(npc.combined_services2 & 0x00000010)
			{
				nt.npc_data.service_change_ds_backward = 1;
			}
			if(npc.combined_services2 & 0x00000020)
			{
				nt.npc_data.service_player_rename = 1;
			}

			if(npc.combined_services2 & 0x00000040)
			{
				nt.npc_data.service_addon_change = 1;
				nt.npc_data.service_addon_replace = 1;
			}
			if(npc.combined_services2 & 0x00000080)
			{
				nt.npc_data.service_kingelection = 1;
			}
			if(npc.combined_services2 & 0x00000100)
			{
				nt.npc_data.service_player_shop = 1;
			}
			if(npc.combined_services2 & 0x00000200)
			{
				nt.npc_data.service_decompose_fashion_item = 1;
			}
			if(npc.combined_services2 & 0x00000400)
			{
				nt.npc_data.service_reincarnation = 1;
			}
			if(npc.combined_services2 & 0x00000800)
			{
				nt.npc_data.service_giftcardredeem = 1;
			}
			if(npc.combined_services2 & 0x00001000)
			{
				nt.npc_data.service_trickbattle_apply = 1;
			}
			if(npc.combined_services2 & 0x00002000)
			{
				nt.npc_data.service_generalcard_rebirth = 1;
			}
			if(npc.combined_services2 & 0x00004000)
			{
				nt.npc_data.service_improve_flysword = 1;
			}
			if(npc.combined_services2 & 0x00008000)
			{
				nt.npc_data.service_mafia_pvp_signup = 1;
			}
			if(npc.combined_services2 & 0x00010000)
			{
				nt.npc_data.service_gold_shop = 1;
			}
			if(npc.combined_services2 & 0x00020000)
			{
				nt.npc_data.service_dividend_shop = 1;
			}
            if (npc.combined_services2 & 0x00040000)
            {
                nt.npc_data.service_player_change_gender = 1;
            }
            if (npc.combined_services2 & 0x00080000)
            {
                nt.npc_data.service_make_slot_for_decoration = 1;
            }
            if (npc.combined_services2 & 0x00100000)
            {
                nt.npc_data.service_select_solo_tower_challenge_stage = 1;
            }
            if (npc.combined_services2 & 0x00200000)
            {
                nt.npc_data.service_solo_challenge_rank = 1;
            }
            if (npc.combined_services2 & 0x00400000)
            {
                nt.npc_data.service_mnfaction_sign_up = 1;
            }
            if (npc.combined_services2 & 0x00800000)
            {
                nt.npc_data.service_mnfaction_award = 1;
            }
            if (npc.combined_services2 & 0x01000000)
            {
                nt.npc_data.service_mnfaction_rank = 1;
            }
            if (npc.combined_services2 & 0x02000000)
            {
                nt.npc_data.service_mnfaction_battle_transmit = 1;
            }
            if (npc.combined_services2 & 0x04000000)
            {
                nt.npc_data.service_mnfaction_join_leave = 1;
            }
            if (npc.combined_services2 & 0x08000000)
            {
                nt.npc_data.service_solo_challenge_rank_global = 1;
            }

			//Arena of Aurora

			if(npc.combined_services2 & 0x10000000)
			{
				nt.npc_data.service_arena_create_team = 1;
			}

			if(npc.combined_services2 & 0x20000000)
			{
				nt.npc_data.service_arena_join_team = 1;
			}

			if(npc.combined_services2 & 0x40000000)
			{
				nt.npc_data.service_arena_characters_view = 1;
			}

			if(npc.combined_services2 & 0x80000000)
			{
				nt.npc_data.service_arena_ranking_view = 1;
				nt.npc_data.service_arena_ranking_view_global = 1;
			}

			//End Arena of Aurora
			
			if(npc.id_pridictable_engrave_service)
			{
				DATA_TYPE dt2;
				const NPC_PRIDICTABLE_ENGRAVE_SERVICE & service = *(const NPC_PRIDICTABLE_ENGRAVE_SERVICE*)dataman.get_data_ptr(npc.id_pridictable_engrave_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_PRIDICTABLE_ENGRAVE_SERVICE))
				{
					continue;
				}
				
				int num = 0;
				for(unsigned int i=0; i < 20; i++)
				{
					for(unsigned int j=0; j < 32; j++)
					{
						if (service.pages[i].id_engraves[j])
						{
							nt.npc_data.pridictable_engrave_service.engrave_entrys[num++] = service.pages[i].id_engraves[j];
						}
					}
				}
				
				nt.npc_data.pridictable_engrave_service.engrave_num = num;
			}
			switch(npc.id_type)
			{
				case 3216:
				nt.npc_data.npc_type = npc_template::npc_statement::NPC_TYPE_GUARD;
				break;

				case 3214:
				default:
				//��ͨ
				nt.npc_data.npc_type = npc_template::npc_statement::NPC_TYPE_NORMAL;
				break;
			}

			if (npc.combined_services3 & 0x20) // serviço Submundo Frígido
			{
				nt.npc_data.storage_999_recv = 1;
			}

			if (npc.combined_services3 & 0xC0) // serviço Refino de livro todos os aciãos
			{
				nt.npc_data.service_bible_refine = 1;
				nt.npc_data.service_bible_refine_transmit = 1;
			}

			if (npc.combined_services3 & 0x100) // serviço Qualificação de Guild
			{
				// TODO
			}

			if (npc.combined_services3 & 0x200) // serviço Mestre aprendiz
			{
				// TODO 
			}

			if (npc.combined_services3 & 0x400) // serviço Cupido 
			{
				// TODO 
			}

			if (npc.combined_services3 & 0x800) // serviço Árvore do Carma 
			{
				// TODO
			}

			if (npc.combined_services3 & 0x1000) // serviço Captura Celestial (Caça Temporal)
			{
				// TODO
			}

			if (npc.combined_services3 & 0x2000) // serviço Guia do Aniquilador Imperial
			{
				// TODO
			}
			
			if (npc.combined_services3 & 0x3FFF) // serviço Menson de Pele Escura
			{
				// TODO
			}

			if (npc.combined_services3 & 0x4000) // serviço Defensor divino
			{
				// TODO
			}

			if (npc.combined_services3 & 0x8000) // serviço Reino do Norte
			{
				// TODO
			}

			if (npc.combined_services3 & 0x10000) // serviço Guarda Real Ramore ( Pareamento )
			{
				// TODO
			}

			if (npc.combined_services3 & 0x20000) /*170+ Bebê Celestial*/
			{
				nt.npc_data.service_kid_create = 1;
			}

			if(npc.combined_services3 & 0x48000) /*170+ Códice*/
			{
				nt.npc_data.service_dye_codex = 1;
				nt.npc_data.service_rename_codex = 1;	
			}

			if (npc.combined_services3 & 0x80000) 
			{
				nt.npc_data.service_material_refine = 1;
			}
			
			if (npc.combined_services3 & 0xA6000) // Maradona
			{
				// TODO 
			}

			if (npc.combined_services3 & 0x100000) // Reino do Norte
			{
				// TODO
			}

			if (npc.combined_services3 & 0x200000) // Teleporte ??
			{
				// TODO
			}	

			if (npc.combined_services3 & 0x2000C0) // Castelão
			{
				// TODO
			}

			if (npc.combined_services3 & 0x400000) // Gerente Personalizado de Jogo
			{
				// TODO
			}

			if (npc.combined_services3 & 0x800000) // Feng Zhenzhen
			{
				// TODO
			}

			if(npc.id_lib_produce_service)
			{
				DATA_TYPE dt2;
				const NPC_LIB_PRODUCE_SERVICE &service = *(const NPC_LIB_PRODUCE_SERVICE*)dataman.get_data_ptr(npc.id_lib_produce_service,ID_SPACE_ESSENCE,dt2);
				if(!(&service && dt2 == DT_NPC_LIB_PRODUCE_SERVICE))
				{
					__PRINTINFO("Lib Service %d Npc %d\n",npc.id_lib_produce_service,nt.tid);
					continue;
				}

				int num = 0;
				for(unsigned int j = 0; j < 16;j ++)
				{
						if(service.id_goods[j])
						{
							nt.npc_data.lib_service_produce.produce_items[num] = service.id_goods[j];
							num ++;
						}
				}			

				nt.npc_data.lib_service_produce.produce_num = num;		
			}
					
			Insert(nt);
		}
	}

	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		if(dt == DT_MINE_ESSENCE)
		{
			//�ǿ���
			const MINE_ESSENCE &mine=*(const MINE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&mine && dt == DT_MINE_ESSENCE);
			//ʹ�������ģ��
			npc_template nt;
			memset(&nt,0,sizeof(nt));
			nt.tid = mine.id;
			nt.mine_info.is_mine = 1;
			nt.mine_info.std_amount = mine.num1;
			nt.mine_info.bonus_amount = mine.num2;
			nt.mine_info.bonus_prop = mine.probability2;
			nt.mine_info.time_min = mine.time_min;
			nt.mine_info.time_max = mine.time_max;
			nt.mine_info.level = mine.level_required;
			nt.mine_info.exp = mine.exp;
			nt.mine_info.sp	 = mine.skillpoint;
			if( (nt.mine_info.std_amount <= 0 && 
				(!mine.task_in || !mine.task_out))||
				nt.mine_info.time_min > 1024 ||
				nt.mine_info.time_max > 1024 ||
				nt.mine_info.time_min > nt.mine_info.time_max ||
				nt.mine_info.time_max <= 0 
				)
			{
				__PRINTINFO("����Ŀ��������ID:%d\n",nt.tid);
				continue;
			}
			
			float prop = 0.f;
			int kinds = 0;
			for(unsigned int i = 0; i < 16; i ++)
			{
				if(mine.materials[i].probability > 0.f)
				{
					nt.mine_info.id_produce[kinds] = mine.materials[i].id;
					nt.mine_info.id_produce_prop[kinds] = mine.materials[i].probability;
					nt.mine_info.id_produce_life[kinds] = mine.materials[i].life;
					prop += mine.materials[i].probability;
					kinds ++;
				}
			}
			nt.mine_info.produce_kinds = kinds;
			if(fabs(prop - 1.0f) >= 1e-5 || !nt.mine_info.produce_kinds)
			{
				__PRINTINFO("����Ŀ�������ĸ���,����IDΪ%d\n",nt.tid);
				continue;
			}
			if ((mine.task_in && !mine.task_out) || (!mine.task_in && mine.task_out))
			{
				__PRINTINFO("����������%d\n",nt.tid);
				continue;
			}

			nt.mine_info.need_equipment = mine.id_equipment_required;
			nt.mine_info.task_in = mine.task_in;
			nt.mine_info.task_out = mine.task_out;
			nt.mine_info.no_interrupted = mine.uninterruptable;
			nt.mine_info.gather_no_disappear = mine.permenent;
			nt.mine_info.eliminate_tool = mine.eliminate_tool;
			nt.mine_info.ask_help_faction = mine.aggros[0].monster_faction;
			nt.mine_info.ask_help_range = mine.aggros[0].radius;
			nt.mine_info.ask_help_aggro = mine.aggros[0].num;
			if(nt.mine_info.ask_help_range > 30) nt.mine_info.ask_help_range = 30.f;
			nt.mine_info.set_owner = mine.combined_switch & MCS_MINE_BELONG_TO_SOMEONE;
			nt.mine_info.broadcast_on_gain = mine.combined_switch & MCS_MINE_BROADCAST_ON_GAIN;
			nt.mine_info.gather_dist = mine.gather_dist;
			if (nt.mine_info.gather_dist < 4.f)	nt.mine_info.gather_dist = 4.f;
			if (nt.mine_info.gather_dist > 20.f) nt.mine_info.gather_dist = 20.f;
			nt.mine_info.gather_player_max = mine.max_gatherer;
			if (nt.mine_info.gather_player_max < 1) nt.mine_info.gather_player_max = 1;
			if (nt.mine_info.gather_player_max > 20) nt.mine_info.gather_player_max = 20;
			nt.mine_info.success_prob = mine.material_gain_ratio;
			if (nt.mine_info.success_prob <= 0.f)
			{
				__PRINTF("����ĳɹ��ɼ�����Ϊ0. id=%d\n",nt.tid);
				continue;
			}
			nt.mine_info.mine_type = mine.mine_type;
			if (nt.mine_info.mine_type < 0 || nt.mine_info.mine_type > 1)
			{
				__PRINTF("����Ŀ�������%d id=%d.\n",nt.mine_info.mine_type,nt.tid);
				continue;
			}
			
			for(unsigned int i = 0; i < 4; i ++)
			{
				nt.mine_info.monster_list[i].id_monster = mine.npcgen[i].id_monster;
				nt.mine_info.monster_list[i].num = mine.npcgen[i].num;
				nt.mine_info.monster_list[i].radius = mine.npcgen[i].radius;
				nt.mine_info.monster_list[i].remain_time = mine.npcgen[i].life_time;
			}
			Insert(nt);
		}
	}

	return true;
}

bool
recipe_manager::LoadTemplate(itemdataman & dataman)
{
	//��ȡ�����䷽
	DATA_TYPE  dt;
	unsigned int id = dataman.get_first_data_id(ID_SPACE_RECIPE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_RECIPE,dt))
	{
		if(dt == DT_RECIPE_ESSENCE)
		{
			const RECIPE_ESSENCE &ess = *(const RECIPE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
			ASSERT(&ess && dt == DT_RECIPE_ESSENCE);
			
			recipe_template rt;
			memset(&rt,0,sizeof(rt));
			
			rt.recipe_id = id;
			rt.fee = ess.price;
			rt.produce_skill = ess.id_skill;
			rt.require_level = ess.skill_level;
			rt.count = ess.num_to_make;
			rt.level = MAX_PLAYER_LEVEL;
			rt.exp = ess.exp;
			rt.sp = ess.skillpoint;
			rt.recipe_level = ess.recipe_level;
			rt.null_prob = ess.fail_probability;
			rt.bind_type = ess.bind_type;
			rt.proc_type = ess.proc_type;
			rt.equipment_need_upgrade = ess.id_upgrade_equip;
			rt.inherit_fee_factor = ess.upgrade_rate;
			rt.inherit_engrave_fee_factor = ess.engrave_upgrade_rate;
		    rt.inherit_addon_fee_factor = ess.addon_inherit_fee_rate;

			// Novos
			rt.require_arena_personal_point = ess.arena_personal_point;
			rt.is_recipe_card = ess.poker_recipe;
			rt.unk = ess.camp_level_limit;
            
			for(unsigned int j = 0; j < 4;j ++)
			{
				rt.targets[j].id = ess.targets[j].id_to_make;
				rt.targets[j].prob = ess.targets[j].probability;
			}

			int use_time = (int)(20*ess.duration);
			if(use_time<= 0) use_time = 1;
			if(use_time > 10000) use_time = 10000;
			rt.use_time = use_time;

			std::set<int > unique_id;
			unsigned int total_meterail_count = 0;
			if(ess.bind_type == 0)
			{
				unsigned int num = 0;
				for(int i = 0; i < 32; ++i)
				{
					if(!ess.materials[i].id) continue;
					unsigned int meterail_count = ess.materials[i].num;
					if(!meterail_count)
					{
						__PRINTINFO("������䷽ %d(����%d) ,ԭ����ĿΪ0\n",ess.id,ess.targets[0].id_to_make);
						continue;
					}
					if(! unique_id.insert(ess.materials[i].id).second )
					{
						__PRINTINFO("�䷽ %d,������ͬ%d\n",ess.id,ess.materials[i].id);
					}
					total_meterail_count += meterail_count;
					rt.material_list[num].item_id = ess.materials[i].id;
					rt.material_list[num].count = meterail_count;
					num ++;
				}
				rt.material_num = num;
			}
			else
			{
				for(int i = 0; i < 32; ++i)
				{
					rt.material_list[i].item_id = ess.materials[i].id;
					rt.material_list[i].count = ess.materials[i].num;
					unsigned int meterail_count = ess.materials[i].num;
					if(!meterail_count)
					{
						//__PRINTINFO("������䷽ %d(����%d) ,ԭ����ĿΪ0\n",ess.id,ess.targets[0].id_to_make);
						continue;
					}
					if(! unique_id.insert(ess.materials[i].id).second )
					{
						__PRINTINFO("�䷽ %d,������ͬ%d\n",ess.id,ess.materials[i].id);
					}
					total_meterail_count += meterail_count;
				}
				rt.material_num = 16;
			}

		/*
			//���ڶĲ������޲�����
			if(!total_meterail_count)
			{
				__PRINTINFO("������䷽%d(����%d) ����ԭ����ĿΪ0\n", ess.id,ess.targets[0].id_to_make);
				continue;
			}
			*/
			
			//����һ��
			float p = rt.targets[0].prob+rt.targets[1].prob+rt.targets[2].prob+rt.targets[3].prob;
			if(fabs(p -1.0f) > 1e-6)
			{
				__PRINTINFO("������䷽%d(����%d) ����δ��һ��\n", ess.id);
				continue;
			}
			
			if(!Insert(rt))
			{
				__PRINTINFO("�����䷽%d(����%d)�����ظ�����\n", ess.id,ess.targets[0].id_to_make);
			}
		}
		else if(dt == DT_ENGRAVE_ESSENCE)
		{
			const ENGRAVE_ESSENCE &ess = *(const ENGRAVE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
			ASSERT(&ess && dt == DT_ENGRAVE_ESSENCE);

			engrave_recipe_template ert;
			memset(&ert,0,sizeof(ert));
			
			ert.recipe_id 		= id;
			ert.engrave_level 	= ess.level;
			ert.equip_mask 		= ess.equip_mask;
			ert.equip_level_min = ess.require_level_min;
			ert.equip_level_max = ess.require_level_max;
			ert.use_time 		= ess.duration*TICK_PER_SEC;
			if(ert.use_time <= 0) ert.use_time = 1;
			if(ert.use_time > 10000) ert.use_time = 10000;
			ASSERT(sizeof(ert.prob_addon_num) == sizeof(ess.probability_addon_num));
			memcpy(ert.prob_addon_num,ess.probability_addon_num,sizeof(ert.prob_addon_num));
			int total_meterail_count = 0;
			int num = 0;
			std::set<int > unique_id;
			for(int i=0; i<8; i++)
			{
				if(!ess.materials[i].id) continue;
				int meterail_count = ess.materials[i].num;
				if(meterail_count <= 0)
				{
					__PRINTINFO("������Կ��䷽ %d,ԭ����ĿС�ڵ���0\n",ess.id);
					continue;
				}
				if(! unique_id.insert(ess.materials[i].id).second )
				{
					__PRINTINFO("�Կ��䷽ %d,������ͬ%d\n",ess.id,ess.materials[i].id);
				}
				total_meterail_count += meterail_count;
				ert.material_list[num].item_id = ess.materials[i].id;
				ert.material_list[num].count = meterail_count;
				num ++;
			}
			if(!total_meterail_count)
			{
				__PRINTINFO("������Կ��䷽ %d����ԭ����ĿΪ0\n", ess.id);
				continue;
			}
			ert.material_num = num;
			ert.material_total_count = total_meterail_count;
			for(int i=0; i<32; i++)
			{
				ert.target_addons[i].addon_id = ess.addons[i].id;
				ert.target_addons[i].prob = ess.addons[i].probability;
			}

			//�����ʹ�һ��
			float p1 = 0.f, p2 = 0.f;
			for(int i=0; i<4; i++)	p1 += ert.prob_addon_num[i];
			for(int i=0; i<32; i++) p2 += ert.target_addons[i].prob;
			if(fabs(p1 -1.0f) > 1e-6 || fabs(p2 -1.0f) > 1e-6)
			{
				__PRINTINFO("������Կ��䷽%d ����δ��һ��\n", ess.id);
				continue;
			}

			if(!Insert(ert))
			{
				__PRINTINFO("�����Կ��䷽%d�����ظ�����\n", ess.id);
			}
		}
		else if(dt == DT_RANDPROP_ESSENCE)
		{
			const RANDPROP_ESSENCE & ess = *(const RANDPROP_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
			ASSERT(&ess && dt == DT_RANDPROP_ESSENCE);

			addonregen_recipe_template arrt;
			memset(&arrt, 0, sizeof(arrt));

			arrt.recipe_id  	= id;
			arrt.produce_skill	= ess.id_skill;
			arrt.require_level	= ess.skill_level;
			arrt.use_time		= ess.duration*TICK_PER_SEC;
			if(arrt.use_time <= 0) arrt.use_time = 1;
			if(arrt.use_time > 10000) arrt.use_time = 10000;
			arrt.fee			= ess.money;
			ASSERT(sizeof(arrt.equip_id_list) == sizeof(ess.equip_id));
			memcpy(arrt.equip_id_list,ess.equip_id,sizeof(arrt.equip_id_list));
			int total_meterail_count = 0;
			int num = 0;
			std::set<int> unique_id;
			for(int i=0; i<8; i++)
			{
				if(!ess.materials[i].id) continue;
				int meterail_count = ess.materials[i].num;
				if(meterail_count <= 0)
				{
					__PRINTINFO("��������������䷽ %d,ԭ����ĿС�ڵ���0\n",ess.id);
					continue;
				}
				if(! unique_id.insert(ess.materials[i].id).second )
				{
					__PRINTINFO("���������䷽ %d,������ͬ%d\n",ess.id,ess.materials[i].id);
				}
				total_meterail_count += meterail_count;
				arrt.material_list[num].item_id = ess.materials[i].id;
				arrt.material_list[num].count = meterail_count;
				num ++;
			}
			if(!total_meterail_count)
			{
				__PRINTINFO("��������������䷽ %d����ԭ����ĿΪ0\n", ess.id);
				continue;
			}
			arrt.material_num = num;
			arrt.material_total_count = total_meterail_count;
		
			if(!Insert(arrt))
			{
				__PRINTINFO("�������������䷽%d�����ظ�����\n", ess.id);
			}
		}
		else if(dt == DT_STONE_CHANGE_RECIPE)
		{
			const STONE_CHANGE_RECIPE & ess = *(const STONE_CHANGE_RECIPE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
			ASSERT(&ess && dt == DT_STONE_CHANGE_RECIPE);

			addonchange_recipe_template acrt;
			memset(&acrt, 0, sizeof(acrt));

			acrt.recipe_id  	= id;
			acrt.fee		= ess.money >= 0 ? ess.money : 0;
			acrt.id_old_stone	= ess.id_old_stone;
			acrt.id_new_stone 	= ess.id_new_stone;

			if(0 >= acrt.id_old_stone || 0 >= acrt.id_new_stone)
			{
				__PRINTINFO("����Ļ�ʯת���䷽ %d����ת����ʯid ���ִ���\n", ess.id);
				continue;
			}

			int total_meterail_count = 0;
			int num = 0;
			std::set<int> unique_id;

			for(int i=0; i<8; i++)
			{
				if(!ess.materials[i].id) continue;
				int meterail_count = ess.materials[i].num;
				if(meterail_count <= 0)
				{
					__PRINTINFO("����Ļ�ʯת���䷽ %d,ԭ����ĿС�ڵ���0\n",ess.id);
					continue;
				}
				if(! unique_id.insert(ess.materials[i].id).second )
				{
					__PRINTINFO("��ʯת���䷽ %d,������ͬ%d\n",ess.id,ess.materials[i].id);
				}

				total_meterail_count += meterail_count;
				acrt.material_list[num].item_id = ess.materials[i].id;
				acrt.material_list[num].count = meterail_count;
				num ++;
			}
			if(!total_meterail_count)
			{
				__PRINTINFO("����Ļ�ʯת���䷽ %d����ԭ����ĿΪ0\n", ess.id);
				continue;
			}
			acrt.material_num = num;
		
			if(!Insert(acrt))
			{
				__PRINTINFO("�����ʯת���䷽%d�����ظ�����\n", ess.id);
			}
		} else if(dt == DT_EASY_PRODUCE_ITEM_ESSENCE)
		{
			const EASY_PRODUCE_ITEM_ESSENCE &ess = *(const EASY_PRODUCE_ITEM_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_EASY_PRODUCE_ITEM_ESSENCE);
			
			easy_recipe_template rt;
			memset(&rt,0,sizeof(rt));
			
			rt.recipe_id = id;
			rt.enable_ui = ess.enable_produce_ui;
			rt.use_count = ess.produce_times;
			rt.unk1 = ess.unk1;
            
			for(unsigned int j = 0; j < 6;j ++)
			{
				rt.targets[j].id = ess.targets[j].id_to_make;
				rt.targets[j].count = ess.targets[j].num_to_make;
				rt.targets[j].prob = ess.targets[j].probability;
			}

			int use_time = (int)(20*ess.duration);
			if(use_time<= 0) use_time = 1;
			if(use_time > 10000) use_time = 10000;
			rt.use_time = use_time;

			std::set<int > unique_id;
			unsigned int total_meterail_count = 0;

			unsigned int num = 0;		
			for(unsigned int i = 0; i < 10;i ++)
			{
				if(ess.materials[i].id) {
				
					rt.material_list[num].item_id = ess.materials[i].id;
					rt.material_list[num].count = ess.materials[i].num;	

					total_meterail_count += rt.material_list[i].count;
					num ++;		
				}				
			}

			rt.material_num = num;
			rt.material_total_count = total_meterail_count;
			
			if(!Insert(rt))
			{
			}

		} else if(dt == DT_LIB_PRODUCE_RECIPE)
		{
			const LIB_PRODUCE_RECIPE &ess = *(const LIB_PRODUCE_RECIPE*)dataman.get_data_ptr(id,ID_SPACE_RECIPE,dt);
			ASSERT(&ess && dt == DT_LIB_PRODUCE_RECIPE);
			
			lib_recipe_template rt;
			memset(&rt,0,sizeof(rt));
			
			rt.recipe_id = id;
			rt.fee = ess.produce_consume_money;
			rt.consume_point = ess.produce_consume_point;
            
			for(unsigned int j = 0; j < 64;j ++)
			{
				rt.targets[j].id = ess.targets[j].id;
				rt.targets[j].count = ess.targets[j].num;
				rt.targets[j].prob = ess.targets[j].probability;
			}

			int use_time = (int)(20*ess.duration);
			if(use_time<= 0) use_time = 1;
			if(use_time > 10000) use_time = 10000;
			rt.use_time = use_time;

			std::set<int > unique_id;
			unsigned int total_meterail_points = 0;

			unsigned int num = 0;		
			for(unsigned int i = 0; i < 256;i ++)
			{
				if(ess.materials[i].id) {
				
					rt.material_list[num].item_id = ess.materials[i].id;
					rt.material_list[num].point = ess.materials[i].point;	

					total_meterail_points += rt.material_list[num++].point;		
				}				
			}

			rt.material_num = num;
			rt.material_total_points = total_meterail_points;
			
			if(!Insert(rt))
			{
			}
		} 


	}

	__PRINTINFO("�ܹ�������%d�������䷽\n",__GetInstance()._rt_map.size());

	//��ʼ�������䷽
	unsigned int num = dataman.get_data_num(ID_SPACE_ESSENCE);
	ASSERT(num);
	int decompose_num_match = 0;
	int decompose_num_nomatch = 0;
	id = dataman.get_first_data_id(ID_SPACE_ESSENCE,dt);
	for(; id != 0; id = dataman.get_next_data_id(ID_SPACE_ESSENCE,dt))
	{
		int element_id = 0;
		unsigned int element_num = 0;
		unsigned int decompose_fee =0;
		unsigned int decompose_time = 0;
		if(dt == DT_WEAPON_ESSENCE)
		{	
			const WEAPON_ESSENCE &ess= *(WEAPON_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_WEAPON_ESSENCE);
			if(!&ess || dt != DT_WEAPON_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}
		else
		if(dt == DT_ARMOR_ESSENCE)
		{	
			const ARMOR_ESSENCE &ess= *(ARMOR_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_ARMOR_ESSENCE);
			if(!&ess || dt != DT_ARMOR_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}
		else
		if(dt == DT_NEW_ARMOR_ESSENCE) // G17/G18
		{	
			const NEW_ARMOR_ESSENCE &ess= *(NEW_ARMOR_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_NEW_ARMOR_ESSENCE);
			if(!&ess || dt != DT_NEW_ARMOR_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}
		else
		if(dt == DT_DECORATION_ESSENCE)
		{	
			const DECORATION_ESSENCE &ess= *(DECORATION_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_DECORATION_ESSENCE);
			if(!&ess || dt != DT_DECORATION_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}
		else		
		if(dt == DT_MATERIAL_ESSENCE)
		{
			const MATERIAL_ESSENCE &ess=*(MATERIAL_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_MATERIAL_ESSENCE);
			if(!&ess || dt != DT_MATERIAL_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}
		else 
		if(dt == DT_ELEMENT_ESSENCE)
		{
			const ELEMENT_ESSENCE &ess=*(ELEMENT_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_ELEMENT_ESSENCE);
			if(!&ess || dt != DT_ELEMENT_ESSENCE) continue;
			if(!(ess.level > 0 && ess.level <=15))
			{
				__PRINTINFO("%dԪʯ����%d \n",id,ess.level);
			}
			//ASSERT(ess.level > 0 && ess.level <=15);
		}
		else
		if(dt == DT_FLYSWORD_ESSENCE)
		{
			const FLYSWORD_ESSENCE &ess=*(FLYSWORD_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_FLYSWORD_ESSENCE);
			if(!&ess || dt != DT_FLYSWORD_ESSENCE) continue;
			if(!(ess.level > 0 && ess.level <=15))
			{
				__PRINTINFO("%d�ɽ�����%d '%s'\n",id,ess.level,ess.file_model);
			}
			//ASSERT(ess.level > 0 && ess.level <=15);
		}
		else 
		if(dt == DT_BIBLE_ESSENCE)
		{	
			const BIBLE_ESSENCE &ess= *(BIBLE_ESSENCE*)dataman.get_data_ptr(id,ID_SPACE_ESSENCE,dt);
			ASSERT(&ess && dt == DT_BIBLE_ESSENCE);
			if(!&ess || dt != DT_BIBLE_ESSENCE) continue;
			element_id = ess.element_id;
			decompose_fee = ess.decompose_price;
			decompose_time = ess.decompose_time ;
			element_num = ess.element_num ;
		}


		if(!element_id || !element_num) continue;
		if(decompose_time < 1) decompose_time =1;
		if(decompose_time > 500) decompose_time = 500;

		recipe_template * rt = GetByItemID(id);
		if(rt == NULL)
		{
			decompose_num_nomatch ++;
			//����û�е��䷽
			__PRINTINFO("ɾ����û�������䷽�Ĳ����� %d\n",id);
			continue;
		}
		decompose_recipe_template drt;
		memset(&drt,0,sizeof(drt));
		
		drt.id = id;
		drt.decompose_fee = decompose_fee;
		drt.decompose_time = decompose_time*20;
		drt.element_num = element_num;
		drt.element_id = element_id;
		drt.produce_skill = rt->produce_skill;
		drt.require_level = rt->require_level;
		drt.recipe_level =  drt.recipe_level;

		if(!Insert(drt))
		{
			ASSERT(false);
		}

		decompose_num_match ++ ;
		//__PRINTINFO("��������Ϣ���䷽%d(%d %d %d %d)\n",id,element_id,element_num,decompose_time,decompose_fee);
	}
	__PRINTINFO("�Զ�ƥ����%d����䷽,����%d��װ��û�п���ƥ����䷽\n",decompose_num_match,decompose_num_nomatch);
	return true;
}

namespace
{
	enum
	{
		NPC_ADDON_EMPTY,		//��
		NPC_ADDON_SPEED_UP,             //��ë��
		NPC_ADDON_DONEY,                //����
		NPC_ADDON_DOUBLE_ARMOR,         //˫������
		NPC_ADDON_MAGIC_MIRROR,         //ħ����
		NPC_ADDON_POWER_OF_BEAR,        //��֮��
		NPC_ADDON_VOODOO,               //����ʦ
		NPC_ADDON_ASSAULT,		//����ͻ��
		NPC_ADDON_VIGOROUS, 		//��Ѫ
		NPC_ADDON_LAMB, 		//��С
		MAX_ADDON_TYPE = NPC_ADDON_LAMB
	};
	
	inline void IncExpAndDrop(gnpc_imp * pImp,float exp_scale, float money_scale)
	{
		int exp = (int)(pImp->_basic.exp * exp_scale);
		if(exp <=0) exp = 1;
		int sp = (int)(pImp->_basic.skill_point * exp_scale);
		if(sp <=0) sp = 0;
		pImp->_basic.exp = exp;
		pImp->_basic.skill_point = sp;
		pImp->_money_scale = money_scale;
	}


	void AddonSpeedUp(gnpc_imp * pImp)
	{
		pImp->_base_prop.walk_speed	= (pImp->_cur_prop.walk_speed *= 1.5f);
		pImp->_base_prop.run_speed 	= (pImp->_cur_prop.run_speed *= 1.5f);
		pImp->_base_prop.flight_speed 	= (pImp->_cur_prop.flight_speed *= 1.5f);
		pImp->_base_prop.swim_speed 	= (pImp->_cur_prop.swim_speed *= 1.5f);
		IncExpAndDrop(pImp,2.0f,2.0f);
	}

	void AddonDonkey(gnpc_imp * pImp)
	{
		//not imp yet
	}

	void AddonDoubleArmor(gnpc_imp * pImp)
	{
		pImp->_base_prop.defense = (pImp->_cur_prop.defense *= 3);
		IncExpAndDrop(pImp,2.0f,2.0f);
	}

	void AddonMagicMirror(gnpc_imp * pImp)
	{
		for(unsigned int i = 0; i < MAGIC_CLASS; i++)
		{
			pImp->_base_prop.resistance[i] = (pImp->_cur_prop.resistance[i] *= 10);
		}
		IncExpAndDrop(pImp,2.0f,2.0f);
	}
	
	void AddonMagicPowerOfBear(gnpc_imp * pImp)
	{
		pImp->_base_prop.damage_low = (pImp->_cur_prop.damage_low += pImp->_cur_prop.damage_low >> 1);
		pImp->_base_prop.damage_high = (pImp->_cur_prop.damage_high += pImp->_cur_prop.damage_high >> 1);
		IncExpAndDrop(pImp,1.5f,1.5f);
	}

	void AddonVoodoo(gnpc_imp * pImp)
	{
		pImp->_base_prop.damage_magic_low = (pImp->_cur_prop.damage_magic_low += pImp->_cur_prop.damage_magic_low >> 1);
		pImp->_base_prop.damage_magic_high = (pImp->_cur_prop.damage_magic_high += pImp->_cur_prop.damage_magic_high >> 1);
		IncExpAndDrop(pImp,1.5f,1.5f);
	}

	void AddonAssault(gnpc_imp * pImp)
	{
		pImp->_base_prop.max_hp = (pImp->_cur_prop.max_hp >>= 1 );
		pImp->_basic.hp = pImp->_base_prop.max_hp;
		pImp->_base_prop.damage_low = (pImp->_cur_prop.damage_low *= 2);
		pImp->_base_prop.damage_high = (pImp->_cur_prop.damage_high *= 2);
		pImp->_base_prop.damage_magic_low = (pImp->_cur_prop.damage_magic_low *= 2);
		pImp->_base_prop.damage_magic_high = (pImp->_cur_prop.damage_magic_high *= 2);
		IncExpAndDrop(pImp,1.5f,1.5f);
	}

	void AddonVigorous(gnpc_imp * pImp)
	{
		pImp->_base_prop.max_hp = (pImp->_cur_prop.max_hp *= 2);
		pImp->_basic.hp = pImp->_base_prop.max_hp;
		IncExpAndDrop(pImp,2.f,2.f);
	}

	void AddonLamb(gnpc_imp * pImp)
	{
		pImp->_base_prop.max_hp = (pImp->_cur_prop.max_hp >>= 1);
		pImp->_basic.hp = pImp->_base_prop.max_hp;
		pImp->_base_prop.damage_low = (pImp->_cur_prop.damage_low >>= 1);
		pImp->_base_prop.damage_high = (pImp->_cur_prop.damage_high >>= 1);
		pImp->_base_prop.damage_magic_low = (pImp->_cur_prop.damage_magic_low >>= 1);
		pImp->_base_prop.damage_magic_high = (pImp->_cur_prop.damage_magic_high >>= 1); 
		IncExpAndDrop(pImp,0.5f,0.5f);
	}

	void AddonEmpty(gnpc_imp * pImp)
	{
	}

	void (*AddonHandler[MAX_ADDON_TYPE + 1])(gnpc_imp * pImp) = 
	{
		AddonEmpty, AddonSpeedUp, AddonDonkey, AddonDoubleArmor,
		AddonMagicMirror, AddonMagicPowerOfBear, AddonVoodoo,
		AddonAssault, AddonVigorous, AddonLamb,
	};
	
	void AdjustNPCAddon(gnpc_imp * pImp, int addon_type)
	{
		ASSERT(addon_type >= 0 && addon_type <= MAX_ADDON_TYPE);
	/*	if(addon_type != 0)
		{
			A3DVECTOR &pos = pImp->_parent->pos;
			__PRINTINFO("��������������addon %d (%f,%f,%f)\n",addon_type,pos.x,pos.y,pos.z);
		}*/
		((gnpc*)pImp->_parent)->object_state &= ~(gactive_object::STATE_NPC_ALLADDON);
		((gnpc*)pImp->_parent)->object_state |= (addon_type & 0x0F) << 8;
		(*AddonHandler[addon_type])(pImp);
	}

};

void 
npc_spawner::AdjustPropByCommonValue(gnpc_imp * pImp, world * pPlane, npc_template * pTemplate)
{
	if(pTemplate->hp_adjust_common_value)
	{
		int hp_adjust = pPlane->GetCommonValue(pTemplate->hp_adjust_common_value);
		if(hp_adjust < -90 || hp_adjust > 5000) hp_adjust = 0;
		if(hp_adjust)
		{
			pImp->_cur_prop.max_hp = (int)(pImp->_cur_prop.max_hp * 0.01f * (100 + hp_adjust));
            if (pImp->_cur_prop.max_hp < 0) pImp->_cur_prop.max_hp = INT_MAX;

			pImp->_base_prop.max_hp = pImp->_cur_prop.max_hp;
			pImp->_basic.hp = pImp->_cur_prop.max_hp;
		}
	}
	if(pTemplate->defence_adjust_common_value)
	{
		int defence_adjust = pPlane->GetCommonValue(pTemplate->defence_adjust_common_value);
		if(defence_adjust < -90 || defence_adjust > 5000) defence_adjust = 0;
		if(defence_adjust)
		{
			pImp->_cur_prop.defense = (int)(pImp->_cur_prop.defense * 0.01f * (100 + defence_adjust));
			pImp->_base_prop.defense = pImp->_cur_prop.defense;
			for(int i=0; i<MAGIC_CLASS; i++)
			{
				pImp->_cur_prop.resistance[i] = (int)(pImp->_cur_prop.resistance[i] * 0.01f * (100 + defence_adjust));
				pImp->_base_prop.resistance[i] = pImp->_cur_prop.resistance[i];
			}
		}
	}
	if(pTemplate->attack_adjust_common_value)
	{
		int attack_adjust = pPlane->GetCommonValue(pTemplate->attack_adjust_common_value);
		if(attack_adjust < -90 || attack_adjust > 5000) attack_adjust = 0;
		if(attack_adjust)
		{
			pImp->_cur_prop.damage_low = (int)(pImp->_cur_prop.damage_low * 0.01f * (100 + attack_adjust));
			pImp->_base_prop.damage_low = pImp->_cur_prop.damage_low;
			pImp->_cur_prop.damage_high = (int)(pImp->_cur_prop.damage_high * 0.01f * (100 + attack_adjust));
			pImp->_base_prop.damage_high = pImp->_cur_prop.damage_high;
			pImp->_cur_prop.damage_magic_low= (int)(pImp->_cur_prop.damage_magic_low* 0.01f * (100 + attack_adjust));
			pImp->_base_prop.damage_magic_low= pImp->_cur_prop.damage_magic_low;
			pImp->_cur_prop.damage_magic_high= (int)(pImp->_cur_prop.damage_magic_high* 0.01f * (100 + attack_adjust));
			pImp->_base_prop.damage_magic_high= pImp->_cur_prop.damage_magic_high;
		}
	}
}

void 
npc_spawner::RegenAddon(gnpc_imp * pImp, int npc_id)
{
	npc_template * pTemplate = npc_stubs_manager::Get(npc_id);
	if(!pTemplate) return;
	pImp->_money_scale = 1.0f;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_cur_item.attack_range = pTemplate->ep.attack_range;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	pImp->_cur_prop = pImp->_base_prop;
	pImp->_cur_item.weapon_type = pTemplate->short_range_mode;	//��������(Զ�̣�����)
	if(pTemplate->short_range_mode) pImp->_cur_item.short_range = 4.0f;
	AdjustPropByCommonValue(pImp, pImp->_plane, pTemplate);

	//����npc���������
	if(pTemplate->addon_choice_count > 1)
	{
		int index = abase::RandSelect(pTemplate->probability_addon,pTemplate->addon_choice_count);
		AdjustNPCAddon(pImp, pTemplate->id_addon[index]);
	}
	else
	{
		//ֻ��һ��
		AdjustNPCAddon(pImp,pTemplate->id_addon[0]);
	}
}

gnpc * 
npc_spawner::CreatePetBase(gplayer_imp * pMaster, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode, const int cid[3],
				unsigned char dir,  unsigned char pet_stamp, 
				int ai_policy_cid, int aggro_policy)
{
	world * pPlane = pMaster->_plane;
	int player_faction = pMaster->GetFaction();
	int player_enemy_faction = pMaster->GetEnemyFaction();

	const pet_data_temp * pTmp = pet_dataman::Get(pData->pet_tid);
	if(pTmp == NULL) 
	{
		GLog::log(GLOG_ERR,"Invalid pet template id %d",pData->pet_tid);
		return NULL;
	}

	gnpc * pNPC = NULL;
	pNPC = pPlane->AllocNPC();
	if(!pNPC) return NULL;
	pNPC->ID.type = GM_TYPE_NPC;
	pNPC->ID.id= MERGE_PET_ID(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = 0;
	pNPC->idle_timer = 100;	
	pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = pData->pet_tid; 
	pNPC->vis_tid = pData->pet_vis_tid; 
	if(!pNPC->vis_tid)
	{
		pNPC->vis_tid = pData->pet_tid; 
	}
	pNPC->monster_faction = player_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);

	//Set pet basic properties
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gpet_imp)));
	gpet_imp * pImp = (gpet_imp *)pNPC->imp;
	pImp->SetPetStamp(pet_stamp);
	pImp->SetAttackHook(	pMaster->GetPetAttackHook(), pMaster->GetPetEnchantHook(),
				pMaster->GetPetAttackFill(), pMaster->GetPetEnchantFill());

	pImp->InitFromMaster(pMaster);
	pImp->SetTID(pData->pet_tid);

	int plevel = pData->level;
	
	extend_prop prop;
	pet_dataman::GenerateBaseProp(pData->pet_tid, plevel,prop);

	pImp->_basic.level = plevel; 
	pImp->_basic.sec.value = 0;
	pImp->_basic.exp = 0;		//This experience is not the pet's current experience
	pImp->_basic.skill_point = 0;	//This skill point is not the pet's current skill point
	pImp->_basic.hp = (int)(prop.max_hp * pData->hp_factor);
	if(pImp->_basic.hp <= 0) pImp->_basic.hp = 1;
	
	pImp->_base_prop = prop;
	pImp->_cur_prop = pImp->_base_prop;
	pImp->_cur_prop.attack_range += pTmp->body_size;
	pImp->_cur_item.attack_range = pImp->_base_prop.attack_range;
	pImp->_cur_item.weapon_type = 0;	//Weapon type (long range, close range)
	pImp->_spawner = NULL;
	pImp->_faction = player_faction;
	pImp->_enemy_faction = player_enemy_faction;
	pImp->_inhabit_type = pTmp->inhabit_type;
	pImp->SetInhabitMode(inhabit_mode);
	pImp->_damage_delay = pTmp->damage_delay;
	pImp->_immune_state = pTmp->immune_type;
	pImp->_corpse_delay = 0;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTmp->body_size;
	pImp->SetVigourBase( pMaster->GetVigour() );

	//Does not regenerate blood quickly
	pImp->SetFastRegen(0);

	//Create AI-controlled objects No threat strategy introduced yet
	if(ai_policy_cid)
	{

		aggro_param aggp;
		ai_param aip;
		memset(&aggp,0,sizeof(aggp));
		memset(&aip,0,sizeof(aip));
		aip.trigger_policy = 0;	//Pets do not have this trigger control, is it possible to consider using it?
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = 60.f;	//The data of the pet is written to death
		aggp.aggro_time = 100; 
		aggp.sight_range = pTmp->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = false;		//Pets do not help others, helping the owner is done by the owner's logic
		aggp.faction_accept_help = false;

		aip.policy_class = ai_policy_cid;

		aip.patrol_mode = false;
		//Here you need to fill in a strategy, the default strategy of pet unity is physical attack
		aip.primary_strategy = ai_policy::STRATEGY_MELEE; 
		//aip.primary_strategy = ai_policy::STRATEGY_MELEE_MAGIC; 

		//Verify and replicate skills
		for(unsigned int j = 0; j < pet_data::MAX_PET_SKILL_COUNT; j ++)
		{
			int id_skill = pData->skills[j].skill;
			int lvl_skill= pData->skills[j].level;
			if(id_skill <= 0) break;

			//add skills
			pImp->AddSkill(id_skill,lvl_skill);
		}

		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);
		
		//Initialize the skills of plants, it must be done after the initialization of AI is completed
		pImp->InitSkill();	

		
	}

	return pNPC;
}

gnpc * 
npc_spawner::CreatePetBase2(gplayer_imp * pMaster, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode, const int cid[3],
				unsigned char dir,  unsigned char pet_stamp, 
				int ai_policy_cid, int aggro_policy, int skill_level)
{
	world * pPlane = pMaster->_plane;
	int player_faction = pMaster->GetFaction();
	int player_enemy_faction = pMaster->GetEnemyFaction();

	const pet_data_temp * pTmp = pet_dataman::Get(pData->pet_tid);
	if(pTmp == NULL) 
	{
		GLog::log(GLOG_ERR,"Invalid pet template id %d",pData->pet_tid);
		return NULL;
	}

	gnpc * pNPC = NULL;
	pNPC = pPlane->AllocNPC();
	if(!pNPC) return NULL;
	pNPC->ID.type = GM_TYPE_NPC;
	pNPC->ID.id= MERGE_PET_ID(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = 0;
	pNPC->idle_timer = 100;	
	pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = pData->pet_tid; 
	pNPC->vis_tid = pData->pet_vis_tid; 
	if(!pNPC->vis_tid)
	{
		pNPC->vis_tid = pData->pet_tid; 
	}
	pNPC->monster_faction = player_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	if(pData->pet_class == pet_data::PET_CLASS_PLANT)	
	{
		//Plants with this flag set can automatically attack people in the hostile camp
		pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);

	//Set pet basic properties
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gpet_imp)));
	gpet_imp * pImp = (gpet_imp *)pNPC->imp;
	pImp->SetPetStamp(pet_stamp);
	pImp->SetAttackHook(	pMaster->GetPetAttackHook(), pMaster->GetPetEnchantHook(),
				pMaster->GetPetAttackFill(), pMaster->GetPetEnchantFill());

	pImp->InitFromMaster(pMaster);
	pImp->SetTID(pData->pet_tid);

	int plevel = pData->level;

	//First calculate the attributes obtained from the pet template
	extend_prop prop;
	int attack_degree = pMaster->_attack_degree; // Nivel de ataque
	int defend_degree = pMaster->_defend_degree; // Nivel de defesa
	int vigour = pMaster->GetVigour(); // Espirito
	int pen_fis = pMaster->_anti_defense_degree;
	int pen_mag = pMaster->_anti_resistance_degree;
	pet_dataman::GenerateBaseProp2(pData->pet_tid, plevel, skill_level, prop, attack_degree, defend_degree);
	//When calculating the attributes obtained from the summoner
	pMaster->CalcPetEnhance3(pData, skill_level, prop, attack_degree, defend_degree, vigour, pen_fis, pen_mag);
	
	pImp->_basic.level = plevel; 
	pImp->_basic.sec.value = 0;
	pImp->_basic.exp = 0;	//This experience is not the pet's current experience
	pImp->_basic.skill_point = 0; //This skill point is not the pet's current skill point
	if(pData->pet_class == pet_data::PET_CLASS_PLANT)
	{
		pImp->_basic.hp = (int)(prop.max_hp * pData->hp_factor);
		if(pImp->_basic.hp <= 0) pImp->_basic.hp = 1;
		pImp->_basic.mp = prop.max_mp/2;
	}
	else
	{
		pImp->_basic.hp = (int)prop.max_hp;
		if(pImp->_basic.hp <= 0) pImp->_basic.hp = 1;
		pImp->_basic.mp = prop.max_mp;
	}
	
	pImp->_base_prop = prop;
	pImp->_cur_prop = pImp->_base_prop;
	pImp->_cur_prop.attack_range += pTmp->body_size;
	pImp->_attack_degree = attack_degree;
	pImp->_defend_degree = defend_degree;
	pImp->SetVigourBase( vigour );
	pImp->_cur_item.attack_range = pImp->_base_prop.attack_range;
	pImp->_cur_item.weapon_type = 0; 	//Weapon type (long range, close range)
	pImp->_spawner = NULL;
	pImp->_faction = player_faction;
	pImp->_enemy_faction = player_enemy_faction;
	pImp->_inhabit_type = pTmp->inhabit_type;
	pImp->SetInhabitMode(inhabit_mode);
	pImp->_damage_delay = pTmp->damage_delay;
	pImp->_immune_state = pTmp->immune_type;
	pImp->_corpse_delay = 0;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTmp->body_size;
	
	//Does not regenerate blood quickly
	pImp->SetFastRegen(0);

	//Create AI-controlled objects No threat strategy introduced yet
	if(ai_policy_cid)
	{

		aggro_param aggp;
		ai_param aip;
		memset(&aggp,0,sizeof(aggp));
		memset(&aip,0,sizeof(aip));
		aip.trigger_policy = 0; 	//Pets do not have this trigger control, is it possible to consider using it?
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = 60.f;	//The data of the pet is written to death
		aggp.aggro_time = 100; 
		aggp.sight_range = pTmp->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = false;	//Pets do not help others, helping the owner is done by the owner's logic
		aggp.faction_accept_help = false;

		aip.policy_class = ai_policy_cid;
		
		aip.patrol_mode = false;
		//Here you need to fill in a strategy, the default strategy of pet unity is physical attack
		aip.primary_strategy = ai_policy::STRATEGY_MELEE; 
		if(pData->pet_class == pet_data::PET_CLASS_PLANT)
		{
			//The default strategy for plants is stakes
			aip.primary_strategy = ai_policy::STRATEGY_STUB;
		}

		//Verify and replicate skills
		for(unsigned int j = 0; j < pet_data::MAX_PET_SKILL_COUNT; j ++)
		{
			int id_skill = pData->skills[j].skill;
			int lvl_skill= pData->skills[j].level;
			if(id_skill <= 0) break;

			pImp->AddSkill(id_skill,lvl_skill);
		}


		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);

		//Initialize the skills of plants, it must be done after the initialization of AI is completed
		pImp->InitSkill();	
	}

	return pNPC;
}

gnpc * 
npc_spawner::CreatePetBase3(gplayer_imp * pMaster, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode, const int cid[3],
				unsigned char dir,  unsigned char pet_stamp, 
				int ai_policy_cid, int aggro_policy)
{
	world *pPlane = pMaster->_plane;
	int player_faction = pMaster->GetFaction();
	int player_enemy_faction = pMaster->GetEnemyFaction();

	const pet_data_temp *pTmp = pet_dataman::Get(pData->pet_tid);
	if(pTmp == NULL)
	{
		GLog::log(GLOG_ERR,"Invalid pet template id %d",pData->pet_tid);
		return NULL;
	}

	gnpc *pNPC = NULL;
	pNPC = pPlane->AllocNPC();
	if(!pNPC) return NULL;
	pNPC->ID.type = GM_TYPE_NPC;
	pNPC->ID.id= MERGE_PET_ID(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = 0;
	pNPC->idle_timer = 100;	
	pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = pData->pet_tid; 
	pNPC->vis_tid = pData->pet_vis_tid; 
	if(!pNPC->vis_tid)
	{
		pNPC->vis_tid = pData->pet_tid; 
	}
	pNPC->monster_faction = player_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);

	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gpet_imp)));
	gpet_imp * pImp = (gpet_imp *)pNPC->imp;
	pImp->SetPetStamp(pet_stamp);
	pImp->SetAttackHook(	pMaster->GetPetAttackHook(), pMaster->GetPetEnchantHook(),
				pMaster->GetPetAttackFill(), pMaster->GetPetEnchantFill());

	pImp->InitFromMaster(pMaster);
	pImp->SetTID(pData->pet_tid);

	int plevel = pData->level;

	extend_prop prop;
	int attack_degree = pMaster->_attack_degree; // Nivel de ataque
	int defend_degree = pMaster->_defend_degree; // Nivel de defesa
	int vigour = pMaster->GetVigour(); // Espirito
	int pen_fis = pMaster->_anti_defense_degree;
	int pen_mag = pMaster->_anti_resistance_degree;
	pet_dataman::GenerateBaseProp2(pData->pet_tid, plevel, 0, prop, attack_degree, defend_degree);
	pMaster->CalcPetEnhance4(pData,prop,attack_degree,defend_degree,vigour,pen_fis,pen_mag);

	pImp->_basic.level = plevel; 
	pImp->_basic.sec.value = 0;
	pImp->_basic.exp = 0;
	pImp->_basic.skill_point = 0;
	//pImp->_basic.hp = (int)(prop.max_hp * pData->hp_factor);
	pImp->_basic.hp = (int)prop.max_hp; //pet hp full
	if(pImp->_basic.hp <= 0) pImp->_basic.hp = 1;
	pImp->_basic.mp = prop.max_mp/2;
	
	pImp->_base_prop = prop;
	pImp->_cur_prop = pImp->_base_prop;
	pImp->_cur_prop.attack_range += pTmp->body_size;
	pImp->_attack_degree = attack_degree;
	pImp->_defend_degree = defend_degree;
	pImp->SetVigourBase( vigour );
	pImp->_cur_item.attack_range = pImp->_base_prop.attack_range;
	pImp->_cur_item.weapon_type = 0;
	pImp->_spawner = NULL;
	pImp->_faction = player_faction;
	pImp->_enemy_faction = player_enemy_faction;
	pImp->_inhabit_type = pTmp->inhabit_type;
	pImp->SetInhabitMode(inhabit_mode);
	pImp->_damage_delay = pTmp->damage_delay;
	pImp->_immune_state = pTmp->immune_type;
	pImp->_corpse_delay = 0;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTmp->body_size;

	pImp->SetFastRegen(0);

	if(ai_policy_cid)
	{

		aggro_param aggp;
		ai_param aip;
		memset(&aggp,0,sizeof(aggp));
		memset(&aip,0,sizeof(aip));
		aip.trigger_policy = 0; 
		aggp.aggro_policy = aggro_policy;	

		aggp.aggro_range = 60.f;
		aggp.aggro_time = 100; 
		aggp.sight_range = pTmp->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = false;		
		aggp.faction_accept_help = false;

		aip.policy_class = ai_policy_cid;

		aip.patrol_mode = false;
		aip.primary_strategy = ai_policy::STRATEGY_MELEE; 

		for(unsigned int j = 0; j < pet_data::MAX_PET_SKILL_COUNT; j ++)
		{
			int id_skill = pData->skills[j].skill;
			int lvl_skill= pData->skills[j].level;
			if(id_skill <= 0) break;

			pImp->AddSkill(id_skill,lvl_skill);
		}


		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);

	}

	return pNPC;
}

gnpc * 
npc_spawner::CreateMobBase(npc_spawner * __this,world * pPlane,const entry_t & et, 
		int spawn_index, const A3DVECTOR & pos,const int cid[3],
		unsigned char dir, int ai_policy_cid,int aggro_policy, gnpc * origin_npc,int life)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);
	if(pTemplate == NULL || pTemplate->mine_info.is_mine)
	{
		GLog::log(GLOG_ERR,"Invalid npc template id %d",et.npc_tid);
		ASSERT(false && "Invlid npc template id");
		return NULL;
	}

	if(pTemplate->role_in_war == 4  && ai_policy_cid == CLS_NPC_AI_POLICY_BASE)
	{
		//����ǹ��̳�����policy�����ǻ������ԣ���ô���ĳɹ��ǳ�����
		ai_policy_cid =  CLS_NPC_AI_POLICY_TURRET;
		aggro_policy = AGGRO_POLICY_TURRET;

	}

	//����NPC����npcȫ������
	gnpc * pNPC = NULL;
	if(!origin_npc)
	{
		pNPC = pPlane->AllocNPC();
		if(!pNPC) return NULL;
		pNPC->Clear();		//��Ϊ�п���zombie״̬û����ȷ���
		pNPC->SetActive();
		pNPC->ID.type = GM_TYPE_NPC;
		pNPC->ID.id= MERGE_ID<gnpc>(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetNPCIndex(pNPC)));
	}
	else
	{
		pNPC = origin_npc;
	}

	pNPC->pos = pos;
	pNPC->imp = CF_Create(cid[0],cid[1],cid[2],pPlane,pNPC);
	pNPC->spawn_index = spawn_index;
	pNPC->idle_timer = 0;	//���ﴴ������Ĭ�������ߵ�
	pNPC->idle_timer_count = pTemplate->normal_heartbeat_in_idle?1:abase::Rand(0,NPC_IDLE_HEARTBEAT);
	pNPC->tid = et.npc_tid; 
	pNPC->vis_tid = et.npc_tid; 
	pNPC->monster_faction = pTemplate->monster_faction;
	pNPC->msg_mask = gobject::MSG_MASK_ATTACK;
	pNPC->dir = dir;
	pNPC->cruise_timer = abase::Rand(0,31);
	pNPC->npc_idle_heartbeat = pTemplate->normal_heartbeat_in_idle?1:0;

	//����npc��������
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_imp)));
	gnpc_imp * pImp = (gnpc_imp *)pNPC->imp;
	pImp->_basic = pTemplate->bp;
	pImp->_base_prop = pTemplate->ep;
	pImp->_base_prop.attack_range += pTemplate->body_size;
	pImp->_cur_prop = pImp->_base_prop;
	pImp->_cur_item.attack_range = pTemplate->ep.attack_range;
	pImp->_cur_item.weapon_type = pTemplate->short_range_mode;	//��������(Զ�̣�����)
	if(pTemplate->short_range_mode) pImp->_cur_item.short_range = 4.0f;
	AdjustPropByCommonValue(pImp, pPlane, pTemplate);
	pImp->_spawner = __this;
	pImp->_faction = pTemplate->faction;
	pImp->_enemy_faction = pTemplate->enemy_faction;
	pImp->_after_death = pTemplate->after_death;
	pImp->_inhabit_type = pTemplate->inhabit_type;
	pImp->SetInhabitMode(pTemplate->inhabit_mode);
	pImp->_damage_delay = pTemplate->damage_delay;
	pImp->_immune_state = pTemplate->immune_type;
	pImp->_corpse_delay = et.corpse_delay;
	pImp->_petegg_id = pTemplate->petegg_id;
	pImp->_drop_no_protected = pTemplate->drop_no_protected;
	pImp->_drop_no_profit_limit = pTemplate->drop_no_profit_limit;
	pImp->_drop_mine_prob = pTemplate->drop_mine_prob;
	pImp->_drop_mine_group = pTemplate->drop_mine_group;
	pImp->_attack_degree = pTemplate->attack_degree;
	pImp->_defend_degree = pTemplate->defend_degree;
	pNPC->invisible_degree = pImp->_invisible_active = pTemplate->invisible_degree;
	pNPC->anti_invisible_degree = pImp->_anti_invisible_active = pTemplate->anti_invisible_degree;
	if(pNPC->invisible_degree)
	{
		pNPC->object_state |= gactive_object::STATE_INVISIBLE;
		__PRINTF("��ʼ������npc,��������%d\n",pNPC->invisible_degree);
	}
	pImp->_no_accept_player_buff = pTemplate->no_accept_player_buff;
	pImp->_fixed_direction = pTemplate->fixed_direction;
	if(pImp->_fixed_direction)
		pNPC->object_state |= gactive_object::STATE_NPC_FIXDIR;
	pNPC->native_state = gnpc::TYPE_NATIVE;
	pNPC->body_size = pTemplate->body_size;

	pImp->_after_death_summon_monster_id = pTemplate->after_death_summon_monster_id;
	pImp->_after_death_summon_monster_radius = pTemplate->after_death_summon_monster_radius;
	pImp->_after_death_summon_monster_num = pTemplate->after_death_summon_monster_num;

	pImp->_max_received_damage_per_hit = pTemplate->max_received_damage_per_hit;

	if(pTemplate->role_in_war)
	{
		pImp->SetFastRegen(0);
	}
	
	if(pTemplate->domain_related)
	{
		int domain_id = city_region::GetDomainID(pos.x,pos.z);
		pNPC->mafia_id = GMSV::GetCityOwner(domain_id);
		pNPC->object_state |= gactive_object::STATE_NPC_MAFIA;
	}

	memcpy(pImp->_local_var,pTemplate->local_var,sizeof(pImp->_local_var));

	//����npc���������
	if(pTemplate->addon_choice_count > 1)
	{
		int index = abase::RandSelect(pTemplate->probability_addon,pTemplate->addon_choice_count);
		AdjustNPCAddon(pImp, pTemplate->id_addon[index]);
	}
	else
	{
		//ֻ��һ��
		AdjustNPCAddon(pImp,pTemplate->id_addon[0]);
	}

	//���������ͱ���
	if(pTemplate->aggressive_mode)
	{
		pNPC->msg_mask |= gobject::MSG_MASK_PLAYER_MOVE;
	}
	pNPC->msg_mask &= et.msg_mask_and;
	pNPC->msg_mask |= et.msg_mask_or;

#ifdef __TEST_PERFORMANCE__
	ASSERT(pNPC->msg_mask & gobject::MSG_MASK_PLAYER_MOVE);
#endif

	//����AI���ƵĶ���  ��δ�����޶Ȳ���
	if(ai_policy_cid)
	{

		aggro_param aggp;
		ai_param aip;
		memset(&aggp,0,sizeof(aggp));
		memset(&aip,0,sizeof(aip));
		aip.trigger_policy = pTemplate->trigger_policy;
		if(aggro_policy)
		{
			aggp.aggro_policy = aggro_policy;	
		}
		else
		{
			//ѡ����ʵĳ�޲���
			int index = abase::RandSelect(pTemplate->aggro_strategy_probs,4);
			aggp.aggro_policy = pTemplate->aggro_strategy_ids[index];	
		}
		aggp.aggro_range = pTemplate->aggro_range;
		aggp.aggro_time = pTemplate->aggro_time; 
		aggp.sight_range = pTemplate->sight_range;
		aggp.enemy_faction = pImp->_enemy_faction;
		aggp.faction = pImp->_faction;
		aggp.faction_ask_help = et.ask_for_help?et.monster_faction_ask_help:pTemplate->monster_faction_ask_help;
		aggp.faction_accept_help = et.accept_ask_for_help?et.monster_faction_accept_for_help:pTemplate->monster_faction_can_help;

		if(aggp.faction_accept_help)
		{
			pNPC->msg_mask |= gobject::MSG_MASK_CRY_FOR_HELP;
		}

		aip.policy_class = ai_policy_cid;
		int cindex = abase::RandSelect(&(pTemplate->skill_hp75[0].prob),sizeof(npc_template::condition_skill),5);
		aip.event[2] = pTemplate->skill_hp75[cindex].skill;
		aip.event_level[2] = pTemplate->skill_hp75[cindex].level;
		cindex = abase::RandSelect(&(pTemplate->skill_hp50[0].prob),sizeof(npc_template::condition_skill),5);
		aip.event[1] = pTemplate->skill_hp50[cindex].skill;
		aip.event_level[1] = pTemplate->skill_hp50[cindex].level;
		cindex = abase::RandSelect(&(pTemplate->skill_hp25[0].prob),sizeof(npc_template::condition_skill),5);
		aip.event[0] = pTemplate->skill_hp25[cindex].skill;
		aip.event_level[0] = pTemplate->skill_hp25[cindex].level;

		/*
		aip.event[2] = 34;
		aip.event_level[2] = 1;
		aip.event[1] = 34;
		aip.event_level[1] = 1;
		aip.event[0] = 34;
		aip.event_level[0] = 1;
		*/

		
		aip.patrol_mode = pTemplate->patrol_mode;
		aip.primary_strategy = pTemplate->id_strategy; 

		//��֤�͸��Ƽ���
		ASSERT(sizeof(aip.skills) == sizeof(pTemplate->skills));
		memcpy(&aip.skills,&pTemplate->skills,sizeof(aip.skills));

		aip.path_id = et.path_id;
		aip.path_type = et.path_type;
		aip.speed_flag = et.speed_flag;
		aip.no_auto_fight = pTemplate->no_auto_fight;
		aip.max_move_range = pTemplate->max_move_range;
		
		gnpc_controller * pCtrl = (gnpc_controller*)pImp->_commander;
		pCtrl->CreateAI(aggp,aip);

		//����AI֮������趨����
		pCtrl->SetLifeTime(life);
	}


	return pNPC;
}

gnpc * 
npc_spawner::CreateNPCBase(npc_spawner * __this, world * pPlane, const entry_t & et,
						int spawn_index, const A3DVECTOR & pos,const int __cid[3], unsigned char dir,
						int ai_policy_cid,int __aggro_policy,gnpc * origin_npc, int life)
{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);
	ASSERT(pTemplate);
	if(!pTemplate) return NULL; 

	//��֯class id
	int cid[3] =  {__cid[0], __cid[1], __cid[2]};
	if(cid[0] < 0)  cid[0] = CLS_SERVICE_NPC_IMP;
	if(cid[1] < 0)  cid[1] = CLS_NPC_DISPATCHER;
	if(cid[2] < 0)  cid[2] = CLS_NPC_CONTROLLER;

	//��֯aipolicy��aggro_policy
	int aipolicy = ai_policy_cid;
	int aggro_policy = __aggro_policy;
	if(aipolicy < 0){
		switch(pTemplate->npc_data.npc_type)
		{
			case npc_template::npc_statement::NPC_TYPE_GUARD:
				aipolicy = CLS_NPC_AI_POLICY_GUARD;
				break;
			case npc_template::npc_statement::NPC_TYPE_NORMAL:
			default:
				//��ͨnpcû��ai
				aipolicy = 0;
				break;
		}
	}

	gnpc * pNPC = CreateMobBase(__this,pPlane,et,spawn_index,pos,cid,dir,aipolicy,aggro_policy,origin_npc,life);
	if(!pNPC) return NULL;
	
	//������ַ���
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)));
	service_npc * pImp = (service_npc *)pNPC->imp;
	pImp->SetTaxRate(pTemplate->npc_data.tax_rate);
	pImp->SetNeedDomain(pTemplate->npc_data.need_domain);
	pImp->SetServeDistanceUnlimited(pTemplate->npc_data.serve_distance_unlimited);
	pImp->SetSrcMonster(pTemplate->npc_data.src_monster);
	if(int num = pTemplate->npc_data.service_sell_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_VENDOR);
		pImp->AddProvider(provider,pTemplate->npc_data.service_sell_goods,sizeof(int)*6*num);
	}

	if(pTemplate->npc_data.service_purchase)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_PURCHASE);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_repair)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_REPAIR);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_heal)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_HEAL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(int num = pTemplate->npc_data.service_transmit_target_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRANSMIT);
		pImp->AddProvider(provider,pTemplate->npc_data.transmit_entry, sizeof(npc_template::npc_statement::__st_ent)* num);
	}

	if(pTemplate->npc_data.service_task_in_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_IN);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_in_list,pTemplate->npc_data.service_task_in_num*sizeof(int));
	}

	if(pTemplate->npc_data.service_task_out_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_OUT);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_out_list,pTemplate->npc_data.service_task_out_num*sizeof(int));
	}

	if(pTemplate->npc_data.service_task_matter_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_MATTER);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_matter_list,pTemplate->npc_data.service_task_matter_num*sizeof(int));
	}

	if(int num = pTemplate->npc_data.service_teach_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_SKILL);
		pImp->AddProvider(provider,pTemplate->npc_data.service_teach_skill_list, sizeof(int)*num);
	}

	
	if(pTemplate->npc_data.service_install)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_INSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_uninstall)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_UNINSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_produce.produce_skill >=0 && pTemplate->npc_data.service_produce.produce_num > 0) 
	{
		npc_template::npc_statement::__service_produce &te = pTemplate->npc_data.service_produce;
		int service_id;
		if(te.type == 0) 
			service_id = service_ns::SERVICE_ID_PRODUCE;
		else if(te.type == 1)
			service_id = 46;
		else if (te.type == 2)
			service_id = 58;
		else if (te.type == 3)
			service_id = 74;
        else if (te.type ==4)
            service_id = 92;
		else if (te.type ==5) // Nova G17
            service_id = 124;

		service_provider * provider = service_manager::CreateProviderInstance(service_id);
		pImp->AddProvider(provider,&te,sizeof(te));
	}

	if(pTemplate->npc_data.service_decompose_skill >=0) 
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_DECOMPOSE);
		int skill = pTemplate->npc_data.service_decompose_skill;
		pImp->AddProvider(provider,&skill,4);
	}

	if(pTemplate->npc_data.service_storage)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_PASS);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_OPEN);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_identify)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_IDENTIFY);
		pImp->AddProvider(provider,&pTemplate->npc_data.service_identify_fee,sizeof(int));
	}

	if(pTemplate->npc_data.service_vehicle_count)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_VEHICLE);
		pImp->AddProvider(provider,&pTemplate->npc_data.vehicle_path_list,
				sizeof(npc_template::npc_statement::vehicle_path_entry));
	}

	if(pTemplate->npc_data.service_waypoint_id > 0)
	{
		service_provider * provider=service_manager::CreateProviderInstance(service_ns::SERVICE_ID_WAYPOINT);
		int wp = pTemplate->npc_data.service_waypoint_id ;
		pImp->AddProvider(provider,&wp,sizeof(wp));
	}

	if(pTemplate->npc_data.service_unlearn_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_UNLEARN_SKILL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_faction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(18);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_face_ticket)
	{
		service_provider * provider = service_manager::CreateProviderInstance(24);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_mail)
	{
		service_provider * provider = service_manager::CreateProviderInstance(25);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_auction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(26);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_double_exp)
	{
		service_provider * provider = service_manager::CreateProviderInstance(27);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_hatch_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(28);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_recover_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(29);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_war_management)
	{
		service_provider * provider = service_manager::CreateProviderInstance(30);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.npc_tower_build_size)
	{
		service_provider * provider = service_manager::CreateProviderInstance(31);
		pImp->AddProvider(provider,pTemplate->npc_data.npc_tower_build,sizeof(pTemplate->npc_data.npc_tower_build));
	}

	if(pTemplate->npc_data.service_war_leave_battle)
	{
		service_provider * provider = service_manager::CreateProviderInstance(32);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_reset_prop_count)
	{
		unsigned int size = pTemplate->npc_data.service_reset_prop_count;
		service_provider * provider = service_manager::CreateProviderInstance(33);
		pImp->AddProvider(provider,pTemplate->npc_data.reset_prop,size * sizeof(npc_template::npc_statement::__reset_prop));
	}

	if(pTemplate->npc_data.service_cash_trade)
	{
		service_provider * provider = service_manager::CreateProviderInstance(42);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(43);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(35);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_change_pet_name)
	{
		service_provider * provider = service_manager::CreateProviderInstance(36);
		pImp->AddProvider(provider,&pTemplate->npc_data.change_pet_name_prop,sizeof(int)*2);
	}

	if(pTemplate->npc_data.service_forget_pet_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(37);
		pImp->AddProvider(provider,&pTemplate->npc_data.forget_pet_skill_prop,sizeof(int)*2);
	}

	if(int num = pTemplate->npc_data.service_pet_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(38);
		pImp->AddProvider(provider,pTemplate->npc_data.service_pet_skill_list, sizeof(int)*num);
	}

	if( pTemplate->npc_data.service_equip_bind)
	{
		//ע��:�� ���� �������ʹ��ͬһ��service_provider
		service_provider * provider = service_manager::CreateProviderInstance(39);
		pImp->AddProvider(provider, &pTemplate->npc_data.service_bind_prop ,sizeof(pTemplate->npc_data.service_bind_prop));
	}

	if( pTemplate->npc_data.service_destroy_bind)
	{
		//ע��:�� ���� �������ʹ��ͬһ��service_provider
		int m[] = {	pTemplate->npc_data.service_destroy_bind_prop.money_need,
				0,
				pTemplate->npc_data.service_destroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(40);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if( pTemplate->npc_data.service_undestroy_bind)
	{
		//ע��:�� ���� �������ʹ��ͬһ��service_provider
		int m[] = {	pTemplate->npc_data.service_undestroy_bind_prop.money_need,
				0,
				pTemplate->npc_data.service_undestroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(41);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if(pTemplate->npc_data.service_dye)
	{
		service_provider * provider = service_manager::CreateProviderInstance(44);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_refine_transmit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(45);
		pImp->AddProvider(provider,NULL,0);
	}

	if(1 || pTemplate->npc_data.service_make_slot)
	{
		service_provider * provider = service_manager::CreateProviderInstance(47);
		pImp->AddProvider(provider,NULL,0);
	}

	//lgc
	if(pTemplate->npc_data.service_elf_dec_attributie)
	{
		service_provider * provider = service_manager::CreateProviderInstance(48);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_flush_genius)
	{
		service_provider * provider = service_manager::CreateProviderInstance(49);
		pImp->AddProvider(provider,NULL,0);
	}
	if(int num = pTemplate->npc_data.service_elf_learn_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(50);
		pImp->AddProvider(provider,pTemplate->npc_data.service_elf_learn_skill_list, sizeof(int)*num);
	}
	if(pTemplate->npc_data.service_elf_forget_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(51);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(52);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_refine_transmit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(53);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_decompose)
	{
		service_provider * provider = service_manager::CreateProviderInstance(54);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_destroy_item)
	{
		service_provider * provider = service_manager::CreateProviderInstance(55);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_dye_suit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(56);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_repair_damaged_item)
	{
		service_provider * provider = service_manager::CreateProviderInstance(57);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_user_trashbox)
	{
		service_provider * provider = service_manager::CreateProviderInstance(59);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_webtrade)
	{
		service_provider * provider = service_manager::CreateProviderInstance(60);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_god_evil_convert)
	{
		service_provider * provider = service_manager::CreateProviderInstance(61);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_wedding_book)
	{
		service_provider * provider = service_manager::CreateProviderInstance(62);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_wedding_invite)
	{
		service_provider * provider = service_manager::CreateProviderInstance(63);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_factionfortress)
	{
		service_provider * provider = service_manager::CreateProviderInstance(64);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_factionfortress2)
	{
		service_provider * provider = service_manager::CreateProviderInstance(65);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_factionfortress_material_exchange)
	{
		service_provider * provider = service_manager::CreateProviderInstance(66);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_dye_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(67);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_trashbox_view)
	{
		service_provider * provider = service_manager::CreateProviderInstance(68);
		pImp->AddProvider(provider,NULL,0);
	}
	if(int num = pTemplate->npc_data.service_engrave.engrave_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(69);
		pImp->AddProvider(provider,pTemplate->npc_data.service_engrave.engrave_entrys, sizeof(int)*num);
	}
	if(pTemplate->npc_data.service_dpsrank)
	{
		service_provider * provider = service_manager::CreateProviderInstance(70);
		pImp->AddProvider(provider,NULL,0);
	}
	if(int num = pTemplate->npc_data.service_addonregen.addonregen_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(71);
		pImp->AddProvider(provider,pTemplate->npc_data.service_addonregen.addonregen_entrys, sizeof(int)*num);
	}
	if(pTemplate->npc_data.service_playerforce_tid)
	{
		service_provider * provider = service_manager::CreateProviderInstance(72);
		pImp->AddProvider(provider,&pTemplate->npc_data.service_playerforce_tid,sizeof(int));
	}
	
	if(pTemplate->npc_data.service_waypoint_id > 0 && pTemplate->npc_data.service_transmit_target_num > 0)
	{
		service_provider * provider = service_manager::CreateProviderInstance(73);
		int wp = pTemplate->npc_data.service_waypoint_id;
		pImp->AddProvider(provider,&wp,sizeof(wp));
		//�ռ��ó������д���·��������ٶ����͵㷢�ַ���ʹ��ͷ������е�waypoint������һ��·��������Ϸ�߼����������Ǻ�����
		int src_wp = pTemplate->npc_data.service_waypoint_id;
		int dst_count = pTemplate->npc_data.service_transmit_target_num;
		for(int i=0; i<dst_count; ++i)
			world_manager::GetInstance()->InsertTransmitEntry(src_wp, pTemplate->npc_data.transmit_entry[i]);
	}
	if(pTemplate->npc_data.service_country_management)
	{
		service_provider * provider = service_manager::CreateProviderInstance(75);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_countrybattle_leave)
	{
		service_provider * provider = service_manager::CreateProviderInstance(76);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_equip_sign)
	{
		service_provider * provider = service_manager::CreateProviderInstance(77);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_change_ds_forward)
	{
		service_provider * provider = service_manager::CreateProviderInstance(78);
		pImp->AddProvider(provider,&pTemplate->npc_data.service_cross,sizeof(pTemplate->npc_data.service_cross));
	}
	if(pTemplate->npc_data.service_change_ds_backward)
	{
		service_provider * provider = service_manager::CreateProviderInstance(79);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_player_rename)
	{
		service_provider * provider = service_manager::CreateProviderInstance(80);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_addon_change)
	{
		service_provider * provider = service_manager::CreateProviderInstance(81);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_addon_replace)
	{
		service_provider * provider = service_manager::CreateProviderInstance(82);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_kingelection)
	{
		service_provider * provider = service_manager::CreateProviderInstance(83);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_decompose_fashion_item)
	{
		service_provider * provider = service_manager::CreateProviderInstance(84);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_player_shop)
	{
		service_provider * provider = service_manager::CreateProviderInstance(85);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_reincarnation)
	{
		service_provider * provider = service_manager::CreateProviderInstance(86);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_giftcardredeem)
	{
		service_provider * provider = service_manager::CreateProviderInstance(87);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_trickbattle_apply)
	{
		service_provider * provider = service_manager::CreateProviderInstance(88);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_generalcard_rebirth)
	{
		service_provider * provider = service_manager::CreateProviderInstance(89);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_improve_flysword)
	{
		service_provider * provider = service_manager::CreateProviderInstance(90);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_mafia_pvp_signup)
	{
		service_provider * provider = service_manager::CreateProviderInstance(91);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_gold_shop)
	{
		service_provider * provider = service_manager::CreateProviderInstance(93);
		pImp->AddProvider(provider,&et.npc_tid,sizeof(int));
	}
	if(pTemplate->npc_data.service_dividend_shop)
	{
		service_provider * provider = service_manager::CreateProviderInstance(94);
		pImp->AddProvider(provider,&et.npc_tid,sizeof(int));
	}
    if (pTemplate->npc_data.service_player_change_gender)
    {
        service_provider* provider = service_manager::CreateProviderInstance(95);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_make_slot_for_decoration)
    {
        service_provider* provider = service_manager::CreateProviderInstance(96);
        pImp->AddProvider(provider, NULL, 0);
    }
	if (pTemplate->npc_data.service_select_solo_tower_challenge_stage)
	{
		service_provider* provider = service_manager::CreateProviderInstance(97);
		pImp->AddProvider(provider, NULL, 0);
	}
    if (pTemplate->npc_data.service_solo_challenge_rank)
    {
        service_provider* provider = service_manager::CreateProviderInstance(98);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_mnfaction_sign_up)
    {
        service_provider* provider = service_manager::CreateProviderInstance(99);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_mnfaction_rank)
    {
        service_provider* provider = service_manager::CreateProviderInstance(100);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_mnfaction_battle_transmit)
    {
        service_provider* provider = service_manager::CreateProviderInstance(101);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_mnfaction_join_leave)
    {
        service_provider* provider = service_manager::CreateProviderInstance(102);
        pImp->AddProvider(provider, NULL, 0);
    }
    if (pTemplate->npc_data.service_solo_challenge_rank_global)
    {
        service_provider* provider = service_manager::CreateProviderInstance(103);
        pImp->AddProvider(provider, NULL, 0);
    }

	// Arena of Aurora
	if (pTemplate->npc_data.service_arena_create_team) // Create Team
    {
        service_provider* provider = service_manager::CreateProviderInstance(105);
        pImp->AddProvider(provider, NULL, 0);
    }
	if (pTemplate->npc_data.service_arena_join_team) // Join Arena
	{
		service_provider* provider = service_manager::CreateProviderInstance(106);
		pImp->AddProvider(provider, NULL, 0);
	}
	if (pTemplate->npc_data.service_arena_characters_view) // View Characters Info
	{
		service_provider* provider = service_manager::CreateProviderInstance(107);
		pImp->AddProvider(provider, NULL, 0);
	}
	if (pTemplate->npc_data.service_arena_ranking_view) // View Ranking Arena
	{
		service_provider* provider = service_manager::CreateProviderInstance(108);
		pImp->AddProvider(provider, NULL, 0);
	}
	if (pTemplate->npc_data.service_arena_ranking_view_global) // View Team Global
	{
		service_provider* provider = service_manager::CreateProviderInstance(109);
		pImp->AddProvider(provider, NULL, 0);
	}
	//End Arena of Aurora

	if(pTemplate->npc_data.lib_service_produce.produce_num > 0) 
	{
		npc_template::npc_statement::__lib_service_produce &te = pTemplate->npc_data.lib_service_produce;
		
		service_provider * provider = service_manager::CreateProviderInstance(110);
		pImp->AddProvider(provider,&te,sizeof(te));
	}

	if (pTemplate->npc_data.storage_999_recv)
    {
        service_provider* provider = service_manager::CreateProviderInstance(118);
        pImp->AddProvider(provider, NULL, 0);
    }

	if (pTemplate->npc_data.service_bible_refine)
    {
        service_provider* provider = service_manager::CreateProviderInstance(119);
        pImp->AddProvider(provider, NULL, 0);
    }
	
	if (pTemplate->npc_data.service_bible_refine_transmit)	
	{
		service_provider* provider = service_manager::CreateProviderInstance(120);
		pImp->AddProvider(provider, NULL, 0);
	}		

	if(int num = pTemplate->npc_data.pridictable_engrave_service.engrave_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(123);
		pImp->AddProvider(provider,pTemplate->npc_data.pridictable_engrave_service.engrave_entrys, sizeof(int)*num);
	}

	/*170+ Códice*/
	if(pTemplate->npc_data.service_dye_codex)
	{
		service_provider * provider = service_manager::CreateProviderInstance(129);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_rename_codex)
	{
		service_provider * provider = service_manager::CreateProviderInstance(130);
		pImp->AddProvider(provider,NULL,0);
	}

	/*170+ Bebê Celestial*/
	if(pTemplate->npc_data.service_kid_create)
	{
		service_provider * provider = service_manager::CreateProviderInstance(131);
		pImp->AddProvider(provider,NULL,0);
	}

	if (pTemplate->npc_data.service_material_refine)
    {
        service_provider* provider = service_manager::CreateProviderInstance(133);
        pImp->AddProvider(provider, NULL, 0);
    }
	return pNPC;
}
		
gnpc *
mobs_spawner::CreateMob(world * pPlane,int index,const entry_t & et)
{
	A3DVECTOR pos;
	GeneratePos(pos,et.offset_terrain,pPlane);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = CreateMobBase(this,pPlane,et, index,pos,cid,GenDir(),CLS_NPC_AI_POLICY_BASE,0,NULL,_mob_life);
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->OnNpcEnterWorld();
		pNPC->imp->_runner->enter_world();
		pNPC->Unlock();
	}
	return pNPC;
}

void 
mobs_spawner::ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR &pos, int index)
{
	XID oldID = pNPC->ID;
	pNPC->Clear();
	pNPC->SetActive();
	pNPC->ID = oldID;

	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	entry_t &et = _entry_list[index];
	gnpc * pNPC2 = CreateMobBase(this,pPlane,et, index,pos,cid,GenDir(),CLS_NPC_AI_POLICY_BASE,0,pNPC,_mob_life);
	ASSERT(pNPC2 == pNPC);
	pNPC2 = NULL;

	pPlane->InsertNPC(pNPC);
	pNPC->imp->OnNpcEnterWorld();
	pNPC->imp->_runner->enter_world();
}

bool 
mobs_spawner::CreateMobs(world * pPlane)
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		unsigned int count = _entry_list[i].mobs_count;
		for(unsigned int j = 0; j < count; j ++)
		{
			gnpc * pNPC = CreateMob(pPlane,i,_entry_list[i]);
			if(pNPC == NULL) return false;
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
			
		}
	}
	return true;
}

void
npc_spawner::ClearObjects(world * pPlane)
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gnpc * pHeader = _npc_pool[i];
		if(pHeader == NULL) continue;

		//�������ڵȴ������IDɾ�������ҽ���������Ҳɾ��

		gnpc * tmp = pHeader;
		do
		{
			_xid_list.erase(tmp->ID);
			gnpc * pNext = (gnpc*)tmp->pNext;
			tmp->Lock();
			tmp->imp->_commander->Release();
			tmp->Unlock();
			tmp = pNext;
		}while(tmp != pHeader);
		_npc_pool[i] = NULL;
	}

	//��������δɾ���Ķ�������Ϣ ������ʧ
	if(_xid_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_SPAWN_DISAPPEAR,XID(-1,-1),XID(-1,-1),A3DVECTOR(0,0,0),0);
		abase::vector<XID> list;
		abase::static_multimap<XID,int, abase::fast_alloc<> >::iterator it = _xid_list.begin();
		list.reserve(_xid_list.size());
		for(;it != _xid_list.end(); ++it)
		{
			list.push_back(it->first);
		}
		pPlane->SendMessage(list.begin(),list.end(), msg);
		_xid_list.clear();
	}
}

bool
mobs_spawner::Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * imp, bool is_reset)
{
	if(!_auto_respawn || !_is_spawned)
	{
		imp->_commander->Release();
		return false;
	}
	slice *pPiece = pNPC->pPiece;
	if(pPiece)
	{
		pPlane->RemoveNPC(pNPC);
	}       
	else
	{
		//û��piece��Ҳ�϶��ڹ�������
		pPlane->RemoveNPCFromMan(pNPC);
	}
	pNPC->ClrActive();

	pNPC->npc_reborn_flag = is_reset?1:0;

	unsigned int index = pNPC->spawn_index;
	ASSERT(index < _entry_list.size());
	if(index >= _entry_list.size())
	{
		//����־���������� 
		GLog::log(GLOG_ERR,"����NPCʱ���ִ��������������");
		ASSERT(false);
		return false;
	}

	spin_autolock  keeper(_pool_lock);
	if(!_is_spawned || (_mobs_total_gen_num && _mobs_cur_gen_num >= _mobs_total_gen_num))
	{
		_xid_list.erase(pNPC->ID);
		//check again
		imp->_commander->Release();
		return false;
	}
	_mobs_cur_gen_num ++;
	gnpc * header = _npc_pool[index];
	int tick = g_timer.get_tick();
	//pNPC->native_state = tick + _entry_list[index].reborn_time;
	pNPC->native_state = tick + abase::Rand(_entry_list[index].reborn_time_lower, _entry_list[index].reborn_time_upper);
	if(header)
	{
		//pNPC->pNext= header;
		//pNPC->pPrev= header->pPrev;
		//header->pPrev->pNext = pNPC;
		//header->pPrev = pNPC;

		if(pNPC->native_state < header->native_state)
		{
			pNPC->pNext= header;
			pNPC->pPrev= header->pPrev;
			header->pPrev->pNext = pNPC;
			header->pPrev = pNPC;
			_npc_pool[index] = pNPC;
		}
		else
		{
			gnpc * p = (gnpc *)header->pNext;
			while(p != header && pNPC->native_state >= p->native_state)
				p = (gnpc *)p->pNext;
			pNPC->pNext= p;
			pNPC->pPrev= p->pPrev;
			p->pPrev->pNext = pNPC;
			p->pPrev = pNPC;			
		}
	}
	else
	{
		_npc_pool[index] = pNPC;
		pNPC->pPrev = pNPC->pNext = pNPC;
	}
	__PRINTF("������npc %d at %d/%d\n",pNPC->ID.id & 0x7FFFFFFF,tick,pNPC->native_state);
	return true;
}

void 
mobs_spawner::Release()
{
	spin_autolock  keeper(_pool_lock);
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;

		gnpc * pTmp = (gnpc *)header;
		do
		{
			gnpc * pNext = (gnpc*)pTmp->pNext;
			spin_autolock keeper(pTmp->spinlock);
			pTmp->imp->_commander->Release();
			pTmp = pNext;
		} while(pTmp!= header);
		_npc_pool[i] = NULL;
	}
	_entry_list.clear();
	_npc_pool.clear();

}

void 
mobs_spawner::OnHeartbeat(world * pPlane)
{
	int tick = g_timer.get_tick();
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;

		if(header->native_state - tick > 0) continue;
		gnpc * pTmp =(gnpc*) header->pNext;
		while(pTmp != header && pTmp->native_state - tick <= 0)
		{
			pTmp = (gnpc*)pTmp->pNext;
		}
		if(pTmp == header)
		{
			_npc_pool[i] = NULL;
		}
		else
		{
			_npc_pool[i] = pTmp;
			header->pPrev->pNext = pTmp;
			pTmp ->pPrev = header->pPrev;
		}

		//��ʱ����ʵ���Բ�������������������¼�������ôƵ���ϣ������޴�
		//��ʼ����
		Reborn(pPlane,header,pTmp,_entry_list[i].offset_terrain,_entry_list[i].npc_tid);
	}
}

void 
mobs_spawner::Reborn(world * pPlane, gnpc * header, gnpc * tail,float offset_terrain,int tid)
{
	int tick = g_timer.get_tick();
	do
	{
		gnpc * pNPC = header;
		header = (gnpc*)header->pNext;
		ASSERT(pNPC->b_zombie == true);
		ASSERT(pNPC->pPiece == NULL);

		GeneratePos(pNPC->pos,offset_terrain,pPlane);

		int tmp_tick = pNPC->native_state;

		pNPC->native_state = gnpc::TYPE_NATIVE;
		pNPC->object_state = 0; 
		pNPC->dir = GenDir();

		pNPC->Lock();
		if(_is_spawned)
		{
			if(pNPC->npc_reborn_flag)
			{
				//��Ҫ������������
				//����ReCreateû���ͷ�imp���������ͷ�һ�£���δ����
				pNPC->imp->_commander->Release(false);
				 unsigned int index = pNPC->spawn_index;
				 ReCreate(pPlane,pNPC,pNPC->pos, index);
			}
			else
			{
				ASSERT(pNPC->tid == tid);
				pNPC->SetActive();
				pNPC->pPrev = pNPC->pNext = NULL;
				pNPC->idle_timer_count = abase::Rand(0,NPC_IDLE_HEARTBEAT);
				pPlane->InsertNPC(pNPC);
				if(tick - pNPC->pPiece->idle_timestamp > NPC_IDLE_TIMER * TICK_PER_SEC)
				{
					pNPC->idle_timer = 0;
				}
				else
				{
					pNPC->idle_timer = NPC_IDLE_TIMER;
				}
				//����ÿ�ζ���ͼ�����������
				RegenAddon((gnpc_imp*)pNPC->imp,tid);

				pNPC->imp->Reborn();
				((gnpc_imp*)(pNPC->imp))->SetLifeTime(_mob_life);
				ASSERT(pNPC->b_zombie == false);
				ASSERT(((gnpc_imp*)(pNPC->imp))->_spawner == this);
			}
		}
		else
		{
			//�����������е�spawner ֱ���ͷŲ����ϵ�NPC
			pNPC->imp->_commander->Release();
		}
		pNPC->Unlock();
		__PRINTF("npc %d(%d/%d)����վ�������\n",pNPC->ID.id & 0x7FFFFFFF,tmp_tick,tick);

	} while(header != tail);
}


/*
bool 
npc_generator::InitIncubator(world * pPlane)		//��ʼ�����е�spawner��ȥ���������Լ��Ĳ���
{
	_plane = pPlane;
	rect base_region = pPlane->GetGrid().base_region;
	int total_count = 0;
	//����npc �������Ĵ���
	for(unsigned int  i =0; i < _spawner_list.size(); i ++)
	{
		rect spawn_region = _spawner_list[i]->GetRegion();
		if(!base_region.IsOverlap(spawn_region))
		{
			delete _spawner_list[i];
			_spawner_list.erase(_spawner_list.begin() + i);
			--i;
			continue;
		}

		if(!spawn_region.GetIntersection(base_region))
		{
			delete _spawner_list[i];
			_spawner_list.erase(_spawner_list.begin() + i);
			--i;
			continue;
		}

		int mobs_count;
		if((mobs_count = _spawner_list[i]->CutRegion(spawn_region)) <= 0)
		{
			//����֮��û�ж����ˣ�ɾ��֮
			delete _spawner_list[i];
			_spawner_list.erase(_spawner_list.begin() + i);
			--i;
			continue;
		}
		total_count += mobs_count;
		//__PRINTINFO("new region:(%f,%f,%f,%f)\n",spawn_region.left,spawn_region.top,spawn_region.right,spawn_region.bottom);
	}
	if(pPlane->w_plane_index == 0) __PRINTINFO("���������ܹ���Ͻ%d��npc�͹���\n",total_count);

	total_count = 0;
	//���п����������Ĵ���
	for(unsigned int  i =0; i < _mine_spawner_list.size(); i ++)
	{
		rect spawn_region = _mine_spawner_list[i]->GetRegion();
		if(!base_region.IsOverlap(spawn_region))
		{
			delete _mine_spawner_list[i];
			_mine_spawner_list.erase(_mine_spawner_list.begin() + i);
			--i;
			continue;
		}

		if(!spawn_region.GetIntersection(base_region))
		{
			delete _mine_spawner_list[i];
			_mine_spawner_list.erase(_mine_spawner_list.begin() + i);
			--i;
			continue;
		}

		int mine_count;
		if((mine_count = _mine_spawner_list[i]->CutRegion(spawn_region)) <= 0)
		{
			//����֮��û�ж����ˣ�ɾ��֮
			delete _mine_spawner_list[i];
			_mine_spawner_list.erase(_mine_spawner_list.begin() + i);
			--i;
			continue;
		}
		total_count += mine_count;
		//__PRINTINFO("new region:(%f,%f,%f,%f)\n",spawn_region.left,spawn_region.top,spawn_region.right,spawn_region.bottom);
	}

	return true;
}
*/

bool 
npc_generator::InitIncubator(world * pPlane)		//��ʼ�����е�spawner��ȥ���������Լ��Ĳ���
{
	_plane = pPlane;
	rect base_region = pPlane->GetGrid().base_region;
//	int total_count = 0;

	//$$$$$$$$$$$����Ķ�����ɾ����

	return true;
}

bool
npc_generator::BeginSpawn()
{
	for(unsigned int i = 0; i < _ctrl_list.size();i ++)
	{
		if(!_ctrl_list[i]->IsAutoSpawn()) continue;
		if(!_ctrl_list[i]->BeginSpawn(_plane)) return false;
	}
	return true;
}

bool
npc_generator::TriggerSpawn(int condition)
{
	//�Ӹ�����˼��˼
	spin_autolock keeper(_tlock);

	spawner_ctrl * ctrl = _ctrl_map[condition];
	if(ctrl == NULL) return false;
	ctrl->BeginSpawn(_plane);
	return true;
}

void
npc_generator::ClearSpawn(int condition)
{
	//�Ӹ�����˼��˼
	spin_autolock keeper(_tlock);

	spawner_ctrl * ctrl = _ctrl_map[condition];
	if(ctrl == NULL) return ;
	ctrl->EndSpawn(_plane);
}


void
npc_generator::Release()
{
	for(unsigned int i = 0; i < _ctrl_list.size();i ++)
	{
		delete _ctrl_list[i];
	}
	_ctrl_list.clear();

	for(unsigned int i = 0; i < _sp_list.size();i ++)
	{
		_sp_list[i]->ReleaseSelf();
	}
	_sp_list.clear();

	_ctrl_map.clear();
}

static int first_load_gen = 0;

bool 
npc_generator::LoadGenData(world* plane, CNPCGenMan & npcgen, rect & world_region)
{
	//��ȡ���ƶ�������
	unsigned int count = npcgen.GetNPCCtrlNum();
	for(unsigned int i = 0; i < count; i ++)
	{
		const CNPCGenMan::CONTROLLER & ctrl = npcgen.GetController(i);
		int id = ctrl.id;
		if(id == 0) 
		{
			__PRINTINFO("������%d�﷢���˷Ƿ���ID 0\n",id);
			return false;
		}

		crontab_t::entry_t act_date,stop_date;
		act_date.min		= ctrl.ActiveTime.iMinutes;
		act_date.hour		= ctrl.ActiveTime.iHours;
		act_date.month		= ctrl.ActiveTime.iMouth;
		act_date.year		= ctrl.ActiveTime.iYear;
		act_date.day_of_months	= ctrl.ActiveTime.iDay;
		act_date.day_of_week	= ctrl.ActiveTime.iWeek;

		stop_date.min		= ctrl.StopTime.iMinutes;
		stop_date.hour		= ctrl.StopTime.iHours;
		stop_date.month		= ctrl.StopTime.iMouth;
		stop_date.year		= ctrl.StopTime.iYear;
		stop_date.day_of_months	= ctrl.StopTime.iDay;
		stop_date.day_of_week	= ctrl.StopTime.iWeek;

		InsertSpawnControl(id, ctrl.iControllerID, ctrl.bActived,ctrl.iWaitTime,ctrl.iStopTime,
					ctrl.iActiveTimeRange,
					!ctrl.bActiveTimeInvalid?&act_date:NULL,
					!ctrl.bStopTimeInvalid?&stop_date:NULL);
	}
	
	//����һ��Ĭ�ϵĿ�����
	InsertSpawnControl(0, 0, true,0,0,0);


	//��ȡNPC�͹�������
	count = npcgen.GetGenAreaNum();
	for(unsigned int i = 0; i <count; i ++)
	{
		const CNPCGenMan::AREA & area = npcgen.GetGenArea(i);
		//__PRINTINFO("ai gen:(%f,%f,%f) (%f,%f,%f)\n",area.vPos[0],area.vPos[1],area.vPos[2],area.vExts[0],area.vExts[1],area.vExts[2]);
		rect rt(area.vPos[0]-area.vExts[0]*.5f, area.vPos[2]-area.vExts[2]*.5f, area.vPos[0]+area.vExts[0]*.5f, area.vPos[2]+area.vExts[2]*.5f);
		if(!world_region.IsOverlap(rt) && !world_region.IsIn(rt))
		{
			continue;
		}
		npc_spawner * pSp;
		if(area.iNPCType)
		{
			pSp = new server_spawner();
		}
		else
		{
			if(area.iGroupType == 0)
			{
				pSp = new mobs_spawner();
			}
			else if(area.iGroupType == 1)
			{
				pSp = new group_spawner();
			}
			else if(area.iGroupType == 2)
			{
				pSp = new boss_spawner();
			}
			else
			{
				pSp = new mobs_spawner();
			}
		}
		pSp->SetRespawn(area.bAutoRevive);
		pSp->SetRegion(area.iType,area.vPos,area.vExts,area.vDir);
		pSp->SetGenLimit(area.iLifeTime,area.iMaxNum);
		
		bool has_collision = false;
		unsigned int entry_count =0;
		for(unsigned int j = (unsigned int)area.iFirstGen; j < (unsigned int)area.iFirstGen + area.iNumGen; j ++)
		{
			const CNPCGenMan::NPCGEN & gen = npcgen.GetGenerator(j);
			npc_spawner::entry_t ent;
			memset(&ent,0,sizeof(ent));
			ent.npc_tid = gen.dwID;
			npc_template * pTemplate = npc_stubs_manager::Get(ent.npc_tid);
			if(pTemplate == NULL)
			{
				__PRINTINFO("�����������Ҳ���ָ����npc entry. npc id %d at pos(%f,%f,%f)\n",ent.npc_tid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}

			if(ent.npc_tid == 4249)
			{
				__PRINTINFO("���������з�����ID����Ĺ�npc id %d at pos(%f,%f,%f)\n",ent.npc_tid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}

			if(pTemplate->role_in_war == 6)
			{
				//��Ǵ��͵�NPC ����NPC���ᱻ����
				if(first_load_gen == 0)
				{
					//��һ��װ�ص�ʱ����븴���ʹ��͵�
					A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
					world_manager::GetInstance()->RecordTownPos(pos,pTemplate->faction);
				}
				continue;
			}

			if(first_load_gen == 0)
			{
				world_manager * pManager = world_manager::GetInstance();
				int cnt = gen.dwNum;
				A3DVECTOR pos(area.vPos[0],area.vPos[1],area.vPos[2]);
				if(pTemplate->role_in_war == 1)
				{
					//��־�Խ���
					pManager->RecordMob(0,ent.npc_tid,pos,pTemplate->faction,cnt);
				}
				else if(area.iNPCType)
				{
					//NPC
					pManager->RecordMob(1,ent.npc_tid,pos,pTemplate->faction,cnt);
				}
				else 
				{
					if(pTemplate->role_in_war)
					{
						//��ս��
						pManager->RecordMob(2,ent.npc_tid,pos,pTemplate->faction,cnt);
					}
					else
					{
						//��ͨ��
						pManager->RecordMob(3,ent.npc_tid,pos,pTemplate->faction,cnt);
					}
				}

			}

			switch(gen.dwAggressive)
			{
				default:
					ASSERT(false);
				case 0:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~0;
					break;
				case 1:
					ent.msg_mask_or = gobject::MSG_MASK_PLAYER_MOVE;
					ent.msg_mask_and = ~0;
					break;
				case 2:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~gobject::MSG_MASK_PLAYER_MOVE;;
					break;
			}
			ent.alarm_mask = 0;
			ent.enemy_faction = 0;		//not use
			ent.has_faction = !gen.bDefFaction;
			ent.faction = gen.dwFaction;
			ent.ask_for_help = !gen.bDefFacHelper;
			ent.monster_faction_ask_help = gen.dwFacHelper;
			ent.accept_ask_for_help = !gen.bDefFacAccept;
			ent.monster_faction_accept_for_help = gen.bDefFacAccept;

			ent.corpse_delay = gen.iDeadTime;
			if(ent.corpse_delay)
			{
				bool flag = (ent.corpse_delay < 10) || (ent.corpse_delay > 240);
				if(ent.corpse_delay < 10) ent.corpse_delay = 10;
				if(ent.corpse_delay > 3600*3) ent.corpse_delay = 3600*3;
				if(flag) __PRINTINFO("���ִ����ʬ�����ʱ�� npc id %d at pos(%f,%f,%f) ʱ��%d\n",
						ent.npc_tid,
						area.vPos[0],area.vPos[1],area.vPos[2] ,gen.iDeadTime);
			}
			
			
			//����̶�����ʱ���Ժ���֮
			int reborn_time_upper, reborn_time_lower;
			if(gen.iRefresh >= 0)
			{
				reborn_time_upper = BASE_REBORN_TIME + gen.iRefresh;	
				if(gen.iRefreshLower > 0 && gen.iRefreshLower < gen.iRefresh) 
					reborn_time_lower = BASE_REBORN_TIME + gen.iRefreshLower;
				else 
					reborn_time_lower = reborn_time_upper;
			}
			else
			{
				reborn_time_upper = reborn_time_lower = -gen.iRefresh + 3;
			}
			if(reborn_time_upper > 2592000) reborn_time_upper = 2592000;
			if(reborn_time_lower > 2592000) reborn_time_lower = 2592000;
			ent.reborn_time_upper = reborn_time_upper * 20;
			ent.reborn_time_lower = reborn_time_lower * 20;
			ent.mobs_count = gen.dwNum;
			ent.offset_terrain = gen.fOffsetTrn;
			ent.path_id = gen.iPathID;
			ent.path_type = gen.iLoopType;
			ent.speed_flag = gen.iSpeedFlag;

#ifdef __TEST_PERFORMANCE__
			ent.msg_mask_and = ~0;
			ent.mobs_count *= 2;
#endif
			pSp->AddEntry(ent);
			if(j == (unsigned int)area.iFirstGen)
			{
				//ΪȺ��׼���ģ������Ⱥ�֣�ʹ�õ�һ������ĸ���ʱ��
				pSp->SetRebornTime(ent.reborn_time_upper);
			}
			entry_count  ++;

			if(pTemplate->has_collision) has_collision = true; 
		}
		if(has_collision) pSp->BuildRegionCollision(plane, i);
		if(entry_count)
		{
			InsertSpawner(area.idCtrl,pSp);
		}
		else 
			delete pSp;
	}

	//�������
	count = npcgen.GetResAreaNum();
	for(unsigned int i = 0; i <count; i ++)
	{
		const CNPCGenMan::RESAREA & area = npcgen.GetResArea(i);
		//rect rt(area.vPos[0]-area.vExts[0]*.5f, area.vPos[2]-area.vExts[2]*.5f, area.vPos[0]+area.vExts[0]*.5f, area.vPos[2]+area.vExts[2]*.5f);
		rect rt(area.vPos[0]-area.fExtX*0.5f, area.vPos[2]-area.fExtZ*0.5f, area.vPos[0]+area.fExtX*.5f, area.vPos[2]+area.fExtZ*.5f);
		if(!world_region.IsOverlap(rt) && !world_region.IsIn(rt))
		{
			continue;
		}
		mine_spawner * pSp;
		pSp = new mine_spawner();
		float vExt[3] = {area.fExtX,0,area.fExtZ};
		float vDir[3] = {0,1,0};
		pSp->SetRegion(0,area.vPos,vExt,vDir); 
		pSp->SetDir(area.dir[0],area.dir[1], area.rad);
		pSp->SetRespawn(area.bAutoRevive); 
		pSp->SetGenLimit(area.iMaxNum);

		unsigned int entry_count = 0;
		for(unsigned int j = (unsigned int)area.iFirstRes; j < (unsigned int)area.iFirstRes+ area.iResNum; j ++)
		{
			const CNPCGenMan::RES & res = npcgen.GetRes(j);
			mine_spawner::entry_t ent;
			ent.mid = res.idTemplate;
			if(npc_stubs_manager::Get(ent.mid) == NULL)
			{
				__PRINTINFO("�����������Ҳ���ָ����mine entry. id %d at pos(%f,%f,%f)\n",ent.mid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}

			ent.mine_count = res.dwNumber;
			int reborn_time = BASE_REBORN_TIME + res.dwRefreshTime;
			if(reborn_time < 15) reborn_time = 15;
			ent.reborn_time = reborn_time * 20;
			pSp->SetOffsetTerrain(res.fHeiOff);
			pSp->AddEntry(ent);
			entry_count ++;
		}
		pSp->BuildRegionCollision(plane, i|0x40000000);
		if(entry_count)
		{
			InsertSpawner(area.idCtrl,pSp);
		}
		else 
			delete pSp;
	}

	//���붯̬��Ʒ
	count = npcgen.GetDynObjectNum();
	for(unsigned int i = 0; i <count; i ++)               
	{
		const CNPCGenMan::DYNOBJ & obj = npcgen.GetDynObject(i);
		if(!world_region.IsIn(obj.vPos[0], obj.vPos[2]))
		{
			continue;
		}
		dyn_object_spawner * pSp;
		pSp = new dyn_object_spawner();
		float vExt[3] = {1e-3,1e-3,1e-3}; 
		float vDir[3] = {0,1,0};
		pSp->SetRegion(1,obj.vPos,vExt,vDir); 
		pSp->SetDir(obj.dir[0],obj.dir[1], obj.rad);
		pSp->SetScale(obj.scale);
		pSp->SetRespawn(true); 

		mine_spawner::entry_t ent;
		memset(&ent,0,sizeof(ent));
		ent.mid = obj.dwDynObjID;
		ent.mine_count = 1;
		ent.reborn_time = 100;
		pSp->AddEntry(ent); 
		pSp->BuildRegionCollision(plane,i|0x80000000);

		InsertSpawner(obj.idCtrl,pSp);
	}       
	first_load_gen = 1;
	return true;
}

bool 
npc_generator::AddCtrlData(CNPCGenMan& ctrldata,unsigned int ctrl_id, unsigned char block_id)
{
	int unq_cid = GenBlockUniqueID(ctrl_id,block_id);	
	
	if(_ctrl_idx_map.find(unq_cid) != _ctrl_idx_map.end()) // �Ѿ�ע��Ŀ�����
		return true;

	//ע��spawn���������
	unsigned int count = ctrldata.GetNPCCtrlNum();
	for(unsigned int i = 0; i < count; i ++)
	{
		const CNPCGenMan::CONTROLLER & ctrl = ctrldata.GetController(i);
		
		if(ctrl.id == ctrl_id)
		{
			crontab_t::entry_t act_date,stop_date;
			act_date.min		= ctrl.ActiveTime.iMinutes;
			act_date.hour		= ctrl.ActiveTime.iHours;
			act_date.month		= ctrl.ActiveTime.iMouth;
			act_date.year		= ctrl.ActiveTime.iYear;
			act_date.day_of_months	= ctrl.ActiveTime.iDay;
			act_date.day_of_week	= ctrl.ActiveTime.iWeek;

			stop_date.min		= ctrl.StopTime.iMinutes;
			stop_date.hour		= ctrl.StopTime.iHours;
			stop_date.month		= ctrl.StopTime.iMouth;
			stop_date.year		= ctrl.StopTime.iYear;
			stop_date.day_of_months	= ctrl.StopTime.iDay;
			stop_date.day_of_week	= ctrl.StopTime.iWeek;

			if(false == InsertSpawnControl(unq_cid, 
						GenBlockUniqueID(ctrl.iControllerID,block_id), 
					    ctrl.bActived, ctrl.iWaitTime,
						ctrl.iStopTime,	ctrl.iActiveTimeRange,
						!ctrl.bActiveTimeInvalid?&act_date:NULL,
						!ctrl.bStopTimeInvalid?&stop_date:NULL))
			{
				__PRINTINFO("��̬���ӿ�����ID=%d����ID=%d block_id=%d  unique_id=[%d][%d]�ظ�\n",
						ctrl.id, ctrl.iControllerID, block_id, unq_cid,
						GenBlockUniqueID(ctrl.iControllerID,block_id));
				return false;			
			}
			return true;
		}

	}	
	
	return false;
}

bool 
npc_generator::AddSpawnData(world* plane, CNPCGenMan& ctrldata, CNPCGenMan& spawndata, unsigned char block_id, const A3DVECTOR& p_offset,bool global_ctrl_gen, bool unique_resource)
{
	
	//��ȡNPC�͹�������
	unsigned int	count = spawndata.GetGenAreaNum();
	for(unsigned int i = 0; i <count; i ++)
	{
		CNPCGenMan::AREA  area = spawndata.GetGenArea(i);
		
		if(area.idCtrl && !AddCtrlData(ctrldata,area.idCtrl,block_id))
		{
			__PRINTINFO("����pos(%f,%f,%f)����Ϊ%d �ķ�����������NPC������%d������\n",
					area.vPos[0],area.vPos[1],area.vPos[2],area.iType,area.idCtrl);
			continue;
		}
		if(global_ctrl_gen == false && !area.idCtrl )
		{
			continue;
		}
		
		area.vPos[0] += p_offset.x;
		area.vPos[1] += p_offset.y;
		area.vPos[2] += p_offset.z;

		npc_spawner * pSp;
		if(area.iNPCType)
		{
			pSp = new server_spawner();
		}
		else
		{
			if(area.iGroupType == 0)
			{
				pSp = new mobs_spawner();
			}
			else if(area.iGroupType == 1)
			{
				pSp = new group_spawner();
			}
			else if(area.iGroupType == 2)
			{
				pSp = new boss_spawner();
			}
			else
			{
				pSp = new mobs_spawner();
			}
		}
		pSp->SetRespawn(area.bAutoRevive);
		pSp->SetRegion(area.iType,area.vPos,area.vExts,area.vDir);
		pSp->SetGenLimit(area.iLifeTime,area.iMaxNum);
		
		bool has_collision = false;
		unsigned int entry_count =0;
		for(unsigned int j = (unsigned int)area.iFirstGen; j < (unsigned int)area.iFirstGen + area.iNumGen; j ++)
		{
			const CNPCGenMan::NPCGEN & gen = spawndata.GetGenerator(j);
			npc_spawner::entry_t ent;
			memset(&ent,0,sizeof(ent));
			ent.npc_tid = gen.dwID;
			npc_template * pTemplate = npc_stubs_manager::Get(ent.npc_tid);
			
			if(pTemplate == NULL)
			{
				__PRINTINFO("�����������Ҳ���ָ����npc entry. npc id %d at pos(%f,%f,%f)\n",ent.npc_tid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}

			switch(gen.dwAggressive)
			{
				default:
					ASSERT(false);
				case 0:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~0;
					break;
				case 1:
					ent.msg_mask_or = gobject::MSG_MASK_PLAYER_MOVE;
					ent.msg_mask_and = ~0;
					break;
				case 2:
					ent.msg_mask_or = 0;
					ent.msg_mask_and = ~gobject::MSG_MASK_PLAYER_MOVE;;
					break;
			}
			ent.alarm_mask = 0;
			ent.enemy_faction = 0;		//not use
			ent.has_faction = !gen.bDefFaction;
			ent.faction = gen.dwFaction;
			ent.ask_for_help = !gen.bDefFacHelper;
			ent.monster_faction_ask_help = gen.dwFacHelper;
			ent.accept_ask_for_help = !gen.bDefFacAccept;
			ent.monster_faction_accept_for_help = gen.bDefFacAccept;

			ent.corpse_delay = gen.iDeadTime;
			if(ent.corpse_delay)
			{
				bool flag = (ent.corpse_delay < 10) || (ent.corpse_delay > 240);
				if(ent.corpse_delay < 10) ent.corpse_delay = 10;
				if(ent.corpse_delay > 3600*3) ent.corpse_delay = 3600*3;
				if(flag) __PRINTINFO("���ִ����ʬ�����ʱ�� npc id %d at pos(%f,%f,%f) ʱ��%d\n",
						ent.npc_tid,
						area.vPos[0],area.vPos[1],area.vPos[2] ,gen.iDeadTime);
			}
			
			
			//����̶�����ʱ���Ժ���֮
			int reborn_time_upper, reborn_time_lower;
			if(gen.iRefresh >= 0)
			{
				reborn_time_upper = BASE_REBORN_TIME + gen.iRefresh;	
				if(gen.iRefreshLower > 0 && gen.iRefreshLower < gen.iRefresh) 
					reborn_time_lower = BASE_REBORN_TIME + gen.iRefreshLower;
				else 
					reborn_time_lower = reborn_time_upper;
			}
			else
			{
				reborn_time_upper = reborn_time_lower = -gen.iRefresh + 3;
			}
			if(reborn_time_upper > 2592000) reborn_time_upper = 2592000;
			if(reborn_time_lower > 2592000) reborn_time_lower = 2592000;
			ent.reborn_time_upper = reborn_time_upper * 20;
			ent.reborn_time_lower = reborn_time_lower * 20;
			ent.mobs_count = gen.dwNum;
			ent.offset_terrain = gen.fOffsetTrn;
			ent.path_id = gen.iPathID;
			ent.path_type = gen.iLoopType;
			ent.speed_flag = gen.iSpeedFlag;

#ifdef __TEST_PERFORMANCE__
			ent.msg_mask_and = ~0;
			ent.mobs_count *= 2;
#endif
			pSp->AddEntry(ent);
			if(j == (unsigned int)area.iFirstGen)
			{
				//ΪȺ��׼���ģ������Ⱥ�֣�ʹ�õ�һ������ĸ���ʱ��
				pSp->SetRebornTime(ent.reborn_time_upper);
			}
			entry_count  ++;

			if(pTemplate->has_collision) has_collision = true; 
		}
		if(has_collision) 
		{
			if(unique_resource)
				pSp->BuildRegionCollision(plane, i);
			else
				pSp->BuildRegionCollision2(plane);	
		}
		if(entry_count)
		{
			InsertSpawner(area.idCtrl ? GenBlockUniqueID(area.idCtrl,block_id) : 0,pSp);
		}
		else 
			delete pSp;
	}

	//�������
	count = spawndata.GetResAreaNum();
	for(unsigned int i = 0; i <count; i ++)
	{
		CNPCGenMan::RESAREA area = spawndata.GetResArea(i);
		
		if(area.idCtrl && !AddCtrlData(ctrldata,area.idCtrl,block_id))
		{
			__PRINTINFO("����pos(%f,%f,%f) �Ŀ�������������Ŀ�����%d������\n",
					area.vPos[0],area.vPos[1],area.vPos[2],area.idCtrl);
			continue;
		}	
		
		if(global_ctrl_gen == false && !area.idCtrl )
		{
			continue;
		}

		area.vPos[0] += p_offset.x;
		area.vPos[1] += p_offset.y;
		area.vPos[2] += p_offset.z;

		mine_spawner * pSp;
		pSp = new mine_spawner();
		float vExt[3] = {area.fExtX,0,area.fExtZ};
		float vDir[3] = {0,1,0};
		pSp->SetRegion(0,area.vPos,vExt,vDir); 
		pSp->SetDir(area.dir[0],area.dir[1], area.rad);
		pSp->SetRespawn(area.bAutoRevive); 
		pSp->SetGenLimit(area.iMaxNum);

		unsigned int entry_count = 0;
		for(unsigned int j = (unsigned int)area.iFirstRes; j < (unsigned int)area.iFirstRes+ area.iResNum; j ++)
		{
			const CNPCGenMan::RES & res = spawndata.GetRes(j);
			mine_spawner::entry_t ent;
			ent.mid = res.idTemplate;
			if(npc_stubs_manager::Get(ent.mid) == NULL)
			{
				__PRINTINFO("�����������Ҳ���ָ����mine entry. id %d at pos(%f,%f,%f)\n",ent.mid,area.vPos[0],area.vPos[1],area.vPos[2]);
				continue;
			}

			ent.mine_count = res.dwNumber;
			int reborn_time = BASE_REBORN_TIME + res.dwRefreshTime;
			if(reborn_time < 15) reborn_time = 15;
			ent.reborn_time = reborn_time * 20;
			pSp->SetOffsetTerrain(res.fHeiOff);
			pSp->AddEntry(ent);
			entry_count ++;
		}
		if(unique_resource)
			pSp->BuildRegionCollision(plane, i|0x40000000);
		else
			pSp->BuildRegionCollision2(plane);	

		if(entry_count)
		{
			InsertSpawner(area.idCtrl ? GenBlockUniqueID(area.idCtrl,block_id) : 0,pSp);
		}
		else 
			delete pSp;
	}

	//���붯̬��Ʒ
	count = spawndata.GetDynObjectNum();
	for(unsigned int i = 0; i <count; i ++)               
	{
		CNPCGenMan::DYNOBJ obj = spawndata.GetDynObject(i);
		
		if(obj.idCtrl && !AddCtrlData(ctrldata,obj.idCtrl,block_id))
		{
			__PRINTINFO("����pos(%f,%f,%f)����Ϊ%d �Ķ�̬��Ʒ�����������Ŀ�����%d������\n",
					obj.vPos[0],obj.vPos[1],obj.vPos[2],(int)obj.dwDynObjID,obj.idCtrl);			
			continue;
		}	
		
		if(global_ctrl_gen == false && !obj.idCtrl )
		{
			continue;
		}

		obj.vPos[0] += p_offset.x;		
		obj.vPos[1] += p_offset.y;
		obj.vPos[2] += p_offset.z;

		dyn_object_spawner * pSp;
		pSp = new dyn_object_spawner();
		float vExt[3] = {1e-3,1e-3,1e-3}; 
		float vDir[3] = {0,1,0};
		pSp->SetRegion(1,obj.vPos,vExt,vDir); 
		pSp->SetDir(obj.dir[0],obj.dir[1], obj.rad);
		pSp->SetScale(obj.scale);
		pSp->SetRespawn(true); 

		mine_spawner::entry_t ent;
		memset(&ent,0,sizeof(ent));
		ent.mid = obj.dwDynObjID;
		ent.mine_count = 1;
		ent.reborn_time = 100;
		pSp->AddEntry(ent);

		if(unique_resource)
			pSp->BuildRegionCollision(plane,i|0x80000000);
		else
			pSp->BuildRegionCollision2(plane);

		InsertSpawner(obj.idCtrl ? GenBlockUniqueID(obj.idCtrl,block_id) : 0,pSp);
	}       

	if(unique_resource) 
		first_load_gen = 1;

	return true;
}

namespace
{
	class terrain_gen_pos : public npc_spawner::generate_pos
	{
		public:
		virtual void Generate(const A3DVECTOR &pos_min,const A3DVECTOR &pos_max,A3DVECTOR &pos,float offset, world * plane)
		{
			int n = 0;
			do
			{
				pos.x = abase::Rand(pos_min.x,pos_max.x);
				pos.z = abase::Rand(pos_min.z,pos_max.z);
				pos.y = offset;
				if(path_finding::GetValidPos(plane, pos))
				{
					float h = plane->GetHeightAt(pos.x,pos.z);
					pos.y += h;
					return;
				}
				n ++;
			}while(n < 5);
			//__PRINTINFO("��������ײ���������\n");
			//ע��ˮ��Ĺ���һ�������
			pos.y += plane->GetHeightAt(pos.x,pos.z);
		}
		virtual float GenerateY(float x, float y, float z,float offset, world * plane)
		{
			y = plane->GetHeightAt(x,z);
			return y + offset;
		}
	};

	class box_gen_pos : public npc_spawner::generate_pos
	{
		public:
		virtual void Generate(const A3DVECTOR &pos_min,const A3DVECTOR &pos_max,A3DVECTOR &pos,float offset, world * plane)
		{
			pos.x = abase::Rand(pos_min.x,pos_max.x);
			pos.y = abase::Rand(pos_min.y,pos_max.y);
			pos.z = abase::Rand(pos_min.z,pos_max.z);
			float height = plane->GetHeightAt(pos.x,pos.z);
			if(pos.y < height) pos.y = height;
			pos.y += offset;
		}

		virtual float GenerateY(float x, float y, float z, float offset, world * plane)
		{
			float height = plane->GetHeightAt(x,z);
			if(y < height) y = height;
			return y+offset;
		}
	};
}
void base_spawner::SetRegion(int region_type,const float vPos[3],const float vExts[3],const float vDir[3])
{
	ASSERT(_pos_generator == NULL);
	switch(region_type)
	{
		case 0:
			_pos_generator = new terrain_gen_pos();
			break;
		case 1:
			_pos_generator = new box_gen_pos();
			break;
		default:
			__PRINTINFO("�������������\n");
			ASSERT(false);
			break;
	}
	_region = rect(vPos[0]-vExts[0]*.5f, vPos[2]-vExts[2]*.5f, vPos[0]+vExts[0]*.5f, vPos[2]+vExts[2]*.5f);
	_pos_min = A3DVECTOR(vPos[0]-vExts[0]*.5f,vPos[1]-vExts[1]*.5f,vPos[2]-vExts[2]*.5f);
	_pos_max = A3DVECTOR(vPos[0]+vExts[0]*.5f,vPos[1]+vExts[1]*.5f,vPos[2]+vExts[2]*.5f);
	_dir = a3dvector_to_dir(vDir);
}

void 
base_spawner::BuildRegionCollision(world * plane, int tid, int did, float offset_terrain, int region_idx)
{
	if(first_load_gen == 0)
	{
		A3DVECTOR pos;
		GeneratePos(pos, offset_terrain,plane);
		_collision_id = world_manager::GetMapRes().GetUniqueTraceMan()->RegisterElement(tid, did, pos, _dir*(2*3.1415926535/256.f),_dir1*(2*3.1415926535/256.f), _rad*(2*3.1415926535/256.f));
		if(_collision_id > 0) world_manager::MapRegionCollisionId(region_idx,_collision_id);
	}
	else
	{
		_collision_id = world_manager::GetRegionCollisionId(region_idx);
	}
}

void 
npc_spawner::BuildRegionCollision(world * plane, int region_idx)
{
	if(_pos_min.squared_distance(_pos_max) < 1e-3 && _entry_list.size() == 1 && _entry_list[0].mobs_count == 1)
		base_spawner::BuildRegionCollision(plane, _entry_list[0].npc_tid, 0, _entry_list[0].offset_terrain, region_idx);
}

void 
mine_spawner::BuildRegionCollision(world * plane, int region_idx)
{
	if(_pos_min.squared_distance(_pos_max) < 1e-3 && _entry_list.size() == 1 && _entry_list[0].mine_count == 1)
		base_spawner::BuildRegionCollision(plane, 0, _entry_list[0].mid, _offset_terrain, region_idx);
}

void 
base_spawner::BuildRegionCollision2(world * plane, int tid, int did, float offset_terrain)
{
	A3DVECTOR pos;
	GeneratePos(pos, offset_terrain,plane);
	_collision_id = plane->GetTraceMan()->RegisterElement(tid, did, pos, _dir*(2*3.1415926535/256.f),_dir1*(2*3.1415926535/256.f), _rad*(2*3.1415926535/256.f));
}

void 
npc_spawner::BuildRegionCollision2(world * plane)
{
	if(_pos_min.squared_distance(_pos_max) < 1e-3 && _entry_list.size() == 1 && _entry_list[0].mobs_count == 1)
		base_spawner::BuildRegionCollision2(plane, _entry_list[0].npc_tid, 0, _entry_list[0].offset_terrain);
}

void 
mine_spawner::BuildRegionCollision2(world * plane)
{
	if(_pos_min.squared_distance(_pos_max) < 1e-3 && _entry_list.size() == 1 && _entry_list[0].mine_count == 1)
		base_spawner::BuildRegionCollision2(plane, 0, _entry_list[0].mid, _offset_terrain);
}

void 
server_spawner::ReCreate(world * pPlane, gnpc * pNPC,const A3DVECTOR & pos, int index)
{
	XID oldID = pNPC->ID;
	pNPC->Clear();
	pNPC->SetActive();
	pNPC->ID = oldID;
	CreateNPC(pPlane,index,_entry_list[index],pos,pNPC);
}

gnpc *
server_spawner::CreateNPC(world * pPlane, int index,const entry_t & et)
{
	A3DVECTOR pos;
	GeneratePos(pos,et.offset_terrain,pPlane);
	return CreateNPC(pPlane,index,et,pos,NULL);
}
	
gnpc * 
server_spawner::CreateNPC(world * pPlane, int index,const entry_t & et, const A3DVECTOR & pos, gnpc * origin_npc)
{
	int cid[3]= {-1,-1,-1};
	gnpc * pNPC = CreateNPCBase(this, pPlane, et, index , pos, cid, _dir, -1,0,origin_npc,_mob_life);
	if(!pNPC) return NULL; 
	pNPC->collision_id = _collision_id;

	pPlane->InsertNPC(pNPC);
	pNPC->imp->OnNpcEnterWorld();
	pNPC->imp->_runner->enter_world();
	if(!origin_npc)
	{
		//ֻ�е��ⲿû��NPC��ʱ�����ô��
		//��Ϊ������ⲿ�ģ���ôӦ���Ѿ�������
		pNPC->Unlock();
	}
	return pNPC;
}
/*{
	npc_template * pTemplate = npc_stubs_manager::Get(et.npc_tid);
	ASSERT(pTemplate);
	if(!pTemplate) return NULL; 

	const int cid[3] = {CLS_SERVICE_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	int aipolicy = 0;
	int aggro_policy = 0;
	switch(pTemplate->npc_data.npc_type)
	{
		case npc_template::npc_statement::NPC_TYPE_GUARD:
			aipolicy = CLS_NPC_AI_POLICY_GUARD;
			break;
		case npc_template::npc_statement::NPC_TYPE_NORMAL:
		default:
			//��ͨ�Ĳ����κδ�������
			break;
	}

	gnpc * pNPC = CreateMobBase(this,pPlane,et, index,pos,cid,_dir,aipolicy,aggro_policy,origin_npc,_mob_life);
	if(!pNPC) return NULL;
	
	//������ַ���
	ASSERT(pNPC->imp->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(service_npc)));
	service_npc * pImp = (service_npc *)pNPC->imp;
	pImp->SetTaxRate(pTemplate->npc_data.tax_rate);
	pImp->SetNeedDomain(pTemplate->npc_data.need_domain);
	if(int num = pTemplate->npc_data.service_sell_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_VENDOR);
		pImp->AddProvider(provider,pTemplate->npc_data.service_sell_goods,sizeof(int)*6*num);
	}

	if(pTemplate->npc_data.service_purchase)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_NPC_PURCHASE);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_repair)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_REPAIR);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_heal)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_HEAL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(int num = pTemplate->npc_data.service_transmit_target_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRANSMIT);
		pImp->AddProvider(provider,pTemplate->npc_data.transmit_entry, sizeof(npc_template::npc_statement::__st_ent)* num);
	}

	if(pTemplate->npc_data.service_task_in_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_IN);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_in_list,pTemplate->npc_data.service_task_in_num*sizeof(int));
	}

	if(pTemplate->npc_data.service_task_out_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_OUT);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_out_list,pTemplate->npc_data.service_task_out_num*sizeof(int));
	}

	if(pTemplate->npc_data.service_task_matter_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TASK_MATTER);
		pImp->AddProvider(provider,pTemplate->npc_data.service_task_matter_list,pTemplate->npc_data.service_task_matter_num*sizeof(int));
	}

	if(int num = pTemplate->npc_data.service_teach_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_SKILL);
		pImp->AddProvider(provider,pTemplate->npc_data.service_teach_skill_list, sizeof(int)*num);
	}

	
	if(pTemplate->npc_data.service_install)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_INSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_uninstall)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_UNINSTALL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_produce.produce_skill >=0 && pTemplate->npc_data.service_produce.produce_num > 0) 
	{
		npc_template::npc_statement::__service_produce &te = pTemplate->npc_data.service_produce;
		int service_id = te.type?46:service_ns::SERVICE_ID_PRODUCE;
		service_provider * provider = service_manager::CreateProviderInstance(service_id);
		pImp->AddProvider(provider,&te,sizeof(te));
	}

	if(pTemplate->npc_data.service_decompose_skill >=0) 
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_DECOMPOSE);
		int skill = pTemplate->npc_data.service_decompose_skill;
		pImp->AddProvider(provider,&skill,4);
	}

	if(pTemplate->npc_data.service_storage)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_PASS);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_TRASHBOX_OPEN);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_identify)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_IDENTIFY);
		pImp->AddProvider(provider,&pTemplate->npc_data.service_identify_fee,sizeof(int));
	}

	if(pTemplate->npc_data.service_vehicle_count)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_VEHICLE);
		pImp->AddProvider(provider,&pTemplate->npc_data.vehicle_path_list,
				sizeof(npc_template::npc_statement::vehicle_path_entry));
	}

	if(pTemplate->npc_data.service_waypoint_id > 0)
	{
		service_provider * provider=service_manager::CreateProviderInstance(service_ns::SERVICE_ID_WAYPOINT);
		int wp = pTemplate->npc_data.service_waypoint_id ;
		pImp->AddProvider(provider,&wp,sizeof(wp));
	}

	if(pTemplate->npc_data.service_unlearn_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(service_ns::SERVICE_ID_UNLEARN_SKILL);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_faction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(18);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_face_ticket)
	{
		service_provider * provider = service_manager::CreateProviderInstance(24);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_mail)
	{
		service_provider * provider = service_manager::CreateProviderInstance(25);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_auction)
	{
		service_provider * provider = service_manager::CreateProviderInstance(26);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_double_exp)
	{
		service_provider * provider = service_manager::CreateProviderInstance(27);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_hatch_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(28);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_recover_pet)
	{
		service_provider * provider = service_manager::CreateProviderInstance(29);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_war_management)
	{
		service_provider * provider = service_manager::CreateProviderInstance(30);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.npc_tower_build_size)
	{
		service_provider * provider = service_manager::CreateProviderInstance(31);
		pImp->AddProvider(provider,pTemplate->npc_data.npc_tower_build,sizeof(pTemplate->npc_data.npc_tower_build));
	}

	if(pTemplate->npc_data.service_war_leave_battle)
	{
		service_provider * provider = service_manager::CreateProviderInstance(32);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_reset_prop_count)
	{
		unsigned int size = pTemplate->npc_data.service_reset_prop_count;
		service_provider * provider = service_manager::CreateProviderInstance(33);
		pImp->AddProvider(provider,pTemplate->npc_data.reset_prop,size * sizeof(npc_template::npc_statement::__reset_prop));
	}

	if(pTemplate->npc_data.service_cash_trade)
	{
		service_provider * provider = service_manager::CreateProviderInstance(42);
		pImp->AddProvider(provider,NULL,0);

		provider = service_manager::CreateProviderInstance(43);
		pImp->AddProvider(provider,NULL,0);
	}
	
	if(pTemplate->npc_data.service_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(35);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_change_pet_name)
	{
		service_provider * provider = service_manager::CreateProviderInstance(36);
		pImp->AddProvider(provider,&pTemplate->npc_data.change_pet_name_prop,sizeof(int)*2);
	}

	if(pTemplate->npc_data.service_forget_pet_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(37);
		pImp->AddProvider(provider,&pTemplate->npc_data.forget_pet_skill_prop,sizeof(int)*2);
	}

	if(int num = pTemplate->npc_data.service_pet_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(38);
		pImp->AddProvider(provider,pTemplate->npc_data.service_pet_skill_list, sizeof(int)*num);
	}

	if( pTemplate->npc_data.service_equip_bind)
	{
		int m[] = {	pTemplate->npc_data.service_bind_prop.money_need,
				pTemplate->npc_data.service_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(39);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if( pTemplate->npc_data.service_destroy_bind)
	{
		int m[] = {	pTemplate->npc_data.service_destroy_bind_prop.money_need,
				pTemplate->npc_data.service_destroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(40);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if( pTemplate->npc_data.service_undestroy_bind)
	{
		int m[] = {	pTemplate->npc_data.service_undestroy_bind_prop.money_need,
				pTemplate->npc_data.service_undestroy_bind_prop.item_need};
		service_provider * provider = service_manager::CreateProviderInstance(41);
		pImp->AddProvider(provider,m ,sizeof(m));
	}

	if(pTemplate->npc_data.service_dye)
	{
		service_provider * provider = service_manager::CreateProviderInstance(44);
		pImp->AddProvider(provider,NULL,0);
	}

	if(pTemplate->npc_data.service_refine_transmit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(45);
		pImp->AddProvider(provider,NULL,0);
	}

	if(1 || pTemplate->npc_data.service_make_slot)
	{
		service_provider * provider = service_manager::CreateProviderInstance(47);
		pImp->AddProvider(provider,NULL,0);
	}

	//lgc
	if(pTemplate->npc_data.service_elf_dec_attributie)
	{
		service_provider * provider = service_manager::CreateProviderInstance(48);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_flush_genius)
	{
		service_provider * provider = service_manager::CreateProviderInstance(49);
		pImp->AddProvider(provider,NULL,0);
	}
	if(int num = pTemplate->npc_data.service_elf_learn_skill_num)
	{
		service_provider * provider = service_manager::CreateProviderInstance(50);
		pImp->AddProvider(provider,pTemplate->npc_data.service_elf_learn_skill_list, sizeof(int)*num);
	}
	if(pTemplate->npc_data.service_elf_forget_skill)
	{
		service_provider * provider = service_manager::CreateProviderInstance(51);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_refine)
	{
		service_provider * provider = service_manager::CreateProviderInstance(52);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_refine_transmit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(53);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_decompose)
	{
		service_provider * provider = service_manager::CreateProviderInstance(54);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_elf_destroy_item)
	{
		service_provider * provider = service_manager::CreateProviderInstance(55);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_dye_suit)
	{
		service_provider * provider = service_manager::CreateProviderInstance(56);
		pImp->AddProvider(provider,NULL,0);
	}
	if(pTemplate->npc_data.service_repair_damaged_item)
	{
		service_provider * provider = service_manager::CreateProviderInstance(57);
		pImp->AddProvider(provider,NULL,0);
	}
	

	pPlane->InsertNPC(pNPC);
	pNPC->imp->_runner->enter_world();
	if(!origin_npc)
	{
		//ֻ�е��ⲿû��NPC��ʱ�����ô��
		//��Ϊ������ⲿ�ģ���ôӦ���Ѿ�������
		pNPC->Unlock();
	}
	return pNPC;
}*/

/*
void 
server_spawner::Reborn(world * pPlane, gnpc * header, gnpc * tail,float height,int tid)
{
	ASSERT(false);
}
*/

void 
server_spawner::OnHeartbeat(world * pPlane)
{
	//��ʲôҲ����
	mobs_spawner::OnHeartbeat(pPlane);
	return;
}

bool 
server_spawner::CreateMobs(world * pPlane)
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		unsigned int count = _entry_list[i].mobs_count;
		for(unsigned int j = 0; j < count; j ++)
		{
			gnpc *pNPC = CreateNPC(pPlane,i,_entry_list[i]);
			if(pNPC == NULL) return false;
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
		}
	}
	return true;
}

bool
server_spawner::Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp,bool is_reset)
{
	//Ҫ����֮
	//pImp->_commander->Release();
	__PRINTF("����NPC������\n");
	return mobs_spawner::Reclaim(pPlane,pNPC,pImp, is_reset);
}

void 
mine_spawner::Reborn(world * pPlane,gmatter * header, gmatter * tail,int mid,int index)
{
	do
	{
		gmatter * pMatter = header;
		header = (gmatter*)header->pNext;
		ASSERT(pMatter->pPiece == NULL);
		ASSERT(pMatter->matter_type == mid);

		GeneratePos(pMatter->pos,_offset_terrain,pPlane);
		pMatter->spawn_index = index;

		pMatter->Lock();

		if(_is_spawned)
		{
			pMatter->SetActive();
			pMatter->pPrev = pMatter->pNext = NULL;

			npc_template * pTemplate = npc_stubs_manager::Get(mid);
			GenerateMineParam((gmatter_mine_imp*)pMatter->imp, pTemplate);

			pPlane->InsertMatter(pMatter);
			pMatter->imp->Reborn();
			ASSERT(((gmatter_mine_imp*)(pMatter->imp))->_spawner == this);
		}
		else
		{
			//�����spawner�Ѿ����ڷ�����״̬����ôֱ���ͷſ������������
			pMatter->imp->_commander->Release();
		}
		pMatter->Unlock();
		__PRINTF("���%d ���²�����\n", pMatter->ID.id & 0x7FFFFFF);

	} while(header != tail);
}

void 
mine_spawner::Release()
{
	spin_autolock  keeper(_pool_lock);
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gmatter * header = _mine_pool[i];
		if(!header) continue;

		gmatter * pTmp = (gmatter *)header;
		do
		{
			gmatter * pNext = (gmatter*)pTmp->pNext;
			spin_autolock keeper(pTmp->spinlock);
			pTmp->imp->_commander->Release();
			pTmp = pNext;
		}
		while(pTmp!= header);
		_mine_pool[i] = NULL;
	}
	_entry_list.clear();
	_mine_pool.clear();
}

void 
mine_spawner::OnHeartbeat(world * pPlane)
{
	int tick = g_timer.get_tick();
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gmatter * header = _mine_pool[i];
		if(!header) continue;

		if(header->spawn_index - tick > 0) continue;
		gmatter * pTmp =(gmatter*) header->pNext;
		while(pTmp != header && pTmp->spawn_index - tick <= 0)
		{
			pTmp = (gmatter*)pTmp->pNext;
		}
		if(pTmp == header)
		{
			_mine_pool[i] = NULL;
		}
		else
		{
			_mine_pool[i] = pTmp;
			header->pPrev->pNext = pTmp;
			pTmp ->pPrev = header->pPrev;
		}

		//��ʼ����
		Reborn(pPlane,header,pTmp,_entry_list[i].mid,i);
	}
}

bool
mine_spawner::Reclaim(world * pPlane,gmatter * pMatter, gmatter_mine_imp * imp)
{
	if(!_auto_respawn || !_is_spawned)
	{
		imp->_commander->Release();
		return false;
	}

	slice *pPiece = pMatter->pPiece;
	if(pPiece)
	{
		pPlane->RemoveMatter(pMatter);
	}       
	else
	{
		//û��piece��Ҳ�϶��ڹ�������
		pPlane->RemoveMatterFromMan(pMatter);
	}
	pMatter->ClrActive();

	unsigned int index = pMatter->spawn_index;
	ASSERT(index < _entry_list.size());
	if(index >= _entry_list.size())
	{
		//����־���������� 
		GLog::log(GLOG_ERR,"���տ���ʱ���ִ��������������");
		ASSERT(false);
		return false;
	}

	spin_autolock  keeper(_pool_lock);
	if(!_is_spawned || (_mine_total_gen_num && _mine_cur_gen_num >= _mine_total_gen_num))
	{
		_xid_list.erase(pMatter->ID);
		imp->_commander->Release();
		return false;
	}
	_mine_cur_gen_num ++;
	gmatter * pTmp = _mine_pool[index];
	pMatter->spawn_index = g_timer.get_tick() + _entry_list[index].reborn_time;
	if(pTmp)
	{
		pMatter->pNext= pTmp;
		pMatter->pPrev= pTmp->pPrev;
		pTmp->pPrev->pNext = pMatter;
		pTmp->pPrev = pMatter;
	}
	else
	{
		_mine_pool[index] = pMatter;
		pMatter->pPrev = pMatter->pNext = pMatter;
	}
	__PRINTF("�����˿���%d\n",pMatter->ID.id);
	return true;
}

void 
mine_spawner::GenerateMineParam(gmatter_mine_imp * imp, npc_template * pt)
{
	npc_template::__mine_info & mine = pt->mine_info;
	int index = abase::RandSelect(mine.id_produce_prop,mine.produce_kinds);
	int id = mine.id_produce[index];
	int life = mine.id_produce_life[index];
	int amount = abase::Rand(0.f,1.f) < mine.bonus_prop?mine.bonus_amount:mine.std_amount;
	imp->SetParam(id,amount,mine.time_min, mine.time_max,mine.need_equipment,mine.level,
			mine.exp,mine.sp,mine.gather_player_max,mine.gather_dist,life,mine.success_prob,
			(bool)mine.broadcast_on_gain,(char)mine.mine_type);
	imp->SetTaskParam(mine.task_in, mine.task_out, 
			mine.no_interrupted,mine.gather_no_disappear,mine.eliminate_tool,0,
			mine.ask_help_faction,mine.ask_help_range,mine.ask_help_aggro);
	imp->SetMonsterParam(mine.monster_list, 4);		
}

gmatter *
mine_spawner::CreateMine(mine_spawner * __this,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & et)
{
//	A3DVECTOR pos;
//	GeneratePos(pos,_offset_terrain);

	npc_template * pTemplate = npc_stubs_manager::Get(et.mid);
	if(pTemplate == NULL )
	{
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}
	if(pTemplate->mine_info.std_amount == 0 && pTemplate->mine_info.task_out == 0)
	{
		__PRINTINFO("���������д�\n");
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}

	//����NPC����npcȫ������
	gmatter * pMatter = pPlane->AllocMatter();
	if(!pMatter) return NULL;
	pMatter->ID.type = GM_TYPE_MATTER;
	pMatter->pos = pos;
	pMatter->ID.id= MERGE_ID<gmatter>(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetMatterIndex(pMatter)));

	gmatter_mine_imp *imp = new gmatter_mine_imp();
	imp->Init(pPlane,pMatter);
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);
	//�����Ȳ��ܰ��ɻ������reset����
	pMatter->imp = imp;
	pMatter->matter_type = et.mid;

	if(__this)
	{
		pMatter->SetDirUp(__this->_dir,__this->_dir1,__this->_rad);
	}
	else
	{
		pMatter->SetDirUp(0,0,abase::Rand(0,255));
	}

	GenerateMineParam(imp, pTemplate);

	pMatter->spawn_index = index;
	imp->_spawner = __this;

	pPlane->InsertMatter(pMatter);
	imp->_runner->enter_world();
	return pMatter;
}

gmatter *
mine_spawner::CreateMine2(mine_spawner * __this,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & et, unsigned char dir, unsigned char dir1, unsigned char rad)
{
//	A3DVECTOR pos;
//	GeneratePos(pos,_offset_terrain);

	npc_template * pTemplate = npc_stubs_manager::Get(et.mid);
	if(pTemplate == NULL )
	{
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}
	if(pTemplate->mine_info.std_amount == 0 && pTemplate->mine_info.task_out == 0)
	{
		__PRINTINFO("���������д�\n");
		GLog::log(GLOG_ERR,"Invalid mine template id %d",et.mid);
		return NULL;
	}

	//����NPC����npcȫ������
	gmatter * pMatter = pPlane->AllocMatter();
	if(!pMatter) return NULL;
	pMatter->ID.type = GM_TYPE_MATTER;
	pMatter->pos = pos;
	pMatter->ID.id= MERGE_ID<gmatter>(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetMatterIndex(pMatter)));

	gmatter_mine_imp *imp = new gmatter_mine_imp();
	imp->Init(pPlane,pMatter);
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);
	//�����Ȳ��ܰ��ɻ������reset����
	pMatter->imp = imp;
	pMatter->matter_type = et.mid;

	pMatter->SetDirUp(dir,dir1,rad);

	GenerateMineParam(imp, pTemplate);

	pMatter->spawn_index = index;
	imp->_spawner = __this;

	pPlane->InsertMatter(pMatter);
	imp->_runner->enter_world();
	return pMatter;
}

bool 
mine_spawner::CreateMines(world *pPlane) 
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		unsigned int count = _entry_list[i].mine_count;
		for(unsigned int j = 0; j < count; j ++)
		{
			A3DVECTOR pos;
			GeneratePos(pos,_offset_terrain,pPlane);
			gmatter * pMatter;
			if((pMatter = CreateMine(this,pos,pPlane,i,_entry_list[i])))
			{
				pMatter->collision_id = _collision_id;
				_xid_list[pMatter->ID] = 1;
				_mine_cur_gen_num ++;
				pMatter->Unlock();
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void 
mine_spawner::ClearObjects(world * pPlane)
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gmatter * pHeader = _mine_pool[i];
		if(pHeader == NULL) continue;

		//�������ڵȴ������IDɾ�������ҽ���������Ҳɾ��
		gmatter * tmp = pHeader;
		do
		{
			_xid_list.erase(tmp->ID);
			gmatter * pNext = (gmatter*)tmp->pNext;
			tmp->Lock();
			tmp->imp->_commander->Release();
			tmp->Unlock();
			tmp = pNext;
		}while(tmp != pHeader);
		_mine_pool[i] = NULL;
	}

	//��������δɾ���Ķ�������Ϣ ������ʧ
	if(_xid_list.size())
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_SPAWN_DISAPPEAR,XID(-1,-1),XID(-1,-1),A3DVECTOR(0,0,0),0);
		abase::vector<XID> list;
		abase::static_multimap<XID,int, abase::fast_alloc<> >::iterator it = _xid_list.begin();
		list.reserve(_xid_list.size());
		for(;it != _xid_list.end(); ++it)
		{
			list.push_back(it->first);
		}
		pPlane->SendMessage(list.begin(),list.end(), msg);
		_xid_list.clear();
	}
}

void 
group_spawner::OnHeartbeat(world * pPlane)
{
	if(_next_time <= 0) return; 
	mutex_spinlock(&_lock);
	if((_next_time-=20) > 0) 
	{
		mutex_spinunlock(&_lock);
		return;
	}
	mutex_spinunlock(&_lock);

	gnpc * pLeader;
	{
		//�����ӳ�
		ASSERT(_npc_pool[0]);
		pLeader = _npc_pool[0];
		if(!pLeader) 
		{
			//�����ǲ��Ǳ���һ������
			ASSERT(false);
			return;
		}
		ASSERT(pLeader->pNext == pLeader);
		_npc_pool[0] = NULL;

		//�����ӳ�
		_gen_pos_mode = false;
		Reborn(pPlane,pLeader,pLeader,_entry_list[0].offset_terrain,_entry_list[0].npc_tid);
		_gen_pos_mode = true;
		_leader_pos = pLeader->pos;
	}

	for(unsigned int i =1;i < _entry_list.size();i ++)
	{
		gnpc* header = _npc_pool[i];
		if(!header) continue;
		_npc_pool[i] = NULL;

		//��ʼ����
		Reborn(pPlane,header,header,_entry_list[i].offset_terrain,_entry_list[i].npc_tid);
	}
}

void 
group_spawner::GeneratePos(A3DVECTOR &pos,float offset_terrain, world * plane)
{
	if(_gen_pos_mode)
	{
		do
		{
			pos = _leader_pos;
			//���������λ�÷ֲ�
			int index = abase::Rand(0,65536) & 0xFF;
			pos.x += sctab[index][0]*7.0f;
			pos.z += sctab[index][1]*7.0f;

			//���¾����߶�ֵ
			pos.y = _pos_generator->GenerateY(pos.x,pos.y,pos.z,offset_terrain,plane);

			//����ɵ����ж�
		}while(0);

	}
	else
	{
		//��׼��ʽ
		base_spawner::GeneratePos(pos,offset_terrain,plane);
	}
}


gnpc*
group_spawner::CreateMasterMob(world * pPlane,int index, const entry_t &et)
{
	A3DVECTOR pos;
	base_spawner::GeneratePos(pos,et.offset_terrain,pPlane);
	_leader_pos = pos;
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = CreateMobBase(this,pPlane,et, index,pos,cid,GenDir(),CLS_NPC_AI_POLICY_MASTER,0,NULL,_mob_life);
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->OnNpcEnterWorld();
		pNPC->imp->_runner->enter_world();
		pNPC->Unlock();
	}
	return pNPC;
}

gnpc *
group_spawner::CreateMinorMob(world * pPlane,int index, int leader_id,const A3DVECTOR & leaderpos, const entry_t &et)
{
	A3DVECTOR pos;
//	GenerateMinorPos(pos,leaderpos,et.offset_terrain);
	GeneratePos(pos,et.offset_terrain,pPlane);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = CreateMobBase(this,pPlane,et, index,pos,cid,GenDir(),CLS_NPC_AI_POLICY_MINOR,0,NULL,_mob_life);
	if(pNPC)
	{
		((gnpc_imp*)pNPC->imp)->_leader_id = XID(GM_TYPE_NPC,leader_id);
		pPlane->InsertNPC(pNPC);
		pNPC->imp->OnNpcEnterWorld();
		pNPC->imp->_runner->enter_world();
		pNPC->Unlock();
	}
	return pNPC;
}

bool
group_spawner::CreateMobs(world * pPlane)
{
	if(_leader_id) return true;	//�����Ѿ����ɵ�Ⱥ��
	_next_time = g_timer.get_tick() + _reborn_time;
	//��ʼ���ɹ���
	gnpc * pLeader = NULL;
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		unsigned int count = _entry_list[i].mobs_count;
		if(i == 0) 
		{
			if(count > 1)
			{
				__PRINTINFO("���ֲ���ȷ��Ⱥ�֣��ӳ���Ŀ���࣬�Զ������Ϊ1\n");
				count = 1;
			}
			_gen_pos_mode = false;
			gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0]);
			if(pNPC == NULL) return false;
			_leader_id = pNPC->ID.id;
			pLeader = pNPC;
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
		}
		else
		{
			_gen_pos_mode = true;
			for(unsigned int j = 0; j < count; j ++)
			{
				gnpc * pNPC = CreateMinorMob(pPlane,i,_leader_id,pLeader->pos,_entry_list[i]);
				if(pNPC)
				{
					_xid_list[pNPC->ID] = 1;
					_mobs_cur_gen_num ++;
				}
			}
		}
	}
	_next_time = -1;
	return true;
}

bool
group_spawner::Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp, bool is_reset) 
{
	int id = pNPC->ID.id;
	if(mobs_spawner::Reclaim(pPlane,pNPC,imp,is_reset))
	{
		if(id == _leader_id)
		{
			//��_leader_id
			mutex_spinlock(&_lock);
			_next_time  = _reborn_time;
			mutex_spinunlock(&_lock);
		}
		return true;
	}
	else
	{
		//���ڲ������������Կ������ʵ���ʱ�����leader_id
		if(id == _leader_id) _leader_id = 0;
		return false;
	}
}

gnpc*
boss_spawner::CreateMasterMob(world * pPlane,int index, const entry_t &et)
{
	A3DVECTOR pos;
	base_spawner::GeneratePos(pos,et.offset_terrain,pPlane);
	_leader_pos = pos;
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = CreateMobBase(this,pPlane,et, index, pos,cid,GenDir(),CLS_NPC_AI_POLICY_BOSS,0,NULL,_mob_life);
	if(pNPC)
	{
		pPlane->InsertNPC(pNPC);
		pNPC->imp->OnNpcEnterWorld();
		pNPC->imp->_runner->enter_world();
		pNPC->Unlock();
	}
	return pNPC;
}

gnpc *
boss_spawner::CreateMinorMob(world * pPlane,int index, int leader_id,const A3DVECTOR & leaderpos, const entry_t &et)
{
	A3DVECTOR pos;
	GeneratePos(pos,et.offset_terrain,pPlane);
	const int cid[3] = {CLS_NPC_IMP,CLS_NPC_DISPATCHER,CLS_NPC_CONTROLLER};
	gnpc * pNPC = CreateMobBase(this,pPlane,et, index,pos,cid,GenDir(),CLS_NPC_AI_POLICY_MINOR,AGGRO_POLICY_BOSS_MINOR,NULL,_mob_life);
	if(pNPC)
	{
		((gnpc_imp*)pNPC->imp)->_leader_id = XID(GM_TYPE_NPC,leader_id);
		pPlane->InsertNPC(pNPC);
		pNPC->imp->OnNpcEnterWorld();
		pNPC->imp->_runner->enter_world();
		pNPC->Unlock();
	}
	return pNPC;
}

bool
boss_spawner::CreateMobs(world * pPlane)
{
	if(_leader_id) return true;
	_next_time = g_timer.get_tick() + _reborn_time;
	//��ʼ���ɹ���
	gnpc * pLeader = NULL;
	spin_autolock keep(_mobs_list_lock);
	_mobs_list.reserve(_entry_list.size());
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		unsigned int count = _entry_list[i].mobs_count;
		if(i == 0) 
		{
			if(count > 1)
			{
				__PRINTINFO("���ֲ���ȷ��Ⱥ�֣��ӳ���Ŀ���࣬�Զ������Ϊ1\n");
				count = 1;
			}
			_gen_pos_mode = false;
			gnpc *pNPC = CreateMasterMob(pPlane,i,_entry_list[0]);
			if(pNPC == NULL) return false;
			_leader_id = pNPC->ID.id;
			pLeader = pNPC;
			_mobs_list.push_back(pNPC->ID);
			_xid_list[pNPC->ID] = 1;
			_mobs_cur_gen_num ++;
		}
		else
		{
			_gen_pos_mode = true;
			for(unsigned int j = 0; j < count; j ++)
			{
				gnpc * pNPC = CreateMinorMob(pPlane,i,_leader_id,pLeader->pos,_entry_list[i]);
				ASSERT(pNPC);
				if(pNPC)
				{
					_mobs_list.push_back(pNPC->ID);
					_xid_list[pNPC->ID] = 1;
					_mobs_cur_gen_num ++;
				}
			}
		}
	}
	_next_time = -1;
	return true;
}

void 
boss_spawner::ForwardFirstAggro(world * pPlane, const XID & id, int rage)
{
	spin_autolock keep(_mobs_list_lock);
	if(_mobs_list.size() > 1)
	{
		MSG msg;
		BuildMessage(msg,GM_MSG_TRANSFER_AGGRO,XID(-1,-1),_mobs_list[0],A3DVECTOR(0,0,0),rage,&id,sizeof(id));
		pPlane->SendMessage(_mobs_list.begin() + 1, _mobs_list.end(), msg);

	}
}

gmatter *
dyn_object_spawner::CreateDynObject(const A3DVECTOR & pos,unsigned int index, world * pPlane,const entry_t & ent)
{
	//����NPC����npcȫ������
	gmatter * pMatter = pPlane->AllocMatter();
	if(!pMatter) return NULL;
	pMatter->ID.type = GM_TYPE_MATTER;
	pMatter->pos = pos; 
	pMatter->ID.id= MERGE_ID<gmatter>(MKOBJID(world_manager::GetWorldIndex(),pPlane->GetMatterIndex(pMatter)));
	pMatter->matter_type = ent.mid;

	gmatter_dyn_imp *imp = new gmatter_dyn_imp();
	imp->Init(pPlane,pMatter); 
	imp->_runner = new gmatter_dispatcher();
	imp->_runner->init(imp);
	imp->_commander = new gmatter_controller();
	imp->_commander->Init(imp);

	pMatter->imp = imp;
	pMatter->SetDirUp(_dir,_dir1,_rad);
	pMatter->SetMatterValue(_scale);

	pMatter->spawn_index = index;
	imp->_spawner = this;

	pPlane->InsertMatter(pMatter);
	imp->_runner->enter_world();
	return pMatter;
}


bool
dyn_object_spawner::CreateMines(world *pPlane)
{
	for(unsigned int i =0;i < _entry_list.size();i ++)
	{
		gmatter * pMatter;
		A3DVECTOR pos;
		GeneratePos(pos,0,pPlane);
		if((pMatter = CreateDynObject(pos,i, pPlane,_entry_list[i])))
		{
			pMatter->collision_id = _collision_id;
			_xid_list[pMatter->ID] = 1;
			_mine_cur_gen_num ++;
			pMatter->Unlock();
		}
		else
		{
			return false;
		}
	}
	return true;
}

void 
spawner_ctrl::Spawn(world * pPlane)
{
	if(IsSpawned()) return;
	for(unsigned int i = 0; i < _list.size();i ++)
	{
		if(!_list[i]->BeginSpawn(pPlane)) break;
	}
	SetSpawnFlag(true);
	return ;
}


void 
spawner_ctrl::Active(world * pPlane)
{
	if(IsActived()) return ;
	_active_flag = true;
	_cur_active_life = _active_life;
	_date_counter_down = 0;//���������¼��㼤��ʱ��

	if(_spawn_after_active <= 0) 
	{
		Spawn(pPlane);
	}
	else
	{	
		_time_before_spawn = _spawn_after_active;
	}
}


void 
spawner_ctrl::Stop(world * pPlane)
{
	if(!IsActived()) return ;
	_active_flag = false;
	_date_counter_down = 0;//���������¼��㼤��ʱ��
	
	if(IsSpawned())
	{
		for(unsigned int i = 0; i < _list.size();i ++)
		{
			_list[i]->EndSpawn(pPlane);
		}
		SetSpawnFlag(false);
	}
}

void
spawner_ctrl::OnHeartbeat(world * pPlane)
{
	spin_autolock keeper(_lock);
	time_t t = (time_t)g_timer.get_systime();
	if(_active_flag)
	{
		//��δ����
		if(!_spawn_flag)
		{
			//δ����
			_time_before_spawn --;
			if(_time_before_spawn <= 0)
			{
				//����....
				Spawn(pPlane);
				return ;
			}
		}

		if(_active_life > 0)
		{
			_cur_active_life --;
			if(_cur_active_life <= 0)
			{
				//����������
				//ȡ������
				Stop(pPlane);
				return ;
			}
		}

		if(_has_stop_date)
		{
			_date_counter_down --;
			if(_date_counter_down <= 0)
			{
				struct tm tt;
				localtime_r(&t,&tt);
				//�ٴμ��ʱ��
				int rst = _stop_date.CheckTime(tt);
				if(rst > 0)
				{
					if(rst > 600) rst = 600;
					_date_counter_down = rst;
				}
				else
				{
					//�ﵽֹͣ��ʱ���
					//ֹͣ����
					Stop(pPlane);
					return ;
				}
			}
		}
	}
	else
	{
		if(_has_active_date)
		{
			_date_counter_down --;
			if(_date_counter_down <= 0)
			{
				int rst;
				if(_active_date_duration <= 60)
				{
					struct tm tt;
					localtime_r(&t,&tt);
					//�ٴμ��ʱ��
					rst = _active_date.CheckTime(tt);
					if(rst > 0)
					{
						if(rst > 600) rst = 600;
						_date_counter_down = rst;
					}
					else
					{
						//���￪ʼ��ʱ���
						//��ʼ����
						Active(pPlane);
					}
					return ;
				}

				//ʹ�õڶ�����ʼ�����
				rst = _active_date.CheckTime2(t,_active_date_duration);
				if(rst >= 0 && rst <= _active_date_duration)
				{
					Active(pPlane);
					return ;
				}
				else
				{
					if(rst > _active_date_duration)
					{
						_date_counter_down = rst - _active_date_duration;
					}
				}
			}
		}
	}
}

int 
crontab_t::CheckTime(const tm &tt)
{
	int offset = 0;
	if(year >= 0 && year != (tt.tm_year + 1900)) 
	{
		//��ݲ�����
		if(tt.tm_year + 1901 != year) return 3600;
		if(tt.tm_mon != 11) return 3600;
		if(tt.tm_mday != 31) return 3600;
		return (23 - tt.tm_hour)*3600 + (59-tt.tm_min)+(60-tt.tm_sec);

	}
	if(month >= 0 && month != tt.tm_mon) 
	{
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//�����������ĩ��23��50���Ժ���ȴ���0���ʱ��
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);
	}


	ASSERT(day_of_months < 0 || day_of_week < 0);
	if(day_of_months >= 0 && day_of_months != tt.tm_mday)
	{
		offset = (day_of_months - tt.tm_mday) * 24 * 3600;
	}
	if(day_of_week >= 0 && day_of_week != tt.tm_wday)
	{
		offset = (day_of_week - tt.tm_wday) * 24 * 3600;
	}

	if(hour >=0 && hour != tt.tm_hour)
	{
		offset += (hour - tt.tm_hour)*3600;
	}

	ASSERT(min >= 0);
	if(min >=0 && min != tt.tm_min)
	{
		offset += (min - tt.tm_min)*60;
	}

	offset -= tt.tm_sec;

	if(offset <= -60)
	{
		//ʱ�䳬����һ���ӣ����Ǽ�����һ�ο����¼�
		if(hour < 0)
		{
			for(int h = tt.tm_hour; h < 23 && offset < 0; h++)
			{
				offset += 3600;
			}
			if(offset > 0)
				return offset;
			else
			{
				//��Сʱ�����޷���ɣ�����Ҫ�����������ֵ
				//���Խ�Сʱ��0
				offset -= 3600*23;
			}
		}

		if(day_of_months < 0 || day_of_week < 0)
		{
			if(day_of_week >= 0)
			{
				//day of_month �� *
				//�����ܵ��� 
				offset %= 3600*24*7;
				offset += 3600*24*7;
				if(offset > 600) offset = 600;
				return offset;

			}
			else if(day_of_months >= 0)
			{
				//���ղ����ܣ���Ҫ�����·ݵ��� 
				//ֱ������10����
				if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

				//�����������ĩ��23��50���Ժ���ȴ���0���ʱ��
				return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

			}
			else
			{
				//����һ�죬��Ҫ�����յ���
				offset %= 3600*24;
				offset += 3600*24;
				if(offset > 600) offset = 600;
				return offset;
			}
		}

		//���ն��ܵ� �����·ݵ���
		if(tt.tm_mday < 28 || tt.tm_hour < 23 || tt.tm_min < 50) return 600;

		//�����������ĩ��23��50���Ժ���ȴ���0���ʱ��
		return (59 - tt.tm_min)*60 +(60 - tt.tm_sec);

	}

	return offset;
}

static inline bool IsLeapYear(int year)
{
	if((year % 4) != 0)  return false;
	if((year % 400) == 0) return true;
	if((year % 100) == 0) return false;
	return true;
}

static inline int GetMDay(int year, int mon)
{
	static int mday[] = { 31,28,31,30,31,30,31,31,30,31,30,31};
	int d = mday[mon];
	if(mon == 1 && IsLeapYear(year)) d = d+1;
	return d;
}

int 
crontab_t::CheckTime2(time_t t1,int DUR)
{
	time_t t2 = t1 - DUR;
	struct tm tt;
	localtime_r(&t2,&tt);

	int offset = 0;
	if(year >= 0 && year != (tt.tm_year + 1900)) 
	{
		//��ݲ�����
		if(tt.tm_year+1901 != year) return DUR+3600;	//�������ȥ����һ��Сʱ����˵
		if(tt.tm_mon != 11) return DUR + 3600;		//�������12����һ��Сʱ����˵

		//Ŀ���·ݱ�����һ�·ݻ�����Ҫ��
		if(month > 0) return DUR + 3600;

		//���㵽�����ʱ�� 
		offset = (31 - tt.tm_mday)* 24 * 3600 + (23 - tt.tm_hour)*3600 + (59-tt.tm_min)*60 +(60-tt.tm_sec);
		//��������
		tt.tm_year = year;		//����
		tt.tm_mon = 0;			//1��
		tt.tm_yday = 0;
		tt.tm_mday = 1;			//1�� 
		tt.tm_wday = (tt.tm_wday + (31 - tt.tm_mday + 1)) % 7;	//������
		tt.tm_hour = 0;
		tt.tm_min = 0;
		tt.tm_sec = 0;	//00:00:00
	}

	if(month >= 0 && month != tt.tm_mon) 
	{
		if((month != tt.tm_mon + 1) && (month != 0 || tt.tm_mon != 11 || year >= 0))
		{
			//�·ݳ���������������һ���£����ؼ���
			return DUR + 3600;
		}

		//�����·����ʱ�� ���ﲻ����ֿ�Խ��ݵ����
		int d = GetMDay(tt.tm_year + 1900, tt.tm_mon);
		int d_adjust = d + 1 - tt.tm_mday;
		offset += d_adjust*24*3600;

		//������������һ���µ�1��
		if(tt.tm_mon == 11)
		{
			tt.tm_year ++;
			tt.tm_mon = 0;
		}
		else
		{
			tt.tm_mon ++;
		}
		tt.tm_yday += d_adjust;
		tt.tm_mday = 1;
		tt.tm_wday = (tt.tm_wday + d_adjust) % 7;	//������
	}

	int y_days = IsLeapYear(tt.tm_year + 1900)?366:365;
	int m_days = GetMDay(tt.tm_year + 1900, tt.tm_mon);

	ASSERT(day_of_months < 0 || day_of_week < 0);
	if(day_of_months >= 0 && day_of_months != tt.tm_mday)
	{
		//ָ������ ��������
		offset += (day_of_months - tt.tm_mday) * 24 * 3600;
		if(offset < 0)
		{
			if(month < 0)
			{
				//ʱ�䲻ƥ�䣬����һ���µ�ʱ��
				if(tt.tm_mon == 11 && year >= 0)
				{
					//�޷�����
					return DUR + 3600;
				}
				offset += m_days*24*3600;
				//��ʹ��12�£��������ڲ���Ҫ�ٴε����ˣ����Բ���tt�����޸���
			}
			else if(year < 0)
			{
				//����һ���ʱ��
				offset += y_days*24*3600;
			}
		}
	}

	if(day_of_week >= 0 && day_of_week != tt.tm_wday)
	{
		//ָ���ܵ�����(�ж��ʱ���)
		int d = day_of_week - tt.tm_wday;
		if(d <0) d = 7 +d;
		if(offset == 0)
		{
			if((tt.tm_mday + d > m_days) && (month >= 0 || tt.tm_mon == 11))
			{
				//�������·ݻ������Ҫ��
				return DUR + 3600;
			}

			offset += d * 24 * 3600;
		}
		else
		{
			//�������·ݺ���ݵĵ�����Ŀǰ�϶���һ��
			offset += d*24*3600;
		}
	}

	if(hour >=0 && hour != tt.tm_hour)
	{
		//Сʱ�ĵ���
		offset += (hour - tt.tm_hour)*3600;
	}

	ASSERT(min >= 0);
	if(min >=0 && min != tt.tm_min)
	{
		offset += (min - tt.tm_min)*60;
	}

	offset -= tt.tm_sec;

	if(offset > 0)
	{
		if(day_of_months < 0 && day_of_week < 0)
		{
			while(offset > 24*3600)
			{
				offset -= 24*3600;
			}
		} else if(day_of_week >= 0 && day_of_months < 0)
		{
			while(offset > 7 *24 *3600 && tt.tm_mday > 7)
			{
				offset -= 7*24*3600;
				tt.tm_mday -= 7;
			}
		}

		if(hour < 0)
		{
			while(offset > 3600 && tt.tm_hour > 0)
			{
				offset -= 3600;
				tt.tm_hour --;
			}
		}

		return offset;
	}
	if(offset < 0)
	{
		//�ܹ���˵ģ��϶���������ܶ�û�н��е���
		if(hour < 0)
		{
			for(int h = tt.tm_hour; h < 23 && offset < 0; h++)
			{
				offset += 3600;
			}       
			if(offset > 0)
				return offset;
			else    
			{
				//��Сʱ�����޷���ɣ�����Ҫ�����������ֵ
				//���Խ�Сʱ��0
				offset -= 3600*23;
			}
		}

		if(day_of_months < 0 && day_of_week < 0)
		{
			//���������ĵ���
			int d = tt.tm_mday;
			while(offset < 0 && d < m_days)
			{
				offset += 24*3600;
				d ++;
			}
			if(offset < 0) //�������С��0
			{
				//���㵽����ʱoffset�϶�����(-24*3600,0),����tt.tm_mday==m_days,
				if(month < 0 && !(tt.tm_mon == 11 && year >= 0))
					return offset + 24*3600;
			}
			else
				return offset;
		}
		else if(day_of_week > 0)
		{
			//��������Ϊ��λ�ĵ���
			if((tt.tm_mday + 7 > m_days) && (month >= 0 || tt.tm_mon == 11))
			{
				//�������·ݻ������Ҫ��
				return DUR + 3600;
			}

			offset += 7 * 24 * 3600;
			return offset;
		}
		return DUR + 3600;
	}
	return offset;
}
