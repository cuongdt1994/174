#ifndef __ONLINEGAME_GS_ARENAOFAURORA_CTRL_H__
#define __ONLINEGAME_GS_ARENAOFAURORA_CTRL_H__

#include "gsp_if.h"

enum
{
	ARENA_ERR = -1,
	ARENA_TEAM_RED = 0,
	ARENA_TEAM_BLUE = 1,
};

enum
{
	ARENA_RESULT_ERR = -1,
	ARENA_RESULT_NOBODY = 0,
	ARENA_RESULT_WINNER_RED = 1,
	ARENA_RESULT_WINNER_BLUE = 2,
	RED_TYPE_IDX = 2048,
	BLUE_TYPE_IDX = 4096,
	IDX_ACTIVATE = 1,
	TIMER_MAX_BATTLE = 600,
};

class arenaofaurora_ctrl : public world_data_ctrl
{
public:
	GMSV::EC_GSArena _data;

	struct arena_team_player_result
	{
		int cls; // OK
		int point_solo; // OK
		int wins; // OK
		int point_team;
		int reserve;
		char player_name[MAX_USERNAME_LENGTH];
		
		arena_team_player_result()
		{
			memset(this, 0x00, sizeof(*this));
		}
		
		~arena_team_player_result() {}
	};


	struct arena_member_entry
	{
		XID id;
		int cs_index;
		int cs_sid;
		bool is_death;
	};

	int _lock;
	int _tick_counter;
	int _battle_timer;
	int _battle_result;
	int _battle_end_timer;
	int _user_list_lock;

	A3DVECTOR _trans_pos_1;
	A3DVECTOR _trans_pos_2;
	int _target_world_tag;

	int _red_damage;
	int _blue_damage;

	cs_user_map  _all_list;

	cs_user_map  _red_list;
	cs_user_map  _blue_list;

	abase::vector<int> _red_enter_list;
	abase::vector<int> _blue_enter_list;

	abase::vector<arena_member_entry> _red_alive_list;
	abase::vector<arena_member_entry> _blue_alive_list;

	abase::vector<int> _death_players;
public:
	arenaofaurora_ctrl() : _lock(0), _tick_counter(0), 
		_battle_timer(-150), _battle_result(0), _battle_end_timer(0),
		_user_list_lock(0), _trans_pos_1(0,0,0), _trans_pos_2(0,0,0), _red_damage(0), _blue_damage(0), _target_world_tag(0)
	{
		_data.red_team.members.clear();
		_data.red_team.teams.clear();
		_data.red_members.clear();
		
		_data.blue_team.members.clear();
		_data.blue_team.teams.clear();
		_data.blue_members.clear();
		
		memset(&_data, 0, sizeof(_data));
	}
	virtual ~arenaofaurora_ctrl(){}
	virtual world_data_ctrl * Clone()
	{
		return new arenaofaurora_ctrl(*this);
	}
	virtual void Reset()
	{
		_data.red_team.members.clear();
		_data.red_team.teams.clear();
		_data.red_members.clear();
		
		_data.blue_team.members.clear();
		_data.blue_team.teams.clear();
		_data.blue_members.clear();
		
		memset(&_data, 0, sizeof(_data));
	}
	virtual void Tick(world * pPlane);

private:
	void ArenaPlayerFaction();
	void ArenaBattleGroup();

	void SyncArenaStat();
	void SendPlayerLeave(int playerid);
	void HideAreaGFX(world* pPlane);

	void CheckBattleResult(world * pPlane);
	void BattleEnd(world * pPlane);
	void ArenaRelaese();

	double CalculateScore(int red_score, int blue_score);
	int CalculateQualScore(int win_count);
	int GetScoreDelta(int red_score, int blue_score, int result);
	int GetArenaResultScore(int result);
	void CalculateWinnerResult(GMSV::EC_GSArenaPlayer & pPlayer);
	void CalculateLoserResult(GMSV::EC_GSArenaPlayer & pPlayer);
	void CalculateBattleResult( GMSV::EC_GSArenaTeam & pWinnerTeam, std::vector<GMSV::EC_GSArenaPlayer> & pWinnerRoles, GMSV::EC_GSArenaTeam & pLoserTeam, std::vector<GMSV::EC_GSArenaPlayer> & pLoserRoles );

	int GetWinner();

	void RefreshStateRedTeam();
	void RefreshStateBlueTeam();

	void NotifyMembers();
	
	inline void AddMapNode(cs_user_map & map, gplayer * pPlayer)
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val(pPlayer->ID.id,pPlayer->cs_sid);
		if(cs_index >= 0 && val.first >= 0)
		{
			map[cs_index].push_back(val);
		}
	}

	inline void DelMapNode(cs_user_map & map, gplayer * pPlayer)
	{
		int cs_index = pPlayer->cs_index;
		std::pair<int,int> val(pPlayer->ID.id,pPlayer->cs_sid);
		if(cs_index >= 0 && val.first >= 0)
		{
			cs_user_list & list = map[cs_index];
			int id = pPlayer->ID.id;
			for(unsigned int i = 0; i < list.size(); i ++)
			{
				if(list[i].first == id)
				{
					list.erase(list.begin() + i);
					i --;
				}
			}
		}
	}

	inline void AddArenaMember(abase::vector<arena_member_entry>& vec, gplayer* pPlayer)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].id.id == pPlayer->ID.id)
			{
				vec[i].id = pPlayer->ID;
				vec[i].cs_sid = pPlayer->cs_sid;
				vec[i].cs_index = pPlayer->cs_index;
				vec[i].is_death = false;

				return;
			}
		}

		arena_member_entry member;

		member.id = pPlayer->ID;
		member.cs_sid = pPlayer->cs_sid;
		member.cs_index = pPlayer->cs_index;
		member.is_death = false;

		vec.push_back(member);
	}

	inline void DelArenaMember(abase::vector<arena_member_entry>& vec, gplayer* pPlayer)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].id.id == pPlayer->ID.id)
			{
				vec.erase(vec.begin() + i);
				i--;
			}
		}
	}

	inline void DeathArenaMember(abase::vector<arena_member_entry>& vec, gplayer* pPlayer)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].id.id == pPlayer->ID.id)
			{
				vec[i].is_death = true;
				break;
			}
		}
	}

	inline int GetAliveMembers(abase::vector<arena_member_entry> vec)
	{
		int count = 0;

		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i].is_death)
				continue;

			count++;
		}

		return count;
	}


	inline void AddVectorInt(abase::vector<int>& vec, gplayer* pPlayer)
	{
		for (unsigned int i = 0; i < vec.size(); i++)
		{
			if (vec[i] == pPlayer->ID.id)
			{
				return;
			}
		}

		vec.push_back(pPlayer->ID.id);
	}

public:

	void OnPlayerEnter(gplayer* pPlayer);
	void OnPlayerLeave(gplayer* pPlayer);
	void OnDeath(gplayer* pPlayer);
	void OnDamage(int attacker, int damage);

	int GetTeamType(int player_id);
	bool IsAlivePlayer(int player_id);
	bool IsDeathPlayer(int player_id);
};

#endif
