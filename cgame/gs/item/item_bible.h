#ifndef __ONLINEGAME_GS_BIBLE_H__
#define __ONLINEGAME_GS_BIBLE_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include "../filter.h"
#include "equip_item.h"
#include <crc.h>
#include "../player_imp.h"

struct  bible_essence
{
	int unknown;
	int unknown2;
	int unknown3;
	int durability_min;
	int durability_max;
	int unknown4;
	int unknown5;
	int refine_valid;
	int refine_list_addons;
	int refine_addon;
	int refine_level;
};

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & wrapper, const bible_essence & ess)
{
	return wrapper.push_back(&ess,sizeof(ess));
}

template <typename WRAPPER> WRAPPER & operator >>(WRAPPER & wrapper, bible_essence & ess)
{
	return wrapper.pop_back(&ess,sizeof(ess));
}


class bible_item : public item_body
{
protected:
	ADDON_LIST _active_addon;
	ADDON_LIST _total_addon;
	ADDON_LIST _refine_addon;
	ADDON_LIST _refine_addon_total;	
	const ADDON_LIST * _extra_addon;
public:
	bible_essence _ess;
	DECLARE_SUBSTANCE(bible_item);

	bible_item():_extra_addon(NULL)
	{
		memset(&_ess, 0, sizeof(_ess));
	}

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
	
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);

	virtual bool ArmorDecDurability(int) { return false;}
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj) 
	{
		//do nothing  ��Ϊ�����������RefreshEquip����
	}
	
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
	{ 
		if(l == item::BODY) 
		{
			Deactivate(l,pos,count,obj);
		}
	}
	virtual ITEM_TYPE GetItemType() {return ITEM_TYPE_BIBLE; }
	virtual void GetDurability(int &dura,int &max_dura) { dura = _ess.durability_min; max_dura = _ess.durability_max; }

	virtual item_body* Clone() const { return  new bible_item(*this); }
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj)
	{
		return true;
	}

	virtual void OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual void OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj);
	virtual void UpdateEssence();

	virtual int RefineBibleAddon(gplayer_imp* pImp, bool bonus_item, int item_bonus_id);
	virtual bool TransmitRefine(int level);

	inline short GetRefineLevel() { return _ess.refine_level; }
	
	inline void RefreshEquip()
	{
		raw_wrapper rw;
		Save(rw);

		CalcCRC();

		_active_addon.clear();
		_refine_addon.clear();
		UpdateAddOn();
		RefineUpdateAddOn();
	}

	inline void LoadAddOn()
	{
		DATA_TYPE dt3;
		const BIBLE_ESSENCE *it3 = (const BIBLE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE,dt3);

		int refine_level = _ess.refine_level;

		ADDON_LIST::iterator it2 = _refine_addon_total.begin();
		for(; it2 != _refine_addon_total.end(); )
		_refine_addon_total.erase(it2);

		ADDON_LIST::iterator it = _total_addon.begin();
		for(; it != _total_addon.end(); )
		_total_addon.erase(it);
		if(refine_level > 0)
		{

			int refine_addon = it3->levelup_addon;
			addon_data  refine_data;
			if(!world_manager::GetDataMan().generate_addon(refine_addon,refine_data)) return; 
			refine_data.arg[0] = (int)(refine_data.arg[0] * refine_factor[refine_level] + 0.1f);
			refine_data.arg[1] = refine_level;
			_refine_addon_total.push_back(refine_data);

			for(int i = 0; i < 12; i++)
			{
				int addon_id = it3->levelup_addons[i].id_addon;
				int addon_level = it3->levelup_addons[i].refine_level;

				if(addon_id > 0 && addon_level <= refine_level)
				{
					addon_data  newdata;
					if(!world_manager::GetDataMan().generate_addon(addon_id,newdata)) return; 
					_total_addon.push_back(newdata);
				}			
			}

			_active_addon.clear();
			_refine_addon.clear();
			UpdateAddOn();
			RefineUpdateAddOn();
		}

	}

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
	}

	void UpdateAddOn()
	{
		unsigned int i;
		for(i = 0; i < _total_addon.size(); i ++)
		{
			UpdateAddOn(_total_addon[i]);
		}
	}

	void RefineUpdateAddOn(const addon_data & data)
	{
		int mask = addon_manager::CheckAndUpdate(data,NULL);
		if(mask == addon_manager::ADDON_MASK_INVALID)
		{
			throw -120;
		}

		if(mask & addon_manager::ADDON_MASK_ACTIVATE)
		{
			_refine_addon.push_back(data);
		}
	}

	void RefineUpdateAddOn()
	{
		unsigned int i;
		for(i = 0; i < _refine_addon_total.size(); i ++)
		{
			UpdateAddOn(_refine_addon_total[i]);
		}
	}

};

#endif

