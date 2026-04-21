#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_fix_monster.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(fix_monster_item,item_body,CLS_ITEM_FIX_MONSTER)

int
fix_monster_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const FIX_MONSTER_ITEM_ESSENCE & ess = *(const FIX_MONSTER_ITEM_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_FIX_MONSTER_ITEM_ESSENCE);


	return 1;
}