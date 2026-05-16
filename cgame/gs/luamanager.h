//---------------------------------------------------------------------------------------------------------------------------
//--PW LUA SCRIPT GDELIVERYD (C) DeadRaky 2022
//---------------------------------------------------------------------------------------------------------------------------
#ifndef __GNET_LUAMANAGER_H
#define __GNET_LUAMANAGER_H

#include <db_if.h>
#include <luabase.h>
#include "playertemplate.h"

#pragma pack(push, 1)

class LuaManager : public LuaBase
{
public:
	enum : size_t
	{
		MIN  					= 60,
		HOUR 					= MIN*60,
		DAY  					= HOUR*24,
		WEEK 					= DAY*7,
		
		COOLDOWN_IDX_CMD		= 8500,
		COOLDOWN_IDX_LVL_UP		= 8501,
		COOLDOWN_IDX_LVL_UP_PET	= 8502,
		COOLDOWN_IDX_LVL_UP_ELF	= 8503,
		COOLDOWN_IDX_INS_REFRESH= 8504,
		COOLDOWN_IDX_LUA_ITEM	= 8505,
		COOLDOWN_IDX_AUTOASSIST	= 8508,
		COOLDOWN_CHECK			= 0,
		COOLDOWN_SET			= 1,
		COOLDOWN_CHECK_AND_SET	= 2,
		COOLDOWN_MAX_COUNT		= 4,
		
		MAX_ROLE_COUNT 			= 6000,
		MAX_TAGS				= 128,
		MAX_SKILL				= 128,
		MAX_ITEM				= 128,
		
		TYPE_INTERVAL  			= 1,   //событие по интервалу
		TYPE_TIME_WEEK 			= 2,   //событие в определённое время раз в неделю
		TYPE_TIME_DAY  			= 3,   //событие раз в день
		TYPE_TIME_HOUR 			= 4,   //событие раз в час

		EVENT_VECTOR_COUNT		= 32,

		MAX_LVL_ELF 			= 105,
		MAX_LVL_PET 			= 999,
		MAX_LVL 				= 999,
		LVLUP_COOLDOWN_TIME		= 15000,

		PACKET_CLEAN 			= 0,
		PACKET_PUSH 			= 1,
		PACKET_ADDR		 		= 2,
		PACKET_SIZE 			= 3,
		
		TYPE_CHAR 				= 0,
		TYPE_SHORT 				= 1,
		TYPE_INT 				= 2,
		TYPE_INT64 				= 3,
		TYPE_FLOAT 				= 4,
		TYPE_DOUBLE 			= 5,
			
		CHAT_SINGLE				= 0,
		CHAT_INSTANCE			= 1,
		CHAT_WORLD				= 2,
		CHAT_MAX				= 3,
			
		NAME_ROLE				= 0,
		NAME_ITEM				= 1,
			
		NPC_TID_BY_ID			= 0,
		NPC_ID_BY_TID			= 1,
		NPC_COORDS_BY_TID		= 2,
		MINE_GET_TID_BY_ID		= 3,
			
		ITEM_CHANGE_PROCTYPE	= 0,
		ITEM_ADD_PROCTYPE		= 1,
		ITEM_CHANGE_PROCTYPE_ALL= 2,
		ITEM_ADD_PROCTYPE_ALL	= 3,

		PIMP_CLS                = 0,
		PIMP_SEC_LEVEL          = 1,
		PIMP_LEVEL              = 2,
		PIMP_SP                 = 3,
		PIMP_MONEY              = 4,
		PIMP_USERID             = 5,
		PIMP_REPUTATION	        = 6,
		PIMP_FACTIONID	        = 7,
		PIMP_FACTIONRANK        = 8,
		PIMP_FACTIONMASTER      = 9,
		PIMP_PVPFLAG            = 10,
		PIMP_PKZONE	            = 11,
		PIMP_GETMAFIAID         = 12,
		PIMP_FORCE				= 13,
		PIMP_SILVER             = 14,
		PIMP_SEC_FLAG           = 15,		
		
		CTRL_GM                 = 100,
		CTRL_DEBUG              = 101,
		CTRL_TARGET             = 102,
		CTRL_FACTION			= 103,
		
		CMD_DELAY				= 1000,
		CMD_INSTANCE_REFRESH 	= 700,
		CMD_MONEY 				= 701,
		CMD_MONEYS 				= 702,
		CMD_OFFLINE_ROLE 		= 721,
		CMD_OFFLINE_CAT 		= 722,
		CMD_SKILL_SET 			= 703,
		CMD_SKILL_SENDER 		= 704,
		CMD_SKILL_SCRIPT 		= 705,
		CMD_PLAYER_DUMP 		= 706,
		CMD_PLAYER_INFO 		= 707,
		CMD_AUTOSWAP 			= 708,
		CMD_ELEMENTSKILL 		= 709,
		CMD_TEST 				= 710,
		CMD_CLIENT_SKILL 		= 711,
		CMD_FILTER_SKILL 		= 712,
		CMD_SHORT_JUMP			= 714,
		CMD_DEBUG_PWD			= 777,
		
