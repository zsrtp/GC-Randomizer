#include <tp/JKRExpHeap.h>
#include <tp/m_Do_ext.h>

#include <cstring>

#include "defines.h"

void* operator new( size_t size )
{
    void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
    void* NewPtr = tp::jkr_exp_heap::do_alloc_JKRExpHeap( ArchiveHeapPtr, size, 0x20 );
    return memset( NewPtr, 0, size );
}
void* operator new[]( size_t size )
{
    void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
    void* NewPtr = tp::jkr_exp_heap::do_alloc_JKRExpHeap( ArchiveHeapPtr, size, 0x20 );
    return memset( NewPtr, 0, size );
}
void operator delete( void* ptr )
{
    void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
    return tp::jkr_exp_heap::do_free_JKRExpHeap( ArchiveHeapPtr, ptr );
}
void operator delete[]( void* ptr )
{
    void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
    return tp::jkr_exp_heap::do_free_JKRExpHeap( ArchiveHeapPtr, ptr );
}