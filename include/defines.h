#pragma once

#include <cstddef>

using std::size_t;

// Data types
typedef unsigned long long 	u64;
typedef	signed long long 	s64;
typedef unsigned int 		u32;
typedef	signed int 			s32;
typedef unsigned short 		u16;
typedef	signed short 		s16;
typedef unsigned char 		u8;
typedef	signed char 		s8;

// Mnemonics
#define RAND_SEED			mod::tools::randomSeed
#define getPlayerPos 		tp::d_map_path_dmap::getMapPlayerPos
#define sysConsolePtr 		tp::jfw_system::systemConsole