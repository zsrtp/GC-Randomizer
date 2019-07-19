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
			u16 startConditions = 0b1110100000000001; // AND, Lantern, IB, slingshot, small key (<-locked anyway) (although IB are not in layer 0 we add them since they will be statically in Bo chest for layer1)

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
			bool checkCondition(item::ItemCheck* sourceCheck);
	};
}