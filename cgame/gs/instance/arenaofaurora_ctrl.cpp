
#include "../world.h"
#include "../worldmanager.h"
#include "../player_imp.h"
#include "arenaofaurora_ctrl.h"

#define MAX_TEAM_NAME_LENGTH 36

#define MAX_SCORE 30
#define MAX_WAIT_QUEUE 600
#define BASE_SCORE 0
#define QUALIFICATION_COUNT_REQUIRE 10

void arenaofaurora_ctrl::Tick(world* pPlane)
{
	spin_autolock l(_lock);
	if ((++_tick_counter) % 20 != 0)
		return;

	_battle_timer += 1;

	SyncArenaStat();

	// Inicia a Batalha
	if (_battle_timer == 0)
	{
		HideAreaGFX(pPlane);
	}

	// Finaliza a Batalha
	if (_battle_timer >= 0)
	{
		CheckBattleResult(pPlane);
	}
}

void arenaofaurora_ctrl::ArenaPlayerFaction()
{
	if(_red_list.size())
	{
		packet_wrapper h3(64);
		using namespace S2C;

		CMD::Make<CMD::arena_player_faction_info>::From(h3, 
		RED_TYPE_IDX,
		IDX_ACTIVATE,
		_battle_timer
		);
		multi_send_ls_msg(_red_list, h3.data(), h3.size(), 0);
	}
		
	if (_blue_list.size())
	{
		packet_wrapper h3(64);
		using namespace S2C;

		CMD::Make<CMD::arena_player_faction_info>::From(h3, 
		BLUE_TYPE_IDX,
		IDX_ACTIVATE,
		_battle_timer
		);
		multi_send_ls_msg(_blue_list, h3.data(), h3.size(), 0);
	}
}

void arenaofaurora_ctrl::ArenaBattleGroup()
{
	if (_all_list.size())
	{		
		packet_wrapper h2(64);
		using namespace S2C;

		int red_members[] = { 0, 0, 0, 0, 0, 0, 0 };
		int blue_members[] = { 0, 0, 0, 0, 0, 0, 0 };
		for (int i = 0; i < _data.red_members.size(); ++i)
		{
			red_members[i] = _data.red_members[i].player_id;			
		}
		for (int i = 0; i < _data.blue_members.size(); ++i)
		{
			blue_members[i] = _data.blue_members[i].player_id;
		}

		CMD::Make<CMD::arena_battle_group_info>::From(h2, 
		_battle_timer,
		GetAliveMembers(_red_alive_list), 
		GetAliveMembers(_blue_alive_list),
		_data.red_members.size(),
		_data.blue_members.size(),
		red_members,
		blue_members
		);

		multi_send_ls_msg(_all_list, h2.data(), h2.size(), 0);		
	}
}

void arenaofaurora_ctrl::SyncArenaStat()
{
	spin_autolock keeper(_user_list_lock);	
		
	ArenaBattleGroup();
}

void arenaofaurora_ctrl::SendPlayerLeave(int playerid)
{
	//
}

void arenaofaurora_ctrl::HideAreaGFX(world* pPlane)
{
	pPlane->ClearSpawn(1001);
}

void arenaofaurora_ctrl::CheckBattleResult(world * pPlane)
{
	if (_battle_result)
		return;

	if (GetAliveMembers(_red_alive_list) == 0 || GetAliveMembers(_blue_alive_list) == 0)
	{
		_battle_result = 1;
		BattleEnd(pPlane);
		return;
	}

	if (_battle_timer >= TIMER_MAX_BATTLE)
	{
		_battle_result = 1;
		BattleEnd(pPlane);
		return;
	}
}

void arenaofaurora_ctrl::BattleEnd(world * pPlane)
{
	ArenaRelaese();
	GMSV::EC_SendArenaBattleEnd(_data.arena_id, world_manager::GetWorldTag(), GetWinner(),
		GetAliveMembers(_red_alive_list), GetAliveMembers(_blue_alive_list),
		_red_damage, _blue_damage, _data);
	
	// Mapa
	pPlane->w_destroy_timestamp = g_timer.get_systime() + 105;
	pPlane->w_battle_result = _battle_result;
	_battle_end_timer = 5;

}

int arenaofaurora_ctrl::GetWinner()
{
	int count_alive_red = GetAliveMembers(_red_alive_list);
	int count_alive_blue = GetAliveMembers(_blue_alive_list);

	if (count_alive_red == count_alive_blue)
	{
		if (_red_damage == _blue_damage)
		{
			return ARENA_RESULT_NOBODY;
		}
		
		if (_red_damage > _blue_damage)
		{
			return ARENA_RESULT_WINNER_RED;
		}

		if (_blue_damage > _red_damage)
		{
			return ARENA_RESULT_WINNER_BLUE;
		}

		return ARENA_RESULT_ERR;
	}

	if (count_alive_blue == 0 && count_alive_red >= 1)
	{
		return ARENA_RESULT_WINNER_RED;
	}

	if (count_alive_red == 0 && count_alive_blue >= 1)
	{
		return ARENA_RESULT_WINNER_BLUE;
	}

	return ARENA_RESULT_ERR;
}

