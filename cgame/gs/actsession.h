#ifndef __ONLINEGAME_GS_ACT_SESSION_H__
#define __ONLINEGAME_GS_ACT_SESSION_H__

#include <common/types.h>
#include <amemobj.h>
#include <timer.h>
#include "config.h"
#include "substance.h"
#include "pathfinding/chaseinfo.h"
#include "item/item_addon.h"

typedef CChaseInfo chase_info;
class	dispatcher;
class	controller;
class 	gactive_imp;
class	world;
//Tentative name This is the name of the current session of the object
//The operation queue can also exist directly as a session
class act_session : public substance
{
protected:
	act_session(): _imp(0), _session_id(-3){}
public:
	gactive_imp * _imp;
	int	_session_id;
	world	* _plane;
public:
	DECLARE_SUBSTANCE(act_session);
	explicit act_session(gactive_imp * imp);
	virtual void Restore(gactive_imp * imp,int session_id = -1);

	void NPCSessionStart(int task_id);
	void NPCSessionEnd(int task_id, int retcode);
	void NPCSessionUpdateChaseInfo(int task_id, const chase_info & info);

	virtual ~act_session(){}
	virtual bool StartSession(bool hasmorecmd = false) = 0; 	//session officially started
	virtual bool EndSession() = 0;		//session completed
	virtual bool RepeatSession() = 0;	//Repeat session
	virtual bool TerminateSession(bool force = true) = 0;	//session aborted
	virtual bool IsTimeSpent() = 0;		//Does this session take time to execute?
	virtual int  GetMask() = 0;		//Obtain your own mask to show your status in the team
	virtual int  GetExclusiveMask() = 0;	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	virtual bool OnAttacked() { return false;}
	virtual bool RestartSession()  { return true;}
	virtual bool IsMoveSession() { return false; }		//Whether to move the session
	virtual bool IsAttackSession() { return false; }	//Is the common attack or skill session

public:
	void SendMsg(int msg,const XID & target,const XID & source);
	void SendRepeatMsg(const XID & self);
	void SendForceRepeat(const XID & self);
	void SendEndMsg(const XID & self);
	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}

	enum{
/*
	Organized all session masks by liuguichen
	SITDOWN Meditates, but also includes situations that require the player to stand still.
	USE_SERVICE uses services, including npc services or some other similar services. The new one is logically no different from SITDOWN, but the meaning is clearer
*/
		SS_MASK_MOVE 	= 0x01,
		SS_MASK_ATTACK 	= 0x02,
		SS_MASK_SITDOWN	= 0x04,
		SS_MASK_USE_SERVICE = 0x08,
		SS_MASK_USE_ITEM = 0x10,
	};
	
};

class session_empty : public act_session
{
public:
	DECLARE_SUBSTANCE(session_empty);
	explicit session_empty():act_session(NULL){}
	virtual bool StartSession(bool hasmorecmd=false) { return false; }
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	//Obtain your own mask to show your status in the team
	virtual int  GetMask() { return 0; }
	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	virtual int  GetExclusiveMask()	{ return 0; }

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class act_timer_session : public act_session, public abase::timer_task
{
protected:
	XID _self_id;
	act_timer_session(){}
public:
	explicit act_timer_session(gactive_imp * imp):act_session(imp){}
	virtual ~act_timer_session()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	virtual bool StartSession(bool hasmorecmd=false) = 0; //session officially started
	virtual bool EndSession()
	{
		RemoveTimer();
		return true;
	}
	virtual bool RepeatSession() = 0;	//Repeat session
	virtual bool TerminateSession(bool force)
	{
		//The default is no unforced interruption
		if(!force) return false;
		EndSession();
		return true;
	}
	virtual bool IsTimeSpent() { return true; }

	bool Save(archive & ar);
	bool Load(archive & ar);

	void OnTimer(int index,int rtimes)
	{
			if(rtimes)
				SendRepeatMsg(_self_id);
			else
				SendEndMsg(_self_id);	

	}

};

class session_move:  public act_timer_session
{
protected:
	session_move(){}
protected:
	A3DVECTOR _target;
	A3DVECTOR _predict;
	int	_use_time;
	int	_seq;
	unsigned short _speed;
	unsigned char _move_mode;
	template <int>
	bool CheckCmdSeq()
	{
		int seq = _imp->_commander->GetCurMoveSeq();
		int seq_offset = (_seq - seq) & 0xFFFF;
		if(_seq != -1 && seq_offset > 2000)
		{
			//ignore this session
			return false;
		}

		if(_seq != -1  && seq_offset)
		{
			//Should a log be recorded
			_imp->_commander->SetNextMoveSeq(_seq);
		}
		else
		{
			//command number one
			_imp->_commander->GetNextMoveSeq();
		}
		return true;
	}
public:
	DECLARE_SUBSTANCE(session_move);
	explicit session_move(gactive_imp * imp):act_timer_session(imp),_seq(-1),_speed(0),_move_mode(1){}
	~session_move()
	{
		if(_timer_index >=0) RemoveTimer();
	}
	void SetPredictPos(const A3DVECTOR &pos)
	{
		_predict = pos;
	}
	void SetDestination(unsigned short speed,unsigned char move_mode, 
				const A3DVECTOR & target,int use_time)
	{	
		_speed = speed;
		_move_mode = move_mode;
		_target = target;
		_use_time = use_time;
	}

