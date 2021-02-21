#pragma once

#include "defines.h"

namespace tp::d_msg_flow
{
	extern "C"
	{
		bool query022(void* unk1, void* unk2, s32 unk3);
		// Used in MsgFlow "Can buy arrows?" checks
		bool query024(void* dMsgFlow_cPtr, void* mesg_flow_node_branchPtr, void* fopAc_ac_cPtr, int unused);
	}
}