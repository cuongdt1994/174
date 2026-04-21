#ifndef __ONLINEGAME_GS_EASY_PRODUCE_H__
#define __ONLINEGAME_GS_EASY_PRODUCE_H__

#include "../config.h"
#include "../item.h" 

class easy_produce_item : public item_body
{
public:
	DECLARE_SUBSTANCE(easy_produce_item);
public:

public:
	ITEM_TYPE GetItemType()  { return ITEM_TYPE_EASYPRODUCE;}
	bool ArmorDecDurability(int) { return false;}
	item_body * Clone() const { return new easy_produce_item(*this);}
public:
	virtual bool IsItemCanUse(item::LOCATION l) { return true;}
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
	virtual int OnGetUseDuration()
	{
		DATA_TYPE dt;
		EASY_PRODUCE_ITEM_ESSENCE *ess = (EASY_PRODUCE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
		if(ess && dt == DT_EASY_PRODUCE_ITEM_ESSENCE)
		{
			int use_time = (int)(20*ess->duration);
			if(use_time<= 0) use_time = 1;
			if(use_time > 10000) use_time = 10000;

			return use_time;
		}	
		return 10;		
	}
};


#endif
