#include <stdlib.h>
#include "actobject.h"
#include "actsession.h"
#include "npcsession.h"
#include "world.h"
#include "npc.h"
#include "ainpc.h"
#include "aipolicy.h"
#include <arandomgen.h>
#include"clstab.h"
#include "skill_filter.h"
#include <common/protocol.h>
#include "pathfinding/pathfinding.h"
#include "invincible_filter.h"


DEFINE_SUBSTANCE(session_npc_attack,session_normal_attack,CLS_SESSION_NPC_ATTACK)
DEFINE_SUBSTANCE(session_npc_range_attack,session_npc_attack,CLS_SESSION_NPC_RANGE_ATTACK)
DEFINE_SUBSTANCE(session_npc_keep_out,act_session,CLS_SESSION_NPC_KEEP_OUT)
DEFINE_SUBSTANCE(session_npc_delay,act_session,CLS_SESSION_NPC_DELAY)
DEFINE_SUBSTANCE(session_npc_flee,session_npc_keep_out,CLS_SESSION_NPC_FLEE)
DEFINE_SUBSTANCE(session_npc_silent_flee,session_npc_flee,CLS_SESSION_NPC_SILENT_FLEE)
DEFINE_SUBSTANCE(session_npc_follow_target,act_session,CLS_SESSION_NPC_FOLLOW_TARGET)
DEFINE_SUBSTANCE(session_npc_empty,act_session,CLS_SESSION_NPC_EMPTY)
DEFINE_SUBSTANCE(session_npc_cruise,act_session,CLS_SESSION_NPC_CRUISE)
DEFINE_SUBSTANCE(session_npc_skill,act_session,CLS_SESSION_NPC_SKILL)
DEFINE_SUBSTANCE(session_npc_follow_master,session_npc_follow_target,CLS_SESSION_NPC_FOLLOW_MASTER)
DEFINE_SUBSTANCE(session_npc_regeneration,act_session,CLS_SESSION_NPC_REGENERATION);
DEFINE_SUBSTANCE(session_npc_patrol,act_session,CLS_SESSION_NPC_PATROL);


bool 
session_npc_attack::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));

	bool rst = session_normal_attack::StartSession(hasmorecmd);
	if(!rst) return rst;
	NPCSessionStart(_ai_task_id);
	return rst;
}

bool
session_npc_attack::RepeatSession()
{
	if(_attack_times)
	{
		if(--_attack_times <= 0) return false;
	}
	
	float dis;
	int flag;
	A3DVECTOR pos;
	if(!_imp->CheckAttack(_target,&flag,&dis,pos))
	{
		return false;
	}
	//�����������
	if(dis < _short_range)
	{
		return false;
	}
	int interval = _imp->_cur_prop.attack_speed;
	if(interval <=0) {
		ASSERT(false);
		interval = 34;
	}
	if(interval != _attack_speed)
	{
		_attack_speed = interval;
		ChangeInterval(_attack_speed);
	}
	((gnpc_controller*)(_imp->_commander))->RefreshAggroTimer(_target);
	_imp->DoAttack(_target,_force_attack);
	return true;
}

bool
session_npc_attack::EndSession()
{
	if(_session_id >= 0) 
	{
		NPCSessionEnd(_ai_task_id,0);
		_session_id = -18;
		RemoveTimer();
	}
	return true;
}


session_npc_follow_target::~session_npc_follow_target()
{
	if(_agent)
	{
		delete _agent;
	}
}

float 
session_npc_follow_target::GetSpeed()
{
	return _imp->_cur_prop.run_speed;
}

bool 
session_npc_follow_target::StartSession(bool hasmorecmd)
{
	//�����в����Ͳ������ˣ���Ϊ�������޷�������ȷ�Ĵ�����
	if(hasmorecmd) return false;
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_target.id != -1);
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	_speed = (unsigned short)(GetSpeed() * 256.0f + 0.5f);

	Run();
	SetTimer(g_timer,(int)(20 * NPC_FOLLOW_TARGET_TIME+0.1f),0);
	NPCSessionStart(_ai_task_id);
	return true;
}

