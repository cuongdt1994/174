#include "../clstab.h"
#include "../world.h"
#include "../player_imp.h"
#include "item_astrolabe.h"
#include "../playertemplate.h"
#include "../template/itemdataman.h"
#include "../emulate_settings.h"

DEFINE_SUBSTANCE(astrolabe_item,item_body,CLS_ITEM_ASTROLABE)

void astrolabe_aptit_limit::Init()
{
enum
{
	ASTROLABE_APTIT_MAX  		= 500,
	ASTROLABE_APTIT_MIN	 		= 10,
	ASTROLABE_APTIT_INIT 		= 500,
	ASTROLABE_APTIT_INIT_LIMIT 	= 200,
	ASTROLABE_APTIT_SUM_MAX 	= 2000,
};
	
	if(max > ASTROLABE_APTIT_MAX) max = ASTROLABE_APTIT_MAX;
	if(min < ASTROLABE_APTIT_MIN) min = ASTROLABE_APTIT_MIN;
	ASSERT(max >= min);	
	if(total > ASTROLABE_APTIT_SUM_MAX) total = ASTROLABE_APTIT_SUM_MAX;
	if(init_total > ASTROLABE_APTIT_INIT) init_total = ASTROLABE_APTIT_INIT;
	if(init_max > ASTROLABE_APTIT_INIT_LIMIT) init_max = ASTROLABE_APTIT_INIT_LIMIT;
	
}

void 
astrolabe_item::OnDump(std::string& str)
{
enum
{
	HIGH_ADDON_COUNT = 6,
};
	std::ostringstream ostr;

	ostr << "[" << _tid << "]";

	if(_total_addon.size() >= HIGH_ADDON_COUNT)
	{
		ostr << "{ ";
		ostr << (int)_ess.level << " " << _ess.exp;
		for(unsigned int n = 0, i = 0; n < ASTROLABE_VIRTUAL_SLOT_COUNT; ++n)
		{
			ostr << " " << _ess.GetAptit(n) << " ";
			if((_ess.slot & (1 << n))&& i < _total_addon.size())	
			{
				ostr << (_total_addon[i].id & addon_manager::ADDON_PURE_TYPE_MASK);
				++i;
			}
			else 
			{
				ostr << "0";
			}
		}
		ostr << " }";
	}

	str = ostr.str();
}

void 
astrolabe_item::OnRebuild(void* data,unsigned int len)
{
#pragma pack(1)
	struct astrolabe_rebulid_data
	{
		int level;
		int exp;
		struct
		{
			int aptit;
			int addon;
		}slot[ASTROLABE_VIRTUAL_SLOT_COUNT];
	};
#pragma pack()

	if(len == sizeof(astrolabe_rebulid_data))
	{
		astrolabe_rebulid_data* ess = (astrolabe_rebulid_data*)data;
		_ess.exp = ess->exp;
		_ess.level = ess->level;
		_ess.slot = 0;

		_total_addon.clear();
		for(int n = 0; n < ASTROLABE_VIRTUAL_SLOT_COUNT; ++n)
		{
			if(n % 2)// ��Ȧ
				_ess.aptit[n/2] = (unsigned short) ess->slot[n].aptit;

			if(ess->slot[n].addon)
			{
				addon_data newaddon;
				if(world_manager::GetDataMan().generate_addon(ess->slot[n].addon,newaddon))
				{
					_total_addon.push_back(newaddon);	
					_ess.slot |= (1 << n);
				}
			}
		}

		OnRefreshItem();
	}
}

void
astrolabe_item::OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	if(l == item::BODY)
		Deactivate(l,pos,count,obj);
}

void astrolabe_item::OnPutIn(item::LOCATION l,item_list & list,unsigned int pos,unsigned int count,gactive_imp* obj)
{
	//do nothing  ��Ϊ�����������RefreshEquip����
}

void 
astrolabe_item::OnRefreshItem()
{

	// ˢ�»���
	_raw_data.clear();
	raw_wrapper rw;
	Save(rw);
	rw.swap(_raw_data);
	CalcCRC();
}

bool
astrolabe_item::Save(archive & ar)
{
	try
	{
		ar << _ess;
		SaveAddOn(ar);
	}
	catch(...)
	{
		return false;
	}
	
	return true;
}

bool
astrolabe_item::Load(archive & ar)
{
	ASSERT(_tid > 0);
	//����������raw_data��
	//Ҫ��ar�ض���ͷ��
	ASSERT(ar.offset() == 0);
	_raw_data.clear();
	_raw_data.push_back(ar.data(),ar.size());
	try
	{
		ar >> _ess;
		LoadAddOn(ar);
	}
	catch(...)
	{
		return false;
	}
	
	UpdateEssence();	
	return true;
}

