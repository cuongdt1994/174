#include <cmath>
#include <sstream>

#include "conf.h"
#include "glog.h"
#include "timer.h"
#include "parsestring.h"

#include "localmacro.h"
#include "mapuser.h"
#include "gproviderserver.hpp"
#include "gdeliveryserver.hpp"
#include "gauthclient.hpp"
#include "uniquenameclient.hpp"
#include "rolestatusannounce.hpp"

#include "ec_arenamanager.h"
#include "ec_arenabattlestart.hpp"
#include "ec_arenabattleenter.hpp"
#include "ec_arenaquery_re.hpp"
#include "ec_arenateamdatanotify.hpp"
#include "ec_arenaplayertotalinfoquery_re.hpp"
#include "ec_arenateaminviterequest_re.hpp"
#include "ec_arenateammatch.hpp"
#include "ec_arenaclientnotify.hpp"
#include "ec_arenamatchcancelnotify.hpp"
#include "ec_arenabattleinfonotifyclient.hpp"
#include "ec_arenaplayertoplistquery.hpp"
#include "ec_arenateamtoplistquery.hpp"
#include "ec_arenateamtoplistdetailquery.hpp"
#include "ec_arenaplayertoplistquery_re.hpp"
#include "ec_arenateamtoplistquery_re.hpp"
#include "ec_arenateamtoplistdetailquery_re.hpp"
#include "ec_arenaonlineinfonotify.hpp"
#include "ec_dbarenaplayertoplist.hrp"
#include "ec_dbarenateamtoplist.hrp"
#include "ec_dbarenateamtoplistdetail.hrp"
#include "ec_sqlcreatearenaplayer.hrp"
#include "ec_sqlcreatearenateam.hrp"
#include "ec_sqldeletearenaplayer.hrp"
#include "ec_sqldeletearenateam.hrp"
#include "ec_sqlgetarenaplayer.hrp"
#include "ec_sqlgetarenateam.hrp"
#include "ec_sqlsetarenaplayer.hrp"
#include "ec_sqlsetarenateam.hrp"


#define WEEK_DAY_CNT 7

namespace GNET
{

bool ArenaOfAuroraManager::Initialize()
{
	//Conf* conf = Conf::GetInstance();
	std::string key = "ARENAOFAURORA";

	IntervalTimer::Attach(this, 1000000/IntervalTimer::Resolution());
	return true;
}

void ArenaOfAuroraManager::ResetArenaMgr()
{
	_arena_map.clear();
}

bool ArenaOfAuroraManager::Update()
{
	time_t now = GetTime();

	SearchProcessing(now);
	WaiterInfoClearTimeout();

	EC_UpdateArenaPlayerTopList(now);
	EC_UpdateArenaTeamTopList(now);

	return true;
}

void ArenaOfAuroraManager::RegisterServerInfo(int world_tag, int server_id)
{
	LOG_TRACE("ArenaOfAuroraManager RegisterServerInfo worldtag=%d, server_id=%d\n", world_tag, server_id);

	SERVER_INFO_MAP::iterator it = _server_info_map.find(world_tag);
	if (it == _server_info_map.end())
	{
		if (ArenaOfAuroraConfig::only_one_arena_server && !_server_info_map.empty())
		{
			Log::log(LOG_ERR,"ArenaOfAuroraManager RegisterServerInfo duplicate. world_tag=%d, server_id=%d", world_tag, server_id);
			return;
		}

		ServerInfo si;
		si.world_tag = world_tag;
		si.server_id = server_id;
		si.time_stamp = GetTime();
		si.stat = SERVER_STAT_NORMAL;
		
		_server_info_map[world_tag] = si;
	}
	else
	{
		if (SERVER_STAT_DISCONNECT != (it->second).stat)
		{
			Log::log(LOG_ERR,"ArenaOfAuroraManager RegisterServerInfo stat wrong. world_tag=%d server_id=%d stat=%d", world_tag, server_id, (int)(it->second).stat);
			return;
		}

		ServerInfo & si = it->second;

		si.time_stamp = GetTime();
		si.stat = SERVER_STAT_NORMAL;
	}
}

void ArenaOfAuroraManager::DisableServerInfo(int world_tag)
{
	ServerInfo* psi = GetServerInfoByWorldTag(world_tag);
	if (psi)
	{
		psi->time_stamp = GetTime();
		psi->stat = SERVER_STAT_DISCONNECT;

		LOG_TRACE("ArenaOfAuroraManager DisableServerInfo worldtag=%d", world_tag);
	}
}

void ArenaOfAuroraManager::StartArena()
{
}

void ArenaOfAuroraManager::OnArenaStart(int retcode, int arena_id)
{
	if (retcode == -1)
	{
		LOG_TRACE("ArenaOfAuroraManager OnArenaStart failed, arena_id=%d", arena_id);
		return;
	}

	LOG_TRACE("ArenaOfAuroraManager OnArenaStart success, arena_id=%d", arena_id);
}

double ArenaOfAuroraManager::CalculateScore(int red_score, int blue_score)
{
	return 1 / (1 + pow(10, (blue_score - red_score) / 400.0));
}


int ArenaOfAuroraManager::GetScoreDelta(int red_score, int blue_score, int result)
{
	return (int)((MAX_SCORE + 1) * ((int)result - CalculateScore(red_score, blue_score)));
}

void ArenaOfAuroraManager::OnArenaEnd(int arena_id, int result, int red_alive, int blue_alive, int red_damage, int blue_damage, EC_SQLArenaTeam & red_team, EC_SQLArenaPlayerVector & red_members, EC_SQLArenaTeam & blue_team, EC_SQLArenaPlayerVector & blue_members)
{
	LOG_TRACE("ArenaOfAuroraManager::OnArenaEnd, arena_id=%d, result=%d", arena_id, result);

	
	ACTIVE_BATTLES_MAP::iterator ab_it = _active_battles_map.find(arena_id);
	if (ab_it == _active_battles_map.end()) {
		LOG_TRACE("ArenaOfAuroraManager::OnArenaEnd, arena battle %d not found", arena_id);
		return;
	}

	ActiveBattle* battle = &(ab_it->second);
	if (!battle) {
		LOG_TRACE("ArenaOfAuroraManager::OnArenaEnd, arena battle %d error", arena_id);
		return;
	}

	if ( red_team.team_id < 1 )
	{
		auto atb_it_r = _active_teams_battles_map_1x1.find( battle->red_members[0].player_id );
		if (atb_it_r != _active_teams_battles_map_1x1.end())
		_active_teams_battles_map_1x1.erase( atb_it_r );
	}

	if ( blue_team.team_id < 1 )
	{
		auto atb_it_r = _active_teams_battles_map_1x1.find( battle->blue_members[0].player_id );
		if (atb_it_r != _active_teams_battles_map_1x1.end())
		_active_teams_battles_map_1x1.erase( atb_it_r );
	}

	{
		auto atb_it_r = _active_teams_battles_map_3x3.find(battle->red_team.team_id);
		if (atb_it_r != _active_teams_battles_map_3x3.end())
			_active_teams_battles_map_3x3.erase(atb_it_r);

		auto atb_it_b = _active_teams_battles_map_3x3.find(battle->blue_team.team_id);
		if (atb_it_b != _active_teams_battles_map_3x3.end())
			_active_teams_battles_map_3x3.erase(atb_it_b);
	}

	{
		auto atb_it_r = _active_teams_battles_map_6x6.find(battle->red_team.team_id);
		if (atb_it_r != _active_teams_battles_map_6x6.end())
			_active_teams_battles_map_6x6.erase(atb_it_r);

		auto atb_it_b = _active_teams_battles_map_6x6.find(battle->blue_team.team_id);
		if (atb_it_b != _active_teams_battles_map_6x6.end())
			_active_teams_battles_map_6x6.erase(atb_it_b);
	}

	_active_battles_map.erase(ab_it);

	if ( red_team.team_id > 0 )
	{
		EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID( red_team.team_id );
		if (pTeam)
		{
			*pTeam = red_team;
			SyncTeamToDB(pTeam->team_id);
		}
	}

	for (size_t i = 0; i < red_members.size(); i++)
	{
		int member = red_members[i].player_id;
		if (member > 0)
		{
			EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(member);
			if (pPlayer) 
			{
				*pPlayer = red_members[i];
				SyncPlayerToDB(member);
				SendArenaInfoToPlayer(member);
			}
		}
	}
	
	if ( blue_team.team_id > 0 )
	{
		EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID( blue_team.team_id );
		if (pTeam)
		{
			*pTeam = blue_team;
			SyncTeamToDB(pTeam->team_id);
		}
	}

	for (size_t i = 0; i < blue_members.size(); i++)
	{
		int member = blue_members[i].player_id;
		if (member > 0)
		{
			EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(member);
			if (pPlayer) 
			{
				*pPlayer = blue_members[i];
				SyncPlayerToDB(member);
				SendArenaInfoToPlayer(member);
			}
		}
	}
}

void ArenaOfAuroraManager::AddMemberToResult(EC_SQLArenaPlayerVector &members, int player_id)
{
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(player_id);
	if (!pPlayer) {
		LOG_TRACE("ArenaOfAuroraManager::AddMemberToResult, member %d not found", player_id);
		return;
	}

	members.push_back(*pPlayer);

	//LOG_TRACE("ArenaOfAuroraManager::AddMemberToResult, successfully added member %d (%d) [ size of vector %d ]", player_id, pPlayer->player_id, members.size());
}

void ArenaOfAuroraManager::OnCountdown(int arena_id, int countdown)
{
	//LOG_TRACE("ArenaOfAuroraManager::OnCountdown, arena_id=%d, countdown=%d", arena_id, countdown);

	ACTIVE_BATTLES_MAP::iterator ab_it = _active_battles_map.find(arena_id);
	if (ab_it == _active_battles_map.end()) {
		LOG_TRACE("ArenaOfAuroraManager::OnCountdown, arena battle %d not found", arena_id);
		return;
	}

	ActiveBattle* battle = &(ab_it->second);
	if (!battle) {
		LOG_TRACE("ArenaOfAuroraManager::OnCountdown, arena battle %d error", arena_id);
		return;
	}

	/*ArenaCountdown proto;
	proto.reason = 1;
	proto.value = countdown;

	// Send countdown to red team
	SendCountdown(battle->red_member_1, proto);
	SendCountdown(battle->red_member_2, proto);
	SendCountdown(battle->red_member_3, proto);

	// Send countdown to blue team
	SendCountdown(battle->blue_member_1, proto);
	SendCountdown(battle->blue_member_2, proto);
	SendCountdown(battle->blue_member_3, proto);*/
}

void ArenaOfAuroraManager::OnBattleStat(int arena_id, int timer, int red_alive, int blue_alive)
{
	//LOG_TRACE("ArenaOfAuroraManager::OnBattleStat, arena_id=%d, timer=%d, red_alive=%d, blue_alive=%d", arena_id, timer, red_alive, blue_alive);

	ACTIVE_BATTLES_MAP::iterator ab_it = _active_battles_map.find(arena_id);
	if (ab_it == _active_battles_map.end()) {
		LOG_TRACE("ArenaOfAuroraManager::OnBattleStat, arena battle %d not found", arena_id);
		return;
	}

	ActiveBattle* battle = &(ab_it->second);
	if (!battle) {
		LOG_TRACE("ArenaOfAuroraManager::OnBattleStat, arena battle %d error", arena_id);
		return;
	}

	/*ArenaBattleStat proto;

	proto.timer = timer;
	proto.red_alive = red_alive;
	proto.blue_alive = blue_alive;

	// Send stat to red team
	SendBattleStat(battle->red_member_1, proto);
	SendBattleStat(battle->red_member_2, proto);
	SendBattleStat(battle->red_member_3, proto);

	// Send stat to blue team
	SendBattleStat(battle->blue_member_1, proto);
	SendBattleStat(battle->blue_member_2, proto);
	SendBattleStat(battle->blue_member_3, proto);*/
}

int ArenaOfAuroraManager::CalculateQualScore(int win_count)
{
	if (win_count >= 0 && win_count <= 1)
		return 1300;

	if (win_count >= 2 && win_count <= 3)
		return 1400;

	if (win_count >= 4 && win_count <= 6)
		return 1500;

	if (win_count >= 7 && win_count <= 8)
		return 1600;

	if (win_count >= 9 && win_count <= 10)
		return 1700;

	return 1000;
}

void ArenaOfAuroraManager::CalculateBattleResult(EC_SQLArenaTeam* pWinnerTeam, IntVector & winners , EC_SQLArenaTeam* pLoserTeam, IntVector & losers )
{
	// increment win
	pWinnerTeam->win_count += 1;

	// increment battle count
	pWinnerTeam->battle_count += 1;
	pLoserTeam->battle_count += 1;

	// increment week battle count
	pWinnerTeam->week_battle_count += 1;
	pLoserTeam->week_battle_count += 1;

	// set if winner is completed qualification
	if (pWinnerTeam->battle_count == QUALIFICATION_COUNT_REQUIRE)
		pWinnerTeam->score = CalculateQualScore(pWinnerTeam->win_count);

	// set if loser is completed qualification
	if (pLoserTeam->battle_count == QUALIFICATION_COUNT_REQUIRE)
		pLoserTeam->score = CalculateQualScore(pLoserTeam->win_count);

	// Winner members & sync
	for (size_t i = 0; i < winners.size(); i++)
	{
		CalculateWinnerResult(winners[i]);
	}

	// Loser members & sync
	for (size_t i = 0; i < losers.size(); i++)
	{
		CalculateLoserResult(losers[i]);
	}

	// Sync teams
	SyncTeamToDB(pWinnerTeam->team_id);
	SyncTeamToDB(pLoserTeam->team_id);
}

void ArenaOfAuroraManager::CalculateWinnerResult(int player_id)
{
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(player_id);
	if (!pPlayer) {
		LOG_TRACE("ArenaOfAuroraManager::CalculateWinnerResult, player %d not found", player_id);
		return;
	}

	pPlayer->battle_count += 1;
	pPlayer->week_battle_count += 1;

	pPlayer->win_count += 1;

	// set if is completed qualification
	if (pPlayer->battle_count == QUALIFICATION_COUNT_REQUIRE)
		pPlayer->score = CalculateQualScore(pPlayer->win_count);

	// Sync to DB
	SyncPlayerToDB(player_id);
}

void ArenaOfAuroraManager::CalculateLoserResult(int player_id)
{
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(player_id);
	if (!pPlayer) {
		LOG_TRACE("ArenaOfAuroraManager::CalculateLoserResult, player %d not found", player_id);
		return;
	}

	pPlayer->battle_count += 1;
	pPlayer->week_battle_count += 1;

	if (pPlayer->score < 0) pPlayer->score = 0;

	// set if is completed qualification
	if (pPlayer->battle_count == QUALIFICATION_COUNT_REQUIRE)
		pPlayer->score = CalculateQualScore(pPlayer->win_count);

	// Sync to DB
	SyncPlayerToDB(player_id);
}

void ArenaOfAuroraManager::PlayerApply(int roleid, int damage)
{
}

void ArenaOfAuroraManager::SendPlayerApplyRe(int roleid, int retcode, unsigned int linksid, unsigned int localsid)
{
}

void ArenaOfAuroraManager::PlayerEnterArena(int roleid, int arena_id, int world_tag)
{
}

void ArenaOfAuroraManager::PlayerLeaveArena(int roleid, int arena_id, int world_tag)
{
}

ArenaOfAuroraManager::ArenaInfo* ArenaOfAuroraManager::GetArenaByArenaID(int arena_id)
{
	return NULL;
}

ArenaOfAuroraManager::ServerInfo* ArenaOfAuroraManager::GetServerInfoByWorldTag(int world_tag)
{
	SERVER_INFO_MAP::iterator it = _server_info_map.find(world_tag);
	if (it != _server_info_map.end())
		return &(it->second);
	return NULL;
}

ArenaOfAuroraManager::PlayerEntry* ArenaOfAuroraManager::GetPlayerEntryByRoleID(int roleid)
{
	return NULL;
}

ArenaOfAuroraManager::ServerInfo* ArenaOfAuroraManager::GetRandomServer()
{
	IntVector active_tags;

	SERVER_INFO_MAP::iterator it = _server_info_map.begin();
	for (; it != _server_info_map.end(); ++it)
	{
		ServerInfo& si = it->second;

		if (si.stat == SERVER_STAT_NORMAL)
		{
			active_tags.push_back(si.world_tag);
		}
	}

	if (!active_tags.size())
	{
		return NULL;
	}

	int index = rand() % active_tags.size();

	return GetServerInfoByWorldTag(active_tags[index]);
}

EC_SQLArenaTeam* ArenaOfAuroraManager::GetArenaTeamByTeamID(int teamid)
{
	TEAM_ENTRY_MAP::iterator it = _team_map.find(teamid);
	if (it != _team_map.end())
		return &(it->second);
	return NULL;
}

EC_SQLArenaPlayer* ArenaOfAuroraManager::GetArenaPlayerByRoleID(int roleid)
{
	PLAYER_ENTRY_MAP::iterator it = _player_map.find(roleid);
	if (it != _player_map.end())
		return &(it->second);
	return NULL;
}

const ArenaOfAuroraManager::SearchBattle* ArenaOfAuroraManager::GetSearchBattleByTeamId(int id, bool is_team)
{
	if (!is_team)
	{
		for (const auto& sb : _search_battle_map_1x1)
		{
			if ( id == sb.id )
			{
				return &sb;
			}
		}
	}
	else
	{
		for (const auto& sb : _search_battle_map_3x3)
		{
			if ( id == sb.id )
			{
				return &sb;
			}
		}
		
		for (const auto& sb : _search_battle_map_6x6)
		{
			if ( id == sb.id )
			{
				return &sb;
			}
		}
	}
	return NULL;
}

int ArenaOfAuroraManager::GetActiveBattleByTeamId(int id, bool is_team)
{
	if (!is_team)
	{
		ACTIVE_TEAMS_BATTLES_MAP::iterator it = _active_teams_battles_map_1x1.find(id);
		if (it != _active_teams_battles_map_1x1.end())
			return it->second;
	}
	else
	{
		{
			ACTIVE_TEAMS_BATTLES_MAP::iterator it = _active_teams_battles_map_3x3.find(id);
			if (it != _active_teams_battles_map_3x3.end())
				return it->second;
		}
		
		{
			ACTIVE_TEAMS_BATTLES_MAP::iterator it = _active_teams_battles_map_6x6.find(id);
			if (it != _active_teams_battles_map_6x6.end())
				return it->second;
		}
	}
	return -1;
}

int ArenaOfAuroraManager::GetGameIdByRoleId(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pInfo == NULL)
		return -1;

