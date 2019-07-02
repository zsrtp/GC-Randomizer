#include "global.h"
#include "types.h"

#include <gc/OSCache.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/JFWSystem.h>

bool checkForButtonInput(u32 buttonCombo)
{
	return tp::m_Do_controller_pad::cpadInfo.wButtonInput & buttonCombo;
}

void setConsoleColor(u32 RGBA)
{
	*reinterpret_cast<u32*>(tp::JFWSystem::systemConsole->wConsoleColor) = RGBA;
}

void setConsole(bool activeFlag, u32 totalLines)
{
	tp::JFWSystem::SystemConsole* Console = tp::JFWSystem::systemConsole;
	Console->wConsoleEnabled = activeFlag;
	
	for (u32 i = 0; i < totalLines; i++)
	{
		Console->wConsoleLine[i].wShowLine = activeFlag;
	}
}

void clear_DC_IC_Cache(void* ptr, u32 size)
{
	gc::OSCache::DCFlushRange(ptr, size);
	gc::OSCache::ICInvalidateRange(ptr, size);
}