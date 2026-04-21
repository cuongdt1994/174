#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_kid_dribis.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(kid_dribis_item,item_body,CLS_ITEM_KID_DEBRIS)

int
kid_dribis_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const KID_DEBRIS_ESSENCE & ess = *(const KID_DEBRIS_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_KID_DEBRIS_ESSENCE);


	return 1;
}