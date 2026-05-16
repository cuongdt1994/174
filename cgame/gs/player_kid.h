#ifndef __ONLINEGAME_GS_PLAYER_KID_H__
#define __ONLINEGAME_GS_PLAYER_KID_H__

#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>
#include <cstring>

#include "luamanager.h"
#include "item/item_addon.h"
#include "vector.h"

class gplayer_kid 
{
public:
	enum
	{
		MAX_RANDOM_COURSE = 5,
		MAX_EQUIPED_COURSE = 6,
		MAX_STORAGE_COURSE = 8,
		MAX_NAME_LENGTH = 16,
		REQUIRED_DAYS_FOR_AWAKENING = 7, //15
		ITEM_ID_REWARD = 67725,
		MAX_CELESTIAL = 6,
		MAX_KID_STAR = 6,
		IDX_KID_QUALITY_CONFIG = 6874,
		IDX_KID_STAR_CONFIG = 6872,
		MAX_KID_LEVEL = 105,
	};

public:

	struct AWAKENING_COURSE_INFO
	{
		int course_id;
		char course_level; // 1 ~ 3
	};	

	struct AWAKENING_COURSE_INFO_2
	{
		int course_id;
		char course_level; // 1 ~ 3
	};

	struct KID_STRUCT
	{
		int level;
		int rank;
		int exp;
		int idx;		
	};

	struct KID_ACTIVITY
	{
		int active_slot;
		int reserved;
	};

public:
	char is_awakening;
	char block_day;
	char type;
	int awakening_random_course[MAX_RANDOM_COURSE];
	AWAKENING_COURSE_INFO awakening_equiped_course[MAX_EQUIPED_COURSE];
	AWAKENING_COURSE_INFO_2 awakening_storage_course[MAX_STORAGE_COURSE];
	short awakening_name_length;
	char awakening_name[MAX_NAME_LENGTH];
	char card_level;
	int points_awakening;
	int awakening_day_count;
	int awakening_cash;
	int awakening_potential;
	char course_random_cost; 
	int exp_course_required;
	char check_day;

	KID_STRUCT celestial[MAX_CELESTIAL];
	KID_ACTIVITY activity;

public:
	gplayer_kid() 
	{ 
		memset(this, 0x00, sizeof(*this)); 
	}
	~gplayer_kid() {}

	inline void Init()
	{
	}

	inline void Clear()
	{
		memset(this, 0x00, sizeof(*this));
	}

	inline void ClearAwakening()
	{
		is_awakening = false;
		block_day = false;
		type = -1;
		memset(awakening_random_course, 0x00, sizeof(awakening_random_course));
		memset(awakening_equiped_course, 0x00, sizeof(awakening_equiped_course));
		memset(awakening_storage_course, 0x00, sizeof(awakening_storage_course));
		memset(awakening_name, 0x00, sizeof(awakening_name));
		awakening_name_length = 0;
		card_level = 0;
		points_awakening = 0;
		awakening_day_count = 0;
		awakening_cash = 0;
		awakening_potential = 0;
		course_random_cost = 0;
		exp_course_required = 0;
		check_day = false;
	}

	inline char IsAwakening() { return is_awakening; }
	inline void SetAwakening(char b) { is_awakening = b; }

	inline char IsBlockDay() { return block_day; }
	inline void SetBlockDay(char b) { block_day = b; }

	inline char GetType() { return type; }
	inline void SetType(char t) { type = t; }

	inline int GetRandomCourse(int idx) { return awakening_random_course[idx]; }
	inline void SetRandomCourse(int idx, int course_id) { awakening_random_course[idx] = course_id; }

	inline AWAKENING_COURSE_INFO* GetEquipedCourse(int idx) { return &awakening_equiped_course[idx]; }
	inline void SetEquipedCourse(int idx, int course_id, char course_level)
	{
		awakening_equiped_course[idx].course_id = course_id;
		awakening_equiped_course[idx].course_level = course_level;
	}

	inline AWAKENING_COURSE_INFO_2* GetStorageCourse(int idx) { return &awakening_storage_course[idx]; }
	inline void SetStorageCourse(int idx, int course_id, char course_level)
	{
		awakening_storage_course[idx].course_id = course_id;
		awakening_storage_course[idx].course_level = course_level;
	}

	inline short GetNameLength() { return awakening_name_length; }
	inline void SetNameLength(short len) { awakening_name_length = len; }

	inline char* GetName() { return awakening_name; }
	inline void SetName(const char* name) { memcpy(awakening_name, name, awakening_name_length); }

	inline char GetCardLevel() { return card_level; }
	inline void SetCardLevel(char level) { card_level = level; }

	inline int GetPointsAwakening() { return points_awakening; }
	inline void SetPointsAwakening(int points) { points_awakening = points; }

	inline int GetAwakeningDayCount() { return awakening_day_count; }
	inline void SetAwakeningDayCount(int count) { awakening_day_count = count; }

	inline int GetAwakeningCash() { return awakening_cash; }
	inline void SetAwakeningCash(int cash) { awakening_cash = cash; }

	inline int GetAwakeningPotential() { return awakening_potential; }
	inline void SetAwakeningPotential(int potential) { awakening_potential = potential; }

	inline char GetCourseRandomCost() { return course_random_cost; }
	inline void SetCourseRandomCost(char cost) { course_random_cost = cost; }

	inline int GetExpCourseRequired() { return exp_course_required; }
	inline void SetExpCourseRequired(int required) { exp_course_required = required; }

	inline char GetCheckDay() { return check_day; }
	inline void SetCheckDay(char day) { check_day = day; }

	inline KID_STRUCT* GetCelestial(int pos) { return &celestial[pos]; }
	inline void SetCelestial(int pos, int level, int rank, unsigned int exp, unsigned int idx)
	{
		celestial[pos].level = level;
		celestial[pos].rank = rank;
		celestial[pos].exp = exp;
		celestial[pos].idx = idx;
	}

	inline KID_ACTIVITY* GetActivity() { return &activity; }
	inline void SetActivity(int active_slot, int reserved)
	{
		activity.active_slot = active_slot;
		activity.reserved = reserved;
	}
	
public:

};

#endif
