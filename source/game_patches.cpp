#include "game_patches.h"
#include "defines.h"
#include "stage.h"
#include "tools.h"

#include <tp/d_menu_collect.h>
#include <tp/d_a_alink.h>
#include <tp/d_save.h>
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
	void assemblyOverwrites()
	{
		// Default to US/JP
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);

		// Get the addresses to overwrite
		#ifdef TP_US
		enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#elif defined TP_EU
		enableCrashScreen = reinterpret_cast<u32*>(0x8000B878);
		#elif defined TP_JP
		enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#endif

		// Perform the overwrites

		/* If the address is loaded into the cache before the overwrite is made, 
		then the cache will need to be cleared after the overwrite */

		// Enable the crash screen
		*enableCrashScreen = 0x48000014; // b 0x14
	}

	void killLinkHouseSpider()
	{
		// Kill Skulltula at Link's house
		gameInfo.localAreaNodes.unk_0[0xA] |= 0x1;
	}

	void increaseClimbSpeed()
	{
		// Adjust Link's climbing speeds
		tp::d_a_alink::LadderVars* LadderVars = &tp::d_a_alink::ladderVars;
		LadderVars->ladderClimbInitSpeed 			= 1.8;
		LadderVars->ladderReachTopClimbUpSpeed 		= 1.765;
		LadderVars->ladderTopStartClimbDownSpeed 	= 1.8;
		LadderVars->ladderBottomGetOffSpeed 		= 1.8;
		LadderVars->ladderClimbSpeed 				= 1.575;
		LadderVars->wallClimbHorizontalSpeed 		= 2.0;
		LadderVars->wallClimbVerticalSpeed 			= 1.875;
	}

	void removeIBLimit()
	{
		// li 0
		u32 li = 0x38600000;
		// b +4C
		u32 b  = 0x4800004C;

		// CheckHeavyState overwrite
		u32 checkHeavyStateOnAddress = reinterpret_cast<u32>(&tp::d_a_alink::checkHeavyStateOn);
		*reinterpret_cast<u32*>(checkHeavyStateOnAddress + 0x84) = li;

		u32 setStickDataAddress = reinterpret_cast<u32>(&tp::d_a_alink::setStickData);
		*reinterpret_cast<u32*>(setStickDataAddress + 0x5FC) = b;
	}

	void increaseWalletSize()
	{
		// li
		u32 li = 0x38600000;
		u32 cmpwi = 0x2c000000;

		u32 getRupeeMaxAddress = reinterpret_cast<u32>(&tp::d_save::getRupeeMax);
		u32 setWalletMaxNumAddress = reinterpret_cast<u32>(&tp::d_menu_collect::setWalletMaxNum);

		// 300 -> 1000
		*reinterpret_cast<u32*>(getRupeeMaxAddress + 0x30) = li | 0x000003E8;
		
		// 600 -> 5000
		*reinterpret_cast<u32*>(getRupeeMaxAddress + 0x38) = li | 0x00001388;

		// 1000 -> 9999 (> 4 digit not good)
		*reinterpret_cast<u32*>(getRupeeMaxAddress + 0x40) = li | 0x0000270F;

		// 600 -> 5000
		*reinterpret_cast<u32*>(setWalletMaxNumAddress + 0x18) = cmpwi | 0x00001388;

		// 300 -> 1000
		*reinterpret_cast<u32*>(setWalletMaxNumAddress + 0x24) = cmpwi | 0x000003E8;

		// 1000 -> 9999
		*reinterpret_cast<u32*>(setWalletMaxNumAddress + 0x30) = cmpwi | 0x0000270F;
	}

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
	
	void skipMDH()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Skipping MDH");

		// Load back to Ordon Spring
		strncpy(gameInfo.nextStageVars.nextStage,stage::allStages[Stage_Hyrule_Castle_Sewers],sizeof(gameInfo.nextStageVars.nextStage) - 1);
		gameInfo.nextStageVars.nextRoom = 0x3;
		gameInfo.nextStageVars.nextSpawnPoint = 0x0;
	}
	
	void allowFaronEscape()
    {
        strcpy(sysConsolePtr->consoleLine[20].line, "state was not 0");
        if (gameInfo.nextStageVars.nextRoom != 5)
        {        
            if (gameInfo.scratchPad.allAreaNodes.Forest_Temple.dungeon.bossBeaten == 0b1 || gameInfo.scratchPad.allAreaNodes.Snowpeak_Ruins.dungeon.bossBeaten == 0b1 ||
                gameInfo.scratchPad.allAreaNodes.Lakebed_Temple.dungeon.bossBeaten == 0b1 || gameInfo.scratchPad.itemFlags.itemFlags3.Vessel_Of_Light_Faron == 0b0 || 
				(tp::d_com_inf_game::current_state == 0x65 && gameInfo.scratchPad.itemFlags.itemFlags3.Vessel_Of_Light_Faron == 0b0))
            {
                return;
            }
            else
            {
                strcpy(sysConsolePtr->consoleLine[20].line, "-> Allowing Faron Escape");
                // reload faron woods as state 2
                //tools::triggerSaveLoad(gameInfo.nextStageVars.nextStage, gameInfo.nextStageVars.nextRoom, gameInfo.nextStageVars.nextSpawnPoint, a); --obsolete code
                gameInfo.nextStageVars.nextState = 0x2;
            }
        }
    }
	
	void unlockHFGates()
	{
		gameInfo.unk_978[0x7] |= 0x6;//2 = lanyru gate 4 = eldin gorge gate
	}
	
	void skipGoats2()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Skipping Goats 2");
		
		gameInfo.localAreaNodes.unk_0[0xE] |= 0x2;//set flag for Fado text before goats
		gameInfo.localAreaNodes.unk_0[0x9] |= 0x60;//set flag for day 3 intro cs and goats 2 done		

		// Load back to Ordon Spring
		tools::triggerSaveLoad(stage::allStages[Stage_Ordon_Village], 0x0, 0x19, 0x8);
	}

	void setFirstTimeWolf()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set first time wolf");

		gameInfo.scratchPad.unk_0[0x030] |= 1;
	}

	void setHuman()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set human");

		gameInfo.scratchPad.unk_0[0x01E] = 0;
	}

	void setWolf()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "-> Set wolf");

		gameInfo.scratchPad.unk_0[0x01E] = 1;
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
		gameInfo.scratchPad.itemFlags.itemFlags1.Master_Sword = 0b1;

		// Equip Master sword (0x49 / 73)
		gameInfo.scratchPad.unk_0[0x014] = 0x49;
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