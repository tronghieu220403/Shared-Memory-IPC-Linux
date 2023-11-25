#ifndef SHAREDMEMORYIPC_HEAP_DEFINE_H_
#define SHAREDMEMORYIPC_HEAP_DEFINE_H_

#include "ulti/framework.h"

namespace heap
{
    struct HeapHeader{
        DWORD prev_distance;
        DWORD next_distance;
        DWORD send_id;
        DWORD recv_id;
        DWORD time_stamp;
        DWORD flag;
        DWORD reserve[2];
    };   

    #define IN_USE 0x1
}

#endif