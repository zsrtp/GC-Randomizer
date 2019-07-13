#include "game_patches.h"
#include "defines.h"
#include "stage.h"
#include "tools.h"

#include <tp/JFWSystem.h>
#include <tp/d_com_inf_game.h>
#include <tp/evt_control.h>
#include <cstring>
#include <cstdio>

namespace mod::cutscene_skip
{
	/**
	 * Runs when master sword CS is skipped
	 */
	s32 onMasterSwordSkip(void* unk, s32 unk2)
	{
		game_patch::giveMasterSword();
		game_patch::giveMidna();
		game_patch::giveMidnaTransform();

		gameInfo.nextStageVars.nextSpawnPoint = 0x96;
		
		tools::triggerSaveLoad(stage::allStages[Stage_Sacred_Grove], 0x1, 0x96);

		// Call actual skip function
		return tp::evt_control::defaultSkipStb(unk, unk2);
	}
}

namespace mod::game_patch
{
	void skipSewers()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Skipping Sewers");

		// Set sewers flags
		giveMidna();
		giveSense();

		// We should be wolf
		setFirstTimeWolf();

		// Set sewers flags (zelda cutscenes, unchained wolf link, bla)
		gameInfo.scratchPad.eventBits[0x05] |= 0x7A;

		// Load back to Ordon Spring
		tools::triggerSaveLoad(stage::allStages[Stage_Ordon_Spring], 0x1, 0x3, 0x4);
	}

	void setFirstTimeWolf()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set first time wolf");

		gameInfo.scratchPad.wQuestLogData[0x030] |= 1;
	}

	void setHuman()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set human");

		gameInfo.scratchPad.wQuestLogData[0x01E] = 0;
	}

	void setWolf()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set wolf");

		gameInfo.scratchPad.wQuestLogData[0x01E] = 1;
	}

	void giveSense()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Give Senses");

		// Set the Sense (sewers) flag
		gameInfo.scratchPad.eventBits[0x43] |= 0x8;
	}

	void giveEpona()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Give Epona");

		// Set Epona tamed
		gameInfo.scratchPad.eventBits[0x6] |= 0x1;
	}

	void giveMasterSword()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Give MasterSword");

		// Set Master sword inventory flag
		gameInfo.scratchPad.wQuestLogData[0x0D2] |= 0x2;

		// Equip Master sword (0x49 / 73)
		gameInfo.scratchPad.wQuestLogData[0x014] = 0x49;
	}

	void giveMidna()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Give Midna (sewers)");

		// Set Midna sewers flag
		gameInfo.scratchPad.eventBits[0xC] |= 0x10;
	}

	void giveMidnaTransform()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Give Midna Transform");

		// Set Midna Transform flag (gets set at Master Sword)
		gameInfo.scratchPad.eventBits[0xD] |= 0x4;
	}
}