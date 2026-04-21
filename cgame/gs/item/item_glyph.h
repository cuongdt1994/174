#ifndef __ONLINEGAME_GS_GLYPH_ITEM_H__
#define __ONLINEGAME_GS_GLYPH_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include "../filter.h"
#include "equip_item.h"
#include <crc.h>

class glyph_item : public item_body
{
protected:
	const ADDON_LIST * _extra_addon;
	virtual unsigned short GetDataCRC() { return 0xFF;}
	virtual void GetItemData(const void ** data, unsigned int &len)
	{
		*data = "";
		len = 0;
	}
	virtual bool Load(archive & ar);
public:
	glyph_item():_extra_addon(NULL)
	{}

	virtual bool ArmorDecDurability(int) { return false;}
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj) 
	{
		//do nothing  因为激活由外面的RefreshEquip控制
	}
	
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
	{ 
		if(l == item::GLYPH_INV) 
		{
			Deactivate(l,pos,count,obj);
		}
	}
	
	virtual ITEM_TYPE GetItemType() {return ITEM_TYPE_RUNE_ITEM_ESSENCE; }
	virtual void GetDurability(int &dura,int &max_dura) { dura = 100; max_dura = 100; }
	virtual item_body* Clone() const { return new glyph_item(*this); }	
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj)
	{
		return true;
	}
	
	virtual void OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual void OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj);
	DECLARE_SUBSTANCE(glyph_item);
};

#endif