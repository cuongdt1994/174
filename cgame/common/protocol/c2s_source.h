#ifndef __C2S_SOURCE_H__
#define __C2S_SOURCE_H__

#include "../types.h"
#include "c2s_header.h"

#pragma pack(1)

namespace C2S
{
	namespace INFO
	{
		struct move_info
		{
			A3DVECTOR cur_pos;
			A3DVECTOR next_pos;
			unsigned short use_time;	//ʹ�õ�ʱ�� ��λ��ms
							//ʹ�õ�ʱ������߼���������˵��ֻ��һ���ο�ֵ
							//ͬʱ���ڼ���û���ָ���Ƿ���ȷ��������Σ�
							//�û����ƶ������ڹ̶���0.5�����Ժ����
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
		};

		struct astrolabe_opt_swallow
		{
			int type;
			int inv_index;
			int itemid;
		};
		
		struct astrolabe_opt_addon_roll
		{
			int times;
			int addon_limit;
			int inv_index;
			int itemid;
		};

		struct astrolabe_opt_aptit_inc
		{
			int inv_index;
			int itemid;
		};

		struct astrolabe_opt_slot_roll
		{
			int inv_index;
			int itemid;
		};
		
		struct astrolabe_opt_lock_unlock
		{
			char lock_unlock;
		};

		struct astrolabe_opt_save_addons
		{
			short addon_pos[10];		
		};

		struct solo_challenge_opt_select_award
		{
			int max_stage_level;
		};

		struct solo_challenge_opt_score_cost
		{
			int filter_index;
		};

		struct fix_position_transmit_opt_add_position
		{
			float pos[3];
			char   position_name[32];
		};
		
		struct fix_position_transmit_opt_delete_position
		{
			int index;
		};

		struct fix_position_transmit_opt_transmit
		{
			int index;
		};

		struct fix_position_transmit_opt_rename
		{
			int    index;
			char   position_name[32];
		};
	}
	
	namespace CMD
	{
		using namespace INFO;
		struct player_move
		{
			cmd_header header;
			move_info info;
			unsigned short cmd_seq;		//�������
		};
		struct player_logout
		{
			cmd_header header;
		};
		struct select_target
		{	
			cmd_header header;
			int id;
		};
		struct normal_attack
		{
			cmd_header header;
			char force_attack;		//ǿ�ƹ���λ 0x01 ǿ�ƹ��� 0x02 ������������ 0x04����������
							//���ܺ���Ʒ�ĵĶ��������ͬ
		};

		struct pickup_matter
		{
			cmd_header header;
			int mid;
			int type;
		};
	
		struct pickup_matter_all
		{
			cmd_header header;

			int count;
			struct entry_t
			{
				int mid;
				int type;
			} matter[];
		};

		struct resurrect
		{
			cmd_header header;
			int param;
		};
		struct player_stop_move
		{
			cmd_header header;
			A3DVECTOR pos;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;		//�������
			unsigned short use_time;
		};

		struct get_item_info
		{
			cmd_header header;
			unsigned char where;
			unsigned char index;
		};

		struct get_inventory
		{
			cmd_header header;
			unsigned char where;
		};

		struct get_inventory_detail
		{
			cmd_header header;
			unsigned char where;
		};
		
		struct exchange_inventory_item
		{
			cmd_header header;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_inventory_item
		{
			cmd_header header;
			unsigned char src;
			unsigned char dest;
			unsigned int amount;
		};

		struct drop_inventory_item
		{
			cmd_header header;
			unsigned char index;
			unsigned int amount;
		};

		struct drop_equipment_item
		{
			cmd_header header;
			unsigned char index;
		};

		struct exchange_equip_item
		{
			cmd_header header;
			unsigned char idx1;
			unsigned char idx2;
		};

		struct equip_item
		{
			cmd_header header;
			unsigned char idx_inv;
			unsigned char idx_eq;
		};

		struct move_item_to_equipment
		{
			cmd_header header;
			unsigned char idx_inv;  //src
			unsigned char idx_eq;	 //dest
		};

		struct player_goto
		{
			cmd_header header;
			A3DVECTOR pos;
		};

		struct drop_money
		{
			cmd_header header;
			unsigned int amount; 
		};

		struct self_get_property
		{
			cmd_header header;
		};

		struct set_status_point
		{
			cmd_header header;
			unsigned int vit;
			unsigned int eng;
			unsigned int str;
			unsigned int agi;
		};

		struct get_extprop_base
		{
			cmd_header header;
		};

		struct get_extprop_move
		{
			cmd_header header;
		};

		struct get_extprop_attack
		{
			cmd_header header;
		};

		struct get_extprop_defense
		{
			cmd_header header;
		};

		struct team_invite
		{
			cmd_header header;
			int id;		//��˭�������� 
		};

		struct team_agree_invite
		{
			cmd_header header;
			int id;		//˭���е�����
			int team_seq;
		};

		struct team_reject_invite
		{
			cmd_header header;
			int id;		//˭���е�����
		};

