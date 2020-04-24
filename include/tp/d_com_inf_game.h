#pragma once

#include "defines.h"

#include <tp/evt_control.h>

namespace tp::d_com_inf_game
{	
	struct Counters
	{
		u16 max_health;
		u16 health;
		u16 rupees;
		u16 max_lantern_oil;//constant
		u16 lantern_oil;//divide by max to get %
		
	} __attribute__((__packed__));

	struct EquipedItems
	{
		u8 unk;
		u8 item_X;
		u8 item_Y;
		u8 item_unk3;
		u8 item_unk4;
		u8 item_combo_X;//bow goes here, bombs/hawkeye go in main slot
		u8 item_combo_Y;//bow goes here, bombs/hawkeye go in main slot
		u8 item_combo_unk3;//bow goes here, bombs/hawkeye go in main slot
		u8 item_combo_unk4;//bow goes here, bombs/hawkeye go in main slot
		u8 clothes;
		u8 sword;
		u8 shield;
		u8 scent;
		
	} __attribute__((__packed__));
	
	struct Epona
	{
		float position_X;
		float position_Y;
		float position_Z;
		u16 angle;
		char stage[8];
		u8 spawn;//unused
		u8 room;
	} __attribute__((__packed__));//size 0x18
	
	struct Link
	{
		char stage[8];
		u8 spawn;
		u8 room;
		u8 unk_62;
		u8 unk_63;
		float previous_position_X;
		float previous_position_Y;
		float previous_position_Z;
		u16 previous_angle;
		char previous_stage[8];
		
	} __attribute__((__packed__));//size 0x22
	
	struct ExploredRegions
	{
		u8 unk_0 : 1,
		 Snowpeak : 1,
		 Desert : 1,
		 Lanayru : 1,
		 Eldin : 1,
		 Faron : 1,
		 Ordon : 1,
		 unk_8 : 1;
		
	} __attribute__((__packed__));
	
	struct Ooccoo
	{
		float position_X;
		float position_Y;
		float position_Z;
		u16 angle;
		char stage[8];// 7th byte will be 37 by default, rest 00. No clue why
		u8 spawn;//unused
		u8 room;
		u8 dungeon;
		
	} __attribute__((__packed__));//size 0x19
	
	struct MovingActors
	{
		Epona epona;
		Link link;
		u8 unk_7A[0x3];
		ExploredRegions exploredRegions;
		u8 unk_7E[0x2];
		Ooccoo ooccoo;
		
	} __attribute__((__packed__));//size 0x59
	
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
		 
		u64 Null_DF : 1,//now used for sky letter 1
		 Null_DE : 1,//now used for sky letter 2
		 Null_DD : 1,//now used for sky letter 3
		 Null_DC : 1,//now used for sky letter 4
		 Null_DB : 1,//now used for sky letter 5
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
	
	struct Ammo
	{
		u8 arrows;
		u8 bombs1;
		u8 bombs2;
		u8 bombs3;
		u8 unk_F0;
		u8 unk_F1;
		u8 unk_F2;
		u8 unk_F3;
		u8 seeds;
		u8 unk_F5;
		u8 unk_F6;
		u8 unk_F7;
		u8 max_arrows;//30,60,100
		u8 max_normal_bombs;//const
		u8 max_water_bombs;//const
		u8 max_bomblings;//const
		
	} __attribute__((__packed__));//size 0x10
	
	struct DungeonRewards
	{
		u16 unk_0 : 1,
		 unk_1 : 1,
		 unk_2 : 1,
		 unk_3 : 1,
		 Palace_of_Twilight : 1,
		 Lakebed_Temple : 1,
		 Goron_Mines : 1,
		 Forest_Temple : 1,
		 
		 unk_8 : 1,
		 unk_9 : 1,
		 unk_10 : 1,
		 unk_11 : 1,
		 City_in_the_Sky : 1,
		 Temple_of_Time : 1,
		 Snowpeak_Ruins : 1,
		 Arbiters_Grounds : 1;
		
	} __attribute__((__packed__));
	
	struct TearsOfLightCounters
	{
		u8 Faron;
		u8 Eldin;
		u8 Lanayru;
	
	} __attribute__((__packed__));
	
	struct RecivedLetters
	{
		u16 From_Wife_of_Yeto : 1,
		 Update : 1,
		 Now_open_for_business : 1,
		 Agithas_Dream : 1,
		 Hey_kid : 1,
		 Challenge_for_you : 1,
		 They_came_so_quickly : 1,
		 Heroes_come_together : 1,
		 URGENT_Bomb_arrows : 1,
		 Rare_item_in_stock : 1,
		 Good_stuff_inside : 1,
		 URGENT_NOTICE : 1,
		 Post_office_notice : 1,
		 Dear_Adventurer_1 : 1,
		 Dear_Adventurer_2 : 1,
		 About_Ilias_memory : 1;
		 
	} __attribute__((__packed__));
	
