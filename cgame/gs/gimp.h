#ifndef __ONLINEGAME_GS_IMP_H__
#define __ONLINEGAME_GS_IMP_H__

#include <octets.h>
#include <ASSERT.h>
#include <common/packetwrapper.h>
#include <vector.h>
#include <common/message.h>
#include "substance.h"
#include "attack.h"
#include "playersolochallenge.h"

int gplayer__GetParentId(void* _pImp);
A3DVECTOR gplayer__GetParentPos(void* _pImp);

namespace GNET
{
	struct syncdata_t;
}

namespace GDB
{
	struct itemlist;
}

struct item_data;
struct slice;
struct gobject;
class world;
class gobject_imp;
class item;
struct extend_prop;
struct instance_key;
struct MNFactionStateInfo;
struct fix_position_transmit_info;

class controller :public substance
{
public:
DECLARE_SUBSTANCE(controller);
public:
	gobject_imp * _imp;

public:
	controller():_imp(NULL){}
	virtual void Init(gobject_imp * imp) {_imp = imp;} 
	virtual ~controller(){}

	/*
	 *	���Ʋ��ֵ���Ϣ�������������ڿ��Ʋ�����ʱ������Ҫ��ȡһЩ��Ϣ
	 *	��������Ҳ���Զ���Ϣ���д���
	 *	�ܹ����д�����ǰ����_do_msg��ֵΪTrue
	 *	����Ϣ����������Ҫ���ʵ�ֲ�����һ����
	 */
	virtual  int MessageHandler(world * pPlane ,const MSG & msg)
	{
		return 0;
	}

	

	/*
	 *	��������������
	 */
	virtual  int CommandHandler(int cmd_type,const void * buf, unsigned int size) = 0;

	/**
	 *	������slice���ƶ������ڿ����ǲ�ͬ�Ķ������������麯��
	 *	����controller��ȻҪ����ȷ��gobject�󶨣���������������
	 *	���ط�0��ʾ�ƶ�ʧ�ܣ�һ��ָ�����Ѿ�����src��
	 */
	virtual int MoveBetweenSlice(gobject * obj,slice * src, slice * dest) = 0;

	/**
	 *	�ͷŶ�����������ֻ��controller�϶���NPC��Player�󶨣������������ͷš�
	 */
	virtual void Release(bool free_parent = true) = 0;

	/**
	 *	����
	 */

	virtual void Reborn()
	{
		ASSERT(false);
	}

	/**
	 	���������л�ʱ�����ĵ���
	 */
	virtual void SwitchSvr(int dest, const A3DVECTOR & oldpos, const A3DVECTOR &newpos,const instance_key * switch_key) { ASSERT(false);}


	virtual void error_cmd(int msg)
	{
	}
	virtual void OnHeartbeat(unsigned int tick)
	{
	}
	virtual void  NPCSessionStart(int task_id, int session_id)
	{}

	virtual void NPCSessionEnd(int task_id,int session_id, int retcode)
	{}

	virtual void NPCSessionUpdateChaseInfo(int task_id,const void * buf ,unsigned int size)
	{}

	virtual int GetNextMoveSeq() { return -1;}
	virtual int GetCurMoveSeq() { return -1;}
	virtual void SetNextMoveSeq(int seq) { return ;}

	virtual bool HasGMPrivilege() { return false;}

	virtual void DenyCmd(unsigned int cmd_type) {}
	virtual void AllowCmd(unsigned int cmd_type){}

public:
	enum
	{
		CMD_MOVE,
		CMD_ATTACK,
		CMD_PICKUP,
		CMD_MARKET,
		CMD_PET,
		CMD_ELF_SKILL,
		CMD_USE_ITEM,
		CMD_NORMAL_ATTACK,
		CMD_MAX,
	};
};

class dispatcher :public substance
{
public:
DECLARE_SUBSTANCE(dispatcher);
protected:
	gobject_imp * _imp;
public:
	dispatcher():_imp(NULL){}
	virtual ~dispatcher(){}

	void set_imp(gobject_imp * imp) {_imp = imp;} 
	virtual void init(gobject_imp * imp) {_imp = imp;} 
	virtual void set_gm_invisible(bool invisible = true) {}
	virtual bool is_gm_invisible() {return false;}

