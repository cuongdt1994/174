#ifndef __ONLINEG_GAME_GS_PLAYER_TRASH_BOX_H__
#define __ONLINEG_GAME_GS_PLAYER_TRASH_BOX_H__

#include "item.h"
#include "item_list.h"
#include "config.h"
#include <string.h>
#include <vector.h>

class player_trashbox
{
public:
	enum
	{
		BOX0_BOX_SIZE = 120 , // Inventory
		BOX1_BOX_SIZE = 39 , // Equipamento
		BOX2_BOX_SIZE = 64 , // Missões (Itens)
		BOX3_BOX_SIZE = 0 ,
		BOX4_BOX_SIZE = 0 ,
		BOX5_BOX_SIZE = 200 , // Roupas
		BOX6_BOX_SIZE = 0 ,
		BOX7_BOX_SIZE = 120 ,
		BOX8_BOX_SIZE = 88 , // Runas
		BOX9_BOX_SIZE = 80 ,
		BOX10_BOX_SIZE = 70 ,
		BOX11_BOX_SIZE = 20 ,
		
		MAX_PASSWORD_SIZE = 24,
		MAX_BOX = 8,
	};
private:
	unsigned int 	  		_money;
	bool			_has_passwd;
	unsigned char	_passwd[16];
	
	item_list 		_box0, _box1, _box2, _box3, _box4, _box5, _box6, _box7;
	
	inline item_list & _get_box(int i)
	{
		switch(i)
		{
		case 0: return _box0;
		case 1: return _box1;
		case 2: return _box2;
		case 3: return _box3;
		case 4: return _box4;
		case 5: return _box5;
		case 6: return _box6;
		case 7: return _box7;
		default: return _box0;
		}
	}
		
public:
	static bool IsPasswordValid(const char * str, unsigned int size)
	{
		static char pass_char[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()+-_='`~;:,<.>/? \"";
		if(size == 0) return true;
		if(size > MAX_PASSWORD_SIZE) return false;
		for(unsigned int i = 0; i < size;i ++)
		{
			if(strchr(pass_char,str[i]) == NULL) return false;
		}
		return true;
	}

	void SetPassword(const char * str, unsigned int size);
	void SetPasswordMD5(const char * str, unsigned int size);
	bool CheckPassword(const char * str, unsigned int size);

	bool HasPassword()
	{
		return _has_passwd;
	}

	const char * GetPassword(unsigned int & size)
	{
		if(_has_passwd)
		{
			size = 16;
			return (const char*) _passwd;
		}
		else
		{
			size = 0;
			return NULL;
		}
	}
	
	
protected:
	player_trashbox( );

public:
	player_trashbox( unsigned int size, unsigned int size3, unsigned int size4, unsigned int size5);
	
	void SetOwner(gactive_imp * obj)
	{
		unsigned int i = 0;
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
		_get_box(i++).SetOwner(obj);
	}

	unsigned int & GetMoney() { return _money; }
	item_list & GetBackpack(unsigned int i) { return i < MAX_BOX ? _get_box(i) : _get_box(0); }
	unsigned int GetTrashBoxSize(unsigned int i) { return i < MAX_BOX ? _get_box(i).Size() : _get_box(i).Size(); }
	void SetTrashBoxSize(unsigned int i , unsigned int size) { if (i < MAX_BOX && _get_box(i).Size() < size && size <= TRASHBOX_MAX_SIZE )	_get_box(i).SetSize(size); }
	
	bool Save(archive & ar)
	{
		bool nosave;
		unsigned int i = 0;
		ar << _money;
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		_get_box(i++).Save(ar,nosave);
		ar << _has_passwd;
		ar.push_back(_passwd,16);
		return true;
	}

	bool Load(archive & ar)
	{
		unsigned int i = 0;
		ar >> _money;
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		_get_box(i++).Load(ar);
		ar >> _has_passwd;
		ar.pop_back(_passwd,16);
		return true;
	}

	void Swap(player_trashbox & rhs)
	{
		unsigned int i = 0;
		abase::swap(_money,rhs._money);
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		_get_box(i).Swap(rhs._get_box(i++));
		abase::swap(_has_passwd,rhs._has_passwd);

		unsigned char	passwd[16];
		ASSERT(sizeof(passwd) == sizeof(_passwd));
		memcpy(passwd,_passwd,sizeof(_passwd));
		memcpy(_passwd,rhs._passwd,sizeof(_passwd));
		memcpy(rhs._passwd,passwd,sizeof(_passwd));
	}
};

#endif

