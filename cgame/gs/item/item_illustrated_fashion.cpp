#include "../clstab.h"
#include "../actobject.h"
#include "../world.h"
#include "../item_list.h"
#include "item_illustrated_fashion.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include "../npcgenerator.h"

DEFINE_SUBSTANCE(illustrated_fashion_item,item_body,CLS_ITEM_ILLUSTRATED_FASHION)

int
illustrated_fashion_item::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	return -1;
}