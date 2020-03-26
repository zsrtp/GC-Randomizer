#include "chestRando.h"
#include "defines.h"
#include "item.h"
#include "items.h"
#include "itemChecks.h"
#include "tools.h"
#include "array.h"
#include "stage.h"

#include <tp/d_com_inf_game.h>
#include <tp/JFWSystem.h>
#include <cstdio>
#include <cstring>

namespace mod
{
	void ChestRandomizer::generate()
	{
		// Reset
		swordState = 0;
		bowState = 0;
		clawshotState = 0;
		walletState = 0;
		bookState = 0;
		bombBagState = 0;
		keyShardState = 0;
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
					if(destCheck->itemID == items::Item::Ordon_Sword)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x1, destCheck);//to prevent woodensword from being overwritten before losing it			
					}
					else if (destCheck->itemID == items::Item::Ordon_Shield || destCheck->itemID == items::Item::Wooden_Shield || destCheck->itemID == items::Item::Hylian_Shield)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x2, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
					}
					else if (destCheck->itemID == items::Item::Ancient_Sky_Book_partly_filled)
					{
						sourceCheck = findSource(destCheck->destLayer, 0x12, destCheck);//to prevent softlocking the game when you try to get ordon shield check		
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
		for(u16 i = 0; i < totalChecks; i++)
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
		}

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
		 (isStageADungeon(sourceCheck->stage) && destCheck->itemID == items::Item::Heart_Container) || //no heart containers in dungeons
		 (destCheck->itemID == items::Item::Ancient_Sky_Book_partly_filled && (0 == strcmp("D_MN06", sourceCheck->stage) || 0 == strcmp("D_MN06A", sourceCheck->stage))) || //no sky letters in ToT
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
			
			case items::Item::Key_Shard_1:
				result = true;
			break;
			
			case items::Item::Key_Shard_2:
				result = true;
			break;
			
			case items::Item::Big_Key_Goron_Mines:
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
		
		/*if (item == items::Item::Sera_Bottle)
		{			//for testing only
			item = itemThatReplacesHalfMilk;
			return item;
		}
		else if (item == items::Item::Slingshot)
		{			//for testing only
			item = itemThatReplacesSlingShot;
			return item;
		}*/
		
		for(u16 i = 0; i < totalChecks; i++)
		{
			sourceCheck = &item::checks[i];

			if(0 == strcmp(gameInfo.currentStage, sourceCheck->stage) || (0 == strcmp(gameInfo.currentStage, "F_SP128") && 0 == strcmp(sourceCheck->stage, "R_SP128")))
			{

				if (isProgressiveEnabled == 1 && item == items::Item::Ancient_Sky_Book_completed)
				{
					item = items::Item::Ancient_Sky_Book_partly_filled;
				}
				// Correct stage
				if(sourceCheck->itemID == item || (isItemBombs(item) && isItemBombs(sourceCheck->itemID)) ||
				(item == items::Item::Red_Rupee && sourceCheck->itemID == items::Item::Giant_Bomb_Bag) || 
				(item == items::Item::Lantern_Refill_Shop && sourceCheck->itemID == items::Item::Lantern_Oil_Shop) || 
				(item == items::Item::Lantern_Refill_Scooped && sourceCheck->itemID == items::Item::Lantern_Oil_Scooped) || 
				(sourceCheck->itemID == items::Item::Superb_Soup && (item == items::Item::Simple_Soup || item == items::Item::Good_Soup)))
				{
					bool isOk = false;
					
					if (sourceCheck->type == item::ItemType::Bug || sourceCheck->type == item::ItemType::Dungeon)
					{//bugs have unique itemids so position doesn't matter
					//dungeon items are unique in their dungeon
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
							else if (sourceCheck->itemID != items::Item::Big_Quiver && sourceCheck->itemID != items::Item::Giant_Quiver && sourceCheck->itemID != items::Item::Zora_Armor) // quiver checks and zora armor check called twice somehow
							{
								// Unset this check
								sourceCheck->destination = nullptr;
							}
							//progressive checks (doesn't work if you already have items when generating seed)
							if (isProgressiveEnabled == 1)
							{
								if(item == items::Item::Wooden_Sword)
								{
									if (swordState == 1)
									{
										item = items::Item::Ordon_Sword;
										swordState = 2;
									}	
									else 
									{
										swordState = 1;
									}
								}
								else if(item == items::Item::Ordon_Sword)
								{
									if (swordState == 0)
									{
										item = items::Item::Wooden_Sword;
										swordState = 1;
									}	
									else 
									{
										swordState = 2;
									}		
								}
								else if(item == items::Item::Clawshot)
								{
									if (clawshotState == 1)
									{
										item = items::Item::Clawshots;
										clawshotState = 2;
									}		
									else 
									{
										clawshotState = 1;
									}	
								}
								else if(item == items::Item::Clawshots)
								{
									if (clawshotState == 0)
									{
										item = items::Item::Clawshot;
										clawshotState = 1;
									}			
									else 
									{
										clawshotState = 2;
									}					
								}
								else if(item == items::Item::Heros_Bow)
								{
									if (bowState == 1)
									{
										item = items::Item::Big_Quiver;
										bowState = 2;
									}
									else if (bowState == 2)
									{
										item = items::Item::Giant_Quiver;
										bowState = 3;
									}									
									else 
									{
										bowState = 1;
									}						
								}
								else if(item == items::Item::Big_Quiver)
								{
									if (bowState == 0)
									{
										item = items::Item::Heros_Bow;
										bowState = 1;
									}
									else if (bowState == 2)
									{
										item = items::Item::Giant_Quiver;
										bowState = 3;
									}									
									else 
									{
										bowState = 2;
									}
								}
								else if(item == items::Item::Giant_Quiver)
								{
									if (bowState == 0)
									{
										item = items::Item::Heros_Bow;
										bowState = 1;
									}
									else if (bowState == 1)
									{
										item = items::Item::Big_Quiver;
										bowState = 2;
									}									
									else 
									{
										bowState = 3;
									}
								}
								else if(item == items::Item::Big_Wallet)
								{
									if (walletState == 1)
									{
										item = items::Item::Giant_Wallet;
										walletState = 2;
									}			
									else 
									{
										walletState = 1;
									}
								}
								else if(item == items::Item::Giant_Wallet)
								{
									if (walletState == 0)
									{
										item = items::Item::Big_Wallet;
										walletState = 1;
									}			
									else 
									{
										walletState = 2;
									}
								}
								else if(item == items::Item::Ancient_Sky_Book_empty)
								{
									if (bookState > 1 && bookState < 6)
									{
										item = items::Item::Ancient_Sky_Book_partly_filled;
										bookState++;
									}
									else if (bookState == 6)
									{
										item = items::Item::Ancient_Sky_Book_completed;
										bookState = 7;
									}									
									else 
									{
										bookState = 1;
									}
								}
								else if(item == items::Item::Ancient_Sky_Book_partly_filled)
								{
									if (bookState == 0)
									{
										item = items::Item::Ancient_Sky_Book_empty;
										bookState = 1;
									}
									else if (bookState == 6)
									{
										item = items::Item::Ancient_Sky_Book_completed;
										bookState = 7;
									}									
									else 
									{
										bookState++;
									}
								}
								else if(item == items::Item::Bomb_Bag_Regular_Bombs)
								{
									if (bombBagState == 1 || bombBagState == 2)
									{
										item = items::Item::Goron_Bomb_Bag;
										bombBagState++;
									}	
									else if (bombBagState == 3)
									{
										item = items::Item::Giant_Bomb_Bag;
										bombBagState = 4;
									} 									
									else 
									{
										bombBagState = 1;
									}
								}
								else if(item == items::Item::Goron_Bomb_Bag)
								{
									if (bombBagState == 0)
									{
										item = items::Item::Bomb_Bag_Regular_Bombs;
										bombBagState = 1;
									}
									else if (bombBagState == 3)
									{
										item = items::Item::Giant_Bomb_Bag;
										bombBagState = 4;
									} 									
									else 
									{
										bombBagState++;
									}
								}
								else if(item == items::Item::Giant_Bomb_Bag)
								{
									if (bombBagState == 0)
									{
										item = items::Item::Bomb_Bag_Regular_Bombs;
										bombBagState = 1;
									}
									else if (bombBagState == 1 || bombBagState == 2)
									{
										item = items::Item::Goron_Bomb_Bag;
										bombBagState++;
									}									
									else 
									{
										bombBagState = 4;
									}
								}
								else if(item == items::Item::Key_Shard_1)
								{
									if (keyShardState == 1)
									{
										item = items::Item::Key_Shard_2;
										bombBagState = 2;
									}
									else if (keyShardState == 2)
									{
										item = items::Item::Big_Key_Goron_Mines;
										bombBagState = 3;
									}									
									else 
									{
										bombBagState = 1;
									}
								}
								else if(item == items::Item::Key_Shard_2)
								{
									if (keyShardState == 0)
									{
										item = items::Item::Key_Shard_1;
										bombBagState = 1;
									}
									else if (keyShardState == 2)
									{
										item = items::Item::Big_Key_Goron_Mines;
										bombBagState = 3;
									}									
									else 
									{
										bombBagState = 2;
									}
								}
								else if(item == items::Item::Big_Key_Goron_Mines)
								{
									if (keyShardState == 0)
									{
										item = items::Item::Key_Shard_1;
										bombBagState = 1;
									}
									else if (keyShardState == 1)
									{
										item = items::Item::Key_Shard_2;
										bombBagState = 2;
									}									
									else 
									{
										bombBagState = 3;
									}
								}
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
		if(0 == strcmp(stage, stage::allStages[Stage_Lakebed_Temple]) || 0 == strcmp(stage, stage::allStages[Stage_Deku_Toad]) ||
		0 == strcmp(stage, stage::allStages[Stage_Goron_Mines]) || 0 == strcmp(stage, stage::allStages[Stage_Dangoro]) ||
		0 == strcmp(stage, stage::allStages[Stage_Forest_Temple]) || 0 == strcmp(stage, stage::allStages[Stage_Ook]) || 
		0 == strcmp(stage, stage::allStages[Stage_Temple_of_Time]) || 0 == strcmp(stage, stage::allStages[Stage_Darknut]) ||
		0 == strcmp(stage, stage::allStages[Stage_City_in_the_Sky]) || 0 == strcmp(stage, stage::allStages[Stage_Aeralfos]) || 
		0 == strcmp(stage, stage::allStages[Stage_Palace_of_Twilight]) || 0 == strcmp(stage, stage::allStages[Stage_Phantom_Zant_1]) ||
		0 == strcmp(stage, stage::allStages[Stage_Phantom_Zant_2]) || 0 == strcmp(stage, stage::allStages[Stage_Arbiters_Grounds]) || 
		0 == strcmp(stage, stage::allStages[Stage_Death_Sword]) || 0 == strcmp(stage, stage::allStages[Stage_Snowpeak_Ruins]) ||
		0 == strcmp(stage, stage::allStages[Stage_Darkhammer]))
		{
			return true;
		}
		else 
		{
			return false;
		}
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
}