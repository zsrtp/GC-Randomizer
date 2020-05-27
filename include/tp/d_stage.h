/**	@file d_stage.h
 *	@brief Stage related functions and fields
 *
 *	@author AECX
 *	@bug No known bugs.
 */

#pragma once

#include "../defines.h"
#include "dzx.h"

namespace tp::d_stage
{
    extern "C"
    {
        void* mStatus_roomControl;

        /**
         *  @brief Initialises Actors, can run multiple times per load
         *
         *  @param mStatus_roomControl Pointer to roomControl data (unknown)
         *  @param chunkTypeInfo Pointer to dzxHeader
         *  @param unk3 unknown
         *  @param unk4 unknown
         */
        void actorCommonLayerInit(void* mStatus_roomControl, dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4);
    }
}  // namespace tp::d_stage