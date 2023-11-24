#ifndef SHAREDMEMORYIPC_HEAP_HEAP_H_
#define SHAREDMEMORYIPC_HEAP_HEAP_H_

#include "ulti/framework.h"

namespace heap
{
    class HeapManager
    {
    private:
        void* ptr_;
    public:
        
        HeapManager(void* ptr);

        void* GetStartPointer() const;
        void SetStartPointer(void* ptr);

        void* Alloc(DWORD size);
        void Free(void* ptr);
    
    private:

    }
}

#endif