	virtual void begin_transfer() = 0;				//��ʼһ�����ݴ���
	virtual void end_transfer() = 0;				//����һ�����ݴ��ͣ���ջ���
	virtual void enter_slice(slice * ,const A3DVECTOR &) = 0;	//����һ�����ӣ����������˽��룬����ȡ��������ӵ���Ϣ
	virtual void leave_slice(slice * ,const A3DVECTOR &) = 0;	//�뿪һ�����ӣ������������뿪
	virtual void notify_pos(const A3DVECTOR & pos) {};		//֪ͨ�Լ��ĵ�ǰλ�ã�ֻ��player���������
	virtual void get_base_info() = 0;				//�����Լ��Ļ������ݸ��Լ����Ժ���ܻ����
	virtual void enter_world() = 0;					//��������
	virtual	void leave_world() = 0;					//�뿪����
	virtual void appear() { enter_world();}				//����
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode) = 0;//�ɷ��ƶ�ָ��
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mode){}//�ɷ�ֹͣ�ƶ�ָ��
	virtual void notify_move(const A3DVECTOR &oldpos, const A3DVECTOR & newpos)  = 0;//���û��ƶ�������ת����������������
	virtual void start_attack(const XID &){}
	virtual void attack_once(unsigned char dec_amount){}
	virtual void stop_attack(int flag){}
	virtual void dodge_attack(const XID &attacker,int skilll_id, const attacker_info_t &, int at_state,char speed,bool orange,unsigned char section) {}
	virtual void be_damaged(const XID & id, int skill_id,const attacker_info_t &,int damage,int dura_index,int at_state,char speed,bool orange,unsigned char section){}
	virtual void be_hurt(const XID & id, const attacker_info_t & ,int damage,bool invader){}
	virtual void equipment_damaged(int index,char reason){}
	virtual void on_death(const XID & killer,bool ) {}
	virtual void error_message(int message) {}
	virtual void disappear() {}					//������ʧ����Ϣ
	virtual void matter_pickup(int id) {}				//������ʧ����Ϣ
	virtual void resurrect(int){}
	//virtual void pickup_money(int money, unsigned char is_silver){}
	virtual void pickup_item(int type,int expire_date, int amount,int slot_amount, int where,int index){}
	virtual void purchase_item(int type, unsigned int money,int amount,int slot_amount, int where,int index){}
	virtual void query_info00(const XID & target, int cs_index,int sid){}
	virtual void query_info00(){}					//�����Լ���info00��ֻ��player��Ч 
	virtual void query_elf_vigor(){} //lgc
	virtual void query_elf_enhance(){}
	virtual void query_elf_stamina(int sta){}
	virtual void query_elf_exp(int exp){}
	virtual void elf_cmd_result(int cmd, int result, int param1, int param2){}
	virtual void elf_levelup(){} 
	virtual void receive_exp(int exp,int sp){}			//���ܾ���ֵ�ͼ��ܵ�
	virtual void level_up(){}					//����
	virtual void unselect(){}					//û��ѡ���κ���
	virtual void player_select_target(int id){}	
	virtual void self_item_empty_info(int where,int index) {}
	virtual void self_item_info(int where,int index,item_data & data,unsigned short crc) {}
	virtual void self_inventory_data(int where,unsigned char inv_size,const void * data, unsigned int len) {}
	virtual void self_inventory_detail_data(int where,unsigned char inv_size,const void * data, unsigned int len) {}
	virtual void exchange_inventory_item(unsigned int idx1,unsigned int idx2) {}
	virtual void move_inventory_item(unsigned int src,unsigned int dest,unsigned int count) {}
	virtual void player_drop_item(unsigned int where, unsigned int index,int type,unsigned int count,unsigned char drop_type) {}
	virtual void exchange_equipment_item(unsigned int index1,unsigned int index2) {}
	virtual void equip_item(unsigned int index_inv,unsigned int index_equip,int count_inv,int count_eq) {}
	virtual void move_equipment_item(unsigned int index_inv,unsigned int index_equip, unsigned int count){}
	virtual void self_get_property(unsigned int status_point, const extend_prop & prop , int attack_degree, int defend_degree, int crit_rate, int crit_damage_bonus, int invisible_degree, int anti_invisible_degree, int penetration, int resilience, int vigour, int anti_def_degree, int anti_resist_degree, const extend_prop & base_prop, int highest_level, int reserve) {}
	virtual void set_status_point(unsigned int vit, unsigned int eng, unsigned int str, unsigned int agi, unsigned int remain) {}
	virtual void get_extprop_base() {}
	virtual void get_extprop_move() {}
	virtual void get_extprop_attack() {}
	virtual void get_extprop_defense() {}
	virtual void player_reject_invite(const XID & member) {}
	virtual void leader_invite(const XID & leader,int seq,int pickup_flag) {}
	virtual void join_team(const XID & leader,int pickup_flag) {}
	virtual void member_leave(const XID & leader, const XID& member,int type) {}
	virtual void leave_party(const XID & leader, int type) {}
	virtual void new_member(const XID & member) {}
	virtual void leader_cancel_party(const XID & leader) {}
	virtual void teammate_get_pos(const XID & target,const A3DVECTOR & pos,int tag, bool same_plane) {}
	virtual void send_equipment_info(const XID & target, int cs_index, int sid) {}
	virtual void equipment_info_changed(uint64_t madd,uint64_t mdel,const void * buf, unsigned int size){} 
	virtual void team_member_pickup(const XID & target,int type, int count) {}
	virtual void npc_greeting(const XID & provider) {}
	virtual void repair_all(unsigned int cost) {}
	virtual void repair(int where,int index,unsigned int cost) {}
	virtual void renew() {}
	//virtual void spend_money(unsigned int cost){}
	virtual void get_player_money(unsigned int money, unsigned long long silver ) {}
	virtual void cast_skill(const XID & target, int skill,unsigned short time, unsigned char level) {}
	virtual void cast_rune_skill(const XID & target, int skill,unsigned short time, unsigned char level) {}
	virtual void skill_interrupt(char reason) {}
	virtual void skill_perform() {}
	virtual void stop_skill() {}
	virtual void get_skill_data() {}
	virtual void clear_embedded_chip(unsigned short equip_idx,unsigned int use_money) {}
	virtual void cost_skill_point(int skill_point) {}
	virtual void learn_skill(int skill, int level) {}
	virtual void takeoff() {}
	virtual void landing() {}
	virtual void flysword_time_capacity(unsigned char where, unsigned char index, int cur_time){}
	virtual void obtain_item(int type,int expire_date, int amount,int slot_amount, int where,int index){}
	
	virtual void produce_start(int type, int use_time ,int count){}
	virtual void produce_once(int type, int amount,int slot_amount, int where,int index){}
	virtual void produce_end(){}

	virtual void decompose_start(int use_time,int type) {}
	virtual void decompose_end() {}
	virtual void get_task_data() {}
	virtual void send_task_var_data(const void * buf, unsigned int size) {}
	virtual void start_use_item(int item_type, int use_time) {}
	virtual void cancel_use_item() {}
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count){}	//���Լ�
	virtual void use_item(int item_type){} //������
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, unsigned int arg_size){}//���Լ�
	virtual void use_item(int item_type,const char * arg, unsigned int arg_size){} //������
	virtual void start_use_item_with_target(int item_type, int use_time,const XID & target) {}

	virtual void sit_down() {}
	virtual void stand_up() {}
	virtual void do_emote(unsigned short emotion) {}
	virtual void do_emote_restore(unsigned short emotion) {}
	virtual void do_action(unsigned char action) {}
	virtual void send_timestamp() {}
	virtual void notify_root(unsigned char type) {}
	virtual void dispel_root(unsigned char type) {}
	virtual void invader_rise(){}
	virtual void pariah_rise(){}
	virtual void invader_fade(){}
	virtual void update_visible_state(unsigned int newstate, unsigned int newstate2, unsigned int newstate3, unsigned int newstate4, unsigned int newstate5, unsigned int newstate6, unsigned int newstate7, unsigned int newstate8, unsigned int newstate9);
	virtual void gather_start(int mine, unsigned char t) {}
	virtual void gather_stop() {}
	virtual void trashbox_passwd_changed(bool has_passwd) {}
	virtual void trashbox_passwd_state(bool has_passwd) {}
	virtual void trashbox_open(char is_usertrashbox) {}
	virtual void trashbox_close(char is_usertrashbox) {}
	virtual void trashbox_wealth(char is_usertrashbox, unsigned int money) {}
	virtual void exchange_trashbox_item(int where, unsigned int idx1, unsigned int idx2) {}
	virtual void move_trashbox_item(int where , unsigned int src, unsigned int dest, unsigned int delta) {}
	virtual void exchange_trashbox_inventory(int where, unsigned int idx_tra,unsigned int idx_inv) {}
	virtual void trash_item_to_inventory(int where, unsigned int idx_tra, unsigned int idx_inv, unsigned int delta) {}
	virtual void inventory_item_to_trash(int where, unsigned int idx_inv, unsigned int idx_tra, unsigned int delta) {}
	virtual void exchange_trash_money(char is_usertrashbox, int inv_money, int tra_money) {}
	virtual void enchant_result(const XID & caster, int skill, char level,bool invader, int at_state,unsigned char section);
	virtual void set_adv_data(int data1,int data2){}
	virtual void clear_adv_data(){}
	virtual void player_in_team(unsigned char state) {}
	virtual void send_party_apply(int id) {}
	virtual void query_info_1(int uid,int cs_index, int cs_sid) {}
	virtual void concurrent_emote_request(int id, unsigned short action) {}
	virtual void do_concurrent_emote(int id, unsigned short action) {}
	virtual void mafia_info_notify() {}
	virtual void task_deliver_reputaion(int offset,int cur_reputaion) {} 
	virtual void task_deliver_exp(int exp, int sp) {}
	virtual void task_deliver_money(unsigned int money,unsigned int cur_money, unsigned int silver) {}
	virtual void task_deliver_item(int type, int expire_date,int amount,int slot_amount, int where,int index){}
	virtual void task_deliver_level2(int level) {}
	virtual void get_reputation(int reputation) {}
	virtual void identify_result(char index, char result){}
	virtual void change_shape(char shape){}
	virtual void elf_refine_activate(char status){}	//lgc
	virtual void enter_sanctuary(){}
	virtual void leave_sanctuary(){}
	virtual void begin_personal_market(int market_id,const char * name,unsigned int len) {}
	virtual void cancel_personal_market() {}
	virtual void market_trade_success(int trader) {}
	virtual void send_market_name(const XID & target, int cs_index, int sid,const char * name ,unsigned int len) {}
	virtual void player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle){}
	virtual void player_complete_travel(int vehicle) {}
	virtual void gm_toggle_invisible(char tmp) {}
	virtual void toggle_invincible(char tmp) {}
	virtual void trace_cur_pos(unsigned short seq) {}
	virtual void cast_instant_skill(const XID & target, int skill,unsigned char level) {}
	virtual void cast_rune_instant_skill(const XID & target, int skill,unsigned char level) {}
	virtual void cast_elf_skill(const XID & target, int skill,unsigned char level) {}	//lgc
	virtual void activate_waypoint(unsigned short waypoint){}
	virtual void player_waypoint_list(const unsigned short * buf, unsigned int count){}
	virtual void unlock_inventory_slot(unsigned char where, unsigned short index) {}
	virtual void team_invite_timeout(int who) {}
	virtual void enable_pvp_state(char type) {}
	virtual void disable_pvp_state(char type) {}
	virtual void player_pvp_cooldown(int cooldown) {}
	virtual void send_cooldown_data(){}
	virtual void skill_ability_notify(int id, int ability){}
	virtual void personal_market_available() {}
	virtual void breath_data(int breath, int breath_capacity) {}
	virtual void stop_dive() {}
	virtual void trade_away_item(int buyer,short inv_idx,int type, unsigned int count) {}
	virtual void player_enable_fashion_mode(unsigned char is_enable, unsigned char new_model ) {}
	virtual void player_enable_free_pvp(char is_enable){}
	virtual void player_enable_effect(short effect) {}
	virtual void player_disable_effect(short effect) {}
	virtual void enable_resurrect_state(float exp_reduce) {}
	virtual void set_cooldown(int idx, int cooldown){}
	virtual void change_team_leader(const XID & old_leader, const XID & new_leader) {}
	virtual void kickout_instance(char reason, int time_out) {}
	virtual void begin_cosmetic(unsigned short inv_index) {}
	virtual void end_cosmetic(unsigned short inv_index) {}
	virtual void cosmetic_success(unsigned short crc) {}
	virtual void cast_pos_skill(const A3DVECTOR & pos, const XID &target, int skill, unsigned short time, unsigned char level){}
	virtual void change_move_seq(unsigned short seq) {}
	virtual void server_config_data() {}
	virtual void active_rush_mode(char is_active) {}
	virtual void produce_null(int recipe_id) {}
	virtual void enable_double_exp_time(int mode, int end_time) {}
	virtual void available_double_exp_time() {}
	virtual void active_pvp_combat_state(bool is_active) {}
	virtual void duel_recv_request(const XID &target) {}
	virtual void duel_reject_request(const XID &target,int reason) {}
	virtual void duel_prepare(const XID & target, int delay) {}
	virtual void duel_cancel(const XID & target) {}
	virtual void duel_start(const XID & who) {}
	virtual void duel_stop() {}
	virtual void duel_result(const XID & target, bool is_failed) {}
	virtual void player_bind_request(const XID & target, int param_id) {}
	virtual void player_bind_invite (const XID & target, int param_id) {}
	virtual void player_bind_request_reply(const XID & target, int param) {}
	virtual void player_bind_invite_reply(const XID & target, int param) {}
	virtual void player_bind_start(const XID & target) {}
	virtual void player_bind_stop() {}
	virtual void player_mounting(int mount_id, unsigned short mount_color) {}
	virtual void send_equip_detail(int cs_indx, int cs_sid, int target, const void * data, unsigned int size){}
	virtual void send_inventory_detail(int cs_indx, int cs_sid, int target, unsigned int money, unsigned char inv_size, const void * data, unsigned int size){}
	virtual void send_others_property(const void * data, unsigned int size, const void * self_data, unsigned int self_size){}
	virtual void pariah_duration(int remain_time){}
	virtual void gain_pet(int index, const void * buf, unsigned int size){}
	virtual void free_pet(int index, int pet_id){}
	virtual void summon_pet(int index, int pet_tid/*ģ��ID*/, int pet_id/*����ID*/, int life_time) {}
	virtual void start_pet_operation(int index, int pet_id,int delay,int operation) {}
	virtual void end_pet_operation() {}
	virtual void recall_pet(int index, int pet_id, char reason) {}
	virtual void pet_recv_exp(int index, int pet_id, int exp) {}
	virtual void pet_level_up(int index, int pet_id, int level, int cur_exp) {}
	virtual void pet_room_capacity(int cap) {}
	virtual void notify_pet_honor(int index,int honor) {}
	virtual void notify_pet_hunger(int index, int hunger) {}
	virtual void enter_battleground(int role, int battle_id,int end_time){}
	virtual void send_turrent_leader(int id) {}
	virtual void battle_result(int result) {}
	virtual void battle_score(int oscore, int ogoal, int dscore, int dgoal) {}
	virtual void pet_dead(int index) {}
	virtual void pet_revive(int index,float hp_factor) {}
	virtual void pet_hp_notify(int index, float hp_factor, int cur_hp, float mp_factor, int cur_mp) {}
	virtual void pet_ai_state(char aggro_state, char stay_state) {}
	virtual void refine_result(int rst) {}
	virtual void pet_set_cooldown(int index, int cd_index, int msec) {}
	virtual void player_cash(int cash) {}
	virtual void player_bind_success(unsigned int index, int id) {}
	virtual void player_change_inventory_size(int size) {}
	virtual void player_pvp_mode(int level) {}
	virtual void player_wallow_info(int level, int ptime, int light_t, int heavy_t, int reason) {}
	virtual void player_change_spouse(int id) {}
	virtual void notify_safe_lock(char active, int time, int max_time, bool paswd) {}
	virtual void mall_item_buy_failed(short index, char reason){}//lgc
	virtual void equip_trashbox_item(int where, unsigned char trash_idx,unsigned char equip_idx){}
	virtual void security_passwd_checked(){}
	virtual void toggle_invisible(int invisible_degree);
	virtual void appear_to_spec(int invi_degree){}
	virtual void disappear_to_spec(int invi_degree){}
	virtual void on_inc_invisible(int prev_invi_degree, int cur_invi_degree){}
	virtual void on_dec_invisible(int prev_invi_degree, int cur_invi_degree){}
	virtual void on_inc_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree){}
	virtual void on_dec_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree){}
	virtual void hp_steal(int hp){}
	virtual void player_dividend(int cash) {}
	virtual void dividend_mall_item_buy_failed(short index, char reason){}
	virtual void multi_exp_info(int last_timestamp, float enhance_factor){}
	virtual void change_multi_exp_state(char state, int enchance_time, int buffer_time, int impair_time, int activate_times_left){}
	virtual void send_world_life_time(int life_time){}
	virtual void wedding_book_success(int type){}
	virtual void calc_network_delay(int timestamp){}
	virtual void player_knockback(const A3DVECTOR & pos, int time){}
	virtual void summon_plant_pet(int plant_tid/*ģ��ID*/, int plant_id/*����ID*/, int life_time) {}
	virtual void plant_pet_disappear(int id, char reason) {}
	virtual void plant_pet_hp_notify(int id, float hp_factor, int cur_hp, float mp_factor, int cur_mp) {}
	virtual void pet_property(int index, const extend_prop & prop, int r_def_lvl_f, int r_def_lvl_m, int r_chi_point){}
	virtual void faction_contrib_notify(){}
	virtual void faction_relation_notify(){}
	virtual void enter_factionfortress(int role_in_war, int factionid, int offense_endtime){}
	virtual void player_equip_disabled(int64_t mask){}
	virtual void send_spec_item_list(int cs_index, int cs_sid, int target, int type, void * data, unsigned int size){}
	virtual void send_error_message(int cs_index, int cs_sid, int target, int message){}
	virtual void start_play_action(char action_name[128],int play_times,int action_last_time,int interval_time){}
	virtual void stop_play_action(){}
	virtual void congregate_request(char type, int sponsor, int timeout){}
	virtual void reject_congregate(char type, int id){}
	virtual void congregate_start(char type, int time){} 
	virtual void cancel_congregate(char type){} 

	virtual void engrave_start(int recipe_id, int use_time){}
	virtual void engrave_end(){}
	virtual void engrave_result(int addon_num){}
	virtual void addonregen_start(int recipe_id, int use_time){}
	virtual void addonregen_end(){}
	virtual void addonregen_result(int addon_num){}
	virtual void invisible_obj_list(gobject ** ppObject, unsigned int count){}
	virtual void set_player_limit(int index, char b){}
	virtual void player_teleport(const A3DVECTOR& pos, unsigned short use_time, char mode){}
	virtual void forbid_be_selected(char b){}
	virtual void send_player_force_data(int cur_force, unsigned int count, const void * data, unsigned int data_size){}
	virtual void player_force_changed(int force){}
	virtual void player_force_data_update(int force, int repu, int contri){}
	virtual void send_force_global_data(char data_ready, unsigned int count, const void * data, unsigned int data_size){}
	virtual void add_multiobj_effect(int target, char type){}
	virtual void remove_multiobj_effect(int target, char type){}
	virtual void enter_wedding_scene(int groom, int bride){}
	virtual void produce4_item_info(int stime, item_data & data, unsigned short crc){} //���Ϳͻ����¼̳�������������Ʒ��Ϣ
	virtual void online_award_data(int total_award_time, unsigned int count, const void * data, unsigned int data_size){}
	virtual void toggle_online_award(int type, char activate){}
	virtual void update_profit_time(char flag, int profit_time, int profit_level){}
	virtual void notify_profit_state(char state) {}//֪ͨ����״̬;�������߹ر�
	virtual void enter_nonpenalty_pvp_state(char state){}
	virtual void self_country_notify(int country_id){}
	virtual void player_country_changed(int country_id){}
	virtual void enter_countrybattle(int role, int battle_id,int end_time,int offense_country, int defence_country){}
	virtual void countrybattle_result(int result) {}
	virtual void countrybattle_score(int oscore, int ogoal, int dscore, int dgoal) {}
	virtual void countrybattle_resurrect_rest_times(int times){}
	virtual void countrybattle_became_flag_carrier(char is_carrier){}
	virtual void countrybattle_personal_score(int combat_time, int attend_time, int kill_count, int death_count, int country_kill_count, int country_death_count){}
	virtual void defense_rune_enabled(char rune_type, char enable){}
	virtual void countrybattle_info(int attacker_count, int defender_count){}
	virtual void cash_money_exchg_rate(char open, int rate){}
	virtual void pet_rebuild_inherit_start(unsigned int index,int use_time) {}
	virtual void pet_rebuild_inherit_info(int stime,int pet_id,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/) {} 
	virtual void pet_rebuild_inherit_end(unsigned int index) {}
	virtual void pet_evolution_done(unsigned int index){}
	virtual void pet_rebuild_nature_start(unsigned int index,int use_time) {}
	virtual void pet_rebuild_nature_info(int stime,int pet_id,unsigned int index,int nature) {}
	virtual void pet_rebuild_nature_end(unsigned int index) {}
	virtual void equip_addon_update_notify(unsigned char update_type,unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type) {}
	virtual void notify_meridian_data(int meridian_level,int lifegate_times,int deathgate_times,int free_refine_times,int paid_refine_times,int continu_login_days,int trigrams_map[3]) {}
	virtual void try_refine_meridian_result(int index,int result) {}
	virtual void self_king_notify(char is_king, int expire_time){}
	virtual void player_king_changed(char is_king){}
	virtual void notify_touch_query(int64_t income,int64_t remain,int retcode) {}
	virtual void notify_touch_cost(int64_t income,int64_t remain,unsigned int cost,unsigned int index,unsigned int lots,int retcode) {}
	virtual void notify_addup_money(int64_t addupmoney) {}
	virtual void notify_giftcard_redeem(int retcode,int cardtype,int parenttype, const char(&cardnumber)[20]) {}
	virtual void query_title(int roleid,int count,int ecount,const void * data, unsigned int data_size,const void * edata, unsigned int edata_size){}
	virtual void notify_curr_title_change(int roleid,unsigned short titleid){}
	virtual void notify_title_modify(unsigned short titleid,int expiretime,char flag){}
	virtual void refresh_signin(char type,int lys, int moncal, int uptime, char latesignintimes, char awardedtimes, char awardedtimes2) {}
	virtual void player_reincarnation(unsigned int reincarnation_times){}
	virtual void activate_reincarnation_tome(char active){}
	virtual void realm_exp_receive(int exp,int receive_exp, int diary_exp){}
	virtual void realm_level_up(unsigned char level){}
	virtual void enter_trickbattle(int role, int battle_id,int end_time){}
	virtual void trickbattle_personal_score(int kill, int death, int score, int multi_kill){}
	virtual void trickbattle_chariot_info(int chariot, int energy){}
	virtual void player_leadership(int leadership, int inc_leadership){}
	virtual void generalcard_collection_data(const void * buf, unsigned int size){}
	virtual void add_generalcard_collection(unsigned int collection_idx){}
	virtual void refresh_fatering(const void * data,unsigned int datasize) {}
	virtual void broadcast_mine_gatherd(int mid, int pid, int item_type) {}
	virtual void player_active_combat(bool is_active) {}
	virtual void random_mall_shopping_result(int eid,int opt,int res,int item,int price,bool flag) {}
	virtual void player_mafia_pvp_mask_notify(unsigned char mafia_pvp_mask) {}
	virtual void player_world_contribution(int points_total, int points_rest, char reward_stage, char count, const char * values) {}
	virtual void send_scene_service_npc_list(unsigned int count, int * data) {}
	virtual void notify_visible_tid_change() {}
	virtual void player_screen_effect_notify(int type, int eid, int state) {}
	virtual void player_combo_skill_prepare(int skillid,int timestamp,int arg1, int arg2, int arg3) {}
	virtual void player_pray_distance_change(float pd) {}
	virtual void instance_reenter_notify(int tag, int timeout) {}
	virtual void astrolabe_info_notify(unsigned char level, int exp) {}
	virtual void astrolabe_operate_result(int opt, int ret, int a0, int a1, int a2) {}
    virtual void property_score_result(int fighting_score, int viability_score, int client_data) {}
    virtual void lookup_enemy_result(int rid, int worldtag, const A3DVECTOR& pos) {}
	virtual void solo_challenge_award_info_notify(int max_stage_level, int total_time, int total_score, int cur_score, int last_success_stage_level, int last_success_stage_cost_time, int draw_award_times,int have_draw_award_times, abase::vector<struct playersolochallenge::player_solo_challenge_award>& award_info){}
	virtual void solo_challenge_operate_result(int opttype, int retcode, int arg0, int arg1, int args2){}
	virtual void solo_challenge_challenging_state_notify(int climbed_layer, unsigned char notify_type){}
	virtual void solo_challenge_buff_info_notify(int *buff_index, int *buff_num, int count, int cur_score){}
	virtual void mnfaction_player_faction_info(int player_faction, int domain_id){}
	virtual void mnfaction_resource_point_info(int attacker_resource_point, int defender_resource_point){}
	virtual void mnfaction_player_count_info(int attend_attacker_player_count, int attend_defender_player_count) {}
	virtual void mnfaction_resource_point_state_info(int index, int cur_degree) {}
	virtual void mnfaction_resource_tower_state_info(int num, MNFactionStateInfo& mnfaction_state_info) {}
	virtual void mnfaction_switch_tower_state_info(int num, MNFactionStateInfo& mnfaction_state_info) {}
	virtual void mnfaction_transmit_pos_state_info(int num, MNFactionStateInfo& mnfaction_state_info) {}
	virtual void mnfaction_result(int result) {}
	virtual void mnfaction_battle_ground_have_start_time(int battle_ground_have_start_time){}
	virtual void mnfaction_faction_killed_player_num(int attacker_killed_player_count, int defender_killed_player_count){}
	virtual void mnfaction_shout_at_the_client(int type, int args){}
	virtual void fix_position_transmit_add_position(int index, int world_tag, A3DVECTOR &pos, unsigned int position_length, const char *position_name){}
	virtual void fix_position_transmit_delete_position(int index){}
	virtual void fix_position_transmit_rename(int index, unsigned int position_length, char *position_name){}
	virtual void fix_position_energy_info(char is_login, int cur_energy){}
	virtual void fix_position_all_info(fix_position_transmit_info *info){}
	virtual void cash_vip_mall_item_buy_result(char result, short index, char reason){}
	virtual void cash_vip_info_notify(int level, int score){}
	virtual void purchase_limit_all_info_notify(){}
	virtual void purchase_limit_info_notify(int limit_type, int item_id, int have_purchase_count){}
    virtual void cash_resurrect_info(int cash_need, int cash_left) {}
	virtual void body_scale_notify(int mode, float scale) {}
	virtual void filter_soul_info(int soul) {}
	virtual void home_task_info( const void *buf, unsigned int size, int task_trigger_times, int task_refresh_times) {}
	
	virtual void get_lotery_info( unsigned int manager, unsigned int score, unsigned int count, unsigned int * item ) {}		
	virtual void lottery_reward_info( unsigned int unk /* 14 */, unsigned int score, unsigned int count, unsigned int itemid ) {}				 
	virtual void get_lotery_items( unsigned int count, unsigned int * item ) {}
	virtual void get_treasure_info( unsigned char manager, unsigned int score, unsigned int reserve, unsigned int lot_count, unsigned int box_count, void * _lot, void * _box ) {}
	virtual void refine_bible_result(int rst) {}
	virtual void transfer_refine_bible(int rst1, int rst2, int item_pos) {}
	virtual void get_storage_points(short rst1, short rst2) {}
	virtual void get_finish_notify(int rst1, int rst2) {}

	//virtual void glyph_info( unsigned int count, void * item ) {}
	virtual void glyph_slot_info( int reserve1, int count, int pvp_point ) {}
	virtual void get_glyph_skill( int mode, int pos, int skill, int item ) {}
	virtual void get_glyph_upgrade( int mode, int result, int pos,  int new_item, int old_item ) {}
	
	virtual void engrave_new_info(int addon_count, unsigned int * engraves){} // Nova Gravura

	virtual void get_lib_items( unsigned int count, unsigned int * item ) {} // Lib Itens
	virtual void result_lib_produce(){}	
	virtual void material_refine_result(int reserve, int rst) {}

	virtual void get_server_time( unsigned int time ) {}
	virtual void get_weapon_vstate( char state ) {}
	virtual void get_shield_energy( short shgen, short shmax ) {}

	/*166+*/
	virtual void pet_skill_data( unsigned int count, int index, unsigned int * skills ) {}
	virtual void pet_skin_info( int index, int skin_pet_id ) {}
	virtual void pet_skin_storage( unsigned int count, unsigned int * values ) {}

	/*156+*/
	virtual void player_carrier_up(int pos, int reserve1, unsigned int npcid) {}
	virtual void player_carrier_down(int pos, unsigned int npcid) {}
	virtual void player_carrier_self_up(int pos, int count, unsigned int npcid, int res, float speed_walk, float speed_run, float speed_swim, float speed_fly) {}
	virtual void carrier_sync_info(unsigned int npcid, int reserve1, int reserve2) {}
	virtual void carrier_prop(unsigned int npcid, int reserve1, unsigned int carrier_data) {}
	virtual void carrier_skill_cooldown(unsigned int skill_id, int cooldown) {}

	/*156+*/
	virtual void get_imperial_spirit(int enabled, unsigned int expired_time) {}


	/*160+*/
	virtual void player_repository_info(unsigned int roleid, unsigned int storage_itens, int itens_storage[], int used_itens, int itens_active[]) {}
	virtual void player_repository_storage(unsigned int count_values, int itens[]) {}
	
	/*171+*/
	virtual void dynamic_genesis_level(unsigned int count, const void * skills_array) {}

	// Novas cargas do mercenário
	virtual void get_charge_merc( char charges) {}
	
	// Novo Range Glyph
	virtual void skill_glyph_info( int type, unsigned int count, unsigned int * values ) {}

	virtual void player_world_speak_info( char enabled, char enabled2, char kid, int skills_count, int * skills ) {}

	// Anedota
	virtual void get_anecdote_info(short uTask1, short uTask2, short uTask3, short uTask4, short uTask5, short uTask6, short uTask7, short uTask8, short uTask9, short uTask10, short uTask11, short uTask12, short uTask13, short uTask14, short uTask15, short uTask16) {}

	// Segment 
	virtual void skill_cooldown_limit(int skill_id , unsigned int skill_last_use_time , unsigned int skill_cooling_sec , unsigned short count , unsigned short max_count) {}

	// new money 172+
	virtual void pickup_gold(unsigned int value) {}
	virtual void pickup_silver(unsigned int value) {}
	/*156+*/
	virtual void arena_team_invite(int inviting_id, int team_type) {}
	virtual void arena_battle_group_info(unsigned int time_rest, int red_team_count_rest, int blue_team_count_rest, int team_count_members_red, int team_count_members_blue, int members_red[], int members_blue[]) {}
	virtual void arena_player_faction_info(int team_id, int type, int time_rest) {}

	/*170+ Códice*/
	virtual void codex_fashion_activate(int roleid, int mode_classic, unsigned int size, const void * fashion_activate) {}
	virtual void codex_fashion_storage(int skins_count, unsigned int size,  const void * skins, int fashion_points) {}
	virtual void codex_fashion_dye(int mode, int count_fashions, unsigned int size, const void * fashion_dye) {}
	virtual void codex_reward_title(int type, int pos) {}
	virtual void codex_reward_first(int type, int reward_idx, int result) {}
	virtual void codex_pet_consum_info(int pet_egg_id, int res, short res2, int mount_points) {}
	virtual void codex_fly_consum_info(int fly_id, char res, int speed, int fly_points) {}
	virtual void codex_improve_info(int fly_id, char apprimored_level, int max_time_speed) {}
	virtual void codex_fly_equip_info(short index_equip, char index_inv, char where_equip, int pos_codex_inv, int fly_item_id, int speed, char apprimored_level) {}
	virtual void codex_rename_pet_info(int pet_id ,const char * name, short name_len) {}

	/*170+ Bebe Celestial*/
	virtual void kid_course_change(char old_slot, char new_slot) {}
	virtual void kid_course_remove(char old_slot) {}
	virtual void kid_name_awakening(short name_len, const char * name) {}
	virtual void kid_course_info(unsigned int * course_info, int count) {}
	virtual void kid_course_perc(char level, int reserved) {}
	virtual void kid_course_insert(char old_slot, char new_slot) {}
	virtual void kid_course_switch(char new_slot, char old_slot1, char old_slot2) {}
	virtual void kid_awakening_points(int points) {}
	virtual void kid_created_info_dialog() {}
	virtual void kid_awakening_info(unsigned int size, const void * kid_awakening_info){}
	virtual void kid_awakening_cash_info(int awakening_cash, int awakening_potential) {}
	
	virtual void kid_celestial_info(unsigned int size, const void * kid_info){}
	virtual void kid_active_info(int active_slot, int reserved) {}
	virtual void kid_award_addon(unsigned int size, const void * info){}
	virtual void kid_celestial_awakening(int type, int reserve){}
	virtual void kid_celestial_transformation(int shape, int roleid, int reserve, int reserve2){}
	/*174*/
	virtual void reward_interface_notify(unsigned int mode, unsigned int value_index, int count) {}
	virtual void activity_event_spend_cash(unsigned int roleid, unsigned int cash) {}
	virtual void activity_event_shop(int count, char unk6, unsigned int * values) {}

	// Memorial Celestial
	virtual void celestial_memorial_info(bool type, unsigned int size, const void * info,unsigned int size2, const void * info2, unsigned int size3, const void * info3) {}
	virtual void celestial_memorial_lottery( int count, unsigned int size, const void * lotterys) {}
	virtual void celestial_memorial_lottery_count( int perg1, int perg2, int perg3) {}
	// G17
	virtual void armor_info_notify(unsigned int reserved, unsigned int count, int reserved3, int reserved4, int inv_slot) {}
	virtual void armor_info_purification(int count, unsigned int * values) {}

	/*176+*/
	virtual void portatil_picture_info(int roleid, int res, int res2, int idx, int res3) {}
	virtual void portatil_picture_storage(int count, int storage[4]) {}
	virtual void kid_system_points_notify(int points) {}

