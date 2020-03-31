#pragma once

#include "defines.h"

#include <tp/evt_control.h>

namespace tp::d_com_inf_game
{		
	// Should try to fill in the variables at some point
	struct ItemSlots
	{
		u8 Boomerang;
		u8 Lantern;
		u8 Spinner;
		u8 Iron_Boots;
		u8 Bow;
		u8 Hawkeye;
		u8 Ball_and_Chain;
		u8 Item_Slot;
		u8 Dominion_Rod;
		u8 Clawshot;
		u8 Double_Clawshot;
		u8 Bottle_1;
		u8 Bottle_2;
		u8 Bottle_3;
		u8 Bottle_4;
		u8 Bomb_Bag_1;
		u8 Bomb_Bag_2;
		u8 Bomb_Bag_3;
		u8 Ooccoo;
		u8 Fishing_Rod;
		u8 Horse_Call;
		u8 Sky_Book;
		u8 Slingshot;
	} __attribute__((__packed__));
	
	struct ItemFlags1
	{
		u64 Recovery_Heart_x3 : 1,
		 Fairy : 1,
		 Bombling_1 : 1,
		 Bomblings_3 : 1,
		 Bomblings_10 : 1,
		 Bomblings_5 : 1,
		 Water_Bombs_3 : 1,
		 Water_Bombs_15 : 1,
		 
		 Water_Bombs_10 : 1,
		 Water_Bombs_5 : 1,
		 Null_15 : 1,
		 Null_14 : 1,
		 Null_13 : 1,
		 Seeds_50 : 1,
		 Arrows_1 : 1,
		 Arrows_30 : 1,
		 
		 Arrows_20 : 1,
		 Arrows_10 : 1,
		 Bombs_30 : 1,
		 Bombs_20 : 1,
		 Bombs_10 : 1,
		 Bombs_5 : 1,
		 Null_9 : 1,
		 Null_8 : 1,
		 
		 Silver_Rupee : 1,
		 Orange_Rupee : 1,
		 Purple_Rupee : 1,
		 Red_Rupee : 1,
		 Yellow_Rupee : 1,
		 Blue_Rupee : 1,
		 Green_Rupee : 1,
		 Recovery_Heart : 1,
		 
		 Wooden_Sword : 1,
		 Hawkeye : 1,
		 Coral_Earring : 1,
		 Null_3C : 1,
		 Null_3B : 1,
		 Null_3A : 1,
		 Null_39 : 1,
		 Null_38 : 1,
		 
		 Null_37 : 1,
		 Giant_Wallet : 1,
		 Big_Wallet : 1,
		 Small_Wallet : 1,//never set in normal game
		 Ooccoo_Dungeon : 1,
		 Shadow_Crystal : 1,//never set in normal game
		 Zora_Armor : 1,
		 Magic_Armor : 1,
		 
		 Heros_Clothes : 1,
		 Ordon_Clothing : 1,//never set in normal game
		 Ooccoos_Note : 1,//never set in normal game
		 Hylian_Shield : 1,
		 Wooden_Shield : 1,
		 Ordon_Shield : 1,
		 Master_Sword : 1,
		 Ordon_Sword : 1,
		 
		 Ooccoo_Jr : 1,//never set in normal game
		 Big_Key : 1,
		 Ooccoo_FT : 1,
		 Compass : 1,
		 Dungeon_Map : 1,
		 Heart_Container : 1,
		 Piece_of_Heart : 1,
		 Small_Key : 1; 
	}__attribute__((__packed__));
	
	struct ItemFlags2
	{
		u64 Fishing_Rod_Earring_Worm : 1,
		 Fishing_Rod_Earring_Bee_Larva : 1,
		 Fishing_Rod_Worm : 1,
		 Fishing_Rod_Coral_Earring : 1,
		 Fishing_Rod_Bee_Larva : 1,
		 Bow_Hawkeye : 1,
		 Bow_Bombs : 1,
		 Fishing_Rod_Lure : 1,
		 
		 Null_57 : 1,
		 Giant_Quiver : 1,
		 Big_Quiver : 1,
		 Small_Quiver : 1,//never set in normal game
		 Null_53 : 1,
		 Null_52 : 1,
		 Goron_Bomb_Bag : 1,
		 Empty_Bomb_Bag : 1,//never set in normal game
		 
		 Giant_Bomb_Bag : 1,
		 Null_4E : 1,
		 Null_4D : 1,
		 Dominion_Rod_Charged : 1,//never set in normal game
		 Slingshot : 1,
		 Fishing_Rod : 1,
		 Master_Sword_Light : 1,
		 Lantern : 1,
		 
		 Clawshots : 1,
		 Dominion_Rod_Uncharged : 1,
		 Iron_Boots : 1,
		 Clawshot : 1,
		 Heros_Bow : 1,
		 Ball_and_Chain : 1,
		 Spinner : 1,
		 Boomerang : 1,
		 
