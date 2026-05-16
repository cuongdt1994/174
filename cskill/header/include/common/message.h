#ifndef __ONLINEGAME_COMMON_MESSAGE_H__
#define __ONLINEGAME_COMMON_MESSAGE_H__

#include <stdlib.h>
#include "ASSERT.h"
#include <amemory.h>
struct MSG 
{
	int 	message;	//message type
	struct XID target;	//The target of receiving messages may be servers, players, items, NPCs, etc.
	struct XID source;	//Where did it come from, the possible id is the same as above
	A3DVECTOR pos;		//The position when the message is sent, some messages may have no effect on the position
	int	ttl;		//time to live, if this value is less than 0, then it will not be forwarded again
	int	param;		//A parameter, if this parameter is sufficient, then use this parameter
	size_t 	content_length;	//The specific data length of the message
	const void * content;	//The specific data of the message This field is invalid when it is transmitted on the network
private:
	enum {FAST_ALLOC_LEN = 128};
	friend void * SerializeMessage(const MSG &);
	friend void FreeMessage(MSG *);
};

inline void * SerializeMessage(const MSG & msg)
{
	void * buf;
	size_t length = msg.content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
	//	printf("%d %dalloced\n",sizeof(MSG) + length,msg.message);
		buf = abase::fast_allocator::align_alloc(sizeof(MSG) + length);		//must be aligned, considering multiple msg
		memcpy(buf,&msg,sizeof(MSG));
		if(length)
		{
			memcpy((char*)buf + sizeof(MSG),msg.content,length);
		}
	}
	else
	{
		buf = abase::fast_allocator::raw_alloc(sizeof(MSG) + length);
		memcpy(buf,&msg,sizeof(MSG));
		memcpy((char*)buf + sizeof(MSG),msg.content,msg.content_length);
	}
	return buf;
}

inline MSG * DupeMessage(const MSG & msg)
{
	MSG * pMsg = (MSG*)SerializeMessage(msg);
	pMsg->content = ((char*)pMsg) + sizeof(MSG);
	return pMsg;
}

inline void FreeMessage(MSG * pMsg)
{
	ASSERT(pMsg->content == ((char*)pMsg) + sizeof(MSG));
	size_t length = pMsg->content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
		abase::fast_allocator::align_free(pMsg, sizeof(MSG) + length);
	}
	else
	{
		abase::fast_allocator::raw_free(pMsg);
	}
}
inline void BuildMessage(MSG & msg, int message, const XID &target, const XID & source,
			const A3DVECTOR & pos,int param = 0,
			const void * content = NULL,size_t content_length = 0)
{
	msg.message = message;
	msg.target = target;
	msg.source = source;
	msg.pos = pos;
	msg.ttl = 2;
	msg.param = param;
	msg.content_length = content_length;
	msg.content = content;
}

