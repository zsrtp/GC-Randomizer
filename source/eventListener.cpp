#include "eventListener.h"

#include <tp/JFWSystem.h>
#include <tp/d_com_inf_game.h>
#include <tp/d_kankyo.h>

#include <cstdio>
#include <cstring>

#include "defines.h"

namespace mod::event
{
    EventListener::EventListener() { sprintf( sysConsolePtr->consoleLine[20].line, "Event Listener Initialized." ); }

    void EventListener::addLoadEvent( char* stage,
                                      u8 room,
                                      u8 spawn,
                                      u8 state,
                                      u8 eventid,
                                      EventFunction trigger,
                                      LoadEventAccuracy accuracy )
    {
        if ( loadEventIndex < MAX_LOAD_EVENTS )
        {
            // Add the event to the array
            loadEvents[loadEventIndex] = { stage, room, spawn, state, eventid, trigger, accuracy };

            loadEventIndex++;
        }
    }

    void EventListener::checkLoadEvents()
    {
        char* stage = gameInfo.nextStageVars.nextStage;
        u8 room = gameInfo.nextStageVars.nextRoom;
        u8 spawn = static_cast<u8>( gameInfo.nextStageVars.nextSpawnPoint );
        u8 state = gameInfo.nextStageVars.nextState;
        u8 eventid = gameInfo.eventSystem.currentEventID;

        for ( u8 i = 0; i < loadEventIndex; i++ )
        {
            LoadEvent e = loadEvents[i];
            if ( strcmp( stage, e.Stage ) == 0 )
            {
                // Stage is minimum requirement
                if ( e.Accuracy == LoadEventAccuracy::Stage )
                {
                    e.Trigger();
                }

                if ( room == e.Room || e.Room == 0xFF )
                {
                    if ( e.Accuracy == LoadEventAccuracy::Stage_Room )
                    {
                        e.Trigger();
                    }

                    if ( spawn == e.Spawn || e.Spawn == 0xFF )
                    {
                        if ( e.Accuracy == LoadEventAccuracy::Stage_Room_Spawn )
                        {
                            e.Trigger();
                        }

                        if ( state == e.State || e.State == 0xFF )
                        {
                            if ( e.Accuracy == LoadEventAccuracy::Stage_Room_Spawn_State )
                            {
                                e.Trigger();
                            }

                            if ( eventid == e.Eventid )
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
}     // namespace mod::event