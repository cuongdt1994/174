#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include "world.h"
#include "player_imp.h"
#include "usermsg.h"
#include "clstab.h"
#include "actsession.h"
#include <common/protocol_imp.h>
#include "luamanager.h"

DEFINE_SUBSTANCE(gplayer_controller,controller,CLS_PLAYER_CONTROLLER)

//just test

void 
gplayer_controller::Release(bool free_parent)
{
	//�����free_parent���������д�����һ�ɰ���true������
	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gactive_imp * pImp = (gactive_imp*)_imp;
	pImp->ClearSession();
	slice * pPiece = pPlayer->pPiece;
	world * pPlane = _imp->_plane;
	if(pPiece)
	{
		pPlane->RemovePlayer(pPlayer);
	}

	pPlane->RemovePlayerToMan(pPlayer);
	pPlane->UnmapPlayer(pPlayer->ID.id);
	
	//�ͷ�˳�������������Ϊimp����������ܻ�ʹ��_imp��_runner
	dispatcher * runner = _imp->_runner;
	delete _imp;
	delete runner;
	delete this;
	pPlayer->imp = NULL;
	pPlane->FreePlayer(pPlayer);
}

gplayer_controller::~gplayer_controller()
{
	if(_gm_auth) delete _gm_auth;
}

gplayer_controller::gplayer_controller():_cur_target(-1,-1),_cur_sub_target(-1,-1),_target_refresh_state(false),_load_stats(0),_peep_counter(0),_select_counter(0),_debug_command_enable(false),_banish_counter(0),_move_cmd_seq(0),_gm_auth(NULL)
{
	memset(_deny_cmd_list,0,sizeof(_deny_cmd_list));
	_pickup_counter = MAX_PICKUP_PER_SECOND;	//Ĭ����5�����
	_pickup_timestamp = g_timer.get_systime();
	_safe_lock_timestamp = 0;
	_max_safe_lock = 0;
}
	
void 
gplayer_controller::Init(gobject_imp * imp)
{
	controller::Init(imp);
	_last_pos = _imp->_parent->pos;
}

int 
gplayer_controller::MoveBetweenSlice(gobject * obj,slice * src, slice * dest)
{
	ASSERT(obj == _imp->_parent);
	return src->MovePlayer((gplayer*)obj,dest);
}

bool 
gplayer_controller::Save(archive & ar)
{
	_load_stats += 5;
	ar << _cur_target << _cur_sub_target << _target_refresh_state << _load_stats << _last_pos << _peep_counter 
		<< _debug_command_enable << _banish_counter << _move_cmd_seq <<_peep_counter << _safe_lock_timestamp << _max_safe_lock;
	ar.push_back(_deny_cmd_list,sizeof(_deny_cmd_list));
	if(_gm_auth)
	{
		char buf[PRIVILEGE_NUM + 1];
		int authsize = _gm_auth->GetBuf(buf,sizeof(buf));
		ASSERT(authsize >= 0);
		if(authsize <0) authsize = 0;
		ar << authsize;
		ar.push_back(buf,authsize);
	}
	else
	{
		ar << (int)0;
	}
	return true;
}
bool 
gplayer_controller::Load(archive & ar)
{
	ar >> _cur_target >> _cur_sub_target >> _target_refresh_state >> _load_stats >> _last_pos >> _peep_counter
		>> _debug_command_enable >> _banish_counter >> _move_cmd_seq >>_peep_counter >> _safe_lock_timestamp >> _max_safe_lock;;
	ar.pop_back(_deny_cmd_list,sizeof(_deny_cmd_list));
	int authsize;
	ar >> authsize;
	ASSERT(authsize >=0);
	ASSERT(authsize <= PRIVILEGE_NUM);
	if(authsize > 0 && authsize <= PRIVILEGE_NUM)
	{
		char buf[PRIVILEGE_NUM + 1];
		ar.pop_back(buf,authsize);
		SetPrivilege(buf,authsize);
	}
	return true;
}

void 
gplayer_controller::SwitchSvr(int dest, const A3DVECTOR & oldpos, const A3DVECTOR &newpos,const instance_key * switch_key)
{
	return _imp->SwitchSvr(dest,oldpos,newpos,switch_key);
}


