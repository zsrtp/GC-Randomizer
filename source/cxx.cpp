#include <tp/m_Do_ext.h>
#include <tp/JKRExpHeap.h>

#include <cstddef>

void* operator new(std::size_t size)
{
	void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
	return tp::JKRExpHeap::do_alloc_JKRExpHeap(ArchiveHeapPtr, size, 0x20);
}
void* operator new[](std::size_t size)
{
	void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
	return tp::JKRExpHeap::do_alloc_JKRExpHeap(ArchiveHeapPtr, size, 0x20);
}
void operator delete(void* ptr)
{
	void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
	return tp::JKRExpHeap::do_free_JKRExpHeap(ArchiveHeapPtr, ptr);
}
void operator delete[](void* ptr)
{
	void* ArchiveHeapPtr = tp::m_Do_ext::archiveHeap;
	return tp::JKRExpHeap::do_free_JKRExpHeap(ArchiveHeapPtr, ptr);
}