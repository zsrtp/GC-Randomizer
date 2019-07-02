#pragma once

#include "types.h"

namespace tp::JKRExpHeap
{
	extern "C"
	{
		void* do_alloc_JKRExpHeap(void* heap, u32 size, s32 padding);
		void do_free_JKRExpHeap(void* heap, void* ptr);
	}
}