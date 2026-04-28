#ifndef __ONLINEGAME_GS_PLAYER_IMP_H__
#define __ONLINEGAME_GS_PLAYER_IMP_H__
#include <uchar.h>
#include "player.h"
#include "actobject.h"
#include "item_list.h"
#include "playerteam.h"
#include "playerinvade.h"
#include "playertrashbox.h"
#include <db_if.h>
#include <gsp_if.h>
#include "playerstall.h"
#include "cooldown.h"
#include "breath_ctrl.h"
#include "petman.h"
#include "playerduel.h"
#include "playerbind.h"
#include "wallow/wallowobj.h"
#include "playermall.h"
#include "vector.h"
#include "item/item_elf.h" //lgc
#include "playermisc.h"
#include "multi_exp_ctrl.h"
#include "playerlimit.h"
#include "playerforce.h"
#include "online_award.h"
#include "meridianmanager.h"
#include "touchtrade.h"
#include "playertitle.h"
#include "playerdailysign.h"
#include "player_reincarnation.h"
#include "playerfatering.h"
#include "playergiftcard.h"
#include "playerclock.h"
#include "playersanctuarycheck.h"
#include "playerinstancereenter.h"
#include "playersolochallenge.h"
#include "world_points_manager.h"
#include "activity_event_manager.h"
#include "celestial_memorial_manager.h"
//#include "question_manager.h"

#include "luamanager.h"
#include "player_lua.h"
#include "player_lottery.h"
#include "player_money.h"
#include "player_lib.h"
#include "player_sequrity.h"
#include "player_celestial.h"
#include "player_codex.h"
#include "player_kid.h"
#include "player_kid_addons.h"

#include "glyph_manager.h"
#include "arenamanager.h"
#include "kid_manager.h"

struct MSG;
struct attack_msg;
struct enchant_msg;
struct pet_leader_prop;
//lgc
struct elf_skill_data;
struct elf_info   //������player_imp�б��浱ǰװ����С������Ϣ
{
	unsigned int id;//��ЧidΪ(unsigned int)-1

	bool refresh_vigor;//vigor����
	bool refresh_enhance;//buff����
	
	bool refine_effect_active;  //����Ч���Ƿ񼤻�
	short refine_level;
	
	short strength;	//С����ı��������ԣ���elf_item��prop��cur_strength��ͬ������ʹ��
	short agility;
	short vitality;
	short energy;

	short final_strength;//����buf��С��������
	short final_agility;
	short final_vitality;
	short final_energy;

	float vigor;		//��ǰԪ��
	float max_vigor;	//Ԫ������
	float vigor_gen;	//Ԫ���ظ�
	
	abase::vector<struct elf_skill_data, abase::fast_alloc<> > skillvect;	//��ǰ���ü���

	elf_info(): id((unsigned int)-1), refresh_vigor(false), refresh_enhance(false), refine_effect_active(false), refine_level(0), strength(0), agility(0), vitality(0), energy(0), final_strength(0), final_agility(0), final_vitality(0), final_energy(0), vigor(0.0f), max_vigor(0.0f), vigor_gen(0.0f){}
	
	void clear()//�����refine_effect_active,vigor
	{
		id = (unsigned int)-1; 
		refresh_vigor = false; refresh_enhance = false; 
		refine_level = 0; 
		strength = 0; agility = 0; vitality = 0; energy = 0; 
		final_strength = 0; final_agility = 0; final_vitality = 0; final_energy = 0;
		max_vigor = 0.0f; vigor_gen = 0.0f;
		skillvect.clear();
	}
	
	template <typename WRAPPER>
	WRAPPER & operator>>(WRAPPER & wrapper) const
	{
		wrapper << id << refresh_vigor << refresh_enhance << refine_effect_active << refine_level << strength << agility << vitality << energy << final_strength << final_agility << final_vitality << final_energy << vigor<< max_vigor << vigor_gen;
		unsigned int size = skillvect.size();
		wrapper << size;
		for(unsigned int i=0; i<size; i++)
			wrapper << skillvect[i];
		return wrapper;
	}

	template <typename WRAPPER>
	WRAPPER & operator<< (WRAPPER & wrapper)
	{
		wrapper >> id >> refresh_vigor >> refresh_enhance >> refine_effect_active >> refine_level >> strength >> agility >> vitality >> energy >> final_strength >> final_agility >> final_vitality >> final_energy >> vigor >> max_vigor >> vigor_gen;
		unsigned int size = -1;
		wrapper >> size;
		if(size < 0 || size > MAX_ELF_SKILL_CNT)
			throw -1;
		struct elf_skill_data sk;
		for(unsigned int i=0; i<size; i++)
		{
			wrapper >> sk;
			skillvect.push_back(sk);
		}
		return wrapper;
	}
};

struct random_mall_info
{
	enum	// random mall cmd
	{
		RAND_MALL_OPT_QUERY,
		RAND_MALL_OPT_ROLL,
		RAND_MALL_OPT_PAY,
	};
	enum
	{
		RM_ENTRY_ERR = -1,
		RM_ROLE_STATE_ERR = -2,
		RM_OPT_STATE_ERR = -3,
		RM_CASH_ERR = -4,
		RM_INV_ERR = -5,
	};
	static const int RAND_MALL_LEVEL_LIMIT = 20;

	struct random_mall_result
	{
		int itemid;
		int price;
		int paytime;
		int reserve1;
		int reserve2;
		random_mall_result(int id = 0,int pr = 0,int pt = 0,int r1 = 0,int r2 = 0) 
			: itemid(id),price(pr),paytime(pt),reserve1(r1),reserve2(r2) {}
	};
	typedef	abase::static_multimap<int,random_mall_result, abase::fast_alloc<> > RAND_MALL_RES_MAP;
	RAND_MALL_RES_MAP	_result_datas;
	
	template <typename WRAPPER>
	bool Save(WRAPPER & wrapper) const
	{
		unsigned int size = _result_datas.size();	
		wrapper << size;

		RAND_MALL_RES_MAP::const_iterator iter = _result_datas.begin();
		RAND_MALL_RES_MAP::const_iterator iend = _result_datas.end();
		for(; iter != iend; ++iter)
		{
			wrapper << iter->first;
			wrapper << iter->second.itemid;
			wrapper << iter->second.price;
			wrapper << iter->second.paytime;
			wrapper << iter->second.reserve1;
			wrapper << iter->second.reserve2;
		}

		return true;
	}

	template <typename WRAPPER>
	bool Load(WRAPPER & wrapper)
	{
		unsigned int size ;
		wrapper >> size;	
		if(0 == size) return true;

		for(unsigned int i = 0; i < size; ++i)
		{
			int entryid,itemid,price,paytime,reserve1,reserve2;			
			wrapper >> entryid;
			wrapper >> itemid;
			wrapper >> price;
			wrapper >> paytime;
			wrapper >> reserve1;
			wrapper >> reserve2;
			_result_datas[entryid] = random_mall_result(itemid,price,paytime,reserve1,reserve2);
		}
		return true;
	}
	
	void Swap(random_mall_info & rhs)
	{
		_result_datas.swap(rhs._result_datas);			
	}
	
	bool InitFromDB(archive & ar,int roleid)
	{
		if(0 != ar.size())
		{
			try{
				unsigned int size ;
				ar >> size;				
				if(0 == size) return true;

				for(unsigned int i = 0; i < size; ++i)
				{
					int entryid,itemid,price,paytime,reserve1,reserve2;										
					ar >> entryid;
					ar >> itemid;
					ar >> price;
					ar >> paytime;
					ar >> reserve1;
					ar >> reserve2;
					_result_datas[entryid] = random_mall_result(itemid,price,paytime,reserve1,reserve2);
				}
			}
			catch(...)
			{
				_result_datas.clear();
				GLog::log(LOG_ERR,"roleid:%d unmarshal randmall info fail \n",roleid);
				return false;
			}
		}		
		return true;
	}
	bool SaveToDB(archive & ar) const
	{
		unsigned int size = _result_datas.size();	
		if(0 == size)
			return true;
		ar << size;

		RAND_MALL_RES_MAP::const_iterator iter = _result_datas.begin();
		RAND_MALL_RES_MAP::const_iterator iend = _result_datas.end();
		for(; iter != iend; ++iter)
		{
			ar << iter->first;
			ar << iter->second.itemid;
			ar << iter->second.price;
			ar << iter->second.paytime;
			ar << iter->second.reserve1;
			ar << iter->second.reserve2;
		}
		return true;
	}

	bool IsFirstPay(int eid) { return _result_datas.find(eid) == _result_datas.end();}
	void PendingPay(int eid,const random_mall_result& res) { _result_datas[eid] = res;}
	void ClosingPay(int eid) { _result_datas[eid].itemid = 0; ++_result_datas[eid].paytime;}
	bool QueryResult(int eid, random_mall_result& res) const
	{  
		RAND_MALL_RES_MAP::iterator iter = _result_datas.find(eid);
		if(iter != _result_datas.end())
		{
			res = iter->second;
			return true;
		}
		return false;
	}
};

#pragma pack(1)
struct stall_info
{
	int stallcard_id;
	int max_sell_slot;
	int max_buy_slot;
	int max_name_length;
	stall_info():stallcard_id(-1),max_sell_slot(PLAYER_MARKET_SELL_SLOT),max_buy_slot(PLAYER_MARKET_BUY_SLOT),max_name_length(PLAYER_MARKET_NAME_LEN){}
};
#pragma pack()

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const struct stall_info & info)
{
	wrapper.push_back(&info, sizeof(info));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, struct stall_info & info)
{
	wrapper.pop_back(&info, sizeof(info));
	return wrapper;
}

struct dividend_mall_info
{
	int referrer_userid;
	int dividend_add;	//�ܹ���ȡ�ĺ����ۼ�ֵ,��һ����deliveryд���ݿ�
	int dividend_used;	//�ܹ���ʹ�õĺ����ۼ�ֵ,��һ����gsд���ݿ�
	
	dividend_mall_info():referrer_userid(-1),dividend_add(0),dividend_used(0){}

	bool Save(archive & ar)
	{
		ar << referrer_userid << dividend_add << dividend_used;
		return true;	
	}
	bool Load(archive & ar)
	{
		ar >> referrer_userid >> dividend_add >> dividend_used;
		return true;	
	}

	void SetDividendMallInfo(int _referrer_userid,int _dividend_add,int _dividend_used)
	{
		referrer_userid = _referrer_userid;
		dividend_add = _dividend_add;	
		dividend_used = _dividend_used;
	}

	void GetDividendMallInfo(int & _referrer_userid,int & _dividend_add, int & _dividend_used)
	{
		_referrer_userid = referrer_userid;
		_dividend_add = dividend_add;	
		_dividend_used = dividend_used;
	}

	int GetDividend()
	{
		return dividend_add - dividend_used;	
	}

	void IncDividendAdd(int _dividend)
	{
		dividend_add += _dividend;
	}

	bool SpendDividend(int used)
	{
		int tmp = dividend_used + used;
		if(tmp <= dividend_used || tmp > dividend_add) return false;
		dividend_used = tmp;
		return true;
	}
	
	bool HasReferrer()
	{
		return referrer_userid > 0;	
	}
};

class gplayer_imp;
struct purchase_limit_info
{
	typedef std::map<int, int> PURCHASE_LIMIT_MAP;
	// item_id -> �������
	PURCHASE_LIMIT_MAP _day_items; //���޹���Ʒ
	PURCHASE_LIMIT_MAP _week_items; //���޹���Ʒ
	PURCHASE_LIMIT_MAP _month_items; //���޹���Ʒ ����mapͬ����db
	PURCHASE_LIMIT_MAP _year_items;  //���޹���Ʒ

	int _next_day_item_clear_timestamp;
	int _next_week_item_clear_timestamp;
	int _next_month_item_clear_timestamp; //��player_clock����ʱ��������һ������޹���Ʒ��ʱ���
	int _next_year_item_clear_timestamp;

	purchase_limit_info():_next_day_item_clear_timestamp(0),_next_week_item_clear_timestamp(0),_next_month_item_clear_timestamp(0),_next_year_item_clear_timestamp(0)
	{
		_day_items.clear();
		_week_items.clear();
		_month_items.clear();
		_year_items.clear();
	}

	int GetNextYearStamp()
	{
		struct tm tt;
		time_t cut_time;
		cut_time = time(NULL);
		localtime_r(&cut_time, &tt);
		tt.tm_hour = 0;
		tt.tm_min  = 0;
		tt.tm_sec  = 0;
		tt.tm_mday = 1;
		tt.tm_mon  = 0;
		tt.tm_year = tt.tm_year + 1;
		return mktime(&tt);
	}

	void SetDayItemClearTimeStamp(int next_timestamp, int now)
	{
		if(next_timestamp > 0 && now >= _next_day_item_clear_timestamp)
		{
			_next_day_item_clear_timestamp = next_timestamp;
			_day_items.clear();
		}
	}
	
	void SetWeekItemClearTimeStamp(int next_timestamp, int now)
	{
		if(next_timestamp > 0 && now >= _next_week_item_clear_timestamp)
		{
			_next_week_item_clear_timestamp = next_timestamp;
			_week_items.clear();
		}
	}

	void SetMonthItemClearTimeStamp(int next_timestamp, int now)
	{
		if(next_timestamp > 0 && now >= _next_month_item_clear_timestamp)
		{
			_next_month_item_clear_timestamp = next_timestamp;
			_month_items.clear();
		}
	}

	void SetYearItemClearTimeStamp(int next_timestamp, int now)
	{
		if(next_timestamp > 0 && now >= _next_year_item_clear_timestamp)
		{
			_next_year_item_clear_timestamp = next_timestamp;
			_year_items.clear();
		}
	}

	void ClearNextClearTimeStamp()
	{
		_next_day_item_clear_timestamp   = 0;
		_next_week_item_clear_timestamp  = 0;
		_next_month_item_clear_timestamp = 0;
		_next_year_item_clear_timestamp  = 0;
	}

	int GetPurchaseLimitItemCount()
	{
		return _day_items.size() + _week_items.size() + _month_items.size() + _year_items.size();
	}
	
	void SaveAllMap(packet_wrapper & ar)
	{
		SaveSendClineMap(ar, _day_items, CASH_VIP_SHOPPING_LIMIT_DAY);
		SaveSendClineMap(ar, _week_items, CASH_VIP_SHOPPING_LIMIT_WEEK);
		SaveSendClineMap(ar, _month_items, CASH_VIP_SHOPPING_LIMIT_MONTH);
		SaveSendClineMap(ar, _year_items, CASH_VIP_SHOPPING_LIMIT_YEAR);
	}
	
	void SaveSendClineMap(packet_wrapper & ar, PURCHASE_LIMIT_MAP &map, int limit_type)
	{
		for(PURCHASE_LIMIT_MAP::iterator it = map.begin(); it != map.end(); ++it)
		{
			ar << limit_type << it->first << it->second;
		}
	}
	
	void SaveMap(archive & ar, PURCHASE_LIMIT_MAP &map)
	{
		unsigned int size = map.size();
		ar << size;
		for(PURCHASE_LIMIT_MAP::iterator it = map.begin(); it != map.end(); ++it)
		{
			ar << it->first << it->second;
		}
	}

	bool Save(archive & ar)
	{
		ar << _next_day_item_clear_timestamp << _next_week_item_clear_timestamp << _next_month_item_clear_timestamp << _next_year_item_clear_timestamp;
		SaveMap(ar, _day_items);
		SaveMap(ar, _week_items);
		SaveMap(ar, _month_items);
		SaveMap(ar, _year_items);
		return true;
	}
	
	void LoadMap(archive & ar, PURCHASE_LIMIT_MAP &map)
	{
		unsigned int size;
		ar >> size;
		for(unsigned int i = 0; i < size; ++i)
		{
			int item_id, purchase_count;
			ar >> item_id >> purchase_count;
			map[item_id] = purchase_count;
		}
	}
	
	bool Load(archive & ar)
	{
		ar >> _next_day_item_clear_timestamp >> _next_week_item_clear_timestamp >> _next_month_item_clear_timestamp >> _next_year_item_clear_timestamp;
		LoadMap(ar, _day_items);
		LoadMap(ar, _week_items);
		LoadMap(ar, _month_items);
		LoadMap(ar, _year_items);
		return true;
	}
	
	bool Swap(purchase_limit_info &rhs)
	{
		_next_day_item_clear_timestamp = rhs._next_day_item_clear_timestamp;
		_next_week_item_clear_timestamp = rhs._next_week_item_clear_timestamp;
		_next_month_item_clear_timestamp = rhs._next_month_item_clear_timestamp;
		_next_year_item_clear_timestamp  = rhs._next_year_item_clear_timestamp;
		_day_items.swap(rhs._day_items);	
		_week_items.swap(rhs._week_items);
		_month_items.swap(rhs._month_items);
		_year_items.swap(rhs._year_items);
		return true;
	}

	void SetPurchaseLimitInfo(int day_stamp, int week_stamp, int month_stamp, int year_stamp)
	{
		if(day_stamp == 0)
			_next_day_item_clear_timestamp = player_clock::GetNextUpdatetime(player_clock::GPC_PER_DAY_LOCAL, g_timer.get_systime());
		else
			_next_day_item_clear_timestamp   = day_stamp;
		if(week_stamp == 0)
			_next_week_item_clear_timestamp = player_clock::GetNextUpdatetime(player_clock::GPC_PER_WEEK_LOCAL, g_timer.get_systime());
		else
			_next_week_item_clear_timestamp  = week_stamp;
		if(month_stamp == 0)
			_next_month_item_clear_timestamp = player_clock::GetNextUpdatetime(player_clock::GPC_PER_MONTH_LOCAL, g_timer.get_systime());
		else
			_next_month_item_clear_timestamp = month_stamp;
		if(year_stamp == 0)
			_next_year_item_clear_timestamp = GetNextYearStamp();
		else
			_next_year_item_clear_timestamp = year_stamp;
	}

	void GetPurchaseLimitInfo(int &day_stamp, int &week_stamp, int &month_stamp, int &year_stamp)
	{
		day_stamp   = _next_day_item_clear_timestamp;
		week_stamp  = _next_week_item_clear_timestamp;
		month_stamp = _next_month_item_clear_timestamp;
		year_stamp = _next_year_item_clear_timestamp;
	}

	void GetPurchaseLimitMapInfo(archive & ar)
	{
		SaveMap(ar, _day_items);
		SaveMap(ar, _week_items);
		SaveMap(ar, _month_items);
		SaveMap(ar, _year_items);
	}
	
	void SetPurchaseLimitMapInfo(archive & ar)
	{
		LoadMap(ar, _day_items);
		LoadMap(ar, _week_items);
		LoadMap(ar, _month_items);
		LoadMap(ar, _year_items);
	}

	PURCHASE_LIMIT_MAP& GetLimitMap(int limit_type)
	{
		ASSERT(limit_type > CASH_VIP_SHOPPING_LIMIT_NONE && limit_type < CASH_VIP_SHOPPING_LIMIT_COUNT);
		if(limit_type == CASH_VIP_SHOPPING_LIMIT_DAY)
			return _day_items;
		else if(limit_type == CASH_VIP_SHOPPING_LIMIT_WEEK)
			return _week_items;
		else if(limit_type == CASH_VIP_SHOPPING_LIMIT_MONTH)
			return _month_items;
		else
			return _year_items;
	}
	
	bool CheckShoppingLimitItem(int item_id, int limit_times, int limit_type, int count)
	{
		ASSERT(limit_type >= CASH_VIP_SHOPPING_LIMIT_NONE && limit_type < CASH_VIP_SHOPPING_LIMIT_COUNT);

		if(limit_type == CASH_VIP_SHOPPING_LIMIT_NONE)
			return true;

		PURCHASE_LIMIT_MAP &map = GetLimitMap(limit_type);
		if(map.find(item_id) != map.end())
		{
			int cur_shopping_times = map[item_id];
			if((cur_shopping_times + count) > limit_times)
				return false;
		}
		else
		{
			if(count > limit_times)
				return false;
		}
		return true;
	}
	
	int AddShoppingLimit(int item_id, int limit_type, int count)
	{
		ASSERT(limit_type >= CASH_VIP_SHOPPING_LIMIT_NONE && limit_type < CASH_VIP_SHOPPING_LIMIT_COUNT);

		if(limit_type == CASH_VIP_SHOPPING_LIMIT_NONE)
			return 0;

		PURCHASE_LIMIT_MAP &map = GetLimitMap(limit_type);
		if(map.find(item_id) != map.end())
		{
			map[item_id] = map[item_id] + count;
		}
		else
		{
			map[item_id] = count;
		}
		return map[item_id];
	}
};
struct cash_vip_info
{ 
private:
	int _vip_level; //VIP�ȼ�
	int _score_add; //��¼��ֵ��õĻ���
	int _score_daily_reduce; //ÿ����ٵĻ�����(�ۼ�) ��������ֵ���Ǵ�gamedb����,��get_role��ʱ��ͬ����gs
	int _score_consume; //ͨ���̳ǵ����ĵĻ��� ��gs�۳���,put_role��ʱ��ͬ����db

public:
	cash_vip_info():_vip_level(0),_score_add(0),_score_daily_reduce(0),_score_consume(0)
	{
	}

	bool Save(archive & ar)
	{
		ar << _vip_level << _score_add << _score_daily_reduce << _score_consume;
		return true;
	}
	
	bool Load(archive & ar)
	{
		ar >> _vip_level >> _score_add >> _score_daily_reduce >> _score_consume;
		return true;
	}
	
	bool Swap(cash_vip_info &rhs)
	{
		_vip_level = rhs._vip_level;
		_score_add = rhs._score_add;
		_score_daily_reduce = rhs._score_daily_reduce;
		_score_consume = rhs._score_consume;
		return true;
	}
	
	int GetCurScore()
	{
		int res = _score_add - (_score_daily_reduce + _score_consume);
		if( res > 0 )
		return res;
		return 0;
	}

	int GetCalcVipLevel()
	{
		int score = GetCurScore();
		int * VipScoreNeed = LuaManager::GetInstance()->GetConfig()->VIP_SCORE_LEVEL_TABLE; //{0,15,75,150,375,900,2250,999999999};
		for(int i = 0; i < 8; ++i)
		{
			if( score >= VipScoreNeed[i] && score < VipScoreNeed[i+1])
			{
				return _vip_level = i;
			}
		}
		return 0;
	}

	void SyncCashVipInfoFromDB(int vip_level, int score_add, int score_cost, gplayer * pPlayer)
	{
		_vip_level          = vip_level;
		_score_add          = score_add;
		_score_daily_reduce = score_cost;
		bool update = true;
		if(_score_add == 0 && _score_consume != 0)
			update = false;
		UpdateClientInfo(update, pPlayer);
	}
	
	void SetCashVipInfo(int vip_level, int score_add, int score_daily_reduce, int score_consume, gplayer * pPlayer)
	{
		_vip_level          = vip_level;
		_score_add          = score_add;
		_score_daily_reduce = score_daily_reduce;
		_score_consume      = score_consume;
		bool update = true;
		if(_score_add == 0 && _score_consume != 0)
			update = false;
		UpdateClientInfo(update, pPlayer);
	}

	void GetCashVipInfo(int &vip_level, int &score_add, int &score_daily_reduce, int &score_consume)
	{
		vip_level          = _vip_level;
		score_add          = _score_add;
		score_daily_reduce = _score_daily_reduce;
		score_consume      = _score_consume;
	}
	
	void UpdateClientInfo(bool update, gplayer * pPlayer)
	{
		pPlayer->cash_vip_score = GetCurScore();
		pPlayer->cash_vip_level = GetCalcVipLevel();//_vip_level;

		if(update)
			pPlayer->object_state2 |= gactive_object::STATE_CASH_VIP_MASK;
		else
			pPlayer->object_state2 &= ~gactive_object::STATE_CASH_VIP_MASK;
		pPlayer->imp->_runner->cash_vip_info_notify(_vip_level, GetCurScore());
	}

	void DeliveryNotifyCashVip(int score_add, int vip_level, gplayer *pPlayer)
	{
		_vip_level          = vip_level;
		_score_add          = score_add;
		bool update = true;
		if(_score_add == 0 && _score_consume != 0)
			update = false;
		UpdateClientInfo(update, pPlayer);
	}

	void AddCashVipScore(int score, gplayer * pPlayer)
	{
		_score_consume -= score;
		_vip_level = GetCalcVipLevel();
		UpdateClientInfo(true, pPlayer);
		return;	
	}

	bool SpendCashVipScore(int score, gplayer * pPlayer)
	{
		int tmp = _score_consume + score;
		if(_score_add < (_score_daily_reduce + tmp))
			return false;
		_score_consume = tmp;
		UpdateClientInfo(true, pPlayer);
		return true;
	}
};

struct pet_enhance		//�ٻ�����Ի�ȡһ���������ٻ��ߵ�����
{
	int hp_percent;
	int mp_percent;
	int damage_percent;
	int magic_damage_percent;
	int defense_percent;
	int magic_defense_percent;
	int attack_degree_percent;
	int defend_degree_percent;
};

struct faction_contrib
{
	int consume_contrib;
	int exp_contrib;
	int cumulate_contrib;
};

struct force_ticket_info
{
	int require_force;
	int repu_inc_ratio;
	force_ticket_info():require_force(0),repu_inc_ratio(0){}
};

struct fix_position_transmit_info
{
	int          index;
	int          world_tag;
	A3DVECTOR    pos;
	char         position_name[FIX_POSITION_TRANSMIT_NAME_MAX_LENGTH];
	fix_position_transmit_info()
	{
		index             = -1;
		world_tag         = -1;
		memset(position_name, 0, sizeof(position_name));
	}
	void Reset()
	{
		index             = -1;
		world_tag         = -1;
		memset(position_name, 0, sizeof(position_name));
	}
};

struct arena_invite_tm
{
	int id;
	time_t time;
};

struct glyph_skill_data
{
	int skill;
	int base_skill;
	int item;
	int type;
	int level;
	int cls;
};

// new structures

	// storage 1
	
struct autoswap_data
{
	char count;
	struct
	{
		char inv;
		char equip;
	}	it[64];
};

struct skill_sender_data
{
	char count;
	int skill[16];
};

struct glyph_data
{
	int count;
	int base_skills[GlyphConfig::MAX_SLOT];
};

struct carrier_data
{
	int npcid;
	int monster_id;
	int pos;
	int reserved1;
};

struct repository_data
{
	bool lock; //true = Habilitado, false = Desabilitado
	int lock_time; //Tempo em segundos
	int history_itens[24]; //Itens que estão selecionados
	int storage_itens[64]; //Itens que já foram equipados
	int active_itens[24]; //Itens que estão selecionados
};

struct pet_skill_temp_data
{
	unsigned int skill_id[32];
	unsigned int skill_level[32];
};	

struct pet_skin_data
{
	int storage[32];
	int storage_pos[32];
	int storage_buy[32];
};

/*struct question_data
{
	int question_day;
	int question_progress;
	int question_last_correct;
	int question_done[MAX_QUESTION_PROGRESS];
};*/

struct day_world_points_data
{
	int  total;
	int  points;
	char reward;
	char count;
	
	struct 
	{
		char points;
		int valid_points;
	}	it[WorldPointsConfig::WORLD_POINT_LIST_COUNT];
	
};
	// storage 2
	
struct activity_data
{
	bool enable_skill;
	
	int event_cash_history;
	struct
	{
		int mode;
		int index;
		int count;
	}	event_login;
	
	int event_shop_count;
	struct 
	{
		int limit_mode;
		int item_id;
		int limit_count;
	}	event_shop[ActivityEventConfig::MAX_SHOP_ITENS];	
	
	int login_day_verify;
};

// struct celestial;

// new structures end

class switch_additional_data : public substance
{
public:
	DECLARE_SUBSTANCE(switch_additional_data);
	virtual ~switch_additional_data(){}
	virtual bool Save(archive & ar) { return true;}
	virtual bool Load(archive & ar) { return true;}
};

class countryterritory_switch_data : public switch_additional_data
{
	countryterritory_switch_data(){}
public:
	int country_id;
	int country_expiretime;
	int major_strength;
	int minor_strength;
public:
	DECLARE_SUBSTANCE(countryterritory_switch_data);
	countryterritory_switch_data(int country, int expiretime, int major_str, int minor_str)
		: country_id(country), country_expiretime(expiretime), major_strength(major_str), minor_strength(minor_str){}
	virtual bool Save(archive & ar)
	{ 
		ar << country_id << country_expiretime << major_strength << minor_strength;	
		return true;
	}
	virtual bool Load(archive & ar)
	{
		ar >> country_id >> country_expiretime >> major_strength >> minor_strength;
		return true;
	}
};

class trickbattle_switch_data : public switch_additional_data
{
	trickbattle_switch_data(){}
public:
	int chariot;
public:
	DECLARE_SUBSTANCE(trickbattle_switch_data);
	trickbattle_switch_data(int _chariot) : chariot(_chariot){}
	virtual bool Save(archive & ar)
	{
		ar << chariot;
		return true;
	}
	virtual bool Load(archive & ar)
	{
		ar >> chariot;
		return true;
	}
};

