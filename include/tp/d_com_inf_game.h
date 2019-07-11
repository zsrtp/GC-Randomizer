#pragma once

#include "defines.h"

namespace tp::d_com_inf_game
{
	// Should try to fill in the variables at some point
	struct ScratchPad
	{
		u8 wQuestLogData[0x958];
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct NextStageVars
	{
		char nextStage[8];
		s16 nextSpawnPoint;
		u8 nextRoom;
		u8 nextState;
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct LinkMapVars
	{
		u8 unk_0[0x4D0];
		float pos[3];
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	struct GameInfo
	{
		ScratchPad scratchPad;
		u8 unk_958[0x44A8];
		char currentStage[8];
		u8 unk_44b0[6];
		NextStageVars nextStageVars;
		u8 unk_4e18[0xF92];
		LinkMapVars* linkMapPtr;
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));

	static_assert(sizeof(ScratchPad) == 0x958);
	static_assert(sizeof(GameInfo) == 0x1DE10);

	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
	}
}