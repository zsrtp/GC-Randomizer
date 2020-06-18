#pragma once

#include "defines.h"

namespace tp::f_op_actor_mng
{
    extern "C"
    {
        s32 createItemForPresentDemo (const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const s16 rot[3], const float scale[3]);
        s32 createItemForTrBoxDemo (const float pos[3], s32 item, s32 itemPickupFlag, s32 roomNo, const s16 rot[3], const float scale[3]);
        s32 createItemForBoss (const float pos[3], s32 item, s32 roomNo, const s16 rot[3], const float scale[3], float unk6, float unk7, s32 parameters);
        s32 createItemForMidBoss (const float pos[3], s32 item, s32 roomNo, const s16 rot[3], const float scale[3], s32 unk6, s32 itemPickupFlag);
        s32 createItemForDirectGet (const float pos[3], s32 item, s32 unk3, const s16 rot[3], const float scale[3], float unk6, float unk7);
        s32 createItemForSimpleDemo (const float pos[3], s32 item, s32 unk3, const s16 rot[3], const float scale[3], float unk6, float unk7);
    }
}