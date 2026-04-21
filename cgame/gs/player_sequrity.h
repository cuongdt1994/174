#ifndef __ONLINEGAME_GS_PLAYER_SEQURITY_H__
#define __ONLINEGAME_GS_PLAYER_SEQURITY_H__

#include <db_if.h>
#include <gsp_if.h>
#include <glog.h>

// 172+
class gplayer_safe_lock_passwd
{
public:
	bool	lock; //true = Desabilitado, false = Habilitado
	char	passwd_active; //0 = Desativado, 1 = Ativado, 2 = Bloqueado
	unsigned char	password[16];
public:
	void SetPassword(const char * str, unsigned int size);
	void SetPasswordMD5(const char * str, unsigned int size);
	bool CheckPassword(const char * str, unsigned int size);

public:

	void Init();

	inline bool HasPasswordSafeLock()
	{
		return lock;
	}

	inline void SetPasswordSafeLock(bool value)
	{
		lock = value;
	}

	inline char IsPasswordSafeLockActive()
	{
		return passwd_active;
	}

	inline void SetPasswordSafeLockActive(char value)
	{
		passwd_active = value;
	}
	inline const char * GetPassword(unsigned int & size)
	{
		if(lock)
		{
			size = 16;
			return (const char*) password;
		}
		else
		{
			size = 0;
			return NULL;
		}
	}


public:
	static bool IsPasswordValid(const char * str, unsigned int size)
	{
		static char pass_char[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()+-_='`~;:,<.>/? \"";
		if(size == 0) return true;
		if(size > 16) return false;
		for(unsigned int i = 0; i < size;i ++)
		{
			if(strchr(pass_char,str[i]) == NULL) return false;
		}
		return true;
	}
};




















#endif

