#ifndef __ONLINEGAME_GS_ITEM_GENERALCARD_H__
#define __ONLINEGAME_GS_ITEM_GENERALCARD_H__

#include "../config.h"
#include "../item.h" 

enum { GENERALCARD_RANK_S = 3, };

struct generalcard_essence
{
	int type;					//����,��װ��λ�����Ӧ,�ƾ�,����,����,���,����,����
	int rank;					//Ʒ��,C,B,A,S,S+
	int require_level;			//����ȼ�
	int require_leadership;		//����ͳ����
	int max_level;				//���ɳ��ȼ�
	int level;					//�ɳ��ȼ�
	int exp;					//�ɳ�����
	int rebirth_times;			//ת������
};

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const generalcard_essence & es)
{
	wrapper.push_back(&es, sizeof(es));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, generalcard_essence & es)
{
	wrapper.pop_back(&es, sizeof(es));
	return wrapper;
}

struct generalcard_extend
{
	int max_hp;
	int damage_low;
	int damage_high;
	int damage_magic_low;
	int damage_magic_high;
	int defense;  
	int resistance[MAGIC_CLASS];
	int vigour;
};

class generalcard_item : public item_body
{
	generalcard_essence _ess;				//��Ҫ���̵ı�������
	generalcard_extend _extend;
	const ADDON_LIST * _extra_addon;
	
public:
	DECLARE_SUBSTANCE(generalcard_item);
	generalcard_item():_extra_addon(NULL),_crc(0)
	{
		memset(&_ess, 0, sizeof(_ess));
		memset(&_extend, 0, sizeof(_extend));
	}

	virtual bool Load(archive & ar);
	virtual void GetItemData(const void ** data, unsigned int &len)
	{
		*data = &_ess;
		len = sizeof(_ess);
 	}
	
	unsigned short _crc;
	virtual unsigned short GetDataCRC() { return _crc; }
	void CalcCRC()
	{   
		_crc = crc16( (unsigned char *)&_ess, sizeof(_ess));
	}

	void ClearData();
	void UpdateEssence();
	virtual void OnRefreshItem();

private:
	//item_body�д��麯��
	ITEM_TYPE GetItemType()  { return ITEM_TYPE_GENERALCARD;}
	bool ArmorDecDurability(int) { return false;}
	item_body * Clone() const { return new generalcard_item(*this);}

private:
	//װ���������
	virtual bool VerifyRequirement(item_list & list, gactive_imp* imp);
	virtual void OnActivate(item::LOCATION, unsigned int pos, unsigned int count, gactive_imp* imp);
	virtual void OnDeactivate(item::LOCATION, unsigned int pos, unsigned int count,gactive_imp* imp);
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj);
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);

	virtual int GetRank(){ return _ess.rank; }
	virtual int GetRebirthTimes(){ return _ess.rebirth_times; }
	virtual int GetCardLevel(){ return _ess.level; }
	virtual bool CheckRebirthCondition(int material_rebirth_times);
	virtual bool DoRebirth(int arg);
	virtual bool InsertExp(int& exp, int na1, int na2,  bool ischeck);
	virtual int GetSwallowExp();
	virtual bool IsGeneralCardMatchPos(unsigned int pos)
	{
		return GetGeneralCardColumnIndex(pos) == _ess.type;
	}

	inline int GetGeneralCardColumnIndex(unsigned int pos)
	{
		return (int)pos - item::EQUIP_INDEX_GENERALCARD1;
	}
};

#endif
