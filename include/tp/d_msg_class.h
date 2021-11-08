#pragma once

#include "defines.h"

namespace tp::d_msg_class
{
    extern "C"
    {
        u32 getFontCCColorTable( u8 colorId, u8 unk );
        u32 getFontGCColorTable( u8 colorId, u8 unk );
    }
}     // namespace tp::d_msg_class