//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT GDELIVERYD (C) DeadRaky 2023
//---------------------------------------------------------------------------------------------------------------------------
#define LUABRIDGE

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>

#include "thread.h"
#include "timer.h"
#include "itimer.h"

#include <lua.hpp>
#include <LuaBridge.h>
#include <luabase.h>

using namespace luabridge;
using namespace GNET;

// ================================================================
// LUA STATIC  
// ================================================================



// ================================================================
// LUA TIMER  
// ================================================================

class LuaTimer : public Thread::Runnable
{
	LuaBase * lua;
	int update_time;
public:
	LuaTimer(LuaBase * _lua, int _time, int _proir=1) : Runnable(_proir), update_time(_time), lua(_lua) { }
	void Run()
	{
		UpdateTimer();
		Thread::HouseKeeper::AddTimerTask(this,update_time);
	}
private:
	void UpdateTimer()
	{
		if (lua) lua->HeartBeat();
	}
};

// ================================================================
// LUA CONSTRICTOR FUNCTIONS 
// ================================================================

LuaBase::LuaBase()
{
    pthread_mutex_init(&lua_mutex, NULL);
	pthread_mutex_lock(&lua_mutex);
	L = NULL;
    tick = 0;
	reload_tm = 0;
	lua_debug = false;
	FName = NULL;
	pthread_mutex_unlock(&lua_mutex);
}

LuaBase::~LuaBase()
{
    lua_close(L);
    pthread_mutex_destroy(&lua_mutex);
}

void LuaBase::Lock()
{
	pthread_mutex_lock(&lua_mutex);
}

void LuaBase::Unlock()
{
	pthread_mutex_unlock(&lua_mutex);
}

// ================================================================
// LUA GLOBAL FUNCTIONS 
// ================================================================

time_t LuaBase::GetFileTime(const char* path)
{
	struct stat statbuf;
	if (stat(path, &statbuf) == -1) {
		perror(path);
		exit(1);
	}
	return statbuf.st_mtime;
}

bool LuaBase::IsTrue(int it, int * table)
{
	for (unsigned int i = 0; table[i] && i < 128 ; i++)
		if (table[i] == it)
			return true;
	return false;
}

bool LuaBase::GetNum(const char* s, double& v)
{
	LuaRef out = getGlobal(L, s);
	if (!out.isNil() && out.isNumber())
	{
		v = out;
		return true;
	}
	printf("LuaBase::GET_NUM: error %s not found! \n", s);
	return false;
}

bool LuaBase::GetString(const char* s, const char* v)
{
	LuaRef out = getGlobal(L, s);
	if (!out.isNil() && out.isString())
	{
		v = out;
		return true;
	}
	printf("LuaBase::GET_STR: error %s not found! \n", s);
	return false;
}

bool LuaBase::SetNum(const char* s, double v)
{
	LuaRef out = getGlobal(L, s);
	if (!out.isNil() && out.isNumber())
	{
		setGlobal(L, v, s);
		return true;
	}
	printf("LuaBase::SET_NUM: error %s not found! \n", s);
	return false;
}

bool LuaBase::SetString(const char* s, const char* v)
{
	LuaRef out = getGlobal(L, s);
	if (!out.isNil() && out.isString())
	{
		setGlobal(L, v, s);
		return true;
	}
	printf("LuaBase::SET_STR: error %s not found! \n", s);
	return false;
}

bool LuaBase::SetAddr(const char* s, long long v)
{
	LuaRef out = getGlobal(L, s);
	if (!out.isNil() && out.isNumber())
	{
		setGlobal(L, v, s);
		return true;
	}
	printf("LuaBase::SET_ADDR: error %s not found! \n", s);
	return false;
}

void LuaBase::game__Patch(long long address, int type, double value)
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

double LuaBase::game__Get(long long address, int type, long long offset)
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

void LuaBase::Register()
{
	getGlobalNamespace(L).addFunction("game__Patch", game__Patch);
	getGlobalNamespace(L).addFunction("game__Get", game__Get);
	FunctionsRegister();
}

void LuaBase::SetIt()
{
	LUA_MUTEX_BEGIN
	SetItem();
	LUA_MUTEX_END
}

void LuaBase::GetIt()
{
	LUA_MUTEX_BEGIN
	double res = -1;
	GetNum("lua_debug", res) ? lua_debug = res : res == -1;
	GetItem();
	LUA_MUTEX_END
}

void LuaBase::Exec()
{
	FunctionsExecute();
}


// ================================================================
// LUA GLOBAL FUNCTIONS 
// ================================================================

void LuaBase::Init(const char * fname)
{
	LUA_MUTEX_BEGIN
	FName = fname;
	reload_tm = GetFileTime(FName);
	L = luaL_newstate();
	luaL_openlibs(L);
	Register();
	Exec();
	luaL_dofile(L, FName);
	LUA_MUTEX_END

	SetIt();
	EventInit();
	GetIt();
	
	lua_timer = new LuaTimer(this,1);
	Thread::Pool::AddTask(lua_timer);
}

void LuaBase::Init()
{
	LUA_MUTEX_BEGIN
	FName = "script.lua";
	reload_tm = GetFileTime(FName);
	L = luaL_newstate();
	luaL_openlibs(L);
	Register();
	Exec();
	luaL_dofile(L, FName);
	LUA_MUTEX_END

	SetIt();
	EventInit();
	GetIt();
	
	lua_timer = new LuaTimer(this,1);
	Thread::Pool::AddTask(lua_timer);
}

void LuaBase::Update()
{
	time_t last_tm = GetFileTime(FName);
	if (reload_tm == last_tm) return;
	reload_tm = last_tm;

	LUA_MUTEX_BEGIN
	lua_close(L);
	L = luaL_newstate();
	luaL_openlibs(L);
	Register();
	Exec();
	luaL_dofile(L, FName);
	LUA_MUTEX_END

	SetIt();
	EventUpdate();
	GetIt();
}

void LuaBase::HeartBeat()
{
	tick++;
	if (!(tick % 30))
	{
		Update();
	}
	
	EventHeartBeat();
}

// ================================================================
// LUA VIRTUAL FUNTIONS 
// ================================================================

void LuaBase::EventInit()
{
	Event("Init");
}

void LuaBase::EventUpdate()
{
	Event("Update");
}

void LuaBase::EventHeartBeat()
{
	Event("HeartBeat", tick);
}

void LuaBase::FunctionsRegister()
{

}

void LuaBase::FunctionsExecute()
{

}

void LuaBase::SetItem()
{

}

void LuaBase::GetItem()
{

}



