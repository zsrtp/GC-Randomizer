#pragma once

#include "defines.h"

namespace gc::OSModule {

struct OSModuleInfo
{
	u32 id;
	OSModuleInfo* next;
	OSModuleInfo* prev;
	u32 numSections;
	u32 sectionInfoOffset;
	u32 nameOffset;
	u32 nameSize;
	u32 version;
} __attribute__((__packed__));

}