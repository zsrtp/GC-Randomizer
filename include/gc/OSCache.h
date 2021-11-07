#pragma once

#include "defines.h"

namespace gc::os_cache
{
    extern "C"
    {
        // DCEnable
        // DCInvalidateRange
        void DCFlushRange( void* startAddr, u32 nBytes );
        // DCStoreRange
        // DCFlushRangeNoSync
        // DCStoreRangeNoSync
        // DCZeroRange
        void ICInvalidateRange( void* startAddr, u32 nBytes );
        // ICFlashInvalidate
        // ICEnable
        // __LCEnable
        // LCEnable
        // LCDisable
        // LCStoreBlocks
        // LCStoreData
        // LCQueueWait
        // L2GlobalInvalidate
        // DMAErrorHandler
        // __OSCacheInit
    }
}     // namespace gc::os_cache