		struct team_leave_party
		{
			cmd_header header;
		};

		struct team_kick_member
		{
			cmd_header header;
			int id;
		};
		
		struct team_get_teammate_pos
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct get_others_equipment
		{
			cmd_header header;
			unsigned short size;
			int idlist[];
		};


		struct set_pickup_flag
		{
			cmd_header header;
			short pickup_flag;
		};


		struct service_hello
		{
			cmd_header header;
			int id;
		};

		struct service_get_content
		{
			cmd_header header;
			int service_type;
		};

		struct service_serve
		{
			cmd_header header;
			int service_type;
			unsigned int len;
			char content[];
		};

		struct logout
		{
			cmd_header header;
			int logout_type;
		};

		struct get_own_wealth
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		};

		struct get_all_data
		{
			cmd_header header;
			char detail_inv;
			char detail_equip;
			char detail_task;
		};

		struct use_item
		{
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
		};


		struct cast_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		struct cancel_action
		{
			cmd_header header;
		};

		struct recharge_equipped_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			int count;
		};
		
		struct recharge_flysword
		{	
			cmd_header header;
			unsigned char element_index; 
			unsigned char flysword_index;
			int count;
			int flysword_id;
		};

		struct use_item_with_target
		{
			cmd_header header;
			unsigned char where;
			unsigned char force_attack; //ֻ�Թ�������Ʒ��Ч
			unsigned short index;
			int  item_id;
		};

		struct sit_down
		{
			cmd_header header;
		};

		struct stand_up
		{
			cmd_header header;
		};
		
		struct emote_action 
		{
			cmd_header header;
			unsigned short action;
		};

		struct task_notify
		{
			cmd_header header;
			unsigned int size;
			char buf[0];
		};

		struct assist_select
		{
			cmd_header header;
			int partner;
		};

		struct continue_action
		{
			cmd_header header;
		};

		struct get_item_info_list
		{
			cmd_header header;
			char  where;
			unsigned char  count;
			unsigned char  item_list[];
		};

		struct gather_material
		{
			cmd_header header;
			int mid;
			short tool_where;
			short tool_index;
			int tool_type;
			//int space_id; //162
			int task_id;
		};

		struct get_trashbox_info
		{
			cmd_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
			char detail;
		};

		struct exchange_trashbox_item
		{
			cmd_header header;
			char where;
			unsigned char index1;
			unsigned char index2;
		};

		struct move_trashbox_item
		{
			cmd_header header;
			char where;
			unsigned char src;
			unsigned char dest;
			unsigned int amount;
		};

		struct exchange_trashbox_inventory
		{
			cmd_header header;
			char where;
			unsigned char idx_tra;
			unsigned char idx_inv;
		};

		struct move_trashbox_item_to_inventory
		{
			cmd_header header;
			char where;
			unsigned char idx_tra;
			unsigned char idx_inv;
			unsigned int amount;
		};

		struct move_inventory_item_to_trashbox
		{
			cmd_header header;
			char where;
			unsigned char idx_inv;
			unsigned char idx_tra;
			unsigned int amount;
		};
		
		struct excnahge_trashbox_money
		{
			cmd_header header;
			char is_usertrashbox;		//1���ʺŲֿ�  0����ɫ��
			unsigned int inv_money;
			unsigned int trashbox_money;
		};

		struct tricks_action 
		{
			cmd_header header;
			unsigned char action;
		};

		struct set_adv_data
		{
			cmd_header header;
			int data1;
			int data2;
		};

		struct clr_adv_data
		{
			cmd_header header;
		};

		struct team_lfg_request
		{
			cmd_header header;
			int id;
		};

		struct team_lfg_reply
		{
			cmd_header header;
			int id;
			bool result;
		};

		struct query_player_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct query_npc_info_1
		{
			cmd_header header;
			unsigned short count;
			int id[];
		};

		struct session_emote_action
		{
			cmd_header header;
			unsigned short action;
		};

		struct concurrent_emote_request
		{
			cmd_header header;
			unsigned short action;
			int target;
		};

		struct concurrent_emote_reply
		{
			cmd_header header;
			unsigned short result;
			unsigned short action;
			int target;
		};

		struct team_change_leader
		{
			cmd_header header;
			int new_leader;
		};

		struct dead_move 
		{
			cmd_header header;
			float y;
			unsigned short use_time;	//ʹ�õ�ʱ�� ��λ��ms
							//ʹ�õ�ʱ������߼���������˵��ֻ��һ���ο�ֵ
							//ͬʱ���ڼ���û���ָ���Ƿ���ȷ��������Σ�
							//�û����ƶ������ڹ̶���0.5�����Ժ����
			unsigned short speed;
			unsigned char  move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};
		
		struct dead_stop_move 
		{
			cmd_header header;
			float y;
			unsigned short speed;
			unsigned char dir;
			unsigned char move_mode;		//walk run swim fly .... walk_back run_back
			unsigned short cmd_seq;
		};

