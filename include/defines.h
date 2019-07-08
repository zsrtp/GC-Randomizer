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

// Helper
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0b10000000 ? '1' : '0'), \
  (byte & 0b01000000 ? '1' : '0'), \
  (byte & 0b00100000 ? '1' : '0'), \
  (byte & 0b00010000 ? '1' : '0'), \
  (byte & 0b00001000 ? '1' : '0'), \
  (byte & 0b00000100 ? '1' : '0'), \
  (byte & 0b00000010 ? '1' : '0'), \
  (byte & 0b00000001 ? '1' : '0')

// Mnemonics
#define RAND_SEED			mod::tools::randomSeed
#define getPlayerPos 		tp::d_map_path_dmap::getMapPlayerPos
#define sysConsolePtr 		tp::jfw_system::systemConsole