class arenaofaurora_switch_data : public switch_additional_data
{
	arenaofaurora_switch_data() {}
public:
	int value;
public:
	DECLARE_SUBSTANCE(arenaofaurora_switch_data);
	arenaofaurora_switch_data(int _value) : value(_value) {}
	virtual bool Save(archive& ar)
	{
		ar << value;
		return true;
	}
	virtual bool Load(archive& ar)
	{
		ar >> value;
		return true;
	}
};

class mnfaction_switch_data : public switch_additional_data
{
	mnfaction_switch_data(){}
public:
	int64_t _faction_id;
	int _domain_id;
public:
	DECLARE_SUBSTANCE(mnfaction_switch_data);
	mnfaction_switch_data(int64_t faction_id, int domain_id)
		: _faction_id(faction_id),_domain_id(domain_id)
		{}
	virtual bool Save(archive & ar)
	{ 
		ar <<  _faction_id << _domain_id;	
		return true;
	}
	virtual bool Load(archive & ar)
	{
		ar >>  _faction_id >> _domain_id;
		return true;
	}
};

class gplayer_dispatcher : public  dispatcher
{
protected:
	packet_wrapper _mw;
	packet_wrapper _nw;
	packet_wrapper _pw;
	packet_wrapper _self;
	packet_wrapper _tbuf;
	link_sid       _header;
	abase::vector<int,abase::fast_alloc<> > _leave_list;		//�����뿪ʱ���ٿ����Ķ���ID
public:
	DECLARE_SUBSTANCE(gplayer_dispatcher);
public:
	bool Save(archive & ar)
	{
		ar << _header;
		return true;
	}
	bool Load(archive & ar)
	{
		ar >> _header;
		return true;
	}
	gplayer_dispatcher()
	{}
	virtual void init(gobject_imp * imp)
	{
		dispatcher::init(imp);
		gplayer * pPlayer = (gplayer*)imp->_parent;
		make_link_sid(pPlayer,_header);
	}
	virtual ~gplayer_dispatcher(){}
	virtual void begin_transfer();
	virtual void end_transfer();
	virtual void enter_slice(slice * ,const A3DVECTOR &pos);
	virtual void leave_slice(slice * ,const A3DVECTOR &pos);
	virtual void notify_pos(const A3DVECTOR & pos);
	virtual void get_base_info();
	virtual void enter_world();	//cache
	virtual void leave_world();
	virtual void appear();
	virtual void move(const A3DVECTOR & target, int cost_time,int speed,unsigned char move_mode);
	virtual void stop_move(const A3DVECTOR & target, unsigned short speed,unsigned char dir,unsigned char move_mmode);
	virtual void notify_move(const A3DVECTOR &oldpos, const A3DVECTOR & newpos);
	virtual void be_damaged(const XID & id,int skill_id, const attacker_info_t &,int damage,int dura_index,int at_state,char speed,bool orange,unsigned char section);
	virtual void dodge_attack(const XID &attacker, int skill_id, const attacker_info_t & , int at_state,char speed,bool orange,unsigned char section);
	virtual void be_hurt(const XID & id, const attacker_info_t & ,int damage,bool invader);

	virtual void equipment_damaged(int index,char reason);
	virtual void start_attack(const XID &);
	virtual void attack_once(unsigned char dec_amount);
	virtual void stop_attack(int flag);
	virtual void on_death(const XID &,bool);
	virtual void error_message(int msg);
	virtual void resurrect(int);
	virtual void disappear();
	//virtual void pickup_money(int money, unsigned char is_silver);
	virtual void pickup_item(int type,int expire_item, int amount,int slot_amount, int where,int index);
	virtual void purchase_item(int type, unsigned int money, int amount,int slot_amount, int where,int index);
	virtual	void query_info00(const XID & target, int cs_index,int sid);
	virtual void query_info00();
	virtual void query_elf_vigor(); //lgc
	virtual void query_elf_enhance();
	virtual void query_elf_stamina(int sta);
	virtual void query_elf_exp(int exp);
	virtual void elf_cmd_result(int cmd, int result, int param1, int param2);
	virtual void elf_levelup(); 
	virtual void receive_exp(int exp,int sp);
	virtual void level_up();
	virtual void unselect();
	virtual void self_item_empty_info(int where,int index);
	virtual void self_item_info(int where,int index,item_data & data,unsigned short crc);
	virtual void self_inventory_data(int where,unsigned char inv_size,const void * data, unsigned int len);
	virtual void self_inventory_detail_data(int where,unsigned char inv_size,const void * data, unsigned int len);
	virtual void exchange_inventory_item(unsigned int idx1,unsigned int idx2);
	virtual void move_inventory_item(unsigned int src,unsigned int dest,unsigned int count);
	virtual void player_drop_item(unsigned int where, unsigned int index,int type,unsigned int count, unsigned char drop_type);
	virtual void exchange_equipment_item(unsigned int index1,unsigned int index2);
	virtual void equip_item(unsigned int index_inv,unsigned int index_equip,int count_inv,int count_equip);
	virtual void move_equipment_item(unsigned int index_inv,unsigned int index_equip, unsigned int count);
	virtual void self_get_property(unsigned int status_point, const extend_prop & prop , int attack_degree, int defend_degree, int crit_rate, int crit_damage_bonus, int invisible_degree, int anti_invisible_degree, int penetration, int resilience, int vigour, int anti_def_degree, int anti_resist_degree, const extend_prop & base_prop, int highest_level, int reserve);
	virtual void set_status_point(unsigned int vit, unsigned int eng, unsigned int str, unsigned int agi, unsigned int remain);
	virtual void player_select_target(int id);
	virtual void get_extprop_base();
	virtual void get_extprop_move();
	virtual void get_extprop_attack();
	virtual void get_extprop_defense();
	virtual void player_reject_invite(const XID & member);
	virtual void leader_invite(const XID & leader,int seq,int pickup_flag);
	virtual void join_team(const XID & leader,int pickup_flag);
	virtual void member_leave(const XID & leader, const XID& member,int type);
	virtual void leave_party(const XID & leader, int type);
	virtual void new_member(const XID & member);
	virtual void leader_cancel_party(const XID & leader);
	virtual void teammate_get_pos(const XID & target,const A3DVECTOR & pos, int tag, bool same_plane);
	virtual void send_equipment_info(const XID & target, int cs_index, int sid);
	virtual void equipment_info_changed(uint64_t madd,uint64_t mdel,const void * buf, unsigned int size);
	virtual void team_member_pickup(const XID & target,int type, int count);
	virtual void npc_greeting(const XID & provider);
	virtual void repair_all(unsigned int cost);
	virtual void repair(int where,int index,unsigned int cost);
	virtual void renew();
	//virtual void spend_money(unsigned int cost);
	virtual void get_player_money(unsigned int money, unsigned long long silver );
	virtual void cast_skill(const XID & target, int skill,unsigned short time, unsigned char level);
	virtual void cast_rune_skill(const XID & target, int skill,unsigned short time, unsigned char level);
	virtual void skill_interrupt(char reason);
	virtual void skill_perform();
	virtual void stop_skill();
	virtual void get_skill_data();
	virtual void clear_embedded_chip(unsigned short equip_idx,unsigned int use_money);
	virtual void cost_skill_point(int sp);
	virtual void learn_skill(int skill, int level);
	virtual void takeoff();
	virtual void landing();
	virtual void flysword_time_capacity(unsigned char where, unsigned char index, int cur_time);
	virtual void obtain_item(int type, int expire_date, int amount,int slot_amount, int where,int index);
	virtual void produce_start(int type, int use_time ,int count);
	virtual void produce_once(int type, int amount,int slot_amount, int where,int index);
	virtual void produce_end();
	virtual void decompose_start(int use_time,int type);
	virtual void decompose_end();
	virtual void get_task_data();
	virtual void send_task_var_data(const void * buf, unsigned int size);
	virtual void start_use_item(int item_type, int use_time);
	virtual void cancel_use_item();
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count); //���Լ�
	virtual void use_item(int item_type); //������
	virtual void use_item(char where, unsigned char index , int item_type , unsigned short count,const char * arg, unsigned int arg_size);
	virtual void use_item(int item_type,const char * arg, unsigned int arg_size);
	virtual void start_use_item_with_target(int item_type, int use_time,const XID & target);
	virtual void sit_down();
	virtual void stand_up();
	virtual void do_emote(unsigned short emotion);
	virtual void do_emote_restore(unsigned short emotion);
	virtual void do_action(unsigned char action);
	virtual void send_timestamp();
	virtual void notify_root(unsigned char type);
	virtual void dispel_root(unsigned char type);
	virtual void invader_rise();
	virtual void pariah_rise();
	virtual void invader_fade();
	virtual void gather_start(int mine,unsigned char t);
	virtual void gather_stop();
	virtual void trashbox_passwd_changed(bool has_passwd);
	virtual void trashbox_passwd_state(bool has_passwd);
	virtual void trashbox_open(char is_usertrashbox);
	virtual void trashbox_close(char is_usertrashbox);
	virtual void trashbox_wealth(char is_usertrashbox, unsigned int money);
	virtual void exchange_trashbox_item(int where, unsigned int idx1, unsigned int idx2);
	virtual void move_trashbox_item(int where, unsigned int src, unsigned int dest, unsigned int delta);
	virtual void exchange_trashbox_inventory(int where, unsigned int idx_tra,unsigned int idx_inv);
	virtual void trash_item_to_inventory(int where, unsigned int idx_tra, unsigned int idx_inv, unsigned int delta);
	virtual void inventory_item_to_trash(int where, unsigned int idx_inv, unsigned int idx_tra, unsigned int delta);
	virtual void exchange_trash_money(char is_usertrashbox, int inv_money, int tra_money);
	virtual void set_adv_data(int data1,int data2);
	virtual void clear_adv_data();
	virtual void player_in_team(unsigned char state);
	virtual void send_party_apply(int id);
	virtual void query_info_1(int uid,int cs_index, int cs_sid);
	virtual void concurrent_emote_request(int id, unsigned short action);
	virtual void do_concurrent_emote(int id, unsigned short action);
	virtual void mafia_info_notify();
	virtual void task_deliver_reputaion(int offset,int cur_reputaion);
	virtual void task_deliver_exp(int exp, int sp);
	virtual void task_deliver_money(unsigned int amount,unsigned int cur_money,unsigned int silver);
	virtual void task_deliver_item(int type, int expire_date,int amount,int slot_amount, int where,int index);
	virtual void task_deliver_level2(int level2);
	virtual void get_reputation(int reputation);
	virtual void identify_result(char index, char result);
	virtual void change_shape(char shape);
	virtual void elf_refine_activate(char);		//lgc
	virtual void enter_sanctuary();
	virtual void leave_sanctuary();
	virtual void begin_personal_market(int market_id,const char *name, unsigned int len);
	virtual void cancel_personal_market();
	virtual void market_trade_success(int trader);
	virtual void send_market_name(const XID & target, int cs_index, int sid,const char * name ,unsigned int len);
	virtual void player_start_travel(int  line_no,const A3DVECTOR & dest_pos,float speed,int vehicle);
	virtual void player_complete_travel(int vehicle);
	virtual void gm_toggle_invisible(char tmp);
	virtual void toggle_invincible(char tmp);
	virtual void trace_cur_pos(unsigned short seq);
	virtual void cast_instant_skill(const XID & target, int skill,unsigned char level);
	virtual void cast_rune_instant_skill(const XID & target, int skill,unsigned char level);
	virtual void cast_elf_skill(const XID & target, int skill,unsigned char level);		//lgc
	virtual void activate_waypoint(unsigned short waypoint);
	virtual void player_waypoint_list(const unsigned short * buf, unsigned int count);
	virtual void unlock_inventory_slot(unsigned char where, unsigned short index);
	virtual void team_invite_timeout(int who);
	virtual void enable_pvp_state(char type);
	virtual void disable_pvp_state(char type);
	virtual void player_pvp_cooldown(int cooldown);
	virtual void send_cooldown_data();
	virtual void skill_ability_notify(int id, int ability);
	virtual void personal_market_available();
	virtual void breath_data(int breath, int breath_capacity);
	virtual void stop_dive();
	virtual void trade_away_item(int buyer,short inv_idx,int type, unsigned int count);
	virtual void player_enable_fashion_mode(unsigned char is_enable, unsigned char new_model );
	virtual void player_enable_free_pvp(char is_enable);
	virtual void player_enable_effect(short effect);
	virtual void player_disable_effect(short effect);
	virtual void enable_resurrect_state(float exp_reduce);
	virtual void set_cooldown(int idx, int cooldown);
	virtual void change_team_leader(const XID & old_leader, const XID & new_leader);
	virtual void kickout_instance(char reason, int timeout);
	virtual void begin_cosmetic(unsigned short inv_index);
	virtual void end_cosmetic(unsigned short inv_index);
	virtual void cosmetic_success(unsigned short crc);
	virtual void cast_pos_skill(const A3DVECTOR & pos, const XID &target, int skill,unsigned short time,unsigned char level);
	virtual void change_move_seq(unsigned short seq);
	virtual void server_config_data();
	virtual void active_rush_mode(char is_active);
	virtual void produce_null(int recipe_id);
	virtual void enable_double_exp_time(int mode, int end_time);
	virtual void available_double_exp_time();
	virtual void active_pvp_combat_state(bool is_active);
	virtual void duel_recv_request(const XID &target);
	virtual void duel_reject_request(const XID &target,int reason);
	virtual void duel_prepare(const XID & target, int delay);
	virtual void duel_cancel(const XID & target);
	virtual void duel_start(const XID & who);
	virtual void duel_stop();
	virtual void duel_result(const XID & target, bool is_failed);
	virtual void player_bind_request(const XID & target, int param_id);
	virtual void player_bind_invite (const XID & target, int param_id);
	virtual void player_bind_request_reply(const XID & target, int param);
	virtual void player_bind_invite_reply(const XID & target, int param); 
	virtual void player_bind_start(const XID & target);
	virtual void player_bind_stop();
	virtual void player_mounting(int mount_id, unsigned short mount_color);
	virtual void send_equip_detail(int cs_indx, int cs_sid, int target, const void * data, unsigned int size);
	virtual void send_inventory_detail(int cs_indx, int cs_sid, int target, unsigned int money, unsigned char inv_size, const void * data, unsigned int size);
	virtual void send_others_property(const void * data, unsigned int size, const void * self_data, unsigned int self_size);
	virtual void pariah_duration(int remain_time);

	virtual void gain_pet(int index, const void * buf, unsigned int size);
	virtual void free_pet(int index, int pet_id);
	virtual void summon_pet(int index, int pet_tid, int pet_id, int life_time);
	virtual void start_pet_operation(int index, int pet_id,int delay,int operation);
	virtual void end_pet_operation();
	virtual void recall_pet(int index, int pet_id, char reason);
	virtual void pet_recv_exp(int index, int pet_id, int exp);
	virtual void pet_level_up(int index, int pet_id, int level, int cur_exp);
	virtual void pet_room_capacity(int cap);
	virtual void notify_pet_honor(int index,int honor);
	virtual void notify_pet_hunger(int index, int hunger);
	virtual void enter_battleground(int role,int battle_id, int end_time);
	virtual void battle_result(int result);
	virtual void battle_score(int oscore, int ogoal, int dscore, int dgoal);
	virtual void pet_dead(int index);
	virtual void pet_revive(int index,float hp_factor);
	virtual void pet_hp_notify(int index, float hp_factor, int cur_hp, float mp_factor, int cur_mp);
	virtual void pet_ai_state(char aggro_state, char stay_state);
	virtual void refine_result(int rst);
	virtual void material_refine_result(int reserve, int rst);
	virtual void pet_set_cooldown(int index, int cd_index, int msec);
	virtual void player_cash(int cash);
	virtual void player_bind_success(unsigned int index, int id);
	virtual void player_change_inventory_size(int size);
	virtual void player_pvp_mode(int level);
	virtual void player_wallow_info(int level, int ptime,int light_t, int heavy_t, int reason);
	
	//void asm_substribe_log();
	void mafia_trade_start();
	void mafia_trade_end();
	//���溯����playerר���������Ϣ���ݺ���,����������ʱû�� ������ʱ�����麯��
	void send_team_data(const XID & leader,unsigned int team_count,unsigned int data_count, const player_team::member_entry ** list);
	void send_team_data(const XID & leader,unsigned int team_count,const player_team::member_entry * list);

	void item_to_money(unsigned int index, int type, unsigned int count,unsigned int price, unsigned int silver_price, unsigned int guild_price);
	void pickup_money_in_trade(unsigned int);
	void pickup_item_in_trade(int type, int amount);
	void receive_money_after_trade(unsigned int);
	void receive_item_after_trade(int type,int expire_date, int amount, int slot_amount, int index);
	void embed_item(unsigned int chip_idx, unsigned int equip_idx);
	void object_is_invalid(int id);
	void trashbox_capacity_notify(int where, int cap);
	void send_pet_room(pet_data **,unsigned int start, unsigned int end);
	void player_change_spouse(int id);
    void equip_can_inherit_addons(int equip_id, unsigned char inv_idx, unsigned char addon_num, int addon_id_list[]);
	virtual void notify_safe_lock(char active, int time, int max_time, bool paswd);
	virtual void mall_item_buy_failed(short index, char reason);//lgc
	virtual void equip_trashbox_item(int where, unsigned char trash_idx,unsigned char equip_idx); 
	virtual void security_passwd_checked();
	virtual void appear_to_spec(int invi_degree);		//�뿪invi_degree������״̬,��ָ���˷���appear��Ϣ
	virtual void disappear_to_spec(int invi_degree);	//����invi_degree������״̬,��ָ���˷���disappear��Ϣ
	virtual void on_inc_invisible(int prev_invi_degree, int cur_invi_degree);	//�����ȼ������ָ���˷���disappear��Ϣ
	virtual void on_dec_invisible(int prev_invi_degree, int cur_invi_degree);	//�����ȼ�������ָ���˷���appear��Ϣ
	virtual void on_inc_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree);	//�����ȼ���߻�ȡ�µĿ��Ӷ���
	virtual void on_dec_anti_invisible(int prev_a_invi_degree, int cur_a_invi_degree);	//�����ȼ����ͻ�ȡ���ٿɼ��Ķ���
	virtual void hp_steal(int hp);
	virtual void player_dividend(int dividend);
	virtual void dividend_mall_item_buy_failed(short index, char reason);
	virtual void multi_exp_info(int last_timestamp, float enhance_factor);
	virtual void change_multi_exp_state(char state, int enchance_time, int buffer_time, int impair_time, int activate_times_left);
	virtual void send_world_life_time(int life_time);
	virtual void wedding_book_success(int type);
	virtual void calc_network_delay(int timestamp);
	virtual void player_knockback(const A3DVECTOR & pos, int time);
	virtual void summon_plant_pet(int plant_tid/*ģ��ID*/, int plant_id/*����ID*/, int life_time);
	virtual void plant_pet_disappear(int id, char reason);
	virtual void plant_pet_hp_notify(int id, float hp_factor, int cur_hp, float mp_factor, int cur_mp);
	virtual void pet_property(int index, const extend_prop & prop, int r_def_lvl_f, int r_def_lvl_m, int r_chi_point);
	virtual void faction_contrib_notify();
	virtual void faction_relation_notify();
	virtual void enter_factionfortress(int role_in_war, int factionid, int offense_endtime);
	virtual void player_equip_disabled(int64_t mask);
	virtual void send_spec_item_list(int cs_index, int cs_sid, int target, int type, void * data, unsigned int size);
	virtual void send_error_message(int cs_index, int cs_sid, int target, int message);
	virtual void congregate_request(char type, int sponsor, int timeout);
	virtual void reject_congregate(char type, int id);
	virtual void congregate_start(char type, int time);
	virtual void cancel_congregate(char type);

	virtual void engrave_start(int recipe_id, int use_time); // Recipe ID e Tempo em Segundos 60
	virtual void engrave_end(); // Somente encerra a gravura
	virtual void engrave_result(int addon_num); // Quantidade de Atributos
	virtual void engrave_new_info(int addon_count, unsigned int * engraves); // Nova Gravura


	virtual void addonregen_start(int recipe_id, int use_time);
	virtual void addonregen_end();
	virtual void addonregen_result(int addon_num);
	virtual void invisible_obj_list(gobject ** ppObject, unsigned int count);
	virtual void set_player_limit(int index, char b);
	virtual void player_teleport(const A3DVECTOR& pos, unsigned short use_time, char mode);
	virtual void forbid_be_selected(char b);
	virtual void send_player_force_data(int cur_force, unsigned int count, const void* data, unsigned int data_size);
	virtual void player_force_changed(int force);
	virtual void player_force_data_update(int force, int repu, int contri);
	virtual void send_force_global_data(char data_ready, unsigned int count, const void * data, unsigned int data_size);
	virtual void add_multiobj_effect(int target, char type);
	virtual void remove_multiobj_effect(int target, char type);
	virtual void enter_wedding_scene(int groom, int bride);
	virtual void produce4_item_info(int stime, item_data & data, unsigned short crc); //���Ϳͻ����¼̳�������������Ʒ��Ϣ
	virtual void online_award_data(int total_award_time, unsigned int count, const void * data, unsigned int data_size);
	virtual void toggle_online_award(int type, char activate);
	virtual void update_profit_time(char flag, int profit_time, int profit_level);
	virtual void notify_profit_state(char state);
	virtual void enter_nonpenalty_pvp_state(char state);
	virtual void self_country_notify(int country_id);
	virtual void player_country_changed(int country_id);
	virtual void enter_countrybattle(int role, int battle_id,int end_time,int offense_country, int defence_country);
	virtual void countrybattle_result(int result);
	virtual void countrybattle_score(int oscore, int ogoal, int dscore, int dgoal);
	virtual void countrybattle_resurrect_rest_times(int times);
	virtual void countrybattle_became_flag_carrier(char is_carrier);
	virtual void countrybattle_personal_score(int combat_time, int attend_time, int kill_count, int death_count, int country_kill_count, int country_death_count);
	virtual void defense_rune_enabled(char rune_type, char enable);
	virtual void countrybattle_info(int attacker_count, int defender_count);
	virtual void cash_money_exchg_rate(char open, int rate);
	virtual void pet_rebuild_inherit_start(unsigned int index,int use_time);
	virtual void pet_rebuild_inherit_info(int stime,int pet_id,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/); 
	virtual void pet_rebuild_inherit_end(unsigned int index);
	virtual void pet_evolution_done(unsigned int index);
	virtual void pet_rebuild_nature_start(unsigned int index,int use_time);
	virtual void pet_rebuild_nature_info(int stime,int pet_id,unsigned int index,int nature);
	virtual void pet_rebuild_nature_end(unsigned int index);
	virtual void notify_meridian_data(int meridian_level,int lifegate_times,int deathgate_times,int free_refine_times,int paid_refine_times,int continu_login_days,int trigrams_map[3]);
	virtual void try_refine_meridian_result(int index,int result);
	virtual void equip_addon_update_notify(unsigned char update_type,unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type);
	virtual void self_king_notify(char is_king, int expire_time);
	virtual void player_king_changed(char is_king);
	virtual void notify_touch_query(int64_t income,int64_t remain,int retcode);
	virtual void notify_touch_cost(int64_t income,int64_t remain,unsigned int cost,unsigned int index,unsigned int lots,int retcode);
	virtual void notify_addup_money(int64_t addupmoney);
	virtual void query_title(int roleid,int count,int ecount, const void * data, unsigned int data_size, const void *edata, unsigned int edata_size);
	virtual void notify_curr_title_change(int roleid,unsigned short titleid);
	virtual void notify_title_modify(unsigned short titleid,int expiretime,char flag);
	virtual void refresh_signin(char type,int lys, int moncal, int uptime, char latesignintimes, char awardedtimes, char awardedtimes2);
	virtual void notify_giftcard_redeem(int retcode,int cardtype,int parenttype, const char(&cardnumber)[player_giftcard::GIFT_CARDNUMBER_LEN] );
	virtual void player_reincarnation(unsigned int reincarnation_times);
	virtual void activate_reincarnation_tome(char active);
	virtual void realm_exp_receive(int exp,int receive_exp, int diary_exp);
	virtual void realm_level_up(unsigned char level);
	virtual void enter_trickbattle(int role, int battle_id,int end_time);
	virtual void trickbattle_personal_score(int kill, int death, int score, int multi_kill);
	virtual void trickbattle_chariot_info(int chariot, int energy);
	virtual void player_leadership(int leadership, int inc_leadership);
	virtual void generalcard_collection_data(const void * buf, unsigned int size);
	virtual void add_generalcard_collection(unsigned int collection_idx);
	virtual void player_active_combat(bool is_active);
	virtual void random_mall_shopping_result(int eid,int opt,int res,int item,int price,bool flag);
	virtual void player_mafia_pvp_mask_notify(unsigned char mafia_pvp_mask);
	virtual void player_world_contribution(int points_total, int points_rest, char reward_stage, char count, const char * values);
	virtual void randommap_order_init(int row,int col,const int* room_src);
	virtual void send_scene_service_npc_list(unsigned int count, int * data);
	virtual void player_screen_effect_notify(int type, int eid, int state);
	virtual void player_combo_skill_prepare(int skillid,int timestamp,int arg1, int arg2, int arg3);
	virtual void player_pray_distance_change(float pd);
	virtual void instance_reenter_notify(int tag, int timeout);
	virtual void astrolabe_info_notify(unsigned char level, int exp);
	virtual void astrolabe_operate_result(int opt, int ret, int a0, int a1, int a2);
    virtual void property_score_result(int fighting_score, int viability_score, int client_data);
    virtual void lookup_enemy_result(int rid, int worldtag, const A3DVECTOR& pos);
	virtual void solo_challenge_award_info_notify(int max_stage_level, int total_time, int total_score, int cur_score, int last_success_stage_level, int last_success_stage_cost_time, int draw_award_times,int have_draw_award_times, abase::vector<struct playersolochallenge::player_solo_challenge_award>& award_info);
	virtual void solo_challenge_operate_result(int opttype, int retcode, int arg0, int arg1, int arg2);
	virtual void solo_challenge_challenging_state_notify(int climbed_layer, unsigned char notify_type);
	virtual void solo_challenge_buff_info_notify(int *buff_index, int * buff_num, int count, int cur_score);
	virtual void mnfaction_player_faction_info(int player_faction, int domain_id);
	virtual void mnfaction_resource_point_info(int attacker_resource_point, int defender_resource_point);
	virtual void mnfaction_player_count_info(int attend_attacker_player_count, int attend_defender_player_count);
	virtual void mnfaction_resource_point_state_info(int index, int cur_degree);
	virtual void mnfaction_resource_tower_state_info(int num, MNFactionStateInfo &mnfaction_state_info);
	virtual void mnfaction_switch_tower_state_info(int num, MNFactionStateInfo &mnfaction_state_info);
	virtual void mnfaction_transmit_pos_state_info(int num, MNFactionStateInfo &mnfaction_state_info);
	virtual void mnfaction_result(int result);
	virtual void mnfaction_battle_ground_have_start_time(int battle_ground_have_start_time);
	virtual void mnfaction_faction_killed_player_num(int attacker_killed_player_count, int defender_killed_player_count);
	virtual void mnfaction_shout_at_the_client(int type, int args);
	virtual void fix_position_transmit_add_position(int index, int world_tag, A3DVECTOR &pos, unsigned int position_length, const char *position_name);
	virtual void fix_position_transmit_delete_position(int index);
	virtual void fix_position_transmit_rename(int index, unsigned int position_length, char *position_name);
	virtual void fix_position_energy_info(char is_login, int cur_energy);
	virtual void fix_position_all_info(fix_position_transmit_info *info);
	virtual void cash_vip_mall_item_buy_result(char result, short index, char reason);
	virtual void cash_vip_info_notify(int level, int score);
	virtual void purchase_limit_all_info_notify();
	virtual void purchase_limit_info_notify(int limit_type, int item_id, int have_purchase_count); 
    virtual void cash_resurrect_info(int cash_need, int cash_left);
	virtual void body_scale_notify(int mode, float scale);
	virtual void filter_soul_info(int soul);
	virtual void home_task_info( void *buf, unsigned int size, int task_trigger_times, int task_refresh_times);
	
	virtual void get_lotery_info( unsigned int manager, unsigned int score, unsigned int count, unsigned int * item );
	virtual void lottery_reward_info( unsigned int unk /* 14 */, unsigned int score, unsigned int count, unsigned int itemid );
	virtual void get_lotery_items( unsigned int count, unsigned int * item );
	virtual void get_treasure_info( unsigned char manager, unsigned int score, unsigned int free_count, unsigned int lot_count, unsigned int box_count, void * _lot, void * _box );
	virtual void get_lib_items( unsigned int count, unsigned int * item ); // Lib Itens
	//virtual void glyph_info( unsigned int count, void * item );
	virtual void glyph_slot_info( int reserve1, int count, int pvp_point );		
	virtual void get_glyph_skill( int mode, int pos, int skill, int item );
	virtual void get_glyph_upgrade( int mode, int result, int pos,  int new_item, int old_item );
	
	virtual void result_lib_produce();
	virtual void refine_bible_result(int rst);

	virtual void get_server_time( unsigned int time );
	virtual void get_weapon_vstate( char state );
	virtual void get_shield_energy( short shgen, short shmax );
	
	virtual void transfer_refine_bible(int rst1, int rst2, int item_pos);
	virtual void get_storage_points(short rst1, short rst2);
	virtual void get_finish_notify(int rst1, int rst2);
	/*166+*/
	virtual void pet_skill_data( unsigned int count, int index,  unsigned int * skills );
	virtual void pet_skin_info( int index,  int skin_pet_id );
	virtual void pet_skin_storage( unsigned int count,  unsigned int * values );

	/*156+*/
	virtual void player_carrier_up(int pos, int reserve1, unsigned int npcid);
	virtual void player_carrier_down(int pos, unsigned int npcid);
	virtual void player_carrier_self_up(int pos, int count, unsigned int npcid, int res, float speed_walk, float speed_run, float speed_swim, float speed_fly);
	virtual void carrier_sync_info(unsigned int npcid, int reserve1, int reserve2);
	virtual void carrier_prop(unsigned int npcid, int reserve1, unsigned int carrier_data);
	virtual void carrier_skill_cooldown(unsigned int skill_id, int cooldown);

	/*156+*/
	virtual void get_imperial_spirit(int enabled, unsigned int expired_time);

	/*160+*/
	virtual void player_repository_info(unsigned int roleid, unsigned int storage_itens, int itens_storage[], int used_itens, int itens_active[]);
	virtual void player_repository_storage(unsigned int count_values, int itens[]);
	
	/*171+*/
	virtual void dynamic_genesis_level(unsigned int count, const void * skills_array);
	
	// Novas cargas do mercenário
	virtual void get_charge_merc(char charges);

	// Novo Range Glyph
	virtual void skill_glyph_info( int type, unsigned int count, unsigned int * values );

	virtual void player_world_speak_info( char enabled, char enabled2, char kid, int skills_count, int * skills );

	// Anedota
	virtual void get_anecdote_info(short uTask1, short uTask2, short uTask3, short uTask4, short uTask5, short uTask6, short uTask7, short uTask8, short uTask9, short uTask10, short uTask11, short uTask12, short uTask13, short uTask14, short uTask15, short uTask16);

	// Segment
	virtual void skill_cooldown_limit(int skill_id , unsigned int skill_last_use_time , unsigned int skill_cooling_sec , unsigned short count , unsigned short max_count);
	
	// new money 172+
	virtual void pickup_gold(unsigned int value);
	virtual void pickup_silver(unsigned int value);
	virtual void arena_team_invite(int inviting_id, int team_type);
	virtual void arena_battle_group_info(unsigned int time_rest, int red_team_count_rest, int blue_team_count_rest, int team_count_members_red, int team_count_members_blue, int members_red[], int members_blue[]);
	virtual void arena_player_faction_info(int team_id, int type, int time_rest);

	/*170+ Códice*/
	virtual void codex_fashion_activate(int roleid, int mode_classic, unsigned int size, const void * fashion_activate);
	virtual void codex_fashion_storage(int skins_count, unsigned int size,  const void * skins, int fashion_points);
	virtual void codex_fashion_dye(int mode, int count_fashions, unsigned int size, const void * fashion_dye);
	virtual void codex_reward_title(int type, int pos);
	virtual void codex_reward_first(int type, int reward_idx, int result);
	virtual void codex_pet_consum_info(int pet_egg_id, int res, short res2, int mount_points);
	virtual void codex_fly_consum_info(int fly_id, char res, int speed, int fly_points);
	virtual void codex_improve_info(int fly_id, char apprimored_level, int max_time_speed);
	virtual void codex_fly_equip_info(short index_equip, char index_inv, char where_equip, int pos_codex_inv, int fly_item_id, int speed, char apprimored_level);
	virtual void codex_rename_pet_info(int pet_id ,const char * name, short name_len);

	/*170+ Bebe Celestial*/
	virtual void kid_course_change(char old_slot, char new_slot);
	virtual void kid_course_remove(char old_slot);
	virtual void kid_name_awakening(short name_len, const char * name);
	virtual void kid_course_info(unsigned int * course_info, int count);
	virtual void kid_course_perc(char level, int reserved);
	virtual void kid_course_insert(char old_slot, char new_slot);
	virtual void kid_course_switch(char new_slot, char old_slot1, char old_slot2);
	virtual void kid_awakening_points(int points);
	virtual void kid_created_info_dialog();
	virtual void kid_awakening_info(unsigned int size, const void * kid_awakening_info);
	virtual void kid_awakening_cash_info(int awakening_cash, int awakening_potential);
	
	virtual void kid_celestial_info( unsigned int size, const void * kid_info);
	virtual void kid_active_info(int active_slot, int reserved);
	virtual void kid_award_addon(unsigned int size, const void * info);
	virtual void kid_celestial_awakening(int type, int reserve);
	virtual void kid_celestial_transformation(int shape, int roleid, int reserve, int reserve2);
	
	/*174+*/
	virtual void reward_interface_notify(unsigned int mode, unsigned int value_index, int count);
	virtual void activity_event_spend_cash(unsigned int roleid, unsigned int cash);
	virtual void activity_event_shop(int count, char unk6, unsigned int * values);
	// Memorial Celestial
	virtual void celestial_memorial_info(bool type, unsigned int size, const void * info,unsigned int size2, const void * info2, unsigned int size3, const void * info3);
	virtual void celestial_memorial_lottery( int count, unsigned int size, const void * lotterys);
	virtual void celestial_memorial_lottery_count( int perg1, int perg2, int perg3);

	// G17
	virtual void armor_info_notify(unsigned int reserved, unsigned int count, int reserved3, int reserved4, int inv_slot);
	virtual void armor_info_purification(int count, unsigned int * values);

	/*176+*/
	virtual void portatil_picture_info(int roleid, int res, int res2, int idx, int res3);
	virtual void portatil_picture_storage(int count, int storage[4]);

	//virtual void question_task(int mode, int progress, int question, int correct, int time);
	virtual void kid_system_points_notify(int points);