void arenaofaurora_ctrl::RefreshStateRedTeam()
{
	for (unsigned int i = 0; i < _data.red_members.size(); i++)
	{
		int index;
		gplayer* pPlayer = world_manager::GetInstance()->FindPlayer(_data.red_members[i].player_id, index);
		if (pPlayer) {
			gactive_imp* pImp = (gactive_imp*)pPlayer->imp;
			if (pImp) {
				pImp->SetRefreshState();
			}
		}
	}
}

void arenaofaurora_ctrl::RefreshStateBlueTeam()
{
	for (unsigned int i = 0; i < _data.blue_members.size(); i++)
	{
		int index;
		gplayer* pPlayer = world_manager::GetInstance()->FindPlayer(_data.blue_members[i].player_id, index);
		if (pPlayer) {
			gactive_imp* pImp = (gactive_imp*)pPlayer->imp;
			if (pImp) {
				pImp->SetRefreshState();
			}
		}
	}
}

void arenaofaurora_ctrl::NotifyMembers()
{
	ArenaPlayerFaction();
}

void arenaofaurora_ctrl::OnPlayerEnter(gplayer* pPlayer)
{
	spin_autolock keeper(_user_list_lock);
	AddMapNode(_all_list, pPlayer);

	int playerId = pPlayer->ID.id;

	switch (GetTeamType(playerId))
	{
	case ARENA_TEAM_RED:
		AddVectorInt(_red_enter_list, pPlayer);
		AddArenaMember(_red_alive_list, pPlayer);
		AddMapNode(_red_list, pPlayer);
		RefreshStateBlueTeam();

		break;
	case ARENA_TEAM_BLUE:
		AddVectorInt(_blue_enter_list, pPlayer);
		AddArenaMember(_blue_alive_list, pPlayer);
		AddMapNode(_blue_list, pPlayer);
		RefreshStateRedTeam();

		break;
	default:
		break;
	}

	NotifyMembers();
}

void arenaofaurora_ctrl::OnPlayerLeave(gplayer* pPlayer)
{
	spin_autolock keeper(_user_list_lock);
	DelMapNode(_all_list, pPlayer);

	int playerId = pPlayer->ID.id;

	switch (GetTeamType(playerId))
	{
	case ARENA_TEAM_RED:
		DelArenaMember(_red_alive_list, pPlayer);
		DelMapNode(_red_list, pPlayer);
		printf(" [ arenaofaurora_ctrl::OnPlayerLeave ] %d is red team\n", playerId);
		break;
	case ARENA_TEAM_BLUE:
		DelArenaMember(_blue_alive_list, pPlayer);
		DelMapNode(_blue_list, pPlayer);
		printf(" [ arenaofaurora_ctrl::OnPlayerLeave ] %d is blue team\n", playerId);
		break;
	default:
		printf(" [ arenaofaurora_ctrl::OnPlayerLeave ] %d is undefined player\n", playerId);
		break;
	}

	AddVectorInt(_death_players, pPlayer);

	SendPlayerLeave(playerId);
}

void arenaofaurora_ctrl::OnDeath(gplayer* pPlayer)
{
	spin_autolock keeper(_user_list_lock);

	int playerId = pPlayer->ID.id;
	
	switch (GetTeamType(playerId))
	{
	case ARENA_TEAM_RED:
		DeathArenaMember(_red_alive_list, pPlayer);
		printf(" [ arenaofaurora_ctrl::OnDeath ] %d is red team\n", playerId);
		break;
	case ARENA_TEAM_BLUE:
		DeathArenaMember(_blue_alive_list, pPlayer);
		printf(" [ arenaofaurora_ctrl::OnDeath ] %d is blue team\n", playerId);
		break;
	default:
		printf(" [ arenaofaurora_ctrl::OnDeath ] %d is undefined player\n", playerId);
		break;
	}

	AddVectorInt(_death_players, pPlayer);

	SendPlayerLeave(playerId);
}

void arenaofaurora_ctrl::OnDamage(int attacker, int damage)
{
	int teamType = GetTeamType(attacker);

	switch (teamType)
	{
	case ARENA_TEAM_RED:
		_red_damage += damage;
		break;
	case ARENA_TEAM_BLUE:
		_blue_damage += damage;
		break;
	}
}