void 
astrolabe_item::UpdateEssence()
{
	DATA_TYPE dt;
	const ASTROLABE_ESSENCE & ess = *(const ASTROLABE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	ASSERT(&ess || dt == DT_ASTROLABE_ESSENCE);
	
	_extend.swallow_exp = ess.base_swallow_exp;
	_extend.level_limit = ess.require_level;
	_extend.race_limit = ess.character_combo_id;

	_aptit_limit.max = ess.max_inner_point_value;
	_aptit_limit.min = ess.min_inner_point_value;
	_aptit_limit.total = ess.max_all_inner_point_value;
	_aptit_limit.init_total = ess.init_all_inner_point_value;
	_aptit_limit.init_max = ess.max_init_inner_point_value;
	_aptit_limit.Init();
}

void 
astrolabe_item::OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Activate(_total_addon[i],NULL,obj,GetAddonRatio(i));
	}
}

void 
astrolabe_item::OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj)
{
	for(unsigned int i = 0;i < _total_addon.size(); i ++)
	{
		addon_manager::Deactivate(_total_addon[i],NULL,obj,GetAddonRatio(i));
	}
}

bool astrolabe_item::VerifyRequirement(item_list & list, gactive_imp* imp)
{
	if(list.GetLocation() == item::BODY)
	{
		return imp->GetHistoricalMaxLevel() >= _extend.level_limit &&
			((1 << (imp->GetObjectClass() & 0x0F)) & _extend.race_limit);
	}
	return false;
}

bool astrolabe_item::InsertExp(int& exp, int max_lvl, int astral_lvl, bool ischeck)
{
	int ASTROLABE_LEVEL_MAX = (EmulateSettings::GetInstance()->GetEmulateVersion() < 176) ? 69 : 104;

	int tmp = _ess.exp + exp;
	if(tmp < _ess.exp) return false;
	_ess.exp = tmp;

	do
	{
		int lvlup_exp = player_template::GetAstrolabeLvlupExp(_ess.level);
		if(_ess.exp < lvlup_exp) break;
		
		_ess.exp -= lvlup_exp;
		++_ess.level;
		
		if(_ess.level >= ASTROLABE_LEVEL_MAX)
		{
			_ess.exp = 0;
			break;
		}
		

	}while(1);

	OnRefreshItem(); 
	return true;
}

int astrolabe_item::GetSwallowExp()
{
	int exp = _ess.exp + player_template::GetAstrolabeLvltotalExp(_ess.level) + _extend.swallow_exp;
	return exp;
}

float astrolabe_item::GetAddonRatio(int index)
{
	if(index >= ASTROLABE_VIRTUAL_SLOT_COUNT) return 0.f; 
	int slot = -1,vslot = 0; 
	for(; vslot < ASTROLABE_VIRTUAL_SLOT_COUNT; ++vslot)
	{
		if(_ess.slot & (1 << vslot)) ++slot;
		if(slot == index) break;
	}
	if(slot != index) 
	{
		// log
		return 0.f;
	}

	if( _aptit_limit.total - _ess.SumAptit() > 0) // ����δ��
		return 25.f + (_ess.level+1)*int(_ess.GetAptit(vslot)/100.f);
	else
		return 25.f + (_ess.level+1)*_ess.GetAptit(vslot)/100.f;
}

bool astrolabe_item::OnInherit(item_body* other)
{
	if(!other || other->GetItemType() != ITEM_TYPE_ASTROLABE) return false;
	astrolabe_item* body = (astrolabe_item*) other;

	if(_aptit_limit.total < body->_aptit_limit.total ||
	   _aptit_limit.min > body->_aptit_limit.min ||
	   _aptit_limit.max < body->_aptit_limit.max) 
		return false; // ���Ʋ���

	memcpy(&_ess.aptit, &body->_ess.aptit, sizeof(_ess.aptit));
	OnRefreshItem();
	return true;
}

