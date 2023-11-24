#ifndef SHAREDMEMORYIPC_HEAP_DEFINE_H_
#define SHAREDMEMORYIPC_HEAP_DEFINE_H_

#include "ulti/framework.h"

namespace heap
{
    struct HeapHeader{
        DWORD fwd;
        DWORD back;
        DWORD send_id;
        DWORD recv_id;
        DWORD time_stamp;
        
    } HeapHeader;   

}

#endif