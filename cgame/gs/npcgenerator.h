#ifndef __NETGAME_GS_NPC_GENERATOR_H__
#define __NETGAME_GS_NPC_GENERATOR_H__

#include <hashtab.h>
#include <timer.h>
#include <threadpool.h>
#include <arandomgen.h>
#include <common/types.h>
#include <glog.h>

#include "staticmap.h"
#include "property.h"

class gplayer_imp;
struct pet_data;
struct npc_template
{
	int tid;
	int addon_choice_count;
	unsigned int	id_addon[16];
	float	probability_addon[16];

	struct  __mine_info
	{
		int is_mine;			//������ֵ��Ϊ�㣬��ʾ����һ��matter
		int std_amount;			//��׼���� 
		int bonus_amount;		//���Ӻ������
		float bonus_prop;		//ʹ�ø��������ĸ���
		int time_min;			//�ɼ�ʱ������
		int time_max;			//�ɼ�ʱ������
		unsigned int produce_kinds;
		unsigned int id_produce[16];
		float id_produce_prop[16];
		int id_produce_life[16];
		int need_equipment;
		int level;
		int exp;
		int sp;
		int task_in;
		int task_out;
		bool no_interrupted;
		bool gather_no_disappear;
		bool eliminate_tool;
		int   ask_help_faction;
		float ask_help_range;
		int   ask_help_aggro;
		int set_owner;
		float gather_dist;
		int life;
		int gather_player_max;
		int mine_type;
		float success_prob;
		int broadcast_on_gain;	//�ɼ��ɹ��Ļ��㲥����Χ
		struct 
		{
			int id_monster;
			int num;
			float radius;
			int remain_time;
		} monster_list[4];
	} mine_info;

	unsigned int faction;	
	unsigned int carrier_id;		
	unsigned int enemy_faction;		
	unsigned int monster_faction;		//�����С��ϵ���������
	unsigned int id_strategy;
	unsigned int inhabit_type;		
	unsigned int inhabit_mode;		//�������pathfinding��ģʽ	
	int role_in_war;			//��ս�еĽ�ɫ
	int immune_type;
	float body_size;
	float sight_range;
	unsigned int 	aggressive_mode;
	unsigned int	monster_faction_ask_help;
	unsigned int	monster_faction_can_help;
	int petegg_id;
	int short_range_mode;	//�Ƿ�Զ�̹���
	float aggro_range;
	int aggro_time;
	int damage_delay;
	int trigger_policy;
	struct condition_skill
	{
		int skill;
		int level;
		float prob;
	};
	condition_skill skill_hp75[5];
	condition_skill skill_hp50[5];
	condition_skill skill_hp25[5];
	unsigned int after_death;
	int drop_no_protected;	//������Ʒ������
	int drop_no_profit_limit;//���䲻������ʱ��Ӱ��
	float drop_mine_prob;	//�������ĸ���
	int drop_mine_group;	//�������ķ���
	int patrol_mode;
	int normal_heartbeat_in_idle;		//����Χ������Ҳ�������ٶȵ�����
	float max_move_range;	//����ƶ���Χ

	int after_death_summon_monster_id;
	int after_death_summon_monster_radius;
	int after_death_summon_monster_num;

	int max_received_damage_per_hit;
	
	struct
	{
		int as_count;
		int bs_count;
		int cs_count;
		struct 
		{
			int id;
			int level;
		} attack_skills[8];
		
		struct 
		{
			int id;
			int level;
		} bless_skills[8];

		struct
		{
			int id;
			int level;
		} curse_skills[8];

	}skills;

	int 		aggro_strategy_count;
	unsigned int	aggro_strategy_ids[4];
	float		aggro_strategy_probs[4];	

	basic_prop bp;
	extend_prop ep;
	int		hp_adjust_common_value;			//	Ѫ������ȫ�ֱ���ID
	int		defence_adjust_common_value;	//	��������ȫ�ֱ���ID
	int		attack_adjust_common_value;		//	��������ȫ�ֱ���ID
	int		highest_level;					
	int		attack_degree;
	int		defend_degree;
	int		invisible_degree;
	int		anti_invisible_degree;
	int 		no_accept_player_buff;
	int 	no_auto_fight;		//���ɽ���ս��״̬
	int 	fixed_direction;	//���򲻱�
	int 	faction_building_id;//���ɻ��ؽ���id
	int		has_collision;		//�Ƿ�������ײ���
	int		set_owner;			//�Ƿ��ڳ���ʱ����������
	bool	record_dps_rank;	//�����ɱ�����Ƿ����dps���а�,set_owner��0ʱ����Ч	
	unsigned int domain_related; // �������
	int		local_var[3];		//npcӵ�е�����ֵ 	

	//�����ǹ���npc������
	struct npc_statement
	{
		enum 
		{
			NPC_TYPE_NORMAL,
			NPC_TYPE_GUARD,
		};

		int is_npc;			//��ֵ����,��ʾ��һ��NPC
		int src_monster;
		int refresh_time;
		int attack_rule;
		int faction;		//���ܲ���Ҫ 
		float tax_rate;
		int life;		//���ܲ���Ҫ
		int npc_type;		//npc ����
		int need_domain;	//��Ҫ���ɵ��̲ſ��Խ��д���
		bool serve_distance_unlimited;	//�������������

		//���ַ���Ĵ���
		
		int  service_sell_num;
		int  service_sell_goods[1536];

		int  service_transmit_target_num;
		struct  __st_ent
		{
			int 		world_tag;
		        A3DVECTOR      	target;
			unsigned int          fee;
			int 		require_level;
			int		target_waypoint;
		}transmit_entry[32];		//������������serviceprovider�����һ��

		int service_task_in_num;
		int service_task_in_list[256];

		int service_task_out_num;
		int service_task_out_list[258];

		int service_task_matter_num;
		int service_task_matter_list[32];

		int service_teach_skill_num;
		int service_teach_skill_list[256];

		struct __service_produce
		{
			int type;	//0 ���� 1 �ϳ� 2 �������� 3 �¼̳�����
			int produce_skill;
			int produce_num;
			int produce_items[256];
		}service_produce;

		// Fulano
		struct __lib_service_produce
		{
			int produce_num;
			int produce_items[16];
		}lib_service_produce;
		
		int service_decompose_skill;

		bool service_identify;
		int  service_identify_fee;

