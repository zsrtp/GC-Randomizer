#pragma once

#include "defines.h"

namespace tp::m_do_controller_pad
{
	// Should try to fill in the missing variables at some point
	struct CPadInfo
	{
		u8 unk_0[0x30];
		u32 buttonInput;
		u32 buttonInputTrg;
		u8 unk_38[0xC8];
	} __attribute__((__packed__));

	static_assert(sizeof(CPadInfo) == 0x100);

	extern "C"
	{
		extern CPadInfo cpadInfo;
	}
}