#ifndef _ELEMENTDATAMAN_H_
#define _ELEMENTDATAMAN_H_

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <unordered_map>
#include <cstring>

#ifdef LINUX
#include "exptypes.h"
#else
#include "exptypes.h"

#endif	//LINUX

#include "../../common/mppc.h"

extern const char * DataTypeName[DT_MAX+1];

class elementdataman
{
public:
	elementdataman();
	~elementdataman();

	//////////////////////////////////////////////////////////////////////////
	
	void add_structure(unsigned int id, EQUIPMENT_ADDON & data);
	void add_structure(unsigned int id, WEAPON_MAJOR_TYPE & data);
	void add_structure(unsigned int id, WEAPON_SUB_TYPE & data);
	void add_structure(unsigned int id, WEAPON_ESSENCE & data);
	void add_structure(unsigned int id, ARMOR_MAJOR_TYPE & data);
	
	void add_structure(unsigned int id, ARMOR_SUB_TYPE & data);
	void add_structure(unsigned int id, ARMOR_ESSENCE & data);
	void add_structure(unsigned int id, DECORATION_MAJOR_TYPE & data);
	void add_structure(unsigned int id, DECORATION_SUB_TYPE & data);
	void add_structure(unsigned int id, DECORATION_ESSENCE & data);

	void add_structure(unsigned int id, MEDICINE_MAJOR_TYPE & data);
	void add_structure(unsigned int id, MEDICINE_SUB_TYPE & data);
	void add_structure(unsigned int id, MEDICINE_ESSENCE & data);
	void add_structure(unsigned int id, MATERIAL_MAJOR_TYPE & data);
	void add_structure(unsigned int id, MATERIAL_SUB_TYPE & data);

	void add_structure(unsigned int id, MATERIAL_ESSENCE & data);
	void add_structure(unsigned int id, DAMAGERUNE_SUB_TYPE & data);
	void add_structure(unsigned int id, DAMAGERUNE_ESSENCE & data);
	void add_structure(unsigned int id, ARMORRUNE_SUB_TYPE & data);	
	void add_structure(unsigned int id, ARMORRUNE_ESSENCE & data);

	void add_structure(unsigned int id, SKILLTOME_SUB_TYPE & data);
	void add_structure(unsigned int id, SKILLTOME_ESSENCE & data);	
	void add_structure(unsigned int id, FLYSWORD_ESSENCE & data);
	void add_structure(unsigned int id, WINGMANWING_ESSENCE & data);		
	void add_structure(unsigned int id, TOWNSCROLL_ESSENCE & data);

	void add_structure(unsigned int id, UNIONSCROLL_ESSENCE & data);
	void add_structure(unsigned int id, REVIVESCROLL_ESSENCE & data);
	void add_structure(unsigned int id, ELEMENT_ESSENCE & data);
	void add_structure(unsigned int id, TASKMATTER_ESSENCE & data);
	void add_structure(unsigned int id, TOSSMATTER_ESSENCE & data);

	void add_structure(unsigned int id, PROJECTILE_TYPE & data);
	void add_structure(unsigned int id, PROJECTILE_ESSENCE & data);
	void add_structure(unsigned int id, QUIVER_SUB_TYPE & data);
	void add_structure(unsigned int id, QUIVER_ESSENCE & data);
	void add_structure(unsigned int id, STONE_SUB_TYPE & data);

	void add_structure(unsigned int id, STONE_ESSENCE & data);	
	void add_structure(unsigned int id, MONSTER_ADDON & data);
	void add_structure(unsigned int id, MONSTER_TYPE & data);
	void add_structure(unsigned int id, MONSTER_ESSENCE & data);

	void add_structure(unsigned int id, NPC_TALK_SERVICE & data);
	void add_structure(unsigned int id, NPC_SELL_SERVICE & data);
	void add_structure(unsigned int id, NPC_BUY_SERVICE & data);
	void add_structure(unsigned int id, NPC_REPAIR_SERVICE & data);
	void add_structure(unsigned int id, NPC_INSTALL_SERVICE & data);

	void add_structure(unsigned int id, NPC_UNINSTALL_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_IN_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_OUT_SERVICE & data);
	void add_structure(unsigned int id, NPC_TASK_MATTER_SERVICE & data);
	void add_structure(unsigned int id, NPC_SKILL_SERVICE & data);
	void add_structure(unsigned int id, NPC_HEAL_SERVICE & data);
	void add_structure(unsigned int id, NPC_TRANSMIT_SERVICE & data);
	
	void add_structure(unsigned int id, NPC_TRANSPORT_SERVICE & data);
	void add_structure(unsigned int id, NPC_PROXY_SERVICE & data);
	void add_structure(unsigned int id, NPC_STORAGE_SERVICE & data);
	void add_structure(unsigned int id, NPC_MAKE_SERVICE & data);
	void add_structure(unsigned int id, NPC_DECOMPOSE_SERVICE & data);

	void add_structure(unsigned int id, NPC_TYPE & data);
	void add_structure(unsigned int id, NPC_ESSENCE & data);
	void add_structure(unsigned int id, talk_proc *  data);

	void add_structure(unsigned int id,  FACE_TEXTURE_ESSENCE & data);
	void add_structure(unsigned int id,  FACE_SHAPE_ESSENCE & data);
	void add_structure(unsigned int id,  FACE_EMOTION_TYPE & data);
	void add_structure(unsigned int id,  FACE_EXPRESSION_ESSENCE & data);
	void add_structure(unsigned int id,  FACE_HAIR_ESSENCE & data);
	void add_structure(unsigned int id,  FACE_MOUSTACHE_ESSENCE & data);
	void add_structure(unsigned int id,  COLORPICKER_ESSENCE & data);
	void add_structure(unsigned int id,  CUSTOMIZEDATA_ESSENCE & data);

	void add_structure(unsigned int id,  RECIPE_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  RECIPE_SUB_TYPE & data);
	void add_structure(unsigned int id,  RECIPE_ESSENCE & data);
	
	void add_structure(unsigned int id,  ENEMY_FACTION_CONFIG & data);
	void add_structure(unsigned int id,  CHARRACTER_CLASS_CONFIG & data);
	void add_structure(unsigned int id,  PARAM_ADJUST_CONFIG & data);
	void add_structure(unsigned int id,  PLAYER_ACTION_INFO_CONFIG & data);
	void add_structure(unsigned int id,  TASKDICE_ESSENCE & data);

	void add_structure(unsigned int id,	 TASKNORMALMATTER_ESSENCE & data);
	void add_structure(unsigned int id,	 FACE_FALING_ESSENCE & data);
	void add_structure(unsigned int id,	 PLAYER_LEVELEXP_CONFIG & data);
	void add_structure(unsigned int id,	 MINE_TYPE & data);
	void add_structure(unsigned int id,	 MINE_ESSENCE & data);

	void add_structure(unsigned int id,	 NPC_IDENTIFY_SERVICE & data);
	void add_structure(unsigned int id,  FASHION_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  FASHION_SUB_TYPE & data);
	void add_structure(unsigned int id,  FASHION_ESSENCE & data);

	void add_structure(unsigned int id,  FACETICKET_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  FACETICKET_SUB_TYPE & data);
	void add_structure(unsigned int id,  FACETICKET_ESSENCE & data);
	void add_structure(unsigned int id,  FACEPILL_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  FACEPILL_SUB_TYPE & data);
	
	void add_structure(unsigned int id,  FACEPILL_ESSENCE & data);
	void add_structure(unsigned int id,  SUITE_ESSENCE & data);
	void add_structure(unsigned int id,  GM_GENERATOR_TYPE & data);
	void add_structure(unsigned int id,  GM_GENERATOR_ESSENCE & data);
	void add_structure(unsigned int id,  PET_TYPE & data);

	void add_structure(unsigned int id,  PET_ESSENCE & data);
	void add_structure(unsigned int id,  PET_EGG_ESSENCE & data);
	void add_structure(unsigned int id,  PET_FOOD_ESSENCE & data);
	void add_structure(unsigned int id,  PET_FACETICKET_ESSENCE & data);
	void add_structure(unsigned int id,  FIREWORKS_ESSENCE & data);

	void add_structure(unsigned int id,  WAR_TANKCALLIN_ESSENCE & data);
	void add_structure(unsigned int id,  NPC_WAR_TOWERBUILD_SERVICE & data);
	void add_structure(unsigned int id,	 PLAYER_SECONDLEVEL_CONFIG & data);
	void add_structure(unsigned int id,  NPC_RESETPROP_SERVICE & data);
	void add_structure(unsigned int id,  NPC_PETNAME_SERVICE & data);

	void add_structure(unsigned int id,  NPC_PETLEARNSKILL_SERVICE & data);
	void add_structure(unsigned int id,  NPC_PETFORGETSKILL_SERVICE & data);
	void add_structure(unsigned int id,  SKILLMATTER_ESSENCE & data);	
	void add_structure(unsigned int id,  REFINE_TICKET_ESSENCE & data);	
	void add_structure(unsigned int id,  DESTROYING_ESSENCE & data);

	void add_structure(unsigned int id,  NPC_EQUIPBIND_SERVICE & data);
	void add_structure(unsigned int id,  NPC_EQUIPDESTROY_SERVICE & data);
	void add_structure(unsigned int id,  NPC_EQUIPUNDESTROY_SERVICE & data);
	void add_structure(unsigned int id,  BIBLE_ESSENCE & data);
	void add_structure(unsigned int id,  SPEAKER_ESSENCE & data);

