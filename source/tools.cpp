#include "defines.h"
#include "tools.h"
#include "global.h"

#include <tp/evt_control.h>
#include <tp/d_com_inf_game.h>
#include <tp/JFWSystem.h>
#include <cstring>
#include <cstdio>

namespace mod::tools
{
	u64 randomSeed = 0x9e3779b97f4a7c15;

	u32 getRandom(u32 max)
	{
		u64 z = (randomSeed += 0x9e3779b97f4a7c15);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
		z = (z ^ (z >> 27)) * 0x94d049bb133111eb;

		return (z % max);
	}

	void triggerRandomGenerator()
	{
		sprintf(sysConsolePtr->consoleLine[12].line, "New rando");
		global::chestRandoPtr->generate();
	}

	void triggerSaveLoad(char* stage, u8 room, u8 spawn, u8 state, u8 event)
	{
		strcpy(gameInfo.nextStageVars.nextStage, stage);
		gameInfo.nextStageVars.nextRoom = room;
		gameInfo.nextStageVars.nextSpawnPoint = spawn;
		gameInfo.nextStageVars.nextState = state;

		gameInfo.eventSystem.nextEventID = event;
		gameInfo.respawnAnimation = 0;
		gameInfo.nextStageVars.isVoidorWarp = 0;
		gameInfo.respawnCutscene = 0;
		gameInfo.eventSystem.immediateControl = 0xFFFF;
		gameInfo.nextStageVars.fadeType = 0x13;

		gameInfo.nextStageVars.triggerLoad = true;
	}

	void setCutscene(bool skippable, bool fade, tp::evt_control::csFunc onSkip)
	{
		gameInfo.eventSystem.fadeOnSkip = fade;
		if(skippable)
		{
			gameInfo.eventSystem.onSkip = onSkip;
		}
		else
		{
			gameInfo.eventSystem.onSkip = nullptr;
		}
	}

	
	u32 fletcher32(u32* data, size_t len)
	{
			u32 c0, c1;
			size_t i;

			for (c0 = c1 = 0; len >= 360; len -= 360)
			{
				for (i = 0; i < 360; i++)
				{
						c0 = c0 + *data++;
						c1 = c1 + c0;
				}
				c0 = c0 % 65535;
				c1 = c1 % 65535;
			}

			for (i = 0; i < len; i++)
			{
					c0 = c0 + *data++;
					c1 = c1 + c0;
			}
			c0 = c0 % 65535;
			c1 = c1 % 65535;
			return (c1 << 16 | c0);
	}
}