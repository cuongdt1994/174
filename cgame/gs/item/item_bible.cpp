
#include "../clstab.h"
#include "../world.h"
#include "../actobject.h"
#include "../item_list.h"
#include "../worldmanager.h"
#include "item_bible.h"
#include "item_addon.h"
#include "set_addon.h"
#include "item.h"
#include <glog.h>

DEFINE_SUBSTANCE(bible_item,item_body,CLS_ITEM_BIBLE)

#define SIZE_AR_FIRST 20
#define SIZE_AR_END 28
#define SIZE_AR_NEW 24


bool 
bible_item::Save(archive & ar)
{
	try
	{
		ar << _ess;
	}
	catch(...)
	{
		return false;
	}
	return true;
}


bool 
bible_item::Load(archive & ar)
{	
	_extra_addon = set_addon_manager::GetAddonList(_tid);
	try
	{	
		if(ar.size() > SIZE_AR_FIRST && ar.size() != SIZE_AR_END && ar.size() != SIZE_AR_NEW)
		{
			ar >> _ess;
		}	
		
		UpdateEssence();			
		CalcCRC();
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

void 
bible_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	if(this->_ess.refine_valid == 1)
	{
		LoadAddOn();
	}
	
	if(_extra_addon)
	{       
		for(unsigned int i = 0;i < _extra_addon->size(); i ++)
		{
			addon_manager::Activate((*_extra_addon)[i],NULL,obj);
		}
	}
	for(unsigned int i = 0;i < _active_addon.size(); i ++)
	{
		addon_manager::Activate(_active_addon[i],NULL,obj);
	}

	for(unsigned int i = 0;i < _refine_addon.size(); i ++)
	{
		addon_manager::Activate(_refine_addon[i],NULL,obj);
	}


	obj->ActivePetNoFeed(true);
}

void 
bible_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	if(this->_ess.refine_valid == 1)
	{
		LoadAddOn();
	}
	
	if(_extra_addon)
	{
		for(unsigned int i = 0;i < _extra_addon->size(); i ++)
		{
			addon_manager::Deactivate((*_extra_addon)[i],NULL,obj);
		}
	}	

	for(unsigned int i = 0;i < _active_addon.size(); i ++)
	{
		addon_manager::Deactivate(_active_addon[i],NULL,obj);
	}

	for(unsigned int i = 0;i < _refine_addon.size(); i ++)
	{
		addon_manager::Deactivate(_refine_addon[i],NULL,obj);
	}

	obj->ActivePetNoFeed(false);
}

void
bible_item::UpdateEssence()
{
	_ess.unknown = 0;
	_ess.unknown2 = 0;
	_ess.unknown3 = 0;
	_ess.durability_min = 100;
	_ess.durability_max = 100;
	_ess.unknown4 = 0;
	_ess.unknown5 = 0;

	if(this->_ess.refine_valid == 0)
	{
		_ess.refine_valid = 1;
		_ess.refine_list_addons = 0;
		_ess.refine_addon = 0;
		_ess.refine_level = 0;
	}
}

static float refine_factor_new[] =
{
	0,
	1.0f,
	2.0f,
	3.05f,
	4.3f,
	5.75f,
	7.55f,
	9.95f,
	13.f,
	17.05f,
	22.3f,
	29.f,
	37.5f,
};