		int service_vehicle_count;
		struct vehicle_path_entry 
		{
			int line_no;
			int target_waypoint;		//Ŀ��·���id�Ƕ���
			A3DVECTOR dest_pos;
			float speed;
			int vehicle;
			int min_time;	//in sec
			int max_time;	//in sec
			unsigned int fee;
		} vehicle_path_list[16]; 

		int service_waypoint_id;	//����� <=0��Ϊ��Ч

		int service_refine_transmit;	//����ת�Ʒ���

		int npc_tower_build_size;	//�������������
		struct __npc_tower_build
		{
			int id_in_build;	
			int id_buildup;	
			int id_object_need;	
			int time_use;	
			int fee;		
		}npc_tower_build[4];

		int service_reset_prop_count;	//����ϴ�����Ŀ��Ŀ
		struct __reset_prop
		{
			int object_need;
			int str_delta;	
			int agi_delta;	
			int vit_delta;	
			int eng_delta;	
		}reset_prop[15];
		
		int service_change_pet_name;
		struct 
		{
			int money_need;
			int item_need;
		}change_pet_name_prop;

		int service_forget_pet_skill;
		struct 
		{
			int money_need;
			int item_need;
		}forget_pet_skill_prop;

		int service_pet_skill_num;
		int service_pet_skill_list[128];

		int service_equip_bind;
		struct 
		{
			int money_need;
			int can_webtrade;
			int item_need[4];
		} service_bind_prop;

		int service_destroy_bind;
		struct 
		{
			int money_need;
			int item_need;
		} service_destroy_bind_prop;

		int service_undestroy_bind;
		struct 
		{
			int money_need;
			int item_need;
		} service_undestroy_bind_prop;

		struct  __pw_ent
		{
			A3DVECTOR       target_pos;
			int             target_tag;
			int             require_level;
			unsigned int          fee;
		};

		int service_elf_learn_skill_num;//�Ƿ���С����ѧϰ���ܷ���
		int service_elf_learn_skill_list[128];//����С����ļ���id
		int service_playerforce_tid;	//�����������,��idΪ������ģ��id
		
		struct
		{
			int service_make_slot:1;
			int service_purchase:1;
			int service_repair:1;
			int service_heal:1;
			int service_install:1;
			int service_uninstall:1;
			int service_storage:1;
			int service_unlearn_skill:1;

			int service_faction:1;		//�Ƿ�ɴ������ɷ���
			int service_face_ticket:1;	//�Ƿ���Խ�������
			int service_mail:1;		//�Ƿ����ʼ�����
			int service_auction:1;		//�Ƿ�����������
			int service_double_exp:1;		//�Ƿ���˫���������
			int service_hatch_pet:1;		//��������
			int service_recover_pet:1;	//��ԭ���ﵰ
			int service_war_management:1;	//��ս����

			int service_war_leave_battle:1;	//�뿪ս������
			int service_cash_trade:1;		//�㿨����
			int service_refine:1;		//��������
			int service_dye:1;		//Ⱦɫ���� 
			int service_elf_dec_attributie:1;	//�Ƿ���С����ϴ���Ե����
			int service_elf_flush_genius:1;		//�Ƿ���С����ϴ�츳�����
			int service_elf_forget_skill:1;		//�Ƿ���С�����������ܷ���
			int service_elf_refine:1;			//�Ƿ���С���龫������
			
			int service_elf_refine_transmit:1;	//�Ƿ���С���龫��ת�Ʒ���
			int service_elf_decompose:1;		//�Ƿ���С�����ַ���
			int service_elf_destroy_item:1;		//�Ƿ���С�����жװ������
			int service_dye_suit:1;				//����Ⱦɫ����	
			int service_repair_damaged_item:1;	//����pk�����������Ʒ����
			int service_user_trashbox:1;		//�ʺŲֿ����
			int service_webtrade:1;				//����ƽ̨����
			int service_god_evil_convert:1;		//��ħת������
			
			int service_wedding_book:1;			//����Ԥ������
			int service_wedding_invite:1;		//�����������
			int service_factionfortress:1;		//���ɻ��ط���
			int service_factionfortress2:1;		//���ɻ��ط���2
			int service_factionfortress_material_exchange:1;	//���ɻ��ز��϶һ�
			int service_dye_pet:1;				//���Ⱦɫ����
			int service_trashbox_view:1;		//�ֿ�鿴����
			int service_dpsrank:1;				//DPS���а����
			
			int service_country_management:1;	//�����뿪���ҵ�
			int service_countrybattle_leave:1;	//�뿪��սս��
			int service_equip_sign:1;			//װ��ǩ��
			int service_change_ds_forward:1;	//���:ԭ��->���
			int service_change_ds_backward:1;	//���:���->ԭ��
			int service_player_rename:1;		//��Ҹ��Ѹ���
			int service_addon_change:1;     	// ��ʯת��
			int service_addon_replace:1;    	// ��ʯ�滻
			
			int service_kingelection:1;     	//����ѡ��
			int service_decompose_fashion_item:1;//ʱװ���
			int service_player_shop:1;      	//��Ҽ���ϵͳ
			int service_reincarnation:1;		//���ת��
			int service_giftcardredeem:1;		//��Ʒ����
			int service_trickbattle_apply:1;	//ս������
			int service_generalcard_rebirth:1;	//��������
			int service_improve_flysword:1;		//�ɽ�����
			int service_mafia_pvp_signup:1;		//����pvp��ս
			int service_gold_shop:1;			//npcԪ���̳�
			int service_dividend_shop:1;		//npc�����̳�
            int service_player_change_gender:1; //npc��ɫ����
            int service_make_slot_for_decoration:1;     // ��Ʒ���
			int service_select_solo_tower_challenge_stage:1;//���˸���ѡ��ؿ�
            int service_solo_challenge_rank:1;          // ���˸������а�
			int service_mnfaction_sign_up:1;
			int service_mnfaction_award:1;
			int service_mnfaction_rank:1;
			int service_mnfaction_battle_transmit:1;
			int service_mnfaction_join_leave:1;
            int service_solo_challenge_rank_global:1;

			//Arena of Aurora
			int service_arena_create_team:1;
			int service_arena_join_team:1;
			int service_arena_characters_view:1;
			int service_arena_ranking_view:1;
			int service_arena_ranking_view_global:1;

			int service_bible_refine:1;
			int service_bible_refine_transmit:1;
			int service_material_refine:1;

			int storage_999_recv:1;
			/*170+ Bebê Celestial*/
			int service_kid_create:1;
			/*170+ Códice*/
			int service_dye_codex:1;
			int service_rename_codex:1;
        };

