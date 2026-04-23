#ifndef __PLAYER_LIB_H__
#define __PLAYER_LIB_H__

class gplayer_lib_items
{
public:
	enum 
	{
		MAX_LOT = MAX_SLOT_LIB_PRODUCE
	};
private:	
	struct ITEM
	{
		unsigned int id;
		unsigned int count;
	};
private:	
	unsigned int count;
	ITEM item[MAX_LOT];
public:	
	void Init();
	unsigned int Rebuild();
	unsigned int Add(unsigned int id , unsigned int count);
	unsigned int Del(unsigned int id);
	unsigned int GetCleanSlot();
	unsigned int GetFreeSlotsCount();
	void GetInfo( unsigned int & cnt , void * buf );
	
};


#endif