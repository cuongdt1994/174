//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT CGAME (C) DeadRaky 2022
//---------------------------------------------------------------------------------------------------------------------------
#define LUABRIDGE

#include <lua.hpp>
#include <LuaBridge.h>
#include <luabase.h>

#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <iostream>
#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include <db_if.h>
#include <gsp_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "usermsg.h"
#include "public_quest.h"
#include "antiwallow.h"
#include "item/equip_item.h"
#include "luamanager.h"

using namespace luabridge;

// ================================================================
// LUA GLOBAL ITEM  
// ================================================================

LuaManager* LuaManager::instance = NULL;
LuaManager::MEMSPACE LuaManager::data;
abase::octets LuaManager::msg;

int TASK_DEFAULT_MAX_SIMULTANEOUS_COUT 	() { int res = LuaManager::GetInstance()->GetConfig()->TASK_DEFAULT_MAX_COUT;	/*printf("TASK_DEFAULT_MAX_COUT = %d \n", res);	*/	return res; }
int TASK_MAX_SIMULTANEOUS_COUT			() { int res = LuaManager::GetInstance()->GetConfig()->TASK_MAX_COUT		;	/*printf("TASK_MAX_COUT	 	 = %d \n", res);	*/	return res; }
int TASK_HIDDEN_COUNT           		() { int res = LuaManager::GetInstance()->GetConfig()->TASK_HIDDEN_COUNT	;	/*printf("TASK_HIDDEN_COUNT     = %d \n", res);	*/	return res; }
int TASK_TITLE_TASK_COUNT				() { int res = LuaManager::GetInstance()->GetConfig()->TASK_TITLE_TASK_COUNT;	/*printf("TASK_TITLE_TASK_COUNT = %d \n", res);	*/	return res; }

// ================================================================
// LUA STRUCT  
// ================================================================

LuaManager::DEBUG_CMD::DEBUG_CMD() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

//-------------------------------------------------------------------

LuaManager::COOLDOWN::COOLDOWN() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

//-------------------------------------------------------------------

LuaManager::HW_MASTER::HW_MASTER() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

//-------------------------------------------------------------------

LuaManager::INCAST::INCAST() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::INCAST::INIT()
{
	memset(this,0x00,sizeof(INCAST));
	for (unsigned int i = 0; i < USER_CLASS_COUNT; i++)
	{
		firstrun[i].time = 99;
		firstrun[i].max_time = 90;
	}
}

int LuaManager::INCAST::RELEASE(int id, int cls)
{
	if (id > 1750 || cls > USER_CLASS_COUNT) 
		return 0;
	for (int i = 0; skill[i].id && i < MAX_SKILL; i++ )
		if (id == skill[i].id)
			return skill[i].time;
	return supressor[cls];
}

int LuaManager::INCAST::FIRSTRUN(int cls, int cast)
{
	if (cast > firstrun[cls].time) 
		cast = firstrun[cls].max_time;
	return cast;
}

//-------------------------------------------------------------------

LuaManager::SKILLSENDER::SKILLSENDER() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::SKILLSENDER::INIT()
{
	memset(cd,0x00,sizeof(cd));
	static const int invalids[] = {
	90 ,91 ,113,114,115,116,120,189,
	191,192,193,306,462,463,466,467,
	542,543,544,545,546,547,550,551,
	552,553,554,555,564,565,566,567,
	568,569,570,571,648,649,0  ,0 };
	memcpy(skill, invalids, sizeof(invalids));
}

bool LuaManager::SKILLSENDER::INVALID(int id, int tid, int filter)
{
	for (unsigned int i = 0; skill[i] && i < MAX_SKILL ; i++)
	{
		if (skill[i] == id)
		{
			return filter || tid<=0;
		}
	}
	return false;
}

//-------------------------------------------------------------------

LuaManager::SWAP::SWAP() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::SWAP::INIT()
{
	memset(this,0x00,sizeof(*this));
	equip[item::EQUIP_INDEX_HP_ADDON] = 2;
	equip[item::EQUIP_INDEX_MP_ADDON] = 2;
	equip[item::EQUIP_INDEX_ELF] = 1;
}

//-------------------------------------------------------------------

LuaManager::BALANCE::BALANCE() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::BALANCE::INIT()
{
	memset(this,0x00,sizeof(*this));
}

void LuaManager::BALANCE::RELEASE(int rcls, int tcls, void * d)
{
	float rate = 0.0f;
	DMG *dmg = (DMG*)d;
	
	rate = data[rcls].cls[tcls].fiz;
	if(rate && rate > -100.f && rate < 99999.0f)
		dmg->fiz += dmg->fiz * 0.01f * rate;

	rate = data[rcls].cls[tcls].mag;
	if(rate && rate > -100.f && rate < 99999.0f)
		for (int i = 0; i < 5; ++i )
		dmg->mag[i] += dmg->mag[i] * 0.01f * rate;
}

//-------------------------------------------------------------------

LuaManager::ASPEED::ASPEED() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::ASPEED::INIT()
{
	memset(this,0x00,sizeof(*this));
}

int LuaManager::ASPEED::RELEASE(int weapon, int speed, int cultivate)
{
	if(weapon)
	{
		int i = 0, j = 0;
		if (cultivate >= 20 && cultivate <= 22) j = 1;
		if (cultivate >= 30 && cultivate <= 32) j = 2;
		for(int i = 0; data[j].type[i].id && i < 20; i++)
			if(data[j].type[i].id == weapon)
			{
				if(data[j].type[i].speed >= speed)
				speed = data[j].type[i].speed;
				break;
			}
	}
	if(speed < 4  ) return 4;
	if(speed > 300) return 300;
	return speed;
}

//-------------------------------------------------------------------

LuaManager::BUGSKILL::BUGSKILL() 
{ 
	memset(this, 0x00, sizeof(*this)); 
}

void LuaManager::BUGSKILL::INIT()
{
	unsigned int i = 0;
	memset(this,0x00,sizeof(*this));
	skill[i].id = 58;	skill[i++].enable = 0;
	skill[i].id = 59;	skill[i++].enable = 0;
	skill[i].id = 100;	skill[i++].enable = 0;	
	skill[i].id = 478;	skill[i++].enable = 0;
	skill[i].id = 479;	skill[i++].enable = 0;
}

int LuaManager::BUGSKILL::RELEASE(int id, int cls)
{
	if (id > 936 || cls > USER_CLASS_COUNT) return 0;
	for (int i = 0; skill[i].id; i++ )
			if (id == skill[i].id)
					return skill[i].enable;
	return data[cls];
}

//-------------------------------------------------------------------

LuaManager::CONFIG::CONFIG()
{
	memset(this, 0x00, sizeof(*this));
}

