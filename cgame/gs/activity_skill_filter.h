#ifndef __ONLINE_GAME_GS_ACTIVITY_SKILL_FILTER_H__
#define __ONLINE_GAME_GS_ACTIVITY_SKILL_FILTER_H__

#include "filter.h"

class activity_skill_filter : public filter
{
	int _counter;
public:
	DECLARE_SUBSTANCE(activity_skill_filter);
	activity_skill_filter() {}
	activity_skill_filter(gactive_imp * imp, int filter_id)
		:filter(object_interface(imp),
		FILTER_MASK_HEARTBEAT|
		FILTER_MASK_WEAK)
	{
		_filter_id = filter_id;
		_counter  = 0;
	}

protected:
	virtual void OnAttach();
	virtual void OnRelease(); 
	virtual bool Save(archive & ar)
	{
		filter::Save(ar);
		ar << _counter;
		return true;
	}

	virtual bool Load(archive & ar)
	{
		filter::Load(ar);
		ar >> _counter;
		return true;
	}

	
};

#endif