		 Superb_Soup : 1,
		 Good_Soup : 1,
		 Simple_Soup : 1,
		 Purple_Chu_Jelly : 1,
		 Yellow_Chu_Jelly : 1,
		 Green_Chu_Jelly : 1,
		 Blue_Chu_Jelly : 1,
		 Red_Chu_Jelly : 1,
		 
		 Rare_Chu_Jelly : 1,
		 Bee_Larva_Scooped : 1,
		 Jovani_Bottle : 1,
		 Worm : 1,
		 Fairy_Tears : 1,
		 Bomb_Bag_Bombslings : 1,
		 Bomb_Bag_Water_Bombs : 1,
		 Bomb_Bag_Regular_Bombs : 1,
		 
		 Lantern_Refill_Shop : 1,
		 Lantern_Refill_Scooped : 1,
		 Hot_Spring_Water_Shop : 1,
		 Fairy_Bottle : 1,
		 Hot_spring_water_Scooped : 1,
		 Nasty_soup : 1,
		 Red_Potion_Scooped : 1,
		 Lantern_Oil_Scooped : 1,
		 
		 Water : 1,
		 Lantern_Oil_Shop : 1,
		 Sera_Bottle : 1,
		 Milk : 1,
		 Blue_Potion : 1,
		 Green_Potion : 1,
		 Red_Potion_Shop : 1,
		 Empty_Bottle : 1;
	}__attribute__((__packed__));
	
	struct ItemFlags3
	{
		u64 Black_Chu_Jelly : 1,
		 Bee_Larva_Shop : 1,
		 Coro_Bottle : 1,
		 Lantern_Yellow_Chu_Chu : 1,
		 Null_9B : 1,
		 Null_9A : 1,
		 Null_99 : 1,
		 Null_98 : 1,
		 
		 Null_97 : 1,
		 Null_96 : 1,
		 Null_95 : 1,
		 Null_94 : 1,
		 Null_93 : 1,
		 Null_92 : 1,
		 Asheis_Sketch : 1,
		 Aurus_Memo : 1,
		 
		 Null_8F : 1,
		 Null_8E : 1,
		 Null_8D : 1,
		 Null_8C : 1,
		 Null_8B : 1,
		 Null_8A : 1,
		 Null_89 : 1,
		 Null_88 : 1,
		 
		 Null_87 : 1,
		 Null_86 : 1,
		 Null_85 : 1,
		 Horse_Call : 1,
		 Ilias_Charm : 1,
		 Wooden_Statue : 1,
		 Invoice : 1,
		 Renardos_Letter : 1,
		 
		 Null_BF : 1,
		 Null_BE : 1,
		 Null_BD : 1,
		 Null_BC : 1,
		 Null_BB : 1,
		 Null_BA : 1,
		 Null_B9 : 1,
		 Null_B8 : 1,
		 
		 Null_B7 : 1,
		 Null_B6 : 1,
		 Medicine_Scent : 1,
		 Youths_Scent : 1,
		 Reekfish_Scent : 1,
		 Poe_Scent : 1,
		 Null_B1 : 1,
		 Ilias_Scent : 1,
		 
		 Null_AF : 1,
		 Null_AE : 1,
		 Null_AD : 1,
		 Null_AC : 1,
		 Null_AB : 1,
		 Null_AA : 1,
		 Null_A9 : 1,
		 Null_A8 : 1,
		 
		 Null_A7 : 1,
		 Null_A6 : 1,
		 Null_A5 : 1,
		 Vessel_Of_Light_Full : 1,//never set in normal game
		 Vessel_Of_Light_Lanayru : 1,
		 Vessel_Of_Light_Eldin : 1,
		 Vessel_Of_Light_Faron : 1,
		 Tear_Of_Light : 1;//never set in normal game
	}__attribute__((__packed__));
	
	struct ItemFlags4
	{
		 
		u64 Null_DF : 1,
		 Null_DE : 1,
		 Null_DD : 1,
		 Null_DC : 1,
		 Null_DB : 1,
		 Null_DA : 1,
		 Null_D9 : 1,
		 Null_D8 : 1,
		 
		 Female_Dayfly : 1,
		 Male_Dayfly : 1,
		 Female_Ant : 1,
		 Male_Ant : 1,
		 Female_Dragonfly : 1,
		 Male_Dragonfly : 1,
		 Female_Snail : 1,
		 Male_Snail : 1,
		 
		 Female_Ladybug : 1,
		 Male_Ladybug : 1,
		 Female_Mantis : 1,
		 Male_Mantis : 1,
		 Female_Pill_Bug : 1,
		 Male_Pill_Bug : 1,
		 Female_Phasmid : 1,
		 Male_Phasmid : 1,
		 
		 Female_Grasshopper : 1,
		 Male_Grasshopper : 1,
		 Female_Stag_Beetle : 1,
		 Male_Stag_Beetle : 1,
		 Female_Butterfly : 1,
		 Male_Butterfly : 1,
		 Female_Beetle : 1,
		 Male_Beetle : 1,
		 