void LuaManager::CONFIG::INIT()
{
	debug_passwd = 0;
	SummonPet136 = 0;
	CountryBattleIsTeam = 0;
	MaxVigour = 9999;
	shutdown_ap = 0;
	IdDistanceShop = 80080;
	ChangeElfStatusTime = 7*24*3600;
	townscroll_ccoldown = 5*1000; //5 second
	enable_safelock = 1;
	pvp_cooldown_time = 10*3600;
	pvp_protect_level_min = 29;
	pvp_protect_level_max = 69;
	vip_enable = 1;
	min_refine_level = 1;
	max_refine_level = 12;
	battle_bid_item = 21652;
	meredian_max = 100;  
	meredian_inc = 50;	
	enable_patch_equip = 1;
	atk_def_enable = 1;
	max_load_state = 449;
	need_load_state = 10;
	level_min_to_send_message = 1;
	speed_skill_enable = 1;
	dg_buff_enable = 1;
	activity_event_enable = 1;
	key_task_enable = 5457519;
	
	LATE_DAY_SIGNIN_ITEM	= 39960;	
	SIGNIN_PERFECT_AWARD	= 39959;
	SIGNIN_YEAR_AWARD		= 39958;
	SIGNIN_MONTH_AWARD		= 39957;
	MAX_LATE_SIGNIN_TIMES	= 4;
	GENERALCARD_RANK_S		= 3;
	
	TASK_DEFAULT_MAX_COUT 	= 20;
	TASK_MAX_COUT			= 30;
	TASK_HIDDEN_COUNT       = 30;
	TASK_TITLE_TASK_COUNT	= 10;
	
	static int WR_ITEM_POPPET_DUMMY_TABLE[] = {86403, ITEM_POPPET_DUMMY_ID2, ITEM_POPPET_DUMMY_ID, ITEM_POPPET_DUMMY_ID3, 0}; //куклы замены
	static int WR_ITEM_WORLD_SPEAKER_TABLE[] = {86401, WORLD_SPEAKER_ID, WORLD_SPEAKER_ID2, 0}; //рупоры
	static int WR_ITEM_SUPERWORLD_SPEAKER_TABLE[] = {86402, SUPERWORLD_SPEAKER_ID, SUPERWORLD_SPEAKER_ID2, 0}; //горны
	static int WR_ITEM_GATCHER_SPEED[] = {86404, 86405, 0};	//итемы для ускорения выкапывания шахт
	static int WR_ITEM_MASS_PICKUP[] = {86406, 86407, 0};	//итемы для масс сбора лута
	static int WR_ITEM_PET_RENAME[] = {12403, 86408, 0};	//итемы для изменения имени питомца
	static int WR_ITEM_REFINE_FULL[] = {86409, 86410, 0};	//Итемы заточки при которых снаряжение сразу затачивается на +12
	static int WR_OTHER_MINE_SPEED[] = {65100, 0}; 		//дополнительные иды шахкт в которых время ускоряется.
	static int WR_ITEM_SCROLL_TOWN[] = {59410, 51130, 51252, 59411, 59412, 0 }; //руны которым разрешено телепортироваться между локациями
	static int WR_ITEM_OFFLINE_MARKET[] = {86413, 86414, 0};//Итемы для оффлайн котов
	static int WR_ITEM_REFINE[] = {11208, 56046, 59663, 0};//Итемы для заточки
	static int WR_ITEM_REVIVE_SCROLL[] = { 86415, REVIVE_SCROLL_ID2, REVIVE_SCROLL_ID, 0}; //Итемы - свитки воскрешения
	static int WR_ITEM_ALLIANCE_SPEAKER_TABLE[] = { 52018, 52019, 0};//Итемы для чата алианса
	static int WR_DISTANCE_SERVICE_TAG[] = { 1,	0 }; //локации в которых работают дистанционные сервисы
	static int WR_MASS_PICKUP_DISTABLE_TAG[] = { 201, 0  };//Локации в которых не работает масс сбор лута
	static int WR_DISTANCE_SERVICE_TYPE[] = { 1, 0 };//иды дистанционных сервисов
	static int WR_GOOD_ITEMS_TABLE[] = { 59410,  59411,  59412,  86401,  86402,  86403,  86411,  86412, 86416, 0  }; //бесконечные итемы
	static int WR_GOOD_AMULETS_TAG[] = {143,144,145,146,147,0};//Локации в которых хирки бесконечные
	static int WR_ELF_DISTABLE_LOCATIONS[] = {120,201,0};//локации где стамина джина = 0
	static int WR_PATTY_DISTABLE_TAGS[] = {201,0};//локации где не раотает пати
	static int WR_SHIELD_SKILL_TABLE[] = {5092,5094,5101,5104,5106,5139,5140,5143,5144,5155,5156,5161,5162,5165,5166,0	}; //скиллы щита
	static int WR_ITEM_GLOBAL_SPEAKER_TABLE[] = {86416, GLOBAL_SPEAKER_ID, GLOBAL_SPEAKER_ID2, 0}; //Межсерверные рупоры
	static int WR_VIP_SCORE_LEVEL_TABLE[] = { 0,15,75,150,375,900,2250,999999999 }; //кривая вип лвл
	static int WR_ALLIANCE_CONGREGATE_TAG[] = {1, 121, 122, 137, 163, 168, 185, 0}; //Локации в которых работает призыв алианса
	static int WR_ITEM_NEW_ENGRAVE[] = {59660, 0}; // Fulano
	static int WR_ITEM_BIBLE_REFINE[] = {59358, 59359, 0};
	static int WR_ITEM_DELAY_SIGNIN[] = { LATE_DAY_SIGNIN_ITEM, 66593 , 0};
	static int WR_ITEM_RECIPE_LIB_PRODUCE[] = {111001, 111002, 111003, 140109, 0};
	static int WR_ITEM_ITEM_LIB_PRODUCE[] = {111020, 0};	
	memcpy( ITEM_POPPET_DUMMY_TABLE       , WR_ITEM_POPPET_DUMMY_TABLE       , sizeof( WR_ITEM_POPPET_DUMMY_TABLE       ));
	memcpy( ITEM_WORLD_SPEAKER_TABLE      , WR_ITEM_WORLD_SPEAKER_TABLE      , sizeof( WR_ITEM_WORLD_SPEAKER_TABLE      ));
	memcpy( ITEM_SUPERWORLD_SPEAKER_TABLE , WR_ITEM_SUPERWORLD_SPEAKER_TABLE , sizeof( WR_ITEM_SUPERWORLD_SPEAKER_TABLE ));
	memcpy( ITEM_GATCHER_SPEED            , WR_ITEM_GATCHER_SPEED            , sizeof( WR_ITEM_GATCHER_SPEED            ));
	memcpy( ITEM_MASS_PICKUP              , WR_ITEM_MASS_PICKUP              , sizeof( WR_ITEM_MASS_PICKUP              ));
	memcpy( ITEM_PET_RENAME               , WR_ITEM_PET_RENAME               , sizeof( WR_ITEM_PET_RENAME               ));
	memcpy( ITEM_REFINE_FULL              , WR_ITEM_REFINE_FULL              , sizeof( WR_ITEM_REFINE_FULL              ));
	memcpy( OTHER_MINE_SPEED              , WR_OTHER_MINE_SPEED              , sizeof( WR_OTHER_MINE_SPEED              ));
	memcpy( ITEM_SCROLL_TOWN              , WR_ITEM_SCROLL_TOWN              , sizeof( WR_ITEM_SCROLL_TOWN              ));
	memcpy( ITEM_OFFLINE_MARKET           , WR_ITEM_OFFLINE_MARKET           , sizeof( WR_ITEM_OFFLINE_MARKET           ));
	memcpy( ITEM_REFINE                   , WR_ITEM_REFINE                   , sizeof( WR_ITEM_REFINE                   ));
	memcpy( ITEM_REVIVE_SCROLL            , WR_ITEM_REVIVE_SCROLL            , sizeof( WR_ITEM_REVIVE_SCROLL            ));
	memcpy( ITEM_ALLIANCE_SPEAKER_TABLE   , WR_ITEM_ALLIANCE_SPEAKER_TABLE   , sizeof( WR_ITEM_ALLIANCE_SPEAKER_TABLE   ));
	memcpy( DISTANCE_SERVICE_TAG          , WR_DISTANCE_SERVICE_TAG          , sizeof( WR_DISTANCE_SERVICE_TAG          ));
	memcpy( MASS_PICKUP_DISTABLE_TAG      , WR_MASS_PICKUP_DISTABLE_TAG      , sizeof( WR_MASS_PICKUP_DISTABLE_TAG      ));
	memcpy( DISTANCE_SERVICE_TYPE         , WR_DISTANCE_SERVICE_TYPE         , sizeof( WR_DISTANCE_SERVICE_TYPE         ));
	memcpy( GOOD_ITEMS_TABLE              , WR_GOOD_ITEMS_TABLE              , sizeof( WR_GOOD_ITEMS_TABLE              ));
	memcpy( GOOD_AMULETS_TAG              , WR_GOOD_AMULETS_TAG              , sizeof( WR_GOOD_AMULETS_TAG              ));
	memcpy( ELF_DISTABLE_LOCATIONS        , WR_ELF_DISTABLE_LOCATIONS        , sizeof( WR_ELF_DISTABLE_LOCATIONS        ));
	memcpy( PATTY_DISTABLE_TAGS           , WR_PATTY_DISTABLE_TAGS           , sizeof( WR_PATTY_DISTABLE_TAGS           ));
	memcpy( SHIELD_SKILL_TABLE            , WR_SHIELD_SKILL_TABLE            , sizeof( WR_SHIELD_SKILL_TABLE            ));
	memcpy( ITEM_GLOBAL_SPEAKER_TABLE     , WR_ITEM_GLOBAL_SPEAKER_TABLE     , sizeof( WR_ITEM_GLOBAL_SPEAKER_TABLE     ));
	memcpy( VIP_SCORE_LEVEL_TABLE         , WR_VIP_SCORE_LEVEL_TABLE         , sizeof( WR_VIP_SCORE_LEVEL_TABLE         ));
	memcpy( ALLIANCE_CONGREGATE_TAG       , WR_ALLIANCE_CONGREGATE_TAG       , sizeof( WR_ALLIANCE_CONGREGATE_TAG       ));
	memcpy( ITEM_NEW_ENGRAVE              , WR_ITEM_NEW_ENGRAVE              , sizeof( WR_ITEM_NEW_ENGRAVE              ));
	memcpy( ITEM_BIBLE_REFINE             , WR_ITEM_BIBLE_REFINE             , sizeof( WR_ITEM_BIBLE_REFINE             ));
	memcpy( ITEM_DELAY_SIGNIN             , WR_ITEM_DELAY_SIGNIN             , sizeof( WR_ITEM_DELAY_SIGNIN             ));
	memcpy( ITEM_RECIPE_LIB_PRODUCE       , WR_ITEM_RECIPE_LIB_PRODUCE       , sizeof( WR_ITEM_RECIPE_LIB_PRODUCE       ));
	memcpy( ITEM_ITEM_LIB_REFINE         , WR_ITEM_ITEM_LIB_PRODUCE         , sizeof( WR_ITEM_ITEM_LIB_PRODUCE         ));
}

//-------------------------------------------------------------------

LuaManager::MEMSPACE::MEMSPACE()
{
	config.INIT();
	incast.INIT();
	skillsender.INIT();
	swap.INIT();
	balance.INIT();
	aspeed.INIT();
	bugskill.INIT();
}

void LuaManager::MEMSPACE::INIT()
{
	config.INIT();
	incast.INIT();
	skillsender.INIT();
	swap.INIT();
	balance.INIT();
	aspeed.INIT();
	bugskill.INIT();
}

// ================================================================
// LUA FUNCTIONS 
// ================================================================

double LuaManager::game__Packet(int cmd, int type, double value )
{
	switch (cmd)
	{
	case PACKET_CLEAN   :	msg.clear(); return 0; break;
	case PACKET_ADDR    :	return (long long)msg.begin(); break;
	case PACKET_SIZE    :	return msg.size(); break;
	default: break;		
	}
	
	if( cmd  == PACKET_PUSH  )
	{
		if( type == TYPE_CHAR    ) { char res = value; msg.push_back(&res,sizeof(res)); return 0; }
		if( type == TYPE_SHORT   ) { short res = value; msg.push_back(&res,sizeof(res)); return 0; }
		if( type == TYPE_INT 	 ) { int res = value; msg.push_back(&res,sizeof(res)); return 0; }
		if( type == TYPE_INT64   ) {long long res = value; msg.push_back(&res,sizeof(res)); return 0; }
		if( type == TYPE_FLOAT   ) { float res = value; msg.push_back(&res,sizeof(res)); return 0; }
		if( type == TYPE_DOUBLE  ) { double res = value; msg.push_back(&res,sizeof(res)); return 0; }
	}
	
	return 0;
}