	struct ReadLetters
	{
		u16 From_Wife_of_Yeto : 1,
		 Update : 1,
		 Now_open_for_business : 1,
		 Agithas_Dream : 1,
		 Hey_kid : 1,
		 Challenge_for_you : 1,
		 They_came_so_quickly : 1,
		 Heroes_come_together : 1,
		 URGENT_Bomb_arrows : 1,
		 Rare_item_in_stock : 1,
		 Good_stuff_inside : 1,
		 URGENT_NOTICE : 1,
		 Post_office_notice : 1,
		 Dear_Adventurer_1 : 1,
		 Dear_Adventurer_2 : 1,
		 About_Ilias_memory : 1;
		 
	} __attribute__((__packed__));
	
	
	struct NumberOfFish
	{
		u16 Hyrule_Bass;
		u16 Hylian_Loach;
		u16 Hylian_Pike;
		u16 Ordon_Catfish;
		u16 Reekfish;
		u16 Greengill;
	} __attribute__((__packed__));
	
	struct LargestFishes
	{
		u8 Hyrule_Bass;
		u8 Hylian_Loach;
		u8 Hylian_Pike;
		u8 Ordon_Catfish;
		u8 Reekfish;
		u8 Greengill;
	} __attribute__((__packed__));
	
	struct FishingJournal
	{
		NumberOfFish numbers;//size 0xC
		u8 unk_E[0x14];//offset 0xC
		LargestFishes largest;//size 0x6 offset 0x20
		
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
	struct AllAreaNodes
	{
		AreaNodes Ordon;//00
		AreaNodes Sewers;//20
		AreaNodes Faron;//40
		AreaNodes Eldin;//60
		AreaNodes Lanyru;//80
		AreaNodes Null_5;//A0
		AreaNodes Hyrule_Field;//C0
		AreaNodes Sacred_Grove;//E0
		AreaNodes Snowpeak;//100
		AreaNodes Castle_Town;//120
		AreaNodes Gerudo_Desert;//140
		AreaNodes Fishing_Pond;//160
		AreaNodes Null_C;
		AreaNodes Null_D;
		AreaNodes Null_E;
		AreaNodes Null_F;
		AreaNodes Forest_Temple;
		AreaNodes Goron_Mines;
		AreaNodes Lakebed_Temple;
		AreaNodes Arbiters_Grounds;
		AreaNodes Snowpeak_Ruins;
		AreaNodes Temple_of_Time;
		AreaNodes City_in_the_Sky;
		AreaNodes Palace_of_Twilight;
		AreaNodes Hyrule_Castle;
		AreaNodes Cave_of_Ordeals;//includes Gorge Cave and Ice Block Cave
		AreaNodes Lake_Hylia_Cave;//includes Goron Stockcave
		AreaNodes Grottos;
		AreaNodes Null_1C;
		AreaNodes Null_1D;
		AreaNodes Null_1E;
		AreaNodes Null_1F;
	} __attribute__((__packed__));
	
	struct StageRooms
	{
		u64 room_31 : 1,
		 room_30 : 1,
		 room_29 : 1,
		 room_28 : 1,
		 room_27 : 1,
		 room_26 : 1,
		 room_25 : 1,
		 room_24 : 1,
		 
		 room_23 : 1,
		 room_22 : 1,
		 room_21 : 1,
		 room_20 : 1,
		 room_19 : 1,
		 room_18 : 1,
		 room_17 : 1,
		 room_16 : 1,
		 
		 room_15 : 1,
		 room_14 : 1,
		 room_13 : 1,
		 room_12 : 1,
		 room_11 : 1,
		 room_10 : 1,
		 room_09 : 1,
		 room_08 : 1,
		 
		 room_07 : 1,
		 room_06 : 1,
		 room_05 : 1,
		 room_04 : 1,
		 room_03 : 1,
		 room_02 : 1,
		 room_01 : 1,
		 room_00 : 1,
		 
		 room_63 : 1,
		 room_62 : 1,
		 room_61 : 1,
		 room_60 : 1,
		 room_59 : 1,
		 room_58 : 1,
		 room_57 : 1,
		 room_56 : 1,
		 
		 room_55 : 1,
		 room_54 : 1,
		 room_53 : 1,
		 room_52 : 1,
		 room_51 : 1,
		 room_50 : 1,
		 room_49 : 1,
		 room_48 : 1,
		 
		 room_47 : 1,
		 room_46 : 1,
		 room_45 : 1,
		 room_44 : 1,
		 room_43 : 1,
		 room_42 : 1,
		 room_41 : 1,
		 room_40 : 1,
		 
		 room_39 : 1,
		 room_38 : 1,
		 room_37 : 1,
		 room_36 : 1,
		 room_35 : 1,
		 room_34 : 1,
		 room_33 : 1,
		 room_32 : 1;
		 
	} __attribute__((__packed__));
	
