#include "crypt.h"

namespace GNET
{

Cryptor* Cryptor::instance = 0;

bool Cryptor::Filter(Octets & o)
{
	if (o.size() < 1)
	{
		return false;
	}
	char * str = (char*)o.begin();
	for (unsigned int i = 0; i < o.size() && i < 64; i++)
	{
		if( !(str[i] >= '0' && str[i] <= '9') &&
			!(str[i] >= 'A' && str[i] <= 'Z') &&
			!(str[i] >= 'a' && str[i] <= 'z') &&
			!(str[i] == '_' || str[i] == '=')  )
		return false;
	}
	return true;
}

bool Cryptor::Crypt(Octets & o, Octets & x)
{
	if ( o.size() < 1 || x.size() != 16 )
	{
		return false;
	}
	
	unsigned int i = 0, j = 0, v = 0, q = 0;
	unsigned int len = o.size();
	unsigned char* data = (unsigned char*)o.begin();
	unsigned char* key = (unsigned char*)x.begin();

	for (j = 13; j < len + 13; ++j)
	{
		for (i = 0; i < 4; ++i)
		{
			key[i] ^= 0x81;
		}
		v = 31 ^ j;
		v ^= len;
		q = KEY1 ^ j;
		for (i = 0; i < 16; ++i)
		{
			q ^= key[i];
			v ^= q;
			++v;
			++q;
		}
		for (i = 0; i < len; ++i)
		{
			data[i] ^= key[i % 16];
			v ^= key[i % 16];
			q ^= key[i % 16];
			data[i] ^= v;
			q ^= v;
			data[i] ^= q;
			--v;
			--q;
		}
		q = 61 ^ j;
		q ^= len;
		v = KEY2 ^ j;
		for (i = 0; i < len; ++i)
		{
			v ^= data[i];
			q ^= v;
			--q;
			--v;
		}
		for (i = 0; i < 16; ++i)
		{
			key[i] ^= data[i % len];
			v ^= data[i % len];
			q ^= data[i % len];
			key[i] ^= q;
			v ^= q;
			key[i] ^= v;
			++v;
			++q;
		}
		v = 51 ^ j;
		for (i = 0; i < len; ++i)
		{
			v += data[i];
			data[i] ^= 31;
			v += data[i];
		}
		v *= KEY3;
		*(unsigned int*)&key[0] ^= v ^ 0x2F8DD8E6 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[4] ^= v ^ 0x2374D869 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[8] ^= v ^ 0x3ABBB4B6 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[12] ^= v ^ 0x3E8AB09 ^ KEY_TIMESHTAMP;
	}
	return true;
}

bool Cryptor::Uncrypt(Octets & o, Octets & x)
{
	if ( o.size() < 1 || x.size() != 16 )
	{
		return false;
	}
	
	unsigned int i = 0, j = 0, v = 0, q = 0;
	unsigned int len = o.size();
	unsigned char* data = (unsigned char*)o.begin();
	unsigned char* key = (unsigned char*)x.begin();

	for (j = len + 12; j >= 13; --j)
	{
		v = 51 ^ j;
		for (i = 0; i < len; ++i)
		{
			v += data[i];
			data[i] ^= 31;
			v += data[i];
		}
		v *= KEY3;
		*(unsigned int*)&key[0] ^= v ^ 0x2F8DD8E6 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[4] ^= v ^ 0x2374D869 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[8] ^= v ^ 0x3ABBB4B6 ^ KEY_TIMESHTAMP;
		*(unsigned int*)&key[12] ^= v ^ 0x3E8AB09 ^ KEY_TIMESHTAMP;
		q = 61 ^ j;
		q ^= len;
		//---
		v = KEY2 ^ j;
		for (i = 0; i < len; ++i)
		{
			v ^= data[i];
			q ^= v;
			--q;
			--v;
		}
		for (i = 0; i < 16; ++i)
		{
			key[i] ^= data[i % len];
			v ^= data[i % len];
			q ^= data[i % len];
			key[i] ^= q;
			v ^= q;
			key[i] ^= v;
			++v;
			++q;
		}
		v = 31 ^ j;
		v ^= len;
		q = KEY1 ^ j;
		for (i = 0; i < 16; ++i)
		{
			q ^= key[i];
			v ^= q;
			++v;
			++q;
		}
		for (i = 0; i < len; ++i)
		{
			data[i] ^= key[i % 16];
			v ^= key[i % 16];
			q ^= key[i % 16];
			data[i] ^= v;
			q ^= v;
			data[i] ^= q;
			--v;
			--q;
		}
		for (i = 0; i < 4; ++i)
		{
			key[i] ^= 0x81;
		}
	}
	return true;
}

}


