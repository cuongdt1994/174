#ifndef __ONLINEGAME_GS_BIBLE_REFINE_TICKET_H__
#define __ONLINEGAME_GS_BIBLE_REFINE_TICKET_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class bible_refine_ticket_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return true;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(bible_refine_ticket_item);

	virtual item_body* Clone() const { return new bible_refine_ticket_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_BIBLE_TICKET;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
