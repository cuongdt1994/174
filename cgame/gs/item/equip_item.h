#ifndef __ONLINEGAME_GS_EQUIP_ITEM_H__
#define __ONLINEGAME_GS_EQUIP_ITEM_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include "set_addon.h"
#include <crc.h>
#include <glog.h>

#include "../player_lua.h"

#pragma pack(push, 1)
struct new_spirit_essence
{
	char spirit_inject;
	char crystal_inject;

	int spirit_id;
	char reserved; //1
	char spirit_level;
	char lock_unlock;

	int addons_count;

	struct addon
	{
		int id_addon;
		int arg;
	} addons[5];

};
#pragma pack(pop)

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const new_spirit_essence & spirit)
{
	return wrapper.push_back(&spirit,sizeof(spirit));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, new_spirit_essence & spirit)
{
	return wrapper.pop_back(&spirit,sizeof(spirit));
}

#pragma pack(push, 1)
struct new_crystal_essence
{
	int crystal_id;
	char crystal_level;
	char crystal_color;

	int exp;
	char lock_unlock;

	char stars[5]; // Estrelas nv 0 até 5

	// Reserve
	char res[15];

	int addons_count;

	struct addon
	{
		int id;
		int arg;
	} addons[2];
};
#pragma pack(pop)

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const new_crystal_essence & crystal)
{
	return wrapper.push_back(&crystal,sizeof(crystal));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, new_crystal_essence & crystal)
{
	return wrapper.pop_back(&crystal,sizeof(crystal));
}

class addon_equip_item : public item_body
{
	//���������ֻ�������addon������ʹ�ú���Ч�����为��
protected:
	ADDON_LIST _active_addon;	//������Ҫ���������
	ADDON_LIST _total_addon;	//�������е�����
	const ADDON_LIST * _extra_addon;
	addon_data _use_addon;		//������Ҫʹ�õ�����
	int	_addon_expire_date;		//������ڸ������ԵĹ���ʱ��

	ADDON_LIST _new_active_addon;
	ADDON_LIST _new_total_addon;

	new_spirit_essence _spirit;
	new_crystal_essence _crystal;

protected:
	addon_equip_item()
	{
		_use_addon.id= -1;
		_extra_addon = NULL;
		_addon_expire_date = 0;

		memset(&_spirit,0,sizeof(_spirit));
		memset(&_crystal,0,sizeof(_crystal));
	}
	
	void LoadAddOn(archive &ar)
	{
		if(GetItemType () != ITEM_TYPE_NEW_ARMOR)
		{
			unsigned int count;
			int argcount;
			ar >> count;
			if (count < 0 || count > 128)
			{
				throw -100;
			}

			for (unsigned int i = 0; i < count; i++)
			{
				addon_data entry;
				memset(&entry, 0, sizeof(entry));

				ar >> entry.id;
				argcount = addon_manager::GetArgCount(entry.id);
				for (int j = 0; j < argcount; j++)
				{
					ar >> entry.arg[j];
				}
				_total_addon.push_back(entry);
			}
		} else 
		{
			if(_spirit.spirit_inject)
			{
				GenerateNewAddon();
			} else
			{
				GenerateAddonOld();
			}

			unsigned int count;
			int argcount;
			ar >> count;
			if (count < 0 || count > 128)
			{
				throw -100;
			}

			for (unsigned int i = 0; i < count; i++)
			{
				addon_data entry;
				memset(&entry, 0, sizeof(entry));

				ar >> entry.id;
				argcount = addon_manager::GetArgCount(entry.id);
				for (int j = 0; j < argcount; j++)
				{
					ar >> entry.arg[j];
				}
				_total_addon.push_back(entry);
			}

			// Valores Fixos
			ar >> _spirit.spirit_inject;
			ar >> _spirit.crystal_inject;


			// Se o Espírito for Injetado
			if(_spirit.spirit_inject)
			{
				ar >> _spirit.spirit_id;
				ar >> _spirit.reserved;
				ar >> _spirit.spirit_level;
				ar >> _spirit.lock_unlock;

				ar >> _spirit.addons_count;
				for(unsigned int i = 0; i < _spirit.addons_count; i++)
				{
					ar >> _spirit.addons[i].id_addon;
					ar >> _spirit.addons[i].arg;
				}
			}


			// Se o cristal for injetado
			if(_spirit.crystal_inject)
			{
				ar >> _crystal.crystal_id;
				ar >> _crystal.crystal_level;
				ar >> _crystal.crystal_color;
				
				ar >> _crystal.exp;

				ar >> _crystal.lock_unlock;

				for(unsigned int i = 0; i < 5; i++)
				{
					ar >> _crystal.stars[i];
				}

				for (unsigned int i = 0; i < 15; i++)
				{
					ar >> _crystal.res[i];
				}

				ar >> _crystal.addons_count;
				for(unsigned int i = 0; i < _crystal.addons_count; i++)
				{
					ar >> _crystal.addons[i].id;
					ar >> _crystal.addons[i].arg;
				}

				unsigned int reserved = 0;
				ar >> reserved;
			}
		}

		_extra_addon = set_addon_manager::GetAddonList(_tid);
		
	}

