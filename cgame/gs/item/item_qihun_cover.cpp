#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_qihun_cover.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(qihun_cover_item,item_body,CLS_ITEM_QIHUN_COVER)

int
qihun_cover_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{		
	return -1;
}