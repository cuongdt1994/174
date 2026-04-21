//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT GDELIVERYD (C) DeadRaky 2022
//---------------------------------------------------------------------------------------------------------------------------
#ifndef __GNET_LUAMANAGER_H
#define __GNET_LUAMANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <sys/stat.h>
#include <sys/time.h>
#include <lua.hpp>
#include "rpcdefs.h"

namespace GNET
{ 

	class LuaManager
	{
	private:
		enum
		{
			MIN  					= 60,
			HOUR 					= MIN*60,
			DAY  					= HOUR*24,
			WEEK 					= DAY*7,
			
			TYPE_CHAR 				= 0,
			TYPE_SHORT 				= 1,
			TYPE_INT 				= 2,
			TYPE_INT64 				= 3,
			TYPE_FLOAT 				= 4,
			TYPE_DOUBLE 			= 5,
				
			CHAT_SINGLE				= 0,
			CHAT_INSTANCE			= 1,
			CHAT_WORLD				= 2,
			CHAT_MAX				= 3,
			
			MAX_TAGS				= 128,
			MAX_SKILL				= 128,
			MAX_ITEM				= 128,
			
			WAR_TYPE_MAX 			= 3,
			
		};
	private:
#pragma pack(push, 1)
		struct CONFIG
		{
			int COUNTRY_MAX_BONUS = 10000;
			int COUNTRY_PLAYERS_CNT[WAR_TYPE_MAX] = {30, 20, 40};
			
			struct
			{
				int tag;
				int limit;
			}	HW_TAGS[MAX_TAGS];
			
			void INIT()
			{
				memset(HW_TAGS, 0x00, sizeof(HW_TAGS));				
			}
			
			size_t IS_TRUE_TAG_LIMITER(int tag)
			{
				for (size_t i = 0; HW_TAGS[i].tag && i < 128 ; i++)
					if (HW_TAGS[i].tag == tag)
						return HW_TAGS[i].limit;
				return 0;
			}
		};
#pragma pack(pop)
	private:
		static lua_State* L;
		static pthread_mutex_t lua_mutex;
		static const char * FName;
		static time_t reload_tm;
		static size_t tick;
		static CONFIG config;
		static bool lua_debug;
		
	public:
		static void game__Patch(long long address, int type, double value);
		static double game__Get(long long address, int type, long long offset);
		static void game__SingleChatMsg(int roleid, int channel, const char * utf8_msg);
		static void game__ChatBroadCast(int roleid, int channel, const char * utf8_msg);
		static int game__CheckRoleGsArena(int roleid);
	public:	
		time_t GetFileTime(const char *path);
		bool GetNum(const char* s, double& v);
		bool GetString(const char* s, const char* v);
		bool SetNum(const char* s, double v);
		bool SetString(const char* s, const char* v);
		bool SetAddr(const char* s, long long v);
		bool IsTrue(int it, int * table);
		
		void FunctionsRegister();
		void FunctionsExec();
		void SetItem();
		void GetItem();
		void Init();
		void Update();
		void HeartBeat();
		
		time_t BidBeginTime(time_t now);
		time_t BidEndTime(time_t now);
		time_t BattleTime(time_t now);
		time_t RewardTime(time_t now);
		time_t BattleInterval(size_t num);
		
		time_t CountryBattleStartTime();
		time_t CountryBattleBonusTime();
		time_t CountryBattleClearTime();
		size_t CountryMaxCount();
		size_t CountryBattleBonus();
		int    CountryBattleItem();
		
		int EventOnEnterServer(int roleid, Octets & hwid, int & gameid);
		int EventOnSwitchServer(int roleid, int tag);
		int EventOnPlayerLuaInfo(int roleid, Octets & info);

	public:
		CONFIG * GetConfig() { return &config; }


		static LuaManager * GetInstance()
		{
			if (!instance)
			instance = new LuaManager();
			return instance;
		}
		static LuaManager * instance;
	};

	class LuaTimer : public Thread::Runnable
	{
		int update_time;
	public:
		LuaTimer(int _time,int _proir=1) : Runnable(_proir),update_time(_time) { }
		void Run();
	private:
		void UpdateTimer();
	};

};

#endif