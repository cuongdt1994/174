#include "pet_filter.h"
#include "sfilterdef.h"
#include "clstab.h"

DEFINE_SUBSTANCE(pet_damage_filter,filter,CLS_FILTER_PET_DAMAGE)

const int __pet_damage_adjust[] = { -40, -20, 0, 20 , 0,0,0,0,0,0,0};

void
pet_damage_filter::OnAttach()
{
	if(_honor_level >= 4) _honor_level = 5;	//使用0伤害的数据
	_parent.EnhanceScaleMagicDamage(__pet_damage_adjust[_honor_level]);
	_parent.EnhanceScaleDamage(__pet_damage_adjust[_honor_level]);
	_parent.UpdateAttackData();
}

void 
pet_damage_filter::OnRelease()
{
	_parent.ImpairScaleMagicDamage(__pet_damage_adjust[_honor_level]);
	_parent.ImpairScaleDamage(__pet_damage_adjust[_honor_level]);
	_parent.UpdateAttackData();
}

void  
pet_damage_filter::OnModify(int ctrlname,void * ctrlval,unsigned int ctrllen)
{
	if(ctrlname == FMID_PET_HONOR && ctrllen == sizeof(int))
	{
		int new_hl = *(int*)ctrlval;
		if((unsigned int)new_hl != _honor_level)
		{	
			_parent.ImpairScaleMagicDamage(__pet_damage_adjust[_honor_level]);
			_parent.ImpairScaleDamage(__pet_damage_adjust[_honor_level]);

			_honor_level = (unsigned int)new_hl;
			if(_honor_level >= 4) _honor_level = 5;
			_parent.EnhanceScaleMagicDamage(__pet_damage_adjust[_honor_level]);
			_parent.EnhanceScaleDamage(__pet_damage_adjust[_honor_level]);
			_parent.UpdateAttackData();
		}
		
	}
}


