#pragma once

#include "defines.h"

namespace tp::resource
{
// Message commands
#define MSG_BEGIN( name ) const char* name =

#define MSG_END() ;

// Message Speeds
#define MSG_SPEED( speed ) "\x1A\x05\x00\x00" speed

#define MSG_SPEED_FAST "\x01"
#define MSG_SPEED_SLOW "\x02"

#define MSG_COLOR( id ) "\x1A\x06\xFF\x00\x00" id

// Message Icons
#define MSG_ICON( icon ) "\x1A\x05\x00\x00" icon

// Message colors
#define MSG_COLOR_WHITE "\x00"
#define MSG_COLOR_RED "\x01"
#define MSG_COLOR_GREEN "\x02"
#define MSG_COLOR_LIGHT_BLUE "\x03"
#define MSG_COLOR_YELLOW "\x04"
#define MSG_COLOR_PURPLE "\x06"
#define MSG_COLOR_ORANGE "\x08"

// Custom message colors
#define CUSTOM_MSG_COLOR_DARK_GREEN "\x09"
#define CUSTOM_MSG_COLOR_BLUE "\x0A"
#define CUSTOM_MSG_COLOR_SILVER "\x0B"

// Standard hex values for custom message colors
// Needed for the getFontCCColorTable and getFontGCColorTable hooks
#define CUSTOM_MSG_COLOR_DARK_GREEN_HEX 0x9
#define CUSTOM_MSG_COLOR_BLUE_HEX 0xA
#define CUSTOM_MSG_COLOR_SILVER_HEX 0xB

// Message Icon Values
#define MSG_ICON_R "\x0E"
#define MSG_ICON_A "\x0A"
#define MSG_ICON_X "\x0F"
#define MSG_ICON_Y "\x10"

    extern "C"
    {
        char parseCharacter_1Byte( const char** text );
    }
}     // namespace tp::resource