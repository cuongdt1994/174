#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_used_for_area.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(used_for_area_item,item_body,CLS_ITEM_USED_FOR_AREA)

int
used_for_area_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const ITEM_USED_FOR_AREA_ESSENCE & ess = *(const ITEM_USED_FOR_AREA_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_ITEM_USED_FOR_AREA_ESSENCE);


	return 1;
}