	void add_structure(unsigned int id,  AUTOHP_ESSENCE & data);
	void add_structure(unsigned int id,  AUTOMP_ESSENCE & data);
	void add_structure(unsigned int id,	 DOUBLE_EXP_ESSENCE & data);
	void add_structure(unsigned int id,	 TRANSMITSCROLL_ESSENCE & data);
	void add_structure(unsigned int id,  DYE_TICKET_ESSENCE & data);

	void add_structure(unsigned int id,  GOBLIN_ESSENCE & data);
	void add_structure(unsigned int id,	 GOBLIN_EQUIP_TYPE & data);
	void add_structure(unsigned int id,	 GOBLIN_EQUIP_ESSENCE & data);
	void add_structure(unsigned int id,  GOBLIN_EXPPILL_ESSENCE & data);
	void add_structure(unsigned int id,  SELL_CERTIFICATE_ESSENCE & data);

	void add_structure(unsigned int id,  TARGET_ITEM_ESSENCE & data);
	void add_structure(unsigned int id,  LOOK_INFO_ESSENCE & data);
	void add_structure(unsigned int id,  UPGRADE_PRODUCTION_CONFIG & data);
	void add_structure(unsigned int id,  ACC_STORAGE_BLACKLIST_CONFIG & data);
	void add_structure(unsigned int id,  FACE_HAIR_TEXTURE_MAP & data);
	
	void add_structure(unsigned int id,  MULTI_EXP_CONFIG & data);
	void add_structure(unsigned int id,  INC_SKILL_ABILITY_ESSENCE & data);
	void add_structure(unsigned int id,  GOD_EVIL_CONVERT_CONFIG & data);
	void add_structure(unsigned int id,  WEDDING_CONFIG & data);
	void add_structure(unsigned int id,  WEDDING_BOOKCARD_ESSENCE & data);

	void add_structure(unsigned int id,  WEDDING_INVITECARD_ESSENCE & data);
	void add_structure(unsigned int id,  SHARPENER_ESSENCE & data);
	void add_structure(unsigned int id,  FACE_THIRDEYE_ESSENCE & data);
	void add_structure(unsigned int id,  FACTION_FORTRESS_CONFIG & data);
	void add_structure(unsigned int id,  FACTION_BUILDING_SUB_TYPE & data);

	void add_structure(unsigned int id,  FACTION_BUILDING_ESSENCE & data);
	void add_structure(unsigned int id,  FACTION_MATERIAL_ESSENCE & data);
	void add_structure(unsigned int id,  CONGREGATE_ESSENCE & data);
	
	void add_structure(unsigned int id,  ENGRAVE_MAJOR_TYPE & data);
	void add_structure(unsigned int id,  ENGRAVE_SUB_TYPE & data);
	void add_structure(unsigned int id,  ENGRAVE_ESSENCE & data);
	void add_structure(unsigned int id, NPC_ENGRAVE_SERVICE & data);
	void add_structure(unsigned int id, NPC_RANDPROP_SERVICE & data);
	
	void add_structure(unsigned int id,	RANDPROP_TYPE & data);
	void add_structure(unsigned int id, RANDPROP_ESSENCE & data);
	void add_structure(unsigned int id, WIKI_TABOO_CONFIG & data);
	void add_structure(unsigned int id, FORCE_CONFIG & data);
	void add_structure(unsigned int id, FORCE_TOKEN_ESSENCE & data);

	void add_structure(unsigned int id, NPC_FORCE_SERVICE & data);
	void add_structure(unsigned int id, PLAYER_DEATH_DROP_CONFIG & data);
	void add_structure(unsigned int id, DYNSKILLEQUIP_ESSENCE & data);
	void add_structure(unsigned int id, CONSUME_POINTS_CONFIG & data);
	void add_structure(unsigned int id, ONLINE_AWARDS_CONFIG & data);

	void add_structure(unsigned int id, COUNTRY_CONFIG & data);
	void add_structure(unsigned int id, GM_ACTIVITY_CONFIG & data);
	void add_structure(unsigned int id, FASHION_WEAPON_CONFIG & data);
	void add_structure(unsigned int id, PET_EVOLVE_CONFIG & data);
	void add_structure(unsigned int id, PET_EVOLVED_SKILL_CONFIG & data);

	void add_structure(unsigned int id, MONEY_CONVERTIBLE_ESSENCE & data);
	void add_structure(unsigned int id, STONE_CHANGE_RECIPE_TYPE & data);
	void add_structure(unsigned int id, STONE_CHANGE_RECIPE & data);
	void add_structure(unsigned int id, MERIDIAN_CONFIG & data);
	void add_structure(unsigned int id, PET_EVOLVED_SKILL_RAND_CONFIG & data);

	void add_structure(unsigned int id, AUTOTASK_DISPLAY_CONFIG & data);
	void add_structure(unsigned int id, TOUCH_SHOP_CONFIG & data);
	void add_structure(unsigned int id, TITLE_CONFIG & data);
	void add_structure(unsigned int id, COMPLEX_TITLE_CONFIG & data);
	void add_structure(unsigned int id, MONSTER_SPIRIT_ESSENCE & data);

	void add_structure(unsigned int id, PLAYER_SPIRIT_CONFIG & data);
	void add_structure(unsigned int id, PLAYER_REINCARNATION_CONFIG & data);
	void add_structure(unsigned int id, HISTORY_STAGE_CONFIG & data);
	void add_structure(unsigned int id, HISTORY_ADVANCE_CONFIG & data);
	void add_structure(unsigned int id, AUTOTEAM_CONFIG & data);

	void add_structure(unsigned int id, PLAYER_REALM_CONFIG & data);
	void add_structure(unsigned int id, CHARIOT_CONFIG & data);
	void add_structure(unsigned int id, CHARIOT_WAR_CONFIG & data);
	void add_structure(unsigned int id, POKER_LEVELEXP_CONFIG & data);
	void add_structure(unsigned int id, POKER_SUITE_ESSENCE & data);

	void add_structure(unsigned int id, POKER_DICE_ESSENCE & data);
	void add_structure(unsigned int id, POKER_SUB_TYPE & data);
	void add_structure(unsigned int id, POKER_ESSENCE & data);
	void add_structure(unsigned int id, TOKEN_SHOP_CONFIG & data);
	void add_structure(unsigned int id, SHOP_TOKEN_ESSENCE & data);

	void add_structure(unsigned int id, GT_CONFIG & data);
	void add_structure(unsigned int id, RAND_SHOP_CONFIG & data);
	void add_structure(unsigned int id, PROFIT_TIME_CONFIG & data);
	void add_structure(unsigned int id, FACTION_PVP_CONFIG & data);
	void add_structure(unsigned int id, UNIVERSAL_TOKEN_ESSENCE & data);

	void add_structure(unsigned int id, TASK_LIST_CONFIG & data);
	void add_structure(unsigned int id, TASK_DICE_BY_WEIGHT_CONFIG & data);
	void add_structure(unsigned int id, FASHION_SUITE_ESSENCE & data);
	void add_structure(unsigned int id, FASHION_BEST_COLOR_CONFIG & data);
	void add_structure(unsigned int id, SIGN_AWARD_CONFIG & data);

	void add_structure(unsigned int id, ASTROLABE_ESSENCE & data);
	void add_structure(unsigned int id, ASTROLABE_RANDOM_ADDON_ESSENCE & data);
	void add_structure(unsigned int id, ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE & data);
	void add_structure(unsigned int id, ASTROLABE_INC_EXP_ESSENCE & data);
	void add_structure(unsigned int id, ITEM_PACKAGE_BY_PROFESSION_ESSENCE & data);

	void add_structure(unsigned int id, ASTROLABE_LEVELEXP_CONFIG & data);
	void add_structure(unsigned int id, ASTROLABE_ADDON_RANDOM_CONFIG & data);
	void add_structure(unsigned int id, ASTROLABE_APPEARANCE_CONFIG & data);
	void add_structure(unsigned int id, EQUIP_MAKE_HOLE_CONFIG & data);
	void add_structure(unsigned int id, SOLO_TOWER_CHALLENGE_LEVEL_CONFIG & data);

	void add_structure(unsigned int id, SOLO_TOWER_CHALLENGE_AWARD_PAGE_CONFIG & data);
	void add_structure(unsigned int id, SOLO_TOWER_CHALLENGE_AWARD_LIST_CONFIG & data);
	void add_structure(unsigned int id, SOLO_TOWER_CHALLENGE_SCORE_COST_CONFIG & data);
	void add_structure(unsigned int id, MNFACTION_WAR_CONFIG & data);
	void add_structure(unsigned int id, NPC_CROSS_SERVER_SERVICE & data);

	void add_structure(unsigned int id, FIREWORKS2_ESSENCE & data);
	void add_structure(unsigned int id, FIX_POSITION_TRANSMIT_ESSENCE & data);

	// New

