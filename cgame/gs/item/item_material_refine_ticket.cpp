#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_material_refine_ticket.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(material_refine_ticket_item,item_body,CLS_ITEM_MATERIAL_REFINE_TICKET)

int
material_refine_ticket_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	DATA_TYPE dt;
	const MATERIAL_REFINE_TICKET_ESSENCE & ess = *(const MATERIAL_REFINE_TICKET_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(&ess && dt == DT_MATERIAL_REFINE_TICKET_ESSENCE);


	return -1;
}