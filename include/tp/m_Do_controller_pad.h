#pragma once

#include "defines.h"

namespace tp::m_Do_controller_pad
{
	// Should try to fill in the missing variables at some point
	struct CPadInfo
	{
		u8 unk_0[0x30];
		u32 wButtonInput;
		u8 unk_34[0xCC];
	} __attribute__((__packed__));

	static_assert(sizeof(CPadInfo) == 0x100);

	extern "C"
	{
		extern CPadInfo cpadInfo;
	}
}