#include "filter_kidform.h"
#include "skillwrapper.h"
#include "obj_interface.h"
#include "clstab.h"

void GNET::SkillWrapper::SetKidFilter(object_interface player, int *buf)
{
	filter_Kidform *f = new filter_Kidform(player, buf);
	player.AddFilter(f);
}

DEFINE_SUBSTANCE(filter_Kidform, filter, CLS_FILTER_KIDFORM)

filter_Kidform::filter_Kidform(object_interface object, int *buf)
	: timeout_filter(object, 30, MASK)
{
	_filter_id           = FILTER_ID;
	_shape               = buf[0];
	_attack_type         = buf[1];
	_hp                  = buf[2];
	_damage_low          = buf[3];
	_damage_high         = buf[4];
	_damage_magic_low    = buf[5];
	_damage_magic_high   = buf[6];
	_defence             = buf[7];
	_resistance[0]       = buf[8];
	_resistance[1]       = buf[9];
	_resistance[2]       = buf[10];
	_resistance[3]       = buf[11];
	_resistance[4]       = buf[12];
	_point               = buf[13];
	_ratio               = buf[14];
	_range               = *(float *)&buf[15];
	_speed               = *(float *)&buf[16];
	_attack_adj          = buf[17];
	_defend_adj          = buf[18];
	_attack_ant          = buf[19];
	_defend_ant          = buf[20];
	_time_reduce         = buf[21];
	_skill_count         = buf[22];
	if (_skill_count > MAX_SKILL_COUNT)
		_skill_count = MAX_SKILL_COUNT;
	memcpy(_skill, buf + 23, sizeof(int) * 2 * _skill_count);
}

void filter_Kidform::OnAttach()
{
	int form = _parent.GetForm();
	gactive_imp *imp = _parent.GetImpl();
	GNET::SkillWrapper *sw = &_parent.GetSkillWrapper();
	sw->EventChange(object_interface(imp), form, 3);

	_parent.LockEquipment(true);
	_parent.SetNoMount(true);
	_parent.SetNoBind(true);

	int shape = _shape | 0xC0;
	_parent.ChangeShape2(shape, 30);

	_parent.EnhanceMaxHP(_hp);
	_parent.EnhanceDamage2(_damage_low, _damage_high);
	_parent.EnhanceMagicDamage2(_damage_magic_low, _damage_magic_high);
	_parent.EnhanceDefense(_defence);
	_parent.EnhanceResistance(0, _resistance[0]);
	_parent.EnhanceResistance(1, _resistance[1]);
	_parent.EnhanceResistance(2, _resistance[2]);
	_parent.EnhanceResistance(3, _resistance[3]);
	_parent.EnhanceResistance(4, _resistance[4]);
	_parent.EnhanceCrit(_point);
	_parent.EnhanceAttackSpeed(_ratio);
	_parent.EnhanceAttackRange(_range);
	_parent.EnhanceSpeed0(_speed);
	_parent.EnhanceAttackDegree(_attack_adj);
	_parent.EnhanceDefendDegree(_defend_adj);
	_parent.IncAntiDefenseDegree(_attack_ant);
	_parent.IncAntiResistanceDegree(_defend_ant);

	sw = &_parent.GetSkillWrapper();
	sw->DecPrayTime(_time_reduce);

	_parent.IncImmuneMask(50339840);

	for (int i = 0; i < _skill_count; ++i)
	{
		sw = &_parent.GetSkillWrapper();
		sw->ActivateDynSkill(_skill[2 * i], 1, _parent, _skill[2 * i + 1]);
	}

	_parent.SendClientAttackData();
	_parent.UpdateDefenseData();
	_parent.UpdateMagicData();
	_parent.UpdateAttackData();
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();
}

void filter_Kidform::OnRelease()
{
	int form = _parent.GetForm();
	gactive_imp *imp = _parent.GetImpl();
	GNET::SkillWrapper *sw = &_parent.GetSkillWrapper();
	sw->EventChange(object_interface(imp), form, 0);

	_parent.LockEquipment(false);
	_parent.SetNoMount(false);
	_parent.SetNoBind(false);
	_parent.ChangeShape2(0, 0);

	// save HP ratio before restoring max HP
	float hp_ratio = (float)_parent.GetBasicProp().hp
	               / (float)_parent.GetExtendProp().max_hp;

	_parent.ImpairMaxHP(_hp);
	_parent.ImpairDefense(_defence);
	_parent.ImpairResistance(0, _resistance[0]);
	_parent.ImpairResistance(1, _resistance[1]);
	_parent.ImpairResistance(2, _resistance[2]);
	_parent.ImpairResistance(3, _resistance[3]);
	_parent.ImpairResistance(4, _resistance[4]);
	_parent.ImpairDamage2(_damage_low, _damage_high);
	_parent.ImpairMagicDamage2(_damage_magic_low, _damage_magic_high);
	_parent.ImpairCrit(_point);
	_parent.ImpairAttackSpeed(_ratio);
	_parent.ImpairAttackRange(_range);
	_parent.ImpairSpeed0(_speed);
	_parent.ImpairAttackDegree(_attack_adj);
	_parent.ImpairDefendDegree(_defend_adj);
	_parent.DecAntiDefenseDegree(_attack_ant);
	_parent.DecAntiResistanceDegree(_defend_ant);

	sw = &_parent.GetSkillWrapper();
	sw->IncPrayTime(_time_reduce);

	_parent.DecImmuneMask(50339840);

	for (int i = 0; i < _skill_count; ++i)
	{
		sw = &_parent.GetSkillWrapper();
		sw->DeactivateDynSkill(_skill[2 * i], 1, _parent, _skill[2 * i + 1]);
	}

	_parent.SendClientAttackData();
	_parent.UpdateDefenseData();
	_parent.UpdateMagicData();
	_parent.UpdateAttackData();
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();

	sw = &_parent.GetSkillWrapper();
	sw->KidTransformAddBuffs(_parent);

	// restore HP proportionally to the new max_hp
	float target_hp = (float)_parent.GetExtendProp().max_hp * hp_ratio;
	int diff = _parent.GetBasicProp().hp - (int)target_hp;
	if (diff > 0)
		_parent.DecHP(diff);
	else if (diff < 0)
		_parent.Heal(-diff);

	_parent.KidTransformEnd();
}

void filter_Kidform::TranslateSendAttack(const XID &target, attack_msg &msg)
{
	if (!msg.skill_id)
		msg.attack_attr = _attack_type;
}

bool filter_Kidform::Save(archive &ar)
{
	timeout_filter::Save(ar);
	ar << _shape << _attack_type << _hp
	   << _damage_low << _damage_high
	   << _damage_magic_low << _damage_magic_high
	   << _defence;
	ar.push_back(_resistance, sizeof(_resistance));
	ar << _point << _ratio << _range << _speed
	   << _attack_adj << _defend_adj
	   << _attack_ant << _defend_ant
	   << _time_reduce << _skill_count;
	ar.push_back(_skill, sizeof(_skill));
	return true;
}

bool filter_Kidform::Load(archive &ar)
{
	timeout_filter::Load(ar);
	ar >> _shape >> _attack_type >> _hp
	   >> _damage_low >> _damage_high
	   >> _damage_magic_low >> _damage_magic_high
	   >> _defence;
	ar.pop_back(_resistance, sizeof(_resistance));
	ar >> _point >> _ratio >> _range >> _speed
	   >> _attack_adj >> _defend_adj
	   >> _attack_ant >> _defend_ant
	   >> _time_reduce >> _skill_count;
	ar.pop_back(_skill, sizeof(_skill));
	return true;
}
