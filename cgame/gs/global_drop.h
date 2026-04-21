#ifndef __ONLINE_GAME_GLOBAL_DROP_H__
#define __ONLINE_GAME_GLOBAL_DROP_H__

#include <cstring>
#include <unordered_map>
#include "hashtab.h"
#include "template/extra_drop_table.h"
#include <arandomgen.h>

class drop_template
{
public:
	struct drop_node
	{
		int id;
		float probability;
		drop_node(int id, float prob):id(id),probability(prob){}
	};
	typedef abase::vector<drop_node> DROP_LIST;

	struct drop_entry
	{
		int type;	// 1 完全替换；2 同时生效
		float drop_num_probability[8]; 
		DROP_LIST drop_list;
	};

protected:

	static abase::hashtab<drop_entry*,int, abase::_hash_function > _drop_table;
public:
	
	static bool LoadDropList();
	static inline drop_entry * GetDropList(int id)
	{
		drop_entry ** pTmp =  _drop_table.nGet(id);
		if(pTmp) return *pTmp;
		return NULL;
	}

	static int GenerateItem(drop_entry * pEntry ,int * list,unsigned int size)
	{
		unsigned int count = abase::RandSelect(pEntry->drop_num_probability,8);
		if(count == 0) return 0;
		if(count > size) count = size;
		float * prob = &(pEntry->drop_list[0].probability);
		unsigned int prob_size = pEntry->drop_list.size();
		for(unsigned int i = 0; i < count; i ++)
		{
			int index = abase::RandSelect(prob,sizeof(drop_node),prob_size);
			int id = pEntry->drop_list[index].id;
			list[i] = id;
		}
		return count;
	}
};


#endif

