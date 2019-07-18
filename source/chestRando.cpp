#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"

#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	void ChestRandomizer::generate()
	{
		currentPlayerConditions = startConditions;
		currentSeed = tools::randomSeed;

		totalChecks = sizeof(item::checks)/sizeof(item::ItemCheck);

		layerCheckCount = 0;
		conditionCheckCount = 0;

		// Set metadata
		for(u16 i = 0; i < totalChecks; i++)
		{
			// Check if this item has to be in a specific layer
			if(item::checks[i].destLayer != 0xFF)
			{
				layerCheckCount++;
			}
			else if(item::checks[i].condition)
			{
				// Else if because layerChecks will be already placed when we process conditionChecks
				// This source chest requires additional items to be reached
				conditionCheckCount++;
			}
		}

		// Set up arrays
		u16* layerCheckIndex = new u16[layerCheckCount];
		u16* conditionCheckIndex = new u16[conditionCheckCount];

		item::ItemCheck* sourceCheck;
		item::ItemCheck* destCheck;

		u16 l = 0; // layer index
		u16 c = 0; // condition index

		// Extract indexes
		for(u16 i = 0; i < totalChecks; i++)
		{
			// Check if this item has to be in a specific layer
			if(item::checks[i].destLayer != 0xFF)
			{
				layerCheckIndex[l] = i;
				l++;
			}
			else if(item::checks[i].condition)
			{
				conditionCheckIndex[c] = i;
				c++;
			}
		}

		// Place layer items
		for(u16 i = layerCheckCount; i > 0; i--)
		{
			// Get the next layer check
			destCheck = &item::checks[layerCheckIndex[i]];

			do
			{
				sourceCheck = findSource(destCheck->destLayer);
			} while(!validate(sourceCheck, destCheck));

			placeCheck(sourceCheck, destCheck);
		}

		// Now Fill up sources with conditions before they're all filled up
		for(u16 i = conditionCheckCount; i > 0; i--)
		{
			// Get next condition check
			destCheck = &item::checks[conditionCheckIndex[i]];

			do
			{
				sourceCheck = findSource(destCheck->destLayer);
			} while(!validate(sourceCheck, destCheck));
		}

		// Fill the rest
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];
			sourceCheck = findSource(destCheck->destLayer);

			if(validate(sourceCheck, destCheck))
			{
				placeCheck(sourceCheck, destCheck);
			}
		}

		// Done
		checkSum = tools::fletcher32(reinterpret_cast<u16*>(item::checks), sizeof(item::checks));
	}

	void ChestRandomizer::placeCheck(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		// Place without asking
		sourceCheck->destination = destCheck;
		destCheck->source = sourceCheck;

		// Update player conditions!
		currentPlayerConditions |= item::getFlags(destCheck->itemID, currentPlayerConditions);
	}

	item::ItemCheck* ChestRandomizer::findSource(u8 destLayer)
	{
		item::ItemCheck* itemCheck;
		do
		{
			u16 index = tools::getRandom(totalChecks);

			itemCheck = &item::checks[index];

		} while(itemCheck->destination || itemCheck->sourceLayer > destLayer);
		// If itemCheck already has a destination OR the layer is outside the replacement's target

		return itemCheck;
	}

	bool ChestRandomizer::validate(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		if(sourceCheck->destination || destCheck->source)
		{
			return false;
		}
		else
		{			
			return checkCondition(sourceCheck);
		}
	}

	bool ChestRandomizer::checkCondition(item::ItemCheck* sourceCheck)
	{
		if((sourceCheck->condition & currentPlayerConditions) == sourceCheck->condition)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	u8 ChestRandomizer::getItemReplacement(const float pos[3], s32 item)
	{
		// TODO: Find item replacement
		return item;
	}
}