	struct ExploredStagesMap
	{
		StageRooms F_SP00;
		StageRooms F_SP103;
		StageRooms F_SP104;
		StageRooms R_SP01;
		StageRooms unk_4;
		StageRooms R_SP107;
		StageRooms F_SP108;
		StageRooms R_SP108;
		StageRooms F_SP117;
		StageRooms F_SP109;
		StageRooms F_SP110;
		StageRooms F_SP111;
		StageRooms R_SP109_R_SP209;
		StageRooms F_SP112_F_SP126;
		StageRooms F_SP113;
		StageRooms F_SP114;
		StageRooms F_SP115;
		StageRooms F_SP116;
		StageRooms F_SP118_F_SP124_F_SP125;
		StageRooms unk_19;
		StageRooms unk_20;
		StageRooms F_SP121_F_SP122;
		StageRooms D_MN05;
		StageRooms D_MN04;
		StageRooms D_MN01;
		StageRooms D_MN10;
		StageRooms D_MN11;
		StageRooms D_MN06;
		StageRooms D_MN07;
		StageRooms D_MN08;
		StageRooms D_MN09;
		StageRooms R_SP110;
		StageRooms R_SP116_R_SP161;
		StageRooms F_SP127;
		StageRooms F_SP128;
		StageRooms R_SP128;
		StageRooms R_SP300;
		StageRooms R_SP301;
		StageRooms F_SP200;
		StageRooms R_SP127;
		StageRooms D_SB00;
		StageRooms D_SB01;
		StageRooms D_SB02;
		StageRooms D_SB03;
		StageRooms D_SB04;
		StageRooms D_SB05_6_7_8_9;
		StageRooms unk_46;
		StageRooms unk_47;
		StageRooms unk_48;
		StageRooms unk_49;
		StageRooms D_SB10;
		StageRooms unk_51;
		StageRooms unk_52;
		StageRooms unk_53;
		StageRooms unk_54;
		StageRooms unk_55;
		StageRooms unk_56;
		StageRooms unk_57;
		StageRooms unk_58;
		StageRooms unk_59;
		StageRooms unk_60;
		StageRooms unk_61;
		StageRooms unk_62;
		StageRooms unk_63;
		 
	} __attribute__((__packed__));
	
	// Should try to fill in the variables at some point
	struct ScratchPad
	{
		Counters counters;//size: 0xA offset: 0x0
		EquipedItems equipedItems;//size: 0xD offset: 0xA
		u8 unk_17[0x1D];//offset 0x17
		float skyAngle;//offset: 0x34
		u8 unk_38[0x8];//offset: 0x38
		MovingActors movingActors;//size:0x59 offset: 0x40
		u8 unk_99[0x3];//offset: 0x99
		ItemSlots itemWheel;//length:0x17 offset 0x9C
		u8 itemSlots[0x19];//offset 0xB3
		ItemFlags itemFlags;//lenght:0x20 offset 0xCC
		Ammo ammo;//size:0x10 offset: 0xEC
		u8 unk_FC[0xC];//offset 0xFC
		DungeonRewards dungeonRewards;//size: 0x2 offset: 0x108
		u8 unk_10A;
		u8 poeCount;
		u8 unk_10C[0x9];//offset 0x10C
		TearsOfLightCounters tearCounters;//size 0x3 offset: 0x115
		u8 unk_118[0x6];//offset: 118
		RecivedLetters lettersRecived;//size: 0x2 offset: 0x11E
		u8 unk_120[0x6];//offset: 0x120
		ReadLetters lettersRead;//size: 0x2 offset: 0x126
		u8 unk_128[0x44];//offset 0x128
		FishingJournal fishJournal; //size: 0x26 offset: 0x16C
		u8 unk_192[0x5E];//offset 0x192
		AllAreaNodes allAreaNodes;//offset 0x1F0
		ExploredStagesMap exploredMap;//size: 200 offset 0x5F0
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
		NextStageVars nextStageVars; // 4E0E - 4E1D
		u8 unk_4e1c[0xAA]; // 4E19 - 4EC7
		EventSystem eventSystem; // 4EC8 - 4FDE
		u8 unk_4fdd[0xDD0]; // 4FDD - 5DBF
		LinkMapVars* linkMapPtr; // 5DA0 - 5DAB
		u8 unk_5dac[0x18060];
	} __attribute__((__packed__));

	static_assert(sizeof(AreaNodes) == 0x20);
	static_assert(sizeof(ScratchPad) == 0x958);
	static_assert(sizeof(GameInfo) == 0x1DE10);
	static_assert(sizeof(DungeonFlags) == 0x1);
	static_assert(sizeof(FishingJournal) == 0x26);
	static_assert(sizeof(ExploredStagesMap) == 0x200);
	static_assert(sizeof(Epona) == 0x18);
	static_assert(sizeof(Link) == 0x22);
	static_assert(sizeof(Ooccoo) == 0x19);
	static_assert(sizeof(MovingActors) == 0x59);
	static_assert(sizeof(Ammo) == 0x10);
	static_assert(sizeof(DungeonRewards) == 0x2);
	
	extern "C"
	{
		extern GameInfo dComIfG_gameInfo;
		extern u8 current_state;
	}
}