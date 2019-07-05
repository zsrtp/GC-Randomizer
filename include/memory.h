#pragma once

#include "defines.h"

#include <cstddef>

namespace mod::memory
{
	extern "C"
	{
		void* clearMemory(void* ptr, std::size_t size);
		void clear_DC_IC_Cache(void* ptr, std::size_t size);
	}
}