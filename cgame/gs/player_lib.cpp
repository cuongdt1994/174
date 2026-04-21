//---------------------------------------------------------------------------------------------------------------------------
//--PW LIB MAGAGER (C) DeadRaky / Fulano
//---------------------------------------------------------------------------------------------------------------------------

#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include <db_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "usermsg.h"
#include "public_quest.h"
#include "luamanager.h"
#include "player_lib.h"


void gplayer_lib_items::Init()
{
	memset(this,0x00,sizeof(gplayer_lib_items));
}

unsigned int gplayer_lib_items::Rebuild()
{
	this->count = 0;
	ITEM v[MAX_LOT];
	memset(v,0x00,MAX_LOT * sizeof(ITEM));
	
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (item[i].id)
		{
			v[this->count++] = item[i];
		}
	}
	memcpy(item,v,MAX_LOT * sizeof(ITEM));
	return this->count;
}

unsigned int gplayer_lib_items::Add(unsigned int id , unsigned int count)
{
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (item[i].id == id)
		{
			item[i].count += count;
			break;
		}
		if(!item[i].id)
		{
			item[i].id = id;
			item[i].count = count;
			break;
		}
	}
	return Rebuild();
}

unsigned int gplayer_lib_items::Del(unsigned int id)
{
	unsigned int iCount = 0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (item[i].id == id)
		{
			iCount = item[i].count;
			unsigned int iMaxCount = world_manager::GetDataMan().get_item_pile_limit(item[i].id);
			
			if (iCount > iMaxCount)
			{
				item[i].count -= iMaxCount;
				iCount = iMaxCount;
			}
			else
			{
				item[i].id = 0;
			}
			Rebuild();
			break;
		}
	}
	return iCount;
}

unsigned int gplayer_lib_items::GetCleanSlot()
{
	unsigned int res = 0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if ( !item[i].id ) res++;
	}
	return res;
}

void gplayer_lib_items::GetInfo( unsigned int & cnt , void * buf )
{
	cnt = Rebuild();
	memcpy(buf,item, (MAX_LOT * sizeof(ITEM)) );
}