int arenaofaurora_ctrl::GetTeamType(int player_id)
{
	for (unsigned int i = 0; i < _data.red_members.size(); i++)
	{
		if ( _data.red_members[i].player_id == player_id )
		{
			return ARENA_TEAM_RED;
		}
	}

	for (unsigned int i = 0; i < _data.blue_members.size(); i++)
	{
		if ( _data.blue_members[i].player_id == player_id )
		{
			return ARENA_TEAM_BLUE;
		}
	}

	return ARENA_ERR;
}

bool arenaofaurora_ctrl::IsAlivePlayer(int player_id)
{
	return std::find(_death_players.begin(), _death_players.end(), player_id) == _death_players.end();
}

bool arenaofaurora_ctrl::IsDeathPlayer(int player_id)
{
	return std::find(_death_players.begin(), _death_players.end(), player_id) != _death_players.end();
}


// result 

int arenaofaurora_ctrl::CalculateQualScore(int win_count)
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

double arenaofaurora_ctrl::CalculateScore(int red_score, int blue_score)
{
	return 1 / (1 + pow(10, (blue_score - red_score) / 400.0));
}

int arenaofaurora_ctrl::GetScoreDelta(int red_score, int blue_score, int result)
{
	return (int)((MAX_SCORE + 1) * ((int)result - CalculateScore(red_score, blue_score)));
}

void arenaofaurora_ctrl::CalculateWinnerResult(GMSV::EC_GSArenaPlayer & pPlayer)
{
	pPlayer.battle_count += 1;
	pPlayer.week_battle_count += 1;
	pPlayer.win_count += 1;

	// set if is completed qualification
	if (pPlayer.battle_count == QUALIFICATION_COUNT_REQUIRE)
		pPlayer.score = CalculateQualScore(pPlayer.win_count);

}

void arenaofaurora_ctrl::CalculateLoserResult(GMSV::EC_GSArenaPlayer & pPlayer)
{
	pPlayer.battle_count += 1;
	pPlayer.week_battle_count += 1;

	if (pPlayer.score < 0) 
	{
		pPlayer.score = 0;
	}

	// set if is completed qualification
	if (pPlayer.battle_count == QUALIFICATION_COUNT_REQUIRE)
		pPlayer.score = CalculateQualScore(pPlayer.win_count);
}



void arenaofaurora_ctrl::CalculateBattleResult( GMSV::EC_GSArenaTeam & pWinnerTeam, std::vector<GMSV::EC_GSArenaPlayer> & pWinnerRoles, GMSV::EC_GSArenaTeam & pLoserTeam, std::vector<GMSV::EC_GSArenaPlayer> & pLoserRoles )
{
	// increment win
	pWinnerTeam.win_count += 1;

	// increment battle count
	pWinnerTeam.battle_count += 1;
	pLoserTeam.battle_count += 1;

	// increment week battle count
	pWinnerTeam.week_battle_count += 1;
	pLoserTeam.week_battle_count += 1;

	// set if winner is completed qualification
	if (pWinnerTeam.battle_count == QUALIFICATION_COUNT_REQUIRE)
		pWinnerTeam.score = CalculateQualScore(pWinnerTeam.win_count);

	// set if loser is completed qualification
	if (pLoserTeam.battle_count == QUALIFICATION_COUNT_REQUIRE)
		pLoserTeam.score = CalculateQualScore(pLoserTeam.win_count);

	// Winner members & sync
	for (unsigned int i = 0; i < pWinnerRoles.size(); i++)
	{
		CalculateWinnerResult( pWinnerRoles[i]);
	}

	// Loser members & sync
	for (unsigned int i = 0; i < pLoserRoles.size(); i++)
	{
		CalculateLoserResult( pLoserRoles[i]);
	}


}

int arenaofaurora_ctrl::GetArenaResultScore(int result)
{
	int delta = 0;
	int delta_r = 0;
	int delta_b = 0;

	switch (result)
	{
	case ARENA_RESULT_WINNER_RED: // red winner
		delta = GetScoreDelta( _data.red_team.score , _data.blue_team.score , 1);
		delta_r = delta;
		delta_b = -delta;
		break;
	case ARENA_RESULT_WINNER_BLUE: // blue winner
		delta = GetScoreDelta( _data.red_team.score , _data.blue_team.score , 0);
		delta_r = -delta;
		delta_b = delta;
		break;
	}

	_data.red_team.score += delta;
	_data.blue_team.score -= delta;

	if (_data.red_team.score < 0) _data.red_team.score = 0;
	if (_data.blue_team.score < 0) _data.blue_team.score = 0;

	switch (result)
	{
	case ARENA_RESULT_WINNER_RED: // red winner
		{
			for (size_t i = 0; i < _data.red_members.size(); i++)
			{
				_data.red_members[i].score -= GetScoreDelta(_data.red_members[i].score, _data.blue_team.score, 1);
			}

			for (size_t i = 0; i < _data.blue_members.size(); i++)
			{
				_data.blue_members[i].score += GetScoreDelta(_data.blue_members[i].score, _data.red_team.score, 0);
			}
		}
		break;
	case ARENA_RESULT_WINNER_BLUE: // blue winner
		{
			for (size_t i = 0; i < _data.red_members.size(); i++)
			{
				_data.red_members[i].score += GetScoreDelta(_data.red_members[i].score, _data.blue_team.score, 0);
			}

			for (size_t i = 0; i < _data.blue_members.size(); i++)
			{
				_data.blue_members[i].score -= GetScoreDelta(_data.blue_members[i].score, _data.red_team.score, 1);
			}
		}
		break;
	}

	switch (result)
	{
		case ARENA_RESULT_WINNER_RED: // red winner
			CalculateBattleResult(_data.red_team, _data.red_members, _data.blue_team, _data.blue_members);
			break;
		case ARENA_RESULT_WINNER_BLUE: // blue winner
			CalculateBattleResult(_data.blue_team, _data.blue_members, _data.red_team, _data.red_members);
			break;
		default:
			break;
	}

	return delta;
}

