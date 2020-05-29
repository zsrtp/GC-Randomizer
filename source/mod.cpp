#include "defines.h"
#include "mod.h"
#include "global.h"
#include "systemConsole.h"
#include "patch.h"
#include "controller.h"
#include "customChecks.h"
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
#include <tp/d_stage.h>
#include <tp/dzx.h>
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
    int num_frames = 120;
    int frame_counter = 0;
    
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
        strcpy(sysConsolePtr->consoleLine[24].line, "rando.tpspeed.run | Twitter: @ztprandomizer");

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
        
        hudConsole->addOption(page, "Bugsanity?", &chestRandomizer->isBugsanityEnabled, 0x1);
        hudConsole->addOption(page, "Poesanity?", &chestRandomizer->isPoesanityEnabled, 0x1);
        hudConsole->addOption(page, "Shopsanity?", &chestRandomizer->isShopsanityEnabled, 0x1);    
        hudConsole->addOption(page, "Dungeon Items?", &chestRandomizer->areDungeonItemsRandomized, 0x1);
        hudConsole->addOption(page, "Keysanity?", &chestRandomizer->isKeysanityEnabled, 0x1);
        hudConsole->addOption(page, "Skybooksanity?", &Singleton::getInstance()->shuffledSkybook, 0x1);
        
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

        hudConsole->addWatch(page, "Search Result:", &itemSearchResults, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "Reverse Result:", &itemReverseSearchResults, 's', WatchInterpretation::_str);

        hudConsole->addWatch(page, "MetadataID:", &lastItemDataID, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "MetadataX:", &lastItemDataX, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "MetadataY:", &lastItemDataY, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "MetadataZ:", &lastItemDataZ, 's', WatchInterpretation::_str);
        
        // Check search
        page = hudConsole->addPage("Check Search");

        hudConsole->addOption(page, "Search ID1:", &checkSearchID1, 0xFF);
        hudConsole->addOption(page, "Search ID2:", &checkSearchID2, 0xFF);
        hudConsole->addOption(page, "Reverse ID1:", &checkReverseSearchID1, 0xFF);
        hudConsole->addOption(page, "Reverse ID2:", &checkReverseSearchID2, 0xFF);

        hudConsole->addWatch(page, "Search ID:", &checkSearchID, 'd', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "Reverse ID:", &checkReverseSearchID, 'd', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "Search Result:", &checkSearchResults, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "Reverse Result:", &checkReverseSearchResults, 's', WatchInterpretation::_str);
        
        
        // Game info
        page = hudConsole->addPage("Game Info");
        
        hudConsole->addOption(page, "MDH skip?", &Singleton::getInstance()->isMDHSkipEnabled, 0x1);
        hudConsole->addOption(page, "Faron Escape?", &Singleton::getInstance()->isForestEscapeEnabled, 0x1);
        hudConsole->addOption(page, "open HF gates?", &Singleton::getInstance()->isGateUnlockEnabled, 0x1);
        hudConsole->addOption(page, "Skip Twilight?", &chestRandomizer->isTwilightSkipEnabled, 0x1);
        hudConsole->addOption(page, "skip goats1&2?", &Singleton::getInstance()->isGoatSkipEnabled, 0x1);
        hudConsole->addOption(page, "skip MS puzzle?", &Singleton::getInstance()->isMSPuzzleSkipEnabled, 0x1);
        hudConsole->addOption(page, "Skip Escort?", &Singleton::getInstance()->isCartEscortSkipEnabled, 0x1);
        hudConsole->addOption(page, "Skip Sewers?", &Singleton::getInstance()->isSewerSkipEnabled, 0x1);
        hudConsole->addOption(page, "Coords as hex?", &coordsAreInHex, 0x1);
        
        hudConsole->addWatch(page, "CurrentStage:", &gameInfo.currentStage, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "CurrentRoom:", &tp::d_kankyo::env_light.currentRoom, 'd', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "CurrentState:", &tp::d_com_inf_game::current_state, 'x', WatchInterpretation::_u8);
        
        hudConsole->addWatch(page, "CurrentPosX:", &currentPosX, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "CurrentPosY:", &currentPosY, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "CurrentPosZ:", &currentPosZ, 's', WatchInterpretation::_str);
        hudConsole->addWatch(page, "CurrentAngle:", &linkAngle, 's', WatchInterpretation::_str);    
        hudConsole->addWatch(page, "Sky Angle:", &skyAngle, 's', WatchInterpretation::_str);            
        
        //event info
        page = hudConsole->addPage("Event Info");
        
        /*hudConsole->addOption(page, "Item half milk", &chestRandomizer->itemThatReplacesHalfMilk, 0xFF); //for testing only
        hudConsole->addOption(page, "Item slingshot", &chestRandomizer->itemThatReplacesSlingShot, 0xFF); //for testing only
        hudConsole->addOption(page, "Normal Time:", &enableNormalTime, 0x1); //for testing only
        hudConsole->addOption(page, "Set Day:", &setDay, 0x1); //for testing only
        hudConsole->addOption(page, "Unused Slot:", &gameInfo.scratchPad.itemWheel.Item_Slot, 0xFF); //for testing only
        hudConsole->addOption(page, "Form:", &gameInfo.scratchPad.form, 0x1); //for testing only*/
        hudConsole->addOption(page, "Fast transform?", &enableQuickTransform, 0x1);
        hudConsole->addOption(page, "no bottle req?", &allowBottleItemsShopAnytime, 0x1);
        hudConsole->addOption(page, "Early CiTS?", &Singleton::getInstance()->isEarlyCiTSEnabled, 0x1);
        hudConsole->addOption(page, "Early Desert?", &Singleton::getInstance()->isEarlyDesertEnabled, 0x1);
        hudConsole->addOption(page, "Boss Keysey?", &Singleton::getInstance()->isBossKeyseyEnabled, 0x1);
                
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
        hudConsole->addOption(page, "X item:", &gameInfo.xItem, 0xFF);
        hudConsole->addOption(page, "Y item:", &gameInfo.yItem, 0xFF);
        hudConsole->addWatch(page, "A button:", &gameInfo.aButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "Y button:", &gameInfo.yButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "X button:", &gameInfo.xButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "B button:", &gameInfo.bButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "R button:", &gameInfo.rButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "stick:", &gameInfo.stickButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "start button:", &gameInfo.startButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "C button:", &gameInfo.cButtonText, 'x', WatchInterpretation::_u8);*/

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
        
        // save load
        /*page = hudConsole->addPage("Save load");
        
        hudConsole->addOption(page, "stage:", &stage, 78); //for testing only
        hudConsole->addOption(page, "room:", &room, 60); //for testing only
        hudConsole->addOption(page, "spawn:", &spawn, 0xFF); //for testing only
        hudConsole->addOption(page, "state:", &state, 0xFF); //for testing only
        hudConsole->addOption(page, "trigger:", &trigerLoadSave, 0x1); //for testing only*/
        
        /*page = hudConsole->addPage("testing adr1");
        
        
        hudConsole->addWatch(page, "D:", &gameInfo.scratchPad.equipedItems.unk, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "17:", &gameInfo.scratchPad.unk_17[0x0], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "1A:", &gameInfo.scratchPad.unk_1A[0x0], 'x', WatchInterpretation::_u32);
        hudConsole->addWatch(page, "1F:", &gameInfo.scratchPad.unk_1F[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "27:", &gameInfo.scratchPad.unk_1F[0x8], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "2F:", &gameInfo.scratchPad.unk_1F[0x10], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "32:", &gameInfo.scratchPad.unk_32[0x0], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "38:", &gameInfo.scratchPad.unk_38[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "62:", &gameInfo.scratchPad.movingActors.link.unk_62[0x0], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "7A:", &gameInfo.scratchPad.movingActors.unk_7A[0x0], 'x', WatchInterpretation::_u32);//actual size:0x3
        
        page = hudConsole->addPage("testing adr2");
        
        hudConsole->addWatch(page, "7E:", &gameInfo.scratchPad.movingActors.unk_7E[0x0], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "99:", &gameInfo.scratchPad.unk_99[0x0], 'x', WatchInterpretation::_u32);//actual size:0x3
        hudConsole->addWatch(page, "F0:", &gameInfo.scratchPad.ammo.unk_F0, 'x', WatchInterpretation::_u32);
        hudConsole->addWatch(page, "F5:", &gameInfo.scratchPad.ammo.unk_F5, 'x', WatchInterpretation::_u32);//actual size:0x3
        hudConsole->addWatch(page, "FC:", &gameInfo.scratchPad.unk_FC[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "104:", &gameInfo.scratchPad.unk_FC[0x8], 'x', WatchInterpretation::_u64);//actual size:0x5
        hudConsole->addWatch(page, "10B:", &gameInfo.scratchPad.unk_10B, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "10D:", &gameInfo.scratchPad.unk_10D[0x0], 'x', WatchInterpretation::_u64);//actual size:0x7
        hudConsole->addWatch(page, "115:", &gameInfo.scratchPad.unk_115[0x0], 'x', WatchInterpretation::_u64);//actual size:0x7
        hudConsole->addWatch(page, "120:", &gameInfo.scratchPad.unk_120[0x0], 'x', WatchInterpretation::_u64);//actual size:0x6
        
        page = hudConsole->addPage("testing adr2");
        
        hudConsole->addWatch(page, "128:", &gameInfo.scratchPad.unk_128[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "130:", &gameInfo.scratchPad.unk_128[0x8], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "138:", &gameInfo.scratchPad.unk_128[0x10], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "140:", &gameInfo.scratchPad.unk_128[0x18], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "148:", &gameInfo.scratchPad.unk_128[0x20], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "150:", &gameInfo.scratchPad.unk_128[0x28], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "158:", &gameInfo.scratchPad.unk_128[0x30], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "160:", &gameInfo.scratchPad.unk_128[0x38], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "168:", &gameInfo.scratchPad.unk_128[0x40], 'x', WatchInterpretation::_u32);
        hudConsole->addWatch(page, "178:", &gameInfo.scratchPad.fishingJournal.unk_178[0x0], 'x', WatchInterpretation::_u64);
        
        page = hudConsole->addPage("testing adr3");
        
        hudConsole->addWatch(page, "180:", &gameInfo.scratchPad.fishingJournal.unk_178[0x8], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "188:", &gameInfo.scratchPad.fishingJournal.unk_178[0x10], 'x', WatchInterpretation::_u32);
        hudConsole->addWatch(page, "192:", &gameInfo.scratchPad.unk_192[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "19A:", &gameInfo.scratchPad.unk_192[0x8], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "1A2:", &gameInfo.scratchPad.unk_192[0x10], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "1AA:", &gameInfo.scratchPad.unk_192[0x18], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "1B2:", &gameInfo.scratchPad.unk_192[0x20], 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "1C4:", &gameInfo.scratchPad.unk_1C4, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "1D5:", &gameInfo.scratchPad.unk_1D5[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "1DD:", &gameInfo.scratchPad.unk_1D5[0x8], 'x', WatchInterpretation::_u32);
        hudConsole->addWatch(page, "1E4:", &gameInfo.scratchPad.unk_1E4[0x0], 'x', WatchInterpretation::_u64);
        hudConsole->addWatch(page, "1EC:", &gameInfo.scratchPad.unk_1E4[0x8], 'x', WatchInterpretation::_u32);*/
        
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
        
        //unlock HF gates
        eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Field], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::unlockHFGates, event::LoadEventAccuracy::Stage);    

        //skip goats 1 & 2
        eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Ranch], 0x0, 0x3, 0xFF, 0xFF, game_patch::skipGoats, event::LoadEventAccuracy::Stage_Room_Spawn);    

        // Set Bublin Camp State
        eventListener->addLoadEvent(stage::allStages[Stage_Bublin_Camp], 0xFF, 0xFF, 0x1, 0xFF, game_patch::setBublinState, event::LoadEventAccuracy::Stage_Room_Spawn);

        //skip MS Puzzle
        eventListener->addLoadEvent(stage::allStages[Stage_Sacred_Grove], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::skipGrovePuzzle, event::LoadEventAccuracy::Stage);

        // skip Cart Escort
        eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Field], 0xC, 0x2, 0xFF, 0xFF, game_patch::skipCartEscort, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // Fix Lanayru Softlock
        eventListener->addLoadEvent(stage::allStages[Stage_Lake_Hylia], 0x0, 0x5, 0xE, 0xFF, game_patch::setLanayruWolf, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // Set the state of Faron if the player decides to do lanayru twilight before faron
        eventListener->addLoadEvent(stage::allStages[Stage_Faron_Woods], 0xFF, 0xFF, 0x0, 0xFF, game_patch::fixLanayruFaron, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // Sets the flags to open up the game
        eventListener->addLoadEvent(stage::allStages[Stage_Ordon_Village], 0x1, 0x15, 0xFF, 0xFF, game_patch::unlockBossDoors, event::LoadEventAccuracy::Stage_Room);
        
        // early CiTS
        eventListener->addLoadEvent(stage::allStages[Stage_Lake_Hylia], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::earlyCiTS, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // early Desert
        eventListener->addLoadEvent(stage::allStages[Stage_Lake_Hylia], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::earlyDesert, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // desert Access
        eventListener->addLoadEvent(stage::allStages[Stage_Gerudo_Desert], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::accessDesert, event::LoadEventAccuracy::Stage_Room_Spawn);
        
        // sell water bombs if you skip the escort
        eventListener->addLoadEvent(stage::allStages[Stage_Kakariko_Village], 0xFF, 0xFF, 0xFF, 0xFF, game_patch::sellWaterBombs, event::LoadEventAccuracy::Stage_Room_Spawn);

        //   =================
        //  | Function Hooks  |
        //   =================

        fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
            []()
            {
                global::modPtr->procNewFrame();
            }
        );
        
        // Hook actor init function to use as a timing for placing custom chests as this function only creates actors at
        // specific times
        actorCommonLayerInit_trampoline = patch::hookFunction(tp::d_stage::actorCommonLayerInit,
            [](void* mStatus_roomControl, tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4)
            {
                // if unk4 is nullptr and unk3 is 0 it's probably ourselves calling this function
                // Thus don't call it again to avoid an infinite loop!
                if (unk3 != 0 && unk4)
                {
                    // doCustomTRESActor will call this function with unk3=0 and unk4=nullptr
                    // So we only need to pass the status_roomControl (which should be static through LST anyway)
                    // to maintain consistency
                    global::modPtr->doCustomTRESActor(mStatus_roomControl);
                }

                return global::modPtr->actorCommonLayerInit_trampoline(mStatus_roomControl, chunkTypeInfo, unk3, unk4);
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
                /*char txt[50];
                snprintf(txt, 50, "0 = %f 1 = %f 2 = %f", pos[0], pos[1], pos[2]);
                strcpy(sysConsolePtr->consoleLine[20].line, txt);*/
                
                
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

        createItemForSimpleDemo_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForSimpleDemo,
            [](const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7)
            {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc(pos, item, "createItemForSimpleDemo");

                return global::modPtr->createItemForSimpleDemo_trampoline(pos, item, unk3, unk4, unk5, unk6, unk7);
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
        
        
        setItemBombNumCount_trampoline = patch::hookFunction(tp::d_com_inf_game::setItemBombNumCount,
            [](u32 unk1, u8 bagNb, short amount)
            {
                u8 bombtype = 0;
                if (bagNb == 0)
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_1;
                }
                else if (bagNb == 1)
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_2;
                }
                else if (bagNb == 2)
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_3;
                }
                char txt[50];
                snprintf(txt, 50, "bag = %x amount = %d type = %x", bagNb, amount, bombtype);
                strcpy(sysConsolePtr->consoleLine[20].line, txt);
                
                /*u8 itemID = 0x0;
                if (bombtype == items::Item::Bomb_Bag_Regular_Bombs)
                {
                    if (amount == 5)
                    {
                        itemID = items::Item::Bombs_5;
                    }
                    else if (amount == 10)
                    {
                        itemID = items::Item::Bombs_10;
                    }
                    else if (amount == 20)
                    {
                        itemID = items::Item::Bombs_20;
                    }
                    else if (amount == 30)
                    {
                        itemID = items::Item::Bombs_30;
                    }
                }
                else if (bombtype == items::Item::Bomb_Bag_Water_Bombs)
                {
                    if (amount == 3)
                    {
                        itemID = items::Item::Water_Bombs_3;
                    }
                    else if (amount == 5)
                    {
                        itemID = items::Item::Water_Bombs_5;
                    }
                    else if (amount == 10)
                    {
                        itemID = items::Item::Water_Bombs_10;
                    }
                    else if (amount == 15)
                    {
                        itemID = items::Item::Water_Bombs_15;
                    }
                }
                else if (bombtype == items::Item::Bomb_Bag_Bomblings)
                {
                    if (amount == 1)
                    {
                        itemID = items::Item::Bombling_1;
                    }
                    else if (amount == 3)
                    {
                        itemID = items::Item::Bomblings_3;
                    }
                    else if (amount == 5)
                    {
                        itemID = items::Item::Bomblings_5;
                    }
                    else if (amount == 10)
                    {
                        itemID = items::Item::Bomblings_10;
                    }
                }
                
                
                float linkPos[3];
                getPlayerPos(linkPos);
                
                const float zero[3] = {0.0f,0.0f,0.0f};
                
                amount = 0;
                
                tp::f_op_actor_mng::createItemForPresentDemo(linkPos, itemID, 0, -1, -1, zero, zero);*/
                
                return global::modPtr->setItemBombNumCount_trampoline(unk1, bagNb, amount);
            }
        );
    }

    void Mod::procNewFrame()
    {
        float linkPos[3];
        getPlayerPos(linkPos);
        
        if (coordsAreInHex == 1)
        {
            typeTransform<float, u32> x = {linkPos[0]};
            typeTransform<float, u32> y = {linkPos[1]};
            typeTransform<float, u32> z = {linkPos[2]};

            snprintf(currentPosX, 30, "%04x", x.b);
            snprintf(currentPosY, 30, "%04x", y.b);
            snprintf(currentPosZ, 30, "%04x", z.b);
            
            snprintf(linkAngle, 30, "%02x", static_cast<u16>(tp::d_map_path_dmap::getMapPlayerAngleY()));
        }
        else 
        {
            snprintf(currentPosX, 30, "%f", linkPos[0]);
            snprintf(currentPosY, 30, "%f", linkPos[1]);
            snprintf(currentPosZ, 30, "%f", linkPos[2]);
            
            snprintf(linkAngle, 30, "%d", static_cast<u16>(tp::d_map_path_dmap::getMapPlayerAngleY()));
        }
        
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
        else if(enableQuickTransform == 1 && gameInfo.rButtonText == 0 && (gameInfo.bButtonText == 0x3 || gameInfo.bButtonText == 0x26) && 
        (gameInfo.scratchPad.eventBits[0xD] & 0x4) != 0 && controller::checkForButtonInputSingleFrame(controller::PadInputs::Button_R))
        {                
            // Make sure Link is actually loaded
            tp::d_com_inf_game::LinkMapVars* linkMapPtr = gameInfo.linkMapPtr;
            if (linkMapPtr)
            {
                if (!((linkMapPtr->isTargeting & 0x400000) != 0 && gameInfo.scratchPad.form == 0))
                {
                    // Transform
                    tp::d_a_alink::procCoMetamorphoseInit(linkMapPtr);
                }
            }
        }

        if(sysConsolePtr->consoleEnabled)
        {
            if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Start)))
            {
                if (customSeed == 0)
                {    
                    u16 lastCheckSum = chestRandomizer->checkSum;
                    do
                    {
                        chestRandomizer->generate();
                    }
                    while (chestRandomizer->checkSum == lastCheckSum);
                }
                else
                {
                    chestRandomizer->generate();
                }
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
        
        
        checkSearchID = (checkSearchID2 * 0x100) + checkSearchID1;
        checkReverseSearchID = (checkReverseSearchID2 * 0x100) + checkReverseSearchID1;
        if(checkSearchID != lastCheckSearchID)
        {
            lastCheckSearchID = checkSearchID;

            strcpy(checkSearchResults, "404");
            if (checkSearchID < chestRandomizer->totalChecks)
            {
                item::ItemCheck* check = &item::checks[checkSearchID];
                if (check->source)
                {
                    snprintf(checkSearchResults, 40, "ID: %x Stage: %s Room: %d", check->source->itemID, check->source->stage, check->source->room);
                }
            }
        }
        else if(checkReverseSearchID != lastCheckReverseSearchID)
        {
            lastCheckReverseSearchID = checkReverseSearchID;

            strcpy(checkReverseSearchResults, "404");

            if (checkReverseSearchID < chestRandomizer->totalChecks)
            {
                item::ItemCheck* check = &item::checks[checkReverseSearchID];
                if (check->destination)
                {
                    snprintf(checkReverseSearchResults, 40, "ID: %x Stage: %s Room: %d", check->destination->itemID, check->destination->stage, check->destination->room);
                }
            }
        }
        
        if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Faron_Woods]))
        {
            if (gameInfo.eventSystem.currentEventID == 0xB)
            {
                tools::setCutscene(false, false);
                if (frame_counter == num_frames)
                {
                    tools::setItemFlag(ItemFlags::Vessel_Of_Light_Faron);  // set flag for vessel since we'll skip it by reloading
                    gameInfo.scratchPad.movingActors.exploredRegions.Eldin = 0b1;  // Set Eldin Map Unlocked so we can warp there
                    tools::setCutscene(true, false);
                }
                else
                {
                    frame_counter++;
                }
            }
            else
            {
                frame_counter = 0;
            }
        }

        if(inputBuffering)
        {
            tp::m_do_controller_pad::cpadInfo.buttonInputTrg = tp::m_do_controller_pad::cpadInfo.buttonInput;
        }
        
        if (dungeonFlagsViewEdit == 0)
        {
            tp::d_com_inf_game::DungeonFlags* dungeon = &gameInfo.localAreaNodes.dungeon;
            if (dungeon->miniBossBeaten == 0b0)
            {
                dungeonFlagsView1 = 0;
            }
            else
            {
                dungeonFlagsView1 = 1;
            }
            if (dungeon->ooccooGotten == 0b0)
            {
                dungeonFlagsView2 = 0;
            }
            else
            {
                dungeonFlagsView2 = 1;
            }
            if (dungeon->bitSix == 0b0)
            {
                dungeonFlagsView3 = 0;
            }
            else
            {
                dungeonFlagsView3 = 1;
            }
            if (dungeon->containerGotten == 0b0)
            {
                dungeonFlagsView4 = 0;
            }
            else
            {
                dungeonFlagsView4 = 1;
            }
            if (dungeon->bossBeaten == 0b0)
            {
                dungeonFlagsView5 = 0;
            }
            else
            {
                dungeonFlagsView5 = 1;
            }
            if (dungeon->bigKeyGotten == 0b0)
            {
                dungeonFlagsView6 = 0;
            }
            else
            {
                dungeonFlagsView6 = 1;
            }
            if (dungeon->compassGotten == 0b0)
            {
                dungeonFlagsView7 = 0;
            }
            else
            {
                dungeonFlagsView7 = 1;
            }
            if (dungeon->mapGotten == 0b0)
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
            tp::d_com_inf_game::DungeonFlags* dungeon = &gameInfo.localAreaNodes.dungeon;
            if (dungeonFlagsView1 == 0)
            {
                dungeon->miniBossBeaten = 0b0;
            }
            else
            {
                dungeon->miniBossBeaten = 0b1;
            }
            if (dungeonFlagsView2 == 0)
            {
                dungeon->ooccooGotten = 0b0;
            }
            else
            {
                dungeon->ooccooGotten = 0b1;
            }
            if (dungeonFlagsView3 == 0)
            {
                dungeon->bitSix = 0b0;
            }
            else
            {
                dungeon->bitSix = 0b1;
            }
            if (dungeonFlagsView4 == 0)
            {
                dungeon->containerGotten = 0b0;
            }
            else
            {
                dungeon->containerGotten = 0b1;
            }
            if (dungeonFlagsView5 == 0)
            {
                dungeon->bossBeaten = 0b0;
            }
            else
            {
                dungeon->bossBeaten = 0b1;
            }
            if (dungeonFlagsView6 == 0)
            {
                dungeon->bigKeyGotten = 0b0;
            }
            else
            {
                dungeon->bigKeyGotten = 0b1;
            }
            if (dungeonFlagsView7 == 0)
            {
                dungeon->compassGotten = 0b0;
            }
            else
            {
                dungeon->compassGotten = 0b1;
            }
            if (dungeonFlagsView8 == 0)
            {
                dungeon->mapGotten = 0b0;
            }
            else
            {
                dungeon->mapGotten = 0b1;
            }
        }
        
        giveAllScents();
        
        giveAllStoryItems();
        
        reorderItemWheel();
        
        allowShopItemsAnytime();
        
        fixYetaAndYeto();
        
        fixLBTBossDoor();
        
        preventPoweringUpDomRod();
        
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
        if(randoEnabled && strcmp(funcIdentifier, "createItemForDirectGet") != 0 && strcmp(funcIdentifier, "createItemForBoss") != 0 &&
        strcmp(funcIdentifier, "createItemForMidBoss") != 0 && strcmp(funcIdentifier, "createItemForSimpleDemo") != 0 )
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
        u8* scent = &gameInfo.scratchPad.equipedItems.scent;
        
        //code to have all scents at once you need to unlock them tho
        if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Hyrule_Field]))
        {
            if (tools::checkItemFlag(ItemFlags::Youths_Scent) && tp::d_kankyo::env_light.currentRoom == 3)
            {
                *scent = items::Item::Youths_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Ilias_Scent) && 
            (tp::d_kankyo::env_light.currentRoom == 9 || tp::d_kankyo::env_light.currentRoom == 10))
            {
                *scent = items::Item::Ilias_Scent;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Village]))
        {
            if (tools::checkItemFlag(ItemFlags::Youths_Scent))
            {
                *scent = items::Item::Youths_Scent;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Outside_Castle_Town]))
        {
            if (tools::checkItemFlag(ItemFlags::Ilias_Scent) && tp::d_kankyo::env_light.currentRoom == 8)
            {
                *scent = items::Item::Ilias_Scent;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town]))
        {
            if (tools::checkItemFlag(ItemFlags::Medicine_Scent))
            {
                *scent = items::Item::Medicine_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Ilias_Scent))
            {
                *scent = items::Item::Ilias_Scent;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Zoras_Domain]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Snowpeak]))
        {
            if (tools::checkItemFlag(ItemFlags::Reekfish_Scent))
            {
                *scent = items::Item::Reekfish_Scent;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Arbiters_Grounds]))
        {
            if (tools::checkItemFlag(ItemFlags::Poe_Scent))
            {
                *scent = items::Item::Poe_Scent;
            }
        }
        else
        {
            if (tools::checkItemFlag(ItemFlags::Medicine_Scent))
            {
                *scent = items::Item::Medicine_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Reekfish_Scent))
            {
                *scent = items::Item::Reekfish_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Poe_Scent))
            {
                *scent = items::Item::Poe_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Ilias_Scent))
            {
                *scent = items::Item::Ilias_Scent;
            }
            else if (tools::checkItemFlag(ItemFlags::Youths_Scent))
            {
                *scent = items::Item::Youths_Scent;
            }
            
        }
    }
    
    void Mod::giveAllStoryItems()
    {
        tp::d_com_inf_game::ItemSlots* itemWheel = &gameInfo.scratchPad.itemWheel;
        
        if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Lake_Hylia]))
        {
            if (tools::checkItemFlag(ItemFlags::Aurus_Memo))
            {
                itemWheel->Story = items::Item::Aurus_Memo;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Snowpeak]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Graveyard]) || 
        tp::d_a_alink::checkStageName(stage::allStages[Stage_Zoras_Domain]))
        {
            if (tools::checkItemFlag(ItemFlags::Asheis_Sketch))
            {
                itemWheel->Story = items::Item::Asheis_Sketch;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Kakariko_Interiors]) && tp::d_kankyo::env_light.currentRoom == 0)
        {
            if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
            {
                itemWheel->Story = items::Item::Ilias_Charm;
            }
            else if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
            {
                itemWheel->Story = items::Item::Wooden_Statue;
            }
            else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
            {
                itemWheel->Story = items::Item::Renardos_Letter;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Outside_Castle_Town]) && tp::d_kankyo::env_light.currentRoom == 3)
        {
            if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
            {
                itemWheel->Story = items::Item::Wooden_Statue;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town_Shops]) && tp::d_kankyo::env_light.currentRoom == 2)
        {
            if (tools::checkItemFlag(ItemFlags::Invoice))
            {
                itemWheel->Story = items::Item::Invoice;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town_Interiors]) && tp::d_kankyo::env_light.currentRoom == 5)
        {
            if (tools::checkItemFlag(ItemFlags::Invoice))
            {
                itemWheel->Story = items::Item::Invoice;
            }
            else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
            {
                itemWheel->Story = items::Item::Renardos_Letter;
            }
        }
        else if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Hidden_Village]) || tp::d_a_alink::checkStageName(stage::allStages[Stage_Impaz_House]))
        {
            if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
            {
                itemWheel->Story = items::Item::Ilias_Charm;
            }
        }
        else
        {
            if (tools::checkItemFlag(ItemFlags::Horse_Call))
            {//finished Ilia Quest
                itemWheel->Story = items::Item::NullItem;
            }
            else if (tools::checkItemFlag(ItemFlags::Ilias_Charm))
            {
                itemWheel->Story = items::Item::Ilias_Charm;
            }
            else if (tools::checkItemFlag(ItemFlags::Wooden_Statue))
            {
                itemWheel->Story = items::Item::Wooden_Statue;
            }
            else if (tools::checkItemFlag(ItemFlags::Invoice))
            {
                itemWheel->Story = items::Item::Invoice;
            }
            else if (tools::checkItemFlag(ItemFlags::Renardos_Letter))
            {
                itemWheel->Story = items::Item::Renardos_Letter;
            }
            
            if (tools::checkItemFlag(ItemFlags::Coral_Earring))
            {//given sketch
                itemWheel->Story = items::Item::NullItem;
            }
            else if (tools::checkItemFlag(ItemFlags::Asheis_Sketch))
            {
                itemWheel->Story = items::Item::Asheis_Sketch;
            }
            
            if (gameInfo.scratchPad.movingActors.exploredRegions.Desert == 0b1)
            {//given memo
                itemWheel->Story = items::Item::NullItem;
            }
            else if (tools::checkItemFlag(ItemFlags::Aurus_Memo))
            {
                itemWheel->Story = items::Item::Aurus_Memo;
            }
            
        }
    }
    
    void Mod::allowShopItemsAnytime()
    {    
        tp::d_com_inf_game::ItemSlots* itemWheel = &gameInfo.scratchPad.itemWheel;
        
        tp::d_com_inf_game::Ammo* ammo = &gameInfo.scratchPad.ammo;
    
        float linkPos[3];
        getPlayerPos(linkPos);
    
        u8 hasEmptyBottleAlready = 1;
        if (itemWheel->Bottle_1 != items::Item::Empty_Bottle && itemWheel->Bottle_2 != items::Item::Empty_Bottle && 
        itemWheel->Bottle_3 != items::Item::Empty_Bottle && itemWheel->Bottle_4 != items::Item::Empty_Bottle)
        {
            hasEmptyBottleAlready = 0;
        }
    
        if (tp::d_a_alink::checkStageName("R_SP109") && tp::d_kankyo::env_light.currentRoom == 3)
        {
            game_patch::handleMaloShop();
        }
    
        if (isStageShop())
        {
            if ((tp::d_a_alink::checkStageName("R_SP160") && tp::d_kankyo::env_light.currentRoom == 4) || 
            (tp::d_a_alink::checkStageName("F_SP108") && tp::d_kankyo::env_light.currentRoom == 4) || 
            (tp::d_a_alink::checkStageName("F_SP116") && (tp::d_kankyo::env_light.currentRoom == 0 || tp::d_kankyo::env_light.currentRoom == 3)))
            {//Coro shop/castle goron shop
                if (gameInfo.aButtonText == 0x1C)
                {//about to speak to merchant
                    if (allowBottleItemsShopAnytime == 1 && hasEmptyBottleAlready == 0)
                    {
                        if (itemWheel->Bottle_4 != items::Item::Empty_Bottle)
                        {
                            bottle4Contents = itemWheel->Bottle_4;
                        }
                        itemWheel->Bottle_4 = items::Item::Empty_Bottle;
                        bottleTrickOn = 1;
                    }
                    if (tools::checkItemFlag(ItemFlags::Hylian_Shield))
                    {
                        hadHShield = 1;
                        tools::clearItemFlag(ItemFlags::Hylian_Shield);
                        shieldTrickOn = 1;
                    }
                }
                if (gameInfo.aButtonText == 0x22)
                {//selecting if you wanna buy or not
                    if (bottleTrickOn == 1)
                    {
                        bottleTrickOn = 2;
                    }
                    if (shieldTrickOn == 1)
                    {
                        shieldTrickOn = 2;
                    }
                }
                if (gameInfo.aButtonText == 0x23)
                {
                    if (bottleTrickOn == 2)
                    {
                        itemWheel->Bottle_4 = bottle4Contents;
                        bottleTrickOn = 0;
                    }
                    if (shieldTrickOn == 2)
                    {
                        if (hadHShield == 1)
                        {
                            tools::setItemFlag(ItemFlags::Hylian_Shield);
                        }
                        shieldTrickOn = 0;
                    }
                }
                if (gameInfo.aButtonText == 0x6 || gameInfo.aButtonText == 0x79)
                {//leaving
                    if (bottleTrickOn >= 1)
                    {
                        itemWheel->Bottle_4 = bottle4Contents;
                        bottleTrickOn = 0;
                    }
                    if (shieldTrickOn >= 1)
                    {
                        if (hadHShield == 1)
                        {
                            tools::setItemFlag(ItemFlags::Hylian_Shield);
                        }
                        shieldTrickOn = 0;
                    }
                }
            }
            else
            {//normal shops
                if (gameInfo.bButtonText == 0x2A)
                {//is in shop and is selecting an item
                    if (allowBottleItemsShopAnytime == 1 && hasEmptyBottleAlready == 0)
                    {
                        if (itemWheel->Bottle_4 != items::Item::Empty_Bottle)
                        {
                            bottle4Contents = itemWheel->Bottle_4;
                        }
                        itemWheel->Bottle_4 = items::Item::Empty_Bottle;
                        bottleTrickOn = 1;
                    }
                    if (tools::checkItemFlag(ItemFlags::Hylian_Shield))
                    {
                        hadHShield = 1;
                        tools::clearItemFlag(ItemFlags::Hylian_Shield);
                        shieldTrickOn = 1;
                    }
                    if (tools::checkItemFlag(ItemFlags::Wooden_Shield))
                    {
                        hadWShield = 1;
                        tools::clearItemFlag(ItemFlags::Wooden_Shield);
                        shieldTrickOn = 1;
                    }
                    if (tools::checkItemFlag(ItemFlags::Ordon_Shield))
                    {
                        hadOShield = 1;
                        tools::clearItemFlag(ItemFlags::Ordon_Shield);
                        shieldTrickOn = 1;
                    }
                    if (!tools::checkItemFlag(ItemFlags::Null_DA) && bombBagTrickOn == 0 && tp::d_a_alink::checkStageName("R_SP109") && tp::d_kankyo::env_light.currentRoom == 1 &&
                    linkPos[2] > -600)
                    {
                        bombBag1Contents = itemWheel->Bomb_Bag_1;
                        bombBag2Contents = itemWheel->Bomb_Bag_2;
                        bombBag3Contents = itemWheel->Bomb_Bag_3;
                        bombBag1Ammo = ammo->bombs1;
                        bombBag2Ammo = ammo->bombs2;
                        bombBag3Ammo = ammo->bombs3;
                        itemWheel->Bomb_Bag_1 = 0xFF;
                        itemWheel->Bomb_Bag_2 = 0xFF;
                        itemWheel->Bomb_Bag_3 = 0xFF;
                        bombBagTrickOn = 1;
                    }
                }
                else if (gameInfo.aButtonText == 0x23)
                {//is in shop and is exiting the item select mode
                    if (bottleTrickOn == 1)
                    {
                        itemWheel->Bottle_4 = bottle4Contents;
                        bottleTrickOn = 0;
                    }
                    if (shieldTrickOn == 1)
                    {
                        if (hadHShield == 1)
                        {
                            tools::setItemFlag(ItemFlags::Hylian_Shield);
                        }
                        if (hadWShield == 1)
                        {
                            tools::setItemFlag(ItemFlags::Wooden_Shield);
                        }
                        if (hadOShield == 1)
                        {
                            tools::setItemFlag(ItemFlags::Ordon_Shield);
                        }
                        shieldTrickOn = 0;
                    }
                    if (bombBagTrickOn == 1)
                    {
                        itemWheel->Bomb_Bag_1 = bombBag1Contents;
                        itemWheel->Bomb_Bag_2 = bombBag2Contents;
                        itemWheel->Bomb_Bag_3 = bombBag3Contents;
                        ammo->bombs1 = bombBag1Ammo;
                        ammo->bombs2 = bombBag2Ammo;
                        ammo->bombs3 = bombBag3Ammo;
                        bombBagTrickOn = 0;
                    }
                }
                if (tools::checkItemFlag(ItemFlags::Null_DA) && bombBagTrickOn == 1)
                {//bought bomb bag check
                    itemWheel->Bomb_Bag_1 = bombBag1Contents;
                    itemWheel->Bomb_Bag_2 = bombBag2Contents;
                    itemWheel->Bomb_Bag_3 = bombBag3Contents;
                    ammo->bombs1 = bombBag1Ammo;
                    ammo->bombs2 = bombBag2Ammo;
                    ammo->bombs3 = bombBag3Ammo;
                    bombBagTrickOn = 0;
                }
            }
        }
        else
        {
            if (bottleTrickOn >= 1)
            {
                itemWheel->Bottle_4 = bottle4Contents;
                bottleTrickOn = 0;
            }
            if (shieldTrickOn >= 1)
            {
                if (hadHShield == 1)
                {
                    tools::setItemFlag(ItemFlags::Hylian_Shield);
                }
                if (hadWShield == 1)
                {
                    tools::setItemFlag(ItemFlags::Wooden_Shield);
                }
                if (hadOShield == 1)
                {
                    tools::setItemFlag(ItemFlags::Ordon_Shield);
                }
                shieldTrickOn = 0;
            }
            if (bombBagTrickOn >= 1)
            {
                itemWheel->Bomb_Bag_1 = bombBag1Contents;
                itemWheel->Bomb_Bag_2 = bombBag2Contents;
                itemWheel->Bomb_Bag_3 = bombBag3Contents;
                ammo->bombs1 = bombBag1Ammo;
                ammo->bombs2 = bombBag2Ammo;
                ammo->bombs3 = bombBag3Ammo;
                bombBagTrickOn = 0;
            }
        }
    }
    
    void Mod::reorderItemWheel()
    {
        u8 currentSlot = 0x0;
        u8* itemSlotsOrder = gameInfo.scratchPad.itemSlotsOrder;
        tp::d_com_inf_game::ItemSlots* itemWheel = &gameInfo.scratchPad.itemWheel;
        if (tools::checkItemFlag(ItemFlags::Clawshots))
        {
            itemWheel->Double_Clawshot = items::Item::Clawshots;
            *(itemSlotsOrder + currentSlot) = 0xA;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Dominion_Rod))
        {
            itemWheel->Dominion_Rod = items::Item::Dominion_Rod;
            *(itemSlotsOrder + currentSlot) = 0x8;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Ball_and_Chain))
        {
            itemWheel->Ball_and_Chain = items::Item::Ball_and_Chain;
            *(itemSlotsOrder + currentSlot) = 0x6;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Spinner))
        {
            itemWheel->Spinner = items::Item::Spinner;
            *(itemSlotsOrder + currentSlot) = 0x2;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Clawshot))
        {
            itemWheel->Clawshot = items::Item::Clawshot;
            *(itemSlotsOrder + currentSlot) = 0x9;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Heros_Bow))
        {
            itemWheel->Bow = items::Item::Heros_Bow;
            *(itemSlotsOrder + currentSlot) = 0x4;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Iron_Boots))
        {
            itemWheel->Iron_Boots = items::Item::Iron_Boots;
            *(itemSlotsOrder + currentSlot) = 0x3;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Boomerang))
        {
            itemWheel->Boomerang = items::Item::Boomerang;
            *(itemSlotsOrder + currentSlot) = 0x0;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Lantern))
        {
            itemWheel->Lantern = items::Item::Lantern;
            *(itemSlotsOrder + currentSlot) = 0x1;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Slingshot))
        {
            itemWheel->Slingshot = items::Item::Slingshot;
            *(itemSlotsOrder + currentSlot) = 0x17;
            currentSlot++;
        }
        if (itemWheel->Fishing_Rod != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x14;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Hawkeye))
        {
            itemWheel->Hawkeye = items::Item::Hawkeye;
            *(itemSlotsOrder + currentSlot) = 0x5;
            currentSlot++;
        }
        if (itemWheel->Bomb_Bag_1 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0xF;
            currentSlot++;
        }
        if (itemWheel->Bomb_Bag_2 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x10;
            currentSlot++;
        }
        if (itemWheel->Bomb_Bag_3 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x11;
            currentSlot++;
        }
        if (itemWheel->Bottle_1 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0xB;
            currentSlot++;
        }
        if (itemWheel->Bottle_2 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0xC;
            currentSlot++;
        }
        if (itemWheel->Bottle_3 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0xD;
            currentSlot++;
        }
        if (itemWheel->Bottle_4 != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0xE;
            currentSlot++;
        }
        if (itemWheel->Story != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x13;
            currentSlot++;
        }
        if (itemWheel->Ooccoo != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x12;
            currentSlot++;
        }
        if (itemWheel->Sky_Book != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x16;
            currentSlot++;
        }
        if (tools::checkItemFlag(ItemFlags::Horse_Call))
        {
            itemWheel->Horse_Call = items::Item::Horse_Call;
            *(itemSlotsOrder + currentSlot) = 0x15;
            currentSlot++;
        }
        if (itemWheel->Item_Slot != 0xFF)
        {
            *(itemSlotsOrder + currentSlot) = 0x7;
            currentSlot++;
        }
        
        for(u16 i = currentSlot; i < sizeof(gameInfo.scratchPad.itemSlotsOrder)/sizeof(u8); i++)
        {
            *(itemSlotsOrder + i) = 0xFF;
        }
    }
    
    void Mod::fixYetaAndYeto()
    {
        if (tools::checkItemFlag(ItemFlags::Bed_Key) && tp::d_a_alink::checkStageName("D_MN11"))
        {
            float linkPos[3];
            getPlayerPos(linkPos);
            
            if (gameInfo.aButtonText == 0x6 && (tp::d_kankyo::env_light.currentRoom == 0 || tp::d_kankyo::env_light.currentRoom == 1 ||
            tp::d_kankyo::env_light.currentRoom == 2 || tp::d_kankyo::env_light.currentRoom == 3 || tp::d_kankyo::env_light.currentRoom == 4 ||
            tp::d_kankyo::env_light.currentRoom == 7) && linkPos[1] == 0)
            {
                gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b0;
                yetaTrickOn = 1;
            }
            if (gameInfo.aButtonText == 0x79 && yetaTrickOn == 1)
            {
                gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b1;
                yetaTrickOn = 0;
            }
        }
    }
    
    void Mod::fixLBTBossDoor()
    {
        if (tp::d_a_alink::checkStageName("D_MN01") && tp::d_kankyo::env_light.currentRoom == 3)
        {
            float linkPos[3];
            getPlayerPos(linkPos);
            if (gameInfo.aButtonText == 0x6 && linkPos[1] >= -340 && linkPos[1] <= -320)
            {
                nbLBTKeys = gameInfo.localAreaNodes.nbKeys;
                LBTBossDoorTrickOn = 1;
            }
            if (gameInfo.aButtonText == 0x79 && LBTBossDoorTrickOn == 1)
            {
                gameInfo.localAreaNodes.nbKeys = nbLBTKeys;
                LBTBossDoorTrickOn = 0;
            }
        }
    }
    
    void Mod::preventPoweringUpDomRod()
    {    
        if (gameInfo.scratchPad.itemWheel.Sky_Book == 0xFF && tools::checkItemFlag(ItemFlags::Ancient_Sky_Book_empty) && !tools::checkItemFlag(ItemFlags::Ancient_Sky_Book_partly_filled))
        {
            gameInfo.scratchPad.itemWheel.Sky_Book = items::Item::Ancient_Sky_Book_empty;
        }
        if (tp::d_a_alink::checkStageName("R_SP209"))
        {
            if (gameInfo.scratchPad.itemWheel.Sky_Book == items::Item::Ancient_Sky_Book_empty)
            {
                gameInfo.scratchPad.itemWheel.Sky_Book = 0xFF;
            }
        }
    }
    
    bool Mod::isStageShop()
    {
        u8 totalShopStages = sizeof(stage::shopStages) / sizeof(stage::shopStages[0]);
        for (u8 i = 0; i < totalShopStages; i++)
        {
            if (tp::d_a_alink::checkStageName(stage::shopStages[i]))
            {
                return true;
            }
        }
        return false;
    }
    
    void Mod::doCustomTRESActor(void* mStatus_roomControl)
    {
        tp::d_com_inf_game::GameInfo* pGameInfo = &tp::d_com_inf_game::dComIfG_gameInfo;

        u32 numChecks = sizeof(customChecks) / sizeof(customChecks[0]);

        // Count and temp array for the checks for this stage+room
        u32 checkCount = 0;
        customCheck* checks = new customCheck[5];  // Assume there's never more than 5 custom checks per stage+room

        // Loop through checks and place if correct room and stage
        for (u32 i = 0; i < numChecks; i++)
        {
            customCheck* check = &customChecks[i];

            if (0 == strcmp(pGameInfo->nextStageVars.nextStage, check->stage) &&
                (0xFF == check->room || pGameInfo->nextStageVars.nextRoom == check->room) && // 0xFF = no need to check for room
                check->requirement())
            {
                checks[checkCount] = *check;
                checkCount++;
            }
        }

        if (checkCount > 0)
        {
            // Create required structs
            tp::d_stage::TRES* TRES = new tp::d_stage::TRES[checkCount];
            tp::d_stage::dzxChunkTypeInfo chunkInfo;
            strcpy(chunkInfo.tag, "ACTR");  // has to be ACTR for the function we use
            chunkInfo.numChunks = checkCount;
            chunkInfo.chunkDataPtr = TRES;

            // Populate TRES array with data
            for (u32 i = 0; i < checkCount; i++)
            {
                customCheck check = checks[i];
                
                check.overrides();

                strcpy(TRES[i].actorName, "tboxA0\0");
                TRES[i].flags = 0xFF0FF000 | (check.chestType << 20) | (check.saveFlag << 4);

                // Translate hex to float (1:1)
                typeTransform<u32, float> X, Y, Z;
                X.a = check.X;
                Y.a = check.Y;
                Z.a = check.Z;

                TRES[i].X = X.b;
                TRES[i].Y = Y.b;
                TRES[i].Z = Z.b;

                TRES[i].angle = check.Angle;

                TRES[i].item = check.itemID;
            }

            // Create the actors; last 2 params 0 and nullptr to avoid infinite loop! (identification for self-call inside the
            // hook)
            tp::d_stage::actorCommonLayerInit(mStatus_roomControl, &chunkInfo, 0, nullptr);

            delete[] TRES;
        }

        delete[] checks;
        return;
    }    
} // namespace mo