#include "amemobj.h"
#include "amemory.h"
#include "spinlock.h"
#include "ASSERT.h"
#include <unordered_map>

#define CHECKMEM 1

namespace abase
{
	static int _lock = 0;
	static std::unordered_map<void * ,int> _lock_map;

	void RecordMemAlloc(const void * p,size_t size)
	{
		spin_autolock alock(_lock);
		int & t = _lock_map[(void*)p];
		ASSERT(t == 0);
		t ++;
	}

	void RecordMemFree(const void * p,size_t size)
	{
		spin_autolock alock(_lock);
		int & t = _lock_map[(void*)p];
		ASSERT( t == 1);
		t --;
	}

	void *
		ASmallObject::operator new(size_t size)
		{
			return fast_allocator::alloc(size);
		}
	void
		ASmallObject::operator delete(void * p, size_t size)
		{
			fast_allocator::free(p,size);
		}
};

