#include "../clstab.h"
#include "../world.h"
#include "../worldmanager.h"
#include "../actobject.h"
#include "../item_list.h"
#include "item_glyph.h"

DEFINE_SUBSTANCE(glyph_item,item_body,CLS_ITEM_RUNE_ITEM_ESSENCE)

/*
struct  addon_data
{
	int id;
	int arg[3];
};
*/

bool 
glyph_item::Load(archive & ar)
{
	_extra_addon = set_addon_manager::GetAddonList(_tid);
	return true;
}

void 
glyph_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	if(_extra_addon)
	{       
		for(unsigned int i = 0;i < _extra_addon->size(); i ++)
		{
			//printf("glyph_item::OnActivate: id = %d , arg1 = %d , arg2 = %d , arg3 = %d , \n", (*_extra_addon)[i].id , (*_extra_addon)[i].arg[0] , (*_extra_addon)[i].arg[1] , (*_extra_addon)[i].arg[2] );
			addon_manager::Activate((*_extra_addon)[i],NULL,obj);
		}
	} 
}

void 
glyph_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	if(_extra_addon)
	{
		for(unsigned int i = 0;i < _extra_addon->size(); i ++)
		{
			addon_manager::Deactivate((*_extra_addon)[i],NULL,obj);
		}
	}
}