int
bible_item::RefineBibleAddon(gplayer_imp* pImp, bool bonus_item, int item_bonus_id)
{	
	DATA_TYPE dt;
	DATA_TYPE dt2;
	DATA_TYPE dt3;
	BIBLE_REFINE_CONFIG *it = (BIBLE_REFINE_CONFIG*)world_manager::GetDataMan().get_data_ptr(5072,ID_SPACE_CONFIG,dt);
	const BIBLE_REFINE_TICKET_ESSENCE *it2 = (const BIBLE_REFINE_TICKET_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_bonus_id, ID_SPACE_ESSENCE,dt2);
	const BIBLE_ESSENCE *it3 = (const BIBLE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE,dt3);
	if(!it || !it3)	return 0; //Em caso de falha na tabela, falha o refino

	float prob = 0.0f;
	float prob_bonus = 0.0f;

	int rst = 2; // Falha (Zera o refino)

	int refine_level = 0;
	refine_level = this->_ess.refine_level;
	
	if(this->_ess.refine_valid != 1)
	{
		refine_level = 0;
	}

	if (it && dt == DT_BIBLE_REFINE_CONFIG)
	{
		prob = it->level[refine_level].success_probability;
	}

	if (it2 && dt2 == DT_BIBLE_REFINE_TICKET_ESSENCE && bonus_item)
	{
		if(it2->ext_reserved_prob > 0)
		{
			rst = 3; // Falha (reduz o refino em 1 nível)
		}		

		if(it2->ext_succeed_prob > 0)
		{
			prob_bonus = it2->ext_succeed_prob;
		} else 
		{
			if(this->_ess.refine_level < 12)
				prob_bonus = it2->fail_ext_succeed_prob[refine_level];
		}		
	}
	
	float prob_final = (prob + prob_bonus)*100.0f;
	float rand = abase::Rand(0,99);

	if(prob_final > rand)
	{		
		refine_level += 1;

		ADDON_LIST::iterator it2 = _refine_addon_total.begin();
		for(; it2 != _refine_addon_total.end(); )
		_refine_addon_total.erase(it2);

		ADDON_LIST::iterator it = _total_addon.begin();
		for(; it != _total_addon.end(); )
		_total_addon.erase(it);

		int refine_addon = it3->levelup_addon;
		addon_data  refine_data;
		if(!world_manager::GetDataMan().generate_addon(refine_addon,refine_data)) return 0; 
		refine_data.arg[0] = (int)(refine_data.arg[0] * refine_factor[refine_level] + 0.1f);
		refine_data.arg[1] = refine_level;
		_refine_addon_total.push_back(refine_data);
				
		
		for(int i = 0; i < 12; i++)
		{
			int addon_id = it3->levelup_addons[i].id_addon;
			int addon_level = it3->levelup_addons[i].refine_level;

			if(addon_id > 0 && addon_level <= refine_level)
			{
				addon_data  newdata;
				if(!world_manager::GetDataMan().generate_addon(addon_id,newdata)) return 0; 
				_total_addon.push_back(newdata);
			}			
		}
	
		this->_ess.refine_valid = 1; // Ativa o refino
		this->_ess.refine_list_addons = (_refine_addon_total)[0].id; // Adiciona a lista de addons
		this->_ess.refine_addon = (int)(_refine_addon_total)[0].arg[0]; // Adiciona o nível do addon
		this->_ess.refine_level = refine_level; // Adiciona o nível do refino

		RefreshEquip();

		rst = 1; // Sucesso
	} else 
	{
		if(refine_level == 0)
		{
			rst = 0; // Falha (mantém o atributo de refino)
		}

		if(rst == 2)
		{
			rst = 2; // Falha (Zera o refino)
			
			ADDON_LIST::iterator it2 = _refine_addon_total.begin();
			for(; it2 != _refine_addon_total.end(); )
			_refine_addon_total.erase(it2);

			ADDON_LIST::iterator it = _total_addon.begin();
			for(; it != _total_addon.end(); )
			_total_addon.erase(it);	
		
			this->_ess.refine_valid = 0;
			this->_ess.refine_list_addons = 0;
			this->_ess.refine_addon = 0; 
			this->_ess.refine_level = 0;

			RefreshEquip();

		} 
		
		if (rst == 3)
		{			
			rst = 3; // Falha (reduz o refino em 1 nível)
			refine_level -= 1;

			if(refine_level > 0)
			{
			
			ADDON_LIST::iterator it2 = _refine_addon_total.begin();
			for(; it2 != _refine_addon_total.end(); )
			_refine_addon_total.erase(it2);
				
			ADDON_LIST::iterator it = _total_addon.begin();
			for(; it != _total_addon.end(); )
			_total_addon.erase(it);

			int refine_addon = it3->levelup_addon;
			addon_data  refine_data;
			if(!world_manager::GetDataMan().generate_addon(refine_addon,refine_data)) return 0; 
			refine_data.arg[0] = (int)(refine_data.arg[0] * refine_factor[refine_level] + 0.1f);
			refine_data.arg[1] = refine_level;
			_refine_addon_total.push_back(refine_data);

			for(int i = 0; i < 12; i++)
			{
				int addon_id = it3->levelup_addons[i].id_addon;
				int addon_level = it3->levelup_addons[i].refine_level;

				if(addon_id > 0 && addon_level <= refine_level)
				{
					addon_data  newdata;
					if(!world_manager::GetDataMan().generate_addon(addon_id,newdata)) return 0; 
					_total_addon.push_back(newdata);
				}			
			}

			this->_ess.refine_valid = 1; // Ativa o refino
			this->_ess.refine_list_addons = (_refine_addon_total)[0].id; // Adiciona a lista de addons
			this->_ess.refine_addon = (int)(_refine_addon_total)[0].arg[0]; // Adiciona o nível do addon
			this->_ess.refine_level = refine_level; // Adiciona o nível do refino

			RefreshEquip();

			} else {

					ADDON_LIST::iterator it2 = _refine_addon_total.begin();
					for(; it2 != _refine_addon_total.end(); )
					_refine_addon_total.erase(it2);	

					ADDON_LIST::iterator it = _total_addon.begin();
					for(; it != _total_addon.end(); )
					_total_addon.erase(it);	
		
					this->_ess.refine_valid = 0;
					this->_ess.refine_list_addons = 0;
					this->_ess.refine_addon = 0; 
					this->_ess.refine_level = 0;

					RefreshEquip();
			}
			
		}

	}
	
	return rst;
}


