#ifndef __GS_PROTO_IF_H__
#define __GS_PROTO_IF_H__

#include <string>
#include <vector>

#include "privilege.hxx"

class object_interface;

void user_login(int cs_index,int sid,int uid,const void * auth_buf, unsigned int auth_size);
void report_cheater(int roleid, int cheattype, const void *cheatinfo, unsigned int size);
void acquestion_ret(int roleid, int ret); // ret: 0 ?y����, 1 �䨪?��, 2 3?����
void player_rename_ret(int roleid, const void *new_name, unsigned int name_len, int ret);
void player_change_gender_ret(int roleid, int ret);
void player_change_gender_logout(int roleid);


namespace GNET
{
class Protocol;
class EC_SQLArenaTeam;
class EC_SQLArenaPlayer;
class EC_SQLArenaTeamMember;
}

namespace GDB
{
class itemlist;
}

namespace GMSV
{
	enum
	{
		PLAYER_LOGOUT_FULL  =  0,
		PLAYER_LOGOUT_HALF  =  1,
	};
	enum
	{
		CHAT_CHANNEL_LOCAL = 0,		//��ͨƵ��
		CHAT_CHANNEL_FARCRY ,		//����Ƶ��
		CHAT_CHANNEL_TEAM ,			//����Ƶ��
		CHAT_CHANNEL_FACTION ,		//����Ƶ��
		CHAT_CHANNEL_WHISPER ,		//������Ϣ
		CHAT_CHANNEL_DAMAGE,		//�˺���Ϣ
		CHAT_CHANNEL_FIGHT,			//ս����Ϣ
		CHAT_CHANNEL_TRADE ,		//����Ƶ��
		CHAT_CHANNEL_SYSTEM,		//ϵͳ��Ϣ
		CHAT_CHANNEL_BROADCAST,		//�㲥��Ϣ
		CHAT_CHANNEL_MISC,			//������Ϣ
		CHAT_CHANNEL_INSTANCE,		//����Ƶ��
		CHAT_CHANNEL_SUPERFARCRY,	//��������Ƶ��
		CHAT_CHANNEL_BATTLE,		//ս������(�������Ƶ��(����))
		CHAT_CHANNEL_COUNTRY,		//����
		CHAT_CHANNEL_GLOBAL,		//ȫ��Ƶ��
		CHAT_CHANNEL_ALLIANCE,		//= 16	--���ݧڧѧߧ�
		CHAT_CHANNEL_CENTER,		//= 17	--�� ��֧ߧ��� ��ܧ�ѧߧ�
	};
	enum
	{
		CMSG_FF_LEVELUP 			= 37,	//���ɻ�������
		CMSG_FF_TECHNOLOGYUP		= 38,	//���ɻ��ؿƼ�����
		CMSG_FF_CONSTRUCT			= 39,	//���ɻ��ؽ���
		CMSG_FF_CONSTRUCTCOMPLETE 	= 40,	//���ɻ��ؽ������
		CMSG_FF_HANDINCONTRIB		= 41,	//���ɻ����Ͻ�����
		CMSG_FF_HANDINMATERIAL		= 42,	//���ɻ����Ͻ�����
		CMSG_FF_BATTLEPREPARECLEAR 	= 43,	//���ɻ���ս��ǰ�峡
		CMSG_FF_KEYBUILDINGDESTROY 	= 44,	//���ɻ��شݻ������ؽ���
		CMSG_FF_DISMANTLE			= 45,	//���ɻ�����ʩ���
	};

	struct city_entry
	{
		int level;
		int owner;
	};

	
	void SetCityOwner(int id, int level,int owner);
	int GetCityOwner(unsigned int id);
	int GetCityLevel(unsigned int id);
	int GetMafiaCityCount(int mafiaid);
	int GetFactionBonus(int faction);
	void RefreshCityBonus();
	
//	typedef std::vector< > player_list;
	typedef std::pair<int /*userid*/,int /*sid*/>  puser;

