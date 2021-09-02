#pragma once

#include <tp/d_com_inf_game.h>

#include "defines.h"
#include "item.h"

namespace mod
{
    class ChestRandomizer
    {
       private:
        u8* itemOrder = nullptr;     // Used for checksum
        u8 itemOrderIndex;

       public:
        // Conditions used by the generator to determine wether a check is already reachable
        u32 currentPlayerConditions;

        // Layer 0 conditions, basically
        u32 startConditions = 0b100000000000000100010;     // AND, small key, master sword (<-locked anyway)

        // Debug values
        char lastSourceInfo[50];
        char lastDestInfo[50];

        u64 currentSeed;
        u16 checkSum;

        u16 totalChecks;
        u16 layerCheckCount;
        u16 empty;
        float rangeX = 400.0f;
        float rangeY = 200.0f;
        float rangeZ = 400.0f;

        u8 isProgressiveEnabled = 1;
        u8 isBugsanityEnabled = 1;
        u8 isPoesanityEnabled = 1;
        u8 isShopsanityEnabled = 1;
        u8 areDungeonItemsRandomized = 1;
        u8 isTwilightSkipEnabled = 1;
        u8 isKeysanityEnabled = 1;
        u8 areHeartPiecesRandomized = 1;
        u8 areRupeesRandomized = 1;
        u8 areAmmoRandomized = 1;

        u8 itemThatReplacesHalfMilk = 0;
        u8 itemThatReplacesSlingShot = 0;

        tp::d_com_inf_game::ItemSlots* itemWheel;

       public:
        /**
         * Generates a new set of chest replacements based
         * on current RAND_SEED
         */
        void generate();

        /**
         * Returns the item replacement if found, otherwise
         * the item that's passed via argument
         *
         * @param pos The cXyz of the chest
         * @param item Internal Item ID of the item
         */
        s32 getItemReplacement( const float pos[3], s32 item );

        /**
         * Checks if the stage given is a boss room
         * to know if we can spawn a HC
         *
         * excludes hyrule castle since boss doesn't spawn heart container
         */
        bool isStageBoss();

        /**
         * checks if the stage is one of the 5 grotto stages
         */

        bool isStageDungeon();

        bool isStageGrotto();

        bool isStageInterior();

        bool isStageCave();

        bool isStageSpecial();

        bool isStageTOD();

       private:
        /**
         * Finds a random source within a specified layer range
         */
        item::ItemCheck* findSource( u8 maxLayer, u8 minLayer = 0, item::ItemCheck* destCheck = nullptr );

        /**
         * Places dest into source and sets the flags necessary
         */
        void placeCheck( item::ItemCheck* sourceCheck, item::ItemCheck* destCheck );

        /**
         * Checks if the player with their current conditions can access the chest
         */
        bool checkCondition( item::ItemCheck* sourceCheck, item::ItemCheck* destCheck );

        /**
         * Checks if the item should be locked in place
         */
        bool isLocked( item::ItemCheck* check );

        /**
         * Checks if the stage given is a dungeon
         * to prevent heart containers to spawn there so boss heart containers can spawn (doesn't fix it entirely based on
         *Zephiles)
         *
         *includes miniboss rooms but not boss rooms
         *excludes hyrule castle since boss doesn't spawn heart container
         */
        bool isStageADungeon( char* stage );

        /**
         * checks if item given is a type of bombs
         * to prevent problems with the game changing bomb type based on type of bombs in inventory
         *
         * excludes bomb bag+bombs
         */
        bool isItemBombs( u8 itemID );

        /**
         * checks if item given is a bottle content without a bottle
         */
        bool isItemBottleFill( u8 itemID );

        /**
         *
         * places the key checks correctly
         */
        void handleKeysanity();

        /**
         * check if the check gotten in a grotto is the right one
         */
        bool isGrottoCheckOk( u16 checkID );
    };
}     // namespace mod