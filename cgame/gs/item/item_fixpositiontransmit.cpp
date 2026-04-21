#include "../clstab.h"
#include "../world.h"
#include "../actobject.h"
#include "item_fixpositiontransmit.h"


DEFINE_SUBSTANCE(item_fixpositiontransmit,item_body,CLS_ITEM_FIX_POSITION_TRANSMIT)
		
int 
item_fixpositiontransmit::OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count)
{
	return obj->AddFixPositionEnergy(_tid);
}
