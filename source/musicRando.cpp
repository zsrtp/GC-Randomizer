#include "musicRando.h"
#include "tools.h"
#include "patch.h"
#include "tp/Z2SceneMgr.h"
#include "tp/Z2SeqMgr.h"

struct BGM { //Struct to store info about the source of BGM and the waves required to play correctly. Not using full Ids here to save memory
    u8 id;
    u8 bgmwave_1;
};

static const BGM bgmSource[] = {
{0x0,0x19},
{0x1,0x19},
{0x4,0x0d},
{0x5,0x03},
{0x6,0x02},
{0x9,0x0a},
{0xd,0x0c},
{0xe,0x0c},
{0xf,0x00},
{0x10,0x1},
{0x11,0x2},
{0x16,0xe},
{0x17,0x5},
{0x18,0x13},
{0x1a,0x11},
{0x1b,0xe},
{0x1d,0x14},
{0x1e,0x7},
{0x1f,0x7},
{0x20,0x15},
{0x23,0xf},
{0x24,0x9},
{0x25,0x16},
{0x26,0x10},
{0x27,0x17},
{0x29,0x18},
{0x2c,0x8},
{0x2d,0x1a},
{0x2e,0x1b},
{0x2f,0x1c},
{0x30,0x1e},
{0x31,0x1e},
{0x34,0x1f},
{0x35,0x20},
{0x37,0x1d},
{0x39,0x48},
{0x3a,0x24},
{0x3b,0x25},
{0x3c,0x26},
{0x3d,0x27},
{0x3e,0x28},
{0x3f,0x29},
{0x40,0x11},
{0x41,0x2a},
{0x42,0x2b},
{0x47,0x2c},
{0x48,0x2c},
{0x49,0x00},
{0x4a,0x2d},
{0x4b,0x3a},
{0x4c,0x2e},
{0x4d,0x2e},
{0x50,0x2f},
{0x51,0x30},
{0x57,0x31},
{0x58,0x32},
{0x59,0x33},
{0x5a,0x34}, //Lake hylia, but plays plums song
{0x5e,0x35},
{0x5f,0x36},
{0x60,0x37},
{0x61,0x3f},
{0x62,0x39},
{0x6b,0x3e},
{0x6c,0x3f},
{0x6d,0x40},
{0x6e,0x57},
{0x6f,0x3f},
{0x70,0x44},
{0x77,0x45},
{0x78,0x46},
{0x7a,0x46},
{0x85,0x48},
{0x86,0x49},
{0x87,0x4a},
{0x88,0x4b},
{0x8b,0x4c},
{0x8c,0x4c},
{0x8f,0x1e},
{0x90,0x1e},
{0x91,0x47},
{0x94,0x4e},
{0x95,0x4e},
{0x96,0x3d},
{0x97,0x3d},
{0x9a,0x8},
{0x9b,0x56},
{0x9e,0x19},
{0x9f,0x55},
{0xa5,0x57},
{0xa8,0x58},
{0xa9,0x59}
};

//Some fanfares can't be played using the current method as when subStartBgm is called it includes additional logic on some, such as stopping the main music track
static const u8 fanfareSrc[] = {
    0x0a,
    0x0b,
    0x12,
    0x14,
    0x1c,
    0x43,
    0x44,
    0x45,
    0x46,
    0x81,
    0x82,
    0x83,
    0x9c,
    0x9d,
    0xa0
};

static const u8 bgmSource_length = sizeof(bgmSource)/sizeof(BGM);

static u8 randomizedBGMs[bgmSource_length];

static const u8 fanfareSourceLength = sizeof(fanfareSrc);

static u8 randomizedFanfares[fanfareSourceLength];

void (*sceneChange_trampoline)(Z2SceneMgr* sceneMgr, JAISoundID id,u8 SeWave1,u8 SeWave2,u8 BgmWave1,u8 BgmWave2,u8 DemoWave,bool param_7) = nullptr;
void (*startBattleBgm_trampoline)(Z2SeqMgr* seqMgr, bool param_1) = nullptr;
void (*subBgmStart_trampoline)(Z2SeqMgr* seqMgr,u32 id) = nullptr;

