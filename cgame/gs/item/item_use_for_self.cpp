#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_use_for_self.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(use_for_self_item,item_body,CLS_ITEM_USE_FOR_SELF)

int
use_for_self_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const USE_FOR_SELF_ITEM_ESSENCE & ess = *(const USE_FOR_SELF_ITEM_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_USE_FOR_SELF_ITEM_ESSENCE);


	return 1;
}