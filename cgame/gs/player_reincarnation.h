#ifndef __ONLINEGAME_PLAYER_REINCARNATION_H__
#define __ONLINEGAME_PLAYER_REINCARNATION_H__


#include <vector.h>
#include <common/base_wrapper.h>
#include <db_if.h>

enum
{
	REINCARNATION_TIMES_LIMIT 				= 2,	//ת����������,���ܳ���Ӳ����
	REINCARNATION_TIMES_HARD_LIMIT			= 10,	//ת������Ӳ����,��elementdata���ݶ�Ӧ,���������޸�
	REINCARNATION_LEVEL_REQUIRED 			= 100,	//ת���ȼ�����
	REINCARNATION_SEC_LEVEL_REQUIRED 		= 20,	//ת������ȼ�����
	REINCARNATION_TOME_EXP_MAX				= 1800000000,	//ת�����ᾭ������
	REINCARNATION_CONFIG_ID					= 1424,
};

class gplayer_imp;
class player_reincarnation
{
public:
	struct Record
	{
		int level;
		int timestamp;
		int exp;
	};

public:
	player_reincarnation(gplayer_imp * imp):_tome_exp(0),_tome_active(false),
											_imp(imp),_historical_max_level(0),_exp_bonus(0.f)
	{
	}
	
	bool Save(archive & ar)
	{
		unsigned int size = _records.size();
		ar << size;
		if(_records.size()) ar.push_back(_records.begin(), _records.size()*sizeof(Record));
		ar << _tome_exp << _tome_active << _historical_max_level << _exp_bonus;
		return true;
	}
	
	bool Load(archive & ar)
	{
		unsigned int size;
		ar >> size;
		_records.reserve(size);
		for(unsigned int i=0; i<size; i++)
		{
			Record r;
			ar >> r.level >> r.timestamp >> r.exp;
			_records.push_back(r);
		}
		ar >> _tome_exp >> _tome_active >> _historical_max_level >> _exp_bonus;
		return true;
	}
	
	void Swap(player_reincarnation & rhs)
	{
		_records.swap(rhs._records);
		abase::swap(_tome_exp, rhs._tome_exp);
		abase::swap(_tome_active, rhs._tome_active);
		abase::swap(_historical_max_level, rhs._historical_max_level);
		abase::swap(_exp_bonus, rhs._exp_bonus);
	}

	bool InitFromDBData(const GDB::reincarnation_data & data);
	bool MakeDBData(GDB::reincarnation_data & data);
	static void ReleaseDBData(GDB::reincarnation_data & data);

public:
	bool CheckCondition();
	void DoReincarnation();
	bool RewriteTome(unsigned int record_index, int record_level);
	void ClientGetTome();
	bool TryActivateTome();
	void DeactivateTome();
	bool CheckActivateTome();
	void IncTomeExp(int exp);
	inline int GetTomeExp() { return _tome_exp; }
	inline void SetTomeExp(int val) { _tome_exp = val; }

	void CalcHistoricalMaxLevel();
	void CalcExpBonus();
	inline unsigned int GetTimes(){ return _records.size(); }
	inline bool IsTomeActive(){ return _tome_active; }
	inline int GetHistoricalMaxLevel(){ return _historical_max_level; }
	inline float GetExpBonus(){ return _exp_bonus; }
	
	static int MaxExtraPPoint();
private:
	//��Ҫ����
	abase::vector<Record> 	_records;
	int 					_tome_exp;
	bool			 		_tome_active;
	//�������
	gplayer_imp *			_imp;
	int						_historical_max_level;
	float					_exp_bonus;
};


#endif
