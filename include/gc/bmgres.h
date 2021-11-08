#pragma once

#include "defines.h"

namespace gc::bmgres
{
    struct FileHeader
    {
        u8 misc[0x100]; // Very start of the file; Should define at some point
    } __attribute__((__packed__));
    
    struct BMGHeader
    {
        char signature[4];
        char identifier[4];
        u32 dataSize;
        u32 numBlocks;
        u8 charset;
        u8 unk_11;
        u16 unk_12;
        s32 unk_14[2];
        s32 unk_1c;
    } __attribute__((__packed__));
    
    struct MessageEntry
    {
        u32 offsetToMessage;
        u16 messageId;
        u8 unk[0xE];
    } __attribute__((__packed__));
    
    struct TextIndexTable
    {
        char kind[4]; // Should be INF1 in ASCII
        u32 size;
        u16 numEntries;
        u16 entrySize;
        u16 groupId;
        u8 defaultColor;
        u8 unk;
        MessageEntry entry[]; // Amount of entries is numEntries
    } __attribute__((__packed__));
    
    static_assert(sizeof(FileHeader) == 0x100);
    static_assert(sizeof(BMGHeader) == 0x20);
    static_assert(sizeof(MessageEntry) == 0x14);
}