bool 
session_npc_follow_target::RepeatSession()
{
	if(--_timeout <= 0) return false;
	return Run();
}

bool 
session_npc_follow_target::EndSession()
{
	if(_session_id >= 0)
	{
		//����NPCֹͣ�������ʱ   
		if(!_imp->IsRootMode() && !_stop_flag) 
		{
			char mode = ((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>();
			_imp->_runner->stop_move(_imp->_parent->pos,_speed,1,mode | C2S::MOVE_MODE_RUN);
		}

		NPCSessionUpdateChaseInfo(_ai_task_id,_chase_info);
		NPCSessionEnd(_ai_task_id,_retcode);
		_session_id = -19;
		RemoveTimer();
	}
	return true;
}

void 
session_npc_follow_target::TrySendStop()
{
	if(!_imp->IsRootMode() && !_stop_flag) 
	{
		_stop_flag = true;
		_imp->_runner->stop_move(_imp->_parent->pos,_speed,1,
				((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>() | C2S::MOVE_MODE_RUN);
	}
}

int 
session_npc_follow_target::Run()
{
#define TEST_GETTOGOAL()  if(_agent->GetToGoal()) \
	{\
		if(++_reachable_count>= 3) \
		{\
			_retcode = NSRC_ERR_PATHFINDING; \
			return 0; \
		}\
		else\
		{\
			TrySendStop();\
			return 1;\
		}\
	}
	
	float range;
	world::object_info info;
	if(!_imp->_plane->QueryObject(_target,info)
			|| (info.state & world::QUERY_OBJECT_STATE_ZOMBIE)
			|| (range= info.pos.squared_distance(_imp->_parent->pos)) >= _range_max)
	{
		// ׷ѰĿ��ʧ��
		_retcode = NSRC_OUT_OF_RANGE;
		return 0;
	}
	if(_imp->IsRootMode()) return 0;

	if(range < _range_min)
	{
		_retcode = 0;
		return 0;
	}
	info.pos.y += _height_offset;

	const A3DVECTOR & tmpPos = _imp->_parent->pos;
	float speed = GetSpeed() * NPC_FOLLOW_TARGET_TIME;
	bool first_call = false;
	if(!_agent)
	{
		first_call = true;
		_agent = new path_finding::follow_target();
		_agent->CreateAgent(_imp->_plane,((gnpc_imp*)_imp)->_inhabit_mode,NPC_MOVE_BEHAVIOR_CHASE);
		_agent->Start(tmpPos,info.pos,speed,_range_target,range,&_chase_info);
		TEST_GETTOGOAL();
	}
	else
	{
		bool is_knocked_back = ((gnpc_imp*)_imp)->TestKnockBackFlag();
		if(_agent->GetToGoal() || is_knocked_back) 
		{
			//����϶�û�дﵽ�ܹ�������Ҫ����Ϊǰ���жϹ���
			_agent->Start(tmpPos,info.pos,speed,_range_target*0.6f,range,&_chase_info);
			//����Ƿ�ɴ���ǣ�������Ÿ߶Ȳ� 
			//���������ôӦ�ð����޷�����������
			TEST_GETTOGOAL();
		}
		else
		{
			//������Ҫ�ж�һ���Ƿ�Ŀ���ƶ����࣬��������Ҫ���¼���λ��
			const A3DVECTOR & oldtarget = _agent->GetTarget();
			float x = oldtarget.x - info.pos.x;
			float z = oldtarget.z - info.pos.z;
			float dis = x*x + z*z;
			if( dis > 49.f || (dis > 16.f && !_agent->IsBlocked()))
			{
				//���½���Start
				_agent->Start(tmpPos,info.pos,speed,_range_target,range,&_chase_info);
				TEST_GETTOGOAL();
			}
		}
	}

	// ���ϵ��ƶ���
	if(!_agent->MoveOneStep(speed))
	{
		if(first_call) return 1;
		_retcode = NSRC_ERR_PATHFINDING;
		return 0;
	}
	
	// �ƶ���õ�����λ�ã��������ͻ��ˣ� Ӧ�����ж�һ���Ƿ�����ƶ���
	A3DVECTOR targetpos;
	_agent->GetCurPos(targetpos);	
	
	A3DVECTOR offset= targetpos;
	offset -= _imp->_parent->pos;
	if(offset.squared_magnitude() < 1e-3)
	{
		//�þ����ж��Ƿ������������ƶ�
		TrySendStop();
		return 1;
	}
	_stop_flag = false;
	
	if(_imp->StepMove(offset)) 
	{
		_imp->_runner->move(targetpos, 
				(int)(NPC_FOLLOW_TARGET_TIME*1000.f + 0.1f),
				(unsigned short)(GetSpeed()* 256.0f + 0.5f),
				((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>() | C2S::MOVE_MODE_RUN); 
	}
	return 1;

#undef TEST_GETTOGOAL
}

//------------------------------------------------------------------------------
bool
session_npc_range_attack::RepeatSession()
{
	float range;
	int flag;
	A3DVECTOR pos;
	if(!_imp->CheckAttack(_target,&flag,&range,pos))
	{
		//�޷�������������
		return false;
	}

	((gnpc_controller*)(_imp->_commander))->RefreshAggroTimer(_target);
		
	//float attack_range = _imp->_cur_prop.attack_range + info.body_size;
	//attack_range *= attack_range;
	if(_auto_interrupt)
	{
		//if(range > attack_range*(0.3f*0.3f) && range < attack_range * (0.6f*0.6f) || range > attack_range)
		if(range < _attack_range * (0.6f*0.6f) || range > _attack_range)
		{
			return false;
		}
	}
	else
	{
		if(range > _attack_range) return false;
	}

	//���Ʊ�Ҫ���������
	if(range < _short_range) return false;
	
	_imp->DoAttack(_target,_force_attack);
	return true;
}



session_npc_keep_out::~session_npc_keep_out()
{
	if(_agent) delete _agent;
}

bool 
session_npc_keep_out::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_target.id != -1);
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	
	Run();
	//������ζ�Ҫ��ʼ 
	SetTimer(g_timer,(int)(20*NPC_FLEE_TIME + 0.1f),0);
	NPCSessionStart(_ai_task_id);
	return true;
}

bool 
session_npc_keep_out::RepeatSession()
{
	if(_timeout <= 0 || _stop_flag) return false;
	return Run();
}

bool 
session_npc_keep_out::EndSession()
{
	if(_session_id >= 0)
	{
		//����NPCֹͣ�������ʱ   
		if(!_stop_flag && !_imp->IsRootMode())
		{
			_imp->_runner->stop_move(_imp->_parent->pos, 
					(unsigned short)(GetSpeed()* 256.0f + 0.5f),1,
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_RUN);
		}

		NPCSessionEnd(_ai_task_id,_retcode);
		_session_id = -20;
		RemoveTimer();
	}
	return true;
}

float 
session_npc_keep_out::GetSpeed()
{
	return _imp->_cur_prop.run_speed;
}

bool
session_npc_keep_out::Run()
{
	world::object_info info;
	if(!_imp->_plane->QueryObject(_target,info)
			|| (info.state & world::QUERY_OBJECT_STATE_ZOMBIE))
	{
		// ׷ѰĿ��ʧ��
		_retcode = -1;
		return false;
	}
	float range;
	if((range= info.pos.squared_distance(_imp->_parent->pos)) >= _range*_range)
	{
		_retcode = 0;
		return false;
	}

	if(_imp->IsRootMode()) return 0;

	A3DVECTOR pos = info.pos;
	const A3DVECTOR & tmpPos = _imp->_parent->pos;
	float speed = GetSpeed() * NPC_FLEE_TIME;
	if(!_agent)
	{
		_agent = new path_finding::keep_out();
		_agent->CreateAgent(_imp->_plane,((gnpc_imp*)_imp)->_inhabit_mode);
		_agent->Start(tmpPos,info.pos,speed,_range);
		if(_agent->GetToGoal()) 
		{
			_retcode = 0;
			return false;
		}
	}
	else
	{
		bool is_knocked_back = ((gnpc_imp*)_imp)->TestKnockBackFlag();
		if(_agent->GetToGoal() || is_knocked_back) 
		{
			//׷���Ļ�����Ȼ˵���ˣ��Ǿ͵��˰�:)
			_retcode = 0;
			return false;
		}

		//�������õ��˵�λ��
		_agent->SetFleePos(info.pos,_range);
	}

	// ���ϵ��ƶ���
	_agent->MoveOneStep(speed);
	
	// �ƶ���õ�����λ�ã��������ͻ��ˣ� Ӧ�����ж�һ���Ƿ�����ƶ���
	A3DVECTOR targetpos;
	_agent->GetCurPos(targetpos);	
	
	A3DVECTOR offset= targetpos;
	offset -= tmpPos;
	if(_imp->StepMove(offset)) 
	{
		if(_agent->GetToGoal())
		{
			_stop_flag = true;
			_imp->_runner->stop_move(targetpos, 
					(unsigned short)(speed * 256.0f + 0.5f),1,
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_RUN);
			//ֻ����һ��stop����
			_retcode = 0;
			return false;
		}
		else
		{
			_stop_flag = false;
			_imp->_runner->move(targetpos, 
					(int)(1000.f * NPC_FLEE_TIME +0.1f),
					(unsigned short)(speed * 256.0f + 0.5f),
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_RUN);
			//C2S::MOVE_MODE_RUN); 
		}
	}
	return true;
}


bool 
session_npc_flee::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_target.id != -1);
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	
	Run();
	SetTimer(g_timer,(int)(20 * NPC_FLEE_TIME + 0.1f),0);
	NPCSessionStart(_ai_task_id);
	return true;
}

bool 
session_npc_flee::RepeatSession()
{
	if(--_timeout <= 0) return false;
	if(_timeout & 0x01 == 0)
	{
		//ÿ�������һ��
		//��һ����������
	}
	return Run();
}

bool
session_npc_silent_flee::RepeatSession()
{
	if(!(_imp->GetSealMode() & gactive_imp::SEAL_MODE_SILENT)) 
	{
		_retcode = 0;
		return false;
	}
	return session_npc_flee::RepeatSession();
}

bool 
session_npc_delay::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	
	SetTimer(g_timer,20,0);
	NPCSessionStart(_ai_task_id);
	return true;
}

bool 
session_npc_delay::RepeatSession()
{
	if(--_timeout <= 0) return false;
	return true;
}

bool 
session_npc_delay::EndSession()
{
	if(_session_id >= 0)
	{
		NPCSessionEnd(_ai_task_id,0);
		_session_id = -21;
		RemoveTimer();
	}
	return true;
}


session_npc_cruise::~session_npc_cruise()
{
	if(_agent)
	{
		delete _agent;
	}
}


bool 
session_npc_cruise::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	ASSERT(_timeout >0);
	_timeout ++;
	Run();
	SetTimer(g_timer,20,0);
	NPCSessionStart(_ai_task_id);
	return true;
}


