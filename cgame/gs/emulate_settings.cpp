#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "emulate_settings.h"
#include "gsp_if.h"

#include <glog.h>

EmulateSettings* EmulateSettings::instance = NULL;

void EmulateSettings::Init()
{	
	memset(this,0x00,sizeof(EmulateSettings));	
		
	Conf *emulate_settings = Conf::GetInstance();
	
	// Versão do Emulador
	emulate_version = atoi(emulate_settings->find("Static_Setting", "emulate_version").c_str()) > MAX_EMULATE_VERSION ? MAX_EMULATE_VERSION : atoi(emulate_settings->find("Static_Setting", "emulate_version").c_str());

	// Define as novas rates
	rate_config.mob_exp = atoi(emulate_settings->find("Rates_Setting", "mob_exp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "mob_exp").c_str());
	rate_config.mob_sp = atoi(emulate_settings->find("Rates_Setting", "mob_sp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "mob_sp").c_str());
	rate_config.mob_money = atoi(emulate_settings->find("Rates_Setting", "mob_money").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "mob_money").c_str());
	rate_config.mob_drop = atoi(emulate_settings->find("Rates_Setting", "mob_drop").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "mob_drop").c_str());
	rate_config.task_exp = atoi(emulate_settings->find("Rates_Setting", "task_exp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "task_exp").c_str());
	rate_config.task_sp = atoi(emulate_settings->find("Rates_Setting", "task_sp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "task_sp").c_str());
	rate_config.task_money = atoi(emulate_settings->find("Rates_Setting", "task_money").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "task_money").c_str());
	rate_config.realm_exp = atoi(emulate_settings->find("Rates_Setting", "realm_exp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "realm_exp").c_str());
	rate_config.pet_exp = atoi(emulate_settings->find("Rates_Setting", "pet_exp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "pet_exp").c_str());
	rate_config.genie_exp = atoi(emulate_settings->find("Rates_Setting", "genie_exp").c_str()) > MAX_RATE_VALUE ? MAX_RATE_VALUE : atoi(emulate_settings->find("Rates_Setting", "genie_exp").c_str());

	// Habilitar tradução do chat
	enabled_translation = atoi(emulate_settings->find("Static_Setting", "enabled_translation").c_str()) > MAX_ENABLED_TRANSLATE ? MAX_ENABLED_TRANSLATE : atoi(emulate_settings->find("Static_Setting", "enabled_translation").c_str());

	// Configurações de Nw
	nw_config.enabled_team = atoi(emulate_settings->find("Nw_Setting", "enabled_team").c_str()) > MAX_ENABLED_NW ? MAX_ENABLED_NW : atoi(emulate_settings->find("Nw_Setting", "enabled_team").c_str());
	nw_config.min_level_required_nw = atoi(emulate_settings->find("Nw_Setting", "min_level_required_nw").c_str()) > MAX_LEVEL ? MAX_LEVEL : atoi(emulate_settings->find("Nw_Setting", "min_level_required_nw").c_str());
	nw_config.min_level2_required_nw = atoi(emulate_settings->find("Nw_Setting", "min_level2_required_nw").c_str()) > MAX_LEVEL_2 ? MAX_LEVEL_2 : atoi(emulate_settings->find("Nw_Setting", "min_level2_required_nw").c_str());
	nw_config.min_realm_level_required_nw = atoi(emulate_settings->find("Nw_Setting", "min_realm_level_required_nw").c_str()) > MAX_REALM_LEVEL ? MAX_REALM_LEVEL : atoi(emulate_settings->find("Nw_Setting", "min_realm_level_required_nw").c_str());

	msg_language = atoi(emulate_settings->find("Static_Setting", "msg_language").c_str()) > MAX_MSG_LANGUAGE ? MAX_MSG_LANGUAGE : atoi(emulate_settings->find("Static_Setting", "msg_language").c_str());

	enabled_memorial_reset = atoi(emulate_settings->find("Default_Setting", "enabled_memorial_reset").c_str()) > MAX_ENABLED_MEMORIAL_RESET ? MAX_ENABLED_MEMORIAL_RESET : atoi(emulate_settings->find("Default_Setting", "enabled_memorial_reset").c_str());
	enabled_child = atoi(emulate_settings->find("Custom_Setting", "enabled_child").c_str()) > MAX_ENABLED_CHILD ? MAX_ENABLED_CHILD : atoi(emulate_settings->find("Custom_Setting", "enabled_child").c_str());

	// Kid (Child Awakening) tuning — all keys in [Custom_Setting]; missing keys fall back to safe defaults via getters.
	child_awakening_days     = atoi(emulate_settings->find("Custom_Setting", "child_awakening_days").c_str())     > MAX_CHILD_AWAKENING_DAYS     ? MAX_CHILD_AWAKENING_DAYS     : atoi(emulate_settings->find("Custom_Setting", "child_awakening_days").c_str());
	kid_points_rate          = atoi(emulate_settings->find("Custom_Setting", "kid_points_rate").c_str())          > MAX_KID_POINTS_RATE          ? MAX_KID_POINTS_RATE          : atoi(emulate_settings->find("Custom_Setting", "kid_points_rate").c_str());
	kid_awakening_cash       = atoi(emulate_settings->find("Custom_Setting", "kid_awakening_cash").c_str())       > MAX_KID_AWAKENING_CASH       ? MAX_KID_AWAKENING_CASH       : atoi(emulate_settings->find("Custom_Setting", "kid_awakening_cash").c_str());
	kid_free_celestial_level = atoi(emulate_settings->find("Custom_Setting", "kid_free_celestial_level").c_str()) > MAX_KID_FREE_CELESTIAL_LEVEL ? MAX_KID_FREE_CELESTIAL_LEVEL : atoi(emulate_settings->find("Custom_Setting", "kid_free_celestial_level").c_str());
}