#pragma once

#include "defines.h"

/**
 * Contains variables that can be edited and read by everyone
 */
namespace mod
{
    class Singleton
    {
       public:
        static Singleton* getInstance();

        u8 isMDHSkipEnabled;
        u8 isForestEscapeEnabled;
        u8 isGateUnlockEnabled;
        u8 isGoatSkipEnabled;
        u8 isMSPuzzleSkipEnabled;
        u8 isCartEscortSkipEnabled;
        u8 isEarlyCiTSEnabled;
        u8 isCannonRepaired;
        u8 isEarlyDesertEnabled;
        u8 isBossKeyseyEnabled;
        u8 isSewerSkipEnabled;
        u8 shuffledSkybook;
        u8 isIntroSkipped;
        u8 isTwilightSkipped;
        u8 diababaMusicFixed;
        u8 midnaTimeControl;
        u8 hasActorCommonLayerRan;
        u8 isEarlyToTEnabled;
        u8 isEarlyPoTEnabled;
        u8 isGMStoryPatch;
        u8 isEarlyHCEnabled;
        u8 startWithCrystal;

        // dungeon flags
        u8 hasFTBeenBeaten;
        u8 hasGMBeenBeaten;
        u8 hasLBTBeenBeaten;
        u8 hasAGBeenBeaten;
        u8 hasSPRBeenBeaten;
        u8 hasToTBeenBeaten;
        u8 hasCiTSBeenBeaten;
        u8 hasCiTSOoccoo;

       private:
        static Singleton* instance;

        Singleton();
    };
}     // namespace mod