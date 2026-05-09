#ifndef __S2C_SOURCE_H__
#define __S2C_SOURCE_H__

#include "../types.h"
#include "s2c_header.h"

#pragma pack(1)

namespace S2C
{
	namespace INFO
	{
		struct player_info_1
		{
			int cid;
			A3DVECTOR pos;
			unsigned short crc;
			unsigned short custom_crc;	//�Զ������ݵ�crcֵ
			unsigned char  dir;		//256�Ķ�������ʾ����
			unsigned char  sec_level;	//���漶��
			unsigned int   object_state;	//��ǰ״̬����0λ��ʾ�Ƿ�����
			unsigned int   object_state2;	//��ǰ״̬
			unsigned int   object_state3; /*176*/
			//unsigned char   unk;	//177
		};

		struct player_info_2			//name ,customize data �����ı�
		{
			unsigned char size;
			char data[];
		};

		struct player_info_3			//viewdata of equipments and special effect
		{
			unsigned char size;
			char  data[];
		};

		struct player_info_4			//detail
		{
			unsigned short size;
			char	data[];
		};
		struct player_info_00			//�Լ��ᾭ���ı������
		{
			short lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int target_id;  //todo ddr 1023
			int glyph_pvp_attr;
		};
		struct self_info_00
		{
			short lvl;
			unsigned char combat_state;
			unsigned char sec_level;
			int hp;
			int max_hp;
			int mp;
			int max_mp;
			int exp;
			int sp;
			int Unk; //174
			int ap;
		};

		struct npc_info
		{
			int nid;			//npc id
			int tid;			//template id
			int vis_id;			//visible template id
			A3DVECTOR pos;
			unsigned short seed;		// seed of customize data
			unsigned char dir;
			unsigned int  object_state;
			unsigned int  object_state2;
		};

		struct npc_info_00
		{
			int hp;
			int max_hp;
			int target_id;  //todo ddr 1023
		};

		struct matter_info_1
		{
			int mid;
			int tid;
			A3DVECTOR pos;
			unsigned char dir0;
			unsigned char dir1;
			unsigned char rad;
			unsigned char state;
			unsigned char value;
		};
		struct matter_info_2
		{
			int mid;
			unsigned char size;
			unsigned char data[];
		};

		struct self_info_1
		{
			int exp;
			int sp;
			int cid;
			A3DVECTOR pos;
			unsigned short crc;
			unsigned short custom_crc;	//�Զ������ݵ�crcֵ
			unsigned char  dir;		//256�Ķ�������ʾ����
			unsigned char  sec_level;	//���漶��
			unsigned int object_state;
			unsigned int object_state2;
			unsigned int object_state3; /*176+*/
			//unsigned char unk; //177
		};

		struct move_info
		{
			int cid;
			A3DVECTOR dest;
			unsigned short use_time;
			unsigned short speed;
			unsigned char  move_mode;
		};
			
		union  market_goods
		{
			struct 
			{
				int type;		//��Ʒ����  �����0 ��ʾû��������
			}empty_item;
			struct 
			{
				int type;		//��Ʒ����
				int count;		//ʣ����ٸ� ������ʾ����
				unsigned int price;		//����
				int reserve;
				unsigned short content_length;
				char content[];
			}item;

			struct 
			{
				int type;		//��Ʒ����
				int count;		//ʣ����ٸ� ������ʾ����
				unsigned int price;		//����
				int reserve;
			}order_item;
			

			
		};
	}

	namespace CMD
	{
		using namespace INFO;

		struct player_enter_slice	//player����ɼ�����
		{	
			single_data_header header;
			player_info_1 data;
		};

		struct npc_enter_slice
		{
			single_data_header header;
			npc_info data;
		};

		struct leave_slice	//player�뿪�ɼ�����
		{	
			single_data_header header;
			int id;
		};

		struct notify_pos		//player����λ��
		{	
			single_data_header header;
			A3DVECTOR pos;
			int tag;
			int key;
		};

		struct self_info_1
		{
			single_data_header header;
			INFO::self_info_1 info;
		};

		struct	player_info_1_list 		//player list
		{
			multi_data_header header;
			player_info_1 list[];
		};

		struct player_info_2_list 
		{
			multi_data_header header;
			/*
			   struct		//����Ĵ��룬ʵ��������info2�Ǳ䳤�Ľṹ�������޷�������֯
			   {
			   int cid;
			   player_info_2 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_3_list 
		{
			multi_data_header header;
			/*
			   struct		//����Ĵ��룬ʵ��������info3�Ǳ䳤�Ľṹ�������޷�������֯
			   {
			   int cid;
			   player_info_3 info;
			   }list[];
			 */
			char data[1];
		};

		struct player_info_23_list 
		{
			multi_data_header header;
			/*
			   struct		//����Ĵ��룬ʵ��������info2/3�Ǳ䳤�Ľṹ�������޷�������֯
			   {
			   int cid;
			   player_info_2 info2;
			   player_info_3 info3;
			   }list[];
			 */
			char data[1];
		};

		struct	npc_info_list 		// npc list
		{
			multi_data_header header;
			npc_info list[1];
		};

		struct matter_info_list
		{
			multi_data_header header;
			matter_info_1 list[1];
		};

		struct matter_enter_world
		{
			single_data_header header;
			matter_info_1 data;
		};

		struct npc_enter_world
		{
			single_data_header header;
			npc_info info;
		};

		struct player_info_00
		{
			single_data_header header;
			int pid;
			INFO::player_info_00 info;
		};

		struct self_info_00
		{
			single_data_header header;
			INFO::self_info_00 info;
			
		};

		struct npc_info_00
		{
			single_data_header header;
			int nid;
			INFO::npc_info_00 info;
		};
		
		struct object_move
		{
			single_data_header header;
			move_info data;
		};
		struct player_enter_world
		{
			single_data_header header;
			player_info_1	info;
		};
		struct player_leave_world
		{
			single_data_header header;
			int	   cid;
		};
		struct player_select_target
		{
			single_data_header header;
			int	id;
		};

		struct npc_dead
		{
			single_data_header header;
			int	   nid;
			int 	   attacker;
		};
		struct object_disappear
		{
			single_data_header header;
			int	   nid;
		};
		struct object_start_attack
		{
			single_data_header header;
			int	oid;
			int	tid;	//target id
			unsigned char	attack_speed;
		};

		struct self_start_attack
		{
			single_data_header header;
			int	tid;	//target id
			unsigned short ammo_remain;
			unsigned char attack_speed;
		};
		struct self_stop_attack
		{	
			single_data_header header;
			int	flag;
		};

		struct self_attack_result 
		{
			single_data_header header;
			int 	target_id;
			int	damage;			//�����0��ʾû�л���
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			unsigned char speed;
		};

		struct self_skill_attack_result 
		{
			single_data_header header;
			int 	target_id;
			int 	skill_id;
			int	damage;			//�����0��ʾû�л���
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			unsigned char speed;
			unsigned char section;	// ���ܶ���
		};
		
		struct hurt_result //hurt_result
		{
			single_data_header header;
			int 	target_id;
			int	damage;
		};

		struct object_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int damage;
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			char speed;			//�����ٶ�
			
		};

		struct object_skill_attack_result
		{
			single_data_header header;
			int attacker_id;
			int target_id;
			int skill_id;
			int damage;
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			char speed;			//�����ٶ�
			unsigned char section;	// ���ܶ���
		};

		struct error_msg
		{
			single_data_header header;
			int  msg;
		};

