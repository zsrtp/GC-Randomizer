#include "keyPlacement.h"

#include "defines.h"

namespace mod::keyPlacement
{
    u16 FT_keys[4] {
        9,       // Windless Bridge Chest
        11,      // totem key
        343,     // big baba
        18       // North Deku like chest
    };
    u16 FT_1[5] {
        6,       // entry
        8,       // map
        9,       // Windless Bridge Chest
        344,     // ooccoo
        17       // Water Cave Chest
    };
    u16 FT_2[5] {
        6,       // entry
        8,       // map
        9,       // Windless Bridge Chest
        344,     // ooccoo
        17       // Water Cave Chest
    };
    u16 FT_3[9] {
        6,       // entry
        8,       // map
        9,       // Windless Bridge Chest
        344,     // ooccoo
        17,      // Water Cave Chest
        13,      // PoH deku ike
        11,      // totem key
        12,      // Small Chest West Tile
        343      // big baba
    };
    u16 FT_4[9] {
        6,       // entry
        8,       // map
        9,       // Windless Bridge Chest
        344,     // ooccoo
        17,      // Water Cave Chest
        13,      // PoH deku ike
        11,      // totem key
        12,      // Small Chest West Tile
        343      // big baba
    };

    u16 GM_keys[3] {
        23,     // Hub Bottom Chest
        27,     // Chest under water
        30      // Outside Bemos Chest
    };
    u16 GM_1[2] {
        22,     // Entry Chest
        23      // Hub Bottom Chest
    };
    u16 GM_2[10] {
        22,      // Entry Chest
        23,      // Hub Bottom Chest
        24,      // map
        25,      // Gor Amato Small Chest
        354,     // key shard 1
        345,     // ooccoo
        26,      // POH Chest
        28,      // Small chest in water room
        27,      // Chest under water
        29       // POH Chest 2
    };
    u16 GM_3[12] {
        22,      // Entry Chest
        23,      // Hub Bottom Chest
        24,      // map
        25,      // Gor Amato Small Chest
        354,     // key shard 1
        345,     // ooccoo
        26,      // POH Chest
        28,      // Small chest in water room
        27,      // Chest under water
        29,      // POH Chest 2
        30,      // Outside Bemos Chest
        37       // Outside Underwater
    };

    u16 LBT_keys[3] {
        51,     // Water Bridge Chest
        53,     // 2nd Floor East SE Chest
        55      // Pre Deku Toad Chest
    };
    u16 LBT_1[7] {
        46,      // Entry  Chest 1
        47,      // Entry  Chest 2
        48,      // Stalactie Small Chest
        49,      // Hub Small Chest
        346,     // ooccoo
        50,      // map
        51       // Water Bridge Chest
    };
    u16 LBT_2[9] {
        46,      // Entry  Chest 1
        47,      // Entry  Chest 2
        48,      // Stalactie Small Chest
        49,      // Hub Small Chest
        346,     // ooccoo
        50,      // map
        51,      // Water Bridge Chest
        52,      // 2nd Floor East SW Chest
        53       // 2nd Floor East SE Chest
    };
    u16 LBT_3[11] {
        46,      // Entry  Chest 1
        47,      // Entry  Chest 2
        48,      // Stalactie Small Chest
        49,      // Hub Small Chest
        346,     // ooccoo
        50,      // map
        51,      // Water Bridge Chest
        52,      // 2nd Floor East SW Chest
        53,      // 2nd Floor East SE Chest
        54,      // East Water Small Chest
        55       // Pre Deku Toad Chest
    };

    u16 AG_keys[5] {
        143,     // Entry Chest
        144,     // East Redead Small Chest
        148,     // East Upper Redead Chest
        149,     // Ghoul Rat Small Chest
        154      // North Turn Room Chest
    };
    u16 AG_1[1] {
        143     // Entry Chest
    };
    u16 AG_2[7] {
        143,     // Entry Chest
        146,     // map
        145,     // Poe Hub POH
        405,     // poe 1
        144,     // East Redead Small Chest
        406,     // poe 2
        150      // West Small Chest
    };
    u16 AG_3[9] {
        143,     // Entry Chest
        146,     // map
        145,     // Poe Hub POH
        405,     // poe 1
        144,     // East Redead Small Chest
        406,     // poe 2
        150,     // West Small Chest
        147,     // compass
        148      // East Upper Redead Chest
    };
    u16 AG_4[11] {
        143,     // Entry Chest
        146,     // map
        145,     // Poe Hub POH
        405,     // poe 1
        144,     // East Redead Small Chest
        406,     // poe 2
        150,     // West Small Chest
        147,     // compass
        148,     // East Upper Redead Chest
        407,     // poe 3
        149      // Ghoul Rat Small Chest
    };
    u16 AG_5[16] {
        143,     // Entry Chest
        146,     // map
        145,     // Poe Hub POH
        405,     // poe 1
        144,     // East Redead Small Chest
        406,     // poe 2
        150,     // West Small Chest
        147,     // compass
        148,     // East Upper Redead Chest
        407,     // poe 3
        149,     // Ghoul Rat Small Chest
        151,     // West Chandelier Chest
        152,     // Stalfos Chest 1
        153,     // Stalfos Chest 2
        408,     // poe 4
        154      // North Turn Room Chest
    };

