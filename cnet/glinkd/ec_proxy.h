//-------------------------------------------------------------------------------
//-- ELEMENT CLIENT PROXY PACKETS CNET --
//-------------------------------------------------------------------------------

#ifndef __GNET_ELEMENTCLIENT_PROXY_H
#define __GNET_ELEMENTCLIENT_PROXY_H

namespace GNET
{

class ArenaTeamInfo_Re;
class ArenaTeamCreate_Re;

class EC_Arena
{
public:
	bool logs;
	static EC_Arena * instance;
public:
	EC_Arena();
	~EC_Arena();

	static EC_Arena * GetInstance();
public:
	void Init();
	void HeartBeat();

	long long GetRoleIdx(int roleid, short zoneid, short reserve1 = 1 );
	long long GetTeamIdx(int teamid, char reserve1 = 1 );

	void ArenaQuery(int roleid, int localsid);
	void ArenaQuery_Re(int roleid, int localsid, ArenaTeamInfo_Re* ati_re );
	
	void ArenaPlayerTotalInfoQuery(int roleid, int localsid, int dest_roleid);
	void ArenaCreate_Re(int roleid, int localsid, ArenaTeamCreate_Re* atc_re );
	void ArenaCreate_Re(int roleid, int localsid);
	
	void SendCounter( int roleid, int localsid );
	void SendCounterExit( int roleid, int localsid, int param );
	
	
	
public:

};


















};

#endif