		CMD_LUA_BEGIN			= 800,
		CMD_LUA_END				= 900,
		
		MASK_DELAY_LUA			= 400,
		MASK_GOLDADD_ITEM 		= 400400,
		MASK_AUTOSWAP_ITEM 		= 400401,
		MASK_SKILLSENDER_ITEM 	= 400402,
		MASK_LVLUP 				= 400403,
		MASK_PETLVLUP 			= 400404,
		MASK_ELFLVLUP 			= 400405,
		MASK_VIPADD 			= 400406,
		MASK_LUA 				= 400407,
		MASK_ALLIANCECONGREGATE	= 400408,
		MASK_ASSIST				= 400409,
		MASK_AUTOASSIST			= 400410,
		MASK_GIVE_ALL_TITTLE	= 400411,
		
		CREATE_MOB				= 0,
		CREATE_NPC				= 1,
		CREATE_MINE				= 2,
		
		STORAGE_TAG				= 0,
		STORAGE_USER			= 1,
		STORAGE_ROLE			= 2,
		STORAGE_OTHER			= 3,
		STORAGE_MAX				= 4,
	};
public:

	struct DEBUG_CMD
	{
		short cmd;
		int arg[];
		DEBUG_CMD();
	};
	
	struct COOLDOWN
	{
		int xid;
		time_t time;
		COOLDOWN();
	};

	struct HW_MASTER
	{
		short cmd;
		unsigned long long Salt;
		HW_MASTER();
	};
	
	struct INCAST
	{
		COOLDOWN cd[COOLDOWN_MAX_COUNT];
		int supressor[USER_CLASS_COUNT];
		
		struct
		{
			int id;
			time_t time;
		}	skill[MAX_SKILL];
		
		struct 
		{
			int time;
			int max_time;
		}	firstrun[USER_CLASS_COUNT];
		
		INCAST();
		void INIT();
		int RELEASE(int id, int cls);
		int FIRSTRUN(int cls, int cast);
	};
	
	struct SKILLSENDER
	{
		int skill[MAX_SKILL];
		COOLDOWN cd[COOLDOWN_MAX_COUNT];
		SKILLSENDER();
		void INIT();
		bool INVALID(int id, int tid, int filter);
	};
	
	struct SWAP
	{
		int equip[MAX_ITEM];
		COOLDOWN cd[COOLDOWN_MAX_COUNT];
		COOLDOWN cd_auto[COOLDOWN_MAX_COUNT];
		SWAP();
		void INIT();
	};
	
	struct BALANCE
	{
		struct
		{
			struct
			{
				float fiz;
				float mag;
			}	cls[USER_CLASS_COUNT];
		}	data[USER_CLASS_COUNT];
		
		struct DMG
		{
			float fiz;
			float mag[5];
		};
		
		BALANCE();
		void INIT();
		void RELEASE(int rcls, int tcls, void * d);
	};
	
	struct ASPEED
	{
		struct 
		{
			struct
			{
				short id, speed;
			}	type[20];
		}	data[3];
		
		ASPEED();
		void INIT();
		int RELEASE(int weapon, int speed, int cultivate);
	};
	
	struct BUGSKILL
	{
		struct 
		{
			int id;
			int enable;
		}	skill[MAX_SKILL];
		int data[USER_CLASS_COUNT];
		BUGSKILL();
		void INIT();
		int RELEASE(int id, int cls);
	};
	
	struct CONFIG
	{
		int debug_passwd;
		int SummonPet136;
		int CountryBattleIsTeam;
		int MaxVigour;
		int shutdown_ap;
		int IdDistanceShop;
		int ChangeElfStatusTime;
		int townscroll_ccoldown;
		int enable_safelock;
		int pvp_cooldown_time;
		int pvp_protect_level_min;
		int pvp_protect_level_max;
		int vip_enable;
		int min_refine_level;
		int max_refine_level;
		int battle_bid_item;
		int meredian_max; 
		int meredian_inc;	
		int enable_patch_equip;
		int atk_def_enable;
		int max_load_state;
		int need_load_state;
		int level_min_to_send_message; // 172
		int speed_skill_enable;
		int dg_buff_enable;
		int activity_event_enable;
		int key_task_enable;
		
