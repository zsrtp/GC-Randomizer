#pragma once

#include "defines.h"

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
        s32 indexOf( u16 needle, u16* haystack, size_t count );
    }
}     // namespace mod::array