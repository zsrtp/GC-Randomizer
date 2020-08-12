#include "item.h"
#include "items.h"
#include "defines.h"
#include <tp/d_com_inf_game.h>

namespace mod::item
{
	u8 itemGetAnimationFlags[10] =
	{
		static_cast<u8>(ItemFlags::Blue_Rupee),
		static_cast<u8>(ItemFlags::Yellow_Rupee),
		static_cast<u8>(ItemFlags::Red_Rupee),
		static_cast<u8>(ItemFlags::Purple_Rupee),
		static_cast<u8>(ItemFlags::Orange_Rupee),
		static_cast<u8>(ItemFlags::Seeds_50),
		static_cast<u8>(ItemFlags::Arrows_30),
		static_cast<u8>(ItemFlags::Arrows_20),
		static_cast<u8>(ItemFlags::Arrows_10),
		static_cast<u8>(ItemFlags::Arrows_1),
	};
	u32 getFlags(u8 item, u32 currentPlayerConditions)
	{
		u32 flags = currentPlayerConditions;
		switch(item)
		{
			case items::Item::Lantern:
				flags |= item::Condition::Lantern;
			break;

			case items::Item::Iron_Boots:
				flags |= item::Condition::Iron_Boots;
			break;

			case items::Item::Boomerang:
				flags |= item::Condition::Boomerang;
			break;

			case items::Item::Slingshot:
				flags |= item::Condition::Slingshot;
			break;

			case items::Item::Heros_Bow:
				flags |= item::Condition::Bow;
			break;

			case items::Item::Goron_Bomb_Bag:
				flags |= item::Condition::Bombs;
				flags |= item::Condition::Water_Bombs;
			break;

			case items::Item::Empty_Bomb_Bag:
				flags |= item::Condition::Bombs;
				flags |= item::Condition::Water_Bombs;
			break;

			case items::Item::Ball_and_Chain:
				flags |= item::Condition::Ball_And_Chain;
			break;

			case items::Item::Clawshot:
				flags |= item::Condition::Clawshot;
			break;

			case items::Item::Clawshots:
				flags |= item::Condition::Double_Clawshot;
			break;

			case items::Item::Spinner:
				flags |= item::Condition::Spinner;
			break;

			case items::Item::Dominion_Rod_Uncharged:
				flags |= item::Condition::Dominion_Rod;
			break;

			case items::Item::Zora_Armor:
				flags |= item::Condition::Zora_Armor;
			break;
			
			case items::Item::Coral_Earring:
				flags |= item::Condition::Coral_Earring;
			break;
			
			case items::Item::Wooden_Sword:
				flags |= item::Condition::Wooden_Sword;
			break;
			
			case items::Item::Ordon_Sword:
				flags |= item::Condition::Ordon_Sword;
			break;
						
			case items::Item::Shadow_Crystal:
				flags |= item::Condition::Shadow_Crystal;
			break;
						
			case items::Item::Ordon_Shield:
				flags |= item::Condition::Shield;
			break;
						
			case items::Item::Wooden_Shield:
				flags |= item::Condition::Shield;
			break;
						
			case items::Item::Hylian_Shield:
				flags |= item::Condition::Shield;
			break;
			
		}

		if ((flags & item::Condition::Bow) && ((flags & item::Condition::Bombs) || (flags & item::Condition::Water_Bombs)))
		{
			// We have bow && (bombs || waterbombs) = bombarrows
			flags |= item::Condition::Bomb_Arrows;
		}

		return flags;
	}

	u8 itemsWithNoFieldModel[0x69] =
	{
		0xA,
		0xB,
		0xC,
		0xD,
		0x16,
		0x17,
		0x18,
		0x19,
		0x1A,
		0x1B,
		0x1C,
		0x1D,
		0x2B,
		0x2C,
		0x32,
		0x35,
		0x36,
		0x3D,
		0x3E,
		0x40,
		0x41,
		0x42,
		0x43,
		0x44,
		0x45,
		0x46,
		0x47,
		0x49,
		0x4A,
		0x4B,
		0x4C,
		0x4F,
		0x50,
		0x51,
		0x55,
		0x56,
		0x60,
		0x61,
		0x63,
		0x64,
		0x65,
		0x66,
		0x67,
		0x6D,
		0x6F,
		0x75,
		0x80,
		0x81,
		0x83,
		0x84,
		0x90,
		0x91,
		0x9D,
		0x9E,
		0xA1,
		0xA2,
		0xA3,
		0xA5,
		0xA6,
		0xA7,
		0xB0,
		0xB1,
		0xB2,
		0xB3,
		0xB4,
		0xB5,
		0xC0,
		0xC1,
		0xC2,
		0xC3,
		0xC4,
		0xC5,
		0xC6,
		0xC7,
		0xC8,
		0xC9,
		0xCA,
		0xCB,
		0xCC,
		0xCD,
		0xCE,
		0xCF,
		0xD0,
		0xD1,
		0xD2,
		0xD3,
		0xD4,
		0xD5,
		0xD6,
		0xD7,
		0xE0,
		0xE9,
		0xEA,
		0xEB,
		0xED,
		0xEE,
		0xF3,
		0xF4,
		0xF5,
		0xF6,
		0xF9,
		0xFA,
		0xFB,
		0xFD,
		0xFE
	};
}