#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arandomgen.h>
#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"

#include "playerdailysign.h"
#include <bitset>

tm player_dailysign::dbgtime;

int GetMonthDiff(int year ,int mon)
{
	static int leapyear[MAX_SIGN_MONTH] = {0,-2,0,-1,0,-1,0,0,-1,0,-1,0};
	static int normalyear[MAX_SIGN_MONTH] = {0,-3,0,-1,0,-1,0,0,-1,0,-1,0};

	return (year%4 == 0) && (year%100 != 0 || year%400 == 0) ? 
		leapyear[mon] : normalyear[mon];
}

void player_dailysign::SaveToDB(int& uptime, int& monthcal, int& curryear, int& lastyear, char& latesignintimes, char& awardedtimes, char& awardedtimes2)
{
	uptime = _update_time;
	monthcal = _month_calendar;
	curryear = _curr_year;
	lastyear = _last_year;
	latesignintimes = _late_signin_times;
    awardedtimes = _awarded_times;
	awardedtimes2 = _awarded_times2;
}

void player_dailysign::InitFromDB(int uptime, int monthcal, int curryear, int lastyear, char latesignintimes, char awardedtimes, char awardedtimes2)
{
	_update_time = uptime;
	_month_calendar = monthcal;
	_curr_year = curryear;
	_last_year = lastyear;
    _late_signin_times = latesignintimes;
    _awarded_times = awardedtimes;
	_awarded_times2 = awardedtimes2;
	//CheckPoint();
	CheckPoint2();
}

void player_dailysign::ClientSync(char type)
{
	_owner->_runner->refresh_signin(type,_last_year,_month_calendar,_update_time, _late_signin_times, _awarded_times, _awarded_times2 );
}

void player_dailysign::PurchaseItem(int type, int num, int expire)
{
	const item_data * pItem = (const item_data*)world_manager::GetDataMan().get_item_for_sell(type);
	if(!pItem) return;

	item_data * pItem2 = DupeItem(*pItem);
	
	int guid1 = 0;
	int guid2 = 0;   
	if(pItem2->guid.guid1 != 0) 
	{ 
		 get_item_guid(type, guid1,guid2); 
		 pItem2->guid.guid1 = guid1;
		 pItem2->guid.guid2 = guid2; 
	} 

//	pItem2->proc_type |= item::ITEM_PROC_TYPE_DAILY_SIGNIN; 
	item_list & inventory = _owner->GetInventory();

	int expire_date = expire ? g_timer.get_systime() + expire : 0; 

	int ocount = num;
	pItem2->count = ocount;
	pItem2->expire_date = expire_date;
	int rst = inventory.Push(*pItem2);
	ASSERT(rst >= 0 && pItem2->count == 0);
    inventory[rst].InitFromShop();

	_owner->_runner->obtain_item(pItem2->type,expire_date,ocount,inventory[rst].count, 0,rst);

	GLog::log(GLOG_INFO,"�û�%d��ǩ��ϵͳ�����%d��item%d",_owner->_parent->ID.id, num, type);
	
	FreeItem(pItem2);
}

bool player_dailysign::RequestAwards2(int type, int mon, int day)
{
    unsigned int award_id = 0;
    int award_num = 0;

	if (type & REQ_DAILY_AWARD)
	{
        DATA_TYPE dt;
        const SIGN_AWARD_CONFIG* signin = (const SIGN_AWARD_CONFIG*)world_manager::GetDataMan().get_data_ptr(AWARD_TEMPLATE_ID[mon], ID_SPACE_CONFIG, dt);
        if ((signin == NULL) || (dt != DT_SIGN_AWARD_CONFIG)) return false;
		
        int size = sizeof(signin->list) / sizeof(signin->list[0]);
        if ((day < 0) || (day >= size)) return false;

        award_id = signin->list[(int)day].id;
        award_num = signin->list[(int)day].num;
	}
	
	
	
	PurchaseItem(award_id, award_num);
	ClientSync(SYNC4AWARD);
	return true;
}

