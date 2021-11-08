#pragma once

#include <tp/d_stage.h>

#include "defines.h"

namespace tp::d_item_data
{
    struct ItemResource     // d_item_data.h
    {
        const char* arcName;
        s16 modelResIdx;
        s16 btkResIdx;
        s16 bckResIdx;
        s16 brkResIdx;
        s16 btpResIdx;
        u8 tevFrm;
        u8 btpFrm;
        s16 ringTexResIdx;     // The icon displayed next to the text shown when you get an item.
        s16 unk_12[3];
    } __attribute__( ( __packed__ ) );

    struct FieldItemRes     // d_item_data.h
    {
        const char* arcName;
        s16 modelResIdx;
        s16 bckAnmResIdx;
        s16 brkAnmResIdx;
        s16 unk_a;
        s16 heapSize;
        s16 unk_e;
    } __attribute__( ( __packed__ ) );

    struct ItemInfo     // d_item_data.h
    {
        u8 mShadowSize;
        u8 mCollisionH;
        u8 mCollisionR;
        u8 mFlags;
    } __attribute__( ( __packed__ ) );

    static_assert( sizeof( ItemResource ) == 0x18 );
    static_assert( sizeof( FieldItemRes ) == 0x10 );
    static_assert( sizeof( ItemInfo ) == 0x4 );

    extern "C"
    {
        extern ItemResource item_resource[255];
        extern FieldItemRes field_item_res[255];
        extern ItemInfo item_info[255];
    }
}     // namespace tp::d_item_data