		struct __service_engrave
		{
			int engrave_num;
			int engrave_entrys[16];
		}service_engrave;
		
		struct __service_addonregen
		{
			int addonregen_num;
			int addonregen_entrys[16];
		}service_addonregen;

		struct __service_cross
		{
			int	activity_type;				//	�����(type=cross_server_activity)
			int	player_count_limit;			//	��������
			int	time_out;					//	�ʱ��_��
			int	need_item_tid;				//	������Ʒid(type=all_type)
			int	need_item_count;			//	������Ʒ����
			bool cost_item;					//	�Ƿ���������Ʒ(type=bool)
			int	history_max_level;			//	��ʷ��ߵȼ�����
			int	second_level;				//	����ȼ�Ҫ��(type=taoist_rank_require)
			int	realm_level;				//	����ȼ�Ҫ��
		}service_cross;

		struct __id_pridictable_engrave_service
		{
			int engrave_num;
			int engrave_entrys[640];
		}pridictable_engrave_service;

	}npc_data;
};

struct recipe_template
{
	int recipe_id;
	int produce_skill;
	int require_level;
	int recipe_level;
	int level;
	int exp;
	int sp;
	float null_prob;
	unsigned int use_time;
	unsigned int count;
	unsigned int fee;
	int material_num;
	int material_total_count;
	int bind_type;		//���˺�һ״̬ 0 ��ǰ 1 �϶����� 2 �������
	int proc_type;		//������Ʒ�Ĵ�����ʽ,��bind_typeΪ0ʱ��Ч�����������Ʒ������ʽΪess.proc_type|proc_type
	int equipment_need_upgrade;	//��������Ϊ��������ʱ��Ч����ʾ��������װ��
	float inherit_fee_factor;		//��������Ϊ��������ʱ��Ч����ʾ�̳д�����װ������Ҫ�ķ���ϵ��
	float inherit_engrave_fee_factor;//��������Ϊ��������ʱ��Ч����ʾ�̳д�����װ�����Կ���������Ҫ�ķ���ϵ��
    float inherit_addon_fee_factor;	//��������Ϊ����5������������������������Ҫ�ķ���ϵ��

	//Novos
	int require_arena_personal_point;
	int is_recipe_card; // true and false
	int unk;

	struct __material
	{
		int item_id;
		unsigned int count;
	} material_list[32];
	struct __target
	{
		int id;
		float  prob;
	}targets[4];
};

struct decompose_recipe_template
{
	//�����Ƿֽ���Ҫ������
	int    id;
	int produce_skill;
	int require_level;
	int recipe_level;
	unsigned int decompose_fee;
	unsigned int decompose_time;
	unsigned int element_num;
	int element_id;
};

struct engrave_recipe_template
{
	int recipe_id;
	int engrave_level;		//�Կ̵ȼ���Ŀǰ����
	int equip_mask;			//װ��λ������
	int equip_level_min;	//Ҫ��װ��Ʒ������
	int equip_level_max;	//Ҫ��װ��Ʒ������
	unsigned int use_time;		//����ռ��ʱ��
	float prob_addon_num[4];//�Կ̲���0-3�����Եĸ���
	int material_total_count;
	int material_num;
	struct __material
	{
		int item_id;
		unsigned int count;
	} material_list[8];
	struct __target_addon
	{
		int addon_id;
		float  prob;
	} target_addons[32];
};

struct addonregen_recipe_template
{
	int recipe_id;
	int produce_skill;		//��Ҫ����������id
	int require_level;		//��Ҫ���������ܵȼ�
	unsigned int use_time;		//����ռ��ʱ��
	unsigned int fee;				//���ѽ��
	int equip_id_list[32];	//�䷽���õ�װ��id�б�
	int material_total_count;
	int material_num;
	struct __material
	{
		int item_id;
		unsigned int count;
	} material_list[8];
};

struct addonchange_recipe_template
{
	int recipe_id;
	int	id_old_stone;				// ת��ǰ��ʯ
	int	id_new_stone;				// ת����ʯ
	
	unsigned int	fee;						// �����Ǯ

	int material_num;
	struct
	{
		int	item_id;				// ת����Ҫԭ��id
		unsigned int	count;					// ת����Ҫԭ������
	}material_list[8];

};

// Lib Produce  

struct lib_recipe_template
{
	int recipe_id;
	int material_num;
	int material_total_points;

	struct __material
	{
		int item_id;
		unsigned int point;

	}material_list[256];

	int	consume_point;
	unsigned int	fee;
	unsigned int	use_time;

	struct __target
	{
		int id;
		int count;
		float  prob;
	}targets[64];
};

struct easy_recipe_template
{
	int recipe_id;
	int material_total_count;
	int material_num;
	int enable_ui;
	unsigned int use_count;
	unsigned int use_time;
	int unk1;

	struct __material
	{
		int item_id;
		int count;

	} material_list[10];	

	struct __target
	{
		int id;
		int count;
		float  prob;
	}targets[6];
};


class itemdataman;
class npc_stubs_manager
{
	typedef abase::hashtab<npc_template, int ,abase::_hash_function> MAP;
	MAP _nt_map;
	bool __InsertTemplate(const npc_template & nt)
	{
		return _nt_map.put(nt.tid, nt);
	}

	npc_template * __GetTemplate(int tid)
	{
		return _nt_map.nGet(tid);
	}
	static npc_stubs_manager & __GetInstance()
	{
		static npc_stubs_manager __Singleton;
		return __Singleton;
	}
	npc_stubs_manager():_nt_map(1024) {}
public:
	static bool Insert(const npc_template & nt)
	{
		bool rst = __GetInstance().__InsertTemplate(nt);
		ASSERT(rst);
		return rst;
	}

