#include "doublylinkedlist.h"

namespace ds
{
    void Node::Delete()
    {
        heap::HeapHeader* prev_ptr = (heap::HeapHeader *)((size_t)ptr - ptr->prev);
        prev_ptr->next += ptr->next;

        heap::HeapHeader* next_ptr = (heap::HeapHeader *)((size_t)ptr + ptr->next);
        next_ptr->prev += ptr->prev;
        
    }

}