void
gplayer_controller::error_cmd(int msg)
{
//	ASSERT(msg != S2C::ERR_FATAL_ERR);
	return _imp->_runner->error_message(msg);
}

void 
gplayer_controller::OnHeartbeat(unsigned int tick)
{
	LuaManager * lua = LuaManager::GetInstance();
	gplayer_imp *pImp =(gplayer_imp*)_imp;

	if(_safe_lock_timestamp > 0)
	{
		if(--_safe_lock_timestamp <= 0)
		{
			_imp->_runner->notify_safe_lock( InSafeActive(), 0, _max_safe_lock,InSafeLock());
		}
	}
	

	//�����Լ������ݻ���Ѫֵ�����������
	slice * pPiece = pImp->_parent->pPiece;
	if(pPiece && pPiece->IsBorder())
	{
		if(pImp->CheckServerNotify<0>())
		{
			extern_object_manager::SendAppearMsg<0>(_imp->_plane,(gplayer*)pImp->_parent,pPiece);
		}
		else if(pImp->_is_moved || pImp->_refresh_state)
		{
			extern_object_manager::SendRefreshMsg<0>(_imp->_plane,pImp->_parent,pImp->_basic.hp,pPiece);
		}
		
	}

	//�����Ƿ����Լ���info00
	if(pImp->_refresh_state)
	{
		pImp->_runner->query_info00();
	}

	//���Է���С�����Ԫ�� �� buffӰ�� lgc
	if(pImp->_cur_elf_info.id != (unsigned int)-1)
	{
		if(pImp->_cur_elf_info.refresh_vigor)
		{
			pImp->_runner->query_elf_vigor();
			pImp->_cur_elf_info.refresh_vigor = false;
		}
		if(pImp->_cur_elf_info.refresh_enhance)
		{
			pImp->_runner->query_elf_enhance();
			pImp->_cur_elf_info.refresh_enhance = false;
		}
	}

	if(_cur_target.id != -1 && _cur_target.id != ((gplayer*)pImp->_parent)->ID.id) // ��Ŀ������Լ�ΪĿ�궼����鳬ʱ
	{
		_select_counter ++;
		world::object_info info;
		if(_select_counter > 30 		//30�볬ʱ
				|| !pImp->_plane->QueryObject(_cur_target,info) 
				|| info.pos.horizontal_distance(_imp->_parent->pos) >125.f*125.f
				|| ( (info.invisible_degree > ((gplayer*)pImp->_parent)->anti_invisible_degree || info.object_state & gactive_object::STATE_FORBIDBESELECTED)
					&& !(_cur_target.IsPlayer() && pImp->IsInTeam() && pImp->IsMember(_cur_target)))
					&& _cur_target.id != ((gplayer*)pImp->_parent)->ID.id
					&& _cur_target.id != pImp->_petman.GetCurPet().id)
		{
			UnSelect(true);
		}
		else
		{
		}
	}
	TryPeepMobs();

	if(_target_refresh_state)
	{
		pImp->NotifyTargetChange(_cur_target);
		_target_refresh_state = false;
	}

	if(_banish_counter >0)
	{
		_banish_counter --;
	}
	//���и��ظ��µĹ���
	int need_load_state = lua->GetConfig()->need_load_state;
	int max_load_state = lua->GetConfig()->max_load_state;
	
	if((_load_stats -= need_load_state)<0)
	{
		_load_stats = need_load_state;
	}

	if(_load_stats >= max_load_state)
	{
		if(!_banish_counter)
		{
			GLog::log(GLOG_INFO,"���%d����ʱ����\n",_imp->_parent->ID.id);
			error_cmd(S2C::ERR_YOU_HAS_BEEN_BANISHED);
		}
		//�����������	
		//�����˱�ģʽ
		_banish_counter = 15;
	}

	return ;
}

void 
gplayer_controller::OnResurrect()
{
	_peep_counter = -PLAYER_REBORN_PROTECT;
}

