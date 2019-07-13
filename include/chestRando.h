#pragma once

#include "defines.h"

namespace mod
{
	class ChestRandomizer
	{
		private:
			// Conditions used by the generator to determine wether a check is already reachable
			u16 currentPlayerConditions;

			// Assumed fill
			u16 startConditions = 0xFFFF;

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
			u8 getItemReplacement(const float pos[3], u8 item);
	};
}