	return pInfo->gameid;
}

void ArenaOfAuroraManager::UpdateServerInfo(time_t now_time)
{
	SERVER_INFO_MAP::iterator it = _server_info_map.begin();
	for ( ; it != _server_info_map.end(); ++it)
	{
		ServerInfo & si = it->second;
		if (SERVER_STAT_CREATE == si.stat)
		{
			if (now_time - si.time_stamp > ArenaOfAuroraConfig::server_create_time_out)
			{
				si.time_stamp = GetTime();
				si.stat = SERVER_STAT_NORMAL;
				
				Log::log(LOG_ERR,"ArenaOfAuroraManager UpdateServerInfo create timeout. worldtag=%d",si.world_tag);
			}
		}
		else if (SERVER_STAT_ERROR == si.stat)
		{
			if (now_time - si.time_stamp > ArenaOfAuroraConfig::server_error_time_out)
			{
				si.time_stamp = GetTime();
				si.stat = SERVER_STAT_NORMAL;

				LOG_TRACE("ArenaOfAuroraManager UpdateServerInfo error timeout. worldtag=%d\n",si.world_tag);
			}
		}
	}
}

int ArenaOfAuroraManager::GetTime() const
{
	return Timer::GetTime();
}

void ArenaOfAuroraManager::OnPlayerLogin(int roleid)
{
	if (roleid == 0)
		return;

	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(roleid);
	if (!pPlayer)
	{
		Log::log(LOG_ERR, "OnPlayerLogin, arena player %d not found! SEND DB PACKET", roleid);
		EC_GetArenaPlayer(roleid);
		return;
	}

	Log::log(LOG_ERR, "OnPlayerLogin, arena player %d found! GET NOW", roleid);

	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(pPlayer->team_id);
	if (!pTeam)
	{
		Log::log(LOG_ERR, "OnPlayerLogin, arena team %d not found! SEND DB PACKET", pPlayer->team_id);
		EC_GetArenaTeam(roleid, pPlayer->team_id);
		return;
	}

	Log::log(LOG_ERR, "OnPlayerLogin, arena team %d found! GET NOW", pTeam->team_id);

	TeamFullLoad(pTeam->team_id);
	SendOnlineNotify(roleid);
}

void ArenaOfAuroraManager::OnPlayerLogout(int roleid)
{
	SendOnlineNotify(roleid);
}

void ArenaOfAuroraManager::DeleteTeam(int teamid)
{
	EC_SQLArenaTeam* arenaTeam = GetArenaTeamByTeamID(teamid);
	if (!arenaTeam) {
		Log::log(LOG_ERR, "DeleteTeam, arena team %d not found", teamid);
		return;
	}
	
	if ( arenaTeam->members.size() > 1 )
	{
		Log::log(LOG_ERR, "DeleteTeam, arena members.size() %d > 1", arenaTeam->members.size() );
		return;
	}
	
	EC_DeleteArenaTeam(arenaTeam->captain_id, arenaTeam->team_id);
}

