#pragma once

#include "defines.h"

namespace mod::system_console
{
	extern "C"
	{
		void setBackground(u32 rgba);
		void setState(bool activeFlag, u32 totalLines);
	}
}