#ifndef __ONLINEGAME_GS_QIHUN_H__
#define __ONLINEGAME_GS_QIHUN_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include "item_addon.h"

#include <crc.h>
#include <glog.h>

#pragma pack(push, 1)
struct qihun_essence
{
	int item_id;
	char crystal_lvl; //1
	char crystal_enable; //0 ~ 1
	char lock_unlock; //0 ~ 1

	int addons_count;
	struct
	{
		int id;
		int arg;
	} addons[5];
};
#pragma pack(pop)

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & wrapper, const qihun_essence & ess)
{
	return wrapper.push_back(&ess,sizeof(ess));
}

template <typename WRAPPER> WRAPPER & operator >>(WRAPPER & wrapper, qihun_essence & ess)
{
	return wrapper.pop_back(&ess,sizeof(ess));
}

class qihun_item : public item_body
{
protected:

	virtual void OnRefreshItem();

public:
	qihun_essence _ess;

	abase::octets _raw_data;
	unsigned short _crc;
	ADDON_LIST _total_addon;

	DECLARE_SUBSTANCE(qihun_item);
	virtual item_body* Clone() const { return new qihun_item(*this); }

	qihun_item()
	{
		memset(&_ess, 0, sizeof(_ess));
		_crc = 0;
	}
	unsigned short GetDataCRC() { return _crc; }
	void CalcCRC()
	{   
		_crc = crc16( (unsigned char *)_raw_data.begin(), _raw_data.size());
	}
	void GetItemData(const void ** data, unsigned int &len)
	{	
		*data = _raw_data.begin();
		len = _raw_data.size();
 	}

	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);

	virtual bool ArmorDecDurability(int) { return false;}
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj) 
	{
	}
	
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
	{ 
		if(l == item::BODY) 
		{
			Deactivate(l,pos,count,obj);
		}
	}

	inline virtual void CrystalLevelEnable(char value)
	{
		_ess.crystal_lvl = value;
		OnRefreshItem();
	}

	virtual void LockUnlockSpirit(bool value) {  _ess.lock_unlock = value; OnRefreshItem(); }
	virtual char GetLockUnlockSpirit() {  return _ess.lock_unlock; }

	inline virtual void TransmitLuarSpirit(char value)
	{
		_ess.crystal_enable = value;
		OnRefreshItem();
	}

	inline virtual char GetLuarSpiritLevel() { return _ess.crystal_enable; }
	inline virtual void SetLuarSpiritLevel(char value)
	{
		_ess.crystal_enable = value;
		OnRefreshItem();
	}

	virtual int GetAddonsSpiritCount() { return *(int*)&_raw_data[7]; }
	virtual void SetAddonsSpiritCount(int value) { _ess.addons_count = value; OnRefreshItem(); }

	virtual int GetAddonsSpiritid(int index) { return *(int*)&_raw_data[11+sizeof(size_t)*index]; }
	virtual int GetAddonsSpiritarg(int index) { return *(int*)&_raw_data[15+sizeof(size_t)*index]; }

	virtual void SetAddonsSpirit(int index, int id, int arg)
	{
		_ess.addons[index].id = id;
		_ess.addons[index].arg = arg;
		OnRefreshItem();
	}

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_QIHUN;
	}
	
};

#endif
