#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_camp_token_rollback.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(camp_token_rollback_item,item_body,CLS_ITEM_CAMP_TOKEN_ROLLBACK)

int
camp_token_rollback_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const CAMP_TOKEN_ROLLBACK_ESSENCE & ess = *(const CAMP_TOKEN_ROLLBACK_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_CAMP_TOKEN_ROLLBACK_ESSENCE);


	return 1;
}