void ArenaOfAuroraManager::OnApplyInvite(int roleid, int invited_roleid, int invited_cls, Octets & invited_name)
{
	if (roleid == invited_roleid)
		return;

	LOG_TRACE("ArenaOfAuroraManager OnApplyInvite roleid=%d, invited_roleid=%d", roleid, invited_roleid);

	EC_SQLArenaPlayer* apInviter = GetArenaPlayerByRoleID(roleid);
	if (!apInviter)
	{
		SendError(roleid, MSG_ERROR_21);
		Log::log(LOG_ERR, "OnApplyInvite, arena player inviter %d not found", roleid);
		return;
	}

	if (apInviter->team_id == 0)
	{
		SendError(roleid, MSG_ERROR_17);
		Log::log(LOG_ERR, "OnApplyInvite, arena player inviter %d not in team", roleid);
		return;
	}

	EC_SQLArenaTeam* teamInfo = GetArenaTeamByTeamID(apInviter->team_id);
	if (!teamInfo)
	{
		SendError(roleid, MSG_ERROR_10);
		Log::log(LOG_ERR, "OnApplyInvite, arena team %d not found", apInviter->team_id);
		return;
	}

	if (teamInfo->captain_id != roleid)
	{
		SendError(roleid, MSG_ERROR_04);
		Log::log(LOG_ERR, "OnApplyInvite, arena team %d insufficient privileges (inviter:%d, captain:%d)", apInviter->team_id, roleid, teamInfo->captain_id);
		return;
	}

	if (teamInfo->members.size() >= MAX_MEMBERS)
	{
		SendError(roleid, MSG_ERROR_11);
		Log::log(LOG_ERR, "OnApplyInvite, arena team %d limit members", teamInfo->team_id);
		return;
	}

	EC_SQLArenaPlayer* apInvitee = GetArenaPlayerByRoleID(invited_roleid);
	if (apInvitee && apInvitee->team_id != 0)
	{
		SendError(roleid, MSG_ERROR_16);
		Log::log(LOG_ERR, "OnApplyInvite, arena player invitee %d in team %d", invited_roleid, apInvitee->team_id);
		return;
	}

	teamInfo->members.push_back(invited_roleid);

	if (apInvitee)
	{
		apInvitee->team_id = teamInfo->team_id;
	}
	else
	{
		EC_SQLArenaPlayer player;
		EC_MakeDefaultPlayer(player, teamInfo->team_id, invited_roleid,  invited_cls, invited_name);
		OnLoadPlayer(player);
	}

	SyncPlayerToDB(invited_roleid);
	SyncTeamToDB(teamInfo->team_id);

	int dd_info[2];
	dd_info[0] = byteorder_32(roleid);
	dd_info[1] = byteorder_32(invited_roleid);

	Octets info;
	info.replace(&dd_info, sizeof(dd_info));

	for (int i = 0; i < (int)teamInfo->members.size(); i++) 
	{
		SendArenaInfoToPlayer(teamInfo->members[i].player_id, teamInfo->team_id, NOTIFY_INVITE, info);
		SendOnlineNotify(teamInfo->members[i].player_id);
	}
}

void ArenaOfAuroraManager::ChangeLeader(int old_leader, int new_leader)
{
	if (old_leader == new_leader)
		return;

	PlayerInfo* leaderInfo = UserContainer::GetInstance().FindRole(old_leader);
	if (leaderInfo == NULL)
		return;

	EC_SQLArenaPlayer* arenaOldLeader = GetArenaPlayerByRoleID(old_leader);
	if (!arenaOldLeader) {
		SendError(old_leader, MSG_ERROR_21);
		Log::log(LOG_ERR, "ChangeLeader, arena player old_leader %d not found", old_leader);
		return;
	}

	EC_SQLArenaPlayer* arenaNewLeader = GetArenaPlayerByRoleID(new_leader);
	if (!arenaNewLeader) {
		SendError(old_leader, MSG_ERROR_21);
		Log::log(LOG_ERR, "ChangeLeader, arena player new_leader %d not found", new_leader);
		return;
	}

	if (arenaOldLeader->team_id == 0) {
		SendError(old_leader, MSG_ERROR_17);
		Log::log(LOG_ERR, "ChangeLeader, arena player old_leader %d not in team", old_leader);
		return;
	}

	if (arenaNewLeader->team_id == 0) {
		SendError(old_leader, MSG_ERROR_17);
		Log::log(LOG_ERR, "ChangeLeader, arena player new_leader %d not in team", new_leader);
		return;
	}

	if (arenaOldLeader->team_id != arenaNewLeader->team_id) {
		SendError(old_leader, MSG_ERROR_22);
		Log::log(LOG_ERR, "ChangeLeader, arena player old_leader %d and new_leader %d not different team %d = %d", old_leader, new_leader, arenaOldLeader->team_id, arenaNewLeader->team_id);
		return;
	}

	EC_SQLArenaTeam* arenaTeam = GetArenaTeamByTeamID(arenaOldLeader->team_id);
	if (!arenaTeam) {
		SendError(old_leader, MSG_ERROR_10);
		Log::log(LOG_ERR, "ChangeLeader, arena team %d not found", arenaOldLeader->team_id);
		return;
	}

	if (arenaTeam->captain_id != old_leader) {
		SendError(old_leader, MSG_ERROR_13);
		Log::log(LOG_ERR, "ChangeLeader, arena team %d insufficient privileges (leader:%d, captain:%d)", arenaTeam->team_id, old_leader, arenaTeam->captain_id);
		return;
	}

	bool found = false;

	std::vector<EC_SQLArenaTeamMember>::iterator it = arenaTeam->members.begin();
	for (; it != arenaTeam->members.end(); it++)
	{
		if (it->player_id == new_leader)
		{
			found = true;
			break;
		}
	}

	if (!found) 
	{
		SendError(old_leader, MSG_ERROR_16);
		Log::log(LOG_ERR, "ChangeLeader, arena player new_leader %d not found in team %d", new_leader, arenaTeam->team_id);
		return;
	}

	arenaTeam->captain_id = new_leader;
	SyncTeamToDB(arenaTeam->team_id);

	LOG_TRACE("ChangeLeader, arena player %d successfully set leader to %d in team %d", old_leader, new_leader, arenaTeam->team_id);

	for (int i = 0; i < (int)arenaTeam->members.size(); i++) {
		SendTeamChangeRe(arenaTeam->members[i].player_id, arenaTeam->team_id, old_leader, arenaTeam->captain_id);
		SendOnlineNotify(arenaTeam->members[i].player_id);
	}
}

void ArenaOfAuroraManager::KickoutTeam(int leader, int kickout)
{
	if (leader == kickout)
		return;

	PlayerInfo* leaderInfo = UserContainer::GetInstance().FindRole(leader);
	if (leaderInfo == NULL)
		return;

	EC_SQLArenaPlayer* arenaLeader = GetArenaPlayerByRoleID(leader);
	if (!arenaLeader) {
		SendError(leader, MSG_ERROR_21);
		Log::log(LOG_ERR, "KickoutTeam, arena player leader %d not found", leader);
		return;
	}

	EC_SQLArenaPlayer* arenaKickout = GetArenaPlayerByRoleID(kickout);
	if (!arenaKickout) {
		SendError(leader, MSG_ERROR_21);
		Log::log(LOG_ERR, "KickoutTeam, arena player kickout %d not found", kickout);
		return;
	}

	if (arenaLeader->team_id == 0) {
		SendError(leader, MSG_ERROR_17);
		Log::log(LOG_ERR, "KickoutTeam, arena player leader %d not in team", leader);
		return;
	}

	if (arenaKickout->team_id == 0) {
		SendError(leader, MSG_ERROR_17);
		Log::log(LOG_ERR, "KickoutTeam, arena player kickout %d not in team", kickout);
		return;
	}

	if (arenaLeader->team_id != arenaKickout->team_id) {
		SendError(leader, MSG_ERROR_22);
		Log::log(LOG_ERR, "KickoutTeam, arena player %d and kickout %d not different team %d = %d", leader, kickout, arenaLeader->team_id, arenaKickout->team_id);
		return;
	}

	EC_SQLArenaTeam* arenaTeam = GetArenaTeamByTeamID(arenaLeader->team_id);
	if (!arenaTeam) {
		SendError(leader, MSG_ERROR_10);
		Log::log(LOG_ERR, "KickoutTeam, arena team %d not found", arenaLeader->team_id);
		return;
	}

	if (arenaTeam->captain_id != leader) 
	{
		SendError(leader, MSG_ERROR_18);
		Log::log(LOG_ERR, "KickoutTeam, arena team %d insufficient privileges (leader:%d, captain:%d)", arenaTeam->team_id, leader, arenaTeam->captain_id);
		return;
	}

	bool found = false;

	std::vector<EC_SQLArenaTeamMember>::iterator it = arenaTeam->members.begin();
	for (; it != arenaTeam->members.end(); it++)
	{
		if (it->player_id == kickout)
		{
			found = true;
			break;
		}
	}

	if (!found) 
	{
		SendError(leader, MSG_ERROR_16);
		Log::log(LOG_ERR, "KickoutTeam, arena player kickout %d not found in team %d", kickout, arenaTeam->team_id);
		return;
	}

	arenaTeam->members.erase(it);

	arenaKickout->team_id = 0;

	SyncPlayerToDB(kickout);
	SyncTeamToDB(arenaTeam->team_id);

	LOG_TRACE("KickoutTeam, arena player %d successfully kickout %d from team %d", leader, kickout, arenaTeam->team_id);

	for (int i = 0; i < (int)arenaTeam->members.size(); i++) {
		SendTeamKickRe(arenaTeam->members[i].player_id, arenaTeam->team_id, arenaTeam->captain_id, kickout);
		SendOnlineNotify(arenaTeam->members[i].player_id);
	}
}

void ArenaOfAuroraManager::LeaveTeam(int roleid)
{
	PlayerInfo* playerInfo = UserContainer::GetInstance().FindRole(roleid);
	if (playerInfo == NULL)
		return;

	EC_SQLArenaPlayer* arenaPlayer = GetArenaPlayerByRoleID(roleid);
	if (!arenaPlayer) {
		SendError(roleid, MSG_ERROR_21);
		Log::log(LOG_ERR, "ArenaTeamLeave, arena player inviter %d not found", roleid);
		return;
	}

	if (arenaPlayer->team_id == 0) {
		SendError(roleid, MSG_ERROR_17);
		Log::log(LOG_ERR, "ArenaTeamLeave, arena player inviter %d not in team", roleid);
		return;
	}

	EC_SQLArenaTeam* arenaTeam = GetArenaTeamByTeamID(arenaPlayer->team_id);
	if (!arenaTeam) {
		SendError(roleid, MSG_ERROR_10);
		Log::log(LOG_ERR, "ArenaTeamLeave, arena team %d not found", arenaPlayer->team_id);
		return;
	}

	if (arenaTeam->captain_id == roleid) 
	{
		SendError(roleid, MSG_ERROR_28);
		Log::log(LOG_ERR, "ArenaTeamLeave, arena team %d captain %d (%d) can't leave", arenaTeam->team_id, arenaTeam->captain_id, roleid);
		return;
	}

	bool found = false;

	std::vector<EC_SQLArenaTeamMember>::iterator it = arenaTeam->members.begin();
	for (; it != arenaTeam->members.end(); it++)
	{
		if (it->player_id == roleid)
		{
			found = true;
			break;
		}
	}

	if (!found) {
		SendError(roleid, MSG_ERROR_16);
		Log::log(LOG_ERR, "ArenaTeamLeave, arena player %d not found in team %d", roleid, arenaPlayer->team_id);
		return;
	}

	arenaTeam->members.erase(it);

	arenaPlayer->team_id = 0;

	SyncPlayerToDB(roleid);
	SyncTeamToDB(arenaTeam->team_id);

	LOG_TRACE("ArenaTeamLeave, arena player %d successfully leaved from team %d", roleid, arenaPlayer->team_id);

	SendTeamLeaveRe(roleid, arenaTeam->team_id, roleid);
	for (int i = 0; i < (int)arenaTeam->members.size(); i++) {
		SendTeamLeaveRe(arenaTeam->members[i].player_id, arenaTeam->team_id, roleid);
		SendOnlineNotify(arenaTeam->members[i].player_id);
	}
}

