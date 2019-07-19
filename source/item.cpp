#include "item.h"
#include "items.h"
#include "defines.h"

namespace mod::item
{
	u16 getFlags(u8 item, u16 currentPlayerConditions)
	{
		u16 flags = 0;
		switch(item)
		{
			case items::Item::Lantern:
				flags = item::Condition::Lantern;
			break;

			case items::Item::Iron_Boots:
				flags = item::Condition::Iron_Boots;
			break;

			case items::Item::Boomerang:
				flags = item::Condition::Boomerang;
			break;

			case items::Item::Slingshot:
				flags = item::Condition::Slingshot;
			break;

			case items::Item::Heros_Bow:
				flags = item::Condition::Bow;
			break;

			case items::Item::Empty_Bomb_Bag:
				flags = item::Condition::Bombs;
			break;

			case items::Item::Bomb_Bag_Water_Bombs:
				flags = item::Condition::Water_Bombs;
			break;

			case items::Item::Ball_and_Chain:
				flags = item::Condition::Ball_And_Chain;
			break;

			case items::Item::Clawshot:
				flags = item::Condition::Clawshot;
			break;

			case items::Item::Clawshots:
				flags = item::Condition::Double_Clawshot;
			break;

			case items::Item::Spinner:
				flags = item::Condition::Spinner;
			break;

			case items::Item::Dominion_Rod:
				flags = item::Condition::Dominion_Rod;
			break;

			case items::Item::Zora_Armor:
				flags = item::Condition::Zora_Armor;
			break;
		}

		if ((currentPlayerConditions & item::Condition::Bow) && ((currentPlayerConditions & item::Condition::Bombs) || (currentPlayerConditions & item::Condition::Water_Bombs)))
		{
			// We have bow && (bombs || waterbombs) = bombarrows
			flags |= item::Condition::Bomb_Arrows;
		}

		return flags;
	}
}