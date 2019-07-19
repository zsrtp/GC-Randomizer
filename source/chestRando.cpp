#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"

#include <tp/d_com_inf_game.h>
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

		// Reset metadata
		layerCheckCount = 0;
		empty = 0;

		itemOrderIndex = 0;

		for(u16 i = 0; i < totalChecks; i++)
		{
			// We also wanna reset the last rando
			item::checks[i].destination = nullptr;
			item::checks[i].source = nullptr;

			// Check if this item has to be in a specific layer
			if(item::checks[i].destLayer != 0xFF)
			{
				layerCheckCount++;
			}
		}

		// Set up arrays
		u16* layerCheckIndex = new u16[layerCheckCount];
		itemOrder = new u8[totalChecks];

		item::ItemCheck* sourceCheck;
		item::ItemCheck* destCheck;

		u16 l = 0; // layer index

		// Extract indexes
		for(u16 i = 0; i < totalChecks; i++)
		{
			// Check if this item has to be in a specific layer
			if(item::checks[i].destLayer != 0xFF)
			{
				layerCheckIndex[l] = i;
				l++;
			}
		}

		// Lock special items in place first
		for(u16 i = 0; i < totalChecks; i++)
		{
			// Interpret this as source
			sourceCheck = &item::checks[i];

			// Just in case, should always be true here
			if(!sourceCheck->destination)
			{
				if(sourceCheck->type == item::ItemType::Key || sourceCheck->type == item::ItemType::Dungeon || sourceCheck->itemID == items::Item::Iron_Boots)
				{
					// It should be locked before placing any other items
					placeCheck(sourceCheck, sourceCheck);
				}
			}
		}

		// Place layer items
		for(u16 i = 0; i < layerCheckCount; i++)
		{
			// Get the next layer check
			destCheck = &item::checks[layerCheckIndex[i]];

			// Just in case a fixed item was marked as layer item aswell
			if(!destCheck->source)
			{
				do
				{
					sourceCheck = findSource(destCheck->destLayer);
				} while(!validate(sourceCheck, destCheck));

				placeCheck(sourceCheck, destCheck);
			}
		}
		
		// All progression items have been placed, ignore conditions from now on
		currentPlayerConditions = 0xFFFF;

		// Select empty destinations
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				// Find new source
				sourceCheck = findSource(0xFF);

				placeCheck(sourceCheck, destCheck);
			}
		}

		// Count empty sources
		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];
			if(!sourceCheck->destination)
			{
				empty++;
			}
		}

		// Done
		checkSum = tools::fletcher16(itemOrder, sizeof(itemOrder));

		delete[] itemOrder;
		itemOrder = nullptr;
		delete[] layerCheckIndex;

		// Reset seed if the player wanted to lock it (otherwise it advances anyways)
		tools::randomSeed = currentSeed;
	}

	void ChestRandomizer::placeCheck(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		// Add source item to the itemOrder array
		itemOrder[itemOrderIndex] = sourceCheck->itemID;
		itemOrderIndex++;

		// Place without asking
		sourceCheck->destination = destCheck;
		destCheck->source = sourceCheck;

		// Update player conditions!
		currentPlayerConditions |= item::getFlags(destCheck->itemID, currentPlayerConditions);
	}

	item::ItemCheck* ChestRandomizer::findSource(u8 destLayer)
	{
		item::ItemCheck* sourceCheck;
		do
		{
			u16 index = tools::getRandom(totalChecks);
			sourceCheck = &item::checks[index];
		} while(sourceCheck->destination || sourceCheck->sourceLayer > destLayer);
		// If itemCheck already has a destination OR the layer is outside the replacement's target

		return sourceCheck;
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
		if((sourceCheck->condition & item::Condition::AND) == item::Condition::AND)
		{
			if((sourceCheck->condition & currentPlayerConditions) == sourceCheck->condition)
			{
				return true;
			}
		}
		else
		{
			if((sourceCheck->condition & currentPlayerConditions) != 0)
			{
				return true;
			}
		}
		return false;
	}

	s32 ChestRandomizer::getItemReplacement(const float pos[3], s32 item)
	{
		item::ItemCheck* sourceCheck;
		snprintf(lastSourceInfo, 50, "%s %4.0f %4.0f %4.0f", gameInfo.currentStage, pos[0], pos[1], pos[2]);
		lastDestInfo[0] = '\0';

		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if(static_cast<u32>(sourceCheck->position[0]) == static_cast<u32>(pos[0]))
			{
				if(static_cast<u32>(sourceCheck->position[1]) == static_cast<u32>(pos[1]))
				{
					if(static_cast<u32>(sourceCheck->position[2]) == static_cast<u32>(pos[2]))
					{
						if(sourceCheck->itemID == item)
						{
							snprintf(lastSourceInfo, 50, "%s->%d->0x%x", sourceCheck->stage, sourceCheck->room, sourceCheck->itemID);
							if(sourceCheck->destination)
							{
								snprintf(lastDestInfo, 50, "%s->%d->0x%x", sourceCheck->destination->stage, sourceCheck->destination->room, sourceCheck->destination->itemID);
								return sourceCheck->destination->itemID;
							}
							else
							{
								snprintf(lastDestInfo, 50, "No Replacement set for this source");
							}
						}
					}
				}
			}
		}
		return item;
	}
}