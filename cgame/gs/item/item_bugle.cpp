
#include "../clstab.h"
#include "../world.h"
#include "../player_imp.h"
#include "item_bugle.h"

DEFINE_SUBSTANCE(bugle_item,item_body,CLS_ITEM_BUGLE)


void bugle_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	((gplayer_imp*)obj)->SetChatEmote(_ess.emote_id);
}

void bugle_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	((gplayer_imp*)obj)->SetChatEmote(0);
}


