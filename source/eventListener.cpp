#include "defines.h"
#include "eventListener.h"

#include <tp/d_com_inf_game.h>
#include <tp/d_kankyo.h>
#include <tp/JFWSystem.h>

#include <cstdio>
#include <cstring>

namespace mod::event
{
	EventListener::EventListener()
	{
		sprintf(sysConsolePtr->consoleLine[20].line, "Event Listener Initialized.");
	}

	void EventListener::addLoadEvent(char* stage, u8 room, u8 spawn, u8 state, eventFunction trigger, LoadEventAccuracy accuracy)
	{
		if(loadEventIndex < MAX_LOAD_EVENTS)
		{
			// Add the event to the array
			loadEvents[loadEventIndex] = {stage, room, spawn, state, trigger, accuracy};

			loadEventIndex++;
		}
	}

	void EventListener::checkLoadEvents()
	{
		char* stage = gameInfo.nextStageVars.nextStage;
		u8 room = gameInfo.nextStageVars.nextRoom;
		u8 spawn = static_cast<u8>(gameInfo.nextStageVars.nextSpawnPoint);
		u8 state = gameInfo.nextStageVars.nextState;

		for(u8 i = 0; i < loadEventIndex; i++)
		{
			LoadEvent e = loadEvents[i];

			if(strcmp(stage, e.Stage) == 0)
			{
				// Stage is minimum requirement
				if(e.Accuracy == LoadEventAccuracy::Stage)
				{
					e.Trigger();
					break;
				}

				if(room == e.Room)
				{
					if(e.Accuracy == LoadEventAccuracy::Stage_Room)
					{
						e.Trigger();
						break;
					}

					if(spawn == e.Spawn)
					{
						if(e.Accuracy == LoadEventAccuracy::Stage_Room_Spawn)
						{
							e.Trigger();
							break;
						}

						if(state == e.State)
						{
							// Cannot be more accurate, trigger without checking
							e.Trigger();
							break;
						}
					}
				}
			}
		}
	}
}