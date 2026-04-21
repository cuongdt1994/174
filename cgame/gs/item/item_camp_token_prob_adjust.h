#ifndef __ONLINEGAME_GS_CAMP_TOKEN_PROB_ADJUST_H__
#define __ONLINEGAME_GS_CAMP_TOKEN_PROB_ADJUST_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "../filter.h"
#include <crc.h>

class camp_token_prob_adjust_item : public item_body
{
protected:
	virtual bool IsItemCanUse(item::LOCATION l) { return false;}
	virtual bool IsItemSitDownCanUse(item::LOCATION l) { return false;}
	virtual bool ArmorDecDurability(int) { return false;}
public:
	DECLARE_SUBSTANCE(camp_token_prob_adjust_item);

	virtual item_body* Clone() const { return new camp_token_prob_adjust_item(*this); }

	virtual ITEM_TYPE GetItemType()
	{
		return ITEM_TYPE_CAMP_TOKEN_PROB_ADJUST;
	}
private:
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
};

#endif
