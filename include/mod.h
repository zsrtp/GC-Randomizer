#pragma once

#include "defines.h"
#include <tp/d_com_inf_game.h>
#include <tp/DynamicLink.h>
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
			
			u8 enableNormalTime = 1;			
			u8 setDay = 1;
			
			char skyAngle[30];
			
			u8 stage = 0;
			u8 room = 0;
			u8 spawn = 0;
			u8 state = 0;
			u8 trigerLoadSave = 0;
			
			u8 dungeonFlagsView1;
			u8 dungeonFlagsView2;
			u8 dungeonFlagsView3;
			u8 dungeonFlagsView4;
			u8 dungeonFlagsView5;
			u8 dungeonFlagsView6;
			u8 dungeonFlagsView7;
			u8 dungeonFlagsView8;
			u8 dungeonFlagsViewEdit;
			
			u16 red1;
			u16 green1;
			u16 blue1;
			u16 red2;
			u16 green2;
			u16 blue2;
			
			u16 scoopResult;
			u16 checkResult;
			u16 itemsResult;
			u16 mapResult;
			
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

			bool proc_query022(void* unk1, void* unk2, s32 unk3);

			bool procDoLink(tp::dynamic_link::DynamicModuleControl* dmc);

			void procItem_func_UTUWA_HEART();
			
			/**
			 * gives the unlocked scent that can be seen in the current area (defaults to most advanced one obtained)
			 */
			void giveAllScents();
			
			/**
			 * gives the unlocked scent that can be seen in the current area (defaults to most advanced one obtained)
			 */
			void giveAllStoryItems();
			
			/**
			 * order the item wheel correctly
			 */
			void reorderItemWheel();

		// Private members
		//private:
			

		// Hook trampolines
		private:
			void (*fapGm_Execute_trampoline)() = nullptr;
			
			s32 (*evt_control_Skipper_trampoline)(void* eventPtr) = nullptr;

			bool (*query022_trampoline)(void* unk1, void* unk2, s32 unk3) = nullptr;

			bool (*do_link_trampoline)(tp::dynamic_link::DynamicModuleControl* dmc) = nullptr;

			bool (*checkTreasureRupeeReturn_trampoline)(void* unk1, s32 item) = nullptr;

			void (*item_func_UTUWA_HEART_trampoline)() = nullptr;

			// Item functions
			s32 (*createItemForPresentDemo_trampoline)(const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3]) = nullptr;
			s32 (*createItemForTrBoxDemo_trampoline)(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3]) = nullptr;
			s32 (*createItemForBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7, s32 unk8) = nullptr;
			s32 (*createItemForMidBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7) = nullptr;
			s32 (*createItemForDirectGet_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7) = nullptr;
	};
}