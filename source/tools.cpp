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
}