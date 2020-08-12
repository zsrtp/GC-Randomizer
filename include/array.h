#pragma once

#include "defines.h"
#include "mod.h"

namespace mod::array
{
	
	extern "C"
	{
		/**
		 * Checks whether an array contains a value and returns it's index
		 * 
		 * @param needle 	The value to be searched for
		 * @param haystack 	Pointer to the array
		 * @param count 	Number of elements within the array
		 * @returns If successful the index of needle inside the array, otherwise -1
		 */
		s32 indexOf(u16 needle, u16* haystack, size_t count);

		u32 getRandomBgmId(u32 originalId);
		u32 getRandomAudioStreamId(u32 originalId);

		extern u8 bgmIndexArray[0xAA];
		extern u8 audioStreamingIndexArray[0x81];
		extern u8 invalidBgmIndexArray[0x51];
		extern u8 bgmFanfareArray[0x8];
		extern bool checkIfBgmIdIsFanfare(u8 bgmId);
		extern bool checkIfBgmIdIsValid(u8 bgmId);
	}
}