void
gplayer_controller::TryPeepMobs()
{
	if(_peep_counter < 0)
	{
		++_peep_counter;
		return ;
	}
	gplayer_imp * pImp = (gplayer_imp *) _imp;
	if(pImp->_runner->is_gm_invisible()) return;
	if(pImp->_parent->IsZombie()) return;
	bool is_peep = false;
	if(pImp->_is_moved)
	{
		if(_last_pos.squared_distance(pImp->_parent->pos) >= 2.f*2.f )
		{
			pImp->_is_moved = false;
			is_peep = true;
		}
		else
		{
			//���Կ����ñ�ķ������������ֹ㲥�ķ���
			is_peep = ((++_peep_counter) >= 3);
			pImp->_is_moved = false;
		}
	}
	else
	{
		is_peep = ((++_peep_counter) >= 3);
	}

	if(is_peep)
	{
		_peep_counter = 0;
		_last_pos = pImp->_parent->pos;
		
		gplayer * pPlayer = (gplayer*)pImp->_parent;
		MSG msg;
		msg_watching_t mwt={pImp->_basic.level,pImp->GetFaction(),pPlayer->invisible_degree};
		BuildMessage(msg,GM_MSG_WATCHING_YOU,XID(GM_TYPE_NPC,-1),pPlayer->ID,pPlayer->pos,0,&mwt,sizeof(mwt));
		float tmp = world_manager::GetMaxMobSightRange();
		world *pPlane = pImp->_plane;
		pPlane->BroadcastMessage(msg,tmp,gobject::MSG_MASK_PLAYER_MOVE);
	}

}

void TTTTT(gactive_imp *pImp)
{
	gplayer * pPlayer  = (gplayer*)pImp->_parent;
	MSG msg;
	msg_watching_t mwt={pImp->_basic.level,pImp->GetFaction(),pPlayer->invisible_degree};
	BuildMessage(msg,GM_MSG_WATCHING_YOU,XID(GM_TYPE_NPC,-1),pPlayer->ID,pPlayer->pos,0,&mwt,sizeof(mwt));
	float tmp = world_manager::GetMaxMobSightRange();
	world *pPlane = pImp->_plane;
	pPlane->BroadcastMessage(msg,tmp,gobject::MSG_MASK_PLAYER_MOVE);
}