public:
	void MoveBetweenSlice(slice * pPiece,slice * pNewPiece,const A3DVECTOR &pos);
};


class gobject_imp : public substance
{
protected:

public:
DECLARE_SUBSTANCE(gobject_imp);
public:
	world * _plane;
	gobject * _parent;

	controller * _commander;
	dispatcher * _runner;
public:
	gobject_imp():_plane(NULL),_parent(NULL),_commander(0),_runner(0){}
	virtual void Init(world * pPlane,gobject*parent)
	{
		_plane = pPlane;
		_parent = parent;
	}
	virtual void ResetPlane(world * new_pPlane)
	{
		_plane = new_pPlane;
	}
	virtual ~gobject_imp(){}
	/**
	 *	�����������ʱ��_parent�Ѿ������������ٴη���send_message��post_messageʱҪע���ȿ���,
	 *	����ֵ�����ʾ����Ҫ�ٴθ�֪���治��Ҫ�ٿ���
	 *	�����������(δ����)һ��Ҫ����0
	 */
	virtual  int MessageHandler(world * pPlane ,const MSG & msg)
	{
		return 0;
	}
	
	typedef int (*msg_filter)(world * pPlane, const MSG & msg);
	/*
	 * 	 ���ÿ��ܵ���Ϣ��������һ������ͬʱֻ����һ����Ϣ������
	 * 	 ������������ΪӦ�ñ�controller�������
	 */
	msg_filter SetMsgFilter(msg_filter filter);
	

