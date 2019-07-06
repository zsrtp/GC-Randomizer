#include "array.h"
#include "memory.h"
#include <cstring>

namespace mod::array
{
	void shuffle(void** array, size_t n, size_t length)
	{
		// TODO
	}

	s32 indexOf(void* needle, void** haystack, size_t n, size_t length)
	{
		for (u32 i = 0; i < length; i += n)
		{
			if (memcmp(needle, haystack[i], n) == 0)
			{
				return static_cast<s32>(i);
			}
		}
		return -1;
	}
}