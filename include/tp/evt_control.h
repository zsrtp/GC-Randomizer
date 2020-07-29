#pragma once

#include "defines.h"

namespace tp::evt_control
{
	typedef s32 (csFunc)(void* unk, s32 unk2);
	extern "C"
	{
		s32 skipper(void* eventPtr);
		s32 defaultSkipStb(void* unk, s32 unk2);
	}
}