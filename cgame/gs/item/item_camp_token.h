#ifndef __ONLINEGAME_GS_ITEM_CAMP_TOKEN_H__
#define __ONLINEGAME_GS_ITEM_CAMP_TOKEN_H__

#include <stddef.h>
#include <octets.h>
#include <common/packetwrapper.h>
#include "../item.h"
#include "../config.h"
#include "item_addon.h"
#include <crc.h>

enum 
{
	MAX_CAMP_TOKEN_LEVEL = 100,
	MAX_ADDONS_STORAGE = 512,
};

#pragma pack(push,1)
struct camp_token_ess
{
	short level;
	short race;
	short strength;
	short vitality;
	short agility;
	short energy;
	int durability;
	int max_durability;

	int size_camptoken;	
	char level_camp;
	unsigned int exp;
};
#pragma pack(pop)

struct camp_token_vector
{
	char pos_camp_token[MAX_CAMP_TOKEN_LEVEL];
	char pos_camp_addon[MAX_CAMP_TOKEN_LEVEL];
	char pos_camp_reserve[MAX_CAMP_TOKEN_LEVEL];

	float addon_ratio_pos[MAX_ADDONS_STORAGE];
};

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & wrapper, const camp_token_ess & ess)
{
	return wrapper.push_back(&ess,sizeof(ess));
}

template <typename WRAPPER> WRAPPER & operator >>(WRAPPER & wrapper, camp_token_ess & ess)
{
	return wrapper.pop_back(&ess,sizeof(ess));
}

template <typename WRAPPER> WRAPPER & operator <<(WRAPPER & wrapper, const camp_token_vector & newvector)
{
	return wrapper.push_back(&newvector,sizeof(newvector));
}

template <typename WRAPPER> WRAPPER & operator >>(WRAPPER & wrapper, camp_token_vector & newvector)
{
	return wrapper.pop_back(&newvector,sizeof(newvector));
}

class camp_token_item : public item_body
{
protected:
	ADDON_LIST _total_addon;
	camp_token_ess _ess;
	abase::octets _raw_data;
	camp_token_vector _vector;

public:
	DECLARE_SUBSTANCE(camp_token_item);
	
	camp_token_item()
	{
		_crc = 0; 		
		memset(&_ess, 0, sizeof(_ess));
		memset(&_vector, 0, sizeof(_vector));
	}
	virtual void GetItemData(const void ** data, unsigned int &len)
	{
		*data = _raw_data.begin();
		len = _raw_data.size();
	}
	virtual bool Save(archive & ar);
	virtual bool Load(archive & ar);

	virtual bool ArmorDecDurability(int) { return false;}
	virtual void OnPutIn(item::LOCATION l,item_list & list,unsigned int pos, unsigned int count, gactive_imp* obj)
	{
		if(l == item::BODY)
		{
			Activate(l,list, pos, count,obj);
		}
	}
	virtual void OnTakeOut(item::LOCATION l,unsigned int pos, unsigned int count, gactive_imp* obj)
	{
		if(l == item::BODY)
		{
			Deactivate(l,pos,count,obj);
		}
	}

	virtual ITEM_TYPE GetItemType()  { return ITEM_TYPE_CAMP_TOKEN;}
	item_body * Clone() const { return new camp_token_item(*this);}
	virtual void GetDurability(int &dura,int &max_dura) { dura = _ess.durability; max_dura = _ess.max_durability; }
	virtual bool VerifyRequirement(item_list & list,gactive_imp* obj);

	virtual void OnActivate(item::LOCATION l,unsigned int pos,unsigned int count, gactive_imp* obj);
	virtual void OnDeactivate(item::LOCATION l,unsigned int pos,unsigned int count,gactive_imp* obj);

	inline void OnRefreshItem()
	{
		_raw_data.clear();
		raw_wrapper rw;
		Save(rw);
		rw.swap(_raw_data);
		CalcCRC();

		GenerateAddons();
	}

	inline void LoadVectorCampToken(archive & ar)
	{
		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar >> _vector.pos_camp_token[i];					
		}

		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar >> _vector.pos_camp_addon[i];					
		}

		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar >> _vector.pos_camp_reserve[i];					
		}
	}

	inline void SaveVectorCampToken(archive & ar)
	{
		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar << _vector.pos_camp_token[i];					
		}

		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar << _vector.pos_camp_addon[i];					
		}

		for (int i = 0; i < _ess.level_camp && i < MAX_CAMP_TOKEN_LEVEL; i++)
		{
			ar << _vector.pos_camp_reserve[i];					
		}
	}

	void GenerateAddons();
	float GetAddonRatio(int index);

protected:
	unsigned short _crc;
	void CalcCRC()
	{   
		_crc = crc16( (unsigned char *)_raw_data.begin(),_raw_data.size());
	}
	virtual unsigned short GetDataCRC() { return _crc; }

public:
	virtual bool AddCampTokenExp(unsigned int exp);
	virtual bool GenerateNewPoint(unsigned int pos_line_key, int item_id_key_bonus, int parent_key_bonus);
	virtual bool ActivateAddonLineKey(unsigned int pos_line_key, int addon_line_key_pos);
	virtual bool ResetAddonLineKey(unsigned int pos_line_key, int return_count);

};

#endif
