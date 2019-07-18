#pragma once

#include "defines.h"
#include <tp/d_com_inf_game.h>
#include "eventListener.h"
#include "chestRando.h"
#include "HUDConsole.h"

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
			HUDConsole* hudConsole;

			// Options
			u8 customSeed; // Use custom seed?
			u8 randoEnabled; // Randomize chests?
			u8 truePause; // Disable controls during console?

			// Debug info
			char lastItemFunc[32]; // Last called item create function

			// Replacment handler
			s32 procItemCreateFunc(const float pos[3], s32 item, const char funcIdentifier[32]);

		// Functions
		private:
			/**
			 * Runs once each frame
			 */
			void procNewFrame();		
			
			s32 procEvtSkipper(void* evtPtr);

		// Private members
		//private:
			

		// Hook trampolines
		private:
			void (*fapGm_Execute_trampoline)() = nullptr;
			
			s32 (*evt_control_Skipper_trampoline)(void* eventPtr) = nullptr;

			bool (*checkTreasureRupeeReturn_trampoline)(void*, s32) = nullptr;

			// Item functions
			s32 (*createItemForPresentDemo_trampoline)(const float pos[3], s32 item, u8 unk1, s32 unk2, s32 unk3, const float unk4[3], const float unk5[3]) = nullptr;
			s32 (*createItemForTrBoxDemo_trampoline)(const float pos[3], s32 item, s32 unk1, s32 unk2, const float unk3[3], const float unk4[3]) = nullptr;
			s32 (*createItemForBoss_trampoline)(const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], float unk4, float unk5, s32 unk6) = nullptr;
			s32 (*createItemForMidBoss_trampoline)(const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], float unk4, float unk5, s32 unk6) = nullptr;
			s32 (*createItemForDirectGet_trampoline)(const float pos[3], s32 item, s32 unk1, const float unk2[3], const float unk3[3], float unk4, float unk5) = nullptr;
	};
}