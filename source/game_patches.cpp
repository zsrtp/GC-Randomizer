#include "game_patches.h"

#include <tp/JFWSystem.h>
#include <tp/d_a_alink.h>
#include <tp/d_a_shop_item_static.h>
#include <tp/d_com_inf_game.h>
#include <tp/d_item.h>
#include <tp/d_item_data.h>
#include <tp/d_menu_collect.h>
#include <tp/d_save.h>
#include <tp/d_stage.h>
#include <tp/evt_control.h>

#include <cstdio>
#include <cstring>

#include "defines.h"
#include "item.h"
#include "itemChecks.h"
#include "items.h"
#include "singleton.h"
#include "stage.h"
#include "tools.h"

namespace mod::cutscene_skip
{
    /**
     * Runs when master sword CS is skipped
     */
    s32 onMasterSwordSkip( void* unk, s32 unk2 )
    {
        game_patch::giveMasterSword();
        game_patch::giveMidna();
        game_patch::giveMidnaTransform();

        gameInfo.nextStageVars.nextSpawnPoint = 0x96;

        tools::triggerSaveLoad( stage::allStages[Stage_Sacred_Grove], 0x1, 0x96 );

        // Call actual skip function
        return tp::evt_control::defaultSkipStb( unk, unk2 );
    }
}     // namespace mod::cutscene_skip

namespace mod::game_patch
{
    void assemblyOverwrites()
    {
        // Default to US/JP
        u32* enableCrashScreen = reinterpret_cast<u32*>( 0x8000B8A4 );

// Get the addresses to overwrite
#ifdef TP_US
        enableCrashScreen = reinterpret_cast<u32*>( 0x8000B8A4 );
#elif defined TP_EU
        enableCrashScreen = reinterpret_cast<u32*>( 0x8000B878 );
#elif defined TP_JP
        enableCrashScreen = reinterpret_cast<u32*>( 0x8000B8A4 );
#endif

        // Perform the overwrites

        /* If the address is loaded into the cache before the overwrite is made,
        then the cache will need to be cleared after the overwrite */

        // Enable the crash screen
        *enableCrashScreen = 0x48000014;     // b 0x14

        // Modify dComIfGs_Wolf_Change_Check to return true when checking to see if mdh is complete
        u32 wolfChangeCheckAddress = reinterpret_cast<u32>( &tp::d_com_inf_game::dComIfGs_Wolf_Change_Check );
        *reinterpret_cast<u32*>( wolfChangeCheckAddress + 0x108 ) = 0x3be00000;     // Previous 0x3be00001
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
        LadderVars->ladderClimbInitSpeed = 1.8;
        LadderVars->ladderReachTopClimbUpSpeed = 1.765;
        LadderVars->ladderTopStartClimbDownSpeed = 1.8;
        LadderVars->ladderBottomGetOffSpeed = 1.8;
        LadderVars->ladderClimbSpeed = 1.575;
        LadderVars->wallClimbHorizontalSpeed = 2.0;
        LadderVars->wallClimbVerticalSpeed = 1.875;
    }

    void removeIBLimit()
    {
        // li 0
        u32 li = 0x38600000;
        // b +4C
        u32 b = 0x4800004C;

        // CheckHeavyState overwrite
        u32 checkHeavyStateOnAddress = reinterpret_cast<u32>( &tp::d_a_alink::checkHeavyStateOn );
        *reinterpret_cast<u32*>( checkHeavyStateOnAddress + 0x84 ) = li;

        u32 setStickDataAddress = reinterpret_cast<u32>( &tp::d_a_alink::setStickData );
        *reinterpret_cast<u32*>( setStickDataAddress + 0x5FC ) = b;
    }

    void increaseWalletSize()
    {
        // li
        u32 li = 0x38600000;
        u32 cmpwi = 0x2c000000;

        u32 getRupeeMaxAddress = reinterpret_cast<u32>( &tp::d_save::getRupeeMax );
        u32 setWalletMaxNumAddress = reinterpret_cast<u32>( &tp::d_menu_collect::setWalletMaxNum );

        // 300 -> 1000
        *reinterpret_cast<u32*>( getRupeeMaxAddress + 0x30 ) = li | 0x000003E8;

        // 600 -> 5000
        *reinterpret_cast<u32*>( getRupeeMaxAddress + 0x38 ) = li | 0x00001388;

        // 1000 -> 9999 (> 4 digit not good)
        *reinterpret_cast<u32*>( getRupeeMaxAddress + 0x40 ) = li | 0x0000270F;

        // 600 -> 5000
        *reinterpret_cast<u32*>( setWalletMaxNumAddress + 0x18 ) = cmpwi | 0x00001388;

        // 300 -> 1000
        *reinterpret_cast<u32*>( setWalletMaxNumAddress + 0x24 ) = cmpwi | 0x000003E8;

        // 1000 -> 9999
        *reinterpret_cast<u32*>( setWalletMaxNumAddress + 0x30 ) = cmpwi | 0x0000270F;
    }

    void skipSewers()
    {
        if ( Singleton::getInstance()->isSewerSkipEnabled == 1 )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "-> Skipping Sewers" );

            // Set sewers flags
            giveSense();

            // We should be wolf
            setFirstTimeWolf();

            // Set sewers flags (escaped cell cs, Midna intro cs, unchained wolf link, zelda cutscene, First trip to Sewers
            // Done)
            gameInfo.scratchPad.eventBits[0x05] |= 0x7A;

            // Set more sewers flags (talk to midna after escaping cell in sewers, listened to first guard in sewers)
            gameInfo.scratchPad.eventBits[0x1A] |= 0x82;

            // Set more sewers flags (spawned into sewers twilight as wolf)
            gameInfo.scratchPad.eventBits[0x4D] |= 0x8;

            gameInfo.scratchPad.equipedItems.sword = 0x3F;

