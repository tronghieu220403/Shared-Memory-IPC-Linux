#include "heap.h"

namespace heap
{
    HeapManager::HeapManager(void *ptr, DWORD size):
        ptr_(ptr),
        size_(size),
        heap_mutex_("heap_sync")
    {
        heap_mutex_.Open();
    }

    void* HeapManager::Alloc(DWORD size)
    {
        if (size == 0)
        {
            return nullptr;
        }
        
        heap_mutex_.Lock();
        heap::HeapHeader* this_ptr = (heap::HeapHeader *)ptr_;
        DWORD true_size = (size+sizeof(heap::HeapHeader))%16 == 0 ? (size+sizeof(heap::HeapHeader)) : ((size+sizeof(heap::HeapHeader))/16+1);

        while(true)
        {
            if (OutOfBound((this_ptr)))
            {
                break;
            }

            if ( (this_ptr->flag & IN_USE) == 0 &&
                this_ptr->next_distance >= true_size)
            {

                // split the heap into 2 part

                heap::HeapHeader* second_half_ptr = (heap::HeapHeader*)((size_t)this_ptr + true_size);
                ulti::ZeroMemory(second_half_ptr, sizeof(heap::HeapHeader));
                second_half_ptr->prev_distance = true_size;
                second_half_ptr->next_distance = this_ptr->next_distance-true_size;

                heap::HeapHeader* first_half_ptr = this_ptr;
                ulti::ZeroMemory(first_half_ptr, sizeof(heap::HeapHeader));
                first_half_ptr->flag = IN_USE;
                first_half_ptr->next_distance = true_size;
                first_half_ptr->prev_distance = this_ptr->prev_distance;

                return (void *)((size_t)first_half_ptr + sizeof(heap::HeapHeader));
            }

            if (this_ptr->next_distance == 0)
            {
                return;
            }
            this_ptr = (heap::HeapHeader *)((size_t)this_ptr + this_ptr->next_distance);
        }
        heap_mutex_.Unlock();
        return nullptr;
    }

    void HeapManager::Free(void *ptr)
    {
        heap_mutex_.Lock();

        heap::HeapHeader* this_ptr;

        this_ptr = (heap::HeapHeader *)((size_t)ptr-sizeof(HeapHeader));

        if ( (this_ptr->flag & IN_USE) == 1)
        {
            this_ptr->flag = this_ptr->flag ^ IN_USE;
        }

        while(true)
        {
            if (this_ptr->next_distance == 0 || OutOfBound(this_ptr))
            {
                break;
            }
            heap::HeapHeader* next_ptr = (heap::HeapHeader *)((size_t)ptr + this_ptr->next_distance);
            if ( (next_ptr->flag & IN_USE) == 0 )
            {
                next_ptr->prev_distance += this_ptr->prev_distance;
            }
            else 
            {
                break;
            }
            this_ptr = next_ptr;
        }

        this_ptr = (heap::HeapHeader *)ptr;

        while(true)
        {
            if (this_ptr->prev_distance == 0 || OutOfBound(this_ptr))
            {
                break;
            }
            heap::HeapHeader* prev_ptr = (heap::HeapHeader *)((size_t)ptr - this_ptr->prev_distance);

            if ( (prev_ptr->flag & IN_USE) == 0 )
            {
                prev_ptr->next_distance += this_ptr->next_distance;
            }
            else 
            {
                break;
            }
            this_ptr = prev_ptr;
        }

        heap_mutex_.Unlock();
    }

    bool HeapManager::OutOfBound(void* ptr)
    {
        if (size_ <= sizeof(heap::HeapHeader) || (size_t)ptr < (size_t)ptr_ || size_t(ptr) > (size_t)(ptr_) + size_ - sizeof(heap::HeapHeader))
        {
            return true;
        }
        return false;
    }


    void *HeapManager::GetStartPointer() const
    {
        return nullptr;
    }

    void HeapManager::SetStartPointer(void* ptr)
    {
        ptr_ = ptr;
    }

    DWORD HeapManager::GetTotalHeapSize() const
    {
        return size_;
    }
    
    void HeapManager::SetTotalHeapSize(DWORD size)
    {
        size_ = size;
    }


}

