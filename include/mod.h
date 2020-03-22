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
			u8 inputBuffering; // En/Disable buffering

			u8 lastItemSearchID;
			u8 itemSearchID;

			u8 lastItemReverseSearchID;
			u8 itemReverseSearchID;
			char itemSearchResults[30];
			char itemReverseSearchResults[30];

			// Debug info
			char lastItemFunc[32]; // Last called item create function
			char lastItemDataID[5];
			char lastItemDataX[30];
			char lastItemDataY[30];
			char lastItemDataZ[30];
			
			char currentPosX[30];
			char currentPosY[30];
			char currentPosZ[30];

			// Replacment handler
			s32 procItemCreateFunc(const float pos[3], s32 item, const char funcIdentifier[32]);

		// Functions
		private:
			/**
			 * Runs once each frame
			 */
			void procNewFrame();		
			
			/**
			 * Runs when checking if the treasure chest content
			 * should be returned or can be obtained
			 */
			bool procCheckTreasureRupeeReturn(void* unk1, s32 item);

			s32 procEvtSkipper(void* evtPtr);

		// Private members
		//private:
			

		// Hook trampolines
		private:
			void (*fapGm_Execute_trampoline)() = nullptr;
			
			s32 (*evt_control_Skipper_trampoline)(void* eventPtr) = nullptr;

			bool (*checkTreasureRupeeReturn_trampoline)(void* unk1, s32 item) = nullptr;

			// Item functions
			s32 (*createItemForPresentDemo_trampoline)(const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3]) = nullptr;
			s32 (*createItemForTrBoxDemo_trampoline)(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3]) = nullptr;
			s32 (*createItemForBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7, s32 unk8) = nullptr;
			s32 (*createItemForMidBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7) = nullptr;
			s32 (*createItemForDirectGet_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7) = nullptr;
	};
}