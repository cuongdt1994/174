#ifndef __GNET_CODEX_MANAGER_H
#define __GNET_CODEX_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class CodexConfig
{
public:
	enum
	{
		// Recompensa Ano do Tigre (Roupa)
		COUNT_TYPE_FASHION = 2,
		IDX_REWARD_SINGLE_FASHION_BEGIN = 6733,

		// Recompensa Ano do Tigre (Voo)
		COUNT_TYPE_FLY = 6,
		IDX_REWARD_SINGLE_FLY_BEGIN = 6865,

		// Recompensa Ano do Tigre (Montaria)
		COUNT_TYPE_MOUNT = 1,
		IDX_REWARD_SINGLE_MOUNT_BEGIN = 6864,

		// Recompensa de Títulos
		COUNT_MAX_TITLE_REWARD = 128,
		COUNT_MAX_TYPE = 4,
		IDX_REWARD_TITLE_BEGIN = 6854,

	};
	static CodexConfig * instance;

private:

	struct REWARD_SINGLE
	{
		int	award_item_count;
		int	award_require_count;
		unsigned int award_item_id;		
	};

	struct REWARD_TITLE
	{
		int	type;
		int	step;
		unsigned int award_item_id[COUNT_MAX_TITLE_REWARD];		
	};


private:

	// Recompensa Ano do Tigre
	REWARD_SINGLE reward_fashion[COUNT_TYPE_FASHION];
	REWARD_SINGLE reward_fly[COUNT_TYPE_FLY];
	REWARD_SINGLE reward_mount[COUNT_TYPE_MOUNT];

	// Recompensa de Títulos
	REWARD_TITLE reward_title[COUNT_MAX_TYPE];
	
public:
	void Init();

	// Recompensa Ano do Tigre
	inline REWARD_SINGLE * GetFashionReward(int pos) { return &reward_fashion[pos]; }
	inline REWARD_SINGLE * GetFlyReward(int pos) { return &reward_fly[pos]; }
	inline REWARD_SINGLE * GetMountReward(int pos) { return &reward_mount[pos]; }

	// Recompensa de Títulos
	inline REWARD_TITLE * GetTitleReward(int pos) { return &reward_title[pos]; }
	
	
CodexConfig()
{

}

~CodexConfig()
{
	
}

static CodexConfig * GetInstance()
{
	if (!instance)
	{
		instance = new CodexConfig();
	}
	return instance;
}

};


#endif