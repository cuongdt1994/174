#ifndef __ONLINEGAME_GS_PLAYER_LUA_H__
#define __ONLINEGAME_GS_PLAYER_LUA_H__

#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>

#include "player_lottery.h"
#include "player_glyph.h"
#include "player_lib.h"
#include "luamanager.h"

#include <openssl/md5.h>
#include "item/item_addon.h"
#include "item/set_addon.h"
#include "petman.h"

class gplayer_lua
{
public:
	enum 
	{
		MAX_STORAGE_COUNT = 32,
	};

public:
	struct POINTS_EVENT
	{
		short task_id;
		short points;
	};

	struct ARMOR_NEW_SAVE_POS
	{
		int armor_id;
		short armor_pos;
		short inject_pos;
	};

#pragma pack(push, 4)
	struct LUA_ROLE
	{
		float storage[MAX_STORAGE_COUNT];

		short _storage_1; // Submundo Frígido
		short _storage_2; // Submundo Frígido

		unsigned int _storage_reward; // 0~6

		short _anecdote_task_id[71];
		POINTS_EVENT _event_task_complete[10];

		int _imperial_enabled;
		int _imperial_expire_time;

		int _portatil_picture_storage[6];
		int _portatil_picture_active;
	
		ARMOR_NEW_SAVE_POS _new_armor_spirit[32];		
		ARMOR_NEW_SAVE_POS _new_armor_cristal[32];		
		int _reset_tower_day;
		char _merge_zenith;
		bool _reset_celestial_memorial;
		char _child_reset_day;
		bool _reset_lottery;
	};

	struct LUA_USER
	{
		float storage[MAX_STORAGE_COUNT];
	};
#pragma pack(pop)
	int roleid;
	LUA_ROLE _lua_role;
	LUA_USER _lua_user;

	
	
public:
	
	void Init();
	void EnterRole(int rid);
	void LoadRole(int rid, void * iData, unsigned int iSize);
	void SaveRole();

	inline double GetLuaStorage(int pos) {	return _lua_role.storage[pos]; }
	inline void SetLuaStorage(int pos, double value) { _lua_role.storage[pos] = value; }
	inline double GetLuaUserStorage(int pos) {	return _lua_user.storage[pos]; }
	inline void SetLuaUserStorage(int pos, double value) { _lua_user.storage[pos] = value; }

	// Submundo Frígido
	inline void SetLuaStorage1(short value) { _lua_role._storage_1 = value; }
	inline void SetLuaStorage2(short value) { _lua_role._storage_2 = value; }
	inline short GetLuaStorage1() { return _lua_role._storage_1; }
	inline short GetLuaStorage2() { return _lua_role._storage_2; }

	// 0~6
	inline void SetLuaStorageReward(unsigned int value) { _lua_role._storage_reward = value; }
	inline unsigned int GetLuaStorageReward() { return _lua_role._storage_reward; }


	// Anedota
	inline void SetAnecdoteTaskid(short uTaskid)
	{
		for( unsigned int i = 0; i < 71; i++)
		{
			if(_lua_role._anecdote_task_id[i] == 0)
			{
				_lua_role._anecdote_task_id[i] = uTaskid;
				break;
			}
		}		
	}

	inline unsigned short GetAnecdoteTaskid(int pos)
	{
		return _lua_role._anecdote_task_id[pos];
	}

	inline bool CheckAnecdoteTaskid(short uTaskid)
	{
		for( unsigned int i = 0; i < 71; i++)
		{
			if(uTaskid == _lua_role._anecdote_task_id[i]) return true;
		}
		return false; // Caso não exista
	}

	// Pontos de evento
	inline void SetEventTaskComplete(unsigned int uTaskid)
	{
		for( unsigned int i = 0; i < 10; i++)
		{
			if(_lua_role._event_task_complete[i].task_id == 0)
			{
				_lua_role._event_task_complete[i].task_id = uTaskid;				
				break;
			}
		}		
	}