void ArenaOfAuroraManager::OnStartSearch(int roleid, int cls, Octets & name)
{
	LOG_TRACE("ArenaOfAuroraManager::OnStartSearchSolo: roleid = %d , cls = %d ", roleid, cls );

	PlayerInfo* playerInfo = UserContainer::GetInstance().FindRole(roleid);
	if (playerInfo == NULL)
		return;

	EC_SQLArenaPlayer* p = GetArenaPlayerByRoleID(roleid);
	if (!p)
	{
		// create new arena role
		EC_SQLArenaPlayer player;
		EC_MakeDefaultPlayer(player, 0, roleid,  cls, name);
		OnLoadPlayer(player);

		p = GetArenaPlayerByRoleID(roleid);
		SendArenaInfoToPlayer(roleid);
	}
	
	if (!p)
	{
		SendError(roleid, MSG_ERROR_07);
		Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchSolo, create new player fatal error!!! %d");
		return;
	}
	
	SyncPlayerToDB(roleid);

	// check team in search battle
	const SearchBattle* eSb = GetSearchBattleByTeamId( roleid, false );
	if (eSb)
	{
		SendError(roleid, MSG_ERROR_23);
		Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearch: 1x1 %d in search battle", roleid );
		return;
	}

	// check team in active battle
	int activeBattleId = GetActiveBattleByTeamId( roleid, false );
	if (activeBattleId != -1)
	{
		SendError(roleid, MSG_ERROR_20);
		Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearch: team %d in active battle %d", roleid , activeBattleId);
		return;
	}
	
	// srnd wait 
	SearchBattle sb;
	sb.id = roleid;
	sb.members.push_back(roleid);
	sb.counter = MAX_WAIT_QUEUE;
	sb.score = p->score;
	sb.active = false;
	SendWaitQueue(sb);
	
	// add map
	_search_battle_map_1x1.push_back(sb);
	return;
}

void ArenaOfAuroraManager::OnStartSearch(IntVector & members)
{
	LOG_TRACE("ArenaOfAuroraManager::OnStartSearchTeam: members_size = %d ", members.size() );
	EC_SQLArenaPlayer* p[MAX_MEMBERS] = { NULL, NULL, NULL, NULL, NULL, NULL };
	
	if (members.size() == 3)
	{
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			PlayerInfo* playerInfo = UserContainer::GetInstance().FindRole(members[i]);
			if (playerInfo == NULL)
				return;
		}
		
		// get players
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			p[i] = GetArenaPlayerByRoleID(members[i]);
			if (!p[i]) 
			{
				SendError(members[i], MSG_ERROR_07);
				Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: member[%d](%d) not found in arena", i, members[i] );
				return;
			}
		}
		
		// check team
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			if ( p[0]->team_id != p[i]->team_id )
			{
				SendError(members[i], MSG_ERROR_16);
				Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: bad teamid p[0]->team_id(%d) != p[%d]->team_id(%d) ", p[0]->team_id , i, p[i]->team_id);
				return;
			}
		}

		// check team in search battle
		const SearchBattle* eSb = GetSearchBattleByTeamId( p[0]->team_id, true );
		if (eSb)
		{
			for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
			{
				SendError(members[i], MSG_ERROR_23);
			}
			Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: team %d in search battle", p[0]->team_id );
			return;
		}
		
		// check team in active battle
		int activeBattleId = GetActiveBattleByTeamId( p[0]->team_id, true );
		if (activeBattleId != -1)
		{
			for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
			{
				SendError(members[i], MSG_ERROR_20);
			}
			Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: team %d in active battle %d", p[0]->team_id , activeBattleId);
			return;
		}

		// send client
		SearchBattle sb;
		sb.id = p[0]->team_id;
		sb.members = members;
		sb.counter = MAX_WAIT_QUEUE;
		sb.score = p[0]->team_score;
		sb.active = false;
		SendWaitQueue(sb);
		
		// add team to search battle
		_search_battle_map_3x3.push_back(sb);
		return;
	}
	
	if (members.size() == 6)
	{
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			PlayerInfo* playerInfo = UserContainer::GetInstance().FindRole(members[i]);
			if (playerInfo == NULL)
				return;
		}
		
		// get players
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			p[i] = GetArenaPlayerByRoleID(members[i]);
			if (!p[i]) 
			{
				SendError(members[i], MSG_ERROR_07);
				Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: member[%d](%d) not found in arena", i, members[i] );
				return;
			}
		}
		
		// check team
		for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
		{
			if ( p[0]->team_id != p[i]->team_id )
			{
				SendError(members[i], MSG_ERROR_16);
				Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: bad teamid p[0]->team_id(%d) != p[%d]->team_id(%d) ", p[0]->team_id , i, p[i]->team_id);
				return;
			}
		}

		// check team in search battle
		const SearchBattle* eSb = GetSearchBattleByTeamId( p[0]->team_id, true );
		if (eSb)
		{
			for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
			{
				SendError(members[i], MSG_ERROR_23);
			}
			Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: team %d in search battle", p[0]->team_id );
			return;
		}
		
		// check team in active battle
		int activeBattleId = GetActiveBattleByTeamId( p[0]->team_id, true );
		if (activeBattleId != -1)
		{
			for (size_t i = 0; i < members.size() && i < MAX_MEMBERS; i++)
			{
				SendError(members[i], MSG_ERROR_20);
			}
			Log::log(LOG_ERR, "ArenaOfAuroraManager::OnStartSearchTeam: team %d in active battle %d", p[0]->team_id , activeBattleId);
			return;
		}

		// send client
		SearchBattle sb;
		sb.id = p[0]->team_id;
		sb.members = members;
		sb.counter = MAX_WAIT_QUEUE;
		sb.score = p[0]->team_score;
		sb.active = false;
		SendWaitQueue(sb);
		
		// add team to search battle
		_search_battle_map_6x6.push_back(sb);
		return;
	}
	
}

void ArenaOfAuroraManager::SearchClearTimeout()
{
	_search_battle_map_1x1.erase(std::remove_if(_search_battle_map_1x1.begin(), _search_battle_map_1x1.end(), [](SearchBattle& sb) {
        if (sb.counter-- <= 0)
		{
			GetInstance()->SendWaitQueueTimeOut(sb);
			LOG_TRACE("ArenaOfAuroraManager SearchClearTimeout 1x1 battle time out = %d \n", sb.id );
			return true;
		}
		return false;
    }), _search_battle_map_1x1.end());
	
    _search_battle_map_3x3.erase(std::remove_if(_search_battle_map_3x3.begin(), _search_battle_map_3x3.end(), [](SearchBattle& sb) {
        if (sb.counter-- <= 0)
		{
			GetInstance()->SendWaitQueueTimeOut(sb);
			LOG_TRACE("ArenaOfAuroraManager SearchClearTimeout 3x3 battle time out = %d \n", sb.id );
			return true;
		}
		return false;
    }), _search_battle_map_3x3.end());	

    _search_battle_map_6x6.erase(std::remove_if(_search_battle_map_6x6.begin(), _search_battle_map_6x6.end(), [](SearchBattle& sb) {
        if (sb.counter-- <= 0)
		{
			GetInstance()->SendWaitQueueTimeOut(sb);
			LOG_TRACE("ArenaOfAuroraManager SearchClearTimeout 3x3 battle time out = %d \n", sb.id );
			return true;
		}
		return false;
    }), _search_battle_map_6x6.end());
}

// search sort score 
bool ArenaOfAuroraManager::CompareSearchBattle(const SearchBattle& t1, const SearchBattle& t2) 
{
    return t1.score > t2.score;
}
// search sort score end

bool ArenaOfAuroraManager::MakeActiveBattle1x1( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb )
{
	ServerInfo* serverInfo = GetRandomServer();
	if (serverInfo == NULL)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager SearchProcessing error not found arena server");
		return false;
	}
	
	ab.arena_id = GetFreeArenaID();
	ab.world_tag = serverInfo->world_tag;
	
	ab.red_team.team_id = -1;
	ab.blue_team.team_id = -1;	
	
	ab.red_members.clear();
	for (size_t i = 0; i < red_sb.members.size() && i < 1; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(red_sb.members[0]);
		if (Member)
		{
			ab.red_members.push_back(*Member);
		}
	}

	ab.blue_members.clear();
	for (size_t i = 0; i < blue_sb.members.size() && i < 1; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(blue_sb.members[0]);
		if (Member)
		{
			ab.blue_members.push_back(*Member);
		}
	}
	
	return (ab.red_members.size() >= 1 && ab.blue_members.size() >= 1);
}

bool ArenaOfAuroraManager::MakeActiveBattle3x3( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb )
{
	ServerInfo* serverInfo = GetRandomServer();
	if (serverInfo == NULL)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager SearchProcessing error not found arena server");
		return false;
	}
	
	ab.arena_id = GetFreeArenaID();
	ab.world_tag = serverInfo->world_tag;
	

	EC_SQLArenaTeam* RedTeam = GetArenaTeamByTeamID(red_sb.id);
	if (!RedTeam) 
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::MakeActiveBattle3x3, arena team %d not found!", red_sb.id);
		return false;
	}
	
	EC_SQLArenaTeam* BlueTeam = GetArenaTeamByTeamID(blue_sb.id);
	if (!BlueTeam) 
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::MakeActiveBattle3x3, arena team %d not found!", blue_sb.id);
		return false;
	}
	
	ab.red_team = *RedTeam;
	ab.blue_team = *BlueTeam;
	
	ab.red_members.clear();
	for (size_t i = 0; i < red_sb.members.size() && i < 3; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(red_sb.members[0]);
		if (Member)
		{
			ab.red_members.push_back(*Member);
		}
	}

	ab.blue_members.clear();
	for (size_t i = 0; i < blue_sb.members.size() && i < 3; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(blue_sb.members[0]);
		if (Member)
		{
			ab.blue_members.push_back(*Member);
		}
	}
	
	return (ab.red_members.size() >= 1 && ab.blue_members.size() >= 1);
}

bool ArenaOfAuroraManager::MakeActiveBattle6x6( ActiveBattle & ab , SearchBattle& red_sb , SearchBattle& blue_sb )
{
	ServerInfo* serverInfo = GetRandomServer();
	if (serverInfo == NULL)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager SearchProcessing error not found arena server");
		return false;
	}
	
	ab.arena_id = GetFreeArenaID();
	ab.world_tag = serverInfo->world_tag;
	

	EC_SQLArenaTeam* RedTeam = GetArenaTeamByTeamID(red_sb.id);
	if (!RedTeam) 
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::MakeActiveBattle3x3, arena team %d not found!", red_sb.id);
		return false;
	}
	
	EC_SQLArenaTeam* BlueTeam = GetArenaTeamByTeamID(blue_sb.id);
	if (!BlueTeam) 
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::MakeActiveBattle3x3, arena team %d not found!", blue_sb.id);
		return false;
	}
	
	ab.red_team = *RedTeam;
	ab.blue_team = *BlueTeam;
	
	ab.red_members.clear();
	for (size_t i = 0; i < red_sb.members.size() && i < 6; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(red_sb.members[0]);
		if (Member)
		{
			ab.red_members.push_back(*Member);
		}
	}

	ab.blue_members.clear();
	for (size_t i = 0; i < blue_sb.members.size() && i < 6; i++)
	{
		EC_SQLArenaPlayer* Member = GetArenaPlayerByRoleID(blue_sb.members[0]);
		if (Member)
		{
			ab.blue_members.push_back(*Member);
		}
	}
	
	return (ab.red_members.size() >= 1 && ab.blue_members.size() >= 1);
}

