#ifndef SHAREDMEMORYIPC_DATASTRUCTURE_DOUBLYLINKEDLIST_H_
#define SHAREDMEMORYIPC_DATASTRUCTURE_DOUBLYLINKEDLIST_H_

#include "heap/define.h"

namespace ds
{
    struct Node
    {
        heap::HeapHeader* ptr;
        void Delete();
    };
}

#endif