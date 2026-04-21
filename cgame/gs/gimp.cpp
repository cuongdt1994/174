#include <stdlib.h>
#include "gimp.h"
#include "object.h"
#include "world.h"
#include "usermsg.h"
#include <threadpool.h>
#include <glog.h>


DEFINE_SUBSTANCE_ABSTRACT(gobject_imp,substance,-1)
DEFINE_SUBSTANCE_ABSTRACT(dispatcher,substance,-1)
DEFINE_SUBSTANCE_ABSTRACT(controller,substance,-1)

namespace
{
	class runner_enter
	{
		dispatcher * _runner;
		const A3DVECTOR & _pos;
	public:
		runner_enter(dispatcher * runner,const A3DVECTOR &pos):_runner(runner),_pos(pos){}
		void operator()(slice * pPiece)
		{
			_runner->enter_slice(pPiece,_pos);
		}
	};
	class runner_leave
	{
		dispatcher * _runner;
		const A3DVECTOR & _pos;
	public:
		runner_leave(dispatcher * runner,const A3DVECTOR &pos):_runner(runner),_pos(pos){}
		void operator()(slice * pPiece)
		{
			_runner->leave_slice(pPiece,_pos);
		}
	};
}

void
dispatcher::MoveBetweenSlice(slice * pPiece,slice * pNewPiece,const A3DVECTOR &pos)
{
	_imp->_plane->MoveBetweenSlice(pPiece,pNewPiece,
			runner_enter(this,pos),
			runner_leave(this,pos));
}

bool
gobject_imp::StepMove(const A3DVECTOR &offset)
{
	//�ڵ�������ط�֮ǰ����Ҫ����
	ASSERT(_parent);
	gobject * pObject = _parent;
	slice *pPiece = pObject->pPiece;
	world *pPlane = _plane;
	if(pPiece == NULL)
	{
		//���ɲ��ڸ����ڣ������п��ܵ� ���ǿ�����ƫ��
		//��Ҫ����־
		GLog::log(GLOG_ERR,"�û�%d�ƶ�����",pObject->ID.id);
		ASSERT(false);
		return false;
	}
	
	A3DVECTOR newpos(pObject->pos),oldpos(pObject->pos);
	newpos += offset;
	const grid * pGrid = &pPlane->GetGrid();
	dispatcher *pRunner = _runner;
	//����Ƿ񳬳��� �����������ķ�Χ
	if(pGrid->IsOutsideGrid(newpos.x,newpos.z))
	{
		//������������ı߽���
		return false;
	}

//	if(pPiece->IsEdge())
	if(!pGrid->IsLocal(newpos.x,newpos.z))
	{
		//��ʱ�ķ�����ת�Ƽ���
		int dest= pPlane->GetSvrNear(newpos);
		if( dest < 0) return false;		//�Ҳ�����ȷ�ķ����������Բ���������ƶ�����
		_commander->SwitchSvr(dest,oldpos,newpos,0);	//׼��ת��(player)����ֱ��ת��(NPC,��Ʒ)
		return true;
	}

	ASSERT(pGrid->IsLocal(newpos.x,newpos.z));

	pObject->pos = newpos;
	pRunner->begin_transfer();
	if(pPiece->IsOutside(newpos.x,newpos.z))
	{
		slice *pNewPiece = pGrid->Locate(newpos.x,newpos.z);
		if(pNewPiece == pPiece) {
			pRunner->end_transfer();
			return true;	//��������ǿ��ܵģ����ڼ�������������жϵĲ�һ��
		}
		if(_commander->MoveBetweenSlice(_parent,pPiece,pNewPiece))
		{
			//�ƶ�ʧ�ܣ�ֱ�ӷ���
			GLog::log(GLOG_ERR,"�û�%d��MoveBetweenSliceʱʧ��",pObject->ID.id);

			pObject->pos = oldpos;
			pRunner->end_transfer();
			return true;
		}
		if(pPiece->IsBorder() || pNewPiece->IsBorder())
		{
			//�߽紦�����û��ƶ�������ת���������������ϡ�
			pRunner->notify_move(oldpos,newpos);
		}

		//���������ƶ����͵���Ϣ
		pRunner->MoveBetweenSlice(pPiece,pNewPiece,newpos);
	}
//	pRunner->notify_pos();	//֪ͨplayer�Լ��ĵ�ǰλ��(NPC���������ʵ��)�����ڲ�������Ϊ����Ҫ���Ƶ��
	pRunner->end_transfer();
	return true;
}

void
dispatcher::update_visible_state(unsigned int newstate, unsigned int newstate2, unsigned int newstate3, unsigned int newstate4, unsigned int newstate5, unsigned int newstate6, unsigned int newstate7,unsigned int newstate8,unsigned int newstate9)
{
	packet_wrapper  h1(68);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::update_visible_state>::From(h1,pObject,newstate,newstate2,newstate3,newstate4,newstate5,newstate6,newstate7,newstate8,newstate9);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}

void 
dispatcher::enchant_result(const XID & caster, int skill, char level,bool invader,int at_state,unsigned char section)
{
	packet_wrapper  h1(64);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::enchant_result>::From(h1,pObject, caster, skill, level, invader,at_state,section);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1,-1);
}

void
dispatcher::toggle_invisible(int invisible_degree)
{
	packet_wrapper  h1(16);
	using namespace S2C;
	gobject * pObject = _imp->_parent;
	CMD::Make<CMD::object_invisible>::From(h1,pObject,invisible_degree);
	AutoBroadcastCSMsg(_imp->_plane,pObject->pPiece,h1);
}