	inline void GenerateAddonOld()
	{
		// Addons Fixos
		DATA_TYPE dt;
		NEW_ARMOR_ESSENCE *ess = (NEW_ARMOR_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
		if (!ess || dt != DT_NEW_ARMOR_ESSENCE)
			return;

		ADDON_LIST::iterator it = _new_total_addon.begin();
		for (; it != _new_total_addon.end();)
			_new_total_addon.erase(it);

		for (unsigned int i = 0; i < 32; i++)
		{
			addon_data addon_data;
			int addon_id = ess->addons[i].id_addon;

			if(addon_id != 0)
			{
				if (!world_manager::GetDataMan().generate_addon(addon_id, addon_data))
				return;

				_new_total_addon.push_back(addon_data);
			}			
		}

		OnRefreshItem();
		return;
	}

	enum
	{
		POS_EQUIP_MASK_HEAD = 1,
		POS_EQUIP_MASK_SHOULDER = 4,
		POS_EQUIP_MASK_LEG = 6,
		POS_EQUIP_MASK_FOOT = 7,
		POS_EQUIP_MASK_ARM = 8,
	};
	
	inline void GenerateNewAddon()
	{	
		DATA_TYPE dt;
		NEW_ARMOR_ESSENCE *ess = (NEW_ARMOR_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid, ID_SPACE_ESSENCE, dt);
		if (!ess || dt != DT_NEW_ARMOR_ESSENCE)
			return;

		DATA_TYPE dt2;
		const QIHUN_ESSENCE & ess2 = *(const QIHUN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_spirit.spirit_id,ID_SPACE_ESSENCE,dt2);
		if(!&ess2 && dt2 != DT_QIHUN_ESSENCE) 
			return;

		ADDON_LIST::iterator it = _new_total_addon.begin();
		for (; it != _new_total_addon.end();)
			_new_total_addon.erase(it);

		for (unsigned int i = 0; i < _spirit.addons_count; i++)
		{
			addon_data new_addon_data;
			int addon_id = _spirit.addons[i].id_addon-ENGRAVE_PACKET_SUM_ID;
		
			bool is_speed = false;
			
			if(!is_speed)
			{
				if (!world_manager::GetDataMan().generate_addon(addon_id, new_addon_data))
				return;

				new_addon_data.id = _spirit.addons[i].id_addon;
				new_addon_data.arg[0] = _spirit.addons[i].arg;
			}
			
			_new_total_addon.push_back(new_addon_data);
		}

		if (_spirit.crystal_inject)
		{
			for (unsigned int i = 0; i < _crystal.addons_count; i++)
			{
				addon_data crystal_data;
				int addon_id = ess->addons[i].id_addon;
				if (!world_manager::GetDataMan().generate_addon(addon_id, crystal_data))
					return;

				crystal_data.id = _crystal.addons[i].id;
				crystal_data.arg[0] = (_crystal.addons[i].arg * (int)_crystal.stars[_crystal.crystal_color]);

				_new_total_addon.push_back(crystal_data);
			}

			// Buff do Cristal
			if ((int)_crystal.stars[_crystal.crystal_color] > 0)
			{				
				DATA_TYPE dt1;
				const QILING_ESSENCE & ess2 = *(const QILING_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_crystal.crystal_id,ID_SPACE_ESSENCE,dt1);
				if(!&ess2 && dt1 != DT_QILING_ESSENCE) 
					return;

				DATA_TYPE dt2;
				const QIHUN_ESSENCE & ess3 = *(const QIHUN_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_spirit.spirit_id,ID_SPACE_ESSENCE,dt2);
				if(!&ess3 && dt2 != DT_QIHUN_ESSENCE) 
					return;
				
				int equip_mask = ess3.equip_mask == item::EQUIP_MASK64_HEAD ? POS_EQUIP_MASK_HEAD :
							 ess3.equip_mask == item::EQUIP_MASK64_BODY ? POS_EQUIP_MASK_SHOULDER : 
							 ess3.equip_mask == item::EQUIP_MASK64_LEG ? POS_EQUIP_MASK_LEG : 
							 ess3.equip_mask == item::EQUIP_MASK64_FOOT ? POS_EQUIP_MASK_FOOT : 
							 ess3.equip_mask == item::EQUIP_MASK64_WRIST ? POS_EQUIP_MASK_ARM : 0;

				int get_addon = ess2.equipment[equip_mask].addon[(int)_crystal.crystal_color];

				DATA_TYPE dt3;
				EQUIPMENT_ADDON *addon = (EQUIPMENT_ADDON *)world_manager::GetDataMan().get_data_ptr(get_addon, ID_SPACE_ADDON, dt3);
				if (addon == NULL || dt3 != DT_EQUIPMENT_ADDON)
					return;

				addon_data buff_data;
				if (!world_manager::GetDataMan().generate_addon(addon->id, buff_data))
					return;

				buff_data.arg[1] = (int)_crystal.stars[_crystal.crystal_color];				
				_new_total_addon.push_back(buff_data);
			}
		}
		
		OnRefreshItem();
		return;
	}

	void SaveAddOn(archive & ar)
	{
		if(GetItemType () != ITEM_TYPE_NEW_ARMOR)
		{
			unsigned int count;
			count = _total_addon.size();
			ar << count;
			for (unsigned int i = 0; i < count; i++)
			{
				int id = _total_addon[i].id;
				int argcount = addon_manager::GetArgCount(id);
				ar << id;
				for (int j = 0; j < argcount; j++)
				{
					ar << _total_addon[i].arg[j];
				}
			}
		} else 
		{
			unsigned int count;
			count = _total_addon.size();
			ar << count;
			for (unsigned int i = 0; i < count; i++)
			{
				int id = _total_addon[i].id;
				int argcount = addon_manager::GetArgCount(id);
				ar << id;
				for (int j = 0; j < argcount; j++)
				{
					ar << _total_addon[i].arg[j];
				}
			}

			// Valores Fixos
			ar << _spirit.spirit_inject;
			ar << _spirit.crystal_inject;


			// Se o Espírito for Injetado
			if(_spirit.spirit_inject)
			{
				ar << _spirit.spirit_id;
				ar << _spirit.reserved;
				ar << _spirit.spirit_level;
				ar << _spirit.lock_unlock;

				ar << _spirit.addons_count;
				for(int i = 0; i < _spirit.addons_count && i < 5; i++)
				{
					ar << _spirit.addons[i].id_addon;
					ar << _spirit.addons[i].arg;
				}
			}


			// Se o cristal for injetado
			if(_spirit.crystal_inject)
			{
				ar << _crystal.crystal_id;
				ar << _crystal.crystal_level;
				ar << _crystal.crystal_color;
				
				ar << _crystal.exp;

				ar << _crystal.lock_unlock;

				for(unsigned int i = 0; i < 5; i++)
				{
					ar << _crystal.stars[i];
				}

				for (unsigned int i = 0; i < 15; i++)
				{
					ar << _crystal.res[i];
				}

				ar << _crystal.addons_count;
				for(unsigned int i = 0; i < _crystal.addons_count; i++)
				{
					ar << _crystal.addons[i].id;
					ar << _crystal.addons[i].arg;
				}

				unsigned int reserved = 0;
				ar << reserved;			
			}

		}
	}

	virtual bool IsItemCanUse(item::LOCATION l)
	{
		return 	_use_addon.id != -1;
	}
	virtual void ClearData()
	{
		_use_addon.id = -1;
		_addon_expire_date = 0;
	}

	
};


struct weapon_essence
{
	enum
	{
		WEAPON_TYPE_MELEE = 0,
		WEAPON_TYPE_RANGE = 1,
		WEAPON_TYPE_MELEE_ASN = 2,
	};

