#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_red_book_upgrade.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(red_book_upgrade_item,item_body,CLS_ITEM_RED_BOOK_UPDATE_ITEM)

int
red_book_upgrade_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const RED_BOOK_UPGRADE_ITEM & ess = *(const RED_BOOK_UPGRADE_ITEM *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_RED_BOOK_UPGRADE_ITEM);


	return 1;
}