		struct be_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char eq_index; //��λ������ι����ǲ���Ӧ�ñ��ɫ
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			char speed;			//�����ٶ�
		};

		struct be_skill_attacked
		{
			single_data_header header;
			int 	attacker_id;
			int 	skill_id;
			int 	damage;
			unsigned char eq_index; //��λ������ι����ǲ���Ӧ�ñ��ɫ
			int attack_flag;	//��Ǹù����Ƿ��й����Ż����ͷ����Ż������ػ�����
			char speed;			//�����ٶ�
			unsigned char section;	// ���ܶ���
		};


		struct be_hurt
		{
			single_data_header header;
			int 	attacker_id;
			int 	damage;
			unsigned char invader;
		};
		
		struct player_dead
		{
			single_data_header header;
			int  killer;
			int  player;
		};
		struct be_killed
		{
			single_data_header header;
			int  killer;
			A3DVECTOR pos;
		};

		struct player_revival
		{
			single_data_header header;
			int id;
			short type;	//��������� 0 �سǸ��1 ��ʼ���� 2��������ȫ���
			A3DVECTOR pos;
		};
		struct player_pickup_money
		{
			single_data_header header;
			int amount;
			unsigned char is_silver;
		};
		struct player_pickup_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned int amount;
			unsigned int slot_amount;
			unsigned char where;		//���ĸ���������0 ��׼��2 ����1 װ��
			unsigned char index;		//��󲿷ַ����ĸ�λ��
		};

		struct player_purchase_item
		{
			single_data_header header;
			unsigned int cost;
			unsigned int yinpiao;			//���ڹ˿͹���ʱ��Ч
			unsigned char type;		//��ʾ�˿����뻹������
			unsigned short item_count;
			struct
			{
				int item_id;
				int expire_date;
				unsigned int count;
				unsigned short inv_index;
				unsigned char  stall_index;
			} item_list[];
		};
		
		struct OOS_list
		{
			single_data_header header;
			unsigned int count;
			int id_list[1];
		};
		struct object_stop_move
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;
		};
		struct receive_exp
		{
			single_data_header header;
			int exp;
			int sp;
		};

		struct level_up
		{
			single_data_header header;
			int id;
		};

		struct unselect 
		{
			single_data_header header;
		};
		
		struct self_item_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int type;
			int expire_date;
			//int item_state;
			int proc_type;
			unsigned int count;
			//unsigned int pile_limit;
			//int equip_mask;
			//int guid1;
			//int guid2;
			//int price;
			unsigned short crc;
			int pickup_date;
			int pickup_expire;
			unsigned short content_length;
			char content[];
		};
		struct self_item_empty_info
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
		};
		struct self_inventory_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			unsigned int content_length;
			char content[];
		};

		struct self_inventory_detail_data
		{
			single_data_header header;
			unsigned char where;
			unsigned char inv_size;
			unsigned int content_length;
			char content[];
		};

		struct  exchange_inventory_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};
		
		struct move_inventory_item
		{
			single_data_header header;
			unsigned char src;
			unsigned char dest;
			unsigned int count;
		};

		struct player_drop_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			unsigned int count;
			int type;
			unsigned char drop_type;
		};
		struct exchange_equipment_item
		{
			single_data_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct equip_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned int count_inv;
			unsigned int count_equip;
		};

		struct move_equipment_item
		{
			single_data_header header;
			unsigned char index_inv;
			unsigned char index_equip;
			unsigned int amount;
		};

		struct self_get_property
		{
			single_data_header header;
			unsigned int status_point;
			int 	attack_degree;
			int	defend_degree;
			int crit_rate;
			int crit_damage_bonus;
			int invisible_degree;
			int anti_invisible_degree;
			int penetration;
			int resilience;
			int vigour;
			int anti_defense_degree;
			int anti_resistance_degree;
			int highest_level; // Nível de Pico
			//	extend_prop prop;		//��������ͷ�ļ�������ע�͵���
		};

		struct set_status_point
		{
			single_data_header header;
			unsigned int vit;
			unsigned int eng;
			unsigned int str;
			unsigned int agi;
			unsigned int remain_point;
		};

		struct player_extprop_base
		{
			single_data_header header;
			int id;
			int vitality;		//��
			int energy;		//��
			int strength;		//��
			int agility;		//��
			int max_hp;		//���hp
			int max_mp;		//���mp
			int hp_gen;		//hp�ָ��ٶ�
			int mp_gen;		//mp�ָ��ٶ� 
		};

		struct player_extprop_move
		{
			single_data_header header;
			int id;
			float walk_speed;	//�����ٶ� ��λ  m/s
			float run_speed;	//�����ٶ� ��λ  m/s
			float swim_speed;	//��Ӿ�ٶ� ��λ  m/s
			float flight_speed;	//�����ٶ� ��λ  m/s
		};

		struct player_extprop_attack
		{
			single_data_header header;
			int id;
			int attack;		//������ attack rate
			int damage_low;		//���damage
			int damage_high;	//�������damage
			int attack_speed;	//����ʱ���� ��tickΪ��λ
			float attack_range;	//������Χ
			struct 			//������������ħ���˺�
			{
				int damage_low;
				int damage_high;
			} addon_damage[5];		
			int damage_magic_low;		//ħ����͹�����
			int damage_magic_high;		//ħ����߹�����
		};

		struct player_extprop_defense
		{
			single_data_header header;
			int id;
			int resistance[5];	//ħ������
			int defense;		//������
			int armor;		//�����ʣ�װ�׵ȼ���
		};
		
		struct team_leader_invite
		{
			single_data_header header;
			int leader;
			int seq;
			short pickup_flag;
		};

		struct team_reject_invite
		{
			single_data_header header;
			int member;
		};

		struct team_join_team
		{
			single_data_header header;
			int leader;
			short pickup_flag;
		};

		struct  team_member_leave
		{
			single_data_header header;
			int leader;
			int member;
			short type;
		};
		
		//�����Լ��뿪�˶���
		struct team_leave_party
		{
			single_data_header header;
			int leader;
			short type;
		};

		struct team_new_member
		{
			single_data_header header;
			int member;
		};

		struct team_leader_cancel_party
		{
			single_data_header header;
			int leader;
		};

		struct team_member_data
		{
			single_data_header header;
			unsigned char member_count;
			unsigned char data_count;
			int leader;
			struct 
			{
				int id;
				short level;
				unsigned char combat_state;
				unsigned char sec_level;
				unsigned char reincarnation_times;
				char wallow_level;
				int hp;
				int mp;
				int max_hp;
				int max_mp;
				int force_id;
				int profit_level;
			} data[1];
		};

		struct teammate_pos
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int tag;
			char same_plane;
		};

		struct send_equipment_info
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			uint64_t mask;
			struct 
			{
				unsigned int item;
				unsigned short mask;
			}data[];	//0 ~ 16
		};
		
		struct equipment_info_changed
		{
			single_data_header header;
			unsigned short crc;
			int id;		//who
			uint64_t mask_add;
			uint64_t mask_del;
			struct 
			{
				unsigned int item;
				unsigned short mask;
			}data_add[];	//0 ~ 16
		};

		struct equipment_damaged
		{
			single_data_header header;
			unsigned char index;
			char reason;	// 0:���;��� 1:�����������װ�����
		};

		struct team_member_pickup
		{
			single_data_header header;
			int id;
			int type;
			int count;
		};

		struct npc_greeting
		{
			single_data_header header;
			int id;
		};

		struct npc_service_content
		{
			single_data_header header;
			int id;
			int type;	//���������
			unsigned int length;
			char data[];
		};

		struct item_to_money
		{
			single_data_header header;
			unsigned short index;		//�ڰ����������������
			int type;			//��Ʒ������
			unsigned int count;
			unsigned int money;
			unsigned int silver_money;
			unsigned int guild_money;
		};

		struct repair_all
		{
			single_data_header header;
			unsigned int cost;
		};
		
		struct repair
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			unsigned int cost;
		};

		struct renew
		{
			single_data_header header;
		};

		struct spend_money
		{
			single_data_header header;
			unsigned int cost;
		};

		struct player_pickup_money_in_trade
		{
			single_data_header header;
			int amount;
		};
		struct player_pickup_item_in_trade
		{
			single_data_header header;
			int type;
			unsigned int amount;
		};

		struct player_pickup_money_after_trade
		{
			single_data_header header;
			unsigned int amount;
		};
		struct player_pickup_item_after_trade
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned int amount;
			unsigned int slot_amount;
			unsigned short index;
		};
		struct get_own_money
		{	
			single_data_header header;
			unsigned int gold;
			unsigned int capacity;
			unsigned long long silver;
		};
		struct object_attack_once
		{
			single_data_header header;
			unsigned char arrow_dec;
		};

		struct object_cast_skill
		{
			single_data_header header;
			int caster;
			int target;
			int skill;
			unsigned short time;
			unsigned char level;
		};

		struct skill_interrupted
		{
			single_data_header header;
			int caster;
		};

		struct self_skill_interrupted
		{
			single_data_header header;
			unsigned char reason;
		};

		struct skill_perform
		{
			single_data_header header;
		};

		struct object_be_attacked
		{
			single_data_header header;
			int id;
		};
		
		struct skill_data
		{
			single_data_header header;
			char content[];
		};

		struct player_use_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
		};

		struct embed_item
		{
			single_data_header header;
			unsigned char chip_idx;
			unsigned char equip_idx;
		};

		struct clear_embedded_chip
		{
			single_data_header header;
			unsigned short equip_idx;
			unsigned int  cost;
		};

		struct cost_skill_point
		{
			single_data_header header;
			int skill_point;
		};

		struct learn_skill
		{
			single_data_header header;
			int skill_id;
			int skill_level;
		};

		struct object_takeoff
		{
			single_data_header header;
			int object_id;
		};

		struct object_landing
		{
			single_data_header header;
			int object_id;
		};

		struct flysword_time_capacity
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int cur_time;
		};

		struct player_obtain_item
		{
			single_data_header header;
			int type;
			unsigned int amount;
			unsigned int slot_amount;
			unsigned char where;		//���ĸ���������0 ��׼��2 ����1 װ��
			unsigned char index;		//��󲿷ַ����ĸ�λ��
		};

		struct produce_start
		{
			single_data_header header;
			unsigned short use_time;
			unsigned short count;
			int type;
		};

		struct produce_once
		{
			single_data_header header;
			int type;
			unsigned int amount;
			unsigned int slot_amount;
			unsigned char where;		//���ĸ���������0 ��׼��2 ����1 װ��
			unsigned char index;		//��󲿷ַ����ĸ�λ��
		};
		
		struct produce_end
		{
			single_data_header header;
		};

		struct result_lib_produce
		{
			single_data_header header;
		};

		struct decompose_start
		{
			single_data_header header;
			unsigned short use_time;
			int type;
		};

		struct decompose_end
		{
			single_data_header header;
		};

		struct task_data
		{
			single_data_header header;
			unsigned int active_list_size;
			char active_list[1];
			unsigned int finished_list_size;
			char finished_list[1];
			unsigned int finished_time_size;
			char finished_time[1];
			unsigned int finished_count_size;
			char finished_count[1];
			unsigned int storage_task_size;
			char storage_task[1];
		};

		struct task_var_data
		{
			single_data_header header;
			unsigned int size;
			char data[1];
		};

		struct object_start_use
		{
			single_data_header header;
			int user;
			int item;
			unsigned short time;
		};
		
		struct object_cancel_use
		{
			single_data_header header;
			int user;
		};

		struct object_use_item
		{
			single_data_header header;
			int user;
			int item;
		};
		
		struct object_start_use_with_target
		{
			single_data_header header;
			int user;
			int target;
			int item;
			unsigned short time;
		};

		struct object_sit_down
		{
			single_data_header header;
			int id;
		//		A3DVECTOR pos; $$$$$$$$$$$$$
		};

		struct object_stand_up
		{
			single_data_header header;
			int id;
		};

		struct object_do_emote
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct server_timestamp
		{
			single_data_header header;
			int timestamp;
			int timezone;
			int lua_version;
		};

		struct notify_root
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
		};

		struct self_notify_root
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned char type;
		};

		struct dispel_root
		{
			single_data_header header;
			unsigned char type;
		};

		struct invader_rise
		{
			single_data_header header;
			int id;
		};

		struct pariah_rise
		{
			single_data_header header;
			int id;
			char pariah_state;
		};

		struct invader_fade
		{
			single_data_header header;
			int id;
		};

		struct self_stop_skill
		{
			single_data_header header;
		};

		struct update_visible_state
		{
			single_data_header header;
			int oid;
			unsigned int newstate;
			unsigned int newstate2;
			unsigned int newstate3;
			unsigned int newstate4;
			unsigned int newstate5;
			unsigned int newstate6;
			unsigned int newstate7;
			unsigned int newstate8;
			unsigned int newstate9;
		};

		struct x_timer
		{
			unsigned int time;
			int unc;
		};

		struct object_state_notify
		{
			single_data_header header;
			int oid;
			unsigned short scount;
			//unsigned short state[];
			unsigned short pcount;
			//int param[];
			unsigned short x_tcount;
			x_timer x_timers[];
		};

		struct player_gather_start
		{
			single_data_header header;
			int pid;		//player id
			int mid;		//mine id
			unsigned char use_time; //use time in sec;
		};

		struct player_gather_stop
		{
			single_data_header header;
			int pid;		//player id
		};

		struct trashbox_passwd_changed
		{
			single_data_header header;
			unsigned char has_passwd;
		};

		struct trashbox_passwd_state
		{
			single_data_header header;
			unsigned char has_passwd;
		};
		
		struct trashbox_open
		{
			single_data_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
			unsigned short trashbox_size;
			unsigned short trashbox2_size;
			unsigned short trashbox3_size;
		};

		struct trashbox_close
		{
			single_data_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
		};

		struct trashbox_wealth
		{
			single_data_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
			unsigned int money;
		};

		struct exchange_trashbox_item 
		{
			single_data_header header;
			char where;
			unsigned char idx1;
			unsigned char idx2;
		};
		struct move_trashbox_item
		{
			single_data_header header;
			char where;
			unsigned char src;
			unsigned char dest;
			unsigned int amount;
		};
		
		struct exchange_trashbox_inventory 
		{
			single_data_header header;
			char where;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};
		
		struct trash_item_to_inventory	 
		{
			single_data_header header;
			char where;
			unsigned char src;
			unsigned char dest;
			unsigned int amount;
		};
		
		struct inventory_item_to_trash  
		{
			single_data_header header;
			char where;
			unsigned char src;
			unsigned char dest;
			unsigned int amount;
		};
		
		struct exchange_trash_money
		{
			single_data_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
			int inv_delta;
			int tra_delta;
		};

		struct enchant_result
		{
			single_data_header header;
			int 	caster;
			int 	target;
			int 	skill;
			char 	level;
			char	orange_name;
			int 	at_state;
			unsigned char section;	// ���ܶ���
		};
		
		struct object_do_action
		{
			single_data_header header;
			int id;
			unsigned char emotion;
		};

		struct player_set_adv_data
		{
			single_data_header header;
			int id;
			int data1;
			int data2;
			
		};

		struct player_clr_adv_data 
		{
			single_data_header header;
			int id;
		};

		struct player_in_team
		{
			single_data_header header;
			int id;
			unsigned char state;	// 0 no team 1, leader, 2 member
		};

		struct team_apply_request
		{
			single_data_header header;
			int id;			//who
		};
		
		struct object_do_emote_restore
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct concurrent_emote_request
		{
			single_data_header header;
			int id;
			unsigned short emotion;
		};

		struct do_concurrent_emote 
		{
			single_data_header header;
			int id1;
			int id2;
			unsigned short emotion;
		};

		struct matter_pickup
		{
			single_data_header header;
			int matter_id;
			int who;
		};

		struct mafia_info_notify
		{
			single_data_header header;
			int pid;
			int mafia_id;
			char mafia_rank;
			int64_t mnfaction_id;
		};

		struct mafia_trade_start
		{
			single_data_header header;
		};

		struct mafia_trade_end
		{
			single_data_header header;
		};
		
		struct task_deliver_item
		{
			single_data_header header;
			int type;
			int expire_date;
			unsigned int amount;
			unsigned int slot_amount;
			unsigned char where;		//���ĸ���������0 ��׼��2 ����1 װ��
			unsigned char index;		//��󲿷ַ����ĸ�λ��
		};

		struct task_deliver_reputaion
		{
			single_data_header header;
			int delta;
			int cur_reputaion;
		};

		struct task_deliver_exp
		{
			single_data_header header;
			int exp;
			int sp;
		};

		struct task_deliver_money
		{
			single_data_header header;
			unsigned int amount;
			unsigned int cur_money;
			unsigned int silver;
		};

		struct task_deliver_level2
		{
			single_data_header header;
			int level2;
		};

		struct player_reputation
		{
			single_data_header header;
			int reputation;
		};

		struct identify_result
		{
			single_data_header header;
			char index;
			char result;	//0 	
		};

		struct player_change_shape
		{
			single_data_header header;
			int pid;
			char shape;
		};

		struct object_enter_sanctuary
		{
			single_data_header header;
			int id;		//self id or pet id
		};

		struct object_leave_sanctuary
		{
			single_data_header header;
			int id;		//self id or pet id
		};

		struct player_open_market
		{
			single_data_header header;
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];		//���62
		};

		struct self_open_market
		{
			single_data_header header;
			unsigned char count;
			unsigned char off;
			struct 
			{
				int type;		//��Ʒ����
				unsigned short index;	//�����0xFFFF����ʾ�ǹ���
				unsigned int count;	//�����ٸ�
				unsigned int price;		//����
			} item_list;
			
		};

		struct player_cancel_market
		{
			single_data_header header;
			int pid;
		};

		struct player_market_info
		{
			single_data_header header;
			int pid;
			int market_id;
			unsigned int count;
			market_goods item_list[];
		};

		struct player_market_trade_success
		{
			single_data_header header;
			int trader;
		};

		struct player_market_name
		{
			single_data_header header;
			int pid;
			unsigned char market_crc;
			unsigned char name_len;
			char name[];	//���62�ֽ�
		};

		struct player_start_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct self_start_travel
		{
			single_data_header header;
			float speed;
			A3DVECTOR dest;
			int line_no;
			unsigned char vehicle;
		};

		struct player_complete_travel
		{
			single_data_header header;
			int pid;
			unsigned char vehicle;
		};

		struct gm_toggle_invisible
		{
			single_data_header header;
			unsigned char is_visible;
		};

		struct gm_toggle_invincible
		{
			single_data_header header;
			unsigned char is_invincible;
		};

		struct self_trace_cur_pos
		{
			single_data_header header;
			A3DVECTOR pos;
			unsigned short seq;
		};

		struct object_cast_instant_skill
		{
			single_data_header header;
			int id;
			int target;
			int skill;
			unsigned char level;
		};

		struct activate_waypoint
		{
			single_data_header header;
			unsigned short waypoint;
		};

		struct player_waypoint_list
		{
			single_data_header header;
			unsigned int count;
			unsigned short list[];
		};
		
		struct unlock_inventory_slot
		{
			single_data_header header;
			unsigned char where;
			unsigned short index;
		};

		struct team_invite_timeout
		{
			single_data_header header;
			int who;
		};

		struct player_enable_pvp
		{
			single_data_header header;
			int who;
			char type;
		};

		struct player_disable_pvp
		{
			single_data_header header;
			int who;
			char type;
		};

		struct player_pvp_cooldown
		{
			single_data_header header;
			int cooldown_time;
			int max_cooldown_time;
		};

		struct cooldown_data
		{
			single_data_header header;
			unsigned short count;
			struct
			{
				unsigned short idx;
				int cooldown;
			}list[1];
		};

		struct skill_ability_notify
		{
			single_data_header header;
			int skill_id;
			int skill_ability;
		};

		struct personal_market_available
		{
			single_data_header header;
		};

		struct breath_data
		{
			single_data_header header;
			int breath;
			int breath_capacity;
		};

		struct player_stop_dive
		{
			single_data_header header;
		};

		struct trade_away_item
		{
			single_data_header header;
			short inv_index;
			int item_type;
			unsigned int item_count;
			int buyer;
		};

		struct player_enable_fashion_mode
		{
			single_data_header header;
			int who;
			unsigned char is_enable;
			unsigned char new_model; // 0 fashion weapon, 1 normal weapon
		};

		struct enable_free_pvp_mode
		{
			single_data_header header;
			unsigned char is_enable;
		};

		struct object_is_invalid
		{
			single_data_header header;
			int id;
		};

		struct player_enable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};

		struct player_disable_effect
		{
			single_data_header header;
			short effect;
			int id;
		};
		
		struct enable_resurrect_state
		{
			single_data_header header;
			float exp_reduce;
		};

		struct set_cooldown
		{
			single_data_header header;
			int cooldown_index; 
			int cooldown_time;
		};

		struct change_team_leader
		{
			single_data_header header;
			int old_leader; 
			int new_leader;
		};

		struct kickout_instance
		{
			single_data_header header;
			int instance_tag; 
			char reason;
			int timeout;		//�����-1��ʾȡ���˴��߳�
		};

		struct player_cosmetic_begin
		{
			single_data_header header;
			unsigned short index;
		};

		struct player_cosmetic_end
		{
			single_data_header header;
			unsigned short index;
		};

		struct cosmetic_success
		{
			single_data_header header;
			unsigned short crc;
			int id;
		};

		struct object_cast_pos_skill
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int target;
			int skill;
			unsigned short time;
			unsigned char level;
		};

		struct change_move_seq
		{
			single_data_header header;
			unsigned short seq;
		};

		struct server_config_data
		{
			single_data_header header;
			int world_tag;
			int region_time;
			int precinct_time;
			int mall_timestamp;
			int mall2_timestamp;
			int mall3_timestamp;
		};

		struct player_rush_mode
		{
			single_data_header header;
			char is_active;
		};

		struct trashbox_capacity_notify
		{
			single_data_header header;
			char where;
			int capacity;
		};

		struct npc_dead_2
		{
			single_data_header header;
			int nid;
			int attacker;
		};

		struct produce_null
		{
			single_data_header header;
			int recipe_id;
		};

		struct double_exp_time
		{
			single_data_header header;
			int mode;
			int end_time;		//����ʱ��
		};
		
		struct available_double_exp_time
		{
			single_data_header header;
			int available_time;	//ʣ��ʱ��
		};

		struct active_pvp_combat_state
		{
			single_data_header header;
			int who;
			char is_active;
		};

		struct duel_recv_request
		{
			single_data_header header;
			int player_id;
		};

		struct duel_reject_request
		{
			single_data_header header;
			int player_id;
			int reason;
		};

		struct duel_prepare
		{
			single_data_header header;
			int player_id;
			int delay;		//sec
		};
		
		struct duel_cancel
		{
			single_data_header header;
			int player_id;
		};

		struct duel_start
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_stop
		{
			single_data_header header;
			int player_id;
		};
		
		struct duel_result
		{
			single_data_header header;
			int id1;
			int id2;
			char result;	//1 id1 win 2 draw
		};

		struct player_bind_request
		{
			single_data_header header;
			int who;
		};

		struct player_bind_invite
		{
			single_data_header header;
			int who;
		};

		struct player_bind_request_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_invite_reply
		{
			single_data_header header;
			int who;
			int param;
		};

		struct player_bind_start
		{
			single_data_header header;
			int param;
			int mule;
			int rider;
			char animations[32];
		};

		struct player_bind_stop
		{
			single_data_header header;
			int who;
		};

		struct player_mounting
		{
			single_data_header header;
			int id;
			int mount_id;
			unsigned short mount_color;
		};

		struct player_equip_detail
		{
			single_data_header header;
			int id;
			int reserve;
			unsigned int content_length;
			char content[];
		};

		struct else_duel_start
		{
			single_data_header header;
			int player_id;
		};

		struct pariah_duration
		{
			single_data_header header;
			int time_left;
		};

		struct player_gain_pet
		{
			single_data_header header;
			int slot_index;
			char content[];	//pet_data
		};

		struct player_free_pet
		{
			single_data_header header;
			int slot_index;
			int pet_id;
		};

		struct player_summon_pet
		{
			single_data_header header;
			int slot_index;
			int pet_tid; 	//�����ʵ��id
			int pet_id;	//�����ڵ� ID 0 ������ id 
			int life_time;	//0���� >0����
		};


		struct player_recall_pet
		{
			single_data_header header;
			int slot_index;
			int pet_id;
			char reason;
		};

		struct player_start_pet_op
		{
			single_data_header header;
			int slot_index;
			int pet_id;
			int delay;	//�ӳ�ʱ�䣬��λ��50ms��tick
			int op;		//��������  0:�ų� 1:�ٻ� 2:����
		};

		struct player_stop_pet_op
		{
			single_data_header header;
		};

		struct player_pet_recv_exp
		{
			single_data_header header;
			int slot_index;
			int pet_id;
			int exp;
		};

		struct player_pet_levelup
		{
			single_data_header header;
			int slot_index;
			int pet_id;
			int level;		//�¼���
			int exp;		//��ǰ�ľ���ֵ 

		};

		struct player_pet_room
		{
			single_data_header header;
			unsigned short count;
			int slot_index;
			char pet_data[];
			/*
			//�ظ� count ��
			   int index;
			   char pet_data[];
			*/
		};

		struct player_pet_room_capacity
		{
			single_data_header header;
			unsigned int capacity;
		};
		
		struct player_pet_honor_point
		{
			single_data_header header;
			int index;
			int cur_honor_point;
		};

		struct player_pet_hunger_gauge
		{
			single_data_header header;
			int index;
			int cur_hunge_gauge;
		};

		struct enter_battleground
		{
			single_data_header header;
			int role_in_war;	//0 ����/����ս��  1 ���� 2 �ط�
			int battle_id;
			int end_time;
		};

		struct turret_leader_notify
		{
			single_data_header header;
			int turret_id;
			int leader_id;
		};

		struct battle_result
		{
			single_data_header header;
			int result;
			
		};

		struct battle_score
		{
			single_data_header header;
			int offense_score;
			int offense_goal;
			int defence_score;
			int defence_goal;
			
		};

		struct pet_dead
		{
			single_data_header header;
			unsigned int pet_index;
		};

		struct pet_revive
		{
			single_data_header header;
			unsigned int pet_index;
			float hp_factor;
		};

		struct pet_hp_notify
		{
			single_data_header header;
			unsigned int pet_index;
			float hp_factor;
			int cur_hp;
			float mp_factor;
			int cur_mp;
		};

		struct pet_ai_state
		{
			single_data_header header;
			char stay_state;
			char aggro_state;
		};

		struct refine_result
		{
			single_data_header header;
			int result;
		};

		struct pet_set_cooldown
		{
			single_data_header header;
			int pet_index;
			int cooldown_index;
			int cooldown_msec;
		};

		struct player_cash
		{
			single_data_header header;
			int cash;
		};

		struct player_bind_success
		{
			single_data_header header;
			unsigned short inv_index;
			int item_id;
		};

		struct player_change_inventory_size
		{
			single_data_header header;
			int new_size;
		};

		struct player_pvp_mode
		{
			single_data_header header;
			unsigned char pvp_mode;
		};

		struct player_wallow_info
		{
			single_data_header header;
			unsigned char anti_wallow_active;
			unsigned char wallow_level;
			int play_time;
			int light_timestamp;
			int heavy_timestamp;
			int reason;
		};
		
		struct player_use_item_with_arg
		{
			single_data_header header;
			unsigned char where;
			unsigned char index;
			int item_id;
			unsigned short use_count;
			unsigned short arg_size;
			char arg[];
		};

		struct object_use_item_with_arg
		{
			single_data_header header;
			int user;
			int item;
			unsigned short arg_size;
			char arg[];
		};

		struct player_change_spouse
		{
			single_data_header header;
			int who;
			int spouse_id;
		};

		struct notify_safe_lock
		{       
			single_data_header header;
			unsigned char active;
			int time;
			int max_time;                
			bool passwd;
		};
		
		//lgc
		struct elf_vigor
		{
			single_data_header header;
			int vigor;
			int max_vigor;
			int vigor_gen;//Ԫ���ָ��ٶȵ�100��				
		};

		struct elf_enhance
		{
			single_data_header header;
			short str_en;
			short agi_en;
			short vit_en;
			short eng_en;
		};

		struct elf_stamina
		{
			single_data_header header;	
			int stamina;	 
		};

		struct elf_cmd_result
		{
			single_data_header header;	
			int cmd;
			int result;
			int param1;
			int param2;
		};

		struct common_data_notify
		{
			single_data_header header;	
			/*
			struct 
			{
				int key;
				int value;
			} nodes[];
			*/
		};

		struct common_data_list
		{
			single_data_header header;	
			/*
			struct 
			{
				int key;
				int value;
			} nodes[];
			*/
		};

		struct player_elf_refine_activate
		{
			single_data_header header;
			int pid;
			char status;
		};
		
		struct player_cast_elf_skill
		{
			single_data_header header;
			int id;
			int target;
			int skill;
			unsigned char level;
		};

		struct mall_item_price
		{
			single_data_header header;
			short start_index;
			short end_index;
			short count;
			struct 
			{
				short good_index;
				char good_slot;
				int good_id;
				char expire_type;
				int expire_time;
				int good_price;
				char good_status;
				int min_vip_level;
			}list[];
		};

		struct mall_item_buy_failed
		{
			single_data_header header;
			short index;
			char reason;
		};

		struct player_elf_levelup
		{
			single_data_header header;
			int player_id;
		};
		
		struct player_property
		{
			single_data_header header;
			struct _data{
				int id;
				int hp;
				int mp;
				int damage_low;			//���damage
				int damage_high;		//�������damage
				int damage_magic_low;	//ħ����͹�����
				int damage_magic_high;	//ħ����߹�����
				int defense;			//������
				int resistance[5];		//ħ������
				int attack;				//������ attack rate
				int armor;				//�����ʣ�װ�׵ȼ���
				int attack_speed;		//����ʱ���� ��tickΪ��λ
				float run_speed;		//�����ٶ� ��λ  m/s
				int attack_degree;		//�����ȼ�
				int defend_degree;		//�����ȼ�
				int crit_rate;			//����
				int damage_reduce;		//�˺�����ٷֱȣ�������
				int prayspeed;			//�����ٶ����Ӱٷֱ�,������	
				int	crit_damage_bonus;	//����ı����˺��ӳ� 
				int invisible_degree;	//�����ȼ�
				int anti_invisible_degree;//�����ȼ�
				int vigour;				//����ֵ
				int anti_defense_degree;	// ��������
				int anti_resistance_degree;	// �����Է�
				int highest_level; // Nível de Pico //reserve1
			}data;
			struct _self_data
			{
				int damage_reduce;
				int prayspeed;
			}self_data;
		};
		
		struct player_cast_rune_skill
		{
			single_data_header header;
			int caster;
			int target;
			int skill;
			unsigned short time;
			unsigned char level;
		};
		
		struct player_cast_rune_instant_skill
		{
			single_data_header header;
			int id;
			int target;
			int skill;
			unsigned char level;
		};
		
		struct equip_trashbox_item
		{
			single_data_header header;
			unsigned char where;
			unsigned char trash_idx;	
			unsigned char equip_idx;
		};

		struct security_passwd_checked
		{
			single_data_header header;	
		};

		struct object_invisible
		{
			single_data_header header;
			int id;
			int invisible_degree;
		};

		struct hp_steal
		{
			single_data_header header;
			int hp;
		};
		
		struct player_dividend
		{
			single_data_header header;
			int dividend;
		};
		
		
		struct dividend_mall_item_price
		{
			single_data_header header;
			short start_index;
			short end_index;
			short count;
			struct 
			{
				short good_index;
				char good_slot;
				int good_id;
				char expire_type;
				int expire_time;
				int good_price;
				char good_status;
				int min_vip_level;
			}list[];
		};

		struct dividend_mall_item_buy_failed
		{
			single_data_header header;
			short index;
			char reason;
		};

		struct elf_exp
		{
			single_data_header header;	
			int exp;	 
		};
		
		struct public_quest_info
		{
			single_data_header header;
			//int space_id; //162
			int task_id;	
			//int child_space_id; //162
			int child_task_id;
			int score;
			int cls_place;
			int all_place;
		};

		struct public_quest_ranks
		{
			single_data_header header;
			int _task_id;		
			int player_count;
			unsigned int ranks_size[14];	//��������������ʦ�� ...�����顢�� ���а��С aumentar quando expandir nova classe
			struct{
				int roleid;
				int race;
				int score;
				int rand;
				int place;
			}ranks_entry[];
		};

		struct multi_exp_info
		{
			single_data_header header;
			int last_timestamp;
			float enhance_factor;
		};

		struct change_multi_exp_state
		{
			single_data_header header;
			char state;
			int enchance_time;
			int buffer_time;
			int impair_time;
			int activate_times_left;
		};

		struct world_life_time
		{
			single_data_header header;
			int life_time;	//>=0:ʣ��ʱ�� -1:���� 
		};

		struct wedding_book_list
		{
			single_data_header header;
			int count;
			struct{
				int start_time;
				int end_time;
				int groom;
				int bride;
				int scene;
				char status;
				char special;
			}list[];
		};

		struct wedding_book_success
		{
			single_data_header header;
			int type;	//1 Ԥ�� 2 ȡ��Ԥ��
		};

		struct calc_network_delay
		{
			single_data_header header;
			int	timestamp;
		};

		struct player_knockback
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			int time;		//����
			int fix_pos;
		};
		
		struct player_summon_plant_pet
		{
			single_data_header header;
			int plant_tid; 	//�����ʵ��id
			int plant_id;	//�����ڵ� ID 0 ������ id 
			int life_time;	//0���� >0����
		};
		
		struct plant_pet_disappear
		{
			single_data_header header;
			int plant_id;	//�����ڵ� ID 0 ������ id 
			char reason;	//0���� 1������ 2������Χ
		};
		
		struct plant_pet_hp_notify
		{
			single_data_header header;
			int plant_id;	//�����ڵ� ID 0 ������ id 
			float hp_factor;
			int cur_hp;
			float mp_factor;
			int cur_mp;
		};

		struct pet_property
		{
			single_data_header header;
			int pet_index;
		//	extend_prop prop;		//��������ͷ�ļ�������ע�͵���
			int r_def_lvl_f;
			int r_def_lvl_m;
			int r_chi_point;			
		};

		struct faction_contrib_notify
		{
			single_data_header header;
			int consume_contrib;
			int exp_contrib;
			int cumulate_contrib;
		};
		
		struct faction_fortress_info
		{
			single_data_header header;
			int factionid;	
			int level;
			int exp;			
			int exp_today;
			int exp_today_time;
			int tech_point;
			int technology[5];
			int material[8];
			int building_count;
			struct building_data
			{
				int id; 
				int finish_time;
			}building[1];
			int health;
		};
		
		struct enter_factionfortress
		{
			single_data_header header;
			int role_in_war;	//0 ����/����ս��  1 ���� 2 �ط�
			int factionid;
			int offense_endtime;
		};
		
		struct faction_relation_notify
		{
			single_data_header header;
			unsigned int alliance_size;
			int alliance[1];
			unsigned int hostile_size;
			int hostile[1];
		};
		
		struct player_equip_disabled
		{
			single_data_header header;
			int id;
			int64_t mask;
		};

		struct player_spec_item_list
		{
			single_data_header header;
			int roleid;
			int type;
			struct entry{
				unsigned char where;
				unsigned char index;
				unsigned int count;
			}list[];
		};

		struct object_start_play_action
		{
			single_data_header header;
			int id;
			int play_times;
			int action_last_time;
			int interval_time;
			unsigned int name_length;
			char action_name[];
		};

		struct object_stop_play_action
		{
			single_data_header header;
			int id;
		};

		struct congregate_request
		{
			single_data_header header;
			char type;
			int sponsor;
			int timeout;
		};

		struct reject_congregate
		{
			single_data_header header;
			char type;
			int id;
		};

		struct congregate_start
		{
			single_data_header header;
			char type;
			int id;
			int time;	//��λ����
		};

		struct cancel_congregate
		{
			single_data_header header;
			char type;
			int id;
		};

		struct engrave_start 
		{
			single_data_header header;
			int recipe_id;
			int use_time;
		};
		
		struct engrave_end
		{
			single_data_header header;
		};
		
		struct engrave_result
		{
			single_data_header header;
			int addon_num;
		};

		struct dps_dph_rank
		{
			single_data_header header;
			int next_refresh_sec;
			unsigned char rank_count;
			unsigned char rank_mask;
			struct _entry
			{
				int roleid;
				unsigned char level;
				int dps_or_dph;
			}entry_list[];
		};

		struct addonregen_start 
		{
			single_data_header header;
			int recipe_id;
			int use_time;
		};
		
		struct addonregen_end
		{
			single_data_header header;
		};
		
		struct addonregen_result
		{
			single_data_header header;
			int addon_num;
		};

		struct invisible_obj_list
		{
			single_data_header header;
			int id;
			unsigned int count;
			int id_list[];	
		};

		struct set_player_limit
		{
			single_data_header header;
			int index;
			char b;
		};

		struct player_teleport
		{
			single_data_header header;
			int id;
			A3DVECTOR pos;
			unsigned short use_time;
			char mode;
		};

		struct object_forbid_be_selected
		{
			single_data_header header;
			int id;
			char b;
		};

		struct player_inventory_detail
		{
			single_data_header header;
			int id;
			unsigned int money;
			unsigned char inv_size;
			unsigned int content_length;
			char content[];
		};

		struct player_force_data
		{
			single_data_header header;
			int cur_force_id;
			unsigned int count;
			struct{
				int force_id;
				int reputation;
				int contribution;
			}list[];
		};

		struct player_force_changed
		{
			single_data_header header;
			int id;
			int cur_force_id;
		};

		struct player_force_data_update
		{
			single_data_header header;
			int force_id;
			int reputation;
			int contribution;
		};
		
		struct force_global_data
		{
			single_data_header header;
			char data_ready;
			unsigned int count;
			struct{
				int force_id;
				int player_count;
				int development;
				int construction;
				int activity;
				int activity_level;
			}list[];
		};

		struct add_multiobj_effect
		{
			single_data_header header;
			int id;
			int target;
			char type;
		};
		
		struct remove_multiobj_effect
		{
			single_data_header header;
			int id;
			int target;
			char type;
		};

		struct enter_wedding_scene
		{
			single_data_header header;
			int groom;
			int bride;
		};

		struct produce4_item_info
		{
			single_data_header header;
			int stime;
			int type;
			int expire_date;
			int proc_type;
			unsigned int count;
			unsigned short crc;
			unsigned short content_length;
			char content[];
		};

		struct online_award_data 
		{
			single_data_header header;
			int total_award_time;
			unsigned int count;
			struct {
				int type;
				int time;
				int reserved;
			}list[];
		};

		struct toggle_online_award
		{
			single_data_header header;
			int type;
			char activate;
		};


		struct player_profit_time
		{
			enum
			{
				PLAYER_QUERY,	//��������
				PLAYER_ONLINE,	//�������
				PLAYER_SWITCH_SERVER,//�л���ͼ
				PROFIT_LEVEL_CHANGE,//���漶��仯
			};

			single_data_header header;
			char flag;		//��־��������ʱ���ԭ��
			char profit_map;//�Ƿ��������ͼ(0:��;1:�����ͼ;2:�����ͼ2)
			int profit_time;//ʣ������ʱ��
			int profit_level;//��ǰ���漶��
		};

		struct set_profittime_state
		{
			single_data_header header;
			char state;	//1:��ʼ;0:ֹͣ
		};

		struct enter_nonpenalty_pvp_state
		{
			single_data_header header;
			char state;
		};

		struct self_country_notify
		{
			single_data_header header;
			int country_id;
		};

		struct player_country_changed
		{
			single_data_header header;
			int id;
			int country_id;
		};
		
		struct enter_countrybattle
		{
			single_data_header header;
			int role_in_war;	//0 ����/����ս��  1 ���� 2 �ط�
			int battle_id;
			int end_time;
			int offense_country;
			int defence_country;
		};

		struct countrybattle_result
		{
			single_data_header header;
			int result;
		};

		struct countrybattle_score
		{
			single_data_header header;
			int offense_score;
			int offense_goal;
			int defence_score;
			int defence_goal;
		};

		struct countrybattle_resurrect_rest_times
		{
			single_data_header header;
			int times;
		};

		struct countrybattle_flag_carrier_notify
		{
			single_data_header header;
			int roleid;
			A3DVECTOR pos;
			char offense;
		};

		struct countrybattle_became_flag_carrier
		{
			single_data_header header;
			char is_carrier;
		};

		struct countrybattle_personal_score
		{
			single_data_header header;
			int combat_time;
			int attend_time;
			int kill_count;
			int death_count;
			int country_kill_count;
			int country_death_count;
		};

		struct countrybattle_flag_msg_notify
		{
			single_data_header header;
			int msg;
			char offense;
		};

		struct defense_rune_enabled
		{
			single_data_header header;
			char rune_type;
			char enable;
		};

		struct countrybattle_info
		{
			single_data_header header;
			int attacker_count;
			int defender_count;
		};
		
		struct cash_money_exchg_rate
		{
			single_data_header header;
			char open;
			int rate;
		};
		
		struct pet_rebuild_inherit_start
		{
			single_data_header header;
			unsigned int index;
			int use_time;
		};

		struct pet_rebuild_inherit_info
		{
			single_data_header header;
			int stime;
			int pet_id;
			unsigned int index;
			int r_attack;
			int r_defense;
			int r_hp;
			int r_atk_lvl;
			int r_def_lvl;
			//int r_pen_lvl_f;
			//int r_pen_lvl_m;
			//int r_chi_point;
		};
		
		struct pet_rebuild_inherit_end
		{
			single_data_header header;
			unsigned int index;
		};

		struct pet_evolution_done
		{
			single_data_header header;
			unsigned int index;
		};

		struct pet_rebuild_nature_start
		{
			single_data_header header;
			unsigned int index;
			int use_time;
		};
		
		struct pet_rebuild_nature_info
		{
			single_data_header header;
			int stime;
			int pet_id;
			unsigned int index;
			int nature;
		};

		struct pet_rebuild_nature_end
		{
			single_data_header header;
			unsigned int index;
		};

		struct equip_addon_update_notify
		{
			single_data_header header;
			unsigned char update_type; //0 for change 1 for replace
			unsigned char equip_idx;
			unsigned char equip_socket_idx;
			int old_stone_type;
			int new_stone_type;
		};
		
		struct notify_meridian_data
		{
			single_data_header header;
			int meridian_level;
			int lifegate_times;
			int deathgate_times;
			int free_refine_times;
			int paid_refine_times;
			int continu_login_days;
			int trigrams_map[3];
		};

		struct try_refine_meridian_result
		{
			single_data_header header;
			int index;
			int result;
		};

		struct self_king_notify
		{
			single_data_header header;
			char is_king;
			int	expire_time;
		};

		struct player_king_changed
		{
			single_data_header header;
			int id;
			char is_king;
		};

		struct countrybattle_stronghold_state_notify
		{
			single_data_header header;
			int count;
			int state[];
		};
		
		struct query_touch_point 
		{ 
			single_data_header header; 
			int64_t income;   // ��������
			int64_t remain;   // ���õ���
			int retcode;
		};

		struct cost_touch_point 
		{  
			single_data_header header; 
			int64_t income; // ��������
			int64_t remain; // ���õ���
			unsigned int costval; // ���λ�����
			unsigned int index; // ��Ʒ����
			unsigned int lots; // ������
			int retcode; // 0 �ɹ�
		};

		struct query_addup_money 
		{ 
			single_data_header header; 
			int64_t addupmoney;   // ��������
		};

		struct query_title
		{
			single_data_header header;
			int roleid;
			int titlescount;  // �ƺŸ���
			int expirecount;
			//unsigned short titles[];   // �ѻ�óƺ�vec
			char data[];    //  pair<unsigned short, int>  vec
		};

		struct change_title
		{
			single_data_header header;
			int roleid;
			unsigned short titleid;
		};

		struct notify_title_modify
		{
			single_data_header header;
			unsigned short titleid;
			int expiretime; //0 ��������
			char flag;  	//0 rmv | 1 add
		};

		struct refresh_signin
		{
			single_data_header header;
			char type;			// ͬ��ԭ�� 0��ʼ1���ݲ�ͬ��2ǩ��3��ǩ4�콱
			int lastyearstate; 	// ȥ���·�ǩ��״̬
			int monthcalenadr; 	// ��ǰ��ǩ������
			int updatetime;  	// ǩ���������һ�α��ʱ��
			char latesignintimes;   // ��ǰ���Ѳ�ǩ����
			char awardedtimes;      //��ǰ�����콱����
			char awardedtimes2;      //��ǰ�����콱����
			char newstate;

        };

		struct parallel_world_info
		{
			single_data_header header;
			int worldtag;
			int count;
			struct{
				int key;
				float load;
			}list[];
		};

		struct unique_data_notify 
		{ 
			single_data_header header;
			int count;
			struct{
				int key;
		 		int type; //ȫ���������� |0 δ��ʼ��|1 INT| 2 DOUBLE| 3 �䳤
				unsigned int size;
				char data[1];
			}elems[]; 
		 };

		struct notify_giftcard_redeem
		{
			single_data_header header;
			int  retcode;
			int  cardtype;
			int  parenttype;
			char cardnumber[20];
		};

		struct player_reincarnation
		{
			single_data_header header;
			int id;
			int reincarnation_times;
		};

		struct reincarnation_tome_info
		{
			single_data_header header;
			int tome_exp;
			char tome_active;
			int count;
			struct
			{
				int level;
				int timestamp;
				int exp;
			}records[];
		};

		struct activate_reincarnation_tome
		{
			single_data_header header;
			char active;	
		};
		
		struct realm_exp
		{
			single_data_header header;
			int exp;
			int receive_exp;
			int diary_exp;
		};
		
		struct realm_level_up
		{
			single_data_header header;
			int roleid;
			unsigned char level;
		};

		struct enter_trickbattle
		{
			single_data_header header;
			int role_in_war;	//0 ����/����ս��  1 ���� 2 �ط�
			int battle_id;
			int end_time;
		};

		struct trickbattle_personal_score
		{
			single_data_header header;
			int kill;
			int death;
			int score;
			int multi_kill;
		};

		struct trickbattle_chariot_info
		{
			single_data_header header;
			int chariot;
			int energy;
		};

		struct player_leadership
		{
			single_data_header header;
			int leadership;
			int inc_leadership;
		};

		struct generalcard_collection_data
		{
			single_data_header header;
			unsigned int size;
			char buf[];
		};

		struct add_generalcard_collection
		{
			single_data_header header;
			unsigned int colloction_idx;
		};

		struct refresh_fatering
		{
			single_data_header header;
			int gain_times;
			unsigned int count;
			struct
			{
				int level;
				int power;
			}fatering[];
		};

		struct mine_gatherd
		{
			single_data_header header;
			int mid;	//mine id
			int pid;	//player id
			int item_type;
		};

		struct player_active_combat
		{
			single_data_header header;
			int who;
			bool is_active;
		};

		struct player_query_chariots
		{
			single_data_header header;
			unsigned int attacker_count;
			unsigned int defender_count;
			struct
			{
				int type;
				int count;
			}chariots[];
		};

		struct countrybattle_live_result
		{
			single_data_header header;
			struct score_rank_entry
			{
				int roleid;
				int rank_val;
				A3DVECTOR pos;
			};
			struct death_entry
			{
				int roleid;
				int timestamp;
				A3DVECTOR pos;
			};

			unsigned int defence_rank_count;
			//score_rank_entry defence_ranks[];
			unsigned int defence_death_count;
			//death_entry defence_death_list[];
			unsigned int offense_rank_count;
			//score_rank_entry offense_ranks[];
			unsigned int offense_death_count;
			death_entry offense_death_list[];
		};

		struct random_mall_shopping_result 
		{ 
			single_data_header header; 
			int entryid;     // 
			int opt;
			int result;		 // 0 �ɹ� -1 entryid���� -2 ״̬���� -3 ��ȷ��״̬���� -4Ԫ������ -5��������
			int itemid;    // ��������Ʒid
			int price;  	 // ������۸�
			bool firstflag;  // �״ι����ʶ 
		};

		struct player_mafia_pvp_mask_notify
		{
			single_data_header header;
			int roleid;
			unsigned char mafia_pvp_mask; // 1 �󳵿ɹ��� 2 ���ؿɹ���
		};

		struct player_world_contribution
		{
			single_data_header header;
			int points_total;
			int points_rest;
			char reward_stage;
			char count;
			char values[];
		};

		struct random_map_order
		{
			single_data_header header;
			int world_tag;
			int row;
			int col;
			int room_src[];     // count = row * col
		};

		struct scene_service_npc_list
		{
			single_data_header header;
			unsigned int count;
			struct{
				int tid;
				int id;
			}list[];
		};

		struct npc_visible_tid_notify
		{
			single_data_header header;
			int nid;			//npc id
			int vistid;			//visible template id
		};

		struct client_screen_effect
		{
			single_data_header header;
			int type;			//effect type   0: screen 1: gfx
			int eid;			//effect id
			int state;			//state			0: close 1: open
		};
        
        struct equip_can_inherit_addons
        {
            single_data_header header;
            int equip_id;
            unsigned char inv_idx;
            unsigned char addon_num;
            int addon_id_list[];
        };

		struct combo_skill_prepare
		{
			single_data_header header;
			int skillid;
			int timestamp;
			int args[3];
		};

		struct instance_reenter_notify
		{
			single_data_header header;
			int instance_tag;    // ����ĸ���id
			int time_out;		 // ���뵹��ʱ��ֹ(������)ʱ��
		};

		struct pray_distance_change
		{
			single_data_header header;
			float pray_distance_plus;
		};


		struct astrolabe_info_notify
		{
			single_data_header header;
			unsigned char level;
			int exp;
		};

		struct astrolabe_operate_result
		{
			single_data_header header;
			int opttype;
			int retcode;
			int args[3];
		};

        struct property_score_result
        {
            single_data_header header;
            int fighting_score;
            int viability_score;
            int client_data;
        };

        struct lookup_enemy_result
        {
            single_data_header header;
            int rid;
            int worldtag;
            A3DVECTOR pos;
        };

		struct solo_challenge_award_info_notify
		{
			single_data_header header;
			int max_layer_climbed;
			int total_first_climbing_time;
			int total_score_earned;
			int cur_score;

			struct{
				int climbed_layer;
				int climbing_time;
				int total_draw_item_times;
				int drawn_item_times;
				struct 
				{
					int item_id;
					int item_count;
				}drawn_items[];
			}layer_climbed_award;
		};

		struct solo_challenge_challenging_state_notify
		{
			single_data_header header;
			int climbed_layer;
			unsigned char notify_type;
		};

		struct solo_challenge_operate_result
		{
			single_data_header header;
			int opttype;
			int retcode;
			int args[3];
		};

		struct solo_challenge_buff_info_notify
		{
			single_data_header header;
			int buff_num;
			int cur_score;
			struct
			{
				int filter_index;
				int filter_layer;
			}buff_info[];
		};
	
		struct mnfaction_player_faction_info
		{
			single_data_header header;
			int player_faction;
			int domain_id;
		};
		
		struct mnfaction_resource_point_info
		{
			single_data_header header;
			int attacker_resource_point;
			int defender_resource_point;
		};

		struct mnfaction_player_count_info
		{
			single_data_header header;
			int attend_attacker_player_count;
			int attend_defender_player_count;
		};

		struct mnfaction_result
		{
			single_data_header header;
			int result;
		};

		struct mnfaction_resource_tower_state_info
		{
			single_data_header header;
			int num;
			struct
			{
				int index;
				int own_faction;
				int state;
				int time_out;
			}state_info[];
		};

		struct mnfaction_switch_tower_state_info
		{
			single_data_header header;
			int num;
			struct
			{
				int index;
				int own_faction;
				int state;
				int time_out;
			}state_info[];
		};

		struct mnfaction_transmit_pos_state_info
		{
			single_data_header header;
			int num;
			struct
			{
				int index;
				int own_faction;
				int state;
				int time_out;
			}state_info[];
		};

		struct mnfaction_resource_point_state_info
		{
			single_data_header header; 
			int index;
			int cur_degree;
		};

		struct mnfaction_battle_ground_have_start_time
		{
			single_data_header header;
			int battle_ground_have_start_time;
		};

		struct mnfaction_faction_killed_player_num
		{
			single_data_header header;
			int attacker_killed_player_count;
			int defender_killed_player_count;
		};

		struct mnfaction_shout_at_the_client
		{
			single_data_header header;
			int type;
			int args;
		};

		struct mnfaction_player_pos_info
		{
			single_data_header header;
			int num;
			struct
			{
				int roleid;
				float player_pos[3];
			}player_pos_info[];
		};
		
		struct fix_position_transmit_add_position
		{
			single_data_header header;
			int     index;
			int     world_tag;
			float   pos[3];
			char    position_name[32];
		};

		struct fix_position_transmit_delete_position
		{
			single_data_header header;
			int index;
		};

		struct fix_position_transmit_rename
		{
			single_data_header header;
			int		index;
			char    position_name[32];
		};

		struct fix_position_energy_info
		{
			single_data_header header;
			char is_login;
			int cur_energy;
		};

		struct fix_position_all_info
		{
			single_data_header header;
			int count;
			struct
			{
				int     index;
				int     world_tag;
				float   pos[3];
				char    position_name[32];
			}position_info[];
		};
		
		struct cash_vip_mall_item_price
		{
			single_data_header header;
			short start_index;
			short end_index;
			short count;
			struct 
			{
				short good_index;
				char good_slot;
				int good_id;
				char expire_type;
				int expire_time;
				int good_price;
				char good_status;
				int min_vip_level;
			}list[];
		};

		struct cash_vip_mall_item_buy_result
		{
			single_data_header header;
			char result;
			short index;
			char reason;
		};

		struct cash_vip_info_notify
		{
			single_data_header header;
			int level;
			int score;
		};

		struct purchase_limit_info_notify
		{
			single_data_header header;
			int count;
			char reserve;
			struct
			{
				int limit_type;
				int item_id;
				int have_purchase_count;
			}item_info[];
		};

        struct cash_resurrect_info
        {
            single_data_header header;
            int cash_need;
            int cash_left;
        };

        struct home_task_info
        {
            single_data_header header;
            int size;
            //char buf[];
			int task_trigger_times;
			int task_refresh_times;
        };

        struct body_scale_notify
        {
            single_data_header header;
			int mode; // == 2(old155) // == 4(new 162)
            int id;
            float scale;
        };

        struct filter_soul_info
        {
            single_data_header header;
            int id;
            int soul;
        };
		
		
