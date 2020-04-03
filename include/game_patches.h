#pragma once

#include "defines.h"

/**
 * Contains functions that fix certain cutscenes
 * those functions should only run when the CS
 * is currently playing and tried to be skipped by the user
 */
namespace mod::cutscene_skip
{
	/**
	 * Runs when master sword CS is skipped
	 */
	s32 onMasterSwordSkip(void* unk, s32 unk2);
}

/**
 * Contains patch functions that take no parameters
 * and immediately perfom the described action
 */
namespace mod::game_patch
{
	/**
	 * Enables the debug screen without
	 * active debug mode; It triggers
	 * automatically when crashing
	 */
	void assemblyOverwrites();

	/**
	 * Kills spider at links house
	 */
	void killLinkHouseSpider();

	/**
	 * Increasees Links climbing speed
	 */
	void increaseClimbSpeed();

	/**
	 * Removes the movementspeed limit
	 * when wearing IB
	 */
	void removeIBLimit();

	/**
	 * Changes the max rupee amounts for each
	 * of the wallets
	 */
	void increaseWalletSize();

	/**
	 * Skips sewers immediately
	 * triggers the load to Ordon Spring
	 * and sets the flags accordingly
	 */
	void skipSewers();
	
	/**
	 * Skips Midna's Desperate Hour
	 * triggers the load to zelda's tower
	 */
	void skipMDH();
	
	/**
	 * Removes midna from preventing you from leaving faron
	 * changes the state of faron from 0 to 2
	 */
	void allowFaronEscape();

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