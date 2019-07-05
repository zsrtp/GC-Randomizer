#pragma once

#include "defines.h"

namespace mod::controller
{
	extern "C"
	{
		bool checkForButtonInput(u32 buttonCombo);
		bool checkForButtonInputSingleFrame(u32 buttonCombo);
	}
}