// -- cgame recv new 155 - 162 -- //

		struct cgame_recv_430
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 0; 
		};
	
		struct cgame_recv_431
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_432
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12;  
		};

		struct get_lotery_items
		{
			struct ITEM
			{
				unsigned int id;
				unsigned int count;
			};
			
			unsigned int count;
			ITEM item[];
		};

		struct get_treasure_info
		{
			struct LOT
			{
				unsigned int id; // Ид итема
				unsigned int count; //количество
				char lot; // номер лота
			};
			
			struct BOX
			{
				unsigned int id; //ид итема
				unsigned int box; // // номер сундучка слева
				unsigned int count; //количество
				char lot; // номер лота
			};
			
			single_data_header header;
			unsigned char manager; //менеджер ( 0 просто информация , 1 поиск сокровищь начался , 2 - сокровище открыто
			unsigned int score; //очки 
			unsigned int free_count; //колличество бесплатных попыток
			unsigned int lot_count; //количество закрытых сундучков
			unsigned int box_count; //колличество открытых сундучков
			LOT lot[];
			//BOX box[];
		};

		struct cgame_recv_435
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 0;  
		};
	
		struct player_world_speak_info
		{
			struct SKILL
			{
				unsigned int id;
				int level;				
			};

			single_data_header header;
			char enabled; // 0
			char desabled; // 1
			int count_skills;
			SKILL skill[];

		};

		struct cgame_recv_437
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 68;  
		};

		struct player_killed_by_player
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;
			int roleid;
		};

		struct get_lib_items
		{
			struct ITEM
			{
				unsigned int id;
				unsigned int count;
			};

			single_data_header header;
			unsigned int count;
			ITEM item[];
		};

		struct cgame_recv_440
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};
	
		struct arena_team_invite 
		{
			single_data_header header;
			unsigned int inviting_id;
			unsigned int team_type; // 1 - 3x3 , 3 - 6x6
		};

		struct arena_player_faction_info
		{
			single_data_header header;
			unsigned int team_id;
			int type; //0 - 1
			unsigned int time_rest; // Remaining time in seconds (heartbeat --)
		};

		struct arena_battle_group_info
		{
			single_data_header header;
			unsigned int time_rest; // Remaining time in seconds (heartbeat --)
			int red_team_count_rest; // Number of live players (red)
			int blue_team_count_rest; // Number of live players (blue)
			int reserve; //0
			int team_count_members; // Number of players in the group
			int roleid_members[]; // List of players in the group (My Team)
		};
		
		struct glyph_slot_info
		{
			single_data_header header;
			int old_sec_level;
			int count;
			int pvp_point;
		};

		struct glyph_data
		{
			struct GLYPH
			{
				int pos;
				int skill;
				int item;
			};
			
			single_data_header header;
			int count;
			GLYPH glyph[];
		};

		struct arena_battle_result
		{
			struct PLAYER
			{
				int cls;
				int point_solo;
				int wins;
				int loses;
				int point_team;
				char name[40];			
			};

			single_data_header header;
			int team_win; //1 - 2
			
			int red_damage;
			int blue_damage;
			
			int red_old_score;
			int blue_old_score;
			int red_new_score;
			int blue_new_score;

			int reserve3; //11
			
			int type; //1 - 1 Win, 2 = draw
			int red_count_players;
			int blue_count_players;
			int red_count_alive_players;
			int blue_count_alive_players;			
			
			char red_team_name[40];
			char blue_team_name[40];

			//PLAYER red_players[];
			//PLAYER blue_players[];
		};

