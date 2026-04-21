#include <openssl/md5.h>
#include "world.h"
#include "playertrashbox.h"

//nothing to do
void
player_trashbox::SetPassword(const char * str, unsigned int size)
{
	if(size == 0) 
	{
		_has_passwd = false;
		return;
	}
	MD5((unsigned char*)str,size,_passwd);
	_has_passwd = true;
}

void
player_trashbox::SetPasswordMD5(const char * str, unsigned int size)
{
	if(size != 16) 
	{
		_has_passwd = false;
		return;
	}
	//ASSERT(size == 16);
	memcpy(_passwd,str,16);
	_has_passwd = true;
}

bool 
player_trashbox::CheckPassword(const char * str, unsigned int size)
{
	if(!size && !_has_passwd) return true;
	if(!size && _has_passwd) return false;
	unsigned char md5[16];
	MD5((unsigned char*)str,size,md5);
	return memcmp(md5,_passwd,16) == 0;
}
	
player_trashbox::player_trashbox( ):_money(0),_has_passwd(false),
_box0(item::BACKPACK,TRASHBOX_BASE_SIZE	), _box1(item::BACKPACK,0					),
_box2(item::BACKPACK,0					), _box3(item::BACKPACK,TRASHBOX_BASE_SIZE4	),
_box4(item::BACKPACK,BOX8_BOX_SIZE		), _box5(item::BACKPACK,BOX9_BOX_SIZE		),
_box6(item::BACKPACK,BOX10_BOX_SIZE		), _box7(item::BACKPACK,BOX11_BOX_SIZE		)
{
}

player_trashbox::player_trashbox( unsigned int size, unsigned int size3, unsigned int size4, unsigned int size5):_money(0),_has_passwd(false),
_box0(item::BACKPACK,size				), _box1(item::BACKPACK,0					),
_box2(item::BACKPACK,0					), _box3(item::BACKPACK,size3				),
_box4(item::BACKPACK,size4				), _box5(item::BACKPACK,size5				),
_box6(item::BACKPACK,BOX10_BOX_SIZE		), _box7(item::BACKPACK,BOX11_BOX_SIZE		)
{
}

