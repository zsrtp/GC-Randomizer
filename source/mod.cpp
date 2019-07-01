#include "mod.h"
#include "patch.h"
#include "types.h"

#include <gc/buttons.h>
#include <tp/f_ap_game.h>
#include <tp/f_op_actor_mng.h>
#include <tp/JFWSystem.h>
#include <tp/d_com_inf_game.h>
#include <tp/d_kankyo.h>
#include <tp/d_map_path_dmap.h>

#include <cstdio>
#include <cstring>

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
		
	}

	void Mod::init()
	{
		gMod = this;
		
		// Set the initial console color
		u32 ConsoleColor = 0x0000001E;
		setConsoleColor(ConsoleColor);
		
		fapGm_Execute_trampoline = patch::hookFunction(tp::f_ap_game::fapGm_Execute, []()
		{
			gMod->run();
		});
		
		createItemForTrBox_trampoline = patch::hookFunction(tp::f_op_actor_mng::createItemForTrBoxDemo, [](
			const float pos[3], s32 item, s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
		{
			gMod->procCreateItemForTrBoxDemo(unk1, item, unk3, unk4, unk5, unk6);
		});
	}
	
	void Mod::run()
	{
		if (chestDisplayInfo.DisplayActive)
		{
			// Check if Y is held
			if (checkForButtonInput(PAD_Y))
			{
				u32 FrameCounter = chestDisplayInfo.FrameCounter;
				FrameCounter++;
				
				// Check if Y has been held for 4 seconds
				if (FrameCounter >= 120)
				{
					// Turn the display off
					chestDisplayInfo.DisplayActive = false;
					chestDisplayInfo.FrameCounter = 0;
					setConsole(false, 6);
				}
				else
				{
					chestDisplayInfo.FrameCounter = FrameCounter;
				}
			}
			else
			{
				// Reset the frame counter
				chestDisplayInfo.FrameCounter = 0;
			}
		}
		
		// Call original function
		fapGm_Execute_trampoline();
	}
	
	void Mod::procCreateItemForTrBoxDemo(const float pos[3], s32 item, 
		s32 unk3, s32 unk4, const float unk5[3], const float unk6[3])
	{
		// Turn the display on
		chestDisplayInfo.DisplayActive = true;
		setConsole(true, 6);
		
		tp::JFWSystem::SystemConsole* Console = tp::JFWSystem::systemConsole;
		u32 MaxLineLength = sizeof(tp::JFWSystem::ConsoleLine::wLine);
		
		// Print values to screen
		snprintf(Console->wConsoleLine[0].wLine, MaxLineLength, "Pos: %f %f %f", pos[0], pos[1], pos[2]);
		snprintf(Console->wConsoleLine[1].wLine, MaxLineLength, "Item: 0x%02x", item);
		
		char* stage = tp::d_com_inf_game::dComIfG_gameInfo.wCurrentStage;
		snprintf(Console->wConsoleLine[2].wLine, MaxLineLength, "Stage: %s", stage);
		
		u8 room = tp::d_kankyo::env_light.wCurrentRoom;
		snprintf(Console->wConsoleLine[3].wLine, MaxLineLength, "Room: %02u", room);
		
		float LinkMapPos[3];
		tp::d_map_path_dmap::getMapPlayerPos(LinkMapPos);
		snprintf(Console->wConsoleLine[4].wLine, MaxLineLength, "Link: %f %f %f", LinkMapPos[0], LinkMapPos[1], LinkMapPos[2]);
		
		strcpy(Console->wConsoleLine[5].wLine, "Hold Y for 4 seconds to close this window");

		// Call original function
		createItemForTrBox_trampoline(pos, item, unk3, unk4, unk5, unk6);
	}

}