void ArenaOfAuroraManager::SearchProcessing(time_t now_time)
{
	SearchClearTimeout();
	
	// 1x1 battle start manager
	if ( !(now_time % 30) )
	{
		LOG_TRACE("ArenaOfAuroraManager SearchProcessing 1x1 battle size = %d \n", _search_battle_map_1x1.size() );
		std::sort(_search_battle_map_1x1.begin(), _search_battle_map_1x1.end(), CompareSearchBattle);
		
		while ( _search_battle_map_1x1.size() > 1 )
		{
			SearchBattle* sb_first = &_search_battle_map_1x1[0];
			SearchBattle* sb_second = &_search_battle_map_1x1[1];
		
			if (sb_first && sb_second && sb_first->members.size() == 1 && sb_second->members.size() == 1)
			{
				LOG_TRACE("ArenaOfAuroraManager SearchProcessing found teams %d and %d\n", sb_first->id, sb_second->id);
		
				// Add to active battle
				ActiveBattle ab;
				if ( MakeActiveBattle1x1( ab, *sb_first, *sb_second ) )
				{
					_active_battles_map[ab.arena_id] = ab;
					_active_teams_battles_map_1x1[sb_first->members[0]] = ab.arena_id;
					_active_teams_battles_map_1x1[sb_second->members[0]] = ab.arena_id;
				}
		
				// Start battle
				BeginBattle(ab);
			}
			
			_search_battle_map_1x1.erase(_search_battle_map_1x1.begin());
			_search_battle_map_1x1.erase(_search_battle_map_1x1.begin());
		}
	}
	
	// 3x3 battle start manager
	if ( !(now_time % 60) )
	{
		LOG_TRACE("ArenaOfAuroraManager SearchProcessing 3x3 battle size = %d \n", _search_battle_map_3x3.size() );
		std::sort(_search_battle_map_3x3.begin(), _search_battle_map_3x3.end(), CompareSearchBattle);
		
		while ( _search_battle_map_3x3.size() > 1 )
		{
			SearchBattle* sb_first = &_search_battle_map_3x3[0];
			SearchBattle* sb_second = &_search_battle_map_3x3[1];

			if (sb_first && sb_second && sb_first->members.size() == 1 && sb_second->members.size() == 1)
			{
				LOG_TRACE("ArenaOfAuroraManager SearchProcessing found teams %d and %d\n", sb_first->id, sb_second->id);

				ServerInfo* serverInfo = GetRandomServer();
				if (serverInfo == NULL)
				{
					Log::log(LOG_ERR, "ArenaOfAuroraManager SearchProcessing error not found arena server");
					return;
				}

				// Add to active battle
				ActiveBattle ab;
				if ( MakeActiveBattle3x3( ab, *sb_first, *sb_second ) )
				{
					_active_battles_map[ab.arena_id] = ab;
					_active_teams_battles_map_3x3[sb_first->id] = ab.arena_id;
					_active_teams_battles_map_3x3[sb_second->id] = ab.arena_id;
				}

				// Start battle
				BeginBattle(ab);
			}
			
			_search_battle_map_3x3.erase(_search_battle_map_3x3.begin());
			_search_battle_map_3x3.erase(_search_battle_map_3x3.begin());
		}
	}
	
	// 6x6 battle start manager
	if ( !(now_time % 60) )
	{
		LOG_TRACE("ArenaOfAuroraManager SearchProcessing 6x6 battle size = %d \n", _search_battle_map_6x6.size() );
		std::sort(_search_battle_map_6x6.begin(), _search_battle_map_6x6.end(), CompareSearchBattle);

		while ( _search_battle_map_6x6.size() > 1 )
		{
			SearchBattle* sb_first = &_search_battle_map_6x6[0];
			SearchBattle* sb_second = &_search_battle_map_6x6[1];

			if (sb_first && sb_second && sb_first->members.size() == 1 && sb_second->members.size() == 1)
			{
				LOG_TRACE("ArenaOfAuroraManager SearchProcessing found teams %d and %d\n", sb_first->id, sb_second->id);

				ServerInfo* serverInfo = GetRandomServer();
				if (serverInfo == NULL)
				{
					Log::log(LOG_ERR, "ArenaOfAuroraManager SearchProcessing error not found arena server");
					return;
				}

				// Add to active battle
				ActiveBattle ab;
				if ( MakeActiveBattle3x3( ab, *sb_first, *sb_second ) )
				{
					_active_battles_map[ab.arena_id] = ab;
					_active_teams_battles_map_3x3[sb_first->id] = ab.arena_id;
					_active_teams_battles_map_3x3[sb_second->id] = ab.arena_id;
				}

				// Start battle
				BeginBattle(ab);
			}
			
			_search_battle_map_6x6.erase(_search_battle_map_6x6.begin());
			_search_battle_map_6x6.erase(_search_battle_map_6x6.begin());
		}
	}
}

void ArenaOfAuroraManager::BeginBattle(ActiveBattle & battle)
{
	// get serverinfo by worldtag
	ServerInfo* serverInfo = GetServerInfoByWorldTag(battle.world_tag);
	if (serverInfo == NULL)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::BeginBattle, error not found arena server. worldtag=%d", battle.world_tag);
		return;
	}

	// send packet to start arena battle
	EC_ArenaBattleStart proto;
	{
		proto.arena_id = battle.arena_id;

		proto.red_team = battle.red_team;
		proto.red_members = battle.red_members;

		proto.blue_team = battle.blue_team;
		proto.blue_members = battle.blue_members;

		proto.end_time = GetTime() + 1800;

		GProviderServer::GetInstance()->DispatchProtocol(serverInfo->server_id, proto);
	}

	WaiterInfo wi;
	wi.arena_id = battle.arena_id;
	wi.world_tag = battle.world_tag;
	wi.counter = MAX_WAIT_BATTLE_QUEUE;

	LOG_TRACE("ArenaOfAuroraManager::BeginBattle, WaiterInfo=[arena_id=%d, world_tag=%d]", wi.arena_id, wi.world_tag);

	// add red members to waiter
	for (size_t i = 0; i < battle.red_members.size() && i < MAX_MEMBERS; i++)
	{
		_players_waiting_enter_map[battle.red_members[i].player_id] = wi;
	}

	// add blue members to waiter
	for (size_t i = 0; i < battle.blue_members.size() && i < MAX_MEMBERS; i++)
	{
		_players_waiting_enter_map[battle.blue_members[i].player_id] = wi;
	}

	// send notice to red members
	SendNoticeBattle( battle.red_team , battle.red_members , wi.counter );

	// send notice to blue members
	SendNoticeBattle( battle.blue_team , battle.blue_members , wi.counter );

	LOG_TRACE("ArenaOfAuroraManager::BeginBattle, arena_id=%d, world_tag=%d, end_time=%d", proto.arena_id, serverInfo->world_tag, proto.end_time);
}

void ArenaOfAuroraManager::BeginEnter(int roleid)
{
	// get current gameid of player if his found in online
	int gameId = GetGameIdByRoleId(roleid);
	if (gameId == -1)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::BeginEnter, player %d not found gameid", roleid);
		return;
	}

	// get iterator of player waiting if he wait battle
	PLAYERS_WAITING_ENTER_MAP::iterator it = _players_waiting_enter_map.find(roleid);
	if (it == _players_waiting_enter_map.end()) {
		SendError(roleid, MSG_ERROR_27);
		Log::log(LOG_ERR, "ArenaOfAuroraManager::BeginEnter, player %d not found in _players_waiting_enter_map", roleid);
		return;
	}

	// get WaiterInfo from iterator
	WaiterInfo* waiterInfo = &(it->second);

	LOG_TRACE("ArenaOfAuroraManager::BeginEnter, WaiterInfo=[arena_id=%d, world_tag=%d]", waiterInfo->arena_id, waiterInfo->world_tag);

	// check started gs by worldtag (arena of aurora)
	if (!GetServerInfoByWorldTag(waiterInfo->world_tag))
	{
		SendError(roleid, MSG_ERROR_37);
		Log::log(LOG_ERR, "ArenaOfAuroraManager::BeginBattle, error not found arena server. worldtag=%d, player=%d", waiterInfo->world_tag, roleid);
		return;
	}

	// send packet enter to arena of aurora
	{
		EC_ArenaBattleEnter proto;

		proto.roleid = roleid;
		proto.arena_id = waiterInfo->arena_id;
		proto.world_tag = waiterInfo->world_tag;

		LOG_TRACE("ArenaOfAuroraManager::BeginEnter, roleid=%d, gameid=%d, arena_id=%d, world_tag=%d", roleid, gameId, proto.arena_id, proto.world_tag);

		GProviderServer::GetInstance()->DispatchProtocol(gameId, proto);
	}

	// remove iterator from waiting map
	 _players_waiting_enter_map.erase(it);
	 
	 // send client end 
	 SendNoticeBattleBegin(roleid);
}

void ArenaOfAuroraManager::SendTeamCreateRe(int retcode, int roleid, int teamid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) {
		Log::log(LOG_ERR, "SendTeamCreateRe, player info %d not found!", roleid);
		return;
	}

	Octets info;
	SendArenaInfoToPlayer(roleid, teamid, NOTIFY_CREATE, info );

	//ArenaTeamCreate_Re proto;
	//
	//proto.retcode = retcode;
	//proto.roleid = roleid;
	//proto.localsid = pInfo->localsid;
	//proto.teamid = teamid;
	//
	//GDeliveryServer::GetInstance()->Send(pInfo->linksid, proto);
}

void ArenaOfAuroraManager::SendTeamChangeRe(int roleid, int team_id, int capitan_id, int new_leader)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) {
		Log::log(LOG_ERR, "SendTeamChangeRe, player info %d not found!", roleid);
		return;
	}

	int dd_info[2];
	dd_info[0] = byteorder_32(capitan_id);
	dd_info[1] = byteorder_32(new_leader);

	Octets info;
	info.replace(&dd_info, sizeof(dd_info));
	
	SendArenaInfoToPlayer(roleid, team_id, NOTIFY_TRANCFER_CAPITAN, info);

	/*
	ArenaTeamChange_Re proto;

	proto.retcode = 0;
	proto.roleid = roleid;
	proto.localsid = pInfo->localsid;
	proto.newleader_roleid = new_leader;

	GDeliveryServer::GetInstance()->Send(pInfo->linksid, proto);
	*/
}

void ArenaOfAuroraManager::SendTeamKickRe(int roleid, int team_id, int capitan_id, int kicked)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) {
		Log::log(LOG_ERR, "SendTeamKickRe, player info %d not found!", roleid);
		return;
	}

	int dd_info[2];
	dd_info[0] = byteorder_32(capitan_id);
	dd_info[1] = byteorder_32(kicked);

	Octets info;
	info.replace(&dd_info, sizeof(dd_info));
	
	SendArenaInfoToPlayer(roleid, team_id, NOTIFY_KICK_PLAYER, info);

	/*
	ArenaTeamKick_Re proto;

	proto.retcode = 0;
	proto.roleid = roleid;
	proto.localsid = pInfo->localsid;
	proto.kicked_roleid = kicked;

	GDeliveryServer::GetInstance()->Send(pInfo->linksid, proto);
	*/
}

void ArenaOfAuroraManager::SendTeamLeaveRe(int roleid, int team_id, int leaved)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) {
		Log::log(LOG_ERR, "SendTeamLeaveRe, player info %d not found!", roleid);
		return;
	}

	int dd_info = byteorder_32(leaved);
	
	Octets info;
	info.replace(&dd_info, sizeof(dd_info));
	SendArenaInfoToPlayer(roleid, team_id, NOTIFY_LEAVE, info);
	
	/*
	ArenaTeamLeave_Re proto;

	proto.retcode = 0;
	proto.roleid = roleid;
	proto.localsid = pInfo->localsid;
	proto.leaved_roleid = leaved;

	GDeliveryServer::GetInstance()->Send(pInfo->linksid, proto);
	*/
	
}

/*
void ArenaOfAuroraManager::SendBattleResult(int roleid, ArenaBattleResult proto)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) {
		LOG_TRACE("ArenaOfAuroraManager::SendBattleResult, player info %d not found!", roleid);
		return;
	}

	
	proto.roleid = roleid;
	proto.localsid = pInfo->localsid;

	GDeliveryServer::GetInstance()->Send(pInfo->linksid, proto);
	
}
*/