bool 
session_npc_cruise::EndSession()
{
	if(_session_id >= 0)
	{
		//����������跢��ֹͣ�ƶ�����Ϣ����Ϊ�����Ѿ�����
		if(!_stop_flag && !_imp->IsRootMode()) 
		{
			_imp->_runner->stop_move(_imp->_parent->pos,
				(unsigned short)(GetSpeed()* 256.0f + 0.5f),1,
				((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_WALK);
				//C2S::MOVE_MODE_RUN); 
		}

		//_imp->_runner->stop_move(_imp->_parent->pos,_speed,1,0);

		NPCSessionEnd(_ai_task_id,0);
		_session_id = -22;
		RemoveTimer();
	}
	return true;
}

bool 
session_npc_cruise::RepeatSession()
{
	if(--_timeout < 0) return false;
	return Run();
}

float
session_npc_cruise::GetSpeed()
{
//������ȷ���ٶ��趨 ���У�ˮ�е�
	return _imp->_cur_prop.walk_speed;
}

bool 
session_npc_cruise::Run()
{
	float speed = GetSpeed();
	if(!_agent)
	{
		_agent = new path_finding::cruise();
		_agent->CreateAgent(_imp->_plane,((gnpc_imp*)_imp)->_inhabit_mode);
		_agent->Start(_imp->_parent->pos,_center,speed,_range);
	}
	if(_agent->GetToGoal())
	{
		return false;
	}
	
	_agent->MoveOneStep(speed);

	// �ƶ���õ�����λ�ã��������ͻ��ˣ� Ӧ�����ж�һ���Ƿ�����ƶ���
	A3DVECTOR targetpos;
	_agent->GetCurPos(targetpos);	
	_stop_flag = false;
	
	A3DVECTOR offset = targetpos;
	offset -= _imp->_parent->pos;

	if(offset.squared_magnitude() < 1e-3)
	{
		if(!_stop_flag)
		{
			_imp->_runner->stop_move(targetpos, 
					(unsigned short)(speed * 256.0f + 0.5f),1,
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_WALK);
		}
		_stop_flag = true;
		//ֻ����һ��stop����
		return !_agent->GetToGoal();
	}

	if(_imp->StepMove(offset)) 
	{
		if(_agent->GetToGoal())
		{
			_stop_flag = true;
			_imp->_runner->stop_move(targetpos, 
					(unsigned short)(speed * 256.0f + 0.5f),1,
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_WALK);
			//ֻ����һ��stop����
			return false;
		}
		else
		{
			_imp->_runner->move(targetpos, 
					(int)(1000.f + 0.1f),
					(unsigned short)(speed * 256.0f + 0.5f),
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>()|C2S::MOVE_MODE_WALK);
			//C2S::MOVE_MODE_RUN); 
		}
	}
	return true;

}



bool 
session_npc_skill::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	_imp->_session_state = gactive_imp::STATE_SESSION_USE_SKILL;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	
	if(_use_cooldown)
	{
		//��鼼����ȴ 
		int cd_id = GNET::SkillWrapper::GetCooldownID(_skill_id);

		if(!_imp->CheckCoolDown(cd_id))
		{
			//��������ȴ�� ͨ��һ��
			_imp->NotifySkillStillCoolDown(cd_id);
			_end_flag = true;
			NPCSessionStart(_ai_task_id);
			SendEndMsg(_self_id);
			return false;
		}
	}
		
	if(_use_mp)
	{
		//���mp�Ƿ��㹻	
		int mp_cost = GNET::SkillWrapper::GetMpCost(_skill_id, object_interface(_imp), _skill_level);	
		if(mp_cost > 0 && _imp->_basic.mp < mp_cost)
		{
			_end_flag = true;
			NPCSessionStart(_ai_task_id);
			SendEndMsg(_self_id);
			return false;
		}
	}

	((gnpc_controller*)(_imp->_commander))->RefreshAggroTimer(_target);

	if(_target.IsActive())
	{
		//����Ƿ��淨��
		int type = GNET::SkillWrapper::GetType(_skill_id);
		if(type == 1 || type == 3)
		{
			_imp->Notify_StartAttack(_target, 0);
		}
	}

	int next_interval = 0;
	int interval = _imp->NPCStartSkill(_skill_id,_skill_level,_target,next_interval);
	
	timeval tv;
	gettimeofday(&tv,NULL);
	__PRINTF("npc use skill.................%d %d %d at %d.%06d\n",_skill_id, interval,next_interval,tv.tv_sec,tv.tv_usec);
	if(interval < 0)
	{
		__PRINTF("npc cannot use skill %d\n",_skill_id);
		return false;
	}
	if(interval == 0)
	{
		__PRINTF("npc ˲������\n");
		_imp->NPCEndSkill(_skill_id,_skill_level,_target);
		return false;
	}
	
	NPCSessionStart(_ai_task_id);

	next_interval /= 50;
	interval /=50;
	if(next_interval <= 0) next_interval = 20;
	SetTimer(g_timer,next_interval,2,interval);

	//ע��һ��filter
	_imp->_filters.AddFilter(new skill_interrupt_filter(_imp,_session_id,FILTER_INDEX_SKILL_SESSION));
	return true;
}

bool 
session_npc_skill::RepeatSession()
{
	if(_end_flag) return false;	//����

	_imp->NPCEndSkill(_skill_id,_skill_level,_target);
	_end_flag = true; //�ڶ��β������Repeat����������ֱ���˳�

	//���³�޾���
	((gnpc_controller*)(_imp->_commander))->RefreshAggroTimer(_target);

	//�Ƚ������жϵ�filterɾ��
	_imp->_filters.RemoveFilter(FILTER_INDEX_SKILL_SESSION);
	return true;	
}

bool 
session_npc_skill::EndSession()
{
	if(_session_id >= 0)
	{
		__PRINTF("npc skill session end \n");
		RemoveTimer();
		_imp->_filters.RemoveFilter(FILTER_INDEX_SKILL_SESSION);
		NPCSessionEnd(_ai_task_id,0);
		_session_id = -23;
		RemoveTimer();
	}
	return true;
}

bool 
session_npc_skill::OnAttacked()
{
	ASSERT(_session_id == _imp->GetCurSessionID());
	if(_imp->NPCSkillOnAttacked(_skill_id,_skill_level))
	{
		_end_flag = true;
		RemoveTimer();
		SendEndMsg(_self_id);
		return true;
	}
	return false;
}

float 
session_npc_follow_master::GetSpeed()
{
	return _imp->_cur_prop.walk_speed;
}


bool 
session_npc_regeneration::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;
	SetTimer(g_timer,20,0);
	NPCSessionStart(_ai_task_id);
	_imp->_filters.AddFilter(new invincible_filter(_imp,FILTER_INVINCIBLE,5));
	return true;
}
void 
session_npc_regeneration::OnTimer(int index,int rtimes)
{
	SendForceRepeat(_self_id);
}