void LuaManager::game__Send(int roleid)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && msg.size())
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->S2CGameDataSend(msg.begin(),msg.size());
	}
}

void LuaManager::game__SendBroadCast(int roleid)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && msg.size())
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->S2CGameDataSendElse(msg.begin(),msg.size());
	}
}

void LuaManager::game__SaveFile(long long Src, unsigned int size, const char* name)
{
	if(!Src || !size || !name || strlen(name) > 128) return;
	static FILE *fp = fopen(name, "wb");
	fwrite((const void*)Src, 1, size, fp);
	fclose(fp);
}

long long LuaManager::game__LoadFile(long long  Src, unsigned int size, const char* name)
{
	if(!Src || !size || !name) return 0;
	int fres = 0;
	static FILE *fp = fopen(name, "a+b");
	if(fp)
	{
		int fres = fread((void*)Src, 1, size, fp);
		if(fres == size)
		{
			puts((const char*)Src);
		}
		fclose(fp);
	}
	return fres;
}

void LuaManager::game__ChatMsg(int type , int roleid, int channel, const char * utf8_msg, int shift_pos)
{
	if (!utf8_msg || type >= CHAT_MAX) return;
	unsigned int utf8_size = strlen(utf8_msg);
	if (utf8_size && utf8_size < 256)
	{
		static std::u16string utf16;
		utf16.clear();
		utf16.resize(utf8_size*2 + 2);
		memset((char*)utf16.c_str(), 0x00, utf16.size());
		utf8_to_utf16((const utf8_t*)utf8_msg, utf8_size, (utf16_t*)utf16.c_str(), utf16.size() );

		unsigned int shift_size = 0;
		static char shift[1024];
		static char aux[5];
		
		switch (type)
		{
			case CHAT_SINGLE:
			{
				int windex1;
				gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
				if(gPlayer && gPlayer->imp)
				{
								gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
					if(shift_pos >= 0)
					{
						memset(aux, 0x00,sizeof(aux));
						aux[3] = shift_pos;
						memset(shift, 0x00, 1024);
						shift_size = pImp->TransformChatData(aux,sizeof(aux), shift, 1024);
					}
				
					GMSV::chat_msg gcm;
					gcm.speaker			= 0;
					gcm.msg				= utf16.c_str();
					gcm.size			= utf16.size();
					gcm.data			= shift_size ? shift : 0;
					gcm.dsize			= shift_size;
					gcm.channel			= channel;
					gcm.emote_id		= 0;
					gcm.speaker_level	= 0;
					GMSV::SendChatMsg(gPlayer->cs_index, gPlayer->ID.id, gPlayer->cs_sid, gcm);
				}
				return;
				break;
			}
			case CHAT_INSTANCE:
			{
				int windex1;
				gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
				if(gPlayer && gPlayer->imp)
				{
								gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
					if(shift_pos >= 0)
					{
						memset(aux, 0x00,sizeof(aux));
						aux[3] = shift_pos;
						memset(shift, 0x00, 1024);
						shift_size = pImp->TransformChatData(aux,sizeof(aux), shift, 1024);
					}
					AutoBroadcastChatMsg(pImp->_plane,gPlayer->pPiece,utf16.c_str(),utf16.size(),channel,0,shift_size ? shift : 0,shift_size,roleid,gPlayer->base_info.level);
				}
				return;
				break;
			}
			case CHAT_WORLD:
			{
				
				if(shift_pos >= 0)
				{
					int windex1;
					gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
					if(gPlayer  && gPlayer->imp)
					{
										gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
						memset(aux, 0x00,sizeof(aux));
						aux[3] = shift_pos;
						memset(shift, 0x00, 1024);
						shift_size = pImp->TransformChatData(aux,sizeof(aux), shift, 1024);
					}
				}
				
				broadcast_chat_msg(roleid,utf16.c_str(),utf16.size(), channel, 0, shift_size ? shift : 0, shift_size);
				return;
				break;
			}
			default: 
				break;
		}
	}
	printf("game__ChatMsg: type=%d , roleid=%d , channel=%d , utf8_msg=%s , shift_pos=%d \n", type , roleid, channel, utf8_msg, shift_pos);
}

const char * LuaManager::game__GetName(int type, int id)
{
	switch (type)
	{
		case NAME_ROLE:
		{
			int windex1;
			gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(id,windex1);
			if(gPlayer && gPlayer->imp)
			{
						gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
				
				unsigned int name_size = 0;
				const void * name = pImp->GetPlayerName(name_size);
				if(name && name_size >= 2)
				{
					static std::string utf8;
					utf8.clear();
					utf8.resize(name_size + 1);
					memset((void*)utf8.c_str(), 0x00, utf8.size());
					utf16_to_utf8((const char16_t*)name, (char*)utf8.c_str(), utf8.size() - 1);
					return (const char *)utf8.c_str();
				}
			}
			break;
		}
		case NAME_ITEM:
		{
			int name_size = 0;
			const void * name = (const utf16_t*)world_manager::GetDataMan().get_item_name(id,name_size);
			if(name && name_size >= 2)
			{
				static std::string utf8;
				utf8.clear();
				utf8.resize(name_size + 1);
				memset((void*)utf8.c_str(), 0x00, utf8.size());
				utf16_to_utf8((const char16_t*)name, (char*)utf8.c_str(), utf8.size() - 1);
				return (const char *)utf8.c_str();
			}
			break;
		}

		default: 
			break;
	}
	return "NULL";
}

long long LuaManager::game__GetWorldTag()
{
	return world_manager::GetWorldTag();
}

long long LuaManager::game__GetWorldIndex()
{
	return world_manager::GetWorldIndex();
}

long long LuaManager::game__InvPlayerGiveItem(int roleid, int inv, int id, int count, int time, int proctype)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		return pImp->InvPlayerGiveItem(inv,id,count,time,proctype);
	}
}

long long LuaManager::game__InvPlayerGiveItem2(int roleid, int id, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		return pImp->InvPlayerGiveItem(id,count);
	}
}
long long LuaManager::game__InvPlayerSpendItem(int roleid, int inv, int id, int count)
{
	int res = 0;
	if(id > 0 && count > 0)
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
		if(gPlayer && gPlayer->imp)
		{
				gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
			
			int index = pImp->GetInventory(inv).Find(0,id);
			if (index != -1)
			{
				item it   = pImp->GetInventory(inv)[index];
				if(it.count > count)
				{
					pImp->UpdateMallConsumptionDestroying(it.type, it.proc_type, count);
					pImp->GetInventory(inv).DecAmount(index, count);
					pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,index,it.type,count,S2C::DROP_TYPE_USE);
					res = 1;
				}
			}
		}
	}
	return res;
}

long long LuaManager::game__InvGetCountPlayerItem(int roleid, int inv, int id)
{
	int res = 0;
	if(id > 0)
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
		if(gPlayer && gPlayer->imp)
		{
				gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
			int index = pImp->GetInventory(inv).Find(0,id);
			if (index != -1)
			{
				res = pImp->GetInventory(inv)[index].count;
			}
		}
	}
	return res;
}

long long LuaManager::game__GetObjectVar(int type, int id, int tid)
{
	long long res = 0;
	world * pPlane = world_manager::GetInstance()->GetWorldByIndex(0);
	if(pPlane)
	{
	
		switch (type)
		{
		case NPC_TID_BY_ID:
		{
			return pPlane->GetNpcType(id);
		}
		break;
		case NPC_ID_BY_TID:
		{
			for (unsigned int i = 0; i < world_manager::GetMaxNPCCount() ; i++)
			{
				gnpc* pNpc = pPlane->GetNPCByIndex(i);
				if( pNpc && pNpc->IsActived() )
				{
					if ( pNpc->tid == tid )
					{
						return pNpc->ID.id;
					}
				}
			}
		}
		break;
		case NPC_COORDS_BY_TID:
		{
			for (unsigned int i = 0; i < world_manager::GetMaxNPCCount() ; i++)
			{
				gnpc* pNpc = pPlane->GetNPCByIndex(i);
				if( pNpc && pNpc->IsActived() )
				{
					if ( pNpc->tid == tid )
					{
						return (long long)&pNpc->pos;
					}
				}
			}
		}
		break;
		case MINE_GET_TID_BY_ID:
		{
			return pPlane->GetMineMatterType(id);
		}
		break;
		default: break;
		}
	
	}
	return res;
}

long long LuaManager::game__GetPQContrib(int task_id, int role_id)
{
	return world_manager::GetPublicQuestMan().GetCurContrib(task_id,role_id);
}

long long LuaManager::game__GetPQAllContrib(int task_id)
{
	return world_manager::GetPublicQuestMan().GetAllCurContrib(task_id);
}
void LuaManager::game__SetPQContrib(int task_id, int role_id, int value)
{
	world_manager::GetPublicQuestMan().SetCurContrib(task_id,role_id,value);
}

double LuaManager::game__GetStorage(int roleid, int pos)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		if(pos >= 0 && pos <= 31)
		{
			return pImp->GetLuaStorage(pos);
		}
	}
	return 0;
}

void LuaManager::game__SetStorage(int roleid, int pos, double value)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		if(pos >= 0 && pos <= 31)
		{
			pImp->SetLuaStorage(pos,value);
		}
	}
}

