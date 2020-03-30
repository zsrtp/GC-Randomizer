#pragma once

#include "defines.h"

#include <tp/evt_control.h>

namespace tp::d_com_inf_game
{
	// Should try to fill in the variables at some point
	struct ScratchPad
	{
		u8 wQuestLogData[0x7F0];
		u8 eventBits[0x150]; // Bitfield (QuestLogOffset - 7F0)
		u8 miniGameBits[0x18]; // Bitfield
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct CurrentStageVars
	{
		char currentStage[8];
		s16 currentSpawnPoint;
		u8 unknown[4];
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct NextStageVars
	{
		char nextStage[8];
		u8 isVoidorWarp;
		u8 nextSpawnPoint;
		u8 nextRoom;
		u8 nextState;
		u8 unk1;
		u8 unk2;
		u8 triggerLoad;
		u8 fadeType;
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	
	struct EventSystem
	{
		u8 unk00[0x13]; // 4ec8 - 4eda
		u16 immediateControl; // 4edb - 4edc
		u8 unk_00[0x4]; // 4edd - 4EE0
		u8 nextEventID; // 4EE1
		u8 unk1A[0xD1];
		u8 currentEventID;
		u8 unk_ec[0x14];
		tp::evt_control::csFunc* onSkip;		 // if != 0 the function stored here will run, otherwise unskippable
		u8 unk_104[0xC];
		u32 fadeOnSkip; // if != 0 this will fade out the screen when skipping
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct LinkMapVars
	{
		u8 unk_0[0x4D0];
		float pos[3];
	} __attribute__((__packed__));
	
	struct DungeonFlags
	{
		u8 miniBossBeaten : 1,
		 ooccooGotten : 1,
		 bitSix : 1,
		 containerGotten : 1,
		 bossBeaten : 1,
		 bigKeyGotten : 1,
		 compassGotten : 1,
		 mapGotten : 1;
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	struct AreaNodes
	{
		u8 unk_0[0x1C];
		u8 nbKeys;
		DungeonFlags dungeon;
		u8 unk_1E[0x2];
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	struct GameInfo
	{
		ScratchPad scratchPad; // 0 - 957
		AreaNodes localAreaNodes; // 958 - 977 holds flags about the current area
        u8 unk_978[0x450]; // 978 - DC7
		u8 respawnCutscene; // DC8 - DC8
		u8 unkdc9[0xA]; // dc9 - dd2
		u8 respawnAnimation; // dd3 - dd3
		u8 unkdd4[0x402C]; // dd4 - 4DFF
		char currentStage[8];  // 4E00 - 4E07
		u8 unk_4e08[6];  // 4E08 - 4E0D
		NextStageVars nextStageVars; // 4E0E - 4E1b
		u8 unk_4e1c[0xAA]; // 4E19 - 4EC7
		EventSystem eventSystem; // 4EC8 - 4FDE
		u8 unk_4fdd[0xDD0]; // 4FDD - 5DBF
		LinkMapVars* linkMapPtr; // 5DA0 - 5DAB
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));


	static_assert(sizeof(ScratchPad) == 0x958);
	static_assert(sizeof(GameInfo) == 0x1DE10);
	static_assert(sizeof(DungeonFlags) == 0x1);

	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
	}
}