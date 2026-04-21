#ifndef __ONLINEGAME_GS_PLAYER_CELESTIAL_H__
#define __ONLINEGAME_GS_PLAYER_CELESTIAL_H__

#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>

#include "luamanager.h"
#include "celestial_memorial_manager.h"

class gplayer_celestial
{
public:
	struct CELESTIAL_M_LOTTERY
	{
		int _item_id_1;
		int _item_count_1;
		int _award_type_1;

		int _item_id_2;
		int _item_count_2;
		int _award_type_2;
	};

	struct CELESTIAL_MEMORIAL_STRUCT
	{
		bool _type;
		int _memorial_level;
		int _memorial_exp;
	};

	struct REWARD
	{
		short item_id;
		short item_pos;
	};

	struct REWARD_STORAGE
	{
		int _reward[CelestialMemorialConfig::MAX_STORAGE_LOTTERY];
	};
public:

	bool _has_celestial_memorial_unlock; // true = unlock , false = lock
	CELESTIAL_MEMORIAL_STRUCT _celestial_memorial_struct;
	CELESTIAL_M_LOTTERY _celestial_m_lottery[CelestialMemorialConfig::LOTTERY_COUNT];
	int _celestial_perg_value[CelestialMemorialConfig::LOTTERY_COUNT];
	int _celestial_ticket_value[CelestialMemorialConfig::TICKET_COUNT];

	REWARD_STORAGE _storage[CelestialMemorialConfig::LOTTERY_COUNT];

	short _celestial_reward_count;
	REWARD _celestial_reward_pos[CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1];		
	short _celestial_reward_count_2;
	REWARD _celestial_reward_pos_2[CelestialMemorialConfig::MAX_LEVEL_RED_BOX];		

	short _count_lottery[CelestialMemorialConfig::LOTTERY_COUNT];

	short _new_max_level;
	short _use_perg_prob[CelestialMemorialConfig::LOTTERY_COUNT];

public:

	gplayer_celestial() { memset(this, 0x00, sizeof(*this)); }
	~gplayer_celestial() {}
	
	inline void Init()
	{
		_has_celestial_memorial_unlock = 0;
		memset(&_celestial_memorial_struct,0,sizeof(_celestial_memorial_struct));
		memset(&_celestial_m_lottery,0,sizeof(_celestial_m_lottery));
		memset(&_celestial_perg_value,0,sizeof(_celestial_perg_value));
		memset(&_celestial_ticket_value,0,sizeof(_celestial_ticket_value));

		_celestial_reward_count = 0;
		_celestial_reward_count_2 = 0;

		memset(&_celestial_reward_pos,0,sizeof(_celestial_reward_pos));	
		memset(&_celestial_reward_pos_2,0,sizeof(_celestial_reward_pos_2));

		memset(&_storage,0,sizeof(_storage));	
		memset(&_count_lottery,0,sizeof(_count_lottery));	

		_new_max_level = 0;
	}

	inline bool GetHasCelestialMemorialUnlock() { return _has_celestial_memorial_unlock; }
	inline void SetHasCelestialMemorialUnlock(bool b) { _has_celestial_memorial_unlock = b; }

	inline CELESTIAL_MEMORIAL_STRUCT & GetCelestialMemorialStruct() { return _celestial_memorial_struct; }

	inline void SetLevelandExpMemorial(int level, int exp)
	{
		_celestial_memorial_struct._memorial_level = level;
		_celestial_memorial_struct._memorial_exp = exp;
	}

	inline void AddCountStorageReward(int type, int pos)
	{
		_storage[type]._reward[pos]++;
	}

	inline void InitCountStorageReward()
	{
		for(unsigned int i = 0; i < CelestialMemorialConfig::COUNT_LOTTERY_RED_BOX; i++)
		{
			for (unsigned int j = 0; j < CelestialMemorialConfig::MAX_STORAGE_LOTTERY; j++)
			{
				_storage[i]._reward[j] = 0;
			}
		}		
	}

	int CountStorageReward(int type, int pos) { return _storage[type]._reward[pos]; }
	inline void SetCountStorageReward(int type, int pos)
	{
		_storage[type]._reward[pos] = 1;
	}

	inline void SetTypeCelestialMemorial(bool type)
	{
		_celestial_memorial_struct._type = type;
	}

	inline int GetCelestialMemorialPerg(int pos) { return _celestial_perg_value[pos]; }
	inline void SetCelestialMemorialPerg(int pos, int value)
	{
		_celestial_perg_value[pos] += value;
	}

	inline void SetCelestialMemorialPerg2(int pos, int value_neg)
	{
		_celestial_perg_value[pos] -= value_neg;
	}

	inline int GetCelestialMemorialTicket(int pos) { return _celestial_ticket_value[pos]; }
	inline void SetCelestialMemorialTicket(int pos, int value)
	{
		_celestial_ticket_value[pos] += value;
	}

	inline int GetCountLottery(int pos) { return _count_lottery[pos]; }
	inline void SetCountLottery(int pos)
	{
		_count_lottery[pos]++;
	}
	inline void WipeCountLottery(int pos) 
	{
		_count_lottery[pos] = 0;
	}