	short weapon_type;		
	short weapon_delay;		
	int weapon_class;		
	int weapon_level;		
	int require_projectile;		
	int damage_low;			
	int damage_high;		
	int magic_damage_low;		
	int magic_damage_high;		
	int attack_speed;		
	float attack_range;		
	float attack_short_range;

};

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const weapon_essence & es)
{
	wrapper.push_back(&es,sizeof(es));
	return wrapper;
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, weapon_essence & es)
{
	wrapper.pop_back(&es,sizeof(es));
	return wrapper;
}

struct projectile_essence 
{
	int projectile_type;		//��ҩ���� 
	int enhance_damage;		//��ǿ�����Ĺ����� 
	int scale_enhance_damage;	//���ձ�����ǿ�Ĺ����� 
	int weapon_level_low;
	int weapon_level_high;
};


template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const projectile_essence & es)
{
	return wrapper.push_back(&es,sizeof(es));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, projectile_essence & es)
{
	return wrapper.pop_back(&es,sizeof(es));
}

struct armor_essence
{
	int defense;
	int armor;
	int mp_enhance;
	int hp_enhance;
	int resistance[MAGIC_CLASS];
};

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const armor_essence & es)
{
	return wrapper.push_back(&es,sizeof(es));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, armor_essence & es)
{
	return wrapper.pop_back(&es,sizeof(es));
}