public:
	friend class gplayer_imp;
	virtual void LoadFrom(gplayer_dispatcher * rhs)		//��ԭ�е�dispatcher��ȡ������
	{
		memcpy(&_header , &rhs->_header,sizeof(_header));
	}
protected:
	virtual void enter_region();	//Ϊ�˸�gm����Ȩ�ޣ������Ϊ�麯������gm_dispatcher��д�������
};

class gplayer_controller : public controller
{
protected:
	XID  _cur_target;
	XID  _cur_sub_target;  // Ŀ���Ŀ��
	bool _target_refresh_state;	// Ŀ��������֪ͨ������
	A3DVECTOR _last_pos;
	int  _load_stats;
	short _peep_counter;		//��ʱ���ͳ�޹㲥�Ķ�ʱ��
	short _select_counter;		//���ڽ��в�ѯ�����Ƿ���ڵļ�����
	bool  _debug_command_enable;	//���������Ƿ񼤻�
	char  _banish_counter;
	unsigned short _move_cmd_seq;
	short  _deny_cmd_list[CMD_MAX];
	short  _pickup_counter;
	int    _pickup_timestamp;
	int    _safe_lock_timestamp;	//��ʣ�೤ʱ�䰲ȫ����,����ʱ��Ϊʱ��ʱ��Ч
	int    _max_safe_lock;			//��ȫ������ʱ��,-1Ϊ���� >0Ϊʱ��
	GNET::Privilege * _gm_auth;
	enum 
	{
		MAX_PICKUP_PER_SECOND = 5, //���ÿ��������
		STD_PICKUP_PER_SECOND = 2, //ƽ��ÿ��������
	};
private:
	//���������ֹ������õ�Ŀ���� ��ֹ�������Ӷ��ص���Ҫ��������ݽṹ��ʹ�ø��ֽṹ�����һ��
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);


	inline bool CheckBanish()
	{
		_load_stats ++;
		if(_banish_counter >0)
		{
			_load_stats += (_banish_counter >> 2) + 2;
			return false;
		}
		return true;
	}
	
	inline void IncreaseLoadStat(int offset)
	{
		_load_stats += offset;
	}

	inline bool CheckDeny(unsigned int  type)
	{
		return _deny_cmd_list[type];
	}
public:
	inline bool xCheckDeny(unsigned int  type) { return _deny_cmd_list[type]; }
	inline const XID & GetCurTarget() { return _cur_target;}
	void SetPrivilege(const void * data, unsigned int size);
	inline void SetDebugMode(bool is_debug)
	{
		_debug_command_enable = is_debug;
	}
	
	inline bool GetDebugMode()
	{
		return _debug_command_enable;
	}
	
	inline bool GetGMPrivelege()
	{
		return (_gm_auth);
	}
	
	
	virtual bool HasGMPrivilege();

	inline void SetSafeLock(int locktime, int max_locktime)
	{
		_safe_lock_timestamp = locktime;
		_max_safe_lock= max_locktime;
		_imp->_runner->notify_safe_lock( InSafeActive(), _safe_lock_timestamp, _max_safe_lock, InSafeLock() );
	}

	inline void UpdateSafeLock()
	{
		_imp->_runner->notify_safe_lock( InSafeActive(), _safe_lock_timestamp, _max_safe_lock, InSafeLock() );
		return;
	}

	bool InSafeLock();
	char InSafeActive();

	inline bool TestSafeLock()
	{
		if(InSafeLock())
		{
			error_cmd(S2C::ERR_FORBIDDED_OPERATION_IN_SAFE_LOCK);
			return false;
		}
		return true;
	}
public:
	DECLARE_SUBSTANCE(gplayer_controller);

	gplayer_controller();
	~gplayer_controller();

	virtual void Init(gobject_imp * imp);
	virtual void ReInit();

	virtual int GetCurMoveSeq() 
	{ 
		return _move_cmd_seq;
	}

	virtual int GetNextMoveSeq() 
	{ 
		_move_cmd_seq = (_move_cmd_seq + 1 ) & 0xFFFF;
		return _move_cmd_seq;
	}

	virtual void SetNextMoveSeq(int seq) 
	{ 
		_move_cmd_seq = seq;
	}

	virtual int MessageHandler(world * pPlane, const MSG & msg);

	virtual int DebugCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int GMCommandHandler(int cmd_type,const void * buf, unsigned int size);
	//����״̬�µ����������
	virtual int UnLockInventoryHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int CommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int ZombieCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int SealedCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int StayInCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int MarketCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int TravelCommandHandler(int cmd_type,const void * buf, unsigned int size);
	virtual int CosmeticCommandHandler(int cmd_type, const void * buf, unsigned int size);
	virtual int BoundCommandHandler(int cmd_type, const void * buf, unsigned int size);

	virtual int MoveBetweenSlice(gobject * obj,slice * src, slice * dest);
	virtual void SwitchSvr(int dest, const A3DVECTOR & oldpos,const A3DVECTOR &newpos,const instance_key *switch_key);
	virtual void Release(bool free_parent);	//������������free_parent
	virtual	void OnHeartbeat(unsigned int tick);
	virtual void DenyCmd(unsigned int cmd_type);
	virtual void AllowCmd(unsigned int cmd_type);

	void ResurrectByItem(float exp_reduce, int param);
	void ResurrectInTown(float exp_reduce, int param);
    void ResurrectByCash(float exp_reduce, int param);

public:
	void OnResurrect();
protected:
	void TryPeepMobs();
	void error_cmd(int msg);
	int cmd_user_move(const void * buf, unsigned int size);
	int cmd_user_stop_move(const void * buf, unsigned int size);
	friend class gplayer_imp;
	
public:
	virtual	void LoadFrom(gplayer_controller * rhs)		//��ԭ�е�controller��ȡ������
	{
		_cur_target = rhs->_cur_target;
		_cur_sub_target = rhs->_cur_sub_target;
		_target_refresh_state = rhs->_target_refresh_state;
		_last_pos = rhs->_last_pos;
		_peep_counter = rhs->_peep_counter;
		_select_counter = rhs->_select_counter;
		_load_stats = rhs->_load_stats;
		_debug_command_enable = rhs->_debug_command_enable;
		_banish_counter = rhs->_banish_counter;
		_move_cmd_seq = rhs->_move_cmd_seq;
		_safe_lock_timestamp = rhs->_safe_lock_timestamp;
		_max_safe_lock =  rhs->_max_safe_lock;
		
		memcpy(_deny_cmd_list,rhs->_deny_cmd_list,sizeof(_deny_cmd_list));

		ASSERT(!_gm_auth);
		_gm_auth = rhs->_gm_auth;
		rhs->_gm_auth = NULL;
	}
	
	//�ڲ����õĺ���
	template < typename T>
	inline void TrySelect(const T & attacker)
	{
		if(_cur_target.type == -1)
		{
			SelectTarget(attacker.id);
		}
	}

	void UnSelect(bool force=false);
	void SelectTarget(int id, bool force=false);
	void SubscibeConfirm(const XID & who);

	void SelectSubTarget(const XID & who, int id);
	void SecondSubscibeConfirm(const XID & who);
};


class player_trade;
class player_stall;
class gplayer_imp : public gactive_imp 
{
protected:
	typedef abase::static_multimap<int, int, abase::fast_alloc<> >  ENEMY_LIST;
	instance_key	_instance_switch_key;	//���и���ת��ʱʹ�õ�key
	item_list 	_inventory;		//��Ʒ��
	item_list	_equipment;		//װ����
	item_list	_task_inventory;	//������Ʒ��
	gplayer_money _money;
	//unsigned int 		_player_money[2];		//money //silver 172
	//unsigned int		_money_capacity;	//Ǯ������		
	int		_player_state;		//��ҵĻ���״̬
	int		_combat_timer;		//�������һ�θ񶷵�ʱ��
	int		_reputation;		//����
	player_team	_team;			//��������
	player_duel	_duel;			//��������
	player_bind	_bind_player;		//��Ҽ��
	player_invade	_invade_ctrl;		//���������������ݣ�����״̬�ĸ�����
	player_trade  * _trade_obj;		//���׶���ֻ�н���״̬����Ч
	player_stall  * _stall_obj;		//��̯����ֻ�а�̯��ʱ�����Ч
	int		_stall_trade_id;	//��̯����������У���̯����ȷ��
	int		_stall_trade_timer;	//��¼�ϴΰ�̯��ʱ����������в����ܰ�̯ ����Ҫ���л��б���
	struct stall_info _stall_info;	//̯λ�����������Ƴ���
	int		_last_move_mode;	//��һ�ε��ƶ�ģʽ
	ENEMY_LIST _enemy_list;			//�����б������Լ�������Щ����ĳ�޶��б�ͷ��
	int		_pvp_cooldown;		//pvp��ȴʱ��
	int		_ap_per_hit;		//һ�ι���������ŭ��ֵ
	breath_ctrl	_breath;
	phase_control _ph_control;	//�������ƶ��ĺϷ��ԣ�������������������
	int		_db_user_id;		//���ݿ���Ҫ������id
	struct
	{
		uint64_t mask;
		abase::octets data;
	} _equip_info;				//װ����Ϣ ����Ҫ������֪����װ�����ͣ��������ͻ�����ʾ��

	struct
	{
		XID  id;
		A3DVECTOR pos;			//�������Ҳ������ʱ��������ʱ��Ŀ���
		int id_mafia;
	} _provider;				//��ǰ���Լ��ṩ��������˵������id  

	unsigned char _inv_level;		//�������ĵȼ����Ǳ仯�� ,���ֵ�����˰������������ͽ�Ǯ��������� 
	bool	_is_moved;			//��gnpc_controllerʹ�õ�_is_moved
	bool	_trash_box_open_flag;		//��Ʒ��򿪱�־
	bool	_trash_box_open_view_only_flag;		//��Ʒ��򿪱�־
	bool	_user_trash_box_open_flag;	//�ʺ���Ʒ��򿪱�־
	bool 	_security_passwd_checked;	//��ȫ����(�ֿ�����)���ͨ����־
	bool	_pvp_enable_flag;		//�Ƿ�����PVP��־
	char	_force_attack;			//�ͻ��˷�����ǿ�ƹ�����־���ڼ��ܵĴ��й����Ļ���Ч����ʹ��
	char 	_refuse_bless;			//�ͻ��˷����Ĳ�����ף����־
	unsigned char _write_counter;		//�ڱ��η�������д��Ĵ��� Ŀǰûʲô��
	bool	_kill_by_player;		//�Ƿ����ɱ����
	bool	_free_pvp_mode;			//�Ƿ��ھ�����ģʽ  ������л�������ʱ������
	bool	_nonpenalty_pvp_state;	//�Ƿ����޳ͷ�PVP״̬����״̬��ɱ�˲�����������������
	bool    _no_cooldown_mode;		//�Ƿ���cooldownģʽ  ������ ���л�
	unsigned short _eq_change_counter;	//װ�����ı�־��ÿ�θ���װ�����ֵ���һ�����Ϊ0���ʾ�Ѿ�д��
	unsigned short _tb_change_counter;	//��Ʒ��д���־��ÿ�δ����ӻ��һ�����Ϊ0���ʾ�Ѿ�д��
	unsigned short _user_tb_change_counter;	//�ʺ���Ʒ��д���־��ÿ�δ����ӻ��һ�����Ϊ0���ʾ�Ѿ�д��

	float	_resurrect_exp_reduce;		//ԭ�ظ���ʱ���ٵ�exp��Ŀ
	float 	_resurrect_hp_factor;		//ԭ�ظ����Ѫ������
	float 	_resurrect_mp_factor;		//ԭ�ظ����������
	int		_resurrect_exp_lost_reduce;	//���з�ʽ�������ʧ���ٰٷֱȣ���������ʹ��
	int 	_con_emote_target;		//����Эͬ�������Ķ���
	int	_pvp_combat_timer;		//�Ƿ����pvp�� �Լ�pvp�е�ʣ��ʱ��(��)
	unsigned short _con_emote_id;		//����Эͬ�����Ķ���id

	bool	_resurrect_state;		//�Ƿ����ԭ�ظ���ı�־
	unsigned char _logout_pos_flag;		//�Ƿ�ʹ����������ǳ�
	A3DVECTOR _logout_pos;			//�ǳ�ʱʹ�õ�����
	int	  _logout_tag;			//�ǳ�ʱʹ�õ�tag

	A3DVECTOR _last_instance_pos;		//���һ�ν��븱���������λ�� ���豣��
	int	  _last_instance_tag;		//���һ�ν��븱���������ID   ���豣��
	int 	  _last_instance_logout_ts; //���һ���˳�������ʱ���     ���豣��	
	int 	  _last_instance_timestamp;	//���һ�ν��븱����ʱ���     ���豣��
	A3DVECTOR _last_source_instance_pos;	//���һ�ν��븱���������λ�� ���豣��
	int	  _last_source_instance_tag;	//���һ�ν��븱���������ID   ���豣��
	int	  _db_save_error;		//���ݴ��̴������
	int	  _db_timestamp;		//���ݿ���̼������������Ҫ
	char	  _username[MAX_USERNAME_LENGTH];	//��ҵ����֣����10��unicode�ַ�
	unsigned int	  _username_len;		//������ֵĳ��ȣ����ֽ�Ϊ��λ
	
	int	  _double_exp_timeout;		//˫������ʱ�䵽��ʱ�� ��಻�ܳ�������Сʱ
	int	  _double_exp_mode;		//˫��ʱ������� 0 �ޣ�1 �� 2 ��Ʒ 3 ����
	int	  _rest_counter_time;		//�ϴμ���˫��ʱ��ļ�ʱ��
	int 	  _rest_time_used;		//����ʱ�������Ѿ�ʹ�õļ�ʱ��
	int	  _rest_time_capacity;		//Ŀǰ���ö���˫��ʱ��
	int	  _mafia_rest_time;		//���ɽ�����˫��ʱ�� ���ʱ��������ʱ������������տ��õ�˫��ʱ��
	int	  _mafia_rest_counter_time;	//�ϴμ�����ɽ�����ʱ���
	int	  _login_timestamp;		//��¼ʱ��ʱ���
	int	  _played_time;			//����ɫ�Ѿ�������Ϸ��ʱ��
	int   _last_login_timestamp;	//�ϴε�½��ʱ���
	int	  _create_timestamp;		//��ɫ����ʱ��
	int	  _spec_task_reward;		//���ݿⷢ��
	int	  _spec_task_reward2;		//���ݿⷢ��
	int	  _spec_task_reward_param;	//���ݿⷢ���Ĳ���
	int	  _spec_task_reward_mask;	//�µ����ݿⷢ������һ��mask
	float	  _speed_ctrl_factor;		//�����ٶȵ�ͳ�����ӣ������ǵ�ǰ����ù������Ϸ��ٶȴ�ֵ�������
	int	  _duel_target;			//����ʱ��Ŀ�꣬���͸�����ʹ�õ�
	bool	  _inv_switch_save_flag;	//���л�����ʱ��¼�Ƿ����뿪������ʧ��Ʒ�ı�־���д˱�־���л���ɺ�ͻ���ͻ��˸��°�������
	bool	  _eqp_switch_save_flag;	//ͬ��
	bool	  _tsk_switch_save_flag;	//ͬ��

	abase::vector<unsigned short, abase::fast_alloc<> > _waypoint_list;	//�Ѽ���·����б�
	cd_manager _cooldown;
	pet_manager _petman;			//�������
	plant_pet_manager _plantpetman;	//ֲ�����,��ֲ���л���������ʧ�����Բ��豣��

	
	elf_info _cur_elf_info;			//��ǰװ����С������Ϣ��//lgc
	int _min_elf_status_value;		//�������д���ת��״̬��С��������Сstatus_valueֵ
	
	int       _mall_cash;                   //ԭ���еĿ��õ���
	int       _mall_cash_used;              //�ܹ�ʹ���˶��ٵ���
	int       _mall_cash_offset;            //�����˶��ٵ���
	int 	  _mall_cash_add;				//��ʷ�ۼƳ�ֵ����,����������ʹ��
	int       _mall_order_id;               //�̳��������ˮ��
	int       _mall_order_id_saved;         //�̳��������ˮ�ţ����һ�δ��̵���ˮ��
	abase::vector<netgame::mall_invoice, abase::fast_alloc<> > _mall_invoice;
	int		  _mall_consumption;			//����ֵ

	char	  _cheat_punish;		//���׳ͷ�
	char	  _cheat_mode;			//���ױ�־
	char 	  _cheat_report;		//�Ƿ���delivery�㱨������
	char	  _chat_emote;			//������� ID 
	int	  _auto_hp_value;		//�Զ���Ѫ״̬��ֵ
	float	  _auto_hp_percent;
	int	  _auto_mp_value;		//�Զ���ħ״̬��ֵ
	float	  _auto_mp_percent;

	int _equip_refine_level[item::EQUIP_INVENTORY_COUNT];//����װ���ľ����ȼ� 
	int _soul_power;									 //���ݾ����ȼ���������Ļ���
	int _soul_power_en;									//�������������Ļ���ֵ
	int _min_addon_expire_date;		//�Ƿ���������޵ĸ������ԣ����ﱣ�����һ�ε��ڵĵ���ʱ��
	pet_enhance _pet_enhance;	//�ٻ�����Ի�ȡһ���������ٻ��ߵ�����,�������ܸı��ֵ
	player_limit _player_limit;
	XID _skill_attack_transmit_target;
	int _country_expire_time;	//���ҹ���ʱ���
	bool _in_central_server;	//�Ƿ��ڿ���������,���ݵ�½ʱflag����
	int _src_zoneid;			//����ڿ�����������ʾ�����ĸ�������
	int _king_expire_time;		//��������ʱ���
	touch_trade _touch_order;       //Touch���� 
	player_title _player_title;    // ��ɫ�ƺ�
	player_dailysign _player_dailysign; // �ճ�ǩ��
	player_giftcard	_player_giftcard;  // ��Ʒ��
	player_fatering _player_fatering;	//����
    player_sanctuary_check _player_sanctuary_check; // �����Ұ�ȫ��״̬
	player_instance_reenter _player_instance_reenter; // ������½��븱�� ����save load
	player_clock _player_clock;  // ������ˢ������ṩʱ������
	//--> ��Ҫ����ˢ�����˳������
	//<--
	random_mall_info _player_randmall;// ����̳�����

	bool _need_refresh_equipment;	//װ�����Ƿ���Ҫˢ�� �л�����ʱ���豣��
	unsigned char _realm_level;		// ����ȼ�
	int _realm_exp;					// ���羭��
	int _leadership;
	int _leadership_occupied;
	int _world_contribution;
	int _world_contribution_cost;
	unsigned char _astrolabe_extern_level;
	int _astrolabe_extern_exp;

	cash_vip_info _cash_vip_info;
	purchase_limit_info _purchase_limit_info;

	//��λ����
	fix_position_transmit_info _fix_position_transmit_infos[FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT];
	int _fix_position_transmit_energy;
    int _cash_resurrect_times_in_cooldown;      // ����ȴ�ڼ�ʹ��Ԫ������Ĵ���

	//NEW PARMS
	int _bind_state;

	//autoassist
	bool auto_assist;
	int check_assist_next_time;

	//shield
	float _shield_energy;
	float _shield_energy_gen;
	short _shield_energy_max;

	//hwid
	unsigned long long _hwid;
	//pet time increase
	int _increase_pet_time;
	int _real_weapon_class;
	int _fake_weapon_class;

	arena_invite_tm _inviting_tm;

// nw
	int _charge_merc;	// no save 
	int _charge_merc_time;	// no save
	const ADDON_LIST * _repository_addons[12]; // no save

public:
	struct ADDONS_ARG
	{
		ADDON_LIST _total_addon[8];
	};
	ADDONS_ARG _kids_addons[6];

protected:
	int _dungeon_999_timer;	// no save
	bool _check_interface;	// no save
	bool _check_genesis_lvl;	// no save
	bool _check_codex_get_storage;	// no save
	int _kid_transformation;
	int _kid_transformation_time;
	struct
	{
		int saved_skill_id[16];
		int saved_skill_level[16];      // level cũ của player (để restore qua SetLevel/Remove)
		int saved_kid_skill_level[16];  // level từ Celestial (j) - dùng cho packet client
		int saved_count;
		int saved_idx;
		int saved_slot;
		// Phase 2: tất cả buff dùng filter TTL — không cần manual restore
		// (filter persist qua _filters.Save/Load và auto-cleanup khi hết TTL)
	} _kid_transform_skill_state;
	//int _question_day;
	//int _question_started;
	//int _question_mode;
	//int _question_progress;
	//int _question_answer;
	//int _question_correct;
	//int _question_time;

	// storage 1
	
	//int roleid; // default key save/load
	//int pk_time; // default pk save
	//int pk_status; // default pk save
	//int money_silver; // player_money
	
	int _color_name;
	int _pvp_rank;
	int _pvp_rank_exp;
	int _player_kill;  
	int _monster_kill;  
	int _player_death;
	int _monster_death;
	int _diary_exp;
	int _realm_day_verify;
	int _verify_itens_valid;
	bool _has_astrolabe_lock;
	bool _enabled_fashion_weapon;

	// storage 2

	bool _double_factor_exp;
	bool _double_factor_sp;
	bool _double_factor_realm;

	// storage data 1

	autoswap_data _autoswap;
	skill_sender_data _skillsender;
	glyph_data _glyph;
	carrier_data _carrier;
	repository_data _repository;
	gplayer_treasure _treasure;
	gplayer_lottery _lottery;
	gplayer_lottery_items _treasure_items;
	gplayer_lib_items _lib_items;
	gplayer_safe_lock_passwd _passwd_safe;
	pet_skill_temp_data _pet_skill_temp;
	pet_skin_data _pet_skin;
	day_world_points_data _day_world_points;
	
	// storage data 2
	
	activity_data _activity;
	gplayer_celestial _celestial;
	
// nw end		
	
	gplayer_lua _glua;
	gplayer_codex _codex;
	gplayer_kid _kid;

	gplayer_kid_addons _kid_addon;
	//gplayer_newstorage _newstorage;
	//gplayer_newstorage2 _newstorage2;
	friend class gplayer_controller;
	friend class gplayer_dispatcher;
public:


/*
	Player ״̬
*/
	enum
	{
		PLAYER_STATE_NORMAL,		//ͨ��״̬
		PLAYER_WAITING_TRADE,		//����״̬ 
		PLAYER_TRADE,			//����״̬ 
		PLAYER_WAIT_TRADE_COMPLETE,	//����״̬ 
		PLAYER_WAIT_TRADE_READ,		//����״̬
		PLAYER_WAITING_FACTION_TRADE,	//����״̬ 
		PLAYER_WAIT_LOGOUT,		//������ڵȴ��ǳ�
		PLAYER_DISCONNECT,		//player �Ѿ����ߣ������ڵȴ��˳���ʱ��
		PLAYER_WAIT_SWITCH,		//player �ڵȴ��л�����������
		PLAYER_SIT_DOWN,		//player �������µ�״̬
		PLAYER_STATE_MARKET,		//player ���ڰ�̯��״̬
		PLAYER_STATE_TRAVEL,		//player �������е�״̬
		PLAYER_STATE_COSMETIC,		//player �������ݵ�״̬ 
		PLAYER_STATE_BIND,		//player ��������״̬
		PLAYER_WAIT_FACTION_TRADE_READ, //player ���ڰ��ɽ��׳�ʱ����ͼ��ȡ���ݵĵȴ�״̬
	};

/*
	�ͻ���������ʾ����
*/
	enum
	{
		CLIENT_SCREEN_EFFECT,
		CLIENT_GFX_EFFECT,
	};

