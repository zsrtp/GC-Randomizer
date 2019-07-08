#pragma once

#include "defines.h"

namespace mod::array
{
	extern "C"
	{
		/**
		 * Shuffles all elements of an array
		 * 
		 * @param array 	Pointer to the array that's being shuffled
		 * @param n 		Size of one element
		 * @param count 	Number of elements within the array
		 * @returns Nothing
		 */
		void shuffle(void** array, size_t n, size_t count);

		/**
		 * Checks whether an array contains a value and returns it's index
		 * 
		 * @param needle 	The value to be searched for
		 * @param haystack 	Pointer to the array
		 * @param count 	Number of elements within the array
		 * @returns If successful the index of needle inside the array, otherwise -1
		 */
		s32 indexOf(u16 needle, u16* haystack, size_t count);
	}
}