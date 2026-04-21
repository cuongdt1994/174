#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_capture.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(capture_item,item_body,CLS_ITEM_CAPTURE)

int
capture_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const CAPTURE_ITEM_ESSENCE & ess = *(const CAPTURE_ITEM_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_CAPTURE_ITEM_ESSENCE);


	return 1;
}