	void add_structure(unsigned int id, HOME_CONFIG & data); // 155
	void add_structure(unsigned int id, HOME_PRODUCTS_CONFIG & data);
	void add_structure(unsigned int id, HOME_RESOURCE_PRODUCE_CONFIG & data);
	void add_structure(unsigned int id, HOME_FORMULAS_PRODUCE_RECIPE & data);
	void add_structure(unsigned int id, HOME_FORMULAS_ITEM_ESSENCE & data);
	void add_structure(unsigned int id, HOME_PRODUCE_SERVICE_CONFIG & data);
	void add_structure(unsigned int id, HOME_FACTORY_CONFIG & data);
	void add_structure(unsigned int id, HOME_ITEM_MAJOR_TYPE & data);
	void add_structure(unsigned int id, HOME_ITEM_SUB_TYPE & data);
	void add_structure(unsigned int id, HOME_ITEM_ENTITY & data);
	void add_structure(unsigned int id, HOME_FORMULAS_PRODUCE_MAJOR_TYPE & data);
	void add_structure(unsigned int id, HOME_FORMULAS_PRODUCE_SUB_TYPE & data);
	void add_structure(unsigned int id, HOME_FORMULAS_ITEM_MAJOR_TYPE & data);
	void add_structure(unsigned int id, HOME_FORMULAS_ITEM_SUB_TYPE & data);
	void add_structure(unsigned int id, HOME_STORAGE_TASK_CONFIG & data);
		
	void add_structure(unsigned int id, WISH_TRIBUTE_ESSENCE & data); // 155
	void add_structure(unsigned int id, RED_PACKET_PAPER_ESSENCE & data);
	void add_structure(unsigned int id, LOTTORY_TICKET_STORAGE_CONFIG & data);
	void add_structure(unsigned int id, LOTTORY_TICKET_COST_CONFIG & data);
	void add_structure(unsigned int id, LOTTORY_TICKET_OTHER_AWARD_CONFIG & data);
	void add_structure(unsigned int id, HOME_TEXTURE_ENTITY & data);
	void add_structure(unsigned int id, HOME_GRASS_ENTITY & data);
	void add_structure(unsigned int id, HOME_UNLOCK_PAPER_ESSENCE & data);
	
	void add_structure(unsigned int id, GUARDIAN_BEAST_EGG_ESSENCE & data); // 155 v2
	void add_structure(unsigned int id, GUARDIAN_BEAST_ENTITY & data);
	void add_structure(unsigned int id, GUARDIAN_BEAST_RACE_CONFIG & data);
	void add_structure(unsigned int id, GUARDIAN_BEAST_UPGRADE_CONFIG & data);
	void add_structure(unsigned int id, GUARDIAN_BEAST_REWARD_CONFIG & data);

	void add_structure(unsigned int id, HOME_SKIN_ENTITY & data); // 156
	void add_structure(unsigned int id, WORLD_SPEAK_COST_CONFIG & data);
	void add_structure(unsigned int id, EASY_PRODUCE_ITEM_ESSENCE & data);
	
	void add_structure(unsigned int id, ARENA_CONFIG & data); // 156
	void add_structure(unsigned int id, ARENA_SCENE_CONFIG & data);
	void add_structure(unsigned int id, ARENA_POINT_CONFIG & data);
	void add_structure(unsigned int id, ARENA_ENTITY_CONFIG & data);
	void add_structure(unsigned int id, NPC_ARENA_SELL_SERVICE & data);

	void add_structure(unsigned int id, UNLOCK_RUNE_SLOT_ITEM_ESSENCE & data); // 156
	void add_structure(unsigned int id, RUNE_ITEM_ESSENCE & data);
	void add_structure(unsigned int id, RUNE_SKILL_CONFIG & data);
	void add_structure(unsigned int id, RUNE_UPGRADE_CONFIG & data);
	void add_structure(unsigned int id, ARENA_MAP_CONFIG & data);
	void add_structure(unsigned int id, NPC_STATUE_CREATE_SERVICE & data);
	void add_structure(unsigned int id, ARENA_FORBID_ITEM_CONFIG & data);
	void add_structure(unsigned int id, ARENA_FORBID_SKILL_CONFIG & data);
		
	void add_structure(unsigned int id, CARRIER_CONFIG & data); // 156
	void add_structure(unsigned int id, CHANGE_PROPERTY_CONFIG & data);
	void add_structure(unsigned int id, PROFESSION_PROPERTY_CONFIG & data);
	void add_structure(unsigned int id, FIX_MONSTER_ITEM_ESSENCE & data);
	void add_structure(unsigned int id, NPC_LIB_PRODUCE_SERVICE & data);
	void add_structure(unsigned int id, LIB_PRODUCE_RECIPE & data);
	void add_structure(unsigned int id, VOTE_CONFIG & data);

	void add_structure(unsigned int id, SIMULATOR_ITEM_LIST_CONFIG & data); // 156
	void add_structure(unsigned int id, EQUIPMENT_PRODUCE_METHOD_CONFIG & data);
	void add_structure(unsigned int id, PRODUCTION_LINE_CONFIG & data);
	void add_structure(unsigned int id, SOURCE_OF_MATERIAL_CONFIG & data);
	void add_structure(unsigned int id, FACTION_TALENT_SINGLE_CONFIG & data);
	void add_structure(unsigned int id, FACTION_TALENT_TREE_CONFIG & data);
	void add_structure(unsigned int id, FACTION_TARGET_CONFIG & data);

	void add_structure(unsigned int id, INSTANCE_STAGE_CONFIG & data); // 157
	void add_structure(unsigned int id, FACTION_PET_FEED_CONFIG & data);
	void add_structure(unsigned int id, FACTION_TILLAGE_CONFIG & data);
	void add_structure(unsigned int id, FACTION_PET_CONFIG & data);	
	void add_structure(unsigned int id, FACTION_PET_BLESS_CONFIG & data);
	void add_structure(unsigned int id, ITEM_USED_FOR_AREA_ESSENCE & data);
	void add_structure(unsigned int id, CAPTURE_ITEM_ESSENCE & data);

	void add_structure(unsigned int id, DRIFT_BOTTLE_DROP_CONFIG & data); // 157-158
	void add_structure(unsigned int id, NPC_FACTION_SELL_SERVICE & data);
	void add_structure(unsigned int id, FACTION_INSTANCE_DROP_CONFIG & data);
	void add_structure(unsigned int id, NPC_PARK_ENTER_SERVICE & data);
	void add_structure(unsigned int id, FACTION_STORAGE_WHITELIST_CONFIG & data);
	void add_structure(unsigned int id, NPC_BOSSRUSH_AWARD_SERVICE & data);
	void add_structure(unsigned int id, PROFESSION_STATE_CONFIG & data);
	void add_structure(unsigned int id, MENTAL_PARAM_ADJUST_CONFIG & data);

	void add_structure(unsigned int id, BIBLE_REFINE_CONFIG & data); // 159
	void add_structure(unsigned int id, BIBLE_REFINE_TICKET_ESSENCE & data); 
	void add_structure(unsigned int id, TITLE_PROGRESS_CONFIG & data);
	void add_structure(unsigned int id, NPC_MYSTERIOUS_SHOP_SERVICE & data);
	void add_structure(unsigned int id, OFFLINEBATTLE_CONFIG & data);
	void add_structure(unsigned int id, NPC_PRIDICTABLE_ENGRAVE_SERVICE & data);

	void add_structure(unsigned int id, NEW_ARMOR_MAJOR_TYPE & data); // 160
	void add_structure(unsigned int id, NEW_ARMOR_SUB_TYPE & data);
	void add_structure(unsigned int id, NEW_ARMOR_ESSENCE & data);
	
	void add_structure(unsigned int id, QIHUN_ESSENCE & data); // 160
	void add_structure(unsigned int id, QILING_ESSENCE & data);
	void add_structure(unsigned int id, STORY_BOOK_MAJOR_TYPE & data);
	void add_structure(unsigned int id, STORY_BOOK_CONFIG & data);

	void add_structure(unsigned int id, MENTOR_LEVEL_CONFIG & data); // 160
	void add_structure(unsigned int id, STUDENT_AWARD_CONFIG & data); // 160
	void add_structure(unsigned int id, QIHUN_COVER_ESSENCE & data);
	void add_structure(unsigned int id, GROWTH_TARGET_CONFIG & data);
	void add_structure(unsigned int id, QUESTION_TIP_CONFIG & data);
	void add_structure(unsigned int id, QUESTION_AWARD_CONFIG & data);

	void add_structure(unsigned int id, SLIDE_SKILL_ESSENCE & data); // 162
	void add_structure(unsigned int id, CONSUME_MONEY_CONFIG & data);
	void add_structure(unsigned int id, USE_FOR_SELF_ITEM_ESSENCE & data);

	// New

	void add_structure(unsigned int id, HIGH_POLY_FACE_TEXTURE_ESSENCE & data); // 162 > 171 start
	void add_structure(unsigned int id, HIGH_POLY_FACE_TEMPL_CONFIG & data);

	void add_structure(unsigned int id, NEW_LOTTERY_CONFIG & data);
	void add_structure(unsigned int id, CURRENCY_ITEM_WHITELIST_CONFIG & data);

	void add_structure(unsigned int id, HIGH_POLY_CUSTOMIZEDATA_ESSENCE & data);
	void add_structure(unsigned int id, HIGH_POLY_RACE_FEATURE & data);

	void add_structure(unsigned int id, BLACK_WHITE_LIST_MAJOR_TYPE & data);
	void add_structure(unsigned int id, BLACK_WHITE_LIST_CONFIG & data);

