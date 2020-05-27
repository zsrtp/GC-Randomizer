#pragma once

#include "defines.h"

#include <tp/evt_control.h>
#include <tp/d_com_inf_game.h>

namespace mod::tools
{
	extern "C"
	{
		extern u64 randomSeed;

		/**
		 * Generates a simple random number (not perfectly random but good enough for most purposes)
		 * Note: It's best to use with a clock that advances randomSeed
		 * (C) http://xoshiro.di.unimi.it/splitmix64.c
		 * 
		 * @param max The maximum number to return
		 * @returns A random integer between 0 and max, excluding max
		 */
		u32 getRandom(u32 max);

		/**
		 * Triggers the generator function which is a member function
		 * (This is a wrapper)
		 */
		void triggerRandomGenerator();

		/**
		 * Triggers a load savely by setting some important
		 * variables for the game to handle it properly
		 */
		void triggerSaveLoad(char* stage, u8 room, u8 spawn, u8 state = 0xFF, u8 event = 0xFF);

		/**
		 * Sets whether the cutscene can be skipped
		 * 
		 * Unskippable cutscenes sometimes set flags so take care of them
		 * in the function that calls this if needed
		 * 
		 * @param skippable If true the user can skip, otherwise unskippable
		 * @param fade If true the skip action will smoothly transition
		 * @param onSkip The tp::evt_control::csFunc that runs when trying to skip
		 */
		void setCutscene(bool skippable, bool fade = true, tp::evt_control::csFunc onSkip = tp::evt_control::defaultSkipStb);

		/**
		 * Compares two floats and returns their difference
		 */
		float fCompare(const float f1, const float f2);

		/**
		 * Generates a simple u32 checksum
		 * 
		 * @param data The data you need a checksum for
		 * @param count The number of bytes
		 */
		u16 fletcher16(u8* data, s32 count);

		/**
		 * Sets a specific bit/flag in the itemFlags variable in tp::d_com_inf_game::gameInfo.scratchPad.itemFlags
		 */
		void setItemFlag(ItemFlags flag);

		/**
		 * Unsets a specific bit/flag in the itemFlags variable in tp::d_com_inf_game::gameInfo.scratchPad.itemFlags
		 */
		void clearItemFlag(ItemFlags flag);


		/**
		 * Checks if a specific bit/flag in the itemFlags variable in tp::d_com_inf_game::gameInfo.scratchPad.itemFlags is set
		 */
		bool checkItemFlag(ItemFlags flag);
	}
}