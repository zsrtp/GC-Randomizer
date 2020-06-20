#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"
#include "stage.h"
#include "keyPlacement.h"
#include "singleton.h"
#include "grottoChecks.h"
#include "game_patches.h"

#include <tp/d_com_inf_game.h>
#include <tp/d_item.h>
#include <tp/d_a_alink.h>
#include <tp/d_kankyo.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	void ChestRandomizer::generate()
	{
		// Reset

		itemWheel = &gameInfo.scratchPad.itemWheel;

		currentPlayerConditions = startConditions;
		currentSeed = tools::randomSeed;

		totalChecks = sizeof(item::checks) / sizeof(item::ItemCheck);
		empty = 0;
		layerCheckCount = 0;

		itemOrderIndex = 0;

		// Set up arrays
		itemOrder = new u8[totalChecks];

		item::ItemCheck* sourceCheck;
		item::ItemCheck* destCheck;

		// Reset randomization
		for (u16 i = 0; i < totalChecks; i++)
		{
			item::checks[i].destination = nullptr;
			item::checks[i].source = nullptr;
		}

		// Lock some checks
		for (u16 i = 0; i < totalChecks; i++)
		{
			if (isLocked(&item::checks[i]))
			{
				placeCheck(&item::checks[i], &item::checks[i]);
			}
		}

		handleKeysanity();

		//do needed items in order
		for (u16 i = 0; i < sizeof(item::checkPriorityOrder) / sizeof(u16); i++)
		{
			destCheck = &item::checks[item::checkPriorityOrder[i]];
			if (!destCheck->source)
			{
				if (destCheck->itemID == items::Item::Ordon_Sword)
				{
					sourceCheck = findSource(destCheck->destLayer, 0x1, destCheck);//to prevent woodensword from being overwritten before losing it			
				}
				else if (destCheck->itemID == items::Item::Ordon_Shield || destCheck->itemID == items::Item::Wooden_Shield || destCheck->itemID == items::Item::Hylian_Shield)
				{
					sourceCheck = findSource(destCheck->destLayer, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
				}
				else if (destCheck->itemID == items::Item::Zora_Armor || destCheck->itemID == items::Item::Magic_Armor)
				{
					sourceCheck = findSource(destCheck->destLayer, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
				}
				else if (isProgressiveEnabled == 0 && destCheck->itemID == items::Item::Clawshots)
				{
					sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent Clawshots from being overwritten by Clawshot
				}
				else
				{
					sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
				}
				placeCheck(sourceCheck, destCheck);
				//layerCheckCount++;
			}
		}

		// Place items that unlock other locations before caring about remaining items
		//useless
		/*for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];
			if(!destCheck->source)
			{
				// Free slot
				if(item::getFlags(destCheck->itemID, 0) != 0)
				{
					// This would unlock new checks, so place it
					if (destCheck->itemID == items::Item::Ordon_Shield || destCheck->itemID == items::Item::Wooden_Shield || destCheck->itemID == items::Item::Hylian_Shield)
					{
						sourceCheck = findSource(0xFF, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check
					}
					else
					{
						sourceCheck = findSource(0xFF, 0, destCheck);
					}
					placeCheck(sourceCheck, destCheck);
				}
			}
		}*/

		//do dungeon items
		for (u16 i = 0; i <= totalChecks; i++)
		{
			destCheck = &item::checks[i];
			if (!destCheck->source && destCheck->type == item::ItemType::Dungeon)
			{
				sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
				placeCheck(sourceCheck, destCheck);
				//layerCheckCount++;
			}
		}

		// Place layer checks
		for (u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if (!destCheck->source)
			{
				// Free slot
				if (destCheck->destLayer != 0xFF)
				{
					// Layer check	
					if (destCheck->itemID == items::Item::Ordon_Sword)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x1, destCheck);//to prevent woodensword from being overwritten before losing it			
					}
					else if (destCheck->itemID == items::Item::Ordon_Shield || destCheck->itemID == items::Item::Wooden_Shield || destCheck->itemID == items::Item::Hylian_Shield)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
					}
					else if (destCheck->itemID == items::Item::Zora_Armor || destCheck->itemID == items::Item::Magic_Armor)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
					}
					else if (isProgressiveEnabled == 0)
					{
						if (destCheck->itemID == items::Item::Clawshots)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent Clawshots from being overwritten by Clawshot
						}
						else if (destCheck->itemID == items::Item::Big_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x4, destCheck);//to prevent bow from being overwritten
						}
						else if (destCheck->itemID == items::Item::Giant_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x8, destCheck);//to prevent bow from being overwritten
						}
						else if (destCheck->itemID == items::Item::Giant_Wallet)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent overwriting giant wallet with big wallet
						}
						else if (destCheck->itemID == items::Item::Giant_Bomb_Bag)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x6, destCheck);//to prevent getting a 4th bag and possibly crashing the game
						}
						else
						{
							sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
						}
					}
					else
					{
						sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
					}
					placeCheck(sourceCheck, destCheck);
					layerCheckCount++;
				}
			}
		}

		

		// Place remaining
		for (u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if (!destCheck->source)
			{
				if (destCheck->itemID == items::Item::Ordon_Shield || destCheck->itemID == items::Item::Wooden_Shield || destCheck->itemID == items::Item::Hylian_Shield)
				{
					sourceCheck = findSource(0xFF, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
				}
				else
				{
					sourceCheck = findSource(0xFF, 0, destCheck);
				}
				placeCheck(sourceCheck, destCheck);
			}
		}

		// Count empty
		for (u16 i = 0; i < totalChecks; i++)
		{
			if (!item::checks[i].destination)
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
		if (minLayer == 0xFF)
		{
			minLayer = 0x0;
		}

		item::ItemCheck* sourceCheck;
		u16 index;
		do
		{
			index = tools::getRandom(totalChecks);
			sourceCheck = &item::checks[index];
		} while (!checkCondition(sourceCheck, destCheck) || sourceCheck->destination || sourceCheck->sourceLayer > maxLayer || sourceCheck->sourceLayer < minLayer ||
			(destCheck->type == item::ItemType::Bug && 0 == strcmp("R_SP160", sourceCheck->stage) && sourceCheck->room == 3) || //agitha can't give bugs
			(areDungeonItemsRandomized == 1 && destCheck->type == item::ItemType::Dungeon && 0 != strcmp(destCheck->stage, sourceCheck->stage)) || //dungeon items only in their own dungeon
			(areDungeonItemsRandomized == 1 && destCheck->type == item::ItemType::Dungeon && index == 115)); //dont place dungeon items at clawshot goron mines check

		return sourceCheck;
	}

	bool ChestRandomizer::checkCondition(item::ItemCheck* sourceCheck, item::ItemCheck* destCheck)
	{
		if ((sourceCheck->condition & item::Condition::AND) == item::Condition::AND)
		{
			if ((sourceCheck->condition & currentPlayerConditions) == sourceCheck->condition)
			{
				return true;
			}
		}
		else
		{
			if ((sourceCheck->condition & currentPlayerConditions) != 0)
			{
				return true;
			}
		}
		// If the destination item (which you'll receive) isn't required for this source and you don't require anything in this layer, it can be placed though
		if ((item::getFlags(destCheck->itemID, 0) & sourceCheck->condition) == 0)
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

		switch (check->type)
		{
		case item::ItemType::Key:
			// Small Keys + ordon pumpkin and cheese
			if (isKeysanityEnabled == 0 || (Singleton::getInstance()->isForestEscapeEnabled == 1 && isKeysanityEnabled == 1 && check->itemID == items::Item::Coro_Key))
			{
				result = true;
			}
			break;

		case item::ItemType::Dungeon:
			// Map, compass, big key
			if (areDungeonItemsRandomized == 0)
			{
				result = true;
			}
			break;

		case item::ItemType::Story:
			if (check->itemID != items::Item::Aurus_Memo && check->itemID != items::Item::Asheis_Sketch)
			{
				result = true;
			}
			break;

		case item::ItemType::Bug:
			if (isBugsanityEnabled == 0)
			{
				result = true;
			}
			break;

		case item::ItemType::PoeSoul:
			if (isPoesanityEnabled == 0)
			{
				result = true;
			}
			break;

		case item::ItemType::Shop:
			if (isShopsanityEnabled == 0)
			{
				result = true;
			}
			break;

		case item::ItemType::Skill:
			result = true;
			break;

		case item::ItemType::Scent:
			result = true;
			break;
		}

		switch (check->itemID)
		{
			/*case items::Item::Iron_Boots:
				result = true;
			break;*/
		case items::Item::Shadow_Crystal:
			if (Singleton::getInstance()->isMDHSkipEnabled == 0)
			{
				result = true;
			}
			break;

		case items::Item::Fishing_Rod:
			result = true;
			break;

		case items::Item::Ancient_Sky_Book_empty:
			if (isProgressiveEnabled == 0)
			{
				result = true;
			}
			break;

		case items::Item::Ancient_Sky_Book_partly_filled:
			if (Singleton::getInstance()->shuffledSkybook == 0)
			{
				result = true;
			}
			break;

		case items::Item::Ancient_Sky_Book_completed:
			if (Singleton::getInstance()->shuffledSkybook == 0)
			{
				result = true;
			}
			break;


		}

		return result;
	}

	s32 ChestRandomizer::getItemReplacement(const float pos[3], s32 item)
	{
		item::ItemCheck* sourceCheck;
		snprintf(lastSourceInfo, 50, "%s %4.0f %4.0f %4.0f", gameInfo.currentStage, pos[0], pos[1], pos[2]);
		snprintf(lastDestInfo, 50, "No Replacement found for this source");

		/*if (item == items::Item::Sera_Bottle)
		{			//for testing only
			item = itemThatReplacesHalfMilk;
			if (item == items::Item::Dominion_Rod)
			{
				gameInfo.scratchPad.eventBits[0x25] |= 0x80;
			}
			else if (item == items::Item::Poe_Soul)
			{//increase poe counter
				gameInfo.scratchPad.poeCount++;
			}
			return item;
		}
		else if (item == items::Item::Slingshot)
		{			//for testing only
			item = itemThatReplacesSlingShot;
			if (item == items::Item::Dominion_Rod)
			{
				item = items::Item::Dominion_Rod;
				gameInfo.scratchPad.eventBits[0x25] |= 0x80;
			}
			else if (item == items::Item::Poe_Soul)
			{//increase poe counter
				gameInfo.scratchPad.poeCount++;
			}
			return item;
		}*/


		if (item == items::Item::Heart_Container && isStageBoss())
		{//set flag for HC gotten
			gameInfo.localAreaNodes.dungeon.containerGotten = 0b1;
			if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Fyrus]))
			{
				gameInfo.localAreaNodes.unk_0[0xA] |= 0x40;	//open doors to mini boss to prevent softlocks		
			}
		}
		else if (item == items::Item::Iron_Boots)
		{//set flag for having talked to Bo
			gameInfo.scratchPad.eventBits[0x1C] |= 0x20;
		}
		else if (item == items::Item::Poe_Soul && gameInfo.scratchPad.poeCount >= 1 && !(tp::d_a_alink::checkStageName(stage::allStages[Stage_Castle_Town_Shops]) && tp::d_kankyo::env_light.currentRoom == 5))
		{//decrease poe counter
			gameInfo.scratchPad.poeCount--;
		}
		else if (item == items::Item::Vessel_Of_Light_Faron)
		{
			tp::d_com_inf_game::ScratchPad* scratchPadPtr = &gameInfo.scratchPad;
			u8* eventBitsPtr = &scratchPadPtr->eventBits[0];
			tp::d_com_inf_game::AllAreaNodes* allAreaNodesPtr = &scratchPadPtr->allAreaNodes;
			if (Singleton::getInstance()->isTwilightSkipped == 1)
			{
				//Set Faron Twilight Flags
					scratchPadPtr->clearedTwilights.Faron = 0b1; //Clear Faron Twilight
					tools::setItemFlag(ItemFlags::Vessel_Of_Light_Faron);
					scratchPadPtr->tearCounters.Faron = 16;
					eventBitsPtr[0x5] = 0xFF; //Ensure Epona is Stolen, give Midna Charge
					eventBitsPtr[0x6] |= 0x10; //Faron Twilight Progression flag
					eventBitsPtr[0xC] |= 0x8; //Set Sword and Shield to not be on back
					tools::setItemFlag(ItemFlags::Heros_Clothes);

					//Set Eldin Twilight Flags
					scratchPadPtr->clearedTwilights.Eldin = 0b1; // Clear Eldin Twilight
					tools::setItemFlag(ItemFlags::Vessel_Of_Light_Eldin);
					scratchPadPtr->tearCounters.Eldin = 16;
					eventBitsPtr[0x6] |= 0x1; //tame Epona
					eventBitsPtr[0xA] |= 0x8; //Beat KB1
					eventBitsPtr[0x14] |= 0x10; //Put Bo Outside
					eventBitsPtr[0x7] = 0xDE; //skip Gor Coron Sumo and Enter Mines also Trigger KB1 and mark Post-KB1 CS as watched, Eldin Twilight Story Progression Flag
					eventBitsPtr[0x41] |= 0x10; //Told Fado about the Kids

					//Set Lanayru Twilight Flags
					scratchPadPtr->clearedTwilights.Lanayru = 0b1; // Clear Lanayru Twilight
					tools::setItemFlag(ItemFlags::Vessel_Of_Light_Lanayru);
					scratchPadPtr->tearCounters.Lanayru = 16;
					allAreaNodesPtr->Hyrule_Field.unk_0[0xB] |= 0x80;//water on Field map
					allAreaNodesPtr->Hyrule_Field.unk_0[0xF] |= 0x10;//open south CT Shortcut to Faron
					allAreaNodesPtr->Lanayru.unk_0[0xF] |= 0x1;//water on Map
					eventBitsPtr[0x30] |= 0x40; //gave springwater to south CT goron
					eventBitsPtr[0x8] |= 0x80; //ZD Thawed
					eventBitsPtr[0xC] |= 0x2; //Lanayru Twilight Story Progression Flag
					eventBitsPtr[0xA] |= 0x10; //Kagorok Howl at Lake

				//Unlock Map Regions
				scratchPadPtr->movingActors.exploredRegions.Snowpeak = 0b1;
				scratchPadPtr->movingActors.exploredRegions.Desert = 0b1;
				scratchPadPtr->movingActors.exploredRegions.Lanayru = 0b1;
				scratchPadPtr->movingActors.exploredRegions.Eldin = 0b1;
				scratchPadPtr->movingActors.exploredRegions.Faron = 0b1;
				scratchPadPtr->movingActors.exploredRegions.Ordon = 0b1;

				//Unlock Warps
				allAreaNodesPtr->Ordon.unk_0[0xD] = 0x10; // give Ordon Spring Warp
				gameInfo.localAreaNodes.unk_0[0x13] = 0x80;//give S faron warp
				gameInfo.localAreaNodes.unk_0[0xB] = 0x4;//give N faron warp
				allAreaNodesPtr->Eldin.unk_0[0x9] |= 0x20; // give Death Mountain Warp
				allAreaNodesPtr->Eldin.unk_0[0x8] |= 0x80; // give Kakariko Warp
				allAreaNodesPtr->Hyrule_Field.unk_0[0x17] = 0x8; //give Bridge of Eldin Warp
				allAreaNodesPtr->Hyrule_Field.unk_0[0xB] |= 0x8;//give castle town warp
				allAreaNodesPtr->Hyrule_Field.unk_0[0x9] |= 0x20; //give Gorge Warp
				allAreaNodesPtr->Lanayru.unk_0[0xB] |= 0x4; // give Zora's Domain Warp
				allAreaNodesPtr->Lanayru.unk_0[0xA] |= 0x4;//give lake hylia warp

				//Faron Escape
				if (Singleton::getInstance()->isForestEscapeEnabled == 1)
				{
					eventBitsPtr[0x6] |= 0x26; //warp the kak bridge, give map warp, set Forest Temple Story Flag
				}
				else
				{
					eventBitsPtr[0x6] |= 0x24; //warp the kak bridge, give map warp
				}

				//Skip MDH?
				if (Singleton::getInstance()->isMDHSkipEnabled == 1)
				{
					//set MDH flags
					gameInfo.scratchPad.eventBits[0xC] |= 0x1; //MDH Started
					gameInfo.scratchPad.eventBits[0x1E] |= 0x8; //MDH Completed
				}

				gameInfo.nextStageVars.triggerLoad |= 1;
				return item;
			}
			else
			{
				//Faron Escape
				if (Singleton::getInstance()->isForestEscapeEnabled == 1)
				{
					eventBitsPtr[0x6] |= 0x26; //warp the kak bridge, give map warp, set Forest Temple Story Flag
				}
				else
				{
					eventBitsPtr[0x6] |= 0x24; //warp the kak bridge, give map warp
				}
				gameInfo.localAreaNodes.unk_0[0x9] = 0x10;//unlock N Faron gate
				tools::setItemFlag(ItemFlags::Vessel_Of_Light_Faron);
				return item;
			}
			return item;
		}
		else if (item == items::Item::Vessel_Of_Light_Eldin)
		{
			tp::d_com_inf_game::ScratchPad* scratchPadPtr = &gameInfo.scratchPad;
			u8* eventBitsPtr = &scratchPadPtr->eventBits[0];
			tools::setItemFlag(ItemFlags::Vessel_Of_Light_Eldin);
			eventBitsPtr[0x6] |= 0x1; //tame Epona
			eventBitsPtr[0xA] |= 0x8; //Beat KB1
			eventBitsPtr[0x14] |= 0x10; //Put Bo Outside
			eventBitsPtr[0x7] = 0xD6; //skip Gor Coron Sumo and Enter Mines also Trigger KB1 and mark Post-KB1 CS as watched
			return item;
		}
		else if (item == items::Item::Vessel_Of_Light_Lanayru)
		{
			tools::setItemFlag(ItemFlags::Vessel_Of_Light_Lanayru);
			return item;
		}
		else if (item == items::Item::Hylian_Shield && tp::d_a_alink::checkStageName("R_SP109"))
		{//set flag for Malo's Hylian Shield check
		tools::setItemFlag(ItemFlags::Null_D9);
		}
		else if (item == items::Item::Hawkeye)
		{//set flag for Hawkeye check
		tools::setItemFlag(ItemFlags::Null_D8);
		}
		else if (item == items::Item::Dungeon_Map && tp::d_a_alink::checkStageName("D_MN11"))
		{//set flag to be able to enter kitchen
		gameInfo.localAreaNodes.unk_0[0x10] |= 0x20;
		}

		for (u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if (tp::d_a_alink::checkStageName(sourceCheck->stage) || (tp::d_a_alink::checkStageName("F_SP128") && 0 == strcmp(sourceCheck->stage, "R_SP128")))
			{

				if (isGrottoCheckOk(i))
				{

					if (isProgressiveEnabled == 1 && item == items::Item::Ancient_Sky_Book_completed)
					{
						item = items::Item::Ancient_Sky_Book_partly_filled;
					}
					// Correct stage
					if (sourceCheck->itemID == item || (isItemBombs(item) && isItemBombs(sourceCheck->itemID)) ||
						(item == items::Item::Red_Rupee && sourceCheck->itemID == items::Item::Giant_Bomb_Bag) ||
						(item == items::Item::Ooccoo_FT && sourceCheck->itemID == items::Item::Ooccoo_Dungeon) ||
						(item == items::Item::Lantern_Refill_Shop && sourceCheck->itemID == items::Item::Lantern_Oil_Shop) ||
						(item == items::Item::Lantern_Refill_Scooped && sourceCheck->itemID == items::Item::Lantern_Oil_Scooped) ||
						(sourceCheck->itemID == items::Item::Superb_Soup && (item == items::Item::Simple_Soup || item == items::Item::Good_Soup)))
					{
						bool isOk = false;

						if (sourceCheck->type == item::ItemType::Bug || sourceCheck->type == item::ItemType::Dungeon || sourceCheck->itemID == items::Item::Heart_Container ||
							sourceCheck->itemID == items::Item::Ball_and_Chain || sourceCheck->itemID == items::Item::Ancient_Sky_Book_empty)
						{//bugs have unique itemids so position doesn't matter
						//dungeon items are unique in their dungeon
						//there can only be one heart container per stage in vanilla, so position doesn't matter (also each one can be at 2 locations: if gotten after boss or if coming back)
						//BaC can be anywhere in the room so don't check the position
						//empty sky book can be outside the house or inside the house so don't check coords
							isOk = true;
						}
						else if (sourceCheck->itemID == items::Item::Piece_of_Heart && (0 == strcmp("F_SP127", sourceCheck->stage)))
						{//fishing hole freestanding PoH
						 //it's alone in the fishing hole so it can only be that one
							isOk = true;
						}
						else
						{
							if (sourceCheck->type == item::ItemType::PoeSoul)
							{//poes can move a lot so give them more range
								//poe range= ~1400
								rangeX = 2800.0f;
								rangeY = 1400.0f;
								rangeZ = 2800.0f;
							}
							else if (sourceCheck->itemID == items::Item::Piece_of_Heart && ((0 == strcmp("F_SP121", sourceCheck->stage) && sourceCheck->room == 6) ||
								(0 == strcmp("F_SP109", sourceCheck->stage) && sourceCheck->room == 0) || (0 == strcmp("F_SP121", sourceCheck->stage) && sourceCheck->room == 3) ||
								(0 == strcmp("F_SP121", sourceCheck->stage) && sourceCheck->room == 0) || (0 == strcmp("F_SP128", sourceCheck->stage) && sourceCheck->room == 0)))
							{//freestanding PoH
							// they can be moved by boomerang and clawshot, so give them more range
							// clawshot and boomrang targetting range ~2000
								rangeX = 3000.0f;
								rangeY = 3000.0f;
								rangeZ = 3000.0f;
							}
							else if (sourceCheck->itemID == items::Item::Small_Key && 0 == strcmp("D_MN05", sourceCheck->stage) && sourceCheck->room == 5)
							{//totem chest in FT (it can be knocked down both ways so more range is required)
								rangeX = 500.0f;
								rangeY = 500.0f;
								rangeZ = 800.0f;
							}
							if (tools::fCompare(sourceCheck->position[0], pos[0]) < rangeX)
							{
								if (tools::fCompare(sourceCheck->position[1], pos[1]) < rangeY)
								{
									if (tools::fCompare(sourceCheck->position[2], pos[2]) < rangeZ)
									{
										isOk = true;
									}
								}
							}
						}
						if (isOk)
						{
							snprintf(lastSourceInfo, 50, "%s->%d->%x", sourceCheck->stage, sourceCheck->room, sourceCheck->itemID);
							if (sourceCheck->destination)
							{
								snprintf(lastDestInfo, 50, "%s->%d->%x", sourceCheck->destination->stage, sourceCheck->destination->room, sourceCheck->destination->itemID);
								item = sourceCheck->destination->itemID;
								if (sourceCheck->type == item::ItemType::Bug || sourceCheck->itemID == items::Item::Heart_Container)
								{
									sourceCheck->destination = &item::checks[263];//green rupee
								}
								else if (sourceCheck->type == item::ItemType::Shop)
								{
									sourceCheck->destination = &item::checks[i];
								}
								else if (sourceCheck->itemID != items::Item::Big_Quiver && sourceCheck->itemID != items::Item::Giant_Quiver && // quiver checks called twice somehow
									sourceCheck->type != item::ItemType::Dungeon && sourceCheck->type != item::ItemType::Gear && sourceCheck->type != item::ItemType::Equip) // some checks are called twice i don't wanna list them all, but dungeon items, gear, and equipable items only have one check each intheir stage
								{
									// Unset this check
									sourceCheck->destination = nullptr;
								}
								//progressive checks (doesn't work if you already have items when generating seed)
								if (isProgressiveEnabled == 1)
								{
									if (item == items::Item::Wooden_Sword && tools::checkItemFlag(ItemFlags::Wooden_Sword))
									{
										item = items::Item::Ordon_Sword;
										gameInfo.scratchPad.equipedItems.sword = 0x28;
									}
									else if (item == items::Item::Ordon_Sword && !tools::checkItemFlag(ItemFlags::Wooden_Sword))
									{
										item = items::Item::Wooden_Sword;
										gameInfo.scratchPad.equipedItems.sword = 0x3F;
									}
									else if (item == items::Item::Master_Sword && tools::checkItemFlag(ItemFlags::Master_Sword))
									{//for when MS and light Ms are implemented
										item = items::Item::Master_Sword_Light;
										gameInfo.scratchPad.equipedItems.sword = 0x49;
									}
									else if (item == items::Item::Master_Sword_Light && !tools::checkItemFlag(ItemFlags::Master_Sword))
									{//for when MS and light Ms are implemented
										item = items::Item::Master_Sword;
										gameInfo.scratchPad.equipedItems.sword = 0x29;
									}
									else if (item == items::Item::Clawshot && tools::checkItemFlag(ItemFlags::Clawshot))
									{
										item = items::Item::Clawshots;
									}
									else if (item == items::Item::Clawshots && !tools::checkItemFlag(ItemFlags::Clawshot))
									{
										item = items::Item::Clawshot;
									}
									else if (item == items::Item::Heros_Bow)
									{
										if (tools::checkItemFlag(ItemFlags::Heros_Bow) &&
											!tools::checkItemFlag(ItemFlags::Big_Quiver))
										{
											item = items::Item::Big_Quiver;
										}
										else if (tools::checkItemFlag(ItemFlags::Heros_Bow) &&
											tools::checkItemFlag(ItemFlags::Big_Quiver))
										{
											item = items::Item::Giant_Quiver;
										}
									}
									else if (item == items::Item::Big_Quiver)
									{
										if (!tools::checkItemFlag(ItemFlags::Heros_Bow))
										{
											item = items::Item::Heros_Bow;
										}
										else if (tools::checkItemFlag(ItemFlags::Big_Quiver))
										{
											item = items::Item::Giant_Quiver;
										}
									}
									else if (item == items::Item::Giant_Quiver)
									{
										if (!tools::checkItemFlag(ItemFlags::Heros_Bow))
										{
											item = items::Item::Heros_Bow;
										}
										else if (!tools::checkItemFlag(ItemFlags::Big_Quiver))
										{
											item = items::Item::Big_Quiver;
										}
									}
									else if (item == items::Item::Big_Wallet &&
										tools::checkItemFlag(ItemFlags::Big_Wallet))
									{
										item = items::Item::Giant_Wallet;
									}
									else if (item == items::Item::Giant_Wallet &&
										!tools::checkItemFlag(ItemFlags::Big_Wallet))
									{
										item = items::Item::Big_Wallet;
									}
									else if (item == items::Item::Ancient_Sky_Book_empty)
									{
										if (!tools::checkItemFlag(ItemFlags::Ancient_Sky_Book_empty))
										{
											item = items::Item::Ancient_Sky_Book_empty;
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DF))
										{//letter 1
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DF);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DE))
										{//letter 2
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DE);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DD))
										{//letter 3
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DD);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DC))
										{//letter 4
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DC);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DB))
										{//letter 5
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DB);
										}
										else if (tools::checkItemFlag(ItemFlags::Null_DB))
										{
											gameInfo.scratchPad.eventBits[0x60] |= 0x4; //set shad to be back in the basement
											gameInfo.scratchPad.eventBits[0x25] |= 0x40; //Set the Owl Statue in Kak to be able to be moved
											gameInfo.scratchPad.eventBits[0x5F] |= 0x20; //Shad leaves so you can warp
											gameInfo.scratchPad.eventBits[0x3B] |= 0x8; //repairs Cannon at lake
											item = items::Item::Ancient_Sky_Book_completed;
										}
									}
									else if (item == items::Item::Ancient_Sky_Book_partly_filled)
									{
										if (!tools::checkItemFlag(ItemFlags::Ancient_Sky_Book_empty))
										{
											item = items::Item::Ancient_Sky_Book_empty;
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DF))
										{//letter 1
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DF);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DE))
										{//letter 2
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DE);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DD))
										{//letter 3
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DD);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DC))
										{//letter 4
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DC);
										}
										else if (!tools::checkItemFlag(ItemFlags::Null_DB))
										{//letter 5
											item = items::Item::Ancient_Sky_Book_partly_filled;
											tools::setItemFlag(ItemFlags::Null_DB);
										}
										else if (tools::checkItemFlag(ItemFlags::Null_DB))
										{
											gameInfo.scratchPad.eventBits[0x60] |= 0x4; //set shad to be back in the basement
											gameInfo.scratchPad.eventBits[0x25] |= 0x40; //Set the Owl Statue in Kak to be able to be moved
											gameInfo.scratchPad.eventBits[0x5F] |= 0x20; //Shad leaves so you can warp
											gameInfo.scratchPad.eventBits[0x3B] |= 0x8; //repairs Cannon at lake
											item = items::Item::Ancient_Sky_Book_completed;
										}
									}
									else if (item == items::Item::Empty_Bomb_Bag)
									{
										if (itemWheel->Bomb_Bag_1 == 0xFF)
										{
											item = items::Item::Bomb_Bag_Regular_Bombs;
										}
										else if (itemWheel->Bomb_Bag_2 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (itemWheel->Bomb_Bag_3 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (!tools::checkItemFlag(ItemFlags::Giant_Bomb_Bag))
										{
											item = items::Item::Giant_Bomb_Bag;
										}
										else
										{
											item = items::Item::Bombs_30;
										}
									}
									else if (item == items::Item::Goron_Bomb_Bag)
									{
										if (itemWheel->Bomb_Bag_1 == 0xFF)
										{
											item = items::Item::Bomb_Bag_Regular_Bombs;
										}
										else if (itemWheel->Bomb_Bag_2 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (itemWheel->Bomb_Bag_3 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (!tools::checkItemFlag(ItemFlags::Giant_Bomb_Bag))
										{
											item = items::Item::Giant_Bomb_Bag;
										}
										else
										{
											item = items::Item::Bombs_30;
										}
									}
									else if (item == items::Item::Giant_Bomb_Bag)
									{
										if (itemWheel->Bomb_Bag_1 == 0xFF)
										{
											item = items::Item::Bomb_Bag_Regular_Bombs;
										}
										else if (itemWheel->Bomb_Bag_2 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (itemWheel->Bomb_Bag_3 == 0xFF)
										{
											item = items::Item::Goron_Bomb_Bag;
										}
										else if (!tools::checkItemFlag(ItemFlags::Giant_Bomb_Bag))
										{
											item = items::Item::Giant_Bomb_Bag;
										}
										else
										{
											item = items::Item::Bombs_30;
										}
									}
									else if (item == items::Item::Key_Shard_1)
									{
										if (tools::checkItemFlag(ItemFlags::Key_Shard_1) &&
											!tools::checkItemFlag(ItemFlags::Key_Shard_2))
										{
											item = items::Item::Key_Shard_2;
										}
										else if (tools::checkItemFlag(ItemFlags::Key_Shard_1) &&
											tools::checkItemFlag(ItemFlags::Key_Shard_2))
										{
											item = items::Item::Big_Key_Goron_Mines;
											tools::setItemFlag(ItemFlags::Key_Shard_3);//set this flag to show full key on the map
										}
									}
									else if (item == items::Item::Key_Shard_2)
									{
										if (!tools::checkItemFlag(ItemFlags::Key_Shard_1))
										{
											item = items::Item::Key_Shard_1;
										}
										else if (tools::checkItemFlag(ItemFlags::Key_Shard_1) &&
											tools::checkItemFlag(ItemFlags::Key_Shard_2))
										{
											item = items::Item::Big_Key_Goron_Mines;
											tools::setItemFlag(ItemFlags::Key_Shard_3);//set this flag to show full key on the map
										}
									}
									else if (item == items::Item::Big_Key_Goron_Mines)
									{
										if (!tools::checkItemFlag(ItemFlags::Key_Shard_1))
										{
											item = items::Item::Key_Shard_1;
										}
										else if (tools::checkItemFlag(ItemFlags::Key_Shard_1) &&
											!tools::checkItemFlag(ItemFlags::Key_Shard_2))
										{
											item = items::Item::Key_Shard_2;
										}
										else
										{
											tools::setItemFlag(ItemFlags::Key_Shard_3);//set this flag to show full key on the map
										}
									}
									else if (item == items::Item::Dominion_Rod && tools::checkItemFlag(ItemFlags::Dominion_Rod))
									{//for when powered dominion rod is implemented
									item = items::Item::Dominion_Rod_Uncharged;
									}
									else if (item == items::Item::Dominion_Rod_Uncharged && !tools::checkItemFlag(ItemFlags::Dominion_Rod))
									{//for when MS and light Ms are implemented
									item = items::Item::Dominion_Rod;
									}
								}
								
								if (item == items::Item::Poe_Soul && gameInfo.scratchPad.poeCount < 60)
								{//increase poe counter
									gameInfo.scratchPad.poeCount++;
								}
								else if (item == items::Item::Shadow_Crystal)
								{//shadow crystal doesn't actually do anything so we have to do its functionnality ourselves
									game_patch::giveMidnaTransform();
									if (Singleton::getInstance()->isMDHSkipEnabled == 1)
									{
										gameInfo.scratchPad.unk_1F[0x11] |= 0x8; //Midna on Back
									}
								}
								else if (item == items::Item::Dominion_Rod_Uncharged)
								{
									gameInfo.scratchPad.eventBits[0x25] |= 0x80;//set flag to charge dominion rod
								}
								else if (item == items::Item::Ordon_Pumpkin)
								{
									gameInfo.scratchPad.allAreaNodes.Snowpeak_Ruins.unk_0[0x9] |= 0x4; //Unlock Lobby Courtyard Door
									gameInfo.scratchPad.eventBits[0x4] |= 0x80; //Told Yeta About Pumpkin
									gameInfo.scratchPad.eventBits[0x0] |= 0x22; //Yeto took pumpkin and put it in soup
									gameInfo.scratchPad.eventBits[0x14] |= 0x40; //Unlock Lobby Door
									gameInfo.localAreaNodes.unk_0[0x9] |= 0x4;//unlock courtyard door
								}
								else if (item == items::Item::Ordon_Goat_Cheese)
								{
									gameInfo.scratchPad.allAreaNodes.Snowpeak_Ruins.unk_0[0x9] |= 0x8; //Unlock West Wing Door
									gameInfo.scratchPad.eventBits[0x1] |= 0x20; //Told Yeta About Cheese
									gameInfo.scratchPad.eventBits[0x0] |= 0x11; //Yeto took Cheese and put it in soup
									gameInfo.scratchPad.eventBits[0x14] |= 0x20; //Unlock West Door
									gameInfo.localAreaNodes.unk_0[0x9] |= 0x8;//unlock west wing door
								}
								else if (item == 0xE1)
								{
									gameInfo.scratchPad.eventBits[0x29] |= 0x4;//give ending blow
								}
								else if (item == 0xE2)
								{
									gameInfo.scratchPad.eventBits[0x29] |= 0x8;//give shield attack
								}
								else if (item == 0xE3)
								{
									gameInfo.scratchPad.eventBits[0x29] |= 0x2;//give back slice
								}
								else if (item == 0xE4)
								{
									gameInfo.scratchPad.eventBits[0x29] |= 0x1;//give helm splitter
								}
								else if (item == 0xE5)
								{
									gameInfo.scratchPad.eventBits[0x2A] |= 0x80;//give mortal draw
								}
								else if (item == 0xE6)
								{
									gameInfo.scratchPad.eventBits[0x2A] |= 0x40;//give jump strike
								}
								else if (item == 0xE7)
								{
									gameInfo.scratchPad.eventBits[0x2A] |= 0x20;//give great spin
								}
								else if (item == items::Item::Reekfish_Scent)
								{
									gameInfo.scratchPad.eventBits[0x61] |= 0x20;//allow you to go to snowpeak
								}
								else if (item == items::Item::Medicine_Scent)
								{
									gameInfo.scratchPad.eventBits[0x2F] |= 0x4;//smelled Medicine Scent
								}
								else if (item == items::Item::Bed_Key)
								{
									gameInfo.scratchPad.allAreaNodes.Snowpeak_Ruins.dungeon.bigKeyGotten = 0b1; //unlock Blizzetta Door
									if (Singleton::getInstance()->isBossKeyseyEnabled == 1)
									{
										item = items::Item::Red_Rupee;
									}
								}
								else if (item == items::Item::Big_Key && Singleton::getInstance()->isBossKeyseyEnabled == 1)
								{
									item = items::Item::Red_Rupee;
								}
								else if (!tools::checkItemFlag(ItemFlags::Slingshot) &&
									(item == items::Item::Seeds_50))
								{
									item = items::Item::Blue_Rupee;
								}
								else if (!tools::checkItemFlag(ItemFlags::Heros_Bow) &&
									(item == items::Item::Arrows_10 ||
										item == items::Item::Arrows_20 ||
										item == items::Item::Arrows_30 ||
										item == items::Item::Arrows_1))
								{
									item = items::Item::Blue_Rupee;
								}
								else if (gameInfo.scratchPad.itemWheel.Bomb_Bag_1 == 0xFF && isItemBombs(item))
								{
									item = items::Item::Blue_Rupee;
								}
								else if (gameInfo.scratchPad.itemWheel.Bottle_1 == 0xFF && isItemBottleFill(item))
								{
									if (tools::checkItemFlag(ItemFlags::Lantern))
									{
										if (item == items::Item::Lantern_Oil_Shop)
										{
											item = items::Item::Lantern_Refill_Shop;
											gameInfo.scratchPad.counters.lantern_oil = gameInfo.scratchPad.counters.max_lantern_oil;
										}
										else if (item == items::Item::Lantern_Oil_Scooped)
										{
											item = items::Item::Lantern_Refill_Scooped;
											gameInfo.scratchPad.counters.lantern_oil = gameInfo.scratchPad.counters.max_lantern_oil;
										}
										else if (item == items::Item::Yellow_Chu_Jelly)
										{
											item = items::Item::Lantern_Yellow_Chu_Chu;
											gameInfo.scratchPad.counters.lantern_oil = gameInfo.scratchPad.counters.max_lantern_oil;
										}
										else
										{
											item = items::Item::Blue_Rupee;
										}
									}
									else
									{
										item = items::Item::Blue_Rupee;
									}
								}
								/*if (item == items::Item::Blue_Rupee)
								{//somehow the blue rupee item get don't work normally
									tp::d_item::execItemGet(items::Item::Blue_Rupee);
								}*/
								return item;
							}
							else
							{
								snprintf(lastDestInfo, 50, "No replacement here.");
								//no return in the case where 2 items are close enough to be considered the same (bug rewards for example)
							}
						}
					}
				}
			}
		}
		return item;
	}


	bool ChestRandomizer::isStageADungeon(char* stage)
	{
		u32 totalDungeonStages = sizeof(stage::dungeonStages) / sizeof(stage::dungeonStages[0]);
		for (u32 i = 0; i < totalDungeonStages; i++)
		{
			if (0 == strcmp(stage, stage::dungeonStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isItemBombs(u8 itemID)
	{
		if (itemID == items::Item::Bombs_5 || itemID == items::Item::Bombs_10 || itemID == items::Item::Bombs_20 || itemID == items::Item::Bombs_30 ||
			itemID == items::Item::Water_Bombs_5 || itemID == items::Item::Water_Bombs_10 || itemID == items::Item::Water_Bombs_15 ||
			itemID == items::Item::Bomblings_5 || itemID == items::Item::Bomblings_10)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ChestRandomizer::isItemBottleFill(u8 itemID)
	{
		if (itemID == items::Item::Red_Potion_Shop || itemID == items::Item::Green_Potion || itemID == items::Item::Blue_Potion || itemID == items::Item::Milk ||
			itemID == items::Item::Lantern_Oil_Shop || itemID == items::Item::Water || itemID == items::Item::Lantern_Oil_Scooped || itemID == items::Item::Red_Potion_Scooped ||
			itemID == items::Item::Nasty_soup || itemID == items::Item::Hot_spring_water_Scooped || itemID == items::Item::Fairy_Bottle || itemID == items::Item::Hot_Spring_Water_Shop ||
			itemID == items::Item::Fairy_Tears || itemID == items::Item::Worm || itemID == items::Item::Bee_Larva_Scooped || itemID == items::Item::Rare_Chu_Jelly ||
			itemID == items::Item::Red_Chu_Jelly || itemID == items::Item::Blue_Chu_Jelly || itemID == items::Item::Green_Chu_Jelly || itemID == items::Item::Yellow_Chu_Jelly ||
			itemID == items::Item::Purple_Chu_Jelly || itemID == items::Item::Simple_Soup || itemID == items::Item::Good_Soup || itemID == items::Item::Superb_Soup ||
			itemID == items::Item::Bee_Larva_Shop || itemID == items::Item::Black_Chu_Jelly)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ChestRandomizer::isStageBoss()
	{
		u32 totalBossStages = sizeof(stage::bossStages) / sizeof(stage::bossStages[0]);
		for (u32 i = 0; i < totalBossStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::bossStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageGrotto()
	{
		u32 totalGrottoStages = sizeof(stage::grottoStages) / sizeof(stage::grottoStages[0]);
		for (u32 i = 0; i < totalGrottoStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::grottoStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageDungeon()
	{
		u32 totalDungeonStages = sizeof(stage::dungeonStages) / sizeof(stage::dungeonStages[0]);
		for (u32 i = 0; i < totalDungeonStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::dungeonStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageInterior()
	{
		u32 totalInteriorStages = sizeof(stage::interiorStages) / sizeof(stage::interiorStages[0]);
		for (u32 i = 0; i < totalInteriorStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::interiorStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageCave()
	{
		u32 totalCaveStages = sizeof(stage::caveStages) / sizeof(stage::caveStages[0]);
		for (u32 i = 0; i < totalCaveStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::caveStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageSpecial()
	{
		u32 totalSpecialStages = sizeof(stage::specialStages) / sizeof(stage::specialStages[0]);
		for (u32 i = 0; i < totalSpecialStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::specialStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	bool ChestRandomizer::isStageTOD()
	{
		u32 totalTODStages = sizeof(stage::timeOfDayStages) / sizeof(stage::timeOfDayStages[0]);
		for (u32 i = 0; i < totalTODStages; i++)
		{
			if (tp::d_a_alink::checkStageName(stage::timeOfDayStages[i]))
			{
				return true;
			}
		}
		return false;
	}

	void ChestRandomizer::handleKeysanity()
	{
		if (isKeysanityEnabled == 1)
		{
			item::ItemCheck* sourceCheck;
			item::ItemCheck* destCheck;
			u32 length;
			u16 index;
			//do FT_1
			length = sizeof(keyPlacement::FT_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::FT_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::FT_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do FT_2
			length = sizeof(keyPlacement::FT_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::FT_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::FT_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do FT_3
			length = sizeof(keyPlacement::FT_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::FT_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::FT_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do FT_4
			length = sizeof(keyPlacement::FT_4) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::FT_keys[3]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::FT_4[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do GM_1
			length = sizeof(keyPlacement::GM_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::GM_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::GM_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do GM_2
			length = sizeof(keyPlacement::GM_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::GM_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::GM_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do GM_3
			length = sizeof(keyPlacement::GM_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::GM_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::GM_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do LBT_1
			length = sizeof(keyPlacement::LBT_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::LBT_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::LBT_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do LBT_2
			length = sizeof(keyPlacement::LBT_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::LBT_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::LBT_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do LBT_3
			length = sizeof(keyPlacement::LBT_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::LBT_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::LBT_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do AG_1
			length = sizeof(keyPlacement::AG_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::AG_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::AG_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do AG_2
			length = sizeof(keyPlacement::AG_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::AG_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::AG_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do AG_3
			length = sizeof(keyPlacement::AG_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::AG_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::AG_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do AG_4
			length = sizeof(keyPlacement::AG_4) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::AG_keys[3]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::AG_4[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do AG_5
			length = sizeof(keyPlacement::AG_5) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::AG_keys[4]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::AG_5[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do SPR_1
			length = sizeof(keyPlacement::SPR_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do SPR_2
			length = sizeof(keyPlacement::SPR_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do SPR_3
			length = sizeof(keyPlacement::SPR_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do SPR_4
			length = sizeof(keyPlacement::SPR_4) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[3]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_4[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do SPR_5
			length = sizeof(keyPlacement::SPR_5) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[4]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_5[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do SPR_6
			length = sizeof(keyPlacement::SPR_6) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::SPR_keys[5]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::SPR_6[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do ToT_1
			length = sizeof(keyPlacement::ToT_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::ToT_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::ToT_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do ToT_2
			length = sizeof(keyPlacement::ToT_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::ToT_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::ToT_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do ToT_3
			length = sizeof(keyPlacement::ToT_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::ToT_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::ToT_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do CitS_1
			length = sizeof(keyPlacement::CitS_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::CitS_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::CitS_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do PoT_1
			length = sizeof(keyPlacement::PoT_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_2
			length = sizeof(keyPlacement::PoT_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_3
			length = sizeof(keyPlacement::PoT_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_4
			length = sizeof(keyPlacement::PoT_4) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[3]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_4[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_5
			length = sizeof(keyPlacement::PoT_5) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[4]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_5[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_6
			length = sizeof(keyPlacement::PoT_6) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[5]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_6[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do PoT_7
			length = sizeof(keyPlacement::PoT_7) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::PoT_keys[6]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::PoT_7[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do HC_1
			length = sizeof(keyPlacement::HC_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::HC_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::HC_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do HC_2
			length = sizeof(keyPlacement::HC_2) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::HC_keys[1]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::HC_2[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			//do HC_3
			length = sizeof(keyPlacement::HC_3) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::HC_keys[2]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::HC_3[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);

			//do F_1
			length = sizeof(keyPlacement::F_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::F_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::F_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
			if (Singleton::getInstance()->isForestEscapeEnabled == 0)
			{
				//do F_2
				length = sizeof(keyPlacement::F_2) / sizeof(u16);
				destCheck = &item::checks[keyPlacement::F_keys[1]];
				do
				{
					index = tools::getRandom(length);
					sourceCheck = &item::checks[keyPlacement::F_2[index]];
				} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
					(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
				placeCheck(sourceCheck, destCheck);
			}

			//do GD_1
			length = sizeof(keyPlacement::GD_1) / sizeof(u16);
			destCheck = &item::checks[keyPlacement::GD_keys[0]];
			do
			{
				index = tools::getRandom(length);
				sourceCheck = &item::checks[keyPlacement::GD_1[index]];
			} while (sourceCheck->destination || (sourceCheck->type == item::ItemType::PoeSoul && isPoesanityEnabled == 0) ||
				(sourceCheck->type == item::ItemType::Bug && isBugsanityEnabled == 0) || (sourceCheck->type == item::ItemType::Shop && isShopsanityEnabled == 0));
			placeCheck(sourceCheck, destCheck);
		}
	}

	bool ChestRandomizer::isGrottoCheckOk(u16 checkID)
	{
		if (isStageGrotto())
		{
			u8 state = tp::d_com_inf_game::current_state;
			if (tp::d_a_alink::checkStageName(stage::grottoStages[0]))
			{
				if (state == '0')
				{
					u32 length = sizeof(grottoChecks::g1_0) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g1_0[i])
						{
							return true;
						}
					}
					return false;
				}
				else if (state == '1')
				{
					u32 length = sizeof(grottoChecks::g1_1) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g1_1[i])
						{
							return true;
						}
					}
					return false;
				}
				else if (state == '2')
				{
					u32 length = sizeof(grottoChecks::g1_2) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g1_2[i])
						{
							return true;
						}
					}
					return false;
				}
				else if (state == '3')
				{
					u32 length = sizeof(grottoChecks::g1_3) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g1_3[i])
						{
							return true;
						}
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else if (tp::d_a_alink::checkStageName(stage::grottoStages[1]))
			{
				if (state == '0')
				{
					u32 length = sizeof(grottoChecks::g2_0) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g2_0[i])
						{
							return true;
						}
					}
					return false;
				}
				else if (state == '1')
				{
					u32 length = sizeof(grottoChecks::g2_1) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g2_1[i])
						{
							return true;
						}
					}
					return false;
				}
				else if (state == '2')
				{
					u32 length = sizeof(grottoChecks::g2_2) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g2_2[i])
						{
							return true;
						}
					}
					return false;
				}
				/*else if (state == '3')
				{
					u32 length = sizeof(grottoChecks::g2_3) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g2_3[i])
						{
							return true;
						}
					}
					return false;
				}*/
				else
				{
					return true;
				}
			}
			else if (tp::d_a_alink::checkStageName(stage::grottoStages[2]))
			{
			if (state == '0')
				{
					u32 length = sizeof(grottoChecks::g3_0) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g3_0[i])
						{
							return true;
						}
					}
					return false;
				}
			/*else if (state == '1')
				{
					u32 length = sizeof(grottoChecks::g3_1) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g3_1[i])
						{
							return true;
						}
					}
					return false;

				}*/
				/*else if (state == '2')
				{
					u32 length = sizeof(grottoChecks::g3_2) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g3_2[i])
						{
							return true;
						}
					}
					return false;
				}*/
				/*else if (state == '3')
				{
					u32 length = sizeof(grottoChecks::g3_3) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g3_3[i])
						{
							return true;
						}
					}
					return false;
				}*/
				else
				{
					return true;
				}
			}
			else if (tp::d_a_alink::checkStageName(stage::grottoStages[3]))
			{
			if (state == '0')
				{
					u32 length = sizeof(grottoChecks::g4_0) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g4_0[i])
						{
							return true;
						}
					}
					return false;
				}
			else if (state == '1')
				{
					u32 length = sizeof(grottoChecks::g4_1) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g4_1[i])
						{
							return true;
						}
					}
					return false;
				}
			else if (state == '2')
				{
					u32 length = sizeof(grottoChecks::g4_2) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g4_2[i])
						{
							return true;
						}
					}
					return false;
				}
			else if (state == '3')
				{
					u32 length = sizeof(grottoChecks::g4_3) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g4_3[i])
						{
							return true;
						}
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else if (tp::d_a_alink::checkStageName(stage::grottoStages[4]))
			{
			if (state == '0')
				{
					u32 length = sizeof(grottoChecks::g5_0) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g5_0[i])
						{
							return true;
						}
					}
					return false;
				}
			/*else if (state == '1')
				{
					u32 length = sizeof(grottoChecks::g5_1) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g5_1[i])
						{
							return true;
						}
					}
					return false;
				}*/
			else if (state == '2')
				{
					u32 length = sizeof(grottoChecks::g5_2) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g5_2[i])
						{
							return true;
						}
					}
					return false;
				}
			else if (state == '3')
				{
					u32 length = sizeof(grottoChecks::g5_3) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g5_3[i])
						{
							return true;
						}
					}
					return false;
				}
			else if (state == '4')
				{
					u32 length = sizeof(grottoChecks::g5_4) / sizeof(u16);
					for (u16 i = 0; i < length; i++)
					{
						if (checkID == grottoChecks::g5_4[i])
						{
							return true;
						}
					}
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return true;
			}
		}
		else
		{
			return true;
		}
	}
}