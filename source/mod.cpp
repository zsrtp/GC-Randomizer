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
#include "itemChecks.h"
#include "HUDConsole.h"
#include "items.h"
#include "singleton.h"

#include <tp/f_op_scene_req.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/d_map_path_dmap.h>
#include <tp/evt_control.h>
#include <tp/f_op_scene_req.h>
#include <tp/d_com_inf_game.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/d_save.h>
#include <tp/JFWSystem.h>
#include <tp/d_kankyo.h>
#include <tp/d_msg_flow.h>
#include <tp/DynamicLink.h>
#include <tp/d_item.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	Mod* global::modPtr = nullptr;
	ChestRandomizer* global::chestRandoPtr = nullptr;
	event::EventListener* global::eventListenerPtr = nullptr;
	mod::HUDConsole* global::hudConsolePtr = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	Mod::Mod()
	{
		eventListener = new event::EventListener();
		chestRandomizer = new ChestRandomizer();
		hudConsole = new HUDConsole("Settings", 0x80);
		global::modPtr = this;
		global::chestRandoPtr = this->chestRandomizer;
		global::eventListenerPtr = this->eventListener;
		global::hudConsolePtr = this->hudConsole;
	}

	void Mod::init()
	{
		// Perform any necessary assembly overwrites
		game_patch::assemblyOverwrites();
		game_patch::increaseWalletSize();
		game_patch::increaseClimbSpeed();
		/*
		 * Causes issues right now (argarok cannot be beaten)
		 * game_patch::removeIBLimit();
		 */

		// Init rando
		tools::randomSeed = 0x9e3779b97f4a7c15;
		randoEnabled = 1;
		truePause = 1;
		inputBuffering = 0;

		// Print controls
		strcpy(sysConsolePtr->consoleLine[20].line, "D-Pad Nav: Up/Down  Left/Right");
		strcpy(sysConsolePtr->consoleLine[21].line, "+/- Value: A/B/X/Y  Console  :  R + Z");
		strcpy(sysConsolePtr->consoleLine[22].line, " Generate: R + Start (auto on new file)");
		strcpy(sysConsolePtr->consoleLine[23].line, "Bring up the console to use commands");
		strcpy(sysConsolePtr->consoleLine[24].line, "https://rando.tpspeed.run | Twitter: @theAECX");

		u8 page = 0;

		// General settings
		hudConsole->addOption(page, "Red:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[0], 0xFF);
		hudConsole->addOption(page, "Green:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[1], 0xFF);
		hudConsole->addOption(page, "Blue:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[2], 0xFF);
		hudConsole->addOption(page, "Alpha:", &reinterpret_cast<u8*>(&sysConsolePtr->consoleColor)[3], 0xFF);
		hudConsole->addOption(page, "True Pause:", &truePause, 0x1);
		hudConsole->addOption(page, "Input Buffer:", &inputBuffering, 0x1);

		// Seed settings
		page = hudConsole->addPage("Seed");

		hudConsole->addOption(page, "Rando enabled?", &randoEnabled, 0x1);
		hudConsole->addOption(page, "Custom Seed?", &customSeed, 0x1);
		hudConsole->addOption(page, "Seed 1:", &reinterpret_cast<u8*>(&tools::randomSeed)[0], 0xFF);
		hudConsole->addOption(page, "Seed 2:", &reinterpret_cast<u8*>(&tools::randomSeed)[1], 0xFF);
		hudConsole->addOption(page, "Seed 3:", &reinterpret_cast<u8*>(&tools::randomSeed)[2], 0xFF);
		hudConsole->addOption(page, "Seed 4:", &reinterpret_cast<u8*>(&tools::randomSeed)[3], 0xFF);

		hudConsole->addOption(page, "Seed 5:", &reinterpret_cast<u8*>(&tools::randomSeed)[4], 0xFF);
		hudConsole->addOption(page, "Seed 6:", &reinterpret_cast<u8*>(&tools::randomSeed)[5], 0xFF);
		hudConsole->addOption(page, "Seed 7:", &reinterpret_cast<u8*>(&tools::randomSeed)[6], 0xFF);
		hudConsole->addOption(page, "Seed 8:", &reinterpret_cast<u8*>(&tools::randomSeed)[7], 0xFF);

		hudConsole->addWatch(page, "Resulting Seed", &tools::randomSeed, 'x', WatchInterpretation::_u64);

		// Debug
		page = hudConsole->addPage("Debug Info");
		
		hudConsole->addOption(page, "Progressive Items?", &chestRandomizer->isProgressiveEnabled, 0x1);
		hudConsole->addOption(page, "Bugsanity?", &chestRandomizer->isBugsanityEnabled, 0x1);
		hudConsole->addOption(page, "Poesanity?", &chestRandomizer->isPoesanityEnabled, 0x1);
		hudConsole->addOption(page, "Shopsanity?", &chestRandomizer->isShopsanityEnabled, 0x1);	
		hudConsole->addOption(page, "Dungeon Items?", &chestRandomizer->areDungeonItemsRandomized, 0x1);	
		
		
		hudConsole->addWatch(page, "Function:", &lastItemFunc, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "  Source:", &chestRandomizer->lastSourceInfo, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "    Dest:", &chestRandomizer->lastDestInfo, 's', WatchInterpretation::_str);

		hudConsole->addWatch(page, "Total Checks:", &chestRandomizer->totalChecks, 'u', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "Layer Checks:", &chestRandomizer->layerCheckCount, 'u', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "Empty:", &chestRandomizer->empty, 'u', WatchInterpretation::_u16);

		hudConsole->addWatch(page, "Active Seed:", &chestRandomizer->currentSeed, 'x', WatchInterpretation::_u64);
		hudConsole->addWatch(page, "   Checksum:", &chestRandomizer->checkSum, 'x', WatchInterpretation::_u16);


		// Item search
		page = hudConsole->addPage("Item Search");

		hudConsole->addOption(page, "Search ID:", &itemSearchID, 0xFF);
		hudConsole->addOption(page, "Reverse ID:", &itemReverseSearchID, 0xFF);

		hudConsole->addWatch(page, "1. Result:", &itemSearchResults, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "1. Reverse:", &itemReverseSearchResults, 's', WatchInterpretation::_str);

		hudConsole->addWatch(page, "MetadataID:", &lastItemDataID, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataX:", &lastItemDataX, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataY:", &lastItemDataY, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "MetadataZ:", &lastItemDataZ, 's', WatchInterpretation::_str);
		
		// Game info
		page = hudConsole->addPage("Game Info");
		
		hudConsole->addOption(page, "MDH skip?", &Singleton::getInstance()->isMDHSkipEnabled, 0x1);
		hudConsole->addOption(page, "Faron Escape?", &Singleton::getInstance()->isForestEscapeEnabled, 0x1);
		hudConsole->addOption(page, "open HF gates?", &Singleton::getInstance()->isGateUnlockEnabled, 0x1);
		hudConsole->addOption(page, "skip goats1&2?", &Singleton::getInstance()->isGoatSkipEnabled, 0x1);
		hudConsole->addOption(page, "skip MS puzzle?", &Singleton::getInstance()->isMSPuzzleSkipEnabled, 0x1);
		
		hudConsole->addWatch(page, "CurrentStage:", &gameInfo.currentStage, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentRoom:", &tp::d_kankyo::env_light.currentRoom, 'd', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "CurrentState:", &tp::d_com_inf_game::current_state, 'x', WatchInterpretation::_u8);
		
		hudConsole->addWatch(page, "CurrentPosX:", &currentPosX, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosY:", &currentPosY, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosZ:", &currentPosZ, 's', WatchInterpretation::_str);	
		hudConsole->addWatch(page, "Sky Angle:", &skyAngle, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "Form:", &gameInfo.scratchPad.form, 'x', WatchInterpretation::_u8);
		
		
		//event info
		page = hudConsole->addPage("Event Info");
		
		/*hudConsole->addOption(page, "Item half milk", &chestRandomizer->itemThatReplacesHalfMilk, 0xFF); //for testing only
		hudConsole->addOption(page, "Item slingshot", &chestRandomizer->itemThatReplacesSlingShot, 0xFF); //for testing only
		hudConsole->addOption(page, "Normal Time:", &enableNormalTime, 0x1); //for testing only
		hudConsole->addOption(page, "Set Day:", &setDay, 0x1); //for testing only
		hudConsole->addOption(page, "Unused Slot:", &gameInfo.scratchPad.itemWheel.Item_Slot, 0xFF); //for testing only
		hudConsole->addOption(page, "Form:", &gameInfo.scratchPad.form, 0x1); //for testing only*/
		
		
				
		hudConsole->addWatch(page, "CurrentEventID:", &gameInfo.eventSystem.currentEventID, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextEventID:", &gameInfo.eventSystem.nextEventID, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextStage:", &gameInfo.nextStageVars.nextStage, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "NextRoom:", &gameInfo.nextStageVars.nextRoom, 'd', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextSpawnPoint:", &gameInfo.nextStageVars.nextSpawnPoint, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextSate:", &gameInfo.nextStageVars.nextState, 'x', WatchInterpretation::_u8);
		
		
		//color
		/*page = hudConsole->addPage("Tunic Color");
		
		hudConsole->addOption(page, "Top toggle:", &topToggle, 0x1);
		hudConsole->addOption(page, "Red top:", &redTop, 0xFF);
		hudConsole->addOption(page, "Green top:", &greenTop, 0xFF);
		hudConsole->addOption(page, "Blue top:", &blueTop, 0xFF);
		hudConsole->addOption(page, "Bottom toggle:", &bottomToggle, 0x1);
		hudConsole->addOption(page, "Red bottom:", &redBottom, 0xFF);
		hudConsole->addOption(page, "Green bottom:", &greenBottom, 0xFF);
		hudConsole->addOption(page, "Blue bottom:", &blueBottom, 0xFF);*/
		
		//buttons
		/*page = hudConsole->addPage("Button texts");
		
		hudConsole->addOption(page, "A button:", &gameInfo.aButtonText, 0xFF);
		hudConsole->addOption(page, "Y button:", &gameInfo.yButtonText, 0xFF);
		hudConsole->addOption(page, "X button:", &gameInfo.xButtonText, 0xFF);
		hudConsole->addOption(page, "B button:", &gameInfo.bButtonText, 0xFF);
		hudConsole->addOption(page, "R button:", &gameInfo.rButtonText, 0xFF);
		hudConsole->addOption(page, "stick button:", &gameInfo.stickButtonText, 0xFF);
		hudConsole->addOption(page, "start button:", &gameInfo.startButtonText, 0xFF);
		hudConsole->addOption(page, "C button:", &gameInfo.cButtonText, 0xFF);
		hudConsole->addWatch(page, "A button:", &gameInfo.aButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "Y button:", &gameInfo.yButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "X button:", &gameInfo.xButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "B button:", &gameInfo.bButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "R button:", &gameInfo.rButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "stick:", &gameInfo.stickButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "start button:", &gameInfo.startButtonText, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "C button:", &gameInfo.cButtonText, 'x', WatchInterpretation::_u8);*/
		/*hudConsole->addWatch(page, "scoop:", &scoopResult, 'x', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "check:", &checkResult, 'x', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "items:", &itemsResult, 'x', WatchInterpretation::_u16);
		hudConsole->addWatch(page, "map:", &mapResult, 'x', WatchInterpretation::_u16);*/

		//local area
		/*page = hudConsole->addPage("Local Area 1");
		hudConsole->addOption(page, "AreaNodes0:", &gameInfo.localAreaNodes.unk_0[0x0], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes1:", &gameInfo.localAreaNodes.unk_0[0x1], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes2:", &gameInfo.localAreaNodes.unk_0[0x2], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes3:", &gameInfo.localAreaNodes.unk_0[0x3], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes4:", &gameInfo.localAreaNodes.unk_0[0x4], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes5:", &gameInfo.localAreaNodes.unk_0[0x5], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes6:", &gameInfo.localAreaNodes.unk_0[0x6], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes7:", &gameInfo.localAreaNodes.unk_0[0x7], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes8:", &gameInfo.localAreaNodes.unk_0[0x8], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes9:", &gameInfo.localAreaNodes.unk_0[0x9], 0xFF); //for testing only		
		hudConsole->addWatch(page, "AreaNodes0:", &gameInfo.localAreaNodes.unk_0[0x0], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes1:", &gameInfo.localAreaNodes.unk_0[0x1], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes2:", &gameInfo.localAreaNodes.unk_0[0x2], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes3:", &gameInfo.localAreaNodes.unk_0[0x3], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes4:", &gameInfo.localAreaNodes.unk_0[0x4], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes5:", &gameInfo.localAreaNodes.unk_0[0x5], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes6:", &gameInfo.localAreaNodes.unk_0[0x6], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes7:", &gameInfo.localAreaNodes.unk_0[0x7], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes8:", &gameInfo.localAreaNodes.unk_0[0x8], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes9:", &gameInfo.localAreaNodes.unk_0[0x9], 'x', WatchInterpretation::_u8);
		page = hudConsole->addPage("Local Area 2");
		hudConsole->addOption(page, "AreaNodesA:", &gameInfo.localAreaNodes.unk_0[0xA], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodesB:", &gameInfo.localAreaNodes.unk_0[0xB], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodesC:", &gameInfo.localAreaNodes.unk_0[0xC], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodesD:", &gameInfo.localAreaNodes.unk_0[0xD], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodesE:", &gameInfo.localAreaNodes.unk_0[0xE], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodesF:", &gameInfo.localAreaNodes.unk_0[0xF], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes10:", &gameInfo.localAreaNodes.unk_0[0x10], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes11:", &gameInfo.localAreaNodes.unk_0[0x11], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes12:", &gameInfo.localAreaNodes.unk_0[0x12], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes13:", &gameInfo.localAreaNodes.unk_0[0x13], 0xFF); //for testing only	
		hudConsole->addWatch(page, "AreaNodesA:", &gameInfo.localAreaNodes.unk_0[0xA], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodesB:", &gameInfo.localAreaNodes.unk_0[0xB], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodesC:", &gameInfo.localAreaNodes.unk_0[0xC], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodesD:", &gameInfo.localAreaNodes.unk_0[0XD], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodesE:", &gameInfo.localAreaNodes.unk_0[0xE], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodesF:", &gameInfo.localAreaNodes.unk_0[0xF], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes10:", &gameInfo.localAreaNodes.unk_0[0x10], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes11:", &gameInfo.localAreaNodes.unk_0[0x11], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes12:", &gameInfo.localAreaNodes.unk_0[0x12], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes13:", &gameInfo.localAreaNodes.unk_0[0x13], 'x', WatchInterpretation::_u8);
		page = hudConsole->addPage("Local Area 3");
		hudConsole->addOption(page, "AreaNodes14:", &gameInfo.localAreaNodes.unk_0[0x14], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes15:", &gameInfo.localAreaNodes.unk_0[0x15], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes16:", &gameInfo.localAreaNodes.unk_0[0x16], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes17:", &gameInfo.localAreaNodes.unk_0[0x17], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes18:", &gameInfo.localAreaNodes.unk_0[0x18], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes19:", &gameInfo.localAreaNodes.unk_0[0x19], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes1A:", &gameInfo.localAreaNodes.unk_0[0x1A], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes1B:", &gameInfo.localAreaNodes.unk_0[0x1B], 0xFF); //for testing only
		hudConsole->addOption(page, "NbKeys:", &gameInfo.localAreaNodes.nbKeys, 0xFF); //for testing only
		hudConsole->addWatch(page, "AreaNodes14:", &gameInfo.localAreaNodes.unk_0[20], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes15:", &gameInfo.localAreaNodes.unk_0[21], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes16:", &gameInfo.localAreaNodes.unk_0[22], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes17:", &gameInfo.localAreaNodes.unk_0[23], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes18:", &gameInfo.localAreaNodes.unk_0[24], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes19:", &gameInfo.localAreaNodes.unk_0[25], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes1A:", &gameInfo.localAreaNodes.unk_0[26], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes1B:", &gameInfo.localAreaNodes.unk_0[27], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NbKeys:", &gameInfo.localAreaNodes.nbKeys, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "Dungeon flags:", &gameInfo.localAreaNodes.dungeon, 'x', WatchInterpretation::_u8);
		page = hudConsole->addPage("Local Area 4");
		hudConsole->addOption(page, "AreaNodes1E:", &gameInfo.localAreaNodes.unk_1E[0x0], 0xFF); //for testing only
		hudConsole->addOption(page, "AreaNodes1F:", &gameInfo.localAreaNodes.unk_1E[0x1], 0xFF); //for testing only
		hudConsole->addWatch(page, "AreaNodes1E:", &gameInfo.localAreaNodes.unk_1E[0x0], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "AreaNodes1F:", &gameInfo.localAreaNodes.unk_1E[0x1], 'x', WatchInterpretation::_u8);

		page = hudConsole->addPage("dungeon flags");

		hudConsole->addOption(page, "Dungeon flag8:", &dungeonFlagsView1, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag7:", &dungeonFlagsView2, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag6:", &dungeonFlagsView3, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag5:", &dungeonFlagsView4, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag4:", &dungeonFlagsView5, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag3:", &dungeonFlagsView6, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag2:", &dungeonFlagsView7, 0x1); //for testing only
		hudConsole->addOption(page, "Dungeon flag1:", &dungeonFlagsView8, 0x1); //for testing only
		
		hudConsole->addOption(page, "edit flags:", &dungeonFlagsViewEdit, 0x1); //for testing only
		
		
		hudConsole->addWatch(page, "Dungeon flags:", &gameInfo.localAreaNodes.dungeon, 'x', WatchInterpretation::_u8);*/



		//item slots
		/*page = hudConsole->addPage("Item slots 1");
		
		hudConsole->addWatch(page, "slot0:", &gameInfo.scratchPad.itemSlotsOrder[0x0], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot1:", &gameInfo.scratchPad.itemSlotsOrder[0x1], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot2:", &gameInfo.scratchPad.itemSlotsOrder[0x2], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot3:", &gameInfo.scratchPad.itemSlotsOrder[0x3], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot4:", &gameInfo.scratchPad.itemSlotsOrder[0x4], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot5:", &gameInfo.scratchPad.itemSlotsOrder[0x5], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot6:", &gameInfo.scratchPad.itemSlotsOrder[0x6], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot7:", &gameInfo.scratchPad.itemSlotsOrder[0x7], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot8:", &gameInfo.scratchPad.itemSlotsOrder[0x8], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot9:", &gameInfo.scratchPad.itemSlotsOrder[0x9], 'x', WatchInterpretation::_u8);
		
		page = hudConsole->addPage("Item slots 2");
		
		hudConsole->addWatch(page, "slotA:", &gameInfo.scratchPad.itemSlotsOrder[0xA], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slotB:", &gameInfo.scratchPad.itemSlotsOrder[0xB], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slotC:", &gameInfo.scratchPad.itemSlotsOrder[0xC], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slotD:", &gameInfo.scratchPad.itemSlotsOrder[0xD], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slotE:", &gameInfo.scratchPad.itemSlotsOrder[0xE], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slotF:", &gameInfo.scratchPad.itemSlotsOrder[0xF], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot10:", &gameInfo.scratchPad.itemSlotsOrder[0x10], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot11:", &gameInfo.scratchPad.itemSlotsOrder[0x11], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot12:", &gameInfo.scratchPad.itemSlotsOrder[0x12], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot13:", &gameInfo.scratchPad.itemSlotsOrder[0x13], 'x', WatchInterpretation::_u8);
		
		page = hudConsole->addPage("Item slots 2");
		
		hudConsole->addWatch(page, "slot14:", &gameInfo.scratchPad.itemSlotsOrder[0x14], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot15:", &gameInfo.scratchPad.itemSlotsOrder[0x15], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot16:", &gameInfo.scratchPad.itemSlotsOrder[0x16], 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "slot17:", &gameInfo.scratchPad.itemSlotsOrder[0x17], 'x', WatchInterpretation::_u8);*/
		
		
		// save load
		/*page = hudConsole->addPage("Save load");
		
		hudConsole->addOption(page, "stage:", &stage, 78); //for testing only
		hudConsole->addOption(page, "room:", &room, 60); //for testing only
		hudConsole->addOption(page, "spawn:", &spawn, 0xFF); //for testing only
		hudConsole->addOption(page, "state:", &state, 0xFF); //for testing only
		hudConsole->addOption(page, "trigger:", &trigerLoadSave, 0x1); //for testing only*/
		
		// Print
		hudConsole->draw();
		system_console::setState(true, 0);

		//   =================
		//  | Custom events   |
		//   =================

		// Generate random data when a new game starts
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x1, 0x15, 0xFF, 0xFF, tools::triggerRandomGenerator, event::LoadEventAccuracy::Stage_Room_Spawn);
		
		// Skip sewers when the load happens
		eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Castle_Sewers], 0x0, 0x18, 0xFF, 0xFF, game_patch::skipSewers, event::LoadEventAccuracy::Stage_Room_Spawn);

		// Fix BiTE
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0x0, 0x17, 0xFF, 0xFF, game_patch::giveEpona, event::LoadEventAccuracy::Stage_Room_Spawn);

		// Kill spider at Link's house
		eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Village], 0x1, 0xFF, 0xFF, 0xFF, game_patch::killLinkHouseSpider, event::LoadEventAccuracy::Stage_Room);

		// Skip MDH when the load happens
		eventListener->addLoadEvent(stage::allStages[Stage_Castle_Town_Interiors], 0x6, 0xC, 0xFF, 0xFF, game_patch::skipMDH, event::LoadEventAccuracy::Stage_Room_Spawn);
		
		// Allow Faron Escape
		eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::allowFaronEscape, event::LoadEventAccuracy::Stage);
		
		//unlock HF gates
		eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Field], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::unlockHFGates, event::LoadEventAccuracy::Stage);	

		//skip goats 1 & 2
		eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Ranch], 0x0, 0x3, 0xFF, 0xFF, game_patch::skipGoats, event::LoadEventAccuracy::Stage_Room_Spawn);	

		//skip MS Puzzle
		eventListener->addLoadEvent(stage::allStages[Stage_Sacred_Grove], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::skipGrovePuzzle, event::LoadEventAccuracy::Stage);


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
			[](void* unk1, s32 item)
			{
				return global::modPtr->procCheckTreasureRupeeReturn(unk1, item);
			}
		);

		createItemForPresentDemo_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForPresentDemo,
			[](const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3])
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForPresentDemo");

				return global::modPtr->createItemForPresentDemo_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);
			

		createItemForTrBoxDemo_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForTrBoxDemo");

				return global::modPtr->createItemForTrBoxDemo_trampoline(pos, item, unk3, unk4, unk5, unk6);
			}
		);
		//this function is called when the heart spawns, not when link gets it		
		//createItemForTrBoxDemo is called when heart container is gotten
		createItemForBoss_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForBoss,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7, s32 unk8)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForBoss");

				return global::modPtr->createItemForBoss_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7, unk8);
			}
		);

		createItemForMidBoss_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForMidBoss,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForMidBoss");

				return global::modPtr->createItemForMidBoss_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);

		createItemForDirectGet_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForDirectGet,
			[](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7)
			{
				// Call replacement function
				item = global::modPtr->procItemCreateFunc(pos, item, "createItemForDirectGet");

				return global::modPtr->createItemForDirectGet_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
			}
		);

		evt_control_Skipper_trampoline = patch::hookFunction(tp::evt_control::skipper,
			[](void* evtPtr)
			{
				return global::modPtr->procEvtSkipper(evtPtr);
			}
		);

		query022_trampoline = patch::hookFunction(tp::d_msg_flow::query022,
			[](void* unk1, void* unk2, s32 unk3)
			{
				return global::modPtr->proc_query022(unk1, unk2, unk3);
			}
		);

		do_link_trampoline = patch::hookFunction(tp::dynamic_link::do_link,
			[](tp::dynamic_link::DynamicModuleControl* dmc)
			{
				return global::modPtr->procDoLink(dmc);
			}
		);

		item_func_UTUWA_HEART_trampoline = patch::hookFunction(tp::d_item::item_func_UTUWA_HEART,
			[]()
			{
				return global::modPtr->procItem_func_UTUWA_HEART();
			}
		);
	}

	void Mod::procNewFrame()
	{
		float linkPos[3];
		getPlayerPos(linkPos);

		snprintf(currentPosX, 30, "%f", linkPos[0]);
		snprintf(currentPosY, 30, "%f", linkPos[1]);
		snprintf(currentPosZ, 30, "%f", linkPos[2]);
		snprintf(skyAngle, 30, "%f", gameInfo.scratchPad.skyAngle);
		
		
		if (gameInfo.ColorPtr != nullptr)
		{
			if (topToggle == 0)
			{
				gameInfo.ColorPtr->redTopToggle = 0x4;
				gameInfo.ColorPtr->greenTopToggle = 0x4;
				gameInfo.ColorPtr->blueTopToggle = 0x4;
			}
			else
			{
				gameInfo.ColorPtr->redTopToggle = 0x0;
				gameInfo.ColorPtr->greenTopToggle = 0x0;
				gameInfo.ColorPtr->blueTopToggle = 0x0;
			}
			gameInfo.ColorPtr->redTop = redTop;
			gameInfo.ColorPtr->greenTop = greenTop;
			gameInfo.ColorPtr->blueTop = blueTop;
			if (bottomToggle == 0)
			{
				gameInfo.ColorPtr->redBottomToggle = 0x4;
				gameInfo.ColorPtr->greenBottomToggle = 0x4;
				gameInfo.ColorPtr->blueBottomToggle = 0x4;
			}
			else
			{
				gameInfo.ColorPtr->redBottomToggle = 0x0;
				gameInfo.ColorPtr->greenBottomToggle = 0x0;
				gameInfo.ColorPtr->blueBottomToggle = 0x0;
			}
			gameInfo.ColorPtr->redBottom = redBottom;
			gameInfo.ColorPtr->greenBottom = greenBottom;
			gameInfo.ColorPtr->blueBottom = blueBottom;
		}
		scoopResult = 0;
		checkResult	= 0;
		itemsResult = 0;
		mapResult = 0;
		/*for (u16 i = 0x0; i < 0xD3; i++)
		{
			if (gameInfo.unk_5de4[i] == 0x6C && scoopResult == 0)
			{
				scoopResult = i;
			}
			if (gameInfo.unk_5de4[i] == 0x4D && skipResult == 0)
			{
				skipResult = i;
			}
			if (gameInfo.unk_5de4[i] == 0x72 && actionResult == 0)
			{
				actionResult = i;
			}
			if (gameInfo.unk_5de4[i] == 0x60 && itemsResult == 0)
			{
				itemsResult = i;
			}
			if (gameInfo.unk_5de4[i] == 0x5F && mapResult == 0)
			{
				mapResult = i;
			}
			if ((gameInfo.unk_5de4[i] == 0x29 || gameInfo.unk_5de4[i] == 0x3D || gameInfo.unk_5de4[i] == 0x69) && zoomResult == 0)
			{
				zoomResult = i;
			}
			if ((gameInfo.unk_5de4[i] == 0x5E || gameInfo.unk_5de4[i] == 0x8 || gameInfo.unk_5de4[i] == 0x80) && checkResult == 0)
			{
				checkResult = i;
			}			
		}*/
		
		if (trigerLoadSave == 1){
			trigerLoadSave = 0;
			tools::triggerSaveLoad(stage::allStages[stage], room, spawn, state);
		}
			
		
		if (!tools::checkItemFlag(ItemFlags::Orange_Rupee))
		{//remove the item get animations for floor pickups (except silver rupee)
			u32 loopAmount = sizeof(item::itemGetAnimationFlags) / sizeof(item::itemGetAnimationFlags[0]);
			for (u32 i = 0; i < loopAmount; i++)
			{
				tools::setItemFlag(static_cast<ItemFlags>(item::itemGetAnimationFlags[i]));
			}
		}
		
		if (enableNormalTime == 0 && setDay == 0)
		{//set night
			gameInfo.scratchPad.skyAngle = 0;
		}
		else if (enableNormalTime == 0 && setDay == 1)
		{//set day
			gameInfo.scratchPad.skyAngle = 180;
		}
		// Increment seed
		if(!customSeed)
		{
			tools::getRandom(0);
		}
		
		// If loading has started check for LoadEvents
		if(isLoading)
		{
			eventListener->checkLoadEvents();
		}

		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Z)))
		{
			// Toggle console			
			system_console::setState(!sysConsolePtr->consoleEnabled, 0);
		}
		else if(gameInfo.rButtonText == 0 && tools::checkItemFlag(ItemFlags::Master_Sword) && controller::checkForButtonInputSingleFrame(controller::PadInputs::Button_R))
		{				
			if (gameInfo.scratchPad.form == 0)
			{// transform into wolf
				gameInfo.scratchPad.form = 1;
			}
			else
			{// transform into human
				gameInfo.scratchPad.form = 0;
			}
		}

		if(sysConsolePtr->consoleEnabled)
		{
			if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Start)))
			{
				chestRandomizer->generate();
			}

			// Parse inputs of this frame
			switch(tp::m_do_controller_pad::cpadInfo.buttonInputTrg)
			{
				case controller::PadInputs::Button_A:
					hudConsole->performAction(ConsoleActions::Option_Increase);
				break;

				case controller::PadInputs::Button_X:
					hudConsole->performAction(ConsoleActions::Option_Increase, 0x10);
				break;

				case controller::PadInputs::Button_B:
					hudConsole->performAction(ConsoleActions::Option_Decrease);
				break;

				case controller::PadInputs::Button_Y:
					hudConsole->performAction(ConsoleActions::Option_Decrease, 0x10);
				break;

				case controller::PadInputs::Button_DPad_Up:
					hudConsole->performAction(ConsoleActions::Move_Up);
				break;

				case controller::PadInputs::Button_DPad_Down:
					hudConsole->performAction(ConsoleActions::Move_Down);
				break;

				case controller::PadInputs::Button_DPad_Left:
					hudConsole->performAction(ConsoleActions::Move_Left);
				break;

				case controller::PadInputs::Button_DPad_Right:
					hudConsole->performAction(ConsoleActions::Move_Right);
				break;
			}
			hudConsole->draw();
		}

		if(truePause && sysConsolePtr->consoleEnabled)
		{
			// Inputs handled, don't pass onto the game
			tp::f_op_scene_req::freezeActors = 1;
			tp::m_do_controller_pad::cpadInfo.buttonInputTrg = 0;
			tp::m_do_controller_pad::cpadInfo.buttonInput = 0;
		}
		else
		{
			tp::f_op_scene_req::freezeActors = 0;
		}

		if(itemSearchID != lastItemSearchID)
		{
			lastItemSearchID = itemSearchID;

			strcpy(itemSearchResults, "404");

			for(u16 i = 0; i < chestRandomizer->totalChecks; i++)
			{
				item::ItemCheck* check = &item::checks[i];
				if (check->destination)
				{
					if(check->destination->itemID == itemSearchID)
					{
						// Found the source
						snprintf(itemSearchResults, 40, "ID: %x Stage: %s Room: %d", check->itemID, check->stage, check->room);
					}
				}
			}
		}
		else if(itemReverseSearchID != lastItemReverseSearchID)
		{
			lastItemReverseSearchID = itemReverseSearchID;

			strcpy(itemReverseSearchResults, "404");

			for(u16 i = 0; i < chestRandomizer->totalChecks; i++)
			{
				item::ItemCheck* check = &item::checks[i];
				if (check->source)
				{
					if(check->source->itemID == itemReverseSearchID)
					{
						// Found the source
						snprintf(itemReverseSearchResults, 40, "ID: %x Stage: %s Room: %d", check->itemID, check->stage, check->room);
					}
				}
			}
		}

		if(inputBuffering)
		{
			tp::m_do_controller_pad::cpadInfo.buttonInputTrg = tp::m_do_controller_pad::cpadInfo.buttonInput;
		}
		
		if (dungeonFlagsViewEdit == 0)
		{
			if (gameInfo.localAreaNodes.dungeon.miniBossBeaten == 0b0)
			{
				dungeonFlagsView1 = 0;
			}
			else
			{
				dungeonFlagsView1 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.ooccooGotten == 0b0)
			{
				dungeonFlagsView2 = 0;
			}
			else
			{
				dungeonFlagsView2 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.bitSix == 0b0)
			{
				dungeonFlagsView3 = 0;
			}
			else
			{
				dungeonFlagsView3 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.containerGotten == 0b0)
			{
				dungeonFlagsView4 = 0;
			}
			else
			{
				dungeonFlagsView4 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.bossBeaten == 0b0)
			{
				dungeonFlagsView5 = 0;
			}
			else
			{
				dungeonFlagsView5 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.bigKeyGotten == 0b0)
			{
				dungeonFlagsView6 = 0;
			}
			else
			{
				dungeonFlagsView6 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.compassGotten == 0b0)
			{
				dungeonFlagsView7 = 0;
			}
			else
			{
				dungeonFlagsView7 = 1;
			}
			if (gameInfo.localAreaNodes.dungeon.mapGotten == 0b0)
			{
				dungeonFlagsView8 = 0;
			}
			else
			{
				dungeonFlagsView8 = 1;
			}
		}
		else if (dungeonFlagsViewEdit == 1)
		{
			if (dungeonFlagsView1 == 0)
			{
				gameInfo.localAreaNodes.dungeon.miniBossBeaten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.miniBossBeaten = 0b1;
			}
			if (dungeonFlagsView2 == 0)
			{
				gameInfo.localAreaNodes.dungeon.ooccooGotten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.ooccooGotten = 0b1;
			}
			if (dungeonFlagsView3 == 0)
			{
				gameInfo.localAreaNodes.dungeon.bitSix = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.bitSix = 0b1;
			}
			if (dungeonFlagsView4 == 0)
			{
				gameInfo.localAreaNodes.dungeon.containerGotten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.containerGotten = 0b1;
			}
			if (dungeonFlagsView5 == 0)
			{
				gameInfo.localAreaNodes.dungeon.bossBeaten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.bossBeaten = 0b1;
			}
			if (dungeonFlagsView6 == 0)
			{
				gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b1;
			}
			if (dungeonFlagsView7 == 0)
			{
				gameInfo.localAreaNodes.dungeon.compassGotten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.compassGotten = 0b1;
			}
			if (dungeonFlagsView8 == 0)
			{
				gameInfo.localAreaNodes.dungeon.mapGotten = 0b0;
			}
			else
			{
				gameInfo.localAreaNodes.dungeon.mapGotten = 0b1;
			}
		}
		
		giveAllScents();
		
		giveAllStoryItems();
		
		reorderItemWheel();
		
		// Call original function
		fapGm_Execute_trampoline();
	}

	s32 Mod::procItemCreateFunc(const float pos[3], s32 item, const char funcIdentifier[32])
	{
		strcpy(lastItemFunc, funcIdentifier);
		snprintf(lastItemDataID, 5, "0x%02x", item);
		snprintf(lastItemDataX, 30, "%f", pos[0]);
		snprintf(lastItemDataY, 30, "%f", pos[1]);
		snprintf(lastItemDataZ, 30, "%f", pos[2]);
		// Runs once when Link picks up an item with text and is holding it towards the camera
		if(randoEnabled && strcmp(funcIdentifier, "createItemForDirectGet") != 0 && strcmp(funcIdentifier, "createItemForBoss") != 0 && strcmp(funcIdentifier, "createItemForMidBoss") != 0)
		{
			item = chestRandomizer->getItemReplacement(pos, item);
		}

		return item;
	}
	
	bool Mod::procCheckTreasureRupeeReturn(void* unk1, s32 item)
	{
		// Allow to open
		return false;
	}

	s32 Mod::procEvtSkipper(void* evtPtr)
	{
		// Runs when the user tries to skip a Cutscene		
		if(tp::d_a_alink::checkStageName(stage::allStages[Stage_Sacred_Grove]))
		{
			// We're at sacred grove
			if(0x2 == gameInfo.eventSystem.currentEventID)
			{
				// Master Sword cutscene
				tools::setCutscene(true, false, cutscene_skip::onMasterSwordSkip);
			}
		}
		// Call original function
		return evt_control_Skipper_trampoline(evtPtr);
	}

	bool Mod::proc_query022(void* unk1, void* unk2, s32 unk3)
	{
		// Check to see if currently in one of the Ordon interiors
		if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Ordon_Interiors]))
		{
			// Check to see if ckecking for the Iron Boots
			u16 item = *reinterpret_cast<u16*>(reinterpret_cast<u32>(unk2) + 0x4);

			if (item == items::Item::Iron_Boots)
			{
				// Return false so that the door in Bo's house can be opened without having the Iron Boots
				return false;
			}
		}

		// Call original function
		return query022_trampoline(unk1, unk2, unk3);
	}
	
	bool Mod::procDoLink(tp::dynamic_link::DynamicModuleControl* dmc)
	{
		// Call the original function immediately, as the REL file needs to be linked before applying patches
		const bool result = do_link_trampoline(dmc);

		// Get the pointer to the current REL file
		gc::OSModule::OSModuleInfo* moduleInfo = dmc->moduleInfo;

		// Make sure a REL file is actually loaded, as do_link will clear the pointer if something goes wrong
		if (!moduleInfo)
		{
			return result;
		}

		// Get the REL pointer as a raw u32, to use for overwrites
		u32 relPtrRaw = reinterpret_cast<u32>(moduleInfo);

		// Modify the current REL file
		switch (moduleInfo->id) // May want to set up enums or defines for the module ids
		{
			case 0x121: // d_a_npc_bouS.rel - Inside Bo's house
			{
				// Prevent Bo from talking after the chest has been opened
				*reinterpret_cast<u32*>(relPtrRaw + 0x1A44) = 0x48000028; // b 0x28
				break;
			}
			default:
			{
				break;
			}
		}

		return result;
	}

	void Mod::procItem_func_UTUWA_HEART()
	{
		/* Call the original function immediately, as the heart container flag 
			for the current area needs to be set before making any adjustments */
		item_func_UTUWA_HEART_trampoline();
		
		// Clear the heart container flag if not currently in a boss room
		if (!chestRandomizer->isStageBoss())
		{
			gameInfo.localAreaNodes.dungeon.containerGotten = 0b0;
		}
	}
	
	void Mod::giveAllScents()
	{
		//code to have all scents at once you need to unlock them tho
		if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Hyrule_Field]))
		{
			if (tools::checkItemFlag(ItemFlags::Youths_Scent) && 
			(tp::d_kankyo::env_light.currentRoom == 3 || tp::d_kankyo::env_light.currentRoom == 2))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Ilias_Scent) && 
			(tp::d_kankyo::env_light.currentRoom == 9 || tp::d_kankyo::env_light.currentRoom == 10))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Village]))
		{
			if (tools::checkItemFlag(ItemFlags::Youths_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Outside_Castle_Town]))
		{
			if (tools::checkItemFlag(ItemFlags::Ilias_Scent) && tp::d_kankyo::env_light.currentRoom == 8)
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town]))
		{
			if (tools::checkItemFlag(ItemFlags::Medicine_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Medicine_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Ilias_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Zoras_Domain]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Snowpeak]))
		{
			if (tools::checkItemFlag(ItemFlags::Reekfish_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Reekfish_Scent;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Arbiters_Grounds]))
		{
			if (tools::checkItemFlag(ItemFlags::Poe_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Poe_Scent;
			}
		}
		else
		{
			if (tools::checkItemFlag(ItemFlags::Medicine_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Medicine_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Reekfish_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Reekfish_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Poe_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Poe_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Ilias_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
			}
			else if (tools::checkItemFlag(ItemFlags::Youths_Scent))
			{
				gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
			}
			
		}
	}
	
	void Mod::giveAllStoryItems()
	{
		if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Lake_Hylia]))
		{
			if (tools::checkItemFlag(ItemFlags::Aurus_Memo))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Aurus_Memo;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Snowpeak]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Graveyard]) || 
		tp::d_a_alink::checkStageName(stage::allStages[Stage_Zoras_Domain]))
		{
			if (tools::checkItemFlag(ItemFlags::Asheis_Sketch))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Asheis_Sketch;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Interiors]) && tp::d_kankyo::env_light.currentRoom == 0)
		{
			if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
			}
			else if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
			}
			else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Outside_Castle_Town]) && tp::d_kankyo::env_light.currentRoom == 3)
		{
			if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town_Shops]) && tp::d_kankyo::env_light.currentRoom == 2)
		{
			if (tools::checkItemFlag(ItemFlags::Invoice))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town_Interiors]) && tp::d_kankyo::env_light.currentRoom == 5)
		{
			if (tools::checkItemFlag(ItemFlags::Invoice))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
			}
			else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
			}
		}
		else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Hidden_Village]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Impaz_House]))
		{
			if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
			}
		}
		else
		{
			if (tools::checkItemFlag(ItemFlags::Horse_Call))
			{//finished Ilia Quest
				gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
			}
			else if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
			}
			else if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
			}
			else if (tools::checkItemFlag(ItemFlags::Invoice))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
			}
			else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
			}
			
			if (tools::checkItemFlag(ItemFlags::Coral_Earring))
			{//given sketch
				gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
			}
			else if (tools::checkItemFlag(ItemFlags::Asheis_Sketch))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Asheis_Sketch;
			}
			
			if (gameInfo.scratchPad.movingActors.exploredRegions.Desert == 0b1)
			{//given memo
				gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
			}
			else if (tools::checkItemFlag(ItemFlags::Aurus_Memo))
			{
				gameInfo.scratchPad.itemWheel.Story = items::Item::Aurus_Memo;
			}
			
		}
	}
	
	void Mod::reorderItemWheel()
	{
		u8 currentSlot = 0x0;
		if (tools::checkItemFlag(ItemFlags::Clawshots))
		{
			gameInfo.scratchPad.itemWheel.Double_Clawshot = items::Item::Clawshots;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xA;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Dominion_Rod))
		{
			gameInfo.scratchPad.itemWheel.Dominion_Rod = items::Item::Dominion_Rod;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x8;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Ball_and_Chain))
		{
			gameInfo.scratchPad.itemWheel.Ball_and_Chain = items::Item::Ball_and_Chain;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x6;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Spinner))
		{
			gameInfo.scratchPad.itemWheel.Spinner = items::Item::Spinner;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x2;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Clawshot))
		{
			gameInfo.scratchPad.itemWheel.Clawshot = items::Item::Clawshot;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x9;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Heros_Bow))
		{
			gameInfo.scratchPad.itemWheel.Bow = items::Item::Heros_Bow;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x4;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Iron_Boots))
		{
			gameInfo.scratchPad.itemWheel.Iron_Boots = items::Item::Iron_Boots;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x3;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Boomerang))
		{
			gameInfo.scratchPad.itemWheel.Boomerang = items::Item::Boomerang;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x0;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Lantern))
		{
			gameInfo.scratchPad.itemWheel.Lantern = items::Item::Lantern;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x1;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Slingshot))
		{
			gameInfo.scratchPad.itemWheel.Slingshot = items::Item::Slingshot;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x17;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Fishing_Rod != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x14;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Hawkeye))
		{
			gameInfo.scratchPad.itemWheel.Hawkeye = items::Item::Hawkeye;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x5;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bomb_Bag_1 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xF;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bomb_Bag_2 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x10;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bomb_Bag_3 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x11;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bottle_1 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xB;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bottle_2 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xC;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bottle_3 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xD;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Bottle_4 != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xE;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Story != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x13;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Ooccoo != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x12;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Sky_Book != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x16;
			currentSlot++;
		}
		if (tools::checkItemFlag(ItemFlags::Horse_Call))
		{
			gameInfo.scratchPad.itemWheel.Horse_Call = items::Item::Horse_Call;
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x15;
			currentSlot++;
		}
		if (gameInfo.scratchPad.itemWheel.Item_Slot != 0xFF)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x7;
			currentSlot++;
		}
		
		for(u16 i = currentSlot; i < sizeof(gameInfo.scratchPad.itemSlotsOrder)/sizeof(u8); i++)
		{
			gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xFF;
		}
	}
}