bool 
bible_item::TransmitRefine(int level)
{
	DATA_TYPE dt3;
	const BIBLE_ESSENCE *it3 = (const BIBLE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE,dt3);

	if(level == 0)
	{
		ADDON_LIST::iterator it2 = _refine_addon_total.begin();
		for(; it2 != _refine_addon_total.end(); )
		_refine_addon_total.erase(it2);

		ADDON_LIST::iterator it = _total_addon.begin();
		for(; it != _total_addon.end(); )
		_total_addon.erase(it);	
		
		this->_ess.refine_valid = 0;
		this->_ess.refine_list_addons = 0;
		this->_ess.refine_addon = 0; 
		this->_ess.refine_level = 0;

		RefreshEquip();
		return true;
	}

	ADDON_LIST::iterator it2 = _refine_addon_total.begin();
	for(; it2 != _refine_addon_total.end(); )
	_refine_addon_total.erase(it2);

	int refine_addon = it3->levelup_addon;
	addon_data  refine_data;
	if(!world_manager::GetDataMan().generate_addon(refine_addon,refine_data)) return 0; 
	refine_data.arg[0] = (int)(refine_data.arg[0] * refine_factor[level] + 0.1f);
	refine_data.arg[1] = level;
	_refine_addon_total.push_back(refine_data);

	ADDON_LIST::iterator it = _total_addon.begin();
	for(; it != _total_addon.end(); )
	_total_addon.erase(it);

	for(int i = 0; i < 12; i++)
	{
		int addon_id = it3->levelup_addons[i].id_addon;
		int addon_level = it3->levelup_addons[i].refine_level;

		if(addon_id > 0 && addon_level <= level)
		{
			addon_data  newdata;
			if(!world_manager::GetDataMan().generate_addon(addon_id,newdata)) return 0; 
			_total_addon.push_back(newdata);
		}			
	}

	this->_ess.refine_valid = 1; // Ativa o refino
	this->_ess.refine_list_addons = (_refine_addon_total)[0].id; // Adiciona a lista de addons
	this->_ess.refine_addon = (int)(_refine_addon_total)[0].arg[0]; // Adiciona o nível do addon
	this->_ess.refine_level = level; // Adiciona o nível do refino

	RefreshEquip();

	return true;
}
