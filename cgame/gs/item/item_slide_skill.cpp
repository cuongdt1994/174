#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_slide_skill.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(slide_skill_item,item_body,CLS_ITEM_SLIDE_SKILL)

int
slide_skill_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const SLIDE_SKILL_ESSENCE & ess = *(const SLIDE_SKILL_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_SLIDE_SKILL_ESSENCE);


	return 1;
}