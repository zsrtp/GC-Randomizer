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

	u16 fletcher16(u8 *data, size_t count)
	{
		u16 sum1 = 0;
		u16 sum2 = 0;
		size_t index;

		for (index = 0; index < count; ++index)
		{
			sum1 = (sum1 + data[index]) % 255;
			sum2 = (sum2 + sum1) % 255;
		}

		return (sum2 << 8) | sum1;
	}
}