static void sceneChangeHook(Z2SceneMgr* sceneMgr, JAISoundID BGMId,u8 SeWave1,u8 SeWave2,u8 BgmWave1,u8 BgmWave2,u8 DemoWave,bool param_7) {
    u32 id = BGMId.id;
    if (mod::musicrando::musicRandoEnabled && id>=0x1000000 && id<0x2000000)
    {
         //Only Sequences are applied here
        id = id-0x1000000;
        u8 index_of_id = 0;
        bool found = false;
        for (u8 i = 0; i < bgmSource_length; i++) {
            if (randomizedBGMs[i] == id) {
                index_of_id = i;
                found = true;
                break;
            }
        }
        if (found) {
            JAISoundID new_id;
            new_id.id = bgmSource[index_of_id].id + 0x1000000;
            if(bgmSource[index_of_id].bgmwave_1==0) {
                sceneChange_trampoline(sceneMgr,new_id,SeWave1,SeWave2,BgmWave1,BgmWave2,DemoWave,param_7);
            }else{
                sceneChange_trampoline(sceneMgr,new_id,SeWave1,SeWave2,bgmSource[index_of_id].bgmwave_1,BgmWave2,DemoWave,param_7);
            }
        }else{
            sceneChange_trampoline(sceneMgr,BGMId,SeWave1,SeWave2,BgmWave1,BgmWave2,DemoWave,param_7);
        }
    }else{
        sceneChange_trampoline(sceneMgr,BGMId,SeWave1,SeWave2,BgmWave1,BgmWave2,DemoWave,param_7);
    }
}

static void subBgmStartHook(Z2SeqMgr* seqMgr, u32 id) {
    id = id - 0x1000000;
    u8 index = 0;
    bool found = false;
    for (u8 i = 0; i < fanfareSourceLength; i++) {
        if (randomizedFanfares[i] == id) {
            index = i;
            found = true;
            break;
        }
    }
    if (found) {
        u32 newId = fanfareSrc[index]+0x1000000;
        subBgmStart_trampoline(seqMgr,newId);
    }else{
        subBgmStart_trampoline(seqMgr,id);
    }
}

static void startBattleBgmHook(Z2SeqMgr* seqMgr, bool param_1) {
    if (mod::musicrando::enemyBgmEnabled) {
        startBattleBgm_trampoline(seqMgr,param_1);
    }
}

static void randomizeTable(void* srcTable,u8 destTable[],size_t srcTableElementSize, size_t tableSize) {
    for(u8 i = 0; i<tableSize; i++){ //Fills in the array with random ids
        bool gotUnique = false;
        while (!gotUnique) { //Depends on randomness to shuffle the original array; needs a better method
            u8 random = mod::tools::getRandom(tableSize);
            u8 idGot = *((u8*)srcTable+(random*srcTableElementSize));
            bool valueExists = false;
            for (u8 j = 0; j<i; j++) {
                if (destTable[j] == idGot){
                    valueExists = true;
                    break;
                }
            }
            if (valueExists == false){
                destTable[i] = *(((u8*)srcTable)+(random*srcTableElementSize));
                gotUnique = true;
                break;
            }
        }
    } 
}

#define HookTrampoline(trampoline,origin,hook) if (trampoline == nullptr) { trampoline = mod::patch::hookFunction(origin,hook); }

namespace mod::musicrando {
    u8 musicRandoEnabled = 0;
    u8 enemyBgmEnabled = 1;
    void initMusicRando() {
        randomizeTable((void*)&bgmSource,randomizedBGMs,sizeof(BGM),bgmSource_length);
        randomizeTable((void*)&fanfareSrc,randomizedFanfares,sizeof(u8),fanfareSourceLength);
        HookTrampoline(sceneChange_trampoline,tp::Z2AudioLib::SceneMgr::sceneChange,sceneChangeHook);
        HookTrampoline(startBattleBgm_trampoline,tp::Z2AudioLib::SeqMgr::startBattleBgm,startBattleBgmHook);
        HookTrampoline(subBgmStart_trampoline,tp::Z2AudioLib::SeqMgr::subBgmStart,subBgmStartHook);
    }
} //namespace mod::musicrando
