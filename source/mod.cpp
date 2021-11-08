#include "mod.h"

#include <tp/DynamicLink.h>
#include <tp/JFWSystem.h>
#include <tp/d_a_alink.h>
#include <tp/d_com_inf_game.h>
#include <tp/d_item.h>
#include <tp/d_item_data.h>
#include <tp/d_kankyo.h>
#include <tp/d_map_path_dmap.h>
#include <tp/d_meter2_info.h>
#include <tp/d_msg_class.h>
#include <tp/d_msg_flow.h>
#include <tp/d_save.h>
#include <tp/d_stage.h>
#include <tp/dzx.h>
#include <tp/evt_control.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/f_op_scene_req.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/processor.h>
#include <tp/resource.h>

#include <cstdio>
#include <cstring>

#include "HUDConsole.h"
#include "array.h"
#include "chestRando.h"
#include "controller.h"
#include "customChecks.h"
#include "customMessage.h"
#include "defines.h"
#include "eventListener.h"
#include "game_patches.h"
#include "global.h"
#include "itemChecks.h"
#include "items.h"
#include "musicRando.h"
#include "patch.h"
#include "singleton.h"
#include "stage.h"
#include "systemConsole.h"
#include "tools.h"

namespace mod
{
    Mod* global::modPtr = nullptr;
    ChestRandomizer* global::chestRandoPtr = nullptr;
    event::EventListener* global::eventListenerPtr = nullptr;
    mod::HUDConsole* global::hudConsolePtr = nullptr;
    uint64_t* global::seedInSaveFile = nullptr;

    int num_frames = 120;
    int frame_counter = 0;

    tp::d_stage::Actr EponaActr = { "Horse", 0x00000F0D, 0.f, 0.f, 0.f, 0, -180, 0, 0xFFFF };

    void main()
    {
        Mod* mod = new Mod();
        mod->init();
    }