	void add_structure(unsigned int id, LOANS_PRODUCE_MAJOR_TYPE & data);
	void add_structure(unsigned int id, LOANS_PRODUCE_SUB_TYPE & data);
	void add_structure(unsigned int id, LOANS_PRODUCE_RECIPE & data);

	void add_structure(unsigned int id, RED_BOOK_CONFIG & data);
	void add_structure(unsigned int id, RED_BOOK_UPGRADE_ITEM & data);
	void add_structure(unsigned int id, RED_BOOK_LOTTERY_CONFIG & data);

	void add_structure(unsigned int id, ARENA_TICKET_CONFIG & data);
	void add_structure(unsigned int id, CAMP_LEVEL_CONFIG & data);
	void add_structure(unsigned int id, CAMP_TOKEN_CONFIG & data);
	void add_structure(unsigned int id, CAMP_TOKEN_TREE_CONFIG & data);
	void add_structure(unsigned int id, CAMP_TOKEN_ESSENCE & data);
	void add_structure(unsigned int id, CAMP_TOKEN_UPGRADE_ESSENCE & data);
	void add_structure(unsigned int id, CAMP_TOKEN_ROLLBACK_ESSENCE & data);
	void add_structure(unsigned int id, CAMP_TOKEN_PROB_ADJUST_ESSENCE & data);
	void add_structure(unsigned int id, CAMP_BATTLE_TECH_TREE_CONFIG & data);
	
	void add_structure(unsigned int id, SPECIAL_ACTIVITY_RESPAWN_CONFIG & data);
	void add_structure(unsigned int id, MAP_EVENT_CONFIG & data);
	void add_structure(unsigned int id, SPECIAL_CAMP_REGION_MAJOR_TYPE & data);

	void add_structure(unsigned int id, SPECIAL_CAMP_REGION_CONFIG & data);
	void add_structure(unsigned int id, PET_SKIN_CONFIG & data);
	void add_structure(unsigned int id, PET_MULTIRIDE_CONFIG & data);

	void add_structure(unsigned int id, FASHION_NEW_MAJOR_TYPE & data);
	void add_structure(unsigned int id, FASHION_NEW_SUB_TYPE & data);
	void add_structure(unsigned int id, FASHION_NEW_ESSENCE & data);

	void add_structure(unsigned int id, WAR_AREA_CONFIG & data);

	void add_structure(unsigned int id, ILLUSTRATED_FASHION_SERIES_CONFIG & data);
	void add_structure(unsigned int id, ILLUSTRATED_FASHION_ESSENCE & data);
	void add_structure(unsigned int id, ILLUSTRATED_WING_EGG_ESSENCE & data);
	void add_structure(unsigned int id, ILLUSTRATED_WING_SERIES_CONFIG & data);
	void add_structure(unsigned int id, ILLUSTRATED_PET_EGG_ESSENCE & data);
	void add_structure(unsigned int id, ILLUSTRATED_PET_SERIES_CONFIG & data);
	void add_structure(unsigned int id, ACROSS_SERVER_MATCH_CONFIG & data);
	void add_structure(unsigned int id, NPC_ASAL_CONFIG & data);
	void add_structure(unsigned int id, WILDERNESS_SURVIVAL_CONFIG & data);
	void add_structure(unsigned int id, ILLUSTRATED_REWARD_CONFIG & data);

	void add_structure(unsigned int id, FAST_PRODUCE_ITEM_TYPE & data);
	void add_structure(unsigned int id, FAST_PRODUCE_ITEM_ESSENCE & data);

	void add_structure(unsigned int id, KID_SYSTEM_CONFIG & data);
	void add_structure(unsigned int id, COURSE_ESSENCE & data);
	void add_structure(unsigned int id, COURSE_SUITE_ESSENCE & data);	
	void add_structure(unsigned int id, KID_PROPERTY_CONFIG & data);
	void add_structure(unsigned int id, KID_QUALITY_CONFIG & data);
	void add_structure(unsigned int id, KID_EXP_CONFIG & data);
	void add_structure(unsigned int id, KID_UPGRADE_STAR_CONFIG & data);
	void add_structure(unsigned int id, KID_LEVEL_REWARD_CONFIG & data);
	void add_structure(unsigned int id, KID_LEVEL_MAX_CONFIG & data);
	void add_structure(unsigned int id, KID_DEBRIS_ESSENCE & data);
	void add_structure(unsigned int id, KID_SKILL_CONFIG & data);
	void add_structure(unsigned int id, KID_DEBRIS_GENERATOR_ESSENCE & data);
	void add_structure(unsigned int id, KID_PROFESSION_CONFIG & data);
	void add_structure(unsigned int id, BIS_SOI_USE_LIMIT_CONFIG & data);

	void add_structure(unsigned int id, REWARD_INTERFACE_CONFIG & data);
	void add_structure(unsigned int id, HISTORY_VARIABLE_PROGRESS_CONFIG & data);
	void add_structure(unsigned int id, ANNIVERSARY_EVENT_INTERFACE_CONFIG & data);
	
	void add_structure(unsigned int id, ACTIVITY_EXPUP_CONFIG & data);
	void add_structure(unsigned int id, ACTIVITY_CHALLENGE_CONFIG & data);
	void add_structure(unsigned int id, ACTIVITY_CONSUMPTION_CONFIG & data);
	void add_structure(unsigned int id, ACTIVITY_SHOP_CONFIG & data);
	
	void add_structure(unsigned int id, REWARD_SHOW_CONFIG & data);
	void add_structure(unsigned int id, EXTR_PROP_SKILL_CONFIG & data);
	void add_structure(unsigned int id, MATERIAL_REFINE_ESSENCE & data);
	void add_structure(unsigned int id, MATERIAL_REFINE_TICKET_ESSENCE & data); // 162 > 171 end

	// 172 > 
	
	void add_structure(unsigned int id, REMOVE_TASK_RESULT_CONFIG & data);
	void add_structure(unsigned int id, LDBATTLE_TRANSFER_POINT_CONFIG & data);
	void add_structure(unsigned int id, HUAZHAO_ACTIVITY_CONFIG & data);
	void add_structure(unsigned int id, HERO_BOOK_CONFIG & data);
	void add_structure(unsigned int id, CROSS_SERVICE_ROOM_CONFIG & data);

	void add_structure(unsigned int id, FLYING_CHESS_CONFIG & data);
	void add_structure(unsigned int id, HIGHPOLY_MAINCUSTOMIZE_CONFIG & data);
	void add_structure(unsigned int id, HUARONG_ROAD_GAME_CONFIG & data);
	void add_structure(unsigned int id, TASK_BRANCH_RESULT_CONFIG & data);
	void add_structure(unsigned int id, TIME_LIMIT_ITEM_CONFIG & data);

	void add_structure(unsigned int id, CHRISTMAS_ACTIVITY_CONFIG & data);
	/*176
	void add_structure(unsigned int id, PORTRAIT_PICTURE_ESSENCE & data);
	void add_structure(unsigned int id, BLINDBOX_CONFIG & data);
	void add_structure(unsigned int id, ARENA_PEAK_SEASON_CONFIG & data);
	void add_structure(unsigned int id, ARENA_PEAK_BATTLE_CONFIG & data);

	void add_structure(unsigned int id, ARENA_PEAK_AWARD_CONFIG & data);
	void add_structure(unsigned int id, DEFAULT_HAIR_CONFIG & data);
	void add_structure(unsigned int id, EXPLORE_PROGRESS_DIALOG_CONFIG & data);
	void add_structure(unsigned int id, EXPLORE_OBJECT_CONFIG & data);
	*/
	

	//////////////////////////////////////////////////////////////////////////
public:
	unsigned int get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE & datatype);
	unsigned int get_first_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_cur_data_id(ID_SPACE idspace, DATA_TYPE & datatype);
	unsigned int get_next_data_id(ID_SPACE idspace, DATA_TYPE & datatype);

	unsigned int get_data_num(ID_SPACE idspace);

	DATA_TYPE get_data_type(unsigned int id, ID_SPACE idspace);
	const void * get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE & datatype);
	bool get_list_by_data_type( ID_SPACE idspace, DATA_TYPE type, std::vector<int> & list );

	int save_data(const char * pathname);
	int load_data(const char * pathname);
	
