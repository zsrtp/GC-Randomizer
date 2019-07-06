#include "mod.h"
#include "defines.h"
#include "systemConsole.h"
#include "patch.h"

#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/d_a_alink.h>

namespace mod
{
	Mod* gMod = nullptr;

	void main()
	{
		Mod* mod = new Mod();
		mod->init();
	}

	Mod::Mod()
	{
		gMod = this;
	}

	void Mod::init()
	{
		// Perform any necessary assembly overwrites
		assemblyOverwrites();

		// Set the initial console color
		system_console::setBackgroundColor(0x00A0A0A0);

		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute,
			[]()
			{
				gMod->run();
			}
		);

		checkTreasureRupeeReturn_trampoline = patch::hookFunction(tp::d_a_alink::checkTreasureRupeeReturn,
			[](void* unused, s32 amount)
			{
				// Disable this function as this check happens before the replacement
				// Might render an item unobtainable if you're having rupees
				return false;
			}
		);

		createItemForTrBox_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo,
			[](const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
			{
				gMod->procCreateItemForTrBoxDemo(pos, item, unk3, unk4, unk5, unk6);
			}
		);
	}
	
	void Mod::run()
	{
		// Runs once each frame

		// Call original function
		fapGm_Execute_trampoline();
	}

	void Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Runs once when Link picks up an item with text and is holding it towards the camera

		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}

	void Mod::assemblyOverwrites()
	{
		// Get the addresses to overwrite
		#ifdef TP_US
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#elif defined TP_EU
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B878);
		#elif defined TP_JP
		u32* enableCrashScreen = reinterpret_cast<u32*>(0x8000B8A4);
		#endif

		// Perform the overwrites

		/* If the address is loaded into the cache before the overwrite is made, 
		then the cache will need to be cleared after the overwrite */

		// Enable the crash screen
		*enableCrashScreen = 0x48000014; // b 0x14
	}
}