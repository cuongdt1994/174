#include "../clstab.h"
#include "../world.h"
#include "../player_imp.h"
#include "../playertemplate.h"
#include "../template/itemdataman.h"
#include "item_camp_token.h"
#include <glog.h>

DEFINE_SUBSTANCE(camp_token_item,item_body,CLS_ITEM_CAMP_TOKEN);

bool 
camp_token_item::Save(archive & ar)
{
	try
	{
		ar << _ess;
		SaveVectorCampToken(ar);
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

bool 
camp_token_item::Load(archive & ar)
{	
	_raw_data.clear();
	_raw_data.push_back(ar.data(),ar.size());
	try
	{
		ar >> _ess;
		LoadVectorCampToken(ar);
	}
	catch(...)
	{
		return false;
	}

	GenerateAddons();	
	return true;
}

void 
camp_token_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Activate(_total_addon[i],NULL,obj,GetAddonRatio(i));
	}
}

void 
camp_token_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Deactivate(_total_addon[i],NULL,obj,GetAddonRatio(i));
	}
}

float
camp_token_item::GetAddonRatio(int index)
{
	if(index > _total_addon.size())
		return 0.f;

	return _vector.addon_ratio_pos[index];
}

bool 
camp_token_item::VerifyRequirement(item_list & list,gactive_imp* obj) 
{
	if(list.GetLocation() == item::BODY)
	{
		return (obj->GetHistoricalMaxLevel() >= _ess.level && 
			((1 << (obj->GetObjectClass() & 0x0F)) & _ess.race) &&
			obj->_cur_prop.strength >= _ess.strength &&
			obj->_cur_prop.vitality >= _ess.vitality &&
			obj->_cur_prop.agility >= _ess.agility &&
			obj->_cur_prop.energy >= _ess.energy &&
			_ess.durability > 0);
	}
	else
	{
		return false;
	}
}

