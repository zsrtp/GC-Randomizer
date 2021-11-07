#pragma once

#include <gc/OSModule.h>

#include "defines.h"

namespace tp::dynamic_link
{
    // Should try to fill in the variables at some point
    struct DynamicModuleControl
    {
        u8 unk_0[0x10];
        gc::OSModule::OSModuleInfo* moduleInfo;
        u8 unk_10[0x18];
    } __attribute__( ( __packed__ ) );

    // This size may not be correct
    static_assert( sizeof( DynamicModuleControl ) == 0x2C );

    extern "C"
    {
        bool do_link( DynamicModuleControl* dmc );
    }
}     // namespace tp::dynamic_link