void LuaManager::game__PlayerUpdateItemsProctype(int roleid, int inv, int id, int proctype, int type)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		if(pImp || id > 0)
		{
			switch (type)
			{
			case ITEM_CHANGE_PROCTYPE: //Заменить привязку у одного итема
				for (unsigned int j = 0; j < pImp->GetInventory(inv).Size(); j++)
					if( pImp->GetInventory(inv)[j].type > 0 && pImp->GetInventory(inv)[j].type == id )
						pImp->GetInventory(inv)[j].proc_type = proctype;
					return;
				break;
			case ITEM_ADD_PROCTYPE: //Добавить к существующей привязке свою привязку у одного итема
				for (unsigned int j = 0; j < pImp->GetInventory(inv).Size(); j++)
					if( pImp->GetInventory(inv)[j].type > 0 && pImp->GetInventory(inv)[j].type == id )
						pImp->GetInventory(inv)[j].proc_type |= proctype;
					return;
				break;
			case ITEM_CHANGE_PROCTYPE_ALL: //Заменить привязку у всех итемов
				for (unsigned int j = 0; j < pImp->GetInventory(inv).Size(); j++)
					if( pImp->GetInventory(inv)[j].type > 0 )
						pImp->GetInventory(inv)[j].proc_type = proctype;
					return;
				break;
			case ITEM_ADD_PROCTYPE_ALL: //Добавить к существующей привязке свою привязку у всех итемов
				for (unsigned int j = 0; j < pImp->GetInventory(inv).Size(); j++)
					if( pImp->GetInventory(inv)[j].type > 0 )
						pImp->GetInventory(inv)[j].proc_type |= proctype;
					return;
				break;
			default: 
				break;
			}
		}
	}
}

void LuaManager::game__DestroyProctypeItems(int roleid, int inv, int proctype)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		for (unsigned int j = 0; j < pImp->GetInventory(inv).Size(); j++)
		{
			item it = pImp->GetInventory(inv)[j];
			if ( it.type > 0 && (it.proc_type & proctype) )
			{
				pImp->UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
				pImp->GetInventory(inv).DecAmount(j, it.count);
				pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,j,it.type,it.count,S2C::DROP_TYPE_USE);
			}	
		}
	}
}

void LuaManager::game__TestItemProctype(int roleid, int pos, int proctype)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		item it = pImp->GetInventory()[pos];
		if ( it.type > 0 && (it.proc_type & proctype) )
		{
			pImp->UpdateMallConsumptionDestroying(it.type, it.proc_type, it.count);
			pImp->GetInventory().DecAmount(pos, it.count);
			pImp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,pos,it.type,it.count,S2C::DROP_TYPE_USE);
		}
	}
}

void LuaManager::game__AddItemProctype(int roleid, int pos, int proctype)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		if( pImp->GetInventory()[pos].type > 0 )
		{
			pImp->GetInventory()[pos].proc_type |= proctype;
		}
	}
}

void LuaManager::game__PlayerTeleport(int roleid, int tag, double x, double y, double z)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		A3DVECTOR pos(x,y,z);
		pImp->LongJump(pos,tag,0);
	}
}

double LuaManager::game__PlayerCheckCoord(int roleid, int tag, double radius, double x, double y, double z)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && tag == world_manager::GetWorldTag())
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		double res  =  
			((x - pImp->_parent->pos.x) * (x - pImp->_parent->pos.x)) +
			((y - pImp->_parent->pos.y) * (y - pImp->_parent->pos.y)) +
			((z - pImp->_parent->pos.z) * (z - pImp->_parent->pos.z)) ;
		return res < radius;
	}
	return 0;
}

double LuaManager::game__GetRoleVar(int type , int roleid)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		gplayer_controller * pCtrl = 	((gplayer_controller*)pImp->_commander);
		switch (type)
		{
		case PIMP_CLS        	: return pImp->GetPlayerClass();	break;	
		case PIMP_SEC_LEVEL  	: return pImp->_basic.sec.level;	break;	
		case PIMP_LEVEL      	: return pImp->_basic.level;		break;	
		case PIMP_SP         	: return pImp->_basic.skill_point;	break;
		case PIMP_MONEY      	: return pImp->GetGold();			break;
		case PIMP_USERID     	: return pImp->GetDBUserId();		break;
		case PIMP_REPUTATION 	: return pImp->GetReputation();		break;	
		case PIMP_FACTIONID	 	: return pImp->GetFaction();		break;
		case PIMP_FACTIONRANK 	: return pImp->OI_GetMafiaRank(); 	break;
		case PIMP_FACTIONMASTER	: return pImp->OI_IsMafiaMaster();	break;
		case PIMP_PVPFLAG	 	: return pImp->GetPVPFlag();		break;				
		case PIMP_PKZONE	 	: return pImp->TestPKProtected(); 	break;
		case PIMP_GETMAFIAID 	: return pImp->OI_GetMafiaID();		break;
		case PIMP_FORCE	 		: return pImp->OI_GetForceID();		break;
		case PIMP_SILVER		: return pImp->GetSilver();			break;
		case CTRL_GM 		 	: return pCtrl->GetGMPrivelege();  	break;
		case CTRL_DEBUG 	 	: return pCtrl->GetDebugMode(); 	break;
		case CTRL_TARGET 	 	: return pCtrl->GetCurTarget().id; 	break;
		case PIMP_SEC_FLAG  	: return pImp->_basic.sec.flag;		break;	
		default: break;		
		}
	}
	return 0;		
}

void LuaManager::game__ActivateTriggerId(int trigger)
{
	world * pPlane = world_manager::GetInstance()->GetWorldByIndex(0);
	if(pPlane) pPlane->TriggerSpawn(trigger);
}

void LuaManager::game__DeactivateTriggerId(int trigger)
{
	world * pPlane = world_manager::GetInstance()->GetWorldByIndex(0);
	if(pPlane) pPlane->ClearSpawn(trigger);
}

void LuaManager::game__CreateObj(int type ,int roleid, int id, int time, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		
		switch (type)
		{
		case CREATE_MOB		: pImp->SummonMonster(id, count, pImp->_parent->ID, 10, time, 0, 0);	return; break;	
		case CREATE_NPC		: pImp->SummonNPC(id, count, pImp->_parent->ID, 10, time);				return; break;		
		case CREATE_MINE	: pImp->SummonMine(id, count, pImp->_parent->ID, 10, time);			return; break;	
		default: break;		
		}
	}
}

void LuaManager::game__PlayerObtainTitle(int roleid, int title, int time)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->GetPlayerTitle().ObtainTitle(title,time);
	}
}

void LuaManager::game__PlayerDeleteTitle(int roleid, int title)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->GetPlayerTitle().LoseTitle(title);
	}
}

void LuaManager::game__PlayerAddGold(int roleid, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->AddMallCash(count);
		pImp->_runner->player_cash(pImp->GetMallCash());
	}
}

void LuaManager::game__PlayerAddVip(int roleid, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->GetCashVipInfo().AddCashVipScore(count,(gplayer *)pImp->_parent);
	}
}

long long LuaManager::game__PlayerLvlUp(int roleid, int max_level, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && count > 0)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;

		if(!pImp || !pImp->CheckAndSetCoolDown(COOLDOWN_IDX_LVL_UP, LVLUP_COOLDOWN_TIME))
			return 0;

		if( max_level >= MAX_LVL) 
			max_level  = MAX_LVL;

		int i = pImp->_basic.level;
		if(i >= max_level) return 0;
		count+=i;
		for (i; i < count && i < max_level ; i++)
		{
			pImp->_basic.exp = player_template::GetLvlupExp(0,pImp->_basic.level);
			pImp->x_LevelUp();
		}
		return 1;
	}
	return 0;
}

long long LuaManager::game__PetLvlUp(int roleid, int max_level, int count)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && count > 0)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		
		if(!pImp || !pImp->CheckAndSetCoolDown(COOLDOWN_IDX_LVL_UP_PET, LVLUP_COOLDOWN_TIME))
			return 0;
		
		int index = pImp->OI_GetCurActivePet();
		pet_data * pData = pImp->OI_GetPetData(index);
		if(!pData) 
			return 0;
		
		if( pData->pet_class != pet_data::PET_CLASS_COMBAT && 
			pData->pet_class != pet_data::PET_CLASS_EVOLUTION )
			return 0;	
		
		if( max_level >= MAX_LVL_PET) 
			max_level =  MAX_LVL_PET;	
		
		int player_lvl =  pImp->GetHistoricalMaxLevel();
		if( player_lvl >= max_level) player_lvl = max_level;	
		
		
		int item_lvl = pData->level;
		if( item_lvl >= player_lvl || item_lvl >= max_level) 
			return 0;
		
		int i = player_lvl - item_lvl;
		
		if( i > count) 
			i = count;

		for (i; i > 0 ; i--)  
			pImp->PlayerPetLevelUp();			
		
		return 1;
	}
	return 0;
}

