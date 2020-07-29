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
	struct Item
	{
		char objectName[8];
		u8 paramOne;
		u8 paramTwo;
		u8 membitFlag;
		u8 item;
		float pos[3];
		s16 rot[3];
		u16 enemyNum;
	} __attribute__((__packed__));

	static_assert(sizeof(Item) == 0x20);

	extern "C"
	{
		extern void* mStatus_roomControl;

		/**
		 *  @brief Initialises Actors, can run multiple times per load
		 *
		 *  @param mStatus_roomControl Pointer to roomControl data (unknown)
		 *  @param chunkTypeInfo Pointer to dzxHeader
		 *  @param unk3 unknown
		 *  @param unk4 unknown
		 */
		bool actorCommonLayerInit(void* mStatus_roomControl, dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4);
		bool actorInit(void* mStatus_roomControl, dzxChunkTypeInfo* chunkTypeInfo, int unk3, void* unk4);
	}
}  // namespace tp::d_stage