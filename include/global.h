#pragma once

#include "types.h"

#include <tp/d_map_path_dmap.h>

#define getPlayerPos tp::d_map_path_dmap::getMapPlayerPos

extern "C"
{
	bool checkForButtonInput(u32 buttonCombo);
	void setConsoleColor(u32 rgba);
	void setConsole(bool activeFlag, u32 totalLines);
	void clear_DC_IC_Cache(void* ptr, u32 size);
}