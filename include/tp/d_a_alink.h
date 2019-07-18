#pragma once

#include "defines.h"

namespace tp::d_a_alink
{
	extern "C"
	{
		void setStickData();
		bool checkHeavyStateOn(s32 unk, s32 unk2);
		bool checkTreasureRupeeReturn(void* unused, s32 amount);
	}
}