#include "heap.h"

namespace heap
{
    HeapManager::HeapManager(void *ptr)
    {
    }

    void *HeapManager::GetStartPointer() const
    {
        return nullptr;
    }

    void HeapManager::SetStartPointer(void* ptr)
    {
        ptr_ = ptr;
    }

    void* HeapManager::Alloc(DWORD size)
    {
        // do something
        return nullptr;
    }

    void HeapManager::Free(void *ptr)
    {
        // do something
    }

}

