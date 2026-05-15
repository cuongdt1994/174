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

class gplayer_imp;
class item_list;

class player_kid
{
public:
	enum
	{
		MAX_RANDOM_COURSE          = 5,
		MAX_EQUIPED_COURSE         = 6,
		MAX_STORAGE_COURSE         = 8,
		MAX_COURSE_INV             = MAX_EQUIPED_COURSE + MAX_STORAGE_COURSE,  // 14

		MAX_NAME_LENGTH            = 16,
		MAX_CELESTIAL              = 6,
		MAX_KID_STAR               = 6,
		MAX_RESISTANCE             = 5,
		MAX_MAGIC_DEF              = 5,
		MAX_SKILL                  = 32,
		MAX_REWARD_BIT             = 64,
		MAX_KID_LEVEL              = 105,

		REQUIRED_DAYS_FOR_AWAKENING = 7,     // 15
		ITEM_ID_REWARD             = 67725,

		DB_DATA_SIZE               = 520,    // 0x208

		// Config IDs
		IDX_KID_QUALITY_CONFIG     = 6874,   // 0x1ADA
		IDX_KID_STAR_CONFIG        = 6872,   // 0x1AD8

		// KidModify command types
		CMD_KID_BASIC              = 265,
		CMD_MOVE_COURSE            = 266,
		CMD_UP_COURSE              = 267,
		CMD_KID_EXTEND             = 268,
	};

	// ----------------------------------------------------------------
	//  course_ess � packed, size 0x05
	// ----------------------------------------------------------------
#pragma pack(push, 1)
	struct course_ess
	{
		int  _tid;                                  // 0x00
		char _lvl;                                  // 0x04
	};
#pragma pack(pop)

	// ----------------------------------------------------------------
	//  kid_ess � size 0x38 (56)
	// ----------------------------------------------------------------
	struct kid_ess
	{
		int _lvl;                                   // 0x00
		int _rahk_lvl;                              // 0x04
		int _debris_exp;                            // 0x08
		int _tid;                                   // 0x0C
		int _physic_damage;                         // 0x10
		int _magic_damage;                          // 0x14
		int _defence;                               // 0x18
		int _magic_defences[MAX_MAGIC_DEF];         // 0x1C
		int _HP;                                    // 0x30
		int _crit;                                  // 0x34
	};                                              // 0x38

	// ----------------------------------------------------------------
	//  celestial_info - view of first 4 fields of kid_ess (same layout)
	// ----------------------------------------------------------------
	struct celestial_info
	{
		int level;   // overlays kid_ess._lvl
		int rank;    // overlays kid_ess._rahk_lvl
		int exp;     // overlays kid_ess._debris_exp
		int idx;     // overlays kid_ess._tid
	};

	// ----------------------------------------------------------------
	//  kid_data� packed, size 0x80 (128)
	// ----------------------------------------------------------------
#pragma pack(push, 1)
	struct kid_data
	{
		int        _course_new[MAX_RANDOM_COURSE];  // 0x00
		char       _pool_lvl;                       // 0x14
		char       _free_times;                     // 0x15
		int        _pool_exp;                       // 0x16
		course_ess _course_inv[MAX_COURSE_INV];     // 0x1A
		char       _name[MAX_NAME_LENGTH];          // 0x60
		char       _gender;                         // 0x70
		int        _exp;                            // 0x71
		int        _growth_days;                    // 0x75
		char       _new_day;                        // 0x79
		int        _energy;                         // 0x7A
		char       _type;                           // 0x7E
		char       _status;                         // 0x7F
	};                                              // 0x80
#pragma pack(pop)

	// ----------------------------------------------------------------
	//  KidModify protocol structs
	// ----------------------------------------------------------------
	struct KidModify
	{
#pragma pack(push, 2)
		struct mma                                  // CMD_KID_EXTEND (268), size 0x12
		{
			unsigned short cmd;
			int            type;
			int            num;
			int            arg_1;
			int            arg_2;
		};
#pragma pack(pop)

#pragma pack(push, 1)
		struct mma_0                                // CMD_UP_COURSE (267), size 0x05
		{
			unsigned short cmd;
			char           type_1;
			char           type_2;
			char           type_3;
		};
#pragma pack(pop)

		struct mma_1                                // CMD_KID_BASIC (265) / CMD_MOVE_COURSE (266), size 0x04
		{
			unsigned short cmd;
			char           type_1;
			char           type_2;
		};
	};

private:
	// ================================================================
	//  Member data � total 0x2F0 (752) bytes
	// ================================================================
	gplayer_imp* const _owner;                      // 0x000

	// --- DB-persisted block (DB_DATA_SIZE = 520 bytes) ---
	kid_data   _kid_data;                           // 0x004
	int        _select;                             // 0x084
	kid_ess    _kid_ess[MAX_CELESTIAL];             // 0x088
	int        _update_time;                        // 0x1D8
	int64_t    _addon_mask[MAX_CELESTIAL];          // 0x1DC
	// --- end DB block (0x20C) ---

	int        _max_data;                           // 0x20C
	int        _lock_data_map;                      // 0x210