		int LATE_DAY_SIGNIN_ITEM;	
		int SIGNIN_PERFECT_AWARD;
		int SIGNIN_YEAR_AWARD;
		int SIGNIN_MONTH_AWARD;
		int MAX_LATE_SIGNIN_TIMES;
		int GENERALCARD_RANK_S;
		int TASK_DEFAULT_MAX_COUT;
		int TASK_MAX_COUT;
		int TASK_HIDDEN_COUNT;
		int TASK_TITLE_TASK_COUNT;
		int ITEM_POPPET_DUMMY_TABLE[16];
		int ITEM_WORLD_SPEAKER_TABLE[16];
		int ITEM_SUPERWORLD_SPEAKER_TABLE[16];
		int ITEM_GATCHER_SPEED[16];
		int ITEM_MASS_PICKUP[16];
		int ITEM_PET_RENAME[16];
		int ITEM_REFINE_FULL[16];
		int OTHER_MINE_SPEED[128];
		int ITEM_SCROLL_TOWN[16];
		int ITEM_OFFLINE_MARKET[16];
		int ITEM_REFINE[16];
		int ITEM_REVIVE_SCROLL[16];
		int ITEM_ALLIANCE_SPEAKER_TABLE[16];
		int DISTANCE_SERVICE_TAG[128];
		int MASS_PICKUP_DISTABLE_TAG[128];
		int DISTANCE_SERVICE_TYPE[128];
		int GOOD_ITEMS_TABLE[128];
		int GOOD_AMULETS_TAG[16];
		int ELF_DISTABLE_LOCATIONS[16];
		int PATTY_DISTABLE_TAGS[16];
		int SHIELD_SKILL_TABLE[128];
		int ITEM_GLOBAL_SPEAKER_TABLE[16];
		int VIP_SCORE_LEVEL_TABLE[16];
		int ALLIANCE_CONGREGATE_TAG[16];
		int ITEM_NEW_ENGRAVE[16];
		int ITEM_BIBLE_REFINE[16];
		int ITEM_DELAY_SIGNIN[16]; //172
		int ITEM_RECIPE_LIB_PRODUCE[128];
		int ITEM_ITEM_LIB_REFINE[16];
		
		CONFIG();
		void INIT();
	};
	
	struct MEMSPACE
	{
		CONFIG config;
		INCAST incast;
		SKILLSENDER skillsender;
		SWAP swap;
		BALANCE balance;
		ASPEED aspeed;
		BUGSKILL bugskill;
		MEMSPACE();
		void INIT();
	};
	
	
public:
	static MEMSPACE data;
	static abase::octets msg;
	static LuaManager * instance;
public:
	LuaManager();
	~LuaManager();
	
	CONFIG * GetConfig() { return &data.config; }
	INCAST * GetIncast() { return &data.incast; }
	SKILLSENDER * GetSkillsender() { return &data.skillsender; }
	SWAP * GetSwap() { return &data.swap; }
	BALANCE * GetBalance() { return &data.balance; }
	ASPEED * GetAttackSpeed() { return &data.aspeed; }
	BUGSKILL * GetBugSkill() { return &data.bugskill; }
	
	bool IsTrueMune(int it) { return  ((it >= 3074 && it <= 3553) || (IsTrue(it,data.config.OTHER_MINE_SPEED))); }
	bool IsGoodItem(int it) { return IsTrue(it,data.config.GOOD_ITEMS_TABLE); }
	
	
public:
	virtual void FunctionsRegister();
	virtual void FunctionsExecute();
	virtual void GetItem();
	virtual void SetItem();
public:
	virtual void EventInit();
	virtual void InitWorld();
	virtual void EventUpdate();
	virtual void EventHeartBeat();
	virtual void PlayerEnterWorld(int roleid);
	virtual void PlayerLeaveWorld(int roleid);
	virtual void EventOnPlayerKilled(int roleid, int killer, int killtype, int is_pariah, int attacker_mode );
	virtual void EventOnMonsterKilled(int mobid,int killer, int tid);
	virtual void EventOnCommandHandler(int roleid, int cmd_type, const void * buf, unsigned int size);
	virtual void EventOnPlayerAttackMsg(int roleid, int attacker, int acls, int pcls, void * d);
	virtual void EventOnRecvCmd(int roleid, int cmd_type, const void * buf, int size);
	virtual void EventOnSendCmd(int roleid, int cmd_type, const void * buf, int size);
	virtual void EventPlayerRefineItem(int roleid, int item_id, int refine_level, int index);
	virtual void EventPlayerRefineItemFail(int roleid, int item_id, int refine_level, int index);
	virtual void EventPlayerObtianItem(int roleid, int pos, int id, int count, int proc_type);
	virtual void EventPlayerHeartBeat(int roleid, int tick);
	virtual void EventNpcHeartBeat(int npcid, int tick);
	virtual void EventOnPlayerEquipChange(int roleid);
	virtual void EventPlayerGetInventory(int roleid, int inv);
	virtual void EventPlayerSave(int roleid);

