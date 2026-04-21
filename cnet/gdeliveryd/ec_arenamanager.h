#ifndef __GNET_ARENAOFAURORA_MANAGER_H__
#define __GNET_ARENAOFAURORA_MANAGER_H__

#include <vector>
#include <unordered_map>
#include <functional>

#include "itimer.h"

// database format
#include "ec_sqlarenaplayer"
#include "ec_sqlarenateam"

// ec format
#include "ec_arenateam"
#include "ec_arenaplayer"
#include "ec_arenateamsync"
#include "ec_arenaplayersync"
#include "ec_arenateamtoplist"
#include "ec_arenaplayertoplist"

#define MAX_MEMBERS 6
#define MAX_SCORE 30
#define MAX_WAIT_QUEUE 600
#define MAX_WAIT_BATTLE_QUEUE 75
#define BASE_SCORE 1000
#define QUALIFICATION_COUNT_REQUIRE 10
#define MAX_BATTLE_POOL 64

namespace GNET
{

namespace ArenaOfAuroraConfig
{
const int server_create_time_out = 30;
const int server_error_time_out = 60;
const bool only_one_arena_server = false;
const int max_arena_count = 50;
}

class ArenaOfAuroraManager : public IntervalTimer::Observer
{
private:

	enum MSG // EC_ArenaClientNotify -- fail 
	{
		// EC_ArenaClientNotify -- error_msg 1--40 
		MSG_ERROR_00 = 0,	// успех
		MSG_ERROR_01,	//13419	"O Pareamento falhou. Seu grupo está em %s - %s."
		MSG_ERROR_02,	//13420	"Erro de Tipo de Grupo"
		MSG_ERROR_03,	//13421	"A Criação Falhou. Você já criou um Grupo para Arena desse tipo."
		MSG_ERROR_04,	//13422	"Falha ao adicionar jogador"
		MSG_ERROR_05,	//13423	"Falha ao criar o Grupo da Arena"
		MSG_ERROR_06,	//13424	"Já existe um Grupo com esse nome na Arena"
		MSG_ERROR_07,	//13425	"Não foi possível encontrar informações da Arena"
		MSG_ERROR_08,	//13426	"Convite recusado. O jogador já se juntou a um Grupo desse tipo."
		MSG_ERROR_09,	//13427	"Está ação está indisponível para equipes temporárias."
		MSG_ERROR_10,	//13428	"Não foi possível encontrar informações do Grupo da Arena"
		MSG_ERROR_11,	//13429	"O Grupo atingiu o tamanho máximo para esse tipo."
		MSG_ERROR_12,	//13430	"Não foi possível encontrar a informação do jogador no Grupo da Arena"
		MSG_ERROR_13,	//13431	"O jogador já está no grupo"
		MSG_ERROR_14,	//13432	"O Pareamento falhou. Violação de restrição de classe."
		MSG_ERROR_15,	//13433	"O Pareamento falhou. Só é permitida 1 classe repetida em batalhas 6v6."
		MSG_ERROR_16,	//13434	"O jogador não está nessa equipe."
		MSG_ERROR_17,	//13435	"Não faz parte da mesma equipe."
		MSG_ERROR_18,	//13436	"Somente o líder do grupo pode expulsar jogadores."
		MSG_ERROR_19,	//13437	"O Pareamento falhou. Você não tem o número necessário de jogadores."
		MSG_ERROR_20,	//13438	"O participante está em outra batalha"
		MSG_ERROR_21,	//13439	"Não foi possível encontrar informações do participante"
		MSG_ERROR_22,	//13440	"O participante não é membro da sua equipe"
		MSG_ERROR_23,	//13441	"Essa equipe está em Pareamento ou em batalha"
		MSG_ERROR_24,	//13442	"O requerimento do número de jogadores não foi atendido"
		MSG_ERROR_25,	//13443	"Não foi possível encontrar informações do interservidor do jogador"
		MSG_ERROR_26,	//13444	"Não foi possível encontrar a batalha"
		MSG_ERROR_27,	//13445	"O jogador não foi selecionado como um combatente"
		MSG_ERROR_28,	//13446	"É preciso transferir a liderança antes de sair da equipe"
		MSG_ERROR_29,	//13447	"Você vai saber, se usar hack."
		MSG_ERROR_30,	//13448	"Não é possível gerenciar seu grupo enquanto as recompensas estão sendo calculadas"
		MSG_ERROR_31,	//13449	"Você participou de batalhas na semana. Não é possível criar ou entrar para uma equipe"
		MSG_ERROR_32,	//13450	"O Pareamento falhou. Está não é a hora para Pareamento da Arena."
		MSG_ERROR_33,	//13451	"A Ação Falhou. Ação de equipe indisponível durante Pareamento ou batalha."
		MSG_ERROR_34,	//13452	"Nenhuma recompensa disponível"
		MSG_ERROR_35,	//13453	"O sistema da Arena ainda não está disponível"
		MSG_ERROR_36,	//13454	"Caracteres inválidos no nome do Grupo"
		MSG_ERROR_37,	//13455	"Nenhuma instância alocada"
		MSG_ERROR_38,	//13456	"Nenhum local alocado"
		MSG_ERROR_39,	//13457	"Esse jogador participou de batalhas na semana. Não é possível fazer o convite."
		MSG_ERROR_40,	//13458	"Foi encontrado um integrante na equipe com 300 vitórias. Depois que você vencer, nenhuma vitória será concedida a qualquer integrante da equipe."
		
