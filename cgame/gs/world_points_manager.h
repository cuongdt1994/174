#ifndef __GNET_WORLD_POINTS_MANAGER_H
#define __GNET_WORLD_POINTS_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class WorldPointsConfig
{
public:
	enum
	{
		GROUP_IDX0 = 7235,	
		WORLD_POINT_LIST_COUNT = 20,
		REWARD_MAX_COUNT = 10,
	};
	static WorldPointsConfig * instance;

private:

	struct LIST
	{
		float probability;
		int point_award;
		int require_task_count;
		int money_award;
		int realm_exp_award;
		int exp_award;
		int sp_award;
		int dividend_award;		
	};

	struct REWARD
	{
		int require_points;
		int item_award_id;
		int item_award_count;		
	};

private:

	int max_points;
	LIST list[WORLD_POINT_LIST_COUNT];
	REWARD award[REWARD_MAX_COUNT];
	
public:
	void Init();

	int GetMaxPoints() { return max_points; }
	LIST * GetList(int pos) { return &list[pos]; }
	REWARD * GetReward(int pos) { return &award[pos]; }
	inline int GetCountValidList()
	{ 
		int res = 0;
		for (int i = 0; i < WORLD_POINT_LIST_COUNT; i++)
		{
			if (list[i].probability > 0)
			{
				res++;
			}
		}		
		return res; 
	}
	
	
WorldPointsConfig()
{

}

~WorldPointsConfig()
{
	
}

static WorldPointsConfig * GetInstance()
{
	if (!instance)
	{
		instance = new WorldPointsConfig();
	}
	return instance;
}

};


#endif