void gplayer_controller::UnSelect(bool force)
{
	if(!force && ((gplayer_imp*)_imp)->GetPlayerLimit(PLAYER_LIMIT_NOCHANGESELECT)) return;
	if(_cur_target.type != -1)
	{
		((gplayer_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,_cur_target,0);
		_cur_target.type = -1;
		_cur_target.id = -1;
	}
	_select_counter = 0;
	_target_refresh_state = true;
	((gactive_imp*)_imp)->SetRefreshState();
	_imp->_runner->unselect();
}

void gplayer_controller::SelectTarget(int id, bool force)
{
	if(!force && ((gplayer_imp*)_imp)->GetPlayerLimit(PLAYER_LIMIT_NOCHANGESELECT)) return;
	if(id == -1)
	{
		UnSelect();
		return ;
	}
	if(id == _cur_target.id) 
	{
		_imp->_runner->player_select_target(id);
		return;
	}

	XID target;
	MAKE_ID(target,id);
	//��ѯ�����Ƿ���ں;����Ƿ��Զ

	gplayer * pPlayer = (gplayer*)_imp->_parent;
	gplayer_imp * pImp = (gplayer_imp *)_imp;
	world::object_info info;
	if(!_imp->_plane->QueryObject(target,info))
	{
		//������ѡ������
		((gplayer_dispatcher*)_imp->_runner)->object_is_invalid(id);
		return;
	}

	if(info.pos.squared_distance(_imp->_parent->pos) >150.f*150.f)
	{
		return;
	}

	if((info.invisible_degree > pPlayer->anti_invisible_degree || info.object_state & gactive_object::STATE_FORBIDBESELECTED)
			&& !(target.IsPlayer() && pImp->IsInTeam() && pImp->IsMember(target))	//���Ƕ��� 
			&& target.id != pPlayer->ID.id											//�����Լ�
			&& target.id != pImp->_petman.GetCurPet().id)							//�����Լ��ĳ���
	{
		return;
	}

	_select_counter = 0;
	if(_cur_target.id != -1)
	{
		((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,_cur_target,0);
	}
	_cur_target = target;
	_target_refresh_state = true;
	 ((gactive_imp*)_imp)->SetRefreshState();
	//��������
	link_sid ld;
	ld.cs_id = pPlayer->cs_index;
	ld.cs_sid = pPlayer->cs_sid;
	ld.user_id = pPlayer->ID.id;
	((gactive_imp*)_imp)->SendTo<0>(GM_MSG_SUBSCIBE_TARGET,target,0,&ld,sizeof(ld));
	_imp->_runner->player_select_target(target.id);
}

void gplayer_controller::SubscibeConfirm(const XID & who)
{
	if(who != _cur_target)
	{
		((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_TARGET,who,0);
	}
	else
	{
		_select_counter = 0;
	}
}

void gplayer_controller::SecondSubscibeConfirm(const XID & who)
{
	if(who != _cur_sub_target)
	{
		((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_SUBTARGET,who,0);
	}
}

void gplayer_controller::SelectSubTarget(const XID & target, int targettarget)
{
	if (target == _cur_target)
	{
		if (targettarget == -1)
		{
			if (_cur_sub_target.id != -1)
			{
				((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_SUBTARGET,_cur_sub_target,0);
			}
			_cur_sub_target = XID(-1, -1);
		}
		else
		{
			XID id;
			MAKE_ID(id, targettarget);
			if (_cur_sub_target == id) return;
			if (_cur_sub_target.id != -1)
			{
				((gactive_imp*)_imp)->SendTo<0>(GM_MSG_UNSUBSCIBE_SUBTARGET,_cur_sub_target,0);
			}
			_cur_sub_target = id;

			gplayer * pPlayer = (gplayer*)_imp->_parent;
			link_sid ld;
			ld.cs_id = pPlayer->cs_index;
			ld.cs_sid = pPlayer->cs_sid;
			ld.user_id = pPlayer->ID.id;
			((gactive_imp*)_imp)->SendTo<0>(GM_MSG_SUBSCIBE_SUBTARGET, _cur_sub_target, 0, &ld, sizeof(ld));
		}
	}
}

void 
gplayer_controller::ReInit()
{
	gactive_object * pParent= (gactive_object*)(_imp->_parent);
	if(_gm_auth)
		pParent->object_state |= gactive_object::STATE_GAMEMASTER;
	else
		pParent->object_state &= ~gactive_object::STATE_GAMEMASTER;
}

void gplayer_controller::SetPrivilege(const void * data, unsigned int size)
{
//	ASSERT(!_gm_auth);
	gactive_imp * pImp = (gactive_imp*)_imp;
	if(size && data)
	{
		_gm_auth = new GNET::Privilege();
		_gm_auth->Init(data,size);
		
		if(pImp && pImp->_parent) ((gactive_object*)pImp->_parent)->object_state |= gactive_object::STATE_GAMEMASTER;
	}
	else
	{
		if(_gm_auth)
		{
			delete _gm_auth;
			_gm_auth = NULL;
		}
		if(pImp && pImp->_parent) ((gactive_object*)pImp->_parent)->object_state &= ~gactive_object::STATE_GAMEMASTER;
	}
}


bool 
gplayer_controller::HasGMPrivilege()
{
	return  _gm_auth;
}

void 
gplayer_controller::DenyCmd(unsigned int cmd_type)
{
	if(cmd_type >= CMD_MAX) return;
	_deny_cmd_list[cmd_type] ++;
}

void 
gplayer_controller::AllowCmd(unsigned int cmd_type)
{
	if(cmd_type >= CMD_MAX) return;
	_deny_cmd_list[cmd_type] --;
	if(_deny_cmd_list[cmd_type] < 0)
	{
		_deny_cmd_list[cmd_type]  = 0;
		GLog::log(LOG_ERR,"�û�%d AllowCmd�������� %d",_imp->_parent->ID.id, cmd_type);
	
	}
}