		struct enter_sanctuary
		{
			cmd_header header;
			int id;		//self id or pet id
		};

		struct enter_pk_protected
		{
			cmd_header header;
		};

		struct open_personal_market
		{
			cmd_header header;
			unsigned short count;
			char name[62];
			struct entry_t
			{
				int type;
				unsigned int index;
				unsigned int count;
				unsigned int price;
			} list[];
		};

		struct cancel_personal_market
		{
			cmd_header header;
		};

		struct query_personal_market_name
		{
			cmd_header header;
			unsigned short count;
			int list[];
		};

		struct complete_travel
		{
			cmd_header header;
		};

		struct cast_instant_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		struct destroy_item
		{
			cmd_header header;
			unsigned char where;		//���ĸ���������0 ��׼��2 ����1 װ��
			unsigned char index;		//����
			int type;			//��������Ʒ
		};

		struct enable_pvp_state
		{
			cmd_header header;
		};

		struct disable_pvp_state
		{
			cmd_header header;
		};

		struct switch_fashion_mode
		{
			cmd_header header;
		};

		struct region_transport
		{
			cmd_header header;
			int region_index;
			int target_tag;
		};

		struct cast_pos_skill
		{
			cmd_header header;
			int skill_id;
			A3DVECTOR pos;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		struct active_rush_mode
		{
			cmd_header header;
			int is_active;
		};

		struct query_double_exp_info
		{
			cmd_header header;
		};

		struct duel_request
		{
			cmd_header header;
			int target;
		};

		struct duel_reply
		{
			cmd_header header;
			int who;
			int param; // 0 ͬ��  1 ��ͬ��
		};

		struct bind_player_request
		{
			cmd_header header;
			int param_id;
			int who;
		};

		struct bind_player_invite
		{
			cmd_header header;
			int param_id;
			int who;
		};

		struct bind_player_request_reply
		{
			cmd_header header;
			int param_id;
			int who;
			int param;
		};

		struct bind_player_invite_reply
		{
			cmd_header header;
			int param_id;
			int who;
			int param;
		};

		struct query_other_equip_detail
		{
			cmd_header header;
			int target;
		};


		struct summon_pet
		{
			cmd_header header;
			unsigned int pet_index;
		};

		struct banish_pet
		{
			cmd_header header;
			unsigned int pet_index;
		};

		struct pet_ctrl_cmd
		{
			cmd_header header;
			int  target;
			int pet_cmd;
			//int pet_id;
			char buf[];
		};

		struct debug_delivery_cmd
		{
			cmd_header header;
			short  type;
			char buf[];
		};

		struct debug_gs_cmd
		{
			cmd_header header;
			char buf[];
		};

		struct mall_shopping
		{       
			cmd_header header;
			unsigned int count;
			struct __entry
			{
				unsigned int goods_count;
				int goods_id;
				int goods_index;
				int goods_slot;
			}list[];
			//.....
		};
		
		struct use_item_with_arg
		{       
			cmd_header header;
			unsigned char where;
			unsigned char count;
			unsigned short index;
			int  item_id;
			char arg[];
		};

		//lgc elf�������
		struct elf_add_attribute
		{
			cmd_header header;
			short str;
			short agi;
			short vit;
			short eng;
		};

		struct elf_add_genius
		{
			cmd_header header;
			short genius[5];
		};

		struct elf_player_insert_exp
		{
			cmd_header header;
			unsigned int exp;
			char use_sp;
		};

		struct elf_equip_item
		{
			cmd_header header;
			unsigned char index_inv;
		};

		struct elf_change_secure_status
		{
			cmd_header header;
			unsigned char status;
		};
		
		struct cast_elf_skill
		{
			cmd_header header;
			unsigned short skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		struct recharge_equipped_elf
		{	
			cmd_header header;
			unsigned char element_index; 
			int count;
		};

		struct get_mall_item_price
		{
			cmd_header header;
			short start_index;  //����������Ϊ0, ���ʾɨ��������
			short end_index;	//����[start_index,end_index)�ڵ���Ʒ��ɨ��
		};

		struct equip_trashbox_fashion_item
		{
			cmd_header header;
			unsigned char trash_idx_body;	//��ʱװ�ֿ���ʱװ�ֱ������ϵ�ʱװ�·�����Ь���׽���,255����������
			unsigned char trash_idx_leg;
			unsigned char trash_idx_foot;
			unsigned char trash_idx_wrist;			
			unsigned char trash_idx_head;
			unsigned char trash_idx_weapon;
		};
	
		struct check_security_passwd
		{
			cmd_header header;
			unsigned int passwd_size;
			char passwd[];
		};

		struct notify_force_attack
		{
			cmd_header header;
			char force_attack;
			char refuse_bless;
		};
		
		struct dividend_mall_shopping
		{       
			cmd_header header;
			unsigned int count;
			struct __entry
			{
				unsigned int goods_count;
				int goods_id;
				int goods_index;
				int goods_slot;
			}list[];
			//.....
		};
		