void ArenaOfAuroraManager::TeamIdx( long long & idx )
{
	EC_CrossTeam ec(idx);
	idx = ec.id;
}

void ArenaOfAuroraManager::RoleIdx( long long & idx )
{
	EC_CrossRole ec(idx, GDeliveryServer::GetInstance()->zoneid);
	idx = ec.id;
}

void ArenaOfAuroraManager::EC_TeamConvert(EC_ArenaTeam & team, const EC_SQLArenaTeam & in)
{
	team.team_id = in.team_id;
	team.captain_id = in.captain_id;
	team.team_type = in.team_type;
	team.score = in.score;
	team.name = in.name;
	team.members = in.members;
	team.teams = in.teams;
	team.last_visite = in.last_visite;
	team.win_count = in.win_count;
	team.team_score = in.team_score;
	team.week_battle_count = in.week_battle_count;
	team.create_time = in.create_time;
	team.battle_count = in.battle_count;

	TeamIdx(team.team_id);
	RoleIdx(team.captain_id);
	team.team_score = in.score;
	
	for (size_t j = 0; j < in.members.size(); j++)
	{
		EC_ArenaTeamMember member;
		member.player_id = in.members[j].player_id;
		RoleIdx(member.player_id);
		member.award_count = 0;
		team.members.push_back(member);
	}
}

void ArenaOfAuroraManager::EC_PlayerConvert(EC_ArenaPlayer & player, const EC_SQLArenaPlayer & in)
{
	player.player_id =          in.player_id;
	player.team_id =            in.team_id;
	player.cls =                in.cls;
	player.score =              in.score;
	player.win_count =          in.win_count;
	player.team_score =         in.team_score;
	player.week_battle_count =  in.week_battle_count;
	player.invite_time =        in.invite_time;
	player.last_visite =        in.last_visite;
	player.battle_count =       in.battle_count;
	
	RoleIdx(player.player_id);
	TeamIdx(player.team_id);
}

void ArenaOfAuroraManager::EC_TeamSyncConvert(EC_ArenaTeamSync & team, const EC_SQLArenaTeam & in)
{
	team.team_id = in.team_id;
	team.captain_id = in.captain_id;
	team.team_type = in.team_type;
	team.score = in.score;
	team.name = in.name;
	team.members = in.members;
	team.teams = in.teams;
	team.last_visite = in.last_visite;
	team.win_count = in.win_count;
	team.team_score = in.team_score;
	team.week_battle_count = in.week_battle_count;
	team.create_time = in.create_time;
	team.battle_count = in.battle_count;

	TeamIdx(team.team_id);
	RoleIdx(team.captain_id);
	team.team_score = in.score;
	
	for (size_t j = 0; j < in.members.size(); j++)
	{
		EC_ArenaTeamMember member;
		member.player_id = in.members[j].player_id;
		RoleIdx(member.player_id);
		member.award_count = 0;
		team.members.push_back(member);
	}
	
	team.sync = 0;
}

void ArenaOfAuroraManager::EC_PlayerSyncConvert(EC_ArenaPlayerSync & player, const EC_SQLArenaPlayer & in)
{
	player.player_id =          in.player_id;
	player.team_id =            in.team_id;
	player.cls =                in.cls;
	player.score =              in.score;
	player.win_count =          in.win_count;
	player.team_score =         in.team_score;
	player.week_battle_count =  in.week_battle_count;
	player.invite_time =        in.invite_time;
	player.last_visite =        in.last_visite;
	player.battle_count =       in.battle_count;
	
	RoleIdx(player.player_id);
	TeamIdx(player.team_id);
	
	player.sync = 0;
}

void ArenaOfAuroraManager::SendArenaInfoToPlayer(int roleid)
{
	if (roleid <= 0)
		return;

	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) 
	{
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", roleid);
		return;
	}
	
	EC_ArenaQuery_Re aq_re(ERR_SUCCESS ,roleid);
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(roleid);
	if (!pPlayer) 
	{
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, aq_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena player %d not found!", roleid);
		return;
	}

	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(pPlayer->team_id);
	if (!pTeam) 
	{
		// member
		{
			EC_ArenaPlayerSync member;
			EC_PlayerSyncConvert(member, *pPlayer);
			aq_re.members.push_back(member);
		}
		
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, aq_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found!", pPlayer->team_id);
		return;
	}

	bool success = true;

	// Check Captain
	if (pTeam->captain_id != roleid)
	{
		if (!GetArenaPlayerByRoleID(pTeam->captain_id))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arenateam %d not found arenaplayer captain %d!", pTeam->team_id, pTeam->captain_id);
			success = false;
		}
	}

	// Check Members
	for (int i = 0; i < (int)pTeam->members.size(); i++)
	{
		int memberId = pTeam->members[i].player_id;
		if (memberId == roleid)
			continue;

		if (!GetArenaPlayerByRoleID(memberId))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found arenaplayer member %d!", pTeam->team_id, memberId);
			success = false;
		}
	}

	if (!success)
		return;
	
	// team
	EC_ArenaTeamSync out_team;
	EC_TeamSyncConvert(out_team, *pTeam);
	aq_re.teams.push_back(out_team);	
	
	// members
	for (size_t i = 0; i < pTeam->members.size(); i++)
	{
		EC_SQLArenaPlayer * in = GetArenaPlayerByRoleID(pTeam->members[i].player_id);
		if (in)
		{
			EC_ArenaPlayerSync out_member;
			EC_PlayerSyncConvert(out_member, *in);
			aq_re.members.push_back(out_member);
		}
	}

	GDeliveryServer::GetInstance()->Send(pInfo->linksid, aq_re);
	
	// is wait queue 
	SendWaitQueue(roleid);
	
	// is member online
	SendOnlineNotify(roleid, true);
}

void ArenaOfAuroraManager::SendArenaInfoToPlayer(int roleid, int team_id, int reason, Octets & info)
{
	if (roleid <= 0)
		return;

	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) 
	{
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", roleid);
		return;
	}
	
	EC_ArenaTeamDataNotify atdn_re(ERR_SUCCESS ,roleid, reason);
	atdn_re.info = info;
	
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(roleid);
	if (!pPlayer) 
	{
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, atdn_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena player %d not found!", roleid);
		return;
	}

	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(team_id);
	if (!pTeam) 
	{
		// member
		{
			EC_ArenaPlayerSync member;
			EC_PlayerSyncConvert(member, *pPlayer);
			atdn_re.members.push_back(member);
		}	
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, atdn_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found!", team_id);
		return;
	}

	bool success = true;

	// Check Captain
	if (pTeam->captain_id != roleid)
	{
		if (!GetArenaPlayerByRoleID(pTeam->captain_id))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arenateam %d not found arenaplayer captain %d!", pTeam->team_id, pTeam->captain_id);
			success = false;
		}
	}

	// Check Members
	for (int i = 0; i < (int)pTeam->members.size(); i++)
	{
		int memberId = pTeam->members[i].player_id;
		if (memberId == roleid)
			continue;

		if (!GetArenaPlayerByRoleID(memberId))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found arenaplayer member %d!", pTeam->team_id, memberId);
			success = false;
		}
	}

	if (!success)
		return;
	
	// team
	EC_TeamSyncConvert(atdn_re.teams, *pTeam);	
	
	// members
	for (size_t i = 0; i < pTeam->members.size(); i++)
	{
		EC_SQLArenaPlayer * in = GetArenaPlayerByRoleID(pTeam->members[i].player_id);
		if (in)
		{
			EC_ArenaPlayerSync out_member;
			EC_PlayerSyncConvert(out_member, *in);
			atdn_re.members.push_back(out_member);
		}
	}
	
	GDeliveryServer::GetInstance()->Send(pInfo->linksid, atdn_re);
}


void ArenaOfAuroraManager::SendArenaInfoToPlayer(int roleid, int dest_roleid)
{
	if (roleid <= 0 || dest_roleid <= 0)
		return;

	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) 
	{
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", roleid);
		return;
	}
	
	EC_ArenaPlayerTotalInfoQuery_Re aptiq_re(ERR_SUCCESS, roleid);
	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(dest_roleid);
	if (!pPlayer) 
	{
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, aptiq_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena player %d not found!", dest_roleid);
		return;
	}

	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(pPlayer->team_id);
	if (!pTeam) 
	{
		// member
		{
			EC_ArenaPlayer member;
			EC_PlayerConvert(member, *pPlayer);
			aptiq_re.members.push_back(member);
		}
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, aptiq_re);
		Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found!", pPlayer->team_id);
		return;
	}

	bool success = true;

	// Check Captain
	if (pTeam->captain_id != dest_roleid)
	{
		if (!GetArenaPlayerByRoleID(pTeam->captain_id))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arenateam %d not found arenaplayer captain %d!", pTeam->team_id, pTeam->captain_id);
			success = false;
		}
	}

	// Check Members
	for (int i = 0; i < (int)pTeam->members.size(); i++)
	{
		int memberId = pTeam->members[i].player_id;
		if (memberId == dest_roleid)
			continue;

		if (!GetArenaPlayerByRoleID(memberId))
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, arena team %d not found arenaplayer member %d!", pTeam->team_id, memberId);
			success = false;
		}
	}

	if (!success)
		return;

	// team
	EC_ArenaTeam out_team;
	EC_TeamConvert(out_team, *pTeam);
	aptiq_re.teams.push_back(out_team);
	
	// members
	for (size_t i = 0; i < pTeam->members.size(); i++)
	{
		EC_SQLArenaPlayer * in = GetArenaPlayerByRoleID(pTeam->members[i].player_id);
		if (in)
		{
			EC_ArenaPlayer out_member;
			EC_PlayerConvert(out_member, *in);
			aptiq_re.members.push_back(out_member);
		}
	}
	
	GDeliveryServer::GetInstance()->Send(pInfo->linksid, aptiq_re);
}

void ArenaOfAuroraManager::EC_PlayerInviteRequest(int roleid, int invited_roleid)
{
	GDeliveryServer* dsm = GDeliveryServer::GetInstance();
	LOG_TRACE("ArenaSendInvite roleid=%d, invited_roleid=%d", roleid, invited_roleid);

	PlayerInfo* inviter = UserContainer::GetInstance().FindRole(roleid);
	if (inviter == NULL)
	{
		return;
	}

	PlayerInfo* invitee = UserContainer::GetInstance().FindRole(invited_roleid);
	if (invitee == NULL)
	{
		dsm->Send(inviter->linksid, RoleStatusAnnounce(_ZONE_INVALID, _ROLE_INVALID, inviter->localsid, _STATUS_OFFLINE, Octets(0)));
		return;
	}

	EC_SQLArenaPlayer* apInviter = GetArenaPlayerByRoleID(roleid);
	if (!apInviter)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena player inviter %d not found", roleid);
		return;
	}

	if (apInviter->team_id == 0)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena player inviter %d not in team", roleid);
		return;
	}

	EC_SQLArenaTeam* teamInfo = GetArenaTeamByTeamID(apInviter->team_id);
	if (!teamInfo)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena team %d not found", apInviter->team_id);
		return;
	}

	if (teamInfo->captain_id != roleid)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena team %d insufficient privileges (inviter:%d, captain:%d)", apInviter->team_id, roleid, teamInfo->captain_id);
		return;
	}

	if (teamInfo->members.size() > 3)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena team %d limit members", teamInfo->team_id);
		return;
	}

	EC_SQLArenaPlayer* apInvitee = GetArenaPlayerByRoleID(invited_roleid);
	if (apInvitee && apInvitee->team_id != 0)
	{
		Log::log(LOG_ERR, "ArenaSendInvite, arena player invitee %d in team %d", invited_roleid, apInvitee->team_id);
		return;
	}

	// get current gameid of player if his found in online
	int gameId = GetGameIdByRoleId(invited_roleid);
	if (gameId == -1)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::BeginEnter, player %d not found gameid", roleid);
		return;
	}

	EC_ArenaTeamInviteRequest_Re atir_re(invited_roleid, roleid, MODE_3X3);
	GProviderServer::GetInstance()->DispatchProtocol(gameId, atir_re);
}

