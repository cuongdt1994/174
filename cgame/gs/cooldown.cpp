#include "cooldown.h"

#pragma pack(1)
struct fix_ent
{
	unsigned int tv_sec;
	unsigned int tv_usec;
	int	interval;
};
#pragma pack()

void cd_manager::DBSave(archive & ar)
{
	ar << -1;
	unsigned int count = _list.size();
	ar << count;
	
	COOLDOWN_LIST::iterator it = _list.begin();
	while(it != _list.end())
	{
		ar << it->first;
		fix_ent ent;
		ent.tv_sec = it->second.t.tv_sec;
		ent.tv_usec = it->second.t.tv_usec;
		ent.interval = it->second.interval;
		
		ar.push_back(&ent,sizeof(ent));
		++it;
	}
}

void cd_manager::DBLoad(archive & ar)
{
	int version;
	ar >> version;
	if(version != -1) return;		//直接跳过,这个只会影响数据库存储
	unsigned int count;
	ar >> count;
	
	if ( ar.size() == ((sizeof(int) + sizeof(unsigned int)) + ((sizeof(int)+sizeof(fix_ent)) * count)) )
	{
		//printf("cd_manager::Load: ver=%d , count = %d \n", version , count );
		
		for(unsigned int i = 0; i < count; i++)
		{
			int id;
			fix_ent ent;
			ar >> id;
			ar.pop_back(&ent,sizeof(ent));
			
			_list[id].t.tv_sec = ent.tv_sec;
			_list[id].t.tv_usec = ent.tv_usec;
			_list[id].interval = ent.interval;
			
			//printf("cd_manager::Load: ent_size = %lld \n", sizeof(ent) );
			//printf("cd_manager::Load: id=%lld , ent.t = %lld, ent.interval = %lld \n", id , ent.t , ent.interval );
		}
	}
	else
	{
		printf("cd_manager::Load: ar_size=%d != size=%d \n", ar.size() , ((sizeof(int) + sizeof(unsigned int)) + ((sizeof(int)+sizeof(fix_ent)) * count)) );
	}
}
	
void cd_manager::Save(archive & ar)
{
	int version = -1;
	ar << version;
	unsigned int count = _list.size();
	ar << count;
	
	COOLDOWN_LIST::iterator it = _list.begin();
	while(it != _list.end())
	{
		ar << it->first;
		ar.push_back(&(it->second),sizeof(cd_ent));
		++it;
	}
}

void cd_manager::Load(archive & ar)
{
	int version;
	ar >> version;
	if(version != -1) return;		//直接跳过,这个只会影响数据库存储
	unsigned int count;
	ar >> count;
	
	for(unsigned int i = 0; i < count; i++)
	{
		int id;
		cd_ent ent;
		ar >> id;
		ar.pop_back(&ent,sizeof(ent));
		_list[id] = ent;
	}
}



