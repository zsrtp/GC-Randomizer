#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"
#include "stage.h"

#include <tp/d_com_inf_game.h>
#include <tp/d_a_alink.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	void ChestRandomizer::generate()
	{
		// Reset
		
		itemFlags = &gameInfo.scratchPad.itemFlags;
		itemWeel = &gameInfo.scratchPad.itemWeel;		
		
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
		
		//do needed items in order
		for(u16 i = 0; i < sizeof(item::checkPriorityOrder)/sizeof(u16); i++)
		{
			destCheck = &item::checks[item::checkPriorityOrder[i]];
			if(!destCheck->source)
			{
				if(destCheck->itemID == items::Item::Ordon_Sword)
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
		
		//do dungeon items
		for(u16 i = 0; i <= totalChecks; i++)
		{
			destCheck = &item::checks[i];
			if(!destCheck->source && destCheck->type == item::ItemType::Dungeon)
			{
				sourceCheck = findSource(destCheck->destLayer, 0x0, destCheck);
				placeCheck(sourceCheck, destCheck);
				//layerCheckCount++;
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
					if(destCheck->itemID == items::Item::Ordon_Sword)
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
						if(destCheck->itemID == items::Item::Clawshots)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent Clawshots from being overwritten by Clawshot
						}
						else if(destCheck->itemID == items::Item::Big_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x4, destCheck);//to prevent bow from being overwritten
						}
						else if(destCheck->itemID == items::Item::Giant_Quiver)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x8, destCheck);//to prevent bow from being overwritten
						}
						else if(destCheck->itemID == items::Item::Giant_Wallet)
						{
							sourceCheck = findSource(destCheck->destLayer, 0x7, destCheck);//to prevent overwriting giant wallet with big wallet
						}
						else if(destCheck->itemID == items::Item::Giant_Bomb_Bag)
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

		// Place remaining
		for(u16 i = 0; i < totalChecks; i++)
		{
			destCheck = &item::checks[i];

			if(!destCheck->source)
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
		u16 index;
		do
		{
			index = tools::getRandom(totalChecks);
			sourceCheck = &item::checks[index];
		} while(!checkCondition(sourceCheck, destCheck) || sourceCheck->destination || sourceCheck->sourceLayer > maxLayer || sourceCheck->sourceLayer < minLayer ||
		 (destCheck->type == item::ItemType::Bug && 0 == strcmp("R_SP160", sourceCheck->stage) &&  sourceCheck->room == 3) || //agitha can't give bugs
		 (destCheck->type == item::ItemType::Dungeon && 0 != strcmp(destCheck->stage, sourceCheck->stage)) || //dungeon items only in their own dungeon
		 (destCheck->type == item::ItemType::Dungeon && index == 115)); //dont place dungeon items at clawshot goron mines check

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
		// If the destination item (which you'll receive) isn't required for this source and you don't require anything in this layer, it can be placed though
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
				// Map, compass, big key
				if (areDungeonItemsRandomized == 0)
				{
					result = true;
				}
			break;

			case item::ItemType::Story:
				// Ilia quest
				result = true;
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
		}

		switch(check->itemID)
		{
			/*case items::Item::Iron_Boots:
				result = true;
			break;*/

			case items::Item::Fishing_Rod:
				result = true;
			break;
						
			case items::Item::Master_Sword:
				result = true;
			break;
			
			case items::Item::Ancient_Sky_Book_empty:
				if (isProgressiveEnabled == 0)
				{
					result = true;
				}
			break;
			
			case items::Item::Ancient_Sky_Book_partly_filled:
				if (isProgressiveEnabled == 0)
				{
					result = true;
				}
			break;
						
			case items::Item::Ancient_Sky_Book_completed:
				if (isProgressiveEnabled == 0)
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
			if (item == items::Item::Dominion_Rod_Uncharged)
			{
				item = items::Item::Dominion_Rod_Charged;
				gameInfo.scratchPad.eventBits[0x25] |= 0x80;
			}
			else if (item == items::Item::Poe_Soul)
			{//increase poe counter
				gameInfo.scratchPad.unk_EC[0x20]++;
			}
			return item;
		}
		else if (item == items::Item::Slingshot)
		{			//for testing only
			item = itemThatReplacesSlingShot;
			if (item == items::Item::Dominion_Rod_Uncharged)
			{
				item = items::Item::Dominion_Rod_Charged;
				gameInfo.scratchPad.eventBits[0x25] |= 0x80;
			}
			else if (item == items::Item::Poe_Soul)
			{//increase poe counter
				gameInfo.scratchPad.unk_EC[0x20]++;
			}
			return item;
		}*/
		
		
		if (item == items::Item::Heart_Container && isStageBoss())
		{//set flag for HC gotten
			gameInfo.localAreaNodes.dungeon.containerGotten = 0b1;
			if (tp::d_a_alink::checkStageName(stage::allStages[Stage_Fyrus]))
			{//setting Dangoro flag, since gron mines can be beaten early and Dangoro won't spawn if you go in his room, but the battle will still start
				gameInfo.localAreaNodes.dungeon.miniBossBeaten = 0b1;			
			}
		}
		else if (item == items::Item::Iron_Boots)
		{//set flag for having talked to Bo
			gameInfo.scratchPad.eventBits[0x1C] |= 0x20;
		}
		else if (item == items::Item::Poe_Soul)
		{//decrease poe counter
			gameInfo.scratchPad.unk_EC[0x20]--;
		}
		else if (item == items::Item::Vessel_Of_Light_Faron)
		{//set tear counter to 16
			gameInfo.scratchPad.unk_EC[0x28] = 16;
			gameInfo.localAreaNodes.unk_0[0xB] |= 0x4;//give N faron warp
			gameInfo.localAreaNodes.unk_0[0x8] = 0xFF;//give midna jumps in mist area
			gameInfo.localAreaNodes.unk_0[0xC] |= 0xD1;//set flag for midna to think you followed the monkey in the mist
			u16* tempAddress = reinterpret_cast<u16*>(&gameInfo.scratchPad.eventBits[0x29]);
            *tempAddress |= 0x400;//give ending blow		
			
			gameInfo.nextStageVars.triggerLoad |= 1;
			return item;
		}
		else if (item == items::Item::Vessel_Of_Light_Eldin)
		{//set tear counter to 16
			gameInfo.scratchPad.unk_EC[0x29] = 16;
			gameInfo.localAreaNodes.unk_0[0x9] |= 0x20;//give death mountain warp
			gameInfo.localAreaNodes.unk_0[0x14] |= 1;//give midna jumps for top of sanctuary		
			gameInfo.scratchPad.itemFlags.itemFlags3.Vessel_Of_Light_Eldin = 0b1;//set flag for vessel since we'll skip it by reloading
			gameInfo.nextStageVars.triggerLoad |= 1;
			return item;
		}
		else if (item == items::Item::Vessel_Of_Light_Lanayru)
		{//set tear counter to 16
			gameInfo.scratchPad.unk_EC[0x2A] = 16;
			gameInfo.localAreaNodes.unk_0[0xA] |= 0x4;//give lake hylia warp
			gameInfo.scratchPad.allAreaNodes.Hyrule_Field.unk_0[0xB] |= 0x8;//give castle town warp
			gameInfo.scratchPad.itemFlags.itemFlags3.Vessel_Of_Light_Lanayru = 0b1;//set flag for vessel since we'll skip it by reloading
			gameInfo.nextStageVars.triggerLoad |= 1;
			return item;
		}
		
		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if(tp::d_a_alink::checkStageName(sourceCheck->stage) || (tp::d_a_alink::checkStageName("F_SP128") && 0 == strcmp(sourceCheck->stage, "R_SP128")))
			{

				if (isProgressiveEnabled == 1 && item == items::Item::Ancient_Sky_Book_completed)
				{
					item = items::Item::Ancient_Sky_Book_partly_filled;
				}
				// Correct stage
				if(sourceCheck->itemID == item || (isItemBombs(item) && isItemBombs(sourceCheck->itemID)) ||
				(item == items::Item::Red_Rupee && sourceCheck->itemID == items::Item::Giant_Bomb_Bag) ||
				(item == items::Item::Ooccoo_FT && sourceCheck->itemID == items::Item::Ooccoo_Dungeon) ||
				(item == items::Item::Lantern_Refill_Shop && sourceCheck->itemID == items::Item::Lantern_Oil_Shop) || 
				(item == items::Item::Lantern_Refill_Scooped && sourceCheck->itemID == items::Item::Lantern_Oil_Scooped) || 
				(sourceCheck->itemID == items::Item::Superb_Soup && (item == items::Item::Simple_Soup || item == items::Item::Good_Soup)))
				{
					bool isOk = false;
					
					if (sourceCheck->type == item::ItemType::Bug || sourceCheck->type == item::ItemType::Dungeon || sourceCheck->itemID == items::Item::Heart_Container || sourceCheck->itemID == items::Item::Ball_and_Chain)
					{//bugs have unique itemids so position doesn't matter
					//dungeon items are unique in their dungeon
					//there can only be one heart container per stage in vanilla, so position doesn't matter (also each one can be at 2 locations: if gotten after boss or if coming back)
					//BaC can be anywhere in the room so don't check the position	
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
						(0 == strcmp("F_SP121", sourceCheck->stage) && sourceCheck->room == 0) || (0 == strcmp("F_SP127", sourceCheck->stage) && sourceCheck->room == 0)  ||
						(0 == strcmp("F_SP128", sourceCheck->stage) && sourceCheck->room == 0)))
						{//freestanding PoH
						// they can be moved by boomerang and clawshot, so give them more range
						// clawshot and boomrang targetting range ~2000
							rangeX = 3000.0f;
							rangeY = 3000.0f;
							rangeZ = 3000.0f;
						}
						if(tools::fCompare(sourceCheck->position[0], pos[0]) < rangeX)
						{
							if(tools::fCompare(sourceCheck->position[1], pos[1]) < rangeY)
							{
								if(tools::fCompare(sourceCheck->position[2], pos[2]) < rangeZ)
								{
									isOk = true;
								}
							}
						}
					}
					if (isOk)
					{
						snprintf(lastSourceInfo, 50, "%s->%d->%x", sourceCheck->stage, sourceCheck->room, sourceCheck->itemID);
						if(sourceCheck->destination)
						{
							snprintf(lastDestInfo, 50, "%s->%d->%x", sourceCheck->destination->stage, sourceCheck->destination->room, sourceCheck->destination->itemID);
							item = sourceCheck->destination->itemID;
							if (sourceCheck->type == item::ItemType::Bug || sourceCheck->type == item::ItemType::Shop || sourceCheck->itemID == items::Item::Heart_Container)
							{
								sourceCheck->destination = &item::checks[263];//green rupee
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
								if(item == items::Item::Wooden_Sword && itemFlags->itemFlags1.Wooden_Sword == 0b1)
								{
									item = items::Item::Ordon_Sword;
								}
								else if(item == items::Item::Ordon_Sword && itemFlags->itemFlags1.Wooden_Sword == 0b0)
								{
									item = items::Item::Wooden_Sword;
								}
								else if(item == items::Item::Clawshot && itemFlags->itemFlags2.Clawshot == 0b1)
								{
									item = items::Item::Clawshots;
								}
								else if(item == items::Item::Clawshots && itemFlags->itemFlags2.Clawshot == 0b0)
								{
									item = items::Item::Clawshot;				
								}
								else if(item == items::Item::Heros_Bow)
								{
									if (itemFlags->itemFlags2.Heros_Bow == 0b1 && itemFlags->itemFlags2.Big_Quiver == 0b0)
									{
										item = items::Item::Big_Quiver;
									}
									else if (itemFlags->itemFlags2.Heros_Bow == 0b1 && itemFlags->itemFlags2.Big_Quiver == 0b1)
									{
										item = items::Item::Giant_Quiver;
									}						
								}
								else if(item == items::Item::Big_Quiver)
								{
									if (itemFlags->itemFlags2.Heros_Bow == 0b0)
									{
										item = items::Item::Heros_Bow;
									}
									else if (itemFlags->itemFlags2.Big_Quiver == 0b1)
									{
										item = items::Item::Giant_Quiver;
									}
								}
								else if(item == items::Item::Giant_Quiver)
								{
									if (itemFlags->itemFlags2.Heros_Bow == 0b0)
									{
										item = items::Item::Heros_Bow;
									}
									else if (itemFlags->itemFlags2.Big_Quiver == 0b0)
									{
										item = items::Item::Big_Quiver;
									}
								}
								else if(item == items::Item::Big_Wallet && itemFlags->itemFlags1.Big_Wallet == 0b1)
								{
									item = items::Item::Giant_Wallet;
								}
								else if(item == items::Item::Giant_Wallet && itemFlags->itemFlags1.Big_Wallet == 0b0)
								{
									item = items::Item::Big_Wallet;
								}
								else if(item == items::Item::Ancient_Sky_Book_empty)
								{
									if (itemFlags->itemFlags4.Ancient_Sky_Book_empty == 0b0)
									{
										item = items::Item::Ancient_Sky_Book_empty;
									}
									else if (itemFlags->itemFlags4.Null_DF == 0b0)
									{//letter 1
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DF = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DE == 0b0)
									{//letter 2
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DE = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DD == 0b0)
									{//letter 3
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DD = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DC == 0b0)
									{//letter 4
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DC = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DB == 0b0)
									{//letter 5
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DB = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b0)
									{//letter 6
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DA = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b1)
									{
										item = items::Item::Ancient_Sky_Book_completed;
									}
								}
								else if(item == items::Item::Ancient_Sky_Book_partly_filled)
								{
									if (itemFlags->itemFlags4.Ancient_Sky_Book_empty == 0b0)
									{
										item = items::Item::Ancient_Sky_Book_empty;
									}
									else if (itemFlags->itemFlags4.Null_DF == 0b0)
									{//letter 1
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DF = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DE == 0b0)
									{//letter 2
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DE = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DD == 0b0)
									{//letter 3
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DD = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DC == 0b0)
									{//letter 4
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DC = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DB == 0b0)
									{//letter 5
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DB = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b0)
									{//letter 6
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DA = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b1)
									{
										item = items::Item::Ancient_Sky_Book_completed;
									}
								}
								else if (item == items::Item::Ancient_Sky_Book_completed)
								{
									if (itemFlags->itemFlags4.Ancient_Sky_Book_empty == 0b0)
									{
										item = items::Item::Ancient_Sky_Book_empty;
									}
									else if (itemFlags->itemFlags4.Null_DF == 0b0)
									{//letter 1
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DF = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DE == 0b0)
									{//letter 2
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DE = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DD == 0b0)
									{//letter 3
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DD = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DC == 0b0)
									{//letter 4
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DC = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DB == 0b0)
									{//letter 5
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DB = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b0)
									{//letter 6
										item = items::Item::Ancient_Sky_Book_partly_filled;
										itemFlags->itemFlags4.Null_DA = 0b1;
									}
									else if (itemFlags->itemFlags4.Null_DA == 0b1)
									{
										item = items::Item::Ancient_Sky_Book_completed;
									}
								}
								else if(item == items::Item::Bomb_Bag_Regular_Bombs)
								{
									if (itemWeel->Bomb_Bag_1 == 0xFF)
									{
										item = items::Item::Bomb_Bag_Regular_Bombs;
									}
									else if (itemWeel->Bomb_Bag_2 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemWeel->Bomb_Bag_3 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemFlags->itemFlags2.Giant_Bomb_Bag == 0b0)
									{
										item = items::Item::Giant_Bomb_Bag;
									}
									else
									{
										item = items::Item::Bombs_30;
									}
								}
								else if(item == items::Item::Goron_Bomb_Bag)
								{
									if (itemWeel->Bomb_Bag_1 == 0xFF)
									{
										item = items::Item::Bomb_Bag_Regular_Bombs;
									}
									else if (itemWeel->Bomb_Bag_2 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemWeel->Bomb_Bag_3 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemFlags->itemFlags2.Giant_Bomb_Bag == 0b0)
									{
										item = items::Item::Giant_Bomb_Bag;
									}
									else
									{
										item = items::Item::Bombs_30;
									}
								}
								else if(item == items::Item::Giant_Bomb_Bag)
								{
									if (itemWeel->Bomb_Bag_1 == 0xFF)
									{
										item = items::Item::Bomb_Bag_Regular_Bombs;
									}
									else if (itemWeel->Bomb_Bag_2 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemWeel->Bomb_Bag_3 == 0xFF)
									{
										item = items::Item::Goron_Bomb_Bag;
									}
									else if (itemFlags->itemFlags2.Giant_Bomb_Bag == 0b0)
									{
										item = items::Item::Giant_Bomb_Bag;
									}
									else
									{
										item = items::Item::Bombs_30;
									}
								}
								else if(item == items::Item::Key_Shard_1)
								{
									if (itemFlags->itemFlags4.Key_Shard_1 == 0b1 && itemFlags->itemFlags4.Key_Shard_2 == 0b0)
									{
										item = items::Item::Key_Shard_2;
									}
									else if (itemFlags->itemFlags4.Key_Shard_1 == 0b1 && itemFlags->itemFlags4.Key_Shard_2 == 0b1)
									{
										item = items::Item::Big_Key_Goron_Mines;
										itemFlags->itemFlags4.Key_Shard_3 = 0b1;//set this flag to show full key on the map
									}
								}
								else if(item == items::Item::Key_Shard_2)
								{
									if (itemFlags->itemFlags4.Key_Shard_1 == 0b0)
									{
										item = items::Item::Key_Shard_1;
									}
									else if (itemFlags->itemFlags4.Key_Shard_1 == 0b1 && itemFlags->itemFlags4.Key_Shard_2 == 0b1)
									{
										item = items::Item::Big_Key_Goron_Mines;
										itemFlags->itemFlags4.Key_Shard_3 = 0b1;//set this flag to show full key on the map
									}
								}
								else if(item == items::Item::Big_Key_Goron_Mines)
								{
									if (itemFlags->itemFlags4.Key_Shard_1 == 0b0)
									{
										item = items::Item::Key_Shard_1;
									}
									else if (itemFlags->itemFlags4.Key_Shard_1 == 0b1 && itemFlags->itemFlags4.Key_Shard_2 == 0b0)
									{
										item = items::Item::Key_Shard_2;
									}
									else
									{
										item = items::Item::Big_Key_Goron_Mines;
										itemFlags->itemFlags4.Key_Shard_3 = 0b1;//set this flag to show full key on the map
									}
								}
								else if(item == items::Item::Master_Sword && itemFlags->itemFlags1.Master_Sword == 0b1)
								{//for when MS and light Ms are implemented
									item = items::Item::Master_Sword_Light;
								}
								else if(item == items::Item::Master_Sword_Light && itemFlags->itemFlags1.Master_Sword == 0b0)
								{//for when MS and light Ms are implemented
									item = items::Item::Master_Sword;
								}
							}
							if (item == items::Item::Dominion_Rod_Uncharged)
							{
								item = items::Item::Dominion_Rod_Charged;
								gameInfo.scratchPad.eventBits[0x25] |= 0x80;//set flag to charge dominion rod
							}
							else if (item == items::Item::Poe_Soul)
							{//increase poe counter
								gameInfo.scratchPad.unk_EC[0x20]++;
							}
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
}