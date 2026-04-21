//---------------------------------------------------------------------------------------------------------------------------
//--PW LOTTERY MAGAGER (C) DeadRaky 2022
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
#include "player_lottery.h"

LotteryConfig * LotteryConfig::instance = NULL;
TreasureConfig * TreasureConfig::instance = NULL;

void TreasureConfig::Init()
{
	memset(this,0x00,sizeof(TreasureConfig));

	DATA_TYPE dt;
	LOTTORY_TICKET_COST_CONFIG *it = (LOTTORY_TICKET_COST_CONFIG*)world_manager::GetDataMan().get_data_ptr(2313,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_LOTTORY_TICKET_COST_CONFIG)
	{
		req.id = it->require_item_id;
		req.count = it->require_item_count;
		for(unsigned int i = 0; i < MAX_COST; i++)
		{
			req.cost[i] = it->cost[i];
		}
	}
	else
	{
		return;
	}
	
	DATA_TYPE dts;
	LOTTORY_TICKET_STORAGE_CONFIG *its = (LOTTORY_TICKET_STORAGE_CONFIG*)world_manager::GetDataMan().get_data_ptr(2312,ID_SPACE_CONFIG,dts);
	if (its && dts == DT_LOTTORY_TICKET_STORAGE_CONFIG)
	{
		for(unsigned int i = 0; i < MAX_LOT; i++)
		{
			lot[i].prob = its->list[i].probability;
			lot[i].id = its->list[i].id;
			lot[i].rank = its->list[i].level;
			lot[i].count = its->list[i].count;
		}
	}
	else
	{
		return;
	}
	
	enable = true;
}

bool TreasureConfig::GetItem(unsigned int & id, unsigned int & count, unsigned int & rank)
{
	if (enable)
	{
		unsigned int it = abase::RandSelect(&lot[0].prob, sizeof(LOT), MAX_LOT);
		id = lot[it].id;
		count = lot[it].count;
		rank = lot[it].rank;
		return true;
	}
	return false;
}

void LotteryConfig::Init()
{
	memset(this,0x00,sizeof(LotteryConfig));
	
	DATA_TYPE dt;
	NEW_LOTTERY_CONFIG *it = (NEW_LOTTERY_CONFIG*)world_manager::GetDataMan().get_data_ptr(6064,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_NEW_LOTTERY_CONFIG)
	{
		for(unsigned int i = 0; i < MAX_PROB; i++)
		{
			prob[i] = it->probability_level[i];
		}
		
		for(unsigned int i = 0; i < MAX_AWARD; i++)
		{
			award[i].id = it->award[i].item_id;
			award[i].score = it->award[i].point;
			award[i].count = it->award[i].item_count;
		}
		
		for(unsigned int i = 0; i < MAX_RANK; i++)
		{
			for(unsigned int j = 0; j < MAX_LOT; j++)
			{
				lot[i][j].prob = it->items[i][j].probability;
				lot[i][j].id = it->items[i][j].id;
				lot[i][j].count = it->items[i][j].count;
				lot[i][j].rank = it->items[i][j].rank;
			}
		}
		
		enable = true;
	}
}

unsigned int LotteryConfig::GetProb()
{
	if (enable)
	{
		return abase::RandSelect(prob,MAX_PROB);
	}
}

bool LotteryConfig::GetAward( unsigned int score, unsigned int & level, unsigned int & item, unsigned int & count )
{
	for(unsigned int i = level; i < MAX_AWARD; i++)
	{
		if ( award[i].id && score >= award[i].score )
		{
			level++;
			item = award[i].id;
			count = award[i].count;
			return true;
		}
	}
	return false;
}

bool LotteryConfig::GetLot( unsigned int i, unsigned int & id, unsigned int & count, unsigned int & rank )
{
	if (enable && i < MAX_RANK)
	{
		unsigned int it = abase::RandSelect(&lot[i][0].prob, sizeof(LOT), MAX_LOT);
		id = lot[i][it].id;
		count = lot[i][it].count;
		rank = lot[i][it].rank;
		return true;
	}
	return false;
}