	/**
	 *	�����ƶ��ĺ���������бȽϸ��ӵĲ���������ʱ�ӽ����ƶ��жϡ�
	 */
	virtual int PrepareMove(const A3DVECTOR &dest) {return 0;};	//�����ƶ��������붨ʱ�������������ƶ�


	/**
	 *	���¸�����������в���״̬��
	 *	������״̬�ظ�
	 */
	virtual void Reborn()
	{
		ASSERT(false);
	}

	/*
	 *	�����Ƿ���Կ�ʼ���ף�ֻ��player ���Խ���
	 */
	virtual bool CanTrade(const XID & target)
	{
		return false;
	}

	/**
	 *	��ʼ���ף�ֻ��player ��������������
	 *
	 */
	virtual void StartTrade(int trade_id,const XID & target)
	{
		return ;
	}

	/*
	 *	��ʼ���ɽ���(���ɷ���),	
	 */
	 virtual bool StartFactionTrade(int trade_id,int get_mask, int put_mask,bool no_response = false)
	 {
	 	ASSERT(false);
	 	return false;
	 }

	 /*
	  *	���ɽ�����ɣ�ͬʱ���л�д���� ���������д������
	  */
	 virtual void FactionTradeComplete(int trade_id, const GNET::syncdata_t &data)
	 {
	 	ASSERT(false);
	 	return ;
	 }
	 