		 NullItem : 1,
		 Coro_Key : 1,
		 Big_Key_Goron_Mines : 1,
		 Null_FC : 1,
		 Key_Shard_3 : 1,
		 Key_Shard_2 : 1,
		 Key_Shard_1 : 1,
		 Got_Lantern_Back : 1,
		 
		 Null_F7 : 1,
		 Bed_Key : 1,
		 Ordon_Goat_Cheese : 1,
		 Ordon_Pumpkin : 1,
		 Gate_Keys : 1,
		 Null_F2 : 1,
		 Null_F1 : 1,
		 Null_F0 : 1,
		 
		 Null_EF : 1,
		 Small_Key_N_Faron_Gate : 1,
		 Purple_Rupee_Links_house : 1,
		 Ooccoo_CitS : 1,
		 Ancient_Sky_Book_completed : 1,
		 Ancient_Sky_Book_partly_filled : 1,
		 Ancient_Sky_Book_empty : 1,
		 Null_E8 : 1,
		 
		 Null_E7 : 1,
		 Null_E6 : 1,
		 Null_E5 : 1,
		 Null_E4 : 1,
		 Null_E3 : 1,
		 Null_E2 : 1,
		 Null_E1 : 1,
		 Poe_Soul : 1;
		 
	} __attribute__((__packed__));
	
	struct ItemFlags
	{
		ItemFlags1 itemFlags1;
		ItemFlags2 itemFlags2;
		ItemFlags3 itemFlags3;
		ItemFlags4 itemFlags4;	
		
	} __attribute__((__packed__));
	
	
	// Should try to fill in the variables at some point
	struct ScratchPad
	{
		u8 wQuestLogData_1[0x9C];
		ItemSlots itemWeel;//length:0x17 offset 0x9C
		u8 itemSlots[0x19];
		ItemFlags itemFlags;
		u8 wQuestLogData_2[0x704];//offset 0xEC
		u8 eventBits[0x150]; // Bitfield (QuestLogOffset - 7F0)
		u8 miniGameBits[0x18]; // Bitfield
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct CurrentStageVars
	{
		char currentStage[8];
		s16 currentSpawnPoint;
		u8 unknown[4];
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct NextStageVars
	{
		char nextStage[8];
		u8 isVoidorWarp;
		u8 nextSpawnPoint;
		u8 nextRoom;
		u8 nextState;
		u8 unk1;
		u8 unk2;
		u8 triggerLoad;
		u8 fadeType;
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	
	struct EventSystem
	{
		u8 unk00[0x13]; // 4ec8 - 4eda
		u16 immediateControl; // 4edb - 4edc
		u8 unk_00[0x4]; // 4edd - 4EE0
		u8 nextEventID; // 4EE1
		u8 unk1A[0xD1];
		u8 currentEventID;
		u8 unk_ec[0x14];
		tp::evt_control::csFunc* onSkip;		 // if != 0 the function stored here will run, otherwise unskippable
		u8 unk_104[0xC];
		u32 fadeOnSkip; // if != 0 this will fade out the screen when skipping
	} __attribute__((__packed__));

	// Should try to fill in the missing variables at some point
	// Need to also get the exact size of this struct
	struct LinkMapVars
	{
		u8 unk_0[0x4D0];
		float pos[3];
	} __attribute__((__packed__));
	
	struct DungeonFlags
	{
		u8 miniBossBeaten : 1,
		 ooccooGotten : 1,
		 bitSix : 1,
		 containerGotten : 1,
		 bossBeaten : 1,
		 bigKeyGotten : 1,
		 compassGotten : 1,
		 mapGotten : 1;
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	struct AreaNodes
	{
		u8 unk_0[0x1C];
		u8 nbKeys;
		DungeonFlags dungeon;
		u8 unk_1E[0x2];
	} __attribute__((__packed__));
	
	// Should try to fill in the missing variables at some point
	struct GameInfo
	{
		ScratchPad scratchPad; // 0 - 957
		AreaNodes localAreaNodes; // 958 - 977 holds flags about the current area
        u8 unk_978[0x450]; // 978 - DC7
		u8 respawnCutscene; // DC8 - DC8
		u8 unkdc9[0xA]; // dc9 - dd2
		u8 respawnAnimation; // dd3 - dd3
		u8 unkdd4[0x402C]; // dd4 - 4DFF
		char currentStage[8];  // 4E00 - 4E07
		u8 unk_4e08[6];  // 4E08 - 4E0D
		NextStageVars nextStageVars; // 4E0E - 4E1b
		u8 unk_4e1c[0xAA]; // 4E19 - 4EC7
		EventSystem eventSystem; // 4EC8 - 4FDE
		u8 unk_4fdd[0xDD0]; // 4FDD - 5DBF
		LinkMapVars* linkMapPtr; // 5DA0 - 5DAB
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));

	static_assert(sizeof(ScratchPad) == 0x958);
	static_assert(sizeof(GameInfo) == 0x1DE10);
	static_assert(sizeof(DungeonFlags) == 0x1);
	
	

	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
	}
}