struct new_armor_essence
{
	int defense;
	int armor;
	int mp_enhance;
	int hp_enhance;
	int resistance[MAGIC_CLASS];
};

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const new_armor_essence & es)
{
	return wrapper.push_back(&es,sizeof(es));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, new_armor_essence & es)
{
	return wrapper.pop_back(&es,sizeof(es));
}

struct decoration_essence
{
	int damage;
	int magic_damage;
	int defense;
	int armor;
	int resistance[MAGIC_CLASS];
};

template <typename WRAPPER>
WRAPPER & operator<<(WRAPPER & wrapper, const decoration_essence & es)
{
	return wrapper.push_back(&es,sizeof(es));
}

template <typename WRAPPER>
WRAPPER & operator>>(WRAPPER & wrapper, decoration_essence & es)
{
	return wrapper.pop_back(&es,sizeof(es));
}

class equip_item : public addon_equip_item
{
public:
	struct  base_data
	{
		int damage_low;
		int damage_high;
		int magic_damage_low;
		int magic_damage_high;
		int defense;
		int armor;
	};

	struct scale_data
	{
		int damage;
		int magic_damage;
		int defense;
		int armor;
	};

	struct made_tag_t
	{
		char tag_type;
		unsigned char tag_size;
		char tag_content[MAX_USERNAME_LENGTH];
	};

protected:
	abase::octets _raw_data;	//����ԭʼ��������,��������º���Ҫ��������
	made_tag_t _m_tag;		//�����߱�ǩ
	unsigned short _crc;
	unsigned short _modify_mask;	//��λ��mask
public:
	struct __prerequisition : public prerequisition
	{
		template <typename WRAPPER>
			WRAPPER & operator <<(WRAPPER & wrapper)
			{
				return 	wrapper >> level >> race
						>> strength >> vitality
						>> agility  >> energy
						>> durability >> max_durability;
			}

		template <typename WRAPPER>
			WRAPPER & operator >>(WRAPPER & wrapper)
			{
				return 	wrapper << level << race
						<< strength << vitality
						<< agility  << energy
						<< durability << max_durability;
			}
	} _base_limit;


	struct base_data 	_base_param;
	struct scale_data 	_base_param_percent;
	

public:
	DECLARE_SUBSTANCE(equip_item);
	equip_item()
	{
		memset(&_base_limit,0,sizeof(_base_limit));
		memset(&_base_param,0,sizeof(_base_param));
		memset(&_base_param_percent,0,sizeof(_base_param_percent));
		_crc = 0;
	}
	virtual void ClearData()
	{
		addon_equip_item::ClearData();
		memset(&_base_param,0,sizeof(_base_param));
		memset(&_base_param_percent,0,sizeof(_base_param_percent));
	}
	~equip_item();

public:
	virtual void GetItemData(const void ** data, unsigned int &len)
	{
		*data = _raw_data.begin();
		len = _raw_data.size();
	}

	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);
	unsigned int LoadMadeTag(archive & ar);
	void SaveMadeTag(archive & ar,unsigned int ess_size);