		struct get_dividend_mall_item_price
		{
			cmd_header header;
			short start_index;  //����������Ϊ0, ���ʾɨ��������
			short end_index;	//����[start_index,end_index)�ڵ���Ʒ��ɨ��
		};

		struct choose_multi_exp
		{
			cmd_header header;
			int index;
		};

		struct toggle_multi_exp
		{
			cmd_header header;
			char is_activate;
		};
		
		struct multi_exchange_item
		{
			cmd_header header;
			unsigned char location;
			unsigned char count;
			struct _operation
			{
				unsigned char index1;
				unsigned char index2;
			}operations[];
		};

		struct sysauction_op
		{
			cmd_header header;
			int type;
			char buf[];
		};
		
		struct calc_network_delay
		{
			cmd_header header;
			int	timestamp;
		};

		struct get_faction_fortress_info 
		{
			cmd_header header;
		};

		struct congregate_reply
		{
			cmd_header header;
			char type;
			char agree;
			int sponsor;
		};

		struct get_force_global_data
		{
			cmd_header header;
		};

		struct produce4_choose
		{
			cmd_header header;
			char remain; //�Ƿ�����װ��(0 ���ѡ����װ�� 1 ������װ��)
		};

		struct recharge_online_award
		{
			cmd_header header;
			int type;
			unsigned int count;
			struct entry
			{
				int type;
				unsigned int index; 
				unsigned int count;
			}list[];
		};

		struct toggle_online_award
		{
			cmd_header header;
			int type;
			char activate;
		};

		struct query_profit_time
		{
			cmd_header header;
		};

		struct countrybattle_get_personal_score
		{
			cmd_header header;
		};

		struct get_server_timestamp
		{
			cmd_header header;
		};

		struct countrybattle_leave 
		{
			cmd_header header;
		};
		
		struct get_cash_money_exchg_rate
		{
			cmd_header header;
		};

		struct evolution_pet
		{
			cmd_header header;
			unsigned int pet_index;
			unsigned int formula_index;
		};
		
		struct add_pet_exp
		{
			cmd_header header;
			unsigned int pet_index;
			unsigned int item_num;
		};
		
		struct rebuild_pet_nature
		{
			cmd_header header;
			unsigned int pet_index;
			unsigned int formula_index;
		};

		struct rebuild_pet_inherit_ratio
		{
			cmd_header header;
			unsigned int pet_index;
			unsigned int formula_index;
		};
		
		struct pet_rebuildinherit_choose
		{
			cmd_header header;
			char isaccept;
		};

		struct pet_rebuildnature_choose
		{
			cmd_header header;
			char isaccept;
		};

		struct exchange_wanmei_yinpiao
		{
			cmd_header header;
			char is_sell;				//1-����Ʊ 0-����Ʊ
			unsigned int count;
		};

		struct try_refine_meridian
		{
			cmd_header header;
			int index;
		};

		struct player_give_present
		{
			cmd_header header;
			int roleid;					//��������roleid
			int mail_id;				//��ȡ��Ʒ�ʼ���������û�еĻ�Ϊ-1
			int goods_id;				//������Ʒ��id
			int goods_index;			//������Ʒ���̳��е�����
			int goods_slot;				//������Ʒ��������Ϣ����
		};

		struct player_ask_for_present
		{
			cmd_header header;
			int roleid;					//��ȡ�����roleid
			int goods_id;				//��ȡ��Ʒ��id
			int goods_index;			//��ȡ��Ʒ���̳��е�����
			int goods_slot;				//��ȡ��Ʒ��������Ϣ����
		};

		struct countrybattle_get_stronghold_state
		{
			cmd_header header;
		};
		
		struct query_touch_point 
		{ 
			cmd_header header;
		};

		struct cost_touch_point 
		{ 
			cmd_header header; 
			unsigned int  index;  // ��Ʒ����
			unsigned int  lots;   // ������
			int  itemid; // ����4���Ϳͻ��˱���ֵ��Ӧ ����У����
			unsigned int  count;
			unsigned int  price;
			int	 expiretime;
		};

		struct query_title
		{
			cmd_header header;
			int roleid;
		};

		struct change_title
		{
			cmd_header header;
			unsigned short titleid;
		};

		struct daily_signin
		{
			cmd_header header;
		};

		struct late_signin
		{
			cmd_header header;
			char type;    	// 0 �첹ǩ 1 �²�ǩ 2 �겹ǩ
			int itempos;   	// ������Ʒλ��
			//int desttime;   // ��ǩʱ��� ���µ�һδǩ����|�µ�һ��|���һ��
		};

		struct apply_signinaward 
		{
			cmd_header header;
			char type;  // 0x01 �¶� 0x02 ��� 0x04 ȫ�� 0x08 ÿ��ǩ���콱
			int mon;    // �·�
		};

		struct refresh_signin
		{
			cmd_header header;
		};

		struct switch_in_parallel_world
		{
			cmd_header header;
			int key;
		};

