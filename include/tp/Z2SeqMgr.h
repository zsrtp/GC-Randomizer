#pragma once

#include "defines.h"

namespace tp::z2seqmgr
{
	extern "C"
	{
		void bgmStart(void* Z2SeqMgr, u32 bgmId, u32 unk3, s32 unk4);
		void subBgmStart(void* Z2SeqMgr, u32 bgmId);
		void bgmStreamPrepare(void* Z2SeqMgr, u32 audioStreamingId);
	}
}