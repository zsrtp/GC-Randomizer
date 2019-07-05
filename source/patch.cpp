#include "patch.h"
#include "defines.h"
#include "memory.h"

namespace mod::patch
{
	void writeBranch(void* ptr, void* destination)
	{
		u32 branch = 0x48000000; // b
		writeBranchMain(ptr, destination, branch);
	}

	void writeBranchLR(void* ptr, void* destination)
	{
		u32 branch = 0x48000001; // bl
		writeBranchMain(ptr, destination, branch);
	}

	void writeBranchMain(void* ptr, void* destination, u32 branch)
	{
		u32 delta = reinterpret_cast<u32>(destination) - reinterpret_cast<u32>(ptr);

		branch |= (delta & 0x03FFFFFC);

		u32* p = reinterpret_cast<u32*>(ptr);
		*p = branch;

		memory::clear_DC_IC_Cache(ptr, sizeof(u32));
	}
}