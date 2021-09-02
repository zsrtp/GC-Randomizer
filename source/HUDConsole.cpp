#include "HUDConsole.h"

#include <cstdio>
#include <cstring>

#include "defines.h"
#include "systemConsole.h"

namespace mod
{
    HUDConsole::HUDConsole( const char firstPage[16], u32 RGBA )
    {
        strcpy( pages[0], firstPage );
        numPages = 1;

        // Set the visibility flags for all lines but disable the console
        system_console::setState( true, 25 );
        system_console::setState( false, 0 );

        system_console::setBackgroundColor( RGBA );
    }

    void HUDConsole::addOption( u8 page, const char title[16], u8* target, u32 limit )
    {
        // Page is 0-based index, numPages the absolute counter
        if ( page < numPages )
        {
            // Find the option struct
            u8 optionIndex = numOptions[page];

            if ( optionIndex < 10 )
            {
                strcpy( options[page][optionIndex].Title, title );
                options[page][optionIndex].Target = target;
                options[page][optionIndex].Limit = limit;

                // Incrase the counter
                numOptions[page]++;
            }
        }
    }

    void HUDConsole::addWatch( u8 page, const char title[16], void* target, char format, u8 interpretation )
    {
        // Page is 0-based index, numPages the absolute counter
        if ( page < numPages )
        {
            // Find the watch struct
            u8 watchIndex = numWatches[page];

            if ( watchIndex < 10 )
            {
                strcpy( watches[page][watchIndex].Title, title );
                watches[page][watchIndex].Target = target;
                watches[page][watchIndex].Format = format;
                watches[page][watchIndex].Interpretation = interpretation;

                // Increase the counter
                numWatches[page]++;
            }
        }
    }

    s8 HUDConsole::addPage( const char title[16] )
    {
        if ( numPages < MAX_HUDCONSOLE_PAGES )
        {
            sprintf( pages[numPages], title );
            numPages++;

            return ( numPages - 1 );
        }
        return -1;
    }

    void HUDConsole::performAction( u8 consoleAction, u8 amount )
    {
        Option* o = &options[selectedPage][selectedOption];
        switch ( consoleAction )
        {
            case ConsoleActions::Move_Up:
                // Y movement (Caret)
                if ( selectedOption > 0 )
                {
                    selectedOption--;
                }
                else
                {
                    // Go to last option
                    selectedOption = numOptions[selectedPage] - 1;
                }
                break;

            case ConsoleActions::Move_Down:
                // Y movement (Caret)
                if ( selectedOption < numOptions[selectedPage] - 1 )
                {
                    selectedOption++;
                }
                else
                {
                    // Reset position
                    selectedOption = 0;
                }
                break;

            case ConsoleActions::Move_Left:
                selectedOption = 0;
                // X Movement (Page)
                if ( selectedPage > 0 )
                {
                    selectedPage--;
                }
                else
                {
                    // Go to last page
                    selectedPage = numPages - 1;
                }
                break;

            case ConsoleActions::Move_Right:
                selectedOption = 0;
                // X Movement (Page)
                if ( selectedPage < numPages - 1 )
                {
                    selectedPage++;
                }
                else
                {
                    // Go to first page
                    selectedPage = 0;
                }
                break;

            case ConsoleActions::Option_Increase:
                if ( numOptions[selectedPage] < 1 )
                    return;
                if ( *o->Target + amount <= o->Limit )
                {
                    *o->Target = *o->Target + amount;
                }
                else
                {
                    // Loop back
                    *o->Target = 0;
                }
                break;

            case ConsoleActions::Option_Decrease:
                if ( numOptions[selectedPage] < 1 )
                    return;
                else if ( *o->Target - amount >= 0 )
                {
                    *o->Target = *o->Target - amount;
                }
                else
                {
                    // Loop back
                    *o->Target = static_cast<u8>( o->Limit );
                }
                break;
        }
    }

    void HUDConsole::draw()
    {
        // Current line index
        u8 i = 0;

        tp::jfw_system::SystemConsole* console = sysConsolePtr;

        // Print heading
        sprintf( console->consoleLine[i].line,
                 "[%d/%d][%-18s]  %s (C) %s",
                 selectedPage + 1,
                 numPages,
                 pages[selectedPage],
                 VERSION,
                 AUTHOR );

        // Print options
        for ( i = 1; i <= numOptions[selectedPage]; i++ )
        {
            // Template:
            // [>] [title] (padding) [value]
            u8 optionindex = i - 1;     // Option[0] is line 1
            Option o = options[selectedPage][optionindex];

            if ( o.Limit == 1 )
            {
                sprintf( console->consoleLine[i].line,
                         "%c%-25s %s",
                         ( optionindex == selectedOption ? '>' : ' ' ),
                         o.Title,
                         ( *o.Target ? "Yes" : "No" ) );
            }
            else
            {
                sprintf( console->consoleLine[i].line,
                         "%c%-25s %02x",
                         ( optionindex == selectedOption ? '>' : ' ' ),
                         o.Title,
                         *o.Target );
            }
        }

        for ( i = i; i < 11; i++ )
        {
            console->consoleLine[i].line[0] = '\0';
        }

        for ( i = i; ( i - 11 ) < numWatches[selectedPage]; i++ )
        {
            u8 watchIndex = i - 11;
            // Print the watch to this line
            Watch w = watches[selectedPage][watchIndex];

            char format[12];
            sprintf( format, "%s%c", "%-15s %", w.Format );

            if ( w.Interpretation == WatchInterpretation::_s64 || w.Interpretation == WatchInterpretation::_u64 )
            {
                sprintf( format, "%s%c", "%-15s %ll", w.Format );
            }

            switch ( w.Interpretation )
            {
                case WatchInterpretation::_u8:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<u8*>( w.Target ) );
                    break;

                case WatchInterpretation::_u16:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<u16*>( w.Target ) );
                    break;

                case WatchInterpretation::_u32:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<u32*>( w.Target ) );
                    break;

                case WatchInterpretation::_u64:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<u64*>( w.Target ) );
                    break;

                case WatchInterpretation::_s8:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<s8*>( w.Target ) );
                    break;

                case WatchInterpretation::_s16:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<s16*>( w.Target ) );
                    break;

                case WatchInterpretation::_s32:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<s32*>( w.Target ) );
                    break;

                case WatchInterpretation::_s64:
                    sprintf( console->consoleLine[i].line, format, w.Title, *reinterpret_cast<s64*>( w.Target ) );
                    break;

                case WatchInterpretation::_str:
                    sprintf( console->consoleLine[i].line, format, w.Title, reinterpret_cast<char*>( w.Target ) );
                    break;

                default:
                    strcpy( console->consoleLine[i].line, "Error parsing watch" );
                    break;
            }
        }

        for ( i = i; i < 20; i++ )
        {
            console->consoleLine[i].line[0] = '\0';
        }
    }
}     // namespace mod