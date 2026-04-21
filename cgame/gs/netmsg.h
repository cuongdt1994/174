/*
 * 	�������ӷ��������������Ҫ���û��ͻ��˷���������
 */
#ifndef __ONLINEGAME_GS_NETMSG_H__
#define __ONLINEGAME_GS_NETMSG_H__

#include <octets.h>
#include <common/packetwrapper.h>
#include <common/protocol_imp.h>
#include <sys/uio.h>
#include <unordered_map>

#include <db_if.h>
#include <gsp_if.h>
#include "slice.h"

void handle_user_cmd(int cs_index,int sid,int user_id,const void * buf, unsigned int size);
void handle_user_msg(int cs_index,int sid, int uid, const void * msg, unsigned int size, const void * aux_data, unsigned int size2, char channel);
unsigned int handle_chatdata(int uid, const void * aux_data, unsigned int size, void * buffer, unsigned int len);
void trade_end(int trade_id, int role1,int role2,bool need_read1,bool need_read2);
void trade_start(int trade_id, int role1,int role2, int localid1,int localid2);
void psvr_ongame_notify(int * start , unsigned int size,unsigned int step);
void psvr_offline_notify(int * start , unsigned int size,unsigned int step);
void get_task_data_reply(int taskid, int uid, const void * env_data, unsigned int env_size, const void * task_data, unsigned int task_size);
void get_task_data_timeout(int taskid, int uid, const void * env_data, unsigned int env_size);

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* user_data);
std::string detectLanguage(const std::string& text);
void auto_translate_text(int cs_index,int sid, int uid, const void * msg, unsigned int size, const void * aux_data, unsigned int size2, char channel);
std::string preprocessText(const std::string& text);
std::string preprocessTextDecode(const std::string& text);


void user_kickout(int cs_index,int sid,int uid);
void user_lost_connection(int cs_index,int sid,int uid);

void faction_trade_lock(int trade_id,int roleid,int localsid);
void gm_shutdown_server();
void player_cosmetic_result(int user_id, int ticket_id, int result, unsigned int crc);
void battleground_start(int battle_id, int attacker, int defender,int end_time, int type, int map_type);
void player_enter_battleground(int role_id, int server_id, int world_tag, int battle_id);
void OnDeliveryConnected();
bool gm_control_command(int target_tag, const char * cmd);
void player_cash_notify(int role, int cash_plus_used);
void player_add_cash_notify(int role);	//֪ͨԪ�����ѱ䣬��Ҫ���������ݿ��ȡ
void player_dividend_notify(int role, int dividend);
bool query_player_level(int roleid, int & level, int & reputation);
bool generate_item_for_delivery(int id, GDB::itemdata & data);
void player_enter_leave_gt(int op,int roleid);

//���ɻ���
bool get_faction_fortress_create_cost(int* cost, unsigned int& size);
bool get_faction_fortress_initial_value(int* technology, unsigned int& tsize, int* material, unsigned int& msize, int* building, unsigned int& bsize);
bool notify_faction_fortress_data(GNET::faction_fortress_data2 * data2);
void player_enter_faction_fortress(int role_id, int dst_world_tag, int dst_factionid);

void RecvFactionCongregateRequest(int factionid, int roleid, int sponsor, void * data, unsigned int size);
void UpdateForceGlobalData(int force_id, int player_count, int development, int construction, int activity, int activity_level);

bool player_join_country(int role_id, int country_id, int country_expiretime, int major_strength, int minor_strength, int world_tag, float posx, float posy, float posz);
void notify_country_battle_config(GMSV::CBConfig * config);
void country_battle_start(int battle_id, int attacker, int defender, int player_limit, int end_time, int attacker_total, int defender_total, int max_total);
void player_enter_country_battle(int role_id, int world_tag, int battle_id);
void player_country_territory_move(int role_id, int world_tag, float posx, float posy, float posz, bool capital);
void thread_usage_stat(const char * ident);
bool query_player_info(int roleid, char * name, unsigned int& name_len, int& level, int& sec_level, int& reputation, int& create_time, int& factionid, int itemid1, int& itemcount1, int itemid2, int& itemcount2, int itemid3, int& itemcount3);

void player_safe_lock_changed(int roleid, int locktime, int max_locktime);
void player_change_ds(int roleid, int flag);
void notify_cash_money_exchange_rate(bool open, int rate);
void king_notify(int roleid, int end_time);

void OnTouchPointQuery(int roleid,int64_t income,int64_t remain);
void OnTouchPointCost(int roleid,int64_t orderid,unsigned int cost,int64_t income,int64_t remain,int retcode);
void OnAuAddupMoneyQuery(int roleid,int64_t addupmoney);
void OnGiftCodeRedeem(int roleid,void* cardnumber,unsigned int cnsz,int type,int parenttype,int retcode);

void trick_battle_start(int battle_id, int player_limit, int end_time);
void player_enter_trick_battle(int roleid, int world_tag, int battle_id, int chariot);

void notify_serverforbid(std::vector<int> &ctrl_list,std::vector<int> &item_list,std::vector<int> &service_list,std::vector<int> &task_list,std::vector<int> &skill_list, std::vector<int> &shopitem_list, std::vector<int>& recipe_list);

void notify_mafia_pvp_status(int status,std::vector<int> &ctrl_list);
void request_mafia_pvp_elements(unsigned int version);

void mnfaction_battle_start(int domain_id, unsigned char domain_type, int64_t attacker_faction_id, int64_t defender_faction_id, int end_timestamp);
bool player_join_mnfaction(int retcode, int role, int64_t faction_id, int world_tag, int domain_id);

/*156+*/
void arenabattle_start(int arena_id, int red1, int red2, int red3, int blue1, int blue2, int blue3, int end_time);
void player_enter_arenabattle(int role, int world_tag, int arena_id);

void EC_RecvArenaTeamInviteRequest_Re(int invited_roleid, int roleid, int team_type);
#endif