	void SetCmdSeq(int seq)
	{
		_seq = seq;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool IsMoveSession() { return true; }

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _target << _predict <<_use_time 
		   << _speed << _move_mode;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _target >> _predict >>_use_time 
		   >> _speed >> _move_mode;
		return true;
	}
};

class session_stop_move : public session_move
{
	unsigned short _dir;
protected:
	session_stop_move(){}
public:
	DECLARE_SUBSTANCE(session_stop_move);
	explicit session_stop_move(gactive_imp * imp):session_move(imp),_dir(0){}
	void SetDir(unsigned short dir)
	{
		_dir = dir;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	bool Save(archive & ar) 
	{
		session_move::Save(ar);
		ar << _dir;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_move::Load(ar);
		ar >> _dir;
		return true;
	}

};

class session_normal_attack: public act_timer_session
{
protected:
	session_normal_attack(){}
protected:
	XID _target;
	char _stop_flag;
	char _force_attack;
	unsigned char _attack_speed;
public:
	DECLARE_SUBSTANCE(session_normal_attack);
	explicit session_normal_attack(gactive_imp * imp)
				:act_timer_session(imp),_target(-1,-1),_stop_flag(0)
	{}
	~session_normal_attack()
	{
		if(_timer_index >=0) RemoveTimer();
	}

	void SetTarget(const XID & target,char force_attack)
	{
		_target = target;
		_force_attack = force_attack;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool IsAttackSession() { return true; }
	
	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_ATTACK;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE);
	}
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _target << _stop_flag <<_force_attack << _attack_speed;

		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _target >> _stop_flag >> _force_attack >> _attack_speed;
		return true;
	}
};


/*no use, watch out by liuguichen
class session_npc_zombie : public act_timer_session
{
	int _delay_time;
protected:
	session_npc_zombie(){}
public:
	DECLARE_SUBSTANCE(session_npc_zombie);
	explicit session_npc_zombie(gactive_imp * imp)
				:act_timer_session(imp),_delay_time(500)
	{}

	~session_npc_zombie()
	{
		if(_timer_index >=0) RemoveTimer();
	}

	void SetDelay(int delay)
	{
		_delay_time = delay;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual void OnTimer(int index,int rtimes);

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return 0;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~0;
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _delay_time;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _delay_time;
		return true;
	}
};
*/