protected:

	template<class T>
	class array
	{
	public:
		inline size_t size() { return _v.size(); }
		inline void push_back(const T & x) { _v.push_back(x); }
		inline const T & operator [](size_t pos) const { return _v[pos]; }
		inline T & operator [](size_t pos) {return _v[pos];}
		int save(FILE * file)
		{
			size_t s = _v.size();
			fwrite(&s, sizeof(size_t), 1, file);
			if(s>0)		fwrite(&(_v[0]), sizeof(T), s, file);
			return 0;
		}
		int load(FILE * file)
		{
			int _count;
			fread(&_count, sizeof(int), 1, file);

			if (_count == 0)
			{
				return 0;
			}

			std::vector<unsigned int> _h_ids;
			std::vector<unsigned short> _h_compressed_sizes;

			for (int i = 0; i < _count; i++)
			{
				unsigned int _h_id;
				fread(&_h_id, sizeof(unsigned int), 1, file);
				_h_ids.push_back(_h_id);

				unsigned short _h_compressed_size;
				fread(&_h_compressed_size, sizeof(unsigned short), 1, file);
				_h_compressed_sizes.push_back(_h_compressed_size);
			}

			unsigned int compress_size;
			fread(&compress_size, sizeof(unsigned int), 1, file);

			void* compress_data = malloc(compress_size);
			fread(compress_data, compress_size, 1, file);

			int _dataSize = sizeof(T);
						
			printf("ELEMENTS_DATA::LOAD: _count = %d, _dataSize = %d\n", _count, _dataSize);
			
			void* os_dst = malloc(_dataSize);

			_v.reserve(_count);

			T data;

			int _pos = 0;
			for (int i = 0; i < _count; i++)
			{
				int item_id = _h_ids[i];
				unsigned short item_compressed_size = _h_compressed_sizes[i];

				//printf("ELEMENTS_DATA::LOAD::ITEM: id = %d, compressed_size = %d\n", item_id, item_compressed_size);

				int dst_len = _dataSize;
				int src_len = item_compressed_size;

				void* os_com = malloc(src_len);
				memcpy(os_com, (void*)((char*)compress_data + _pos), src_len);

				int res;

				if (dst_len > 8192)
				{
					res = GNET::mppc::uncompress2((unsigned char*)os_dst, &dst_len, (unsigned char*)os_com, src_len);
				}
				else
				{
					res = GNET::mppc::uncompress((unsigned char*)os_dst, &dst_len, (unsigned char*)os_com, src_len);
				}

				free(os_com);

				if (res == -1)
				{
					free(os_dst);
					return -1;
				}

				_pos = _pos + item_compressed_size;

				T* tmp = reinterpret_cast<T*>(static_cast<char*>(os_dst));

				_v.push_back(*tmp);
			}

			free(os_dst);

			return 0;		
		}	
	protected:
		std::vector<T> _v;
	};

	void setup_hash_map();

	void add_id_index(ID_SPACE idspace, unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_essence(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_addon(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_talk(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_face(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_recipe(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	void add_id_index_config(unsigned int id, DATA_TYPE type, unsigned int pos, void * start);
	
	static void NormalizeRandom(float* r, int n);

	array<EQUIPMENT_ADDON>			equipment_addon_array;
	array<WEAPON_MAJOR_TYPE>		weapon_major_type_array;
	array<WEAPON_SUB_TYPE>			weapon_sub_type_array;
	array<WEAPON_ESSENCE>			weapon_essence_array;
	array<ARMOR_MAJOR_TYPE>			armor_major_type_array;
	
	array<ARMOR_SUB_TYPE>			armor_sub_type_array;
	array<ARMOR_ESSENCE>			armor_essence_array;
	array<DECORATION_MAJOR_TYPE>	decoration_major_type_array;
	array<DECORATION_SUB_TYPE>		decoration_sub_type_array;
	array<DECORATION_ESSENCE>		decoration_essence_array;	// 10
	
	array<MEDICINE_MAJOR_TYPE>		medicine_major_type_array;
	array<MEDICINE_SUB_TYPE>		medicine_sub_type_array;
	array<MEDICINE_ESSENCE>			medicine_essence_array;
	array<MATERIAL_MAJOR_TYPE>		material_major_type_array;
	array<MATERIAL_SUB_TYPE>		material_sub_type_array;

	array<MATERIAL_ESSENCE>			material_essence_array;
	array<DAMAGERUNE_SUB_TYPE>		damagerune_sub_type_array;
	array<DAMAGERUNE_ESSENCE>		damagerune_essence_array;
	array<ARMORRUNE_SUB_TYPE>		armorrune_sub_type_array;
	array<ARMORRUNE_ESSENCE>		armorrune_essence_array;	// 20

	array<SKILLTOME_SUB_TYPE>		skilltome_sub_type_array;
	array<SKILLTOME_ESSENCE>		skilltome_essence_array;
	array<FLYSWORD_ESSENCE>			flysword_essence_array;
	array<WINGMANWING_ESSENCE>		wingmanwing_essence_array;
	array<TOWNSCROLL_ESSENCE>		townscroll_essence_array;

	array<UNIONSCROLL_ESSENCE>		unionscroll_essence_array;
	array<REVIVESCROLL_ESSENCE>		revivescroll_essence_array;
	array<ELEMENT_ESSENCE>			element_essence_array;	
	array<TASKMATTER_ESSENCE>		taskmatter_essence_array;
	array<TOSSMATTER_ESSENCE>		tossmatter_essence_array;	// 30

	array<PROJECTILE_TYPE>			projectile_type_array;
	array<PROJECTILE_ESSENCE>		projectile_essence_array;
	array<QUIVER_SUB_TYPE>			quiver_sub_type_array;
	array<QUIVER_ESSENCE>			quiver_essence_array;
	array<STONE_SUB_TYPE>			stone_sub_type_array;

	array<STONE_ESSENCE>			stone_essence_array;	
	array<MONSTER_ADDON>			monster_addon_array;	
	array<MONSTER_TYPE>				monster_type_array;
	array<MONSTER_ESSENCE>			monster_essence_array;		
	array<NPC_TALK_SERVICE>			npc_talk_service_array;		// 40

	array<NPC_SELL_SERVICE>			npc_sell_service_array;
	array<NPC_BUY_SERVICE>			npc_buy_service_array;
	array<NPC_REPAIR_SERVICE>		npc_repair_service_array;
	array<NPC_INSTALL_SERVICE>		npc_install_service_array;
	array<NPC_UNINSTALL_SERVICE>	npc_uninstall_service_array;

	array<NPC_TASK_IN_SERVICE>		npc_task_in_service_array;
	array<NPC_TASK_OUT_SERVICE>		npc_task_out_service_array;
	array<NPC_TASK_MATTER_SERVICE>	npc_task_matter_service_array;
	array<NPC_SKILL_SERVICE>		npc_skill_service_array;
	array<NPC_HEAL_SERVICE>			npc_heal_service_array;		// 50

	array<NPC_TRANSMIT_SERVICE>		npc_transmit_service_array;
	array<NPC_TRANSPORT_SERVICE>	npc_transport_service_array;
	array<NPC_PROXY_SERVICE>		npc_proxy_service_array;
	array<NPC_STORAGE_SERVICE>		npc_storage_service_array;
	array<NPC_MAKE_SERVICE>			npc_make_service_array;

	array<NPC_DECOMPOSE_SERVICE>	npc_decompose_service_array;	
	array<NPC_TYPE>					npc_type_array;
	array<NPC_ESSENCE>				npc_essence_array;
	array<talk_proc*>				talk_proc_array;
	array<FACE_TEXTURE_ESSENCE>		face_texture_essence_array;	// 60

	array<FACE_SHAPE_ESSENCE>		face_shape_essence_array;	
	array<FACE_EMOTION_TYPE>		face_emotion_type_array;
	array<FACE_EXPRESSION_ESSENCE>	face_expression_essence_array;
	array<FACE_HAIR_ESSENCE>		face_hair_essence_array;
	array<FACE_MOUSTACHE_ESSENCE>	face_moustache_essence_array;
	array<COLORPICKER_ESSENCE>		colorpicker_essence_array;
	array<CUSTOMIZEDATA_ESSENCE>	customizedata_essence_array;
	
	array<RECIPE_MAJOR_TYPE>		recipe_major_type_array;
	array<RECIPE_SUB_TYPE>			recipe_sub_type_array;
	array<RECIPE_ESSENCE>			recipe_essence_array;

	array<ENEMY_FACTION_CONFIG>		enemy_faction_config_array;
	array<CHARRACTER_CLASS_CONFIG>	character_class_config_array;
	array<PARAM_ADJUST_CONFIG>		param_adjust_config_array;
	array<PLAYER_ACTION_INFO_CONFIG> player_action_info_config_array;
	array<TASKDICE_ESSENCE>			taskdice_essence_array;

	array<TASKNORMALMATTER_ESSENCE>	tasknormalmatter_essence_array;
	array<FACE_FALING_ESSENCE>	face_faling_essence_array;
	array<PLAYER_LEVELEXP_CONFIG>	player_levelexp_config_array;
	array<MINE_TYPE>	mine_type_array;
	array<MINE_ESSENCE>	mine_essence_array;

	array<NPC_IDENTIFY_SERVICE>		npc_identify_service_array;	

	array<FASHION_MAJOR_TYPE>		fashion_major_type_array;
	
	array<FASHION_SUB_TYPE>			fashion_sub_type_array;
	array<FASHION_ESSENCE>			fashion_essence_array;

	array<FACETICKET_MAJOR_TYPE>	faceticket_major_type_array;
	array<FACETICKET_SUB_TYPE>		faceticket_sub_type_array;
	array<FACETICKET_ESSENCE>		faceticket_essence_array;
	array<FACEPILL_MAJOR_TYPE>		facepill_major_type_array;
	array<FACEPILL_SUB_TYPE>		facepill_sub_type_array;
	array<FACEPILL_ESSENCE>			facepill_essence_array;

	array<SUITE_ESSENCE>			suite_essence_array;
	array<GM_GENERATOR_TYPE>		gm_generator_type_array;
	array<GM_GENERATOR_ESSENCE>		gm_generator_essence_array;
	array<PET_TYPE>					pet_type_array;
	array<PET_ESSENCE>				pet_essence_array;
	array<PET_EGG_ESSENCE>			pet_egg_essence_array;
	array<PET_FOOD_ESSENCE>			pet_food_essence_array;
	array<PET_FACETICKET_ESSENCE>	pet_faceticket_essence_array;
	array<FIREWORKS_ESSENCE>		fireworks_essence_array;
	array<FIREWORKS2_ESSENCE>		fireworks2_essence_array;
	array<FIX_POSITION_TRANSMIT_ESSENCE> fix_position_transmit_essence_array;

	array<WAR_TANKCALLIN_ESSENCE>	war_tankcallin_essence_array;
	array<NPC_WAR_TOWERBUILD_SERVICE> npc_war_towerbuild_service_array;
	array<PLAYER_SECONDLEVEL_CONFIG>	player_secondlevel_config_array;
	array<NPC_RESETPROP_SERVICE>	npc_resetprop_service_array;
	array<NPC_PETNAME_SERVICE>	npc_petname_service_array;

	array<NPC_PETLEARNSKILL_SERVICE>	npc_petlearnskill_service_array;
	array<NPC_PETFORGETSKILL_SERVICE>	npc_petforgetskill_service_array;
	array<SKILLMATTER_ESSENCE>		skillmatter_essence_array;
	array<REFINE_TICKET_ESSENCE>	refine_ticket_essence_array;
	array<DESTROYING_ESSENCE>		destroying_essence_array;

	array<NPC_EQUIPBIND_SERVICE>	npc_equipbind_service_array;
	array<NPC_EQUIPDESTROY_SERVICE>	npc_equipdestroy_service_array;
	array<NPC_EQUIPUNDESTROY_SERVICE> npc_equipundestroy_service_array;
	array<BIBLE_ESSENCE>			bible_essence_array;
	array<SPEAKER_ESSENCE>			speaker_essence_array;

	array<AUTOMP_ESSENCE>			automp_essence_array;
	array<AUTOHP_ESSENCE>			autohp_essence_array;
	array<DOUBLE_EXP_ESSENCE>		double_exp_essence_array;
	array<TRANSMITSCROLL_ESSENCE>	transmitscroll_essence_array;
	array<DYE_TICKET_ESSENCE>		dye_ticket_essence_array;

	array<GOBLIN_ESSENCE>			goblin_essence_array;
	array<GOBLIN_EQUIP_TYPE>		goblin_equip_type_array;
	array<GOBLIN_EQUIP_ESSENCE>		goblin_equip_essence_array;
	array<GOBLIN_EXPPILL_ESSENCE>	goblin_exppill_essence_array;
	array<SELL_CERTIFICATE_ESSENCE>	sell_certificate_essence_array;

	array<TARGET_ITEM_ESSENCE>		target_item_essence_array;
	array<LOOK_INFO_ESSENCE>		look_info_essence_array;
	array<UPGRADE_PRODUCTION_CONFIG>	upgrade_production_config_array;
	array<ACC_STORAGE_BLACKLIST_CONFIG>	acc_storage_blacklist_config_array;
	array<FACE_HAIR_TEXTURE_MAP>    face_hair_texture_map_array;

	array<MULTI_EXP_CONFIG>			multi_exp_config_array;	
	array<INC_SKILL_ABILITY_ESSENCE> inc_skill_ability_essence_array;
	array<GOD_EVIL_CONVERT_CONFIG>	god_evil_convert_config_array;	
	array<WEDDING_CONFIG>			wedding_config_array;
	array<WEDDING_BOOKCARD_ESSENCE> wedding_bookcard_essence_array;

	array<WEDDING_INVITECARD_ESSENCE> wedding_invitecard_essence_array;
	array<SHARPENER_ESSENCE> sharpener_essence_array;
	array<FACE_THIRDEYE_ESSENCE> face_thirdeye_essence_array;
	array<FACTION_FORTRESS_CONFIG> faction_fortress_config_array;
	array<FACTION_BUILDING_SUB_TYPE> faction_building_sub_type_array;

	array<FACTION_BUILDING_ESSENCE> faction_building_essence_array;
	array<FACTION_MATERIAL_ESSENCE> faction_material_essence_array;
	array<CONGREGATE_ESSENCE> congregate_essence_array;
	
	array<ENGRAVE_MAJOR_TYPE>		engrave_major_type_array;
	array<ENGRAVE_SUB_TYPE>			engrave_sub_type_array;
	array<ENGRAVE_ESSENCE>			engrave_essence_array;
	array<NPC_ENGRAVE_SERVICE>			npc_engrave_service_array;
	array<NPC_RANDPROP_SERVICE>			npc_randprop_service_array;
	
	array<RANDPROP_TYPE>				randprop_type_array;
	array<RANDPROP_ESSENCE>			randprop_essence_array;
	array<WIKI_TABOO_CONFIG>			wiki_taboo_config_array;
	array<FORCE_CONFIG>						force_config_array;
	array<FORCE_TOKEN_ESSENCE>	force_token_essence_array;

	array<NPC_FORCE_SERVICE>	npc_force_service_array;
	array<PLAYER_DEATH_DROP_CONFIG>	player_death_drop_config_array;
	array<DYNSKILLEQUIP_ESSENCE>	dynskillequip_essence_array;
	array<CONSUME_POINTS_CONFIG>	consume_points_config_array;
	array<ONLINE_AWARDS_CONFIG>	online_awards_config_array;

	array<COUNTRY_CONFIG>	country_config_array;
	array<GM_ACTIVITY_CONFIG>	gm_activity_config_array;
	array<FASHION_WEAPON_CONFIG>	fashion_weapon_config_array;
	array<PET_EVOLVE_CONFIG>	pet_evolve_config_array;
	array<PET_EVOLVED_SKILL_CONFIG>	pet_evolved_skill_config_array;

	array<MONEY_CONVERTIBLE_ESSENCE>	money_convertible_essence_array;
	array<STONE_CHANGE_RECIPE_TYPE>	stone_change_recipe_type_array;
	array<STONE_CHANGE_RECIPE>	stone_change_recipe_array;
	array<MERIDIAN_CONFIG>	meridian_config_array;
	array<PET_EVOLVED_SKILL_RAND_CONFIG>	pet_evolved_skill_rand_config_array;

	array<AUTOTASK_DISPLAY_CONFIG>	autotask_display_config_array;
	array<TOUCH_SHOP_CONFIG>	touch_shop_config_array;
	array<TITLE_CONFIG>			title_config_array;
	array<COMPLEX_TITLE_CONFIG>	complex_title_config_array;
	array<MONSTER_SPIRIT_ESSENCE>	monster_spirit_essence_array;
	
	array<PLAYER_SPIRIT_CONFIG>	player_spirit_config_array;
	array<PLAYER_REINCARNATION_CONFIG>	player_reincarnation_config_array;
	array<HISTORY_STAGE_CONFIG>	history_stage_config_array;
	array<HISTORY_ADVANCE_CONFIG>	history_advance_config_array;
	array<AUTOTEAM_CONFIG>	autoteam_config_array;

	array<PLAYER_REALM_CONFIG>	player_realm_config_array;
	array<CHARIOT_CONFIG>	chariot_config_array;
	array<CHARIOT_WAR_CONFIG>	chariot_war_config_array;
	array<POKER_LEVELEXP_CONFIG>	poker_levelexp_config_array;
	array<POKER_SUITE_ESSENCE>	poker_suite_essence_array;

	array<POKER_DICE_ESSENCE>	poker_dice_essence_array;
	array<POKER_SUB_TYPE>		poker_sub_type_array;
	array<POKER_ESSENCE>		poker_essence_array;
	array<TOKEN_SHOP_CONFIG>		token_shop_config_array;
	array<SHOP_TOKEN_ESSENCE>		shop_token_essence_array;

	array<GT_CONFIG>		gt_config_array;
	array<RAND_SHOP_CONFIG>		rand_shop_config_array;
	array<PROFIT_TIME_CONFIG>		profit_time_config_array;
	array<FACTION_PVP_CONFIG>		faction_pvp_config_array;
	array<UNIVERSAL_TOKEN_ESSENCE>		universal_token_essence_array;

	array<TASK_LIST_CONFIG>				task_list_config_array;
	array<TASK_DICE_BY_WEIGHT_CONFIG>	task_dice_by_weight_config_array;
	array<FASHION_SUITE_ESSENCE>		fashion_suite_essence_array;
	array<FASHION_BEST_COLOR_CONFIG>	fashion_best_color_config_array;
	array<SIGN_AWARD_CONFIG>			sign_award_config_array;
	
	array<ASTROLABE_ESSENCE>			astrolabe_essence_array;
	array<ASTROLABE_RANDOM_ADDON_ESSENCE>	astrolabe_random_addon_essence_array;
	array<ASTROLABE_INC_INNER_POINT_VALUE_ESSENCE>	astrolabe_inc_inner_point_value_essence_array;
	array<ASTROLABE_INC_EXP_ESSENCE>	astrolabe_inc_exp_essence_array;
	array<ITEM_PACKAGE_BY_PROFESSION_ESSENCE>	item_package_by_profession_essence_array;

	array<ASTROLABE_LEVELEXP_CONFIG>	astrolabe_levelexp_config_array;
	array<ASTROLABE_ADDON_RANDOM_CONFIG>	astrolabe_addon_random_config_array;
	array<ASTROLABE_APPEARANCE_CONFIG>		astrolabe_appearance_config_array;
	array<EQUIP_MAKE_HOLE_CONFIG>		equip_make_hole_config_array;
	array<SOLO_TOWER_CHALLENGE_LEVEL_CONFIG>		solo_tower_challenge_level_config_array;

	array<SOLO_TOWER_CHALLENGE_AWARD_PAGE_CONFIG>	solo_tower_challenge_award_page_config_array;
	array<SOLO_TOWER_CHALLENGE_AWARD_LIST_CONFIG>	solo_tower_challenge_award_list_config_array;
	array<SOLO_TOWER_CHALLENGE_SCORE_COST_CONFIG>	solo_tower_challenge_score_cost_config_array;
	array<MNFACTION_WAR_CONFIG> mnfaction_war_config_array;
	array<NPC_CROSS_SERVER_SERVICE> npc_cross_server_service_array;


	// New 

	array<HOME_CONFIG>	home_config_array;
	array<HOME_PRODUCTS_CONFIG>	home_products_config_array;
	array<HOME_RESOURCE_PRODUCE_CONFIG>	home_resource_produce_config_array;
	array<HOME_FORMULAS_PRODUCE_RECIPE>	home_formulas_produce_recipe_array;
	array<HOME_FORMULAS_ITEM_ESSENCE>	home_formulas_item_essence_array;
	array<HOME_PRODUCE_SERVICE_CONFIG>	home_produce_service_config_array;
	array<HOME_FACTORY_CONFIG>	home_factory_config_array;
	array<HOME_ITEM_MAJOR_TYPE>	home_item_major_type_array;
	array<HOME_ITEM_SUB_TYPE>	home_item_sub_type_array;
	array<HOME_ITEM_ENTITY>	home_item_entity_array;
	array<HOME_FORMULAS_PRODUCE_MAJOR_TYPE>	home_formulas_produce_major_type_array;
	array<HOME_FORMULAS_PRODUCE_SUB_TYPE>	home_formulas_produce_sub_type_array;
	array<HOME_FORMULAS_ITEM_MAJOR_TYPE>	home_formulas_item_major_type_array;
	array<HOME_FORMULAS_ITEM_SUB_TYPE>	home_formulas_item_sub_type_array;
	array<HOME_STORAGE_TASK_CONFIG>	home_storage_task_config_array;

	array<WISH_TRIBUTE_ESSENCE>	wish_tribute_essence_array;
	array<RED_PACKET_PAPER_ESSENCE>	red_packet_paper_essence_array;
	array<LOTTORY_TICKET_STORAGE_CONFIG>	lottory_ticket_storage_config_array;
	array<LOTTORY_TICKET_COST_CONFIG>	lottory_ticket_cost_config_array;
	array<LOTTORY_TICKET_OTHER_AWARD_CONFIG>	lottory_ticket_other_award_config_array;
	array<HOME_TEXTURE_ENTITY>	home_texture_entity_array;
	array<HOME_GRASS_ENTITY>	home_grass_entity_array;
	array<HOME_UNLOCK_PAPER_ESSENCE>	home_unlock_paper_essence_array;

	array<GUARDIAN_BEAST_EGG_ESSENCE>	guardian_beast_egg_essence_array;
	array<GUARDIAN_BEAST_ENTITY>	guardian_beast_entity_array;
	array<GUARDIAN_BEAST_RACE_CONFIG>	guardian_beast_race_config_array;
	array<GUARDIAN_BEAST_UPGRADE_CONFIG>	guardian_beast_upgrade_config_array;
	array<GUARDIAN_BEAST_REWARD_CONFIG>	guardian_beast_reward_config_array;

	array<HOME_SKIN_ENTITY>	home_skin_entity_array;
	array<WORLD_SPEAK_COST_CONFIG>	world_speak_cost_config_array;
	array<EASY_PRODUCE_ITEM_ESSENCE>	easy_produce_item_essence_array;

	array<ARENA_CONFIG>	arena_config_array;
	array<ARENA_SCENE_CONFIG>	arena_scene_config_array;
	array<ARENA_POINT_CONFIG>	arena_point_config_array;
	array<ARENA_ENTITY_CONFIG>	arena_entity_config_array;
	array<NPC_ARENA_SELL_SERVICE>	npc_arena_sell_service_array;

	array<UNLOCK_RUNE_SLOT_ITEM_ESSENCE>	unlock_rune_slot_item_essence_array;
	array<RUNE_ITEM_ESSENCE>	rune_item_essence_array;
	array<RUNE_SKILL_CONFIG>	rune_skill_config_array;
	array<RUNE_UPGRADE_CONFIG>	rune_upgrade_config_array;
	array<ARENA_MAP_CONFIG>	arena_map_config_array;
	array<NPC_STATUE_CREATE_SERVICE>	npc_statue_create_service_array;
	array<ARENA_FORBID_ITEM_CONFIG>	arena_forbid_item_config_array;
	array<ARENA_FORBID_SKILL_CONFIG>	arena_forbid_skill_config_array;

	array<CARRIER_CONFIG>	carrier_config_array;
	array<CHANGE_PROPERTY_CONFIG>	change_property_config_array;
	array<PROFESSION_PROPERTY_CONFIG>	profession_property_config_array;
	array<FIX_MONSTER_ITEM_ESSENCE>	fix_monster_item_essence_array;
	array<NPC_LIB_PRODUCE_SERVICE>	npc_lib_produce_service_array;
	array<LIB_PRODUCE_RECIPE>	lib_produce_recipe_array;
	array<VOTE_CONFIG>	vote_config_array;

	array<SIMULATOR_ITEM_LIST_CONFIG>	simulator_item_list_config_array;
	array<EQUIPMENT_PRODUCE_METHOD_CONFIG>	equipment_produce_method_config_array;
	array<PRODUCTION_LINE_CONFIG>	production_line_config_array;
	array<SOURCE_OF_MATERIAL_CONFIG>	source_of_material_config_array;
	array<FACTION_TALENT_SINGLE_CONFIG>	faction_talent_single_config_array;
	array<FACTION_TALENT_TREE_CONFIG>	faction_talent_tree_config_array;
	array<FACTION_TARGET_CONFIG>	faction_target_config_array;

	array<INSTANCE_STAGE_CONFIG>	instance_stage_config_array;
	array<FACTION_PET_FEED_CONFIG>	faction_pet_feed_config_array;
	array<FACTION_TILLAGE_CONFIG>	faction_tillage_config_array;
	array<FACTION_PET_CONFIG>	faction_pet_config_array;	
	array<FACTION_PET_BLESS_CONFIG>	faction_pet_bless_config_array;
	array<ITEM_USED_FOR_AREA_ESSENCE>	item_used_for_area_essence_array;
	array<CAPTURE_ITEM_ESSENCE>	capture_item_essence_array;


	array<DRIFT_BOTTLE_DROP_CONFIG>	drift_bottle_drop_config_array;
	array<NPC_FACTION_SELL_SERVICE>	npc_faction_sell_service_array;
	array<FACTION_INSTANCE_DROP_CONFIG>	faction_instance_drop_config_array;
	array<NPC_PARK_ENTER_SERVICE>	npc_park_enter_service_array;
	array<FACTION_STORAGE_WHITELIST_CONFIG>	faction_storage_whitelist_config_array;
	array<NPC_BOSSRUSH_AWARD_SERVICE>	npc_bossrush_award_service_array;
	array<PROFESSION_STATE_CONFIG>	profession_state_config_array;
	array<MENTAL_PARAM_ADJUST_CONFIG>	mental_param_adjust_config_array;

	array<BIBLE_REFINE_CONFIG>	bible_refine_config_array;
	array<BIBLE_REFINE_TICKET_ESSENCE>	bible_refine_ticket_essence_array;
	array<TITLE_PROGRESS_CONFIG>	title_progress_config_array;
	array<NPC_MYSTERIOUS_SHOP_SERVICE>	npc_mysterious_shop_service_array;
	array<OFFLINEBATTLE_CONFIG>	offlinebattle_config_array;
	array<NPC_PRIDICTABLE_ENGRAVE_SERVICE>	npc_pridictable_engrave_service_array;
	
	array<NEW_ARMOR_MAJOR_TYPE>	new_armor_major_type_array;
	array<NEW_ARMOR_SUB_TYPE>	new_armor_sub_type_array;
	array<NEW_ARMOR_ESSENCE>	new_armor_essence_array;

	array<QIHUN_ESSENCE>	qihun_essence_array;
	array<QILING_ESSENCE>	qiling_essence_array;
	array<STORY_BOOK_MAJOR_TYPE>	story_book_major_type_array;
	array<STORY_BOOK_CONFIG>	story_book_config_array;

	array<MENTOR_LEVEL_CONFIG>	mentor_level_config_array;
	array<STUDENT_AWARD_CONFIG>	student_award_config_array; 
	
	
	array<QIHUN_COVER_ESSENCE>	qihun_cover_essence_array;
	array<GROWTH_TARGET_CONFIG>	growth_target_config_array;
	array<QUESTION_TIP_CONFIG>	question_tip_config_array;
	array<QUESTION_AWARD_CONFIG>	question_award_config_array;

	array<SLIDE_SKILL_ESSENCE>	slide_skill_essence_array;
	array<CONSUME_MONEY_CONFIG>	consume_money_config_array;
	array<USE_FOR_SELF_ITEM_ESSENCE>	use_for_self_item_essence_array;

	// New End

	array<HIGH_POLY_FACE_TEXTURE_ESSENCE>	high_poly_face_texture_essence_array;
	array<HIGH_POLY_FACE_TEMPL_CONFIG>	high_poly_face_templ_config_array;

	array<NEW_LOTTERY_CONFIG>	new_lottery_config_array;
	array<CURRENCY_ITEM_WHITELIST_CONFIG>	currency_item_whitelist_config_array;

	array<HIGH_POLY_CUSTOMIZEDATA_ESSENCE>	high_poly_customizedata_essence_array;
	array<HIGH_POLY_RACE_FEATURE>	high_poly_race_feature_array;

	array<BLACK_WHITE_LIST_MAJOR_TYPE>	black_white_lst_major_type_array;
	array<BLACK_WHITE_LIST_CONFIG>	black_white_list_config_array;

	array<LOANS_PRODUCE_MAJOR_TYPE>	loans_produce_major_type_array;
	array<LOANS_PRODUCE_SUB_TYPE>	loans_produce_sub_type_array;
	array<LOANS_PRODUCE_RECIPE>	loans_produce_recipe_array;

	array<RED_BOOK_CONFIG>	red_book_config_array;
	array<RED_BOOK_UPGRADE_ITEM>	red_book_upgrade_item_array;
	array<RED_BOOK_LOTTERY_CONFIG>	red_book_lottery_config_array;

	array<ARENA_TICKET_CONFIG>	arena_ticket_config_array;
	array<CAMP_LEVEL_CONFIG>	camp_level_config_array;
	array<CAMP_TOKEN_CONFIG>	camp_token_config_array;
	array<CAMP_TOKEN_TREE_CONFIG>	camp_token_tree_config_array;
	array<CAMP_TOKEN_ESSENCE>	camp_token_essence_array;
	array<CAMP_TOKEN_UPGRADE_ESSENCE>	camp_token_upgrade_essence_array;
	array<CAMP_TOKEN_ROLLBACK_ESSENCE>	camp_token_rollback_essence_array;
	array<CAMP_TOKEN_PROB_ADJUST_ESSENCE>	camp_token_prob_adjust_essence_array;
	array<CAMP_BATTLE_TECH_TREE_CONFIG>	camp_battle_tech_tree_config_array;
	
	array<SPECIAL_ACTIVITY_RESPAWN_CONFIG>	special_activity_respawn_config_array;
	array<MAP_EVENT_CONFIG>	map_event_config_array;
	array<SPECIAL_CAMP_REGION_MAJOR_TYPE>	special_camp_region_major_type_array;

	array<SPECIAL_CAMP_REGION_CONFIG>	special_camp_region_config_array;
	array<PET_SKIN_CONFIG>	pet_skin_config_array;
	array<PET_MULTIRIDE_CONFIG>	pet_multiride_config_array;

	array<FASHION_NEW_MAJOR_TYPE>	fashion_new_major_type_array;
	array<FASHION_NEW_SUB_TYPE>	fashion_new_sub_type_array;
	array<FASHION_NEW_ESSENCE>	fashion_new_essence_array;

	array<WAR_AREA_CONFIG>	war_area_config_array;

	array<ILLUSTRATED_FASHION_SERIES_CONFIG>	illustrated_fashion_series_config_array;
	array<ILLUSTRATED_FASHION_ESSENCE>	illustrated_fashion_essence_array;
	array<ILLUSTRATED_WING_EGG_ESSENCE>	illustrated_wing_egg_essence_array;
	array<ILLUSTRATED_WING_SERIES_CONFIG>	illustrated_wing_series_config_array;
	array<ILLUSTRATED_PET_EGG_ESSENCE>	illustrated_pet_egg_essence_array;
	array<ILLUSTRATED_PET_SERIES_CONFIG>	illustrated_pet_series_config_array;
	array<ACROSS_SERVER_MATCH_CONFIG>	across_server_match_config_array;
	array<NPC_ASAL_CONFIG>	npc_asal_config_array;
	array<WILDERNESS_SURVIVAL_CONFIG>	wilderness_survival_config_array;
	array<ILLUSTRATED_REWARD_CONFIG>	illustrated_reward_config_array;

	array<FAST_PRODUCE_ITEM_TYPE>	fast_produce_item_type_array;
	array<FAST_PRODUCE_ITEM_ESSENCE>	fast_produce_item_essence_array;

	array<KID_SYSTEM_CONFIG>	kid_system_config_array;
	array<COURSE_ESSENCE>	course_essence_array;
	array<COURSE_SUITE_ESSENCE>	course_suite_essence_array;
	array<KID_PROPERTY_CONFIG>	kid_property_config_array;
	array<KID_QUALITY_CONFIG>	kid_quality_config_array;
	array<KID_EXP_CONFIG>	kid_exp_config_array;
	array<KID_UPGRADE_STAR_CONFIG>	kid_upgrade_star_config_array;
	array<KID_LEVEL_REWARD_CONFIG>	kid_level_reward_config_array;
	array<KID_LEVEL_MAX_CONFIG>	kid_level_max_config_array;
	array<KID_DEBRIS_ESSENCE>	kid_debris_essence_array;
	array<KID_SKILL_CONFIG>	kid_skill_config_array;
	array<KID_DEBRIS_GENERATOR_ESSENCE>	kid_debris_generator_essence_array;
	array<KID_PROFESSION_CONFIG>	kid_profession_config_array;
	array<BIS_SOI_USE_LIMIT_CONFIG>	bis_soi_use_limit_config_array;

	array<REWARD_INTERFACE_CONFIG>	reward_interface_config_array;
	array<HISTORY_VARIABLE_PROGRESS_CONFIG>	history_variable_progress_config_array;
	array<ANNIVERSARY_EVENT_INTERFACE_CONFIG>	anniversary_event_interface_config_array;
	
	array<ACTIVITY_EXPUP_CONFIG>	activity_expup_config_array;
	array<ACTIVITY_CHALLENGE_CONFIG>	activity_challenge_config_array;
	array<ACTIVITY_CONSUMPTION_CONFIG>	activity_consumption_config_array;
	array<ACTIVITY_SHOP_CONFIG>	activity_shop_config_array;
	
	array<REWARD_SHOW_CONFIG>	reward_show_config_array;
	array<EXTR_PROP_SKILL_CONFIG>	extr_prop_skill_config_array;
	array<MATERIAL_REFINE_ESSENCE>	material_refine_essence_array;
	array<MATERIAL_REFINE_TICKET_ESSENCE>	material_refine_ticket_essence_array; // 162 > 171 end

	// 172 > 
	
	array<REMOVE_TASK_RESULT_CONFIG>	remove_task_result_config_array;
	array<LDBATTLE_TRANSFER_POINT_CONFIG>	ldbattle_transfer_point_config_array;
	array<HUAZHAO_ACTIVITY_CONFIG>	huazhao_activity_config_array;
	array<HERO_BOOK_CONFIG>	hero_book_config_array;
	array<CROSS_SERVICE_ROOM_CONFIG>	cross_service_room_config_array;

	array<FLYING_CHESS_CONFIG>	flying_chess_config_array;
	array<HIGHPOLY_MAINCUSTOMIZE_CONFIG>	highpoly_maincustomize_config_array;
	array<HUARONG_ROAD_GAME_CONFIG>	huarong_road_game_config_array;
	array<TASK_BRANCH_RESULT_CONFIG>	task_branch_result_config_array;
	array<TIME_LIMIT_ITEM_CONFIG>	time_limit_item_config_array;

	array<CHRISTMAS_ACTIVITY_CONFIG>	christmas_activity_config_array;
	/*176
	array<PORTRAIT_PICTURE_ESSENCE>	portrait_picture_essence_array;
	array<BLINDBOX_CONFIG>	blindbox_config_array;
	array<ARENA_PEAK_SEASON_CONFIG>	arena_peak_season_config_array;
	array<ARENA_PEAK_BATTLE_CONFIG>	arena_peak_battle_config_array;
	array<ARENA_PEAK_AWARD_CONFIG>	arena_peak_award_config_array;

	array<DEFAULT_HAIR_CONFIG>	default_hair_config_array;
	array<EXPLORE_PROGRESS_DIALOG_CONFIG>	explore_progress_dialog_config_array;
	array<EXPLORE_OBJECT_CONFIG>	explore_object_config_array;
	*/


	// the global hash_map for each id space
private:
	struct LOCATION
	{
		DATA_TYPE		type;
		unsigned int	pos;
	};
	typedef std::unordered_map<unsigned int, LOCATION> IDToLOCATIONMap;
	
	IDToLOCATIONMap				essence_id_index_map;
	IDToLOCATIONMap				addon_id_index_map;
	IDToLOCATIONMap				talk_id_index_map;
	IDToLOCATIONMap				face_id_index_map;
	IDToLOCATIONMap				recipe_id_index_map;
	IDToLOCATIONMap				config_id_index_map;
	IDToLOCATIONMap				home_id_index_map;


	IDToLOCATIONMap::iterator	esssence_id_index_itr;
	IDToLOCATIONMap::iterator	addon_id_index_itr;
	IDToLOCATIONMap::iterator	talk_id_index_itr;
	IDToLOCATIONMap::iterator	face_id_index_itr;
	IDToLOCATIONMap::iterator	recipe_id_index_itr;
	IDToLOCATIONMap::iterator	config_id_index_itr;
	IDToLOCATIONMap::iterator	home_id_index_itr;

	array<void *>				start_ptr_array;
	array<size_t>				type_size_array;
};

#endif
