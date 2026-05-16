#ifndef __ONLINEGAME_GS_PLAYER_KID_ADDONS_H__
#define __ONLINEGAME_GS_PLAYER_KID_ADDONS_H__

#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>
#include <cstring>

#include "luamanager.h"
#include "item/item_addon.h"
#include "vector.h"

class gplayer_kid_addons
{
public:
	enum
	{
		IDX_MAX_LEVEL_MONEY_COST = 6875,
		IDX_MAX_LEVEL_CHECK = 6877,
	};

public:
	struct KID_ADDON
	{
		int pos;
		int addons_count;
		unsigned int addons_pos[8];
	};	

public:
	unsigned int addons_count;
	KID_ADDON addons[6];

	int kids_wipe;
	int kids_fix_max_level;
	unsigned int team_new_created;

	int lottery_reset[24];

	unsigned int time_end;
	int awards_pos;
	unsigned int cash_history_used;

	int codex_reset[24];

public:
	gplayer_kid_addons() {}
	~gplayer_kid_addons() {}

	inline void Init()
	{
		Clear();
	}
	inline void Clear()
	{
		addons_count = 0;
		for (int i = 0; i < 6; i++)
		{
			addons[i].pos = -1;
			addons[i].addons_count = 0;
			memset(addons[i].addons_pos, 0, sizeof(addons[i].addons_pos));			
		}

		kids_wipe = 0;
		kids_fix_max_level = 0;
		team_new_created = 0;
		memset(lottery_reset, 0, sizeof(lottery_reset));

		time_end = 0;
		awards_pos = 0;
		cash_history_used = 0;

		memset(codex_reset, 0, sizeof(codex_reset));
	}
	
public:
	void GenerateKidsAddons(int roleid);
	void ActivateKidsAddons(int roleid);
	void DeactivateKidsAddons(int roleid);
	void UpdateKidsAddonsProtocol(int roleid);
	void SetCelestialNewLevel(int roleid, int pos, int level);
	void SetRecvKidsAddons(int roleid, int pos, int addon_pos);

	void SetKidsWipe(int roleid);	

	inline void SetNewTimeCreated(unsigned int time) { team_new_created = time; }	
	inline int GetNewTimeCreated() { return team_new_created; }

	inline bool GetLotteryReset(int pos)
	{
		if (pos < 0 || pos >= 24)
			return false;

		return lottery_reset[pos] == 1;
	}
	inline void SetLotteryReset(int pos)
	{
		if (pos < 0 || pos >= 24)
			return;

		lottery_reset[pos] = 1;
	}

	inline void SetTimeEnd(unsigned int time) { time_end = time; }
	inline unsigned int GetTimeEnd() { return time_end; }

	inline void SetAwardsPos(int pos) { awards_pos = pos; }
	inline int GetAwardsPos() { return awards_pos; }

	inline void SetCashHistoryUsed(unsigned int used) { cash_history_used += used; }
	inline unsigned int GetCashHistoryUsed() { return cash_history_used; }

	inline void SetCodexReset(int pos)
	{
		if (pos < 0 || pos >= 24)
			return;

		codex_reset[pos] = 1;
	}

	inline bool GetCodexReset(int pos)
	{
		if (pos < 0 || pos >= 24)
			return false;

		return codex_reset[pos] == 1;
	}

};

#endif
