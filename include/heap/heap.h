#ifndef SHAREDMEMORYIPC_HEAP_HEAP_H_
#define SHAREDMEMORYIPC_HEAP_HEAP_H_

#include "heap/define.h"
#include "sync/mutex.h"

namespace heap
{
    class HeapManager
    {
    private:
        void* ptr_;
        DWORD size_;
        synchronization::NamedMutex heap_mutex_;
    public:
        HeapManager() = delete;
        HeapManager(void* ptr, DWORD size);

        void* Alloc(DWORD size);
        void Free(void* data_ptr);
    

        void* GetStartPointer() const;

        DWORD GetTotalHeapSize() const;

    protected:
        bool OutOfBound(void* header_ptr);
        void SetTotalHeapSize(DWORD size);
        void SetStartPointer(void* ptr);

    };
}

#endif