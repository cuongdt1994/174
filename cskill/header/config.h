#ifndef __ONLINEGAME_GS_CONFIG_H__
#define __ONLINEGAME_GS_CONFIG_H__

#define MAX_MATTER_COUNT 	64000		//Cannot exceed 65536*16
#define MAX_NPC_COUNT 		164000		//Cannot exceed 65536*16
#define MAX_PLAYER_COUNT 	4096
#define MAX_CS_NUM		1024
#define MAX_GS_NUM		1024


#define ITEM_LIST_SIZE		50
//#define EQUIP_LIST_SIZE		13	//For the definition of each slot, see item.h
#define TASKITEM_LIST_SIZE	20

#define PICKUP_DISTANCE		10.f
#define MAGIC_CLASS		5
#define MAX_MAGIC_FACTION	50
#define MAX_MESSAGE_LATENCY	256
#define MAX_AGGRO_ENTRY		50
#define MAX_SOCKET_COUNT	4
#define MAX_SPAWN_COUNT		10240
#define MAX_PLAYER_SESSION	64
#define NORMAL_COMBAT_TIME	5
#define MAX_COMBAT_TIME		30
#define MAX_HURT_ENTRY		512		//Maximum number of entries in damage list
#define LOGOUT_TIME_IN_COMBAT   15		//15 seconds logout time

#define NPC_IDLE_TIMER		20		//20 seconds to enter the IDLE state
#define NPC_IDLE_HEARTBEAT	30		//An idle state heartbeat is performed every 60 normal heartbeats
#define LEAVE_IDLE_MSG_LIMIT	40		//40 ticks are sent once, which is limited to 2 seconds
#define TICK_PER_SEC		20		//How many ticks are there in one second, this value cannot be changed arbitrarily

#define GET_EQUIP_INFO_DIS	150.f		//Limit distance for obtaining equipment information (horizontal direction)
#define TEAM_INVITE_DIS		50.f		//invitation limit distance
#define TEAM_EXP_DISTANCE	100.f		//The limit distance of team experience
#define TEAM_ITEM_DISTANCE	100.f		//The distance at which the group randomly distributes items
#define NORMAL_EXP_DISTANCE	100.f		//The limit distance of team experience
#define TEAM_MEMBER_CAPACITY	10
#define TEAM_WAIT_TIMEOUT	5		//5 seconds, or 5 heartbeats
#define TEAM_INVITE_TIMEOUT	30		//Invite timeout 30 seconds, Invite timeout
#define TEAM_INVITE_TIMEOUT2	25		//Invitation timeout The timeout of the invitee should be less than the timeout of the inviter above
#define TEAM_LEADER_TIMEOUT	30		//Thirty seconds member did not receive a message from the captain, that is timeout
#define TEAM_MEMBER_TIMEOUT	15		//15 seconds without receiving a message from a player, that is, a timeout
#define TEAM_LEADER_UPDATE_INTERVAL 20		//The leader updates all data every 20 seconds
#define TEAM_LEADER_NOTIFY_TIME	10		//Every 10 seconds the captain will notify all team members that he is still there
#define MAX_PROVIDER_PER_NPC	32
#define MAX_SERVICE_DISTANCE	10.f		//The maximum service distance is 10.f meters
#define PURCHASE_PRICE_SCALE	0.3f		//The ratio of the price of the item to the NPC
#define DURABILITY_UNIT_COUNT	100		//A unit of durability displayed on the outside corresponds to the value inside
#define REPAIR_PRICE_FACTOR	0.1f		//The factor by which the base price is multiplied when repairing
#define TOSSMATTER_USE_TIME	40		//The use time of hidden weapons is fixed at 1.5 seconds
#define MAX_TOWN_REGION		1024
#define LINK_NOTIFY_TIMER	33
#define MAX_EXTEND_OBJECT_STATE 32		//The maximum number of extended attributes that exist at the same time for each object
#define UNARMED_ATTACK_DELAY	12		//Unarmed attack delay time is 0.3 seconds
#define HELP_RANGE_FACTIOR	2.0f		//Factors of distress distance and vision distance
#define MAX_AGGRESSIVE_RANGE	10.f		//Maximum active npc hunting range
#define MAX_INVADER_ENTRY_COUNT 10		//The maximum number of records given to the orange name object

#endif