protected:
	virtual void * GetEssence() = 0;
	virtual unsigned int GetEssenceSize() = 0;
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj);
	virtual bool LoadEssence(archive & ar) = 0;	//װ�ر�������
	virtual bool SaveEssence(archive & ar) = 0;	//װ�ر�������
	virtual void UpdateEssence() = 0;		//���»������Ե�ͨ��������
	virtual void UpdateData() = 0;			//���ݸ������Ժ���ǿ���Զ����ݽ������ĵ���
	virtual void SetSocketCount(unsigned int count) {}
	virtual void SetSocketType(unsigned int index, int type) { ASSERT(false);}
	virtual unsigned int GetSocketCount() { return 0;}
	virtual int GetSocketType(unsigned int index) { return 0;}
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj) = 0;
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj) = 0;
	virtual bool ArmorDecDurability(int amount) 
	{ 
		ASSERT(CheckRawDurability());

		_base_limit.durability -= amount;
		bool bRst = false;
		if(_base_limit.durability < 0)
		{
			_base_limit.durability = 0;
			bRst = true;
		}

		UpdateRawDurability();
		return bRst;
	}

	virtual void GetDurability(int &dura,int &max_dura) 
	{ 
		dura = _base_limit.durability; 
		max_dura = _base_limit.max_durability;
	}
	virtual void Repair()
	{
		_base_limit.durability = _base_limit.max_durability;
		UpdateRawDurability();
	}

	virtual unsigned short GetDataCRC() { return _crc; }
	virtual bool RegenAddon(int item_id,bool (*regen_addon)(int item_id, addon_data& ent));
	virtual int RefineAddon(int addon_id, int & level_result, float adjust[4], float adjust2[12]);
	virtual int GetAddonExpireDate(){ return _addon_expire_date; }
	virtual int RemoveExpireAddon(int cur_t);	//���ظ��º��addon_expire_date
	virtual bool Sharpen(addon_data * addon_list, unsigned int count, int sharpener_gfx);
	virtual bool Engrave(addon_data * addon_list, unsigned int count);
	virtual unsigned int GetEngraveAddon(addon_data * addon_list, unsigned int max_count);
    virtual bool InheritAddon(addon_data * addon_list, unsigned int count);
    virtual unsigned int GetCanInheritAddon(addon_data * addon_list, unsigned int max_count, int ex_addon_id);
	virtual int RegenInherentAddon();
	virtual int GetRefineLevel(int addon_id);
	virtual int SetRefineLevel(int addon_id , int level);
	virtual void OnRefreshItem();
	virtual bool Sign(unsigned short color, const char * signature, unsigned int signature_len);

	//G17

	virtual void LockUnlockSpirit(bool value) {  return; }
	virtual char GetLockUnlockSpirit() {  return (char)_spirit.lock_unlock; }
	
	inline virtual char GetLuarSpiritLevel() { return (char)_spirit.spirit_level; }

	inline virtual void SetLuarSpiritLevel(char value)
	{
		_spirit.spirit_level = value;
		OnRefreshItem();
	}

	inline virtual void SetLuarSpiritid(int id)
	{
		_spirit.spirit_id = id;
		OnRefreshItem();
	}

	inline virtual int GetLuarSpiritid() { return _spirit.spirit_id; }

	virtual int GetAddonsSpiritCount() { return _spirit.addons_count; }
	virtual void SetAddonsSpiritCount(int value) 
	{ 
		_spirit.addons_count = value;
		OnRefreshItem(); 
	}

	virtual int GetAddonsSpiritid(int index) { return _spirit.addons[index].id_addon; }
	virtual int GetAddonsSpiritarg(int index) { return _spirit.addons[index].arg; }

	virtual void SetAddonsSpirit(int index, int id, int arg)
	{
		_spirit.addons[index].id_addon = id;
		_spirit.addons[index].arg = arg;
		OnRefreshItem();
	}

	virtual void EnabledAddonsSpirit(bool value)
	{
		_spirit.spirit_inject = value;
		_spirit.reserved = (int)value;
		OnRefreshItem();
	}

	virtual void EnabledAddonsCrystal(bool value)
	{
		_spirit.crystal_inject = value;
		OnRefreshItem();
	}

	// Cristal

	virtual int GetCrystalId() { return _crystal.crystal_id; }
	virtual void SetCrystalId(int value) { _crystal.crystal_id = value; OnRefreshItem(); }

	virtual char GetCrystalLevel() { return _crystal.crystal_level; }
	virtual void SetCrystalLevel(char value) { _crystal.crystal_level = value; OnRefreshItem(); }

	virtual char GetCrystalColor() { return _crystal.crystal_color; }
	virtual void SetCrystalColor(char value) { _crystal.crystal_color = value; OnRefreshItem(); }

	virtual int GetCrystalExp() { return _crystal.exp; }
	virtual void SetCrystalExp(int value) { _crystal.exp = value; OnRefreshItem(); }

	virtual char GetCrystalLock() { return _crystal.lock_unlock; }
	virtual void SetCrystalLock(char value) { _crystal.lock_unlock = value; OnRefreshItem(); }

	virtual char GetCrystalStar(int index) { return _crystal.stars[index]; }
	virtual void SetCrystalStar(int index, char value) { _crystal.stars[index] = value; OnRefreshItem(); }
	
	virtual int GetAddonsCrystalCount() { return _crystal.addons_count; }
	virtual void SetAddonsCrystalCount(int value) { _crystal.addons_count = value; OnRefreshItem(); }

	virtual int GetAddonsCrystalid(int index) { return _crystal.addons[index].id; }
	virtual int GetAddonsCrystalarg(int index) { return _crystal.addons[index].arg; }

	virtual void SetAddonsCrystal(int index, int id, int arg)
	{
		_crystal.addons[index].id = id;
		_crystal.addons[index].arg = arg;
		OnRefreshItem();
	}


