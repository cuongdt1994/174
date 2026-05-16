#ifndef __GNET_EMULATE_SETTINGS_H
#define __GNET_EMULATE_SETTINGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class EmulateSettings
{
public:
	enum
	{
		MAX_RATE_VALUE = 999999,
		MAX_EMULATE_VERSION = 176,
		MAX_ENABLED_TRANSLATE = 1,
		MAX_ENABLED_NW = 1,
		MAX_LEVEL = 105,
		MAX_LEVEL_2 = 41,
		MAX_REALM_LEVEL = 120,
		MAX_ENABLED_CHILD = 1,
		MAX_ENABLED_MEMORIAL_RESET = 1,
		MAX_MSG_LANGUAGE = 2,
		MAX_CHILD_AWAKENING_DAYS = 30,
		MAX_KID_POINTS_RATE = 9999,
		MAX_KID_AWAKENING_CASH = 9999,
		MAX_KID_FREE_CELESTIAL_LEVEL = 1,
		MAX_KID_FORCE_NEW_DAY = 1,
	};

	static EmulateSettings * instance;

private:

	struct RATES
	{
		int	mob_exp;	
		int	mob_sp;
		int	mob_money;
		int	mob_drop;
				
		int	task_exp;
		int	task_sp;
		int	task_money;
		
		int realm_exp;

		int pet_exp;
		int genie_exp;
	};

	struct NW
	{
		int enabled_team;
		int min_level_required_nw;
		int min_level2_required_nw;
		int min_realm_level_required_nw;
	};

private:
	// Config do emulador
	int emulate_version;
	int enabled_translation;

	RATES rate_config;
	NW nw_config;

	int msg_language;
	int enabled_child;
	int child_awakening_days;
	int enabled_memorial_reset;
	char google_translate_api_key[256];

	// Kid custom settings
	int kid_points_rate;
	int kid_awakening_cash;
	int kid_free_celestial_level;
	int kid_force_new_day;
public:
	void Init();
	void SetRatesConfig();
	RATES * GetRatesConfig() { return &rate_config; };
	inline int GetEmulateVersion() { return emulate_version; }
	inline bool GetEnabledTranslation() { return (char)enabled_translation; }

	NW * GetNwConfig() { return &nw_config; };
	inline int GetMsgLanguage() { return msg_language; }
		
	inline bool GetEnabledChild() { return (char)enabled_child; }
	// Days required to complete the awakening cycle. Default 7 if not set.
	inline int GetChildAwakeningDays() { return child_awakening_days > 0 ? child_awakening_days : 7; }
	inline bool GetEnabledMemorialReset() { return (char)enabled_memorial_reset; }
	const char* GetGoogleTranslateApiKey() { return google_translate_api_key; }

	// Kid getters — default-safe values preserve original hardcoded behavior when key absent in gs.conf
	inline int GetKidPointsRate() { return kid_points_rate > 0 ? kid_points_rate : 1; }
	inline int GetKidAwakeningCash() { return kid_awakening_cash > 0 ? kid_awakening_cash : 2000; }
	inline bool GetKidFreeCelestialLevel() { return kid_free_celestial_level == 1; }
	inline bool GetKidForceNewDay() { return kid_force_new_day == 1; }
EmulateSettings()
{

}

~EmulateSettings()
{
	
}

static EmulateSettings * GetInstance()
{
	if (!instance)
	{
		instance = new EmulateSettings();
	}
	return instance;
}

};


#endif