long long LuaManager::game__ElfLvlUp(int roleid, int max_level, int count, int exp, int sp )
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && count > 0)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		if(!pImp || !pImp->CheckAndSetCoolDown(COOLDOWN_IDX_LVL_UP_ELF, LVLUP_COOLDOWN_TIME))
			return 0;

		item & it = pImp->GetEquipInventory()[item::EQUIP_INDEX_ELF];
		if (!it.GetItemBody()) 
			return 0;
		
		if( max_level >= MAX_LVL_ELF) 
			max_level =  MAX_LVL_ELF;
		
		int player_lvl =  pImp->GetHistoricalMaxLevel();
		if( player_lvl >= max_level) player_lvl = max_level;
		
		int item_lvl = it.GetLevel();
		if( item_lvl >= player_lvl || item_lvl >= max_level) 
			return 0;
		
		if(pImp->_basic.skill_point < sp || pImp->_basic.exp < exp)
			return 0;
		
		pImp->_basic.skill_point -= sp;
		pImp->_basic.exp -= exp;
		int i = player_lvl - item_lvl;
		
		if( i > count) 
			i = count; 
		
		elf_item *elf_it = (elf_item*)it.GetItemBody();
		for (i; i > 0 ; i--)  
			elf_it->x_LevelUp(pImp);
			elf_it->OnRefreshItem();
		
		pImp->SetRefreshState();
		pImp->PlayerGetItemInfo(1,item::EQUIP_INDEX_ELF);
		return 1;
	}
	return 0;
}

void LuaManager::game__PlayerDisconnect(int roleid)
{
	int windex1 = world_manager::GetWorldIndex();
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		GMSV::SendDisconnect(gPlayer->cs_index, gPlayer->ID.id, gPlayer->cs_sid,0);
		GMSV::ReportCheater2Gacd(gPlayer->ID.id , 777, NULL,0);
	}
}

long long LuaManager::game__Cooldown(int type, int roleid, int idx, time_t time)
{
	//0 дальше, 1 останавливаем
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && idx)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		return pImp->CooldownManager(type , idx , time);
	}
	return 0;
}

long long LuaManager::game__MassiveCooldown(int type, int roleid,long long table)
{
	//0 дальше, 1 останавливаем
	int windex1;
	COOLDOWN * cd = (COOLDOWN*)table;
	
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && cd && cd->xid)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		return pImp->CooldownManagerMassive(type , (unsigned int*)table);
	}
	return 0;
}

long long LuaManager::game__InsRefresh(int roleid)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		
		if ( game__GetWorldTag() == 1 && !pImp->IsInTeam() )
		{
			if( pImp->CheckAndSetCoolDown(COOLDOWN_IDX_INS_REFRESH, MIN*1000) )
			{	
				pImp->RebuildInstanceKey();
				return true;
			}
		}
	}
	return false;
}

long long LuaManager::game__GetInvPosItemTable(int roleid, int inv,long long table)
{
	int windex1;
	int index = -1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp && table)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		index = pImp->GetInvPosItemTable(inv, (int*)table);
	}
	return index;
}

void LuaManager::game__PlayerGiveGold(int roleid, int money)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->PlayerGiveGold(money);
	}
}

void LuaManager::game__PlayerSpendGold(int roleid, int money)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->PlayerSpendGold(money);
	}
}

void LuaManager::game__PlayerGiveSilver(int roleid, int money)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->PlayerGiveSilver(money);
	}
}

void LuaManager::game__PlayerSpendSilver(int roleid, int money)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->PlayerSpendSilver(money);
	}
}

void LuaManager::game__LoadLuaStorage(int type, int key)
{
	if(type >= STORAGE_MAX)
	{
		printf("LuaManager::LoadLuaStorage: type = %d >= STORAGE_MAX \n", type);
		return;
	}
	
	class GetLuaDataResult : public abase::ASmallObject, public GDB::OnGetLuaDataResult
	{
		int type;
		int key;
	public:
		GetLuaDataResult( int type, int key )
		{
			this->type = type;
			this->key = key;
		}
		virtual void OnTimeOut()
		{
			printf("LuaManager::LoadLuaStorage: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("LuaManager::LoadLuaStorage: Failed! \n");
			delete this;
		}
		virtual void OnSucceed(int type, int key, void * buf, unsigned int size)
		{
			if (buf && size && this->type == type && this->key == key)
			{
				switch (type)
				{
					case STORAGE_TAG: 
					{ 
						//data.cworld.Load(key ? WORLD_DB : WORLD_GLOBAL , buf, size);
						break; 
					}
					case STORAGE_USER: 
					{ 
		 
						break; 
					}
					case STORAGE_ROLE: 
					{ 
						int windex1;
						gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(key,windex1);
						if(gPlayer && gPlayer->imp)
						{
							spin_autolock keeper(gPlayer->spinlock);
							gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
							pImp->GetLua()->LoadRole(key, buf, size);
						}
						break; 
					}
					case STORAGE_OTHER:
					{ 

						break; 
					}					
					default: 
					{ 
						GLog::log(GLOG_ERR,"LuaManager::LoadLuaStorage: Error type %d !!! \n", type); 
						break; 
					}	
				}
			}
			else
			{
				printf("LuaManager::LoadLuaStorage: !(data=%d && size=%d && this->type=%d == type=%d && this->key=%d == key=%d) BAD!!! \n", 
					data , size , this->type , type , this->key , key); 
			}
			delete this;
		}
	};
	
	GDB::OnGetLuaData(type, key, new GetLuaDataResult(type , key) );
}

void LuaManager::game__SaveLuaStorage(int type, int key,long long buf, unsigned int size)
{
	if(!buf || !size || type >= STORAGE_MAX)
	{
		printf("LuaManager::SaveLuaStorage: buf = %d , size = %d , type = %d BAD!!!\n", buf, size, type);
		return;
	}
	
	class SetLuaDataResult : public abase::ASmallObject, public GDB::OnSetLuaDataResult
	{
	public:
		SetLuaDataResult()
		{

		}
		virtual void OnTimeOut()
		{
			printf("LuaManager::SaveLuaStorage: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("LuaManager::SaveLuaStorage: Failed! \n");
			delete this;
		}
		virtual void OnSucceed()
		{
			delete this;
		}
	};
	
	GDB::OnSetLuaData(type, key, (void*)buf, size, new SetLuaDataResult() );
}

void LuaManager::game__LearnSkill(int roleid, int skill_id, int skill_level)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->LearnSkill(skill_id,skill_level);
	}
}

void LuaManager::game__RemoveSkill(int roleid, int skill_id)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		pImp->DeleteSkill(skill_id);
	}
}

void LuaManager::game__LoadTrashBox(int roleid)
{
	class OnGetNewTrashBoxResult : public abase::ASmallObject, public GDB::OnGetNewTrashBoxResult
	{
		int roleid;
	public:
		OnGetNewTrashBoxResult( int roleid )
		{
			this->roleid = roleid;
		}
		virtual void OnTimeOut()
		{
			printf("LuaManager::game__LoadTrashBox: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("LuaManager::game__LoadTrashBox: Failed! \n");
			delete this;
		}
		virtual void OnSucceed( int roleid, GDB::itemlist & box4, GDB::itemlist & box5, GDB::itemlist & box6, GDB::itemlist & box7 )
		{
			if (this->roleid == roleid)
			{
				int windex1;
				gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
				if(gPlayer && gPlayer->imp)
				{
					spin_autolock keeper(gPlayer->spinlock);
					gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
					pImp->LoadTrashBox(box4, 4);
					pImp->LoadTrashBox(box5, 5);
					pImp->LoadTrashBox(box6, 6);
					pImp->LoadTrashBox(box7, 7);
				}
			}
			else
			{
				printf("LuaManager::game__LoadTrashBox: key_roleid = %d , roleid = %d FAILED!!! \n", this->roleid, roleid);
			}
			delete this;
		}
	};
	GDB::OnGetNewTrashBox(roleid, new OnGetNewTrashBoxResult(roleid) );
}

void LuaManager::game__SaveTrashBox(int roleid, GDB::itemlist & box4 , GDB::itemlist & box5 , GDB::itemlist & box6 , GDB::itemlist & box7)
{
	class OnSetNewTrashBoxResult : public abase::ASmallObject, public GDB::OnSetNewTrashBoxResult
	{
		int roleid;
	public:
		OnSetNewTrashBoxResult(int roleid)
		{
			this->roleid = roleid;
		}
		virtual void OnTimeOut()
		{
			printf("LuaManager::game__SaveTrashBox: TimeOut! \n");
			delete this;
		}
		virtual void OnFailed()
		{
			printf("LuaManager::game__SaveTrashBox: Failed! \n");
			delete this;
		}
		virtual void OnSucceed(int roleid)
		{
			if (this->roleid == roleid)
			{
				
			}
			else
			{
				printf("LuaManager::game__SaveTrashBox: key_roleid = %d , roleid = %d FAILED!!! \n", this->roleid, roleid);
			}
			delete this;
		}
	};
	GDB::OnSetNewTrashBox(roleid, box4 , box5 , box6 , box7, new OnSetNewTrashBoxResult(roleid) );
}

const char * LuaManager::game__GetHwid(int roleid)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && gPlayer->imp)
	{
		static char hwid[12];
		gplayer_imp * pImp = (gplayer_imp *)gPlayer->imp;
		unsigned long long lhwid = pImp->GetHwid();
		snprintf(hwid, sizeof(hwid), "%llu", lhwid);
		return (const char*)hwid;
	}
	return "0";
}

// ================================================================
// LUA ITEMS GET & SET
// ================================================================

