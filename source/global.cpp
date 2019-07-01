#include "global.h"
#include "types.h"

#include <gc/OSCache.h>
#include <tp/m_Do_controller_pad.h>
#include <tp/JFWSystem.h>

struct ChestDisplayInfo chestDisplayInfo;

bool checkForButtonInput(u32 buttonCombo)
{
	return tp::m_Do_controller_pad::cpadInfo.wButtonInput & buttonCombo;
}

void setConsoleColor(u8 R, u8 G, u8 B, u8 A)
{
	*reinterpret_cast<u32*>(tp::JFWSystem::systemConsole->wConsoleColor) = (R << 24) | (G << 16) | (B << 8) | A;
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