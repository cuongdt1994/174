#include "../clstab.h"
#include "../world.h"
#include "../worldmanager.h"
#include "../player_imp.h"
#include "item_easy_produce.h"

DEFINE_SUBSTANCE(easy_produce_item,item_body,CLS_ITEM_EASYPRODUCE)

int 
easy_produce_item::OnUse(item::LOCATION l, gactive_imp * imp, unsigned int count)
{
	DATA_TYPE dt;
	EASY_PRODUCE_ITEM_ESSENCE *ess = (EASY_PRODUCE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	if(ess && dt == DT_EASY_PRODUCE_ITEM_ESSENCE)
	{
		gplayer_imp * pImp = (gplayer_imp *)imp;
		if(pImp->ProduceEasyItem(_tid, ess->produce_times))
		{			
			return 1;
		}
	}
	return -1;
}