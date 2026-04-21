#ifndef __PLAYER_GLYPH_H__
#define __PLAYER_GLYPH_H__

class gplayer_glyph
{
public:
	enum : unsigned int
	{
		MAX_SLOT = 12,
		MAX_PVP_ATTR = 5,
		MAX_GLYPH_TYPE = 5,
		MAX_GLYPH_LEVEL = 10,
		
		MODE_INC = 0,
		MODE_DEC = 1,
		MODE_UPDATE = 2,
	};
private:
	struct GLYPH
	{
		unsigned int is_open;
		int base_skill;
		int skill;
		unsigned int pvp_attr;
		int it;
		unsigned int type;
		unsigned int level;
	};
	
	struct SEND
	{
		int pos;
		int skill;
		int it;
	};
	
private:
	GLYPH glyph[MAX_SLOT];

public:
	void Init();
	unsigned int Size();
	void Update(int cls);
	void GetInfo(int & iCount , int * _tbl );
	bool SetSkill(int cls, int base_skill, int pvp_point, unsigned int pos);
	bool SetGlyph(int glyph_id, unsigned int pos);
	bool SlotUnlock(unsigned int pos);
	bool SlotCheck(unsigned int pos);
	int GetAttr();
	void SlotSkillClear();
	
	bool Reset(int m_cls, int m_glyph, int m_pos);
	
	inline int GetSkill(unsigned int pos) { return pos < MAX_SLOT ? glyph[pos].skill : 0; }
	inline int GetLevel(unsigned int pos) { return pos < MAX_SLOT ? glyph[pos].level : 0; }
	inline int GetBaseSkill(unsigned int pos) { return pos < MAX_SLOT ? glyph[pos].base_skill : 0; }
	inline int GetItem(unsigned int pos) { return pos < MAX_SLOT ? glyph[pos].it : 0; }
	
};


#endif