/*
		struct arena_battle_result
		{
			struct TEAM_INFO
			{
				int cls;
				int point_solo;
				int wins;
				int loses;
				int point_team;
				char name[40];			
			};

			single_data_header header;
			unsigned int team_win; //1 - 2
			int total_damage_team[2];
			int old_point[2];
			int new_point[2];
			int reserve3; //11
			int type; //1 - 1 Win, 2 = draw
			int team_count[2]; //3 (Count Players)
			int team_count_rest[2]; //0 (Count Alives)

			char team_name[2][40];

			TEAM_INFO team_info[2][12];
		};
*/


		struct arena_battle_group_prop
		{
			struct TEAM_PROP
			{
				unsigned int roleid;
				unsigned short level;
				unsigned char reserved; //0
				unsigned short cls;
				unsigned int hp;
				unsigned int max_hp;
				unsigned int mp;
				unsigned int max_mp;
			};			

			single_data_header header;
			TEAM_PROP member;
			unsigned int leader_id; //-1
		};

		struct arena_battle_object_state_notify
		{
			single_data_header header;
			int oid;
			unsigned short scount;
			//unsigned short state[];
			unsigned short pcount;
			//int param[];
			unsigned short x_tcount;
			x_timer x_timers[];
		};

		struct cgame_recv_449
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4 + buf[];  
		};

		struct player_carrier_up
		{
			single_data_header header;
			int pos;
			unsigned int reserve1; // 32562755
			unsigned int npcid;
		};

		struct player_carrier_self_up
		{						
			single_data_header header;
			int pos; //0
			int count; //3
			unsigned int npcid;
			int res; //1
			float speed_walk; //2
			float speed_run; //2
			float speed_swim; //4
			float speed_fly; //10
		};

		struct player_carrier_down
		{		
			single_data_header header;
			int pos;
			unsigned int npcid;
		};

		struct cgame_recv_453
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 24;  
		};

		struct glyph_manager_re
		{
			single_data_header header;
			int mode;  // 0 inc , 1 dec , 2 craft , 3 get info
			int result; // create rune success
			int pos; // skill pos , item pos
			int id1; // skill id , next_id & pre_id
			int id2; // inc item id , update item id
		};

		// Novas cargas do mercenário
		struct get_charge_merc
		{
			single_data_header header;
			char charges; //0~5
		};
	
		struct carrier_sync_info
		{
			single_data_header header;
			unsigned int npcid;
			int reserve1; //-1
			int reserve2; //2
		};

		struct cgame_recv_457
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16;  
		};

		struct cgame_recv_458
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16;  
		};

		// Novo Range Glyph
		struct skill_glyph_info
		{
			struct INFO
			{
				int feature; // 1,2,3,4,5 (feature skill)
				unsigned int value_info; //1900
			};
			single_data_header header;
			int type; // 1 
			int count; // 1
			INFO pray[];

		};

		struct carrier_skill_cooldown
		{
			single_data_header header;
			unsigned int skill_id; //skill+1024
			int cooldown; // 0
		};
	
		struct carrier_prop
		{
			single_data_header header;
			unsigned int npcid;
			int reserve2; //2
			unsigned int carrier_data; //1655689362
		};

		struct cgame_recv_462
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};

		struct cgame_recv_463
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 136;  
		};

		struct cgame_recv_464
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};

		struct cgame_recv_465
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};
	
		struct cgame_recv_466
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_467
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 3;  
		};

		struct cgame_recv_468
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 6 + buf[];  
		};

		struct cgame_recv_469
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 6 + buf[];  
		};

		struct cgame_recv_470
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 24 + buf[];  
		};
	
		struct cgame_recv_471
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 20 + buf[];  
		};

		struct cgame_recv_472
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};

		struct cgame_recv_473
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8 + buf[];  
		};

		struct cgame_recv_474
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 10 + buf[];  
		};

		struct cgame_recv_475
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12;  
		};
	
		struct cgame_recv_476
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_477
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_478
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 9 + buf[];  
		};

		struct cgame_recv_479
		{
			single_data_header header;
			//size = sizeof(single_data_header) + ;  
		};

		struct get_imperial_spirit
		{
			single_data_header header;
			int enabled; // 0 disabled | 1 enabled
			unsigned int expired_time; // 0
		};
	
		struct cgame_recv_481
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 1;  
		};

		struct cgame_recv_482
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 2;  
		};

		struct cgame_recv_483
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12 + buf[];  
		};

		struct cgame_recv_484
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 1;  
		};

		struct cgame_recv_485
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12 + buf[];  
		};
	
		struct cgame_recv_486
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12;  
		};

		struct get_storage_points
		{
			single_data_header header;
			unsigned short points_1;
			unsigned short points_2;
		};

		struct get_finish_notify
		{
			single_data_header header;
			int res; //1
			int res2; //120000
		};

		struct refine_bible_result
		{
			single_data_header header;
			int result;
		};

		struct transfer_refine_bible
		{
			single_data_header header;
			int rst1;
			int rst2;
			int item_pos;
		};
	
		struct cgame_recv_491
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 88;  
		};

		struct cgame_recv_492
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12;  
		};

		struct cgame_recv_493
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 36 + buf[];  
		};

		struct cgame_recv_494
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_495
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16 + buf[];  
		};
	
		struct cgame_recv_496
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};

		struct engrave_new_info
		{
			struct ENGRAVES
			{
				int addon_id;
				int addon_value;
				int reserve1;
				int reserve2;
			};

			single_data_header header;
			int addon_count;
			ENGRAVES engraves[];			
		};

		struct cgame_recv_498
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_499
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4;  
		};

		struct cgame_recv_500
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 6;  
		};
	
		struct cgame_recv_501
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8;  
		};

		struct cgame_recv_502
		{
			single_data_header header;
			//size = sizeof(single_data_header) +8 ;  
		};

		struct get_anecdote_info
		{
			single_data_header header;
			unsigned short uTaskStorage[16];
		};

		struct cgame_recv_504
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4 + buf[];  
		};

		struct cgame_recv_505
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16 + buf[];  
		};
	
		struct skill_cooldown_limit
		{
			single_data_header header;
			int index; // skill id + 1024
			int last_use_time; // unix time
			int cooling_time_sec; // second 15
			unsigned short max_count; // max count skill cooldown
			unsigned short count; // count use skill cooldown
		};

		struct player_repository_info
		{
			single_data_header header;
			unsigned int roleid;
			int	itens[];
		};

		struct player_repository_storage
		{
			single_data_header header;
			int active;
			unsigned int count_itens;
			int	itens[];
		};

		struct cgame_recv_509
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16;  
		};

		
		struct armor_info_purification
		{
			struct ADDONS
			{
				int addon_id;
				int addon_arg0;		
				int addon_arg1;	
				int addon_arg2;	
			};

			single_data_header header;
			int addon_count;
			ADDONS addons[];			
		};
	
		struct armor_info_notify
		{
			single_data_header header;

			unsigned int reserved; //0 awakening || 3 extract || 4 decompose || 1 devour crystal || 2 inc || 5 dec
			unsigned int count; //3 awakening || 0 decompose || 0 devour crystal || 1 inc || 0 dec

			unsigned int reserved3; //0 awakening || 1 pos
			unsigned int reserved4; //0 awakening ||

			unsigned int inv_slot; //90 awakening ||

		};

		struct get_weapon_vstate
        {
            single_data_header header;
            unsigned int roleid;
            unsigned char state;
        };

		struct get_shield_energy
		{
			single_data_header header;
			short shgen; 
			short shmax;
		};

		struct cgame_recv_514
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16;  
		};

		struct cgame_recv_515
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 20;  
		};
	
		struct cgame_recv_516
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12;  
		};

		struct cgame_recv_517
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 20;  
		};

		struct cgame_recv_518
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 0;  
		};

