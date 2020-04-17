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
		 
		game_patch::isMDHSkipEnabled = 1;
		game_patch::isForestEscapeEnabled = 1;
		game_patch::isGateUnlockEnabled = 1;
		game_patch::isGoatSkipEnabled = 1;

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
		
		/*hudConsole->addOption(page, "Item half milk", &chestRandomizer->itemThatReplacesHalfMilk, 0xFF); //for testing only
		hudConsole->addOption(page, "Item slingshot", &chestRandomizer->itemThatReplacesSlingShot, 0xFF); //for testing only
		hudConsole->addOption(page, "Normal Time:", &enableNormalTime, 0x1); //for testing only
		hudConsole->addOption(page, "Set Day:", &setDay, 0x1); //for testing only*/
		hudConsole->addOption(page, "MDH skip?", &game_patch::isMDHSkipEnabled, 0x1);
		hudConsole->addOption(page, "Faron Escape?", &game_patch::isForestEscapeEnabled, 0x1);
		hudConsole->addOption(page, "open HF gates?", &game_patch::isGateUnlockEnabled, 0x1);
		hudConsole->addOption(page, "skip goats2?", &game_patch::isGoatSkipEnabled, 0x1);
		
		
		hudConsole->addWatch(page, "CurrentStage:", &gameInfo.currentStage, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentRoom:", &tp::d_kankyo::env_light.currentRoom, 'd', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "CurrentState:", &tp::d_com_inf_game::current_state, 'x', WatchInterpretation::_u8);
		
		hudConsole->addWatch(page, "CurrentPosX:", &currentPosX, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosY:", &currentPosY, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "CurrentPosZ:", &currentPosZ, 's', WatchInterpretation::_str);	
		hudConsole->addWatch(page, "Time of day:", &gameInfo.scratchPad.unk_0[0x34], 'd', WatchInterpretation::_u32);
		
		//event info
		page = hudConsole->addPage("Event Info");
				
		hudConsole->addWatch(page, "CurrentEventID:", &gameInfo.eventSystem.currentEventID, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextEventID:", &gameInfo.eventSystem.nextEventID, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextStage:", &gameInfo.nextStageVars.nextStage, 's', WatchInterpretation::_str);
		hudConsole->addWatch(page, "NextRoom:", &gameInfo.nextStageVars.nextRoom, 'd', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextSpawnPoint:", &gameInfo.nextStageVars.nextSpawnPoint, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "NextSate:", &gameInfo.nextStageVars.nextState, 'x', WatchInterpretation::_u8);
		
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
		hudConsole->addWatch(page, "AreaNodes1F:", &gameInfo.localAreaNodes.unk_1E[0x1], 'x', WatchInterpretation::_u8);*/

		//item slots
		/*page = hudConsole->addPage("Item slots 1");		
		hudConsole->addWatch(page, "Boomerang:", &gameInfo.scratchPad.itemWheel.Boomerang, 'x', WatchInterpretation::_u8);
		hudConsole->addWatch(page, "Slot 1:", &gameInfo.scratchPad.itemSlots[0x1], 'x', WatchInterpretation::_u8);
		
		hudConsole->addWatch(page, "flags1:", &gameInfo.scratchPad.itemFlags.itemFlags1, 'x', WatchInterpretation::_u64);
		hudConsole->addWatch(page, "flags2:", &gameInfo.scratchPad.itemFlags.itemFlags2, 'x', WatchInterpretation::_u64);
		hudConsole->addWatch(page, "flags3:", &gameInfo.scratchPad.itemFlags.itemFlags3, 'x', WatchInterpretation::_u64);
		hudConsole->addWatch(page, "falgs4:", &gameInfo.scratchPad.itemFlags.itemFlags4, 'x', WatchInterpretation::_u64);*/
		
		/*page = hudConsole->addPage("Warps 1");
		hudConsole->addOption(page, "Mirror Chamber:", &gameInfo.scratchPad.allAreaNodes.Gerudo_Desert.unk_0[0xE], 0xFF);
		hudConsole->addOption(page, "Gerudo Mesa:", &gameInfo.scratchPad.allAreaNodes.Gerudo_Desert.unk_0[0x9], 0xFF);
		hudConsole->addOption(page, "Snowpeak Top:", &gameInfo.scratchPad.allAreaNodes.Snowpeak.unk_0[0x9], 0xFF);
		hudConsole->addOption(page, "Sacred Grove:", &gameInfo.scratchPad.allAreaNodes.Sacred_Grove.unk_0[0x17], 0xFF);
		hudConsole->addOption(page, "Eldin Bridge:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0x17], 0xFF);
		hudConsole->addOption(page, "Castle Town:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0xB], 0xFF);
		hudConsole->addOption(page, "Kakariko Gorge:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0x9], 0xFF);
		hudConsole->addOption(page, "Zoras Domain:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0xB], 0xFF);
		hudConsole->addOption(page, "Lake Hylia:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0xA], 0xFF);
		hudConsole->addOption(page, "Zora River:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0x9], 0xFF);
		
		hudConsole->addWatch(page, "Mirror Chamber:", &gameInfo.scratchPad.allAreaNodes.Gerudo_Desert.unk_0[0xE], 'x', WatchInterpretation::_u8);//1
		hudConsole->addWatch(page, "Gerudo Mesa:", &gameInfo.scratchPad.allAreaNodes.Gerudo_Desert.unk_0[0x9], 'x', WatchInterpretation::_u8);//32
		hudConsole->addWatch(page, "Snowpeak Top:", &gameInfo.scratchPad.allAreaNodes.Snowpeak.unk_0[0x9], 'x', WatchInterpretation::_u8);//32
		hudConsole->addWatch(page, "Sacred Grove:", &gameInfo.scratchPad.allAreaNodes.Sacred_Grove.unk_0[0x17], 'x', WatchInterpretation::_u8);//16
		hudConsole->addWatch(page, "Eldin Bridge:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0x17], 'x', WatchInterpretation::_u8);//8
		hudConsole->addWatch(page, "Castle Town:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0xB], 'x', WatchInterpretation::_u8);//8
		hudConsole->addWatch(page, "Kakariko Gorge:", &gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0x9], 'x', WatchInterpretation::_u8);//32
		hudConsole->addWatch(page, "Zoras Domain:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0xB], 'x', WatchInterpretation::_u8);//4
		hudConsole->addWatch(page, "Lake Hylia:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0xA], 'x', WatchInterpretation::_u8);//4
		hudConsole->addWatch(page, "Zora River:", &gameInfo.scratchPad.allAreaNodes.Lanyru.unk_0[0x9], 'x', WatchInterpretation::_u8);//32
		page = hudConsole->addPage("Warps 2");
		hudConsole->addOption(page, "Death Mountain:", &gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x9], 0xFF);
		hudConsole->addOption(page, "Kakariko:", &gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x8], 0xFF);
		hudConsole->addOption(page, "South Faron:", &gameInfo.scratchPad.allAreaNodes.Faron.unk_0[0x13], 0xFF);
		hudConsole->addOption(page, "North Faron:", &gameInfo.scratchPad.allAreaNodes.Faron.unk_0[0xB], 0xFF);
		hudConsole->addOption(page, "Ordon Spring:", &gameInfo.scratchPad.allAreaNodes.Ordon.unk_0[0xD], 0xFF);
		
		hudConsole->addWatch(page, "Death Mountain:", &gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x9], 'x', WatchInterpretation::_u8);//32
		hudConsole->addWatch(page, "Kakariko:", &gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x8], 'x', WatchInterpretation::_u8);//128
		hudConsole->addWatch(page, "South Faron:", &gameInfo.scratchPad.allAreaNodes.Faron.unk_0[0x13], 'x', WatchInterpretation::_u8);//128
		hudConsole->addWatch(page, "North Faron:", &gameInfo.scratchPad.allAreaNodes.Faron.unk_0[0xB], 'x', WatchInterpretation::_u8);//4
		hudConsole->addWatch(page, "Ordon Spring:", &gameInfo.scratchPad.allAreaNodes.Ordon.unk_0[0xD], 'x', WatchInterpretation::_u8);//16*/
		
		
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

		//skip goats 2
		eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Ranch], 0x0, 0x2, 0xFF, 0xFF, game_patch::skipGoats2, event::LoadEventAccuracy::Stage_Room_Spawn);	



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
		
		if (trigerLoadSave == 1){
			trigerLoadSave = 0;
			tools::triggerSaveLoad(stage::allStages[stage], room, spawn, state);
		}
		
		if (gameInfo.scratchPad.itemFlags.itemFlags1.Orange_Rupee == 0b0)
		{//remove the item get animations for floor pickups (except silver rupee)
			gameInfo.scratchPad.itemFlags.itemFlags1.Blue_Rupee = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Yellow_Rupee = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Red_Rupee = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Purple_Rupee = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Orange_Rupee = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Seeds_50 = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Arrows_30 = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Arrows_20 = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Arrows_10 = 0b1;
			gameInfo.scratchPad.itemFlags.itemFlags1.Arrows_1 = 0b1;
		}
		
		if (enableNormalTime == 0 && setDay == 0)
		{//set night
			gameInfo.scratchPad.unk_0[0x34] = 0b01000010;
			gameInfo.scratchPad.unk_0[0x35] = 0b00101001;
			gameInfo.scratchPad.unk_0[0x36] = 0b01000001;
			gameInfo.scratchPad.unk_0[0x37] = 0b10000000;
		}
		else if (enableNormalTime == 0 && setDay == 1)
		{//set day
			gameInfo.scratchPad.unk_0[0x34] = 0b01000010;
			gameInfo.scratchPad.unk_0[0x35] = 0b11000001;
			gameInfo.scratchPad.unk_0[0x36] = 0b11011000;
			gameInfo.scratchPad.unk_0[0x37] = 0b00000000;
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
}