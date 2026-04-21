#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_fast_produce.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(fast_produce_item,item_body,CLS_ITEM_FAST_PRODUCE)

int
fast_produce_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const FAST_PRODUCE_ITEM_ESSENCE & ess = *(const FAST_PRODUCE_ITEM_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_FAST_PRODUCE_ITEM_ESSENCE);


	return 1;
}