#include "item.h"

#include <tp/d_com_inf_game.h>

#include "defines.h"
#include "items.h"

namespace mod::item
{
    u8 itemGetAnimationFlags[10] = {
        static_cast<u8>( ItemFlags::Blue_Rupee ),
        static_cast<u8>( ItemFlags::Yellow_Rupee ),
        static_cast<u8>( ItemFlags::Red_Rupee ),
        static_cast<u8>( ItemFlags::Purple_Rupee ),
        static_cast<u8>( ItemFlags::Orange_Rupee ),
        static_cast<u8>( ItemFlags::Seeds_50 ),
        static_cast<u8>( ItemFlags::Arrows_30 ),
        static_cast<u8>( ItemFlags::Arrows_20 ),
        static_cast<u8>( ItemFlags::Arrows_10 ),
        static_cast<u8>( ItemFlags::Arrows_1 ),
    };
    u32 getFlags( u8 item, u32 currentPlayerConditions )
    {
        u32 flags = currentPlayerConditions;
        switch ( item )
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

        if ( ( flags & item::Condition::Bow ) &&
             ( ( flags & item::Condition::Bombs ) || ( flags & item::Condition::Water_Bombs ) ) )
        {
            // We have bow && (bombs || waterbombs) = bombarrows
            flags |= item::Condition::Bomb_Arrows;
        }

        return flags;
    }

    u8 customSmallKeyItemIDs[10] = { items::Forest_Temple_Small_Key,
                                     items::Goron_Mines_Small_Key,
                                     items::Lakebed_Temple_Small_Key,
                                     items::Arbiters_Grounds_Small_Key,
                                     items::Snowpeak_Ruins_Small_Key,
                                     items::Temple_of_Time_Small_Key,
                                     items::City_in_The_Sky_Small_Key,
                                     items::Palace_of_Twilight_Small_Key,
                                     items::Hyrule_Castle_Small_Key,
                                     items::Bublin_Camp_Key };

    u8 customBigKeyItemIDs[7] = { items::Forest_Temple_Big_Key,
                                  items::Lakebed_Temple_Big_Key,
                                  items::Arbiters_Grounds_Big_Key,
                                  items::Temple_of_Time_Big_Key,
                                  items::City_in_The_Sky_Big_Key,
                                  items::Palace_of_Twilight_Big_Key,
                                  items::Hyrule_Castle_Big_Key };

    u8 customDungeonMapItemIDs[9] = { items::Forest_Temple_Dungeon_Map,
                                      items::Goron_Mines_Dungeon_Map,
                                      items::Lakebed_Temple_Dungeon_Map,
                                      items::Arbiters_Grounds_Dungeon_Map,
                                      items::Snowpeak_Ruins_Dungeon_Map,
                                      items::Temple_of_Time_Dungeon_Map,
                                      items::City_in_The_Sky_Dungeon_Map,
                                      items::Palace_of_Twilight_Dungeon_Map,
                                      items::Hyrule_Castle_Dungeon_Map };

    u8 customCompassItemIDs[9] = { items::Forest_Temple_Compass,
                                   items::Goron_Mines_Compass,
                                   items::Lakebed_Temple_Compass,
                                   items::Arbiters_Grounds_Compass,
                                   items::Snowpeak_Ruins_Compass,
                                   items::Temple_of_Time_Compass,
                                   items::City_in_The_Sky_Compass,
                                   items::Palace_of_Twilight_Compass,
                                   items::Hyrule_Castle_Compass };

    u8 customHiddenSkillItemIDs[7] = { items::Ending_Blow,
                                       items::Shield_Attack,
                                       items::Back_Slice,
                                       items::Helm_Splitter,
                                       items::Mortal_Draw,
                                       items::Jump_Strike,
                                       items::Great_Spin };
}     // namespace mod::item