void ArenaOfAuroraManager::SendWaitQueue( SearchBattle & sb )
{
	// check members size
	if (sb.members.size() != 1 && sb.members.size() != 3 && sb.members.size() != 6)
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::SendWaitQueue: bad size = %d ", sb.members.size() );
		return;
	}
	
	int mode = sb.members.size() == 6 ? MSG_START_QUEUE_TEAM_6X6 : sb.members.size() == 3 ? MSG_START_QUEUE_TEAM_3X3 : MSG_START_QUEUE_SOLO_3X3;
	
	// send packets
	for (size_t i = 0; i < sb.members.size(); i++)
	{
		if (sb.members[i] <= 0)
			continue;
		
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(sb.members[i]);
		if (!pInfo) 
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", sb.members[i]);
			continue;
		}
		
		EC_ArenaClientNotify acn(ERR_SUCCESS, sb.members[i], 0, 5603);
		Marshal::OctetsStream os;
		os << mode;
		os << CompactUINT(sb.members.size());
		for (size_t i = 0; i < sb.members.size(); i++)
		{
			long long role = sb.members[i]; RoleIdx(role); os << role;
		}
		os << sb.counter;
		acn.success_data = os;
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, acn);
	}
}

void ArenaOfAuroraManager::SendWaitQueueTimeOut( SearchBattle & sb )
{
	for (size_t i = 0; i < sb.members.size(); i++)
	{
		if (sb.members[i] <= 0)
			continue;
		
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(sb.members[i]);
		if (!pInfo) 
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", sb.members[i]);
			continue;
		}
		
		EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, sb.members[i], 4);
		Marshal::OctetsStream os;
		os << CompactUINT(sb.members.size());
		for (size_t i = 0; i < sb.members.size(); i++)
		{
			long long role = sb.members[i]; RoleIdx(role); os << role;
		}
		long long team = sb.id; TeamIdx(team); os << team;
		os << 0;
		abinc.success_data = os;
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, abinc);		
	}
}

void ArenaOfAuroraManager::SendNoticeBattle( EC_SQLArenaTeam & pTeam , EC_SQLArenaPlayerVector & members , int counter )
{
	LOG_TRACE("ArenaOfAuroraManager::SendNoticeBattle: teamid=%d, member_size=%d counter=%d", pTeam.team_id, members.size(), counter);
	
	for (size_t i = 0; i < members.size(); i++)
	{
		if (members[i].player_id <= 0)
			continue;
		
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(members[i].player_id);
		if (!pInfo) 
		{
			Log::log(LOG_ERR, "SendArenaInfoToPlayer, player info %d not found!", members[i].player_id);
			continue;
		}
		
		EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, members[i].player_id, 4);
		Marshal::OctetsStream os;
		os << CompactUINT(members.size());
		for (size_t i = 0; i < members.size(); i++)
		{
			long long role = members[i].player_id; RoleIdx(role); os << role;
			LOG_TRACE("ArenaOfAuroraManager::SendNoticeBattle: members[%d]=%d", i, members[i].player_id);
		}
		long long team = 0; os << team;
		os << counter;
		abinc.success_data = os;
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, abinc);		
	}
}

void ArenaOfAuroraManager::SendNoticeBattleBegin(int roleid)
{
	if ( roleid <= 0 )
		return;
	
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline( roleid );
	if (!pInfo) 
	{
		Log::log(LOG_ERR, "SendNoticeBattleBegin, player info %d not found!", roleid);
		return;
	}
	
	EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, roleid, 4);
	Marshal::OctetsStream os;
	os << CompactUINT(1);
	long long role = roleid; RoleIdx(role); os << role;
	long long team = 0; os << team;
	os << 0;
	abinc.success_data = os;
	GDeliveryServer::GetInstance()->Send(pInfo->linksid, abinc);
}

void ArenaOfAuroraManager::WaiterInfoClearTimeout()
{
	for (PLAYERS_WAITING_ENTER_MAP::iterator it = _players_waiting_enter_map.begin(), next_it = it; it != _players_waiting_enter_map.end(); it = next_it)
	{
		++next_it;

		WaiterInfo& wi = it->second;
		
		if ( wi.counter-- < 0 )
		{
			_players_waiting_enter_map.erase(it);
		}
		
	}
}

void ArenaOfAuroraManager::SendWaitQueue( int roleid )
{
	// is wait queue 1x1
	for (SearchBattle & sb : _search_battle_map_1x1)
	{
		for (size_t i = 0; i < sb.members.size(); i++)
		{
			if ( roleid == sb.members[i] )
			{
				SendWaitQueue(sb);
				return;
			}
		}
	}

	// is wait queue 3x3
	for (SearchBattle & sb : _search_battle_map_3x3)
	{
		for (size_t i = 0; i < sb.members.size(); i++)
		{
			if ( roleid == sb.members[i] )
			{
				SendWaitQueue(sb);
				return;
			}
		}
	}

	// is wait queue 6x6
	for (SearchBattle & sb : _search_battle_map_6x6)
	{
		for (size_t i = 0; i < sb.members.size(); i++)
		{
			if ( roleid == sb.members[i] )
			{
				SendWaitQueue(sb);
				return;
			}
		}
	}	
	
}

void ArenaOfAuroraManager::SendError( int roleid, int message )
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (!pInfo) 
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager::SendError, player info %d not found!", roleid);
		return;
	}
	
	EC_ArenaClientNotify acn(ERR_SUCCESS, roleid, message, 5603);
	GDeliveryServer::GetInstance()->Send(pInfo->linksid, acn);
	return;
}

void ArenaOfAuroraManager::SendOnlineNotify(int roleid, bool self)
{
	if (roleid <= 0)
		return;

	EC_SQLArenaPlayer* pPlayer = GetArenaPlayerByRoleID(roleid);
	if (!pPlayer) 
	{
		Log::log(LOG_ERR, "SendOnlineNotify, arena player %d not found!", roleid);
		return;
	}

	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(pPlayer->team_id);
	if (!pTeam) 
	{
		Log::log(LOG_ERR, "SendOnlineNotify, arena team %d not found!", pPlayer->team_id);
		return;
	}

	size_t pool_count = 0;
	PlayerInfo* send_pool[MAX_MEMBERS + 1];
	memset(send_pool, 0x00, sizeof(send_pool));
	EC_ArenaTeamMemberOnlineInfoVector members;
	
	for (size_t i = 0; i < pTeam->members.size() && i < MAX_MEMBERS; i++)
	{
		long long player_id = pTeam->members[i].player_id; 
		
		RoleIdx(player_id);
		EC_ArenaTeamMemberOnlineInfo member( player_id, 0 );
		
		Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
		PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(pTeam->members[i].player_id);
		if (pInfo && pInfo->gameid > 0)
		{
			member.online = 1;
			send_pool[pool_count++] = pInfo;
		}
		
		members.push_back(member);
	}

	if (!self)
	{
		for (size_t i = 0; i < pool_count && send_pool[i]; i++)
		{
			if ( send_pool[i]->roleid )
			{
				EC_ArenaOnlineInfoNotify aoin(ERR_SUCCESS, send_pool[i]->roleid, members);
				GDeliveryServer::GetInstance()->Send(send_pool[i]->linksid, aoin);	
			}
		}
	}
	else
	{
		for (size_t i = 0; i < pool_count && send_pool[i]; i++)
		{
			if ( send_pool[i]->roleid == roleid )
			{
				EC_ArenaOnlineInfoNotify aoin(ERR_SUCCESS, send_pool[i]->roleid, members);
				GDeliveryServer::GetInstance()->Send(send_pool[i]->linksid, aoin);	
			}
		}
	}
}

// arena top list

void ArenaOfAuroraManager::EC_ArenaPlayerTopList_Re(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pInfo && pInfo->gameid > 0)
	{
		EC_ArenaPlayerTopListQuery_Re aptlq_re(ERR_SUCCESS, roleid, MODE_3X3, _players_top_list);
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, aptlq_re);
	}
}

void ArenaOfAuroraManager::EC_ArenaTeamTopList_Re(int roleid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pInfo && pInfo->gameid > 0)
	{
		EC_ArenaTeamTopListQuery_Re attlq_re(ERR_SUCCESS, roleid, MODE_3X3, _teams_top_list);
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, attlq_re);
	}
}

void ArenaOfAuroraManager::EC_ArenaTeamTopListDetail(int roleid, long long teamid)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pInfo && pInfo->gameid > 0)
	{
		EC_DBArenaTeamTopListDetailArg arg;
		arg.roleid = roleid;
		arg.teamid = teamid;
		GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_DBARENATEAMTOPLISTDETAIL, arg));
	}
}

void ArenaOfAuroraManager::EC_ArenaTeamTopListDetail_Re(int roleid, long long teamid, EC_ArenaPlayerTopListVector & vec)
{
	Thread::RWLock::RDScoped l(UserContainer::GetInstance().GetLocker());
	PlayerInfo* pInfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	if (pInfo && pInfo->gameid > 0)
	{
		EC_ArenaTeamTopListDetailQuery_Re attldq_re(ERR_SUCCESS, roleid, teamid, vec);
		GDeliveryServer::GetInstance()->Send(pInfo->linksid, attldq_re);
	}
}

void ArenaOfAuroraManager::EC_UpdateArenaPlayerTopList(time_t now_time)
{
	if (!(now_time % 60))
	{
		EC_DBArenaPlayerTopListArg arg;
		arg.count = 200;
		GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_DBARENAPLAYERTOPLIST, arg));
	}
}

void ArenaOfAuroraManager::EC_UpdateArenaTeamTopList(time_t now_time)
{
	if (!(now_time % 60))
	{
		EC_DBArenaTeamTopListArg arg;
		arg.count = 50;
		GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_DBARENATEAMTOPLIST, arg));
	}
}

void ArenaOfAuroraManager::EC_SetArenaPlayerTopList(EC_ArenaPlayerTopListVector & vec)
{
	_players_top_list.clear();
	_players_top_list = vec;
	Log::log(LOG_ERR, "EC_SetArenaPlayerTopList: size=%d ", _players_top_list.size() );
}

void ArenaOfAuroraManager::EC_SetArenaTeamTopList(EC_ArenaTeamTopListVector & vec)
{
	_teams_top_list.clear();
	_teams_top_list = vec;
	Log::log(LOG_ERR, "EC_SetArenaTeamTopList: size=%d ", _teams_top_list.size() );
}

//-----------------------------------------------------------------------------------------------------------//
// -- default templates db 
//-----------------------------------------------------------------------------------------------------------//

bool ArenaOfAuroraManager::EC_MakeDefaultPlayer(EC_SQLArenaPlayer & pPlayer, int team_id, int roleid, int cls, Octets & name)
{
	pPlayer.player_id = roleid;
	pPlayer.team_id = team_id;
	pPlayer.cls = cls;
	pPlayer.score = 0;
	pPlayer.win_count = 0;
	pPlayer.team_score = BASE_SCORE;
	pPlayer.week_battle_count = 0;
	pPlayer.invite_time = GetTime();
	pPlayer.last_visite = 0;
	pPlayer.battle_count = 0;
	pPlayer.name = name;
	return true;
}