enum
{
//	normal message
	GM_MSG_NULL,				//Empty news
	GM_MSG_FORWARD_USERBC,			//forwarded user broadcast
	GM_MSG_FORWARD,				//forwarded message, the content should be interpreted as a new message content
	GM_MSG_FORWARD_BROADCAST,		//forwarded message broadcast message,content is another complete message
	GM_MSG_USER_GET_INFO,			//The user obtains the necessary data

//5
	GM_MSG_IDENTIFICATION,			//The server The original type must be server and id is its symbolidentity,原的类型必须是server并且id是他的符号
	GM_MSG_SWITCH_GET,			//Get user data, server switch, get user data param is tag, content is key
	GM_MSG_SWITCH_USER_DATA,		//User data, response of SWITCH_GET
	GM_MSG_SWITCH_NPC,			//NPC switch server
	GM_MSG_USER_MOVE_OUTSIDE,		//User moves across borders

//10	
	GM_MSG_USER_NPC_OUTSIDE,		//The NPC moves at the border, the difference is that the NPC does not need to fetch the objects of the newly seen area
	GM_MSG_ENTER_WORLD,			//For the controller, indicating that the user has entered the world
	GM_MSG_ATTACK,				//Both target and source must be individuals
	GM_MSG_SKILL,				//Both target and source must be individuals
	GM_MSG_PICKUP,				//Pick up items, the target is usually an item

//15
	GM_MSG_FORCE_PICKUP,			//Mandatory to pick up items without verifying the author's ID and team ID
	GM_MSG_PICKUP_MONEY,			//The item notifies the user that the money has been picked up. Param is the amount of money. Who discarded the content?
	GM_MSG_PICKUP_TEAM_MONEY,		//The item notifies the team leader to pick up the money. Param is the amount of money. Who discarded the content?
	GM_MSG_RECEIVE_MONEY,			//Notify player to get money (possibly team up)
	GM_MSG_PICKUP_ITEM,			//The item notifies the user that the item has been picked param is palyer_id | 0x80000000(if teaming)

//20
	GM_MSG_ERROR_MESSAGE,			//Let the player send an error message
	GM_MSG_NPC_SVR_UPDATE,			//NPC has a server switch, this message is only sent to the original NPC in the removed state
	GM_MSG_EXT_NPC_DEAD,			//Death messages for external NPCs(really delete), this message is only sent to native NPCs in the removed state
	GM_MSG_EXT_NPC_HEARTBEAT,		//The heartbeat of the external NPC, used to determine whether it is timed out 
	GM_MSG_WATCHING_YOU,			//Active monster activation message,Sent by the player or npc, followed by a watching_t structure

//25
//	AGGRO  message 
	GM_MSG_GEN_AGGRO,			//Generate aggro, followed by an aggro_info_t structure
	GM_MSG_TRANSFER_AGGRO,			//The transmission of aggro currently only transmits the first content. The content is an XID. If the id of the XID is -1, the hatred list will be cleared. Param is the hatred value of the person
	GM_MSG_AGGRO_ALARM,			//aggro alarm, which will be sent when under attack, followed by an aggro_alarm_t unused
	GM_MSG_AGGRO_WAKEUP,			//aggro alarm, wake up the dormant monster, followed by an aggro_alarm_t unused
	GM_MSG_AGGRO_TEST,			//aggro test, only when the sender is in the aggro list, a new aggro will be triggered, followed by an aggro_info_t unused
	
//30
	GM_MSG_OBJ_SESSION_END,			//The object's session is completed
	GM_MSG_OBJ_SESSION_REPEAT,		//Indicates that the session should continue to execute 
	GM_MSG_OBJ_ZOMBIE_END,			//Indicates to end the zombie state
	GM_MSG_EXPERIENCE,			//get experience points	content is a msg_exp_t
	GM_MSG_GROUP_EXPERIENCE,		//Get team experience value conennt is the total damage caused by multiple msg_grp_exp_t, param
	
//35
	GM_MSG_TEAM_EXPERIENCE,			//Get the team experience value conennt is msg_exp_t, the experience value will be ignored if it exceeds the distance. param is the npcid killed. If scrape?, it is not killed by the team
	GM_MSG_QUERY_OBJ_INFO00,		//The info00 param of the obtained object is the sid of the sender, and the content is an int representing cs_index
	GM_MSG_HEARTBEAT,			//The parameter of the heartbeat message sent to oneself is the number of seconds of this Heartbeat
	GM_MSG_HATE_YOU,
	GM_MSG_TEAM_INVITE,			//Request someone to join the team param is teamseq, content is an int that represents pickup_flag

//40	
	GM_MSG_TEAM_AGREE_INVITE,		//The invitee agrees to join the team. content is an int (representing occupation) + team_mutable_prop
	GM_MSG_TEAM_REJECT_INVITE,		//decline invitation to join
	GM_MSG_JOIN_TEAM,			//The captain agrees that someone joins the team. The high part of param is the way of picking. The low part of param is the number of players, and the content is the expression of member_entry
	GM_MSG_JOIN_TEAM_FAILED,		//Subject cannot join the party and should be removed from the party
	GM_MSG_MEMBER_NOTIFY_DATA,		//Team members notify others of their basic information content is a team_mutable_prop

//45	
	GM_MSG_NEW_MEMBER,			//The leader notifies new members to join, content is a member_entry list param is the quantity
	GM_MSG_LEAVE_PARTY_REQUEST,
	GM_MSG_LEADER_CANCEL_PARTY,
	GM_MSG_MEMBER_NOT_IN_TEAM,
	GM_MSG_LEADER_KICK_MEMBER,

//50	
	GM_MSG_MEMBER_LEAVE,
	GM_MSG_LEADER_UPDATE_MEMBER,
	GM_MSG_GET_MEMBER_POS,			//Ask teammates to send the location param is the sid of the sender, content is an int representing cs_index
	GM_MSG_QUERY_PLAYER_EQUIPMENT,		//To obtain the data of a specific player, the plane distance is required to be within a certain range. param is the sid of the sender, and content is an int representing cs_index
	GM_MSG_TEAM_PICKUP,			//Teammates are assigned to items, param is type, content is count

//55	
	GM_MSG_TEAM_CHAT,			//team chat param is channel, content is the content
	GM_MSG_SERVICE_REQUEST,			//A message from a player requesting a service param is the service type content is specific data ?	
	GM_MSG_SERVICE_DATA,			//Service data arrives param is the service type  content yes precise data
	GM_MSG_SERVICE_HELLO,			//player Say hello to the service provider  param yes player's own faction
	GM_MSG_SERVICE_GREETING,		//The service provider calls back May need to return list of services inside$$$$(not done now)

//60	
	GM_MSG_SERVICE_QUIERY_CONTENT,		//Get service content 	 param is the service type, content can be regarded as a pair<cs_index,sid>
	GM_MSG_EXTERN_OBJECT_APPEAR,		//content is extern_object_manager::object_appear
	GM_MSG_EXTERN_OBJECT_DISAPPEAR,		//disappear or
	GM_MSG_EXTERN_OBJECT_REFRESH,		//Update the position and blood value, the blood value is saved in param 
	GM_MSG_USER_APPEAR_OUTSIDE,		//The user appears outside, and the necessary data must be sent to the player, content where is sid,param is linkd id

//65
	GM_MSG_FORWARD_BROADCAST_SPHERE,	//forwarded message broadcast message,content is another complete message
	GM_MSG_FORWARD_BROADCAST_CYLINDER,	//forwarded message broadcast message,content is another complete message
	GM_MSG_FORWARD_BROADCAST_TAPER,		//forwarded message broadcast message,content is another complete message
	GM_MSG_ENCHANT,				//use auxiliary magic
	GM_MSG_ENCHANT_ZOMBIE,			//Use auxiliary magic, specially for the dead

//70
	GM_MSG_OBJ_SESSION_REPEAT_FORCE,	//Indicates that the session should be repeated, even if there are tasks later, it must continue to execute
	GM_MSG_NPC_BE_KILLED,			//The message is sent to the player who killed the npc, param indicates the type of the npc to be killed, content is the level of the npc
	GM_MSG_NPC_CRY_FOR_HELP,		//npc for help operation
	GM_MSG_PLAYER_TASK_TRANSFER,		//The function of task transfer and communication between players
	GM_MSG_PLAYER_BECOME_INVADER,		//Become a fan name msg.param is added time

//75
	GM_MSG_PLAYER_BECOME_PARIAH,		//become famous 
	GM_MSG_FORWARD_CHAT_MSG,		//Forwarded user chat information, param is rlevel, source is XID(-channel, self_id)
	GM_MSG_QUERY_SELECT_TARGET,		//Get the object selected by the teammate
	GM_MSG_NOTIFY_SELECT_TARGET,		//Get the object selected by the teammate
	GM_MSG_SUBSCIBE_TARGET,			//request to subscribe to an object

//80
	GM_MSG_UNSUBSCIBE_TARGET,		//request to subscribe to an object
	GM_MSG_SUBSCIBE_CONFIRM,		//Check if the subscription exists
	GM_MSG_PRODUCE_MONEY,			//The notification system generates money. The sending source is the owner, param is the group id, and content is the amount of money
	GM_MSG_PRODUCE_MONSTER_DROP,		//The notification system generates items and money dropped by monsters, the sending source is the owner, param is money, content is struct { int team_id; int team_seq; int npc_id; int item_count; int item[];}
	GM_MSG_GATHER_REQUEST,			//Request to collect raw materials, param is the player's faction, content is the player's level, collection tool and task ID

//85
	GM_MSG_GATHER_REPLY,			//The notification can be collected. param is the time required for collection
	GM_MSG_GATHER_CANCEL,			//cancel collection
	GM_MSG_GATHER,				//To gather, to claim an item
	GM_MSG_GATHER_RESULT,			//The collection is completed, the param is the id of the collected item, the content is the quantity and the task ID that may be attached
	GM_MSG_HP_STEAL,				//received the result of blood sucking

//90
	GM_MSG_INSTANCE_SWITCH_GET,		//Obtain user data, server switching, obtain user data for switching between replicas param is key
	GM_MSG_INSTANCE_SWITCH_USER_DATA,	//User data, response of SWITCH_SWITCH_GET
	GM_MSG_EXT_AGGRO_FORWARD,		//Notify the native npc to forward hate. param is the size of rage, content is the id of hatred
	GM_MSG_TEAM_APPLY_PARTY,		//Apply for Entry Team Options
	GM_MSG_TEAM_APPLY_REPLY,		//The application is successfully replied, where param is seq

//95
	GM_MSG_QUERY_INFO_1,			//Query INFO1, which can be sent to players or NPCs, the content of param is cs_index, and content is sid
	GM_MSG_CON_EMOTE_REQUEST,		//The request param for collaborative action is action
	GM_MSG_CON_EMOTE_REPLY,			//The response param for coordinated actions is a combination of two bytes of action and agreement
	GM_MSG_TEAM_CHANGE_TO_LEADER,		//Inform others to become the leader
	GM_MSG_TEAM_LEADER_CHANGED,		//Notify teammates of captain changes

//100
	GM_MSG_OBJ_ZOMBIE_SESSION_END,		//Perform session operations after death, other definitions are the same as normal session operations
	GM_MSG_QUERY_PERSONAL_MARKET_NAME,	//Get the name of the stall, param is the sid of the sender, content is an int representing cs_index
	GM_MSG_HURT,				//The object generates damage content is msg_hurt_extra_info_t
	GM_MSG_DEATH,				//Force the object to die, param=0 non-task=1 task lossy=2 task lossless (this param is only valid for the player)
	GM_MSG_PLANE_SWITCH_REQUEST,		//Request to start transmission, content is the key, if transmission, return SWITCH_REPLAY

//105
	GM_MSG_PLANE_SWITCH_REPLY,		//The transfer request is confirmed, content is the key
	GM_MSG_SCROLL_RESURRECT,		//The scroll resurrection param indicates whether the resurrected person has turned on the pvp mode 1 means it is turned on
	GM_MSG_LEAVE_COSMETIC_MODE,		//out of cosmetic condition
	GM_MSG_DBSAVE_ERROR,			//database save error
	GM_MSG_SPAWN_DISAPPEAR,			//Notify NPC and items disappear param is condition

//110
	GM_MSG_PET_CTRL_CMD,			//Control messages from players will use this message to send pets
	GM_MSG_ENABLE_PVP_DURATION,		//Activate PVP status
	GM_MSG_PLAYER_KILLED_BY_NPC,		//NPC logic after player is killed by NPC
	GM_MSG_PLAYER_DUEL_REQUEST,             //The player issues a request for a duel
	GM_MSG_PLAYER_DUEL_REPLY,               //The player responds to duel's request, param is whether to agree to duel

//115
	GM_MSG_PLAYER_DUEL_PREPARE,      	//The duel is ready to start after a 3-second countdown
	GM_MSG_PLAYER_DUEL_START,               //duel begins 
	GM_MSG_PLAYER_DUEL_CANCEL,		//stop the duel
	GM_MSG_PLAYER_DUEL_STOP,		//duel over
	GM_MSG_DUEL_HURT,			//PVP object damage content is ignored

//120
	GM_MSG_PLAYER_BIND_REQUEST,		//ask to ride on someone else
	GM_MSG_PLAYER_BIND_INVITE,		//Invite others to ride on you
	GM_MSG_PLAYER_BIND_REQUEST_REPLY,	//response to request ride
	GM_MSG_PLAYER_BIND_INVITE_REPLY,	//response to invitation to ride
	GM_MSG_PLAYER_BIND_PREPARE,		//ready to connect

//125
	GM_MSG_PLAYER_BIND_LINK,		//connection started
	GM_MSG_PLAYER_BIND_STOP,		//stop connection
	GM_MSG_PLAYER_BIND_FOLLOW,		//ask the player to follow
	GM_MSG_QUERY_EQUIP_DETAIL,		//param is faction, content is cs_index and cs_sid
	GM_MSG_PLAYER_RECALL_PET,		//Let the player forcefully remove the summoning state

//130
	GM_MSG_CREATE_BATTLEGROUND,		//Request the battlefield server to create a battlefield message, mainly for testing
	GM_MSG_BECOME_TURRET_MASTER,		//Become the master of the siege engine, param is tid, content is faction
	GM_MSG_REMOVE_ITEM,			//The message of deleting an item is used to reduce the item after the control of the siege engine. The param is tid
	GM_MSG_NPC_TRANSFORM,			//NPC deformation effect, save the intermediate state in the content, the intermediate time, the intermediate mark and the final state
	GM_MSG_NPC_TRANSFORM2,			//NPC deformation effect 2, param is the target ID. If it is consistent with the target ID, then it will not be deformed.

//135
	GM_MSG_TURRET_NOTIFY_LEADER,		//The siege engine notifies the leader of its existence so that it cannot be summoned again
	GM_MSG_PET_RELOCATE_POS,		//Pet requests to relocate coordinates
	GM_MSG_PET_CHANGE_POS,			//The owner modifies the coordinates of the pet
	GM_MSG_PET_DISAPPEAR,			//The data is incorrect, or in other situations, the owner asks the pet to disappear
	GM_MSG_PET_NOTIFY_HP,			//The pet notifies the owner that its blood volume param is stamp, content is float hp ratio

//140
	GM_MSG_PET_NOTIFY_DEATH,		//Pet notifies owner of own death
	GM_MSG_PET_MASTER_INFO,			//The owner notifies the pet of its own data
	GM_MSG_PET_LEVEL_UP,			//The owner notifies that the pet has been upgraded ,content is level
	GM_MSG_PET_HONOR_MODIFY,		//The owner notifies the pet's loyalty of a change
	GM_MSG_MASTER_ASK_HELP,			//Owner asks pet for help

//145	
	GM_MSG_PET_SET_COOLDOWN,		//The pet notifies the owner to set the cooldown time msg.param is cooldown id, content is msec
	GM_MSG_MOB_BE_TRAINED,			//The monster is tamed, and the pet egg is sent to the caster
	GM_MSG_PET_AUTO_ATTACK,			//The owner notifies the pet to automatically attack msg.param is force attack, content is the target
	GM_MSG_PET_SKILL_LIST,			//The owner notifies the pet of the new skill list
	GM_MSG_SWITCH_FAILED,			//The copy notifies that the transfer failed

//150	
	GM_MSG_PET_ANTI_CHEAT,
	GM_MSG_QUERY_PROPERTY,			//Query the properties of other people, param is the index of the query props in the package column
	GM_MSG_QUERY_PROPERTY_REPLY,	//Query other people's attributes to return, param is the index of the query props in the package column, content is the attribute data
	GM_MSG_TRY_CLEAR_AGGRO,			//If your own invisibility level is greater than the npc anti-invisibility level, the npc's hatred towards you will be cleared, param is your own invisibility level
	GM_MSG_NOTIFY_INVISIBLE_DATA,	//Notify pets of their own stealth data, making pets invisible or visible

//155
	GM_MSG_NOTIFY_CLEAR_INVISIBLE,	//The pet unstealth informs the owner, so that the owner also unstealth
	GM_MSG_CONTRIBUTION_TO_KILL_NPC,//After the player kills the npc, the message sent by the npc to the player, param is the npc world_tag ??content is the structure msg_contribution_t
	GM_MSG_GROUP_CONTRIBUTION_TO_KILL_NPC,//After the team kills the npc, the message sent by the npc to the player, param is the npc world_tag ??content is the structure msg_group_contribution_t
	GM_MSG_REBUILD_TEAM_INSTANCE_KEY_REQ,	//The request sent by the team member to the captain to rebuild the team copy key, param is worldtag, and content is the team_instance_key before rebuilding
	GM_MSG_REBUILD_TEAM_INSTANCE_KEY,		//The rebuilding team dungeon key sent by the team leader, param is worldtag, content is the old and new team_instance_key

//160
	GM_MSG_TRANSFER_FILTER_DATA,		//Filter transfer, param is the number of filters, content is filter data
	GM_MSG_PLANT_PET_NOTIFY_DEATH,		//The plant pet notifies the owner of death, the parameter is pet_stamp
	GM_MSG_PLANT_PET_NOTIFY_HP,			//The plant pet notifies the owner information, the parameter is pet_stamp, and the data is msg_plant_pet_hp_notify
	GM_MSG_PLANT_PET_NOTIFY_DISAPPEAR,	//The plant pet notifies the owner of the disappearance, the parameter is pet_stamp
	GM_MSG_PLANT_PET_SUICIDE,			//The owner notifies the plant to explode, the parameter is pet_stamp

//165
	GM_MSG_MASTER_NOTIFY_LAYER,		//The owner notifies the pet's own layer, the parameter is petstamp data is char layer
	GM_MSG_INJECT_HP_MP,			//Add hp and mp to the target, data is msg_hp_mp_t
	GM_MSG_DRAIN_HP_MP,				//Make the target consume hp and mp, data is msg_hp_mp_t
	GM_MSG_CONGREGATE_REQUEST,		//Congregate request, param Congregate type data:msg_congregate_req_t
	GM_MSG_REJECT_CONGREGATE,		//Reject the assembly request, param assembly type

//170
	GM_MSG_NPC_BE_KILLED_BY_OWNER,	//NPC is killed by player, param is npc tid, content is msg_dps_dph_t
	GM_MSG_EXCHANGE_POS,			//Exchange positions between players, will be sent to two people at the same time
	GM_MSG_EXTERN_HEAL,				//The message of adding blood to so-and-so
	GM_MSG_QUERY_INVENTORY_DETAIL,	//Query player package details
	GM_MSG_TURRET_OUT_OF_CONTROL,	//Disarm the siege engine

//175
	GM_MSG_TRANSFER_FILTER_GET,		//filter transfer, param is filter_mask content is the number of transfers
	GM_MSG_PET_TEST_SANCTUARY,		//Notify that pets have entered the safe zone
	GM_MSG_PLAYER_KILLED_BY_PLAYER,	//The player is killed by the player, param is msg_player_killed_info_t
	GM_MSG_CREATE_COUNTRYBATTLE,	//A message requesting the National War battlefield server to create a battlefield, mainly for testing
	GM_MSG_COUNTRYBATTLE_HURT_RESULT,	//In the national war, the actual damage caused by the attacker is notified, param is the damage value, and the content is affected by the soul power of the attacker (player) or 0 (npc)
	
//180
	GM_MSG_LONGJUMP,				//Player teleports, param is worldtag, content is pos
	GM_MSG_TRICKBATTLE_PLAYER_KILLED,
	GM_MSG_COUNTRYBATTLE_PLAYER_KILLED,	//Player died in the national war
	GM_MSG_MAFIA_PVP_AWARD, //Gang pvp special event rewards
	GM_MSG_MAFIA_PVP_STATUS, //Gang pvp status notification
//185	
	GM_MSG_MAFIA_PVP_ELEMENT,// gang pvp loading request
	GM_MSG_PUNISH_ME,	// Request the other party's own enchant skills
	GM_MSG_REDUCE_CD,	// Lower the cd of the target
	GM_MSG_DELIVER_TASK, // Send tasks to targets
	GM_MSG_OBJ_ACTION_END,			//The action of the object is completed
//190	
	GM_MSG_OBJ_ACTION_REPEAT,		//Indicates that the action should continue 
	GM_MSG_SUBSCIBE_SUBTARGET,			//Requires subscription to a sub-goal
	GM_MSG_UNSUBSCIBE_SUBTARGET,		//Request to unsubscribe from a secondary goal
	GM_MSG_SUBSCIBE_SUBTARGET_CONFIRM, // Verify that the secondary target subscription exists
	GM_MSG_NOTIFY_SELECT_SUBTARGET,	   // Notify subscribers of subsubscriber changes
//195
	GM_MSG_ATTACK_CRIT_FEEDBACK,
	GM_MSG_DELIVER_STORAGE_TASK,       // Issue random library tasks
    GM_MSG_CHANGE_GENDER_LOGOUT,       // Offline after successful character change
	GM_MSG_CLEAR_TOWER_TASK,           // Clear single-player missions
	GM_MSG_CREATE_MNFACTION,	//A message requesting the cross-service gang battlefield server to create a battlefield, mainly for testing
//200
    GM_MSG_LOOKUP_ENEMY,
    GM_MSG_LOOKUP_ENEMY_REPLY,

//GM所采用的消息	
	GM_MSG_GM_GETPOS=600,			//Get the coordinates of the specified player, param is cs_index, content is sid
	GM_MSG_GM_MQUERY_MOVE_POS,		//The GM asks to query the coordinates for the next step to jump to the player
	GM_MSG_GM_MQUERY_MOVE_POS_REPLY,	//GM requests a response to query coordinates, used for GM's jump command content is the current instance key
	GM_MSG_GM_RECALL,			//GM asks for a jump
	GM_MSG_GM_CHANGE_EXP,			//GM increases exp and sp, param is exp, content is sp
	GM_MSG_GM_ENDUE_ITEM,			//The GM has given several items, param is the item id, content is the number
	GM_MSG_GM_ENDUE_SELL_ITEM,		//The GM gave the items sold in the store, the other same as above
	GM_MSG_GM_REMOVE_ITEM,			//GM requests to delete certain items, param is the item id, content is the number
	GM_MSG_GM_ENDUE_MONEY,			//GM increases or decreases money
	GM_MSG_GM_RESURRECT,			//GM calls for resurrection
	GM_MSG_GM_OFFLINE,			//GM asks to go offline 
	GM_MSG_GM_DEBUG_COMMAND,		//GM asks to go offline 
	GM_MSG_GM_RESET_PP,			//GM performs wash point operation
	GM_MSG_GM_QUERY_SPEC_ITEM,	//The GM queries whether the player has the specified item
	GM_MSG_GM_REMOVE_SPEC_ITEM,	//GM deletes player-specified items

