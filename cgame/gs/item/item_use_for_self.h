#ifndef __ONLINEGAME_GS_USE_FOR_SELF_H__
#define __ONLINEGAME_GS_USE_FOR_SELF_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class use_for_self_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return true;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(use_for_self_item);

	virtual item_body* Clone() const { return new use_for_self_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_USE_FOR_SELF;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