	static npc_template * Get(int tid)
	{
		return __GetInstance().__GetTemplate(tid);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

class recipe_manager
{
	typedef abase::hashtab<recipe_template, int ,abase::_hash_function> MAP;
	typedef abase::hashtab<decompose_recipe_template, int ,abase::_hash_function> MAP2;
	typedef abase::hashtab<engrave_recipe_template, int ,abase::_hash_function> ENGRAVE_MAP;
	typedef abase::hashtab<addonregen_recipe_template, int ,abase::_hash_function> ADDONREGEN_MAP;
	typedef abase::hashtab<addonchange_recipe_template, int ,abase::_hash_function> ADDONCHANGE_MAP;
	typedef abase::hashtab<lib_recipe_template, int ,abase::_hash_function> LIB_MAP;
	typedef abase::hashtab<easy_recipe_template, int ,abase::_hash_function> EASY_MAP;

	MAP _rt_map;
	MAP2 _drt_map;
	ENGRAVE_MAP _ert_map;
	ADDONREGEN_MAP _arrt_map;
	ADDONCHANGE_MAP _acrt_map;
	LIB_MAP _lib_map;
	EASY_MAP _easy_map;

	bool __InsertTemplate(const recipe_template & rt)
	{
		recipe_template tmp= rt;
		tmp.material_total_count = 0;
		for(int i = 0; i < rt.material_num; i ++)
		{
			tmp.material_total_count += rt.material_list[i].count;
		}
	//	if(tmp.material_total_count > 0) 
			return _rt_map.put(rt.recipe_id, tmp);
	//	else 
	//		return false;
	}

	bool __InsertTemplate(const decompose_recipe_template & rt)
	{
		decompose_recipe_template tmp= rt;
		return _drt_map.put(rt.id, tmp);
	}

	bool __InsertTemplate(const engrave_recipe_template & ert)
	{
		return _ert_map.put(ert.recipe_id, ert);
	}

	bool __InsertTemplate(const addonregen_recipe_template & arrt)
	{
		return _arrt_map.put(arrt.recipe_id, arrt);
	}

	bool __InsertTemplate(const addonchange_recipe_template & acrt)
	{
		return _acrt_map.put(acrt.recipe_id, acrt);
	}

	bool __InsertTemplate(const lib_recipe_template & lib)
	{
		lib_recipe_template tmp= lib;
		tmp.material_total_points = 0;
		for(int i = 0; i < lib.material_num; i ++)
		{
			tmp.material_total_points += lib.material_list[i].point;
		}
		return _lib_map.put(lib.recipe_id, tmp);
	}

	bool __InsertTemplate(const easy_recipe_template & easy)
	{
		easy_recipe_template tmp= easy;
		tmp.material_total_count = 0;
		for(int i = 0; i < easy.material_num; i ++)
		{
			tmp.material_total_count += easy.material_list[i].count;
		}

		return _easy_map.put(easy.recipe_id, tmp);

	}


	recipe_template * __GetTemplate(int id)
	{
		return _rt_map.nGet(id);
	}

	decompose_recipe_template * __GetDecomposeTemplate(int id)
	{
		return _drt_map.nGet(id);
	}

	engrave_recipe_template * __GetEngraveTemplate(int id)
	{
		return _ert_map.nGet(id);
	}

	addonregen_recipe_template * __GetAddonRegenTemplate(int id)
	{
		return _arrt_map.nGet(id);
	}

	addonchange_recipe_template * __GetAddonChangeTemplate(int id)
	{
		return _acrt_map.nGet(id);
	}

	lib_recipe_template * __GetLibTemplate(int id)
	{
		return _lib_map.nGet(id);
	}

	easy_recipe_template * __GetEasyTemplate(int id)
	{
		return _easy_map.nGet(id);
	}

	static recipe_manager & __GetInstance()
	{
		static recipe_manager __Singleton;
		return __Singleton;
	}

	recipe_manager():_rt_map(1024) ,_drt_map(1024), _ert_map(1024), _arrt_map(1024), _acrt_map(1024), _lib_map(1024), _easy_map(1024) {}

	recipe_template * __GetByItemID(int id)
	{
		if(id <=0) return NULL;
		MAP::iterator it = _rt_map.begin();
		for(;it != _rt_map.end(); ++it)
		{
			recipe_template & pTmp = *it.value();
			if(pTmp.targets[0].id == id ||
					pTmp.targets[1].id == id ||
					pTmp.targets[2].id == id ||
					pTmp.targets[3].id == id)
					{
						return &pTmp;
					}
		}
		return NULL;
	}


public:
	static bool Insert(const recipe_template & rt)
	{
		return __GetInstance().__InsertTemplate(rt);
	}

	static bool Insert(const decompose_recipe_template & drt)
	{
		return __GetInstance().__InsertTemplate(drt);
	}

	static bool Insert(const engrave_recipe_template & ert)
	{
		return __GetInstance().__InsertTemplate(ert);
	}

	static bool Insert(const addonregen_recipe_template & arrt)
	{
		return __GetInstance().__InsertTemplate(arrt);
	}
	
	static bool Insert(const addonchange_recipe_template & acrt)
	{
		return __GetInstance().__InsertTemplate(acrt);
	}

	static bool Insert(const lib_recipe_template & lib)
	{
		return __GetInstance().__InsertTemplate(lib);
	}

	static bool Insert(const easy_recipe_template & easy)
	{
		return __GetInstance().__InsertTemplate(easy);
	}

	static recipe_template * GetRecipe(int id)
	{
		return __GetInstance().__GetTemplate(id);
	}

	static decompose_recipe_template * GetDecomposeRecipe(int id)
	{
		return __GetInstance().__GetDecomposeTemplate(id);
	}
	
	static engrave_recipe_template * GetEngraveRecipe(int id)
	{
		return __GetInstance().__GetEngraveTemplate(id);
	}
	
	static addonregen_recipe_template * GetAddonRegenRecipe(int id)
	{
		return __GetInstance().__GetAddonRegenTemplate(id);
	}

	static addonchange_recipe_template * GetAddonChangeRecipe(int id)
	{
		return __GetInstance().__GetAddonChangeTemplate(id);
	}

	static lib_recipe_template * GetLibRecipe(int id)
	{
		return __GetInstance().__GetLibTemplate(id);
	}

	static easy_recipe_template * GetEasyRecipe(int id)
	{
		return __GetInstance().__GetEasyTemplate(id);
	}
	
	static recipe_template * GetByItemID(int id)
	{
		return __GetInstance().__GetByItemID(id);
	}

	
	static bool LoadTemplate(itemdataman & dataman);
};

struct gnpc;
class world;
class gnpc_imp;
class gmatter_mine_imp;
class base_spawner
{
public:
	class generate_pos : public substance
	{
		public:
			virtual void Generate(const A3DVECTOR &pos_min,const A3DVECTOR &pos_max,A3DVECTOR &pos,float offset, world * plane)=0;
			virtual float GenerateY(float x, float y,float z, float offset,world * plane) = 0;
	};
	base_spawner():_region(0,0,0,0),_pos_min(0,0,0),_pos_max(0,0,0),_pos_generator(0),_pool_lock(0),
		       _auto_respawn(true),_is_spawned(false),_dir(0),_dir1(0),_rad(0),_collision_id(0)
		       {}
protected:
	rect	_region;
	A3DVECTOR _pos_min;				//��С������λ��
	A3DVECTOR _pos_max;				//��������λ��
	generate_pos * _pos_generator;
	int _pool_lock;					//���ǿ����޸�NPC�صĲ���ʱ�ӵ���
	abase::static_multimap<XID,int, abase::fast_alloc<> > _xid_list;
	bool	_auto_respawn;				//�Զ�����
	bool	_is_spawned;				//�Ƿ���Ч
	virtual ~base_spawner() { delete _pos_generator;}
	virtual void Release() = 0;
public:
	char _dir;					//����ģ����ķ��򣬹�NPCʹ��
	char _dir1;
	char _rad;
	int  _collision_id;
	void SetDir(unsigned char dir0,unsigned char dir1, unsigned char rad)
	{
		_dir = dir0;
		_dir1 = dir1;
		_rad = rad;
	}
	virtual void GeneratePos(A3DVECTOR &pos,float offset_terrain, world * plane)
	{
		_pos_generator->Generate(_pos_min, _pos_max,pos,offset_terrain,plane);
	}
	virtual char GenDir()
	{
		if(_pos_min.squared_distance( _pos_max) < 1e-3)
		{
			return _dir;
		}
		else
		{
			return abase::Rand(0,255);
		}
	}
	void SetRegion(int region_type, const float vPos[3],const float vExts[3], const float vDir[3]);
	const rect & GetRegion() { return _region;}
	void BuildRegionCollision(world * plane, int tid, int did, float offset_terrain, int region_idx);//�̶���ͼʹ��
	void BuildRegionCollision2(world * plane, int tid, int did, float offset_terrain);//�����ͼʹ��
	virtual void BuildRegionCollision(world * plane, int region_idx){}	//�̶���ͼʹ��
	virtual void BuildRegionCollision2(world * plane){}	//�����ͼʹ��
	void ReleaseSelf()
	{
		Release();
		delete this;
	}
	void SetRespawn(bool are) 
	{ 
		_auto_respawn = are;
	}

	void Heartbeat(world * pPlane)
	{
		mutex_spinlock(&_pool_lock);
		if(IsSpawned())
		{
			OnHeartbeat(pPlane);
		}
		mutex_spinunlock(&_pool_lock);
	}

	bool BeginSpawn(world * pPlane)
	{
		bool bRst = false;
		mutex_spinlock(&_pool_lock);
		if(!IsSpawned())
		{
			bRst = CreateObjects(pPlane);	
			SetSpawned(bRst);
		}
		mutex_spinunlock(&_pool_lock);
		return bRst;
	}
	
	bool EndSpawn(world * pPlane)
	{
		bool bRst = false;
		mutex_spinlock(&_pool_lock);
		if(IsSpawned())
		{
			ClearObjects(pPlane);	
			SetSpawned(false);
			bRst = true;
		}
		mutex_spinunlock(&_pool_lock);
		return bRst;
	}
private:
	virtual void OnHeartbeat(world * pPlane) = 0;
	virtual bool CreateObjects(world *pPlane) = 0;
	virtual void ClearObjects(world *pPlane) = 0;
	void SetSpawned(bool sp)
	{
		_is_spawned = sp;
	}
	bool IsSpawned() {return _is_spawned;}
	
};


struct crontab_t
{
private:
	int min;
	int hour;
	int month;
	int year;
	int day_of_months;
	int day_of_week;
public:
	struct entry_t
	{
		int min;
		int hour;
		int month;
		int year;
		int day_of_months;
		int day_of_week;
	};
public:
	crontab_t():min(-1), hour(-1), month(-1), year(-1), day_of_months(-1), day_of_week(-1)
	{}

	bool SetParam(int __year, int __month, int __day_of_months, int __hour, int __min, int __day_of_week)
	{
		min		= __min	;
		hour		= __hour;	
		month		= __month;	
		year		= __year;	
		day_of_months	= __day_of_months;	
		day_of_week	= __day_of_week;
		
		return min >= 0 && !(day_of_week >=0 && day_of_months >= 1) && day_of_months != 0;
	}

	bool SetParam(const entry_t & param)
	{
		min		= param.min;
		hour		= param.hour;	
		month		= param.month;	
		year		= param.year;	
		day_of_months	= param.day_of_months;	
		day_of_week	= param.day_of_week;
		
		return min >= 0 && !(day_of_week >=0 && day_of_months >= 1) && day_of_months != 0;
	}

	int CheckTime(const tm &tt);
	int CheckTime2(time_t t1,int DUR);
};

class spawner_ctrl 
{
	int _spawn_id;			//�������ķ���ID
	bool _auto_spawn;		//�Ƿ��Զ�����
	bool _spawn_flag;		//�Ƿ��Ѿ�����
	bool _active_flag;		//�Ƿ��Ѿ�����		�����һ����������
	abase::vector<base_spawner *, abase::fast_alloc<> > _list;

	int _spawn_after_active;	//������ɵĻ���ʱ��
	int _active_life;		//���������� 0Ϊ����

	bool _has_active_date;
	bool _has_stop_date;
	int _active_date_duration;
	crontab_t _active_date;
	crontab_t _stop_date;

	int  _date_counter_down;
	int  _cur_active_life;	
	int  _time_before_spawn;
	int  _lock;
public:
	
	spawner_ctrl():_spawn_id(0),_auto_spawn(true),_spawn_flag(false),_active_flag(false)
	{
		_spawn_after_active = 0;
		_active_life = 0;
		_has_active_date = 0;
		_has_stop_date = 0;

		_date_counter_down = 0;
		_cur_active_life = 0;
		_time_before_spawn = 0;
		_active_date_duration = 0;
		_lock = 0;
	}
	inline bool IsAutoSpawn() { return _auto_spawn;}
	inline bool IsActived() { return _active_flag;}
	inline void AddSpawner(base_spawner * sp) {_list.push_back(sp);}

	void SetCondition(int spawn_id, bool auto_spawn, int spawn_delay = 0, int active_life = 0)
	{
		_spawn_id = spawn_id;
		_auto_spawn = auto_spawn;
		_spawn_after_active =  spawn_delay;
		_active_life = active_life;
	}

	bool SetActiveDate(const crontab_t::entry_t & ent, int duration)
	{
		_has_active_date = false;
		if(!_active_date.SetParam(ent)) return false;
		_active_date_duration = duration;
		_has_active_date = true;
		return true;
	}

	bool SetStopDate(const crontab_t::entry_t & ent)
	{
		_has_stop_date = false;
		if(!_stop_date.SetParam(ent)) return false;
		_has_stop_date = true;
		return true;
	}

	void OnHeartbeat(world * pPlane);
protected:
	inline void SetSpawnFlag(bool sf) { _spawn_flag = sf;}
	inline bool IsSpawned() { return _spawn_flag;}
	void Spawn(world * pPlane);
	void Active(world * pPlane);
	void Stop(world * pPlane);

public:
	bool BeginSpawn(world * pPlane)
	{
		spin_autolock keeper(_lock);
		if(IsActived()) return false;
		Active(pPlane);
		return true; 
	}

	bool EndSpawn(world * pPlane)
	{
		spin_autolock keeper(_lock);
		if(!IsActived()) return false;
		Stop(pPlane);
		return true; 
	}
};

class gnpc_imp;
class npc_spawner : public base_spawner
{
	public:
		struct entry_t
		{
			int npc_tid;
			int msg_mask_or;
			int msg_mask_and;
			int alarm_mask;			//δ�� 
			int enemy_faction;		//δ�� 
			bool has_faction;
			int faction;			//δ�� 
			bool ask_for_help;
			int monster_faction_ask_help;
			bool accept_ask_for_help; 
			int monster_faction_accept_for_help;
			int reborn_time_upper;	//����ʱ������
			int reborn_time_lower;	//����ʱ������
			int path_id;			//Ѳ��·�ߣ������0��δ��
			int path_type;			//Ѳ������
			int corpse_delay;		//ʬ�����ʱ�䣬��λΪ�� 
			bool speed_flag;		//�ٶȱ��
			unsigned int mobs_count;
			float offset_terrain;		//�͵��θ߶ȵ�ƫ����
		};
	protected:
		int _reborn_time;				//�ܵ�����ʱ�䣬��group_spawnerʹ��
		abase::vector<entry_t, abase::fast_alloc<> > _entry_list;//���������е�����entry�б�
		abase::vector<gnpc *, abase::fast_alloc<> > _npc_pool;   //npc�����ɳأ������ͷŵ�npc �ᱻ���嵽����
		int _mobs_total_gen_num;				//�ܹ����ɶ��ٶ���
		int _mobs_cur_gen_num;					//��ǰ�����˶��ٶ���
		int _mob_life;						//�������������

	public:
		npc_spawner():_reborn_time(0),_mobs_total_gen_num(0),_mobs_cur_gen_num(0),_mob_life(0){}
		virtual ~npc_spawner() {}
		virtual int Init(const void * buf, unsigned int len) { return 0;}
		virtual void OnHeartbeat(world * pPlane) = 0;
		virtual bool Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp,bool is_reset) = 0;
		virtual bool CreateMobs(world *pPlane) {return false;}
		virtual void ForwardFirstAggro(world * pPlane,const XID & id, int rage) {}

		virtual bool CreateObjects(world *pPlane)
		{
			_mobs_cur_gen_num = 0;
			return CreateMobs(pPlane);
		}
		virtual void ClearObjects(world * pPlane);

		static gnpc * CreateMobBase(npc_spawner * __this,world * pPlane,const entry_t & et,
				int spawn_index, const A3DVECTOR & pos,const int cid[3],unsigned char dir, 
				int ai_policy_cid,int aggro_policy, gnpc * orign_npc,int life = 0);
		static gnpc * CreatePetBase(gplayer_imp *pImp, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode,
				const int cid[3],unsigned char dir, unsigned char pet_stamp, 
				int ai_policy_cid, int aggro_policy);
		static gnpc * CreatePetBase2(gplayer_imp *pImp, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode,
				const int cid[3],unsigned char dir, unsigned char pet_stamp, 
				int ai_policy_cid, int aggro_policy, int skill_level);
		static gnpc * CreatePetBase3(gplayer_imp * pMaster, const pet_data * pData, const A3DVECTOR & pos, char inhabit_mode, const int cid[3],
				unsigned char dir,  unsigned char pet_stamp, 
				int ai_policy_cid , int aggro_policy);
		static gnpc * CreateNPCBase(npc_spawner * __this, world * pPlane, const entry_t & et,
				int spawn_index, const A3DVECTOR & pos,const int cid[3], unsigned char dir,
				int ai_policy_cid = -1,int aggro_policy = 0,gnpc * origin_npc = NULL, int life = 0);

		static void AdjustPropByCommonValue(gnpc_imp * pImp, world * pPlane, npc_template * pTemplate);
		void RegenAddon(gnpc_imp * pImp, int npc_id);		
		
		void SetGenLimit(int life, int total_num)
		{
			_mob_life = life;
			_mobs_total_gen_num = total_num;
		}


	public:
		bool AddEntry(const entry_t & ent)
		{
			_entry_list.push_back(ent);
			_npc_pool.push_back(NULL);
			return true;
		}
		
		virtual void BuildRegionCollision(world * plane, int region_idx);	//�̶���ͼʹ��
		virtual void BuildRegionCollision2(world * plane);	//�����ͼʹ��

		int CutRegion(const rect & rt)
		{
			float oa = _region.GetArea();
			if(oa <= 0.01f) 
			{
				//��������ǳ�С������
				//��Ϊ������
				int total_count = 0;
				for(unsigned int i = 0; i < _entry_list.size(); i ++)
				{
					total_count += _entry_list[i].mobs_count;
				}
				return total_count;
			}
			
			float na = rt.GetArea();
			float factor = na/oa;
			int total_count = 0;
			//�����������������ֵ
			for(unsigned int i = 0; i < _entry_list.size(); i ++)
			{
				ASSERT(_npc_pool[i] == 0);
				int mobs_count = (int)(_entry_list[i].mobs_count * factor + 0.5f);
				if(mobs_count <= 0)
				{
					//Ӧ��ɾ�������Ŀ
					_entry_list.erase(_entry_list.begin() + i);
					_npc_pool.erase(_npc_pool.begin() + i);
					--i;
					continue;
				}
				_entry_list[i].mobs_count = mobs_count;
				total_count += mobs_count;
			}
			_region = rt;
			_pos_min.x = rt.left; _pos_min.z = rt.top;
			_pos_max.x = rt.right; _pos_max.z = rt.bottom;
			return total_count;
		}

		void SetRebornTime(int rtime) 
		{
			_reborn_time = rtime;
		}
};

class mine_spawner : public base_spawner
{
public:
	struct entry_t
	{
		int mid;
		int mine_count;
		int reborn_time;
	};
protected:
	abase::vector<entry_t,abase::fast_alloc<> > _entry_list;//���������е�����entry�б�
	abase::vector<gmatter *,abase::fast_alloc<> > _mine_pool;//npc�����ɳأ������ͷŵ�npc �ᱻ���嵽����
	float _offset_terrain;
	int _mine_total_gen_num;				//�ܹ����ɶ��ٶ���
	int _mine_cur_gen_num;					//��ǰ�����˶��ٶ���

public:
	mine_spawner():_offset_terrain(0),_mine_total_gen_num(0),_mine_cur_gen_num(0) {}
	virtual ~mine_spawner() {}
	virtual void OnHeartbeat(world * pPlane);
	virtual bool Reclaim(world * pPlane,gmatter * pMatter, gmatter_mine_imp * imp);
	virtual bool CreateMines(world * pPlane);
	static gmatter * CreateMine(mine_spawner* ,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & ent);
	static gmatter * CreateMine2(mine_spawner* ,const A3DVECTOR & pos, world * pPlane,int index,const entry_t & ent,unsigned char dir, unsigned char dir1, unsigned char rad);
	static void GenerateMineParam(gmatter_mine_imp * imp, npc_template * pTemplate);
	void Reborn(world * pPlane,gmatter * header, gmatter * tail,int mid,int index);
	void SetOffsetTerrain(float offset) { _offset_terrain = offset; }
	void Release();
	void SetGenLimit(int total_num)
	{
		_mine_total_gen_num = total_num;
	}


