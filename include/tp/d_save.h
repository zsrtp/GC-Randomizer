#pragma once

#include <tp/d_com_inf_game.h>

#include "defines.h"

namespace tp::d_save
{
    extern "C"
    {
        u16 getRupeeMax();
        void getSave( tp::d_com_inf_game::GameInfo* gameInfoPtr, s32 areaID );
        void putSave( tp::d_com_inf_game::GameInfo* gameInfoPtr, s32 areaID );
    }
}     // namespace tp::d_save