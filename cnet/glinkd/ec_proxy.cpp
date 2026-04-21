#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"

#include "gdeliveryclient.hpp"
#include "gproviderserver.hpp"
#include "glinkserver.hpp"

// joslian
//#include "arenateaminfo.hpp"
//#include "arenateaminfo_re.hpp"
//#include "arenateamcreate_re.hpp"

// elementclient
#include "ec_arenaquery_re.hpp"
#include "ec_arenateamdatanotify.hpp"
#include "ec_arenaplayertotalinfoquery_re.hpp"

#include "ec_arenateammatch.hpp"
#include "ec_arenaclientnotify.hpp"
#include "ec_arenamatchcancelnotify.hpp"
#include "ec_arenabattleinfonotifyclient.hpp"

#include "ec_proxy.h"

using namespace GNET;

//-------------------------------------------------------------------------------
//-- ELEMENT CLIENT ARENA GUAN YU EMULATOR BEGIN
//-------------------------------------------------------------------------------

EC_Arena* EC_Arena::instance = NULL;

//-------------------------------------------------------------------------------
	
EC_Arena::EC_Arena()
{
	logs = true;
}

EC_Arena::~EC_Arena()
{
	
}

EC_Arena * EC_Arena::GetInstance()
{
	if (!instance)
	instance = new EC_Arena();
	return instance;
}

//-------------------------------------------------------------------------------

void EC_Arena::Init()
{
	
}

void EC_Arena::HeartBeat()
{
	
}

long long EC_Arena::GetRoleIdx(int roleid, short zoneid, short reserve1 )
{
	struct ROLE_IDX
	{
		union
		{
			struct
			{
				int roleid;
				short reserve1;
				short zoneid;
			};

			long long id;
		};
		
		ROLE_IDX() { id = 0; }
		~ROLE_IDX() { }
	}	role_idx;
	
	role_idx.id = 0;
	role_idx.roleid = roleid;
	role_idx.zoneid = zoneid;
	role_idx.reserve1 = reserve1;
	return role_idx.id;
}

long long EC_Arena::GetTeamIdx(int teamid, char reserve1)
{
	struct TEAM_IDX
	{
		union
		{
			struct 
			{
				int teamid;
				char reserve1[4];
			};
			long long id;
		};
		
		TEAM_IDX() { id = 0; }
		~TEAM_IDX() { }
	}	team_idx;
	
	team_idx.id = 0;
	team_idx.teamid = teamid;
	team_idx.reserve1[3] = reserve1;
	return team_idx.id;
}

void EC_Arena::ArenaQuery(int roleid, int localsid)
{
	/*
	if (!GLinkServer::ValidRole(localsid, roleid))
	{
		GLinkServer::GetInstance()->SessionError(localsid, ERR_INVALID_ACCOUNT, "Error userid or roleid.");
		return;
	}
	
	ArenaTeamInfo ati( roleid, localsid, -1 );
	GDeliveryClient::GetInstance()->SendProtocol(ati);
	
	if (logs)
	{
		printf("EC_Arena::ArenaQuery: roleid=%d, localsid=%d \n", roleid, localsid );
	}
	*/
}

void EC_Arena::ArenaQuery_Re(int roleid, int localsid, ArenaTeamInfo_Re* ati_re )
{
	/*
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;
	
	int zoneid = 1;
	sinfo->teamid = ati_re->team.team_id;
	EC_ArenaQuery_Re aq_re(ati_re->retcode, ati_re->roleid);
	
	// team data
	aq_re.teamsync.resize(1);
	aq_re.teamsync[0].sync = 0;

	aq_re.teamsync[0].team.team_idx = GetTeamIdx( ati_re->team.team_id );
	aq_re.teamsync[0].team.role_idx = GetRoleIdx( ati_re->team.captain_id, zoneid );

	aq_re.teamsync[0].team.server = zoneid;
	aq_re.teamsync[0].team.score = ati_re->team.score;
	aq_re.teamsync[0].team.name = ati_re->team.name;

	// team data members
	aq_re.teamsync[0].team.members_idx.resize( ati_re->members.size() );
	for (size_t i = 0; i < aq_re.teamsync[0].team.members_idx.size(); i++)
	{
		aq_re.teamsync[0].team.members_idx[i].role_idx = GetRoleIdx( ati_re->members[i].player_id, zoneid );
		aq_re.teamsync[0].team.members_idx[i].award_count = 0;
	}

	aq_re.teamsync[0].team.teams_idx.clear(); // other teams 

	aq_re.teamsync[0].team.last_visite = ati_re->team.last_visite;
	aq_re.teamsync[0].team.win_count = ati_re->team.win_count;
	aq_re.teamsync[0].team.team_score = ati_re->team.score;
	
	
	aq_re.teamsync[0].team.week_battle_count = ati_re->team.week_battle_count;
	
	aq_re.teamsync[0].team.create_time = 0;
	aq_re.teamsync[0].team.battle_count = ati_re->team.battle_count;

	// player data
	aq_re.playerssync.resize( ati_re->members.size() );
	for (size_t i = 0; i < aq_re.playerssync.size(); i++)
	{
		aq_re.playerssync[i].player.role_idx = GetRoleIdx( ati_re->members[i].player_id, zoneid );
		aq_re.playerssync[i].player.team_idx = GetTeamIdx( ati_re->members[i].team_id );
		aq_re.playerssync[i].player.cls = 0; 
		
		
		aq_re.playerssync[i].player.team_score = ati_re->team.score;
		aq_re.playerssync[i].player.win_count = ati_re->members[i].win_count;
		aq_re.playerssync[i].player.score = ati_re->members[i].score;
		aq_re.playerssync[i].player.week_battle_count = ati_re->members[i].week_battle_count;
		aq_re.playerssync[i].player.invite_time = 0;
		aq_re.playerssync[i].player.last_visite = ati_re->members[i].last_visite;
		aq_re.playerssync[i].player.battle_count = ati_re->members[i].battle_count;
		
		aq_re.playerssync[i].sync = 0;
	}
	// player data end

	lsm->Send(localsid, aq_re);
	
	if (logs)
	{
		printf("EC_Arena::ArenaQuery_Re: roleid=%d, localsid=%d, retcode=%d, teamsize=%d, playersize=%d \n", roleid, localsid, aq_re.retcode, aq_re.teamsync.size(), aq_re.playerssync.size() );
	}
	*/
}

