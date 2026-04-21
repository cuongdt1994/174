//------------------------------------------------------------------------------------------------------------------------
//--GLYPH MANAGER (C) 2022 deadraky
//------------------------------------------------------------------------------------------------------------------------
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "glyph_manager.h"

GlyphConfig* GlyphConfig::instance = 0;

void GlyphConfig::Init()
{
	memset(this,0x00,sizeof(GlyphConfig));
	
	int glyph_cls[] = { CLS_GLYPH_CONF00, CLS_GLYPH_CONF01, CLS_GLYPH_CONF02, CLS_GLYPH_CONF03, CLS_GLYPH_CONF04, 
						CLS_GLYPH_CONF05, CLS_GLYPH_CONF06, CLS_GLYPH_CONF07, CLS_GLYPH_CONF08, CLS_GLYPH_CONF09, 
						CLS_GLYPH_CONF10, CLS_GLYPH_CONF11, CLS_GLYPH_CONF12, CLS_GLYPH_CONF13, CLS_GLYPH_CONF14, 0 };

	for(unsigned int i = 0; i < CLS_GLYPH_CONF_MAX; i++)
	{
		DATA_TYPE dt;
		RUNE_SKILL_CONFIG *it = (RUNE_SKILL_CONFIG*)world_manager::GetDataMan().get_data_ptr(glyph_cls[i],ID_SPACE_CONFIG,dt);
		if (it && dt == DT_RUNE_SKILL_CONFIG)
		{
			for(unsigned int j = 0; j < MAX_GLYPH_SKILL; j++)
			{
				glyph[i].skill[j].id[0] = it->list[j].skills_id_skill_base;
				for(unsigned int k = 0; k < MAX_SLOT; k++)
				{
					glyph[i].skill[j].id[(k+1)] = it->list[j].skills_id_skill_rune[k];
				}
			}
		}
		else
		{
			printf("GLYPH_CONFIG INIT FAILED!!! \n");
			ASSERT(false);
			return;
		}
	}
	
	DATA_TYPE dt;
	RUNE_UPGRADE_CONFIG *it = (RUNE_UPGRADE_CONFIG*)world_manager::GetDataMan().get_data_ptr(UPGRADE_GLYPH_CONF,ID_SPACE_CONFIG,dt);
	if (it && dt == DT_RUNE_UPGRADE_CONFIG) 
	{
		for(unsigned int i = 0; i < MAX_GLYPH_LEVEL; i++)
		{
			for(unsigned int j = 0; j < MAX_GLYPH_LEVEL; j++)
			{
				glyph_upgrade[i][j] = it->level[i].probability[j];
			}
		}
	}
	else
	{
		printf("RUNE_UPGRADE_CONFIG INIT FAILED!!! \n");
		ASSERT(false);
		return;
	}
}

int GlyphConfig::GetGlyph(int cls, int base, int type)
{
	if (cls < CLS_GLYPH_CONF_MAX && type < MAX_SLOT)
	{
		for(unsigned int j = 0; j < MAX_GLYPH_SKILL; j++)
		{
			if (glyph[cls].skill[j].id[0] == base)
			{
				return glyph[cls].skill[j].id[type];
			}
		}
	}
	return 0;
}

bool GlyphConfig::CheckBaseSkill(int cls, int base)
{
	if ( cls < CLS_GLYPH_CONF_MAX )
	{
		for(unsigned int j = 0; j < MAX_GLYPH_SKILL; j++)
		{
			if (glyph[cls].skill[j].id[0] == base)
			{
				return true;
			}
		}
	}
	return false;
}

bool GlyphConfig::CheckGlyph(int glyph_id, int & glyph_type, int & glyph_level)
{
	if (glyph_id > 0)
	{
		DATA_TYPE dt;
		RUNE_ITEM_ESSENCE *it = (RUNE_ITEM_ESSENCE*)world_manager::GetDataMan().get_data_ptr(glyph_id,ID_SPACE_ESSENCE,dt);
		if (it && dt == DT_RUNE_ITEM_ESSENCE)
		{
			glyph_type = it->type;
			glyph_level = it->level;
			return true;
		}
	}
	return false;
}
