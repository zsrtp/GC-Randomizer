#pragma once

#include "defines.h"
#include <tp/d_com_inf_game.h>
#include <tp/DynamicLink.h>
#include <tp/dzx.h>
#include "eventListener.h"
#include "chestRando.h"
#include "HUDConsole.h"
#include <string>

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

			//item search
			u8 lastItemSearchID;
			u8 itemSearchID;

			u8 lastItemReverseSearchID;
			u8 itemReverseSearchID;
			char itemSearchResults[30];
			char itemReverseSearchResults[30];

			//check search
			u8 checkSearchID1;
			u8 checkSearchID2;

			u8 checkReverseSearchID1;
			u8 checkReverseSearchID2;

			u16 lastCheckSearchID;
			u16 checkSearchID;

			u16 lastCheckReverseSearchID;
			u16 checkReverseSearchID;
			char checkSearchResults[30];
			char checkReverseSearchResults[30];

			// Debug info
			char lastItemFunc[32]; // Last called item create function
			char lastItemDataID[5];
			char lastItemDataX[30];
			char lastItemDataY[30];
			char lastItemDataZ[30];
			
			char currentPosX[30];
			char currentPosY[30];
			char currentPosZ[30];

			char linkAngle[30];

			u8 coordsAreInHex = 0;

			// Replacment handler
			s32 procItemCreateFunc(const float pos[3], s32 item, const char funcIdentifier[32]);
			
			u8 enableNormalTime = 1;			
			u8 setDay = 1;

			u32 skyAngle;

			u8 enableQuickTransform = 1;
			
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

			u16 colorResult;

			u8 topToggle = 1;
			u8 redTop = 0;
			u8 greenTop = 0;
			u8 blueTop = 0;
			u8 bottomToggle = 1;
			u8 redBottom = 0;
			u8 greenBottom = 0;
			u8 blueBottom = 0;

			u8 bottle4Contents;
			u8 bottleTrickOn = 0;
			u8 allowBottleItemsShopAnytime = 1;
			u8 shieldTrickOn = 0;
			u8 hadHShield;
			u8 hadOShield;
			u8 hadWShield;
			u8 bombBagTrickOn = 0;
			u8 bombBag1Contents;
			u8 bombBag2Contents;
			u8 bombBag3Contents;
			u8 bombBag1Ammo;
			u8 bombBag2Ammo;
			u8 bombBag3Ammo;
			u8 lastGoodSpawn;

			u8 yetaTrickOn = 0;

			u8 LBTBossDoorTrickOn = 0;
			u8 nbLBTKeys = 0;

			u8 eventFlagToEdit = 0;
			u8 newValueForEventFlag = 0;
			u8 triggerEventFlagEdit = 0;
			u8 innerRed = 0x50;
			u8 innerGreen = 0x28;
			u8 innerBlue = 0x14;
			u8 outerRed = 0x28;
			u8 outerGreen = 0x1E;
			u8 outerBlue = 0x0A;

			char itemName[10];
			//u8 newItemId;

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

			bool canQuickTransform();

			bool canChangeToD();

			/**
			 * gives the unlocked scent that can be seen in the current area (defaults to most advanced one obtained)
			 */
			void giveAllStoryItems();

			/**
			 * order the item wheel correctly
			 */
			void reorderItemWheel();

			/**
			 * Allows Yeta to always be in the living room even with the BK
			 * Allows Yeto to always be in the kitchen even with the BK
			 */
			void fixYetaAndYeto();

			/**
			 * fix problem where opening LBT boss door removes a key
			 */
			void fixLBTBossDoor();

			/**
			 * removes the empty skybook if you are in the sanctuary basement
			 */
			void preventPoweringUpDomRod();

			/**
			 * gives the unlocked scent that can be seen in the current area (defaults to most advanced one obtained)
			 */
			void giveAllScents();

			/**
			 * renders all shop items buyable no matter what you have in you inv
			 * only works for bottle items for now
			 */
			void allowShopItemsAnytime();

			/**
			 * checks if the current stage contains a shop
			 */
			bool isStageShop();

			/**
			 * Inserts custom TRES Boxes if applicable to this stage+room
			 */
			void doCustomTRESActor(void* mStatus_roomControl);

			void changeLanternColor();

			void fixFTTotemMonkey();

			//void setFieldModels();

			//bool procActorCommonLayerInit(void* mStatus_roomControl, tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo, s32 unk3, void* unk4);
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

			bool (*actorCommonLayerInit_trampoline)(void* mStatus_roomControl, tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4) = nullptr;

			void (*putSave_trampoline)(tp::d_com_inf_game::GameInfo* gameInfoPtr, s32 areaID) = nullptr;

			// Item functions
			s32 (*createItemForPresentDemo_trampoline)(const float pos[3], s32 item, u8 unk3, s32 unk4, s32 unk5, const float unk6[3], const float unk7[3]) = nullptr;
			s32 (*createItemForTrBoxDemo_trampoline)(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3]) = nullptr;
			s32 (*createItemForBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7, s32 unk8) = nullptr;
			s32 (*createItemForMidBoss_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], s32 unk6, s32 unk7) = nullptr;
			s32 (*createItemForDirectGet_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7) = nullptr;
			s32(*createItemForSimpleDemo_trampoline)(const float pos[3], s32 item, s32 unk3, const float unk4[3], const float unk5[3], float unk6, float unk7) = nullptr;
			s32(*createItem_trampoline)(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3], s32 unk7) = nullptr;

			void(*setItemBombNumCount_trampoline)(u32 unk1, u8 bagNb, short amount) = nullptr;
	};
}