#pragma once

#include "defines.h"
#include "memory.h"

namespace mod::patch
{
	void writeBranch(void* ptr, void* destination);
	void writeBranchLR(void* ptr, void* destination);
	void writeBranchMain(void* ptr, void* destination, u32 branch);

	template<typename Func, typename Dest>
	Func hookFunction(Func function, Dest destination)
	{
		u32* instructions = reinterpret_cast<u32*>(function);

		u32* trampoline = new u32[2];

		// Original instruction
		trampoline[0] = instructions[0];
		memory::clear_DC_IC_Cache(&trampoline[0], sizeof(u32));

		// Branch to original function past hook
		writeBranch(&trampoline[1], &instructions[1]);

		// Write actual hook
		writeBranch(&instructions[0], reinterpret_cast<void*>(static_cast<Func>(destination)));

		return reinterpret_cast<Func>(trampoline);
	}
}