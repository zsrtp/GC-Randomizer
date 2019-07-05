#pragma once

#include "defines.h"

namespace mod::systemconsole
{
	extern "C"
	{
		void setConsoleColor(u32 rgba);
		void setConsole(bool activeFlag, u32 totalLines);
	}
}