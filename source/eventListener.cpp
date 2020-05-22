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

	void EventListener::addLoadEvent(char* stage, u8 room, u8 spawn, u8 state, u8 eventid, EventFunction trigger, LoadEventAccuracy accuracy)
	{
		if(loadEventIndex < MAX_LOAD_EVENTS)
		{
			// Add the event to the array
			loadEvents[loadEventIndex] = {stage, room, spawn, state, eventid, trigger, accuracy};

			loadEventIndex++;
		}
	}

	void EventListener::checkLoadEvents()
	{
		tp::d_com_inf_game::NextStageVars* nextStageVars = &gameInfo.nextStageVars;
		
		
		char* stage = nextStageVars->nextStage;
		u8 room = nextStageVars->nextRoom;
		u8 spawn = static_cast<u8>(nextStageVars->nextSpawnPoint);
		u8 state = nextStageVars->nextState;
		u8 eventid = gameInfo.eventSystem.currentEventID;

		for(u8 i = 0; i < loadEventIndex; i++)
		{
			LoadEvent e = loadEvents[i];
			if(strcmp(stage, e.Stage) == 0)
			{
				// Stage is minimum requirement
				if(e.Accuracy == LoadEventAccuracy::Stage)
				{
					e.Trigger();
				}

				if(room == e.Room)
				{
					if(e.Accuracy == LoadEventAccuracy::Stage_Room)
					{
						e.Trigger();
					}

					if(spawn == e.Spawn)
					{
						if(e.Accuracy == LoadEventAccuracy::Stage_Room_Spawn)
						{
							e.Trigger();
						}

						if(state == e.State)
						{
							if(e.Accuracy == LoadEventAccuracy::Stage_Room_Spawn_State)
							{
								e.Trigger();
							}

							if(eventid == e.Eventid)
							{
								// No accuracy check since it's already ::All
								e.Trigger();
							}
						}
					}
				}
			}
		}
	}
}