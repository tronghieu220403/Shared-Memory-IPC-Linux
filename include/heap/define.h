#ifndef SHAREDMEMORYIPC_HEAP_DEFINE_H_
#define SHAREDMEMORYIPC_HEAP_DEFINE_H_

#include "ulti/framework.h"

namespace heap
{
    struct ChunkHeader{
        DWORD prev_distance;
        DWORD next_distance;
        DWORD flag;
        DWORD reserve[1];
    };   

    #define IN_USE 0x1
    #define NOT_IN_USE 0x0
}

#endif