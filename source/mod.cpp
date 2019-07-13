#include "defines.h"
#include "mod.h"
#include "global.h"
#include "systemConsole.h"
#include "patch.h"
#include "controller.h"
#include "tools.h"
#include "array.h"
#include "eventListener.h"
#include "stage.h"
#include "game_patches.h"
#include "chestRando.h"

#include <tp/d_map_path_dmap.h>
#include <tp/evt_control.h>
#include <tp/f_op_scene_req.h>
#include <tp/d_com_inf_game.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	Mod* global::modPtr = nullptr;
	ChestRandomizer* global::chestRandoPtr = nullptr;
	event::EventListener* global::eventListenerPtr = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	Mod::Mod()
	{
		eventListener = new event::EventListener();
		chestRandomizer = new ChestRandomizer();
		global::modPtr = this;
		global::chestRandoPtr = this->chestRandomizer;
		global::eventListenerPtr = this->eventListener;
	}

	void Mod::init()
	{
		// Perform any necessary assembly overwrites
		assemblyOverwrites();

		// Init rando seed
		tools::randomSeed = 0x9e3779b97f4a7c15;

		// Set the initial console color
		system_console::setBackgroundColor(0x00A0A0A0);
		system_console::setState(true, 21);

		sprintf(sysConsolePtr->consoleLine[0].line, "AECX' TP Randomizer %s", VERSION);
		strcpy(sysConsolePtr->consoleLine[1].line, "Controls:");
		strcpy(sysConsolePtr->consoleLine[2].line, "  [R + Z] Toggle console");
		strcpy(sysConsolePtr->consoleLine[3].line, "  [L + Z + B] Force New Rando");
		strcpy(sysConsolePtr->consoleLine[4].line, "Auto rando upon new game!");


		//   =================
		//  | Custom events   |
		//   =================

		// Generate random data when a new game starts
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x1, 0x15, 0xFF, 0xFF, tools::triggerRandomGenerator, event::LoadEventAccuracy::Stage_Room_Spawn);
		
		// Skip sewers when the load happens
		eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Castle_Sewers], 0x0, 0x18, 0xFF, 0xFF, game_patch::skipSewers, event::LoadEventAccuracy::Stage_Room_Spawn);

		// Fix BiTE
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x0, 0x17, 0xFF, 0xFF, game_patch::giveEpona, event::LoadEventAccuracy::Stage_Room_Spawn);


		//   =================
		//  | Function Hooks  |
		//   =================
		
		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
			[]()
			{
				global::modPtr->procNewFrame();
			}
		);

		checkTreasureRupeeReturn_trampoline = patch::hookFunction(tp::d_a_alink::checkTreasureRupeeReturn,
			[](void* unused, s32 amount)
			{
				// Disable this function as this check happens before the replacement
				// Might render an item unobtainable if you're having rupees
				return false;
			}
		);

		createItemForTrBox_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				return global::modPtr->procCreateItemForTrBoxDemo(pos, item, unk3, unk4, unk5, unk6);
			}
		);

		evt_control_Skipper_trampoline = patch::hookFunction(tp::evt_control::skipper,
			[](void* evtPtr)
			{
				return global::modPtr->procEvtSkipper(evtPtr);
			}
		);
	}

	void Mod::procNewFrame()
	{
		// Increment seed
		tools::getRandom(0);

		// Print dbg
	/*
		sprintf(sysConsolePtr->consoleLine[10].line, "respawnCs: %p", &gameInfo.respawnCutscene);
		sprintf(sysConsolePtr->consoleLine[11].line, "respawnAn: %p", &gameInfo.respawnAnimation);
		sprintf(sysConsolePtr->consoleLine[12].line, "immediateC: %p", &gameInfo.eventSystem.immediateControl);
		sprintf(sysConsolePtr->consoleLine[13].line, "cEvId: %p   nEvId: %p", &gameInfo.eventSystem.currentEventID, &gameInfo.eventSystem.nextEventID);
		sprintf(sysConsolePtr->consoleLine[14].line, "cstage: %p  nstage: %p", &gameInfo.currentStage, &gameInfo.nextStageVars.nextStage);
	*/


		// If loading has started check for LoadEvents
		if(isLoading)
		{
			eventListener->checkLoadEvents();
		}

		// Runs once each frame
		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Z)))
		{
			// Toggle console
			system_console::setState(!sysConsolePtr->consoleEnabled, 0);
		}

		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_L | controller::PadInputs::Button_Z | controller::PadInputs::Button_B)))
		{
			chestRandomizer->generate();
		}

		// Call original function
		fapGm_Execute_trampoline();
	}

	s32 Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Runs once when Link picks up an item with text and is holding it towards the camera

		item = chestRandomizer->getItemReplacement(pos, item);

		// Call original function
		return createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}
	
	s32 Mod::procEvtSkipper(void* evtPtr)
	{
		// TODO: Make this a dynamic function that can be manipulated outside (struct csTrigger[])
		// Runs when the user tries to skip a Cutscene		
		if(0 == strcmp(gameInfo.currentStage, stage::allStages[Stage_Sacred_Grove]))
		{
			// We're at sacred grove
			if(0x2 == gameInfo.eventSystem.currentEventID)
			{
				// Master Sword cutscene
				//game_patch::masterSwordCutscene();
				tools::setCutscene(true, true, cutscene_skip::onMasterSwordSkip);
			}
		}
		// Call original function
		return evt_control_Skipper_trampoline(evtPtr);
	}

	void Mod::assemblyOverwrites()
	{
		// Get the addresses to overwrite
		#ifdef TP_US
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#elif defined TP_EU
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B878);
		#elif defined TP_JP
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#endif

		// Perform the overwrites

		/* If the address is loaded into the cache before the overwrite is made, 
		then the cache will need to be cleared after the overwrite */

		// Enable the crash screen
		*enableCrashScreen = 0x48000014; // b 0x14
	}
}