	 /*
	 	������ɣ�ͬʱ���л�д����
	 */
	 virtual void SyncTradeComplete(int trade_id, unsigned int money, const GDB::itemlist & item_change,bool write_trashbox, bool is_write_shoplog)
	 {
	 	ASSERT(false);
		return;
	 }

	/**
	 *	�������ݻ�д�ɹ�����ʧ��(�ڵȴ����׽׶ε���)
	 */
	virtual void WaitingTradeWriteBack(int trade_id, bool bSuccess)
	{
		return;
	}

	/*
	 *	
	 *	���׷����������˽�����ɵ���Ϣ ���ɹ�����ʧ�ܣ�
	 */
	virtual void TradeComplete(int trade_id,int reason, bool need_read){}

	
	/*
	*	ȡ���л��������Ĳ���
	*/
	virtual void CancelSwitch(){}

	/**
		���°�����Ϣ
	*/

	virtual void UpdateMafiaPvP(unsigned char pvp_mask)
	{
		//do nothing
	}

	virtual void UpdateMafiaInfo(int id, char rank, unsigned char pvp_mask, int64_t unifid)
	{
		//do nothing
	}

	virtual void UpdateFactionRelation(int * alliance, unsigned int asize, int * hostile, unsigned int hsize, bool notify_client)
	{
		//do nothing
	}
	