	struct                                          // 0x214 � 220 bytes
	{
		int   shape;
		int   attack_type;
		int   hp;
		int   damage_low;
		int   damage_high;
		int   damage_magic_low;
		int   damage_magic_high;
		int   defence;
		int   resistance[MAX_RESISTANCE];
		int   crit_hit;
		int   attack_speed;
		float attack_range;
		float speed;
		int   attack_degree;
		int   defend_degree;
		int   phy_inherit;
		int   mag_inherit;
		int   time_reduce;
		int   skill_count;
		int   skill[MAX_SKILL];
	} tmp_data;

	// ================================================================
	//  Static data tables
	// ================================================================

	// Pool level exp
	static const int exp_required_next_level[10];
	static const int exp_min_level[10];

	// Course card pools per quality tier
	static const int card_level_1[6];
	static const int card_level_2[8];
	static const int card_level_3[7];
	static const int card_level_4[7];
	static const int card_level_5[1];
	static const int* card_pools[MAX_RANDOM_COURSE];
	static const int  card_counts[MAX_RANDOM_COURSE];

	// Suite combo (EndTeach)
	static const int          suite_idx_course[12];
	static const int          suite_idx_mask[12];

	// Kid reward config IDs per celestial slot
	static const unsigned int KID_REWARD_ID[MAX_CELESTIAL];

public:
	// ================================================================
	//  Lifecycle
	// ================================================================
	player_kid(gplayer_imp* p)
		: _owner(p)
		, _select(-1)
		, _lock_data_map(0)
		, _max_data(0)
	{
		memset(&_kid_data, 0, DB_DATA_SIZE);
		memset(&tmp_data, 0, sizeof(tmp_data));
	}

	~player_kid() {}

	// ================================================================
	//  Serialization
	// ================================================================
	bool        Save(archive& ar);
	bool        Load(archive& ar);
	void        Swap(player_kid& rhs);
	const void* SaveToDB(size_t& size);
	void        InitFromDB(const void* buf, size_t size);

	// ================================================================
	//  Celestial accessors (used by player_kid_addons)
	// ================================================================
	inline celestial_info* GetCelestial(int pos)
	{
		if ((unsigned int)pos >= MAX_CELESTIAL) return nullptr;
		return reinterpret_cast<celestial_info*>(&_kid_ess[pos]);
	}
	inline void SetCelestial(int pos, int level, int rank, int exp, int idx)
	{
		if ((unsigned int)pos >= MAX_CELESTIAL) return;
		_kid_ess[pos]._lvl         = level;
		_kid_ess[pos]._rahk_lvl    = rank;
		_kid_ess[pos]._debris_exp  = exp;
		_kid_ess[pos]._tid         = idx;
		UpdateKid(pos);
	}

	// ================================================================
	//  Core gameplay
	// ================================================================
	void        Heartbeat(int cur_time);
	bool        CreateKid(const void* buf);
	bool        StartDay();
	bool        OnCreateKid();

	// ================================================================
	//  Course (pool / shop / inventory)
	// ================================================================
	bool        UpPool();
	bool        RePool();
	char        BuyCourse(int num);
	bool        SellCourse(int num);
	bool        MoveCourse(int src_num, int dst_num);
	bool        UpCourse(int num1, int num2, int num3);
	int         EndTeach();

	// ================================================================
	//  Kid evolution / debris
	// ================================================================
	void        UpdateKid(int num);
	bool        UpKidLvl(size_t num, int count);
	bool        UseDebri(size_t num, int where, size_t inv_index);
	void        AddDebri(size_t num, size_t& count);

	// ================================================================
	//  Addon / reward
	// ================================================================
	bool        ActivateReward(size_t num2, size_t num);
	void        ActivateAllAddon();
	void        Activate(int addon_id);
	void        Deactivate(int addon_id);

	// ================================================================
	//  Transform
	// ================================================================
	void        ActivateTransform();
	void        DeactivateTransform();

	// ================================================================
	//  Client protocol
	// ================================================================
	void        ClientSync(int type);
	bool        KidModify(int cmd_type, const void* buf, size_t size);

	// ================================================================
	//  Debug
	// ================================================================
	void        KidDeubug(int cmd_type);

private:
	int         NEXT_DAY_TIME();
};

// ====================================================================
//  Compile-time size verification
// ====================================================================
static_assert(sizeof(player_kid::course_ess)       == 0x05,  "course_ess size mismatch");
static_assert(sizeof(player_kid::kid_ess)          == 0x38,  "kid_ess size mismatch");
static_assert(sizeof(player_kid::kid_data)         == 0x80,  "kid_data size mismatch");
static_assert(sizeof(player_kid::KidModify::mma)   == 0x12,  "KidModify::mma size mismatch");
static_assert(sizeof(player_kid::KidModify::mma_0) == 0x05,  "KidModify::mma_0 size mismatch");
static_assert(sizeof(player_kid::KidModify::mma_1) == 0x04,  "KidModify::mma_1 size mismatch");
static_assert(sizeof(player_kid)                   == 0x2F0, "player_kid size mismatch");

#endif // __ONLINEGAME_GS_PLAYER_KID_H__