	GM_MSG_PICKUP_MONEY2,			//The item notifies the user that the money has been picked up. Param is the amount of money. The content is who discarded it. 172
	GM_MSG_PICKUP_TEAM_MONEY2,		//The item notifies the captain to pick up the money param is the amount of money  who discarded the content 172
	GM_MSG_RECEIVE_MONEY2,			//Notify player to get money (possibly team up) 172

	GM_MSG_MAX,

};

struct msg_usermove_t	//Messages where users move and cross borders
{
	int cs_index;
	int cs_sid;
	int user_id;
	A3DVECTOR newpos;	//There is oldpos in the message
	size_t leave_data_size;	//The size of the message sent away (the message is appended)
	size_t enter_data_size;	//The size of the message sent away (the message is appended)
};

struct msg_aggro_info_t
{
	XID source;		//who generated this hate
	int aggro;		//size of hate
	int aggro_type;		//type of hate
	int faction;		//opponent's faction
	int level;		//opponent's level
};

struct msg_watching_t
{
	int level;		//source level
	int faction;		//source faction
	int invisible_degree;//Stealth level of the source
};

struct msg_aggro_list_t
{
	int count;
	struct 
	{
		XID id;
		int aggro;
	}list[1];
};

struct msg_cry_for_help_t
{
	XID attacker;
	int lamb_faction;
	int helper_faction;
};