		struct query_parallel_world
		{
			cmd_header header;
		};
		
		struct query_unique_data 
		{ 
			cmd_header header;
			int count;
			int keys[];
		};

		struct get_reincarnation_tome
		{
			cmd_header header;
		};

		struct rewrite_reincarnation_tome
		{
			cmd_header header;
			unsigned int record_index;
			int record_level;
		};

		struct activate_reincarnation_tome
		{
			cmd_header header;
			char active;
		};
		
		struct auto_team_set_goal
		{
			cmd_header header;
			char goal_type;
			char op;
			int goal_id;
		};
		
		struct auto_team_jump_to_goal
		{
			cmd_header header;
			int goal_id;
		};

		struct trickbattle_leave
		{
			cmd_header header;
		};

		struct trickbattle_upgrade_chariot
		{
			cmd_header header;
			int chariot;	
		};

		struct swallow_generalcard
		{
			cmd_header header;
			unsigned char equip_idx;
			unsigned char is_inv;	//1-���� 0-���Ʋֿ�
			unsigned char inv_idx;
			unsigned int count;
		};

		struct equip_trashbox_item
		{
			cmd_header header;
			unsigned char where;
			unsigned char trash_idx;
			unsigned char equip_idx;
		};
		
		struct send_mass_mail
		{
			cmd_header header;
			int	service_id;
			char data[];
		};

		struct random_mall_shopping 
		{ 
			cmd_header header;
			int entryid;
			int opt;
		};

		struct query_mafia_pvp_info
		{
			cmd_header header;
			int mafia_id;
		};

        struct query_can_inherit_addons
        {
            cmd_header header;
            int equip_id;
            unsigned char inv_idx;
        };

        struct activate_region_waypoints
        {
            cmd_header header;
            unsigned char num;
            int waypoints[];
        };

		struct instance_reenter_request
		{
			cmd_header header;
			bool agree_reenter;
		};

		struct solo_challenge_operate_request
		{
			cmd_header header;
			int opttype;
			char data[];
		};

		struct astrolabe_operate_request
		{
			cmd_header header;
			int  opttype;
			char data[];
		};

        struct property_score_request
        {
            cmd_header header;
            int client_data;
        };
		
		struct fix_position_transmit_operate_request
		{
			cmd_header header;
			int opttype;
			char data[];
		};
		
		struct get_cash_vip_mall_item_price
		{
			cmd_header header;
			short start_index;  //����������Ϊ0, ���ʾɨ��������
			short end_index;	//����[start_index,end_index)�ڵ���Ʒ��ɨ��
		};
		
		struct cash_vip_mall_shopping
		{       
			cmd_header header;
			unsigned int count;
			struct __entry
			{
				unsigned int goods_count;
				int goods_id;
				int goods_index;
				int goods_slot;
			}list[];
			//.....
		};

        struct update_enemylist
        {
            cmd_header header;
            char optype;
            int rid;
        };

        struct lookup_enemy
        {
            cmd_header header;
            int rid;
        };
		
		struct home_operate_request
		{
			cmd_header header;
			int opttype;
			int len;
			char data[];
		};

		struct home_leave
		{
			cmd_header header;
		};

		struct home_refresh_task
		{
			cmd_header header;
			char opttype;
		};

		struct home_kick_out_players
		{
			cmd_header header;
			int count;
			int role_ids[];
		};

		struct home_enter_request
		{
			cmd_header header;
			int home_id;
			int role_id;
			int reason;
		};

// -- cgame recv new 155 - 162 -- //

		struct lottery_manager
		{
			cmd_header header;
			unsigned int manager;
			unsigned char param[];
		};

		struct cgame_send_198
		{
			cmd_header header;
		};

		struct lib_items_manager
		{
			cmd_header header;
			unsigned int manager;
			unsigned char param[];
		};

		struct cgame_send_200
		{
			cmd_header header;
		};
		
		// ec arena man
		struct arena_manager
		{
			enum
			{
				// Invite Team Group (Request)
				ARENA_MANAGER_PROTOCOL_SEND_INVITE_TEAM = 5593,
				ARENA_MANAGER_PROTOCOL_SEND_INVITE_TEAM_SZ = 16,

				// Invite Team Group (Response)
				ARENA_MANAGER_PROTOCOL_RESPONSE_INVITE_TEAM = 5595,
				ARENA_MANAGER_PROTOCOL_RESPONSE_INVITE_TEAM_SZ = 21,

				// Leave Team Group
				ARENA_MANAGER_PROTOCOL_LEAVE_TEAM = 5597,
				ARENA_MANAGER_PROTOCOL_LEAVE_TEAM_SZ = 12,

				// Remove Member Team Group
				ARENA_MANAGER_PROTOCOL_KICKER_MEMBER_TEAM = 5599,
				ARENA_MANAGER_PROTOCOL_KICKER_MEMBER_TEAM_SZ = 16,	

