#pragma once

#include "defines.h"
#include "item.h"

namespace mod
{
	class ChestRandomizer
	{
		public:
			// Conditions used by the generator to determine wether a check is already reachable
			u16 currentPlayerConditions;

			// Layer 0 conditions, basically
			u16 startConditions = 0b0110100000000000; // Lantern, IB, slingshot (although IB are not in layer 0 we add them since they will be statically in Bo chest for layer1)

			// Debug values
			char lastSourceInfo[50];
			char lastDestInfo[50];

			u64 currentSeed;
			u32 checkSum;

			u16 totalChecks;
			u16 layerCheckCount;
			u16 conditionCheckCount;

		public:
			/**
			 * Generates a new set of chest replacements based
			 * on current RAND_SEED
			 */
			void generate();

			/**
			 * Finds a random source within a specified layer
			 */
			item::ItemCheck* findSource(u8 destLayer);

			/**
			 * Places dest into source and sets the flags necessary
			 */
			void placeCheck(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck);

			/**
			 * Validates two checks, if true they can be placed
			 */
			bool validate(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck);

			/**
			 * Checks if the player with their current conditions can access the chest
			 */
			bool ChestRandomizer::checkCondition(item::ItemCheck* sourceCheck);

			/**
			 * Returns the item replacement if found, otherwise
			 * the item that's passed via argument
			 * 
			 * @param pos The cXyz of the chest
			 * @param item Internal Item ID of the item
			 */
			u8 getItemReplacement(const float pos[3], s32 item);
	};
}