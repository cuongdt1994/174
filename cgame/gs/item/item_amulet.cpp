
#include "../clstab.h"
#include "../world.h" 
#include "../player_imp.h"
#include "item_amulet.h"

DEFINE_SUBSTANCE(hp_amulet_item,item_body,CLS_ITEM_HP_AMULET)
DEFINE_SUBSTANCE(mp_amulet_item,item_body,CLS_ITEM_MP_AMULET)

int 
base_amulet::OnAutoTrigger(gactive_imp * obj, int cooldown_idx,int offset)
{
	if(_ess.point <= 0) return 0;
	if(offset <=0) return -1;
	if(offset > _ess.point) offset = _ess.point;
	OnTrigger(obj, offset);
	int cooltime = world_manager::GetDataMan().get_cool_time(_tid);
	cooltime += obj->_heal_cool_time_adj;
	if(cooltime >= 0) obj->SetCoolDown(cooldown_idx, cooltime);
	
	LuaManager * lua = LuaManager::GetInstance();
	
	if( lua->IsGoodItem(_tid) ||
		lua->IsTrue( world_manager::GetWorldTag() , lua->GetConfig()->GOOD_AMULETS_TAG ) )
	//if(IsGoodAmuletTags(world_manager::GetWorldTag()) || IsGoodItems(_tid))
	return  _ess.point;
	return (_ess.point -= offset);
}

void hp_amulet_item::OnActivate(item::LOCATION l,unsigned int pos, unsigned int count, gactive_imp* obj)
{       
	((gplayer_imp*)obj)->SetHPAutoGen(_ess.point, _ess.trigger_percent);
}               

void hp_amulet_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	((gplayer_imp*)obj)->SetHPAutoGen(0,10.f);
} 

void hp_amulet_item::OnTrigger(gactive_imp * obj , int value)
{
	obj->_filters.EF_AdjustHeal(value,2);
	if(value > 0) obj->Heal(value);
}

void mp_amulet_item::OnActivate(item::LOCATION l,unsigned int pos, unsigned int count, gactive_imp* obj)
{      
	((gplayer_imp*)obj)->SetMPAutoGen(_ess.point, _ess.trigger_percent);
}              

void mp_amulet_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	((gplayer_imp*)obj)->SetMPAutoGen(0,10.f);
}       

void mp_amulet_item::OnTrigger(gactive_imp * obj , int value)
{
	obj->InjectMana(value);
}