				// Change Leader Team Group
				ARENA_MANAGER_PROTOCOL_CHANGE_LEADER_TEAM = 5601,
				ARENA_MANAGER_PROTOCOL_CHANGE_LEADER_TEAM_SZ = 16,

			};

			cmd_header header;
			unsigned int protocol;
			unsigned int protocol_size;

		};
		
		struct arena_manager_invite : public arena_manager
		{
			int idx;
			int team_type; 
			int roleid; 
			int invited_roleid; 
			int invited_cls;
			char name_size;
			char name[];
		};

		struct arena_manager_invite_request : public arena_manager
		{
			int roleid;
			int team_type;
			int invited_roleid;
			int idx;
		};
		
		struct arena_manager_kickout : public arena_manager
		{
			int idx;
			int roleid;
			int kickout_id;
			int team_type;
		};

		struct arena_manager_transfer : public arena_manager
		{
			int idx;
			int roleid;
			int transfer_id;
			int team_type;
		};		

		struct arena_manager_leave : public arena_manager
		{
			int idx;
			int roleid;
			int team_type;
		};
		// ec arena man end

		// glyph managewr
		struct glyph_manager
		{
			cmd_header header;  // 202
			unsigned int mode; // 0 inc , 1 dec , 2 upgrade , 3 get info
			unsigned int count; // inc & dec 1 , craft 6 
		};

		struct glyph_skill_inc : public glyph_manager
		{
			unsigned int pos; // inc & dec add glyph slot , craft 0
			int skill; //skill id glyph
		};
		
		struct glyph_item_upgrade : public glyph_manager
		{
			struct req_item
			{
				unsigned int reserve; //wtf
				int id;
			} req[6];
		};
		// glyph managewr end
		
		struct arena_battle_enter
		{
			cmd_header header;	
			short is_team; //0 (solo) - 1 (team)
			short is_6x6; //0 (3x3) - 1 (6x6)
		};

		struct arena_battle_leave
		{
			cmd_header header;	
		};

		struct enter_carrier
		{
			cmd_header header;
			int pos;
			unsigned int npcid;
			int reserve1;
		};

		struct leave_carrier
		{
			cmd_header header;
			int pos;
			unsigned int npcid;
		};

		struct cgame_send_207
		{
			cmd_header header;
		};

		struct cgame_send_208
		{
			cmd_header header;
		};

		struct cgame_send_209
		{
			cmd_header header;
		};

		struct cgame_send_210
		{
			cmd_header header;
		};

		struct cgame_send_211
		{
			cmd_header header;
		};

		struct cgame_send_212
		{
			cmd_header header;
		};

		struct cgame_send_213
		{
			cmd_header header;
		};

		struct cgame_send_214
		{
			cmd_header header;
		};

		struct cgame_send_215
		{
			cmd_header header;
		};

		struct cgame_send_216
		{
			cmd_header header;
		};

		struct cgame_send_217
		{
			cmd_header header;
		};

		// card manager 
		struct card_manager
		{			
			cmd_header header;			
		};

		struct produce_card_manager : public card_manager
		{
			int count_itens;
			int recipe_id;
			int index_inv[];
		};

		struct new_card_manager : public card_manager
		{
			enum 
			{
				MODE_DECOMPOSE,
				MODE_RESTORE,
				MODE_ACTIVATE_IMPERIAL,
			};

			int mode; 
			int res;
			int index_inv;
			int res2;
			int res3;
			int res4;
		};

		// card manager end

		struct cgame_send_219
		{
			cmd_header header;
		};

		struct cgame_send_220
		{
			cmd_header header;
		};

		struct engrave_choose
		{
			cmd_header header;
			char remain; //1 para continuar o antigo //0 para adicionar o novo
		};

		struct cgame_send_222
		{
			cmd_header header;
		};

		struct card_s_decompose
		{
			cmd_header header;
			
			int count_itens;
			int index_inv[];
		};

		struct repository_change_inc
		{
			cmd_header header;	
			unsigned int item_id;
			int index;
		};

		/*162+*/
		struct armor_manager_spirit_inc // Adiciona o Espírito
		{
			cmd_header header;
			int type; //0
			int mode; //2
			int pos_spirit; //91
			int pos_armor; //92
		};

		struct armor_manager_spirit_dec // Remove
		{
			cmd_header header;
			int type; //1
			int mode; //1
			int pos_armor; //92
		};

		struct armor_manager_crystal_inc // Adiciona o Crystal
		{
			cmd_header header;
			int type; //0
			int mode; //2
			int pos_crystal; //88
			int pos_armor; //92
		};

		struct armor_manager_crystal_dec // Remove
		{
			cmd_header header;
			int type; //3
			int mode; //1
			int pos_armor; //92
		};

		// Bloqueia / Desbloqueia o Cristal 

		struct lock_crystal_armor
		{
			cmd_header header;
			int type; //2
			int mode; //2
			int pos_armor; //92
			int lock; //1 or 0 - unlock
		};

		// Desperar do Espírito

