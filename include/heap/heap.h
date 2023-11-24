#ifndef SHAREDMEMORYIPC_HEAP_HEAP_H_
#define SHAREDMEMORYIPC_HEAP_HEAP_H_

#include "ulti/framework.h"

namespace heap
{
    class HeapManager
    {
    private:
        UCHAR ptr_;

    public:
        
        void* Alloc(DWORD size);
        void Free(void* ptr);
    
    private:

    }
}

#endif