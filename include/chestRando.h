#pragma once

#include "defines.h"
#include "item.h"

namespace mod
{
	class ChestRandomizer
	{
		private:
			u8* itemOrder = nullptr; // Used for checksum
			u8 itemOrderIndex;

		public:
			// Conditions used by the generator to determine wether a check is already reachable
			u16 currentPlayerConditions;

			// Layer 0 conditions, basically
			u16 startConditions = 0b1010000000000001; // AND, IB, slingshot, small key (<-locked anyway)

			// Debug values
			char lastSourceInfo[50];
			char lastDestInfo[50];

			u64 currentSeed;
			u16 checkSum;

			u16 totalChecks;
			u16 layerCheckCount;
			u16 empty;

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
			s32 getItemReplacement(const float pos[3], s32 item);

		private:
			/**
			 * Finds a random source within a specified layer range
			 */
			item::ItemCheck* findSource(u8 maxLayer, u8 minLayer = 0, item::ItemCheck* destCheck = nullptr);

			/**
			 * Places dest into source and sets the flags necessary
			 */
			void placeCheck(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck);

			/**
			 * Checks if the player with their current conditions can access the chest
			 */
			bool checkCondition(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck);

			/**
			 * Checks if the item should be locked in place
			 */
			bool isLocked(item::ItemCheck* check);
	};
}