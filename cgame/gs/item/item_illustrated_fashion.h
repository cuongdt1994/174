#ifndef __ONLINEGAME_GS_ILLUSTRATED_FASHION_H__
#define __ONLINEGAME_GS_ILLUSTRATED_FASHION_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class illustrated_fashion_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return false;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return false;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(illustrated_fashion_item);

	virtual item_body* Clone() const { return new illustrated_fashion_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_ILLUSTRATED_FASHION;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