		// EC_ArenaClientNotify -- 5603 -- EC_ArenaTeamMatchSuccessData -- sucess_msg
		MSG_START_QUEUE_SOLO_3X3 = 0,	//13548	"Pareamento solo 3v3"
		MSG_START_QUEUE_TEAM_3X3 = 1,	//13549	"Pareamento equipe 3v3"
		MSG_START_QUEUE_SOLO_6X6 = 2,	//13550	"Pareamento solo 6v6"
		MSG_START_QUEUE_TEAM_6X6 = 3,	//13551	"Pareamento equipe 6v6"	

	};

	enum SERVER_STAT
	{
		SERVER_STAT_NORMAL,
		SERVER_STAT_CREATE,
		SERVER_STAT_DISCONNECT,
		SERVER_STAT_ERROR,
		SERVER_STAT_FULL,
	};
	
	enum ARENA_MAN_STAT
	{
		ARENA_MAN_STAT_CLOSE,
		ARENA_MAN_STAT_OPEN,
		ARENA_MAN_STAT_WAIT_CLOSE,
	};

	enum ARENA_STAT
	{
		ARENA_STAT_CREATE,
		ARENA_STAT_OPEN,
		ARENA_STAT_WAIT_CLOSE,
		ARENA_STAT_CLOSE,
	};

	enum PLAYER_STAT
	{
		PLAYER_STAT_APPLY,
		PLAYER_STAT_SWITCH,
		PLAYER_STAT_ENTER,
		PLAYER_STAT_LEAVE,
	};

	enum ARENA_MODE
	{
		MODE_1X1 = 0,
		MODE_3X3 = 1,
		MODE_6X6 = 3,
	};
	
	enum ARENA_NOTUFY
	{
		NOTIFY_CREATE = 0,
		NOTIFY_INVITE = 1,
		NOTIFY_LEAVE = 2,
		NOTIFY_KICK_PLAYER = 3,
		NOTIFY_TRANCFER_CAPITAN = 4,
	};
	
	struct ServerInfo
	{
		int world_tag;
		int server_id;
		int arena_count;
		int time_stamp;
		SERVER_STAT stat;

		ServerInfo()
		{
			world_tag = 0;
			server_id = 0;
			arena_count = 0;
			time_stamp = 0;
			stat = SERVER_STAT_NORMAL;
		}
	};
	
	struct ArenaInfo
	{
		int arena_id;
		int world_tag;
		int end_time;
		int time_stamp;
		ARENA_STAT status;

		IntVector players;

		ArenaInfo()
		{
			arena_id = 0;
			world_tag = 0;
			end_time = 0;
			time_stamp = 0;
			status = ARENA_STAT_CLOSE;
		}

		inline bool DelPlayer(int roleid)
		{
			IntVector::iterator it = players.begin(),eit=players.end();
			for( ; it != eit; ++it)
			{
				if (*it == roleid)
				{
					players.erase(it);
					return true;
				}
			}
			return false;
		}
	};
	
	struct PlayerApplyEntry
	{
		int gameid;
		int damage;
	};
	
	struct PlayerEntry
	{
		int roleid;
		int arena_id;
		time_t time_stamp;
		PLAYER_STAT stat;

		PlayerEntry()
		{
			roleid = 0;
			arena_id = 0;
			time_stamp = 0;
			stat = PLAYER_STAT_APPLY;
		}
	};

	struct SearchBattle
	{
		int id;
		int counter;
		int score;
		bool active;
		IntVector members;
		
