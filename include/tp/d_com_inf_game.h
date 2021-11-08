#pragma once

#include <tp/evt_control.h>

#include "defines.h"

namespace tp::d_com_inf_game
{
    enum class ItemFlagBits : u32
    {
        Recovery_Heart = 0,
        Green_Rupee = 1,
        Blue_Rupee = 2,
        Yellow_Rupee = 3,
        Red_Rupee = 4,
        Purple_Rupee = 5,
        Orange_Rupee = 6,
        Silver_Rupee = 7,

        Null_8 = 8,
        Null_9 = 9,
        Bombs_5 = 10,
        Bombs_10 = 11,
        Bombs_20 = 12,
        Bombs_30 = 13,
        Arrows_10 = 14,
        Arrows_20 = 15,

        Arrows_30 = 16,
        Arrows_1 = 17,
        Seeds_50 = 18,
        Null_13 = 19,
        Null_14 = 20,
        Null_15 = 21,
        Water_Bombs_5 = 22,
        Water_Bombs_10 = 23,

        Water_Bombs_15 = 24,
        Water_Bombs_3 = 25,
        Bomblings_5 = 26,
        Bomblings_10 = 27,
        Bomblings_3 = 28,
        Bombling_1 = 29,
        Fairy = 30,
        Recovery_Heart_x3 = 31,

        Small_Key = 32,
        Piece_of_Heart = 33,
        Heart_Container = 34,
        Dungeon_Map = 35,
        Compass = 36,
        Ooccoo_FT = 37,
        Big_Key = 38,
        Ooccoo_Jr = 39,     // never set in normal game

        Ordon_Sword = 40,
        Master_Sword = 41,
        Ordon_Shield = 42,
        Wooden_Shield = 43,
        Hylian_Shield = 44,
        Ooccoos_Note = 45,       // never set in normal game
        Ordon_Clothing = 46,     // never set in normal game
        Heros_Clothes = 47,

        Magic_Armor = 48,
        Zora_Armor = 49,
        Shadow_Crystal = 50,     // never set in normal game
        Ooccoo_Dungeon = 51,
        Small_Wallet = 52,     // never set in normal game
        Big_Wallet = 53,
        Giant_Wallet = 54,
        Null_37 = 55,

        Null_38 = 56,
        Null_39 = 57,
        Null_3A = 58,
        Null_3B = 59,
        Null_3C = 60,
        Coral_Earring = 61,
        Hawkeye = 62,
        Wooden_Sword = 63,

        Boomerang = 64,
        Spinner = 65,
        Ball_and_Chain = 66,
        Heros_Bow = 67,
        Clawshot = 68,
        Iron_Boots = 69,
        Dominion_Rod = 70,
        Clawshots = 71,

        Lantern = 72,
        Master_Sword_Light = 73,
        Fishing_Rod = 74,
        Slingshot = 75,
        Dominion_Rod_Uncharged = 76,     // never set in normal game
        Null_4D = 77,
        Null_4E = 78,
        Giant_Bomb_Bag = 79,

        Empty_Bomb_Bag = 80,     // never set in normal game
        Goron_Bomb_Bag = 81,
        Null_52 = 82,
        Null_53 = 83,
        Small_Quiver = 84,     // never set in normal game
        Big_Quiver = 85,
        Giant_Quiver = 86,
        Null_57 = 87,

        Fishing_Rod_Lure = 88,
        Bow_Bombs = 89,
        Bow_Hawkeye = 90,
        Fishing_Rod_Bee_Larva = 91,
        Fishing_Rod_Coral_Earring = 92,
        Fishing_Rod_Worm = 93,
        Fishing_Rod_Earring_Bee_Larva = 94,
        Fishing_Rod_Earring_Worm = 95,

        Empty_Bottle = 96,
        Red_Potion_Shop = 97,
        Green_Potion = 98,
        Blue_Potion = 99,
        Milk = 100,
        Sera_Bottle = 101,
        Lantern_Oil_Shop = 102,
        Water = 103,

        Lantern_Oil_Scooped = 104,
        Red_Potion_Scooped = 105,
        Nasty_soup = 106,
        Hot_spring_water_Scooped = 107,
        Fairy_Bottle = 108,
        Hot_Spring_Water_Shop = 109,
        Lantern_Refill_Scooped = 110,
        Lantern_Refill_Shop = 111,