bool astrolabe_item::DoRebirth(int arg)
{
	if(!arg || arg > ASTROLABE_ADDON_MAX) return false;
	_total_addon.clear();
	
	DATA_TYPE dt;
	const ASTROLABE_ESSENCE & ess = *(const ASTROLABE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	ASSERT(&ess || dt == DT_ASTROLABE_ESSENCE);

	addon_data emptyaddon = {0,0,0,0};
	_total_addon.insert(_total_addon.begin(),arg,emptyaddon);
	world_manager::GetDataMan().generate_astrolabe_addonlist((const char*)&(ess.rands[0]),32,&_total_addon[0],arg, ess.id_rand_when_fail);	
	ShuffleSlot();
	OnRefreshItem();
	return true;
}

bool astrolabe_item::ForceAddonsGenerate(short * addons_pos) /*176+*/
{
	_total_addon.clear();
	
	ADDON_LIST::iterator it = _total_addon.begin();
	for(; it != _total_addon.end(); )
	_total_addon.erase(it);

	DATA_TYPE dt;
	const ASTROLABE_ESSENCE & ess = *(const ASTROLABE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
	ASSERT(&ess || dt == DT_ASTROLABE_ESSENCE);

	for(int i = 0; i < ASTROLABE_ADDON_MAX; i++)
	{
		int addon_id = ess.rands[addons_pos[i]].id_rand;
		if(addon_id > 0)
		{
			addon_data newdata;
			if(!world_manager::GetDataMan().generate_addon(addon_id,newdata)) return false; 
			_total_addon.push_back(newdata);
		}			
	}

	ShuffleSlot();
	OnRefreshItem();
	return true;
}

void astrolabe_item::ShuffleSlot()
{
	if(_total_addon.size() > ASTROLABE_VIRTUAL_SLOT_COUNT)
	{
		//log
		return;
	}

	_ess.slot = 0;
	int slotnum = 0;
	for(int n = 0; n < MAX_TRY_LOOP_TIME && slotnum < (int) _total_addon.size(); ++n)
	{
		int index = rand()%ASTROLABE_VIRTUAL_SLOT_COUNT;
		if(_ess.slot & (1 << index)) continue;
		if(index%2 == 0) // ��Ȧ
		{
			int i1 = (index+1)%ASTROLABE_VIRTUAL_SLOT_COUNT;
			int i2 = index ? (index-1) : ASTROLABE_VIRTUAL_SLOT_COUNT - 1;
			if((_ess.slot & (1 << i1)) == 0 || (_ess.slot & (1 << i2)) == 0)
				continue;
		}
		_ess.slot |= (1 << index); 
		++slotnum;			
	}

	if(slotnum <  (int) _total_addon.size())
	{
		int needslot = _total_addon.size() - slotnum;
		while(needslot)
		{
			for(int n = 0; n < ASTROLABE_VIRTUAL_SLOT_COUNT; ++n)
			{
				if(_ess.slot & (1 << n)) continue;
				if(n%2 == 0) // ��Ȧ
				{
					int i1 = (n+1)%ASTROLABE_VIRTUAL_SLOT_COUNT;
					int i2 = n ? (n-1) : ASTROLABE_VIRTUAL_SLOT_COUNT - 1;
					if((_ess.slot & (1 << i1)) == 0 || (_ess.slot & (1 << i2)) == 0)
						continue;
				}

				_ess.slot |= (1 << n);
				--needslot;
			}
		}	
	}
}

void astrolabe_item::OnUnpackage(gactive_imp* imp)
{
	_ess.InitAptit(_aptit_limit.init_total, _aptit_limit.init_max, _aptit_limit.min);
	gplayer_imp* player = (gplayer_imp*) imp;
	DoRebirth(player_template::GetAstrolabeAddonCount(player->GetAstrolabeExternLevel()));
}

bool astrolabe_item::FlushGeniusPoint()
{
	ShuffleSlot();
	ShuffleAddon();
	OnRefreshItem();
	return true;
}

void astrolabe_item::ShuffleAddon()
{
	unsigned int ts = _total_addon.size();
	if(ts <= 2)  return;
	unsigned int t = 1 + ts/2;
	for(unsigned int i = 0; i < t; ++i)
	{
		unsigned int s1 = rand()%ts;
		unsigned int s2 = rand()%ts;
		if(s1 != s2)	abase::swap(_total_addon[s1],_total_addon[s2]);
	}
}

bool astrolabe_item::AddGeniusPoint(short g0, short g1, short g2, short g3, short g4, bool ischeck)
{
	int sum = _ess.SumAptit();
	
	if(sum < g1 || sum > g2) return false;

	int limit = _aptit_limit.total - sum;
	
	if(limit > 0)
	{
		_ess.AddAptit(g0 > limit ? limit : g0, _aptit_limit.max);	
	}
	else
	{
		_ess.InitAptit(_aptit_limit.total, _aptit_limit.max, _aptit_limit.min);
	}
	OnRefreshItem();
	return true;
}

int astrolabe_item::OnGetLevel()
{
	return  _ess.SumAptit();
}

int astrolabe_item::GetLevel()
{
	return  _ess.level;
}
