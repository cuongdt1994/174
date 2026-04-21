#ifndef __ONLINE_GAME_GS_AI_TRIGGER_H__
#define __ONLINE_GAME_GS_AI_TRIGGER_H__

#include "substance.h"
#include <common/types.h>
#include <amemory.h>
#include <arandomgen.h>
#include <ASSERT.h>
#include <string.h>

class ai_object;
class ai_policy;
namespace ai_trigger
{
	class policy;
	class condition : public substance
	{
	public:
		enum
		{
			TYPE_INVALID,
			TYPE_HEARTBEAT,
			TYPE_TIMER,
			TYPE_KILL_TARGET,
			TYPE_START_COMBAT,
			TYPE_START_DEATH,
			TYPE_ON_DAMAGE,
			TYPE_PATH_END,
			TYPE_END_COMBAT,
		};

	public:
		virtual condition *Clone() const = 0;
		virtual ~condition() {}
		virtual bool Check(policy *self, int param) = 0;
		virtual int GetConditionType() = 0; // �������ͣ� �������ͺʹ�������
		virtual bool IsAutoDisable() = 0;
		virtual void Reset() {}
	};

	class expr
	{
	public:
		enum
		{
			EXPR_CONSTANT,
			EXPR_COMMON_DATA,
			EXPR_PLUS,
			EXPR_MINUS,
			EXPR_MULTIPLY,
			EXPR_DIVIDE,
			EXPR_HISTORY_VALUE,
			EXPR_LOCAL_VALUE,
			EXPR_ROOM_INDEX,
			EXPR_SPHERE_PLAYER_COUNT,
			EXPR_BOX_PLAYER_COUNT,
			// EXPR_NOT_BIND_CARRIER,
			EXPR_C_35,
			EXPR_C_GET_RADIUS_PLAYER_COUNT,
			EXPR_C_37,
			EXPR_C_38,
			EXPR_C_39,
			EXPR_C_45,
			EXPR_C_47,
		};
		expr() {}
		virtual expr *Clone() const = 0;
		virtual ~expr() {}
		virtual int GetValue(policy *self) = 0;
		virtual int GetExprType() = 0;
		class Exception
		{
		}; // �쳣���������
	};

	class target : public substance
	{
	public:
		virtual target *Clone() const = 0;
		virtual ~target() {}
		virtual void GetTarget(policy *self, XID &target) = 0;
	};

	class operation : public substance
	{
	protected:
		target *_target;
		operation *_next;

	public:
		virtual operation *Clone() const = 0;
		operation() : _target(0), _next(0) {}
		operation(const operation &rhs)
		{
			if (rhs._target)
			{
				_target = rhs._target->Clone();
			}
			else
			{
				_target = 0;
			}
			_next = 0;
		}

		virtual ~operation()
		{
			if (_target)
				delete _target;
		}

		void SetTarget(target *__tar)
		{
			_target = __tar;
		}

		void SetNext(operation *next)
		{
			ASSERT(next == 0 || _next == 0);
			_next = next;
		}
		operation *GetNext()
		{
			return _next;
		}

	public:
		virtual bool DoSomething(policy *self) = 0;
		virtual void Reset() {}
		virtual bool RequireTarget() = 0;
	};

	class trigger : public abase::ASmallObject
	{
	protected:
		int _id;
		condition *_cond;
		operation *_ops;
		bool _enable;
		bool _default_enable;
		bool _battle_trigger;

	public:
		trigger() : _id(-1), _cond(0), _ops(0), _enable(false), _default_enable(false), _battle_trigger(true)
		{
		}
		~trigger()
		{
			delete _cond;
			operation *tmp = _ops;
			while (tmp)
			{
				operation *tmp2 = tmp->GetNext();
				delete tmp;
				tmp = tmp2;
			}
		}

		trigger(const trigger &rhs)
		{
			_enable = rhs._enable;
			_default_enable = rhs._default_enable;
			_battle_trigger = rhs._battle_trigger;
			_id = rhs._id;
			_cond = rhs._cond->Clone();
			if (rhs._ops)
			{
				operation *tmp = rhs._ops;
				operation *tmp2 = rhs._ops;
				_ops = tmp->Clone();
				tmp2 = _ops;
				while (tmp->GetNext())
				{
					tmp2->SetNext(0);
					tmp2->SetNext(tmp->GetNext()->Clone());
					tmp2 = tmp2->GetNext();
					tmp = tmp->GetNext();
				}
				tmp2->SetNext(0);
			}
			else
			{
				_ops = 0;
			}
		}

		inline void SetData(int id, condition *cond)
		{
			_id = id;
			_cond = cond;
		}

		inline void AddOp(operation *op)
		{
			op->SetNext(0);
			if (_ops)
			{
				operation *tmp = _ops;
				while (tmp->GetNext())
				{
					tmp = tmp->GetNext();
				}
				tmp->SetNext(op);
			}
			else
			{
				_ops = op;
			}
		}

		inline bool IsEnable()
		{
			return _enable;
		}

		inline bool IsBattleTrigger()
		{
			return _battle_trigger;
		}

		inline void EnableTrigger(bool enable)
		{
			_enable = enable;
		}

		inline void SetBattleEnable(bool battle_trigger)
		{
			_battle_trigger = battle_trigger;
		}

		inline void SetDefaultEnable(bool enable)
		{
			_default_enable = enable;
			_enable = enable;
		}

		inline int GetTriggerID()
		{
			return _id;
		}
		/*inline int GetCondType()
		{
			return _cond->GetConditionType();
		}*/
		inline int GetCondType()
		{
			if (!_cond)
				return -1;
			return _cond->GetConditionType();
		}

	public:
		inline bool TestTrigger(policy *self, int param = 0)
		{
			try
			{
				if (_cond->Check(self, param))
				{
					bool bRst = true;
					operation *tmp = _ops;
					while (tmp)
					{
						if (!tmp->DoSomething(self))
						{
							bRst = false;
							break;
						}
						tmp = tmp->GetNext();
					}
					if (_cond->IsAutoDisable())
					{
						EnableTrigger(false);
					}
					return bRst;
				}
			}
			catch (...)
			{
				// do nothing
			}
			return true;
		}

		inline void Reset()
		{
			if (_cond)
				_cond->Reset();
			else
				printf("ai_trigger::Reset: _cond == NULL! , id = %d , _enable = %d , _default_enable = %d , _battle_trigger = %d  \n", _id, _enable, _default_enable, _battle_trigger);
			if (_ops)
				_ops->Reset();
			else
				printf("ai_trigger::Reset: _ops  == NULL! , id = %d , _enable = %d , _default_enable = %d , _battle_trigger = %d  \n", _id, _enable, _default_enable, _battle_trigger);
			_enable = _default_enable;
		}
	};

	class policy : public abase::ASmallObject
	{
		struct timer
		{
			int id;
			int timeout;
			int period;
			int times;
			timer(int id, int to, int p, int ts) : id(id), timeout(to), period(p), times(ts)
			{
			}
		};
		typedef abase::vector<trigger *, abase::fast_alloc<>> TRIGGER_LIST;
		TRIGGER_LIST _trigger_list;	  // �������ܱ�
		TRIGGER_LIST _hb_tri_list;	  // ������������Ѫ���жϵȣ�
		TRIGGER_LIST _tm_tri_list;	  // ��ʱ��������
		TRIGGER_LIST _st_bat_list;	  // ��ʼս��������
		TRIGGER_LIST _kl_ply_list;	  // ɱ������
		TRIGGER_LIST _death_list;	  // ɱ������
		TRIGGER_LIST _on_damage_list; // �ܵ��˺�������
		TRIGGER_LIST _path_list;	  // ·����ش�����
		TRIGGER_LIST _ed_bat_list;	  // ����ս��������

		abase::vector<timer> _timer_list;
		int _timer_flag;  // ���β����Ķ�ʱ��״̬	 0 ,�޲��� 0x01�ж�ʱ������,0x02,�ж�ʱ��Ҫ��ɾ��
		ai_object *_self; // ����ָ�����
		ai_policy *_aip;  // ai ����
		int _id;		  // ����id
		int _peace_trigger_count;

	public:
		void AddTrigger(trigger *pTrigger)
		{
			int t = pTrigger->GetCondType();
			switch (t)
			{
			case condition::TYPE_HEARTBEAT:
				_hb_tri_list.push_back(pTrigger);
				if (!pTrigger->IsBattleTrigger())
				{
					_peace_trigger_count++;
				}
				break;
			case condition::TYPE_TIMER:
				_tm_tri_list.push_back(pTrigger);
				break;
			case condition::TYPE_KILL_TARGET:
				_kl_ply_list.push_back(pTrigger);
				break;
			case condition::TYPE_START_COMBAT:
				pTrigger->SetDefaultEnable(true);
				_st_bat_list.push_back(pTrigger);
				break;
			case condition::TYPE_START_DEATH:
				pTrigger->SetDefaultEnable(true);
				_death_list.push_back(pTrigger);
				break;
			case condition::TYPE_ON_DAMAGE:
				_on_damage_list.push_back(pTrigger);
				break;
			case condition::TYPE_PATH_END:
				_path_list.push_back(pTrigger);
				break;
			case condition::TYPE_END_COMBAT:
				pTrigger->SetDefaultEnable(true);
				_ed_bat_list.push_back(pTrigger);
				break;
			default:
				ASSERT(false);
				break;
			}
			_trigger_list.push_back(pTrigger);
		}

