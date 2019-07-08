#include "tools.h"
#include "defines.h"

namespace mod::tools
{
	u32 getRandom(u32 max)
	{
		u64 z = (RAND_SEED += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

		return (z % max);
	}

	u16 fletcher16(u8 *data, size_t count)
	{
		u16 sum1 = 0;
		u16 sum2 = 0;
		size_t index;

		for (index = 0; index < count; ++index)
		{
			sum1 = (sum1 + data[index]) % 255;
			sum2 = (sum2 + sum1) % 255;
		}

		return (sum2 << 8) | sum1;
	}
}