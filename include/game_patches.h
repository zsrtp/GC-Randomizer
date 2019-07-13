#pragma once

#include "defines.h"

/**
 * Contains functions that fix certain cutscenes
 * those functions run only when the cs is skipped
 */
namespace mod::cutscene_skip
{
	/**
	 * Runs when master sword CS is skipped
	 */
	s32 onMasterSwordSkip(void* unk, s32 unk2);
}

namespace mod::game_patch
{
	/**
	 * Skips sewers immediately
	 * triggers the load to Ordon Spring
	 * and sets the flags accordingly
	 */
	void skipSewers();

	/**
	 * Sets the flag after Ilia CS
	 * which tells the game that you started
	 * with sewers
	 */
	void setFirstTimeWolf();

	/**
	 * Sets form to human
	 */
	void setHuman();

	/**
	 * Sets form to wolf
	 */
	void setWolf();


	/**
	 * Activates the sense button (X)
	 * for Wolf Link
	 */
	void giveSense();

	/**
	 * Tames Epona
	 */
	void giveEpona();

	/**
	 * Gives Master Sword and equips it
	 */
	void giveMasterSword();

	/**
	 * Gives Midna and sets the according sewers flag
	 * Makes her appear on Z
	 */
	void giveMidna();

	/**
	 * Gives Midna Text that allows transformation
	 */
	void giveMidnaTransform();
}