	public:
		policy(int id) : _timer_flag(0), _self(NULL), _aip(NULL), _id(id), _peace_trigger_count(0)
		{
		}

		policy(const policy &rhs) : _timer_flag(rhs._timer_flag), _self(NULL), _aip(NULL), _id(rhs._id)
		{
			_peace_trigger_count = rhs._peace_trigger_count;
			unsigned int count = rhs._trigger_list.size();
			for (unsigned int i = 0; i < count; i++)
			{
				trigger *pTri = new trigger(*rhs._trigger_list[i]);
				AddTrigger(pTri);
			}
		}

		~policy()
		{
			unsigned int count = _trigger_list.size();
			for (unsigned int i = 0; i < count; i++)
			{
				delete _trigger_list[i];
			}
		}

		void Reset()
		{
			unsigned int count = _trigger_list.size();
			for (unsigned int i = 0; i < count; i++)
			{
				if (_trigger_list[i]->IsBattleTrigger())
					_trigger_list[i]->Reset();
			}

			_timer_flag = 0;
			_timer_list.clear();
		}

		void ResetAll()
		{
			unsigned int count = _trigger_list.size();
			for (unsigned int i = 0; i < count; i++)
			{
				_trigger_list[i]->Reset();
			}

			_timer_flag = 0;
			_timer_list.clear();
		}

		void SetParent(ai_object *self, ai_policy *aip)
		{
			_self = self;
			_aip = aip;
		}

		int GetID()
		{
			return _id;
		}

		virtual ai_object *GetAIObject()
		{
			return _self;
		}

		ai_policy *GetAIPolicy()
		{
			return _aip;
		}


		void EnableTrigger(int id, bool enable)
		{
			for (unsigned int i = 0; i < _trigger_list.size(); i++)
			{
				if (_trigger_list[i]->GetTriggerID() == id)
				{
					_trigger_list[i]->EnableTrigger(enable);
				}
			}
		}

		void CreateTimer(int id, int period, int times)
		{
			for (unsigned int i = 0; i < _timer_list.size(); i++)
			{
				if (_timer_list[i].id == id)
				{
					_timer_list[i].id = id;
					_timer_list[i].timeout = period;
					_timer_list[i].period = period;
					_timer_list[i].times = times;
					return;
				}
			}
			_timer_list.push_back(timer(id, period, period, times));
		}

		void RefreshTimer()
		{
			_timer_flag = 0;
			for (unsigned int i = 0; i < _timer_list.size(); i++)
			{
				timer &t = _timer_list[i];
				if (!t.timeout)
					t.timeout = t.period;
				t.timeout--;
				if (t.timeout == 0)
				{
					_timer_flag |= 0x01;
					if (t.times > 0)
					{
						t.times--;
						if (!t.times)
						{
							t.times = -1;
							_timer_flag |= 0x02;
						}
					}
				}
			}
		}

		void RemoveTimer(int id)
		{
			for (unsigned int i = 0; i < _timer_list.size(); i++)
			{
				if (_timer_list[i].id == id)
				{
					_timer_list.erase(_timer_list.begin() + i);
					return;
				}
			}
		}

		void RemoveEmptyTimer()
		{
			_timer_flag = 0;
			for (unsigned int i = 0; i < _timer_list.size(); i++)
			{
				timer &t = _timer_list[i];
				if (t.times < 0)
				{
					_timer_list.erase(_timer_list.begin() + i);
					i--;
				}
			}
		}

		bool CheckTimer(int id)
		{
			if (_timer_flag == 0)
				return false;
			for (unsigned int i = 0; i < _timer_list.size(); i++)
			{
				timer &t = _timer_list[i];
				if (t.id != id)
					continue;
				return (t.timeout == 0);
			}
			return false;
		}

		void CheckTriggers(abase::vector<trigger *, abase::fast_alloc<>> &list, int param = 0)
		{
			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (!list[i]->IsEnable())
					continue;
				if (!list[i]->TestTrigger(this, param))
				{
					return;
				}
			}
		}

