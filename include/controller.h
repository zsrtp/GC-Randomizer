#pragma once

#include "defines.h"

namespace mod::controller
{
	enum PadInputs : u32
	{
		Button_DPad_Left 	= 0x00000001,
		Button_DPad_Right 	= 0x00000002,
		Button_DPad_Down 	= 0x00000004,
		Button_DPad_Up 		= 0x00000008,
		Button_Z 			= 0x00000010,
		Button_R 			= 0x00000020,
		Button_L 			= 0x00000040,
		Button_A 			= 0x00000100,
		Button_B 			= 0x00000200,
		Button_X 			= 0x00000400,
		Button_Y 			= 0x00000800,
		Button_Start 		= 0x00001000,
		Stick_C_Left 		= 0x00010000,
		Stick_C_Right 		= 0x00020000,
		Stick_C_Down 		= 0x00040000,
		Stick_C_Up 			= 0x00080000,
		Stick_Left 			= 0x01000000,
		Stick_Right 		= 0x02000000,
		Stick_Down 			= 0x04000000,
		Stick_Up 			= 0x08000000
	};

	extern "C"
	{
		bool checkForButtonInput(u32 buttonCombo);
		bool checkForButtonInputSingleFrame(u32 buttonCombo);
	}
}