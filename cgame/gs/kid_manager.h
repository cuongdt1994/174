#ifndef __GNET_KID_MANAGER_H
#define __GNET_KID_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <mutex>
#include <vector>
#include <sys/stat.h>
#include <sys/time.h>

class KidManager
{
public:
	enum
	{
		GROUP_IDX0 = 2367,		
	};
	static KidManager * instance;

private:

	struct RES
	{
		int reserved;	
	};


private:

	RES reserved[0];
	
public:
	void Init();

	
KidManager()
{

}

~KidManager()
{
	
}

static KidManager * GetInstance()
{
	if (!instance)
	{
		instance = new KidManager();
	}
	return instance;
}

};


#endif