// -- cgame recv new 162 - 172 -- //

		struct cgame_recv_520
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 0; 
		};

		struct cgame_recv_521
		{
			single_data_header header;
		};

		struct get_server_time
		{
			single_data_header header;
			unsigned int time;
		};

		struct get_lotery_info
		{
			struct ITEM
			{
				unsigned int id; // ���� �ڧ�֧ާ�
				unsigned int rank_item; //open 0 - ��֧���� , 1 - ��ڧߧڧ� , 2 ��ڧ�� , 3 �ԧ�ݧ� , 4 ���ѧߧ� , 5 ��֧� 
				unsigned int rank_lot; //close 0 - ��֧���� , 1 - ��ڧߧڧ�
				unsigned int is_open; // 0 �٧ѧܧ������� , 1 ���ܧ������� 
			};

			single_data_header header;
			unsigned int manager; //2 �ߧ�ާ֧� �ާ֧ߧ֧էا֧�� 1 - ���ߧէ�ܧ�, 2 �ܧѧ���
			unsigned int score; //�ܧ�ݧݧڧ�֧��ӧ� ���ܧ��
			unsigned int count; // �ܧ�ݧڧ�֧��ӧ� �ڧ�֧ާ��
			ITEM item[];
		};

		struct cgame_recv_524
		{
			single_data_header header;
		};

		struct cgame_recv_525
		{
			single_data_header header;
		};

		struct cgame_recv_526
		{
			single_data_header header;
		};

		struct cgame_recv_527
		{
			single_data_header header;
		};

		struct cgame_recv_528
		{
			single_data_header header;
		};

		struct cgame_recv_529
		{
			single_data_header header;
		};

		struct celestial_memorial_info
		{
			
			single_data_header header;
			bool type; //0 ~ 1 (174+)

			unsigned int level; //1 ~ 500+
			int exp; //0 

			int perg_03; //0 ~ 2 (Legend)
			int perg_02; //0 ~ 1
			int perg_01; //0 ~ 1 (Basic)
		
			unsigned int level_max; //500
			int unk7; // Item unk (não utilizado)
			int unk8; // Item unk (não utilizado)

			unsigned int level_max_2; //500 (174+)

			unsigned int count_level_2; // (174+)
			unsigned int count_level_1;
			int pos_level_2[500]; // (174+)
			int pos_level_1[500];

		};

		struct celestial_memorial_lottery
		{
			struct LOTTERY
			{
				int lottery_draw_lvl;

				int item_id_1;	
				int item_id_2;
				int item_count_1;
				int item_count_2;

				int storage_rest_cnt_0;
				int storage_rest_cnt_1;
				int storage_rest_cnt_2;
				int storage_rest_cnt_3;
				int storage_rest_cnt_4;
				
				char storage_reward_1;				
				char storage_reward_2;				
				char storage_reward_3;				
				char storage_reward_4;				
				char storage_reward_5;				
				char storage_reward_6;				
				char storage_reward_7;				
				char storage_reward_8;				
			};

			single_data_header header;
			unsigned int count; //1 ~ 3 (174+)
			LOTTERY lottery[];	
		};

		struct cgame_recv_532
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12; 	
		};

		struct cgame_recv_533
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 18; 	
		};

		struct cgame_recv_534
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16;
		};

		struct cgame_recv_535
		{
			single_data_header header;			
			//size = sizeof(single_data_header) + 78;
		};

		struct cgame_recv_536 // Reino do Norte (Muda o nick)
		{
			single_data_header header;
			unsigned char color; // 0 = normal, 1 = vermelho, 2 = azul
		};

		struct cgame_recv_537 // Reino do Norte (Brasão no meio da tela)
		{
			single_data_header header;
			unsigned short reserve;
		};

		struct cgame_recv_538
		{
			single_data_header header;
			//size = sizeof(single_data_header) + ??; 	
		};

		struct pet_skill_data
		{
			struct SKILLS
			{
				unsigned int skill_id;
				unsigned int skill_level;
			};
			
			single_data_header header;
			unsigned int attack_basic_lvl; //0
			SKILLS skill[];		
		};

		struct pet_master_prop
		{
			single_data_header header;	
			//char pet_data[];
			//extend_prop prop;		
		};

		struct pet_skin_info
		{
			single_data_header header;
			unsigned int index;
			unsigned int skin_pet_id;
		};
		
		struct pet_skin_storage
		{
			struct SKIN
			{
				unsigned int skin_pet_id;
				unsigned int index;
			};
			
			single_data_header header;
			unsigned int count;
			SKIN skin[];
		};

		struct cgame_recv_543 // fashion_point_storage
		{
			single_data_header header;

			int res;
			int res2;
			int fashion_points;

			//size = sizeof(single_data_header) + 12; 	
		};

		struct cgame_recv_544 // ??
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12; 	
		};

		struct codex_fashion_dye
		{
			struct FASHION
			{
				int fashion_id;
				int fashion_color_pos;
			};

			single_data_header header;

			int mode; // 0
			unsigned int count_fashions;
			FASHION fashions[];
		};

		struct codex_fashion_activate
		{
			struct FASHION
			{
				int fashion_id;
				int fashion_color;
			};

			single_data_header header;
			unsigned int roleid;
			int mode_classic;
			FASHION fashions[];
		};

		struct codex_reward_first
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 12; 	

			int type; //0~4			
			int reward_idx; //6734
			int res; //1
		};

		struct cgame_recv_548 // ? Report bug
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 13; 	
		};

		struct codex_fly_consum_info
		{
			single_data_header header;

			unsigned int fly_id;
			char res;
			int max_speed;			
			//int fly_points; /*176+*/
		};

		struct codex_improve_info
		{
			single_data_header header;
			int fly_id;
			char apprimored_level;
			int max_time_speed;
		};

		struct cgame_recv_550
		{
			single_data_header header;
			//size = sizeof(single_data_header) + ??; 	
		};

		struct codex_fly_equip_info
		{
			single_data_header header;
			short index_equip;
			char index_inv;
			char where_equip;
			int pos_codex_inv;
			int fly_item_id;
			int speed;
			char apprimored_level;
		};

		struct codex_pet_consum_info
		{
			single_data_header header;

			unsigned int pet_egg_id;
			int res;
			short res2;			
			//unsigned int mount_points; /*176+*/
		};

		struct cgame_recv_553
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8; 			
		};

		struct codex_rename_pet_info
		{
			single_data_header header;
			unsigned int pet_id;
			short name_len; //0
			char name[]; //0	
		};

		struct codex_reward_title
		{
			single_data_header header;
			int type; //0~4
			int max_pos; //1
		};

		struct codex_fashion_storage
		{
			struct SKIN_PART
			{
				int fashion_id;
				int color;
				int fashion_pos;
			};
			
			single_data_header header;
			unsigned int skins_count;
			SKIN_PART skins[];
		};

		struct cgame_recv_557 //Campo de batalha?
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8; 	
		};

		struct reward_interface_notify
		{
			single_data_header header;
			int mode; //1
			int value_index; //21
			int count;
		};

		struct cgame_recv_559
		{
			single_data_header header; //??
			
			int res; //4
			int res2; //0		
			int res3; //0
		};

		struct cgame_recv_560 // ??
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 8; 	
		};

		struct cgame_recv_561
		{
			single_data_header header; //??

			int res;
			int res2;
			int res3;
			int res4;
		};

		struct cgame_recv_562 // Colápso temporal
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16; 	
		};

		struct cgame_recv_563
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 36; 	
		};

		struct cgame_recv_564
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16; 	
		};

		struct cgame_recv_565
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 16; 	
		};

		struct cgame_recv_566
		{
			single_data_header header;
			//size = sizeof(single_data_header) + ??; 	
		};

		struct cgame_recv_567
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 24; 	
		};

		struct cgame_recv_568
		{
			single_data_header header;
		};

		struct cgame_recv_569
		{
			single_data_header header;
		};

		struct cgame_recv_570
		{
			single_data_header header;
		};

		struct cgame_recv_571
		{
			single_data_header header;
		};

		struct cgame_recv_572
		{
			single_data_header header;
		};

		struct cgame_recv_573
		{
			single_data_header header;
		};

		struct cgame_recv_574
		{
			single_data_header header;
		};

		struct cgame_recv_575
		{
			single_data_header header;
		};

		struct cgame_recv_576
		{
			single_data_header header;
		};

		struct cgame_recv_577
		{
			single_data_header header;
		};

		struct cgame_recv_578
		{
			single_data_header header;
		};

		struct cgame_recv_579
		{
			single_data_header header;
		};

		struct cgame_recv_580
		{
			single_data_header header;
		};

		struct cgame_recv_581
		{
			single_data_header header;
		};

		struct cgame_recv_582
		{
			single_data_header header;
		};

		struct cgame_recv_583
		{
			single_data_header header;
		};

		struct cgame_recv_584
		{
			single_data_header header;
		};

		struct cgame_recv_585
		{
			single_data_header header;
		};

		struct cgame_recv_586
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 28; 	
		};

		struct dynamic_genesis_level
		{
			struct SKILL
			{
				short skill_id;
				char dynamic_level;
			};			

			single_data_header header;
			int count;
			SKILL arraaay[];	
		};

		struct cgame_recv_588
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 4; 	
		};

		struct material_refine_result
		{
			single_data_header header;
			int reserve;
			int result; //0 - falha 1 - sucesso
		};

		struct activity_event_spend_cash
		{
			single_data_header header;
			int roleid;
			int cash_count;
		};

		struct activity_event_shop
		{
			struct ITEMS
			{
				int limit_mode;
				int item_id;
				int limit_count;
			};			

			single_data_header header;
			int count; //1
			char unk6; //0
			ITEMS items[];
		};

		struct cgame_recv_592
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 28; 	
		};

		struct cgame_recv_593
		{
			single_data_header header;
			//size = sizeof(single_data_header) + 5; 	
		};

		struct cgame_recv_594
		{
			single_data_header header;
			//size = sizeof(single_data_header) + ??; 	
		};

		/*176+*/
		struct portatil_picture_info
		{
			single_data_header header;
			int roleid;
			int res; //0
			int res2; //0
			int idx;
			int res3; //0
		};		

		struct portatil_picture_storage
		{
			single_data_header header;
			int count;
			int storage[]; //0
		};	
	};
	
	// main structs
	namespace CMD
	{
		using namespace INFO;
		
		struct get_main_info_other
		{
			single_data_header header;
			int roleid;
			int colot_name;
			int rank_exp;
			int rank_level;
		};				
	};
	
	
};

#pragma pack()
#endif