private:
	virtual void OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual void OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj);
	virtual int OnUse(item::LOCATION l,gactive_imp * obj,unsigned int count);
	virtual short GetIdModify() { return _modify_mask; }

public:

	void UpdateAddOn(const addon_data & data)
	{
		int mask = addon_manager::CheckAndUpdate(data,this);
		if(mask == addon_manager::ADDON_MASK_INVALID)
		{
			throw -120;
		}

		if(mask & addon_manager::ADDON_MASK_ACTIVATE)
		{
			_active_addon.push_back(data);
		}

		if(mask & addon_manager::ADDON_MASK_USE)
		{
			if(_use_addon.id == -1)
				_use_addon = data;
		}
		//���㸽�����ԵĹ���ʱ��	
		int tmp = addon_manager::GetExpireDate(data);
		if(tmp > 0)
		{
			if(_addon_expire_date == 0)
				_addon_expire_date = tmp;
			else if(_addon_expire_date > tmp)
				_addon_expire_date = tmp;				
		}
	}

	void NewUpdateAddOn(const addon_data & data)
	{
		int mask = addon_manager::CheckAndUpdate(data,this);
		if(mask & addon_manager::ADDON_MASK_ACTIVATE)
		{
			_new_active_addon.push_back(data);
		}
		
		
	}
	
protected:
	
	void NewUpdateAddOn()
	{
		unsigned int i;
		for(i = 0; i < _new_total_addon.size(); i ++)
		{
			NewUpdateAddOn(_new_total_addon[i]);
		}
	}

	void UpdateAddOn()
	{
		unsigned int i;
		for(i = 0; i < _total_addon.size(); i ++)
		{
			UpdateAddOn(_total_addon[i]);
		}
	}	
	
	void LoadLimit(archive &ar);
	
	void SaveLimit(archive & ar)
	{
		_base_limit >> ar;
	}
	
	void LoadSocketData(archive & ar)
	{
		unsigned short count;
		ar >> count >> _modify_mask;
		if(count > MAX_SOCKET_COUNT) throw -103;
		SetSocketCount(count);
		for(unsigned int i = 0; i < count; i++)
		{
			int type;
			ar >> type;
			SetSocketType(i,type);
		}
	}

	void SaveSocketData(archive & ar)
	{
		unsigned short count = GetSocketCount();
		ar << count << _modify_mask;
		for(unsigned int i = 0; i < count; i++)
		{
			ar << GetSocketType(i);
		}
	}
	
	bool CheckRawDurability()
	{
		return ((prerequisition*)(_raw_data.begin()))->durability== _base_limit.durability;
	}

	void UpdateRawDurability()
	{
		((prerequisition*)(_raw_data.begin()))->durability = _base_limit.durability;
	}

	bool CheckRawRace()
	{
		return ((prerequisition*)(_raw_data.begin()))->race == _base_limit.race;
	}

	void UpdateRawRace()
	{
		((prerequisition*)(_raw_data.begin()))->race = _base_limit.race;
	}

	void CalcCRC()
	{
		ASSERT(_raw_data.size() > sizeof(prerequisition));
		_crc = crc16( (unsigned char *)_raw_data.begin() + sizeof(prerequisition),_raw_data.size() - sizeof(prerequisition));
	}
	
};


