#pragma once
#include "defines.h"

struct Z2SceneMgr;

struct JAISoundID
{
    u32 id;
};
namespace tp::Z2AudioLib::SceneMgr
{
    extern "C"
    {
        void sceneChange( Z2SceneMgr* sceneMgr,
                          JAISoundID id,
                          u8 SeWave1,
                          u8 SeWave2,
                          u8 BgmWave1,
                          u8 BgmWave2,
                          u8 DemoWave,
                          bool param_7 );
    };
}     // namespace tp::Z2AudioLib::SceneMgr
