#ifndef __ONLINEGAME_GS_KID_DRIBIS_H__
#define __ONLINEGAME_GS_KID_DRIBIS_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class kid_dribis_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return false;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return true;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(kid_dribis_item);

	virtual item_body* Clone() const { return new kid_dribis_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_KID_DEBRIS;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