	virtual bool CreateObjects(world *pPlane)
	{
		_mine_cur_gen_num = 0;
		return CreateMines(pPlane);
	}

	virtual void ClearObjects(world * pPlane);

public:
	bool AddEntry(const entry_t & ent)
	{
		_entry_list.push_back(ent);
		_mine_pool.push_back(NULL);
		return true;
	}

	virtual void BuildRegionCollision(world * plane, int region_idx);	//�̶���ͼʹ��
	virtual void BuildRegionCollision2(world * plane);	//�����ͼʹ��

	int CutRegion(const rect & rt)
	{
		float oa = _region.GetArea();
		if(oa <= 0.01f) 
		{
			//��������ǳ�С������
			//��Ϊ������
			int total_count = 0;
			for(unsigned int i = 0; i < _entry_list.size(); i ++)
			{
				total_count += _entry_list[i].mine_count;
			}
			return total_count;
		}

		float na = rt.GetArea();
		float factor = na/oa;
		int total_count = 0;
		//�����������������ֵ
		for(unsigned int i = 0; i < _entry_list.size(); i ++)
		{
			ASSERT(_mine_pool[i] == 0);
			int mine_count = (int)(_entry_list[i].mine_count * factor + 0.5f);
			if(mine_count <= 0)
			{
				//Ӧ��ɾ�������Ŀ
				_entry_list.erase(_entry_list.begin() + i);
				_mine_pool.erase(_mine_pool.begin() + i);
				--i;
				continue;
			}
			_entry_list[i].mine_count = mine_count;
			total_count += mine_count;
		}
		_region = rt;
		_pos_min.x = rt.left; _pos_min.z = rt.top;
		_pos_max.x = rt.right; _pos_max.z = rt.bottom;
		return total_count;
	}
};
class dyn_object_spawner : public mine_spawner  
{       
	unsigned char _scale;
public:         
	virtual bool CreateMines(world * pPlane);
	dyn_object_spawner() :_scale(0) {} 
	inline void SetScale(unsigned char s) {_scale = s;}
protected:      
	gmatter * CreateDynObject(const A3DVECTOR & pos,unsigned int index, world * pPlane,const entry_t & ent);
};

class CNPCGenMan;
class npc_generator : public ONET::Thread::Runnable
{

private:
//	abase::vector<npc_spawner*,abase::fast_alloc<> > _spawner_list;
//	abase::vector<mine_spawner*,abase::fast_alloc<> > _mine_spawner_list;

