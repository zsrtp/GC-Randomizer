#pragma once

#include <tp/DynamicLink.h>
#include <tp/control.h>
#include <tp/d_com_inf_game.h>
#include <tp/dzx.h>

#include <string>

#include "HUDConsole.h"
#include "chestRando.h"
#include "defines.h"
#include "eventListener.h"

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
        u8 customSeed;         // Use custom seed?
        u8 randoEnabled;       // Randomize chests?
        u8 truePause;          // Disable controls during console?
        u8 inputBuffering;     // En/Disable buffering

        // item search
        u8 lastItemSearchID;
        u8 itemSearchID;

        u8 lastItemReverseSearchID;
        u8 itemReverseSearchID;
        char itemSearchResults[30];
        char itemReverseSearchResults[30];

        // check search
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
        char lastItemFunc[32];     // Last called item create function
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
        s32 procItemCreateFunc( const float pos[3], s32 item, const char funcIdentifier[32] );

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
        // u8 newItemId;

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
        bool procCheckTreasureRupeeReturn( void* unk1, s32 item );

        s32 procEvtSkipper( void* evtPtr );

        bool proc_query022( void* unk1, void* unk2, s32 unk3 );

        bool proc_query023( void* unk1, void* unk2, s32 unk3 );

        bool proc_query024( void* dMsgFlow_cPtr, void* mesg_flow_node_branchPtr, void* fopAc_ac_cPtr, int unused );

        bool proc_query025( void* unk1, void* unk2, s32 unk3 );

        bool proc_isDungeonItem( void* memBitPtr, const int param_1 );

        bool procDoLink( tp::dynamic_link::DynamicModuleControl* dmc );

        void procItem_func_UTUWA_HEART();

        bool canQuickTransform();

        bool canChangeToD();

        s32 getMsgIndex( const void* TProcessor, u16 unk2, u32 msgId );

        s32 getItemMsgIndex( const void* TProcessor, u16 unk2, u32 itemId );

        s32 getItemIdFromMsgId( const void* TProcessor, u16 unk3, u32 msgId );

        u32 getCustomMsgColor( u8 colorId );

        s32 proc_checkItemGet( u8 item, s32 defaultValue );

        bool proc_isEventBit( u8* eventSystem, u16 indexNumber );

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
         * removes the empty skybook if you are in the sanctuary basement
         */
        void preventPoweringUpDomRod();

        /**
         * gives the unlocked scent that can be seen in the current area (defaults to most advanced one obtained)
         */
        void giveAllScents();

        /**
         * checks if the current stage contains a shop
         */
        bool isStageShop();

        /**
         * Inserts custom TRES Boxes if applicable to this stage+room
         */
        void doCustomTRESActor( void* mStatus_roomControl );

        void changeLanternColor();

        // Private members
        // private:

        // Hook trampolines
       private:
        void ( *fapGm_Execute_trampoline )() = nullptr;

        s32 ( *evt_control_Skipper_trampoline )( void* eventPtr ) = nullptr;

        bool ( *query022_trampoline )( void* unk1, void* unk2, s32 unk3 ) = nullptr;

        bool ( *query023_trampoline )( void* unk1, void* unk2, s32 unk3 ) = nullptr;

        bool ( *query024_trampoline )( void* dMsgFlow_cPtr,
                                       void* mesg_flow_node_branchPtr,
                                       void* fopAc_ac_cPtr,
                                       int unused ) = nullptr;

        bool ( *query025_trampoline )( void* unk1, void* unk2, s32 unk3 ) = nullptr;

        bool ( *do_link_trampoline )( tp::dynamic_link::DynamicModuleControl* dmc ) = nullptr;

        bool ( *checkTreasureRupeeReturn_trampoline )( void* unk1, s32 item ) = nullptr;

        void ( *item_func_UTUWA_HEART_trampoline )() = nullptr;

        bool ( *actorCommonLayerInit_trampoline )( void* mStatus_roomControl,
                                                   tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo,
                                                   int unk3,
                                                   void* unk4 ) = nullptr;

        bool ( *actorInit_always_trampoline )( void* mStatus_roomControl,
                                               tp::d_stage::dzxChunkTypeInfo* chunkTypeInfo,
                                               int unk3,
                                               void* unk4 ) = nullptr;

        void ( *putSave_trampoline )( tp::d_com_inf_game::GameInfo* gameInfoPtr, s32 areaID ) = nullptr;

        // Item functions
        s32 ( *createItemForPresentDemo_trampoline )( const float pos[3],
                                                      s32 item,
                                                      u8 unk3,
                                                      s32 unk4,
                                                      s32 unk5,
                                                      const float unk6[3],
                                                      const float unk7[3] ) = nullptr;
        s32 ( *createItemForTrBoxDemo_trampoline )( const float pos[3],
                                                    s32 item,
                                                    s32 unk3,
                                                    s32 unk4,
                                                    const float unk5[3],
                                                    const float unk6[3] ) = nullptr;
        s32 ( *createItemForBoss_trampoline )( const float pos[3],
                                               s32 item,
                                               s32 unk3,
                                               const float unk4[3],
                                               const float unk5[3],
                                               float unk6,
                                               float unk7,
                                               s32 unk8 ) = nullptr;
        s32 ( *createItemForMidBoss_trampoline )( const float pos[3],
                                                  s32 item,
                                                  s32 unk3,
                                                  const float unk4[3],
                                                  const float unk5[3],
                                                  s32 unk6,
                                                  s32 unk7 ) = nullptr;
        s32 ( *createItemForDirectGet_trampoline )( const float pos[3],
                                                    s32 item,
                                                    s32 unk3,
                                                    const float unk4[3],
                                                    const float unk5[3],
                                                    float unk6,
                                                    float unk7 ) = nullptr;
        s32 ( *createItemForSimpleDemo_trampoline )( const float pos[3],
                                                     s32 item,
                                                     s32 unk3,
                                                     const float unk4[3],
                                                     const float unk5[3],
                                                     float unk6,
                                                     float unk7 ) = nullptr;
        s32 ( *createItem_trampoline )( const float pos[3],
                                        s32 item,
                                        s32 unk3,
                                        s32 unk4,
                                        const float unk5[3],
                                        const float unk6[3],
                                        s32 unk7 ) = nullptr;

        void ( *setItemBombNumCount_trampoline )( u32 unk1, u8 bagNb, short amount ) = nullptr;

        bool ( *isDungeonItem_trampoline )( void* memBitPtr, int param_1 ) = nullptr;

        int ( *getLayerNo_common_common_trampoline )( const char* stageName, int roomId, int layerOverride ) = nullptr;

        bool ( *setMessageCode_inSequence_trampoline )( tp::control::TControl* control,
                                                        const void* TProcessor,
                                                        u16 unk3,
                                                        u16 msgId ) = nullptr;

        u32 ( *getFontCCColorTable_trampoline )( u8 colorId, u8 unk ) = nullptr;
        u32 ( *getFontGCColorTable_trampoline )( u8 colorId, u8 unk ) = nullptr;

        s32 ( *checkItemGet_trampoline )( u8 item, s32 defaultValue ) = nullptr;

        char ( *parseCharacter1Byte_trampoline )( const char** text ) = nullptr;

        bool ( *isEventBit_trampoline )( u8* eventSystem, u16 indexNumber ) = nullptr;

        void ( *onEventBit_trampoline )( u8* eventSystem, u16 indexNumber ) = nullptr;

        void ( *setGetItemFace_trampoline )( void* daAlink_c, u16 itemId ) = nullptr;
    };
}     // namespace mod