    u16 SPR_keys[6] {
        174,     // CY Partially Buried Chest
        173,     // pumpkin
        171,     // Buried Chest
        178,     // cheese
        180,     // Wooden Beam C. Chest
        184      // Chilfos Small Chest
    };
    u16 SPR_1[5] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174      // CY Partially Buried Chest
    };
    u16 SPR_2[6] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174,     // CY Partially Buried Chest
        173      // pumpkin
    };
    u16 SPR_3[10] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174,     // CY Partially Buried Chest
        173,     // pumpkin
        185,     // Storage Chest Right
        175,     // Small Beams Chest
        176,     // compass
        171      // Buried Chest
    };
    u16 SPR_4[13] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174,     // CY Partially Buried Chest
        173,     // pumpkin
        185,     // Storage Chest Right
        175,     // Small Beams Chest
        176,     // compass
        171,     // Buried Chest
        177,     // Courtyard South Chest
        290,     // BaC
        178      // cheese
    };
    u16 SPR_5[19] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174,     // CY Partially Buried Chest
        173,     // pumpkin
        185,     // Storage Chest Right
        175,     // Small Beams Chest
        176,     // compass
        171,     // Buried Chest
        177,     // Courtyard South Chest
        290,     // BaC
        178,     // cheese
        179,     // Breakable Floor POH
        180,     // Wooden Beam C. Chest
        457,     // poe 2
        182,     // Entry Chest Left
        183,     // Entry Chest Right
        186      // Storage Chest Left
    };
    u16 SPR_6[22] {
        456,     // poe 1
        339,     // map
        348,     // ooccoo
        172,     // Courtyard Open Chest
        174,     // CY Partially Buried Chest
        173,     // pumpkin
        185,     // Storage Chest Right
        175,     // Small Beams Chest
        176,     // compass
        171,     // Buried Chest
        177,     // Courtyard South Chest
        290,     // BaC
        178,     // cheese
        179,     // Breakable Floor POH
        180,     // Wooden Beam C. Chest
        457,     // poe 2
        182,     // Entry Chest Left
        183,     // Entry Chest Right
        186,     // Storage Chest Left
        181,     // Entry POH Chest
        458,     // poe 3
        184      // Chilfos Small Chest
    };

    u16 ToT_keys[3] {
        193,     // Entry Chest
        197,     // Armos Room Left Chest
        200      // Third Stair Window Chest
    };
    u16 ToT_1[2] {
        193,     // Entry Chest
        349      // ooccoo
    };
    u16 ToT_2[8] {
        193,     // Entry Chest
        349,     // ooccoo
        194,     // First Stair Small Chest pots
        195,     // First Stair Small Chest
        196,     // map
        459,     // poe 1
        207,     // Armos Room Small Chest
        197      // Armos Room Left Chest
    };
    u16 ToT_3[16] {
        193,     // Entry Chest
        349,     // ooccoo
        194,     // First Stair Small Chest pots
        195,     // First Stair Small Chest
        196,     // map
        459,     // poe 1
        207,     // Armos Room Small Chest
        197,     // Armos Room Left Chest
        198,     // compass
        199,     // Scale Room Spider Chest
        460,     // poe 2
        203,     // Small Chest Scale Room
        204,     // Helmasaur Small Chest
        205,     // boss key
        200,     // Third Stair Window Chest
        201      // Gilloutine Chest
    };

    u16 CitS_keys[1] {
        219     // West Wing Chest 1
    };
    u16 CitS_1[7] {
        217,     // Underwater Chest 1
        218,     // Underwater Chest 2
        219,     // West Wing Chest 1
        350,     // ooccoo
        488,     // Lantern_Oil
        489,     // Red_Potion
        490      // Blue_Potion
    };

    u16 PoT_keys[7] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        251,     // East 1st North Chest
        255,     // East 2nd SE Chest
        258,     // Central Room 1st Chest
        260,     // Central Outdoor Chest
        261      // Central Tower Chest
    };
    u16 PoT_1[1] {
        245     // West 1st Central Chest
    };
    u16 PoT_2[4] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248      // West 2nd SE Chest
    };
    u16 PoT_3[6] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248,     // West 2nd SE Chest
        250,     // East 1st Small Chest
        251      // East 1st North Chest
    };
    u16 PoT_4[10] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248,     // West 2nd SE Chest
        250,     // East 1st Small Chest
        251,     // East 1st North Chest
        252,     // East 2nd NE Chest
        253,     // East 2nd NW Chest
        254,     // map
        255      // East 2nd SE Chest
    };
    u16 PoT_5[14] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248,     // West 2nd SE Chest
        250,     // East 1st Small Chest
        251,     // East 1st North Chest
        252,     // East 2nd NE Chest
        253,     // East 2nd NW Chest
        254,     // map
        255,     // East 2nd SE Chest
        249,     // West 1st POH Chest
        256,     // East 1st POH Chest
        257,     // East 1st West Chest
        258      // Central Room 1st Chest
    };
    u16 PoT_6[16] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248,     // West 2nd SE Chest
        250,     // East 1st Small Chest
        251,     // East 1st North Chest
        252,     // East 2nd NE Chest
        253,     // East 2nd NW Chest
        254,     // map
        255,     // East 2nd SE Chest
        249,     // West 1st POH Chest
        256,     // East 1st POH Chest
        257,     // East 1st West Chest
        258,     // Central Room 1st Chest
        259,     // big key
        260      // Central Outdoor Chest
    };
    u16 PoT_7[17] {
        245,     // West 1st Central Chest
        246,     // West 2nd Central Chest
        247,     // compass
        248,     // West 2nd SE Chest
        250,     // East 1st Small Chest
        251,     // East 1st North Chest
        252,     // East 2nd NE Chest
        253,     // East 2nd NW Chest
        254,     // map
        255,     // East 2nd SE Chest
        249,     // West 1st POH Chest
        256,     // East 1st POH Chest
        257,     // East 1st West Chest
        258,     // Central Room 1st Chest
        259,     // big key
        260,     // Central Outdoor Chest
        261      // Central Tower Chest
    };

    u16 HC_keys[3] {
        342,     // king bulblin key
        265,     // Grave Owl Chest
        274      // SE Balcony Tower Chest
    };
    u16 HC_1[9] {
        262,     // Grave Switch Right Chest
        263,     // Grave Switch Left Chest 1
        264,     // Grave Switch Left Chest 2
        265,     // Grave Owl Chest
        266,     // map
        267,     // East Castle Balcony
        268,     // West CY North Chest
        269,     // West CY Central Chest
        342      // king bulblin key
    };
    u16 HC_2[15] {
        262,     // Grave Switch Right Chest
        263,     // Grave Switch Left Chest 1
        264,     // Grave Switch Left Chest 2
        265,     // Grave Owl Chest
        266,     // map
        267,     // East Castle Balcony
        268,     // West CY North Chest
        269,     // West CY Central Chest
        342,     // king bulblin key
        270,     // compass
        271,     // Lantern Staircase
        272,     // HUB SW Chest
        273,     // HUB NW Chest
        274,     // SE Balcony Tower Chest
        275      // big key
    };
    u16 HC_3[15] {
        262,     // Grave Switch Right Chest
        263,     // Grave Switch Left Chest 1
        264,     // Grave Switch Left Chest 2
        265,     // Grave Owl Chest
        266,     // map
        267,     // East Castle Balcony
        268,     // West CY North Chest
        269,     // West CY Central Chest
        342,     // king bulblin key
        270,     // compass
        271,     // Lantern Staircase
        272,     // HUB SW Chest
        273,     // HUB NW Chest
        274,     // SE Balcony Tower Chest
        275      // big key
    };

    u16 F_keys[2] {
        2,      // North Cave Key
        351     // coro key
    };
    u16 F_1[4] {
        296,     // lantern
        2,       // North Cave Key
        3,       // PoH key cave
        1        // transition cave
    };
    u16 F_2[2] {
        329,     // coro bottle
        351      // coro key
    };

    u16 GD_keys[1] {
        353     // camp key
    };

    u16 GD_1[21] {
        125,     // desert chest
        126,     // desert chest
        127,     // desert chest
        131,     // desert chest
        133,     // desert chest
        134,     // desert chest
        135,     // desert chest
        136,     // desert chest
        137,     // desert chest
        138,     // desert chest
        139,     // desert chest
        140,     // camp small chest 1
        141,     // camp small chest 2
        338,     // camp PoH
        353,     // camp key
        377,     // Male Dayfly
        378,     // Female Dayfly
        417,     // poe 13
        418,     // poe 14
        419,     // poe 15
        422      // poe 18
    };

}     // namespace mod::keyPlacement