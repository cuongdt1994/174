//------------------------------------------------------------------------------------------------------------------------
//--CRIPTOGRAFY (C) 2021 deadraky
//------------------------------------------------------------------------------------------------------------------------

#ifndef __CRYPTOGRAPHY_H
#define __CRYPTOGRAPHY_H
#include "octets.h"

namespace GNET
{

class Cryptor
{
private:
	unsigned char  KEY1;
	unsigned char  KEY2;
	unsigned short KEY3;
	unsigned int   KEY_TIMESHTAMP;
public:
	Cryptor() : KEY1(51), KEY2(93), KEY3(3001), KEY_TIMESHTAMP(0)  { }
	~Cryptor() {}

	void Init( unsigned int key ) { KEY_TIMESHTAMP = key; }
	void Init(unsigned char k1 , unsigned char k2, unsigned short k3, unsigned int key ) { KEY1=k1, KEY2=k2, KEY3=k3, KEY_TIMESHTAMP = key; }

	bool Filter (Octets & o);
	bool Crypt  (Octets & o, Octets & x);
	bool Uncrypt(Octets & o, Octets & x);
	
	static Cryptor * GetInstance()
	{
		if (!instance)
		instance = new Cryptor();
		return instance;
	}
	static Cryptor * instance;
};

};

#endif	