	virtual int  EventOnPlayerUseItem(int roleid, int item_id);
	virtual int  EventOnPlayerTryGather(int roleid, int mineid, int tid, int task_id);
	virtual int  EventOnPlayerChat(int roleid, int cannel, const void * message, int size);
	virtual int  EventOnSendServiceRequest(int roleid, int npcid, int service_type);
	virtual int  EventOnPlayerDoShopping(int roleid, int id, int count, int point);
	virtual int  EventOnPlayerDoDividendShopping(int roleid, int id,int count, int point);
	virtual int  EventOnCanActivateItem(int roleid, int id, int slot, int location);
	
	virtual void SendPlayerLuaInfo(int roleid);
	
public:
	static double game__Packet(int cmd, int type, double value );
	static void game__Send(int roleid);
	static void game__SendBroadCast(int roleid);
	static void game__SaveFile(long long Src, unsigned int size, const char* name);
	static long long game__LoadFile(long long  Src, unsigned int size, const char* name);
	static void game__ChatMsg(int type , int roleid, int channel, const char * utf8_msg, int shift_pos);
	static const char * game__GetName(int type, int id);
	static long long game__GetWorldTag();
	static long long game__GetWorldIndex();
	static long long game__InvPlayerGiveItem(int roleid, int inv, int id, int count, int time, int proctype);
	static long long game__InvPlayerGiveItem2(int roleid, int id, int count);
	static long long game__InvPlayerSpendItem(int roleid, int inv, int id, int count);
	static long long game__InvGetCountPlayerItem(int roleid, int inv, int id);
	static long long game__GetObjectVar(int type, int id, int tid);
	static long long game__GetPQContrib(int task_id, int role_id);
	static long long game__GetPQAllContrib(int task_id);
	static void game__SetPQContrib(int task_id, int role_id, int value);
	static double game__GetStorage(int roleid, int pos);
	static void game__SetStorage(int roleid, int pos, double value);
	static void game__PlayerUpdateItemsProctype(int roleid, int inv, int id, int proctype, int type);
	static void game__DestroyProctypeItems(int roleid, int inv, int proctype);
	static void game__TestItemProctype(int roleid, int pos, int proctype);
	static void game__AddItemProctype(int roleid, int pos, int proctype);
	static void game__PlayerTeleport(int roleid, int tag, double x, double y, double z);
	static double game__PlayerCheckCoord(int roleid, int tag, double radius, double x, double y, double z);
	static double game__GetRoleVar(int type , int roleid);
	static void game__ActivateTriggerId(int trigger);
	static void game__DeactivateTriggerId(int trigger);
	static void game__CreateObj(int type ,int roleid, int id, int time, int count);
	static void game__PlayerObtainTitle(int roleid, int title, int time);
	static void game__PlayerDeleteTitle(int roleid, int title);
	static void game__PlayerAddGold(int roleid, int count);
	static void game__PlayerAddVip(int roleid, int count);
	static long long game__PlayerLvlUp(int roleid, int max_level, int count);
	static long long game__PetLvlUp(int roleid, int max_level, int count);
	static long long game__ElfLvlUp(int roleid, int max_level, int count, int exp, int sp );
	static void game__PlayerDisconnect(int roleid);
	static long long game__Cooldown(int type, int roleid, int idx, time_t time);
	static long long game__MassiveCooldown(int type, int roleid,long long table);
	static long long game__InsRefresh(int roleid);
	static long long game__GetInvPosItemTable(int roleid, int inv,long long table);
	static void game__PlayerGiveGold(int roleid, int money);
	static void game__PlayerSpendGold(int roleid, int money);
	static void game__PlayerGiveSilver(int roleid, int money);
	static void game__PlayerSpendSilver(int roleid, int money);
	static void game__LoadLuaStorage(int type, int key);
	static void game__SaveLuaStorage(int type, int key,long long buf, unsigned int size);
	static void game__LearnSkill(int roleid, int skill_id, int skill_level);
	static void game__RemoveSkill(int roleid, int skill_id);
	static void game__LoadTrashBox(int roleid);
	static void game__SaveTrashBox(int roleid, GDB::itemlist & box4 , GDB::itemlist & box5 , GDB::itemlist & box6 , GDB::itemlist & box7);
	static const char * game__GetHwid(int roleid);

public:
	static LuaManager * GetInstance();
};


#pragma pack(pop)


#endif