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

public:
	void Init();
	void SetRatesConfig();
	RATES * GetRatesConfig() { return &rate_config; };
	inline int GetEmulateVersion() { return emulate_version; }
	inline bool GetEnabledTranslation() { return (char)enabled_translation; }

	NW * GetNwConfig() { return &nw_config; };
		
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