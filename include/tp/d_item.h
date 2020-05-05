#pragma once

#include "defines.h"

namespace tp::d_item
{
	extern "C"
	{
		s32 execItemGet(u8 item);
		s32 checkItemGet(u8 item, s32 defaultValue);
		void item_func_UTUWA_HEART();
	}
} 