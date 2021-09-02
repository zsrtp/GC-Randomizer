#pragma once

#include "defines.h"

namespace tp::d_kankyo
{
    // Should try to fill in the missing variables at some point
    struct EnvLight
    {
        u8 unk_0[0x98C];
        u8 currentRoom;
        u8 unk_98d[0x983];
    } __attribute__( ( __packed__ ) );

    static_assert( sizeof( EnvLight ) == 0x1310 );

    extern "C"
    {
        extern EnvLight env_light;
    }
}     // namespace tp::d_kankyo