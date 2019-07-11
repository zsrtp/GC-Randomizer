#include "mod.h"
#include "defines.h"
#include "systemConsole.h"
#include "patch.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "controller.h"
#include "tools.h"
#include "array.h"
#include "eventListener.h"
#include "stage.h"

#include <tp/f_op_scene_req.h>
#include <tp/d_com_inf_game.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>


namespace mod
{
	Mod* gMod = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	void giveEpona()
	{
		strcpy(sysConsolePtr->consoleLine[20].line, "Event: giveEpona");
		// Unset Epona stolen flag
		gameInfo.scratchPad.wQuestLogData[0x7F5] &= ~0x80;

		// Set Epona tamed flag
		gameInfo.scratchPad.wQuestLogData[0x7F6] |= 0x2;
	}

	Mod::Mod()
	{
		gMod = this;
	}

	void Mod::init()
	{
		// Perform any necessary assembly overwrites
		assemblyOverwrites();
		
		// Set the initial console color
		system_console::setBackgroundColor(0x00A0A0A0);
		system_console::setState(true, 21);

		// Set version
		strcpy(version, "v0.3b");

		sprintf(sysConsolePtr->consoleLine[0].line, "AECX' TP Randomizer %s", version);
		strcpy(sysConsolePtr->consoleLine[1].line, "Controls:");
		strcpy(sysConsolePtr->consoleLine[2].line, "  [R + Z] Toggle console");
		strcpy(sysConsolePtr->consoleLine[3].line, "  [L + Z + B] New seed (time dependent)");
		strcpy(sysConsolePtr->consoleLine[4].line, "  You have to set one initially!");

		// Give the player epona when he's at sewers
		eventListener->addLoadEvent(stage::allStages[Stage_Hyrule_Castle_Sewers], 0, 0, 0, giveEpona, event::LoadEventAccuracy::Stage);


		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
			[]()
			{
				gMod->run();
			}
		);

		checkTreasureRupeeReturn_trampoline = patch::hookFunction(tp::d_a_alink::checkTreasureRupeeReturn,
			[](void* unused, s32 amount)
			{
				// Disable this function as this check happens before the replacement
				// Might render an item unobtainable if you're having rupees
				return false;
			}
		);

		createItemForTrBox_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				gMod->procCreateItemForTrBoxDemo(pos, item, unk3, unk4, unk5, unk6);
			}
		);
	}
	
	void Mod::run()
	{
		// Increment rando seed
		tools::randomSeed += 0xf83b877a;

		// If loading has started check for LoadEvents
		if(isLoading && !loadTriggered)
		{
			eventListener->checkLoadEvents();
			loadTriggered = true;
		}
		if(!isLoading)
		{
			loadTriggered = false;
		}


		// Runs once each frame
		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_R | controller::PadInputs::Button_Z)))
		{
			// Toggle console
			system_console::setState(!sysConsolePtr->consoleEnabled, 0);
		}

		if(controller::checkForButtonInputSingleFrame((controller::PadInputs::Button_L | controller::PadInputs::Button_Z | controller::PadInputs::Button_B)))
		{
			// Generate seed
			// TODO put this into extra function at some point?

			tp::jfw_system::SystemConsole* Console = sysConsolePtr;
			// Print initial seed data for recreation
			// Use lines 10 to 20!
			sprintf(Console->consoleLine[10].line, "== GENERATOR - %s ==", version);
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
				}/*
				else if(sItem->itemID == items::Iron_Boots)
				{
					// Commented because only faked IBs are in the list currently
					// Original won't be touched
					// Will be needed once we put the original IB chest back in
					// Unless we find a workaround to change the contents of that chest
					// Bo will start talking infinitely if != IB
					dItem = sItem;
				}*/
				else if (sItem->itemID == items::Ordon_Goat_Cheese)
				{
					dItem = sItem;
				}
				else if (dItem->itemID == items::Ordon_Pumpkin)
				{
					dItem = sItem;
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

		// Call original function
		fapGm_Execute_trampoline();
	}

	void Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Runs once when Link picks up an item with text and is holding it towards the camera
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
				}
			}
		}
		strcpy(sysConsolePtr->consoleLine[6].line, "Condition:");
		sprintf(sysConsolePtr->consoleLine[7].line, BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(cItem.condition >> 7), BYTE_TO_BINARY(cItem.condition));
		if(!replaceFound)
		{
			strcpy(sysConsolePtr->consoleLine[5].line, "No item replacement found.");
		}

		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}
	
	void Mod::assemblyOverwrites()
	{
		// Get the addresses to overwrite
		#ifdef TP_US
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#elif defined TP_EU
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B878);
		#elif defined TP_JP
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#endif

		// Perform the overwrites

		/* If the address is loaded into the cache before the overwrite is made, 
		then the cache will need to be cleared after the overwrite */

		// Enable the crash screen
		*enableCrashScreen = 0x48000014; // b 0x14
	}
}