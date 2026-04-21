#ifndef __ONLINEGAME_GS_RED_BOOK_UPGRADE_ITEM_H__
#define __ONLINEGAME_GS_RED_BOOK_UPGRADE_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class red_book_upgrade_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return false;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(red_book_upgrade_item);

	virtual item_body* Clone() const { return new red_book_upgrade_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_RED_BOOK_UPDATE_ITEM;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