bool 
session_npc_regeneration::EndSession()
{
	NPCSessionEnd(_ai_task_id,0);
	//�����޵е�filter���Զ���ʱ����
	return true;
}

bool 
session_npc_regeneration::RepeatSession()
{
	if(--_timeout <= 0) return false;
	int max_hp = _imp->_cur_prop.max_hp;
	if(_imp->IsCombatState())
	{
		//ս��״̬���ֶ���Ѫ
		_imp->Heal(max_hp >> 1); 
	}
	if(_imp->_basic.hp >= max_hp) return false ;
	return true;
}

float 
session_npc_patrol::GetSpeed()
{
	return _is_run?_imp->_cur_prop.run_speed:_imp->_cur_prop.walk_speed;
}

bool 
session_npc_patrol::StartSession(bool hasmorecmd)
{
	ASSERT(_imp->_commander->GetRunTimeClass()->IsDerivedFrom(CLASSINFO(gnpc_controller)));
	ASSERT(_imp->_session_state == gactive_imp::STATE_SESSION_IDLE);
	
	_imp->_session_state = gactive_imp::STATE_SESSION_MOVE;
	_session_id = _imp->GetNextSessionID();
	_self_id = _imp->_parent->ID;

	_imp->CheckNPCData();
	Run();
	_imp->CheckNPCData();
	SetTimer(g_timer,(int)(20 * NPC_PATROL_TIME+0.1f),0);
	_imp->CheckNPCData();
	NPCSessionStart(_ai_task_id);
	_imp->CheckNPCData();
	return true;
}