	abase::vector<base_spawner *,abase::fast_alloc<> > _sp_list;
	abase::vector<spawner_ctrl *,abase::fast_alloc<> > _ctrl_list;

	std::unordered_map<int, spawner_ctrl *> _ctrl_map;		//condition id-->ctrl �Ķ��ձ������沢δ��������ctrl
	std::unordered_map<int, spawner_ctrl *> _ctrl_idx_map; 	//ctrl id-->ctrl�Ķ��ձ� ������������е�ctrl

	npc_generator & operator=(const npc_generator & );
public:
	npc_generator():_task_offset(0),_tcount(0),_tlock(0),_plane(0)
	{
		//_spawner_list.reserve(MAX_SPAWN_COUNT);
		//_mine_spawner_list.reserve(MAX_SPAWN_COUNT);
		_sp_list.reserve(MAX_SPAWN_COUNT);
	}

	bool LoadGenData(world* plane, CNPCGenMan & npcgen, rect & region);
	bool AddCtrlData(CNPCGenMan& ctrldata,unsigned int ctrl_id, unsigned char block_id);
	bool AddSpawnData(world* plane, CNPCGenMan& ctrldata,CNPCGenMan& spawndata, unsigned char block_id, const A3DVECTOR& p_offset, bool global_ctrl_gen, bool unique_resource);
	