		void CheckPeaceTriggers(abase::vector<trigger *, abase::fast_alloc<>> &list)
		{
			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (list[i]->IsBattleTrigger())
					continue;
				if (!list[i]->IsEnable())
					continue;
				if (!list[i]->TestTrigger(this))
				{
					return;
				}
			}
		}

		void CheckTriggersNoBreak(abase::vector<trigger *, abase::fast_alloc<>> &list)
		{
			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (!list[i]->IsEnable())
					continue;
				list[i]->TestTrigger(this);
			}
		}

		void CheckTriggersNoTest(abase::vector<trigger *, abase::fast_alloc<>> &list)
		{
			for (unsigned int i = 0; i < list.size(); i++)
			{
				if (!list[i]->TestTrigger(this))
				{
					return;
				}
			}
		}

		void OnHeartbeat()
		{
			RefreshTimer();
			if (_timer_flag & 0x01)
			{
				CheckTriggersNoBreak(_tm_tri_list);
			}

			if (_timer_flag & 0x02)
			{
				RemoveEmptyTimer();
			}

			CheckTriggers(_hb_tri_list);
		}

		void OnPeaceHeartbeat()
		{
			RefreshTimer();
			if (_timer_flag & 0x01)
			{
				CheckTriggersNoBreak(_tm_tri_list);
			}

			if (_timer_flag & 0x02)
			{
				RemoveEmptyTimer();
			}

			if (!_peace_trigger_count)
				return;
			CheckPeaceTriggers(_hb_tri_list);
		}

		void OnPeaceHeartbeatInCombat()
		{
			if (!_peace_trigger_count)
				return;
			CheckPeaceTriggers(_hb_tri_list);
		}

		void StartCombat()
		{
			CheckTriggersNoTest(_st_bat_list);
		}

		void KillTarget(const XID &target)
		{
			// ���ﻹ��Ҫ�������һЩ����
			CheckTriggers(_kl_ply_list);
		}

		void OnDeath()
		{
			CheckTriggers(_death_list);
		}

		void OnDamage()
		{
			CheckTriggers(_on_damage_list);
		}

		void PathEnd(int path_id)
		{
			CheckTriggers(_path_list, path_id);
		}

		void EndCombat()
		{
			CheckTriggersNoTest(_ed_bat_list);
		}
	};

	// �����Ƕ��������,Ŀ��ѡ��Ͳ���
	//-------------���������������� -------- ����
	class cond_unary : public condition
	{
	protected:
		condition *_cond;

	public:
		cond_unary(condition *cond) : _cond(cond)
		{
			ASSERT(_cond);
		}

		cond_unary(const cond_unary &rhs)
		{
			_cond = rhs._cond->Clone();
		}

		~cond_unary()
		{
			delete _cond;
		}

		virtual bool Check(policy *self, int p)
		{
			return !_cond->Check(self, p);
		}
		virtual int GetConditionType()
		{
			return _cond->GetConditionType();
		}
	};

	class cond_not : public cond_unary
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_not(*this);
		}

		cond_not(condition *cond) : cond_unary(cond)
		{
		}

		virtual bool Check(policy *self, int p)
		{
			return !_cond->Check(self, p);
		}
		virtual bool IsAutoDisable()
		{
			return _cond->IsAutoDisable();
		}
	};

	class cond_binary : public condition
	{
	protected:
		condition *_left;
		condition *_right;

	public:
		cond_binary(condition *left, condition *right) : _left(left), _right(right)
		{
			ASSERT(_left && _right);
		}

		cond_binary(const cond_binary &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}

		~cond_binary()
		{
			delete _left;
			delete _right;
		}

		virtual int GetConditionType()
		{
			return _left->GetConditionType();
		}
	};

	class cond_and : public cond_binary
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_and(*this);
		}

		cond_and(condition *left, condition *right) : cond_binary(left, right)
		{
		}

		virtual bool Check(policy *self, int p)
		{
			return _left->Check(self, p) && _right->Check(self, p);
		}
		virtual bool IsAutoDisable()
		{
			return _left->IsAutoDisable() && _right->IsAutoDisable();
		}
	};

	class cond_or : public cond_binary
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_or(*this);
		}

		cond_or(condition *left, condition *right) : cond_binary(left, right)
		{
		}

		virtual bool Check(policy *self, int p)
		{
			return _left->Check(self, p) || _right->Check(self, p);
		}
		virtual bool IsAutoDisable()
		{
			return _left->IsAutoDisable() || _right->IsAutoDisable();
		}
	};

	// �����ǻ���Ԫ��
	class cond_hp_less : public condition
	{
		float _ratio;

	public:
		virtual condition *Clone() const
		{
			return new cond_hp_less(*this);
		}

		cond_hp_less(float ratio) : _ratio(ratio)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_timer : public condition
	{
		int _timer_id;

	public:
		virtual condition *Clone() const
		{
			return new cond_timer(*this);
		}

		cond_timer(int id) : _timer_id(id)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_TIMER;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_timer_2 : public condition
	{
		int _timer_id;

	public:
		virtual condition *Clone() const
		{
			return new cond_timer_2(*this);
		}

		cond_timer_2(int id) : _timer_id(id)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_TIMER;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_start_combat : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_start_combat(*this);
		}

		virtual bool Check(policy *self, int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_START_COMBAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_on_death : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_on_death(*this);
		}

		virtual bool Check(policy *self, int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_START_DEATH;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_random : public condition
	{
		float _rate;

	public:
		virtual condition *Clone() const
		{
			return new cond_random(*this);
		}

		cond_random(float rate) : _rate(rate)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_kill_target : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_kill_target(*this);
		}

		virtual bool Check(policy *self, int)
		{
			return true;
		}
		virtual int GetConditionType()
		{
			return TYPE_KILL_TARGET;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_expr : public condition
	{
		expr *_e;

	public:
		cond_expr(expr *e) : _e(e) {}

		virtual condition *Clone() const
		{
			return new cond_expr(*this);
		}
		cond_expr(const cond_expr &rhs)
		{
			_e = rhs._e->Clone();
		}
		~cond_expr()
		{
			delete _e;
		}
		virtual bool Check(policy *self, int)
		{
			return _e->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_compare_less : public condition // С��
	{
		expr *_left;
		expr *_right;

	public:
		cond_compare_less(expr *l, expr *r) : _left(l), _right(r) {}
		virtual condition *Clone() const
		{
			return new cond_compare_less(*this);
		}
		cond_compare_less(const cond_compare_less &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_less()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy *self, int)
		{
			return _left->GetValue(self) < _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_compare_greater : public condition // ����
	{
		expr *_left;
		expr *_right;

	public:
		cond_compare_greater(expr *l, expr *r) : _left(l), _right(r) {}

		virtual condition *Clone() const
		{
			return new cond_compare_greater(*this);
		}
		cond_compare_greater(const cond_compare_greater &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_greater()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy *self, int)
		{
			return _left->GetValue(self) > _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};
	class cond_compare_equal : public condition // ����
	{
		expr *_left;
		expr *_right;

	public:
		cond_compare_equal(expr *l, expr *r) : _left(l), _right(r) {}

		virtual condition *Clone() const
		{
			return new cond_compare_equal(*this);
		}
		cond_compare_equal(const cond_compare_equal &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~cond_compare_equal()
		{
			delete _left;
			delete _right;
		}
		virtual bool Check(policy *self, int)
		{
			return _left->GetValue(self) == _right->GetValue(self);
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_time_point : public condition // lgc
	{
		int _hour;
		int _minute;

	public:
		virtual condition *Clone() const
		{
			return new cond_time_point(*this);
		}

		cond_time_point(int h, int m) : _hour(h), _minute(m)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_on_damage : public condition
	{
		int _min_damage;
		int _max_damage;

	public:
		virtual condition *Clone() const
		{
			return new cond_on_damage(*this);
		}

		cond_on_damage(int min_dmg, int max_dmg) : _min_damage(min_dmg), _max_damage(max_dmg)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_ON_DAMAGE;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_path_end : public condition
	{
		int _global_path_id;

	public:
		virtual condition *Clone() const
		{
			return new cond_path_end(*this);
		}

		cond_path_end(int global_path_id) : _global_path_id(global_path_id)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_PATH_END;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_path_end_2 : public condition
	{
		int _index;
		int _type;

	public:
		virtual condition *Clone() const
		{
			return new cond_path_end_2(*this);
		}

		cond_path_end_2(int id, int type) : _index(id), _type(type)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_PATH_END;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};

	class cond_at_history_stage : public condition
	{
		int _history_stage_id;

	public:
		virtual condition *Clone() const
		{
			return new cond_at_history_stage(*this);
		}

		cond_at_history_stage(int hid) : _history_stage_id(hid)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}
		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_end_combat : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_end_combat(*this);
		}

		virtual bool Check(policy *self, int)
		{
			return true;
		}

		virtual int GetConditionType()
		{
			return TYPE_END_COMBAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_spec_filter : public condition
	{
		int spec_filter_id;

	public:
		cond_spec_filter(int sfi) : spec_filter_id(sfi) {}

		virtual condition *Clone() const
		{
			return new cond_spec_filter(*this);
		}

		virtual bool Check(policy *self, int);

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_get_servertime_day : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_get_servertime_day(*this);
		}

		virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_get_servertime_week : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_get_servertime_week(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_not_bind_carrier : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_not_bind_carrier(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_reach_start : public condition
	{
		int _index;
		int _type;

	public:
		virtual condition *Clone() const
		{
			return new cond_reach_start(*this);
		}

		cond_reach_start(int id, int type) : _index(id), _type(type)
		{
		}

		virtual bool Check(policy *self, int);
		virtual int GetConditionType()
		{
			return TYPE_PATH_END;
		}
		virtual bool IsAutoDisable()
		{
			return true;
		}
	};



	class cond_carrier_full : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_carrier_full(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_35 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_35(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_36 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_36(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_37 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_37(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_38 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_38(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_39 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_39(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_40 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_40(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_41 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_41(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_42 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_42(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_43 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_43(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_44 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_44(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_45 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_45(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_46 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_46(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};

	class cond_47 : public condition
	{
	public:
		virtual condition *Clone() const
		{
			return new cond_47(*this);
		}

			virtual bool Check(policy *self, int)
		{
			return false;
		}

		virtual int GetConditionType()
		{
			return TYPE_HEARTBEAT;
		}

		virtual bool IsAutoDisable()
		{
			return false;
		}
	};
	// ���ڵ��ڡ�С�ڵ��ںͲ����ڿ�����cond_not��ϳ�����

	// �±������ֻ�������ʽ
	class expr_constant : public expr // ����
	{
		int _value;

	public:
		expr_constant(const int value) : _value(value) {}
		virtual expr *Clone() const
		{
			return new expr_constant(*this);
		}
		expr_constant(const expr_constant &rhs)
		{
			_value = rhs._value;
		}
		virtual int GetValue(policy *)
		{
			return _value;
		}
		virtual int GetExprType()
		{
			return EXPR_CONSTANT;
		}
	};

	class expr_common_data : public expr // ��ȫ��������ȡ
	{
		int _key;

	public:
		expr_common_data(const int key) : _key(key) {}
		virtual expr *Clone() const
		{
			return new expr_common_data(*this);
		}
		expr_common_data(const expr_common_data &rhs)
		{
			_key = rhs._key;
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_COMMON_DATA;
		}
	};

	class expr_history_value : public expr // ȡ��ʷ��������
	{
		int _key;

	public:
		expr_history_value(const int key) : _key(key) {}
		virtual expr *Clone() const
		{
			return new expr_history_value(*this);
		}
		expr_history_value(const expr_history_value &rhs)
		{
			_key = rhs._key;
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_HISTORY_VALUE;
		}
	};

	class expr_local_value : public expr // ȡnpc������������
	{
		int _key;

	public:
		expr_local_value(const int key) : _key(key) {}
		virtual expr *Clone() const
		{
			return new expr_local_value(*this);
		}
		expr_local_value(const expr_local_value &rhs)
		{
			_key = rhs._key;
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_LOCAL_VALUE;
		}
	};

	class expr_room_index : public expr
	{
	public:
		expr_room_index() {}
		virtual expr *Clone() const
		{
			return new expr_room_index(*this);
		}
		expr_room_index(const expr_room_index &rhs)
		{
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_ROOM_INDEX;
		}
	};

	class expr_sphere_player_count : public expr
	{
		float _radius;

	public:
		expr_sphere_player_count(const float r) : _radius(r) {}
		virtual expr *Clone() const
		{
			return new expr_sphere_player_count(*this);
		}
		expr_sphere_player_count(const expr_sphere_player_count &rhs)
		{
			_radius = rhs._radius;
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_SPHERE_PLAYER_COUNT;
		}
	};

	class expr_box_player_count : public expr
	{
		rect _rect;

	public:
		expr_box_player_count(float l, float t, float r, float b) : _rect(l, t, r, b) {}
		virtual expr *Clone() const
		{
			return new expr_box_player_count(*this);
		}
		expr_box_player_count(const expr_box_player_count &rhs)
		{
			_rect = rect(rhs._rect.left, rhs._rect.top, rhs._rect.right, rhs._rect.bottom);
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_BOX_PLAYER_COUNT;
		}
	};

	// �±��ǻ�������ʽ����������
	class expr_plus : public expr // �ӷ�����
	{
		expr *_left;
		expr *_right;

	public:
		expr_plus(expr *a1, expr *a2) : _left(a1), _right(a2) {}
		virtual expr *Clone() const
		{
			return new expr_plus(*this);
		}
		expr_plus(const expr_plus &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~expr_plus()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self)
		{
			return _left->GetValue(self) + _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_PLUS;
		}
	};
	class expr_minus : public expr // ��������
	{
		expr *_left;
		expr *_right;

	public:
		expr_minus(expr *a1, expr *a2) : _left(a1), _right(a2) {}
		virtual expr *Clone() const
		{
			return new expr_minus(*this);
		}
		expr_minus(const expr_minus &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~expr_minus()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self)
		{
			return _left->GetValue(self) - _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_MINUS;
		}
	};

	class expr_multiply : public expr // �˷�����
	{
		expr *_left;
		expr *_right;

	public:
		expr_multiply(expr *a1, expr *a2) : _left(a1), _right(a2) {}
		virtual expr *Clone() const
		{
			return new expr_multiply(*this);
		}
		expr_multiply(const expr_multiply &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~expr_multiply()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self)
		{
			return _left->GetValue(self) * _right->GetValue(self);
		}
		virtual int GetExprType()
		{
			return EXPR_MULTIPLY;
		}
	};
	class expr_divide : public expr // ��������
	{
		expr *_left;
		expr *_right;

	public:
		expr_divide(expr *a1, expr *a2) : _left(a1), _right(a2) {}
		virtual expr *Clone() const
		{
			return new expr_divide(*this);
		}
		expr_divide(const expr_divide &rhs)
		{
			_left = rhs._left->Clone();
			_right = rhs._right->Clone();
		}
		~expr_divide()
		{
			delete _left;
			delete _right;
		}

		virtual int GetValue(policy *self)
		{
			int value = _right->GetValue(self);
			if (value == 0)
			{
				throw expr::Exception();
			}
			// return _left->GetValue(self) / _right->GetValue(self);
			return _left->GetValue(self) / value;
		}
		virtual int GetExprType()
		{
			return EXPR_DIVIDE;
		}
	};

	/*class expr_not_bind_carrier:public expr
	{
		public:
		expr_not_bind_carrier () {}
		virtual expr* Clone () const
		{
			return new expr_not_bind_carrier(*this);
		}
		expr_not_bind_carrier ( const expr_not_bind_carrier& rhs)
		{
		}
		virtual int GetValue(policy*);
		virtual int GetExprType()
		{
			return EXPR_NOT_BIND_CARRIER;
		}
	};*/

	class expr_c_35 : public expr
	{
	public:
		expr_c_35() {}
		virtual expr *Clone() const
		{
			return new expr_c_35(*this);
		}
		expr_c_35(const expr_c_35 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_35;
		}
	};

	class expr_c_get_radius_player_count : public expr
	{
		rect _rect;

	public:
		expr_c_get_radius_player_count(float l, float t, float r, float b) : _rect(l, t, r, b) {}
		virtual expr *Clone() const
		{
			return new expr_c_get_radius_player_count(*this);
		}
		expr_c_get_radius_player_count(const expr_c_get_radius_player_count &rhs)
		{
			_rect = rect(rhs._rect.left, rhs._rect.top, rhs._rect.right, rhs._rect.bottom);
		}
		virtual int GetValue(policy *);
		virtual int GetExprType()
		{
			return EXPR_C_GET_RADIUS_PLAYER_COUNT;
		}
	};



	class expr_c_37 : public expr
	{
	public:
		expr_c_37() {}
		virtual expr *Clone() const
		{
			return new expr_c_37(*this);
		}
		expr_c_37(const expr_c_37 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_37;
		}
	};

	class expr_c_38 : public expr
	{
	public:
		expr_c_38() {}
		virtual expr *Clone() const
		{
			return new expr_c_38(*this);
		}
		expr_c_38(const expr_c_38 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_38;
		}
	};

	class expr_c_39 : public expr
	{
	public:
		expr_c_39() {}
		virtual expr *Clone() const
		{
			return new expr_c_39(*this);
		}
		expr_c_39(const expr_c_39 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_39;
		}
	};

	class expr_c_45 : public expr
	{
	public:
		expr_c_45() {}
		virtual expr *Clone() const
		{
			return new expr_c_45(*this);
		}
		expr_c_45(const expr_c_45 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_45;
		}
	};

	class expr_c_47 : public expr
	{
	public:
		expr_c_47() {}
		virtual expr *Clone() const
		{
			return new expr_c_47(*this);
		}
		expr_c_47(const expr_c_47 &rhs)
		{
		}
		virtual int GetValue(policy *) { return 0; }
		virtual int GetExprType()
		{
			return EXPR_C_47;
		}
	};
	//------ ������ѡ���߼� --------

	class target_self : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_self(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_aggro_first : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_aggro_first(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_aggro_second : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_aggro_second(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_aggro_second_rand : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_aggro_second_rand(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_least_hp : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_least_hp(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_most_hp : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_most_hp(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_most_mp : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_most_mp(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_class_combo : public target
	{
		int _combo_state;

	public:
		target_class_combo(int combo) : _combo_state(combo)
		{
		}

		virtual target *Clone() const
		{
			return new target_class_combo(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_monster_killer : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_monster_killer(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_monster_birthplace_faction : public target
	{
	public:
		virtual target *Clone() const
		{
			return new target_monster_birthplace_faction(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_aggro_special : public target
	{
		int _type;

	public:
		enum
		{
			ATAS_RAND,
			ATAS_NEAR,
			ATAS_FAR,
		};

		target_aggro_special(int type) : _type(type) {}

		virtual target *Clone() const
		{
			return new target_aggro_special(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	class target_aggro_first_redirected : public target // ����Ŀ��Ϊ���������һ�����ʱ������һ���Ŀ��ѡ��Ϊ���������
	{
	public:
		virtual target *Clone() const
		{
			return new target_aggro_first_redirected(*this);
		}
		virtual void GetTarget(policy *self, XID &target);
	};

	/*class targert_random_in_range : public target
	{
		int _range;
	public:
		targert_random_in_range(int range):_range(range)
		{}

		virtual target * Clone() const
		{
			return new targert_random_in_range(*this);
		}
		virtual void GetTarget(policy * self, XID & target);

	};

	class targert_nearest_in_range : public target
	{
		int _range;
	public:
		targert_nearest_in_range(int range):_range(range)
		{}

		virtual target * Clone() const
		{
			return new targert_nearest_in_range(*this);
		}
		virtual void GetTarget(policy * self, XID & target);

	};

class targert_farthest_in_range : public target
	{
		int _range;
	public:
		targert_farthest_in_range(int range):_range(range)
		{}

		virtual target * Clone() const
		{
			return new targert_farthest_in_range(*this);
		}
		virtual void GetTarget(policy * self, XID & target);

	};*/

	//-----------�������еĲ���-----------------------
	class op_attack : public operation
	{
		int _attack_strategy; // �ⲫ �����������ܣ� ����
	public:
		op_attack(int strategy) : _attack_strategy(strategy)
		{
		}

		virtual operation *Clone() const
		{
			return new op_attack(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_skill : public operation
	{
		int _skill_id;
		int _skill_lvl;

	public:
		op_skill(int skill, int level) : _skill_id(skill), _skill_lvl(level)
		{
		}

		virtual operation *Clone() const
		{
			return new op_skill(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_skill_2 : public operation
	{
		int _skill_id;
		int _skill_id_type;
		int _skill_lvl;
		int _skill_lvl_type;

	public:
		op_skill_2(int skill, int st, int level, int lt) : _skill_id(skill), _skill_id_type(st), _skill_lvl(level), _skill_lvl_type(lt)
		{
		}

		virtual operation *Clone() const
		{
			return new op_skill_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_flee : public operation
	{
	public:
		op_flee()
		{
		}

		virtual operation *Clone() const
		{
			return new op_flee(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_create_timer : public operation
	{
		int _timerid;
		int _interval;
		int _count;

	public:
		op_create_timer(int id, int interval, int count) : _timerid(id), _interval(interval), _count(count)
		{
		}

		virtual operation *Clone() const
		{
			return new op_create_timer(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_remove_timer : public operation
	{
		int _timerid;

	public:
		op_remove_timer(int id) : _timerid(id)
		{
		}

		virtual operation *Clone() const
		{
			return new op_remove_timer(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_enable_trigger : public operation
	{
		int _trigger_id;
		bool _is_enable;

	public:
		op_enable_trigger(int id, bool is_enable) : _trigger_id(id), _is_enable(is_enable)
		{
		}

		virtual operation *Clone() const
		{
			return new op_enable_trigger(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_exec_trigger : public operation
	{
		trigger *_trigger;

	public:
		op_exec_trigger(trigger *ptri) : _trigger(ptri)
		{
		}

		op_exec_trigger(const op_exec_trigger &rhs)
		{
			_trigger = new trigger(*rhs._trigger);
		}

		~op_exec_trigger()
		{
			if (_trigger)
				delete _trigger;
		}

		virtual operation *Clone() const
		{
			return new op_exec_trigger(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_say : public operation
	{
		void *_msg;
		unsigned int _size;

	public:
		op_say(const char *str)
		{
			unsigned int len = strlen(str);
			_size = len * 2;
			_msg = abase::fastalloc(_size);
			for (unsigned int i = 0; i < len; i++)
			{
				((char *)_msg)[i * 2] = str[i];
				((char *)_msg)[i * 2 + 1] = 0;
			}
		}

		op_say(const void *msg, unsigned int size)
		{
			_msg = abase::fastalloc(size);
			_size = size;
			memcpy(_msg, msg, size);
		}

		~op_say()
		{
			abase::fastfree(_msg, _size);
		}

		op_say(const op_say &rhs)
		{
			_size = rhs._size;
			_msg = abase::fastalloc(_size);
			memcpy(_msg, rhs._msg, _size);
		}

		virtual operation *Clone() const
		{
			return new op_say(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_say_2 : public operation
	{
		void *_msg;
		unsigned int _size;
		unsigned int _mask;

	public:
		op_say_2(const void *msg, unsigned int size, unsigned int mask)
		{
			_msg = abase::fastalloc(size);
			_size = size;
			_mask = mask;
			memcpy(_msg, msg, size);
		}

		~op_say_2()
		{
			abase::fastfree(_msg, _size);
		}

		op_say_2(const op_say_2 &rhs) : operation(rhs)
		{
			_size = rhs._size;
			_mask = rhs._mask;
			_msg = abase::fastalloc(_size);
			memcpy(_msg, rhs._msg, _size);
		}

		virtual operation *Clone() const
		{
			return new op_say_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	// Novo

	class op_say_new : public operation
	{
		void *_msg;
		unsigned int _size;

	public:
		op_say_new(const char *str)
		{
			unsigned int len = strlen(str);
			_size = len * 2;
			_msg = abase::fastalloc(_size);
			for (unsigned int i = 0; i < len; i++)
			{
				((char *)_msg)[i * 2] = str[i];
				((char *)_msg)[i * 2 + 1] = 0;
			}
		}

		op_say_new(const void *msg, unsigned int size)
		{
			_msg = abase::fastalloc(size);
			_size = size;
			memcpy(_msg, msg, size);
		}

		~op_say_new()
		{
			abase::fastfree(_msg, _size);
		}

		op_say_new(const op_say_new &rhs)
		{
			_size = rhs._size;
			_msg = abase::fastalloc(_size);
			memcpy(_msg, rhs._msg, _size);
		}

		virtual operation *Clone() const
		{
			return new op_say_new(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_say_2_new : public operation
	{
		void *_msg;
		unsigned int _size;
		unsigned int _mask;

	public:
		op_say_2_new(const void *msg, unsigned int size, unsigned int mask)
		{
			_msg = abase::fastalloc(size);
			_size = size;
			_mask = mask;
			memcpy(_msg, msg, size);
		}

		~op_say_2_new()
		{
			abase::fastfree(_msg, _size);
		}

		op_say_2_new(const op_say_2_new &rhs) : operation(rhs)
		{
			_size = rhs._size;
			_mask = rhs._mask;
			_msg = abase::fastalloc(_size);
			memcpy(_msg, rhs._msg, _size);
		}

		virtual operation *Clone() const
		{
			return new op_say_2_new(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_reset_aggro : public operation
	{
	public:
		op_reset_aggro()
		{
		}

		virtual operation *Clone() const
		{
			return new op_reset_aggro(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_swap_aggro : public operation
	{
		unsigned int _index1;
		unsigned int _index2;

	public:
		op_swap_aggro(unsigned int index1, unsigned int index2) : _index1(index1), _index2(index2)
		{
		}

		virtual operation *Clone() const
		{
			return new op_swap_aggro(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_be_taunted : public operation
	{
	public:
		op_be_taunted()
		{
		}

		virtual operation *Clone() const
		{
			return new op_be_taunted(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_fade_target : public operation
	{
	public:
		op_fade_target()
		{
		}

		virtual operation *Clone() const
		{
			return new op_fade_target(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return true;
		}
	};

	class op_aggro_fade : public operation
	{
	public:
		op_aggro_fade()
		{
		}

		virtual operation *Clone() const
		{
			return new op_aggro_fade(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_break : public operation
	{
	public:
		op_break()
		{
		}

		virtual operation *Clone() const
		{
			return new op_break(*this);
		}
		virtual bool DoSomething(policy *self)
		{
			return false;
		}
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_active_spawner : public operation
	{
		bool _is_active_spawner;
		int _ctrl_id;

	public:
		op_active_spawner(int id, bool bStop)
		{
			_ctrl_id = id;
			_is_active_spawner = !bStop;
		}

		virtual operation *Clone() const
		{
			return new op_active_spawner(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy *self);
	};

	class op_active_spawner_2 : public operation
	{
		bool _is_active_spawner;
		int _ctrl_id;
		int _ctrl_id_type;

	public:
		op_active_spawner_2(int id, int itype, bool bStop)
		{
			_ctrl_id = id;
			_ctrl_id_type = itype;
			_is_active_spawner = !bStop;
		}

		virtual operation *Clone() const
		{
			return new op_active_spawner_2(*this);
		}
		virtual bool RequireTarget()
		{
			return false;
		}
		virtual bool DoSomething(policy *self);
	};

	class op_set_common_data : public operation
	{
		int _key;
		int _set_value;
		bool _is_value; // true: ��key��Ӧ��ȫ�ֱ�����Ϊset_value false: ��key��Ӧ��ȫ�ֱ�������Ϊȫ�ֱ���(id=_set_value)��ֵ
	public:
		op_set_common_data(int key, int set_value, bool is_value) : _key(key), _set_value(set_value), _is_value(is_value) {}
		virtual operation *Clone() const
		{
			return new op_set_common_data(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_add_common_data : public operation
	{
		int _key;
		int _add_value;

	public:
		op_add_common_data(int key, int add_value) : _key(key), _add_value(add_value) {}
		virtual operation *Clone() const
		{
			return new op_add_common_data(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_summon_monster : public operation
	{
		int _mob_id;
		int _count;
		int _target_distance; // ��Ŀ��ľ���
		int _remain_time;
		char _die_with_who; // mask, 0x01����leader������0x02����target����
		int _path_id;

	public:
		op_summon_monster(int mob_id, int count, int target_distance, int remain_time, char die_with_who, int path_id) : _mob_id(mob_id), _count(count), _target_distance(target_distance), _remain_time(remain_time), _die_with_who(die_with_who), _path_id(path_id) {}
		virtual operation *Clone() const
		{
			return new op_summon_monster(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_summon_monster_2 : public operation
	{
		int _mob_id;
		int _mob_id_type;
		int _count;
		int _count_type;
		int _target_distance; // ��Ŀ��ľ���
		int _remain_time;
		char _die_with_who; // mask, 0x01����leader������0x02����target����
		int _path_id;
		int _path_id_type;

	public:
		op_summon_monster_2(int mob_id, int mt, int count, int ct, int target_distance, int remain_time, char die_with_who, int path_id, int pt) : _mob_id(mob_id), _mob_id_type(mt), _count(count), _count_type(ct), _target_distance(target_distance), _remain_time(remain_time), _die_with_who(die_with_who), _path_id(path_id), _path_id_type(pt) {}
		virtual operation *Clone() const
		{
			return new op_summon_monster_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_summon_npc : public operation
	{
		int _npc_id;
		int _npc_id_type;
		int _count;
		int _count_type;
		int _target_distance; // ��Ŀ��ľ���
		int _remain_time;
		int _remain_time_type;
		int _path_id;
		int _path_id_type;

	public:
		op_summon_npc(int npc_id, int nt, int count, int ct, int target_distance, int remain_time, int rt, int path_id, int pt) : _npc_id(npc_id), _npc_id_type(nt), _count(count), _count_type(ct), _target_distance(target_distance), _remain_time(remain_time), _remain_time_type(rt), _path_id(path_id), _path_id_type(pt) {}
		virtual operation *Clone() const
		{
			return new op_summon_npc(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_summon_mine : public operation
	{
		int _mine_id;
		int _mine_id_type;
		int _count;
		int _count_type;
		int _remain_time;
		int _remain_time_type;
		int _target_distance; // ��Ŀ��ľ���
	public:
		op_summon_mine(int mine_id, int mt, int count, int ct, int remain_time, int rt, int target_distance) : _mine_id(mine_id), _mine_id_type(mt), _count(count), _count_type(ct), _remain_time(remain_time), _remain_time_type(rt), _target_distance(target_distance) {}
		virtual operation *Clone() const
		{
			return new op_summon_mine(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_change_path : public operation
	{
		int _world_tag;
		int _global_path_id;
		int _path_type;
		char _speed_flag;

	public:
		op_change_path(int world_tag, int global_path_id, int path_type, char speed_flag) : _world_tag(world_tag), _global_path_id(global_path_id), _path_type(path_type), _speed_flag((speed_flag == 2) ? 1 : 0) {}

		virtual operation *Clone() const
		{
			return new op_change_path(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_change_path_2 : public operation
	{
		int _world_tag;
		int _global_path_id;
		int _global_path_id_type;
		int _path_type;
		char _speed_flag;

	public:
		op_change_path_2(int world_tag, int global_path_id, int gptype, int path_type, char speed_flag) : _world_tag(world_tag), _global_path_id(global_path_id), _global_path_id_type(gptype), _path_type(path_type), _speed_flag((speed_flag == 2) ? 1 : 0) {}

		virtual operation *Clone() const
		{
			return new op_change_path_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_play_action : public operation
	{
		char _action_name[128];
		int _play_times;
		int _action_last_time;
		int _interval_time;

	public:
		op_play_action(char action_name[128], int play_times, int action_last_time, int interval_time) : _play_times(play_times), _action_last_time(action_last_time), _interval_time(interval_time)
		{
			memcpy(_action_name, action_name, sizeof(_action_name));
		}
		virtual operation *Clone() const
		{
			return new op_play_action(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_revise_history : public operation
	{
		int _key;
		int _value;

	public:
		op_revise_history(int key, int val) : _key(key), _value(val) {}
		virtual operation *Clone() const
		{
			return new op_revise_history(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_set_history : public operation
	{
		int _key;
		int _value;
		bool _flag;

	public:
		op_set_history(int key, int val, bool flag) : _key(key), _value(val), _flag(flag) {}
		virtual operation *Clone() const
		{
			return new op_set_history(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_deliver_faction_pvp_points : public operation
	{
		int _type;

	public:
		op_deliver_faction_pvp_points(int type) : _type(type) {}
		virtual operation *Clone() const
		{
			return new op_deliver_faction_pvp_points(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_deliver_task : public operation
	{
		int _taskid;
		int _taskidtype;

	public:
		op_deliver_task(int task, int ttype) : _taskid(task), _taskidtype(ttype) {}
		virtual operation *Clone() const
		{
			return new op_deliver_task(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_calc_var : public operation
	{
		int _dest;
		int _dtype;
		int _src1;
		int _stype1;
		int _src2;
		int _stype2;
		int _op;

	public:
		op_calc_var(int d, int dt, int s1, int st1, int s2, int st2, int op) : _dest(d), _dtype(dt), _src1(s1), _stype1(st1), _src2(s2), _stype2(st2), _op(op) {}
		virtual operation *Clone() const
		{
			return new op_calc_var(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_calc_var_2 : public operation
	{
		int _dest;
		int _dtype;
		int _dest1;
		int _dtype1;

	public:
		op_calc_var_2(int d, int dt, int d1, int dt1) : _dest(d), _dtype(dt), _dest1(d1), _dtype1(dt1) {}
		virtual operation *Clone() const
		{
			return new op_calc_var_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_calc_var_3 : public operation
	{
		int _dest;
		int _dtype;
		int _dest1;
		int _dtype1;
		int _dest2;
		int _dtype2;

	public:
		op_calc_var_3(int d, int dt, int d1, int dt1, int d2, int dt2) : _dest(d), _dtype(dt), _dest1(d1), _dtype1(dt1), _dest2(d2), _dtype2(dt2) {}
		virtual operation *Clone() const
		{
			return new op_calc_var_3(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_deliver_random_task_in_region : public operation
	{
		int _task_storageid;
		rect _rect;

	public:
		op_deliver_random_task_in_region(int tsid, float l, float t, float r, float b) : _task_storageid(tsid), _rect(l, t, r, b) {}
		virtual operation *Clone() const
		{
			return new op_deliver_random_task_in_region(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_deliver_task_in_dmglist : public operation
	{
		int _taskid;
		int _ttype;
		int _distance;
		int _count;

	public:
		op_deliver_task_in_dmglist(int tid, int ttype, int dis, int count) : _taskid(tid), _ttype(ttype), _distance(dis), _count(count) {}
		virtual operation *Clone() const
		{
			return new op_deliver_task_in_dmglist(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_clear_tower_task_in_region : public operation
	{
		rect _rect;

	public:
		op_clear_tower_task_in_region(float l, float t, float r, float b) : _rect(l, t, r, b) {}
		virtual operation *Clone() const
		{
			return new op_clear_tower_task_in_region(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_get_sphere_player_count : public operation
	{
		int _radius;
		int _rtype;
		int _dest;
		int _dtype;

	public:
		op_get_sphere_player_count(int r, int rt, int d, int dt) : _radius(r), _rtype(rt), _dest(d), _dtype(dt) {}
		virtual operation *Clone() const
		{
			return new op_get_sphere_player_count(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_get_box_player_count : public operation
	{
		rect _rect;
		int _dest;
		int _dtype;

	public:
		op_get_box_player_count(float l, float t, float r, float b, int d, int dt) : _rect(l, t, r, b), _dest(d), _dtype(dt) {}
		virtual operation *Clone() const
		{
			return new op_get_box_player_count(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_skill_with_talk : public operation
	{
		int _uSkill;
		int _uSkillType;
		int _uLevel;
		int _uLevelType;
		int _uSize;
		char _szData[128];
		int _uAppendDataMask;

	public:
		op_skill_with_talk(int uSkill, int uSkillType, int uLevel, int uLevelType, int uSize, const void * szData, int uAppendDataMask) : _uSkill(uSkill), _uSkillType(uSkillType),
																																 _uLevel(uLevel), _uLevelType(uLevelType), _uSize(uSize), _uAppendDataMask(uAppendDataMask) 
		{
			memcpy(_szData, szData, sizeof(_szData));
		}
		virtual operation *Clone() const
		{
			return new op_skill_with_talk(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_sort_num : public operation
	{
		int _uNum;
		unsigned int _uuNumType;
		unsigned int _varID;
		unsigned int _varType;

	public:
		op_sort_num(int uNum, unsigned int uuNumType, unsigned int varID, unsigned int varType) : _uNum(uNum), _uuNumType(uuNumType), _varID(varID), _varType(varType) {}

		virtual operation *Clone() const
		{
			return new op_sort_num(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_get_pos_num : public operation
	{
		int _uNum;
		unsigned int _uuNumType;
		unsigned int _pos;
		unsigned int _posType;
		unsigned int _varID;
		unsigned int _varType;

	public:
		op_get_pos_num(int uNum, unsigned int uuNumType, unsigned int pos, unsigned int posType, unsigned int varID, unsigned int varType) : _uNum(uNum), _uuNumType(uuNumType), _pos(pos), _posType(posType),
																																			 _varID(varID), _varType(varType) {}

		virtual operation *Clone() const
		{
			return new op_get_pos_num(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_auto_bind_carrier : public operation
	{
		float _range;

	public:
		op_auto_bind_carrier(float range) : _range(range) {}

		virtual operation *Clone() const
		{
			return new op_auto_bind_carrier(*this);
		}

		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_add_range_to_hate_list : public operation
	{
		int _range;
		unsigned int _rangeType;

	public:
		op_add_range_to_hate_list(int range, unsigned int rangetype) : _range(range), _rangeType(rangetype) {}

		virtual operation *Clone() const
		{
			return new op_add_range_to_hate_list(*this);
		}

		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_get_sphere_alive_player_count : public operation
	{
		int _radius;
		int _rtype;
		int _dest;
		int _dtype;

	public:
		op_get_sphere_alive_player_count(int r, int rt, int d, int dt) : _radius(r), _rtype(rt), _dest(d), _dtype(dt) {}
		virtual operation *Clone() const
		{
			return new op_get_sphere_alive_player_count(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_get_box_alive_player_count : public operation
	{
		rect _rect;
		int _dest;
		int _dtype;

	public:
		op_get_box_alive_player_count(float l, float t, float r, float b, int d, int dt) : _rect(l, t, r, b), _dest(d), _dtype(dt) {}
		virtual operation *Clone() const
		{
			return new op_get_box_alive_player_count(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};


	class op_walk_along_3 : public operation
	{
		int iWorldID;
		int iStartPathID;
		int iStartPathIDType;
		int iEndPathID;
		int iEndPathIDType;
		int iPatrolType;
		int iSpeedType;

	public:
		op_walk_along_3(int w, int s, int st, int e, int et, int p, int sp) : iWorldID(w), iStartPathID(s), iStartPathIDType(st),
																			  iEndPathID(e), iEndPathIDType(et), iPatrolType(p), iSpeedType(sp) {}

		virtual operation *Clone() const
		{
			return new op_walk_along_3(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_walk_along_4 : public operation
	{
		int iWorldID;
		int iStartPathID;
		int iStartPathIDType;
		int iEndPathID;
		int iEndPathIDType;
		int iMinPathID;
		int iMinPathIDType;
		int iMaxPathID;
		int iMaxPathIDType;
		int iPatrolType;
		int iSpeedType;
		int iOrientationType;

	public:
		op_walk_along_4(int w, int s, int st, int e, int et, int min, int mint, int max, int maxt, int p, int sp, int o) : iWorldID(w), iStartPathID(s), iStartPathIDType(st), iEndPathID(e), iEndPathIDType(et), iMinPathID(min), iMinPathIDType(mint),
																														   iMaxPathID(max), iMaxPathIDType(maxt), iPatrolType(p), iSpeedType(sp), iOrientationType(o) {}

		virtual operation *Clone() const
		{
			return new op_walk_along_4(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_save_time : public operation
	{
		int _iTimeType;
		int _iParamID;
		int _iParamIDType;

	public:
		op_save_time(int t, int d, int dt) : _iTimeType(t), _iParamID(d), _iParamIDType(dt) {}
		virtual operation *Clone() const
		{
			return new op_save_time(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_random_assignment : public operation
	{
		int _iRandomMin;
		int _iRandomMax;
		int _iQuantity;
		int _iTargetID;

	public:
		op_random_assignment(int min, int max, int quantity, int target) : _iRandomMin(min), _iRandomMax(max), _iQuantity(quantity), _iTargetID(target) {}
		virtual operation *Clone() const
		{
			return new op_random_assignment(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_carrier_voting : public operation
	{
		int _iVoteID;
		int _iVoteIDType;

	public:
		op_carrier_voting(int id, int type) : _iVoteID(id), _iVoteIDType(type) {}
		virtual operation *Clone() const
		{
			return new op_carrier_voting(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};
	class op_voting_result : public operation
	{
		int _iVoteID;
		int _iVoteIDType;
		int _iSelect;
		int _iSelectType;
		int _iSaveResult;
		int _iSaveResultType;

	public:
		op_voting_result(int id, int type, int select, int selectType, int saveResult, int saveResultType) : _iVoteID(id), _iVoteIDType(type), _iSelect(select), _iSelectType(selectType), _iSaveResult(saveResult), _iSaveResultType(saveResultType) {}
		virtual operation *Clone() const
		{
			return new op_voting_result(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};
	class op_voting_show : public operation
	{
		struct POLICY_VOTING_SHOW
		{	
			int unk1;
			int unk2;
		};

		int _iStyle;
		POLICY_VOTING_SHOW _cwData[8];

	public:
		op_voting_show(int style, void * buf) : _iStyle(style)
		{					
		}
		virtual operation *Clone() const
		{
			return new op_voting_show(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_carrier_delivery_task : public operation
	{
		int _iTaskID;
		int _iTaskIDType;

	public:
		op_carrier_delivery_task(int id, int type) : _iTaskID(id), _iTaskIDType(type) {}
		virtual operation *Clone() const
		{
			return new op_carrier_delivery_task(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_carrier_noentry : public operation
	{
		int _iOpen;

	public:
		op_carrier_noentry(int open) : _iOpen(open) {}
		virtual operation *Clone() const
		{
			return new op_carrier_noentry(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_talk_text_2 : public operation
	{
		unsigned int _uSize;
		char _szData[128];
		unsigned int _uAppendDataMask;

		int _iChannelKinds;
		int _iChannelKindsType;
		int _iChannelID;
		int _iChannelIDType;

	public:
		op_talk_text_2(unsigned int uSize, const void * szData,  unsigned int uAppendDataMask , int channelKinds, int channelKindsType, int channelID, int channelIDType) : _uSize(uSize), _uAppendDataMask(uAppendDataMask), _iChannelKinds(channelKinds), _iChannelKindsType(channelKindsType), _iChannelID(channelID), _iChannelIDType(channelIDType) 
		{
			memcpy(_szData, szData, sizeof(_szData));
		}

		virtual operation *Clone() const
		{
			return new op_talk_text_2(*this);
		}

		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_use_skill_3: public operation
	{
		int _uSkill;
		int _uSkillType;
		int _uLevel;
		int _uLevelType;
		int _tmpID;
		int _life;

		public:
		op_use_skill_3(int uSkill, int uSkillType, int uLevel, int uLevelType, int tmpID, int life) : _uSkill(uSkill), _uSkillType(uSkillType), 
		_uLevel(uLevel), _uLevelType(uLevelType),_tmpID(tmpID), _life(life) {}
		virtual operation * Clone() const
		{
			return new op_use_skill_3(*this);
		}
		virtual bool DoSomething(policy * self);			
		virtual bool RequireTarget() {return false;}
	};

	class op_create_timer_2 : public operation
	{
		unsigned int _uID;
		int _iIDType;
		unsigned int _uPeriod;
		int _iPeriodType;
		unsigned int _uCounter;
		int _iCounterType;

	public:
		op_create_timer_2(unsigned int id, int idType, unsigned int period, int periodType, unsigned int counter, int counterType) : _uID(id), _iIDType(idType), _uPeriod(period), _iPeriodType(periodType), _uCounter(counter), _iCounterType(counterType) {}
		virtual operation *Clone() const
		{
			return new op_create_timer_2(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_kill_timer_2 : public operation
	{
		unsigned int _uID;
		int _iIDType;

	public:
		op_kill_timer_2(unsigned int id, int idType) : _uID(id), _iIDType(idType) {}
		virtual operation *Clone() const
		{
			return new op_kill_timer_2(*this);
		}

		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_change_monster_fighting_state : public operation
	{
		int _iChangeState;

	public:
		op_change_monster_fighting_state(int state) : _iChangeState(state) {}
	
		virtual operation *Clone() const
		{
			return new op_change_monster_fighting_state(*this);
		}

		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_change_monster_active_passive : public operation
	{
		int _iChoose;

	public:
		op_change_monster_active_passive(int choose) : _iChoose(choose) {}
		virtual operation *Clone() const
		{
			return new op_change_monster_active_passive(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_child_carrier_parent_monster : public operation
	{
		float _fRange;
		int _iPos;

	public:
		op_child_carrier_parent_monster(float range, int pos) : _fRange(range), _iPos(pos) {}
		virtual operation *Clone() const
		{
			return new op_child_carrier_parent_monster(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_close_child_monster : public operation
	{
		int _iMonsterID;

	public:
		op_close_child_monster(int monsterID) : _iMonsterID(monsterID) {}
		virtual operation *Clone() const
		{
			return new op_close_child_monster(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_deliver_hate_targets : public operation
	{
		int _iMonsterID;

	public:
		op_deliver_hate_targets(int monsterID) : _iMonsterID(monsterID) {}
		virtual operation *Clone() const
		{
			return new op_deliver_hate_targets(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_change_monster_attack_policy : public operation
	{
		int _iAttackPolicyID;

	public:
		op_change_monster_attack_policy(int attackPolicyID) : _iAttackPolicyID(attackPolicyID) {}
		virtual operation *Clone() const
		{
			return new op_change_monster_attack_policy(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return true; }
	};

	class op_specify_failed_task_id : public operation
	{
		int _iTaskID;
		int _iTaskIDType;

	public:
		op_specify_failed_task_id(int taskID, int taskIDType) : _iTaskID(taskID), _iTaskIDType(taskIDType) {}
		virtual operation *Clone() const
		{
			return new op_specify_failed_task_id(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_specify_failed_task_id_regional : public operation
	{
		rect _rect;
		int _iTaskID;
		int _iTaskIDType;

	public:
		op_specify_failed_task_id_regional(float l, float t, float r, float b, int d, int dt) : _rect(l, t, r, b), _iTaskID(d), _iTaskIDType(dt) {}
		virtual operation *Clone() const
		{
			return new op_specify_failed_task_id_regional(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_64 : public operation
	{
		int _ID;
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;
		int _Val12;
		int _Val13;
		int _Val14;
		int _Val15;
		int _Val16;
		int _Val17;
		int _Val18;
		int _Val19;
		int _Val20;

	public:
		op_64(int ID, int Val1, int Val2, int Val3, int Val4, int Val5, int Val6, int Val7, int Val8, int Val9, int Val10, int Val11, int Val12, int Val13, int Val14, int Val15, int Val16, int Val17, int Val18, int Val19, int Val20) : _ID(ID), _Val1(Val1), _Val2(Val2), _Val3(Val3), _Val4(Val4), _Val5(Val5), _Val6(Val6), _Val7(Val7), _Val8(Val8), _Val9(Val9), _Val10(Val10), _Val11(Val11), _Val12(Val12), _Val13(Val13), _Val14(Val14), _Val15(Val15), _Val16(Val16), _Val17(Val17), _Val18(Val18), _Val19(Val19), _Val20(Val20) {}
		virtual operation *Clone() const
		{
			return new op_64(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_65 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;

	public:
		op_65(int Val1, int Val2, int Val3, int Val4, int Val5, int Val6, int Val7, int Val8, int Val9, int Val10, int Val11) : _Val1(Val1), _Val2(Val2), _Val3(Val3), _Val4(Val4), _Val5(Val5), _Val6(Val6), _Val7(Val7), _Val8(Val8), _Val9(Val9), _Val10(Val10), _Val11(Val11) {}
		virtual operation *Clone() const
		{
			return new op_65(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_66 : public operation
	{
		int _val1;
		int _val2;
		int _val3;
		int _val4;
		int _val5;
		int _val6;
		int _val7;
		int _val8;

	public:
		op_66(int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8) : _val1(v1), _val2(v2), _val3(v3), _val4(v4), _val5(v5), _val6(v6), _val7(v7), _val8(v8) {}
		virtual operation *Clone() const
		{
			return new op_66(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_67 : public operation
	{
		int _val1;
		int _val2;
		int _val3;
		int _val4;
		int _val5;
		int _val6;
		int _val7;

	public:
		op_67(int v1, int v2, int v3, int v4, int v5, int v6, int v7) : _val1(v1), _val2(v2), _val3(v3), _val4(v4), _val5(v5), _val6(v6), _val7(v7) {}
		virtual operation *Clone() const
		{
			return new op_67(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_68 : public operation
	{
		int _val1;
		int _val2;
		int _val3;

	public:
		op_68(int v1, int v2, int v3) : _val1(v1), _val2(v2), _val3(v3) {}
		virtual operation *Clone() const
		{
			return new op_68(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_69 : public operation
	{
		int _val1;
		int _val2;

	public:
		op_69(int v1, int v2) : _val1(v1), _val2(v2) {}
		virtual operation *Clone() const
		{
			return new op_69(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_70 : public operation
	{
		int _val1;

	public:
		op_70(int v1) : _val1(v1) {}
		virtual operation *Clone() const
		{
			return new op_70(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_71 : public operation
	{
		int _val1;
		int _val2;

	public:
		op_71(int v1, int v2) : _val1(v1), _val2(v2) {}
		virtual operation *Clone() const
		{
			return new op_71(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_72 : public operation
	{
	public:
		op_72()
		{
		}

		virtual operation *Clone() const
		{
			return new op_72(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};

	class op_73 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_73(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_73(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_74 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;
		int _Val12;
		int _Val13;
		int _Val14;
		int _Val15;
		int _Val16;
		int _Val17;

	public:
		op_74(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11, int val12, int val13, int val14, int val15, int val16, int val17) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10), _Val11(val11), _Val12(val12), _Val13(val13), _Val14(val14), _Val15(val15), _Val16(val16), _Val17(val17) {}
		virtual operation *Clone() const
		{
			return new op_74(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_75 : public operation
	{
		int _Val1;

	public:
		op_75(int val1) : _Val1(val1) {}
		virtual operation *Clone() const
		{
			return new op_75(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_76 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;

	public:
		op_76(int val1, int val2, int val3, int val4) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4) {}
		virtual operation *Clone() const
		{
			return new op_76(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_77 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;

	public:
		op_77(int val1, int val2, int val3, int val4, int val5, int val6) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6) {}
		virtual operation *Clone() const
		{
			return new op_77(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_78 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;

	public:
		op_78(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10) {}
		virtual operation *Clone() const
		{
			return new op_78(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_79 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_79(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_79(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_80 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;

	public:
		op_80(int val1, int val2, int val3, int val4, int val5, int val6) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6) {}
		virtual operation *Clone() const
		{
			return new op_80(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_81 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;

	public:
		op_81(int val1, int val2, int val3, int val4, int val5, int val6) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6) {}
		virtual operation *Clone() const
		{
			return new op_81(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_82 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;

	public:
		op_82(int val1, int val2, int val3, int val4, int val5, int val6, int val7) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7) {}
		virtual operation *Clone() const
		{
			return new op_82(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_83 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;

	public:
		op_83(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10), _Val11(val11) {}
		virtual operation *Clone() const
		{
			return new op_83(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_84 : public operation
	{
	public:
		op_84()
		{
		}

		virtual operation *Clone() const
		{
			return new op_84(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget()
		{
			return false;
		}
	};
	class op_85 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;

	public:
		op_85(int val1, int val2, int val3, int val4, int val5, int val6, int val7) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7) {}
		virtual operation *Clone() const
		{
			return new op_85(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_86 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;

	public:
		op_86(int val1, int val2, int val3) : _Val1(val1), _Val2(val2), _Val3(val3) {}
		virtual operation *Clone() const
		{
			return new op_86(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_87 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;

	public:
		op_87(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10), _Val11(val11) {}
		virtual operation *Clone() const
		{
			return new op_87(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_88 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;

	public:
		op_88(int val1, int val2, int val3) : _Val1(val1), _Val2(val2), _Val3(val3) {}
		virtual operation *Clone() const
		{
			return new op_88(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_89 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;

	public:
		op_89(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10) {}
		virtual operation *Clone() const
		{
			return new op_89(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_90 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;

	public:
		op_90(int val1, int val2, int val3, int val4) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4) {}
		virtual operation *Clone() const
		{
			return new op_90(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_91 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;

	public:
		op_91(int val1, int val2, int val3, int val4, int val5, int val6) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6) {}
		virtual operation *Clone() const
		{
			return new op_91(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_92 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;

	public:
		op_92(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9) {}
		virtual operation *Clone() const
		{
			return new op_92(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_93 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;

	public:
		op_93(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10) {}
		virtual operation *Clone() const
		{
			return new op_93(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_94 : public operation
	{
		bool _Val1;

	public:
		op_94(bool val1) : _Val1(val1) {}
		virtual operation *Clone() const
		{
			return new op_94(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_95 : public operation
	{
		bool _Val1;

	public:
		op_95(bool val1) : _Val1(val1) {}
		virtual operation *Clone() const
		{
			return new op_95(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_96 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_96(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_96(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_97 : public operation
	{
	public:
		op_97() {}
		virtual operation *Clone() const
		{
			return new op_97(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_98 : public operation
	{
		int _Val1;

	public:
		op_98(int val1) : _Val1(val1) {}
		virtual operation *Clone() const
		{
			return new op_98(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_99 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;
		int _Val12;
		int _Val13;
		int _Val14;
		int _Val15;
		int _Val16;
		int _Val17;
		int _Val18;
		int _Val19;
		int _Val20;

	public:
		op_99(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11, int val12, int val13, int val14, int val15, int val16, int val17, int val18, int val19, int val20) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10), _Val11(val11), _Val12(val12), _Val13(val13), _Val14(val14), _Val15(val15), _Val16(val16), _Val17(val17), _Val18(val18), _Val19(val19), _Val20(val20) {}
		virtual operation *Clone() const
		{
			return new op_99(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_100 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_100(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_100(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_101 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_101(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_101(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_102 : public operation
	{
		int _Val1;
		int _Val2;

	public:
		op_102(int val1, int val2) : _Val1(val1), _Val2(val2) {}
		virtual operation *Clone() const
		{
			return new op_102(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_103 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;

	public:
		op_103(int val1, int val2, int val3) : _Val1(val1), _Val2(val2), _Val3(val3) {}
		virtual operation *Clone() const
		{
			return new op_103(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_104 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;
		int _Val6;
		int _Val7;
		int _Val8;
		int _Val9;
		int _Val10;
		int _Val11;
		int _Val12;
		int _Val13;
		int _Val14;
		int _Val15;
		int _Val16;
		int _Val17;
		int _Val18;
		int _Val19;
		int _Val20;
		int _Val21;
		int _Val22;
		int _Val23;
		int _Val24;
		int _Val25;
		int _Val26;

	public:
		op_104(int val1, int val2, int val3, int val4, int val5, int val6, int val7, int val8, int val9, int val10, int val11, int val12, int val13, int val14, int val15, int val16, int val17, int val18, int val19, int val20, int val21, int val22, int val23, int val24, int val25, int val26) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5), _Val6(val6), _Val7(val7), _Val8(val8), _Val9(val9), _Val10(val10), _Val11(val11), _Val12(val12), _Val13(val13), _Val14(val14), _Val15(val15), _Val16(val16), _Val17(val17), _Val18(val18), _Val19(val19), _Val20(val20), _Val21(val21), _Val22(val22), _Val23(val23), _Val24(val24), _Val25(val25), _Val26(val26) {}
		virtual operation *Clone() const
		{
			return new op_104(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};

	class op_105 : public operation
	{
		int _Val1;
		int _Val2;
		int _Val3;
		int _Val4;
		int _Val5;

	public:
		op_105(int val1, int val2, int val3, int val4, int val5) : _Val1(val1), _Val2(val2), _Val3(val3), _Val4(val4), _Val5(val5) {}
		virtual operation *Clone() const
		{
			return new op_105(*this);
		}
		virtual bool DoSomething(policy *self);
		virtual bool RequireTarget() { return false; }
	};
}

#endif