	void StartPollIO();	//��ʼ����PollIO���������������ס��һֱִ��
	bool InitGSP(const char* conf,int gid, int worldtag, float x_min, float x_max, float z_min, float z_max, const char *version);
	bool SendClientData(int lid, int userid,int sid,const void * buf, unsigned int size);
	bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, unsigned int size,int except_id);
	bool MultiSendClientData(int lid, const puser * first, const puser * last,const void * buf, unsigned int size);
	bool SendLoginRe(int lid, int userid, int sid,int result, char flag);
	bool SendKickoutRe(int userid, int sid , int result);
	bool SendDisconnect(int lid, int userid,int sid, int result);
	bool SendLogout(int lid, int userid, int sid,int type=PLAYER_LOGOUT_FULL);
	bool SendOfflineRe(int lid,int userid,int sid, int result);
	bool SendPlayerInfoUpdate(int roleid, int level);
	bool SendSNSRoleBrief(int roleid, object_interface obj_if);
	void DiscardTrade(int trade_id, int userid);
	void ReplyTradeRequest(int trade_id, int userid,int localsid,bool isSuccess);
	bool SendPlayerTeamOp(char operation,int64_t team_uid,int captain,std::vector<int> members);
	bool SendPlayerTeamMemberOp(int64_t team_uid,char operation,int member);
	
	struct chat_msg
	{
		int speaker;
		const void * msg;
		unsigned int size;
		const void * data;
		unsigned int dsize;
		char channel;
		char emote_id;
		int speaker_level;
	};
	bool MultiChatMsg(int lid,const puser * first, const puser * last, const chat_msg & chat,int except_id);
	bool SendChatMsg(int lid, int userid, int sid,const chat_msg & chat);
	bool BroadChatMsg(const chat_msg &chat);
	bool SetChatEmote(int userid, char emote_id);
	bool SystemChatMsg(const void * msg, unsigned int size, char channel,	const void * data = NULL, unsigned int dsize = 0);
	bool CountryChatMsg(const chat_msg &chat);
	bool FactionBroadcastMsg(int fid, int type, const void * msg, unsigned int size);
	bool SendPlayerHeartbeat(int lid, int userid, int localsid);
	bool SendSwitchServerSuccess(int lid, int userid, int localsid,int gs_id);
	bool SendSwitchServerTimeout(int lid, int userid, int localsid);
	bool SendSwitchServerCancel(int lid, int userid, int localsid);
	bool SendSwitchServerStart(int lid, int userid, int localsid,int src_gsid, int dst_gsid,const void * key,unsigned int key_size);
	void SendCosmeticRequest(int userid, int ticket_index, int ticket_id);
	void CancelCosmeticRequest(int userid);
	void SendDebugCommand(int roleid, int type, const char * buf, unsigned int size);
	void SetCouple(int id1, int id2, int op); //op = 1 marry op = 0 unmarry
	void SendRefCashUsed(int roleid, int cash_used, int level);
	void SendTaskReward(int roleid, int bonus_add);	//���������Ӻ���
	bool SendFactionCongregateRequest(int fid, int sponsor, void* data, unsigned int size);
	void SendPlayerJoinOrLeaveForce(int force_id, bool is_join);
	void SendIncreaseForceActivity(int force_id, int activity);
	void SendSynMutaData(int roleid,int level,int reincarnation_times); 
	struct CBApplyEntry
	{
		int roleid;
		int major_strength;
		int minor_strength;
	};
	void CountryBattleApply(CBApplyEntry * list, unsigned int count);
	void CountryBattleJoin(int roleid, int country_id, int worldtag, int major_strength, int minor_strength, char is_king);
	void CountryBattleLeave(int roleid, int country_id, int major_strength, int minor_strength);
	void CountryBattleOnline(int roleid, int country_id, int worldtag, int minor_strength, char is_king);
	void CountryBattleOffline(int roleid, int country_id);
	void CountryBattleEnterMap(int roleid, int worldtag);
	void SendCountryBattleServerRegister(int type/*0�׶� 1ս��*/, int world_index, int world_tag, int battle_type);
	struct CBConfig
	{
		unsigned int capital_count;
		struct CountryCapital
		{
			int country_id;
			int worldtag;
			float posx;
			float posy;
			float posz;
		}capital_list[];
	};
	void ResponseCountryBattleStart(int battle_id, int world_tag, int retcode, int defender, int attacker);
	struct CBPersonalScore
	{
		int roleid;
		int cls;
		int minor_strength;
		int combat_time;
		int attend_time;
		int kill_count;
		int death_count;
		int score;
		int dmg_output;
		int dmg_output_weighted;
		int dmg_endure;
		int kill_count_weighted;
	};
	void SendCountryBattleEnd(int battle_id, int result, int defender, int attacker, CBPersonalScore* dscore, unsigned int dcount, CBPersonalScore* ascore, unsigned int acount);

	bool SendPlayerRename(int roleid, int item_pos, int item_id, int item_num, const void * new_name, unsigned int new_name_len, object_interface & obj_if);

    bool SendPlayerChangeGender(int roleid, int item_pos, int item_id, int item_num, unsigned char new_gender, const void* custom_data, unsigned int custom_data_len, object_interface& obj_if);

    bool SendUpdateSoloChallengeRank(int roleid, int total_time);
    bool SendGetSoloChallengeRank(int roleid, char ranktype, char cls);
    bool SendUpdateEnemyList(char optype, int srcroleid, int dstroleid);

	bool SendPlayerGivePresent(int roleid, int target_roleid, unsigned int cash_cost, char has_gift, int log_price1,
		   	int log_price2, int mail_id, GDB::itemlist & ilist, object_interface & obj_if);

	bool SendPlayerAskForPresent(int roleid, int target_roleid, int goods_id, int goods_index, int goods_slot);

	bool InitMatcher(const char * file, const char * in_code, const char * check_code, const char * table_code);
	bool CheckMatcher(char * str, unsigned int size);

	bool SendPlayerQueryTouchPoint(int roleid);
    bool SendPlayerCostTouchPoint(int roleid,int64_t orderid,unsigned int cost);
	bool SendPlayerGiftCodeRedeem(int roleid,const char (&cardnumber)[20]);
	
	enum	//��localmacro.h�����һ��
	{
		CHDS_FLAG_DS_TO_CENTRALDS = 1,
		CHDS_FLAG_CENTRALDS_TO_DS = 2,
		CHDS_FLAG_DIRECT_TO_CENTRALDS = 3,
	};
	bool SendTryChangeDS(int roleid, int flag, short type, int64_t mnfid);
	bool SendPlayerChangeDSRe(int retcode, int roleid, int flag);
	void SendMobileServerRegister(int world_index, int world_tag);

	void InitUniqueData(int world_tag,int version);
	//  �޸�unique ���ݵĽӿڣ����²�������������
	void ModifyUniqueData(int key,int type,const void* odata, unsigned int osz,const void* ndata,unsigned int nsz,int world_tag, bool exclusive, bool broadcast,int version,bool timeout);

	struct AutoTeamConfig
	{
		unsigned int goal_cnt;
		
		struct OccupationInfo
		{
			char occupation;
			char need_cnt;
		};
		
		struct Goal
		{
			int id;
			unsigned char need_player_cnt;
			unsigned char occupation_list_size;
			OccupationInfo* occupation_list;
		} goal_list[];
	};
	void SendAutoTeamData(const AutoTeamConfig* data);
	void SendPlayerSetAutoTeamGoal(int roleid, char goal_type, char op, int goal_id);
	void SendAutoTeamPlayerReady_Re(int roleid, int leader_id, char retcode);

	void SendTrickBattleServerRegister(int world_index, int world_tag);
	void ResponseTrickBattleStart(int battle_id, int world_tag, int retcode);
	void SendTrickBattleEnd(int battle_id, int world_tag);
	struct TBPersonalScore
	{
		int roleid;
		int kill_count;
		int death_count;
		int score;
	};
	void SendTrickBattlePersonalScore(int battle_id,  int world_tag, TBPersonalScore * list, unsigned int size);
	void SendTrickBattleApply(int roleid, int chariot);
	void SendTrickBattleEnter(int roleid, int battle_id, int world_tag);
	void SendTrickBattleLeave(int roleid, int battle_id, int world_tag);

	struct MafiaMemberInfo
	{
		int roleid;
		unsigned char rank;
	};
	void SendMafiaPvPEvent(int type, int mafia_src, int mafia_dest , int roleid, unsigned char rank, int domainid, std::vector<MafiaMemberInfo>* memberlist = NULL);
	void SendMafiaPvPRegister(int serverid,int worldtag);
	void SendMafiaPvPQuery(int roleid, int mafiaid);
	struct MPDomainConfig
	{
		unsigned short domain_count;
		unsigned short minebase_count;
		unsigned int bonus_base;
		unsigned int bonus_minecar;
		unsigned short rob_minebase_limit;
		unsigned short rob_minecar_limit;
	};
	void SendMafiaDomainConfig(MPDomainConfig* dlist, unsigned int dsize, int* clist, unsigned int csize);


	//�������ս�������
	struct MnfactionEnterEntry
	{
		int roleid;
		int64_t faction_id;
		int domain_id;
	};
	void MnfactionEnter(MnfactionEnterEntry *ent);
	void ResponseMnfactionBattleStart(int ret_code, int domain_id, int world_tag);
	void MnfactionLeaveNotify(int roleid, int64_t faction_id, int domain_id);
	void SendMNFactionServerRegister(int world_index, int world_tag);
	void MNDomainBattleLeaveNotice(int roleid, int64_t unifid, int domain_id);
	int MNDomainBattleEnterSuccessNotice(int roleid, int64_t unifid, int domain_id);
	void SendMnfactionBattleEnd(int domain_id, int64_t win_unifid);

	struct mnfaction_domain_entry
	{
		int _domain_id;
		unsigned char _domain_type;
		int64_t _owner_unifid;
		int64_t _attacker_unifid;
		int64_t _defender_unifid;
		int _expiretime;
	};
	void SetMnDomain(int domain_id, unsigned char domain_type, int64_t owner_unifid, int64_t attacker_unifid, int64_t defender_unifid/*, int expiretime*/);
	int GetMnDomainCount(int64_t unifid, unsigned char domain_type);
	int64_t GetMnDomainOwner(int domain_id);

	int MnFactionSignUp(unsigned char domain_type, int id_mafia, int64_t unifid, object_interface& obj_if, int roleid, unsigned int cost);
	void MnFactionRank(int roleid);
	void MnFactionGetDomainData(int roleid);

	/*
		����ĺ���һ�㲻��Ҫ����
	*/
	bool SendToLS(int lid, GNET::Protocol * p);

	void SetTaskData(int taskid, const void * buf, unsigned int size);
	void GetTaskData(int taskid, int uid, const void * env_data, unsigned int env_size);

	void ReportCheater2Gacd(int roleid, int cheattype, const void *buf, unsigned int size);
	void TriggerQuestion2Gacd(int roleid);

	void ThreadUsageStat();
	
	void SendArenaBattleEnd(int battle_id , char battle_mode, char red_win, int score);
	
	void SendArenaJoin( int roleid , char mode, int * team, unsigned int count , int strong );
	void SendArenaQuit( int roleid);
	
	void SendPlayerLuaInfo( int roleid, int gs_id, unsigned int localsid, void *buf, unsigned int size);
	
	// ec arena 
	
	struct EC_GSArenaTeamMember
	{
		int player_id;
		short award_count;
	};
	
	struct EC_GSArenaTeam
	{
		int team_id;
		int captain_id	;
		char team_type	;
		int score	;
		int last_visite	;
		int win_count	;
		int team_score	;
		int week_battle_count	;
		int create_time	;
		int battle_count	;
		char name[40];
		int name_len	;	
		std::vector <EC_GSArenaTeamMember> members;
		std::vector <int> teams;
		
		EC_GSArenaTeam()
		{
			team_id = 0;
			captain_id = 0;
			team_type = 0;
			score = 0;
			last_visite = 0;
			win_count = 0;
			team_score = 0;
			week_battle_count = 0;
			create_time = 0;
			battle_count = 0;
			name_len = 0;
			memset(name, 0x00, sizeof(name));
			members.clear();
			teams.clear();
		}
		
		~EC_GSArenaTeam()
		{
			members.clear();
			teams.clear();
		}
		
		void GSConvert( GNET::EC_SQLArenaTeam * pTeam);
		void GNETConvert( GNET::EC_SQLArenaTeam * pTeam);
	};

	struct EC_GSArenaPlayer
	{
		int player_id;
		int team_id;		
		char cls;		
		int score;	
		int win_count;	
		int team_score;	
		int week_battle_count;
		int invite_time;		
		int last_visite;		
		int battle_count;	
		int name_len;
		char name[40];
		
		EC_GSArenaPlayer()
		{
			player_id = 0;
			team_id = 0;		
			cls = 0;		
			score = 0;	
			win_count = 0;	
			team_score = 0;	
			week_battle_count = 0;
			invite_time = 0;		
			last_visite = 0;		
			battle_count = 0;
			name_len = 0;			
			memset(name, 0x00, sizeof(name));
		}
		
		~EC_GSArenaPlayer()
		{
			
		}
		
		void GSConvert( GNET::EC_SQLArenaPlayer * pPlayer);
		void GNETConvert( GNET::EC_SQLArenaPlayer * pPlayer);
	};

	struct EC_GSArena
	{
		int arena_id;
		int end_time;
		EC_GSArenaTeam red_team;
		std::vector<EC_GSArenaPlayer> red_members;
		EC_GSArenaTeam blue_team;
		std::vector<EC_GSArenaPlayer> blue_members;
	};


	void EC_SendArenaBattleServerRegister(int world_index, int world_tag);

	// teams 
	void EC_SendArenaTeamCreate(int roleid, int cls, const void * name, unsigned int name_size, int team_type, const void* team_name, unsigned int team_len);
	void EC_SendArenaTeamInviteRequest(int roleid, int team_type, int invited_roleid, int idx);
	void EC_SendArenaTeamInvite(int idx, int team_type, int roleid, int invited_roleid, int invited_cls, const void * name, unsigned int name_len);
	void EC_SendArenaTeamKickout(int idx, int roleid, int kickid, int team_type);
	void EC_SendArenaTeamCaptainTransfer(int idx, int roleid, int transferid, int team_type);
	void EC_SendArenaTeamLeave(int idx, int roleid, int team_type);
	
	// arena 
	void EC_SendArenaEnterQueue(int roleid, int cls, const void * name, unsigned int name_size, int * members, unsigned int count);
	
	//  battle 
	void EC_SendArenaAcceptBattle(int roleid);
	void EC_SendArenaBattleStart_Re(int arena_id, int world_tag, int retcode);
	
	//  top 
	void EC_SendArenaPlayerTopListQuery(int roleid, int cmd_mode);
	void EC_SendArenaTeamTopListQuery(int roleid, int team_type);
	void EC_SendArenaTeamTopListDetailQuery(int roleid, long long team_id);
	// result
	void EC_SendArenaBattleEnd(int arena_id, int world_tag, int result, int red_alive, int blue_alive, int red_damage, int blue_damage, EC_GSArena & arena );
	
	// ec arena end

	// Codex 
	void CodexRequestStorage(int roleid, int fashion_count, unsigned int fashion_size, const void * fashion_data, int fly_count, unsigned int fly_size, const void * fly_data,
		int mount_count, unsigned int mount_size, const void * mount_data, int pet_count, unsigned int pet_size, const void * pet_data);

	
}

#endif

