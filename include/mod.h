#pragma once

#include "defines.h"
#include "item.h"

namespace mod
{
	class Mod
	{
		public:
			Mod();
			void init();
			
		private:
			void run();

			// Conditions used by the generator to determine wether a check is already reachable
			u16 currentPlayerConditions;

			// Assumed fill
			u16 startConditions = 0xFFFF;

			char version[6];

			u16 getItemFlags(u8 item);

			void assemblyOverwrites();

			void procCreateItemForTrBoxDemo(const float[3], s32, s32, s32, const float[3], const float[3]);
			
		private:
			void (*fapGm_Execute_trampoline)() = nullptr;
			
			bool (*checkTreasureRupeeReturn_trampoline)(void*, s32) = nullptr;

			void (*createItemForTrBox_trampoline)(const float[3], s32, s32, s32, const float[3], const float[3]) = nullptr;
	};
}