void EC_Arena::ArenaCreate_Re(int roleid, int localsid, ArenaTeamCreate_Re* atc_re )
{
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;
	/*
	long long ll_name = 14637166947926113; // asm4
	Octets name;
	name.replace(&ll_name, sizeof(ll_name));
	
	EC_ArenaTeamCreate atc
	{
		roleid,
		roleid,
		roleid,
		roleid,
		roleid,
		roleid,
		roleid,
		name,
		name
	};
	
	lsm->Send(localsid, atc);
	
	if (logs)
	{
		printf("EC_Arena::ArenaCreate_Re: roleid=%d, localsid=%d, retcode=%d \n", roleid, localsid, atc_re->retcode );
	}
	*/
}

void EC_Arena::ArenaCreate_Re(int roleid, int localsid )
{
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;
	
	/*
	long long ll_name = 14637166947926113; // asm4
	Octets name;
	name.replace(&ll_name, sizeof(ll_name));
	
	EC_ArenaTeamCreate atc
	{
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		name,
		name
	};
	
	lsm->Send(localsid, atc);
	
	if (logs)
	{
		printf("EC_Arena::ArenaCreate_Re: roleid=%d, localsid=%d \n", roleid, localsid );
	}
	*/
}

void EC_Arena::ArenaPlayerTotalInfoQuery(int roleid, int localsid, int dest_roleid)
{
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;
	
	if (logs)
	{
		printf("EC_Arena::ArenaCreate_Re: srcroleid=%d, localsid=%d, dstroleid=%d \n", roleid, localsid, dest_roleid );
	}
}


void EC_Arena::SendCounter( int roleid, int localsid )
{
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;
	/*
	EC_ArenaTeamMatch atm(0, roleid);
	atm.members.push_back(roleid);
	atm.members.push_back(roleid);
	atm.members.push_back(roleid);
	atm.queue_time = 600;
	*/
	
	EC_ArenaClientNotify acn(ERR_SUCCESS, roleid, 0, 5603);
	
	//if ( mask == 0 )
	{
		Marshal::OctetsStream os;
		os << (int)2;
		os << CompactUINT(3);
		
		for (size_t i = 0; i < 3; i++)
		{
			long long role = GetRoleIdx(roleid, 1 , 1); os << role;
		}
		
		os << 600;
		
		acn.success_data = os;
	}
	
	
	lsm->Send(localsid, acn);
}

void EC_Arena::SendCounterExit( int roleid, int localsid, int param )
{
	GLinkServer* lsm = GLinkServer::GetInstance();
	SessionInfo * sinfo = lsm->GetSessionInfo(localsid);
	if (!sinfo) 
		return;



	EC_ArenaClientNotify acn(ERR_SUCCESS, roleid, 0, 5603);
	
	//if ( mask == 0 )
	{
		Marshal::OctetsStream os;
		os << (int)param;
		os << CompactUINT(1);
		
		for (size_t i = 0; i < 1; i++)
		{
			long long role = GetRoleIdx(roleid, 1 , 1); os << role;
		}
		
		os << 600;
		
		acn.success_data = os;
	}
	
	
	lsm->Send(localsid, acn);

	
	if (param == 51)
	{
		EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, roleid, 4);
		{
			Marshal::OctetsStream os;
			os << CompactUINT(1);
			long long role = GetRoleIdx(roleid, 1 , 1); os << role;
			long long team = 0; os << team;
			os << 75;
			abinc.success_data = os;
			lsm->Send(localsid, abinc);
		}
		//if (type == 4)
		/*
		{
			Marshal::OctetsStream os;
			os << CompactUINT(1);
			
			for (size_t i = 0; i < 1; i++)
			{
				long long role = GetRoleIdx(roleid, 1 , 1); os << role;
			}
			
			long long team = GetTeamIdx(0,1); os << team;
			os << 75;
			
			abinc.success_data = os;
			lsm->Send(localsid, abinc);
		}
		*/
	}
	if (param == 52)
	{
		EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, roleid, 4);
		//if (type == 4)
		{
			Marshal::OctetsStream os;
			os << CompactUINT(3);
			
			for (size_t i = 0; i < 3; i++)
			{
				long long role = GetRoleIdx(roleid, 1 , 1); os << role;
			}
			
			long long team = GetTeamIdx(1,1); os << team;
			os << -1;
			
			abinc.success_data = os;
			lsm->Send(localsid, abinc);
		}
	}
	else if (param == 53)
	{
		EC_ArenaBattleInfoNotifyClient abinc(ERR_SUCCESS, roleid, 3);
		//if (type == 3)
		{
			Marshal::OctetsStream os;
			os << CompactUINT(3);
			
			for (size_t i = 0; i < 3; i++)
			{
				long long role = GetRoleIdx(roleid, 1 , 1); os << role;
			}
			
			os << 1;
			os << 1;
			
			abinc.success_data = os;
			lsm->Send(localsid, abinc);
		}
	}
}




//-------------------------------------------------------------------------------
//-- ELEMENT CLIENT ARENA GUAN YU EMULATOR END
//-------------------------------------------------------------------------------