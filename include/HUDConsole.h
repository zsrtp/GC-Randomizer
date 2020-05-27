#pragma once

#include <tp/JFWSystem.h>

#include "defines.h"

namespace mod
{
	struct Option
	{
		char Title[16];
		u8* Target;
		u32 Limit;
	};

	struct Watch
	{
		char Title[16];
		void* Target;
		char Format;
		u8 Interpretation;
	};

	enum WatchInterpretation : u8
	{
		_u8 = 0,
		_u16 = 1,
		_u32 = 2,
		_u64 = 3,
		_s8 = 4,
		_s16 = 5,
		_s32 = 6,
		_s64 = 7,
		_str = 8
	};

	enum ConsoleActions : u8
	{
		Move_Up = 0,
		Move_Down = 1,
		Move_Left = 2,
		Move_Right = 3,
		Option_Increase = 4,
		Option_Decrease = 5
	};

	class HUDConsole
	{
		public:
			HUDConsole(const char firstPage[16], u32 RGBA = 0x000000FF);

			/**
			 * Adds an option to the Console
			 * 
			 * @param page Page index where the option should appear
			 * @param title Short description (limit 16)
			 * @param target The target value that can be set via the menu
			 * @param limit	When target >= limit it will reset to 0
			 */
			void addOption(u8 page, const char title[16], u8* target, u32 limit);

			/**
			 * Adds a watch to the console
			 * 
			 * @param page Page index where the watch should appear
			 * @param title Short description (limit 16)
			 * @param target Target value to be represented
			 * @param format Formatting char (s, d, u, x, ...)
			 * @param interpretation How to read from the target* (mod::WatchInterpretation)
			 */
			void addWatch(u8 page, const char title[16], void* target, char format, u8 interpretation);

			/**
			 * Adds a page to the Console (pages can be switched with R/L)
			 * 
			 * @param title The name to appear when this page is opened
			 * @returns The index of the new page if successful otherwise -1
			 */
			s8 addPage(const char title[16]);

			/**
			 * Performs an action
			 * 
			 * @param consoleAction A valid mod::ConsoleAction that's going to be performed
			 */
			void performAction(u8 consoleAction, u8 amount = 1);

			/**
			 * Draws the current page and it's options
			 */
			void draw();

		private:
			u8 selectedOption; // Y Position
			u8 selectedPage;  // X Position

			u8 numOptions[MAX_HUDCONSOLE_PAGES];
			u8 numWatches[MAX_HUDCONSOLE_PAGES];
			u8 numPages;

			char pages[MAX_HUDCONSOLE_PAGES][16];
			Option options[MAX_HUDCONSOLE_PAGES][10];
			Watch watches[MAX_HUDCONSOLE_PAGES][10];
	};
}