#pragma once

#include "mod.h"
#include "chestRando.h"
#include "eventListener.h"

namespace mod::global
{
	extern mod::Mod* modPtr;
	extern mod::ChestRandomizer* chestRandoPtr;
	extern mod::event::EventListener* eventListenerPtr;
}