	/*
	*	���ǿ�����ߵĲ���
	*/
	virtual void PlayerForceOffline(){}

	/*
	*	�رյĲ���
	*/
	virtual void ServerShutDown(){}

	virtual bool CanTeamRelation() { return false;}
	
public:
	/**
	 *	�ַ������������Ҫ�󽫱������������ٵ���
	 *	�� ��Ϣ�������ƣ���������˷�0����ô��ʾ���治��Ҫ����
	 */
	virtual int DispatchCommand(int cmd_type, const void * buf,unsigned int size)
	{
		ASSERT(_commander);
		return _commander->CommandHandler(cmd_type,buf,size);
	}

	virtual int DispatchMessage(world * pPlane ,const MSG & msg)
	{
		return MessageHandler(pPlane,msg);
	}

	void EnterWorld()
	{
		_runner->begin_transfer();
		_runner->get_base_info();
		_runner->query_info00();
		_runner->enter_world();
		_runner->end_transfer();
	}

	virtual void CheckNPCData() {}

	virtual void OnNpcEnterWorld(){}
	virtual void OnNpcLeaveWorld(){}
	
//protected:
public:
	/*
	 * ��ʵ�ƶ������ƶ�һ����С�������ƶ������������Ļ�
	 * �ᶪʧ��ԭ������������뿪��Ϣ���Ժ���Կ��Ǽ��ϴ��ж�
	 * ����ֵ��ʾ�Ƿ�������ƶ�
	 */
	virtual bool StepMove(const A3DVECTOR &offset);

