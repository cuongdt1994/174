#include "../clstab.h"
#include "../world.h"
#include "../worldmanager.h"
#include "../actobject.h"
#include "../item_list.h"
#include "item_glyph_unlock.h"

DEFINE_SUBSTANCE(glyph_unlock_item,item_body,CLS_ITEM_UNLOCK_RUNE_SLOT_ITEM_ESSENCE)

int
glyph_unlock_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const UNLOCK_RUNE_SLOT_ITEM_ESSENCE & ess = *(const UNLOCK_RUNE_SLOT_ITEM_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_UNLOCK_RUNE_SLOT_ITEM_ESSENCE);

	if (!obj->UseGlyphUnlockItem(ess.slot , ess.require_level2 , ess.require_level))
	{
		obj->_runner->error_message(S2C::ERR_RUNE_SLOT_NOT_OPEN);
		return -1;
	}

	return 1;
}