	DECLARE_SUBSTANCE(gplayer_imp);
	int 	_disconnect_timeout;	//����ʱ�ĳ�ʱ����
	int 	_offline_type;		//���ߵ����ͣ��ǳ������߻����߳�
	int	_write_timer;		//д��ʹ�õļ�ʱ��
	int	_link_notify_timer;	//��link server���������Ķ�ʱ��
	int 	_general_timeout;	//ͨ�õĳ�ʱ����Ŀǰֻ�н���ʱʹ��
	float 	_fall_counter;
	int	 _wallow_level;		//���Եȼ�
//	wallow_object _wallow_obj;	//����ʱ����ƶ���

	int _profit_time;		//ʣ������ʱ��(��λΪ��,>0,������ʱ��;=0������ʱ��)
	int _profit_level;		//���漶��,������ʱ��ת���õ�
	int _profit_timestamp;	//����ʱ���,Ϊ�����0��ʱ���
	int _active_state_delay;//��������ͼ2,ս�����ƶ�ʱ����

	bool	_level_up;

	struct 
	{
		int error_counter;
	} move_checker;
	
	int DecMoveCheckerError(int offset)
	{
		if((move_checker.error_counter -= offset) < 0)
		{
			move_checker.error_counter = 0;
		}
		return 	move_checker.error_counter;
	}

	int IncMoveCheckerError(int offset)
	{
		return 	(move_checker.error_counter += offset);
	}

	void ClrMoveCheckerError()
	{
		move_checker.error_counter  = 0;
	}

	//������ص�����
	unsigned int _task_mask;
	abase::vector<char> _active_task_list;
	abase::vector<char> _finished_task_list;
	abase::vector<char> _finished_time_task_list;
	abase::vector<char>	_finish_task_count_list;
	abase::vector<char>	_storage_task_list;

	//������(��ɫ������)�������
	abase::vector<unsigned char> _role_reputation_uchar;
	abase::vector<unsigned short> _role_reputation_ushort;
	abase::vector<unsigned int> _role_reputation_uint;

	//����key��ص�����
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>  _cur_ins_key_list;
	abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>  _team_ins_key_list;
	abase::vector<abase::pair<int,int> >  _cur_tag_counter;
	abase::vector<abase::pair<int,int> >  _ins_key_timer;		//��¼����key��ʹ��ʱ�䣬����ˢ����

	//�ֿ�ṹ
	player_trashbox  _trashbox;
	//�ʺŲֿ⣬ͬ�ʺ��µ����н�ɫ���ɲ���
	player_trashbox  _user_trashbox;

	//�����̳��������
	dividend_mall_info _dividend_mall_info;
	
	multi_exp_ctrl _multi_exp_ctrl;	//�౶�������
	faction_contrib _faction_contrib; //���ɹ��׶�
	std::unordered_map<int/*factionid*/,int> _faction_alliance; 	//����ͬ��
	std::unordered_map<int/*factionid*/,int> _faction_hostile;		//���ɵж�

	enum CONGREGATE_TYPE
	{
		CONGREGATE_TYPE_TEAM,
		CONGREGATE_TYPE_FACTION,
		CONGREGATE_TYPE_TEAM_MEMBER,
	};	
	struct congregate_req
	{
		char type;
		int sponsor;
		int timeout;
		int world_tag;
		A3DVECTOR pos;
	};
	abase::vector<congregate_req> _congregate_req_list;
	player_force _player_force;
	force_ticket_info _force_ticket_info;
	online_award _online_award;
	switch_additional_data * _switch_additional_data;	//���л�����ǰ����Ҫ�õ�������
	meridian_manager _meridianman; //��ҵľ���ϵͳ
	player_reincarnation _player_reincarnation;			//ת��
	abase::bitmap<GENERALCARD_MAX_COLLECTION> _generalcard_collection;//�����ռ�ͼ��
	playersolochallenge _solochallenge;

    struct mnfaction_info_t
    {
        int64_t unifid;
    };

    struct visa_info_t
    {
        int type;
        int stay_timestamp;
		int cost;
		int count;
    };

    mnfaction_info_t _player_mnfaction_info;
    visa_info_t _player_visa_info;


	//��ҵ��������ͣ�������˸�link���ͺ���Э��
	enum
	{
		PLAYER_OFF_LOGOUT,
		PLAYER_OFF_OFFLINE,
		PLAYER_OFF_KICKOUT,
		PLAYER_OFF_LPG_DISCONNECT,
		PLAYER_OFF_CHANGEDS,
	};

	//��Ʒ�б��Ķ��壬��Ҫ���ںͿͻ��˵���ϵ
	enum
	{
		IL_INVENTORY,		// 0 - Inventário
		IL_EQUIPMENT,		// 1 - Equipamento
		IL_TASK_INVENTORY,	// 2 - Missões
		IL_TRASH_BOX,		// 3 - Banqueiro
		IL_TRASH_BOX2,		// 4 - Armazém de Recursos
		IL_TRASH_BOX3,		// 5 - Roupas
		IL_USER_TRASH_BOX,	// 6 - Armazém Comum
		IL_TRASH_BOX4,		// 7 - Inventário de Cartas do Usuário
		IL_TRASH_BOX5,		// 8 - Inventário de Runas
		IL_TRASH_BOX6,		// 9 - Inventário de Vôo
		IL_TRASH_BOX7,		// 10 - ???
		IL_TRASH_BOX8,		// 11 - ???

		IL_MAX,

		IL_INVENTORY_BEGIN = IL_INVENTORY,
		IL_INVENTORY_END = IL_TASK_INVENTORY,
		IL_TRASH_BOX_BEGIN = IL_TRASH_BOX,
		IL_TRASH_BOX_END = IL_TRASH_BOX8,
	};

	//��ҿ����͹ر�pvp״̬��ԭ��
	enum
	{
		PLAYER_PVP_CLIENT,		//�ͻ�����������,������ر�pvp״̬(������PVE������)
		PLAYER_PVP_PROTECTED,	//��ҽ�����������ֱ�������
	};

	//����pvp���������
	enum
	{
		AWARD_MAFIAPVP_MINECAR = 1, 	// ����pvp�󳵽���
		AWARD_MAFIAPVP_MINEBASE,		// ����pvp���ؽ���
		AWARD_MAFIAPVP_MINECAR_ARRIVED,	// �󳵵���
		AWARD_MAFIAPVP_HIJACK_KILL,		// �Ӷ��߻�ɱ
		AWARD_MAFIAPVP_MINECAR_PROTECT, // �󳵱���
		AWARD_MAFIAPVP_NO_OWNER_MINECAR,// �޹����󳵽���
		AWARD_MAFIAPVP_NO_OWNER_MINEBASE,// �޹������ؽ���
	};

    enum
    {
        PRODUCE_INHERIT_REFINE = 0x0001,
        PRODUCE_INHERIT_SOCKET = 0x0002,
        PRODUCE_INHERIT_STONE = 0x0004,
        PRODUCE_INHERIT_ENGRAVE = 0x0008,
        PRODUCE_INHERIT_ADDON = 0x0010,
    };

public:
	gplayer_imp();
	~gplayer_imp();
public:
	inline gplayer * GetParent() { return (gplayer*)_parent; }
	inline int GetPlayerClass() { return ((gplayer*)_parent)->base_info.race  & 0x7FFFFFFF;}
	virtual int GetObjectClass() { return GetPlayerClass();}			//ȡ�ö����ְҵ
	inline bool IsPlayerFemale() { return ((gplayer*)_parent)->base_info.race < 0;}
	inline void SetPlayerClass(int cls, bool gender) 
	{ 
		gplayer * pPlayer = GetParent();
		if(gender)
		{
			pPlayer->base_info.race  = cls | 0x80000000;
            GetParent()->object_state2 |= gactive_object::STATE_PLAYER_GENDER;
		}
		else
		{
			pPlayer->base_info.race  = cls;
		}
	}

	inline void GetPlayerClass(int & cls,bool & gender)
	{
		int race = ((gplayer*)_parent)->base_info.race;
		cls = race & 0x7FFFFFFF;
		gender = race & 0x80000000;
	}

	inline void SetPlayerName(const void  * name, unsigned int size)
	{
		if(size > MAX_USERNAME_LENGTH) size = MAX_USERNAME_LENGTH;
		memcpy(_username,name,size);
		_username_len = size;
	}

	inline const void  * GetPlayerName(unsigned int & len)
	{
		len = _username_len;
		return _username;
	}
	
	inline const void  * GetPlayerName2()
	{
		return _username;
	}

	inline int GetDBUserId(){ return _db_user_id; }

	inline void MallSaveDone(int id)
	{
		_mall_order_id_saved = id;
		unsigned int i = 0;
		for(i = 0;i  < _mall_invoice.size(); i ++)
		{
			const netgame::mall_invoice & mi =  _mall_invoice[i];
			if(mi.order_id - id >= 0) break;
		}

		if(i == _mall_invoice.size())
		{
			_mall_invoice.clear();
		}
		else if( i > 0)
		{
			_mall_invoice.erase(_mall_invoice.begin(), _mall_invoice.begin() + i);
		}
	}
	
	inline void SetMallInfo(int cash, int cash_used, int delta, int cash_add, int order_id)
	{
		_mall_cash = cash;
		_mall_cash_used = cash_used;
		_mall_cash_offset = delta;
		_mall_cash_add = cash_add;
		_mall_order_id = order_id;
		_mall_order_id_saved = order_id;
	}

	inline void SetMallConsumption(int consumption)
	{
		_mall_consumption = consumption;
	}

	inline void GetMallInfo(int & cash,int & cash_used, int & delta, int & order_id)
	{
		cash = _mall_cash;
		cash_used = _mall_cash_used;
		delta = _mall_cash_offset;
		order_id = _mall_order_id;
	}

	inline int GetMallCash()
	{
		return _mall_cash + _mall_cash_offset;
	}

	inline void AddMallCash(int cash)
	{
		_mall_cash_offset += cash;
		++_mall_order_id;
	}

	inline void RemoveMallCash(int cash)
	{
		if(LuaManager::GetInstance()->GetConfig()->activity_event_enable > 0)
		GetKidAddons()->SetCashHistoryUsed(cash);
		_activity.event_cash_history = cash;
		_mall_cash_offset -= cash;
	}

	inline int GetMallCashAdd()
	{
		return _mall_cash_add;	
	}

	inline int GetMallConsumption() const
	{
		return _mall_consumption;
	}

	inline bool NeedSaveMallInfo()
	{
		return _mall_order_id != _mall_order_id_saved;
	}

	inline void DeliveryNotifyCash(int cash_plus_used)
	{
		_mall_cash = cash_plus_used - _mall_cash_used;
		_runner->player_cash(GetMallCash());
	} 
	
	inline void DeliveryNotifyModifyCashUsed(int cash_used)
	{
		//����gamedbd�޸���cash_used������һ��Ҫ����gamedbd������ˮ�ż�1��Ȼ��������gs��
		_mall_order_id++;
		_mall_cash = _mall_cash - (cash_used - _mall_cash_used);   
		_mall_cash_used = cash_used;
		_mall_cash_offset = 0;
		_runner->player_cash(GetMallCash());
	}

	int GetCashVipLevel() 
	{ 
		return _cash_vip_info.GetCalcVipLevel();
	}

	// Solicita os pontos
	inline int GetPointsEventTask(int uTaskID) 
	{ 
		return _glua.GetEventTaskCompletePoints(uTaskID);
	}

	// Adiciona uma nova task
	inline void SetEventTaskComplete(int uTaskID) 
	{ 
		if(uTaskID > 0)
		_glua.SetEventTaskComplete(uTaskID);
	}

	// Adiciona pontos a uma task
	inline void SetEventTaskCompleteAddPoints(int uTaskID, short uPoints) 
	{ 
		if(uTaskID > 0)
		_glua.SetEventTaskCompleteAddPoints(uTaskID, uPoints);
	}

	// Limpa as tasks
	inline void SetFreeTaskComplete(int uTaskID) 
	{
		if(uTaskID > 0)
		_glua.SetFreeTaskComplete(uTaskID);
	}


	int GetCashVipScore()
	{
		return _cash_vip_info.GetCurScore();
	}

	bool CheckVipService(int type);

	inline void GetBasicData(int &level,int & sec_level,int &exp,int &sp,int &hp,int &mp, int & pp,int &money, unsigned int & db_user_id)
	{
		player_sec_level sec;
		sec.level = _basic.sec.level;
		sec.flag = _basic.sec.flag;
		
		level = _basic.level;
		sec_level = sec.value;
		exp = _basic.exp;
		sp = _basic.skill_point;
		hp = _basic.hp;
		mp = _basic.mp;
		pp = _basic.status_point;

		money = _money.GetGold();
		db_user_id = _db_user_id;
	}

	inline void SetBasicData(int level,int sec_level,int exp,int sp,int hp,int mp,int pp,int money,int db_user_id)
	{
		player_sec_level sec;
		sec.value = sec_level;
		
		_basic.level = level;
		_basic.sec.level = sec.level;
		_basic.exp = exp;
		_basic.skill_point = sp;
		_basic.hp = hp;
		_basic.mp = mp;
		_basic.status_point = pp;
		_basic.sec.flag = sec.flag;

		_money.SetGold ( money );
		_db_user_id = db_user_id;
	}

	//����ԭ�еĺ���
	//Ŀǰ��״̬����ֻ��player�ڲ�ʹ��
	inline void ActiveCombatState(bool state) 
	{
		if(state != _combat_state) 
		{
			SetRefreshState();		
			_combat_state = state;
			if(!_combat_state)
			{
				SetAttackMonster(false);
				GetParent()->object_state2 &= ~gactive_object::STATE_IN_COMBAT;	
			}
			else
				GetParent()->object_state2 |= gactive_object::STATE_IN_COMBAT;
			
			_runner->player_active_combat(_combat_state);
		}

		if(_combat_state)
		{
			//����ս��ʱ����ʱ���������
			SetActiveStateDelay(MAX_ACTIVE_STATE_DELAY);
		}
	} 
	
	inline bool IsDeliverLegal()
	{
		return _player_state == PLAYER_STATE_NORMAL || _player_state == PLAYER_SIT_DOWN;
	}

	inline int GetPlayerState()
	{
		return _player_state;
	}

	inline gplayer_lua & GetStorageLua() { return _glua; }

	inline item_list & GetInventory(){ return _inventory;}
	inline item_list & GetEquipInventory(){ return _equipment;}
	inline item_list & GetTaskInventory(){ return _task_inventory;}
	inline player_trashbox & GetTrashBox() { return _trashbox;}
	inline player_trashbox & GetUserTrashBox() { return _user_trashbox;}
	inline item_list & GetInventory(int where)
	{
		switch(where)
		{
			case IL_INVENTORY:
			default:
				return _inventory;
			case IL_EQUIPMENT:
				return _equipment;
			case IL_TASK_INVENTORY:
				return _task_inventory;
		}
	}

	inline item_list & GetTrashInventory(int where)
	{
		switch(where)
		{
			case IL_TRASH_BOX:
			default:
				return _trashbox.GetBackpack(0);
			case IL_TRASH_BOX2:
				return _trashbox.GetBackpack(1);
			case IL_TRASH_BOX3:
				return _trashbox.GetBackpack(2);
			case IL_TRASH_BOX4:
				return _trashbox.GetBackpack(3);
			case IL_TRASH_BOX5:
				return _trashbox.GetBackpack(4);
			case IL_TRASH_BOX6:
				return _trashbox.GetBackpack(5);
			case IL_TRASH_BOX7:
				return _trashbox.GetBackpack(6);
			case IL_TRASH_BOX8:
				return _trashbox.GetBackpack(7);
			case IL_USER_TRASH_BOX:
				return _user_trashbox.GetBackpack(0);
		}
	}

	inline bool IsPortableTrashBox(int where)	//Is it a portable warehouse
	{
		return where == IL_TRASH_BOX3 || where == IL_TRASH_BOX4 || where == IL_TRASH_BOX5 || where == IL_TRASH_BOX6;
	}
	
	// count_consumption: �Ƿ���Ҫ��������ֵ ����ֵ: ʵ��ɾ��������Ʒ
	inline unsigned int RemoveItems(int type, unsigned int num, int drop_type, bool count_consumption)
	{
		unsigned int old_num = num;
		int rst = 0;
		while(num && (rst = _inventory.Find(rst,type)) >= 0)
		{
			unsigned int count = num;
			if(_inventory[rst].count < count) count = _inventory[rst].count;

			if (count_consumption)
			{
				item& it = _inventory[rst];
				UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
			}
			
			_inventory.DecAmount(rst,count);
			_runner->player_drop_item(IL_INVENTORY,rst,type,count,drop_type);
			num -= count;
			rst ++;
		}
		return old_num - num;
	}

	inline bool InventoryIsFull(int item_type) { return _inventory.HasSlot(item_type);} 
	inline bool InventoryHasSlot(unsigned int count) { return count <= _inventory.GetEmptySlotCount();} 
	inline bool IsItemExist(unsigned int inv_index, int type, unsigned int count) { return _inventory.IsItemExist(inv_index,type,count); }
	inline bool IsItemExist(int where, unsigned int index, int type, unsigned int count)
	{
		switch(where)
		{
			case IL_INVENTORY:
				return _inventory.IsItemExist(index,type,count);
			case IL_EQUIPMENT:
				return _equipment.IsItemExist(index,type,count);
			case IL_TASK_INVENTORY:
				return _task_inventory.IsItemExist(index,type,count);
		}
		return false;
	}
	
	inline bool IsItemExist(int type)
	{
		if(type == 0 || type == -1) return false;
		return _inventory.Find(0,type) >= 0;
	}
	
	inline bool IsItemCanSell(unsigned int inv_index, int type, unsigned int count)
	{
		return _inventory.IsItemExist(inv_index,type,count) && 
			!(_inventory[inv_index].proc_type & item::ITEM_PROC_TYPE_NOSELL);
	}

	inline bool IsItemNeedRepair(int where, unsigned int index, int type)
	{
		ASSERT(type != -1);
		item_list & inv = GetInventory(where);
		if(!inv.IsItemExist(index,type,1)) return false;
		int d,md;
		inv[index].GetDurability(d,md);
		return (md >0 && md > d);
	}

	inline int GetItemCount(int item_id)
	{
		int start = 0;
		int count = 0;
		while((start = _inventory.Find(start, item_id)) >= 0)
		{
			count += _inventory[start].count;
			start ++;
		}
		return count;
	}

	inline int GetItemPos(int item_id)
	{
		return  _inventory.Find(0,item_id);
	}

	inline int GetPKCount()
	{
		return _invade_ctrl._kill_count;
	}

	inline int GetPVPCoolDown()
	{
		return _pvp_cooldown;
	}

	inline bool GetPVPFlag()
	{
		return _pvp_enable_flag;
	}

	inline char GetDeadFlag()
	{
		if(!_parent->IsZombie())  return false;
		return _kill_by_player?player_var_data::KILLED_BY_PLAYER:player_var_data::KILLED_BY_NPC;
	}

	inline void SetDeadFlag(char d)
	{
		if(d)
		{
			_parent->b_zombie = true;
			if(d == player_var_data::KILLED_BY_PLAYER) 
				_kill_by_player = true;
			else 
				_kill_by_player = false;
		}
	}

	inline bool GetResurrectState(float & exp_reduce, float & hp_factor, float & mp_factor)
	{
		if(!_parent->IsZombie()) return false;
		exp_reduce = _resurrect_exp_reduce;
		hp_factor = _resurrect_hp_factor;
		mp_factor = _resurrect_mp_factor;
		return _resurrect_state;
	}

	inline void SetResurrectState(bool state, float  exp_reduce, float hp_factor, float mp_factor)
	{
		if(!_parent->IsZombie()) return;
		if(exp_reduce <0.f) exp_reduce = 0.f;
		if(exp_reduce >1.0f) exp_reduce = 1.0f;
		if(hp_factor <0.f) hp_factor = 0.f;
		if(hp_factor >1.0f) hp_factor = 1.0f;
		if(mp_factor <0.f) mp_factor = 0.f;
		if(mp_factor >1.0f) mp_factor = 1.0f;
		_resurrect_state = state;
		_resurrect_exp_reduce = exp_reduce;
		_resurrect_hp_factor = hp_factor;
		_resurrect_mp_factor = mp_factor;
	}

	inline void SetPVPState(int pkcount, int pvp_cooldown, bool pvp_flag)
	{
		_invade_ctrl._kill_count = pkcount;
		_pvp_cooldown = pvp_cooldown;
		//����Ǻ��������ǿ�ƿ���PK����
		if(_invader_state != INVADER_LVL_0) pvp_flag = true;
		_pvp_enable_flag = pvp_flag;
		if(pvp_flag)
		{
			GetParent()->object_state |= gactive_object::STATE_PVPMODE;
		}
	}

	inline void GetInvadeState(int &invader_state, int &invader_time, int &pariah_time)
	{
		ASSERT(_invader_state == _invade_ctrl._invader_state);
		invader_state = _invade_ctrl._invader_state;
		invader_time = _invade_ctrl._invader_time;
		pariah_time  = _invade_ctrl._pariah_time;
	}

	inline void SetInvadeState(int invader_state, int invader_time, int pariah_time) 
	{
		_invader_state = invader_state & 0x03;
		_invade_ctrl.SetState(_invader_state,invader_time,pariah_time);
		_invade_ctrl.SetParentState<0>(_invader_state);
	}

	inline unsigned int GetGold()
	{
		return _money.GetGold();
	}

	inline unsigned int GetSilver()
	{
		return _money.GetSilver();
	}	
	
	inline long long GetAllMoney()
	{
		return _money.GetMoney();
	}
	
	/*
	inline unsigned int GetMoney(bool is_silver)
	{
		return is_silver ? _player_money[1] : _player_money[0];
	}
	*/

/*
	inline void IncMoney(unsigned int inc)
	{
		unsigned int newmoney = _money + inc;
		ASSERT((int)_money >= 0);
		if((int)newmoney < (int)_money || newmoney > _money_capacity)
		{
			//���������
			newmoney = (unsigned int)-1;
			unsigned int delta = _money_capacity - _money;
			inc -= delta;
			_money = _money_capacity; 
			DropMoneyItem(_plane,_parent->pos,inc,_parent->ID,0,0);
		}
		else
		{
			_money = newmoney;
		}
	}

	inline void DecMoney(unsigned int offset)
	{
		ASSERT(offset <= _money);
		_money -= offset;
	}
*/
	inline bool CanUseService()
	{
		//�����жϸ�Ϊֻ��npc����
		return _provider.id.id != -1 
			&& (!_provider.id_mafia || _provider.id_mafia == GetParent()->id_mafia);
	}


	inline void ClearTrahsBoxWriteFlag()
	{
		_tb_change_counter = 0;
	}

	inline bool IsTrashBoxChanged()
	{
		return _tb_change_counter;
	}

	inline void IncTrashBoxChangeCounter()
	{
		_tb_change_counter ++;
		if(_tb_change_counter == 0) _tb_change_counter = 1;
	}

	inline void ClearUserTrahsBoxWriteFlag()
	{
		_user_tb_change_counter = 0;
	}

	inline bool IsUserTrashBoxChanged()
	{
		return _user_tb_change_counter;
	}

	inline void IncUserTrashBoxChangeCounter()
	{
		_user_tb_change_counter ++;
		if(_user_tb_change_counter == 0) _user_tb_change_counter = 1;
	}

	inline void IncEquipChangeFlag()
	{
		_eq_change_counter ++;
		if(_eq_change_counter == 0)
		{
			_eq_change_counter = 1;
		}
	}


	inline bool IsBled()
	{
		//�Ժ�Ҫ��������״̬�ļ���
		return _basic.hp < _cur_prop.max_hp ||_basic.mp < _cur_prop.max_mp;
	}

	inline void Renew()
	{
		_basic.hp = _cur_prop.max_hp;
		_basic.mp = _cur_prop.max_mp;
		SetRefreshState();
		_filters.ClearSpecFilter(filter::FILTER_MASK_DEBUFF);
		_runner->renew();
	}

	inline void EnterStayInState()
	{
		ASSERT(_player_state == PLAYER_STATE_NORMAL);
		_player_state = PLAYER_SIT_DOWN;
		//����player��Ѫ���ظ�״̬

		ObjectSitDown();
		//Ҫ�������µ���Ϣ��������
		_runner->sit_down();
	}

	inline void LeaveStayInState()
	{
		ASSERT(_player_state == PLAYER_SIT_DOWN);
		_player_state = PLAYER_STATE_NORMAL;
		//����player��Ѫ���ظ�״̬

		ObjectStandUp();
		_runner->stand_up();

		//������µ�filter
		_filters.RemoveFilter(FILTER_INDEX_SITDOWN);
	}
	
	inline player_team & GetTeamCtrl() { return _team;}
	inline bool IsTeamLeader() { return _team.IsLeader();}
	inline bool IsMember(const XID & member) { return _team.IsMember(member);}
	inline bool IsInTeam() { return _team.IsInTeam();}
	inline bool IsAutoComposingTeam() { return _team.IsAutoComposingTeam(); }
	inline const XID & GetTeamLeader() { return _team.GetLeader();}
	inline int GetTeamMemberNum() { return _team.GetMemberNum(); }
	inline int GetTeamID()
	{
		if(IsInTeam()) 
			return _team.GetLeader().id;
		else
			return 0;
	}
	inline int GetTeamSeq() 
	{ 
		if(IsInTeam()) 
			return _team.GetTeamSeq();
		else
			return -1;
	}
	inline const player_team::member_entry & GetTeamMember(int index) {return _team.GetMember(index);}
	inline void TeamChangeLeader(int id) { return _team.CliChangeLeader(XID(GM_TYPE_PLAYER,id));}
	inline int GetMemberList(XID * list) { return _team.GetMemberList(list);}
	inline void SendTeamData(const XID & leader,unsigned int team_count,unsigned int data_count, 
			const player_team::member_entry ** list)
	{
		((gplayer_dispatcher*)_runner)->send_team_data(leader,team_count,data_count,list);
	}

	inline void SendTeamData(const XID & leader,unsigned int team_count,
			const player_team::member_entry * list)
	{
		((gplayer_dispatcher*)_runner)->send_team_data(leader,team_count,list);
	}
	
	inline void ReceiveTaskExp(int exp,int sp) 		//����Ӿ������
	{
		if(exp < 0) exp = 0;
		if(sp  < 0) sp = 0;

		float double_exp_sp_factor = 1.0f;
		//ת���������
		double_exp_sp_factor += _player_reincarnation.GetExpBonus();

        double_exp_sp_factor += _exp_sp_factor;
        if (double_exp_sp_factor <= 0.0f) return;

		IncExp(exp,sp,double_exp_sp_factor,false);
		_runner->task_deliver_exp(exp,sp);
		//С�����þ���
		if(exp/10 > 0)
			ElfReceiveExp((unsigned int)(exp/10));
	}

	inline void ReceiveCommonExp(int exp, int sp)
	{
		if(exp < 0) exp = 0;
		if(sp  < 0) sp = 0;
		IncExp(exp,sp);
		_runner->receive_exp(exp,sp);
	}	

	inline void SetConcurrentEmote(int target, unsigned char emote)
	{
		_con_emote_target = target;
		_con_emote_id = emote;
	}

	inline void ClearConcurrentEmote()
	{
		_con_emote_target = 0;
		_con_emote_id = 0;
	}

	inline void SetReputation(int rep)
	{
		_reputation = rep;
	}

	inline int GetReputation()
	{
		return _reputation;
	}

	inline void SetWaypointList(const void * buf, unsigned int size)
	{
		if((size & 0x01) || size > MAX_WAYPOINT_COUNT*sizeof(unsigned short)) 
		{
			ASSERT(false);
			return ;
		}
		unsigned int count = size / 2;
		if(!count) return;
		_waypoint_list.reserve(count);
		unsigned short * list = (unsigned short*)buf;
		for(unsigned int i = 0; i < count; i ++)
		{
			_waypoint_list.push_back(list[i]);
		}
	}

	inline bool IsWaypointActived(unsigned short wp )
	{
		for(unsigned int i = 0; i < _waypoint_list.size(); i ++)
		{
			if(wp == _waypoint_list[i]) return true;
		}
		return false;
	}

	inline void ActivateWaypoint(unsigned short wp)
	{
		for(unsigned int i = 0; i < _waypoint_list.size(); i ++)
		{
			if(wp == _waypoint_list[i]) return ;
		}
		_waypoint_list.push_back(wp);
		_runner->activate_waypoint(wp);
		return ;
	}
	inline const void * GetWaypointBuffer(unsigned int & size)
	{
		size = _waypoint_list.size() * sizeof(unsigned short);
		return _waypoint_list.begin();
	}