class session_skill : public act_timer_session
{
protected:
	session_skill():_data(0){}
	SKILL::Data _data;
	int  _next_interval;
	bool _end_flag;
	char _force_attack;
	int  _skill_skip_time;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
public:
	DECLARE_SUBSTANCE(session_skill);
	explicit session_skill(gactive_imp * imp)
				:act_timer_session(imp),_data(0),
				_next_interval(20),_end_flag(false),_force_attack(0), _skill_skip_time(0)
	{
	}
	void SetTarget(int skill_id, char force_attack,int target_num,int * targets);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RestartSession();
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);
	virtual bool IsAttackSession() { return true; }

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_ATTACK;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE);
	}

	bool OnAttacked();		// return true to interrupt
	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _next_interval
		   << _end_flag << _force_attack << _skill_skip_time;
		unsigned int size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _next_interval
		   >> _end_flag >> _force_attack >> _skill_skip_time;
		unsigned int size;
		ar >> size;
		XID id;
		for(unsigned int i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

struct recipe_template;
struct decompose_recipe_template;
class gplayer_imp;
class session_produce : public act_timer_session
{
protected:
	session_produce(){}
	const recipe_template * _rt;
	unsigned int _count;

public:
	DECLARE_SUBSTANCE(session_produce);
	explicit session_produce(gplayer_imp * imp,const recipe_template * rt, unsigned int count)
				:act_timer_session((gactive_imp*)imp),_rt(rt),_count(count)
	{
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_produce2 : public act_timer_session
{
protected:
	session_produce2(){}
	const recipe_template * _rt;
	int _materials[16];
	int _idxs[16];

public:
	DECLARE_SUBSTANCE(session_produce2);
	explicit session_produce2(gplayer_imp * imp,const recipe_template * rt, int materials[16], int idxs[16])
				:act_timer_session((gactive_imp*)imp),_rt(rt)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_idxs, idxs, sizeof(_idxs));
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_decompose : public act_timer_session
{
protected:
	session_decompose(){}
	int _index;
	const decompose_recipe_template * _rt;

public:
	DECLARE_SUBSTANCE(session_decompose); 
	session_decompose(gplayer_imp * imp,int index, const decompose_recipe_template * rt)
		:act_timer_session((gactive_imp*)imp), _index(index),_rt(rt)
	{
	}
	void OnTimer(int index,int rtimes);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_use_item : public  act_timer_session
{
protected:
	session_use_item(){}
	int _where;
	int _index;
	int _type;
	unsigned int _count;
	unsigned int _usetime;

public:
	DECLARE_SUBSTANCE(session_use_item); 
	session_use_item(gplayer_imp * imp,int where,int index,int type, unsigned int count,unsigned int usetime)
		:act_timer_session((gactive_imp*)imp), _where(where),_index(index),
		_type(type),_count(count),_usetime(usetime)
	{
	}
	void OnTimer(int index,int rtimes);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_ITEM;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_use_item_with_target : public  session_use_item
{
protected:
	session_use_item_with_target(){}
	XID _target;
	char _force_attack;

public:
	DECLARE_SUBSTANCE(session_use_item_with_target); 
	session_use_item_with_target(gplayer_imp * imp,int where,int index,int type, unsigned int count,unsigned int usetime)
		:session_use_item(imp,where,index,type,count,usetime),_target(-1,-1),_force_attack(0)
	{
	}

	virtual bool StartSession(bool hasmorecmd=false);
	void SetTarget(const XID & target, char force_attack)
	{
		_target = target;
		_force_attack = force_attack;
	}

	virtual bool RepeatSession();
	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_cancel_action: public act_session
{
public:
	DECLARE_SUBSTANCE(session_cancel_action);
	explicit session_cancel_action():act_session(NULL){}
	virtual bool StartSession(bool hasmorecmd=false) { return false; }
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	//Obtain your own mask to show your status in the team
	virtual int  GetMask() { return 0; }
	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE); }
};

class session_sit_down: public  act_session
{
protected:
	session_sit_down(){}
public:
	DECLARE_SUBSTANCE(session_sit_down); 
	session_sit_down(gplayer_imp * imp)
		:act_session((gactive_imp*)imp)
	{
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession(); 
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}

	virtual int  GetMask() { return SS_MASK_SITDOWN;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
};

class session_gather_prepare : public act_session
{
public:
	session_gather_prepare(){}
	int   _target;
	short _where;
	short _index;
	int _tool_type;
	int _task_id;
	int _soul_gather_num;
public:
	DECLARE_SUBSTANCE(session_gather_prepare);
	explicit session_gather_prepare(gplayer_imp * imp)
		:act_session((gactive_imp*)imp)
	{}

	void SetTarget(int target, short where, short index, int tool_type,int task_id, int soul_gather_num)
	{
		_target = target;
		_where = where;
		_index = index;
		_tool_type = tool_type;
		_task_id = task_id;
		_soul_gather_num = soul_gather_num;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession() { return false;}
	virtual bool EndSession() {return true;}
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() {return false;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//Same level as SitDown
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}


	bool Save(archive & ar) 
	{
		ar << _target << _where << _index << _tool_type << _task_id << _soul_gather_num;
		return true;
	}

	bool Load(archive & ar) 
	{ 
		ar >> _target >> _where >> _index >> _tool_type >> _task_id >> _soul_gather_num;
		return true;
	}
};

class session_gather : public  act_timer_session
{
public:
	session_gather(){}
	int _mine;
	unsigned short _gather_time;
	bool _gather_flag;
	bool _can_be_interruputed;
	bool _lock_inventory;
public:
	DECLARE_SUBSTANCE(session_gather);
	explicit session_gather(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp),_mine(0),_gather_time(0),_can_be_interruputed(true),_lock_inventory(false)
	{}

	void SetTarget(int mine, unsigned short gather_time, bool cbi)
	{
		_mine = mine;
		_gather_time = gather_time;
		_can_be_interruputed = cbi;
	}

	void LockInventory()
	{
		_lock_inventory = true;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//Same level as SitDown
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
	bool OnAttacked();		// return true to interrupt

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _mine << _gather_time <<  _gather_flag << _can_be_interruputed << _lock_inventory;
		return true;
	}

	bool Load(archive & ar) 
	{ 
		act_timer_session::Load(ar);
		ar >> _mine >> _gather_time >> _gather_flag >> _can_be_interruputed >> _lock_inventory;
		return true;
	}
};

class session_use_trashbox : public  act_timer_session
{
	bool _view_only;
public:
	session_use_trashbox() {}
public:
	DECLARE_SUBSTANCE(session_use_trashbox);
	explicit session_use_trashbox(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp),_view_only(false)
	{}

	void SetViewOnly(bool b)
	{ 
		_view_only = b; 
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _view_only;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _view_only;
		return true;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}	
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

};


class session_emote_action : public  act_timer_session
{
	unsigned short _action;
public:
	session_emote_action() {}
public:
	DECLARE_SUBSTANCE(session_emote_action);
	explicit session_emote_action(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp),_action(0)
	{}

	void SetAction(unsigned short action) 
	{
		_action = action;
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _action;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _action;
		return true;
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_SITDOWN;}		//Same level as SitDown
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}
};

class session_dead_move : public session_move
{
protected:
	session_dead_move(){}
public:
	DECLARE_SUBSTANCE(session_dead_move);
	explicit session_dead_move(gactive_imp * imp):session_move(imp){} 
	virtual void OnTimer(int index,int rtimes);
};

class session_dead_stop_move: public session_stop_move
{
protected:
	session_dead_stop_move(){}
public:
	DECLARE_SUBSTANCE(session_dead_stop_move);
	explicit session_dead_stop_move(gactive_imp * imp):session_stop_move(imp){} 
	virtual void OnTimer(int index,int rtimes);
};

class session_resurrect : public act_timer_session
{
protected:
	float _exp_reduce;
	int _param;
	int _time;
	session_resurrect():_exp_reduce(0){}
public:
	DECLARE_SUBSTANCE(session_resurrect);
	explicit session_resurrect(gplayer_imp * imp,int param=0,int t=17)
		:act_timer_session((gactive_imp*)imp),_exp_reduce(0),_param(param),_time(t){}


	void SetExpReduce(float reduce)
	{
		_exp_reduce = reduce;
	}

	bool Save(archive & ar)
	{
		act_timer_session::Save(ar);
		ar << _exp_reduce << _param << _time;
		return true;
	}

	bool Load(archive & ar)
	{
		act_timer_session::Load(ar);
		ar >> _exp_reduce >> _param >> _time;
		return true;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession(); 
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return true; }
	//Obtain your own mask to show your status in the team
	virtual int  GetMask() { return 0; }
	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE); }

	void OnTimer(int index,int rtimes);
};

class session_resurrect_by_item : public session_resurrect
{
	session_resurrect_by_item(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_by_item);
	explicit session_resurrect_by_item(gplayer_imp * imp,int param,int time):session_resurrect(imp,param,time){}
	virtual bool EndSession();
};

class session_resurrect_in_town: public session_resurrect
{
	session_resurrect_in_town(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_in_town);
	explicit session_resurrect_in_town(gplayer_imp * imp,int param,int time):session_resurrect(imp,param,time){}
	virtual bool EndSession();
};

class session_resurrect_by_cash : public session_resurrect
{
    session_resurrect_by_cash() {}
public:
    DECLARE_SUBSTANCE(session_resurrect_by_cash);
    explicit session_resurrect_by_cash(gplayer_imp* imp, int param, int time) : session_resurrect(imp, param, time) {}
    virtual bool EndSession();
};

class session_complete_travel : public act_session
{
	session_complete_travel(){}
public:
	DECLARE_SUBSTANCE(session_complete_travel);
	explicit session_complete_travel(gplayer_imp * imp):act_session((gactive_imp*)imp){}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return SS_MASK_ATTACK;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class session_enter_sanctuary : public act_session
{
	session_enter_sanctuary(){}
public:
	DECLARE_SUBSTANCE(session_enter_sanctuary);
	explicit session_enter_sanctuary(gplayer_imp * imp):act_session((gactive_imp*)imp){}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return 0;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class session_enter_pk_protected : public act_session
{
	session_enter_pk_protected(){}
public:
	DECLARE_SUBSTANCE(session_enter_pk_protected);
	explicit session_enter_pk_protected(gplayer_imp * imp):act_session((gactive_imp*)imp){}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return 0;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { return true;}
	bool Load(archive & ar) { return true;}
};

class session_say_hello : public act_session
{
	session_say_hello(){}
	XID _target;
public:
	DECLARE_SUBSTANCE(session_say_hello);
	explicit session_say_hello(gplayer_imp * imp):act_session((gactive_imp*)imp),_target(-1,-1){}
	void SetTarget(const XID & target)
	{
		_target = target;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession() { return true; }
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { 
		ar << _target;
		return true;
	}
	bool Load(archive & ar) 
	{ 
		ar >> _target;
		return true;
	}
};

class session_instant_skill : public act_session
{
protected:
	session_instant_skill():_data(0){}
	SKILL::Data _data;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
public:
	DECLARE_SUBSTANCE(session_instant_skill);
	explicit session_instant_skill(gactive_imp * imp)
				:act_session(imp),_data(0)
	{}

	void SetTarget(int skill_id, char force_attack,int target_num,int * targets);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession() { return false;}
	virtual bool EndSession() { return true;}
	virtual bool TerminateSession(bool force) { return true;}
	virtual bool IsTimeSpent() { return false;}
	virtual bool IsAttackSession() { return true; }

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE);
	}

	bool Save(archive & ar) 
	{
		act_session::Save(ar);
		unsigned int size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_session::Load(ar);
		unsigned int size;
		ar >> size;
		XID id;
		for(unsigned int i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

class session_cosmetic : public act_session
{
	session_cosmetic(){}
	int _inv_idx;
	int _id;
public:
	DECLARE_SUBSTANCE(session_cosmetic);
	explicit session_cosmetic(gplayer_imp * imp,int inv_idx,int id):act_session((gactive_imp*)imp),_inv_idx(inv_idx),_id(id)
	{}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession();
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { _id = -1; EndSession(); return true; }
	virtual bool IsTimeSpent() { return true; }
	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { 
		ar << _inv_idx << _id;
		return true;
	}
	bool Load(archive & ar) 
	{ 
		ar >> _inv_idx >>  _id;
		return true;
	}
};

class session_region_transport : public act_session
{
	session_region_transport(){}
	int _ridx;
	int _tag;
public:
	DECLARE_SUBSTANCE(session_region_transport);
	explicit session_region_transport(gplayer_imp * imp,int ridx, int tag):act_session((gactive_imp*)imp),_ridx(ridx),_tag(tag)
	{}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession() {return true;}
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return true; }
	virtual bool IsTimeSpent() { return false; }
	virtual int  GetMask() { return SS_MASK_MOVE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) { 
		ar << _ridx << _tag;
		return true;
	}
	bool Load(archive & ar) 
	{ 
		ar >> _ridx >> _tag;
		return true;
	}
};

class session_resurrect_protect : public act_timer_session
{
protected:
    int _more_time;
	session_resurrect_protect(){}
public:
	DECLARE_SUBSTANCE(session_resurrect_protect);
	explicit session_resurrect_protect(gplayer_imp * imp, int more_time = 0):act_timer_session((gactive_imp*)imp), _more_time(more_time) {}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool EndSession(); 
	virtual bool RepeatSession() { return false; }
	virtual bool TerminateSession(bool force) { return false; }
	virtual bool IsTimeSpent() { return true; }
	//Obtain your own mask to show your status in the team
	virtual int  GetMask() { return 0; }
	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	virtual int  GetExclusiveMask()	{ return 0xFFFFFFFF; }

};

class session_pos_skill : public act_timer_session
{
protected:
	session_pos_skill():_data(0){}
	SKILL::Data _data;
	int  _next_interval;
	bool _end_flag;
	A3DVECTOR _target_pos;
	char _force_attack;
	abase::vector<XID, abase::fast_alloc<> > _target_list;
public:
	DECLARE_SUBSTANCE(session_pos_skill);
	explicit session_pos_skill(gactive_imp * imp)
				:act_timer_session(imp),_data(0), _next_interval(20),_end_flag(false) 
	{
	}
	void SetTarget(int skill_id, const A3DVECTOR & pos, char force_attack,int target_num,int * targets);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);
	virtual bool IsAttackSession() { return true; }

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _next_interval
		   << _end_flag << _target_pos << _force_attack;
		unsigned int size = _target_list.size();
		ar << size;
		ar.push_back(_target_list.begin(), sizeof(XID)*_target_list.size());
		ar.push_back(&_data,sizeof(_data));
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _next_interval
		   >> _end_flag >> _target_pos >> _force_attack;
		unsigned int size;
		ar >> size;
		XID id;
		for(unsigned int i = 0; i < size; i ++)
		{
			ar.pop_back(&id,sizeof(id));
			_target_list.push_back(id);
		}
		ar.pop_back(&_data,sizeof(_data));
		return true;
	}
};

class session_general : public act_timer_session
{
protected:
	session_general(){}
	int _delay;
public:
	explicit session_general(gactive_imp * imp)
				:act_timer_session(imp),_delay(60)
	{
	}

	void SetDelay(int delay)
	{
		_delay = delay;
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_SITDOWN;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE);
	}


	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _delay;
		return true;
	}

	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _delay;
		return true;
	}
	virtual void OnStart() =0;
	virtual void OnRepeat() = 0;
	virtual void OnEnd() = 0;
};

class session_pet_operation : public session_general
{
protected:
	session_pet_operation(){}
	int _pet_id;
	int _index;
	int _op;
public:
	explicit session_pet_operation(gactive_imp * imp,int op)
				:session_general(imp),_op(op)
	{}

	void SetTarget(int pet_id, int index)
	{
		_pet_id = pet_id;
		_index = index;
	}

	bool Save(archive & ar) 
	{
		session_general::Save(ar);
		ar << _pet_id << _index << _op;
		return true;
	}

	bool Load(archive & ar) 
	{
		session_general::Load(ar);
		ar >> _pet_id >> _index >> _op;
		return true;
	}
	virtual void OnStart();
	virtual void OnEnd();
};

class session_summon_pet : public session_pet_operation
{
protected:
	session_summon_pet(){}
public:
	DECLARE_SUBSTANCE(session_summon_pet);
	explicit session_summon_pet(gactive_imp * imp)
				:session_pet_operation(imp,0)
	{}
	virtual void OnRepeat();
};

class session_recall_pet : public session_pet_operation
{
protected:
	session_recall_pet(){}
public:
	DECLARE_SUBSTANCE(session_recall_pet);
	explicit session_recall_pet(gactive_imp * imp)
				:session_pet_operation(imp,1)
	{}
	virtual void OnRepeat();
};

class session_free_pet : public session_pet_operation
{
protected:
	session_free_pet(){}
public:
	DECLARE_SUBSTANCE(session_free_pet);
	explicit session_free_pet(gactive_imp * imp)
				:session_pet_operation(imp,2)
	{}
	virtual void OnRepeat();
};

class session_restore_pet : public session_pet_operation
{
protected:
	session_restore_pet(){}
public:
	DECLARE_SUBSTANCE(session_restore_pet);
	explicit session_restore_pet(gactive_imp * imp)
				:session_pet_operation(imp,3)
	{}
	virtual void OnRepeat();
};

class session_rune_skill : public session_skill
{
	unsigned int level;
	int inv_index;	//������Ʒ�ڰ������е�����
	bool consume_if_use;	//ʹ�ú��Ƿ�������Ʒ
protected:
	session_rune_skill(){}
public:
	DECLARE_SUBSTANCE(session_rune_skill);
	explicit session_rune_skill(gactive_imp * imp):session_skill(imp),level(1),inv_index(0),consume_if_use(true){}
	virtual bool StartSession(bool hasmorecmd=false); 
	virtual bool RestartSession();
	virtual bool RepeatSession();
	virtual bool TerminateSession(bool force);
	virtual bool OnAttacked();
	
	void SetLevel(unsigned int l){level = l;}
	void SetInvIndex(int i){inv_index = i;}
	void SetConsumeIfUse(bool c){consume_if_use = c;}
	
	bool Save(archive & ar)
	{
		session_skill::Save(ar);
		ar << level << inv_index << consume_if_use;
		return true;
	}
	
	bool Load(archive & ar)
	{
		session_skill::Load(ar);
		ar >> level >> inv_index >> consume_if_use;
		return true;
	}
};

class session_rune_instant_skill : public session_instant_skill
{
	unsigned int level;
	int inv_index;	//������Ʒ�ڰ������е�����
	bool consume_if_use;	//ʹ�ú��Ƿ�������Ʒ
protected:
	session_rune_instant_skill(){}
public:
	DECLARE_SUBSTANCE(session_rune_instant_skill);
	explicit session_rune_instant_skill(gactive_imp* imp):session_instant_skill(imp),level(1),inv_index(0),consume_if_use(true){}
	virtual bool StartSession(bool hasmorecmd=false);
	
	void SetLevel(unsigned int l){level = l;}
	void SetInvIndex(int i){inv_index = i;}
	void SetConsumeIfUse(bool c){consume_if_use = c;}
	
	bool Save(archive & ar)
	{
		session_instant_skill::Save(ar);
		ar << level << inv_index << consume_if_use;
		return true;
	}
	
	bool Load(archive & ar)
	{
		session_instant_skill::Load(ar);
		ar >> level >> inv_index >> consume_if_use;
		return true;
	}
};

class session_produce3 : public act_timer_session	//��������
{
protected:
	session_produce3(){}
	const recipe_template * _rt;
	int _materials[16];
	int _idxs[16];
	int _equip_id;
	int _equip_inv_idx;
	char _inherit_type;
	
public:
	DECLARE_SUBSTANCE(session_produce3);
	explicit session_produce3(gplayer_imp * imp,const recipe_template * rt, int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
				:act_timer_session((gactive_imp*)imp),_rt(rt),_equip_id(equip_id),_equip_inv_idx(equip_inv_idx),_inherit_type(inherit_type)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_idxs, idxs, sizeof(_idxs));
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_produce4 : public act_timer_session	//�¼̳�����
{
protected:
	session_produce4(){}
	const recipe_template * _rt;
	int _materials[16];
	int _idxs[16];
	int _equip_id;
	int _equip_inv_idx;
	char _inherit_type;
	void *pItem; // ������Ʒ��ָ��
	int _eq_refine_level;
	int _eq_socket_count;
	int _eq_stone_type[4];
	addon_data _eq_engrave_addon_list[3];
	unsigned int _eq_engrave_addon_count;
	unsigned short _crc; // ԭ��Ʒ��crc,�����´���֤��
	
public:
	DECLARE_SUBSTANCE(session_produce4);
	explicit session_produce4(gplayer_imp * imp,const recipe_template * rt, int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
				:act_timer_session((gactive_imp*)imp),_rt(rt),_equip_id(equip_id),_equip_inv_idx(equip_inv_idx),_inherit_type(inherit_type), pItem(NULL), _eq_refine_level(0), _eq_socket_count(0), _eq_engrave_addon_count(0), _crc(0)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_idxs, idxs, sizeof(_idxs));
		for (int i = 0; i < 4; i++)
			_eq_stone_type[i] = 0;
		memset(&_eq_engrave_addon_list[0], 0, sizeof(_eq_engrave_addon_list));
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual void ChooseItem(bool remain);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_produce5 : public act_timer_session	//��������
{
protected:
	session_produce5(){}
	const recipe_template* _rt;
	int _materials[16];
	int _idxs[16];
	int _equip_id;
	int _equip_inv_idx;
	char _inherit_type;
	
public:
	DECLARE_SUBSTANCE(session_produce5);
	explicit session_produce5(gplayer_imp* imp,const recipe_template* rt, int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
        :act_timer_session((gactive_imp*)imp), _rt(rt), _equip_id(equip_id), _equip_inv_idx(equip_inv_idx), _inherit_type(inherit_type)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_idxs, idxs, sizeof(_idxs));
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_use_user_trashbox : public  act_timer_session
{
public:
	session_use_user_trashbox() {}
public:
	DECLARE_SUBSTANCE(session_use_user_trashbox);
	explicit session_use_user_trashbox(gplayer_imp * imp)
		:act_timer_session((gactive_imp*)imp)
	{}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual bool IsTimeSpent() {return true;}
	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

};

class session_knockback : public  act_timer_session
{
protected:
	session_knockback(){}
protected:
	XID _attacker;
	A3DVECTOR _attacker_pos;
	float _distance;
	int _time;
	int _stun_time;
public:
	DECLARE_SUBSTANCE(session_knockback);
	explicit session_knockback(gactive_imp* imp):act_timer_session(imp),_distance(0.f),_time(0),_stun_time(0)
	{}

	void SetInfo(const XID & attacker, const A3DVECTOR & attacker_pos, float distance, int time, int stun_time)
	{
		_attacker = attacker;
		_attacker_pos = attacker_pos;
		_distance = distance;
		_time = time;
		_stun_time = stun_time;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _attacker << _attacker_pos << _distance << _time << _stun_time;
		return true;
	}
	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _attacker >> _attacker_pos >> _distance >> _time >> _stun_time;
		return true;
	}
};

class session_knockup : public  act_timer_session
{
protected:
	session_knockup(){}
protected:
	float _distance;
	int _time;
public:
	DECLARE_SUBSTANCE(session_knockup);
	explicit session_knockup(gactive_imp* imp):act_timer_session(imp),_distance(0.f),_time(0)
	{}

	void SetInfo(float distance, int time)
	{
		_distance = distance;
		_time = time;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _distance << _time;
		return true;
	}
	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _distance >> _time;
		return true;
	}
};

class session_test : public  act_timer_session
{
protected:
	session_test(){}
	struct timeval _start;
	struct timeval _end;
	
public:
	DECLARE_SUBSTANCE(session_test);
	explicit session_test(gactive_imp* imp):act_timer_session(imp)
	{
		_start.tv_sec = _start.tv_usec = 0;
		_end.tv_sec = _end.tv_usec = 0;
	}

	virtual void OnTimer(int index,int rtimes);
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		ASSERT(false);
		return true;
	}
	bool Load(archive & ar) 
	{
		ASSERT(false);
		return true;
	}
};

class session_congregate : public  act_timer_session
{
protected:
	session_congregate(){}
	char _type;
	int _world_tag;
	A3DVECTOR _pos;

	enum{ CONGREGATE_PREPARE_TICK = 100,};
public:
	DECLARE_SUBSTANCE(session_congregate);
	explicit session_congregate(gactive_imp* imp):act_timer_session(imp),_world_tag(0)
	{
	}

	void SetDestination(char type, int world_tag, A3DVECTOR& pos)
	{
		_type = type;
		_world_tag = world_tag;
		_pos = pos;
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	void OnTimer(int index,int rtimes);

	virtual int  GetMask(){ return SS_MASK_MOVE;}
	virtual int  GetExclusiveMask(){ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar) 
	{
		ASSERT(false);
		return true;
	}
	bool Load(archive & ar) 
	{
		ASSERT(false);
		return true;
	}
};

struct engrave_recipe_template;
class session_engrave : public act_timer_session
{
protected:
	session_engrave(){}
	const engrave_recipe_template * _ert;
	unsigned int _inv_index;

public:
	DECLARE_SUBSTANCE(session_engrave);
	explicit session_engrave(gplayer_imp * imp,const engrave_recipe_template * ert, unsigned int inv_index)
				:act_timer_session((gactive_imp*)imp),_ert(ert),_inv_index(inv_index)
	{
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

struct addonregen_recipe_template;
class session_addonregen : public act_timer_session
{
protected:
	session_addonregen(){}
	const addonregen_recipe_template * _arrt;
	unsigned int _inv_index;

public:
	DECLARE_SUBSTANCE(session_addonregen);
	explicit session_addonregen(gplayer_imp * imp,const addonregen_recipe_template * arrt, unsigned int inv_index)
				:act_timer_session((gactive_imp*)imp),_arrt(arrt),_inv_index(inv_index)
	{
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	
	
	virtual void OnTimer(int index,int rtimes);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_pullover : public  act_timer_session
{
protected:
	session_pullover(){}
protected:
	XID _attacker;
	float _distance;
	int _time;
public:
	DECLARE_SUBSTANCE(session_pullover);
	explicit session_pullover(gactive_imp* imp):act_timer_session(imp),_distance(0.f),_time(0)
	{}

	void SetInfo(const XID & attacker, float distance, int time)
	{
		_attacker = attacker;
		_distance = distance;
		_time = time;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _attacker << _distance << _time;
		return true;
	}
	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _attacker >> _distance >> _time;
		return true;
	}
};

class session_teleport : public  act_timer_session
{
protected:
	session_teleport(){}
protected:
	A3DVECTOR _pos;
	int _time;
	char _mode;
public:
	DECLARE_SUBSTANCE(session_teleport);
	explicit session_teleport(gactive_imp* imp):act_timer_session(imp),_time(0),_mode(0)
	{}

	void SetInfo(const A3DVECTOR & pos, int time, char mode)
	{
		_pos = pos;
		_time = time;
		_mode = mode;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _pos << _time << _mode;
		return true;
	}
	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _pos >> _time >> _mode;
		return true;
	}
};

class session_teleport2 : public  act_timer_session
{
protected:
	session_teleport2(){}
protected:
	A3DVECTOR _pos;
	int _time;
	char _mode;
public:
	DECLARE_SUBSTANCE(session_teleport2);
	explicit session_teleport2(gactive_imp* imp):act_timer_session(imp),_time(0),_mode(0)
	{}

	void SetInfo(const A3DVECTOR & pos, int time, char mode)
	{
		_pos = pos;
		_time = time;
		_mode = mode;
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();

	virtual int  GetMask()		//Obtain your own mask to show your status in the team
	{
		return SS_MASK_MOVE;
	}
	
	virtual int  GetExclusiveMask()	//Set an exclusive mask to indicate which sessions in the queue will be excluded
	{
		return ~(SS_MASK_MOVE|SS_MASK_USE_ITEM);
	}

	bool Save(archive & ar) 
	{
		act_timer_session::Save(ar);
		ar << _pos << _time << _mode;
		return true;
	}
	bool Load(archive & ar) 
	{
		act_timer_session::Load(ar);
		ar >> _pos >> _time >> _mode;
		return true;
	}
};

class session_rebuild_pet_inheritratio : public act_timer_session
{
protected:
	session_rebuild_pet_inheritratio(){}
	int _pet_id;
	int _index;
	int _formula_index;
	int _r_attack;
	int _r_defense;
	int _r_hp;
	int _r_atk_lvl;
	int _r_def_lvl;
	//int _r_pen_lvl_f;
	//int _r_pen_lvl_m;
	//int _r_chi_point;
public:
	DECLARE_SUBSTANCE(session_rebuild_pet_inheritratio);
	explicit session_rebuild_pet_inheritratio(gplayer_imp *imp,int pet_id,int index,int formula_index)
			: act_timer_session((gactive_imp*)imp),_pet_id(pet_id),_index(index),_formula_index(formula_index),_r_attack(0),_r_defense(0),
			_r_hp(0),_r_atk_lvl(0),_r_def_lvl(0)//,_r_pen_lvl_f(0),_r_pen_lvl_m(0),_r_chi_point(0)
	{
	}

	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);

	virtual void AcceptResult(bool isaccept);

	virtual int GetMask() { return SS_MASK_SITDOWN; }
	virtual int GetExclusiveMask() { return ~(SS_MASK_MOVE);}
	
	bool Save(archive & ar);
	bool Load(archive & ar);
};

class session_rebuild_pet_nature : public act_timer_session
{
protected:
	session_rebuild_pet_nature() {}
	int _pet_id;
	int _index;
	int _formula_index;
	int _nature;
public:
	DECLARE_SUBSTANCE(session_rebuild_pet_nature);
	explicit session_rebuild_pet_nature(gplayer_imp *imp,int pet_id,int index,int formula_index)
		:act_timer_session((gactive_imp*)imp),_pet_id(pet_id),_index(index),_formula_index(formula_index),_nature(0)
		{
		}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void OnTimer(int index,int rtimes);

	virtual void AcceptResult(bool isaccept);

	virtual int GetMask() { return SS_MASK_SITDOWN; }
	virtual int GetExclusiveMask() { return ~(SS_MASK_MOVE);}
	
	bool Save(archive & ar);
	bool Load(archive & ar);
};


// Lib Produce

struct lib_recipe_template;
class session_lib_produce : public act_timer_session
{
protected:
	session_lib_produce(){}
	const lib_recipe_template * _lib;
	int _materials[64];
	int _itens_count[64];
	int _index_inv[64];

public:
	DECLARE_SUBSTANCE(session_lib_produce);
	explicit session_lib_produce(gplayer_imp * imp,const lib_recipe_template * lib, int materials[64], int material_count[64], int index_inv[64])
				:act_timer_session((gactive_imp*)imp),_lib(lib)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_itens_count, material_count, sizeof(_itens_count));
		memcpy(_index_inv, index_inv, sizeof(_index_inv));
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};

// New Engrave

class session_new_engrave : public act_timer_session
{
protected:
	session_new_engrave(){}
	const engrave_recipe_template * _ert;
	unsigned int _inv_index;	
	addon_data _addon_list[3];
	unsigned int _addon_count;
			
public:
	DECLARE_SUBSTANCE(session_new_engrave);
	explicit session_new_engrave(gplayer_imp * imp,const engrave_recipe_template * ert, unsigned int inv_index, addon_data addon_list[3], unsigned int addon_count)
				:act_timer_session((gactive_imp*)imp),_ert(ert),_inv_index(inv_index),_addon_count(addon_count)
	{	
		memcpy(_addon_list, addon_list, sizeof(_addon_list));
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void ChooseItem(bool remain);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);	
	bool Load(archive & ar);
};


// G17

class session_new_armor : public act_timer_session
{
protected:
	session_new_armor(){}
	unsigned int _inv_index;	
	int* _addons_lock_pos;
	addon_data _addons_new[5];
	int _count_rem_item;
	addon_data _addons_notify[5];
	int _addons_count_notify;
			
public:
	DECLARE_SUBSTANCE(session_new_armor);
	explicit session_new_armor(gplayer_imp * imp,unsigned int inv_index, int* addons_lock_pos, addon_data addons_new[5], addon_data addons_notify[5], int addons_count_notify, int count_rem_item)
                :act_timer_session((gactive_imp*)imp),_inv_index(inv_index),_addons_lock_pos(addons_lock_pos),_count_rem_item(count_rem_item),_addons_count_notify(addons_count_notify)
	{   
		memcpy(_addons_new, addons_new, sizeof(_addons_new));
		memcpy(_addons_notify, addons_notify, sizeof(_addons_notify));
	}
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession() { return false;}
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);
	virtual void ChooseItem(int change);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);	
	bool Load(archive & ar);
};


class session_carrier_skill : public session_skill
{
	unsigned int level;

protected:
	session_carrier_skill(){}
public:
	DECLARE_SUBSTANCE(session_carrier_skill);
	explicit session_carrier_skill(gactive_imp * imp):session_skill(imp),level(5){}
	virtual bool StartSession(bool hasmorecmd=false); 
	virtual bool RestartSession();
	virtual bool RepeatSession();
	virtual bool TerminateSession(bool force);
	virtual bool OnAttacked();
	
	void SetLevel(unsigned int l){level = l;}
	
	bool Save(archive & ar)
	{
		session_skill::Save(ar);
		ar << level;
		return true;
	}
	
	bool Load(archive & ar)
	{
		session_skill::Load(ar);
		ar >> level;
		return true;
	}
};

// Nova armadura

class session_produce_new_armor : public act_timer_session	//��������
{
protected:
	session_produce_new_armor(){}
	const recipe_template * _rt;
	int _materials[16];
	int _idxs[16];
	int _equip_id;
	int _equip_inv_idx;
	char _inherit_type;
	
public:
	DECLARE_SUBSTANCE(session_produce_new_armor);
	explicit session_produce_new_armor(gplayer_imp * imp,const recipe_template * rt, int materials[16], int idxs[16], int equip_id, int equip_inv_idx, char inherit_type)
				:act_timer_session((gactive_imp*)imp),_rt(rt),_equip_id(equip_id),_equip_inv_idx(equip_inv_idx),_inherit_type(inherit_type)
	{
		memcpy(_materials, materials, sizeof(_materials));
		memcpy(_idxs, idxs, sizeof(_idxs));
	}
	
	virtual bool StartSession(bool hasmorecmd=false);
	virtual bool RepeatSession();
	virtual bool EndSession();
	virtual bool TerminateSession(bool force);

	virtual int  GetMask() { return SS_MASK_USE_SERVICE;}
	virtual int  GetExclusiveMask()	{ return ~(SS_MASK_MOVE);}

	bool Save(archive & ar);
	bool Load(archive & ar);
};


#endif

