#pragma once

#include "defines.h"

namespace tp::d_map_path_dmap
{
    extern "C"
    {
        void getMapPlayerPos( float posOut[3] );
        u32 getMapPlayerAngleY();
    }
}     // namespace tp::d_map_path_dmap