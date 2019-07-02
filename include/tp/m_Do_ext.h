#pragma once

#include "types.h"

namespace tp::m_Do_ext
{
	extern "C"
	{
		void* getArchiveHeapPtr();

		extern void* archiveHeap;
	}
}