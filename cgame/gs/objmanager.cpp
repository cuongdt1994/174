#include "world.h"

bool 
extern_object_manager::Init()
{
	int rst = SetTimer(g_timer,20*15,0);
	ASSERT(rst >=0);
	return true;

}

void 
extern_object_manager::Run()
{
	//每10秒一次heartbeat
	//估计一个服务的量为1000左右
	ONET::Thread::Mutex::Scoped keeper(_lock);
	OBJECT_MAP::iterator it = _map.begin();
	for(;it != _map.end(); )
	{
		object_entry & ent = it->second;
		if(--ent.ttl <= 0) 
		{
			__PRINTF("object %x removed\n",ent.id);
			_map.erase(it++); 
		}
		else
		{
			++it;
		}
	}
}

void 
extern_object_manager::OnTimer(int index,int rtimes)
{
	ONET::Thread::Pool::AddTask(this);
}

int extern_object_manager::GetState(int state)
{
	return state ? world::QUERY_OBJECT_STATE_ZOMBIE : world::QUERY_OBJECT_STATE_ACTIVE;
}

int extern_object_manager::GetWorldIndex() 
{ 
	return world_manager::GetWorldIndex(); 
}
