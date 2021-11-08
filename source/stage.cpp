#include "stage.h"

#include "defines.h"

namespace mod::stage
{
    char allStages[76][8] = {
        "D_MN01",  "D_MN01A", "D_MN01B", "D_MN04",  "D_MN04A", "D_MN04B", "D_MN05",  "D_MN05A", "D_MN05B", "D_MN06",  "D_MN06A",
        "D_MN06B", "D_MN07",  "D_MN07A", "D_MN07B", "D_MN08",  "D_MN08A", "D_MN08B", "D_MN08C", "D_MN08D", "D_MN09",  "D_MN09A",
        "D_MN09B", "D_MN09C", "D_MN10",  "D_MN10A", "D_MN10B", "D_MN11",  "D_MN11A", "D_MN11B", "D_SB00",  "D_SB01",  "D_SB02",
        "D_SB03",  "D_SB04",  "D_SB05",  "D_SB06",  "D_SB07",  "D_SB08",  "D_SB09",  "D_SB10",  "F_SP00",  "F_SP102", "F_SP103",
        "F_SP104", "F_SP108", "F_SP109", "F_SP110", "F_SP111", "F_SP112", "F_SP113", "F_SP114", "F_SP115", "F_SP116", "F_SP117",
        "F_SP118", "F_SP121", "F_SP122", "F_SP123", "F_SP124", "F_SP125", "F_SP126", "F_SP127", "F_SP128", "F_SP200", "R_SP01",
        "R_SP107", "R_SP108", "R_SP109", "R_SP110", "R_SP116", "R_SP127", "R_SP128", "R_SP160", "R_SP161", "R_SP209" };

    const char* dungeonStages[18] {
        allStages[Stage_Lakebed_Temple],
        allStages[Stage_Deku_Toad],
        allStages[Stage_Goron_Mines],
        allStages[Stage_Dangoro],
        allStages[Stage_Forest_Temple],
        allStages[Stage_Ook],
        allStages[Stage_Temple_of_Time],
        allStages[Stage_Darknut],
        allStages[Stage_City_in_the_Sky],
        allStages[Stage_Aeralfos],
        allStages[Stage_Palace_of_Twilight],
        allStages[Stage_Phantom_Zant_1],
        allStages[Stage_Phantom_Zant_2],
        allStages[Stage_Hyrule_Castle],
        allStages[Stage_Arbiters_Grounds],
        allStages[Stage_Death_Sword],
        allStages[Stage_Snowpeak_Ruins],
        allStages[Stage_Darkhammer],
    };

    const char* bossStages[8] { allStages[Stage_Morpheel],
                                allStages[Stage_Fyrus],
                                allStages[Stage_Diababa],
                                allStages[Stage_Armogohma],
                                allStages[Stage_Argorok],
                                allStages[Stage_Zant_Main],
                                allStages[Stage_Stallord],
                                allStages[Stage_Blizzeta] };

    const char* shopStages[8] { allStages[Stage_Ordon_Interiors],
                                allStages[Stage_Faron_Woods],
                                allStages[Stage_Kakariko_Interiors],
                                allStages[Stage_Death_Mountain],
                                allStages[Stage_Kakariko_Village],
                                allStages[Stage_Castle_Town_Shops],
                                allStages[Stage_Castle_Town],
                                allStages[Stage_City_in_the_Sky] };

    const char* grottoStages[5] { allStages[Stage_Grotto_1],
                                  allStages[Stage_Grotto_2],
                                  allStages[Stage_Grotto_3],
                                  allStages[Stage_Grotto_4],
                                  allStages[Stage_Grotto_5] };

    const char* caveStages[6] { allStages[Stage_Lanayru_Ice_Puzzle_Cave],
                                allStages[Stage_Cave_of_Ordeals],
                                allStages[Stage_Eldin_Long_Cave],
                                allStages[Stage_Lake_Hylia_Long_Cave],
                                allStages[Stage_Eldin_Goron_Stockcave],
                                allStages[Stage_Faron_Woods_Cave] };

    const char* interiorStages[8] { allStages[Stage_Ordon_Interiors],
                                    allStages[Stage_Kakariko_Interiors],
                                    allStages[Stage_Castle_Town_Shops],
                                    allStages[Stage_Sanctuary_Basement],
                                    allStages[Stage_Impaz_House],
                                    allStages[Stage_Henas_Cabin],
                                    allStages[Stage_Castle_Town_Interiors],
                                    allStages[Stage_Castle_Town] };

    const char* specialStages[3] { allStages[Stage_Title_Screen], allStages[Stage_Bublin_2], allStages[Stage_Hidden_Skill] };

    const char* timeOfDayStages[18] { allStages[Stage_Ordon_Village],
                                      allStages[Stage_Ordon_Spring],
                                      allStages[Stage_Ordon_Ranch],
                                      allStages[Stage_Faron_Woods],
                                      allStages[Stage_Death_Mountain],
                                      allStages[Stage_Kakariko_Graveyard],
                                      allStages[Stage_Kakariko_Village],
                                      allStages[Stage_Zoras_River],
                                      allStages[Stage_Zoras_Domain],
                                      allStages[Stage_Upper_Zoras_River],
                                      allStages[Stage_Lake_Hylia],
                                      allStages[Stage_Outside_Castle_Town],
                                      allStages[Stage_Hyrule_Field],
                                      allStages[Stage_Sacred_Grove],
                                      allStages[Stage_Bublin_Camp],
                                      allStages[Stage_Gerudo_Desert],
                                      allStages[Stage_Hidden_Village],
                                      allStages[Stage_Fishing_Pond] };

    const char* mainDungeonStages[9] { allStages[Stage_Lakebed_Temple],
                                       allStages[Stage_Goron_Mines],
                                       allStages[Stage_Forest_Temple],
                                       allStages[Stage_Temple_of_Time],
                                       allStages[Stage_City_in_the_Sky],
                                       allStages[Stage_Palace_of_Twilight],
                                       allStages[Stage_Hyrule_Castle],
                                       allStages[Stage_Arbiters_Grounds],
                                       allStages[Stage_Snowpeak_Ruins] };

    const char* allDungeonStages[26] { allStages[Stage_Lakebed_Temple],
                                       allStages[Stage_Deku_Toad],
                                       allStages[Stage_Goron_Mines],
                                       allStages[Stage_Dangoro],
                                       allStages[Stage_Forest_Temple],
                                       allStages[Stage_Ook],
                                       allStages[Stage_Temple_of_Time],
                                       allStages[Stage_Darknut],
                                       allStages[Stage_City_in_the_Sky],
                                       allStages[Stage_Aeralfos],
                                       allStages[Stage_Palace_of_Twilight],
                                       allStages[Stage_Phantom_Zant_1],
                                       allStages[Stage_Phantom_Zant_2],
                                       allStages[Stage_Hyrule_Castle],
                                       allStages[Stage_Arbiters_Grounds],
                                       allStages[Stage_Death_Sword],
                                       allStages[Stage_Snowpeak_Ruins],
                                       allStages[Stage_Darkhammer],
                                       allStages[Stage_Morpheel],
                                       allStages[Stage_Fyrus],
                                       allStages[Stage_Diababa],
                                       allStages[Stage_Armogohma],
                                       allStages[Stage_Argorok],
                                       allStages[Stage_Zant_Main],
                                       allStages[Stage_Stallord],
                                       allStages[Stage_Blizzeta] };
}     // namespace mod::stage