struct msg_aggro_alarm_t
{
	XID attacker;	//attacker
	int rage;	
	int faction;	//sender's faction
	int target_faction;	//The type of distress accepted by the target
};

struct team_exp_entry
{
	int exp;
	int sp;
	XID who;
};

struct msg_exp_t
{
	int level;
	int exp;
	int sp;
};

struct msg_grp_exp_t
{
	int level;
	int exp;
	int sp;
	float rand;
};

struct msg_grpexp_t
{
	XID who;
	int damage;
	int reserve;
	/*
		The team has more data
		The experience of the team is composed of the damage of multiple people
		So comes the damage list for everyone,
		The first element of the list stores the experience value level/sp and team_seq of the team in who.type and who.id damage respectively
		Among them, the upper 16 bits of who.id are the level, and the lower 16 bits are the number of sp
		The object type of this structure inside npc.cpp is TempDmgNode (for reference)

		If it was the team that did the most damage, the second element of the list holds the name and level of the monster killed
		Among them, who.type saves the npc tid, and who.id saves a random number, so that everyone's task rewards are consistent

		The second element of damage always saves the monster's world tag, regardless of whether the team did the most damage
	*/
};

struct gather_reply
{
	int can_be_interrupted;
	int eliminate_tool;	//The ID of the consumer tool
	unsigned short gather_time_min;
	unsigned short gather_time_max;
};

