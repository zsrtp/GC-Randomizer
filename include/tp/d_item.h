#pragma once

#include "defines.h"

namespace tp::d_item
{
    typedef void ( *ItemFunc )();
    typedef s32 ( *ItemGetCheckFunc )();

    extern "C"
    {
        s32 execItemGet( u8 item );
        s32 checkItemGet( u8 item, s32 defaultValue );
        void item_func_UTUWA_HEART();
        extern ItemFunc item_func_ptr[0x100];
        extern ItemGetCheckFunc item_getcheck_func_ptr[0x100];
    }
}     // namespace tp::d_item