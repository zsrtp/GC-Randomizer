#pragma once

#include "defines.h"

namespace mod::tools
{
	extern "C"
	{
		// Seed for getRandom
		static u64 randomSeed = 0x9e3779b97f4a7c15;

		/**
		 * Generates a simple random number (not perfectly random but good enough for most purposes)
		 * Note: It's best to use with a clock that advances randomSeed (or when using defines.h RAND_SEED)
		 * (C) http://xoshiro.di.unimi.it/splitmix64.c
		 * 
		 * @param max The maximum number to return
		 * @returns A random integer between 0 and max, excluding max
		 */
		u32 getRandom(u32 max);
	}
}