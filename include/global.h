#pragma once

#include "mod.h"
#include "chestRando.h"
#include "eventListener.h"
#include "HUDConsole.h"
#include "game_patches.h"

namespace mod::global
{
	extern mod::Mod* modPtr;
	extern mod::ChestRandomizer* chestRandoPtr;
	extern mod::event::EventListener* eventListenerPtr;
	extern mod::HUDConsole* hudConsolePtr;
}