#include "singleton.h"
#include "defines.h"


namespace mod
{	

	Singleton* Singleton::instance = 0;

	Singleton* Singleton::getInstance()
	{
		if (instance == 0)
		{
			instance = new Singleton();
		}
		return instance;
	}

	Singleton::Singleton()
	{
		isMDHSkipEnabled = 1;
		isForestEscapeEnabled = 1;
		isGateUnlockEnabled = 1;
		isGoatSkipEnabled = 1;
		isMSPuzzleSkipEnabled = 1;
		isCartEscortSkipEnabled = 1;
		isEarlyCiTSEnabled = 0;
		isCannonRepaired = 0;
		isEarlyDesertEnabled = 0;
		isBossKeyseyEnabled = 0;
		isSewerSkipEnabled = 1;
		shuffledSkybook = 1;
		isIntroSkipped = 1;
		isTwilightSkipped = 1;
		diababaMusicFixed = 0;
		midnaTimeControl = 1;
		hasActorCommonLayerRan = 0;
		isEarlyToTEnabled = 0;
		isEarlyPoTEnabled = 0;
		isGMStoryPatch = 0;
		isEarlyHCEnabled = 0;
		startWithCrystal = 0;
		isRainbowLanternEnabled = 0;
		isCustomMusicEnabled = 0;

		//dungeon fixes
		hasFTBeenBeaten = 0;
		hasGMBeenBeaten = 0;
		hasLBTBeenBeaten = 0;
		hasAGBeenBeaten = 0;
		hasSPRBeenBeaten = 0;
		hasToTBeenBeaten = 0;
		hasCiTSBeenBeaten = 0;
		hasCiTSOoccoo = 0;
	}

} 