/*
	struct
	{
		��������;
		short ����ṹ��С;
		����������  //����Ϊ�˺���ǰ����Ʒ����
		char ����ṹ[];
		int �ײ۵���Ŀ;
		int �ײ۵����ͱ�[];
		int ���Ա�
		char ���Ա�[];		//addon
	};

*/

/*
 *	����Ƕ������Ķ���
 */
class socket_item : public equip_item
{
protected:
	abase::vector<int> _socket_list;
protected:
	virtual bool OnHasSocket() { return !_socket_list.empty();}
	virtual bool OnInsertChip(int chip_type,addon_data * data, unsigned int count);
	virtual bool OnClearChips();
	virtual void SetSocketCount(unsigned int count); 
	virtual void SetSocketType(unsigned int index, int type);
	virtual unsigned int GetSocketCount(); 
	virtual int GetSocketType(unsigned int index);
	virtual void AfterChipChanged() = 0;
	virtual bool HasAddonAtSocket(unsigned char s_idx,int s_type) { return s_idx >= _socket_list.size() ? false : _socket_list[s_idx] == s_type;}
	
	virtual	bool RemoveAddon(unsigned char s_idx);
public:
	virtual bool ModifyAddonAtSocket(unsigned char s_idx,int stone_id);

};

class weapon_item : public socket_item
{
protected:
	weapon_essence _ess;
public:
	DECLARE_SUBSTANCE(weapon_item);

	static int GetWeaponType(const item_data * pData)
	{
		weapon_item * pTmp;
		const char * pos = ((char*)pData->item_content) + sizeof(pTmp->_base_limit);
		unsigned char namesize = *(pos + sizeof(short) + sizeof(char));
		ASSERT(namesize <=MAX_USERNAME_LENGTH);
		pos += sizeof(int) + offsetof(weapon_essence,weapon_type) + namesize;
		short type = *(const short*)pos;
		ASSERT(type == 0 || type == 1 || type == 2 || type == 3);
		return type;
	}
	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_WEAPON;}
	virtual int MakeSlot(gactive_imp*, int& count, unsigned int material_id = 0, int material_count = 0);
protected:
	virtual void * GetEssence()  {return &_ess;}
	virtual unsigned int GetEssenceSize() {return sizeof(_ess);}
	virtual bool LoadEssence(archive & ar);
	virtual bool SaveEssence(archive & ar);
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj);
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj);
	virtual void UpdateEssence();
	virtual void UpdateData();
	virtual int OnGetProjectileReqType() const ;
	virtual void OnAfterAttack(item_list & list, bool * pUpdate);
	virtual bool ArmorDecDurability(int amount) { ASSERT(false); return false;}
	virtual void AfterChipChanged(); 
	virtual void SetSocketAndStone(int count, int * stone_type);
	virtual int Is16Por9JWeapon();
public:
	virtual void ForceOnRefreshItem() { OnRefreshItem (); };

};

class armor_item : public socket_item
{
protected:
	armor_essence _ess;	
public:
	DECLARE_SUBSTANCE(armor_item);
	armor_item * Clone() const { return new armor_item(*this);}

	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_ARMOR;}
	virtual int MakeSlot(gactive_imp*, int& count, unsigned int material_id = 0, int material_count = 0);
