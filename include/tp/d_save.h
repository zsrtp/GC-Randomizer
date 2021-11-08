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
        bool isDungeonItem( void* memBitPtr, const int param_1 );
        bool isEventBit( u8* eventSystem, u16 indexNumber );
        void onEventBit( u8* eventSystem, u16 indexNumber );
    }
}     // namespace tp::d_save