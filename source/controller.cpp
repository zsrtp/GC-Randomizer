#include "controller.h"
#include "defines.h"

#include <tp/m_Do_controller_pad.h>

namespace mod::controller
{
	bool checkForButtonInput(u32 buttonCombo)
	{
		return (tp::m_do_controller_pad::cpadInfo.buttonInput & buttonCombo) == buttonCombo;
	}

	bool checkForButtonInputSingleFrame(u32 buttonCombo)
	{
		tp::m_do_controller_pad::CPadInfo* PadInfo = &tp::m_do_controller_pad::cpadInfo;
		if ((PadInfo->buttonInput & buttonCombo) == buttonCombo)
		{
			if (PadInfo->buttonInputTrg & buttonCombo)
			{
				return true;
			}
		}
		return false;
	}
}