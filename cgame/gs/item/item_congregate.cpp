#include "../clstab.h"
#include "../world.h"
#include "../worldmanager.h"
#include "../player_imp.h"
#include "item_congregate.h"

DEFINE_SUBSTANCE(congregate_item,item_body, CLS_ITEM_CONGREGATE)

int 
congregate_item::OnUse(item::LOCATION l, gactive_imp * imp, unsigned int count)
{
	gplayer_imp* pImp = (gplayer_imp*)imp;
	DATA_TYPE dt;
	CONGREGATE_ESSENCE * ess = (CONGREGATE_ESSENCE *)world_manager::GetDataMan().get_data_ptr(_tid,ID_SPACE_ESSENCE,dt);
	ASSERT(ess && dt == DT_CONGREGATE_ESSENCE);
	
	int world_tag = world_manager::GetWorldTag();
	int index = -1;
	for(int i=0; (unsigned int)i<sizeof(ess->area)/sizeof(ess->area[0]); i++)
	{
		if(ess->area[i].id <= 0) break;
		if(ess->area[i].id == world_tag)
		{
			index = i;
			break;
		}
	}
	if(index == -1) return -1;
	if(pImp->_basic.level < ess->area[index].require_level) return -1;
	
	//�������������������ĸ�����
	if(ess->congregate_type == gplayer_imp::CONGREGATE_TYPE_TEAM)
	{
	 	if(!pImp->TeamCongregateRequest(ess->area[index].require_reply_level, ess->area[index].require_reply_level2, ess->area[index].require_reply_reincarnation_times))
		 	return -1;
		GLog::log(GLOG_INFO,"���%dʹ���˶��鼯����%d", pImp->_parent->ID.id, _tid);
	}
	else if(ess->congregate_type == gplayer_imp::CONGREGATE_TYPE_FACTION)
	{
	 	if(!pImp->FactionCongregateRequest(ess->area[index].require_reply_level, ess->area[index].require_reply_level2, ess->area[index].require_reply_reincarnation_times))
		 	return -1;
		GLog::log(GLOG_INFO,"���%dʹ���˰��ɼ�����%d", pImp->_parent->ID.id, _tid);
	}
	
	return 1;
}

