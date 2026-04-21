#include "antiwallow.h"
#include "emulate_settings.h"

namespace anti_wallow
{



punitive_param list[MAX_WALLOW_LEVEL] = {{0}};



const punitive_param & GetParam(unsigned int level)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	return list[level];
}

void SetParam(unsigned int level, const punitive_param & param)
{
	if(level >= MAX_WALLOW_LEVEL)
	{
		level = MAX_WALLOW_LEVEL - 1;
	}
	list[level] = param;
	list[level].active = true;
}

void AdjustNormalExpSP(unsigned int level, int & exp, int & sp)
{
	const punitive_param & param = GetParam(level);

	int new_rate_exp = EmulateSettings::GetInstance()->GetRatesConfig()->mob_exp;
	int new_rate_sp = EmulateSettings::GetInstance()->GetRatesConfig()->mob_sp;

	exp = (int)(exp * new_rate_exp + 0.5f);
	sp = (int)(sp * new_rate_sp + 0.5f);
}

void AdjustTaskExpSP(unsigned int level, int & exp, int & sp)
{
	int new_rate_exp = EmulateSettings::GetInstance()->GetRatesConfig()->task_exp;
	int new_rate_sp = EmulateSettings::GetInstance()->GetRatesConfig()->task_sp;

	const punitive_param & param = GetParam(level);
	exp = (int)(exp * new_rate_exp + 0.5f);
	sp = (int)(sp * new_rate_sp + 0.5f);
}

void AdjustNormalMoneyItem(unsigned int level, float & money, float & item)
{
	int new_rate_silver = EmulateSettings::GetInstance()->GetRatesConfig()->mob_money;
	int new_rate_drop = EmulateSettings::GetInstance()->GetRatesConfig()->mob_drop;

	const punitive_param & param = GetParam(level);
	money *= new_rate_silver;
	item *= new_rate_drop;
}

void AdjustTaskMoney(unsigned int level, int & money)
{
	int new_rate_silver = EmulateSettings::GetInstance()->GetRatesConfig()->task_money;

	const punitive_param & param = GetParam(level);
	money = (int)(money * new_rate_silver + 0.5f);
}

}

