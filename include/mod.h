#pragma once

#include "defines.h"
#include "eventListener.h"
#include "chestRando.h"

namespace mod
{
	class Mod
	{
		public:
			Mod();
			/**
			 * Init this and other stuff in order for all
			 * modifications to work
			 */
			void init();

			/**
			 * Custom event listener that can
			 * schedule function calls
			 */
			event::EventListener* eventListener;
			ChestRandomizer* chestRandomizer;

		// Functions
		private:
			/**
			 * Runs once each frame
			 */
			void procNewFrame();
			
			void assemblyOverwrites();


			s32 procCreateItemForTrBoxDemo(const float[3], s32, s32, s32, const float[3], const float[3]);
			
			s32 procEvtSkipper(void* evtPtr);

		// Private members
		//private:
			

		// Hook trampolines
		private:
			void (*fapGm_Execute_trampoline)() = nullptr;
			
			s32 (*evt_control_Skipper_trampoline)(void* eventPtr) = nullptr;

			bool (*checkTreasureRupeeReturn_trampoline)(void*, s32) = nullptr;

			s32 (*createItemForTrBox_trampoline)(const float[3], s32, s32, s32, const float[3], const float[3]) = nullptr;
	};
}