    Mod::Mod()
    {
        eventListener = new event::EventListener();
        chestRandomizer = new ChestRandomizer();
        hudConsole = new HUDConsole( "Settings", 0x80 );
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
        game_patch::setCustomItemData();
        game_patch::setCustomItemFunctions();

        // Causes issues right now (argarok cannot be beaten)
        // game_patch::removeIBLimit();

        // Init rando
        tools::randomSeed = 0x9e3779b97f4a7c15;

        global::seedInSaveFile = reinterpret_cast<u64*>( &tp::d_com_inf_game::dComIfG_gameInfo.scratchPad.eponaName[9] );

        randoEnabled = 1;
        truePause = 1;
        inputBuffering = 0;

        // Print controls
        strcpy( sysConsolePtr->consoleLine[20].line, "D-Pad Nav: Up/Down  Left/Right" );
        strcpy( sysConsolePtr->consoleLine[21].line, "+/- Value: A/B/X/Y  Console  :  R + Z" );
        strcpy( sysConsolePtr->consoleLine[22].line, " Generate: R + Start (auto on new file)" );
        strcpy( sysConsolePtr->consoleLine[23].line, "Bring up the console to use commands" );
        strcpy( sysConsolePtr->consoleLine[24].line, "rando.zeldatp.net | Twitter: @tprandomizer" );

        u8 page = 0;

        // General settings
        hudConsole->addOption( page, "Red:", &reinterpret_cast<u8*>( &sysConsolePtr->consoleColor )[0], 0xFF );
        hudConsole->addOption( page, "Green:", &reinterpret_cast<u8*>( &sysConsolePtr->consoleColor )[1], 0xFF );
        hudConsole->addOption( page, "Blue:", &reinterpret_cast<u8*>( &sysConsolePtr->consoleColor )[2], 0xFF );
        hudConsole->addOption( page, "Alpha:", &reinterpret_cast<u8*>( &sysConsolePtr->consoleColor )[3], 0xFF );
        hudConsole->addOption( page, "True Pause:", &truePause, 0x1 );
        hudConsole->addOption( page, "Input Buffer:", &inputBuffering, 0x1 );

        // Seed settings
        page = hudConsole->addPage( "Seed" );

        hudConsole->addOption( page, "Rando enabled?", &randoEnabled, 0x1 );
        hudConsole->addOption( page, "Custom Seed?", &customSeed, 0x1 );
        hudConsole->addOption( page, "Seed 1:", &reinterpret_cast<u8*>( &tools::randomSeed )[0], 0xFF );
        hudConsole->addOption( page, "Seed 2:", &reinterpret_cast<u8*>( &tools::randomSeed )[1], 0xFF );
        hudConsole->addOption( page, "Seed 3:", &reinterpret_cast<u8*>( &tools::randomSeed )[2], 0xFF );
        hudConsole->addOption( page, "Seed 4:", &reinterpret_cast<u8*>( &tools::randomSeed )[3], 0xFF );

        hudConsole->addOption( page, "Seed 5:", &reinterpret_cast<u8*>( &tools::randomSeed )[4], 0xFF );
        hudConsole->addOption( page, "Seed 6:", &reinterpret_cast<u8*>( &tools::randomSeed )[5], 0xFF );
        hudConsole->addOption( page, "Seed 7:", &reinterpret_cast<u8*>( &tools::randomSeed )[6], 0xFF );
        hudConsole->addOption( page, "Seed 8:", &reinterpret_cast<u8*>( &tools::randomSeed )[7], 0xFF );

        hudConsole->addWatch( page, "Resulting Seed", &tools::randomSeed, 'x', WatchInterpretation::_u64 );

        // Debug
        page = hudConsole->addPage( "Debug Info" );

        hudConsole->addWatch( page, "InSave: ", global::seedInSaveFile, 'x', WatchInterpretation::_u64 );
        hudConsole->addWatch( page, "Function:", &lastItemFunc, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "  Source:", &chestRandomizer->lastSourceInfo, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "    Dest:", &chestRandomizer->lastDestInfo, 's', WatchInterpretation::_str );

        hudConsole->addWatch( page, "Total Checks:", &chestRandomizer->totalChecks, 'u', WatchInterpretation::_u16 );
        hudConsole->addWatch( page, "Layer Checks:", &chestRandomizer->layerCheckCount, 'u', WatchInterpretation::_u16 );
        hudConsole->addWatch( page, "Empty:", &chestRandomizer->empty, 'u', WatchInterpretation::_u16 );

        hudConsole->addWatch( page, "Active Seed:", &chestRandomizer->currentSeed, 'x', WatchInterpretation::_u64 );
        hudConsole->addWatch( page, "   Checksum:", &chestRandomizer->checkSum, 'x', WatchInterpretation::_u16 );

        // Item search
        page = hudConsole->addPage( "Item Search" );

        hudConsole->addOption( page, "Search ID:", &itemSearchID, 0xFF );
        hudConsole->addOption( page, "Reverse ID:", &itemReverseSearchID, 0xFF );

        hudConsole->addWatch( page, "Search Result:", &itemSearchResults, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "Reverse Result:", &itemReverseSearchResults, 's', WatchInterpretation::_str );

        hudConsole->addWatch( page, "MetadataID:", &lastItemDataID, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "MetadataX:", &lastItemDataX, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "MetadataY:", &lastItemDataY, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "MetadataZ:", &lastItemDataZ, 's', WatchInterpretation::_str );

        // Check search
        page = hudConsole->addPage( "Check Search" );

        hudConsole->addOption( page, "Search ID1:", &checkSearchID1, 0xFF );
        hudConsole->addOption( page, "Search ID2:", &checkSearchID2, 0xFF );
        hudConsole->addOption( page, "Reverse ID1:", &checkReverseSearchID1, 0xFF );
        hudConsole->addOption( page, "Reverse ID2:", &checkReverseSearchID2, 0xFF );

        hudConsole->addWatch( page, "Search ID:", &checkSearchID, 'd', WatchInterpretation::_u16 );
        hudConsole->addWatch( page, "Reverse ID:", &checkReverseSearchID, 'd', WatchInterpretation::_u16 );
        hudConsole->addWatch( page, "Search Result:", &checkSearchResults, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "Reverse Result:", &checkReverseSearchResults, 's', WatchInterpretation::_str );

        // Shuffled Checks
        page = hudConsole->addPage( "Shuffled Checks" );

        hudConsole->addOption( page, "Bugsanity?", &chestRandomizer->isBugsanityEnabled, 0x1 );
        hudConsole->addOption( page, "Poesanity?", &chestRandomizer->isPoesanityEnabled, 0x1 );
        hudConsole->addOption( page, "Shopsanity?", &chestRandomizer->isShopsanityEnabled, 0x1 );
        hudConsole->addOption( page, "Dungeon Items?", &chestRandomizer->areDungeonItemsRandomized, 0x1 );
        hudConsole->addOption( page, "Key Shuffle?", &chestRandomizer->isKeysanityEnabled, 0x1 );
        hudConsole->addOption( page, "Sky Character?", &Singleton::getInstance()->shuffledSkybook, 0x1 );

        // Game info 1
        page = hudConsole->addPage( "Skips 1" );

        hudConsole->addOption( page, "MDH Skip?", &Singleton::getInstance()->isMDHSkipEnabled, 0x1 );
        hudConsole->addOption( page, "Faron Escape?", &Singleton::getInstance()->isForestEscapeEnabled, 0x1 );
        hudConsole->addOption( page, "Open HF gates?", &Singleton::getInstance()->isGateUnlockEnabled, 0x1 );
        hudConsole->addOption( page, "Skip Twilight?", &Singleton::getInstance()->isTwilightSkipped, 0x1 );
        hudConsole->addOption( page, "Skip Goats?", &Singleton::getInstance()->isGoatSkipEnabled, 0x1 );
        hudConsole->addOption( page, "Skip MS Puzzle?", &Singleton::getInstance()->isMSPuzzleSkipEnabled, 0x1 );
        hudConsole->addOption( page, "Skip Escort?", &Singleton::getInstance()->isCartEscortSkipEnabled, 0x1 );
        hudConsole->addOption( page, "Skip Sewers?", &Singleton::getInstance()->isSewerSkipEnabled, 0x1 );

        hudConsole->addWatch( page, "CurrentStage:", &gameInfo.currentStage, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "CurrentRoom:", &tp::d_kankyo::env_light.currentRoom, 'd', WatchInterpretation::_u8 );
        hudConsole->addWatch( page, "CurrentState:", &tp::d_com_inf_game::current_state, 'x', WatchInterpretation::_u8 );

        hudConsole->addWatch( page, "CurrentPosX:", &currentPosX, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "CurrentPosY:", &currentPosY, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "CurrentPosZ:", &currentPosZ, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "CurrentAngle:", &linkAngle, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "Sky Angle:", &skyAngle, 's', WatchInterpretation::_str );

        // Game info 2
        page = hudConsole->addPage( "Skips 2" );
        hudConsole->addOption( page, "Early CiTS?", &Singleton::getInstance()->isEarlyCiTSEnabled, 0x1 );
        hudConsole->addOption( page, "Early Desert?", &Singleton::getInstance()->isEarlyDesertEnabled, 0x1 );
        hudConsole->addOption( page, "Boss Keysey?", &Singleton::getInstance()->isBossKeyseyEnabled, 0x1 );
        hudConsole->addOption( page, "No Shop Bottl?", &allowBottleItemsShopAnytime, 0x1 );
        hudConsole->addOption( page, "Fast transform?", &enableQuickTransform, 0x1 );
        hudConsole->addOption( page, "Skip Intro?", &Singleton::getInstance()->isIntroSkipped, 0x1 );
        hudConsole->addOption( page, "Midna ToD Skip?", &Singleton::getInstance()->midnaTimeControl, 0x1 );
        hudConsole->addOption( page, "Early ToT?", &Singleton::getInstance()->isEarlyToTEnabled, 0x1 );
        hudConsole->addOption( page, "Early PoT?", &Singleton::getInstance()->isEarlyPoTEnabled, 0x1 );
        hudConsole->addOption( page, "Open HC?", &Singleton::getInstance()->isEarlyHCEnabled, 0x1 );

        // buttons
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
        hudConsole->addWatch(page, "C button:", &gameInfo.cButtonText, 'x', WatchInterpretation::_u8);
        hudConsole->addWatch(page, "scoop:", &scoopResult, 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "check:", &checkResult, 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "items:", &itemsResult, 'x', WatchInterpretation::_u16);
        hudConsole->addWatch(page, "map:", &mapResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "equip:", &equipResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "back:", &backResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "zoom in:", &zoomInResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "zoom out:", &zoomOutResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "move:", &moveResult, 'x', WatchInterpretation::_u16);
hudConsole->addWatch(page, "throw:", &throwResult, 'x', WatchInterpretation::_u16);*/

        // event info
        page = hudConsole->addPage( "Skips 3/ Event" );
        // hudConsole->addOption(page, "Coords as hex?", &coordsAreInHex, 0x1);
        hudConsole->addOption( page, "GM Story Flag?", &Singleton::getInstance()->isGMStoryPatch, 0x1 );
        hudConsole->addOption( page, "Start w/ Crstl?", &Singleton::getInstance()->startWithCrystal, 0x1 );
        hudConsole->addOption( page, "Hidden Skills?", &Singleton::getInstance()->shuffleHiddenSkills, 0x1 );

        hudConsole->addWatch( page, "CurrentEventID:", &gameInfo.eventSystem.currentEventID, 'x', WatchInterpretation::_u8 );
        hudConsole->addWatch( page, "NextEventID:", &gameInfo.eventSystem.nextEventID, 'x', WatchInterpretation::_u8 );
        hudConsole->addWatch( page, "NextStage:", &gameInfo.nextStageVars.nextStage, 's', WatchInterpretation::_str );
        hudConsole->addWatch( page, "NextRoom:", &gameInfo.nextStageVars.nextRoom, 'd', WatchInterpretation::_u8 );
        hudConsole->addWatch( page, "NextSpawnPoint:", &gameInfo.nextStageVars.nextSpawnPoint, 'x', WatchInterpretation::_u8 );
        hudConsole->addWatch( page, "NextSate:", &gameInfo.nextStageVars.nextState, 'x', WatchInterpretation::_u8 );

        // Cosmetic Changes
        page = hudConsole->addPage( "Cosmetic" );
        hudConsole->addOption( page, "LTN In Rd:", &innerRed, 0xFF );
        hudConsole->addOption( page, "LTN In Green:", &innerGreen, 0xFF );
        hudConsole->addOption( page, "LTN In Blue:", &innerBlue, 0xFF );
        hudConsole->addOption( page, "LTN Ot Red:", &outerRed, 0xFF );
        hudConsole->addOption( page, "LTN Ot Green:", &outerGreen, 0xFF );
        hudConsole->addOption( page, "LTN Ot Blue:", &outerBlue, 0xFF );

        // Bgm Options
        page = hudConsole->addPage( "BGM" );
        hudConsole->addOption( page, "BGM Rando: ", &musicrando::musicRandoEnabled, 0x1 );
        hudConsole->addOption( page, "No Enemy BGM?: ", &musicrando::enemyBgmDisabled, 0x1 );
        hudConsole->addOption( page, "Item Fanfares: ", &musicrando::fanfareRandoEnabled, 0x1 );

        // Print
        hudConsole->draw();
        system_console::setState( true, 0 );

        //   =================
        //  | Custom events   |
        //   =================

        // Generate random data when a new game starts
        eventListener->addLoadEvent( stage::allStages[Stage_Faron_Woods],
                                     0x1,
                                     0x15,
                                     0xFF,
                                     0xFF,
                                     tools::triggerRandomGenerator,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Skip sewers when the load happens
        eventListener->addLoadEvent( stage::allStages[Stage_Hyrule_Castle_Sewers],
                                     0x0,
                                     0x18,
                                     0xFF,
                                     0xFF,
                                     game_patch::skipSewers,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Fix BiTE
        // eventListener->addLoadEvent(stage::allStages[Stage_Title_Screen], 0x0, 0x17, 0xFF, 0xFF, game_patch::giveEpona,
        // event::LoadEventAccuracy::Stage_Room_Spawn);

        // Kill spider at Link's house
        eventListener->addLoadEvent( stage::allStages[Stage_Ordon_Village],
                                     0x1,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::killLinkHouseSpider,
                                     event::LoadEventAccuracy::Stage_Room );

        // unlock HF gates and check for MDH
        eventListener->addLoadEvent( stage::allStages[Stage_Hyrule_Field],
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::setFieldBits,
                                     event::LoadEventAccuracy::Stage );

        // skip goats 2
        eventListener->addLoadEvent( stage::allStages[Stage_Ordon_Ranch],
                                     0x0,
                                     0x3,
                                     0xFF,
                                     0xFF,
                                     game_patch::skipGoats,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // skip MS Puzzle
        eventListener->addLoadEvent( stage::allStages[Stage_Sacred_Grove],
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::setGroveFlags,
                                     event::LoadEventAccuracy::Stage );

        // Fix Lanayru Softlock
        eventListener->addLoadEvent( stage::allStages[Stage_Lake_Hylia],
                                     0x0,
                                     0x5,
                                     0xE,
                                     0xFF,
                                     game_patch::setLanayruWolf,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // desert Access
        eventListener->addLoadEvent( stage::allStages[Stage_Gerudo_Desert],
                                     0x0,
                                     0x0,
                                     0xFF,
                                     0xFF,
                                     game_patch::accessDesert,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Skip Midna Text and CS's
        eventListener->addLoadEvent( stage::allStages[Stage_Faron_Woods],
                                     0x1,
                                     0x15,
                                     0xFF,
                                     0xFF,
                                     game_patch::skipTextAndCS,
                                     event::LoadEventAccuracy::Stage_Room_Spawn_State );

        // Allow Escort Any Time
        eventListener->addLoadEvent( stage::allStages[Stage_Castle_Town_Interiors],
                                     0x5,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::setEscortState,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Skip MDH Trigger
        eventListener->addLoadEvent( stage::allStages[Stage_Lake_Hylia],
                                     0x1,
                                     0x16,
                                     0xFF,
                                     0xFF,
                                     game_patch::skipMDHCS,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Allow Faron Escape
        eventListener->addLoadEvent( stage::allStages[Stage_Faron_Woods],
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::allowFaronEscape,
                                     event::LoadEventAccuracy::Stage );

        // Skip MDH After Lanayru
        eventListener->addLoadEvent( stage::allStages[Stage_Lake_Hylia],
                                     0x1,
                                     0x14,
                                     0xFF,
                                     0xFF,
                                     game_patch::skipMDH,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // set Coro Lantern Flag
        eventListener->addLoadEvent( stage::allStages[Stage_Faron_Woods],
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::setLanternFlag,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );

        // Break Barrier
        eventListener->addLoadEvent( stage::allStages[Stage_Castle_Town],
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     0xFF,
                                     game_patch::breakBarrier,
                                     event::LoadEventAccuracy::Stage_Room_Spawn );
        //   =================
        //  | Function Hooks  |
        //   =================

        fapGm_Execute_trampoline =
            patch::hookFunction( tp::f_ap_game::fapGm_Execute, []() { global::modPtr->procNewFrame(); } );

        // Hook actor init function to use as a timing for placing custom chests as this function only creates actors at
        // specific times
        actorCommonLayerInit_trampoline = patch::hookFunction(
            tp::d_stage::actorCommonLayerInit,
            []( void* mStatus_roomControl, tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4 ) {
                if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Faron_Woods] ) )
                {
                    if ( Singleton::getInstance()->hasActorCommonLayerRan <= 4 )
                    {
                        global::modPtr->doCustomTRESActor( mStatus_roomControl );
                    }
                }
                else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Hyrule_Field] ) &&
                          gameInfo.nextStageVars.nextSpawnPoint == 0xFC )
                {
                    if ( Singleton::getInstance()->hasActorCommonLayerRan <= 2 )
                    {
                        global::modPtr->doCustomTRESActor( mStatus_roomControl );
                    }
                }
                else
                {
                    global::modPtr->doCustomTRESActor( mStatus_roomControl );
                }
                return global::modPtr->actorCommonLayerInit_trampoline( mStatus_roomControl, chunkTypeInfo, unk3, unk4 );
            } );

        actorInit_always_trampoline = patch::hookFunction(
            tp::d_stage::actorInit_always,
            []( void* mStatus_roomControl, tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4 ) {
                if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Faron_Woods] ) )
                {
                    // Add Epona to the global Actor list
                    tp::d_stage::Actr* EponaActrPtr = &EponaActr;
                    tp::d_stage::dzxChunkTypeInfo chunkInfo;
                    strcpy( chunkInfo.tag, "ACTR" );
                    chunkInfo.numChunks = 0x1;
                    typeTransform<u32, float> X, Y, Z;
                    X.a = 0x0;
                    Y.a = 0x0;
                    Z.a = 0x0;
                    EponaActrPtr->pos[0] = X.b;
                    EponaActrPtr->pos[1] = Y.b;
                    EponaActrPtr->pos[2] = Z.b;
                    chunkInfo.chunkDataPtr = EponaActrPtr;
                    // Add the actor to the actor list
                    global::modPtr->actorInit_always_trampoline( mStatus_roomControl, &chunkInfo, 0, nullptr );
                }

                return global::modPtr->actorCommonLayerInit_trampoline( mStatus_roomControl, chunkTypeInfo, unk3, unk4 );
            } );