        Bomb_Bag_Regular_Bombs = 112,
        Bomb_Bag_Water_Bombs = 113,
        Bomb_Bag_Bomblings = 114,
        Fairy_Tears = 115,
        Worm = 116,
        Jovani_Bottle = 117,
        Bee_Larva_Scooped = 118,
        Rare_Chu_Jelly = 119,

        Red_Chu_Jelly = 120,
        Blue_Chu_Jelly = 121,
        Green_Chu_Jelly = 122,
        Yellow_Chu_Jelly = 123,
        Purple_Chu_Jelly = 124,
        Simple_Soup = 125,
        Good_Soup = 126,
        Superb_Soup = 127,

        Renardos_Letter = 128,
        Invoice = 129,
        Wooden_Statue = 130,
        Ilias_Charm = 131,
        Horse_Call = 132,
        Null_85 = 133,
        Null_86 = 134,
        Null_87 = 135,

        Null_88 = 136,
        Null_89 = 137,
        Null_8A = 138,
        Null_8B = 139,
        Null_8C = 140,
        Null_8D = 141,
        Null_8E = 142,
        Null_8F = 143,

        Aurus_Memo = 144,
        Asheis_Sketch = 145,
        Null_92 = 146,
        Null_93 = 147,
        Null_94 = 148,
        Null_95 = 149,
        Null_96 = 150,
        Null_97 = 151,

        Null_98 = 152,
        Null_99 = 153,
        Null_9A = 154,
        Null_9B = 155,
        Lantern_Yellow_Chu_Chu = 156,
        Coro_Bottle = 157,
        Bee_Larva_Shop = 158,
        Black_Chu_Jelly = 159,

        Tear_Of_Light = 160,     // never set in normal game
        Vessel_Of_Light_Faron = 161,
        Vessel_Of_Light_Eldin = 162,
        Vessel_Of_Light_Lanayru = 163,
        Vessel_Of_Light_Full = 164,     // never set in normal game
        Null_A5 = 165,
        Null_A6 = 166,
        Null_A7 = 167,

        Null_A8 = 168,
        Null_A9 = 169,
        Null_AA = 170,
        Null_AB = 171,
        Null_AC = 172,
        Null_AD = 173,
        Null_AE = 174,
        Null_AF = 175,

        Ilias_Scent = 176,
        Null_B1 = 177,
        Poe_Scent = 178,
        Reekfish_Scent = 179,
        Youths_Scent = 180,
        Medicine_Scent = 181,
        Null_B6 = 182,
        Null_B7 = 183,

        Null_B8 = 184,
        Null_B9 = 185,
        Null_BA = 186,
        Null_BB = 187,
        Null_BC = 188,
        Null_BD = 189,
        Null_BE = 190,
        Null_BF = 191,

        Male_Beetle = 192,
        Female_Beetle = 193,
        Male_Butterfly = 194,
        Female_Butterfly = 195,
        Male_Stag_Beetle = 196,
        Female_Stag_Beetle = 197,
        Male_Grasshopper = 198,
        Female_Grasshopper = 199,

        Male_Phasmid = 200,
        Female_Phasmid = 201,
        Male_Pill_Bug = 202,
        Female_Pill_Bug = 203,
        Male_Mantis = 204,
        Female_Mantis = 205,
        Male_Ladybug = 206,
        Female_Ladybug = 207,

        Male_Snail = 208,
        Female_Snail = 209,
        Male_Dragonfly = 210,
        Female_Dragonfly = 211,
        Male_Ant = 212,
        Female_Ant = 213,
        Male_Dayfly = 214,
        Female_Dayfly = 215,

        Null_D8 = 216,     // now used for hawkeye check
        Null_D9 = 217,     // now used for malo hylian shield check
        Null_DA = 218,
        Null_DB = 219,     // now used for sky letter 5
        Null_DC = 220,     // now used for sky letter 4
        Null_DD = 221,     // now used for sky letter 3
        Null_DE = 222,     // now used for sky letter 2
        Null_DF = 223,     // now used for sky letter 1