void LuaManager::FunctionsRegister()
{
	getGlobalNamespace(L).addFunction("game__Packet",game__Packet);
	getGlobalNamespace(L).addFunction("game__Send",game__Send);
	getGlobalNamespace(L).addFunction("game__SendBroadCast",game__SendBroadCast);
	getGlobalNamespace(L).addFunction("game__SaveFile",game__SaveFile);
	getGlobalNamespace(L).addFunction("game__LoadFile",game__LoadFile);
	getGlobalNamespace(L).addFunction("game__ChatMsg",game__ChatMsg);
	getGlobalNamespace(L).addFunction("game__GetName",game__GetName);
	getGlobalNamespace(L).addFunction("game__GetWorldTag",game__GetWorldTag);
	getGlobalNamespace(L).addFunction("game__GetWorldIndex",game__GetWorldIndex);
	getGlobalNamespace(L).addFunction("game__InvPlayerGiveItem",game__InvPlayerGiveItem);
	getGlobalNamespace(L).addFunction("game__InvPlayerGiveItem2",game__InvPlayerGiveItem2);
	getGlobalNamespace(L).addFunction("game__InvPlayerSpendItem",game__InvPlayerSpendItem);
	getGlobalNamespace(L).addFunction("game__InvGetCountPlayerItem",game__InvGetCountPlayerItem);
	getGlobalNamespace(L).addFunction("game__GetObjectVar",game__GetObjectVar);
	getGlobalNamespace(L).addFunction("game__GetPQContrib",game__GetPQContrib);
	getGlobalNamespace(L).addFunction("game__GetPQAllContrib",game__GetPQAllContrib);
	getGlobalNamespace(L).addFunction("game__SetPQContrib",game__SetPQContrib);
	getGlobalNamespace(L).addFunction("game__GetStorage",game__GetStorage);
	getGlobalNamespace(L).addFunction("game__SetStorage",game__SetStorage);
	getGlobalNamespace(L).addFunction("game__PlayerUpdateItemsProctype",game__PlayerUpdateItemsProctype);
	getGlobalNamespace(L).addFunction("game__DestroyProctypeItems",game__DestroyProctypeItems);
	getGlobalNamespace(L).addFunction("game__TestItemProctype",game__TestItemProctype);
	getGlobalNamespace(L).addFunction("game__AddItemProctype",game__AddItemProctype);
	getGlobalNamespace(L).addFunction("game__PlayerTeleport",game__PlayerTeleport);
	getGlobalNamespace(L).addFunction("game__PlayerCheckCoord",game__PlayerCheckCoord);
	getGlobalNamespace(L).addFunction("game__GetRoleVar",game__GetRoleVar);
	getGlobalNamespace(L).addFunction("game__ActivateTriggerId",game__ActivateTriggerId);
	getGlobalNamespace(L).addFunction("game__DeactivateTriggerId",game__DeactivateTriggerId);
	getGlobalNamespace(L).addFunction("game__CreateObj",game__CreateObj);
	getGlobalNamespace(L).addFunction("game__PlayerObtainTitle",game__PlayerObtainTitle);
	getGlobalNamespace(L).addFunction("game__PlayerDeleteTitle",game__PlayerDeleteTitle);		
	getGlobalNamespace(L).addFunction("game__PlayerAddGold",game__PlayerAddGold);
	getGlobalNamespace(L).addFunction("game__PlayerAddVip",game__PlayerAddVip);
	getGlobalNamespace(L).addFunction("game__PlayerLvlUp",game__PlayerLvlUp);
	getGlobalNamespace(L).addFunction("game__PetLvlUp",game__PetLvlUp);
	getGlobalNamespace(L).addFunction("game__ElfLvlUp",game__ElfLvlUp);
	getGlobalNamespace(L).addFunction("game__PlayerDisconnect",game__PlayerDisconnect);
	getGlobalNamespace(L).addFunction("game__Cooldown",game__Cooldown);
	getGlobalNamespace(L).addFunction("game__MassiveCooldown",game__MassiveCooldown);
	getGlobalNamespace(L).addFunction("game__InsRefresh",game__InsRefresh);
	getGlobalNamespace(L).addFunction("game__GetInvPosItemTable",game__GetInvPosItemTable);
	getGlobalNamespace(L).addFunction("game__PlayerGiveGold",game__PlayerGiveGold);
	getGlobalNamespace(L).addFunction("game__PlayerSpendGold",game__PlayerSpendGold);
	getGlobalNamespace(L).addFunction("game__PlayerGiveSilver",game__PlayerGiveSilver);
	getGlobalNamespace(L).addFunction("game__PlayerSpendSilver",game__PlayerSpendSilver);
	getGlobalNamespace(L).addFunction("game__LoadLuaStorage",game__LoadLuaStorage);
	getGlobalNamespace(L).addFunction("game__SaveLuaStorage",game__SaveLuaStorage);
	getGlobalNamespace(L).addFunction("game__LearnSkill",game__LearnSkill);
	getGlobalNamespace(L).addFunction("game__RemoveSkill",game__RemoveSkill);
	getGlobalNamespace(L).addFunction("game__GetHwid",game__GetHwid);
}

void LuaManager::SetItem()
{
	//SetAddr("RATE_LIST_TABLE"					, (long long)anti_wallow::list          				);
	SetAddr("REFINE_TABLE"						, (long long)refine_table          					);
	SetAddr("REFINE_FACTOR"						, (long long)refine_factor          					);
	
	SetAddr("INCAST_SKILL"						, (long long)data.incast.skill							);
	SetAddr("INCAST_CLS"						, (long long)data.incast.supressor						);
	SetAddr("INCAST_FIRSTRUN"					, (long long)data.incast.firstrun						);
	SetAddr("INCAST_COOLDOWN"					, (long long)data.incast.cd							);
	SetAddr("SKILLSENDER_COOLDOWN"				, (long long)data.skillsender.cd						);
	SetAddr("SKILLSENDER_INVALID_NPC_BUFF"		, (long long)data.skillsender.skill					);
	SetAddr("SWAP_EQUIP_COMBAT"					, (long long)data.swap.equip							);
	SetAddr("SWAP_COOLDOWN"						, (long long)data.swap.cd								);
	SetAddr("AUTOSWAP_COOLDOWN"					, (long long)data.swap.cd_auto							);
	SetAddr("BALANCE_DAMAGE"					, (long long)data.balance.data							);
	SetAddr("ATTACK_SPEED"						, (long long)data.aspeed.data							);
	SetAddr("BUG_SKILL"							, (long long)data.bugskill.skill						);
	SetAddr("BUG_SKILL_CLS"						, (long long)data.bugskill.data						);
	SetAddr("ITEM_POPPET_DUMMY_TABLE"			, (long long)data.config.ITEM_POPPET_DUMMY_TABLE	    );
	SetAddr("ITEM_WORLD_SPEAKER_TABLE"			, (long long)data.config.ITEM_WORLD_SPEAKER_TABLE	    );
	SetAddr("ITEM_SUPERWORLD_SPEAKER_TABLE"		, (long long)data.config.ITEM_SUPERWORLD_SPEAKER_TABLE	);
	SetAddr("ITEM_GATCHER_SPEED"				, (long long)data.config.ITEM_GATCHER_SPEED	        );
	SetAddr("ITEM_MASS_PICKUP"					, (long long)data.config.ITEM_MASS_PICKUP	            );
	SetAddr("ITEM_PET_RENAME"					, (long long)data.config.ITEM_PET_RENAME              	);
	SetAddr("ITEM_REFINE_FULL"					, (long long)data.config.ITEM_REFINE_FULL              );
	SetAddr("OTHER_MINE_SPEED"					, (long long)data.config.OTHER_MINE_SPEED              );
	SetAddr("ITEM_SCROLL_TOWN"					, (long long)data.config.ITEM_SCROLL_TOWN              );
	SetAddr("ITEM_OFFLINE_MARKET"				, (long long)data.config.ITEM_OFFLINE_MARKET	        );
	SetAddr("ITEM_REFINE"						, (long long)data.config.ITEM_REFINE	                );
	SetAddr("ITEM_REVIVE_SCROLL"				, (long long)data.config.ITEM_REVIVE_SCROLL	        );
	SetAddr("ITEM_ALLIANCE_SPEAKER_TABLE"		, (long long)data.config.ITEM_ALLIANCE_SPEAKER_TABLE   );
	SetAddr("DISTANCE_SERVICE_TAG"				, (long long)data.config.DISTANCE_SERVICE_TAG          );
	SetAddr("MASS_PICKUP_DISTABLE_TAG"			, (long long)data.config.MASS_PICKUP_DISTABLE_TAG      );
	SetAddr("DISTANCE_SERVICE_TYPE"				, (long long)data.config.DISTANCE_SERVICE_TYPE         );
	SetAddr("GOOD_ITEMS_TABLE"					, (long long)data.config.GOOD_ITEMS_TABLE              );
	SetAddr("GOOD_AMULETS_TAG"					, (long long)data.config.GOOD_AMULETS_TAG              );
	SetAddr("ELF_DISTABLE_LOCATIONS"			, (long long)data.config.ELF_DISTABLE_LOCATIONS        );
	SetAddr("PATTY_DISTABLE_TAGS"				, (long long)data.config.PATTY_DISTABLE_TAGS           );
	SetAddr("SHIELD_SKILL_TABLE"				, (long long)data.config.SHIELD_SKILL_TABLE            );
	SetAddr("ITEM_GLOBAL_SPEAKER_TABLE"			, (long long)data.config.ITEM_GLOBAL_SPEAKER_TABLE	    );
	SetAddr("VIP_SCORE_LEVEL_TABLE"				, (long long)data.config.VIP_SCORE_LEVEL_TABLE         );
	SetAddr("ALLIANCE_CONGREGATE_TAG"			, (long long)data.config.ALLIANCE_CONGREGATE_TAG       );
	SetAddr("ITEM_NEW_ENGRAVE"					, (long long)data.config.ITEM_NEW_ENGRAVE	  		    );
	SetAddr("ITEM_BIBLE_REFINE"					, (long long)data.config.ITEM_BIBLE_REFINE	  		    );
	SetAddr("ITEM_DELAY_SIGNIN"					, (long long)data.config.ITEM_DELAY_SIGNIN	  		    );
	SetAddr("ITEM_RECIPE_LIB_PRODUCE"			, (long long)data.config.ITEM_RECIPE_LIB_PRODUCE	    );
	SetAddr("ITEM_ITEM_LIB_REFINE"				, (long long)data.config.ITEM_ITEM_LIB_REFINE	    );
}