// owner 
void
arenaofaurora_ctrl::ArenaRelaese( )
{
	if (_all_list.size())
	{		
		int result = GetWinner();
		int red_old_score = _data.red_team.score;
		int blue_old_score = _data.blue_team.score;
		int delta = GetArenaResultScore(result);
		int delta_r = ( _data.red_team.score - red_old_score );
		int delta_b = ( _data.blue_team.score - blue_old_score );
		
		//int reserve3 = 11; this delta
		int reward_point_count = result ? 1 : 2;
		
		int red_player_count = _data.red_members.size();
		int blue_player_count = _data.blue_members.size();
		int red_alive_player_count = GetAliveMembers(_red_alive_list);
		int blue_alive_player_count = GetAliveMembers(_blue_alive_list);

		char red_team_name[MAX_TEAM_NAME_LENGTH]; memcpy(red_team_name, _data.red_team.name, MAX_TEAM_NAME_LENGTH);
		char blue_team_name[MAX_TEAM_NAME_LENGTH]; memcpy(blue_team_name, _data.blue_team.name, MAX_TEAM_NAME_LENGTH);
		
		packet_wrapper h1(2048);
		using namespace S2C;
		CMD::Make<CMD::arena_battle_result>::From(h1);
		
		h1 << result;
		h1 << _red_damage << _blue_damage;
		h1 << red_old_score << blue_old_score;
		h1 << delta_r << delta_b;
		h1 << delta;
		h1 << reward_point_count;

		h1 << red_player_count << blue_player_count;
		h1 << red_alive_player_count << blue_alive_player_count;

		h1.push_back(red_team_name, sizeof(red_team_name));
		h1.push_back(blue_team_name, sizeof(blue_team_name));

		for (int i = 0; i < _data.red_members.size(); ++i)
		{
			if ( _data.red_members[i].player_id >= 1024 )
			{
				int index;
				gplayer* pPlayer = world_manager::GetInstance()->FindPlayer(_data.red_members[i].player_id, index);
				if (pPlayer && pPlayer->imp)
				{
					gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
					arena_team_player_result player_result;
					unsigned int  len = 0;
					const void * player_name = pImp->GetPlayerName(len);
					
					player_result.cls = pImp->GetPlayerClass();
					player_result.point_solo = _data.red_members[i].score;
					player_result.wins = _data.red_members[i].win_count;
					player_result.point_team = _data.red_members[i].team_score;
					player_result.reserve = 0;
					if (player_name && len)
					{
						memcpy(player_result.player_name, player_name, len);
					}
					h1.push_back(&player_result, sizeof(player_result));
				}
			}
		}

		for (int i = 0; i < _data.blue_members.size(); ++i)
		{
			if ( _data.blue_members[i].player_id >= 1024 )
			{
				int index;
				gplayer* pPlayer = world_manager::GetInstance()->FindPlayer(_data.blue_members[i].player_id, index);
				if (pPlayer && pPlayer->imp)
				{
					gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
					arena_team_player_result player_result;
					unsigned int  len = 0;
					const void * player_name = pImp->GetPlayerName(len);
					
					player_result.cls = pImp->GetPlayerClass();
					player_result.point_solo = _data.blue_members[i].score;
					player_result.wins = _data.blue_members[i].win_count;
					player_result.point_team = _data.blue_members[i].team_score;
					player_result.reserve = 0;
					if (player_name && len)
					{
						memcpy(player_result.player_name, player_name, len);
					}
					h1.push_back(&player_result, sizeof(player_result));
				}
			}
		}

		multi_send_ls_msg(_all_list, h1.data(), h1.size(), 0);
	}
}



