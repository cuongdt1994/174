#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include <db_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "usermsg.h"
#include "luamanager.h"
#include <glog.h>


void gplayer_lua::Init()
{
	memset(&_lua_role,0,sizeof(_lua_role));
	memset(&_lua_user,0,sizeof(_lua_user));

	_lua_role._storage_1 = 0;
	_lua_role._storage_2 = 0;

	_lua_role._storage_reward = 0;
	memset(&_lua_role._anecdote_task_id,0,sizeof(_lua_role._anecdote_task_id));
	memset(&_lua_role._event_task_complete,0,sizeof(_lua_role._event_task_complete));

	_lua_role._imperial_enabled = 0;
	_lua_role._imperial_expire_time = 0;

	memset(&_lua_role._portatil_picture_storage,0,sizeof(_lua_role._portatil_picture_storage));
	_lua_role._portatil_picture_active = 0;

	memset(&_lua_role._new_armor_spirit,0,sizeof(_lua_role._new_armor_spirit));
	memset(&_lua_role._new_armor_cristal,0,sizeof(_lua_role._new_armor_cristal));
	_lua_role._reset_tower_day = 0;
	_lua_role._merge_zenith = false;
	_lua_role._reset_celestial_memorial = false;
	_lua_role._child_reset_day = -1;
	_lua_role._reset_lottery = false;
}

void gplayer_lua::EnterRole(int rid)
{
	if (rid >= 1024)
	{
		roleid = rid;
		LuaManager::GetInstance()->game__LoadLuaStorage(LuaManager::STORAGE_ROLE, rid);
	}
	else
	{
		printf("PLAYER_LUA::EnterRole: roleid=%d roleid_failed!!! \n", rid);
	}
}

void gplayer_lua::LoadRole(int rid, void * iData, unsigned int iSize)
{
	if (roleid == rid && iData && iSize)
	{
		if ( iSize <= sizeof(_lua_role) )
		{
			memcpy(&_lua_role , iData , iSize);
		}
		else
		{
			printf("PLAYER_LUA::LoadRole: roleid=%d iSize=%d , lua_size=%d size_failed!!! \n", rid, iSize, sizeof(_lua_role));
		}
	}
	else
	{
		if (roleid != rid)
		{
			printf("PLAYER_LUA::LoadRole: roleid=%d rid=%d roleid_failed!!! \n", roleid, rid);
		}
		printf("PLAYER_LUA::LoadRole: roleid=%d iSize=%d , lua_size=%d buff_failed!!! \n", rid, iSize, sizeof(_lua_role));
	}
}

void gplayer_lua::SaveRole()
{
	if (roleid >= 1024)
	{
		LuaManager::GetInstance()->game__SaveLuaStorage(LuaManager::STORAGE_ROLE, roleid, (long long)&_lua_role, sizeof(_lua_role) );
	}
	else
	{
		printf("PLAYER_LUA::SaveRole: roleid=%d roleid_failed!!! \n", roleid);
	}
}