	CELESTIAL_M_LOTTERY & GetCelestialMLottery(int pos) { return _celestial_m_lottery[pos]; }
	inline void SetCelestialMLottery(int type_idx, int item_id_1, int item_count_1, int award_type_1 )
	{
		_celestial_m_lottery[type_idx]._item_id_1 = item_id_1;
		_celestial_m_lottery[type_idx]._item_count_1 = item_count_1;
		_celestial_m_lottery[type_idx]._award_type_1 = award_type_1;	
	}
	inline void SetCelestialMLottery2(int type_idx, int item_id_2, int item_count_2, int award_type_2 )
	{
		_celestial_m_lottery[type_idx]._item_id_2 = item_id_2;
		_celestial_m_lottery[type_idx]._item_count_2 = item_count_2;
		_celestial_m_lottery[type_idx]._award_type_2 = award_type_2;
	}
	

	inline bool SetCelestialMemorialRewardPos(int type, short item_id, short pos)
	{
		if(type == 0)
		{
			for(unsigned int i = 0; i < CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1; i++)
			{
				if(_celestial_reward_pos[i].item_id  == item_id && _celestial_reward_pos[i].item_pos == pos)
				return false;	
			}

			for(unsigned int j = 0; j < CelestialMemorialConfig::MAX_LEVEL_RED_BOX_1; j++)
			{
				if(_celestial_reward_pos[j].item_id == 0 && _celestial_reward_pos[j].item_pos == 0)
				{
					_celestial_reward_pos[j].item_id = item_id;	
					_celestial_reward_pos[j].item_pos = pos;	
					break;
				}
			}

			_celestial_reward_count++;

		}
		else if(type == 1)
		{
		
			for(unsigned int i = 0; i < CelestialMemorialConfig::MAX_LEVEL_RED_BOX; i++)
			{
				if(_celestial_reward_pos_2[i].item_id == item_id && _celestial_reward_pos_2[i].item_pos == pos)
				return false;	
			}

			for(unsigned int j = 0; j < CelestialMemorialConfig::MAX_LEVEL_RED_BOX; j++)
			{
				if(_celestial_reward_pos_2[j].item_id == 0 && _celestial_reward_pos_2[j].item_pos == 0)
				{
					_celestial_reward_pos_2[j].item_id = item_id;	
					_celestial_reward_pos_2[j].item_pos = pos;	
					break;
				}
			}

			_celestial_reward_count_2++;

		}

		return true;	
	}

	inline int GetCelestialMemorialRewardCount(unsigned int type)
	{
		if(type == 0)
		{
			return _celestial_reward_count;
		} else 
		{
			return _celestial_reward_count_2;
		}
	}

	inline REWARD & GetCelestialMemorialRewardPos(int pos) { return _celestial_reward_pos[pos];}	
	inline REWARD & GetCelestialMemorialRewardPos_2(int pos) { return _celestial_reward_pos_2[pos];}

	inline void ClearPos500MemorialRewardPos_1()
	{
		for(unsigned int i = 0; i < CelestialMemorialConfig::MAX_LEVEL_RED_BOX; i++)
		{
			if(_celestial_reward_pos[i].item_pos == 500)
			{
				_celestial_reward_pos[i].item_id = 0;
				_celestial_reward_pos[i].item_pos = 0;

				_celestial_reward_count--;
			}

			if(_celestial_reward_pos_2[i].item_pos == 500)
			{
				_celestial_reward_pos_2[i].item_id = 0;
				_celestial_reward_pos_2[i].item_pos = 0;

				_celestial_reward_count_2--;
			}

		}
	}	

	inline void SetCelestialNewMaxLevel(int level)
	{
		_new_max_level = level;
	}

	inline int GetCelestialNewMaxLevel()
	{
		return _new_max_level;
	}

	inline void SetCelestialUsePergProb(int pos, int value)
	{
		_use_perg_prob[pos] = value;
	}

	inline int GetCelestialUsePergProb(int pos)
	{
		return _use_perg_prob[pos];
	}

	inline void ClearAllCelestialMemorial ()
	{
		_has_celestial_memorial_unlock = 0;
		memset(&_celestial_memorial_struct, 0, sizeof(_celestial_memorial_struct));
		memset(&_celestial_m_lottery, 0, sizeof(_celestial_m_lottery));
		memset(&_celestial_perg_value, 0, sizeof(_celestial_perg_value));
		memset(&_celestial_ticket_value, 0, sizeof(_celestial_ticket_value));

		_celestial_reward_count = 0;
		_celestial_reward_count_2 = 0;

		memset(&_celestial_reward_pos, 0, sizeof(_celestial_reward_pos));
		memset(&_celestial_reward_pos_2, 0, sizeof(_celestial_reward_pos_2));

		memset(&_storage, 0, sizeof(_storage));
		memset(&_count_lottery, 0, sizeof(_count_lottery));

		_new_max_level = 0;
	}


public:
};











#endif