	inline void ClearExpiredCoolDown()
	{
		_cooldown.ClearExpiredCoolDown();
	}

	//ֻȡ���Լ��ĸ���KEY
	inline void GetSelfInstanceKey(int world_tag, instance_key & key)
	{
		key.essence.key_level1 = _parent->ID.id;
		key.essence.key_level3 = ((gplayer*)_parent)->id_mafia;
		key.essence.key_level4 = 0;

		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
		it = _cur_ins_key_list.find(world_tag);
		if(it == _cur_ins_key_list.end())
		{
			//ֻ�д�����͹��������Ż�����  ���Զ���һ������
			key.essence.key_level2.first = key.essence.key_level2.second = 0;
		}
		else
		{
			key.essence.key_level2.first = it->second.first;
			key.essence.key_level2.second = it->second.second;
		}
	}

	inline void GetInstanceKey(int world_tag, instance_key & key)
	{
		key.essence.key_level1 = _parent->ID.id;
		key.essence.key_level3 = ((gplayer*)_parent)->id_mafia;
		key.essence.key_level5 = GetCountryId();

		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc>::iterator it;
		abase::static_multimap<int ,abase::pair<int,int> ,abase::default_alloc> * pMap;
		if(_team.IsInTeam())
		{
			pMap = &_team_ins_key_list; 
			it = _team_ins_key_list.find(world_tag);
		}
		else
		{
			pMap = &_cur_ins_key_list; 
			it = _cur_ins_key_list.find(world_tag);
		}

		if(it == pMap->end())
		{
			//ֻ�д�����͹��������Ż�����  ���Զ���һ������
			key.essence.key_level2.first = key.essence.key_level2.second = 0;
		}
		else
		{
			key.essence.key_level2.first = it->second.first;
			key.essence.key_level2.second = it->second.second;
		}
	}

	inline void SetLastInstancePos(int tag, const A3DVECTOR &pos, int timestamp)
	{
		_last_instance_pos = pos;
		_last_instance_tag = tag;
		_last_instance_timestamp = timestamp;
	}

	inline void SetLastInstanceSourcePos(int tag, const A3DVECTOR &pos)
	{
		_last_source_instance_pos = pos;
		_last_source_instance_tag = tag;
	}

	inline void GetLastInstancePos(int &tag, A3DVECTOR &pos, int & timestamp)
	{
		pos = _last_instance_pos;
		tag = _last_instance_tag;
		timestamp = _last_instance_timestamp;
	}

	inline void GetLastInstanceSourcePos(int &tag, A3DVECTOR &pos)
	{
		pos = _last_source_instance_pos;
		tag = _last_source_instance_tag;
	}

	inline void SetPVPCombatState()
	{
		if(!_pvp_combat_timer)
		{
			_runner->active_pvp_combat_state(true);
			GetParent()->object_state |= gactive_object::STATE_IN_PVP_COMBAT;
		}
		_pvp_combat_timer = PVP_COMBAT_HIGH_TH;
	}

	inline void PVPCombatHeartbeat()
	{
		if(!_pvp_combat_timer) return;
		if((--_pvp_combat_timer) <= 0)
		{
			_runner->active_pvp_combat_state(false);
			GetParent()->object_state &= ~gactive_object::STATE_IN_PVP_COMBAT;
			_pvp_combat_timer = 0;
		}
	}

	inline bool IsInPVPCombatStateHigh()
	{
		return _pvp_combat_timer > PVP_COMBAT_LOW_TH;
	}

	inline bool IsInPVPCombatState()
	{
		return _pvp_combat_timer;
	}

	inline int GetPVPCombatTimer()
	{
		return _pvp_combat_timer;
	}

	inline void CalcRestTime()
	{
		world_manager::CalcRestTime(_rest_counter_time,_rest_time_used,_rest_time_capacity,
				GetParent()->id_mafia, _mafia_rest_time, _mafia_rest_counter_time);
	}

	inline bool TestRestTime(int t)
	{
		return false;
		//��Ѱ�û��˫��������񣬶�ʹ����Ʒ���
		//return _rest_time_used + t <= _rest_time_capacity + _mafia_rest_time;
		
	}

	inline void SetRestTimeParam(int dbl_timeout,int  dbl_mode,int  rest_counter_time,int rest_time_used,int rest_time_cap)
	{
		_double_exp_timeout	= dbl_timeout;
		_double_exp_mode	= dbl_mode;
		_rest_counter_time	= rest_counter_time;
		_rest_time_used		= rest_time_used;
		_rest_time_capacity	= rest_time_cap;
		if(_double_exp_mode)
		{
			int t = g_timer.get_systime();
			if(t >= _double_exp_timeout) 
			{
				_double_exp_mode = 0;
			}
		}
		
	}
	
	void SetExtRestParam(const void * buf, unsigned int size);
	void GetExtRestParam(archive & ar);
	
	inline int GetDoubleExpTime()
	{
		if(_double_exp_mode)
		{
			return _double_exp_timeout - g_timer.get_systime();
		}
		else
		{
			return 0;
		}
	}

	inline int GetDoubleExpAvailTime()
	{
		return _mafia_rest_time + _rest_time_capacity - _rest_time_used;
	}

	inline void GetRestTimeParam(int &dbl_timeout,int  &dbl_mode,int  &rest_counter_time,int &rest_time_used,int &rest_time_cap)
	{
		dbl_timeout 		=_double_exp_timeout;
		dbl_mode 		=_double_exp_mode;
		rest_counter_time 	=_rest_counter_time;
		rest_time_used 		=_rest_time_used;
		rest_time_cap 		=_rest_time_capacity;
	}

	inline void SetPlayEd(int timestamp, int playtime)
	{
		_login_timestamp = timestamp;
		_played_time = playtime;
	}

	inline void SetWallowData(const void * buf, unsigned int len)
	{
	/*	time_t t = g_timer.get_systime();
		_wallow_obj.Init(t, buf, len);
		if(world_manager::AntiWallow())
		{
			_wallow_level = _wallow_obj.Tick(t);
		}
		else
		{
			_wallow_level = 0;
		}*/
	}
	
	inline int GetPlayEd()
	{
		return _played_time + (g_timer.get_systime() - _login_timestamp);
	}

	inline void SetLastLoginTime(int lastlogin)
	{
		_last_login_timestamp = lastlogin;	
	}

	inline void SetCreateTime(int createtime)
	{
		_create_timestamp = createtime;	
	}
	
	inline int GetLastLoginTime()
	{
		return _last_login_timestamp;	
	}

	inline int GetCreateTime()
	{
		return _create_timestamp;	
	}

	inline void GetSpecailTaskAward(unsigned int & id,unsigned int & id2,unsigned int &param , unsigned int& mask)
	{
		id = _spec_task_reward;
		id2 = _spec_task_reward2;
		param = _spec_task_reward_param;
		mask = _spec_task_reward_mask;
	}
	
	inline void SetSpecailTaskAward(int id,int id2,int param, int rewardmask)
	{
		_spec_task_reward = id;
		_spec_task_reward2 = id2;
		_spec_task_reward_param = param;
		_spec_task_reward_mask = rewardmask;
	}
	inline bool IsUnderWater(float offset = 0.0f)
	{
		return _breath.IsUnderWater(offset);
	}
	
	int TransformChatData(const void * data,unsigned int dsize, void * out_buffer, unsigned int len);
	int PhaseControl(const A3DVECTOR & target, float terrain_height, int mode, int use_time);
	
	inline void SetSecLevel(unsigned int per)
	{
		_basic.sec.level = per & 0xFF;
		
		if ( _basic.sec.level >= 1 && _basic.sec.level <= 8 )
		{
			_basic.sec.flag = 0;
		}
		
		if ( _basic.sec.level >= 20 && _basic.sec.level <= 22 )
		{
			_basic.sec.flag = 1;
		}
		
		if ( _basic.sec.level >= 30 && _basic.sec.level <= 32 )
		{
			_basic.sec.flag = 2;
		}
		
		SetRefreshState();
		GetParent()->sec_level = _basic.sec.level;
		_runner->task_deliver_level2(_basic.sec.level);
	}

	inline void SetDBTimeStamp(int db_timestamp)
	{
		_db_timestamp = db_timestamp;
	}

	inline void SetCombatTimer(int new_timer)
	{
		if(_combat_timer < new_timer)
		{
			_combat_timer = new_timer;
		}
	}
	
	inline void GetDBDailySignin(int& uptime, int& monthcal, int& curryear, int& lastyear, char& latesignintimes, char& awardedtimes, char& awardedtimes2)
	{
		_player_dailysign.SaveToDB(uptime, monthcal, curryear, lastyear, latesignintimes, awardedtimes, awardedtimes2);
	}
	
	inline void SetDBDailySignin(int uptime, int monthcal, int curryear, int lastyear, char latesignintimes, char awardedtimes, char awardedtimes2)
	{
		_player_dailysign.InitFromDB(uptime, monthcal, curryear, lastyear, latesignintimes, awardedtimes, awardedtimes2);
	}
	
	inline void GetDBTouchTrade(int64_t &sn,char &state,unsigned int& cost,int &type,unsigned int &count,int& expiretime,unsigned int &lots )
	{
		_touch_order.GetData(sn,state,cost,type,count,expiretime,lots);
	}

	inline void SetDBTouchTrade(int64_t sn,char state,unsigned int cost,int type,unsigned int count,int expiretime,unsigned int lots )
	{
		_touch_order.InitData(sn,state,cost,type,count,expiretime,lots,_parent->ID.id);
	}
	
	inline void GetDBGiftCard(char& state,int& type,int& parenttype,char (&cardnumber)[player_giftcard::GIFT_CARDNUMBER_LEN] )
	{
		_player_giftcard.GetData(state,type,parenttype,cardnumber);
	}

	inline void SetDBGiftCard(char state,int type,int parenttype,const char (&cardnumber)[player_giftcard::GIFT_CARDNUMBER_LEN] )
	{
		_player_giftcard.InitData(state,type,parenttype,cardnumber);
	}

	inline void GetDBClockData(archive & ar)
	{
		_player_clock.SaveToDB(ar);
	}

	inline void SetDBClockData(archive & ar)
	{
		_player_clock.InitFromDB(ar,_parent->ID.id);
	}
	
	inline void GetDBRandMallData(archive & ar)
	{
		_player_randmall.SaveToDB(ar);
	}

	inline void SetDBRandMallData(archive & ar)
	{
		_player_randmall.InitFromDB(ar,_parent->ID.id);
	}
	
	inline void GetDBTitleData(archive & ar)
	{
		_player_title.SaveToDB(ar);
	}

	inline void InitDBTitleData(archive & ar)
	{
		_player_title.InitFromDB(ar);
	}

	inline void InitDBTitleEnhance()
	{
		_player_title.CalcAllTitleEnhance(false);
	}

	inline void SetDBRealmData(int level,int exp)
	{
		_realm_level = level;
		_realm_exp = exp;
		
		SetVigourBase(player_template::GetRealmVigour(_realm_level));
		
		if(_realm_level)
		{
			GetParent()->realmlevel = _realm_level;	
			GetParent()->object_state2 |= gactive_object::STATE_REALMLEVEL;
		}
	}

	inline void GetDBRealmData(int& level,int& exp)
	{
		level = _realm_level;
		exp = _realm_exp;
	}

 	inline void GetDBSoloChallengeInfo(GDB::base_info::solo_challenge_info_t &solo_challenge_info)
	{
		_solochallenge.GetDBSoloChallengeInfo(solo_challenge_info);
	}
	inline void SetDBSoloChallengeInfo(const GDB::base_info::solo_challenge_info_t &solo_challenge_info)
	{
		_solochallenge.SetDBSoloChallengeInfo(solo_challenge_info);
	}

	inline void InitInstanceReenter(int tag,int type,instance_hash_key key,int timeout,A3DVECTOR pos)
	{
		_player_instance_reenter.LoadFromDB(tag,type,key,timeout,pos);
	}

	inline player_title& GetPlayerTitle() { return _player_title;}
	inline player_clock& GetPlayerClock() { return _player_clock;}

	inline void GetDBFateRingData(archive & ar)
	{
		_player_fatering.SaveToDB(ar);
	}

	inline void InitDBFateRingData(archive & ar)
	{
		_player_fatering.InitFromDB(ar);
	}

	inline void InitDBFateRingEnhance()
	{
		_player_fatering.EnhanceAll();
	}

	inline bool IsMarried()
	{
		return GetParent()->spouse_id != 0;
	}

	inline int GetSpouse()
	{
		return GetParent()->spouse_id;
	}

	inline void SetSpouse(int id)
	{
		//��������������͹㲥
		gplayer * pPlayer = GetParent();
		if(id == 0)
		{
			pPlayer->object_state &= ~gactive_object::STATE_SPOUSE;
			pPlayer->spouse_id = 0;
		}
		else
		{
			pPlayer->spouse_id = id;
			pPlayer->object_state |= gactive_object::STATE_SPOUSE;
		}
	}

	inline void ExternSaveDB()
	{
		if(_player_state == PLAYER_STATE_NORMAL)
		{
			AutoSaveData();
		}
		else
		{
			//������������������
			ASSERT(false);
		}
	}

	void EnablePVPFlag(char type);
	/*
	{
		if(!_pvp_enable_flag)
		{
			GetParent()->object_state |= gactive_object::STATE_PVPMODE;
			_pvp_enable_flag = true;
			_pvp_cooldown = X_PVP_STATE_COOLDOWN;
			_runner->enable_pvp_state(type);
			_runner->player_pvp_cooldown(_pvp_cooldown);
		}
	}
	*/
	void DisablePVPFlag(char type);
	/*
	{
		if(_pvp_enable_flag)
		{
			GetParent()->object_state &= ~gactive_object::STATE_PVPMODE;
			_pvp_enable_flag = false;
			_pvp_cooldown = 0;
			_runner->disable_pvp_state(type);
			_runner->player_pvp_cooldown(_pvp_cooldown);
		}
	}
	*/
	inline void SetSecurityPasswdChecked(bool b)
	{
		_security_passwd_checked = b;
		if(b) 
			_runner->security_passwd_checked();
	}

	inline float GetCurSpeed()
	{
		float speed[]={_cur_prop.run_speed,_cur_prop.flight_speed,_cur_prop.swim_speed,_cur_prop.run_speed}; 
		int index = _layer_ctrl.GetLayer(); 
		if(index == 2 && !IsUnderWater(2.0f)) return speed[0]; //����ˮ�в�����Ӿ
		return speed[index];	
	}
	
	void UpdateEquipRefineLevel()
	{
		memset(_equip_refine_level, 0, sizeof(_equip_refine_level));
		for(unsigned int i=0; i<item::EQUIP_INVENTORY_COUNT; i++)
		{
			item& it = _equipment[i];
			if(it.type <= 0 || it.body == NULL || !it.body->IsActive()) continue;
			int material_need;
			int refine_addon = world_manager::GetDataMan().get_item_refine_addon(it.type, material_need);
			if(material_need <= 0 || refine_addon <= 0) continue;
			_equip_refine_level[i] = it.body->GetRefineLevel(refine_addon);
			
			if( _equip_refine_level[i] < 0  )
				_equip_refine_level[i] = 0  ;
			if( _equip_refine_level[i] > 12 )
				_equip_refine_level[i] = 12 ;			
		}
		UpdateBaseSoulPower();
	}
	
	void UpdateBaseSoulPower()
	{
		const int refine_cost[13] = {0,1,5,15,40,100,230,445,850,1600,2970,5495,10140};
		_soul_power = _basic.level*_basic.level + 50*_basic.level;
		_soul_power += int(refine_cost[_equip_refine_level[0]] * 1.5f);
		for(unsigned int i=1; i<item::EQUIP_INVENTORY_COUNT; i++)
		{ 
			_soul_power += int(refine_cost[_equip_refine_level[i]]*0.25f);	
		}
		__PRINTF("����:%d\n",_soul_power + _soul_power_en);
	}

	bool CheckEquipRefineLevel(int count ,int level)
	{
		for(unsigned int i=0; i<item::EQUIP_INVENTORY_COUNT; i++)
		{
			if(_equip_refine_level[i] >= level)
			{
				if(--count <= 0) return true;
			}
		}
	
		return false;
	}

	inline int GetFactionConsumeContrib()
	{
		return _faction_contrib.consume_contrib;
	}

	inline int GetFactionExpContrib()
	{
		return _faction_contrib.exp_contrib;
	}

	inline int GetFactionCumulateContrib()
	{
		return _faction_contrib.cumulate_contrib;
	}

	inline bool IncFactionContrib(int consume, int exp)
	{
		int tmp1 = _faction_contrib.consume_contrib + consume;
		int tmp2 = _faction_contrib.exp_contrib + exp;
		int tmp3 = _faction_contrib.cumulate_contrib + consume;
		if(tmp1 < _faction_contrib.consume_contrib || tmp2 < _faction_contrib.exp_contrib || tmp3 < _faction_contrib.cumulate_contrib)
			return false;
		_faction_contrib.consume_contrib = tmp1;
		_faction_contrib.exp_contrib = tmp2;
		_faction_contrib.cumulate_contrib = tmp3;
		_runner->faction_contrib_notify();
		return true;
	}
	
	inline void DecFactionContrib(int consume, int exp)
	{
		_faction_contrib.consume_contrib -= consume;
		if(_faction_contrib.consume_contrib < 0) _faction_contrib.consume_contrib = 0;
		_faction_contrib.exp_contrib -= exp;
		if(_faction_contrib.exp_contrib < 0) _faction_contrib.exp_contrib = 0;
		_runner->faction_contrib_notify();
	}

	inline int GetDisabledEquipMask(){ return ((gplayer*)_parent)->disabled_equip_mask;}

	//Ԥ���ӿ�,ͨ������ȡ������ʱ������
	inline void SetProfitTime(int time) {_profit_time = time;}
	inline void SetActiveStateDelay(int delay)
	{
		if(world_manager::ProfitTimeLimit2() && _active_state_delay == 0)
		{
			_runner->notify_profit_state(1);
		}
		_active_state_delay = delay;
	}
	inline void UpdateProfitTime()
	{
		if(_profit_time > 0)
		{
			//���������ͼ
			_profit_time--;
			int level = player_template::GetProfitLevel(_profit_time);
			if(level != _profit_level)
			{
			 	_profit_level = level;
				_runner->update_profit_time(S2C::CMD::player_profit_time::PROFIT_LEVEL_CHANGE, _profit_time, _profit_level);
			}
		}
	}
	inline void CalcProfitLevel()
	{
		if(!world_manager::GetInstance()->ProfitMap())
		{
			_profit_level = PROFIT_LEVEL_NORMAL;
			return;
		}

		int level = player_template::GetProfitLevel(_profit_time);
		if(level != _profit_level)
		{
			_profit_level = level;
		}
	}
	inline void SaveProfitTime(archive & ar)
	{
		ar << _profit_time << _profit_timestamp;
	}
	inline void LoadProfitTime(archive & ar)
	{
		time_t now = g_timer.get_systime();
		struct tm *tm_now = localtime(&now);
		ASSERT(tm_now);

		if(ar.size() == 2*sizeof(int))
		{
			ar >> _profit_time >> _profit_timestamp;
			if(now - _profit_timestamp >= TOTAL_SEC_PER_DAY)
			{
				//����ʱ�����,��������ʱ��
				_profit_time = world_manager::GetWorldConfig().profit_time;
				_profit_timestamp = now - tm_now->tm_hour*3600 - tm_now->tm_min*60 - tm_now->tm_sec;
			}
		}
		else
		{
			//DB������ʱ������,�״�����
			_profit_time = world_manager::GetWorldConfig().profit_time;
			_profit_timestamp = now - tm_now->tm_hour*3600 - tm_now->tm_min*60 - tm_now->tm_sec;
		}
	}

	inline int GetCountryId(){	return ((gplayer*)_parent)->country_id &0xffff; }
	inline int GetCountryGroup() {  return (((gplayer*)_parent)->country_id >> 16) &0xffff; }
	inline int GetCountryExpireTime(){ return _country_expire_time; }
	void SetCountryId(int country_id, int expire_time)
	{
		gplayer * pPlayer = (gplayer*)_parent;
		pPlayer->country_id = country_id;
		if(pPlayer->country_id)
		{
			pPlayer->object_state |= gactive_object::STATE_COUNTRY;
			_country_expire_time = expire_time;
		}
		else
		{
			pPlayer->object_state &= ~gactive_object::STATE_COUNTRY;
			_country_expire_time = 0;
		}	
		_runner->player_country_changed((pPlayer->country_id&0xffff));
	}
	void SetCountryData(int country_id, int expire_time)
	{
		gplayer * pPlayer = (gplayer*)_parent;
		if(country_id && !(expire_time && g_timer.get_systime() >= expire_time))
		{
			pPlayer->country_id = country_id;
			pPlayer->object_state |= gactive_object::STATE_COUNTRY;
			_country_expire_time = expire_time;
		}
	}
	inline bool InCentralServer(){ return _in_central_server; }
	inline void SetInCentralServer(bool b, int src_zoneid)
	{ 
		if(b)
		{
			_in_central_server = true;
			_src_zoneid = src_zoneid;
		} else 
		{
			_in_central_server = false;
			_src_zoneid = src_zoneid;
		}
	}
	inline int GetKingExpireTime(){ return _king_expire_time; }
	void SetKing(bool is_king, int expire_time)
	{
		gplayer * pPlayer = (gplayer*)_parent;
		if(is_king)
		{
			pPlayer->object_state2 |= gactive_object::STATE_KING;
			_king_expire_time = expire_time;
		}
		else
		{
			pPlayer->object_state2 &= ~gactive_object::STATE_KING;
			_king_expire_time = 0;
		}
		_runner->player_king_changed(is_king ? 1 : 0);
	}
	void SetKingData(int is_king, int expire_time)
	{
		gplayer * pPlayer = (gplayer*)_parent;
		if(is_king && !(expire_time && g_timer.get_systime() >= expire_time))
		{
			pPlayer->object_state2 |= gactive_object::STATE_KING;
			_king_expire_time = expire_time;
		}
	}
	int Get16Por9JWeapon()		//0-��ͨ 1-16Ʒ 2-9��
	{
		int rst = _equipment[item::EQUIP_INDEX_WEAPON].Is16Por9JWeapon();
		if(rst == 2) return rst;
		for(unsigned int i=0; i<_inventory.Size(); i++)
		{
			int tmp = _inventory[i].Is16Por9JWeapon();
			if(tmp > rst)
			{
				rst = tmp;
				if(rst == 2) return rst;	
			}
		}
		return rst;	
	}
	void ClearSwitchAdditionalData()
	{
		if(_switch_additional_data)
		{
			delete _switch_additional_data;
			_switch_additional_data = NULL;
		}
	}
	void UpdateReincarnation(unsigned int times, bool notify_client)
	{
		gplayer * pPlayer = GetParent();
		if(times)
		{
			pPlayer->object_state2 |= gactive_object::STATE_REINCARNATION;	
			pPlayer->reincarnation_times = times & 0xFF;
		}
		else
		{
			pPlayer->object_state2 &= ~gactive_object::STATE_REINCARNATION;
			pPlayer->reincarnation_times = 0;
		}
		if(notify_client) _runner->player_reincarnation(times);	
	}
	inline unsigned int GetReincarnationTimes(){ return _player_reincarnation.GetTimes(); }

	inline int GetRealmLevel() const { return _realm_level;}
	inline void SetDBLeadership(int v){ _leadership = v; }
	inline int GetDBLeadership(){ return _leadership; }
	inline void IncLeadership(int v)
	{
		_leadership += v;
		_runner->player_leadership(_leadership, v);
	}
	inline void SetDBWorldContrib(int wc,int wcc) { _world_contribution = wc; _world_contribution_cost = wcc;}
	inline void GetDBWorldContrib(int& wc,int& wcc) { wc = _world_contribution; wcc = _world_contribution_cost; }
	inline void IncWorldContrib(int wc)
	{
		_world_contribution += wc;
		if(wc < 0)	_world_contribution_cost -= wc;
		//_runner->player_world_contribution(_world_contribution, wc, _world_contribution_cost);
	}
	inline void ClearWorldContrib()
	{
		_world_contribution = 0;
		_world_contribution_cost = 0;
		//_runner->player_world_contribution(0,0,0);
	}
	
	inline bool _IsArenaMember(const XID& member)
	{
		return IsArenaMember(member);
	}


	inline void SetDBAstrolabeExtern(unsigned char level, int exp) 
	{
		_astrolabe_extern_level = level;
		_astrolabe_extern_exp = exp;
	}
	inline void GetDBAstrolabeExtern(unsigned char& level, int& exp) 
	{
		level = _astrolabe_extern_level;
		exp = _astrolabe_extern_exp;
	}

	int  GetAstrolabeExternLevel() { return (int)_astrolabe_extern_level;}
	bool IncAstrolabeExternExp(int exp);

    void SetDBMNFactionInfo(int64_t unifid);
    inline void GetDBMNFactionInfo(int64_t& unifid)
    {
        unifid = _player_mnfaction_info.unifid;
    }
	inline int64_t GetMNFactionID() const
	{
		return _player_mnfaction_info.unifid;
	}

    inline void SetDBVisaInfo(int type, int stay_timestamp, int cost, int count)
    {
        _player_visa_info.type = type;
        _player_visa_info.stay_timestamp = stay_timestamp;
		_player_visa_info.cost = cost;
		_player_visa_info.count = count;
    }

    inline void GetDBVisaInfo(int& type, int& stay_timestamp, int& cost, int& count)
    {
        type = _player_visa_info.type;
        stay_timestamp = _player_visa_info.stay_timestamp;
		cost = _player_visa_info.cost;
		count = _player_visa_info.count;
    }

	inline int GetSrcZoneId()
	{
		return _src_zoneid;
	}

    void SetDBFixPositionTransmit(archive & ar);
	void GetDBFixPositionTransmit(archive & ar);

    inline void SetDBCashResurrectTimesInCoolDown(int times)
    {
        _cash_resurrect_times_in_cooldown = times;
    }

    inline void GetDBCashResurrectTimesInCoolDown(int& times)
    {
        times = _cash_resurrect_times_in_cooldown;
    }

	void GetDBCashVipInfo(int &vip_level, int &score_add, int &score_cost, int &score_consume)
	{
		_cash_vip_info.GetCashVipInfo(vip_level, score_add, score_cost, score_consume);
	}
	void SetDBCashVipInfo(int vip_level, int score_add, int score_cost, int score_consume)
	{
		gplayer * pPlayer = (gplayer*)(_parent);
		_cash_vip_info.SetCashVipInfo(vip_level, score_add, score_cost, score_consume, pPlayer);
	}
	void GetDBPurchaseLimitInfo(archive & ar)
	{
		_purchase_limit_info.GetPurchaseLimitMapInfo(ar);
	}
	void SetDBPurchaseLimitInfo(archive & ar)
	{
		if(0 != ar.size())
			_purchase_limit_info.SetPurchaseLimitMapInfo(ar);
	}

	void GetDBPurchaseLimitData(int &day_stamp, int &week_stamp, int &month_stamp, int &year_stamp)
	{
		_purchase_limit_info.GetPurchaseLimitInfo(day_stamp, week_stamp, month_stamp, year_stamp);
	}

	void SetDBPurchaseLimitInfo(int day_stamp, int week_stamp, int month_stamp, int year_stamp)
	{
		_purchase_limit_info.SetPurchaseLimitInfo(day_stamp, week_stamp, month_stamp, year_stamp);
	}
	
	int GetFixPositionCount()
	{
		int count = 0;
		for(int i = 0; i < FIX_POSITION_TRANSMIT_MAX_POSITION_COUNT; ++i)
		{
			if(_fix_position_transmit_infos[i].index != -1)
				++count;
		}
		return count;
	}

	cash_vip_info &GetCashVipInfo()
	{
		return _cash_vip_info;
	}

