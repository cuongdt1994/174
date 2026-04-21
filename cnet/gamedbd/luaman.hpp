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
			
			STORAGE_TAG				= 0,
			STORAGE_USER			= 1,
			STORAGE_ROLE			= 2,
			STORAGE_OTHER			= 3,
			STORAGE_MAX				= 4,
			
			
			MODE_1X1 = 1,
			MODE_3X3 = 2,
			MODE_6X6 = 3,
			
		};
	private:
	#pragma pack(push, 1)
	
	struct AUTOSWAP
	{
		short count;
		struct 
		{
			char inv;
			char equip;
		} pos[64];	
	};

	struct SKILLSENDER
	{
		short pos;
		short filter;
		struct 
		{
			short id;
			short type;
		} skill[32];	
	};

	struct CONFIG
	{
		size_t max_member1;
		size_t max_member2;
		size_t max_member3;
		int battle_bid_item;
		int alliance_off;
		
		void INIT() 
		{
			max_member1 = 50;
			max_member2 = 100;
			max_member3 = 200;
			battle_bid_item = 21652;
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
		
		static int game__CreateData(int type, int key);
		static int game__DeleteData(int type, int key);
		static int game__SetData(int type, int key, Octets & o);
		static int game__GetData(int type, int key, Octets & o);
		
		static int game__CreateBox(int key);
		static int game__DeleteBox(int key);
		static int game__SetBox(int key, GNewTrashBox & o );
		static int game__GetBox(int key, GNewTrashBox & o);
		
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