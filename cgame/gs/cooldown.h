#ifndef __ONLINE_GAME_GS_COOLDOWN_H__
#define __ONLINE_GAME_GS_COOLDOWN_H__

#include <sys/time.h>
#include "staticmap.h"
#include "world.h"

class cd_manager
{
	struct cd_ent
	{
		timeval t;
		int	interval;
		cd_ent(){}
		cd_ent(const timeval & t1, int i):t(t1),interval(i){}
	};
	
	typedef abase::static_multimap<int,cd_ent, abase::fast_alloc<> >  COOLDOWN_LIST;
	COOLDOWN_LIST _list;
public:
	cd_manager()
	{
		_list.reserve(5);
	}

	void Swap(cd_manager & cd)
	{
		_list.swap(cd._list);
	}

	bool TestCoolDown(int id)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it != _list.end())
		{
			timeval tv;
			g_timer.get_systime(tv);
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec) 
				return false;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				return false;
			_list.erase(it);
		}
		return true;
	}


	bool SetCoolDown(int id, int cooldown_msec)
	{
		if(cooldown_msec <= 0) return true;
		//������ȴ�ĵ���ʱ��
		timeval tv;
		g_timer.get_systime(tv);
		int sec = cooldown_msec / 1000;
		int usec = (cooldown_msec % 1000)*1000;
		tv.tv_sec += sec;
		if((tv.tv_usec += usec) > 1000000)
		{
			tv.tv_sec += 1;
			tv.tv_usec -= 1000000;
		}

		COOLDOWN_LIST::iterator it = _list.find(id);
		if(it == _list.end())
		{
			_list[id] = cd_ent(tv,cooldown_msec);
		}
		else
		{
			it->second.t = tv;
			it->second.interval = cooldown_msec;
		}
		return true;
	}

	int CoolDownReduceAtr(int id, int reduction_msec)
	{
		COOLDOWN_LIST::iterator it = _list.find(id);

		timeval tv;
		g_timer.get_systime(tv);
		int sec = reduction_msec / 1000;
		int usec = (reduction_msec % 1000)*1000;
		tv.tv_sec += sec;
		if((tv.tv_usec += usec) > 1000000)
		{
			tv.tv_sec += 1;
			tv.tv_usec -= 1000000;
		}

		if (it == _list.end())
		{
			return 0; // cooldown não encontrado
		}
		else
		{
			int new_cooldown_msec = it->second.interval - reduction_msec;
			if (new_cooldown_msec < 0)
			{
				new_cooldown_msec = 0;
			}
			
			it->second.t = tv;
			it->second.interval = new_cooldown_msec;
			
			return new_cooldown_msec;
		}
	}

	void ClearExpiredCoolDown()
	{
		timeval tv;
		g_timer.get_systime(tv);
		COOLDOWN_LIST::iterator it = _list.end();
		while(it != _list.begin())
		{
			--it;
			timeval & ct = it->second.t;
			if(ct.tv_sec > tv.tv_sec)  
				continue;
			else if(ct.tv_sec == tv.tv_sec && ct.tv_usec > tv.tv_usec) 
				continue;
			_list.erase(it);
		}
	}

	void Save(archive & ar);
	void Load(archive & ar);
	void DBSave(archive & ar);
	void DBLoad(archive & ar);

	void SaveForClient(archive &ar)
	{	
		unsigned short count = _list.size();
		ar << count;

		timeval tv;
		g_timer.get_systime(tv);
		COOLDOWN_LIST::iterator it = _list.begin();
		for(;it != _list.end();++it)
		{
			int t = 0;
			if(it->second.t.tv_sec < tv.tv_sec)
			{
			}
			else if(it->second.t.tv_sec == tv.tv_sec && it->second.t.tv_usec < tv.tv_usec) 
			{
			}
			else 
			{
				t = (it->second.t.tv_sec - tv.tv_sec)*1000 + (it->second.t.tv_usec - tv.tv_usec)/1000;
			}
			ar << (unsigned short)(it->first);
			ar << t;
			ar << it->second.interval;
		}
	}
	
	bool SetAllCoolDown(int id1, int id2, int cooldown_msec, int except_id)
	{
		if(cooldown_msec > 0)
		{
			timeval tv;
			g_timer.get_systime(tv);
			tv.tv_sec += cooldown_msec / 1000;
			tv.tv_usec += 1000 * (cooldown_msec % 1000);
			if(tv.tv_usec > 1000000)
			{
				++tv.tv_sec;
				tv.tv_usec -= 1000000;
			}
			COOLDOWN_LIST::iterator it = _list.begin();
			for(;it != _list.end();++it)
			{
				if ( it->first >= id1 && (id2 < 0 || it->first <= id2) && except_id != it->first )
				{
					it->second.t.tv_sec = tv.tv_sec;
					it->second.t.tv_usec = tv.tv_usec;
					it->second.interval = cooldown_msec;
				}
			}
		}
		return true;
	}
	
	
};


#endif

