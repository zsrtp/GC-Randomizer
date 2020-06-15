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
	}

} 