	inline void SetEventTaskCompleteAddPoints(unsigned int uTaskid, short points)
	{
		for( unsigned int i = 0; i < 10; i++)
		{
			if(_lua_role._event_task_complete[i].task_id == uTaskid)
			{
				_lua_role._event_task_complete[i].points += points;
				break;
			}
		}		
	}

	inline void SetFreeTaskComplete(unsigned int uTaskid)
	{
		for( unsigned int i = 0; i < 10; i++)
		{
			if(_lua_role._event_task_complete[i].task_id == uTaskid)
			{
				_lua_role._event_task_complete[i].task_id = 0;
				_lua_role._event_task_complete[i].points = 0;
				break;
			}
		}		
	}

	inline unsigned int GetEventTaskCompletePoints(unsigned int uTaskid)
	{
		for (unsigned int i = 0; i < 10; i++)
		{
			if (uTaskid == _lua_role._event_task_complete[i].task_id) return _lua_role._event_task_complete[i].points;
		}	

	}
	
	inline LUA_ROLE * GetRole() { return &_lua_role; }
	inline LUA_USER * GetUser() { return &_lua_user; }	


	// Imperial
	inline void SetImperialEnabled(int value) { _lua_role._imperial_enabled = value; }
	inline int GetImperialEnabled() { return _lua_role._imperial_enabled; }

	inline void SetImperialExpireTime(int value) { _lua_role._imperial_expire_time = value; }
	inline int GetImperialExpireTime() { return _lua_role._imperial_expire_time; }
	
	/*176+*/
	inline void SetPortatilPictureStorage(int pos, int value) { _lua_role._portatil_picture_storage[pos] = value; }
	inline int GetPortatilPictureStorage(int pos) { return _lua_role._portatil_picture_storage[pos]; }

	inline void SetPortatilPictureActive(int value) { _lua_role._portatil_picture_active = value; }
	inline int GetPortatilPictureActive() { return _lua_role._portatil_picture_active; }

	inline ARMOR_NEW_SAVE_POS * GetNewArmorSpirit(int pos) { return &_lua_role._new_armor_spirit[pos]; }
	inline ARMOR_NEW_SAVE_POS * GetNewArmorCrystal(int pos) { return &_lua_role._new_armor_cristal[pos]; }

	inline void SetNewArmorSpirit(int pos, int armor_id, short armor_pos, short inject_pos)
	{
		_lua_role._new_armor_spirit[pos].armor_id = armor_id;
		_lua_role._new_armor_spirit[pos].armor_pos = armor_pos;
		_lua_role._new_armor_spirit[pos].inject_pos = inject_pos;
	}

	inline void SetNewArmorCrystal(int pos, int armor_id, short armor_pos, short inject_pos)
	{
		_lua_role._new_armor_cristal[pos].armor_id = armor_id;
		_lua_role._new_armor_cristal[pos].armor_pos = armor_pos;
		_lua_role._new_armor_cristal[pos].inject_pos = inject_pos;
	}		
	inline int GetResetTowerDay() { return _lua_role._reset_tower_day; }
	inline void SetResetTowerDay(int value) { _lua_role._reset_tower_day = value; }

	inline bool GetMergeZenith() { return _lua_role._merge_zenith; }
	inline void SetMergeZenith(bool b) { _lua_role._merge_zenith = b; }
	inline bool GetResetCelestialMemorial() { return _lua_role._reset_celestial_memorial; }
	inline void SetResetCelestialMemorial(bool b) { _lua_role._reset_celestial_memorial = b; }

	inline char GetChildResetDay() { return _lua_role._child_reset_day; }
	inline void SetChildResetDay(char value) { _lua_role._child_reset_day = value; }
	inline bool GetResetLottery() { return _lua_role._reset_lottery; }
	inline void SetResetLottery(bool b) { _lua_role._reset_lottery = b; }

	inline bool GetReset2Lottery() { _lua_role._merge_zenith; }
	inline void SetReset2Lottery(bool b) { _lua_role._merge_zenith = b; }
};







#endif