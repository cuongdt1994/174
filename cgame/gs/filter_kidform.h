#ifndef __ONLINEGAME_GS_FILTER_KIDFORM_H__
#define __ONLINEGAME_GS_FILTER_KIDFORM_H__

#include "filter.h"

class filter_Kidform : public timeout_filter
{
	int _shape;
	int _attack_type;
	int _hp;
	int _damage_low;
	int _damage_high;
	int _damage_magic_low;
	int _damage_magic_high;
	int _defence;
	int _resistance[5];
	int _point;
	int _ratio;
	float _range;
	float _speed;
	int _attack_adj;
	int _defend_adj;
	int _attack_ant;
	int _defend_ant;
	int _time_reduce;
	int _skill_count;
	int _skill[32]; // 16 skills * 2 ints (skill_id, counter)

	enum
	{
		FILTER_ID       = 4722,
		MAX_SKILL_COUNT = 16,
		MASK = FILTER_MASK_TRANSLATE_SEND_MSG | FILTER_MASK_HEARTBEAT |
		       FILTER_MASK_UNIQUE | FILTER_MASK_REMOVE_ON_DEATH | FILTER_MASK_NOSAVE,
	};

public:
	DECLARE_SUBSTANCE(filter_Kidform);
	filter_Kidform(object_interface object, int *buf);

protected:
	virtual void OnAttach();
	virtual void OnRelease();
	virtual void TranslateSendAttack(const XID &target, attack_msg &msg);
	virtual bool Save(archive &ar);
	virtual bool Load(archive &ar);

	filter_Kidform() {}
};

#endif // __ONLINEGAME_GS_FILTER_KIDFORM_H__
