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

namespace GNET
{ 

	class LuaManager
	{
	public:
		enum
		{
			TYPE_CHAR 				= 0,
			TYPE_SHORT 				= 1,
			TYPE_INT 				= 2,
			TYPE_INT64 				= 3,
			TYPE_FLOAT 				= 4,
			TYPE_DOUBLE 			= 5,
		};
	private:
	#pragma pack(push, 1)
		struct CONFIG
		{
			int time_battle_enter = 3600*24;
			
			int GUILD_UPGRADE_COST[16];
			int GUILD_MAX_MEMBERS[16];
			int GUILD_ROLE_RANK[16];
			
			void INIT() 
			{
				size_t i;
				
				i=0;
				GUILD_UPGRADE_COST[i++] = 100000;
				GUILD_UPGRADE_COST[i++] = 2000000;
				GUILD_UPGRADE_COST[i++] = 10000000;
				
				i=0;
				GUILD_MAX_MEMBERS[i++] = 50;
				GUILD_MAX_MEMBERS[i++] = 100;
				GUILD_MAX_MEMBERS[i++] = 200;

				i=0;
				GUILD_ROLE_RANK[i++] = 0;
				GUILD_ROLE_RANK[i++] = 0;
				GUILD_ROLE_RANK[i++] = 1;
				GUILD_ROLE_RANK[i++] = 1;
				GUILD_ROLE_RANK[i++] = 4;
				GUILD_ROLE_RANK[i++] = 12;
				GUILD_ROLE_RANK[i++] = 200;
			}
		};
	#pragma pack(pop)
	private:
		static lua_State* L;
		static pthread_mutex_t lua_mutex;
		static pthread_mutex_t storage_mutex;
		static const char * FName;
		static time_t reload_tm;
		static size_t tick;
		static bool lua_debug;
		static CONFIG config;
		
	public:
		static void game__Patch(long long address, int type, double value);
		static double game__Get(long long address, int type, long long offset);
		
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


	public:
		CONFIG * GetConfig() { return &config; }

	public:
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