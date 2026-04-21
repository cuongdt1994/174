//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT GDELIVERYD (C) DeadRaky 2022
//---------------------------------------------------------------------------------------------------------------------------
#include <utf.h>
#include "rpcdefs.h"
#include "dbbuffer.h"
#include "grefstore"
#include "gluastorage"
#include "gnewtrashbox"

#include "luaman.hpp"
#include <LuaBridge.h>
using namespace luabridge;

namespace GNET
{ 
	LuaManager* LuaManager::instance = 0;
	lua_State* LuaManager::L;
	pthread_mutex_t LuaManager::lua_mutex;
	pthread_mutex_t LuaManager::storage_mutex;
	const char * LuaManager::FName;
	time_t LuaManager::reload_tm = 0;
	size_t LuaManager::tick = 0;
	bool   LuaManager::lua_debug = false;
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

	int LuaManager::game__CreateData(int type, int key)
	{
		int res = 0;
		const char * name;
		
		switch (type)
		{
		case STORAGE_TAG : { name = "luatag";   break; }
		case STORAGE_USER: { name = "luauser";  break; }
		case STORAGE_ROLE: { name = "luarole";  break; }
		case STORAGE_OTHER:{ name = "luaother"; break; }
		default: { printf("LUA::CreateData: Error type %d !!! \n", type); return res; break; }	
		}
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os;
		GLuaStorage	value;
		value.key = key;
		value.data.clear();
		
		try
		{
			StorageEnv::Storage * plua = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				os << value;
				plua->insert( Marshal::OctetsStream() << key, os , txn );
				res = 1;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			
			
		}
		catch ( DbException e )
		{
			printf("LUA::CreateData: Error file %s !!! \n", name);
		}

		pthread_mutex_unlock(&storage_mutex);
		if(lua_debug)printf("LUA::CreateData: type = %d , key = %d , db_name = %s , result = %d \n", type,key,name,res);
		return res;
	}

