#pragma once

#include "defines.h"

namespace mod::item
{
	/**
	 * Returns the flag(s) to an item under certain conditions
	 * 
	 * @param item Internal item ID
	 * @param currentPlayerConditions Current conditions of the player
	 */
	u16 getFlags(u8 item, u16 currentPlayerConditions);

	/**
	 * Bitwise conditions for ItemCheck
	 */
	enum Condition : u16
	{
		OR 				= 0b0000000000000000,
		AND 			= 0b1000000000000000,
		Lantern 		= 0b0100000000000000,
		Iron_Boots 		= 0b0010000000000000,
		Boomerang 		= 0b0001000000000000,
		Slingshot 		= 0b0000100000000000, // Currently not randomized! so should be always true!
		Bow 			= 0b0000010000000000,
		Bombs 			= 0b0000001000000000,
		Water_Bombs 	= 0b0000000100000000,
		Bomb_Arrows 	= 0b0000000010000000,
		Ball_And_Chain 	= 0b0000000001000000,
		Clawshot 		= 0b0000000000100000,
		Double_Clawshot = 0b0000000000010000,
		Spinner 		= 0b0000000000001000,
		Dominion_Rod 	= 0b0000000000000100,
		Zora_Armor 		= 0b0000000000000010
	};

	enum ItemType : u8
	{
		Equip = 0,
		Gear = 1,
		Dungeon = 2,
		HeartPiece = 3,
		Story = 4,
		Ammo = 5,
		Misc = 6,
		Rupee = 7,
		Key = 8
	};

	/**
	 * Contains an item that proc createItemForTrBoxDemo
	 */
	struct ItemCheck
	{
		u8 itemID;
		u8 type;
		u16 condition; // Bitwise condition
		float position[3];
		ItemCheck* source;
		ItemCheck* destination;
	};
}