
#include "platform.h"

#if defined(__x86_64__)
	#include "byteorder_x86_64.h"
#elif defined(__i386__)
	#include "byteorder_i386.h"
#endif