		struct awakening_spirit_armor
		{
			cmd_header header;
			int type; //2
			int mode; //1
			int pos_armor; //92
		};

		// Bloqueia e Desbloqueia 

		struct lock_spirit_armor
		{
			cmd_header header;
			int type; //6
			int mode; //2
			int pos_armor; //92
			int lock; //1 or 0 - unlock
		};

		// Bloqueia e Desbloqueia 

		struct extract_spirit_armor
		{
			cmd_header header;
			int type; //5
			int mode; //2
			int pos_armor_rmv; //26
			int pos_armor_rcv; //14
		};

		// Decompoe os Espíritos

		struct decompose_spirit_armor
		{
			cmd_header header;
			int type; //7
			int count_itens; //2
			int pos_inv[]; //90	- 91		
		};

		// Purificação (Roleta de atributos)

		struct purification_spirit_armor
		{
			cmd_header header;
			int type; //3
			int count_addons_lock; //1~5
			int idx_pos_armor; //78
			int addons_lock[];	// 0 ~ 4 (for)	
		};

		struct purification_spirit_armor_change
		{
			cmd_header header;
			int type; //4
			int mode; //2
			int pos_armor; //78
			int change; //0 mantém antigo 1// escolhe o novo
		};

	
		// Devorar Cristal 

		struct devour_crystal_armor
		{
			cmd_header header;
			int type; //1
			int count_cystals; //2
			int pos_crystal_consum[]; //64 (item recv), 65
		};

		struct cast_shield_skill
		{
			cmd_header header;
			int skill_id;
			unsigned char force_attack;
			unsigned char target_count;
			int  targets[];
		};

		struct cgame_send_228
		{
			cmd_header header;
		};

		struct cgame_send_229
		{
			cmd_header header;
		};

// -- cgame recv new 162 - 172 -- //

		struct cgame_send_230
		{
			cmd_header header;
		};

		struct cgame_send_231
		{
			cmd_header header;
		};

		struct cgame_send_232
		{
			cmd_header header;
		};

		struct safe_lock_new_inc
		{
			cmd_header header;	
			unsigned int origin_size;
			unsigned int new_size;
			char origin_passwd[1];
			char new_passwd[1];
		};

		struct change_gold_silver
		{
			enum
			{
				SUC_MSG_SILVER_GOLD = 534,
				SUC_MSG_GOLD_SILVER = 535,
			};

			cmd_header header;
			unsigned char is_silver;
			unsigned int money;
		};

		struct cgame_send_235
		{
			cmd_header header;
		};

		struct lottery2_manager
		{
			cmd_header header;
			unsigned int manager;
			unsigned char param[];
		};

		struct cgame_send_237
		{
			cmd_header header;
		};

		struct celestial_memorial_manager
		{
			enum // Type
			{
				OPEN_RED_BOX = 0,
				GET_REWARD = 1,
				INIT_LOTTERY = 2,
				GET_LOTTERY_REWARD = 3,
				GET_RANKING = 4,
			};

			cmd_header header;
			unsigned int type;
			unsigned int pos_idx; // Open: index_inv || Reward: pos_reward || 0,1,2 = lottery init && get reward
			int reward_level_2; // 0~1  174+
		};

		struct cgame_send_239
		{
			cmd_header header;
		};

		struct cgame_send_240
		{
			cmd_header header;
		};

		struct camp_token_add_exp
		{
			cmd_header header;

			unsigned int mode;
			int count_ints; // N/A

			int idx_camp_token;
			int count_item;
		};	

		struct camp_token_new_point
		{
			cmd_header header;

			unsigned int mode;
			int count_ints; // N/A

			int idx_camp_token;
			int pos_line_key;
			int upgrade_item_idx; // -1
			int upgrade_perc_pos; // 0
		};	

		struct camp_token_purification
		{
			cmd_header header;

			unsigned int mode;
			int count_ints; // N/A

			int idx_camp_token;
			int pos_line_key;
			int return_count; //1 (Volta 1 casa) - 0 (Reseta)		
		};	
		
		struct cgame_send_242
		{
			cmd_header header;
		};

		struct skill_pet_move
		{
			cmd_header header;
			unsigned int index_from; //0~7
			unsigned int index_to; //0~7
		};

		struct res_pet_cmd
		{
			cmd_header header;
		};

		struct pet_skin_inc
		{
			cmd_header header;
			unsigned int mode; //0:Use 1:Sell
			unsigned int index; //0~MAX_SLOT_PET
			unsigned int pos_data; //1
		};

		struct codex_consum_fashion_single
		{
			cmd_header header;
			int idx;
		};

		struct codex_reward_first
		{
			cmd_header header;
			int type; //0~4
			int idx;
		};

		struct codex_change_single_fashion
		{
			cmd_header header;
			int idx;
			int fashion_id;
			int color;
		};

		struct codex_fashion_equip
		{
			struct FASHION
			{
				int fashion_id;
				int fashion_color;
				int fashion_pos;
			};

