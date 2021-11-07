#include "systemConsole.h"

#include <tp/JFWSystem.h>

#include "defines.h"

namespace mod::system_console
{
    void setBackgroundColor( u32 rgba )
    {
        u32* ConsoleColor = reinterpret_cast<u32*>( sysConsolePtr->consoleColor );
        *ConsoleColor = rgba;
    }

    void setState( bool activeFlag, u32 totalLines )
    {
        tp::jfw_system::SystemConsole* Console = sysConsolePtr;
        Console->consoleEnabled = activeFlag;

        for ( u32 i = 0; i < totalLines; i++ )
        {
            Console->consoleLine[i].showLine = activeFlag;
        }
    }
}     // namespace mod::system_console