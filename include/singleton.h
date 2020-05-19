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
		u8 isKB1Skipped;
		u8 shuffledSkybook;

		private:

		static Singleton* instance;

		Singleton();

	};
} 