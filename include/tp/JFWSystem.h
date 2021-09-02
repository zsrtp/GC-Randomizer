#pragma once

#include "defines.h"

namespace tp::jfw_system
{
    struct ConsoleLine
    {
        bool showLine;
        char line[61];
    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point
    // Need to also get the exact size of this struct
    struct SystemConsole
    {
        u8 unk_0[0x60];
        u8 consoleColor[4];     // rgba
        u8 unk_64[0x4];
        bool consoleEnabled;
        u8 unk_69[3];
        ConsoleLine consoleLine[25];     // Should figure out the total amount of lines at some point
    } __attribute__( ( __packed__ ) );

    static_assert( sizeof( ConsoleLine ) == 0x3E );

    extern "C"
    {
        extern SystemConsole* systemConsole;
    }
}     // namespace tp::jfw_system