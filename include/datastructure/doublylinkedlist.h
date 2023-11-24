#ifndef SHAREDMEMORYIPC_DATASTRUCTURE_DOUBLYLINKEDLIST_H_
#define SHAREDMEMORYIPC_DATASTRUCTURE_DOUBLYLINKEDLIST_H_

#include "ulti/frameword.h"

namespace ds
{
    class Node
    {
    private:
        DWORD prev_distance_ = 0;
        DWORD next_distance_ = 0;
        void* ptr_;
    public:
        Node() = default;
        Node(void* ptr);
        
        void SetNodePointer(void* ptr);
        

        DWORD GetPrevDistance() const;
        void SetPrevDistance(DWORD prev_distance);

        DWORD GetNextDistance() const;
        void SetNextDistance(DWORD next_distance);

        void Delete();
    
    }
}

#endif