void LuaManager::GetItem()
{
	double res = -1;
	GetNum( "DebugPasswd"				, res ) ? data.config.debug_passwd 				= res : res == -1;
	GetNum( "SummonPet136"				, res ) ? data.config.SummonPet136 				= res : res == -1;
	GetNum( "CountryBattleIsTeam"		, res ) ? data.config.CountryBattleIsTeam 		= res : res == -1;
	GetNum( "MaxVigour"					, res ) ? data.config.MaxVigour 				= res : res == -1;
	GetNum( "shutdown_ap"				, res ) ? data.config.shutdown_ap 				= res : res == -1;
	GetNum( "IdDistanceShop"			, res ) ? data.config.IdDistanceShop 			= res : res == -1;
	GetNum( "ChangeElfStatusTime"		, res ) ? data.config.ChangeElfStatusTime 		= res : res == -1;
	GetNum( "townscroll_ccoldown"		, res ) ? data.config.townscroll_ccoldown 		= res : res == -1;
	GetNum( "enable_safelock"			, res ) ? data.config.enable_safelock 			= res : res == -1;
	GetNum( "pvp_cooldown_time"			, res ) ? data.config.pvp_cooldown_time 		= res : res == -1;
	GetNum( "pvp_protect_level_min"		, res ) ? data.config.pvp_protect_level_min 	= res : res == -1;
	GetNum( "pvp_protect_level_max"		, res ) ? data.config.pvp_protect_level_max 	= res : res == -1;
	GetNum( "vip_enable"				, res ) ? data.config.vip_enable 				= res : res == -1;
	GetNum( "max_refine_level"			, res ) ? data.config.max_refine_level 			= res : res == -1;
	GetNum( "min_refine_level"			, res ) ? data.config.min_refine_level 			= res : res == -1;	
	GetNum( "battle_bid_item"			, res ) ? data.config.battle_bid_item 			= res : res == -1;
	GetNum( "meredian_max"				, res ) ? data.config.meredian_max 				= res : res == -1;	
	GetNum( "meredian_inc"				, res ) ? data.config.meredian_inc	 			= res : res == -1;
	GetNum( "LATE_DAY_SIGNIN_ITEM"		, res ) ? data.config.LATE_DAY_SIGNIN_ITEM 		= res : res == -1;
	GetNum( "SIGNIN_PERFECT_AWARD"		, res ) ? data.config.SIGNIN_PERFECT_AWARD 		= res : res == -1;
	GetNum( "SIGNIN_YEAR_AWARD"			, res ) ? data.config.SIGNIN_YEAR_AWARD 		= res : res == -1;
	GetNum( "SIGNIN_MONTH_AWARD"		, res ) ? data.config.SIGNIN_MONTH_AWARD 		= res : res == -1;
	GetNum( "MAX_LATE_SIGNIN_TIMES"		, res ) ? data.config.MAX_LATE_SIGNIN_TIMES 	= res : res == -1;
	GetNum( "enable_patch_equip"		, res ) ? data.config.enable_patch_equip 		= res : res == -1;
	GetNum( "atk_def_enable"			, res ) ? data.config.atk_def_enable 			= res : res == -1;
	GetNum( "max_load_state"			, res ) ? data.config.max_load_state 			= res : res == -1;
	GetNum( "need_load_state"			, res ) ? data.config.need_load_state 			= res : res == -1;
	
	GetNum( "GENERALCARD_RANK_S"		, res ) ? data.config.GENERALCARD_RANK_S 		= res : res == -1;
	GetNum( "TASK_DEFAULT_MAX_COUT"		, res ) ? data.config.TASK_DEFAULT_MAX_COUT 	= res : res == -1;
	GetNum( "TASK_MAX_COUT"		    	, res ) ? data.config.TASK_MAX_COUT		    	= res : res == -1;
	GetNum( "TASK_HIDDEN_COUNT"     	, res ) ? data.config.TASK_HIDDEN_COUNT     	= res : res == -1;
	GetNum( "TASK_TITLE_TASK_COUNT"		, res ) ? data.config.TASK_TITLE_TASK_COUNT 	= res : res == -1;
	
	
	GetNum( "level_min_to_send_message"	, res ) ? data.config.level_min_to_send_message = res : res == -1; //172
	GetNum( "speed_skill_enable"	, res ) ? data.config.speed_skill_enable 	= res : res == -1;
	GetNum( "dg_buff_enable"	, res ) ? data.config.dg_buff_enable 	= res : res == -1;
	GetNum( "activity_event_enable"		, res ) ? data.config.activity_event_enable 		= res : res == -1;
	GetNum( "key_task_enable"		, res ) ? data.config.key_task_enable 		= res : res == -1;
}

void LuaManager::FunctionsExecute()
{
	msg.clear();
	data.INIT();
}

// ================================================================
// LUA EVENTS 
// ================================================================

void LuaManager::EventInit()
{
	Event("Init");
}

void LuaManager::EventUpdate()
{
	Event("Update");
}

void LuaManager::EventHeartBeat()
{
	Event("HeartBeat", tick);
}

void LuaManager::InitWorld()
{
	Event("InitWorld");
}

void LuaManager::PlayerEnterWorld(int roleid)
{
	Event("PlayerEnterWorld", roleid);
}

void LuaManager::PlayerLeaveWorld(int roleid)
{
	Event("PlayerLeaveWorld", roleid);
}

void LuaManager::EventOnPlayerKilled(int roleid, int killer, int killtype, int is_pariah, int attacker_mode )
{
	Event("EventOnPlayerKilled", roleid, killer, killtype, is_pariah, attacker_mode);

	if(killer >= 1024)
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(killer,windex1);
		if(gPlayer && gPlayer->imp)
		{
			gplayer_imp *pImp = (gplayer_imp*)gPlayer->imp;
			int kill_count = pImp->GetPlayerKillCount();
			pImp->SetPlayerKillCount(++kill_count);
		}
	}
}

void LuaManager::EventOnMonsterKilled(int mobid,int killer, int tid)
{
	Event("EventOnMonsterKilled", mobid, killer, tid);
	
	if(killer >= 1024)
	{
		int windex1;
		gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(killer,windex1);
		if(gPlayer)
		{
			gplayer_imp *pImp = (gplayer_imp*)gPlayer->imp;
			int kill_count = pImp->GetMonsterKillCount();
			pImp->SetMonsterKillCount(++kill_count);
		}
	}
}

int LuaManager::EventOnPlayerTryGather(int roleid, int mineid, int tid, int task_id)
{
	return Event<int>("EventOnPlayerTryGather", roleid, mineid, tid , task_id);
}

void LuaManager::EventPlayerRefineItem(int roleid, int item_id, int refine_level, int index)
{
	Event("EventPlayerRefineItem", roleid,item_id,refine_level,index);
}

void LuaManager::EventPlayerRefineItemFail(int roleid, int item_id, int refine_level, int index)
{
	Event("EventPlayerRefineItemFail", roleid,item_id,refine_level,index);
}

void LuaManager::EventPlayerObtianItem(int roleid, int pos, int id, int count, int proc_type)
{
	Event("EventPlayerObtianItem", roleid, pos, id, count, proc_type);
}

void LuaManager::EventPlayerHeartBeat(int roleid, int tick)
{
	Event("EventPlayerHeartBeat", roleid, tick);
}

void LuaManager::EventOnPlayerEquipChange(int roleid)
{
	Event("EventOnPlayerEquipChange", roleid);
}

int  LuaManager::EventOnPlayerChat(int roleid, int cannel, const void * message, int size)
{
	int res = 0;
	if (size >= 2 && size < 512)
	{
		static std::string utf8;
		utf8.clear();
		utf8.resize(size+1);
		memset((void*)utf8.c_str(), 0x00, utf8.size());
		utf16_to_utf8((const char16_t*)message, (char*)utf8.c_str(), utf8.size()-1);
		res = Event<int>( "EventOnPlayerChat", roleid, cannel, (const char *)utf8.c_str() );
	}
	return res;
}

int LuaManager::EventOnSendServiceRequest(int roleid, int npcid, int service_type)
{
	return Event<int>("EventOnSendServiceRequest",roleid, npcid, service_type);
}

int LuaManager::EventOnPlayerDoShopping(int roleid, int id, int count, int point)
{
	return Event<int>("EventOnPlayerDoShopping",roleid, id, count, point);
}

int LuaManager::EventOnPlayerDoDividendShopping(int roleid, int id,int count, int point)
{
	return Event<int>("EventOnPlayerDoDividendShopping",roleid, id, count, point);
}

int LuaManager::EventOnCanActivateItem(int roleid, int id, int slot, int location)
{
	return Event<int>("EventOnCanActivateItem",roleid,id,slot,location);
}

void LuaManager::EventPlayerGetInventory(int roleid, int inv)
{
	Event("EventPlayerGetInventory",roleid,inv);
}

void LuaManager::EventPlayerSave(int roleid)
{
	Event("EventPlayerSave",roleid);
}

void LuaManager::SendPlayerLuaInfo(int roleid)
{
	char buf[64];
	memset(buf, 0x00, sizeof(buf));
	Event("SendPlayerLuaInfo",roleid, roleid, (long long)buf, (unsigned int)sizeof(buf));
	GMSV::SendPlayerLuaInfo( roleid, 0, 0,  buf, sizeof(buf));
}

