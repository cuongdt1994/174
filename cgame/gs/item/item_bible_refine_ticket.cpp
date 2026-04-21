#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_bible_refine_ticket.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(bible_refine_ticket_item,item_body,CLS_ITEM_BIBLE_REFINE_TICKET)

int
bible_refine_ticket_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const BIBLE_REFINE_TICKET_ESSENCE & ess = *(const BIBLE_REFINE_TICKET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_BIBLE_REFINE_TICKET_ESSENCE);


	return 1;
}