bool player_dailysign::DaySignIn2(int late_type)
{
	CheckPoint2();

	bool res = false;
	struct tm tm1,tm2;
	time_t upt = _update_time;
	time_t now = MK_LOCAL_TIME();
	localtime_r(&now, &tm1);
	localtime_r(&upt, &tm2);
	
	unsigned int count = 31; // + GetMonthDiff(tm1.tm_year+1900,tm1.tm_mon);

    switch (late_type)
    {
    case 0:
	{
		count += GetMonthDiff(tm1.tm_year+1900,tm1.tm_mon) - 1;
		if ( _awarded_times < count )
		{
			if (_awarded_times <= tm1.tm_mday && tm1.tm_mday > tm2.tm_mday && !_late_signin_times)
			{
				_update_time = MK_LOCAL_TIME();
				_late_signin_times = 1;
				res = RequestAwards2(REQ_DAILY_AWARD, tm1.tm_mon, _awarded_times++ );
			}
		}
		else
		{
			if( !(_month_calendar & (1 << (tm1.tm_mon))) )
			{
				_month_calendar |= (1 << (tm1.tm_mon));
				PurchaseItem(REQ_MONTH_AWARD, 1);
				res = RequestAwards2(REQ_DAILY_AWARD, tm1.tm_mon, _awarded_times++ );
			}
		}
		break;
	}
	
    case 1:
	{
		count += GetMonthDiff(tm1.tm_year+1900,tm1.tm_mon) - 1;
		if ( _awarded_times < count )
		{
			if (_awarded_times <= tm1.tm_mday)
			res = RequestAwards2(REQ_DAILY_AWARD, tm1.tm_mon, _awarded_times++ );
		}
		else
		{
			if( !(_month_calendar & (1 << (tm1.tm_mon))) )
			{
				_month_calendar |= (1 << (tm1.tm_mon));
				PurchaseItem(REQ_MONTH_AWARD, 1);
				res = RequestAwards2(REQ_DAILY_AWARD, tm1.tm_mon, _awarded_times++ );
			}
		}
		break;
	}
	
	case 2:
	{
		count += GetMonthDiff(tm1.tm_year+1900,(tm1.tm_mon-1)) -1;
		if ( _awarded_times2 < count )
		{
			res = RequestAwards2(REQ_DAILY_AWARD, (tm1.tm_mon-1), _awarded_times2++ );
		}
		else
		{
			if( !(_month_calendar & (1 << (tm1.tm_mon-1))) )
			{
				_month_calendar |= (1 << (tm1.tm_mon-1));
				PurchaseItem(REQ_MONTH_AWARD, 1);
				res = RequestAwards2(REQ_DAILY_AWARD, (tm1.tm_mon-1), _awarded_times2++ );
			}
		}
		break;
	}
	
    default:
		return false;
		break;
    }
	
	if (res)
	{
		int exp = (_owner->_basic.level + 1)*_owner->_basic.level*10;
		int sp  = (int)(exp * 0.226f); 
		_owner->ReceiveCommonExp(exp, sp);	
		ClientSync( !late_type ? SYNC4DAILY : SYNC4LATE);
	}
	return res;
}

void player_dailysign::CheckPoint2()
{
	if(_update_time == 0)
	{
		_update_time = (time(NULL) - 86400);
	}

	bool res = false;
	struct tm now,upd;
	time_t upt_t = _update_time;
	time_t now_t = MK_LOCAL_TIME();
	localtime_r(&now_t, &now);
	localtime_r(&upt_t, &upd);
	
	if (_late_signin_times && upd.tm_mday < now.tm_mday)
	{
		_late_signin_times = 0;
	}
	
	if ( upd.tm_mon != now.tm_mon )
	{
		_awarded_times2 = _awarded_times;
		_awarded_times = 0;
	}
	
	if(upd.tm_year != now.tm_year)
	{
		if( _month_calendar == 0xFFF )
		{
			_awarded_times = 0;
			_awarded_times2 = 0;
			_month_calendar = 0;
			
			PurchaseItem(REQ_YEAR_AWARD, 1);
		}
	}

	//printf("CheckPoint2: upd.tm_mday = %d , now.tm_mday = %d , _late_signin_times = %d \n", upd.tm_mday, now.tm_mday, _late_signin_times );

	return;
}