	bool InsertSpawner(int ctrl_id, base_spawner * sp)
	{
		spawner_ctrl * ctrl = _ctrl_idx_map[ctrl_id];
		if(ctrl == NULL) 
		{
			__PRINTF("���������޷��ҵ����ʵĿ��ƶ���\n");
			return false;
		}

		_sp_list.push_back(sp);
		ctrl->AddSpawner(sp);
		return true;
	}

	bool InsertSpawnControl(int ctrl_idx, int condition_id, bool auto_spawn,int spawn_delay, int active_life,
				int active_date_duration,
				const crontab_t::entry_t * active_date = NULL, 
				const crontab_t::entry_t * stop_date = NULL)
	{
		spawner_ctrl * ctrl = _ctrl_idx_map[ctrl_idx];
		if(ctrl) return false;
		if(condition_id > 0)
		{
			ctrl = _ctrl_map[condition_id];
			if(ctrl) return false;
		}
		ctrl = new  spawner_ctrl();
		ctrl->SetCondition(condition_id, auto_spawn, spawn_delay, active_life);
		bool bRst = true;
		if(active_date)
		{
			bRst = bRst  && ctrl->SetActiveDate(*active_date,active_date_duration);
		}

		if(stop_date)
		{
			bRst = bRst  && ctrl->SetStopDate(*stop_date);
		}

		_ctrl_idx_map[ctrl_idx] = ctrl;
		_ctrl_list.push_back(ctrl);

		if(condition_id > 0) _ctrl_map[condition_id] = ctrl;
		return bRst;
	}