	int LuaManager::game__DeleteData(int type, int key)
	{
		int res = 0;
		const char * name;
		
		switch (type)
		{
		case STORAGE_TAG : { name = "luatag";   break; }
		case STORAGE_USER: { name = "luauser";  break; }
		case STORAGE_ROLE: { name = "luarole";  break; }
		case STORAGE_OTHER:{ name = "luaother"; break; }
		default: { printf("LUA::DeleteData: Error type %d !!! \n", type); return res; break; }	
		}
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os;
		GLuaStorage	value;
		
		try
		{
			StorageEnv::Storage * plua = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( plua->find( Marshal::OctetsStream() << key, os, txn ) )
				{
					try 
					{
						os >> value;
						if (value.key == key)
						{
							plua->del( Marshal::OctetsStream() << key, txn );
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::DeleteData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::DeleteData: Error file %s !!! \n", name);
		}
		
		pthread_mutex_unlock(&storage_mutex);
		if(lua_debug)printf("LUA::DeleteData: type = %d , key = %d , db_name = %s , result = %d \n", type,key,name,res);
		return res;
	}

	int LuaManager::game__SetData(int type, int key, Octets & o)
	{
		int res = 0;
		
		if(o.size() < 0) 
		{
			printf("LUA::SetData: Error o.size() < 0 !!! \n");
			return res;
		}
		
		const char * name;
		switch (type)
		{
		case STORAGE_TAG : { name = "luatag";   break; }
		case STORAGE_USER: { name = "luauser";  break; }
		case STORAGE_ROLE: { name = "luarole";  break; }
		case STORAGE_OTHER:{ name = "luaother"; break; }
		default: { printf("LUA::SetData: Error type %d !!! \n", type); return res; break; }	
		}
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os, old_os;
		GLuaStorage	value, old_value;
		value.key = key;
		value.data= o;
		
		try
		{
			StorageEnv::Storage * plua = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( plua->find( Marshal::OctetsStream() << key, old_os, txn ) )
				{
					try 
					{
						old_os >> old_value;
						if (old_value.key == key)
						{
							os << value;
							plua->insert( Marshal::OctetsStream() << key, os , txn );
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::SetData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::SetData: Error file %s !!! \n", name);
		}
		
		pthread_mutex_unlock(&storage_mutex);
		if(lua_debug)printf("LUA::SetData: type = %d , key = %d , db_name = %s , result = %d , size=%d \n", type,key,name,res,o.size());
		return res;
	}

	int LuaManager::game__GetData(int type, int key, Octets & o)
	{
		int res = 0;
		const char * name;
		switch (type)
		{
		case STORAGE_TAG : { name = "luatag";   break; }
		case STORAGE_USER: { name = "luauser";  break; }
		case STORAGE_ROLE: { name = "luarole";  break; }
		case STORAGE_OTHER:{ name = "luaother"; break; }
		default: { printf("LUA::GetData: Error type %d !!! \n", type); return res; break; }	
		}
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os;
		GLuaStorage	value;
		
		try
		{
			StorageEnv::Storage * plua = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( plua->find( Marshal::OctetsStream() << key, os, txn ) )
				{
					try 
					{
						os >> value;
						size_t val_size = value.data.size();
						if (value.key == key && val_size > 0)
						{
							o = value.data;
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::GetData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::GetData: Error file %s !!! \n", name);
		}
		
		pthread_mutex_unlock(&storage_mutex);
		if(lua_debug)printf("LUA::GetData: type = %d , key = %d , db_name = %s , result = %d , size=%d \n", type,key,name,res,o.size());
		return res;
	}

//---------------------------------------------------------------------------------------------------------------

	int LuaManager::game__CreateBox(int key)
	{
		int res = 0;
		const char * name = "newtrashbox";
		pthread_mutex_lock(&storage_mutex);
		
		Marshal::OctetsStream os;
		GNewTrashBox value;
		
		value.roleid = key;
		value.size4 = 0; value.size5 = 0; value.size6 = 0; value.size7 = 0;
		value.box4.clear(); value.box5.clear(); value.box6.clear(); value.box7.clear();
		
		try
		{
			StorageEnv::Storage * pnewtrashbox = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				os << value;
				pnewtrashbox->insert( Marshal::OctetsStream() << key, os , txn );
				res = 1;
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
			
			
		}
		catch ( DbException e )
		{
			printf("LUA::CreateData: Error file %s !!! \n", name);
		}

		pthread_mutex_unlock(&storage_mutex);
		return res;
	}
	
	

	int LuaManager::game__DeleteBox(int key)
	{
		int res = 0;
		const char * name = "newtrashbox";
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os;
		GNewTrashBox value;
		
		try
		{
			StorageEnv::Storage * pnewtrashbox = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( pnewtrashbox->find( Marshal::OctetsStream() << key, os, txn ) )
				{
					try 
					{
						os >> value;
						if (value.roleid == key)
						{
							pnewtrashbox->del( Marshal::OctetsStream() << key, txn );
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::DeleteData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::DeleteData: Error file %s !!! \n", name);
		}
		
		pthread_mutex_unlock(&storage_mutex);
		return res;
	}

	int LuaManager::game__SetBox(int key, GNewTrashBox & o )
	{
		int res = 0;
		const char * name = "newtrashbox";
		
		if(key != o.roleid)
		{
			printf("LUA::SetData: Error key != o.roleid !!! \n");
			return res;
		}
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os, old_os;
		GNewTrashBox value, old_value;
		value = o;
		
		try
		{
			StorageEnv::Storage * pnewtrashbox = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( pnewtrashbox->find( Marshal::OctetsStream() << key, old_os, txn ) )
				{
					try 
					{
						old_os >> old_value;
						if (old_value.roleid == key)
						{
							os << value;
							pnewtrashbox->insert( Marshal::OctetsStream() << key, os , txn );
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::SetData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::SetData: Error file %s !!! \n", name);
		}
		
		//printf("LUA::SetData: roleid=%d , size4=%d, size5=%d !!! \n", o.roleid, o.size4, o.size5 );
		pthread_mutex_unlock(&storage_mutex);
		return res;
	}

	int LuaManager::game__GetBox(int key, GNewTrashBox & o)
	{
		int res = 0;
		const char * name = "newtrashbox";
		
		pthread_mutex_lock(&storage_mutex);
		Marshal::OctetsStream os;
		GNewTrashBox value;
		
		try
		{
			StorageEnv::Storage * plua = StorageEnv::GetStorage(name);
			StorageEnv::CommonTransaction txn;
			try
			{
				if( plua->find( Marshal::OctetsStream() << key, os, txn ) )
				{
					try 
					{
						os >> value;
						if (value.roleid == key)
						{
							o = value;
							res = 1;
						}
					}
					catch ( Marshal::Exception & )
					{
						printf("LUA::GetData: Decode Error role =  %d \n", key);
					}
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			printf("LUA::GetData: Error file %s !!! \n", name);
		}
		pthread_mutex_unlock(&storage_mutex);
		return res;
	}

//--------------------------------------------------------------------------------------------------------

	void LuaManager::FunctionsRegister()
	{
		getGlobalNamespace(L).addFunction("game__Patch",game__Patch);
		getGlobalNamespace(L).addFunction("game__Get",game__Get);
	}

	void LuaManager::SetItem()
	{
		//config.SET_ADDR("GUILD_ROLE_RANK",config.GUILD_ROLE_RANK);
	}

	void LuaManager::GetItem()
	{
		double res = -1;
		GetNum( "lua_debug"  		, res ) ? lua_debug					= res : res == -1;
		GetNum( "max_member1"		, res ) ? config.max_member1		= res : res == -1;
		GetNum( "max_member2"		, res ) ? config.max_member2		= res : res == -1;
		GetNum( "max_member3"		, res ) ? config.max_member3		= res : res == -1;
		GetNum( "battle_bid_item"	, res ) ? config.battle_bid_item	= res : res == -1;
		GetNum( "alliance_off"		, res ) ? config.alliance_off		= res : res == -1;
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
		reload_tm = GetFileTime(FName);
		L = luaL_newstate();
		luaL_openlibs(L);
		FunctionsRegister();
		FunctionsExec();
		luaL_dofile(L, FName);
		SetItem();
		
		static const char * EventName = "Init";
		LuaRef Event = getGlobal(L, EventName);
		if(L && !Event.isNil() && Event.isFunction() )
		{
			if (lua_debug) printf("LUA::Event::LOG: Event = %s \n", EventName);
			Event();
		}
		else 
		{
			printf("LUA::Event: NULL!!! %s \n", EventName);
		}
		
		GetItem();
		LUA_MUTEX_END
	}

	void LuaManager::Update()
	{
		time_t last_tm = GetFileTime(FName);
		if(reload_tm == last_tm) return;	
		reload_tm = last_tm;
		LUA_MUTEX_BEGIN
		lua_close(L);
		L = luaL_newstate();
		luaL_openlibs(L);
		FunctionsRegister();
		FunctionsExec();
		luaL_dofile(L, FName);
		SetItem();
		static const char * EventName = "Update";
		LuaRef Event = getGlobal(L, EventName);
		if(L && !Event.isNil() && Event.isFunction() )
		{
			if (lua_debug) printf("LUA::Event::LOG: Event = %s \n", EventName);
			Event();
		}
		else 
		{
			printf("LUA::Event: NULL!!! %s \n", EventName);
		}
		GetItem();
		LUA_MUTEX_END
	}

	void LuaManager::HeartBeat()
	{
		LUA_MUTEX_BEGIN
		static const char * EventName = "HeartBeat";
		LuaRef Event = getGlobal(L, EventName);
		if(L && !Event.isNil() && Event.isFunction() )
		{
			//if (lua_debug) printf("LUA::Event::LOG: Event = %s , Tick = %d \n", EventName, tick);
			Event(tick);
		}
		else 
		{
			printf("LUA::Event: NULL!!! %s \n", EventName);
		}
		tick++;
		LUA_MUTEX_END
		
		if( !(tick % 30) )
		{
			Update();
		}
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