        Poe_Soul = 224,
        Null_E1 = 225,     // now used for ending blow
        Null_E2 = 226,     // now used for shield bash
        Null_E3 = 227,     // now used for back slice
        Null_E4 = 228,     // now used for helm splitter
        Null_E5 = 229,     // now used for mortal draw
        Null_E6 = 230,     // now used for jump strike
        Null_E7 = 231,     // now used for great spin

        Null_E8 = 232,
        Ancient_Sky_Book_empty = 233,
        Ancient_Sky_Book_partly_filled = 234,
        Ancient_Sky_Book_completed = 235,
        Ooccoo_CitS = 236,
        Purple_Rupee_Links_house = 237,
        Small_Key_N_Faron_Gate = 238,
        Null_EF = 239,

        Null_F0 = 240,
        Null_F1 = 241,
        Null_F2 = 242,
        Gate_Keys = 243,
        Ordon_Pumpkin = 244,
        Ordon_Goat_Cheese = 245,
        Bed_Key = 246,
        Null_F7 = 247,

        Got_Lantern_Back = 248,
        Key_Shard_1 = 249,
        Key_Shard_2 = 250,
        Key_Shard_3 = 251,
        Null_FC = 252,
        Big_Key_Goron_Mines = 253,
        Coro_Key = 254,
        NullItem = 255,
    };

    struct Counters
    {
        u16 max_health;
        u16 health;
        u16 rupees;
        u16 max_lantern_oil;     // constant
        u16 lantern_oil;         // divide by max to get %

    } __attribute__( ( __packed__ ) );

    struct EquipedItems
    {
        u8 unk;
        u8 item_X;
        u8 item_Y;
        u8 item_unk3;
        u8 item_unk4;
        u8 item_combo_X;        // bow goes here, bombs/hawkeye go in main slot
        u8 item_combo_Y;        // bow goes here, bombs/hawkeye go in main slot
        u8 item_combo_unk3;     // bow goes here, bombs/hawkeye go in main slot
        u8 item_combo_unk4;     // bow goes here, bombs/hawkeye go in main slot
        u8 clothes;
        u8 sword;
        u8 shield;
        u8 scent;

    } __attribute__( ( __packed__ ) );

    struct ClearedTwilights
    {
        u8 unk_0 : 1, unk_1 : 1, unk_2 : 1, unk_3 : 1, unk_4 : 1, Lanayru : 1, Eldin : 1, Faron : 1;

    } __attribute__( ( __packed__ ) );

    struct Epona
    {
        float position_X;
        float position_Y;
        float position_Z;
        u16 angle;
        char stage[8];
        u8 spawn;     // unused
        u8 room;
    } __attribute__( ( __packed__ ) );     // size 0x18

    struct Link
    {
        char stage[8];
        u8 spawn;
        u8 room;
        u8 unk_62[0x2];
        float previous_position_X;
        float previous_position_Y;
        float previous_position_Z;
        u16 previous_angle;
        char previous_stage[8];

    } __attribute__( ( __packed__ ) );     // size 0x22

    struct ExploredRegions
    {
        u8 unk_0 : 1, Snowpeak : 1, Desert : 1, Lanayru : 1, Eldin : 1, Faron : 1, Ordon : 1, unk_8 : 1;

    } __attribute__( ( __packed__ ) );

    struct Ooccoo
    {
        float position_X;
        float position_Y;
        float position_Z;
        u16 angle;
        char stage[8];     // 7th byte will be 37 by default, rest 00. No clue why
        u8 spawn;          // unused
        u8 room;
        u8 dungeon;

    } __attribute__( ( __packed__ ) );     // size 0x19

