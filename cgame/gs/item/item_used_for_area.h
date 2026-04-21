#ifndef __ONLINEGAME_GS_USED_FOR_AREA_H__
#define __ONLINEGAME_GS_USED_FOR_AREA_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class used_for_area_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return true;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(used_for_area_item);

	virtual item_body* Clone() const { return new used_for_area_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_USED_FOR_AREA;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
