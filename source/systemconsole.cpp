#include "systemconsole.h"
#include "defines.h"

#include <tp/JFWSystem.h>

namespace mod::systemconsole
{
	void setConsoleColor(u32 rgba)
	{
		u32* ConsoleColor = reinterpret_cast<u32*>(tp::JFWSystem::systemConsole->consoleColor);
		*ConsoleColor = rgba;
	}

	void setConsole(bool activeFlag, u32 totalLines)
	{
		tp::JFWSystem::SystemConsole* Console = tp::JFWSystem::systemConsole;
		Console->consoleEnabled = activeFlag;

		for (u32 i = 0; i < totalLines; i++)
		{
			Console->consoleLine[i].showLine = activeFlag;
		}
	}
}