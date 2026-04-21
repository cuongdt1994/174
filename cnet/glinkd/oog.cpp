#include <cstdio>
#include <cstddef>
#include "oog.hpp"

OOG* OOG::instance = 0;

void OOG::Init()
{
	size_t i = 0;
	pool[i].type = 0x03;		pool[i++].load_state = OOG_HIGHT_LIMIT * 10;
	pool[i].type = 0x22;		pool[i++].load_state = OOG_LOW_LIMIT;
	pool[i].type = 0x52;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x5B;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x62;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x64;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5;
	pool[i].type = 0x66;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5;
	pool[i].type = 0x6B;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x74;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x80;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5;
	pool[i].type = 0xCA;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0xCE;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0xD9;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0xE3;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0xF0;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x352;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5;
	pool[i].type = 0x3B7;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x3B9;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x3BA;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x451;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5; //10 Cross Remove
	pool[i].type = 0xFA1;		pool[i++].load_state = OOG_HIGHT_LIMIT;
	pool[i].type = 0x12D0;		pool[i++].load_state = OOG_HIGHT_LIMIT * 5; 
	pool[i].type = 0x1517;		pool[i++].load_state = OOG_HIGHT_LIMIT;
}

int OOG::Check(int type, size_t & load_state)
{
	//устанавливаем размер нагрузки по умолчанию
	size_t add_load_state = OOG_HORMAL_LIMIT;
	
	//получаем размер нагрузки пакета
	for (size_t i = 0; pool[i].type && i < OOG_HIGHT_COUNT; i++)
	{
		if (type ==  pool[i].type)
		{
			add_load_state = pool[i].load_state;
			break;
		}
	}
	
	//проверяем не привышены ли лимиты
	load_state += add_load_state;
	if (load_state > OOG_MAX_LIMIT_KICKOUT)
	{
		//printf("OOG_PLAYER_DROP: sid = %d , roleid = %d , load_state = %d , protocol_type = %d , protocol_size = %d \n", sid, roleid, load_state, type, size );
		return 2; //игрок привысил максимальный лимит, кикаем
	}
	
	if( load_state > OOG_MAX_LIMIT && add_load_state >= OOG_HIGHT_LIMIT )
	{
		//printf("OOG_PLAYER_IGNORE: sid = %d , roleid = %d , load_state = %d , protocol_type = %d , protocol_size = %d \n", sid, roleid, load_state, type, size );
		return 1; //лимит тяжелых пакетов привышен, игнорируем тяжелые пакеты.
	}
	
	return 0; //всё хорошо, лимиты не привышены.
}

void OOG::HeartBeat(size_t & load_state)
{
	if (load_state >  OOG_TIMER_LIMIT)
		load_state -= OOG_TIMER_LIMIT;
}




