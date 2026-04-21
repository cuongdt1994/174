//------------------------------------------------------------------------------------------------------------------------
//--GLYPH MANAGER (C) 2022 deadraky
//------------------------------------------------------------------------------------------------------------------------
#ifndef __GNET_GLYPH_MANAGER_H
#define __GNET_GLYPH_MANAGER_H


class GlyphConfig
{
public:
	enum 
	{
		MAX_SLOT = 12,
		MAX_GLYPH_SKILL = 50,
		MAX_GLYPH_LEVEL = 10,

		CLS_GLYPH_CONF00 = 2369, //вар 
		CLS_GLYPH_CONF01 = 2370, //маг
		CLS_GLYPH_CONF02 = 2376, //шаман
		CLS_GLYPH_CONF03 = 2372, //друид
		CLS_GLYPH_CONF04 = 2371, //обор
		CLS_GLYPH_CONF05 = 2375, //убийца
		CLS_GLYPH_CONF06 = 2373, //лучник
		CLS_GLYPH_CONF07 = 2374, //жрец
		CLS_GLYPH_CONF08 = 2377, //страж
		CLS_GLYPH_CONF09 = 2378, //мистик
		CLS_GLYPH_CONF10 = 2379, //призрак
		CLS_GLYPH_CONF11 = 2380, //жнец
		CLS_GLYPH_CONF12 = 5146, //стрелок
		CLS_GLYPH_CONF13 = 5147, //паладин				
		CLS_GLYPH_CONF14 = 7267, //Andarilho
		
		CLS_GLYPH_CONF_MAX = 15, //15
		
		UPGRADE_GLYPH_CONF = 2368,
		
	};
private:	
	struct GLYPH
	{
		struct
		{
			int id[MAX_SLOT];
		}	skill[MAX_GLYPH_SKILL];
	};
	
	float glyph_upgrade[MAX_GLYPH_LEVEL][MAX_GLYPH_LEVEL];
	
private:
	GLYPH glyph[CLS_GLYPH_CONF_MAX];
	
public:
	void Init();
	int GetGlyph(int cls, int base, int type);
	bool CheckBaseSkill(int cls, int base);
	bool CheckGlyph(int glyph_id, int & glyph_type, int & glyph_level);

	inline float GetUpgradeProb(unsigned int level, unsigned int req_level) { return level < MAX_GLYPH_LEVEL && req_level < MAX_GLYPH_LEVEL ? glyph_upgrade[level][req_level] : 0.0f; }
	
	static GlyphConfig * GetInstance()
	{
		if (!instance)
		instance = new GlyphConfig();
		return instance;
	}
	static GlyphConfig * instance;
};

#endif