	/*
	 * ��������ж������Ƿ���Է����ƶ������Բ��������������ֱ�ӵ���StepMove
	 * ����������ڲ������������ƶ����ж��Ƿ�����ƶ�
	 */
	virtual bool CanMove() { return true; }

	/*
	 * ���з��������л�
	 */
	virtual void SwitchSvr(int dest,const A3DVECTOR &oldpos , const A3DVECTOR &newpos,const instance_key *)
	{
		ASSERT(false);
	}
	friend class gobject;

};

gobject_imp * 	CF_CreateImp(int guid,world * plane,gobject * obj);
dispatcher * 	CF_CreateDispatcher(int guid,gobject_imp*);
controller*	CF_CreateCtrl(int guid,gobject_imp*);

inline 	gobject_imp * CF_Create(int guid_imp,int guid_dis,int guid_ctrl,world *plane,gobject *obj)
{
	gobject_imp * imp = CF_CreateImp(guid_imp,plane,obj);
	ASSERT(imp);
	if(!imp) return NULL;
	controller *ctrl = CF_CreateCtrl(guid_ctrl,imp);
	dispatcher *runner = CF_CreateDispatcher(guid_dis,imp);
	ASSERT(ctrl && runner);
	if(!ctrl || !runner)
	{
		delete ctrl;
		delete runner;
		delete imp;
		return NULL;
	}
	imp->_runner = runner;
	imp->_commander = ctrl;
	return imp;
}

#endif
