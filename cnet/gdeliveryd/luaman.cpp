//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT GDELIVERYD (C) DeadRaky 2022
//---------------------------------------------------------------------------------------------------------------------------
#include <utf.h>
#include "mapuser.h"
#include "rpcdefs.h"
#include "luaman.hpp"
#include "maplinkserver.h"
#include "gdeliveryserver.hpp"
#include "chatsinglecast.hpp"
#include "chatbroadcast.hpp"
#include "playerluainfo.hpp"
#include <LuaBridge.h>
using namespace luabridge;

namespace GNET
{ 
	LuaManager* LuaManager::instance = 0;
	lua_State* LuaManager::L;
	pthread_mutex_t LuaManager::lua_mutex;
	const char * LuaManager::FName;
	time_t LuaManager::reload_tm = 0;
	size_t LuaManager::tick = 0;
	bool LuaManager::lua_debug = false;
	LuaManager::CONFIG LuaManager::config;

	time_t LuaManager::GetFileTime(const char *path)
	{
		struct stat statbuf;
		if (stat(path, &statbuf) == -1) {
			perror(path);
			exit(1);
		}
		return statbuf.st_mtime;
	}

	bool LuaManager::IsTrue(int it, int * table)
	{
		for (unsigned int i = 0; table[i] && i < 128 ; i++)
			if (table[i] == it)
				return true;
		return false;
	}

	bool LuaManager::GetNum(const char* s, double& v)
	{
		LuaRef out = getGlobal(L, s);
		if (!out.isNil() && out.isNumber())
		{
			v = out;
			return true;
		}
		printf("LuaManager::GET_NUM: error %s not found! \n", s);
		return false;
	}

	bool LuaManager::GetString(const char* s, const char* v)
	{
		LuaRef out = getGlobal(L, s);
		if (!out.isNil() && out.isString())
		{
			v = out;
			return true;
		}
		printf("LuaManager::GET_STR: error %s not found! \n", s);
		return false;
	}

	bool LuaManager::SetNum(const char* s, double v)
	{
		LuaRef out = getGlobal(L, s);
		if (!out.isNil() && out.isNumber())
		{
			setGlobal(L, v, s);
			return true;
		}
		printf("LuaManager::SET_NUM: error %s not found! \n", s);
		return false;
	}

	bool LuaManager::SetString(const char* s, const char* v)
	{
		LuaRef out = getGlobal(L, s);
		if (!out.isNil() && out.isString())
		{
			setGlobal(L, v, s);
			return true;
		}
		printf("LuaManager::SET_STR: error %s not found! \n", s);
		return false;
	}

	bool LuaManager::SetAddr(const char* s, long long v)
	{
		LuaRef out = getGlobal(L, s);
		if (!out.isNil() && out.isNumber())
		{
			setGlobal(L, v, s);
			return true;
		}
		printf("LuaManager::SET_ADDR: error %s not found! \n", s);
		return false;
	}

	void LuaManager::game__Patch(long long address, int type, double value)
	{
		if (address)
		{
			switch (type)
			{
			case TYPE_CHAR: { *(char*)address = value; return; break; }
			case TYPE_SHORT: { *(short*)address = value; return; break; }
			case TYPE_INT: { *(int*)address = value; return; break; }
			case TYPE_INT64: { *(long long*)address = value; return; break; }
			case TYPE_FLOAT: { *(float*)address = value; return; break; }
			case TYPE_DOUBLE: { *(double*)address = value; return; break; }
			default: { printf("game__Patch: ERROR TYPE %d ! \n", type); return; break; }
			}
		}
	}

	double LuaManager::game__Get(long long address, int type, long long offset)
	{
		if (address)
		{
			switch (type)
			{
			case TYPE_CHAR: { return *(char*)(&((char*)address)[offset]); break; }
			case TYPE_SHORT: { return *(short*)(&((char*)address)[offset]); break; }
			case TYPE_INT: { return *(int*)(&((char*)address)[offset]); break; }
			case TYPE_INT64: { return *(long long*)(&((char*)address)[offset]); break; }
			case TYPE_FLOAT: { return *(float*)(&((char*)address)[offset]); break; }
			case TYPE_DOUBLE: { return *(double*)(&((char*)address)[offset]); break; }
			default: { printf("game__Get: ERROR TYPE %d ! \n", type);   return 0; break; }
			}
		}
		return 0;
	}