		SearchBattle()
		{
			id = -1;
			counter = 0;
			score = 0;
			active = false;
			members.clear();
		}
	};

	struct ActiveBattle
	{
		int arena_id;
		int world_tag;

		EC_SQLArenaTeam red_team;
		EC_SQLArenaPlayerVector red_members;

		EC_SQLArenaTeam blue_team;
		EC_SQLArenaPlayerVector blue_members;

		ActiveBattle()
		{
			arena_id = -1;
			world_tag = -1;
			
			red_team.team_id = -1;
			red_members.clear();

			blue_team.team_id = -1;
			blue_members.clear();

		}
	};

	struct WaiterInfo
	{
		//int team_id;
		int arena_id;
		int world_tag;
		int counter;

		WaiterInfo()
		{
			//team_id = 0;
			arena_id = 0;
			world_tag = 0;
			counter = 0;
		}
	};

	// ec_arena
	struct EC_CrossRole
	{
		union
		{
			struct
			{
				int roleid;
				short mode;
				short zoneid;
			};
			long long id;
		};
		EC_CrossRole() { id = 0; zoneid = 0; mode = MODE_3X3; }
		EC_CrossRole(int rid, int zid) { roleid = rid; zoneid=zid; mode = MODE_3X3; }
		EC_CrossRole(long long xid, int zid) { id = xid; zoneid=zid; mode = MODE_3X3; }
		~EC_CrossRole() { }
	};

	struct EC_CrossTeam
	{
		union
		{
			struct 
			{
				int teamid;
				char reserve1;
				char reserve2;
				char reserve3;
				char mode;
			};
			long long id;
		};
		EC_CrossTeam() { id = 0; mode = MODE_3X3; }
		EC_CrossTeam(int tid) { id = tid; mode = MODE_3X3; }
		EC_CrossTeam(long long xid) { id = xid; mode = MODE_3X3; }
		~EC_CrossTeam() { }
	};
	
	typedef RpcDataVector<EC_CrossRole>			EC_CrossRoleVector;
	typedef RpcDataVector<EC_CrossTeam>			EC_CrossTeamVector;
	// ec_arena end
	
	typedef std::unordered_map<int/*roleid*/, PlayerApplyEntry> PLAYER_APPLY_MAP;
	typedef std::unordered_map<int/*world_tag*/, ServerInfo> SERVER_INFO_MAP;
	typedef std::unordered_map<int/*arenaid*/, ArenaInfo> ARENA_MAP;
	//typedef std::unordered_map<int/*roleid*/, PlayerEntry> PLAYER_ENTRY_MAP;

	// TEAMS AND PLAYERS
	typedef std::unordered_map<int/*teamid*/, EC_SQLArenaTeam> TEAM_ENTRY_MAP;
	typedef std::unordered_map<int/*roleid*/, EC_SQLArenaPlayer> PLAYER_ENTRY_MAP;
	typedef std::unordered_map<int/*roleid*/, int/*teamid*/> ROLEID_TEAMID_MAP;

	// SEARCH BATTLE
	// typedef std::unordered_map<int/*idx*/, SearchBattle> SEARCH_BATTLE_MAP;
	typedef std::vector <SearchBattle> SEARCH_BATTLE_VECTOR;

	// ACTIVE BATTLES
	typedef std::unordered_map<int/*arenaid*/, ActiveBattle> ACTIVE_BATTLES_MAP;
	typedef std::unordered_map<int/*teamid*/, int/*arenaid*/> ACTIVE_TEAMS_BATTLES_MAP;

	// PLAYERS WAITING ENTER
	typedef std::unordered_map<int/*roleid*/, WaiterInfo> PLAYERS_WAITING_ENTER_MAP;

	ArenaOfAuroraManager() : _status(ARENA_MAN_STAT_CLOSE), _arena_index(1)
	{
		memset(_open_days,0,sizeof(_open_days));
	}

public:

	static ArenaOfAuroraManager * GetInstance()
	{
		static ArenaOfAuroraManager instance;
		return &instance;
	}

	bool Initialize();

	void ResetArenaMgr();

	void RegisterServerInfo(int world_tag, int server_id);
	void DisableServerInfo(int world_tag);

	void StartArena();

	void OnArenaStart(int retcode, int arena_id);

	double CalculateScore(int red_score, int blue_score);
	int GetScoreDelta(int red_score, int blue_score, int result);