// ================================================================
// LUA OTHER EVENTS
// ================================================================

int LuaManager::EventOnPlayerUseItem(int roleid, int item_id)
{
	
	// 0 продолжать по дефолту функцию ;
	// 1 не удалять итем и завершать функцию ;
	// 2 удалять итем и завершать функцию ;
	
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer)
	{
		DATA_TYPE dt;
		TASKDICE_ESSENCE *it = (TASKDICE_ESSENCE*)world_manager::GetDataMan().get_data_ptr(item_id,ID_SPACE_ESSENCE,dt);
		if ( dt != DT_TASKDICE_ESSENCE ) return 0;
		int cmd_type = it->task_lists[0].id;
		if (cmd_type < 400400 || cmd_type > 400500) return 0;
		gplayer_imp *pImp = (gplayer_imp*)gPlayer->imp;
		switch (cmd_type)
		{
			case MASK_GOLDADD_ITEM: //400400
			{
				int newgold = it->task_lists[1].id;
				if( newgold )
				{
					pImp->AddMallCash(newgold);
					pImp->_runner->player_cash(pImp->GetMallCash());
					return it->task_lists[19].id;
				}
			}
			break;
			
			case MASK_AUTOSWAP_ITEM: //400401
			{
				int swap_count = it->task_lists[1].id; //максимальное количество ячеек для свапа
				pImp->AutoSwapRelease(swap_count);
				return it->task_lists[19].id;
			}
			break;
			
			/*
			case MASK_SKILLSENDER_ITEM: //400402
			{
				int skill_count = it->task_lists[1].id; //максимальное количество скиллов в макросе
				pImp->SkillSenderStartByItem(skill_count);
				return it->task_lists[19].id;
			}
			break;
			*/
			
			case MASK_LVLUP: //400403
			{
				int max_lvl = it->task_lists[1].id; //максимальный уровень который можно апнуть
				int cnt_lvl = it->task_lists[2].id; //количество лвлов которые апнуть
				int res = game__PlayerLvlUp(roleid,max_lvl,cnt_lvl);
				if (!res) return 1;
				return it->task_lists[19].id;	
			}
			break;
			
			case MASK_PETLVLUP: //400404
			{
				int max_lvl_pet = it->task_lists[1].id; //максимальный уровень который можно апнуть
				int cnt_lvl_pet = it->task_lists[2].id; //количество лвлов которые апнуть
				int res = game__PetLvlUp(roleid,max_lvl_pet,cnt_lvl_pet);
				if (!res) return 1;
				return it->task_lists[19].id;
			}
			break;
			
			case MASK_ELFLVLUP: //400405
			{
				int max_lvl_elf = it->task_lists[1].id; //максимальный уровень который можно апнуть
				int cnt_lvl_elf = it->task_lists[2].id; //количество лвлов которые апнуть
				int req_exp_elf = it->task_lists[3].id; //требовать опыт 
				int req__sp_elf = it->task_lists[4].id; //требовать дух
				int res = game__ElfLvlUp(roleid,max_lvl_elf,cnt_lvl_elf,req_exp_elf,req__sp_elf);
				if (!res) return 1;
				return it->task_lists[19].id;
			}
			break;
			
			case MASK_VIPADD: //400406
			{
				int newvip = it->task_lists[1].id;
				if( newvip )
				{
					pImp->GetCashVipInfo().AddCashVipScore(newvip,(gplayer *)pImp->_parent);
					return it->task_lists[19].id;
				}
			}
			break;
			
			case MASK_LUA: //400407
			{
				if(pImp->CheckAndSetCoolDown(COOLDOWN_IDX_LUA_ITEM,MASK_DELAY_LUA))
				{
					return Event<int>("EventOnPlayerUseItem",roleid,item_id);
				}
				return 1;
			}
			break;

			case MASK_ALLIANCECONGREGATE:
			{
				pImp->FactionAllianceCongregateRequest();
				return it->task_lists[19].id;
			}
			break;
			
			case MASK_ASSIST:
			{
				pImp->ChangeLeaderAssist();
				return it->task_lists[19].id;
			}
			break;
			
			case MASK_AUTOASSIST:
			{
				int is_enable = pImp->SetAutoAssist();
				if(pImp->CheckAndSetCoolDown(COOLDOWN_IDX_AUTOASSIST,MASK_DELAY_LUA))
				{
					Event("EventOnPlayerUseItem",roleid,is_enable);
				}
				return it->task_lists[19].id;
			}
			break;
			
			case MASK_GIVE_ALL_TITTLE:
			{
				pImp->GetPlayerTitle().GiveAllTittle();
				return it->task_lists[19].id;
			}
			break;
			
			
			
		}
	
	}
	return 0; //config.IS_GOOD_ITEM(item_id) ? 1 : 0;
}

void LuaManager::EventOnCommandHandler(int roleid, int cmd_type, const void * buf, unsigned int size)
{
	int windex1;
	gplayer * gPlayer = world_manager::GetInstance()->FindPlayer(roleid,windex1);
	if(gPlayer && size >= sizeof(DEBUG_CMD))
	{
		DEBUG_CMD * cmd = (DEBUG_CMD *)buf;
		gplayer_imp *pImp = (gplayer_imp*)gPlayer->imp;
		gplayer_controller * pCtrl = ((gplayer_controller*)pImp->_commander);
		
		switch(cmd_type)
		{
			case CMD_INSTANCE_REFRESH: //обновить локации
			{
				if(game__InsRefresh(roleid))
				{
					Event("EventOnInsRefresh",roleid);
					return;
				}
				break;
			}
			case CMD_MONEY: //обменять юани на чеки
			{
				pImp->PlayerChangeMoney(false);
				return;
				break;
			}
			case CMD_MONEYS: //обменять все юани на чеки
			{
				pImp->PlayerChangeMoney(true);
				return;
				break;
			}	
			
			/*
			case CMD_SKILL_SET: //запомнить скрипт скилл сендера
			{
				pImp->SetSkillSender(buf,size);
				return;
				break;
			}
			case CMD_SKILL_SENDER: //кинуть дорогу
			{
				if(size == 4 && pCtrl->GetDebugMode() && pCtrl->GetGMPrivelege())
				{
					pImp->SkillSenderStart(cmd->arg[0]);
					return;
				}
				break;
			}
			case CMD_SKILL_SCRIPT: //кинуть один скилл с помощью скрипта
			{
				if(size == 8 && pCtrl->GetDebugMode() && pCtrl->GetGMPrivelege())
				{
					pImp->SkillSenderRelease(cmd->arg[0],cmd->arg[1],cmd->arg[2]);
					return;
				}
				break;
			}
			*/
			
			case CMD_SHORT_JUMP:
			{
				if ( pCtrl->GetDebugMode() && pCtrl->GetGMPrivelege() )
				{
					world::object_info info;
					if ( pImp->_plane->QueryObject(pCtrl->GetCurTarget(),info) )
					{
						A3DVECTOR newpos = info.pos;
						pImp->ShortJump(newpos);
					}
				}
			}
			
			
			case CMD_AUTOSWAP: //запомнить скрипт автосвапа
			{
				pImp->SetAutoSwap((short*)cmd->arg,(size-2));
				break;
			}
			/*
			case CMD_FILTER_SKILL: //вставить фильтр скиллов
			{
				if(size >= 6)
				{
					pImp->SkillSenderSetFilter(cmd->arg[0]);
					return 1;
				}
				break;
			}
			*/
			case CMD_DEBUG_PWD:
			{
				if (size == 6)
				{
					DEBUG_CMD * pwd = (DEBUG_CMD *)buf;
					if( pwd->arg[0] == data.config.debug_passwd )
						pCtrl->SetDebugMode(true);
					return;
				}
				break;
			}
				
			default:
				break;	
		}
	
		if(cmd_type >= CMD_LUA_BEGIN && cmd_type <= CMD_LUA_END && pImp->CheckAndSetCoolDown(COOLDOWN_IDX_CMD,CMD_DELAY))
		{
			return Event("EventOnCommandHandler",roleid, cmd_type, (long long)buf, (unsigned int)size);
		}
	}
}

// ================================================================
// LUA DEBUG EVENTS
// ================================================================

void LuaManager::EventOnPlayerAttackMsg(int roleid, int attacker, int acls, int pcls, void * d)
{
	if(roleid >= 1024 && attacker >= 1024 && d)
	{
		data.balance.RELEASE(acls , pcls , d);
	}
	if(lua_debug)
	{
		Event("EventOnPlayerAttackMsg",roleid, attacker);
	}
}

void LuaManager::EventNpcHeartBeat(int npcid, int tick)
{
	if(lua_debug)
	{
		Event("EventNpcHeartBeat",npcid, tick);
	}
}

void LuaManager::EventOnRecvCmd(int roleid, int cmd_type, const void * buf, int size)
{
	if(lua_debug)
	{
		Event("EventOnRecvCmd",roleid, cmd_type, (long long)buf, size);
	}
}

void LuaManager::EventOnSendCmd(int roleid, int cmd_type, const void * buf, int size)
{
	if(lua_debug)
	{
		Event("EventOnSendCmd",roleid, cmd_type, (long long)buf, size);
	}
}

// ================================================================
// LUA CONSTRUCTOR 
// ================================================================

LuaManager::LuaManager()
{

}

LuaManager::~LuaManager()
{
	
}

LuaManager * LuaManager::GetInstance()
{
	if (!instance)
	{
		instance = new LuaManager();
	}
	return instance;
}



