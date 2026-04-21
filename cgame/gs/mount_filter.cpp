#include "mount_filter.h"
#include "clstab.h"
#include "arandomgen.h"
#include "actobject.h"
#include "world.h"

DEFINE_SUBSTANCE(mount_filter,filter,CLS_FILTER_MOUNT)

void 
mount_filter::AdjustDamage(damage_entry&, const XID &, const attack_msg&,float)
{
	//ÿ�α����ж���һ�����ʵ�����
	if(abase::Rand(0.f,1.f) < _drop_rate)
	{
		//����������ٻس������Ϣ
		//_is_deleted = true;
		_parent.GetImpl()->SendTo<0>(GM_MSG_PLAYER_RECALL_PET,_parent.GetSelfID(),0);
	}
}

void 
mount_filter::OnAttach()
{
	//1:�������״̬ 
	_parent.ActiveMountState(_mount_id,_mount_color);

	//2:�����ƶ��ٶ�
	_parent.EnhanceOverrideSpeed(_speedup);
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();
}

void 
mount_filter::OnRelease()
{
	//1:�뿪���״̬
	_parent.DeactiveMountState();

	//2:�޸��ƶ��ٶ�
	_parent.ImpairOverrideSpeed(_speedup);
	_parent.UpdateSpeedData();
	_parent.SendClientCurSpeed();
}
	
bool 
mount_filter::Save(archive & ar)
{
	filter::Save(ar);
	ar << _mount_id << _mount_color << _speedup << _drop_rate;
	return true;
}

bool 
mount_filter::Load(archive & ar)
{
	filter::Load(ar);
	ar >> _mount_id >> _mount_color >> _speedup >> _drop_rate;
	return true;
}

void 
mount_filter::Merge(filter * f)
{
	mount_filter * pFilter = substance::DynamicCast<mount_filter>(f);
	ASSERT(pFilter);
	if(!pFilter) return ;
	
	//��������
	_mount_id = pFilter->_mount_id;
	_mount_color = pFilter->_mount_color;

	//�ٶ��и��ģ����½��м���
	if(_speedup != pFilter->_speedup)
	{
		_parent.ImpairOverrideSpeed(_speedup);
		_speedup = pFilter->_speedup;
		_parent.EnhanceOverrideSpeed(_speedup);
		_parent.UpdateSpeedData();
		_parent.SendClientCurSpeed();
	}

	_drop_rate = pFilter->_drop_rate;
}

