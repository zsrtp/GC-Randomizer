#pragma once

#include "types.h"

struct ChestDisplayInfo
{
	bool DisplayActive;
	u8 FrameCounter;
};

extern "C" {

bool checkForButtonInput(u32 buttonCombo);
void setConsoleColor(u8, u8, u8, u8);
void setConsole(bool activeFlag, u32 totalLines);
void clear_DC_IC_Cache(void* ptr, u32 size);

extern ChestDisplayInfo chestDisplayInfo;

}