            // Load back to Ordon Spring
            tools::triggerSaveLoad( stage::allStages[Stage_Ordon_Spring], 0x1, 0x3, 0x4 );
        }
        giveMidna();
        gameInfo.scratchPad.equipedItems.sword = 0x3F;
    }

    void setFieldBits()
    {
        if ( ( Singleton::getInstance()->isGateUnlockEnabled == 1 && ( gameInfo.localAreaNodes.unk_0[0x8] & 0x10 ) == 0 ) ||
             tools::checkItemFlag( ItemFlags::Gate_Keys ) )
        {
            gameInfo.unk_979[0x7] |= 0x6;     // 2 = lanyru gate 4 = eldin gorge gate
        }
    }

    void skipGoats()
    {
        if ( tp::d_com_inf_game::current_state == 0x63 )
        {     // goats 1
            if ( Singleton::getInstance()->isGoatSkipEnabled == 1 )
            {
                strcpy( sysConsolePtr->consoleLine[20].line, "-> Skipping Goats 1" );

                // Load back to Ordon ranch after goats 1
                tools::triggerSaveLoad( stage::allStages[Stage_Ordon_Ranch], 0x0, 0x4, 0x7 );
            }
        }
        else if ( tp::d_com_inf_game::current_state == 0x39 )
        {     // goats 2
            if ( Singleton::getInstance()->isGoatSkipEnabled == 1 )
            {
                strcpy( sysConsolePtr->consoleLine[20].line, "-> Skipping Goats 2" );

                gameInfo.localAreaNodes.unk_0[0xE] |= 0x2;      // set flag for Fado text before goats
                gameInfo.localAreaNodes.unk_0[0x9] |= 0x60;     // set flag for day 3 intro cs and goats 2 done
                // gameInfo.localAreaNodes.unk_0[0x9] |= 0x40;//set flag for day 3 intro cs and goats 2 done

                // Load back to Ordon village
                tools::triggerSaveLoad( stage::allStages[Stage_Ordon_Village], 0x0, 0x19, 0x8 );
            }
        }
    }

    void setGroveFlags()
    {
        if ( Singleton::getInstance()->isMSPuzzleSkipEnabled == 1 && ( gameInfo.localAreaNodes.unk_0[0xB] & 0x4 ) == 0 )
        {
            strcpy( sysConsolePtr->consoleLine[20].line, "Skipping MS Puzzle" );
            gameInfo.localAreaNodes.unk_0[0xB] |= 0x4;     // skip Sacred Grove Puzzle
        }
    }

    void checkBossKeysey()
    {
        if ( Singleton::getInstance()->isBossKeyseyEnabled == 1 )
        {
            tp::d_com_inf_game::AllAreaNodes* dungeonAreaNodesPtr = &gameInfo.scratchPad.allAreaNodes;
            dungeonAreaNodesPtr->Forest_Temple.dungeon.bigKeyGotten = 0b1;          // unlock Diababa Door
            dungeonAreaNodesPtr->Goron_Mines.dungeon.bigKeyGotten = 0b1;            // unlock Fryus Door
            dungeonAreaNodesPtr->Lakebed_Temple.dungeon.bigKeyGotten = 0b1;         // unlock Morpheel Door
            dungeonAreaNodesPtr->Arbiters_Grounds.dungeon.bigKeyGotten = 0b1;       // unlock Stallord Door
            dungeonAreaNodesPtr->Snowpeak_Ruins.dungeon.bigKeyGotten = 0b1;         // unlock Blizzetta Door
            dungeonAreaNodesPtr->Temple_of_Time.dungeon.bigKeyGotten = 0b1;         // unlock Armaghoma Door
            dungeonAreaNodesPtr->City_in_the_Sky.dungeon.bigKeyGotten = 0b1;        // unlock Argorok Door
            dungeonAreaNodesPtr->Palace_of_Twilight.dungeon.bigKeyGotten = 0b1;     // unlock Zant Door
            dungeonAreaNodesPtr->Hyrule_Castle.dungeon.bigKeyGotten = 0b1;          // unlock Ganon Door
        }
    }

    /* Still in the works
    void unlockDungeonDoors()
    {
        if (Singleton::getInstance()->isMSPuzzleSkipEnabled == 1 && (gameInfo.localAreaNodes.unk_0[0xB] & 0x4) == 0)
        {
            //Forest Temple
            allAreaNodesPtr->Forest_Temple.unk_0[0xA] |= 0x8; //unlock Second Monkey Door
            allAreaNodesPtr->Forest_Temple.unk_0[0xB] |= 0x80; //unlock Gale Puzzle Monkey Door

            //Goron Mines


            //Lakebed Temple

            //Arbiter's Grounds

            //Snowpeak Ruins

            //Temple of Time

            //City in the Sky

            //Palace of Twilight

            //Hyrule Castle
        }
    }*/

    void earlyCiTS()
    {
        if ( Singleton::getInstance()->isEarlyCiTSEnabled == 1 )
        {
            gameInfo.scratchPad.eventBits[0x3B] |= 0x8;     // repairs Cannon at lake
        }
    }

    void earlyDesert()
    {
        if ( Singleton::getInstance()->isEarlyDesertEnabled )
        {
            gameInfo.scratchPad.eventBits[0x26] |= 0x80;     // Allow you to use the cannon in the desert
        }
    }

    void accessDesert()
    {
        if ( tools::checkItemFlag( ItemFlags::Shadow_Crystal ) )
        {
            return;
        }
        else
        {
            strncpy( gameInfo.nextStageVars.nextStage,
                     stage::allStages[Stage_Lake_Hylia],
                     sizeof( gameInfo.nextStageVars.nextStage ) - 1 );
            gameInfo.nextStageVars.nextRoom = 0x0;
            gameInfo.nextStageVars.nextSpawnPoint = 0x4D;
            tp::d_item::execItemGet( 0x2 );
        }
    }

    void breakBarrier()
    {
        if ( Singleton::getInstance()->isEarlyHCEnabled == 1 && ( ( gameInfo.scratchPad.eventBits[0x1E] & 0x8 ) != 0 ) )
        {
            gameInfo.scratchPad.eventBits[0x42] |= 0x8;
        }
        else if ( Singleton::getInstance()->isEarlyHCEnabled == 0 && ( ( gameInfo.scratchPad.eventBits[0x1E] & 0x8 ) != 0 ) &&
                  ( ( gameInfo.scratchPad.allAreaNodes.Palace_of_Twilight.dungeon.bossBeaten != 0 ) ) )
        {
            gameInfo.scratchPad.eventBits[0x42] |= 0x8;
        }
    }

    void setEscortState()
    {
        if ( ( ( gameInfo.scratchPad.eventBits[0x8] & 0x40 ) == 0 ) &&
             ( Singleton::getInstance()->isCartEscortSkipEnabled == 0 ) && tools::checkItemFlag( ItemFlags::Heros_Bow ) &&
             tools::checkItemFlag( ItemFlags::Boomerang ) && gameInfo.scratchPad.clearedTwilights.Lanayru == 0b1 &&
             ( ( Singleton::getInstance()->isMDHSkipEnabled == 1 ) ||
               ( !tp::d_a_alink::dComIfGs_isEventBit( 0xC1 ) ||
                 ( tp::d_a_alink::dComIfGs_isEventBit( 0xC1 ) && tp::d_a_alink::dComIfGs_isEventBit( 0x1E08 ) ) ) ) )
        {
            gameInfo.nextStageVars.nextState = 0x8;
            gameInfo.nextStageVars.nextSpawnPoint = 0x14;
        }
    }

    void setFirstTimeWolf()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Set first time wolf" );

        gameInfo.scratchPad.unk_1F[0x11] |= 1;
    }

    void setLanayruWolf()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Set wolf" );

        if ( gameInfo.scratchPad.form == 0 && !tools::checkItemFlag( ItemFlags::Shadow_Crystal ) &&
             !tools::checkItemFlag( ItemFlags::Vessel_Of_Light_Lanayru ) )
        {
            strncpy( gameInfo.nextStageVars.nextStage,
                     stage::allStages[Stage_Hyrule_Field],
                     sizeof( gameInfo.nextStageVars.nextStage ) - 1 );
            gameInfo.nextStageVars.nextRoom = 0x9;
            gameInfo.nextStageVars.nextSpawnPoint = 0xA;
        }
        else
        {
            return;
        }
    }

    void setHuman()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Set human" );

        gameInfo.scratchPad.form = 0;
    }

    void setWolf()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Set wolf" );

        gameInfo.scratchPad.form = 1;
    }

    void giveSense()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Give Senses" );

        // Set the Sense (sewers) flag
        gameInfo.scratchPad.eventBits[0x43] |= 0x8;
    }

    void giveEpona()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Give Epona" );

        // Set Epona tamed
        gameInfo.scratchPad.eventBits[0x6] |= 0x1;
    }

    void giveMasterSword()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Give MasterSword" );

        // Set Master sword inventory flag
        tools::setItemFlag( ItemFlags::Master_Sword );

        // Equip Master sword (0x49 / 73)
        gameInfo.scratchPad.equipedItems.sword = 0x49;
    }

    void giveMidna()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Give Midna (sewers)" );

        // Set Midna sewers flag
        gameInfo.scratchPad.eventBits[0xC] |= 0x10;
    }

    void giveMidnaTransform()
    {
        strcpy( sysConsolePtr->consoleLine[20].line, "-> Give Midna Transform" );

        // Set Midna Transform flag (gets set at Master Sword)
        gameInfo.scratchPad.eventBits[0xD] |= 0x4;
    }

    void allowFaronEscape()
    {
        if ( Singleton::getInstance()->isForestEscapeEnabled == 0 )
        {
            if ( ( ( gameInfo.scratchPad.eventBits[0x6] & 0x2 ) != 0 ) || ( gameInfo.scratchPad.eventBits[0x6] & 0x10 ) == 0 )
            {
                return;
            }
            else
            {
                gameInfo.nextStageVars.nextState = 0x0;
            }
        }
    }

    void setLanternFlag() { gameInfo.scratchPad.eventBits[0xF] |= 0x1; /*got lantern from Coro*/ }

    void skipMDHCS()
    {
        if ( Singleton::getInstance()->isMDHSkipEnabled == 1 )
        {
            strncpy( gameInfo.nextStageVars.nextStage,
                     stage::allStages[Stage_Lake_Hylia],
                     sizeof( gameInfo.nextStageVars.nextStage ) - 1 );
            gameInfo.nextStageVars.nextSpawnPoint = 0x7;
            gameInfo.nextStageVars.nextRoom = 0x0;
        }
    }

    void skipMDH()
    {
        if ( Singleton::getInstance()->isMDHSkipEnabled == 1 )
        {
            // set MDH flags
            gameInfo.scratchPad.eventBits[0xC] |= 0x1;      // MDH Started
            gameInfo.scratchPad.eventBits[0x1E] |= 0x8;     // MDH Completed
        }
    }

    void skipTextAndCS()
    {
        // Set Scratchpad Pointer
        tp::d_com_inf_game::ScratchPad* scratchPadPtr = &gameInfo.scratchPad;

        // Set Letters Pointer
        tp::d_com_inf_game::Letters* LettersPtr = &scratchPadPtr->recivedLetters;
        LettersPtr->From_Wife_of_Yeto = 0b1;
        LettersPtr->Update = 0b1;
        LettersPtr->Now_open_for_business = 0b1;
        LettersPtr->Agithas_Dream = 0b1;
        LettersPtr->Hey_kid = 0b1;
        LettersPtr->Challenge_for_you = 0b1;
        LettersPtr->They_came_so_quickly = 0b1;
        LettersPtr->Heroes_come_together = 0b1;
        LettersPtr->URGENT_Bomb_arrows = 0b1;
        LettersPtr->Rare_item_in_stock = 0b1;
        LettersPtr->Good_stuff_inside = 0b1;
        LettersPtr->URGENT_NOTICE = 0b1;
        LettersPtr->Post_office_notice = 0b1;
        LettersPtr->Dear_Adventurer_1 = 0b1;
        LettersPtr->Dear_Adventurer_2 = 0b1;
        LettersPtr->About_Ilias_memory = 0b1;

        // Set Event Bits
        u8* eventBitsPtr = &scratchPadPtr->eventBits[0];
        eventBitsPtr[0x1] |= 0x40;      // talked to Yeto First Time
        eventBitsPtr[0x3] |= 0x90;      // Jaggle Calls out to link, talked to squirrel as wolf in Ordon
        eventBitsPtr[0x5] |= 0x10;      // unchain wolf link
        eventBitsPtr[0x6] |= 0xC0;      // CS after beating Ordon Shadow, cs after entering Faron twilight
        eventBitsPtr[0x7] |= 0x20;      // Talked to Bo outside his house
        eventBitsPtr[0xB] |= 0x20;      // Talked to Yeta First Time
        eventBitsPtr[0x10] |= 0x2;      // Talked to Jaggle after climbing vines
        eventBitsPtr[0xF] |= 0x40;      // Talked to Doctor for the first time
        eventBitsPtr[0x5E] |= 0x10;     // Midna Text After Beating Forest Temple
        eventBitsPtr[0x40] |= 0x88;     // have been to desert (prevents cannon warp crash) saved monkey from puppets
        eventBitsPtr[0x1B] |= 0x78;     // skip the monkey escort
        eventBitsPtr[0x1D] = 0x40;      // fight bublin after Fyer
        eventBitsPtr[0x22] |= 0x1;      // Plumm initial CS watched
        eventBitsPtr[0x26] |= 0x2;      // Talked to Yeto on Snowpeak
        eventBitsPtr[0x2C] |= 0x10;     // Mirror Rose
        eventBitsPtr[0x28] |= 0x40;     // Used Ooccoo for the First Time
        eventBitsPtr[0x37] |= 0x4;      // Postman Twilight Text
        eventBitsPtr[0x38] |= 0x6;      // Enter Hena Cabin CS
        eventBitsPtr[0x3A] |= 0x1;      // Talked to Ralis
        eventBitsPtr[0x40] |= 0x2;      // Agreed to help Rusl after snowpeak
        eventBitsPtr[0x42] |= 0x3;      // Watched post ToT Ooccoo CS and triggered monkey puppet CS
        eventBitsPtr[0x45] |= 0x8;      // Postman Letters Text
        eventBitsPtr[0x4A] |= 0x10;     // Talo Cage CS
        eventBitsPtr[0x3E] |= 0x2;      // city OoCCoo CS watched
        eventBitsPtr[0x59] |= 0x40;     // Postman Met
        eventBitsPtr[0x5D] |= 0x40;     // Midna text after Kagorok FLight
        eventBitsPtr[0x54] |= 0x10;     // PoT story flag

        // Set Area Node Flags
        tp::d_com_inf_game::AllAreaNodes* allAreaNodesPtr = &scratchPadPtr->allAreaNodes;

        allAreaNodesPtr->Ordon.unk_0[0x9] |=
            0xAA;     // exit shield house CS watched, day 3 intro CS, bee nest CS, Ranch first time CS
        allAreaNodesPtr->Ordon.unk_0[0xA] |= 0xF;       // Ilia spring CS, Ordon Village CS
        allAreaNodesPtr->Ordon.unk_0[0xD] |= 0x82;      // Approach Faron Twilight CS, Shield house intro cs
        allAreaNodesPtr->Ordon.unk_0[0xE] |= 0x4;       // Midna CS after watching Bo and Jaggle Talk about shield
        allAreaNodesPtr->Ordon.unk_0[0xF] |= 0xC;       // rusl talking to wife CS
        allAreaNodesPtr->Ordon.unk_0[0x17] |= 0x80;     // enter village as wolf CS

        allAreaNodesPtr->Sewers.unk_0[0x8] |= 0x38;     // wake up in Jail CS, text after first gate, text after rooftops
        allAreaNodesPtr->Sewers.unk_0[0x9] |= 0x18;     // midna cs after digging out, midna intro cs
        allAreaNodesPtr->Sewers.unk_0[0xA] |= 0x41;     // zelda tower intro cs, outside top door intro cs
        allAreaNodesPtr->Sewers.unk_0[0xB] |= 0x38;     // other side of fence cs, top of stairway cs watched, stairway intro cs
        allAreaNodesPtr->Sewers.unk_0[0xF] |=
            0x11;     // midna cs after digging out watched, midna text when approaching first rooftop guard

        allAreaNodesPtr->Faron.unk_0[0x9] |= 0x2;       // Saved monkey from puppets
        allAreaNodesPtr->Eldin.unk_0[0x9] |= 0x40;      // goron mines DM cs
        allAreaNodesPtr->Eldin.unk_0[0x14] |= 1;        // give midna jumps for top of sanctuary
        allAreaNodesPtr->Eldin.unk_0[0x10] |= 0x10;     // skip Graveyard CS
        allAreaNodesPtr->Eldin.unk_0[0x11] |= 0x8;      // midna text after meteor
        allAreaNodesPtr->Eldin.unk_0[0x13] |= 0x20;     // skip Kak CS
        allAreaNodesPtr->Eldin.unk_0[0x16] |= 0x10;     // Watched Colin saved CS

        allAreaNodesPtr->Lanayru.unk_0[0xB] |= 0x81;      // Zora domain frozen CS, talked to reluta
        allAreaNodesPtr->Lanayru.unk_0[0xC] |= 0x1;       // midna text after jumping to lake from bridge
        allAreaNodesPtr->Lanayru.unk_0[0xD] |= 0x10;      // ZD exit CS
        allAreaNodesPtr->Lanayru.unk_0[0xE] |= 0x20;      // midna text after kagorok CS trigger
        allAreaNodesPtr->Lanayru.unk_0[0x12] |= 0x40;     // midna text after frozen zd
        allAreaNodesPtr->Lanayru.unk_0[0x16] |= 0x80;     // watched Ooccoo CiTS CS

        allAreaNodesPtr->Hyrule_Field.unk_0[0xB] |= 0x60;     // cutscene before eldin twilight and Lanayru twilight
        allAreaNodesPtr->Hyrule_Field.unk_0[0xD] |= 0x80;     // midna text after warping gorge bridge
        allAreaNodesPtr->Hyrule_Field.unk_0[0xF] |=
            0xB;     // cutscene for Gorge Bridge Watched, Ilia Scent CS, midna text after lanayru field cs
        allAreaNodesPtr->Hyrule_Field.unk_0[0xE] |= 0x20;     // cutscene for entering Field Watched
        allAreaNodesPtr->Hyrule_Field.unk_0[0x8] |= 0x81;     // Midna text for warping the bridge, lanayru field CS
        allAreaNodesPtr->Hyrule_Field.unk_0[0x9] |= 0x9;      // cs before kak, watched CS of Eldin Bridge returning
        allAreaNodesPtr->Hyrule_Field.unk_0[0x16] |=
            0x13;     // midna text after entering eldin twilight, midna text after entering Lanayru twilight, midna text before
                      // eldin twilight, midna text after eldin twilight
        allAreaNodesPtr->Hyrule_Field.unk_0[0x17] |= 0x10;     // youth scent CS

        allAreaNodesPtr->Sacred_Grove.unk_0[0x8] |=
            0x38;     // lost woods intro cs, human block cs, midna text after human block

        allAreaNodesPtr->Snowpeak.unk_0[0x8] |= 0x4;     // Mountiain top CS
        allAreaNodesPtr->Snowpeak.unk_0[0xB] |= 0x3;     // Snowpeak Entrance CS, midna text in front of SPR

        allAreaNodesPtr->Castle_Town.unk_0[0xA] |= 0x4;     // STAR Tent CS
        allAreaNodesPtr->Castle_Town.unk_0[0xF] |= 0x4;     // Jovani House CS

        allAreaNodesPtr->Gerudo_Desert.unk_0[0x13] |= 0x40;     // watched the CS when entering the desert
        allAreaNodesPtr->Gerudo_Desert.unk_0[0xA] |= 0x10;      // Mirror Chamber Intro CS

        allAreaNodesPtr->Forest_Temple.unk_0[0x8] |=
            0x61;     // set Midna Text for Ook Bridge Broken, Boomerang, and freeing first monkey
        allAreaNodesPtr->Forest_Temple.unk_0[0x9] |= 0x40;     // Ook Bridge Broken
        allAreaNodesPtr->Forest_Temple.unk_0[0xA] |= 0x3;      // Boko bridge CS, one bridge CS
        allAreaNodesPtr->Forest_Temple.unk_0[0xE] |= 0x22;     // Worm Bridge CS, second monkey CS
        allAreaNodesPtr->Forest_Temple.unk_0[0xF] |=
            0xE;     // Midna text for pre-diababa room and open room monkey, Big Baba CS
        allAreaNodesPtr->Forest_Temple.unk_0[0x10] |= 0x14;     // Midna text for compass, eat open area bridge CS
        allAreaNodesPtr->Forest_Temple.unk_0[0x16] |= 0x2;      // Entrance CS watched

        allAreaNodesPtr->Goron_Mines.unk_0[0x8] |= 0x1F;     // upper magnet CS, first room switch 3 cs, first room switch 2 cs,
                                                             // outside bridge cs, crystal switch bridge cs
        allAreaNodesPtr->Goron_Mines.unk_0[0x9] |= 0x20;     // bottom magnet cs
        allAreaNodesPtr->Goron_Mines.unk_0[0xD] |= 0x80;     // outside magnet cs
        allAreaNodesPtr->Goron_Mines.unk_0[0xE] |= 0x30;     // enter tall room cs
        allAreaNodesPtr->Goron_Mines.unk_0[0xF] |= 0x20;     // open room CS
        allAreaNodesPtr->Goron_Mines.unk_0[0x10] |= 0xCC;     // lava slug room cs, outside area beamos wall cs, second magnet
                                                              // in crystal switch cs, crystal switch room gate
        allAreaNodesPtr->Goron_Mines.unk_0[0x11] |=
            0xB8;     // opening cs, first room gate cs, first room switch 1 cs, water room magnet cs
        allAreaNodesPtr->Goron_Mines.unk_0[0x17] |=
            0xF;     // lava slug room cs watched, lava slug room flag, tall room CS flags

        allAreaNodesPtr->Lakebed_Temple.unk_0[0x8] |= 0x2;       // stalactite midna text
        allAreaNodesPtr->Lakebed_Temple.unk_0[0xF] |= 0x3;       // one gear room spinning cs, two gear room spinning cs
        allAreaNodesPtr->Lakebed_Temple.unk_0[0x14] |= 0x7;      // main room cs, front bridge cs, opening cs
        allAreaNodesPtr->Lakebed_Temple.unk_0[0x15] |= 0x20;     // staircase turn CS
        allAreaNodesPtr->Lakebed_Temple.unk_0[0x16] |= 0x8;      // chu worm cs watched

        allAreaNodesPtr->Arbiters_Grounds.unk_0[0xB] |= 0x20;     // spinner wall cs
        allAreaNodesPtr->Arbiters_Grounds.unk_0[0xE] |= 0x10;     // opening CS
        allAreaNodesPtr->Arbiters_Grounds.unk_0[0x12] |= 0x8;     // cs after raising rails

        allAreaNodesPtr->Snowpeak_Ruins.unk_0[0x10] |= 0x10;     // enter cannonball room cs
        allAreaNodesPtr->Snowpeak_Ruins.unk_0[0x14] |= 0x41;     // entrance cs, midna text after bedroom key
        allAreaNodesPtr->Snowpeak_Ruins.unk_0[0x15] |=
            0x2A;     // freezard tower cs, enter NE room cs watched, East Courtyard Dig CS
        allAreaNodesPtr->Snowpeak_Ruins.unk_0[0x16] |= 0x28;     // midna text after cheese and pumpkin

        allAreaNodesPtr->Temple_of_Time.unk_0[0x8] |= 0x80;     // Midna text about missing statue
        allAreaNodesPtr->Temple_of_Time.unk_0[0x9] |= 0x3C;     // scales cs, entrance cs, scales move cs
        allAreaNodesPtr->Temple_of_Time.unk_0[0xA] |= 0x8;      // statue move cs watched
        allAreaNodesPtr->Temple_of_Time.unk_0[0xF] |= 0x8;      // gate outside darknut cs
        allAreaNodesPtr->Temple_of_Time.unk_0[0x12] |=
            0xEE;     // first room switch cs, 8f hall switch cs, before darknut gate open cs, elevator room switches cs, 3F
                      // hall switch cs, 2F hall switch cs
        allAreaNodesPtr->Temple_of_Time.unk_0[0x14] |= 0x4;     // darknut room gate open cs

        allAreaNodesPtr->City_in_the_Sky.unk_0[0xC] |=
            0xF;     // fan tower cs, room before aerolfos, first room exit cs, entrance cs
        allAreaNodesPtr->City_in_the_Sky.unk_0[0xD] |= 0x6;     // west bridge extended cs, east bridge extended cs

        allAreaNodesPtr->Palace_of_Twilight.unk_0[0x9] |= 0x4;      // Phantom zant 1 cs watched
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0xB] |= 0x2;      // Entrance CS watched
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0xC] |= 0x2;      // Midna Text when west hand seals sol
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0xD] |= 0x1;      // Midna text after forced transform
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0xE] |= 0xB0;     // Midna text after west hand drops sol, midna text about
                                                                    // transformed twili, midna text after post zant save
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0xF] |=
            0xC0;     // Midna text after light sword, midna text after west sol in slot
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0x12] |= 0x4;      // midna text after re-entering west after sol stolen
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0x13] |= 0x2;      // midna text after entrance cs
        allAreaNodesPtr->Palace_of_Twilight.unk_0[0x16] |= 0x10;     // watched EW 2nd room stairs CS

        allAreaNodesPtr->Hyrule_Castle.unk_0[0x9] |= 0x1;       // graveyard entrance cs
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x11] |= 0xC;      // east wing cs
        allAreaNodesPtr->Hyrule_Castle.unk_0[0xe] |= 0x1;       // midna text after outside gale
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x10] |= 0x24;     // entrance cs, chandelier room east cs watched
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x13] |= 0x40;     // Darknuts room CS
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x14] |=
            0xB;     // Midna text post KB4, lizalfos Room CS, midna text after graveyard SK
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x16] |= 0x1;      // lone darknut room se torch cs
        allAreaNodesPtr->Hyrule_Castle.unk_0[0x17] |= 0x80;     // lone darknut room me torch watched

        // Set Local Area Node flags
        allAreaNodesPtr->Faron.unk_0[0x8] = 0xFB;     // give midna jumps in mist area
        allAreaNodesPtr->Faron.unk_0[0xC] |=
            0x90;     // set flag for midna to think you followed the monkey in the mist, trill lets you shop
        allAreaNodesPtr->Faron.unk_0[0x12] |= 0x4;      // mark read the midna text when you warp to N Faron for bridge
        allAreaNodesPtr->Faron.unk_0[0xF] |= 0x8;       // set flag for midna text after twilight
        allAreaNodesPtr->Faron.unk_0[0xE] |= 0x9;       // cs after entering Faron,spring cs with spirit
        allAreaNodesPtr->Faron.unk_0[0x17] |= 0xC0;     // kill bugs in Coro's House

        // Apply Overrides for custom chests
        gameInfo.scratchPad.eventBits[0x49] |= 0x2; /*Bought Slingshot from Sera*/

        // Apply Randomizer Options
        checkBossKeysey();
        earlyCiTS();
        earlyDesert();

        if ( Singleton::getInstance()->isEarlyToTEnabled == 1 )
        {
            gameInfo.scratchPad.allAreaNodes.Sacred_Grove.unk_0[0x17] |= 0x10;     // SG Portal
            gameInfo.scratchPad.allAreaNodes.Sacred_Grove.unk_0[0x9] |= 0x70;      // ToT Stairs
            gameInfo.scratchPad.allAreaNodes.Sacred_Grove.unk_0[0xB] |= 0x10;      // Pushed block as human
        }

        if ( Singleton::getInstance()->isEarlyPoTEnabled == 1 )
        {
            gameInfo.scratchPad.eventBits[0x20] |= 0x2;     // CiTS Story Flag
        }

        if ( Singleton::getInstance()->isGMStoryPatch == 1 )
        {
            gameInfo.scratchPad.eventBits[0x7] |= 0x1;                      // Mines Story Flag
            gameInfo.scratchPad.eventBits[0x13] |= 0x20;                    // watched post GM cs
            gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x14] |= 0x10;     // barnes sells bombs
        }

        if ( Singleton::getInstance()->isCartEscortSkipEnabled == 1 )
        {
            gameInfo.scratchPad.eventBits[0x8] |= 0x40;                     // escort started
            gameInfo.scratchPad.eventBits[0x8] |= 0x10;                     // escort finished
            gameInfo.scratchPad.eventBits[0x8] |= 0x4;                      // got zora armor from Rutela
            gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x17] |= 0x40;     // remove rock in graveyard
            gameInfo.scratchPad.allAreaNodes.Eldin.unk_0[0x16] |= 0x40;
        }

        if ( Singleton::getInstance()->startWithCrystal == 1 )
        {
            giveMidnaTransform();
            tools::setItemFlag( ItemFlags::Shadow_Crystal );

            if ( Singleton::getInstance()->isMDHSkipEnabled == 1 )
            {
                gameInfo.scratchPad.unk_1F[0x11] |= 0x8;     // Midna on Back
            }
        }

        if ( Singleton::getInstance()->isIntroSkipped == 1 )
        {
            // set Ordon Days 1,2, and 3 Flags
            allAreaNodesPtr->Ordon.unk_0[0x3] |= 0x1;      // Gave cradle to Uli, got Fishing Rod from uli
            eventBitsPtr[0x4] |= 0x4;                      // Talked to Uli Day 1
            allAreaNodesPtr->Ordon.unk_0[0x9] |= 0x40;     // Goats 2 done
            allAreaNodesPtr->Ordon.unk_0[0xC] |= 0x1A;     // Day 2 CS watched, Goats 1 done, spawn wooden sword chest
            eventBitsPtr[0x45] |= 0x10;                    // Saved Talo
            eventBitsPtr[0x46] |= 0x11;                    // Came Back to Links House with Epona Day 1, took cradle from monkey
            eventBitsPtr[0x47] |= 0x2F;                    // Called Epona and Talked to everyone
            eventBitsPtr[0x48] |= 0x10;                    // Talked to Sera
            eventBitsPtr[0x62] |= 0x80;                    // Talked to Colin Day 1
            eventBitsPtr[0x4A] |= 0x60;                    // Day 1 done, sword training done

            eventBitsPtr[0x2] |= 0x40;                     // Slingshot and Sword Training started
            allAreaNodesPtr->Ordon.unk_0[0x8] |= 0x20;     // Sword Training Started
            eventBitsPtr[0x10] |= 0x1;                     // Cat got Fish
            eventBitsPtr[0x16] |= 0x1;                     // Day 2 done

            allAreaNodesPtr->Ordon.unk_0[0xE] |= 0x2;      // set flag for Fado text before goats
            allAreaNodesPtr->Ordon.unk_0[0x9] |= 0x60;     // set flag for day 3 intro cs and goats 2 done
            eventBitsPtr[0x15] |= 0x80;                    // Watched CS for goats 2 done

            eventBitsPtr[0x1] |= 0x4;     // Talked to Colin Day 3
            eventBitsPtr[0x3] |= 0x2;     // Gave Sword to Talo

            if ( Singleton::getInstance()->isSewerSkipEnabled == 1 )
            {
                // Set sewers flags (escaped cell cs, Midna intro cs, unchained wolf link, zelda cutscene, First trip to Sewers
                // Done) Set more sewers flags (talk to midna after escaping cell in sewers, listened to first guard in sewers)
                gameInfo.scratchPad.eventBits[0x1A] |= 0x82;

                // Set more sewers flags (spawned into sewers twilight as wolf)
                gameInfo.scratchPad.eventBits[0x4D] |= 0x8;

                if ( Singleton::getInstance()->isTwilightSkipped == 1 )
                {
                    // Set Faron Twilight Flags
                    scratchPadPtr->clearedTwilights.Faron = 0b1;     // Clear Faron Twilight
                    tools::setItemFlag( ItemFlags::Vessel_Of_Light_Faron );
                    scratchPadPtr->tearCounters.Faron = 16;
                    eventBitsPtr[0x5] |= 0xFF;     // Ensure Epona is Stolen, give Midna Charge, finished sewers, midna text for
                                                   // entering Faron Twilight, Met zelda in sewers
                    eventBitsPtr[0x6] |= 0x10;     // Faron Twilight Progression flag
                    eventBitsPtr[0xC] |= 0x8;      // Set Sword and Shield to not be on back
                    tools::setItemFlag( ItemFlags::Heros_Clothes );

                    // Set Eldin Twilight Flags
                    scratchPadPtr->clearedTwilights.Eldin = 0b1;     // Clear Eldin Twilight
                    tools::setItemFlag( ItemFlags::Vessel_Of_Light_Eldin );
                    scratchPadPtr->tearCounters.Eldin = 16;
                    eventBitsPtr[0x6] |= 0x1;       // tame Epona
                    eventBitsPtr[0xA] |= 0x8;       // Beat KB1
                    eventBitsPtr[0x14] |= 0x10;     // Put Bo Outside
                    eventBitsPtr[0x7] |= 0xDE;      // skip Gor Coron Sumo and Enter Mines also Trigger KB1 and mark Post-KB1 CS
                                                    // as watched, Eldin Twilight Story Progression Flag
                    eventBitsPtr[0x41] |= 0x10;     // Told Fado about the Kids

                    // Set Lanayru Twilight Flags
                    scratchPadPtr->clearedTwilights.Lanayru = 0b1;     // Clear Lanayru Twilight
                    tools::setItemFlag( ItemFlags::Vessel_Of_Light_Lanayru );
                    scratchPadPtr->tearCounters.Lanayru = 16;
                    allAreaNodesPtr->Hyrule_Field.unk_0[0xB] |= 0x80;     // water on Field map
                    allAreaNodesPtr->Hyrule_Field.unk_0[0xF] |= 0x10;     // open south CT Shortcut to Faron
                    allAreaNodesPtr->Lanayru.unk_0[0xF] |= 0x1;           // water on Map
                    eventBitsPtr[0x30] |= 0x40;                           // gave springwater to south CT goron
                    eventBitsPtr[0x8] |= 0x80;                            // ZD Thawed
                    eventBitsPtr[0xC] |= 0x2;                             // Lanayru Twilight Story Progression Flag
                    eventBitsPtr[0xA] |= 0x10;                            // Kagorok Howl at Lake

                    // Skip MDH?
                    if ( Singleton::getInstance()->isMDHSkipEnabled == 1 )
                    {
                        // set MDH flags
                        gameInfo.scratchPad.eventBits[0xC] |= 0x1;      // MDH Started
                        gameInfo.scratchPad.eventBits[0x1E] |= 0x8;     // MDH Completed
                    }

                    // Set sewers flags
                    giveMidna();
                    giveSense();

                    // Set sewers flags (zelda cutscenes, unchained wolf link, bla)
                    eventBitsPtr[0x05] |= 0x7A;

                    // Unlock Map Regions
                    scratchPadPtr->movingActors.exploredRegions.Snowpeak = 0b1;
                    scratchPadPtr->movingActors.exploredRegions.Desert = 0b1;
                    scratchPadPtr->movingActors.exploredRegions.Lanayru = 0b1;
                    scratchPadPtr->movingActors.exploredRegions.Eldin = 0b1;
                    scratchPadPtr->movingActors.exploredRegions.Faron = 0b1;
                    scratchPadPtr->movingActors.exploredRegions.Ordon = 0b1;

                    // Unlock Warps
                    allAreaNodesPtr->Ordon.unk_0[0xD] = 0x10;             // give Ordon Spring Warp
                    allAreaNodesPtr->Faron.unk_0[0x13] = 0x80;            // give S faron warp
                    allAreaNodesPtr->Faron.unk_0[0xB] = 0x4;              // give N faron warp
                    allAreaNodesPtr->Eldin.unk_0[0x9] |= 0x20;            // give Death Mountain Warp
                    allAreaNodesPtr->Eldin.unk_0[0x8] |= 0x80;            // give Kakariko Warp
                    allAreaNodesPtr->Hyrule_Field.unk_0[0xB] |= 0x8;      // give castle town warp
                    allAreaNodesPtr->Hyrule_Field.unk_0[0x9] |= 0x20;     // give Gorge Warp
                    allAreaNodesPtr->Hyrule_Field.unk_0[0x17] |= 0x8;     // give Eldin warp
                    allAreaNodesPtr->Lanayru.unk_0[0xB] |= 0x4;           // give Zora's Domain Warp
                    allAreaNodesPtr->Lanayru.unk_0[0xA] |= 0x4;           // give lake hylia warp
                }
            }

            // Set Other Flags
            u16* secondTempAddress = reinterpret_cast<u16*>( &eventBitsPtr[0xF7] );
            *secondTempAddress |= 0x1F4;     // make it so you only have to donate 500 Rupees to Charlo
            eventBitsPtr[0x20] |= 0x24;      // MS Story Progression Flag and ToT flag
            eventBitsPtr[0x1E] |= 0x80;      // Gor Ebizo at Malo Mart
            eventBitsPtr[0xA] |= 0x20;       // Steal Eldin Bridge
            eventBitsPtr[0xF] |= 0x8;        // Put Eldin BRidge Back

            // Faron Escape
            if ( Singleton::getInstance()->isForestEscapeEnabled == 1 )
            {
                eventBitsPtr[0x6] |= 0x26;     // warp the kak bridge, give map warp, set Forest Temple Story Flag
            }
            else
            {
                eventBitsPtr[0x6] |= 0x24;     // warp the kak bridge, give map warp
            }

            // Start Link in His House
            strncpy( gameInfo.nextStageVars.nextStage,
                     stage::allStages[Stage_Ordon_Interiors],
                     sizeof( gameInfo.nextStageVars.nextStage ) - 1 );
            gameInfo.nextStageVars.nextRoom = 0x4;
            gameInfo.nextStageVars.nextSpawnPoint = 0x4;
            gameInfo.scratchPad.skyAngle = 180;
        }
        else
        {
            // Set Other Flags
            u16* secondTempAddress = reinterpret_cast<u16*>( &eventBitsPtr[0xF7] );
            *secondTempAddress |= 0x1F4;     // make it so you only have to donate 500 Rupees to Charlo
            eventBitsPtr[0x20] |= 0x24;      // MS Story Progression Flag and ToT Flag
            eventBitsPtr[0x1E] |= 0x80;      // Gor Ebizo at Malo Mart
            eventBitsPtr[0xA] |= 0x20;       // Steal Eldin Bridge
            eventBitsPtr[0xF] |= 0x8;        // Put Eldin BRidge Back
        }
    }

    void setCustomItemData()
    {
        // Generic item variables
        tp::d_item_data::ItemResource* itemResourcePtr = &tp::d_item_data::item_resource[0];
        tp::d_item_data::ItemInfo* itemInfoPtr = &tp::d_item_data::item_info[0];
        u8* getSeTypePtr = &tp::d_a_alink::getSeType[0];

        // Vanilla Small Key variables
        tp::d_item_data::ItemResource* smallKeyItemResourcePtr = &itemResourcePtr[items::Small_Key];
        u32 smallKeyItemInfo = *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[items::Small_Key] ) );
        u8 smallKeySeType = getSeTypePtr[items::Small_Key];

        // Vanilla Big Key variables
        tp::d_item_data::ItemResource* bigKeyItemResourcePtr = &itemResourcePtr[items::Big_Key];
        u32 bigKeyItemInfo = *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[items::Big_Key] ) );
        u8 bigKeySeType = getSeTypePtr[items::Big_Key];

        // Vanilla Dungeon Map variables
        tp::d_item_data::ItemResource* dungeonMapItemResourcePtr = &itemResourcePtr[items::Dungeon_Map];
        u32 dungeonMapItemInfo = *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[items::Dungeon_Map] ) );
        u8 dungeonMapSeType = getSeTypePtr[items::Dungeon_Map];

        // Vanilla Compass variables
        tp::d_item_data::ItemResource* compassItemResourcePtr = &itemResourcePtr[items::Compass];
        u32 compassItemInfo = *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[items::Compass] ) );
        u8 compassSeType = getSeTypePtr[items::Compass];

        // Set the item info for the custom small keys to that of the current Small Key
        for ( u32 i = 0; i < sizeof( item::customSmallKeyItemIDs ) / sizeof( item::customSmallKeyItemIDs[0] ); i++ )
        {
            *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[item::customSmallKeyItemIDs[i]] ) ) = smallKeyItemInfo;

            getSeTypePtr[item::customSmallKeyItemIDs[i]] = smallKeySeType;

            memcpy( &itemResourcePtr[item::customSmallKeyItemIDs[i]],
                    smallKeyItemResourcePtr,
                    sizeof( tp::d_item_data::ItemResource ) );
        }

        // Set the item info for the custom big keys to that of the current Big Key
        for ( u32 i = 0; i < sizeof( item::customBigKeyItemIDs ) / sizeof( item::customBigKeyItemIDs[0] ); i++ )
        {
            *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[item::customBigKeyItemIDs[i]] ) ) = bigKeyItemInfo;

            getSeTypePtr[item::customBigKeyItemIDs[i]] = bigKeySeType;

            memcpy( &itemResourcePtr[item::customBigKeyItemIDs[i]],
                    bigKeyItemResourcePtr,
                    sizeof( tp::d_item_data::ItemResource ) );
        }

        // Set the item info for the custom dungeon map to that of the current Dungeon Map
        for ( u32 i = 0; i < sizeof( item::customDungeonMapItemIDs ) / sizeof( item::customDungeonMapItemIDs[0] ); i++ )
        {
            *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[item::customDungeonMapItemIDs[i]] ) ) =
                dungeonMapItemInfo;

            getSeTypePtr[item::customDungeonMapItemIDs[i]] = dungeonMapSeType;

            memcpy( &itemResourcePtr[item::customDungeonMapItemIDs[i]],
                    dungeonMapItemResourcePtr,
                    sizeof( tp::d_item_data::ItemResource ) );
        }

        // Set the item info for the custom compass to that of the current compass
        for ( u32 i = 0; i < sizeof( item::customCompassItemIDs ) / sizeof( item::customCompassItemIDs[0] ); i++ )
        {
            *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[item::customCompassItemIDs[i]] ) ) = compassItemInfo;

            getSeTypePtr[item::customCompassItemIDs[i]] = compassSeType;

            memcpy( &itemResourcePtr[item::customCompassItemIDs[i]],
                    compassItemResourcePtr,
                    sizeof( tp::d_item_data::ItemResource ) );
        }

        // Set the item info for the custom hidden skill items
        // Set the item info for the custom compass to that of the current compass
        for ( u32 i = 0; i < sizeof( item::customHiddenSkillItemIDs ) / sizeof( item::customHiddenSkillItemIDs[0] ); i++ )
        {
            itemResourcePtr[item::customHiddenSkillItemIDs[i]].arcName = "O_gD_memo";
            itemResourcePtr[item::customHiddenSkillItemIDs[i]].ringTexResIdx = 0x003D;
            itemResourcePtr[item::customHiddenSkillItemIDs[i]].modelResIdx = 0x0003;
            itemResourcePtr[item::customHiddenSkillItemIDs[i]].brkResIdx = 0xFFFF;
            itemResourcePtr[item::customHiddenSkillItemIDs[i]].tevFrm = 0x00;
            getSeTypePtr[item::customHiddenSkillItemIDs[i]] = 0x2;
        }

        // Set the Model and SeType for the Master/Light Swords. We will use the Ordon Sword for now.
        memcpy( &itemResourcePtr[items::Master_Sword],
                &itemResourcePtr[items::Ordon_Sword],
                sizeof( tp::d_item_data::ItemResource ) );
        itemResourcePtr[items::Master_Sword].ringTexResIdx = 0x0042;
        getSeTypePtr[items::Master_Sword] = getSeTypePtr[items::Clawshot];

        memcpy( &itemResourcePtr[items::Master_Sword_Light],
                &itemResourcePtr[items::Ordon_Sword],
                sizeof( tp::d_item_data::ItemResource ) );
        itemResourcePtr[items::Master_Sword_Light].ringTexResIdx = 0x0042;
        getSeTypePtr[items::Master_Sword_Light] = getSeTypePtr[items::Clawshot];

        // Set the model and SeType for all other items that need it.

        itemResourcePtr[items::Shadow_Crystal].ringTexResIdx = 0x002E;
        getSeTypePtr[items::Shadow_Crystal] = getSeTypePtr[items::Clawshot];

        itemResourcePtr[items::Bomb_Bag_Regular_Bombs].arcName = itemResourcePtr[items::Goron_Bomb_Bag].arcName;
        itemResourcePtr[items::Bomb_Bag_Regular_Bombs].modelResIdx = itemResourcePtr[items::Goron_Bomb_Bag].modelResIdx;
        itemResourcePtr[items::Bomb_Bag_Regular_Bombs].brkResIdx = 0xFFFF;
        itemResourcePtr[items::Bomb_Bag_Regular_Bombs].tevFrm = 0x00;
        getSeTypePtr[items::Bomb_Bag_Regular_Bombs] = getSeTypePtr[items::Goron_Bomb_Bag];

        memcpy( &itemResourcePtr[items::Horse_Call],
                &itemResourcePtr[items::Ilias_Charm],
                sizeof( tp::d_item_data::ItemResource ) );
        getSeTypePtr[items::Horse_Call] = getSeTypePtr[items::Clawshot];

        *reinterpret_cast<u32*>( reinterpret_cast<u32>( &itemInfoPtr[items::Bublin_Camp_Key] ) ) = smallKeyItemInfo;
        getSeTypePtr[items::Bublin_Camp_Key] = smallKeySeType;
        memcpy( &itemResourcePtr[items::Bublin_Camp_Key], smallKeyItemResourcePtr, sizeof( tp::d_item_data::ItemResource ) );
    }

    void giveNodeDungeonItems( const tp::d_com_inf_game::AreaNodesID nodeId,
                               const char* stage,
                               const item::NodeDungeonItemType type )
    {
        tp::d_com_inf_game::GameInfo* gameInfoPtr = &gameInfo;

        // If the key is for the current area, then update the local node
        // Only check the first 4 characters, since those are what determine each area
        if ( strncmp( gameInfoPtr->currentStage, stage, 4 ) == 0 )
        {
            switch ( type )
            {
                case item::NodeDungeonItemType::Small_Key:
                    gameInfoPtr->localAreaNodes.nbKeys++;
                    break;
                case item::NodeDungeonItemType::Dungeon_Map:
                    gameInfoPtr->localAreaNodes.dungeon.mapGotten = 0b1;
                    break;
                case item::NodeDungeonItemType::Compass:
                    gameInfoPtr->localAreaNodes.dungeon.compassGotten = 0b1;
                    break;
                case item::NodeDungeonItemType::Big_Key:
                    gameInfoPtr->localAreaNodes.dungeon.bigKeyGotten = 0b1;
                    break;

                default:
                    break;
            }
        }
        else     // Key is not for the current area, so update the appropriate node
        {
            tp::d_com_inf_game::AreaNodes* node =
                reinterpret_cast<tp::d_com_inf_game::AreaNodes*>( &gameInfoPtr->scratchPad.allAreaNodes.Ordon );

            switch ( type )
            {
                case item::NodeDungeonItemType::Small_Key:
                    node[static_cast<u32>( nodeId )].nbKeys++;
                    break;
                case item::NodeDungeonItemType::Dungeon_Map:
                    node[static_cast<u32>( nodeId )].dungeon.mapGotten = 0b1;
                    break;
                case item::NodeDungeonItemType::Compass:
                    node[static_cast<u32>( nodeId )].dungeon.compassGotten = 0b1;
                    break;
                case item::NodeDungeonItemType::Big_Key:
                    node[static_cast<u32>( nodeId )].dungeon.bigKeyGotten = 0b1;
                    break;
                default:
                    break;
            }
        }
    }

    void setCustomItemFunctions()
    {
        tp::d_item::ItemFunc* itemFuncPtr = &tp::d_item::item_func_ptr[0];

        // Forest Temple
        tp::d_item::ItemFunc onGetForestTempleSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Forest_Temple;
            const char* stage = stage::allStages[Stage_Forest_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Forest_Temple_Small_Key] = onGetForestTempleSmallKey;

        tp::d_item::ItemFunc onGetForestTempleDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Forest_Temple;
            const char* stage = stage::allStages[Stage_Forest_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Forest_Temple_Dungeon_Map] = onGetForestTempleDungeonMap;

        tp::d_item::ItemFunc onGetForestTempleCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Forest_Temple;
            const char* stage = stage::allStages[Stage_Forest_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Forest_Temple_Compass] = onGetForestTempleCompass;

        tp::d_item::ItemFunc onGetForestTempleBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Forest_Temple;
            const char* stage = stage::allStages[Stage_Forest_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Forest_Temple_Big_Key] = onGetForestTempleBigKey;

        // Goron Mines
        tp::d_item::ItemFunc onGetGoronMinesSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Goron_Mines;
            const char* stage = stage::allStages[Stage_Goron_Mines];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Goron_Mines_Small_Key] = onGetGoronMinesSmallKey;

        tp::d_item::ItemFunc onGetGoronMinesDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Goron_Mines;
            const char* stage = stage::allStages[Stage_Goron_Mines];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Goron_Mines_Dungeon_Map] = onGetGoronMinesDungeonMap;

        tp::d_item::ItemFunc onGetGoronMinesCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Goron_Mines;
            const char* stage = stage::allStages[Stage_Goron_Mines];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Goron_Mines_Compass] = onGetGoronMinesCompass;

        // Lakebed Temple
        tp::d_item::ItemFunc onGetLakebedTempleSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Lakebed_Temple;
            const char* stage = stage::allStages[Stage_Lakebed_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Lakebed_Temple_Small_Key] = onGetLakebedTempleSmallKey;

        tp::d_item::ItemFunc onGetLakebedTempleDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Lakebed_Temple;
            const char* stage = stage::allStages[Stage_Lakebed_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Lakebed_Temple_Dungeon_Map] = onGetLakebedTempleDungeonMap;

        tp::d_item::ItemFunc onGetLakebedTempleCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Lakebed_Temple;
            const char* stage = stage::allStages[Stage_Lakebed_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Lakebed_Temple_Compass] = onGetLakebedTempleCompass;

        tp::d_item::ItemFunc onGetLakebedTempleBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Lakebed_Temple;
            const char* stage = stage::allStages[Stage_Lakebed_Temple];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Lakebed_Temple_Big_Key] = onGetLakebedTempleBigKey;

        // Arbiters Grounds
        tp::d_item::ItemFunc onGetArbitersGroundsSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Arbiters_Grounds;
            const char* stage = stage::allStages[Stage_Arbiters_Grounds];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Arbiters_Grounds_Small_Key] = onGetArbitersGroundsSmallKey;

        tp::d_item::ItemFunc onGetArbitersGroundsDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Arbiters_Grounds;
            const char* stage = stage::allStages[Stage_Arbiters_Grounds];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Arbiters_Grounds_Dungeon_Map] = onGetArbitersGroundsDungeonMap;

        tp::d_item::ItemFunc onGetArbitersGroundsCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Arbiters_Grounds;
            const char* stage = stage::allStages[Stage_Arbiters_Grounds];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Arbiters_Grounds_Compass] = onGetArbitersGroundsCompass;

        tp::d_item::ItemFunc onGetArbitersGroundsBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Arbiters_Grounds;
            const char* stage = stage::allStages[Stage_Arbiters_Grounds];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Arbiters_Grounds_Big_Key] = onGetArbitersGroundsBigKey;

        // Snowpeak Ruins
        tp::d_item::ItemFunc onGetSnowpeakRuinsSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Snowpeak_Ruins;
            const char* stage = stage::allStages[Stage_Snowpeak_Ruins];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Snowpeak_Ruins_Small_Key] = onGetSnowpeakRuinsSmallKey;

        tp::d_item::ItemFunc onGetSnowpeakRuinsDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Snowpeak_Ruins;
            const char* stage = stage::allStages[Stage_Snowpeak_Ruins];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Snowpeak_Ruins_Dungeon_Map] = onGetSnowpeakRuinsDungeonMap;

        tp::d_item::ItemFunc onGetSnowpeakRuinsCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Snowpeak_Ruins;
            const char* stage = stage::allStages[Stage_Snowpeak_Ruins];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Snowpeak_Ruins_Compass] = onGetSnowpeakRuinsCompass;

        // Temple of Time
        tp::d_item::ItemFunc onGetTempleofTimeSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Temple_of_Time;
            const char* stage = stage::allStages[Stage_Temple_of_Time];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Temple_of_Time_Small_Key] = onGetTempleofTimeSmallKey;

        tp::d_item::ItemFunc onGetTempleofTimeDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Temple_of_Time;
            const char* stage = stage::allStages[Stage_Temple_of_Time];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Temple_of_Time_Dungeon_Map] = onGetTempleofTimeDungeonMap;

        tp::d_item::ItemFunc onGetTempleofTimeCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Temple_of_Time;
            const char* stage = stage::allStages[Stage_Temple_of_Time];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Temple_of_Time_Compass] = onGetTempleofTimeCompass;

        tp::d_item::ItemFunc onGetTempleofTimeBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Temple_of_Time;
            const char* stage = stage::allStages[Stage_Temple_of_Time];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Temple_of_Time_Big_Key] = onGetTempleofTimeBigKey;

        // City in The Sky
        tp::d_item::ItemFunc onGetCityinTheSkySmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::City_in_the_Sky;
            const char* stage = stage::allStages[Stage_City_in_the_Sky];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::City_in_The_Sky_Small_Key] = onGetCityinTheSkySmallKey;

        tp::d_item::ItemFunc onGetCityinTheSkyDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::City_in_the_Sky;
            const char* stage = stage::allStages[Stage_City_in_the_Sky];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::City_in_The_Sky_Dungeon_Map] = onGetCityinTheSkyDungeonMap;

        tp::d_item::ItemFunc onGetCityinTheSkyCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::City_in_the_Sky;
            const char* stage = stage::allStages[Stage_City_in_the_Sky];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::City_in_The_Sky_Compass] = onGetCityinTheSkyCompass;

        tp::d_item::ItemFunc onGetCityinTheSkyBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::City_in_the_Sky;
            const char* stage = stage::allStages[Stage_City_in_the_Sky];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::City_in_The_Sky_Big_Key] = onGetCityinTheSkyBigKey;

        // Palace of Twilight
        tp::d_item::ItemFunc onGetPalaceofTwilightSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Palace_of_Twilight;
            const char* stage = stage::allStages[Stage_Palace_of_Twilight];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Palace_of_Twilight_Small_Key] = onGetPalaceofTwilightSmallKey;

        tp::d_item::ItemFunc onGetPalaceofTwilightDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Palace_of_Twilight;
            const char* stage = stage::allStages[Stage_Palace_of_Twilight];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Palace_of_Twilight_Dungeon_Map] = onGetPalaceofTwilightDungeonMap;

        tp::d_item::ItemFunc onGetPalaceofTwilightCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Palace_of_Twilight;
            const char* stage = stage::allStages[Stage_Palace_of_Twilight];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Palace_of_Twilight_Compass] = onGetPalaceofTwilightCompass;

        tp::d_item::ItemFunc onGetPalaceofTwilightBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Palace_of_Twilight;
            const char* stage = stage::allStages[Stage_Palace_of_Twilight];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Palace_of_Twilight_Big_Key] = onGetPalaceofTwilightBigKey;

        // Hyrule Castle
        tp::d_item::ItemFunc onGetHyruleCastleSmallKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Hyrule_Castle;
            const char* stage = stage::allStages[Stage_Hyrule_Castle];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Hyrule_Castle_Small_Key] = onGetHyruleCastleSmallKey;

        tp::d_item::ItemFunc onGetHyruleCastleDungeonMap = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Hyrule_Castle;
            const char* stage = stage::allStages[Stage_Hyrule_Castle];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Dungeon_Map );
        };
        itemFuncPtr[items::Hyrule_Castle_Dungeon_Map] = onGetHyruleCastleDungeonMap;

        tp::d_item::ItemFunc onGetHyruleCastleCompass = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Hyrule_Castle;
            const char* stage = stage::allStages[Stage_Hyrule_Castle];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Compass );
        };
        itemFuncPtr[items::Hyrule_Castle_Compass] = onGetHyruleCastleCompass;

        tp::d_item::ItemFunc onGetHyruleCastleBigKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Hyrule_Castle;
            const char* stage = stage::allStages[Stage_Hyrule_Castle];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Big_Key );
        };
        itemFuncPtr[items::Hyrule_Castle_Big_Key] = onGetHyruleCastleBigKey;

        // Bublin Camp
        tp::d_item::ItemFunc onGetBublinCampKey = []() {
            const tp::d_com_inf_game::AreaNodesID nodeId = tp::d_com_inf_game::AreaNodesID::Gerudo_Desert;
            const char* stage = stage::allStages[Stage_Bublin_Camp];
            giveNodeDungeonItems( nodeId, stage, item::NodeDungeonItemType::Small_Key );
        };
        itemFuncPtr[items::Bublin_Camp_Key] = onGetBublinCampKey;
    }

    u16 dungeonStoryFlags[8] = { 0x602, 0x701, 0x904, 0x2010, 0x2008, 0x2004, 0x2002, 0x5410 };
}     // namespace mod::game_patch