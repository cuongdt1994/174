#ifndef __ONLINEGAME_GS_ATTACK_H__
#define __ONLINEGAME_GS_ATTACK_H__


#include "config.h"
#include <common/types.h>

//map revenue time level
enum
{
	PROFIT_LEVEL_NONE,	//There is no profit from fighting monsters and witches
	PROFIT_LEVEL_YELLOW,//Located on the yield map and the yield time is less than 1 hour
	PROFIT_LEVEL_NORMAL,//Players can get normal income
};

//Consider adding pet content here, so that pet attacks count as player attacks
struct attacker_info_t
{
	XID attacker;			//Attacker's ID For pets, this ID does not match the message ID
	short level;			//attacker level
	short eff_level;		//Effective level Prepared for team formation
	int team_id;			//attacker group id
	int team_seq;			//The sequence number of the attacker group
	int cs_index;			//If it is a player, it indicates the cs_index of the player
	int sid;			//If it is a player, it indicates the sid corresponding to the cs_index of the player
	int mafia_id;			//Group number, if it is not 0, it means a group attack, this attack will not affect the members of the group
	int wallow_level;		//obsession level npc
	int profit_level;		//income level
	int force_id;			//Attacker faction id, non-0 means not attacking players of the same faction
};

struct attack_msg
{
	attacker_info_t ainfo;		//attacker information

	float attack_range;		//The scope of the attack (the attack point is inside the message)
	float short_range;		//The close range of this attack, the damage will be adjusted when close
	float short_range_adjust_factor;	//Close range damage adjustment >1 amplifies >0&&<1 weakens 1 unchanged, only used in skill attacks
	int target_layer_adjust_mask;	//0x00-Instead of damage 0x01-Amplified damage of ground targets 0x02-Amplified damage of air targets 0x04-Amplified damage of water targets can be selected
	float target_layer_adjust_factor;	//Open ground target damage multiplier >1
	int physic_damage;		//Physical attack damage
	int attack_rate;		//physical attack hit
	int magic_damage[MAGIC_CLASS];	//magic damage
	int attacker_faction;		//attacker camp
	int target_faction;		//Attacker's enemy camp (your own camp can only be damaged by non-compulsory attacks if it matches this camp)
	int crit_rate;
	int crit_damage_bonus;	//Extra Crit Damage Bonus
	int attack_degree;		//attack level
	int penetration;
	char attack_attr;		//Attack attribute, the value is defined by enum ATTACK_ATTR
	char force_attack;		//Whether to force the attack
	char attacker_layer;		//Where is the attacker located 0 ground 1 sky 2 water
	int _attack_state;		//0x01 Bash 0x02 Attack Optimizer
	short speed;
	char attacker_mode;		//Attacker status (0x01 PK , 0x02 FREE, 0x04 has entered PK mode)
	char is_invader;		//Whether it is a despicable attack or not is now judged in advance
	int hp_steal_rate;		//Lifesteal percentage, only valid for physical attacks
	int skill_id;			//Whether it is a skill attack, and its corresponding skill number
	int skill_modify[5];
	struct
	{
		int skill;
		int level;
	} attached_skill;
	int skill_enhance;		//Enhance the damage of using skills, it will only take effect if the target is an npc
	int skill_enhance2;		//Enhances the damage of skills used, all targets
	int vigour;				//boldness
	unsigned char section;  //skill stage
	int weapon_class;
	int fake_weapon_class;
	int real_weapon_class;
	int feedback_filter;
	int anti_defense_degree; //Ignore physical defense level
	int anti_resistance_degree; // Ignores defense level
	int highest_level;
	int get_attack;
	int get_magic_attack;
	int get_equip_attack;
	int get_equip_magic_attack;
	int weapon_damage;
	int weapon_magic_damage;
	struct
	{
		int skill;
		int level;
	} infected_skill;
	enum 
	{
		PVP_ENABLE 	= 0x01,		//The PK switch is turned on
		PVP_FREE   	= 0x02,		//Free PK
		PVP_DURATION 	= 0x04,		//Already in PK with players
		PVP_DUEL	= 0x08,		//This time it's a duel attack
		PVP_FEEDBACK_KILL = 0x10,	//After killing the player, you need to send the target's information to yourself
	};

	enum ATTACK_ATTR
	{
		MAGIC_ATTACK = 0,		//magic attack
		PHYSIC_ATTACK = 1,		//physical attack
		MAGIC_ATTACK_GOLD = 2,		//magic attack gold
		MAGIC_ATTACK_WOOD = 3,		//spell attack wood
		MAGIC_ATTACK_WATER = 4,		//spell attack water
		MAGIC_ATTACK_FIRE = 5,		//magic attack fire
		MAGIC_ATTACK_EARTH = 6,		//magic attack earth
		PHYSIC_ATTACK_HIT_DEFINITE = 7,	//Physical attack must hit
	};
};

struct enchant_msg
{
	attacker_info_t ainfo;		//attacker information

	int attacker_faction;		//attacker camp
	int target_faction;		//Attacker's enemy camp (your own camp can only be damaged by non-compulsory attacks if it matches this camp)
	float attack_range;
	int skill;
	int skill_reserved1;		//internal use of skills
	int skill_modify[5];
	int attack_degree;		//attack level
	int penetration;
	char force_attack;		//Whether to force the attack
	char skill_level;
	char attacker_layer;
	char helpful;			//0: Curse 1: Blessing 2: Neutral blessing
	char attacker_mode;		//Attacker's state (0 not PK,1:PK)
	char is_invader;		//Whether it is a despicable attack or not is now judged in advance
	int _attack_state;		//0x01 Bash 0x02 Attack Optimizer 0x80 Immune to this cock
	int vigour;				//boldness
	unsigned char section;  //skill stage
	int weapon_class;
	int fake_weapon_class;
	int real_weapon_class;
	int get_attack;
	int get_magic_attack;
	int get_equip_attack;
	int get_equip_magic_attack;
	int weapon_damage;
	int weapon_magic_damage;
	struct
	{
		int skill;
		int level;
	} infected_skill;
};

struct damage_entry
{
	float physic_damage;
	float magic_damage[MAGIC_CLASS];
	float distance;
};

#endif

