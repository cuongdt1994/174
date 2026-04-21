#include "../world.h"
#include "../clstab.h"
#include "../actobject.h"
#include "../clstab.h"
#include "fashion_new_item.h"
#include "../worldmanager.h"
#include "item_addon.h"
#include "set_addon.h"
#include "item.h"
#include <glog.h>

DEFINE_SUBSTANCE(fashion_new_item,item_body, CLS_ITEM_FASHION_NEW_ITEM)

void fashion_new_item::OnRefreshItem()
{
	_raw_data.clear();
	_active_addon.clear();

	UpdateAddOn();

	raw_wrapper rw;
	Save(rw);
	rw.swap(_raw_data);
	CalcCRC();
}

bool fashion_new_item::Save(archive &ar)
{
	try
	{
		ar << _ess;		
		
		if(_ess.effect_indice == 5)
		{
			SaveAddOn(ar);
		}
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool fashion_new_item::Load(archive &ar)
{
	ASSERT(_tid > 0);
	ASSERT(ar.offset() == 0);

	_raw_data.clear();
	_raw_data.push_back(ar.data(), ar.size());

	try
	{
		ar >> _ess;

		if(_ess.effect_indice == 5)
		{
			LoadAddOn(ar);
		}
	}
	catch (...)
	{
		return false;
	}

	UpdateEssence();
	CalcCRC();
	return true;
}

void 
fashion_new_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Activate(_total_addon[i],NULL,obj);
	}
}

void 
fashion_new_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Deactivate(_total_addon[i],NULL,obj);
	}
}

bool 
fashion_new_item::VerifyRequirement(item_list & list,gactive_imp* obj)
{ 
	int class_limit = world_manager::GetDataMan().get_item_class_limit(_tid);

	if(class_limit && !((1 << (obj->GetObjectClass() & 0x0F)) & class_limit))
	{
		return false;
	}

	bool isFemale = ((gactive_object*)obj->_parent)->IsFemale();
	return _ess.require_level <= obj->GetHistoricalMaxLevel() && ( _ess.gender >= 2 || (!_ess.gender &&  !isFemale) || (_ess.gender && isFemale) );
}

void 
fashion_new_item::InitFromShop() 
{
    DATA_TYPE dt;
    FASHION_NEW_ESSENCE* pTemplateEss = (FASHION_NEW_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
    if(dt != DT_FASHION_NEW_ESSENCE || pTemplateEss == NULL) return;

    unsigned short color = 0;
    if(pTemplateEss->combined_switch & FCS_RANDOM_COLOR_IN_RANGE)
    {
        float h,s,v;
        h = abase::Rand(pTemplateEss->h_min, pTemplateEss->h_max);
		s = abase::Rand((float)pow(pTemplateEss->s_min,2), (float)pow(pTemplateEss->s_max,2));
		v = abase::Rand((float)pow(pTemplateEss->v_min,3), (float)pow(pTemplateEss->v_max,3));
		
		s = sqrtf(s);
		v = pow(v,1.0/3); 
		
		if(s > pTemplateEss->s_max) s = pTemplateEss->s_max; else if(s < pTemplateEss->s_min) s = pTemplateEss->s_min;
		if(v > pTemplateEss->v_max) v = pTemplateEss->v_max; else if(v < pTemplateEss->v_min) v = pTemplateEss->v_min;
		
        int color_tmp = hsv2rgb(h,s,v);

        unsigned short r = ((color_tmp >> 16) >> 3) & 0x1F;
        unsigned short g = ((color_tmp >> 8) >> 3) & 0x1F;
        unsigned short b = (color_tmp >> 3) & 0x1F;
        color = ((r << 10) | (g << 5) | b) & 0x7FFF;
    }
    else
    {
        unsigned short r = abase::Rand(0,(1<<5) -1);
		unsigned short g = abase::Rand(0,(1<<5) -1);
		unsigned short b = abase::Rand(0,(1<<5) -1);
        color = ((r << 10) | (g << 5) | b) & 0x7FFF;
    }

    _color_mask = (color << 16);
}

void 
fashion_new_item::UpdateEssence() 
{
	DATA_TYPE dt;
	const FASHION_NEW_ESSENCE & ess = *(const FASHION_NEW_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	ASSERT(&ess || dt == DT_FASHION_NEW_ESSENCE);

	_ess.require_level = ess.require_level;
	_ess.cls = ess.character_combo_id;
	_ess.unk1 = 0;
	_ess.unk2 = 0;
	_ess.durability = 100;
	_ess.max_durability = 100;
	_ess.unk3 = 0;
	_ess.unk4 = 0;
	_ess.unk5 = 0;
	_ess.color = 0;
	_ess.gender = ess.gender;
	_ess.unk6 = 0;
	_ess.unk7 = 0;
}

int 
fashion_new_item::RemoveExpireAddon(int cur_t)
{
	bool changed = false;
	ADDON_LIST::iterator it = _total_addon.begin();
	for(; it != _total_addon.end(); )
	{
		int tmp = addon_manager::GetExpireDate(*it);
		if(tmp > 0 && cur_t >= tmp)
		{
			changed = true;
			it = _total_addon.erase(it);	
		}
		else
		{
			++it;
		}
	}
	if(changed)
	{
		_modify_mask &= ~0xFF00;
		_ess.color = 0;
		_ess.unk3 = 0;
		_ess.unk4 = 0;
		_ess.unk5 = 0;
		_ess.unk6 = 0;
		_ess.unk7 = 0;
		_ess.effect_indice = 0;
		OnRefreshItem();
	}
	return _addon_expire_date;
}

bool 
fashion_new_item::Sharpen(addon_data * addon_list, unsigned int count, int sharpener_gfx)
{
	ADDON_LIST::iterator it = _total_addon.begin();
	for(; it != _total_addon.end(); )
	{
		int tmp = addon_manager::GetExpireDate(*it);
		if(tmp > 0)
			it = _total_addon.erase(it);		
		else
			++it;
	}

	for(unsigned int i=0; i<count; i++)
	{
		_total_addon.push_back(addon_list[i]);	
	}

	_modify_mask &= ~0xFF00;
	_modify_mask |= ((sharpener_gfx & 0xFF)<<8);

	_ess.color = 19406;
	_ess.unk3 = 8;
	_ess.unk4 = 1;
	_ess.unk5 = 1;
	_ess.unk6 = 0;
	_ess.unk7 = 1;
	_ess.effect_indice = 5;
	
	OnRefreshItem();
	return true;
}