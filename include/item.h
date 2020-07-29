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
	u32 getFlags(u8 item, u32 currentPlayerConditions);

	/**
	 * Bitwise conditions for ItemCheck
	 */
	enum Condition : u32
	{
		OR 				= 0b000000000000000000000,
		AND 			= 0b100000000000000000000,
		Lantern 		= 0b010000000000000000000,//0x48
		Iron_Boots 		= 0b001000000000000000000,//0x45
		Boomerang 		= 0b000100000000000000000,//0x40
		Slingshot 		= 0b000010000000000000000,//0x4B
		Bow 			= 0b000001000000000000000,//0x43
		Bombs 			= 0b000000100000000000000,//0x70 0x51
		Water_Bombs 	= 0b000000010000000000000,//0x70 0x51
		Bomb_Arrows 	= 0b000000001000000000000,//0x43 0x70 0x51
		Ball_And_Chain 	= 0b000000000100000000000,//0x42
		Clawshot 		= 0b000000000010000000000,//0x44
		Double_Clawshot = 0b000000000001000000000,//0x47
		Spinner 		= 0b000000000000100000000,//0x41
		Dominion_Rod 	= 0b000000000000010000000,//0x46
		Zora_Armor 		= 0b000000000000001000000,//0x31
		Small_Key		= 0b000000000000000100000,//locked
		Coral_Earring	= 0b000000000000000010000,//0x3D
		Wooden_Sword	= 0b000000000000000001000,//0x3F
		Ordon_Sword		= 0b000000000000000000100,//0x28
		Shadow_Crystal	= 0b000000000000000000010,//0x32 
		Shield			= 0b000000000000000000001,//0x2C
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
		Key = 8,
		Bottle = 9,
		Bug = 10,
		PoeSoul = 11,
		Shop = 12,
		Skill = 13,
		Scent = 14
	};

	/**
	 * Contains item check info
	 */
	struct ItemCheck
	{
		u8 itemID;
		u8 type;
		char* stage;
		u8 room;
		u8 sourceLayer;
		u8 destLayer;
		u32 condition;
		float position[3];
		ItemCheck* source;
		ItemCheck* destination;
	};

	/**
	 * Contains the values for the flags to be set to skip the animations of first getting specific items
	 */
	extern u8 itemGetAnimationFlags[10];
	extern u8 itemsWithNoFieldModel[2];
}