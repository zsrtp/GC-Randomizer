#pragma once

#include "defines.h"

namespace mod::event
{
	typedef void (*eventFunction)();

	enum LoadEventAccuracy : u8
	{
		All = 0,
		Stage = 1,
		Stage_Room = 2,
		Stage_Room_Spawn = 3
	};

	struct LoadEvent
	{
		char* Stage;
		u8 Room;
		u8 Spawn;
		u8 State;
		eventFunction Trigger;
		LoadEventAccuracy Accuracy;
	};

	class EventListener
	{
		private:
			u8 loadEventIndex;
			LoadEvent loadEvents[MAX_LOAD_EVENTS];

		public:
			
			EventListener();

			/**
			 * Runs a loop that triggers the according event if it's conditions match
			 */
			void checkLoadEvents();

			/**
			 * Executes a function when certain load conditions match
			 * 
			 * @param stage The stage identifier (internal id)
			 * @param room The room identifier (internal room id)
			 * @param spawn The spawnpoint identifier (internal spawn id)
			 * @param state The state (internal state id)
			 * @param trigger The (void) function(void) to be executed when the event occurs
			 * @param accuracy Defines which of the identifiers have to match for the event to trigger
			 */
			void addLoadEvent(char* stage, u8 room, u8 spawn, u8 state, eventFunction trigger, LoadEventAccuracy accuracy = LoadEventAccuracy::All);
	};
}