	purchase_limit_info &GetPurchaseLimit()
	{
		return _purchase_limit_info;
	}
protected:
	template <typename MESSAGE,typename ENCHANT_MSG>
	inline bool TestHelpfulEnchant(const MESSAGE & msg, ENCHANT_MSG & emsg)
	{
		//�������淨���޷���PK״̬�򿪵����ʹ�� ����������������Ļ�
		//�����ͷ��������Խ������������淨��
		XID attacker = emsg.ainfo.attacker;
		if(attacker == _parent->ID) return true;
		if(attacker.IsPlayerClass())
		{
			if(!(emsg.attacker_mode & attack_msg::PVP_ENABLE)
					&& (IsInPVPCombatState() ) )
			{
				return false;
			}
		}
		//��ҿ���ѡ���Ƿ��������ף��
		if(emsg.helpful == 1)
		{
			if((_refuse_bless & C2S::REFUSE_NON_TEAMMATE_BLESS) 
					&& (!IsInTeam() || !IsMember(attacker))) return false;
		}
		else if(emsg.helpful == 2)
		{
			if(_refuse_bless & C2S::REFUSE_NEUTRAL_BLESS) return false;
		}
			
		emsg.is_invader = IsInPVPCombatState() && (_invader_state != INVADER_LVL_0) && (emsg.attacker_mode & attack_msg::PVP_ENABLE);

		//����Է�����PVP״̬�������Լ�����PVP״̬����һ����Ϣ�öԷ���ΪPVP״̬
		if(!(emsg.attacker_mode & attack_msg::PVP_DURATION) && IsInPVPCombatState())
		{
			SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,emsg.ainfo.attacker,0);
		}
		return true;
	}

	template <typename MESSAGE,typename ENCHANT_MSG>
	inline bool TestHarmfulEnchant(const MESSAGE & msg, ENCHANT_MSG & emsg)
	{
		//�к������Ĺ����ж��͹����ж���ȫһ�� 
		//���ڱ�����Ҳһ�£����Կ���ֱ�ӵ��ù����ж��ĺ���
		return TestAttackMsg(msg, emsg);
	}

	template <typename MESSAGE,typename ATTACK_MSG>
	inline bool TestAttackMsg(const MESSAGE & msg, ATTACK_MSG & amsg)
	{
		//�����Ĺ����ж�
		//���ܹ����Լ��Ͷ���
		bool IsInvader = false;
		if(amsg.attacker_mode & attack_msg::PVP_DUEL)
		{
			amsg.target_faction = 0xFFFFFFFF;
		}
		else
		{
			XID attacker = amsg.ainfo.attacker;
			if( attacker.IsPlayerClass())
			{
				//����Լ�δ��PK���ز��ᱻ��ҹ���
				if(!_pvp_enable_flag) return false;

				//�������ǿ�ƹ�������������
				if(!amsg.force_attack) return false;

				//���Ѻ��Լ����ᱻ����
				if(attacker.id == _parent->ID.id || (IsInTeam() && IsMember(attacker))) return false;

				//����Է�δ��PK����Ҳ���ᱻ����
				if(!(amsg.attacker_mode & attack_msg::PVP_ENABLE))
				{
					return false;
				}

				//����ܹ������ǰ������ҹ�����Ϣ�Ǳ���������ô���ᱻ������
				if(_invader_state == INVADER_LVL_0 && (amsg.force_attack & C2S::FORCE_ATTACK_NO_WHITE))
				{
					return false;
				}

				//��������˰��ɱ��������ҹ����ߺͱ������ߵİ�����ͬ���򲻻ᱻ������
				if(amsg.ainfo.mafia_id)
				{
					if(amsg.force_attack & C2S::FORCE_ATTACK_NO_MAFIA 
							&& amsg.ainfo.mafia_id == ((gplayer*)_parent)->id_mafia) return false;
					if(amsg.force_attack & C2S::FORCE_ATTACK_NO_MAFIA_ALLIANCE 
							&& OI_IsFactionAlliance(amsg.ainfo.mafia_id)) return false;
				}

				//���������������������ͬ������Ҳ��ᱻ����
				if(amsg.ainfo.force_id && amsg.ainfo.force_id == _player_force.GetForce()) return false;

				//����Է�����PVP״̬����ش���Ϣ����Ҵ���PVP
				if(!(amsg.attacker_mode & attack_msg::PVP_DURATION))
				{
					SendTo<0>(GM_MSG_ENABLE_PVP_DURATION,attacker,0);
				}

				//����������߲��Ǻ�����Ϊ�Ƿ�����
				//IsInvader = (_invader_state != INVADER_LVL_2);
				//���ڸĳ�һ�ɷǷ�������������������Ҳ�ǷǷ�����
				IsInvader = true;
			}
		}
		amsg.is_invader = IsInvader;
		return true;
	}

	inline int AutoGenStat(int cooldown_idx, int idx ,int offset, bool bSave)
	{
		//Ӧ���Զ�ʹ��ҩ���� Ҫ�ȼ����ȴ
		if(CheckCoolDown(cooldown_idx))
		{
			//��ʼʹ�ûظ���Ʒ
			int rst = _equipment[idx].AutoTrigger(this,cooldown_idx,offset);
			if(rst < 0) return 0;
			if(rst == 0)
			{
				//��ʾ�����ƷӦ��ɾ����
				if(bSave)
				{
					int type = _equipment[idx].type;
					int proc_type = _equipment[idx].proc_type;

					UpdateMallConsumptionDestroying(type, proc_type, 1);

					_equipment.DecAmount(idx,1);
					_runner->player_drop_item(IL_EQUIPMENT,idx,type,1,S2C::DROP_TYPE_USE);
				}
				else
				{
					PlayerGetItemInfo(IL_EQUIPMENT,idx);
				}
				return 0;
			}
			//������Ʒ
			PlayerGetItemInfo(IL_EQUIPMENT,idx);
		}
		return 1;
	}


public:
//����״̬�µ���Ϣ��������
	int GeneralTradeMessageHandler(world * pPlane, const MSG & msg);
	int TradeMessageHandler(world * pPlane ,const MSG & msg);
	int WaitingTradeMessageHandler(world * pPlane ,const MSG & msg);
	int WaitingTradeCompleteHandler(world * pPlane ,const MSG & msg);
	int WaitingTradeReadHandler(world * pPlane ,const MSG & msg);
	int WatingFactionTradeReadHandler(world * pPlane ,const MSG & msg);
	int WaitingFactionTradeTradeHandler(world * pPlane ,const MSG & msg);
	int DisconnectMessageHandler(world * pPlane ,const MSG & msg);
	int WaitingSwitchServer(world * pPlane, const MSG & msg);
	int StayInHandler(world * pPlane, const MSG & msg);
	int MarketHandler(world * pPlane, const MSG & msg);
	int TravelMessageHandler(world * pPlane, const MSG & msg);
	virtual int ZombieMessageHandler(world * pPlane ,const MSG & msg);
	virtual int MessageHandler(world * pPlane ,const MSG & msg);
	virtual int DispatchMessage(world * pPlane ,const MSG & msg);

	
	void FromTradeToNormal(int type = 0);		//�ӽ���״̬�ع鵽����״̬
	void FromFactionTradeToNormal(int type = 0);	//�Ӱ��ɽ���״̬�ع鵽����״̬

	void GetPlayerCharMode(archive & ar);		//�������ݿ�ʱ������ҵ�ǰ��һЩ״̬���б���
	void SetPlayerCharMode(const void * buf, unsigned int size);
	void PlaneEnterNotify(bool is_enter);
	void GetCommonDataList(bool send_content);

private:
	//���������ֹ������õ�Ŀ���� ��ֹ�������Ӷ��ص���Ҫ��������ݽṹ��ʹ�ø��ֽṹ�����һ��
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);

protected:
	static bool __GetPetAttackHook(gactive_imp * __this, const MSG & , attack_msg & );
	static bool __GetPetEnchantHook(gactive_imp * __this, const MSG & ,enchant_msg & );
	static void __GetPetAttackFill(gactive_imp * __this, attack_msg & attack);
	static void __GetPetEnchantFill(gactive_imp * __this, enchant_msg & enchant);

public:
//�麯��Ⱥ ������npc�����ͳһ�Ĳ������Ժ������չ�Ĳ���
	virtual void PlayerEnterServer(int source_tag);		//��ҽ���������������л�������
	virtual void PlayerEnterWorld();		//��ҽ�������
	virtual void PlayerLeaveServer();		//����뿪�������������л�������
	virtual void PlayerLeaveWorld();		//����뿪����
	virtual void PlayerEnterParallelWorld();//��ҽ���ƽ�����磬������һ���
	virtual void PlayerLeaveParallelWorld();//����뿪ƽ������
	virtual void Init(world * pPlane,gobject*parent);
	virtual void InitClock();
	virtual void SwitchSvr(int dest, const A3DVECTOR & oldpos, const A3DVECTOR &newpos,const instance_key *switch_key);
	virtual int DoAttack(const XID & target, char force_attack);
	virtual void AdjustDamage(const MSG & msg, attack_msg * attack,damage_entry & dmg,float & damage_adjust);
	virtual void OnDamage(const XID & attacker,int skill_id,const attacker_info_t&info,int damage,int at_state,char speed,bool orange,unsigned char section);
	virtual void OnHurt(const XID & attacker,const attacker_info_t&info,int damage,bool invader);
	virtual void OnHeal(const XID & healer, int life);
	virtual bool CanTrade(const XID & target);
	virtual void StartTrade(int trade_id,const XID & target);
	virtual bool StartFactionTrade(int trade_id,int get_mask, int put_mask,bool no_response = false);
	virtual void TradeComplete(int trade_id,int reason,bool need_read);
	virtual void FactionTradeTimeout();
	virtual void FactionTradeComplete(int trade_id,const GNET::syncdata_t & data);
	virtual void SyncTradeComplete(int trade_id, unsigned int money, const GDB::itemlist & item_change,bool writetrashbox, bool cash_change);
	virtual void WaitingTradeWriteBack(int trade_id, bool bSuccess);
	virtual void WaitingTradeReadBack(int trade_id,const GDB::itemlist * inventory,int money);
	virtual void WaitingFactionTradeReadBack(const GDB::itemlist * inventory,int money,int timestamp,int mask);
	virtual bool CanAttack(const XID & target);
	virtual bool CheckLevitate();
	virtual void PhaseControlInit();
	virtual const XID & GetCurTarget();
	virtual int GetAmmoCount();
	virtual int TakeOutItem(int item_id);
	virtual int TakeOutItem(int item_id, unsigned int count);//lgc
	virtual void TakeOutItem(const int * id_list, unsigned int list_count, unsigned int count);	//���İ�����count��list��������Ʒ
	virtual bool CheckItemExist(int item_id, unsigned int count);
	virtual bool CheckItemExist(int inv_index,int item_id, unsigned int count);
	virtual bool CheckItemExist(const int * id_list, unsigned int list_count, unsigned int count);	//���������Ƿ����count��list��������Ʒ
	virtual int CheckItemPrice(int inv_index, int item_id);
	virtual void DropSpecItem(bool isProtected, const XID & owner);
	//virtual unsigned int GetMoneyAmount(bool is_silver) { return is_silver ? _player_money[1] : _player_money[0];}
	//virtual void DecMoneyAmount(unsigned int money, bool is_silver);// { SpendMoney(money,true,is_silver); _runner->get_player_money(_player_money[0], _money_capacity , _player_money[1] , _lua_role.money_guild ); }
	//virtual void DropMoneyAmount(unsigned int money, bool isProtected);
	virtual int DispatchCommand(int cmd_type, const void * buf,unsigned int size);
	virtual bool StepMove(const A3DVECTOR &offset);
	virtual void SendDataToSubscibeList();
	virtual void SendTeamDataToSubscibeList();
	virtual void SendTeamDataToMembers();
	virtual	void OnHeartbeat(unsigned int tick);
	virtual void SetCombatState();
	virtual bool CheckInvaderAttack(const XID & who);
	virtual void FillAttackMsg(const XID & target, attack_msg & attack,int dec_arrow);
	virtual void FillEnchantMsg(const XID & target,enchant_msg & enchant);
	virtual void SendAttackMsg(const XID & target, attack_msg & attack);
	virtual int  GetCSIndex() { return GetParent()->cs_index;}
	virtual int  GetCSSid() { return GetParent()->cs_sid;}
	virtual void SendEnchantMsg(int message,const XID & target, enchant_msg & attack);
	virtual void SendMsgToTeam(const MSG & msg, float range, bool exclude_self);
	virtual bool OI_IsMember(const XID & member) { return IsMember(member);}
	virtual bool OI_IsInTeam() { return IsInTeam();}
	virtual bool OI_IsTeamLeader() { return IsTeamLeader();}
	virtual int SpendFlyTime(int tick);
	virtual int GetFlyTime();
	virtual void KnockBack(const XID & target, const A3DVECTOR & source, float distance,int time,int stun_time);
	virtual void PullOver(const XID & target, const A3DVECTOR & source,float distance, int time);
	virtual void Teleport(const A3DVECTOR & pos, int time, char mode);
	virtual void Teleport2(const A3DVECTOR & pos, int time, char mode);
	virtual void KnockUp(float distance,int time);
	virtual void AddAggroToEnemy(const XID & who,int rage);
	virtual void ClearAggroToEnemy();	//������ҽ�������״̬ʱ���npc���Լ��ĳ��
	virtual bool DrainMana(int mana);
	virtual void SetIdleMode(bool sleep, bool stun);
	virtual void SetSealMode(bool silent,bool root);
	virtual bool Resurrect(float exp_reduce); //�������õĽӿ�
	virtual void OnDuelStart(const XID & target);
	virtual void OnDuelStop();
	virtual void Die(const XID & attacker, bool is_pariah, char attacker_mode, int taskdead);
	virtual void OnDeath(const XID & lastattack,bool is_pariah, char attacker_mode, int taskdead);
	//virtual void OnPickupMoney(unsigned int money,int drop_id, bool is_silver);
	virtual void OnPickupItem(const A3DVECTOR & pos,const void * data, unsigned int size,bool isTeam,int drop_id);
	virtual void OnAttacked(world *pPlane, 	const MSG & msg, attack_msg * attack, damage_entry & dmg,bool hit); 
	virtual bool UseProjectile(int count);
	virtual void CancelSwitch();
	virtual void PlayerForceOffline();
	virtual void ServerShutDown();
	virtual void UpdateMafiaPvP(unsigned char pvp_mask);
	virtual void UpdateMafiaInfo(int id, char rank, unsigned char pvp_mask, int64_t unifid);	//���°�����Ϣ
	virtual void UpdateFactionRelation(int * alliance, unsigned int asize, int * hostile, unsigned int hsize, bool notify_client);//���°���ͬ����Ϣ
	virtual const A3DVECTOR & GetLogoutPos(int & world_tag);//ȡ�õǳ�ʱӦ��ʹ�õ����꣬���Ա�����
	virtual bool CheckCoolDown(int idx);
	virtual void SetCoolDown(int idx, int msec);
	virtual void CoolDownReduceAtr(int idx, int msec);
	virtual void GetCoolDownData(archive & ar);
	virtual void GetCoolDownDataForClient(archive & ar);
	virtual int GetMonsterFaction() { return PLAYER_MONSTER_TYPE;}
	virtual int GetFactionAskHelp() { return PLAYER_MONSTER_TYPE;}
	virtual void EnhanceBreathCapacity(int value);
	virtual void ImpairBreathCapacity(int value);
	virtual void InjectBreath(int value);
	virtual void EnableEndlessBreath(bool bRst);
	virtual void AdjustBreathDecPoint(int offset);
	virtual void EnableFreePVP(bool bVal);
	virtual void ObjReturnToTown();
	virtual void AddEffectData(short effect);
	virtual void RemoveEffectData(short effect);
	virtual void EnterCosmeticMode(unsigned short inv_index,int cos_id);
	virtual void LeaveCosmeticMode(unsigned short inv_index);
	virtual void SetPerHitAP(int ap_per_hit);
	virtual void ModifyPerHitAP(int delta);
	virtual bool IsPlayerClass() { return true;}
	virtual bool IsEquipWing();
	virtual void OnUseAttackRune();
	virtual int GetLinkIndex();
	virtual int GetLinkSID();
	virtual int SummonMonster(int mod_id, int count, const XID& target, int target_distance, int remain_time, char die_with_who, int path_id);
	virtual int SummonNPC(int npc_id, int count, const XID& target, int target_distance, int remain_time);
	virtual int SummonMine(int mine_id, int count, const XID& target, int target_distance, int remain_time);
	virtual bool UseSoulItem(int type, int level, int power);
	virtual void IncAntiInvisiblePassive(int val);
	virtual void DecAntiInvisiblePassive(int val);
	virtual void IncAntiInvisibleActive(int val);
	virtual void DecAntiInvisibleActive(int val);
	virtual void IncInvisiblePassive(int val);
	virtual void DecInvisiblePassive(int val);

	virtual void IncPetTime(int val){ _increase_pet_time += val; }
	virtual void DecPetTime(int val){ _increase_pet_time -= val; }
	virtual int GetPetTime(){ return _increase_pet_time; }

	virtual void SetRealWeaponClass(int real_weapon_class){ _real_weapon_class = real_weapon_class; }
	virtual int GetRealWeaponClass(){ return _real_weapon_class; }

	virtual void SetFakeWeaponClass(int fake_weapon_class){ _fake_weapon_class = fake_weapon_class; }
	virtual int GetFakeWeaponClass(){ return _fake_weapon_class; }

	virtual void SetInvisible(int invisible_degree);
	virtual void ClearInvisible();
	virtual void SendDisappearToTeam();	//����Ҵ�������״̬��,���ǰҪ����ѷ�����ʧ��Ϣ
	virtual void SendDisappearToTeamMember(int id,int cs_index,int cs_sid);//����Ҵ�������״̬��,���������Ҫ���䷢����ʧ��Ϣ
	virtual void SendAppearToTeam();	//����Ҵ�������״̬��,��Ӻ�Ҫ����ѷ��ͳ�����Ϣ
	virtual void SendAppearToTeamMember(int id,int cs_index,int cs_sid);//����Ҵ�������״̬��,���������Ҫ���䷢�ͳ�����Ϣ
	virtual bool CanSeeMe(int player_id);	//���ݷ����ȼ��ж϶Է��Ƿ���Կ����Լ�,������������
	virtual int GetSoulPower(){ return _soul_power + _soul_power_en; } 
	virtual void EnhanceSoulPower(int val){ _soul_power_en += val; }
	virtual void ImpairSoulPower(int val){ _soul_power_en -= val; }
	virtual void UpdateMinAddonExpireDate(int addon_expire);  
	virtual void SetGMInvisible();
	virtual void ClearGMInvisible();
	virtual bool ActivateSharpener(int id, int equip_index);
	virtual bool SummonPet2(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);
	virtual bool SummonPlantPet(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);
	virtual bool CalcPetEnhance(int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour);
	virtual bool PetSacrifice();
	virtual bool IsPetActive();
	virtual bool PlantSuicide(float distance, const XID & target, char force_attack);
	virtual void InjectPetHPMP(int hp, int mp);
	virtual void DrainPetHPMP(const XID & pet_id, int hp, int mp);
	virtual void LongJumpToSpouse();
	virtual void WeaponDisabled(bool disable);
	virtual void ElmoDisabled(bool disable);
	virtual void DetectInvisible(float range);
	virtual void ForceSelectTarget(const XID & target);
	virtual void ExchangePosition(const XID & target);
	virtual void SetSkillAttackTransmit(const XID & target);
	virtual void CallUpTeamMember(const XID& member){ TeamMemberCongregateRequest(member); }
	virtual void QueryOtherInventory(const XID& target);
	virtual void IncPetHp(int inc){ _pet_enhance.hp_percent += inc; }
	virtual void IncPetMp(int inc){ _pet_enhance.mp_percent += inc; }
	virtual void IncPetDamage(int inc){ _pet_enhance.damage_percent += inc; }
	virtual void IncPetMagicDamage(int inc){ _pet_enhance.magic_damage_percent += inc; }
	virtual void IncPetDefense(int inc){ _pet_enhance.defense_percent += inc; }
	virtual void IncPetMagicDefense(int inc){ _pet_enhance.magic_defense_percent += inc; }
	virtual void IncPetAttackDegree(int inc){ _pet_enhance.attack_degree_percent += inc; }
	virtual void IncPetDefendDegree(int inc){ _pet_enhance.defend_degree_percent += inc; }
	virtual void ReduceResurrectExpLost(int value)	{ _resurrect_exp_lost_reduce += value; }
	virtual void IncreaseResurrectExpLost(int value){ _resurrect_exp_lost_reduce -= value; }
	virtual void SetPlayerLimit(int index, bool b);
	virtual bool GetPlayerLimit(int index);
	virtual void DenyAttackCmd(){ _commander->DenyCmd(controller::CMD_ATTACK); }
	virtual void AllowAttackCmd(){ _commander->AllowCmd(controller::CMD_ATTACK); }
	virtual void DenyElfSkillCmd(){ _commander->DenyCmd(controller::CMD_ELF_SKILL);}
	virtual void AllowElfSkillCmd(){ _commander->AllowCmd(controller::CMD_ELF_SKILL);}
	virtual void DenyUseItemCmd(){_commander->DenyCmd(controller::CMD_USE_ITEM);}
	virtual void AllowUseItemCmd(){_commander->AllowCmd(controller::CMD_USE_ITEM);}
	virtual void DenyNormalAttackCmd(){_commander->DenyCmd(controller::CMD_NORMAL_ATTACK);}
	virtual void AllowNormalAttackCmd(){_commander->AllowCmd(controller::CMD_NORMAL_ATTACK);}
	virtual void DenyPetCmd(){_commander->DenyCmd(controller::CMD_PET);}
	virtual void AllowPetCmd(){_commander->AllowCmd(controller::CMD_PET);}
	virtual void EnterNonpenaltyPVPState(bool b);
	virtual int GetHistoricalMaxLevel(){ return _player_reincarnation.GetHistoricalMaxLevel(); }
	virtual int GetAvailLeadership(){ return _leadership - _leadership_occupied; }	
	virtual void OccupyLeadership(int v){ _leadership_occupied += v; }
	virtual void RestoreLeadership(int v){ _leadership_occupied -= v; }
	virtual void SyncScoreToPlane(){}
	virtual void SetFlagCarrier(bool b){}
	virtual bool TrickBattleTransformChariot(int chariot){ return false; }
	virtual bool TrickBattleUpgradeChariot(int chariot){ return false; }
	virtual void TrickBattleIncChariotEnergy(int energy){}
	virtual unsigned int OI_GetInventorySize();
	virtual unsigned int OI_GetEmptySlotSize();
	virtual int OI_GetInventoryDetail(GDB::itemdata * list, unsigned int size);
	virtual int OI_GetEquipmentDetail(GDB::itemdata * list, unsigned int size);
	virtual unsigned int OI_GetEquipmentSize();
	virtual int TradeLockPlayer(int get_mask, int put_mask);
	virtual int TradeUnLockPlayer();
	virtual void ActiveMountState(int mount_id, unsigned short mount_color);
	virtual void DeactiveMountState();

	virtual bool AddPetToSlot(void * data, int inv_index);
	virtual bool FeedPet(int food_mask, int honor);
	virtual bool OI_IsMafiaMember();
	virtual int OI_GetMafiaID();
	virtual char OI_GetMafiaRank();
	virtual bool OI_IsMafiaMaster(); 
	virtual bool OI_IsFactionAlliance(int fid);
	virtual bool OI_IsFactionHostile(int fid);
	virtual int OI_GetSpouseID(){ return GetSpouse(); }
	virtual int OI_GetReputation(){ return _reputation; }
	virtual bool CheckGMPrivilege();
	virtual unsigned int OI_GetTrashBoxCapacity(int where);
	virtual int OI_GetTrashBoxDetail(int where, GDB::itemdata * list, unsigned int size);
	virtual bool OI_IsTrashBoxModified();
	virtual bool OI_IsEquipmentModified();
	virtual unsigned int OI_GetTrashBoxMoney();
	virtual void EnterBattleground(int world_tag, int battle_id);
	virtual void EnterFactionFortress(int dst_world_tag, int dst_factionid);
	virtual void EnterCountryBattle(int world_tag, int battle_id);
	virtual int OI_GetDBTimeStamp();
	virtual int OI_InceaseDBTimeStamp();
	virtual bool CanResurrect(int param);
	virtual int  Resurrect(const A3DVECTOR & pos,bool nomove,float exp_reduce,int target_tag,float hp_factor, float mp_factor, int param, float ap_factor, int extra_invincible_time);
	virtual bool CheckWaypoint(int point_index, int & point_domain);
	virtual bool ReturnWaypoint(int point);
	virtual attack_judge GetPetAttackHook();
	virtual enchant_judge GetPetEnchantHook();
	virtual attack_fill GetPetAttackFill();
	virtual enchant_fill GetPetEnchantFill();
	virtual bool OI_IsPVPEnable() { return _pvp_enable_flag;}
	virtual char OI_GetForceAttack() { return _force_attack;}
	virtual bool OI_IsInPVPCombatState() { return IsInPVPCombatState();}
	virtual int CheckUseTurretScroll();
	virtual bool OI_IsInventoryFull() { return _inventory.IsFull();} 
	virtual XID OI_GetPetID();
	virtual void OI_ResurrectPet();
	virtual void OI_RecallPet(){ RecallPet(); }
	virtual void Notify_StartAttack(const XID & target,char force_attack);
	virtual bool OI_GetMallInfo(int & cash, int &cash_used, int &cash_delta,  int &order_id);
	virtual bool OI_IsCashModified();
	virtual void ActivePetNoFeed(bool feed);
	virtual unsigned int OI_GetMallOrdersCount();
	virtual int OI_GetMallOrders(GDB::shoplog * list, unsigned int size);
	virtual void OnAntiCheatAttack(float rate);
	virtual bool CanTeamRelation();
	virtual bool OI_TestSafeLock();
	virtual unsigned int OI_x_LevelUp(gplayer_imp *pImp, unsigned int index, int level_counter) { return _petman.x_LevelUp(pImp, index, level_counter); }
	virtual unsigned int OI_GetCurActivePet() { return _petman.GetCurActivePet(); }
	virtual unsigned int OI_GetPetsCount() { return _petman.GetPetsCount(); }
	virtual unsigned int OI_GetPetSlotCapacity() { return _petman.GetAvailPetSlot(); }
	virtual pet_data * OI_GetPetData(unsigned int index){ return _petman.GetPetData(index); }
	virtual void OI_TryCancelPlayerBind();
	virtual int OI_GetTaskMask(){ return _task_mask; }
	virtual int OI_GetForceID(){ return _player_force.GetForce(); }
	virtual void UpdateMallConsumptionShopping(int id, unsigned int proc_type, int count, int total_price); // ����ʱ�����������ֵ
	virtual void UpdateMallConsumptionBinding(int id, unsigned int proc_type, int count); // ���˺�һʱ�����������ֵ
	virtual void UpdateMallConsumptionDestroying(int id, unsigned int proc_type, int count); // ���Ļ�۳�ʱ�����������ֵ
	virtual bool CalcPetEnhance2(const pet_data *pData, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag);
	virtual bool CalcPetEnhance3(const pet_data *pData, int skill_level, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag);
	virtual bool CalcPetEnhance4(const pet_data *pData, extend_prop& prop, int& attack_degree, int& defend_degree, int& vigour, int& pen_fis, int& pen_mag);
	virtual	void GetNatureSkill(int nature,int &skill1,int &skill2); 
	virtual void SaveRealWorldPos(int world_tag, const A3DVECTOR & pos){}
	virtual void SendAllData(bool detail_inv, bool detail_equip, bool detail_task);
	virtual int OI_GetRealm() { return _realm_level; }
	virtual void QueryTrickBattleChariots() {}
    virtual void SetHasPVPLimitFilter(bool has_pvp_limit_filter);
	virtual void EnhanceMountSpeedEn(float sp);
	virtual void ImpairMountSpeedEn(float sp);
	
	virtual void EnhanceFlySpeedEn(float sp);
	virtual void ImpairFlySpeedEn(float sp);	
	
	virtual int  UseFireWorks2(char is_cast_action, int target_role_id, int item_type, const char * target_user_name);
	virtual int	 AddFixPositionEnergy(int item_id);
	virtual bool IsArenaMember(const XID& member)
	{
		printf("gplayer::IsArenaMember : %d\n", member.id);
		return false;
	}
	
	// money 172
	virtual unsigned int GetGoldAmount() { return  _money.GetGold();}
	virtual bool DecGoldAmount(unsigned int value);
	
	virtual unsigned int GetSilverAmount() { return _money.GetSilver();}
	virtual bool DecSilverAmount(unsigned int value);
	
	virtual unsigned long long GetAllMoneyAmount() { return _money.GetMoney(); }
	virtual bool DecAllMoneyAmount(unsigned int value);
	
	virtual void DropGoldAmount(unsigned int money, bool isProtected);
	virtual void DropSilverAmount(unsigned int money, bool isProtected);
	
	virtual void OnPickupGold(unsigned int money,int drop_id);
	virtual void OnPickupSilver(unsigned int money,int drop_id);
	
	// money 172 end
	
