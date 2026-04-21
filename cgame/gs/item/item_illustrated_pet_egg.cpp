#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_illustrated_pet_egg.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(illustrated_pet_egg_item,item_body,CLS_ITEM_ILLUSTRATED_PET_EGG)

int
illustrated_pet_egg_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const ILLUSTRATED_PET_EGG_ESSENCE & ess = *(const ILLUSTRATED_PET_EGG_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_ILLUSTRATED_PET_EGG_ESSENCE);


	return 1;
}