bool 
session_npc_patrol::RepeatSession()
{
	if(--_timeout <= 0) return false;
	_imp->CheckNPCData();
	return Run();
}

bool 
session_npc_patrol::EndSession()
{
	if(_session_id >= 0)
	{
		//����NPCֹͣ�������ʱ   
		if(!_imp->IsRootMode() && !_stop_flag) 
		{
		//��ʱֹͣ����stop
			char mode = ((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>();
			_imp->_runner->stop_move(_imp->_parent->pos,
					(unsigned short)(GetSpeed()* 256.0f + 0.5f),1,
					mode | _is_run ?C2S::MOVE_MODE_RUN : C2S::MOVE_MODE_WALK);
		}

		NPCSessionEnd(_ai_task_id,_retcode);
		_session_id = -24;
		RemoveTimer();
	}
	return true;
}

int 
session_npc_patrol::Run()
{
#define TEST_GETTOGOAL()  if(_agent->GetToGoal()) \
	{\
		if(++_reachable_count>= 3) \
		{\
			_retcode = NSRC_ERR_PATHFINDING; \
			return 0; \
		}\
		else\
		{\
			TrySendStop();\
			return 1;\
		}\
	}
	
	_imp->CheckNPCData();
	const A3DVECTOR & tmpPos = _imp->_parent->pos;
	float speed = GetSpeed()*NPC_PATROL_TIME;
	if(!_agent)
	{
		_agent = new path_finding::follow_target();
		_agent->CreateAgent(_imp->_plane,((gnpc_imp*)_imp)->_inhabit_mode,NPC_MOVE_BEHAVIOR_CHASE);
		_agent->Start(tmpPos,_target,speed,0.8,15,NULL);
		_imp->CheckNPCData();
	}

	_imp->CheckNPCData();
	if(tmpPos.squared_distance(_target) <= 1.44*speed*speed)
	{
		if(_path_agent)
		{
			bool first_end = false;
			if(_path_agent->GetNextWayPoint(_target, first_end))
			{
				_agent->Start(tmpPos,_target,speed,1.8,15,NULL);
				_imp->CheckNPCData();
			}
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	else
	if(_agent->GetToGoal())
	{
		_imp->CheckNPCData();
		if(_path_agent)
		{
			bool first_end = false;
			if(_path_agent->GetNextWayPoint(_target, first_end))
			{
				_agent->Start(tmpPos,_target,speed,0.8,15,NULL);
				_imp->CheckNPCData();
			}
			else
				return 0;
		}
		else
		{
			return 0;
		}
	}
	

	_imp->CheckNPCData();
	// ���ϵ��ƶ���
	if(!_agent->MoveOneStep(speed))
	{
		_retcode = NSRC_ERR_PATHFINDING;
		return 0;
	}
	_imp->CheckNPCData();
	
	// �ƶ���õ�����λ�ã��������ͻ��ˣ� Ӧ�����ж�һ���Ƿ�����ƶ���
	A3DVECTOR targetpos;
	_agent->GetCurPos(targetpos);	
	_imp->CheckNPCData();
	
	A3DVECTOR offset= targetpos;
	offset -= _imp->_parent->pos;
	if(offset.squared_magnitude() < 1e-3)
	{
		//�þ����ж��Ƿ������������ƶ�
		if(!_imp->IsRootMode() && !_stop_flag) 
		{
			_stop_flag = true;
			_imp->_runner->stop_move(targetpos,
					(unsigned short)(GetSpeed()* 256.0f + 0.5f),1,
					((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>() |_is_run ?C2S::MOVE_MODE_RUN : C2S::MOVE_MODE_WALK);
		}
		_imp->CheckNPCData();
		return 1;
	}
	_imp->CheckNPCData();

	_stop_flag = false;
	if(_imp->StepMove(offset)) 
	{
		_imp->_runner->move(targetpos, 
				(int)(NPC_PATROL_TIME*1000.f + 0.1f),
				(unsigned short)(GetSpeed()* 256.0f + 0.5f),
				((gnpc_imp*)_imp)->GetMoveModeByInhabitType<0>() |_is_run ?C2S::MOVE_MODE_RUN : C2S::MOVE_MODE_WALK);
	}
	_imp->CheckNPCData();
	return 1;

#undef TEST_GETTOGOAL
}

session_npc_patrol::~session_npc_patrol()
{
	if(_agent) delete _agent;
}