        putSave_trampoline =
            patch::hookFunction( tp::d_save::putSave, []( tp::d_com_inf_game::GameInfo* gameInfoPtr, s32 areaID ) {
                Singleton::getInstance()->hasActorCommonLayerRan = 0;
                return global::modPtr->putSave_trampoline( gameInfoPtr, areaID );
            } );

        checkTreasureRupeeReturn_trampoline = patch::hookFunction(
            tp::d_a_alink::checkTreasureRupeeReturn,
            []( void* unk1, s32 item ) { return global::modPtr->procCheckTreasureRupeeReturn( unk1, item ); } );

        createItemForPresentDemo_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForPresentDemo,
            []( const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3] ) {
                // Call replacement function
                /*char txt[50];
                snprintf(txt, 50, "0 = %f 1 = %f 2 = %f", pos[0], pos[1], pos[2]);
                strcpy(sysConsolePtr->consoleLine[20].line, txt);*/

                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForPresentDemo" );

                return global::modPtr->createItemForPresentDemo_trampoline( pos, item, unk3, unk4, unk5, unk6, unk7 );
            } );

        createItemForTrBoxDemo_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForTrBoxDemo,
            []( const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3] ) {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForTrBoxDemo" );

                return global::modPtr->createItemForTrBoxDemo_trampoline( pos, item, unk3, unk4, unk5, unk6 );
            } );
        // this function is called when the heart spawns, not when link gets it
        // createItemForTrBoxDemo is called when heart container is gotten
        createItemForBoss_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForBoss,
            []( const float pos[3],
                s32 item,
                s32 unk3,
                const float unk4[3],
                const float unk5[3],
                float unk6,
                float unk7,
                s32 unk8 ) {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForBoss" );

                return global::modPtr->createItemForBoss_trampoline( pos, item, unk3, unk4, unk5, unk6, unk7, unk8 );
            } );

        createItemForMidBoss_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForMidBoss,
            []( const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7 ) {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForMidBoss" );

                return global::modPtr->createItemForMidBoss_trampoline( pos, item, unk3, unk4, unk5, unk6, unk7 );
            } );

        createItemForDirectGet_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForDirectGet,
            []( const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7 ) {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForDirectGet" );

                return global::modPtr->createItemForDirectGet_trampoline( pos, item, unk3, unk4, unk5, unk6, unk7 );
            } );

        createItemForSimpleDemo_trampoline = patch::hookFunction(
            tp::f_op_actor_mng::createItemForSimpleDemo,
            []( const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7 ) {
                // Call replacement function
                item = global::modPtr->procItemCreateFunc( pos, item, "createItemForSimpleDemo" );

                return global::modPtr->createItemForSimpleDemo_trampoline( pos, item, unk3, unk4, unk5, unk6, unk7 );
            } );

        evt_control_Skipper_trampoline = patch::hookFunction( tp::evt_control::skipper, []( void* evtPtr ) {
            return global::modPtr->procEvtSkipper( evtPtr );
        } );

        query022_trampoline = patch::hookFunction( tp::d_msg_flow::query022, []( void* unk1, void* unk2, s32 unk3 ) {
            return global::modPtr->proc_query022( unk1, unk2, unk3 );
        } );

        query023_trampoline = patch::hookFunction( tp::d_msg_flow::query023, []( void* unk1, void* unk2, s32 unk3 ) {
            return global::modPtr->proc_query023( unk1, unk2, unk3 );
        } );

        query024_trampoline = patch::hookFunction(
            tp::d_msg_flow::query024,
            []( void* dMsgFlow_cPtr, void* mesg_flow_node_branchPtr, void* fopAc_ac_cPtr, int unused ) {
                return global::modPtr->proc_query024( dMsgFlow_cPtr, mesg_flow_node_branchPtr, fopAc_ac_cPtr, unused );
            } );

        query025_trampoline = patch::hookFunction( tp::d_msg_flow::query025, []( void* unk1, void* unk2, s32 unk3 ) {
            return global::modPtr->proc_query025( unk1, unk2, unk3 );
        } );

        isEventBit_trampoline = patch::hookFunction( tp::d_save::isEventBit, []( u8* eventSystem, u16 indexNumber ) {
            return global::modPtr->proc_isEventBit( eventSystem, indexNumber );
        } );

        onEventBit_trampoline = patch::hookFunction( tp::d_save::onEventBit, []( u8* eventSystem, u16 indexNumber ) {
            if ( indexNumber == 0x1E08 && ( gameInfo.scratchPad.form == 1 ) )
            {
                gameInfo.scratchPad.form = 0;
            }
            return global::modPtr->onEventBit_trampoline( eventSystem, indexNumber );
        } );

        setGetItemFace_trampoline = patch::hookFunction( tp::d_a_alink::setGetItemFace, []( void* daAlink_c, u16 itemId ) {
            switch ( itemId )
            {
                case items::Shadow_Crystal:
                {
                    itemId = items::Clawshot;
                    break;
                }
                case items::Master_Sword:
                {
                    itemId = items::Clawshot;
                    break;
                }
                case items::Master_Sword_Light:
                {
                    itemId = items::Clawshot;
                    break;
                }
                default:
                {
                    return global::modPtr->setGetItemFace_trampoline( daAlink_c, itemId );
                }
            }
            return global::modPtr->setGetItemFace_trampoline( daAlink_c, itemId );
        } );

        do_link_trampoline = patch::hookFunction( tp::dynamic_link::do_link, []( tp::dynamic_link::DynamicModuleControl* dmc ) {
            return global::modPtr->procDoLink( dmc );
        } );

        item_func_UTUWA_HEART_trampoline = patch::hookFunction( tp::d_item::item_func_UTUWA_HEART,
                                                                []() { return global::modPtr->procItem_func_UTUWA_HEART(); } );

        setItemBombNumCount_trampoline =
            patch::hookFunction( tp::d_com_inf_game::setItemBombNumCount, []( u32 unk1, u8 bagNb, short amount ) {
                u8 bombtype = 0;
                if ( bagNb == 0 )
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_1;
                }
                else if ( bagNb == 1 )
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_2;
                }
                else if ( bagNb == 2 )
                {
                    bombtype = gameInfo.scratchPad.itemWheel.Bomb_Bag_3;
                }
                char txt[50];
                snprintf( txt, 50, "bag = %x amount = %d type = %x", bagNb, amount, bombtype );
                strcpy( sysConsolePtr->consoleLine[20].line, txt );
                return global::modPtr->setItemBombNumCount_trampoline( unk1, bagNb, amount );
            } );
        isDungeonItem_trampoline = patch::hookFunction( tp::d_save::isDungeonItem, []( void* memBitPtr, const int param_1 ) {
            return global::modPtr->proc_isDungeonItem( memBitPtr, param_1 );
        } );

        getLayerNo_common_common_trampoline = patch::hookFunction(
            tp::d_com_inf_game::getLayerNo_common_common,
            []( const char* stageName, int roomId, int layerOverride ) {
                int chosenLayer;
                bool condition;
                // Bublin Camp
                bool stageComparison = strcmp( stageName, "F_SP118" );
                if ( stageComparison == 0 )
                {
                    condition = tp::d_a_alink::dComIfGs_isEventBit( 0xb40 );     // Escaped Burning Tent in Bublin Camp
                    if ( condition != false )
                    {
                        if ( ( gameInfo.localAreaNodes.unk_0[0xB] & 0x10 ) == 0 )
                        {
                            chosenLayer = 0x1;
                        }
                        else
                        {
                            chosenLayer = 0x3;
                        }
                    }
                    else
                    {
                        chosenLayer = 0x0;
                    }
                    return chosenLayer;
                }
                stageComparison = strcmp( stageName, "F_SP117" );
                if ( ( stageComparison == 0 ) && ( Singleton::getInstance()->isEarlyToTEnabled == 1 ) )
                {
                    chosenLayer = 0x2;
                    return chosenLayer;
                }
                return global::modPtr->getLayerNo_common_common_trampoline( stageName, roomId, layerOverride );
            } );

        setMessageCode_inSequence_trampoline = patch::hookFunction(
            tp::control::setMessageCode_inSequence,
            []( tp::control::TControl* control, const void* TProcessor, u16 unk3, u16 msgId ) {
                // Call the original function immediately, as a lot of stuff needs to be set before our code runs
                const bool ret = global::modPtr->setMessageCode_inSequence_trampoline( control, TProcessor, unk3, msgId );

                // Make sure the function ran successfully
                if ( !ret )
                {
                    return ret;
                }

                // Check if the current text is for an item
                s32 index = global::modPtr->getItemIdFromMsgId( TProcessor, unk3, msgId );
                // The current text is for an item
                switch ( static_cast<u32>( index ) )
                {
                    const char* replacementText;
                    case items::Forest_Temple_Small_Key:
                    {
                        replacementText = customMessage::customForestSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Forest_Temple_Dungeon_Map:
                    {
                        replacementText = customMessage::customForestMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Forest_Temple_Compass:
                    {
                        replacementText = customMessage::customForestCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Forest_Temple_Big_Key:
                    {
                        replacementText = customMessage::customForestBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Goron_Mines_Small_Key:
                    {
                        replacementText = customMessage::customMinesSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Goron_Mines_Dungeon_Map:
                    {
                        replacementText = customMessage::customMinesMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Goron_Mines_Compass:
                    {
                        replacementText = customMessage::customMinesCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Lakebed_Temple_Small_Key:
                    {
                        replacementText = customMessage::customLakebedSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Lakebed_Temple_Dungeon_Map:
                    {
                        replacementText = customMessage::customLakebedMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Lakebed_Temple_Compass:
                    {
                        replacementText = customMessage::customLakebedCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Lakebed_Temple_Big_Key:
                    {
                        replacementText = customMessage::customLakebedBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Arbiters_Grounds_Small_Key:
                    {
                        replacementText = customMessage::customArbitersSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Arbiters_Grounds_Dungeon_Map:
                    {
                        replacementText = customMessage::customArbitersMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Arbiters_Grounds_Compass:
                    {
                        replacementText = customMessage::customArbitersCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Arbiters_Grounds_Big_Key:
                    {
                        replacementText = customMessage::customArbitersBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Snowpeak_Ruins_Small_Key:
                    {
                        replacementText = customMessage::customSnowpeakSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Snowpeak_Ruins_Dungeon_Map:
                    {
                        replacementText = customMessage::customSnowpeakMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Snowpeak_Ruins_Compass:
                    {
                        replacementText = customMessage::customSnowpeakCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Temple_of_Time_Small_Key:
                    {
                        replacementText = customMessage::customToTSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Temple_of_Time_Dungeon_Map:
                    {
                        replacementText = customMessage::customToTMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Temple_of_Time_Compass:
                    {
                        replacementText = customMessage::customToTCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Temple_of_Time_Big_Key:
                    {
                        replacementText = customMessage::customToTBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::City_in_The_Sky_Small_Key:
                    {
                        replacementText = customMessage::customCitySKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::City_in_The_Sky_Dungeon_Map:
                    {
                        replacementText = customMessage::customCityMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::City_in_The_Sky_Compass:
                    {
                        replacementText = customMessage::customCityCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::City_in_The_Sky_Big_Key:
                    {
                        replacementText = customMessage::customCityBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Palace_of_Twilight_Small_Key:
                    {
                        replacementText = customMessage::customPalaceSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Palace_of_Twilight_Dungeon_Map:
                    {
                        replacementText = customMessage::customPalaceMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Palace_of_Twilight_Compass:
                    {
                        replacementText = customMessage::customPalaceCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Palace_of_Twilight_Big_Key:
                    {
                        replacementText = customMessage::customPalaceBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Hyrule_Castle_Small_Key:
                    {
                        replacementText = customMessage::customCastleSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Hyrule_Castle_Dungeon_Map:
                    {
                        replacementText = customMessage::customCastleMapText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Hyrule_Castle_Compass:
                    {
                        replacementText = customMessage::customCastleCompassText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Hyrule_Castle_Big_Key:
                    {
                        replacementText = customMessage::customCastleBigKeyText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Shadow_Crystal:
                    {
                        replacementText = customMessage::customCrystalText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Ending_Blow:
                    {
                        replacementText = customMessage::endingBlowText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Shield_Attack:
                    {
                        replacementText = customMessage::shieldAttackText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Back_Slice:
                    {
                        replacementText = customMessage::backSliceText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Helm_Splitter:
                    {
                        replacementText = customMessage::helmSplitterText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Mortal_Draw:
                    {
                        replacementText = customMessage::mortalDrawText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Jump_Strike:
                    {
                        replacementText = customMessage::jumpStrikeText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }
                    case items::Great_Spin:
                    {
                        replacementText = customMessage::greatSpinText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Item::Bublin_Camp_Key:
                    {
                        replacementText = customMessage::customBublinSKText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    case items::Dominion_Rod_Uncharged:
                    {
                        replacementText = customMessage::customPoweredRodText;
                        control->msg = replacementText;
                        control->wMsgRender = replacementText;
                        break;
                    }

                    default:
                    {
                        break;
                    }
                }

                return ret;
            } );

        getFontCCColorTable_trampoline = patch::hookFunction( tp::d_msg_class::getFontCCColorTable, []( u8 colorId, u8 unk ) {
            if ( colorId >= 0x9 )
            {
                return global::modPtr->getCustomMsgColor( colorId );
            }
            else
            {
                return global::modPtr->getFontCCColorTable_trampoline( colorId, unk );
            }
        } );

        getFontGCColorTable_trampoline = patch::hookFunction( tp::d_msg_class::getFontGCColorTable, []( u8 colorId, u8 unk ) {
            if ( colorId >= 0x9 )
            {
                return global::modPtr->getCustomMsgColor( colorId );
            }
            else
            {
                return global::modPtr->getFontGCColorTable_trampoline( colorId, unk );
            }
        } );

        checkItemGet_trampoline = patch::hookFunction( tp::d_item::checkItemGet, []( u8 item, s32 defaultValue ) {
            return global::modPtr->proc_checkItemGet( item, defaultValue );
        } );

        parseCharacter1Byte_trampoline = patch::hookFunction( tp::resource::parseCharacter_1Byte, []( const char** text ) {
            const char* bigWalletRupeeText = { "600 Rupees" MSG_COLOR( MSG_COLOR_WHITE ) "!" };
            const char* giantWalletRupeeText = { "1,000 Rupees" MSG_COLOR( MSG_COLOR_WHITE ) "!" };
            const char* pauseWalletRupeeText = { "1,000 Rupees." };

            if ( strncmp( *text, bigWalletRupeeText, strlen( bigWalletRupeeText ) ) == 0 )
            {
                // Replacement text
                const char* replacementText = { "5,000 Rupees" MSG_COLOR( MSG_COLOR_WHITE ) "!" };
                *text = replacementText;
            }
            if ( strncmp( *text, giantWalletRupeeText, strlen( giantWalletRupeeText ) ) == 0 )
            {
                // Replacement text
                const char* replacementText = { "9,999 Rupees" MSG_COLOR( MSG_COLOR_WHITE ) "!" };
                *text = replacementText;
            }
            else if ( strncmp( *text, pauseWalletRupeeText, strlen( pauseWalletRupeeText ) ) == 0 )
            {
                // Replacement text
                switch ( gameInfo.scratchPad.equippedWallet )
                {
                    case 0x0:
                    {
                        const char* replacementText = { "1,000 Rupees." };
                        *text = replacementText;
                        break;
                    }
                    case 0x1:
                    {
                        const char* replacementText = { "5,000 Rupees." };
                        *text = replacementText;
                        break;
                    }
                    case 0x2:
                    {
                        const char* replacementText = { "9,999 Rupees." };
                        *text = replacementText;
                        break;
                    }
                }
            }

            return global::modPtr->parseCharacter1Byte_trampoline( text );
        } );
    }

    void Mod::procNewFrame()
    {
        float linkPos[3];
        getPlayerPos( linkPos );

        if ( coordsAreInHex == 1 )
        {
            typeTransform<float, u32> x = { linkPos[0] };
            typeTransform<float, u32> y = { linkPos[1] };
            typeTransform<float, u32> z = { linkPos[2] };

            snprintf( currentPosX, 30, "%04x", x.b );
            snprintf( currentPosY, 30, "%04x", y.b );
            snprintf( currentPosZ, 30, "%04x", z.b );

            snprintf( linkAngle, 30, "%02x", static_cast<u16>( tp::d_map_path_dmap::getMapPlayerAngleY() ) );
        }
        else
        {
            snprintf( currentPosX, 30, "%f", linkPos[0] );
            snprintf( currentPosY, 30, "%f", linkPos[1] );
            snprintf( currentPosZ, 30, "%f", linkPos[2] );

            snprintf( linkAngle, 30, "%d", static_cast<u16>( tp::d_map_path_dmap::getMapPlayerAngleY() ) );
        }

        if ( gameInfo.nextStageVars.nextSpawnPoint != 0xFF )
        {
            lastGoodSpawn = gameInfo.nextStageVars.nextSpawnPoint;
        }

        if ( gameInfo.ColorPtr != nullptr )
        {
            if ( topToggle == 0 )
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
            if ( bottomToggle == 0 )
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
            /*u16 last14 = 0;
            for(colorResult = 0x0; colorResult < 0x6000; colorResult++)
            {

                    if (gameInfo.ColorPtr->unk_0[colorResult] == 0x14)
                    {
                            if (colorResult - last14 == 8)
                            {
                                    break;
                            }
                            else
                            {
                                    last14 = colorResult;
                            }
                    }
            }*/
        }

        if ( trigerLoadSave == 1 )
        {
            trigerLoadSave = 0;
            tools::triggerSaveLoad( stage::allStages[stage], room, spawn, state );
        }

        if ( !tools::checkItemFlag( ItemFlags::Orange_Rupee ) )
        {     // remove the item get animations for floor pickups (except silver rupee)
            u32 loopAmount = sizeof( item::itemGetAnimationFlags ) / sizeof( item::itemGetAnimationFlags[0] );
            for ( u32 i = 0; i < loopAmount; i++ )
            {
                tools::setItemFlag( static_cast<ItemFlags>( item::itemGetAnimationFlags[i] ) );
            }
        }

        if ( enableNormalTime == 0 && setDay == 0 )
        {     // set night
            gameInfo.scratchPad.skyAngle = 0;
        }
        else if ( enableNormalTime == 0 && setDay == 1 )
        {     // set day
            gameInfo.scratchPad.skyAngle = 180;
        }
        // Increment seed
        if ( !customSeed )
        {
            tools::getRandom( 0 );
        }

        // If loading has started check for LoadEvents
        if ( isLoading )
        {
            eventListener->checkLoadEvents();

            // Check if there's a random seed in the current save data
            if ( ( *global::seedInSaveFile > 0 ) && ( *global::seedInSaveFile != tools::randomSeed ) )
            {
                customSeed = 1;
                tools::randomSeed = *global::seedInSaveFile;
                chestRandomizer->generate();
                musicrando::initMusicRando();
            }
        }

        if ( controller::checkForButtonInputSingleFrame(
                 ( controller::PadInputs::Button_R | controller::PadInputs::Button_Z ) ) )
        {
            // Toggle console
            system_console::setState( !sysConsolePtr->consoleEnabled, 0 );
        }

        if ( controller::checkForButtonInputSingleFrame( controller::PadInputs::Button_R | controller::PadInputs::Button_Y ) )
        {
            if ( canQuickTransform() )
            {
                // Transform
                tp::d_a_alink::procCoMetamorphoseInit( gameInfo.linkMapPtr );
            }
        }

        if ( canChangeToD() &&
             controller::checkForButtonInputSingleFrame( controller::PadInputs::Button_R | controller::PadInputs::Button_Y ) )
        {
            if ( gameInfo.scratchPad.skyAngle >= 105 && gameInfo.scratchPad.skyAngle <= 284 )
            {
                gameInfo.scratchPad.skyAngle = 285;
                if ( gameInfo.nextStageVars.nextSpawnPoint == 0xFF )
                {
                    gameInfo.nextStageVars.nextSpawnPoint = lastGoodSpawn;
                }
                gameInfo.nextStageVars.triggerLoad |= 1;
            }
            else if ( gameInfo.scratchPad.skyAngle >= 285 || gameInfo.scratchPad.skyAngle <= 104 )
            {
                gameInfo.scratchPad.skyAngle = 105;
                if ( gameInfo.nextStageVars.nextSpawnPoint == 0xFF )
                {
                    gameInfo.nextStageVars.nextSpawnPoint = lastGoodSpawn;
                }
                gameInfo.nextStageVars.triggerLoad |= 1;
            }
        }

        if ( sysConsolePtr->consoleEnabled )
        {
            if ( controller::checkForButtonInputSingleFrame(
                     ( controller::PadInputs::Button_R | controller::PadInputs::Button_Start ) ) )
            {
                // Check if we have a seed sitting in our save file somewhere and if so, apply this automatically (unless custom
                // seed is turned on)

                if ( !customSeed && ( *global::seedInSaveFile > 0 ) &&
                     !tp::d_a_alink::checkStageName( stage::allStages[Stage_Title_Screen] ) )
                {
                    tools::randomSeed = *global::seedInSaveFile;
                }

                // In case this person hasn't started the run through the load event earlier we still have to store this seed
                // manually in the save data
                *global::seedInSaveFile = tools::randomSeed;
                chestRandomizer->generate();
                musicrando::initMusicRando();
            }

            // Parse inputs of this frame
            switch ( tp::m_do_controller_pad::cpadInfo.buttonInputTrg )
            {
                case controller::PadInputs::Button_A:
                    hudConsole->performAction( ConsoleActions::Option_Increase );
                    break;

                case controller::PadInputs::Button_X:
                    hudConsole->performAction( ConsoleActions::Option_Increase, 0x10 );
                    break;

                case controller::PadInputs::Button_B:
                    hudConsole->performAction( ConsoleActions::Option_Decrease );
                    break;

                case controller::PadInputs::Button_Y:
                    hudConsole->performAction( ConsoleActions::Option_Decrease, 0x10 );
                    break;

                case controller::PadInputs::Button_DPad_Up:
                    hudConsole->performAction( ConsoleActions::Move_Up );
                    break;

                case controller::PadInputs::Button_DPad_Down:
                    hudConsole->performAction( ConsoleActions::Move_Down );
                    break;

                case controller::PadInputs::Button_DPad_Left:
                    hudConsole->performAction( ConsoleActions::Move_Left );
                    break;

                case controller::PadInputs::Button_DPad_Right:
                    hudConsole->performAction( ConsoleActions::Move_Right );
                    break;
            }
            hudConsole->draw();
        }

        if ( truePause && sysConsolePtr->consoleEnabled )
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

        if ( itemSearchID != lastItemSearchID )
        {
            lastItemSearchID = itemSearchID;

            strcpy( itemSearchResults, "404" );

            for ( u16 i = 0; i < chestRandomizer->totalChecks; i++ )
            {
                item::ItemCheck* check = &item::checks[i];
                if ( check->destination )
                {
                    if ( check->destination->itemID == itemSearchID )
                    {
                        // Found the source
                        snprintf( itemSearchResults,
                                  40,
                                  "ID: %x Stage: %s Room: %d",
                                  check->itemID,
                                  check->stage,
                                  check->room );
                    }
                }
            }
        }
        else if ( itemReverseSearchID != lastItemReverseSearchID )
        {
            lastItemReverseSearchID = itemReverseSearchID;

            strcpy( itemReverseSearchResults, "404" );

            for ( u16 i = 0; i < chestRandomizer->totalChecks; i++ )
            {
                item::ItemCheck* check = &item::checks[i];
                if ( check->source )
                {
                    if ( check->source->itemID == itemReverseSearchID )
                    {
                        // Found the source
                        snprintf( itemReverseSearchResults,
                                  40,
                                  "ID: %x Stage: %s Room: %d",
                                  check->itemID,
                                  check->stage,
                                  check->room );
                    }
                }
            }
        }

        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Faron_Woods] ) )
        {
            if ( 0xB == gameInfo.eventSystem.currentEventID )
            {
                tools::setCutscene( false, false );
                if ( frame_counter == num_frames )
                {
                    tools::setItemFlag(
                        ItemFlags::Vessel_Of_Light_Faron );     // set flag for vessel since we'll skip it by reloading
                    gameInfo.scratchPad.movingActors.exploredRegions.Eldin =
                        0b1;     // Set Eldin Map Unlocked so we can warp there
                    tools::setCutscene( true, false );
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
        checkSearchID = ( checkSearchID2 * 0x100 ) + checkSearchID1;
        checkReverseSearchID = ( checkReverseSearchID2 * 0x100 ) + checkReverseSearchID1;
        if ( checkSearchID != lastCheckSearchID )
        {
            lastCheckSearchID = checkSearchID;

            strcpy( checkSearchResults, "404" );
            if ( checkSearchID < chestRandomizer->totalChecks )
            {
                item::ItemCheck* check = &item::checks[checkSearchID];
                if ( check->source )
                {
                    snprintf( checkSearchResults,
                              40,
                              "ID: %x Stage: %s Room: %d",
                              check->source->itemID,
                              check->source->stage,
                              check->source->room );
                }
            }
        }
        else if ( checkReverseSearchID != lastCheckReverseSearchID )
        {
            lastCheckReverseSearchID = checkReverseSearchID;

            strcpy( checkReverseSearchResults, "404" );

            if ( checkReverseSearchID < chestRandomizer->totalChecks )
            {
                item::ItemCheck* check = &item::checks[checkReverseSearchID];
                if ( check->destination )
                {
                    snprintf( checkReverseSearchResults,
                              40,
                              "ID: %x Stage: %s Room: %d",
                              check->destination->itemID,
                              check->destination->stage,
                              check->destination->room );
                }
            }
        }

        /*if (gameInfo.scratchPad.unk_0[0x019] == 0)
        {
                const char* walletText = "Wallet";
                strncpy(tp::d_com_inf_game::wallet_text, walletText, sizeof(tp::d_com_inf_game::wallet_text) - 1);

                const char* walletDescription = "A wallet from your childhood. It holds up to 1,000 Rupees.";
                strncpy(tp::d_com_inf_game::wallet_description, walletDescription,
        sizeof(tp::d_com_inf_game::wallet_description) - 1);
        }
        else if (gameInfo.scratchPad.unk_0[0x019] == 1)
        {
                const char* walletText = "Big Wallet";
                strncpy(tp::d_com_inf_game::wallet_text, walletText, sizeof(tp::d_com_inf_game::wallet_text) - 1);

                const char* walletDescription = "A wallet given to you by Agitha, princess of the insect kingdom. It can hold
        5,000 Rupees."; strncpy(tp::d_com_inf_game::wallet_description, walletDescription,
        sizeof(tp::d_com_inf_game::wallet_description) - 1);
        }
        else if (gameInfo.scratchPad.unk_0[0x019] == 2)
        {
                const char* walletText = "Giant Wallet";
                strncpy(tp::d_com_inf_game::wallet_text, walletText, sizeof(tp::d_com_inf_game::wallet_text) - 1);

                const char* walletDescription = "The wallet given by Agitha to benefactors of the insect kingdom. It can hold up
        to 9,999 Rupees."; strncpy(tp::d_com_inf_game::wallet_description, walletDescription,
        sizeof(tp::d_com_inf_game::wallet_description) - 1);
        }*/

        if ( inputBuffering )
        {
            tp::m_do_controller_pad::cpadInfo.buttonInputTrg = tp::m_do_controller_pad::cpadInfo.buttonInput;
        }

        giveAllStoryItems();

        reorderItemWheel();

        giveAllScents();

        fixYetaAndYeto();

        preventPoweringUpDomRod();

        // Call original function
        fapGm_Execute_trampoline();

        changeLanternColor();
    }

    s32 Mod::procItemCreateFunc( const float pos[3], s32 item, const char funcIdentifier[32] )
    {
        strcpy( lastItemFunc, funcIdentifier );
        snprintf( lastItemDataID, 5, "0x%02x", item );
        snprintf( lastItemDataX, 30, "%f", pos[0] );
        snprintf( lastItemDataY, 30, "%f", pos[1] );
        snprintf( lastItemDataZ, 30, "%f", pos[2] );
        // Runs once when Link picks up an item with text and is holding it towards the camera
        if ( randoEnabled && strcmp( funcIdentifier, "createItemForDirectGet" ) != 0 &&
             strcmp( funcIdentifier, "createItemForBoss" ) != 0 && strcmp( funcIdentifier, "createItemForMidBoss" ) != 0 &&
             strcmp( funcIdentifier, "createItemForSimpleDemo" ) != 0 )
        {
            item = chestRandomizer->getItemReplacement( pos, item );
        }

        return item;
    }

    bool Mod::procCheckTreasureRupeeReturn( void* unk1, s32 item )
    {
        // Allow to open
        return false;
    }

    s32 Mod::procEvtSkipper( void* evtPtr )
    {
        // Runs when the user tries to skip a Cutscene
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Sacred_Grove] ) )
        {
            // We're at sacred grove
            if ( 0x2 == gameInfo.eventSystem.currentEventID )
            {
                // Master Sword cutscene
                tools::setCutscene( true, false, cutscene_skip::onMasterSwordSkip );
            }
        }

        // Call original function
        return evt_control_Skipper_trampoline( evtPtr );
    }

    bool Mod::proc_query022( void* unk1, void* unk2, s32 unk3 )
    {
        // Check to see if currently in one of the Ordon interiors
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Ordon_Interiors] ) )
        {
            // Check to see if ckecking for the Iron Boots
            u16 item = *reinterpret_cast<u16*>( reinterpret_cast<u32>( unk2 ) + 0x4 );

            if ( item == items::Item::Iron_Boots )
            {
                // Return false so that the door in Bo's house can be opened without having the Iron Boots
                return false;
            }
        }

        // Call original function
        return query022_trampoline( unk1, unk2, unk3 );
    }

    bool Mod::proc_query023( void* unk1, void* unk2, s32 unk3 )
    {
        // Check to see if currently in the Kakariko Interiors. Specifically in Barnes' Bomb Shop
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Interiors] ) &&
             tp::d_kankyo::env_light.currentRoom == 1 )
        {
            // If player has not bought Barnes' Bomb Bag, we want to allow them to be able to get the check.
            if ( ( !tp::d_a_alink::dComIfGs_isEventBit( 0x908 ) ) )
            {
                return false;
            }
            // If the player has bought the bomb bag check, we won't allow them to get the check, regardless of if they have
            // bombs or not
            else
            {
                return true;
            }
        }

        // Call original function if the conditions aren't met.
        return query023_trampoline( unk1, unk2, unk3 );
    }

    bool Mod::proc_query024( void* dMsgFlow_cPtr, void* mesg_flow_node_branchPtr, void* fopAc_ac_cPtr, int unused )
    {
        // Check to see if currently in the Castle Town. Specifically in the central square
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Castle_Town] ) && tp::d_kankyo::env_light.currentRoom == 0 )
        {
            return true;     // Allow the player to buy arrows, even if the quiver is full
        }
        // Call original function if the conditions aren't met.
        return query024_trampoline( dMsgFlow_cPtr, mesg_flow_node_branchPtr, fopAc_ac_cPtr, unused );
    }

    bool Mod::proc_query025( void* unk1, void* unk2, s32 unk3 )
    {
        // Check to see if currently in a shop and that we have the bottle requirement enabled
        if ( isStageShop() && ( allowBottleItemsShopAnytime == 1 ) )
        {
            // Tell the game that we have an empty bottle, even if we don't
            return false;
        }
        // Call original function if the conditions aren't met.
        return query025_trampoline( unk1, unk2, unk3 );
    }

    bool Mod::proc_isDungeonItem( void* memBitPtr, const int param_1 )
    {
        u32 totalMainDungeonStages = sizeof( stage::mainDungeonStages ) / sizeof( stage::mainDungeonStages[0] );
        for ( u32 i = 0; i < totalMainDungeonStages; i++ )
        {
            if ( tp::d_a_alink::checkStageName( stage::mainDungeonStages[i] ) && ( param_1 == 3 ) )
            {
                if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Forest_Temple] ) && ( param_1 == 7 ) &&
                     ( ( tp::d_kankyo::env_light.currentRoom == 3 ) || ( tp::d_kankyo::env_light.currentRoom == 1 ) ) )
                {
                    return false;
                }
                return false;
            }
        }

        // Call original function
        return isDungeonItem_trampoline( memBitPtr, param_1 );
    }

    bool Mod::proc_isEventBit( u8* eventSystem, u16 indexNumber )
    {
        if ( ( ( indexNumber == 0x2280 ) || ( indexNumber == 0x2320 ) || ( indexNumber == 0x3E02 ) ) &&
             tp::d_a_alink::checkStageName( stage::allStages[Stage_Hidden_Village] ) )
        {
            if ( ( ( gameInfo.scratchPad.eventBits[0x22] & 0x80 ) == 0 ) )
            {
                return false;
            }
        }
        else if ( Singleton::getInstance()->isEarlyHCEnabled == 0 && ( indexNumber == 0x5410 ) &&
                  ( ( gameInfo.scratchPad.allAreaNodes.Palace_of_Twilight.dungeon.bossBeaten == 0 ) ) )
        {
            return false;
        }

        else
        {
            u32 totalAllDungeonStages = sizeof( stage::allDungeonStages ) / sizeof( stage::allDungeonStages[0] );
            u32 totalStageFlags = sizeof( game_patch::dungeonStoryFlags ) / sizeof( game_patch::dungeonStoryFlags[0] );
            for ( u32 i = 0; i < totalAllDungeonStages; i++ )
            {
                if ( tp::d_a_alink::checkStageName( stage::allDungeonStages[i] ) )
                {
                    for ( u32 j = 0; j < totalStageFlags; j++ )
                    {
                        if ( indexNumber == game_patch::dungeonStoryFlags[j] )
                        {
                            if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Zant_Main] ) &&
                                 ( gameInfo.localAreaNodes.dungeon.bossBeaten != 0 ) )
                            {
                                return isEventBit_trampoline( eventSystem, indexNumber );
                            }
                            return false;
                        }
                    }
                }
            }
        }

        // Call original function
        return isEventBit_trampoline( eventSystem, indexNumber );
    }

    bool Mod::procDoLink( tp::dynamic_link::DynamicModuleControl* dmc )
    {
        // Call the original function immediately, as the REL file needs to be linked before applying patches
        const bool result = do_link_trampoline( dmc );

        // Get the pointer to the current REL file
        gc::OSModule::OSModuleInfo* moduleInfo = dmc->moduleInfo;

        // Make sure a REL file is actually loaded, as do_link will clear the pointer if something goes wrong
        if ( !moduleInfo )
        {
            return result;
        }

        // Get the REL pointer as a raw u32, to use for overwrites
        u32 relPtrRaw = reinterpret_cast<u32>( moduleInfo );

        // Modify the current REL file
        switch ( moduleInfo->id )     // May want to set up enums or defines for the module ids
        {
            case 0x121:     // d_a_npc_bouS.rel - Inside Bo's house
            {
                // Prevent Bo from talking after the chest has been opened
                *reinterpret_cast<u32*>( relPtrRaw + 0x1A44 ) = 0x48000028;     // b 0x28
                break;
            }
            case 0x280:     // d_a_obj_swBallC.rel - Light Sword Cutscene
            {
                item::ItemCheck* itemCheck = &item::checks[495];     // Light Sword Item
                // Check if the item has a replacement
                if ( itemCheck->destination )
                {
                    // Replace Item given
                    *reinterpret_cast<u32*>( relPtrRaw + 0xC10 ) =
                        SET_LOAD_IMMEDIATE( 3, itemCheck->destination->itemID );     // 38600000 + item ID
                    // Replace Item Get text
                    *reinterpret_cast<u32*>( relPtrRaw + 0xCD8 ) = SET_LOAD_IMMEDIATE(
                        3,
                        ( itemCheck->destination->itemID + 0x65 ) );     // 38600000 + item index (item ID + 0x65)
                }

                // The cutscene gives link the MS during the cutscene by default, so we just nop out the link to the function.
                *reinterpret_cast<u32*>( relPtrRaw + 0xB50 ) = 0x60000000;
                break;
            }
            case 0x1FA:     // d_a_obj_kshutter.rel - Doors that do not cause a cutscene after opened
            {
                if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Lakebed_Temple] ) &&
                     ( tp::d_kankyo::env_light.currentRoom == 3 ) )
                {
                    // Set it to not remove a small key from the inventory when opening the boss door
                    *reinterpret_cast<u32*>( relPtrRaw + 0x1198 ) = 0x60000000;     // Previous: 0x3803ffff
                }
                break;
            }
            case 0x13F:     // d_a_npc_impal.rel - Impaz
            {
                item::ItemCheck* itemCheck1 = &item::checks[327];     // Ilia's Charm Item
                item::ItemCheck* itemCheck2 = &item::checks[466];     // Empty Skybook Item
                // Replace the Ilias Charm item
                *reinterpret_cast<u32*>( relPtrRaw + 0x2D98 ) =
                    SET_LOAD_IMMEDIATE( 4, itemCheck1->destination->itemID );     // 38800000 + item ID
                // Replace the Ancient Skybook Item
                *reinterpret_cast<u32*>( relPtrRaw + 0x3240 ) =
                    SET_LOAD_IMMEDIATE( 4, itemCheck2->destination->itemID );     // 38800000 + item ID

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
        if ( !chestRandomizer->isStageBoss() )
        {
            gameInfo.localAreaNodes.dungeon.containerGotten = 0b0;
        }
    }

    s32 Mod::proc_checkItemGet( u8 item, s32 defaultValue )
    {
        // Check to see if currently in one of the Ordon interiors
        switch ( item )
        {
            case items::Hylian_Shield:
            {
                // Check if we are at Kakariko Malo mart and verify that we have not bought the shield.
                if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Interiors] ) &&
                       tp::d_kankyo::env_light.currentRoom == 3 ) &&
                     ( ( gameInfo.localAreaNodes.unk_0[0xC] & 0x2 ) == 0 ) )
                {
                    // Return false so we can buy the shield.
                    return 0;
                }
                // Check if we are at Castle Town Goron Shops and verify that we have not bought the shield.
                // Since there is not a flag for this, we will just check if the randomizer has set a nullptr for the check.
                else if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Castle_Town_Shops] ) &&
                            tp::d_kankyo::env_light.currentRoom == 4 ) )
                {
                    // Return false so we can buy the shield.
                    return 0;
                }
            }
            case items::Hawkeye:
            {
                // Check if we are at Kakariko Village and that the hawkeye is currently not for sale.
                if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Village] ) &&
                       ( ( gameInfo.localAreaNodes.unk_0[0xC] & 0x40 ) == 0 ) ) )
                {
                    // Return false so we can buy the hawkeye.
                    return 0;
                }
            }
            case items::Ordon_Shield:
            {
                // Check if we are at Kakariko Malo mart.
                if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Interiors] ) &&
                       tp::d_kankyo::env_light.currentRoom == 3 ) )
                {
                    // Return false so we can buy the wooden shield.
                    return 0;
                }
                // Check if we are at the Death Mountain Goron Shop.
                else if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Death_Mountain] ) &&
                            tp::d_kankyo::env_light.currentRoom == 3 ) )
                {
                    // Return false so we can buy the shield.
                    return 0;
                }
            }
            case items::Wooden_Shield:
            {
                // Check if we are at Kakariko Malo mart.
                if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Interiors] ) &&
                       tp::d_kankyo::env_light.currentRoom == 3 ) )
                {
                    // Return false so we can buy the wooden shield.
                    return 0;
                }
                // Check if we are at the Death Mountain Goron Shop.
                else if ( ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Death_Mountain] ) &&
                            tp::d_kankyo::env_light.currentRoom == 3 ) )
                {
                    // Return false so we can buy the shield.
                    return 0;
                }
            }
            default:
            {
                // Call original function if the conditions are not met.
                return global::modPtr->checkItemGet_trampoline( item, defaultValue );
            }
        }
    }

    void Mod::giveAllScents()
    {
        // code to have all scents at once you need to unlock them tho
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Hyrule_Field] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Youths_Scent ) && tp::d_kankyo::env_light.currentRoom == 3 )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Ilias_Scent ) &&
                      ( tp::d_kankyo::env_light.currentRoom == 9 || tp::d_kankyo::env_light.currentRoom == 10 ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Village] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Youths_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Outside_Castle_Town] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Ilias_Scent ) && tp::d_kankyo::env_light.currentRoom == 8 )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Castle_Town] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Medicine_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Medicine_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Ilias_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Zoras_Domain] ) ||
                  tp::d_a_alink::checkStageName( stage::allStages[Stage_Snowpeak] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Reekfish_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Reekfish_Scent;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Arbiters_Grounds] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Poe_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Poe_Scent;
            }
        }
        else
        {
            if ( tools::checkItemFlag( ItemFlags::Medicine_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Medicine_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Reekfish_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Reekfish_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Poe_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Poe_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Ilias_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Ilias_Scent;
            }
            else if ( tools::checkItemFlag( ItemFlags::Youths_Scent ) )
            {
                gameInfo.scratchPad.equipedItems.scent = items::Item::Youths_Scent;
            }
        }
    }

    void Mod::giveAllStoryItems()
    {
        if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Lake_Hylia] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Aurus_Memo ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Aurus_Memo;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Snowpeak] ) ||
                  tp::d_a_alink::checkStageName( stage::allStages[Stage_Zoras_Domain] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Asheis_Sketch ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Asheis_Sketch;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Graveyard] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Asheis_Sketch ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Kakariko_Interiors] ) &&
                  tp::d_kankyo::env_light.currentRoom == 0 )
        {
            if ( tools::checkItemFlag( ItemFlags::Ilias_Charm ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
            }
            else if ( tools::checkItemFlag( ItemFlags::Wooden_Statue ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
            }
            else if ( tools::checkItemFlag( ItemFlags::Renardos_Letter ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Outside_Castle_Town] ) &&
                  tp::d_kankyo::env_light.currentRoom == 3 )
        {
            if ( tools::checkItemFlag( ItemFlags::Wooden_Statue ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Castle_Town_Shops] ) &&
                  tp::d_kankyo::env_light.currentRoom == 2 )
        {
            if ( tools::checkItemFlag( ItemFlags::Invoice ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Castle_Town_Interiors] ) &&
                  tp::d_kankyo::env_light.currentRoom == 5 )
        {
            if ( tools::checkItemFlag( ItemFlags::Invoice ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
            }
            else if ( tools::checkItemFlag( ItemFlags::Renardos_Letter ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
            }
        }
        else if ( tp::d_a_alink::checkStageName( stage::allStages[Stage_Hidden_Village] ) ||
                  tp::d_a_alink::checkStageName( stage::allStages[Stage_Impaz_House] ) )
        {
            if ( tools::checkItemFlag( ItemFlags::Ilias_Charm ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
            }
        }
        else
        {
            if ( tools::checkItemFlag( ItemFlags::Horse_Call ) )
            {     // finished Ilia Quest
                gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
            }
            else if ( tools::checkItemFlag( ItemFlags::Ilias_Charm ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Ilias_Charm;
            }
            else if ( tools::checkItemFlag( ItemFlags::Wooden_Statue ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Wooden_Statue;
            }
            else if ( tools::checkItemFlag( ItemFlags::Invoice ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Invoice;
            }
            else if ( tools::checkItemFlag( ItemFlags::Renardos_Letter ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Renardos_Letter;
            }

            if ( tools::checkItemFlag( ItemFlags::Coral_Earring ) )
            {     // given sketch
                gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
            }
            else if ( tools::checkItemFlag( ItemFlags::Asheis_Sketch ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Asheis_Sketch;
            }

            if ( gameInfo.scratchPad.movingActors.exploredRegions.Desert == 0b1 )
            {     // given memo
                gameInfo.scratchPad.itemWheel.Story = items::Item::NullItem;
            }
            else if ( tools::checkItemFlag( ItemFlags::Aurus_Memo ) )
            {
                gameInfo.scratchPad.itemWheel.Story = items::Item::Aurus_Memo;
            }
        }
    }

    void Mod::reorderItemWheel()
    {
        u8 currentSlot = 0x0;
        if ( tools::checkItemFlag( ItemFlags::Clawshots ) )
        {
            gameInfo.scratchPad.itemWheel.Double_Clawshot = items::Item::Clawshots;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xA;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Dominion_Rod ) )
        {
            gameInfo.scratchPad.itemWheel.Dominion_Rod = items::Item::Dominion_Rod;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x8;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Ball_and_Chain ) )
        {
            gameInfo.scratchPad.itemWheel.Ball_and_Chain = items::Item::Ball_and_Chain;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x6;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Spinner ) )
        {
            gameInfo.scratchPad.itemWheel.Spinner = items::Item::Spinner;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x2;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Clawshot ) )
        {
            gameInfo.scratchPad.itemWheel.Clawshot = items::Item::Clawshot;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x9;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Heros_Bow ) )
        {
            gameInfo.scratchPad.itemWheel.Bow = items::Item::Heros_Bow;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x4;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Iron_Boots ) )
        {
            gameInfo.scratchPad.itemWheel.Iron_Boots = items::Item::Iron_Boots;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x3;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Boomerang ) )
        {
            gameInfo.scratchPad.itemWheel.Boomerang = items::Item::Boomerang;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x0;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Lantern ) )
        {
            gameInfo.scratchPad.itemWheel.Lantern = items::Item::Lantern;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x1;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Slingshot ) )
        {
            gameInfo.scratchPad.itemWheel.Slingshot = items::Item::Slingshot;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x17;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Fishing_Rod != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x14;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Hawkeye ) )
        {
            gameInfo.scratchPad.itemWheel.Hawkeye = items::Item::Hawkeye;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x5;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bomb_Bag_1 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xF;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bomb_Bag_2 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x10;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bomb_Bag_3 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x11;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bottle_1 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xB;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bottle_2 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xC;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bottle_3 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xD;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Bottle_4 != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xE;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Story != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x13;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Ooccoo != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x12;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Sky_Book != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x16;
            currentSlot++;
        }
        if ( tools::checkItemFlag( ItemFlags::Horse_Call ) )
        {
            gameInfo.scratchPad.itemWheel.Horse_Call = items::Item::Horse_Call;
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x15;
            currentSlot++;
        }
        if ( gameInfo.scratchPad.itemWheel.Item_Slot != 0xFF )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0x7;
            currentSlot++;
        }

        for ( u16 i = currentSlot; i < sizeof( gameInfo.scratchPad.itemSlotsOrder ) / sizeof( u8 ); i++ )
        {
            gameInfo.scratchPad.itemSlotsOrder[currentSlot] = 0xFF;
        }
    }

    void Mod::fixYetaAndYeto()
    {
        if ( tools::checkItemFlag( ItemFlags::Bed_Key ) && tp::d_a_alink::checkStageName( "D_MN11" ) )
        {
            float linkPos[3];
            getPlayerPos( linkPos );

            if ( gameInfo.aButtonText == 0x6 &&
                 ( tp::d_kankyo::env_light.currentRoom == 0 || tp::d_kankyo::env_light.currentRoom == 1 ||
                   tp::d_kankyo::env_light.currentRoom == 2 || tp::d_kankyo::env_light.currentRoom == 3 ||
                   tp::d_kankyo::env_light.currentRoom == 4 || tp::d_kankyo::env_light.currentRoom == 7 ) &&
                 linkPos[1] == 0 )
            {
                gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b0;
                yetaTrickOn = 1;
            }
            if ( gameInfo.aButtonText == 0x79 && yetaTrickOn == 1 )
            {
                gameInfo.localAreaNodes.dungeon.bigKeyGotten = 0b1;
                yetaTrickOn = 0;
            }
        }
    }

    void Mod::preventPoweringUpDomRod()
    {
        if ( gameInfo.scratchPad.itemWheel.Sky_Book == 0xFF && tools::checkItemFlag( ItemFlags::Ancient_Sky_Book_empty ) &&
             !tools::checkItemFlag( ItemFlags::Ancient_Sky_Book_partly_filled ) )
        {
            gameInfo.scratchPad.itemWheel.Sky_Book = items::Item::Ancient_Sky_Book_empty;
        }
        if ( tp::d_a_alink::checkStageName( "R_SP209" ) )
        {
            if ( gameInfo.scratchPad.itemWheel.Sky_Book == items::Item::Ancient_Sky_Book_empty )
            {
                gameInfo.scratchPad.itemWheel.Sky_Book = 0xFF;
            }
        }
    }

    bool Mod::isStageShop()
    {
        u8 totalShopStages = sizeof( stage::shopStages ) / sizeof( stage::shopStages[0] );
        for ( u8 i = 0; i < totalShopStages; i++ )
        {
            if ( tp::d_a_alink::checkStageName( stage::shopStages[i] ) )
            {
                return true;
            }
        }
        return false;
    }

    void Mod::doCustomTRESActor( void* mStatus_roomControl )
    {
        tp::d_com_inf_game::GameInfo* pGameInfo = &tp::d_com_inf_game::dComIfG_gameInfo;

        u32 numChecks = sizeof( customChecks ) / sizeof( customChecks[0] );

        // Count and temp array for the checks for this stage+room
        u32 checkCount = 0;
        customCheck* checks = new customCheck[5];     // Assume there's never more than 5 custom checks per stage+room

        // Loop through checks and place if correct room and stage
        for ( u32 i = 0; i < numChecks; i++ )
        {
            customCheck* check = &customChecks[i];

            if ( 0 == strcmp( pGameInfo->nextStageVars.nextStage, check->stage ) &&
                 ( 0xFF == check->room ||
                   pGameInfo->nextStageVars.nextRoom == check->room ) &&     // 0xFF = no need to check for room
                 check->requirement() )
            {
                checks[checkCount] = *check;
                checkCount++;
            }
        }

        if ( checkCount > 0 )
        {
            // Create required structs
            tp::d_stage::TRES* TRES = new tp::d_stage::TRES[checkCount];
            tp::d_stage::dzxChunkTypeInfo chunkInfo;
            strcpy( chunkInfo.tag, "ACTR" );     // has to be ACTR for the function we use
            chunkInfo.numChunks = checkCount;
            chunkInfo.chunkDataPtr = TRES;

            // Populate TRES array with data
            for ( u32 i = 0; i < checkCount; i++ )
            {
                customCheck check = checks[i];

                if ( check.overrides != nullptr )
                {
                    check.overrides();
                }

                strcpy( TRES[i].actorName, "tboxA0" );
                TRES[i].flags = 0xFF0FF000 | ( check.chestType << 20 ) | ( check.saveFlag << 4 );

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

            /// Create the actors
            global::modPtr->actorCommonLayerInit_trampoline( mStatus_roomControl, &chunkInfo, 0, nullptr );
            delete[] TRES;
            Singleton::getInstance()->hasActorCommonLayerRan++;
        }

        delete[] checks;
        return;
    }

    void Mod::changeLanternColor()
    {
        // set lantern variables
        u32 lanternVariableAddress = reinterpret_cast<u32>( &tp::d_a_alink::lanternVariables );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x3D ) = reinterpret_cast<u8>( innerRed );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x3F ) = reinterpret_cast<u8>( innerGreen );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x41 ) = reinterpret_cast<u8>( innerBlue );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x43 ) = reinterpret_cast<u8>( outerRed );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x45 ) = reinterpret_cast<u8>( outerGreen );
        *reinterpret_cast<u8*>( lanternVariableAddress + 0x47 ) = reinterpret_cast<u8>( outerBlue );
    }

    bool Mod::canQuickTransform()
    {
        // Make sure Link is actually loaded
        tp::d_com_inf_game::LinkMapVars* linkMapPtr = gameInfo.linkMapPtr;
        // check to make sure that the quick transform variable is enabled
        if ( enableQuickTransform == 0 )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "-> option disabled" );
            return false;
        }

        if ( !linkMapPtr )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "-> no valid spot" );
            return false;
        }

        // Make sure Link currently isnt in an event
        if ( tp::d_a_alink::checkEventRun( linkMapPtr ) )
        {
            return false;
        }

        // Get the value for the alpha of the Z button
        // Pointer path is not currently defined yet
        u32 zButtonAlphaPtr = reinterpret_cast<u32>( tp::d_meter2_info::wZButtonPtr );
        if ( !zButtonAlphaPtr )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "-> alpha ptr 1 failed" );
            return false;
        }

        zButtonAlphaPtr = *reinterpret_cast<u32*>( zButtonAlphaPtr + 0x10C );
        if ( !zButtonAlphaPtr )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "-> alpha ptr 2 failed" );
            return false;
        }

        float zButtonAlpha = *reinterpret_cast<float*>( zButtonAlphaPtr + 0x720 );

        // Check if the Z button is dimmed
        if ( zButtonAlpha != 1.f )
        {
            // Z button is currently dimmed, so don't allow transforming
            strcpy( sysConsolePtr->consoleLine[20].line, "-> z button dimmed" );
            return false;
        }

        // make sure link is not underwater, or talking to anyone
        if ( tp::d_a_alink::linkStatus->status != 0x1 )
        {
            // link is in an invalid state
            return false;
        }

        // Make sure you have the ability to warp
        if ( ( gameInfo.scratchPad.eventBits[0xD] & 0x4 ) == 0 )
        {
            return false;
        }

        // Check to see if link is riding a snowboard
        if ( tp::d_a_alink::checkBoardRide( linkMapPtr ) )
        {
            return false;
        }

        // Check to see if link is riding the canoe
        if ( tp::d_a_alink::checkCanoeRide( linkMapPtr ) )
        {
            return false;
        }

        // Check to see if link is riding Epona
        if ( tp::d_a_alink::checkHorseRide( linkMapPtr ) )
        {
            return false;
        }

        // Check to see if link is riding a boar
        if ( tp::d_a_alink::checkBoarRide( linkMapPtr ) )
        {
            return false;
        }

        // Check to see if link is riding the spinner
        if ( tp::d_a_alink::checkSpinnerRide( linkMapPtr ) )
        {
            return false;
        }

        if ( gameInfo.linkMapPtr->equippedItem == items::Ball_and_Chain )
        {
            return false;
        }

        return true;
    }

    bool Mod::canChangeToD()
    {
        if ( ( gameInfo.scratchPad.eventBits[0xD] & 0x4 ) == 0 )
        {
            return false;
        }

        if ( gameInfo.aButtonText != 0x23 )
        {
            return false;
        }

        if ( gameInfo.eventSystem.actionStatus != 0x29 )
        {
            return false;
        }

        if ( Singleton::getInstance()->midnaTimeControl == 0 )
        {
            return false;
        }

        if ( !chestRandomizer->isStageTOD() )
        {
            return false;
        }
        return true;
    }

    s32 Mod::getMsgIndex( const void* TProcessor, u16 unk2, u32 msgId )
    {
        void* unk = tp::processor::getResource_groupID( TProcessor, unk2 );
        if ( !unk )
        {
            return -1;
        }

        gc::bmgres::TextIndexTable* textIndexTable =
            *reinterpret_cast<gc::bmgres::TextIndexTable**>( reinterpret_cast<u32>( unk ) + 0xC );

        gc::bmgres::MessageEntry* entry = &textIndexTable->entry[0];
        u32 loopCount = textIndexTable->numEntries;

        for ( u32 i = 0; i < loopCount; i++ )
        {
            if ( entry->messageId == msgId )
            {
                return static_cast<s32>( i );
            }
            entry++;
        }

        // Didn't find the index
        return -1;
    }

    s32 Mod::getItemMsgIndex( const void* TProcessor, u16 unk2, u32 itemId )
    {
        // Increment itemId to be at the start of the table entries for items
        u32 msgId = itemId + 0x65;
        return getMsgIndex( TProcessor, unk2, msgId );
    }

    s32 Mod::getItemIdFromMsgId( const void* TProcessor, u16 unk3, u32 msgId )
    {
        void* unk = tp::processor::getResource_groupID( TProcessor, unk3 );
        if ( !unk )
        {
            return -1;
        }

        gc::bmgres::TextIndexTable* textIndexTable =
            *reinterpret_cast<gc::bmgres::TextIndexTable**>( reinterpret_cast<u32>( unk ) + 0xC );

        gc::bmgres::MessageEntry* entry = &textIndexTable->entry[0];
        u32 loopCount = textIndexTable->numEntries;

        // Loop through the item IDs until msgId is found
        u32 itemId = items::Item::Recovery_Heart;
        for ( u32 i = 0; i < loopCount; i++ )
        {
            u16 entryMessageId = entry->messageId;
            if ( entryMessageId == ( itemId + 0x64 ) )
            {
                if ( entryMessageId == msgId )
                {
                    return static_cast<s32>( itemId );
                }
                else
                {
                    itemId++;

                    // Make sure itemId is valid
                    if ( itemId > items::Item::NullItem )
                    {
                        break;
                    }
                }
            }
            entry++;
        }

        // Didn't find the index
        return -1;
    }

    u32 Mod::getCustomMsgColor( u8 colorId )
    {
        u32 newColorCode;     // RGBA
        switch ( colorId )
        {
            case CUSTOM_MSG_COLOR_DARK_GREEN_HEX:
            {
                newColorCode = 0x4BBE4BFF;
                break;
            }
            case CUSTOM_MSG_COLOR_BLUE_HEX:
            {
                newColorCode = 0x4B96D7FF;
                break;
            }
            case CUSTOM_MSG_COLOR_SILVER_HEX:
            {
                newColorCode = 0xBFBFBFFF;
                break;
            }
            default:
            {
                // Return the color white if there is not a match
                newColorCode = 0xFFFFFFFF;
                break;
            }
        }
        return newColorCode;
    }

}     // namespace mod