			cmd_header header;
			//int suite_fashion_id; /*176+*/
			int fashion_count;
			//char classic_mode; /*176+*/
			FASHION fashions[];
		};

		struct codex_request_storage
		{
			cmd_header header;
			int reserve;
			int reserve2;
			int reserve3;
		};

		struct codex_consum_fly_single
		{
			cmd_header header;
			int idx;
		};

		struct codex_change_fly_equip
		{
			cmd_header header;
			int fly_id;
		};

		struct cgame_send_253
		{
			cmd_header header;
		};

		struct cgame_send_254
		{
			cmd_header header;
		};

		struct codex_consum_mount_single
		{
			cmd_header header;
			int idx;
		};

		struct codex_gain_pet
		{
			cmd_header header;
			int pet_id;
		};

		struct codex_free_pet
		{
			cmd_header header;
			int pet_idx;
		};

		struct codex_reward_title
		{
			cmd_header header;
			int type; //0~4
		};

		struct codex_consum_fashion_mutiple
		{
			cmd_header header;
			int idx;
			int fashion_colors[];
		};

		struct cgame_send_260
		{
			cmd_header header;
		};

		struct cgame_send_261
		{
			cmd_header header;
		};

		struct cgame_send_262
		{
			cmd_header header;
		};

		struct cgame_send_263
		{
			cmd_header header;
		};

		struct cgame_send_264
		{
			cmd_header header;
		};

		struct cgame_send_265
		{
			cmd_header header;
		};

		struct cgame_send_266
		{
			cmd_header header;
		};

		struct cgame_send_267
		{
			cmd_header header;
		};

		struct cgame_send_268
		{
			cmd_header header;
		};

		struct activity_event_reward_login
		{
			cmd_header header;
			unsigned int idx; //7024
			unsigned int pos; //1
		};

		struct activity_event_manager
		{
			enum
			{
				LOGIN_REWARD = 0,
				REWARD_FOR_USE = 1,
				SHOP_BUY = 2,
			};

			cmd_header header;
			unsigned int type; //0
			unsigned int idx; //7028
			unsigned int pos; //0 (reserved)
			unsigned int count; //0 ~ 1 
		};

		struct enable_fashion_weapon
		{
			cmd_header header;
		};

		struct world_points_reward
		{
			cmd_header header;
		};

		struct world_points_manager
		{
			cmd_header header;
			unsigned int value_pos; //3
			unsigned int count_tasks; //2
			unsigned int count_ob_tasks; //10
			unsigned int max_ob_tasks; //11
		};

		struct cgame_send_274
		{
			cmd_header header;
		};

		/*176+*/
		struct portatil_picture
		{
			cmd_header header;
			unsigned int idx;
		};
	};
	
	namespace CMD // gm 
	{
		using namespace INFO;
		
		struct  gmcmd_move_to_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_recall_player
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_player_inc_exp
		{
			cmd_header header;
			int exp;
			int sp;
		};

		struct gmcmd_endue_item
		{
			cmd_header header;
			int item_id;
			unsigned int count;
		};

		struct gmcmd_endue_sell_item
		{
			cmd_header header;
			int item_id;
			unsigned int count;
		};

		struct gmcmd_remove_item
		{
			cmd_header header;
			int item_id;
			unsigned int count;
		};

		struct gmcmd_endue_money
		{
			cmd_header header;
			int money;
		};


		struct gmcmd_offline
		{
			cmd_header header;
		};

		struct gmcmd_resurrect
		{
			cmd_header header;
		};

		struct gmcmd_enable_debug_cmd
		{
			cmd_header header;
		};

		struct gmcmd_drop_generator
		{
			cmd_header header;
			int id;
		};

		struct gmcmd_active_spawner
		{
			cmd_header header;
			bool is_active;
			int sp_id;
		};

		struct gmcmd_generate_mob
		{
			cmd_header header;
			int mob_id;
			int vis_id;
			short count;
			short life;
			unsigned int name_len;
			char name[];
		};
		
		struct gmcmd_get_common_value
		{
			cmd_header header;
			int key;
		};

		struct gmcmd_query_spec_item
		{
			cmd_header header;
			int roleid;
			int type;
		};

		struct gmcmd_remove_spec_item
		{
			cmd_header header;
			int roleid;
			int type;
			unsigned char where;
			unsigned char index;
			unsigned int count;
		};

		struct gmcmd_open_activity
		{
			cmd_header header;
			int activity_id;
			char is_open;
		};
		
		struct gmcmd_change_ds
		{
			cmd_header header;
			int flag;
		};
		
		struct gmcmd_query_unqiue_data
		{
			cmd_header header;
			int key;
		};
	};


	namespace CMD // main struct 
	{
		using namespace INFO;
		
		struct set_color_name
		{
			cmd_header header;
			unsigned int color;
		};
		
		struct get_main_info
		{
			cmd_header header;
			unsigned int roleid;
		};
		
	};

};


#pragma pack()
#endif


