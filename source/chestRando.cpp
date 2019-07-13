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
		// Randomize the chest targets
		tp::jfw_system::SystemConsole* Console = sysConsolePtr;
		// Print initial seed data for recreation
		// Use lines 10 to 20!
		sprintf(Console->consoleLine[10].line, "== GENERATOR - %s ==", VERSION);
		sprintf(Console->consoleLine[11].line, "Seed: %016llX", RAND_SEED);

		size_t size = sizeof(item::ItemCheck);
		u16 numItems = sizeof(item::checks) / size;

		u16 numPlaced = 0;

		u32 totalLoops = 0;
		u32 threshhold = 0x000FFFFF;

		u16 zeroes = 0;

		u16 sum = 0;

		// Set current checksum
		sum = tools::fletcher16(reinterpret_cast<u8*>(&item::checks), sizeof(item::checks));

		// Reset conditions
		currentPlayerConditions = startConditions;

		// Reset items
		for(u16 i = 0; i < numItems; i++)
		{
			item::checks[i].source = nullptr;
			item::checks[i].destination = nullptr;
		}

		sprintf(Console->consoleLine[12].line, "[%d] Items reset", numItems);
		sprintf(Console->consoleLine[13].line, "Checksum old: [%x]", sum);

		// Represents the current sourceItem who's alias has to be found and set
		item::ItemCheck* sItem;

		// Represents the current destinationItem who's conditions have to be checked
		item::ItemCheck* dItem;

		while(numPlaced < numItems)
		{
			totalLoops++;
			/**
			 * Assumed fill:
			 * We fill in the chests "backwards" that means
			 * we assume we have everything and upon
			 * placing the item we remove that item out of
			 * the player's conditions
			 */

			bool sourceReachable = true;
			bool destinationValid = true;

			// Find source
			do
			{
				totalLoops++;
				u16 sourceIndex = tools::getRandom(numItems);

				sItem = &item::checks[sourceIndex];

				// Check whether the source already has a destination (has been placed)
				while(sItem->destination)
				{
					sourceIndex++;
					// Index is 0 based, numItems absolute
					if(sourceIndex == numItems)
					{
						sourceIndex = 0;
						zeroes++;
					}
					sItem = &item::checks[sourceIndex];
					if(totalLoops > threshhold) goto ESCAPE;
				}

				// Store the player conditions with the indicating bit removed
				u16 pCondition = (sItem->condition & ~item::Condition::AND);
				// Make the necessary condition check
				if(numPlaced > 100)
				{
					// Ignore conditions lol
					sourceReachable = true;
				}
				else if(item::Condition::AND == (sItem->condition & item::Condition::AND))
				{
					// All items in pCondition have to in currentPlayerConditions
					sourceReachable = (pCondition & currentPlayerConditions) == pCondition;
				}
				else
				{
					// One of the items in pCondition has to be in currentPlayerConditions
					sourceReachable = (pCondition & currentPlayerConditions) != 0;
				}

				if(totalLoops > threshhold) goto ESCAPE;
			} while(!sourceReachable);
			// The chest we're replacing is now reachable

			do
			{
				totalLoops++;
				// Find destination that has no source yet

				do
				{
					u16 destinationIndex = tools::getRandom(numItems);
					dItem = &item::checks[destinationIndex];

					if(totalLoops > threshhold) goto ESCAPE;
				} while(dItem->source);


				// Destination is not placed yet
				// This is technically redundant but for code consistency I'll keep the while(!destinationValid)
				destinationValid = true;

				if (totalLoops > threshhold) goto ESCAPE;
			} while (!destinationValid);
			// The chest (content) we're receiving when opening sItem

			// Fix special Items
			if (sItem->type == item::ItemType::Key)
			{
				dItem = sItem;
			}
			else if (sItem->itemID == items::Ordon_Goat_Cheese)
			{
				dItem = sItem;
			}
			else if (sItem->itemID == items::Ordon_Pumpkin)
			{
				dItem = sItem;
			}

			else if (dItem->type == item::ItemType::Key)
			{
				sItem = dItem;
			}
			else if (dItem->itemID == items::Ordon_Goat_Cheese)
			{
				sItem = dItem;
			}
			else if (dItem->itemID == items::Ordon_Pumpkin)
			{
				sItem = dItem;
			}

			// Place item
			sItem->destination 	= dItem;
			dItem->source 		= sItem;

			// Conditionhandling
			currentPlayerConditions &= ~item::getFlags(dItem->itemID, currentPlayerConditions);

			numPlaced++;
		} // numPlaced < numItems

		ESCAPE:
		if(totalLoops > threshhold) sprintf(Console->consoleLine[17].line, "Infinite loop protection.");
		else Console->consoleLine[17].line[0] = '\0';

		sum = tools::fletcher16(reinterpret_cast<u8*>(&item::checks), sizeof(item::checks));
		sprintf(Console->consoleLine[14].line, "Checksum new: [%x]", sum);
		sprintf(Console->consoleLine[15].line, "Zeroes: %d", zeroes);
		sprintf(Console->consoleLine[16].line, "Done with %d/%d items", numPlaced, numItems);
	}

	u8 ChestRandomizer::getItemReplacement(const float pos[3], u8 item)
	{
		// identify soley by position

		size_t size = sizeof(item::ItemCheck);
		u16 numItems = sizeof(item::checks) / size;

		item::ItemCheck cItem;
		bool replaceFound = false;

		for(u16 i = 0; i < numItems; i++)
		{
			cItem = item::checks[i];
			// Check pos against the item struct, floor the values due to fluctuations
			// Don't check for coords if unique
			if(item == cItem.itemID && (cItem.type == item::ItemType::Equip || cItem.type == item::ItemType::Gear))
			{
				// TODO move the sprintfs() to the end of the function and set the text dynamically+
				replaceFound = true;
				if(cItem.destination)
				{
					// Item found, load destination
					u8 newItem = cItem.destination->itemID;
					sprintf(sysConsolePtr->consoleLine[5].line, "[%d] item: %d -> %d", i, item, newItem);

					item = newItem;
					break;
				}
				else
				{
					sprintf(sysConsolePtr->consoleLine[5].line, "[%d] nullptr exception", i);
					break;
				}
			}
			else if ((static_cast<s32>(pos[0]) & ~0xF) == (static_cast<s32>(cItem.position[0]) & ~0xF))
			{
				if ((static_cast<s32>(pos[1]) & ~0xF) == (static_cast<s32>(cItem.position[1]) & ~0xF))
				{
					if ((static_cast<s32>(pos[2]) & ~0xF) == (static_cast<s32>(cItem.position[2]) & ~0xF))
					{
						replaceFound = true;
						if(cItem.destination != nullptr)
						{
							// Item found, load destination
							item::ItemCheck* newItem = cItem.destination;

							u16 j;
							for(j = 0; j < numItems; j++)
							{
								if(&item::checks[j] == newItem)
									break;
							}

							sprintf(sysConsolePtr->consoleLine[5].line, "item: {%u}%u -> {%u}%u", i, item, j, newItem->itemID);

							item = newItem->itemID;
							break;
						}
						else
						{
							sprintf(sysConsolePtr->consoleLine[5].line, "[%d] nullptr exception", i);
							break;
						}
					}
				}
			}
		}
		strcpy(sysConsolePtr->consoleLine[6].line, "Condition:");
		sprintf(sysConsolePtr->consoleLine[7].line, BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(cItem.condition >> 7), BYTE_TO_BINARY(cItem.condition));
		if(!replaceFound)
		{
			strcpy(sysConsolePtr->consoleLine[5].line, "No item replacement found.");
		}

		return item;
	}
}