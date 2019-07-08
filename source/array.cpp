#include "array.h"
#include "memory.h"

#include <cstring>

namespace mod::array
{
	void shuffle(void** array, size_t n, size_t count)
	{
		// TODO
	}

	s32 indexOf(u16 needle, u16* haystack, size_t count)
	{
		for (u32 i = 0; i < count; i++)
		{
			if (haystack[i] == needle)
			{
				return static_cast<s32>(i);
			}
		}
		return -1;
	}
}