void
camp_token_item::GenerateAddons()
{
	_total_addon.clear();
	
	ADDON_LIST::iterator it = _total_addon.begin();
	for(; it != _total_addon.end(); )
	_total_addon.erase(it);

	memset(&_vector.addon_ratio_pos, 0, sizeof(_vector.addon_ratio_pos));

	// Busca as informacoes do item
	DATA_TYPE dt;
	CAMP_TOKEN_ESSENCE *ess = (CAMP_TOKEN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_CAMP_TOKEN_ESSENCE)
		return;

	// Busca a configuração 
	DATA_TYPE dt2;
	CAMP_TOKEN_CONFIG *cfg = (CAMP_TOKEN_CONFIG *)world_manager::GetDataMan().get_data_ptr(ess->camp_token_id, ID_SPACE_CONFIG, dt2);
	if (!cfg || dt2 != DT_CAMP_TOKEN_CONFIG)
		return;

	for(int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
	{
		DATA_TYPE dt3;
		CAMP_TOKEN_TREE_CONFIG *cfg2 = (CAMP_TOKEN_TREE_CONFIG *)world_manager::GetDataMan().get_data_ptr(cfg->level[i].camp_token_tree_id, ID_SPACE_CONFIG, dt3);
		if (!cfg2 || dt3 != DT_CAMP_TOKEN_TREE_CONFIG)
			return;

		addon_data addon_data;
		int addon_id = cfg2->node[_vector.pos_camp_addon[i]].addon_id;
		float addon_ratio = cfg2->node[_vector.pos_camp_addon[i]].addon_ratio;

		if (addon_id != 0)
		{
			if (!world_manager::GetDataMan().generate_addon(addon_id, addon_data))
				return;

			_total_addon.push_back(addon_data);
			_vector.addon_ratio_pos[i] = addon_ratio;		
		}
	}
}

static const float 
bonus_factor_level_percent[] = { 0, 8.33f, 7.14f, 6.25f, 5.56f, 5.00f, 4.55f, 4.17f, 3.85f, 3.57f, 3.33f, 3.13f, 2.94f, 2.78f, 2.63f, 2.50f, 2.38f, 2.27f, 2.17f, 2.08f, 2.00f, 1.92f, 1.85f, 1.79f, 1.72f, 1.67f, 1.61f, 1.52f, 1.47f, 1.43f, 1.39f,
1.35f, 1.32f, 1.28f, 1.25f, 1.22f, 1.19f, 1.16f, 1.14f, 1.11f, 1.09f, 1.07f, 1.05f, 1.03f, 1.01f, 1.00f, 0.98f, 0.97f, 0.95f, 0.94f, 0.93f, 0.91f, 0.90f, 0.89f, 0.88f, 0.87f, 0.86f, 0.85f, 0.84f, 0.83f, 0.82f, 0.81f, 0.80f, 0.79f, 0.78f, 0.77f, 0.76f, 0.75f, 
0.74f, 0.73f, 0.72f, 0.71f, 0.70f, 0.69f, 0.68f, 0.67f, 0.66f, 0.65f, 0.64f, 0.63f, 0.62f, 0.61f, 0.60f, 0.59f, 0.58f, 0.57f, 0.56f, 0.55f, 0.54f, 0.53f, 0.52f, 0.51f, 0.50f, 0.49f, 0.48f, 0.47f, 0.46f, 0.45f, 0.44f, 0.43f, 0.42f, 0.41f, 0.40f, 0.39f, 0.38f, 
0.37f, 0.36f, 0.35f, 0.34f, 0.33f, 0.32f, 0.31f, 0.30f, 0.29f, 0 };

bool 
camp_token_item::AddCampTokenExp(unsigned int exp)
{
	// Busca as informacoes do item
	DATA_TYPE dt;
	CAMP_TOKEN_ESSENCE *ess = (CAMP_TOKEN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_CAMP_TOKEN_ESSENCE)
		return false;

	// Busca a configuração 
	DATA_TYPE dt2;
	CAMP_TOKEN_CONFIG *cfg = (CAMP_TOKEN_CONFIG *)world_manager::GetDataMan().get_data_ptr(ess->camp_token_id, ID_SPACE_CONFIG, dt2);
	if (!cfg || dt2 != DT_CAMP_TOKEN_CONFIG)
		return false;

	int get_exp = _ess.exp;
	int get_level = _ess.level_camp;
	int get_required_exp = cfg->level[get_level - 1].require_exp;

	if(get_level >= MAX_CAMP_TOKEN_LEVEL)
		return false;

	float rand_new = 0.00f;
	rand_new += (float)(rand() % 10001) / 100.0f;

	// Sistema de bonificação de level
	if ( bonus_factor_level_percent[get_level] >= rand_new )
	{
		_ess.exp = 0;
		_ess.level_camp++;

		_ess.size_camptoken += 3;

		// Adiciona novos vectores
		_vector.pos_camp_token[_ess.level_camp - 1] = 1;
		_vector.pos_camp_addon[_ess.level_camp - 1] = 0;
		_vector.pos_camp_reserve[_ess.level_camp - 1] = 0;

		// Atualiza o item
		OnRefreshItem();
		return true;
	}

	// Upa o item
	if(get_exp + exp >= get_required_exp)
	{
		_ess.exp = get_exp + exp - get_required_exp;
		_ess.level_camp++;

		_ess.size_camptoken += 3;
		// Adiciona novos vectores
		_vector.pos_camp_token[_ess.level_camp - 1] = 1;
		_vector.pos_camp_addon[_ess.level_camp - 1] = 0;
		_vector.pos_camp_reserve[_ess.level_camp - 1] = 0;
	}
	else // Somente adiciona a exp
	{
		_ess.exp += exp;
	}	

	// Atualiza o item
	OnRefreshItem();
	return true;
}

bool 
camp_token_item::GenerateNewPoint(unsigned int pos_line_key, int item_id_key_bonus, int parent_key_bonus)
{
	float bonus_factor = 0.00f;
	float get_prob = (float)((rand()%100)*0.01f);

	// Busca as informacoes do item
	DATA_TYPE dt;
	CAMP_TOKEN_ESSENCE *ess = (CAMP_TOKEN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_CAMP_TOKEN_ESSENCE)
		return false;

	// Busca a configuração (id)
	DATA_TYPE dt2;
	CAMP_TOKEN_CONFIG *cfg = (CAMP_TOKEN_CONFIG *)world_manager::GetDataMan().get_data_ptr(ess->camp_token_id, ID_SPACE_CONFIG, dt2);
	if (!cfg || dt2 != DT_CAMP_TOKEN_CONFIG)
		return false;

	// Busca a configuração geral
	DATA_TYPE dt3;
	CAMP_TOKEN_TREE_CONFIG *cfg2 = (CAMP_TOKEN_TREE_CONFIG *)world_manager::GetDataMan().get_data_ptr(cfg->level[pos_line_key - 1].camp_token_tree_id, ID_SPACE_CONFIG, dt3);
	if (!cfg2 || dt3 != DT_CAMP_TOKEN_TREE_CONFIG)
		return false;

	// Bonus de item
	bool is_bonus = false;
	if(item_id_key_bonus != -1)
	{
		DATA_TYPE dt4;
		CAMP_TOKEN_PROB_ADJUST_ESSENCE *ess2 = (CAMP_TOKEN_PROB_ADJUST_ESSENCE *)world_manager::GetDataMan().get_data_ptr(item_id_key_bonus, ID_SPACE_ESSENCE, dt4);
		if (!ess2 || dt4 != DT_CAMP_TOKEN_PROB_ADJUST_ESSENCE)
			return false;

		bonus_factor += ess2->adjust_probability;
		is_bonus = true;
	}

	int atual_child_node = _vector.pos_camp_token[pos_line_key - 1];
	int next_child_node = cfg2->node[atual_child_node].first_child;

	if(atual_child_node == 0 || next_child_node == 0) return false; // Caso o ítem chegou no nível máximo
	if(is_bonus && parent_key_bonus == 0) return false; // Caso tenha um item e a posição não foi selecionada

	int count = 0;
	while (true) 
	{
		float perg_bonus = (is_bonus ? next_child_node == parent_key_bonus ? cfg2->node[next_child_node].probability += bonus_factor : 
							cfg2->node[next_child_node].probability -= bonus_factor : cfg2->node[next_child_node].probability);

		// Corrige os valores para não ficar negativo
		if(perg_bonus < 0.00f) perg_bonus = 0.00f;	
		if(get_prob < 0.00f) get_prob = 0.00f;		
		if(get_prob > 1.00f) get_prob = 0.00f;		

		// Corrige o problema da probabilidade ficar negativa
		if(is_bonus && next_child_node == parent_key_bonus && perg_bonus <= 0.00f) perg_bonus = cfg2->node[next_child_node].probability + bonus_factor;
		
		if (get_prob < perg_bonus)	
		{		
			_vector.pos_camp_token[pos_line_key - 1] = next_child_node;
			if(cfg2->node[next_child_node].addon_id > 0 && _vector.pos_camp_addon[pos_line_key - 1] == 0)
			{
				_vector.pos_camp_addon[pos_line_key - 1] = next_child_node;
			}			
			break;
		}
		else 
		{
			get_prob -= cfg2->node[next_child_node].probability;
		}

		next_child_node = cfg2->node[next_child_node].next_sibling;

		count++;
		if (count > 100) break;
	}
	
	// Atualiza o item
	OnRefreshItem();
	return true;
}

// Substitui o atributo por um novo
bool 
camp_token_item::ActivateAddonLineKey(unsigned int pos_line_key, int addon_line_key_pos)
{
	if(pos_line_key < 0)
		return false;

	_vector.pos_camp_addon[pos_line_key - 1] = addon_line_key_pos;

	// Atualiza o item
	OnRefreshItem();
	return true;
}

bool 
camp_token_item::ResetAddonLineKey(unsigned int pos_line_key, int return_count)
{
	if(return_count < 0 || return_count > 1)
		return false;

	// Busca as informacoes do item
	DATA_TYPE dt;
	CAMP_TOKEN_ESSENCE *ess = (CAMP_TOKEN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	if (!ess || dt != DT_CAMP_TOKEN_ESSENCE)
		return false;

	// Busca a configuração (id)
	DATA_TYPE dt2;
	CAMP_TOKEN_CONFIG *cfg = (CAMP_TOKEN_CONFIG *)world_manager::GetDataMan().get_data_ptr(ess->camp_token_id, ID_SPACE_CONFIG, dt2);
	if (!cfg || dt2 != DT_CAMP_TOKEN_CONFIG)
		return false;

	// Busca a configuração geral
	DATA_TYPE dt3;
	CAMP_TOKEN_TREE_CONFIG *cfg2 = (CAMP_TOKEN_TREE_CONFIG *)world_manager::GetDataMan().get_data_ptr(cfg->level[pos_line_key - 1].camp_token_tree_id, ID_SPACE_CONFIG, dt3);
	if (!cfg2 || dt3 != DT_CAMP_TOKEN_TREE_CONFIG)
		return false;

	if(return_count) // Volta 1 slot
	{
		int get_old_pos = -1;

		for (int i = 0; i < 128; i++)
		{
			if(cfg2->node[i].first_child == _vector.pos_camp_token[pos_line_key - 1])
			{
				get_old_pos = i;
				break;
			}			
		}

		if(get_old_pos < 1) get_old_pos = 1;

		_vector.pos_camp_token[pos_line_key - 1] = get_old_pos;
	} else // Reseta
	{
		_vector.pos_camp_token[pos_line_key - 1] = 1;
	}

	// Atualiza o item
	OnRefreshItem();
	return true;
}
