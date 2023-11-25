#ifndef SHAREDMEMORYIPC_HEAP_DEFINE_H_
#define SHAREDMEMORYIPC_HEAP_DEFINE_H_

#include "ulti/framework.h"

namespace heap
{
    struct HeapHeader{
        DWORD prev_distance;
        DWORD next_distance;
        DWORD flag;
        DWORD reserve[1];
    };   

    #define IN_USE 0x1
}

#endif