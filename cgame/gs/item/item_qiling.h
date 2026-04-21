#ifndef __ONLINEGAME_GS_QILING_H__
#define __ONLINEGAME_GS_QILING_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"

#include <crc.h>
#include <glog.h>

#pragma pack(push, 1)
struct qiling_essence
{
	int crystal_id;
	char crystal_level;
	char crystal_color;

	int exp;
	char lock_unlock;

	char stars[5]; // Estrelas nv 0 até 5

	// Reserve
	char res[15];

	int addons_count;

	struct addon
	{
		int id;
		int arg;
	} addons[2];

	unsigned int reserve;
};
#pragma pack(pop)

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const qiling_essence & crystal)
{
	return wrapper.push_back(&crystal,sizeof(crystal));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, qiling_essence & crystal)
{
	return wrapper.pop_back(&crystal,sizeof(crystal));
}

class qiling_item : public item_body
{
protected:

	virtual void OnRefreshItem();

public:
	qiling_essence _ess;

	abase::octets _raw_data;
	unsigned short _crc;
	ADDON_LIST _total_addon;

	DECLARE_SUBSTANCE(qiling_item);
	virtual item_body* Clone() const { return new qiling_item(*this); }

	qiling_item()
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

	virtual int GetCrystalId() { return _ess.crystal_id; }
	virtual void SetCrystalId(int value) { _ess.crystal_id = value; OnRefreshItem(); }

	virtual char GetCrystalLevel() { return _ess.crystal_level; }
	virtual void SetCrystalLevel(char value) { _ess.crystal_level = value; OnRefreshItem(); }

	virtual char GetCrystalColor() { return _ess.crystal_color; }
	virtual void SetCrystalColor(char value) { _ess.crystal_color = value; OnRefreshItem(); }

	virtual int GetCrystalExp() { return _ess.exp; }
	virtual void SetCrystalExp(int value) { _ess.exp = value; OnRefreshItem(); }

	virtual char GetCrystalLock() { return _ess.lock_unlock; }
	virtual void SetCrystalLock(char value) { _ess.lock_unlock = value; OnRefreshItem(); }

	virtual char GetCrystalStar(int index) { return _ess.stars[index]; }
	virtual void SetCrystalStar(int index, char value) { _ess.stars[index] = value; OnRefreshItem(); }
	
	virtual int GetAddonsCrystalCount() { return _ess.addons_count; }
	virtual void SetAddonsCrystalCount(int value) { _ess.addons_count = value; OnRefreshItem(); }

	virtual int GetAddonsCrystalid(int index) { return  _ess.addons[index].id; }
	virtual int GetAddonsCrystalarg(int index) { return _ess.addons[index].arg; }

	virtual void SetAddonsCrystal(int index, int id, int arg)
	{
		_ess.addons[index].id = id;
		_ess.addons[index].arg = arg;
		OnRefreshItem();
	}

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_QILING;
	}
};

#endif