	void OnArenaEnd(int arena_id, int result, int red_alive, int blue_alive, int red_damage, int blue_damage, EC_SQLArenaTeam & red_team, EC_SQLArenaPlayerVector & red_members, EC_SQLArenaTeam & blue_team, EC_SQLArenaPlayerVector & blue_members);
	
	void OnCountdown(int arena_id, int countdown);
	void OnBattleStat(int arena_id, int timer, int red_alive, int blue_alive);

	void AddMemberToResult(EC_SQLArenaPlayerVector &members, int player_id);

	int  CalculateQualScore(int win_count);
	void CalculateBattleResult(EC_SQLArenaTeam* pWinnerTeam, IntVector & winners , EC_SQLArenaTeam* pLoserTeam, IntVector & losers );
	void CalculateWinnerResult(int player_id);
	void CalculateLoserResult(int player_id);

	void PlayerApply(int roleid, int damage);

	void PlayerEnterArena(int roleid, int arena_id, int world_tag);
	void PlayerLeaveArena(int roleid, int arena_id, int world_tag);

	void OnPlayerLogin(int roleid);
	void OnPlayerLogout(int roleid);

	void TeamFullLoad(int team_id);

	void DeleteTeam(int teamid);

	void LoadTeam(int teamid);
	void OnLoadTeam(EC_SQLArenaTeam & data);

	void LoadPlayer(int roleid);
	void OnLoadPlayer(EC_SQLArenaPlayer & data);

	void SendInvite();
	void OnApplyInvite(int roleid, int invited_roleid, int invited_cls, Octets & invited_name);

	void ChangeLeader(int roleid, int new_leader);
	void KickoutTeam(int roleid, int kickout_player);
	void LeaveTeam(int roleid);

	void OnStartSearch(int roleid, int cls, Octets & name);
	void OnStartSearch(IntVector & members);
	void SearchClearTimeout();
	void SearchProcessing(time_t now_time);
	static bool CompareSearchBattle(const SearchBattle& t1, const SearchBattle& t2);

	void WaiterInfoClearTimeout();
	void BeginBattle(ActiveBattle & ab);
	void BeginEnter(int roleid);

	void SyncPlayerToDB(int roleid);
	void SyncTeamToDB(int teamid);

	void SendTeamCreateRe(int retcode, int roleid, int teamid);
	void SendTeamChangeRe(int roleid, int team_id, int capitan_id, int new_leader);
	void SendTeamKickRe(int roleid, int team_id, int capitan_id, int kicked);
	void SendTeamLeaveRe(int roleid, int team_id, int leaved);
	void SendWaitQueue( int roleid );
	void SendWaitQueue( SearchBattle & sb );
	void SendWaitQueueTimeOut( SearchBattle & sb );
	void SendNoticeBattleBegin(int roleid);
	void SendNoticeBattle( EC_SQLArenaTeam & team, EC_SQLArenaPlayerVector & members , int counter );

	EC_SQLArenaTeam* GetArenaTeamByTeamID(int teamid);
	EC_SQLArenaPlayer* GetArenaPlayerByRoleID(int roleid);

	void SendArenaInfoToPlayer(int roleid);
	void SendArenaInfoToPlayer(int roleid, int dest_roleid);
	void SendArenaInfoToPlayer(int roleid, int team_id, int reason, Octets & info);

	// converting long long to ec_idx
	void EC_TeamConvert(EC_ArenaTeam & team, const EC_SQLArenaTeam & in);
	void EC_PlayerConvert(EC_ArenaPlayer & player, const EC_SQLArenaPlayer & in);
	void EC_TeamSyncConvert(EC_ArenaTeamSync & team, const EC_SQLArenaTeam & in);
	void EC_PlayerSyncConvert(EC_ArenaPlayerSync & player, const EC_SQLArenaPlayer & in);
	
	void EC_PlayerInviteRequest(int roleid, int invited_roleid);

	void SendError( int roleid, int message );
	void SendOnlineNotify(int roleid, bool self = false);
	
	bool MakeActiveBattle1x1( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb );
	bool MakeActiveBattle3x3( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb );
	bool MakeActiveBattle6x6( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb );
	
	// top list
	void EC_ArenaPlayerTopList_Re(int roleid);
	void EC_ArenaTeamTopList_Re(int roleid);
	
	void EC_ArenaTeamTopListDetail(int roleid, long long teamid);
	void EC_ArenaTeamTopListDetail_Re(int roleid, long long teamid, EC_ArenaPlayerTopListVector & vec);
	