struct gather_result
{
	int amount;
	int task_id;
	int eliminate_tool;		//Append this ID if item is deleted
	int mine_tid;
	int life;		//Lifespan of collected items
	char mine_type;	//type of mineral
};

struct msg_pickup_t
{
	XID who;
	int team_seq;
};

struct msg_gen_money
{
	int team_id;
	int team_seq;
};

struct msg_npc_transform
{
	int id_in_build;
	int time_use;
	int flag;
	int id_buildup;
	enum 
	{
		FLAG_DOUBLE_DMG_IN_BUILD = 1,
	};
};

struct msg_pet_pos_t
{
	A3DVECTOR pos;
	char inhabit_mode;
};

struct msg_pet_hp_notify
{
	float hp_ratio;
	int   cur_hp;
	char  aggro_state;		//Three hatred states 0 passive 1 active 2 dazed
	char  stay_mode;		//Two follow modes: 0 follow, 1 stay
	char  combat_state;		//are you fighting
	char  attack_monster;	//Whether to attack the monster
	float mp_ratio;
	int   cur_mp;
};

struct msg_invisible_data
{
	int invisible_degree;
	int anti_invisible_degree;
};

struct msg_contribution_t
{
	int npc_id;				//npc template id
	bool is_owner;			//Whether it belongs to the monster or not, the determination of belonging is the same as that of killing monsters in the task
	float team_contribution;//Team contribution, or individual contribution if the player is not in a team
	int team_member_count;	//The number of people in the team, or 1 if the player is not in the team
	float personal_contribution;	//personal contribution
};

