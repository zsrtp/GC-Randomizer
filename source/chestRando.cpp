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
		// Reset
		currentPlayerConditions = startConditions;
		currentSeed = tools::randomSeed;

		totalChecks = sizeof(item::checks)/sizeof(item::ItemCheck);
		empty = 0;
		layerCheckCount = 0;

		itemOrderIndex = 0;

		// Set up arrays
		itemOrder = new u8[totalChecks];

		item::ItemCheck* sourceCheck;
		item::ItemCheck* destCheck;

		// Reset randomization
		for(u16 i = 0; i < totalChecks; i++)
		{
			item::checks[i].destination = nullptr;
			item::checks[i].source = nullptr;
		}

		// Lock some checks
		for(u16 i = 0; i < totalChecks; i++)
		{
			if(isLocked(&item::checks[i]))
			{
				placeCheck(&item::checks[i], &item::checks[i]);
			}
		}

		// Place layer checks
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				// Free slot
				if(destCheck->destLayer != 0xFF)
				{
					// Layer check
					sourceCheck = findSource(destCheck->destLayer, (destCheck->destLayer - 1), destCheck);
					placeCheck(sourceCheck, destCheck);
					layerCheckCount++;
				}
			}
		}

		// Place items that unlock other locations before caring about remaining items
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				// Free slot
				if(item::getFlags(destCheck->itemID, 0) != 0)
				{
					// This would unlock new checks, so place it
					sourceCheck = findSource(0xFF, 0, destCheck);
					placeCheck(sourceCheck, destCheck);
				}
			}
		}

		// Place remaining
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
			{
				sourceCheck = findSource(0xFF, 0, destCheck);
				placeCheck(sourceCheck, destCheck);
			}
		}

		// Count empty
		for(u16 i = 0; i < totalChecks; i++)
		{
			if(!item::checks[i].destination)
			{
				empty++;
			}
		}

		// Done
		checkSum = tools::fletcher16(itemOrder, sizeof(itemOrder));

		delete[] itemOrder;

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
		currentPlayerConditions = item::getFlags(destCheck->itemID, currentPlayerConditions);
	}

	item::ItemCheck* ChestRandomizer::findSource(u8 maxLayer, u8 minLayer, item::ItemCheck* destCheck)
	{
		if(minLayer == 0xFF)
		{
			minLayer = 0x0;
		}

		item::ItemCheck* sourceCheck;
		do
		{
			u16 index = tools::getRandom(totalChecks);
			sourceCheck = &item::checks[index];
		} while(!checkCondition(sourceCheck, destCheck) || sourceCheck->destination || sourceCheck->sourceLayer > maxLayer || sourceCheck->sourceLayer < minLayer);

		return sourceCheck;
	}

	bool ChestRandomizer::checkCondition(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
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

		// If the destination item (which you'll receive) isn't required for this souce it can be placed though
		if((item::getFlags(destCheck->itemID, 0) & sourceCheck->condition) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ChestRandomizer::isLocked(item::ItemCheck* check)
	{
		bool result = false;

		switch(check->type)
		{
			case item::ItemType::Key:
				// Small + Big Keys
				result = true;
			break;

			case item::ItemType::Dungeon:
				// Map, compass
				result = true;
			break;

			case item::ItemType::Story:
				// Ilia quest
				result = true;
			break;
		}

		switch(check->itemID)
		{
			case items::Item::Iron_Boots:
				result = true;
			break;

			case items::Item::Fishing_Rod:
				result = true;
			break;
		}

		return result;
	}

	s32 ChestRandomizer::getItemReplacement(const float pos[3], s32 item)
	{
		item::ItemCheck* sourceCheck;
		snprintf(lastSourceInfo, 50, "%s %4.0f %4.0f %4.0f", gameInfo.currentStage, pos[0], pos[1], pos[2]);
		snprintf(lastDestInfo, 50, "No Replacement found for this source");

		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if(0 == strcmp(gameInfo.currentStage, sourceCheck->stage))
			{
				// Correct stage
				if(tools::fCompare(sourceCheck->position[0], pos[0]) < 400.0f)
				{
					if(tools::fCompare(sourceCheck->position[1], pos[1]) < 200.0f)
					{
						if(tools::fCompare(sourceCheck->position[2], pos[2]) < 400.0f)
						{
							if(sourceCheck->itemID == item)
							{
								snprintf(lastSourceInfo, 50, "%s->%d->%x", sourceCheck->stage, sourceCheck->room, sourceCheck->itemID);
								if(sourceCheck->destination)
								{
									snprintf(lastDestInfo, 50, "%s->%d->%x", sourceCheck->destination->stage, sourceCheck->destination->room, sourceCheck->destination->itemID);
									item = sourceCheck->destination->itemID;
									// Unset this check
									sourceCheck->destination = nullptr;
									return item;
								}
								else
								{
									snprintf(lastDestInfo, 50, "No replacement here.");
									return item;
								}
							}
						}
					}
				}
			}
		}
		return item;
	}
}