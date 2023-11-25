#ifndef SHAREDMEMORYIPC_HEAP_HEAP_H_
#define SHAREDMEMORYIPC_HEAP_HEAP_H_

#include "heap/define.h"

namespace heap
{
    class HeapManager
    {
    private:
        void* ptr_;
        DWORD size_;
    public:
        
        HeapManager(void* ptr, DWORD size);

        void* Alloc(DWORD size);
        void Free(void* ptr);
    
        bool OutOfBound(void* ptr);

        void* GetStartPointer() const;

        DWORD GetTotalHeapSize() const;

    protected:
        void SetTotalHeapSize(DWORD size);
        void SetStartPointer(void* ptr);

    };
}

#endif