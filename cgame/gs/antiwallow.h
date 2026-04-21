#ifndef __ONLINE_GAME_GS_ANTI_WALLOW_H__
#define __ONLINE_GAME_GS_ANTI_WALLOW_H__

#include <stdio.h>

namespace anti_wallow
{

struct punitive_param
{
	bool active;
	float exp;
	float sp;
	float item;
	float money;
	float task_exp;
	float task_sp;
	float task_money;
};

enum 
{
	MAX_WALLOW_LEVEL = 4
};


const punitive_param & Get(unsigned int level);
void SetParam(unsigned int level, const punitive_param & param);

void AdjustNormalExpSP(unsigned int level, int & exp, int & sp);
void AdjustTaskExpSP(unsigned int level, int & exp, int & sp);
void AdjustNormalMoneyItem(unsigned int level, float & money, float & item);
void AdjustTaskMoney(unsigned int level, int & money);

extern punitive_param list[];

}
#endif