bool ArenaOfAuroraManager::EC_MakeDefaultTeam(EC_SQLArenaTeam & pTeam, int capitan_id, int team_type, Octets & team_name)
{
	pTeam.team_id = 0;
	pTeam.captain_id = capitan_id;
	pTeam.team_type = team_type;
	pTeam.score = 0;
	pTeam.last_visite = 0;
	pTeam.win_count = 0;
	pTeam.team_score = BASE_SCORE;
	pTeam.week_battle_count = 0;
	pTeam.create_time = GetTime();
	pTeam.battle_count = 0;
	pTeam.name = team_name;
	pTeam.members.push_back(EC_SQLArenaTeamMember(capitan_id, 0));
	pTeam.teams.clear();
	return true;
}

//-----------------------------------------------------------------------------------------------------------//
// -- player db 
//-----------------------------------------------------------------------------------------------------------//

void ArenaOfAuroraManager::EC_CreateArenaPlayer(int roleid, int cls, Octets & name)
{
	EC_SQLCreateArenaPlayerArg arg;
	arg.roleid = roleid;
	arg.cls = cls;
	arg.name = name;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLCREATEARENAPLAYER, arg));
}

void ArenaOfAuroraManager::EC_CreateArenaPlayer_Re(int roleid, int retcode, EC_SQLArenaPlayer & pPlayer)
{
	if ( retcode == ERR_SUCCESS && roleid == pPlayer.player_id )
	{
		Log::log(LOG_ERR, "EC_CreateArenaPlayer_Re: player=%d ", roleid);
		OnLoadPlayer(pPlayer);
	}
	else
	{
		Log::log(LOG_ERR, "EC_CreateArenaPlayer_Re: ERROR player=%d , retcode=%d ", roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_DeleteArenaPlayer(int roleid)
{
	EC_SQLDeleteArenaPlayerArg arg;
	arg.roleid = roleid;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLDELETEARENAPLAYER, arg));
}


void ArenaOfAuroraManager::EC_DeleteArenaPlayer_Re(int roleid, int retcode)
{
	if (retcode == ERR_SUCCESS)
	{
		_player_map.erase(roleid);
		Log::log(LOG_ERR, "EC_DeleteArenaPlayer_Re: player %d ", roleid);
	}
	else
	{
		Log::log(LOG_ERR, "EC_DeleteArenaPlayer_Re: ERROR player %d , retcode = %d ", roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_GetArenaPlayer(int roleid)
{
	EC_SQLGetArenaPlayerArg arg;
	arg.roleid = roleid;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLGETARENAPLAYER, arg));
}

void ArenaOfAuroraManager::EC_GetArenaPlayer_Re(int roleid, int retcode, EC_SQLArenaPlayer & pPlayer)
{
	if ( retcode == ERR_SUCCESS && roleid == pPlayer.player_id )
	{
		Log::log(LOG_ERR, "EC_GetArenaPlayer_Re: Get player %d ", roleid);
		OnLoadPlayer(pPlayer);
	}
	else
	{
		Log::log(LOG_ERR, "EC_GetArenaPlayer_Re: ERROR get player %d , retcode = %d ", roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_SetArenaPlayer(int roleid, EC_SQLArenaPlayer & pPlayer)
{
	EC_SQLSetArenaPlayerArg arg;
	arg.roleid = roleid;
	arg.player = pPlayer;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLSETARENAPLAYER, arg));
}

void ArenaOfAuroraManager::EC_SetArenaPlayer_Re(int roleid, int retcode)
{
	if (retcode == ERR_SUCCESS)
	{
		Log::log(LOG_ERR, "EC_SetArenaPlayer_Re: set player %d ", roleid);
	}
	else
	{
		Log::log(LOG_ERR, "EC_SetArenaPlayer_Re: ERROR set player %d , retcode = %d ", roleid, retcode);
	}
}

//-----------------------------------------------------------------------------------------------------------//
// -- team db 
//-----------------------------------------------------------------------------------------------------------//

void ArenaOfAuroraManager::EC_CreateArenaTeam(int capitan_id, int capitan_cls, Octets & capitan_name, int team_type, Octets & team_name)
{
	EC_SQLArenaPlayer* player = GetArenaPlayerByRoleID(capitan_id);

	if ( player && player->team_id > 0)
	{
		SendError(capitan_id, MSG_ERROR_03);
		Log::log(LOG_ERR, "EC_CreateArenaTeam CreateTeam player %d in team", capitan_id);
		return;
	}

	EC_SQLCreateArenaTeamArg arg;
	arg.capitan_id = capitan_id;
	arg.capitan_cls = capitan_cls;
	arg.capitan_name = capitan_name;
	arg.team_type = team_type;
	arg.team_name = team_name;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLCREATEARENATEAM, arg));

	Log::log(LOG_ERR, "EC_CreateArenaTeam roleid=%d, teamname:%d", capitan_id, team_name.size());
}

void ArenaOfAuroraManager::EC_CreateArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaTeam & team, EC_SQLArenaPlayer & player)
{
	if ( retcode == ERR_SUCCESS )
	{
		// create arena team
		{
			team.team_id = team_id;
			team.captain_id = roleid;
			team.members.clear();
			team.members.push_back(EC_SQLArenaTeamMember(roleid, 0));
			OnLoadTeam(team);
			SyncTeamToDB(team_id);
		}

		// create arena player
		{
			player.team_id = team_id;
			OnLoadPlayer(player);
			SyncPlayerToDB(roleid);
		}

		SendTeamCreateRe(retcode, roleid, team_id);
		SendOnlineNotify(roleid);
		Log::log(LOG_ERR, "EC_CreateArenaTeam_Re: team=%d , player=%d ", team_id, roleid);
	}
	else
	{
		Log::log(LOG_ERR, "EC_CreateArenaTeam_Re: ERROR team=%d , player=%d , retcode = %d ", team_id, roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_DeleteArenaTeam(int capitan_id, int team_id)
{
	EC_SQLDeleteArenaTeamArg arg;
	arg.capitan_id = capitan_id;
	arg.team_id = team_id;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLDELETEARENATEAM, arg));
}

void ArenaOfAuroraManager::EC_DeleteArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaPlayer & pPlayer)
{
	if (retcode == ERR_SUCCESS)
	{
		_team_map.erase(team_id);
		_player_map[roleid] = pPlayer;
		Log::log(LOG_ERR, "EC_DeleteArenaTeam_Re: team=%d , player %d ", team_id, roleid);
	}
	else
	{
		Log::log(LOG_ERR, "EC_DeleteArenaTeam_Re: ERROR player %d , retcode = %d ", roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_GetArenaTeam(int capitan_id, int team_id)
{
	EC_SQLGetArenaTeamArg arg;
	arg.capitan_id = capitan_id;
	arg.team_id = team_id;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLGETARENATEAM, arg));
}

void ArenaOfAuroraManager::EC_GetArenaTeam_Re(int roleid, int team_id, int retcode, EC_SQLArenaTeam & team)
{
	if ( retcode == ERR_SUCCESS && team_id == team.team_id )
	{
		_team_map[team_id] = team;
		Log::log(LOG_ERR, "EC_GetArenaTeam_Re: team=%d , player=%d , members_size=%d ", team_id, roleid, team.members.size());
		OnLoadTeam(team);
	}
	else
	{
		Log::log(LOG_ERR, "EC_GetArenaTeam_Re: ERROR team=%d , player=%d , retcode = %d ", team_id, roleid, retcode);
	}
}

void ArenaOfAuroraManager::EC_SetArenaTeam(int capitan_id, int team_id, EC_SQLArenaTeam & team)
{
	EC_SQLSetArenaTeamArg arg;
	arg.capitan_id = capitan_id;
	arg.team_id = team_id;
	arg.team = team;
	GAuthClient::GetInstance()->SendProtocol(Rpc::Call(RPC_EC_SQLSETARENATEAM, arg));
}


void ArenaOfAuroraManager::EC_SetArenaTeam_Re(int roleid, int team_id, int retcode)
{
	if (retcode == ERR_SUCCESS)
	{
		Log::log(LOG_ERR, "EC_SetArenaTeam_Re: team=%d , player=%d ", team_id, roleid);
	}
	else
	{
		Log::log(LOG_ERR, "EC_SetArenaTeam_Re: ERROR set player %d , retcode = %d ", roleid, retcode);
	}
}

//-----------------------------------------------------------------------------------------------------------//
// -- func db 
//-----------------------------------------------------------------------------------------------------------//

void ArenaOfAuroraManager::SyncPlayerToDB(int roleid)
{
	EC_SQLArenaPlayer* player = GetArenaPlayerByRoleID(roleid);
	if (!player) {
		Log::log(LOG_ERR, "ArenaOfAuroraManager::SyncPlayerToDB, arena player %d not found", roleid);
		return;
	}
	
	EC_SetArenaPlayer(roleid, *player);
}

void ArenaOfAuroraManager::SyncTeamToDB(int teamid)
{
	EC_SQLArenaTeam* team = GetArenaTeamByTeamID(teamid);
	if (!team) {
		Log::log(LOG_ERR, "ArenaOfAuroraManager::SyncTeamToDB, arena team %d not found", teamid);
		return;
	}

	EC_SetArenaTeam(team->captain_id, teamid, *team);
}

void ArenaOfAuroraManager::LoadTeam(int teamid)
{
	if (teamid == 0)
		return;

	if (GetArenaTeamByTeamID(teamid))
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager LoadTeam exists %d", teamid);
		return;
	}

	EC_GetArenaTeam(0, teamid);
}

void ArenaOfAuroraManager::OnLoadTeam(EC_SQLArenaTeam & data)
{
	if (data.team_id == 0)
		return;

	if (GetArenaTeamByTeamID(data.team_id))
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager OnLoadTeam duplicate %d", data.team_id);
		return;
	}

	// members
	{
		std::vector<EC_SQLArenaTeamMember>::iterator it = data.members.begin();
		for (; it != data.members.end(); it++)
		{
			if (it->player_id == 0)
				continue;

			LoadPlayer(it->player_id);
		}
	}

	_team_map[data.team_id] = data;
	TeamFullLoad(data.team_id);
}

void ArenaOfAuroraManager::LoadPlayer(int roleid)
{
	if (roleid == 0)
		return;

	if (GetArenaPlayerByRoleID(roleid))
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager LoadPlayer exists %d", roleid);
		return;
	}

	EC_GetArenaPlayer(roleid);
}

void ArenaOfAuroraManager::OnLoadPlayer(EC_SQLArenaPlayer & data)
{
	if (data.player_id == 0)
		return;

	if (GetArenaPlayerByRoleID(data.player_id))
	{
		Log::log(LOG_ERR, "ArenaOfAuroraManager OnLoadPlayer duplicate %d", data.player_id);
		return;
	}

	_player_map[data.player_id] = data;

	LoadTeam(data.team_id);
}

void ArenaOfAuroraManager::TeamFullLoad(int team_id)
{
	EC_SQLArenaTeam* pTeam = GetArenaTeamByTeamID(team_id);
	if (!pTeam) {
		Log::log(LOG_ERR, "TeamFullLoad, arena team %d not found!", team_id);
		return;
	}

	LOG_TRACE("ArenaOfAuroraManager TeamFullLoad id=%d, name=%.*s", team_id, pTeam->name.size(), (char*)pTeam->name.begin());

	// Load members
	for (int i = 0; i < (int)pTeam->members.size(); i++)
	{
		int memberId = pTeam->members[i].player_id;
		if (memberId <= 0)
			continue;

		if (!GetArenaPlayerByRoleID(memberId))
		{
			EC_GetArenaPlayer(memberId);
		}
	}
}


};

