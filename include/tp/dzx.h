/**	@file dzx.h
 *	@brief dzx related definitions
 *
 *	@author AECX
 *	@bug No known bugs.
 */
#pragma once
#include <string.h>

#include "../defines.h"

namespace tp::d_stage
{
	/**
	 *  @brief Holds information about the given dzx Chunktype
	 *
	 *  Example:
	 *  TRES
	 *  0001
	 *  80401234 = 1 TRES Chunk at this address
	 */
	
	struct Actr
	{
		char objectName[8];
		u32 parameters;
		float pos[3];
		s16 rot[3];
		u16 enemyNum;
	} __attribute__((__packed__));
	static_assert(sizeof(Actr) == 0x20);

	struct dzxChunkTypeInfo
	{
		char tag[4];
		u32 numChunks;
		void* chunkDataPtr;
	} __attribute__((__packed__));

	struct TRES
	{
		char actorName[8];
		u32 flags;

		float X;
		float Y;
		float Z;

		s32 angle;

		u8 item;
		u8 unk2[3];  // Seems to always be 0xFF

		TRES() { memset(this, 0xFF, sizeof(TRES)); }
	} __attribute__((__packed__));
	static_assert(sizeof(TRES) == 0x20);

}  // namespace tp