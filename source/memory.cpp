#include "memory.h"
#include "defines.h"

#include <gc/OSCache.h>

#include <cstddef>
#include <cstring>

namespace mod::memory
{
	void* clearMemory(void* ptr, std::size_t size)
	{
		return memset(ptr, 0, size);
	}

	void clear_DC_IC_Cache(void* ptr, std::size_t size)
	{
		gc::OSCache::DCFlushRange(ptr, size);
		gc::OSCache::ICInvalidateRange(ptr, size);
	}
}