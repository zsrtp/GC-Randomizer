#pragma once

#include "defines.h"

namespace tp::f_op_actor_mng
{
	extern "C"
	{
		s32 createItemForPresentDemo(const float pos[3], s32 item, u8  unk1, s32            unk2, s32            unk3, const float unk4[3], const float unk5[3]);
		s32 createItemForTrBoxDemo  (const float pos[3], s32 item, s32 unk1, s32            unk2, const float unk3[3], const float unk4[3]);
		s32 createItemForBoss       (const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], float          unk4, float       unk5, s32 unk6);
		s32 createItemForMidBoss    (const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], s32            unk4, s32         unk5);
		s32 createItemForDirectGet  (const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], float          unk4, float       unk5);
	}
}