bool gplayer_lottery::Init()
{
	memset(lot, 0x00 , (MAX_LOT * sizeof(LOT)));
	manager = 2; //Use apenas Bilhetes
	count = 0;
	return true;
}

bool gplayer_lottery::AddLot(unsigned int rank_lot, unsigned int pos)
{
	if ( pos < MAX_LOT && rank_lot < MAX_RANK && !lot[pos].id )
	{
		unsigned int id, cnt, rank;
		if ( LotteryConfig::GetInstance()->GetLot(rank_lot,id,cnt,rank ) )
		{
			lot[pos].is_open = 0;
			lot[pos].id = id;
			lot[pos].count = cnt;
			lot[pos].rank_item = rank;
			lot[pos].rank_lot = rank_lot;
			return true;
		}
	}
	return false;
}

unsigned int gplayer_lottery::GetLotCount()
{
	this->count =  0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if ( lot[i].id ) this->count++;
	}
	return this->count;
}

bool gplayer_lottery::StartX1()
{
	Init();
	score++;
	AddLot(WHITE_LOT,0);
	return GetLotCount();
}

bool gplayer_lottery::StartX5()
{
	Init();
	score += 5;
	LotteryConfig * lc = LotteryConfig::GetInstance();
	unsigned int rank_lot = (rand() % MAX_LOT);
	unsigned int rank_card = lc->GetProb();
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (i == rank_lot) 
			AddLot(rank_card ? ++rank_card : rank_card , i);
		else
			AddLot(WHITE_LOT, i);
	}
	return GetLotCount();
}

unsigned int gplayer_lottery::GetLotRank(unsigned int i)
{
	return i < MAX_LOT ? lot[i].rank_lot : WHITE_LOT;
}


void gplayer_lottery::OpenLot(unsigned int i, unsigned int & item_id, unsigned int & item_count)
{
	if ( i < MAX_LOT && !lot[i].is_open )
	{
		lot[i].is_open = 1;
		item_id = lot[i].id;
		item_count = lot[i].count;
	}
}

bool gplayer_lottery::IsStart()
{
		int res = 0;
		for(unsigned int i = 0; i < count; i++)
		{
			if (lot[i].is_open) res++;
		}
		return res == count;
}

void gplayer_lottery::GetInfo(unsigned int & manager, unsigned int & score, unsigned int & cnt, void * buf)
{
	S2C::CMD::get_lotery_info::ITEM * it = (S2C::CMD::get_lotery_info::ITEM *)buf;

	manager = this->manager;
	score = this->score;

	// Remove o bug de score negativo
	if(score < 0) score = 0;
	if(score > 50000) score = 0;
	
	cnt = GetLotCount();
	
	for(unsigned int i = 0; i < cnt && i < MAX_LOT; i++)
	{
		it[i].id = this->lot[i].is_open ? this->lot[i].id : 0;
		it[i].rank_item = this->lot[i].is_open ? this->lot[i].rank_item : 0;
		it[i].rank_lot = this->lot[i].rank_lot;
		it[i].is_open = this->lot[i].is_open;
	}
	
}

void gplayer_lottery_items::Init()
{
	memset(this,0x00,sizeof(gplayer_lottery_items));
}

unsigned int gplayer_lottery_items::Rebuild()
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

unsigned int gplayer_lottery_items::Add(unsigned int id , unsigned int count)
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

unsigned int gplayer_lottery_items::Del(unsigned int id)
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

unsigned int gplayer_lottery_items::GetCleanSlot()
{
	unsigned int res = 0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if ( !item[i].id ) res++;
	}
	return res;
}

unsigned int gplayer_lottery_items::GetCountItems()
{
	unsigned int res = 0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (item[i].id) res++;
	}
	return res;
}