	bool InitIncubator(world * pPlane);		//��ʼ�����е�spawner��ȥ���������Լ��Ĳ���

	bool BeginSpawn();
	bool TriggerSpawn(int condition);
	void ClearSpawn(int condition);
	void Release();
public:
	
	void StartHeartbeat()
	{
		_task_offset = 0;
		_task_offset2 = 0;
		_tcount = 0;
		_tcount2 = 0;
		_tlock = 0;
	}
	
	static int GenBlockUniqueID(int id, unsigned char block_id)
	{
		return (id&0xFFFFFF) + ((block_id << 24)& 0xFF000000);
	}

protected:
	unsigned int _task_offset;
	unsigned int _task_offset2;
	int _tcount;
	int _tcount2;
	int _tlock;
	world * _plane;

	void OnTimer(int index,int rtimes)
	{
		ONET::Thread::Pool::AddTask(this);
	}
public:

/*
	virtual void Run()
	{
		spin_autolock keeper(_tlock);
		_tcount += _spawner_list.size();

		while(_tcount > TICK_PER_SEC)
		{
			_spawner_list[_task_offset]->OnHeartbeat(_plane);
			_task_offset ++;
			if(_task_offset >= _spawner_list.size()) _task_offset = 0;
			_tcount -= TICK_PER_SEC;
		}

		_tcount2 += _mine_spawner_list.size();
		while(_tcount2 > TICK_PER_SEC)
		{
			_mine_spawner_list[_task_offset2]->OnHeartbeat(_plane);
			_task_offset2 ++;
			if(_task_offset2 >= _mine_spawner_list.size()) _task_offset2 = 0;
			_tcount2 -= TICK_PER_SEC;
		}
	}*/

	virtual void Run()
	{
		spin_autolock keeper(_tlock);
		_tcount += _sp_list.size();

		while(_tcount > TICK_PER_SEC)
		{
			_sp_list[_task_offset]->Heartbeat(_plane);
			_task_offset ++;
			if(_task_offset >= _sp_list.size()) _task_offset = 0;
			_tcount -= TICK_PER_SEC;
		}

		_tcount2 += _ctrl_list.size();
		while(_tcount2 > TICK_PER_SEC)
		{
			_ctrl_list[_task_offset2]->OnHeartbeat(_plane);
			_task_offset2 ++;
			if(_task_offset2 >= _ctrl_list.size()) _task_offset2 = 0;
			_tcount2 -= TICK_PER_SEC;
		}
	}

};

class mobs_spawner : public npc_spawner
{
public:
	int _reborn_count;
	int _rrcount;
protected:
	//path_graph ..
	gnpc * CreateMob(world * pPlane,int spawn_index,const entry_t & et);
	void Reborn(world * pPlane,gnpc * header, gnpc * tail,float height,int tid);

	virtual void ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos, int index);
public:
	mobs_spawner():_reborn_count(0),_rrcount(50){}
	void OnHeartbeat(world * pPlane);

	//�������е�mobs 
	virtual bool CreateMobs(world * pPlane);
	virtual bool Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp, bool is_reset);
	virtual void Release();

};

//npc�����Ƿ�Ӧ�ý�����npc����һ��spawner����
class server_spawner : public mobs_spawner
{
public:
protected:
	//path_graph ..
	gnpc* CreateNPC(world * pPlane, int spawn_index,const entry_t & et);
	gnpc* CreateNPC(world * pPlane, int spawn_index,const entry_t & et, const A3DVECTOR & pos, gnpc * pNPC);

	//void Reborn(world * pPlane,gnpc * header, gnpc * tail,float height,int tid);
	virtual void ReCreate(world * pPlane, gnpc * pNPC, const A3DVECTOR & pos, int index);
public:
	server_spawner(){}
	void OnHeartbeat(world * pPlane);

	//�������е�mobs 
	virtual bool CreateMobs(world * pPlane);
	virtual bool Reclaim(world * pPlane,gnpc * pNPC,gnpc_imp * pImp, bool is_reset);
	
};

//��ͨȺ��
class group_spawner : public mobs_spawner
{
protected:
	int _next_time;
	int _lock;				//����Ⱥ�ֵ���������������ʱ��ʱ�����������������������Թ鲢��_pool_lock��
	int _leader_id;
	A3DVECTOR _leader_pos;
	bool _gen_pos_mode;
	gnpc * CreateMasterMob(world * pPlane,int spawner_index, const entry_t &et);
	gnpc * CreateMinorMob(world * pPlane,int spawner_index, int leader_id, const A3DVECTOR & leaderpos, const entry_t &et);
public:
	group_spawner():_next_time(0),_lock(0),_leader_id(0),_gen_pos_mode(false)
	{
		_rrcount = 1;
	}
	virtual void ClearObjects(world *pPlane) 
	{
		mobs_spawner::ClearObjects(pPlane);
		_next_time = 0;
		_leader_id = 0;
	}
	
	void OnHeartbeat(world * pPlane);
	virtual bool CreateMobs(world *pPlane);
	virtual bool Reclaim(world * pPlane, gnpc * pNPC, gnpc_imp * imp, bool is_reset);
	virtual void GeneratePos(A3DVECTOR &pos,float offset_terrain,world * plane);
};

//����Ⱥ��
class boss_spawner : public group_spawner
{
protected:
	abase::vector<XID,abase::fast_alloc<> > _mobs_list;
	int _mobs_list_lock;
public:
	boss_spawner()
	{
		_mobs_list_lock = 0;
	}
	virtual void ClearObjects(world *pPlane) 
	{
		group_spawner::ClearObjects(pPlane);
		spin_autolock keep(_mobs_list_lock);
		_mobs_list.clear();
	}
	
	virtual bool CreateMobs(world *pPlane);
	gnpc * CreateMasterMob(world * pPlane,int spawner_index, const entry_t &et);
	gnpc * CreateMinorMob(world * pPlane,int spawner_index, int leader_id, const A3DVECTOR & leaderpos, const entry_t &et);
	virtual void ForwardFirstAggro(world* pPlane,const XID & id, int rage);
};
#endif

