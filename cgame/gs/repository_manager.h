//------------------------------------------------------------------------------------------------------------------------
//--GLYPH MANAGER (C) 2022 deadraky
//------------------------------------------------------------------------------------------------------------------------
#ifndef __GNET_REPOSITORY_MANAGER_H
#define __GNET_REPOSITORY_MANAGER_H


class RepositoryConfig
{
public:
	enum 
	{
		MAX_REPOSITORY = 64,
		MAX_SUITE_ITEM = 12,
	};
public:
	struct repository_item
	{
		int suite_id;
		int equip[MAX_SUITE_ITEM];
	};
	
public:
	int count;	
	repository_item list[MAX_REPOSITORY];

public:
	void Init();
	int Check(int item_id);

	
public:

	RepositoryConfig();
	~RepositoryConfig();

	static RepositoryConfig * GetInstance()
	{
		if (!instance)
		instance = new RepositoryConfig();
		return instance;
	}
	static RepositoryConfig * instance;
};

#endif