public:
//�߼���������Ⱥ
	int  GetInstanceReenterTimeout();
	int  GetWorldType();
	void CalcEquipmentInfo();		//����װ����������������
	bool ObtainItem(int where, item_data * pData, bool isTask = false);//����false��ʾ���data��ʹ���ˣ���Ҫ�ͷ�
	void DropItemOnDeath(unsigned int drop_count_inv,unsigned int drop_count_equip, const XID & spec_owner);
	void DamageItemOnDeath(bool restrict_bind, const XID & killer);
	void ThrowDeadDropItem();
	bool ReturnToTown();
	bool SitDown();
	bool CanSitDown();
	void StandUp();
	void PurchaseItem(abase::pair<const item_data *, int> *pItem,unsigned int size,unsigned int money, int consume_contrib, int force_contrib);
	bool ItemToMoney(unsigned int inv_index, int type, unsigned int count, int price);
	inline bool UseItem(item_list & inv, int inv_index, int where, int item_type, unsigned int count)
	{
		bool bRst = inv[inv_index].IsBroadcastUseMsg();
		int type = inv[inv_index].type;
		int proc_type = inv[inv_index].proc_type;
		int rst = inv.UseItem(inv_index,this,count);
		if(rst >= 0)
		{
			ASSERT(rst == item_type);
			UpdateMallConsumptionDestroying(type, proc_type, count);
			_runner->use_item(where,inv_index, item_type,count);
			if(bRst) _runner->use_item(item_type);
			return true;
		}
		return false;
	}
	
	inline bool UseItemWithTarget(item_list & inv, int inv_index, int where, int item_type,const XID& target, char force_attack )
	{
		bool bRst = inv[inv_index].IsBroadcastUseMsg();
		int type = inv[inv_index].type;
		int proc_type = inv[inv_index].proc_type;
		int count = 0;
		int rst = inv.UseItemWithTarget(inv_index,this,target, force_attack,count);
		if(rst >= 0)
		{
			ASSERT(rst == item_type);
			UpdateMallConsumptionDestroying(type, proc_type, count);
			_runner->use_item(where,inv_index, item_type,count);
			if(bRst) _runner->use_item(item_type);
			return true;
		}
		return false;
	}
	
	inline bool UseItemWithTarget(int where,unsigned int inv_index,int item_type,const XID & target,char force_attack) //�ڲ�����
	{
		ASSERT(where == IL_INVENTORY || where == IL_EQUIPMENT);
		item_list & inv = GetInventory(where);
		ASSERT(inv_index < inv.Size());
		if(inv[inv_index].type != item_type) return false;
		return UseItemWithTarget(inv,inv_index,where, item_type, target,force_attack);
	}

	inline bool UseItem(int where, unsigned int inv_index, int item_type,unsigned int count) //�ڲ�����
	{
		ASSERT(where == IL_INVENTORY || where == IL_EQUIPMENT);
		item_list & inv = GetInventory(where);
		ASSERT(inv_index < inv.Size());
		if(inv[inv_index].type != item_type) return false;
		return UseItem(inv,inv_index,where,item_type, count);
	}
	bool UseItemWithArg(item_list & inv, int inv_index, int where, int item_type, const char * arg, unsigned int arg_size);

	int ThrowDart(const XID & target,int damage,float throw_range,char force_attack = false);
	void MakeDartAttack(int damage, float throw_range,attack_msg &attack,char force_attack);
	void RepairAllEquipment();
	void RepairEquipment(int where, unsigned int index);
	void RemoteAllRepair();
	bool EmbedChipToEquipment(unsigned int chip,unsigned int equip);
	bool SharpenEquipment(unsigned int index, addon_data * addon_list, unsigned int count, int sharpener_level, int sharpener_gfx);
	bool RechargeEquippedFlySword(unsigned int index, unsigned int count);
	bool RechargeFlySword(unsigned int element_index, unsigned int count,unsigned int fw_index,int fw_id);
	bool LongJump(const A3DVECTOR &pos); 				//�ռ���ת
	bool LongJump(const A3DVECTOR &pos,int target_tag,int ctrl_id = 0); //��Խλ����ת
	void Swap(gplayer_imp * rhs);		//����������ݣ��Ը�������߼�
	void IdentifyItemAddon(unsigned int index, unsigned int fee);
	bool RefineItemAddon(unsigned int index, int item_type,int rt_index, int npcid);
	bool StartTravelSession(unsigned int min_time,unsigned int max_time, const A3DVECTOR & dest_pos,float speed,int vehicle);
	bool CompleteTravel(int vehicle,const A3DVECTOR & target);
	bool TestSanctuary();			//����Ƿ���ڰ�ȫ��
	bool TestPKProtected();		//����Ƿ������ְ�ȫ��
	int CheckPlayerMove(const A3DVECTOR & offset, int mode, int use_time);
	void SetCoolDownData(const void * buf, unsigned int size);
	bool CheckAndSetCoolDown(int idx, int msec);
	void SwitchFashionMode();
	bool RegionTransport(int rindex, int tag);
	void EnterResurrectReadyState(float exp_reduce, float hp_factor, float mp_factor);
	void RebuildAllInstanceData(int create_time);	  //�ؽ����е�����
	void RebuildInstanceKey();			  //�ؽ����е�key
	bool GetInstanceKeyBuf(char * buf, unsigned int size);  //ȡ��Instance Key buf
	bool SetInstanceKeyBuf(const void * buf, unsigned int size); //�������е�instance key 
	int  CheckInstanceTimer(int tag);		  //����Ƿ���Խ��и������ã�����������õ�ǰ��timer
	void SetInstanceTimer(int tag);			  //���ý��븱����ʱ�䣬�����޶��Ƿ���Խ��и������õ� 
	void SaveInstanceKey(archive & ar);		  //���渱��key���ݽ������ݿ�
	void InitInstanceKey(int create_time, const void * buf , unsigned int size);	//�����ݿ��ʼ���������ݣ�ֻ����buf��sizeΪ0��ʱ��create_time������
	void ResetInstance(int world_tag);		//����world_tag��Ӧ����
	void CosmeticSuccess(int ticket_inv_idx, int ticket_id, int result, unsigned int crc);
	void Say(const char * msg, int channel = GMSV::CHAT_CHANNEL_LOCAL);
	void GBKSay(const char * msg, int channel = GMSV::CHAT_CHANNEL_LOCAL);
	//void GainMoneyWithDrop(unsigned int inc, bool,bool);
	//void GainMoney(unsigned int inc,bool);		//no drop
	//void SpendMoney(unsigned int delta,bool,bool);
	//bool CheckIncMoney(unsigned int inc,bool);
	//bool CheckDecMoney(unsigned int delta,bool);
	void LeaveDoubleExpMode();
	int EnterDoubleExpMode(int time_mode, int timeout);	//���ص�ǰ���õ�˫��ʱ��
	bool ActiveDoubleExpTime(int time);
	void TrashBoxOpen(bool view_only);
	void TrashBoxClose(bool view_only);
	void TryClearTBChangeCounter();
	void UserTrashBoxOpen();
	void UserTrashBoxClose();
	void TryClearUserTBChangeCounter();
	bool CheckPlayerAutoSupport();
	bool CheckPlayerBindRequest();
	bool CheckPlayerBindInvite();
	bool EnterBindMoveState(const XID & target);
	bool EnterBindFollowState(const XID & target);
	void ReturnBindNormalState();
	void TestUnderWater();
	void UpdatePlayerLayer();

	bool TestCreatePet(unsigned int index);
	bool SummonPet(unsigned int index);
	bool BanishPet(unsigned int index);
	bool RecallPet();
	void SetPetSlotCapacity(unsigned int new_size, bool notify = true);
	unsigned int GetPetSlotCapacity();
	void DBSetPetData(unsigned int index, const void * data, unsigned int size, bool codex = false);
	pet_data * GetPetData(unsigned int index);
	
	int ServiceConvertPetToEgg(unsigned int index);
	void SendPetCommand(int cur_target,int cmd,const void * buf, unsigned int size);
	void SetPetLeaderData(pet_leader_prop & data);
	void SayHelloToNPC(const XID & target);
	int ConvertPetToEgg(unsigned int slot_index);
	int ConvertPetDataToEggData(void * data, unsigned int size, pet_data * pData, const void * src_temp);
	int RegroupPropPoint(int str, int agi, int vit,int eng);	//ϴ��
	void LeaveAbnormalState();		//��ͼ�뿪������״̬
	void SendFarCryChat(char channgel, const void * msg, unsigned int size, const void* data, unsigned int dsize, bool cooldown = false);
	bool CheckAllianceChat(char channel);
	void SendTeamChat(char channel, const void * buf, unsigned int len, const void* data, unsigned int dsize, int use_id = 0);
	void SendNormalChat(char channel, const void * buf, unsigned int len, const void* data, unsigned int dsize);
	void SendBattleFactionChat(char channel, const void * buf, unsigned int len, const void* data, unsigned int dsize);
	void SendCountryChat(char channel, const void * msg, unsigned int size, const void* data, unsigned int dsize);
	void SendGlobalChat(char channel, const void * msg, unsigned int size, const void* data, unsigned int dsize, bool cooldown = false);
	bool ResurrectPet(unsigned int index);
	void NotifyMasterInfoToPet(bool at_once = false);
	void FirstAcquireItem(const item_data* itemdata);
	bool ChangePetName(unsigned int index,const char name[] , unsigned int name_len);
	bool ForgetPetSkill(int skill_id);
	int LearnPetSkill(int skill_id);
	bool IsPetExist(unsigned int index, int pet_tid);
	int DyePet(unsigned int pet_index, unsigned int dye_index);
	void RemoveAllExpireItems();
	void RemoveAllExpireAddon();
	bool CheckItemBindCondition(unsigned int index,  int id, int can_webtrade);
	bool BindItem(unsigned int index, int id, int can_webtrade);
	bool CheckBindItemDestory(unsigned int index, int id);
	bool DestoryBindItem(unsigned int index, int id);
	bool CheckRestoreDestoryItem(unsigned int index, int id);
	bool RestoreDestoryItem(unsigned int index, int id);
	void SetChatEmote(int emote_id);
	void SetHPAutoGen(int value, float rate);
	void SetMPAutoGen(int value, float rate);
	bool StackProduceItem(const recipe_template & rt, unsigned int & act_count);
	bool ProduceItem(const recipe_template & rt);

	bool ProduceItem2(const recipe_template & rt,int materials[16], int idxs[16]);
	bool ProduceItem3(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type);
	bool ProduceItem4(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char & inherit_type, void **pItem, unsigned short &crc, int &eq_refine_level, int &eq_socket_count, int eq_stone_type[], addon_data eq_engrave_addon_list[3], unsigned int & eq_engrave_addon_count);
    bool ProduceItem5(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type);
	bool ProduceItemNewArmor(const recipe_template & rt,int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type);
	bool EngraveItem(const engrave_recipe_template& ert, unsigned int inv_index);
	bool ItemAddonRegen(const addonregen_recipe_template& arrt, unsigned int inv_index);
	void ChangeInventorySize(int size);
	int  DyeFashionItem(unsigned int f_index, unsigned int d_index);
	int  DyeSuitFashionItem(unsigned char inv_idx_body,unsigned char inv_idx_leg,unsigned char inv_idx_foot,unsigned char inv_idx_wrist,unsigned char inv_idx_dye);
	int GetRequireDyeCount(unsigned char inv_idx, int equip_index);
	void FindCheater(int type, bool noreport);
	void PunishCheat();
	void FindCheater2();
	void WallowControl(int level, int p_time, int l_time, int h_time, int reason);
	void QuestionBonus();
	void DoTeamRelationTask(int reason);
	int RefineTransmit(unsigned int src_index, unsigned int dest_index);
	void TaskSendMessage(int task_id, int channel, int param);
    void ItemMakeSlot(unsigned int index, int id, unsigned int material_id = 0, int material_count = 0);
	void RepairDamagedItem(unsigned char inv_idx);
	void GodEvilConvert(unsigned char mode);
	int WeddingBook(int start_time, int end_time, int scene, int bookcard_index);
	int WeddingCancelBook(int start_time, int end_time, int scene);
	int WeddingInvite(int invitecard_index, int invitee);
	int GetKnockBackPos(const A3DVECTOR& attacker_pos, float back_distance, A3DVECTOR & back_pos); 
	int FactionFortressLevelUp();
	int FactionFortressSetTechPoint(unsigned int tech_index);
	int FactionFortressConstruct(int id, int accelerate);
	int FactionFortressHandInMaterial(unsigned int inv_index, int id, unsigned int count);
	int FactionFortressHandInContrib(int contrib);
	int FactionFortressMaterialExchange(unsigned int src_index,unsigned int dst_index,int material);
	int FactionFortressDismantle(int id);
	int FactionFortressResetTechPoint(unsigned int tech_index, unsigned int inv_index, int id);
	void FindSpecItem(unsigned char where, int type, archive& ar);
	int RemoveSpecItem(unsigned char where, unsigned char index, unsigned int count, int type);
	bool TryTransmitSkillAttack(const MSG & msg);
	int GetForceReputation(){ return _player_force.GetReputation(); }
	int GetForceContribution(){ return _player_force.GetContribution(); }
	bool IncForceReputation(int value);
	bool DecForceReputation(int value);
	bool IncForceContribution(int value);
	bool DecForceContribution(int value);
	void UpdateForceTicketInfo(int require_force, int repu_inc_ratio);
	int CountryJoinApply();
	bool CountryJoinStep1(int country_id, int country_expiretime, int major_strength, int minor_strength, int world_tag, const A3DVECTOR & pos);
	bool CountryJoinStep2();
	bool CountryReturn();
	int CountryLeave();
	void CountryTerritoryMove(const A3DVECTOR & pos, bool capital);
	void GetCarnivalKickoutPos(int & world_tag, A3DVECTOR & pos);
	bool ReturnRestWorld();
	void GetAUMailTask(int level,char ex_reward);

	bool EquipSign(int ink_inv_idx, int ink_id, int equip_inv_idx, int equip_id, const char * signature, unsigned int signature_len);
	int PlayerEvolutionPet(unsigned int index,int formula_index);
	/*166+*/	
	bool PlayerSkillDataInc(int index_from,int index_to);
	bool PlayerPetUseSkinInc(int index, int pos_data, int mode);
	void PetEnterWorldSkinStorage();

	int PlayerAddPetExp(unsigned int index,unsigned int num);
	void PlayerPetLevelUp();
	int PlayerRebuildPetInheritRatio(unsigned int index,int formula_index);
	int PlayerRebuildPetNature(unsigned int index,int formula_index);
	bool RebulidPetInheritRatio(int pet_id,unsigned int index,int formula_index,int &r_attack,int &r_defense,int &r_hp,int &r_atk_lvl,int &r_def_lvl/*, int &r_pen_lvl_f, int &r_pen_lvl_m, int &r_chi_point*/);
	void AcceptInheritRatioResult(int pet_id,unsigned int index,int r_attack,int r_defense,int r_hp,int r_atk_lvl,int r_def_lvl/*, int r_pen_lvl_f, int r_pen_lvl_m, int r_chi_point*/);
	bool RebuildPetNature(int pet_id,unsigned int index,int formula_index,int &nature);
	void AcceptNatureResult(int pet_id,unsigned int index,int nature);
	bool ChangeEquipAddon(unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type,int recipe_type,int* materials_ids,unsigned char* idxs,int count);
	bool ReplaceEquipAddon(unsigned char equip_idx,unsigned char equip_socket_idx,int old_stone_type,int new_stone_type,unsigned char new_stone_idx);
	void OnTouchPointQuery(int64_t income,int64_t remain);
	void OnTouchPointCost(int64_t orderid,unsigned int cost,int64_t income,int64_t remain,int retcode);
	void OnGiftCardRedeem(const char(&cn)[player_giftcard::GIFT_CARDNUMBER_LEN],int type,int parenttype,int retcode);
	void OnAuAddupMoneyQuery(int64_t addupmoney);
	void UpdateDisplayTitle(TITLE_ID titleid);
	void OnObtainRareTitle(TITLE_ID titleid);

	void EnterTrickBattleStep1(int world_tag, int battle_id, int chariot);
	void EnterTrickBattleStep2();
	void OnAutoTeamPlayerReady(int leader_id);
	void OnAutoTeamComposeFailed(int leader_id);
	void OnAutoTeamComposeStart(int member_list[], unsigned int cnt);

	void ReceiveRealmExp(int exp, int daily_exp);
	void UpdateRealmLevel();
	bool ExpandRealmLevelMax();
	bool IsRealmExpFull();
	int GetObtainedGeneralCardCountByRank(int rank);
	void OnMafiaPvPAward(int type,const XID& sourceid, const A3DVECTOR& pos, int mafiaid, int domainid);
	bool CheckVisaValid();
	void Repatriate();
    void OnLookupEnemyReply(const MSG& msg);

	int MnfactionJoinApply(int domain_id);
	int MnfactionLeave();
	bool MnfactionJoinStep1(int retcode, int64_t faction_id, int domain_id, int world_tag);
	bool MnfactionJoinStep2();
	virtual void SetMnfactionDomainID(int mnfaction_domain_id){}
	virtual int GetMnfactionDomainID(){return 0;}
	int GetFixPositionTransmitEnergy()
	{
		return _fix_position_transmit_energy;
	}
	int ReduceFixPositionTransmitEnergy(int value)
	{
		int tmp = _fix_position_transmit_energy - value;
		if(tmp < 0)
		{
			_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_ENERGY_NOT_ENOUGH);
			return -1;
		}
		_fix_position_transmit_energy = tmp;
		_runner->fix_position_energy_info(0, _fix_position_transmit_energy);
		return _fix_position_transmit_energy;
	}
	int AddFixPositionTransmitEnergy(int value)
	{
		int tmp = _fix_position_transmit_energy + value;
		if(tmp < 0)
		{
			_runner->error_message(S2C::ERR_FIX_POSITION_TRANSMIT_ENERGY_MAX);
			return 0;
		}
		_fix_position_transmit_energy = tmp;
		_runner->fix_position_energy_info(0, _fix_position_transmit_energy);
		return _fix_position_transmit_energy;
	}
// arena of aurora
	void EnterArenaOfAuroraStep1(int world_tag, int arena_id);
	void EnterArenaOfAuroraStep2();
// end arena of aurora

// money 172

	void GainGoldWithDrop(unsigned int inc, bool write_log = false);
	void GainGold(unsigned int inc);
	void SpendGold(unsigned int delta,bool write_log = false);
	bool CheckIncGold(unsigned int inc);
	bool CheckDecGold(unsigned int delta);

	void GainSilverWithDrop(unsigned int inc, bool write_log = false);
	void GainSilver(unsigned int inc);
	void SpendSilver(unsigned int delta,bool write_log = false);
	bool CheckIncSilver(unsigned int inc);
	bool CheckDecSilver(unsigned int delta);

	void SpendAllMoney(unsigned int delta,bool write_log = false);
	void SpendGoldMoney(unsigned int delta,bool write_log = false);
	void SpendSilverMoney(unsigned int delta,bool write_log = false);

	bool CheckDecAllMoney(unsigned int delta);


// money 172 end


public:

//��Ҳ�������Ⱥ
//��Щ��������player�Ĳ��������Բ��������Ǵ����
	int  PlayerGetItemInfo(int where, int index);
	int  PlayerGetItemInfoList(int where, unsigned int count , unsigned char *list);
	void PlayerGetInventory(int where);
	void PlayerGetInventoryDetail(int where);
	void PlayerGetTrashBoxInfo(bool detail);
	void PlayerGetPortableTrashBoxInfo(bool detail);
	void PlayerGetUserTrashBoxInfo(bool detail);
	void PlayerExchangeInvItem(unsigned int idx1,unsigned int idx2);
	void PlayerMoveInvItem(unsigned int src,unsigned int dest,unsigned int count);
	void PlayerExchangeTrashItem(int where, unsigned int idx1,unsigned int idx2);
	void PlayerMoveTrashItem(int where, unsigned int src,unsigned int dest,unsigned int count);
	void PlayerTrashItemToInv(int where, unsigned int idx_tra, unsigned int idx_inv, unsigned int count);
	void PlayerInvItemToTrash(int where, unsigned int idx_inv, unsigned int idx_tra, unsigned int count);
	void PlayerExchangeTrashInv(int where, unsigned int idx_tra, unsigned int idx_inv);
	void PlayerExchangeTrashMoney(char is_usertrashbox, unsigned int inv_money, unsigned int tra_money);
	void PlayerDropInvItem(unsigned int index,unsigned int count,bool isProtected);
	void PlayerDropEquipItem(unsigned int index,bool isProtected);
	void PlayerExchangeEquipItem(unsigned int index1,unsigned int index2);
	void PlayerEquipItem(unsigned int index_inv, unsigned int index_equip);
	void PlayerMoveEquipItem(unsigned int index_inv, unsigned int index_equip);
	void PlayerGoto(const A3DVECTOR &pos);
	//void PlayerDropMoney(unsigned int amount,bool isProtected);
	void PlayerGetProperty();
	void PlayerSetStatusPoint(unsigned int vit, unsigned int eng, unsigned int str, unsigned int agi);
	bool PlayerSitDown();
	bool PlayerStandUp();
	void PlayerRestartSession();
	void PlayerAssistSelect(const XID & cur_target);
	bool PlayerUseItem(int where, unsigned int inv_index, int item_type,unsigned int count);
	bool PlayerUseItem2(int where, unsigned int inv_index, int item_type,unsigned int count);
	bool PlayerSitDownUseItem(int where, unsigned int inv_index, int item_type,unsigned int count);
	bool PlayerUseItemWithTarget(int where,unsigned int inv_index,int item_type,char force_attack);
	bool PlayerUseItemWithArg(int where,unsigned int inv_index,int item_type,unsigned int count, const char * buf, unsigned int buf_size);
	bool PlayerOpenPersonalMarket(unsigned int count, const char name[PLAYER_MARKET_MAX_NAME_LEN], int * entry_list);
	bool PlayerTestPersonalMarket();
	bool PlayerDestroyItem(int where, unsigned int index, int item_type);
	void PlayerEnablePVPState();
	void PlayerDisablePVPState();
	void PlayerRegionTransport(int rindex, int tag);
	void PlayerLogout(int type);
	void PlayerDuelRequest(const XID & target);
	void PlayerDuelReply(const XID & target,int param);
	void PlayerBindRequest(const XID & target, int param_id);
	void PlayerBindInvite (const XID & target, int param_id);
	void PlayerBindRequestReply(const XID & target, int param, int param_id);
	void PlayerBindInviteReply (const XID & target, int param, int param_id);
	void PlayerBindCancel();
	int PlayerSummonPet(unsigned int index);
	int PlayerRecallPet();
	int PlayerBanishPet(unsigned int index);
	bool PlayerDoShopping(unsigned int item_count,const int * order, int shop_tid = 0);
	bool ForeignDoShoppingStep1(int goods_id, unsigned int goods_index, unsigned int goods_slot);
	bool ForeignDoShoppingStep2(int item_id, unsigned int count, int expire_time, unsigned int cash_need);
	bool PlayerGetMallItemPrice(int start_index, int end_index);	//lgc
	bool PlayerEquipTrashboxFashionItem(unsigned char trash_idx_body,unsigned char trash_idx_leg,unsigned char trash_idx_foot,unsigned char trash_idx_wrist,unsigned char trash_idx_head,unsigned char trash_idx_weapon); 
	bool CheckEquipTrashboxItem(item_list & trashbox, unsigned char trash_idx,unsigned char equip_idx);
	bool PlayerEquipTrashboxItem(int where, unsigned char trash_idx,unsigned char equip_idx); 
	void PlayerCheckSecurityPasswd(const char * passwd, unsigned int passwd_size);	
	void PlayerNotifyForceAttack(char force_attack, char refuse_bless);
	bool PlayerDoDividendShopping(unsigned int item_count,const int * order, int shop_tid = 0);
	bool PlayerGetDividendMallItemPrice(int start_index, int end_index);	//lgc
	bool PlayerChooseMultiExp(int index);
	bool PlayerGetFactionFortressInfo();
	bool TeamCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req);
	bool TeamMemberCongregateRequest(const XID& member);
	bool FactionCongregateRequest(int reply_level_req, int reply_sec_level_req, int reply_reincarnation_times_req);
	void RecvCongregateRequest(char type, int sponsor, int world_tag, const A3DVECTOR& pos, int level_req, int sec_level_req, int reply_reincarnation_times_req);
	void PlayerCongregateReply(char type, char agree, int id);
	void PlayerGetDpsDphRank(unsigned char rank_mask);
	int PlayerJoinForce(int force_id);
	int PlayerLeaveForce();
	void PlayerProduce4Choose(bool remain);
	void PlayerRechargeOnlineAward(int type, unsigned int count, int * list);
	bool PlayerGetCountryBattlePersonalScore();
	bool PlayerGetCountryBattleStrongholdState();
	bool PlayerGetCountryBattleLiveShow();
	void PlayerLeaveCountryBattle();
	int CheckChangeDs(int type,int climit,int item,int item_count,int level,int sec_lvl,int realm_lvl);
	int PlayerTryChangeDS(int flag);
	void PlayerChangeDSLogout(int flag);
	void MakeVisaData(int type,int stay_timestamp,int item,int item_count);

	void PlayerExchangeWanmeiYinpiao(bool is_sell, unsigned int count);	//����̯״̬�¿ɲ���
	bool GenerateNatureSkills(pet_data *pData); 
	void PlayerAcceptRebuildInheritResult(bool isaccept);
	void PlayerAcceptRebuildNatureResult(bool isaccept);
	void PlayerGivePresent(int roleid, int mail_id, int goods_id, int goods_index, int goods_slot);
	void PlayerAskForPresent(int roleid, int goods_id, int goods_index, int goods_slot);
	int PlayerTryRefineMeridian(int index);
	void PlayerAddFreeRefineTimes(int num);
	int PlayerDecomposeFashionItem(unsigned int inv_index, int fashion_type, unsigned int fee, int production_type);
	void PlayerTouchPointQuery();
	void PlayerTouchPointCost(unsigned int index,int type,unsigned int count,unsigned int price,int expire,unsigned int lots);
	void PlayerRedeemGiftCard(const char(&cn)[player_giftcard::GIFT_CARDNUMBER_LEN]);
	void PlayerQueryTitle(int roleid);
	void PlayerChangeTitle(TITLE_ID titleid);
	void PlayerDailySignin();
	void PlayerLateSignin(char type,int itempos,int desttime);
	void PlayerApplySigninAward(char type ,int mon);
	void PlayerRefreshSignin();
	void PlayerSwitchInParallelWorld(const instance_hash_key & key);
	void PlayerQueryParallelWorld();
	void PlayerReincarnation();
	void PlayerGetReincarnationTome();
	void PlayerRewriteReincarnationTome(unsigned int record_index, int record_level);
	void PlayerActiveReincarnationTome(bool b);
	int PlayerTrickBattleApply(int chariot);
	void PlayerLeaveTrickBattle();
	void PlayerSetAutoTeamGoal(char goal_type, char op, int goal_id);
	void PlayerJumpToGoal(int goal_id);
	int PlayerGeneralCardRebirth(unsigned int major_inv_idx, unsigned int minor_inv_idx);
	int PlayerSwallowGeneralCard(unsigned int equip_idx, bool is_inv, unsigned int inv_idx, unsigned int count);
	void PlayerQueryChariots();
	int PlayerImproveFlysword(unsigned int inv_idx, int flysword_id);
	void PlayerRandMallQuery(int eid);
	void PlayerRandMallRoll(int eid);
	void PlayerRandMallPay(int eid);
	void PlayerQueryMafiaPvPInfo();
    void PlayerQueryCanInheritAddons(int equip_id, unsigned int inv_idx);
    void ActivateRegionWayPoints(int num, int waypoints[]);
	void PlayerReenterInstance();
    void PlayerStartEnterSanctuarySession();
    void PlayerAddPVPLimitFilter();
	int PlayerAstrolabeSwallow(int type, int inv_idx, int item_id); // Novo Astrolábio
	int GetAstralLevelRestriction(int level); // Novo Astrolábio
	int GetAstrolabeLevelMax(); // Novo Astrolábio
	int GetAstralLevel(int index); // Novo Astrolábio
	int  PlayerAstrolabeAddonRoll(int times, int limit, int inv_idx, int item_id, int (&args)[3] );
	int  PlayerAstrolabeAptitInc(int inv_idx, int item_id);
	int  PlayerAstrolabeSlotRoll(int inv_idx, int item_id);
	int  PlayerAstrolabeLockUnlock(char inc);
	int  PlayerAstrolabeAddonSave(short * addon_pos);
	void PlayerSoloChallengeSelectStage(int stage_level);
	void PlayerSoloChallengeStartTask(bool isStartSuccess);
	void PlayerSoloChallengeStageComplete(bool isCompleteSuccess);
	int  PlayerSoloChallengeUserSelectAward(int stage_level, int args[]);
	int  PlayerSoloChallengeScoreCost(int filter_index, int args[]);
	int  PlayerSoloChallengeClearFilter(int args[]);
	void PlayerEnterSoloChallengeInstance();
	void PlayerDeliverSoloChallengeScore(int score);
	int  PlayerSoloChallengeLeaveTheRoom();
	inline void PlayerResetSoloTower()
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(_parent->ID.id,windex1);
		if(gPlayer && gPlayer->imp)
		{
			gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
			_solochallenge.SetResetSolloTower(pImp);
		}
	}

	inline void PlayerTeasouresReset()
	{
		memset(&_lottery, 0, sizeof(_lottery));
		memset(&_treasure_items, 0, sizeof(_treasure_items));
		memset(&_treasure, 0, sizeof(_treasure));

		GetLoteryInfo();
		GetLoteryItems();
		TreasureInfo();				
	}
	void PlayerLeaveSoloChallengeInstance();
	void PlayerFixPositionTransmitAdd(float *pos, const char *position_name);
	void PlayerFixPositionTransmitDelete(int index);
	void PlayerFixPositionTransmit(int index);
	void PlayerFixPositionTransmitRename(int index, char *position_name);
	void PlayerSendAllFixPositionInfo();
	bool PlayerGetCashVipMallItemPrice(int start_index, int end_index);
	bool PlayerDoCashVipShopping(unsigned int item_count,const int * order, int shop_tid = 0);
	
    void SendRefineSuccessMessage(int npcid, unsigned int index, int refine_level);
    void SpecialItensNotify(int item_id);

    /*156+*/
	bool CanEnterArenaQueue(int is_6x6);

	int PlayerArenaOfAuroraApply();
	void PlayerLeaveArenaOfAurora();

	inline int ArenaGetMoneyRequired(int mode)
	{
		return ArenaConfig::GetInstance()->GetMoneyRequired(mode);
	}