struct msg_group_contribution_t
{
	int npc_id;				//npc template id
	bool is_owner;			//Whether it belongs to the monster or not, the determination of belonging is the same as that of killing monsters in the task
	int count;
	struct _list{
		XID xid;	
		float contribution;
	}list[];
};

struct msg_plant_pet_hp_notify
{
	float hp_ratio;
	int   cur_hp;
	float mp_ratio;
	int   cur_mp;
};

struct msg_hp_mp_t
{
	int hp;
	int mp;
};

struct msg_query_spec_item_t
{
	int type;
	int cs_index;
	int cs_sid;
};

struct msg_remove_spec_item_t
{
	int type;
	unsigned char where;
	unsigned char index;
	size_t count;
	int cs_index;
	int cs_sid;
};

struct msg_congregate_req_t
{
	int world_tag;
	int level_req;
	int sec_level_req;
    int reincarnation_times_req;
};

struct msg_dps_dph_t
{
	int level;
	int dps;
	int dph;
	bool update_rank;
};

struct msg_player_t
{
	int id;
	int cs_index;
	int cs_sid;
};

struct msg_player_killed_info_t
{
	int cls;
	bool gender;
	int level;
	int force_id;
};

struct msg_hurt_extra_info_t
{
	bool orange_name;
	char attacker_mode;
};

struct msg_mafia_pvp_award_t
{
	int mafia_id;
	int domain_id;
};

struct msg_punish_me_t
{
	int skill_id;
	int skill_lvl;
};

struct msg_reduce_cd_t
{
	int skill_id;
	int msec;
};

#endif

