#include <common/utf.h>
#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include <db_if.h>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "usermsg.h"
#include "public_quest.h"
#include "luamanager.h"
#include "glyph_manager.h"
#include "player_glyph.h"

void gplayer_glyph::Init()
{
	memset(this,0x00,sizeof(gplayer_glyph));
}

unsigned int gplayer_glyph::Size()
{
	unsigned int res = 0;
	for(unsigned int i = 0; i < MAX_SLOT; i++)
	{
		if (glyph[i].is_open) 
		{
			res++;
		}
	}
	return res;
}

void gplayer_glyph::GetInfo(int & iCount , int * _tbl )
{
	iCount = Size();
	if (_tbl && iCount)
	{
		SEND * tbl = (SEND *)_tbl;
		for(unsigned int i = 0; glyph[i].is_open && i < MAX_SLOT; i++)
		{
			tbl[i].pos 		= i;
			tbl[i].skill	= glyph[i].base_skill;
			tbl[i].it		= glyph[i].it;
		}
	}
}

void gplayer_glyph::Update(int cls)
{
	GlyphConfig * gc = GlyphConfig::GetInstance();
	for(unsigned int i = 0; i < MAX_SLOT; i++)
	{
		if ( glyph[i].is_open && glyph[i].base_skill)
		{
			glyph[i].skill = glyph[i].it ? gc->GetGlyph(cls, glyph[i].base_skill, glyph[i].type) : 0;
		}
	}
}

bool gplayer_glyph::SetSkill(int cls, int base_skill, int pvp_attr, unsigned int pos)
{
	GlyphConfig * gc = GlyphConfig::GetInstance();
	if (pos < MAX_SLOT && glyph[pos].is_open)
	{
		if (base_skill < 1) //очищаем слот 
		{
			glyph[pos].base_skill = 0;
			glyph[pos].pvp_attr = 0;
			glyph[pos].skill = 0;
			return true;
		}
		else // инкрустируем слот
		{
			for(unsigned int i = 0; i < MAX_SLOT; i++)
			{
				//очищаем если где-то используется
				if ( glyph[i].base_skill ==  base_skill)
				{
					glyph[i].base_skill = 0;
					glyph[i].pvp_attr = 0;
					glyph[i].skill = 0;
				}
			}
			
			if (gc->CheckBaseSkill(cls, base_skill))
			{
				glyph[pos].base_skill = base_skill;
				if (pvp_attr < MAX_PVP_ATTR)
				{
					glyph[pos].pvp_attr = pvp_attr;
					glyph[pos].skill = glyph[pos].it ? gc->GetGlyph(cls, glyph[pos].base_skill, glyph[pos].type) : 0;
				}
				return true;
			}
		}
	}
	return false;
}

bool gplayer_glyph::SetGlyph(int glyph_it, unsigned int pos)
{
	if (pos < MAX_SLOT && glyph[pos].is_open)
	{
		if( glyph_it < 1 ) //очищаем слот 
		{
			glyph[pos].it = 0;
			glyph[pos].type = 0;
			glyph[pos].level = 0;
			return true;
		}
		else // инкрустируем слот
		{
			int glyph_type = 0;
			int glyph_level = 0;
			if (GlyphConfig::GetInstance()->CheckGlyph(glyph_it, glyph_type, glyph_level) )
			{
				glyph[pos].it = glyph_it;
				glyph[pos].type = glyph_type;
				glyph[pos].level = glyph_level;
				return true;
			}
		}
	}
	return false;
}

bool gplayer_glyph::SlotUnlock(unsigned int pos)
{
	if (pos < MAX_SLOT && !glyph[pos].is_open && (!pos || glyph[pos-1].is_open) )
	{
		glyph[pos].is_open = true;
		return true;
	}
	return false;
}

bool gplayer_glyph::SlotCheck(unsigned int pos)
{
	return pos < MAX_SLOT && glyph[pos].is_open;
}

int gplayer_glyph::GetAttr()
{
	int res = 0;
	if ( glyph[5].pvp_attr == glyph[4].pvp_attr && glyph[5].pvp_attr == glyph[3].pvp_attr )
	{
		res = glyph[5].pvp_attr;
	}
	return res;
}

void gplayer_glyph::SlotSkillClear()
{
	for(unsigned int i = 0; i < MAX_SLOT; i++)
	{
		SetSkill(0, 0, 0, i);
	}
}

bool gplayer_glyph::Reset(int m_cls, int m_glyph, int m_pos)
{
	bool res = false;
	GlyphConfig * gc = GlyphConfig::GetInstance();
	if (m_pos < Size() && m_pos < MAX_SLOT)
	{
		if ( glyph[m_pos].is_open && glyph[m_pos].base_skill )
		{
			SetGlyph(m_glyph, m_pos);
			glyph[m_pos].skill = glyph[m_pos].it ? gc->GetGlyph(m_cls, glyph[m_pos].base_skill, glyph[m_pos].type) : 0;
			res = true;
		}
		else
		{
			SetGlyph(0, m_pos);
		}
	}
	return res;
}