void gplayer_lottery_items::GetInfo( unsigned int & cnt , void * buf )
{
	cnt = Rebuild();
	memcpy(buf,item, (MAX_LOT * sizeof(ITEM)) );
}

//------------------------------------------------------------------------

void gplayer_treasure::Init()
{
	memset(lot, 0x00 , (MAX_LOT * sizeof(LOT)));
}

void gplayer_treasure::InitLot(unsigned int _free_count)
{
	TreasureConfig * tc = TreasureConfig::GetInstance();
	if ( tc->IsEnable() )
	{
		unsigned int box [MAX_LOT] = {0}; 
		unsigned int num = 1;
		unsigned int val; 
		while (num != MAX_LOT+1) 
		{  
			val = 1 + rand() % MAX_LOT; 
			if (box[val - 1] == 0)
			{
				box[val - 1] = num++;
			}
		}

		if ( _free_count == 1 ) score++;
		manager = 1;
		free_count = _free_count;
		lot_count = 10;
		box_count = 0;
		num_open = 1;
		for(unsigned int i = 0; i < MAX_LOT; i++)
		{
			unsigned int id, count, rank;
			tc->GetItem(id, count, rank);
			lot[i].id = id;
			lot[i].count = count;
			lot[i].rank = rank;
			lot[i].box = box[i] - 1;
			lot[i].lot = i;
			lot[i].is_open = 0;
		}
	}
}

void gplayer_treasure::RebuildLot()
{
	lot_count = 0;
	box_count = 0;
	for(unsigned int i = 0; i < MAX_LOT; i++)
	{
		if (lot[i].is_open) box_count++;
		else lot_count++;
	}
}

void gplayer_treasure::OpenLot(unsigned int param, unsigned int & item_id, unsigned int & item_count)
{
	if (param < MAX_LOT)
	{		
		for(unsigned int i = 0; i < MAX_LOT; i++)
		{
			if ( lot[i].box == param ) 
			{
				manager = 2;
				if (free_count) free_count--;
				lot[i].is_open = ++num_open;
				item_id = lot[i].id;
				item_count = lot[i].count;
				break;
			}
		}
		RebuildLot();
	}
}

void gplayer_treasure::GetInfo(unsigned char & _manager, unsigned int & _score, unsigned int & _free_count, unsigned int & _lot_count, unsigned int & _box_count, void * __lot, void * __box)
{
	unsigned int counter, i, j;
	RebuildLot();
	_manager = manager;
	_score = score;

	// Remove o bug de score negativo
	if(_score < 0) _score = 0;
	if(_score > 50000) _score = 0;

	_free_count = free_count;
	_lot_count = lot_count;
	_box_count = box_count;
	S2C::CMD::get_treasure_info::LOT * _lot = (S2C::CMD::get_treasure_info::LOT *)__lot;
	S2C::CMD::get_treasure_info::BOX * _box = (S2C::CMD::get_treasure_info::BOX *)__box;

	
	counter = 0;
	for(i = 0; i < MAX_LOT; i++)
	{
		if ( !lot[i].is_open )
		{
			_lot[counter].id = lot[i].id;
			_lot[counter].count = lot[i].count;
			_lot[counter].lot = lot[i].lot;
			counter++;
		}
	}
	
	j = 0;
	counter = 0;
	for(i = 0; i < (MAX_LOT*MAX_LOT+MAX_LOT); i++)
	{
		if (   !(i % MAX_LOT) ) counter++;
		if ( lot[i % MAX_LOT].is_open && lot[i % MAX_LOT].is_open == counter )
		{
			_box[j].id = lot[i % MAX_LOT].id;
			_box[j].box = lot[i % MAX_LOT].box;
			_box[j].count = lot[i % MAX_LOT].count;
			_box[j].lot = lot[i % MAX_LOT].lot;
			j++;
		}
	}

	
}

