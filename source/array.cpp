#include "array.h"
#include "memory.h"
#include "mod.h"
#include "singleton.h"

#include <cstring>

namespace mod::array
{
	s32 indexOf(u16 needle, u16* haystack, size_t count)
	{
		for (u32 i = 0; i < count; i++)
		{
			if (haystack[i] == needle)
			{
				return static_cast<s32>(i);
			}
		}
		return -1;
	}

	u8 bgmIndexArray[0xAA];
	u8 audioStreamingIndexArray[0x81];

	u8 invalidBgmIndexArray[0x51] =
	{
		0x1,
		0x3,
		0x9,
		0xA,
		0xB,
		0x12,
		0x14,
		0x18,
		0x1C,
		0x21,
		0x24,
		0x2B,
		0x2D,
		0x33,
		0x34,
		0x3C,
		0x3D,
		0x3E,
		0x40,
		0x43,
		0x44,
		0x45,
		0x46,
		0x4A,
		0x4B,
		0x4D,
		0x4E,
		0x4F,
		0x50,
		0x51,
		0x52,
		0x53,
		0x54,
		0x56,
		0x57,
		0x5B,
		0x5C,
		0x5F,
		0x60,
		0x63,
		0x66,
		0x67,
		0x68,
		0x69,
		0x6A,
		0x6B,
		0x71,
		0x72,
		0x73,
		0x74,
		0x75,
		0x76,
		0x77,
		0x79,
		0x7B,
		0x7C,
		0x7D,
		0x7E,
		0x7F,
		0x80,
		0x81,
		0x82,
		0x83,
		0x84,
		0x89,
		0x8A,
		0x8D,
		0x8E,
		0x92,
		0x93,
		0x98,
		0x99,
		0x9C,
		0x9D,
		0xA0,
		0xA1,
		0xA2,
		0xA3,
		0xA4,
		0xA6,
		0xA7
	};

	u8 bgmFanfareArray[0x8] =
	{
		0xA,
		0xB,
		0x81,
		0x82,
		0x83,
		0x99,
		0xA0,
		0xA6
	};

	u32 getRandomBgmId(u32 originalId)
	{
		if (/*mod::Mod::randoEnabled == 0 || */Singleton::getInstance()->isCustomMusicEnabled == 0x0)
		{
			return originalId;
		}

		// Make sure the original id is valid
		if ((originalId < 0x1000000) || (originalId > 0x10000A9))
		{
			return originalId;
		}

		// Get the id out of the full value
		u32 tempId = originalId - 0x1000000;

		// Get the new id
		return bgmIndexArray[tempId] + 0x1000000;
	}

	u32 getRandomAudioStreamId(u32 originalId)
	{
		if (/*mod::Mod::randoEnabled == 0 || */Singleton::getInstance()->isCustomMusicEnabled == 0x0)
		{
			return originalId;
		}

		// Make sure the original id is valid
		if ((originalId < 0x2000000) || (originalId > 0x2000080))
		{
			return originalId;
		}

		// Get the id out of the full value
		u32 tempId = originalId - 0x2000000;

		// Get the new id
		return audioStreamingIndexArray[tempId] + 0x2000000;
	}

	bool checkIfBgmIdIsValid(u8 bgmId)
	{
		u32 invalidBgmIndexArraySize = sizeof(array::invalidBgmIndexArray) / sizeof(array::invalidBgmIndexArray[0]);
		for (u32 i = 0; i < invalidBgmIndexArraySize; i++)
		{
			if (bgmId == array::invalidBgmIndexArray[i])
			{
				return false;
			}
		}
		return true;
	}

	bool checkIfBgmIdIsFanfare(u8 bgmId)
	{
		u32 bgmFanfareArraySize = sizeof(array::bgmFanfareArray) / sizeof(array::bgmFanfareArray[0]);
		for (u32 i = 0; i < bgmFanfareArraySize; i++)
		{
			if (bgmId == array::bgmFanfareArray[i])
			{
				return true;
			}
		}
		return false;
	}
}