protected:
	virtual void * GetEssence()  {return &_ess;}
	virtual unsigned int GetEssenceSize() {return sizeof(_ess);}
	virtual bool LoadEssence(archive & ar);
	virtual bool SaveEssence(archive & ar);
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj);
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj);
	virtual void UpdateEssence();
	virtual void UpdateData();
	virtual bool ArmorDecDurability(int amount) 
	{ 
		ASSERT(CheckRawDurability());

		_base_limit.durability -= amount;
		bool bRst = false;
		if(_base_limit.durability < 0)
		{
			_base_limit.durability = 0;
			bRst = true;
		}
		UpdateRawDurability();
		return bRst;
	}
	virtual void AfterChipChanged(); 
	virtual void SetSocketAndStone(int count, int * stone_type);
};

class new_armor_item : public socket_item
{
protected:
	new_armor_essence _ess;	

public:
	DECLARE_SUBSTANCE(new_armor_item);
	new_armor_item * Clone() const { return new new_armor_item(*this);}

	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_NEW_ARMOR;}
	virtual int MakeSlot(gactive_imp*, int& count, unsigned int material_id = 0, int material_count = 0);
protected:
	virtual void * GetEssence() {return &_ess;}
	virtual unsigned int GetEssenceSize() {return sizeof(_ess);}
	virtual bool LoadEssence(archive & ar);
	virtual bool SaveEssence(archive & ar);
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj);
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj);
	virtual void UpdateEssence();
	virtual void UpdateData();
	virtual bool ArmorDecDurability(int amount) 
	{ 
		ASSERT(CheckRawDurability());

		_base_limit.durability -= amount;
		bool bRst = false;
		if(_base_limit.durability < 0)
		{
			_base_limit.durability = 0;
			bRst = true;
		}
		UpdateRawDurability();
		return bRst;
	}

	virtual void AfterChipChanged(); 
	virtual void SetSocketAndStone(int count, int * stone_type);
};


class melee_weapon_item: public weapon_item
{
public:
	DECLARE_SUBSTANCE(melee_weapon_item);
	virtual melee_weapon_item * Clone() const { return new melee_weapon_item(*this);}
private:
	virtual bool OnCheckAttack(item_list & list);
};

class range_weapon_item: public weapon_item
{
public:
	DECLARE_SUBSTANCE(range_weapon_item);
	virtual range_weapon_item * Clone() const { return new range_weapon_item(*this);}
private:
	virtual bool OnCheckAttack(item_list & list);
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj);
};

class projectile_equip_item : public socket_item
{
	projectile_essence _ess;

public:
	DECLARE_SUBSTANCE(projectile_equip_item);
	virtual projectile_equip_item * Clone() const { return new projectile_equip_item(*this);}
	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_PROJECTILE;}
protected:
	virtual void * GetEssence()  {return &_ess;}
	virtual unsigned int GetEssenceSize() {return sizeof(_ess);}
	virtual bool LoadEssence(archive & ar);
	virtual bool SaveEssence(archive & ar);
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj);
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj);
	virtual void UpdateEssence();
	virtual void UpdateData();
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj);
	virtual int OnGetProjectileType() const;
	virtual void AfterChipChanged() {}
};

class decoration_equip_item : public socket_item
{
	decoration_essence _ess;
public:
	DECLARE_SUBSTANCE(decoration_equip_item);
	virtual decoration_equip_item * Clone()  const { return new decoration_equip_item(*this);}
	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_DECORATION;}
    virtual int MakeSlot(gactive_imp*, int& count, unsigned int material_id = 0, int material_count = 0);
protected:
	virtual void * GetEssence()  {return &_ess;}
	virtual unsigned int GetEssenceSize() {return sizeof(_ess);}
	virtual bool LoadEssence(archive & ar);
	virtual bool SaveEssence(archive & ar);
	virtual void EssenceActivate(item::LOCATION l,gactive_imp* obj);
	virtual void EssenceDeactivate(item::LOCATION l,gactive_imp* obj);
	virtual void UpdateEssence();
	virtual void UpdateData();
    virtual void AfterChipChanged() { /* �޹�ЧЧ�� */ };
    virtual void SetSocketAndStone(int count, int* stone_type);
};

struct refine_param_t
{
	int need_level;
	float prop[4];	//�ֱ��Ӧ �ɹ� REFINE_FAILED_LEVEL_0 REFINE_FAILED_LEVEL_1 REFINE_FAILED_LEVEL_2
};

extern refine_param_t refine_table[];
extern float refine_factor[];


#endif