	void LuaManager::game__SingleChatMsg(int roleid, int channel, const char * utf8_msg)
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		size_t utf8_size = strlen(utf8_msg);
		if (pinfo && utf8_size && utf8_size < 256)
		{
			Octets msg;
			msg.resize(utf8_size*2 + 2);
			memset(msg.begin(), 0x00, msg.size());
			utf8_to_utf16((const utf8_t*)utf8_msg, utf8_size, (utf16_t*)msg.begin(), msg.size() );
			ChatSingleCast csc(channel,0,0, pinfo->roleid, pinfo->localsid, msg, 0 );
			GDeliveryServer::GetInstance()->Send(pinfo->linksid,csc);
		}
	}

	void LuaManager::game__ChatBroadCast(int roleid, int channel, const char * utf8_msg)
	{
		size_t utf8_size = strlen(utf8_msg);
		if (utf8_size && utf8_size < 256)
		{
			Octets msg;
			msg.resize(utf8_size*2 + 2);
			memset(msg.begin(), 0x00, msg.size());
			utf8_to_utf16((const utf8_t*)utf8_msg, utf8_size, (utf16_t*)msg.begin(), msg.size() );
			WorldChat chat(channel,0, roleid, 0, msg, 0);
			LinkServer::GetInstance().BroadcastProtocol(&chat);
		}
	}

	void LuaManager::FunctionsRegister()
	{
		getGlobalNamespace(L).addFunction("game__Patch",game__Patch);
		getGlobalNamespace(L).addFunction("game__Get",game__Get);
		getGlobalNamespace(L).addFunction("game__SingleChatMsg",game__SingleChatMsg);
		getGlobalNamespace(L).addFunction("game__ChatBroadCast",game__ChatBroadCast);
	}

	void LuaManager::SetItem()
	{
		SetAddr("HW_TAGS"				, (long long)config.HW_TAGS    );
		SetAddr("COUNTRY_PLAYERS_CNT"	, (long long)config.COUNTRY_PLAYERS_CNT );
	}

	void LuaManager::GetItem()
	{
		double res = -1;
		GetNum( "lua_debug"				, res ) ? lua_debug						= res  : res == -1;
		GetNum( "COUNTRY_MAX_BONUS"		, res ) ? config.COUNTRY_MAX_BONUS		= res  : res == -1;	
	}

	void LuaManager::FunctionsExec()
	{
		config.INIT();
	}

	#define LUA_MUTEX_BEGIN pthread_mutex_lock(&lua_mutex); \
							try { 

	#define LUA_MUTEX_END	} \
							catch (...) { printf("LUA::PANIC::EXCEPTION: ERROR \n"); } \
							pthread_mutex_unlock(&lua_mutex);

	void LuaManager::Init()
	{
		tick = 0;
		FName = "script.lua";
		pthread_mutex_init(&lua_mutex,0);
		LUA_MUTEX_BEGIN
		usleep(64);
		reload_tm = GetFileTime(FName);
		L = luaL_newstate();
		luaL_openlibs(L);
		FunctionsRegister();
		FunctionsExec();
		luaL_dofile(L, FName);
		SetItem();
		LuaRef Event = getGlobal(L, "Init");
		if(!Event.isNil())
		Event();
		else 
		printf("LUA::Event: NULL!!! Init \n");
		GetItem();
		LUA_MUTEX_END
	}

	void LuaManager::Update()
	{
		time_t last_tm = GetFileTime(FName);
		if(reload_tm == last_tm) return;	
		reload_tm = last_tm;
		LUA_MUTEX_BEGIN
		usleep(64);
		lua_close(L);
		L = luaL_newstate();
		luaL_openlibs(L);
		FunctionsRegister();
		FunctionsExec();
		luaL_dofile(L, FName);
		SetItem();
		LuaRef Event = getGlobal(L, "Update");
		if(!Event.isNil())
		Event();
		else 
		printf("LUA::Event: NULL!!! Update \n");
		GetItem();
		LUA_MUTEX_END
	}

	void LuaManager::HeartBeat()
	{
		LUA_MUTEX_BEGIN
		usleep(4);
		LuaRef Event = getGlobal(L, "HeartBeat");
		if(!Event.isNil())
		Event(tick);
		else 
		printf("LUA::Event: NULL!!! HeartBeat \n");
		LUA_MUTEX_END
		if( !(tick++ & 30) )
		Update();
	}

	time_t LuaManager::BidBeginTime(time_t now)
	{
		time_t res = 327600;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "BidBeginTime");
		if(!Event.isNil())
		res = Event(now)[0];
		else 
		printf("LUA::Event: NULL!!! BidBeginTime \n");
		LUA_MUTEX_END
		return res;
	}

	time_t LuaManager::BidEndTime(time_t now)
	{
		time_t res = 414000;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "BidEndTime");
		if(!Event.isNil())
		res = Event(now)[0];
		else 
		printf("LUA::Event: NULL!!! BidEndTime \n");
		LUA_MUTEX_END
		return res;
	}

	time_t LuaManager::BattleTime(time_t now)
	{
		time_t res = 591000;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "BattleTime");
		if(!Event.isNil())
		res = Event(now)[0];
		else 
		printf("LUA::Event: NULL!!! BattleTime \n");
		LUA_MUTEX_END
		return res;
	}

	time_t LuaManager::RewardTime(time_t now)
	{
		time_t res = 475200;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "RewardTime");
		if(!Event.isNil())
		res = Event(now)[0];
		else 
		printf("LUA::Event: NULL!!! RewardTime \n");
		LUA_MUTEX_END
		return res;
	}

	time_t LuaManager::BattleInterval(size_t num)
	{
		time_t res = 180;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "BattleInterval");
		if(!Event.isNil())
		res = Event(num)[0];
		else 
		printf("LUA::Event: NULL!!! BattleInterval \n");
		LUA_MUTEX_END
		if(res < 180) res = 180;
		return res;
	}

	time_t LuaManager::CountryBattleStartTime()
	{
		time_t res = (20 * 3600 + 20 * 60);
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryBattleStartTime");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryBattleStartTime \n");
		LUA_MUTEX_END
		return res;
	}	

	time_t LuaManager::CountryBattleBonusTime()
	{
		time_t res = (22 * 3600 + 21 * 60);
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryBattleBonusTime");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryBattleBonusTime \n");
		LUA_MUTEX_END
		return res;
	}

	time_t LuaManager::CountryBattleClearTime()
	{
		time_t res = (23 * 3600 + 30 * 60);
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryBattleClearTime");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryBattleClearTime \n");
		LUA_MUTEX_END
		return res;
	}

	size_t LuaManager::CountryMaxCount()
	{
		size_t res = 4;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryMaxCount");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryMaxCount \n");
		LUA_MUTEX_END
		if(res < 2 || res > 4)
		res = 4;
		return res;
	}
	
	size_t LuaManager::CountryBattleBonus()
	{
		time_t res = 85000;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryBattleBonus");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryBattleBonus \n");
		LUA_MUTEX_END
		return res;
	}

	int LuaManager::CountryBattleItem()
	{
		int res = 36343;
		LUA_MUTEX_BEGIN
		usleep(8);
		LuaRef Event = getGlobal(L, "CountryBattleItem");
		if(!Event.isNil())
		res = Event()[0];
		else 
		printf("LUA::Event: NULL!!! CountryBattleItem \n");
		LUA_MUTEX_END
		return res;
	}

	int LuaManager::EventOnEnterServer(int roleid, Octets & hwid, int & gameid)
	{
		int res = 0;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo && pinfo->gameid > 0)
		{
			pinfo->hwid = hwid;
			
			gameid = pinfo->gameid;
			if ( gameid > 1 && hwid.size() == 8 && *(unsigned long long*)hwid.begin() )
			{
				size_t counter = config.IS_TRUE_TAG_LIMITER(gameid);
				if (counter)
				{
					size_t count_users = UserContainer::GetInstance().GetTagHwidCounter( gameid, hwid );
					printf("LuaManager::EventOnEnterServer: roleid=%d, hwid=%lld, gameid=%d, counter=%lld , max_counter=%lld \n", roleid, *(unsigned long long*)hwid.begin(), gameid, count_users, counter );
					
					if ( count_users > counter)
					{
						res = 1;
						printf("LuaManager::EventOnEnterServer: roleid=%d , counter=%d, max_counter=%d res=%d \n", roleid, count_users, counter, res);
					}
				}
			}
		}
		//printf("LuaManager::EventOnEnterServer: roleid=%d , hSize = %d, gameid=%d, res = %d \n", roleid, hwid.size(), gameid, res);
		return res;
	}

	int LuaManager::EventOnSwitchServer(int roleid, int gameid)
	{
		int res = 0;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo && pinfo->gameid > 0)
		{
			Octets hwid = pinfo->hwid;
			if ( gameid > 1 && hwid.size() == 8 && *(unsigned long long*)hwid.begin() )
			{
				size_t counter = config.IS_TRUE_TAG_LIMITER(gameid);
				if (counter)
				{
					size_t count_users = UserContainer::GetInstance().GetTagHwidCounter( gameid, hwid );
					printf("LuaManager::EventOnSwitchServer: roleid=%d, hwid=%lld, gameid=%d, counter=%lld , max_counter=%lld \n", roleid, *(unsigned long long*)hwid.begin(), gameid, count_users, counter );
					
					if ( count_users >= counter)
					{
						res = 1;
						printf("LuaManager::EventOnSwitchServer: roleid=%d , counter=%d, max_counter=%d res=%d \n", roleid, count_users, counter, res);
					}
				}
			}
		}
		//printf("LuaManager::EventOnSwitchServer: roleid=%d , gameid = %d res=%d \n", roleid, gameid, res);
		return res;
	}

	int LuaManager::EventOnPlayerLuaInfo(int roleid, Octets & info)
	{
		int res = 0;
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo && pinfo->gameid > 0)
		{
			GDeliveryServer::GetInstance()->Send(pinfo->linksid, PlayerLuaInfo(roleid , pinfo->gameid, pinfo->localsid));
			res = 1;
			//TODO
		}
		//printf("LuaManager::EventOnPlayerLuaInfo: roleid=%d , size=%d, game_id=%d, res=%d \n", roleid, info.size(), pinfo->gameid, res);
		return res;
	}
	
	void LuaTimer::UpdateTimer()
	{
		LuaManager::GetInstance()->HeartBeat();
	}

	void LuaTimer::Run()
	{
		UpdateTimer();
		Thread::HouseKeeper::AddTimerTask(this,update_time);
	}

};