    struct MovingActors
    {
        Epona epona;
        Link link;
        u8 unk_7A[0x3];
        ExploredRegions exploredRegions;
        u8 unk_7E[0x2];
        Ooccoo ooccoo;

    } __attribute__( ( __packed__ ) );     // size 0x59

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
        u8 Story;
        u8 Fishing_Rod;
        u8 Horse_Call;
        u8 Sky_Book;
        u8 Slingshot;
    } __attribute__( ( __packed__ ) );

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
        u8 max_arrows;           // 30,60,100
        u8 max_normal_bombs;     // const (giant bomb bag just does this value x2)
        u8 max_water_bombs;      // const (giant bomb bag just does this value x2)
        u8 max_bomblings;        // const (giant bomb bag just does this value x2)

    } __attribute__( ( __packed__ ) );     // size 0x10

    struct DungeonRewards
    {
        u16 unk_0 : 1, unk_1 : 1, unk_2 : 1, unk_3 : 1, Palace_of_Twilight : 1, Lakebed_Temple : 1, Goron_Mines : 1,
            Forest_Temple : 1,

            unk_8 : 1, unk_9 : 1, unk_10 : 1, unk_11 : 1, City_in_the_Sky : 1, Temple_of_Time : 1, Snowpeak_Ruins : 1,
            Arbiters_Grounds : 1;

    } __attribute__( ( __packed__ ) );

    struct TearsOfLightCounters
    {
        u8 Faron;
        u8 Eldin;
        u8 Lanayru;

    } __attribute__( ( __packed__ ) );

    struct Letters
    {
        u16 From_Wife_of_Yeto : 1, Update : 1, Now_open_for_business : 1, Agithas_Dream : 1, Hey_kid : 1, Challenge_for_you : 1,
            They_came_so_quickly : 1, Heroes_come_together : 1,

            URGENT_Bomb_arrows : 1, Rare_item_in_stock : 1, Good_stuff_inside : 1, URGENT_NOTICE : 1, Post_office_notice : 1,
            Dear_Adventurer_1 : 1, Dear_Adventurer_2 : 1, About_Ilias_memory : 1;

    } __attribute__( ( __packed__ ) );

    struct NumberOfFish
    {
        u16 Hyrule_Bass;
        u16 Hylian_Loach;
        u16 Hylian_Pike;
        u16 Ordon_Catfish;
        u16 Reekfish;
        u16 Greengill;
    } __attribute__( ( __packed__ ) );

    struct LargestFishes
    {
        u8 Hyrule_Bass;
        u8 Hylian_Loach;
        u8 Hylian_Pike;
        u8 Ordon_Catfish;
        u8 Reekfish;
        u8 Greengill;
    } __attribute__( ( __packed__ ) );

    struct FishingJournal
    {
        NumberOfFish numbers;      // size 0xC
        u8 unk_178[0x14];          // offset 0xC
        LargestFishes largest;     // size 0x6 offset 0x20

    } __attribute__( ( __packed__ ) );

    struct Settings
    {
        u8 sound;              // Mono=0 / Stereo = 1 / Surround = 2
        u8 targettingType;     // Hold = 0 / Switch = 1
        u8 rumble;             // Off = 0 / On = 1

    } __attribute__( ( __packed__ ) );

    struct DungeonFlags
    {
        u8 miniBossBeaten : 1, ooccooGotten : 1, bitSix : 1, containerGotten : 1, bossBeaten : 1, bigKeyGotten : 1,
            compassGotten : 1, mapGotten : 1;
    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point
    struct AreaNodes
    {
        u8 unk_0[0x1C];
        u8 nbKeys;
        DungeonFlags dungeon;
        u8 unk_1E[0x2];
    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point
    struct AllAreaNodes
    {
        AreaNodes Ordon;             // 00
        AreaNodes Sewers;            // 20
        AreaNodes Faron;             // 40
        AreaNodes Eldin;             // 60
        AreaNodes Lanayru;           // 80
        AreaNodes Null_5;            // A0
        AreaNodes Hyrule_Field;      // C0
        AreaNodes Sacred_Grove;      // E0
        AreaNodes Snowpeak;          // 100
        AreaNodes Castle_Town;       // 120
        AreaNodes Gerudo_Desert;     // 140
        AreaNodes Fishing_Pond;      // 160
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
        AreaNodes Cave_of_Ordeals;     // includes Gorge Cave and Ice Block Cave
        AreaNodes Lake_Hylia_Cave;     // includes Goron Stockcave
        AreaNodes Grottos;
        AreaNodes Null_1C;
        AreaNodes Null_1D;
        AreaNodes Null_1E;
        AreaNodes Null_1F;
    } __attribute__( ( __packed__ ) );

    struct StageRooms
    {
        u64 room_31 : 1, room_30 : 1, room_29 : 1, room_28 : 1, room_27 : 1, room_26 : 1, room_25 : 1, room_24 : 1,

            room_23 : 1, room_22 : 1, room_21 : 1, room_20 : 1, room_19 : 1, room_18 : 1, room_17 : 1, room_16 : 1,

            room_15 : 1, room_14 : 1, room_13 : 1, room_12 : 1, room_11 : 1, room_10 : 1, room_09 : 1, room_08 : 1,

            room_07 : 1, room_06 : 1, room_05 : 1, room_04 : 1, room_03 : 1, room_02 : 1, room_01 : 1, room_00 : 1,

            room_63 : 1, room_62 : 1, room_61 : 1, room_60 : 1, room_59 : 1, room_58 : 1, room_57 : 1, room_56 : 1,

            room_55 : 1, room_54 : 1, room_53 : 1, room_52 : 1, room_51 : 1, room_50 : 1, room_49 : 1, room_48 : 1,

            room_47 : 1, room_46 : 1, room_45 : 1, room_44 : 1, room_43 : 1, room_42 : 1, room_41 : 1, room_40 : 1,

            room_39 : 1, room_38 : 1, room_37 : 1, room_36 : 1, room_35 : 1, room_34 : 1, room_33 : 1, room_32 : 1;

    } __attribute__( ( __packed__ ) );

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

    } __attribute__( ( __packed__ ) );

    // Should try to fill in the variables at some point
    struct ScratchPad
    {
        Counters counters;                     // size: 0xA offset: 0x0
        EquipedItems equipedItems;             // size: 0xD offset: 0xA
        u8 unk_17[0x2];                        // offset: 0x17
        u8 equippedWallet;                     // offset: 0x19 (0 = small, 1 = big, 2 = giant)
        u8 unk_1A[0x4];                        // offset: 0x1A
        u8 form;                               // offset: 0x1E (0 = human, 1 = wolf)
        u8 unk_1F[0x12];                       // offset: 0x1F
        ClearedTwilights clearedTwilights;     // offset: 0x31
        u8 unk_32[0x2];                        // offset: 0x32
        float skyAngle;                        // offset: 0x34 Impacts time of day poes and towns
        u8 unk_38[0x8];                        // offset: 0x38
        MovingActors movingActors;             // size:0x59 offset: 0x40
        u8 unk_99[0x3];                        // offset: 0x99
        ItemSlots itemWheel;                   // size:0x18 offset 0x9C
        u8 itemSlotsOrder[0x18];               // offset 0xB4
        u32 itemFlags[8];                      // size:0x20 offset 0xCC
        Ammo ammo;                             // size:0x10 offset: 0xEC
        u8 unk_FC[0xD];                        // offset 0xFC
        DungeonRewards dungeonRewards;         // size: 0x2 offset: 0x109
        u8 unk_10B;
        u8 poeCount;                           // offset 0x10C
        u8 unk_10D[0x7];                       // offset 0x10D
        TearsOfLightCounters tearCounters;     // size 0x3 offset: 0x114
        u8 unk_115[0x7];                       // offset: 0x115
        Letters recivedLetters;                // size: 0x2 offset: 0x11E
        u8 unk_120[0x6];                       // offset: 0x120
        Letters readLetters;                   // size: 0x2 offset: 0x126
        u8 unk_128[0x44];                      // offset 0x128
        FishingJournal fishingJournal;         // size: 0x26 offset: 0x16C
        u8 unk_192[0x22];                      // offset 0x192
        char linkName[16];                     // offset 0x1B4
        u8 unk_1C4;
        char eponaName[16];                // offset 0x1C5
        u8 unk_1D5[0xC];                   // offset 0x1D5
        Settings optionsSettings;          // size: 0x3 offset 0x1E1
        u8 unk_1E4[0xC];                   // offset 1E4
        AllAreaNodes allAreaNodes;         // size: 0x400 offset 0x1F0
        ExploredStagesMap exploredMap;     // size: 0x200 offset 0x5F0
        u8 eventBits[0x150];               // Bitfield (QuestLogOffset - 7F0)
        u8 miniGameBits[0x18];             // Bitfield
    } __attribute__( ( __packed__ ) );

    struct RespawnValues
    {
        u8 room;                 // 0xDB4
        u8 unk_DB5[0x5];         // 0xDB5
        u8 spawnPoint;           // 0xDB9
        u16 angle;               // 0xDBA
        float position_X;        // 0xDBC
        float position_Y;        // 0xDC0
        float position_Z;        // 0xDC4
        u8 respawnCutscene;      // 0xDC8
        u8 unk_DC9[0x7];         // 0xDC9
        u8 lastItem;             // 0xDD0
        u8 voidedAsWolf;         // 0xDD1
        u8 unk_DD2;              // 0xDD2
        u8 respawnAnimation;     // 0xDD3

    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point
    // Need to also get the exact size of this struct
    struct CurrentStageVars
    {
        char currentStage[8];
        s16 currentSpawnPoint;
        u8 unknown[4];
    } __attribute__( ( __packed__ ) );

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
    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point

    struct EventSystem
    {
        u8 unk00[0x13];           // 4ec8 - 4eda
        u16 immediateControl;     // 4edb - 4edc
        u8 unk_00[0x4];           // 4edd - 4EE0
        u8 nextEventID;           // 4EE1
        u8 unk1A[0xC5];
        u8 actionStatus;
        u8 unkDF[0x5];
        u8 eventFlag;
        u8 unkE7[0x5];
        u8 currentEventID;
        u8 unk_ec[0x14];
        tp::evt_control::csFunc* onSkip;     // if != 0 the function stored here will run, otherwise unskippable
        u8 unk_104[0xC];
        u32 fadeOnSkip;     // if != 0 this will fade out the screen when skipping
    } __attribute__( ( __packed__ ) );

    // Should try to fill in the missing variables at some point
    // Need to also get the exact size of this struct
    struct LinkMapVars
    {
        u8 unk_0[0x4D0];
        float pos[3];
        u8 unk_4DC[0x94];
        u32 isTargeting;     // 570    at 0x400000 link is targeting
        u8 unk_574[0x2A68];
        u16 equippedItem;
    } __attribute__( ( __packed__ ) );

    struct TunicColor
    {
        u8 unk_0[0x32A0];
        u8 redTopToggle;     // 0x4 disables red
        u8 redTop;
        u8 greenTopToggle;     // 0x4 disables green
        u8 greenTop;
        u8 blueTopToggle;     // 0x4 disables blue
        u8 blueTop;
        u8 unk_32A6[0x2];
        u8 redBottomToggle;     // 0x4 disables red
        u8 redBottom;
        u8 greenBottomToggle;     // 0x4 disables green
        u8 greenBottom;
        u8 blueBottomToggle;     // 0x4 disables blue
        u8 blueBottom;
    } __attribute__( ( __packed__ ) );

    enum class AreaNodesID : u32
    {
        Ordon = 0,
        Sewers,
        Faron,
        Eldin,
        Lanayru,
        Unk_5,
        Hyrule_Field,
        Sacred_Grove,
        Snowpeak,
        Castle_Town,
        Gerudo_Desert,
        Fishing_Pond,
        Unk_C,
        Unk_D,
        Unk_E,
        Unk_F,
        Forest_Temple,
        Goron_Mines,
        Lakebed_Temple,
        Arbiters_Grounds,
        Snowpeak_Ruins,
        Temple_of_Time,
        City_in_the_Sky,
        Palace_of_Twilight,
        Hyrule_Castle,
        Cave_of_Ordeals,     // Includes Gorge Cave and Ice Block Cave
        Lake_Hylia_Cave,     // Includes Goron Stockcave
        Grottos,
        Unk_1C,
        Unk_1D,
        Unk_1E,
        Unk_1F,
    };

    // Should try to fill in the missing variables at some point
    struct GameInfo
    {
        ScratchPad scratchPad;           // 0 - 957
        AreaNodes localAreaNodes;        // 958 - 978 holds flags about the current area
        u8 unk_979[0x43B];               // 979 - DB3
        RespawnValues respawnValues;     // DB4 - DD3
        u8 unkdd4[0x402C];               // dd4 - 4DFF
        char currentStage[8];            // 4E00 - 4E07
        u8 unk_4e08;                     // 4E08
        u8 currentSpawnID;               // 4E09
        u8 unk_4e0A[4];                  // 4E0A - 4E0D
        NextStageVars nextStageVars;     // 4E0E - 4E1D
        u8 unk_4e1E[0xAA];               // 4E1E - 4EC7
        EventSystem eventSystem;         // 4EC8 - 4FDB
        u8 unk_4fdc[0xDD0];              // 4FDC - 5DAB
        LinkMapVars* linkMapPtr;         // 5DAC - 5DAF
        u8 unk_5db0[0x4];
        TunicColor* ColorPtr;     // 5DB4 - 5DB7
        u8 unk_5db8[0x2A];        // 5DB8 - 5DE1
        u16 airMeter;             // 5DE2 - 5DE3 max = 258
        u8 unk_5de4[0x38];        // 5DE4 - 5E1B
        u8 rButtonText;           // 5E1C - 51EC
        u8 bButtonText;           // 5E1D - 51ED
        u8 unk_5E1E[0x6];         // 5E1E - 5E23
        u8 aButtonText;           // 5E24 - 5E24
        u8 unk_5e25;              // 5E25 - 5E25
        u8 stickButtonText;       // 5E26 - 5E26
        u8 unk_5e27[0xB];         // 5E27 - 5E5B
        u8 startButtonText;       // 5E32 - 5E32
        u8 unk_5e33[0xC];         // 5E33 - 5E3E
        u8 cButtonText;           // 5E3F - 5E3F
        u8 unk_5e40[0x1C];        // 5E40 - 5E5B
        u8 xButtonText;           // 5E5C - 5E5C set to sense and doesn't unset
        u8 unk_5e5D[0x3];         // 5E5D - 5E5F
        u8 yButtonText;           // 5E60 - 5E60 set to dig and doesn't unset
        u8 unk_5e61[0xB];         // 5E61 - 5E6B
        u8 xItem;                 // 5E6C - 5E6C
        u8 yItem;                 // 5E6D - 5E6D
        u8 unk_5E6E[0x22];        // 5E6E - 5E8F
        u8 bottomPromptShown;     // 5E90 set to 0x8 when button promt apprears on bottom of screen
        u8 topPromptShown;        // 5E91 set to 0x8 when button promt apprears on top of screen
        u8 unk_5e92[0x25];        // 5E92 - 5EB6
        u8 airTimerDepleted;      // 5EB7 - 5EB7
        u8 unk_5eb8[0x17F58];

    } __attribute__( ( __packed__ ) );

    static_assert( sizeof( AreaNodes ) == 0x20 );
    static_assert( sizeof( ScratchPad ) == 0x958 );
    static_assert( sizeof( GameInfo ) == 0x1DE10 );
    static_assert( sizeof( DungeonFlags ) == 0x1 );
    static_assert( sizeof( FishingJournal ) == 0x26 );
    static_assert( sizeof( ExploredStagesMap ) == 0x200 );
    static_assert( sizeof( Epona ) == 0x18 );
    static_assert( sizeof( Link ) == 0x22 );
    static_assert( sizeof( Ooccoo ) == 0x19 );
    static_assert( sizeof( MovingActors ) == 0x59 );
    static_assert( sizeof( Ammo ) == 0x10 );
    static_assert( sizeof( RespawnValues ) == 0x21 );
    static_assert( sizeof( LinkMapVars* ) == 0x4 );
    static_assert( sizeof( EventSystem ) == 0x114 );
    static_assert( sizeof( NextStageVars ) == 0x10 );

    extern "C"
    {
        extern GameInfo dComIfG_gameInfo;
        extern u8 current_state;
        extern void setItemBombNumCount(
            u32 unk1,
            u8 bagNb,
            short amout );     // amount will be the og (ex: if you can only have 5 more bombs and buy 10, it'll still ahow 10)
        extern u8 next_state;
        // extern char wallet_description[96];
        extern u8 can_warp;
        int getLayerNo_common_common( const char* stageName, int roomId, int layerOverride );

        void dComIfGs_Wolf_Change_Check();
    }
}     // namespace tp::d_com_inf_game