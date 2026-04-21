#include <openssl/md5.h>
#include <uchar.h>
#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>

#include "player_sequrity.h"


/*170+*/
void
gplayer_safe_lock_passwd::Init()
{
	memset(this,0x00,sizeof(*this));
	passwd_active = 2;
}


void
gplayer_safe_lock_passwd::SetPassword(const char * str, unsigned int size)
{
	if(size == 0) 
	{
		lock = false;
		return;
	}
	MD5((unsigned char*)str,size,password);
	lock = true;
}

void
gplayer_safe_lock_passwd::SetPasswordMD5(const char * str, unsigned int size)
{
	if(size != 16) 
	{
		lock = false;
		return;
	}
	//ASSERT(size == 16);
	memcpy(password,str,16);
	lock = true;
}

bool 
gplayer_safe_lock_passwd::CheckPassword(const char * str, unsigned int size)
{
	if(!size && !lock) return true;
	if(!size && lock) return false;
	unsigned char md5[16];
	MD5((unsigned char*)str,size,md5);
	return memcmp(md5,password,16) == 0;
}


