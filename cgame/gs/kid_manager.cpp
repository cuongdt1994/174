#include <threadpool.h>
#include <malloc.h>
#include <unordered_map>
#include "threadusage.h"
#include "world.h"
#include "worldmanager.h"
#include "arandomgen.h"
#include "threadusage.h"
#include "player_imp.h"
#include "kid_manager.h"
#include "gsp_if.h"

KidManager* KidManager::instance = NULL;

void KidManager::Init()
{
	memset(this,0x00,sizeof(KidManager));
				
}

