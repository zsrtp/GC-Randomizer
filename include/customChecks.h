#pragma once
#include "defines.h"
#include "tools.h"

#include <tp/dzx.h>
#include <tp/d_com_inf_game.h>

struct customCheck
{
	char stage[8];
	u8 room;

	u8 chestType;

	u8 saveFlag;
	u8 itemID;

	u32 X;
	u32 Y;
	u32 Z;

	s32 Angle;

	void(*overrides)();

	bool(*requirement)();
};

namespace mod
{
	customCheck customChecks[37] = {
		/*Ordon Shield*/
		{"F_SP103", 0, 0, 0x74, 0x2A, 0x456C12A0, 0x43390000, 0x44405C5E, 0x5FA0, []() { gameInfo.localAreaNodes.unk_0[0x8] |= 0x4;/*remove ordon shield*/ }, []() { return (gameInfo.scratchPad.eventBits[0x5] & 0x7A) != 0;/*have sewers been done*/ } },
		/*Ordon Sword*/
		{"R_SP01", 4, 0, 0x70, 0x28, 0x439D0602, 0x0, 0xC26ABE99, 0xCC7D, []() { gameInfo.localAreaNodes.unk_0[0x8] |= 0x1;/*got ordon sword*/ gameInfo.localAreaNodes.unk_0[0x1B] |= 0x8;/*remove ordon sword*/ }, []() { return (gameInfo.scratchPad.eventBits[0x5] & 0x7A) != 0;/*have sewers been done*/ } },
		/*Fishing rod*/
	    {"F_SP103", 0, 1, 0x68, 0x4A, 0xC3FEB5F1, 0x42960000, 0x4514FB40, 0x883A, []() { gameInfo.scratchPad.eventBits[0x3] |= 0x5;/*brought Cradle to Uli and got fishing rod*/ gameInfo.scratchPad.eventBits[0x46] |= 0x1;/*took cradle from monkey*/ }, []() { return (gameInfo.localAreaNodes.unk_0[0xC] & 0x2) != 0;/*is goats 1 done*/ } },
	    /*Sera Bottle*/
		{"F_SP103", 0, 1, 0x7C, 0x65, 0x444C8DC3, 0x42AF0000, 0xC4CB2577, 0xA3E7, []() { gameInfo.scratchPad.eventBits[0x12] |= 0x8;/*can shop at Sera's shop*/ gameInfo.scratchPad.eventBits[0x14] |= 0x8;/*Sera Bottle gotten*/ }, []() { return (gameInfo.localAreaNodes.unk_0[0xC] & 0x2) != 0;/*is goats 1 done*/ } },
		/*Slingshot*/
		{"F_SP103", 0, 0, 0x78, 0x4B, 0xC3EDF8A9, 0x44CD922E, 0x45F31BF5, 0x7881, nullptr /*Flag is set in game_patches to avoid interaction with vanilla check*/, []() { return (gameInfo.localAreaNodes.unk_0[0xC] & 0x2) != 0;/*is goats 1 done*/ } },
		/*Lantern*/
		{"F_SP108", 0xFF, 1, 0xF8, 0x48, 0xC66D4C1B, 0x41C19A25, 0xC65D2696, 0x36EC, nullptr, []() { return true; } },
		/*Zora Armor*/
		{"F_SP111", 0, 1, 0x7C, 0x31, 0x46A85A96, 0x43FA0000, 0x43944190, 0xC270, []() { gameInfo.scratchPad.eventBits[0x8] |= 0x4;/*got zora armor from Rutela*/ }, []() { return true; } },
		/*Coral Earring*/
		{"F_SP111", 0, 1, 0x78, 0x3D, 0x46A81087, 0x43FA57D9, 0xC3BCEFAC, 0xC270, []() { gameInfo.scratchPad.eventBits[0x3B] |= 0x80;/*Got Coral Earring from Ralis*/ }, []() { return tools::checkItemFlag(ItemFlags::Asheis_Sketch); } },
		/*Auru's Memo*/
		{"F_SP115", 0, 1, 0x7C, 0x90, 0xC7E2E398, 0xC6770800, 0x47656746, 0x143D, []() { gameInfo.scratchPad.eventBits[0x25] |= 0x30;/*Auru Cutscene Complete + Auru's Memo gotten*/ }, []() { return gameInfo.scratchPad.allAreaNodes.Lakebed_Temple.dungeon.bossBeaten == 0b1; } },
		/*Ashei's Sketch*/
		{"F_SP114", 0, 1, 0x7C, 0x91, 0x46F71891, 0xC6502A32, 0xC6348FA6, 0x86E6, []() { gameInfo.scratchPad.eventBits[0x29] |= 0x40;/*Got Ashei's Sketch from Ashei*/ }, []() { return true; } },
		/*Renardo's Letter*/
		{"R_SP109", 0, 1, 0x68, 0x80, 0x43D3A702, 0x0, 0x43D5A60B, 0xC1FD, []() { gameInfo.scratchPad.eventBits[0xF] |= 0x80;/*Got Renardo's Letter from Renardo*/ }, []() { return gameInfo.scratchPad.allAreaNodes.Temple_of_Time.dungeon.bossBeaten == 0b1; } },
		/*Invoice*/
		{"R_SP116", 5, 1, 0x70, 0x81, 0x45282E22, 0xC48FC000, 0x453BED7D, 0x0000, []() { gameInfo.scratchPad.eventBits[0x21] |= 0x80;/*Got Invoice from Telma*/ }, []() { return tools::checkItemFlag(ItemFlags::Renardos_Letter); } },
		/*Wooden Statue*/
		{"F_SP122", 16, 1, 0x68, 0x82, 0xC7493734, 0xC5C3E9D7, 0x46F956C6, 0x7FE1, []() { gameInfo.scratchPad.eventBits[0x22] |= 0x80;/*Got Wooden Statue from wolves*/ }, []() { return tools::checkItemFlag(ItemFlags::Medicine_Scent); } },
		/*Ilia's Charm*/
		{"F_SP128", 0, 1, 0x64, 0x83, 0x44F7650C, 0x42D20705, 0xC4A1A68B, 0x4226, nullptr /*Flag is set in game_patches to avoid interaction with vanilla check*/, []() { return true; } },
		/*Horse Call*/
		{"R_SP109", 0, 1, 0x74, 0x84, 0x43CDBCA1, 0x0, 0xC31EEBF3, 0xBDBE, []() { gameInfo.scratchPad.eventBits[0x23] |= 0x20;/*Got horse call from Illia*/ }, []() { return tools::checkItemFlag(ItemFlags::Ilias_Charm); } },
		/*Fishing Hole Bottle*/
		{"F_SP127", 0, 1, 0x7C, 0x60, 0x45B27147, 0x420C0000, 0x450F716A, 0x62E5, []() { gameInfo.scratchPad.eventBits[0x39] |= 0x8;/*Got fishing hole bottle*/ }, []() { return true; } },
		/*Coro Key*/
		 {"F_SP108", 0xFF, 0, 0xF4, 0xFE, 0xC64BA600, 0x403DA884, 0xC663BC8E, 0x7A11, [](){ gameInfo.scratchPad.eventBits[0x1A] |= 0x10;/*Talked to Coro after Faron Twilight*/ gameInfo.localAreaNodes.unk_0[0xD] |= 0x4;/*got Coro key*/ }, [](){ return gameInfo.scratchPad.clearedTwilights.Faron == 0b1; } },
		/*Gate Keys*/
		 {"F_SP121", 0xFF, 1, 0x7C, 0xF3, 0xC781DFDB, 0xC607B38E, 0x47897B50, 0xC0C9, nullptr, []() { return (gameInfo.scratchPad.eventBits[0x8] & 0x40) != 0;/*was escort started*/ } },
		/*Camp Key*/
		{"F_SP118", 1, 0, 0x7C, 0x20, 0x457F816B, 0x43820000, 0xC572F680, 0x0000, []() { gameInfo.localAreaNodes.unk_0[0x4] |= 0x80;/*get camp key*/ }, []() { return true; } },
		/*Jovani Poe*/
		{"R_SP160", 5, 0, 0x7C, 0xE0, 0x45906531, 0xC2960000, 0x45229AEB, 0xC3C9, []() { gameInfo.localAreaNodes.unk_0[0x8] |= 0x80;/*killed poe*/ gameInfo.localAreaNodes.unk_0[0xF] |= 0x7;/*cs + open path to sewers*/ gameInfo.localAreaNodes.unk_0[0x17] |= 0x8; /*Gengle Free*/}, []() { return true; } },
		/*Shadow Crystal*/
		{"F_SP117", 1, 0, 0x7C, 0x32, 0xC36EB7DC, 0x44CB2000, 0xC5964574, 0x0000, []() { gameInfo.scratchPad.eventBits[0x10] |= 0x20;/*got master sword cs*/ }, []() { return true; } },
		/*Master Sword*/
		{"F_SP117", 1, 2, 0x78, 0x29, 0x4372ACFB, 0x44CB2000, 0xC5991A55, 0x0000, []() { gameInfo.scratchPad.eventBits[0x10] |= 0x20;/*got master sword cs*/ }, []() { return true; } },
		/*Powered Dominion Rod*/
		{"R_SP209", 7, 1, 0x70, 0x4C, 0xC3DB30E9, 0xC4408000, 0xC523C471, 0x3CF0, nullptr, []() { return tools::checkItemFlag(ItemFlags::Ancient_Sky_Book_empty); } },
		/*Light Master Sword*/
		{"F_SP125", 4, 1, 0x74, 0x49, 0x44E0DBF7, 0x45898B09, 0xC6A4AAFA, 0x7DBC, nullptr, []() { return ((gameInfo.scratchPad.eventBits[0x26] & 0x40) != 0) && (gameInfo.scratchPad.eventBits[0x26] & 0x20) != 0;/*both sols outside of Palace*/ } },
		/*Ending Blow*/
		{"F_SP108", 6, 2, 0xF0, 0xE1, 0xC71A5B41, 0x44898000, 0xC6E08544, 0x0000, nullptr, []() { return gameInfo.scratchPad.clearedTwilights.Faron == 0b1; } },
		/*Shield Bash*/
		 {"F_SP104", 1, 2, 0x6C, 0xE2, 0xC4BB1F1C, 0x438C0000, 0xC613CAAA, 0x4138, []() { gameInfo.scratchPad.eventBits[0x3C] |= 0x8;/*Got skill from Ordon Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x80) != 0;/*DMT howling stone done*/ } },
		/*Back Slice*/
		{"F_SP122", 8, 2, 0x78, 0xE3, 0xC78590C8, 0xC4834000, 0x45B7BC37, 0xC15B, []() { gameInfo.scratchPad.eventBits[0x3C] |= 0x4;/*Got skill from West CT Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x40) != 0;/*UZR howling stone done*/ } },
		/*Helm Splitter*/
		{"F_SP122", 16, 2, 0x74, 0xE4, 0xC75990D6, 0xC5BEA000, 0x46C3B269, 0x0000, []() { gameInfo.scratchPad.eventBits[0x3C] |= 0x2;/*Got skill from South CT Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x20) != 0;/*Faron howling stone done*/ } },
		/*Mortal Draw*/
		{"F_SP124", 0, 2, 0x78, 0xE5, 0xC53E0B78, 0x41C428F6, 0x46318C93, 0x4565, []() { gameInfo.scratchPad.eventBits[0x3C] |= 0x1;/*Got skill from Bublin Camp Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x10) != 0;/*Lake Hylia howling stone done*/ } },
		/*Jump Strike*/
		{"F_SP111", 0, 2, 0x6C, 0xE6, 0x467D1395, 0x44034000, 0xC1F855A7, 0x3EE7, []() { gameInfo.scratchPad.eventBits[0x3D] |= 0x80;/*Got skill from Graveyard Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x8) != 0;/*Snowpeak howling stone done*/ } },
		/*Great Spin*/
		{"F_SP116", 1, 2, 0x78, 0xE7, 0x42204113, 0x44480000, 0xC61FB41E, 0x0000, []() { gameInfo.scratchPad.eventBits[0x3D] |= 0x40;/*Got skill from Barrier Wolf*/ }, []() { return (gameInfo.scratchPad.eventBits[0x3A] & 0x4) != 0;/*Hidden Village howling stone done*/ } },
		/*Youth's Scent*/
		 {"F_SP121", 0xFF, 0, 0x70, 0xB4, 0xC681A76B, 0xC5C99000, 0x4783ED29, 0xA6A9, []() { gameInfo.scratchPad.eventBits[0x22] |= 0x40;/*Got Youth's scent*/ gameInfo.localAreaNodes.unk_0[0x16] |= 0x4;/*Midna text after getting Youth's scent*/ gameInfo.localAreaNodes.unk_0[0x17] |= 0x40;/*got youth's scent cs*/ }, []() { return true; } },
		/*Ilia's Scent*/
		{"F_SP121", 0xFF, 0, 0x6C, 0xB0, 0xC5DD1EF2, 0x4529ED9A, 0xC73F5AD1, 0x71A6, []() { gameInfo.scratchPad.eventBits[0x22] |= 0x20;/*Got Ilia's scent*/ gameInfo.localAreaNodes.unk_0[0x16] |= 0x20;/*Midna text after getting Ilia's scent*/ gameInfo.localAreaNodes.unk_0[0x17] |= 0x80;/*got Ilia's scent cs*/ }, []() { return true; } },
		/*Poe Scent*/
		{"D_MN10", 0xFF, 0, 0x3C, 0xB2, 0xC1571A39, 0xC1200000, 0x4355109A, 0x0000, nullptr, []() { return true; } },
		/*Reekfish Scent*/
		{"F_SP113", 1, 0, 0x64, 0xB3, 0x456A7496, 0xC5BB109B, 0x464D4470, 0xD6E8, nullptr, []() { return tools::checkItemFlag(ItemFlags::Coral_Earring); } },
		/*Medicine Scent*/
		{"R_SP160", 2, 0, 0x74, 0xB5, 0x45EC0BF6, 0xC33E0000, 0xC4D9EA4C, 0xC26D, nullptr, []() { return true; } },
		/*Snowpeak Map*/
		 {"D_MN11", 0xFF, 0, 0x74, 0x23, 0x447AF8E0, 0x00000000, 0xC42C93A9, 0xFEED, []() { gameInfo.localAreaNodes.unk_0[0xA] |= 0x10;/*got map from Yeta*/ gameInfo.scratchPad.eventBits[0xB] |= 0x20;/*Talked to Yeta in SPR first time*/ gameInfo.localAreaNodes.unk_0[0x10] |= 0x20;/*Yeta lets you open kitchen door*/ gameInfo.localAreaNodes.unk_0[0x16] |= 0x40;/*Yeta points to kitchen cs*/ }, []() { return true; } } };
};