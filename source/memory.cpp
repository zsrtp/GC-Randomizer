#include "memory.h"

#include <gc/OSCache.h>

#include <cstring>

#include "defines.h"

namespace mod::memory
{
    void* clearMemory( void* ptr, size_t size ) { return memset( ptr, 0, size ); }

    void clear_DC_IC_Cache( void* ptr, u32 size )
    {
        gc::os_cache::DCFlushRange( ptr, size );
        gc::os_cache::ICInvalidateRange( ptr, size );
    }
}     // namespace mod::memory