#ifndef __ONLINEGAME_GS_ITEM_FASHION_NEW_ITEM_H__
#define __ONLINEGAME_GS_ITEM_FASHION_NEW_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include "equip_item.h"
#include "../filter.h"
#include <arandomgen.h>
#include <crc.h>
#include "../player_imp.h"

#pragma pack(1)
struct fashion_new_item_essence 
{
	short require_level; // 1
	short cls; // 4096
	int unk1; // 0
	int unk2; // 0
	int durability; // 100
	int max_durability; // 100
	short unk3; // 8
	short unk4; // 1
	int unk5; // 1
	unsigned short color; // 19406
	unsigned short gender; // 1
	short unk6;
	char unk7; // 0
	char effect_indice; // 5	
};
#pragma pack()

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & wrapper, const fashion_new_item_essence & ess)
{
	return wrapper.push_back(&ess,sizeof(ess));
}

template <typename WRAPPER> WRAPPER & operator >>(WRAPPER & wrapper, fashion_new_item_essence & ess)
{
	return wrapper.pop_back(&ess,sizeof(ess));
}

class fashion_new_item : public item_body
{
protected:
	ADDON_LIST _active_addon;
	ADDON_LIST _total_addon;	
	int	_addon_expire_date;
	unsigned short _modify_mask;
	int _color_mask;
	abase::octets _raw_data;
	unsigned short _crc;

public:
	fashion_new_item_essence _ess;
	DECLARE_SUBSTANCE(fashion_new_item);

	fashion_new_item()
	{
		memset(&_ess, 0, sizeof(_ess));
		_addon_expire_date = 0;	
		_crc = 0;
	}

public:
	virtual item_body* Clone() const { return  new fashion_new_item(*this); }
	virtual bool ArmorDecDurability(int) { return false;}
	virtual ITEM_TYPE GetItemType() {return ITEM_TYPE_NEW_FASHION; }
	virtual void GetDurability(int &dura,int &max_dura) { dura = 100; max_dura = 100; }

	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);

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

    virtual void InitFromShop(); 
	virtual void UpdateEssence();
	    
    virtual void DyeItem(int value) 
	{
		_ess.color = value & 0x7FFF;
		_color_mask = (_ess.color << 16);
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

	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj);
	virtual short GetIdModify() { return _modify_mask; }

	virtual int GetAddonExpireDate(){ return _addon_expire_date; }
	virtual int RemoveExpireAddon(int cur_t);
	virtual bool Sharpen(addon_data * addon_list, unsigned int count, int sharpener_gfx);

	void UpdateAddOn(const addon_data & data)
	{
		int mask = addon_manager::CheckAndUpdate(data,NULL);
		if(mask == addon_manager::ADDON_MASK_INVALID)
		{
			throw -120;
		}

		if(mask & addon_manager::ADDON_MASK_ACTIVATE)
		{
			_active_addon.push_back(data);
		}
	
		int tmp = addon_manager::GetExpireDate(data);
		if(tmp > 0)
		{
			if(_addon_expire_date == 0)
				_addon_expire_date = tmp;
			else if(_addon_expire_date > tmp)
				_addon_expire_date = tmp;				
		}		
	}

	virtual void OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual void OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj);	

protected:
	inline void OnRefreshItem();

	void UpdateAddOn()
	{
		unsigned int i;
		for(i = 0; i < _total_addon.size(); i ++)
		{
			UpdateAddOn(_total_addon[i]);
		}
	}

	void LoadAddOn(archive &ar)
	{
		unsigned int count;
		int argcount;
		ar >> count;
		if (count < 0 || count > 128)
		{
			throw -100;
		}

		for (unsigned int i = 0; i < count; i++)
		{
			addon_data entry;
			memset(&entry, 0, sizeof(entry));

			ar >> entry.id;
			argcount = addon_manager::GetArgCount(entry.id);
			for (int j = 0; j < argcount; j++)
			{
				ar >> entry.arg[j];
			}
			_total_addon.push_back(entry);
		}
	}

	void SaveAddOn(archive & ar)
	{
		unsigned int count;
		count = _total_addon.size();
		ar << count;
		for (unsigned int i = 0; i < count; i++)
		{
			int id = _total_addon[i].id;
			int argcount = addon_manager::GetArgCount(id);
			ar << id;
			for (int j = 0; j < argcount; j++)
			{
				ar << _total_addon[i].arg[j];
			}
		}
	}
};

#endif

