#pragma once

#include "HUDConsole.h"
#include "chestRando.h"
#include "eventListener.h"
#include "game_patches.h"
#include "mod.h"

namespace mod::global
{
    extern mod::Mod* modPtr;
    extern mod::ChestRandomizer* chestRandoPtr;
    extern mod::event::EventListener* eventListenerPtr;
    extern mod::HUDConsole* hudConsolePtr;
    extern uint64_t* seedInSaveFile;
}     // namespace mod::global