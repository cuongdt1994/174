#pragma once

// macros para um trabalho mais conveniente com memoria

#define BSWAP_16(_Val) (((_Val << 8) | (_Val >> 8)));

#define BSWAP_32(_Val) ((_Val << 24) | ((_Val << 8) & 0x00FF'0000) | ((_Val >> 8) & 0x0000'FF00) | (_Val >> 24));

#define BSWAP_64(_Val) ((_Val << 56) | ((_Val << 40) & 0x00FF'0000'0000'0000) | ((_Val << 24) & 0x0000'FF00'0000'0000) \
| ((_Val << 8) & 0x0000'00FF'0000'0000) | ((_Val >> 8) & 0x0000'0000'FF00'0000) \
| ((_Val >> 24) & 0x0000'0000'00FF'0000) | ((_Val >> 40) & 0x0000'0000'0000'FF00) | (_Val >> 56));

#define setup_char(address,value) *(char*)address = value;
#define setup_uchar(address,value) *(unsigned char*)address = value;
#define setup_short(address,value) *(short*)address = value;
#define setup_ushort(address,value) *(unsigned short*)address = value;
#define setup_int(address,value) *(int*)address = value;
#define setup_uint(address,value) *(unsigned int*)address = value;
#define setup_float(address,value) *(float*)address = value;
#define setup_double(address,value) *(double*)address = value;
#define setup_void(address,value) *(void**)address = (void*)value;
#define setup_long(address,value) *(long long*)address = value;
#define setup_ulong(address,value) *(unsigned long long*)address = value;
#define setup_fillmem(address,value,size) memset((void*)address,value,size);
#define setup_copymem(address,value,size) memcpy((void*)address,value,size);
#define setup_size_t(address,value) *(size_t*)address = value;
#define get_int(address,offset) ( *(int*)( &((char*)address)[offset] )  )
#define get_uint(address,offset) ( *(unsigned int*)( &((char*)address)[offset] )  )
#define get_char(address,offset) ( *(char*)( &((char*)address)[offset] )  )
#define get_uchar(address,offset) ( *(unsigned char*)( &((char*)address)[offset] )  )
#define get_short(address,offset) ( *(short*)( &((char*)address)[offset] )  )
#define get_ushort(address,offset) ( *(unsigned short*)( &((char*)address)[offset] )  )
#define get_float(address,offset) ( *(float*)( &((char*)address)[offset] )  )
#define get_double(address,offset) ( *(double*)( &((char*)address)[offset] )  )
#define get_void(address,offset) ( *(void**)( &((char*)address)[offset] )  )
#define get_long(address,offset) ( *(long long*)( &((char*)address)[offset] )  )
#define get_ulong(address,offset) ( *(unsigned long long*)( &((char*)address)[offset] )  )

#define setup_call(address,dest) ((int*)((unsigned int)address + 1))[0] = (int)((unsigned int)dest - (unsigned int)(address + 5)); \
								 ((char*)address)[0] = 0xE8;

#define setup_farjmp(address,dest) ((int*)((size_t)address + 1))[0] = (int)((size_t)dest - (size_t)(address + 5)); \
									 ((char*)address)[0] = 0xE9;

#define setup_farjmp64(address,dest) ((unsigned short*)address)[0] = 0x0B848; \
			((unsigned long long*)(address + 2))[0] = (unsigned long long)dest; \
			((unsigned short*)(address + 10))[0] = 0x0E0FF;

#ifdef _WIN64
#define THISCALL __fastcall
#define THISARG void * _this
#define THISPUSH(_this) _this
#else
#define THISCALL __fastcall
#define THISARG void * _this, void * 
#define THISPUSH(_this) _this, NULL
#endif