	void EC_UpdateArenaPlayerTopList(time_t now_time);
	void EC_UpdateArenaTeamTopList(time_t now_time);
	
	void EC_SetArenaPlayerTopList(EC_ArenaPlayerTopListVector & vec);
	void EC_SetArenaTeamTopList(EC_ArenaTeamTopListVector & vec);	

	// ed db templates 
	bool EC_MakeDefaultPlayer(EC_SQLArenaPlayer & pPlayer, int team_id, int roleid, int cls, Octets & name);
	bool EC_MakeDefaultTeam(EC_SQLArenaTeam & pTeam, int capitan_id, int team_type, Octets & team_name);

	// ec db player
	void EC_CreateArenaPlayer(int roleid, int cls, Octets & name);
	void EC_CreateArenaPlayer_Re(int roleid, int retcode, EC_SQLArenaPlayer & pPlayer);
	void EC_DeleteArenaPlayer(int roleid);
	void EC_DeleteArenaPlayer_Re(int roleid, int retcode);
	void EC_GetArenaPlayer(int roleid);
	void EC_GetArenaPlayer_Re(int roleid, int retcode, EC_SQLArenaPlayer & pPlayer);
	void EC_SetArenaPlayer(int roleid, EC_SQLArenaPlayer & pPlayer);
	void EC_SetArenaPlayer_Re(int roleid, int retcode);
	
	// ec db team
	void EC_CreateArenaTeam(int capitan_id, int capitan_cls, Octets & capitan_name, int team_type, Octets & team_name);
	void EC_CreateArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaTeam & team, EC_SQLArenaPlayer & player);
	void EC_DeleteArenaTeam(int captain_id, int team_id);
	void EC_DeleteArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaPlayer & pPlayer);
	void EC_GetArenaTeam(int captain_id, int team_id);
	void EC_GetArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaTeam & team);
	void EC_SetArenaTeam(int captain_id, int team_id, EC_SQLArenaTeam & team);
	void EC_SetArenaTeam_Re(int roleid, int team_id, int retcode);

	//void SendBattleResult(int roleid, ArenaBattleResult proto);

private:

	bool Update();

	void SendPlayerApplyRe(int roleid, int retcode, unsigned int linksid = 0, unsigned int localsid = 0);

	void UpdateServerInfo(time_t nowtime);

	ArenaInfo* GetArenaByArenaID(int arena_id);
	ServerInfo* GetServerInfoByWorldTag(int world_tag);
	PlayerEntry* GetPlayerEntryByRoleID(int roleid);
	ServerInfo* GetRandomServer();

	//int GetTeamIdByRoleId(int roleid);
	const SearchBattle* GetSearchBattleByTeamId(int teamid, bool is_team);
	int GetActiveBattleByTeamId(int teamid, bool is_team);
	int GetGameIdByRoleId(int roleid);

	int GetTime() const;

	inline int GetFreeArenaID() { return _arena_index++;}

	// converting long long to ec_idx
	void TeamIdx( long long & idx );
	void RoleIdx( long long & idx );

private:

	ARENA_MAN_STAT _status;
	int _arena_index;
	unsigned char _open_days[7];

	ARENA_MAP _arena_map;
	SERVER_INFO_MAP _server_info_map;
	PLAYER_APPLY_MAP _player_apply_map;
	//PLAYER_ENTRY_MAP _player_entry_map;

	TEAM_ENTRY_MAP _team_map;
	PLAYER_ENTRY_MAP _player_map;
	//ROLEID_TEAMID_MAP _roleid_teamid_map;

	// SEARCH BATTLE
	SEARCH_BATTLE_VECTOR _search_battle_map_1x1;
	SEARCH_BATTLE_VECTOR _search_battle_map_3x3;
	SEARCH_BATTLE_VECTOR _search_battle_map_6x6;

	// ACTIVE BATTLES
	ACTIVE_BATTLES_MAP _active_battles_map;
	ACTIVE_TEAMS_BATTLES_MAP _active_teams_battles_map_1x1;
	ACTIVE_TEAMS_BATTLES_MAP _active_teams_battles_map_3x3;
	ACTIVE_TEAMS_BATTLES_MAP _active_teams_battles_map_6x6;

	// PLAYERS WAITING ENTER
	PLAYERS_WAITING_ENTER_MAP _players_waiting_enter_map;
	
	// Arena top lists cache
	EC_ArenaTeamTopListVector _teams_top_list;
	EC_ArenaPlayerTopListVector _players_top_list;
	
};

}
#endif
