#ifndef __ONLINE_GAME_GS_GENERALCARD_SET_MAN_H__
#define __ONLINE_GAME_GS_GENERALCARD_SET_MAN_H__

#include <unordered_map>

struct generalcard_set
{
	int id;				//�佫����װģ��ID
	unsigned int total_count;	//��װ�п�������
	float enhance;		//��װ��������������������	
	generalcard_set():id(0),total_count(0),enhance(0.f){}
};

class itemdataman;
class generalcard_set_manager
{
	typedef std::unordered_map<int, generalcard_set *> MAP;
	MAP _map;
	
	generalcard_set_manager(){}

	bool __Insert(const int * cardidlist, unsigned int count, generalcard_set * cardset)
	{
		int num = 0;
		for(unsigned int i = 0; i < count ; i ++)
		{
			int id = cardidlist[i];
			if(id > 0)
			{
				generalcard_set * tmp = _map[id];
				if(tmp) return false; //�ظ���id
				_map[id] = cardset;
				num ++;
			}
		}
		return num > 0;
	}
	
	generalcard_set * __Get(int cardid)
	{
		MAP::iterator it = _map.find(cardid);
		if(it == _map.end()) return NULL;
		return it->second;
	}

public:
	static generalcard_set_manager & GetInstance()
	{
		static generalcard_set_manager instance;
		return instance;
	}

	static bool Insert(const int * cardidlist, unsigned int count, generalcard_set * cardset)
	{
		return GetInstance().__Insert(cardidlist, count, cardset);
	}

	static generalcard_set * Get(int cardid)
	{
		return GetInstance().__Get(cardid);
	}

	static bool LoadTemplate(itemdataman & dataman);
};

#endif
