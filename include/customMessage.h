#pragma once

#include <tp/resource.h>

#include "defines.h"

namespace mod::customMessage
{
    MSG_BEGIN( customForestSKText )
    MSG_SPEED( MSG_SPEED_FAST )
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "small key" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "!\nIt can be used in the\n" 
    MSG_COLOR(MSG_COLOR_GREEN) 
    "Forest Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customMinesSKText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "small key" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n" 
    MSG_COLOR(MSG_COLOR_RED) 
    "Goron Mines" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customLakebedSKText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "small key" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_BLUE) 
    "Lakebed Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customArbitersSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_ORANGE)
    "Arbiter's Grounds"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customSnowpeakSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_LIGHT_BLUE)
    "Snowpeak Ruins"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customToTSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_DARK_GREEN)
    "Temple of Time"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCitySKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_YELLOW)
    "City in The Sky"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customPalaceSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_PURPLE)
    "Palace of Twilight"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCastleSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_SILVER)
    "Hyrule Castle"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customBublinSKText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "small key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_ORANGE)
    "Bulblin Camp"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCrystalText) 
    MSG_SPEED(MSG_SPEED_SLOW) 
    "You got the " 
    MSG_COLOR(MSG_COLOR_RED) 
    "Shadow Crystal" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "!\nThis is a dark manifestation\nof "
    MSG_COLOR(MSG_COLOR_RED)
    "Zant's "
    MSG_COLOR(MSG_COLOR_WHITE)
    "power that allows\nyou to transform at will!\nHold "
    MSG_ICON(MSG_ICON_R)
    MSG_COLOR(MSG_COLOR_WHITE)
    " then "
    MSG_ICON(MSG_ICON_Y)
    MSG_COLOR(MSG_COLOR_WHITE)
    " at the same\ntime to quickly transform.\nWhile talking to "
    MSG_COLOR(MSG_COLOR_LIGHT_BLUE)
    "Midna"
    MSG_COLOR(MSG_COLOR_WHITE)
    ", hold\n"
    MSG_ICON(MSG_ICON_R)
    MSG_COLOR(MSG_COLOR_WHITE)
    " then "
    MSG_ICON(MSG_ICON_Y)
    MSG_COLOR(MSG_COLOR_WHITE)
    " to change time."
    MSG_END()

    MSG_BEGIN(customForestMapText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "dungeon map" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "!\nIt can be used in the\n" 
    MSG_COLOR(MSG_COLOR_GREEN) 
    "Forest Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customMinesMapText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "dungeon map" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n" 
    MSG_COLOR(MSG_COLOR_RED) 
    "Goron Mines" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customLakebedMapText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "dungeon map" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_BLUE) 
    "Lakebed Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customArbitersMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_ORANGE)
    "Arbiter's Grounds"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customSnowpeakMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_LIGHT_BLUE)
    "Snowpeak Ruins"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customToTMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_DARK_GREEN)
    "Temple of Time"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCityMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_YELLOW)
    "City in The Sky"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customPalaceMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_PURPLE)
    "Palace of Twilight"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCastleMapText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "dungeon map"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_SILVER)
    "Hyrule Castle"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customForestCompassText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "compass" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "!\nIt can be used in the\n" 
    MSG_COLOR(MSG_COLOR_GREEN) 
    "Forest Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customMinesCompassText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "compass" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n" 
    MSG_COLOR(MSG_COLOR_RED) 
    "Goron Mines" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customLakebedCompassText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "compass" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_BLUE) 
    "Lakebed Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customArbitersCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_ORANGE)
    "Arbiter's Grounds"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customSnowpeakCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_LIGHT_BLUE)
    "Snowpeak Ruins"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customToTCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_DARK_GREEN)
    "Temple of Time"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCityCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_YELLOW)
    "City in The Sky"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customPalaceCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_PURPLE)
    "Palace of Twilight"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCastleCompassText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "compass"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_SILVER)
    "Hyrule Castle"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customForestBigKeyText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "big key" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "!\nIt can be used in the\n" 
    MSG_COLOR(MSG_COLOR_GREEN) 
    "Forest Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customMinesBigKeyText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "big key" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n" 
    MSG_COLOR(MSG_COLOR_RED) 
    "Goron Mines" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customLakebedBigKeyText) 
    MSG_SPEED(MSG_SPEED_FAST) 
    "You got a " 
    MSG_COLOR(MSG_COLOR_RED) 
    "big key" 
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_BLUE) 
    "Lakebed Temple" 
    MSG_COLOR(MSG_COLOR_WHITE) 
    "." 
    MSG_END()

    MSG_BEGIN(customArbitersBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_ORANGE)
    "Arbiter's Grounds"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customSnowpeakBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(MSG_COLOR_LIGHT_BLUE)
    "Snowpeak Ruins"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customToTBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_DARK_GREEN)
    "Temple of Time"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCityBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_YELLOW)
    "City in The Sky"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customPalaceBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in the\n"
    MSG_COLOR(MSG_COLOR_PURPLE)
    "Palace of Twilight"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(customCastleBigKeyText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got a "
    MSG_COLOR(MSG_COLOR_RED)
    "big key"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!\nIt can be used in\n"
    MSG_COLOR(CUSTOM_MSG_COLOR_SILVER)
    "Hyrule Castle"
    MSG_COLOR(MSG_COLOR_WHITE)
    "."
    MSG_END()

    MSG_BEGIN(endingBlowText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Ending Blow"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(shieldAttackText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Shield Attack"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(backSliceText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Back Slice"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(helmSplitterText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Helm Splitter"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(mortalDrawText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Mortal Draw"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(jumpStrikeText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Jump Strike"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(greatSpinText)
    MSG_SPEED(MSG_SPEED_FAST)
    "You got the "
    MSG_COLOR(MSG_COLOR_RED)
    "Great Spin"
    MSG_COLOR(MSG_COLOR_WHITE)
    "!"
    MSG_END()

    MSG_BEGIN(customPoweredRodText)
    MSG_SPEED(MSG_SPEED_FAST)
    "Power has been restored to\nthe "
    MSG_COLOR(MSG_COLOR_RED)
    "Dominion Rod"
    MSG_COLOR(MSG_COLOR_WHITE)
    "! Now it can\nbe used to imbude statues\nwith life in the present!"
    MSG_END()
}     // namespace mod::customMessage