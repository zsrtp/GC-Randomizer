#pragma once

#include "defines.h"

namespace tp::processor
{
    extern "C"
    {
        void* getResource_groupID( const void* TProcessor, u16 unk2 );
    }
}     // namespace tp::processor