// money 172 
	void PlayerDropGold(unsigned int amount,bool isProtected);
	void PlayerDropSilver(unsigned int amount,bool isProtected);
// money 172 end


public:
//���߲�������
	void LostConnection(int offline_type = PLAYER_OFF_OFFLINE);
	void KickOut() { LostConnection(PLAYER_OFF_KICKOUT); }
	void SendLogoutRequest(int type, int retcode = 0);
    instance_hash_key  GetLogoutInstanceKey();

protected:
//˽�еĺ����߼�
	static int DoPlayerMarketTrade(const XID & trader,const XID & customer ,gplayer * pTrade, gplayer *pBuyer, const void * order, unsigned int length);		//���̵깺��
	static int DoPlayerMarketPurchase(const XID & trader,const XID & customer,gplayer * pTrade, gplayer *pBuyer, const void * order, unsigned int length);		//���̵�����
	void DoPlayerMarketTrade(player_stall::trade_request & req, gplayer_imp * pImp, unsigned int need_money, unsigned int money_to_yinpiao);
	bool CheckMarketTradeRequest(player_stall::trade_request & req, unsigned int &need_money, unsigned int& money_to_yinpiao, gplayer_imp * pBuyerImp);
	bool CheckMarketTradeRequire(player_stall::trade_request & req, unsigned int need_money, gplayer_imp * pTraderImp);
	bool CancelPersonalMarket();

	bool CheckMarketPurchaseRequest(player_stall::trade_request & req,unsigned int & total_price, gplayer_imp * pBuyerImp);
	bool CheckMarketPurchaseRequire(player_stall::trade_request & req,unsigned int total_price, unsigned int& money_to_yinpiao, gplayer_imp * pTraderImp);
	void DoPlayerMarketPurchase(player_stall::trade_request & req, gplayer_imp * pImp, unsigned int total_price, unsigned int money_to_yinpiao);

	int Repair(item & it,int where,int index);
	void RefreshEquipment();		//���¼���װ������
	void RefreshCardEquipment();
	int SelectRandomArmor();		//ѡ������ĸ�װ�����м����;öȵ�ѡ��
	bool EquipItem(unsigned int index_inv,unsigned int index_equip);	//װ����Ʒ
	void PlayerWeaponUpdateEnterWorld();	//װ����Ʒ
	void Logout(int type);			//�ǳ�����

	void QueryOtherPlayerInfo1(unsigned int count, int id[]);
	void QueryNPCInfo1(unsigned int count, int id[]);

	inline int DispatchNormalMessage(world * pPlane, const MSG & msg)
	{
		if(!(gplayer*)_parent->IsZombie())
		{
			return MessageHandler(pPlane,msg);
		}
		else
		{
			return ZombieMessageHandler(pPlane,msg);
		}
	}
	void PutItemAfterTrade(item_list & backpack);
	void DropAllAfterTrade(item_list & backpack,unsigned int money);
	void AutoSaveData();
	int DoSwitch(const MSG & msg);
	void FromSwitchToNormal();
	void IncExp(int& exp,int& sp,float double_exp_sp_factor=1.0f, bool double_sp = false);	 //ֱ�ӼӾ��飬�������κ����� �����������ᵽ����
	void ReceiveExp(int exp,int sp);			//ֱ�ӼӾ��飬�����м���ͷ��������о���ӳ�
	void ReceiveExp(const msg_exp_t & entry);		//����о���ͷ�
	void ReceiveGroupExp(const A3DVECTOR &pos,int total_damage,int exp,int sp,int level,unsigned int count,int npcid, int npctag, int npc_planeindex, float r, const msg_grpexp_t * list);
	void LevelUp();
	void PickupMoneyInTrade(unsigned int money);
	void PickupItemInTrade(const A3DVECTOR & pos,const void * data, unsigned int size,bool isTeam, int drop_id);
	void SendServiceRequest(int service_type, const void * buf, unsigned int length);
	void QueryServiceContent(int service_type);
	void ThrowInvItem(unsigned int index, unsigned int count, bool isProtected, unsigned char drop_type, const XID & spec_owner=XID(-1,-1));
	void ThrowEquipItem(unsigned int index, bool isProtected,unsigned char drop_type, int throw_count = 0, const XID & spec_owner=XID(-1,-1));
	//void ThrowMoney(unsigned int amount, bool isProtected);
	void ReduceSaveTimer(int counter) { _write_timer -= counter;}
	
	// money 172
	void ThrowGold(unsigned int amount, bool isProtected);
	void ThrowSilver(unsigned int amount, bool isProtected);
	// money 172 end

	template <int foo>
	inline void DoWeaponOperation()
	{
		bool is_update = false;
		_equipment[item::EQUIP_INDEX_WEAPON].AfterAttack(_equipment,&is_update);
		if(is_update)
		{
			RefreshEquipment();
		}
	}

public:	//lgc
	void x_LevelUp() { LevelUp(); }
	bool IsElfEquipped(){return _cur_elf_info.id != (unsigned int)-1;}
	void UpdateCurElfInfo(unsigned int id, short refine_level, short str, short agi, short vit, short eng, const char * skilldata, int cnt);
	void ClearCurElfInfo();
	void ClearCurElfVigor(){ _cur_elf_info.vigor = 0.0f; }
	void UpdateElfProp(); //��elf_enhance���µ�_cur_elf_info��final_...��
	void UpdateElfVigor();//����С����Ԫ�����

	inline void OnRefreshEquipment()
	{
		RefreshEquipment();	
	}
	
	void UpdateMinElfStatusValue(int value);
	void UpdateAllElfSecureStatus();
	void TriggerElfRefineEffect();
	bool IsElfRefineEffectActive(){return _cur_elf_info.id != (unsigned int)-1 && _cur_elf_info.refine_effect_active;}
	void ElfRefineActivate(short refine_level);
	void ElfRefineDeactivate(short refine_level);
	
	void ElfAddAttribute(short str, short agi, short vit, short eng);
	void ElfAddGenius(short g0, short g1, short g2, short g3, short g4);
	void ElfPlayerInsertExp(unsigned int exp, char use_sp);
	unsigned int ElfInsertExpUsePill(unsigned int exp, int exp_level);
	void ElfReceiveExp(unsigned int exp);
	void ElfEquipItem(unsigned int index_inv);
	void ElfChangeSecureStatus(int status);
	void CastElfSkill(unsigned short skill_id, char force_attack, int target_count, int * targets);
	bool RechargeEquippedElf(unsigned int index, unsigned int count);	//��ɽ��������߼���ȫ��ͬ
	
	void ElfDecAttribute(unsigned int inv_idx_elf, unsigned int inv_idx_ticket, short str, short agi, short vit,  short eng);
	void ElfFlushGenius(unsigned int inv_idx_elf, unsigned int inv_idx_ticket);
	void ElfLearnSkill(unsigned int inv_idx_elf, unsigned short skill_id);
	void ElfForgetSkill(unsigned int inv_idx_elf, unsigned short skill_id, short forget_level);
	void ElfRefine(unsigned int inv_idx_elf, unsigned int inv_idx_ticket, int ticket_cnt);
	void ElfRefineTransmit(unsigned int inv_idx_src, unsigned int inv_idx_dst);
	void ElfDecompose(unsigned int inv_idx_elf);
	void ElfDestroyItem(unsigned int inv_idx_elf, int mask, unsigned int inv_idx_equip);
	virtual void UpdateStallInfo(int id, int max_sell, int max_buy, int max_name);
	virtual void ClearStallInfo();
	virtual void OnStallCardTakeOut();
	virtual bool Produce4ChooseExec(const recipe_template & rt, int equip_id, int equip_inv_idx, char inherit_type, void **pItem, unsigned short crc, int eq_refine_level, int eq_socket_count, int eq_stone_type[], addon_data eq_engrave_addon_list[3], unsigned int eq_engrave_addon_count);
	void PlayerRenameRet(const void *new_name, unsigned int name_len, int ret);
	void PlayerForbidLink();

	//obj_interface�ӿ�	
	bool OI_GetElfProp(short& level, short& str, short& agi, short& vit, short& eng);
	int OI_GetElfVigor(){return ((_cur_elf_info.id != (unsigned int)-1) ? (int)_cur_elf_info.vigor : -1);}
	int OI_GetElfStamina(){return _equipment[item::EQUIP_INDEX_ELF].GetStamina();}
	bool OI_DrainElfVigor(int dec);
	bool OI_DrainElfStamina(int dec);

//debug only
	void dump_elf_info();
	void get_mall_detail();	//��ȡ��ǰ�̳��д�������ʱ����Ƶ���Ʒ����ϸ��Ϣ
	void change_elf_property(int index, int value);
	void change_factionfortress(int index, int value);
    void copy_other_role_data(int src_roleid);
//debug only


//NEW FUNCTIONS
	void S2CGameDataSend(void * buf, unsigned int size);
	plant_pet_manager & GetPlantPetMan() { return _plantpetman; }
	pet_manager & GetPetMan() { return _petman; }
	inline carrier_data & GetCarrierStorage() { return _carrier; }
	
	virtual void SetBodyScale(float scale);
	inline const char16_t * GetItemName(unsigned int it, unsigned int & len);
	inline void SetHwid(unsigned long long value) { _hwid = value; }
	inline unsigned long long  GetHwid() { return _hwid; }
	
	virtual void EnhanceShieldEnergyMax(int val) { _shield_energy_max += val; }
	virtual void ImpairShieldEnergyMax(int val)	{ _shield_energy_max -= val; }
	virtual void EnhanceShieldGen(int val) { _shield_energy_gen += val;	}
	virtual void ImpairShieldGen(int val) { _shield_energy_gen -= val; }

	void ShieldEnergyHeartBeat();
	virtual bool ShieldEnergyUseSkill(float cost);
	virtual bool EnhanceShieldEnergy(float cost);
	virtual bool ImpairShieldEnergy(float cost);

	inline int GetPlayerKillCount() { return _player_kill; }
	inline int GetMonsterKillCount() { return _monster_kill; }
	inline void SetPlayerKillCount( int val ) { _player_kill = val; }
	inline void SetMonsterKillCount(  int val ) { _monster_kill = val; }

	inline bool GetSaledModeSilent()  {	return (_seal_mode_flag & gactive_imp::SEAL_MODE_SILENT); }
	inline void SetBindParam(int value) { _bind_state = value; }
	inline int  GetBindParam() { return _bind_state; }
	
	inline double GetLuaStorage(int pos) {	return _glua.GetLuaStorage(pos); }
	inline void SetLuaStorage(int pos, double value) {	_glua.SetLuaStorage(pos , value); }
	inline double GetLuaUserStorage(int pos) { return _glua.GetLuaUserStorage(pos); }
	inline void SetLuaUserStorage(int pos, double value) { _glua.SetLuaUserStorage(pos, value); }
	
	inline cd_manager GetCooldown() { return _cooldown; }
	
	void GetEquipSuite();
	bool GetTaskFinishExist(short uID);
	int SetAutoSwap(void *buf, int size);
	int AutoSwapRelease(int swap_count);
	
	bool ItemSetEquip(unsigned int it_index , unsigned int eq_index);
	bool InstallEquipStone(int chip_id, unsigned int chip_index, int equip_index);
	virtual void ShortJump(A3DVECTOR & newpos);
	virtual void ShortJump(A3DVECTOR newpos, int tag);

	int CheckNPCSopDistance(int id_item_list,const void *buf, unsigned int size);
	
	virtual bool CooldownManager(char type, int idx, time_t time);
	virtual bool CooldownManagerMassive(char type, unsigned int * table);
	
	unsigned int InvPlayerGiveItem(int id, int count);
	unsigned int InvPlayerGiveItem(int inv, int id, int count, int time, int proctype);
	unsigned int InvPlayerSpendItem(int inv, int id, int count);
	unsigned int InvPlayerSpendItem(int inv, int id);
	
	virtual int GetInvPosItemTable(int inv , int * table);
	virtual int GetInvIdItemTable(int inv , int * table);
	virtual int GatcherMineSupressor(int mine_id , int time);
	virtual void SetAllCoolDown(int id1, int id2, int msec, int except_id);
	
	void LearnSkill(int skill_id, int skill_level);
	void DeleteSkill(int skill_id);

	void PrivateChat(int roleid, char channel, const void * buf, unsigned int len, const void * aux_data, unsigned int dsize);
	void GetLoteryInfo();//-- Новая лотерея 1.6.5+
	void GetTreasureInfo();//-- Новая лотерея 1.6.5+
	void GetLoteryItems();//-- Новая лотерея 1.6.5+
	void LotteryAward();//-- Новая лотерея 1.6.5+
	void Lottery2Init(unsigned int param); //-- Новая лотерея 1.6.5+
	void Lottery2OpenBox(unsigned int param);//-- Новая лотерея 1.6.5+
	void LotteryGiveItem(unsigned int pItem);//-- Новая лотерея 1.6.5+
	void TreasureInit(unsigned int param);//-- Новая лотерея 1.6.5+
	void GetTreasureOpenBox(unsigned int lot);//-- Новая лотерея 1.6.5+
	void TreasureInfo();//-- Новая лотерея 1.6.5+
	bool FactionAllianceCongregateRequest(int reply_level_req = 0, int reply_sec_level_req = 0, int reply_reincarnation_times_req = 0);
    bool ChangeLeaderAssist();
	bool SetAutoAssist();
	void S2CGameDataSendElse(void * buf, unsigned int size);
	void SendPlayerOtherData(bool, int);
	void SetColorName(unsigned int value);
	unsigned int GetEquipLevel();
		
	bool NewEngraveItem(const engrave_recipe_template& ert, unsigned int inv_index, addon_data * session_addon_list, unsigned int session_count);
	void PlayerEngraveChoose(bool remain);
	bool EngraveChooseExec(const engrave_recipe_template& ert, unsigned int inv_index, addon_data * session_addon_list, unsigned int session_count);

	// Trash
	void LoadTrashBox(GDB::itemlist & box, unsigned int box_num);
	void SaveTrashBox();
	
	//glyph 
	bool SetGlyphSkill( int m_skill, unsigned int m_pos );
	int MakeGlyphSkill( glyph_skill_data & gsd , unsigned int m_pos );
	int GlyphGetAttr();
	void GetSkillAttr();
	void GetGlyphData();
	bool GlyphSkillInc(int m_skill, unsigned int m_pos);
	bool GlyphSkillDec(int m_skill, unsigned int m_pos);
	int  GlyphActivate(unsigned int m_pos);
	int  GlyphActivate();
	int  GlyphDectivate(unsigned int m_pos);
	int  GlyphDectivate();
	void GlyphUpdate(unsigned int m_pos);
	void GlyphUpdate();
	int  GlyphEquipRefresh();
	bool GlyphVerifyInv(char where, unsigned char src, unsigned char dst, unsigned int count, bool is_move = false);
	int  GlyphPlayerMoveTrashItem(int where, unsigned int src,unsigned int dest,unsigned int count);
	void GlyphAttrRelease(int else_attr, int & damege);
	bool UseGlyphUnlockItem(unsigned int pos, int rank, int level);
	bool GlyphItemUpgrade( C2S::CMD::glyph_item_upgrade::req_item * req );
	//glyph  end


	// Trash
	bool GetTrashBoxPosItem(int where, int id, int & pos);
	bool GetTrashBoxItemByTable(int where, int * table, int & id, int & pos);
	bool GiveTrashBoxItem(int where, int id, int count=1, int time=0, int proctype=-1);
	bool SpendTrashBoxItem(int where, int id, int count=1);
	bool SpendTrashBoxItem2(int where, int inv, int count);

	// Easy Produce
	bool ProduceEasyItem(unsigned int recipe_id, int times);
	bool ProduceCardItem(int count_itens, int recipe_id, int * index_inv);

	// New card info
	bool DecomposeCardS(int index_inv_card);
	bool RestaureCardS(int index_inv_card);
	
	// Bible Refine New
	bool RefineBibleAddon(unsigned int index, int item_type,int rt_index, int npcid);
	int RefineBibleTransmit(unsigned int src_index, unsigned int dest_index);

	// Inventário de Voos


	// Lib Produce 
	bool ProduceLibItem(const lib_recipe_template & rt, int * materials, int * counts, int * idxs);
	void GetLibItems();
	void LibGiveItem(unsigned int pItem);

	virtual bool CheckForbidItemArena(int item_id) { return true; };
	virtual bool CheckForbidSkillArena(unsigned int skill_id) { return true; };
	unsigned int GetTableDailyExp(unsigned int level);
	void SystemRealmExpDaily(unsigned int mobid);	
	void CheckRealmDay();
	
	inline gplayer_lua * GetLua() { return & _glua; }
	inline gplayer_lua::LUA_ROLE * GetLuaRole() { return _glua.GetRole(); }
	inline gplayer_codex * GetCodex() { return & _codex; }
	inline gplayer_kid * GetKid() { return & _kid; }
	inline gplayer_kid_addons * GetKidAddons() { return & _kid_addon; }
	virtual bool SummonPet3(int pet_egg_id, int skill_level, int life_time, const XID & target, char force_attack);
	
	bool RefineMaterial(int item_type,int item_count, int inv_index, int refine_material, int inv_refine_material);
	void ChecPetChell();

	//Carrier System

	bool EnterCarrier(unsigned int npcid, int pos, int reserved1);
	bool LeaveCarrier(unsigned int npcid, int pos);

	// Lock System

	void ChangeSafeLockNew(bool b);
	void ChangeSafeLockActive(char inc);

	// Repository

	int GetItensStorageRepository();
	void ProtocolRepositoryStorage();
	bool UnlockRepositorySystem();	
	void CheckValidRepositoryItem(int item_id, item & iqt);
	bool CheckStorageItem(int item_id);
	bool CheckActiveItem(int item_id);
	void ChangeRepositoryHistory(int item_id, int index);
	void ProtocolRepositoryHistory();
	bool EquipRepositoryItem(int item_id, int index);
	void ClearRepositoryAddons();
	void ActivateRepositoryAddons();
	void DeactivateRepositoryAddons();
	inline bool HasRepositoryLock() { return _repository.lock; }
	
	// Imperial Spirit
	inline bool HasImperialSpiritEnabled() { return _glua.GetImperialEnabled() && _glua.GetImperialExpireTime () > 0 && _glua.GetImperialExpireTime () > g_timer.get_systime() ? true : false; }
	void ImperialSpirtHeartBeat(bool is_active = false);
	void SetImperialSpirtActivate();

	/*176+*/
	void SetPortatilPicture(int idx);
	void ProtocolPortatilPicture();

	// Novos vstate Paladino
	virtual void GetPaladinWeaponLight( unsigned char state );

	// G17
	int GetClassPos();
	uint16_t floatTohalf(float value);
	bool SetIncSpiritArmor(unsigned int idx_pos_spirit, unsigned int idx_pos_armor, bool apply = false);
	bool SetDecSpiritArmor(unsigned int idx_pos_armor, bool remove = false);
	bool SetAwakeningSpirit(unsigned int idx_inv, int count);
	bool SetDecomposeSpirit(unsigned int count_itens, int * idx_pos);
	bool SetLockSpirit(unsigned int idx_inv, int lock);
	bool SetExtractSpirit(unsigned int idx_inv_rmv, unsigned int idx_inv_rcv);
	bool SetPurificationSpirit(unsigned int idx_inv, int* addons_lock_pos, addon_data * addons_new, addon_data * addons_notify, int addons_count_notify, int count_rem_item);
	bool SetPurificationChangeSpirit(unsigned int idx_inv, int change);
	bool SetPurificationChangeSpiritExec(unsigned int idx_inv, addon_data * addons_new);
	
	bool SetIncCrystalArmor(unsigned int idx_pos_crystal, unsigned int idx_pos_armor, bool apply = false);
	bool SetDecCrystalArmor(unsigned int idx_pos_armor, bool remove = false);
	bool SetLockCrystal(unsigned int idx_inv, int lock);
	bool SetDevourCrystal(unsigned int count_cystals, int * pos_crystal_consum);

	void RefreshInventoryNewArmorEnter(bool trade = false, bool equip = false);
	void UpdateInventoryType(item_list& itemList, int inventoryType, bool refreshEquipment);

	// Novas cargas do mercenário
	virtual void EnhanceChargeMerc(int val); 
	virtual void ImpairChargeMerc(int val);
	virtual int GetCountChargeMerc(){ return _charge_merc; }
	void ChargeMercHeartBeat();
	void GetChargeMerc();
	
	// cd skill limit
	virtual void GetClearSkillCD(int skill_id);
	virtual void GetUseSkillCD(int skill_id, unsigned int cdlimit, unsigned int coolingtime);
	
	// money 172
	void SelfPlayerMoney();
	bool ConvertMoney(unsigned int money);
	bool SyncMoney(unsigned int money);
	bool IsGoldItem(int item);
	
	
	bool PlayerSpendGold(unsigned int money);
	bool PlayerGiveGold(unsigned int money);
	
	bool PlayerSpendSilver(unsigned int money);
	bool PlayerGiveSilver(unsigned int money);
	
	int PlayerChangeMoney(bool mode);
	
	// money 172 end

	/*172+*/
	void ProtocolWorldPoints();
	bool ManagerWorldPoints(unsigned int value_task, unsigned int count_tasks);
	void RewardWorldPoints();
	
	/*160+*/
	void SpeedSkillManager();
	/*174+*/
	void ActivityEventActivate();
	void ActivityEventSkillManager();
	bool ActivityRewardLogin(int idx, int pos);
	bool ActivityShopBuy(int idx, int pos, int count);
	bool CheckActivityDay();
	void ProtocolActivityEvent();
	bool CheckEventValidTask(int task_id);

	/*156+*/	
	bool EC_ArenaTeamInviteRequest(int inviting_roleid, int team_type, int invited_roleid, int idx);
	bool EC_ArenaTeamInviteRequest_Re(int inviting_roleid, int team_type);
	bool EC_ArenaTeamInvite(int idx, int team_type, int inviting_roleid, int invited_roleid, int invited_cls);
	
	bool EC_ArenaTeamKickout(int idx, int roleid, int kickid, int team_type);
	bool EC_ArenaTeamCaptainTransfer(int idx, int roleid, int transferid, int team_type);
	bool EC_ArenaTeamLeave(int idx, int roleid, int team_type);
	bool EC_ArenaQuery(int roleid);

	bool ArenaBattleEnter(int roleid, int is_team, int is_6x6);

	// Memorial Celestial
	bool CheckRedBookUpgradeItem(int item_id, bool key = false);
	void ProtocolCelestialMemorial();
	void TaskExpCelestialMemorial(int swallow_exp);
	void ConsumCelestialMemorial(int idx_inv);
	bool HasCelestialMemorialOpen();

	void ProtocolCelestialMemorialLottery();
	void ProtocolCelestialMemorialLotteryReward(unsigned int idx_pos_type);
	void GetCelestialMemorialReward(unsigned int pos, unsigned int type);
	void GetCelestialReward(unsigned int idx_pos_storage);
	void InitCelestialLottery(unsigned int idx_pos_type /*0,1,2*/);
	void GetCelestiralLotteryReward(unsigned int idx_pos_type /*0,1,2*/);

	/*170+ Códice*/
	void FashionUpdateActivate (bool enter_world = false);
	bool FashionCodexApplyStorageSingle (int idx);
	bool FashionCodexChange (int idx, int fashion_id, int color);
	unsigned short GetCodexColorGenerate(int item_id);
	bool FashionCodexApplyStorageMutiple (int idx, int * fashion_pos);
	bool FashionCodexSetActivate (unsigned int size, const void * buf);
	unsigned short GetDyeCodexColorGenerate(int item_id);
	int CodexDyeFashionItem (int mode, int fashion_count, int dye_index, int * fashions);
	bool CodexRewardTitle (int type);
	bool CodexRewardFirst (int type, int idx);
	bool PetCodexApplyStorageSingle (int idx);
	bool FlyCodexApplyStorageSingle (int idx);
	int CodexPlayerImproveFlysword(int mode, int fashion_count, int dye_index, int * fashions);
	bool FlyCodexChangeEquip (int fly_id);
	bool CodexAddPetToSlot(int pet_egg_id);
	bool ConvertPetToCodex(unsigned int slot_index);
	bool CodexChangePetName(unsigned int pet_id,const char name[] , unsigned int name_len);
	bool CheckProctypeCodexCondition(unsigned int index);

	/*170+ Bebe Celestial*/
	void KidAwakeningNameProtocol ();
	void KidAwakeningInfoProtocol ();
	void KidAwakeningCashProtocol ();
	void KidAwakeningPercProtocol ();
	int GetCardLevel(std::mt19937& rng, std::uniform_real_distribution<double>& dist, int level);
	void GenerateCards(int level);
	bool KidAwakeningCreate (char type, char name_len, const char name[]);
	bool KidAwakeningNewDay ();
	int KidGetSuitePoints ();
	bool KidAwakeningNewDay2 ();
	bool KidAwakeningCardLevel ();
	bool KidAwakeningCardRandom ();
	bool KidAwakeningCardPutInventory(char old_slot);
	void KidAwakeningCardSwitchInventory(char new_slot, char old_slot1, char old_slot2);
	bool KidAwakeningCardRemoveInventory(char old_slot);
	void KidAwakeningCardMoveEquipInventory(char old_slot, char new_slot);
	void KidCelestialInfoProtocol (int type = 0);
	bool KidAwakeningNewDay3 ();
	void SendClientMsgChild(char* child_name, int child_name_len, int type);
	void KidCelestialActivityProtocol ();
	bool KidCelestialActivity(int val1, int val2, int val3);
	bool KidCelestialUpgradeRank(int celestial_idx, int where, int inv_idx);
	int KidCelestialDebrisLevelUp(int idx, int exp);
	void KidCelestialTransformation(int mode);

	void KidUnlockNewDay ();

	// Segurança, para evitar sobrecarga
	bool GetCheckCodexStorage() { return _check_codex_get_storage; }
	void SetCheckCodexStorage(bool b) { _check_codex_get_storage = b; }

	// Passe de Batalha
	int GetEquipSlotCount ();
	//nw

	inline char GetEnabledFashionWeapon() { return _enabled_fashion_weapon; }
	inline void SetEnabledFashionWeapon() {	_enabled_fashion_weapon ^= 1; }
	inline gplayer_lottery_items & GetLotteryItems() { return _treasure_items; }
	inline gplayer_safe_lock_passwd & GetSafePasswd() { return _passwd_safe; }

	bool GetActivityEnableSkill() { return _activity.enable_skill; }
	inline void SetActivityEnableSkill(bool b) { _activity.enable_skill = b; }

	void SetPetSkillTemp(int *skills, int count);
	void SetPetSkinStorage(int value, int pos);
	void SetPetSkinStoragePos(int value, int pos);
	void SetPetSkinStorageBuy(int value, int pos);

	inline int GetPetSkillTempId(unsigned int pos) { return pos < 32 ? _pet_skill_temp.skill_id[pos] : 0; }
    inline int GetPetSkillTempLevel(unsigned int pos) { return pos < 32 ? _pet_skill_temp.skill_level[pos] : 0; }
    inline int GetPetSkinStorage(unsigned int pos) {    return pos < 32 ? _pet_skin.storage[pos] : 0; }
    inline int GetPetSkinStoragePos(unsigned int pos) {    return pos < 32 ? _pet_skin.storage_pos[pos] : 0; }
    inline int GetPetSkinStorageBuy(unsigned int pos) {    return pos < 32 ? _pet_skin.storage_buy[pos] : 0; }

	bool DBLoadNewData( const GDB::vecdata_new & base );
	bool DBSaveNewData( GDB::vecdata_new & base );
	//nw end
	void SetGlyphRange(int type, unsigned int count, int mode_1, int range_plus_1, int mode_2, int range_plus_2);

	void SetGlyphInfo(int type, int count, int feature1, int value1, int feature2, int value2, int feature3, int value3);

	//void CheckQuestionDay();
	//void QuestionTask();
	//void QuestionHeartBeat();
	//void QuestionStart();
	//void QuestionAnswered(int selected);
	//void QuestionReward(int progress);

	//void QuestionTaskCMD(int mode, int selected);

	//int GetQuestion();

	void ImmuneTurrent(int test);
	void AutoGenStatAmulet(int cooldown_idx, int idx ,int offset, bool bSave);
	void SetAnecdotePoints();
	void ProtcolAnecdotePoints();
	int OI_GetPetType();

//NEW FUNCTIONS END

	void FixChildSystem();
	void FixExpHeartBeat();
};

void TrySwapPlayerData(world * pPlane,const int cid[3],gplayer * pPlayer);

#endif
