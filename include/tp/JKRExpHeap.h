#pragma once

#include "defines.h"

namespace tp::jkr_exp_heap
{
	extern "C"
	{
		void* do_alloc_JKRExpHeap(void* heap, u32 size, s32 unk3);
		void do_free_JKRExpHeap(void* heap, void* ptr);
	}
}