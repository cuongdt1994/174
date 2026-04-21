#ifndef __BYTEORDER_H
#define __BYTEORDER_H

#include "platform.h"

namespace GNET
{

#if defined BYTE_ORDER_BIG_ENDIAN
	#define byteorder_16(x)	(x)
	#define byteorder_32(x)	(x)
	#define byteorder_64(x)	(x)
#elif defined __GNUC__
	/*
	#define byteorder_16 htons
	#define byteorder_32 htonl
	*/
	inline unsigned short byteorder_16(unsigned short _Val)
	{
		return (((_Val << 8) | (_Val >> 8)));
	}
	inline unsigned int byteorder_32(unsigned int _Val)
	{
		return ((_Val << 24) | ((_Val << 8) & 0x00FF'0000) | ((_Val >> 8) & 0x0000'FF00) | (_Val >> 24));
	}
	inline unsigned long long byteorder_64(unsigned long long _Val)
	{
		return ((_Val << 56) | ((_Val << 40) & 0x00FF'0000'0000'0000) | ((_Val << 24) & 0x0000'FF00'0000'0000) 
				| ((_Val << 8) & 0x0000'00FF'0000'0000) | ((_Val >> 8) & 0x0000'0000'FF00'0000) 
				| ((_Val >> 24) & 0x0000'0000'00FF'0000) | ((_Val >> 40) & 0x0000'0000'0000'FF00) | (_Val >> 56));
	}

#elif defined WIN32
	inline unsigned __int16 byteorder_16(unsigned __int16 x)
	{
		__asm ror x, 8
		return x;
	}
	inline unsigned __int32 byteorder_32(unsigned __int32 x)
	{
		__asm mov eax, x
		__asm bswap eax
		__asm mov x, eax
		return x;
	}
	inline unsigned __int64 byteorder_64(unsigned __int64 x)
	{
		__asm mov rax, x
		__asm bswap rax
		__asm mov x,rax
		return x;
	}
#endif
};

#endif
