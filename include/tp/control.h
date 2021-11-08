#pragma once

#include <gc/bmgres.h>

#include "defines.h"

namespace tp::control
{
    struct TControl
    {
        void* unk_0;
        void* unk_4;
        void* unk_8;
        u16 unk_E;     // unk3 in setMessageCode_inSequence
        u16 msgId;
        void* unk_10;
        gc::bmgres::MessageEntry* msgEntry;
        const char* msg;
        u8 unk_1C[0x4];
        const char* wMsgRender;
        u32 unk_24;
    } __attribute__( ( __packed__ ) );

    extern "C"
    {
        bool setMessageCode_inSequence( TControl* control, const void* TProcessor, u16 unk3, u16 msgId );
    }
}     // namespace tp::control