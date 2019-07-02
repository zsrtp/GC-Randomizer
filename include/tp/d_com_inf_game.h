#pragma once

#include "defines.h"

namespace tp::d_com_inf_game
{
	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct LinkMapVars
	{
		u8 unk_0[0x4D0];
		float wPosition[3];
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	struct GameInfo
	{
		u8 unk_0[0x4E00];
		char wCurrentStage[8];
		u8 unk_4e04[0xFA4];
		LinkMapVars* wLinkMapPtr;
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));

	static_assert(sizeof(GameInfo) == 0x1DE10);

	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
	}
}