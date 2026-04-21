#ifndef __ONLINEGAME_GS_GLYPH_UNLOCK_ITEM_H__
#define __ONLINEGAME_GS_GLYPH_UNLOCK_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class glyph_unlock_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return true;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(glyph_unlock_item);

	virtual item_body* Clone() const { return new glyph_unlock_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_UNLOCK_RUNE_SLOT_ITEM_ESSENCE;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif